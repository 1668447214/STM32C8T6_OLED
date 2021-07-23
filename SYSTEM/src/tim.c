#include "stm32f10x.h"
#include "tim.h"
#include "adc.h"
#include "stdio.h"


u16 Date_U0_AD[Value4];
u16 n = 0;
void TIM4_Init(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,DISABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		Date_U0_AD[n] = Get_Adc(ADC_Channel_2);		
//			Date_U[N] = (float)adcx*(3.3/4096);//��ѹֵ
		n++;
		if(n == Value4)
		{
			TIM_Cmd(TIM4, DISABLE);  //�ر�TIMx����,ʱ��Ҳ���ر��ˣ��ǵÿ���ʱ��
			TIM_ITConfig(TIM4,TIM_IT_Update  |TIM_IT_Trigger,DISABLE);
			n = 0;
		}
	}
}
/*
	������ʱ��4�ж�
*/
void TIM4_start(void)
{
	TIM_Cmd(TIM4, ENABLE);  //�ر�TIMx����
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
}

/*
	�ҳ����ֵ��Сֵ
*/
float U0_AD_Max_Min(void)//���ط�-��ֵ
{
	float U0=0;
	u16 i=0,ADC_Max=0,ADC_Min=4095;
	for(i=0;i<Value4;i++)
	{
			if(ADC_Max < Date_U0_AD[i])
			{
				ADC_Max = Date_U0_AD[i];//ȡ�����ֵ
			}
			if(ADC_Min > Date_U0_AD[i])
			{
				ADC_Min = Date_U0_AD[i];//ȡ����Сֵ
			}
	}
//	delay_ms(200);
	U0 = ((float)(ADC_Max - ADC_Min)/4095)*3.3;
	return U0;
}

void TIM4_AD_U1U2(void)//�ѵ�ѹ����Ū�����飬�ö�ʱ���ж�
{
	u16 arr;//ADֵ����װ��ֵ
	float onretime = 23;
	arr = (onretime*Tout)-1;//�����װ��ֵ,23us��ʱ���жϣ���ת��ֵΪ1655��
	TIM4_Init(arr,0);//23us
}



