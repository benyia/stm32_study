#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Serial.h"
#include "OLED.h"
uint8_t RxData;
int main(void)
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1,1,"RxData:");
	LED_Init();
	while(1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			LED1_ON();
			RxData = Serial_GetRxData();
//			Serial_SendByte(RxData);
			OLED_ShowHexNum(2,1,RxData,2);
		}
		else
		{
			LED1_OFF();
		}
		
	}
}

//RxData = Serial_GetRxFlag(); 
//			Serial_SendByte(RxData);
//			OLED_ShowHexNum(1,6,RxData,2);
