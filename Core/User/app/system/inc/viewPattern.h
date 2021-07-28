/*
 * @Author: your name
 * @Date: 2021-03-11 13:54:55
 * @LastEditTime: 2021-06-29 17:02:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\viewPattern.h
 */
//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  viewPattern.h							*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.74					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*

#ifndef _VIEW_PATTERN_H
#define _VIEW_PATTERN_H



#include "stm32cpu.h"

#define PAT_FM_L 0x0001 //���ٷ���
#define PAT_FM_MD2 0x0021 //�е��ٷ���
#define PAT_FM_MD1 0x0041 //�и��ٷ���
#define PAT_FM_H 0x0061 //���ٷ���
#define PAT_FM_TRIM 0x0002 //����
#define PAT_FM_EMPTY 0x0003 //����(����)
#define PAT_FM_EMP_ED 0x001B //������
#define PAT_FM_F1 0x0005//����-1
#define PAT_FM_F2 0x000B//����-2
#define PAT_FM_F3 0x000C//����-3
#define PAT_FM_F4 0x000D//����-4
#define PAT_FM_F5 0x001D//����-5
#define PAT_FM_F6 0x011D//����-6
#define PAT_FM_F7 0x021D//����-7
#define PAT_FM_F8 0x031D//����-8
#define PAT_FM_H2 0x0006//��ԭ��
#define PAT_FM_J 0x0007//����
#define PAT_FM_U 0x0004//����ͣ
#define PAT_FM_D 0x0014//����ͣ
#define PAT_FM_P 0x01CC//��ѹ������
#define PAT_FM_T 0x003A//���Ӽ�����
#define PAT_FM_C 0x0002//����
#define PAT_FM_END 0x001F//��������
#define PAT_FM_FEND 0x01CC//����������

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


