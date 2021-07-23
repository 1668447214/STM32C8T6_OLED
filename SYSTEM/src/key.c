#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "key.h"
#include "delay.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//GPIO_Init(GPIOA,&GPIO_InitStruct);
}

u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1; //按键按松开标志
	if(mode)key_up=1; //支持连按
	if(key_up && KEY0 == 0)
	{
	delay_ms(10); //去抖动
	key_up=0;
	if(KEY0 == 0)return 1;//按下
	}
	else if(KEY0==1)key_up=1;
	return 0; 
}

