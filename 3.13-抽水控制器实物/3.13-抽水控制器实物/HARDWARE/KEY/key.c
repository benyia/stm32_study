#include "key.h"
#include "delay.h"

  
//按键初始化函数 
//PC8和PC9设置成输入
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);//使能PORTC和PORTA时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PC8设置成输入，默认上拉	 
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PC9设置成输入，默认上拉	 
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
	
} 


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY1_PRES，KEY1按下
//KEY2_PRES，KEY2按下
//KEY3_PRES，KEY3按下
//注意此函数有响应优先级,KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1 == 1|| KEY2 == 0|| KEY3 == 0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==1)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
	}else if(KEY1==0&&KEY2==1&&KEY3==1)
	{
		key_up=1; 
	}
	return 0;// 无按键按下
}
