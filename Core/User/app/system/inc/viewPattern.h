/*
 * @Author: your name
 * @Date: 2021-03-11 13:54:55
 * @LastEditTime: 2021-06-29 17:02:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\viewPattern.h
 */
//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  viewPattern.h							*
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

#ifndef _VIEW_PATTERN_H
#define _VIEW_PATTERN_H



#include "stm32cpu.h"

#define PAT_FM_L 0x0001 //低速缝纫
#define PAT_FM_MD2 0x0021 //中低速缝纫
#define PAT_FM_MD1 0x0041 //中高速缝纫
#define PAT_FM_H 0x0061 //高速缝纫
#define PAT_FM_TRIM 0x0002 //剪线
#define PAT_FM_EMPTY 0x0003 //移送(空松)
#define PAT_FM_EMP_ED 0x001B //移送完
#define PAT_FM_F1 0x0005//功能-1
#define PAT_FM_F2 0x000B//功能-2
#define PAT_FM_F3 0x000C//功能-3
#define PAT_FM_F4 0x000D//功能-4
#define PAT_FM_F5 0x001D//功能-5
#define PAT_FM_F6 0x011D//功能-6
#define PAT_FM_F7 0x021D//功能-7
#define PAT_FM_F8 0x031D//功能-8
#define PAT_FM_H2 0x0006//次原点
#define PAT_FM_J 0x0007//跳缝
#define PAT_FM_U 0x0004//上暂停
#define PAT_FM_D 0x0014//下暂停
#define PAT_FM_P 0x01CC//中压脚上升
#define PAT_FM_T 0x003A//电子夹线器
#define PAT_FM_C 0x0002//剪线
#define PAT_FM_END 0x001F//正常结束
#define PAT_FM_FEND 0x01CC//不正常结束

extern tydstPatRule patRule;
extern tydstConvasNature patConvas;
extern void pat_clearConvas(U8 id);
extern S32 pat_getScale(U8 id,U8 rulesIndex);
extern S32 pat_getFinalScale(S32 imgScale, S32 zoomScale);
extern void pat_setDrawXY(U8 id,U16 posX,U16 posY);
extern void pat_resetDrawXY(void);
extern void pat_resetPatRule(void);
extern void pat_setPatRule(U8 type);
extern void pat_drawPattern(U8 rulesIndex, S32 scale, S16 ox1, S16 oy1, U8 PatType, U32 Patlen, U16 inxpos, U16 inypos, U32 *infeed);


extern void Draw_Pattern(U32 vn, S32 scale,S16 ox1,S16 oy1,U8 PatType,U32 Patlen,U16 inxpos,U16 inypos,U32 *infeed);
extern void ViewRunPoint(U16 x,U16 y,U32 color,U8 linId) ;
extern void ViewPoint(U16 x,U16 y,U32 color,U8 linId);
extern void ViewMutPoint(U32 vn, S32 scale,S16 ox1,S16 oy1,U8 PatType,U32 Patlen,U16 infeed);
extern U8 ViewPoint1(U32 feedc,S32 scale,S16 ox1,S16 oy1,U8 Type,U32 Patlen,U8 flag);
#endif


