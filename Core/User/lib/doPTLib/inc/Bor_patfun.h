//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名: Bor_patfun.h 								*
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

#ifndef _BOR_PATFUN_H
#define _BOR_PATFUN_H
#include "stm32cpu.h"
#include "main.h"
#define AutoFeedMax   200	//倒针时添加的最多空送数

#define ctBackInLen		12	//打板备份信息大小
#define ctBackSaddH		0	//打板备份数据起始段地址高16位
#define ctBackSaddL		1	//打板备份数据起始段地址低16位
#define ctBackEaddH		2	//打板备份数据结束段地址高16位
#define ctBackEaddL		3	//打板备份数据结束段地址低16位
#define ctBackAbsX		4	//打板备份X坐标绝对位置
#define ctBackAbsY		5	//打板备份Y坐标绝对位置
#define ctBackSpeed		6	//打板备份速度值
#define ctBackAbsPF		7	//打板备份花样中压脚绝对值
#define ctBackDisPF		8	//打板备份花样中压脚相对值
#define ctBackAbsLF		9	//打板备份张力绝对值
#define ctBackOrgX		10	//打板备份第二原点X坐标
#define ctBackOrgY		11	//打板备份第二原点Y坐标

typedef struct _BorFeed
{
	U16 NO;		//记录打板倒针自动添加空送的编号
	U16	addr;	//记录打板倒针自动添加空送的寄存器地址
}BorFeed;

typedef struct _BorFeedINFO
{
	U8 MaxFeed;
	BorFeed Feed[AutoFeedMax];	//最大支持200个自动添加空送

}BorFeedINFO;


extern volatile BorFeedINFO g_BorAutoFeed;

U16 get_checkSum(U16* reg,U32 len,U8 intype);
U8 Bor_FileAnaly(U16 *Reg,U32 len);
//U8 Bor_AnaInfo(U16 *Reg,U32 len,U32 *patlen,U32 *lenaddr,U32 *pataddr,U32 *limaddr,U32 *orgaddr,U32 *nameaddr);
U8 Bor_AnaInfo(U16 *Reg,U32 len,ISEWADD *info);
U8 sew_Patcheck(U16 *Reg,U32 len);
U8 Bor_BaseAddr(U16 *Reg,U32 len,U32 *PfBase,U32 *LfBase);
void Bor_CtBefDatAny(U32 *endaddr,U32 *Patlen,U8 *lspeed,U8 Type);
//U8 Bor_DrawCmd(U16 *indata ,U16 *cmd , S16 *xch ,S16 *ych ,U8 *offset);
U8 Bor_DrawCmd(U16 *indata ,ISEWCMD *info);
void Bor_CreatBack(U16 *backTimes,U32 stadd,U32 enaddr,S16 abs_xpos,S16 abs_ypos,U8 speed,S16 abs_pf,S8 dis_pf,U8 abs_lf,S16 orgx,S16 orgy);
U16 sewFindPinAndPos(U32 ptlen,U16 minf,U16 maxf,S16 *px,S16 *py);
void sewP2Pmove(U32 ptlen,U16 sf,U16 ef,U32 ptno);
void Bor_CalPFval(U16 *Reg,U32 TPatlen,U32 endaddr,S16 *pfabs);
void Bor_CalLFval(U16 *Reg,U32 TPatlen,U16 feedc,U8 *lfabs,S8 *lfds);
void Bor_doEndcmd(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen,U16 *feedsum);
U8 Bor_dofeedOrg(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen);
U8 Bor_IsneedTrim(U16 *Reg,U32 patstaddr,U32 endaddr,U16 feed,U8 type);
U8 Bor_doCode(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen,U8 CreatType,U8 AddFuns);
U8 Bor_doExCode(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen,U8 mode,U16 dat);
void Bor_ModPF(U32 *Point,S8 pfVal,U32 Patlen);
U8 Bor_CreatCalLF(U32 addr,U32 TPatlen);
U8 Bor_ModLF(U16 StPin,U8 linF,U32 Patlen);
void Bor_feedInfo(U16 feedc,U32 Patlen,U16 *cmd,S16 *xpos,S16 *ypos,S16 *dx,S16 *dy,U8* speed,S8 *pfoodval, S8 *pfdisval);
U8 Bor_ModOrg(S16 dx,S16 dy,S16 px,S16 py,U8 mode,U32 Patlen);
U8 Bor_ModDelPins(U16 StPin,U16 nums,U8 DelMode,U32 Patlen);
U8 Bor_ModAddPin(U16 *StPin,S16 dxpos,S16 dypos,U8 AddMode,U32 Patlen);
U8 Bor_ModMoPin(U16 *StPin,S16 dxpos,S16 dypos,U8 ModMode,U32 Patlen);
U8 Bor_ModMoveLine(U16 StPin,U16 EnPin,S16 dxpos,S16 dypos,U8 Pindis,U8 MovMode,U32 Patlen);
void Bor_ModMoPinSwitch(U16 StPin,U16 Pinnums,U8 ModType,U8 Pindis,U16 limang,U32 Patlen);
void Bor_ModPindis(U16* Reg,U16 StPin,U16 Pinnums,U16 *newpins,U8 ModType,U8 Pindis,U16 limang,U32 *Patlen);
void Bor_ModSpeed(U16 StPin,U16 Modpins,U8 Speed,U8 ModType,U32 Patlen);
U8 Bor_ModCode(U16 StPin,U8 ModType,U8 AddCode,U32 Patlen,U8 delflag);
U8 Bor_ModMoArea(U16 StPin,U16 EnPin,S16 dxpos,S16 dypos,U8 Pindis,U8 ModMode,U32 Patlen);
void Bor_dofeed(U16 *Reg,U32 staddr,U32 endaddr,U32 *endaddr1,S16 stpx,S16 stpy,S16 endpx,S16 endpy,U32 *Patlen,U32 *TPatlen,U8 Addtrim);
void Bor_doline(U16 *Reg,U32 staddr,U32 endaddr,U32 *endaddr1,S16 stpx,S16 stpy,S16 endpx,S16 endpy,U8 Pindis,U32 *Patlen,U32 *TPatlen,U8 extype);
U8 Bor_docir(U16 *Reg,U32 staddr,U32 endaddr,U32 *endaddr1,S16 stpx,S16 stpy,S16 asspx,S16 asspy,S16 endpx,S16 endpy,U8 Pindis,U32 *Patlen,U32 *TPatlen,U8 extype);
U8 Bor_docir1(U16 *Reg,U32 staddr,U32 endaddr,U32 *endaddr1,S16 stpx,S16 stpy,s16 endpx,S16 endpy,U8 Pindis,U8 mode,U32 *Patlen,U32 *TPatlen,U8 extype);
void Bor_docur(U16 *Reg,U32 staddr,U32 endaddr,U32 *endaddr1,S16 stpx,S16 stpy,S16 endpx,S16 endpy,U8 Pindis,U16 ctlnum,U32 *Patlen,U32 *TPatlen,U8 extype);
void Bor_CalPatLim( U16 *Reg,U32 Patlen,S16 *xMax,S16 *xMim,S16 *yMax,S16 *yMim,U16 *pins);
void Bor_CalPatLim1( U16 *Reg,U32 Patlen,S16 *xMax,S16 *xMim,S16 *yMax,S16 *yMim);
void Bor_MoveData(U16 *Reg,U32 Reglen,S16 dlen,U32 movaddr);
S32 Sew_GetSewLen( U16 *Reg,U32 Patlen);
void Bor_Change_E(U32 *len,U8 *Name,U16 *sreg);
//void Bor_embTOsew(U32 *len,U8 *Name);
void Bor_CalPFlimt(U16 *Reg,U32 len,S8 *MAX,S8 *MIN);
U8 Bor_CreatData(U32 *staddr,U32 *endaddr,S16 stpx,S16 stpy,S16 endpx,S16 endpy,U8 Pindis,U32 *TPatlen,U8 CreatType,U8 exFlag);
U8 sew_ctOverlap(U32 *endaddr,U32 *TPatlen);
void Bor_doTack(U16 *Reg,U32 addr1,U32 addr2,U32 *addr3,U32 *Patlen,U8 st_stitch,U8 en_stitch,U8 rep_switch,U32 *TPatlen);
U8 Bor_Updatctl(U16 *ctlnums,U16 exdis,U8 type,U8 flag,COORDINATE *org);
void Bor_Updatct2(U16 pos1,U16 pos2,U16 *cnt);
void Bor_Ctsplinectl(U16* Reg,U32 staddr,U32 endaddr,U16 *ctlnums);
void Bor_ctNewPat(U32 *NewPatLen,U16 *newpins,U8 Pindis,U16 pinnums);
U8 Bor_ctlIsocclu(U16 ctlmus);
U8 Bor_doSpeed(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen,U8 SpeedType,U16 *feedsum);
U8 Bor_doPF(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen,S8 pfabsval,U16 *feedsum);
U8 Bor_doLF(U16 *Reg,U32 *staddr,U32 *endaddr,U32 *TPatlen,U8 lfabsval,U16 *feedsum);
void Bor_CreatUnDO(U32 Staddr,U32 Endaddr,U32 *TPatlen);
U8 Bor_CreatDelData(U16 *backTimes,U32 *stadd,U32 *enaddr,S16 *abs_xpos,S16 *abs_ypos,U8 *speed,S16 *abs_pf,S8 *dis_pf,U8 *abs_lf,S16 *orgx,S16 *orgy,U32 *TPatlen);
U8 Bor_ConZoom(U8 Modmode,U16 Xscal,U16 Yscal,S16 xorgp,S16 yorgp,U8 Pindis,U16 ConAng,U32 Patlen);
U8 Bor_ConMirr(U8 Convmode,U8 ConvType,U32 Patlen);
U8 Bor_ConCopy(U8 Convmode,U32 Patlen);
U8 Bor_ConShrin(U8 Convmode,U32 Patlen);
U8 Bor_ConSpin(U8 Convemode,U16 ang,S16 xorgp,S16 yorgp,U32 Patlen);
void Bor_ConCalAssArea(U32 Patlen,U8 flag);
U16 Bor_getStPin(U32 Patlen,U16 point);
U8 Bor_ConveAss(U8 Convemode,U8 areaNO,S16 Convedis,U8 Pindis,U16 SetAng,U32 Patlen,U32 *TPatlen);
U8 Bor_ConveRepe(U8 ConveType,U8 Convemode,U8 areaNO,S16 Convedis,U8 Repetims,U8 Pindis,U16 SetAng,U32 Patlen);
U8 Bor_ConveStartEnd(U16 StPin,U32 Patlen);
U8 Bor_Isocclu(U32 cur_point,U32 *Stpoint,U32 *Enpoint,U32 Patlen);
void Bor_ModTack(U16 StPin,U8 st_stitch,U8 en_stitch,U8 rep_switch,U32 Patlen);
void Bor_2deal(void);
void Juki_TO_Bor(U32 *len,U8 *Reg);
void Bor_TO_Juki(U32 *len);
void bor_getPointXY(U32 point,U32 len,S16 *xpos,S16 *ypos);
U8 Bor_ModPinAng(U16 StPin,U32 Patlen,U16 dat);
U32 Bor_copyImgs(U16 *inbuf,U16 *outbuf,U32 inlen);
U32 getSewPoint_sum(U32 dis,U16 temp_pitch,U8 extention);
void sewVerPosZag(U8 dir,U32 dis,S32 xx,S32 yy,U8 f,U8 mode,double ang); 
U8 EmbToSew(void);
//U16 emb_getDrawCmd(U16 *reg,U32 addr,U32 *len,U16 *pinsLen,U16 *cmd,U16 *cmd1,U8 *ctls);
U8 emb_getDcmd(U8 *indat,U32 off,EMBDINFO *outinfo);
void emb_getLimit(U8 *indat,U8 macType,S16 *xmin,S16 *xmax,S16 *ymin,S16 *ymax);
void emb_getOrgs(U8 *indat,U8 macType,S16 *xorg,S16 *yorg);
U8 emb_getMachineTypeBySreg(U16 *reg);
U8 emb_getMacTypeByPT(U8 *indat,U32 off);
U8 emb_getEndCodeBySreg(U16 *reg);
U8 emb_getPcmd(U8 *indat,U32 off,EMBPINFO *outinfo);
//U8 emb_getPinCmd(U16 *reg,U32 addr,U16 *cmd,U16 *pins);
//U8 emb_getPinCmd1(U16 *reg,U32 addr,U16 *cmd,U16 *pins);
U8 emb_getPXY(U8 *indat,U32 off,U8 macType,S16 *outx,S16 *outy,U16 pin);
U8 emb_getPXY1(U8 *indat,U32 off,U8 macType,S16 *outx,S16 *outy,U16 pin);
U8 emb_getPXY2(U8 *indat,U32 off,U8 macType,S16 *outx,S16 *outy,U16 pin);
U8 emb_getPinXY(U16 *reg,U8 macType,U32 addr,S16 *x,S16 *y,U16 pin);
U8 emb_FileAnaly(U8 *indat,U32 flen);
void emb_getComment(U8 *indat,U32 off,U8 *notes);
void emb_Analy(U8 *indat,U16 *tDraws,U16 *ctrls,U16 *pins);
void emb_getIndex(U8* indat,U32* drawAddr,U32 *drawLen,U32 *pinAddr,U32 *pinlen);
U8 emb_getAreaAddr(U8 *indat,U16 ano,U32 *daddr,U32 *dlen,U32 *pdaar,U32 *plen);
U32 sew_getPinAddr(U16 *reg,U32 Patlen,U16 infeed,U8 flag);
U16 sew_UpFeed(U16 *reg,S16 dx,S16 dy,U32 *addr,U32 *ptSize);
U16 sew_UpFeed1(U16 *reg,S16 dx,S16 dy,U32 *addr,U32 *ptSize);
U32 sew_getEndCode(U16 *reg,U32 Patlen);
void Sew_ModPindis(U16* Reg,U32 StAddr,U32 EndAddr,U8 Pindis,U32 *PTlen);
U32 sew_PtAdd(U16* srcPT,U32 srclen,U16* addPT,U32 addlen);
U32 sew_DealSaveData(U32 Patlen ,U8 *desc,U16 saveno);
//#ifdef SYSTEM2
#ifndef SYSTEM1
#ifndef SYSTEM4
#ifndef SYSTEM5
Embcheck setEmbMatchCheck(U16 *reg);
Embcheck getEmbMatchCheck(U16 *reg);
Embcheck setSewMatchCheck(U16 *reg,U32 plen,Embcheck check);
Embcheck getSewMatchCheck(U16 *reg,U32 plen);
void updataPinsInfo(U16 *pins);
void emb_getDrawXY(U8 macType,U8 *p,S16 *x,S16 *y);
U8 emb_getPinXY1(U16 *reg,U8 macType,U32 addr,S16 *x,S16 *y,U16 pin);
U8 emb_AreaCanEdit(U8 *indat,U16 inArea,U8 mode);
U8 emb_CrlCanEdit(U8 *indat,U16 ctlno,U8 mode);
U8 emb_PinCanEdit(U8 *inda,U16 inArea,U8 mode);
U8 emb_isNormalArea(U8* indat,U16 no,U8 flag);
void emb_setIndex(U8* indat,U32 drawLen,U32 pinAddr,U32 pinlen);
void emb_setComment(U8 *indat,U16 off,char *text);
void emb_upDrawAddrs(U8 *indat);
U8 getSelectClts(U8 maxCtl,U8 ltype,U16 nobase);
U16 getSelectPins(U32 paddr,U16 nobase);
U16 getAreaPins(U16 area);
U16 getAreaSelectPins(U16 area,U16 off);
void movePinsBuf(U16 addr,S16 len,U16 max);
EMBNEWPINS emb_doNewPins(U8* p8,U16 area);
void embCreatDisData(U8 mode);
void BorH_newEmbData(void);
U8 BorH_CreatEmbData(U8 mode);
void emb_updataCtl(U8 type,U8 mode,U8 time);
U16 emb_SplUpdataCtl(U8 *indat,U8 macType,U16 plen,U8 time,U8 inflag);
U16 emb_SplUpdataCtl1(U16 nums);
U16 emb_SplUpdataCtl2(U8* indat,U8 macType,U16 pinsLen,U16 base,U8 upflag);
U16 emb_SplUpdataCtl3(U8* indat,U8 macType,U16 pinsLen,U16 base,U8 upflag);
U8 emb_getCtType(U8 machineType,U16 inPointNum,U32 *sdrawAddr,U32 *spinAddr,S16 *px,S16 *py);
U16 BorH_embDoDraw(U8 *indat,U32 *inaddr,U32 taddr,U8 mode);
U16 BorH_embDoDraw1(U16 *reg,U32 *inaddr,U32 taddr,U8 mode);
void BorH_embDoPins(U8 *indat,U16 *pins,U32 *lens,U16 pitchs,U8 mode,U8 pinmode);
void embDoPins(U8 *indat,U16 *pins,U32 *lens,U16 pmax);
void emb_doFeeds(U8 *indat,U16 *pins,U32 *lens);
U16 emb_doReinforce(U8 *indat,U32 *staddr,U32 *endaddr,U32 len);
void emb_autoCtPoint(U8 mode);
U16 emb_doline(U8 *indat,U32 staddr,U32 *endaddr1,S16 stpx,S16 stpy,S16 endpx,S16 endpy,U16 Pindis,U8 mode,U8 iflag);
U16 emb_docur(U8 *indat,U32 staddr,U32 *endaddr1,S16 stpx,S16 stpy,S16 endpx,S16 endpy,U16 Pindis,U8 mode);
U16 emb_docir(U8 *indat,U32 staddr,U32 *endaddr1,U16 Pindis,U8 mode,U8 pinmode);
U16 emb_docir1(U8 *indat,U32 staddr,U32 *endaddr1,U16 Pindis,U8 mode,U8 pinmode);
U16 emb_doEllipse(U8 *indat,U32 staddr,U32 *endaddr1,U16 Pindis,U8 pinmode);
U16 emb_doPins(U8 *indat,U32 staddr,U32 *endaddr1);
void emb_segzig(U8 *indat,U32 *addr,U16 *pins,S16 xpos,S16 ypos,S16 dx,S16 dy);
U8 emb_CopyAreas(U8 machineType,S16 dx,S16 dy);
U8 emb_RotatAreas(U8 machineType,U16 rotAng);
U8 emb_symMoveAreas(U8 machineType);
U8 emb_symCopyAreas(U8 machineType);
U8 emb_compMovAreas(U8 machineType);
U8 emb_compCopyAreas(U8 machineType);
void emb_getDrawCtls(U8 *p,U8 cnt);
void emb_getPinParas(U8 *indat,U32 addr);
void emb_getRoateCenPos(void);
void emb_getSymCenPos(void);
void emb_toSymPosXY(S16 *xpos,S16 *ypos);
void emb_movePos(U8 *p,S16 dx,S16 dy);
void emb_scalePos(U8 *p,float scale);
void emb_roatPos(U8 *p,double rdat1,double rdat2);
void emb_smyMovePos(U8 *p);
void emb_setDrawCtls(U8 *p,U8 cnt);
void emb_setDrawCtls1(U8 *p,S16 px,S16 py);
U8 emb_addCtrl(void);
U8 emb_splitCtrl(void);
U8 emb_chShapeCtrl(void);
U8 emb_chAbtCtrl(void);
U8 splAutoCreatCrl(U32 pinaddr,U32 pinlen,S16 px1,S16 py1,S16 px2,S16 py2,S16 *outx,S16 *outy);
U8 emb_addPin(U16 mode,S16 movdx,S16 movdy);
U8 emb_splitPin(void);
U8 emb_reinforcePin(void);
U8 emb_areaIsEdit(U8 flag);
U8 emb_chConnectAreas(void);
U8 emb_connectAreas(void);
void emb_setConnect(U8 *indat,U32 daddr,U16 mode);
U8 emb_chOrderAreas(void);
U8 emb_chSToEndAreas(void);
void emb_setSfirm(U8 *indat,U32 daddr,U16 mode,U16 needles);
void emb_setEfirm(U8 *indat,U32 daddr,U16 mode,U16 needles);
U8 emb_changeLineAreas(void);
void emb_setLtype(U8 *indat,U32 daddr,U16 type);
void emb_setSpitch(U8 *indat,U32 daddr,U16 pitch);
void emb_setSmode(U8 *indat,U32 daddr,U16 mode);
void emb_setWpitch(U8 *indat,U32 daddr,U16 pitch);
void emb_setWlwidth(U8 *indat,U32 daddr,U16 width);
void emb_setWrwidth(U8 *indat,U32 daddr,U16 width);
void emb_setWneedles(U8 *indat,U32 daddr,U16 needles);
U8 getAreaCtls(EMBDINFO dinfo);
U8 getAreaSelectClts(EMBDINFO dinfo,U16 nobase);
U8 emb_deleteAreas(void);
U8 emb_deleteCtrl(void);
U8 emb_moveCtrl(S16 movdx,S16 movdy);
U8 emb_deletePin(void);
U8 emb_firmAreas(void);
U32 emb_getDrawSize(void);
U32 emb_getPinSize(void);
U8 emb_moveAreas(U8 machineType,S16 dx,S16 dy);
U8 emb_movePin(S16 movdx,S16 movdy);
U8 emb_chAbtPin(void);
void emb_getZoomCenPos(void);
U8 emb_zoomAreas(U8 machineType);
void emb_zoomCtls(S16 cdx,S16 cdy);
U8 SewToEmb(U32 Patlen);
U32 emb_getSize(void);
void emb_datScale(U8 *intat,float scale);
#endif
#endif
#endif

#endif
