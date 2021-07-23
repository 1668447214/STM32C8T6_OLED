#ifndef __FFT256_H
#define __FFT256_H


#include "stm32_dsp.h"


void fft_mag_tranf(long *in);
//float THD_Cal(void);
void test_fft(void);
uint16_t getFreq_fft(void);
double THD_Cal();




#endif

