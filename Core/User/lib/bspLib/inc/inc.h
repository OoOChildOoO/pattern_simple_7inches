//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  inc.h									*
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









//����Ϊ��ຯ��
extern void WFI_SET(void);		//ִ��WFIָ��
extern void INTX_DISABLE(void);//�ر������ж�
extern void INTX_ENABLE(void);	//���������ж�
extern void MSR_MSP(u32 addr);	//���ö�ջ��ַ 



#endif


