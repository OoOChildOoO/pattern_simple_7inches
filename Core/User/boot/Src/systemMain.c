//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  system2Main.c							*
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

#include "systemMain.h"
#include "menu_p.h"
#include "menu_help.h"
#include "menu_create.h"
#include "menu_documents.h"
#include "menu_function.h"
#include "menu_modify.h"
#include "menu_program.h"
#include "menu_iotest.h"
#include "menu_convert.h"

#include "ds2460.h"
#include "utility.h"
#include "stm32cpu.h"
#include "register.h"
#include "w25qxx.h"
#include "menu.h"
#include "communication.h"

extern void Menu_Scrnsvr(U8 tt);
extern void Menu_StReadData(void);
extern void Menu_DesktopInit(void);
extern void CheckTouchPara(void);
extern void Init_SysIfo(void);
extern U8 isUpInfoData(U8 *dat);
extern void TIM4_Int_Init(u16 arr, u16 psc);

extern BOOTSCR g_bootscr; //开机boot相关信息
extern volatile U8 g_vs1003err;
extern volatile U8 g_uperr; //升级失败
extern volatile U32 mp3dver, mp3ddate;
extern volatile U8 g_disk_memory; //存储标志选择
extern volatile U8 gPeessVal;	  //按键灵敏度
extern volatile U8 g_UdZhouFlag;
extern volatile U16 g_fno;
extern volatile U16 g_Fmax;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern volatile U16 g_line_sum;
extern volatile U8 g_init_flag;
extern MachineInfo g_Mac;
extern THDTSN g_tdtensiond;
extern PRFOOT g_pfootd;
extern volatile U8 gEnverify;
extern volatile U8 g_ImgMove;
extern volatile U32 gverTim, gverTimes;
extern volatile U32 g_light_c;
extern volatile U8 g_netSelectMac;
extern volatile U8 g_jogmethod;
extern volatile U8 g_SysErr;
extern volatile U16 g_selectdisno;
extern volatile U8 g_upinfo[0x20];
extern volatile U8 g_num_mov_flag;
extern volatile U8 g_modify_code;

volatile U8 g_MachineType;
volatile EMBPINS *g_pinsInfo = (EMBPINS *)&PATCRRRD[0][0];
volatile U16 g_softUpdata[4] = {0};
extern uint8_t disktest(void);

volatile U16 g_curPTtype;
volatile U8 g_modzig = 0;

/******************防报错*begin*****************/
volatile U8 g_enctTackCode = 0; //区段自动追加重叠缝使能
volatile U8 g_needtohome = 0;	//需要手动回原点
volatile U8 g_readcovnflag;
volatile U8 g_readcovnflag1;
volatile U16 g_saveflag;
volatile S16 g_temp_PfootOffVal;
/******************防报错*end*****************/

/*-----------------------------------------------
//函数名: void Run_Bor_APP(void)
//功能  : 系统程序入口
//输入  : 	
//输出  : 
//返回值: 无
//-----------------------------------------------*/
void systemApp(void)
{
	U16 i;
	U16 flasherr = 0;				   //flash更新异常标志
	U16 mp3err = 0;					   //MP3更新异常标志
	U8 defver[3] = {0x30, 0x30, 0x30}; //更新失败时将版本号设置为0
	U8 Info[0x20] = {0};
	U8 check1 = 0, check2 = 0;
	U8 tcheck1 = 0, tcheck2 = 0;
	U16 MainBoardVer;
	U16 tmp_po = MENU_P1;

	pno = tmp_po;
	pnoindex = 0;
	EEPREG[1] = pno;

	//	g_modzig =0;
	usepageno = 1;

	if (g_bootscr.mode != 3) //开机画面等待
	{
		DelayMs(200);
#if defined(USESST39VFXX)
		DelayMs(200);
#else
		DelayMs(400);
#endif
	}

	hleppara = 0;
	g_para0 = 0;
	g_para1 = 0;
	g_imgpara = 0;
	g_data_flash = 0;
	g_disk_memory = 0;

	Menu_StReadData();
	Menu_DesktopInit();

	g_jogmethod = ReadJogmethod();
	gPeessVal = P_CTR;
	if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL) //主板保存压力值出错,读取打板器保存值
	{
		gPeessVal = ReadPessVal();
	}
	Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 7, (U16 *)(&YREG[SYS_MACHINE_TYPE])); //读系统版本,编号信息

	i = Read_D(D_SYS + SYS_UDZHOU, 1, (U16 *)(&YREG[SYS_UDZHOU]));
	if ((i) && (YREG[SYS_UDZHOU] == 0x5a)) //90
	{
		g_UdZhouFlag = 1;
	}
	else if ((i) && (YREG[SYS_UDZHOU] == 0xa5)) //165
	{
		g_UdZhouFlag = 2;
	}
	else if ((i) && (YREG[SYS_UDZHOU] == 0x55)) //85
	{
		g_UdZhouFlag = 3;
	}

	if (g_data_flash)
	{
	}
	else
	{
		g_Fmax = 1;
		g_fno = 1;

		wdt();
		g_draw_p = 0x24;  //绘图数据当前位置指针
		g_line_p = 32000; //线条数据当前位置指针
		g_pat_p = 52000;  //针数据当前位置指针
		g_init_flag = 0;
		g_data_flash = 1;
	}
	g_Mac.max_xl = -2000;
	g_Mac.max_xr = 2000;
	g_Mac.max_yu = 1000;
	g_Mac.max_yd = -1000;
	Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS])); //读取系统状态

	g_tdtensiond.base = 60;
	g_tdtensiond.val = 60;
	g_tdtensiond.max = 20;
	g_tdtensiond.min = 10;

	g_pfootd.base = 30;
	g_pfootd.val = 20;
	g_pfootd.set = 4;
	g_pfootd.max = 0;
	g_pfootd.min = 0;

	buttonflag[0] = 1;
	buttonflag[1] = 1;
	buttonflag[2] = 1;
	buttonflag[4] = 1;

	gEnverify = 0;
	g_ImgMove = 0;
	gverTim = 0;
	gverTimes = 0;

	SetLcdPwm(1, P_CNR);
	g_light_c = P_R * 1000;

	CheckTouchPara(); //触摸屏校正检测
	Init_SysIfo();

	if (g_vs1003err && P_VOI)
	{
		pno = MENU_RWHELP;
		hleppara = 147;
		poldno[0] = tmp_po;
		pnoindex = 1; /*InitScreen();ViewPage(usepageno)*/
		;
	}

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
		if (isUpInfoData(Info))
		{
			flasherr = (Info[FL_ADDR] == '0' ? 0 : 1);
			mp3err = (Info[MP_ADDR] == '0' ? 0 : 1);
		}
	}

	if (flasherr) // Flash 更新不成功
	{
		W25QXX_Write(W25Q1_DAT, (U8 *)defver, FLASHINDEXADDR + 12, 3);
		flashver = 0;
		pno = MENU_RWHELP;
		hleppara = 96;
		poldno[0] = tmp_po;
		g_uperr = 1;
		pnoindex = 1 /*InitScreen();ViewPage(usepageno)*/;
	}
	if (mp3err) // mp3 更新不成功
	{
		W25QXX_Write(W25Q2_MP3, (U8 *)defver, 10 + MP3_SADDR, 3);
		mp3dver = 0;
		pno = MENU_RWHELP;
		hleppara = 96;
		poldno[0] = tmp_po;
		g_uperr = 1;
		pnoindex = 1 /*InitScreen();ViewPage(usepageno)*/;
		g_uperr = 1;
	}

	if (flashver == 0) //flash错误
	{
		pno = MENU_RWHELP;
		hleppara = 96;
		poldno[0] = tmp_po;
		g_uperr = 1;
		pnoindex = 1;
	}

	Read_D(D_Address[SYS] + SYS_MAINBOARD_VER, 1, (U16 *)(&YREG[SYS_MAINBOARD_VER])); //读系统版本,编号信息
	MainBoardVer = YREG[SYS_MAINBOARD_VER];

	g_SysErr = ((MainBoardVer / 1000 == 2) ? 0 : 1);
	g_SysErr = 0;

	g_selectdisno = HREG[0];

	//P_R = 1 * 60;	//屏保时间一分钟
	//pno = MENU_C2_FPROG;
	//pno = MENU_G_HELP;hleppara = 147;
	while (1)
	{
		//printf("pno = %d \r\n",pno);
		EEPREG[1] = pno;
		switch (pno)
		{

		case MENU_P1: //缝纫主界面
			Menu_P1();
			break;
		case MENU_P2:
			Menu_P2(); // 基本画面二(次画面)
			break;
		case MENU_MENU:
			Menu_Menu(); // 菜单界面
			break;
		case MENU_THREAD: //穿线模式
			Menu_Thread();
			break;
		case MENU_COUNTER:
			Menu_Counter(g_para0); // 计数器
			break;
		case MENU_READ:
			Menu_Read(); // 读取花样画面
			break;
		case MENU_DIRREAD: //直接读取目录
			Menu_DirRead();
			break;
		case MENU_WRITE:
			Menu_Write(); // 保存花样画面
			break;
		case MENU_WRITE1:
			Menu_Write1(); // 保存模式
			break;
		case MENU_WIND:
			Menu_Wind(); // 卷线显示菜单
			break;
		case MENU_SKIPJOG:
			Menu_SkipJog(); // 跳跃寸动设置
			break;
		case MENU_PFOOT:
			Menu_CtPfootSet(g_para0); // 中压脚设置、修改界面
			break;
		case MENU_LINEFORCE:
			Menu_Ctlineforce(g_para0); // 面线张力设置菜单
			break;
		case MENU_HELP:
			Menu_HeSupportConnect(g_para1); //技术连线
			break;

		case MENU_RWHELP:
			if ((hleppara == 21) && (DREG[D_ERRWAR] - 0x8000 == 388))
			{
				pno = MENU_RWHELP1;
				// Menu_RwHelp1();	// 读写帮助画面
			}
			else
			{
				Menu_RwHelp(hleppara); // 读写帮助画面
			}
			break;
		case MENU_INPUT:
			Menu_CtInput(); // 花样输入画面
			break;
		case MENU_CURSOR:
			Menu_CtCursor(); // 花样生成
			break;
		case MENU_INMENU:
			Menu_CtInputMenu(); // 菜单画面
			break;
		case MENU_PITCH:
			Menu_Ctpitch(); // 针距修改
			break;
		case MENU_METHOD:
			Menu_CtMethod(); // 输入方法设置画面
			break;
		case MENU_MULTI:
			Menu_Ctmultiple(); // 多重缝设定画面
			break;
		case MENU_ZIGZAG:
			Menu_Ctzigzag(); // Z字缝设定画面
			break;
		case MENU_TACKING:
			Menu_Cttacking(); // 倒缝设置菜单
			break;
		case MENU_TACKING1:
			Menu_CtTack1(); // 倒缝设置菜单
			break;
		case MENU_CODE:
			Menu_CtCode(); // 输入辅助设置画面
			break;
		case MENU_EXCODE:
			Menu_CtExCode(); // 辅助功能扩充画面1
			break;
		case MENU_INHELP:
			Menu_CtInHelp(hleppara); // 输入帮助菜单
			break;
		case MENU_CALLFILE:
			Menu_CtCallFile(); // 打板调用花样
			break;
		case MENU_DIRCALLFILE:
			Menu_DirCallFile();
			break;

		case MENU_MODIFY:
			Menu_Modify(); // 花样修改画面
			break;
		case MENU_MODIFYADD:
			Menu_ModiAdd(g_para1); // 寸动操作画面
			break;
		case MENU_MODIFY1:
			if (g_para1 == 5 && g_num_mov_flag == 2)
			{
				Menu_ModifyPatChangeMethod(g_para1);
			}
			else
			{
				Menu_Modify1(g_para1); // 缝目修正
			}
			break;
		case MENU_MODIFY2:
			Menu_Modify2(0); // 速度指令修正
			break;
		case MENU_MODIFY3:
			Menu_Modify3(g_modify_code); // 辅加功能修正
			break;
		case MENU_MOIMAGE: // 修改模组中的图案画面

			if (g_imgpara == MOIMAGE_CVIEW || g_imgpara == MOIMAGE_ENSURE1 || g_imgpara == MOIMAGE_ENSURE2)
			{
				Menu_MoView2(g_imgpara);
			}
			else
			{
				Menu_MoView(g_imgpara);
			}
			break;
		case MENU_CONVERT:
			Menu_Convert(); // 花样变换画面
			break;
		case MENU_CONVERT1:
			Menu_Convert1(g_para1); // 花样变换菜单
			break;

		case MENU_PROG: //程式模组
			Menu_Prog();
			break;
		case MENU_PROFRAME: //程式模组二级菜单
			Menu_ProFrame();
			break;
		case MENU_PROPARA: //程式模组参数输入
			Menu_ProPara();
			break;
		case MENU_DIFFERENT: //程式模组参数输入
			Menu_ProDifferent();
			break;
		case MENU_FPROG: //快速程式模组参数输入
			Menu_FastProg();
			break;

		case MENU_IOTEST:
			Menu_Iotest(); // IO测试画面
			break;
		case MENU_INPUTSTATUS:
			Menu_Inputstatus(); // 输入信号状态显示
			break;
		case MENU_OUTPUTSTATUS:
			Menu_Outputstatus(); // 输出信号状态显示
			break;
		case MENU_SENSORSTATUS:
			Menu_Sensorstatus(); // 检测器信号状态显示
			break;
		case MENU_INPUTSET:
			Menu_Inputset(); // 输入信号设置
			break;
		case MENU_OUTPUTSET:
			Menu_Outputset(); // 输出信号设置
			break;
		case MENU_ANALOGSET: //模拟量输入输出设定
			Menu_AnalogSet();
			break;

		case MENU_FUNCTION:
			Menu_Function(); // 功能画面
			break;
		case MENU_FTCOPY:
			Menu_FtCopy(); // 图形拷贝
			break;
			//   case MENU_FTNEEDLE:
			// 	Menu_FtNeedle();		// 下停针设置
			// 	break;
		case MENU_FTFORMAT:
			Menu_FtFormat(); // 格式化
			break;
		case MENU_FTGROUP:
			Menu_FtGroup(); // 图形组合
			break;
		case MENU_FTGROUPREAD1: //图形组合读内存花样
			Menu_FtReadMem();
			break;
		case MENU_FTDIRREAD: //图形组合读内存花样-直接读取花样
			Menu_FtDirReadMem();
			break;
		case MENU_FTGROUPREAD2: //图形组合读组合花样
			Menu_FtReadCombine();
			break;
		case MENU_FTGROUPWRITE: //图形组合保存组合花样
			Menu_FtWriteCombine();
			break;
		case MENU_FTSYSVERSION:
			Menu_FtSysversion(); // 版本
			break;
		case MENU_FTSCREENSET:
			Menu_FtScreenset(); // 显示设置
			break;
		case MENU_FTSCREENBOOTSET:
			Menu_FtBootScreen();
			break;
		case MENU_FTSYSRESTORE:
			Menu_FtSysrestore(); // 系统还原升级
			break;
		case MENU_FTSYSBACKUP:
			Menu_FtSysbackup(); // 系统备份
			break;
		case MENU_FTUSER:
			Menu_FtUser(); // 用户登录
			break;
			//   case MENU_FTERRRECORD:
			// 	Menu_FtErrrecord();		// 错误信息记录
			// 	break;
			//   case MENU_FTRUNRECORD:
			// 	Menu_FtRunrecord();		// 运行信息记录
			// 	break;
			//   case MENU_FTNETWORD:
			// 	Menu_FtNetword();		// 网络工作组
			// 	break;
			//   case MENU_FTCHAT:
			// 	Menu_FtNewChat();		//网络聊天
			// 	break;
			//   case	MENU_FTSETIP:		//设置机器IP、名称
			// 	Menu_FtSetIP(g_para1);
			// 	break;
			//   case MENU_FTNETRW:		//发送读取远程文件
			// 	Menu_FtNetwordRW(g_para1,g_netSelectMac);
			// 	break;
			//   case MENU_FTRESET:
			// 	Menu_FtReset();			// 一键还原
			// 	break;
			// case MENU_PARAMODIFY:       //参数修正画面
			// 	Menu_FtParaCorr();
			// 	break;
			//   case MENU_FTSERVO:		//伺服参数
			// 	 Menu_FtServo();
			// 	 break;
		default:
			pno = MENU_P1;
			break;
		}
		wdt();
		keyolddata = 0;
	}
}
