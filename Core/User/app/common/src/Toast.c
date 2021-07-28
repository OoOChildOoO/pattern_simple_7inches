//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Bor_Toast.c						*
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
//Description: ��ʼ����˾
//------------------------------------------
void ToastInit(void)
{
	g_Toast.isShow = 0;
	g_Toast.vIndex = 0;
	g_Toast.bColor = Color_Silver; //������ɫ
	g_Toast.tColor = Color_White;  //������ɫ
	if (P_LAN == 0)
	{ // ����
		g_textAddr = ((U16 *)(tmpflashadd[86]));
		g_textLen = 31;
	}
	else if (P_LAN == 1)
	{ //Ӣ��
		g_textAddr = ((U16 *)(tmpflashadd[136]));
		g_textLen = 61;
	}
}

//------------------------------------------
//Function: void ToastSetBackColor(void)
//Description: ������˾������ɫ
//------------------------------------------
void ToastSetBackColor(U16 color)
{
	g_Toast.bColor = color;
}

//------------------------------------------
//Function: void ToastSetTextColor(void)
//Description: ������˾������ɫ
//------------------------------------------
void ToastSetTextColor(U16 color)
{
	g_Toast.tColor = color;
}
//------------------------------------------
//Function: void ToastShow1(U16 xpos,U16 ypos,U16 dspox,U16 msgIndex)
//Description: ��˾��ʾ1
//para:
//	xpos,xpos-x,y�������,xdpox-x�������ֵ.����xpos,dspox������ʾ
//	msgIndex-��ʾ��������
//	vTime-��ʾʱ��
//------------------------------------------
void ToastShow1(U16 xpos, U16 ypos, U16 xdpox, U16 vTime)
{
	U16 *msgText;
	U16 msglen;
	U8 rows;
	U8 rowViews = 0;
	if (g_Toast.isShow)
		return; //��һ����Ϣ��ʾ��
	if (g_Toast.vIndex > TOAST_MSG_MAX)
		return; //��ʾ���ݲ���
	if (P_LAN < 2)
	{ //��Ӣ��
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
//Description: ��˾����
//------------------------------------
void ToastHide(void)
{
	if (g_Toast.isShow == 0)
		return;
	recoverAreaColor(g_Toast.xpos, g_Toast.ypos, g_Toast.xdpos, g_Toast.ydpos);
	g_Toast.isShow = 0;

	wdt();
}
