//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  communication.h							*
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

#include <string.h>
#include "communication.h"
#include "register.h"
#include "uart.h"
#include "menu.h"
#include "utility.h"
#include "iap.h"
#include "main.h"

#define UART_ADD 1
//#define RGB(r, g, b) (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3))

volatile U8 rsdata[300];

//通讯协议与寄存器参数
const U32 D_Length[26] = {
	//  1   2   3   4   5   6    7    8   9  
	0, 64, 64, 64, 64, 64, 64, 128, 128, 64,
	//  11    12   13   14   15    16    17  18      19  
	64, 64, 1024, 256, 192, 192, 1024, 3200, 64, 184144,
	//    21   22     23	24	     25
	192, 384, 640, 42000, 1280, MAXLREG};
const U32 D_Address[26] = {
	//1  2  3   4   5   6   7   8   9  10  11   12   13  14   15   16   17   18   19   20      21      22		23	  24	25
	0, 0, 64, 128, 192, 256, 320, 384, 512, 640, 704, 768, 832, 1856, 2112, 2304, 2496, 3520, 6720, 6784, 190928, 191120, 191504, 192144, 234144, 236472};
const U8 D_Command[26] = {
  //0  1  2  3  4  5  6  7  8  9    10 11 12    13 14 15    16    17    18    19    20    21    22    23    24	   25
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0x66, 0, 0, 0x71, 0, 0, 0x00, 0x65, 0x69, 0x68, 0x70, 0x69, 0x69, 0x69, 0x36, 0x69, 0x69};

extern volatile U8 testdat;
extern volatile U16 commtimes;
extern volatile U16 commtime1ms;
extern const U8 systypel[];
extern const U8 systypeh[];
extern ProInfo g_ProInfo;
extern volatile ERRMSG g_errMsg;

volatile U8 ComErrMessage = 0;

extern void Select_ExtMemory(uint8_t usbsd);
extern uint8_t ExtMemory_Status(uint8_t usbsd);
extern uint8_t Read_ProgramFile(uint8_t list, uint8_t num, uint32_t *sumbyte, PROGRESSBAR pgbar);
extern void DrawProgressbar(U32 x, U32 y, U32 dx, U32 dy, U8 percent, U16 color, U16 bkcolor);
extern U8 isContain_U16(U16 *buff, U16 len, U16 dat);
//------------------------------------------
// Function   : U8 wait_rsd(void)
// Description:  等待接收正确写入DREG
//------------------------------------------
U8 wait_rsd(void)
{
	U16 i;

	commtimes = 0;
	while (commtimes < 100)
	{
		wdt();
		if (receivef == 1)// 如果有待处理数据
		{ 
			if (uart2_rbuf[1] == 0x03)
			{
				i = uart2_rbuf[2] + 3;
			}
			else
			{
				i = 6;
			}
			if (CRC16((U8 *)(uart2_rbuf), i, 1))
			{
				com_err = 0;
				//				printf("%0x \r\n",DMA1_Stream5->M0AR);
				//				printf("com_err0 \r\n");
				return (1);
			}
			else
			{
				com_err++;
				//				printf("com_err1 \r\n");
				return (0);
			}
		}
		else if (receivef > 1)
		{
			//			printf("com_err2 \r\n");
			com_err++;
			return (0);
		}
	}
	com_err += 2;
	//	printf("com_err3 \r\n");
	//
	//	printf("DMA1_Stream5->CR=%0x \r\n",DMA1_Stream5->CR);
	//	printf("DMA1_Stream5->PAR=%0x \r\n",DMA1_Stream5->PAR);
	//	printf("DMA1_Stream5->M0AR=%0x \r\n",DMA1_Stream5->M0AR);
	//	printf("DMA1_Stream5->M1AR=%0x \r\n",DMA1_Stream5->M1AR);
	//	printf("DMA1_Stream5->NDTR=%0x \r\n",DMA1_Stream5->NDTR);
	//	printf("DMA1_Stream5->FCR=%0x \r\n",DMA1_Stream5->FCR);
	//	printf("USART2->SR=%0x \r\n",USART2->SR);
	//	printf("USART2->DR=%0x \r\n",USART2->DR);
	//	printf("USART2->CR1=%0x \r\n",USART2->CR1);
	//	printf("USART2->CR2=%0x \r\n",USART2->CR2);
	//	printf("USART2->CR3=%0x \r\n",USART2->CR3);
	//	printf("USART2->BRR=%0x \r\n",USART2->BRR);
	USART2->SR = 0;

	return (0);
}

//------------------------------------------
// Function   : U8 Write_D(U32 Dn,U32 len,U16 *add)
// Description: 发送D数据DREG D0-D511 1000H-13FFH
//------------------------------------------
U8 Write_D(U32 Dn, U32 len, U16 *add)
{
	U16 *p;
	U32 d_add;
	U8 re = 0;
	U8 l, n;

	U32 i = 0;

	//	if(P_LCD==0)return(1); 					// 不连接

	for (n = 1; n < 24; n++)
	{
		if (Dn < D_Address[n + 1])
			break;
	}
	p = add;
	d_add = Dn;
	if (len > D_Length[n])
		len = D_Length[n];
	//	if(n==19){
	//		printf("Dn = %d \r\n",Dn);
	//		printf("Len = %d \r\n",len);
	//		printf("Add = %0x \r\n",(U32)(add));
	//	}
	while (i < len)
	{
		wdt();
		if ((len - i) >= COMWDNUM)
			l = COMWDNUM;
		else
			l = len - i;
		re = Write_DS(D_Command[n], d_add, (U16)(l), p);
		i += COMWDNUM;
		p += COMWDNUM;
		d_add += COMWDNUM;
		if (re == FALSE)
			break;
		//	DelayMs(1);
		DelayUs(200);
	}
	return (re);
}

//------------------------------------------
// Function   : U8 Read_D(U16 Dn,U32 len,U16 *add)
// Description: 读取D数据 DREG D0-D511 1000H-13FFH
//------------------------------------------
U8 Read_D(U32 Dn, U32 len, U16 *add)
{
	U16 *p;
	U32 d_add;
	U8 re = 0;
	U8 l, n;

	U32 i = 0;

	//if(P_LCD==0){DREG[D_CMDSTATUS]=100;return(1);} 	// 不连接
	for (n = 1; n < 23; n++)
	{
		if (Dn < D_Address[n + 1])
			break;
	}
	p = add;
	d_add = Dn;
	if (len > D_Length[n])
		len = D_Length[n];
	while (i < len)
	{
		wdt();
		if ((len - i) >= COMWDNUM)
			l = COMWDNUM;
		else
			l = len - i;
		re = Read_DS(D_Command[n], d_add, (U16)(l), p);
		i += COMWDNUM;
		p += COMWDNUM;
		d_add += COMWDNUM;
		if (re == FALSE)
			break;
		//DelayMs(1);
		DelayUs(200);
	}
	return (re);
}

//------------------------------------------
// Function   : U8 Read_D(U16 Dn,U32 len,U16 *add)
// Description: 读取D数据DREG D0-D511 1000H-13FFH
//------------------------------------------
U8 Read_Dat(U32 Dn, U32 len, U16 *add)
{

	U16 *p;
	U32 d_add;
	U8 re = 0;
	U8 l;

	U32 i = 0;

	//if(P_LCD==0){DREG[D_CMDSTATUS]=100;return(1);} 	// 不连接
	p = add;
	d_add = Dn;
	while (i < len)
	{
		wdt();
		if ((len - i) >= COMWDNUM)
			l = COMWDNUM;
		else
			l = len - i;
		re = Read_DS(0, d_add, (U16)(l), p);
		i += COMWDNUM;
		p += COMWDNUM;
		d_add += COMWDNUM;
		if (re == FALSE)
			break;
		//DelayMs(1);
		DelayUs(200);
	}
	return (re);
}

//------------------------------------------
// Function   : U8 Write_DS(U8 comm, U32 Dn, U16 len, U16 *p)
// Description: 发送数据
//------------------------------------------
U8 Write_DS(U8 comm, U32 Dn, U16 len, U16 *p)
{
	U8 i;
	U8 Rep;
	U16 *pn;
	U16 add, dat;

	pn = p;

	rsdata[0] = UART_ADD; // 从机地址
	rsdata[1] = comm;	  // 指令
	rsdata[2] = (Dn >> 16) % 256;
	rsdata[3] = (Dn & 0x0000ffff) / 256;
	rsdata[4] = (Dn & 0x0000ffff) % 256;
	rsdata[5] = 0; //len%256;
	rsdata[6] = (U8)(len * 2);
	add = 7;
	for (i = 0; i < len; i++)// 重复发送字节数
	{ 
		dat = (*pn);
		pn++;
		rsdata[add++] = dat / 256;
		rsdata[add++] = dat % 256;
	}

	wdt();
	CRC16((U8 *)(rsdata), add, 0);
	add += 2;
	if (com_err < 30)
		Rep = 5;
	else
		Rep = 1; // 重复次数
	while (Rep--)// 每次减一
	{ 
		wdt();
		receivef = 0;
		UART2_DMASendBuf(add, (U8 *)(rsdata));
		//UART2_ISendBuf(add, (U8 *)(rsdata));	// 发送一次数据
		if (wait_rsd())
		{
			return (1); // 等待接收正确
		}
		//if(com_err>50)DelayMs(20);
		//else DelayMs(2);
	}
#ifdef SYSTEM0
	if (com_err > 30)
	{
		if (ComErrMessage == 0)
		{
			ComErrMessage = 1;
			poldno[pnoindex] = pno;
			pnoindex++;
			pno = MENU_RWHELP;
			hleppara = 60;
		}
	}
#endif

#ifdef SYSTEM1
	if (com_err > 30)
	{
		if (ComErrMessage == 0)
		{
			ComErrMessage = 1;
			poldno[pnoindex] = pno;
			pnoindex++;
			pno = MENU_RWHELP;
			hleppara = 60;
		}
	}
#endif

#ifdef SYSTEM2
	if (com_err > 30)
	{
		if (ComErrMessage == 0)
		{
			ComErrMessage = 1;
			poldno[pnoindex++] = pno;
			pno = MENU_BOR_HELP;
			hleppara = 78;
		}
	}
#endif

	return (0); // 不能正确接收
}

//------------------------------------------
// Function   : U8 Read_DS(U8 comm, U32 Dn, U16 len, U16 *p)
// Description: 读取数据
//------------------------------------------
U8 Read_DS(U8 comm, U32 Dn, U16 len, U16 *p)
{
	U8 i;
	U8 add = 0, Rep;
	U16 *pn;
	U16 dat;

	pn = p;

	rsdata[0] = UART_ADD; // 从机地址
	rsdata[1] = 0x3;	  // 指令
	rsdata[2] = (Dn >> 16) % 256;
	rsdata[3] = (Dn & 0x0000ffff) / 256;
	rsdata[4] = (Dn & 0x0000ffff) % 256;
	rsdata[5] = len % 256;

	wdt();
	CRC16((U8 *)(rsdata), 6, 0);
	add += 2;
	if (com_err < 30)
		Rep = 5;
	else
		Rep = 1; // 重复次数
	while (Rep--)
	{ // 每次减一
		wdt();
		receivef = 0;
		UART2_DMASendBuf(8, (U8 *)(rsdata));
		//UART2_ISendBuf(8,(U8 *)(rsdata));		// 发送一次数据
		if (wait_rsd())
		{
			if ((uart2_rbuf[1] == 0x3) && (uart2_rbuf[2] == (len * 2)))
			{
				add = 3;
				for (i = 0; i < len; i++)
				{
					dat = uart2_rbuf[add++];
					*(pn) = (dat << 8) + uart2_rbuf[add++];
					pn++;
				}
				return (1); // 等待接收正确
			}
		}
		//if(com_err>50)DelayMs(20);
		//else DelayMs(2);
	}
#ifdef SYSTEM0
	if (com_err > 50)
	{
		if (ComErrMessage == 0)
		{
			ComErrMessage = 1;
			poldno[pnoindex] = pno;
			pnoindex++;
			pno = MENU_RWHELP;
			hleppara = 60;
		}
	}
#endif

#ifdef SYSTEM1
	if (com_err > 50)
	{
		if (ComErrMessage == 0)
		{
			ComErrMessage = 1;
			poldno[pnoindex] = pno;
			pnoindex++;
			pno = MENU_RWHELP;
			hleppara = 60;
		}
	}
#endif

#ifdef SYSTEM2
	if (com_err > 50)
	{
		if (ComErrMessage == 0)
		{
			ComErrMessage = 1;
			poldno[pnoindex++] = pno;
			pno = MENU_BOR_HELP;
			hleppara = 78;
		}
	}
#endif

	return (0); // 不能正确接收
}

//------------------------------------------
// Function   : U8 Write_Cmd(U16 cmd,U16 para1,U16 para2)
// Description: 发送指令  发送2个指令参数
//------------------------------------------
U8 Write_Cmd(U16 cmd, U16 para1, U16 para2)
{
	U8 re;
	//	#if Debug
	//	if(cmd==C_DISK_STATUS)
	//		printf("SEND :%d\r\n",C_DISK_STATUS);
	//	#endif
	DREG[D_COMMAND] = cmd;
	DREG[D_CMDSTATUS] = 0;
	DREG[D_CMDPARA1] = para1;
	DREG[D_CMDPARA2] = para2;
	re = Write_D(D_Address[DISP] + D_COMMAND, 4, (U16 *)(&DREG[D_COMMAND]));

	return (re);
}

//------------------------------------------
// Function   : U8 Write_Cmd2(U16 cmd,U16 para1,U16 para2)
// Description: 发送指令  发送3个指令参数
//------------------------------------------
U8 Write_Cmd2(U16 cmd, U16 para1, U16 para2, U16 para3)
{
	U8 re;
	//	#if Debug
	//	printf("cmd :%d  para1:%d\r\n",cmd,para1);
	//	#endif
	DREG[D_COMMAND] = cmd;
	DREG[D_CMDSTATUS] = 0;
	DREG[D_CMDPARA1] = para1;
	DREG[D_CMDPARA2] = para2;
	DREG[D_CMDPARA3] = para3;
	re = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND]));

	return (re);
}

//------------------------------------------
// Function   : U8 Write_Cmd2(U16 cmd,U16 para1,U16 para2)
// Description: 发送指令  发送4个指令参数
//------------------------------------------
U8 Write_Cmd3(U16 cmd, U16 para1, U16 para2, U16 para3, U16 para4)
{
	U8 re;
	//	#if Debug
	//		printf("cmd :%d\r\n",cmd);
	//	#endif
	DREG[D_COMMAND] = cmd;
	DREG[D_CMDSTATUS] = 0;
	DREG[D_CMDPARA1] = para1;
	DREG[D_CMDPARA2] = para2;
	DREG[D_CMDPARA3] = para3;
	DREG[D_CMDPARA4] = para4;
	re = Write_D(D_Address[DISP] + D_COMMAND, 6, (U16 *)(&DREG[D_COMMAND]));

	return (re);
}

//------------------------------------------
// Function   : U8 Read_Status(void)
// Description: 判断指令是否执行	01 03 00 02 95 01 4A 9A
// note: 固定连续读取次数
//------------------------------------------
U16 Read_Status(void)
{
	U8 re;
	U16 i;

	for (i = 0; i < RDTIMES; i++)
	{
		re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
		//if((DREG[D_CMDSTATUS]) == 100){return(1);}
		if ((DREG[D_CMDSTATUS]) && (re))
		{
			return (DREG[D_CMDSTATUS]);
		}
		DelayMs(WAITTIME);
	}

	return (0);
}

//------------------------------------------
// Function   : U8 Read_Status(void)
// Description: 判断指令是否执行	01 03 00 02 95 01 4A 9A
// note: 可设置连续读取次数
//------------------------------------------
U16 Read1_Status(U8 times)
{
	U8 re;
	U16 i;

	for (i = 0; i < (times + 1) * RDTIMES; i++)
	{
		re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
		//if((DREG[D_CMDSTATUS]) == 100){return(1);}
		if ((DREG[D_CMDSTATUS]) && (re))
		{
			return (DREG[D_CMDSTATUS]);
		}
		DelayMs(WAITTIME);
	}

	//	#if Debug
	//	printf("Read1_Status time out\r\n");
	//	#endif
	return (0);
}
//------------------------------------------
// Function   : U8 Write_Cmd1(U16 cmd,U16 para1,U16 para2)
// Description: 发送指令  DISP  打板器数据交换指令
// note: 固定连续读取次数
//------------------------------------------
U8 Write_Cmd1(U16 cmd, U16 para1, U16 para2)
{
	//	#if Debug
	//	printf("Write_Cmd1(%d,%d,%d) \r\n",cmd,para1,para2);
	//	#endif
	if (Write_Cmd(cmd, para1, para2))
	{
		if (Read_Status() == 100)
			return (1);
		else
			return (0);
	}
	return (0);
}

//------------------------------------------
// Function   : U8 Write_Cmd1(U16 cmd,U16 para1,U16 para2)
// Description: 发送指令 DISP  打板器数据交换指令
// note: 可调整连续读取次数
//------------------------------------------
U8 Write1_Cmd1(U16 cmd, U16 para1, U16 para2, U8 wtime)
{
	//	#if Debug
	//	printf("Write1_Cmd1(%d,%d,%d) \r\n",cmd,para1,para2);
	//	#endif
	if (Write_Cmd(cmd, para1, para2))
	{
		if (Read1_Status(wtime) == 100)
			return (1);
		else
			return (0);
	}
	return (0);
}
//------------------------------------------
// Function   : void  ErrorMsg(U32 tmpbpon)
// Description: 显示错误信息
//------------------------------------------
void ErrorMsg(U32 tmpbpon)
{
	U16 SHIELDMSG = 0;
	static U16 oSHIELDMSG = 0;

	if (commtime1ms > 300)
	{
		commtime1ms = 0;
		Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
		if (S_ERROR)
		{
			Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
			if (DREG[D_ERRWAR])
			{
				if (DREG[D_ERRWAR] - 0x8000 == 392)
				{
					pno = MENU_FTCHAT;
					poldno[pnoindex] = tmpbpon;
					pnoindex++;
				}
				else
				{
					if ((g_errMsg.max == 0) || (isContain_U16((U16 *)g_errMsg.err, ERRMSGMAX, DREG[D_ERRWAR]) == 0))
					{ //不包包括屏蔽信息,报警
						pno = MENU_RWHELP;
						hleppara = 21;
						poldno[pnoindex] = tmpbpon;
						pnoindex++;
					}
				}
			}
		}
		SHIELDMSG = (U16)S_SHIELDMSG;
		if (oSHIELDMSG != SHIELDMSG)
		{
			oSHIELDMSG = SHIELDMSG;
			if (SHIELDMSG)
			{
				pno = MENU_RWHELP;
				hleppara = 149;
				poldno[pnoindex] = tmpbpon;
				pnoindex++;
			}
		}
	}
}

#include "disk_oper.h"
//------------------------------------------
// Function   : U8 ReadMboard_ProgramData(U8 list,U8 num,U32 *sumbyte,PROGRESSBAR pgbar)
// Description: 从主板USB读取升级包中升级数据	list:升级包中程序号(如 1:H款打版器 2:H款打版器FLH 3:H款其他,可放MP3语音数据)	num:小于250时是文件比较大,分多次读次序,大于250时是一次读完,等于0只读信息不读数据
//------------------------------------------
U8 ReadMboard_ProgramData(U8 list, U8 num, U32 *sumbyte, PROGRESSBAR pgbar, U8 netflag)
{
	U8 f;
	U32 i, n /*,j*/;
	U32 len, dat, dat1, add;
	U32 pn, tpn = 0, plen;
	U8 *Image_Buf;

	//---------------------
	*sumbyte = 0;
	flashcheck.hexlen = 0;
	if (pgbar.str > 100)
		pgbar.str = 100;
	if (pgbar.end > 100)
		pgbar.end = 100;
	if (pgbar.str < pgbar.end)
		plen = pgbar.end - pgbar.str;
	else
		plen = 0;
	//	#if Debug
	//	printf("ReadMboard_ProgramData  list = %d  netflag = %d \r\n",list,netflag);
	//	#endif
	if (Write1_Cmd1(netflag ? C_DOWNLOAD_NETCODE : C_DOWNLOAD_FLASH, 100, 0, netflag ? 4 : 0))
	{ // 读升级文件表索引 成功
		f = Read_D(D_Address[PAGEBUF], 512, (U16 *)(&IAPBuff[0]));
		if (f == 0)
		{
			return (1);
		}
		else
		{
			//#if Debug

			//			Image_Buf = (U8 *)IAPBuff;
			//			for(i =0;i<1024;i++){
			//				printf("%02x ",Image_Buf[i]);
			//				if((i+1)%24==0) printf("\r\n");
			//			}
			//#endif
			if (strcmp((const char *)(&IAPBuff[4]), "GZMSMMC_H") == 0)
			{
				if (strcmp((const char *)(&IAPBuff[260]), "GZMSMMC_H") == 0)
				{
					if (list < 20)
					{
						Image_Buf = (U8 *)IAPBuff;
						i = (list + 1) * 24 + 512;
						len = (((U32)Image_Buf[i + 6]) << 24) + (((U32)Image_Buf[i + 7]) << 16) + (((U32)Image_Buf[i + 8]) << 8) + ((U32)Image_Buf[i + 9]);
						if ((Image_Buf[i] == systypel[list]) && (Image_Buf[i + 1] == systypeh[list]) && (len > 0))
						{
							flashcheck.hexlen = len;
							flashcheck.hexadd = (Image_Buf[i + 2] << 24) + (Image_Buf[i + 3] << 16) + (Image_Buf[i + 4] << 8) + Image_Buf[i + 5];
							flashcheck.hexcrc = (Image_Buf[i + 10] << 8) + Image_Buf[i + 11];
							flashcheck.hexsum = (Image_Buf[i + 12] << 8) + Image_Buf[i + 13];
							flashcheck.hexxor = (Image_Buf[i + 14] << 8) + Image_Buf[i + 15];
						}
					}
					else if (list == 100)
					{
						return (0);
					}
				}
			}
		}
	}
	else
	{
		return (1);
	}
	if (num == 0)
	{
		dat = 0;
		dat1 = 0;
	}
	else if (num < 250)
	{
		dat = ((U32)MCPUUSBRDLEN) * (num - 1);
		dat1 = MCPUUSBRDLEN;
	} // 分多次读，最大可读8000K字节 50次160K
	else
	{
		dat = 0;
		dat1 = MCPUUSBRDLEN;
	} // 一次读完，最大可读8000K字节

#if Debug
	printf("flashcheck.hexlen = %d  dat = %d num = %d\r\n", flashcheck.hexlen, dat, num);
#endif
	if ((flashcheck.hexlen < dat) || (flashcheck.hexlen == 0))
	{
		return (1);
	}
	else if (num == 0)
	{
		if (Write1_Cmd1(netflag ? C_DOWNLOAD_NETCODE : C_DOWNLOAD_FLASH, list, 0, netflag ? 4 : 0) == 0)
		{
			printf("DREG[D_CMDSTATUS] = %d***\r\n", DREG[D_CMDSTATUS]);
			return (1);
		}
		Read_D(D_Address[PTDATA], 256, (U16 *)(IAPDATA_ADD));

		return (0);
	}
	len = flashcheck.hexlen - dat;
	if (dat1 > flashcheck.hexlen)
		dat1 = flashcheck.hexlen;
	if (dat1 > len)
		dat1 = len;

	len = 0;
	add = IAPDATA_ADD;
	dat /= (160 * 1024);
	n = (dat1 + (160 * 1024 - 1)) / (160 * 1024);

#if Debug
	printf("num = %d  n = %d \r\n", num, n);
#endif
	for (i = 0; i < n; i++)
	{
		if (Write1_Cmd1(netflag ? C_DOWNLOAD_NETCODE : C_DOWNLOAD_FLASH, list, (dat + i), netflag ? 4 : 0) == 0)
		{
			return (1);
		} // 发指令从主板USB读160K数据
		f = Read_D(D_Address[PTDATA], (160 * 512), (U16 *)(add));
		add += (160 * 1024);
		len += (160 * 1024);
		wdt();
		if (pgbar.mode == 0x5a)
		{
			pn = len * plen / dat1;
			if (pn != tpn)
			{
				tpn = pn;
				DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str + tpn, pgbar.color, pgbar.bkcolor);
			}
		}
	}
	*sumbyte = dat1;
	return (0);
}

//------------------------------------------
// Function   : U8 update_dispprogram(U8 selextm,U8 flag,U16 bkcolor)
// Description: 从升级包中读取数据升级主板程序		bkcolor:进度条底色
//------------------------------------------
U8 update_mcpuprogram(U8 selextm, U8 seltype, U8 flag, U16 bkcolor)
{
	U8 f = 0, t = 0;
	U32 i, m, l;
	U32 len;
	PROGRESSBAR pgbar;

	if (flag == 1)
	{
		pgbar.mode = 0x5a;
		pgbar.color = 0xf800; // Color_Red
		pgbar.bkcolor = bkcolor;
		pgbar.px = 220;
		pgbar.py = 328;
		pgbar.pdx = 328;
		pgbar.pdy = 32;
		pgbar.str = 0;
		pgbar.end = 0;
		DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
	}
	else if (flag == 2)
	{
		pgbar.mode = 0x5a;
		pgbar.color = 0xf800; // Color_Red
		pgbar.bkcolor = bkcolor;
#if LCD_TYPE == LCD_5cun
		pgbar.px = 240;
#endif
#if LCD_TYPE == LCD_8cun
		pgbar.px = 208;
#endif
		pgbar.py = 348;
		pgbar.pdx = 344;
		pgbar.pdy = 32;
		pgbar.str = 0;
		pgbar.end = 0;
	}
	else if (flag == 3)
	{
		pgbar.mode = 0x5a;
#if USEMULTCOLOR			  //彩色
		pgbar.color = 0xf800; // Color_Red
#else
		pgbar.color = RGB(135, 206, 235); // Color_Red
#endif
		pgbar.bkcolor = bkcolor;
		pgbar.px = 32;
		pgbar.py = 304;
		pgbar.pdx = 522;
		pgbar.pdy = 44;
		pgbar.str = 0;
		pgbar.end = 0;
		DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
	}
	else if (flag == 4)
	{
		pgbar.mode = 0x5a;
#if USEMULTCOLOR			  //彩色
		pgbar.color = 0xf800; // Color_Red
#else
		pgbar.color = RGB(135, 206, 235); // Color_Red
#endif
		pgbar.bkcolor = bkcolor;
		pgbar.px = 150;
		pgbar.py = 312;
		pgbar.pdx = 322;
		pgbar.pdy = 36;
		pgbar.str = 0;
		pgbar.end = 0;
		DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
	}
	else if (flag == 5)
	{
		pgbar.mode = 0x5a;
		pgbar.color = RGB(135, 206, 235);
		pgbar.bkcolor = bkcolor;
		pgbar.px = 48;
		pgbar.py = 212;
		pgbar.pdx = 600;
		pgbar.pdy = 40;
		pgbar.str = 0;
		pgbar.end = 0;
		DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
	}
	else if (flag == 6)
	{
		pgbar.mode = 0x5a;
		pgbar.color = RGB(255, 255, 255);
		pgbar.bkcolor = bkcolor;
		pgbar.px = 200;
		pgbar.py = 200;
		pgbar.pdx = 500;
		pgbar.pdy = 40;
		pgbar.str = 0;
		pgbar.end = 0;
		DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
	}
	else
	{
		pgbar.mode = 0;
	}

	if (selextm < 3)
	{ // selextm:0_USB 1_SD 2_TF
		if (ExtMemory_Status(selextm) != 0)
			return (FALSE);
		Select_ExtMemory(selextm);
		f = Read_ProgramFile((seltype + 4), 0, &len, pgbar);
	}
	else if (selextm == 3)
	{
		f = ReadMboard_ProgramData((seltype + 4), 0, &len, pgbar, 0);
	}

	if (f == 0)
	{ // 打开升级包,判断是否有FLASH数据,只读数据长度,暂不读数据
		if (selextm < 3)
		{
			l = (flashcheck.hexlen + (DISPUSBRDLEN - 1)) / DISPUSBRDLEN;
		} // 计算分几次读数据,每次可读8M Byte
		else
		{
			l = (flashcheck.hexlen + (MCPUUSBRDLEN - 1)) / MCPUUSBRDLEN;
		} // 主板USB,每次发指令读160K,缓冲区最大可连续读50次
		if (l != 1)
			return (FALSE); // 没有升级数据,直接退出
		pgbar.str = 0;

		if (selextm < 3)
		{ // selextm:0_USB 1_SD 2_TF
			pgbar.end = 20;
			f = Read_ProgramFile((seltype + 4), 0xff, &len, pgbar);
			if (f != 0)
				return (FALSE);
			i = flashcheck.hexlen;
			if (i & 0x1)
				i++;
			i >>= 1;
			l = 0;
			len = i;
			pgbar.str = 20;
			for (; i > 0;)
			{
				if (i > 5120)
					m = 5120;
				else
					m = i;
				//				#if Debug
				//				printf("D_Address[PTDATA]+l = %d + %d = %d \r\n",D_Address[PTDATA],l,D_Address[PTDATA]+l);
				//				#endif
				Write_D((D_Address[PTDATA] + l), m, (U16 *)((IAPDATA_ADD + l + l)));
				l += m;
				i -= m;
				DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, (pgbar.str + (l * 10 / len)), pgbar.color, pgbar.bkcolor);
			}
			IAPBuff[1] = flashcheck.hexadd >> 16;
			IAPBuff[2] = flashcheck.hexadd;
			IAPBuff[3] = flashcheck.hexlen >> 16;
			IAPBuff[4] = flashcheck.hexlen;
			IAPBuff[5] = flashcheck.hexcrc;
			IAPBuff[6] = flashcheck.hexsum;
			IAPBuff[7] = flashcheck.hexxor;
			Write_D(D_Address[PAGEBUF], 8, (U16 *)(&IAPBuff[0]));
			if (seltype == 15)
			{ //升级wifi
				Write_Cmd(214, 0xa5, 0x5a);
			}
			else
			{ //升级主板
				Write_Cmd(213, 0xa5, 0x5a);
			}
			pgbar.str = 30;
			pgbar.end = 100;
		}
		else if (selextm == 3)
		{ // 主板上USB
			Write_Cmd(212, (seltype + 4), 0x5a);
			pgbar.end = 100;
		}
		commtime1ms = 0;
		if (seltype == 15)
		{ //升级wifi
			while (pgbar.str < pgbar.end)
			{
				if ((commtime1ms > 120) && (t == 0))
				{
					t = 1;
					f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
					if ((DREG[D_CMDSTATUS] == 100) && (f))
					{
						break;
					}
					else if ((DREG[D_CMDSTATUS] == 2) && (f))
					{
						break;
					}
				}
				else if (commtime1ms > 240)
				{
					t = 0;
					commtime1ms = 0;
					pgbar.str++;
					f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
					if ((DREG[D_CMDSTATUS] == 100) && (f))
					{
						break;
					}
					else if ((DREG[D_CMDSTATUS] == 2) && (f))
					{
						break;
					}
					DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
				}
				wdt();
			}
		}
		else
		{ //升级主板
			while (pgbar.str < pgbar.end)
			{
				if ((commtime1ms > 60) && (t == 0))
				{
					t = 1;
					f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
					if ((DREG[D_CMDSTATUS] == 100) && (f))
					{
						break;
					}
					else if ((DREG[D_CMDSTATUS] == 2) && (f))
					{
						break;
					}
				}
				else if (commtime1ms > 120)
				{
					t = 0;
					commtime1ms = 0;
					pgbar.str++;
					f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
					if ((DREG[D_CMDSTATUS] == 100) && (f))
					{
						break;
					}
					else if ((DREG[D_CMDSTATUS] == 2) && (f))
					{
						break;
					}
					DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.str, pgbar.color, pgbar.bkcolor);
				}
				wdt();
			}
		}

		//		#if Debug
		//		printf("DREG[D_CMDSTATUS] = %d \r\n",DREG[D_CMDSTATUS]);
		//		#endif
		if (DREG[D_CMDSTATUS] == 100)
		{
			DrawProgressbar(pgbar.px, pgbar.py, pgbar.pdx, pgbar.pdy, pgbar.end, pgbar.color, pgbar.bkcolor);
			DelayMs(200);
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}
	return (FALSE);
}

//------------------------------------------
// Function   : U32 AnalyseUpdateList(U8 selextm)
// Description: 分析升级包中是否有升级数据
//------------------------------------------
U32 AnalyseUpdateList(U8 selextm, U8 netflag)
{
	U8 i, f = 0;
	U32 j, re, b;
	U32 len;
	U8 *Image_Buf;
	PROGRESSBAR pgbar;

	pgbar.mode = 0; // 不显示进度条
	if (selextm < 3)
	{ // selextm:0_USB 1_SD 2_TF(打板器存储设备)
		if (ExtMemory_Status(selextm) != 0)
			return (FALSE);
		Select_ExtMemory(selextm);
		f = Read_ProgramFile(100, 0, &len, pgbar);
	}
	else if (selextm == 3)
	{ //主板U盘
		f = ReadMboard_ProgramData(100, 0, &len, pgbar, netflag);
	}

	if (f == 0)
	{
		Image_Buf = (U8 *)(IAPBuff);

		g_ProInfo.Data[0] = IAPBuff[0];
		g_ProInfo.Data[1] = IAPBuff[1];
		g_ProInfo.Data[2] = IAPBuff[2];



		j = 512 + 24;
		re = 0;
		b = 0x1;
		for (i = 0; i < 20; i++)// 0-19 最大20个文件
		{ 
			len = (((U32)Image_Buf[j + 6]) << 24) + (((U32)Image_Buf[j + 7]) << 16) + (((U32)Image_Buf[j + 8]) << 8) + ((U32)Image_Buf[j + 9]);
			if ((Image_Buf[j] == systypel[i]) && (Image_Buf[j + 1] == systypeh[i]) && (len > 0))
			{
				re |= b;
			}
			j += 24;
			b <<= 1;
		}
		return (re);
	}
	else
	{
		return (0);
	}
}
