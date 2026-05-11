#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "myiic.h"
#include "INA226.h"
#include "oled.h"
#include "timer.h"
#include "key.h"
#include "brake.h"
#include "ds3231.h"
#include "myiic_1.h"
#include "BUZZER.h"
#include "HC05_UART3.h"
#include "exti.h" 


//变量定义
u8 mode = 0;		//模式(0为设定开关,1为设定时间)
float current;		//电流
float voltage;		//电压
float power;		//功率
extern int time;		//设定的计时时间
extern u16 time_cnt;		//计时
extern u8 flag;		//水泵开启标志
u8 set_time_flag;		//设定时间的标志
u8 set_time_cnt_flag;		//设定的计时时间的标志
u8 set_switch_flag;		//设定开关的标志
extern uint8_t Flag;		//判断是否接收到数据的标志
INPUT Rec;		//接收到的数据的结构体
int flow_rate;		//流量
u8 i=0;
extern u16 water_time;		//水流时间
//时间数据
extern int year_return;//年
extern int moonth_return;//月
extern int day_return;//日
extern int hour_return;//小时
extern int min_return; //分钟
extern int sec_return; //秒数


//函数定义
void Key_Control(void);		//按键控制函数


int main(void)
{	 
		
	delay_init();		//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);		//串口初始化为115200
 	LED_Init();		//LED端口初始化
	INA226_Init();		//INA226初始化
	OLED_GPIO_Init();//显示屏IO初始化
  OLED_Init();//OLED初始化
	DS3231_Init();		//DS3231初始化
	BUZZER_Init();		//蜂鸣器初始化
	KEY_Init();		//按键初始化
	EXTIX_Init(3,1);		//外部中断初始化
	BRAKE_Init();		//继电器初始化
	TIM3_Int_Init(7999,8999);//10Khz的计数频率，计数到8000为1s, 计算公式:Tout= ((arr+1)*(psc+1))/72;
	uart3_init(9600,2,2);	 	//串口初始化为9600(蓝牙模块使用的串口)
	Alarmclock1_close();//初始先将闹钟1关闭并清除闹钟标志位
	Alarmclock2_close();//初始先将闹钟2关闭并清除闹钟标志位	
	
	hour_return = 0;//小时
	min_return = 0; //分钟
	sec_return = 0; //秒数
	printf("初始化OK\r\n");

	//电压
	OLED_ShowFontHZ(0,0,111,16,1);		//显示汉字(电)
	OLED_ShowFontHZ(16,0,113,16,1);		//显示汉字(压)
	OLED_ShowString(32,0,":",16);		//:
	OLED_ShowChar(8*5,0,(int)voltage/10000+'0',16,1);		//十位
	OLED_ShowChar(8*6,0,((int)(voltage/1000)-((int)voltage/10000)*10)+'0',16,1);		//各位
	OLED_ShowChar(8*7,0,'.',16,1);
	OLED_ShowChar(8*8,0,((int)(voltage/100)-((int)voltage/1000)*10)+'0',16,1);	//小数后一位
	OLED_ShowChar(8*9,0,((int)(voltage/10)-((int)voltage/100)*10)+'0',16,1);	//小数后两位
	OLED_ShowChar(8*10,0,((int)(voltage)-((int)voltage/10)*10)+'0',16,1);	//小数后三位
	OLED_ShowString(88,0,"V",16);		//显示"V"
	OLED_ShowFontHZ(96,0,80,16,1);	
	OLED_ShowFontHZ(112,0,80,16,1);	
		
	//水泵状态(关闭)
	OLED_ShowFontHZ(0,16,138,16,1);		//显示汉字(水)
	OLED_ShowFontHZ(16,16,139,16,1);		//显示汉字(泵)	
	OLED_ShowFontHZ(32,16,142,16,1);		//显示汉字(关)	
	OLED_ShowFontHZ(48,16,143,16,1);		//显示汉字(闭)		
	OLED_ShowFontHZ(64,16,80,16,1);
	OLED_ShowFontHZ(80,16,80,16,1);	
	OLED_ShowFontHZ(96,16,80,16,1);	
	OLED_ShowFontHZ(112,16,80,16,1);
	
	//计时
	OLED_ShowFontHZ(0,32,123,16,1);		//显示汉字(计)
	OLED_ShowFontHZ(16,32,132,16,1);		//显示汉字(时)
	OLED_ShowString(32,32,":",16);		//:
	OLED_ShowChar(8*5,32,(int)time/10+'0',16,1);		//十位
	OLED_ShowChar(8*6,32,((int)(time)-((int)time/10)*10)+'0',16,1);		//各位
	OLED_ShowString(8*7,32,"S ",16);		//S 
	OLED_ShowFontHZ(72,32,80,16,1);
	OLED_ShowFontHZ(88,32,80,16,1);	
	OLED_ShowFontHZ(104,32,80,16,1);
	
	OLED_ShowChar(8*0,48,((int)calendar.hour)/10+'0',16,1);		//十位
	OLED_ShowChar(8*1,48,((int)calendar.hour)%10+'0',16,1);		//各位	
	OLED_ShowChar(8*2,48,'.',16,1);		//.
	OLED_ShowChar(8*3,48,((int)calendar.min)/10+'0',16,1);		//十位
	OLED_ShowChar(8*4,48,((int)calendar.min)%10+'0',16,1);		//各位	
	OLED_ShowChar(8*5,48,'.',16,1);		//.
	OLED_ShowChar(8*6,48,((int)calendar.sec)/10+'0',16,1);		//十位
	OLED_ShowChar(8*7,48,((int)calendar.sec)%10+'0',16,1);		//各位
	
	OLED_Refresh_Gram();		//更新显示
	
	while(1)
	{ 
		
		get_time();		//获取时间
		
		printf("芯片ID = %x\r\n",INA226_Read2Byte(ID_Reg));
		printf("data=%-5d, Bus_V  =%f mV\r\n", INA226_Read2Byte(Bus_V_Reg),INA226_Read2Byte(Bus_V_Reg)*1.29);
		printf("data=%-5d, Shunt_V=%f mV\r\n", INA226_Read2Byte(Shunt_V_Reg),INA226_Read2Byte(Shunt_V_Reg)*2.5*0.001);
		printf("data=%-5d, Curent =%f mA\r\n", INA226_Read2Byte(Current_Reg),INA226_Read2Byte(Current_Reg)*0.2);
		printf("data=%-5d, Power  =%f mW\r\n", INA226_Read2Byte(Power_Reg),INA226_Read2Byte(Power_Reg)*0.2*26);
		
		current = INA226_Read2Byte(Current_Reg)*0.2;		//获得电流
		voltage = INA226_Read2Byte(Bus_V_Reg)*1.29;		//获得电压
		power = INA226_Read2Byte(Power_Reg)*0.2*25;		//获得功率
		
		//电压
		OLED_ShowChar(8*5,0,(int)voltage/10000+'0',16,1);		//十位
		OLED_ShowChar(8*6,0,((int)(voltage/1000)-((int)voltage/10000)*10)+'0',16,1);		//各位
		OLED_ShowChar(8*7,0,'.',16,1);
		OLED_ShowChar(8*8,0,((int)(voltage/100)-((int)voltage/1000)*10)+'0',16,1);	//小数后一位
		OLED_ShowChar(8*9,0,((int)(voltage/10)-((int)voltage/100)*10)+'0',16,1);	//小数后两位
		OLED_ShowChar(8*10,0,((int)(voltage)-((int)voltage/10)*10)+'0',16,1);	//小数后三位
		
		//模式显示
		if(mode == 0)
		{
			OLED_ShowFontHZ(64,48,138,16,1);		//显示汉字(水)
			OLED_ShowFontHZ(80,48,139,16,1);		//显示汉字(泵)	
			OLED_ShowFontHZ(96,48,140,16,1);		//显示汉字(开)	
			OLED_ShowFontHZ(112,48,142,16,1);		//显示汉字(关)	
		}
		else if(mode == 1)
		{
			OLED_ShowFontHZ(64,48,144,16,1);		//显示汉字(设)
			OLED_ShowFontHZ(80,48,145,16,1);		//显示汉字(定)	
			OLED_ShowFontHZ(96,48,123,16,1);		//显示汉字(计)	
			OLED_ShowFontHZ(112,48,132,16,1);		//显示汉字(时)	
		}		
		else if(mode == 2)
		{
			OLED_ShowFontHZ(64,48,144,16,1);		//显示汉字(设)
			OLED_ShowFontHZ(80,48,145,16,1);		//显示汉字(定)
			OLED_ShowFontHZ(96,48,146,16,1);		//显示汉字(时)
			OLED_ShowFontHZ(112,48,147,16,1);		//显示汉字(间)
		}		
		
		if(set_time_flag == 0)		//显示实时时间
		{
			OLED_ShowChar(8*0,48,((int)calendar.hour)/10+'0',16,1);		//十位
			OLED_ShowChar(8*1,48,((int)calendar.hour)%10+'0',16,1);		//各位	
			OLED_ShowChar(8*2,48,'.',16,1);		//.
			OLED_ShowChar(8*3,48,((int)calendar.min)/10+'0',16,1);		//十位
			OLED_ShowChar(8*4,48,((int)calendar.min)%10+'0',16,1);		//各位	
			OLED_ShowChar(8*5,48,'.',16,1);		//.
			OLED_ShowChar(8*6,48,((int)calendar.sec)/10+'0',16,1);		//十位
			OLED_ShowChar(8*7,48,((int)calendar.sec)%10+'0',16,1);		//各位
		}

		OLED_Refresh_Gram();		//更新显示
	
		Key_Control();		//按键控制函数	
		
		if(Flag == 1)		//接收到数据
		{
			Rec = Data_Recv();		//将数据包的数据赋值到结构体中
			if(Rec.c == 0)		//如果char类型的数据为0,计时增加
			{
				time++;	
				if(time > 99)
				{
					time = 99;
				}
			}
			else if(Rec.c == 1)		//如果char类型的数据为1,计时减少
			{
				time--;
				if(time <= 0)
				{
					time = 0;
				}
			}
		}
		Blue_Send_re(USART3,flow_rate);		//将接收到flow_rate发送回去
		printf("flow_rate:%06d\r\n",flow_rate);		//显示flow_rate

		if(calendar.hour == 0 && calendar.min == 0 && calendar.sec == 0)
		{
			water_time = 0;		//自动复位
		}		
		//定时0.5秒循环
		if(i<50)
		{
			i++;
		}
		else
		{
			Velocity_Calculation(water_time);		//流量计算函数
			i = 0;
			//water_time = 0;
		}	
		
		delay_ms(100);	
		
	}
	
}


void Key_Control(void)	//按键控制函数
{
	u8 temp = 0;
	temp =	KEY_Scan(0);	//获取键值
	switch(temp)
	{
		case 1:
////////////////////////////////////////////////////////////////////////////////////
			if(mode == 0 && set_switch_flag == 0)		//模式切换
			{
				mode = 1;
				set_time_cnt_flag = 0;		//清除设定计时时间标志	
				LED0 = 1;		//灭灯	
			}
			else if(mode == 1 && set_time_cnt_flag == 0)
			{
				mode = 2;	
				set_time_flag = 0;		//清除设置时间标志	
				LED0 = 1;		//灭灯					
			}
			else if(mode == 2 && set_time_flag == 0)
			{
				mode = 0;	
				set_switch_flag = 0;		//清除开关标志	
				LED0 = 1;		//灭灯					
			}
////////////////////////////////////////////////////////////////////////////////////
			if(set_switch_flag != 0 && mode == 0)		//设定开关
			{
				set_switch_flag++;
			}
			if(set_switch_flag >=2 && mode == 0)
			{
				set_switch_flag = 0;		//退出设定开关
				LED0 = 1;		//灭灯	
			}
////////////////////////////////////////////////////////////////////////////////////
			if(set_time_cnt_flag != 0 && mode == 1)		//设定计时
			{
				set_time_cnt_flag++;
			}
			if(set_time_cnt_flag >=3 && mode == 1)
			{
				set_time_cnt_flag = 0;		//退出设置计时
				LED0 = 1;		//灭灯	
			}
////////////////////////////////////////////////////////////////////////////////////
			if(set_time_flag != 0 && mode == 2)		//设定时间时
			{
				set_time_flag++;
			}
			if(set_time_flag >=7 && mode == 2)
			{
				set_time_flag = 0;		//退出设置时间模式
				ModifyTime(year_return,moonth_return,day_return,hour_return,min_return,sec_return);		//修改时间
				LED0 = 1;		//灭灯	
			}
////////////////////////////////////////////////////////////////////////////////////
		break;
	  case 2:
////////////////////////////////////////////////////////////////////////////////////
			if(set_switch_flag == 1 && mode == 0)		//设定开关
			{
				if(flag == 0)
				{
					BRAKE = 1;		//水泵启动	
					//水泵状态(开启)
					OLED_ShowFontHZ(0,16,138,16,1);		//显示汉字(水)
					OLED_ShowFontHZ(16,16,139,16,1);		//显示汉字(泵)	
					OLED_ShowFontHZ(32,16,140,16,1);		//显示汉字(开)	
					OLED_ShowFontHZ(48,16,141,16,1);		//显示汉字(启)		
					OLED_Refresh_Gram();		//更新显示	
					flag = 1;					
				}
				else
				{
					BRAKE = 0;		//水泵关闭
					//水泵状态(关闭)
					OLED_ShowFontHZ(0,16,138,16,1);		//显示汉字(水)
					OLED_ShowFontHZ(16,16,139,16,1);		//显示汉字(泵)	
					OLED_ShowFontHZ(32,16,142,16,1);		//显示汉字(关)	
					OLED_ShowFontHZ(48,16,143,16,1);		//显示汉字(闭)		
					OLED_Refresh_Gram();		//更新显示		
					flag = 0;						
				}
			}	
////////////////////////////////////////////////////////////////////////////////////
			if(set_time_cnt_flag == 1 && mode == 1)		//设定计时加
			{
				time = time + 10;		//设置十位	
				if(time>=99)
				{
					time = 99;
				}
				OLED_ShowChar(8*5,32,(int)time/10+'0',16,1);		//十位
				OLED_ShowChar(8*6,32,((int)(time)-((int)time/10)*10)+'0',16,1);		//各位
				OLED_Refresh_Gram();		//更新显示				
			}	
			else if(set_time_cnt_flag == 2 && mode == 1)
			{
				time = time + 1;		//设置个位	
				if(time>=99)
				{
					time = 99;
				}
				OLED_ShowChar(8*5,32,(int)time/10+'0',16,1);		//十位
				OLED_ShowChar(8*6,32,((int)(time)-((int)time/10)*10)+'0',16,1);		//各位
				OLED_Refresh_Gram();		//更新显示				
			}
////////////////////////////////////////////////////////////////////////////////////
			if(set_time_flag == 1 && mode == 2)		//设定时间加
			{
				hour_return = hour_return + 10;		//设置小时的十位
				if(hour_return>=24)
				{
					hour_return = 24;
				}
				else if(hour_return < 0)
				{
					hour_return = 0;
				}
				OLED_ShowChar(8*0,48,((int)hour_return)/10+'0',16,1);		//十位	
				OLED_ShowChar(8*1,48,((int)hour_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示
			}
			else if(set_time_flag == 2 && mode == 2)
			{
				hour_return = hour_return + 1;		//设置小时的个位	
				if(hour_return>=24)
				{
					hour_return = 24;
				}
				else if(hour_return < 0)
				{
					hour_return = 0;
				}
				OLED_ShowChar(8*0,48,((int)hour_return)/10+'0',16,1);		//十位	
				OLED_ShowChar(8*1,48,((int)hour_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示				
			}
			else if(set_time_flag == 3 && mode == 2)
			{
				min_return = min_return + 10;		//设置分钟的十位
				if(min_return>=60)
				{
					min_return = 60;
				}
				else if(min_return < 0)
				{
					min_return = 0;
				}
				OLED_ShowChar(8*3,48,((int)min_return)/10+'0',16,1);		//十位		
				OLED_ShowChar(8*4,48,((int)min_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示
			}
			else if(set_time_flag == 4 && mode == 2)
			{
				min_return = min_return + 1;		//设置分钟的个位
				if(min_return>=60)
				{
					min_return = 60;
				}
				else if(min_return < 0)
				{
					min_return = 0;
				}
				OLED_ShowChar(8*3,48,((int)min_return)/10+'0',16,1);		//十位		
				OLED_ShowChar(8*4,48,((int)min_return)%10+'0',16,1);		//各位			
				OLED_Refresh_Gram();		//更新显示				
			}
			else if(set_time_flag == 5 && mode == 2)
			{
				sec_return = sec_return + 10;		//设置秒的十位
				if(sec_return>=60)
				{
					sec_return = 60;
				}
				else if(sec_return < 0)
				{
					sec_return = 0;
				}
				OLED_ShowChar(8*6,48,((int)sec_return)/10+'0',16,1);		//十位
				OLED_ShowChar(8*7,48,((int)sec_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示
			}
			else if(set_time_flag == 6 && mode == 2)
			{
				sec_return = sec_return + 1;		//设置秒的个位	
				if(sec_return>=60)
				{
					sec_return = 60;
				}
				else if(sec_return < 0)
				{
					sec_return = 0;
				}
				OLED_ShowChar(8*6,48,((int)sec_return)/10+'0',16,1);		//十位
				OLED_ShowChar(8*7,48,((int)sec_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示				
			}
////////////////////////////////////////////////////////////////////////////////////
		break;		
	  case 3:
////////////////////////////////////////////////////////////////////////////////////
			if(mode == 0 && set_switch_flag == 0)		//确认键
			{
				set_switch_flag = 1;		//进入设置开关模式
				LED0 = 0;		//亮灯
				break;
			}
			else if(mode == 1 && set_time_cnt_flag == 0)
			{
				set_time_cnt_flag = 1;		//进入设置计时模式
				LED0 = 0;		//亮灯
				break;
			}
			else if(mode == 2 && set_time_flag == 0)
			{
				set_time_flag = 1;		//进入设置时间模式	
				LED0 = 0;		//亮灯
				break;
			}
////////////////////////////////////////////////////////////////////////////////////
			if(set_time_cnt_flag == 1 && mode == 1)		//设定计时减
			{
				time = time - 10;		//设置十位	
				if(time<=0)
				{
					time = 0;
				}
				OLED_ShowChar(8*5,32,(int)time/10+'0',16,1);		//十位
				OLED_ShowChar(8*6,32,((int)(time)-((int)time/10)*10)+'0',16,1);		//各位
				OLED_Refresh_Gram();		//更新显示				
			}	
			else if(set_time_cnt_flag == 2 && mode == 1)
			{
				time = time - 1;		//设置个位	
				if(time<=0)
				{
					time = 0;
				}
				OLED_ShowChar(8*5,32,(int)time/10+'0',16,1);		//十位
				OLED_ShowChar(8*6,32,((int)(time)-((int)time/10)*10)+'0',16,1);		//各位
				OLED_Refresh_Gram();		//更新显示				
			}
////////////////////////////////////////////////////////////////////////////////////
			if(set_time_flag == 1 && mode == 2)		//设定时间减
			{
				hour_return = hour_return - 10;		//设置小时的十位
				if(hour_return>=24)
				{
					hour_return = 24;
				}
				else if(hour_return < 0)
				{
					hour_return = 0;
				}
				OLED_ShowChar(8*0,48,((int)hour_return)/10+'0',16,1);		//十位	
				OLED_ShowChar(8*1,48,((int)hour_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示
			}
			else if(set_time_flag == 2 && mode == 2)
			{
				hour_return = hour_return - 1;		//设置小时的个位	
				if(hour_return>=24)
				{
					hour_return = 24;
				}
				else if(hour_return < 0)
				{
					hour_return = 0;
				}
				OLED_ShowChar(8*0,48,((int)hour_return)/10+'0',16,1);		//十位	
				OLED_ShowChar(8*1,48,((int)hour_return)%10+'0',16,1);		//各位		
				OLED_Refresh_Gram();		//更新显示				
			}
			else if(set_time_flag == 3 && mode == 2)
			{
				min_return = min_return - 10;		//设置分钟的十位
				if(min_return>=60)
				{
					min_return = 60;
				}
				else if(min_return < 0)
				{
					min_return = 0;
				}
				OLED_ShowChar(8*3,48,((int)min_return)/10+'0',16,1);		//十位		
				OLED_ShowChar(8*4,48,((int)min_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示
			}
			else if(set_time_flag == 4 && mode == 2)
			{
				min_return = min_return - 1;		//设置分钟的个位
				if(min_return>=60)
				{
					min_return = 60;
				}
				else if(min_return < 0)
				{
					min_return = 0;
				}
				OLED_ShowChar(8*3,48,((int)min_return)/10+'0',16,1);		//十位		
				OLED_ShowChar(8*4,48,((int)min_return)%10+'0',16,1);		//各位			
				OLED_Refresh_Gram();		//更新显示				
			}
			else if(set_time_flag == 5 && mode == 2)
			{
				sec_return = sec_return - 10;		//设置秒的十位
				if(sec_return>=60)
				{
					sec_return = 60;
				}
				else if(sec_return < 0)
				{
					sec_return = 0;
				}
				OLED_ShowChar(8*6,48,((int)sec_return)/10+'0',16,1);		//十位
				OLED_ShowChar(8*7,48,((int)sec_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示
			}
			else if(set_time_flag == 6 && mode == 2)
			{
				sec_return = sec_return - 1;		//设置秒的个位	
				if(sec_return>=60)
				{
					sec_return = 60;
				}
				else if(sec_return < 0)
				{
					sec_return = 0;
				}
				OLED_ShowChar(8*6,48,((int)sec_return)/10+'0',16,1);		//十位
				OLED_ShowChar(8*7,48 ,((int)sec_return)%10+'0',16,1);		//各位	
				OLED_Refresh_Gram();		//更新显示				
			}
////////////////////////////////////////////////////////////////////////////////////
		break;	
		default:
			delay_ms(10);	
	}

}


