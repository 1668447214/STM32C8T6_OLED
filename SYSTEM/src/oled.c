 //******************************************************************************/

#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "adc.h"
#include "timer.h"
#include "fft256.h"
#include "measure.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   




/* a serial of data(float*10) */
float data[64] = {20,     23,     27,     31,     34,     36,     38,     39,     39,     39,     38,     36,     34,     31,     27,
        23,     20,     17,     13,     9,      6,      4,      2,      1,      1,      1,      2,      4,      6,
        9,      13,     17,     20};

u8 OLED_GRAM[128][8];


//float Date_U_OLED[Value];//��ѹ���ݣ���ʾ��	
				
				
//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

#if OLED_MODE==1
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
//		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
//		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //������ʾ
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ

}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)//1.0
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}

//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)//2.0
//{      			    
//	u8 temp,t,t1;
//	u8 y0=y;
//	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
//    for(t=0;t<csize;t++)
//    {   
//		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
//		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
//		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
//		else return;								//û�е��ֿ�
//        for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp<<=1;
//			y++;
//			if((y-y0)==size)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}  	 
//    }          
//}

//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

//void OLED_Showfloat(u8 x,u8 y,float flt)
//{
//	u16 integer,decimal1,decimal2;//������С������
//}
 void OLED_Showfloat(u8 x,u8 y,float flt)//��������ʾ
  {
	u8 num[20] = {0};
	uint16_t temp = flt;
	int16_t i = 0;
	uint16_t n = 0;
	//num[n++] = ((uint16_t)(flt*1000))%10+48;
	num[n++] = ((uint16_t)(flt*100))%10+48;
	num[n++] = ((uint16_t)(flt*10))%10+48;
	num[n++] = '.';
	
	while(1)
	{
		num[n++] = (temp %10)+48;
		temp = temp/10;
		if(temp==0)
		break;
	}
	n--;
	
	while(1)
	{
		temp = num[i++];
		num[i-1] = num[n--];
		num[n+1] = temp;
		if(n==i||n-i<0)
		break;
	}
	OLED_ShowString(x,y,num);
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��A�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	



 
  OLED_RST_Set();//��λʹ��ϵͳ��λ�����д˴�����Ҫ�ٸ�λ
	delay_ms(10);
	OLED_RST_Clr();
	delay_ms(10);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)  
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
//void OLED_ShowChar(u8 x,u8 y,u8 chr)//1.0
extern float ADC_Max,ADC_Min;//��ѹ���ֵ��Сֵ
void Date_face(void)
{
//	u8 i=0;
	float ad_u;
//	u16 integer,decimal1,decimal2;//��ѹ��������С������
		
	OLED_ShowChar(0,0,'+');
	OLED_ShowChar(0,2,'0');
//	for(i=1;i<15;i++)//ԭ��������
//	{
//		OLED_ShowChar(8*i,2,'-');
//	}
	OLED_ShowChar(0,4,'-');
	OLED_ShowString(0,6,"U:");
	
	OLED_ShowString(50,6,"V");
	OLED_ShowString(60,6,"f:");
  OLED_Showfloat(68,6,(float)getFreq_fft()/1000.0);
	OLED_ShowString(100,6,"k");
	
	ad_u = ADC_Max - ADC_Min;//������ڷ�-��ֵ
	ad_u =( (float)(ad_u/4095)*3.3)*5/3;//ADֵת��Ϊ��ѹֵ
	OLED_Showfloat(15,6,ad_u);
	
//	OLED_Showfloat(80,6,freq);//��ʾ��ǰƵ�ʣ�GetFreq�������Ƶ��ֵ

//	OLED_ShowString(116,6,"Hz");
//}
}
void WaveDisplay(void)
	{
		int i = 0;
		long *Data_OLED;
		float	oled_y;
		//static float oled_y[Value];
		Data_OLED = Re_Data_U();		
		for(i = 0;i<96;i++)//��һ������
		{
			//oled_y = (float);
			oled_y = (float)(Data_OLED[i])*(3.3/4096);//��ѹֵ
			//printf("RRRRRRRRR ===== %f\r\n",oled_y);
			oled_y *= 12;
			OLED_DrawPoint(i+16,oled_y+23,1);
		}
		OLED_Refresh_Gram();//������ʾ------------
	}


//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}

float Rin,Rout,A0,THD;//���������������,�����THD

void pri_R_A(void)
{
	
	
	THD = THD_Cal();
	Rin = Measure_Rin()/1000;//�������
	delay_ms(50);
	Rout = Measure_Rout()/1000;//�������
	delay_ms(50);
	A0 = Measure_gain();
	delay_ms(50);
}

void R_A_Display(void)
{
	OLED_ShowString(0,0,"Rin");
	OLED_Showfloat(60,0,Rin);
	OLED_ShowString(100,0,"K");
	
	OLED_ShowString(0,2,"Rout");
	OLED_Showfloat(60,2,Rout);
	OLED_ShowString(100,2,"K");
	
	OLED_ShowString(0,4,"A0");
	OLED_Showfloat(60,4,A0);
	
	OLED_ShowString(0,6,"THD");
	OLED_Showfloat(60,6,THD);
	OLED_ShowString(100,6,"%");
}








