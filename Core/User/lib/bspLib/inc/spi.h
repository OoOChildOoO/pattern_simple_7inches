//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  spi.h									*
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
//*															*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
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


