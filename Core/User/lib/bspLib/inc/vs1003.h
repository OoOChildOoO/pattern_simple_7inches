//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  vs1003.h									*
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

#ifndef __VS1003_H 	 
#define __VS1003_H

#include "stm32cpu.h"


#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03
//VS1003寄存器定义
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


#define CMD_PLAY			0xA0		//播放语音
#define	CMD_STOP			0xA3		//停止语音
#define	CMD_PAUSE			0xA1		//暂停语音
#define	CMD_SETVOL			0xA4		//调节音量





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
