//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  inc.h									*
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


#ifndef __INC_H 	 
#define __INC_H

#include "stm32cpu.h"

extern volatile U32 Times;
extern volatile U32 Timer_ms;


extern void wdt(void);
extern void DelayMs(unsigned int n);
extern void DelayUs(unsigned int n);
extern void Beep_On(U32 t);
extern U16 get_keydata(KEYPAD *keyd,U8 keynum,U8 basenum);









//以下为汇编函数
extern void WFI_SET(void);		//执行WFI指令
extern void INTX_DISABLE(void);//关闭所有中断
extern void INTX_ENABLE(void);	//开启所有中断
extern void MSR_MSP(u32 addr);	//设置堆栈地址 



#endif


