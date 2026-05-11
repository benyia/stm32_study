#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"
#include "Servo.h"

uint8_t KeyNum;
float Angle;

int main(void)
{
	OLED_Init();
	Servo_Init();
	PWM_SetCompare2(1200);
	OLED_ShowString(1,1,"Servo");
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum==1)
		{
			Angle += 30;
			if(Angle>180)
			{	
				Angle=0;
			}
			PWM_SetCompare2(Angle);
		}
		
	}
}
