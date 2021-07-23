//////////////////////////////////////////////////////////////////////////////////	 
//
//  �� �� ��   : main.c
//  ��������   : OLED �ӿ���ʾ����
//              ˵��: 
//              ----------------------------------------------------------------
//              VCC  ��5V��3.3v��Դ
//              GND  ��Դ��
//              D1   ��PB15��SDA��
//              D0   ��PB13��SCL��
//              CS   ��PB12    
//              RST  ��ϵͳ��λ��һ��Ҫ��Ϊ�ߵ�ƽ���ɽ�ΪPB14��
//              DC   ��PB1
//              NC   ����          
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_12)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_14)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_14)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)//DC  A0
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//CLK  SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//DIN  SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

 
//PC0~7,��Ϊ������
#define DATAOUT(x) GPIO_Write(GPIOC,x);//���  
//ʹ��4�ߴ��нӿ�ʱʹ�� 


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����





//OLED�����ú���
void OLED_Showfloat(u8 x,u8 y,float flt);//��������ʾ
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);//������ʾ
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);//ʹ��OLED
void OLED_Clear(void);//�����Ļ
void OLED_DrawPoint(u8 x,u8 y,u8 t);//����
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);//д���ַ���
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//д����ֵ
void OLED_ShowString(u8 x,u8 y, u8 *p);	 //д��һ���ַ�
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);//д�������ַ�
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void Date_face(void);//������ʾ����
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void WaveDisplay(void);//��ʾ����
void R_A_Display(void);//��ʾ����������������
void pri_R_A(void);//�������ֵ

#endif  
	 



