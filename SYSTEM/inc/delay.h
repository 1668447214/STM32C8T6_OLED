#ifndef _DELAY_H
#define _DELAY_H
#include "stdint.h"


//void delay(uint16_t timers);
void delay_Init(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);



#endif



