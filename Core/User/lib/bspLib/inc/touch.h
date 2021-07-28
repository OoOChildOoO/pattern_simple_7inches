//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  touch.h									*
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

#ifndef __TOUCH_H 	 
#define __TOUCH_H

#include "stm32cpu.h"


#define	 INT32			signed int
#define  MATRIX_Type	INT32
#define  TPHARDWAREMD				// 使用硬件SPI


typedef struct
{
							/* This arrangement of values facilitates 
							*  calculations within getDisplayPoint() 
							*/
   MATRIX_Type    An;		/* A = An/Divider */
   MATRIX_Type    Bn;		/* B = Bn/Divider */
   MATRIX_Type    Cn;		/* C = Cn/Divider */
   MATRIX_Type    Dn;		/* D = Dn/Divider */
   MATRIX_Type    En;		/* E = En/Divider */
   MATRIX_Type    Fn;		/* F = Fn/Divider */
   MATRIX_Type    Divider ;
}MATRIX;

#define CHX		0x90		//通道X+的选择控制字 d0 	YPOSI
#define CHY		0xd0		//通道Y+的选择控制字 90		XPOSI
#define CHZ1	0xb0		//Z1
#define CHZ2	0xc0		//Z2

typedef struct
{ 
   INT32    x;
   INT32    y;
}POINT;

#define		OK		        0
#define		NOT_OK		   -1
#define TP_INT()	(WTPENIRQ)//(WTPIRQ)

extern MATRIX	Matrix[];

extern volatile U8  Touchfshflag;//触摸按下标志
extern volatile U16 Tpeess_z1;
extern volatile U16 Tpeess_z2;
extern volatile U16 Touch_x;
extern volatile U16 Touch_y;
extern volatile U16 Tposad_x;
extern volatile U16 Tposad_y;
extern volatile U16 Touch_pssz;

void Read_TchData(U8 mode);
U8 Touch_AutoGet_XY(void);



extern void Delayus( unsigned int k);
extern U16	Read_Z(void);
extern U16 Read_XY(U8 xy);
extern U8 TP_GetAdXY(U16 *x, U16 *y);
extern U8 TP_GetAdXY2(U16 *x, U16 *y, U32 delay);
extern U8 CheckKeyZone(U16 xx,U16 yy);
extern int getDisplayPoint( POINT * displayPtr, POINT * screenPtr, MATRIX * matrixPtr );
extern int setCalibrationMatrix( POINT * displayPtr2, POINT * screenPtr2);
extern void TouchParaOutPut(void);
extern void SaveTouchPara(void);
extern void RecoveTouchPara(void);
extern U8 ReadTouchPara(void);


extern U8 Touch_Get_XY(U32 Delay);
extern void ChkTouch(U32 k);
extern void TouchParaOutPut(void);

extern void DrawIconon(U16 x, U16 y, U32 Color);
extern void DrawIconoff(U16 x, U16 y);
extern void DrawIconon(U16 x, U16 y, U32 Color);
extern void DrawIcon(U16 x, U16 y, U32 Color);
extern void CLR_Screen (U32 Color);









#endif

