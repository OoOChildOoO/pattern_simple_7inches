//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  flashsave.c									*
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
//*															*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*

#ifndef _FLASH_SAVE_H
#define _FLASH_SAVE_H

#define	FAT_TABLE	0		// �ļ������� 72k  (4x18)
#define	FAT_STSEC	18		// ��ʼSEC��һ 4KB (4x1)
#define	FAT_SEC		19		// SEC�� 32KB	   (4x8)
#define	FAT_FILE	27		// �ļ�

#define	FAT_TOTAL	256*16	//��������


#include "stm32cpu.h"
extern volatile U16 SYS_PTINDEXF; 

void teststsec(void);

U32 PT_Del(U32 t, U32 fno,U16 *ireg);
U32 PT_ReadFromFlash(U32 fno,U16 *sreg,U16* ireg);
U32 PT_SaveToFlash(U32 fno,U16 *sreg,U16* ireg);
void readFlashIndex(U16 *ireg);
void writeFlashIndex(U16 *ireg);
U32 read_stsec(U32 fno);
U32 save_stsec(U32 ssec, U32 fno);
U32 del_stsec(U32 fno);
U32 delsec(U32 sec);
U32 findsec(U32 sec0);
U32 nextsec(U32 sec, U32 sno, U32 fno);
U32 sevesec(U32 sec, U32 psec, U32 nsec, U32 secno, U32 fno);

#endif

