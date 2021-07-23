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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,DISABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		Date_U0_AD[n] = Get_Adc(ADC_Channel_2);		
//			Date_U[N] = (float)adcx*(3.3/4096);//电压值
		n++;
		if(n == Value4)
		{
			TIM_Cmd(TIM4, DISABLE);  //关闭TIMx外设,时钟也被关闭了，记得开启时钟
			TIM_ITConfig(TIM4,TIM_IT_Update  |TIM_IT_Trigger,DISABLE);
			n = 0;
		}
	}
}
/*
	开启定时器4中断
*/
void TIM4_start(void)
{
	TIM_Cmd(TIM4, ENABLE);  //关闭TIMx外设
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
}

/*
	找出最大值最小值
*/
float U0_AD_Max_Min(void)//返回峰-峰值
{
	float U0=0;
	u16 i=0,ADC_Max=0,ADC_Min=4095;
	for(i=0;i<Value4;i++)
	{
			if(ADC_Max < Date_U0_AD[i])
			{
				ADC_Max = Date_U0_AD[i];//取出最大值
			}
			if(ADC_Min > Date_U0_AD[i])
			{
				ADC_Min = Date_U0_AD[i];//取出最小值
			}
	}
//	delay_ms(200);
	U0 = ((float)(ADC_Max - ADC_Min)/4095)*3.3;
	return U0;
}

void TIM4_AD_U1U2(void)//把电压数据弄进数组，用定时器中断
{
	u16 arr;//AD值和重装载值
	float onretime = 23;
	arr = (onretime*Tout)-1;//算出重装载值,23us定时器中断，重转载值为1655，
	TIM4_Init(arr,0);//23us
}



