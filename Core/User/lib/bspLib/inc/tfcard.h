//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  tfcard.h									*
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

#ifndef __TFCARD_H 	 
#define __TFCARD_H

#include "stm32cpu.h"

#define SDTFRDY		35
// SD卡类型定义 
#define		SD_TYPE_MMC     0
#define		SD_TYPE_V1      1
#define		SD_TYPE_V2      2
#define		SD_TYPE_V2HC    4

// SD传输数据结束后是否释放总线宏定义
#define		NO_RELEASE      0
#define		RELEASE         1

#define		STA_NOINIT			0x01	// Drive not initialized 
#define		STA_NODISK			0x02	// No medium in the drive 
#define		STA_PROTECT			0x04	// Write protected 


// SD卡指令表
#define CMD0    0       //卡复位
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define ACMD41  41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC


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




