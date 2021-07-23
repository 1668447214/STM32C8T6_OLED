#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//#include "stm32f10x.h"

#define Value 	256		//数据大小
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

float GetFreq(void);    //获取频率，失败返回0x00，需开启捕获。
void StartGetFreq(void);  //开启捕获
void pri_Data_U(void);//把电压数据输入到数组里面
float Max_Min(void);//找出最大值和最小值
long* Re_Data_U(void);//返回值为电压值头地址，以便其他地方用到
ICState getICState(void);//获取捕获状态
void StartGetFreq(void);   //开启捕获
#endif

