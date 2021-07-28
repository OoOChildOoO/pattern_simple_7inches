//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  stm32cpu.h								*
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

// SYSTEM0 - 640x480 显示屏
// SYSTEM1 - 800x600 显示屏 (三菱风格)
// SYSTEM2 - 800x600 显示屏 (兄弟H款风格)

#ifndef __STM32CPU_H
#define __STM32CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx.h"

#define Debug 0	 //1:开启调试打印
#define Debug1 0 //1:开启调试打印
#define Debug2 0 //1:开启调试打印
#define Debug3 0 //1:开启调试打印
#define Debug4 0 //1:开启调试打印

#define EN_SEWCHECK 0 //0 不校验测试sew文件

#define PROMGMAX 30 //编程参数最大数(U16) 保存在ds2460
/*--Flash 版本说明
--640x480显示屏flash版本 0-500
--800x600显示屏flash版本500-1000
--800x480显示屏flash版本1000-1500
*/

#define UPDAT_MB 0	//切换系统升级主板
#define RDTIMES 400 // 连续读数据的次数
//#define		W25Q128_MAX		0x1000000			// W25q128容量 B

#define UPSFTRESET 1 //1-一键还原画面需要密码登录 0-不需要

#define USEMULTCOLOR 1 //0-黑白 1-彩色

#define SPLSHAPE_MIN 0
#define SPLSHAPE_MAX 359
#define SPLSHAPE_BAX 10
//#define		SPLSHAPE_MIN	-9
//#define		SPLSHAPE_MAX	20
//#define		SPLSHAPE_BAX	10
// SPLSHAPE_BAX+SPLSHAPE_MIN>0

#define PI 3.14156

#define Pindismax 127 * 2 //兄弟款最长针距

//#define DIS_LOGO		0		//无商标logo
#define DIS_LOGO 1 //茗菱商标logo
//#define DIS_LOGO		2		//开机动画avi logo.aiv

#define DIS_MODE 0 //显示模式 寸动,打板,修改画面不显示图案
//#define DIS_MODE	1	//显示模式 寸动,打板,修改画面显示图案

#define P1_MODE 0 //P1 画面通用显示模式
//#define P1_MODE		1	//P1 画面减算显示模式

//-----运行参数->波形分析
#define ADDR_OFF 10 //波形实际有效数据从第10个寄存器开始

//-----液晶屏---------
#define LCD_5cun 0 //5寸屏 640*480
#define LCD_8cun 1 //8寸屏 800*600
#define LCD_7cun 2 //7寸屏 800*480

#define HORIZONTAL 0 //横屏
#define VERTICAL 1	 //竖屏

/*********************系统设置-bigin************************/

#define MOVEUNIT 0 //移动单位0-0.1;1-0.05

#define MP3_SADDR 0

#define DISSUM 16 //每页显示的文件数目
#define SELECTNO_MAX 16

#define VMAXPAG 3 //0-3显存最大切换页面
//#define	DISSRC_TYPE		1	//电阻屏
//#define	DISSRC_TYPE		2	//电容屏
#define DISSRC_TYPE 3 //电阻屏+电容屏

#define LCD_TYPE LCD_7cun

#define VIEWTYPE HORIZONTAL
#define LCD_WIDTH 800  //显示屏宽度
#define LCD_HEIGHT 480 //显示屏高度

#define LCD_width 800  //显示屏宽度
#define LCD_height 480 //显示屏高度

#define LCD_TIME_WIDTH_LIMIT TIT_DISK_XPOS //标题时间显示栏显示位置 限制

#define UPGRADEADDR 0x08020600 //升级地址
#define UPGRADESIZE 0xdfa00	   //升级地址

#define DISP_VER 2021 // 打版器软件版本 1017
#define PUB_DATE 126  // 打版器发布时间 月-日

#define W25QUPIFOADDR 0xe00000 // W25Q存储升级程序信息起始地址

#define W25QMIFOADDR 0xe00020 // W25Q存储M-操作系统升级程序信息起始地址
#define W25QMDATADDR 0xe00040 // W25Q存储M-操作系统升级程序数据起始地址

#define W25QHIFOADDR 0xf00000 // W25Q存储H-操作系统升级程序信息起始地址
#define W25QHDATADDR 0xf00020 // W25Q存储H-操作系统升级程序数据起始地址

/*********************系统设置-end************************/

#define PLK_G 0x47
#define PLK_ER 0x54
#define MLK_G 0x45

//EMB 格式地址
#define DA_ADDR 0x11
#define DL_ADDR 0x13
#define PA_ADDR 0x19
#define PL_ADDR 0x1b
#define NA_ADDR 0x54
#define ORG_ADDR 0x112

#define EMB_AREAMAX 1024	  //最大绘图数据段数
#define EMB_CTRLMAX 1024 * 20 //最大控制点数

//---------独立按键
#define K_TOL 1	 //左
#define K_TOR 2	 //右
#define K_TOU 3	 //上
#define K_TOD 4	 //下
#define K_TOLU 5 //左上
#define K_TORU 6 //右上
#define K_TOLD 7 //左下
#define K_TORD 8 //右下
#define K_MENU 9 //菜单键
#define K_OK 10	 //确定键
#define K_CAL 11 //取消键
#define K_E 12	 //备用

//---伺服参数显示设置
//						  | | | | | | | |
#define EnDisP0 0x0000000000f67ff  //选择P0显示端口  25位
#define EnDisP1 0x00000026000effff //选择P1显示端口  40位
#define EnDisP2 0x0000000000c140fe //选择P2显示端口  30位
#define EnDisP3 0x00000000013f7fff //选择P3显示端口  30位
#define EnDisP4 0x0000f8ff80003c1f //选择P4显示端口  50位
#define EnDisP5 0x0000000910102347 //选择P5显示端口  40位
#define EnDisP6 0x0000000200704bf8 //选择P6显示端口  40位
#define EnDisP7 0x0000000000018000 //选择P7显示端口  50位

//---  磁盘类型----
#define MEM_IN 0
//#define	MEM_SD		1
#define MEM_DSP 1
#define MEM_USB 2
#define MEM_TF 3

#define LOOPMAX 20	//循环程序最大值
#define CFILEMAX 20 //循环程序文件数最大值

#define CreatBlkMax 12 //兄弟款，打板备份信息大小
#define f00000_1 0.00001

#define PEESS_MAXVAL 100 //压力阀值(该值越小压力越大)1	2
#define PEESS_MINVAL 1
#define PEESS_DEfVAL 4 //压力阀默认值

#define TolLangs 16 //多国语言 支持语言种类最大值

//------------存储设备------------------------------------
#define USB 0
#define SD 1
#define TF 2
#define SDTFRDY 35
#define USBTF USB //USB//TF

//Unicode 字库范围 24x24
#define Unicode0_Min 0
#define Unicode0_Max 0x24f
#define Unicode1_Min 0x1e00
#define Unicode1_Max 0x1eff
#define Unicode2_Min 0x1000
#define Unicode2_Max 0x109f

#define Mit_line_staddr 32000 //三菱款线条数据起始地址
#define Mit_pat_staddr 52000  //三菱款针数据起始地址
//#define USESST39VFXX						// 打开该定义时使用并行flash SST39VF6401B,否则使用串行flash W25Q128

#define FLASH_BASEAdd 0x2800 // 音乐数据起始位置
#define FILE_ALL 424		 // 一个音乐文件夹文件总数
#define DIR_SUM 600			 // 共保存3个文件夹,每个文件夹最大目录数600(最后一个只有80),每个目录8个字节

#define FLASHINDEXADDR 0x00003000 // flash前面索引区启始地址
#define FLASHDATAADDR 0x000a0000  // flash前面数据区启始地址
#define BINFILE_ALL 178			  // BIN文件个数
#define SST39VFADD 0x60000000	  //
#define SDRAMFLASHADD 0xc1800000  // 显示内容 图片 字库等数据缓冲区(在SDRAM第四块0xc1800000 - 0xc1ffffff)
#define Bank1ADDR 0xc0800000	  //

#define UP_ADDR 7//volatile U8 g_upinfo[0x20]
#define OS_ADDR 12
#define FL_ADDR 16
#define MP_ADDR 22

//#ifdef OLD_BOOT
//	#define W25QMIFOADDR		0xd00000		// W25Q存储M-操作系统升级程序信息起始地址
//	#define W25QMDATADDR		0xd00020		// W25Q存储M-操作系统升级程序数据起始地址

//	#define W25QHIFOADDR		0xe00000		// W25Q存储H-操作系统升级程序信息起始地址
//	#define W25QHDATADDR		0xe00020		// W25Q存储H-操作系统升级程序数据起始地址

//	#define W25QBACIFOADDR		0xf00000		// W25Q存储升级程序信息起始地址
//	#define W25QBACDATADDR		0xf00020		// W25Q存储升级程序数据起始地址

//#else
//	#define W25QUPIFOADDR		0xe00000		// W25Q存储升级程序信息起始地址

//	#define W25QMIFOADDR		0xe00020		// W25Q存储M-操作系统升级程序信息起始地址
//	#define W25QMDATADDR		0xe00040		// W25Q存储M-操作系统升级程序数据起始地址

//	#define W25QHIFOADDR		0xf00000		// W25Q存储H-操作系统升级程序信息起始地址
//	#define W25QHDATADDR		0xf00020		// W25Q存储H-操作系统升级程序数据起始地址
//#endif

//开机画面导入 文件位置:U盘(boot/boot.jpg) 文件大小600x800 其他不认
#define W25QBSIFOADDR 0xc00000 // W25Q存储开机换面信息
#define W25QBSDATADDR 0xc00020 // W25Q存储开机换面数据

#define DISPUSBRDLEN 0x800000 // 打版器上USB SD,一次读8MB
#define MCPUUSBRDLEN 0x7d0000 // 主板上USB,一次读160KB*50=8000KB

#define USEPRINTFDEBUG // 需要使用printf打印调试信息时定义

#define USESCREENSAVER 1 // 0：使用屏保功能		1：不使用屏保功能

#define ENREADPROTECT 0 // 0 :禁止读保护   1：开启读保护功能
#define PROTECTLEVEL2 0 // 1 :使用读保护级别2   其他：读保护级别1

#define FileIndexSize 36   //每帧文件信息大小
#define FileIndexBarpox 2  //每帧文件每帧文件条形码起始位置
#define FileIndexNamepox 4 //每帧文件每帧文件名称起始位置 ，旧系统为4
#define PatFilesMAX 999	   //花样文件数最大值  1~999

//------------------中压脚----------------------------------
#define PF_ABS_MAX 80 //中压脚最大值
#define PF_ABS_MIN 0  //中压脚最小值

//---------------电子夹线器-------------------------------
#define LF_ABS_MAX 100 //电子夹线器最大值
#define LF_ABS_MIN 20  //电子夹线器最小值

//文件类型
#define FILE_SEW 0
#define FILE_MITE 1
#define FILE_MITG 2
#define FILE_EMB 3
#define FILE_OTHER 4
#define FILE_FOLDER 5
#define FILE_SEQ 6
#define FILE_GPKMC 7
#define FILE_GPKMD 8
#define FILE_MACP 9
//花样类型
#define PAT_MIT 1 //三菱
#define PAT_SEW 2 //兄弟
#define PAT_EMB 3

#define GET_ALLFILE 0
#define GET_SEQFILE 1
#define GET_PATFILE 2  //mit+sew+emb
#define GET_EMBFILE 3  //EMB
#define GET_MITFILE 4  //mit
#define GET_SEWFILE 5  //sew
#define GET_GROFILE 6  //组合图形
#define GET_KMCFILE 7  //组合图 .kmc
#define GET_KMDFILE 8  //接合器 .kmd
#define GET_FOLDER 9   //获取文件夹
#define GET_MACFILE 10 //获取机器参数.mac

#define T_UNKNOW 0 //未知类型
#define T_MITE 1   //三菱E款
#define T_MITG 2   //三菱G款
#define T_SEWNO 3  //SEW有编号
#define T_SEW 4	   //SEW无编号
#define T_SEWENO 5 //SEW有编号E款
#define T_SEQ 6	   //SEO文件
#define T_EMBNO 7  //EMB文件有编号
#define T_EMB 8	   //EMB文件无编号
#define T_KMC 9	   //组合图.kmc文件
#define T_KMD 10   //接合器.kmd文件
#define T_MAC 10   //机器参数.mac文件

//机器类型
#define BAS311HN 0
#define BAS326H 1
#define BAS326H484 2
#define BAS341H 3
#define BAS342H 4
#define BAS34XH 5
#define UNKNOWN 6

#define MitFontColor Color_Blue		 //三菱款文字显示颜色
#define BorFontColor Color_Coral	 //兄弟款文字显示颜色 原RGB(177, 0, 3)
#define EmbFontColor Color_Red		 //兄弟款EMB文字显示颜色
#define NO_DATAColor Color_SlateBlue //无文件显示颜色

#define MINFILENO 1		 // 文件读写最小编号
#define MAXFILENO 999	 // 文件读写最大编号
#define SUMFILENO 999	 // 文件读写总文件数
#define PTINDEXNOADD 0	 // 文件索引表文件编号位置		0
#define PTINDEXNAMEADD 4 // 文件索引表文件名位置			32
#define PTINDEXSECADD 0	 // 文件索引表文件启始扇区位置	0
#define PTINDEXENCADD 2	 // 文件索引表文件条形吗位置		2
#define PTINDEXLENADD 0	 // 文件索引表文件长度位置		0
#define PTINDEXREDLEN 36 // 文件索引表每个目录项寄存器数

#define GroupPatMax 30	  //合并花样每组寄存器大小
#define GroupPNoAddr 0	  //合并花样花样编号起始地址
#define GroupPNameAddr 20 //合并花样名称起始地址
#define GroupPNumAddr 28  //合并花样记录花样数量起始地址
#define GroupPTypeAddr 29 //合并花样记录组合类型起始地址

#define GroupMax 20		 //合并模式最大组数
#define GroupComAddr 60	 //组合模式数据起始地址
#define GroupConAddr 660 //接合模式数据起始地址

//兄弟文件系统保存文件信息(保存在 BLKBUF寄存器中)
#define CUR_PATH_ADDR 0 //当前路径
#define CUR_PATH_MAX 512
#define FROM_PATH_ADDR CUR_PATH_ADDR + CUR_PATH_MAX //复制剪切来源路径
#define FROM_PATH_MAX 512
#define SAVE_PATH_ADDR FROM_PATH_MAX + FROM_PATH_MAX //保存操作过的文件路径
#define SAVE_PATH_MAX 0xfff

#define FALSE 0
#define TRUE 1
//#define WDTC()	{SETWDI();SETWDI();SETWDI();CLRWDI();}
#define WDTC()    \
	{             \
		SETWDI(); \
		CLRWDI(); \
		SETWDI(); \
		CLRWDI(); \
	}

//------------------数据类型定义----------------------------------
#define U8 unsigned char	   // 无符号 8位整型变量
#define u8 unsigned char	   // 无符号 8位整型变量
#define S8 signed char		   // 有符号 8位整型变量
#define s8 signed char		   // 有符号 8位整型变量
#define U16 unsigned short	   // 无符号16位整型变量
#define u16 unsigned short	   // 无符号16位整型变量
#define S16 signed short	   // 有符号16位整型变量
#define s16 signed short	   // 有符号16位整型变量
#define U32 unsigned int	   // 无符号32位整型变量
#define u32 unsigned int	   // 无符号32位整型变量
#define S32 signed int		   // 有符号32位整型变量
#define s32 signed int		   // 有符号32位整型变量
#define U64 unsigned long long // 无符号64位整型变量
#define u64 unsigned long long // 无符号64位整型变量
#define S64 signed long long   // 有符号64位整型变量
#define s64 signed long long   // 有符号64位整型变量
#define F32 float			   // 单精度浮点数（32位长度）
#define f32 float			   // 单精度浮点数（32位长度）
#define F64 double			   // 双精度浮点数（64位长度）
#define f64 double			   // 双精度浮点数（64位长度）
#define uint unsigned short	   // 无符号16位整型变量
#define uint16 unsigned short  // 无符号16位整型变量

#define FOSC ((U32)8000000)	   // 外部晶振频率
#define FPLL ((U32)192000000)  // PLL输出频率
#define FCLK ((U32)192000000)  // 内核运行频率
#define FHCLK ((U32)192000000) // 内部高速总线频率
#define FPCLK1 ((U32)48000000) // 分频器1频率 定时器2 3 4 串口2 3使用
#define FPCLK2 ((U32)96000000) // 分频器2频率 定时器1 串口1 ADC使用
#define FUSART1 ((U32)6000000) // 串口1时钟频率=96MHz / 16
#define FUSARTx ((U32)3000000) // 串口n时钟频率=48MHz / 16(n=2 3 4 5)
#define THCLK ((U32)12000000)  // 定时器1 8 9 10 11 时钟频率 			分频器2频率 / 16 = 6M
#define TLCLK ((U32)12000000)  // 定时器2 3 4 5 6 7 12 13 14 时钟频率 	分频器1频率 / 8  = 6M

#define SYSID 212020101	 // 系统编号 32bit
#define SYSKEY 12345678	 // 加密		32bit
#define SYSKEY 12345678	 // 加密		32bit
#define SYSDATE 20121202 // 日期		32bit

#define SYSTYPE 3020 // 系统型号
#define SYSVER 4000	 // 版本		8bit   2012.12.02

extern volatile U32 BIND, BINND;
extern volatile U32 statusf;
extern U32 Timer_lsn;
extern U32 Timer_plg;
extern volatile U8 tempout;

typedef struct
{
	U8 flag;  // 标记
	U32 len;  // 计算的理论长度
	U32 len1; // 花样当前长度
} SEW_CHECK;

typedef struct
{
	U16 datab;	// 数据长，
	U16 stopb;	// 停止位，
	U16 parity; // 奇偶校验位
} UARTMODE;

typedef struct
{
	U16 xpis; // 水平数据 0-19
	U16 ypis; // 垂直数据 0-14
	U16 xsize;
	U16 ysize;
} KEYPAD;

typedef struct
{
	U16 xpis; // 水平数据 0-19
	U16 ypis; // 垂直数据 0-14
	U16 xsize;
	U16 ysize;
	U16 colorB[4]; // 方框底色 、   方框底色
	U16 colorT[4]; // 文字颜色 或者 方框顶色
	U16 index[4];  // 文字索引 或者 文字颜色
} RECTPAD2;

typedef struct
{
	U16 xpis; // 水平数据 0-19
	U16 ypis; // 垂直数据 0-14
	U16 xsize;
	U16 ysize;
	U16 colorB[3]; // 方框底色 、   方框底色
	U16 colorT[2]; // 文字颜色 或者 方框顶色
	U16 index[3];  // 文字索引 或者 文字颜色
} RECTPAD;

typedef struct
{
	U16 xpis; // 水平数据 0-19
	U16 ypis; // 垂直数据 0-14
	U16 xsize;
	U16 ysize;
	U16 icon[4]; //按键内bmp图片序号或者按键颜色
	U16 filter; //过滤颜色
} BMPPAD;

typedef struct
{
	U16 border[3];	  //按键边框颜色
	U16 textColor[3]; //文字按钮文字颜色
	U16 icon[4];	  //按键内bmp图片序号或者按键颜色
	U16 titleStr[4];  // TIT提示文字显示数据索引
	U16 nameStr[4];	  // 按钮名称提示索引
} tydDisSrcIndex;

typedef struct
{
	U16 bflashAllFlag : 1;	  //更新标志位
	U16 bFlashPageFlag : 1;	  //换页标志
	u16 bFlashSelectFlag : 1; //选中序号更新
	U16 disNumDiv;			  //每页显示数量
	U16 selectMode;			  //0:单选，1:多选

	U16 patternNum; //总花样数 1-999
	U16 pageNum;	//总页数 1-999/div
	U16 pageNow;	//当前所在页 1-999/div
	U16 pageLast;	//选中的花样所在页 1-999/div

	U16 noInPageNow;  //当前选中序号，相对当前页   0-div
	U16 noInPageLast; //上一个序号，相对当前页   0-div
	U16 selectNoNow;  //当前选中序号,相对总花样数 1-999
	U16 selectNoLast; //上一个序号,相对总花样数 1-999

	U8 selectList[12]; //多选模式，当前页选中列表
	U32 dataAdd[12];   //一页的花样文件信息首地址
	U8 selectEnable[12]; //按钮使能
} tydPatternList;


#define PAT_CODEH 0
#define PAT_CODES 1
#define PAT_CODEE 2
#define PAT_CODEP 3

#define CONVAS1 1 //主界面
#define CONVAS2 2 //读取花样列表界面-多页面
#define CONVAS3 3 //图片内容信息、跳针界面
#define CONVAS4 4 //图片简单预览
#define CONVAS5 5 //打板主界面
#define CONVAS6 6 //修改、变换主界面
#define CONVAS7 7 //修改、变换选段界面
#define CONVAS8 8 //修改、变换绘制界面-大
#define CONVAS9 9 //修改、变换绘制界面-小

#define PM_BASE 0x00	//基本显示
#define PM_SEWING 0x01	//缝纫中
#define PM_PATTING 0x02 //绘制中
// #define PM_SEL_PART1 0x04 //选中段(x-x+n)
// #define PM_SEL_PART2 0x08 //选中段(a-b)
// #define PM_SEL_PART3 0x10 //选中段(a-b||a-x)

typedef struct
{
	U8 codeH : 1;  //原点图片
	U8 codeH2 : 1; //次原点图片
	U8 codeP : 1;  //中压脚图片
	U8 codeU : 1;  //上暂停图片
	U8 codeD : 1;  //下暂停图片
	U8 codeJ : 1;  //跳缝图片
	U8 codeC : 1;  //剪线图片
	U8 codeT : 1;  //张力图片
	U8 codeF1 : 1; //功能1图片
	U8 codeF2 : 1; //功能2图片
	U8 codeF3 : 1; //功能3图片
	U8 codeF4 : 1; //功能4图片
	U8 codeF5 : 1; //功能5图片
	U8 codeF6 : 1; //功能6图片
	U8 codeF7 : 1; //功能7图片
	U8 codeF8 : 1; //功能8图片
	U8 codeFX : 1; //功能9图片
} tydunPatCodeChild;

typedef union
{
	U32 code; //代码
	tydunPatCodeChild codeChild;
} tydunPatCode;

typedef struct
{
	U8 h_patMode;				   //高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
								   //0x04:选中区段(x-x+n);0x08:选中区段(a-b);0x10:选中区段(a-b||a-x);
	U8 h_patConvasIndex;		   //高-画布序号
	FunctionalState h_subline : 1; //高-辅助线
	FunctionalState h_imgZoom : 1; //高-图像缩放
	FunctionalState h_imgDrag : 1; //高-图像拖拽
	FunctionalState h_line : 1;	   //高-线段
	FunctionalState h_point : 1;   //高-针点
	FunctionalState h_org : 1;	   //高-原点
	FunctionalState h_code : 1;	   //高-代码
	FunctionalState h_exCode : 1;  //高-其他显示代码
	FunctionalState h_orgOffset : 1;  //高-原点偏移显示标志位

	FunctionalState l_line_udZhou : 1; //低-高-分离轴
	FunctionalState l_line_speed : 1;  //低-低-速度

	U8 l_org_M_XL; //低-0x01:小点;0x02:大十字

	FunctionalState l_ex_select : 1;	 //(显示选中的线段使用)低-共存-选中区段 H/S/E/P/+
	FunctionalState l_ex_pat : 1;		 //(显示绘图的线段使用)低-共存-花样段代码/11/22/33/ 变换花样-显示针段
	FunctionalState l_ex_ctrl : 1;		 //(绘制控制点使用-多点输入)低-共存-控制点/1/2/3/4/+/
	FunctionalState l_ex_cursor : 1;	 //(绘制当前操作光标使用-单点输入)低-共存-当前光标
	FunctionalState l_ex_selectNear : 1; //(选择触摸周围点使用)与l_ex_topNear互斥-附近点选择吸附
	FunctionalState l_ex_topNear : 1;	 //(提示光标周围点使用)与l_ex_selectNear互斥-附近点高亮提示

	U8 l_ex_select_mode; //0x01:显示代码;0x02：显示大十字光标;0x04:显示小十字光标
	U8 l_ex_select_type; //0x00: 仅P点 0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
	U8 l_ex_ctrl_mode;	 //0x01:显示代码;0x02：显示大十字光标
	U8 l_ex_ctrl_type;	 //1.修改花样ADD-段修改-显示多点控制;2:变换花样-缩放/旋转-显示基准点;3:
						 //4:绘制花样-单点输入;5:绘制花样-多点输入;6:绘制花样-花样插入
						 //7:修改花样MODIFY1-单点输入;8:修改花样MODIFY1-A-B单点输入;
						 //9:修改花样MODIFY1-A-B多点输入g_jogmode=0 10:修改花样MODIFY1-A-B多点输入g_jogmode=1

	U8 l_ex_cursor_mode; //0x01:显示代码;0x02:显示大十字光标;0x04:显示小十字光标
	U8 l_ex_cursor_type; //1:缝纫中;2:变换花样-单点输入;
						 //4:绘制花样-单点输入;5:绘制花样-多点输入;6:绘制花样-花样插入
						 //7:修改花样MODIFY1-单点输入;8:修改花样MODIFY1-A-B单点输入;
						 //9:修改花样MODIFY1-A-B多点输入g_jogmode=0 10:修改花样MODIFY1-A-B多点输入g_jogmode=1

	tydunPatCode l_code; //低-共存-显示代码
} tydstPatRule;

extern tydstPatRule patRule;
#define PAT_CODE_ALL patRule.l_code.code
#define PAT_CODE_H patRule.l_code.codeChild.codeH
#define PAT_CODE_H2 patRule.l_code.codeChild.codeH2
#define PAT_CODE_P patRule.l_code.codeChild.codeP
#define PAT_CODE_U patRule.l_code.codeChild.codeU
#define PAT_CODE_D patRule.l_code.codeChild.codeD
#define PAT_CODE_J patRule.l_code.codeChild.codeJ
#define PAT_CODE_C patRule.l_code.codeChild.codeC
#define PAT_CODE_T patRule.l_code.codeChild.codeT
#define PAT_CODE_F1 patRule.l_code.codeChild.codeF1
#define PAT_CODE_F2 patRule.l_code.codeChild.codeF2
#define PAT_CODE_F3 patRule.l_code.codeChild.codeF3
#define PAT_CODE_F4 patRule.l_code.codeChild.codeF4
#define PAT_CODE_F5 patRule.l_code.codeChild.codeF5
#define PAT_CODE_F6 patRule.l_code.codeChild.codeF6
#define PAT_CODE_F7 patRule.l_code.codeChild.codeF7
#define PAT_CODE_F8 patRule.l_code.codeChild.codeF8
#define PAT_CODE_FX patRule.l_code.codeChild.codeFX

#define PATRULES_ORG_OFFSET 0  //原点偏移使能标志

#define PATRULES_P1 1		   //主界面
#define PATRULES_READ 2		   //花样读取界面
#define PATRULES_VIEW_DETAIL 3 //花样直接预览-含有详细信息

#define PATRULES_JOG_DETAIL 4 //花样跳针预览-可以实现跳针等操作
#define PATRULES_JOG_X_N 5	  //花样跳针预览-可以实现跳针等操作
#define PATRULES_JOG_A_B 6	  //花样跳针预览-可以实现跳针等操作
#define PATRULES_JOG_A_X 7	  //花样跳针预览-可以实现跳针等操作

#define PATRULES_VIEW_SIMPLE 8 //花样生成预览-不含有详细信息

#define PATRULES_CREATE1 9	//创建花样-单点输入
#define PATRULES_CREATE2 10 //创建花样-多点输入-移动的光标超过显示范围
#define PATRULES_CREATE3 11 //创建花样-花样插入

#define PATRULES_MODIFY_MAIN 12 //修改花样-主界面
#define PATRULES_MODIFY_ADD 13	//修改花样-选择针点
#define PATRULES_MODIFY_ADD1 14 //修改花样-一个控制点
#define PATRULES_MODIFY_ADD2 15 //修改花样-两个控制点
#define PATRULES_MODIFY_ADD3 16 //修改花样-多个控制点

#define PATRULES_MODIFY1_SINGLE 17	   //修改花样MODIFY1-单点输入
#define PATRULES_MODIFY1_SINGLE_A_B 18 //修改花样MODIFY1-A-B单点输入
#define PATRULES_MODIFY1_MUL_A_B0 19   //修改花样MODIFY1-A-B多点输入g_jogmode=0
#define PATRULES_MODIFY1_MUL_A_B1 20   //修改花样MODIFY1-A-B多点输入g_jogmode=1
#define PATRULES_MODIFY1_MUL_A_X 21	   //修改花样-控制多点-X-X+N

#define PATRULES_MODIFY2_MUL_A_X 22 //修改花样-控制多点-X-X+N

#define PATRULES_MODIFY3_ENSURE 23	  //修改花样-确认输入

#define PATRULES_CONVERT_MAIN 24	  //变换花样-主界面
#define PATRULES_CONVERT1_MOVE 25	  //变换花样-坐标输入
#define PATRULES_CONVERT1_MOVE_ORG 26	  //变换花样-坐标输入-原点偏移输入
#define PATRULES_CONVERT1_NUM 27	  //变换花样-数字输入
#define PATRULES_CONVERT1_ENSURE 28	  //变换花样-确认输入
#define PATRULES_CONVERT1_OFFSET 29	  //变换花样-离边缝段显示
#define PATRULES_CONVERT1_OFFSMULT 30 //变换花样-多重缝段显示

typedef struct
{
	U16 conPosX;
	U16 conPosY;
	U16 conSizeX;
	U16 conSizeY;
	U16 conOrgX;
	U16 conOrgY;
	// U16 imgSizeX;
	// U16 imgSizeY;
} tydstConvasNature;

typedef struct
{
	S16 x;
	S16 y;
} Point;

typedef struct
{
	U16 check_sum;
	U16 check_xor;
} Embcheck;

typedef struct __UserPW
{
	U8 name[12];
	U8 password[12];
} UserPW;

typedef struct __PassWordPara
{
	U8 newuser;
	U8 newmode;
	U8 olduser;
	U8 oldmode;
	UserPW users[10];
} PassWordPara;

typedef struct __SystemTime
{
	U8 year;
	U8 month;
	U8 day;
	U8 hour;
	U8 minute;
} SystemTime;

typedef struct _saveINFO
{
	U16 No;	  // 花样编号
	U32 len;  //花样长度
	U16 Type; //花样类型
} saveINFO;

typedef struct _MachineInfo
{
	volatile S16 max_xl;
	volatile S16 max_xr;
	volatile S16 max_yu;
	volatile S16 max_yd;
} MachineInfo;

typedef struct
{
	U8 base;
	U8 oldbase;
	U8 val;
	U8 set;
	U8 max;
	U8 min;
} THDTSN;

typedef struct
{
	S8 base;
	S8 oldbase;
	S8 val;
	S8 set;
	S8 max;
	S8 min;
} PRFOOT;

typedef struct //追加重叠缝参数
{
	S16 stposx; //区段开始点坐标值
	S16 stposy;
	S16 enposx; //区段结束点坐标值
	S16 enposy;
	U8 occlude; //区段封闭标记 1-封闭 0-非封闭
	U8 retimes; //重叠缝次数
	U8 evepins; //每次重叠缝针数
	U8 evecnts;
	S16 repdis[9][2];
	//	S16 repdis1[9][2];	//第一重XY坐标增量
	//	S16 repdis2[9][2];	//第二重XY坐标增量
	//	S16 repdis3[9][2];	//第三重XY坐标增量
	//	S16 repdis4[9][2];	//第四重XY坐标增量

} OVLP;

typedef struct
{
	U16 sno;		// 记存体编号
	U16 fno;		// 文件编号			01-00
	char fname[65]; // 文件名			20-27

	U32 feed_c;	  // 当前步行位置			//2013.3.27
	U16 feed_sum; // 花样总针数		09-08/4   //2013.3.27
	U16 feed_pat; // 花样缝针数		09-08/4-(pat中不含01,21,41,61指令)  //2013.3.27
	U16 draw_add; // 绘图起始位置		48h
	U16 line_add; // 线条起始位置		05-04+40h
	U32 pat_add;  // 针起始位置		2013.3.27

	S16 patmX; // 花样最大X值
	S16 patmY; // 花样最大Y值
	S16 patiX; // 花样最小X值
	S16 patiY; // 花样最小Y值
	S32 scale; // 比例
	U16 p_2hd; // 次原点置位
	U16 org_f; // 原点置位

	S16 X2hd; // 次原点位置X坐标
	S16 Y2hd; // 次原点位置Y坐标
	S16 orgX; // 原点位置X坐标
	S16 orgY; // 原点位置Y坐标

	char ver[5]; // 版本				3C-3F
	U16 maxX;	 // 机型最大移动X
	U16 maxY;	 // 机型最大移动Y

	S8 PFmax;	 //中压脚最大值
	S8 PFmin;	 //中压脚最小值
	U32 pat_len; //花样数据长度 8b
	S32 sew_len; //花样缝纫总长

	U8 macType; //机器类型 针对EMB格式
	U8 endcode; //结束代码
	U16 year;	//年
	U8 month;	//月
	U8 day;		//日
	U8 hour;	//时
	U8 min;		//分
	U8 sec;		//秒
} PATHEAD;

typedef struct
{
	S16 ox; //
	S16 oy; //

	U16 feedc; //
	U16 ppc[9];
} OLDPAT;

typedef struct
{
	U32 ptsize;	   //花样数据大小B
	U32 lenaddr;   //输出打板数据长度存放地址
	U32 pataddr;   //输出花样数据起始地址
	U32 limaddr;   //输出花样极限存放地址
	U32 orgaddr;   //输出花样坐标起点存放地址
	U32 ecodeaddr; //输出花样结束代码存放地址
	U32 nameaddr;  //输出花样名称存放地址
	U32 timeaddr;  //输出花样建立时间存放地址
} ISEWADD;

typedef struct
{
	U16 cmd; //查找到的打板命令
	S16 dx;	 //X方向变化量
	S16 dy;	 //Y方向变化量
	U8 off;	 //本次打板命令数据总长 2B
} ISEWCMD;

typedef struct
{
	U32 ox; //
	U32 oy; //
	U8 IsDat;
	U16 ppc[18][18];
} OLDPAT1;

typedef struct
{
	S32 x;
	S32 y;
} COORDINATE;

typedef struct
{
	S16 x0;
	S16 y0;
	S16 x1;
	S16 y1;
} COORDINATE2;

typedef struct
{
	U16 flag;	   //标记 0x5a5a
	U16 mode;	   //当前使用开机画面
	U32 size;	   //开机画面大小U16
	U16 vmlk;	   //显示mlk信息
	U16 checksum;  //开机画面数据校验和
	U16 checksum1; //开机画面信息头校验和
} BOOTSCR;

#define MAXZONE 30 //最大区段数
typedef struct
{
	U8 cmd;			   // 绘图数据指令
	U32 dlen;		   // 本段绘图数据长度
	U16 plen;		   // 针数据长度
	U8 zone;		   // 区段数
	U8 type;		   // 区段类型
	U8 ztype[MAXZONE]; // 每段区段类型
	U8 ctls[MAXZONE];  // 每段区段控制点数
	U8 plink;		   // 连接方式
	U16 ptich;		   //	针距
} EMBDINFO;			   //emb花样格式绘图数据信息

typedef struct
{
	U8 cmd;	  // 针数据指令
	U8 cmd1;  // 针数据指令
	U16 pins; // 针数
	U8 code;  // 附加功能 1-上暂停,2-下暂停,3-减速,4-中压脚,5-松线,6-缝纫速度,7-附加功能,8-暂停开,9-暂停关
	U16 cval; // 代码值
} EMBPINFO;	  //emb花样格式绘图数据信息

typedef struct
{
	U8 cmd;		   // 针数据指令
	S16 px;		   // x坐标绝度值
	S16 py;		   // y坐标绝对值
	S16 dx;		   // 到下一针X变化值
	S16 dy;		   // 到下一针y变化值
	U8 code;	   // 附加功能
	U8 code1;	   // 附加功能1
	U8 codeval[7]; // code值 0-断缝;1-压脚;2-张力;3-缝纫速度;4-低速;5-选项输出;6-暂停
	U16 area;	   //所属绘图数据号
	U8 conect;	   //区域链接方式
} EMBPINS;		   //emb针信息

typedef struct
{
	U16 drawPins; //绘图数据针信息
	U32 newLen;	  //针数据度
} EMBNEWPINS;

#pragma pack(1)
typedef struct _select_id_message
{
	U16 Select_No; // 花样文件号
	U16 Fileno;	   // 选择列表的序号
} select_id_message;

typedef struct __ProInfo //当前程序版本
{
	U16 Data[3];
} ProInfo;

typedef struct __RunRecord //运行记录
{
	U8 usrNo;	 //用户编号
	U16 patNo;	 //花样编号
	U8 hour;	 //小时
	U8 min;		 //分钟
	U8 sec;		 //秒
	U8 timeuint; //单位
	U8 times;	 //加工时长
} RunRecord;

typedef struct _PATPOINT
{
	U16 oposx;
	U16 oposy;
	U16 ocolor[40];

	U16 posx;
	U16 posy;
	//	U16	color[9];
} PATPOINT;

typedef struct _CREATEPT
{
	U8 Type;	  //类型
	U8 L_Type;	  //线种
	U8 P_dis;	  //针距
	U8 C_mode;	  //生成模式
	U8 C_menthod; //连接方式
	U8 T_dir;	  //旋转方向
} CREATEPT;

typedef struct
{
	S16 posx; // x绝对坐标
	S16 posy; // y绝对坐标
	S16 dx;	  // x下一针的增量
	S16 dy;	  // y下一针的增量
	U8 speed; // 速度
	U8 abspf; // 中压脚绝对值
} PINMSG;	  //针点信息

#define ERRMSGMAX 10 //最多报警信息次数
typedef struct _ERRMSG
{
	U16 max;			//不同报警信息次数
	U16 err[ERRMSGMAX]; //保存不同报警信息编号
} ERRMSG;

//typedef struct _WAVESDAT{
//	U32 raddr;	//读取地址
//	U16 rlen;	//线种
//	U32 saddr;	//保存地址
//	U16 sdat[120];	//保存数据
//	U16 v[3];
//	//U16 c[2];
//}WAVESDAT;

#define FALSES 0
#define TRUES 1

// 8  4  2  1	//
// 3  2	 1	0	H000x
// 7  6	 5	4	H00x0
// 11 10 9  8	H0x00
// 15 14 13 12	Hx000

//#define KEYIN0			0x2000			// PC 13
//#define KEYIN1			0x4000			// PC 14
//#define KEYIN2			0x8000			// PC 15
//#define KEYIN3			0x0002			// PC 1
//#define KEYIN4			0x0040			// PC 6
//#define KEYIN5			0x0080			// PC 7
//#define KEYIN6			0x0010			// PC 4
//#define KEYIN7			0x0020			// PC 5

#define KEYIN0 0x0002 // PC 1 ->S5
#define KEYIN1 0x8000 // PC 15 ->S4
#define KEYIN2 0x4000 // PC 14 ->S1
#define KEYIN3 0x2000 // PC 13 ->S2
#define KEYIN4 0x0080 // PC 7 ->S3
#define KEYIN5 0x0040 // PC 6 ->S6
#define KEYIN6 0x0020 // PC 5 ->S7
#define KEYIN7 0x0010 // PC 4

#define MCIDAT0 0x0100 // PC 8
#define MCIDAT1 0x0200 // PC 9
#define MCIDAT2 0x0400 // PC 10
#define MCIDAT3 0x0800 // PC 11
#define MCICLK 0x1000  // PC 12
#define MCICMD 0x0004  // PD 2
#define MCIPWR 0x0008  // PD 3
#define MCIINS 0x0008  // PI 3
#define INKEYD 0xe0f2  // PC 1 4 5 6 7 13 14 15
#define INADATA 0x0020 // PH 5
#define INBDATA 0x0004 // PE 2
#define INCDATA 0x0108 // PI 8 3
#define INDDATA 0x4000 // PG 14

#define CANRD 0x1000  // PB 12
#define CANTD 0x2000  // PB 13
#define I2CSCL 0x0100 // PB 8
#define I2CSDA 0x0200 // PB 9
#define TXD1 0x0040	  // PB 6
#define RXD1 0x0400	  // PA 10
#define TXD2 0x0004	  // PA 2
#define RXD2 0x0008	  // PA 3

#define SCK 0x0020	  // PA 5
#define MISO 0x0040	  // PA 6
#define MOSI 0x0080	  // PA 7
#define WCS1 0x0010	  // PH 4
#define WCS2 0x0001	  // PB 0
#define WCS3 0x8000	  // PA 15
#define TFCS 0x0002	  // PB 1
#define LTCS 0x0010	  // PA 4
#define LTRST 0x0004  // PB 2
#define LTPKTF 0x0020 // PH 5

#define T_SCK 0x0400	// PB 10
#define T_MISO 0x4000	// PB 14
#define T_MOSI 0x8000	// PB 15
#define T_SSEL 0x0800	// PB 11
#define T_BUSY 0x0100	// PI 8
#define T_PENIRQ 0x0004 // PE 2

#define C_INT 0x0008 // PH 3

#define VSCLK 0x0008  // PB 3
#define VSMISO 0x0010 // PB 4
#define VSMOSI 0x0020 // PB 5
#define VSXCS 0x0080  // PB 7
#define VSRST 0x1000  // PG 12
#define VSXDCS 0x2000 // PG 13
#define DREQ 0x4000	  // PG 14

#define USBVBUS 0x0200	// PA 9
#define USBFSEN 0x0100	// PA 8
#define USBFSDM 0x0800	// PA 11
#define USBFSDP 0x1000	// PA 12
#define WDI 0x0001		// PA 0
#define NBELL 0x0002	// PA 1
#define USBLED 0x0100	// PH 8
#define SDLED 0x0040	// PE 6
#define LCDONOFF 0x0800 // PI 11
#define LCDPWM 0x0200	// PF 9
#define LEDPWM 0x0100	// PF 8
#define TEST1 0x0040	// PH 6
#define TEST2 0x0080	// PH 7
#define IOMODE 0x0040	// PF 6
#define TFINS 0x0004	// PH 2

#define WKEYIN0 GPIOC->IDR &KEYIN0 // PC 1 ->S5
#define WKEYIN1 GPIOC->IDR &KEYIN1 // PC 15 ->S4
#define WKEYIN2 GPIOC->IDR &KEYIN2 // PC 14 ->S1
#define WKEYIN3 GPIOC->IDR &KEYIN3 // PC 13 ->S2
#define WKEYIN4 GPIOC->IDR &KEYIN4 // PC 7 ->S3
#define WKEYIN5 GPIOC->IDR &KEYIN5 // PC 6 ->S6
#define WKEYIN6 GPIOC->IDR &KEYIN6 // PC 5 ->S7
#define WKEYIN7 GPIOC->IDR &KEYIN7 // PC 4

#define WMCIDAT0 GPIOC->IDR &MCIDAT0 // PC 8
#define WMCIDAT1 GPIOC->IDR &MCIDAT1 // PC 9
#define WMCIDAT2 GPIOC->IDR &MCIDAT2 // PC 10
#define WMCIDAT3 GPIOC->IDR &MCIDAT3 // PC 11
#define WMCICLK GPIOC->IDR &MCICLK	 // PC 12
#define WMCICMD GPIOD->IDR &MCICMD	 // PD 2
#define WMCIPWR GPIOD->ODR &MCIPWR	 // PD 3
#define WMCIINS GPIOI->IDR &MCIINS	 // PI 3
#define WINKEYD GPIOC->IDR &INKEYD	 // PC 1 4 5 6 7 13 14 15	Y
#define WINADATA GPIOH->IDR &INADATA // PH 5						>>5	= 0
#define WINBDATA GPIOE->IDR &INBDATA // PE 2						Y
#define WINCDATA GPIOI->IDR &INCDATA // PI 8 3					Y
#define WINDDATA GPIOG->IDR &INDDATA // PG 14					>>2	= 12
#define WINEDATA GPIOH->IDR &TFINS	 // PH 2						<<7 = 9

#define WCANRD GPIOB->IDR &CANRD   // PB 12
#define WCANTD GPIOB->ODR &CANTD   // PB 13
#define WI2CSCL GPIOB->ODR &I2CSCL // PB 8
#define WI2CSDA GPIOB->ODR &I2CSDA // PB 9
#define WTXD1 GPIOB->ODR &TXD1	   // PB 6
#define WRXD1 GPIOA->IDR &RXD1	   // PA 10
#define WTXD2 GPIOA->ODR &TXD2	   // PA 2
#define WRXD2 GPIOA->IDR &RXD2	   // PA 3

#define WSCK GPIOA->ODR &SCK	   // PA 5
#define WMISO GPIOA->IDR &MISO	   // PA 6
#define WMOSI GPIOA->ODR &MOSI	   // PA 7
#define WWCS1 GPIOH->ODR &WCS1	   // PH 4
#define WWCS2 GPIOB->ODR &WCS2	   // PB 0
#define WTFCS GPIOB->ODR &TFCS	   // PB 1
#define WLTCS GPIOA->ODR &LTCS	   // PA 4
#define WLTRST GPIOB->ODR &LTRST   // PB 2
#define WLTPKTF GPIOH->IDR &LTPKTF // PH 5

#define WT_SCK GPIOB->ODR &T_SCK	   // PB 10
#define WT_MISO GPIOB->IDR &T_MISO	   // PB 14
#define WT_MOSI GPIOB->ODR &T_MOSI	   // PB 15
#define WT_SSEL GPIOB->ODR &T_SSEL	   // PB 11
#define WT_BUSY GPIOI->IDR &T_BUSY	   // PI 8
#define WT_PENIRQ GPIOE->IDR &T_PENIRQ // PE 2

#define WVSCLK GPIOB->ODR &VSCLK   // PB 3
#define WVSMISO GPIOB->IDR &VSMISO // PB 4
#define WVSMOSI GPIOB->ODR &VSMOSI // PB 5
#define WVSXCS GPIOB->ODR &VSXCS   // PB 7
#define WVSRST GPIOG->ODR &VSRST   // PG 12
#define WVSXDCS GPIOG->ODR &VSXDCS // PG 13
#define WDREQ GPIOG->IDR &DREQ	   // PG 14

#define WUSBVBUS GPIOA->IDR &USBVBUS   // PA 9
#define WUSBFSEN GPIOA->ODR &USBFSEN   // PA 8
#define WUSBFSDM GPIOA->IDR &USBFSDM   // PA 11
#define WUSBFSDP GPIOA->IDR &USBFSDP   // PA 12
#define WWDI GPIOA->ODR &WDI		   // PA 0
#define WNBELL GPIOA->ODR &NBELL	   // PA 1
#define WUSBLED GPIOH->ODR &USBLED	   // PH 8
#define WSDLED GPIOE->ODR &SDLED	   // PE 6
#define WLCDONOFF GPIOI->ODR &LCDONOFF // PI 11
#define WLCDPWM GPIOF->ODR &LCDPWM	   // PF 9
#define WLEDPWM GPIOF->ODR &LEDPWM	   // PF 8
#define WTEST1 GPIOH->ODR &TEST1	   // PH 6
#define WTEST2 GPIOH->ODR &TEST2	   // PH 7
#define WIOMODE GPIOF->IDR &IOMODE	   // PF 6
#define WTFINS GPIOH->IDR &TFINS	   // PH 2

#define WWKEYIN0 BIND &KEYIN0		// PC 13
#define WWKEYIN1 BIND &KEYIN1		// PC 14
#define WWKEYIN2 BIND &KEYIN2		// PC 15
#define WWKEYIN3 BIND &KEYIN3		// PC 1
#define WWKEYIN4 BIND &KEYIN4		// PC 6
#define WWKEYIN5 BIND &KEYIN5		// PC 7
#define WWKEYIN6 BIND &KEYIN6		// PC 4
#define WWKEYIN7 BIND &KEYIN7		// PC 5
#define WWLTPKTF BIND &bit0			// PH 5
#define WWT_PENIRQ BIND &T_PENIRQ	// PE 2
#define WWT_BUSY BIND &T_BUSY		// PI 8
#define WWMCIINS BIND &MCIINS		// PI 3
#define WWDREQ BIND &bit12			// PG 14
#define WWTFINS BIND &bit9			// PH 2
#define WWKEYIN0N BINND &KEYIN0		// PC 13
#define WWKEYIN1N BINND &KEYIN1		// PC 14
#define WWKEYIN2N BINND &KEYIN2		// PC 15
#define WWKEYIN3N BINND &KEYIN3		// PC 1
#define WWKEYIN4N BINND &KEYIN4		// PC 6
#define WWKEYIN5N BINND &KEYIN5		// PC 7
#define WWKEYIN6N BINND &KEYIN6		// PC 4
#define WWKEYIN7N BINND &KEYIN7		// PC 5
#define WWLTPKTFN BINND &bit0		// PH 5
#define WWT_PENIRQN BINND &T_PENIRQ // PE 2
#define WWT_BUSYN BINND &T_BUSY		// PI 8
#define WWMCIINSN BINND &MCIINS		// PI 3
#define WWDREQN BINND &bit12		// PG 14
#define WWTFINSN BINND &bit9		// PH 2

#if 0
#define SETMCIPWR() GPIOD->BSRRL = MCIPWR // PD 3
#define SETCANTD() GPIOB->BSRRL = CANTD	  // PB 13
#define SETI2CSCL() GPIOB->BSRRL = I2CSCL // PB 8
#define SETI2CSDA() GPIOB->BSRRL = I2CSDA // PB 9
#define SETTXD1() GPIOB->BSRRL = TXD1	  // PB 6
#define SETTXD2() GPIOA->BSRRL = TXD2	  // PA 2

#define SETSCK() GPIOA->BSRRL = SCK		// PA 5
#define SETMOSI() GPIOA->BSRRL = MOSI	// PA 7
#define SETWCS1() GPIOH->BSRRL = WCS1	// PH 4
#define SETWCS2() GPIOB->BSRRL = WCS2	// PB 0
#define SETWCS3() GPIOA->BSRRL = WCS3	// PA 15
#define SETTFCS() GPIOB->BSRRL = TFCS	// PB 1
#define SETLTCS() GPIOA->BSRRL = LTCS	// PA 4
#define SETLTRST() GPIOB->BSRRL = LTRST // PB 2

#define SETT_SCK() GPIOB->BSRRL = T_SCK	  // PB 10
#define SETT_MOSI() GPIOB->BSRRL = T_MOSI // PB 15
#define SETT_SSEL() GPIOB->BSRRL = T_SSEL // PB 11

#define SETVSCLK() GPIOB->BSRRL = VSCLK	  // PB 3
#define SETVSMOSI() GPIOB->BSRRL = VSMOSI // PB 5
#define SETVSXCS() GPIOB->BSRRL = VSXCS	  // PB 7
#define SETVSRST() GPIOG->BSRRL = VSRST	  // PG 12
#define SETVSXDCS() GPIOG->BSRRL = VSXDCS // PG 13

#define SETUSBFSEN() GPIOA->BSRRL = USBFSEN	  // PA 8
#define SETWDI() GPIOA->BSRRL = WDI			  // PA 0
#define SETNBELL() GPIOA->BSRRL = NBELL		  // PA 1
#define SETUSBLED() GPIOH->BSRRL = USBLED	  // PH 8
#define SETSDLED() GPIOE->BSRRL = SDLED		  // PE 6
#define SETLCDONOFF() GPIOI->BSRRL = LCDONOFF // PI 11
#define SETLCDPWM() GPIOF->BSRRL = LCDPWM	  // PF 9
#define SETLEDPWM() GPIOF->BSRRL = LEDPWM	  // PF 8
#define SETTEST1() GPIOH->BSRRL = TEST1		  // PH 6
#define SETTEST2() GPIOH->BSRRL = TEST2		  // PH 7

#define CLRMCIPWR() GPIOD->BSRRH = MCIPWR // PD 3
#define CLRCANTD() GPIOB->BSRRL = CANTD	  // PB 13
#define CLRI2CSCL() GPIOB->BSRRL = I2CSCL // PB 8
#define CLRI2CSDA() GPIOB->BSRRL = I2CSDA // PB 9
#define CLRTXD1() GPIOB->BSRRL = TXD1	  // PB 6
#define CLRTXD2() GPIOA->BSRRL = TXD2	  // PA 2

#define CLRSCK() GPIOA->BSRRH = SCK		// PA 5
#define CLRMOSI() GPIOA->BSRRH = MOSI	// PA 7
#define CLRWCS1() GPIOH->BSRRH = WCS1	// PH 4
#define CLRWCS2() GPIOB->BSRRH = WCS2	// PB 0
#define CLRWCS3() GPIOA->BSRRH = WCS3	// PA 15
#define CLRTFCS() GPIOB->BSRRH = TFCS	// PB 1
#define CLRLTCS() GPIOA->BSRRH = LTCS	// PA 4
#define CLRLTRST() GPIOB->BSRRH = LTRST // PB 2

#define CLRT_SCK() GPIOB->BSRRH = T_SCK	  // PB 10
#define CLRT_MOSI() GPIOB->BSRRH = T_MOSI // PB 15
#define CLRT_SSEL() GPIOB->BSRRH = T_SSEL // PB 11

#define CLRVSCLK() GPIOB->BSRRH = VSCLK	  // PB 3
#define CLRVSMOSI() GPIOB->BSRRH = VSMOSI // PB 5
#define CLRVSXCS() GPIOB->BSRRH = VSXCS	  // PB 7
#define CLRVSRST() GPIOG->BSRRH = VSRST	  // PG 12
#define CLRVSXDCS() GPIOG->BSRRH = VSXDCS // PG 13

#define CLRUSBFSEN() GPIOA->BSRRH = USBFSEN	  // PA 8
#define CLRWDI() GPIOA->BSRRH = WDI			  // PA 0
#define CLRNBELL() GPIOA->BSRRH = NBELL		  // PA 1
#define CLRUSBLED() GPIOH->BSRRH = USBLED	  // PH 8
#define CLRSDLED() GPIOE->BSRRH = SDLED		  // PE 6
#define CLRLCDONOFF() GPIOI->BSRRH = LCDONOFF // PI 11
#define CLRLCDPWM() GPIOF->BSRRH = LCDPWM	  // PF 9
#define CLRLEDPWM() GPIOF->BSRRH = LEDPWM	  // PF 8
#define CLRTEST1() GPIOH->BSRRH = TEST1		  // PH 6
#define CLRTEST2() GPIOH->BSRRH = TEST2		  // PH 7

#else //支持旧编译器

#define SETMCIPWR()            \
	GPIOD->BSRR &= 0xFFFF0000; \
	GPIOD->BSRR |= MCIPWR // PD 3
#define SETCANTD()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= CANTD // PB 13
#define SETI2CSCL()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSCL // PB 8
#define SETI2CSDA()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSDA // PB 9
#define SETTXD1()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= TXD1 // PB 6
#define SETTXD2()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= TXD2 // PA 2

#define SETSCK()               \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= SCK // PA 5
#define SETMOSI()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= MOSI // PA 7
#define SETWCS1()              \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= WCS1 // PH 4
#define SETWCS2()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= WCS2 // PB 0
#define SETWCS3()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= WCS3 // PA 15
#define SETTFCS()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= TFCS // PB 1
#define SETLTCS()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= LTCS // PA 4
#define SETLTRST()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= LTRST // PB 2

#define SETT_SCK()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= T_SCK // PB 10
#define SETT_MOSI()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= T_MOSI // PB 15
#define SETT_SSEL()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= T_SSEL // PB 11

#define SETVSCLK()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= VSCLK // PB 3
#define SETVSMOSI()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= VSMOSI // PB 5
#define SETVSXCS()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= VSXCS // PB 7
#define SETVSRST()             \
	GPIOG->BSRR &= 0xFFFF0000; \
	GPIOG->BSRR |= VSRST // PG 12
#define SETVSXDCS()            \
	GPIOG->BSRR &= 0xFFFF0000; \
	GPIOG->BSRR |= VSXDCS // PG 13

#define SETUSBFSEN()           \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= USBFSEN // PA 8
#define SETWDI()               \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= WDI // PA 0
#define SETNBELL()             \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= NBELL // PA 1
#define SETUSBLED()            \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= USBLED // PH 8
#define SETSDLED()             \
	GPIOE->BSRR &= 0xFFFF0000; \
	GPIOE->BSRR |= SDLED // PE 6
#define SETLCDONOFF()          \
	GPIOI->BSRR &= 0xFFFF0000; \
	GPIOI->BSRR |= LCDONOFF // PI 11
#define SETLCDPWM()            \
	GPIOF->BSRR &= 0xFFFF0000; \
	GPIOF->BSRR |= LCDPWM // PF 9
#define SETLEDPWM()            \
	GPIOF->BSRR &= 0xFFFF0000; \
	GPIOF->BSRR |= LEDPWM // PF 8
#define SETTEST1()             \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= TEST1 // PH 6
#define SETTEST2()             \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= TEST2 // PH 7

#define CLRMCIPWR()            \
	GPIOD->BSRR &= 0x0000FFFF; \
	GPIOD->BSRR |= MCIPWR << 16 // PD 3
#define CLRCANTD()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= CANTD // PB 13
#define CLRI2CSCL()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSCL // PB 8
#define CLRI2CSDA()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSDA // PB 9
#define CLRTXD1()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= TXD1 // PB 6
#define CLRTXD2()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= TXD2 // PA 2

#define CLRSCK()               \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= SCK << 16 // PA 5
#define CLRMOSI()              \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= MOSI << 16 // PA 7
#define CLRWCS1()              \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= WCS1 << 16 // PH 4
#define CLRWCS2()              \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= WCS2 << 16 // PB 0
#define CLRWCS3()              \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= WCS3 << 16 // PA 15
#define CLRTFCS()              \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= TFCS << 16 // PB 1
#define CLRLTCS()              \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= LTCS << 16 // PA 4
#define CLRLTRST()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= LTRST << 16 // PB 2

#define CLRT_SCK()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= T_SCK << 16 // PB 10
#define CLRT_MOSI()            \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= T_MOSI << 16 // PB 15
#define CLRT_SSEL()            \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= T_SSEL << 16 // PB 11

#define CLRVSCLK()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= VSCLK << 16 // PB 3
#define CLRVSMOSI()            \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= VSMOSI << 16 // PB 5
#define CLRVSXCS()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= VSXCS << 16 // PB 7
#define CLRVSRST()             \
	GPIOG->BSRR &= 0x0000FFFF; \
	GPIOG->BSRR |= VSRST << 16 // PG 12
#define CLRVSXDCS()            \
	GPIOG->BSRR &= 0x0000FFFF; \
	GPIOG->BSRR |= VSXDCS << 16 // PG 13

#define CLRUSBFSEN()           \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= USBFSEN << 16 // PA 8
#define CLRWDI()               \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= WDI << 16 // PA 0
#define CLRNBELL()             \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= NBELL << 16 // PA 1
#define CLRUSBLED()            \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= USBLED << 16 // PH 8
#define CLRSDLED()             \
	GPIOE->BSRR &= 0x0000FFFF; \
	GPIOE->BSRR |= SDLED << 16 // PE 6
#define CLRLCDONOFF()          \
	GPIOI->BSRR &= 0x0000FFFF; \
	GPIOI->BSRR |= LCDONOFF << 16 // PI 11
#define CLRLCDPWM()            \
	GPIOF->BSRR &= 0x0000FFFF; \
	GPIOF->BSRR |= LCDPWM << 16 // PF 9
#define CLRLEDPWM()            \
	GPIOF->BSRR &= 0x0000FFFF; \
	GPIOF->BSRR |= LEDPWM << 16 // PF 8
#define CLRTEST1()             \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= TEST1 << 16 // PH 6
#define CLRTEST2()             \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= TEST2 << 16 // PH 7

#endif

#define bit0 0x00000001
#define bit1 0x00000002
#define bit2 0x00000004
#define bit3 0x00000008
#define bit4 0x00000010
#define bit5 0x00000020
#define bit6 0x00000040
#define bit7 0x00000080
#define bit8 0x00000100
#define bit9 0x00000200
#define bit10 0x00000400
#define bit11 0x00000800
#define bit12 0x00001000
#define bit13 0x00002000
#define bit14 0x00004000
#define bit15 0x00008000
#define bit16 0x00010000
#define bit17 0x00020000
#define bit18 0x00040000
#define bit19 0x00080000
#define bit20 0x00100000
#define bit21 0x00200000
#define bit22 0x00400000
#define bit23 0x00800000
#define bit24 0x01000000
#define bit25 0x02000000
#define bit26 0x04000000
#define bit27 0x08000000
#define bit28 0x10000000
#define bit29 0x20000000
#define bit30 0x40000000
#define bit31 0x80000000

#endif
