#ifndef   _TIM_H
#define   _TIM_H
#include "stdint.h"

#define Value4 200

float U0_AD_Max_Min(void);//���ط�-��ֵ
void TIM4_Init(uint16_t arr, uint16_t psc);//���ڲ������������Ķ�ʱ��
void TIM4_start(void);//������ʱ���ж�
void TIM4_AD_U1U2(void);//�ѵ�ѹ����Ū�����飬�ö�ʱ���ж�

#endif

