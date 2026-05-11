#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
#include "Sensor.h"
#include "Encoder.h"
#include "LCD1602.h"
#include "Timer.h"
#include "Motor.h"

uint16_t i;
uint16_t Num=0;
int main(void)
{
	LED_Init();
	OLED_Init();
	PWM_Init();
	IC_Init();
	PWM_SetPrescaler(720-1);       //Freq=72M / (PSC+1) / 100
	PWM_SetCompare3(100);           //Duty=CCR/100
	while(1)
	{
		if(Key_GetNum()==2)
		{
			Num=Num+1;
			LED_ON();
			Delay_ms(500);
			LED_OFF();
			Delay_ms(500);
			OLED_ShowNum(2,6,Num,5);
		}
		else if(Key_GetNum()==1)
		{
			LED_ON();
			Num=Num-1;
			OLED_ShowNum(3,6,Num,5);
		}
//		OLED_ShowNum(1,6,IC_GetFreq(),5);
	}
}

