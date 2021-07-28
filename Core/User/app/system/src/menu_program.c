//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  menu_program.c							*
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
#include "menu_program.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "uart.h"
#include "main.h"
#include "utility.h"
#include "color.h"
#include "communication.h"
#include "ds2460.h"
#include "Toast.h"

#define TLFNADD_PROG 0
#define TLFNLEN_PROG 24 //0
#define TLFNADD_FPROG (TLFNADD_PROG + TLFNLEN_PROG)
#define TLFNLEN_FPROG 15 //1
#define TLFNADD_FPROGS (TLFNADD_FPROG + TLFNLEN_FPROG)
#define TLFNLEN_FPROGS 7 //1

extern U16 SheetPage;
extern volatile U8 gPeessVal;
extern volatile U8 g_modifyflag;
extern volatile U8 g_Restorepropara; //还原修改参数 0:未还原  1:设置还原  2:还原成功

// volatile U8 g_pageno; //参数页码
U16 g_parapage;			  // 参数类序号
U16 g_lpage;			  // 参数页
U16 g_parano;			  // 参数序号
U8 g_modeselectf;		  //模式选择-0:已修改;1：当前修改
U16 g_pageno1, g_pageno2; //修改过的参数页码,当前修改的参数页码
U16 g_paraNoLimit = 0;
volatile U8 g_prochange;

//U8 Restoreflag =0;  //还原修改参数 0:未还原  1:设置还原  2:还原成功

U32 Menu_ProgramNumber(U32 para, U32 *def, U32 vmin, U32 vmax, U16 *unit);
U32 Menu_ProgramLogic(U32 *def, U32 vmin, U32 vmax);
void Menu_ProgramLogicHelp(U32 def);

//--------------------------------------------
#define PROG_FAST_MAX 183
#define PROG_MAX 255

// #define setParaNoLimit(limit) g_paraNoLimit = limit
static void setParaNoLimit(U16 limit)
{
	g_paraNoLimit = limit;
}
// #define getParaNoLimit() g_paraNoLimit
static U16 getParaNoLimit(void)
{
	return g_paraNoLimit;
}

//根据编号获取参数简称
U8 getCodeByNo(U8 *code, U16 progno)
{
	U8 codelen = 0;

	for (codelen = 0; codelen < 6; codelen++)
	{
		*(code + codelen) = 0;
	}
	codelen = 0;
	if (progno < g_paraNoLimit)
	{
		U8 *p = (U8 *)(MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME);
		for (codelen = 0; codelen < 6; codelen++)
		{
			if (*(p + codelen) == 0)
			{
				break;
			}
			*(code + codelen) = *(p + codelen);
		}
	}
	wdt();
	return codelen;
}

//显示参数的标题简称
void disProgCode(U16 xpos, U16 ypos, U16 xsize, U16 ysize, tyden_alignment alignment, tyden_strSize size, U16 progno, U16 color)
{
	U16 PROG_MSG2_Sheet = 45;
	if (progno >= g_paraNoLimit)
	{
		return;
	}
	if (size == STR24X12)
	{
		if (alignment == CENTER)
		{
			xpos += (xsize - 24 * strlen((char *)(MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME))) / 2;
			ypos += (ypos - 24) / 2;
		}
		// Draw24X24FFStr(xpos, ypos, (MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME), color);
		Draw24x24Str(xpos, ypos, (MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME), PROG_MSG2_Sheet, 1, color);
	}
	else if (size == STR16X8)
	{
		if (alignment == CENTER)
		{
			xpos += (xsize - 8 * strlen((char *)(MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME))) / 2;
			ypos += (ypos - 16) / 2;
		}
		// Draw24X24FFStr(xpos, ypos, (MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME), color);
		Draw16x16Str(xpos, ypos, (MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_PARANAME), PROG_MSG2_Sheet, 1, color);
	}
}

//显示参数的标题信息
void disProgDesc(U16 xpos, U16 ypos, U16 progno, U8 linemus, U8 maxline, U16 color)
{
	U16 PROG_MSG2_Sheet = 45;
	if (progno >= g_paraNoLimit)
	{
		return;
	}
	Draw24x24MulStr(xpos, ypos, (MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_MSG), linemus, PROG_MSG2_Sheet, progno, maxline, color);
}

//根据编号获取参数值类型
U16 getProgType(U16 progno)
{
	if (progno >= g_paraNoLimit)
	{
		return 0;
	}
	return *(MEM_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_TYPE);
}

//显示参数的值(数值/选项)
void disProgVal(U16 xpos, U16 ypos, U16 xsize, U16 ysize, tyden_alignment alignment, U16 progno, U16 value, U16 type, U16 color)
{
	U32 add;
	U16 PROG_MSG3_Sheet = 46;
	if (progno >= g_paraNoLimit)
	{
		return;
	}

	if (type == 1)
	{ //数字
		if (value > 9999)
		{
			if (alignment == CENTER)
			{
				xpos += (xsize - 12 * 5) / 2;
				ypos += (ysize - 24) / 2;
			}
			DrawMFU32ZR(xpos, ypos, value, 5, color);
		}
		else
		{
			if (alignment == CENTER)
			{
				xpos += (xsize - 12 * 4) / 2;
				ypos += (ysize - 24) / 2;
			}
			else
			{
				xpos += 12;
			}
			DrawMFU32ZR(xpos, ypos, value, 4, color);
		}
	}
	else
	{ //逻辑
		add = (*(USE_PROG_MSG2 + progno * MSG2_LENGTH + PMSG2_MSG3));
		add += value;
		if (alignment == CENTER)
		{
			xpos += (xsize - 12 * strlen((char *)((MEM_PROG_MSG3 + add * MSG3_LENGTH + PMSG3_PARANAME)))) / 2;
			ypos += (ysize - 24) / 2;
		}
		else
		{
			xpos += 12;
		}
		// Draw24X24FFStr(xpos, ypos, (MEM_PROG_MSG3 + add * MSG3_LENGTH + PMSG3_PARANAME), color);
		Draw24x24Str(xpos, ypos, (MEM_PROG_MSG3 + add * MSG3_LENGTH + PMSG3_PARANAME), PROG_MSG3_Sheet, 1, color);
	}
}

//根据编号获取参数当前值
U16 getProgVal(U16 progno)
{
	if (progno >= g_paraNoLimit)
	{
		return 0;
	}
	return PREG[progno * PREG_LENGTH + PREG_NOW];
}

//根据编号设置参数当前值
void setProgVal(U16 progno, U16 val)
{
	U16 paraNowIndex = progno * PREG_LENGTH + PREG_NOW;
	if (progno >= g_paraNoLimit)
	{
		return;
	}
	PREG[paraNowIndex] = val;
	Write_D((D_Address[PARA] + paraNowIndex), 1, (U16 *)&PREG[paraNowIndex]);
}

//根据编号获取参数最小值
U16 getProgMin(U16 progno)
{
	if (progno >= g_paraNoLimit)
	{
		return 0;
	}
	return PREG[progno * PREG_LENGTH + PREG_MIN];
}
//根据编号获取参数最大值
U16 getProgMax(U16 progno)
{
	if (progno >= g_paraNoLimit)
	{
		return 0;
	}
	return PREG[progno * PREG_LENGTH + PREG_MAX];
}

//--- 判断字符串是否完全相等
U8 strComp(U8 *p1, U8 *p2)
{
	U16 len = 0, len1 = 0, len2 = 0;
	while (*(p1 + len1) != 0)
		len1++;
	while (*(p2 + len2) != 0)
		len2++;
	if (len1 != len2 || len1 == 0 || len2 == 0)
	{
		return 0;
	}

	while (*(p1 + len) == *(p2 + len) && *(p1 + len) != 0)
	{
		len++;
	}
	wdt();
	if (len != len1)
	{
		return 0;
	}
	return 1;
}

//-获取完全重合时的参数序号
U16 getNoByCode(U8 *code)
{
	U16 no = 0xffff;
	U8 *p;
	U16 i;
	for (i = 0; i < g_paraNoLimit; i++)
	{
		p = (U8 *)(MEM_PROG_MSG2 + i * MSG2_LENGTH + PMSG2_PARANAME);
		if (strComp(p, code))
		{
			no = i;
			break;
		}
	}
	return no;
}

//--- 判断字符串是否相等-返回相等字符数
U8 strComp1(U8 *p1, U8 *p2, U8 len)
{
	U8 num = 0;
	if (len == 0)
	{
		return 0;
	}
	for (num = 0; num < len; num++)
	{
		if (*(p1 + num) != *(p2 + num))
		{
			break;
		}
	}
	return num;
}

//--- 判断输入代码，判断输入代码是否存在于列表中，有返回对应序号1-256，无返回0
U16 getAdjNoByCode(U8 *code, U8 len)
{
	U16 no = 0;
	U8 *p;
	U16 i;
	U8 res = 0, res1 = 0;
	for (i = 0; i < g_paraNoLimit; i++)
	{
		p = (U8 *)(MEM_PROG_MSG2 + i * MSG2_LENGTH + PMSG2_PARANAME);
		if (strComp(p, code))
		{
			no = i;
			break;
		}
		else
		{
			res = strComp1(p, code, len);
			if (res > res1)
			{
				no = i;
				res1 = res;
			}
		}
	}
	return no;
}

//------------------------------------------------
#define KEY_PROG_SCRNSVR 1
#define KEY_PROG_BACK 2 //返回
#define KEY_PROG_PREV 3 //上一页
#define KEY_PROG_NEXT 4 //下一页
#define KEY_PROG_DIFFET 5
#define KEY_PROG_SELC1 6
#define KEY_PROG_SELC2 7
#define KEY_PROG_SELC3 8
#define KEY_PROG_SELC4 9
#define KEY_PROG_SELC5 10
#define KEY_PROG_SELC6 11
#define KEY_PROG_SELC7 12
#define KEY_PROG_SELC8 13
#define KEY_PROG_SELC9 14
#define KEY_PROG_MAX 15

#define RECT_PROG_PAGE 1

#define PORG_SELC 9
#define PAGEMAXNO 3 //3-最大页数
#define PROGNUM 21	//最大参数个数

//------------------------------------------
// Function   : void Menu_Prog(void)
// Description: 参数主界面
//------------------------------------------
void Menu_Prog(void)
{
	// U8 page_flash = 1;
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U16 PROG_MSG11_Sheet = 43;
	U8 linemaxdis = 16;																  //一行显示最大字符数
	tydPatternList ParaList = {1, 0, 0, 9, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数
	const U8 keynum = {KEY_PROG_MAX};															  //
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},												  //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},								  //2 返回
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},								  //3 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},								  //4 下一页
		{TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},								  //5 查看已修改的参数
		{TYPE_X90, TYPE_Y6, KEY_X9, KEY_Y9},							  //6 参数选项1
		{TYPE_X91, TYPE_Y6, KEY_X9, KEY_Y9},							  //7 参数选项2
		{TYPE_X92, TYPE_Y6, KEY_X9, KEY_Y9},							  //8 参数选项3
		{TYPE_X90, TYPE_Y6 + (KEY_Y9 + SAFE_DIST10), KEY_X9, KEY_Y9},	  //9 参数选项4
		{TYPE_X91, TYPE_Y6 + (KEY_Y9 + SAFE_DIST10), KEY_X9, KEY_Y9},	  //10 参数选项5
		{TYPE_X92, TYPE_Y6 + (KEY_Y9 + SAFE_DIST10), KEY_X9, KEY_Y9},	  //11 参数选项6
		{TYPE_X90, TYPE_Y6 + (KEY_Y9 + SAFE_DIST10) * 2, KEY_X9, KEY_Y9}, //12 参数选项7
		{TYPE_X91, TYPE_Y6 + (KEY_Y9 + SAFE_DIST10) * 2, KEY_X9, KEY_Y9}, //13 参数选项8
		{TYPE_X92, TYPE_Y6 + (KEY_Y9 + SAFE_DIST10) * 2, KEY_X9, KEY_Y9}, //14 参数选项9
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},					//1 屏保
		{{KEY_Silver}, {0}, {241, 242}, {5, 0, 0, 0}, {0}},				//2 返回
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {6, 0, 0, 0}, {0}},	//3 上一页
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {7, 0, 0, 0}, {0}},	//4 下一页
		{{KEY_Silver}, {0}, {240}, {15, 0, 0, 0}, {0}},					//5查看已修改的参数
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {2, 0, 0, 0}, {0}},	//6 选项1 修线器
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {7, 0, 0, 0}, {0}},	//7 选项2 断线检出器
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {3, 0, 0, 0}, {0}},	//8 选项3 起步慢针
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {8, 0, 0, 0}, {0}},	//9 选项4 原点位置
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {4, 0, 0, 0}, {0}},	//10 选项5 压板
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {9, 0, 0, 0}, {0}},	//11 选项6 暂停
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {5, 0, 0, 0}, {0}},	//12 选项7 范围限制
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {10, 0, 0, 0}, {0}}, //13 选项8 计算器
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {6, 0, 0, 0}, {0}},	//14 选项9 机针位置

		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {11, 0, 0, 0}, {0}}, //15 选项1 马达制动
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {12, 0, 0, 0}, {0}}, //16 选项2 中压脚
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {17, 0, 0, 0}, {0}}, //17 选项3 程序
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {13, 0, 0, 0}, {0}}, //18 选项4 绕线芯
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {18, 0, 0, 0}, {0}}, //19 选项5 移动
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {14, 0, 0, 0}, {0}}, //20 选项6 移动方式
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {19, 0, 0, 0}, {0}}, //21 选项7 送料角度
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {15, 0, 0, 0}, {0}}, //22 选项8 速度
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {20, 0, 0, 0}, {0}}, //23 选项9 液晶屏幕

		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {16, 0, 0, 0}, {0}}, //24 选项1 剪线时序
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {21, 0, 0, 0}, {0}}, //25 选项2 其他
		{{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {22, 0, 0, 0}, {0}}, //26 选项3 特殊

	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {0}},

		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示

	};
	SetLanguage(MENU_PROG);
	InitScreen(); //清屏
	initViewStatus();
	updateViewStatus();
	setParaNoLimit(PROG_MAX);
	// USE_PROG_MSG2SE = MEM_PROG_MSG21SE;
	UADD_PROG_MSG2SE = ADD_PROG_MSG21SE;
	if (P_LAN)
	{
		// USE_PROG_MSG1 = MEM_PROG_MSG11_E;
		// USE_PROG_MSG2 = MEM_PROG_MSG2_E;
		// USE_PROG_MSG3 = MEM_PROG_MSG3_E;
		// USE_PROG_MSG4 = MEM_PROG_MSG4_E;
		UADD_PROG_MSG1 = ADD_PROG_MSG11_E;
		UADD_PROG_MSG2 = ADD_PROG_MSG2_E;
		UADD_PROG_MSG3 = ADD_PROG_MSG3_E;
		UADD_PROG_MSG4 = ADD_PROG_MSG4_E;
	}
	else
	{
		// USE_PROG_MSG1 = MEM_PROG_MSG11;
		// USE_PROG_MSG2 = MEM_PROG_MSG2;
		// USE_PROG_MSG3 = MEM_PROG_MSG3;
		// USE_PROG_MSG4 = MEM_PROG_MSG4;
		UADD_PROG_MSG1 = ADD_PROG_MSG11;
		UADD_PROG_MSG2 = ADD_PROG_MSG2;
		UADD_PROG_MSG3 = ADD_PROG_MSG3;
		UADD_PROG_MSG4 = ADD_PROG_MSG4;
	}
	// g_lpage = 1;
	// if ((g_pageno > PAGEMAXNO) || (g_pageno < 1))
	// 	g_pageno = 1;

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0], SheetPage, rect[0].index[0], rect[0].colorT[0]);
	//按顺序画出返回，上一页，下一页的按钮

	DrawButton(keyd[KEY_PROG_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_PROG_BACK].border[0], keybmpind[KEY_PROG_BACK].icon[0]);
	DrawButton(keyd[KEY_PROG_DIFFET], (U16 *)(ADD_ICONBMP), keybmpind[KEY_PROG_DIFFET].border[0], keybmpind[KEY_PROG_DIFFET].icon[0]);

	ViewPage(usepageno); //指定显示页面

	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//第一页的上一页按钮，最后一页的下一页按钮和4到9按钮无效
		if (keydata == KEY_PROG_PREV)
		{
			if (ParaList.pageNow == 1)
			{
				keydata = 0;
			}
		}
		if (keydata == KEY_PROG_NEXT)
		{
			if (ParaList.pageNow == ParaList.pageNum)
			{
				keydata = 0;
			}
		}
		if (keydata >= KEY_PROG_SELC1 && keydata <= KEY_PROG_SELC9)
		{
			i = keydata - KEY_PROG_SELC1;
			if (ParaList.selectEnable[i] == 0)
			{
				keydata = 0;
			}
		}

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
				if (keyon == KEY_PROG_SCRNSVR)
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
					case KEY_PROG_SELC1:
					case KEY_PROG_SELC2:
					case KEY_PROG_SELC3:
					case KEY_PROG_SELC4:
					case KEY_PROG_SELC5:
					case KEY_PROG_SELC6:
					case KEY_PROG_SELC7:
					case KEY_PROG_SELC8:
					case KEY_PROG_SELC9:

						DrawFMenuStrP(MENU_PROGMSG11, keybmpind[(keyon + ParaList.disNumDiv * (ParaList.pageNow - 1))].titleStr[0]);
						break;
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
			if (keyoff >= KEY_PROG_SELC1 && keyoff <= KEY_PROG_SELC9)
			{
				pno = MENU_PROFRAME;
				poldno[pnoindex++] = MENU_PROG;
				g_parapage = keybmpind[((ParaList.pageNow - 1) * ParaList.disNumDiv + keyoff)].titleStr[0] - 1;
				if (g_parapage == ParaList.patternNum)
				{
#if 0
					if ((g_user.newuser > 7) && (g_user.newuser < 10))
					{
						pno = MENU_PROFRAME;
					}
					else
					{
						// poldno[2] = MENU_PROFRAME;
						poldno[pnoindex++] = MENU_PROFRAME;
						pno = MENU_FTUSER;
					}
#else
					pno = MENU_PROFRAME;
#endif
				}
				else
				{
					pno = MENU_PROFRAME;
				}
			}
			else
			{

				switch (keyoff)
				{
				case KEY_PROG_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_PROG_BACK: //返回
					if (g_prochange)
					{
						if (Write_Cmd(C_UPDATEPARA, 0, 0) == 0)
						{
							pno = MENU_RWHELP;
							poldno[pnoindex++] = MENU_PROG;
							hleppara = 15;
							// poldno[1] = MENU_PROG;
							// pnoindex = 2;
							break;
						}
						gPeessVal = P_CTR;
						if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL)
							gPeessVal = PEESS_DEfVAL;
						WritePessVal(gPeessVal);
					}
					// pno = poldno[0];
					// pnoindex = 0;
					pno = poldno[--pnoindex];
					Menu_DesktopInit();
					break;
				case KEY_PROG_PREV: //上一页
					if (ParaList.pageNow > 1)
					{
						ParaList.pageNow--;
						ParaList.bFlashPageFlag = 1;
						// page_flash = 1;
					}
					break;
				case KEY_PROG_NEXT: //下一页
					if (ParaList.pageNow < ParaList.pageNum)
					{
						ParaList.pageNow++;
						ParaList.bFlashPageFlag = 1;
						// page_flash = 1;
					}
					break;

				case KEY_PROG_DIFFET:
					pno = MENU_DIFFERENT;
					poldno[pnoindex++] = MENU_PROG;
					g_modeselectf = 0;
					for (i = 0; i < 255; i++)
						g_publicbuf[i + 256] = PREG[i * PREG_LENGTH + PREG_NOW];

					break;
				}
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_PROG_SCRNSVR) //不需要显示的按键
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

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;

			ParaList.patternNum = PROGNUM;
			ParaList.pageNow = 1;
			ParaList.pageLast = ParaList.pageNow;
			ParaList.pageNum = (ParaList.patternNum - 1) / ParaList.disNumDiv + 1;

			if (ParaList.pageNum == 0)
				ParaList.pageNum = 1;

			if (ParaList.pageNow == 0)
				ParaList.pageNow = 1;
			else if (ParaList.pageNow > ParaList.pageNum)
				ParaList.pageNow = ParaList.pageNum;
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U8 len = (ParaList.pageNow < ParaList.pageNum) ? ParaList.disNumDiv : (ParaList.patternNum - (ParaList.pageNow - 1) * ParaList.disNumDiv);
			U8 offset;
			ParaList.bFlashPageFlag = 0;

			//显示页码
			DrawFillRect(rect[RECT_PROG_PAGE].xpis, rect[RECT_PROG_PAGE].ypis,
						 rect[RECT_PROG_PAGE].xsize, rect[RECT_PROG_PAGE].ysize, rect[RECT_PROG_PAGE].colorB[0]);
			DrawFU32(rect[RECT_PROG_PAGE].xpis + SAFE_DIST2, rect[RECT_PROG_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNow, 3, rect[RECT_PROG_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_PROG_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_PROG_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_PROG_PAGE].colorT[0]);
			DrawFU32(rect[RECT_PROG_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_PROG_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNum, 3, rect[RECT_PROG_PAGE].colorT[0]);

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				if (i < len)
					ParaList.selectEnable[i] = 1;
				else
					ParaList.selectEnable[i] = 0;
			}
			for (offset = 0; offset < ParaList.disNumDiv; offset++)
			{
				i = KEY_PROG_SELC1 + offset;
				if (ParaList.selectEnable[offset] == 1)
				{
					tydDisSrcIndex keybAdd = (keybmpind[(i + ParaList.disNumDiv * (ParaList.pageNow - 1))]);
					DrawButtonSingleColor(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
					Draw24x24MulStr(keyd[i].xpis + (keyd[i].xsize - strlen((char *)(MEM_PROG_MSG11 + keybAdd.titleStr[0] * MSG1_LENGTH)) * 12) / 2,
									keyd[i].ypis + (keyd[i].ysize - 24) / 2, (MEM_PROG_MSG11 + (keybAdd.titleStr[0] * MSG1_LENGTH)),
									linemaxdis, PROG_MSG11_Sheet, keybAdd.titleStr[0], 2, keybAdd.textColor[0]);
				}
				else
				{
					BmpFillRect(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
				}
			}
			DrawButton(keyd[KEY_PROG_PREV], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROG_PREV].border[1] : keybmpind[KEY_PROG_PREV].border[0],
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROG_PREV].icon[1] : keybmpind[KEY_PROG_PREV].icon[0]);
			DrawButton(keyd[KEY_PROG_NEXT], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROG_NEXT].border[1] : keybmpind[KEY_PROG_NEXT].border[0],
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROG_NEXT].icon[1] : keybmpind[KEY_PROG_NEXT].icon[0]);

			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		ErrorMsg(MENU_PROG);

		if (pno != MENU_PROG)
		{
			plastno = MENU_PROG;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//------------------------------------------------------
#define KEY_PROF_SCRNSVR 1
#define KEY_PROF_BACK 2
#define KEY_PROF_PREV 3
#define KEY_PROF_NEXT 4
#define KEY_PROF_SELC1 5
#define KEY_PROF_SELC2 6
#define KEY_PROF_SELC3 7
#define KEY_PROF_SELC4 8
#define KEY_PROF_SELC5 9
#define KEY_PROF_MAX 10

#define RECT_PROF_PAGE 1
#define RECT_PROF_PARA 2
#define RECT_PROF_STR 3
#define RECT_PROF_NUM 4

//------------------------------------------
// Function   : void Menu_ProFrame(void)
// Description: 编程设置菜单框架
//------------------------------------------
void Menu_ProFrame(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U16 PROG_MSG2_Sheet = 45;
	//U16 PROG_MSG11_Sheet = 43;
	//	U16 RectWinth = 464;
	tydPatternList ParaList = {1, 0, 0, 5, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数
	const U8 keynum = {10};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},								//1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},				//2 返回
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},				//3 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},				//4 下一页
		{TYPE_X35, TYPE_Y44, KEY_X7, KEY_Y5},			//5 参数选项1
		{TYPE_X35, TYPE_Y44 + 74, KEY_X7, KEY_Y5},		//6 参数选项2
		{TYPE_X35, TYPE_Y44 + 2 * 74, KEY_X7, KEY_Y5},	//7 参数选项3
		{TYPE_X35, TYPE_Y44 + 3 * 74, KEY_X7, KEY_Y5},	//8 参数选项4
		{TYPE_X35, TYPE_Y44 + 4 * 74, KEY_X7, KEY_Y5}}; //9 参数选项5
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},					//1 屏保
		{{KEY_Silver}, {0}, {72}, {8, 0, 0, 0}, {0}},					//2 返回
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {6, 0, 0, 0}, {0}},	//3 上一页
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {7, 0, 0, 0}, {0}},	//4 下一页
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {0, 0, 0, 0}, {0}}, //6  选项1
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {0, 0, 0, 0}, {0}}, //7  选项2
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {0, 0, 0, 0}, {0}}, //8  选项3
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {0, 0, 0, 0}, {0}}, //9  选项4
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {0, 0, 0, 0}, {0}}, //10 选项5
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {1}},

		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示
		{TYPE_X45, TYPE_Y2, 632, 66, {Color_Black}, {Color_Black}, {0}},									   //2 整体框显示框
		{TYPE_X45 + 1, TYPE_Y2 + 1, 630, 32, {Color_Silver}, {Color_Black}, {0}},							   //3 文本显示框
		{TYPE_X45 + 1, TYPE_Y2 + 33, 630, 32, {Color_White}, {Color_Black}, {0}},							   //4 参数显示框

	};
	SetLanguage(MENU_PROG);
	InitScreen();
	initViewStatus();
	updateViewStatus();

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
				 SheetPage, rect[0].index[0], rect[0].colorT[0]);
	DrawButton(keyd[KEY_PROF_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_PROF_BACK].border[0], keybmpind[KEY_PROF_BACK].icon[0]);

	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//第一页的上一页按钮，最后一页的下一页按钮和4到9按钮无效
		if (keydata == KEY_PROF_PREV)
		{
			if (ParaList.pageNow == 1)
			{
				keydata = 0;
			}
		}
		if (keydata == KEY_PROF_NEXT)
		{
			if (ParaList.pageNow == ParaList.pageNum)
			{
				keydata = 0;
			}
		}
		if (keydata >= KEY_PROF_SELC1 && keydata <= KEY_PROF_SELC5)
		{
			i = keydata - KEY_PROF_SELC1;
			if (ParaList.selectEnable[i] == 0)
			{
				keydata = 0;
			}
		}

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
				if (keyon == KEY_PROF_SCRNSVR)
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
					case KEY_PROF_SELC1:
					case KEY_PROF_SELC2:
					case KEY_PROF_SELC3:
					case KEY_PROF_SELC4:
					case KEY_PROF_SELC5:
						// i = (ParaList.pageNow - 1) * ParaList.disNumDiv + (*(MEM_PROG_MSG2 + (g_parapage * 2 - 2))) + keyon - KEY_PROF_SELC1; //参数号
						BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
						// Draw24X24FFStr(TITLE_XPOS, SAFE_DIST4, (MEM_PROG_MSG2 + ParaList.dataAdd[keyon - KEY_PROF_SELC1] * MSG2_LENGTH + PMSG2_PARANAME), rect[0].colorT[0]);
						Draw24x24Str(TITLE_XPOS, SAFE_DIST4, (MEM_PROG_MSG2 + ParaList.dataAdd[keyon - KEY_PROF_SELC1] * MSG2_LENGTH + PMSG2_PARANAME), PROG_MSG2_Sheet, 1, rect[0].colorT[0]);

						break;
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

		if (keyoff)
		{
			if (keyoff >= KEY_PROF_SELC1 && keyoff <= KEY_PROF_SELC5)
			{
				U8 f = keyoff - KEY_PROF_SELC1;
				g_parano = (*(MEM_PROG_MSG21SE + (g_parapage * 2 - 2))) + (ParaList.pageNow - 1) * ParaList.disNumDiv + f;
				pno = MENU_PROPARA;
				poldno[pnoindex++] = MENU_PROFRAME;
				ParaList.bFlashSelectFlag = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_PROF_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_PROF_BACK:
					pno = poldno[--pnoindex];
					Read_D(D_Address[PARA] + 468, 16, (U16 *)(&PREG[468])); // 读取速度设置参数值
					if (g_parapage == 21)
					{
						if (g_user.newmode == 2)
						{
							g_user.newuser = g_user.olduser;
							g_user.newmode = g_user.oldmode;
							g_user.olduser = 0;
							g_user.oldmode = 0;
						}
					}
					break;
				case KEY_PROF_PREV:
					if (ParaList.pageNow > 1)
					{
						ParaList.pageNow--;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				case KEY_PROF_NEXT:
					if (ParaList.pageNow < ParaList.pageNum)
					{
						ParaList.pageNow++;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				}
			}
			keyoff = 0;
		}

		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_PROF_SCRNSVR) //不需要显示的按键
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

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;

			ParaList.patternNum = (*(MEM_PROG_MSG21SE + (g_parapage * 2 - 1)));
			ParaList.pageNow = 1;
			ParaList.pageLast = ParaList.pageNow;
			ParaList.pageNum = (ParaList.patternNum - 1) / ParaList.disNumDiv + 1;

			if (ParaList.pageNum == 0)
				ParaList.pageNum = 1;

			if (ParaList.pageNow == 0)
				ParaList.pageNow = 1;
			else if (ParaList.pageNow > ParaList.pageNum)
				ParaList.pageNow = ParaList.pageNum;
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U8 len = (ParaList.pageNow < ParaList.pageNum) ? ParaList.disNumDiv : (ParaList.patternNum - (ParaList.pageNow - 1) * ParaList.disNumDiv);
			U16 stAdd;
			U8 linemaxdis = 35;
			ParaList.bFlashPageFlag = 0;

			//显示页码
			DrawFillRect(rect[RECT_PROF_PAGE].xpis, rect[RECT_PROF_PAGE].ypis,
						 rect[RECT_PROF_PAGE].xsize, rect[RECT_PROF_PAGE].ysize, rect[RECT_PROF_PAGE].colorB[0]);
			DrawFU32(rect[RECT_PROF_PAGE].xpis + SAFE_DIST2, rect[RECT_PROF_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNow, 3, rect[RECT_PROF_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_PROF_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_PROF_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_PROF_PAGE].colorT[0]);
			DrawFU32(rect[RECT_PROF_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_PROF_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNum, 3, rect[RECT_PROF_PAGE].colorT[0]);

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				if (i < len)
					ParaList.selectEnable[i] = 1;
				else
					ParaList.selectEnable[i] = 0;
			}

			stAdd = (ParaList.pageNow - 1) * ParaList.disNumDiv + (*(MEM_PROG_MSG21SE + (g_parapage * 2 - 2))); // 计算(当前页)prog_msg2的起始地址
			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				U8 keyIndex = KEY_PROF_SELC1 + i;
				U16 rectdy = 74 * i;
				if (ParaList.selectEnable[i] == 1)
				{
					DrawButtonSingleColor(keyd[keyIndex], keybmpind[keyIndex].border[0], keybmpind[keyIndex].icon[0]);
					// Draw24X24FFStr(keyd[keyIndex].xpis + (keyd[keyIndex].xsize - strlen((char *)(MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_PARANAME)) * 12) / 2,
					// 			   keyd[keyIndex].ypis + (keyd[keyIndex].ysize - 24) / 2,
					// 			   (MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_PARANAME), keybmpind[keyIndex].textColor[0]);
					Draw24x24Str(keyd[keyIndex].xpis + (keyd[keyIndex].xsize - strlen((char *)(MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_PARANAME)) * 12) / 2,
								 keyd[keyIndex].ypis + (keyd[keyIndex].ysize - 24) / 2,
								 (MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_PARANAME), PROG_MSG2_Sheet, 1, keybmpind[keyIndex].textColor[0]);

					ParaList.dataAdd[i] = stAdd;
					DrawRect(rect[RECT_PROF_PARA].xpis, rect[RECT_PROF_PARA].ypis + rectdy,
							 rect[RECT_PROF_PARA].xsize, rect[RECT_PROF_PARA].ysize, rect[RECT_PROF_PARA].colorB[0]);
					DrawFillRect(rect[RECT_PROF_STR].xpis, rect[RECT_PROF_STR].ypis + rectdy,
								 rect[RECT_PROF_STR].xsize, rect[RECT_PROF_STR].ysize, rect[RECT_PROF_STR].colorB[0]);
					BmpFillRect(rect[RECT_PROF_NUM].xpis, rect[RECT_PROF_NUM].ypis + rectdy, rect[RECT_PROF_NUM].xsize, rect[RECT_PROF_NUM].ysize);
					Draw24x24MulStr(rect[RECT_PROF_STR].xpis + SAFE_DIST4, rect[RECT_PROF_STR].ypis + SAFE_DIST4 + rectdy,
									(MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_MSG), linemaxdis, PROG_MSG2_Sheet,
									stAdd, 1, rect[RECT_PROF_STR].colorT[0]);

					disProgVal(rect[RECT_PROF_NUM].xpis + SAFE_DIST4, rect[RECT_PROF_NUM].ypis + SAFE_DIST4 + rectdy, 0, 0, LEFT, stAdd,
							   getProgVal(stAdd), getProgType(stAdd), rect[RECT_PROF_NUM].colorT[0]);
					// if ((*(MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_TYPE)) == 1)
					// {
					// 	if (PREG[stAdd * PREG_LENGTH + PREG_NOW] > 9999)
					// 		DrawMFU32ZR(rect[RECT_PROF_NUM].xpis + SAFE_DIST4, rect[RECT_PROF_NUM].ypis + SAFE_DIST4 + rectdy,
					// 					PREG[stAdd * PREG_LENGTH + PREG_NOW], 5, rect[RECT_PROF_NUM].colorT[0]);
					// 	else
					// 		DrawMFU32ZR(rect[RECT_PROF_NUM].xpis + SAFE_DIST4, rect[RECT_PROF_NUM].ypis + SAFE_DIST4 + rectdy,
					// 					PREG[stAdd * PREG_LENGTH + PREG_NOW], 4, rect[RECT_PROF_NUM].colorT[0]);
					// }
					// else
					// {
					// 	U32 itemAdd = (*(MEM_PROG_MSG2 + stAdd * MSG2_LENGTH + PMSG2_MSG3)); //获取选项内容所在地址
					// 	itemAdd += PREG[stAdd * PREG_LENGTH + PREG_NOW];
					// 	Draw24X24FFStr(rect[RECT_PROF_NUM].xpis + SAFE_DIST4, rect[RECT_PROF_NUM].ypis + SAFE_DIST4 + rectdy,
					// 				   (MEM_PROG_MSG3 + itemAdd * MSG3_LENGTH + PMSG3_PARANAME), rect[RECT_PROF_NUM].colorT[0]);
					// }
				}
				else
				{
					ParaList.dataAdd[i] = 0xff;
					BmpFillRect(keyd[keyIndex].xpis, keyd[keyIndex].ypis, keyd[keyIndex].xsize, keyd[keyIndex].ysize);
					BmpFillRect(rect[RECT_PROF_PARA].xpis, rect[RECT_PROF_PARA].ypis + rectdy, rect[RECT_PROF_PARA].xsize, rect[RECT_PROF_PARA].ysize);
				}

				stAdd++;
			}

			DrawButton(keyd[KEY_PROF_PREV], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROF_PREV].border[1] : keybmpind[KEY_PROF_PREV].border[0],
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROF_PREV].icon[1] : keybmpind[KEY_PROF_PREV].icon[0]);
			DrawButton(keyd[KEY_PROF_NEXT], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROF_NEXT].border[1] : keybmpind[KEY_PROF_NEXT].border[0],
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROF_NEXT].icon[1] : keybmpind[KEY_PROF_NEXT].icon[0]);

			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		ErrorMsg(MENU_PROFRAME);

		if (pno != MENU_PROFRAME)
		{
			plastno = MENU_PROFRAME;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//------------------------------------------
// Function   : void Menu_ProPara(void)
// Description: 编程设置菜单参数输入
//------------------------------------------
void Menu_ProPara(void)
{
	U8 f;
	U32 paranum;

	SetLanguage(MENU_PROG);
	paranum = PREG[g_parano * PREG_LENGTH + PREG_NOW];

	if ((*(MEM_PROG_MSG2 + (U32)(g_parano)*MSG2_LENGTH + PMSG2_TYPE)) == 1)
	{
		f = Menu_ProgramNumber(1, &paranum, PREG[g_parano * PREG_LENGTH + PREG_MIN], PREG[g_parano * PREG_LENGTH + PREG_MAX],
							   (MEM_PROG_MSG2 + (U32)(g_parano)*MSG2_LENGTH + PMSG2_UNIT));
	}
	else
	{
	Loop:
		f = Menu_ProgramLogic(&paranum, PREG[g_parano * PREG_LENGTH + PREG_MIN], PREG[g_parano * PREG_LENGTH + PREG_MAX]);
		if (f == 2)
		{
			goto Loop;
		}
	}

	if ((f) && (PREG[g_parano * PREG_LENGTH + PREG_NOW] != paranum))
	{
		PREG[g_parano * PREG_LENGTH + PREG_NOW] = paranum;
		Write_D((D_Address[PARA] + g_parano * PREG_LENGTH + PREG_NOW), 1, (U16 *)&PREG[g_parano * PREG_LENGTH + PREG_NOW]);
		g_prochange |= 0x01;

		if (g_parano == 151)
		{
			SetLcdPwm(1, P_CNR); //修改了液晶对比度值,要修改PWM的占空比
			DelayMs(10);
		}
		else if ((g_parano > 203) && (g_parano < 208)) //速度档位设置
		{
			if (Write_Cmd(C_UPDATEPARA, 0, 0) == 0)
			{
				pno = MENU_RWHELP;
				poldno[pnoindex++] = MENU_PROPARA;
				// pnoindex = 4;
				// poldno[3] = MENU_PROPARA;
				hleppara = 15;
				return;
			}
		}
	}

	if (P_LAN)
	{
		//USE_PROG_MSG1  = MEM_PROG_MSG11_E;
		//USE_PROG_MSG2  = MEM_PROG_MSG2_E;
		//USE_PROG_MSG3  = MEM_PROG_MSG3_E;
		//USE_PROG_MSG4  = MEM_PROG_MSG4_E;
		UADD_PROG_MSG1 = ADD_PROG_MSG11_E;
		UADD_PROG_MSG2 = ADD_PROG_MSG2_E;
		UADD_PROG_MSG3 = ADD_PROG_MSG3_E;
		UADD_PROG_MSG4 = ADD_PROG_MSG4_E;
	}
	else
	{
		//USE_PROG_MSG1  = MEM_PROG_MSG11;
		//USE_PROG_MSG2  = MEM_PROG_MSG2;
		//USE_PROG_MSG3  = MEM_PROG_MSG3;
		//USE_PROG_MSG4  = MEM_PROG_MSG4;
		UADD_PROG_MSG1 = ADD_PROG_MSG11;
		UADD_PROG_MSG2 = ADD_PROG_MSG2;
		UADD_PROG_MSG3 = ADD_PROG_MSG3;
		UADD_PROG_MSG4 = ADD_PROG_MSG4;
	}
	if (pno != MENU_PROPARA)
	{
		plastno = MENU_PROPARA;
		// break;
	}
}

#if 0
//---------------------------------------------------------------
//------------------------------------------
// Function   : void Menu_ProPass(void)
// Description: 编程设置菜单密码输入
//------------------------------------------
void Menu_ProPass(void)
{
	U8 f;
	U32 pass;

	SetLanguage(MENU_PROG);
	pass = 0;
	f = Menu_ProgramNumber(0, &pass, 0, 9999, (USE_PROG_MSG1 + 25 * MSG1_LENGTH));
	if (pno != MENU_SCRNSVR)
	{
		if (f == 0)
		{
			pno = MENU_PROG;
		}
		else if (pass == 1010)
		{
			pno = MENU_PROFRAME;
			pnoindex = 2;
			poldno[1] = MENU_PROG;
		}
		else
		{
			pno = MENU_RWHELP;
			pnoindex = 2;
			poldno[1] = MENU_PROPASS;
			hleppara = 22;
			EREG[1] = 192;
		}
	}
	plastno = MENU_PROPASS;
}
#endif

//-----------------------------------------------------------
#define KEY_PRON_SCRNSVR 1
#define KEY_PRON_CLOSE 2
#define KEY_PRON_ENTER 3
#define KEY_PRON_RESET 4
#define KEY_PRON_CLEAN 5
#define KEY_PRON_UP 6
#define KEY_PRON_DOWN 7
#define KEY_PRON_0 8
#define KEY_PRON_1 9
#define KEY_PRON_2 10
#define KEY_PRON_3 11
#define KEY_PRON_4 12
#define KEY_PRON_5 13
#define KEY_PRON_6 14
#define KEY_PRON_7 15
#define KEY_PRON_8 16
#define KEY_PRON_9 17
#define KEY_PRON_MAX 18

#define RECT_PRON_PARA_TIT 1
#define RECT_PRON_PARA_STR 2
#define RECT_PRON_PARA_NUM 3
#define RECT_PRON_PARA_RANGE 4
#define RECT_PRON_PARA_DETAIL 5

//------------------------------------------
// Function   : U32 Menu_ProgramNumber(U32 para, U32 *def, U32 vmin, U32 vmax, uint *unit)
// Description: 编程设置数字输入菜单 para:0/1 密码/参数 def:初始值　min:最小值 max:最大值 unit:单位
//------------------------------------------
U32 Menu_ProgramNumber(U32 para, U32 *def, U32 vmin, U32 vmax, U16 *unit)
{
	U8 dis_update = 1;
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U32 vol = *def;
	//U32 tmp_vol = 0;
	U32 re = 0;
	U32 loop = 0;
	U32 tmp_max;
	U32 dat;
	U16 PROG_MSG11_Sheet = 43;
	U16 PROG_MSG2_Sheet = 45;
	// U16 PROG_MSG3_Sheet = 46;
	U16 PROG_MSG4_Sheet = 47;
	U16 *paraIndex = MEM_PROG_MSG2 + (U32)(g_parano)*MSG2_LENGTH;
	U8 linemaxdis = 40;
	//U16 RectWinth1 = 464;
	//U16 addr1 = 32;
	const U8 keynum = {KEY_PRON_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},																			  //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},															  //2 返回
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},														  //3 确认
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},														  //4 还原
		{TYPE_X46, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 4, KEY_X2, KEY_Y2},							  //5 清除
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5), TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},	  //6 +1
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //7 -1
		{TYPE_X46, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},							  //8 0
		{TYPE_X46, TYPE_Y45, KEY_X2, KEY_Y2},														  //9 1
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5), TYPE_Y45, KEY_X2, KEY_Y2},								  //10 2
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y45, KEY_X2, KEY_Y2},							  //11 3
		{TYPE_X46, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5), KEY_X2, KEY_Y2},								  //12 4
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5), TYPE_Y45 + (KEY_Y2 + SAFE_DIST5), KEY_X2, KEY_Y2},		  //13 5
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5), KEY_X2, KEY_Y2},	  //14 6
		{TYPE_X46, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},							  //15 7
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5), TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},	  //16 8
		{TYPE_X46 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y45 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //17 9
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},		 //1 屏保
		{{KEY_Silver}, {0}, {72}, {8, 0, 0, 0}, {0}},		 //2 返回
		{{KEY_Silver}, {0}, {71}, {9, 0, 0, 0}, {0}},		 //3 确认
		{{KEY_Silver}, {0}, {236}, {16, 0, 0, 0}, {0}},		 //4  还原
		{{KEY_Silver}, {0}, {113, 114}, {28, 0, 0, 0}, {0}}, //5 清除
		{{KEY_Silver}, {0}, {199, 200}, {11, 0, 0, 0}, {0}}, //6 +1
		{{KEY_Silver}, {0}, {201, 201}, {12, 0, 0, 0}, {0}}, //7 -1
		{{KEY_Silver}, {0}, {91, 101}, {0, 0, 0, 0}, {0}},	 //8 0
		{{KEY_Silver}, {0}, {92, 102}, {0, 0, 0, 0}, {0}},	 //9 1
		{{KEY_Silver}, {0}, {93, 103}, {0, 0, 0, 0}, {0}},	 //10 2
		{{KEY_Silver}, {0}, {94, 104}, {0, 0, 0, 0}, {0}},	 //11 3
		{{KEY_Silver}, {0}, {95, 105}, {0, 0, 0, 0}, {0}},	 //12 4
		{{KEY_Silver}, {0}, {96, 106}, {0, 0, 0, 0}, {0}},	 //13 5
		{{KEY_Silver}, {0}, {97, 107}, {0, 0, 0, 0}, {0}},	 //14 6
		{{KEY_Silver}, {0}, {98, 108}, {0, 0, 0, 0}, {0}},	 //15 7
		{{KEY_Silver}, {0}, {99, 109}, {0, 0, 0, 0}, {0}},	 //16 8
		{{KEY_Silver}, {0}, {100, 110}, {0, 0, 0, 0}, {0}},	 //17 9

	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {2, 3}},

		{TYPE_X18, TYPE_Y2, KEY_X7, KEY_Y6, {Color_KeyBlue}, {Color_White}, {0}},									   //1 参数名的显示框
		{TYPE_X16, TYPE_Y2, 610, KEY_Y6, {Color_Black}, {Color_Black}, {0}},										   //2 参数的说明显示框
		{TYPE_X18, TYPE_Y45, TYPE_X16, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //3 当前参数值显示框
		{TYPE_X12, TYPE_Y45, 185, TYPE_SIZE5, {Color_Black}, {Color_Black}, {0}},									   //4 参数范围显示框
		{TYPE_X18, 158, 535, 245, {Color_Black}, {Color_Black}, {0}},												   //5 参数的详细说明显示框

	};

	InitScreen();
	initViewStatus();
	updateViewStatus();
	SetLanguage(MENU_PROG);

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[para ? 0 : 1] * TitleLengthMax,
				 SheetPage, rect[0].index[para ? 0 : 1], rect[0].colorT[0]);

	for (i = 2; i < keynum; i++) //画2-17键
	{
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	if (para)
	{
		U16 *detailStrIndex;
		//显示参数栏
		DrawFillRect(rect[RECT_PRON_PARA_TIT].xpis, rect[RECT_PRON_PARA_TIT].ypis, rect[RECT_PRON_PARA_TIT].xsize, rect[RECT_PRON_PARA_TIT].ysize, rect[RECT_PRON_PARA_TIT].colorB[0]);
		Draw24x24Str(rect[RECT_PRON_PARA_TIT].xpis + (rect[RECT_PRON_PARA_TIT].xsize - 12 * strlen((char *)(paraIndex + PMSG2_PARANAME))) / 2,
					 rect[RECT_PRON_PARA_TIT].ypis + (rect[RECT_PRON_PARA_TIT].ysize - 24) / 2,
					 (paraIndex + PMSG2_PARANAME), PROG_MSG2_Sheet, g_parano, rect[RECT_PRON_PARA_TIT].colorT[0]); //画出方框的文字

		//显示上方文本说明
		DrawRect(rect[RECT_PRON_PARA_STR].xpis, rect[RECT_PRON_PARA_STR].ypis, rect[RECT_PRON_PARA_STR].xsize, rect[RECT_PRON_PARA_STR].ysize, rect[RECT_PRON_PARA_STR].colorB[0]);
		Draw24x24MulStr(rect[RECT_PRON_PARA_STR].xpis + SAFE_DIST4, rect[RECT_PRON_PARA_STR].ypis + SAFE_DIST4,
						(paraIndex + PMSG2_MSG), linemaxdis, PROG_MSG2_Sheet, g_parano, 2, rect[RECT_PRON_PARA_STR].colorT[0]);

		//显示数值范围
		Draw24x24Str(rect[RECT_PRON_PARA_RANGE].xpis - 25 * 2 - SAFE_DIST4, rect[RECT_PRON_PARA_RANGE].ypis + SAFE_DIST4,
					 (MEM_PROG_MSG11 + 27 * MSG1_LENGTH), PROG_MSG11_Sheet, 27, rect[RECT_PRON_PARA_RANGE].colorT[0]); //其他扇区的"范围"字样
		DrawRect(rect[RECT_PRON_PARA_RANGE].xpis, rect[RECT_PRON_PARA_RANGE].ypis, rect[RECT_PRON_PARA_RANGE].xsize, rect[RECT_PRON_PARA_RANGE].ysize, rect[RECT_PRON_PARA_RANGE].colorB[0]);
		DrawMFU32Z(rect[RECT_PRON_PARA_RANGE].xpis + SAFE_DIST4, rect[RECT_PRON_PARA_RANGE].ypis + SAFE_DIST4,
				   vmin, 5, rect[RECT_PRON_PARA_RANGE].colorT[0]);
		Draw24X24FStr(rect[RECT_PRON_PARA_RANGE].xpis + SAFE_DIST4 + 6 * 13, rect[RECT_PRON_PARA_RANGE].ypis + SAFE_DIST4,
					  "-", rect[RECT_PRON_PARA_RANGE].colorT[0]);
		DrawMFU32Z(rect[RECT_PRON_PARA_RANGE].xpis + SAFE_DIST4 + 7 * 13, rect[RECT_PRON_PARA_RANGE].ypis + SAFE_DIST4,
				   vmax, 5, rect[RECT_PRON_PARA_RANGE].colorT[0]);

		//显示单位
		Draw24x24Str(rect[RECT_PRON_PARA_NUM].xpis + rect[RECT_PRON_PARA_NUM].xsize + SAFE_DIST4, rect[RECT_PRON_PARA_NUM].ypis + SAFE_DIST4,
					 (unit), PROG_MSG2_Sheet, g_parano, rect[RECT_PRON_PARA_NUM].colorT[0]); //其他扇区的"范围"字样

		//多行显示下方文本框内容
		DrawRect(rect[RECT_PRON_PARA_DETAIL].xpis, rect[RECT_PRON_PARA_DETAIL].ypis, rect[RECT_PRON_PARA_DETAIL].xsize, rect[RECT_PRON_PARA_DETAIL].ysize, rect[RECT_PRON_PARA_DETAIL].colorB[0]);
		detailStrIndex = MEM_PROG_MSG4 + (U32)(*(paraIndex + PMSG2_HELP)) * ((P_LAN) ? MSG4_LENGTH_E : MSG4_LENGTH);
		Draw24x24MulStr(rect[RECT_PRON_PARA_DETAIL].xpis + SAFE_DIST4, rect[RECT_PRON_PARA_DETAIL].ypis + SAFE_DIST4,
						(detailStrIndex), linemaxdis, PROG_MSG4_Sheet, *(paraIndex + PMSG2_HELP), 8, rect[RECT_PRON_PARA_DETAIL].colorT[0]);
	}
	else
	{
		Draw24x24Str(128, 144, (MEM_PROG_MSG11 + 25 * MSG1_LENGTH), PROG_MSG11_Sheet, 25, Color_Black);
	}

	dat = vmax / 10;
	tmp_max = 1;
	while (dat)
	{
		tmp_max *= 10; //tmp_max是10的vmax的位数次方
		dat /= 10;
	}

	ViewPage(usepageno);

	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值

		// /[按键键值返回预处理*begin]**********************************************************

		//特定情况下不生效按键预处理
		//无效按键

		if (keydata == 0xff) //按键松开后的键值预处理
		{
			if ((keyon == KEY_PRON_UP) || (keyon == KEY_PRON_DOWN))
			{
			}
			else if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理    与提示性信息处理
		{
			//特定情况下不生效按键

			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_PRON_SCRNSVR) //无按键特效
				{
				}
				//非常规按键-//绘制颜色文字按键
				else
				{
					DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
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
				if (beeponf == 0) //蜂鸣器标志位 //第一次进入
				{
					beeponf = 1;

					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);

					switch (keyon) //按键按下标题提示语显示
					{

					case KEY_PRON_0:
					case KEY_PRON_1:
					case KEY_PRON_2:
					case KEY_PRON_3:
					case KEY_PRON_4:
					case KEY_PRON_5:
					case KEY_PRON_6:
					case KEY_PRON_7:
					case KEY_PRON_8:
					case KEY_PRON_9:
						BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
						DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_PRON_0, 1, rect[0].colorT[0]);
						break;
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					//带有长按连续功能按键的功能

					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}

				if ((keyon == KEY_PRON_UP) || (keyon == KEY_PRON_DOWN)) //带有长按连续功能按键的功能
				{
					keyoff = keyon;
					key_on_flag1 = 1;
				}
				Times = 0; //当前
			}
		}
		else //按键没有按下
		{
			key_on_flag1 = 0;
		}

		// /[按键键值返回预处理*end]**********************************************************

		// /[按键松开键值处理*begin]**********************************************************
		if (keyoff)
		{
			if ((keyoff >= KEY_PRON_0) && (keyoff <= KEY_PRON_9))
			{
				if (tmp_max > 1) //初始值取余tmp*10+当前键值-0的键值
					vol = (vol % tmp_max) * 10 + keyoff - KEY_PRON_0;
				else //tmp_max=1
					vol = keyoff - KEY_PRON_0;
				if (vol > vmax)
				{
					if (tmp_max > 1)
						vol = vol % tmp_max;
					else if (vmax)
						vol = vol % vmax;
					else
						vol = 0;
				}
				dis_update = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_PRON_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_PRON_CLOSE:
					loop = 1;
					re = 0;
					pno = poldno[--pnoindex];
					break;
				case KEY_PRON_ENTER:
					if ((vol >= vmin) && (vol <= vmax)) //正确写入
					{
						loop = 1;
						re = 1;
						*def = vol;
						pno = poldno[--pnoindex];
					}
					else //错误提示
					{
						g_Toast.vIndex = TOAST_MSG12;
						ToastShow1(TYPE_X12 - TYPE_SIZE18 / 2, TYPE_Y12, TYPE_SIZE18, TOAST_SHOW_5S);
					}
					break;
				case KEY_PRON_CLEAN: //清零
					vol = 0;
					dis_update = 1;
					break;
				case KEY_PRON_RESET: //清零
					if (PREG[g_parano * PREG_LENGTH + PREG_DEFAULT] >= (*(paraIndex + PMSG2_MIN)) &&
						PREG[g_parano * PREG_LENGTH + PREG_DEFAULT] <= (*(paraIndex + PMSG2_MAX)))
					{
						vol = PREG[g_parano * PREG_LENGTH + PREG_DEFAULT];
					}
					else
					{
						vol = *def;
					}

					dis_update = 1;
					break;
				case KEY_PRON_UP: //加
					if (vol < vmax)
					{
						vol++;
						dis_update = 1;
					}
					break;
				case KEY_PRON_DOWN: //减
					if (vol > vmin)
					{
						vol--;
						dis_update = 1;
					}
					break;
				}
			}
			keyoff = 0;
		}
		// /[按键松开键值处理*end]**********************************************************

		// /[按键弹起显示信息处理*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{

			if (keyon == KEY_PRON_SCRNSVR) //不需要显示的按键
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
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[para ? 0 : 1] * TitleLengthMax,
						 SheetPage, rect[0].index[para ? 0 : 1], rect[0].colorT[0]);
		}
		// /[按键弹起显示信息处理*end]**********************************************************

		if (dis_update)
		{
			dis_update = 0;
			DrawEdit(rect[RECT_PRON_PARA_NUM], rect[RECT_PRON_PARA_NUM].colorT[1], rect[RECT_PRON_PARA_NUM].colorB[1]);
			DrawMFU32Z(rect[RECT_PRON_PARA_NUM].xpis + (rect[RECT_PRON_PARA_NUM].xsize - 4 * 13) / 2,
					   rect[RECT_PRON_PARA_NUM].ypis + SAFE_DIST4, vol, 4, rect[RECT_PRON_PARA_NUM].colorT[0]);
		}
		ErrorMsg(MENU_PROPARA); //MENU_PROFRAME	poldno[2]
		if (loop == 1)
			break;
		updateViewStatus();
		wdt();
	}
	return (re);
}

//----------------------------------------------------
#define KEY_PROL_SCRNSVR 1
#define KEY_PROL_CLOSE 2
#define KEY_PROL_PREV 3
#define KEY_PROL_NEXT 4
#define KEY_PROL_ENTER 5
#define KEY_PROL_HELP 6
#define KEY_PROL_RESET 7
#define KEY_PROL_SELC1 8
#define KEY_PROL_SELC2 9
#define KEY_PROL_SELC3 10
#define KEY_PROL_MAX 11

#define RECT_PROL_PAGE 1
#define RECT_PROL_PARA_TIT 2
#define RECT_PROL_PARA_STR 3
#define RECT_PROL_PARA_ITEM1 4
#define RECT_PROL_PARA_ITEM2 5
#define RECT_PROL_PARA_ITEM3 6

#define SELC_MAX 3

//------------------------------------------
// Function   : U32 Menu_ProgramLogic(U32 *def)
// Description:  编程设置逻辑输入菜单 def:当前数据
//------------------------------------------
U32 Menu_ProgramLogic(U32 *def, U32 vmin, U32 vmax)
{
	// U8 page_flash = 1, page_flash1 = 1, page_flash2 = 1;
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U32 re = 0;
	U32 loop = 0;
	U32 tmpvol = *def, vol = *def;
	// U32 tmptmp;
	// U16 log_mpos, log_page, log_mpage, log_tpos, log_num;
	U16 PROG_MSG2_Sheet = 45;
	U16 PROG_MSG3_Sheet = 46;
	U8 dis_update = 1; //刷新按钮
	//U16 tmpP_LAN = 0;
	U8 linemaxdis = 40;
	tydPatternList ParaList = {1, 0, 0, 3, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数
	U16 *paraIndex = MEM_PROG_MSG2 + (U32)(g_parano)*MSG2_LENGTH;
	const U8 keynum = {KEY_PROL_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},										//1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},						//2 返回
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},						//3 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},						//4 下一页
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},					//5 确认
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},					//6 帮助
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},						//7 还原
		{TYPE_X18, TYPE_Y46, 88, 56},							//8 选项1
		{TYPE_X18, TYPE_Y46 + TYPE_SIZE14, KEY_X7, KEY_Y6},		//9 选项2
		{TYPE_X18, TYPE_Y46 + 2 * TYPE_SIZE14, KEY_X7, KEY_Y6}, //10 选项3
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},													 //1 屏保
		{{KEY_Silver}, {0}, {72}, {0, 0, 0, 0}, {0}},													 //2 返回
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {0, 0, 0, 0}, {0}},									 //3 上一页
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {0, 0, 0, 0}, {0}},									 //4 下一页
		{{KEY_Silver}, {0}, {71}, {0, 0, 0, 0}, {0}},													 //5 确认
		{{KEY_Silver}, {0}, {239}, {0, 0, 0, 0}, {0}},													 //6 帮助
		{{KEY_Silver}, {0}, {236}, {0, 0, 0, 0}, {0}},													 //7 还原
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //8 1
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //9 2
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //10 3
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {2}},

		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示
		{TYPE_X18, TYPE_Y2, KEY_X7, KEY_Y6, {Color_KeyBlue}, {Color_White}, {0}},							   //1 参数名的显示框
		{TYPE_X16, TYPE_Y2, 610, KEY_Y6, {Color_Black}, {Color_Black}, {0}},								   //2 参数的说明显示框
		{TYPE_X16, 106, 610, 90, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},		   //5 参数选项的显示框
		{TYPE_X16, 206, 610, 90, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},		   //5 参数选项的显示框
		{TYPE_X16, 306, 610, 90, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},		   //5 参数选项的显示框
	};
	InitScreen();
	initViewStatus();
	updateViewStatus();
	SetLanguage(MENU_PROG);

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	DrawFillRect(rect[RECT_PROL_PARA_TIT].xpis, rect[RECT_PROL_PARA_TIT].ypis, rect[RECT_PROL_PARA_TIT].xsize, rect[RECT_PROL_PARA_TIT].ysize, rect[RECT_PROL_PARA_TIT].colorB[0]);
	Draw24x24Str(rect[RECT_PROL_PARA_TIT].xpis + (rect[RECT_PROL_PARA_TIT].xsize - 12 * strlen((char *)(paraIndex + PMSG2_PARANAME))) / 2,
				 rect[RECT_PROL_PARA_TIT].ypis + (rect[RECT_PROL_PARA_TIT].ysize - 24) / 2,
				 (paraIndex + PMSG2_PARANAME), PROG_MSG2_Sheet, g_parano, rect[RECT_PROL_PARA_TIT].colorT[0]); //画出方框的文字
	//显示上方文本说明
	DrawRect(rect[RECT_PROL_PARA_STR].xpis, rect[RECT_PROL_PARA_STR].ypis, rect[RECT_PROL_PARA_STR].xsize, rect[RECT_PROL_PARA_STR].ysize, rect[RECT_PROL_PARA_STR].colorB[0]);
	Draw24x24MulStr(rect[RECT_PROL_PARA_STR].xpis + SAFE_DIST4, rect[RECT_PROL_PARA_STR].ypis + SAFE_DIST4,
					(paraIndex + PMSG2_MSG), linemaxdis, PROG_MSG2_Sheet, g_parano, 2, rect[RECT_PROL_PARA_STR].colorT[0]);

	for (i = KEY_PROL_CLOSE; i <= KEY_PROL_RESET; i++)
	{
		if ((i == KEY_PROL_PREV) || (i == KEY_PROL_NEXT))
		{
		}
		else
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
	}

	ViewPage(usepageno);
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值

		// /[按键键值返回预处理*begin]**********************************************************
		if (keydata == KEY_PROL_PREV)
		{
			if (ParaList.pageNow == 1)
			{
				keydata = 0;
			}
		}
		if (keydata == KEY_PROL_NEXT)
		{
			if (ParaList.pageNow == ParaList.pageNum)
			{
				keydata = 0;
			}
		}
		if (keydata >= KEY_PROL_SELC1 && keydata <= KEY_PROL_SELC3)
		{
			i = keydata - KEY_PROL_SELC1;
			if (ParaList.selectEnable[i] == 0)
			{
				keydata = 0;
			}
		}
		//特定情况下不生效按键预处理
		//无效按键

		if (keydata == 0xff) //按键松开后的键值预处理
		{
			if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理    与提示性信息处理
		{
			//特定情况下不生效按键

			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_PROL_SCRNSVR) //无按键特效
				{
				}
				//非常规按键-//绘制颜色文字按键
				else
				{
					DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
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
				if (beeponf == 0) //蜂鸣器标志位 //第一次进入
				{
					beeponf = 1;

					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);

					switch (keyon) //按键按下标题提示语显示
					{
					case KEY_PROL_SELC1:
					case KEY_PROL_SELC2:
					case KEY_PROL_SELC3:
						break;

					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					//带有长按连续功能按键的功能

					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
			}
		}
		else //按键没有按下
		{
			key_on_flag1 = 0;
		}

		// /[按键键值返回预处理*end]**********************************************************

		// /[按键松开键值处理*begin]**********************************************************
		if (keyoff)
		{
			if (keyoff == KEY_PROL_SELC1 || keyoff == KEY_PROL_SELC2 || keyoff == KEY_PROL_SELC3)
			{
				ParaList.noInPageNow = keyoff - KEY_PROL_SELC1;
				ParaList.bFlashSelectFlag = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_PROL_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_PROL_CLOSE:
					loop = 1;
					re = 0;
					pno = poldno[--pnoindex];
					break;
				case KEY_PROL_PREV:
					if (ParaList.pageNow > 1)
					{
						ParaList.pageNow--;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				case KEY_PROL_NEXT:
					if (ParaList.pageNow < ParaList.pageNum)
					{
						ParaList.pageNow++;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				case KEY_PROL_ENTER:
					if (tmpvol != vol) //没有修改按确定无效
					{
						loop = 1;
						re = 1;
						*def = tmpvol;
						pno = poldno[--pnoindex];
					}
					else
					{
						loop = 1;
						re = 0;
					}
					break;
				case KEY_PROL_HELP:
					Menu_ProgramLogicHelp(tmpvol); //跳转帮助菜单
					loop = 1;
					re = 2;
					break;
				case KEY_PROL_RESET:
					if (PREG[g_parano * PREG_LENGTH + PREG_DEFAULT] >= (*(paraIndex + PMSG2_MIN)) &&
						PREG[g_parano * PREG_LENGTH + PREG_DEFAULT] <= (*(paraIndex + PMSG2_MAX)))
					{
						tmpvol = PREG[g_parano * PREG_LENGTH + PREG_DEFAULT];
					}
					else
					{
						tmpvol = *def;
					}
					ParaList.bflashAllFlag = 1;
					break;
				}
			}
			keyoff = 0;
		}
		// /[按键松开键值处理*end]**********************************************************

		// /[按键弹起显示信息处理*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{

			if (keyon == KEY_PROL_SCRNSVR) //不需要显示的按键
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
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[按键弹起显示信息处理*end]**********************************************************

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;

			ParaList.patternNum = (*(paraIndex + PMSG2_MAX)) - (*(paraIndex + PMSG2_MIN)) + 1;
			ParaList.selectNoNow = vol - vmin;

			ParaList.noInPageNow = (ParaList.selectNoNow) % ParaList.disNumDiv;
			ParaList.noInPageLast = ParaList.noInPageNow;
			ParaList.pageNow = (ParaList.selectNoNow) / ParaList.disNumDiv + 1;
			ParaList.pageLast = ParaList.pageNow;
			ParaList.pageNum = (ParaList.patternNum - 1) / ParaList.disNumDiv + 1;

			if (ParaList.pageNum == 0)
				ParaList.pageNum = 1;

			if (ParaList.pageNow == 0)
				ParaList.pageNow = 1;
			else if (ParaList.pageNow > ParaList.pageNum)
				ParaList.pageNow = ParaList.pageNum;
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;

			dis_update = 1;

			if (ParaList.dataAdd[ParaList.noInPageNow] >= 0xFFFF) //选中空
			{
				ParaList.noInPageNow = ParaList.noInPageLast;
			}
			else
			{
				ParaList.noInPageLast = ParaList.noInPageNow;
			}

			ParaList.selectNoNow = ParaList.noInPageNow + (ParaList.pageNow - 1) * ParaList.disNumDiv;
			ParaList.selectNoLast = ParaList.selectNoNow;
			ParaList.pageLast = ParaList.pageNow;
			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				ParaList.selectList[i] = 0;
			}
			ParaList.selectList[(ParaList.selectNoNow) % ParaList.disNumDiv] = 1;

			tmpvol = ParaList.selectNoNow + vmin;
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U8 len = (ParaList.pageNow < ParaList.pageNum) ? ParaList.disNumDiv : (ParaList.patternNum - (ParaList.pageNow - 1) * ParaList.disNumDiv);
			U16 stAdd = (*(paraIndex + PMSG2_MSG3));
			ParaList.bFlashPageFlag = 0;
			dis_update = 1;

			//显示页码
			DrawFillRect(rect[RECT_PROL_PAGE].xpis, rect[RECT_PROL_PAGE].ypis,
						 rect[RECT_PROL_PAGE].xsize, rect[RECT_PROL_PAGE].ysize, rect[RECT_PROL_PAGE].colorB[0]);
			DrawFU32(rect[RECT_PROL_PAGE].xpis + SAFE_DIST2, rect[RECT_PROL_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNow, 3, rect[RECT_PROL_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_PROL_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_PROL_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_PROL_PAGE].colorT[0]);
			DrawFU32(rect[RECT_PROL_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_PROL_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNum, 3, rect[RECT_PROL_PAGE].colorT[0]);

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				ParaList.selectList[i] = 0;
				if (i < len)
				{
					ParaList.selectEnable[i] = 1;
					ParaList.dataAdd[i] = stAdd + (ParaList.pageNow - 1) * ParaList.disNumDiv;
					stAdd++;
				}
				else
				{
					ParaList.selectEnable[i] = 0;
					ParaList.dataAdd[i] = 0xFFFFFFFF;
				}
			}
			if (ParaList.pageLast == ParaList.pageNow)
			{
				ParaList.selectList[(ParaList.selectNoNow) % ParaList.disNumDiv] = 1;
			}

			DrawButton(keyd[KEY_PROL_PREV], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROL_PREV].border[1] : keybmpind[KEY_PROL_PREV].border[0],
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROL_PREV].icon[1] : keybmpind[KEY_PROL_PREV].icon[0]);
			DrawButton(keyd[KEY_PROL_NEXT], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROL_NEXT].border[1] : keybmpind[KEY_PROL_NEXT].border[0],
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROL_NEXT].icon[1] : keybmpind[KEY_PROL_NEXT].icon[0]);

			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		if (dis_update == 1)
		{
			dis_update = 0;
			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				U8 keyIndex = KEY_PROL_SELC1 + i;
				U8 selectIndex = RECT_PROL_PARA_ITEM1 + i;
				if (ParaList.selectEnable[i] == 1)
				{
					DrawButtonSingleColor(keyd[keyIndex],
										  (ParaList.selectList[i]) ? keybmpind[keyIndex].border[1] : keybmpind[keyIndex].border[0],
										  (ParaList.selectList[i]) ? keybmpind[keyIndex].icon[1] : keybmpind[keyIndex].icon[0]);
					Draw24x24Str(keyd[keyIndex].xpis + (keyd[keyIndex].xsize - strlen((char *)(MEM_PROG_MSG3 + ParaList.dataAdd[i] * MSG3_LENGTH + PMSG3_PARANAME)) * 12) / 2,
								 keyd[keyIndex].ypis + (keyd[keyIndex].ysize - 24) / 2,
								 (MEM_PROG_MSG3 + ParaList.dataAdd[i] * MSG3_LENGTH + PMSG3_PARANAME), PROG_MSG3_Sheet, 1,
								 (ParaList.selectList[i]) ? keybmpind[keyIndex].textColor[1] : keybmpind[keyIndex].textColor[0]);

					DrawFillRect(rect[selectIndex].xpis, rect[selectIndex].ypis,
								 rect[selectIndex].xsize, rect[selectIndex].ysize, (ParaList.selectList[i]) ? rect[selectIndex].colorB[1] : rect[selectIndex].colorB[0]);
					Draw24x24MulStr(rect[selectIndex].xpis + SAFE_DIST4, rect[selectIndex].ypis + SAFE_DIST4,
									(MEM_PROG_MSG3 + ParaList.dataAdd[i] * MSG3_LENGTH + PMSG3_MSG), linemaxdis, PROG_MSG3_Sheet,
									ParaList.dataAdd[i], 3,
									(ParaList.selectList[i]) ? rect[selectIndex].colorT[1] : rect[selectIndex].colorT[0]);
				}
				else
				{
					BmpFillRect(keyd[keyIndex].xpis, keyd[keyIndex].ypis, keyd[keyIndex].xsize, keyd[keyIndex].ysize);
					BmpFillRect(rect[selectIndex].xpis, rect[selectIndex].ypis, rect[selectIndex].xsize, rect[selectIndex].ysize);
				}
			}
		}

		ErrorMsg(MENU_PROPARA); //MENU_PROFRAME	poldno[2]
		if (loop == 1)
			break;
		wdt();
		updateViewStatus();
	}
	return (re);
}

//------------------------------------------------------------
#define KEY_PROLH_SCRNSVR 1
#define KEY_PROLH_CLOSE 2
#define KEY_PROLH_PREV 3
#define KEY_PROLH_NEXT 4
#define KEY_PROLH_MAX 5

#define RECT_PROLH_PAGE 1
#define RECT_PROLH_PARA_TIT 2
#define RECT_PROLH_PARA_STR 3
#define RECT_PROLH_ITEM_TIT 4
#define RECT_PROLH_ITEM_STR 5

//------------------------------------------
// Function   : void Menu_ProgramLogicHelp(U32 def)
// Description:  编程设置逻辑输入帮助菜单
//------------------------------------------
void Menu_ProgramLogicHelp(U32 def)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 vol = def;
	U32 loop = 0;
	U16 PROG_MSG2_Sheet = 45;
	U16 PROG_MSG3_Sheet = 46;
	U16 PROG_MSG4_Sheet = 47;
	U8 linemaxdis = 40;
	U16 *paraIndex = MEM_PROG_MSG2 + (U32)(g_parano)*MSG2_LENGTH;
	tydPatternList ParaList = {1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数
	const U8 keynum = {KEY_PROLH_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},					//1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1}, //2 返回
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1}, //3 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1}, //4 下一页
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},				  //1 屏保
		{{KEY_Silver}, {0}, {72}, {0, 0, 0, 0}, {0}},				  //2 返回
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {0, 0, 0, 0}, {0}}, //3 上一页
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {0, 0, 0, 0}, {0}}, //4 下一页
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {4}},

		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示
		{TYPE_X18, TYPE_Y2, KEY_X7, KEY_Y6, {Color_KeyBlue}, {Color_White}, {0}},							   //2 参数名的显示框
		{TYPE_X16, TYPE_Y2, 610, KEY_Y6, {Color_Black}, {Color_Black}, {0}},								   //3 参数的说明显示框
		{TYPE_X18, 116, KEY_X7, KEY_Y6, {Color_White, Color_KeyBlue}, {Color_Black, Color_White}, {0}},		   //4 参数名的显示框
		{TYPE_X16, 116, 610, 280, {Color_Black, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},		   //5 参数选项的显示框
	};
	InitScreen();
	initViewStatus();
	updateViewStatus();
	SetLanguage(MENU_PROG);

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	DrawFillRect(rect[RECT_PROLH_PARA_TIT].xpis, rect[RECT_PROLH_PARA_TIT].ypis, rect[RECT_PROLH_PARA_TIT].xsize, rect[RECT_PROLH_PARA_TIT].ysize, rect[RECT_PROLH_PARA_TIT].colorB[0]);
	Draw24x24Str(rect[RECT_PROLH_PARA_TIT].xpis + (rect[RECT_PROLH_PARA_TIT].xsize - 12 * strlen((char *)(paraIndex + PMSG2_PARANAME))) / 2,
				 rect[RECT_PROLH_PARA_TIT].ypis + (rect[RECT_PROLH_PARA_TIT].ysize - 24) / 2,
				 (paraIndex + PMSG2_PARANAME), PROG_MSG2_Sheet, g_parano, rect[RECT_PROLH_PARA_TIT].colorT[0]); //画出方框的文字
	//显示上方文本说明
	DrawRect(rect[RECT_PROLH_PARA_STR].xpis, rect[RECT_PROLH_PARA_STR].ypis, rect[RECT_PROLH_PARA_STR].xsize, rect[RECT_PROLH_PARA_STR].ysize, rect[RECT_PROLH_PARA_STR].colorB[0]);
	Draw24x24MulStr(rect[RECT_PROLH_PARA_STR].xpis + SAFE_DIST4, rect[RECT_PROLH_PARA_STR].ypis + SAFE_DIST4,
					(paraIndex + PMSG2_MSG), linemaxdis, PROG_MSG2_Sheet, g_parano, 2, rect[RECT_PROLH_PARA_STR].colorT[0]);

	DrawButton(keyd[KEY_PROLH_CLOSE], (U16 *)(ADD_ICONBMP), keybmpind[KEY_PROLH_CLOSE].border[0], keybmpind[KEY_PROLH_CLOSE].icon[0]);

	ViewPage(usepageno);

	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值
		if (keydata == KEY_PROLH_NEXT && ParaList.pageNow == ParaList.pageNum)
		{
			keydata = 0;
		}
		if (keydata == KEY_PROLH_PREV && ParaList.pageNow == 1)
		{
			keydata = 0;
		}

		// /[按键键值返回预处理*begin]**********************************************************
		//特定情况下不生效按键预处理
		//无效按键

		if (keydata == 0xff) //按键松开后的键值预处理
		{
			if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理    与提示性信息处理
		{
			//特定情况下不生效按键

			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_PROLH_SCRNSVR) //无按键特效
				{
				}
				//非常规按键-//绘制颜色文字按键
				else
				{
					DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
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
				if (beeponf == 0) //蜂鸣器标志位 //第一次进入
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
					//带有长按连续功能按键的功能

					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
			}
		}
		else //按键没有按下
		{
			key_on_flag1 = 0;
		}

		// /[按键键值返回预处理*end]**********************************************************

		// /[按键松开键值处理*begin]**********************************************************
		if (keyoff)
		{

			switch (keyoff)
			{
			case KEY_PROLH_SCRNSVR:
				pno = MENU_SCRNSVR;
				//loop = 1;
				break;
			case KEY_PROLH_CLOSE:
				loop = 1;
				break;
			case KEY_PROLH_PREV:
				if (ParaList.pageNow > 1)
				{
					ParaList.pageNow--;
					ParaList.bFlashPageFlag = 1;
				}
				break;
			case KEY_PROLH_NEXT:
				if (ParaList.pageNow < ParaList.pageNum)
				{
					ParaList.pageNow++;
					ParaList.bFlashPageFlag = 1;
				}
				break;
			}

			keyoff = 0;
		}
		// /[按键松开键值处理*end]**********************************************************

		// /[按键弹起显示信息处理*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{

			if (keyon == KEY_PROLH_SCRNSVR) //不需要显示的按键
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
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[按键弹起显示信息处理*end]**********************************************************

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;

			ParaList.patternNum = (*(paraIndex + PMSG2_MAX)) - (*(paraIndex + PMSG2_MIN)) + 1 + 1;
			ParaList.selectNoNow = vol - (*(paraIndex + PMSG2_MIN)) + 1;

			ParaList.pageNow = ParaList.selectNoNow + 1;
			ParaList.pageLast = ParaList.pageNow;
			ParaList.pageNum = ParaList.patternNum;

			if (ParaList.pageNum == 0)
				ParaList.pageNum = 1;

			if (ParaList.pageNow == 0)
				ParaList.pageNow = 1;
			else if (ParaList.pageNow > ParaList.pageNum)
				ParaList.pageNow = ParaList.pageNum;
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U16 titAdd = (*(paraIndex + PMSG2_MSG3));
			U16 helpAdd = (*(paraIndex + PMSG2_HELP));
			ParaList.bFlashPageFlag = 0;

			//显示页码
			DrawFillRect(rect[RECT_PROLH_PAGE].xpis, rect[RECT_PROLH_PAGE].ypis,
						 rect[RECT_PROLH_PAGE].xsize, rect[RECT_PROLH_PAGE].ysize, rect[RECT_PROLH_PAGE].colorB[0]);
			DrawFU32(rect[RECT_PROLH_PAGE].xpis + SAFE_DIST2, rect[RECT_PROLH_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNow, 3, rect[RECT_PROLH_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_PROLH_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_PROLH_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_PROLH_PAGE].colorT[0]);
			DrawFU32(rect[RECT_PROLH_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_PROLH_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNum, 3, rect[RECT_PROLH_PAGE].colorT[0]);

			BmpFillRect(rect[RECT_PROLH_ITEM_TIT].xpis, rect[RECT_PROLH_ITEM_TIT].ypis, rect[RECT_PROLH_ITEM_TIT].xsize, rect[RECT_PROLH_ITEM_TIT].ysize);
			BmpFillRect(rect[RECT_PROLH_ITEM_STR].xpis, rect[RECT_PROLH_ITEM_STR].ypis, rect[RECT_PROLH_ITEM_STR].xsize, rect[RECT_PROLH_ITEM_STR].ysize);

			ParaList.dataAdd[0] = helpAdd + (ParaList.pageNow - 1);
			if (ParaList.pageNow != 1)
			{
				ParaList.dataAdd[1] = titAdd + (ParaList.pageNow - 1 - 1);
			}

			if (ParaList.dataAdd[0] != 0xFFFF)
			{
				if (ParaList.pageNow != 1)
				{
					DrawFillRect(rect[RECT_PROLH_ITEM_TIT].xpis, rect[RECT_PROLH_ITEM_TIT].ypis,
								 rect[RECT_PROLH_ITEM_TIT].xsize, rect[RECT_PROLH_ITEM_TIT].ysize,
								 (ParaList.pageNow == ParaList.pageNum) ? rect[RECT_PROLH_ITEM_TIT].colorB[1] : rect[RECT_PROLH_ITEM_TIT].colorB[0]);
					Draw24x24Str(rect[RECT_PROLH_ITEM_TIT].xpis + (rect[RECT_PROLH_ITEM_TIT].xsize - strlen((char *)(MEM_PROG_MSG3 + ParaList.dataAdd[1] * MSG3_LENGTH)) * 12) / 2,
								 rect[RECT_PROLH_ITEM_TIT].ypis + (rect[RECT_PROLH_ITEM_TIT].ysize - 24) / 2,
								 (MEM_PROG_MSG3 + ParaList.dataAdd[1] * MSG3_LENGTH), PROG_MSG3_Sheet, 1,
								 (ParaList.pageNow == ParaList.pageNum) ? rect[RECT_PROLH_ITEM_TIT].colorT[1] : rect[RECT_PROLH_ITEM_TIT].colorT[0]);
				}
				DrawRect(rect[RECT_PROLH_ITEM_STR].xpis, rect[RECT_PROLH_ITEM_STR].ypis,
						 rect[RECT_PROLH_ITEM_STR].xsize, rect[RECT_PROLH_ITEM_STR].ysize, rect[RECT_PROLH_ITEM_STR].colorB[0]);
				Draw24x24MulStr(rect[RECT_PROLH_ITEM_STR].xpis + SAFE_DIST4, rect[RECT_PROLH_ITEM_STR].ypis + SAFE_DIST4,
								(MEM_PROG_MSG4 + ParaList.dataAdd[0] * MSG4_LENGTH), linemaxdis, PROG_MSG4_Sheet,
								ParaList.dataAdd[0], 8, rect[RECT_PROLH_ITEM_STR].colorT[0]);
			}

			DrawButton(keyd[KEY_PROLH_PREV], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROLH_PREV].border[1] : keybmpind[KEY_PROLH_PREV].border[0],
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PROLH_PREV].icon[1] : keybmpind[KEY_PROLH_PREV].icon[0]);
			DrawButton(keyd[KEY_PROLH_NEXT], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROLH_NEXT].border[1] : keybmpind[KEY_PROLH_NEXT].border[0],
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PROLH_NEXT].icon[1] : keybmpind[KEY_PROLH_NEXT].icon[0]);

			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		if (loop == 1)
			break;

		updateViewStatus();
		wdt();
	}
}

//----------------------------------------------------------
#define KEY_PRODT_SCRNSVR 1
#define KEY_PRODT_BACK 2
#define KEY_PRODT_PREV 3
#define KEY_PRODT_NEXT 4
#define KEY_PRODT_RESETA 5
#define KEY_PRODT_RESET 6
#define KEY_PRODT_DEFT 7 //修改过的
#define KEY_PRODT_MODI 8 //当前修改
#define KEY_PRODT_SELC1 9
#define KEY_PRODT_SELC2 10
#define KEY_PRODT_SELC3 11
#define KEY_PRODT_SELC4 12
#define KEY_PRODT_SELC5 13
#define KEY_PRODT_SELM1 14
#define KEY_PRODT_SELM2 15
#define KEY_PRODT_SELM3 16
#define KEY_PRODT_SELM4 17
#define KEY_PRODT_SELM5 18
#define KEY_PRODT_MAX 19

#define RECT_PRODT_PAGE 1
#define RECT_PRODT_NOW_TIT 2
#define RECT_PRODT_DEF_TIT 3
#define RECT_PRODT_NOW1 4
#define RECT_PRODT_NOW2 5
#define RECT_PRODT_NOW3 6
#define RECT_PRODT_NOW4 7
#define RECT_PRODT_NOW5 8
#define RECT_PRODT_DEF1 9
#define RECT_PRODT_DEF2 10
#define RECT_PRODT_DEF3 11
#define RECT_PRODT_DEF4 12
#define RECT_PRODT_DEF5 13
//------------------------------------------
// Function   : void Menu_ProDifferent(void)
// Description:  查看修改过的参数
//------------------------------------------
void Menu_ProDifferent(void)
{
	U8 dis_update = 0; //显示item
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U32 tmp_code = 0;
	u16 selectParNoList[256] = {0};													  //记录了选中的按钮序号
	tydPatternList ParaList = {1, 0, 0, 5, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数
	U8 linemaxdis = 28;
	//U16 RectWidth = 372;
	U16 PROG_MSG2_Sheet = 45;
	const U8 keynum = {KEY_PRODT_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},											 //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},							 //2 返回
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},							 //3 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},							 //4 下一页
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},						 //5 还原all
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},							 //6 还原
		{0, 0, 0, 0},												 //7
		{0, 0, 0, 0},												 //8
		{TYPE_X18, 75, KEY_X7, KEY_Y6},								 //9 选项1
		{TYPE_X18, 75 + 1 * (KEY_Y6 + SAFE_DIST6), KEY_X7, KEY_Y6},	 //10 选项2
		{TYPE_X18, 75 + 2 * (KEY_Y6 + SAFE_DIST6), KEY_X7, KEY_Y6},	 //11 选项3
		{TYPE_X18, 75 + 3 * (KEY_Y6 + SAFE_DIST6), KEY_X7, KEY_Y6},	 //12 选项4
		{TYPE_X18, 75 + 4 * (KEY_Y6 + SAFE_DIST6), KEY_X7, KEY_Y6},	 //13 选项5
		{TYPE_X16 - 2, 75, 414, KEY_Y6},							 //14
		{TYPE_X16 - 2, 75 + 1 * (KEY_Y6 + SAFE_DIST6), 414, KEY_Y6}, //15
		{TYPE_X16 - 2, 75 + 2 * (KEY_Y6 + SAFE_DIST6), 414, KEY_Y6}, //16
		{TYPE_X16 - 2, 75 + 3 * (KEY_Y6 + SAFE_DIST6), 414, KEY_Y6}, //17
		{TYPE_X16 - 2, 75 + 4 * (KEY_Y6 + SAFE_DIST6), 414, KEY_Y6}, // 18
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},													 //1 屏保
		{{KEY_Silver}, {0}, {72}, {5, 0, 0, 0}, {0}},													 //2 返回
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {6, 0, 0, 0}, {0}},									 //3 上一页
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {7, 0, 0, 0}, {0}},									 //4 下一页
		{{KEY_Silver}, {0}, {238}, {16, 0, 0, 0}, {0}},													 //5 还原all
		{{KEY_Silver}, {0}, {237}, {17, 0, 0, 0}, {0}},													 //6 还原
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},													 //7
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},													 //8
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //9 1
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //10 2
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //11 3
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //12 4
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {0, 0, 0, 0}, {0}}, //13 5
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {0, 0, 0, 0}, {0}}, //14 1
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {0, 0, 0, 0}, {0}}, //15 2
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {0, 0, 0, 0}, {0}}, //16 3
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {0, 0, 0, 0}, {0}}, //17 4
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {0, 0, 0, 0}, {0}}, //18 5
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {14}},
		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示
		{582, TYPE_Y2, TYPE_SIZE10, TYPE_SIZE5, {0}, {Color_Black}, {19, 19}},								   //2 当前值-标题
		{673, TYPE_Y2, TYPE_SIZE10, TYPE_SIZE5, {0}, {Color_Black}, {20, 21}},								   //3 复位值-标题
		{582, 75, TYPE_SIZE10, 56, {Color_LightGray}, {Color_Black}, {0}},									   //4 当前数值-1
		{582, 75 + 1 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_LightGray}, {Color_Black}, {0}},		   //5 当前数值-2
		{582, 75 + 2 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_LightGray}, {Color_Black}, {0}},		   //6 当前数值-3
		{582, 75 + 3 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_LightGray}, {Color_Black}, {0}},		   //7 当前数值-4
		{582, 75 + 4 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_LightGray}, {Color_Black}, {0}},		   //8 当前数值-5
		{674, 75, TYPE_SIZE10, 56, {Color_DarkGray}, {Color_Black}, {0}},									   //9 复位数值-1
		{674, 75 + 1 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_DarkGray}, {Color_Black}, {0}},		   //10 复位数值-2
		{674, 75 + 2 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_DarkGray}, {Color_Black}, {0}},		   //11 复位数值-3
		{674, 75 + 3 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_DarkGray}, {Color_Black}, {0}},		   //12 复位数值-4
		{674, 75 + 4 * (KEY_Y6 + SAFE_DIST6), TYPE_SIZE10, 56, {Color_DarkGray}, {Color_Black}, {0}},		   //13 复位数值-5

	};
	InitScreen();
	initViewStatus();
	updateViewStatus();
	SetLanguage(MENU_PROG);

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	DrawButton(keyd[KEY_PRODT_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_PRODT_BACK].border[0], keybmpind[KEY_PRODT_BACK].icon[0]);

	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值

		// /[按键键值返回预处理*begin]**********************************************************
		if (keydata == KEY_PRODT_DEFT || keydata == KEY_PRODT_MODI)
		{
			keydata = 0;
		}
		if (keydata == KEY_PRODT_PREV)
		{
			if (ParaList.pageNow == 1)
			{
				keydata = 0;
			}
		}
		if (keydata == KEY_PRODT_NEXT)
		{
			if (ParaList.pageNow == ParaList.pageNum)
			{
				keydata = 0;
			}
		}
		if (keydata >= KEY_PRODT_SELM1 && keydata <= KEY_PRODT_SELM5)
		{
			keydata = 0;
			i = keydata - KEY_PRODT_SELM1;
			if (ParaList.selectEnable[i] == 0)
			{
				keydata = 0;
			}
		}
		if (keydata >= KEY_PRODT_SELC1 && keydata <= KEY_PRODT_SELC5)
		{
			i = keydata - KEY_PRODT_SELC1;
			if (ParaList.selectEnable[i] == 0)
			{
				keydata = 0;
			}
		}
		if (keydata == KEY_PRODT_RESETA && ParaList.patternNum == 0)
		{
			keydata = 0;
		}
		if (keydata == KEY_PRODT_RESET)
		{
			U8 tmpCnt = 0;
			keydata = 0;
			for (i = 0; i < 256; i++)
			{
				if (selectParNoList[i] == 1)
				{
					tmpCnt++;
				}
			}
			if (tmpCnt == 0)
				keydata = 0;
		}
		//特定情况下不生效按键预处理
		//无效按键

		if (keydata == 0xff) //按键松开后的键值预处理
		{
			if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //按键按下的键值预处理    与提示性信息处理
		{
			//特定情况下不生效按键

			if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
			{
				keyon = keydata;
				if (keyon == KEY_PRODT_SCRNSVR) //无按键特效
				{
				}
				else if (keyon >= KEY_PRODT_SELM1 && keyon <= KEY_PRODT_SELM5) //无按键特效
				{
					DrawButtonOnZoomIn(keyd[keyon], 2);
				}
				else
				{
					DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
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
				if (beeponf == 0) //蜂鸣器标志位 //第一次进入
				{
					beeponf = 1;

					if (g_keyBellT) //按键时蜂鸣器发声时间
						Beep_On(g_keyBellT);

					switch (keyon) //按键按下标题提示语显示
					{
					case KEY_PRODT_SELC1:
					case KEY_PRODT_SELC2:
					case KEY_PRODT_SELC3:
					case KEY_PRODT_SELC4:
					case KEY_PRODT_SELC5:
						break;
					case KEY_PRODT_SELM1:
					case KEY_PRODT_SELM2:
					case KEY_PRODT_SELM3:
					case KEY_PRODT_SELM4:
					case KEY_PRODT_SELM5:
						break;
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //第二次进入
				{
					beeponf = 2;
					//带有长按连续功能按键的功能

					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}
				Times = 0; //当前
			}
		}
		else //按键没有按下
		{
			key_on_flag1 = 0;
		}

		// /[按键键值返回预处理*end]**********************************************************

		// /[按键松开键值处理*begin]**********************************************************
		if (keyoff)
		{
			if (keyoff >= KEY_PRODT_SELC1 && keyoff <= KEY_PRODT_SELC5)
			{
				U16 tmpPubParaNo = 600 + keyoff - KEY_PRODT_SELC1;
				if (g_publicbuf[tmpPubParaNo] < 204)
				{
					pno = MENU_PROPARA;
					poldno[pnoindex++] = MENU_DIFFERENT;
					// poldno[2] = MENU_DIFFERENT;
					// pnoindex = 3;
					g_parano = g_publicbuf[tmpPubParaNo];
				}
				else if (g_publicbuf[tmpPubParaNo] < 245)
				{
#if 0
					if ((g_user.newuser > 7) && (g_user.newuser < 10))
					{
						g_parano = g_publicbuf[tmpPubParaNo];
						pno = MENU_PROPARA;
						poldno[pnoindex++] = MENU_DIFFERENT;
						// poldno[2] = MENU_DIFFERENT;
						// pnoindex = 3;
					}
					else
					{
						g_parano = g_publicbuf[tmpPubParaNo];
						pno = MENU_FTUSER;
						poldno[pnoindex++] = MENU_DIFFERENT;
						// poldno[2] = MENU_DIFFERENT;
						// pnoindex = 3;
					}
#else
					g_parano = g_publicbuf[tmpPubParaNo];
					pno = MENU_PROPARA;
					poldno[pnoindex++] = MENU_DIFFERENT;
#endif
				}
			}
			else if (keyoff >= KEY_PRODT_SELM1 && keyoff <= KEY_PRODT_SELM5)
			{
				ParaList.noInPageNow = keyoff - KEY_PRODT_SELM1;
				ParaList.bFlashSelectFlag = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_PRODT_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_PRODT_BACK:
					// pno = poldno[1];
					// pnoindex = 1;
					pno = poldno[--pnoindex];
					break;
				case KEY_PRODT_PREV:
					if (ParaList.pageNow > 1)
					{
						ParaList.pageNow--;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				case KEY_PRODT_NEXT:
					if (ParaList.pageNow < ParaList.pageNum)
					{
						ParaList.pageNow++;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				case KEY_PRODT_RESETA:
					pno = MENU_RWHELP;
					poldno[pnoindex++] = MENU_DIFFERENT;
					// pnoindex = 3;
					hleppara = 63;
					g_Restorepropara = 1;
					break;
				case KEY_PRODT_RESET:
					tmp_code = 1;
					ParaList.bFlashPageFlag = 1;
					break;
				case KEY_PRODT_DEFT:
					g_modeselectf = 0;
					ParaList.bflashAllFlag = 1;
					break;
				case KEY_PRODT_MODI:
					g_modeselectf = 1;
					ParaList.bflashAllFlag = 1;
					break;
				}
			}
			keyoff = 0;
		}
		// /[按键松开键值处理*end]**********************************************************

		// /[按键弹起显示信息处理*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{

			if (keyon == KEY_PRODT_SCRNSVR) //不需要显示的按键
			{
			}
			else if (keyon >= KEY_PRODT_SELM1 && keyon <= KEY_PRODT_SELM5) //无按键特效
			{
				DrawButtonOnZoomInRes(keyd[keyon]);
			}
			else //常规按键
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[按键弹起显示信息处理*end]**********************************************************

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;
			ParaList.patternNum = 0;
			if (g_Restorepropara == 2) //还原成功参数
			{

				Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0]));
				g_modeselectf = 0;
				g_pageno1 = 1;
				g_pageno2 = 1;
				g_Restorepropara = 0;
				if (g_modeselectf > 1)
					g_modeselectf = 0;

				for (i = 0; i < 255; i++)
				{
					if (g_modeselectf == 1)
					{
						if (PREG[i * PREG_LENGTH + PREG_DEFAULT] != PREG[i * PREG_LENGTH + PREG_NOW])
						{

							g_publicbuf[600 + ParaList.patternNum] = i;
							ParaList.patternNum++;
						}
						for (i = 0; i < 255; i++)
							g_publicbuf[i + 256] = PREG[i * PREG_LENGTH + PREG_NOW];
					}
					else if (g_modeselectf == 0)
					{
						if (g_publicbuf[i + 256] != PREG[i * PREG_LENGTH + PREG_NOW])
						{

							g_publicbuf[600 + ParaList.patternNum] = i;
							ParaList.patternNum++;
						}
					}
				}
			}
			else
			{
				if (g_modeselectf > 1)
					g_modeselectf = 0;

				for (i = 0; i < 245; i++) //含部分不可还原参数
				{
					if (g_modeselectf == 0)
					{
						if (g_publicbuf[i + 256] != PREG[i * PREG_LENGTH + PREG_DEFAULT])
						{

							g_publicbuf[600 + ParaList.patternNum] = i;
							ParaList.patternNum++;
						}
					}
					else if (g_modeselectf == 1)
					{
						if (g_publicbuf[i + 256] != g_publicbuf[i])
						{

							g_publicbuf[600 + ParaList.patternNum] = i;
							ParaList.patternNum++;
						}
					}
				}
			}

			if (ParaList.patternNum != 0)
			{
				ParaList.selectNoNow = 1;
				ParaList.noInPageNow = (ParaList.selectNoNow - 1) % ParaList.disNumDiv;
				ParaList.noInPageLast = ParaList.noInPageNow;
				ParaList.pageNow = (ParaList.selectNoNow - 1) / ParaList.disNumDiv + 1;
				ParaList.pageLast = ParaList.pageNow;
				ParaList.pageNum = (ParaList.patternNum - 1) / ParaList.disNumDiv + 1;
			}
			else
			{
				ParaList.selectNoNow = 0;
				ParaList.noInPageNow = 0;
				ParaList.noInPageLast = ParaList.noInPageNow;
				ParaList.pageNow = 1;
				ParaList.pageLast = ParaList.pageNow;
				ParaList.pageNum = 1;
			}

			if (ParaList.pageNum == 0)
				ParaList.pageNum = 1;

			if (ParaList.pageNow == 0)
				ParaList.pageNow = 1;
			else if (ParaList.pageNow > ParaList.pageNum)
				ParaList.pageNow = ParaList.pageNum;

			if (ParaList.patternNum != 0)
			{
				Draw24x24Str(rect[RECT_PRODT_NOW_TIT].xpis + (rect[RECT_PRODT_NOW_TIT].xsize - 12 * strlen((char *)(MEM_TITLE_ADD + rect[RECT_PRODT_NOW_TIT].index[(g_modeselectf) ? 1 : 0] * TitleLengthMax))) / 2,
							 rect[RECT_PRODT_NOW_TIT].ypis + (rect[RECT_PRODT_NOW_TIT].ysize - 24) / 2,
							 (MEM_TITLE_ADD + rect[RECT_PRODT_NOW_TIT].index[(g_modeselectf) ? 1 : 0] * TitleLengthMax), SheetPage, rect[RECT_PRODT_NOW_TIT].index[(g_modeselectf) ? 1 : 0], rect[RECT_PRODT_NOW_TIT].colorT[0]); //画出方框的文字

				Draw24x24Str(rect[RECT_PRODT_DEF_TIT].xpis + (rect[RECT_PRODT_DEF_TIT].xsize - 12 * strlen((char *)(MEM_TITLE_ADD + rect[RECT_PRODT_DEF_TIT].index[(g_modeselectf) ? 1 : 0] * TitleLengthMax))) / 2,
							 rect[RECT_PRODT_DEF_TIT].ypis + (rect[RECT_PRODT_DEF_TIT].ysize - 24) / 2,
							 (MEM_TITLE_ADD + rect[RECT_PRODT_DEF_TIT].index[(g_modeselectf) ? 1 : 0] * TitleLengthMax), SheetPage, rect[RECT_PRODT_DEF_TIT].index[(g_modeselectf) ? 1 : 0], rect[RECT_PRODT_DEF_TIT].colorT[0]); //画出方框的文字

				DrawButton(keyd[KEY_PRODT_RESETA], (U16 *)(ADD_ICONBMP), keybmpind[KEY_PRODT_RESETA].border[0], keybmpind[KEY_PRODT_RESETA].icon[0]);
			}
			else
			{
				BmpFillRect(rect[RECT_PRODT_NOW_TIT].xpis, rect[RECT_PRODT_NOW_TIT].ypis, rect[RECT_PRODT_NOW_TIT].xsize, rect[RECT_PRODT_NOW_TIT].ysize);
				BmpFillRect(rect[RECT_PRODT_DEF_TIT].xpis, rect[RECT_PRODT_DEF_TIT].ypis, rect[RECT_PRODT_DEF_TIT].xsize, rect[RECT_PRODT_DEF_TIT].ysize);
				BmpFillRect(keyd[KEY_PRODT_RESETA].xpis, keyd[KEY_PRODT_RESETA].ypis, keyd[KEY_PRODT_RESETA].xsize, keyd[KEY_PRODT_RESETA].ysize);
			}
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;

			dis_update = 1;

			if (ParaList.dataAdd[ParaList.noInPageNow] >= 0xFFFF) //选中空
			{
			}
			else
			{
				if (selectParNoList[ParaList.noInPageNow + (ParaList.pageNow - 1) * ParaList.disNumDiv] == 1)
				{
					selectParNoList[ParaList.noInPageNow + (ParaList.pageNow - 1) * ParaList.disNumDiv] = 0;
				}
				else
				{
					selectParNoList[ParaList.noInPageNow + (ParaList.pageNow - 1) * ParaList.disNumDiv] = 1;
				}
				for (i = 0; i < ParaList.disNumDiv; i++)
				{
					ParaList.selectList[i] = selectParNoList[i + (ParaList.pageNow - 1) * ParaList.disNumDiv];
				}
			}
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U16 numA = 0;
			U16 addA = 0;
			ParaList.bFlashPageFlag = 0;
			dis_update = 1;

			//显示页码
			DrawFillRect(rect[RECT_PRODT_PAGE].xpis, rect[RECT_PRODT_PAGE].ypis,
						 rect[RECT_PRODT_PAGE].xsize, rect[RECT_PRODT_PAGE].ysize, rect[RECT_PRODT_PAGE].colorB[0]);
			DrawFU32(rect[RECT_PRODT_PAGE].xpis + SAFE_DIST2, rect[RECT_PRODT_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNow, 3, rect[RECT_PRODT_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_PRODT_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_PRODT_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_PRODT_PAGE].colorT[0]);
			DrawFU32(rect[RECT_PRODT_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_PRODT_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNum, 3, rect[RECT_PRODT_PAGE].colorT[0]);

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				ParaList.dataAdd[i] = 0xFFFF;
				ParaList.selectList[i] = 0;
				ParaList.selectEnable[i] = 0;
			}

			if (ParaList.patternNum != 0)
			{
				if (g_modeselectf == 0)
				{
					Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); //修改
					//寻找当前页的修改参数序号*begin***************
					numA = (ParaList.pageNow - 1) * ParaList.disNumDiv;
					for (addA = 0; addA < 255; addA++) //找到本页首地址
					{
						if (numA == 0)
							break;
						if (g_publicbuf[addA + 256] != PREG[addA * PREG_LENGTH + PREG_DEFAULT])
							numA--;
					}
					for (; addA < 255; addA++)
					{
						if (g_publicbuf[addA + 256] != PREG[addA * PREG_LENGTH + PREG_DEFAULT])
						{
							// g_publicbuf[600 + numA] = addA;
							ParaList.dataAdd[numA] = addA;	 //记录了不等于默认值的参数序号
							ParaList.selectEnable[numA] = 1; //对应的参数序号按键使能
							numA++;
							if (numA >= ParaList.disNumDiv)
								break;
						}
					}
					for (i = 0; i < ParaList.disNumDiv; i++) //获取已经选中的参数序号
					{
						ParaList.selectList[i] = selectParNoList[i + (ParaList.pageNow - 1) * ParaList.disNumDiv];
					}
					//寻找当前页的修改参数序号*end***************

					if (tmp_code == 1) //还原选定值
					{
						tmp_code = 0;
						for (i = 0; i < ParaList.patternNum; i++)
						{
							if (selectParNoList[i] == 1)
							{
								PREG[g_publicbuf[600 + i] * PREG_LENGTH + PREG_NOW] = PREG[g_publicbuf[600 + i] * PREG_LENGTH + PREG_DEFAULT];
							}
						}
						for (i = 0; i < ParaList.disNumDiv; i++) //获取已经选中的参数序号
						{
							selectParNoList[i + (ParaList.pageNow - 1) * ParaList.disNumDiv] = 0;
							ParaList.selectList[i] = 0;
						}
					}
				}
				else if (g_modeselectf == 1)
				{
					//寻找当前页的修改参数序号*begin***************
					numA = (ParaList.pageNow - 1) * ParaList.disNumDiv;
					for (addA = 0; addA < 255; addA++) //找到本页首地址
					{
						if (numA == 0)
							break;
						if (g_publicbuf[addA + 256] != g_publicbuf[addA])
							numA--;
					}
					for (; addA < 255; addA++)
					{
						if (g_publicbuf[addA + 256] != g_publicbuf[addA])
						{
							// g_publicbuf[600 + numA] = addA;
							ParaList.dataAdd[numA] = addA;	 //记录了不等于默认值的参数序号
							ParaList.selectEnable[numA] = 1; //对应的参数序号按键使能
							numA++;
							if (numA >= ParaList.disNumDiv)
								break;
						}
					}
					for (i = 0; i < ParaList.disNumDiv; i++) //获取已经选中的参数序号
					{
						ParaList.selectList[i] = selectParNoList[i + (ParaList.pageNow - 1) * ParaList.disNumDiv];
					}
					//寻找当前页的修改参数序号*end***************
					if (tmp_code == 1) //还原选定值
					{
						tmp_code = 0;
						for (i = 0; i < ParaList.patternNum; i++)
						{
							if (selectParNoList[i] == 1)
							{
								PREG[g_publicbuf[600 + i] * PREG_LENGTH + PREG_NOW] = g_publicbuf[g_publicbuf[600 + i]];
							}
						}
						for (i = 0; i < ParaList.disNumDiv; i++) //获取已经选中的参数序号
						{
							selectParNoList[i + (ParaList.pageNow - 1) * ParaList.disNumDiv] = 0;
							ParaList.selectList[i] = 0;
						}
					}
				}
			}
			DrawButton(keyd[KEY_PRODT_PREV], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PRODT_PREV].border[1] : keybmpind[KEY_PRODT_PREV].border[0],
					   (ParaList.pageNow == 1) ? keybmpind[KEY_PRODT_PREV].icon[1] : keybmpind[KEY_PRODT_PREV].icon[0]);
			DrawButton(keyd[KEY_PRODT_NEXT], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PRODT_NEXT].border[1] : keybmpind[KEY_PRODT_NEXT].border[0],
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_PRODT_NEXT].icon[1] : keybmpind[KEY_PRODT_NEXT].icon[0]);

			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		if (dis_update == 1)
		{
			dis_update = 0;
			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				U8 keyIndex = KEY_PRODT_SELC1 + i;
				U8 selectIndex = KEY_PRODT_SELM1 + i;
				U8 rectNowIndex = RECT_PRODT_NOW1 + i;
				U8 rectDefIndex = RECT_PRODT_DEF1 + i;
				if (ParaList.selectEnable[i] == 1 && ParaList.dataAdd[i] != 0xFFFF)
				{
					DrawButtonSingleColor(keyd[keyIndex],
										  (ParaList.selectList[i]) ? keybmpind[keyIndex].border[1] : keybmpind[keyIndex].border[0],
										  (ParaList.selectList[i]) ? keybmpind[keyIndex].icon[1] : keybmpind[keyIndex].icon[0]);
					Draw24x24Str(keyd[keyIndex].xpis + (keyd[keyIndex].xsize - strlen((char *)(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_PARANAME)) * 12) / 2,
								 keyd[keyIndex].ypis + (keyd[keyIndex].ysize - 24) / 2,
								 (MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_PARANAME), PROG_MSG2_Sheet, 1,
								 (ParaList.selectList[i]) ? keybmpind[keyIndex].textColor[1] : keybmpind[keyIndex].textColor[0]);

					DrawFillRect(keyd[selectIndex].xpis, keyd[selectIndex].ypis,
								 keyd[selectIndex].xsize, keyd[selectIndex].ysize, (ParaList.selectList[i]) ? keybmpind[selectIndex].icon[1] : keybmpind[selectIndex].icon[0]);
					Draw24x24MulStr(keyd[selectIndex].xpis + SAFE_DIST4, keyd[selectIndex].ypis + SAFE_DIST4,
									(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_MSG), linemaxdis, PROG_MSG2_Sheet,
									ParaList.dataAdd[i], 2,
									(ParaList.selectList[i]) ? keybmpind[selectIndex].textColor[1] : keybmpind[selectIndex].textColor[0]);

					DrawFillRect(rect[rectNowIndex].xpis, rect[rectNowIndex].ypis,
								 rect[rectNowIndex].xsize, rect[rectNowIndex].ysize, rect[rectNowIndex].colorB[0]);
					disProgVal(rect[rectNowIndex].xpis, rect[rectNowIndex].ypis, rect[rectNowIndex].xsize, rect[rectNowIndex].ysize,
							   CENTER, ParaList.dataAdd[i], PREG[ParaList.dataAdd[i] * PREG_LENGTH + PREG_NOW],
							   (*(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_TYPE)), rect[rectNowIndex].colorT[0]);

					DrawFillRect(rect[rectDefIndex].xpis, rect[rectDefIndex].ypis,
								 rect[rectDefIndex].xsize, rect[rectDefIndex].ysize, rect[rectDefIndex].colorB[0]);
					if (g_modeselectf == 0)
					{
						disProgVal(rect[rectDefIndex].xpis, rect[rectDefIndex].ypis, rect[rectDefIndex].xsize, rect[rectDefIndex].ysize,
								   CENTER, ParaList.dataAdd[i], PREG[ParaList.dataAdd[i] * PREG_LENGTH + PREG_DEFAULT],
								   (*(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_TYPE)), rect[rectDefIndex].colorT[0]);
					}
					else
					{
						disProgVal(rect[rectDefIndex].xpis, rect[rectDefIndex].ypis, rect[rectDefIndex].xsize, rect[rectDefIndex].ysize,
								   CENTER, ParaList.dataAdd[i], g_publicbuf[ParaList.dataAdd[i] + 256],
								   (*(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_TYPE)), rect[rectDefIndex].colorT[0]);
					}
				}
				else
				{
					BmpFillRect(keyd[keyIndex].xpis, keyd[keyIndex].ypis, keyd[keyIndex].xsize, keyd[keyIndex].ysize);
					BmpFillRect(keyd[selectIndex].xpis, keyd[selectIndex].ypis, keyd[selectIndex].xsize, keyd[selectIndex].ysize);
					BmpFillRect(rect[rectNowIndex].xpis, rect[rectNowIndex].ypis, rect[rectNowIndex].xsize, rect[rectNowIndex].ysize);
					BmpFillRect(rect[rectDefIndex].xpis, rect[rectDefIndex].ypis, rect[rectDefIndex].xsize, rect[rectDefIndex].ysize);
				}
			}
		}

		ErrorMsg(MENU_DIFFERENT);

		if (pno != MENU_DIFFERENT)
		{
			if ((pno != MENU_RWHELP) || (hleppara != 63))
				g_Restorepropara = 0;
			plastno = MENU_DIFFERENT;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

#define SETP_1 43
#define SETP_2 44
#define FP_SELCMAX 5

#define KEY_FPROG_SCRNSVR 1
#define KEY_FPROG_BACK 2
#define KEY_FPROG_PREV 3
#define KEY_FPROG_NEXT 4
#define KEY_FPROG_SELC1 5
#define KEY_FPROG_SELC2 6
#define KEY_FPROG_SELC3 7
#define KEY_FPROG_SELC4 8
#define KEY_FPROG_SELC5 9
#define KEY_FPROG_MAX 10

#define RECT_FPROG_PAGE 1
#define RECT_FPROG_PARA 2
#define RECT_FPROG_STR 3
#define RECT_FPROG_NUM 4

#define PARA_FAST_MAX 10
//------------------------------------------
// Function   : void Menu_FastProg(void)
// Description: 快速程序模组
//------------------------------------------
void Menu_FastProg(void)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;

	// U8 fp_page = 1;
	// U8 fp_pagemax = 2;
	// U8 disupdata = 1;

	// U8 ref = 0;
	U16 modProgNo[10] = {0};
	U16 modProgVal[10] = {0};
	U16 modProgType[10] = {0};
	U16 PROG_MSG2_Sheet = 45;
	U8 bFirst = 1;
	tydPatternList ParaList = {1, 0, 0, 5, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数

	// U8 setProInd = 0, osetProInd = 0;
	const U8 keynum = {KEY_FPROG_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},							   //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},			   //2 返回
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},			   //3 上一页
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},			   //4 下一页
		{TYPE_X35, TYPE_Y44, KEY_X7, KEY_Y5},		   //5 参数选项1
		{TYPE_X35, TYPE_Y44 + 74, KEY_X7, KEY_Y5},	   //6 参数选项2
		{TYPE_X35, TYPE_Y44 + 2 * 74, KEY_X7, KEY_Y5}, //7 参数选项3
		{TYPE_X35, TYPE_Y44 + 3 * 74, KEY_X7, KEY_Y5}, //8 参数选项4
		{TYPE_X35, TYPE_Y44 + 4 * 74, KEY_X7, KEY_Y5}, //9 参数选项5
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {TLFNADD_FPROG + 1, 0, 0, 0}, {0}},					 //1 屏保
		{{KEY_Silver, KEY_Gray}, {0}, {241, 242}, {TLFNADD_FPROG + 2, 0, 0, 0}, {0}},	 //2 返回
		{{KEY_Silver, KEY_Gray}, {0}, {223, 224}, {TLFNADD_FPROG + 3, 0, 0, 0}, {0}},	 //3 上一页
		{{KEY_Silver, KEY_Gray}, {0}, {225, 226}, {TLFNADD_FPROG + 4, 0, 0, 0}, {0}},	 //4 下一页
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_FPROG + 7, 0, 0, 0}, {0}},	 //5  选项1
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_FPROG + 8, 0, 0, 0}, {0}},	 //6  选项2
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_FPROG + 9, 0, 0, 0}, {0}},	 //7  选项3
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_FPROG + 10, 0, 0, 0}, {0}}, //8  选项4
		{{KEY_Silver}, {Color_Black}, {KEY_Silver}, {TLFNADD_FPROG + 11, 0, 0, 0}, {0}}, //9 选项5
	};
	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FPROG}},

		{TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示
		{TYPE_X45, TYPE_Y2, 632, 66, {Color_Black}, {Color_Black}, {0}},									   //2 整体框显示框
		{TYPE_X45 + 1, TYPE_Y2 + 1, 630, 32, {Color_Silver}, {Color_Black}, {0}},							   //3 文本显示框
		{TYPE_X45 + 1, TYPE_Y2 + 33, 630, 32, {Color_White}, {Color_Black}, {0}},							   //4 参数显示框

	};
	SetLanguage(MENU_PROG);
	InitScreen();
	initViewStatus();
	updateViewStatus();
	setParaNoLimit(PROG_FAST_MAX);

	UADD_PROG_MSG2SE = ADD_PROG_MSG21SE;
	if (P_LAN)
	{
		//USE_PROG_MSG1  = MEM_PROG_MSG11_E;
		//USE_PROG_MSG2  = MEM_PROG_MSG2_E;
		//USE_PROG_MSG3  = MEM_PROG_MSG3_E;
		//USE_PROG_MSG4  = MEM_PROG_MSG4_E;
		UADD_PROG_MSG1 = ADD_PROG_MSG11_E;
		UADD_PROG_MSG2 = ADD_PROG_MSG2_E;
		UADD_PROG_MSG3 = ADD_PROG_MSG3_E;
		UADD_PROG_MSG4 = ADD_PROG_MSG4_E;
	}
	else
	{
		//USE_PROG_MSG1  = MEM_PROG_MSG11;
		//USE_PROG_MSG2  = MEM_PROG_MSG2;
		//USE_PROG_MSG3  = MEM_PROG_MSG3;
		//USE_PROG_MSG4  = MEM_PROG_MSG4;
		UADD_PROG_MSG1 = ADD_PROG_MSG11;
		UADD_PROG_MSG2 = ADD_PROG_MSG2;
		UADD_PROG_MSG3 = ADD_PROG_MSG3;
		UADD_PROG_MSG4 = ADD_PROG_MSG4;
	}

	Read_D(D_Address[DISP] + D_PGROP, PARA_FAST_MAX, modProgNo);
	for (i = 0; i < PARA_FAST_MAX; i++)
	{
		if (modProgNo[i] == 0 || modProgNo[i] > getParaNoLimit())
		{
			modProgNo[i] = 0xffff;
		}
		else
		{
			modProgNo[i] -= 1;
		}
	}

	Timer_ms = 30720;
	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//第一页的上一页按钮，最后一页的下一页按钮和4到9按钮无效
		if (keydata == KEY_FPROG_PREV)
		{
			if (ParaList.pageNow == 1)
			{
				keydata = 0;
			}
		}
		if (keydata == KEY_FPROG_NEXT)
		{
			if (ParaList.pageNow == ParaList.pageNum)
			{
				keydata = 0;
			}
		}
		if (keydata >= KEY_FPROG_SELC1 && keydata <= KEY_FPROG_SELC5)
		{
			i = keydata - KEY_FPROG_SELC1;
			if (ParaList.selectEnable[i] == 0)
			{
				keydata = 0;
			}
		}

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
				Timer_ms = 0;
				if (keyon == KEY_FPROG_SCRNSVR)
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
					case KEY_FPROG_SELC1:
					case KEY_FPROG_SELC2:
					case KEY_FPROG_SELC3:
					case KEY_FPROG_SELC4:
					case KEY_FPROG_SELC5:
						DrawFMenuStr(keybmpind[keyon].titleStr[0] + ParaList.pageNow * ParaList.disNumDiv);
						break;
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

		if ((keyon >= KEY_FPROG_SELC1) && (keyon <= KEY_FPROG_SELC5) && Timer_ms > 1000)
		{
			ParaList.selectNoNow = keyon - KEY_FPROG_SELC1 + (ParaList.pageNow - 1) * ParaList.disNumDiv;

			modProgNo[ParaList.selectNoNow] = Menu_SetProgNo(modProgNo[ParaList.selectNoNow]);
			modProgVal[ParaList.selectNoNow] = getProgVal(modProgNo[ParaList.selectNoNow]);
			modProgType[ParaList.selectNoNow] = getProgType(modProgNo[ParaList.selectNoNow]);
			for (i = 0; i < PARA_FAST_MAX; i++)
			{
				if (modProgNo[i] >= getParaNoLimit())
				{
					modProgNo[i] = 0;
				}
				else
				{
					modProgNo[i] += 1;
				}
			}
			Write_D(D_Address[DISP] + D_PGROP, PARA_FAST_MAX, modProgNo);
			for (i = 0; i < PARA_FAST_MAX; i++)
			{
				if (modProgNo[i] == 0 || modProgNo[i] > getParaNoLimit())
				{
					modProgNo[i] = 0xffff;
				}
				else
				{
					modProgNo[i] -= 1;
				}
			}

			ParaList.bflashAllFlag = 1;
			keyon = 0;
			keyoff = 0;
			beeponf = 0;
		}

		if (keyoff)
		{
			if ((keyoff >= KEY_FPROG_SELC1) && (keyoff <= KEY_FPROG_SELC5))
			{
				ParaList.bFlashSelectFlag = 1;
				ParaList.selectNoNow = keyoff - KEY_FPROG_SELC1 + (ParaList.pageNow - 1) * ParaList.disNumDiv;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_FPROG_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_FPROG_BACK:
					for (i = 0; i < PARA_FAST_MAX; i++)
					{
						if (modProgNo[i] >= getParaNoLimit())
						{
							modProgNo[i] = 0;
						}
						else
						{
							modProgNo[i] += 1;
						}
					}
					Write_D(D_Address[DISP] + D_PGROP, PARA_FAST_MAX, modProgNo);
					Read_Status();
					if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
					Write_Cmd(C_SAVE_MPROG, 0, 0);
					Read_Status();
					if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}

					for (i = 0; i < PARA_FAST_MAX; i++)
					{
						setProgVal(modProgNo[i] - 1, modProgVal[i]);
					}
					Write_Cmd(C_UPDATEPARA, 0, 0);
					Read_Status();
					if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
					pno = poldno[--pnoindex];
					break;

				case KEY_FPROG_PREV:
					if (ParaList.pageNow > 1)
					{
						ParaList.pageNow--;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				case KEY_FPROG_NEXT:
					if (ParaList.pageNow < ParaList.pageNum)
					{
						ParaList.pageNow++;
						ParaList.bFlashPageFlag = 1;
					}
					break;
				}
			}
			keyoff = 0;
		}

		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_FPROG_SCRNSVR) //不需要显示的按键
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

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;
			if (bFirst != 1)
			{
				SetLanguage(MENU_PROG);
				InitScreen();
				initViewStatus();
				updateViewStatus();
				ViewPage(usepageno);
			}
			bFirst = 0;

			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]);
			DrawButton(keyd[KEY_FPROG_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_FPROG_BACK].border[0], keybmpind[KEY_FPROG_BACK].icon[0]);

			for (i = 0; i < PARA_FAST_MAX; i++)
			{
				modProgVal[i] = getProgVal(modProgNo[i]);
				modProgType[i] = getProgType(modProgNo[i]);
			}

			ParaList.patternNum = PARA_FAST_MAX;
			ParaList.pageNow = 1;
			ParaList.pageLast = ParaList.pageNow;
			ParaList.pageNum = (ParaList.patternNum - 1) / ParaList.disNumDiv + 1;

			if (ParaList.pageNum == 0)
				ParaList.pageNum = 1;

			if (ParaList.pageNow == 0)
				ParaList.pageNow = 1;
			else if (ParaList.pageNow > ParaList.pageNum)
				ParaList.pageNow = ParaList.pageNum;
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;
			if (modProgNo[ParaList.selectNoNow] == 0xffff)
			{
				ParaList.selectNoNow = 0;
			}
			else
			{
				g_parano = modProgNo[ParaList.selectNoNow];
				pno = MENU_PROPARA;
				poldno[pnoindex++] = MENU_FPROG;
			}
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U8 linemaxdis = 35;
			ParaList.bFlashPageFlag = 0;

			//显示页码
			DrawFillRect(rect[RECT_FPROG_PAGE].xpis, rect[RECT_FPROG_PAGE].ypis,
						 rect[RECT_FPROG_PAGE].xsize, rect[RECT_FPROG_PAGE].ysize, rect[RECT_FPROG_PAGE].colorB[0]);
			DrawFU32(rect[RECT_FPROG_PAGE].xpis + SAFE_DIST2, rect[RECT_FPROG_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNow, 3, rect[RECT_FPROG_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_FPROG_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_FPROG_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_FPROG_PAGE].colorT[0]);
			DrawFU32(rect[RECT_FPROG_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_FPROG_PAGE].ypis + SAFE_DIST4,
					 ParaList.pageNum, 3, rect[RECT_FPROG_PAGE].colorT[0]);

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				ParaList.selectEnable[i] = 1;
				ParaList.dataAdd[i] = modProgNo[i + (ParaList.pageNow - 1) * ParaList.disNumDiv];
			}

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				U8 keyIndex = KEY_FPROG_SELC1 + i;
				U16 rectdy = 74 * i;
				if (ParaList.selectEnable[i] == 1)
				{
					DrawButtonSingleColor(keyd[keyIndex], keybmpind[keyIndex].border[0], keybmpind[keyIndex].icon[0]);
					if (ParaList.dataAdd[i] != 0xFFFF)
					{

						Draw24x24Str(keyd[keyIndex].xpis + (keyd[keyIndex].xsize - strlen((char *)(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_PARANAME)) * 12) / 2,
									 keyd[keyIndex].ypis + (keyd[keyIndex].ysize - 24) / 2,
									 (MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_PARANAME), PROG_MSG2_Sheet, 1, keybmpind[keyIndex].textColor[0]);
					}

					DrawRect(rect[RECT_FPROG_PARA].xpis, rect[RECT_FPROG_PARA].ypis + rectdy,
							 rect[RECT_FPROG_PARA].xsize, rect[RECT_FPROG_PARA].ysize, rect[RECT_FPROG_PARA].colorB[0]);
					DrawFillRect(rect[RECT_FPROG_STR].xpis, rect[RECT_FPROG_STR].ypis + rectdy,
								 rect[RECT_FPROG_STR].xsize, rect[RECT_FPROG_STR].ysize, rect[RECT_FPROG_STR].colorB[0]);
					BmpFillRect(rect[RECT_FPROG_NUM].xpis, rect[RECT_FPROG_NUM].ypis + rectdy, rect[RECT_FPROG_NUM].xsize, rect[RECT_FPROG_NUM].ysize);
					if (ParaList.dataAdd[i] != 0xFFFF)
					{
						Draw24x24MulStr(rect[RECT_FPROG_STR].xpis + SAFE_DIST4, rect[RECT_FPROG_STR].ypis + SAFE_DIST4 + rectdy,
										(MEM_PROG_MSG2 + ParaList.dataAdd[i] * MSG2_LENGTH + PMSG2_MSG), linemaxdis, PROG_MSG2_Sheet,
										ParaList.dataAdd[i], 1, rect[RECT_FPROG_STR].colorT[0]);

						disProgVal(rect[RECT_FPROG_NUM].xpis + SAFE_DIST4, rect[RECT_FPROG_NUM].ypis + SAFE_DIST4 + rectdy, 0, 0, LEFT, ParaList.dataAdd[i],
								   modProgVal[i + (ParaList.pageNow - 1) * ParaList.disNumDiv],
								   modProgType[i + (ParaList.pageNow - 1) * ParaList.disNumDiv], rect[RECT_FPROG_NUM].colorT[0]);
					}
				}
				else
				{
					BmpFillRect(keyd[keyIndex].xpis, keyd[keyIndex].ypis, keyd[keyIndex].xsize, keyd[keyIndex].ysize);
					BmpFillRect(rect[RECT_FPROG_PARA].xpis, rect[RECT_FPROG_PARA].ypis + rectdy, rect[RECT_FPROG_PARA].xsize, rect[RECT_FPROG_PARA].ysize);
				}
			}

			DrawButton(keyd[KEY_FPROG_PREV], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == 1) ? keybmpind[KEY_FPROG_PREV].border[1] : keybmpind[KEY_FPROG_PREV].border[0],
					   (ParaList.pageNow == 1) ? keybmpind[KEY_FPROG_PREV].icon[1] : keybmpind[KEY_FPROG_PREV].icon[0]);
			DrawButton(keyd[KEY_FPROG_NEXT], (U16 *)(ADD_ICONBMP),
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_FPROG_NEXT].border[1] : keybmpind[KEY_FPROG_NEXT].border[0],
					   (ParaList.pageNow == ParaList.pageNum) ? keybmpind[KEY_FPROG_NEXT].icon[1] : keybmpind[KEY_FPROG_NEXT].icon[0]);

			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		if (pno != MENU_FPROG)
		{
			plastno = MENU_FPROG;
			break;
		}
		updateViewStatus();
		wdt();
	}
}

//--------------------------------------
#define KEY_FPROGS_SCRNSVR 1
#define KEY_FPROGS_BACK 2
#define KEY_FPROGS_ENTER 3
#define KEY_FPROGS_CLR 4
#define KEY_FPROGS_0 5
#define KEY_FPROGS_1 6
#define KEY_FPROGS_2 7
#define KEY_FPROGS_3 8
#define KEY_FPROGS_4 9
#define KEY_FPROGS_5 10
#define KEY_FPROGS_6 11
#define KEY_FPROGS_7 12
#define KEY_FPROGS_8 13
#define KEY_FPROGS_9 14
#define KEY_FPROGS_A 15
#define KEY_FPROGS_B 16
#define KEY_FPROGS_C 17
#define KEY_FPROGS_D 18
#define KEY_FPROGS_E 19
#define KEY_FPROGS_F 20
#define KEY_FPROGS_G 21
#define KEY_FPROGS_H 22
#define KEY_FPROGS_I 23
#define KEY_FPROGS_J 24
#define KEY_FPROGS_K 25
#define KEY_FPROGS_L 26
#define KEY_FPROGS_M 27
#define KEY_FPROGS_N 28
#define KEY_FPROGS_O 29
#define KEY_FPROGS_P 30
#define KEY_FPROGS_Q 31
#define KEY_FPROGS_R 32
#define KEY_FPROGS_S 33
#define KEY_FPROGS_T 34
#define KEY_FPROGS_U 35
#define KEY_FPROGS_V 36
#define KEY_FPROGS_W 37
#define KEY_FPROGS_X 38
#define KEY_FPROGS_Y 39
#define KEY_FPROGS_Z 40
#define KEY_FPROGS_UP 41
#define KEY_FPROGS_DN 42
#define KEY_FPROGS_MAX 43
// #define KEY_FPROG_SEL 43
//#define		KEY_FPROGS_INPUT	43
//#define		KEY_FPROGS_INQU		44

#define RECT_FPROG_CODE_TIT 1
#define RECT_FPROG_CODE_STR 2
#define RECT_FPROG_NAME_STR 3
#define RECT_FPROG_SELAll 4
#define RECT_FPROG_SEL1 5
#define RECT_FPROG_SEL2 6
#define RECT_FPROG_SEL3 7
#define RECT_FPROG_SEL4 8
#define RECT_FPROG_SEL5 9
#define RECT_FPROG_SEL6 10
#define RECT_FPROG_SEL7 11

#define INPUT_CODE_MAX 6
//------------------------------------------
// Function   : void Menu_FastProg(void)
// Description: 快速程序模组,inNo:0-255
//------------------------------------------
U16 Menu_SetProgNo(U16 inNo)
{
	U8 beeponf = 0, delay = KEYDT;
	U32 keyon = 0, keyoff = 0, keydata = 0;
	U32 i;
	U8 code[INPUT_CODE_MAX] = {0}, codelen = 0;
	U8 loop = 1;
	U8 bFirst = 1;
	U16 screenList[PROG_FAST_MAX] = {0}; //筛选后的参数序号索引表，目前无效，等价于参数表
	tydPatternList ParaList = {1, 1, 0, 7, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}};
	const U8 keynum = {KEY_FPROGS_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},					 //1 屏保
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //2 返回
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 确认

		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 10, TYPE_Y95, KEY_X5, KEY_Y2}, //4 清除
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y47, KEY_X2, KEY_Y2},  //5 0
		{TYPE_X47, TYPE_Y47, KEY_X2, KEY_Y2},							   //6 1
		{TYPE_X47 + KEY_X2 + SAFE_DIST2, TYPE_Y47, KEY_X2, KEY_Y2},		   //7 2
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y47, KEY_X2, KEY_Y2},  //8 3
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y47, KEY_X2, KEY_Y2},  //9 4
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y47, KEY_X2, KEY_Y2},  //10 5
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y47, KEY_X2, KEY_Y2},  //11 6
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y47, KEY_X2, KEY_Y2},  //12 7
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y47, KEY_X2, KEY_Y2},  //13 8
		{TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y47, KEY_X2, KEY_Y2},  //14 9

		{TYPE_X49, TYPE_Y49, KEY_X2, KEY_Y2},							  //15 A
		{TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y50, KEY_X2, KEY_Y2}, //16 B
		{TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y50, KEY_X2, KEY_Y2}, //17 C
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y49, KEY_X2, KEY_Y2}, //18 D
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y48, KEY_X2, KEY_Y2}, //19 E
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y49, KEY_X2, KEY_Y2}, //20 F
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y49, KEY_X2, KEY_Y2}, //21 G
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y49, KEY_X2, KEY_Y2}, //22 H
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y48, KEY_X2, KEY_Y2}, //23 I
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y49, KEY_X2, KEY_Y2}, //24 J
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y49, KEY_X2, KEY_Y2}, //25 K
		{TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y49, KEY_X2, KEY_Y2}, //26 L
		{TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y50, KEY_X2, KEY_Y2}, //27 M
		{TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y50, KEY_X2, KEY_Y2}, //28 N
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y48, KEY_X2, KEY_Y2}, //29 O
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y48, KEY_X2, KEY_Y2}, //30 P
		{TYPE_X48, TYPE_Y48, KEY_X2, KEY_Y2},							  //31 Q
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y48, KEY_X2, KEY_Y2}, //32 R
		{TYPE_X49 + KEY_X2 + SAFE_DIST2, TYPE_Y49, KEY_X2, KEY_Y2},		  //33 S
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y48, KEY_X2, KEY_Y2}, //34 T
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y48, KEY_X2, KEY_Y2}, //35 U
		{TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y50, KEY_X2, KEY_Y2}, //36 V
		{TYPE_X48 + KEY_X2 + SAFE_DIST2, TYPE_Y48, KEY_X2, KEY_Y2},		  //37 W
		{TYPE_X50 + KEY_X2 + SAFE_DIST2, TYPE_Y50, KEY_X2, KEY_Y2},		  //38 X
		{TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y48, KEY_X2, KEY_Y2}, //39 Y
		{TYPE_X50, TYPE_Y50, KEY_X2, KEY_Y2},							  //40 Z
		{670, 75, KEY_X1, KEY_Y1},										  //41   上翻
		{670, 75 + 75, KEY_X1, KEY_Y1},									  //42   下翻
																		  // {0, 0, 0, 0},													  //43   选取
	};
	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},

		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},				   //1 屏保
		{{KEY_Silver}, {0}, {72}, {26, 0, 0, 0}, {0}},				   //2 返回
		{{KEY_Silver}, {0}, {71}, {27, 0, 0, 0}, {0}},				   //3 确认
		{{KEY_Silver}, {0}, {189}, {28, 0, 0, 0}, {0}},				   //4 删除一位
		{{KEY_Silver, KEY_Gray}, {0}, {91, 101}, {10, 0, 0, 0}, {0}},  //5 0
		{{KEY_Silver, KEY_Gray}, {0}, {92, 102}, {11, 0, 0, 0}, {0}},  //6 1
		{{KEY_Silver, KEY_Gray}, {0}, {93, 103}, {12, 0, 0, 0}, {0}},  //7 2
		{{KEY_Silver, KEY_Gray}, {0}, {94, 104}, {13, 0, 0, 0}, {0}},  //8 3
		{{KEY_Silver, KEY_Gray}, {0}, {95, 105}, {14, 0, 0, 0}, {0}},  //9 4
		{{KEY_Silver, KEY_Gray}, {0}, {96, 106}, {15, 0, 0, 0}, {0}},  //10 5
		{{KEY_Silver, KEY_Gray}, {0}, {97, 107}, {16, 0, 0, 0}, {0}},  //11 6
		{{KEY_Silver, KEY_Gray}, {0}, {98, 108}, {17, 0, 0, 0}, {0}},  //12 7
		{{KEY_Silver, KEY_Gray}, {0}, {99, 109}, {18, 0, 0, 0}, {0}},  //13 8
		{{KEY_Silver, KEY_Gray}, {0}, {100, 110}, {19, 0, 0, 0}, {0}}, //14 9

		{{KEY_Silver}, {0}, {121}, {20, 0, 0, 0}, {0}},						 //15 A
		{{KEY_Silver}, {0}, {122}, {21, 0, 0, 0}, {0}},						 //16 B
		{{KEY_Silver}, {0}, {123}, {22, 0, 0, 0}, {0}},						 //17 C
		{{KEY_Silver}, {0}, {124}, {23, 0, 0, 0}, {0}},						 //18 D
		{{KEY_Silver}, {0}, {125}, {24, 0, 0, 0}, {0}},						 //19 E
		{{KEY_Silver}, {0}, {126}, {25, 0, 0, 0}, {0}},						 //20 F
		{{KEY_Silver}, {0}, {127}, {26, 0, 0, 0}, {0}},						 //21 G
		{{KEY_Silver}, {0}, {128}, {27, 0, 0, 0}, {0}},						 //22 H
		{{KEY_Silver}, {0}, {129}, {28, 0, 0, 0}, {0}},						 //23 I
		{{KEY_Silver}, {0}, {130}, {29, 0, 0, 0}, {0}},						 //24 J
		{{KEY_Silver}, {0}, {131}, {30, 0, 0, 0}, {0}},						 //25 K
		{{KEY_Silver}, {0}, {132}, {31, 0, 0, 0}, {0}},						 //26 L
		{{KEY_Silver}, {0}, {133}, {32, 0, 0, 0}, {0}},						 //27 M
		{{KEY_Silver}, {0}, {134}, {33, 0, 0, 0}, {0}},						 //28 N
		{{KEY_Silver}, {0}, {135}, {34, 0, 0, 0}, {0}},						 //29 O
		{{KEY_Silver}, {0}, {136}, {35, 0, 0, 0}, {0}},						 //30 P
		{{KEY_Silver}, {0}, {137}, {36, 0, 0, 0}, {0}},						 //31 Q
		{{KEY_Silver}, {0}, {138}, {37, 0, 0, 0}, {0}},						 //32 R
		{{KEY_Silver}, {0}, {139}, {38, 0, 0, 0}, {0}},						 //33 S
		{{KEY_Silver}, {0}, {140}, {39, 0, 0, 0}, {0}},						 //34 T
		{{KEY_Silver}, {0}, {141}, {40, 0, 0, 0}, {0}},						 //35 U
		{{KEY_Silver}, {0}, {142}, {41, 0, 0, 0}, {0}},						 //36 V
		{{KEY_Silver}, {0}, {143}, {42, 0, 0, 0}, {0}},						 //37 W
		{{KEY_Silver}, {0}, {144}, {43, 0, 0, 0}, {0}},						 //38 X
		{{KEY_Silver}, {0}, {145}, {44, 0, 0, 0}, {0}},						 //39 Y
		{{KEY_Silver}, {0}, {146}, {45, 0, 0, 0}, {0}},						 //40 Z
		{{KEY_Silver}, {0}, {219, 220}, {TLFNADD_FPROGS + 5, 0, 0, 0}, {0}}, //41 上翻
		{{KEY_Silver}, {0}, {221, 222}, {TLFNADD_FPROGS + 6, 0, 0, 0}, {0}}, //42 下翻
																			 // {{KEY_Blue}, {Color_Black}, {Color_DeepSkyBlue}, {0, 0, 0, 0}, {0}}, //43 选择
	};

	const RECTPAD rect[] = {
		//方框
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FPROGS}},

		{TYPE_X51, TYPE_Y51, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FPROGS + 7}},										 //1 标题代码-标题
		{TYPE_X51 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y51, TYPE_SIZE17, TYPE_SIZE5, {Color_DeepSkyBlue, Color_White}, {Color_Black, Color_Black}, {0}}, //2 标题代码-内容
		{TYPE_X51, TYPE_Y52, 480, 136, {Color_Black, Color_White}, {Color_Black}, {0}},																 //3 文本显示框
		{TYPE_X52, TYPE_Y51, TYPE_SIZE14 + 2, TYPE_SIZE4 * 6 + TYPE_SIZE5 + 2, {Color_Black, Color_White}, {Color_Black}, {0}},						 //4 参数框-大

		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2, TYPE_SIZE14, TYPE_SIZE4, {Color_Gray}, {Color_Black}, {0}},									  //5 参数框-1
		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2 + TYPE_SIZE4, TYPE_SIZE14, TYPE_SIZE4, {Color_DarkGray}, {Color_Black}, {0}},					  //6 参数框-2
		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2 + TYPE_SIZE4 * 2, TYPE_SIZE14, TYPE_SIZE4, {Color_LightGray}, {Color_Black}, {0}},			  //7 参数框-3
		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2 + TYPE_SIZE4 * 3, TYPE_SIZE14, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}},			  //8 参数框-4
		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2 + TYPE_SIZE4 * 3 + TYPE_SIZE5, TYPE_SIZE14, TYPE_SIZE4, {Color_LightGray}, {Color_Black}, {0}}, //9 参数框-5
		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2 + TYPE_SIZE4 * 4 + TYPE_SIZE5, TYPE_SIZE14, TYPE_SIZE4, {Color_DarkGray}, {Color_Black}, {0}},  //10 参数框-6
		{TYPE_X52 + SAFE_DIST2, TYPE_Y51 + SAFE_DIST2 + TYPE_SIZE4 * 5 + TYPE_SIZE5, TYPE_SIZE14, TYPE_SIZE4, {Color_Gray}, {Color_Black}, {0}},	  //11 参数框-7

	};
	SetLanguage(MENU_PROG);
	InitScreen();
	initViewStatus();
	updateViewStatus();
	setParaNoLimit(PROG_FAST_MAX);

	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

	for (i = KEY_FPROGS_BACK; i <= KEY_FPROGS_DN; i++)
	{
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	DrawEditTit(rect[RECT_FPROG_CODE_TIT], rect[RECT_FPROG_CODE_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST4);
	DrawRect(rect[RECT_FPROG_SELAll].xpis, rect[RECT_FPROG_SELAll].ypis, rect[RECT_FPROG_SELAll].xsize, rect[RECT_FPROG_SELAll].ysize, rect[RECT_FPROG_SELAll].colorB[0]);
	DrawEdit(rect[RECT_FPROG_CODE_STR], rect[RECT_FPROG_CODE_STR].colorT[0], rect[RECT_FPROG_CODE_STR].colorB[0]);

	ViewPage(usepageno);
	while (loop)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);

		//第一页的上一页按钮，最后一页的下一页按钮和4到9按钮无效

		//键值为按键松开时
		if (keydata == 0xff)
		{
			if ((keyon == KEY_FPROGS_UP) || (keyon == KEY_FPROGS_DN) || (keyon == KEY_FPROGS_CLR))
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
				if (keyon == KEY_FPROGS_SCRNSVR)
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
					if (keyon >= KEY_FPROGS_BACK && keyon <= KEY_FPROGS_CLR)
					{
						DrawFMenuStrP(MENU_WRITE, keybmpind[keyon].titleStr[0]);
					}
					else
					{
						switch (keyon) //按键按下标题提示语显示
						{

						default:
							DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						}
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
				if ((keyon == KEY_FPROGS_UP) || (keyon == KEY_FPROGS_DN) || (keyon == KEY_FPROGS_CLR))
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
			if ((keyoff >= KEY_FPROGS_0) && (keyoff <= KEY_FPROGS_9))
			{
				if (codelen < INPUT_CODE_MAX)
				{
					code[codelen++] = '0' + keyoff - KEY_FPROGS_0;
					ParaList.bFlashSelectFlag = 1;
				}
			}
			else if ((keyoff >= KEY_FPROGS_A) && (keyoff <= KEY_FPROGS_Z))
			{
				if (codelen < INPUT_CODE_MAX)
				{
					code[codelen++] = 'A' + keyoff - KEY_FPROGS_A;
					ParaList.bFlashSelectFlag = 1;
				}
			}
			else
			{
				switch (keyoff)
				{
				case KEY_FPROGS_SCRNSVR:
					loop = 0;
					break;
				case KEY_FPROGS_BACK:
					screenList[ParaList.selectNoNow] = inNo; //还原进来时的编号
					loop = 0;
					break;
				case KEY_FPROGS_ENTER:
					loop = 0;
					break;
				case KEY_FPROGS_CLR:
					if (codelen)
					{
						code[--codelen] = 0;
						ParaList.bFlashSelectFlag = 1;
					}
					break;
				case KEY_FPROGS_UP:
					if (ParaList.selectNoNow > 0)
					{
						ParaList.selectNoNow--;
					}
					else
					{
						ParaList.selectNoNow = ParaList.patternNum - 1;
					}
					ParaList.bFlashPageFlag = 1;

					break;
				case KEY_FPROGS_DN:
					if (ParaList.selectNoNow < ParaList.patternNum - 1)
					{
						ParaList.selectNoNow++;
					}
					else
					{
						ParaList.selectNoNow = 0;
					}
					ParaList.bFlashPageFlag = 1;

					break;
				}
			}
			keyoff = 0;
		}

		if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
		{
			if (keyon == KEY_FPROGS_SCRNSVR) //不需要显示的按键
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

		// /[更新参数列表数据*begin]**********************************************************
		if (ParaList.bflashAllFlag) //索引信息更新
		{
			ParaList.bflashAllFlag = 0;
			ParaList.bFlashPageFlag = 1;

			if (bFirst == 1)
			{
				ParaList.patternNum = getParaNoLimit();
				ParaList.selectNoNow = inNo;
				for (i = 0; i < ParaList.patternNum; i++)
				{
					screenList[i] = i;
				}

				if (ParaList.selectNoNow > ParaList.patternNum - 1)
				{
					ParaList.selectNoNow = 0;
				}
				ParaList.selectNoNow = screenList[ParaList.selectNoNow];
				ParaList.selectNoLast = ParaList.selectNoNow;
			}
			bFirst = 0;
		}

		if (ParaList.bFlashSelectFlag) //索引选中项更新
		{
			ParaList.bFlashSelectFlag = 0;

			DrawEdit(rect[RECT_FPROG_CODE_STR], rect[RECT_FPROG_CODE_STR].colorT[0], rect[RECT_FPROG_CODE_STR].colorB[0]);
			Draw24X24FStr(rect[RECT_FPROG_CODE_STR].xpis + (rect[RECT_FPROG_CODE_STR].xsize - strlen((char *)code) * 12) / 2,
						  rect[RECT_FPROG_CODE_STR].ypis + (rect[RECT_FPROG_CODE_STR].ysize - 24) / 2, (char *)code, rect[RECT_FPROG_CODE_STR].colorT[0]);
			ParaList.selectNoNow = getNoByCode(code);		 //获取字符完全是适配时的参数序号
			if (ParaList.selectNoNow > getParaNoLimit() - 1) //不完全适配时，获取最大适配程度参数序号
			{
				ParaList.selectNoNow = getAdjNoByCode(code, codelen);
			}
			ParaList.selectNoLast = ParaList.selectNoNow;

			ParaList.bFlashPageFlag = 1;
		}

		if (ParaList.bFlashPageFlag) //索引换页更新
		{
			U8 tmpIndex;
			ParaList.bFlashPageFlag = 0;

			ParaList.dataAdd[3] = ParaList.selectNoNow;
			if (ParaList.patternNum >= ParaList.disNumDiv)
			{
				if (ParaList.selectNoNow < ParaList.disNumDiv / 2)
				{
					U16 oneSide = ParaList.disNumDiv / 2 - ParaList.selectNoNow; //超出范围数量
					for (i = 0; i < oneSide; i++)								 //给左侧超过范围的值赋值
					{
						ParaList.dataAdd[oneSide - 1 - i] = ParaList.patternNum - 1 - i;
					}
					for (i = oneSide; i < ParaList.disNumDiv; i++) //给未超范围的值赋值
					{
						if (i == 3)
						{
							ParaList.dataAdd[3] = ParaList.selectNoNow;
						}
						else
						{
							ParaList.dataAdd[i] = i - oneSide;
						}
					}
				}
				else if (ParaList.selectNoNow >= (ParaList.patternNum - ParaList.disNumDiv / 2) && ParaList.selectNoNow < ParaList.patternNum)
				{
					U16 oneSide = (ParaList.patternNum - 1 - ParaList.selectNoNow); //未超出范围数量

					for (i = 4 + oneSide; i < ParaList.disNumDiv; i++) //给右侧超过范围的值赋值
					{
						ParaList.dataAdd[i] = i - (4 + oneSide);
					}
					for (i = 3 + oneSide;; i--) //给未超范围的值赋值
					{
						if (i == 3)
						{
							ParaList.dataAdd[3] = ParaList.selectNoNow;
						}
						else
						{
							ParaList.dataAdd[i] = ParaList.patternNum - 1 - (3 + oneSide - i);
							if (i == 0)
								break;
						}
					}
				}
				else
				{
					for (i = 1; i <= 3; i++)
					{
						ParaList.dataAdd[3 - i] = ParaList.dataAdd[3] - i;
						ParaList.dataAdd[3 + i] = ParaList.dataAdd[3] + i;
					}
				}
			}
			else
			{
				U8 tmpDn = 3;
				U8 tmpUp = 3;
				for (i = 1; i < ParaList.patternNum; i++)
				{
					tmpDn--;
					tmpUp++;
					if (ParaList.dataAdd[3] >= i)
					{

						ParaList.dataAdd[tmpDn] = ParaList.dataAdd[3] - i;
					}
					else
					{
						ParaList.dataAdd[tmpUp] = 0xFFFF;
					}
					if (ParaList.dataAdd[3] + i <= ParaList.patternNum)
					{

						ParaList.dataAdd[tmpUp] = ParaList.dataAdd[3] + i;
					}
					else
					{
						ParaList.dataAdd[tmpUp] = 0xFFFF;
					}
				}
			}

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				tmpIndex = RECT_FPROG_SEL1 + i;
				DrawFillRect(rect[tmpIndex].xpis, rect[tmpIndex].ypis,
							 rect[tmpIndex].xsize, rect[tmpIndex].ysize, rect[tmpIndex].colorB[0]);
			}
			DrawDottedRect(rect[RECT_FPROG_SEL4].xpis + 1, rect[RECT_FPROG_SEL4].ypis + 1,
						   rect[RECT_FPROG_SEL4].xsize - 2, rect[RECT_FPROG_SEL4].ysize - 2, rect[RECT_FPROG_SEL4].colorT[0]);

			for (i = 0; i < ParaList.disNumDiv; i++)
			{
				if (ParaList.dataAdd[i] != 0xFFFF)
				{
					tmpIndex = RECT_FPROG_SEL1 + i;
					disProgCode(rect[tmpIndex].xpis + SAFE_DIST4, rect[tmpIndex].ypis + SAFE_DIST4,
								rect[tmpIndex].xsize, rect[tmpIndex].ysize, LEFT,
								(i == 3) ? STR24X12 : STR16X8, screenList[ParaList.dataAdd[i]], rect[tmpIndex].colorT[0]);
				}
			}

			BmpFillRect(rect[RECT_FPROG_NAME_STR].xpis, rect[RECT_FPROG_NAME_STR].ypis, rect[RECT_FPROG_NAME_STR].xsize, rect[RECT_FPROG_NAME_STR].ysize);
			DrawRect(rect[RECT_FPROG_NAME_STR].xpis, rect[RECT_FPROG_NAME_STR].ypis,
					 rect[RECT_FPROG_NAME_STR].xsize, rect[RECT_FPROG_NAME_STR].ysize, rect[RECT_FPROG_NAME_STR].colorB[0]);
			if (ParaList.dataAdd[3] != 0xFFFF)
			{
				disProgDesc(rect[RECT_FPROG_NAME_STR].xpis + SAFE_DIST4, rect[RECT_FPROG_NAME_STR].ypis + SAFE_DIST4,
							screenList[ParaList.dataAdd[3]], 38, 5, rect[RECT_FPROG_NAME_STR].colorT[0]);
			}

			if (ParaList.selectNoNow != ParaList.selectNoLast)
			{
				ParaList.selectNoLast = ParaList.selectNoNow;
				codelen = getCodeByNo(code, screenList[ParaList.selectNoNow]);
				DrawEdit(rect[RECT_FPROG_CODE_STR], rect[RECT_FPROG_CODE_STR].colorT[0], rect[RECT_FPROG_CODE_STR].colorB[0]);
				Draw24X24FStr(rect[RECT_FPROG_CODE_STR].xpis + (rect[RECT_FPROG_CODE_STR].xsize - strlen((char *)code) * 12) / 2,
							  rect[RECT_FPROG_CODE_STR].ypis + (rect[RECT_FPROG_CODE_STR].ysize - 24) / 2, (char *)code, rect[RECT_FPROG_CODE_STR].colorT[0]);
			}
			wdt();
		}
		// /[更新参数列表数据*end]**********************************************************

		if (beeponf == 0)
		{

			if (Timer_ms >= 30000)
			{
				Timer_ms = 0;
			}
		}
		updateViewStatus();
		wdt();
	}
	return screenList[ParaList.selectNoNow];
}
