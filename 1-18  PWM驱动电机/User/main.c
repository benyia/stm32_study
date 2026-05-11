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
#include "Motor.h"

uint16_t Num;
int main(void)
{
	OLED_Init();
	Timer_Init();
	Motor_init();
	
	while(1)
	{
		PWM_SetCompare3(100);
	}
}

