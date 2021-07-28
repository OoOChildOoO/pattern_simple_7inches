/*
 * @Author: your name
 * @Date: 2021-03-11 13:54:55
 * @LastEditTime: 2021-07-20 10:51:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\inc\utility.h
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
//*  文   件  名:  utility.h								*
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

#ifndef __UTILITY_H
#define __UTILITY_H

#include "stm32cpu.h"

#define KEYDT 5   // 单击按键执行延时时间
#define KEYMT 10  // 连续按键执行延时时间(第二次及以后)
#define KEYNT 50  // 连续按键执行延时时间(第二次及以后_数字键盘用)
#define KEYLT 100 // 连续按键执行延时时间(慢速)
#define KEYFT 250 // 连续按键执行延时时间(第一次) 220
#define BELLT 70  // 按键时蜂鸣器发声时间

#define DELAY_MOVE 20 // 压框移送延迟
#define DELAY_JOG1 15 // 系统M跳针移动延迟
#define DELAY_JOG2 5  // 系统B跳针移动延迟

extern volatile U16 g_keyBellT; // 按键时蜂鸣器发声时间
extern volatile U16 g_allBellT; // 按键时蜂鸣器发声时间
extern volatile U32 Times;
extern volatile U8 keyolddata;

extern volatile U16 keyxpct, keyypct;
extern volatile U8 g_logorun;
extern volatile U8 ioinnum, ioflag;

extern void Rotary_Logo(U8 ll);
extern void DisAnimation(U8 ll);

#define IND_T 6 // 滤波值ms	10
extern volatile U32 Timerms;
extern volatile U32 Timer_ms;
extern volatile U32 Timer_tms; //时间显示
extern volatile U32 Timer_dms; //磁盘显示
extern volatile U32 Timer_ms1;
extern volatile U16 sys_mrst2;
extern volatile U32 userLoninTime;
void system_init(void);
void iap_init(void);
void Beep_On(U32 t);
void Time6_Init(void);
void Time13_Init(void);
void Time14_Init(void);
void SetLedPwm(U32 pwm);
void SetLcdPwm(U8 lcdoff, U32 pwm);
void wdt(void);
void DelayMs(unsigned int n);
void DelayUs(unsigned int n);
void resetKeyData(void);
U16 get_keydata(KEYPAD *keyd, U8 keynum, U8 basenum);
U16 get_keydata1(KEYPAD *keyd, U8 keynum, U16 basenum);
U16 get_ikeydata(U8 basenum);
U8 asctonum(U8 asc);
U8 numtoasc(U8 num);

//#if Debug
//void DebugLog(U8 *log,U8 f);
//extern U8 DebLog[64];
//#endif

//以下为汇编函数
void WFI_SET(void);      //执行WFI指令
void INTX_DISABLE(void); //关闭所有中断
void INTX_ENABLE(void);  //开启所有中断
void MSR_MSP(u32 addr);  //设置堆栈地址

#endif
