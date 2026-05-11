#ifndef _DS3231_H_
#define _DS3231_H_
#include "sys.h"


typedef struct
{
	u8 sec;
	u8 min;
	u8 hour;
	u8 week;
	u8 day;
	u8 month;
	u32 year;
}_calendar_part;	//时间结构体

extern _calendar_part calendar;


typedef struct
{
	u8 hour;
	u8 min;
	u8 sec;
}_Alarmclock_part;//闹钟结构体

extern _Alarmclock_part Alarmclock;


/*  IIC_SDA时钟端口、引脚定义 */
#define DS3231_SDA_PORT 			GPIOB  
#define DS3231_SDA_PIN 			(GPIO_Pin_7)
#define DS3231_SDA_PORT_RCC		RCC_APB2Periph_GPIOB

#define DS3231_Write_Address  0xD0		//写地址
#define DS3231_Read_Address   0xD1		//读地址

#define EEPROM_Write_Address  0xAE		//写ee地址
#define EEPROM_Read_Address   0xAF		//读ee地址

#define DS3231_SEC_REG        0x00		//秒
#define DS3231_MIN_REG        0x01  	//分
#define DS3231_HOUR_REG       0x02		//时
#define DS3231_WDAY_REG       0x03		//星期
#define DS3231_MDAY_REG       0x04		//天
#define DS3231_MONTH_REG      0x05		//月
#define DS3231_YEAR_REG       0x06		//年
/***闹钟1***/
#define DS3231_AL1SEC_REG     0x07		//秒
#define DS3231_AL1MIN_REG     0x08		//分
#define DS3231_AL1HOUR_REG    0x09 		//时
#define DS3231_AL1WDAY_REG    0x0A		//天/星期
/***闹钟2***/
#define DS3231_AL2MIN_REG     0x0B		//分
#define DS3231_AL2HOUR_REG    0x0C		//时
#define DS3231_AL2WDAY_REG    0x0D		//天/星期

#define DS3231_CONTROL_REG          0x0E		//控制
#define DS3231_STATUS_REG           0x0F		//状态
#define DS3231_AGING_OFFSET_REG     0x10		//老化补偿寄存器
#define DS3231_TMP_UP_REG           0x11		//温度寄存器高8字节
#define DS3231_TMP_LOW_REG          0x12		//温度低2字节

void DS3231_Init(void);		//初始化
void DS3231_IIC_Write(u8 addr,u8 data);		//写寄存器
u8 DS3231_IIC_Read(u8 addr);		//读寄存器
void ModifyTime(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec);		//修改时间日历
void get_time(void);		//获取时间
void SetAlarmclock_1(u8 ahour,u8 amin,u8 asec);		//设置闹钟1
void SetAlarmclock_2(u8 ahour,u8 amin);		//设置闹钟2
void Alarmclock1_cmd(u8 mode);		//使能闹钟1 mode = 1 / 0
void Alarmclock2_cmd(u8 mode);		//使能闹钟2 mode = 1 / 0
u8 Alarmclock1state_1(void);		//获取闹钟1标志位
u8 Alarmclock1state_2(void);		//获取闹钟2标志位
void Alarmclock1_close(void);		//关闭闹钟1
void Alarmclock2_close(void);		//关闭闹钟2
u8 BCD_BIN(u8 val);		//BCD码转换为二进制
u8 BIN_BCD(u8 val);		//二进制转换为BCD码

#endif


