//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  ds2460.h									*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: SHA-1Э�鴦����										*
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

#ifndef  __DS2460_H		  
#define  __DS2460_H

#include 	"stm32cpu.h"
#include	"stdlib.h"


extern U8 DS2460_Init(U8 slaveAddr);
extern void msDelay(unsigned int delay);
extern void DelayMs(unsigned int n);

extern U8 ReadBackLight(void);
extern void SaveBackLight(U8 bl);

extern U8 ReadBatType(void);
extern void SaveBatType(U8 bl);

extern U8 ReadStartupWait(void);
extern void SaveStartupWait(U8 bl);
	
extern U8 ReadUserNo(void);
extern void SaveUserNo(U8 bl);

extern U8 ReadPessVal(void);
extern void WritePessVal(U8 bl);

extern U8 ReadJogmethod(void);
extern void WriteJogmethod(U8 bl);

extern U8 ReadSysid(void);
extern U8 SaveSysid(void);

extern U8 ReadVoice(U8 type);
extern void SaveVoice(U8 vo1,U8 vo2);

extern U8 ReadTimeMode(U8 type);
extern void WriteTimeMode(U8 data,U8 hour);

extern void ReadPromgParas(U16 *outbt);
extern void WriteromgParas(U16 *inbt);



#endif


