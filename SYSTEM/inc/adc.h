#ifndef _ADC_H
#define _ADC_H
#include "sys.h"


#define Tout		72


void Adc_Init(void);//使能ADC
u16  Get_Adc(u8 ADC_Channel_x); //选取通道进行ADC转化
u16 Get_Adc_Average(u8 ADC_Channel_x,u8 times); //取出ADC的平均值
void pri_Data_U(void);//把ADC数据装进数组

#endif


