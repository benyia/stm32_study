#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

   	  
/*
引脚连接
    SCL  时钟线  - PB12
    SDA  数据线  - PB13
*/


//IO方向设置
#define SDA_IN_1()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=8<<20;}
#define SDA_OUT_1() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=3<<20;}


//IO操作函数	 
#define IIC_SCL_1    PBout(12) //SCL
#define IIC_SDA_1    PBout(13) //SDA	 
#define READ_SDA_1   PBin(13)  //输入SDA 

//IIC所有操作函数
void IIC_Init_1(void);                //初始化IIC的IO口				 
void IIC_Start_1(void);				//发送IIC开始信号
void IIC_Stop_1(void);	  			//发送IIC停止信号
void IIC_Send_Byte_1(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte_1(u8 ack);//IIC读取一个字节
u8 IIC_Wait_Ack_1(void); 				//IIC等待ACK信号
void IIC_Ack_1(void);					//IIC发送ACK信号
void IIC_NAck_1(void);				//IIC不发送ACK信号

#endif
















