/*
 * @Author: your name
 * @Date: 2021-03-11 13:54:55
 * @LastEditTime: 2021-05-31 19:40:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_p.h
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
//*  文   件  名:  menu_p.h									*
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

#ifndef MENU_P_H
#define MENU_P_H



#include "stm32cpu.h"

#define		TLFNADD_P1				0
#define		TLFNLEN_P1				44
#define		TLFNADD_THREAD			TLFNADD_P1+TLFNLEN_P1
#define		TLFNLEN_THREAD			1

void Menu_P1(void);

void Menu_P2(void);

void Menu_Menu(void);

void Menu_Thread(void);

void Menu_Counter(U16 para);

void Menu_Wind(void);

void Menu_Read(void);

U8 isViewNo(U16 patno, U16 viewno);

void Menu_DirRead(void);

void Menu_Write(void);

void Menu_Write1(void);

void Menu_SkipJog(void);

U8 fun_getPatData(U16 chFileNoTemp, U8 chDiskTemp,U16 chPageNo);
#endif


