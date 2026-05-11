#include "HC05_UART3.h"
#include "delay.h"


#if EN_USART3_RX   //如果使能了接收
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//通信协议:
//包头(0xA5)+数据+校验位+包尾(0x5A)
//数据为一个char型和一个int整形,共五个字节,故整个数据包为八字节,校验位为数据字节之和的低八位

u16 USART3_RX_STA=0;		//接收状态标记变量  
uint8_t usart_value=0;		//接收一个字节数据的变量
uint8_t len=0;		//接收数据的数组当前下标
uint8_t Flag=0;		//接收到数据后Flag=1
static uint8_t f = 0;      //从(0xA5)开始,接收(0x5A)结束
unsigned char rdata[20];		//串口接收到数据的存放数组  


void USART3_IRQHandler(void)		//串口3中断服务程序
{
	if(USART3->SR&(1<<5))		//接收到数据
	{	 
		USART3_RX_STA=0;		//重新开始接收	
		usart_value=USART3->DR;		//接收一个字节数据的数据
		
	  if(usart_value==0xA5)		//判断起始位(0xA5)是否到来
		{
			f = 1;		//接收到(0xA5)
		}
		if(f == 1)		//(0xA5)后的数据存入rdata[len]数组
		{
			rdata[len]=usart_value;		//将接收的数据存入数组
			len++;
		}
	}
	if(len==REC_BUF_SIZE && usart_value == 0x5A)		//接收到包尾，结束此次接收
	{
		Flag=1;
		len=0;
		f = 0;
	}
	else if(len > REC_BUF_SIZE)		//接收到的长度大于数据包的长度，也结束本次
	{
		f = 0;
		len = 0;
	}
} 
#endif		


//初始化IO 串口3
void uart3_init(unsigned int bound,unsigned int PreemptionPriority,unsigned int SubPriority)		//bound:波特率设置;PreemptionPriority:抢占优先级设置;SubPriority:响应优先级设置
{  	 
	
	//GPIO端口设置
	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef usartInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//使能USART3，GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//PB10	TXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	gpioInitStruct.GPIO_Pin = GPIO_Pin_10;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct);
	
	//PB11	RXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	gpioInitStruct.GPIO_Pin = GPIO_Pin_11;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct);
	
  //USART 初始化设置
	usartInitStruct.USART_BaudRate = bound;		//串口波特率
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//接收和发送
	usartInitStruct.USART_Parity = USART_Parity_No;		//无奇偶校验位
	usartInitStruct.USART_StopBits = USART_StopBits_1;		//1位停止位
	usartInitStruct.USART_WordLength = USART_WordLength_8b;		//8位数据位
	USART_Init(USART3, &usartInitStruct);		//初始化串口3
	
	USART_Cmd(USART3, ENABLE);		//使能串口3
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		//开启串口3接受中断							//使能接收中断
	
  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;		//抢占优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		//响应优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);		//根据指定的参数初始化VIC寄存器	
	
}


/**************************************************************************
函数名:Data_Recv
作用:将中断接收到的数据包导出到INPUT结构体
返回值:INPUT类型的结构体
***************************************************************************/
INPUT Data_Recv()
{
	
	int a=0;		//能到四个字节
	uint8_t i;
	INPUT structure;
  
	//接收到一个char类型的数据
	structure.c=rdata[1];
	//接收到一个int类型的数据
	for(i=2;i<6;i++){ 
		a+=rdata[i]<<((5-i)*8);		//四个字节的十六进制数转化为整形
	}
	structure.num=a;
	a = 0;
	Flag=0;		//接收完成
	return structure;

}


/**************************************************************************
函数名:void Blue_Send(USART_TypeDef*pUSARTx,char send_char,u32 send_data)
作用:将单片机数据发给上位机
返回值:无
***************************************************************************/
void Blue_Send(USART_TypeDef*pUSARTx,char send_char,u32 send_data)
{
	
	u8 send_data_H1 = 0;		//int数据的高16位
	u8 send_data_H2 = 0;
	u8 send_data_L1 = 0;		//int数据的低16位
	u8 send_data_L2 = 0;
	u8 sum=0;		//校验位(数据字节之和的低八位)
	
	USART_SendData(pUSARTx,0xA5);		//发送包头
	delay_ms(1);

	//发送char型数据	
	USART_SendData(pUSARTx,send_char);
	delay_ms(1);
	
	//发送整形数据
	send_data_H1 = send_data>>24;
	send_data_H2 = send_data>>16;
	send_data_L1 = send_data>>8;
	send_data_L2 = send_data;
  USART_SendData(pUSARTx,send_data_H1);
	delay_ms(1);
	
  USART_SendData(pUSARTx,send_data_H2);
	delay_ms(1);
	
	USART_SendData(pUSARTx,send_data_L1);
	delay_ms(1);
	
	USART_SendData(pUSARTx,send_data_L2);
	delay_ms(1);
	
	sum+=send_data;		//校验位为数据每个字节相加
	
	USART_SendData(pUSARTx,sum);		//发送校验位
	delay_ms(1);
	
	USART_SendData(pUSARTx,0x5A);		//包尾
	
}


/**************************************************************************
函数名:void Blue_Send_re(USART_TypeDef*pUSARTx,int flow_rate)
作用:将单片机数据发给上位机
返回值:无
***************************************************************************/
void Blue_Send_re(USART_TypeDef*pUSARTx,int flow_rate)
{
	
	USART_SendData(pUSARTx,0xA5);		//发送包头
	delay_ms(1);
	USART_SendData(pUSARTx,0x2C);		//发送","
	delay_ms(1);
	
	//发送flow_rate数据
	USART_SendData(pUSARTx,0x66);		//发送"f"
	delay_ms(1);
	USART_SendData(pUSARTx,0x6C);		//发送"l"
	delay_ms(1);
	USART_SendData(pUSARTx,0x6F);		//发送"o"
	delay_ms(1);
	USART_SendData(pUSARTx,0x77);		//发送"w"
	delay_ms(1);
	USART_SendData(pUSARTx,0x72);		//发送"r"
	delay_ms(1);
	USART_SendData(pUSARTx,0x61);		//发送"a"
	delay_ms(1);
	USART_SendData(pUSARTx,0x74);		//发送"t"
	delay_ms(1);
	USART_SendData(pUSARTx,0x65);		//发送"e"
	delay_ms(1);
	USART_SendData(pUSARTx,0x3A);		//发送":"
	delay_ms(1);
	
  USART_SendData(pUSARTx,Int_To_Hex(((int)flow_rate/100)));		//百位
	delay_ms(1);
	
  USART_SendData(pUSARTx,Int_To_Hex(((int)(flow_rate-100*(int)(flow_rate/100))/10)));		//十位
	delay_ms(1);
	
	USART_SendData(pUSARTx,Int_To_Hex(((int)flow_rate%10)));		//个位
	delay_ms(1);
	
	USART_SendData(pUSARTx,0x2C);		//发送","
	delay_ms(1);
	
	
	//USART_SendData(pUSARTx,sum);		//发送校验位
	//delay_ms(1);
	
	USART_SendData(pUSARTx,0x5A);		//包尾
	
}


//整形转十六进制
u8 Int_To_Hex(u8 integer)
{
	u8 c;
	c = integer + 0x30;
	return c;
}







