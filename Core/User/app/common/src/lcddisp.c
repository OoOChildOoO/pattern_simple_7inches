//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  lcddisp.c								*
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
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lcddisp.h"
#include "utility.h"
#include "S39vf160.h"
#include "main.h"
#include "register.h"
#include "touch.h"
#include "function.h"
#include "uart.h"
#include "color.h"
#include "w25qxx.h"
#include "vs1003.h"
#include "Patfun.h"
#include "function.h"
#include "viewPattern.h"

extern U8 PatternType[900]; //花样类型 1：三菱款  2：兄弟款   新增2016-11-29
extern volatile U32 g_bmplen;
extern const U32 D_Address[];
extern BOOTSCR g_bootscr;
extern U8 getWeekDay(U16 y, U8 m, U8 d);

volatile U32 flashver, flashdate; //flash版本与日期
volatile U32 mp3dver, mp3ddate;	  //MP3版本与日期
volatile U32 BufAdd;			  //缓存地址
volatile U8 Reflash = 0;		  //页面刷新标志
U16 *pMem;						  //volatile

U16 SheetPage; //多国语言Sheet表页码，从0开始
U16 *MEM_TITLE_ADD;
U16 *MEM_TITLE_AUDIO_ADD;
U32 TitleLengthMax;
U32 TitleAudioLengthMax;
U16 g_lang;

OLDPAT1 g_dispat1;		 //       获取的针点
OLDPAT1 g_dispat2;		 //模式一 当前针点
OLDPAT1 g_dispat3 = {0}; //模式一 定位针点

volatile U32 tmpflashadd[200];//flash缓存数据地址
volatile U8 flashdataflag[200];//flash数据使能标志

//存放点显示的最大x,y坐标
volatile U8 DrawLimtf = 0; //限制显示范围标志符
volatile U16 Drawmx0 = 0;
volatile U16 Drawmy0 = 0;
volatile U16 Drawmx1 = XLEN;
volatile U16 Drawmy1 = YLEN;

extern volatile U8 g_dataType;
extern volatile U8 g_hourType;
extern volatile U8 g_logdir;
extern volatile U8 g_vs1003err;
extern volatile U32 Timerms1; //计算播放语音间隔时间

extern volatile U8 g_BarcodeFlag[MAXFILENO - MINFILENO];
extern U8 Read_D(U32 Dn, U32 len, U16 *add);

extern const unsigned char msgTest[][65];

const U16 keypicdata[] = {
	Color_Silver,	  // RGB(192,192,192)	银白色	   Color_Gainsboro	 Color_LightGray	Color_Silver
	Color_BlueViolet, // RGB(138,43,226)	深紫罗兰的蓝色
	Color_KeyBlue,	  // RGB(0,0,255)		蓝色	 Color_Blue
	Color_Gray,		  // RGB(128,128,128)	灰色
	Color_Gainsboro,  //
	Color_Green,	  // RGB(0,255,0)		绿色
	Color_Red,		  // RGB(255,0,0)		红色
	RGB(250, 250, 250),
	Color_Silver, // RGB(192,192,192)	银白色	   Color_Gainsboro	 Color_LightGray	Color_Silver
};
//渐变色色图
const U16 keypicdatb[][3] = {
	// Color_White
	RGB(245, 245, 245), RGB(230, 230, 230), RGB(223, 223, 223), // Color_Silver	RGB(192,192,192)		银白色
	RGB(227, 205, 248), RGB(199, 155, 241), RGB(171, 105, 234), // Color_BlueViolet	RGB(138,43,226)		深紫罗兰的蓝色
	RGB(210, 210, 248), RGB(150, 150, 241), RGB(100, 100, 234), // Color_Blue	RGB(0,0,255)			蓝色
	RGB(235, 235, 235), RGB(200, 200, 200), RGB(160, 160, 160), // Color_Gray	RGB(128,128,128)		灰色
	RGB(240, 240, 240), RGB(225, 225, 225), RGB(210, 210, 210), // Color_Silver	RGB(192,192,192)		银白色
	RGB(200, 248, 200), RGB(150, 243, 150), RGB(100, 238, 100), // Color_Green 	RGB(0,255,0)			绿色
	RGB(248, 200, 200), RGB(243, 150, 150), RGB(238, 100, 100), // Color_Red	RGB(255,0,0)			红色
	RGB(240, 240, 240), RGB(150, 150, 150), RGB(120, 120, 120), // Color_Silver	RGB(192,192,192)		银白色
	RGB(245, 245, 245), RGB(230, 230, 230), RGB(223, 223, 223), // Color_Silver1	RGB(192,192,192)		银白色
};
//渐变色色图
const U16 keypicdatc[][3] = {
	// Color_Black
	RGB(80, 80, 80), RGB(120, 120, 120), RGB(150, 150, 150),	// Color_Silver		RGB(192,192,192)	银白色
	RGB(43, 13, 80), RGB(73, 23, 140), RGB(105, 33, 190),		// Color_BlueViolet	RGB(138,43,226)		深紫罗兰的蓝色
	RGB(30, 30, 120), RGB(55, 55, 170), RGB(75, 75, 220),		// Color_Blue	RGB(0,0,255)			蓝色
	RGB(30, 30, 30), RGB(60, 60, 60), RGB(90, 90, 90),			// Color_Gray	RGB(128,128,128)		灰色
	RGB(80, 80, 80), RGB(120, 120, 120), RGB(150, 150, 150),	// Color_Silver		RGB(192,192,192)	银白色
	RGB(22, 90, 22), RGB(40, 140, 40), RGB(50, 185, 50),		// Color_Green 	 RGB(0,255,0)			绿色
	RGB(90, 22, 22), RGB(140, 40, 40), RGB(185, 50, 50),		// Color_Red	RGB(255,0,0)			红色
	RGB(80, 80, 80), RGB(100, 100, 100), RGB(120, 120, 120),	// 			白色
	RGB(120, 120, 120), RGB(150, 150, 150), RGB(211, 211, 211), // Color_Silver		RGB(192,192,192)	银白色
};

const U16 keypicdatd[] = {
	Color_Silver,
	Color_KeyBlue,
	Color_KeyBlue,
	Color_Gray,
};
//extern const U8 ASCII_24x12A[];
//extern const U8 ASCII_24x24A[];
const U8 bmpdat[];
//const U8 bmpdat1[];
extern ProInfo g_ProInfo;
extern volatile U8 g_upinfo[0x20];//#define UP_ADDR 7，#define OS_ADDR 12，#define FL_ADDR 16，#define MP_ADDR 22
extern OLDPAT1 g_dispat1;
extern void ReadPointData(U32 x, U32 y);
extern U8 isUpInfoData(U8 *dat);
extern void creatUpInfoData(U8 *outDat);
U8 CountUnicodeInfo(U16 *addr, U16 Sheetcnt, U16 linenum, U16 langsnum, U32 *outoffest, U16 *outstrlen);
//------------------------------------------
// Function   : void initflashadd(void)
// Description: 初始化数据在flash中地址
//------------------------------------------
void initflashadd(void)
{
	U32 j, l;
	U16 checksum /*,tmpchecksum*/;
	U16 datasum, tmpdatasum;
	U32 listlen;
	U16 *add;
	U16 *dadd;
	U32 i, dat, cnt;
	U32 *padd;
	U32 maxadd;
	U16 buff[12];
	U8 err = 0;
	//	U16 flasherr = 0;

	const U8 Check_FLASH[] = {"GZSK_FLASH_V"};

	U8 Info[0x20] = {0};
	U8 check1 = 0, check2 = 0;
	U8 tcheck1 = 0, tcheck2 = 0;
	U8 /*upflag =0,*/ ostype = 0;
	//U8 *p8;
	//初始化g_upinfo
	W25QXX_Read(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
	check1 = Info[0x1e];
	check2 = Info[0x1f];
	tcheck1 = Info[0];
	tcheck2 = Info[0];
	for (i = 1; i < 0x1e; i++)
	{
		tcheck1 += Info[i];
		tcheck2 ^= Info[i];
	}
	if (check1 == tcheck1 && check2 == tcheck2)
	{
		isUpInfoData(Info);
		for (i = 0; i < 0x1e; i++)
		{
			g_upinfo[i] = Info[i];
		}
	}

	Timer_ms = 0;
	W25QXX_Read(W25Q2_MP3, (U8 *)buff, 0 + MP3_SADDR, 24);
	if (strncmp((char *)buff, "GZSK_MP3_V", 10) == 0)
	{
		mp3dver = ((U16)asctonum(buff[5])) * 100 + ((U16)asctonum(buff[5] >> 8)) * 10 + asctonum(buff[6]);
		dat = ((U16)asctonum(buff[7])) * 1000 + ((U16)asctonum(buff[7] >> 8)) * 100 + ((U16)asctonum(buff[8])) * 10 + asctonum(buff[8] >> 8);
		mp3ddate = dat * 10000 + ((U16)asctonum(buff[9])) * 1000 + ((U16)asctonum(buff[9] >> 8)) * 100 + ((U16)asctonum(buff[10])) * 10 + asctonum(buff[10] >> 8);
	}
	else
	{
		mp3dver = 0;
		mp3ddate = 0;
	}
	//	p8 = (U8 *)buff;
	//	for(i=0;i<24;i++){
	//	printf("%02x ",p8[i]);
	//	}
	//#if DIS_LOGO ==0
	//	if(g_vs1003err==0){
	//		PlaySound(CMD_PLAY,2,1);		// 开机音乐
	//	}
	//#endif
	//
	//#if DIS_LOGO ==1
	//	//printf("--g_vs1003err = %d \r\n",g_vs1003err);
	//	if(g_vs1003err==0){
	//		PlaySound(CMD_PLAY,2,1);		// 开机音乐
	//	}
	//#endif

	if (g_bootscr.mode != 3)
	{
		if (g_vs1003err == 0)
		{
			PlaySound(CMD_PLAY, 2, 1); // 开机音乐
		}
	}

	W25QXX_Read(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
	check1 = Info[0x1e];
	check2 = Info[0x1f];
	tcheck1 = Info[0];
	tcheck2 = Info[0];
	for (i = 1; i < 0x1e; i++)
	{
		tcheck1 += Info[i];
		tcheck2 ^= Info[i];
	}
	//upflag =0;
	if (check1 == tcheck1 && check2 == tcheck2)
	{
		if (isUpInfoData(Info))
		{
			ostype = (Info[OS_ADDR] == '1' ? 1 : 0);
			//	flasherr = (Info[FL_ADDR]=='0'?0:1);
		}
	}

	W25QXX_Read(W25Q1_DAT, (U8 *)&g_ProInfo, (ostype == 0 ? W25QMIFOADDR : W25QHIFOADDR) + 18, 6);

	W25QXX_Read(W25Q1_DAT, (U8 *)buff, FLASHINDEXADDR, 24);//读取flash索引的版本

	if (strncmp((char *)buff, (char *)Check_FLASH, 12) == 0)//判断版本/型号/数据是否合规是否一致
	{
		while (err < 5)
		{
			flashver = ((U16)asctonum(buff[6])) * 100 + ((U16)asctonum(buff[6] >> 8)) * 10 + asctonum(buff[7]);
			dat = ((U16)asctonum(buff[8])) * 1000 + ((U16)asctonum(buff[8] >> 8)) * 100 + ((U16)asctonum(buff[9])) * 10 + asctonum(buff[9] >> 8);
			flashdate = dat * 10000 + ((U16)asctonum(buff[10])) * 1000 + ((U16)asctonum(buff[10] >> 8)) * 100 + ((U16)asctonum(buff[11])) * 10 + asctonum(buff[11] >> 8);

			W25QXX_Read(W25Q1_DAT, (U8 *)buff, FLASHINDEXADDR + 24, 12);
			maxadd = (((U32)buff[1]) << 16) + buff[0];
			if ((maxadd > FLASHINDEXADDR) && (maxadd < 0x800000))
			{
				W25QXX_Read(W25Q1_DAT, (U8 *)SDRAMFLASHADD, 0, maxadd);
				add = (U16 *)(SDRAMFLASHADD + FLASHINDEXADDR + 28);
				listlen = ((*(add + 1)) << 16) + (*add);
				//tmpchecksum = (*(add+2));
				checksum = 0;
				add = (U16 *)(SDRAMFLASHADD + FLASHINDEXADDR + 36);
				padd = (U32 *)(SDRAMFLASHADD + FLASHINDEXADDR + 36);
				l = 0;
				for (i = 1; i <= BINFILE_ALL; i++)
				{
					for (j = 0; j < 5; j++)
					{
						checksum += (*add);
						add++;
					}

					tmpflashadd[i] = (*padd);
					if (tmpflashadd[i] >= maxadd)
					{
						{
							if (i == 0)
								tmpflashadd[i] = 0;
							else
								tmpflashadd[i] = tmpflashadd[i - 1] - SDRAMFLASHADD + 2 * l;
							l = 0;
						};
						flashdataflag[i] = FALSE;
					}
					else
					{
						flashdataflag[i] = TRUE;
					}

					tmpflashadd[i] += SDRAMFLASHADD;
					if (flashdataflag[i] == TRUE)
					{
						dadd = (U16 *)(tmpflashadd[i]);
						l = (*(padd + 1));
						tmpdatasum = (*(padd + 2));
						datasum = 0;
						for (j = 0; j < l; j++)
						{
							datasum += *dadd;
							dadd++;
						}
						if (i == 148)
							g_bmplen = l * 2;
						if (datasum != tmpdatasum)
						{
							flashdataflag[i] = FALSE;
						}
					}
					padd += 3;
					add++;
				}

				listlen -= FLASHINDEXADDR;
				listlen /= 12;
				listlen -= 3;
				//				if((checksum!=tmpchecksum)||(listlen!=BINFILE_ALL)){
				//					for(i=1;i<=BINFILE_ALL;i++){
				//						flashdataflag[i] = FALSE;
				//					}
				//				}

				for (i = 1; i <= BINFILE_ALL; i++) //检测是否校验都通过，不通过重读5次校验
				{
					if (flashdataflag[i] == FALSE)
					{
						break;
					}
				}
				if (i > BINFILE_ALL)
				{
					g_upinfo[FL_ADDR] = '0'; //不需要升级
					creatUpInfoData(Info);
					W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
					break;
				}
				else
				{
					flashver = 0;
					err++;
				}
			}
			else
			{
				flashver = 0;
				flashdate = 0;
				err++;
			}
		}
	}
	else
	{
		flashver = 0;
		flashdate = 0;
	}

	if ((flashver == 0) && (flashdate == 0))// FLASH中数据校验借误,将各类数据初始地址初始化并将数据区清零,防止读错地址死机
	{ 
		for (i = 1; i <= BINFILE_ALL; i++)
		{
			flashdataflag[i] = FALSE;
			tmpflashadd[i] = SDRAMFLASHADD;
		}
		dadd = (U16 *)(SDRAMFLASHADD);
		for (i = SDRAMFLASHADD; i < (SDRAMFLASHADD + 0x7fffff); i += 2)
		{
			(*dadd) = 0;
			dadd++;
		}
	}
	else
	{
		for (i = 1; i <= BINFILE_ALL; i++)
		{
			if ((flashdataflag[i] == FALSE) && (i < BINFILE_ALL))//无用的flash区域指向首地址
			{
				dadd = (U16 *)tmpflashadd[i];
				for (cnt = tmpflashadd[i]; cnt < tmpflashadd[i + 1]; cnt += 2)
				{
					(*dadd) = 0;
					dadd++;
				}
			}
			else if ((flashdataflag[i] == FALSE) && (i == BINFILE_ALL))//最后一位
			{
				dadd = (U16 *)tmpflashadd[i];
				for (cnt = tmpflashadd[i]; cnt < (SDRAMFLASHADD + 0x7fffff); cnt += 2)
				{
					(*dadd) = 0;
					dadd++;
				}
			}
		}
	}

	//mp3dver = 0;
	//flashver = 0;
}

//------------------------------------------
// Function   : U16 RGB(U8 r, U8 g, U8 b)
// Description: R8+G8+B8->R5+G6+B5
//------------------------------------------
//U16 RGB(U8 r, U8 g, U8 b)
//{
//	U16 ColorTemp=0;
//
//	ColorTemp = ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
//
//	return(ColorTemp);
//}

//------------------------------------------
// Function   : U16 STNToTFT(U8 color)
// Description: 8bpp to 16bpp
//------------------------------------------
U16 STNToTFT(U8 color)
{
	return ((color & 0xE0) << 6 | (color & 0x1C) << 3 | (color & 0x03) << 3);
}

//------------------------------------------
// Function   : void WritePage(U32 page_no)
// Description: 指定数据写页面 显存一个区大小为0x400000 / 0x96000 = 6页
//------------------------------------------
void WritePage(U32 page_no)
{
	if (page_no > DISP_PAGE_MAXNO)
		page_no = 0;

	BufAdd = LCD_VRAM_BASE_ADDR + page_no * DISP_PAGE_SIZE;
	Reflash = 0;
}

//------------------------------------------
// Function   : void ViewPage(U32 page_no)
// Description: 指定显示页面 SDRAM = 16MW = 0x1000000 分四个区
//------------------------------------------
void ViewPage(U32 page_no)
{
	if (page_no > DISP_PAGE_MAXNO)
		page_no = 0;

	LTDC_Layer1->CFBAR = LCD_VRAM_BASE_ADDR + page_no * DISP_PAGE_SIZE;
	LTDC->SRCR |= 1;

	Reflash = 1;
}

//------------------------------------------
// Function   : void SDRAM_memcpy( U16 *dest, U16 *src, U32 count )
// Description: 复制页面
//------------------------------------------
void SDRAM_memcpy(U32 dest, U32 src, U32 count)
{
	U32 i = count;

	wdt();
	for (; i > 0; i -= 2)
	{
		*((U16 *)(dest + i)) = *((U16 *)(src + i));
	}
	wdt();
}

//------------------------------------------
// Function   : void PageWritern( U8 des_no,U8 org_no)
// Description: 粘贴页面(U16 *)	(U16 *)
//------------------------------------------
void PageWritern(U8 des_no, U8 org_no)
{
	SDRAM_memcpy((LCD_VRAM_BASE_ADDR + des_no * DISP_PAGE_SIZE), (LCD_VRAM_BASE_ADDR + org_no * DISP_PAGE_SIZE), DISP_PAGE_SIZE);
}

//------------------------------------------
// Function   : void CLR_Screen(U32 color)
// Description: 清屏
//------------------------------------------
void CLR_Screen(U32 color)
{
	U32 padd, pmax;

	padd = BufAdd;
	pmax = padd + DISP_PAGE_SIZE;
	wdt();
	for (; padd < pmax; padd += 2)
	{
		*((U16 *)padd) = color;
	}
	wdt();
}

//------------------------------------------
// Function   : void InitScreen(void)
// Description: 清屏
//------------------------------------------
void InitScreen(void)
{
	U32 i, j;
	U32 srcadd = LCD_VRAM_BASE_ADDR + VMAXPAG * DISP_PAGE_SIZE;

	wdt();
	if (usepageno == 1)
		usepageno = 2;
	else
		usepageno = 1;
	WritePage(usepageno);
	EEPREG[0] = usepageno;
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,512,32,Color_Blue);
	//	{
	//		logorun = 0;
	//		DrawFillRect(0,0,36,32,Color_White);
	//		DrawFBMP2(9,0,14,23,MEM_BMPLOGO_ML1,Color_Red);
	//		DrawFBMP2(0,23,22,9,MEM_BMPLOGO_ML2,Color_Red);
	//		DrawFBMP2(18,15,18,17,MEM_BMPLOGO_ML3,Color_Red);
	//		logorun = 1;
	//	}				   //(U16 *)((*BMPLOGO_MLKG)+SST39VF640ADD)
	//	DrawFBMP2(36,0,92,32,MEM_BMPLOGO_MLKG,Color_Gainsboro);
	//	if(P_KNO==4){DrawFillRect(0,32,640,448,Color_LightSkyBlue);}	 // Color_SkyBlue
	//	else if(P_KNO==3){DrawFillRect(0,32,640,448,Color_Gainsboro);}
	//	else if(P_KNO==2){DrawBMP(0,32,640,448,MEM_STARTWLBMP);}
	//	else{DrawBMP(0,32,640,448,MEM_DESKTOPBMP);}
	g_logorun = 0;
	g_logdir = 0;
	for (i = BufAdd, j = BufAdd + DISP_PAGE_SIZE; i < j; i += 2)
	{
		*((U16 *)i) = *((U16 *)srcadd);
		srcadd += 2;
	}
	g_logorun = 1;
	wdt();
	keyolddata = 0xff;
}

//------------------------------------------
// Function   : void DrawIcon(U16 x, U16 y, U32 Color)
// Description: 校正时显示当前点
//------------------------------------------
void DrawIcon(U16 x, U16 y, U32 Color)
{
	U32 i, j;

	if (x > 3)
		i = x - 4;
	else
		i = 0;
	if (x < (LCD_width - 4))
		j = x + 5;
	else
		j = (LCD_width - 1);
	for (; i < j; i++)
		PutPixel(i, y, Color);
	if (y > 3)
		i = y - 4;
	else
		i = 0;
	if (y < (LCD_height - 6))
		j = y + 5;
	else
		j = (LCD_height - 1);
	for (; i < j; i++)
		PutPixel(x, i, Color);
}

const U8 Arrow[6] = {0xf8, 0xe0, 0xf0, 0xb8, 0x9c, 0x0c}; // 6x6箭头图片
//------------------------------------------
// Function   : void DrawIconon(U16 x, U16 y, U32 Color)
// Description: 校正时显示当前点
//------------------------------------------
void DrawIconon(U16 x, U16 y, U32 Color)
{
	U8 ff, l, b;
	U32 i, j, n, m;

	if ((x < LCD_width) && (y < LCD_height))
	{
		n = y + 6;
		if (n > (LCD_height - 1))
			n = (LCD_height - 1);
		m = x + 6;
		if (m > (LCD_width - 1))
			m = (LCD_width - 1);
		l = 0;
		for (i = y; i < n; i++)
		{
			ff = 0x80;
			b = Arrow[l++];
			for (j = x; j < m; j++)
			{
				if (b & ff)
					PutPixel(j, i, Color);
				ff >>= 1;
			}
		}
	}
}

//------------------------------------------
// Function   : void DrawIconoff(U16 x, U16 y)
// Description: 清除校正时显示当前点
//------------------------------------------
void DrawIconoff(U16 x, U16 y)
{
	U32 n, m;
	U32 p, i, j;
	U32 srcadd = LCD_VRAM_BASE_ADDR + VMAXPAG * DISP_PAGE_SIZE;

	//	if(x>3)i = x - 4;else i = 0;if(x<796)j = x + 5;else j = 799;
	//	for(;i<j;i++){p = srcadd + y * YOFFSET + i + i;PutPixel(i,y,(*((U16 *)p)));}
	//	if(y>3)i = y - 4;else i = 0;if(y<474)j = y + 5;else j = 479;
	//	for(;i<j;i++){p = srcadd + i * YOFFSET + x + x;PutPixel(x,i,(*((U16 *)p)));}

#if VIEWTYPE == HORIZONTAL //横屏
	if ((x < LCD_width) && (y < LCD_height))
	{
		n = y + 6;
		if (n > (LCD_height - 1))
			n = (LCD_height - 1);
		m = x + 6;
		if (m > (LCD_width - 1))
			m = (LCD_width - 1);
		for (i = y; i < n; i++)
		{
			for (j = x; j < m; j++)
			{
				p = srcadd + i * YOFFSET + j + j;
				PutPixel(j, i, (*((U16 *)p)));
			}
		}
	}
#else //竖屏
	m = x + 6;
	if (m > (LCD_width - 1))
		m = (LCD_width - 1);
	m = m * XOFFSET;
	for (j = x * XOFFSET; j < m; j += XOFFSET)
	{
		n = y + 6;
		if (n > (LCD_height - 1))
			n = (LCD_height - 1);
		i = BufAdd + j;
		n = i + n + n;
		p = srcadd + j + y + y;
		for (i = i + y + y; i < n; i += 2)
		{
			*((U16 *)i) = (*((U16 *)p));
			p += 2;
		}
	}
#endif
}

//------------------------------------------
// Function   : void DrawProgressbar(U32 x,U32 y,U32 dx,U32 dy,U8 percent,U16 color,U16 bkcolor)
// Description: 描绘进度条
// note : percent当前进度
//------------------------------------------
void DrawProgressbar(U32 x, U32 y, U32 dx, U32 dy, U8 percent, U16 color, U16 bkcolor)
{
	U32 px, py, pdx, pdy;
	U32 xlen;

	if (percent > 100)
		percent = 100;
	//printf("percent = %d \r\n",percent);
	DrawRect(x, y, dx, dy, color);
	if (dx > 8)
	{
		px = x + 4;
		pdx = dx - 8;
	}
	else if (dx)
	{
		px = x + (dx >> 1);
		pdx = 1;
	}
	else
	{
		px = x;
		pdx = 0;
	}
	if (dy > 8)
	{
		py = y + 4;
		pdy = dy - 8;
	}
	else if (dy)
	{
		py = y + (dy >> 1);
		pdy = 1;
	}
	else
	{
		py = y;
		pdy = 0;
	}
	xlen = pdx * percent / 100;
	DrawFillRect(px, py, xlen, pdy, color);
	if (bkcolor == 0)
		BmpFillRect(px + xlen, py, pdx - xlen, pdy);
	else
		DrawFillRect(px + xlen, py, pdx - xlen, pdy, bkcolor);
}

//------------------------------------------
// Function   : void BmpFillRect(U32 x,U32 y,U32 dx,U32 dy)
// Description: 块清除(用背景图片)
// note：当底图存在图案时，用这种方法擦除指定区域显示，不会擦除背景
//------------------------------------------
void BmpFillRect(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 x2, y2;
	U32 p, i, j;
	U32 srcadd = LCD_VRAM_BASE_ADDR + VMAXPAG * DISP_PAGE_SIZE;
#if VIEWTYPE == HORIZONTAL //横屏
	if ((x < XLEN) && (y < YLEN))
	{
		//		wdt();
		y2 = y + dy;
		y2 = y2 * YOFFSET;
		for (j = y * YOFFSET; j < y2; j += YOFFSET)
		{
			x2 = x + dx;
			i = BufAdd + j;
			x2 = i + x2 + x2;
			p = srcadd + j + x + x;
			for (i = i + x + x; i < x2; i += 2)
			{
				*((U16 *)i) = (*((U16 *)p));
				p += 2;
			}
		}
	}
#else
	if ((x < XLEN) && (y < YLEN))
	{
		x2 = x + dx;
		if (x2 > XLEN)
			x2 = XLEN;
		x2 = x2 * XOFFSET;
		for (j = x * XOFFSET; j < x2; j += XOFFSET)
		{
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			i = BufAdd + j;
			y2 = i + y2 + y2;
			p = srcadd + j + y + y;
			for (i = i + y + y; i < y2; i += 2)
			{
				*((U16 *)i) = (*((U16 *)p));
				p += 2;
			}
		}
	}
#endif
}

//------------------------------------------
// Function   : void KeyFillBmp(KEYPAD bn)
// Description: 按键清除
// note：删除按键显示
//------------------------------------------
void KeyFillBmp(KEYPAD bn)
{
	BmpFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize);
}

//------------------------------------------
// Function   : void PutPixel(U32 x, U32 y,U32 color)
// Description: 输出点
// note：不含大小限制，可以输出至屏幕外
//------------------------------------------
void PutPixel(U32 x, U32 y, U32 color)
{
#if VIEWTYPE == HORIZONTAL //横屏
	*((U16 *)(BufAdd + y * YOFFSET + x + x)) = color;
#else
	*((U16 *)(BufAdd + x * XOFFSET + y + y)) = color;
#endif
}
//------------------------------------------
// Function   : void PutPixel(U32 x, U32 y,U32 color)
// Description: 输出点
// note：含大小限制
//------------------------------------------
void PutPixel1(S32 x, S32 y, U32 color)
{
#if VIEWTYPE == HORIZONTAL //横屏
	if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0) && (y >= Drawmy0))
	{
		*((U16 *)(BufAdd + y * YOFFSET + x + x)) = color;
	}
#else
	if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0) && (y >= Drawmy0))
	{
		*((U16 *)(BufAdd + x * XOFFSET + y + y)) = color;
	}
#endif
}

////------------------------------------------
//// Function   : U32 ReadPixel(U32 x, U32 y)
//// Description: 读点数据
////------------------------------------------
//U32 ReadPixel(U32 x, U32 y)
//{
//	U32 color;
//#if VIEWTYPE==HORIZONTAL
//	color = *((U16 *)(BufAdd+y*YOFFSET+x+x));
//#else
//	color = *((U16 *)(BufAdd+x*XOFFSET+y+y));
//#endif
//	return (color);
//}

//------------------------------------------
// Function   : U32 ReadPixel(U32 x, U32 y)
// Description: 读点数据
//------------------------------------------
U16 ReadPixel(U32 x, U32 y)
{
	U16 color;
#if VIEWTYPE == HORIZONTAL
	color = *((U16 *)(BufAdd + y * YOFFSET + x + x));
#else
	color = *((U16 *)(BufAdd + x * XOFFSET + y + y));
#endif
	return (color);
}
//------------------------------------------
// Function   : void PutPixel22(U32 x, U32 y,U16 color)
// Description: 描绘一个2*2的点，点的锚点为(0,0)
//------------------------------------------
void PutPixel22(U32 x, U32 y, U16 color)
{
	PutPixel(x, y, color);
	PutPixel(x + 1, y, color);
	PutPixel(x, y + 1, color);
	PutPixel(x + 1, y + 1, color);
}

//------------------------------------------
// Function   : void DrawHLine(U32 x, U32 y, U32 dx, U32 color)
// Description: 水平实线
//------------------------------------------
void DrawHLine(S32 x, S32 y, U32 dx, U32 color)
{
	S32 i, x2;
#if VIEWTYPE == HORIZONTAL //横屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (y >= Drawmy0))
		{
			wdt();
			x2 = x + dx;
			if (x2 > Drawmx1)
				x2 = Drawmx1;
			if (x < Drawmx0)
				x = Drawmx0;
			i = BufAdd + y * YOFFSET;
			x2 = i + x2 + x2;
			for (i = i + x + x; i < x2; i += 2)
			{
				*((U16 *)i) = color;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			x2 = x + dx;
			if (x2 > XLEN)
				x2 = XLEN;
			i = BufAdd + y * YOFFSET;
			x2 = i + x2 + x2;
			for (i = i + x + x; i < x2; i += 2)
			{
				*((U16 *)i) = color;
			}
		}
	}
#else
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (y >= Drawmy0))
		{
			wdt();
			x2 = x + dx;
			if (x2 > Drawmx1)
				x2 = Drawmx1;
			if (x < Drawmx0)
				x = Drawmx0;
			i = BufAdd + x * XOFFSET + y + y;
			x2 = BufAdd + x2 * XOFFSET + y + y;
			for (; i < x2; i += XOFFSET)
			{
				*((U16 *)i) = color;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			x2 = x + dx;
			if (x2 > XLEN)
				x2 = XLEN;
			i = BufAdd + x * XOFFSET + y + y;
			x2 = BufAdd + x2 * XOFFSET + y + y;
			for (; i < x2; i += XOFFSET)
			{
				*((U16 *)i) = color;
			}
		}
	}
#endif
}

//------------------------------------------
// Function   : void DrawVLine(U32 x,U32 y,U32 dy,U32 color)
// Description: 垂直实线
//------------------------------------------
void DrawVLine(S32 x, S32 y, U32 dy, U32 color)
{
	S32 i, y2;
#if VIEWTYPE == HORIZONTAL //横屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0))
		{
			wdt();
			y2 = y + dy;
			if (y2 > Drawmy1)
				y2 = Drawmy1;
			if (y < Drawmy0)
				y = Drawmy0;
			y2 = BufAdd + x + x + y2 * YOFFSET;
			i = BufAdd + x + x + y * YOFFSET;
			for (; i < y2; i += YOFFSET)
			{
				*((U16 *)i) = color;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			y2 = BufAdd + x + x + y2 * YOFFSET;
			i = BufAdd + x + x + y * YOFFSET;
			for (; i < y2; i += YOFFSET)
			{
				*((U16 *)i) = color;
			}
		}
	}
#else //竖屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0))
		{
			wdt();
			y2 = y + dy;
			if (y2 > Drawmy1)
				y2 = Drawmy1;
			if (y < Drawmy0)
				y = Drawmy0;
			y2 = BufAdd + x * XOFFSET + y2 + y2;
			i = BufAdd + x * XOFFSET + y + y;
			for (; i < y2; i += 2)
			{
				*((U16 *)i) = color;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			i = BufAdd + x * XOFFSET + y + y;
			y2 = BufAdd + x * XOFFSET + y2 + y2;
			for (; i < y2; i += 2)
			{
				*((U16 *)i) = color;
			}
		}
	}
#endif
}

//------------------------------------------
// Function   : void DrawHLine1(S32 x, S32 y, U32 dx, U16 color,U16 color2)
// Description: 水平实线
// note: 略过指定的颜色绘制该直线
//------------------------------------------
void DrawHLine1(S32 x, S32 y, S32 dx, U16 color, U16 color2)
{
	U16 i;
	U16 tcolor;
	for (i = 0; i < dx; i++)
	{
		if ((DrawLimtf) && (x + i < Drawmx0 || x + i > Drawmx1 || y < Drawmy0 || y > Drawmy1))
			continue;
		tcolor = ReadPixel(x + i, y);
		if (tcolor != color2)
		{
			PutPixel(x + i, y, color);
		}
	}
	wdt();
}

//------------------------------------------
// Function   : void DrawHLine1(S32 x, S32 y, U32 dx, U16 color,U16 color2)
// Description: 垂直实线
// note: 略过指定的颜色绘制该直线
//------------------------------------------
void DrawVLine1(S32 x, S32 y, S32 dy, U16 color, U16 color2)
{
	U16 i;
	U16 tcolor;
	for (i = 0; i < dy; i++)
	{
		if ((DrawLimtf) && (x < Drawmx0 || x > Drawmx1 || y + i < Drawmy0 || y + i > Drawmy1))
			continue;
		tcolor = ReadPixel(x, y + i);
		if (tcolor != color2)
		{
			PutPixel(x, y + i, color);
		}
	}
	wdt();
}

#define DISP_PARA_X 1 //x缩放因子
#define DISP_PARA_Y 1 //y缩放因子
//------------------------------------------
// Function   : void DrawLine(U32 x1,U32 y1,U32 x2,U32 y2,U32 color)
// Description: 描线 A点坐标--B点坐标
//------------------------------------------
void DrawLine(S32 x1, S32 y1, S32 x2, S32 y2, U32 color)
{
	S32 dx, dy, e;

	dx = x2 - x1;
	dy = y2 - y1;
	if (dy == 0)
	{
		if (x2 > x1)
			DrawHLine(x1, y1, x2 - x1, color);
		else
			DrawHLine(x2, y1, x1 - x2, color);
	}
	else if (dx == 0)
	{
		if (y2 > y1)
			DrawVLine(x1, y1, y2 - y1, color);
		else
			DrawVLine(x1, y2, y1 - y2, color);
	}
	else
	{

		//x1 *= DISP_PARA_X;y1 *= DISP_PARA_Y;
		//x2 *= DISP_PARA_X;y2 *= DISP_PARA_Y;
		//dx *= DISP_PARA_X;dy *= DISP_PARA_X;///////////???????????
		if (dx >= 0)
		{
			if (dy >= 0)
			{
				if (dx >= dy)
				{ // 1/8 octant
					e = dy - dx / 2;
					while (x1 <= x2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){y1+=1;e-=dx;}
						//x1+=1;
						if (e > 0)
						{
							y1 += DISP_PARA_Y;
							e -= dx;
						}
						x1 += DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 2/8 octant
					e = dx - dy / 2;
					while (y1 <= y2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){x1+=1;e-=dy;}
						//y1+=1;
						if (e > 0)
						{
							x1 += DISP_PARA_X;
							e -= dy;
						}
						y1 += DISP_PARA_Y;
						e += dx;
					}
				}
			}
			else
			{			  // dy<0
				dy = -dy; // dy=abs(dy)
				if (dx >= dy)
				{ // 8/8 octant
					e = dy - dx / 2;
					//e=dy-dx/4;
					while (x1 <= x2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){y1-=1;e-=dx;}
						//x1+=1;
						if (e > 0)
						{
							y1 -= DISP_PARA_Y;
							e -= dx;
						}
						x1 += DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 7/8 octant
					e = dx - dy / 2;
					while (y1 >= y2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){x1+=1;e-=dy;}
						//y1-=1;
						if (e > 0)
						{
							x1 += DISP_PARA_X;
							e -= dy;
						}
						y1 -= DISP_PARA_Y;
						e += dx;
					}
				}
			}
		}
		else
		{			  //dx<0
			dx = -dx; //dx=abs(dx)
			if (dy >= 0)
			{ // dy>=0
				if (dx >= dy)
				{ // 4/8 octant
					e = dy - dx / 2;
					while (x1 >= x2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){y1+=1;e-=dx;}
						//x1-=1;
						if (e > 0)
						{
							y1 += DISP_PARA_Y;
							e -= dx;
						}
						x1 -= DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 3/8 octant
					e = dx - dy / 2;
					while (y1 <= y2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){x1-=1;e-=dy;}
						//y1+=1;
						if (e > 0)
						{
							x1 -= DISP_PARA_X;
							e -= dy;
						}
						y1 += DISP_PARA_Y;
						e += dx;
					}
				}
			}
			else
			{			  // dy<0
				dy = -dy; // dy=abs(dy)
				if (dx >= dy)
				{ // 5/8 octant
					e = dy - dx / 2;
					while (x1 >= x2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){y1-=1;e-=dx;}
						//x1-=1;
						if (e > 0)
						{
							y1 -= DISP_PARA_Y;
							e -= dx;
						}
						x1 -= DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 6/8 octant
					e = dx - dy / 2;
					while (y1 >= y2)
					{
						PutPixel1(x1, y1, color);
						//if(e>0){x1-=1;e-=dy;}
						//y1-=1;
						if (e > 0)
						{
							x1 -= DISP_PARA_X;
							e -= dy;
						}
						y1 -= DISP_PARA_Y;
						e += dx;
					}
				}
			}
		}
	}
}

//------------------------------------------
// Function   : void DrawLine(U32 x1,U32 y1,U32 x2,U32 y2,U32 color)
// Description: 描线
// note: 略过指定的颜色绘制该直线
//------------------------------------------
void DrawLine1(S32 x1, S32 y1, S32 x2, S32 y2, U16 color, U16 color2)
{
	S32 dx, dy, e;
	U16 tcolor;
	dx = x2 - x1;
	dy = y2 - y1;
	if (dy == 0)
	{
		if (x2 > x1)
			DrawHLine1(x1, y1, x2 - x1, color, color2);
		else
			DrawHLine1(x2, y1, x1 - x2, color, color2);
	}
	else if (dx == 0)
	{
		if (y2 > y1)
			DrawVLine1(x1, y1, y2 - y1, color, color2);
		else
			DrawVLine1(x1, y2, y1 - y2, color, color2);
	}
	else
	{

		//x1 *= DISP_PARA_X;y1 *= DISP_PARA_Y;
		//x2 *= DISP_PARA_X;y2 *= DISP_PARA_Y;
		//dx *= DISP_PARA_X;dy *= DISP_PARA_X;///////////???????????
		if (dx >= 0)
		{
			if (dy >= 0)
			{
				if (dx >= dy)
				{ // 1/8 octant
					e = dy - dx / 2;
					while (x1 <= x2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}

						//if(e>0){y1+=1;e-=dx;}
						//x1+=1;
						if (e > 0)
						{
							y1 += DISP_PARA_Y;
							e -= dx;
						}
						x1 += DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 2/8 octant
					e = dx - dy / 2;
					while (y1 <= y2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){x1+=1;e-=dy;}
						//y1+=1;
						if (e > 0)
						{
							x1 += DISP_PARA_X;
							e -= dy;
						}
						y1 += DISP_PARA_Y;
						e += dx;
					}
				}
			}
			else
			{			  // dy<0
				dy = -dy; // dy=abs(dy)
				if (dx >= dy)
				{ // 8/8 octant
					e = dy - dx / 2;
					//e=dy-dx/4;
					while (x1 <= x2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){y1-=1;e-=dx;}
						//x1+=1;
						if (e > 0)
						{
							y1 -= DISP_PARA_Y;
							e -= dx;
						}
						x1 += DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 7/8 octant
					e = dx - dy / 2;
					while (y1 >= y2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){x1+=1;e-=dy;}
						//y1-=1;
						if (e > 0)
						{
							x1 += DISP_PARA_X;
							e -= dy;
						}
						y1 -= DISP_PARA_Y;
						e += dx;
					}
				}
			}
		}
		else
		{			  //dx<0
			dx = -dx; //dx=abs(dx)
			if (dy >= 0)
			{ // dy>=0
				if (dx >= dy)
				{ // 4/8 octant
					e = dy - dx / 2;
					while (x1 >= x2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){y1+=1;e-=dx;}
						//x1-=1;
						if (e > 0)
						{
							y1 += DISP_PARA_Y;
							e -= dx;
						}
						x1 -= DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 3/8 octant
					e = dx - dy / 2;
					while (y1 <= y2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){x1-=1;e-=dy;}
						//y1+=1;
						if (e > 0)
						{
							x1 -= DISP_PARA_X;
							e -= dy;
						}
						y1 += DISP_PARA_Y;
						e += dx;
					}
				}
			}
			else
			{			  // dy<0
				dy = -dy; // dy=abs(dy)
				if (dx >= dy)
				{ // 5/8 octant
					e = dy - dx / 2;
					while (x1 >= x2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){y1-=1;e-=dx;}
						//x1-=1;
						if (e > 0)
						{
							y1 -= DISP_PARA_Y;
							e -= dx;
						}
						x1 -= DISP_PARA_X;
						e += dy;
					}
				}
				else
				{ // 6/8 octant
					e = dx - dy / 2;
					while (y1 >= y2)
					{
						if ((DrawLimtf) && (x1 < Drawmx0 || x1 > Drawmx1 || y1 < Drawmy0 || y1 > Drawmy1))
						{
						}
						else
						{
							tcolor = ReadPixel(x1, y1);
							if (tcolor != color2)
								PutPixel1(x1, y1, color);
						}
						//if(e>0){x1-=1;e-=dy;}
						//y1-=1;
						if (e > 0)
						{
							x1 -= DISP_PARA_X;
							e -= dy;
						}
						y1 -= DISP_PARA_Y;
						e += dx;
					}
				}
			}
		}
	}
}

//------------------------------------------
// Function   : DrawPoint(S32 x, S32 y, U16 color)
// Description: 绘制3*3的点
//------------------------------------------
void DrawPoint(S32 x, S32 y, U16 color)
{

	DrawHLine(x - 1, y, 3, color);
	DrawVLine(x, y - 1, 3, color);
}
//------------------------------------------
// Function   : DrawCross7x7(S32 x, S32 y, U16 color)
// Description: 绘制7*7的十字光标
//------------------------------------------
void DrawCross7x7(S32 x, S32 y, U16 color)
{

	DrawHLine(x - 4, y, 9, color);
	DrawVLine(x, y - 4, 9, color);
}

//------------------------------------------
// Function   : void DrawRect(U32 x,U32 y,U32 dx,U32 dy,U32 color)
// Description: 描绘长方形
//------------------------------------------
void DrawRect(U32 x, U32 y, U32 dx, U32 dy, U32 color)
{
	DrawHLine(x, y, dx, color);
	DrawVLine(x + dx - 1, y, dy, color);
	DrawHLine(x, y + dy - 1, dx, color);
	DrawVLine(x, y, dy, color);
}

//------------------------------------------
// Function   : void DrawHLine(U32 x, U32 y, U32 dx, U32 color)
// Description: 水平虚线
// note: 绘制 实虚比为  4:2 虚线
//------------------------------------------
void DrawHDottedLine(S32 x, S32 y, U32 dx, U32 color)
{
	S32 i, x2;
	U16 j = 0;
#if VIEWTYPE == HORIZONTAL //横屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (y >= Drawmy0))
		{
			wdt();
			x2 = x + dx;
			if (x2 > Drawmx1)
				x2 = Drawmx1;
			if (x < Drawmx0)
				x = Drawmx0;
			i = BufAdd + y * YOFFSET;
			x2 = i + x2 + x2;
			for (i = i + x + x; i < x2; i += 2)
			{
				if (j < 5)
					*((U16 *)i) = color;
				if (++j > 10)
					j = 0;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			x2 = x + dx;
			if (x2 > XLEN)
				x2 = XLEN;
			i = BufAdd + y * YOFFSET;
			x2 = i + x2 + x2;
			for (i = i + x + x; i < x2; i += 2)
			{
				if (j < 5)
					*((U16 *)i) = color;
				if (++j > 10)
					j = 0;
			}
		}
	}
#else //竖屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (y >= Drawmy0))
		{
			wdt();
			x2 = x + dx;
			if (x2 > Drawmx1)
				x2 = Drawmx1;
			if (x < Drawmx0)
				x = Drawmx0;
			i = BufAdd + x * XOFFSET + y + y;
			x2 = BufAdd + x2 * XOFFSET + y + y;
			for (; i < x2; i += XOFFSET)
			{
				if (j < 4)
					*((U16 *)i) = color;
				if (++j > 6)
					j = 0;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			x2 = x + dx;
			if (x2 > XLEN)
				x2 = XLEN;
			i = BufAdd + x * XOFFSET + y + y;
			x2 = BufAdd + x2 * XOFFSET + y + y;
			for (; i < x2; i += XOFFSET)
			{
				if (j < 4)
					*((U16 *)i) = color;
				if (++j > 6)
					j = 0;
			}
		}
	}

#endif
}

//------------------------------------------
// Function   : void DrawVLine(U32 x,U32 y,U32 dy,U32 color)
// Description: 垂直虚线
// note: 绘制 实虚比为  4:2 虚线
//------------------------------------------
void DrawVDottedLine(S32 x, S32 y, U32 dy, U32 color)
{
	S32 i, y2;
	U16 j = 0;
#if VIEWTYPE == HORIZONTAL //横屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0))
		{
			wdt();
			y2 = y + dy;
			if (y2 > Drawmy1)
				y2 = Drawmy1;
			if (y < Drawmy0)
				y = Drawmy0;
			y2 = BufAdd + x + x + y2 * YOFFSET;
			i = BufAdd + x + x + y * YOFFSET;
			for (; i < y2; i += YOFFSET)
			{
				if (j < 5)
					*((U16 *)i) = color;
				if (++j > 10)
					j = 0;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			y2 = BufAdd + x + x + y2 * YOFFSET;
			i = BufAdd + x + x + y * YOFFSET;
			for (; i < y2; i += YOFFSET)
			{
				if (j < 5)
					*((U16 *)i) = color;
				if (++j > 10)
					j = 0;
			}
		}
	}
#else //竖屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0))
		{
			wdt();
			y2 = y + dy;
			if (y2 > Drawmy1)
				y2 = Drawmy1;
			if (y < Drawmy0)
				y = Drawmy0;
			y2 = BufAdd + x * XOFFSET + y2 + y2;
			i = BufAdd + x * XOFFSET + y + y;
			for (; i < y2; i += 2)
			{
				if (j < 4)
					*((U16 *)i) = color;
				if (++j > 6)
					j = 0;
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			i = BufAdd + x * XOFFSET + y + y;
			y2 = BufAdd + x * XOFFSET + y2 + y2;
			for (; i < y2; i += 2)
			{
				if (j < 4)
					*((U16 *)i) = color;
				if (++j > 6)
					j = 0;
			}
		}
	}
#endif
}
//------------------------------------------
// Function   : void DrawRect(U32 x,U32 y,U32 dx,U32 dy,U32 color)
// Description: 描绘虚线长方形
//------------------------------------------
void DrawDottedRect(U32 x, U32 y, U32 dx, U32 dy, U32 color)
{
	DrawHDottedLine(x, y, dx, color);
	DrawVDottedLine(x + dx - 1, y, dy, color);
	DrawHDottedLine(x, y + dy - 1, dx, color);
	DrawVDottedLine(x, y, dy, color);
}

//------------------------------------------
// Function   : void DrawFillRect(U32 x,U32 y,U32 dx,U32 dy,U32 color)
// Description: 填长方形颜色
// note: 根据DrawLimtf确定是否描绘超过范围的形状
//------------------------------------------
void DrawFillRect(U32 x, U32 y, U32 dx, U32 dy, U32 color)
{
	U32 i, j, x2, y2;
#if VIEWTYPE == HORIZONTAL //横屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0) && (y >= Drawmy0))
		{
			wdt();
			if (x < Drawmx0)
				x = Drawmx0;
			if (y < Drawmy0)
				y = Drawmy0;

			y2 = y + dy;
			if (y2 > Drawmy1)
				y2 = Drawmy1;
			//if(y2>YLEN)y2 = YLEN;
			y2 = y2 * YOFFSET;
			for (j = y * YOFFSET; j < y2; j += YOFFSET)
			{
				x2 = x + dx;
				if (x2 > Drawmx1)
					x2 = Drawmx1;

				//if(x2>XLEN)x2 = XLEN;
				i = BufAdd + j;
				x2 = i + x2 + x2;
				for (i = i + x + x; i < x2; i += 2)
				{
					*((U16 *)i) = color;
				}
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			wdt();
			y2 = y + dy;
			//if(y2>YLEN)y2 = YLEN;
			y2 = y2 * YOFFSET;
			for (j = y * YOFFSET; j < y2; j += YOFFSET)
			{
				x2 = x + dx;
				//if(x2>XLEN)x2 = XLEN;
				i = BufAdd + j;
				x2 = i + x2 + x2;
				for (i = i + x + x; i < x2; i += 2)
				{
					*((U16 *)i) = color;
				}
			}
		}
	}

#else //竖屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0) && (y >= Drawmy0))
		{
			x2 = x + dx;
			if (x2 > Drawmx1)
				x2 = Drawmx1;
			if (x < Drawmx0)
				x = Drawmx0;
			x2 = x2 * XOFFSET;
			for (j = x * XOFFSET; j < x2; j += XOFFSET)
			{
				y2 = y + dy;
				if (y2 > Drawmy1)
					y2 = Drawmy1;
				if (y < Drawmy0)
					y = Drawmy0;
				i = BufAdd + j;
				y2 = i + y2 + y2;
				for (i = i + y + y; i < y2; i += 2)
				{
					*((U16 *)i) = color;
				}
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			x2 = x + dx;
			if (x2 > XLEN)
				x2 = XLEN;
			x2 = x2 * XOFFSET;
			for (j = x * XOFFSET; j < x2; j += XOFFSET)
			{
				y2 = y + dy;
				if (y2 > YLEN)
					y2 = YLEN;
				i = BufAdd + j;
				y2 = i + y2 + y2;
				for (i = i + y + y; i < y2; i += 2)
				{
					*((U16 *)i) = color;
				}
			}
		}
	}
#endif
}

//------------------------------------------
// Function   : void DrawFillRect(U32 x,U32 y,U32 dx,U32 dy,U32 color)
// Description: 填长方形颜色
// note: 不限制显示范围
//------------------------------------------
void DrawFillRect1(U32 x, U32 y, U32 dx, U32 dy, U32 color)
{
	U32 i, j, x2, y2;
#if VIEWTYPE == HORIZONTAL //横屏
	//wdt();
	y2 = y + dy;
	//if(y2>YLEN)y2 = YLEN;
	y2 = y2 * YOFFSET;
	for (j = y * YOFFSET; j < y2; j += YOFFSET)
	{
		x2 = x + dx;
		//if(x2>XLEN)x2 = XLEN;
		i = BufAdd + j;
		x2 = i + x2 + x2;
		for (i = i + x + x; i < x2; i += 2)
		{
			*((U16 *)i) = color;
		}
	}
#else //竖屏
	if (DrawLimtf)
	{
		if ((x < Drawmx1) && (y < Drawmy1) && (x >= Drawmx0) && (y >= Drawmy0))
		{
			x2 = x + dx;
			if (x2 > Drawmx1)
				x2 = Drawmx1;
			if (x < Drawmx0)
				x = Drawmx0;
			x2 = x2 * XOFFSET;
			for (j = x * XOFFSET; j < x2; j += XOFFSET)
			{
				y2 = y + dy;
				if (y2 > Drawmy1)
					y2 = Drawmy1;
				if (y < Drawmy0)
					y = Drawmy0;
				i = BufAdd + j;
				y2 = i + y2 + y2;
				for (i = i + y + y; i < y2; i += 2)
				{
					if (*((U16 *)i) != color)
						*((U16 *)i) = color;
				}
			}
		}
	}
	else
	{
		if ((x < XLEN) && (y < YLEN))
		{
			x2 = x + dx;
			if (x2 > XLEN)
				x2 = XLEN;
			x2 = x2 * XOFFSET;
			for (j = x * XOFFSET; j < x2; j += XOFFSET)
			{
				y2 = y + dy;
				if (y2 > YLEN)
					y2 = YLEN;
				i = BufAdd + j;
				y2 = i + y2 + y2;
				for (i = i + y + y; i < y2; i += 2)
				{
					if (*((U16 *)i) != color)
						*((U16 *)i) = color;
				}
			}
		}
	}
#endif
}
//------------------------------------------
// Function   : void DrawDFrame(U32 x, U32 y, U32 dx, U32 dy, U32 t, U32 c1)
// Description: 画双线框
// note: 绘制带内描边的实线方框
//------------------------------------------
void DrawDFrame(U32 x, U32 y, U32 dx, U32 dy, U32 t, U32 c1)
{
	DrawVLine(x, y, dy, c1);
	DrawHLine(x, y, dx, c1);
	DrawHLine(x, y + dy - 1, dx, c1);
	DrawVLine(x + dx - 1, y, dy, c1);

	DrawVLine(x + t, y + t, dy - 2 * t, c1);
	DrawHLine(x + t, y + t, dx - 2 * t, c1);
	DrawHLine(x + t, y + dy - t - 1, dx - 2 * t, c1);
	DrawVLine(x + dx - t - 1, y + t, dy - 2 * t, c1);

	DrawLine(x, y, x + t, y + t, c1);
	DrawLine(x, y + dy - 1, x + t, y + dy - t - 1, c1);
	DrawLine(x + dx - 1, y, x + dx - t - 1, y + t, c1);
	DrawLine(x + dx - 1, y + dy - 1, x + dx - t - 1, y + dy - t - 1, c1);
}

//------------------------------------------
// Function   : void DrawFrame(U32 x, U32 y, U32 dx, U32 dy, U32 c1, U32 c2)
// Description: 画框
// note: 绘制实线方框，过起始点的线颜色为c1，过对称点的线颜色为c2
// 如按钮的边框渐变
//------------------------------------------
void DrawFrame(U32 x, U32 y, U32 dx, U32 dy, U32 c1, U32 c2)
{
	DrawVLine(x, y, dy, c1);
	DrawHLine(x, y, dx, c1);
	DrawHLine(x, y + dy - 1, dx, c2);
	DrawVLine(x + dx - 1, y, dy, c2);
}

//------------------------------------------
// Function   : void DrawFrame2(U32 x, U32 y, U32 dx, U32 dy, U32 c1, U32 c2)
// Description: 画框
// note: 绘制实线方框，过起始点的线颜色为c1，过对称点的线颜色为c2
// 该方框的 起始点 的颜色 为 Color_LightGray
//------------------------------------------
void DrawFrame2(U32 x, U32 y, U32 dx, U32 dy, U32 c1, U32 c2)
{
	DrawVLine(x, y, dy, Color_LightGray);
	DrawVLine(x + 1, y, dy, c1);
	DrawHLine(x, y, dx, Color_LightGray);
	DrawHLine(x, y + 1, dx, c1);

	DrawHLine(x, y + dy - 1, dx, c2);
	DrawHLine(x, y + dy - 1 - 1, dx, c2);
	DrawVLine(x + dx - 1, y, dy, c2);
	DrawVLine(x + dx - 1 - 1, y, dy, c2);
}

//------------------------------------------
// Function   : void DrawCorFrame(U32 x, U32 y, U32 d, U32 dt, U32 w, U32 c1, U32 c2)
// Description: 带角正方形方框	w:1,2,3,4=左上,右上,左下,右下
// note： d:方框的边长，dt:正三角边长   d>dt
//------------------------------------------
void DrawCorFrame(U32 x, U32 y, U32 d, U32 dt, U32 w, U32 c1, U32 c2)
{
	if (w == 1)
	{
		DrawLine(x, y + dt, x + dt, y, c1);

		DrawHLine(x + dt, y, d - dt, c1);
		DrawVLine(x, y + dt, d - dt, c1);
		DrawHLine(x, y + d - 1, d, c2);
		DrawVLine(x + d - 1, y, d, c2);
	}
	else if (w == 2)
	{
		DrawLine(x + d - dt, y, x + d, y + dt, c1);

		DrawVLine(x, y, d, c1);
		DrawHLine(x, y, d - dt, c1);
		DrawHLine(x, y + d - 1, d, c2);
		DrawVLine(x + d - 1, y + dt, d - dt, c2);
	}
	else if (w == 3)
	{
		DrawLine(x, y + d - dt, x + dt - 1, y + d - 1, c2);

		DrawVLine(x, y, d - dt, c1);
		DrawHLine(x, y, d, c1);
		DrawHLine(x + dt, y + d - 1, d - dt, c2);
		DrawVLine(x + d - 1, y, d, c2);
	}
	else if (w == 4)
	{
		DrawLine(x + d - dt, y + d - 1, x + d - 1, y + d - dt, c2);

		DrawVLine(x, y, d, c1);
		DrawHLine(x, y, d, c1);
		DrawHLine(x, y + d - 1, d - dt, c2);
		DrawVLine(x + d - 1, y, d - dt, c2);
	}
}

//------------------------------------------
// Function   : void DrawFillCorRect(U32 x, U32 y, U32 d, U32 dt, U32 w, U32 color)
// Description: 填充带角正方形颜色	w:1,2,3,4=左上,右上,左下,右下
// note： d:方框的边长，dt:正三角边长   d>dt   【此处是通过绘制N条线的方式进行填充】
//------------------------------------------
void DrawFillCorRect(U32 x, U32 y, U32 d, U32 dt, U32 w, U32 color)
{
	U32 i, xx, dx;

	for (i = 0; i < dt; i++)
	{
		if (w == 1)
			xx = x + dt - i;
		else
			xx = x;
		if ((w == 1) || (w == 2))
			dx = d - dt + i;
		else
			dx = d;
		DrawHLine(xx, y + i, dx, color);
	}
	for (i = dt; i < d - dt; i++)
	{
		DrawHLine(x, y + i, d, color);
	}
	for (i = 0; i < dt; i++)
	{
		if (w == 3)
			xx = x + i;
		else
			xx = x;
		if ((w == 3) || (w == 4))
			dx = d - i;
		else
			dx = d;
		DrawHLine(xx, y + d - dt + i, dx, color);
	}
}

//------------------------------------------
// Function   : void DrawColor_Gray(U32 x, U32 y,U32 dx,U32 dy)
// Description: 图形变灰
//------------------------------------------
void DrawColor_Gray(U32 x, U32 y, U32 dx, U32 dy)
{

#if VIEWTYPE == HORIZONTAL //横屏
	U32 i, j, x2, y2;
	U16 col_r, col_g, col_b;
	U16 *dval;
	U16 *dadd;
	U16 data2;
	U32 data3;

	x2 = (x + dx) * 2;
	y2 = y + y + dy * YOFFSET;
	for (j = y + y; j < y2; j += YOFFSET)
	{
		dadd = (U16 *)(LCD_VRAM_BASE_ADDR + (3 - usepageno) * DISP_PAGE_SIZE + j + x + x);
		for (i = x + x; i < x2; i += 2)
		{
			dval = (U16 *)(BufAdd + j + i);
			data2 = *(dval);

			col_r = ((data2 & 0xf800) >> 11) * 8;
			col_g = ((data2 & 0x07e0) >> 5) * 4;
			col_b = (data2 & 0x001f) * 8;
			col_r = (col_r + col_g + col_b) / 3;
			data3 = RGB(col_r, col_r, col_r);

			*(dadd) = data3;
			dadd++;
		}
		wdt();
	}
#else //竖屏
	U16 data2;
	U32 data3;
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;
	U32 dadd;
	//printf("DrawColor_Gray 竖屏未实现!!!");
	x2 = x + dx;
	y2 = y + dy;
	if (x2 > LCD_width)
		x2 = LCD_width;
	if (y2 > LCD_height)
		y2 = LCD_height;

	if (y >= y2)
		return;
	if (x >= x2)
		return;

	x2 *= XOFFSET;
	x *= XOFFSET;
	while (x < x2)
	{
		y2 = y + dy;
		if (y2 > YLEN)
			y2 = YLEN;
		i = BufAdd + x;
		dadd = LCD_VRAM_BASE_ADDR + (3 - usepageno) * DISP_PAGE_SIZE;
		y2 = i + y2 + y2;
		for (i = i + y + y; i < y2; i += 2, dadd += 2)
		{
			data2 = *((U16 *)i);

			col_r = ((data2 & 0xf800) >> 11) * 8;
			col_g = ((data2 & 0x07e0) >> 5) * 4;
			col_b = (data2 & 0x001f) * 8;
			col_r = (col_r + col_g + col_b) / 3;
			data3 = RGB(col_r, col_r, col_r);

			//*((U16 *)i) = data3;
			//*(dadd) = data3;
			*((U16 *)dadd) = data3;
		}
		x += XOFFSET;

		wdt();
	}

#endif
}

//------------------------------------------
// Function   : void DrawColorRreverse(U32 x, U32 y,U32 dx,U32 dy)
// Description: 指定区域 图形取反
// 在565格式下，它们则分别为：R 0xF800(1111 1000 0000 0000)、G 0x07E0(0000 0111 1110 0000)、B 0x001F(0000 0000 0001 1111)
//------------------------------------------
void DrawColorRreverse(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;

	U16 data2;
	U32 data3;

#if VIEWTYPE == HORIZONTAL //横屏
	U16 *dadd;
	U16 *dval;

	x2 = x + dx;
	y2 = y + dy;
	if (y2 > LCD_height)
		y2 = LCD_height;
	if (y >= y2)
		return;

	if (x2 > LCD_width)
		x2 = LCD_width;
	if (x >= x2)
		return;
	while (y < y2)
	{
		dadd = (U16 *)(BufAdd + 2 * LCD_width * y + 2 * x);
		for (i = (2 * x); i < (2 * x2); i += 2)
		{
			dval = (U16 *)(BufAdd + 2 * LCD_width * y + i);
			data2 = *(dval);
			col_r = 255 - ((data2 & 0xf800) >> 11) * 8;
			col_g = 255 - ((data2 & 0x07e0) >> 5) * 4;
			col_b = 255 - (data2 & 0x001f) * 8;

			data3 = RGB(col_r, col_g, col_b);

			*(dadd) = data3;
			dadd++;
		}
		y++;
		wdt();
	}
#else //竖屏
	x2 = x + dx;
	y2 = y + dy;
	if (x2 > LCD_width)
		x2 = LCD_width;
	if (y2 > LCD_height)
		y2 = LCD_height;

	if (y >= y2)
		return;
	if (x >= x2)
		return;

	x2 *= XOFFSET;
	x *= XOFFSET;
	while (x < x2)
	{
		y2 = y + dy;
		if (y2 > YLEN)
			y2 = YLEN;
		i = BufAdd + x;
		y2 = i + y2 + y2;
		for (i = i + y + y; i < y2; i += 2)
		{
			data2 = *((U16 *)i);

			col_r = 255 - ((data2 & 0xf800) >> 11) * 8;
			col_g = 255 - ((data2 & 0x07e0) >> 5) * 4;
			col_b = 255 - (data2 & 0x001f) * 8;
			// col_r = 0xaa ^ (((data2&0xf800)>>11)*8);
			//col_g = 0xaa ^ (((data2&0x07e0)>>5)*4);
			//col_b = 0xaa ^ ((data2&0x001f)*8);
			data3 = RGB(col_r, col_g, col_b);

			*((U16 *)i) = data3;
		}
		x += XOFFSET;

		wdt();
	}
#endif
}

//------------------------------------------
// Function   : void DrawColor_Gray(U32 x, U32 y,U32 dx,U32 dy)
// Description: 指定区域 图形变灰
//------------------------------------------
void DrawColor_Gray1(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;

	U16 data2;
	U32 data3;

#if VIEWTYPE == HORIZONTAL //横屏
	U16 *dadd;
	U16 *dval;

	x2 = x + dx;
	y2 = y + dy;
	if (y2 > LCD_height)
		y2 = LCD_height;
	if (y >= y2)
		return;

	if (x2 > LCD_width)
		x2 = LCD_width;
	if (x >= x2)
		return;
	while (y < y2)
	{
		dadd = (U16 *)(BufAdd + 2 * LCD_width * y + 2 * x);
		for (i = (2 * x); i < (2 * x2); i += 2)
		{
			dval = (U16 *)(BufAdd + 2 * LCD_width * y + i);
			data2 = *(dval);
			col_r = ((data2 & 0xf800) >> 11) * 8;
			col_g = ((data2 & 0x07e0) >> 5) * 4;
			col_b = (data2 & 0x001f) * 8;
			col_r = (col_r + col_g + col_b) / 3;
			data3 = RGB(col_r, col_r, col_r);

			*(dadd) = data3;
			dadd++;
		}
		y++;
		wdt();
	}
#else //竖屏
	x2 = x + dx;
	y2 = y + dy;
	if (x2 > LCD_width)
		x2 = LCD_width;
	if (y2 > LCD_height)
		y2 = LCD_height;

	if (y >= y2)
		return;
	if (x >= x2)
		return;

	x2 *= XOFFSET;
	x *= XOFFSET;
	while (x < x2)
	{
		y2 = y + dy;
		if (y2 > YLEN)
			y2 = YLEN;
		i = BufAdd + x;
		y2 = i + y2 + y2;
		for (i = i + y + y; i < y2; i += 2)
		{
			data2 = *((U16 *)i);

			col_r = ((data2 & 0xf800) >> 11) * 8;
			col_g = ((data2 & 0x07e0) >> 5) * 4;
			col_b = (data2 & 0x001f) * 8;
			col_r = (col_r + col_g + col_b) / 3;
			data3 = RGB(col_r, col_r, col_r);

			*((U16 *)i) = data3;
		}
		x += XOFFSET;

		wdt();
	}
#endif
}

//------------------------------------------
// Function   : void DrawBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
// Description: 图象显示			  198mS		154mS
// note : 原图显示
//------------------------------------------
void DrawBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
{
#if VIEWTYPE == HORIZONTAL //横屏
	U32 i, j;			   //, x2, y2;
	U16 *padd;
	padd = (U16 *)(BufAdd + y * YOFFSET + x + x);
	for (i = 0; i < dy; i++)
	{
		wdt();
		for (j = 0; j < dx; j++)
		{
			*(padd + j) = *p;
			p++;
		}
		padd += XLEN;
	}
#else
	U32 i, j, x2, y2;
	for (i = 0; i < dy; i++)
	{
		wdt();
		for (j = 0; j < dx; j++)
		{
			PutPixel(x + j, y + i, *p);
			p++;
		}
	}
//	if((x<XLEN)&&(y<YLEN)){
//		x2 = x+dx;
//		if(x2>XLEN)x2 = XLEN;
//		x2 *= XOFFSET;
//		x *= XOFFSET;
//		while(x<x2){
//			y2 = y+dy;
//			if(y2>YLEN)y2 = YLEN;
//			i = BufAdd + x;
//			y2 = i + y2 + y2;
//			for(i=i+y+y;i<y2;i+=2){
//				*((U16 *)i) = *p;
//				p +=2;
//			}
//			x += XOFFSET;
//
//			wdt();
//		}
//}
//		x2 = x2 * XOFFSET;
//		for(j=x*XOFFSET;j<x2;j+=XOFFSET){
//			y2 = y+dy;
//			if(y2>YLEN)y2 = YLEN;
//			i = BufAdd + j;
//			y2 = i + y2 + y2;
//			for(i=i+y+y;i<y2;i+=2){
//				*((U16 *)i) = *p;
//				p +=1;
//			}
//		}
//	}
#endif
	//CLRTEST1();
}

//------------------------------------------
// Function   : void DrawBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
// Description: 图象显示			  198mS		154mS
// note : 剔除白色显示
//------------------------------------------
void DrawBMP1(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
{
#if VIEWTYPE == HORIZONTAL //横屏
	U32 i, j;			   //, x2, y2;
	U16 *padd;
	padd = (U16 *)(BufAdd + y * YOFFSET + x + x);
	for (i = 0; i < dy; i++)
	{
		wdt();
		for (j = 0; j < dx; j++)
		{
			if (*p != 0xffff)
			{
				*(padd + j) = *p;
			}
			p++;
		}
		padd += XLEN;
	}
#else
	U32 i, j, x2, y2;
	for (i = 0; i < dy; i++)
	{
		wdt();
		for (j = 0; j < dx; j++)
		{
			if (*p != 0xffff)
				PutPixel(x + j, y + i, *p);
			p++;
		}
	}

#endif
	//CLRTEST1();
}
//------------------------------------------
// Function   : void DrawBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
// Description: 图象显示			  198mS		154mS
// note : R与B反色
// 在565格式下，它们则分别为：R 0xF800(1111 1000 0000 0000)、G 0x07E0(0000 0111 1110 0000)、B 0x001F(0000 0000 0001 1111)
//------------------------------------------

void DrawBMP2(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
{
	U32 i, j, x2, y2;
	U16 dat;

	x2 = x + dx;
	y2 = y + dy;
	for (i = y; i < y2; i++)
	{
		wdt();
		for (j = x; j < x2; j++)
		{
			dat = *p;
			dat = (dat >> 11) | (dat << 11) | (dat & 0x7e0); //0x7e0位16位BMP的其中一个单色最值 565格式
			PutPixel(j, i, dat);
			p++;
		}
	}
}

//------------------------------------------
// Function   : void DrawPBMP(U32 x, U32 y, U32 dx, U32 dy, U8 *p)
// Description: 图象显示程序压缩数据
// note:
//------------------------------------------
void DrawPBMP(U32 x, U32 y, U32 dx, U32 dy, U8 *p)
{
	U32 i, j, x2, y2;
	U8 dat1, dat2;
	U32 color = 0;
	U32 tc = 0;

	x2 = x + dx;
	y2 = y + dy;
	for (i = x; i < x2; i += 1)
	{
		for (j = y; j < y2; j += 1)
		{
			if (tc == 0)
			{
				tc = *p;
				p++;
				dat1 = *p;
				p++;
				dat2 = *p;
				p++;
				color = 0;
				color = dat1 + dat2 * 256;
				color = (color << 16) | color; //
			}
			PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}
//------------------------------------------
// Function   : void DrawFPBMP3(U32 x, U32 y, U32 dx, U32 dy, U8 *p)
// Description: 图象显示程序压缩数据
// note:
//------------------------------------------
void DrawFPBMP3(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U16 color = 0;
	U32 tc = 0;

	if (f == 1)
	{
		dat = (*p >> 8) & 0xff;
		p++;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j++)
	{
		wdt();
		for (i = x; i < x2; i++)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = *p & 0xff;
					dat1 = (*p >> 8) & 0xff;
					p++;
					dat2 = *p & 0xff;
					dat = (*p >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = *p & 0xff;
					dat2 = (*p >> 8) & 0xff;
					p++;
				}
				color = dat1 + dat2 * 256;
			}
			PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}

//------------------------------------------
// Function   : void DrawFPBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f)
// Description: 图象显示程序压缩数据
// note:?????
//------------------------------------------
void DrawFPBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U32 color = 0;
	U32 tc = 0;

	if (f == 1)
	{
		dat = (*p >> 8) & 0xff;
		p++;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (i = x; i < x2; i += 1)
	{
		for (j = y; j < y2; j += DISP_PARA_Y)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = *p & 0xff;
					dat1 = (*p >> 8) & 0xff;
					p++;
					dat2 = *p & 0xff;
					dat = (*p >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = *p & 0xff;
					dat2 = (*p >> 8) & 0xff;
					p++;
				}
				color = 0;
				color = dat1 + dat2 * 256;
				color = (color << 16) | color;
			}
			PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}
//------------------------------------------
// Function   : void DrawFBMP2(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 color)
// Description: 单色数据图象显示
//------------------------------------------
void DrawFBMP2(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 color)
{
	U32 i, j, k;
	U32 x2, y2;
	U8 Ch, bb;
	U32 f = 0;
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j += 1)
	{
		i = x;
		while (i < x2)
		{
			if (f == 0)
			{
				f = 1;
				Ch = *p & 0xff;
			}
			else
			{
				f = 0;
				Ch = (*p >> 8) & 0xff;
				p++;
			}
			bb = 0x80;
			for (k = 0; k < 8; k++)
			{
				if (Ch & bb)
				{
					PutPixel(i, j, color);
				}
				bb = bb >> 1;
				i += 1;
			}
		}
	}
}

//------------------------------------------
// Function   : void DrawFPBMPNEW(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f)
// Description: 图象显示程序压缩数据，并去掉白色像素显示
// note:用于显示底图为白色的图片h或gif
//------------------------------------------
void DrawFPBMPNEW(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U16 num)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U8 f;
	U16 color = 0;
	U32 tc = 0;
	//U16 *padd;

	j = num * 2;
	i = (*(p + j)) + ((*(p + j + 1)) << 16);
	//	DrawMFHEX(8, 60, i, 8, Color_Black);
	if ((i % 2) == 1)
	{ // 奇数地址
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // 偶数地址
		i >>= 1;
		p += i;
		f = 0;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j += 1)
	{
		for (i = x; i < x2; i += 1)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = (*p) & 0xff;
					dat1 = ((*p) >> 8) & 0xff;
					p++;
					dat2 = (*p) & 0xff;
					dat = ((*p) >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = (*p) & 0xff;
					dat2 = ((*p) >> 8) & 0xff;
					p++;
				}
				color = dat1 + dat2 * 256;
			}
			if (color != 0xffff)
				PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}
//------------------------------------------
// Function   : void DrawFPBMPNEW(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f)
// Description: 图象显示程序压缩数据，并去掉白色像素显示，图像灰化
// note:用于显示底图为白色的图片h或gif，
//------------------------------------------
void DrawFPBMPGray(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U16 num)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U8 f;
	U16 color = 0, col_r, col_g, col_b;

	U32 tc = 0;
	//U16 *padd;

	j = num * 2;
	i = (*(p + j)) + ((*(p + j + 1)) << 16);
	//	DrawMFHEX(8, 60, i, 8, Color_Black);
	if ((i % 2) == 1)
	{ // 奇数地址
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // 偶数地址
		i >>= 1;
		p += i;
		f = 0;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j += 1)
	{
		for (i = x; i < x2; i += 1)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = (*p) & 0xff;
					dat1 = ((*p) >> 8) & 0xff;
					p++;
					dat2 = (*p) & 0xff;
					dat = ((*p) >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = (*p) & 0xff;
					dat2 = ((*p) >> 8) & 0xff;
					p++;
				}
				color = dat1 + dat2 * 256;
				if (color != 0xffff)
				{
					col_r = ((color & 0xf800) >> 11) * 8;
					col_g = ((color & 0x07e0) >> 5) * 4;
					col_b = (color & 0x001f) * 8;

					col_r = (col_r + col_g + col_b) / 3;
					color = RGB(col_r, col_r, col_r);
				}
				wdt();
			}
			if (color != 0xffff)
				PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}

//------------------------------------------
// Function   : void DrawFPBMPBTN(U32 x, U32 y, U32 dx, U32 dy, U32 num, U16 color2)
// Description: 图象显示程序压缩数据，并去掉白色像素显示
// note:用于显示底图为白色的图片h或gif
//------------------------------------------
void DrawFPBMPBTN(U32 x, U32 y, U32 dx, U32 dy, U32 num, U16 color2)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U8 f;
	U16 flag;
	U16 color = 0, color1 = 0;
	U32 tc = 0;
	U16 *p;

	flag = num >> 16;
	num = num & 0xffff;
	p = MEM_ICONBMP;

	j = num * 2;
	i = (*(p + j)) + ((*(p + j + 1)) << 16);

	if (i > g_bmplen)
		return; //图形起始地址超范围

	if ((i % 2) == 1)
	{ // 奇数地址
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // 偶数地址
		i >>= 1;
		p += i;
		f = 0;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j += 1)
	{
		for (i = x; i < x2; i += 1)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = (*p) & 0xff;
					dat1 = ((*p) >> 8) & 0xff;
					p++;
					dat2 = (*p) & 0xff;
					dat = ((*p) >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = (*p) & 0xff;
					dat2 = ((*p) >> 8) & 0xff;
					p++;
				}
				color = dat1 + dat2 * 256;
				color1 = color;
				if (flag)
					color = ~color;
			}
			if (color1 == color2)
			{
			}
			else
				PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}

//------------------------------------------
// Function   : void DrawFPBMPBTN1(U32 x, U32 y, U32 dx, U32 dy, U32 num)
// Description: 图象显示程序压缩数据，
// note:
//------------------------------------------
void DrawFPBMPBTN1(U32 x, U32 y, U32 dx, U32 dy, U32 num)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U8 f;
	U16 flag;
	U16 color = 0;
	U32 tc = 0;
	U16 *p;

	flag = num >> 16;
	num = num & 0xffff;
	p = MEM_ICONBMP;

	j = num * 2;
	i = (*(p + j)) + ((*(p + j + 1)) << 16);

	if (i > g_bmplen)
		return; //图形起始地址超范围

	if ((i % 2) == 1)
	{ // 奇数地址
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // 偶数地址
		i >>= 1;
		p += i;
		f = 0;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j += 1)
	{
		for (i = x; i < x2; i += 1)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = (*p) & 0xff;
					dat1 = ((*p) >> 8) & 0xff;
					p++;
					dat2 = (*p) & 0xff;
					dat = ((*p) >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = (*p) & 0xff;
					dat2 = ((*p) >> 8) & 0xff;
					p++;
				}
				color = dat1 + dat2 * 256;
				//color1 = color;
				if (flag)
					color = ~color;
			}
			PutPixel(i, j, color);
			if (tc > 0)
				tc--;
		}
	}
}

//------------------------------------------
// Function   : void DrawFPBMPBTN1(U32 x, U32 y, U32 dx, U32 dy, U32 num)
// Description: 图象显示程序压缩数据，
// note:存在显示位置大小限制
//------------------------------------------
void DrawFPBMPBTN2(U32 x, U32 y, U32 dx, U32 dy, U32 num)
{
	U32 i, j, x2, y2;
	U8 dat = 0, dat1, dat2;
	U8 f;
	U16 color = 0;
	U32 tc = 0;
	U16 *p;

	num = num & 0xffff;
	p = MEM_ICONBMP;

	j = num * 2;
	i = (*(p + j)) + ((*(p + j + 1)) << 16);

	if (i > g_bmplen)
		return; //图形起始地址超范围

	if ((i % 2) == 1)
	{ // 奇数地址
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // 偶数地址
		i >>= 1;
		p += i;
		f = 0;
	}
	x2 = x + dx;
	y2 = y + dy;
	for (j = y; j < y2; j += 1)
	{
		for (i = x; i < x2; i += 1)
		{
			if (tc == 0)
			{
				if (f == 0)
				{
					f = 1;
					tc = (*p) & 0xff;
					dat1 = ((*p) >> 8) & 0xff;
					p++;
					dat2 = (*p) & 0xff;
					dat = ((*p) >> 8) & 0xff;
					p++;
				}
				else
				{
					f = 0;
					tc = dat;
					dat1 = (*p) & 0xff;
					dat2 = ((*p) >> 8) & 0xff;
					p++;
				}
				color = dat1 + dat2 * 256;
				//color1 = color;
			}
			if ((DrawLimtf) && (i < Drawmx0 || i > Drawmx1 || j < Drawmy0 || j > Drawmy1))
			{
			}
			else
			{
				PutPixel(i, j, color);
			}

			if (tc > 0)
				tc--;
		}
	}
}

//------------------------------------------
// Function   : void dsp_DatTime(U32 x, U32 y,U16 yr, U16 md, U16 hm, U32 color)
// Description: 显示日期 时间
//------------------------------------------
void dsp_DatTime(U32 x, U32 y, U16 yr, U16 md, U16 hm, U32 color)
{
	char s[2];
	U32 px = x;

	s[1] = 0;

	s[0] = 0x32;
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30;
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + ((yr >> 4) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + (yr & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = '-';
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + ((md >> 12) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + ((md >> 8) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = '-';
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + ((md >> 4) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + (md & 0xf);
	DrawFChar(px, y, s, color);
	px += 16;

	s[0] = 0x30 + ((hm >> 12) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + ((hm >> 8) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = ':';
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + ((hm >> 4) & 0xf);
	DrawFChar(px, y, s, color);
	px += 8;
	s[0] = 0x30 + (hm & 0xf);
	DrawFChar(px, y, s, color);
}

//------------------------------------------
// Function   : void dsp_F16x8(U32 x, U32 y, U16 *p, U32 color)
// Description: 16*8数据描绘 直接根据16进制数据进行画图
//------------------------------------------
void dsp_F16x8(U32 x, U32 y, U16 *p, U32 color)
{
	U32 i, j;
	U16 dat, b, bb;
	U32 xx, yy;
	U16 *add;

	add = p;
	yy = y;
	for (i = 0; i < 8; i++)
	{
		dat = *add;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		xx = x;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++;
			if (j == 7)
			{
				yy++;
				xx = x;
			}
		}
		yy++;
		add++;
	}
}

//------------------------------------------
// Function   : void dsp_F24x12(U32 x, U32 y, U16 *p, U32 color)
// Description: 24*12数据描绘  直接根据16进制数据进行画图
//------------------------------------------
void dsp_F24x12(U32 x, U32 y, U16 *p, U32 color)
{
	U32 i, j;
	U16 dat, b, bb;
	U32 xx, yy;
	U16 *add;

	add = p;
	yy = y;
	for (i = 0; i < 24; i++)
	{
		dat = *add;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		xx = x;
		for (j = 0; j < 12; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++;
		}
		yy++;
		add++;
	}
}

//------------------------------------------
// Function   : void dsp_F24x24(U32 x, U32 y, U16 *p, U32 color)
// Description: 24*24ASCII描绘  直接根据16进制数据进行画图
//------------------------------------------
void dsp_F24x24(U32 x, U32 y, U16 *p, U32 color)
{
	U32 i, j;
	U16 dat, b, bb;
	U32 xx, yy;
	U16 *add;

	add = p;
	yy = y; //xx=x;//
	for (i = 0; i < 12; i++)
	{
		dat = *add;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		xx = x; //yy = y;//
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++; //yy++;//
		}
		add++;
		dat = *add;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		for (j = 0; j < 8; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++; //yy++;//
		}
		yy++;	//xx++;//
		xx = x; //yy = y;//
		for (j = 0; j < 8; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++; //yy++;//
		}
		add++;
		dat = *add;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++; //yy++;//
		}
		yy++; //xx++;//
		add++;
	}
}

//------------------------------------------
// Function   : void dsp_F32x16(U32 x, U32 y, U16 *p, U32 color)
// Description: 32*16数据描绘  直接根据16进制数据进行画图
//------------------------------------------
void dsp_F32x16(U32 x, U32 y, U16 *p, U32 color)
{
	U32 i, j;
	U16 dat, b, bb;
	U32 xx, yy;
	U16 *add;

	add = p;
	yy = y;
	for (i = 0; i < 32; i++)
	{
		dat = *add;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		xx = x;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++;
		}
		yy++;
		add++;
	}
}

//------------------------------------------
// Function   : void DrawFChar(U32 x, U32 y, char *s, U32 color)
// Description: 16*8数据描绘 ASCLL
//------------------------------------------
void DrawFChar(U32 x, U32 y, char *s, U32 color)
{
	U32 i, len;
	U16 *p;

	p = MEM_ASCII_16x8A; //(U16 *)MEM_ASCII_16x8;
	i = 0;
	while (*(s + i) != '\0')
	{
		len = (U32)((*(s + i) - 0x20)) * 8;
		dsp_F16x8(x + i * 9, y, (U16 *)(p + len), color);
		i++;
	}
}

//------------------------------------------
// Function   : void DrawFStr16(U32 x, U32 y, char *s, U32 color)
// Description: 16*16字符串描绘 ASCLL
//------------------------------------------
void DrawFStr16(U32 x, U32 y, char *s, U32 color)
{
	U32 i, len;
	U16 *p;

	p = MEM_ASCII_16x16A; //(U16 *)(ASCII_16x16AADD);//(U16 *)MEM_ASCII_16x16;
	i = 0;
	while (*(s + i) != '\0')
	{
		len = (*(s + i) - 0x20) * 16;
		dsp_F16x8(x + i * 16, y, (U16 *)(p + len), color);
		dsp_F16x8(x + i * 16 + 8, y, (U16 *)(p + len + 8), color);
		i++;
	}
}

//------------------------------------------
// Function   : void DrawFASC12(U32 x, U32 y, char *s, U32 color)
// Description: 24*12数据描绘 ASCLL
//------------------------------------------
void DrawFASC12(U32 x, U32 y, char *s, U32 color)
{
	U32 i, len;
	U16 *p;

	p = MEM_ASCII_24x12A; //(U16 *)(ASCII_24x12AADD);//(U16 *)ASCII_24x12A;//
	i = 0;
	while (*(s + i) != '\0')
	{
		len = (U32)((*(s + i) - 0x20)) * 24;
		dsp_F24x12(x + i * 13, y, (U16 *)(p + len), color);
		i++;
	}
}

//------------------------------------------
// Function   : void DrawFASC24(U32 x, U32 y, char *s, U32 color)
// Description: 24*24数据描绘 ASCLL
//------------------------------------------
void DrawFASC24(U32 x, U32 y, char *s, U32 color)
{
	U32 i, len;
	U16 *p;

	p = MEM_ASCII_24x24A; //(U16 *)(ASCII_24x24AADD);//(U16 *)ASCII_24x24A;//
	i = 0;
	while (*(s + i) != '\0')
	{
		len = (U32)((*(s + i) - 0x20)) * 36;
		dsp_F24x24(x + i * 24, y, (U16 *)(p + len), color);
		i++;
	}
}

//------------------------------------------
// Function   : void DrawFASC3216(U32 x, U32 y, char *s, U32 color)
// Description: 32*16数据描绘 ASCLL
//------------------------------------------
void DrawFASC3216(U32 x, U32 y, char *s, U32 color)
{
	U32 i, len;
	U16 *p;

	p = MEM_ASCII_32x16A; //(U16 *)(ASCII_32x16AADD);//(U16 *)ASCII_24x24A;//
	i = 0;
	while (*(s + i) != '\0')
	{
		len = (U32)((*(s + i) - 0x20)) * 32;
		dsp_F32x16(x + i * 20, y, (U16 *)(p + len), color);
		i++;
	}
}

//------------------------------------------
// Function   : void DrawFHz(U32 x, U32 y, char c1, char c2, U32 color)
// Description: 16*16汉字描绘(查字库) CHINESE  宋体
//------------------------------------------
void DrawFHz(U32 x, U32 y, char c1, char c2, U32 color)
{
	U32 i, j;
	U32 c = 0;
	U16 *p;
	U16 dat, b, bb;
	U32 xx, yy;

	p = MEM_HzLib_CHS16A; //(U16 *)(HzLib_CHS16AADD);//(U16 *)MEM_HzLib_16x16;
	if (c1 >= 0xb0)
	{
		c = (((U32)(c1 - 0xb0)) * 94 + c2 - 0xa1) * 16;
	}
	p = p + c;
	yy = y;

	for (i = 0; i < 16; i++)
	{
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		xx = x;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			xx++;
		}
		yy++;
		p++;
	}
}

//------------------------------------------
// Function   : void DrawFHz24X24(U32 x, U32 y, char c1, char c2, U32 color)
// Description: 24*24汉字描绘(查字库) CHINESE   楷体
//------------------------------------------
void DrawFHz24X24(U32 x, U32 y, char c1, char c2, U32 color)
{
	U32 i, j;
	U32 c = 0;
	U16 *p;
	U16 dat, b, bb;
	U32 xx, yy;

	p = MEM_HzLib_CHS24X24SA; //(U16 *)(HzLib_CHS24X24SAADD);//(U16 *)HzLib_CHSK24X24;//	// 使用宋体字库
	if (c1 >= 0xb0)
	{
		c = (((U32)(c1 - 0xb0)) * 94 + c2 - 0xa1) * 36;
		//c = ((U32)(c1)) * 72;
	}
	p = p + c;
	xx = x; //yy = y;

	for (i = 0; i < 12; i++)
	{
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		yy = y; //xx=x;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		p++;
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		for (j = 0; j < 8; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		xx++;	//yy++;
		yy = y; //xx=x;
		for (j = 0; j < 8; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		p++;
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		xx++; //yy++;
		p++;
	}
}
//------------------------------------------
// Function   : void DrawFHz24X24(U32 x, U32 y, char c1, char c2, U32 color)
// Description: 24*24汉字描绘(查字库) CHINESE  宋体
//------------------------------------------

void DrawFHz24X24S(U32 x, U32 y, char c1, char c2, U32 color)
{
	U32 i, j;
	U32 c = 0;
	U16 *p;
	U16 dat, b, bb;
	U32 xx, yy;

	p = MEM_HzLib_CHS24X24KA; //(U16 *)(HzLib_CHS24X24KAADD);//(U16 *)HzLib_CHSK24X24;//	// 使用楷体字库
	if (c1 >= 0xb0)
	{
		c = (((U32)(c1 - 0xb0)) * 94 + c2 - 0xa1) * 36;
		//c = ((U32)(c1)) * 72;
	}
	p = p + c;
	xx = x; //yy = y;

	for (i = 0; i < 12; i++)
	{
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		yy = y; //xx=x;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		p++;
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		for (j = 0; j < 8; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		xx++;	//yy++;
		yy = y; //xx=x;
		for (j = 0; j < 8; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		p++;
		dat = *p;
		b = ((dat & 0x00ff) << 8) | ((dat & 0xff00) >> 8);
		bb = 0x8000;
		for (j = 0; j < 16; j++)
		{
			if (b & bb)
			{
				PutPixel(xx, yy, color);
			}
			bb = bb >> 1;
			yy++; //xx++;
		}
		xx++; //yy++;
		p++;
	}
}

//------------------------------------------
// Function   : void DrawFStr(U32 x, U32 y, char *str, U32 color)
// Description: 16*16汉字及16x8字符串描绘
// note: 自动判断ASCLL与字符 ，横轴缩进为1px
//------------------------------------------
void DrawFStr(U32 x, U32 y, char *str, U32 color)
{
	U8 c1, c2;
	U8 i, uLen;
	char s[3];

	i = 0;
	uLen = 0;
	while (str[uLen] != 0)
	{
		uLen++;
	}; //探测字串长度
	while (i < uLen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位
		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFChar(x, y, s, color);
			i += 1;
			x += 9; // 下一个字符
		}
		else
		{								  // 中文
										  //			DrawHz(x, y, c1, c2, color);		// 汉字库在lcdfont_data.c
			DrawFHz(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i += 2;
			x += 17; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFStr16(U32 x, U32 y, char *str, U32 color)
// Description: 16*16汉字及16x8字符串描绘
// note: 自动判断ASCLL与字符 ，横轴缩进为0px
//------------------------------------------
void DrawF16Str(U32 x, U32 y, char *str, U32 color)
{
	U8 c1, c2;
	U8 i, uLen;
	char s[3];

	i = 0;
	uLen = 0;
	while (str[uLen] != 0)
	{
		uLen++;
	}; //探测字串长度
	while (i < uLen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位
		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFChar(x, y, s, color);
			i += 1;
			x += 8; // 下一个字符
		}
		else
		{								  // 中文
										  //			DrawHz(x, y, c1, c2, color);		// 汉字库在lcdfont_data.c
			DrawFHz(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i += 2;
			x += 16; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void Draw24X24FStr(U32 x, U32 y, char *str, U32 color)
// Description: 24*24汉字及24x12字符串描绘   中文为楷体
// note: 自动判断ASCLL与字符 ，横轴缩进为2px
//------------------------------------------
U16 Draw24X24FStr(U32 x, U32 y, char *str, U32 color)
{
	U8 c1, c2;
	U8 i, uLen;
	U16 reslen = 0;
	char s[3];

	i = 0;
	uLen = 0;
	while (str[uLen] != 0)
	{
		uLen++;
	}; //探测字串长度
	while (i < uLen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位

		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFASC12(x, y, s, color);
			i += 1;
			x += 13; // 下一个字符
			reslen++;
		}
		else
		{									   // 中文
			DrawFHz24X24(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i += 2;
			x += 26; // 下一个字符
			reslen += 1;
		}
	}

	wdt();
	return reslen;
}

//------------------------------------------
// Function   : void Draw32X32FStr(U32 x, U32 y, char *str, U32 color)
// Description: 24*24汉字及32x16字符串描绘
//------------------------------------------
void Draw32X32FStr(U32 x, U32 y, char *str, U32 color)
{
	U8 c1, c2;
	U8 i, uLen;
	char s[3];

	i = 0;
	uLen = 0;
	while (str[uLen] != 0)
	{
		uLen++;
	}; //探测字串长度
	while (i < uLen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位
		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFASC3216(x, y, s, color);
			i += 1;
			x += 20; // 下一个字符
		}
		else
		{									   // 中文
			DrawFHz24X24(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			//DrawFHz24X24S(x, y+32, c1, c2, color);		// 将汉字库写入FLASH后使用
			i += 2;
			x += 40; // 下一个字符
		}
	}
}
//------------------------------------------
// Function   : void DrawFStr(U32 x, U32 y, char *str, U16 color)
// Description: 32*32汉字及32x16字符串描绘
// note: 无32*32字库   重映射到 Draw24X24FStr
//------------------------------------------
void DrawFStr32(U32 x, U32 y, char *str, U32 color)
{
	//	U8  c1;
	//	U8 c2;
	//	U8  i, uLen;
	//	char s[3];

	//	i = 0; uLen = 0;
	//	while (str[uLen]!=0) {uLen++;};	//探测字串长度
	//	while (i<uLen){
	//		c1 = str[i];				// 汉字低八位
	//		c2 = str[i+1];				// 汉字高八位
	//		if (c1 <= 128) 				// ASCII
	//		{
	//			s[0] = c1; s[1] = 0;
	//			DrawFChar32(x, y, s, color);
	//			i += 1;
	//			x += 16;				// 下一个字符
	//		} else {					// 中文
	//			DrawHz32(x, y, c1, c2, color);		// 汉字库在lcdfont_data.c
	//			i += 2;
	//			x += 32;				// 下一个字符
	//		}
	//	}
	Draw24X24FStr(x, y, str, color);
}

//------------------------------------------
// Function   : void Draw24x24StrSinLineLimt(U16 x,U16 y,U16 p)
// Description: FLASH中的字符显示，一行显示 24x12字体,有显示字符长度限制
// U16 x,y :显示起点坐标
//U16 SheetNO:多国语言Sheet表页码 从0开始.
// U16 p:Sheet表行数
// U8 linemus :每行显示字符个数
// U8 maxline:最多显示多少行，后面数据不现实 为0时自动显示全部字符
// return : 显示行数
//------------------------------------------
void Draw24x24StrSinLineLimt(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U32 color)
{
	if (lmax == 0)
		return;
	if (P_LAN < 2)
	{ //中英文
		DrawFMultiLStr24LimtL(x, y, p, lmax, 1, color);
	}
	else
	{ //其他国语言
		DrawUnicodeMulStr(x, y, SheetNO, lines, lmax, 1, color);
	}
}

//------------------------------------------
// Function   : DrawCen24x24StrSinLineLimt(U16 x,U16 y,U16 *p,U16 SheetNO,U16 lines,U8 lmax,U32 color)
// Description: FLASH中的字符显示，一行 居中 显示 24x12字体,有显示字符长度限制
// note: 可换行显示
//------------------------------------------
void DrawCen24x24StrSinLineLimt(U16 x, U16 y, U16 dx, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U32 color)
{
	U8 *addr;
	U32 stroffest;
	U16 strlen = 0;
	U16 vx = 0, vdx = 0;
	if (lmax == 0)
		return;
	//探索显示字符长度
	if (P_LAN < 2)
	{ //中英文
		addr = (U8 *)p;
		while ((*addr) != 0)
		{
			addr++;
			strlen++;
		}
	}
	else
	{ //其他国语言
		if (CountUnicodeInfo(MEM_MULTILINGUA, SheetNO, lines, P_LAN - 2, &stroffest, &strlen))
		{
		}
		else
		{
			return;
		}
	}
	if (strlen > lmax)
		strlen = lmax;
	vx = strlen * 13;
	if (vx > dx)
		vdx = 0;
	else
		vdx = (dx - vx) / 2;

	if (P_LAN < 2)
	{ //中英文
		DrawFMultiLStr24LimtL(x + vdx, y, p, lmax, 1, color);
	}
	else
	{ //其他国语言
		DrawUnicodeMulStr(x + vdx, y, SheetNO, lines, lmax, 1, color);
	}
}

//------------------------------------------
// Function   : void Draw24X24FStrByLine(U32 x, U32 y, char *str, U32 color)
// Description: 24*24汉字及24x12字符串描绘 行指定最大字符数
// note: 可换行显示
//------------------------------------------
void Draw24X24FStrByLine(U32 x, U32 y, char *str, U8 maxchar, U8 rows, U32 color)
{
	U8 c1, c2;
	U8 i, uLen;
	U8 charnum = 0;
	U32 xpos = x;
	char s[3];
	U8 trows = 1;

	if (maxchar == 0)
		maxchar = 50;
	i = 0;
	uLen = 0;

	while (str[uLen] != 0)
	{
		uLen++;
	}; //探测字串长度
	while (i < uLen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位

		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			if (charnum >= maxchar)
			{
				y += 28;
				x = xpos;
				charnum = 0;
				trows++;
				if (rows && trows > rows)
					break;
			}
			DrawFASC12(x, y, s, color);
			i += 1;
			x += 13; // 下一个字符
			charnum++;
		}
		else
		{ // 中文
			charnum += 2;
			if (charnum >= maxchar)
			{
				y += 28;
				x = xpos;
				charnum = 0;
				trows++;
				if (rows && trows > rows)
					break;
			}
			DrawFHz24X24(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i += 2;
			x += 26; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void Draw16x16StrLineLimt(U16 x,U16 y,U16 p)
// Description: FLASH中的字符显示，自动换行，显示 16x16,有显示字符长度限制
//------------------------------------------
void Draw16x16StrLineLimt(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U8 mlines, U32 color)
{
	if (lmax == 0)
		return;
	if (P_LAN < 2)
	{ //中英文
		Draw16X16FStrByLine(x, y, (char *)p, lmax, mlines, color);
	}
	else
	{ //其他国语言
		DrawUnicodeMulStr1(x, y, SheetNO, lines, lmax, mlines, color);
	}
}
//------------------------------------------
// Function   : void Draw24X24FStrByLine(U32 x, U32 y, char *str, U32 color)
// Description: 16*16汉字及16x8字符串描绘 行指定最大字符数
//------------------------------------------
void Draw16X16FStrByLine(U32 x, U32 y, char *str, U8 maxchar, U8 lines, U32 color)
{
	U8 c1, c2;
	U8 i, uLen;
	U8 charnum = 0;
	U32 xpos = x;
	char s[3];
	i = 0;
	uLen = 0;

	while (str[uLen] != 0)
	{
		uLen++;
	}; //探测字串长度
	if (lines)
	{ //设定显示行数
		if (uLen > maxchar * lines)
		{
			uLen = maxchar * lines;
		}
	}
	while (i < uLen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位

		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			if (charnum >= maxchar)
			{
				y += 18;
				x = xpos;
				charnum = 0;
			}
			DrawFChar(x, y, s, color);
			i += 1;
			x += 8; // 下一个字符
			charnum++;
		}
		else
		{ // 中文
			charnum += 2;
			if (charnum >= maxchar)
			{
				y += 18;
				x = xpos;
				charnum = 0;
			}
			DrawFHz(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i += 2;
			x += 16; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFFStr(U32 x, U32 y, U16 *p, U32 color)
// Description: FLASH中的16*16汉字及16x8字符串描绘
// note : Draw16x16FFStr
//------------------------------------------
void DrawFFStr(U32 x, U32 y, U16 *p, U32 color)
{
	U16 c;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			DrawFHz(x, y, c3, c1, color);
			x += 16;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				DrawFChar(x, y, s, color);
				x += 8;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
			}
			DrawFChar(x, y, s, color);
			if (s[1] > 0)
				x += 16;
			else
				x += 8;
			i++;
		}
		else
		{								  // 中文
			DrawFHz(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			x += 16; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFFStr(U32 x, U32 y, U16 *p, U32 color)
// Description: FLASH中的16*16汉字及16x8字符串描绘，带有长度限制
// note : Draw16x16FFStrN
//------------------------------------------
void DrawFFStrN(U32 x, U32 y, U16 *p, U32 n, U32 color)
{
	U16 c;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;
	if (uLen > n)
		uLen = n;
	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			DrawFHz(x, y, c3, c1, color);
			x += 16;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				DrawFChar(x, y, s, color);
				x += 8;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
			}
			DrawFChar(x, y, s, color);
			if (s[1] > 0)
				x += 16;
			else
				x += 8;
			i++;
		}
		else
		{								  // 中文
			DrawFHz(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			x += 16; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void Draw24X24FFStr(U32 x, U32 y, U16 *p, U32 color)
// Description: 24*24汉字及24x12字符串描绘
//------------------------------------------
U16 Draw24X24FFStr(U32 x, U32 y, U16 *p, U32 color)
{
	U16 c;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen;
	U16 reslen = 0;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			DrawFHz24X24(x, y, c3, c1, color);
			x += 26;
			reslen += 2;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				DrawFASC12(x, y, s, color);
				x += 13;
				reslen++;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
			}
			DrawFASC12(x, y, s, color);
			if (s[1] > 0)
			{
				x += 26;
				reslen += 2;
			}
			else
			{
				x += 13;
				reslen++;
			}
			i++;
		}
		else
		{									   // 中文
			DrawFHz24X24(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			x += 26; // 下一个字符
			reslen += 2;
		}
	}

	wdt();
	return reslen;
}

//------------------------------------------
// Function   : void Draw24X24FFStrN(U32 x, U32 y, U16 *p, U32 n, U32 color)
// Description: 24*24汉字及24x12字符串描绘，指针数据为16位
//------------------------------------------
void Draw24X24FFStrN(U32 x, U32 y, U16 *p, U32 n, U32 color)
{
	U16 c;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;
	if (uLen > n)
		uLen = n;
	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			DrawFHz24X24(x, y, c3, c1, color);
			x += 26;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				DrawFASC12(x, y, s, color);
				x += 13;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
			}
			DrawFASC12(x, y, s, color);
			if (s[1] > 0)
				x += 26;
			else
				x += 13;
			i++;
		}
		else
		{									   // 中文
			DrawFHz24X24(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			x += 26; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void Draw24X24FPY(U32 x, U32 y, char *str, U8 ulen, U32 color)
// Description: 24*24汉字及24x12字符串描绘,只显示ulen限制的数量字符，指针数据为8位
//------------------------------------------
void Draw24X24FPY(U32 x, U32 y, char *str, U8 ulen, U32 color)
{
	U8 c1, c2;
	U8 i;
	char s[3];

	i = 0;

	while (i < ulen)
	{
		c1 = str[i];	 // 汉字低八位
		c2 = str[i + 1]; // 汉字高八位
		if (c1 == 0)
		{
			i += 1;
		}
		else if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFASC12(x, y, s, color);
			i += 1;
			x += 13; // 下一个字符
		}
		else
		{									   // 中文
			DrawFHz24X24(x, y, c1, c2, color); // 将汉字库写入FLASH后使用
			//DrawFHz24X24S(x, y+32, c1, c2, color);		// 将汉字库写入FLASH后使用
			i += 2;
			x += 26; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH中的16*16汉字及16x8字符串描绘  多行
//------------------------------------------
void DrawFMultiLStr(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color)
{
	U16 c, xx, yy;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen, temp_Len;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	xx = x;
	yy = y;
	temp_Len = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	if (line)
	{
		if (uLen > (line * num / 2))
			uLen = line * num / 2;
	}
	while (i < uLen)
	{
		wdt();
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 16;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c3, c1, color);
			xx += 16;
			temp_Len += 2;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 16;
					temp_Len = 0;
				}
				DrawFChar(xx, yy, s, color);
				xx += 8;
				temp_Len++;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 16;
					temp_Len = 0;
				}
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
				if ((temp_Len + 2) > num)
				{
					xx = x;
					yy += 16;
					temp_Len = 0;
				}
			}
			DrawFChar(xx, yy, s, color);
			if (s[1] > 0)
			{
				xx += 16;
				temp_Len += 2;
			}
			else
			{
				xx += 8;
				temp_Len++;
			}
			i++;
		}
		else
		{ // 中文
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 16;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			xx += 16;
			temp_Len += 2; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr16(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH中的16*16汉字及16x8字符串描绘  多行
// return :返回实际显示的行数
//------------------------------------------
U16 DrawFMultiLStr16(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color)
{
	U16 c, xx, yy;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen, temp_Len;
	char s[3];
	U16 vlines = 1;

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	xx = x;
	yy = y;
	temp_Len = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	if (line)
	{
		if (uLen > (line * num / 2))
			uLen = line * num / 2;
	}

	while (i < uLen)
	{
		wdt();
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 18;
				vlines++;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c3, c1, color);
			xx += 18;
			temp_Len += 2;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 18;
					vlines++;
					temp_Len = 0;
				}
				DrawFChar(xx, yy, s, color);
				xx += 9;
				temp_Len++;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 28;
					vlines++;
					temp_Len = 0;
				}
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
				if ((temp_Len + 2) > num)
				{
					xx = x;
					yy += 28;
					vlines++;
					temp_Len = 0;
				}
			}
			DrawFChar(xx, yy, s, color);
			if (s[1] > 0)
			{
				xx += 18;
				temp_Len += 2;
			}
			else
			{
				xx += 9;
				temp_Len++;
			}
			i++;
		}
		else
		{ // 中文
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 18;
				vlines++;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			xx += 18;
			temp_Len += 2; // 下一个字符
		}
	}

	return vlines;
}

//------------------------------------------
// Function   : void DrawFMultiLStrR(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH中的16*16汉字及16x8字符串描绘  多行(右对齐)
//------------------------------------------
void DrawFMultiLStrR(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color)
{
	U16 c, xx, yy;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen, temp_Len, temp_Len1;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	temp_Len = 0;
	temp_Len1 = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		temp_Len1 += 2;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
	{
		uLen++;
		temp_Len1++;
	}
	//printf("0 temp_Len1 = %d \r\n",temp_Len1);
	if (line)
	{
		if (uLen > (line * num / 2))
			uLen = line * num / 2;
		if (temp_Len1 > (line * num))
			temp_Len1 = line * num;
	}
	//printf("temp_Len1 = %d \r\n",temp_Len1);
	x = x - temp_Len1 * 8;
	xx = x;
	yy = y;
	while (i < uLen)
	{
		wdt();
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 16;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c3, c1, color);
			xx += 16;
			temp_Len += 2;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 16;
					temp_Len = 0;
				}
				DrawFChar(xx, yy, s, color);
				xx += 8;
				temp_Len++;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 16;
					temp_Len = 0;
				}
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
				if ((temp_Len + 2) > num)
				{
					xx = x;
					yy += 16;
					temp_Len = 0;
				}
			}
			DrawFChar(xx, yy, s, color);
			if (s[1] > 0)
			{
				xx += 16;
				temp_Len += 2;
			}
			else
			{
				xx += 8;
				temp_Len++;
			}
			i++;
		}
		else
		{ // 中文
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 16;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			xx += 16;
			temp_Len += 2; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr24(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH中的24*24汉字及24x12字符串描绘  多行 有行数限定
//------------------------------------------
void DrawFMultiLStr24LimtL(U32 x, U32 y, U16 *p, U8 num, U8 mline, U32 color)
{
	U16 c, xx, yy;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen, temp_Len;
	char s[3];

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	xx = x;
	yy = y;
	temp_Len = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;
	if (mline)
	{
		if (uLen > (mline * num / 2))
			uLen = mline * num / 2;
	}
	while (i < uLen)
	{
		wdt();
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 28;
				temp_Len = 0;
			}
			DrawFHz24X24(xx, yy, c3, c1, color);
			xx += 26;
			temp_Len += 2;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 28;
					temp_Len = 0;
				}
				DrawFASC12(xx, yy, s, color);
				xx += 13;
				temp_Len++;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 28;
					temp_Len = 0;
				}
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
				if ((temp_Len + 2) > num)
				{
					xx = x;
					yy += 28;
					temp_Len = 0;
				}
			}
			DrawFASC12(xx, yy, s, color);
			if (s[1] > 0)
			{
				xx += 26;
				temp_Len += 2;
			}
			else
			{
				xx += 13;
				temp_Len++;
			}
			i++;
		}
		else
		{ // 中文
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 28;
				temp_Len = 0;
			}
			DrawFHz24X24(xx, yy, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			xx += 26;
			temp_Len += 2; // 下一个字符
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr24(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH中的24*24汉字及24x12字符串描绘  多行
// return :返回实际显示的行数
//------------------------------------------
U16 DrawFMultiLStr24(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color)
{
	U16 c, xx, yy;
	U16 *add;
	U8 c1, c2, c3;
	U8 i, uLen, temp_Len;
	char s[3];
	U16 vlines = 1;

	add = p;
	c = (*add);
	c1 = c & 0xff;
	c2 = (c >> 8) & 0xff;
	c3 = 0;
	i = 0;
	uLen = 0;
	xx = x;
	yy = y;
	temp_Len = 0;
	while ((c1 != 0) && (c2 != 0))
	{
		uLen++;
		c = *(add + uLen);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
	} //探测字串长度
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	if (line)
	{
		if (uLen > (line * num / 2))
			uLen = line * num / 2;
	}

	while (i < uLen)
	{
		wdt();
		c = *(add + i);
		c1 = c & 0xff;		  // 汉字低八位
		c2 = (c >> 8) & 0xff; // 汉字高八位
		if (c3 > 0)
		{
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 28;
				vlines++;
				temp_Len = 0;
			}
			DrawFHz24X24(xx, yy, c3, c1, color);
			xx += 26;
			temp_Len += 2;
			if (c2 > 128)
			{
				c3 = c2;
			}
			else if (c2 > 0)
			{
				s[0] = c2;
				s[1] = 0;
				c3 = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 28;
					vlines++;
					temp_Len = 0;
				}
				DrawFASC12(xx, yy, s, color);
				xx += 13;
				temp_Len++;
			}
			i++;
		}
		else if (c1 <= 128)
		{ // ASCII
			if ((c2 == 0) || (c2 > 128))
			{
				c3 = c2;
				s[0] = c1;
				s[1] = 0;
				if ((temp_Len + 1) > num)
				{
					xx = x;
					yy += 28;
					vlines++;
					temp_Len = 0;
				}
			}
			else
			{
				s[0] = c1;
				s[1] = c2;
				s[2] = 0;
				if ((temp_Len + 2) > num)
				{
					xx = x;
					yy += 28;
					vlines++;
					temp_Len = 0;
				}
			}
			DrawFASC12(xx, yy, s, color);
			if (s[1] > 0)
			{
				xx += 26;
				temp_Len += 2;
			}
			else
			{
				xx += 13;
				temp_Len++;
			}
			i++;
		}
		else
		{ // 中文
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 28;
				vlines++;
				temp_Len = 0;
			}
			DrawFHz24X24(xx, yy, c1, c2, color); // 将汉字库写入FLASH后使用
			i++;
			xx += 26;
			temp_Len += 2; // 下一个字符
		}
	}

	return vlines;
}

//------------------------------------------
// Function   : void SetLanguage(U16 pn)
// Description: 设置选择的语言
//------------------------------------------
void SetLanguage(U16 pn)
{
	U16 num = 0;
	g_lang = pn;

	switch (pn)
	{
	case MENU_P1: //2
		SheetPage = 0;
		num = 41;
		break;
	case MENU_P2: //3
		SheetPage = 0;
		num = 42;
		break;
	case MENU_READ: //		5
		SheetPage = 6;
		num = 46;
		break;
	case MENU_WRITE: //		6
		SheetPage = 7;
		num = 47;
		break;
	case MENU_WRITE1: //		61
		SheetPage = 39;
		num = 79;
		break;
	case MENU_INPUT: //		7
		SheetPage = 9;
		num = 49;
		break;
	case MENU_CURSOR: //		71
		SheetPage = 10;
		num = 50;
		break;
	case MENU_INMENU: //		72
		SheetPage = 11;
		num = 51;
		break;
	case MENU_PITCH: //		73
		SheetPage = 14;
		num = 54;
		break;
	case MENU_PROG: //		10
		SheetPage = 28;
		num = 68;
		break;
	case MENU_PROGMSG11: //		10
		SheetPage = 43;
		num = 17;
		break;
	case MENU_METHOD: //		76
		SheetPage = 12;
		num = 52;
		break;
	case MENU_MULTI: //		77
		SheetPage = 15;
		num = 55;
		break;
	case MENU_ZIGZAG: //		78
		SheetPage = 16;
		num = 56;
		break;
	case MENU_TACKING: //		79
		SheetPage = 17;
		num = 57;
		break;
	case MENU_CODE: //		710
		SheetPage = 13;
		num = 53;
		break;
	case MENU_EXCODE:  //		711
	case MENU_EXCODE1: //	714
		SheetPage = 13;
		num = 53;
		break;
	case MENU_INHELP: //		74
		SheetPage = 27;
		num = 67;
		break;
	case MENU_MOIMAGE: //		712
		SheetPage = 26;
		num = 66;
		break;
	case MENU_MODIFY: //		8
		SheetPage = 20;
		num = 60;
		break;
	case MENU_MODIFYADD: //		81
		SheetPage = 19;
		num = 59;
		break;
	case MENU_MODIFY1: //		82
		SheetPage = 21;
		num = 61;
		break;
	case MENU_MODIFY2: //		83
		SheetPage = 22;
		num = 62;
		break;
	case MENU_MODIFY3: //		84
		SheetPage = 23;
		num = 63;
		break;
	case MENU_PARAMODIFY: //		85
		SheetPage = 37;
		num = 77;
		break;

	case MENU_CONVERT: //		9
		SheetPage = 24;
		num = 64;
		break;
	case MENU_FTNETCIG: //		11120
		SheetPage = 40;
		num = 80;
		break;
	case MENU_CONVERT1: //		913
		SheetPage = 25;
		num = 65;
		break;

	case MENU_SKIPJOG: //		75
		SheetPage = 18;
		num = 58;
		break;
	case MENU_LINEFORCE: //		22
		SheetPage = 33;
		num = 73;
		break;
	case MENU_PFOOT: //		23
		SheetPage = 32;
		num = 72;
		break;
	case MENU_MENU: //4
		SheetPage = 0;
		num = 40;
		break;
	case MENU_WIND: //		32
		SheetPage = 3;
		num = 43;
		break;
	case MENU_THREAD: //15
		SheetPage = 0;
		num = 43; //80
		break;
	case MENU_COUNTER: //		31
		SheetPage = 4;
		num = 44;
		break;

	case MENU_HELP: //13
		SheetPage = 31;
		num = 71;
		break;
	case MENU_RWHELP: //		14
		SheetPage = 8;
		num = 48;
		break;
	case MENU_FUNCTION: //		11
		SheetPage = 30;
		num = 70;
		break;
	case MENU_IOTEST: //		12
		SheetPage = 29;
		num = 69;
		break;
	default:
		SheetPage = 0;
		num = 41;
		break;
	}

	if (P_LAN)
	{ // 英文菜单
		MEM_TITLE_ADD = ((U16 *)(tmpflashadd[num + 50]));
		TitleLengthMax = TIT_MAXLENGTH_E;
	}
	else
	{ // 中文菜单
		MEM_TITLE_ADD = ((U16 *)(tmpflashadd[num]));
		TitleLengthMax = TIT_MAXLENGTH;
	}

	MEM_TITLE_AUDIO_ADD = ((U16 *)(tmpflashadd[num]));
	TitleAudioLengthMax = TIT_MAXLENGTH;
}

//------------------------------------------
// Function   : void getLanguage()
// Description: 获取选择的语言
//------------------------------------------
U16 getLanguage(void)
{
	return g_lang;
}

//------------------------------------------
//Function   : U8 CountUnicodeInfo(U16 *addr,U16 Sheetcnt,U16 linenum,U16 langsnum)
//Description: 计算显示的字符串信息
//U16 *addr:查询地址
//U16 Sheetcnt:输入查询表编号  从0开始
//U16 linenum:输入查询表行数 从0开始
//U16 langsnum:输入查询表语言编号 从0开始
//U32 *outoffest:输出显示字符串记录Unicode值的起始地址偏移值
//U16 *outstrlen:显示字符串长度
//返回值 ： 1成功/0失败
//------------------------------------------
U8 CountUnicodeInfo(U16 *addr, U16 Sheetcnt, U16 linenum, U16 langsnum, U32 *outoffest, U16 *outstrlen)
{
	//	U32 len;
	U32 i;
	U16 LangTol, SheetTol;
	U32 FirstIndexAddr; //一级检索表起始地址
	U32 SecondIndexAdd; //二级检索表起始地址
	U32 StringStartAdd; //二级检索表检索到的字符串起始地址
	U32 StringAdd;		//显示的字符数据起始地址
	U16 Stringlen;		//显示字符串长度
	U16 Offset[TolLangs] = {0};
	//	U8 *p;
	//	U8 res;
	//	U16 StrCnt;

	if (*addr != 0x5246) //	格式错误
		return 0;

	//	len = (*(addr+3)<<16)| *(addr+2);
	//	p = (U8*)addr;

	SheetTol = *(addr + 4); //表格数
	LangTol = *(addr + 5);	//语音种类数
	if (Sheetcnt >= SheetTol)//查找表号超出范围
	{ 
		return 0;
	}
	if (LangTol > TolLangs)//超过支持的语言种类数
	{ 
		return 0;
	}
	if (langsnum >= LangTol)//查找语言编号超出范围
	{ 
		return 0;
	}
	FirstIndexAddr = 16 + 6 * LangTol;

	//	p = (U8*)addr;
	//	StrCnt = *(addr+(FirstIndexAddr+22*Sheetcnt+16)/2);
	SecondIndexAdd = (*(addr + (FirstIndexAddr + 22 * Sheetcnt + 20) / 2) << 16) | *(addr + (FirstIndexAddr + 22 * Sheetcnt + 18) / 2);
	StringStartAdd = (*(addr + (SecondIndexAdd + 60 + 2 + linenum * (60 + 4 + 2 * LangTol)) / 2) << 16) | *(addr + (SecondIndexAdd + 60 + linenum * (60 + 4 + 2 * LangTol)) / 2);

	for (i = 0; i <= LangTol; i++)
	{
		Offset[i] = *(addr + (SecondIndexAdd + 64 + 2 * i + linenum * (60 + 4 + 2 * LangTol)) / 2);
	}

	if (langsnum == 0)
	{
		StringAdd = StringStartAdd;
		Stringlen = Offset[langsnum];
	}
	else
	{
		StringAdd = StringStartAdd + Offset[langsnum - 1];
		Stringlen = Offset[langsnum] - Offset[langsnum - 1];
	}

	StringAdd /= 2; //转为16位
	Stringlen /= 2;

	*outoffest = StringAdd;
	*outstrlen = Stringlen;

	return 1;
}

//------------------------------------------
//Function   : void DrawUnicodeStr(U16 x,U16 y,U16 *p,U16 len,U32 color,U8 Font)
//Description: 显示Unicode编码字体
//U16 x,y : 显示xy起点坐标
//U16 *p：显示字符串起始地址
//U16 len:显示长度
//U32 color:显示颜色
//------------------------------------------
void DrawUnicodeStr24x24(U16 x, U16 y, U16 *p, U16 len, U32 color)
{
	U16 *U16_addr;
	U8 *U8_addr;
	U16 i, j, strnum;
	U32 addr, offset;

	U8 dat, n;
	U16 xx = x, yy = y;
	U8 FontHeight, FontWith;
	U16 base;
	U8 ratio;

	FontHeight = 24;
	FontWith = 24;
	ratio = 2;
	//	#if Debug
	//	for(strnum=0;strnum<len;strnum++)
	//		printf("*(p+%d) = %04x \r\n",strnum,*(p+strnum));
	//	#endif
	for (strnum = 0; strnum < len; strnum++)
	{
		if ((*(p + strnum) > Unicode0_Min) && (*(p + strnum) <= Unicode0_Max))
		{
			U16_addr = MEM_FONT_0;
			base = Unicode0_Min;
		}
		else if ((*(p + strnum) >= Unicode1_Min) && (*(p + strnum) <= Unicode1_Max))
		{
			U16_addr = MEM_FONT_1;
			base = Unicode1_Min;
		}
		else if ((*(p + strnum) >= Unicode2_Min) && (*(p + strnum) <= Unicode2_Max))
		{
			U16_addr = MEM_FONT_2;
			base = Unicode2_Min;
		}
		else
		{ //不支持的字体
			return;
		}
		U8_addr = (U8 *)U16_addr;

		addr = (*(p + strnum) - base) * FontHeight * FontWith / 8;
		offset = 0;
		for (i = 0; i < FontHeight; i++)
		{
			for (j = 0; j < FontWith / 8; j++)
			{
				dat = *(U8_addr + addr + offset);
				for (n = 0; n < 8; n++)
				{
					if (dat & 0x80)
						PutPixel(xx, yy, color);
					dat = dat << 1;
					xx++;
				}
				offset++;
			}
			xx = x;
			yy++;
		}
		x += FontWith / ratio + 1;
		xx = x;
		yy = y;
	}
}

//------------------------------------------
//Function   : void DrawUnicodeMulStr24x24(U16 x,U16 y,U16 *p,U16 len,U8 linemus,U8 maxline,U32 color)
//Description: 多行显示Unicode编码字体
//U16 x,y : 显示xy起点坐标
//U16 *p：显示字符串起始地址
//U16 len:显示长度
// U8 linemus :每行显示字符个数
// U8 maxline:最多显示多少行，后面数据不现实 为0时自动显示全部字符
//U32 color:显示颜色
// return:实际显示行数
//------------------------------------------
U16 DrawUnicodeMulStr24x24(U16 x, U16 y, U16 *p, U16 len, U8 linemus, U8 maxline, U32 color)
{
	U16 *U16_addr;
	U8 *U8_addr;
	U16 i, j, strnum;
	U32 addr, offset;

	U8 dat, n;
	U16 xx = x, yy = y, stx = x, sty = y;
	U8 FontHeight, FontWith;
	U16 base;
	U8 ratio;
	U16 vlines = 1;
	U8 flag = 0;
	FontHeight = 24;
	FontWith = 24;
	ratio = 2;

	if (maxline)
	{
		if (len > (linemus * maxline))
			len = linemus * maxline;
	}
	for (strnum = 0; strnum < len; strnum++)
	{
		if ((*(p + strnum) > Unicode0_Min) && (*(p + strnum) <= Unicode0_Max))
		{
			U16_addr = MEM_FONT_0;
			base = Unicode0_Min;
		}
		else if ((*(p + strnum) >= Unicode1_Min) && (*(p + strnum) <= Unicode1_Max))
		{
			U16_addr = MEM_FONT_1;
			base = Unicode1_Min;
		}
		else if ((*(p + strnum) >= Unicode2_Min) && (*(p + strnum) <= Unicode2_Max))
		{
			U16_addr = MEM_FONT_2;
			base = Unicode2_Min;
		}
		else
		{ //不支持的字体
			return flag ? vlines : 0;
		}
		U8_addr = (U8 *)U16_addr;

		addr = (*(p + strnum) - base) * FontHeight * FontWith / 8;
		offset = 0;
		for (i = 0; i < FontHeight; i++)
		{
			for (j = 0; j < FontWith / 8; j++)
			{
				dat = *(U8_addr + addr + offset);
				for (n = 0; n < 8; n++)
				{
					if (dat & 0x80)
						PutPixel(xx, yy, color);
					dat = dat << 1;
					xx++;
				}
				offset++;
			}
			flag = 1;
			xx = x;
			yy++;
		}
		x += FontWith / ratio + 1;
		if (((strnum + 1) % linemus) == 0)
		{ //换行
			x = stx;
			y = sty + FontHeight * (strnum + 1) / linemus;
			if (strnum < len - 1)
				vlines++;
		}
		xx = x;
		yy = y;
	}

	wdt();
	return vlines;
}

//------------------------------------------
//Function   : void DrawUnicodeMulStr24x24(U16 x,U16 y,U16 *p,U16 len,U8 linemus,U8 maxline,U32 color)
//Description: 多行显示Unicode编码字体
//U16 x,y : 显示xy起点坐标
//U16 *p：显示字符串起始地址
//U16 len:显示长度
// U8 linemus :每行显示字符个数
// U8 maxline:最多显示多少行，后面数据不现实 为0时自动显示全部字符
//U32 color:显示颜色
// return:实际显示行数
//------------------------------------------
U16 DrawUnicodeMulStr16x16(U16 x, U16 y, U16 *p, U16 len, U8 linemus, U8 maxline, U32 color)
{
	U16 *U16_addr;
	U8 *U8_addr;
	U16 i, j, strnum;
	U32 addr, offset;

	U8 dat, n;
	U16 xx = x, yy = y, stx = x, sty = y;
	U8 FontHeight, FontWith;
	U16 base;
	U8 ratio;
	U16 vlines = 1;
	U8 flag = 0;
	FontHeight = 16;
	FontWith = 16;
	ratio = 2;

	if (maxline)
	{
		if (len > (linemus * maxline))
			len = linemus * maxline;
	}
	for (strnum = 0; strnum < len; strnum++)
	{
		if ((*(p + strnum) > Unicode0_Min) && (*(p + strnum) <= Unicode0_Max))
		{
			U16_addr = MEM_FONT_3;
			base = Unicode0_Min;
		}
		else if ((*(p + strnum) >= Unicode1_Min) && (*(p + strnum) <= Unicode1_Max))
		{
			U16_addr = MEM_FONT_4;
			base = Unicode1_Min;
		}
		else if ((*(p + strnum) >= Unicode2_Min) && (*(p + strnum) <= Unicode2_Max))
		{
			U16_addr = MEM_FONT_5;
			base = Unicode2_Min;
		}
		else
		{ //不支持的字体
			return flag ? vlines : 0;
		}
		U8_addr = (U8 *)U16_addr;

		addr = (*(p + strnum) - base) * FontHeight * FontWith / 8;
		offset = 0;
		for (i = 0; i < FontHeight; i++)
		{
			for (j = 0; j < FontWith / 8; j++)
			{
				dat = *(U8_addr + addr + offset);
				for (n = 0; n < 8; n++)
				{
					if (dat & 0x80)
						PutPixel(xx, yy, color);
					dat = dat << 1;
					xx++;
				}
				offset++;
			}
			flag = 1;
			xx = x;
			yy++;
		}
		x += FontWith / ratio + 1;
		if (((strnum + 1) % linemus) == 0)
		{ //换行
			x = stx;
			y = sty + FontHeight * (strnum + 1) / linemus;
			if (strnum < len - 1)
				vlines++;
		}
		xx = x;
		yy = y;
	}

	wdt();
	return vlines;
}

//------------------------------------------
// Function   : void DrawUnicodeStr(U16 x,U16 y,U16 SheetNO, U16 p,U32 color)
// Description: FLASH中的字符显示，Unicode显示字符串
// note : DrawUnicodeStr24x24
// U16 x,y :显示起点坐标
//U16 SheetNO:多国语言Sheet表页码 从0开始.
// U16 p:Sheet表行数
//------------------------------------------
U16 DrawUnicodeStr(U16 x, U16 y, U16 SheetNO, U16 p, U32 color)
{
	U16 strlen;
	U32 stroffest;
	if (CountUnicodeInfo(MEM_MULTILINGUA, SheetNO, p, P_LAN - 2, &stroffest, &strlen))
	{
		DrawUnicodeStr24x24(x, y, MEM_MULTILINGUA + stroffest, strlen, color);
	}
	wdt();
	return strlen;
}

//------------------------------------------
// Function   : void DrawUnicodeStr(U16 x,U16 y,U16 SheetNO, U16 p,U32 color)
// Description: FLASH中的字符显示，Unicode显示字符串
// note : DrawUnicodeMulStr16x16
// U16 x,y :显示起点坐标
//U16 SheetNO:多国语言Sheet表页码 从0开始.
// U16 p:Sheet表行数
//------------------------------------------
U16 DrawUnicodeStr1(U16 x, U16 y, U16 SheetNO, U16 p, U32 color)
{
	U16 strlen;
	U32 stroffest;
	if (CountUnicodeInfo(MEM_MULTILINGUA, SheetNO, p, P_LAN - 2, &stroffest, &strlen))
	{
		DrawUnicodeMulStr16x16(x, y, MEM_MULTILINGUA + stroffest, strlen, 0, 0, color);
	}
	wdt();
	return strlen;
}

//------------------------------------------
// Function   : void DrawUnicodeMulStr(U16 x,U16 y,U32 color)
// Description: FLASH中的字符显示，Unicode显示字符串 多行显示
// note : DrawUnicodeMulStr24x24
// U16 x,y :显示起点坐标
//U16 SheetNO:多国语言Sheet表页码 从0开始.
// U16 p:Sheet表行数
// U8 linemus :每行显示字符个数
// U8 maxline:最多显示多少行，后面数据不现实 为0时自动显示全部字符
//return : 显示行数
//------------------------------------------
U16 DrawUnicodeMulStr(U16 x, U16 y, U16 SheetNO, U16 p, U8 linemus, U8 maxline, U32 color)
{
	U16 strlen;
	U32 stroffest;
	if (CountUnicodeInfo(MEM_MULTILINGUA, SheetNO, p, P_LAN - 2, &stroffest, &strlen))
	{
		return DrawUnicodeMulStr24x24(x, y, MEM_MULTILINGUA + stroffest, strlen, linemus, maxline, color);
	}
	return 0;
}

//------------------------------------------
// Function   : void DrawUnicodeMulStr(U16 x,U16 y,U32 color)
// Description: FLASH中的字符显示，Unicode显示字符串 多行显示
// note : DrawUnicodeMulStr16x16
// U16 x,y :显示起点坐标
//U16 SheetNO:多国语言Sheet表页码 从0开始.
// U16 p:Sheet表行数
// U8 linemus :每行显示字符个数
// U8 maxline:最多显示多少行，后面数据不现实 为0时自动显示全部字符
//return : 显示行数
//------------------------------------------
U16 DrawUnicodeMulStr1(U16 x, U16 y, U16 SheetNO, U16 p, U8 linemus, U8 maxline, U32 color)
{
	U16 strlen;
	U32 stroffest;
	if (CountUnicodeInfo(MEM_MULTILINGUA, SheetNO, p, P_LAN - 2, &stroffest, &strlen))
	{
		return DrawUnicodeMulStr16x16(x, y, MEM_MULTILINGUA + stroffest, strlen, linemus, maxline, color);
	}
	return 0;
}

//------------------------------------------
// Function   : void Draw16x16Str(U16 x,U16 y,U16 p)
// Description: FLASH中的字符显示，文字 16x8字体
//------------------------------------------
void Draw16x16Str(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U32 color)
{
	if (P_LAN < 2)
	{ //中英文
		DrawFFStr(x, y, p, color);
	}
	else
	{ //其他国语言
		//DrawUnicodeStr(x,y,SheetNO,lines,color);
		//DrawFFStr(x,y,p,color); //越南语字库没有 先用英语替代
		DrawUnicodeStr1(x, y, SheetNO, lines, color);
	}
}

//------------------------------------------
// Function   : void Draw24x24Str(U16 x,U16 y,U16 p)
// Description: FLASH中的字符显示，文字 24x12字体
//return: 实际显示字符数
//------------------------------------------
U16 Draw24x24Str(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U32 color)
{
	if (P_LAN < 2)
	{ //中英文
		return Draw24X24FFStr(x, y, p, color);
	}
	else
	{ //其他国语言
		return DrawUnicodeStr(x, y, SheetNO, lines, color);
	}
}

//------------------------------------------
// Function   : void DrawFMenuStr1(U16 x,U16 y,U16 p)
// Description: FLASH中的字符显示，多行显示 24x12字体
// U16 x,U16 y - 显示坐标起点
// U16 *p - 显示字符首地址(中英文)
// U8 linemus - 每行最大显示字符数
// U16 SheetNO - 多国语言对应标号(其他国语言)
// U16 lines - Sheet表行数 (其他国语言)
// U8 maxline -最大显示几行 (其他国语言)
// return: 实际显示行数
//------------------------------------------
U16 Draw24x24MulStr(U16 x, U16 y, U16 *p, U8 linemus, U16 SheetNO, U16 lines, U8 maxline, U32 color)
{
	if (P_LAN < 2)
	{ //中英文
		return DrawFMultiLStr24(x, y, p, linemus, maxline, color);
	}
	else
	{ //其他国语言
		return DrawUnicodeMulStr(x, y, SheetNO, lines, linemus, maxline, color);
	}
}

//------------------------------------------
// Function   : void DrawUnicodeFMenuStr(U16 x,U16 y,U32 color)
// Description: 菜单功能Unicode显示专用(相对地址)
//------------------------------------------
void DrawUnicodeFMenuStr(U16 x, U16 y, U16 p, U32 color)
{
	U16 strlen;
	U32 stroffest;
	if (CountUnicodeInfo(MEM_MULTILINGUA, SheetPage, p, P_LAN - 2, &stroffest, &strlen))
	{
		DrawUnicodeStr24x24(x, y, MEM_MULTILINGUA + stroffest, strlen, color);
	}
}

//------------------------------------------
// Function   : void DrawFMenuStr(U16 p)
// Description: 菜单功能显示专用(固定坐标)(相对地址)
//------------------------------------------
void DrawFMenuStr(U16 p)
{
	U16 *add;
	U16 str = 0;
	U16 endpx;

	//	endpx = LCD_width;
	endpx = LCD_TIME_WIDTH_LIMIT - TITLE_XPOS_MIT;

	if (P_LAN < 2)
	{ //英文和中文
		add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
		str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
		if (P_LHP)
		{
			BmpFillRect(TITLE_XPOS_MIT, 4, endpx, 24);
			Draw24X24FFStr(TITLE_XPOS_MIT, 4, add, Color_White);
		}
	}
	else
	{ //其他国语言
		if (P_LHP)
		{
			BmpFillRect(TITLE_XPOS_MIT, 4, endpx, 24);
			DrawUnicodeFMenuStr(TITLE_XPOS_MIT, 4, p, Color_White);
		}
	}

	if ((P_VOI) && (g_vs1003err == 0))
	{
		//		if(Timerms1 > WAITTIME3){
		//			Timerms1 = 0;
		PlaySound(CMD_PLAY, (P_LAN) ? 1 : 0, str);
		//		}
	}
}

//------------------------------------------
// Function   : void DrawFMenuStr1(U16 x,U16 y,U16 p)
// Description: 菜单功能显示专用(动态坐标)(相对地址)
//------------------------------------------
void DrawFMenuStr1(U16 x, U16 y, U16 p)
{
	U16 *add;
	U16 str = 0;

	if (P_LAN < 2)
	{ //英文和中文
		if (P_LHP)
		{
			add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
			str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);

			DrawFillRect(x, y, LCD_TIME_WIDTH_LIMIT - x, TITLE_YLEN, Color_Black);
			Draw24X24FFStr(x, y, add, Color_White);
		}
	}
	else
	{ //其他国语言
		if (P_LHP)
		{
			DrawFillRect(x, y, LCD_TIME_WIDTH_LIMIT - x, TITLE_YLEN, Color_Navy);
			DrawUnicodeFMenuStr(x, y, p, Color_White);
		}
	}

	if ((P_VOI) && (g_vs1003err == 0))
	{
		//		if(Timerms1 > WAITTIME3){
		//			Timerms1 = 0;
		PlaySound(CMD_PLAY, (P_LAN) ? 1 : 0, str);
		//		}
	}
}

//------------------------------------------
// Function   : void DrawFMenuStr2(char *p)
// Description: 菜单功能显示专用(动态内容)(绝对地址)
//------------------------------------------
void DrawFMenuStr2(U16 p, U16 dx)
{
	U16 *add;
	U16 str = 0;
	if (P_LAN < 2)
	{ //英文和中文
		add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
		str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
		if (P_LHP)
		{
			BmpFillRect(TITLE_XPOS_BOR, 4, dx, 24);
			Draw24X24FFStr(TITLE_XPOS_BOR, 4, add, Color_White);
		}
	}
	else
	{ //其他国语言
		if (P_LHP)
		{
			BmpFillRect(TITLE_XPOS_BOR, 4, dx, 24);
			DrawUnicodeFMenuStr(TITLE_XPOS_BOR, 4, p, Color_White);
		}
	}

	if ((P_VOI) && (g_vs1003err == 0))
	{
		//		if(Timerms1 > WAITTIME3){
		//			Timerms1 = 0;
		PlaySound(CMD_PLAY, (P_LAN) ? 1 : 0, str);
		//		}
	}
}

//------------------------------------------
// Function   : void DrawFMenuStr2(char *p)
// Description: 菜单功能显示专用(动态内容)(绝对地址)
//------------------------------------------
void DrawFMenuStr3(U16 p, U16 x, U16 dx)
{
	U16 *add;
	U16 str = 0;
	if (P_LAN < 2)
	{ //英文和中文
		add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
		str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
		if (P_LHP)
		{
			BmpFillRect(x, 4, dx, 24);
			Draw24X24FFStr(x, 4, add, Color_White);
		}
	}
	else
	{ //其他国语言
		if (P_LHP)
		{
			BmpFillRect(x, 4, dx, 24);
			DrawUnicodeFMenuStr(x, 4, p, Color_White);
		}
	}

	if ((P_VOI) && (g_vs1003err == 0))
	{
		//		if(Timerms1 > WAITTIME3){
		//			Timerms1 = 0;
		PlaySound(CMD_PLAY, (P_LAN) ? 1 : 0, str);
		//		}
	}
}

//------------------------------------------
// Function   : void DrawFMenuStrP(U16 p)
// Description: 菜单功能显示专用
//------------------------------------------
void DrawFMenuStr3P(U16 pn, U16 p)
{
}

//------------------------------------------
// Function   : void DrawFMenuStrP(U16 p)
// Description: 菜单功能显示专用
//------------------------------------------
void DrawFMenuStrP(U16 pn, U16 p)
{


	U16 *tmpMEM_TITLE_ADD;
	U16 *tmpMEM_TITLE_AUDIO_ADD;
	U32 tmpTitleLengthMax;
	U32 tmpTitleAudioLengthMax;
	U16 tmpSheetPage;

	tmpMEM_TITLE_ADD = MEM_TITLE_ADD;
	tmpMEM_TITLE_AUDIO_ADD = MEM_TITLE_AUDIO_ADD;
	tmpTitleLengthMax = TitleLengthMax;
	tmpTitleAudioLengthMax = TitleAudioLengthMax;
	tmpSheetPage = SheetPage;

	SetLanguage(pn);
	DrawFMenuStr(p);

	MEM_TITLE_ADD = tmpMEM_TITLE_ADD;
	MEM_TITLE_AUDIO_ADD = tmpMEM_TITLE_AUDIO_ADD;
	TitleLengthMax = tmpTitleLengthMax;
	TitleAudioLengthMax = tmpTitleAudioLengthMax;
	SheetPage = tmpSheetPage;

}

//------------------------------------------
// Function   : void DrawFMenuStrP(U16 p)
// Description: 菜单功能显示专用
//------------------------------------------
void DrawFMenuStrP1(U16 x, U16 y, U16 pn, U16 p)
{
	U16 *tmpMEM_TITLE_ADD;
	U16 *tmpMEM_TITLE_AUDIO_ADD;
	U32 tmpTitleLengthMax;
	U32 tmpTitleAudioLengthMax;
	U16 tmpSheetPage;

	tmpMEM_TITLE_ADD = MEM_TITLE_ADD;
	tmpMEM_TITLE_AUDIO_ADD = MEM_TITLE_AUDIO_ADD;
	tmpTitleLengthMax = TitleLengthMax;
	tmpTitleAudioLengthMax = TitleAudioLengthMax;
	tmpSheetPage = SheetPage;

	SetLanguage(pn);
	DrawFMenuStr1(x, y, p);
	MEM_TITLE_ADD = tmpMEM_TITLE_ADD;
	MEM_TITLE_AUDIO_ADD = tmpMEM_TITLE_AUDIO_ADD;
	TitleLengthMax = tmpTitleLengthMax;
	TitleAudioLengthMax = tmpTitleAudioLengthMax;
	SheetPage = tmpSheetPage;
}

//------------------------------------------
// Function   : void DrawFMenuStrUser(U16 x,U16 y,U16 p)
// Description: 绝对坐标显示  用户自定义颜色与位置的标题栏
//------------------------------------------
void DrawFMenuStrUser(U16 x, U16 y, U16 p, U32 colorB, U32 colorT)
{
	U16 *add;
	U16 str = 0;

	if (P_LAN < 2)
	{ //英文和中文
		if (P_LHP)
		{
			add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
			str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
			DrawFillRect(x + SAFE_DIST1, y + SAFE_DIST4, TIT_TIME_XPOS - x, TITLE_YLEN - SAFE_DIST4, colorB);
			Draw24X24FFStr(x + SAFE_DIST1, y + SAFE_DIST4, add, colorT);
		}
	}
	else
	{ //其他国语言
		if (P_LHP)
		{

			DrawFillRect(x + SAFE_DIST1, y + SAFE_DIST4, TIT_TIME_XPOS - x, TITLE_YLEN - SAFE_DIST4, colorB);
			DrawUnicodeFMenuStr(x + SAFE_DIST1, y + SAFE_DIST4, p, colorT);
		}
	}

	if ((P_VOI) && (g_vs1003err == 0))
	{

		PlaySound(CMD_PLAY, (P_LAN) ? 1 : 0, str);
	}
}

//------------------------------------------
// Function   : void DrawFU32(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字,含0显示,16*8描绘,正整数，
//------------------------------------------
void DrawFU32(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0'; //倒序显示
		nd = nd / 10;
		px = x + (dig - i) * 8;
		DrawFChar(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字,不含0显示,16*8描绘,正整数
//------------------------------------------
void DrawFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0'; //倒序显示
		if ((nd == 0) && (i > 1))
			s[0] = ' ';
		nd = nd / 10;
		px = x + (dig - i) * 8;
		DrawFChar(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字,不含0显示,16*8描绘,正整数，左对齐，最大9位
//------------------------------------------
void DrawFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd, dat = 10;
	U32 px;

	for (i = 1; i < dig; i++) //对应位数最大值，4位数最大9999，则最大值位1000
		dat *= 10;

	nd = n % dat;			  //未超大小nd为原数值；超过大小，nd为超过的大小
	dat /= 10;				  //缩小一位
	for (i = 1; i < dig; i++) //获取数值的最大位
	{
		if ((nd / dat))
			break;
		dat /= 10;
	}
	px = x; //正序显示
	s[1] = 0;
	for (; i <= dig; i++)
	{
		if (dat == 0) //最大位为0
			break;
		s[0] = nd / dat + '0'; //数值的ascll码值
		nd = nd % dat;		   //剩余值
		DrawFChar(px, y, s, color);
		px += 8;
		dat /= 10;
	}
}

//------------------------------------------
// Function   : void DrawFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32位数字,含0显示,16*8描绘,整数，缩进为0
// note: 含指定位置小数点，最后一位不能有小数点，小数点占用一个dig显示位，'-'号可以小于输入的'x'
//------------------------------------------
void DrawFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
{
	char s[2];
	U32 nd;
	U32 px, pointx;

	nd = abs(n);
	px = x + (dig - 1) * 8; //移动至最后一位
	s[1] = 0;
	if (point > 0)
		pointx = px - point * 8;
	else
		pointx = XLEN;
	while (px >= x)
	{
		s[0] = nd % 10 + '0'; //倒序显示
		nd = nd / 10;
		DrawFChar(px, y, s, color);
		px -= 8;
		if (nd == 0 && px < pointx)
			break;
		if (px == pointx)
		{
			s[0] = '.';
			DrawFChar(px, y, s, color);
			px -= 8;
		}
	}
	if (n < 0)
	{
		s[0] = '-';
		DrawFChar(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32位数字,含0显示,16*16描绘,整数，缩进为0
// note: 含指定位置小数点，最后一位不能有小数点，小数点占用一个dig显示位，'-'号可以小于输入的'x'
//------------------------------------------
void DrawFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
{
	char s[2];
	U32 nd;
	U32 px, pointx;

	nd = abs(n);
	px = x + (dig - 1) * 16;
	s[1] = 0;
	if (point > 0)
		pointx = px - point * 16;
	else
		pointx = XLEN;
	while (px >= x)
	{
		s[0] = nd % 10 + '0';
		nd = nd / 10;
		DrawFStr16(px, y, s, color);
		px -= 16;
		if (nd == 0 && px < pointx)
			break;
		if (px == pointx)
		{
			s[0] = '.';
			DrawFStr16(px, y, s, color);
			px -= 16;
		}
	}
	if (n < 0)
	{
		s[0] = '-';
		DrawFStr16(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字显示16*16描绘左开始
// note: 32位数字,含0显示,16*16描绘,正整数，缩进为0//补充
//------------------------------------------
void DrawFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U32 i, nd, ndig;

	nd = 10;
	ndig = 1;
	for (i = 0; i < dig; i++)
	{
		if (n >= nd)
			ndig++;
		nd = nd * 10;
	}

	s[1] = 0;
	if (ndig > dig)
		ndig = dig;
	x = x + (ndig - 1) * 16;
	while (ndig--)
	{
		s[0] = n % 10 + '0'; //倒序显示
		n = n / 10;
		DrawFStr16(x, y, s, color);
		x -= 16;
	}
}

//------------------------------------------
// Function   : void DrawFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字,含0显示,16*16描绘,正整数，缩进为0
//------------------------------------------
void DrawFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char c, s[2];
	U8 i;
	U32 nd;
	U32 px;

	s[1] = 0;
	nd = n;
	for (i = 1; i <= dig; i++)
	{
		c = nd % 10; //倒序显示
		s[0] = c + '0';
		nd = nd / 10;
		px = x + (dig - i) * 16;
		DrawFStr16(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字,含0显示，含ABCDEF的16进制,16*8描绘,正整数，缩进为0
//------------------------------------------
void DrawFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char c, s[2];
	U8 i;
	U32 nd;
	U32 px;

	s[1] = 0;
	nd = n;
	for (i = 1; i <= dig; i++)
	{
		c = nd % 16;
		if (c < 10)
			s[0] = c + '0';
		else
			s[0] = c - 10 + 'A';
		nd = nd / 16;
		px = x + (dig - i) * 8;
		DrawFChar(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFU32(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，含0显示，24*12描绘,正整数，缩进为1
//------------------------------------------
void DrawMFU32(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0';
		nd = nd / 10;
		px = x + (dig - i) * NUMB2412S;
		DrawFASC12(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFU321(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，含0显示，24*12描绘,正整数，缩进为0
//------------------------------------------
void DrawMFU321(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0';
		nd = nd / 10;
		px = x + (dig - i) * NUMB2412S1;
		DrawFASC12(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，不含0显示，24*12描绘,正整数，缩进为1
//------------------------------------------
void DrawMFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0';
		if ((nd == 0) && (i > 1))
			s[0] = ' ';
		nd = nd / 10;
		px = x + (dig - i) * NUMB2412S;
		DrawFASC12(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字,不含0显示,24*12描绘,正整数，左对齐，最大9位，缩进为1
//------------------------------------------
void DrawMFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd, dat = 10;
	U32 px;

	for (i = 1; i < dig; i++)
		dat *= 10;
	nd = n % dat;
	dat /= 10;
	for (i = 1; i < dig; i++)
	{
		if ((nd / dat))
			break;
		dat /= 10;
	}
	px = x;
	s[1] = 0;
	for (; i <= dig; i++)
	{
		if (dat == 0)
			break;
		s[0] = nd / dat + '0';
		nd = nd % dat;
		DrawFASC12(px, y, s, color);
		px += NUMB2412S;
		dat /= 10;
	}
}

//------------------------------------------
// Function   : void DrawMFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32位数字,不含0显示,24*12描绘,整数，缩进为1
// note: 含指定位置小数点，最后一位不能有小数点，小数点占用一个dig显示位，'-'号可以小于输入的'x'
//------------------------------------------
void DrawMFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
{
	char s[2];
	U32 nd;
	U32 px, pointx;

	nd = abs(n);
	px = x + (dig - 1) * NUMB2412S;
	s[1] = 0;
	if (point > 0)
		pointx = px - point * NUMB2412S;
	else
		pointx = XLEN;
	while (px >= x)
	{
		s[0] = nd % 10 + '0';
		nd = nd / 10;
		DrawFASC12(px, y, s, color);
		px -= NUMB2412S;
		if (nd == 0 && px < pointx)
			break;
		if (px == pointx)
		{
			s[0] = '.';
			DrawFASC12(px, y, s, color);
			px -= NUMB2412S;
		}
	}
	if (n < 0)
	{
		s[0] = '-';
		DrawFASC12(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字显示，24*24描绘，左开始
//------------------------------------------
void DrawMFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U32 i, nd, ndig;

	nd = 10;
	ndig = 1;
	for (i = 0; i < dig; i++)
	{
		if (n >= nd)
			ndig++;
		nd = nd * 10;
	}

	s[1] = 0;
	if (ndig > dig)
		ndig = dig;
	x = x + (ndig - 1) * 20;
	while (ndig--)
	{
		s[0] = n % 10 + '0';
		n = n / 10;
		DrawFASC24(x, y, s, color);
		x -= 20;
	}
}
//------------------------------------------
// Function   : void DrawMFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32位数字,含0显示,24*24描绘,整数，缩进为0
// note: 含指定位置小数点，最后一位不能有小数点，小数点占用一个dig显示位，'-'号可以小于输入的'x'
//------------------------------------------
void DrawMFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
{
	char s[2];
	U32 nd;
	U32 px, pointx;

	nd = abs(n);
	px = x + (dig - 1) * 20;
	s[1] = 0;
	if (point > 0)
		pointx = px - point * 20;
	else
		pointx = XLEN;
	while (px >= x)
	{
		s[0] = nd % 10 + '0';
		nd = nd / 10;
		DrawFASC24(px, y, s, color);
		px -= 20;
		if (nd == 0 && px < pointx)
			break;
		if (px == pointx)
		{
			s[0] = '.';
			DrawFASC24(px, y, s, color);
			px -= 20;
		}
	}
	if (n < 0)
	{
		s[0] = '-';
		DrawFASC24(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，含0显示，24*24描绘，正整数
//------------------------------------------
void DrawMFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char c, s[2];
	U8 i;
	U32 nd;
	U32 px;

	s[1] = 0;
	nd = n;
	for (i = 1; i <= dig; i++)
	{
		c = nd % 10;
		s[0] = c + '0';
		nd = nd / 10;
		px = x + (dig - i) * 20;
		DrawFASC24(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawMFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，含0显示，24*12描绘，包含ABCDEF的16进制，缩进为1//左对齐
//------------------------------------------
void DrawMFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char c, s[2];
	U8 i;
	U32 nd;
	U32 px;

	s[1] = 0;
	nd = n;
	for (i = 1; i <= dig; i++)
	{
		c = nd % 16;
		if (c < 10)
			s[0] = c + '0';
		else
			s[0] = c - 10 + 'A';
		nd = nd / 16;
		px = x + (dig - i) * NUMB2412S;
		DrawFASC12(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawLFU32(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，含0显示，32*16描绘
//------------------------------------------
void DrawLFU32(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0'; //倒序显示
		nd = nd / 10;
		px = x + (dig - i) * 20;
		DrawFASC3216(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawLFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，不含0显示，32*16描绘
//------------------------------------------
void DrawLFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd;
	U32 px;

	nd = n;
	s[1] = 0;
	for (i = 1; i <= dig; i++)
	{
		s[0] = nd % 10 + '0';
		if ((nd == 0) && (i > 1))
			s[0] = ' ';
		nd = nd / 10;
		px = x + (dig - i) * 20;
		DrawFASC3216(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawLFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，不含0显示，32*16描绘(左对齐)(最大9位)
//------------------------------------------
void DrawLFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd, dat = 10;
	U32 px;

	for (i = 1; i < dig; i++)
		dat *= 10;
	nd = n % dat;
	dat /= 10;
	for (i = 1; i < dig; i++)
	{
		if ((nd / dat))
			break;
		dat /= 10;
	}
	px = x;
	s[1] = 0;
	for (; i <= dig; i++)
	{
		if (dat == 0)
			break;
		s[0] = nd / dat + '0';
		nd = nd % dat;
		DrawFASC3216(px, y, s, color);
		px += 20;
		dat /= 10;
	}
}

//------------------------------------------
// Function   : void DrawLFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32位数字,含0显示,32*16描绘,整数，缩进为0
// note: 含指定位置小数点，最后一位不能有小数点，小数点占用一个dig显示位，'-'号可以小于输入的'x'
//------------------------------------------
void DrawLFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
{
	char s[2];
	U32 nd;
	U32 px, pointx;

	nd = abs(n);
	px = x + (dig - 1) * 20;
	s[1] = 0;
	if (point > 0)
		pointx = px - point * 20;
	else
		pointx = XLEN;
	while (px >= x)
	{
		s[0] = nd % 10 + '0';
		nd = nd / 10;
		DrawFASC3216(px, y, s, color);
		px -= 20;
		if (nd == 0 && px < pointx)
			break;
		if (px == pointx)
		{
			s[0] = '.';
			DrawFASC3216(px, y, s, color);
			px -= 20;
		}
	}
	if (n < 0)
	{
		s[0] = '-';
		DrawFASC3216(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawErrTest(U32 x, U32 y, char *s, U32 color)
// Description: 显示YES或NO 、32*16
//------------------------------------------
void DrawErrTest(U32 x, U32 y, char *s, U32 color)
{
	U32 i, j;
	U8 *p;
	const char errTest[] = "YESNO";
	i = 0;
	while (*(s + i) != 0)
	{
		for (j = 0; j < 5; j++)
		{
			if (*(s + i) == errTest[j])
				break;
		}
		if (j < 5)
		{
			p = (U8 *)&msgTest[j][1];
			dsp_F32x16(x + i * 20, y, (U16 *)(p), color);
		}
		i++;
	}
}

//------------------------------------------
// Function   : void DrawLFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32位数字显示24*24描绘
//------------------------------------------
//void DrawLFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
//{
//	char s[2];
//	U32 nd;
//	U32 px, pointx;
//
//	nd = abs(n);
//	px = x+(dig-1)*20;
//	s[1] = 0;
//	if(point>0)	pointx = px-point*20; else pointx = XLEN;
//	while(px>=x)
//	{
//		s[0] = nd%10+'0';
//		nd = nd/10;
//		DrawFASC24(px, y, s, color);
//		px -= 20;
//		if(nd==0 && px<pointx) break;
//		if(px==pointx)
//		{
//			s[0] = '.';
//			DrawFASC24(px, y, s, color);
//			px -= 20;
//		}
//	}
//	if(n<0)
//	{
//		s[0] = '-';
//		DrawFASC24(px, y, s, color);
//	}
//}

//------------------------------------------
// Function   : void DrawLFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字显示24*24描绘左开始
//------------------------------------------
//void DrawLFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color)
//{
//	char s[2];
//	U32 i, nd, ndig;
//
//	nd = 10;
//	ndig = 1;
//	for(i=0; i<dig; i++){
//		if(n>=nd) ndig++;
//		nd = nd*10;
//	}
//
//	s[1] = 0;
//	if(ndig>dig) ndig = dig;
//	x = x+(ndig-1)*20;
//	while(ndig--){
//		s[0] = n%10+'0';
//		n = n/10;
//		DrawFASC24(x, y, s, color);
//		x -= 20;
//	}
//}

//------------------------------------------
// Function   : void DrawLFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字含0显示24*24描绘
//------------------------------------------
//void DrawLFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
//{
//	char c, s[2];
//	U8 i;
//	U32 nd;
//	U32 px;
//
//	s[1] = 0;
//	nd = n;
//	for(i=1; i<=dig; i++)
//	{
//		c = nd%10;
//        s[0] = c+'0';
//		nd = nd/10;
//		px = x+(dig-i)*20;
//        DrawFASC24(px, y, s,color);
//	}
//}

//------------------------------------------
// Function   : void DrawLFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32位数字，含0显示，含ABCDEF的十六进制显示，32*16描绘
//------------------------------------------
void DrawLFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char c, s[2];
	U8 i;
	U32 nd;
	U32 px;

	s[1] = 0;
	nd = n;
	for (i = 1; i <= dig; i++)
	{
		c = nd % 16;
		if (c < 10)
			s[0] = c + '0';
		else
			s[0] = c - 10 + 'A';
		nd = nd / 16;
		px = x + (dig - i) * 20;
		DrawFASC3216(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFFileno(U32 x, U32 y, U16 dat, U32 color)
// Description: 读取画面显示文件编号
//------------------------------------------
void DrawFFileno(U32 x, U32 y, U16 dat, U32 color)
{
	char s2[4];

	if (dat >= MINFILENO && dat <= MAXFILENO)
	{
		if (dat < MINFILENO)
		{
			s2[0] = 0x2a;
			s2[1] = 0x2a;
			s2[2] = 0x2a;
		}
		else
		{
			s2[0] = (dat / 100) % 10 + 0x30;
			s2[1] = (dat / 10) % 10 + 0x30;
			s2[2] = (dat) % 10 + 0x30;
		}
	}
	else
	{
		s2[0] = 0x2d; //0x20;
		s2[1] = 0x2d; //0x20;
		s2[2] = 0x2d; //0x20;
	}
	s2[3] = 0;
	//DrawFASC3216(x, y, s2, color);
	DrawFASC12(x + 8, y + 4, s2, color);
}

//------------------------------------------
// Function   : void DrawFFileno(U32 x, U32 y, U16 dat, U32 color)
// Description: 读取画面显示文件编号
//------------------------------------------
void DrawFFileno1(U32 x, U32 y, U16 dat, U32 color)
{
	char s2[4];

	if (dat >= MINFILENO && dat <= MAXFILENO)
	{
		s2[0] = (dat / 100) % 10 + 0x30;
		s2[1] = (dat / 10) % 10 + 0x30;
		s2[2] = (dat) % 10 + 0x30;
	}
	else
	{
		s2[0] = 0x20; //0x20;
		s2[1] = 0x20; //0x20;
		s2[2] = 0x20; //0x20;
	}
	s2[3] = 0;
	//DrawFASC3216(x, y, s2, color);
	DrawFASC12(x + 8, y + 4, s2, color);
}
//------------------------------------------
// Function   : void DrawFFileno_24x12(U32 x, U32 y, U16 dat, U32 color)
// Description: 读取画面显示文件编号
//------------------------------------------
void DrawFFileno_24x12(U32 x, U32 y, U16 dat, U32 color)
{
	char s2[4];
	if (dat >= MINFILENO && dat <= MAXFILENO)
	{
		if (dat < MINFILENO)
		{
			s2[0] = 0x2a;
			s2[1] = 0x2a;
			s2[2] = 0x2a;
		}
		else
		{
			s2[0] = (dat / 100) % 10 + 0x30;
			s2[1] = (dat / 10) % 10 + 0x30;
			s2[2] = (dat) % 10 + 0x30;
		}
	}
	else
	{
		s2[0] = 0x2d; //0x20;
		s2[1] = 0x2d; //0x20;
		s2[2] = 0x2d; //0x20;
	}
	s2[3] = 0;
	DrawFASC12(x, y, s2, color);
}

//------------------------------------------
// Function   : void DrawFFileno_16x8(U32 x, U32 y, U16 dat, U32 color)
// Description: 读取画面显示文件编号
//------------------------------------------
void DrawFFileno_16x8(U32 x, U32 y, U16 dat, U32 color)
{
	char s2[4];
	if (dat >= MINFILENO && dat <= MAXFILENO)
	{
		if (dat < MINFILENO)
		{
			s2[0] = 0x2a;
			s2[1] = 0x2a;
			s2[2] = 0x2a;
		}
		else
		{
			s2[0] = (dat / 100) % 10 + 0x30;
			s2[1] = (dat / 10) % 10 + 0x30;
			s2[2] = (dat) % 10 + 0x30;
		}
	}
	else
	{
		s2[0] = 0x2d; //0x20;
		s2[1] = 0x2d; //0x20;
		s2[2] = 0x2d; //0x20;
	}
	s2[3] = 0;
	DrawFChar(x, y, s2, color);
}

//------------------------------------------
// Function   : void DrawFFilename(U32 x, U32 y, char *s,U8 namelen, U32 color,U8 len,U8 dy,U8 *RealRows)
// Description: 读取画面显示文件名
//	U8 len : 单行显示的长度
//	U8 dy :第一行和第二行之间的间距
//  U8 *RealRows: 实际显示行数
//------------------------------------------
void DrawFFilename(U32 x, U32 y, char *s, U8 namelen, U32 color, U8 len, U8 dy, U8 *RealRows)
{

	char s2[64] = {0};
	U16 i, j;
	U8 ChineseNums = 0;
	U8 disrow2 = 0;
	U8 tmplen;
	U8 chLineLimit = len;
	if (namelen > 64)
		return; //最大支持64字节名字
	if (namelen == 64)
		tmplen = namelen - 1;
	else
		tmplen = namelen;

	for (i = 0, j = 0; i < tmplen; i++)
	{ //前63字节不合乎指定字符的数据清零，第64字节为类型标志
		if (s[i] == 0)
			break;
		if (((s[i] >= 0x30) && (s[i] <= 0x39)) ||	/*数字*/
			((s[i] >= 0x41) && (s[i] <= 0x5a)) ||	/*大写字母*/
			((s[i] >= 0x61) && (s[i] <= 0x7a)) ||	/*小写字母*/
			((s[i] > 0x7f) && (s[i + 1] > 0x7f)) || /*汉字B0A1-F7FE*/
			(s[i] == 0x5f))							/*下划线*/
		{
			if ((s[i] > 0x7f) && (s[i + 1] > 0x7f))
			{
				s[j] = s[i];
				j++;
				i++;
			}
			s[j] = s[i];
			j++;
		}
	}
	for (; j < tmplen; j++)
		s[j] = 0;

	//先计算需两行还是一行显示	单行显示字体24*12  两行显示字体16*8
	for (i = 0; i < tmplen; i++)
	{
		if (s[i] == 0)
			break;
	}
	if (i >= (16 * chLineLimit / 12 - 1)) //从>修改为>=
		disrow2 = 1;

	wdt();

	if (disrow2 == 1)
	{ //两行显示 字体16*8
		chLineLimit = chLineLimit * 3 / 2;
		for (i = 0; i < (chLineLimit); i++) //先查找第一行其他字符（除ASCLL外）
		{
			if (*(s + i) == 0)
				break;
			if (*(s + i) & 0x80) //中文/其他字符
			{
				ChineseNums++;
			}
		}

		if (i < (chLineLimit)) //实际上一行显示，字体16*8
		{
			for (i = 0; i < (chLineLimit); i++) //把第一行的内容复制到s2中
			{
				if (*(s + i) == 0)
					break;
				s2[i] = *(s + i);
			}
			DrawF16Str(x, y + 4, s2, color);
			if (RealRows != NULL)
				*RealRows = 1;
		}
		else
		{ //两行显示 字体16*8
			wdt();
			if (ChineseNums % 2) //第一行英文个数奇数，显示不完全部，即最后一个中文字符显示溢出
			{
				for (j = 0; j < (chLineLimit - 1); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 4, s2, color); //显示第一行
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (chLineLimit - 1), j = 0; j < (chLineLimit); i++, j++)
				{ //先查找第二行中文个数
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //中文
						ChineseNums++;
					}
				}

				if ((j < (chLineLimit)) || ((ChineseNums % 2) == 0))
				{ //显示完全部名字，或最后一个中文字符没有溢出
					for (j = 0; j < (chLineLimit); j++)
					{
						s2[j] = *(s + chLineLimit - 1 + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //显示第二行
				}
				else
				{
					for (j = 0; j < (chLineLimit - 1); j++)
					{
						s2[j] = *(s + chLineLimit - 1 + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //显示第二行
				}
			}
			else //第一行显示玩去哪
			{
				for (j = 0; j < (chLineLimit); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 4, s2, color); //显示第一行
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (chLineLimit), j = 0; j < (chLineLimit); i++, j++)
				{ //先查找第二行中文个数
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //中文
						ChineseNums++;
					}
				}

				if ((j < (chLineLimit)) || ((ChineseNums % 2) == 0))
				{ //显示完全部名字，或最后一个中文字符没有溢出
					for (j = 0; j < (chLineLimit); j++)
					{
						s2[j] = *(s + chLineLimit + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //显示第二行
				}
				else
				{
					for (j = 0; j < (chLineLimit - 1); j++)
					{
						s2[j] = *(s + chLineLimit + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //显示第二行
				}
			}
			if (RealRows != NULL)
				*RealRows = 2;
		}
	}
	else
	{ //一行显示 字体24*12
		wdt();
		for (i = 0; i < tmplen; i++)
		{
			if (*(s + i) == 0)
				break;
			s2[i] = *(s + i);
		}
		Draw24X24FStr(x, y, s2, color);
		if (RealRows != NULL)
			*RealRows = 1;
	}
}

//------------------------------------------
// Function   : void DrawFFilename(U32 x, U32 y, char *s, U32 color)
// Description: 读取画面显示文件名
//	U8 len : 单行显示的长度
//	U8 dy :第一行和第二行之间的间距
//  U8 *RealRows: 实际显示行数
//------------------------------------------
void DrawFFilename1(U32 x, U32 y, char *s, U8 namelen, U32 color, U8 len, U8 dy, U8 *RealRows)
{

	char s2[64] = {0};
	U16 i, j;
	U8 ChineseNums = 0;
	U8 disrow2 = 0;
	U8 tmplen;

	if (namelen > 64)
		return; //最大支持64字节名字
	if (namelen == 64)
		tmplen = namelen - 1;
	else
		tmplen = namelen;

	//先计算需两行还是一行显示	单行显示字体24*12  两行显示字体16*8
	for (i = 0; i < tmplen; i++)
	{
		if (s[i] == 0)
			break;
	}
	if (i >= (16 * len / 12 - 1))
		disrow2 = 1;

	wdt();

	if (disrow2 == 1)
	{ //两行显示 字体16*8
		for (i = 0; i < (2 * len); i++)
		{ //先查找第一行中文个数
			if (*(s + i) == 0)
				break;
			if (*(s + i) & 0x80)
			{ //中文
				ChineseNums++;
			}
		}

		if (i < (2 * len))
		{ //实际上一行显示，字体16*8
			for (i = 0; i < (2 * len); i++)
			{
				if (*(s + i) == 0)
					break;
				s2[i] = *(s + i);
			}
			DrawF16Str(x, y + 6, s2, color);
			if (RealRows != NULL)
				*RealRows = 1;
		}
		else
		{ //两行显示 字体16*8
			wdt();
			if (ChineseNums % 2)
			{ //第一行英文个数奇数，显示不完全部，即最后一个中文字符显示溢出
				for (j = 0; j < (2 * len - 1); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 6, s2, color); //显示第一行
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (2 * len - 1), j = 0; j < (2 * len); i++, j++)
				{ //先查找第二行中文个数
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //中文
						ChineseNums++;
					}
				}

				if ((j < (2 * len)) || ((ChineseNums % 2) == 0))
				{ //显示完全部名字，或最后一个中文字符没有溢出
					for (j = 0; j < (2 * len); j++)
					{
						s2[j] = *(s + 2 * len - 1 + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //显示第二行
				}
				else
				{
					for (j = 0; j < (2 * len - 1); j++)
					{
						s2[j] = *(s + 2 * len - 1 + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //显示第二行
				}
			}
			else
			{
				for (j = 0; j < (2 * len); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 6, s2, color); //显示第一行
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (2 * len), j = 0; j < (2 * len); i++, j++)
				{ //先查找第二行中文个数
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //中文
						ChineseNums++;
					}
				}

				if ((j < (2 * len)) || ((ChineseNums % 2) == 0))
				{ //显示完全部名字，或最后一个中文字符没有溢出
					for (j = 0; j < (2 * len); j++)
					{
						s2[j] = *(s + 2 * len + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //显示第二行
				}
				else
				{
					for (j = 0; j < (2 * len - 1); j++)
					{
						s2[j] = *(s + 2 * len + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //显示第二行
				}
			}
			if (RealRows != NULL)
				*RealRows = 2;
		}
	}
	else
	{ //一行显示 字体24*12
		wdt();
		for (i = 0; i < tmplen; i++)
		{
			if (*(s + i) == 0)
				break;
			s2[i] = *(s + i);
		}
		Draw24X24FStr(x, y, s2, color);
		if (RealRows != NULL)
			*RealRows = 1;
	}
}

//------------------------------------------
// Function   : void DrawFFileBarcode(U32 x, U32 y,U16 nn, U32 color)
// Description: 显示条形码
//------------------------------------------
void DrawFFileBarcode(U32 x, U32 y, U16 nn, U32 color, U8 Type)
{
	U32 barC;

	barC = ((U32)(IREG2[nn + 1]) << 16) + (U32)IREG2[nn];

	if (Type != 2)
	{
		if ((barC != 0) && (barC < 1000000000))
		{
			DrawFBMP2(x, y, 88, 27, (MEM_BMPIO_STATUS + 864), color);
			DrawFU32(x + 8, y + 12, barC, 9, color);
			if (g_BarcodeFlag[nn - MINFILENO] != 0)
			{
				DrawSerialNumber2(x + 110, y + 18, g_BarcodeFlag[nn - MINFILENO], 0, Color_Red);
			}
		}
	}
	else
	{
		if ((barC != 0) && (barC < 1000000000))
		{
			DrawFBMP2(x, y - 2, 88, 21, (MEM_BMPIO_STATUS + 864), color);
			BmpFillRect(x + 8, y + 5, 72, 8);
			DrawFU32(x + 8, y + 5, barC, 9, color);
			if (g_BarcodeFlag[nn - MINFILENO] != 0)
			{
				DrawSerialNumber2(x + 110, y + 11, g_BarcodeFlag[nn - MINFILENO], 0, Color_Red);
			}
		}
	}
}

//------------------------------------------
// Function   : void DrawButton(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 c1, U32 c2)
// Description: 描绘按钮	//绘制按键松开始的标准色按键
// note: *p实际无效，默认选中148.bin库中图标显示
// c1：边框颜色（KEY_COLOR中选取） C2：按钮颜色(RGB任意颜色)
//------------------------------------------
void DrawButton(KEYPAD bn, U16 *p, U32 c1, U32 c2)
{
	DrawButtonOff1(bn, c1);
	if ((c2 == 0) || (c2 == Color_Gray))
		DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, keypicdata[c1]); //根据C1显示底色，不显示UI
	else
		DrawFPBMPBTN(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c2, keypicdata[c1]); //根据C2显示UI，根据C1显示底色
}

//------------------------------------------
// Function   : void DrawButton1(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 c1, U32 c2)
// Description: 绘制颜色按钮  只可绘制keypicdata[]表中的颜色	 同DrawButton
//------------------------------------------
void DrawButton1(KEYPAD bn, U32 c1, U32 c2)
{
	DrawButtonOff2(bn, c1);
	if (c2 == 0)
	{
#if USEMULTCOLOR //彩屏
		if (Color_White1 == c1 || Color_White == c1)
		{ //白色变换
			c1 = Color_Silver;
		}
		else if (Color_Gray == c1)
		{
		}
		else
		{
			c1 = Color_KeyBlue;
		}
#endif
		DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c1); //根据C1显示底色，不显示UI
	}
	else
	{
		DrawFPBMPBTN(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c2, c1); //根据C2显示UI，根据C1显示底色
	}
}

//------------------------------------------
// Function   : void DrawButton(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 c1, U32 c2)
// Description: 描绘按钮
// note：: 边框大小自选 0:4x4 、1:8x8、2:12x8、3:0x4
//------------------------------------------
void DrawButton2(KEYPAD bn, U16 c1, U16 c2, U16 flag)
{
	U16 datx = 0, daty = 0;
	DrawButtonOff3(bn, c1, flag);
	if (flag == 0)
	{
		datx = 4;
		daty = 4;
	}
	else if (flag == 1)
	{
		datx = 8;
		daty = 8;
	}
	else if (flag == 2)
	{
		datx = 12;
		daty = 8;
	}
	else if (flag == 3)
	{
		datx = 0;
		daty = 4;
	}
	//else if(flag==3) {datx = 0;daty = 4;}
	// if (c2 == 0 || c2 > g_bmplen)
	if (c2 > g_bmplen)
	{
#if USEMULTCOLOR //彩屏
		if (Color_White1 == c1)
		{ //白色变换
			c1 = Color_Silver;
		}
		else if (Color_Gray == c1)
		{
		}
		else
		{
			c1 = Color_KeyBlue;
		}
#endif
		DrawFillRect(bn.xpis + datx, bn.ypis + daty, bn.xsize - 2 * datx, bn.ysize - 2 * daty, c1);
	}
	else
	{
		DrawFPBMPBTN(bn.xpis + datx, bn.ypis + daty, bn.xsize - 2 * datx, bn.ysize - 2 * daty, c2, keypicdata[c1]);
	}
}

//------------------------------------------
// Function   : void DrawButton3(KEYPAD bn,U16 c1,U16 c2)
// Description: 描绘按钮	 //显示图标原样，不添加按钮阴影
//------------------------------------------
void DrawButton3(KEYPAD bn, U16 c1, U16 c2)
{
	if (c2 == 0 || c2 > g_bmplen)
	{
		DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, c1);
	}
	else
	{
		DrawFPBMPBTN1(bn.xpis, bn.ypis, bn.xsize, bn.ysize, c2);
	}
}
//------------------------------------------
// Function   : void DrawButtonSingleColor(KEYPAD bn, U32 c1, U32 c2)
// Description: 绘制颜色按钮 c1:边框颜色 c2:背景颜色 只可绘制keypicdata[]表中的颜色
//------------------------------------------
void DrawButtonSingleColor(KEYPAD bn, U32 c1, U32 c2)
{
	DrawButtonOff1(bn, c1);
	DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, keypicdata[c2]); //根据C1显示底色，不显示UI
}

//------------------------------------------
// Function   : void DrawButtonDisPat(KEYPAD bn, U32 colorBg, U32 colorBor)
// Description: 绘制用于显示花样的无按键特效按钮
//------------------------------------------
void DrawButtonDisPat(KEYPAD bn, U32 colorBor, U32 colorBg)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBg);
	DrawRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBor);
}

//------------------------------------------
// Function   : DrawButtonEdit(KEYPAD bn, U32 colorBor, U32 colorBg)
// Description: 绘制文本框按钮
//------------------------------------------
void DrawButtonEdit(KEYPAD bn, U32 colorBor, U32 colorBg)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBg);
	DrawDottedRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBor);
}

//------------------------------------------
// Function   :
// Description: 绘制文本框标题
//------------------------------------------
void DrawEditTit(RECTPAD bn, U32 strIndex, U16 sheetPage, tyden_alignment alignment, tyden_strSize size, U16 safeDist_X)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, bn.colorB[0]);

	if (size == STR16X8)
	{
		if (alignment == LEFT || alignment == RIGHT)
		{
			Draw16x16Str(bn.xpis + safeDist_X,
						 bn.ypis + (bn.ysize - 16) / 2,
						 (MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax)),
						 sheetPage, strIndex, bn.colorT[0]); //画出方框的文字
		}
		else
		{
			U16 len = strlen((char *)((MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax))));
			if ((9 * len) > bn.xsize)
			{
				len = 0;
			}
			else
			{
				len = (bn.xsize - (9 * len)) / 2;
			}
			Draw16x16Str(bn.xpis + len,
						 bn.ypis + (bn.ysize - 16) / 2,
						 (MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax)),
						 sheetPage, strIndex, bn.colorT[0]); //画出方框的文字
		}
	}
	else if (size == STR24X12)
	{
		if (alignment == LEFT || alignment == RIGHT)
		{
			Draw24x24Str(bn.xpis + safeDist_X,
						 bn.ypis + (bn.ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax)),
						 sheetPage, strIndex, bn.colorT[0]); //画出方框的文字
		}
		else
		{
			U16 len = strlen((char *)((MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax))));
			if ((9 * len) > bn.xsize)
			{
				len = 0;
			}
			else
			{
				len = (bn.xsize - (13 * len)) / 2;
			}
			Draw24x24Str(bn.xpis + len,
						 bn.ypis + (bn.ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax)),
						 sheetPage, strIndex, bn.colorT[0]); //画出方框的文字
		}
	}
}

//------------------------------------------
// Function   : void DrawEdit(RECTPAD bn, U32 colorBor, U32 colorBg)
// Description: 绘制文本框
//------------------------------------------
void DrawEdit(RECTPAD bn, U32 colorBor, U32 colorBg)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBg);
	DrawDottedRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBor);
}

//------------------------------------------
// Function   : void DrawButtonOn(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮按下   //绘制按钮按下时候的按键
// note:存在现实颜色范围限制 KEY_COLORTL
//		当按钮显示位置(1,1)的颜色不属于keypicdatb[][]颜色表时，限制按钮渐渐变边框可选的颜色范围
//------------------------------------------
void DrawButtonOn(KEYPAD bn, U16 c)
{
	U16 cc, i;
	cc = ReadPixel(bn.xpis + 1, bn.ypis + 1);
	for (i = 0; i < KEY_COLORTL; i++)
	{
		if (cc == keypicdatb[i][0])
			break;
	}

	if (i >= KEY_COLORTL)
		i = c;

	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, Color_Black, Color_White);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatc[i][0], keypicdatb[i][0]);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, keypicdatc[i][1], keypicdatb[i][1]);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, keypicdata[i], keypicdata[i]);
}

//------------------------------------------
// Function   : void DrawButtonOn1(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮按下		//绘制按钮按下时候的按键
// note:显示颜色无限制，一样受限于 keypicdatb keypicdatc
//------------------------------------------
void DrawButtonOn1(KEYPAD bn, U16 c)
{
	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, Color_Black, Color_White);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatc[c][0], keypicdatb[c][0]);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, keypicdatc[c][1], keypicdatb[c][1]);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, keypicdata[c], keypicdata[c]);
}
//------------------------------------------
// Function   : void DrawButtonOnZoomIn(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮，缩放按键		//绘制按钮按下时候的按键
// note: ZoomInNum:按键缩放像素数量
//------------------------------------------
U16 chZoomInColor = 0, chZoomInPx = 0, chZoomInColor2 = 0;
void DrawButtonOnZoomIn(KEYPAD bn, U16 ZoomInNum)
{
	if (ZoomInNum < 1)
		return;
	chZoomInColor = ReadPixel(bn.xpis + 1, bn.ypis + 1);
	chZoomInColor2 = ReadPixel(bn.xpis, bn.ypis);
	chZoomInPx = ZoomInNum;
	BmpFillRect(bn.xpis, bn.ypis, bn.xsize, ZoomInNum);													   //上
	BmpFillRect(bn.xpis, bn.ypis + bn.ysize - ZoomInNum, bn.xsize, ZoomInNum);							   //下
	BmpFillRect(bn.xpis, bn.ypis + ZoomInNum, ZoomInNum, bn.ysize - 2 * ZoomInNum);						   //左
	BmpFillRect(bn.xpis + bn.xsize - ZoomInNum, bn.ypis + ZoomInNum, ZoomInNum, bn.ysize - 2 * ZoomInNum); //右
}
//------------------------------------------
// Function   : void DrawButtonOnZoomInRes(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮，缩放按键重置		//绘制按钮按下时候的按键
// note: ZoomInNum:按键缩放像素数量
//------------------------------------------
void DrawButtonOnZoomInRes(KEYPAD bn)
{
	if (chZoomInPx < 1)
		return;

	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, chZoomInPx, chZoomInColor);													   //上
	DrawFillRect(bn.xpis, bn.ypis + bn.ysize - chZoomInPx, bn.xsize, chZoomInPx, chZoomInColor);							   //下
	DrawFillRect(bn.xpis, bn.ypis + chZoomInPx, chZoomInPx, bn.ysize - 2 * chZoomInPx, chZoomInColor);						   //左
	DrawFillRect(bn.xpis + bn.xsize - chZoomInPx, bn.ypis + chZoomInPx, chZoomInPx, bn.ysize - 2 * chZoomInPx, chZoomInColor); //右
	DrawDottedRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, chZoomInColor2);
	chZoomInPx = 0;
	chZoomInColor = 0;
	chZoomInColor2 = 0;
}

//------------------------------------------
// Function   : void DrawButtonOff(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮
// note:存在现实颜色范围限制 KEY_COLORTL
//		当按钮显示位置(1,1)的颜色不属于keypicdatb[][]颜色表时，限制按钮渐渐变边框可选的颜色范围
//------------------------------------------
void DrawButtonOff(KEYPAD bn, U16 c)
{
	U16 cc, i;
	cc = ReadPixel(bn.xpis + 1, bn.ypis + 1);
	for (i = 0; i < KEY_COLORTL; i++)
	{
		if (cc == keypicdatc[i][0])
			break;
	}
	if (i >= KEY_COLORTL)
		i = c;

	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, Color_White, Color_Black);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatb[i][0], keypicdatc[i][0]);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, keypicdatb[i][1], keypicdatc[i][1]);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, keypicdatb[i][2], keypicdatc[i][2]);
}

//------------------------------------------
// Function   : void DrawButtonOff1(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮		//彩色的按键边框过渡
// note:显示颜色无限制，一样受限于 keypicdatb keypicdatc
//------------------------------------------
void DrawButtonOff1(KEYPAD bn, U16 c)
{
	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, Color_White, Color_Black);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatb[c][0], keypicdatc[c][0]);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, keypicdatb[c][1], keypicdatc[c][1]);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, keypicdatb[c][2], keypicdatc[c][2]);
}

//------------------------------------------
// Function   : void DrawButtonOff1(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮	//彩色的按键边框过渡 同DrawButtonOff1
// note: 过渡边框颜色转变：白色转变为银色    ，灰色不变，其他颜色转换为蓝色
//------------------------------------------
void DrawButtonOff2(KEYPAD bn, U16 c)
{
	//printf("---  xpis = %d ypis = %d xsize = %d ysize =%d c = %04x-------------\r\n",bn.xpis,bn.ypis,bn.xsize,bn.ysize,c);
	// #if USEMULTCOLOR //彩屏
	U16 c1;
	if (Color_White1 == c || Color_White == c)
	{ //白色变换
		c1 = KEY_Silver;
	}
	else if (Color_Gray == c)
	{
		c1 = KEY_Gray;
	}
	else
	{
		c1 = KEY_Blue;
	}
	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, Color_White, Color_Black);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatb[c1][0], keypicdatc[c1][0]);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, keypicdatb[c1][1], keypicdatc[c1][1]);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, keypicdatb[c1][2], keypicdatc[c1][2]);

	// #else //灰度

	// 	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, c, c);
	// 	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, c, c);
	// 	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, c, c);
	// 	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, c, c);
	// #endif
}

//------------------------------------------
// Function   : void DrawButtonOff1(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮	//绘制不同大小的按键阴影 flag 0：标志 1：大 2：特大
// note: 过渡边框颜色转变：白色转变为银色    ，灰色不变，其他颜色转换为蓝色
//		 flag可以选择显示边框的大小，默认0:4x4、1:8x8、2:12x12
//------------------------------------------
void DrawButtonOff3(KEYPAD bn, U16 c, U16 flag)
{

#if USEMULTCOLOR //彩屏
	U16 c1;
	if (c > g_bmplen)
	{
		if (Color_White1 == c)
		{ //白色变换
			c1 = KEY_Silver;
			c = Color_Silver;
		}
		else if (Color_Gray == c)
		{
			c1 = KEY_Gray;
		}
		else
		{
			c1 = KEY_Blue;
			c = Color_KeyBlue;
		}
	}
	else
	{
		c1 = c;
		c = keypicdata[c1];
	}

	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, Color_White, Color_Black);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatb[c1][0], keypicdatc[c1][0]);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, keypicdatb[c1][1], keypicdatc[c1][1]);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, keypicdatb[c1][2], keypicdatc[c1][2]);

#else //灰度显示
	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, c, c);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, c, c);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, c, c);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, c, c);
#endif

	if (flag == 1)
	{
		DrawFrame(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c, c);
		DrawFrame(bn.xpis + 5, bn.ypis + 5, bn.xsize - 10, bn.ysize - 10, c, c);
		DrawFrame(bn.xpis + 6, bn.ypis + 6, bn.xsize - 12, bn.ysize - 12, c, c);
		DrawFrame(bn.xpis + 7, bn.ypis + 7, bn.xsize - 14, bn.ysize - 14, c, c);
	}
	else if (flag == 2)
	{
		DrawFrame(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c, c);
		DrawFrame(bn.xpis + 5, bn.ypis + 5, bn.xsize - 10, bn.ysize - 10, c, c);
		DrawFrame(bn.xpis + 6, bn.ypis + 6, bn.xsize - 12, bn.ysize - 12, c, c);
		DrawFrame(bn.xpis + 7, bn.ypis + 7, bn.xsize - 14, bn.ysize - 14, c, c);
		DrawFrame(bn.xpis + 8, bn.ypis + 6, bn.xsize - 16, bn.ysize - 12, c, c);
		DrawFrame(bn.xpis + 9, bn.ypis + 7, bn.xsize - 18, bn.ysize - 14, c, c);
		DrawFrame(bn.xpis + 10, bn.ypis + 6, bn.xsize - 20, bn.ysize - 12, c, c);
		DrawFrame(bn.xpis + 11, bn.ypis + 7, bn.xsize - 22, bn.ysize - 14, c, c);
	}
}

//------------------------------------------
// Function   : void DrawButtonOff1(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮
// note:颜色不再限制为 keypicdatb 与 keypicdatc的颜色表
//------------------------------------------
void DrawButtonOff4(KEYPAD bn, U16 c)
{
	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, c, c);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, c, c);
	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, c, c);
	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, c, c);
}

//------------------------------------------
// Function   : void DrawButtonOff1(U32 x, U32 y, U32 dx, U32 dy)
// Description: 描绘按钮
// note:窄边框。2x2
//------------------------------------------
void DrawButtonOff5(KEYPAD bn, U16 c)
{
	DrawFrame(bn.xpis + 0, bn.ypis + 0, bn.xsize - 0, bn.ysize - 0, keypicdatb[c][1], keypicdatc[c][1]);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatb[c][2], keypicdatc[c][2]);
}
//------------------------------------------
// Function   : void DrawTextbox(KEYPAD bn, U16 *p, U32 c1, U32 c2)
// Description: 绘制文本显示框
//------------------------------------------
void DrawTextbox(KEYPAD bn, U32 c1)
{
	DrawButtonOn1(bn, c1);
	DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, keypicdata[c1]);
}

extern volatile S32 g_array3[2][2];
//------------------------------------------
// Function   : void DrawArc(U32 x1,U32 y1,U32 x2,U32 y2, U32 x3,U32 y3,U8 f,U32 color)
// Description: 绘园弧,输入三点坐标 0.0-360.0夹角旋转描绘
//------------------------------------------
void DrawArc(S32 x1, S32 y1, S32 x2, S32 y2, S32 x3, S32 y3, U8 f, U32 color)
{
	U32 i, cw;				  // 园心
	S32 xo, yo, x, y;		  // 方向
	U32 an, ang1, ang2, ang3; // 三个点的象限位置
	S32 dx, dy;
	F32 r; // 半径
	COORDINATE POSI;

	g_array3[0][0] = (S32)(x2) - (S32)(x1);
	g_array3[0][1] = (S32)(y2) - (S32)(y1);
	g_array3[1][0] = (S32)(x3) - (S32)(x1);
	g_array3[1][1] = (S32)(y3) - (S32)(y1);

	circle_center(g_array3[0][0], g_array3[0][1], g_array3[1][0], g_array3[1][1], &POSI);
	xo = (S32)(x1) + POSI.x;
	yo = (S32)(y1) + POSI.y;

	r = sqrt((x1 - xo) * (x1 - xo) + (y1 - yo) * (y1 - yo));
	ang1 = GetAng(x1 - xo, yo - y1);
	ang2 = GetAng(x2 - xo, yo - y2);
	if (ang2 < ang1)
		ang2 += 3600;
	ang3 = GetAng(x3 - xo, yo - y3);
	if (ang3 < ang1)
		ang3 += 3600;
	if (ang2 >= ang3)
		cw = 0; // 顺圆
	else
		cw = 1; // 逆圆

	if (cw == 0)
	{
		an = ang1;
		ang1 = ang3;
		ang3 = an + 3600;
	}
	if (f)
		ang3 = ang1 + 3600;

	for (i = ang1; i <= ang3; i += 5)
	{ //////////????????			(5*DISP_PARA_Y)
		an = i;
		while (an >= 3600)
			an -= 3600;
		dx = abs(r * cos(an * 3.14159 / 1800));
		dy = abs(r * sin(an * 3.14159 / 1800));
		if ((an <= 900) || (an > 2700))
			x = xo + dx;
		else
			x = xo - dx;
		if (an <= 1800)
			y = yo - dy;
		else
			y = yo + dy;

		//x*=DISP_PARA_X;y*=DISP_PARA_Y;

		PutPixel1(x, y, color);
	}
}

//------------------------------------------
// Function   : void DrawSerialNumber(U32 x, U32 y, char n, U32 color)
// Description: 显示输入点,选择点的编号		0 1 2 3 4 5 6 7 8 9 H  S  E  P
// note : flag:读取指定点的像素 f:用指定颜色擦除对应位置
//------------------------------------------
void DrawSerialNumber(S32 x, S32 y, char n, U8 f, U32 color, U8 flag) //注意坐标范围，负值不显示
{
	char s[2];
	U16 xx, yy;
	U16 xx1, yy1;
	const U8 tabbuf[] = {0x48, 0x53, 0x45, 0x50}; //H,S,E,P

	s[0] = tabbuf[n];
	s[1] = 0;
	if (x > 9)
		xx = x - 9;
	else
		xx = 0;
	if (y > 9)
		yy = y - 9;
	else
		yy = 0;
	xx1 = xx + 18;
	yy1 = yy + 18;

	g_dispat1.IsDat = 0;
	if (DrawLimtf)
	{
		if ((xx < Drawmx0) || (xx1 > Drawmx1) || (yy < Drawmy0) || (yy1 > Drawmy1))
		{
			return;
		}
	}
	if (flag)
		ReadPointData1(xx, yy);
	if (f)
		// DrawFillRect(xx, yy, 18, 18, USEMULTCOLOR ? Color_Silver : Color_White);
		DrawFillRect(xx, yy, 18, 18, COLOR_CONVAS);
	else
		BmpFillRect(xx, yy, 18, 18);
	DrawRect(xx, yy, 18, 18, color);
	DrawFChar(xx + 5, yy, s, color);
}

//------------------------------------------
// Function   : void DrawSerialNumber(U32 x, U32 y, char n, U32 color)
// Description: 显示输入点,选择点的编号		0 1 2 3 4 5 6 7 8 9
// note ： f:用指定颜色擦除对应位置
//------------------------------------------
void DrawSerialNumber2(S32 x, S32 y, U16 n, U8 f, U32 color)
{
	U16 xx, yy, dx;
	U16 xx1, yy1;
	U16 dat;
	U8 num;

	dat = n;
	for (num = 0; num < 5; num++)
	{
		if (dat == 0)
			break;
		dat = dat / 10;
	}
	if (num == 0)
		num = 1;
	n = n % 10000;

	dat = 8 * num + 2;
	if (dat < 18)
		dat = 18;
	dx = dat >> 1;
	if (x > dx)
		xx = x - dx;
	else
		xx = 0;
	if (y > 9)
		yy = y - 9;
	else
		yy = 0;
	xx1 = xx + dat;
	yy1 = yy + 18;
	if (DrawLimtf)
	{
		if ((xx < Drawmx0) || (xx1 > Drawmx1) || (yy < Drawmy0) || (yy1 > Drawmy1))
		{
			return;
		}
	}
	if (f)
		// DrawFillRect(xx, yy, dat, 18, USEMULTCOLOR ? Color_Silver : Color_White);
		DrawFillRect(xx, yy, dat, 18, COLOR_CONVAS);
	else
		BmpFillRect(xx, yy, dat, 18);
	DrawRect(xx, yy, dat, 18, color);
	DrawFU32(xx + ((num > 1) ? 1 : 5), yy, n, num, color);
}

//------------------------------------------
// Function   : void DrawColor_Gray(U32 x, U32 y,U32 dx,U32 dy)
// Description: 图形变灰
//------------------------------------------
void DrawColorGray(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;
	U16 *dval;
	U16 *dadd;
	U16 data2;
	U32 data3;

	x2 = x + dx;
	y2 = y + dy;
	if (y2 > LCD_height)
		y2 = LCD_height;
	if (y >= y2)
		return;

	if (x2 > LCD_width)
		x2 = LCD_width;
	if (x >= x2)
		return;
	while (y < y2)
	{
		dadd = (U16 *)(BufAdd + 2 * LCD_width * y + 2 * x);
		for (i = (2 * x); i < (2 * x2); i += 2)
		{
			dval = (U16 *)(BufAdd + 2 * LCD_width * y + i);
			data2 = *(dval);
			col_r = ((data2 & 0xf800) >> 11) * 8;
			col_g = ((data2 & 0x07e0) >> 5) * 4;
			col_b = (data2 & 0x001f) * 8;
			col_r = (col_r + col_g + col_b) / 3;
			data3 = RGB(col_r, col_r, col_r);

			*(dadd) = data3;
			dadd++;
		}
		y++;
		wdt();
	}
}

//备份指定区域颜色
void backupAreaColor(U16 x, U16 y, U16 dx, U16 dy)
{
	U8 savePage;
	U32 srcAddr, desAddr;
	U32 i, ii, j, x2, y2;
	//printf("backupAreaColor ------------\r\n");
	if (usepageno == 1)
		savePage = 2;
	else
		savePage = 1;

	srcAddr = LCD_VRAM_BASE_ADDR + usepageno * DISP_PAGE_SIZE;
	desAddr = LCD_VRAM_BASE_ADDR + savePage * DISP_PAGE_SIZE;

#if VIEWTYPE == HORIZONTAL //横屏
	if ((x < XLEN) && (y < YLEN))
	{
		wdt();
		y2 = y + dy;
		y2 = y2 * YOFFSET;
		for (j = y * YOFFSET; j < y2; j += YOFFSET)
		{
			x2 = x + dx;
			i = srcAddr + j;
			ii = desAddr + j;
			x2 = i + x2 + x2;
			for (i = i + x + x, ii = ii + x + x; i < x2; i += 2, ii += 2)
			{
				*((U16 *)ii) = *((U16 *)i);
			}
		}
	}
#else //竖屏
	if ((x < XLEN) && (y < YLEN))
	{
		x2 = x + dx;
		if (x2 > XLEN)
			x2 = XLEN;
		x2 = x2 * XOFFSET;
		for (j = x * XOFFSET; j < x2; j += XOFFSET)
		{
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			i = srcAddr + j;
			ii = desAddr + j;
			y2 = i + y2 + y2;
			for (i = i + y + y, ii = ii + y + y; i < y2; i += 2, ii += 2)
			{
				*((U16 *)ii) = *((U16 *)i);
			}
		}
	}
#endif
}

//恢复指定区域颜色
void recoverAreaColor(U16 x, U16 y, U16 dx, U16 dy)
{
	U8 savePage;
	U32 srcAddr, desAddr;
	U32 i, ii, j, x2, y2;

	if (usepageno == 1)
		savePage = 2;
	else
		savePage = 1;

	//printf("recoverAreaColor ------------\r\n");
	srcAddr = LCD_VRAM_BASE_ADDR + usepageno * DISP_PAGE_SIZE;
	desAddr = LCD_VRAM_BASE_ADDR + savePage * DISP_PAGE_SIZE;

#if VIEWTYPE == HORIZONTAL //横屏
	if ((x < XLEN) && (y < YLEN))
	{
		wdt();
		y2 = y + dy;
		y2 = y2 * YOFFSET;
		for (j = y * YOFFSET; j < y2; j += YOFFSET)
		{
			x2 = x + dx;
			i = srcAddr + j;
			ii = desAddr + j;
			x2 = i + x2 + x2;
			for (i = i + x + x, ii = ii + x + x; i < x2; i += 2, ii += 2)
			{
				*((U16 *)i) = *((U16 *)ii);
			}
		}
	}
#else //竖屏
	if ((x < XLEN) && (y < YLEN))
	{
		x2 = x + dx;
		if (x2 > XLEN)
			x2 = XLEN;
		x2 = x2 * XOFFSET;
		for (j = x * XOFFSET; j < x2; j += XOFFSET)
		{
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			i = srcAddr + j;
			ii = desAddr + j;
			y2 = i + y2 + y2;
			for (i = i + y + y, ii = ii + y + y; i < y2; i += 2, ii += 2)
			{
				*((U16 *)i) = *((U16 *)ii);
			}
		}
	}
#endif
}

//覆盖指定区域颜色
void recoverAreaColor1(U16 x, U16 y, U16 x1, U16 y1, U16 dx, U16 dy)
{
	U8 savePage;
	U32 srcAddr, desAddr;
	U32 i, ii, j, j1, x2, y2, x12, y12; //i1

	if (usepageno == 1)
		savePage = 2;
	else
		savePage = 1;

	//printf("recoverAreaColor ------------\r\n");
	srcAddr = LCD_VRAM_BASE_ADDR + usepageno * DISP_PAGE_SIZE;
	desAddr = LCD_VRAM_BASE_ADDR + savePage * DISP_PAGE_SIZE;

#if VIEWTYPE == HORIZONTAL //横屏
	if ((x < XLEN) && (y < YLEN))
	{
		wdt();
		y2 = y + dy;
		y2 = y2 * YOFFSET;

		y12 = y1 + dy;
		y12 = y12 * YOFFSET;
		for (j = y * YOFFSET, j1 = y1 * YOFFSET; j < y2; j += YOFFSET, j1 += YOFFSET)
		{
			x2 = x + dx;
			x12 = x1 + dx;

			i = srcAddr + j1;
			ii = desAddr + j;
			x2 = i + x12 + x12;
			for (i = i + x1 + x1, ii = ii + x + x; i < x2; i += 2, ii += 2)
			{
				*((U16 *)i) = *((U16 *)ii);
			}
		}
	}
#else //竖屏
	if ((x < XLEN) && (y < YLEN))
	{
		x2 = x + dx;
		if (x2 > XLEN)
			x2 = XLEN;
		x2 = x2 * XOFFSET;
		for (j = x * XOFFSET; j < x2; j += XOFFSET)
		{
			y2 = y + dy;
			if (y2 > YLEN)
				y2 = YLEN;
			i = srcAddr + j;
			ii = desAddr + j;
			y2 = i + y2 + y2;
			for (i = i + y + y, ii = ii + y + y; i < y2; i += 2, ii += 2)
			{
				*((U16 *)i) = *((U16 *)ii);
			}
		}
	}
#endif
}

/*-----------------------------------------------
//函数名:void setDiskStatus(U8 status)
//功能  :设置磁盘状态
--------------------------------------*/
void initViewStatus(void)
{
	setDiskStatus(0);
	clrShowTime();
	Timer_dms = 1023;
	Timer_tms = 30720;
}

/*-----------------------------------------------
//函数名:void updateViewStatus(U8 status)
//功能  :更新时间磁盘状态
--------------------------------------*/
void updateViewStatus(void)
{
	if ((Timer_dms > 1000))
	{
		Timer_dms = 0;
		showDiskStatus();
	}
	if (Timer_tms >= 30000)
	{ //30720
		Timer_tms = 0;
		showTimeOnMenu();
	}
	refreshPointOnMenu(Timer_tms >> 10);
}

/*-----------------------------------------------
//函数名: U8 showTime(U16 y,U8 m,U8 d)
//功能  : 指定位置显示系统时间
--------------------------------------*/
void showTime(U16 x, U16 y)
{
	U8 week;
	U16 tlang = getLanguage();
	Read_D(D_Address[SYS] + SYS_MS, 4, (U16 *)(&YREG[SYS_MS])); // 读秒分
	g_SysTimes.minute = BCD_2Hex((U8)(YREG[SYS_MS] >> 8));
	SetLanguage(MENU_P1);

	if (g_old_minute != g_SysTimes.minute)
	{ //每分钟刷新一次时间
		g_old_minute = g_SysTimes.minute;

		g_SysTimes.hour = BCD_2Hex((U8)(YREG[SYS_WH] & 0x007f));
		g_SysTimes.day = BCD_2Hex((U8)(YREG[SYS_MD] & 0x00ff));
		g_SysTimes.month = BCD_2Hex((U8)(YREG[SYS_MD] >> 8));
		g_SysTimes.year = BCD_2Hex((U8)(YREG[SYS_Y] & 0x00ff));

		//week = (g_SysTime.day+2*g_SysTime.month+3*(g_SysTime.month+1)/5+(g_SysTime.year+2000)+(g_SysTime.year+2000)/4-(g_SysTime.year+2000)/100+(g_SysTime.year+2000)/400)%7;
		//week = (g_SysTime.day+2*g_SysTime.month+3*(g_SysTime.month+1)/5+(g_SysTime.year+2000)+(g_SysTime.year+2000)/4-(g_SysTime.year+2000)/100+(g_SysTime.year+2000)/400)%7;
		week = getWeekDay(2000 + g_SysTimes.year, g_SysTimes.month, g_SysTimes.day);
		//刷新显示当前时间
		BmpFillRect(x, y, LCD_WIDTH - TIT_TIME_XPOS, 24);

		DrawMFU32(x, y, 2000 + g_SysTimes.year, 4, Color_White); //年
		Draw24X24FStr(x + 52, y, "/", Color_White);
		DrawMFU32(x + 65, y, g_SysTimes.month, 2, Color_White); //月
		Draw24X24FStr(x + 91, 4, "/", Color_White);
		DrawMFU32(x + 104, y, g_SysTimes.day, 2, Color_White); //日

		Draw24x24Str(x + 130, y, MEM_TITLE_ADD + (week + 42) * TitleLengthMax, SheetPage, week + 42, Color_White);

		DrawMFU32(x + 220, y, g_SysTimes.hour, 2, Color_White);	  //时
		DrawMFU32(x + 259, y, g_SysTimes.minute, 2, Color_White); //分
	}
	SetLanguage(tlang);
}

/*-----------------------------------------------
//函数名: void showTimeOnMenu()
//功能  : 菜单栏显示系统时间
--------------------------------------*/
void showTimeOnMenu(void)
{
	showTime(TIT_TIME_XPOS, TIT_TIME_YPOS);
}
/*-----------------------------------------------
//函数名:void refreshPoint(U16 x,U16 y,U16 tims)
//功能  : 刷新指定位置的':'
--------------------------------------*/
void refreshPoint(U16 x, U16 y, U16 time)
{
	U16 static tmptimes = 0xffff;
	if (tmptimes != time)
	{
		tmptimes = time;
		if (tmptimes & 0x1)
			Draw24X24FStr(x, y, ":", Color_White);
		else
			BmpFillRect(x, y, 12, 24);
	}
}

/*-----------------------------------------------
//函数名:void refreshPointOnMenu(void)
//功能  : 刷新菜单栏指定位置的':'
--------------------------------------*/
void refreshPointOnMenu(U16 time)
{
	U16 xpos = TIT_TIME_XPOS + 200 + 46;//754
	refreshPoint(xpos, TIT_TIME_YPOS, time);
}

/*-----------------------------------------------
//函数名:void showTimeAndDisk()
//功能  : 标题栏显示外部设备状态和时间
--------------------------------------*/
void showTimeAndDisk(void)
{
	if ((Timer_dms > 1000))
	{
		Timer_dms = 0;
		showDiskStatus();
	}
	if (Timer_tms >= 30000)
	{ //30720
		Timer_tms = 0;
		showTimeOnMenu();
	}
	refreshPointOnMenu(Timer_tms >> 10);
}
/*-----------------------------------------------
//函数名:void showDiskStatus()
//功能  : 标题栏显示外部设备状态
--------------------------------------*/
void showDiskStatus(void)
{
	U16 xpos;
	U8 status = getDiskStatus();

	xpos = TIT_DISK_XPOS;
	if ((getDiskStatus1() & 0x02) != (status & 0x02)) //U盘状态变化
	{
		if (status & 0x02)
		{
			DrawFPBMPBTN(xpos, 0, 32, 32, 260, Color_White);
		}
		else
		{
			BmpFillRect(xpos, 0, 32, 32);
		}
	}
	setDiskStatus(status);
}

//------------------------------------------
// Function   : void FillRectColorChang(U32 x,U32 y,U32 dx,U32 dy,U32 color1,U32 color2)
// Description: 长方形颜色替换
//------------------------------------------
void FillRectColorChang(U32 x, U32 y, U32 dx, U32 dy, U32 color1, U32 color2)
{
	U32 x0, y0, x2, y2;
	x2 = x + dx;
	y2 = y + dy;
	if (x2 > XLEN)
		x2 = XLEN;
	if (y2 > YLEN)
		y2 = YLEN;

	if ((x < XLEN) && (y < YLEN))
	{
		wdt();
		for (y0 = y; y0 < y2; y0++)
		{
			wdt();
			for (x0 = x; x0 < x2; x0++)
			{
				if (ReadPixel(x0, y0) == color1)
				{
					PutPixel(x0, y0, color2);
				}
			}
		}
	}
}

//------------------------------------------
// Function   : DrawCenterStrArea(U32 x, U32 y,U16 dx,U16 dy, char *str, U32 color)
// Description: 指定区域居中显示字符串16x8
//------------------------------------------
void DrawCenterStrArea1(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U32 color)
{
	U16 dpox = 0, dpoy = 0;
	U16 dispix = getStrLen((U8 *)str) * 8; //计算显示数字占有像素点
	U16 strlen;
	U8 trow = 0;
	U8 i, j;
	char tstr[64] = {0};
	if (row && col)
	{ //设置显示行数和列数
		strlen = getStrLen((U8 *)str);
		trow = strlen / col + ((strlen % col) ? 1 : 0);
		if (trow > row)
			trow = row;
		//if(trow>1) dpox =0;
		//else if(dx>dispix) dpox = (dx-dispix)/2;
		dispix = (strlen > col ? col : strlen) * 8;
		if (dx > dispix)
			dpox = (dx - dispix) / 2;

		if (trow == 1)
		{
			if (dy > 16)
				dpoy = (dy - 16) / 2;
		}
		else if (dy > trow * 18)
		{
			dpoy = (dy - trow * 18) / 2;
		}

		for (i = 0; i < trow; i++)
		{
			mymemset(tstr, 0, sizeof(tstr));
			for (j = 0; j < col; j++)
			{
				if ((*str) == 0)
					break;
				if ((*str) & 0x80 && (*(str + 1)) & 0x80)
				{
					if (j + 2 >= col)
					{
						break;
					}
					else
					{
						tstr[j] = (*str);
						str++;
						j++;
						tstr[j] = (*str);
						str++;
					}
				}
				else
				{
					tstr[j] = (*str);
					str++;
				}
			}
			DrawF16Str(x + dpox, y + dpoy, tstr, color);
			y += 18;
			wdt();
		}
	}
	else
	{
		if (dx > dispix)
			dpox = (dx - dispix) / 2;
		if (dy > 16)
			dpoy = (dy - 16) / 2;
		DrawF16Str(x + dpox, y + dpoy, str, color);
	}
}

//------------------------------------------
// Function   : void DrawCenterTextArea1(U32 x, U32 y,U16 dx,U16 dy, char *str,U16 sheet, U32 color)
// Description: 指定区域居中显示字符串16x8
//------------------------------------------
void DrawCenterTextArea1(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U16 sheet, U32 color)
{
	//printf("str %s \r\n",str);
	if (P_LAN < 2)
	{ //中英文
		DrawCenterStrArea1(x, y, dx, dy, row, col, str, color);
	}
	else
	{
		DrawCenterStrArea1(x, y, dx, dy, row, col, str, color);
	}
}

//------------------------------------------
// Function   : void DrawCenterTextArea1(U32 x, U32 y,U16 dx,U16 dy, char *str,U16 sheet, U32 color)
// Description: 指定区域居中显示字符串16x8
//------------------------------------------
void DrawCenterTextArea16(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U16 sheet, U16 p, U32 color)
{
	U32 stroffest;
	U16 strlen = 0;
	U16 dispix = 0; //计算显示数字占有像素点
	U16 vdx = 0, vdy = 0;
	U16 trow = 0;
	//printf("str %s \r\n",str);
	if (P_LAN < 2)
	{ //中英文
		DrawCenterStrArea1(x, y, dx, dy, row, col, str, color);
	}
	else
	{
		if (CountUnicodeInfo(MEM_MULTILINGUA, sheet, p, P_LAN - 2, &stroffest, &strlen))
		{
		}
		else
		{
			return;
		}
		dispix = (strlen > col ? col : strlen) * 9;
		if (dx > dispix)
		{
			vdx = (dx - dispix) / 2;
		}
		trow = strlen / col + (strlen % col ? 1 : 0);
		if (trow > row)
			trow = row;

		if (dy > trow * 16)
		{
			vdy = (dy - trow * 16) / 2;
		}

		DrawUnicodeMulStr1(x + vdx, y + vdy, sheet, p, col, row, color);
	}
}
//------------------------------------------
// Function   : U32 GetAng(S32 dx, S32 dy)
// Description: 求夹角度
//------------------------------------------
U32 GetAng(S32 dx, S32 dy)
{
	F32 ax, ay;
	U32 ang = 0;

	ax = dx;
	ay = dy; // S32转F32为代入atan用
	if (dx == 0)
	{
		if (dy > 0)
			ang = 900;
		else
			ang = 2700; //
	}
	else if (dy == 0)
	{
		if (dx < 0)
			ang = 1800; //
	}
	else if ((dx > 0) && (dy >= 0))
	{ // 第I象限
		ang = (U32)(atan(ay / ax) * 1800 / 3.14159);
	}
	else if ((dx <= 0) && (dy > 0))
	{ // 第II象限
		ang = (U32)(atan(-ax / ay) * 1800 / 3.14159) + 900;
	}
	else if ((dx < 0) && (dy <= 0))
	{ // 第III象限
		ang = (U32)(atan(ay / ax) * 1800 / 3.14159) + 1800;
	}
	else if ((dx > 0) && (dy <= 0))
	{ // 第IV象限
		ang = (U32)(atan(-ax / ay) * 1800 / 3.14159) + 2700;
	}
	return (ang);
}

U8 BCD_2Hex(U8 val)
{
	U8 value;

	value = ((val >> 4) * 10) + (val & 0x0f);
	return value;
}

U8 Hex_2BCD(U8 val)
{
	U8 value;

	value = ((val / 10) << 4) + ((val % 10) & 0x0f);
	return value;
}

void DisTestData(U32 x1, U32 y1, S32 d1, S32 d2, U8 f, U32 color)
{
	DrawFS32P(x1, y1, d1, 6, 0, color);
	DrawFS32P(x1 + 60, y1, d2, 6, 0, color);
	if (f)
	{
		while (WWT_PENIRQ)
			wdt();
		while (WWT_PENIRQN)
			wdt();
	}
}

const U8 bmpdat[] = {
	0x00};
//------------------------------------------
// Function   : void SetDrawXY(U16 xx0, U16 yy0, U16 xx1, U16 yy1, U8 ff)
// Description: 设置限制显示的范围
//------------------------------------------
void SetDrawXY(U16 xx0, U16 yy0, U16 xx1, U16 yy1, U8 ff)
{
	DrawLimtf = ff;
	Drawmx0 = xx0;
	Drawmx1 = xx1;
	Drawmy0 = yy0;
	Drawmy1 = yy1;
}

//------------------------------------------
// Function   : void U8 CheckTextLen(U16 *p,U16 SheetNO,U16 lines)
// Description: 探测显示文字长度
//------------------------------------------
U8 CheckTextLen(U16 *p, U16 SheetNO, U16 lines)
{
	U8 uLen = 0;
	U16 c;
	U8 c1, c2;
	if (P_LAN < 2)
	{ //中英文
		c = (*p);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
		while ((c1 != 0) && (c2 != 0))
		{
			uLen++;
			c = *(p + uLen);
			c1 = c & 0xff;
			c2 = (c >> 8) & 0xff;
		} //探测字串长度
		if ((c1 > 0) && (c2 == 0))
			uLen++;
		uLen *= 2; //返回英文字符个数
	}
	else
	{ //多国语言	后面做
	}
	return uLen;
}
//------------------------------------------
// Function   : void ReadPointData1(U32 x, U32 y)
// Description: 读取指定点的数据，含周围 18px内的像素
// note: 为了绘制花样功能标识符时，不将周围数据覆盖
//------------------------------------------
void ReadPointData1(U32 x, U32 y)
{
	U8 i, j;
	if (x > LCD_width || y > LCD_height)
		return;
	//printf("ReadPointData1(%d,%d)\r\n",x,y);
	g_dispat1.IsDat = 1;
	g_dispat1.ox = x;
	g_dispat1.oy = y;
	//printf("g_dispat1.ox = %d  g_dispat1.oy = %d\r\n",g_dispat1.ox,g_dispat1.oy);
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 18; j++)
		{
			g_dispat1.ppc[i][j] = ReadPixel(j + g_dispat1.ox, i + g_dispat1.oy);
		}
	}
}
//------------------------------------------
// Function   : void RecoveData1(void)
// Description: 覆盖针点1的数据，读取指定点周围 18px内的像素
//------------------------------------------
void RecoveData1(void)
{
	U8 i, j;
	if (g_dispat1.IsDat == 0)
		return;
	g_dispat1.IsDat = 1;
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 18; j++)
		{
			PutPixel(j + g_dispat1.ox, i + g_dispat1.oy, g_dispat1.ppc[i][j]);
		}
	}
	g_dispat1.IsDat = 0;
	wdt();
}

//------------------------------------------
// Function   : void ResetData1(void)
// Description: 复位针点1的数据，读取指定点周围 18px内的像素
//------------------------------------------
void ResetData1(void)
{
	if (g_dispat1.IsDat == 1)
		g_dispat1.IsDat = 0;
	wdt();
}
//------------------------------------------
// Function   : void ReadPointData2(U32 x, U32 y)
// Description: 读取指定点的数据，含周围 18px内的像素
//------------------------------------------
void ReadPointData2(U32 x, U32 y)
{
	U8 i, j;
	if (x > LCD_width || y > LCD_height)
		return;
	g_dispat2.IsDat = 1;
	g_dispat2.ox = x;
	g_dispat2.oy = y;
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 18; j++)
		{
			g_dispat2.ppc[i][j] = ReadPixel(j + g_dispat2.ox, i + g_dispat2.oy);
		}
	}
	wdt();
}
//------------------------------------------
// Function   : void RecoveData2(void)
// Description: 覆盖针点2的数据，读取指定点周围 18px内的像素
//------------------------------------------
void RecoveData2(void)
{
	U8 i, j;
	if (g_dispat2.IsDat == 0)
		return;
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 18; j++)
		{
			PutPixel(j + g_dispat2.ox, i + g_dispat2.oy, g_dispat2.ppc[i][j]);
		}
	}
	g_dispat2.IsDat = 0;
	wdt();
}
//------------------------------------------
// Function   : void ResetData2(void)
// Description: 复位针点2的数据，读取指定点周围 18px内的像素
//------------------------------------------
void ResetData2(void)
{
	if (g_dispat2.IsDat == 1)
		g_dispat2.IsDat = 0;
	wdt();
}
//------------------------------------------
// Function   : void ReadPointData3(U32 x, U32 y)
// Description: 读取指定点的数据，含周围 18px内的像素
//------------------------------------------
void ReadPointData3(U32 x, U32 y)
{
	U8 i, j;
	if (x > LCD_width || y > LCD_height)
		return;
	//printf("ReadPointData3 \r\n");
	g_dispat3.IsDat = 1;
	g_dispat3.ox = x;
	g_dispat3.oy = y;
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 18; j++)
		{
			g_dispat3.ppc[i][j] = ReadPixel(j + g_dispat3.ox, i + g_dispat3.oy);
		}
	}
	wdt();
}
//------------------------------------------
// Function   : void RecoveData3(void)
// Description: 覆盖针点3的数据，读取指定点周围 18px内的像素
//------------------------------------------
void RecoveData3(void)
{
	U8 i, j;
	if (g_dispat3.IsDat == 0)
		return;
	//printf("RecoveData3 \r\n");
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 18; j++)
		{
			PutPixel(j + g_dispat3.ox, i + g_dispat3.oy, g_dispat3.ppc[i][j]);
		}
	}
	g_dispat3.IsDat = 0;
	wdt();
}

void Draw_Circle(U16 Xpos, U16 Ypos, U16 Radius, U32 Color)
{
	S16 mx = Xpos, my = Ypos, x = 0, y = Radius;
	S16 d = 1 - Radius;
	while (y > x)
	{
		PutPixel1(x + mx, y + my, Color);
		PutPixel1(-x + mx, y + my, Color);
		PutPixel1(-x + mx, -y + my, Color);
		PutPixel1(x + mx, -y + my, Color);
		PutPixel1(y + mx, x + my, Color);
		PutPixel1(-y + mx, x + my, Color);
		PutPixel1(y + mx, -x + my, Color);
		PutPixel1(-y + mx, -x + my, Color);
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}

void Draw_FullCircle(U16 Xpos, U16 Ypos, U16 Radius, U32 Color)
{
	U16 x, y, r = Radius;
	for (y = Ypos - r; y < Ypos + r; y++)
	{
		for (x = Xpos - r; x < Xpos + r; x++)
		{
			if (((x - Xpos) * (x - Xpos) + (y - Ypos) * (y - Ypos)) <= r * r)
			{
				PutPixel1(x, y, Color);
			}
		}
	}
}

void Draw_FullPoint(U16 Xpos, U16 Ypos, U16 Radius, U32 Color)
{
	// Draw_FullCircle(Xpos, Ypos, Radius, Color);
	DrawPoint(Xpos,Ypos,Color);
}

void Draw_BigCross(U16 x, U16 y, U16 color)
{
	DrawBigPoint(x, y, color);
}
void Draw_BigCrossLimit(U16 x, U16 y, U16 color,U8 convas)
{
	pat_setDrawXY(convas,0,0);
	DrawBigPoint(x, y, color);
	pat_resetDrawXY();
}

void Draw_SmallCross(S32 x, S32 y, U16 color)
{
	DrawCross7x7(x, y, color);
}

void Draw_SmallCrossLimit(S32 x, S32 y, U16 color,U8 convas)
{
	pat_setDrawXY(convas,0,0);
	DrawCross7x7(x, y, color);
	pat_resetDrawXY();
}

void Draw_Code_BMP(U32 x, U32 y, U32 dx, U32 dy, U32 num)
{
	U8 enable = 0;
	if (PAT_CODE_ALL)
	{
		switch (num)
		{
		case BMP_H:
			if (PAT_CODE_H)
				enable = 1;
			break;
		case BMP_H2:
			if (PAT_CODE_H2)
				enable = 1;
			break;
		case BMP_P:
			if (PAT_CODE_P)
				enable = 1;
			break;
		case BMP_U:
			if (PAT_CODE_U)
				enable = 1;
			break;
		case BMP_D:
			if (PAT_CODE_D)
				enable = 1;
			break;
		case BMP_J:
			if (PAT_CODE_J)
				enable = 1;
			break;
		case BMP_C:
			if (PAT_CODE_C)
				enable = 1;
			break;
		case BMP_T:
			if (PAT_CODE_T)
				enable = 1;
			break;
		case BMP_F1:
			if (PAT_CODE_F1)
				enable = 1;
			break;
		case BMP_F2:
			if (PAT_CODE_F2)
				enable = 1;
			break;
		case BMP_F3:
			if (PAT_CODE_F3)
				enable = 1;
			break;
		case BMP_F4:
			if (PAT_CODE_F4)
				enable = 1;
		case BMP_F5:
			if (PAT_CODE_F5)
				enable = 1;
		case BMP_F6:
			if (PAT_CODE_F6)
				enable = 1;
		case BMP_F7:
			if (PAT_CODE_F7)
				enable = 1;
		case BMP_F8:
			if (PAT_CODE_F8)
				enable = 1;
			break;
		case BMP_FX:
			if (PAT_CODE_FX)
				enable = 1;
			break;
		default:
			break;
		}
	}
	if (enable)
	{
		DrawFPBMPBTN2(x - 7, y - 7, dx, dy, num);
	}
}

void Draw_Code_HSEP(S32 x, S32 y, U8 n, U32 color)
{
	DrawSerialNumber(x, y, n, 1, color, 1);
}

void Draw_Code_Num(S32 x, S32 y, U16 n, U32 color)
{
	DrawSerialNumber2(x, y, n, 1, color);
}

