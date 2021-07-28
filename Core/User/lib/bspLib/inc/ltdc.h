//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  ltdc.h									*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision4 V4.74					*
//*  编程													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*


#ifndef __LTDC_H   	  
#define __LTDC_H

#include "stm32cpu.h"




#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888格式
#define LCD_PIXFORMAT_RGB888 		0X01			//ARGB8888格式
#define LCD_PIXFORMAT_RGB565 		0X02 			//ARGB8888格式
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB8888格式
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB8888格式
#define LCD_PIXFORMAT_L8			0X05			//ARGB8888格式
#define LCD_PIXFORMAT_AL44			0X06			//ARGB8888格式
#define LCD_PIXFORMAT_AL88 			0X07			//ARGB8888格式



//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXFORMAT_RGB565	
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0XC0000000  





//LCD LTDC重要参数集
typedef struct  
{							 
	u32 pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	u32 pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	u16 hsw;			//水平同步宽度
	u16 vsw;			//垂直同步宽度
	u16 hbp;			//水平后廊
	u16 vbp;			//垂直后廊
	u16 hfp;			//水平前廊
	u16 vfp;			//垂直前廊 
	u8 activelayer;		//当前层编号:0/1	
	u8 dir;				//0,竖屏;1,横屏;
	u16 width;			//LCD宽度
	u16 height;			//LCD高度
	u32 pixsize;		//每个像素所占字节数
}_ltdc_dev; 	  
extern _ltdc_dev lcdltdc;							//管理LCD LTDC参数

extern void LTDC_Init(void);
U8 LTDC_Clk_Set(U32 pllsain,U32 pllsair,U32 pllsaidivr);
void LTDC_Layer_Parameter_Config(U8 layerx,U32 bufaddr,U8 pixformat,U8 alpha,U8 alpha0,U8 bfac1,U8 bfac2,U32 bkcolor);
void LTDC_Layer_Window_Config(U8 layerx,U16 sx,U16 sy,U16 width,U16 height);
void LTDC_Select_Layer(U8 layerx);
void LTDC_Layer_Switch(U8 layerx,U8 sw);


#endif
