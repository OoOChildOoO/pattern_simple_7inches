//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Bor_program_fun.h						*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision5 V5.21					*
//*  编程													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#ifndef _EMB_PROGRAM_FUN_H
#define _EMB_PROGRAM_FUN_H

#ifndef SYSTEM1
#ifndef SYSTEM4
#ifndef SYSTEM5

#include "stm32cpu.h"

#define		LOG			0		//逻辑
#define		NUM			1		//数字无单位
#define		NUMU		2		//数字有单位
#define		SIGSEL		3		//单选
#define		CHECK		4		//复选
#define		BACK		5		//返回


//单选按键组号
#define		STOPNEED	1	//针停止
#define		LOWSPEED	2	//低速
#define		SUSPEND		3	//暂停

//单选组最大值
#define		STNEED_MAX		2	//针停止
#define		LOWSPEED_MAX	5	//低速
#define		CHOUTPUT_MAX	3	//输出选项
#define		SUSPEND_MAX		5	//暂停


#define		ADDM		0	//追加方法
#define 	MCURRM		1	//移动光标方法
#define		LTYPE		2	//线的种类
#define		SPITCH		3	//直线缝针距
#define		WPITCH		4	//曲折缝针距
#define		WLWIDTH		5	//曲折缝左宽度
#define		WRWIDTH		6	//曲折缝右宽度
#define		WNEEDLES	7	//曲折缝针数
#define		SMODE		8	//直线缝形成模式
#define		SFMODE		9	//加固缝开始模式	
#define		SFNEEDLE	10	//加固缝开始针数	
#define		EFMODE		11	//加固缝结束模式	
#define		EFNEEDLE	12	//加固缝结束针数
#define		CONNECT		13	//轮廓连接
#define		ROTADIR		14	//旋转方向
#define		MAXPITCH	15	//最大针距
#define		INSBREAK	16	//断缝号自动插入
#define		ROTAANG		17	//旋转角度
#define		ROTACEN		18	//旋转中心
#define		SYMM		19	//对称模式
#define		COMDIR		20	//补偿方向
#define		COMWIDTH	21	//补偿宽度
#define		COMCOPYS	22	//补偿复制数
#define		COMM		23	//补偿复制作方法
#define		XRATIO		24	//x倍率
#define		YRATIO		25	//y倍率
#define		ZOOMCEN		26	//扩大缩放中心
#define		PINCM		27	//落针生成模式
#define		LTYPE1		28	//线的种类
#define		CORDER		29	//复制顺序变更
#define		CCONT		30	//选择轮廓构成点
#define		ATTRIBUTE	31	//属性
#define		PINSMV		32	//针数移动量
#define		DISCODE		33	//显示/隐藏编号
#define		FREPATES	34	//加固反复数
#define		FNEEDLES	35	//加固缝针数
#define		ADDNEEDM	36	//针迹数据追加方法

typedef struct _SETPARA{
	U8 no;
	U16 val;
	U16 min;
	U16 max;
	U16 img;
	U8 type;	// 0-逻辑 1-数字无单位 2-数字有单位
}SETPARA;

//typedef struct
//{
//	U32		ox;				// 
//	U32 	oy;				// 
//	U8		IsDat;
//	U16		ppc[7][7];
//} OLDPAT2;

extern const SETPARA ADDM_D;		//追加方法	
extern const SETPARA MCURRM_D;	//移动光标方法 
extern const SETPARA LTYPE_D;  	//线的种类
extern const SETPARA SPITCH_D;  //直线缝针距
extern const SETPARA WPITCH_D;	//曲折缝针距
extern const SETPARA WLWIDTH_D;	//曲折缝左宽度
extern const SETPARA WRWIDTH_D;	//曲折缝右宽度
extern const SETPARA WNEEDLES_D;//曲折缝针数
extern const SETPARA SMODE_D;	//直线缝形成模式
extern const SETPARA SFMODE_D;	//加固缝开始模式
extern const SETPARA SFMODE1_D;	//加固缝开始模式,有重叠缝
extern const SETPARA SFNEEDLE_D; //加固缝开始针数	
extern const SETPARA EFMODE_D;	//加固缝结束模式
extern const SETPARA EFMODE1_D;	//加固缝结束模式,有重叠缝
extern const SETPARA EFNEEDLE_D; //加固缝结束针数	
extern const SETPARA CONNECT_D;	//轮廓连接
extern const SETPARA ROTADIR_D;	//旋转方向
extern const SETPARA MAXPITCH_D;//最大针距
extern const SETPARA INSBREAK_D;	//断缝号自动插入
extern const SETPARA ROTAANG_D;	//旋转角度
extern const SETPARA ROTACEN_D;		//旋转中心
extern const SETPARA SYMM_D;	//对称模式	
extern const SETPARA COMDIR_D;	//补偿方向
extern const SETPARA COMWIDTH_D;//补偿宽度
extern const SETPARA COMCOPYS_D;	//补偿复制数
extern const SETPARA COMM_D;	//补偿复制作方法
extern const SETPARA XRATIO_D;	//x倍率
extern const SETPARA YRATIO_D;	//y倍率
extern const SETPARA ZOOMCEN_D;	//扩大缩放中心
extern const SETPARA PINCM_D;	//落针生成模式
extern const SETPARA LTYPE1_D;  //线的种类
extern const SETPARA CORDER_D;	//缝制顺序变更方法
extern const SETPARA CCONT_D;	//选择轮廓构成点
extern const SETPARA ATTRIBUTE_D;//属性
extern const SETPARA PINSMV_D;	//针数移动量
extern const SETPARA DISCODE_D;	//显示/隐藏编号
extern const SETPARA FREPATES_D;	//加固反复数
extern const SETPARA FNEEDLES_D;	//加固缝针数
extern const SETPARA ADDNEEDM_D;	//针迹数据追加方法

extern volatile U16 g_ctmode;		//设计模式
extern volatile U16 g_addMethod;	//追加方法
extern volatile U16 g_moveCurrMethod;	//移动光标方法
extern volatile U16 g_lineType;	//线的种类
extern volatile U16 g_strPitch;	//直线缝针距
extern volatile U16 g_winPitch;	//曲折缝针距
extern volatile U16 g_winLwidth;	//曲折缝左宽度
extern volatile U16 g_winRwidth;	//曲折缝右宽度
extern volatile U16 g_winNeedles;	//曲折缝针数
extern volatile U16 g_strMode;		//直线缝形成模式
extern volatile U16 g_stFirmMode;	//加固缝开始模式	
extern volatile U16 g_stFirmNeedles;	//加固缝开始针数	
extern volatile U16 g_endFirmMode;	//加固缝结束模式	
extern volatile U16 g_endFirmNeedles;	//加固缝结束针数
extern volatile U16 g_conConnect;		//轮廓连接
extern volatile U16 g_rotateDir;		//旋转方向
extern volatile U16 g_maxPitch;		//最大针距
extern volatile U16 g_inserBreak;		//断缝号自动插入
extern volatile U16 g_rotateAngle;		//旋转角度
extern volatile U16 g_rotateCenter;	//旋转中心
extern volatile U16 g_symMode;			//对称模式
extern volatile U16 g_compDir;			//补偿方向
extern volatile U16 g_compWidth;		//补偿宽度
extern volatile U16 g_compCopys;		//补偿复制数
extern volatile U16 g_compModes;		//补偿复制作方法
extern volatile U16 g_xRatio;			//x倍率
extern volatile U16 g_yRatio;			//y倍率
extern volatile U16 g_zoomCenter;		//扩大缩放中心
extern volatile U16 g_pinCreMode;		//落针生成模式
extern volatile U16 g_lineType1;		//线的种类1
extern volatile U16 g_codeOrder;		//缝制顺序变更方法
extern volatile U16 g_chooseCont;		//选择轮廓构成点
extern volatile U16 g_pinMoves;		//针数移动量
extern volatile U16 g_attributes;		//属性
extern volatile U16 g_displayCode;		//显示/隐藏编号
extern volatile U16 g_firmRepeats;		//加固反复数
extern volatile U16 g_firmNeedlets;		//加固缝针数
extern volatile U16 g_addNeedMethod;	//针迹数据追加方法
extern volatile U16 g_enableCode;		//使能代码标志 一位一个代码
extern SETPARA g_curSetPara[12];
extern volatile S16 g_xorg;			//启缝点x位置
extern volatile S16 g_yorg;			//启缝点y位置
extern volatile U8 g_hasStPoint;			//已有启缝点标志
extern volatile U16 g_endCode;			//结束代码标志
extern volatile U16 g_genLine;			//普通直线
extern volatile U16 g_isColse;					//封闭图形,重叠缝判断标记
extern volatile U16 g_tattributes;		//缝纫点临时属性

#define		TOUPPER		0		//返回上一级
#define		STONNEED	1		//针上停止
#define		STUNNEED	2		//针下停止
#define		PFVALID		3		//压脚有效
#define		PFHEIGHT	4		//压脚高度
#define		LFVALID		5		//张力有效
#define		LFNO		6		//张力编号
#define		SPVALID		7		//缝纫速度有效
#define		SPEEDVAL	8		//缝纫速度
#define		LOWSPEED0	9		//解除低速
#define		LOWSPEED1	10		//1200以下
#define		LOWSPEED2	11		//800以下
#define		LOWSPEED3	12		//600以下
#define		LOWSPEED4	13		//400以下
#define		CHOUTPUT1	14		//选项输出1
#define		CHOUTPUT2	15		//选项输出2
#define		CHOUTPUT3	16		//选项输出3
#define		SUSPEND1	17		//暂停1
#define		SUSPEND2	18		//暂停2
#define		SUSPEND3	19		//暂停3
#define		SUSPEND4	20		//暂停4
#define		SUSPEND5	21		//暂停5
#define		SUSPEND1A	22		//暂停1-关
#define		SUSPEND1B	23		//暂停1-开
#define		SUSPEND1C	24		//暂停1-编号
#define		SUSPEND2A	25		//暂停2-关
#define		SUSPEND2B	26		//暂停2-开
#define		SUSPEND2C	27		//暂停2-编号
#define		SUSPEND3A	28		//暂停3-关
#define		SUSPEND3B	29		//暂停3-开
#define		SUSPEND3C	30		//暂停3-编号
#define		SUSPEND4A	31		//暂停4-关
#define		SUSPEND4B	32		//暂停4-开
#define		SUSPEND4C	33		//暂停4-编号
#define		SUSPEND5A	34		//暂停5-关
#define		SUSPEND5B	35		//暂停5-开
#define		SUSPEND5C	36		//暂停5-编号

extern const SETPARA TOUPPER_D;		//返回上一级
extern const SETPARA STONNEED_D;	//针上停止
extern const SETPARA STUNNEED_D;	//针下停止
extern const SETPARA PFVALID_D;		//压脚有效
extern SETPARA PFHEIGHT_D;	//压脚高度
extern const SETPARA LFVALID_D;		//张力有效
extern const SETPARA LFNO_D;		//张力编号
extern const SETPARA SPVALID_D;		//缝纫速度有效
extern const SETPARA SPEEDVAL_D;	//缝纫速度
extern const SETPARA LOWSPEED0_D;	//解除低速
extern const SETPARA LOWSPEED1_D;	//1200以下
extern const SETPARA LOWSPEED2_D;	//800以下
extern const SETPARA LOWSPEED3_D;	//600以下
extern const SETPARA LOWSPEED4_D;	//400以下
extern const SETPARA CHOUTPUT1_D;	//选项输出1
extern const SETPARA CHOUTPUT2_D;	//选项输出2
extern const SETPARA CHOUTPUT3_D;	//选项输出3
extern const SETPARA SUSPEND1_D;	//暂停1
extern const SETPARA SUSPEND2_D;	//暂停2
extern const SETPARA SUSPEND3_D;	//暂停3
extern const SETPARA SUSPEND4_D;	//暂停4
extern const SETPARA SUSPEND5_D;	//暂停5 
extern const SETPARA SUSPEND1A_D;	//暂停1-开
extern const SETPARA SUSPEND1B_D;   //暂停1-关
extern const SETPARA SUSPEND1C_D;   //暂停1-编号
extern const SETPARA SUSPEND2A_D;   //暂停2-开
extern const SETPARA SUSPEND2B_D;   //暂停2-关
extern const SETPARA SUSPEND2C_D;   //暂停2-编号
extern const SETPARA SUSPEND3A_D;   //暂停3-开
extern const SETPARA SUSPEND3B_D;   //暂停3-关
extern const SETPARA SUSPEND3C_D;   //暂停3-编号
extern const SETPARA SUSPEND4A_D;   //暂停4-开
extern const SETPARA SUSPEND4B_D;   //暂停4-关
extern const SETPARA SUSPEND4C_D;   //暂停4-编号
extern const SETPARA SUSPEND5A_D;   //暂停1-开
extern const SETPARA SUSPEND5B_D;   //暂停1-关
extern const SETPARA SUSPEND5C_D;   //暂停1-编号


extern volatile U16 g_needlsStop;		//针上/针下停止
extern volatile U16 g_pfValid;			//压脚有效
extern volatile U16 g_pfHeight;			//压脚高度
extern volatile U16 g_lfValid;			//张力有效
extern volatile U16 g_lfNO;				//张力编号
extern volatile U16 g_speedValid;		//缝纫速度有效.
extern volatile U16 g_speeds;			//缝纫速度
extern volatile U16 g_lowSpeed;			//低速
extern volatile U16 g_chOutput1;		//选项输出1
extern volatile U16 g_chOutput2;		//选项输出2
extern volatile U16 g_chOutput3;		//选项输出3
extern volatile U16 g_suspend1;		//暂停1
extern volatile U16 g_suspend2;		//暂停2
extern volatile U16 g_suspend3;		//暂停3
extern volatile U16 g_suspend4;		//暂停4
extern volatile U16 g_suspend5;		//暂停5
extern volatile U16 g_suspend1S;		//暂停1状态
extern volatile U16 g_suspend1N;		//暂停1编号
extern volatile U16 g_suspend2S;		//暂停2状态
extern volatile U16 g_suspend2N;		//暂停2编号
extern volatile U16 g_suspend3S;		//暂停3状态
extern volatile U16 g_suspend3N;		//暂停3编号
extern volatile U16 g_suspend4S;		//暂停4状态
extern volatile U16 g_suspend4N;		//暂停4编号
extern volatile U16 g_suspend5S;		//暂停5状态
extern volatile U16 g_suspend5N;		//暂停5编号

extern SETPARA g_setcode[10];
extern volatile S16 g_areaX,g_areaY,g_areaDx,g_areaDy;	
extern volatile S16 g_rotCenX,g_rotCenY;	


void setValue(SETPARA *des,SETPARA src,U16 val,U8 flag);
void initSetPara(void);
U16 getTextAddr(U8 makeType);
U16 getSetTextAddr(SETPARA val);
char *getValUnit(SETPARA val,U8 flag);
U8 getValPoints(SETPARA val,U8 flag);
U16 getSetTextAddr1(SETPARA val);
U16 getSetTextAddr2(U16 val);
U16 getSetTextAddr3(U16 val);
U8 getDefCmds(U8 makeType,U8 doType);
U8 isCanModified(SETPARA data,U8 flag);
U16 getStep(SETPARA data,U8 flag,U16 maxstep,U8 ctime);
U8 alterSetValue(SETPARA data,U8 makeType,U8 doType,U8 index,U8 flag,U8 ctime,U8 *totalcmd);
U8 alterSetValue1(SETPARA data,U8 makeType,U8 doType,U8 index,U8 sdat,U8 *totalcmd);
U8 getDefCodes(U8 scode,U8 flag);
U16 getCodeTextAddr1(SETPARA data);
U8 isbtnChooese(SETPARA data);
U8 alterCodeValue(U8 *indat,U8 machineType,SETPARA data,U8 index,U8 flag,U8 ctime,U16 *pinNo);
U8 getSingleVal(SETPARA data);
U8 getSingleGroup(SETPARA data);
U8 getSinGroMac(U8 gno);
void setPointColor(U16 color);
void clrSelectArea(void);
void setSetectArea(U16 setno,U8 flag);
U8 isSelestArea(U16 setno);
void clrSelectCtrl(void);
void setSetectCtrl(U16 setno,U8 flag);
U8 isSelestCtrl(U16 setno);
U8 getConnetModes(U8 *indat,U16 areaNo);
void getXYpos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,U8 flag);
void getXYDrawPos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,U16 areaNo);
void getXYCtlPos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,U16 ctlNo);
void getXYPinPos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,S16 *dx,S16 *dy,U8 *code,U16 *code1,U16 pinNo);
void getSetectAreaXY(U8 *indat,S16 *xt,S16 *yt,S16 *dx,S16 *dy);
void embSetBackupBlock(U16 block);
void embGetBackupBlock(U16 block);
U16 getMoveStep(U16 ll);
U8 pointIsOver(S32 xp,S32 yp,S32 xmin,S32 xmax,S32 ymin, S32 ymax);
U8 getVkeyType(U8 inno);
S32 getViewScale(U16 id,U8 flag);
void setViewScale(U16 multiple);
U8 canChangePara(U8 makeType,U8 doType,U8 changeNo);
U8 emb_areaIsClose(void);
U16 getPinNo(U8 *reg,U16 indat,U8 flag,U16 opinno);
U16 getArea(U8 *reg,U16 indat,U8 flag);
U16 getCtrl(U8 *reg,U16 indat,U8 flag);
#endif
#endif
#endif

#endif

