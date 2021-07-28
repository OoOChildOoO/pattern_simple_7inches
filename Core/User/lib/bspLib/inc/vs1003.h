//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  vs1003.h									*
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

#ifndef __VS1003_H 	 
#define __VS1003_H

#include "stm32cpu.h"


#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03
//VS1003�Ĵ�������
#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09 
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f   
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 


#define CMD_PLAY			0xA0		//��������
#define	CMD_STOP			0xA3		//ֹͣ����
#define	CMD_PAUSE			0xA1		//��ͣ����
#define	CMD_SETVOL			0xA4		//��������





void VS1003B_Delay(U32 n);
void VS1003B_WriteCMD(U8 addr, U16 dat);
U16 VS1003B_ReadCMD(U8 addr);
U8 VS1003B_Init(void);
void VS1003B_Reset(void);
void VS1003B_SoftReset(void);
void VS1003B_SimTest(void);
void VS1003B_SimTestChg(U8 f);
void VS1003B_WriteDAT(void);
void VS1003B_Mp3On(U32 dat);
void VS1003B_Mp3Off(void);
U8 VS1003B_Mp3Stats(void);


void PlaySound(U8 cmd,U8 language,U16 p);
void Playmp3(void);
void Playmp3_1(U8 *buf,U32 dat);
#endif
