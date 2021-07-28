//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  flashsave.c									*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision4 V4.21					*
//*  编程													*
//*															*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#ifndef _FLASH_SAVE_H
#define _FLASH_SAVE_H

#define	FAT_TABLE	0		// 文件索引表 72k  (4x18)
#define	FAT_STSEC	18		// 开始SEC表一 4KB (4x1)
#define	FAT_SEC		19		// SEC表 32KB	   (4x8)
#define	FAT_FILE	27		// 文件

#define	FAT_TOTAL	256*16	//扇区总数


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

