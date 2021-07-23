/*#include "stdint.h"



void delay(uint16_t timers)
{
	uint16_t i,j;
	for(i=0;i<timers;i++)
	{
		for(j=0;j<0xffff;j++)
		{
		
		}
	}

}*/
#include "delay.h"
#include "stm32f10x.h"


static uint8_t   D_us=0;		//微妙系数
static uint16_t  D_ms=0;		//毫秒系数


void delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//系统时钟初始为72M除8M等于9，SysTick值减一就要花费1/9毫秒时间
	
	D_us = SystemCoreClock/8000000;
	D_ms = (uint16_t)D_us * 1000;
	
}

void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->CTRL = 0x00;			//关闭SysTick定时器
	SysTick->LOAD = nus*D_us; 		//延时重装载值	  		 
	SysTick->VAL  = 0x00;        	//清空计数器
	SysTick->CTRL|= 0x01 ;			//启动SysTick定时器  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	  	//等待延时结束  
	SysTick->CTRL = 0x00;						//关闭SysTick定时器
	SysTick->VAL  = 0X00;      					//清空计数器
}


void delay_ms(uint32_t nms)
{
	uint32_t temp;
	SysTick->CTRL = 0x00;			//关闭SysTick定时器
	SysTick->LOAD = nms*D_ms; 		//延时重装载值	  		 
	SysTick->VAL  = 0x00;        	//清空计数器
	SysTick->CTRL|= 0x01 ;			//启动SysTick定时器  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	  	//等待延时结束  
	SysTick->CTRL = 0x00;						//关闭SysTick定时器
	SysTick->VAL  = 0X00;      					//清空计数器	
}




