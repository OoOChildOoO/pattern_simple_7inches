//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  iap.h									*
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
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#ifndef __IAP_H 		 
#define __IAP_H

#include "stm32cpu.h"

#define IAPDATA_ADD						0xc0800000

#ifdef _2M_CPU
	#define USER_FLASH_FIRST_PAGE_ADDRESS	0x08010000
	#define USER_FLASH_LAST_PAGE_ADDRESS	0x081E0000
	#define USER_FLASH_END_ADDRESS      	0x081FFFFF
#else
	#define USER_FLASH_FIRST_PAGE_ADDRESS	0x08010000
	#define USER_FLASH_LAST_PAGE_ADDRESS	0x080E0000
	#define USER_FLASH_END_ADDRESS      	0x080FFFFF
#endif

#define SECTOR_MASK					((uint32_t)0xFFFFFF07)

#define RDP_KEY						((uint16_t)0x00A5)
#define FLASH_KEY1					((uint32_t)0x45670123)
#define FLASH_KEY2					((uint32_t)0xCDEF89AB)
#define FLASH_OPT_KEY1				((uint32_t)0x08192A3B)
#define FLASH_OPT_KEY2				((uint32_t)0x4C5D6E7F)

#define FLASH_Sector_0				((uint16_t)0x0000)		// Sector Number 0		16Kbyte 
#define FLASH_Sector_1				((uint16_t)0x0008)		// Sector Number 1		16Kbyte 
#define FLASH_Sector_2				((uint16_t)0x0010)		// Sector Number 2		16Kbyte 
#define FLASH_Sector_3				((uint16_t)0x0018)		// Sector Number 3		16Kbyte 
#define FLASH_Sector_4				((uint16_t)0x0020)		// Sector Number 4		64Kbyte 
#define FLASH_Sector_5				((uint16_t)0x0028)		// Sector Number 5		128Kbyte 
#define FLASH_Sector_6				((uint16_t)0x0030)		// Sector Number 6		128Kbyte 
#define FLASH_Sector_7				((uint16_t)0x0038)		// Sector Number 7		128Kbyte 
#define FLASH_Sector_8				((uint16_t)0x0040)		// Sector Number 8		128Kbyte 
#define FLASH_Sector_9				((uint16_t)0x0048)		// Sector Number 9		128Kbyte 
#define FLASH_Sector_10				((uint16_t)0x0050)		// Sector Number 10		128Kbyte 
#define FLASH_Sector_11				((uint16_t)0x0058)		// Sector Number 11		128Kbyte 

#define FLASH_Sector_12				((uint16_t)0x0080)		// Sector Number 12		16Kbyte 
#define FLASH_Sector_13				((uint16_t)0x0088)		// Sector Number 13		16Kbyte 
#define FLASH_Sector_14				((uint16_t)0x0090)		// Sector Number 14		16Kbyte 
#define FLASH_Sector_15				((uint16_t)0x0098)		// Sector Number 15		16Kbyte 
#define FLASH_Sector_16				((uint16_t)0x00a0)		// Sector Number 16		64Kbyte 
#define FLASH_Sector_17				((uint16_t)0x00a8)		// Sector Number 17		128Kbyte 
#define FLASH_Sector_18				((uint16_t)0x00b0)		// Sector Number 18		128Kbyte 
#define FLASH_Sector_19				((uint16_t)0x00b8)		// Sector Number 19		128Kbyte 
#define FLASH_Sector_20				((uint16_t)0x00c0)		// Sector Number 20		128Kbyte 
#define FLASH_Sector_21				((uint16_t)0x00c8)		// Sector Number 21		128Kbyte 
#define FLASH_Sector_22				((uint16_t)0x00d0)		// Sector Number 22		128Kbyte 
#define FLASH_Sector_23				((uint16_t)0x00d8)		// Sector Number 23		128Kbyte 


#define FLASH_FLAG_EOP				((uint32_t)0x00000001)  // FLASH End of Operation flag 
#define FLASH_FLAG_OPERR			((uint32_t)0x00000002)  // FLASH operation Error flag 
#define FLASH_FLAG_WRPERR			((uint32_t)0x00000010)  // FLASH Write protected error flag 
#define FLASH_FLAG_PGAERR			((uint32_t)0x00000020)  // FLASH Programming Alignment error flag 
#define FLASH_FLAG_PGPERR			((uint32_t)0x00000040)  // FLASH Programming Parallelism error flag  
#define FLASH_FLAG_PGSERR			((uint32_t)0x00000080)  // FLASH Programming Sequence error flag  
#define FLASH_FLAG_BSY				((uint32_t)0x00010000)  // FLASH Busy flag  
#define IS_FLASH_CLEAR_FLAG(FLAG)	((((FLAG) & (uint32_t)0xFFFFFF0C) == 0x00000000) && ((FLAG) != 0x00000000))
#define IS_FLASH_GET_FLAG(FLAG)		(((FLAG) == FLASH_FLAG_EOP) || ((FLAG) == FLASH_FLAG_OPERR) || \
									((FLAG) == FLASH_FLAG_WRPERR) || ((FLAG) == FLASH_FLAG_PGAERR) || \
									((FLAG) == FLASH_FLAG_PGPERR) || ((FLAG) == FLASH_FLAG_PGSERR) || \
									((FLAG) == FLASH_FLAG_BSY))

#define ACR_BYTE0_ADDRESS			((uint32_t)0x40023C00) 

#define OPTCR_BYTE0_ADDRESS			((uint32_t)0x40023C14)
#define OPTCR_BYTE1_ADDRESS			((uint32_t)0x40023C15)
#define OPTCR_BYTE2_ADDRESS			((uint32_t)0x40023C16)

#define OB_RDP_Level_0				((uint8_t)0xAA)
#define OB_RDP_Level_1				((uint8_t)0x55)
#define OB_RDP_Level_2				((uint8_t)0xCC)

#define VoltageRange_1				((uint8_t)0x00)			// Device operating range: 1.8V to 2.1V 
#define VoltageRange_2				((uint8_t)0x01)			//Device operating range: 2.1V to 2.7V 
#define VoltageRange_3				((uint8_t)0x02)			//Device operating range: 2.7V to 3.6V 
#define VoltageRange_4				((uint8_t)0x03)			//Device operating range: 2.7V to 3.6V + External Vpp 

#define FLASH_PSIZE_BYTE			((uint32_t)0x00000000)
#define FLASH_PSIZE_HALF_WORD		((uint32_t)0x00000100)
#define FLASH_PSIZE_WORD			((uint32_t)0x00000200)
#define FLASH_PSIZE_DOUBLE_WORD		((uint32_t)0x00000300)
#define CR_PSIZE_MASK				((uint32_t)0xFFFFFCFF)




typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_PGS,
  FLASH_ERROR_PGP,
  FLASH_ERROR_PGA,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_ERROR_OPERATION,
  FLASH_COMPLETE
}FLASH_Status;

typedef struct
{
  U32 hexadd;		// 编程启始地址
  U32 hexlen;		// 编程长度
  U16 hexcrc;		// crc校验位
  U16 hexsum;		// sum校验位
  U16 hexxor;		// xor校验位
  U16 hexdat1;		// 备用1
  U16 hexdat2;		// 备用2
  U16 verf;			// 版本
} HEXCHECK;

typedef struct
{
  U16 mode;			// 是否用进度条
  U16 color;		// 进度条颜色
  U16 bkcolor;		// 底色	
  U16 px;			// 进度条X启始坐标
  U16 py;			// 进度条Y启始坐标
  U16 pdx;			// 进度条X长度
  U16 pdy;			// 进度条Y高度
  U16 str;			// 进度条启始显示位置
  U16 end;			// 进度条结束显示位置
  U16 RESERVED;	
} PROGRESSBAR;


extern HEXCHECK flashcheck;

extern void FLASH_Unlock(void);
extern void FLASH_Lock(void);
extern void FLASH_OB_Unlock(void);
extern void FLASH_OB_Lock(void);
extern FlagStatus FLASH_OB_GetRDP(void);
extern void FLASH_OB_RDPConfig(uint8_t OB_RDP);
extern FLASH_Status FLASH_OB_Launch(void);
extern int8_t FLASH_If_Erase(uint32_t StartSector,PROGRESSBAR pgbar);
extern U8 checkflash(void);
extern U8 iapprogram(PROGRESSBAR pgbar);
extern uint8_t OTP_Write(__IO uint32_t FlashAddress, uint8_t* Data ,uint16_t DataLength);
extern uint32_t FLASH_If_Write(__IO uint32_t FlashAddress, uint32_t* Data ,uint32_t DataLength,PROGRESSBAR pgbar);

#ifdef APPLICATION
/* 名称 ： 精度条函数*/
/* 输入 ： x,y,dx,dy,显示位置与大小，percent当前进度 color进度条颜色 bkcolor进度条底色*/
extern void DrawProgressbar(U32 x,U32 y,U32 dx,U32 dy,U8 percent,U16 color,U16 bkcolor); 
#endif

#endif
