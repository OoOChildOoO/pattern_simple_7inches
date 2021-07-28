//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  diskint.h								*
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
