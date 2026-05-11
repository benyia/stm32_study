#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"
#include "Servo.h"
#include "Sensor.h"


int main(void)
{
	OLED_Init();
	Sensor_Init();
	while(1)
	{
		OLED_ShowString(1,3,"HelloWorld!");
		OLED_ShowString(3,1,"Count:");
		OLED_ShowNum(3,7,CountSensor_Get(),5);
	}
}

