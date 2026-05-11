#include "exti.h"
#include "delay.h"
#include "usart.h"	  

u16 water_time = 0;		//水流时间
extern int flow_rate;		//流量

//外部中断初始化函数
//PA15为中断输入.
void EXTIX_Init(unsigned int PreemptionPriority,unsigned int SubPriority)		//PreemptionPriority:抢占优先级设置;SubPriority:响应优先级设置
{

 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//外部中断，需要使能AFIO时钟
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//使能PORTA时钟

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//关闭jtag，使能SWD，可以用SWD模式调试
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;		//PA15
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//设置成下拉输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化GPIOA15

    //GPIOA.15	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发 
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//使能传感器所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;		//抢占优先级设置
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		//响应优先级设置
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}


//外部中断10~15服务程序
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1)
		{
			water_time++;
			if(water_time >=65535)
			{
				water_time = 0;
			}
			//printf("times:%d\r\n",times);
		}
	}
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}


//速度计算函数
void Velocity_Calculation(u16 i)
{
	//流量=(脉冲数/300)*120(单位:L/min)
	  //flow_rate = (int)i*0.4;
		flow_rate = (int)i/300;
	
    //printf("当前流量为：");
    //printf("%d",flow_rate);
    //printf("L/min\r\n");   
}

