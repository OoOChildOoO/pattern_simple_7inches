//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  ltdc.h									*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.74					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*


#ifndef __LTDC_H   	  
#define __LTDC_H

#include "stm32cpu.h"




#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888��ʽ
#define LCD_PIXFORMAT_RGB888 		0X01			//ARGB8888��ʽ
#define LCD_PIXFORMAT_RGB565 		0X02 			//ARGB8888��ʽ
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB8888��ʽ
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB8888��ʽ
#define LCD_PIXFORMAT_L8			0X05			//ARGB8888��ʽ
#define LCD_PIXFORMAT_AL44			0X06			//ARGB8888��ʽ
#define LCD_PIXFORMAT_AL88 			0X07			//ARGB8888��ʽ



//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXFORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC0000000  





//LCD LTDC��Ҫ������
typedef struct  
{							 
	u32 pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	u32 pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	u16 hsw;			//ˮƽͬ�����
	u16 vsw;			//��ֱͬ�����
	u16 hbp;			//ˮƽ����
	u16 vbp;			//��ֱ����
	u16 hfp;			//ˮƽǰ��
	u16 vfp;			//��ֱǰ�� 
	u8 activelayer;		//��ǰ����:0/1	
	u8 dir;				//0,����;1,����;
	u16 width;			//LCD���
	u16 height;			//LCD�߶�
	u32 pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 	  
extern _ltdc_dev lcdltdc;							//����LCD LTDC����

extern void LTDC_Init(void);
U8 LTDC_Clk_Set(U32 pllsain,U32 pllsair,U32 pllsaidivr);
void LTDC_Layer_Parameter_Config(U8 layerx,U32 bufaddr,U8 pixformat,U8 alpha,U8 alpha0,U8 bfac1,U8 bfac2,U32 bkcolor);
void LTDC_Layer_Window_Config(U8 layerx,U16 sx,U16 sy,U16 width,U16 height);
void LTDC_Select_Layer(U8 layerx);
void LTDC_Layer_Switch(U8 layerx,U8 sw);


#endif
