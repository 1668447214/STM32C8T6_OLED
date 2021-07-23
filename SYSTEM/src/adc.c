#include "adc.h"
#include "delay.h"
#include "timer.h"
/*
ADCת��
PA1��׽ת��
*/

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_Initstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	//PA1���ڲ������Ҳ��Ͳ����������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//ʹ��PA1
	
	//PA2���ڲ�����������ADC
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//ʹ��PA2
  
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//APB2ʱ��Ƶ��Ϊ72MHz,72/6=12MHz��72/14��������ڣ� = 5.14
	
	ADC_DeInit(ADC1);//��λADC
	
	//����ADC��ز���
	ADC_Initstruct.ADC_ContinuousConvMode=DISABLE;
	ADC_Initstruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_Initstruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_Initstruct.ADC_Mode=ADC_Mode_Independent;
	ADC_Initstruct.ADC_NbrOfChannel=1;
	ADC_Initstruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_Initstruct);//ʹ��ADC1
	
//	//�������ݺ�ADC����ľ���ԽӺ���   (ADC�˿� ,ADCͨ�� ,ת�����-�ڼ���ת�� ,ת��������)
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);  //PA1��ADCͨ��1          
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);  //PA2��ADCͨ��2
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_1Cycles5);  //PA3��ADCͨ��3 

	ADC_Cmd(ADC1,ENABLE);//����ADcʱ��
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}
u16  Get_Adc(u8 ADC_Channel_x)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_x,1,ADC_SampleTime_1Cycles5);//chΪͨ��0��ͨ��17
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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
//	adcx = Get_Adc_Average(ADC_Channel_1,10);//ADC��ֵ�����ص���u16
//	u = (float)adcx*(3.3/4096);//��ѹֵ
//	return u;
//}

/*
Tout= ((arr+1)*(psc+1))/Tclk

����arr���Զ���װ��ֵ��

psc��Ԥ��Ƶ����

Tclk����Ƶ������Ƶ��Ϊ72MHz

ToutΪ�ж����ʱ�䣨�����жϣ�
*/

/*С�޸ĵ�ʱ��

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_Initstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	//PA1���ڲ������Ҳ�
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA2���ڲ�����������ADC
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA3���ڲ�����������ADC
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//APB2ʱ��Ƶ��Ϊ72MHz,72/6=12MHz��72/14��������ڣ� = 5.14
	
	ADC_DeInit(ADC1);//��λADC
	
	//����ADC��ز���
	ADC_Initstruct.ADC_ContinuousConvMode=DISABLE;
	ADC_Initstruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_Initstruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_Initstruct.ADC_Mode=ADC_Mode_Independent;
	ADC_Initstruct.ADC_NbrOfChannel=1;
	ADC_Initstruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_Initstruct);//ʹ��ADC1
	
	
	//PA1��ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);            
	//�������ݺ�ADC����ľ���ԽӺ���   (ADC�˿� ,ADCͨ�� ,ת�����-�ڼ���ת�� ,ת��������) 
	
	
	
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}
*/





















