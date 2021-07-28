//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  disk_iper.h								*
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


#ifndef __DISK_OPER_H__
#define __DISK_OPER_H__


#include "diskint.h"
#include "usbh_core.h"
#include <stdio.h>
#include "usbh_msc_core.h"
#include "iap.h"


extern int SDIO_USR_MSC_Application(void);
extern int TFCD_USR_MSC_Application(void);
extern void SDIO_USR_MSC_FATFSRESET(void);
extern void TFCD_USR_MSC_FATFSRESET(void);
extern void Select_ExtMemory(uint8_t usbsd);
extern uint8_t ExtMemory_Status(uint8_t usbsd);
extern void USBH_USR_DeInit(void);
extern int USBH_USR_MSC_Application(void);
extern uint8_t Read_PatFile (uint16_t patnum);
extern uint8_t Write_PatFile (uint16_t patnum,uint8_t extype);
extern uint8_t Delete_PatFile (uint16_t patnum);
extern uint8_t Write_SysBackupkFile(U16 *Reg,U16 reglen,U8 FileType);
extern uint8_t Read_SysBackupkFile(U16 *Reg,U16 *reglen,U8 FileType);
extern uint8_t Write_RecordFile(void);
extern uint8_t Delete_GroupFile (U8 *FileName,U8 FileType);
extern uint8_t Write_GroupFile(U8 *FileName,U8 FileType);  
extern uint8_t Explore_Disk_Group (char* path , uint8_t recu_level);
extern uint8_t Explore_Disk (char* path , uint8_t recu_level);
static uint8_t Image_Browser (char* path);
static void     Show_Image(void);
static void     Toggle_Leds(void);
uint8_t Format_Disk (void);
uint32_t G_Changto_E(U16 *sreg);
extern uint8_t Read_ProgramFile(uint8_t list,uint8_t num,uint32_t *sumbyte,PROGRESSBAR pgbar);

extern void USB_Init(void);
extern void USB_Process(void);

#ifdef APPLICATION
//uint16_t  getFileNo(uint16_t base,U16 *indexReg);
uint8_t pnoIsContain(uint16_t pno);
uint8_t getFileTypeByName(char *fn);
uint8_t getDiskFiles(char* path,U16 *indexReg,uint8_t ftype);
uint8_t newDiskFolder(char* path,char *name);
uint8_t renameDiskFile(char* path,char *oldName,char *newName);
uint8_t readDiskPatFile (char* path,uint16_t patnum);
uint8_t witeDiskPatFile(char* path,uint16_t patnum);
uint8_t deleteDiskFile(char *path,char* name);
uint8_t copyDiskPatFile(char* spath,uint8_t sdisk,char* dpath,uint8_t ddisk,char *name,uint8_t mode);
uint8_t writeDiskEmbFile(char* path,uint16_t patnum);
uint8_t readDiskPatFileByName(char* path,char *fname,U16 *sreg);
uint8_t readDiskGropFileByName(char* path,uint8_t ftype,char *fname);
void exportErrMxg(U16 bkcolor);
uint8_t exportSysIoPara(U16 *Reg,U16 reglen,U8 FileType,U16 bkcolor); 
uint8_t inportSysIoPara(U16 *Reg,U16 *reglen,U8 FileType,U16 bkcolor); 
#endif


#endif

