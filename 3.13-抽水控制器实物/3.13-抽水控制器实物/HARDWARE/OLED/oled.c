#include "oled.h"
#include "codetab.h"		//引用字库文件
#include "delay.h"


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];	


// ------------------------------------------------------------
// IO口模拟I2C通信(即软件IIC)
// SCL接PB6
// SDA接PB7
//函数原型:		void OLED_GPIO_Init(void)
//功　　能:		IO口设置函数
// ------------------------------------------------------------
void OLED_GPIO_Init(void)
{
	
	RCC->APB2ENR|=1<<3;		//先使能外设IO PORTB时钟 							 
	GPIOB->CRL&=0X00FFFFFF;		//选择PB6/7
	GPIOB->CRL|=0X77000000;		//设置PB6/7通用开漏输出模式
	GPIOB->ODR|=2<<7;		//设置PC6/7输出高
	
}


/**************************实现函数********************************************
*函数原型:		void IIC_Start(void)
*功　　能:		产生IIC起始信号
*******************************************************************************/
void OLED_IIC_Start(void)
{
	
	OLED_IIC_SDA=1;
	OLED_IIC_SCL=1;
	delay_us(1);
 	OLED_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	OLED_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	
}


/**************************实现函数********************************************
*函数原型:		void IIC_Stop(void)
*功　　能:	  产生IIC停止信号
*******************************************************************************/	  
void  OLED_IIC_Stop(void)
{
	
	OLED_IIC_SCL=0;
	OLED_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	OLED_IIC_SCL=1; 
	OLED_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(1);	
	
}


/**************************实现函数********************************************
*函数原型:		void Write_IIC_Byte(unsigned char IIC_Byte)
*功　　能:	  写入1byte信息
*******************************************************************************/	
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	
	unsigned char i;
  for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
			OLED_IIC_SDA=1;
		else
			OLED_IIC_SDA=0;
			OLED_IIC_SCL=1;
      delay_us(1);  //必须有保持SCL脉冲的延时
			OLED_IIC_SCL=0;
			IIC_Byte<<=1;
	}
			OLED_IIC_SDA = 1;	//原程序这里有一个拉高SDA，根据OLED的DATASHEET，此句必须去掉。
			OLED_IIC_SCL=1;
			delay_us(1);
			OLED_IIC_SCL=0;
	
}


/**************************实现函数********************************************
*函数原型:		void OLED_WrDat(unsigned char IIC_Data)
*功　　能:	  OLED写数据
*******************************************************************************/
void OLED_WrDat(unsigned char IIC_Data)
{
	
	OLED_IIC_Start();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40);			//write data
	Write_IIC_Byte(IIC_Data);
	OLED_IIC_Stop();
	
}


/**************************实现函数********************************************
*函数原型:		void OLED_WrCmd(unsigned char IIC_Command)
*功　　能:	  OLED写命令
*******************************************************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	
	OLED_IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	Write_IIC_Byte(0x00);			//write command
	Write_IIC_Byte(IIC_Command);
	OLED_IIC_Stop();
	
}


/**************************实现函数********************************************
*函数原型:		void OLED_Set_Pos(unsigned char x, unsigned char y) 
*功　　能:	  OLED 设置坐标
*******************************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
	
} 


/**************************实现函数********************************************
*函数原型:		void OLED_Fill(unsigned char bmp_dat) 
*功　　能:	  OLED全屏填充（可用于清屏）
*******************************************************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}


/**************************实现函数********************************************
*函数原型:		void OLED_CLS(void)
*功　　能:	  OLED复位
*******************************************************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}


/**************************实现函数********************************************
*函数原型:		void OLED_OFF(void)
*功　　能:	  让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
*******************************************************************************/
void OLED_OFF(void)
{
	OLED_WrCmd(0X8D);  //设置电荷泵
	OLED_WrCmd(0X10);  //关闭电荷泵
	OLED_WrCmd(0XAE);  //OLED休眠
}


/**************************实现函数********************************************
*函数原型:		void OLED_ON(void)
*功　　能:	  将OLED从休眠中唤醒
*******************************************************************************/
void OLED_ON(void)
{
	OLED_WrCmd(0X8D);  //设置电荷泵
	OLED_WrCmd(0X14);  //开启电荷泵
	OLED_WrCmd(0XAF);  //OLED唤醒
}


/**************************实现函数********************************************
*函数原型:		void OLED_Init(void)
*功　　能:	  OLED初始化
*******************************************************************************/
void OLED_Init(void)
{
	
	delay_ms(500);//初始化之前的延时很重要！

	OLED_WrCmd(0xAE); //关闭显示
	OLED_WrCmd(0xD5); //设置时钟分频因子,震荡频率
	OLED_WrCmd(80);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WrCmd(0xA8); //设置驱动路数
	OLED_WrCmd(0X3F); //默认0X3F(1/64) 
	OLED_WrCmd(0xD3); //设置显示偏移
	OLED_WrCmd(0X00); //默认为0

	OLED_WrCmd(0x40); //设置显示开始行 [5:0],行数.
													    
	OLED_WrCmd(0x8D); //电荷泵设置
	OLED_WrCmd(0x14); //bit2，开启/关闭
	OLED_WrCmd(0x20); //设置内存地址模式
	OLED_WrCmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WrCmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WrCmd(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WrCmd(0xDA); //设置COM硬件引脚配置
	OLED_WrCmd(0x12); //[5:4]配置
		 
	OLED_WrCmd(0x81); //对比度设置
	OLED_WrCmd(Brightness); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WrCmd(0xD9); //设置预充电周期
	OLED_WrCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WrCmd(0xDB); //设置VCOMH 电压倍率
	OLED_WrCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WrCmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WrCmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WrCmd(0xAF); //开启显示
	
	OLED_Fill(0x00); //初始清屏
	//OLED_Fill(0XFF);  //全屏亮
	//OLED_Fill(0X00);  //全屏灭
	OLED_Set_Pos(0,0);
	
} 


/**************************实现函数********************************************
*函数原型:		void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
*功　　能:	  在指定位置显示一个字符,包括部分字符 ; x:0~127 ; y:0~63 ; mode:0,反白显示;1,正常显示 ; size:选择字体 12/16/24
*******************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';		//得到偏移后的值		 
  for(t=0;t<csize;t++)
  {   
		if(size==12)temp=ascii_1206[chr][t];		//调用1206字体
		else if(size==16)temp=ascii_1608[chr][t];		//调用1608字体
		else if(size==24)temp=ascii_2412[chr][t];		//调用2412字体
		else return;		//没有的字库
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
  }          
}


/**************************实现函数********************************************
*函数原型:		void OLED_ShowFontHZ(u8 x,u8 y,u8 pos,u8 size,u8 mode)
*功　　能:	  显示codetab.h中的汉字 ; x,y:起点坐标 ; pos:数组位置汉字显示 ; size:字体大小 ; mode:0,反白显示;1,正常显示
*******************************************************************************/
void OLED_ShowFontHZ(u8 x,u8 y,u8 pos,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;  
	u8 csize=(size/8+((size%8)?1:0))*(size);		//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;		//不支持的size
	 
	for(t=0;t<csize;t++)
	{   												   
		if(size==12)temp=Hz_12[pos][t];		//调用1206字体
		else if(size==16)temp=FontHzk[pos][t];		//调用1608字体
		else if(size==24)temp=Hz_24[pos][t];		//调用2412字体
		else return;		//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  	
} 


/**************************实现函数********************************************
*函数原型:		void OLED_Refresh_Gram(void)
*功　　能:	  更新显存到LCD		 
*******************************************************************************/
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WrCmd (0xb0+i);		//设置页地址（0~7）
		OLED_WrCmd (0x00);		//设置显示位置—列低地址
		OLED_WrCmd (0x10);		//设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WrDat(OLED_GRAM[n][i]); 
	}   
}


/**************************实现函数********************************************
*函数原型:		void OLED_DrawPoint(u8 x,u8 y,u8 t)
*功　　能:	  画点;x:0~127,y:0~63,t:1 填充 0,清空
*******************************************************************************/		   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;		//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}


/**************************实现函数********************************************
*函数原型:		void OLED_Fill_Area(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
*功　　能:	  x1,y1,x2,y2 填充区域的对角坐标,确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63;dot:0,清空;1,填充
*******************************************************************************/	
void OLED_Fill_Area(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
		{ 	
			OLED_DrawPoint(x,y,dot);	
		}
	}													    
	OLED_Refresh_Gram();		//更新显示
}


/**************************实现函数********************************************
*函数原型:		u32 oled_pow(u8 m,u8 n)
*功　　能:	  m^n函数
*******************************************************************************/	
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


/**************************实现函数********************************************
*函数原型:		void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
*功　　能:	  显示数字; x,y :起点坐标; len :数字的位数; size:字体大小; num:数值(0~4294967295);
*******************************************************************************/	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 


/**************************实现函数********************************************
*函数原型:		void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
*功　　能:	  显示字符串; x,y:起点坐标; size:字体大小;*p:字符串起始地址 
*******************************************************************************/	 		
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2)))
				{
					x=0;
					y+=size;
				}
        if(y>(64-size))
				{
					y=x=0;
					OLED_Fill(0x00);
				}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
}


/**************************实现函数********************************************
*函数原型:		void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[])
*功　　能:	  显示BMP图片128×64; 起始点坐标(x,y),x的范围0～127，y为页的范围0～7
*******************************************************************************/	
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[])
{ 	
 	u16 j=0;
 	u8 x,y;
  
  if(y1%8==0)y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    	for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);	    	
	    }
	}
}


