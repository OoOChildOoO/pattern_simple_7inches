

//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  sdio.h									*
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

#ifndef __SDIO_H   	  
#define __SDIO_H

#include "stm32cpu.h"



//用户配置区			  
//SDIO时钟计算公式:SDIO_CK时钟=SDIOCLK/[clkdiv+2];其中,SDIOCLK固定为48Mhz
//使用DMA模式的时候,传输速率可以到48Mhz(bypass on时),不过如果你的卡不是高速
//卡,可能也会出错,出错就请降低时钟
#define SDIO_INIT_CLK_DIV        0xf0 		//SDIO初始化频率，最大400Kh  0x76
#define SDIO_TRANSFER_CLK_DIV    0x10		//SDIO传输频率,该值太小可能会导致读写文件出错 
										 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
//SDIO工作模式定义,通过SD_SetDeviceMode函数设置.
#define SD_POLLING_MODE    	0  	//查询模式,该模式下,如果读写有问题,建议增大SDIO_TRANSFER_CLK_DIV的设置.
#define SD_DMA_MODE    		1	//DMA模式,该模式下,如果读写有问题,建议增大SDIO_TRANSFER_CLK_DIV的设置.  


#define SDIO_FLAG_CCRCFAIL                  ((U32)0x00000001)
#define SDIO_FLAG_DCRCFAIL                  ((U32)0x00000002)
#define SDIO_FLAG_CTIMEOUT                  ((U32)0x00000004)
#define SDIO_FLAG_DTIMEOUT                  ((U32)0x00000008)
#define SDIO_FLAG_TXUNDERR                  ((U32)0x00000010)
#define SDIO_FLAG_RXOVERR                   ((U32)0x00000020)
#define SDIO_FLAG_CMDREND                   ((U32)0x00000040)
#define SDIO_FLAG_CMDSENT                   ((U32)0x00000080)
#define SDIO_FLAG_DATAEND                   ((U32)0x00000100)
#define SDIO_FLAG_STBITERR                  ((U32)0x00000200)
#define SDIO_FLAG_DBCKEND                   ((U32)0x00000400)
#define SDIO_FLAG_CMDACT                    ((U32)0x00000800)
#define SDIO_FLAG_TXACT                     ((U32)0x00001000)
#define SDIO_FLAG_RXACT                     ((U32)0x00002000)
#define SDIO_FLAG_TXFIFOHE                  ((U32)0x00004000)
#define SDIO_FLAG_RXFIFOHF                  ((U32)0x00008000)
#define SDIO_FLAG_TXFIFOF                   ((U32)0x00010000)
#define SDIO_FLAG_RXFIFOF                   ((U32)0x00020000)
#define SDIO_FLAG_TXFIFOE                   ((U32)0x00040000)
#define SDIO_FLAG_RXFIFOE                   ((U32)0x00080000)
#define SDIO_FLAG_TXDAVL                    ((U32)0x00100000)
#define SDIO_FLAG_RXDAVL                    ((U32)0x00200000)
#define SDIO_FLAG_SDIOIT                    ((U32)0x00400000)
#define SDIO_FLAG_CEATAEND                  ((U32)0x00800000)


//SDIO 各种错误枚举定义
typedef enum
{	 
	//特殊错误定义 
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),
	//标准错误定义
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,  
	SD_ERROR,  
	SD_OK = 0 
} SD_Error;		

//SD卡CSD寄存器数据		  
typedef struct
{
	U8  CSDStruct;            /*!< CSD structure */
	U8  SysSpecVersion;       /*!< System specification version */
	U8  Reserved1;            /*!< Reserved */
	U8  TAAC;                 /*!< Data read access-time 1 */
	U8  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	U8  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	U16 CardComdClasses;      /*!< Card command classes */
	U8  RdBlockLen;           /*!< Max. read data block length */
	U8  PartBlockRead;        /*!< Partial blocks for read allowed */
	U8  WrBlockMisalign;      /*!< Write block misalignment */
	U8  RdBlockMisalign;      /*!< Read block misalignment */
	U8  DSRImpl;              /*!< DSR implemented */
	U8  Reserved2;            /*!< Reserved */
	U32 DeviceSize;           /*!< Device Size */
	U8  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	U8  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	U8  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	U8  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	U8  DeviceSizeMul;        /*!< Device size multiplier */
	U8  EraseGrSize;          /*!< Erase group size */
	U8  EraseGrMul;           /*!< Erase group size multiplier */
	U8  WrProtectGrSize;      /*!< Write protect group size */
	U8  WrProtectGrEnable;    /*!< Write protect group enable */
	U8  ManDeflECC;           /*!< Manufacturer default ECC */
	U8  WrSpeedFact;          /*!< Write speed factor */
	U8  MaxWrBlockLen;        /*!< Max. write data block length */
	U8  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	U8  Reserved3;            /*!< Reserded */
	U8  ContentProtectAppli;  /*!< Content protection application */
	U8  FileFormatGrouop;     /*!< File format group */
	U8  CopyFlag;             /*!< Copy flag (OTP) */
	U8  PermWrProtect;        /*!< Permanent write protection */
	U8  TempWrProtect;        /*!< Temporary write protection */
	U8  FileFormat;           /*!< File Format */
	U8  ECC;                  /*!< ECC code */
	U8  CSD_CRC;              /*!< CSD CRC */
	U8  Reserved4;            /*!< always 1*/
} SD_CSD;   

//SD卡CID寄存器数据
typedef struct
{
	U8  ManufacturerID;       /*!< ManufacturerID */
	U16 OEM_AppliID;          /*!< OEM/Application ID */
	U32 ProdName1;            /*!< Product Name part1 */
	U8  ProdName2;            /*!< Product Name part2*/
	U8  ProdRev;              /*!< Product Revision */
	U32 ProdSN;               /*!< Product Serial Number */
	U8  Reserved1;            /*!< Reserved1 */
	U16 ManufactDate;         /*!< Manufacturing Date */
	U8  CID_CRC;              /*!< CID CRC */
	U8  Reserved2;            /*!< always 1 */
} SD_CID;	 
//SD卡状态
typedef enum
{
	SD_CARD_READY                  = ((uint32_t)0x00000001),
	SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
	SD_CARD_STANDBY                = ((uint32_t)0x00000003),
	SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
	SD_CARD_SENDING                = ((uint32_t)0x00000005),
	SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
	SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
	SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
	SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;

//SD卡信息,包括CSD,CID等数据
typedef struct
{
	SD_CSD SD_csd;
	SD_CID SD_cid;
	long long CardCapacity;  	//SD卡容量,单位:字节,最大支持2^64字节大小的卡.
	U32 CardBlockSize; 			//SD卡块大小	
	U16 RCA;					//卡相对地址
	U8 CardType;				//卡类型
} SD_CardInfo;
extern SD_CardInfo SDCardInfo;//SD卡信息	


//SDIO 指令集
#define SD_CMD_GO_IDLE_STATE                       ((U8)0)
#define SD_CMD_SEND_OP_COND                        ((U8)1)
#define SD_CMD_ALL_SEND_CID                        ((U8)2)
#define SD_CMD_SET_REL_ADDR                        ((U8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((U8)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((U8)5)
#define SD_CMD_HS_SWITCH                           ((U8)6)
#define SD_CMD_SEL_DESEL_CARD                      ((U8)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((U8)8)
#define SD_CMD_SEND_CSD                            ((U8)9)
#define SD_CMD_SEND_CID                            ((U8)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((U8)11) /*!< SD Card doesn't sUpport it */
#define SD_CMD_STOP_TRANSMISSION                   ((U8)12)
#define SD_CMD_SEND_STATUS                         ((U8)13)
#define SD_CMD_HS_BUSTEST_READ                     ((U8)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((U8)15)
#define SD_CMD_SET_BLOCKLEN                        ((U8)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((U8)17)
#define SD_CMD_READ_MULT_BLOCK                     ((U8)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((U8)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((U8)20) 
#define SD_CMD_SET_BLOCK_COUNT                     ((U8)23) 
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((U8)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((U8)25)
#define SD_CMD_PROG_CID                            ((U8)26)
#define SD_CMD_PROG_CSD                            ((U8)27)
#define SD_CMD_SET_WRITE_PROT                      ((U8)28)
#define SD_CMD_CLR_WRITE_PROT                      ((U8)29)
#define SD_CMD_SEND_WRITE_PROT                     ((U8)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((U8)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((U8)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((U8)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((U8)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((U8)38)
#define SD_CMD_FAST_IO                             ((U8)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((U8)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((U8)42)
#define SD_CMD_APP_CMD                             ((U8)55)
#define SD_CMD_GEN_CMD                             ((U8)56)
#define SD_CMD_NO_CMD                              ((U8)64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD ：CMD55 should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((U8)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((U8)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((U8)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((U8)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((U8)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((U8)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((U8)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((U8)53) /*!< For SD I/O Card only */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_SD_APP_GET_MKB                      ((U8)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((U8)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((U8)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((U8)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((U8)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((U8)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((U8)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((U8)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((U8)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((U8)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((U8)48) /*!< For SD Card only */
  			   
//支持的SD卡定义
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((U32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((U32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((U32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((U32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((U32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((U32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((U32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((U32)0x00000007)

//SDIO相关参数定义
#define NULL 0
#define SDIO_STATIC_FLAGS               ((U32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((U32)0x00100000)	  
#define SDIO_DATATIMEOUT                ((U32)0xFFFFFFFF)	  
#define SDIO_FIFO_Address               ((U32)0x40018080)

//Mask for errors Card Status R1 (OCR Register)  
#define SD_OCR_ADDR_OUT_OF_RANGE        ((U32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((U32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((U32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((U32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((U32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((U32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((U32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((U32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((U32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((U32)0x00200000)
#define SD_OCR_CC_ERROR                 ((U32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((U32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((U32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((U32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((U32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((U32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((U32)0x00004000)
#define SD_OCR_ERASE_RESET              ((U32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((U32)0x00000008)
#define SD_OCR_ERRORBITS                ((U32)0xFDFFE008)

//Masks for R6 Response 
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((U32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((U32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((U32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((U32)0x80100000)
#define SD_HIGH_CAPACITY                ((U32)0x40000000)
#define SD_STD_CAPACITY                 ((U32)0x00000000)
#define SD_CHECK_PATTERN                ((U32)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC           ((U32)0x80FF8000)

#define SD_MAX_VOLT_TRIAL               ((U32)0x0000FFFF)
#define SD_ALLZERO                      ((U32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((U32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((U32)0x00010000)
#define SD_CARD_LOCKED                  ((U32)0x02000000)
#define SD_CARD_PROGRAMMING             ((U32)0x00000007)
#define SD_CARD_RECEIVING               ((U32)0x00000006)
#define SD_DATATIMEOUT                  ((U32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((U32)0x000000FF)
#define SD_8TO15BITS                    ((U32)0x0000FF00)
#define SD_16TO23BITS                   ((U32)0x00FF0000)
#define SD_24TO31BITS                   ((U32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((U32)0x01FFFFFF)

#define SD_HALFFIFO                     ((U32)0x00000008)
#define SD_HALFFIFOBYTES                ((U32)0x00000020)

//Command Class Supported  
#define SD_CCCC_LOCK_UNLOCK             ((U32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((U32)0x00000040)
#define SD_CCCC_ERASE                   ((U32)0x00000020)
																	 
//CMD8指令
#define SDIO_SEND_IF_COND               ((U32)0x00000008)


















extern SD_Error SD_Init(void);
extern void SD_ReSet(void);
extern U8 SD_ReadDisk(U8*buf,U32 sector,U32 cnt); 	//读SD卡,fatfs/usb调用
extern U8 SD_WriteDisk(U8*buf,U32 sector,U32 cnt);	//写SD卡,fatfs/usb调用

void SDIO_Clock_Set(U8 clkdiv);
void SDIO_Send_Cmd(U8 cmdindex,U8 waitrsp,U32 arg);
void SDIO_Send_Data_Cfg(U32 datatimeout,U32 datalen,U8 blksize,U8 dir);
SD_Error SD_PowerON(void);    
SD_Error SD_PowerOFF(void);
SD_Error SD_InitializeCards(void);
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);		  
SD_Error SD_EnableWideBusOperation(U32 wmode);
SD_Error SD_SetDeviceMode(U32 mode);
SD_Error SD_SelectDeselect(U32 addr); 
SD_Error SD_SendStatus(U32 *pcardstatus);
SDCardState SD_GetState(void);  
SD_Error SD_ReadBlocks(U8 *buf,long long  addr,U16 blksize,U32 nblks);   
SD_Error SD_WriteBlocks(U8 *buf,long long addr,U16 blksize,U32 nblks);
SD_Error SD_ProcessIRQSrc(void);
SD_Error CmdError(void);  
SD_Error CmdResp7Error(void);
SD_Error CmdResp1Error(U8 cmd);
SD_Error CmdResp3Error(void);
SD_Error CmdResp2Error(void);
SD_Error CmdResp6Error(U8 cmd,U16*prca);  
SD_Error SDEnWideBus(U8 enx);	  
SD_Error IsCardProgramming(U8 *pstatus); 
SD_Error FindSCR(U16 rca,U32 *pscr); 
void SD_DMA_Config(U32*mbuf,U32 bufsize,U8 dir); 






#endif
