//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Patfun.h 								*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2017年09月14日 							*
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
