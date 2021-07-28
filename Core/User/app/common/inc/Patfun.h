//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Patfun.h 								*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2017��09��14�� 							*
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

#ifndef _PATFUN_H
#define _PATFUN_H

#include "stm32cpu.h"
#include "menu.h"
#include "main.h"



extern void wdt(void);

int sqr(U64 num);
U32 twopoint_dis(S32 xx,S32 yy);
U32 twopoint_dis1(S32 xx,S32 yy);
U32 twopoint_dis_zag(S32 xx,S32 yy);
U32 getpoint_sum(U32 dis,U8 temp_pitch,U8 extention,U8 flag);
U32 getpoint_sum2(U32 dis,U8 temp_pitch,U8 extention,U8 flag);
void verticalline_pos(U8 dir,U32 dis,S32 xx,S32 yy,U8 f);
void verticalline_pos1(U8 dir,U32 dis,S32 xx,S32 yy,U8 f);
void verticalline_pos2(U8 dir,U32 dis,S32 xx,S32 yy,U8 f);
void circle_center(S32 x0,S32 y0,S32 x1,S32 y1,COORDINATE *posi);
U8 lines_intersection(S32 k1,S32 k2);
U8 lines_intersection1(S32 k1,S32 k2);
void verticalline_pos_zag(U8 dir,U32 dis,S32 xx,S32 yy,U8 f);

#endif
