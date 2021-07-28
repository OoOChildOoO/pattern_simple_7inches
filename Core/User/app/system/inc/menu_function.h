/*
 * @Author: your name
 * @Date: 2021-05-12 13:18:36
 * @LastEditTime: 2021-07-27 14:23:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_function.h
 */
#ifndef _MENU_FUNCTION_H
#define	_MENU_FUNCTION_H

#include "stm32cpu.h"

// #define		TLFNADD_COPY		19
// #define		TLFNLEN_COPY		26	//1
// #define		TLFNADD_NEEDLE		(TLFNADD_COPY+TLFNLEN_COPY)
// #define		TLFNLEN_NEEDLE		23	//2
// #define		TLFNADD_FORMAT		(TLFNADD_NEEDLE+TLFNLEN_NEEDLE)
// #define		TLFNLEN_FORMAT		6	//3
// #define		TLFNADD_GROUP_1		(TLFNADD_FORMAT+TLFNLEN_FORMAT)
// #define		TLFNLEN_GROUP_1		33	//4	
// #define		TLFNADD_GROUP_2		(TLFNADD_GROUP_1+TLFNLEN_GROUP_1)
// #define		TLFNLEN_GROUP_2		18	//4	
// #define		TLFNADD_GROUP_3		(TLFNADD_GROUP_2+TLFNLEN_GROUP_2)
// #define		TLFNLEN_GROUP_3		28	//4	
// #define		TLFNADD_GROUP_4		(TLFNADD_GROUP_3+TLFNLEN_GROUP_3)
// #define		TLFNLEN_GROUP_4		53	//4	
// #define		TLFNADD_SYSVERSION	(TLFNADD_GROUP_4+TLFNLEN_GROUP_4)
// #define		TLFNLEN_SYSVERSION	14	//5
// #define		TLFNADD_SCREENSET	(TLFNADD_SYSVERSION+TLFNLEN_SYSVERSION)
// #define		TLFNLEN_SCREENSET	43	//6
// #define		TLFNADD_SYSRESTORE	(TLFNADD_SCREENSET+TLFNLEN_SCREENSET)
// #define		TLFNLEN_SYSRESTORE	36	//7
// #define		TLFNADD_SYSBACKUP	(TLFNADD_SYSRESTORE+TLFNLEN_SYSRESTORE)
// #define		TLFNLEN_SYSBACKUP	12	//8
// #define		TLFNADD_USER		(TLFNADD_SYSBACKUP+TLFNLEN_SYSBACKUP)
// #define		TLFNLEN_USER		48	//9
// #define		TLFNADD_ERRRECORD	(TLFNADD_USER+TLFNLEN_USER)
// #define		TLFNLEN_ERRRECORD	11	//10
// #define		TLFNADD_RUNRECORD	(TLFNADD_ERRRECORD+TLFNLEN_ERRRECORD)
// #define		TLFNLEN_RUNRECORD	31	//11
// #define		TLFNADD_NETWORD		(TLFNADD_RUNRECORD+TLFNLEN_RUNRECORD)
// #define		TLFNLEN_NETWORD		10	//12
// #define		TLFNADD_NETWORD_1	(TLFNADD_NETWORD+TLFNLEN_NETWORD)
// #define		TLFNLEN_NETWORD_1	50	//12
// #define		TLFNADD_NETWORD_2	(TLFNADD_NETWORD_1+TLFNLEN_NETWORD_1)
// #define		TLFNLEN_NETWORD_2	17	//12
// #define		TLFNADD_NETWORD_3	(TLFNADD_NETWORD_2+TLFNLEN_NETWORD_2)
// #define		TLFNLEN_NETWORD_3	20	//12
// #define		TLFNADD_RESET		(TLFNADD_NETWORD_3+TLFNLEN_NETWORD_3)
// #define		TLFNLEN_RESET		5	//13
// #define		TLFNADD_PARAC		(TLFNADD_RESET+TLFNLEN_RESET)
// #define		TLFNLEN_PARAC		40	//14
// #define		TLFNADD_SERVO		(TLFNADD_PARAC+TLFNLEN_PARAC)
// #define		TLFNLEN_SERVO		46	//15
// #define		TLFNADD_STEPPER		(TLFNADD_SERVO+TLFNLEN_SERVO)
// #define		TLFNLEN_STEPPER		28	//16
// #define		TLFNADD_OTHER		(TLFNADD_STEPPER+TLFNLEN_STEPPER)
// #define		TLFNLEN_OTHER		5	//17
// #define		TLFNADD_FBAG		(TLFNADD_OTHER+TLFNLEN_OTHER)
// #define		TLFNLEN_FBAG		53	//18
// #define		TLFNADD_BOOTSET		(TLFNADD_FBAG+TLFNLEN_FBAG)
// #define		TLFNLEN_BOOTSET		17	//18
// #define		TLFNADD_OTHER2		(TLFNADD_BOOTSET+TLFNLEN_BOOTSET)
// #define		TLFNLEN_OTHER2		35	//17


void Menu_FtCopy(void);

void Menu_Function(void);

void Menu_FtFormat(void);

void Menu_FtGroup(void);

void Menu_FtReadMem(void);

void Menu_FtDirReadMem(void);

void Menu_FtReadCombine(void);

void Menu_FtWriteCombine(void);

void Menu_FtUser(void);

void Menu_FtSysversion(void);

void Menu_FtScreenset(void);

void Menu_FtBootScreen(void);

void Menu_FtBootScreen(void);

void Menu_FtSysbackup(void);

void Menu_FtSysrestore(void);

#endif
