#include "measure.h"
#include "adc.h"
#include "tim.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "stdio.h"

void GPIOB_config(uint16_t GPIO_Pin_x)//使能或使能相关GPIOB引脚
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//默认为下拉
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_x;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_x);
}

void MOSFET_CONTROL_INIT(void)
{
	
	GPIOB_config(GPIO_Pin_9);
	GPIOB_config(GPIO_Pin_8);
	GPIOB_config(GPIO_Pin_7);//配置开关引脚|GPIO_Pin_5
	GPIO_SetBits(GPIOB,GPIO_Pin_7   );//Q2导通
}

/*
	PA2 -> 输入排插1脚AD采样
	PB4 -> 输入排插2脚
	PB5 -> 输入排插3脚
	输入排插4脚 -> 输入信号
*/
float top_u1,top_u2;//分别为输入电压和输入电压
extern u16 Date_U0_AD[Value4];
float Measure_Rin(void)//输入电阻
{
//	int t = 0;
	float Rin=0;
	float U1=0,U2=0;//
	
	TIM4_start();//开启定时器
	while(((TIM4->CR1) & 0x01 )!= 0);
	U1 = U0_AD_Max_Min();//获得Q2导通，Q1断开的交流电压峰-峰值
	top_u1 = U1;//输入的峰-峰值u1
//	printf("U1 = %f\r\n",U1);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//Q1导通
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);//Q2截止
	delay_ms(250);
	
	TIM4_start();//开启定时器，重新采样
	while(((TIM4->CR1) & 0x01 )!= 0);
	U2 = U0_AD_Max_Min();
//	printf("U2 = %f\r\n",U2);
	Rin = R4*(U2/(U1-U2));
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//关闭PB8
	GPIO_SetBits(GPIOB,GPIO_Pin_7);//打开PB7
	
	return Rin;
}


float Measure_Rout(void)//输出电阻
{
	float Rout=0;
	float U1=0,U2=0;//

	
	U1 = Max_Min();//这便是U1
	U1 = (float)(U1/4095)*3.3;//AD值转换为电压值，为输出电压的峰-峰值除以2
	top_u2 = U1;
	printf("输出电阻的U1 = %f\r\n",U1);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);//Q3导通
	delay_ms(250);
	
	TIM_Cmd(TIM3, ENABLE);  //开启TIMx外设,开启时钟，开启ADC采样转换
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	
	delay_ms(10);
	
	U2 = Max_Min();//
	U2 = (float)(U2/4095)*3.3;
	printf("输出电阻的U2 = %f\r\n",U2);
	
	Rout = R16*U1/U2;
	printf("输出电阻的Rout = %f\r\n",Rout);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//Q3截止
	
	return Rout;
}	



float Measure_gain(void)//电路增益
{
	float A0;
	A0 = (top_u2*250/3);
	return A0;
}





