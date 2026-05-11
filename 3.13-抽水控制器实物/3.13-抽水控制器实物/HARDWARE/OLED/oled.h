#ifndef _OLED_H
#define _OLED_H
#include "sys.h"

#define OLED_IIC_SCL PBout(6)		//串行时钟
#define OLED_IIC_SDA PBout(7)		//串行数据
 
#define high 1
#define low 0

#define	Brightness	0xCF		//用于设置对比度(0x00~0xff)
#define X_WIDTH 	128
#define Y_WIDTH 	64
void OLED_GPIO_Init(void);		//IO口设置函数
void IIC_Start(void);		//开启I2C总线
void IIC_Stop(void);		//关闭I2C总线
void Write_IIC_Byte(unsigned char IIC_Byte);		//通过I2C总线写一个byte的数据
void OLED_WrDat(unsigned char dat);		//向OLED屏写数据
void OLED_WrCmd(unsigned char cmd);		//向OLED屏写命令
void OLED_Set_Pos(unsigned char x, unsigned char y);		//设置显示坐标
void OLED_Fill(unsigned char bmp_dat);		//全屏显示(显示BMP图片时才会用到此功能)
void OLED_CLS(void);		//复位/清屏
void OLED_OFF(void);		//OLED休眠
void OLED_ON(void);		//OLED从休眠中唤醒
void OLED_Init(void);		//OLED屏初始化程序，此函数应在操作屏幕之前最先调用
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);		//在指定位置显示一个字符
void OLED_ShowFontHZ(u8 x,u8 y,u8 pos,u8 size,u8 mode);		//显示codetab.h中的汉字
void OLED_Refresh_Gram(void);		//更新显存到LCD
void OLED_DrawPoint(u8 x,u8 y,u8 t);		//画点
void OLED_Fill_Area(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot) ;		//x1,y1,x2,y2 填充区域的对角坐标
u32 oled_pow(u8 m,u8 n);		//m^n函数
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);		//显示数字
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);		//显示字符串
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);		//显示BMP图片128×64
#endif
