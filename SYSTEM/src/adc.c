#include "adc.h"
#include "delay.h"
#include "timer.h"
/*
ADC转换
PA1捕捉转化
*/

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_Initstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	//PA1用于捕获正弦波和测量输出电阻
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//使能PA1
	
	//PA2用于测量输入电阻的ADC
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//使能PA2
  
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//APB2时钟频率为72MHz,72/6=12MHz，72/14（最大周期） = 5.14
	
	ADC_DeInit(ADC1);//复位ADC
	
	//配置ADC相关参数
	ADC_Initstruct.ADC_ContinuousConvMode=DISABLE;
	ADC_Initstruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_Initstruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_Initstruct.ADC_Mode=ADC_Mode_Independent;
	ADC_Initstruct.ADC_NbrOfChannel=1;
	ADC_Initstruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_Initstruct);//使能ADC1
	
//	//配置内容和ADC外设的具体对接函数   (ADC端口 ,ADC通道 ,转换序号-第几个转换 ,转换的周期)
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);  //PA1的ADC通道1          
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);  //PA2的ADC通道2
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_1Cycles5);  //PA3的ADC通道3 

	ADC_Cmd(ADC1,ENABLE);//开启ADc时钟
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}
u16  Get_Adc(u8 ADC_Channel_x)
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_x,1,ADC_SampleTime_1Cycles5);//ch为通道0到通道17
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ADC_Channel_x,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ADC_Channel_x);
		//Delay_us(5);
	}
	return temp_val/times;
}

//float GetFreq(void)
//{
//	u16  adcx;
//	float u=0;
//	adcx = Get_Adc_Average(ADC_Channel_1,10);//ADC的值，返回的是u16
//	u = (float)adcx*(3.3/4096);//电压值
//	return u;
//}

/*
Tout= ((arr+1)*(psc+1))/Tclk

其中arr是自动重装载值；

psc是预分频数；

Tclk是主频；不分频则为72MHz

Tout为中断溢出时间（进入中断）
*/

/*小修改的时候

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_Initstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	//PA1用于捕获正弦波
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA2用于测量输入电阻的ADC
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA3用于测量输出电阻的ADC
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//APB2时钟频率为72MHz,72/6=12MHz，72/14（最大周期） = 5.14
	
	ADC_DeInit(ADC1);//复位ADC
	
	//配置ADC相关参数
	ADC_Initstruct.ADC_ContinuousConvMode=DISABLE;
	ADC_Initstruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_Initstruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_Initstruct.ADC_Mode=ADC_Mode_Independent;
	ADC_Initstruct.ADC_NbrOfChannel=1;
	ADC_Initstruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_Initstruct);//使能ADC1
	
	
	//PA1的通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);            
	//配置内容和ADC外设的具体对接函数   (ADC端口 ,ADC通道 ,转换序号-第几个转换 ,转换的周期) 
	
	
	
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}
*/





















