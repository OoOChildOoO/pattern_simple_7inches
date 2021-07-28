//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Bor_ScrollBar.h							*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision5 V5.21					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
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



