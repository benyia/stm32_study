#ifndef __INA226_H
#define __INA226_H
#include "sys.h"

#define READ_ADDR                  0x81		//A1=GND，A2=GND // R=1, W=0
#define WRITE_ADDR                 0x80		//芯片地址

#define Config_Reg                 0x00
#define Shunt_V_Reg                0x01		//分流电压,分流电阻为0.1欧姆
#define Bus_V_Reg                  0x02		//总线电压
#define Power_Reg                  0x03		//电源功率
#define Current_Reg                0x04		//电流
#define Calib_Reg                  0x05		//校准，设定满量程范围以及电流功率测数
#define Mask_En_Reg                0x06		//屏蔽 使能
#define Alert_Reg                  0x07		//警报功能的阈值
#define Man_ID_Reg                 0xFE		//0x5449
#define ID_Reg                     0xFF		//唯一芯片识别号,为0x2260


u16 INA226_Read2Byte(u8 reg_addr);
u8 INA226_Write2Byte(u8 reg_addr,u16 reg_data);
void INA226_Init(void);

#endif

