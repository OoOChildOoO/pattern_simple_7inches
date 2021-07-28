/*
 * @Author: your name
 * @Date: 2021-05-07 11:31:19
 * @LastEditTime: 2021-06-29 19:31:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_create.h
 */
#ifndef _MEMU_CREATE_H
#define _MEMU_CREATE_H

#include "stm32cpu.h"

#define LOOSEN_MAX 127
#define LOOSEN_MIN 1

U8 getctNearPointPos(s16 inx, S16 iny, S16 *outx, S16 *outy);

void Menu_CtInput(void);

void Menu_CtCursor(void);

void Menu_CtInputMenu(void);

void Menu_CtCallFile(void);

void Menu_DirCallFile(void);

void Menu_Ctpitch(void);

void Menu_CtMethod(void);

void Menu_Ctmultiple(void);

void Menu_Ctzigzag(void);

void Menu_Cttacking(void);

void Menu_CtTack1(void);

void Menu_CtCode(void);

void Menu_CtExCode(void);

void Menu_Ctlineforce(U16 para);

void Menu_CtPfootSet(U16 para);

void Menu_CtInHelp(U16 para);

#if 0
U8 fun_callFile(U32 tmpselectdisno, U8 StPointFlag, U8 EndPointFlag, u32 patlen,U16 chPageNo);
#endif

#endif
