//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  function.h								*
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


#ifndef __FUNCTION_H 
#define __FUNCTION_H


#include "stm32cpu.h"
#include "main.h"

extern U8 g_old_minute;
extern volatile U16 g_feedct;
extern volatile U8 g_init_flag;
extern volatile U16 g_patd_fno;
extern volatile U8 g_BorSTflag;

int absInt(int dat);
S32 abs32(S32 dat);
void mymemset(void *s,uint8_t c,uint32_t count);
void mysetU16(U16 *inbt,U16 dat,U32 len);
void copyU16(U16 *tar,U16 *sr,U32 len);
void moveBuff(U8 *buff,S32 dlen,U32 baseAddr,U32 maxbuff);
uint8_t strEqual(char *str1,char *str2);
U8 DataCheckSum(U16 *data,U32 len,U8 Type);
U8 CheckDrawScale(U8 tt,U8 PatType, U32 Patlen,U8 upmac);
U8 CheckDrawData(U32 pno11,U8 flag);
void BarcodeCompare(void);
U16 FinePYFirstAdd(U8 *inabc, U16 c);
U16 ReadHzData(U16 *hzbuf, U8 len, U16 add);
void strcopy(U8 *src,U8 *des,U16 len);
U16 getStrLen(U8 *buf);
U8 getDiskStatus(void);
U8 getDiskStatus1(void);
void setDiskStatus(U8 status);
void clrShowTime(void);
U16 getFileNumsByIREG(U16 *iReg,U8 ftype);
U16 getFileNumsByReg(U16 *reg,U8 ftype);
U8 getFileTypeByIREG(U16 *iReg,U16 fileno);
void setKeyPos(KEYPAD *key,U16 xpos,U16 ypos,U16 xsize,U16 ysize);
void BackupSHREG(U8 Type);
void RestoreSHREG(U8 Type);
void BackupIREG(void);
void RestoreIREG(void);
void mitBackupSREG(void);
void mitRecovrSREG(void);
void BackupSREG(U8 block,U8 loop,U32 len,u8 flag);
void RecovrSREG(U8 block,u8 flag);
void AnaPattern(U8 mode,U8 Type,U32 Patlen);
void AnaPattern2(U8 mode);
void GetDrawPatPos(U32 feedc,S32 scale,S16 *ox,S16 *oy,U8 Type,U32 Patlen,U8 flag);
void DrawBigPoint(U16 x,U16 y,U16 color);
U8 newDrawPoint(U8 vn,U32 feedc,S32 scale,S16 ox1,S16 oy1,U8 flag);
U8 viewLPoint(U16 feedc,S32 scale,S16 ox1,S16 oy1);
void clrLPoint(void);
void getFeedXY(U16 feed,S16 *outx,S16 *outy);
void getFeedXY1(S16 *outx,S16 *outy);
int ReadFileIndex(U16 cmd,U16 pold);
U8 ReadMemorryList(U32 pno20);
U8 CheckSREG(U8 wrmode,U8 patType,U32 Patlen);
U8 MemoryDrv_Opera(U16 dispcmd,U16 PARA1);
U8 ReadPatList(U8 tt,U16 pold);
void CailImageRange(COORDINATE2 *POS,U16 x0,U16 y0,U16 x1,U16 y1);
void SaveBakData(U16 dat);
U8 pathTohome(U32 tmp_pno);
U8 Go_Home(U32 tmp_pno,U8 flag) ;
U8 Send_Pfoot(U8 mode,U32 tmp_pno,U8 flag);
U8 NeddleUp(U32 tmp_pno);
U8 Comm_Move(S16 x,S16 y,U8 para,U32 tmp_pno,U8 borflag,U8 movfeed);
U8 Comm_Move1(S16 x,S16 y,U8 para,U32 tmp_pno,U8 borflag,U8 movfeed);
void Read_Move1(U32 tmp_pno);
U8 Comm_RelationMove(S16 x,S16 y,U32 tmp_pno,U8 borflag,U8 movfeed);
void Wait_Move_End(void);
U8 Read_xyposi(void);
U8 Read_ModifyPara(void);
U8 CopyFileCom(U16 cmd,U16* p,U8 cnt);
U8 Read_DCMDSTATUS(U32 Waitime);
void Go_Stpoint(U32 tmp_pno,U8 flag);
U8 OutputExcode(U8 excode,U16 tmppo);
U8 OutputExcode1(U8 excode,U16 tmppo,U8 swi);
void dis_data(U16 dat,U8  div);
U8 CailDate(U8 type,S8 val,S8 year,S8 mon);
U8 WriteTimer(U16 cmd,U8 t[5]);
U32 Cail_Yneedle(S16 angle,U16 maxy,U16 miny);
U32 MakeAutPassword(U8 *srcword,U8 mainBoardType,u8 len);
U8 checkuserpassword(U8 *inputpassword,U8 index,U8 len);
U8 checkusername(U8 *inputname,U8 len);
void getDateRunRecord(U32 disDate,U8 *Msg,U16 *Index,U16 *maxpage,U32 *startAddr,U32 *endAddr,U16 *date);
void getUsrName(U8 No,U8 *name);
U8 getPT_ZoomPara(U8 PT_type,S16 *Xscal,S16 *Yscal,S16 *Xoffset,S16 *Yoffset,U32 Patlen);
U8 PT_Zoom(U8 PT_type,S16 Xscal,S16 Yscal,S16 xorgp,S16 yorgp,S16 xmove,S16 ymove,U32 Patlen);
void get_ScaleCenter(U8 PT_type,U8 mode,S16 *xCenter,S16 *yCenter,U32 Patlen);
U16 get_MaxPitch(U8 PT_type,U16 *reg,U32 Patlen);
void CalGroSaveNo(U8 *NO,U8 Type);
void getFeedPos(U16 inFeed,U16 inType,S16 *outx,S16 *outy);
void viewFeedCode(U16 inX,U16 inY,U16 inFeed,U16 inType,U8 f,U8 text);

U16 get_ExitPatNO(U16 *reg,U8 flag);
U16 get_EmptyPatNo(U16 *reg,U16 sno);
U8 myStrCompare(char *str1,char *str2,U8 ignoreCase);

void newDrawPat(U32 vn, S32 scale,S16 ox1,S16 oy1,U16 tx,U16 ty);


#endif
