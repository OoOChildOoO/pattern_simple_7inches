//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  diskint.h								*
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

#ifndef __DISKIO_H 	 
#define __DISKIO_H

#include "stm32cpu.h"




#include "ff.h"	
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

#define IMAGE_BUFFER_SIZE    512
extern USB_OTG_CORE_HANDLE          USB_OTG_Core; 

extern U8 Image_Buf[IMAGE_BUFFER_SIZE];
extern FATFS fatfs;
extern FATFS fatsd;
extern FATFS fattf;
extern FIL file;
extern U8 line_idx;  

extern volatile U32 Sdindelay;
extern volatile U32 Tfindelay;


extern void UART1_SendByte(U8 dat);
extern void UART1_ISendBuf(U16 len, U8 *dat);










#endif
