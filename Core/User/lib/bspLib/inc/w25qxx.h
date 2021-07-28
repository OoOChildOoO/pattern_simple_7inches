//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  w25qxx.h									*
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

#ifndef __W25QXX_H 	 
#define __W25QXX_H

#include "stm32cpu.h"

extern U16 W25QXX_1TYPE;				//����W25QXXоƬ�ͺ�
extern U16 W25QXX_2TYPE;				//����W25QXXоƬ�ͺ�

#define W25Q1_DAT	0 	
#define W25Q2_MP3	1 
#define W25Q3_DAT	2 

//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q08 	0XEF13					//����Ϊ 1M�ֽ�,����  16��Block(64KB),  256��Sector(4KB),  4096Page(256Byte),Add:0-0x100000 (3B Add Mode)	
#define W25Q16 	0XEF14					//����Ϊ 2M�ֽ�,����  32��Block(64KB),  512��Sector(4KB),  8192Page(256Byte),Add:0-0x200000 (3B Add Mode)
#define W25Q32 	0XEF15					//����Ϊ 4M�ֽ�,����  64��Block(64KB), 1024��Sector(4KB), 16384Page(256Byte),Add:0-0x400000 (3B Add Mode)
#define W25Q64 	0XEF16					//����Ϊ 8M�ֽ�,���� 128��Block(64KB), 2048��Sector(4KB), 32768Page(256Byte),Add:0-0x800000 (3B Add Mode)
#define W25Q128	0XEF17					//����Ϊ16M�ֽ�,���� 256��Block(64KB), 4096��Sector(4KB), 65536Page(256Byte),Add:0-0x1000000(3B Add Mode)
#define W25Q256 0XEF18					//����Ϊ32M�ֽ�,���� 512��Block(64KB), 8192��Sector(4KB),131072Page(256Byte),Add:0-0x2000000(3B/4B Add Mode)
#define W25M512 0XEF19					//����Ϊ64M�ֽ�,����1024��Block(64KB),16384��Sector(4KB),262144Page(256Byte),Add:0-0x4000000(3B/4B Add Mode) 




//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20
#define W25X_Block32KErase		0x52
#define W25X_Block64KErase		0xD8
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9






void W25QXX_Init(void);
U16  W25QXX_CheckID(U8 wcs,U8 mode);
U16  W25QXX_ReadID(U8 wcs);  	    									//��ȡFLASH ID
U8 	 W25QXX_ReadSR(U8 wcs,U8 regno);           							//��ȡ״̬�Ĵ��� 
void W25QXX_4ByteAddr_Enable(void);     								//ʹ��4�ֽڵ�ַģʽ
void W25QXX_Write_SR(U8 wcs,U8 regno,U8 sr);   							//д״̬�Ĵ���
void W25QXX_Write_Enable(U8 wcs);  										//дʹ�� 
void W25QXX_Write_Disable(U8 wcs);										//д����
void W25QXX_Write_NoCheck(U8 wcs,U8* pBuffer,U32 WriteAddr,U32 NumByteToWrite);
void W25QXX_Read(U8 wcs,U8* pBuffer,U32 ReadAddr,U32 NumByteToRead);	//��ȡflash
void W25QXX_Write(U8 wcs,U8* pBuffer,U32 WriteAddr,U32 NumByteToWrite);	//д��flash
void W25QXX_Erase_Chip(U8 wcs);											//��Ƭ����
void W25QXX_Erase_Sector(U8 wcs,U32 Dst_Addr);							//��������
void W25QXX_Erase_Block32K(U8 wcs,U32 Dst_Addr) ;						//�����
void W25QXX_Erase_Block64K(U8 wcs,U32 Dst_Addr) ;						//�����
void W25QXX_Wait_Busy(U8 wcs);											//�ȴ�����
void W25QXX_PowerDown(U8 wcs);        									//�������ģʽ
void W25QXX_WAKEUP(U8 wcs);												//����
void W25QXX_Read_One_Sector (U8 wcs,U8* pBuffer, U32 SecNo);
void W25QXX_Write_One_Sector(U8 wcs,U8* pBuffer,U32 SecNo) ;













#endif


