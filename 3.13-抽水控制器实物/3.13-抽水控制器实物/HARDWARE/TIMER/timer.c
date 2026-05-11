#include "timer.h"
#include "led.h"	
#include "usart.h"
#include "brake.h"
#include "BUZZER.h"
#include "oled.h"


int time = 0;		//设定的计时时间
u16 time_cnt = 0;
u8 flag = 0;		//水泵开启标志


//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		
		if(time != 0 && flag == 1)		//计时不为0,且水泵开启
		{
			time_cnt++;		//计时		
			printf("%d\r\n",time_cnt);
			if(time >= time_cnt)
			{
				OLED_ShowChar(8*5,32,(int)(time - time_cnt)/10+'0',16,1);		//十位
				OLED_ShowChar(8*6,32,((int)(time - time_cnt)-((int)(time - time_cnt)/10)*10)+'0',16,1);		//各位
				OLED_Refresh_Gram();		//更新显示	
			}
			if(time_cnt == time)
			{
				BRAKE = 0;		//水泵关闭
				//水泵状态(关闭)
				OLED_ShowFontHZ(0,16,138,16,1);		//显示汉字(水)
				OLED_ShowFontHZ(16,16,139,16,1);		//显示汉字(泵)	
				OLED_ShowFontHZ(32,16,142,16,1);		//显示汉字(关)	
				OLED_ShowFontHZ(48,16,143,16,1);		//显示汉字(闭)		
				OLED_Refresh_Gram();		//更新显示	
				BUZZER = 1;		//蜂鸣器启动
			}
			else if(time_cnt == time+1)
			{
				flag = 0;		//水泵关闭		
				time_cnt = 0;		//计时清零
				time  = 0;		//计时清零
				BUZZER = 0;		//蜂鸣器关闭
			}
		}
		else
		{
			time_cnt = 0;		//计时清零
			OLED_ShowChar(8*5,32,(int)(time)/10+'0',16,1);		//十位
			OLED_ShowChar(8*6,32,((int)(time)-((int)(time)/10)*10)+'0',16,1);		//各位
			OLED_Refresh_Gram();		//更新显示	
		}
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
			
	}
}












