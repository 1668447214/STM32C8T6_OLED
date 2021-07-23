//////////////////////////////////////////////////////////////////////////////////	 
//
//  文 件 名   : main.c
//  功能描述   : OLED 接口演示例程
//              说明: 
//              ----------------------------------------------------------------
//              VCC  接5V或3.3v电源
//              GND  电源地
//              D1   接PB15（SDA）
//              D0   接PB13（SCL）
//              CS   接PB12    
//              RST  接系统复位（一定要接为高电平，可接为PB14）
//              DC   接PB1
//              NC   悬空          
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   
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

 
//PC0~7,作为数据线
#define DATAOUT(x) GPIO_Write(GPIOC,x);//输出  
//使用4线串行接口时使用 


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据





//OLED控制用函数
void OLED_Showfloat(u8 x,u8 y,float flt);//浮点型显示
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);//开启显示
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);//使能OLED
void OLED_Clear(void);//清楚屏幕
void OLED_DrawPoint(u8 x,u8 y,u8 t);//画点
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);//写入字符串
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//写入数值
void OLED_ShowString(u8 x,u8 y, u8 *p);	 //写入一个字符
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);//写入中文字符
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void Date_face(void);//数据显示界面
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void WaveDisplay(void);//显示波形
void R_A_Display(void);//显示输入输出电阻和增益
void pri_R_A(void);//储存各个值

#endif  
	 



