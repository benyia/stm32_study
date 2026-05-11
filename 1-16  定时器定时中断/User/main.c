#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"
#include "Servo.h"
#include "Sensor.h"
#include "Encoder.h"
#include "LCD1602.h"
#include "Timer.h"


uint16_t KeyNum;
uint16_t S=52;
uint16_t min=14;
uint16_t hour=13;
int main(void)
{
	OLED_Init();
	Timer_Init();
	while(1)
	{
		OLED_ShowString(1,3,"HelloWorld!");
		OLED_ShowString(2,1,"Clock:");
		if(S==60)
		{
			S=0;
			min++;
			if(min==60)
			{
				hour++;
				min=0;
				if(hour==24)
				{
					hour=0;
				}
			}
		}
		KeyNum=Key_GetNum();
		if(KeyNum==1)
		{
			S=min=hour=0;
		}
		OLED_ShowNum(2,7,hour,2);
		OLED_ShowString(2,9,":");
		OLED_ShowNum(2,11,min,2);
		OLED_ShowString(2,13,":");
		OLED_ShowNum(2,15,S,2);
//		OLED_ShowNum(3,7,TIM_GetCounter(TIM2),5);
	}
}

