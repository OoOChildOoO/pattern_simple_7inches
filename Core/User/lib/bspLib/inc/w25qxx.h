//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  w25qxx.h									*
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

#ifndef __W25QXX_H 	 
#define __W25QXX_H

#include "stm32cpu.h"

extern U16 W25QXX_1TYPE;				//定义W25QXX芯片型号
extern U16 W25QXX_2TYPE;				//定义W25QXX芯片型号

#define W25Q1_DAT	0 	
#define W25Q2_MP3	1 
#define W25Q3_DAT	2 

//W25X系列/Q系列芯片列表
#define W25Q08 	0XEF13					//容量为 1M字节,共有  16个Block(64KB),  256个Sector(4KB),  4096Page(256Byte),Add:0-0x100000 (3B Add Mode)	
#define W25Q16 	0XEF14					//容量为 2M字节,共有  32个Block(64KB),  512个Sector(4KB),  8192Page(256Byte),Add:0-0x200000 (3B Add Mode)
#define W25Q32 	0XEF15					//容量为 4M字节,共有  64个Block(64KB), 1024个Sector(4KB), 16384Page(256Byte),Add:0-0x400000 (3B Add Mode)
#define W25Q64 	0XEF16					//容量为 8M字节,共有 128个Block(64KB), 2048个Sector(4KB), 32768Page(256Byte),Add:0-0x800000 (3B Add Mode)
#define W25Q128	0XEF17					//容量为16M字节,共有 256个Block(64KB), 4096个Sector(4KB), 65536Page(256Byte),Add:0-0x1000000(3B Add Mode)
#define W25Q256 0XEF18					//容量为32M字节,共有 512个Block(64KB), 8192个Sector(4KB),131072Page(256Byte),Add:0-0x2000000(3B/4B Add Mode)
#define W25M512 0XEF19					//容量为64M字节,共有1024个Block(64KB),16384个Sector(4KB),262144Page(256Byte),Add:0-0x4000000(3B/4B Add Mode) 




//指令表
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
U16  W25QXX_ReadID(U8 wcs);  	    									//读取FLASH ID
U8 	 W25QXX_ReadSR(U8 wcs,U8 regno);           							//读取状态寄存器 
void W25QXX_4ByteAddr_Enable(void);     								//使能4字节地址模式
void W25QXX_Write_SR(U8 wcs,U8 regno,U8 sr);   							//写状态寄存器
void W25QXX_Write_Enable(U8 wcs);  										//写使能 
void W25QXX_Write_Disable(U8 wcs);										//写保护
void W25QXX_Write_NoCheck(U8 wcs,U8* pBuffer,U32 WriteAddr,U32 NumByteToWrite);
void W25QXX_Read(U8 wcs,U8* pBuffer,U32 ReadAddr,U32 NumByteToRead);	//读取flash
void W25QXX_Write(U8 wcs,U8* pBuffer,U32 WriteAddr,U32 NumByteToWrite);	//写入flash
void W25QXX_Erase_Chip(U8 wcs);											//整片擦除
void W25QXX_Erase_Sector(U8 wcs,U32 Dst_Addr);							//扇区擦除
void W25QXX_Erase_Block32K(U8 wcs,U32 Dst_Addr) ;						//块擦除
void W25QXX_Erase_Block64K(U8 wcs,U32 Dst_Addr) ;						//块擦除
void W25QXX_Wait_Busy(U8 wcs);											//等待空闲
void W25QXX_PowerDown(U8 wcs);        									//进入掉电模式
void W25QXX_WAKEUP(U8 wcs);												//唤醒
void W25QXX_Read_One_Sector (U8 wcs,U8* pBuffer, U32 SecNo);
void W25QXX_Write_One_Sector(U8 wcs,U8* pBuffer,U32 SecNo) ;













#endif


