//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Bor_ScrollBar.c							*
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
#include "ScrollBar.h"
#include "lcddisp.h" 

ScrollBar g_ScrollBar;

//------------------------------------------
//Function: void ScrollBarInit(ScrollBar scrollBar)
//Description: 初始化滚动条
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
//Description: 设置滚动条背景颜色
//------------------------------------------
void ScrollBarSetBgColor(U16 color)
{
	g_ScrollBar.bgcolor = color;
}

//------------------------------------------
//Function: void ToastSetTextColor(void)
//Description: 设置滚动颜色
//------------------------------------------
void ScrollBarSetColor(U16 color)
{
	g_ScrollBar.color = color;
}

//------------------------------------------
//Function: void ScrollBarSetPos(U16 xpis,U16 ypis)
//Description: 设置滚动条起点坐标
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
//Description: 设置滚动条X方向大小
//------------------------------------------
void ScrollBarSetXsize(U16 dx)
{
	g_ScrollBar.dx = dx;
	
	g_ScrollBar.idx = g_ScrollBar.dx-2*g_ScrollBar.padx;	
	g_ScrollBar.ixpis = g_ScrollBar.xpis+g_ScrollBar.padx;
}

//------------------------------------------
//Function: void ScrollBarSetYsize(U16 dy)
//Description: 设置滚动条Y方向大小
//------------------------------------------
void ScrollBarSetYsize(U16 dy)
{
	g_ScrollBar.dy = dy;
	
	g_ScrollBar.idy = g_ScrollBar.dy-2*g_ScrollBar.pady;
	g_ScrollBar.iypis = g_ScrollBar.ypis+g_ScrollBar.pady;
}


//------------------------------------------
//Function: void ScrollBarSetXsize(U16 dx)
//Description: 设置滚动条X内边距大小
//------------------------------------------
void ScrollBarSetPadx(U16 padx)
{
	g_ScrollBar.padx = padx;
	
	g_ScrollBar.idx = g_ScrollBar.dx-2*g_ScrollBar.padx;	
	g_ScrollBar.ixpis = g_ScrollBar.xpis+g_ScrollBar.padx;
}

//------------------------------------------
//Function: void ScrollBarSetYsize(U16 dy)
//Description: 设置滚动条Y内边距大小
//------------------------------------------
void ScrollBarSetPady(U16 pady)
{
	g_ScrollBar.pady = pady;

	g_ScrollBar.idy = g_ScrollBar.dy-2*g_ScrollBar.pady;
	g_ScrollBar.iypis = g_ScrollBar.ypis+g_ScrollBar.pady;
}

//------------------------------------------
//Function: void ScrollBarSetMaxVaule(U16 mvalue)
//Description: 设置滚动条最大值
//------------------------------------------
void ScrollBarSetMaxVaule(U16 mvalue)
{
	g_ScrollBar.maxvalue = mvalue;
}

//------------------------------------------
//Function: void ScrollBarSetMaxVaule(U16 mvalue)
//Description: 获取滚动条最大值
//------------------------------------------
U16 ScrollBarGetMaxVaule(void)
{
	return g_ScrollBar.maxvalue;
}

//------------------------------------------
//Function: void ScrollBarSetVaule(U16 value)
//Description: 设置滚动条当前值
//------------------------------------------
void ScrollBarSetVaule(U16 value)
{
	g_ScrollBar.value = value;
}

//------------------------------------------
//Function: void ScrollBarSetVaule(U16 value)
//Description: 设置滚动条临界值
//------------------------------------------
void ScrollBarSetCirVaule(U16 crivalue)
{
	g_ScrollBar.crivalue = crivalue;
}

//------------------------------------------
//Function: void ScrollBarGetCirVaule(void)
//Description: 设置滚动条临界值
//------------------------------------------
U16 ScrollBarGetCirVaule(void)
{
	return g_ScrollBar.crivalue;
}

//------------------------------------------
//Function: void ScrollBarSetMindiv(U16 mdiv)
//Description: 设置滚动条最小滑块大小
//------------------------------------------
void ScrollBarSetMindiv(U16 mdiv)
{
	g_ScrollBar.mindiv = mdiv;
}

//------------------------------------------
//Function: void ScrollBarUpdateDiv()
//Description: 更新滚动条块大小
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
//Description: 滚动条绘制
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
//Description: 滚动条绘制
//------------------------------------------
void ScrollBarOnDrawByVal(U16 value)
{
	g_ScrollBar.value = value;
	ScrollBarOnDraw();	
}

//------------------------------------------
//Function: U16 ScrollBargetYsize(void)
//Description: 获取滚动条内侧Y大小
//------------------------------------------
U16 ScrollBargetIYsize(void)
{
	return g_ScrollBar.idy  ;
}

//------------------------------------------
//Function: U16 ScrollBargetDiv(void)
//Description: 获取滚动条显示滑块大小
//------------------------------------------
U16 ScrollBargetDiv(void)
{
	return g_ScrollBar.div ;
}

