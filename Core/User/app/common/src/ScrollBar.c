//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Bor_ScrollBar.c							*
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
#include "ScrollBar.h"
#include "lcddisp.h" 

ScrollBar g_ScrollBar;

//------------------------------------------
//Function: void ScrollBarInit(ScrollBar scrollBar)
//Description: ��ʼ��������
//------------------------------------------
void ScrollBarInit(ScrollBar scrollBar)
{
	g_ScrollBar.bgcolor = scrollBar.bgcolor;
	g_ScrollBar.color = scrollBar.color;
	g_ScrollBar.xpis = scrollBar.xpis;
	g_ScrollBar.ypis = scrollBar.ypis;
	g_ScrollBar.dx = scrollBar.dx;
	g_ScrollBar.dy = scrollBar.dy;
	g_ScrollBar.padx = scrollBar.padx;
	g_ScrollBar.pady = scrollBar.pady;
	g_ScrollBar.value = scrollBar.value;
	g_ScrollBar.crivalue = scrollBar.crivalue;
	g_ScrollBar.maxvalue = scrollBar.maxvalue;	
	g_ScrollBar.mindiv = scrollBar.mindiv;
	
	g_ScrollBar.ixpis = g_ScrollBar.xpis+g_ScrollBar.padx;
	g_ScrollBar.iypis = g_ScrollBar.ypis+g_ScrollBar.pady;
	g_ScrollBar.idx = g_ScrollBar.dx-2*g_ScrollBar.padx;
	g_ScrollBar.idy = g_ScrollBar.dy-2*g_ScrollBar.pady;
	
	ScrollBarUpdateDiv();
}

//------------------------------------------
//Function: void crollBarSetgColor(U16 color)
//Description: ���ù�����������ɫ
//------------------------------------------
void ScrollBarSetBgColor(U16 color)
{
	g_ScrollBar.bgcolor = color;
}

//------------------------------------------
//Function: void ToastSetTextColor(void)
//Description: ���ù�����ɫ
//------------------------------------------
void ScrollBarSetColor(U16 color)
{
	g_ScrollBar.color = color;
}

//------------------------------------------
//Function: void ScrollBarSetPos(U16 xpis,U16 ypis)
//Description: ���ù������������
//------------------------------------------
void ScrollBarSetPos(U16 xpis,U16 ypis)
{
	g_ScrollBar.xpis = xpis;
	g_ScrollBar.ypis = ypis;
	
	g_ScrollBar.ixpis = g_ScrollBar.xpis+g_ScrollBar.padx;
	g_ScrollBar.iypis = g_ScrollBar.ypis+g_ScrollBar.pady;
}

//------------------------------------------
//Function: void ScrollBarSetXsize(U16 dx)
//Description: ���ù�����X�����С
//------------------------------------------
void ScrollBarSetXsize(U16 dx)
{
	g_ScrollBar.dx = dx;
	
	g_ScrollBar.idx = g_ScrollBar.dx-2*g_ScrollBar.padx;	
	g_ScrollBar.ixpis = g_ScrollBar.xpis+g_ScrollBar.padx;
}

//------------------------------------------
//Function: void ScrollBarSetYsize(U16 dy)
//Description: ���ù�����Y�����С
//------------------------------------------
void ScrollBarSetYsize(U16 dy)
{
	g_ScrollBar.dy = dy;
	
	g_ScrollBar.idy = g_ScrollBar.dy-2*g_ScrollBar.pady;
	g_ScrollBar.iypis = g_ScrollBar.ypis+g_ScrollBar.pady;
}


//------------------------------------------
//Function: void ScrollBarSetXsize(U16 dx)
//Description: ���ù�����X�ڱ߾��С
//------------------------------------------
void ScrollBarSetPadx(U16 padx)
{
	g_ScrollBar.padx = padx;
	
	g_ScrollBar.idx = g_ScrollBar.dx-2*g_ScrollBar.padx;	
	g_ScrollBar.ixpis = g_ScrollBar.xpis+g_ScrollBar.padx;
}

//------------------------------------------
//Function: void ScrollBarSetYsize(U16 dy)
//Description: ���ù�����Y�ڱ߾��С
//------------------------------------------
void ScrollBarSetPady(U16 pady)
{
	g_ScrollBar.pady = pady;

	g_ScrollBar.idy = g_ScrollBar.dy-2*g_ScrollBar.pady;
	g_ScrollBar.iypis = g_ScrollBar.ypis+g_ScrollBar.pady;
}

//------------------------------------------
//Function: void ScrollBarSetMaxVaule(U16 mvalue)
//Description: ���ù��������ֵ
//------------------------------------------
void ScrollBarSetMaxVaule(U16 mvalue)
{
	g_ScrollBar.maxvalue = mvalue;
}

//------------------------------------------
//Function: void ScrollBarSetMaxVaule(U16 mvalue)
//Description: ��ȡ���������ֵ
//------------------------------------------
U16 ScrollBarGetMaxVaule(void)
{
	return g_ScrollBar.maxvalue;
}

//------------------------------------------
//Function: void ScrollBarSetVaule(U16 value)
//Description: ���ù�������ǰֵ
//------------------------------------------
void ScrollBarSetVaule(U16 value)
{
	g_ScrollBar.value = value;
}

//------------------------------------------
//Function: void ScrollBarSetVaule(U16 value)
//Description: ���ù������ٽ�ֵ
//------------------------------------------
void ScrollBarSetCirVaule(U16 crivalue)
{
	g_ScrollBar.crivalue = crivalue;
}

//------------------------------------------
//Function: void ScrollBarGetCirVaule(void)
//Description: ���ù������ٽ�ֵ
//------------------------------------------
U16 ScrollBarGetCirVaule(void)
{
	return g_ScrollBar.crivalue;
}

//------------------------------------------
//Function: void ScrollBarSetMindiv(U16 mdiv)
//Description: ���ù�������С�����С
//------------------------------------------
void ScrollBarSetMindiv(U16 mdiv)
{
	g_ScrollBar.mindiv = mdiv;
}

//------------------------------------------
//Function: void ScrollBarUpdateDiv()
//Description: ���¹��������С
//------------------------------------------
void ScrollBarUpdateDiv(void)
{
	if(g_ScrollBar.crivalue>=g_ScrollBar.maxvalue){
		g_ScrollBar.div = g_ScrollBar.idy ;
	}else{
		g_ScrollBar.div = g_ScrollBar.idy /(g_ScrollBar.maxvalue-(g_ScrollBar.crivalue-1))+((g_ScrollBar.idy %(g_ScrollBar.maxvalue-(g_ScrollBar.crivalue-1)))?1:0);
		if(g_ScrollBar.div<g_ScrollBar.mindiv){
			g_ScrollBar.div = g_ScrollBar.mindiv;
		}	
	}
}

//------------------------------------------
//Function: void ScrollBarOnDraw()
//Description: ����������
//------------------------------------------
void ScrollBarOnDraw(void)
{	
	//printf("g_ScrollBar.div = %d \r\n",g_ScrollBar.div);
	DrawFillRect(g_ScrollBar.xpis,g_ScrollBar.ypis,g_ScrollBar.dx,g_ScrollBar.dy,g_ScrollBar.bgcolor);
	DrawFillRect(g_ScrollBar.ixpis,g_ScrollBar.iypis+getIncreY(),g_ScrollBar.idx,g_ScrollBar.div,g_ScrollBar.color);		
}

U16 getIncreY(void)
{
	U16 incrementY = 0;
	if(g_ScrollBar.value>g_ScrollBar.crivalue) incrementY = (g_ScrollBar.value-g_ScrollBar.crivalue)*(g_ScrollBar.idy-g_ScrollBar.div)/(g_ScrollBar.maxvalue-g_ScrollBar.crivalue);
	//printf("incrementY = %d  value = %d  maxvalue = %d crivalue = %d \r\n",incrementY,g_ScrollBar.value,g_ScrollBar.maxvalue,g_ScrollBar.crivalue);
	return incrementY;
}
//------------------------------------------
//Function: void ScrollBarOnDraw()
//Description: ����������
//------------------------------------------
void ScrollBarOnDrawByVal(U16 value)
{
	g_ScrollBar.value = value;
	ScrollBarOnDraw();	
}

//------------------------------------------
//Function: U16 ScrollBargetYsize(void)
//Description: ��ȡ�������ڲ�Y��С
//------------------------------------------
U16 ScrollBargetIYsize(void)
{
	return g_ScrollBar.idy  ;
}

//------------------------------------------
//Function: U16 ScrollBargetDiv(void)
//Description: ��ȡ��������ʾ�����С
//------------------------------------------
U16 ScrollBargetDiv(void)
{
	return g_ScrollBar.div ;
}

