//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  main.h									*
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


#ifndef __MAIN_H   	  
#define __MAIN_H

#include "stm32cpu.h"
#include "iap.h"

//------------------系统基本设置---------------------------------
extern U16 SYSDAT[];//系统名称
extern U32 SYSSETID;//系统ID
extern const U8 cpuidbuf[];

extern void checkcuid(U8 *buf);
extern HEXCHECK flashcheck;




extern volatile U8  usepageno;	
//------------------数据类型定义----------------------------------


extern volatile U32 BINDO;				// 输入状态
#define BKEYIN0			0x2000			// PC 13
#define BKEYIN1			0x4000			// PC 14
#define BKEYIN2			0x8000			// PC 15
#define BKEYIN3			0x0002			// PC 1
#define BKEYIN4			0x0040			// PC 6
#define BKEYIN5			0x0080			// PC 7
#define BKEYIN6			0x0010			// PC 4
#define BKEYIN7			0x0020			// PC 5
#define BMCIINS			0x0008			// PI 3
#define LEDFLAG			0x80000000		// 
#define SETKI0HO()		BINDO|=BKEYIN0
#define SETKI1HO()		BINDO|=BKEYIN1
#define SETKI2HO()		BINDO|=BKEYIN2
#define SETKI3HO()		BINDO|=BKEYIN3
#define SETKI4HO()		BINDO|=BKEYIN4
#define SETKI5HO()		BINDO|=BKEYIN5
#define SETKI6HO()		BINDO|=BKEYIN6
#define SETKI7HO()		BINDO|=BKEYIN7
#define SETMCIINSHO()	BINDO|=BMCIINS
#define SETLEDFLAG()	BINDO|=LEDFLAG
#define CLRKI0HO()		BINDO&=~BKEYIN0
#define CLRKI1HO()		BINDO&=~BKEYIN1
#define CLRKI2HO()		BINDO&=~BKEYIN2
#define CLRKI3HO()		BINDO&=~BKEYIN3
#define CLRKI4HO()		BINDO&=~BKEYIN4
#define CLRKI5HO()		BINDO&=~BKEYIN5
#define CLRKI6HO()		BINDO&=~BKEYIN6
#define CLRKI7HO()		BINDO&=~BKEYIN7
#define CLRMCIINSHO()	BINDO&=~BMCIINS
#define CLRLEDFLAG()	BINDO&=~LEDFLAG
#define KI0HO			(BINDO&BKEYIN0)
#define KI1HO			(BINDO&BKEYIN1)
#define KI2HO			(BINDO&BKEYIN2)
#define KI3HO			(BINDO&BKEYIN3)
#define KI4HO			(BINDO&BKEYIN4)
#define KI5HO			(BINDO&BKEYIN5)
#define KI6HO			(BINDO&BKEYIN6)
#define KI7HO			(BINDO&BKEYIN7)
#define MCIINSHO		(BINDO&BMCIINS)
#define WLEDFLAG		(BINDO&LEDFLAG)


#define   SPLINESUM_MAX	300//曲线最大入力控制点  2013.5.24







extern volatile U16 g_fno;
extern volatile U16 g_Fmax;

extern THDTSN	g_tdtensiond;
extern PRFOOT	g_pfootd;
extern PATHEAD	g_patd;
extern OLDPAT   g_dispat;
extern SystemTime g_SysTimes;
extern PassWordPara g_user;


extern void Menu_DesktopInit(void);


#endif



