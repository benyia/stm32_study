//#include <stdio.h> 
//#include <stdarg.h> 
//#include <string.h> 
//#include <stdlib.h>
// 
//#include "lcd1602.h"
//#include "stdlib.h" 
//#include "delay.h"
// 
// 
//void WriteData(u8 data)
//{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_7,(BitAction)((data & 0x80) >> 7 )); 		//D7
//	GPIO_WriteBit(GPIOA, GPIO_Pin_6,(BitAction)((data & 0x40) >> 6 )); 
//	GPIO_WriteBit(GPIOA, GPIO_Pin_5,(BitAction)((data & 0x20) >> 5 )); 
//	GPIO_WriteBit(GPIOA, GPIO_Pin_4,(BitAction)((data & 0x10) >> 4 )); 
//	GPIO_WriteBit(GPIOA, GPIO_Pin_3,(BitAction)((data & 0x08) >> 3 )); 
//	GPIO_WriteBit(GPIOA, GPIO_Pin_2,(BitAction)((data & 0x04) >> 2 )); 
//	GPIO_WriteBit(GPIOA, GPIO_Pin_1,(BitAction)((data & 0x02) >> 1 )); 
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0,(BitAction)(data & 0x01));              //D0
//}
// 
//void LCD1602_GPIO_Configuration(void)
//{
//	GPIO_InitTypeDef	GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ,ENABLE);//使能PB,PE端口时钟
// 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//    
// 
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//}
// 
///* 等待液晶准备好 */
//void LCD1602_Wait_Ready(void)
//{
//	u16 sta;
//	
//	WriteData(0xff);
//	LCD_RS_Clr();
//	LCD_RW_Set();
//	do
//	{
//		LCD_EN_Set();
//		Delay_ms(5);	//延时5ms，非常重要
// 		sta = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);//读取状态字
//		LCD_EN_Clr();
//	}while(sta & 0x80);//bit15等于1表示液晶正忙，重复检测直到其等于0为止
//}
// 
///* 向LCD1602液晶写入一字节命令，cmd-待写入命令值 */
//void LCD1602_Write_Cmd(u8 cmd)
//{
//	LCD_RS_Clr();
//	LCD_RW_Clr();
//	LCD_EN_Clr();
//	WriteData(cmd);
//	LCD_EN_Set();
//	Delay_ms(5);
//	LCD_EN_Clr();
//}
// 
///* 向LCD1602液晶写入一字节数据，dat-待写入数据值 */
//void LCD1602_Write_Dat(u8 dat)
//{
//	LCD_RS_Set();
//	LCD_RW_Clr();
//	LCD_EN_Clr();
//	WriteData(dat);
//	LCD_EN_Set();
//	Delay_ms(5);
//	LCD_EN_Clr();
//}
// 
///* 清屏 */
//void LCD1602_ClearScreen(void)
//{
//	LCD1602_Write_Cmd(0x01);
//}
// 
///* 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
//void LCD1602_Set_Cursor(u8 x, u8 y)
//{
//	u8 addr;
//	
//	if (y == 0)
//		addr = 0x00 + x;
//	else
//		addr = 0x40 + x;
//	LCD1602_Write_Cmd(addr | 0x80);
//}
// 
///* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
//void LCD1602_Show_Str(u8 x, u8 y, u8 *str)
//{
//	LCD1602_Set_Cursor(x, y);
//	while(*str != '\0')
//	{
//		LCD1602_Write_Dat(*str++);
//	}
//}
// 
///* 初始化1602液晶 */
//void LCD1602_Init(void)
//{
//	LCD1602_Write_Cmd(0x38);	//16*2显示，5*7点阵，8位数据口
//	Delay_ms(5);
//	LCD1602_Write_Cmd(0x0c);	//开显示，光标关闭
//	Delay_ms(5);
//	LCD1602_Write_Cmd(0x06);	//文字不动，地址自动+1
//	Delay_ms(5);
//	LCD1602_Write_Cmd(0x01);	//清屏
//	Delay_ms(5);
//}

