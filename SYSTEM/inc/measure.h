#ifndef _MEASURE_H
#define _MEASURE_H
#include "sys.h"

#define R4		40000
#define R16		2400

float Measure_Rin(void);//输入电阻
float Measure_Rout(void);//输出电阻
float Measure_gain(void);//电路增益
float Get_U(void);//等到电压峰-峰值
void MOSFET_CONTROL_INIT(void);
void GPIOB_config(uint16_t GPIO_Pin_x);//使能或使能相关GPIOB，默认低电平

#endif


