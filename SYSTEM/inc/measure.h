#ifndef _MEASURE_H
#define _MEASURE_H
#include "sys.h"

#define R4		40000
#define R16		2400

float Measure_Rin(void);//�������
float Measure_Rout(void);//�������
float Measure_gain(void);//��·����
float Get_U(void);//�ȵ���ѹ��-��ֵ
void MOSFET_CONTROL_INIT(void);
void GPIOB_config(uint16_t GPIO_Pin_x);//ʹ�ܻ�ʹ�����GPIOB��Ĭ�ϵ͵�ƽ

#endif


