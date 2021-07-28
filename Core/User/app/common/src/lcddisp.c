//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  lcddisp.c								*
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

extern U8 PatternType[900]; //�������� 1�������  2���ֵܿ�   ����2016-11-29
extern volatile U32 g_bmplen;
extern const U32 D_Address[];
extern BOOTSCR g_bootscr;
extern U8 getWeekDay(U16 y, U8 m, U8 d);

volatile U32 flashver, flashdate; //flash�汾������
volatile U32 mp3dver, mp3ddate;	  //MP3�汾������
volatile U32 BufAdd;			  //�����ַ
volatile U8 Reflash = 0;		  //ҳ��ˢ�±�־
U16 *pMem;						  //volatile

U16 SheetPage; //�������Sheet��ҳ�룬��0��ʼ
U16 *MEM_TITLE_ADD;
U16 *MEM_TITLE_AUDIO_ADD;
U32 TitleLengthMax;
U32 TitleAudioLengthMax;
U16 g_lang;

OLDPAT1 g_dispat1;		 //       ��ȡ�����
OLDPAT1 g_dispat2;		 //ģʽһ ��ǰ���
OLDPAT1 g_dispat3 = {0}; //ģʽһ ��λ���

volatile U32 tmpflashadd[200];//flash�������ݵ�ַ
volatile U8 flashdataflag[200];//flash����ʹ�ܱ�־

//��ŵ���ʾ�����x,y����
volatile U8 DrawLimtf = 0; //������ʾ��Χ��־��
volatile U16 Drawmx0 = 0;
volatile U16 Drawmy0 = 0;
volatile U16 Drawmx1 = XLEN;
volatile U16 Drawmy1 = YLEN;

extern volatile U8 g_dataType;
extern volatile U8 g_hourType;
extern volatile U8 g_logdir;
extern volatile U8 g_vs1003err;
extern volatile U32 Timerms1; //���㲥���������ʱ��

extern volatile U8 g_BarcodeFlag[MAXFILENO - MINFILENO];
extern U8 Read_D(U32 Dn, U32 len, U16 *add);

extern const unsigned char msgTest[][65];

const U16 keypicdata[] = {
	Color_Silver,	  // RGB(192,192,192)	����ɫ	   Color_Gainsboro	 Color_LightGray	Color_Silver
	Color_BlueViolet, // RGB(138,43,226)	������������ɫ
	Color_KeyBlue,	  // RGB(0,0,255)		��ɫ	 Color_Blue
	Color_Gray,		  // RGB(128,128,128)	��ɫ
	Color_Gainsboro,  //
	Color_Green,	  // RGB(0,255,0)		��ɫ
	Color_Red,		  // RGB(255,0,0)		��ɫ
	RGB(250, 250, 250),
	Color_Silver, // RGB(192,192,192)	����ɫ	   Color_Gainsboro	 Color_LightGray	Color_Silver
};
//����ɫɫͼ
const U16 keypicdatb[][3] = {
	// Color_White
	RGB(245, 245, 245), RGB(230, 230, 230), RGB(223, 223, 223), // Color_Silver	RGB(192,192,192)		����ɫ
	RGB(227, 205, 248), RGB(199, 155, 241), RGB(171, 105, 234), // Color_BlueViolet	RGB(138,43,226)		������������ɫ
	RGB(210, 210, 248), RGB(150, 150, 241), RGB(100, 100, 234), // Color_Blue	RGB(0,0,255)			��ɫ
	RGB(235, 235, 235), RGB(200, 200, 200), RGB(160, 160, 160), // Color_Gray	RGB(128,128,128)		��ɫ
	RGB(240, 240, 240), RGB(225, 225, 225), RGB(210, 210, 210), // Color_Silver	RGB(192,192,192)		����ɫ
	RGB(200, 248, 200), RGB(150, 243, 150), RGB(100, 238, 100), // Color_Green 	RGB(0,255,0)			��ɫ
	RGB(248, 200, 200), RGB(243, 150, 150), RGB(238, 100, 100), // Color_Red	RGB(255,0,0)			��ɫ
	RGB(240, 240, 240), RGB(150, 150, 150), RGB(120, 120, 120), // Color_Silver	RGB(192,192,192)		����ɫ
	RGB(245, 245, 245), RGB(230, 230, 230), RGB(223, 223, 223), // Color_Silver1	RGB(192,192,192)		����ɫ
};
//����ɫɫͼ
const U16 keypicdatc[][3] = {
	// Color_Black
	RGB(80, 80, 80), RGB(120, 120, 120), RGB(150, 150, 150),	// Color_Silver		RGB(192,192,192)	����ɫ
	RGB(43, 13, 80), RGB(73, 23, 140), RGB(105, 33, 190),		// Color_BlueViolet	RGB(138,43,226)		������������ɫ
	RGB(30, 30, 120), RGB(55, 55, 170), RGB(75, 75, 220),		// Color_Blue	RGB(0,0,255)			��ɫ
	RGB(30, 30, 30), RGB(60, 60, 60), RGB(90, 90, 90),			// Color_Gray	RGB(128,128,128)		��ɫ
	RGB(80, 80, 80), RGB(120, 120, 120), RGB(150, 150, 150),	// Color_Silver		RGB(192,192,192)	����ɫ
	RGB(22, 90, 22), RGB(40, 140, 40), RGB(50, 185, 50),		// Color_Green 	 RGB(0,255,0)			��ɫ
	RGB(90, 22, 22), RGB(140, 40, 40), RGB(185, 50, 50),		// Color_Red	RGB(255,0,0)			��ɫ
	RGB(80, 80, 80), RGB(100, 100, 100), RGB(120, 120, 120),	// 			��ɫ
	RGB(120, 120, 120), RGB(150, 150, 150), RGB(211, 211, 211), // Color_Silver		RGB(192,192,192)	����ɫ
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
extern volatile U8 g_upinfo[0x20];//#define UP_ADDR 7��#define OS_ADDR 12��#define FL_ADDR 16��#define MP_ADDR 22
extern OLDPAT1 g_dispat1;
extern void ReadPointData(U32 x, U32 y);
extern U8 isUpInfoData(U8 *dat);
extern void creatUpInfoData(U8 *outDat);
U8 CountUnicodeInfo(U16 *addr, U16 Sheetcnt, U16 linenum, U16 langsnum, U32 *outoffest, U16 *outstrlen);
//------------------------------------------
// Function   : void initflashadd(void)
// Description: ��ʼ��������flash�е�ַ
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
	//��ʼ��g_upinfo
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
	//		PlaySound(CMD_PLAY,2,1);		// ��������
	//	}
	//#endif
	//
	//#if DIS_LOGO ==1
	//	//printf("--g_vs1003err = %d \r\n",g_vs1003err);
	//	if(g_vs1003err==0){
	//		PlaySound(CMD_PLAY,2,1);		// ��������
	//	}
	//#endif

	if (g_bootscr.mode != 3)
	{
		if (g_vs1003err == 0)
		{
			PlaySound(CMD_PLAY, 2, 1); // ��������
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

	W25QXX_Read(W25Q1_DAT, (U8 *)buff, FLASHINDEXADDR, 24);//��ȡflash�����İ汾

	if (strncmp((char *)buff, (char *)Check_FLASH, 12) == 0)//�жϰ汾/�ͺ�/�����Ƿ�Ϲ��Ƿ�һ��
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

				for (i = 1; i <= BINFILE_ALL; i++) //����Ƿ�У�鶼ͨ������ͨ���ض�5��У��
				{
					if (flashdataflag[i] == FALSE)
					{
						break;
					}
				}
				if (i > BINFILE_ALL)
				{
					g_upinfo[FL_ADDR] = '0'; //����Ҫ����
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

	if ((flashver == 0) && (flashdate == 0))// FLASH������У�����,���������ݳ�ʼ��ַ��ʼ����������������,��ֹ�����ַ����
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
			if ((flashdataflag[i] == FALSE) && (i < BINFILE_ALL))//���õ�flash����ָ���׵�ַ
			{
				dadd = (U16 *)tmpflashadd[i];
				for (cnt = tmpflashadd[i]; cnt < tmpflashadd[i + 1]; cnt += 2)
				{
					(*dadd) = 0;
					dadd++;
				}
			}
			else if ((flashdataflag[i] == FALSE) && (i == BINFILE_ALL))//���һλ
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
// Description: ָ������дҳ�� �Դ�һ������СΪ0x400000 / 0x96000 = 6ҳ
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
// Description: ָ����ʾҳ�� SDRAM = 16MW = 0x1000000 ���ĸ���
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
// Description: ����ҳ��
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
// Description: ճ��ҳ��(U16 *)	(U16 *)
//------------------------------------------
void PageWritern(U8 des_no, U8 org_no)
{
	SDRAM_memcpy((LCD_VRAM_BASE_ADDR + des_no * DISP_PAGE_SIZE), (LCD_VRAM_BASE_ADDR + org_no * DISP_PAGE_SIZE), DISP_PAGE_SIZE);
}

//------------------------------------------
// Function   : void CLR_Screen(U32 color)
// Description: ����
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
// Description: ����
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
// Description: У��ʱ��ʾ��ǰ��
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

const U8 Arrow[6] = {0xf8, 0xe0, 0xf0, 0xb8, 0x9c, 0x0c}; // 6x6��ͷͼƬ
//------------------------------------------
// Function   : void DrawIconon(U16 x, U16 y, U32 Color)
// Description: У��ʱ��ʾ��ǰ��
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
// Description: ���У��ʱ��ʾ��ǰ��
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

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ��������
// note : percent��ǰ����
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
// Description: �����(�ñ���ͼƬ)
// note������ͼ����ͼ��ʱ�������ַ�������ָ��������ʾ�������������
//------------------------------------------
void BmpFillRect(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 x2, y2;
	U32 p, i, j;
	U32 srcadd = LCD_VRAM_BASE_ADDR + VMAXPAG * DISP_PAGE_SIZE;
#if VIEWTYPE == HORIZONTAL //����
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
// Description: �������
// note��ɾ��������ʾ
//------------------------------------------
void KeyFillBmp(KEYPAD bn)
{
	BmpFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize);
}

//------------------------------------------
// Function   : void PutPixel(U32 x, U32 y,U32 color)
// Description: �����
// note��������С���ƣ������������Ļ��
//------------------------------------------
void PutPixel(U32 x, U32 y, U32 color)
{
#if VIEWTYPE == HORIZONTAL //����
	*((U16 *)(BufAdd + y * YOFFSET + x + x)) = color;
#else
	*((U16 *)(BufAdd + x * XOFFSET + y + y)) = color;
#endif
}
//------------------------------------------
// Function   : void PutPixel(U32 x, U32 y,U32 color)
// Description: �����
// note������С����
//------------------------------------------
void PutPixel1(S32 x, S32 y, U32 color)
{
#if VIEWTYPE == HORIZONTAL //����
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
//// Description: ��������
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
// Description: ��������
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
// Description: ���һ��2*2�ĵ㣬���ê��Ϊ(0,0)
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
// Description: ˮƽʵ��
//------------------------------------------
void DrawHLine(S32 x, S32 y, U32 dx, U32 color)
{
	S32 i, x2;
#if VIEWTYPE == HORIZONTAL //����
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
// Description: ��ֱʵ��
//------------------------------------------
void DrawVLine(S32 x, S32 y, U32 dy, U32 color)
{
	S32 i, y2;
#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ˮƽʵ��
// note: �Թ�ָ������ɫ���Ƹ�ֱ��
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
// Description: ��ֱʵ��
// note: �Թ�ָ������ɫ���Ƹ�ֱ��
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

#define DISP_PARA_X 1 //x��������
#define DISP_PARA_Y 1 //y��������
//------------------------------------------
// Function   : void DrawLine(U32 x1,U32 y1,U32 x2,U32 y2,U32 color)
// Description: ���� A������--B������
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
// Description: ����
// note: �Թ�ָ������ɫ���Ƹ�ֱ��
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
// Description: ����3*3�ĵ�
//------------------------------------------
void DrawPoint(S32 x, S32 y, U16 color)
{

	DrawHLine(x - 1, y, 3, color);
	DrawVLine(x, y - 1, 3, color);
}
//------------------------------------------
// Function   : DrawCross7x7(S32 x, S32 y, U16 color)
// Description: ����7*7��ʮ�ֹ��
//------------------------------------------
void DrawCross7x7(S32 x, S32 y, U16 color)
{

	DrawHLine(x - 4, y, 9, color);
	DrawVLine(x, y - 4, 9, color);
}

//------------------------------------------
// Function   : void DrawRect(U32 x,U32 y,U32 dx,U32 dy,U32 color)
// Description: ��泤����
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
// Description: ˮƽ����
// note: ���� ʵ���Ϊ  4:2 ����
//------------------------------------------
void DrawHDottedLine(S32 x, S32 y, U32 dx, U32 color)
{
	S32 i, x2;
	U16 j = 0;
#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ��ֱ����
// note: ���� ʵ���Ϊ  4:2 ����
//------------------------------------------
void DrawVDottedLine(S32 x, S32 y, U32 dy, U32 color)
{
	S32 i, y2;
	U16 j = 0;
#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ������߳�����
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
// Description: �������ɫ
// note: ����DrawLimtfȷ���Ƿ���泬����Χ����״
//------------------------------------------
void DrawFillRect(U32 x, U32 y, U32 dx, U32 dy, U32 color)
{
	U32 i, j, x2, y2;
#if VIEWTYPE == HORIZONTAL //����
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

#else //����
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
// Description: �������ɫ
// note: ��������ʾ��Χ
//------------------------------------------
void DrawFillRect1(U32 x, U32 y, U32 dx, U32 dy, U32 color)
{
	U32 i, j, x2, y2;
#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ��˫�߿�
// note: ���ƴ�����ߵ�ʵ�߷���
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
// Description: ����
// note: ����ʵ�߷��򣬹���ʼ�������ɫΪc1�����ԳƵ������ɫΪc2
// �簴ť�ı߿򽥱�
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
// Description: ����
// note: ����ʵ�߷��򣬹���ʼ�������ɫΪc1�����ԳƵ������ɫΪc2
// �÷���� ��ʼ�� ����ɫ Ϊ Color_LightGray
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
// Description: ���������η���	w:1,2,3,4=����,����,����,����
// note�� d:����ı߳���dt:�����Ǳ߳�   d>dt
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
// Description: ��������������ɫ	w:1,2,3,4=����,����,����,����
// note�� d:����ı߳���dt:�����Ǳ߳�   d>dt   ���˴���ͨ������N���ߵķ�ʽ������䡿
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
// Description: ͼ�α��
//------------------------------------------
void DrawColor_Gray(U32 x, U32 y, U32 dx, U32 dy)
{

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
	U16 data2;
	U32 data3;
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;
	U32 dadd;
	//printf("DrawColor_Gray ����δʵ��!!!");
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
// Description: ָ������ ͼ��ȡ��
// ��565��ʽ�£�������ֱ�Ϊ��R 0xF800(1111 1000 0000 0000)��G 0x07E0(0000 0111 1110 0000)��B 0x001F(0000 0000 0001 1111)
//------------------------------------------
void DrawColorRreverse(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;

	U16 data2;
	U32 data3;

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ָ������ ͼ�α��
//------------------------------------------
void DrawColor_Gray1(U32 x, U32 y, U32 dx, U32 dy)
{
	U32 i, x2, y2;
	U16 col_r, col_g, col_b;

	U16 data2;
	U32 data3;

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
// Description: ͼ����ʾ			  198mS		154mS
// note : ԭͼ��ʾ
//------------------------------------------
void DrawBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
{
#if VIEWTYPE == HORIZONTAL //����
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
// Description: ͼ����ʾ			  198mS		154mS
// note : �޳���ɫ��ʾ
//------------------------------------------
void DrawBMP1(U32 x, U32 y, U32 dx, U32 dy, U16 *p)
{
#if VIEWTYPE == HORIZONTAL //����
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
// Description: ͼ����ʾ			  198mS		154mS
// note : R��B��ɫ
// ��565��ʽ�£�������ֱ�Ϊ��R 0xF800(1111 1000 0000 0000)��G 0x07E0(0000 0111 1110 0000)��B 0x001F(0000 0000 0001 1111)
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
			dat = (dat >> 11) | (dat << 11) | (dat & 0x7e0); //0x7e0λ16λBMP������һ����ɫ��ֵ 565��ʽ
			PutPixel(j, i, dat);
			p++;
		}
	}
}

//------------------------------------------
// Function   : void DrawPBMP(U32 x, U32 y, U32 dx, U32 dy, U8 *p)
// Description: ͼ����ʾ����ѹ������
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
// Description: ͼ����ʾ����ѹ������
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
// Description: ͼ����ʾ����ѹ������
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
// Description: ��ɫ����ͼ����ʾ
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
// Description: ͼ����ʾ����ѹ�����ݣ���ȥ����ɫ������ʾ
// note:������ʾ��ͼΪ��ɫ��ͼƬh��gif
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
	{ // ������ַ
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // ż����ַ
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
// Description: ͼ����ʾ����ѹ�����ݣ���ȥ����ɫ������ʾ��ͼ��һ�
// note:������ʾ��ͼΪ��ɫ��ͼƬh��gif��
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
	{ // ������ַ
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // ż����ַ
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
// Description: ͼ����ʾ����ѹ�����ݣ���ȥ����ɫ������ʾ
// note:������ʾ��ͼΪ��ɫ��ͼƬh��gif
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
		return; //ͼ����ʼ��ַ����Χ

	if ((i % 2) == 1)
	{ // ������ַ
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // ż����ַ
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
// Description: ͼ����ʾ����ѹ�����ݣ�
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
		return; //ͼ����ʼ��ַ����Χ

	if ((i % 2) == 1)
	{ // ������ַ
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // ż����ַ
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
// Description: ͼ����ʾ����ѹ�����ݣ�
// note:������ʾλ�ô�С����
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
		return; //ͼ����ʼ��ַ����Χ

	if ((i % 2) == 1)
	{ // ������ַ
		i >>= 1;
		p += i;
		dat = (*p) >> 8;
		p++;
		f = 1;
	}
	else
	{ // ż����ַ
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
// Description: ��ʾ���� ʱ��
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
// Description: 16*8������� ֱ�Ӹ���16�������ݽ��л�ͼ
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
// Description: 24*12�������  ֱ�Ӹ���16�������ݽ��л�ͼ
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
// Description: 24*24ASCII���  ֱ�Ӹ���16�������ݽ��л�ͼ
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
// Description: 32*16�������  ֱ�Ӹ���16�������ݽ��л�ͼ
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
// Description: 16*8������� ASCLL
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
// Description: 16*16�ַ������ ASCLL
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
// Description: 24*12������� ASCLL
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
// Description: 24*24������� ASCLL
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
// Description: 32*16������� ASCLL
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
// Description: 16*16�������(���ֿ�) CHINESE  ����
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
// Description: 24*24�������(���ֿ�) CHINESE   ����
//------------------------------------------
void DrawFHz24X24(U32 x, U32 y, char c1, char c2, U32 color)
{
	U32 i, j;
	U32 c = 0;
	U16 *p;
	U16 dat, b, bb;
	U32 xx, yy;

	p = MEM_HzLib_CHS24X24SA; //(U16 *)(HzLib_CHS24X24SAADD);//(U16 *)HzLib_CHSK24X24;//	// ʹ�������ֿ�
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
// Description: 24*24�������(���ֿ�) CHINESE  ����
//------------------------------------------

void DrawFHz24X24S(U32 x, U32 y, char c1, char c2, U32 color)
{
	U32 i, j;
	U32 c = 0;
	U16 *p;
	U16 dat, b, bb;
	U32 xx, yy;

	p = MEM_HzLib_CHS24X24KA; //(U16 *)(HzLib_CHS24X24KAADD);//(U16 *)HzLib_CHSK24X24;//	// ʹ�ÿ����ֿ�
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
// Description: 16*16���ּ�16x8�ַ������
// note: �Զ��ж�ASCLL���ַ� ����������Ϊ1px
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
	}; //̽���ִ�����
	while (i < uLen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ
		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFChar(x, y, s, color);
			i += 1;
			x += 9; // ��һ���ַ�
		}
		else
		{								  // ����
										  //			DrawHz(x, y, c1, c2, color);		// ���ֿ���lcdfont_data.c
			DrawFHz(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 17; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFStr16(U32 x, U32 y, char *str, U32 color)
// Description: 16*16���ּ�16x8�ַ������
// note: �Զ��ж�ASCLL���ַ� ����������Ϊ0px
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
	}; //̽���ִ�����
	while (i < uLen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ
		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFChar(x, y, s, color);
			i += 1;
			x += 8; // ��һ���ַ�
		}
		else
		{								  // ����
										  //			DrawHz(x, y, c1, c2, color);		// ���ֿ���lcdfont_data.c
			DrawFHz(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 16; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void Draw24X24FStr(U32 x, U32 y, char *str, U32 color)
// Description: 24*24���ּ�24x12�ַ������   ����Ϊ����
// note: �Զ��ж�ASCLL���ַ� ����������Ϊ2px
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
	}; //̽���ִ�����
	while (i < uLen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ

		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFASC12(x, y, s, color);
			i += 1;
			x += 13; // ��һ���ַ�
			reslen++;
		}
		else
		{									   // ����
			DrawFHz24X24(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 26; // ��һ���ַ�
			reslen += 1;
		}
	}

	wdt();
	return reslen;
}

//------------------------------------------
// Function   : void Draw32X32FStr(U32 x, U32 y, char *str, U32 color)
// Description: 24*24���ּ�32x16�ַ������
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
	}; //̽���ִ�����
	while (i < uLen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ
		if (c1 <= 128)
		{ // ASCII
			s[0] = c1;
			s[1] = 0;
			DrawFASC3216(x, y, s, color);
			i += 1;
			x += 20; // ��һ���ַ�
		}
		else
		{									   // ����
			DrawFHz24X24(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			//DrawFHz24X24S(x, y+32, c1, c2, color);		// �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 40; // ��һ���ַ�
		}
	}
}
//------------------------------------------
// Function   : void DrawFStr(U32 x, U32 y, char *str, U16 color)
// Description: 32*32���ּ�32x16�ַ������
// note: ��32*32�ֿ�   ��ӳ�䵽 Draw24X24FStr
//------------------------------------------
void DrawFStr32(U32 x, U32 y, char *str, U32 color)
{
	//	U8  c1;
	//	U8 c2;
	//	U8  i, uLen;
	//	char s[3];

	//	i = 0; uLen = 0;
	//	while (str[uLen]!=0) {uLen++;};	//̽���ִ�����
	//	while (i<uLen){
	//		c1 = str[i];				// ���ֵͰ�λ
	//		c2 = str[i+1];				// ���ָ߰�λ
	//		if (c1 <= 128) 				// ASCII
	//		{
	//			s[0] = c1; s[1] = 0;
	//			DrawFChar32(x, y, s, color);
	//			i += 1;
	//			x += 16;				// ��һ���ַ�
	//		} else {					// ����
	//			DrawHz32(x, y, c1, c2, color);		// ���ֿ���lcdfont_data.c
	//			i += 2;
	//			x += 32;				// ��һ���ַ�
	//		}
	//	}
	Draw24X24FStr(x, y, str, color);
}

//------------------------------------------
// Function   : void Draw24x24StrSinLineLimt(U16 x,U16 y,U16 p)
// Description: FLASH�е��ַ���ʾ��һ����ʾ 24x12����,����ʾ�ַ���������
// U16 x,y :��ʾ�������
//U16 SheetNO:�������Sheet��ҳ�� ��0��ʼ.
// U16 p:Sheet������
// U8 linemus :ÿ����ʾ�ַ�����
// U8 maxline:�����ʾ�����У��������ݲ���ʵ Ϊ0ʱ�Զ���ʾȫ���ַ�
// return : ��ʾ����
//------------------------------------------
void Draw24x24StrSinLineLimt(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U32 color)
{
	if (lmax == 0)
		return;
	if (P_LAN < 2)
	{ //��Ӣ��
		DrawFMultiLStr24LimtL(x, y, p, lmax, 1, color);
	}
	else
	{ //����������
		DrawUnicodeMulStr(x, y, SheetNO, lines, lmax, 1, color);
	}
}

//------------------------------------------
// Function   : DrawCen24x24StrSinLineLimt(U16 x,U16 y,U16 *p,U16 SheetNO,U16 lines,U8 lmax,U32 color)
// Description: FLASH�е��ַ���ʾ��һ�� ���� ��ʾ 24x12����,����ʾ�ַ���������
// note: �ɻ�����ʾ
//------------------------------------------
void DrawCen24x24StrSinLineLimt(U16 x, U16 y, U16 dx, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U32 color)
{
	U8 *addr;
	U32 stroffest;
	U16 strlen = 0;
	U16 vx = 0, vdx = 0;
	if (lmax == 0)
		return;
	//̽����ʾ�ַ�����
	if (P_LAN < 2)
	{ //��Ӣ��
		addr = (U8 *)p;
		while ((*addr) != 0)
		{
			addr++;
			strlen++;
		}
	}
	else
	{ //����������
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
	{ //��Ӣ��
		DrawFMultiLStr24LimtL(x + vdx, y, p, lmax, 1, color);
	}
	else
	{ //����������
		DrawUnicodeMulStr(x + vdx, y, SheetNO, lines, lmax, 1, color);
	}
}

//------------------------------------------
// Function   : void Draw24X24FStrByLine(U32 x, U32 y, char *str, U32 color)
// Description: 24*24���ּ�24x12�ַ������ ��ָ������ַ���
// note: �ɻ�����ʾ
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
	}; //̽���ִ�����
	while (i < uLen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ

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
			x += 13; // ��һ���ַ�
			charnum++;
		}
		else
		{ // ����
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
			DrawFHz24X24(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 26; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void Draw16x16StrLineLimt(U16 x,U16 y,U16 p)
// Description: FLASH�е��ַ���ʾ���Զ����У���ʾ 16x16,����ʾ�ַ���������
//------------------------------------------
void Draw16x16StrLineLimt(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U8 mlines, U32 color)
{
	if (lmax == 0)
		return;
	if (P_LAN < 2)
	{ //��Ӣ��
		Draw16X16FStrByLine(x, y, (char *)p, lmax, mlines, color);
	}
	else
	{ //����������
		DrawUnicodeMulStr1(x, y, SheetNO, lines, lmax, mlines, color);
	}
}
//------------------------------------------
// Function   : void Draw24X24FStrByLine(U32 x, U32 y, char *str, U32 color)
// Description: 16*16���ּ�16x8�ַ������ ��ָ������ַ���
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
	}; //̽���ִ�����
	if (lines)
	{ //�趨��ʾ����
		if (uLen > maxchar * lines)
		{
			uLen = maxchar * lines;
		}
	}
	while (i < uLen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ

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
			x += 8; // ��һ���ַ�
			charnum++;
		}
		else
		{ // ����
			charnum += 2;
			if (charnum >= maxchar)
			{
				y += 18;
				x = xpos;
				charnum = 0;
			}
			DrawFHz(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 16; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFFStr(U32 x, U32 y, U16 *p, U32 color)
// Description: FLASH�е�16*16���ּ�16x8�ַ������
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
	} //̽���ִ�����
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{								  // ����
			DrawFHz(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			x += 16; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFFStr(U32 x, U32 y, U16 *p, U32 color)
// Description: FLASH�е�16*16���ּ�16x8�ַ�����棬���г�������
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
	} //̽���ִ�����
	if ((c1 > 0) && (c2 == 0))
		uLen++;
	if (uLen > n)
		uLen = n;
	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{								  // ����
			DrawFHz(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			x += 16; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void Draw24X24FFStr(U32 x, U32 y, U16 *p, U32 color)
// Description: 24*24���ּ�24x12�ַ������
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
	} //̽���ִ�����
	if ((c1 > 0) && (c2 == 0))
		uLen++;

	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{									   // ����
			DrawFHz24X24(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			x += 26; // ��һ���ַ�
			reslen += 2;
		}
	}

	wdt();
	return reslen;
}

//------------------------------------------
// Function   : void Draw24X24FFStrN(U32 x, U32 y, U16 *p, U32 n, U32 color)
// Description: 24*24���ּ�24x12�ַ�����棬ָ������Ϊ16λ
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
	} //̽���ִ�����
	if ((c1 > 0) && (c2 == 0))
		uLen++;
	if (uLen > n)
		uLen = n;
	while (i < uLen)
	{
		c = *(add + i);
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{									   // ����
			DrawFHz24X24(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			x += 26; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void Draw24X24FPY(U32 x, U32 y, char *str, U8 ulen, U32 color)
// Description: 24*24���ּ�24x12�ַ������,ֻ��ʾulen���Ƶ������ַ���ָ������Ϊ8λ
//------------------------------------------
void Draw24X24FPY(U32 x, U32 y, char *str, U8 ulen, U32 color)
{
	U8 c1, c2;
	U8 i;
	char s[3];

	i = 0;

	while (i < ulen)
	{
		c1 = str[i];	 // ���ֵͰ�λ
		c2 = str[i + 1]; // ���ָ߰�λ
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
			x += 13; // ��һ���ַ�
		}
		else
		{									   // ����
			DrawFHz24X24(x, y, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			//DrawFHz24X24S(x, y+32, c1, c2, color);		// �����ֿ�д��FLASH��ʹ��
			i += 2;
			x += 26; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH�е�16*16���ּ�16x8�ַ������  ����
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
	} //̽���ִ�����
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
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{ // ����
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 16;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			xx += 16;
			temp_Len += 2; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr16(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH�е�16*16���ּ�16x8�ַ������  ����
// return :����ʵ����ʾ������
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
	} //̽���ִ�����
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
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{ // ����
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 18;
				vlines++;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			xx += 18;
			temp_Len += 2; // ��һ���ַ�
		}
	}

	return vlines;
}

//------------------------------------------
// Function   : void DrawFMultiLStrR(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH�е�16*16���ּ�16x8�ַ������  ����(�Ҷ���)
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
	} //̽���ִ�����
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
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{ // ����
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 16;
				temp_Len = 0;
			}
			DrawFHz(xx, yy, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			xx += 16;
			temp_Len += 2; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr24(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH�е�24*24���ּ�24x12�ַ������  ���� �������޶�
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
	} //̽���ִ�����
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
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{ // ����
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 28;
				temp_Len = 0;
			}
			DrawFHz24X24(xx, yy, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			xx += 26;
			temp_Len += 2; // ��һ���ַ�
		}
	}
}

//------------------------------------------
// Function   : void DrawFMultiLStr24(U32 x, U32 y, U16 *p, U8 num,U32 color)
// Description: FLASH�е�24*24���ּ�24x12�ַ������  ����
// return :����ʵ����ʾ������
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
	} //̽���ִ�����
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
		c1 = c & 0xff;		  // ���ֵͰ�λ
		c2 = (c >> 8) & 0xff; // ���ָ߰�λ
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
		{ // ����
			if ((temp_Len + 2) > num)
			{
				xx = x;
				yy += 28;
				vlines++;
				temp_Len = 0;
			}
			DrawFHz24X24(xx, yy, c1, c2, color); // �����ֿ�д��FLASH��ʹ��
			i++;
			xx += 26;
			temp_Len += 2; // ��һ���ַ�
		}
	}

	return vlines;
}

//------------------------------------------
// Function   : void SetLanguage(U16 pn)
// Description: ����ѡ�������
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
	{ // Ӣ�Ĳ˵�
		MEM_TITLE_ADD = ((U16 *)(tmpflashadd[num + 50]));
		TitleLengthMax = TIT_MAXLENGTH_E;
	}
	else
	{ // ���Ĳ˵�
		MEM_TITLE_ADD = ((U16 *)(tmpflashadd[num]));
		TitleLengthMax = TIT_MAXLENGTH;
	}

	MEM_TITLE_AUDIO_ADD = ((U16 *)(tmpflashadd[num]));
	TitleAudioLengthMax = TIT_MAXLENGTH;
}

//------------------------------------------
// Function   : void getLanguage()
// Description: ��ȡѡ�������
//------------------------------------------
U16 getLanguage(void)
{
	return g_lang;
}

//------------------------------------------
//Function   : U8 CountUnicodeInfo(U16 *addr,U16 Sheetcnt,U16 linenum,U16 langsnum)
//Description: ������ʾ���ַ�����Ϣ
//U16 *addr:��ѯ��ַ
//U16 Sheetcnt:�����ѯ����  ��0��ʼ
//U16 linenum:�����ѯ������ ��0��ʼ
//U16 langsnum:�����ѯ�����Ա�� ��0��ʼ
//U32 *outoffest:�����ʾ�ַ�����¼Unicodeֵ����ʼ��ַƫ��ֵ
//U16 *outstrlen:��ʾ�ַ�������
//����ֵ �� 1�ɹ�/0ʧ��
//------------------------------------------
U8 CountUnicodeInfo(U16 *addr, U16 Sheetcnt, U16 linenum, U16 langsnum, U32 *outoffest, U16 *outstrlen)
{
	//	U32 len;
	U32 i;
	U16 LangTol, SheetTol;
	U32 FirstIndexAddr; //һ����������ʼ��ַ
	U32 SecondIndexAdd; //������������ʼ��ַ
	U32 StringStartAdd; //������������������ַ�����ʼ��ַ
	U32 StringAdd;		//��ʾ���ַ�������ʼ��ַ
	U16 Stringlen;		//��ʾ�ַ�������
	U16 Offset[TolLangs] = {0};
	//	U8 *p;
	//	U8 res;
	//	U16 StrCnt;

	if (*addr != 0x5246) //	��ʽ����
		return 0;

	//	len = (*(addr+3)<<16)| *(addr+2);
	//	p = (U8*)addr;

	SheetTol = *(addr + 4); //�����
	LangTol = *(addr + 5);	//����������
	if (Sheetcnt >= SheetTol)//���ұ�ų�����Χ
	{ 
		return 0;
	}
	if (LangTol > TolLangs)//����֧�ֵ�����������
	{ 
		return 0;
	}
	if (langsnum >= LangTol)//�������Ա�ų�����Χ
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

	StringAdd /= 2; //תΪ16λ
	Stringlen /= 2;

	*outoffest = StringAdd;
	*outstrlen = Stringlen;

	return 1;
}

//------------------------------------------
//Function   : void DrawUnicodeStr(U16 x,U16 y,U16 *p,U16 len,U32 color,U8 Font)
//Description: ��ʾUnicode��������
//U16 x,y : ��ʾxy�������
//U16 *p����ʾ�ַ�����ʼ��ַ
//U16 len:��ʾ����
//U32 color:��ʾ��ɫ
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
		{ //��֧�ֵ�����
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
//Description: ������ʾUnicode��������
//U16 x,y : ��ʾxy�������
//U16 *p����ʾ�ַ�����ʼ��ַ
//U16 len:��ʾ����
// U8 linemus :ÿ����ʾ�ַ�����
// U8 maxline:�����ʾ�����У��������ݲ���ʵ Ϊ0ʱ�Զ���ʾȫ���ַ�
//U32 color:��ʾ��ɫ
// return:ʵ����ʾ����
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
		{ //��֧�ֵ�����
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
		{ //����
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
//Description: ������ʾUnicode��������
//U16 x,y : ��ʾxy�������
//U16 *p����ʾ�ַ�����ʼ��ַ
//U16 len:��ʾ����
// U8 linemus :ÿ����ʾ�ַ�����
// U8 maxline:�����ʾ�����У��������ݲ���ʵ Ϊ0ʱ�Զ���ʾȫ���ַ�
//U32 color:��ʾ��ɫ
// return:ʵ����ʾ����
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
		{ //��֧�ֵ�����
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
		{ //����
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
// Description: FLASH�е��ַ���ʾ��Unicode��ʾ�ַ���
// note : DrawUnicodeStr24x24
// U16 x,y :��ʾ�������
//U16 SheetNO:�������Sheet��ҳ�� ��0��ʼ.
// U16 p:Sheet������
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
// Description: FLASH�е��ַ���ʾ��Unicode��ʾ�ַ���
// note : DrawUnicodeMulStr16x16
// U16 x,y :��ʾ�������
//U16 SheetNO:�������Sheet��ҳ�� ��0��ʼ.
// U16 p:Sheet������
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
// Description: FLASH�е��ַ���ʾ��Unicode��ʾ�ַ��� ������ʾ
// note : DrawUnicodeMulStr24x24
// U16 x,y :��ʾ�������
//U16 SheetNO:�������Sheet��ҳ�� ��0��ʼ.
// U16 p:Sheet������
// U8 linemus :ÿ����ʾ�ַ�����
// U8 maxline:�����ʾ�����У��������ݲ���ʵ Ϊ0ʱ�Զ���ʾȫ���ַ�
//return : ��ʾ����
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
// Description: FLASH�е��ַ���ʾ��Unicode��ʾ�ַ��� ������ʾ
// note : DrawUnicodeMulStr16x16
// U16 x,y :��ʾ�������
//U16 SheetNO:�������Sheet��ҳ�� ��0��ʼ.
// U16 p:Sheet������
// U8 linemus :ÿ����ʾ�ַ�����
// U8 maxline:�����ʾ�����У��������ݲ���ʵ Ϊ0ʱ�Զ���ʾȫ���ַ�
//return : ��ʾ����
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
// Description: FLASH�е��ַ���ʾ������ 16x8����
//------------------------------------------
void Draw16x16Str(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U32 color)
{
	if (P_LAN < 2)
	{ //��Ӣ��
		DrawFFStr(x, y, p, color);
	}
	else
	{ //����������
		//DrawUnicodeStr(x,y,SheetNO,lines,color);
		//DrawFFStr(x,y,p,color); //Խ�����ֿ�û�� ����Ӣ�����
		DrawUnicodeStr1(x, y, SheetNO, lines, color);
	}
}

//------------------------------------------
// Function   : void Draw24x24Str(U16 x,U16 y,U16 p)
// Description: FLASH�е��ַ���ʾ������ 24x12����
//return: ʵ����ʾ�ַ���
//------------------------------------------
U16 Draw24x24Str(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U32 color)
{
	if (P_LAN < 2)
	{ //��Ӣ��
		return Draw24X24FFStr(x, y, p, color);
	}
	else
	{ //����������
		return DrawUnicodeStr(x, y, SheetNO, lines, color);
	}
}

//------------------------------------------
// Function   : void DrawFMenuStr1(U16 x,U16 y,U16 p)
// Description: FLASH�е��ַ���ʾ��������ʾ 24x12����
// U16 x,U16 y - ��ʾ�������
// U16 *p - ��ʾ�ַ��׵�ַ(��Ӣ��)
// U8 linemus - ÿ�������ʾ�ַ���
// U16 SheetNO - ������Զ�Ӧ���(����������)
// U16 lines - Sheet������ (����������)
// U8 maxline -�����ʾ���� (����������)
// return: ʵ����ʾ����
//------------------------------------------
U16 Draw24x24MulStr(U16 x, U16 y, U16 *p, U8 linemus, U16 SheetNO, U16 lines, U8 maxline, U32 color)
{
	if (P_LAN < 2)
	{ //��Ӣ��
		return DrawFMultiLStr24(x, y, p, linemus, maxline, color);
	}
	else
	{ //����������
		return DrawUnicodeMulStr(x, y, SheetNO, lines, linemus, maxline, color);
	}
}

//------------------------------------------
// Function   : void DrawUnicodeFMenuStr(U16 x,U16 y,U32 color)
// Description: �˵�����Unicode��ʾר��(��Ե�ַ)
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
// Description: �˵�������ʾר��(�̶�����)(��Ե�ַ)
//------------------------------------------
void DrawFMenuStr(U16 p)
{
	U16 *add;
	U16 str = 0;
	U16 endpx;

	//	endpx = LCD_width;
	endpx = LCD_TIME_WIDTH_LIMIT - TITLE_XPOS_MIT;

	if (P_LAN < 2)
	{ //Ӣ�ĺ�����
		add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
		str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
		if (P_LHP)
		{
			BmpFillRect(TITLE_XPOS_MIT, 4, endpx, 24);
			Draw24X24FFStr(TITLE_XPOS_MIT, 4, add, Color_White);
		}
	}
	else
	{ //����������
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
// Description: �˵�������ʾר��(��̬����)(��Ե�ַ)
//------------------------------------------
void DrawFMenuStr1(U16 x, U16 y, U16 p)
{
	U16 *add;
	U16 str = 0;

	if (P_LAN < 2)
	{ //Ӣ�ĺ�����
		if (P_LHP)
		{
			add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
			str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);

			DrawFillRect(x, y, LCD_TIME_WIDTH_LIMIT - x, TITLE_YLEN, Color_Black);
			Draw24X24FFStr(x, y, add, Color_White);
		}
	}
	else
	{ //����������
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
// Description: �˵�������ʾר��(��̬����)(���Ե�ַ)
//------------------------------------------
void DrawFMenuStr2(U16 p, U16 dx)
{
	U16 *add;
	U16 str = 0;
	if (P_LAN < 2)
	{ //Ӣ�ĺ�����
		add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
		str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
		if (P_LHP)
		{
			BmpFillRect(TITLE_XPOS_BOR, 4, dx, 24);
			Draw24X24FFStr(TITLE_XPOS_BOR, 4, add, Color_White);
		}
	}
	else
	{ //����������
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
// Description: �˵�������ʾר��(��̬����)(���Ե�ַ)
//------------------------------------------
void DrawFMenuStr3(U16 p, U16 x, U16 dx)
{
	U16 *add;
	U16 str = 0;
	if (P_LAN < 2)
	{ //Ӣ�ĺ�����
		add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
		str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
		if (P_LHP)
		{
			BmpFillRect(x, 4, dx, 24);
			Draw24X24FFStr(x, 4, add, Color_White);
		}
	}
	else
	{ //����������
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
// Description: �˵�������ʾר��
//------------------------------------------
void DrawFMenuStr3P(U16 pn, U16 p)
{
}

//------------------------------------------
// Function   : void DrawFMenuStrP(U16 p)
// Description: �˵�������ʾר��
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
// Description: �˵�������ʾר��
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
// Description: ����������ʾ  �û��Զ�����ɫ��λ�õı�����
//------------------------------------------
void DrawFMenuStrUser(U16 x, U16 y, U16 p, U32 colorB, U32 colorT)
{
	U16 *add;
	U16 str = 0;

	if (P_LAN < 2)
	{ //Ӣ�ĺ�����
		if (P_LHP)
		{
			add = (U16 *)(MEM_TITLE_ADD + p * TitleLengthMax);
			str = *(MEM_TITLE_ADD + p * TitleLengthMax + TitleLengthMax - 1);
			DrawFillRect(x + SAFE_DIST1, y + SAFE_DIST4, TIT_TIME_XPOS - x, TITLE_YLEN - SAFE_DIST4, colorB);
			Draw24X24FFStr(x + SAFE_DIST1, y + SAFE_DIST4, add, colorT);
		}
	}
	else
	{ //����������
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
// Description: 32λ����,��0��ʾ,16*8���,��������
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
		s[0] = nd % 10 + '0'; //������ʾ
		nd = nd / 10;
		px = x + (dig - i) * 8;
		DrawFChar(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32λ����,����0��ʾ,16*8���,������
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
		s[0] = nd % 10 + '0'; //������ʾ
		if ((nd == 0) && (i > 1))
			s[0] = ' ';
		nd = nd / 10;
		px = x + (dig - i) * 8;
		DrawFChar(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32λ����,����0��ʾ,16*8���,������������룬���9λ
//------------------------------------------
void DrawFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color)
{
	char s[2];
	U8 i;
	U32 nd, dat = 10;
	U32 px;

	for (i = 1; i < dig; i++) //��Ӧλ�����ֵ��4λ�����9999�������ֵλ1000
		dat *= 10;

	nd = n % dat;			  //δ����СndΪԭ��ֵ��������С��ndΪ�����Ĵ�С
	dat /= 10;				  //��Сһλ
	for (i = 1; i < dig; i++) //��ȡ��ֵ�����λ
	{
		if ((nd / dat))
			break;
		dat /= 10;
	}
	px = x; //������ʾ
	s[1] = 0;
	for (; i <= dig; i++)
	{
		if (dat == 0) //���λΪ0
			break;
		s[0] = nd / dat + '0'; //��ֵ��ascll��ֵ
		nd = nd % dat;		   //ʣ��ֵ
		DrawFChar(px, y, s, color);
		px += 8;
		dat /= 10;
	}
}

//------------------------------------------
// Function   : void DrawFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
// Description: 32λ����,��0��ʾ,16*8���,����������Ϊ0
// note: ��ָ��λ��С���㣬���һλ������С���㣬С����ռ��һ��dig��ʾλ��'-'�ſ���С�������'x'
//------------------------------------------
void DrawFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color)
{
	char s[2];
	U32 nd;
	U32 px, pointx;

	nd = abs(n);
	px = x + (dig - 1) * 8; //�ƶ������һλ
	s[1] = 0;
	if (point > 0)
		pointx = px - point * 8;
	else
		pointx = XLEN;
	while (px >= x)
	{
		s[0] = nd % 10 + '0'; //������ʾ
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
// Description: 32λ����,��0��ʾ,16*16���,����������Ϊ0
// note: ��ָ��λ��С���㣬���һλ������С���㣬С����ռ��һ��dig��ʾλ��'-'�ſ���С�������'x'
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
// Description: 32λ������ʾ16*16�����ʼ
// note: 32λ����,��0��ʾ,16*16���,������������Ϊ0//����
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
		s[0] = n % 10 + '0'; //������ʾ
		n = n / 10;
		DrawFStr16(x, y, s, color);
		x -= 16;
	}
}

//------------------------------------------
// Function   : void DrawFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32λ����,��0��ʾ,16*16���,������������Ϊ0
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
		c = nd % 10; //������ʾ
		s[0] = c + '0';
		nd = nd / 10;
		px = x + (dig - i) * 16;
		DrawFStr16(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32λ����,��0��ʾ����ABCDEF��16����,16*8���,������������Ϊ0
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
// Description: 32λ���֣���0��ʾ��24*12���,������������Ϊ1
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
// Description: 32λ���֣���0��ʾ��24*12���,������������Ϊ0
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
// Description: 32λ���֣�����0��ʾ��24*12���,������������Ϊ1
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
// Description: 32λ����,����0��ʾ,24*12���,������������룬���9λ������Ϊ1
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
// Description: 32λ����,����0��ʾ,24*12���,����������Ϊ1
// note: ��ָ��λ��С���㣬���һλ������С���㣬С����ռ��һ��dig��ʾλ��'-'�ſ���С�������'x'
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
// Description: 32λ������ʾ��24*24��棬��ʼ
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
// Description: 32λ����,��0��ʾ,24*24���,����������Ϊ0
// note: ��ָ��λ��С���㣬���һλ������С���㣬С����ռ��һ��dig��ʾλ��'-'�ſ���С�������'x'
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
// Description: 32λ���֣���0��ʾ��24*24��棬������
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
// Description: 32λ���֣���0��ʾ��24*12��棬����ABCDEF��16���ƣ�����Ϊ1//�����
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
// Description: 32λ���֣���0��ʾ��32*16���
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
		s[0] = nd % 10 + '0'; //������ʾ
		nd = nd / 10;
		px = x + (dig - i) * 20;
		DrawFASC3216(px, y, s, color);
	}
}

//------------------------------------------
// Function   : void DrawLFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color)
// Description: 32λ���֣�����0��ʾ��32*16���
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
// Description: 32λ���֣�����0��ʾ��32*16���(�����)(���9λ)
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
// Description: 32λ����,��0��ʾ,32*16���,����������Ϊ0
// note: ��ָ��λ��С���㣬���һλ������С���㣬С����ռ��һ��dig��ʾλ��'-'�ſ���С�������'x'
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
// Description: ��ʾYES��NO ��32*16
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
// Description: 32λ������ʾ24*24���
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
// Description: 32λ������ʾ24*24�����ʼ
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
// Description: 32λ���ֺ�0��ʾ24*24���
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
// Description: 32λ���֣���0��ʾ����ABCDEF��ʮ��������ʾ��32*16���
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
// Description: ��ȡ������ʾ�ļ����
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
// Description: ��ȡ������ʾ�ļ����
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
// Description: ��ȡ������ʾ�ļ����
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
// Description: ��ȡ������ʾ�ļ����
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
// Description: ��ȡ������ʾ�ļ���
//	U8 len : ������ʾ�ĳ���
//	U8 dy :��һ�к͵ڶ���֮��ļ��
//  U8 *RealRows: ʵ����ʾ����
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
		return; //���֧��64�ֽ�����
	if (namelen == 64)
		tmplen = namelen - 1;
	else
		tmplen = namelen;

	for (i = 0, j = 0; i < tmplen; i++)
	{ //ǰ63�ֽڲ��Ϻ�ָ���ַ����������㣬��64�ֽ�Ϊ���ͱ�־
		if (s[i] == 0)
			break;
		if (((s[i] >= 0x30) && (s[i] <= 0x39)) ||	/*����*/
			((s[i] >= 0x41) && (s[i] <= 0x5a)) ||	/*��д��ĸ*/
			((s[i] >= 0x61) && (s[i] <= 0x7a)) ||	/*Сд��ĸ*/
			((s[i] > 0x7f) && (s[i + 1] > 0x7f)) || /*����B0A1-F7FE*/
			(s[i] == 0x5f))							/*�»���*/
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

	//�ȼ��������л���һ����ʾ	������ʾ����24*12  ������ʾ����16*8
	for (i = 0; i < tmplen; i++)
	{
		if (s[i] == 0)
			break;
	}
	if (i >= (16 * chLineLimit / 12 - 1)) //��>�޸�Ϊ>=
		disrow2 = 1;

	wdt();

	if (disrow2 == 1)
	{ //������ʾ ����16*8
		chLineLimit = chLineLimit * 3 / 2;
		for (i = 0; i < (chLineLimit); i++) //�Ȳ��ҵ�һ�������ַ�����ASCLL�⣩
		{
			if (*(s + i) == 0)
				break;
			if (*(s + i) & 0x80) //����/�����ַ�
			{
				ChineseNums++;
			}
		}

		if (i < (chLineLimit)) //ʵ����һ����ʾ������16*8
		{
			for (i = 0; i < (chLineLimit); i++) //�ѵ�һ�е����ݸ��Ƶ�s2��
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
		{ //������ʾ ����16*8
			wdt();
			if (ChineseNums % 2) //��һ��Ӣ�ĸ�����������ʾ����ȫ���������һ�������ַ���ʾ���
			{
				for (j = 0; j < (chLineLimit - 1); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 4, s2, color); //��ʾ��һ��
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (chLineLimit - 1), j = 0; j < (chLineLimit); i++, j++)
				{ //�Ȳ��ҵڶ������ĸ���
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //����
						ChineseNums++;
					}
				}

				if ((j < (chLineLimit)) || ((ChineseNums % 2) == 0))
				{ //��ʾ��ȫ�����֣������һ�������ַ�û�����
					for (j = 0; j < (chLineLimit); j++)
					{
						s2[j] = *(s + chLineLimit - 1 + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //��ʾ�ڶ���
				}
				else
				{
					for (j = 0; j < (chLineLimit - 1); j++)
					{
						s2[j] = *(s + chLineLimit - 1 + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //��ʾ�ڶ���
				}
			}
			else //��һ����ʾ��ȥ��
			{
				for (j = 0; j < (chLineLimit); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 4, s2, color); //��ʾ��һ��
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (chLineLimit), j = 0; j < (chLineLimit); i++, j++)
				{ //�Ȳ��ҵڶ������ĸ���
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //����
						ChineseNums++;
					}
				}

				if ((j < (chLineLimit)) || ((ChineseNums % 2) == 0))
				{ //��ʾ��ȫ�����֣������һ�������ַ�û�����
					for (j = 0; j < (chLineLimit); j++)
					{
						s2[j] = *(s + chLineLimit + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //��ʾ�ڶ���
				}
				else
				{
					for (j = 0; j < (chLineLimit - 1); j++)
					{
						s2[j] = *(s + chLineLimit + j);
					}
					DrawF16Str(x, y + 16 - 4 + dy, s2, color); //��ʾ�ڶ���
				}
			}
			if (RealRows != NULL)
				*RealRows = 2;
		}
	}
	else
	{ //һ����ʾ ����24*12
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
// Description: ��ȡ������ʾ�ļ���
//	U8 len : ������ʾ�ĳ���
//	U8 dy :��һ�к͵ڶ���֮��ļ��
//  U8 *RealRows: ʵ����ʾ����
//------------------------------------------
void DrawFFilename1(U32 x, U32 y, char *s, U8 namelen, U32 color, U8 len, U8 dy, U8 *RealRows)
{

	char s2[64] = {0};
	U16 i, j;
	U8 ChineseNums = 0;
	U8 disrow2 = 0;
	U8 tmplen;

	if (namelen > 64)
		return; //���֧��64�ֽ�����
	if (namelen == 64)
		tmplen = namelen - 1;
	else
		tmplen = namelen;

	//�ȼ��������л���һ����ʾ	������ʾ����24*12  ������ʾ����16*8
	for (i = 0; i < tmplen; i++)
	{
		if (s[i] == 0)
			break;
	}
	if (i >= (16 * len / 12 - 1))
		disrow2 = 1;

	wdt();

	if (disrow2 == 1)
	{ //������ʾ ����16*8
		for (i = 0; i < (2 * len); i++)
		{ //�Ȳ��ҵ�һ�����ĸ���
			if (*(s + i) == 0)
				break;
			if (*(s + i) & 0x80)
			{ //����
				ChineseNums++;
			}
		}

		if (i < (2 * len))
		{ //ʵ����һ����ʾ������16*8
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
		{ //������ʾ ����16*8
			wdt();
			if (ChineseNums % 2)
			{ //��һ��Ӣ�ĸ�����������ʾ����ȫ���������һ�������ַ���ʾ���
				for (j = 0; j < (2 * len - 1); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 6, s2, color); //��ʾ��һ��
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (2 * len - 1), j = 0; j < (2 * len); i++, j++)
				{ //�Ȳ��ҵڶ������ĸ���
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //����
						ChineseNums++;
					}
				}

				if ((j < (2 * len)) || ((ChineseNums % 2) == 0))
				{ //��ʾ��ȫ�����֣������һ�������ַ�û�����
					for (j = 0; j < (2 * len); j++)
					{
						s2[j] = *(s + 2 * len - 1 + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //��ʾ�ڶ���
				}
				else
				{
					for (j = 0; j < (2 * len - 1); j++)
					{
						s2[j] = *(s + 2 * len - 1 + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //��ʾ�ڶ���
				}
			}
			else
			{
				for (j = 0; j < (2 * len); j++)
				{
					s2[j] = *(s + j);
				}
				DrawF16Str(x, y - 6, s2, color); //��ʾ��һ��
				for (i = 0; i < 64; i++)
				{
					s2[i] = 0;
				}

				ChineseNums = 0;
				for (i = (2 * len), j = 0; j < (2 * len); i++, j++)
				{ //�Ȳ��ҵڶ������ĸ���
					if (*(s + i) == 0)
						break;
					if (*(s + i) & 0x80)
					{ //����
						ChineseNums++;
					}
				}

				if ((j < (2 * len)) || ((ChineseNums % 2) == 0))
				{ //��ʾ��ȫ�����֣������һ�������ַ�û�����
					for (j = 0; j < (2 * len); j++)
					{
						s2[j] = *(s + 2 * len + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //��ʾ�ڶ���
				}
				else
				{
					for (j = 0; j < (2 * len - 1); j++)
					{
						s2[j] = *(s + 2 * len + j);
					}
					DrawF16Str(x, y + 16 - 6 + dy, s2, color); //��ʾ�ڶ���
				}
			}
			if (RealRows != NULL)
				*RealRows = 2;
		}
	}
	else
	{ //һ����ʾ ����24*12
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
// Description: ��ʾ������
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
// Description: ��水ť	//���ư����ɿ�ʼ�ı�׼ɫ����
// note: *pʵ����Ч��Ĭ��ѡ��148.bin����ͼ����ʾ
// c1���߿���ɫ��KEY_COLOR��ѡȡ�� C2����ť��ɫ(RGB������ɫ)
//------------------------------------------
void DrawButton(KEYPAD bn, U16 *p, U32 c1, U32 c2)
{
	DrawButtonOff1(bn, c1);
	if ((c2 == 0) || (c2 == Color_Gray))
		DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, keypicdata[c1]); //����C1��ʾ��ɫ������ʾUI
	else
		DrawFPBMPBTN(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c2, keypicdata[c1]); //����C2��ʾUI������C1��ʾ��ɫ
}

//------------------------------------------
// Function   : void DrawButton1(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 c1, U32 c2)
// Description: ������ɫ��ť  ֻ�ɻ���keypicdata[]���е���ɫ	 ͬDrawButton
//------------------------------------------
void DrawButton1(KEYPAD bn, U32 c1, U32 c2)
{
	DrawButtonOff2(bn, c1);
	if (c2 == 0)
	{
#if USEMULTCOLOR //����
		if (Color_White1 == c1 || Color_White == c1)
		{ //��ɫ�任
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
		DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c1); //����C1��ʾ��ɫ������ʾUI
	}
	else
	{
		DrawFPBMPBTN(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, c2, c1); //����C2��ʾUI������C1��ʾ��ɫ
	}
}

//------------------------------------------
// Function   : void DrawButton(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 c1, U32 c2)
// Description: ��水ť
// note��: �߿��С��ѡ 0:4x4 ��1:8x8��2:12x8��3:0x4
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
#if USEMULTCOLOR //����
		if (Color_White1 == c1)
		{ //��ɫ�任
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
// Description: ��水ť	 //��ʾͼ��ԭ��������Ӱ�ť��Ӱ
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
// Description: ������ɫ��ť c1:�߿���ɫ c2:������ɫ ֻ�ɻ���keypicdata[]���е���ɫ
//------------------------------------------
void DrawButtonSingleColor(KEYPAD bn, U32 c1, U32 c2)
{
	DrawButtonOff1(bn, c1);
	DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, keypicdata[c2]); //����C1��ʾ��ɫ������ʾUI
}

//------------------------------------------
// Function   : void DrawButtonDisPat(KEYPAD bn, U32 colorBg, U32 colorBor)
// Description: ����������ʾ�������ް�����Ч��ť
//------------------------------------------
void DrawButtonDisPat(KEYPAD bn, U32 colorBor, U32 colorBg)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBg);
	DrawRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBor);
}

//------------------------------------------
// Function   : DrawButtonEdit(KEYPAD bn, U32 colorBor, U32 colorBg)
// Description: �����ı���ť
//------------------------------------------
void DrawButtonEdit(KEYPAD bn, U32 colorBor, U32 colorBg)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBg);
	DrawDottedRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBor);
}

//------------------------------------------
// Function   :
// Description: �����ı������
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
						 sheetPage, strIndex, bn.colorT[0]); //�������������
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
						 sheetPage, strIndex, bn.colorT[0]); //�������������
		}
	}
	else if (size == STR24X12)
	{
		if (alignment == LEFT || alignment == RIGHT)
		{
			Draw24x24Str(bn.xpis + safeDist_X,
						 bn.ypis + (bn.ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)(strIndex * TitleLengthMax)),
						 sheetPage, strIndex, bn.colorT[0]); //�������������
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
						 sheetPage, strIndex, bn.colorT[0]); //�������������
		}
	}
}

//------------------------------------------
// Function   : void DrawEdit(RECTPAD bn, U32 colorBor, U32 colorBg)
// Description: �����ı���
//------------------------------------------
void DrawEdit(RECTPAD bn, U32 colorBor, U32 colorBg)
{
	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBg);
	DrawDottedRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, colorBor);
}

//------------------------------------------
// Function   : void DrawButtonOn(U32 x, U32 y, U32 dx, U32 dy)
// Description: ��水ť����   //���ư�ť����ʱ��İ���
// note:������ʵ��ɫ��Χ���� KEY_COLORTL
//		����ť��ʾλ��(1,1)����ɫ������keypicdatb[][]��ɫ��ʱ�����ư�ť������߿��ѡ����ɫ��Χ
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
// Description: ��水ť����		//���ư�ť����ʱ��İ���
// note:��ʾ��ɫ�����ƣ�һ�������� keypicdatb keypicdatc
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
// Description: ��水ť�����Ű���		//���ư�ť����ʱ��İ���
// note: ZoomInNum:����������������
//------------------------------------------
U16 chZoomInColor = 0, chZoomInPx = 0, chZoomInColor2 = 0;
void DrawButtonOnZoomIn(KEYPAD bn, U16 ZoomInNum)
{
	if (ZoomInNum < 1)
		return;
	chZoomInColor = ReadPixel(bn.xpis + 1, bn.ypis + 1);
	chZoomInColor2 = ReadPixel(bn.xpis, bn.ypis);
	chZoomInPx = ZoomInNum;
	BmpFillRect(bn.xpis, bn.ypis, bn.xsize, ZoomInNum);													   //��
	BmpFillRect(bn.xpis, bn.ypis + bn.ysize - ZoomInNum, bn.xsize, ZoomInNum);							   //��
	BmpFillRect(bn.xpis, bn.ypis + ZoomInNum, ZoomInNum, bn.ysize - 2 * ZoomInNum);						   //��
	BmpFillRect(bn.xpis + bn.xsize - ZoomInNum, bn.ypis + ZoomInNum, ZoomInNum, bn.ysize - 2 * ZoomInNum); //��
}
//------------------------------------------
// Function   : void DrawButtonOnZoomInRes(U32 x, U32 y, U32 dx, U32 dy)
// Description: ��水ť�����Ű�������		//���ư�ť����ʱ��İ���
// note: ZoomInNum:����������������
//------------------------------------------
void DrawButtonOnZoomInRes(KEYPAD bn)
{
	if (chZoomInPx < 1)
		return;

	DrawFillRect(bn.xpis, bn.ypis, bn.xsize, chZoomInPx, chZoomInColor);													   //��
	DrawFillRect(bn.xpis, bn.ypis + bn.ysize - chZoomInPx, bn.xsize, chZoomInPx, chZoomInColor);							   //��
	DrawFillRect(bn.xpis, bn.ypis + chZoomInPx, chZoomInPx, bn.ysize - 2 * chZoomInPx, chZoomInColor);						   //��
	DrawFillRect(bn.xpis + bn.xsize - chZoomInPx, bn.ypis + chZoomInPx, chZoomInPx, bn.ysize - 2 * chZoomInPx, chZoomInColor); //��
	DrawDottedRect(bn.xpis, bn.ypis, bn.xsize, bn.ysize, chZoomInColor2);
	chZoomInPx = 0;
	chZoomInColor = 0;
	chZoomInColor2 = 0;
}

//------------------------------------------
// Function   : void DrawButtonOff(U32 x, U32 y, U32 dx, U32 dy)
// Description: ��水ť
// note:������ʵ��ɫ��Χ���� KEY_COLORTL
//		����ť��ʾλ��(1,1)����ɫ������keypicdatb[][]��ɫ��ʱ�����ư�ť������߿��ѡ����ɫ��Χ
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
// Description: ��水ť		//��ɫ�İ����߿����
// note:��ʾ��ɫ�����ƣ�һ�������� keypicdatb keypicdatc
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
// Description: ��水ť	//��ɫ�İ����߿���� ͬDrawButtonOff1
// note: ���ɱ߿���ɫת�䣺��ɫת��Ϊ��ɫ    ����ɫ���䣬������ɫת��Ϊ��ɫ
//------------------------------------------
void DrawButtonOff2(KEYPAD bn, U16 c)
{
	//printf("---  xpis = %d ypis = %d xsize = %d ysize =%d c = %04x-------------\r\n",bn.xpis,bn.ypis,bn.xsize,bn.ysize,c);
	// #if USEMULTCOLOR //����
	U16 c1;
	if (Color_White1 == c || Color_White == c)
	{ //��ɫ�任
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

	// #else //�Ҷ�

	// 	DrawFrame(bn.xpis, bn.ypis, bn.xsize, bn.ysize, c, c);
	// 	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, c, c);
	// 	DrawFrame(bn.xpis + 2, bn.ypis + 2, bn.xsize - 4, bn.ysize - 4, c, c);
	// 	DrawFrame(bn.xpis + 3, bn.ypis + 3, bn.xsize - 6, bn.ysize - 6, c, c);
	// #endif
}

//------------------------------------------
// Function   : void DrawButtonOff1(U32 x, U32 y, U32 dx, U32 dy)
// Description: ��水ť	//���Ʋ�ͬ��С�İ�����Ӱ flag 0����־ 1���� 2���ش�
// note: ���ɱ߿���ɫת�䣺��ɫת��Ϊ��ɫ    ����ɫ���䣬������ɫת��Ϊ��ɫ
//		 flag����ѡ����ʾ�߿�Ĵ�С��Ĭ��0:4x4��1:8x8��2:12x12
//------------------------------------------
void DrawButtonOff3(KEYPAD bn, U16 c, U16 flag)
{

#if USEMULTCOLOR //����
	U16 c1;
	if (c > g_bmplen)
	{
		if (Color_White1 == c)
		{ //��ɫ�任
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

#else //�Ҷ���ʾ
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
// Description: ��水ť
// note:��ɫ��������Ϊ keypicdatb �� keypicdatc����ɫ��
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
// Description: ��水ť
// note:խ�߿�2x2
//------------------------------------------
void DrawButtonOff5(KEYPAD bn, U16 c)
{
	DrawFrame(bn.xpis + 0, bn.ypis + 0, bn.xsize - 0, bn.ysize - 0, keypicdatb[c][1], keypicdatc[c][1]);
	DrawFrame(bn.xpis + 1, bn.ypis + 1, bn.xsize - 2, bn.ysize - 2, keypicdatb[c][2], keypicdatc[c][2]);
}
//------------------------------------------
// Function   : void DrawTextbox(KEYPAD bn, U16 *p, U32 c1, U32 c2)
// Description: �����ı���ʾ��
//------------------------------------------
void DrawTextbox(KEYPAD bn, U32 c1)
{
	DrawButtonOn1(bn, c1);
	DrawFillRect(bn.xpis + 4, bn.ypis + 4, bn.xsize - 8, bn.ysize - 8, keypicdata[c1]);
}

extern volatile S32 g_array3[2][2];
//------------------------------------------
// Function   : void DrawArc(U32 x1,U32 y1,U32 x2,U32 y2, U32 x3,U32 y3,U8 f,U32 color)
// Description: ��԰��,������������ 0.0-360.0�н���ת���
//------------------------------------------
void DrawArc(S32 x1, S32 y1, S32 x2, S32 y2, S32 x3, S32 y3, U8 f, U32 color)
{
	U32 i, cw;				  // ԰��
	S32 xo, yo, x, y;		  // ����
	U32 an, ang1, ang2, ang3; // �����������λ��
	S32 dx, dy;
	F32 r; // �뾶
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
		cw = 0; // ˳Բ
	else
		cw = 1; // ��Բ

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
// Description: ��ʾ�����,ѡ���ı��		0 1 2 3 4 5 6 7 8 9 H  S  E  P
// note : flag:��ȡָ��������� f:��ָ����ɫ������Ӧλ��
//------------------------------------------
void DrawSerialNumber(S32 x, S32 y, char n, U8 f, U32 color, U8 flag) //ע�����귶Χ����ֵ����ʾ
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
// Description: ��ʾ�����,ѡ���ı��		0 1 2 3 4 5 6 7 8 9
// note �� f:��ָ����ɫ������Ӧλ��
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
// Description: ͼ�α��
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

//����ָ��������ɫ
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

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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

//�ָ�ָ��������ɫ
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

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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

//����ָ��������ɫ
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

#if VIEWTYPE == HORIZONTAL //����
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
#else //����
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
//������:void setDiskStatus(U8 status)
//����  :���ô���״̬
--------------------------------------*/
void initViewStatus(void)
{
	setDiskStatus(0);
	clrShowTime();
	Timer_dms = 1023;
	Timer_tms = 30720;
}

/*-----------------------------------------------
//������:void updateViewStatus(U8 status)
//����  :����ʱ�����״̬
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
//������: U8 showTime(U16 y,U8 m,U8 d)
//����  : ָ��λ����ʾϵͳʱ��
--------------------------------------*/
void showTime(U16 x, U16 y)
{
	U8 week;
	U16 tlang = getLanguage();
	Read_D(D_Address[SYS] + SYS_MS, 4, (U16 *)(&YREG[SYS_MS])); // �����
	g_SysTimes.minute = BCD_2Hex((U8)(YREG[SYS_MS] >> 8));
	SetLanguage(MENU_P1);

	if (g_old_minute != g_SysTimes.minute)
	{ //ÿ����ˢ��һ��ʱ��
		g_old_minute = g_SysTimes.minute;

		g_SysTimes.hour = BCD_2Hex((U8)(YREG[SYS_WH] & 0x007f));
		g_SysTimes.day = BCD_2Hex((U8)(YREG[SYS_MD] & 0x00ff));
		g_SysTimes.month = BCD_2Hex((U8)(YREG[SYS_MD] >> 8));
		g_SysTimes.year = BCD_2Hex((U8)(YREG[SYS_Y] & 0x00ff));

		//week = (g_SysTime.day+2*g_SysTime.month+3*(g_SysTime.month+1)/5+(g_SysTime.year+2000)+(g_SysTime.year+2000)/4-(g_SysTime.year+2000)/100+(g_SysTime.year+2000)/400)%7;
		//week = (g_SysTime.day+2*g_SysTime.month+3*(g_SysTime.month+1)/5+(g_SysTime.year+2000)+(g_SysTime.year+2000)/4-(g_SysTime.year+2000)/100+(g_SysTime.year+2000)/400)%7;
		week = getWeekDay(2000 + g_SysTimes.year, g_SysTimes.month, g_SysTimes.day);
		//ˢ����ʾ��ǰʱ��
		BmpFillRect(x, y, LCD_WIDTH - TIT_TIME_XPOS, 24);

		DrawMFU32(x, y, 2000 + g_SysTimes.year, 4, Color_White); //��
		Draw24X24FStr(x + 52, y, "/", Color_White);
		DrawMFU32(x + 65, y, g_SysTimes.month, 2, Color_White); //��
		Draw24X24FStr(x + 91, 4, "/", Color_White);
		DrawMFU32(x + 104, y, g_SysTimes.day, 2, Color_White); //��

		Draw24x24Str(x + 130, y, MEM_TITLE_ADD + (week + 42) * TitleLengthMax, SheetPage, week + 42, Color_White);

		DrawMFU32(x + 220, y, g_SysTimes.hour, 2, Color_White);	  //ʱ
		DrawMFU32(x + 259, y, g_SysTimes.minute, 2, Color_White); //��
	}
	SetLanguage(tlang);
}

/*-----------------------------------------------
//������: void showTimeOnMenu()
//����  : �˵�����ʾϵͳʱ��
--------------------------------------*/
void showTimeOnMenu(void)
{
	showTime(TIT_TIME_XPOS, TIT_TIME_YPOS);
}
/*-----------------------------------------------
//������:void refreshPoint(U16 x,U16 y,U16 tims)
//����  : ˢ��ָ��λ�õ�':'
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
//������:void refreshPointOnMenu(void)
//����  : ˢ�²˵���ָ��λ�õ�':'
--------------------------------------*/
void refreshPointOnMenu(U16 time)
{
	U16 xpos = TIT_TIME_XPOS + 200 + 46;//754
	refreshPoint(xpos, TIT_TIME_YPOS, time);
}

/*-----------------------------------------------
//������:void showTimeAndDisk()
//����  : ��������ʾ�ⲿ�豸״̬��ʱ��
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
//������:void showDiskStatus()
//����  : ��������ʾ�ⲿ�豸״̬
--------------------------------------*/
void showDiskStatus(void)
{
	U16 xpos;
	U8 status = getDiskStatus();

	xpos = TIT_DISK_XPOS;
	if ((getDiskStatus1() & 0x02) != (status & 0x02)) //U��״̬�仯
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
// Description: ��������ɫ�滻
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
// Description: ָ�����������ʾ�ַ���16x8
//------------------------------------------
void DrawCenterStrArea1(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U32 color)
{
	U16 dpox = 0, dpoy = 0;
	U16 dispix = getStrLen((U8 *)str) * 8; //������ʾ����ռ�����ص�
	U16 strlen;
	U8 trow = 0;
	U8 i, j;
	char tstr[64] = {0};
	if (row && col)
	{ //������ʾ����������
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
// Description: ָ�����������ʾ�ַ���16x8
//------------------------------------------
void DrawCenterTextArea1(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U16 sheet, U32 color)
{
	//printf("str %s \r\n",str);
	if (P_LAN < 2)
	{ //��Ӣ��
		DrawCenterStrArea1(x, y, dx, dy, row, col, str, color);
	}
	else
	{
		DrawCenterStrArea1(x, y, dx, dy, row, col, str, color);
	}
}

//------------------------------------------
// Function   : void DrawCenterTextArea1(U32 x, U32 y,U16 dx,U16 dy, char *str,U16 sheet, U32 color)
// Description: ָ�����������ʾ�ַ���16x8
//------------------------------------------
void DrawCenterTextArea16(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U16 sheet, U16 p, U32 color)
{
	U32 stroffest;
	U16 strlen = 0;
	U16 dispix = 0; //������ʾ����ռ�����ص�
	U16 vdx = 0, vdy = 0;
	U16 trow = 0;
	//printf("str %s \r\n",str);
	if (P_LAN < 2)
	{ //��Ӣ��
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
// Description: ��нǶ�
//------------------------------------------
U32 GetAng(S32 dx, S32 dy)
{
	F32 ax, ay;
	U32 ang = 0;

	ax = dx;
	ay = dy; // S32תF32Ϊ����atan��
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
	{ // ��I����
		ang = (U32)(atan(ay / ax) * 1800 / 3.14159);
	}
	else if ((dx <= 0) && (dy > 0))
	{ // ��II����
		ang = (U32)(atan(-ax / ay) * 1800 / 3.14159) + 900;
	}
	else if ((dx < 0) && (dy <= 0))
	{ // ��III����
		ang = (U32)(atan(ay / ax) * 1800 / 3.14159) + 1800;
	}
	else if ((dx > 0) && (dy <= 0))
	{ // ��IV����
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
// Description: ����������ʾ�ķ�Χ
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
// Description: ̽����ʾ���ֳ���
//------------------------------------------
U8 CheckTextLen(U16 *p, U16 SheetNO, U16 lines)
{
	U8 uLen = 0;
	U16 c;
	U8 c1, c2;
	if (P_LAN < 2)
	{ //��Ӣ��
		c = (*p);
		c1 = c & 0xff;
		c2 = (c >> 8) & 0xff;
		while ((c1 != 0) && (c2 != 0))
		{
			uLen++;
			c = *(p + uLen);
			c1 = c & 0xff;
			c2 = (c >> 8) & 0xff;
		} //̽���ִ�����
		if ((c1 > 0) && (c2 == 0))
			uLen++;
		uLen *= 2; //����Ӣ���ַ�����
	}
	else
	{ //�������	������
	}
	return uLen;
}
//------------------------------------------
// Function   : void ReadPointData1(U32 x, U32 y)
// Description: ��ȡָ��������ݣ�����Χ 18px�ڵ�����
// note: Ϊ�˻��ƻ������ܱ�ʶ��ʱ��������Χ���ݸ���
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
// Description: �������1�����ݣ���ȡָ������Χ 18px�ڵ�����
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
// Description: ��λ���1�����ݣ���ȡָ������Χ 18px�ڵ�����
//------------------------------------------
void ResetData1(void)
{
	if (g_dispat1.IsDat == 1)
		g_dispat1.IsDat = 0;
	wdt();
}
//------------------------------------------
// Function   : void ReadPointData2(U32 x, U32 y)
// Description: ��ȡָ��������ݣ�����Χ 18px�ڵ�����
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
// Description: �������2�����ݣ���ȡָ������Χ 18px�ڵ�����
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
// Description: ��λ���2�����ݣ���ȡָ������Χ 18px�ڵ�����
//------------------------------------------
void ResetData2(void)
{
	if (g_dispat2.IsDat == 1)
		g_dispat2.IsDat = 0;
	wdt();
}
//------------------------------------------
// Function   : void ReadPointData3(U32 x, U32 y)
// Description: ��ȡָ��������ݣ�����Χ 18px�ڵ�����
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
// Description: �������3�����ݣ���ȡָ������Χ 18px�ڵ�����
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

