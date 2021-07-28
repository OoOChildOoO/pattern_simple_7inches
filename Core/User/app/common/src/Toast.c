//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Bor_Toast.c						*
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

#include "Toast.h"
#include "function.h"
#include "register.h"
#include "lcddisp.h"
#include "color.h"
#include "utility.h"

extern volatile U32 tmpflashadd[200];
extern U16 getStrLen(U8 *buf);
extern void backupAreaColor(U16 x, U16 y, U16 dx, U16 dy);
extern void recoverAreaColor(U16 x, U16 y, U16 dx, U16 dy);

TOAST g_Toast;

U16 *g_textAddr;
U16 g_textLen;
U16 g_textSheet;
//------------------------------------------
//Function: void ToastInit(void)
//Description: 初始化吐司
//------------------------------------------
void ToastInit(void)
{
	g_Toast.isShow = 0;
	g_Toast.vIndex = 0;
	g_Toast.bColor = Color_Silver; //背景颜色
	g_Toast.tColor = Color_White;  //字体颜色
	if (P_LAN == 0)
	{ // 中文
		g_textAddr = ((U16 *)(tmpflashadd[86]));
		g_textLen = 31;
	}
	else if (P_LAN == 1)
	{ //英文
		g_textAddr = ((U16 *)(tmpflashadd[136]));
		g_textLen = 61;
	}
}

//------------------------------------------
//Function: void ToastSetBackColor(void)
//Description: 设置吐司背景颜色
//------------------------------------------
void ToastSetBackColor(U16 color)
{
	g_Toast.bColor = color;
}

//------------------------------------------
//Function: void ToastSetTextColor(void)
//Description: 设置吐司字体颜色
//------------------------------------------
void ToastSetTextColor(U16 color)
{
	g_Toast.tColor = color;
}
//------------------------------------------
//Function: void ToastShow1(U16 xpos,U16 ypos,U16 dspox,U16 msgIndex)
//Description: 吐司显示1
//para:
//	xpos,xpos-x,y区域起点,xdpox-x区域最大值.根据xpos,dspox居中显示
//	msgIndex-显示内容索引
//	vTime-显示时间
//------------------------------------------
void ToastShow1(U16 xpos, U16 ypos, U16 xdpox, U16 vTime)
{
	U16 *msgText;
	U16 msglen;
	U8 rows;
	U8 rowViews = 0;
	if (g_Toast.isShow)
		return; //上一条信息显示中
	if (g_Toast.vIndex > TOAST_MSG_MAX)
		return; //显示数据不对
	if (P_LAN < 2)
	{ //中英文
		msgText = g_textAddr + (g_Toast.vIndex) * g_textLen;
		msglen = getStrLen((U8 *)msgText);
		rows = (msglen * 13) / (xdpox - 16) + ((msglen * 13) % (xdpox - 16) ? 1 : 0);
		if (rows == 1)
		{
			g_Toast.xpos = xpos + (xdpox - (msglen * 13)) / 2;
			g_Toast.xdpos = msglen * 13 + 16;
			rowViews = msglen;
		}
		else
		{
			g_Toast.xpos = xpos;
			g_Toast.xdpos = xdpox;
			rowViews = g_Toast.xdpos / 13;
		}
		g_Toast.ypos = ypos;
		g_Toast.ydpos = 16 + 24 * rows + 4 * (rows - 1);
	}

	backupAreaColor(g_Toast.xpos, g_Toast.ypos, g_Toast.xdpos, g_Toast.ydpos);
	DrawFillRect(g_Toast.xpos, g_Toast.ypos, g_Toast.xdpos, g_Toast.ydpos, g_Toast.bColor);
	Draw24x24MulStr(g_Toast.xpos + 8, g_Toast.ypos + 8, g_textAddr + (g_Toast.vIndex) * g_textLen, rowViews, g_Toast.vIndex, 0, 0, g_Toast.tColor);
	g_Toast.curTime = 0;
	g_Toast.viewTime = vTime;
	g_Toast.isShow = 1;

	wdt();
}

//------------------------------------------
//Function: void ToastHide(void)
//Description: 吐司隐藏
//------------------------------------
void ToastHide(void)
{
	if (g_Toast.isShow == 0)
		return;
	recoverAreaColor(g_Toast.xpos, g_Toast.ypos, g_Toast.xdpos, g_Toast.ydpos);
	g_Toast.isShow = 0;

	wdt();
}
