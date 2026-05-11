#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Serial.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
//	Serial_SendByte(0x41);
//	Serial_SendByte('A');
//	uint8_t MyArray[] = {0x41,0x42,0x43,0x44};
//	Serial_SendArray(MyArray,4);
//	Serial_SendString("Hello world!\n");
//	Serial_SendNumber(456789,6);
//	printf("Number=%d\n\r",789);
//	printf("\r\nNum2=%d", 222);
	Serial_Printf("你好");
	while(1)
	{
		
		
	}
}
