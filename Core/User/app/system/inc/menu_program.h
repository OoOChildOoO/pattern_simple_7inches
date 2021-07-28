/*
 * @Author: your name
 * @Date: 2021-05-12 08:47:57
 * @LastEditTime: 2021-07-12 13:44:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_program.h
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
//*  文   件  名:  menu_program.h							*
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
