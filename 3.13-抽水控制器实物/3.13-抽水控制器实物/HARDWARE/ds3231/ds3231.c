#include "ds3231.h"
#include "myiic_1.h"
#include "delay.h"
#include "usart.h"


//时间数据
int year_return = 24;//年
int moonth_return = 3;//月
int day_return = 28;//日
int hour_return = 14;//小时
int min_return = 43; //分钟
int sec_return = 30; //秒数

_calendar_part calendar;//声明时钟结构体
_Alarmclock_part Alarmclock;//声明闹钟结构体


void DS3231_Init(void)	//初始化
{
	
	IIC_Init_1();
	//ModifyTime(year_return,moonth_return,day_return,hour_return,min_return,sec_return);		//修改时间
	printf("\r\n时间数据： \r\n");
	printf("%04d,%02d,%02d,%02d:%02d:%02d \r\n",year_return,moonth_return,day_return,hour_return,min_return,sec_return);	

}


void DS3231_IIC_Write(u8 addr,u8 data)	//写数据
{
	IIC_Start_1();
	IIC_Send_Byte_1(DS3231_Write_Address);
	IIC_Wait_Ack_1();
	delay_us(5);
	IIC_Send_Byte_1(addr);
	IIC_Wait_Ack_1();
	delay_us(5);
	IIC_Send_Byte_1(data);
	IIC_Wait_Ack_1();
	delay_us(5);
	IIC_Stop_1();
}


u8 DS3231_IIC_Read(u8 addr)	//读数据
{
	u8 data_s;
	IIC_Start_1();
	IIC_Send_Byte_1(DS3231_Write_Address);
	IIC_Wait_Ack_1();
	delay_us(5);
	IIC_Send_Byte_1(addr);
	IIC_Wait_Ack_1();
	delay_us(5);
	IIC_Start_1();
	IIC_Send_Byte_1(DS3231_Read_Address);
	IIC_Wait_Ack_1();
	delay_us(5);
	data_s = IIC_Read_Byte_1(1);
	IIC_Stop_1();
	return data_s;
}


void SetAlarmclock_1(u8 ahour,u8 amin,u8 asec)//设置闹钟1
{
	u8 d;
	d = BIN_BCD(ahour);
	DS3231_IIC_Write(DS3231_AL1HOUR_REG,d);
	d = BIN_BCD(amin);
	DS3231_IIC_Write(DS3231_AL1MIN_REG,d);
	d = BIN_BCD(asec);
	DS3231_IIC_Write(DS3231_AL1SEC_REG,d);
	DS3231_IIC_Write(DS3231_AL1WDAY_REG,0x80);
}


void SetAlarmclock_2(u8 ahour,u8 amin)//设置闹钟2
{
	u8 d;
	d = BIN_BCD(ahour);
	DS3231_IIC_Write(DS3231_AL2HOUR_REG,d);
	d = BIN_BCD(amin);
	DS3231_IIC_Write(DS3231_AL2MIN_REG,d);
	DS3231_IIC_Write(DS3231_AL2WDAY_REG,0x80);
}


void Alarmclock1_cmd(u8 mode)		//mode = 1使能 / 0失能  闹钟1使能
{	
	if(mode)
	DS3231_IIC_Write(DS3231_CONTROL_REG,0x1D);
	else
	DS3231_IIC_Write(DS3231_CONTROL_REG,0x1C);
}


void Alarmclock2_cmd(u8 mode)		//mode = 1使能 / 0失能  闹钟2使能
{	
	if(mode)
	DS3231_IIC_Write(DS3231_CONTROL_REG,0x1E);
	else
	DS3231_IIC_Write(DS3231_CONTROL_REG,0x1C);	
}


u8 Alarmclock1state_1(void)	//读闹钟1响应标志
{
	u8 A1F;
	A1F=DS3231_IIC_Read(DS3231_STATUS_REG)&0x01;
	return A1F;
}


u8 Alarmclock1state_2(void)	//读闹钟2响应标志
{
	u8 A2F;
	A2F=DS3231_IIC_Read(DS3231_STATUS_REG)&0x02;
	return A2F;
}


void Alarmclock1_close(void)	//关闭闹钟1并清除标志位
{
	Alarmclock1_cmd(0);
	DS3231_IIC_Write(DS3231_STATUS_REG,0x00);
}


void Alarmclock2_close(void)	//关闭闹钟2并清除标志位
{
	Alarmclock1_cmd(0);
	DS3231_IIC_Write(DS3231_STATUS_REG,0x00);
}


void ModifyTime(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)//修改时间
{
	u8 temp;
	temp = BIN_BCD(year);
	DS3231_IIC_Write(DS3231_YEAR_REG,temp);
	temp = BIN_BCD(month);
	DS3231_IIC_Write(DS3231_MONTH_REG,temp);
	temp = BIN_BCD(day);
	DS3231_IIC_Write(DS3231_MDAY_REG,temp);
	temp = BIN_BCD(hour);
	DS3231_IIC_Write(DS3231_HOUR_REG,temp);
	temp = BIN_BCD(min);
	DS3231_IIC_Write(DS3231_MIN_REG,temp);
	temp = BIN_BCD(sec);
	DS3231_IIC_Write(DS3231_SEC_REG,temp);
}


void get_time(void)//获取时间
{
	calendar.year = DS3231_IIC_Read(DS3231_YEAR_REG);
	calendar.year = BCD_BIN(calendar.year);
	
	calendar.month = DS3231_IIC_Read(DS3231_MONTH_REG);
	calendar.month = BCD_BIN(calendar.month);
	
	calendar.day = DS3231_IIC_Read(DS3231_MDAY_REG);
	calendar.day = BCD_BIN(calendar.day);
	
	calendar.hour = DS3231_IIC_Read(DS3231_HOUR_REG);
	calendar.hour &= 0x3f;
	calendar.hour = BCD_BIN(calendar.hour);
	
	calendar.min = DS3231_IIC_Read(DS3231_MIN_REG);
	calendar.min = BCD_BIN(calendar.min);
	
	calendar.sec = DS3231_IIC_Read(DS3231_SEC_REG);
	calendar.sec = BCD_BIN(calendar.sec);
	
	return;
}


u8 BCD_BIN(u8 val)//BCD码转换为二进制
{
	u8 temp;
	temp=val&0x0f;
	val>>=4;
	val&=0x0f;
	val*=10;
	temp+=val;
	return temp;
}


u8 BIN_BCD(u8 val)//二进制转换为BCD码
{
	u8 i,j,k;
	i=val/10;
	j=val%10;
	k=j+(i<<4);
	return k;
}

