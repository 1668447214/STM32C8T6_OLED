#ifndef   _TIM_H
#define   _TIM_H
#include "stdint.h"

#define Value4 200

float U0_AD_Max_Min(void);//返回峰-峰值
void TIM4_Init(uint16_t arr, uint16_t psc);//用于测输入输出电阻的定时器
void TIM4_start(void);//开启定时器中断
void TIM4_AD_U1U2(void);//把电压数据弄进数组，用定时器中断

#endif

