//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  usbh_msc_fatfs.c							*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.21					*
//*  ���													*
//*															*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*


#include "usb_conf.h"	  
#include "diskio.h"
#include "sdio.h"
#include "usbh_msc_core.h"
#define USB			0
#define SD			1
#define TF			2
#define SDTFRDY		35

volatile uint8_t diskoperatef = 0;
static volatile DSTATUS Stat = STA_NOINIT;		// Disk status
static volatile DSTATUS SDStat = STA_NOINIT;	// Disk status
static volatile DSTATUS TFStat = STA_NOINIT;	// Disk status

extern uint8_t usbsdflag;
extern volatile uint32_t Sdindelay;
extern volatile uint32_t Tfindelay;
extern USB_OTG_CORE_HANDLE		USB_OTG_Core;
extern USBH_HOST				USB_Host;
extern void wdt(void);
extern uint8_t SD_ReadDisk(uint8_t *buf,uint32_t sector,uint32_t cnt);
extern uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt);

extern uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer);
extern uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *data);

extern u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt);
extern u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt);
//------------------------------------------
// Function   : disk_initialize				
// Description: Initialize Disk Drive					
//------------------------------------------
DSTATUS disk_initialize (BYTE drv)
{
	if(drv==USB){
		if(HCD_IsDeviceConnected(&USB_OTG_Core)){  
			Stat = STA_READY;
		}else{
			Stat = STA_NOINIT;
		}
		return Stat; 
	}else if(drv==SD){
		if(Sdindelay==SDTFRDY){
			SDStat = STA_READY;			
		}else{
			SDStat = STA_NOINIT;
		}
		return SDStat; 
	}else if(drv==TF){
		if(Tfindelay==SDTFRDY){
			TFStat = STA_READY;			
		}else{
			TFStat = STA_NOINIT;
		}
		return TFStat; 
	}
	return STA_NOINIT;  
}

//------------------------------------------
// Function   : disk_status				
// Description: Get Disk Status					
//------------------------------------------
DSTATUS disk_status (BYTE drv)
{
	if(drv==USB) return Stat;
	else if(drv==SD) return SDStat;
	else if(drv==TF) return TFStat;
	return STA_NOINIT;
}

//------------------------------------------
// Function   : disk_read				
// Description: Read Sector(s)					
//------------------------------------------
DRESULT disk_read (BYTE drv,BYTE *buff,DWORD sector,BYTE count)
{
	BYTE re;
	DRESULT status = RES_ERROR;
	
	if ((drv>2) || !count) return RES_PARERR;
	
	diskoperatef = 1;
	if(drv==USB){
		if ((Stat&STA_NOINIT)){status =  RES_NOTRDY;}
		else{
			re = USBH_UDISK_Read(buff,sector,count);
			if(re==0)status = RES_OK;else status = RES_ERROR;
		}
	}else if(drv==SD){
		if ((SDStat&STA_NOINIT)){status =  RES_NOTRDY;}
		else if(Sdindelay==SDTFRDY){
			re = SD_ReadDisk((uint8_t *)buff,sector,1);
			if(re==0)status = RES_OK;else status = RES_ERROR;
		}
	}else if(drv==TF){
		if ((TFStat&STA_NOINIT)){status =  RES_NOTRDY;}
		else if(Tfindelay==SDTFRDY){
			re = SD_ReadSingleBlock(sector,(uint8_t *)buff);
			if(re==0)status = RES_OK;else status = RES_ERROR;
		}		
	}
	diskoperatef = 0;
	
	return status;  
}

//------------------------------------------
// Function   : disk_write				
// Description: Write Sector(s)					
//------------------------------------------
#if _READONLY == 0
DRESULT disk_write (BYTE drv,const BYTE *buff,DWORD sector,BYTE count)
{
	BYTE re;
	DRESULT status = RES_ERROR;
		
	if ((drv>2) || !count) return RES_PARERR;
	wdt();
	diskoperatef = 1;
	if(drv==USB){
		if (Stat & STA_NOINIT){status = RES_NOTRDY;}
		else if (Stat & STA_PROTECT){status = RES_WRPRT;}
		else{
			re = USBH_UDISK_Write((u8*)buff,sector,count);
			if(re==0)status = RES_OK;else status = RES_ERROR;
		}
	}else if(drv==SD){
		if ((SDStat&STA_NOINIT)){status = RES_NOTRDY;}
		else if((SDStat&STA_PROTECT)){status = RES_WRPRT;}
		else if(Sdindelay==SDTFRDY){
			re = SD_WriteDisk((uint8_t *)buff,sector,1);
			if(re==0)status = RES_OK;else status = RES_ERROR;
		}
	}else if(drv==TF){
		if ((TFStat&STA_NOINIT)){status = RES_NOTRDY;}
		else if ((TFStat&STA_PROTECT)){status = RES_WRPRT;}
		else if(Tfindelay==SDTFRDY){
			re = SD_WriteSingleBlock(sector,(uint8_t *)buff);
			if(re==0)status = RES_OK;else status = RES_ERROR;
		}		
	}
	diskoperatef = 0;
	
	return status;
}
#endif		// _READONLY == 0

//------------------------------------------
// Function   : disk_ioctl				
// Description: Miscellaneous Functions					
//------------------------------------------
#if _USE_IOCTL != 0
DRESULT disk_ioctl (BYTE drv,BYTE ctrl,void *buff)
{
	DRESULT res = RES_OK;
	if (drv>1) return RES_PARERR;
	wdt();
	res = RES_ERROR;
	if(drv==0)			//USB
	{
		if (Stat & STA_NOINIT) return RES_NOTRDY;	
			
		switch (ctrl) 
		{
			case CTRL_SYNC :		// Make sure that no pending write process
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT :	// Get number of sectors on the disk (DWORD)
				*(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;	
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE :	// Get R/W sector size (WORD)
				*(WORD*)buff = 512;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE :	// Get erase block size in unit of sector (DWORD)
				*(DWORD*)buff = 512;
				break;
			default:
				res = RES_PARERR;
		}
	}
	else		//SD
	{
		if (SDStat & STA_NOINIT) return RES_NOTRDY;	
		
		switch (ctrl) 
		{
			case CTRL_SYNC :		// Make sure that no pending write process
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT :	// Get number of sectors on the disk (DWORD)
				*(DWORD*)buff = SDCardInfo.CardCapacity/512;
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE :	// Get R/W sector size (WORD)
				*(WORD*)buff = 512;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE :	// Get erase block size in unit of sector (DWORD)
				*(WORD*)buff = SDCardInfo.CardBlockSize;
				break;
			default:
				res = RES_PARERR;
		}		
	}	

	


	return res;
}
#endif // _USE_IOCTL != 0


