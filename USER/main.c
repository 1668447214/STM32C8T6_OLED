#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "key.h"
#include "bmp.h"
#include "oled.h"
#include "timer.h"
#include "adc.h"
#include "fft256.h"
#include "tim.h"
#include "measure.h"

#define onetime 23
long qwer[256] = {0,
108,108,20,-31,33,158,214,154,68,76,187,287,275,182,134,204,324,366,295,207,
219,328,416,388,287,240,311,422,448,362,271,285,390,463,418,308,261,331,432,440,
341,247,262,362,420,359,242,197,267,358,350,240,146,166,261,306,232,113,73,146,
229,208,93,3,30,125,161,79,-38,-67,11,90,61,-54,-134,-96,2,32,-52,-163,
-179,-90,-11,-44,-157,-223,-170,-66,-38,-123,-223,-221,-121,-41,-75,-181,-231,-163,-52,-26,
-109,-196,-177,-66,14,-20,-120,-154,-70,43,67,-13,-89,-54,64,143,104,11,-9,84,
200,218,135,68,114,238,311,265,173,162,263,374,381,294,232,285,407,469,412,319,
314,416,518,510,414,354,411,526,573,502,405,402,502,590,566,460,400,456,563,592,
507,405,403,499,573,531,416,355,412,509,521,422,317,318,410,470,413,291,233,292,
381,378,270,166,172,263,313,244,120,69,133,218,205,91,-5,10,103,146,71,-49,
-89,-16,67,48,-64,-151,-122,-23,16,-60,-173,-197,-112,-27,-49,-158,-231,-185,-78,-40,
-117,-219,-225,-128,-40,-63,-167,-223,-161,-48,-11,-87,-178,-167,-58,29,4,-94,-136,-60,
57,90,13,-67,-43,73,158,127,32,1,86,203,229,148,73};
static void SystemInit(void);
extern long Date_U[Value];//��ѹ����
extern u16 Date_U0_AD[Value4];//������������ĵ�ѹ��Ż�����
int main(void)
{
	u8 key=0,mode=0;
//	int t;
//	float freq,adu,oled_y;
//	float THD = 0;
//	float Rin,Rout,A0;//���������������
//	u16 i=1,adcx;
  SystemInit();
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	pri_Data_U();//�������ݵ�ѹ����װ������
	
	delay_ms(100);
	
	fft_mag_tranf(qwer);
	test_fft();
 
	

	pri_R_A();//��������������裬����ѹ����
	
	
	WaveDisplay();
	
	Date_face();
	
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);
	while(1)
	{
		delay_ms(20);
		key = KEY_Scan(0);
		if(key == 1)
		{
			if(mode == 0)
			{
				OLED_Clear();
				R_A_Display();
				mode = 1;
			}
			else if(mode == 1)
			{
				OLED_Clear();
				WaveDisplay();
				Date_face();
				mode = 0;
			}
		
		}
	}
	
	
}

void SystemInit(void)
{
	delay_Init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
	LED_Init(); 
	USART_init(115200);
	OLED_Init();
	Adc_Init();
	TIM4_Init((onetime*Tout)-1,0); //�����װ��ֵ,23us��ʱ���жϣ���ת��ֵΪ1655��
	
	
	
	TIM3_PWN_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
////													//	PWMƵ��=��/720=100kHz,����Ϊ0.1us	
 	TIM_SetCompare2(TIM3,360);

	
	TIM2_Cap_Init(0XFFFF,0);//��ƵΪ72,72MHz/72=1MHz��1us
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  MOSFET_CONTROL_INIT();
	delay_ms(250);
	
}


