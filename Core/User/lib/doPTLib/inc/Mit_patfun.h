//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名: Mit_patfun.h 								*
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

#ifndef _MIT_PATFUN_H
#define _MIT_PATFUN_H
#include "stm32cpu.h"
#include "main.h"

typedef struct
{
	U32 drawp;	// 绘图数据地址
	U8 times;	// 第几重
	U16 pins;	// 连接针数
	U16 nexpins;	// 下一重针数
} SPDRAW;

extern uint32_t G_Changto_E(U16 *sreg);

S8 Mit_CailPadOffestPF(U32 jogpoint,S8 mode,U8 UpdataLim);
S8 Mit_CailPadOffestLF(U32 jogpoint,S8 mode);
void Mit_DealSaveData(U8 Type,U32 Patlen);
void Mit_CalPFlimt(U16 *Reg,U32 len,S8 *MAX,S8 *MIN);
void Mit_To_Bor(U32 *len);
U8 Bor_To_Mit(U32 Patlen);
U32 Mit_do_feed1(S16 dx,S16 dy,U32 *draw_addr,U32 *line_addr,U32 *pat_addr,U16 *line_counts);
U8 BorCmdAnaly(U32 *addr,S16 *dx,S16 *dy,S16 *pfval,U8 *excode,U8 *speed,U32 srcTlen);
U16 mitFindPinAndPos(U16 minf,U16 maxf,S16 *px,S16 *py);
void mitP2Pmove(U16 sf,U16 ef,U32 ptno);
U32 Mit_do_feed(S32 xx,S32 yy,U8 mode);
U8 Mit_do_point(S32 xx,S32 yy,U8 mode);
U16 Mit_getLinePins(S16 xx,S16 yy,U8 pitch);
U32 Mit_do_line(S32 xx,S32 yy,U8 mode,U8 flag);
U32 Mit_do_polygon(U8 point_n,U8 mode);
U32 Mit_do_arc(U8 mode);
U32 new_do_arc(U8 mode);//新建
U32 Mit_do_spline(U16 point_n,U8 mode,U8 nn);
void Mit_do_srtbt(U32 addr1,U32 addr2);
void Mit_do_endbt(U16 sum);
void Mit_do_repeat(U32 addr,U16 sum);
U16 Mit_figurepoint_spline(U8 ii,U8 occlude);
U16 Mit_CialMutiSplinePoint(U16 points,U32 addr,U32 number);
void Mit_Addition_XYData(U32 add,S16 *x1,S16 *y1);
U16 Mit_Analyse_Data(U8 f,U32 *n);
U16 Mit_Analyse_Data1(U32 *n,U32 *sum,U16 draw_add0);
U16 Mit_Analyse_SplineData(void);
U16 Mit_CailPolygonLineVal(U16 DrawAdder);
void Mit_AnalyseDataCorrect(void);
U8 Mit_ConvertMultiDraw(void);
void Mit_CailSavePadAdd(void);
U16 Mit_UpdataSregData(U16 tmp_sreg_add,U8 mode);
void Mit_CailSplinePNum(U16 drawaddS,U16 drawaddE);
U16 *Mit_Do_Offset(U32 pat_num,U16 sum,U16 tmp_draw_addstr,U16 tmp_draw_add,U8 offset_times,U16 *P);
U16 *Mit_Set_DrawData(U16 cmd,U16 param,S16 x1,S16 y1,U16 *SREG_P);
U16 *Mit_Set_DrawData1(U16 cmd,U16 param,S16 x1,S16 y1,U16 *SREG_P);
U16 Mit_Disconnect_DrawData(U32 posi,S16 mode,U16 *P,U32 *numb);
U16 Mit_Disconnect_DrawData_N(U32 posi,S16 mode,U16 *P);
U16 *Mit_SetPfootDrawData(U8 type,U32 sum1,U16 *SREG_P);
U32 Mit_cailNextPfootDrawadd(U32 sum);
U8 Mit_CailPfmode(U32 num,U32 sum1,U32 pos,S16 type);
void Mit_Get_XYData(U32 add,S16 *x1,S16 *y1);
S8 Mit_Check_Direction(U16 dat1,U16 dat2);
S8 Mit_CailPfootVal(U32 stpois,U32 endpois);
U8 Mit_Npoint_Dis(U32 strpoint,U32 endpoint,COORDINATE *posi);
void Mit_CailModifyStitchPara(U16 pos);
void Mit_Draw_Centre(S32 *x,S32 *y);
void getspldata(U16 block,U16 lines,U16 *splfeeds,U32 *spladdr);
U16 Mit_SaveSplPatData(void);
U16 Mit_SaveSregData(U16 tmp_sreg_add,U8 mode);
U32 Mit_ReadSregData(U32 list_add);
U16 Mit_figurepoint_spline1(U8 ii,U8 occlude);
void mit_getPointXY(U32 point,U32 patAddr,S16 *xpos,S16 *ypos);
U16 mit_getExCodeData(U32 pAddr,U32 eAddr,U8 mode);

U8 mit_ctOverlap(void);
U16 Mit_CreateAllData(void);
U8 Mit_Modify_Data(U32 para);
U8 Mit_Create_Data(U8 mode);
U8 new_Create_Data(U8 mode);//新建
U8 Mit_Convert_Data(U32 para);
void mit_setRepConPins(void);
U16 mit_getRepConPins(U32 drawp,U8 times);
U16 mit_getRepNexPins(U32 drawp,U8 times);
U32 Mit_CailSplinePad(U16 DrawAdd,U32 PadAdd,U32 padsum,U8 mode);



PINMSG mit_getPinsMsg(U16 *reg,U32 pinaddr,U32 feed);
void mit_CtToMod(void);
void mit_ModToCt(void);
void mit_upInsetPs(S16 dpins,S16 inposx,S16 inposy);
#endif
