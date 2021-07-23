#include "timer.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "stdio.h"
#include "delay.h"

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

#define TIMxClock 72000  //kHz
#define TIMxClock1 TIMxClock * 1000
ICStruct TIM1IC_CHANNEL2 = {0,0,0,0,0,0};



void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

/*
Tout= ((arr+1)*(psc+1))/Tclk

其中arr是自动重装载值；

psc是预分频数；

Tclk是主频；不分频则为72MHz

Tout为中断溢出时间（进入中断）
*/
u16 N=0;//数组索引
long Date_U[Value] = {0};//电压AD值数据
float ADC_Max=0,ADC_Min=4095;//电压最大值最小值
void TIM3_IRQHandler(void)   //TIM3中断,把实时电压值写入数组
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		Date_U[N] = Get_Adc(ADC_Channel_1);		
//			Date_U[N] = (float)adcx*(3.3/4096);//电压值
		N++;
		if(N == Value)
		{
			TIM_Cmd(TIM3, DISABLE);  //关闭TIMx外设,时钟也被关闭了，记得开启时钟
			TIM_ITConfig(TIM3,TIM_IT_Update  |TIM_IT_Trigger,DISABLE);
			N = 0;
		}
	}
}

void pri_Data_U(void)//把电压数据弄进数组，用定时器中断
{
	u16 arr;//AD值和重装载值
	float onretime = 23;
	arr = (onretime*Tout)-1;//算出重装载值,23us定时器中断，重转载值为1655，
	TIM3_Int_Init(arr,0);//23us
}

/*
	找出最大值最小值
*/
float Max_Min(void)
{
	u16 i=0;
	for(i=0;i<Value;i++)
	{
			if(ADC_Max < Date_U[i])
			{
				ADC_Max = Date_U[i];//取出最大值
			}
			if(ADC_Min > Date_U[i])
			{
				ADC_Min = Date_U[i];//取出最小值
			}
	}
	 return ((ADC_Max - ADC_Min));
}
/*
	返回电压数据的数组
*/
long* Re_Data_U(void)
{
	return Date_U;
}



//PWM输出初始化，测试用，可不要
//arr：自动重装值
//psc：时钟预分频数

void TIM3_PWN_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}
//----------------------------------------------------------------------------------------------------
//定时器2通道1输入捕获配置

TIM_ICInitTypeDef  TIM2_ICInitStructure;

void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);						 //PA0 上拉
	
	TIM2->CCMR1 = 0X0100;
  TIM2->SR = 0X00;
	TIM2->ARR = 0XFFFF;
	TIM2->PSC = 0X00;
	TIM2->CCER = 0X10;
	TIM2->CR1 = 0X01;
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update | TIM_IT_CC4);
  TIM_Cmd(TIM2,ENABLE ); 	//使能定时器5
   


}

u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
 
void startCapture()
{
	TIM2->SR = 0x00;
	TIM2->DIER = 0x11;
	TIM1IC_CHANNEL2.CompleteState = 0x00;
	TIM1IC_CHANNEL2.Repeation = 0x00;
}

void TIM2_IRQHandler(void)//中断要和启动文件里面的一抹一眼，否则会出现死循环.
{ 

 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET && TIM1IC_CHANNEL2.CompleteState != 2) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM1IC_CHANNEL2 .Repeation++;	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		LED0=!LED0;
		}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC4) != RESET)
	{
	 if(TIM1IC_CHANNEL2.CompleteState == 0)
	 {
		 TIM1IC_CHANNEL2.first = TIM2->CCR4;//定时器计数值
		 TIM1IC_CHANNEL2.CompleteState++;
	 }
	 else if(TIM1IC_CHANNEL2.CompleteState == 1)
	 {
     TIM1IC_CHANNEL2.second = TIM2->CCR4;//定时器计数值
		 TIM1IC_CHANNEL2.CompleteState++;
	 }
	 else          //When CompleteState == 2, IC hava completed ,Waiting calculate
	 {
		 TIM2->DIER = 0X00;
	 }
	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
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

void StartGetFreq(void)   //开启捕获
{
	TIM1IC_CHANNEL2.CompleteState = 0X00;
	TIM_Cmd(TIM3, ENABLE);  //关闭TIMx外设
  TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC4,ENABLE);
}



