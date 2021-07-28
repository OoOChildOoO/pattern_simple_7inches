/*
 * @Author: your name
 * @Date: 2021-05-12 08:47:57
 * @LastEditTime: 2021-07-12 13:44:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_program.h
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
//*  ��   ��  ��:  menu_program.h							*
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
#ifndef _MENU_PROGRAM_H
#define _MENU_PROGRAM_H


	
#include "stm32cpu.h"
#include "string.h"


void Menu_Prog(void);
void Menu_ProFrameFlash(void);
void Menu_ProFrame(void);
void Menu_ProPara(void);
void Menu_ProPass(void);
U32 Menu_ProgramNumber(U32 para, U32 *def, U32 vmin, U32 vmax, U16 *unit);
U32 Menu_ProgramLogic(U32 *def,U32 vmin, U32 vmax);
void Menu_ProgramLogicHelp(U32 def);
void Menu_ProDifferent(void);
void Menu_FastProg(void);
U16 Menu_SetProgNo(U16 inNo);



#endif
