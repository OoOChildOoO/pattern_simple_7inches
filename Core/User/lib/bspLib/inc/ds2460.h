//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  ds2460.h									*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: SHA-1协议处理器										*
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


