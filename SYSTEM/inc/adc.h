#ifndef _ADC_H
#define _ADC_H
#include "sys.h"


#define Tout		72


void Adc_Init(void);//ʹ��ADC
u16  Get_Adc(u8 ADC_Channel_x); //ѡȡͨ������ADCת��
u16 Get_Adc_Average(u8 ADC_Channel_x,u8 times); //ȡ��ADC��ƽ��ֵ
void pri_Data_U(void);//��ADC����װ������

#endif


