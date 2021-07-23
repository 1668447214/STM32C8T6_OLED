#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"

#define KEY0_PRES 0 //KEY0 按下
#define KEY0 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//返回按键状态1为断开，0为按下


void Key_Init(void);
u8 KEY_Scan(u8 mode);

#endif





