//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  s39vF160.h								*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.21					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*

#ifndef __S39VF160_H 	  
#define __S39VF160_H

#include 	"stm32cpu.h"
//#include	"register.h"
//#include	"utility.h"


// �ڴ����
#define FLAADD		(U16 *)(0x60000000)		// FLASH

#define ADD5555					0x5555//	0xaaaa
#define ADD2AAA					0x2aaa// 	0x5554
#define FLADD5555				(U16 *)(0x6000aaaa)//	0xaaaa
#define FLADD2AAA				(U16 *)(0x60005554)// 	0x5554



extern void EMI_Bank0Init(void);
extern void EMI_Bank1Init(void);
extern void EMI_Bank2Init(void);
extern void EMI_Bank3Init(void);
extern U16 Check_SST_39VF160(void);					// 
extern U16 SST39VFXX_CheckID(U8 mode);
extern void CFI_Query(void);
extern void Erase_Entire_Chip(void);
extern void Erase_One_Sector (U32 SecAdd);
extern void Erase_One_Block (U32 BloAdd);
extern U8   Program_One_Word (U16 SrcWord, U32 Add);
extern U8 Program_One_Word1 (U16 SrcWord, U32 Add);
extern U8   Program_One_Sector (U16 *Src, U32 SecAdd);
extern U8   Program_One_Block (U16 *Src, U32 BloAdd);
extern U16   Read_One_Word (U32 Add);
extern U8   Read_One_Sector (U16 *Src, U32 SecAdd);
extern U8 FlashCheck_Entire_Chip(void);
extern void SST39VFXX_Write(U8 *pBuffer,U32 WriteAddr,U32 NumByteToWrite);
extern void SST39VFXX_Read(U8 *pBuffer,U32 WriteAddr,U32 NumByteToWrite);

#endif
