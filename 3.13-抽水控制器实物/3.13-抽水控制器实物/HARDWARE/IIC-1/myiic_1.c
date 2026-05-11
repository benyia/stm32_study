#include "myiic_1.h"
#include "delay.h"
//软件IIC


/*
引脚连接
    SCL  时钟线  - PB12
    SDA  数据线  - PB13
*/


//初始化IIC
void IIC_Init_1(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	//先使能外设IO PORTB时钟 
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IIC_SCL_1=1;
	IIC_SDA_1=1;

}


//产生IIC起始信号
void IIC_Start_1(void)
{
	SDA_OUT_1();     //sda线输出
	IIC_SDA_1=1;	  	  
	IIC_SCL_1=1;
	delay_us(4);
 	IIC_SDA_1=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_1=0;//钳住I2C总线，准备发送或接收数据 
}	  


//产生IIC停止信号
void IIC_Stop_1(void)
{
	SDA_OUT_1();//sda线输出
	IIC_SCL_1=0;
	IIC_SDA_1=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_1=1; 
	IIC_SDA_1=1;//发送I2C总线结束信号
	delay_us(4);							   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack_1(void)
{
	u8 ucErrTime=0;
	SDA_IN_1();      //SDA设置为输入  
	IIC_SDA_1=1;
	delay_us(1);	   
	IIC_SCL_1=1;
	delay_us(1);	 
	while(READ_SDA_1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_1();
			return 1;
		}
	}
	IIC_SCL_1=0;//时钟输出0 	   
	return 0;  
} 


//产生ACK应答
void IIC_Ack_1(void)
{
	IIC_SCL_1=0;
	SDA_OUT_1();
	IIC_SDA_1=0;
	delay_us(2);
	IIC_SCL_1=1;
	delay_us(2);
	IIC_SCL_1=0;
}


//不产生ACK应答		    
void IIC_NAck_1(void)
{
	IIC_SCL_1=0;
	SDA_OUT_1();
	IIC_SDA_1=1;
	delay_us(2);
	IIC_SCL_1=1;
	delay_us(2);
	IIC_SCL_1=0;
}			


//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte_1(u8 txd)
{                        
  u8 t;   
	SDA_OUT_1(); 	    
	IIC_SCL_1=0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		IIC_SDA_1=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_1=1;
		delay_us(2); 
		IIC_SCL_1=0;	
		delay_us(2);
	}	 
} 	


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte_1(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN_1();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_1=0; 
        delay_us(2);
		IIC_SCL_1=1;
        receive<<=1;
        if(READ_SDA_1)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck_1();//发送nACK
    else
        IIC_Ack_1(); //发送ACK   
    return receive;
}


