//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  lcddisp.h								*
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

#ifndef __LCDDISP_H
#define __LCDDISP_H

#include "stm32cpu.h"

typedef enum
{
    STR16X8 = 1,
    STR24X12 = 2,
} tyden_strSize;

typedef enum
{
    LEFT = 1,
    RIGHT = 2,
    CENTER = 3,
} tyden_alignment;

#define DISP_PAGE_SIZE LCD_width *LCD_height * 2
#define DISP_PAGE_MAXNO 5
#define DISP_PARA 2 //缩放因子

#define XLEN LCD_width
#define YLEN LCD_height
#define YOFFSET XLEN * 2
#define XOFFSET YLEN * 2

#define TITLE_XLEN LCD_width
#define TITLE_YLEN 32
#define TITLE_XPOS_BOR 44
#define TITLE_XPOS_MIT 44
#define TITLE_XPOS_G 44
#define TITLE_XPOS_C2 44
#define TITLE_XPOS_GROUP (TIT_DISK_XPOS - 280)
#define TITLE_XPOS 44

#define NUMKEYPOS 10
#define TIT_MAXLENGTH 15    //中文标题单位长度
#define TIT_MAXLENGTH_E 22  //34			//英文标题单位长度
#define TIT_MAXLENGTH1 20   //中文标题单位长度
#define TIT_MAXLENGTH1_E 27 //34			//英文标题单位长度
#define TIT_LOGO_XLEN 36
#define TIT_LOGO_YLEN TITLE_YLEN
#define TIT_TIME_XPOS 508
#define TIT_DISK_XPOS (TIT_TIME_XPOS - TYPE_SIZE6)
#define TIT_TIME_YPOS 4

#define MENUMAX 452

#define G_MAXLENG_MSG1 25
#define G_MAXLENG_MSG1_E 50

#define TIT_MAXLENGTH2 22

#define NUMB2412S1 12
#define NUMB2412S 13
#define HZLIBSIZE 3844

#define SDRAM_BASE_ADDR 0xc0000000
#define LCD_VRAM_BASE_ADDR ((U32)SDRAM_BASE_ADDR)

#if defined(USESST39VFXX)
#define FLASHINDEXLIST (SST39VFADD + FLASHINDEXADDR + 36)
#else
#define FLASHINDEXLIST (SDRAMFLASHADD + FLASHINDEXADDR + 36)
#endif

extern volatile U8 usepageno;

extern volatile U32 flashver, flashdate;
extern volatile U32 mp3dver, mp3ddate;
extern volatile U8 flashdataflag[200];
extern volatile U32 tmpflashadd[200];
extern U16 SheetPage; //多国语言Sheet表页码，从0开始
extern U16 *MEM_TITLE_ADD;
extern U16 *MEM_TITLE_AUDIO_ADD;
extern U32 TitleLengthMax;
extern U32 TitleAudioLengthMax;

extern void initflashadd(void);
//extern U16 RGB(U8 r, U8 g, U8 b);
extern U16 STNToTFT(U8 color);
extern void WritePage(U32 page_no);
extern void ViewPage(U32 page_no);
extern void SDRAM_memcpy(U32 dest, U32 src, U32 count);
extern void PageWritern(U8 des_no, U8 org_no);
extern void CLR_Screen(U32 color);
extern void DrawIcon(U16 x, U16 y, U32 Color);
extern void DrawIconoff(U16 x, U16 y);
extern void PutPixel(U32 x, U32 y, U32 color);
extern void PutPixel1(S32 x, S32 y, U32 color);
extern U16 ReadPixel(U32 x, U32 y);
//extern void DrawHLine(U32 x, U32 y, U32 dx, U32 color);
//extern void DrawVLine(U32 x, U32 y, U32 dy, U32 color);
//extern void DrawLine(U32 x1,U32 y1,U32 x2,U32 y2,U32 color);
extern void DrawHLine(S32 x, S32 y, U32 dx, U32 color);
extern void DrawVLine(S32 x, S32 y, U32 dy, U32 color);
extern void DrawHLine1(S32 x, S32 y, S32 dx, U16 color, U16 color2);
extern void DrawVLine1(S32 x, S32 y, S32 dx, U16 color, U16 color2);
extern void DrawLine(S32 x1, S32 y1, S32 x2, S32 y2, U32 color);
extern void DrawLine1(S32 x1, S32 y1, S32 x2, S32 y2, U16 color, U16 color2);
extern void DrawPoint(S32 x, S32 y, U16 color);
extern void DrawCross7x7(S32 x, S32 y, U16 color);
extern void DrawRect(U32 x, U32 y, U32 dx, U32 dy, U32 color);
extern void DrawHDottedLine(S32 x, S32 y, U32 dx, U32 color);
extern void DrawVDottedLine(S32 x, S32 y, U32 dy, U32 color);
extern void DrawDottedRect(U32 x, U32 y, U32 dx, U32 dy, U32 color);
extern void DrawFillRect(U32 x, U32 y, U32 dx, U32 dy, U32 color);
extern void DrawFillRect1(U32 x, U32 y, U32 dx, U32 dy, U32 color);
extern void DrawDFrame(U32 x, U32 y, U32 dx, U32 dy, U32 t, U32 c1);
extern void DrawFrame(U32 x, U32 y, U32 dx, U32 dy, U32 c1, U32 c2);
extern void DrawCorFrame(U32 x, U32 y, U32 d, U32 dt, U32 w, U32 c1, U32 c2);
extern void DrawFillCorRect(U32 x, U32 y, U32 d, U32 dt, U32 w, U32 color);
extern void DrawColor_Gray(U32 x, U32 y, U32 dx, U32 dy);
extern void DrawColor_Gray1(U32 x, U32 y, U32 dx, U32 dy);
extern void DrawColorRreverse(U32 x, U32 y, U32 dx, U32 dy);
extern void DrawBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p);
extern void DrawBMP1(U32 x, U32 y, U32 dx, U32 dy, U16 *p);
extern void DrawBMP2(U32 x, U32 y, U32 dx, U32 dy, U16 *p);

extern void DrawPBMP(U32 x, U32 y, U32 dx, U32 dy, U8 *p);
extern void DrawFPBMP3(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f);
extern void DrawFPBMP(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U8 f);
extern void DrawFBMP2(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 color);

extern void DrawFPBMPNEW(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U16 num); // 20151130
extern void DrawFPBMPGray(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U16 num);
extern void DrawFPBMPBTN(U32 x, U32 y, U32 dx, U32 dy, U32 num, U16 color2); // 20160314
extern void DrawFPBMPBTN1(U32 x, U32 y, U32 dx, U32 dy, U32 num);
extern void DrawFPBMPBTN2(U32 x, U32 y, U32 dx, U32 dy, U32 num);

extern void dsp_F16x8(U32 x, U32 y, U16 *p, U32 color);
extern void DrawFChar(U32 x, U32 y, char *s, U32 color);
extern void DrawFStr16(U32 x, U32 y, char *s, U32 color);
extern void DrawFHz(U32 x, U32 y, char c1, char c2, U32 color);
extern void DrawFStr(U32 x, U32 y, char *str, U32 color);
extern void DrawFFStr(U32 x, U32 y, U16 *p, U32 color);
extern void DrawFFStrN(U32 x, U32 y, U16 *p, U32 n, U32 color);
extern void DrawFMultiLStr(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color);
extern void DrawFMultiLStrR(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color);
extern U16 DrawFMultiLStr24(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color);
extern U16 DrawFMultiLStr16(U32 x, U32 y, U16 *p, U8 num, U8 line, U32 color);

extern void SetLanguage(U16 pn);
extern void DrawUnicodeFMenuStr(U16 x, U16 y, U16 p, U32 color);
extern U16 DrawUnicodeStr(U16 x, U16 y, U16 SheetNO, U16 p, U32 color);
extern U16 DrawUnicodeStr1(U16 x, U16 y, U16 SheetNO, U16 p, U32 color);
extern U16 DrawUnicodeMulStr(U16 x, U16 y, U16 SheetNO, U16 p, U8 linemus, U8 maxline, U32 color);
extern void DrawFMenuStr(U16 p);
extern void DrawFMenuStr1(U16 x, U16 y, U16 p);
extern void DrawFMenuStr2(U16 p, U16 dx);
extern void DrawFMenuStr3(U16 p, U16 x, U16 dx);
extern void DrawFMenuStr3P(U16 pn, U16 p);
extern void DrawFMenuStrP(U16 pn, U16 p);
extern void DrawFMenuStrP1(U16 x, U16 y, U16 pn, U16 p);
extern void DrawFMenuStrUser(U16 x, U16 y, U16 p, U32 colorB,U32 colorT);//新增

extern U16 Draw24x24Str(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U32 color);
extern void Draw16x16Str(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U32 color);
extern U16 Draw24x24MulStr(U16 x, U16 y, U16 *p, U8 linemus, U16 SheetNO, U16 lines, U8 maxline, U32 color);
extern void DrawFU32(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color);
extern void DrawFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color);
extern void DrawFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFHz24X24(U32 x, U32 y, char c1, char c2, U32 color);
extern U16 Draw24X24FStr(U32 x, U32 y, char *str, U32 color);
extern U16 Draw24X24FFStr(U32 x, U32 y, U16 *p, U32 color);
extern void Draw24X24FFStrN(U32 x, U32 y, U16 *p, U32 n, U32 color);
extern void Draw24X24FPY(U32 x, U32 y, char *str, U8 ulen, U32 color);
extern void Draw24x24StrSinLineLimt(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U32 color);
extern void DrawCen24x24StrSinLineLimt(U16 x, U16 y, U16 dx, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U32 color);
extern void Draw24X24FStrByLine(U32 x, U32 y, char *str, U8 maxchar, U8 rows, U32 color);
extern void DrawFMultiLStr24LimtL(U32 x, U32 y, U16 *p, U8 num, U8 mline, U32 color);
extern void Draw32X32FStr(U32 x, U32 y, char *str, U32 color);
extern void DrawF16Str(U32 x, U32 y, char *str, U32 color);
extern void DrawFASC24(U32 x, U32 y, char *s, U32 color);
extern void DrawMFU32(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawMFU321(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawMFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawMFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawMFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color);
extern void DrawMFS32P_16x16(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color);
extern void DrawMFU32_16x16R(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawMFHEX_16(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawMFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawFASC3216(U32 x, U32 y, char *s, U32 color);
extern void DrawLFU32(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawLFU32Z(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawLFU32ZR(U32 x, U32 y, U32 n, U8 dig, U32 color);
extern void DrawLFS32P(U32 x, U32 y, S32 n, U8 dig, U8 point, U32 color);
extern void DrawErrTest(U32 x, U32 y, char *s, U32 color);
extern void DrawLFHEX(U32 x, U32 y, U32 n, U8 dig, U32 color);
//extern void DrawButton(U32 x, U32 y, U32 dx, U32 dy, U16 *p, U32 c1, U32 c2);
extern void DrawButton(KEYPAD bn, U16 *p, U32 c1, U32 c2);
extern void DrawButtonOn(KEYPAD bn, U16 c);
extern void DrawButtonOnZoomIn(KEYPAD bn, U16 ZoomInNum);//新增
extern void DrawButtonOnZoomInRes(KEYPAD bn);//新增
extern void DrawButton1(KEYPAD bn, U32 c1, U32 c2);
extern void DrawButton2(KEYPAD bn, U16 c1, U16 c2, U16 flag);
extern void DrawButton3(KEYPAD bn, U16 c1, U16 c2);
extern void DrawButtonSingleColor(KEYPAD bn, U32 c1, U32 c2);
extern void DrawButtonDisPat(KEYPAD bn, U32 colorBor, U32 colorBg);
extern void DrawButtonEdit(KEYPAD bn, U32 colorBor, U32 colorBg);
extern void DrawEditTit(RECTPAD bn, U32 strIndex, U16 sheetPage, tyden_alignment alignment, tyden_strSize size, U16 safeDist_X);
extern void DrawEdit(RECTPAD bn, U32 colorBor, U32 colorBg);
extern void DrawButtonOff(KEYPAD bn, U16 c);
extern void DrawButtonOff1(KEYPAD bn, U16 c);
extern void DrawButtonOff2(KEYPAD bn, U16 c);
extern void DrawButtonOff3(KEYPAD bn, U16 c, U16 flag);
extern void DrawButtonOff4(KEYPAD bn, U16 c);
extern void DrawButtonOff5(KEYPAD bn, U16 c);
extern void DrawTextbox(KEYPAD bn, U32 c1);
extern void InitScreen(void);
extern void BmpFillRect(U32 x, U32 y, U32 dx, U32 dy);
extern void KeyFillBmp(KEYPAD bn);
extern void dsp_DatTime(U32 x, U32 y, U16 yr, U16 md, U16 hm, U32 color);
extern void DrawFFileno(U32 x, U32 y, U16 dat, U32 color);
extern void DrawFFileno1(U32 x, U32 y, U16 dat, U32 color);
extern void DrawFFileno_24x12(U32 x, U32 y, U16 dat, U32 color);
extern void DrawFFileno_16x8(U32 x, U32 y, U16 dat, U32 color);
extern void DrawFFilename(U32 x, U32 y, char *s, U8 namelen, U32 color, U8 len, U8 dy, U8 *RealRows);
extern void DrawFFilename1(U32 x, U32 y, char *s, U8 namelen, U32 color, U8 len, U8 dy, U8 *RealRows);
extern void DrawFFileBarcode(U32 x, U32 y, U16 nn, U32 color, U8 Type);
//extern void DrawArc(U32 x1,U32 y1,U32 x2,U32 y2, U32 x3,U32 y3,U8 f,U32 color);
//extern void DrawSerialNumber(U32 x, U32 y, char n, U32 color);
//extern void DrawSerialNumber2(U32 x, U32 y, U16 n, U32 color);
extern void DrawArc(S32 x1, S32 y1, S32 x2, S32 y2, S32 x3, S32 y3, U8 f, U32 color);
extern void DrawSerialNumber(S32 x, S32 y, char n, U8 f, U32 color, U8 flag);
extern void DrawSerialNumber2(S32 x, S32 y, U16 n, U8 f, U32 color);
extern U32 GetAng(S32 dx, S32 dy);
extern U8 BCD_2Hex(U8 val);
extern U8 Hex_2BCD(U8 val);
extern void DrawFStr32(U32 x, U32 y, char *str, U32 color);
extern void DrawFASC12(U32 x, U32 y, char *s, U32 color);
extern void DrawColorGray(U32 x, U32 y, U32 dx, U32 dy);
extern void DrawButtonSys4(KEYPAD bn, U16 c1, U16 c2);
extern void DrawButtonOffOn4(KEYPAD bn);
extern void DrawButtonOffSys4(KEYPAD bn);
extern void DrawButtonSys5(KEYPAD bn, U32 c1, U32 c2);
extern void DrawButtonOnSys5(KEYPAD bn, U16 c);
extern void DrawButtonOffSys5(KEYPAD bn, U16 c);
extern U16 DrawUnicodeMulStr1(U16 x, U16 y, U16 SheetNO, U16 p, U8 linemus, U8 maxline, U32 color);
extern void Draw16x16StrLineLimt(U16 x, U16 y, U16 *p, U16 SheetNO, U16 lines, U8 lmax, U8 mlines, U32 color);
extern void Draw16X16FStrByLine(U32 x, U32 y, char *str, U8 maxchar, U8 lines, U32 color);

extern void DisTestData(U32 x1, U32 y1, S32 d1, S32 d2, U8 f, U32 color);
extern void backupAreaColor(U16 x, U16 y, U16 dx, U16 dy);
extern void recoverAreaColor(U16 x, U16 y, U16 dx, U16 dy);
extern void recoverAreaColor1(U16 x, U16 y, U16 x1, U16 y1, U16 dx, U16 dy);
void SetLanguage(U16 pn);
U16 getLanguage(void);
void initViewStatus(void);
void updateViewStatus(void);
void showTime(U16 x, U16 y);
void showTimeOnMenu(void);
void refreshPoint(U16 x, U16 y, U16 time);
void refreshPointOnMenu(U16 time);
void showDiskStatus(void);
void showTimeAndDisk(void);
void FillRectColorChang(U32 x, U32 y, U32 dx, U32 dy, U32 color1, U32 color2);
void DrawCenterStrArea1(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U32 color);
void DrawCenterTextArea1(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U16 sheet, U32 color);
void DrawCenterTextArea16(U32 x, U32 y, U16 dx, U16 dy, U8 row, U8 col, char *str, U16 sheet, U16 p, U32 color);

void SetDrawXY(U16 xx0, U16 yy0, U16 xx1, U16 yy1, U8 ff);
extern void DrawProgressbar(U32 x,U32 y,U32 dx,U32 dy,U8 percent,U16 color,U16 bkcolor);
U8 CheckTextLen(U16 *p, U16 SheetNO, U16 lines);
void ReadPointData1(U32 x, U32 y);
void RecoveData1(void);
void ReadPointData2(U32 x, U32 y);
void RecoveData2(void);
void ReadPointData3(U32 x, U32 y);
void RecoveData3(void);
// 数组在flash中的起始地址表
#define STARTWLBMP ((U32 *)(FLASHINDEXLIST + 0x00000000))       // 1
#define DESKTOPBMP ((U32 *)(FLASHINDEXLIST + 0x0000000c))       // 2
#define ASCII_16x8A ((U32 *)(FLASHINDEXLIST + 0x00000018))      // 3
#define ASCII_16x16A ((U32 *)(FLASHINDEXLIST + 0x00000024))     // 4
#define ASCII_24x12A ((U32 *)(FLASHINDEXLIST + 0x00000030))     // 5
#define ASCII_24x24A ((U32 *)(FLASHINDEXLIST + 0x0000003c))     // 6
#define ASCII_32x16A ((U32 *)(FLASHINDEXLIST + 0x00000048))     // 7
#define HzLib_CHS16A ((U32 *)(FLASHINDEXLIST + 0x00000054))     // 8	16X16宋体汉字库
#define HzLib_CHS24X24SA ((U32 *)(FLASHINDEXLIST + 0x00000060)) // 9	24X24宋体汉字库
#define HzLib_CHS24X24KA ((U32 *)(FLASHINDEXLIST + 0x0000006c)) // 10	24X24楷体汉字库
#define UNICODETOGBKDATA ((U32 *)(FLASHINDEXLIST + 0x00000078)) // 11
#define GBKTOUNICODEDATA ((U32 *)(FLASHINDEXLIST + 0x00000084)) // 12
#define PYINDEXDATA ((U32 *)(FLASHINDEXLIST + 0x00000090))      // 13
#define PYMBDATA ((U32 *)(FLASHINDEXLIST + 0x0000009c))         // 14
#define PROG_MSG21SE ((U32 *)(FLASHINDEXLIST + 0x000000a8))     // 15
#define PROG_MSG22SE ((U32 *)(FLASHINDEXLIST + 0x000000b4))     // 16
#define PROG_MSG11 ((U32 *)(FLASHINDEXLIST + 0x000000c0))       // 17
#define PROG_MSG12 ((U32 *)(FLASHINDEXLIST + 0x000000cc))       // 18
#define PROG_MSG2 ((U32 *)(FLASHINDEXLIST + 0x000000d8))        // 19
#define PROG_MSG3 ((U32 *)(FLASHINDEXLIST + 0x000000e4))        // 20
#define PROG_MSG4 ((U32 *)(FLASHINDEXLIST + 0x000000f0))        // 21
#define PROG_MSG11_E ((U32 *)(FLASHINDEXLIST + 0x000000fc))     // 22
#define PROG_MSG12_E ((U32 *)(FLASHINDEXLIST + 0x00000108))     // 23
#define PROG_MSG2_E ((U32 *)(FLASHINDEXLIST + 0x00000114))      // 24
#define PROG_MSG3_E ((U32 *)(FLASHINDEXLIST + 0x00000120))      // 25
#define PROG_MSG4_E ((U32 *)(FLASHINDEXLIST + 0x0000012c))      // 26
#define ERROR_MSG1 ((U32 *)(FLASHINDEXLIST + 0x00000138))       // 27
#define ERROR_MSG2 ((U32 *)(FLASHINDEXLIST + 0x00000144))       // 28
#define ERROR_MSG3 ((U32 *)(FLASHINDEXLIST + 0x00000150))       // 29
#define ERROR_MSG4 ((U32 *)(FLASHINDEXLIST + 0x0000015c))       // 30
#define ERROR_MSG5 ((U32 *)(FLASHINDEXLIST + 0x00000168))       // 31
#define ERROR_MSG1_E ((U32 *)(FLASHINDEXLIST + 0x00000174))     // 32
#define ERROR_MSG2_E ((U32 *)(FLASHINDEXLIST + 0x00000180))     // 33
#define INSETNAME ((U32 *)(FLASHINDEXLIST + 0x0000018C))        // 34
#define OUTSETNAME ((U32 *)(FLASHINDEXLIST + 0x00000198))       // 35
#define CONTENTMSG ((U32 *)(FLASHINDEXLIST + 0x000001a4))       // 36
#define CONTENT_RESERVE1 ((U32 *)(FLASHINDEXLIST + 0x000001b0)) // 37
#define CONTENT_RESERVE2 ((U32 *)(FLASHINDEXLIST + 0x000001bc)) // 38
#define CONTENT_RESERVE3 ((U32 *)(FLASHINDEXLIST + 0x000001c8)) // 39

#define TITLE_MENU ((U32 *)(FLASHINDEXLIST + 0x000001d4))          // 40
#define TITLE_P1 ((U32 *)(FLASHINDEXLIST + 0x000001e0))            // 41
#define TITLE_P2 ((U32 *)(FLASHINDEXLIST + 0x000001ec))            // 42
#define TITLE_WIND ((U32 *)(FLASHINDEXLIST + 0x000001f8))          // 43
#define TITLE_COUNTER ((U32 *)(FLASHINDEXLIST + 0x00000204))       // 44
#define TITLE_IMAGE ((U32 *)(FLASHINDEXLIST + 0x00000210))         // 45
#define TITLE_READ ((U32 *)(FLASHINDEXLIST + 0x0000021c))          // 46
#define TITLE_WRITE ((U32 *)(FLASHINDEXLIST + 0x00000228))         // 47
#define TITLE_RWHELP ((U32 *)(FLASHINDEXLIST + 0x00000234))        // 48
#define TITLE_INPUT ((U32 *)(FLASHINDEXLIST + 0x00000240))         // 49
#define TITLE_CURSOR ((U32 *)(FLASHINDEXLIST + 0x0000024c))        // 50
#define TITLE_INPUTMENU ((U32 *)(FLASHINDEXLIST + 0x00000258))     // 51
#define TITLE_INPUTMETHOD ((U32 *)(FLASHINDEXLIST + 0x00000264))   // 52
#define TITLE_CODE ((U32 *)(FLASHINDEXLIST + 0x00000270))          // 53
#define TITLE_INPUTPITCH ((U32 *)(FLASHINDEXLIST + 0x0000027c))    // 54
#define TITLE_INPUTOFFSET ((U32 *)(FLASHINDEXLIST + 0x00000288))   // 55
#define TITLE_INPUTZIG ((U32 *)(FLASHINDEXLIST + 0x00000294))      // 56
#define TITLE_INPUTTACKING ((U32 *)(FLASHINDEXLIST + 0x000002a0))  // 57
#define TITLE_SKIPJOG ((U32 *)(FLASHINDEXLIST + 0x000002ac))       // 58
#define TITLE_MODIADD ((U32 *)(FLASHINDEXLIST + 0x000002b8))       // 59
#define TITLE_MODIFY ((U32 *)(FLASHINDEXLIST + 0x000002c4))        // 60
#define TITLE_MODIFY1 ((U32 *)(FLASHINDEXLIST + 0x000002d0))       // 61
#define TITLE_MODIFY2 ((U32 *)(FLASHINDEXLIST + 0x000002dc))       // 62
#define TITLE_MODIFY3 ((U32 *)(FLASHINDEXLIST + 0x000002e8))       // 63
#define TITLE_CONVERT ((U32 *)(FLASHINDEXLIST + 0x000002f4))       // 64
#define TITLE_CONVERT1 ((U32 *)(FLASHINDEXLIST + 0x00000300))      // 65
#define TITLE_MOIMAGE ((U32 *)(FLASHINDEXLIST + 0x0000030c))       // 66
#define TITLE_INHELP ((U32 *)(FLASHINDEXLIST + 0x00000318))        // 67
#define TITLE_PROGRAM ((U32 *)(FLASHINDEXLIST + 0x00000324))       // 68
#define TITLE_IOSET ((U32 *)(FLASHINDEXLIST + 0x00000330))         // 69
#define TITLE_FUNCTION ((U32 *)(FLASHINDEXLIST + 0x0000033c))      // 70
#define TITLE_HELP ((U32 *)(FLASHINDEXLIST + 0x00000348))          // 71
#define TITLE_FOOTSET ((U32 *)(FLASHINDEXLIST + 0x00000354))       // 72
#define TITLE_LINEFORCE ((U32 *)(FLASHINDEXLIST + 0x00000360))     // 73
#define TITLE_CODENAME ((U32 *)(FLASHINDEXLIST + 0x0000036C))      // 74
#define TITLE_InputMenuRead ((U32 *)(FLASHINDEXLIST + 0x00000378)) // 75
#define TITLE_WORKSHOP ((U32 *)(FLASHINDEXLIST + 0x00000384))      // 76

#define TITLE_ParaCorrect ((U32 *)(FLASHINDEXLIST + 0x00000390)) // 77
#define TITLE_DISPLAYPARA ((U32 *)(FLASHINDEXLIST + 0x0000039C)) // 78
#define TITLE_WRITEMODE ((U32 *)(FLASHINDEXLIST + 0x000003a8))   // 79
#define TITLE_Reserve1 ((U32 *)(FLASHINDEXLIST + 0x000003b4))    // 80
#define TITLE_Reserve2 ((U32 *)(FLASHINDEXLIST + 0x000003C0))    // 81
#define TITLE_Reserve3 ((U32 *)(FLASHINDEXLIST + 0x000003CC))    // 82
#define TITLE_Reserve4 ((U32 *)(FLASHINDEXLIST + 0x000003D8))    // 83
#define TITLE_Reserve5 ((U32 *)(FLASHINDEXLIST + 0x000003E4))    // 84
#define TITLE_Reserve6 ((U32 *)(FLASHINDEXLIST + 0x000003F0))    // 85
#define TITLE_Reserve7 ((U32 *)(FLASHINDEXLIST + 0x000003FC))    // 86
#define TITLE_Reserve8 ((U32 *)(FLASHINDEXLIST + 0x00000408))    // 87
#define TITLE_Reserve9 ((U32 *)(FLASHINDEXLIST + 0x00000414))    // 88
#define TITLE_Reserve10 ((U32 *)(FLASHINDEXLIST + 0x00000420))   // 89

#define TITLE_MENU_E ((U32 *)(FLASHINDEXLIST + 0x0000042C))          // 90
#define TITLE_P1_E ((U32 *)(FLASHINDEXLIST + 0x00000438))            // 91
#define TITLE_P2_E ((U32 *)(FLASHINDEXLIST + 0x00000444))            // 92
#define TITLE_WIND_E ((U32 *)(FLASHINDEXLIST + 0x00000450))          // 93
#define TITLE_COUNTER_E ((U32 *)(FLASHINDEXLIST + 0x0000045C))       // 94
#define TITLE_IMAGE_E ((U32 *)(FLASHINDEXLIST + 0x00000468))         // 95
#define TITLE_READ_E ((U32 *)(FLASHINDEXLIST + 0x00000474))          // 96
#define TITLE_WRITE_E ((U32 *)(FLASHINDEXLIST + 0x00000480))         // 97
#define TITLE_RWHELP_E ((U32 *)(FLASHINDEXLIST + 0x0000048C))        // 98
#define TITLE_INPUT_E ((U32 *)(FLASHINDEXLIST + 0x00000498))         // 99
#define TITLE_CURSOR_E ((U32 *)(FLASHINDEXLIST + 0x000004A4))        // 100
#define TITLE_INPUTMENU_E ((U32 *)(FLASHINDEXLIST + 0x000004B0))     // 101
#define TITLE_INPUTMETHOD_E ((U32 *)(FLASHINDEXLIST + 0x000004BC))   // 102
#define TITLE_CODE_E ((U32 *)(FLASHINDEXLIST + 0x000004C8))          // 103
#define TITLE_INPUTPITCH_E ((U32 *)(FLASHINDEXLIST + 0x000004D4))    // 104
#define TITLE_INPUTOFFSET_E ((U32 *)(FLASHINDEXLIST + 0x000004E0))   // 105
#define TITLE_INPUTZIG_E ((U32 *)(FLASHINDEXLIST + 0x000004EC))      // 106
#define TITLE_INPUTTACKING_E ((U32 *)(FLASHINDEXLIST + 0x000004F8))  // 107
#define TITLE_SKIPJOG_E ((U32 *)(FLASHINDEXLIST + 0x00000504))       // 108
#define TITLE_MODIADD_E ((U32 *)(FLASHINDEXLIST + 0x00000510))       // 109
#define TITLE_MODIFY_E ((U32 *)(FLASHINDEXLIST + 0x0000051C))        // 110
#define TITLE_MODIFY1_E ((U32 *)(FLASHINDEXLIST + 0x00000528))       // 111
#define TITLE_MODIFY2_E ((U32 *)(FLASHINDEXLIST + 0x00000534))       // 112
#define TITLE_MODIFY3_E ((U32 *)(FLASHINDEXLIST + 0x00000540))       // 113
#define TITLE_CONVERT_E ((U32 *)(FLASHINDEXLIST + 0x0000054C))       // 114
#define TITLE_CONVERT1_E ((U32 *)(FLASHINDEXLIST + 0x00000558))      // 115
#define TITLE_MOIMAGE_E ((U32 *)(FLASHINDEXLIST + 0x00000564))       // 116
#define TITLE_INHELP_E ((U32 *)(FLASHINDEXLIST + 0x00000570))        // 117
#define TITLE_PROGRAM_E ((U32 *)(FLASHINDEXLIST + 0x0000057C))       // 118
#define TITLE_IOSET_E ((U32 *)(FLASHINDEXLIST + 0x00000588))         // 119
#define TITLE_FUNCTION_E ((U32 *)(FLASHINDEXLIST + 0x00000594))      // 120
#define TITLE_HELP_E ((U32 *)(FLASHINDEXLIST + 0x000005A0))          // 121
#define TITLE_FOOTSET_E ((U32 *)(FLASHINDEXLIST + 0x000005AC))       // 122
#define TITLE_LINEFORCE_E ((U32 *)(FLASHINDEXLIST + 0x000005B8))     // 123
#define TITLE_CODENAME_E ((U32 *)(FLASHINDEXLIST + 0x000005C4))      // 124
#define TITLE_InputMenuRead_E ((U32 *)(FLASHINDEXLIST + 0x000005D0)) // 125
#define TITLE_WORKSHOP_E ((U32 *)(FLASHINDEXLIST + 0x000005DC))      // 126

#define TITLE_ParaCorrect_E ((U32 *)(FLASHINDEXLIST + 0x000005E8)) // 127
#define TITLE_DISPLAYPARA_E ((U32 *)(FLASHINDEXLIST + 0x000005F4)) // 128
#define TITLE_WRITEMODE_E ((U32 *)(FLASHINDEXLIST + 0x00000600))   // 129
#define TITLE_Reserve1_E ((U32 *)(FLASHINDEXLIST + 0x0000060C))    // 130
#define TITLE_Reserve2_E ((U32 *)(FLASHINDEXLIST + 0x00000618))    // 131
#define TITLE_Reserve3_E ((U32 *)(FLASHINDEXLIST + 0x00000624))    // 132
#define TITLE_Reserve4_E ((U32 *)(FLASHINDEXLIST + 0x00000630))    // 133
#define TITLE_Reserve5_E ((U32 *)(FLASHINDEXLIST + 0x0000063C))    // 134
#define TITLE_Reserve6_E ((U32 *)(FLASHINDEXLIST + 0x00000648))    // 135
#define TITLE_Reserve7_E ((U32 *)(FLASHINDEXLIST + 0x00000654))    // 136
#define TITLE_Reserve8_E ((U32 *)(FLASHINDEXLIST + 0x00000660))    // 137
#define TITLE_Reserve9_E ((U32 *)(FLASHINDEXLIST + 0x0000066C))    // 138
#define TITLE_Reserve10_E ((U32 *)(FLASHINDEXLIST + 0x00000678))   // 139

#define BMPLOGO_EGH ((U32 *)(FLASHINDEXLIST + 0x00000684))    // 140
#define BMPLOGO_MODEL ((U32 *)(FLASHINDEXLIST + 0x00000690))  // 141
#define BMPLOGO_MLLOGO ((U32 *)(FLASHINDEXLIST + 0x0000069C)) // 142
#define BMPLOGO_MLSTR1 ((U32 *)(FLASHINDEXLIST + 0x000006A8)) // 143
#define BMPLOGO_GSTR2 ((U32 *)(FLASHINDEXLIST + 0x000006B4))  // 144
#define BMPLOGO_HSTR3 ((U32 *)(FLASHINDEXLIST + 0x000006C0))  // 145
#define BMPIO_STATUS ((U32 *)(FLASHINDEXLIST + 0x000006CC))   // 146
#define BMPGIF ((U32 *)(FLASHINDEXLIST + 0x000006D8))         // 147

#define ICONBMP ((U16 *)(FLASHINDEXLIST + 0x000006E4)) // 148

#define FONT_0 ((U32 *)(FLASHINDEXLIST + 0x000006F0))        // 149	字库0 24x24 Unicode Unicode0_Min - Unicode0_Max
#define FONT_1 ((U32 *)(FLASHINDEXLIST + 0x000006FC))        // 150	字库1 24x24 Unicode Unicode1_Min - Unicode1_Max
#define FONT_2 ((U32 *)(FLASHINDEXLIST + 0x00000708))        // 151	预留字库2
#define FONT_3 ((U32 *)(FLASHINDEXLIST + 0x00000714))        // 152	预留字库3
#define FONT_4 ((U32 *)(FLASHINDEXLIST + 0x00000720))        // 153	预留字库4
#define FONT_5 ((U32 *)(FLASHINDEXLIST + 0x0000072C))        // 154	预留字库5
#define FONT_6 ((U32 *)(FLASHINDEXLIST + 0x00000738))        // 155	预留字库6
#define FONT_7 ((U32 *)(FLASHINDEXLIST + 0x00000744))        // 156	预留字库7
#define MULTILINGUA ((U32 *)(FLASHINDEXLIST + 0x00000750))   // 157	多国语言
#define MULTILINGUA_1 ((U32 *)(FLASHINDEXLIST + 0x0000075C)) // 158	多国语言预留1

#define TITLE_BOR_MSG1 ((U32 *)(FLASHINDEXLIST + 0x00000768))  // 159
#define TITLE_BOR_MSG2 ((U32 *)(FLASHINDEXLIST + 0x00000774))  // 160
#define TITLE_BOR_MSG3 ((U32 *)(FLASHINDEXLIST + 0x00000780))  // 161
#define TITLE_BOR_MSG4 ((U32 *)(FLASHINDEXLIST + 0x0000078C))  // 162
#define TITLE_BOR_MSG5 ((U32 *)(FLASHINDEXLIST + 0x00000798))  // 163
#define TITLE_BOR_MSG6 ((U32 *)(FLASHINDEXLIST + 0x000007A4))  // 164
#define TITLE_BOR_MSG7 ((U32 *)(FLASHINDEXLIST + 0x000007B0))  // 165
#define TITLE_BOR_MSG8 ((U32 *)(FLASHINDEXLIST + 0x000007BC))  // 166
#define TITLE_BOR_MSG9 ((U32 *)(FLASHINDEXLIST + 0x000007C8))  // 167
#define TITLE_BOR_MSG10 ((U32 *)(FLASHINDEXLIST + 0x000007D4)) // 168

#define TITLE_BOR_MSG1_E ((U32 *)(FLASHINDEXLIST + 0x000007E0))  // 169
#define TITLE_BOR_MSG2_E ((U32 *)(FLASHINDEXLIST + 0x000007EC))  // 170
#define TITLE_BOR_MSG3_E ((U32 *)(FLASHINDEXLIST + 0x000007F8))  // 171
#define TITLE_BOR_MSG4_E ((U32 *)(FLASHINDEXLIST + 0x00000804))  // 172
#define TITLE_BOR_MSG5_E ((U32 *)(FLASHINDEXLIST + 0x00000810))  // 173
#define TITLE_BOR_MSG6_E ((U32 *)(FLASHINDEXLIST + 0x0000081C))  // 174
#define TITLE_BOR_MSG7_E ((U32 *)(FLASHINDEXLIST + 0x00000828))  // 175
#define TITLE_BOR_MSG8_E ((U32 *)(FLASHINDEXLIST + 0x00000834))  // 176
#define TITLE_BOR_MSG9_E ((U32 *)(FLASHINDEXLIST + 0x00000840))  // 177
#define TITLE_BOR_MSG10_E ((U32 *)(FLASHINDEXLIST + 0x0000084C)) // 178

#define MEM_STARTWLBMP ((U16 *)(tmpflashadd[1]))        // 1    开机界面-BMP(目前存了GIF)
#define MEM_DESKTOPBMP ((U16 *)(tmpflashadd[2]))        // 2    状态栏-BMP(USB图标等) 
#define MEM_ASCII_16x8A ((U16 *)(tmpflashadd[3]))       // 3
#define MEM_ASCII_16x16A ((U16 *)(tmpflashadd[4]))      // 4
#define MEM_ASCII_24x12A ((U16 *)(tmpflashadd[5]))      // 5
#define MEM_ASCII_24x24A ((U16 *)(tmpflashadd[6]))      // 6
#define MEM_ASCII_32x16A ((U16 *)(tmpflashadd[7]))      // 7
#define MEM_HzLib_CHS16A ((U16 *)(tmpflashadd[8]))      // 8	16X16宋体汉字库
#define MEM_HzLib_CHS24X24SA ((U16 *)(tmpflashadd[9]))  // 9	24X24宋体汉字库
#define MEM_HzLib_CHS24X24KA ((U16 *)(tmpflashadd[10])) // 10	24X24楷体汉字库
#define MEM_UNICODETOGBKDATA ((U16 *)(tmpflashadd[11])) // 11
#define MEM_GBKTOUNICODEDATA ((U16 *)(tmpflashadd[12])) // 12
#define MEM_PYINDEXDATA ((U16 *)(tmpflashadd[13]))      // 13
#define MEM_PYMBDATA ((U16 *)(tmpflashadd[14]))         // 14
#define MEM_PROG_MSG21SE ((U16 *)(tmpflashadd[15]))     // 15
#define MEM_PROG_MSG22SE ((U16 *)(tmpflashadd[16]))     // 16
#define MEM_PROG_MSG11 ((U16 *)(tmpflashadd[17]))       // 17
#define MEM_PROG_MSG12 ((U16 *)(tmpflashadd[18]))       // 18
#define MEM_PROG_MSG2 ((U16 *)(tmpflashadd[19]))        // 19
#define MEM_PROG_MSG3 ((U16 *)(tmpflashadd[20]))        // 20
#define MEM_PROG_MSG4 ((U16 *)(tmpflashadd[21]))        // 21
#define MEM_PROG_MSG11_E ((U16 *)(tmpflashadd[22]))     // 22
#define MEM_PROG_MSG12_E ((U16 *)(tmpflashadd[23]))     // 23
#define MEM_PROG_MSG2_E ((U16 *)(tmpflashadd[24]))      // 24
#define MEM_PROG_MSG3_E ((U16 *)(tmpflashadd[25]))      // 25
#define MEM_PROG_MSG4_E ((U16 *)(tmpflashadd[26]))      // 26
#define MEM_ERROR_MSG1 ((U16 *)(tmpflashadd[27]))       // 27
#define MEM_ERROR_MSG2 ((U16 *)(tmpflashadd[28]))       // 28
#define MEM_ERROR_MSG3 ((U16 *)(tmpflashadd[29]))       // 29
#define MEM_ERROR_MSG4 ((U16 *)(tmpflashadd[30]))       // 30
#define MEM_ERROR_MSG5 ((U16 *)(tmpflashadd[31]))       // 31
#define MEM_ERROR_MSG1_E ((U16 *)(tmpflashadd[32]))     // 32
#define MEM_ERROR_MSG2_E ((U16 *)(tmpflashadd[33]))     // 33
#define MEM_INSETNAME ((U16 *)(tmpflashadd[34]))        // 34
#define MEM_OUTSETNAME ((U16 *)(tmpflashadd[35]))       // 35
#define MEM_CONTENTMSG ((U16 *)(tmpflashadd[36]))       // 36
#define MEM_CONTENT_RESERVE1 ((U16 *)(tmpflashadd[37])) // 37
#define MEM_CONTENT_RESERVE2 ((U16 *)(tmpflashadd[38])) // 38
#define MEM_CONTENT_RESERVE3 ((U16 *)(tmpflashadd[39])) // 39

#define MEM_TITLE_MENU ((U16 *)(tmpflashadd[40]))          // 40
#define MEM_TITLE_P1 ((U16 *)(tmpflashadd[41]))            // 41
#define MEM_TITLE_P2 ((U16 *)(tmpflashadd[42]))            // 42
#define MEM_TITLE_WIND ((U16 *)(tmpflashadd[43]))          // 43
#define MEM_TITLE_COUNTER ((U16 *)(tmpflashadd[44]))       // 44
#define MEM_TITLE_IMAGE ((U16 *)(tmpflashadd[45]))         // 45
#define MEM_TITLE_READ ((U16 *)(tmpflashadd[46]))          // 46
#define MEM_TITLE_WRITE ((U16 *)(tmpflashadd[47]))         // 47
#define MEM_TITLE_RWHELP ((U16 *)(tmpflashadd[48]))        // 48
#define MEM_TITLE_INPUT ((U16 *)(tmpflashadd[49]))         // 49
#define MEM_TITLE_CURSOR ((U16 *)(tmpflashadd[50]))        // 50
#define MEM_TITLE_INPUTMENU ((U16 *)(tmpflashadd[51]))     // 51
#define MEM_TITLE_INPUTMETHOD ((U16 *)(tmpflashadd[52]))   // 52
#define MEM_TITLE_CODE ((U16 *)(tmpflashadd[53]))          // 53
#define MEM_TITLE_INPUTPITCH ((U16 *)(tmpflashadd[54]))    // 54
#define MEM_TITLE_INPUTOFFSET ((U16 *)(tmpflashadd[55]))   // 55
#define MEM_TITLE_INPUTZIG ((U16 *)(tmpflashadd[56]))      // 56
#define MEM_TITLE_INPUTTACKING ((U16 *)(tmpflashadd[57]))  // 57
#define MEM_TITLE_SKIPJOG ((U16 *)(tmpflashadd[58]))       // 58
#define MEM_TITLE_MODIADD ((U16 *)(tmpflashadd[59]))       // 59
#define MEM_TITLE_MODIFY ((U16 *)(tmpflashadd[60]))        // 60
#define MEM_TITLE_MODIFY1 ((U16 *)(tmpflashadd[61]))       // 61
#define MEM_TITLE_MODIFY2 ((U16 *)(tmpflashadd[62]))       // 62
#define MEM_TITLE_MODIFY3 ((U16 *)(tmpflashadd[63]))       // 63
#define MEM_TITLE_CONVERT ((U16 *)(tmpflashadd[64]))       // 64
#define MEM_TITLE_CONVERT1 ((U16 *)(tmpflashadd[65]))      // 65
#define MEM_TITLE_MOIMAGE ((U16 *)(tmpflashadd[66]))       // 66
#define MEM_TITLE_INHELP ((U16 *)(tmpflashadd[67]))        // 67
#define MEM_TITLE_PROGRAM ((U16 *)(tmpflashadd[68]))       // 68
#define MEM_TITLE_IOSET ((U16 *)(tmpflashadd[69]))         // 69
#define MEM_TITLE_FUNCTION ((U16 *)(tmpflashadd[70]))      // 70
#define MEM_TITLE_HELP ((U16 *)(tmpflashadd[71]))          // 71
#define MEM_TITLE_FOOTSET ((U16 *)(tmpflashadd[72]))       // 72
#define MEM_TITLE_LINEFORCE ((U16 *)(tmpflashadd[73]))     // 73
#define MEM_TITLE_CODENAME ((U16 *)(tmpflashadd[74]))      // 74
#define MEM_TITLE_InputMenuRead ((U16 *)(tmpflashadd[75])) // 75
#define MEM_TITLE_WORKSHOP ((U16 *)(tmpflashadd[76]))      // 76
#define MEM_TITLE_ParaCorrect ((U16 *)(tmpflashadd[77]))   // 77

#define MEM_TITLE_DISPLAYPARA ((U16 *)(tmpflashadd[78])) // 78
#define MEM_TITLE_WRITEMODE ((U16 *)(tmpflashadd[79]))   // 79
#define MEM_TITLE_Reserve1 ((U16 *)(tmpflashadd[80]))    // 80
#define MEM_TITLE_Reserve2 ((U16 *)(tmpflashadd[81]))    // 81
#define MEM_TITLE_Reserve3 ((U16 *)(tmpflashadd[82]))    // 82
#define MEM_TITLE_Reserve4 ((U16 *)(tmpflashadd[83]))    // 83
#define MEM_TITLE_Reserve5 ((U16 *)(tmpflashadd[84]))    // 84
#define MEM_TITLE_Reserve6 ((U16 *)(tmpflashadd[85]))    // 85
#define MEM_TITLE_Reserve7 ((U16 *)(tmpflashadd[86]))    // 86
#define MEM_TITLE_Reserve8 ((U16 *)(tmpflashadd[87]))    // 87
#define MEM_TITLE_Reserve9 ((U16 *)(tmpflashadd[88]))    // 88
#define MEM_TITLE_Servo_Msg ((U16 *)(tmpflashadd[89]))   // 89

#define MEM_TITLE_MENU_E ((U16 *)(tmpflashadd[90]))           // 90
#define MEM_TITLE_P1_E ((U16 *)(tmpflashadd[91]))             // 91
#define MEM_TITLE_P2_E ((U16 *)(tmpflashadd[92]))             // 92
#define MEM_TITLE_WIND_E ((U16 *)(tmpflashadd[93]))           // 93
#define MEM_TITLE_COUNTER_E ((U16 *)(tmpflashadd[94]))        // 94
#define MEM_TITLE_IMAGE_E ((U16 *)(tmpflashadd[95]))          // 95
#define MEM_TITLE_READ_E ((U16 *)(tmpflashadd[96]))           // 96
#define MEM_TITLE_WRITE_E ((U16 *)(tmpflashadd[97]))          // 97
#define MEM_TITLE_RWHELP_E ((U16 *)(tmpflashadd[98]))         // 98
#define MEM_TITLE_INPUT_E ((U16 *)(tmpflashadd[99]))          // 99
#define MEM_TITLE_CURSOR_E ((U16 *)(tmpflashadd[100]))        // 100
#define MEM_TITLE_INPUTMENU_E ((U16 *)(tmpflashadd[101]))     // 101
#define MEM_TITLE_INPUTMETHOD_E ((U16 *)(tmpflashadd[102]))   // 102
#define MEM_TITLE_CODE_E ((U16 *)(tmpflashadd[103]))          // 103
#define MEM_TITLE_INPUTPITCH_E ((U16 *)(tmpflashadd[104]))    // 104
#define MEM_TITLE_INPUTOFFSET_E ((U16 *)(tmpflashadd[105]))   // 105
#define MEM_TITLE_INPUTZIG_E ((U16 *)(tmpflashadd[106]))      // 106
#define MEM_TITLE_INPUTTACKING_E ((U16 *)(tmpflashadd[107]))  // 107
#define MEM_TITLE_SKIPJOG_E ((U16 *)(tmpflashadd[108]))       // 108
#define MEM_TITLE_MODIADD_E ((U16 *)(tmpflashadd[109]))       // 109
#define MEM_TITLE_MODIFY_E ((U16 *)(tmpflashadd[110]))        // 110
#define MEM_TITLE_MODIFY1_E ((U16 *)(tmpflashadd[111]))       // 111
#define MEM_TITLE_MODIFY2_E ((U16 *)(tmpflashadd[112]))       // 112
#define MEM_TITLE_MODIFY3_E ((U16 *)(tmpflashadd[113]))       // 113
#define MEM_TITLE_CONVERT_E ((U16 *)(tmpflashadd[114]))       // 114
#define MEM_TITLE_CONVERT1_E ((U16 *)(tmpflashadd[115]))      // 115
#define MEM_TITLE_MOIMAGE_E ((U16 *)(tmpflashadd[116]))       // 116
#define MEM_TITLE_INHELP_E ((U16 *)(tmpflashadd[117]))        // 117
#define MEM_TITLE_PROGRAM_E ((U16 *)(tmpflashadd[118]))       // 118
#define MEM_TITLE_IOSET_E ((U16 *)(tmpflashadd[119]))         // 119
#define MEM_TITLE_FUNCTION_E ((U16 *)(tmpflashadd[120]))      // 120
#define MEM_TITLE_HELP_E ((U16 *)(tmpflashadd[121]))          // 121
#define MEM_TITLE_FOOTSET_E ((U16 *)(tmpflashadd[122]))       // 122
#define MEM_TITLE_LINEFORCE_E ((U16 *)(tmpflashadd[123]))     // 123
#define MEM_TITLE_CODENAME_E ((U16 *)(tmpflashadd[124]))      // 124
#define MEM_TITLE_InputMenuRead_E ((U16 *)(tmpflashadd[125])) // 125
#define MEM_TITLE_WORKSHOP_E ((U16 *)(tmpflashadd[126]))      // 126
#define MEM_TITLE_ParaCorrect_E ((U16 *)(tmpflashadd[127]))   // 127

#define MEM_TITLE_DISPLAYPARA_E ((U16 *)(tmpflashadd[128])) // 128
#define MEM_TITLE_WRITEMODE_E ((U16 *)(tmpflashadd[129]))   // 129
#define MEM_TITLE_Reserve1_E ((U16 *)(tmpflashadd[130]))    // 130
#define MEM_TITLE_Reserve2_E ((U16 *)(tmpflashadd[131]))    // 131
#define MEM_TITLE_Reserve3_E ((U16 *)(tmpflashadd[132]))    // 132
#define MEM_TITLE_Reserve4_E ((U16 *)(tmpflashadd[133]))    // 133
#define MEM_TITLE_Reserve5_E ((U16 *)(tmpflashadd[134]))    // 134
#define MEM_TITLE_Reserve6_E ((U16 *)(tmpflashadd[135]))    // 135
#define MEM_TITLE_Reserve7_E ((U16 *)(tmpflashadd[136]))    // 136
#define MEM_TITLE_Reserve8_E ((U16 *)(tmpflashadd[137]))    // 137
#define MEM_TITLE_Reserve9_E ((U16 *)(tmpflashadd[138]))    // 138
#define MEM_TITLE_Servo_Msg_E ((U16 *)(tmpflashadd[139]))   // 139

#define MEM_BMPLOGO_EGH ((U16 *)(tmpflashadd[140]))                  // 140
#define MEM_BMPLOGO_MODEL ((U16 *)(tmpflashadd[141]))                // 141		92x32=384 单色
#define MEM_BMPLOGO_MLLOGO ((U16 *)(tmpflashadd[142]))               // 142 开机界面-LOGO-MLK-M
#define MEM_BMPLOGO_MLSTR1 ((U16 *)(tmpflashadd[143]))               // 143 开机界面-文字-MLK-M
#define MEM_BMPLOGO_GSTR2 ((U16 *)(tmpflashadd[144]))                // 144 开机界面-文字-MLK-G
#define MEM_BMPLOGO_HSTR3 ((U16 *)(tmpflashadd[145]))                // 145 
#define MEM_BMPLOGO_HSTR4 ((U16 *)(tmpflashadd[145] + 12596))        // 145
#define MEM_BMPLOGO_HSTR5 ((U16 *)(tmpflashadd[145] + 12596 + 6600)) // 145

#define MEM_BMPIO_STATUS ((U16 *)(tmpflashadd[146])) // 146
#define MEM_BMPGIF ((U16 *)(tmpflashadd[147]))       // 147		128x128 68张		彩色压缩

#define MEM_ICONBMP ((U16 *)(tmpflashadd[148])) // 148 主要图标区

#define MEM_FONT_0 ((U16 *)(tmpflashadd[149])) // 149		字库0 24x24 Unicode Unicode0_Min - Unicode0_Max
#define MEM_FONT_1 ((U16 *)(tmpflashadd[150])) // 150		字库1 24x24 Unicode Unicode1_Min - Unicode1_Max
#define MEM_FONT_2 ((U16 *)(tmpflashadd[151])) // 151		字库2 24x24 Unicode Unicode2_Min - Unicode2_Max 缅甸语

#define MEM_FONT_3 ((U16 *)(tmpflashadd[152])) // 152		预留字库3
#define MEM_FONT_4 ((U16 *)(tmpflashadd[153])) // 153		预留字库4
#define MEM_FONT_5 ((U16 *)(tmpflashadd[154])) // 154		预留字库5

#define MEM_FONT_6 ((U16 *)(tmpflashadd[155]))        // 155		预留字库6
#define MEM_FONT_7 ((U16 *)(tmpflashadd[156]))        // 156		预留字库7
#define MEM_MULTILINGUA ((U16 *)(tmpflashadd[157]))   // 157		多国语言
#define MEM_MULTILINGUA_1 ((U16 *)(tmpflashadd[158])) // 158		多国语言预留1

#define MEM_BOR_MSG1 ((U16 *)(tmpflashadd[159]))  // 159
#define MEM_BOR_MSG2 ((U16 *)(tmpflashadd[160]))  // 160
#define MEM_BOR_MSG3 ((U16 *)(tmpflashadd[161]))  // 161
#define MEM_BOR_MSG4 ((U16 *)(tmpflashadd[162]))  // 162
#define MEM_BOR_MSG5 ((U16 *)(tmpflashadd[163]))  // 163
#define MEM_BOR_MSG6 ((U16 *)(tmpflashadd[164]))  // 164
#define MEM_BOR_MSG7 ((U16 *)(tmpflashadd[165]))  // 165
#define MEM_BOR_MSG8 ((U16 *)(tmpflashadd[166]))  // 166
#define MEM_BOR_MSG9 ((U16 *)(tmpflashadd[167]))  // 167
#define MEM_BOR_MSG10 ((U16 *)(tmpflashadd[168])) // 168

#define MEM_BOR_MSG1_E ((U16 *)(tmpflashadd[169]))  // 169
#define MEM_BOR_MSG2_E ((U16 *)(tmpflashadd[170]))  // 170
#define MEM_BOR_MSG3_E ((U16 *)(tmpflashadd[171]))  // 171
#define MEM_BOR_MSG4_E ((U16 *)(tmpflashadd[172]))  // 172
#define MEM_BOR_MSG5_E ((U16 *)(tmpflashadd[173]))  // 173
#define MEM_BOR_MSG6_E ((U16 *)(tmpflashadd[174]))  // 174
#define MEM_BOR_MSG7_E ((U16 *)(tmpflashadd[175]))  // 175
#define MEM_BOR_MSG8_E ((U16 *)(tmpflashadd[176]))  // 176
#define MEM_BOR_MSG9_E ((U16 *)(tmpflashadd[177]))  // 177
#define MEM_BOR_MSG10_E ((U16 *)(tmpflashadd[178])) // 178

#define ADD_STARTWLBMP (tmpflashadd[1])        // 1
#define ADD_DESKTOPBMP (tmpflashadd[2])        // 2
#define ADD_ASCII_16x8A (tmpflashadd[3])       // 3
#define ADD_ASCII_16x16A (tmpflashadd[4])      // 4
#define ADD_ASCII_24x12A (tmpflashadd[5])      // 5
#define ADD_ASCII_24x24A (tmpflashadd[6])      // 6
#define ADD_ASCII_32x16A (tmpflashadd[7])      // 7
#define ADD_HzLib_CHS16A (tmpflashadd[8])      // 8	16X16宋体汉字库
#define ADD_HzLib_CHS24X24SA (tmpflashadd[9])  // 9	24X24宋体汉字库
#define ADD_HzLib_CHS24X24KA (tmpflashadd[10]) // 10	24X24楷体汉字库
#define ADD_UNICODETOGBKDATA (tmpflashadd[11]) // 11
#define ADD_GBKTOUNICODEDATA (tmpflashadd[12]) // 12
#define ADD_PYINDEXDATA (tmpflashadd[13])      // 13
#define ADD_PYMBDATA (tmpflashadd[14])         // 14
#define ADD_PROG_MSG21SE (tmpflashadd[15])     // 15
#define ADD_PROG_MSG22SE (tmpflashadd[16])     // 16
#define ADD_PROG_MSG11 (tmpflashadd[17])       // 17
#define ADD_PROG_MSG12 (tmpflashadd[18])       // 18
#define ADD_PROG_MSG2 (tmpflashadd[19])        // 19
#define ADD_PROG_MSG3 (tmpflashadd[20])        // 20
#define ADD_PROG_MSG4 (tmpflashadd[21])        // 21
#define ADD_PROG_MSG11_E (tmpflashadd[22])     // 22
#define ADD_PROG_MSG12_E (tmpflashadd[23])     // 23
#define ADD_PROG_MSG2_E (tmpflashadd[24])      // 24
#define ADD_PROG_MSG3_E (tmpflashadd[25])      // 25
#define ADD_PROG_MSG4_E (tmpflashadd[26])      // 26
#define ADD_ERROR_MSG1 (tmpflashadd[27])       // 27	M警告		error_msg2
#define ADD_ERROR_MSG2 (tmpflashadd[28])       // 28	E报警		prog_msg7
#define ADD_ERROR_MSG3 (tmpflashadd[29])       // 29				prog_msg6
#define ADD_ERROR_MSG4 (tmpflashadd[30])       // 30				prog_msg7已经屏蔽
#define ADD_ERROR_MSG5 (tmpflashadd[31])       // 31				prog_msg9
#define ADD_ERROR_MSG1_E (tmpflashadd[32])     // 32	M警告_e		error_msg2_e
#define ADD_ERROR_MSG2_E (tmpflashadd[33])     // 33	E报警_e		未翻译
#define ADD_INSETNAME (tmpflashadd[34])        // 34
#define ADD_OUTSETNAME (tmpflashadd[35])       // 35
#define ADD_CONTENTMSG (tmpflashadd[36])       // 36
#define ADD_CONTENT_RESERVE1 (tmpflashadd[37]) // 37
#define ADD_CONTENT_RESERVE2 (tmpflashadd[38]) // 38
#define ADD_CONTENT_RESERVE3 (tmpflashadd[39]) // 39

#define ADD_TITLE_MENU (tmpflashadd[40])          // 40
#define ADD_TITLE_P1 (tmpflashadd[41])            // 41
#define ADD_TITLE_P2 (tmpflashadd[42])            // 42
#define ADD_TITLE_WIND (tmpflashadd[43])          // 43
#define ADD_TITLE_COUNTER (tmpflashadd[44])       // 44
#define ADD_TITLE_IMAGE (tmpflashadd[45])         // 45
#define ADD_TITLE_READ (tmpflashadd[46])          // 46
#define ADD_TITLE_WRITE (tmpflashadd[47])         // 47
#define ADD_TITLE_RWHELP (tmpflashadd[48])        // 48
#define ADD_TITLE_INPUT (tmpflashadd[49])         // 49
#define ADD_TITLE_CURSOR (tmpflashadd[50])        // 50
#define ADD_TITLE_INPUTMENU (tmpflashadd[51])     // 51
#define ADD_TITLE_INPUTMETHOD (tmpflashadd[52])   // 52
#define ADD_TITLE_CODE (tmpflashadd[53])          // 53
#define ADD_TITLE_INPUTPITCH (tmpflashadd[54])    // 54
#define ADD_TITLE_INPUTOFFSET (tmpflashadd[55])   // 55
#define ADD_TITLE_INPUTZIG (tmpflashadd[56])      // 56
#define ADD_TITLE_INPUTTACKING (tmpflashadd[57])  // 57
#define ADD_TITLE_SKIPJOG (tmpflashadd[58])       // 58
#define ADD_TITLE_MODIADD (tmpflashadd[59])       // 59
#define ADD_TITLE_MODIFY (tmpflashadd[60])        // 60
#define ADD_TITLE_MODIFY1 (tmpflashadd[61])       // 61
#define ADD_TITLE_MODIFY2 (tmpflashadd[62])       // 62
#define ADD_TITLE_MODIFY3 (tmpflashadd[63])       // 63
#define ADD_TITLE_CONVERT (tmpflashadd[64])       // 64
#define ADD_TITLE_CONVERT1 (tmpflashadd[65])      // 65
#define ADD_TITLE_MOIMAGE (tmpflashadd[66])       // 66
#define ADD_TITLE_INHELP (tmpflashadd[67])        // 67
#define ADD_TITLE_PROGRAM (tmpflashadd[68])       // 68
#define ADD_TITLE_IOSET (tmpflashadd[69])         // 69
#define ADD_TITLE_FUNCTION (tmpflashadd[70])      // 70
#define ADD_TITLE_HELP (tmpflashadd[71])          // 71
#define ADD_TITLE_FOOTSET (tmpflashadd[72])       // 72
#define ADD_TITLE_LINEFORCE (tmpflashadd[73])     // 73
#define ADD_TITLE_CODENAME (tmpflashadd[74])      // 74
#define ADD_TITLE_InputMenuRead (tmpflashadd[75]) // 75
#define ADD_TITLE_WORKSHOP (tmpflashadd[76])      // 76
#define ADD_TITLE_ParaCorrect (tmpflashadd[77])   // 77

#define ADD_TITLE_DISPLAYPARA (tmpflashadd[78]) // 78
#define ADD_TITLE_WRITEMODE (tmpflashadd[79])   // 79
#define ADD_TITLE_Reserve1 (tmpflashadd[80])    // 80
#define ADD_TITLE_Reserve2 (tmpflashadd[81])    // 81
#define ADD_TITLE_Reserve3 (tmpflashadd[82])    // 82
#define ADD_TITLE_Reserve4 (tmpflashadd[83])    // 83
#define ADD_TITLE_Reserve5 (tmpflashadd[84])    // 84
#define ADD_TITLE_Reserve6 (tmpflashadd[85])    // 85
#define ADD_TITLE_Reserve7 (tmpflashadd[86])    // 86
#define ADD_TITLE_Reserve8 (tmpflashadd[87])    // 87
#define ADD_TITLE_Reserve9 (tmpflashadd[88])    // 88
#define ADD_SERVO_MSG (tmpflashadd[89])         // 89

#define ADD_TITLE_MENU_E (tmpflashadd[90])           // 90
#define ADD_TITLE_P1_E (tmpflashadd[91])             // 91
#define ADD_TITLE_P2_E (tmpflashadd[92])             // 92
#define ADD_TITLE_WIND_E (tmpflashadd[93])           // 93
#define ADD_TITLE_COUNTER_E (tmpflashadd[94])        // 94
#define ADD_TITLE_IMAGE_E (tmpflashadd[95])          // 95
#define ADD_TITLE_READ_E (tmpflashadd[96])           // 96
#define ADD_TITLE_WRITE_E (tmpflashadd[97])          // 97
#define ADD_TITLE_RWHELP_E (tmpflashadd[98])         // 98
#define ADD_TITLE_INPUT_E (tmpflashadd[99])          // 99
#define ADD_TITLE_CURSOR_E (tmpflashadd[100])        // 100
#define ADD_TITLE_INPUTMENU_E (tmpflashadd[101])     // 101
#define ADD_TITLE_INPUTMETHOD_E (tmpflashadd[102])   // 102
#define ADD_TITLE_CODE_E (tmpflashadd[103])          // 103
#define ADD_TITLE_INPUTPITCH_E (tmpflashadd[104])    // 104
#define ADD_TITLE_INPUTOFFSET_E (tmpflashadd[105])   // 105
#define ADD_TITLE_INPUTZIG_E (tmpflashadd[106])      // 106
#define ADD_TITLE_INPUTTACKING_E (tmpflashadd[107])  // 107
#define ADD_TITLE_SKIPJOG_E (tmpflashadd[108])       // 108
#define ADD_TITLE_MODIADD_E (tmpflashadd[109])       // 109
#define ADD_TITLE_MODIFY_E (tmpflashadd[110])        // 110
#define ADD_TITLE_MODIFY1_E (tmpflashadd[111])       // 111
#define ADD_TITLE_MODIFY2_E (tmpflashadd[112])       // 112
#define ADD_TITLE_MODIFY3_E (tmpflashadd[113])       // 113
#define ADD_TITLE_CONVERT_E (tmpflashadd[114])       // 114
#define ADD_TITLE_CONVERT1_E (tmpflashadd[115])      // 115
#define ADD_TITLE_MOIMAGE_E (tmpflashadd[116])       // 116
#define ADD_TITLE_INHELP_E (tmpflashadd[117])        // 117
#define ADD_TITLE_PROGRAM_E (tmpflashadd[118])       // 118
#define ADD_TITLE_IOSET_E (tmpflashadd[119])         // 119
#define ADD_TITLE_FUNCTION_E (tmpflashadd[120])      // 120
#define ADD_TITLE_HELP_E (tmpflashadd[121])          // 121
#define ADD_TITLE_FOOTSET_E (tmpflashadd[122])       // 122
#define ADD_TITLE_LINEFORCE_E (tmpflashadd[123])     // 123
#define ADD_TITLE_CODENAME_E (tmpflashadd[124])      // 124
#define ADD_TITLE_InputMenuRead_E (tmpflashadd[125]) // 125
#define ADD_TITLE_WORKSHOP_E (tmpflashadd[126])      // 126
#define ADD_TITLE_ParaCorrect_E (tmpflashadd[127])   // 127

#define ADD_TITLE_DISPLAYPARA_E (tmpflashadd[128]) // 128
#define ADD_TITLE_WRITEMODE_E (tmpflashadd[129])   // 129
#define ADD_TITLE_Reserve1_E (tmpflashadd[130])    // 130
#define ADD_TITLE_Reserve2_E (tmpflashadd[131])    // 131
#define ADD_TITLE_Reserve3_E (tmpflashadd[132])    // 132
#define ADD_TITLE_Reserve4_E (tmpflashadd[133])    // 133
#define ADD_TITLE_Reserve5_E (tmpflashadd[134])    // 134
#define ADD_TITLE_Reserve6_E (tmpflashadd[135])    // 135
#define ADD_TITLE_Reserve7_E (tmpflashadd[136])    // 136
#define ADD_TITLE_Reserve8_E (tmpflashadd[137])    // 137
#define ADD_TITLE_Reserve9_E (tmpflashadd[138])    // 138
#define ADD_SERVO_MSG_E (tmpflashadd[139])         // 139

#define ADD_BMPLOGO_EGH (tmpflashadd[140])    // 140
#define ADD_BMPLOGO_MODEL (tmpflashadd[141])  // 141
#define ADD_BMPLOGO_MLLOGO (tmpflashadd[142]) // 142
#define ADD_BMPLOGO_MLSTR1 (tmpflashadd[143]) // 143
#define ADD_BMPLOGO_GSTR2 (tmpflashadd[144])  // 144
#define ADD_BMPLOGO_GSTR3 (tmpflashadd[145])  // 145
#define ADD_BMPIO_STATUS (tmpflashadd[146])   // 146
#define ADD_BMPGIF (tmpflashadd[147])         // 147

#define ADD_ICONBMP (tmpflashadd[148]) // 148

#define ADD_FONT_0 (tmpflashadd[149])        // 149		字库0 24x24 Unicode Unicode0_Min - Unicode0_Max
#define ADD_FONT_1 (tmpflashadd[150])        // 150		字库1 24x24 Unicode Unicode1_Min - Unicode1_Max
#define ADD_FONT_2 (tmpflashadd[151])        // 151		预留字库2
#define ADD_FONT_3 (tmpflashadd[152])        // 152		预留字库3
#define ADD_FONT_4 (tmpflashadd[153])        // 153		预留字库4
#define ADD_FONT_5 (tmpflashadd[154])        // 154		预留字库5
#define ADD_FONT_6 (tmpflashadd[155])        // 155		预留字库6
#define ADD_FONT_7 (tmpflashadd[156])        // 156		预留字库7
#define ADD_MULTILINGUA (tmpflashadd[157])   // 157		多国语言
#define ADD_MULTILINGUA_1 (tmpflashadd[158]) // 158		多国语言预留1

#define ADD_BOR_MSG1 (tmpflashadd[159])  // 159
#define ADD_BOR_MSG2 (tmpflashadd[160])  // 160
#define ADD_BOR_MSG3 (tmpflashadd[161])  // 161
#define ADD_BOR_MSG4 (tmpflashadd[162])  // 162
#define ADD_BOR_MSG5 (tmpflashadd[163])  // 163
#define ADD_BOR_MSG6 (tmpflashadd[164])  // 164
#define ADD_BOR_MSG7 (tmpflashadd[165])  // 165
#define ADD_BOR_MSG8 (tmpflashadd[166])  // 166
#define ADD_BOR_MSG9 (tmpflashadd[167])  // 167
#define ADD_BOR_MSG10 (tmpflashadd[168]) // 168

#define ADD_BOR_MSG1_E (tmpflashadd[169])  // 169
#define ADD_BOR_MSG2_E (tmpflashadd[170])  // 170
#define ADD_BOR_MSG3_E (tmpflashadd[171])  // 171
#define ADD_BOR_MSG4_E (tmpflashadd[172])  // 172
#define ADD_BOR_MSG5_E (tmpflashadd[173])  // 173
#define ADD_BOR_MSG6_E (tmpflashadd[174])  // 174
#define ADD_BOR_MSG7_E (tmpflashadd[175])  // 175
#define ADD_BOR_MSG8_E (tmpflashadd[176])  // 176
#define ADD_BOR_MSG9_E (tmpflashadd[177])  // 177
#define ADD_BOR_MSG10_E (tmpflashadd[178]) // 178

#define FT_STARTWLBMP (flashdataflag[1])        // 1
#define FT_DESKTOPBMP (flashdataflag[2])        // 2
#define FT_ASCII_16x8A (flashdataflag[3])       // 3
#define FT_ASCII_16x16A (flashdataflag[4])      // 4
#define FT_ASCII_24x12A (flashdataflag[5])      // 5
#define FT_ASCII_24x24A (flashdataflag[6])      // 6
#define FT_ASCII_32x16A (flashdataflag[7])      // 7
#define FT_HzLib_CHS16A (flashdataflag[8])      // 8	16X16宋体汉字库
#define FT_HzLib_CHS24X24SA (flashdataflag[9])  // 9	24X24宋体汉字库
#define FT_HzLib_CHS24X24KA (flashdataflag[10]) // 10	24X24楷体汉字库
#define FT_UNICODETOGBKDATA (flashdataflag[11]) // 11
#define FT_GBKTOUNICODEDATA (flashdataflag[12]) // 12
#define FT_PYINDEXDATA (flashdataflag[13])      // 13
#define FT_PYMBDATA (flashdataflag[14])         // 14
#define FT_PROG_MSG21SE (flashdataflag[15])     // 15
#define FT_PROG_MSG22SE (flashdataflag[16])     // 16
#define FT_PROG_MSG11 (flashdataflag[17])       // 17
#define FT_PROG_MSG12 (flashdataflag[18])       // 18
#define FT_PROG_MSG2 (flashdataflag[19])        // 19
#define FT_PROG_MSG3 (flashdataflag[20])        // 20
#define FT_PROG_MSG4 (flashdataflag[21])        // 21
#define FT_PROG_MSG11_E (flashdataflag[22])     // 22
#define FT_PROG_MSG12_E (flashdataflag[23])     // 23
#define FT_PROG_MSG2_E (flashdataflag[24])      // 24
#define FT_PROG_MSG3_E (flashdataflag[25])      // 25
#define FT_PROG_MSG4_E (flashdataflag[26])      // 26
#define FT_ERROR_MSG1 (flashdataflag[27])       // 27	M警告		error_msg2
#define FT_ERROR_MSG2 (flashdataflag[28])       // 28	E报警		prog_msg7
#define FT_ERROR_MSG3 (flashdataflag[29])       // 29				prog_msg6
#define FT_ERROR_MSG4 (flashdataflag[30])       // 30				prog_msg7已经屏蔽
#define FT_ERROR_MSG5 (flashdataflag[31])       // 31				prog_msg9
#define FT_ERROR_MSG1_E (flashdataflag[32])     // 32	M警告_e		error_msg2_e
#define FT_ERROR_MSG2_E (flashdataflag[33])     // 33	E报警_e		未翻译
#define FT_INSETNAME (flashdataflag[34])        // 34
#define FT_OUTSETNAME (flashdataflag[35])       // 35
#define FT_CONTENTMSG (flashdataflag[36])       // 36
#define FT_CONTENT_RESERVE1 (flashdataflag[37]) // 37
#define FT_CONTENT_RESERVE2 (flashdataflag[38]) // 38
#define FT_CONTENT_RESERVE3 (flashdataflag[39]) // 39

#define FT_TITLE_MENU (flashdataflag[40])          // 40
#define FT_TITLE_P1 (flashdataflag[41])            // 41
#define FT_TITLE_P2 (flashdataflag[42])            // 42
#define FT_TITLE_WIND (flashdataflag[43])          // 43
#define FT_TITLE_COUNTER (flashdataflag[44])       // 44
#define FT_TITLE_IMAGE (flashdataflag[45])         // 45
#define FT_TITLE_READ (flashdataflag[46])          // 46
#define FT_TITLE_WRITE (flashdataflag[47])         // 47
#define FT_TITLE_RWHELP (flashdataflag[48])        // 48
#define FT_TITLE_INPUT (flashdataflag[49])         // 49
#define FT_TITLE_CURSOR (flashdataflag[50])        // 50
#define FT_TITLE_INPUTMENU (flashdataflag[51])     // 51
#define FT_TITLE_INPUTMETHOD (flashdataflag[52])   // 52
#define FT_TITLE_CODE (flashdataflag[53])          // 53
#define FT_TITLE_INPUTPITCH (flashdataflag[54])    // 54
#define FT_TITLE_INPUTOFFSET (flashdataflag[55])   // 55
#define FT_TITLE_INPUTZIG (flashdataflag[56])      // 56
#define FT_TITLE_INPUTTACKING (flashdataflag[57])  // 57
#define FT_TITLE_SKIPJOG (flashdataflag[58])       // 58
#define FT_TITLE_MODIADD (flashdataflag[59])       // 59
#define FT_TITLE_MODIFY (flashdataflag[60])        // 60
#define FT_TITLE_MODIFY1 (flashdataflag[61])       // 61
#define FT_TITLE_MODIFY2 (flashdataflag[62])       // 62
#define FT_TITLE_MODIFY3 (flashdataflag[63])       // 63
#define FT_TITLE_CONVERT (flashdataflag[64])       // 64
#define FT_TITLE_CONVERT1 (flashdataflag[65])      // 65
#define FT_TITLE_MOIMAGE (flashdataflag[66])       // 66
#define FT_TITLE_INHELP (flashdataflag[67])        // 67
#define FT_TITLE_PROGRAM (flashdataflag[68])       // 68
#define FT_TITLE_IOSET (flashdataflag[69])         // 69
#define FT_TITLE_FUNCTION (flashdataflag[70])      // 70
#define FT_TITLE_HELP (flashdataflag[71])          // 71
#define FT_TITLE_FOOTSET (flashdataflag[72])       // 72
#define FT_TITLE_LINEFORCE (flashdataflag[73])     // 73
#define FT_TITLE_CODENAME (flashdataflag[74])      // 74
#define FT_TITLE_InputMenuRead (flashdataflag[75]) // 75
#define FT_TITLE_WORKSHOP (flashdataflag[76])      // 76
#define FT_TITLE_ParaCorrect (flashdataflag[77])   // 77

#define FT_TITLE_DISPLAYPARA (flashdataflag[78]) // 78
#define FT_TITLE_WRITEMODE (flashdataflag[79])   // 79
#define FT_TITLE_Reserve1 (flashdataflag[80])    // 80
#define FT_TITLE_Reserve2 (flashdataflag[81])    // 81
#define FT_TITLE_Reserve3 (flashdataflag[82])    // 82
#define FT_TITLE_Reserve4 (flashdataflag[83])    // 83
#define FT_TITLE_Reserve5 (flashdataflag[84])    // 84
#define FT_TITLE_Reserve6 (flashdataflag[85])    // 85
#define FT_TITLE_Reserve7 (flashdataflag[86])    // 86
#define FT_TITLE_Reserve8 (flashdataflag[87])    // 87
#define FT_TITLE_Reserve9 (flashdataflag[88])    // 88
#define FT_TITLE_Reserve10 (flashdataflag[89])   // 89

#define FT_TITLE_MENU_E (flashdataflag[90])           // 90
#define FT_TITLE_P1_E (flashdataflag[91])             // 91
#define FT_TITLE_P2_E (flashdataflag[92])             // 92
#define FT_TITLE_WIND_E (flashdataflag[93])           // 93
#define FT_TITLE_COUNTER_E (flashdataflag[94])        // 94
#define FT_TITLE_IMAGE_E (flashdataflag[95])          // 95
#define FT_TITLE_READ_E (flashdataflag[96])           // 96
#define FT_TITLE_WRITE_E (flashdataflag[97])          // 97
#define FT_TITLE_RWHELP_E (flashdataflag[98])         // 98
#define FT_TITLE_INPUT_E (flashdataflag[99])          // 99
#define FT_TITLE_CURSOR_E (flashdataflag[100])        // 100
#define FT_TITLE_INPUTMENU_E (flashdataflag[101])     // 101
#define FT_TITLE_INPUTMETHOD_E (flashdataflag[102])   // 102
#define FT_TITLE_CODE_E (flashdataflag[103])          // 103
#define FT_TITLE_INPUTPITCH_E (flashdataflag[104])    // 104
#define FT_TITLE_INPUTOFFSET_E (flashdataflag[105])   // 105
#define FT_TITLE_INPUTZIG_E (flashdataflag[106])      // 106
#define FT_TITLE_INPUTTACKING_E (flashdataflag[107])  // 107
#define FT_TITLE_SKIPJOG_E (flashdataflag[108])       // 108
#define FT_TITLE_MODIADD_E (flashdataflag[109])       // 109
#define FT_TITLE_MODIFY_E (flashdataflag[110])        // 110
#define FT_TITLE_MODIFY1_E (flashdataflag[111])       // 111
#define FT_TITLE_MODIFY2_E (flashdataflag[112])       // 112
#define FT_TITLE_MODIFY3_E (flashdataflag[113])       // 113
#define FT_TITLE_CONVERT_E (flashdataflag[114])       // 114
#define FT_TITLE_CONVERT1_E (flashdataflag[115])      // 115
#define FT_TITLE_MOIMAGE_E (flashdataflag[116])       // 116
#define FT_TITLE_INHELP_E (flashdataflag[117])        // 117
#define FT_TITLE_PROGRAM_E (flashdataflag[118])       // 118
#define FT_TITLE_IOSET_E (flashdataflag[119])         // 119
#define FT_TITLE_FUNCTION_E (flashdataflag[120])      // 120
#define FT_TITLE_HELP_E (flashdataflag[121])          // 121
#define FT_TITLE_FOOTSET_E (flashdataflag[122])       // 122
#define FT_TITLE_LINEFORCE_E (flashdataflag[123])     // 123
#define FT_TITLE_CODENAME_E (flashdataflag[124])      // 124
#define FT_TITLE_InputMenuRead_E (flashdataflag[125]) // 125
#define FT_TITLE_WORKSHOP_E (flashdataflag[126])      // 126
#define FT_TITLE_ParaCorrect_E (flashdataflag[127])   // 127

#define FT_TITLE_DISPLAYPARA_E (flashdataflag[128]) // 128
#define FT_TITLE_WRITEMODE_E (flashdataflag[129])   // 129
#define FT_TITLE_Reserve1_E (flashdataflag[130])    // 130
#define FT_TITLE_Reserve2_E (flashdataflag[131])    // 131
#define FT_TITLE_Reserve3_E (flashdataflag[132])    // 132
#define FT_TITLE_Reserve4_E (flashdataflag[133])    // 133
#define FT_TITLE_Reserve5_E (flashdataflag[134])    // 134
#define FT_TITLE_Reserve6_E (flashdataflag[135])    // 135
#define FT_TITLE_Reserve7_E (flashdataflag[136])    // 136
#define FT_TITLE_Reserve8_E (flashdataflag[137])    // 137
#define FT_TITLE_Reserve9_E (flashdataflag[138])    // 138
#define FT_TITLE_Reserve10_E (flashdataflag[139])   // 139

#define FT_BMPLOGO_EGH (flashdataflag[140])    // 140
#define FT_BMPLOGO_MODEL (flashdataflag[141])  // 141
#define FT_BMPLOGO_MLLOGO (flashdataflag[142]) // 142
#define FT_BMPLOGO_MLSTR1 (flashdataflag[143]) // 143
#define FT_BMPLOGO_GSTR2 (flashdataflag[144])  // 144
#define FT_BMPLOGO_GSTR3 (flashdataflag[145])  // 145
#define FT_BMPIO_STATUS (flashdataflag[146])   // 146
#define FT_BMPGIF (flashdataflag[147])         // 147

#define FT_ICONBMP (flashdataflag[148]) // 148

#define FT_FONT_0 (flashdataflag[149])        // 149		字库0 24x24 Unicode Unicode0_Min - Unicode0_Max
#define FT_FONT_1 (flashdataflag[150])        // 150		字库1 24x24 Unicode Unicode1_Min - Unicode1_Max
#define FT_FONT_2 (flashdataflag[151])        // 151		预留字库2
#define FT_FONT_3 (flashdataflag[152])        // 152		预留字库3
#define FT_FONT_4 (flashdataflag[153])        // 153		预留字库4
#define FT_FONT_5 (flashdataflag[154])        // 154		预留字库5
#define FT_FONT_6 (flashdataflag[155])        // 155		预留字库6
#define FT_FONT_7 (flashdataflag[156])        // 156		预留字库7
#define FT_MULTILINGUA (flashdataflag[157])   // 157		多国语言
#define FT_MULTILINGUA_1 (flashdataflag[158]) // 158		多国语言预留1

#define FT_BOR_MSG1 (flashdataflag[159])  // 159
#define FT_BOR_MSG2 (flashdataflag[160])  // 160
#define FT_BOR_MSG3 (flashdataflag[161])  // 161
#define FT_BOR_MSG4 (flashdataflag[162])  // 162
#define FT_BOR_MSG5 (flashdataflag[163])  // 163
#define FT_BOR_MSG6 (flashdataflag[164])  // 164
#define FT_BOR_MSG7 (flashdataflag[165])  // 165
#define FT_BOR_MSG8 (flashdataflag[166])  // 166
#define FT_BOR_MSG9 (flashdataflag[167])  // 167
#define FT_BOR_MSG10 (flashdataflag[168]) // 168

#define FT_BOR_MSG1_E (flashdataflag[169])  // 169
#define FT_BOR_MSG2_E (flashdataflag[170])  // 170
#define FT_BOR_MSG3_E (flashdataflag[171])  // 171
#define FT_BOR_MSG4_E (flashdataflag[172])  // 172
#define FT_BOR_MSG5_E (flashdataflag[173])  // 173
#define FT_BOR_MSG6_E (flashdataflag[174])  // 174
#define FT_BOR_MSG7_E (flashdataflag[175])  // 175
#define FT_BOR_MSG8_E (flashdataflag[176])  // 176
#define FT_BOR_MSG9_E (flashdataflag[177])  // 177
#define FT_BOR_MSG10_E (flashdataflag[178]) // 178

#define USE_PROG_MSG2SE ((U16 *)(tmpflashadd[188]))
#define UADD_PROG_MSG2SE (tmpflashadd[188])

#define USE_PROG_MSG1 ((U16 *)(tmpflashadd[189]))
#define UADD_PROG_MSG1 (tmpflashadd[189])

#define USE_PROG_MSG2 ((U16 *)(tmpflashadd[190]))
#define UADD_PROG_MSG2 (tmpflashadd[190])

#define USE_PROG_MSG3 ((U16 *)(tmpflashadd[191]))
#define UADD_PROG_MSG3 (tmpflashadd[191])

#define USE_PROG_MSG4 ((U16 *)(tmpflashadd[192]))
#define UADD_PROG_MSG4 (tmpflashadd[192])

#define USE_ERROR_MSG ((U16 *)(tmpflashadd[193]))
#define UADD_ERROR_MSG (tmpflashadd[193])

#define USE_TITLE_CODENAME ((U16 *)(tmpflashadd[194]))
#define UADD_TITLE_CODENAME (tmpflashadd[194])

//#define		USE_PROG_MSG6				((U16 *)(tmpflashadd[165]))
//#define		UADD_PROG_MSG6				(tmpflashadd[165])
//
#define USE_PROG_MSG7 ((U16 *)(tmpflashadd[196]))
#define UADD_PROG_MSG7 (tmpflashadd[196])
//
//#define		USE_PROG_MSG8				((U16 *)(tmpflashadd[167]))
//#define		UADD_PROG_MSG8				(tmpflashadd[167])
//
//#define		USE_PROG_MSG9				((U16 *)(tmpflashadd[168]))
//#define		UADD_PROG_MSG9				(tmpflashadd[168])
//
//
#define USE_PROG_MSG5 ((U16 *)(tmpflashadd[199]))
#define UADD_PROG_MSG5 (tmpflashadd[199])

#define STARTWLBMPADD tmpflashadd[49]
#define MEM_BMPLOGO_ML1 ((U16 *)(tmpflashadd[142]))      // (MEM_BMPLOGO_MLLOGO)
#define MEM_BMPLOGO_ML2 ((U16 *)(tmpflashadd[142] + 46)) // (MEM_BMPLOGO_MLLOGO+23)
#define MEM_BMPLOGO_ML3 ((U16 *)(tmpflashadd[142] + 74)) // (MEM_BMPLOGO_MLLOGO+37)
#define MEM_BMPLOGO_MR1 ((U16 *)(tmpflashadd[142] + 126))
#define MEM_BMPLOGO_MR2 ((U16 *)(tmpflashadd[142] + 132))
#define MEM_BMPLOGO_MR3 ((U16 *)(tmpflashadd[142] + 156))
#define MEM_BMPLOGO_MR4 ((U16 *)(tmpflashadd[142] + 210))

#define MEM_BMPLOGO_MLKG ((U16 *)(tmpflashadd[141]))             // (MEM_BMPLOGO_MODEL)
#define MEM_BMPLOGO_MLKH ((U16 *)(tmpflashadd[141] + 384))       // (MEM_BMPLOGO_MODEL+192)
#define MEM_BMPLOGO_1010 ((U16 *)(tmpflashadd[141] + 384 * 2))   // (MEM_BMPLOGO_MODEL+192*2)
#define MEM_BMPLOGO_1510 ((U16 *)(tmpflashadd[141] + 384 * 3))   // (MEM_BMPLOGO_MODEL+192*3)
#define MEM_BMPLOGO_2010 ((U16 *)(tmpflashadd[141] + 384 * 4))   // (MEM_BMPLOGO_MODEL+192*4)
#define MEM_BMPLOGO_2516 ((U16 *)(tmpflashadd[141] + 384 * 5))   // (MEM_BMPLOGO_MODEL+192*5)
#define MEM_BMPLOGO_3020 ((U16 *)(tmpflashadd[141] + 384 * 6))   // (MEM_BMPLOGO_MODEL+192*6)
#define MEM_BMPLOGO_4030 ((U16 *)(tmpflashadd[141] + 384 * 7))   // (MEM_BMPLOGO_MODEL+192*7)
#define MEM_BMPLOGO_5040 ((U16 *)(tmpflashadd[141] + 384 * 8))   // (MEM_BMPLOGO_MODEL+192*8)
#define MEM_BMPLOGO_5050 ((U16 *)(tmpflashadd[141] + 384 * 9))   // (MEM_BMPLOGO_MODEL+192*9)
#define MEM_BMPLOGO_6030 ((U16 *)(tmpflashadd[141] + 384 * 10))  // (MEM_BMPLOGO_MODEL+192*10)
#define MEM_BMPLOGO_6050 ((U16 *)(tmpflashadd[141] + 384 * 11))  // (MEM_BMPLOGO_MODEL+192*11)
#define MEM_BMPLOGO_10050 ((U16 *)(tmpflashadd[141] + 384 * 12)) // (MEM_BMPLOGO_MODEL+192*12)
#define MEM_BMPLOGO_6040 ((U16 *)(tmpflashadd[141] + 384 * 13))  // (MEM_BMPLOGO_MODEL+192*13)
#define MEM_BMPLOGO_311 ((U16 *)(tmpflashadd[141] + 384 * 14))   // (MEM_BMPLOGO_MODEL+192*14)
#define MEM_BMPLOGO_326 ((U16 *)(tmpflashadd[141] + 384 * 15))   // (MEM_BMPLOGO_MODEL+192*15)
#define MEM_BMPLOGO_342 ((U16 *)(tmpflashadd[141] + 384 * 16))   // (MEM_BMPLOGO_MODEL+192*16)

void Draw_Circle(U16 Xpos, U16 Ypos, U16 Radius, U32 Color);
void Draw_FullCircle(U16 Xpos, U16 Ypos, U16 Radius, U32 Color);
void Draw_FullPoint(U16 Xpos, U16 Ypos, U16 Radius, U32 Color);
void Draw_BigCross(U16 x, U16 y, U16 color);
void Draw_BigCrossLimit(U16 x, U16 y, U16 color,U8 convas);
void Draw_SmallCross(S32 x, S32 y, U16 color);
void Draw_SmallCrossLimit(S32 x, S32 y, U16 color,U8 convas);
void Draw_Code_BMP(U32 x, U32 y, U32 dx, U32 dy, U32 num);
void Draw_Code_HSEP(S32 x, S32 y, U8 n, U32 color);
void Draw_Code_Num(S32 x, S32 y, U16 n, U32 color);
void ResetData1(void);
void ResetData2(void);

#endif
