#include "timer.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "stdio.h"
#include "delay.h"

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

#define TIMxClock 72000  //kHz
#define TIMxClock1 TIMxClock * 1000
ICStruct TIM1IC_CHANNEL2 = {0,0,0,0,0,0};



void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

/*
Tout= ((arr+1)*(psc+1))/Tclk

����arr���Զ���װ��ֵ��

psc��Ԥ��Ƶ����

Tclk����Ƶ������Ƶ��Ϊ72MHz

ToutΪ�ж����ʱ�䣨�����жϣ�
*/
u16 N=0;//��������
long Date_U[Value] = {0};//��ѹADֵ����
float ADC_Max=0,ADC_Min=4095;//��ѹ���ֵ��Сֵ
void TIM3_IRQHandler(void)   //TIM3�ж�,��ʵʱ��ѹֵд������
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		Date_U[N] = Get_Adc(ADC_Channel_1);		
//			Date_U[N] = (float)adcx*(3.3/4096);//��ѹֵ
		N++;
		if(N == Value)
		{
			TIM_Cmd(TIM3, DISABLE);  //�ر�TIMx����,ʱ��Ҳ���ر��ˣ��ǵÿ���ʱ��
			TIM_ITConfig(TIM3,TIM_IT_Update  |TIM_IT_Trigger,DISABLE);
			N = 0;
		}
	}
}

void pri_Data_U(void)//�ѵ�ѹ����Ū�����飬�ö�ʱ���ж�
{
	u16 arr;//ADֵ����װ��ֵ
	float onretime = 23;
	arr = (onretime*Tout)-1;//�����װ��ֵ,23us��ʱ���жϣ���ת��ֵΪ1655��
	TIM3_Int_Init(arr,0);//23us
}

/*
	�ҳ����ֵ��Сֵ
*/
float Max_Min(void)
{
	u16 i=0;
	for(i=0;i<Value;i++)
	{
			if(ADC_Max < Date_U[i])
			{
				ADC_Max = Date_U[i];//ȡ�����ֵ
			}
			if(ADC_Min > Date_U[i])
			{
				ADC_Min = Date_U[i];//ȡ����Сֵ
			}
	}
	 return ((ADC_Max - ADC_Min));
}
/*
	���ص�ѹ���ݵ�����
*/
long* Re_Data_U(void)
{
	return Date_U;
}



//PWM�����ʼ���������ã��ɲ�Ҫ
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

void TIM3_PWN_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}
//----------------------------------------------------------------------------------------------------
//��ʱ��2ͨ��1���벶������

TIM_ICInitTypeDef  TIM2_ICInitStructure;

void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);						 //PA0 ����
	
	TIM2->CCMR1 = 0X0100;
  TIM2->SR = 0X00;
	TIM2->ARR = 0XFFFF;
	TIM2->PSC = 0X00;
	TIM2->CCER = 0X10;
	TIM2->CR1 = 0X01;
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update | TIM_IT_CC4);
  TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��5
   


}

u8  TIM2CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
 
void startCapture()
{
	TIM2->SR = 0x00;
	TIM2->DIER = 0x11;
	TIM1IC_CHANNEL2.CompleteState = 0x00;
	TIM1IC_CHANNEL2.Repeation = 0x00;
}

void TIM2_IRQHandler(void)//�ж�Ҫ�������ļ������һĨһ�ۣ�����������ѭ��.
{ 

 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET && TIM1IC_CHANNEL2.CompleteState != 2) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM1IC_CHANNEL2 .Repeation++;	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		LED0=!LED0;
		}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC4) != RESET)
	{
	 if(TIM1IC_CHANNEL2.CompleteState == 0)
	 {
		 TIM1IC_CHANNEL2.first = TIM2->CCR4;//��ʱ������ֵ
		 TIM1IC_CHANNEL2.CompleteState++;
	 }
	 else if(TIM1IC_CHANNEL2.CompleteState == 1)
	 {
     TIM1IC_CHANNEL2.second = TIM2->CCR4;//��ʱ������ֵ
		 TIM1IC_CHANNEL2.CompleteState++;
	 }
	 else          //When CompleteState == 2, IC hava completed ,Waiting calculate
	 {
		 TIM2->DIER = 0X00;
	 }
	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ
}

ICState getICState(void)
{
	if(TIM1IC_CHANNEL2.CompleteState == 2)
	{
		//TIM1IC_CHANNEL2.CompleteState = 0;
		return IC_Complete;
	}
	else
	{
		//TIM1IC_CHANNEL2.CompleteState = 0;
		return IC_Working;
	}
}

float GetFreq(void)
{
	if(getICState() != 0)
	{
		return 0x00;
	}
		else
	{ 
		TIM1IC_CHANNEL2.Freq = (TIMxClock / (float)((TIM1IC_CHANNEL2.Repeation * 65536) + (TIM1IC_CHANNEL2.second - TIM1IC_CHANNEL2.first)));
    TIM1IC_CHANNEL2.time = 1000 / TIM1IC_CHANNEL2.Freq ;
	//	TIM1IC_CHANNEL2.Freq = 1000/TIM1IC_CHANNEL2.time; 
		
		TIM1IC_CHANNEL2.CompleteState = IC_Sleeping;
		TIM1IC_CHANNEL2.Repeation = 0;
		printf("t:%f",TIM1IC_CHANNEL2.Freq);
		return TIM1IC_CHANNEL2.Freq;
	}
	
}

void StartGetFreq(void)   //��������
{
	TIM1IC_CHANNEL2.CompleteState = 0X00;
	TIM_Cmd(TIM3, ENABLE);  //�ر�TIMx����
  TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC4,ENABLE);
}



