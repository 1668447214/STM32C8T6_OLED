#include "fft256.h"
#include "math.h"
#include "stm32f10x.h"
#include "stdio.h"
#define NPT 256

/*
*void GetPowerMag()
{
    unsigned short i;
	  float  X,Y,P;
	 	c_abs(FFT_256PointIn,FFT_256PointOut,N/2);
    for(i=0; i<N/2; i++)
    {
			  X = FFT_256PointIn[i].real/N;    //计算实部
			  Y = FFT_256PointIn[i].imag/N;    //计算虚部
				if(i==0)
					Mag[i] = FFT_256PointOut[i]/N;    //计算幅值
				else
					Mag[i] = FFT_256PointOut[i]*2/N;
			  P = atan2(Y,X)*180/PI;           //计算相位
			
				printf("%d      ",i);
				printf("%d      ",F*i); 
				printf("%f      \r\n",Mag[i]);		
    }
}

//计算欧总谐振失真函数

void GetTHD()
{
	unsigned short i=20;//控制字符平移
	float  Uo1,Uo2,Uo3,Uo4,Uo5;
	double THD,thd_fz=0,thd_fm=0;
	Uo1=Mag[100];
	Uo2=Mag[200];
	Uo3=Mag[300];
	Uo4=Mag[400];
	Uo5=Mag[500];
	thd_fm=Uo1;
	thd_fz=Uo2*Uo2 + Uo3*Uo3 + Uo4*Uo4 + Uo5*Uo5;
	thd_fz=sqrt(thd_fz);
	THD=thd_fz/thd_fm*100;
	sprintf((unsigned char *)temp1,"Uo1:%5lfV",Uo1);
	LCD_ShowString(180,0+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo2:%5lfV",Uo2);
	LCD_ShowString(180,15+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo3:%5lfV",Uo3);
	LCD_ShowString(180,30+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo4:%5lfV",Uo4);
	LCD_ShowString(180,45+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo5:%5lfV",Uo5);
	LCD_ShowString(180,60+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"THD:%5lf%%",THD);
	LCD_ShowString(180,75+i,200,16,16,temp1);
	//测试
//	printf("%lf      ",Uo1);
//	printf("%lf      ",Uo2);
//	printf("%lf      ",Uo3);
//	printf("%lf      ",Uo4);
//	printf("%lf      ",Uo5);
}

*
*
*
*
*/



uint32_t adc_buf[NPT]={0};
long lBufOutArray[NPT/2];
long lBufMagArray[NPT/2];

// Fs = 40k   number of point = 256   Fn = fs *(n-1) /256   d(f) = 156.25hz     F(1k) = OutArray[6]
void fft_tranf(long *in)  //256 points
{
	cr4_fft_256_stm32(lBufOutArray, in, NPT);
}

void Mag_tranf()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
				//除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}

void fft_mag_tranf(long *in)
{
	fft_tranf(in);
	Mag_tranf();
}
uint16_t getFreq_fft(void)
{
	uint8_t i = 0;
	uint8_t max_w = 1;
  for(i =1;i<127;i++)
	{
			if(lBufMagArray[i] >= lBufMagArray[max_w])
				max_w = i;
	}
	max_w++;
	return (uint16_t)(max_w * 156.25);
}

//float THD_Cal()
//{
//	
////	float THD = 0;
////	uint16_t i;
////	for(i = 0;i<128;i++)
////		THD += (lBufMagArray[i] / lBufMagArray[6]) * (lBufMagArray[i] / lBufMagArray[6]);
////	THD = sqrt(THD);
////	return THD;
////000	printf("THD_Cal 3.0 = %f",THD);
//	float THD;
//	uint16_t i;
// THD = 0;
// 
// for(i = 1;i<128;i++)
//  THD += ((float)(lBufMagArray[i] / lBufMagArray[6])) * ((float)(lBufMagArray[i] / lBufMagArray[6]));
//	printf("THD_Cal 3.0 = %f",THD);
// THD--;
//	printf("THD_Cal 3.0 = %f",THD);
// THD = sqrt(THD);
//	printf("THD_Cal 3.0 = %f",THD);
// return THD * 100;
//}

double THD_Cal()
{
 double THD;
 uint16_t i;
 THD = 0;
 for(i = 1;i<128;i++)
		THD += ((double)(lBufMagArray[i] / (double)lBufMagArray[6])) * ((double)(lBufMagArray[i] / (double)lBufMagArray[6]));
 THD--;
 THD =  sqrt(THD);
 return THD *100;
}

void test_fft(void)
{
	uint16_t i = 0;
	float fft_adc = 0;
	for(i = 0;i<128;i++)
	{
		fft_adc = lBufMagArray[i] *3.3 / 4096;
		printf("%f\r\n",fft_adc);
	}
	//printf("%f\r\n",THD_Cal());
//	int i;
// float fft_adc;
// for(i = 0;i<128;i++)
// {
//  fft_adc = lBufMagArray[i] *3.3 / 4096;
//  printf("%f\r\n",fft_adc);
// }
 //printf("%f\r\n",THD_Cal());
}


