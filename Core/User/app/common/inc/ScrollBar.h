//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Bor_ScrollBar.h							*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision5 V5.21					*
//*  编程													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*
#ifndef _SCROLLBAR_H
#define _SCROLLBAR_H

#include "stm32cpu.h"

typedef struct _ScrollBar
{
	U16 xpis;
	U16 ypis;
	U16 dx;
	U16 dy;
	U16 padx;
	U16 pady;
	U16 bgcolor;
	U16 color;
	U16 value;
	U16 crivalue;
	U16 maxvalue;
	U16 mindiv;
	U16 div;
	U16 ixpis;
	U16 iypis;
	U16 idx;
	U16 idy;
	
}ScrollBar;

extern ScrollBar g_ScrollBar;

void ScrollBarInit(ScrollBar scrollBar);
void ScrollBarSetBgColor(U16 color);
void ScrollBarSetColor(U16 color);
void ScrollBarSetPos(U16 xpis,U16 ypis);
void ScrollBarSetXsize(U16 dx);
void ScrollBarSetYsize(U16 dy);
U16 ScrollBargetIYsize(void);
void ScrollBarSetPadx(U16 padx);
void ScrollBarSetPady(U16 pady);
void ScrollBarSetMaxVaule(U16 mvalue);
void ScrollBarSetVaule(U16 value);
U16 ScrollBarGetMaxVaule(void);
void ScrollBarSetCirVaule(U16 crivalue);
U16 ScrollBarGetCirVaule(void);
void ScrollBarSetMindiv(U16 mdiv);
void ScrollBarUpdateDiv(void);
void ScrollBarOnDraw(void);
void ScrollBarOnDrawByVal(U16 value);
U16 getIncreY(void);
U16 ScrollBargetDiv(void);

#endif



