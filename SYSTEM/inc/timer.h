#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//#include "stm32f10x.h"

#define Value 	256		//���ݴ�С
#define TIM5_IRQn        50
#define TIMx						TIM2

typedef struct ICStruct
{
	uint8_t CompleteState;
	uint8_t Repeation;
	uint32_t first;
	uint32_t second;
	uint32_t time; //us
	float Freq;  //khz
}ICStruct;

typedef enum 
{
  IC_Complete  = 0x00U,
  IC_Working   = 0x01U,
  IC_Sleeping  = 0x02U,
}ICState;


void TIM3_Init(u16 arr,u16 psc);

void TIM3_PWN_Init(u16 arr,u16 psc);

void TIM2_Cap_Init(u16 arr,u16 psc);

void TIM3_Int_Init(u16 arr,u16 psc);

float GetFreq(void);    //��ȡƵ�ʣ�ʧ�ܷ���0x00���迪������
void StartGetFreq(void);  //��������
void pri_Data_U(void);//�ѵ�ѹ�������뵽��������
float Max_Min(void);//�ҳ����ֵ����Сֵ
long* Re_Data_U(void);//����ֵΪ��ѹֵͷ��ַ���Ա������ط��õ�
ICState getICState(void);//��ȡ����״̬
void StartGetFreq(void);   //��������
#endif

