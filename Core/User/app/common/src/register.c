//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  register.c								*
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

#include <absacc.h>
#include "register.h"
#include "menu.h"
#include "main.h"
#include "patdata.h"
#include "touch.h"
#include "ds2460.h"

//SDRAM:H57V2562GTR		16M x 16bit		0xc0000000 - 0xc1ffffff		分四个区
//bank0:0xc0000000 - 0xc07fffff		0x400000 x 16bit				显存区域
//bank1:0xc0800000 - 0xc0ffffff		0x400000 x 16bit
//bank2:0xc1000000 - 0xc17fffff		0x400000 x 16bit				各通讯用寄存器
//bank3:0xc1800000 - 0xc1ffffff		0x400000 x 16bit
//显存 SDRAM
//bank2
volatile U16 EEPREG[MAXEEPREG] __attribute__((at(ADDEEPREG))); //0xc1000000
volatile U16 DREG[MAXDREG] __attribute__((at(ADDDREG)));	   // 状态数据							9		c1000000
volatile U16 EREG[MAXEREG] __attribute__((at(ADDEREG)));	   // 错误信息							13		c1000080
volatile U16 MREG[MAXMREG] __attribute__((at(ADDMREG)));	   // 系统运行信息数据			14		c1000280
volatile U16 YREG[MAXYREG] __attribute__((at(ADDYREG)));	   // 系统数据							15		c1000400
volatile U16 PREG[MAXPREG] __attribute__((at(ADDPREG)));	   // 参数数据							16		c1000580
//volatile U16 IREG[MAXIREG] __attribute__((at(ADDIREG)));			// 花样文件索引表					17		c1000d80
volatile U16 HREG[MAXHREG] __attribute__((at(ADDHREG)));		   // 当前花样文件信息					18		c1002680
volatile U16 SREG[MAXSREG] __attribute__((at(ADDSREG)));		   // 花样数据							19		c1002700
volatile U16 IOREG[MAXIOREG] __attribute__((at(ADDIOREG)));		   // IO参数数据						20		c102f608
volatile U16 TMP_SREG[MAXTSREG] __attribute__((at(ADDTSREG)));	   // TMP_SREG							21		c102f8a8
volatile U16 IAPBuff[MAXTIAPBUFF] __attribute__((at(ADDIAPBUFF))); //									22		c10394E8
volatile U16 IREG2[MAXIREG2] __attribute__((at(ADDIREG2)));		   // 花样文件索引表2					23
//volatile U16 DIR_DREG2[MAXDIR2] __attribute__((at(ADDDIR_DREG2)));	// 文件目录2最大个数(操作板) 2560*2
volatile U16 NEWSREG[MAXSREG2] __attribute__((at(ADDNEWSREG))); // 新转换为兄弟款花样数据			19		c1002700
volatile S16 PATCRRRD[MAXCOORD][2] __at(ADDPATCRRRD);			// 暂存新花样绝对坐标缓冲
volatile S16 PATCRRRD1[MAXCOORD][2] __at(ADDPATCRRRD1);			// 暂存新花样绝对坐标缓冲
volatile U16 INPOINTPOS[MAXCOORD] __at(ADDRINFPOINT);			// 拐点位置
volatile U16 UREG[MAXUREG] __at(ADDUREG);						// 用户信息
volatile U16 VREG[MAXVREG] __at(ADDVREG);						// 伺服参数
volatile U16 GREG[MAXGREG] __at(ADDGREG);						// 组合图形数据
volatile U16 LREG[MAXLREG] __at(ADDLREG);						// 电子夹线器数据
//bank1
volatile U8 AVI_VIDEO_BUF[AVI_VIDEO_BUF_SIZE] __attribute__((at(ADDAVIVIDEO))); // AVI视频缓冲区
volatile U8 AVI_AUDIO_BUF[AVI_AUDIO_BUF_SIZE] __attribute__((at(ADDAVIAUDIO))); // AVI音频缓冲区

volatile U16 BLKSREG[MAXBLKSREG] __attribute__((at(ADDBLKSREG))); // 备份花样数据缓冲索引
volatile U16 BLKHREG[MAXBLKHREG] __attribute__((at(ADDBLKHREG))); // 备份花样文件信息
volatile U16 BLKIREG[MAXBLKIREG] __attribute__((at(ADDBLKIREG))); // 备份花样文件索引信息

volatile U16 BLKIND[MAXBLIND] __attribute__((at(ADDBLKINDEX))); // 备份数据缓冲索引
volatile U16 BLKBUF[MAXBLKBUF] __attribute__((at(ADDBLKBUF)));	// 备份数据缓冲

//volatile U16 WAVEREG[MAXWAVE] __attribute__((at(ADDWAVE)));	// 波形查看数据缓存

volatile U16 WREG[MAXWREG]; // 2013.10.25

extern volatile U32 FLASHADD_P;
extern U8 gBatType; //打板类型 1：三菱  2：兄弟
extern volatile U8 g_usersave;

const U16 PARA_DATA[];
const U16 PARA_INSET[];
const U16 PARA_OUTSET[];
//------------------------------------------
// Function   : void Reg_Init(void)
// Description: 参数初始化
//------------------------------------------
void Reg_Init(void)
{
	U32 i;

	for (i = 0; i < MAXDREG; i++)
		DREG[i] = 0;
	for (i = 0; i < MAXMREG; i++)
		MREG[i] = 0;
	for (i = 0; i < MAXSREG; i++)
		SREG[i] = 0;

	//	padd = (U16 *)(PAT_DATA);
	for (i = 0; i < 1024; i++)
		PREG[i] = PARA_DATA[i];
	//	n = sizeof(PAT_DATA);n >>= 1;
	//	for(i=0;i<n;i++){SREG[i] = *padd;padd++;}
	//	for(;i<MAXSREG;i++){SREG[i] = 0;}
	//	for(i=0;i<72;i++){IOREG2[i+INSETREGI] = PARA_INSET[i];}
	//	for(i=0;i<132;i++){IOREG[i+OUTSETREGI] = PARA_OUTSET[i];}
	//	for(i=0;i<168;i++){IOREG[i+IOSETREGI] = IOREG[i+INSETREGI];}
	for (i = 0; i < MAXMREG; i++)
	{
		MREG[i] = 0;
	}
	for (i = 0; i < MAXEREG; i++)
	{
		EREG[i] = 0;
	}
	ReadTouchPara(); // 读取触摸屏校准参数
	ReadSysid();	 // 读出打版器编号

	FLASHADD_P = SST_SREG_SADD;

	YREG[SYS_STATUS] = 0;
	YREG[SYS_ENABLE] = 0;
	YREG[SYS_UPCOUNT] = 0;
	YREG[SYS_DNCOUNT] = 0;

	YREG[SYS_INPUTH] = 0;
	YREG[SYS_INPUTL] = 0;
	YREG[SYS_OUTPUTH] = 0;
	YREG[SYS_OUTPUTL] = 0;

	g_SysTimes.year = 16;
	g_SysTimes.month = 1;
	g_SysTimes.day = 1;
	g_SysTimes.hour = 0;
	g_SysTimes.minute = 0;
	YREG[SYS_MS] = 0;
	YREG[SYS_WH] = 0;
	YREG[SYS_MD] = 0x101;
	YREG[SYS_Y] = 0x16;

	P_CNR = ReadBackLight();
	if ((P_CNR < 10) || (P_CNR > 100))
		P_CNR = 40;

	gBatType = ReadBatType();  //读取上次打板类型
	g_usersave = ReadUserNo(); //读取上次用户编号
	//	ReadSeed();		//读出上一次保存的随机种子		???无此函数
}

const U16 PARA_DATA[] = {
	// 227*4= 908
	// 拔线器()
	0, 1, 1, 1,			   //	P_WIP	PREG[0]			// 拔线的ON/OFF													ON(1)
	0, 998, 30, 30,		   //	P_W1	PREG[1]			// 拔线输出(W)的开始输出时间		msec		0		998		30
	0, 998, 40, 40,		   //	P_W2	PREG[2]			// 拔线输出(W)的输出时间			msec		0		998		40
						   // 慢速起缝(Slow start)
	0, 2299, 200, 200,	   //	P_S		PREG[3]			// 慢速起缝速度						s/min		0		2299	200
	0, 1, 0, 0,			   //	P_SL	PREG[4]			// 慢速起缝的设定												OF(0)
	0, 5, 2, 2,			   //	P_SLN	PREG[5]			// 慢速起缝的针数					sti.		0		5		2
						   // 外压脚(Clamp)
	0, 1, 0, 0,			   //	P_OST	PREG[6]			// 外压脚没降下也可/不可动转									OF(0)
	0, 1, 0, 0,			   //	P_RPT	PREG[7]			// 重复缝制的设定												OF(0)
	0, 1, 1, 1,			   //	P_RP1	PREG[8]			// 压脚1的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP2	PREG[9]			// 压脚2的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP3	PREG[10]		// 压脚3的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP4	PREG[11]		// 压脚4的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP5	PREG[12]		// 压脚5的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP6	PREG[13]		// 压脚6的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP7	PREG[14]		// 压脚7的重复缝制设定											ON(1)
	0, 1, 1, 1,			   //	P_RP8	PREG[15]		// 压脚8的重复缝制设定											ON(1)
	0, 1, 0, 0,			   //	P_WHY	PREG[16]		// 压脚动作的优先顺序设定										OF(0)
	0, 1, 0, 0,			   //	P_FSR	PREG[17]		// 压脚步进动作的解除											OF(0)
	0, 1, 0, 0,			   //	P_1PD	PREG[18]		// 单踏板操作的设定												OF(0)
	0, 498, 0, 0,		   //	P_1T	PREG[19]		// 单踏板操作的延迟时间设定			x10msec		0		498		0
	0, 999, 300, 300,	   //	P_1A	PREG[20]		// 压脚1延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_2A	PREG[21]		// 压脚2延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_3A	PREG[22]		// 压脚3延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_4A	PREG[23]		// 压脚4延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_5A	PREG[24]		// 压脚5延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_6A	PREG[25]		// 压脚6延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_7A	PREG[26]		// 压脚7延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_8A	PREG[27]		// 压脚8延迟输出ON时间设定			x10msec		0		999		0
	0, 999, 300, 300,	   //	P_1B	PREG[28]		// 压脚1延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_2B	PREG[29]		// 压脚2延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_3B	PREG[30]		// 压脚3延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_4B	PREG[31]		// 压脚4延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_5B	PREG[32]		// 压脚5延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_6B	PREG[33]		// 压脚6延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_7B	PREG[34]		// 压脚7延迟输出OFF时间设定			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_8B	PREG[35]		// 压脚8延迟输出OFF时间设定			x10msec		0		999		0
	0, 2, 0, 0,			   //	P_OFB	PREG[36]		// 压脚动作分割数量的设定										NO(0)
	1, 8, 1, 1,			   //	P_FN	PREG[37]		// 有效压脚动作数量的设定						1		8		1
	0, 1, 0, 0,			   //	P_CF	PREG[38]		// 压脚动作连动的设定											OF(0)
	1, 4, 1, 1,			   //	P_F21N	PREG[39]		// 压脚动作分割时有效压脚数OF1-OF4				1		4		1
	1, 4, 1, 1,			   //	P_F22N	PREG[40]		// 压脚动作分割时有效压脚数OF5-OF8				1		4		1
	0, 1, 0, 0,			   //	P_CF1	PREG[41]		// 压脚动作分割时的连动设定OF1-OF4								OF(0)
	0, 1, 0, 0,			   //	P_CF2	PREG[42]		// 压脚动作分割时的连动设定OF5-OF8								OF(0)
	1, 4, 1, 1,			   //	P_F4BN	PREG[43]		// 有效压脚分组数量的设定						1		4		1
	1, 4, 1, 1,			   //	P_F4SN	PREG[44]		// 分组步进数量的设定							1		4		1
	0, 1, 0, 0,			   //	P_AF2	PREG[45]		// 空压两段动作压脚的选择										OF(0)
	0, 1, 0, 0,			   //	P_OPR	PREG[46]		// 读入资料受中压脚位置限制										OF(0)
						   // 缝制范围(Sewing area)
	0, 1, 0, 0,			   //	P_ALC	PREG[47]		// 缝制范围限制的解除											OF(0)
	0, 205, 100, 100,	   //	P_XL	PREG[48]		// X轴左侧范围限制的设定			mm			0		254		101
	0, 205, 100, 100,	   //	P_XR	PREG[49]		// X轴右侧范围限制的设定			mm			0		254		101
	0, 105, 50, 50,		   //	P_YU	PREG[50]		// Y轴后侧范围限制的设定			mm			0		254		51
	0, 105, 50, 50,		   //	P_YD	PREG[51]		// Y轴前侧范围限制的设定			mm			0		254		51
						   // 针位置(Needle position)
	0, 1, 0, 0,			   //	P_RU	PREG[52]		// 切线后针逆转上升机能											OF(0)
	0, 360, 30, 30,		   //	P_R8	PREG[53]		// RU的逆转角度的设定				deg.		0		360		30
	0, 360, 14, 14,		   //	P_U8	PREG[54]		// 针上停位置的停止角度				deg.		0		360		14
	0, 360, 50, 50,		   //	P_D8	PREG[55]		// 针下停位置的停止角度				deg.		0		360		50
						   // 断线检知(Thread trimming sensor)
	0, 1, 0, 0,			   //	P_S1	PREG[56]		// 断线检测的有效/无效
	0, 15, 3, 3,		   //	P_S2	PREG[57]		// 起缝后断线检测的无效针数
	0, 99, 2, 2,		   //	P_S3	PREG[58]		// 缝制途中断线检测的元效针数
	0, 2499, 300, 300,	   //	P_B		PREG[59]		// 断线检测动作速度的设定
	0, 1, 0, 0,			   //	P_TST	PREG[60]		// 断线检测时的切线有/无
						   // 原点(Home position)
	0, 3, 0, 0,			   //	P_HPM	PREG[61]		// 中途暂停时的原点复归方法										--(0) --/JS/JE/JC
	0, 1, 1, 1,			   //	P_HPC	PREG[62]		// 打开电源时原点复归动作的禁止									ON(1)
	0, 1, 1, 1,			   //	P_HPF	PREG[63]		// 自动原点复归动作的禁止										OF(0)		改过 与三菱不同
	0, 3, 0, 0,			   //	P_HPK	PREG[64]		// 复归键的设定													--(0) --/2/2L/NO
	0, 2, 0, 0,			   //	P_2HS	PREG[65]		// 到达第2原点时停止的方法										ST(0) ST/SW/NS
	0, 1, 0, 0,			   //	P_FHP	PREG[66]		// 任意原点的使用												OF(0)
	0, 1, 1, 1,			   //	P_CHS	PREG[67]		// 花样资料读出时的原点補正选择									ON(1)
	0, 1, 0, 0,			   //	P_SHX	PREG[68]		// X轴范围限制的原点复归										OF(0)
	0, 1, 0, 0,			   //	P_SHY	PREG[69]		// Y轴范围限制的原点复归										OF(0)
	0, 1, 0, 0,			   //	P_HPS	PREG[70]		// 上电时原点复归时间内,外压脚状态的设定						OF(0)
						   // 中途暂停(Halt)
	0, 1, 0, 0,			   //	P_STF	PREG[71]		// 紧急暂停按钮按下时空送的停止状态								CP(0) CP/ST
	0, 2, 1, 1,			   //	P_STN	PREG[72]		// 紧急暂停按钮按下时针的停止状态								UP(1) DN/UP/RN
	0, 2, 0, 0,			   //	P_STS	PREG[73]		// 紧急暂停按钮按下时各种输出的保护								AL(0) AL/FU/OF
	0, 2, 2, 2,			   //	P_ST2	PREG[74]		// 按第二次紧急暂停按钮时的动作									ST(2) UT/UP/ST
	0, 1, 0, 0,			   //	P_STP	PREG[75]		// 按紧急暂停按钮时电源的操作									--(0) --/PD
	0, 1, 1, 1,			   //	P_STD	PREG[76]		// 按中途暂停功能停止时外压脚的动作								FU(0) FU/FD	  改过 与三菱不同
	0, 1, 1, 1,			   //	P_STL	PREG[77]		// 中途暂停后再起缝时同位置的防止								ON(1)
						   // 计数(Counter)
	0, 3, 1, 1,			   //	P_CUP	PREG[78]		// 加算UP计数功能												ED(1) --/ED/ST/CY
	0, 3, 1, 1,			   //	P_CDN	PREG[79]		// 减算DN计数功能												ED(1) --/ED/ST/CY
	0, 3, 0, 0,			   //	P_UCM	PREG[80]		// 加算UP计数设定值的消除方法									--(0) --/RE/IT/CL
	0, 3, 0, 0,			   //	P_DCM	PREG[81]		// 减算UP计数设定值的消除方法									--(0) --/RE/IT/CL
	0, 1, 0, 0,			   //	P_PCM	PREG[82]		// 重开电源时计数值的消除方法									--(0) --/IT
	0, 1, 0, 0,			   //	P_CN	PREG[83]		// 针数的运算功能												OF(0)
	0, 1, 0, 0,			   //	P_UCC	PREG[84]		// 加算UP计数值的修正禁止										OF(0)
	0, 1, 0, 0,			   //	P_DCC	PREG[85]		// 减算DN计数值的修正禁止										OF(0)
	0, 1, 0, 0,			   //	P_USC	PREG[86]		// 依加算UP计数值而缝制作业的设定								OF(0)
	0, 1, 0, 0,			   //	P_DSC	PREG[87]		// 依减算DN计数值而缝制作业的设定								OF(0)
						   // 刹车(Brake)
	0, 1, 0, 0,			   //	P_WBK	PREG[88]		// 微弱刹车的设定												OF(0)
	0, 1, 0, 0,			   //	P_BKM	PREG[89]		// 微弱刹车的功能设定											E(0)  E/H
	0, 1, 0, 0,			   //	P_BNR	PREG[90]		// 微弱刹车的声音设定											OF(0)
	0, 99, 14, 14,		   //	P_BKT	PREG[91]		// 刹车时间的设定					x10msec		0		99		14
	0, 255, 50, 50,		   //	P_PW	PREG[92]		// 中压脚动作时的微弱刹车时间		msec		0		254		50
						   // 中压脚(Presser foot)
	0, 498, 2, 2,		   //	P_PD	PREG[93]		// 中压脚时间1(下降时)的设定		x10msec		0		498		36
	0, 498, 2, 2,		   //	P_PU	PREG[94]		// 中压脚时间2(上升时)的设定		x10msec		0		498		16
	0, 498, 10, 10,		   //	P_UW	PREG[95]		// 中压脚时间3(上升时)的设定		x10msec		0		498		10
	0, 1, 0, 0,			   //	P_BPF	PREG[96]		// 假缝后的空送移动时中压脚的设定								OF(0)
	0, 150, 150, 150,	   //  P_PLP   PREG[97]		// 中压脚行程设定
	0, 360, 100, 100,	   //	P_ZU8	PREG[98]		// 中压脚上升角度
	0, 360, 0, 0,		   //	P_Zd8	PREG[99]		// 中压脚上降角度
	0, 1, 1, 1,			   //	P_ZTM	PREG[100]		// 输入时中压脚同步设定
						   // 捲线(Bobbin winding)
	200, 3000, 1500, 1500, //	P_W		PREG[101]		// 捲线速度的设定					s/min		0		2299	1500
	0, 2, 0, 0,			   //	P_WSM	PREG[102]		// 捲线动作的操作设定											NO(0)
	2, 498, 30, 30,		   //	P_WT	PREG[103]		// 捲线的动作时间					sec			1		498		30
						   // 移动方式(送料方式)(Feed method)
	2, 2, 0, 0,			   //	P_WET	PREG[104]		// 外压脚的重量选择												L(0)  L/M/H
	0, 99, 20, 20,		   //	P_WEL	PREG[105]		// 外压脚的重量(WET=L)时的设定值				0		99		20
	0, 99, 26, 26,		   //	P_WEM	PREG[106]		// 外压脚的重量(WET=M)时的设定值				0		99		26
	0, 99, 35, 35,		   //	P_WEH	PREG[107]		// 外压脚的重量(WET=H)时的设定值				0		99		35
	0, 2, 0, 0,			   //	P_THI	PREG[108]		// 缝物厚度的选择												L(0)  L/M/H
	0, 60, 0, 0,		   //	P_TL	PREG[109]		// 缝物厚度(THI=L)时的设定值					0		60		0
	0, 60, 32, 32,		   //	P_TM	PREG[110]		// 缝物厚度(THI=M)时的设定值					0		60		32
	0, 60, 44, 44,		   //	P_TH	PREG[111]		// 缝物厚度(THI=H)时的设定值					0		60		44
	0, 2, 1, 1,			   //	P_FED	PREG[112]		// 送布方式的设定												ID(1) IT/ID/S
	0, 1, 1, 1,			   //	P_FEM	PREG[113]		// 空送动作的选择												S(1)  K/S
	0, 9, 4, 4,			   //	P_FES	PREG[114]		// 平顺空送的设定								0		9		9
	0, 1, 1, 1,			   //	P_FSM	PREG[115]		// 平顺空送的设定												ON(1)
	1, 15, 2, 2,		   //	P_TYA	PREG[116]		// 打版时箭印最大移动量(倍数)的设定	time		1		15		1
						   // 缝制速度(Sewing speed)
	0, 3000, 2800, 2800,   //	P_H		PREG[117]		// 最高速度							s/min		0		2299	2299
	0, 500, 200, 200,	   //	P_L		PREG[118]		// 最低速度							s/min		0		399		200
	0, 3000, 1500, 1500,   //	P_M		PREG[119]		// 中高速度MD1						s/min		0		2299	1500
	0, 3000, 800, 800,	   //	P_N		PREG[120]		// 中低速度MD2						s/min		0		2299	800
						   // 切线时间(Thread trimming timing)
	0, 4, 0, 0,			   //	P_LTM	PREG[121]		// 切线输出(T)的输出种类										T1(0) T1/T2/T3/T4/T5
	0, 4, 0, 0,			   //	P_LLM	PREG[122]		// 放线输出(L)的输出种类										L1(0) L1/L2/L3/L4/L5
	0, 360, 120, 120,	   //	P_T1	PREG[123]		// 切线输出(T)开始输出的时间/角度	msec		0		998		50
	0, 360, 240, 240,	   //	P_T2	PREG[124]		// 切线输出(T)输出的时间/结束角度	msec		0		998		0
	0, 998, 196, 196,	   //	P_L1	PREG[125]		// 放线输出(L)开始输出的时间/角度	msec		0		998		196
	0, 998, 0, 0,		   //	P_L2	PREG[126]		// 放线输出(L)输出的时间/结束角度	msec		0		998		0
	0, 399, 200, 200,	   //	P_T		PREG[127]		// 切线速度							s/min		0		399		200
	0, 1, 0, 0,			   //	P_ETC	PREG[128]		// 依针上升键使针上升时解除切线动作								OF(0)
	0, 1, 1, 1,			   //	P_TRM	PREG[129]		// 切线动作的ON/OFF												ON(1)
	0, 998, 30, 30,		   //	P_LP	PREG[130]		// Setting for gension release outputs time length at presser foot rise		x10msec		0		998		0
	0, 1, 0, 0,			   //	P_LSP	PREG[131]		// 一般松线器电磁阀的使用情况		x10msec		0		998		0
						   // 程序(步骤)(Step)
	0, 1, 0, 0,			   //	P_STE	PREG[132]		// 设定桌面转换时的步骤要点删除设定								OF(0)
	0, 1, 0, 0,			   //	P_STO	PREG[133]		// 步骤动作的ON/OFF												OF(0)
	0, 359, 0, 0,		   //	P_SUU	PREG[134]		// 解释度的执行行数					line		1		192		2
	0, 1, 0, 0,			   //	P_SP1	PREG[135]		// 程序运行方式 版本1				line		1		192		2
	0, 1, 0, 0,			   //	P_SKF	PREG[136]		// 当IOF输入时,程序完全中止		line		1		192		2
						   // 移动(寸动)(Jog)
	0, 2, 0, 0,			   //	P_JGM	PREG[137]		// 动键的功能设定												--(0) --/AL/HA
	0, 1, 0, 0,			   //	P_UJC	PREG[138]		// 针停在上停位置以外时的寸动键的设定							OF(0)
	0, 1, 0, 0,			   //	P_JGS	PREG[139]		// 连续移送时暂停代码的处理							OF(0)
						   // 送布角度(Feed angle)
	20, 198, 94, 94,	   //	P_S8	PREG[140]		// 开始送布的角度设定				deg.		25		198		94
	200, 356, 344, 344,	   //	P_E8	PREG[141]		// 停止送布的角度设定				deg.		100		356		344
	300, 356, 324, 324,	   //	P_T8	PREG[142]		// 低速时停止送布的角度设定			deg.		150		356		324
	0, 360, 0, 0,		   //	P_8D	PREG[143]		// 显示针下停位置的角度				deg.
						   // 液晶显示器(LCD panel)
	0, 1, 0, 0,			   //	P_KBC	PREG[144]		// 打版器的呜声解除												OF(0)
	0, 1, 1, 1,			   //	P_LCD	PREG[145]		// 打版器的连接													ON(1)
	0, 1, 1, 1,			   //	P_LOF	PREG[146]		// 打版器的省电功能												ON(1)
	0, 1, 1, 1,			   //	P_LNN	PREG[147]		// 打版器的显示颜色												ON(1)
	0, 1, 1, 1,			   //	P_LHP	PREG[148]		// 按键说明的显示												OF(1)
	1, 3, 1, 1,			   //	P_KNO	PREG[149]		// 标准模组的背景选择							1		3		1
	0, 10, 0, 4,		   //	P_CTR	PREG[150]		// 按键回音的次数选择							0		10		0
	0, 99, 50, 50,		   //	P_CNR	PREG[151]		// 打版器画面的对比								0		100		50
	60, 540, 120, 120,	   //	P_R		PREG[152]		// 打版器的省电亮灯时间				/60min		0		9998	300
						   // 输入信号(Input signal)
	0, 1, 0, 0,			   //	P_I1	PREG[153]		// I1输入的功能选择
	0, 1, 0, 0,			   //	P_I1L	PREG[154]		// I1输入的电平转换
	0, 1, 0, 0,			   //	P_I1M	PREG[155]		// I1输入的动作选择
						   // 输出信号(Output signal)
	0, 1, 0, 0,			   //	P_O1	PREG[156]		// O1输出的功能选择
	0, 1, 0, 0,			   //	P_O1L	PREG[157]		// O1输出的电平转换
	0, 1, 0, 0,			   //	P_O1C	PREG[158]		// O1输出的断续功能
	0, 1, 0, 0,			   //	P_O1M	PREG[159]		// O1输出的动作选择1
	0, 1, 0, 0,			   //	P_O1D	PREG[160]		// O1输出的ON延迟设定
	0, 1, 0, 0,			   //	P_O1E	PREG[161]		// O1输出的动作选择2
	0, 1, 0, 0,			   //	P_O_1A	PREG[162]		// O1输出到ON的延迟时间/脉波输出的OFF时间设定
	0, 1, 0, 0,			   //	P_O_1B	PREG[163]		// O1输出的ON时间/脉波的ON时间设定
	0, 1, 0, 0,			   //	P_O_1T	PREG[164]		// O1输出的全波输出时间设定
	0, 1, 0, 0,			   //	P_O_1C	PREG[165]		// O1输出的断续效率的设定
						   // 其它(Other)
	0, 2, 0, 0,			   //	P_GA	PREG[166]		// 高/低放大功能设定											L(0) L/LL/H
	0, 1, 0, 0,			   //	P_MIL	PREG[167]		// 铣床功能的ON/OFF												OF(0)
	0, 1, 0, 0,			   //	P_APC	PREG[168]		// 依端子台的输入指定缝制资料功能								OF(0)
	0, 1, 0, 0,			   //	P_POF	PREG[169]		// 依端子台的输入指定缝制资料时的补偿值							6(0) 6/1
	0, 1, 0, 0,			   //	P_ASC	PREG[170]		// 依外部的输入指定最高速度限制的功能							OF(0)
	0, 1, 0, 0,			   //	P_AER	PREG[171]		// 错误资料出力功能												OF(0)
	0, 1, 0, 0,			   //	P_PFL	PREG[172]		// 内部记忆体禁止书写删除设定									OF(0)
	1, 127, 43, 43,		   //	P_RN	PREG[173]		// Setting of length of continuous feed sew in maximum	/10mm	1	127	43
	0, 127, 0, 0,		   //	P_LH	PREG[174]		// 连续送布时的最大缝目长度
	0, 1, 0, 0,			   //	P_LCK	PREG[175]		// 禁止有控制箱的设定面板转换模组
	0, 1, 0, 0,			   //	P_PTC	PREG[176]		// 从缝制资料变更设定桌面的编号									OF(0)
	0, 1, 0, 0,			   //	P_SCP	PREG[177]		// 便捷功能键的交换												OF(0)
	0, 1, 0, 0,			   //	P_DFR	PREG[178]		// 从磁碟片读出的资料要改写或添加的方法							OF(0)
	0, 998, 0, 0,		   //	P_RT	PREG[179]		// 设定动力被解除延时到达							OF(0)
	0, 1, 0, 0,			   //	P_SPC	PREG[180]		// 序列通讯							OF(0)
	0, 1, 0, 0,			   //	P_APT	PREG[181]		// 图形时序由外接埠选择							OF(0)
	0, 2, 0, 0,			   //	P_LAN	PREG[182]		// 语言选择							OF(0)
	0, 1, 0, 0,			   //	P_VOI	PREG[183]		// 语音设定							OF(0)
	0, 1, 0, 0,			   //	P_E_1	PREG[184]		// 错误信息1
	0, 1, 0, 0,			   //	P_E_2	PREG[185]		// 错误信息2
	0, 1, 0, 0,			   //	P_E_3	PREG[186]		// 错误信息3
	0, 1, 0, 0,			   //	P_E_4	PREG[187]		// 错误信息4
	0, 1, 0, 0,			   //	P_E_I1	PREG[188]		// I1输入信号显示
	0, 1, 0, 0,			   //	P_E_UP	PREG[189]		// UP输入信号显示
	0, 1, 0, 0,			   //	P_E_DN	PREG[190]		// DN输入信号显示
	0, 1, 0, 0,			   //	P_E_XH	PREG[191]		// XH输入信号显示
	0, 1, 0, 0,			   //	P_E_YH	PREG[192]		// YH输入信号显示
	0, 1, 0, 0,			   //	P_E_ECA	PREG[193]		// ECA输入信号显示
	0, 1, 0, 0,			   //	P_E_ECB	PREG[194]		// ECB输入信号显示
	0, 1, 0, 0,			   //	P_E_CRS	PREG[195]		// CRS输入信号显示
	0, 1, 0, 0,			   //	P_E_O1O	PREG[196]		// 输出
	0, 1, 0, 0,			   //	P_E_O1D	PREG[197]		// 输出
	0, 1, 0, 0,			   //	P_E_P	PREG[198]		// 显示累积通电时间
	0, 1, 0, 0,			   //	P_E_M	PREG[199]		// 显示累积马达转动的时间
	0, 1, 0, 0,			   //	P_E_DV	PREG[200]		// 显示设定桌面的版本号
	0, 1, 0, 0,			   //	P_E_T	PREG[201]		// 显示机种
	0, 1, 0, 0,			   //	P_E_BV	PREG[202]		// 显示系统ROM的版本号
	0, 1, 0, 0,			   //	P_E_SV	PREG[203]		// 显示系统软件的版本号
						   // 特殊(Special)
	0, 99, 28, 28,		   //	P_LHH	PREG[204]		// High speed H upper limit setting			x100s/min		0		99		23
	0, 99, 0, 0,		   //	P_LHL	PREG[205]		// High speed H lower limit setting			x100s/min		0		99		0
	0, 99, 4, 4,		   //	P_LLH	PREG[206]		// Low speed L upper limit setting			x100s/min		0		99		4
	0, 99, 0, 0,		   //	P_LLL	PREG[207]		// LOw speed H lower limit setting			x100s/min		0		99		0
	0, 1, 0, 0,			   //	P_DSL	PREG[208]		// Speed limit setting														OF(0)
	0, 1, 1, 1,			   //	P_FSL	PREG[209]		// Speed limit cancel when continuos feed									ON(1)
	0, 1, 0, 0,			   //	P_STM	PREG[210]		// Needle stop mode selection												OF(0)
	0, 1, 0, 0,			   //	P_KU	PREG[211]		// Selection of UP signal													OF(0)
	0, 360, 270, 270,	   //	P_K8	PREG[212]		// UP position angle from down position		deg.			0		360		270
	0, 1, 0, 0,			   //	P_E8C	PREG[213]		// Detector error cancel mode												OF(0)
	0, 1, 0, 0,			   //	P_MSP	PREG[214]		// Max. speed for main motor												36(0)
	0, 1, 0, 0,			   //	P_PL	PREG[215]		// Sewing machine shaft/motor shaft speed setting selection					OF(0)
	0, 200, 100, 100,	   //	P_MR	PREG[216]		// Motor pulley diameter					mm				0		254		60	// 该参数改为电子夹线压力修正参数
	0, 300, 20, 20,		   //	P_SR	PREG[217]		// Sewing machine pulley diameter			mm				0		254		70	// 该参数改为梭床和针杆转动机型的最小转动角度 	 1600 = 360度 4.4444
	0, 99, 14, 14,		   //	P_ACT	PREG[218]		// Acceleration time simple setting 		msec			0		99		14
	0, 99, 0, 0,		   //	P_DCT	PREG[219]		// 减速时间简易设定											OF(0)
	0, 1, 0, 0,			   //	P_CCW	PREG[220]		// Main motor rotating direction											OF(0)
	0, 360, 28, 28,		   //	P_L8	PREG[221]		// Setting ofr down angle at low speed		deg.			0		360		28
	0, 2, 1, 1,			   //	P_PPD	PREG[222]		// XY轴回原点顺序设置													ON(1)
	0, 1, 1, 1,			   //	P_HXC	PREG[223]		// X motor CW/CCW															ON(1)
	0, 1, 1, 1,			   //	P_HXS	PREG[224]		// Stepping motor rotation direction at home positioning					ON(1)
	0, 1, 0, 0,			   //	P_HYC	PREG[225]		// Y motor CW/CCW															OF(0)
	0, 1, 1, 1,			   //	P_HYS	PREG[226]		// Stepping motor rotation direction at home positioning					ON(1)
	0, 65535, 2500, 2000,  //	P_F		PREG[227]		// Feed speed setting										0		65535	80
	0, 255, 105, 105,	   //	P_XHP	PREG[228]		// X axis pulse setting for home positioning				0		255		37
	0, 65535, 3300, 3300,  //	P_XH1	PREG[229]		// X axis pulse setting for home positioning(high speed)	0		65535	160
	0, 65535, 300, 300,	   //	P_XL1	PREG[230]		// X axis pulse setting for home positioning(low speed)		0		65535	2048
	0, 255, 20, 20,		   //	P_XT	PREG[231]		// Lowest movement amount when X axis return to home position	0	254		20
	0, 255, 55, 55,		   //	P_YHP	PREG[232]		// Y axis pulse setting for home positioning				0		255		37
	0, 65535, 3300, 3300,  //	P_YH1	PREG[233]		// Y axis pulse setting for home positioning(high speed)	0		65535	160
	0, 65535, 300, 300,	   //	P_YL1	PREG[234]		// Y axis pulse setting for home positioning(low speed)		0		65535	2048
	0, 255, 20, 20,		   //	P_YT	PREG[235]		// Lowest movement amount when Y axis return to home position	0	254		20
	0, 9999, 640, 640,	   //	P_L3	PREG[236]		// Pulse motor start time when X smooth feed is low			0		9998	640
	0, 9999, 175, 175,	   //	P_H3	PREG[237]		// Pulse motor start time when X smooth feed is high		0		9998	175
	0, 9999, 1, 1,		   //	P_X3	PREG[238]		// Change value of smooth feed of X							0		9998	1
	0, 9999, 640, 640,	   //	P_L4	PREG[239]		// Pulse motor start time when Y smooth feed is low			0		9998	640
	0, 9999, 175, 175,	   //	P_H4	PREG[240]		// Pulse motor start time when Y smooth feed is high		0		9998	175
	0, 9999, 1, 1,		   //	P_Y4	PREG[241]		// Change value of smooth feed of Y							0		9998	1
	0, 1, 1, 1,			   //	P_HME	PREG[242]		// 原点扩充功能							0		9998	1
	0, 2, 1, 1,			   //	P_MOS	PREG[243]		// 故障输出讯号被改变							0		9998	1
	0, 360, 1, 20,		   //	P_MS8	PREG[244]		// 主轴角度偏差值							0		9998	1
	0, 0, 0, 0,			   //	P_	PREG[245]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[246]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[247]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[248]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[249]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[250]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[251]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[252]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[253]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[254]		// 备用
	0, 0, 0, 0,			   //	P_	PREG[255]		// 备用
};

const U16 PARA_INSET[] = { //内容、逻辑、方式
	1, 0, 0,			   // I1，设置为SRT
	15, 0, 0,			   // I2，设置为IF1
	16, 0, 0,			   // I3，设置为IF2
	6, 1, 0,			   // I4，设置为STP
	80, 0, 0,			   // I5，设置为HES
	37, 0, 0,			   // I6，设置为THS
	15, 0, 0,			   // I7，设置为
	1, 0, 0,			   // I8，设置为
	2, 0, 0,			   // I9，设置为
	0, 0, 0,			   // IA，设置为
	0, 0, 0,			   // IB，设置为
	0, 0, 0,			   // IC，设置为
	0, 0, 0,			   // IE1，设置为
	0, 0, 0,			   // IE2，设置为
	0, 0, 0,			   // IE3，设置为
	0, 0, 0,			   // IE4，设置为
	0, 0, 0,			   // IE5，设置为
	0, 0, 0,			   // IE6，设置为
	0, 0, 0,			   // IE7，设置为
	0, 0, 0,			   // IE8，设置为
	0, 0, 0,			   // IE9，设置为
	0, 0, 0,			   // IEA，设置为
	0, 0, 0,			   // IEB，设置为
	0, 0, 0,			   // IEC，设置为
	0x5a, 0xa5, 0x01, 0x26};

const U16 PARA_OUTSET[] = {				  //内容、逻辑、方式、脉冲、启动延时、关闭延时、开启、关闭、全波、启延、关延
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O1，设置为T
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O2，设置为W
	2, 0, 0, 8, 0, 0, 20, 50, 1500, 0, 0, // O3，设置为L
	60, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O4，设置为OF1
	61, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O5，设置为OF2
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O6，设置为CB
	44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O7，设置为FN1
	45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O8，设置为FN2
	46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O9，设置为FN3
	47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OA，设置为FN4
	48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OB，设置为
	49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OC，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE1，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE2，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE3，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE4，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE5，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE6，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE7，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE8，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE9，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OEA，设置为F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OEB，设置为
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OEC，设置为
	0xa5, 0x5a, 0x00, 0x88};

//编程器设定参数
const volatile S16 PROMG_DATA[PROMGMAX][3] = {
	//最小值 - 最大值 - 默认值
	{1, 5, 1},		//0-JOG加速模式
	{0, 1, 0},		//1-连续的数据输入
	{-999, 999, 0}, //2-X坐标初期值(mm)
	{-999, 999, 0}, //3-Y坐标初期值(mm)
	{0, 1, 0},		//4-缝制点确认时的选择输出
	{0, 1, 0},		//5-交换HOME键和ENTER键
	{0, 1, 0},		//6-标记表示
	{-999, 999, 0}, //7-标记X位置(光标开始的移动量)(mm)
	{-999, 999, 0}, //8-标记Y位置(光标开始的移动量)(mm)
	{0, 1, 0},		//9-打板输入点自动连接
	{0, 1, 0},		//10-折线,曲线生成提示
	{1, 4, 2},		//11-打板移动速度
};
