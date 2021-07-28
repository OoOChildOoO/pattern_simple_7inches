/*
 * @Author: your name
 * @Date: 2021-03-11 13:54:55
 * @LastEditTime: 2021-07-20 10:51:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\inc\utility.h
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
//*  ��   ��  ��:  utility.h								*
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

#ifndef __UTILITY_H
#define __UTILITY_H

#include "stm32cpu.h"

#define KEYDT 5   // ��������ִ����ʱʱ��
#define KEYMT 10  // ��������ִ����ʱʱ��(�ڶ��μ��Ժ�)
#define KEYNT 50  // ��������ִ����ʱʱ��(�ڶ��μ��Ժ�_���ּ�����)
#define KEYLT 100 // ��������ִ����ʱʱ��(����)
#define KEYFT 250 // ��������ִ����ʱʱ��(��һ��) 220
#define BELLT 70  // ����ʱ����������ʱ��

#define DELAY_MOVE 20 // ѹ�������ӳ�
#define DELAY_JOG1 15 // ϵͳM�����ƶ��ӳ�
#define DELAY_JOG2 5  // ϵͳB�����ƶ��ӳ�

extern volatile U16 g_keyBellT; // ����ʱ����������ʱ��
extern volatile U16 g_allBellT; // ����ʱ����������ʱ��
extern volatile U32 Times;
extern volatile U8 keyolddata;

extern volatile U16 keyxpct, keyypct;
extern volatile U8 g_logorun;
extern volatile U8 ioinnum, ioflag;

extern void Rotary_Logo(U8 ll);
extern void DisAnimation(U8 ll);

#define IND_T 6 // �˲�ֵms	10
extern volatile U32 Timerms;
extern volatile U32 Timer_ms;
extern volatile U32 Timer_tms; //ʱ����ʾ
extern volatile U32 Timer_dms; //������ʾ
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

//����Ϊ��ຯ��
void WFI_SET(void);      //ִ��WFIָ��
void INTX_DISABLE(void); //�ر������ж�
void INTX_ENABLE(void);  //���������ж�
void MSR_MSP(u32 addr);  //���ö�ջ��ַ

#endif
