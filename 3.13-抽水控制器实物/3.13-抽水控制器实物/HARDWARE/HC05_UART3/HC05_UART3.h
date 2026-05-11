#ifndef _HC05_UART3_H
#define _HC05_UART3_H
#include "HC05_UART3.h"
#include "sys.h"


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收
#define REC_BUF_SIZE 8    //接收数据包大小

	
typedef struct {
	int  num;
	char c;
}INPUT;		//INPUT类型的结构体


extern u8  USART3_RX_BUF[USART_REC_LEN];		//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;		//接收状态标记变量  
void uart3_init(unsigned int bound,unsigned int PreemptionPriority,unsigned int SubPriority);		//串口三 IO口初始化函数

INPUT Data_Recv(void);		//接收上位机的数据
void Blue_Send(USART_TypeDef*pUSARTx,char send_char,u32 send_data);		//发送数据给上位机
void Blue_Send_re(USART_TypeDef*pUSARTx,int flow_rate);		//将单片机数据发给上位机

u8 Int_To_Hex(u8 integer);		//整形转十六进制


#endif	   
















