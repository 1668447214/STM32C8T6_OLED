#include "measure.h"
#include "adc.h"
#include "tim.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "stdio.h"

void GPIOB_config(uint16_t GPIO_Pin_x)//ʹ�ܻ�ʹ�����GPIOB����
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//Ĭ��Ϊ����
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_x;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_x);
}

void MOSFET_CONTROL_INIT(void)
{
	
	GPIOB_config(GPIO_Pin_9);
	GPIOB_config(GPIO_Pin_8);
	GPIOB_config(GPIO_Pin_7);//���ÿ�������|GPIO_Pin_5
	GPIO_SetBits(GPIOB,GPIO_Pin_7   );//Q2��ͨ
}

/*
	PA2 -> �����Ų�1��AD����
	PB4 -> �����Ų�2��
	PB5 -> �����Ų�3��
	�����Ų�4�� -> �����ź�
*/
float top_u1,top_u2;//�ֱ�Ϊ�����ѹ�������ѹ
extern u16 Date_U0_AD[Value4];
float Measure_Rin(void)//�������
{
//	int t = 0;
	float Rin=0;
	float U1=0,U2=0;//
	
	TIM4_start();//������ʱ��
	while(((TIM4->CR1) & 0x01 )!= 0);
	U1 = U0_AD_Max_Min();//���Q2��ͨ��Q1�Ͽ��Ľ�����ѹ��-��ֵ
	top_u1 = U1;//����ķ�-��ֵu1
//	printf("U1 = %f\r\n",U1);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//Q1��ͨ
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);//Q2��ֹ
	delay_ms(250);
	
	TIM4_start();//������ʱ�������²���
	while(((TIM4->CR1) & 0x01 )!= 0);
	U2 = U0_AD_Max_Min();
//	printf("U2 = %f\r\n",U2);
	Rin = R4*(U2/(U1-U2));
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//�ر�PB8
	GPIO_SetBits(GPIOB,GPIO_Pin_7);//��PB7
	
	return Rin;
}


float Measure_Rout(void)//�������
{
	float Rout=0;
	float U1=0,U2=0;//

	
	U1 = Max_Min();//�����U1
	U1 = (float)(U1/4095)*3.3;//ADֵת��Ϊ��ѹֵ��Ϊ�����ѹ�ķ�-��ֵ����2
	top_u2 = U1;
	printf("��������U1 = %f\r\n",U1);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);//Q3��ͨ
	delay_ms(250);
	
	TIM_Cmd(TIM3, ENABLE);  //����TIMx����,����ʱ�ӣ�����ADC����ת��
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	
	delay_ms(10);
	
	U2 = Max_Min();//
	U2 = (float)(U2/4095)*3.3;
	printf("��������U2 = %f\r\n",U2);
	
	Rout = R16*U1/U2;
	printf("��������Rout = %f\r\n",Rout);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//Q3��ֹ
	
	return Rout;
}	



float Measure_gain(void)//��·����
{
	float A0;
	A0 = (top_u2*250/3);
	return A0;
}





