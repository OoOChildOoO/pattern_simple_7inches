//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  spi.h									*
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
//*															*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*


#ifndef __SPI_H 	 
#define __SPI_H

#include "stm32cpu.h"


#define 	SP_48MHZ			0xff
#define 	SP_24MHZ			0
#define 	SP_12MHZ			1
#define 	SP_6MHZ				2
#define 	SP_3MHZ				3
#define 	SP_1M5HZ			4
#define 	SP_750000HZ			5
#define 	SP_375000HZ			6
#define 	SP_187500HZ			7

#define 	SPI1_DMA_RF			(DMA2->LISR&(1<<21))		// stream2
#define 	SPI1_DMA_TF			(DMA2->LISR&(1<<27))		// stream3
#define 	SPI2_DMA_RF			(DMA1->LISR&(1<<27))		// stream3
#define 	SPI2_DMA_TF			(DMA1->HISR&(1<<5))			// stream4
#define 	SPI3_DMA_RF			(DMA1->LISR&(1<<21))		// stream2
#define 	SPI3_DMA_TF			(DMA1->HISR&(1<<27))		// stream7









extern U8 SpiSpeed;



extern void SPI1_Init(U8 baud);
extern void SPI2_Init(U8 baud);
extern void SPI3_Init(U8 baud);
extern void SPI1_SetSpeed(U8 SpeedSet);
extern void SPI2_SetSpeed(U8 SpeedSet);
extern void SPI3_SetSpeed(U8 SpeedSet);
extern U8 SPI1_ReadWriteByte(U8 TxData);
extern U8 SPI2_ReadWriteByte(U8 TxData);
extern U8 SPI3_ReadWriteByte(U8 TxData);
extern void SPI1_DMAR_Config(U8*mbuf,U16 bufsize);
extern void SPI1_DMATR_Config(U16 bufsize);
extern void SPI1_DMAT_Config(U8*mbuf,U16 bufsize);
extern void SPI2_DMAR_Config(U8*mbuf,U16 bufsize);
extern void SPI2_DMATR_Config(U16 bufsize);
extern void SPI2_DMAT_Config(U8*mbuf,U16 bufsize);	
extern void SPI3_DMAR_Config(U8*mbuf,U16 bufsize);
extern void SPI3_DMATR_Config(U16 bufsize);
extern void SPI3_DMAT_Config(U8*mbuf,U16 bufsize);



#endif


