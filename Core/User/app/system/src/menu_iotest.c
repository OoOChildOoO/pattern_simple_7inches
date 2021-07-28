/*
 * @Author: your name
 * @Date: 2021-06-16 11:03:16
 * @LastEditTime: 2021-07-23 09:27:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\src\menu_iotest.c
 */
//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  menu_iotest.c							*
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
//#ifdef SYSTEM0
#include "menu_iotest.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "uart.h"
#include "main.h"
#include "color.h"
#include "communication.h"
#include "function.h"

#define TLFNADD_IOALL 0
#define TLFNLEN_IOALL 132 //1
#define TLFNADD_ANALOGSET (TLFNADD_IOALL + TLFNLEN_IOALL)
#define TLFNLEN_ANALOGSET 6 //2

#define TLFNADD_CONTENT 0
#define TLFNLEN_CONTENT 21 //1

#define INPUT1 0x00000080
#define INPUT2 0x00000100
#define INPUT3 0x00001000
#define INPUT4 0x00000004
#define INPUT5 0x00000008
#define INPUT6 0x00000040
#define INPUT7 0x00000010
#define INPUT8 0x00000020
#define INPUT9 0x00002000
#define INPUTA 0x00000002
#define INPUTB 0x00000001
#define INPUTC 0x00000000
#define INPUT_XYAL 0x00000400
#define INPUT_UVAL 0x00000200
#define INPUT_MALM 0x00004000

#define BCHA 0x00100000		 //主轴-A信号
#define BCHB 0x00200000		 //主轴-B信号
#define BCHZ 0x00400000		 //主轴-Z信号
#define BYOG 0x20000000		 //Y轴-步进
#define BXOG 0x80000000		 //X轴-步进
#define BWOG 0x00800000		 //主轴-W信号
#define BVOG 0x01000000		 //主轴-V信号
#define BMOG 0x08000000		 //主轴-伺服
#define BPFOG 0x10000000	 //压脚-步进
#define NEEDLE_UP 0x02000000 //上针位-伺服
#define NEEDLE_DW 0x00000000 //下针位-伺服

//扩展输入
#define EXINPUT1 0x0001
#define EXINPUT2 0x0002
#define EXINPUT3 0x0004
#define EXINPUT4 0x0008
#define EXINPUT5 0x0010
#define EXINPUT6 0x0020
#define EXINPUT7 0x0040
#define EXINPUT8 0x0080
#define EXINPUT9 0x0100
#define EXINPUTA 0x0200
#define EXINPUTB 0x0400
#define EXINPUTC 0x0800

#define OUTPUT1 0x00000008
#define OUTPUT2 0x00000080
#define OUTPUT3 0x00000040
#define OUTPUT4 0x00000004
#define OUTPUT5 0x00000001
#define OUTPUT6 0x00000002
#define OUTPUT7 0x00000400
#define OUTPUT8 0x00000200
#define OUTPUT9 0x00000100
#define OUTPUTA 0x00000020
#define OUTPUTB 0x00000010
#define OUTPUTC 0x00000800

//扩展输出
#define EXOUTPUT1 0x0001
#define EXOUTPUT2 0x0002
#define EXOUTPUT3 0x0004
#define EXOUTPUT4 0x0008
#define EXOUTPUT5 0x0010
#define EXOUTPUT6 0x0020
#define EXOUTPUT7 0x0040
#define EXOUTPUT8 0x0080
#define EXOUTPUT9 0x0100
#define EXOUTPUTA 0x0200
#define EXOUTPUTB 0x0400
#define EXOUTPUTC 0x0800

#define OUTPUT_H 1
#define OUTPUT_L 0

#define XYTESTON 1
#define XYTESTOFF 0

#define XLEFT 1
#define XRIGHT 2
#define YUP 3
#define YDOWN 4
#define PFUP 5
#define PFDOWN 6
#define NEEDLEL 7
#define NEEDLER 8
#define SCL 9
#define SCR 10
U32 inputdat[] = {INPUT1, INPUT2, INPUT3, INPUT4, INPUT5, INPUT6, INPUT7, INPUT8, INPUT9, INPUTA, INPUTB, INPUTC, INPUT_XYAL, INPUT_UVAL, INPUT_MALM};
U16 exinputdat[] = {EXINPUT1, EXINPUT2, EXINPUT3, EXINPUT4, EXINPUT5, EXINPUT6, EXINPUT7, EXINPUT8, EXINPUT9, EXINPUTA, EXINPUTB, EXINPUTC};
U32 outputdat[] = {OUTPUT1, OUTPUT2, OUTPUT3, OUTPUT4, OUTPUT5, OUTPUT6, OUTPUT7, OUTPUT8, OUTPUT9, OUTPUTA, OUTPUTB, OUTPUTC};
U16 exoutputdat[] = {EXOUTPUT1, EXOUTPUT2, EXOUTPUT3, EXOUTPUT4, EXOUTPUT5, EXOUTPUT6, EXOUTPUT7, EXOUTPUT8, EXOUTPUT9, EXOUTPUTA, EXOUTPUTB, EXOUTPUTC};
U32 motordat[] = {NEEDLE_UP, NEEDLE_DW, BWOG, BVOG, BXOG, BYOG, BPFOG, BMOG};

extern U16 SheetPage; //多国语言Sheet表页码，从0开始
extern const U8 title_IotestP[][10];
extern const U16 PARA_INSET[];
extern const U16 PARA_OUTSET[];

volatile U32 g_OutputPointStatus = 0; //输出点状态,0:相应位无输出; 1:相应位有输出
volatile U32 g_OutputPointStatusOld;
volatile U32 g_InputPointStatus = 0;	//输入点状态,0:相应位无输入; 1:相应位有输入
volatile U16 g_ExInputPointStatus = 0;	//扩展输入点状态,0:相应位无输入; 1:相应位有输入
volatile U32 g_ExOutputPointStatus = 0; //扩展输出点状态,0:相应位无输出; 1:相应位有输出
volatile U32 g_ExOutputPointStatusOld;
volatile U16 g_CheckAngle = 0;	 //编码器角度
volatile U8 g_runstep, g_slcnum; //g_runstep:输入/输出设置界面内的分页步骤
extern volatile U8 g_NO_ReagIOf; //不读取IO参数标志

//-----------------------------------------------------------------------

//显示IO名称
void disIOname(U16 x, U16 y, U16 *p, U8 len, U16 sheet, U16 line, U16 color)
{
	if (P_LAN < 2)
	{
		Draw24X24FFStrN(x, y, p, len, color);
	}
	else
	{
		DrawUnicodeMulStr(x, y, sheet, line, len + 1, 1, color);
	}
}

//------------------------------------------
// Function   : void DispInput(U8 a, U8 NPoint)
// Description: 输入信号显示更新
//------------------------------------------
void DispInput(U8 a, U8 NPoint)
{
	U8 i, j;

	i = NPoint % 9;
	j = NPoint / 9;
	BmpFillRect(55 + i * 64, 96 + j * 120, 32, 28);
	if (a)
	{ //有输入
		DrawFBMP2(55 + i * 64, 96 + j * 120, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{ //无输入
		DrawFBMP2(55 + i * 64, 96 + j * 120, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
}

//------------------------------------------
// Function   : void DispInputStatus(void)
// Description: 输入信号显示
//------------------------------------------
void DispInputStatus(void)
{
	U8 i;

	for (i = 0; i < 15; i++)
	{
		DispInput((g_InputPointStatus & inputdat[i]) ? 1 : 0, i > 11 ? (i + 12) : i);
	}

	for (i = 0; i < 12; i++) //扩展输入
	{
		DispInput((g_ExInputPointStatus & exinputdat[i]) ? 1 : 0, i + 12);
	}
}

//------------------------------------------
// Function   : void DispInStatus(void)
// Description: 输出输入信号显示
//------------------------------------------
void DispInStatus(const RECTPAD *bn, U8 length, U32 stateList, U32 *bitList)
{
	U8 i, state;
	char *stateStr[2] = {"OFF", "ON"};
	for (i = 0; i < length; i++)
	{
		state = ((stateList & bitList[i]) ? 1 : 0);

		DrawFillRect(bn[i].xpis, bn[i].ypis, bn[i].xsize, bn[i].ysize, bn[i].colorB[state]);
		Draw24X24FStr(bn[i].xpis + bn[i].xsize / 2 - 16, bn[i].ypis + bn[i].ysize / 2 - 12, stateStr[state], bn[i].colorT[0]);
	}
}

//------------------------------------------
// Function   : void DispOutput(U8 a, U8 NPoint)
// Description: 输出信号显示更新
//------------------------------------------
void DispOutput(U8 a, U8 NPoint)
{
	const KEYPAD keyd[] = {{32, 82, KEY_X1, KEY_Y1}, {96, 82, KEY_X1, KEY_Y1}, {160, 82, KEY_X1, KEY_Y1}, {224, 82, KEY_X1, KEY_Y1}, {288, 82, KEY_X1, KEY_Y1}, {352, 82, KEY_X1, KEY_Y1}, {416, 82, KEY_X1, KEY_Y1}, {480, 82, KEY_X1, KEY_Y1}, {544, 82, KEY_X1, KEY_Y1}, {32, 202, KEY_X1, KEY_Y1}, {96, 202, KEY_X1, KEY_Y1}, {160, 202, KEY_X1, KEY_Y1}, {224, 202, KEY_X1, KEY_Y1}, {288, 202, KEY_X1, KEY_Y1}, {352, 202, KEY_X1, KEY_Y1}, {416, 202, KEY_X1, KEY_Y1}, {480, 202, KEY_X1, KEY_Y1}, {544, 202, KEY_X1, KEY_Y1}, {32, 322, KEY_X1, KEY_Y1}, {96, 322, KEY_X1, KEY_Y1}, {160, 322, KEY_X1, KEY_Y1}, {224, 322, KEY_X1, KEY_Y1}, {288, 322, KEY_X1, KEY_Y1}, {352, 322, KEY_X1, KEY_Y1}};

	if (a == 1)
	{
		DrawButton(keyd[NPoint], (U16 *)(ADD_ICONBMP), KEY_Blue, 24);
		//DrawFBMP2(36+(NPoint%9)*64,86+(NPoint/9)*120,56,56,(MEM_BMPIO_STATUS+472),Color_White);
		//DrawFBMP2(36+(NPoint%9)*64,86+(NPoint/9)*120,56,56,(MEM_BMPIO_STATUS+668),Color_Red);
		//}else if(a==2){
		//	DrawButton(keyd[NPoint],(U16 *)(ADD_ICONBMP),KEY_Silver,Color_Gray);
		//	DrawFBMP2(36+(NPoint%9)*64,86+(NPoint/9)*120,56,56,(MEM_BMPIO_STATUS+472),Color_Blue);
	}
	else
	{ //反白显示
		DrawButton(keyd[NPoint], (U16 *)(ADD_ICONBMP), KEY_Silver, 23);
		//DrawFBMP2(36+(NPoint%9)*64,86+(NPoint/9)*120,56,56,(MEM_BMPIO_STATUS+472),Color_Blue);
	}
}

#if 0
//------------------------------------------
// Function   : void DispOutputStatus(U8 t)
// Description: 输出信号显示
//------------------------------------------
void DispOutputStatus(U8 t)
{
	U8 i;
	U32 dat;
	U16 dat1;

	if (t)
		dat = 0xffffffff;
	else
		dat = g_OutputPointStatusOld ^ g_OutputPointStatus;
	for (i = 0; i < 12; i++)
	{
		if (dat & outputdat[i])
			DispOutput((g_OutputPointStatus & outputdat[i]) ? 1 : 0, i);
	}

	if (t)
		dat1 = 0xffff;
	else
		dat1 = g_ExOutputPointStatusOld ^ g_ExOutputPointStatus;
	for (i = 0; i < 12; i++)
	{ //扩展输出
		if (dat1 & exoutputdat[i])
			DispOutput((g_ExOutputPointStatus & exoutputdat[i]) ? 1 : 0, i + 12);
	}
}
#endif

//------------------------------------------
// Function   : void DispOutStatus(void)
// Description: 输出输入信号显示
//------------------------------------------
void DispOutStatus(const RECTPAD *bn, U8 length, U32 stateList, U32 *bitList)
{
	U8 i, state;
	char *stateStr[2] = {"OFF", "ON"};
	for (i = 0; i < length; i++)
	{
		state = ((stateList & bitList[i]) ? 1 : 0);

		DrawFillRect(bn[i].xpis, bn[i].ypis, bn[i].xsize, bn[i].ysize, bn[i].colorB[state]);
		Draw24X24FStr(bn[i].xpis + bn[i].xsize / 2 - 16, bn[i].ypis + bn[i].ysize / 2 - 12, stateStr[state], bn[i].colorT[0]);
	}
}

#if 0
//------------------------------------------
// Function   : void DispInput2Status(void)
// Description: 更新输入状态显示
//------------------------------------------
void DispInput2Status(void)
{

	BmpFillRect(28 + 23, 188 + 12, 32, 28); // 上停针
	if ((g_InputPointStatus & NEEDLE_UP))
	{
		//printf("g_InputPointStatus = %x ",g_InputPointStatus);
		DrawFBMP2(28 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(28 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
	BmpFillRect(148 + 23, 188 + 12, 32, 28); // 下停针
	if ((g_InputPointStatus & NEEDLE_DW))
	{
		DrawFBMP2(148 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(148 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
	BmpFillRect(28 + 23, 270 + 12, 32, 28); // V检测器
	if ((g_InputPointStatus & BVOG))
	{
		DrawFBMP2(28 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(28 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
	BmpFillRect(148 + 23, 270 + 12, 32, 28); // W检测器
	if ((g_InputPointStatus & BWOG))
	{
		DrawFBMP2(148 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(148 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}

	BmpFillRect(404 + 23, 188 + 12, 32, 28); // X检测器
	if ((g_InputPointStatus & BXOG))
	{
		DrawFBMP2(404 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(404 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
	BmpFillRect(540 + 23, 188 + 12, 32, 28); // Y检测器
	if ((g_InputPointStatus & BYOG))
	{
		DrawFBMP2(540 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(540 + 23, 188 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
	BmpFillRect(404 + 23, 270 + 12, 32, 28); // PF检测器
	if ((g_InputPointStatus & BPFOG))
	{
		DrawFBMP2(404 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(404 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}
	BmpFillRect(540 + 23, 270 + 12, 32, 28); // M检测器
	if ((g_InputPointStatus & BMOG))
	{
		DrawFBMP2(540 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 416), Color_Red);
	}
	else
	{
		DrawFBMP2(540 + 23, 270 + 12, 32, 28, (MEM_BMPIO_STATUS + 360), Color_Red);
	}

	if ((g_InputPointStatus & BCHA))
	{ // A盘
		DrawFPBMPNEW(240, 292, 128, 128, MEM_BMPGIF, 73);
	}
	else if ((g_InputPointStatus & BCHB))
	{ // B盘
		DrawFPBMPNEW(240, 292, 128, 128, MEM_BMPGIF, 74);
	}
}
#endif
//------------------------------------------
// Function   : void DispMotorStatus(const RECTPAD *bn, U8 length, U32 stateList, U32 *bitList)
// Description: 更新电机状态
//------------------------------------------
void DispMotorStatus(const RECTPAD *bn, U8 length, U32 stateList, U32 *bitList)
{
	U8 i, state;
	char *stateStr[2] = {"OFF", "ON"};
	for (i = 0; i < length; i++)
	{
		state = ((stateList & bitList[i]) ? 1 : 0);

		DrawFillRect(bn[i].xpis, bn[i].ypis, bn[i].xsize, bn[i].ysize, bn[i].colorB[state]);
		Draw24X24FStr(bn[i].xpis + bn[i].xsize / 2 - 16, bn[i].ypis + bn[i].ysize / 2 - 12, stateStr[state], bn[i].colorT[0]);
	}
}



//------------------------------------------------------------
#define KEY_IOTEST_SCRNSVR 1
#define KEY_IOTEST_BACK 2		  //返回
#define KEY_IOTEST_INPUTSTATUS 3  //输入信号检测
#define KEY_IOTEST_OUTPUTSTATUS 4 //输出信号检测
#define KEY_IOTEST_SENSORSTATUS 5 //电机检测
// #define KEY_IOTEST_ANALOGSET 6	  //模拟量检测
#define KEY_IOTEST_MAX 6
// #define KEY_IOTEST_INPUTSET 6
// #define KEY_IOTEST_OUTPUTSET 7
#define RECT_IOTEST_INPUT 1
#define RECT_IOTEST_OUTPUT 2
#define RECT_IOTEST_SENSOR 3
// #define RECT_IOTEST_ANA 4

//------------------------------------------
// Function   : void Menu_Iotest(void)
// Description: 测试画面
//------------------------------------------
void Menu_Iotest(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U16 tmpPosX = 0;
	// U16 tmpPosY = 0;
	const U8 keynum = {KEY_IOTEST_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},												//1	屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},								//2 返回
		{122, TYPE_Y7, KEY_X10, KEY_Y10},								//3	输入信号检测
		{122 + KEY_X10 + SAFE_DIST13, TYPE_Y7, KEY_X10, KEY_Y10},		//4	输出信号检测
		{122 + (KEY_X10 + SAFE_DIST13) * 2, TYPE_Y7, KEY_X10, KEY_Y10}, //5	电机检测
																		// {122, TYPE_Y7 + KEY_Y10 + SAFE_DIST14, KEY_X10, KEY_Y10},		 //6	模拟量检测
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},				   //1 	屏保
		{{KEY_Silver}, {0}, {241}, {TLFNADD_IOALL + 2, 0, 0, 0}, {0}}, //2	返回
		{{KEY_Silver}, {0}, {851}, {TLFNADD_IOALL + 3, 0, 0, 0}, {0}}, //3	输入信号检测
		{{KEY_Silver}, {0}, {852}, {TLFNADD_IOALL + 4, 0, 0, 0}, {0}}, //4	输出信号检测
		{{KEY_Silver}, {0}, {853}, {TLFNADD_IOALL + 5, 0, 0, 0}, {0}}, //5	电机检测
																	   // {{KEY_Silver}, {0}, {854}, {131, 0, 0, 0}, {0}}, //6	模拟量检测

	};

	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_IOALL}},
		{122, TYPE_Y7 + KEY_Y10, KEY_X10, TYPE_SIZE5, {Color_White}, {Color_Black}, {TLFNADD_IOALL + 3}},
		{122 + KEY_X10 + SAFE_DIST13, TYPE_Y7 + KEY_Y10, KEY_X10, TYPE_SIZE5, {Color_White}, {Color_Black}, {TLFNADD_IOALL + 4}},
		{122 + (KEY_X10 + SAFE_DIST13) * 2, TYPE_Y7 + KEY_Y10, KEY_X10, TYPE_SIZE5, {Color_White}, {Color_Black}, {TLFNADD_IOALL + 5}},
		// {122, TYPE_Y7 + KEY_Y10 + SAFE_DIST14 + KEY_Y10, KEY_X10, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
	};

	SetLanguage(MENU_IOTEST);
	InitScreen();
	initViewStatus();
	updateViewStatus();

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	//按钮绘制
	for (i = KEY_IOTEST_BACK; i < keynum; i++)
	{
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}
	for (i = RECT_IOTEST_INPUT; i <= RECT_IOTEST_SENSOR; i++)
	{
		tmpPosX = rect[i].xpis + rect[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax)) * 8 / 2;
		Draw16x16Str(tmpPosX, rect[i].ypis + SAFE_DIST3,
					 MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax, SheetPage, rect[i].index[0], rect[i].colorT[0]);
	}

	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//键值为按键松开时
		if (keydata == 0xff)
		{
			if ((keyon) && (beeponf))
			{
				keyoff = keyon; //按键键值传给松开键值
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理
		{
			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_IOTEST_SCRNSVR)
				{
				}
				else
				{
					DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]); //常规按键特效
				}

				Times = 0;
				delay = KEYDT; //按键防抖次数
			}
			else if (keyon != keydata) //已有按键按下，不处理新按键信息
			{
				keydata = 0;
			}
			if (Times > delay) //按键防抖
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);
					switch (keyon) //按键按下标题提示语显示
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
			}
		}
		else
		{
			key_on_flag1 = 0;
		}
		if (keyoff) //按键松开处理
		{
			switch (keyoff)
			{
			case KEY_IOTEST_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_IOTEST_BACK:
				pno = poldno[0];
				pnoindex = 0;
				break;
			case KEY_IOTEST_INPUTSTATUS:
				pno = MENU_INPUTSTATUS;
				poldno[pnoindex++] = MENU_IOTEST;
				break;
			case KEY_IOTEST_OUTPUTSTATUS:
				pno = MENU_OUTPUTSTATUS;
				poldno[pnoindex++] = MENU_IOTEST;
				break;
			case KEY_IOTEST_SENSORSTATUS:
				pno = MENU_SENSORSTATUS;
				poldno[pnoindex++] = MENU_IOTEST;
				break;
#if 0	
			case KEY_IOTEST_ANALOGSET:
				pno = MENU_ANALOGSET;
				poldno[pnoindex++] = MENU_IOTEST;
				break;
			

			case KEY_IOTEST_OUTPUTSET:
				g_NO_ReagIOf = 0;
				if ((g_user.newuser > 5) && (g_user.newuser < 10))
				{
					pno = MENU_OUTPUTSET;
				}
				else
				{
					poldno[2] = MENU_OUTPUTSET;
					pno = MENU_FTUSER;
				}
				poldno[1] = MENU_IOTEST;
				pnoindex = 2;
				g_runstep = 0;
				break;
#endif
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_IOTEST_SCRNSVR) //不需要显示的按键
			{
			}
			//非常规按键-//绘制颜色文字按键
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
		}

		if (pno != MENU_IOTEST)
		{
			plastno = MENU_IOTEST;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//--------------------------------------------------
#define KEY_INPTSTS_SCRNSVR 1
#define KEY_INPTSTS_BACK 2
#define KEY_INPTSTS_IO 3
#define KEY_INPTSTS_SELC1 4
#define KEY_INPTSTS_SELC2 5
#define KEY_INPTSTS_SELC3 6
#define KEY_INPTSTS_SELC4 7
#define KEY_INPTSTS_SELC5 8
#define KEY_INPTSTS_SELC6 9
#define KEY_INPTSTS_SELC7 10
#define KEY_INPTSTS_SELC8 11
#define KEY_INPTSTS_SELC9 12
#define KEY_INPTSTS_SELC10 13
#define KEY_INPTSTS_SELC11 14
#define KEY_INPTSTS_SELC12 15
#define KEY_INPTSTS_SELCE1 16
#define KEY_INPTSTS_SELCE2 17
#define KEY_INPTSTS_SELCE3 18
#define KEY_INPTSTS_SELCE4 19
#define KEY_INPTSTS_MAX 4

#define RECT_INPTSTS_SEL1 1
#define RECT_INPTSTS_SEL2 2
#define RECT_INPTSTS_SEL3 3
#define RECT_INPTSTS_SEL4 4
#define RECT_INPTSTS_SEL5 5
#define RECT_INPTSTS_SEL6 6
#define RECT_INPTSTS_SEL7 7
#define RECT_INPTSTS_SEL8 8
#define RECT_INPTSTS_SEL9 9
#define RECT_INPTSTS_SEL10 10
#define RECT_INPTSTS_SEL11 11
#define RECT_INPTSTS_SEL12 12
#define RECT_INPTSTS_SELE1 13
#define RECT_INPTSTS_SELE2 14
#define RECT_INPTSTS_SELE3 15
#define RECT_INPTSTS_SELE4 16

#define RECT_INPTSTS_STA1 17
#define RECT_INPTSTS_STA2 18
#define RECT_INPTSTS_STA3 19
#define RECT_INPTSTS_STA4 20
#define RECT_INPTSTS_STA5 21
#define RECT_INPTSTS_STA6 22
#define RECT_INPTSTS_STA7 23
#define RECT_INPTSTS_STA8 24
#define RECT_INPTSTS_STA9 25
#define RECT_INPTSTS_STA10 26
#define RECT_INPTSTS_STA11 27
#define RECT_INPTSTS_STA12 28
#define RECT_INPTSTS_STAE1 29
#define RECT_INPTSTS_STAE2 30
#define RECT_INPTSTS_STAE3 31
#define RECT_INPTSTS_STAE4 32

#define RECT_INPTSTS_STR1 33
#define RECT_INPTSTS_STR2 34
#define RECT_INPTSTS_STR3 35
#define RECT_INPTSTS_STR4 36
#define RECT_INPTSTS_STR5 37
#define RECT_INPTSTS_STR6 38
#define RECT_INPTSTS_STR7 39
#define RECT_INPTSTS_STR8 40
#define RECT_INPTSTS_STR9 41
#define RECT_INPTSTS_STR10 42
#define RECT_INPTSTS_STR11 43
#define RECT_INPTSTS_STR12 44
#define RECT_INPTSTS_STRE1 45
#define RECT_INPTSTS_STRE2 46
#define RECT_INPTSTS_STRE3 47
#define RECT_INPTSTS_STRE4 48

//------------------------------------------
// Function   : void Menu_Inputstatus(void)
// Description: 输入信号状态显示
//------------------------------------------
void Menu_Inputstatus(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U8 f;
	U16 Iosetnamepos;
	U32 InputPointStatuOld = 0;
	U16 ExInputPointStatuOld = 0;
	//const char Alarm[][8] = {"XYAL", "UVAL", "MALM"};
	const U8 keynum = {KEY_INPTSTS_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},													 //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},									 //2 返回
		{TYPE_X10, TYPE_Y4, KEY_X12, KEY_Y12},								 //3 可编程IO设置
		{TYPE_X35, TYPE_Y3, KEY_X11, KEY_Y11},								 //4 选项1
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //5 选项2
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //6 选项3
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //7 选项4
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //8 选项5
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //9 选项6
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //10 选项7
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //11 选项8
		{TYPE_X53, TYPE_Y3, KEY_X11, KEY_Y11},								 //12 选项9
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //13 选项10
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //14 选项11
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //15 选项12
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //16 选项13
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //17 选项14
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //18 选项15
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //19 选项16

	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {TLFNADD_IOALL + 1, 0, 0, 0}, {0}},										  //1 	屏保
		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 2, 0, 0, 0}, {0}},										  //2	返回
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_IOALL + 131, 0, 0, 0}, {TLFNADD_IOALL + 131}},	  //3	可编程IO设置
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 53, 0, 0, 0}, {TLFNADD_IOALL + 53}},	  //4	选项1
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 54, 0, 0, 0}, {TLFNADD_IOALL + 54}},	  //5	选项2
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 55, 0, 0, 0}, {TLFNADD_IOALL + 55}},	  //6	选项3
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 56, 0, 0, 0}, {TLFNADD_IOALL + 56}},	  //7	选项4
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 57, 0, 0, 0}, {TLFNADD_IOALL + 57}},	  //8	选项5
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 58, 0, 0, 0}, {TLFNADD_IOALL + 58}},	  //9	选项6
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 59, 0, 0, 0}, {TLFNADD_IOALL + 59}},	  //10	选项7
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 60, 0, 0, 0}, {TLFNADD_IOALL + 60}},	  //11	选项8
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 61, 0, 0, 0}, {TLFNADD_IOALL + 61}},	  //12	选项9
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 62, 0, 0, 0}, {TLFNADD_IOALL + 62}},	  //13	选项10
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 63, 0, 0, 0}, {TLFNADD_IOALL + 63}},	  //14	选项11
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 64, 0, 0, 0}, {TLFNADD_IOALL + 64}},	  //15	选项12
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 106, 0, 0, 0}, {TLFNADD_IOALL + 106}}, //16	选项13
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 107, 0, 0, 0}, {TLFNADD_IOALL + 107}}, //17	选项14
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 108, 0, 0, 0}, {TLFNADD_IOALL + 108}}, //18	选项15
		{{KEY_Silver}, {Color_Black}, {Color_Silver}, {TLFNADD_IOALL + 109, 0, 0, 0}, {TLFNADD_IOALL + 109}}, //19	选项16

	};

	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_IOALL + 32}},

		{TYPE_X35, TYPE_Y3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 53}},								   //1 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 54}},  //2 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 55}},  //3 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 56}},  //4 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 57}},  //5 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 58}},  //6 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 59}},  //7 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 60}},  //8 IO号显示框
		{TYPE_X53, TYPE_Y3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 61}},								   //9 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 62}},  //10 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 63}},  //11 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 64}},  //12 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 106}}, //13 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 107}}, //14 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 108}}, //15 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 109}}, //16 IO号显示框

		{TYPE_X35 + 284, TYPE_Y3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},								  //1 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //2 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //3 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //4 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //5 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //6 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //7 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //8 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},								  //9 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //10 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //11 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //12 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //13 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //14 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //15 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //16 IO号显示框

		{TYPE_X35 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //1 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //2 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //3 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //4 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //5 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //6 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //7 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //8 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //9 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //10 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //11 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //12 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //13 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //14 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //15 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //16 IO号显示框
	};

	if (P_LAN)
	{
		Iosetnamepos = IOSETNAME_ENGLISH;
	}
	else
	{
		Iosetnamepos = IOSETNAME_CHINA;
	}
	SetLanguage(MENU_IOTEST);
	InitScreen();
	initViewStatus();
	updateViewStatus();

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	DrawButton(keyd[KEY_INPTSTS_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_INPTSTS_BACK].border[0], keybmpind[KEY_INPTSTS_BACK].icon[0]);
	DrawButtonSingleColor(keyd[KEY_INPTSTS_IO], keybmpind[KEY_INPTSTS_IO].border[0], keybmpind[KEY_INPTSTS_IO].icon[0]);
	Draw24x24Str(keyd[KEY_INPTSTS_IO].xpis + keyd[KEY_INPTSTS_IO].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + keybmpind[KEY_INPTSTS_IO].nameStr[0] * TitleLengthMax)) * 12 / 2,
				 keyd[KEY_INPTSTS_IO].ypis + (keyd[KEY_INPTSTS_IO].ysize - 24) / 2,
				 (MEM_TITLE_ADD + keybmpind[KEY_INPTSTS_IO].nameStr[0] * TitleLengthMax), SheetPage, keybmpind[KEY_INPTSTS_IO].nameStr[0], keybmpind[KEY_INPTSTS_IO].textColor[0]);

	for (i = RECT_INPTSTS_SEL1; i <= RECT_INPTSTS_SELE4; i++)
	{
		DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		Draw24x24Str(rect[i].xpis + rect[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax)) * 12 / 2,
					 rect[i].ypis + (rect[i].ysize - 24) / 2,
					 (MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax), SheetPage, rect[i].index[0], rect[i].colorT[0]);
	}

	for (i = RECT_INPTSTS_STR1; i <= RECT_INPTSTS_STRE4; i++)
	{
		DrawLine(rect[i].xpis, rect[i].ypis + rect[i].ysize, rect[i].xpis + rect[i].xsize, rect[i].ypis + rect[i].ysize, rect[i].colorB[0]);
	}

	//IO参数和参数说明
	Read_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG)); //读取IO参数
	if ((IOREG[0] != 0x5aa5) && (IOREG[0] != 0x5a5a))	   //通信故障
	{
		pno = MENU_RWHELP;
		hleppara = 15;
	}
	for (i = RECT_INPTSTS_STR1; i <= RECT_INPTSTS_STRE4; i++)
	{
		DrawFFStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST7,
				  (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_INPTSTS_STR1) * INSETREGLEN + INSETREGI] + IOSETNAME_CODE), rect[i].colorT[0]);
		if ((strlen((char *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_INPTSTS_STR1) * INSETREGLEN + INSETREGI] + Iosetnamepos))) > 18)
		{
			DrawFMultiLStr16(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis - SAFE_DIST4,
							 (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * (IOREG[(i - RECT_INPTSTS_STR1) * INSETREGLEN + INSETREGI]) + Iosetnamepos),
							 18, 2, rect[i].index[0]);
		}
		else
		{
			DrawFFStr(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis + SAFE_DIST7,
					  (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_INPTSTS_STR1) * INSETREGLEN + INSETREGI] + Iosetnamepos), rect[i].colorT[0]);
		}
	}

	E_IOTESTEN();
	f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
	if (f == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		return;
	}
	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//键值为按键松开时
		if (keydata == 0xff)
		{
			if ((keyon) && (beeponf))
			{
				keyoff = keyon; //按键键值传给松开键值
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理
		{
			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_INPTSTS_SCRNSVR)
				{
				}
				else
				{
					DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]); //常规按键特效
				}

				Times = 0;
				delay = KEYDT; //按键防抖次数
			}
			else if (keyon != keydata) //已有按键按下，不处理新按键信息
			{
				keydata = 0;
			}
			if (Times > delay) //按键防抖
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);
					switch (keyon) //按键按下标题提示语显示
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
			}
		}
		else
		{
			key_on_flag1 = 0;
		}
		if (keyoff) //按键松开处理
		{
			switch (keyoff)
			{
			case KEY_INPTSTS_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_INPTSTS_BACK:
				pno = poldno[--pnoindex];
				E_IOTESTDIS();
				break;
			case KEY_INPTSTS_IO:
#if 0
				if ((g_user.newuser > 5) && (g_user.newuser < 10))
				{
					pno = MENU_INPUTSET;
					poldno[pnoindex++] = MENU_INPUTSTATUS;
				}
				else
				{
					pno = MENU_FTUSER;
					poldno[pnoindex++] = MENU_INPUTSTATUS;
					poldno[pnoindex++] = MENU_INPUTSET;
					// poldno[2] = MENU_INPUTSTATUS;
				}
#else
				pno = MENU_INPUTSET;
				poldno[pnoindex++] = MENU_INPUTSTATUS;
#endif
				g_NO_ReagIOf = 0;
				g_runstep = 0;
				break;
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_INPTSTS_SCRNSVR) //不需要显示的按键
			{
			}
			//非常规按键-//绘制颜色文字按键
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
		}

		if (Timer_ms > 50)
		{
			Timer_ms = 0;
			f = Read_D(D_Address[SYS] + SYS_INPUTL, 2, (U16 *)(&YREG[SYS_INPUTL])); // 输入口当前状态低位
			if (f == 0)
			{
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex++] = MENU_INPUTSTATUS;
				plastno = MENU_INPUTSTATUS;
				break;
			}
			f = Read_D(D_Address[SYS] + SYS_EXINPUT, 1, (U16 *)(&YREG[SYS_EXINPUT])); // 扩展输入口当前状态低位
			if (f == 0)
			{
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex++] = MENU_INPUTSTATUS;
				plastno = MENU_INPUTSTATUS;
				break;
			}

			g_InputPointStatus = ((U32)(YREG[SYS_INPUTH]) << 16) + (U32)(YREG[SYS_INPUTL]);
			g_ExInputPointStatus = (U16)(YREG[SYS_EXINPUT]);
			if ((InputPointStatuOld != g_InputPointStatus) || (ExInputPointStatuOld != g_ExInputPointStatus))
			{
				InputPointStatuOld = g_InputPointStatus;
				ExInputPointStatuOld = g_ExInputPointStatus;

				DispInStatus(&rect[RECT_INPTSTS_STA1], RECT_INPTSTS_STA12 - RECT_INPTSTS_STA1 + 1, g_InputPointStatus, (U32 *)inputdat);
				DispInStatus(&rect[RECT_INPTSTS_STAE1], RECT_INPTSTS_STAE4 - RECT_INPTSTS_STAE1 + 1, (U32)g_ExInputPointStatus, (U32 *)exinputdat);
				ErrorMsg(MENU_INPUTSTATUS);
			}
		}
		if (pno != MENU_INPUTSTATUS)
		{
			plastno = MENU_INPUTSTATUS;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//-------------------------------------------------------------
#define KEY_OUPTSTS_SCRNSVR 1
#define KEY_OUPTSTS_BACK 2
#define KEY_OUPTSTS_STATE 3
#define KEY_OUPTSTS_TEST 4
#define KEY_OUPTSTS_IO 5
#define KEY_OUPTSTS_OUT1 6
#define KEY_OUPTSTS_OUT2 7
#define KEY_OUPTSTS_OUT3 8
#define KEY_OUPTSTS_OUT4 9
#define KEY_OUPTSTS_OUT5 10
#define KEY_OUPTSTS_OUT6 11
#define KEY_OUPTSTS_OUT7 12
#define KEY_OUPTSTS_OUT8 13
#define KEY_OUPTSTS_OUT9 14
#define KEY_OUPTSTS_OUTA 15
#define KEY_OUPTSTS_OUTB 16
#define KEY_OUPTSTS_OUTC 17
#define KEY_OUPTSTS_OUTE1 18
#define KEY_OUPTSTS_OUTE2 19
#define KEY_OUPTSTS_OUTE3 20
#define KEY_OUPTSTS_OUTE4 21

#define KEY_OUPTSTS_MAX 22

#define RECT_OUPTSTS_SEL1 1
#define RECT_OUPTSTS_SEL2 2
#define RECT_OUPTSTS_SEL3 3
#define RECT_OUPTSTS_SEL4 4
#define RECT_OUPTSTS_SEL5 5
#define RECT_OUPTSTS_SEL6 6
#define RECT_OUPTSTS_SEL7 7
#define RECT_OUPTSTS_SEL8 8
#define RECT_OUPTSTS_SEL9 9
#define RECT_OUPTSTS_SEL10 10
#define RECT_OUPTSTS_SEL11 11
#define RECT_OUPTSTS_SEL12 12
#define RECT_OUPTSTS_SELE1 13
#define RECT_OUPTSTS_SELE2 14
#define RECT_OUPTSTS_SELE3 15
#define RECT_OUPTSTS_SELE4 16

#define RECT_OUPTSTS_STA1 17
#define RECT_OUPTSTS_STA2 18
#define RECT_OUPTSTS_STA3 19
#define RECT_OUPTSTS_STA4 20
#define RECT_OUPTSTS_STA5 21
#define RECT_OUPTSTS_STA6 22
#define RECT_OUPTSTS_STA7 23
#define RECT_OUPTSTS_STA8 24
#define RECT_OUPTSTS_STA9 25
#define RECT_OUPTSTS_STA10 26
#define RECT_OUPTSTS_STA11 27
#define RECT_OUPTSTS_STA12 28
#define RECT_OUPTSTS_STAE1 29
#define RECT_OUPTSTS_STAE2 30
#define RECT_OUPTSTS_STAE3 31
#define RECT_OUPTSTS_STAE4 32

#define RECT_OUPTSTS_STR1 33
#define RECT_OUPTSTS_STR2 34
#define RECT_OUPTSTS_STR3 35
#define RECT_OUPTSTS_STR4 36
#define RECT_OUPTSTS_STR5 37
#define RECT_OUPTSTS_STR6 38
#define RECT_OUPTSTS_STR7 39
#define RECT_OUPTSTS_STR8 40
#define RECT_OUPTSTS_STR9 41
#define RECT_OUPTSTS_STR10 42
#define RECT_OUPTSTS_STR11 43
#define RECT_OUPTSTS_STR12 44
#define RECT_OUPTSTS_STRE1 45
#define RECT_OUPTSTS_STRE2 46
#define RECT_OUPTSTS_STRE3 47
#define RECT_OUPTSTS_STRE4 48

//------------------------------------------
// Function   : void Menu_Outputstatus(void)
// Description: 输出信号状态显示
//------------------------------------------
void Menu_Outputstatus(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U8 f;
	U16 Iosetnamepos;
	U8 FuntionStatus = 0; //功能状态,0:输出点状态; 1:输出点测试
	U8 TouchCheck = 0;
	U32 OutputPointStatus2 = 0;
	U16 ExOutputPointStatus2 = 0;
	U8 UPRESS = 0;
	U8 btnEnable[16] = {0};
	U8 dis_update = 1;
	const U8 keynum = {KEY_OUPTSTS_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},													 //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},									 //2 返回
		{TYPE_X12 - KEY_X12, TYPE_Y4, KEY_X12, KEY_Y12},					 //3	输出状态
		{TYPE_X12, TYPE_Y4, KEY_X12, KEY_Y12},								 //4	输出测试
		{TYPE_X10, TYPE_Y4, KEY_X12, KEY_Y12},								 //5 可编程IO设置
		{TYPE_X35, TYPE_Y3, KEY_X11, KEY_Y11},								 //6 选项1
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //7 选项2
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //8 选项3
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //9 选项4
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //10 选项5
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //11 选项6
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //12 选项7
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //13 选项8
		{TYPE_X53, TYPE_Y3, KEY_X11, KEY_Y11},								 //14 选项9
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //15 选项10
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //16 选项11
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //17 选项12
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //18 选项13
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //19 选项14
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //20 选项15
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //21 选项16
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {TLFNADD_IOALL + 1, 0, 0, 0}, {0}},																		 //1 	屏保
		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 2, 0, 0, 0}, {0}},																		 //2	返回
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_IOALL + 8, 0, 0, 0}, {TLFNADD_IOALL + 8}},	 //3	输出状态
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_IOALL + 9, 0, 0, 0}, {TLFNADD_IOALL + 9}},	 //4	输出测试
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_IOALL + 131, 0, 0, 0}, {TLFNADD_IOALL + 131}},									 //5	可编程IO设置
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 10, 0, 0, 0}, {TLFNADD_IOALL + 10}},	 //6	选项1
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 11, 0, 0, 0}, {TLFNADD_IOALL + 11}},	 //7	选项2
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 12, 0, 0, 0}, {TLFNADD_IOALL + 12}},	 //8	选项3
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 13, 0, 0, 0}, {TLFNADD_IOALL + 13}},	 //9	选项4
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 14, 0, 0, 0}, {TLFNADD_IOALL + 14}},	 //10	选项5
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 15, 0, 0, 0}, {TLFNADD_IOALL + 15}},	 //11	选项6
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 16, 0, 0, 0}, {TLFNADD_IOALL + 16}},	 //12	选项7
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 17, 0, 0, 0}, {TLFNADD_IOALL + 17}},	 //13	选项8
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 18, 0, 0, 0}, {TLFNADD_IOALL + 18}},	 //14	选项9
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 19, 0, 0, 0}, {TLFNADD_IOALL + 19}},	 //15	选项10
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 20, 0, 0, 0}, {TLFNADD_IOALL + 20}},	 //16	选项11
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 21, 0, 0, 0}, {TLFNADD_IOALL + 21}},	 //17	选项12
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 118, 0, 0, 0}, {TLFNADD_IOALL + 118}}, //18	选项13
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 119, 0, 0, 0}, {TLFNADD_IOALL + 119}}, //19	选项14
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 120, 0, 0, 0}, {TLFNADD_IOALL + 120}}, //20	选项15
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 121, 0, 0, 0}, {TLFNADD_IOALL + 121}}, //21	选项16
	};

	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_IOALL + 33}},

		{TYPE_X35, TYPE_Y3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 10}},								   //1 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 11}},  //2 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 12}},  //3 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 13}},  //4 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 14}},  //5 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 15}},  //6 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 16}},  //7 IO号显示框
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 17}},  //8 IO号显示框
		{TYPE_X53, TYPE_Y3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 18}},								   //9 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 19}},  //10 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 20}},  //11 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 21}},  //12 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 118}}, //13 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 119}}, //14 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 120}}, //15 IO号显示框
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 121}}, //16 IO号显示框

		{TYPE_X35 + 284, TYPE_Y3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},								  //1 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //2 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //3 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //4 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //5 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //6 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //7 IO号显示框
		{TYPE_X35 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //8 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},								  //9 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //10 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //11 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //12 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //13 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //14 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //15 IO号显示框
		{TYPE_X53 + 284, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //16 IO号显示框

		{TYPE_X35 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //1 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //2 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //3 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //4 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //5 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //6 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //7 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //8 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //9 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //10 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //11 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //12 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //13 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //14 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //15 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //16 IO号显示框
	};

	SetLanguage(MENU_IOTEST);
	InitScreen();
	initViewStatus();
	updateViewStatus();

	if (P_LAN)
	{
		Iosetnamepos = IOSETNAME_ENGLISH;
	}
	else
	{
		Iosetnamepos = IOSETNAME_CHINA;
	}

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	DrawButton(keyd[KEY_OUPTSTS_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_OUPTSTS_BACK].border[0], keybmpind[KEY_OUPTSTS_BACK].icon[0]);
	DrawButtonSingleColor(keyd[KEY_OUPTSTS_IO], keybmpind[KEY_OUPTSTS_IO].border[0], keybmpind[KEY_OUPTSTS_IO].icon[0]);
	Draw24x24Str(keyd[KEY_OUPTSTS_IO].xpis + keyd[KEY_OUPTSTS_IO].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSTS_IO].nameStr[0] * TitleLengthMax)) * 12 / 2,
				 keyd[KEY_OUPTSTS_IO].ypis + (keyd[KEY_OUPTSTS_IO].ysize - 24) / 2,
				 (MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSTS_IO].nameStr[0] * TitleLengthMax), SheetPage, keybmpind[KEY_OUPTSTS_IO].nameStr[0], keybmpind[KEY_OUPTSTS_IO].textColor[0]);

	for (i = RECT_OUPTSTS_STR1; i <= RECT_OUPTSTS_STRE4; i++)
	{
		DrawLine(rect[i].xpis, rect[i].ypis + rect[i].ysize, rect[i].xpis + rect[i].xsize, rect[i].ypis + rect[i].ysize, rect[i].colorB[0]);
	}

	//IO参数和参数说明
	Read_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG)); //读取IO参数
	if ((IOREG[0] != 0x5aa5) && (IOREG[0] != 0x5a5a))	   //通信故障
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		return;
	}

	for (i = RECT_OUPTSTS_STR1; i <= RECT_OUPTSTS_STRE4; i++)
	{
		DrawFFStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST7,
				  (U16 *)(MEM_OUTSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSTS_STR1) * OUTSETREGLEN + OUTSETREGI] + IOSETNAME_CODE), rect[i].colorT[0]);

		if ((strlen((char *)(MEM_OUTSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSTS_STR1) * OUTSETREGLEN + OUTSETREGI] + Iosetnamepos))) > 18)
		{
			DrawFMultiLStr16(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis - SAFE_DIST4,
							 (U16 *)(MEM_OUTSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSTS_STR1) * OUTSETREGLEN + OUTSETREGI] + Iosetnamepos),
							 18, 2, rect[i].index[0]);
		}
		else
		{
			DrawFFStr(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis + SAFE_DIST7,
					  (U16 *)(MEM_OUTSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSTS_STR1) * OUTSETREGLEN + OUTSETREGI] + Iosetnamepos), rect[i].colorT[0]);
		}
		if (IOREG[(i - RECT_OUPTSTS_STR1) * OUTSETREGLEN + OUTSETREGI] == 0)
		{
			btnEnable[i - RECT_OUPTSTS_STR1] = 0;
		}
		else
		{
			btnEnable[i - RECT_OUPTSTS_STR1] = 1;
		}
	}

	E_IOTESTEN();
	f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
	if (f == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		return;
	}
	f = Read_D(D_Address[SYS] + SYS_OUTPUTL, 2, (U16 *)(&YREG[SYS_OUTPUTL])); // 输出口当前状态低位
	if (f == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		return;
	}
	f = Read_D(D_Address[SYS] + SYS_EXOUTPUT, 1, (U16 *)(&YREG[SYS_EXOUTPUT])); // 扩展输出口当前状态低位
	if (f == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		return;
	}

	g_OutputPointStatus = ((U32)(YREG[SYS_OUTPUTH]) << 16) + (U32)(YREG[SYS_OUTPUTL]);
	g_ExOutputPointStatus = (U16)(YREG[SYS_EXOUTPUT]);
	DispOutStatus(&rect[RECT_OUPTSTS_STA1], RECT_OUPTSTS_STA12 - RECT_OUPTSTS_STA1 + 1, g_OutputPointStatus, (U32 *)outputdat);
	DispOutStatus(&rect[RECT_OUPTSTS_STAE1], RECT_OUPTSTS_STAE4 - RECT_OUPTSTS_STAE1 + 1, (U32)g_ExOutputPointStatus, (U32 *)exoutputdat);

	g_OutputPointStatusOld = g_OutputPointStatus;
	g_ExOutputPointStatusOld = g_ExOutputPointStatus;
	ViewPage(usepageno);
	//更新输出状态显示

	Timer_ms = 0;
	Timerms = 0;

	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);
		if ((keydata >= KEY_OUPTSTS_OUT1) && (keydata <= KEY_OUPTSTS_OUTE4))
		{
			if ((FuntionStatus == 0) || (btnEnable[keydata - KEY_OUPTSTS_OUT1] == 0))
			{
				keydata = 0;
				resetKeyData();
			}
		}
		if (keydata == 0xff)
		{
			if ((keyon) && (beeponf))
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER)
		{

			if (keyon == 0)
			{
				keyon = keydata;
				if (keyon == KEY_OUPTSTS_SCRNSVR)
				{
				}
				else
				{
					DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]); //常规按键特效
					if ((keydata >= KEY_OUPTSTS_OUT1) && (keydata <= KEY_OUPTSTS_OUTE4))
					{
						UPRESS = 1;
					}
				}

				Times = 0;
				delay = KEYDT;
			}
			else if (keyon != keydata)
			{
				keydata = 0;
			}
			if (Times > delay)
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (P_KBC == 0)
						Beep_On(BELLT);
					switch (keyon) //按键按下标题提示语显示
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					delay = KEYNT;
					if ((keyon >= KEY_OUPTSTS_OUT1) && (keyon <= KEY_OUPTSTS_OUTE4)) // 测试指令输出
					{
						U8 tmpIndex = 0;
						tmpIndex = keyon - KEY_OUPTSTS_OUT1 + RECT_OUPTSTS_STA1;
						TouchCheck = 1;
						if (FuntionStatus)
						{
							f = Write_Cmd(C_IOOUTPUT_CHECK, keyon - KEY_OUPTSTS_OUT1 + 1, OUTPUT_H);
							DrawFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis, rect[tmpIndex].xsize, rect[tmpIndex].ysize, rect[tmpIndex].colorB[1]);
							Draw24X24FStr(rect[tmpIndex].xpis + rect[tmpIndex].xsize / 2 - 16, rect[tmpIndex].ypis + rect[tmpIndex].ysize / 2 - 12, "ON", rect[tmpIndex].colorT[1]);
							if (f == 0)
							{
								pno = MENU_RWHELP;
								poldno[pnoindex++] = MENU_OUTPUTSTATUS;
								hleppara = 15;
								break;
							}
						}
					}
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0;
			}
		}
		else
		{
			key_on_flag1 = 0;
		}

		if (keyoff)
		{
			if ((keyoff > KEY_OUPTSTS_IO) && (keyoff < keynum))
			{
			}
			else
			{
				switch (keyoff)
				{
				case KEY_OUPTSTS_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_OUPTSTS_IO:
#if 0
					if ((g_user.newuser > 5) && (g_user.newuser < 10))
					{
						pno = MENU_OUTPUTSET;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
					}
					else
					{
						pno = MENU_FTUSER;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						poldno[pnoindex++] = MENU_OUTPUTSET;
						// poldno[2] = MENU_OUTPUTSET;
					}
#else
					pno = MENU_OUTPUTSET;
					poldno[pnoindex++] = MENU_OUTPUTSTATUS;
#endif
					g_NO_ReagIOf = 0;
					g_runstep = 0;
					break;
				case KEY_OUPTSTS_BACK:
					pno = poldno[--pnoindex];
					if (FuntionStatus)
					{
						g_OutputPointStatus = OutputPointStatus2;
						g_ExOutputPointStatus = ExOutputPointStatus2;
					}

					for (i = 0; i < 12; i++) //输出口
					{
						if (g_OutputPointStatus & outputdat[i])
						{
							f = Write_Cmd(C_IOOUTPUT_CHECK, i + 1, OUTPUT_H);
						}
						else
						{
							f = Write_Cmd(C_IOOUTPUT_CHECK, i + 1, OUTPUT_L);
						}

						if (f == 0)
						{
							pno = MENU_RWHELP;
							poldno[pnoindex++] = MENU_OUTPUTSTATUS;
							hleppara = 15;
							break;
						}
					}

					for (i = 0; i < 4; i++) //扩展输出口  需改
					{
						if (g_ExOutputPointStatus & exoutputdat[i])
						{
							f = Write_Cmd(C_IOOUTPUT_CHECK, i + 12 + 1, OUTPUT_H);
						}
						else
						{
							f = Write_Cmd(C_IOOUTPUT_CHECK, i + 12 + 1, OUTPUT_L);
						}

						if (f == 0)
						{
							pno = MENU_RWHELP;
							hleppara = 15;
							poldno[pnoindex++] = MENU_OUTPUTSTATUS;
							break;
						}
					}

					f = Write_Cmd(C_IOOUTPUT_CHECK, 25, 0); //恢复原来状态
					if (f == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						break;
					}
					E_IOTESTDIS();
					f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
					break;
				case KEY_OUPTSTS_STATE:
					if (FuntionStatus == 1) // 状态
					{
						FuntionStatus = 0;
						dis_update = 1;
						for (i = 0; i < 12; i++)
						{ //输出口
							if (OutputPointStatus2 & outputdat[i])
							{
								f = Write_Cmd(C_IOOUTPUT_CHECK, i + 1, OUTPUT_H);
							}
							else
							{
								f = Write_Cmd(C_IOOUTPUT_CHECK, i + 1, OUTPUT_L);
							}

							if (f == 0)
							{
								pno = MENU_RWHELP;
								hleppara = 15;
								poldno[pnoindex++] = MENU_OUTPUTSTATUS;
								break;
							}
						}
						for (i = 0; i < 4; i++)
						{ //扩展输出口  需改
							if (ExOutputPointStatus2 & exoutputdat[i])
							{
								f = Write_Cmd(C_IOOUTPUT_CHECK, i + 12 + 1, OUTPUT_H);
							}
							else
							{
								f = Write_Cmd(C_IOOUTPUT_CHECK, i + 12 + 1, OUTPUT_L);
							}
							if (f == 0)
							{
								pno = MENU_RWHELP;
								hleppara = 15;
								poldno[pnoindex++] = MENU_OUTPUTSTATUS;
								break;
							}
						}
						f = Write_Cmd(C_IOOUTPUT_CHECK, 25, 0); //恢复原来状态
						if (f == 0)
						{
							pno = MENU_RWHELP;
							hleppara = 15;
							poldno[pnoindex++] = MENU_OUTPUTSTATUS;
							break;
						}
					}
					break;
				case KEY_OUPTSTS_TEST:
					if (FuntionStatus == 0) // 测试
					{
						FuntionStatus = 1;
						dis_update = 1;
						OutputPointStatus2 = g_OutputPointStatus;
						ExOutputPointStatus2 = g_ExOutputPointStatus;
					}
					break;
				}
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_OUPTSTS_SCRNSVR) //不需要显示的按键
			{
			}
			//非常规按键-//绘制颜色文字按键
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
				if ((keydata >= KEY_OUPTSTS_OUT1) && (keydata <= KEY_OUPTSTS_OUTE4))
				{
					TouchCheck = 0;
					UPRESS = 0;
				}
			}
			if ((keyon >= KEY_OUPTSTS_OUT1) && (keyon <= KEY_OUPTSTS_OUTE4)) // 测试指令输出
			{
				U8 tmpIndex = 0;
				tmpIndex = keyon - KEY_OUPTSTS_OUT1 + RECT_OUPTSTS_STA1;
				TouchCheck = 0;
				if (FuntionStatus)
				{
					f = Write_Cmd(C_IOOUTPUT_CHECK, keyon - KEY_OUPTSTS_OUT1 + 1, 2);
					DrawFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis, rect[tmpIndex].xsize, rect[tmpIndex].ysize, rect[tmpIndex].colorB[0]);
					Draw24X24FStr(rect[tmpIndex].xpis + rect[tmpIndex].xsize / 2 - 12, rect[tmpIndex].ypis + rect[tmpIndex].ysize / 2 - 12, "OFF", rect[tmpIndex].colorT[1]);
					if (f == 0)
					{
						pno = MENU_RWHELP;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						hleppara = 15;
						break;
					}
				}
			}
			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
		}

		if ((Timer_ms > 50) && ((FuntionStatus == 0) || ((FuntionStatus) && (TouchCheck == 0))))
		{
			Timer_ms = 0;

			f = Read_D(D_Address[SYS] + SYS_OUTPUTL, 2, (U16 *)(&YREG[SYS_OUTPUTL])); // 输入口当前状态低位
			if (f == 0)
			{
				pno = MENU_RWHELP;
				poldno[pnoindex++] = MENU_OUTPUTSTATUS;
				hleppara = 15;
				return;
			}
			f = Read_D(D_Address[SYS] + SYS_EXOUTPUT, 1, (U16 *)(&YREG[SYS_EXOUTPUT])); // 扩展输出口当前状态低位
			if (f == 0)
			{
				pno = MENU_RWHELP;
				poldno[pnoindex++] = MENU_OUTPUTSTATUS;
				hleppara = 15;
				return;
			}
			g_OutputPointStatus = ((U32)(YREG[SYS_OUTPUTH]) << 16) + (U32)(YREG[SYS_OUTPUTL]);
			g_ExOutputPointStatus = (U16)(YREG[SYS_EXOUTPUT]);

			if ((g_OutputPointStatusOld != g_OutputPointStatus) || (g_ExOutputPointStatusOld != g_ExOutputPointStatus))
			{
				DispOutStatus(&rect[RECT_OUPTSTS_STA1], RECT_OUPTSTS_STA12 - RECT_OUPTSTS_STA1 + 1, g_OutputPointStatus, (U32 *)outputdat);
				DispOutStatus(&rect[RECT_OUPTSTS_STAE1], RECT_OUPTSTS_STAE4 - RECT_OUPTSTS_STAE1 + 1, (U32)g_ExOutputPointStatus, (U32 *)exoutputdat);
				//更新输出状态显示
				g_OutputPointStatusOld = g_OutputPointStatus;
				g_ExOutputPointStatusOld = g_ExOutputPointStatus;
			}
		}

		if (Timerms > 200)
		{
			Timerms = 0;
			if (UPRESS)
			{
				Read_D(D_Address[SYS] + SYS_CAR_OUTI, 1, (U16 *)(&YREG[SYS_CAR_OUTI]));
				// DrawMFS32P(550, 4, YREG[SYS_CAR_OUTI], 4, 1, Color_White);
				// Draw24X24FStr(628, 4, "A", Color_White);
			}
		}

		if (dis_update == 1)
		{
			dis_update = 0;
			for (i = KEY_OUPTSTS_STATE; i <= KEY_OUPTSTS_TEST; i++)
			{
				f = i - KEY_OUPTSTS_STATE;
				DrawButtonSingleColor(keyd[i], keybmpind[i].border[(f == FuntionStatus) ? 1 : 0], keybmpind[i].icon[(f == FuntionStatus) ? 1 : 0]);
				Draw24x24Str(keyd[i].xpis + keyd[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax)) * 12 / 2,
							 keyd[i].ypis + (keyd[i].ysize - 24) / 2,
							 (MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax), SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[(f == FuntionStatus) ? 1 : 0]);
			}
			if (FuntionStatus == 1)
			{
				for (i = KEY_OUPTSTS_OUT1; i <= KEY_OUPTSTS_OUTE4; i++)
				{
					if (btnEnable[i - KEY_OUPTSTS_OUT1] == 0)
					{
						DrawButtonSingleColor(keyd[i], keybmpind[i].border[1], keybmpind[i].icon[1]);
					}
					else
					{
						DrawButtonSingleColor(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
					}

					Draw24x24Str(keyd[i].xpis + keyd[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax)) * 12 / 2,
								 keyd[i].ypis + (keyd[i].ysize - 24) / 2,
								 (MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax), SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[0]);
				}
			}
			else if (FuntionStatus == 0)
			{
				for (i = RECT_OUPTSTS_SEL1; i <= RECT_OUPTSTS_SELE4; i++)
				{
					DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
					Draw24x24Str(rect[i].xpis + rect[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax)) * 12 / 2,
								 rect[i].ypis + (rect[i].ysize - 24) / 2,
								 (MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax), SheetPage, rect[i].index[0], rect[i].colorT[0]);
				}
			}
		}

		if (pno != MENU_OUTPUTSTATUS)
		{
			plastno = MENU_OUTPUTSTATUS;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//----------------------------------------------------
#define KEY_INPTSET_SCRNSVR 1

#define KEY_INPTSET_BACK 2
#define KEY_INPTSET_RESTORE 3
#define KEY_INPTSET_IN1 4
#define KEY_INPTSET_IN2 5
#define KEY_INPTSET_IN3 6
#define KEY_INPTSET_IN4 7
#define KEY_INPTSET_IN5 8
#define KEY_INPTSET_IN6 9
#define KEY_INPTSET_IN7 10
#define KEY_INPTSET_IN8 11
#define KEY_INPTSET_IN9 12
#define KEY_INPTSET_INA 13
#define KEY_INPTSET_INB 14
#define KEY_INPTSET_INC 15
#define KEY_INPTSET_INE1 16
#define KEY_INPTSET_INE2 17
#define KEY_INPTSET_INE3 18
#define KEY_INPTSET_INE4 19

#define KEY_INPTSET_BACK1 20
#define KEY_INPTSET_ENTER1 21
#define KEY_INPTSET_NOUSE 22
#define KEY_INPTSET_SETTING 23
#define KEY_INPTSET_UP 24
#define KEY_INPTSET_DOWN 25

#define KEY_INPTSET_BACK2 26
#define KEY_INPTSET_ENTER2 27
#define KEY_INPTSET_LOGIC 28
#define KEY_INPTSET_METHOD 29

#define RECT_INPTSET1_STR1 1
#define RECT_INPTSET1_STR2 2
#define RECT_INPTSET1_STR3 3
#define RECT_INPTSET1_STR4 4
#define RECT_INPTSET1_STR5 5
#define RECT_INPTSET1_STR6 6
#define RECT_INPTSET1_STR7 7
#define RECT_INPTSET1_STR8 8
#define RECT_INPTSET1_STR9 9
#define RECT_INPTSET1_STR10 10
#define RECT_INPTSET1_STR11 11
#define RECT_INPTSET1_STR12 12
#define RECT_INPTSET1_STRE1 13
#define RECT_INPTSET1_STRE2 14
#define RECT_INPTSET1_STRE3 15
#define RECT_INPTSET1_STRE4 16

#define RECT_INPTSET2_TIT 17
#define RECT_INPTSET2_MSG 18
#define RECT_INPTSET2_SELAll 19
#define RECT_INPTSET2_SEL1 20
#define RECT_INPTSET2_SEL2 21
#define RECT_INPTSET2_SEL3 22
#define RECT_INPTSET2_SEL4 23
#define RECT_INPTSET2_SEL5 24
#define RECT_INPTSET2_SEL6 25
#define RECT_INPTSET2_SEL7 26
#define RECT_INPTSET2_LINE 27

#define RECT_INPTSET3_TIT 28
#define RECT_INPTSET3_ITEM1_TIT 29
#define RECT_INPTSET3_ITEM1_MSG 30
#define RECT_INPTSET3_ITEM2_TIT 31
#define RECT_INPTSET3_ITEM2_MSG 32

#if 0
#define KEY_INPTSET_E5 29
#define KEY_INPTSET_E6 30
#define KEY_INPTSET_E7 31
#define KEY_INPTSET_E8 32
#define KEY_INPTSET_E9 33
#define KEY_INPTSET_EA 34
#define KEY_INPTSET_EB 35
#define KEY_INPTSET_EC 36

//------------------------------------------
// Function   : void Menu_InPSSelc(void)
// Description: 输入信号设置
//------------------------------------------
void Menu_InPSSelc(void)
{
	for (i = KEY_INPTSET_BACK1; i <= KEY_INPTSET_SETTING; i++)
	{
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	DrawRect(rect[2].xpis, rect[2].ypis, rect[2].xsize, rect[2].ysize, rect[2].colorT[0]); //参数选择整体框的绘制
	DrawRect(rect[8].xpis, rect[8].ypis, rect[8].xsize, rect[8].ysize, rect[8].colorT[0]); //输入讯号详细介绍框														//输入讯号选择框
	DrawFillRect(rect[7].xpis, rect[7].ypis, rect[7].xsize, rect[7].ysize, rect[7].colorB[0]);													//填充蓝色
	Draw24x24Str(rect[7].xpis + SAFE_DIST4, rect[7].ypis + SAFE_DIST4, (MEM_TITLE_ADD + 79 * TitleLengthMax), SheetPage, 79, rect[7].index[0]); //输入讯号选择
	DrawHLine(rect[9].xpis, rect[9].ypis, rect[9].xsize, rect[9].colorT[0]);																	//画水平实线
	//DrawHLine(8, 311, 80, Color_Black);
	if (g_slcnum < 13) //显示IO号
	{
		disIOname(rect[9].xpis + 11, rect[9].ypis - 25, (MEM_TITLE_ADD + (52 + g_slcnum) * TitleLengthMax), 1, SheetPage, 52 + g_slcnum, rect[9].index[0]);
	}
	else
	{
		disIOname(rect[9].xpis + 11, rect[9].ypis - 25, (MEM_TITLE_ADD + (93 + g_slcnum) * TitleLengthMax), 2, SheetPage, 93 + g_slcnum, rect[9].index[0]);
	}
	dis_updata1 = 1;

	i = (g_slcnum - 1) * 3;
	pa = IOREG[i + INSETREGI];
	if (pa >= INSETPRSUM)
		pa = 0;
	pb = IOREG[i + INSETREGI + 1];
	if (pb > 1)
		pb = 0;
	pc = IOREG[i + INSETREGI + 2];
	if (pc > 1)
		pc = 0;

					dis_updata1 = 0;

			BmpFillRect(rect[8].xpis + 83, rect[8].ypis + 6, 480, 28);
			Draw24x24MulStr(rect[8].xpis + 83, rect[8].ypis + 6, (MEM_INSETNAME + IOSETNAME_LENGTH * pa + Iosetnamepos), 40, Inset_Sheet, pa, 0, rect[8].index[0]);
			if (pa < 3)
			{
				a = 3 - pa;			// num1
				b = INSETPRSUM - a; // add1
			}
			else
			{
				if (pa >= INSETPRSUM)
					pa = INSETPRSUM - 1;
				b = pa - 3;			// add1
				a = INSETPRSUM - b; // num1
				if (a > 7)
					a = 7;
			}
			for (i = 0; i < a; i++)
			{
				DrawFillRect(rect[6].xpis, rect[6].ypis + i * 32, rect[6].xsize, rect[6].ysize, (i > 3) ? rect[6].colorB[6 - i] : rect[6].colorB[i]);
				Draw24X24FFStr(rect[6].xpis + SAFE_DIST7, rect[6].ypis + i * 32 + SAFE_DIST4, (MEM_INSETNAME + IOSETNAME_LENGTH * (b + i)), rect[6].index[0]);
			}
			for (a = 0; i < 7; i++)
			{
				DrawFillRect(rect[6].xpis, rect[6].ypis + i * 32, rect[6].xsize, rect[6].ysize, (i > 3) ? rect[6].colorB[6 - i] : rect[6].colorB[i]);
				Draw24X24FFStr(rect[6].xpis + SAFE_DIST7, rect[6].ypis + i * 32 + SAFE_DIST4, (MEM_INSETNAME + IOSETNAME_LENGTH * (a)), rect[6].index[0]);
				a++;
			}

			vINgroup = pa / 16;
			vINno = pa % 16;
			vINlevel = (YREG[SYS_INLEVEL + vINgroup] >> vINno) & 0x01;
			//DrawFPBMPBTN(rect[8].xpis+1,rect[8].ypis+1 , 80,80 , vINlevel ? 0 : 0, 0xffff);
			ovINlevel = vINlevel;
}
#endif
//------------------------------------------
// Function   : void Menu_Inputset(void)
// Description: 输入信号设置
//------------------------------------------
void Menu_Inputset(void)
{
#if 1
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U8 dis_updata = 1, dis_updata1 = 0, dis_updata2 = 0, dis_updata3 = 0;
	U8 tmprunstep = 0xff;
	U32 i;
	// U8 a, b;
	//U8 editflag[24];

	// U8 pa = 0, pb = 0, pc = 0;	 // pa:H组号 L:序号;pb:输入信号开关逻辑;pc:输入信号操作方式
	U8 pbuf[INSETREGLEN] = {0, 0, 0}; //内容、逻辑、方式
	U16 checksum = 0;
	U16 Iosetnamepos;
	U16 Contentmsgpos;
	U16 Inset_Sheet = 52;
	U16 Contentmsg_Sheet = 54;
	U8 vINgroup = 0, vINno = 0, vINlevel = 0, ovINlevel = 0;
	U8 selNoSum = INSETPRSUM;
	// U8 dp = 0;
	//const U8 keynum = {30};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32}, //1 屏保

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},									 //2 返回0
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},								 //3 还原
		{TYPE_X35, TYPE_Y3, KEY_X11, KEY_Y11},								 //6 选项1
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //7 选项2
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //8 选项3
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //9 选项4
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //10 选项5
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //11 选项6
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //12 选项7
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //13 选项8
		{TYPE_X53, TYPE_Y3, KEY_X11, KEY_Y11},								 //14 选项9
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //15 选项10
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //16 选项11
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //17 选项12
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //18 选项13
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //19 选项14
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //20 选项15
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //21 选项16

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},				  //20 返回1
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},			  //21 确认1
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},			  //24 回到NO
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},				  //25 详细设定
		{480, 215, KEY_X1, KEY_Y1},						  //22 上翻
		{480, 215 + KEY_Y1 + SAFE_DIST1, KEY_X1, KEY_Y1}, //23 下翻

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //26 返回2
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //27 确认2
		{TYPE_X14, 111, KEY_X13, KEY_Y13},	 //28 逻辑按钮
		{TYPE_X14, 190, KEY_X13, KEY_Y13},	 //29 交替开关
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {TLFNADD_IOALL + 1, 0, 0, 0}, {0}}, //1 屏保

		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 2, 0, 0, 0}, {0}},																		 //2	返回0
		{{KEY_Silver}, {0}, {236}, {TLFNADD_IOALL + 130, 0, 0, 0}, {0}},																	 //3	还原
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 53, 0, 0, 0}, {TLFNADD_IOALL + 53}},	 //6	选项1
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 54, 0, 0, 0}, {TLFNADD_IOALL + 54}},	 //7	选项2
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 55, 0, 0, 0}, {TLFNADD_IOALL + 55}},	 //8	选项3
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 56, 0, 0, 0}, {TLFNADD_IOALL + 56}},	 //9	选项4
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 57, 0, 0, 0}, {TLFNADD_IOALL + 57}},	 //10	选项5
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 58, 0, 0, 0}, {TLFNADD_IOALL + 58}},	 //11	选项6
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 59, 0, 0, 0}, {TLFNADD_IOALL + 59}},	 //12	选项7
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 60, 0, 0, 0}, {TLFNADD_IOALL + 60}},	 //13	选项8
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 61, 0, 0, 0}, {TLFNADD_IOALL + 61}},	 //14	选项9
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 62, 0, 0, 0}, {TLFNADD_IOALL + 62}},	 //15	选项10
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 63, 0, 0, 0}, {TLFNADD_IOALL + 63}},	 //16	选项11
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 64, 0, 0, 0}, {TLFNADD_IOALL + 64}},	 //17	选项12
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 106, 0, 0, 0}, {TLFNADD_IOALL + 106}}, //18	选项13
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 107, 0, 0, 0}, {TLFNADD_IOALL + 107}}, //19	选项14
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 108, 0, 0, 0}, {TLFNADD_IOALL + 108}}, //20	选项15
		{{KEY_Silver, KEY_Gray}, {Color_Black, Color_Black}, {KEY_Silver, KEY_Gray}, {TLFNADD_IOALL + 109, 0, 0, 0}, {TLFNADD_IOALL + 109}}, //21	选项16

		{{KEY_Silver}, {Color_Black}, {72}, {TLFNADD_IOALL + 67, 0, 0, 0}, {0}},	   //20 返回1
		{{KEY_Silver}, {Color_Black}, {71}, {TLFNADD_IOALL + 68, 0, 0, 0}, {0}},	   //21 确认1
		{{KEY_Silver}, {Color_Black}, {111, 112}, {TLFNADD_IOALL + 71, 0, 0, 0}, {0}}, //24 回到NO
		{{KEY_Silver}, {Color_Black}, {239}, {TLFNADD_IOALL + 72, 0, 0, 0}, {0}},	   //25 详细设定
		{{KEY_Silver}, {Color_Black}, {219, 220}, {TLFNADD_IOALL + 69, 0, 0, 0}, {0}}, //22 上翻
		{{KEY_Silver}, {Color_Black}, {221, 222}, {TLFNADD_IOALL + 70, 0, 0, 0}, {0}}, //23 下翻

		{{KEY_Silver}, {Color_Black}, {72}, {TLFNADD_IOALL + 67, 0, 0, 0}, {0}},																							   //26 返回2
		{{KEY_Silver}, {Color_Black}, {71}, {TLFNADD_IOALL + 68, 0, 0, 0}, {0}},																							   //27 确认2
		{{KEY_Silver, KEY_Blue}, {Color_Blue, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_IOALL + 77, TLFNADD_IOALL + 75, 0, 0}, {TLFNADD_IOALL + 77, TLFNADD_IOALL + 75}}, //28 逻辑按钮
		{{KEY_Silver, KEY_Blue}, {Color_Blue, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_IOALL + 78, TLFNADD_IOALL + 76, 0, 0}, {TLFNADD_IOALL + 78, TLFNADD_IOALL + 76}}, //29 操作开关
	};

	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_IOALL + 35}},

		{TYPE_X35 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //1 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //2 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //3 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //4 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //5 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //6 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //7 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //8 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //9 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //10 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //11 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //12 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //13 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //14 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //15 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //16 IO号显示框

		{TYPE_X14, TYPE_Y2, TYPE_SIZE33, TYPE_SIZE5, {Color_MidSkyBlue}, {Color_Black}, {TLFNADD_IOALL + 79}},						 //17 输入选择信号框/详细设定框
		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE33, TYPE_SIZE12 + 2, {Color_Black, Color_White}, {Color_Black}, {0}}, //18 文本显示框
		{340, 170, TYPE_SIZE15, TYPE_SIZE5 * 7 + SAFE_DIST2 * 2, {Color_Black, Color_White}, {Color_Black}, {0}},					 //19 参数框-大
		{340 + SAFE_DIST2, 170 + SAFE_DIST2, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_Gray}, {Color_Black}, {0}},						 //20 参数框-1
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_DarkGray}, {Color_Black}, {0}},		 //21 参数框-2
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 2, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {0}},	 //22 参数框-3
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 3, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}}, //23 参数框-4
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 4, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {0}},	 //24 参数框-5
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 5, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_DarkGray}, {Color_Black}, {0}},	 //25 参数框-6
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 6, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_Gray}, {Color_Black}, {0}},		 //26 参数框-7
		{240, 284, 100, 1, {Color_Black}, {Color_Black}, {TLFNADD_IOALL + 53, TLFNADD_IOALL + 106}},								 //27 IO号显示线

		{TYPE_X14, TYPE_Y2, TYPE_SIZE33, TYPE_SIZE5, {Color_MidSkyBlue}, {Color_Black}, {TLFNADD_IOALL + 72}},																	   //28 输入选择信号框/详细设定框
		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 80}},											   //29 状态设定
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, 395, TYPE_SIZE10 + 1, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 0}},					   //30 状态设定文本框
		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5 * 2 + TYPE_SIZE10 + 1, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 81}},						   //31 状态设定
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5 * 2 + TYPE_SIZE10 + 1, 395, TYPE_SIZE10 + 1, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 1}}, //32 状态设定文本框
	};
	if (P_LAN)
	{
		Iosetnamepos = IOSETNAME_ENGLISH;
		Contentmsgpos = CONTENTMSG_ENGLISH;
	}
	else
	{
		Iosetnamepos = IOSETNAME_CHINA;
		Contentmsgpos = CONTENTMSG_CHINA;
	}

	// if (P_LAN == 2)
	// { //越南语
	// 	dp = 16;
	// }
	if (g_NO_ReagIOf == 0)
	{
		Read_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG)); //读取IO参数
		if ((IOREG[0] != 0x5aa5) && (IOREG[0] != 0x5a5a))
		{ //通信故障
			pno = MENU_RWHELP;
			hleppara = 15;
		}
	}
	Read_D(D_Address[SYS] + SYS_INLEVEL, 6, (U16 *)(&YREG[SYS_INLEVEL])); // 读输入电平状态
	Timer_ms = 0;

	while (1)
	{
		if (dis_updata)
		{
			dis_updata = 0;

			if (tmprunstep != g_runstep)
			{
				SetLanguage(MENU_IOTEST);
				InitScreen();
				initViewStatus();
				updateViewStatus();
				Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
			}
			if (g_runstep == 0)
			{
				DrawButton(keyd[KEY_INPTSET_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_INPTSET_BACK].border[0], keybmpind[KEY_INPTSET_BACK].icon[0]);
				DrawButton(keyd[KEY_INPTSET_RESTORE], (U16 *)(ADD_ICONBMP), keybmpind[KEY_INPTSET_RESTORE].border[0], keybmpind[KEY_INPTSET_RESTORE].icon[0]);

				for (i = KEY_INPTSET_IN1; i <= KEY_INPTSET_INE4; i++)
				{
					DrawButtonSingleColor(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
					Draw24x24Str(keyd[i].xpis + keyd[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax)) * 12 / 2,
								 keyd[i].ypis + (keyd[i].ysize - 24) / 2,
								 (MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax), SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[0]);
				}

				for (i = RECT_INPTSET1_STR1; i <= RECT_INPTSET1_STRE4; i++)
				{
					DrawLine(rect[i].xpis, rect[i].ypis + rect[i].ysize, rect[i].xpis + rect[i].xsize, rect[i].ypis + rect[i].ysize, rect[i].colorB[0]);
				}

				for (i = RECT_INPTSET1_STR1; i <= RECT_INPTSET1_STRE4; i++)
				{
					DrawFFStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST7,
							  (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_INPTSET1_STR1) * INSETREGLEN + INSETREGI] + IOSETNAME_CODE), rect[i].colorT[0]);
					if ((strlen((char *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_INPTSET1_STR1) * INSETREGLEN + INSETREGI] + Iosetnamepos))) > 18)
					{
						DrawFMultiLStr16(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis - SAFE_DIST4,
										 (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * (IOREG[(i - RECT_INPTSET1_STR1) * INSETREGLEN + INSETREGI]) + Iosetnamepos),
										 18, 2, rect[i].index[0]);
					}
					else
					{
						DrawFFStr(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis + SAFE_DIST7,
								  (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_INPTSET1_STR1) * INSETREGLEN + INSETREGI] + Iosetnamepos), rect[i].colorT[0]);
					}
				}
			}
			else if (g_runstep == 1) //按钮进入后的界面
			{

				DrawFillRect(rect[RECT_INPTSET2_TIT].xpis, rect[RECT_INPTSET2_TIT].ypis, rect[RECT_INPTSET2_TIT].xsize, rect[RECT_INPTSET2_TIT].ysize, rect[RECT_INPTSET2_TIT].colorB[0]);
				Draw24x24Str(rect[RECT_INPTSET2_TIT].xpis + SAFE_DIST4, rect[RECT_INPTSET2_TIT].ypis + SAFE_DIST4,
							 (MEM_TITLE_ADD + (U32)rect[RECT_INPTSET2_TIT].index[0] * TitleLengthMax), SheetPage, rect[RECT_INPTSET2_TIT].index[0], rect[RECT_INPTSET2_TIT].colorT[0]); //输入讯号选择
				DrawRect(rect[RECT_INPTSET2_MSG].xpis, rect[RECT_INPTSET2_MSG].ypis, rect[RECT_INPTSET2_MSG].xsize, rect[RECT_INPTSET2_MSG].ysize, rect[RECT_INPTSET2_MSG].colorB[0]);
				DrawRect(rect[RECT_INPTSET2_SELAll].xpis, rect[RECT_INPTSET2_SELAll].ypis, rect[RECT_INPTSET2_SELAll].xsize, rect[RECT_INPTSET2_SELAll].ysize, rect[RECT_INPTSET2_SELAll].colorB[0]);
				for (i = KEY_INPTSET_BACK1; i <= KEY_INPTSET_DOWN; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}

				DrawHLine(rect[RECT_INPTSET2_LINE].xpis, rect[RECT_INPTSET2_LINE].ypis, rect[RECT_INPTSET2_LINE].xsize, rect[RECT_INPTSET2_LINE].colorT[0]); //画水平实线
				if (g_slcnum < 13)																															 //显示IO号
				{
					U8 tmpIndex = rect[RECT_INPTSET2_LINE].index[0] + g_slcnum - 1;
					disIOname(rect[RECT_INPTSET2_LINE].xpis - SAFE_DIST11, rect[RECT_INPTSET2_LINE].ypis - SAFE_DIST1, (MEM_TITLE_ADD + (U32)(tmpIndex)*TitleLengthMax),
							  2, SheetPage, tmpIndex, rect[RECT_INPTSET2_LINE].colorT[0]);
				}
				else
				{
					U8 tmpIndex = rect[RECT_INPTSET2_LINE].index[1] + g_slcnum - 13;
					disIOname(rect[RECT_INPTSET2_LINE].xpis - SAFE_DIST11, rect[RECT_INPTSET2_LINE].ypis - SAFE_DIST1, (MEM_TITLE_ADD + (U32)(tmpIndex)*TitleLengthMax),
							  3, SheetPage, tmpIndex, rect[RECT_INPTSET2_LINE].colorT[0]);
				}

				dis_updata1 = 1;
			}
			else if (g_runstep == 2)
			{
				U8 tmpIndex = 0;
				DrawFillRect(rect[RECT_INPTSET3_TIT].xpis, rect[RECT_INPTSET3_TIT].ypis, rect[RECT_INPTSET3_TIT].xsize, rect[RECT_INPTSET3_TIT].ysize, rect[RECT_INPTSET3_TIT].colorB[0]);
				Draw24x24Str(rect[RECT_INPTSET3_TIT].xpis + SAFE_DIST4, rect[RECT_INPTSET3_TIT].ypis + SAFE_DIST4,
							 (MEM_TITLE_ADD + (U32)rect[RECT_INPTSET3_TIT].index[0] * TitleLengthMax), SheetPage, rect[RECT_INPTSET3_TIT].index[0], rect[RECT_INPTSET3_TIT].colorT[0]); //详细设定
				for (i = 0; i < 2; i++)
				{
					tmpIndex = 2 * i + RECT_INPTSET3_ITEM1_TIT;
					DrawEditTit(rect[tmpIndex], rect[tmpIndex].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST4);
					DrawRect(rect[tmpIndex + 1].xpis, rect[tmpIndex + 1].ypis, rect[tmpIndex + 1].xsize, rect[tmpIndex + 1].ysize, rect[tmpIndex + 1].colorB[0]);
					Draw24x24MulStr(rect[tmpIndex + 1].xpis + SAFE_DIST4, rect[tmpIndex + 1].ypis + SAFE_DIST4,
									(MEM_CONTENTMSG + (U32)(CONTENTMSG_LENGTH * rect[tmpIndex + 1].index[0]) + Contentmsgpos), 30, Contentmsg_Sheet, rect[tmpIndex + 1].index[0], 0, rect[tmpIndex + 1].colorT[0]);
				}

				for (i = KEY_INPTSET_BACK2; i <= KEY_INPTSET_ENTER2; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}

				dis_updata2 = 1;
				dis_updata3 = 1;
			}
			if (tmprunstep != g_runstep)
			{
				tmprunstep = g_runstep;
				ViewPage(usepageno);
			}
		}

		keydata = get_keydata((KEYPAD *)keyd, KEY_INPTSET_SCRNSVR + 1, 0);
		if (keydata == 0)
		{
			if (g_runstep == 0)
			{

				keydata = get_keydata((KEYPAD *)keyd, KEY_INPTSET_INE4 + 1, KEY_INPTSET_BACK);
			}
			else if (g_runstep == 1)
			{
				keydata = get_keydata((KEYPAD *)keyd, KEY_INPTSET_DOWN + 1, KEY_INPTSET_BACK1);
			}
			else
			{
				keydata = get_keydata((KEYPAD *)keyd, KEY_INPTSET_METHOD + 1, KEY_INPTSET_BACK2);
			}
		}

		//键值为按键松开时
		if (keydata == 0xff)
		{
			if ((keyon == KEY_INPTSET_UP) || (keyon == KEY_INPTSET_DOWN))
			{
			}
			else if ((keyon) && (beeponf))
			{
				keyoff = keyon; //按键键值传给松开键值
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理
		{
			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_INPTSET_SCRNSVR)
				{
				}
				else
				{
					DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]); //常规按键特效
				}

				Times = 0;
				delay = KEYDT; //按键防抖次数
			}
			else if (keyon != keydata) //已有按键按下，不处理新按键信息
			{
				keydata = 0;
			}
			if (Times > delay) //按键防抖
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);

					switch (keyon) //按键按下标题提示语显示
					{

					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}

					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
				if ((keyon == KEY_INPTSET_UP) || (keyon == KEY_INPTSET_DOWN))
				{
					keyoff = keyon;
				}
			}
		}
		else
		{
			key_on_flag1 = 0;
		}
		if (keyoff)
		{
			if ((keyoff >= KEY_INPTSET_IN1) && (keyoff <= KEY_INPTSET_INE4)) //输入口 + 扩展输入
			{
				U8 tmpIndex = 0;
				g_slcnum = keyoff - KEY_INPTSET_IN1 + 1;

				tmpIndex = (g_slcnum - 1) * INSETREGLEN;
				pbuf[0] = (U8)IOREG[tmpIndex + INSETREGI];
				if (pbuf[0] >= selNoSum)
					pbuf[0] = 0;
				pbuf[1] = (U8)IOREG[tmpIndex + INSETREGI + 1];
				if (pbuf[1] > 1)
					pbuf[1] = 0;
				pbuf[2] = (U8)IOREG[tmpIndex + INSETREGI + 2];
				if (pbuf[2] > 1)
					pbuf[2] = 0;
				dis_updata = 1;
				g_runstep = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_INPTSET_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_INPTSET_BACK:
					pno = MENU_INPUTSTATUS;
					pnoindex--;
					// pnoindex = 1;
					// poldno[pnoindex--] = MENU_INPUTSET;
					if (g_user.newmode == 2)
					{
						g_user.newuser = g_user.olduser;
						g_user.newmode = g_user.oldmode;
						g_user.olduser = 0;
						g_user.oldmode = 0;
					}

					checksum = 0;
					for (i = INSETREGI; i < IOSETREGI; i++)
					{ //校验
						checksum += IOREG[i];
					}
					IOREG[IOREGCHECK] = checksum;
					if (Write_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG[0])) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 16; //操作失败
						break;
					}

					if (Write_Cmd(C_SAVEIO, 0, 0) == 0) //发送保存指令
					{
						pno = MENU_RWHELP;
						hleppara = 16; //操作失败
						break;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						poldno[pnoindex++] = MENU_INPUTSET;
						hleppara = 15;
						break;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{

						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}

					break;
				case KEY_INPTSET_BACK1:
					dis_updata = 1;
					g_runstep = 0;
					break;
				case KEY_INPTSET_ENTER1:
				case KEY_INPTSET_ENTER2:
					dis_updata = 1;
					g_runstep = 0;
					i = (g_slcnum - 1) * INSETREGLEN;
					IOREG[i + INSETREGI] = pbuf[0];
					IOREG[i + INSETREGI + 1] = pbuf[1];
					IOREG[i + INSETREGI + 2] = pbuf[2];
					break;
				case KEY_INPTSET_UP:
					if (pbuf[0])
					{
						pbuf[0]--;
					}
					else
					{
						pbuf[0] = selNoSum - 1;
					}
					dis_updata1 = 1;
					break;
				case KEY_INPTSET_DOWN:
					if (pbuf[0] >= selNoSum - 1)
					{
						pbuf[0] = 0;
					}
					else
					{
						pbuf[0]++;
					}
					dis_updata1 = 1;
					break;
				case KEY_INPTSET_NOUSE:
					pbuf[0] = 0;
					dis_updata1 = 1;
					break;
				case KEY_INPTSET_SETTING:
					dis_updata = 1;
					g_runstep = 2;
					break;
				case KEY_INPTSET_BACK2:
					dis_updata = 1;
					g_runstep = 1;
					break;
				case KEY_INPTSET_LOGIC:
					if (pbuf[1])
					{
						pbuf[1] = 0;
					}
					else
					{
						pbuf[1] = 1;
					}
					dis_updata2 = 1;
					break;
				case KEY_INPTSET_METHOD:
					if (pbuf[2])
					{
						pbuf[2] = 0;
					}
					else
					{
						pbuf[2] = 1;
					}
					dis_updata3 = 1;
					break;

				case KEY_INPTSET_RESTORE:
					pno = MENU_RWHELP;
					hleppara = 136;
					poldno[pnoindex++] = MENU_INPUTSET;
					break;
					// if (Write_Cmd(C_REINPUTSETTING, 0, 0) == 0)
					// { //发送还原设定指令
					// 	pno = MENU_RWHELP;
					// 	hleppara = 16;
					// 	poldno[pnoindex] = MENU_INPUTSET;
					// 	pnoindex++;
					// 	break;
					// }
					// if (Read_Status() == 0)
					// {
					// 	pno = MENU_RWHELP;
					// 	hleppara = 15;
					// 	poldno[pnoindex] = MENU_INPUTSET;
					// 	pnoindex++;
					// 	break;
					// }
					// else if (DREG[D_CMDSTATUS] != 100)
					// {
					// 	pno = MENU_RWHELP;
					// 	hleppara = 16;
					// 	poldno[pnoindex] = MENU_INPUTSET;
					// 	pnoindex++;
					// 	break;
					// }

					// Read_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG)); //读取IO参数
					// if ((IOREG[0] != 0x5aa5) && (IOREG[0] != 0x5a5a))
					// { //通信故障
					// 	pno = MENU_RWHELP;
					// 	hleppara = 15;
					// 	poldno[pnoindex] = MENU_INPUTSET;
					// 	pnoindex++;
					// 	break;
					// }

					// dis_updata = 1;
					// break;
				}
			}
			keyoff = 0;
		}

		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_INPTSET_SCRNSVR) //不需要显示的按键
			{
			}
			//非常规按键-//绘制颜色文字按键
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
		}

		if (dis_updata1)
		{
			U8 selNoList[7] = {0};
			U8 tmpIndex = 0;
			dis_updata1 = 0;
			BmpFillRect(rect[RECT_INPTSET2_MSG].xpis + 1, rect[RECT_INPTSET2_MSG].ypis + 1, rect[RECT_INPTSET2_MSG].xsize - 2, rect[RECT_INPTSET2_MSG].ysize - 2);

			Draw24x24MulStr(rect[RECT_INPTSET2_MSG].xpis + SAFE_DIST4, rect[RECT_INPTSET2_MSG].ypis + SAFE_DIST4,
							(MEM_INSETNAME + (U32)(IOSETNAME_LENGTH * pbuf[0]) + Iosetnamepos), 40, Inset_Sheet, pbuf[0], 3, rect[RECT_INPTSET2_MSG].index[0]);
			selNoList[3] = pbuf[0];
			if (pbuf[0] < 3)
			{
				U16 oneSide = 3 - pbuf[0];	  //超出范围数量
				for (i = 0; i < oneSide; i++) //给左侧超过范围的值赋值
				{
					selNoList[oneSide - 1 - i] = selNoSum - 1 - i;
				}
				for (i = oneSide; i < 7; i++) //给未超范围的值赋值
				{
					if (i == 3)
					{
						selNoList[3] = pbuf[0];
					}
					else
					{
						selNoList[i] = i - oneSide;
					}
				}
			}
			else if (pbuf[0] >= (selNoSum - 3) && pbuf[0] < selNoSum)
			{
				U16 oneSide = (selNoSum - 1 - pbuf[0]); //未超出范围数量

				for (i = 4 + oneSide; i < 7; i++) //给右侧超过范围的值赋值
				{
					selNoList[i] = i - (4 + oneSide);
				}
				for (i = 3 + oneSide;; i--) //给未超范围的值赋值
				{
					if (i == 3)
					{
						selNoList[3] = pbuf[0];
					}
					else
					{
						selNoList[i] = selNoSum - 1 - (3 + oneSide - i);
						if (i == 0)
							break;
					}
				}
			}
			else
			{
				for (i = 1; i <= 3; i++)
				{
					selNoList[3 - i] = selNoList[3] - i;
					selNoList[3 + i] = selNoList[3] + i;
				}
			}

			for (i = 0, tmpIndex = 0; i < 7; i++)
			{
				tmpIndex = RECT_INPTSET2_SEL1 + i;
				DrawFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis,
							 rect[tmpIndex].xsize, rect[tmpIndex].ysize, rect[tmpIndex].colorB[0]);
			}
			DrawDottedRect(rect[RECT_INPTSET2_SEL4].xpis + 1, rect[RECT_INPTSET2_SEL4].ypis + 1,
						   rect[RECT_INPTSET2_SEL4].xsize - 2, rect[RECT_INPTSET2_SEL4].ysize - 2, rect[RECT_INPTSET2_SEL4].colorT[0]);

			for (i = 0, tmpIndex = 0; i < 7; i++)
			{
				if (selNoList[i] != 0xFF)
				{
					tmpIndex = RECT_INPTSET2_SEL1 + i;
					Draw24x24Str(rect[tmpIndex].xpis + SAFE_DIST4, rect[tmpIndex].ypis + rect[tmpIndex].ysize / 2 - 12,
								 (MEM_INSETNAME + (U32)(selNoList[i] * IOSETNAME_LENGTH)), Inset_Sheet, selNoList[i], rect[tmpIndex].colorT[0]);
				}
			}

			// if (pa < 3)
			// {
			// 	a = 3 - pa;			// num1
			// 	b = INSETPRSUM - a; // add1
			// }
			// else
			// {
			// 	if (pa >= INSETPRSUM)
			// 		pa = INSETPRSUM - 1;
			// 	b = pa - 3;			// add1
			// 	a = INSETPRSUM - b; // num1
			// 	if (a > 7)
			// 		a = 7;
			// }
			// for (i = 0; i < a; i++)
			// {
			// 	DrawFillRect(rect[6].xpis, rect[6].ypis + i * 32, rect[6].xsize, rect[6].ysize, (i > 3) ? rect[6].colorB[6 - i] : rect[6].colorB[i]);
			// 	Draw24X24FFStr(rect[6].xpis + SAFE_DIST7, rect[6].ypis + i * 32 + SAFE_DIST4, (MEM_INSETNAME + IOSETNAME_LENGTH * (b + i)), rect[6].index[0]);
			// }
			// for (a = 0; i < 7; i++)
			// {
			// 	DrawFillRect(rect[6].xpis, rect[6].ypis + i * 32, rect[6].xsize, rect[6].ysize, (i > 3) ? rect[6].colorB[6 - i] : rect[6].colorB[i]);
			// 	Draw24X24FFStr(rect[6].xpis + SAFE_DIST7, rect[6].ypis + i * 32 + SAFE_DIST4, (MEM_INSETNAME + IOSETNAME_LENGTH * (a)), rect[6].index[0]);
			// 	a++;
			// }
			vINgroup = pbuf[0] / 16;
			vINno = pbuf[0] % 16;
			vINlevel = (YREG[SYS_INLEVEL + vINgroup] >> vINno) & 0x01;
			ovINlevel = vINlevel;
		}
		if (dis_updata2)
		{
			dis_updata2 = 0;

			DrawButtonSingleColor(keyd[KEY_INPTSET_LOGIC], keybmpind[KEY_INPTSET_LOGIC].border[0], keybmpind[KEY_INPTSET_LOGIC].icon[0]);
			Draw24x24Str(keyd[KEY_INPTSET_LOGIC].xpis + keyd[KEY_INPTSET_LOGIC].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_INPTSET_LOGIC].nameStr[pbuf[1]] * TitleLengthMax)) * 12 / 2,
						 keyd[KEY_INPTSET_LOGIC].ypis + (keyd[KEY_INPTSET_LOGIC].ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)keybmpind[KEY_INPTSET_LOGIC].nameStr[pbuf[1]] * TitleLengthMax), SheetPage, keybmpind[KEY_INPTSET_LOGIC].nameStr[pbuf[1]], keybmpind[KEY_INPTSET_LOGIC].textColor[0]);
		}
		if (dis_updata3)
		{
			dis_updata3 = 0;

			DrawButtonSingleColor(keyd[KEY_INPTSET_METHOD], keybmpind[KEY_INPTSET_METHOD].border[0], keybmpind[KEY_INPTSET_METHOD].icon[0]);
			Draw24x24Str(keyd[KEY_INPTSET_METHOD].xpis + keyd[KEY_INPTSET_METHOD].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_INPTSET_METHOD].nameStr[pbuf[2]] * TitleLengthMax)) * 12 / 2,
						 keyd[KEY_INPTSET_METHOD].ypis + (keyd[KEY_INPTSET_METHOD].ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)keybmpind[KEY_INPTSET_METHOD].nameStr[pbuf[2]] * TitleLengthMax), SheetPage, keybmpind[KEY_INPTSET_METHOD].nameStr[pbuf[2]], keybmpind[KEY_INPTSET_METHOD].textColor[0]);
		}

		if (ovINlevel != vINlevel)
		{
			// BmpFillRect(16, 84, 32, 32);
			// DrawFPBMPBTN(16, 84, 32, 32, vINlevel ? 569 : 568, 0xffff);
			ovINlevel = vINlevel;
		}

		if ((Timer_ms > 200) && (g_runstep == 1))
		{
			Timer_ms = 0;
			Read_D(D_Address[SYS] + SYS_INLEVEL, 6, (U16 *)(&YREG[SYS_INLEVEL])); // 读输入电平状态
			vINlevel = (YREG[SYS_INLEVEL + vINgroup] >> vINno) & 0x01;
		}

		if (pno != MENU_INPUTSET)
		{
			plastno = MENU_INPUTSET;
			break;
		}
		updateViewStatus();
		wdt();
	}
#endif
}
//---------------------------------------------------
#define KEY_OUPTSET_SCRNSVR 1

#define KEY_OUPTSET_BACK 2
#define KEY_OUPTSET_RESTORE 3
#define KEY_OUPTSET_OUT1 4
#define KEY_OUPTSET_OUT2 5
#define KEY_OUPTSET_OUT3 6
#define KEY_OUPTSET_OUT4 7
#define KEY_OUPTSET_OUT5 8
#define KEY_OUPTSET_OUT6 9
#define KEY_OUPTSET_OUT7 10
#define KEY_OUPTSET_OUT8 11
#define KEY_OUPTSET_OUT9 12
#define KEY_OUPTSET_OUTA 13
#define KEY_OUPTSET_OUTB 14
#define KEY_OUPTSET_OUTC 15
#define KEY_OUPTSET_OUTE1 16
#define KEY_OUPTSET_OUTE2 17
#define KEY_OUPTSET_OUTE3 18
#define KEY_OUPTSET_OUTE4 19

#define KEY_OUPTSET1_BACK 20 //1
#define KEY_OUPTSET1_ENTER 21
#define KEY_OUPTSET1_NOUSE 22
#define KEY_OUPTSET1_SETTING 23
#define KEY_OUPTSET1_UP 24
#define KEY_OUPTSET1_DOWN 25

#define KEY_OUPTSET2_BACK 26 //2
#define KEY_OUPTSET2_ENTER 27
#define KEY_OUPTSET2_PREV 28
#define KEY_OUPTSET2_NEXT 29

#define KEY_OUPTSET2_LOGIC 30
#define KEY_OUPTSET2_METHOD 31

#define KEY_OUPTSET3_CLR 32
#define KEY_OUPTSET3_UP 33
#define KEY_OUPTSET3_DOWN 34
#define KEY_OUPTSET3_0 35
#define KEY_OUPTSET3_1 36
#define KEY_OUPTSET3_2 37
#define KEY_OUPTSET3_3 38
#define KEY_OUPTSET3_4 39
#define KEY_OUPTSET3_5 40
#define KEY_OUPTSET3_6 41
#define KEY_OUPTSET3_7 42
#define KEY_OUPTSET3_8 43
#define KEY_OUPTSET3_9 44
#define KEY_OUPTSET3_MODSLC 45
#define KEY_OUPTSET3_FULL 46
#define KEY_OUPTSET3_LOW 47
#define KEY_OUPTSET3_HIGH 48

#define KEY_OUPTSET4_ONDELYT 49
#define KEY_OUPTSET4_UPON 50
#define KEY_OUPTSET4_DNON 51
#define KEY_OUPTSET4_OFFDELYT 52
#define KEY_OUPTSET4_UPOFF 53
#define KEY_OUPTSET4_DNOFF 54

//********
#define RECT_OUPTSET_STR1 1
#define RECT_OUPTSET_STR2 2
#define RECT_OUPTSET_STR3 3
#define RECT_OUPTSET_STR4 4
#define RECT_OUPTSET_STR5 5
#define RECT_OUPTSET_STR6 6
#define RECT_OUPTSET_STR7 7
#define RECT_OUPTSET_STR8 8
#define RECT_OUPTSET_STR9 9
#define RECT_OUPTSET_STR10 10
#define RECT_OUPTSET_STR11 11
#define RECT_OUPTSET_STR12 12
#define RECT_OUPTSET_STRE1 13
#define RECT_OUPTSET_STRE2 14
#define RECT_OUPTSET_STRE3 15
#define RECT_OUPTSET_STRE4 16

#define RECT_OUPTSET1_TIT 17
#define RECT_OUPTSET1_MSG 18
#define RECT_OUPTSET1_SELAll 19
#define RECT_OUPTSET1_SEL1 20
#define RECT_OUPTSET1_SEL2 21
#define RECT_OUPTSET1_SEL3 22
#define RECT_OUPTSET1_SEL4 23
#define RECT_OUPTSET1_SEL5 24
#define RECT_OUPTSET1_SEL6 25
#define RECT_OUPTSET1_SEL7 26
#define RECT_OUPTSET1_LINE 27

#define RECT_OUPTSET2_TIT 28
#define RECT_OUPTSET2_PAGE 29

#define RECT_OUPTSET2_ITEM1_TIT 30
#define RECT_OUPTSET2_ITEM1_MSG 31
#define RECT_OUPTSET2_ITEM2_TIT 32
#define RECT_OUPTSET2_ITEM2_MSG 33

#define RECT_OUPTSET3_ITEM1_TIT 34
#define RECT_OUPTSET3_ITEM1_MSG 35
#define RECT_OUPTSET3_ITEM2_ALL 36
#define RECT_OUPTSET3_ITEM2_TIT 37
#define RECT_OUPTSET3_ITEM2_MSG 38
#define RECT_OUPTSET3_ITEM2_PARA 39
#define RECT_OUPTSET3_ITEM3_ALL 40
#define RECT_OUPTSET3_ITEM3_TIT 41
#define RECT_OUPTSET3_ITEM3_MSG 42
#define RECT_OUPTSET3_ITEM3_PARA 43
#define RECT_OUPTSET3_ITEM4_ALL 44
#define RECT_OUPTSET3_ITEM4_TIT 45
#define RECT_OUPTSET3_ITEM4_MSG 46
#define RECT_OUPTSET3_ITEM4_PARA 47

#define RECT_OUPTSET4_ITEM1_TIT 48
#define RECT_OUPTSET4_ITEM1_MSG 49
#define RECT_OUPTSET4_ITEM1_PARA 50
#define RECT_OUPTSET4_ITEM2_TIT 51
#define RECT_OUPTSET4_ITEM2_MSG 52
#define RECT_OUPTSET4_ITEM2_PARA 53

//------------------------------------------
// Function   : void Menu_Outputset(void)
// Description: 输出信号设置
//------------------------------------------
void Menu_Outputset(void)
{
#if 1
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U8 dis_updata = 1,	  //页面切换初始化
		dis_updata1 = 0,  //选择添加项目
		dis_updata2 = 0,  //修改逻辑按钮显示
		dis_updata3 = 0,  //修改方式按钮显示
		dis_updata4 = 0,  //修改脉冲按钮显示
		dis_updata5 = 0,  //修改脉冲数据选项显示
		dis_updata6 = 0,  //修改脉冲数据选项显示的数据
		dis_updata7 = 0,  //修改开启延迟按钮
		dis_updata8 = 0,  //修改开启延迟数据选项
		dis_updata9 = 0,  //修改开启延迟数据
		dis_updata10 = 0, //修改关闭延迟按钮
		dis_updata11 = 0, //修改关闭延迟数据选项
		dis_updata12 = 0, //修改关闭延迟数据
		dis_updata13 = 0; //换页按钮刷新
	U8 tmprunstep = 0xff;
	U32 i, j;
	// U8 a, b;
	U8 selItemS3 = 3;
	//U8 editflag[24];
	U8 selNoSum = OUTSETPRSUM;
	U16 Iosetnamepos;
	U16 Contentmsgpos; //0     1     2     3     4     5     6           7           8         9         10
	U16 pbuf[11];	   //内容、逻辑、方式、脉冲、开启、关闭、高电平时间、低电平时间、全波时间、启动延时、关闭延时
	U16 checksum;
	U8 PressDN = 0, Presscnt = 0;
	U8 pageNum = 3, pageNow = 1;
	S32 dat = 0;
	U16 Outset_Sheet = 53;
	U16 Contentmsg_Sheet = 54;
	U8 vOUTgroup = 0, vOUTno = 0, vOUTlevel = 0, ovOUTlevel = 0;
	// U8 dp = 0;

	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32}, //1 屏保

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},									 //2 返回0
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},								 //3 还原
		{TYPE_X35, TYPE_Y3, KEY_X11, KEY_Y11},								 //4 选项1
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //5 选项2
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //6 选项3
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //7 选项4
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //8 选项5
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //9 选项6
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //10 选项7
		{TYPE_X35, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //11 选项8
		{TYPE_X53, TYPE_Y3, KEY_X11, KEY_Y11},								 //12 选项9
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, KEY_X11, KEY_Y11}, //13 选项10
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, KEY_X11, KEY_Y11}, //14 选项11
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, KEY_X11, KEY_Y11}, //15 选项12
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, KEY_X11, KEY_Y11}, //16 选项13
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, KEY_X11, KEY_Y11}, //17 选项14
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, KEY_X11, KEY_Y11}, //18 选项15
		{TYPE_X53, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, KEY_X11, KEY_Y11}, //19 选项16

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},				  //20 返回1
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},			  //21 确认1
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},			  //22 回到NO
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},				  //23 详细设定
		{480, 215, KEY_X1, KEY_Y1},						  //24 上翻
		{480, 215 + KEY_Y1 + SAFE_DIST1, KEY_X1, KEY_Y1}, //25 下翻

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //26 返回1
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //27 确认1
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},	 //28 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},	 //29 下一页

		{TYPE_X14, 111, KEY_X13, KEY_Y13}, //30 逻辑按钮
		{TYPE_X14, 190, KEY_X13, KEY_Y13}, //31 交替开关

		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 3, TYPE_Y54, KEY_X2, KEY_Y2},							  //32 清除一位
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 1, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //33 加1
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //34 减1
		{TYPE_X54, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},							  //35 0
		{TYPE_X54, TYPE_Y54, KEY_X2, KEY_Y2},														  //36 1
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 1, TYPE_Y54, KEY_X2, KEY_Y2},							  //37 2
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y54, KEY_X2, KEY_Y2},							  //38 3
		{TYPE_X54, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 1, KEY_X2, KEY_Y2},							  //39 4
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 1, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 1, KEY_X2, KEY_Y2}, //40 5
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 1, KEY_X2, KEY_Y2}, //41 6
		{TYPE_X54, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},							  //42 7
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 1, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //43 8
		{TYPE_X54 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y54 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //44 9
		{TYPE_X14, 111, KEY_X14, KEY_Y14},															  //45 状态选择
		{TYPE_X14 + 150, TYPE_Y54 + TYPE_SIZE6, TYPE_SIZE14, 28},									  //46 全波时间
		{TYPE_X14 + 150, TYPE_Y54 + TYPE_SIZE6 + 71 * 2, TYPE_SIZE14, 28},							  //47 脉冲关闭
		{TYPE_X14 + 150, TYPE_Y54 + TYPE_SIZE6 + 71 * 1, TYPE_SIZE14, 28},							  //48 脉冲开启

		{TYPE_X14, 111, KEY_X13, KEY_Y13}, //49 启用延时
		{307, 135, KEY_X2, KEY_Y2},		   //50 上翻4
		{645, 135, KEY_X2, KEY_Y2},		   //51 下翻4
		{TYPE_X14, 223, KEY_X13, KEY_Y13}, //52 关闭延时
		{307, 247, KEY_X2, KEY_Y2},		   //53 上翻4
		{645, 247, KEY_X2, KEY_Y2},		   //54 下翻4

	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {TLFNADD_IOALL + 1, 0, 0, 0}, {0}},										//1 屏保
		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 2, 0, 0, 0}, {0}},										//2	返回0
		{{KEY_Silver}, {0}, {236}, {TLFNADD_IOALL + 130, 0, 0, 0}, {0}},									//3	还原
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 10, 0, 0, 0}, {TLFNADD_IOALL + 10}},	//4	选项1
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 11, 0, 0, 0}, {TLFNADD_IOALL + 11}},	//5	选项2
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 12, 0, 0, 0}, {TLFNADD_IOALL + 12}},	//6	选项3
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 13, 0, 0, 0}, {TLFNADD_IOALL + 13}},	//7	选项4
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 14, 0, 0, 0}, {TLFNADD_IOALL + 14}},	//8	选项5
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 15, 0, 0, 0}, {TLFNADD_IOALL + 15}},	//9	选项6
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 16, 0, 0, 0}, {TLFNADD_IOALL + 16}},	//10 选项7
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 17, 0, 0, 0}, {TLFNADD_IOALL + 17}},	//11 选项8
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 18, 0, 0, 0}, {TLFNADD_IOALL + 18}},	//12 选项9
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 19, 0, 0, 0}, {TLFNADD_IOALL + 19}},	//13 选项10
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 20, 0, 0, 0}, {TLFNADD_IOALL + 20}},	//14 选项11
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 21, 0, 0, 0}, {TLFNADD_IOALL + 21}},	//15 选项12
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 118, 0, 0, 0}, {TLFNADD_IOALL + 118}}, //16 选项13
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 119, 0, 0, 0}, {TLFNADD_IOALL + 119}}, //17 选项14
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 120, 0, 0, 0}, {TLFNADD_IOALL + 120}}, //18 选项15
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_IOALL + 121, 0, 0, 0}, {TLFNADD_IOALL + 121}}, //19 选项16

		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 67, 0, 0, 0}, {0}},		 //20 返回1
		{{KEY_Silver}, {0}, {71}, {TLFNADD_IOALL + 68, 0, 0, 0}, {0}},		 //21 确认1
		{{KEY_Silver}, {0}, {111, 112}, {TLFNADD_IOALL + 71, 0, 0, 0}, {0}}, //22 回到NO
		{{KEY_Silver}, {0}, {239}, {TLFNADD_IOALL + 72, 0, 0, 0}, {0}},		 //23 详细设定
		{{KEY_Silver}, {0}, {219, 220}, {TLFNADD_IOALL + 69, 0, 0, 0}, {0}}, //24 上翻
		{{KEY_Silver}, {0}, {221, 222}, {TLFNADD_IOALL + 70, 0, 0, 0}, {0}}, //25 下翻

		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 67, 0, 0, 0}, {0}},				   //26 返回2
		{{KEY_Silver}, {0}, {71}, {TLFNADD_IOALL + 68, 0, 0, 0}, {0}},				   //27 确认2
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {TLFNADD_IOALL + 89, 0, 0, 0}, {0}}, //28 上一页2
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {TLFNADD_IOALL + 90, 0, 0, 0}, {0}}, //29 下一页2

		{{KEY_Silver, KEY_Blue}, {Color_Blue, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_IOALL + 77, TLFNADD_IOALL + 75, 0, 0}, {TLFNADD_IOALL + 77, TLFNADD_IOALL + 75}}, //30 逻辑按钮
		{{KEY_Silver, KEY_Blue}, {Color_Blue, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_IOALL + 78, TLFNADD_IOALL + 76, 0, 0}, {TLFNADD_IOALL + 78, TLFNADD_IOALL + 76}}, //31 操作开关

		{{KEY_Silver, KEY_Gray}, {0}, {113, 114}, {TLFNADD_IOALL + 91, 0, 0, 0}, {0}},															 //32 清除
		{{KEY_Silver, KEY_Gray}, {0}, {199, 200}, {TLFNADD_IOALL + 92, 0, 0, 0}, {0}},															 //33 上翻
		{{KEY_Silver, KEY_Gray}, {0}, {201, 201}, {TLFNADD_IOALL + 93, 0, 0, 0}, {0}},															 //34 下翻
		{{KEY_Silver, KEY_Gray}, {0}, {91, 101}, {10, 0, 0, 0}, {0}},																			 //35 0
		{{KEY_Silver, KEY_Gray}, {0}, {92, 102}, {11, 0, 0, 0}, {0}},																			 //36 1
		{{KEY_Silver, KEY_Gray}, {0}, {93, 103}, {12, 0, 0, 0}, {0}},																			 //37 2
		{{KEY_Silver, KEY_Gray}, {0}, {94, 104}, {13, 0, 0, 0}, {0}},																			 //38 3
		{{KEY_Silver, KEY_Gray}, {0}, {95, 105}, {14, 0, 0, 0}, {0}},																			 //39 4
		{{KEY_Silver, KEY_Gray}, {0}, {96, 106}, {15, 0, 0, 0}, {0}},																			 //40 5
		{{KEY_Silver, KEY_Gray}, {0}, {97, 107}, {16, 0, 0, 0}, {0}},																			 //41 6
		{{KEY_Silver, KEY_Gray}, {0}, {98, 108}, {17, 0, 0, 0}, {0}},																			 //42 7
		{{KEY_Silver, KEY_Gray}, {0}, {99, 109}, {18, 0, 0, 0}, {0}},																			 //43 8
		{{KEY_Silver, KEY_Gray}, {0}, {100, 110}, {19, 0, 0, 0}, {0}},																			 //44 9
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_CONTENT + 7, TLFNADD_CONTENT + 15, 0}, {TLFNADD_CONTENT + 7, TLFNADD_CONTENT + 15}}, //45 状态开关
		{{Color_Black, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {TLFNADD_IOALL + 86, 0, 0, 0}, {0}},			 //46 全波输出时间
		{{Color_Black, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {TLFNADD_IOALL + 85, 0, 0, 0}, {0}},			 //47 脉冲关闭
		{{Color_Black, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {TLFNADD_IOALL + 84, 0, 0, 0}, {0}},			 //48 脉冲开启

		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_IOALL + 95, TLFNADD_IOALL + 102, 0}, {TLFNADD_IOALL + 95, TLFNADD_IOALL + 102}}, //49 启用
		{{KEY_Silver}, {0}, {199, 200}, {TLFNADD_IOALL + 98, 0, 0, 0}, {0}},																 //50 上翻4
		{{KEY_Silver}, {0}, {201, 201}, {TLFNADD_IOALL + 99, 0, 0, 0}, {0}},																 //51 下翻4
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_IOALL + 96, TLFNADD_IOALL + 103, 0}, {TLFNADD_IOALL + 96, TLFNADD_IOALL + 103}}, //52 关闭
		{{KEY_Silver}, {0}, {199, 200}, {TLFNADD_IOALL + 100, 0, 0, 0}, {0}},																 //53 上翻5
		{{KEY_Silver}, {0}, {201, 201}, {TLFNADD_IOALL + 101, 0, 0, 0}, {0}},																 //54 下翻5
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_IOALL + 36}},

		{TYPE_X35 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //1 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //2 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //3 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //4 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //5 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //6 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //7 IO号显示框
		{TYPE_X35 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //8 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}},								  //9 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 1, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //10 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 2, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //11 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 3, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //12 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 4, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //13 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 5, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //14 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 6, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //15 IO号显示框
		{TYPE_X53 + KEY_X11, TYPE_Y3 + (KEY_Y11 + SAFE_DIST15) * 7, TYPE_SIZE32, KEY_Y11, {Color_Black}, {Color_Black}, {0}}, //16 IO号显示框

		{TYPE_X14, TYPE_Y2, TYPE_SIZE33, TYPE_SIZE5, {Color_MidSkyBlue}, {Color_Black}, {TLFNADD_IOALL + 82}},						 //17 输出选择信号框/详细设定框
		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE33, TYPE_SIZE12 + 2, {Color_Black, Color_White}, {Color_Black}, {0}}, //18 文本显示框
		{340, 170, TYPE_SIZE15, TYPE_SIZE5 * 7 + SAFE_DIST2 * 2, {Color_Black, Color_White}, {Color_Black}, {0}},					 //19 参数框-大
		{340 + SAFE_DIST2, 170 + SAFE_DIST2, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_Gray}, {Color_Black}, {0}},						 //20 参数框-1
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_DarkGray}, {Color_Black}, {0}},		 //21 参数框-2
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 2, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {0}},	 //22 参数框-3
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 3, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}}, //23 参数框-4
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 4, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {0}},	 //24 参数框-5
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 5, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_DarkGray}, {Color_Black}, {0}},	 //25 参数框-6
		{340 + SAFE_DIST2, 170 + SAFE_DIST2 + TYPE_SIZE5 * 6, TYPE_SIZE15 - 2, TYPE_SIZE5, {Color_Gray}, {Color_Black}, {0}},		 //26 参数框-7
		{240, 284, 100, 1, {Color_Black}, {Color_Black}, {TLFNADD_IOALL + 10, TLFNADD_IOALL + 118}},								 //27 IO号显示线

		{TYPE_X14, TYPE_Y2, TYPE_SIZE33, TYPE_SIZE5, {Color_MidSkyBlue}, {Color_Black}, {TLFNADD_IOALL + 72}}, //28 输出选择信号框/详细设定框
		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //29 换页提示

		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 80}},											   //30 设定框-1-标题
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, 395, TYPE_SIZE10 + 1, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 2}},					   //31 设定框-1-提示文本
		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5 * 2 + TYPE_SIZE10 + 1, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 81}},						   //32 设定框-2-标题
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5 * 2 + TYPE_SIZE10 + 1, 395, TYPE_SIZE10 + 1, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 3}}, //33 设定框-2-提示文本

		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 83}},							 //34 设定框-3-标题
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, 395, 105, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 4}},				 //35 设定框-3-提示文本
		{TYPE_X14, TYPE_Y54, 389, 66, {Color_Black}, {Color_Black}, {0}},																						 //36 设定框-3-按钮1-整体
		{TYPE_X14 + 2, TYPE_Y54 + 2, 385, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 86}},													 //37 设定框-3-按钮1-标题
		{TYPE_X14 + 2, TYPE_Y54 + 2 + TYPE_SIZE5, 385, 28, {Color_Black}, {Color_Black}, {0}},																	 //38 设定框-3-按钮1-提示文本
		{TYPE_X14 + 150, TYPE_Y54 + 4 + TYPE_SIZE5, TYPE_SIZE14, 28, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},						 //39 设定框-3-按钮1-参数
		{TYPE_X14, TYPE_Y54 + (66 + SAFE_DIST5), 389, 66, {Color_Black}, {Color_Black}, {0}},																	 //40 设定框-3-按钮2-整体
		{TYPE_X14 + 2, TYPE_Y54 + (66 + SAFE_DIST5) + 2, 385, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 84}},								 //41 设定框-3-按钮2-标题
		{TYPE_X14 + 2, TYPE_Y54 + (66 + SAFE_DIST5) + 2 + TYPE_SIZE5, 385, 28, {Color_Black}, {Color_Black}, {0}},												 //42 设定框-3-按钮2-提示文本
		{TYPE_X14 + 150, TYPE_Y54 + (66 + SAFE_DIST5) + 4 + TYPE_SIZE5, TYPE_SIZE14, 28, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},	 //43 设定框-3-按钮2-参数
		{TYPE_X14, TYPE_Y54 + (66 + SAFE_DIST5) * 2, 389, 66, {Color_Black}, {Color_Black}, {0}},																 //44 设定框-3-按钮3-整体
		{TYPE_X14 + 2, TYPE_Y54 + (66 + SAFE_DIST5) * 2 + 2, 385, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 85}},							 //45 设定框-3-按钮3-标题
		{TYPE_X14 + 2, TYPE_Y54 + (66 + SAFE_DIST5) * 2 + 2 + TYPE_SIZE5, 385, 28, {Color_Black}, {Color_Black}, {0}},											 //46 设定框-3-按钮3-提示文本
		{TYPE_X14 + 150, TYPE_Y54 + (66 + SAFE_DIST5) * 2 + 4 + TYPE_SIZE5, TYPE_SIZE14, 28, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //47 设定框-3-按钮3-参数

		{TYPE_X14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 104}},															 //48 设定框-4-标题
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5, 395, 107, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 5}},												 //49 设定框-4-提示文本
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5 + TYPE_SIZE14, TYPE_Y2 + TYPE_SIZE5 + SAFE_DIST5 + 66, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //50 设定框-4-参数
		{TYPE_X14, 190, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_IOALL + 105}},																						 //51 设定框-5-标题
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5, 190, 395, 107, {Color_Black}, {Color_Black}, {TLFNADD_CONTENT + 6}},																				 //52 设定框-5-提示文本
		{TYPE_X14 + TYPE_SIZE18 + SAFE_DIST5 + TYPE_SIZE14, 190 + 66, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},								 //53 设定框-5-参数

	};
	if (P_LAN)
	{
		Iosetnamepos = IOSETNAME_ENGLISH;
		Contentmsgpos = CONTENTMSG_ENGLISH;
	}
	else
	{
		Iosetnamepos = IOSETNAME_CHINA;
		Contentmsgpos = CONTENTMSG_CHINA;
	}
	// if (P_LAN == 2)
	// { //越南语
	// 	dp = 16;
	// }

	if (g_NO_ReagIOf == 0)
	{
		Read_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG)); //读取IO参数
		if ((IOREG[0] != 0x5aa5) && (IOREG[0] != 0x5a5a))	   //通信故障
		{
			pno = MENU_RWHELP;
			hleppara = 15;
		}
	}

	Timer_ms = 0;
	Read_D(D_Address[SYS] + SYS_OUTLEVEL, 6, (U16 *)(&YREG[SYS_OUTLEVEL])); // 读输出电平状态

	while (1)
	{
		if (dis_updata)
		{
			dis_updata = 0;

			if (tmprunstep != g_runstep)
			{
				SetLanguage(MENU_IOTEST);
				InitScreen();
				initViewStatus();
				updateViewStatus();
				Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
			}
			if (g_runstep == 0)
			{
				DrawButton(keyd[KEY_OUPTSET_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_OUPTSET_BACK].border[0], keybmpind[KEY_OUPTSET_BACK].icon[0]);
				DrawButton(keyd[KEY_OUPTSET_RESTORE], (U16 *)(ADD_ICONBMP), keybmpind[KEY_OUPTSET_RESTORE].border[0], keybmpind[KEY_OUPTSET_RESTORE].icon[0]);

				for (i = KEY_OUPTSET_OUT1; i <= KEY_OUPTSET_OUTE4; i++)
				{
					DrawButtonSingleColor(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
					Draw24x24Str(keyd[i].xpis + keyd[i].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax)) * 12 / 2,
								 keyd[i].ypis + (keyd[i].ysize - 24) / 2,
								 (MEM_TITLE_ADD + (U32)keybmpind[i].nameStr[0] * TitleLengthMax), SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[0]);
				}

				for (i = RECT_OUPTSET_STR1; i <= RECT_OUPTSET_STRE4; i++)
				{
					DrawLine(rect[i].xpis, rect[i].ypis + rect[i].ysize, rect[i].xpis + rect[i].xsize, rect[i].ypis + rect[i].ysize, rect[i].colorB[0]);
				}

				for (i = RECT_OUPTSET_STR1; i <= RECT_OUPTSET_STRE4; i++)
				{
					DrawFFStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST7,
							  (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSET_STR1) * OUTSETREGLEN + OUTSETREGI] + IOSETNAME_CODE), rect[i].colorT[0]);
					if ((strlen((char *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSET_STR1) * OUTSETREGLEN + OUTSETREGI] + Iosetnamepos))) > 18)
					{
						DrawFMultiLStr16(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis - SAFE_DIST4,
										 (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * (IOREG[(i - RECT_OUPTSET_STR1) * OUTSETREGLEN + OUTSETREGI]) + Iosetnamepos),
										 18, 2, rect[i].index[0]);
					}
					else
					{
						DrawFFStr(rect[i].xpis + SAFE_DIST4 + SAFE_DIST11, rect[i].ypis + SAFE_DIST7,
								  (U16 *)(MEM_INSETNAME + IOSETNAME_LENGTH * IOREG[(i - RECT_OUPTSET_STR1) * OUTSETREGLEN + OUTSETREGI] + Iosetnamepos), rect[i].colorT[0]);
					}
				}
			}
			else if (g_runstep == 1)
			{

				DrawFillRect(rect[RECT_OUPTSET1_TIT].xpis, rect[RECT_OUPTSET1_TIT].ypis, rect[RECT_OUPTSET1_TIT].xsize, rect[RECT_OUPTSET1_TIT].ysize, rect[RECT_OUPTSET1_TIT].colorB[0]);
				Draw24x24Str(rect[RECT_OUPTSET1_TIT].xpis + SAFE_DIST4, rect[RECT_OUPTSET1_TIT].ypis + SAFE_DIST4,
							 (MEM_TITLE_ADD + (U32)rect[RECT_OUPTSET1_TIT].index[0] * TitleLengthMax), SheetPage, rect[RECT_OUPTSET1_TIT].index[0], rect[RECT_OUPTSET1_TIT].colorT[0]); //输入讯号选择
				DrawRect(rect[RECT_OUPTSET1_MSG].xpis, rect[RECT_OUPTSET1_MSG].ypis, rect[RECT_OUPTSET1_MSG].xsize, rect[RECT_OUPTSET1_MSG].ysize, rect[RECT_OUPTSET1_MSG].colorB[0]);
				DrawRect(rect[RECT_OUPTSET1_SELAll].xpis, rect[RECT_OUPTSET1_SELAll].ypis, rect[RECT_OUPTSET1_SELAll].xsize, rect[RECT_OUPTSET1_SELAll].ysize, rect[RECT_OUPTSET1_SELAll].colorB[0]);
				for (i = KEY_OUPTSET1_BACK; i <= KEY_OUPTSET1_DOWN; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}

				DrawHLine(rect[RECT_OUPTSET1_LINE].xpis, rect[RECT_OUPTSET1_LINE].ypis, rect[RECT_OUPTSET1_LINE].xsize, rect[RECT_OUPTSET1_LINE].colorT[0]); //画水平实线
				if (g_slcnum < 13)																															 //显示IO号
				{
					U8 tmpIndex = rect[RECT_OUPTSET1_LINE].index[0] + g_slcnum - 1;
					disIOname(rect[RECT_OUPTSET1_LINE].xpis - SAFE_DIST11, rect[RECT_OUPTSET1_LINE].ypis - SAFE_DIST1, (MEM_TITLE_ADD + (U32)(tmpIndex)*TitleLengthMax),
							  2, SheetPage, tmpIndex, rect[RECT_OUPTSET1_LINE].colorT[0]);
				}
				else
				{
					U8 tmpIndex = rect[RECT_OUPTSET1_LINE].index[1] + g_slcnum - 13;
					disIOname(rect[RECT_OUPTSET1_LINE].xpis - SAFE_DIST11, rect[RECT_OUPTSET1_LINE].ypis - SAFE_DIST1, (MEM_TITLE_ADD + (U32)(tmpIndex)*TitleLengthMax),
							  3, SheetPage, tmpIndex, rect[RECT_OUPTSET1_LINE].colorT[0]);
				}

				dis_updata1 = 1;
			}
			else if (g_runstep == 2)
			{
				U8 tmpIndex = 0;
				DrawFillRect(rect[RECT_OUPTSET2_TIT].xpis, rect[RECT_OUPTSET2_TIT].ypis, rect[RECT_OUPTSET2_TIT].xsize, rect[RECT_OUPTSET2_TIT].ysize, rect[RECT_OUPTSET2_TIT].colorB[0]);
				Draw24x24Str(rect[RECT_OUPTSET2_TIT].xpis + SAFE_DIST4, rect[RECT_OUPTSET2_TIT].ypis + SAFE_DIST4,
							 (MEM_TITLE_ADD + (U32)rect[RECT_OUPTSET2_TIT].index[0] * TitleLengthMax), SheetPage, rect[RECT_OUPTSET2_TIT].index[0], rect[RECT_OUPTSET2_TIT].colorT[0]); //详细设定
				for (i = KEY_OUPTSET2_BACK; i <= KEY_OUPTSET2_ENTER; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}
				for (i = 0; i < 2; i++)
				{
					tmpIndex = 2 * i + RECT_OUPTSET2_ITEM1_TIT;
					DrawEditTit(rect[tmpIndex], rect[tmpIndex].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST4);
					DrawRect(rect[tmpIndex + 1].xpis, rect[tmpIndex + 1].ypis, rect[tmpIndex + 1].xsize, rect[tmpIndex + 1].ysize, rect[tmpIndex + 1].colorB[0]);
					Draw24x24MulStr(rect[tmpIndex + 1].xpis + SAFE_DIST4, rect[tmpIndex + 1].ypis + SAFE_DIST4,
									(MEM_CONTENTMSG + (U32)(CONTENTMSG_LENGTH * rect[tmpIndex + 1].index[0]) + Contentmsgpos), 30, Contentmsg_Sheet, rect[tmpIndex + 1].index[0], 0, rect[tmpIndex + 1].colorT[0]);
				}

				dis_updata2 = 1;
				dis_updata3 = 1;
				dis_updata13 = 1;
			}
			else if (g_runstep == 3)
			{
				U8 tmpIndex = 0;
				DrawFillRect(rect[RECT_OUPTSET2_TIT].xpis, rect[RECT_OUPTSET2_TIT].ypis, rect[RECT_OUPTSET2_TIT].xsize, rect[RECT_OUPTSET2_TIT].ysize, rect[RECT_OUPTSET2_TIT].colorB[0]);
				Draw24x24Str(rect[RECT_OUPTSET2_TIT].xpis + SAFE_DIST4, rect[RECT_OUPTSET2_TIT].ypis + SAFE_DIST4,
							 (MEM_TITLE_ADD + (U32)rect[RECT_OUPTSET2_TIT].index[0] * TitleLengthMax), SheetPage, rect[RECT_OUPTSET2_TIT].index[0], rect[RECT_OUPTSET2_TIT].colorT[0]); //详细设定
				for (i = KEY_OUPTSET2_BACK; i <= KEY_OUPTSET2_ENTER; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}
				for (i = 0; i < 1; i++)
				{
					tmpIndex = 2 * i + RECT_OUPTSET3_ITEM1_TIT;
					DrawEditTit(rect[tmpIndex], rect[tmpIndex].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST4);
					DrawRect(rect[tmpIndex + 1].xpis, rect[tmpIndex + 1].ypis, rect[tmpIndex + 1].xsize, rect[tmpIndex + 1].ysize, rect[tmpIndex + 1].colorB[0]);
					Draw24x24MulStr(rect[tmpIndex + 1].xpis + SAFE_DIST4, rect[tmpIndex + 1].ypis + SAFE_DIST4,
									(MEM_CONTENTMSG + (U32)(CONTENTMSG_LENGTH * rect[tmpIndex + 1].index[0]) + Contentmsgpos), 30, Contentmsg_Sheet, rect[tmpIndex + 1].index[0], 0, rect[tmpIndex + 1].colorT[0]);
				}

				dis_updata4 = 1;
				dis_updata5 = 1;
				dis_updata13 = 1;
			}
			else if (g_runstep == 4)
			{
				U8 tmpIndex = 0;
				DrawFillRect(rect[RECT_OUPTSET2_TIT].xpis, rect[RECT_OUPTSET2_TIT].ypis, rect[RECT_OUPTSET2_TIT].xsize, rect[RECT_OUPTSET2_TIT].ysize, rect[RECT_OUPTSET2_TIT].colorB[0]);
				Draw24x24Str(rect[RECT_OUPTSET2_TIT].xpis + SAFE_DIST4, rect[RECT_OUPTSET2_TIT].ypis + SAFE_DIST4,
							 (MEM_TITLE_ADD + (U32)rect[RECT_OUPTSET2_TIT].index[0] * TitleLengthMax), SheetPage, rect[RECT_OUPTSET2_TIT].index[0], rect[RECT_OUPTSET2_TIT].colorT[0]); //详细设定
				for (i = KEY_OUPTSET2_BACK; i <= KEY_OUPTSET2_ENTER; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}
				for (i = 0; i < 2; i++)
				{
					tmpIndex = 3 * i + RECT_OUPTSET4_ITEM1_TIT;
					DrawEditTit(rect[tmpIndex], rect[tmpIndex].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST4);
					DrawRect(rect[tmpIndex + 1].xpis, rect[tmpIndex + 1].ypis, rect[tmpIndex + 1].xsize, rect[tmpIndex + 1].ysize, rect[tmpIndex + 1].colorB[0]);
					Draw24x24MulStr(rect[tmpIndex + 1].xpis + SAFE_DIST4, rect[tmpIndex + 1].ypis + SAFE_DIST4,
									(MEM_CONTENTMSG + (U32)(CONTENTMSG_LENGTH * rect[tmpIndex + 1].index[0]) + Contentmsgpos), 30, Contentmsg_Sheet, rect[tmpIndex + 1].index[0], 0, rect[tmpIndex + 1].colorT[0]);
				}

				dis_updata7 = 1;
				dis_updata8 = 1;
				dis_updata10 = 1;
				dis_updata11 = 1;
				dis_updata13 = 1;
			}
			if (tmprunstep != g_runstep)
			{
				tmprunstep = g_runstep;
				ViewPage(usepageno);
			}
		}

		keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET_SCRNSVR + 1, 0);
		if (keydata == 0)
		{
			if (g_runstep == 0)
			{
				keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET_OUTE4 + 1, KEY_OUPTSET_BACK);
			}
			else if (g_runstep == 1)
			{
				keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET1_DOWN + 1, KEY_OUPTSET1_BACK);
			}
			else if ((g_runstep == 2 || g_runstep == 3 || g_runstep == 4))
			{
				keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET2_NEXT + 1, KEY_OUPTSET2_BACK);
				if (keydata == 0)
				{
					if (g_runstep == 2)
					{
						keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET2_METHOD + 1, KEY_OUPTSET2_LOGIC);
					}
					else if (g_runstep == 3)
					{
						keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET3_HIGH + 1, KEY_OUPTSET3_CLR);
						if ((keydata >= KEY_OUPTSET3_CLR && keydata <= KEY_OUPTSET3_9))
						{
							if (pbuf[3] == 0)
							{
								keydata = 0;
								resetKeyData();
							}
						}
						else if ((keydata >= KEY_OUPTSET3_FULL && keydata <= KEY_OUPTSET3_HIGH))
						{
							if (pbuf[3] != 8)
							{
								keydata = 0;
								resetKeyData();
							}
						}
					}
					else if (g_runstep == 4)
					{
						keydata = get_keydata((KEYPAD *)keyd, KEY_OUPTSET4_DNOFF + 1, KEY_OUPTSET4_ONDELYT);
						if (((keydata == KEY_OUPTSET4_UPON) || (keydata == KEY_OUPTSET4_DNON)))
						{
							if (pbuf[4] == 0)
							{
								keydata = 0;
								resetKeyData();
							}
						}
						else if (((keydata == KEY_OUPTSET4_UPOFF) || (keydata == KEY_OUPTSET4_DNOFF)))
						{
							if (pbuf[5] == 0)
							{
								keydata = 0;
								resetKeyData();
							}
						}
					}
				}
				else
				{
					if (keydata == KEY_OUPTSET2_PREV && pageNow == 1)
					{
						keydata = 0;
						resetKeyData();
					}
					if (keydata == KEY_OUPTSET2_NEXT && pageNow == pageNum)
					{
						keydata = 0;
						resetKeyData();
					}
				}
			}
		}

		if (keydata == 0xff)
		{
			if ((keyon == KEY_OUPTSET1_UP) || (keyon == KEY_OUPTSET1_DOWN) ||
				(keyon == KEY_OUPTSET3_UP) || (keyon == KEY_OUPTSET3_DOWN) ||
				(keyon == KEY_OUPTSET4_UPON) || (keyon == KEY_OUPTSET4_DNON) ||
				(keyon == KEY_OUPTSET4_UPOFF) || (keyon == KEY_OUPTSET4_DNOFF))
			{
			}
			else if ((keyon) && (beeponf))
			{
				keyoff = keyon; //按键键值传给松开键值
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理
		{
			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_OUPTSET_SCRNSVR)
				{
				}
				else if (keyon >= KEY_OUPTSET3_FULL && keyon <= KEY_OUPTSET3_HIGH)
				{
					DrawButtonOnZoomIn(keyd[keyon], 2);
				}
				else
				{
					DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]); //常规按键特效
				}

				Times = 0;
				delay = KEYDT; //按键防抖次数
			}
			else if (keyon != keydata) //已有按键按下，不处理新按键信息
			{
				keydata = 0;
			}
			if (Times > delay) //按键防抖
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);
					switch (keyon) //按键按下标题提示语显示
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
				if ((keyon == KEY_OUPTSET1_UP) || (keyon == KEY_OUPTSET1_DOWN) ||
					(keyon == KEY_OUPTSET3_UP) || (keyon == KEY_OUPTSET3_DOWN) ||
					(keyon == KEY_OUPTSET4_UPON) || (keyon == KEY_OUPTSET4_DNON) ||
					(keyon == KEY_OUPTSET4_UPOFF) || (keyon == KEY_OUPTSET4_DNOFF))
				{
					PressDN = 1;
					keyoff = keyon;
				}
			}
		}
		else
		{
			key_on_flag1 = 0;
		}
		if (keyoff)
		{
			if ((keyoff >= KEY_OUPTSET_OUT1) && (keyoff <= KEY_OUPTSET_OUTE4)) //输入口 + 扩展输入
			{
				U8 tmpIndex = 0;
				g_slcnum = keyoff - KEY_OUPTSET_OUT1 + 1;
				tmpIndex = (g_slcnum - 1) * OUTSETREGLEN;
				for (j = 0; j < OUTSETREGLEN; j++, tmpIndex++)
				{
					pbuf[j] = IOREG[tmpIndex + OUTSETREGI];
				}

				dis_updata = 1;
				g_runstep = 1;
			}
			// if ((keyoff >= KEY_OUPTSET_OUT1) && (keyoff <= KEY_OUPTSET_OUTC))
			// {
			// 	g_slcnum = keyoff - KEY_OUPTSET_RESTORE;
			// 	dis_updata = 1;
			// 	g_runstep = 1;
			// }
			// else if ((keyoff >= KEY_OUPTSET_OUTE1) && (keyoff <= KEY_OUPTSET_OUTE4))
			// { //扩展输出
			// 	g_slcnum = keyoff - KEY_OUPTSET_RESTORE;
			// 	dis_updata = 1;
			// 	g_runstep = 1;
			// }
			else if ((keyoff >= KEY_OUPTSET3_0) && (keyoff < KEY_OUPTSET3_9))
			{
				if (selItemS3 < 3)
				{
					i = pbuf[6 + selItemS3];
					i = (i % 10000) * 10 + keyoff - KEY_OUPTSET3_0;
					if (i > 65535)
						i = (i % 10000);
					pbuf[6 + selItemS3] = i;
					dis_updata6 = 1;
				}
			}
			else
			{
				switch (keyoff)
				{
				case KEY_OUPTSET_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_OUPTSET_BACK:
					pno = MENU_OUTPUTSTATUS;
					pnoindex--;
					if (g_user.newmode == 2)
					{
						g_user.newuser = g_user.olduser;
						g_user.newmode = g_user.oldmode;
						g_user.olduser = 0;
						g_user.oldmode = 0;
					}

					checksum = 0;
					for (i = 2; i < IOSETREGI; i++)
					{ //校验
						checksum += IOREG[i];
					}
					IOREG[1] = checksum;
					if (Write_D(D_Address[IOPARA], IOSETREGI, (U16 *)(&IOREG[0])) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						break;
					}

					if (Write_Cmd(C_SAVEIO, 0, 0) == 0)
					{ //发送保存指令
						pno = MENU_RWHELP;
						hleppara = 16;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						break;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						break;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{

						pno = MENU_RWHELP;
						hleppara = 16;
						poldno[pnoindex++] = MENU_OUTPUTSTATUS;
						break;
					}
					break;
				case KEY_OUPTSET1_BACK:
					dis_updata = 1;
					g_runstep = 0;
					break;
				case KEY_OUPTSET1_ENTER:
				case KEY_OUPTSET2_ENTER:
					dis_updata = 1;
					g_runstep = 0;
					i = (g_slcnum - 1) * OUTSETREGLEN;
					for (j = 0; j < OUTSETREGLEN; j++, i++)
					{
						IOREG[i + OUTSETREGI] = pbuf[j];
					}
					break;
				case KEY_OUPTSET1_UP:
					if (pbuf[0])
					{
						pbuf[0]--;
					}
					else
					{
						pbuf[0] = OUTSETPRSUM - 1;
					}
					dis_updata1 = 1;
					break;
				case KEY_OUPTSET1_DOWN:
					pbuf[0]++;
					if (pbuf[0] >= OUTSETPRSUM)
						pbuf[0] = 0;
					dis_updata1 = 1;
					break;
				case KEY_OUPTSET1_NOUSE:
					pbuf[0] = 0;
					dis_updata1 = 1;
					break;
				case KEY_OUPTSET1_SETTING:
					dis_updata = 1;
					g_runstep = 2;
					break;
				case KEY_OUPTSET2_BACK:
					dis_updata = 1;
					g_runstep = 1;
					break;
				case KEY_OUPTSET2_LOGIC:
					if (pbuf[1])
						pbuf[1] = 0;
					else
						pbuf[1] = 1;
					dis_updata2 = 1;
					break;
				case KEY_OUPTSET2_METHOD:
					if (pbuf[2])
						pbuf[2] = 0;
					else
						pbuf[2] = 1;
					dis_updata3 = 1;
					break;
				case KEY_OUPTSET2_NEXT:
					if (pageNow < pageNum)
					{
						pageNow++;
						dis_updata = 1;
						g_runstep = pageNow + 1;
					}
					break;
				case KEY_OUPTSET2_PREV:
					if (pageNow > 1)
					{
						pageNow--;
						dis_updata = 1;
						g_runstep = pageNow + 1;
					}
					break;

				case KEY_OUPTSET3_CLR:
					if (selItemS3 < 3)
					{
						pbuf[6 + selItemS3] = 0;
						dis_updata6 = 1;
					}
					break;
				case KEY_OUPTSET3_UP:
					if (selItemS3 < 3)
					{
						if (pbuf[6 + selItemS3] < 65535)
							pbuf[6 + selItemS3]++;
						dis_updata6 = 1;
					}
					break;
				case KEY_OUPTSET3_DOWN:
					if (selItemS3 < 3)
					{
						if (pbuf[6 + selItemS3])
							pbuf[6 + selItemS3]--;
						dis_updata6 = 1;
					}
					break;
				case KEY_OUPTSET3_MODSLC:
					pbuf[3]++;
					if (pbuf[3] > 8)
						pbuf[3] = 0;
					dis_updata4 = 1;
					dis_updata5 = 1;
					break;
				case KEY_OUPTSET3_HIGH:
					selItemS3 = 0;
					dis_updata6 = 1;
					break;
				case KEY_OUPTSET3_LOW:
					selItemS3 = 1;
					dis_updata6 = 1;
					break;
				case KEY_OUPTSET3_FULL:
					selItemS3 = 2;
					dis_updata6 = 1;
					break;

				case KEY_OUPTSET4_ONDELYT:
					if (pbuf[4])
						pbuf[4] = 0;
					else
						pbuf[4] = 1;
					dis_updata7 = 1;
					break;
				case KEY_OUPTSET4_OFFDELYT:
					if (pbuf[5])
						pbuf[5] = 0;
					else
						pbuf[5] = 1;
					dis_updata10 = 1;
					break;

				case KEY_OUPTSET4_UPON:
					if (PressDN)
					{
						dat = pbuf[9];
						if (Presscnt < 10)
						{
							if (dat < 65535)
							{
								dat++;
								Presscnt++;
							}
						}
						else if (Presscnt < 20)
						{
							if (dat < 65535)
							{
								dat += 10;
								Presscnt++;
							}
						}
						else if (Presscnt < 30)
						{
							if (dat < 65535)
							{
								dat += 100;
								Presscnt++;
							}
						}
						else if (Presscnt < 40)
						{
							if (dat < 65535)
							{
								dat += 1000;
								Presscnt++;
							}
						}
						else if (Presscnt < 50)
						{
							if (dat < 65535)
							{
								dat += 10000;
								Presscnt++;
							}
						}
						if (dat > 65535)
							dat = 65535;
						pbuf[9] = dat;
					}
					else
					{
						if (pbuf[9] < 65535)
							pbuf[9]++;
					}
					dis_updata9 = 1;
					// DrawFillRect(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4, rect[18].xsize - SAFE_DIST1, rect[18].ysize - SAFE_DIST1, rect[18].colorB[0]);
					// DrawMFU32Z(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4, pbuf[9], 5, rect[18].index[0]);
					break;
				case KEY_OUPTSET4_DNON:
					if (PressDN)
					{
						dat = pbuf[9];
						if (Presscnt < 10)
						{
							if (dat > 0)
							{
								dat--;
								Presscnt++;
							}
						}
						else if (Presscnt < 20)
						{
							if (dat > 0)
							{
								dat -= 10;
								Presscnt++;
							}
						}
						else if (Presscnt < 30)
						{
							if (dat > 0)
							{
								dat -= 100;
								Presscnt++;
							}
						}
						else if (Presscnt < 40)
						{
							if (dat > 0)
							{
								dat -= 1000;
								Presscnt++;
							}
						}
						else if (Presscnt < 50)
						{
							if (dat > 0)
							{
								dat -= 10000;
								Presscnt++;
							}
						}
						if (dat < 0)
							dat = 0;
						pbuf[9] = dat;
					}
					else
					{
						if (pbuf[9])
							pbuf[9]--;
					}
					dis_updata9 = 1;
					// DrawFillRect(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4, rect[18].xsize - SAFE_DIST1, rect[18].ysize - SAFE_DIST1, rect[18].colorB[0]);
					// DrawMFU32Z(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4, pbuf[9], 5, rect[18].index[0]);
					break;
				case KEY_OUPTSET4_UPOFF:
					if (PressDN)
					{
						dat = pbuf[10];
						if (Presscnt < 10)
						{
							if (dat < 65535)
							{
								dat++;
								Presscnt++;
							}
						}
						else if (Presscnt < 20)
						{
							if (dat < 65535)
							{
								dat += 10;
								Presscnt++;
							}
						}
						else if (Presscnt < 30)
						{
							if (dat < 65535)
							{
								dat += 100;
								Presscnt++;
							}
						}
						else if (Presscnt < 40)
						{
							if (dat < 65535)
							{
								dat += 1000;
								Presscnt++;
							}
						}
						else if (Presscnt < 50)
						{
							if (dat < 65535)
							{
								dat += 10000;
								Presscnt++;
							}
						}
						if (dat > 65535)
							dat = 65535;
						pbuf[10] = dat;
					}
					else
					{
						if (pbuf[10] < 65535)
							pbuf[10]++;
					}
					dis_updata12 = 1;
					// DrawFillRect(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4 + 109, rect[18].xsize - SAFE_DIST1, rect[18].ysize - SAFE_DIST1, rect[18].colorB[0]);
					// DrawMFU32Z(rect[18].xpis + SAFE_DIST4, rect[18].ypis + 109 + SAFE_DIST4, pbuf[10], 5, rect[18].index[0]);
					break;
				case KEY_OUPTSET4_DNOFF:
					if (PressDN)
					{
						dat = pbuf[10];
						if (Presscnt < 10)
						{
							if (dat > 0)
							{
								dat--;
								Presscnt++;
							}
						}
						else if (Presscnt < 20)
						{
							if (dat > 0)
							{
								dat -= 10;
								Presscnt++;
							}
						}
						else if (Presscnt < 30)
						{
							if (dat > 0)
							{
								dat -= 100;
								Presscnt++;
							}
						}
						else if (Presscnt < 40)
						{
							if (dat > 0)
							{
								dat -= 1000;
								Presscnt++;
							}
						}
						else if (Presscnt < 50)
						{
							if (dat > 0)
							{
								dat -= 10000;
								Presscnt++;
							}
						}
						if (dat < 0)
							dat = 0;
						pbuf[10] = dat;
					}
					else
					{
						if (pbuf[10])
							pbuf[10]--;
					}
					dis_updata12 = 1;
					// DrawFillRect(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4 + 109, rect[18].xsize - SAFE_DIST1, rect[18].ysize - SAFE_DIST1, rect[18].colorB[0]);
					// DrawMFU32Z(rect[18].xpis + SAFE_DIST4, rect[18].ypis + SAFE_DIST4 + 109, pbuf[10], 5, rect[18].index[0]);
					break;

				case KEY_OUPTSET_RESTORE:
					pno = MENU_RWHELP;
					hleppara = 137;
					poldno[pnoindex++] = MENU_OUTPUTSET;

					break;
				}
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_OUPTSET_SCRNSVR) //不需要显示的按键
			{
			}
			else if (keyon >= KEY_OUPTSET3_FULL && keyon <= KEY_OUPTSET3_HIGH)
			{
				DrawButtonOnZoomInRes(keyd[keyon]);
			}
			//非常规按键-//绘制颜色文字按键
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			PressDN = 0;
			Presscnt = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
		}

		// [选择添加项目*begin]*********************************
		if (dis_updata1)
		{
			U8 selNoList[7] = {0};
			U8 tmpIndex = 0;
			dis_updata1 = 0;
			BmpFillRect(rect[RECT_OUPTSET1_MSG].xpis + 1, rect[RECT_OUPTSET1_MSG].ypis + 1, rect[RECT_OUPTSET1_MSG].xsize - 2, rect[RECT_OUPTSET1_MSG].ysize - 2);

			Draw24x24MulStr(rect[RECT_OUPTSET1_MSG].xpis + SAFE_DIST4, rect[RECT_OUPTSET1_MSG].ypis + SAFE_DIST4,
							(MEM_OUTSETNAME + (U32)(IOSETNAME_LENGTH * pbuf[0]) + Iosetnamepos), 40, Outset_Sheet, pbuf[0], 3, rect[RECT_OUPTSET1_MSG].index[0]);
			selNoList[3] = pbuf[0];
			if (pbuf[0] < 3)
			{
				U16 oneSide = 3 - pbuf[0];	  //超出范围数量
				for (i = 0; i < oneSide; i++) //给左侧超过范围的值赋值
				{
					selNoList[oneSide - 1 - i] = selNoSum - 1 - i;
				}
				for (i = oneSide; i < 7; i++) //给未超范围的值赋值
				{
					if (i == 3)
					{
						selNoList[3] = pbuf[0];
					}
					else
					{
						selNoList[i] = i - oneSide;
					}
				}
			}
			else if (pbuf[0] >= (selNoSum - 3) && pbuf[0] < selNoSum)
			{
				U16 oneSide = (selNoSum - 1 - pbuf[0]); //未超出范围数量

				for (i = 4 + oneSide; i < 7; i++) //给右侧超过范围的值赋值
				{
					selNoList[i] = i - (4 + oneSide);
				}
				for (i = 3 + oneSide;; i--) //给未超范围的值赋值
				{
					if (i == 3)
					{
						selNoList[3] = pbuf[0];
					}
					else
					{
						selNoList[i] = selNoSum - 1 - (3 + oneSide - i);
						if (i == 0)
							break;
					}
				}
			}
			else
			{
				for (i = 1; i <= 3; i++)
				{
					selNoList[3 - i] = selNoList[3] - i;
					selNoList[3 + i] = selNoList[3] + i;
				}
			}

			for (i = 0, tmpIndex = 0; i < 7; i++)
			{
				tmpIndex = RECT_OUPTSET1_SEL1 + i;
				DrawFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis,
							 rect[tmpIndex].xsize, rect[tmpIndex].ysize, rect[tmpIndex].colorB[0]);
			}
			DrawDottedRect(rect[RECT_OUPTSET1_SEL4].xpis + 1, rect[RECT_OUPTSET1_SEL4].ypis + 1,
						   rect[RECT_OUPTSET1_SEL4].xsize - 2, rect[RECT_OUPTSET1_SEL4].ysize - 2, rect[RECT_OUPTSET1_SEL4].colorT[0]);

			for (i = 0, tmpIndex = 0; i < 7; i++)
			{
				if (selNoList[i] != 0xFF)
				{
					tmpIndex = RECT_OUPTSET1_SEL1 + i;
					Draw24x24Str(rect[tmpIndex].xpis + SAFE_DIST4, rect[tmpIndex].ypis + rect[tmpIndex].ysize / 2 - 12,
								 (MEM_OUTSETNAME + (U32)(selNoList[i] * IOSETNAME_LENGTH)), Outset_Sheet, selNoList[i], rect[tmpIndex].colorT[0]);
				}
			}
			vOUTgroup = pbuf[0] / 16;
			vOUTno = pbuf[0] % 16;
			vOUTlevel = (YREG[SYS_OUTLEVEL + vOUTgroup] >> vOUTno) & 0x01;
			ovOUTlevel = vOUTlevel;
		}
		// [选择添加项目*end]*********************************

		// [修改逻辑按钮显示*begin]*********************************
		if (dis_updata2)
		{
			dis_updata2 = 0;

			DrawButtonSingleColor(keyd[KEY_OUPTSET2_LOGIC], keybmpind[KEY_OUPTSET2_LOGIC].border[0], keybmpind[KEY_OUPTSET2_LOGIC].icon[0]);
			Draw24x24Str(keyd[KEY_OUPTSET2_LOGIC].xpis + keyd[KEY_OUPTSET2_LOGIC].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET2_LOGIC].nameStr[pbuf[1]] * TitleLengthMax)) * 12 / 2,
						 keyd[KEY_OUPTSET2_LOGIC].ypis + (keyd[KEY_OUPTSET2_LOGIC].ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET2_LOGIC].nameStr[pbuf[1]] * TitleLengthMax), SheetPage, keybmpind[KEY_OUPTSET2_LOGIC].nameStr[pbuf[1]], keybmpind[KEY_OUPTSET2_LOGIC].textColor[0]);
		}
		// [修改逻辑按钮显示*end]*********************************

		// [修改方式按钮显示*begin]*********************************
		if (dis_updata3)
		{
			dis_updata3 = 0;

			DrawButtonSingleColor(keyd[KEY_OUPTSET2_METHOD], keybmpind[KEY_OUPTSET2_METHOD].border[0], keybmpind[KEY_OUPTSET2_METHOD].icon[0]);
			Draw24x24Str(keyd[KEY_OUPTSET2_METHOD].xpis + keyd[KEY_OUPTSET2_METHOD].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET2_METHOD].nameStr[pbuf[2]] * TitleLengthMax)) * 12 / 2,
						 keyd[KEY_OUPTSET2_METHOD].ypis + (keyd[KEY_OUPTSET2_METHOD].ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET2_METHOD].nameStr[pbuf[2]] * TitleLengthMax), SheetPage, keybmpind[KEY_OUPTSET2_METHOD].nameStr[pbuf[2]], keybmpind[KEY_OUPTSET2_METHOD].textColor[0]);
		}
		// [修改方式按钮显示*end]*********************************

		// [修改脉冲按钮显示*begin]*********************************
		if (dis_updata4)
		{
			dis_updata4 = 0;
			dis_updata5 = 1;
			DrawButtonSingleColor(keyd[KEY_OUPTSET3_MODSLC], keybmpind[KEY_OUPTSET3_MODSLC].border[0], keybmpind[KEY_OUPTSET3_MODSLC].icon[0]);
			// Draw24x24Str(keyd[KEY_OUPTSET3_MODSLC].xpis + keyd[KEY_OUPTSET3_MODSLC].xsize / 2 - strlen((char *)(MEM_CONTENTMSG + (U32)(keybmpind[KEY_OUPTSET3_MODSLC].nameStr[0] + pbuf[3]) * CONTENTMSG_LENGTH)) * 12 / 2,
			// 			 keyd[KEY_OUPTSET3_MODSLC].ypis + (keyd[KEY_OUPTSET3_MODSLC].ysize - 24) / 2,
			// 			 (MEM_CONTENTMSG + (U32)(keybmpind[KEY_OUPTSET3_MODSLC].nameStr[0] + pbuf[3]) * CONTENTMSG_LENGTH), Contentmsg_Sheet, (keybmpind[KEY_OUPTSET3_MODSLC].nameStr[0] + pbuf[3]), keybmpind[KEY_OUPTSET3_MODSLC].textColor[0]);

			Draw24x24MulStr(keyd[KEY_OUPTSET3_MODSLC].xpis + SAFE_DIST1,
							keyd[KEY_OUPTSET3_MODSLC].ypis + 12,
							(MEM_CONTENTMSG + (U32)(keybmpind[KEY_OUPTSET3_MODSLC].nameStr[0] + pbuf[3]) * CONTENTMSG_LENGTH + Contentmsgpos),
							14, Contentmsg_Sheet, (keybmpind[KEY_OUPTSET3_MODSLC].nameStr[0] + pbuf[3]), 0, keybmpind[KEY_OUPTSET3_MODSLC].textColor[0]);
			dis_updata5 = 1;
		}
		// [修改脉冲按钮显示*end]*********************************

		// [修改脉冲数据选项显示*begin]*********************************
		if (dis_updata5)
		{
			U8 tmpIndex = 0;
			dis_updata5 = 0;
			if (pbuf[3])
			{
				if (pbuf[3] == 8)
				{
					if (selItemS3 > 2)
						selItemS3 = 2;
				}
				else
				{
					selItemS3 = 2;
				}

				for (i = 0; i < 3; i++)
				{
					if (i == 0 || pbuf[3] == 8)
					{
						tmpIndex = RECT_OUPTSET3_ITEM2_ALL + i * 4;
						DrawRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis, rect[tmpIndex].xsize, rect[tmpIndex].ysize, rect[tmpIndex].colorB[0]);
						DrawEditTit(rect[tmpIndex + 1], rect[tmpIndex + 1].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST4);
						Draw24X24FStr(rect[tmpIndex + 2].xpis + SAFE_DIST5, rect[tmpIndex + 2].ypis + SAFE_DIST3, "(1-65535)", rect[tmpIndex + 2].colorT[0]);
						Draw24X24FStr(rect[tmpIndex + 2].xpis + rect[tmpIndex + 2].xsize - 8 * 12, rect[tmpIndex + 2].ypis + SAFE_DIST3, "x0.1 ms", rect[tmpIndex + 2].colorT[0]);
						tmpIndex = KEY_OUPTSET3_FULL + i;
						DrawButtonEdit(keyd[tmpIndex],
									   (selItemS3 == (2 - i)) ? keybmpind[tmpIndex].border[1] : keybmpind[tmpIndex].icon[0],
									   (selItemS3 == (2 - i)) ? keybmpind[tmpIndex].icon[1] : keybmpind[tmpIndex].icon[0]);
						DrawMFU32Z(keyd[tmpIndex].xpis + keyd[tmpIndex].xsize / 2 - 5 * 6,
								   keyd[tmpIndex].ypis + SAFE_DIST3, pbuf[6 + (2 - i)], 5,
								   (selItemS3 == (2 - i)) ? keybmpind[tmpIndex].textColor[1] : keybmpind[tmpIndex].textColor[0]);
					}
					else
					{
						tmpIndex = RECT_OUPTSET3_ITEM2_ALL + i * 4;
						BmpFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis, rect[tmpIndex].xsize, rect[tmpIndex].ysize);
					}
				}
				for (i = KEY_OUPTSET3_CLR; i <= KEY_OUPTSET3_9; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}
			}
			else
			{
				selItemS3 = 3;
				for (i = 0; i < 3; i++)
				{
					tmpIndex = RECT_OUPTSET3_ITEM2_ALL + i * 4;
					BmpFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis, rect[tmpIndex].xsize, rect[tmpIndex].ysize);
				}
				for (i = KEY_OUPTSET3_CLR; i <= KEY_OUPTSET3_9; i++)
				{
					BmpFillRect(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
				}
			}
		}
		// [修改脉冲数据选项显示*end]*********************************

		// [修改脉冲数据选项显示的数据*begin]*********************************
		if (dis_updata6)
		{
			U8 tmpIndex = 0;
			dis_updata6 = 0;
			if (pbuf[3])
			{
				for (i = 0; i < 3; i++)
				{
					if (i == 0 || pbuf[3] == 8)
					{
						tmpIndex = KEY_OUPTSET3_FULL + i;
						DrawButtonEdit(keyd[tmpIndex],
									   (selItemS3 == (2 - i)) ? keybmpind[tmpIndex].border[1] : keybmpind[tmpIndex].icon[0],
									   (selItemS3 == (2 - i)) ? keybmpind[tmpIndex].icon[1] : keybmpind[tmpIndex].icon[0]);
						DrawMFU32Z(keyd[tmpIndex].xpis + keyd[tmpIndex].xsize / 2 - 5 * 6,
								   keyd[tmpIndex].ypis + SAFE_DIST3, pbuf[6 + (2 - i)], 5,
								   (selItemS3 == (2 - i)) ? keybmpind[tmpIndex].textColor[1] : keybmpind[tmpIndex].textColor[0]);
					}
				}
			}
		}
		// [修改脉冲数据选项显示的数据*end]*********************************

		// [修改开启延迟按钮*begin]*********************************
		if (dis_updata7)
		{
			dis_updata7 = 0;
			dis_updata8 = 1;
			DrawButtonSingleColor(keyd[KEY_OUPTSET4_ONDELYT], keybmpind[KEY_OUPTSET4_ONDELYT].border[0], keybmpind[KEY_OUPTSET4_ONDELYT].icon[0]);
			Draw24x24Str(keyd[KEY_OUPTSET4_ONDELYT].xpis + keyd[KEY_OUPTSET4_ONDELYT].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET4_ONDELYT].nameStr[pbuf[4]] * TitleLengthMax)) * 12 / 2,
						 keyd[KEY_OUPTSET4_ONDELYT].ypis + (keyd[KEY_OUPTSET4_ONDELYT].ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET4_ONDELYT].nameStr[pbuf[4]] * TitleLengthMax), SheetPage, keybmpind[KEY_OUPTSET4_ONDELYT].nameStr[pbuf[4]], keybmpind[KEY_OUPTSET4_ONDELYT].textColor[0]);
		}
		// [修改开启延迟按钮*end]*********************************

		// [修改开启延迟数据选项*begin]*********************************
		if (dis_updata8)
		{
			dis_updata8 = 0;
			if (pbuf[4])
			{
				DrawEdit(rect[RECT_OUPTSET4_ITEM1_PARA], rect[RECT_OUPTSET4_ITEM1_PARA].colorB[0], rect[RECT_OUPTSET4_ITEM1_PARA].colorB[0]);
				DrawMFU32Z(rect[RECT_OUPTSET4_ITEM1_PARA].xpis + rect[RECT_OUPTSET4_ITEM1_PARA].xsize / 2 - 5 * 6,
						   rect[RECT_OUPTSET4_ITEM1_PARA].ypis + SAFE_DIST3, pbuf[9], 5, rect[RECT_OUPTSET4_ITEM1_PARA].colorT[0]);
				DrawFFStr(rect[RECT_OUPTSET4_ITEM1_PARA].xpis + rect[RECT_OUPTSET4_ITEM1_PARA].xsize + SAFE_DIST4, rect[RECT_OUPTSET4_ITEM1_PARA].ypis + SAFE_DIST1, (U16 *)"x0.1 ms", rect[RECT_OUPTSET4_ITEM1_PARA].index[0]);

				for (i = KEY_OUPTSET4_UPON; i <= KEY_OUPTSET4_DNON; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}
			}
			else
			{
				BmpFillRect(rect[RECT_OUPTSET4_ITEM1_MSG].xpis + 1, keyd[KEY_OUPTSET4_UPON].ypis,
							rect[RECT_OUPTSET4_ITEM1_MSG].xsize - 2, keyd[KEY_OUPTSET4_UPON].ysize);
			}
		}
		// [修改开启延迟数据选项*end]*********************************

		// [修改开启延迟数据*begin]*********************************
		if (dis_updata9)
		{
			dis_updata9 = 0;
			if (pbuf[4])
			{
				DrawEdit(rect[RECT_OUPTSET4_ITEM1_PARA], rect[RECT_OUPTSET4_ITEM1_PARA].colorB[0], rect[RECT_OUPTSET4_ITEM1_PARA].colorB[0]);
				DrawMFU32Z(rect[RECT_OUPTSET4_ITEM1_PARA].xpis + rect[RECT_OUPTSET4_ITEM1_PARA].xsize / 2 - 5 * 6,
						   rect[RECT_OUPTSET4_ITEM1_PARA].ypis + SAFE_DIST3, pbuf[9], 5, rect[RECT_OUPTSET4_ITEM1_PARA].colorT[0]);
			}
		}
		// [修改开启延迟数据*end]*********************************

		// [修改关闭延迟按钮*begin]*********************************
		if (dis_updata10)
		{
			dis_updata10 = 0;
			dis_updata11 = 1;
			DrawButtonSingleColor(keyd[KEY_OUPTSET4_OFFDELYT], keybmpind[KEY_OUPTSET4_OFFDELYT].border[0], keybmpind[KEY_OUPTSET4_OFFDELYT].icon[0]);
			Draw24x24Str(keyd[KEY_OUPTSET4_OFFDELYT].xpis + keyd[KEY_OUPTSET4_OFFDELYT].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET4_OFFDELYT].nameStr[pbuf[5]] * TitleLengthMax)) * 12 / 2,
						 keyd[KEY_OUPTSET4_OFFDELYT].ypis + (keyd[KEY_OUPTSET4_OFFDELYT].ysize - 24) / 2,
						 (MEM_TITLE_ADD + (U32)keybmpind[KEY_OUPTSET4_OFFDELYT].nameStr[pbuf[5]] * TitleLengthMax), SheetPage, keybmpind[KEY_OUPTSET4_OFFDELYT].nameStr[pbuf[5]], keybmpind[KEY_OUPTSET4_OFFDELYT].textColor[0]);
		}
		// [修改关闭延迟按钮*end]*********************************

		// [修改关闭延迟数据选项*begin]*********************************
		if (dis_updata11)
		{
			dis_updata11 = 0;
			if (pbuf[5])
			{
				DrawEdit(rect[RECT_OUPTSET4_ITEM2_PARA], rect[RECT_OUPTSET4_ITEM2_PARA].colorB[0], rect[RECT_OUPTSET4_ITEM2_PARA].colorB[0]);
				DrawMFU32Z(rect[RECT_OUPTSET4_ITEM2_PARA].xpis + rect[RECT_OUPTSET4_ITEM2_PARA].xsize / 2 - 5 * 6,
						   rect[RECT_OUPTSET4_ITEM2_PARA].ypis + SAFE_DIST3, pbuf[10], 5, rect[RECT_OUPTSET4_ITEM2_PARA].colorT[0]);
				DrawFFStr(rect[RECT_OUPTSET4_ITEM2_PARA].xpis + rect[RECT_OUPTSET4_ITEM2_PARA].xsize + SAFE_DIST4, rect[RECT_OUPTSET4_ITEM2_PARA].ypis + SAFE_DIST1, (U16 *)"x0.1 ms", rect[RECT_OUPTSET4_ITEM2_PARA].index[0]);

				for (i = KEY_OUPTSET4_UPOFF; i <= KEY_OUPTSET4_DNOFF; i++)
				{
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
				}
			}
			else
			{
				BmpFillRect(rect[RECT_OUPTSET4_ITEM2_MSG].xpis + 1, keyd[KEY_OUPTSET4_UPOFF].ypis,
							rect[RECT_OUPTSET4_ITEM2_MSG].xsize - 2, keyd[KEY_OUPTSET4_UPOFF].ysize);
			}
		}
		// [修改关闭延迟数据选项*end]*********************************

		// [修改关闭延迟数据*begin]*********************************
		if (dis_updata12)
		{
			dis_updata12 = 0;
			if (pbuf[4])
			{
				DrawEdit(rect[RECT_OUPTSET4_ITEM2_PARA], rect[RECT_OUPTSET4_ITEM2_PARA].colorB[0], rect[RECT_OUPTSET4_ITEM2_PARA].colorB[0]);
				DrawMFU32Z(rect[RECT_OUPTSET4_ITEM2_PARA].xpis + rect[RECT_OUPTSET4_ITEM2_PARA].xsize / 2 - 5 * 6,
						   rect[RECT_OUPTSET4_ITEM2_PARA].ypis + SAFE_DIST3, pbuf[10], 5, rect[RECT_OUPTSET4_ITEM2_PARA].colorT[0]);
			}
		}
		// [修改关闭延迟数据*end]*********************************

		// [换页按钮刷新*begin]*********************************
		if (dis_updata13)
		{
			dis_updata13 = 0;
			DrawFillRect(rect[RECT_OUPTSET2_PAGE].xpis, rect[RECT_OUPTSET2_PAGE].ypis,
						 rect[RECT_OUPTSET2_PAGE].xsize, rect[RECT_OUPTSET2_PAGE].ysize, rect[RECT_OUPTSET2_PAGE].colorB[0]);
			DrawFU32(rect[RECT_OUPTSET2_PAGE].xpis + SAFE_DIST2, rect[RECT_OUPTSET2_PAGE].ypis + SAFE_DIST4,
					 pageNow, 3, rect[RECT_OUPTSET2_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_OUPTSET2_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_OUPTSET2_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_OUPTSET2_PAGE].colorT[0]);
			DrawFU32(rect[RECT_OUPTSET2_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_OUPTSET2_PAGE].ypis + SAFE_DIST4,
					 pageNum, 3, rect[RECT_OUPTSET2_PAGE].colorT[0]);

			DrawButton(keyd[KEY_OUPTSET2_PREV], (U16 *)(ADD_ICONBMP),
					   (pageNow == 1) ? keybmpind[KEY_OUPTSET2_PREV].border[1] : keybmpind[KEY_OUPTSET2_PREV].border[0],
					   (pageNow == 1) ? keybmpind[KEY_OUPTSET2_PREV].icon[1] : keybmpind[KEY_OUPTSET2_PREV].icon[0]);
			DrawButton(keyd[KEY_OUPTSET2_NEXT], (U16 *)(ADD_ICONBMP),
					   (pageNow == pageNum) ? keybmpind[KEY_OUPTSET2_NEXT].border[1] : keybmpind[KEY_OUPTSET2_NEXT].border[0],
					   (pageNow == pageNum) ? keybmpind[KEY_OUPTSET2_NEXT].icon[1] : keybmpind[KEY_OUPTSET2_NEXT].icon[0]);
		}
		// [换页按钮刷新*end]*********************************

		if (ovOUTlevel != vOUTlevel)
		{
			// BmpFillRect(16, 84, 32, 32);
			// DrawFPBMPBTN(16, 84, 32, 32, vOUTlevel ? 569 : 568, 0xffff);
			ovOUTlevel = vOUTlevel;
		}

		if ((Timer_ms > 200) && (g_runstep == 1))
		{
			Timer_ms = 0;
			Read_D(D_Address[SYS] + SYS_OUTLEVEL, 6, (U16 *)(&YREG[SYS_OUTLEVEL])); // 读输入电平状态
			vOUTlevel = (YREG[SYS_OUTLEVEL + vOUTgroup] >> vOUTno) & 0x01;
		}
		if (pno != MENU_OUTPUTSET)
		{
			plastno = MENU_OUTPUTSET;
			break;
		}
		updateViewStatus();
		wdt();
	}
#endif
}

//------------------------------------------------------
#define KEY_SNSRSTS_SCRNSVR 1
#define KEY_SNSRSTS_BACK 2
#define KEY_SNSRSTS_XF 3 //3 x左右移
#define KEY_SNSRSTS_XZ 4
#define KEY_SNSRSTS_YZ 5 //5 y上下移
#define KEY_SNSRSTS_YF 6
#define KEY_SNSRSTS_PFUP 7	 //7 中压脚上
#define KEY_SNSRSTS_PFDOWN 8 //下
#define KEY_SNSRSTS_VZ 9	 //9 针杆正反左右转
#define KEY_SNSRSTS_VF 10
#define KEY_SNSRSTS_WZ 11 //11梭床正反转
#define KEY_SNSRSTS_WF 12
#define KEY_SNSRSTS_MAX 13

#define RECT_SNSRSTS_ANG_TIT 1
#define RECT_SNSRSTS_ANG_PARA1 2
#define RECT_SNSRSTS_ANG_PARA2 3
#define RECT_SNSRSTS_ENC_TIT 4
#define RECT_SNSRSTS_ITEM1_TIT 5
#define RECT_SNSRSTS_ITEM2_TIT 6
#define RECT_SNSRSTS_ITEM3_TIT 7
#define RECT_SNSRSTS_ITEM4_TIT 8
#define RECT_SNSRSTS_ITEM5_TIT 9
#define RECT_SNSRSTS_ITEM6_TIT 10
#define RECT_SNSRSTS_ITEM7_TIT 11
#define RECT_SNSRSTS_ITEM8_TIT 12
#define RECT_SNSRSTS_ITEM1_STA 13
#define RECT_SNSRSTS_ITEM2_STA 14
#define RECT_SNSRSTS_ITEM3_STA 15
#define RECT_SNSRSTS_ITEM4_STA 16
#define RECT_SNSRSTS_ITEM5_STA 17
#define RECT_SNSRSTS_ITEM6_STA 18
#define RECT_SNSRSTS_ITEM7_STA 19
#define RECT_SNSRSTS_ITEM8_STA 20

#define BMP_SNSRSTS_ANG 1
#define BMP_SNSRSTS_ENC 2

//------------------------------------------
// Function   : void Menu_Sensorstatus(void)
// Description: 检测器信号状态显示
//------------------------------------------
void Menu_Sensorstatus(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U8 f = 1;
	U32 InputPointStatuOld = 0;
	U16 CheckAngle_old = 0xffff;
	U16 DnAngle_old = 0xffff;

	const U8 keynum = {KEY_SNSRSTS_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},					 //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //2 返回
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},	 //3 x左
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //4 x右
		{TYPE_X10, TYPE_Y5, KEY_X1, KEY_Y1}, //5 y上
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //6 y下
		{TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},	 //7 中压脚上
		{TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},	 //8 中压脚下
		{TYPE_X4, TYPE_Y4, KEY_X1, KEY_Y1},	 //9 针杆正转
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},	 //10 针杆反转
		{TYPE_X6, TYPE_Y4, KEY_X1, KEY_Y1},	 //11 梭床正转
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},	 //12 梭床反转
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {TLFNADD_IOALL + 1, 0, 0, 0}, {0}},	   //1 	屏保
		{{KEY_Silver}, {0}, {72}, {TLFNADD_IOALL + 2, 0, 0, 0}, {0}},	   //2	返回
		{{KEY_Silver}, {0}, {58}, {TLFNADD_IOALL + 22, 0, 0, 0}, {0}},	   //3	x左右
		{{KEY_Silver}, {0}, {59}, {TLFNADD_IOALL + 23, 0, 0, 0}, {0}},	   //4
		{{KEY_Silver}, {0}, {56}, {TLFNADD_IOALL + 24, 0, 0, 0}, {0}},	   //5	y上下
		{{KEY_Silver}, {0}, {61}, {TLFNADD_IOALL + 25, 0, 0, 0}, {0}},	   //6
		{{KEY_Silver}, {0}, {17, 18}, {TLFNADD_IOALL + 26, 0, 0, 0}, {0}}, //7	中压脚上下
		{{KEY_Silver}, {0}, {19, 20}, {TLFNADD_IOALL + 27, 0, 0, 0}, {0}}, //8
		{{KEY_Silver}, {0}, {857}, {TLFNADD_IOALL + 28, 0, 0, 0}, {0}},	   //9	针杆正反转
		{{KEY_Silver}, {0}, {858}, {TLFNADD_IOALL + 29, 0, 0, 0}, {0}},	   //10
		{{KEY_Silver}, {0}, {859}, {TLFNADD_IOALL + 30, 0, 0, 0}, {0}},	   //11	梭床正反转
		{{KEY_Silver}, {0}, {860}, {TLFNADD_IOALL + 31, 0, 0, 0}, {0}},	   //12
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_IOALL + 34}},
		{240, 137, 150, 32, {Color_Black}, {Color_Black}, {TLFNADD_IOALL + 47}}, //1 检出器角度
		{400, 137, 72, 32, {Color_Black}, {Color_Black}, {0}},					 //2 检出器文本框1
		{482, 137, 72, 32, {Color_Black}, {Color_Black}, {0}},					 //3 检出器文本框2
		{350, 184, 100, 32, {Color_Black}, {Color_Black}, {TLFNADD_IOALL + 46}}, //4 编码器文本框

		{140, 185, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 37}},											 //5 名称-1
		{140, 185 + (32 + SAFE_DIST5) * 1, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 38}},					 //6 名称-2
		{140, 185 + (32 + SAFE_DIST5) * 2, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 44}},					 //7 名称-3
		{140, 185 + (32 + SAFE_DIST5) * 3, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 45}},					 //8 名称-4
		{490, 185, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 40}},											 //9 名称-5
		{490, 185 + (32 + SAFE_DIST5) * 1, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 41}},					 //10 名称-6
		{490, 185 + (32 + SAFE_DIST5) * 2, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 42}},					 //11 名称-7
		{490, 185 + (32 + SAFE_DIST5) * 3, 100, 32, {Color_Silver}, {Color_Black}, {TLFNADD_IOALL + 43}},					 //12 名称-8
		{140 + 100 + SAFE_DIST5, 185, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},						 //13 状态-1
		{140 + 100 + SAFE_DIST5, 185 + (32 + SAFE_DIST5) * 1, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //14 状态-2
		{140 + 100 + SAFE_DIST5, 185 + (32 + SAFE_DIST5) * 2, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //15 状态-3
		{140 + 100 + SAFE_DIST5, 185 + (32 + SAFE_DIST5) * 3, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //16 状态-4
		{490 + 100 + SAFE_DIST5, 185, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},						 //17 状态-5
		{490 + 100 + SAFE_DIST5, 185 + (32 + SAFE_DIST5) * 1, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //18 状态-6
		{490 + 100 + SAFE_DIST5, 185 + (32 + SAFE_DIST5) * 2, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //19 状态-7
		{490 + 100 + SAFE_DIST5, 185 + (32 + SAFE_DIST5) * 3, 64, 32, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //20 状态-8

	};
	const BMPPAD disBmp[] = {
		{0, 0, 0, 0, {0}},
		{128, 40, 545, 140, {867, 0}, Color_White},	   //1 检出器信号
		{336, 220, 128, 128, {865, 866}, Color_White}, //2 编码器
	};
	SetLanguage(MENU_IOTEST);
	InitScreen();
	initViewStatus();
	updateViewStatus();
	TESTKEYF = 1;

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	DrawFPBMPBTN(disBmp[BMP_SNSRSTS_ANG].xpis, disBmp[BMP_SNSRSTS_ANG].ypis, disBmp[BMP_SNSRSTS_ANG].xsize, disBmp[BMP_SNSRSTS_ANG].ysize,
				 disBmp[BMP_SNSRSTS_ANG].icon[0], disBmp[BMP_SNSRSTS_ANG].filter);
	DrawFPBMPBTN(disBmp[BMP_SNSRSTS_ENC].xpis, disBmp[BMP_SNSRSTS_ENC].ypis, disBmp[BMP_SNSRSTS_ENC].xsize, disBmp[BMP_SNSRSTS_ENC].ysize,
				 disBmp[BMP_SNSRSTS_ENC].icon[0], disBmp[BMP_SNSRSTS_ENC].filter);

	for (i = KEY_SNSRSTS_BACK; i <= KEY_SNSRSTS_PFDOWN; i++)
	{
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	DrawRect(rect[RECT_SNSRSTS_ANG_TIT].xpis, rect[RECT_SNSRSTS_ANG_TIT].ypis, rect[RECT_SNSRSTS_ANG_TIT].xsize, rect[RECT_SNSRSTS_ANG_TIT].ysize, rect[RECT_SNSRSTS_ANG_TIT].colorB[0]);
	Draw24x24Str(rect[RECT_SNSRSTS_ANG_TIT].xpis + rect[RECT_SNSRSTS_ANG_TIT].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + TitleLengthMax * rect[RECT_SNSRSTS_ANG_TIT].index[0])) * 6,
				 rect[RECT_SNSRSTS_ANG_TIT].ypis + rect[RECT_SNSRSTS_ANG_TIT].ysize / 2 - 12,
				 (MEM_TITLE_ADD + TitleLengthMax * rect[RECT_SNSRSTS_ANG_TIT].index[0]), SheetPage,
				 rect[RECT_SNSRSTS_ANG_TIT].index[0], rect[RECT_SNSRSTS_ANG_TIT].colorT[0]); //检出器角度
	DrawRect(rect[RECT_SNSRSTS_ENC_TIT].xpis, rect[RECT_SNSRSTS_ENC_TIT].ypis, rect[RECT_SNSRSTS_ENC_TIT].xsize, rect[RECT_SNSRSTS_ENC_TIT].ysize, rect[RECT_SNSRSTS_ENC_TIT].colorB[0]);
	Draw24x24Str(rect[RECT_SNSRSTS_ENC_TIT].xpis + rect[RECT_SNSRSTS_ENC_TIT].xsize / 2 - strlen((char *)(MEM_TITLE_ADD + TitleLengthMax * rect[RECT_SNSRSTS_ENC_TIT].index[0])) * 6,
				 rect[RECT_SNSRSTS_ENC_TIT].ypis + rect[RECT_SNSRSTS_ENC_TIT].ysize / 2 - 12,
				 (MEM_TITLE_ADD + TitleLengthMax * rect[RECT_SNSRSTS_ENC_TIT].index[0]), SheetPage,
				 rect[RECT_SNSRSTS_ENC_TIT].index[0], rect[RECT_SNSRSTS_ENC_TIT].colorT[0]); //编码器

	DrawRect(rect[RECT_SNSRSTS_ANG_PARA1].xpis, rect[RECT_SNSRSTS_ANG_PARA1].ypis, rect[RECT_SNSRSTS_ANG_PARA1].xsize, rect[RECT_SNSRSTS_ANG_PARA1].ysize, rect[RECT_SNSRSTS_ANG_PARA1].colorB[0]);
	DrawMFU32(rect[RECT_SNSRSTS_ANG_PARA1].xpis + rect[RECT_SNSRSTS_ANG_PARA1].xsize / 2 - 3 * 6,
			  rect[RECT_SNSRSTS_ANG_PARA1].ypis + rect[RECT_SNSRSTS_ANG_PARA1].ysize / 2 - 12,
			  g_CheckAngle, 3, rect[RECT_SNSRSTS_ANG_PARA1].colorT[0]); //检出器角度-参数1

	if (TESTKEYF) //测试功能使能
	{
		for (i = KEY_SNSRSTS_VZ; i <= KEY_SNSRSTS_WF; i++)
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
		DrawRect(rect[RECT_SNSRSTS_ANG_PARA2].xpis, rect[RECT_SNSRSTS_ANG_PARA2].ypis, rect[RECT_SNSRSTS_ANG_PARA2].xsize, rect[RECT_SNSRSTS_ANG_PARA2].ysize, rect[RECT_SNSRSTS_ANG_PARA2].colorB[0]);
		DrawMFU32(rect[RECT_SNSRSTS_ANG_PARA2].xpis + rect[RECT_SNSRSTS_ANG_PARA2].xsize / 2 - 3 * 6,
				  rect[RECT_SNSRSTS_ANG_PARA2].ypis + rect[RECT_SNSRSTS_ANG_PARA2].ysize / 2 - 12,
				  DnAngle_old, 3, rect[RECT_SNSRSTS_ANG_PARA2].colorT[0]); //检出器角度-参数2
	}

	for (i = 0; i < 8; i++) //显示测试信息
	{
		f = i + RECT_SNSRSTS_ITEM1_TIT;
		DrawEditTit(rect[f], rect[f].index[0], SheetPage, LEFT, STR16X8, SAFE_DIST5);
	}

	E_IOTESTEN();
	if (Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE) == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		// poldno[2] = MENU_SENSORSTATUS;
		// pnoindex = 3;
	}

	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//键值为按键松开时
		if (keydata == 0xff)
		{
			if ((keyon) && (beeponf))
			{
				keyoff = keyon; //按键键值传给松开键值
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理
		{
			if ((keydata >= KEY_SNSRSTS_VZ && keydata <= KEY_SNSRSTS_WF) && (TESTKEYF == 0))
			{
				keydata = 0;
			}
			else
			{
				if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
				{
					keyon = keydata;
					if (keyon == KEY_SNSRSTS_SCRNSVR)
					{
					}
					else
					{
						DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]); //常规按键特效
					}

					Times = 0;
					delay = KEYDT; //按键防抖次数
				}
				else if (keyon != keydata) //已有按键按下，不处理新按键信息
				{
					keydata = 0;
				}
				if (Times > delay) //按键防抖
				{
					if (beeponf == 0)
					{
						beeponf = 1;
						if (g_keyBellT) //按键时蜂鸣器发声时间
							Beep_On(g_keyBellT);
						switch (keyon) //按键按下标题提示语显示
						{
						default:
							DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						}
						delay = KEYFT;
						if (keyon >= KEY_SNSRSTS_XF && keyon < keynum)
						{
							f = keyon - KEY_SNSRSTS_XF + 1;
							Write_Cmd(C_XYTEST, f, XYTESTON);
							for (i = 0; i < RDTIMES; i++)
							{
								Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
								if ((DREG[D_CMDSTATUS]) == 100)
									break;
								DelayMs(WAITTIME);
							}
						}
					}
					else if (beeponf == 1) //第二次进入
					{
						beeponf = 2;
						delay = KEYNT;
					}
					else if (beeponf < 20)
					{
						beeponf++;
					}
					Times = 0; //当前
				}
			}
		}
		else
		{
			key_on_flag1 = 0;
		}
		if (keyoff) //按键松开处理
		{
			switch (keyoff)
			{
			case KEY_SNSRSTS_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_SNSRSTS_BACK:
				pno = MENU_IOTEST;
				pnoindex--;
				E_IOTESTDIS();
				if (Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE) == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[pnoindex++] = MENU_SENSORSTATUS;
				}
				break;
			case KEY_SNSRSTS_PFUP:
				Send_Pfoot(1, MENU_CURSOR, 1); //压脚升
				break;
			case KEY_SNSRSTS_PFDOWN:
				Send_Pfoot(2, MENU_CURSOR, 1); //压脚降
				break;
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_SNSRSTS_SCRNSVR) //不需要显示的按键
			{
			}
			//非常规按键-//绘制颜色文字按键
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}
			if (keyon >= KEY_SNSRSTS_XF && keyon < keynum)
			{
				f = keyon - KEY_SNSRSTS_XF + 1;
				Write_Cmd(C_XYTEST, f, XYTESTOFF);
				for (i = 0; i < RDTIMES; i++)
				{
					Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
					if ((DREG[D_CMDSTATUS]) == 100)
						break;
					DelayMs(WAITTIME);
				}
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
		}

		if (Timer_ms > 50)
		{
			Timer_ms = 0;

			f = Read_D(D_Address[SYS] + SYS_INPUTL, 2, (U16 *)(&YREG[SYS_INPUTL])); // 输入口当前状态低位
			if (f == 0)
			{
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex++] = MENU_SENSORSTATUS;
				break;
			}

			g_InputPointStatus = ((U32)(YREG[SYS_INPUTH]) << 16) + (U32)(YREG[SYS_INPUTL]);

			f = Read_D(D_Address[SYS] + SYS_MANG, 1, (U16 *)(&YREG[SYS_MANG]));
			if (f == 0)
			{
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex++] = MENU_SENSORSTATUS;
				break;
			}
			g_CheckAngle = YREG[SYS_MANG];
			g_CheckAngle = g_CheckAngle % 360;

			if (InputPointStatuOld != g_InputPointStatus)
			{
				InputPointStatuOld = g_InputPointStatus;
				DispMotorStatus(&rect[RECT_SNSRSTS_ITEM1_STA], RECT_SNSRSTS_ITEM8_STA - RECT_SNSRSTS_ITEM1_STA + 1, g_InputPointStatus, (U32 *)motordat);
			}

			if (CheckAngle_old != g_CheckAngle)
			{
				CheckAngle_old = g_CheckAngle;
				BmpFillRect(rect[RECT_SNSRSTS_ANG_PARA1].xpis + 1, rect[RECT_SNSRSTS_ANG_PARA1].ypis + 1, rect[RECT_SNSRSTS_ANG_PARA1].xsize - 2, rect[RECT_SNSRSTS_ANG_PARA1].ysize - 2);
				DrawMFU32(rect[RECT_SNSRSTS_ANG_PARA1].xpis + rect[RECT_SNSRSTS_ANG_PARA1].xsize / 2 - 3 * 6,
						  rect[RECT_SNSRSTS_ANG_PARA1].ypis + rect[RECT_SNSRSTS_ANG_PARA1].ysize / 2 - 12,
						  g_CheckAngle, 3, rect[RECT_SNSRSTS_ANG_PARA1].colorT[0]); //检出器角度-参数2
			}
			if (TESTKEYF)
			{
				f = Read_D(D_Address[SYS] + SYS_DNANGLE, 1, (U16 *)(&YREG[SYS_DNANGLE])); //下轴角度
				if (f == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[pnoindex++] = MENU_SENSORSTATUS;
					break;
				}
				if (DnAngle_old != YREG[SYS_DNANGLE])
				{
					DnAngle_old = YREG[SYS_DNANGLE];

					BmpFillRect(rect[RECT_SNSRSTS_ANG_PARA2].xpis + 1, rect[RECT_SNSRSTS_ANG_PARA2].ypis + 1, rect[RECT_SNSRSTS_ANG_PARA2].xsize - 2, rect[RECT_SNSRSTS_ANG_PARA2].ysize - 2);
					DrawMFU32(rect[RECT_SNSRSTS_ANG_PARA2].xpis + rect[RECT_SNSRSTS_ANG_PARA2].xsize / 2 - 3 * 6,
							  rect[RECT_SNSRSTS_ANG_PARA2].ypis + rect[RECT_SNSRSTS_ANG_PARA2].ysize / 2 - 12,
							  DnAngle_old, 3, rect[RECT_SNSRSTS_ANG_PARA2].colorT[0]); //检出器角度-参数2
				}
			}
		}

		if (pno != MENU_SENSORSTATUS)
		{
			plastno = MENU_SENSORSTATUS;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//	  _INPTSET_

//------------------------------------------------------------
#define KEY_ANASET_SCRNSVR 1
#define KEY_ANASET_BACK 2
#define KEY_ANASET_ENTER 3
#define KEY_ANASET_VMAX 4
#define KEY_ANASET_VMIN 5
#define KEY_ANASET_ARAI 6
#define KEY_ANASET_ARASWON 7
#define KEY_ANASET_ARASWOFF 8
#define KEY_ANASET_CLR 9
#define KEY_ANASET_UP 10
#define KEY_ANASET_DN 11
#define KEY_ANASET_0 12
#define KEY_ANASET_1 13
#define KEY_ANASET_2 14
#define KEY_ANASET_3 15
#define KEY_ANASET_4 16
#define KEY_ANASET_5 17
#define KEY_ANASET_6 18
#define KEY_ANASET_7 19
#define KEY_ANASET_8 20
#define KEY_ANASET_9 21

#define VMAX 999
#define VMIN 0
#define IMAX 999
#define IMIN 0
//------------------------------------------
// Function   : void Menu_AnalogSet(void)
// Description: 模拟量设定
//------------------------------------------
void Menu_AnalogSet(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U8 disupdata1 = 1, disupdata2 = 0, disupdata3 = 0, disupdata4 = 0, disupdata5 = 1, disupdata6 = 1;
	U8 settype = 1;
	U32 i;
	U16 vmax = 0, vmin = 0;
	U16 araI = 0;
	U8 araS = 0;
	U8 dp0 = 0;
	Read_D(D_Address[SYS] + SYS_PEDAL_V, 6, (U16 *)(&YREG[SYS_PEDAL_V]));
	vmax = YREG[SYS_PEDAL_VMAX];
	vmin = YREG[SYS_PEDAL_VMIN];
	araI = YREG[SYS_ARA_I];
	araS = YREG[SYS_ARA_S];
	U16 tmp_num[4] = {vmax, vmin, araI};
	const U8 keynum = {22};
	const KEYPAD keyd[] =
		{
			{0, 0, 0, 0},
			{0, 0, 128, 32},							  //1 屏保
			{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},			  //2 返回
			{TYPE_X11 - 2, TYPE_Y4, KEY_X1, KEY_Y1},	  //3 确认
			{339, 119 + 37, KEY_X1, 32},				  //4 电流最大值
			{339, 119, KEY_X1, 32},						  //5 电流最小值
			{339, 249, KEY_X1, 32},						  //6 报警电流
			{339, 286, KEY_X1, 32},						  //7 报警开关on
			{339 + 64, 286, KEY_X1, 32},				  //8 报警开关off
			{566 + 53 * 0, 115 + 53 * 4, KEY_X2, KEY_Y2}, //9 清除一位
			{566 + 53 * 1, 115 + 53 * 3, KEY_X2, KEY_Y2}, //10 加1
			{566 + 53 * 2, 115 + 53 * 3, KEY_X2, KEY_Y2}, //11 减1
			{566, 115 + 53 * 3, KEY_X2, KEY_Y2},		  //12 0
			{566, 115, KEY_X2, KEY_Y2},					  //13 1
			{566 + 53 * 1, 115 + 53 * 0, KEY_X2, KEY_Y2}, //14 2
			{566 + 53 * 2, 115 + 53 * 0, KEY_X2, KEY_Y2}, //15 3
			{566, 115 + 53 * 1, KEY_X2, KEY_Y2},		  //16 4
			{566 + 53 * 1, 115 + 53 * 1, KEY_X2, KEY_Y2}, //17 5
			{566 + 53 * 2, 115 + 53 * 1, KEY_X2, KEY_Y2}, //18 6
			{566, 115 + 53 * 2, KEY_X2, KEY_Y2},		  //19 7
			{566 + 53 * 1, 115 + 53 * 2, KEY_X2, KEY_Y2}, //20 8
			{566 + 53 * 2, 115 + 53 * 2, KEY_X2, KEY_Y2}, //21 9
		};
	const tydDisSrcIndex keybmpind[] =
		{
			{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
			{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},										  //1 屏保
			{{KEY_Silver}, {0}, {72}, {132, 0, 0, 0}, {0}},										  //2	返回
			{{KEY_Silver}, {0}, {71}, {133, 0, 0, 0}, {0}},										  //3	确认
			{{KEY_Silver}, {Color_Black}, {Color_DeepSkyBlue, Color_White}, {135, 0, 0, 0}, {0}}, //4	电流最大值
			{{KEY_Silver}, {Color_Black}, {Color_DeepSkyBlue, Color_White}, {134, 0, 0, 0}, {0}}, //5	电流最小值
			{{KEY_Silver}, {Color_Black}, {Color_DeepSkyBlue, Color_White}, {136, 0, 0, 0}, {0}}, //6	报警电流
			{{KEY_Silver, KEY_Blue}, {0}, {861, 862}, {137, 148, 0, 0}, {0}},					  //7	报警开关on
			{{KEY_Silver, KEY_Blue}, {0}, {863, 864}, {137, 148, 0, 0}, {0}},					  //8	报警开关off
			{{KEY_Silver}, {0}, {113, 114}, {91, 0, 0, 0}, {0}},								  //9 清除
			{{KEY_Silver}, {0}, {199, 200}, {92, 0, 0, 0}, {0}},								  //10 上翻
			{{KEY_Silver}, {0}, {201, 201}, {93, 0, 0, 0}, {0}},								  //11 下翻
			{{KEY_Silver}, {0}, {91, 101}, {10, 0, 0, 0}, {0}},									  //12 0
			{{KEY_Silver}, {0}, {92, 102}, {11, 0, 0, 0}, {0}},									  //13 1
			{{KEY_Silver}, {0}, {93, 103}, {12, 0, 0, 0}, {0}},									  //14 2
			{{KEY_Silver}, {0}, {94, 104}, {13, 0, 0, 0}, {0}},									  //15 3
			{{KEY_Silver}, {0}, {95, 105}, {14, 0, 0, 0}, {0}},									  //16 4
			{{KEY_Silver}, {0}, {96, 106}, {15, 0, 0, 0}, {0}},									  //17 5
			{{KEY_Silver}, {0}, {97, 107}, {16, 0, 0, 0}, {0}},									  //18 6
			{{KEY_Silver}, {0}, {98, 108}, {17, 0, 0, 0}, {0}},									  //19 7
			{{KEY_Silver}, {0}, {99, 109}, {18, 0, 0, 0}, {0}},									  //20 8
			{{KEY_Silver}, {0}, {100, 110}, {19, 0, 0, 0}, {0}},								  //21 9
		};
	// const U16 keybmpind[] = {0, 0, 69, 70, 0, 0, 0, 578, 71, 40, 41, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
	// const KEYPAD keyd1[] = {{200, 48, 88, 40}, {200, 208, 88, 40}, {200, 386, 88, 40}};
	const RECTPAD rect[] =
		{
			//方框
			{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
			{TYPE_X17 + SAFE_DIST4, SAFE_DIST4, 426, 24, {Color_Black}, {Color_White}, {Color_White}}, //1 系统文本框
			{TYPE_X14 + 1, 80, 368, 110, {0}, {Color_Black}, {0}},									   //2 上部整体框
			{TYPE_X14 + 1, 210, 368, 110, {0}, {Color_Black}, {0}},									   //3 下部整体框
			{103, 82, 364, 32, {Color_KeyBlue}, {Color_Black}, {Color_White, 142, 141}},			   //4 单踏板电流框
			{339, 82, 64, 32, {Color_KeyBlue}, {0}, {Color_White}},									   //5 数字设定框
			{424, 82, 0, 0, {0}, {0}, {Color_White}},												   //6 A/V
		};
	SetLanguage(MENU_IOTEST);
	InitScreen();
	initViewStatus();
	updateViewStatus();
	if (P_LAN == 2)
	{ //越南语
		dp0 = 24;
	}
	Draw24x24Str(rect[1].xpis, rect[1].ypis, MEM_TITLE_ADD + TLFNADD_ANALOGSET * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET, rect[1].index[0]);
	DrawRect(rect[2].xpis, rect[2].ypis, rect[2].xsize, rect[2].ysize, rect[2].colorT[0]); //上部框绘制
	DrawRect(rect[3].xpis, rect[3].ypis, rect[3].xsize, rect[3].ysize, rect[3].colorT[0]); //下部框绘制

	for (i = KEY_ANASET_BACK; i < keynum; i++)
	{
		if ((i <= KEY_ANASET_ARAI) && (i >= KEY_ANASET_VMAX))
		{
		}
		else
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}
	for (i = 0; i < 6; i++)
	{
		if (i <= 2)
		{
			DrawFillRect(rect[4].xpis, rect[4].ypis + 37 * i, rect[4].xsize, rect[4].ysize, rect[4].colorB[0]);
			if (i == 1)
				Draw24x24Str(rect[4].xpis + SAFE_DIST4 + dp0, rect[4].ypis + SAFE_DIST4 + 37 * i, MEM_TITLE_ADD + (rect[4].index[1] + i + 1) * TitleLengthMax, SheetPage, (rect[4].index[1] + i + 1), rect[4].index[0]); //单踏板输入电压
			else if (i == 2)
				Draw24x24Str(rect[4].xpis + SAFE_DIST4 + dp0, rect[4].ypis + SAFE_DIST4 + 37 * i, MEM_TITLE_ADD + (rect[4].index[1] + i - 1) * TitleLengthMax, SheetPage, (rect[4].index[1] + i - 1), rect[4].index[0]);
			else
				Draw24x24Str(rect[4].xpis + SAFE_DIST4 + dp0, rect[4].ypis + SAFE_DIST4 + 37 * i, MEM_TITLE_ADD + (rect[4].index[1] + i) * TitleLengthMax, SheetPage, (rect[4].index[1] + i), rect[4].index[0]);
			Draw24X24FStr(rect[6].xpis, rect[6].ypis + SAFE_DIST4 + 37 * i, "V", rect[6].index[0]);
		}
		else if (i == 3)
		{
			DrawFillRect(rect[4].xpis, rect[4].ypis + 37 * i + 19, rect[4].xsize, rect[4].ysize, rect[4].colorB[0]);
			Draw24x24Str(rect[4].xpis + SAFE_DIST4 + dp0, rect[4].ypis + SAFE_DIST4 + 37 * i + 19, MEM_TITLE_ADD + (rect[4].index[2]) * TitleLengthMax, SheetPage, (rect[4].index[2]), rect[4].index[0]);
			Draw24X24FStr(rect[6].xpis, rect[6].ypis + SAFE_DIST4 + 37 * i + 19, "A", rect[6].index[0]);
		}
		else
		{
			DrawFillRect(rect[4].xpis, rect[4].ypis + 37 * i + 19, rect[4].xsize, rect[4].ysize, rect[4].colorB[0]);
			Draw24x24Str(rect[4].xpis + SAFE_DIST4 + dp0, rect[4].ypis + SAFE_DIST4 + 37 * i + 19, MEM_TITLE_ADD + (rect[4].index[1] + i - 1) * TitleLengthMax, SheetPage, (rect[4].index[1] + i - 1), rect[4].index[0]);
			if (i != 5)
				Draw24X24FStr(rect[6].xpis, rect[6].ypis + SAFE_DIST4 + 37 * i + 19, "A", rect[6].index[0]);
		}
	}
	// Draw24x24Str(108 - dp1, 104, MEM_TITLE_ADD + (TLFNADD_ANALOGSET + 13) * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET + 13, Color_Black); //最小值
	// Draw24X24FStr(300, 104, "V", Color_Black);
	// Draw24x24Str(108 - dp1, 154, MEM_TITLE_ADD + (TLFNADD_ANALOGSET + 12) * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET + 12, Color_Black); //最大值
	// Draw24X24FStr(300, 154, "V", Color_Black);
	// DrawRect(keyd1[0].xpis, keyd1[0].ypis, keyd1[0].xsize, keyd1[0].ysize, Color_Black);
	// //	DrawRect(keyd1[3].xpis,keyd1[3].ypis,keyd1[3].xsize,keyd1[3].ysize,Color_Black);

	// Draw24x24Str(4, 216, MEM_TITLE_ADD + (TLFNADD_ANALOGSET + 10) * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET + 10, Color_Black); //车头板输出电流
	// Draw24X24FStr(300, 216, "A", Color_Black);
	// Draw24x24Str(82 - dp2, 268, MEM_TITLE_ADD + (TLFNADD_ANALOGSET + 14) * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET + 14, Color_Black); //报警电流
	// Draw24X24FStr(300, 268, "A", Color_Black);
	// DrawRect(keyd1[1].xpis, keyd1[1].ypis, keyd1[1].xsize, keyd1[1].ysize, Color_Black);

	// Draw24x24Str(82 - dp2, 332, MEM_TITLE_ADD + (TLFNADD_ANALOGSET + 15) * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET + 15, Color_Black); //报警开关

	// Draw24x24Str(134 - dp3, 394, MEM_TITLE_ADD + (TLFNADD_ANALOGSET + 16) * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET + 16, Color_Black); //温度
	// Draw24X24FStr(300, 394, "C", Color_Black);
	// DrawRect(keyd1[2].xpis, keyd1[2].ypis, keyd1[2].xsize, keyd1[2].ysize, Color_Black);

	Read_D(D_Address[SYS] + SYS_TEMPERATURE, 1, (U16 *)(&YREG[SYS_TEMPERATURE]));
	Timer_ms = 0;
	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);
		if (keydata == 0xff)
		{
			if ((keyon == KEY_ANASET_UP) || (keyon == KEY_ANASET_DN) || (keyon == KEY_ANASET_CLR))
			{
			}
			else if ((keyon) && (beeponf))
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER)
		{
			if (keyon == 0)
			{
				keyon = keydata;
#if Debug
				printf("Menu_AnalogSet keyon = %d \r\n", keyon);
#endif
				if (keyon == KEY_ANASET_SCRNSVR)
					DrawRect(36, 0, 92, 32, Color_Green);
				else if ((keyon <= KEY_ANASET_ARAI) && (keyon >= KEY_ANASET_VMAX))
				{
				}
				else
				{
					DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]);
				}
				Times = 0, delay = KEYDT;
			}
			else if (keyon != keydata)
			{
				keydata = 0;
			}
			if (Times > delay)
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (P_KBC == 0)
						Beep_On(BELLT);
					if ((keyon >= KEY_ANASET_0) && (keyon <= KEY_ANASET_9))
					{
						DrawFMenuStrP(MENU_WRITE, keybmpind[keyon].titleStr[0]);
						SetLanguage(MENU_IOTEST);
					}
					else if ((araS == 0) && ((keyon == KEY_ANASET_ARASWON) || (keyon == KEY_ANASET_ARASWOFF)))
					{
						DrawFMenuStr(keybmpind[keyon].titleStr[1]);
					}
					else
					{
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
					}

					delay = KEYFT;
				}
				Times = 0;
				if ((keyon == KEY_ANASET_UP) || (keyon == KEY_ANASET_DN) || (keyon == KEY_ANASET_CLR))
				{
					keyoff = keyon;
				}
			}
		}
		if (keyoff)
		{
			if (keyoff >= KEY_ANASET_VMAX && keyoff <= KEY_ANASET_ARAI)
			{
				settype = keyoff - KEY_ANASET_VMAX;
				disupdata1 = 1;
			}
			else if (keyoff >= KEY_ANASET_0 && keyoff <= KEY_ANASET_9)
			{
				if (settype == 0)
				{
					vmax = (vmax % 100) * 10 + keyoff - KEY_ANASET_0;
					disupdata1 = 1;
				}
				else if (settype == 1)
				{
					vmin = (vmin % 100) * 10 + keyoff - KEY_ANASET_0;
					disupdata1 = 1;
				}
				else if (settype == 2)
				{
					araI = (araI % 100) * 10 + keyoff - KEY_ANASET_0;
					disupdata1 = 1;
				}
			}
			else
			{
				switch (keyoff)
				{
				case KEY_ANASET_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_ANASET_BACK:
					pnoindex--;
					pno = poldno[pnoindex];
					break;

				case KEY_ANASET_ENTER:
					if (vmax > VMAX)
						vmax = VMAX;
					if (vmin > VMAX)
						vmin = VMAX;
					if (araI > IMAX)
						araI = IMAX;

					if (araS != 0 && araS != 1)
						araS = 0;

					if (vmin >= vmax)
					{
						poldno[pnoindex] = MENU_ANALOGSET;
						pnoindex++;
						pno = MENU_RWHELP;
						hleppara = 154;
						break;
					}
					//						YREG[SYS_PEDAL_VMAX] = vmax;
					//						YREG[SYS_PEDAL_VMIN] = vmin;
					//						YREG[SYS_ARA_I] = araI;
					//						YREG[SYS_ARA_S] = araS;
					if (Write_Cmd3(C_ANALOG_SET, vmin, vmax, araI, araS) == 0)
					{
						poldno[pnoindex] = MENU_ANALOGSET;
						pnoindex++;
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
					if (Read_Status() == 0)
					{
						poldno[pnoindex] = MENU_ANALOGSET;
						pnoindex++;
						pno = MENU_RWHELP;
						hleppara = 15;
						break;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{
						poldno[pnoindex] = MENU_ANALOGSET;
						pnoindex++;
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
					pnoindex--;
					pno = poldno[pnoindex];
					break;

				case KEY_ANASET_UP:
					if (settype == 0)
					{
						if (vmax < VMAX)
						{
							vmax++;
							disupdata2 = 1;
						}
					}
					else if (settype == 1)
					{
						if (vmin < VMAX)
						{
							vmin++;
							disupdata3 = 1;
						}
					}
					else if (settype == 2)
					{
						if (araI < IMAX)
						{
							araI++;
							disupdata4 = 1;
						}
					}
					break;

				case KEY_ANASET_DN:
					if (settype == 0)
					{
						if (vmax > VMIN)
						{
							vmax--;
							disupdata2 = 1;
						}
					}
					else if (settype == 1)
					{
						if (vmin > VMIN)
						{
							vmin--;
							disupdata3 = 1;
						}
					}
					else if (settype == 2)
					{
						if (araI > IMIN)
						{
							araI--;
							disupdata4 = 1;
						}
					}
					break;

				case KEY_ANASET_CLR:
					if (settype == 0)
					{
						if (vmax)
						{
							vmax /= 10;
							disupdata2 = 1;
						}
					}
					else if (settype == 1)
					{
						if (vmin)
						{
							vmin /= 10;
							disupdata3 = 1;
						}
					}
					else if (settype == 2)
					{
						if (araI)
						{
							araI /= 10;
							disupdata4 = 1;
						}
					}
					break;

				case KEY_ANASET_ARASWON:
				case KEY_ANASET_ARASWOFF:
					if (araS)
						araS = 0;
					else
						araS = 1;
					disupdata5 = 1;
					break;
				}
			}

			keyoff = 0;
		}
		if ((keyon) && (keydata == 0))
		{
			if (keyon == KEY_IOTEST_SCRNSVR)
				DrawRect(36, 0, 92, 32, Color_Orange);
			else
			{
				if (keyon < KEY_ANASET_VMAX || keyon > KEY_ANASET_ARAI)
				{
					DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
				}
			}
			DrawFillRect(rect[1].xpis, rect[1].ypis, rect[1].xsize, rect[1].ysize, rect[1].colorB[0]);
			Draw24x24Str(rect[1].xpis, rect[1].ypis, MEM_TITLE_ADD + TLFNADD_ANALOGSET * TitleLengthMax, SheetPage, TLFNADD_ANALOGSET, rect[1].index[0]);

			keyon = 0;
			beeponf = 0;
		}

		if (disupdata1)
		{
			disupdata1 = 0;
			for (i = 0; i < 3; i++)
			{

				DrawFillRect(keyd[i + KEY_ANASET_VMAX].xpis, keyd[i + KEY_ANASET_VMAX].ypis, keyd[i + KEY_ANASET_VMAX].xsize, keyd[i + KEY_ANASET_VMAX].ysize, i == settype ? keybmpind[i + KEY_ANASET_VMAX].icon[0] : keybmpind[i + KEY_ANASET_VMAX].icon[1]);
				if (i == settype)
				{
					DrawDottedRect(keyd[i + KEY_ANASET_VMAX].xpis + SAFE_DIST2, keyd[i + KEY_ANASET_VMAX].ypis + SAFE_DIST2, keyd[i + KEY_ANASET_VMAX].xsize - SAFE_DIST3, keyd[i + KEY_ANASET_VMAX].ysize - SAFE_DIST3, keybmpind[i + KEY_ANASET_VMAX].textColor[0]);
				}
				DrawMFS32P(keyd[i + KEY_ANASET_VMAX].xpis + SAFE_DIST5, keyd[i + KEY_ANASET_VMAX].ypis + SAFE_DIST5, tmp_num[i], 4, 2, keybmpind[i + KEY_ANASET_VMAX].textColor[0]);
				// DrawButton(keyd[i + KEY_ANASET_VMAX], (U16 *)(ADD_ICONBMP), i == settype ? KEY_Blue : KEY_Silver, keybmpind[i + KEY_ANASET_VMAX].icon[0]);
				// DrawButtonOn(keyd[i + KEY_ANASET_VMAX], keybmpind[i + KEY_ANASET_VMAX].border[0]);
				//				Draw24x24Str(keyd[i+KEY_IOTEST_VMAX].xpis+(i==2?6:19),keyd[i+KEY_IOTEST_VMAX].ypis+8,MEM_TITLE_ADD+(TLFNADD_ANALOGSET+i+3)*TitleLengthMax,SheetPage,TLFNADD_ANALOGSET+i+3,i==settype?Color_White:Color_Black);	//最大值设定
			}
			// DrawMFS32P(keyd[KEY_ANASET_VMIN].xpis + 14, keyd[KEY_ANASET_VMIN].ypis + 12, vmin, 4, 2, settype == 1 ? Color_White : Color_Black);
			// DrawMFS32P(keyd[KEY_ANASET_ARAI].xpis + 14, keyd[KEY_ANASET_ARAI].ypis + 12, araI, 4, 1, settype == 2 ? Color_White : Color_Black);
		}

		if (disupdata2)
		{
			disupdata2 = 0;
			DrawFillRect(keyd[KEY_ANASET_VMAX].xpis + SAFE_DIST3, keyd[KEY_ANASET_VMAX].ypis + SAFE_DIST3, keyd[KEY_ANASET_VMAX].xsize - SAFE_DIST4, keyd[KEY_ANASET_VMAX].ysize - SAFE_DIST4, keybmpind[KEY_ANASET_VMAX].icon[0]);
			// DrawDottedRect(keyd[KEY_ANASET_VMAX].xpis, keyd[KEY_ANASET_VMAX].ypis, keyd[KEY_ANASET_VMAX].xsize, keyd[KEY_ANASET_VMAX].ysize, keybmpind[KEY_ANASET_VMAX].textColor[0]);
			DrawMFS32P(keyd[KEY_ANASET_VMAX].xpis + SAFE_DIST5, keyd[KEY_ANASET_VMAX].ypis + SAFE_DIST5, vmax, 4, 2, keybmpind[KEY_ANASET_VMAX].textColor[0]);
			// DrawFillRect(keyd[KEY_ANASET_VMAX].xpis + 4, keyd[KEY_ANASET_VMAX].ypis + 12, keyd[KEY_ANASET_VMAX].xsize - 8, 24, Color_KeyBlue);
			// DrawMFS32P(keyd[KEY_ANASET_VMAX].xpis + 14, keyd[KEY_ANASET_VMAX].ypis + 12, vmax, 4, 2, Color_White);
		}

		if (disupdata3)
		{
			disupdata3 = 0;
			DrawFillRect(keyd[KEY_ANASET_VMIN].xpis + SAFE_DIST3, keyd[KEY_ANASET_VMIN].ypis + SAFE_DIST3, keyd[KEY_ANASET_VMIN].xsize - SAFE_DIST4, keyd[KEY_ANASET_VMIN].ysize - SAFE_DIST4, keybmpind[KEY_ANASET_VMIN].icon[0]);
			// DrawDottedRect(keyd[KEY_ANASET_VMIN].xpis, keyd[KEY_ANASET_VMIN].ypis, keyd[KEY_ANASET_VMIN].xsize, keyd[KEY_ANASET_VMIN].ysize, keybmpind[KEY_ANASET_VMIN].textColor[0]);
			DrawMFS32P(keyd[KEY_ANASET_VMIN].xpis + SAFE_DIST5, keyd[KEY_ANASET_VMIN].ypis + SAFE_DIST5, vmin, 4, 2, keybmpind[KEY_ANASET_VMIN].textColor[0]);
			// DrawFillRect(keyd[KEY_ANASET_VMIN].xpis + 4, keyd[KEY_ANASET_VMIN].ypis + 12, keyd[KEY_ANASET_VMIN].xsize - 8, 24, Color_KeyBlue);
			// DrawMFS32P(keyd[KEY_ANASET_VMIN].xpis + 14, keyd[KEY_ANASET_VMIN].ypis + 12, vmin, 4, 2, Color_White);
		}

		if (disupdata4)
		{
			disupdata4 = 0;
			DrawFillRect(keyd[KEY_ANASET_ARAI].xpis + SAFE_DIST3, keyd[KEY_ANASET_ARAI].ypis + SAFE_DIST3, keyd[KEY_ANASET_ARAI].xsize - SAFE_DIST4, keyd[KEY_ANASET_ARAI].ysize - SAFE_DIST4, keybmpind[KEY_ANASET_ARAI].icon[0]);
			// DrawDottedRect(keyd[KEY_ANASET_ARAI].xpis, keyd[KEY_ANASET_ARAI].ypis, keyd[KEY_ANASET_ARAI].xsize, keyd[KEY_ANASET_ARAI].ysize, keybmpind[KEY_ANASET_ARAI].textColor[0]);
			DrawMFS32P(keyd[KEY_ANASET_ARAI].xpis + SAFE_DIST5, keyd[KEY_ANASET_ARAI].ypis + SAFE_DIST5, araI, 4, 2, keybmpind[KEY_ANASET_ARAI].textColor[0]);
			// DrawFillRect(keyd[KEY_ANASET_ARAI].xpis + 4, keyd[KEY_ANASET_ARAI].ypis + 12, keyd[KEY_ANASET_ARAI].xsize - 8, 24, Color_KeyBlue);
			// DrawMFS32P(keyd[KEY_ANASET_ARAI].xpis + 14, keyd[KEY_ANASET_ARAI].ypis + 12, araI, 4, 1, Color_White);
		}

		if (disupdata5)
		{
			disupdata5 = 0;
			if (araS)
			{
				DrawButton(keyd[KEY_ANASET_ARASWON], (U16 *)(ADD_ICONBMP), keybmpind[KEY_ANASET_ARASWON].border[1], keybmpind[KEY_ANASET_ARASWON].icon[1]);
				DrawButton(keyd[KEY_ANASET_ARASWOFF], (U16 *)(ADD_ICONBMP), keybmpind[KEY_ANASET_ARASWOFF].border[0], keybmpind[KEY_ANASET_ARASWOFF].icon[0]);
			}
			else
			{
				DrawButton(keyd[KEY_ANASET_ARASWON], (U16 *)(ADD_ICONBMP), keybmpind[KEY_ANASET_ARASWON].border[0], keybmpind[KEY_ANASET_ARASWON].icon[0]);
				DrawButton(keyd[KEY_ANASET_ARASWOFF], (U16 *)(ADD_ICONBMP), keybmpind[KEY_ANASET_ARASWOFF].border[1], keybmpind[KEY_ANASET_ARASWOFF].icon[1]);
			}
		}
#if 1 //温度
		if (disupdata6)
		{
			disupdata6 = 0;
			// for (i = 0; i < 3; i++)
			// {
			// 	BmpFillRect(keyd1[i].xpis + 14, keyd1[i].ypis + 8, keyd[KEY_ANASET_ARAI].xsize - 16, 24);
			// }
			DrawFillRect(rect[5].xpis, rect[5].ypis, rect[5].xsize, rect[5].ysize, rect[5].colorB[0]);
			DrawMFS32P(rect[5].xpis + SAFE_DIST5, rect[5].ypis + SAFE_DIST5, YREG[SYS_PEDAL_V], 4, 2, rect[5].index[0]);
			DrawFillRect(rect[5].xpis, rect[5].ypis + 130, rect[5].xsize, rect[5].ysize, rect[5].colorB[0]);
			DrawMFS32P(rect[5].xpis + SAFE_DIST5, rect[5].ypis + 128 + SAFE_DIST5, YREG[SYS_CAR_OUTI], 4, 2, rect[5].index[0]);
			// DrawMFS32P(keyd1[1].xpis + 14, keyd1[1].ypis + 8, YREG[SYS_CAR_OUTI], 4, 1, Color_Black);
			// DrawMFS32P(keyd1[2].xpis + 14, keyd1[2].ypis + 8, (S16)YREG[SYS_TEMPERATURE], 4, 0, Color_Black);
		}

		if (Timer_ms > 500)
		{
			Timer_ms = 0;
			disupdata6 = 1;
			// Read_D(D_Address[SYS] + SYS_TEMPERATURE, 1, (U16 *)(&YREG[SYS_TEMPERATURE]));
			Read_D(D_Address[SYS] + SYS_PEDAL_V, 6, (U16 *)(&YREG[SYS_PEDAL_V]));
		}
#endif
		if (pno != MENU_ANALOGSET)
		{
			plastno = MENU_ANALOGSET;
			break;
		}
		updateViewStatus();
		wdt();
	}
}
