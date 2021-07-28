//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  tfcard.h									*
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

#ifndef __TFCARD_H 	 
#define __TFCARD_H

#include "stm32cpu.h"

#define SDTFRDY		35
// SD�����Ͷ��� 
#define		SD_TYPE_MMC     0
#define		SD_TYPE_V1      1
#define		SD_TYPE_V2      2
#define		SD_TYPE_V2HC    4

// SD�������ݽ������Ƿ��ͷ����ߺ궨��
#define		NO_RELEASE      0
#define		RELEASE         1

#define		STA_NOINIT			0x01	// Drive not initialized 
#define		STA_NODISK			0x02	// No medium in the drive 
#define		STA_PROTECT			0x04	// Write protected 


// SD��ָ���
#define CMD0    0       //����λ
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC


extern U8  SD_Type;
extern volatile U32 Tfindelay;

extern int TFCD_USR_MSC_FATFSINIT(void);
extern void TFCD_USR_MSC_FATFSRESET(void);
extern void DelayMs(unsigned int n);
U8 TF_Init(void);
U8 SD_WaitReady(void);
U8 SD_SendCommand(U8 cmd, U32 arg, U8 crc);
U8 SD_SendCommand_NoDeassert(U8 cmd, U32 arg, U8 crc);
U8 SD_GetCID(U8 *cid_data);
U8 SD_GetCSD(U8 *csd_data);
U64 SD_GetCapacity(void);
//U8 Explore_Disk(char* path , U8 recu_level);
//void testsd(void);
//U8 ReadPrg_Dir(char* path , U8 selnum);
//U8 ReadPrg_Data(void);
extern U8 SD_ReadSingleBlock(U32 sector, U8 *buffer);
extern U8 SD_WriteSingleBlock(U32 sector, const U8 *data);

//void SD_Remove(void);

















#endif




