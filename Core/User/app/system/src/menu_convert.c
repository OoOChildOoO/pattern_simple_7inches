//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  convert.c								*
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

#include "menu_convert.h"
#include "menu_modify.h"
#include "menu_create.h"
#include "main.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "function.h"
#include "uart.h"
#include "color.h"
#include "communication.h"
#include "Patfun.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"
#include "function.h"
#include "viewPattern.h"

extern U16 SheetPage; //多国语言Sheet表页码，从0开始
extern volatile U32 g_temp_pno;
extern MachineInfo g_Mac;
extern volatile U16 g_savetimes;
extern volatile U16 g_Tsavetimes;
extern volatile S16 g_dxpos, g_dypos;
extern volatile S16 g_tmpoxpos, g_tmpoypos;
extern volatile U8 g_para_offset;
extern volatile U8 g_modifyflag;
extern volatile U32 g_jog_point;
extern volatile U8 g_selectmode;
extern volatile U8 g_num_mov_flag;
extern volatile U8 g_direction; //多重缝方向
extern volatile U8 g_distance;  //多重缝距离
extern volatile U8 g_times;     //多重缝次数
extern volatile U16 g_parameter;
extern volatile U16 g_point_num;
extern volatile U8 g_reorigin;
extern volatile U16 g_array4[386][2];
extern volatile U16 g_array5[630][2];
extern volatile U32 g_list_nadd;
extern volatile U32 g_modaddr[30];
extern volatile U8 g_endtype;
extern volatile U8 g_UdZhouFlag;

extern volatile U16 g_convert_xscale;
extern volatile U16 g_convert_yscale;
extern volatile U16 g_convert_pindis;
extern volatile U8 g_datum_mark;
extern volatile S16 g_tdxpos, g_tdypos;
extern volatile U16 g_curPTtype;
extern volatile S16 g_maxX;
extern volatile S16 g_minX;
extern volatile S16 g_maxY;
extern volatile S16 g_minY;

extern SEW_CHECK g_sewcheck;
extern volatile U8 g_zhouDown;
//extern volatile U32 g_list_oadd;
//extern volatile U32 g_list_nadd;
//extern volatile U32 g_modaddr[30];

volatile U8 g_convert_mode;
volatile U8 g_convert_orgf; //原点偏移变换

volatile U16 g_tSREG3;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos;

/**************************************************/
#define tmp_convert_mode g_publicbuf[61]

#define KEY_CONVERT_SCRNSVR 1 //1 屏保
#define KEY_CONVERT_BACK 2    //2 返回
#define KEY_CONVERT_ENTER 3   //3 确认
#define KEY_CONVERT_SKIPJOG 4 //4 跳针方法
#define KEY_CONVERT_UNDO 5    //5 撤销
#define KEY_CONVERT_REDO 6    //6 重做

#define KEY_CONVERT_ORGSUB 7    //7 原点修正
#define KEY_CONVERT_TACKING 8   //8 前后倒缝
#define KEY_CONVERT_ZIG 9       //9 人字缝
#define KEY_CONVERT_SCALE 10    //10 缩放
#define KEY_CONVERT_MIRROR 11   //11 镜像
#define KEY_CONVERT_ROTATE 12   //12 旋转
#define KEY_CONVERT_OFFSET 13   //13 离边缝
#define KEY_CONVERT_OFFSMULT 14 //14 多重缝
#define KEY_CONVERT_CHANGE 15   //15 首位互换
#define KEY_CONVERT_PINANG 16   //16 针角度调整

#define KEY_CONVERT_MAGETOUCH 17 //17 移动图形
#define KEY_CONVERT_MAX 18

#define RECT_CONVERT_METHOD 1    //1 方法
#define RECT_CONVERT_XM_TIT 2    //2 X轴最大-标题
#define RECT_CONVERT_XM_STR 3    //3 X轴最大-内容
#define RECT_CONVERT_YM_TIT 4    //4 Y轴最大-标题
#define RECT_CONVERT_YM_STR 5    //5 Y轴最大-内容
#define RECT_CONVERT_XI_TIT 6    //6 X轴最小-标题
#define RECT_CONVERT_XI_STR 7    //7 X轴最小-内容
#define RECT_CONVERT_YI_TIT 8    //8 Y轴最小-标题
#define RECT_CONVERT_YI_STR 9    //9 Y轴最小-内容
#define RECT_CONVERT_PINS_TIT 10 //10 针数-标题
#define RECT_CONVERT_PINS_STR 11 //11 针数-内容

#define RECT_CONVERT_MAX 12

void Menu_Convert(void)
{

    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息

    /****按键相关参数end****/

    /****标志位相关参数begin****/
    U8 dis_updata = 1,   //dis_updata:方法修改
        dis_updata1 = 1, //dis_updata1:刷新花样显示
        dis_updata2 = 1; //dis_updata2:刷新花样信息
    U32 i;
    //	U32 dat;
    //	U8 xx,yy;
    U8 f;
    S16 x, y;
    S16 tmp_PfootOffVal;
    S8 pfootval;
    U16 Borcmd;
    S8 Borpooft, Borpfdis;
    S16 tmpx = 0, tmpy = 0, dx = 0, dy = 0;
    U8 speed;
    U8 enMethod[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    U8 *enCHANGE, *enPINANG;
    U8 *enORGSUB, *enZIG;

    /****标志位相关参数end***/

    const U8 keynum = {KEY_CONVERT_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 确认

        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //4 跳针设置
        {TYPE_X8, TYPE_Y4, KEY_X1, KEY_Y1},  //5 取消
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},  //6 重做

        {45, 68, KEY_X6, KEY_Y6},                              //7 原点修正
        {45 + (KEY_X6 + SAFE_DIST10), 68, KEY_X6, KEY_Y6},     //8 前后倒缝
        {45 + (KEY_X6 + SAFE_DIST10) * 2, 68, KEY_X6, KEY_Y6}, //9 人字缝
        {45 + (KEY_X6 + SAFE_DIST10) * 3, 68, KEY_X6, KEY_Y6}, //10 缩放

        {45, 68 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6},                              //11 镜像
        {45 + (KEY_X6 + SAFE_DIST10), 68 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6},     //12 旋转
        {45 + (KEY_X6 + SAFE_DIST10) * 2, 68 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6}, //13 离边缝
        {45 + (KEY_X6 + SAFE_DIST10) * 3, 68 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6}, //14 多重缝

        {45, 68 + (KEY_Y6 + SAFE_DIST8) * 2, KEY_X6, KEY_Y6},                          //15 首位互换
        {45 + (KEY_X6 + SAFE_DIST10), 68 + (KEY_Y6 + SAFE_DIST8) * 2, KEY_X6, KEY_Y6}, //16 针角度调整

        {CONVAS_PX6, CONVAS_PY6, CONVAS_X6, CONVAS_Y6}, //17 移动图形
    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {1}},                          //1 屏保
        {{KEY_Silver, KEY_Gray}, {0}, {241, 242, 72, 88}, {2, 0, 0, 0}, {0}}, //2 返回
        {{KEY_Silver, KEY_Gray}, {0}, {71, 87}, {3, 0, 0, 0}, {0}},           //3 确认
        {{KEY_Silver, KEY_Gray}, {0}, {509, 0}, {4, 0, 0, 0}, {0}},           //4 跳针设置
        {{KEY_Silver, KEY_Gray}, {0}, {661, 0}, {5, 0, 0, 0}, {23}},          //5 取消
        {{KEY_Silver, KEY_Gray}, {0}, {662, 0}, {6, 0, 0, 0}, {24}},          //6 重做

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {591, 593, 592}, {7, 0, 0, 0}, {7}},   //7 原点修正
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {460, 462, 461}, {8, 0, 0, 0}, {8}},   //8 前后倒缝
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {445, 447, 446}, {9, 0, 0, 0}, {9}},   //9 人字缝
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {601, 0, 602}, {10, 0, 0, 0}, {10}},   //10 缩放
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {604, 0, 605}, {11, 0, 0, 0}, {11}},   //11 镜像
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {607, 0, 608}, {12, 0, 0, 0}, {12}},   //12 旋转
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {428, 430, 429}, {13, 0, 0, 0}, {13}}, //13 离边缝
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {422, 424, 423}, {14, 0, 0, 0}, {14}}, //14 多重缝
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {609, 611, 610}, {15, 0, 0, 0}, {15}}, //15 首位互换
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {612, 614, 613}, {16, 0, 0, 0}, {16}}, //16 针角度调整

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {CONVAS_COLOR_B, CONVAS_COLOR_T}, {0, 0, 0, 0}, {0}}, //17 移动图形

    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X35, TYPE_Y2, KEY_X6 * 4 + SAFE_DIST7 * 8, TYPE_SIZE31, {Color_Black}, {Color_Black}, {25}}, //1 方法

        {CONVAS_PX6, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {18}},                                                  //2 X轴最大-标题
        {CONVAS_PX6 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                           //3 X轴最大-内容
        {510, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {20}},                                                         //4 Y轴最大-标题
        {510 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                  //5 Y轴最大-内容
        {CONVAS_PX6, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {19}},                        //6 X轴最小-标题
        {CONVAS_PX6 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //7 X轴最小-内容
        {510, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {21}},                               //8 Y轴最小-标题
        {510 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},        //9 Y轴最小-内容
        {630, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {22}},                                                         //10 针数-标题
        {630 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                  //11 针数-内容

    };

    enCHANGE = &(enMethod[8]);
    enPINANG = &(enMethod[9]);
    enORGSUB = &(enMethod[0]);
    enZIG = &(enMethod[2]);
    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_CONVERT); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_CONVERT_MAIN);

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************
    if (F_FILETYPE == PAT_MIT)
    { //禁用首尾交换
        *enCHANGE = 0;
        *enORGSUB = 0;
    }
    else
    {
        *enORGSUB = 0;
        *enZIG = 0;
    }
    if ((g_UdZhouFlag != 1) && (g_UdZhouFlag != 2))
    { //禁用针角度
        *enPINANG = 0;
    }

    if ((plastno != MENU_SCRNSVR) && (plastno != MENU_SKIPJOG))
    {
        if (g_convert_mode > 9)
        {
            g_convert_mode = 0;
        }
        if ((g_convert_mode == 8 && *enCHANGE == 0) || (g_convert_mode == 9 && *enPINANG == 0))
        {
            g_convert_mode = 0;
        }
        if (g_convert_mode == 0 && *enORGSUB == 0)
        {
            g_convert_mode = 1;
        }
        tmp_convert_mode = g_convert_mode;
    }
    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                 SheetPage, rect[0].index[0], rect[0].colorT[0]); //花样设计

    //画出方框显示
    for (i = 1; i < RECT_CONVERT_MAX; i++)
    {

        if (i == RECT_CONVERT_METHOD)
        {
            DrawRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);

            Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST3,
                         MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
                         SheetPage, rect[i].index[0], rect[i].colorT[0]); //画出方框的文字
        }
    }
    DrawFillRect(keyd[KEY_CONVERT_MAGETOUCH].xpis, keyd[KEY_CONVERT_MAGETOUCH].ypis,
                 keyd[KEY_CONVERT_MAGETOUCH].xsize, keyd[KEY_CONVERT_MAGETOUCH].ysize, keybmpind[KEY_CONVERT_MAGETOUCH].icon[0]);
    //画出按键显示
    for (i = 2; i <= KEY_CONVERT_REDO; i++)
    {
        if ((i == KEY_CONVERT_UNDO) && (g_savetimes == 0)) //无效按键
        {
        }
        else if ((i == KEY_CONVERT_REDO) && (g_savetimes >= g_Tsavetimes))
        {
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************
    ViewPage(usepageno); //设置显示缓存

    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值

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
            if ((keydata == KEY_CONVERT_UNDO) && (g_savetimes == 0))
            {
                keydata = 0;
            }
            else if ((keydata == KEY_CONVERT_REDO) && (g_savetimes >= g_Tsavetimes))
            {
                keydata = 0;
            }
            else if (((keydata == KEY_CONVERT_ORGSUB) || (keydata == KEY_CONVERT_ZIG)) && (F_FILETYPE == PAT_SEW))
            {
                keydata = 0;
            }
            else if ((keydata == KEY_CONVERT_CHANGE && *enCHANGE == 0) || (keydata == KEY_CONVERT_PINANG && *enPINANG == 0) ||
                     (keydata == KEY_CONVERT_ORGSUB && *enORGSUB == 0))
            {
                keydata = 0;
            }
            else if (keydata == KEY_CONVERT_MAGETOUCH)
            {
                keydata = 0;
            }
            else
            {

                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
                {
                    keyon = keydata;
                    if (keyon == KEY_CONVERT_SCRNSVR) //无按键特效
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
        else //按键没有按下
        {
            key_on_flag1 = 0;
        }

        // /[按键键值返回预处理*end]**********************************************************

        // /[按键松开键值处理*begin]**********************************************************
        if (keyoff)
        {
            if ((keyoff >= KEY_CONVERT_ORGSUB) && (keyoff <= KEY_CONVERT_PINANG))
            {
                tmp_convert_mode = keyoff - KEY_CONVERT_ORGSUB;
                dis_updata = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_CONVERT_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_CONVERT_BACK:
                    g_dxpos = 0;
                    g_dypos = 0;
                    g_para_offset = 0;
                    pno = MENU_INHELP;
                    hleppara = 0;
                    poldno[pnoindex] = MENU_CONVERT;
                    pnoindex++;

                    if (F_FILETYPE == PAT_SEW)
                    {
                        if (g_savetimes == 0)
                            g_modifyflag = 0;
                        //							if(modifyflag !=0)  gPatModflag = 1;	   //盖写原来花样
                    }
                    else
                    {
                        if ((g_savetimes == 0) && (g_convert_orgf == 0))
                            g_modifyflag = 0;
                        //							 if(modifyflag !=0)  gPatModflag = 1;	   //盖写原来花样
                    }
                    g_endtype = 3;
                    break;

                case KEY_CONVERT_ENTER:
                    g_convert_mode = tmp_convert_mode;
                    if ((g_convert_mode == 0) && (F_FILETYPE == PAT_MIT)) //原点修正  兄弟款暂时不支持原点修正功能
                    {
                        g_para1 = g_convert_mode;
                        pno = MENU_CONVERT1;
                        if (pathTohome(MENU_CONVERT1) == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        DREG[D_COMMAND] = C_GOHOME_PRESS_ONT;
                        DREG[D_CMDSTATUS] = 0;
                        f = Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));

                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else
                        {
                            Read_Status();
                        }
                        x = (S16)(SREG[6]);
                        y = (S16)(SREG[7]);
                        f = Comm_Move(x, y, 50, MENU_CONVERT, 0, 0);
                        if (f)
                        {
                            g_jog_point = 0;
                        }
                        if (SREG[3] == 7)
                        {
                            g_selectmode = 1;
                        }
                        else
                        {
                            g_selectmode = 0;
                        }
                        g_dxpos = (S16)(SREG[6]);
                        g_dypos = (S16)(SREG[7]);

#if DIS_MODE == 1
                        g_tSREG3 = SREG[3];
                        SREG[3] = 6;
#endif
                    }
                    else if ((g_convert_mode == 1) || (g_convert_mode == 2)) //倒缝 / 人字缝
                    {
                        if ((F_FILETYPE == PAT_SEW) && (g_convert_mode == 1)) //兄弟款不支持人字缝变换
                        {
                            g_para1 = 26;
                        }
                        else
                        {
                            g_para1 = g_convert_mode + 25;
                        }
                        pno = MENU_MODIFYADD;
                    }
                    else if (g_convert_mode == 3) //放大、缩小
                    {
                        g_para1 = g_convert_mode;
                        g_convert_xscale = 1000;
                        g_convert_yscale = 1000;
                        if (F_FILETYPE == PAT_SEW)
                            g_convert_pindis = 30;
                        g_num_mov_flag = 2;
                        pno = MENU_CONVERT1;
                        g_dxpos = 0;
                        g_dypos = 0;
                    }
                    else if (g_convert_mode == 4) //图形对称
                    {
                        g_para1 = g_convert_mode;
                        g_num_mov_flag = 0;
                        pno = MENU_CONVERT1;
                        g_dxpos = 0;
                        g_dypos = 0;
                    }
                    else if (g_convert_mode == 5) //图形选转
                    {
                        g_para1 = g_convert_mode;
                        g_num_mov_flag = 2;
                        pno = MENU_CONVERT1;
                        g_dxpos = 0;
                        g_dypos = 0;
                    }
                    else if ((g_convert_mode == 6) || (g_convert_mode == 7)) //辅助缝、多重逢
                    {
                        g_para1 = g_convert_mode;
                        if (F_FILETYPE == PAT_SEW)
                            g_convert_pindis = 30;
                        g_num_mov_flag = 0;
                        pno = MENU_CONVERT1;
                        g_dxpos = 0;
                        g_dypos = 0;
                    }
                    else if (g_convert_mode == 8 && F_FILETYPE == PAT_SEW) //首尾变换 仅兄弟格式
                    {
                        //							g_para1 = g_convert_mode;
                        //							g_num_mov_flag = 0;
                        //							pno = MENU_CONVERT1;
                        g_para1 = g_convert_mode + 25;
                        pno = MENU_MODIFYADD;
                    }
                    else if (g_convert_mode == 9) //针角度
                    {
                        g_para1 = g_convert_mode + 25;
                        pno = MENU_MODIFYADD;
                    }
                    poldno[pnoindex] = MENU_CONVERT;
                    pnoindex++;
                    Wait_Move_End(); //等待平台移动结束

                    g_maxX = g_patd.patmX;
                    g_minX = g_patd.patiX;
                    g_maxY = g_patd.patmY;
                    g_minY = g_patd.patiY;
                    break;
                case KEY_CONVERT_SKIPJOG:
                    pno = MENU_SKIPJOG;
                    poldno[pnoindex] = MENU_CONVERT;
                    pnoindex++;
                    break;
                case KEY_CONVERT_UNDO:
                    if (F_FILETYPE == PAT_MIT)
                    { //三菱
                        g_savetimes--;
                        g_list_nadd = Mit_ReadSregData(g_list_nadd);
                        g_reorigin = 0;
                        //RecovrSREG(g_savetimes,0);
                        Mit_CreateAllData();

                        //printf("g_patd.feed_sum = %d  g_jog_point = %d \r\n",g_patd.feed_sum,g_jog_point);
                        AnaPattern(0, PAT_MIT, 0);
                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        //							g_tmpoxpos = 0;g_tmpoypos = 0;
                        //							for(ii=0;ii<g_jog_point;ii++){
                        //								dat = SREG[g_patd.pat_add+ii*2+1];
                        //								xx = dat&0x000000ff;
                        //								yy = ((U32)(dat>>8))&0x000000ff;
                        //								if(xx>=0x80){xx-=0x80;g_tmpoxpos-=xx;}else{g_tmpoxpos+=xx;}
                        //								if(yy>=0x80){yy-=0x80;g_tmpoypos-=yy;}else{g_tmpoypos+=yy;}
                        //							}
                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);

                        f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

                        if (f == 0)
                        { //读设定中压脚修正量
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_CONVERT;
                            pnoindex++;
                            return;
                        }
                        tmp_PfootOffVal = YREG[SYS_PFOOTSET];
                        pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //计算jog_point之前的中压脚高度变化量
                        tmp_PfootOffVal += pfootval;
                        Send_Pfoot(1, MENU_CONVERT, 1); //压脚升
                        //printf("g_patd.feed_sum = %d  g_jog_point = %d g_tmpoxpos = %d g_tmpoypos = %d g_patd.pat_add = %d \r\n",g_patd.feed_sum,g_jog_point,g_tmpoxpos,g_tmpoypos,g_patd.pat_add);
                        //Go_Home(MENU_CONVERT,0);
                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT, 0, 0); //移动到当前坐标
                        Wait_Move_End();                                           //等待平台移动结束
                        f = Write_Cmd(C_PFOOTACTION, tmp_PfootOffVal, 0);
                        Read_Status();
                        //for(ii=0;ii<RDTIMES;ii++){
                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));		// 读状态
                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                        //	DelayMs(WAITTIME);
                        //}
                        dis_updata1 = 1;
                        dis_updata2 = 1;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //兄弟
                        g_savetimes--;
                        RecovrSREG(g_savetimes, 1);
                        //g_jog_point = 0;
                        //Go_Home(MENU_CONVERT,0);
                        AnaPattern(0, PAT_SEW, (F_FILELEN));
                        Send_Pfoot(1, MENU_CONVERT, 1); //压脚升
                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &tmpx, &tmpy, &dx, &dy, &speed, &Borpooft, &Borpfdis);
                        Comm_Move(tmpx + dx, tmpy + dy, 50, MENU_CONVERT, 1, 1);
                        Wait_Move_End(); //等待平台移动结束

                        dis_updata1 = 1;
                        dis_updata2 = 1;
                    }
                    break;

                case KEY_CONVERT_REDO:
                    if (F_FILETYPE == PAT_MIT)
                    { //三菱
                        g_savetimes++;
                        g_reorigin = 0;

                        if (g_savetimes < g_Tsavetimes)
                        {
                            g_list_nadd = g_modaddr[g_savetimes];
                            g_list_nadd = Mit_ReadSregData(g_list_nadd);
                        }
                        else
                        {
                            //break;
                            g_list_nadd = g_modaddr[g_savetimes - 1];
                            mitRecovrSREG();
                        }
                        //							g_savetimes++;
                        //							g_reorigin = 0;
                        //							RecovrSREG(g_savetimes,0);
                        Mit_CreateAllData();

                        //
                        //							g_jog_point = 0;
                        //							Go_Home(MENU_CONVERT,0);
                        AnaPattern(0, PAT_MIT, 0);
                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        //							g_tmpoxpos = 0;g_tmpoypos = 0;
                        //							for(ii=0;ii<g_jog_point;ii++){
                        //								dat = SREG[g_patd.pat_add+ii*2+1];
                        //								xx = dat&0x000000ff;
                        //								yy = ((U32)(dat>>8))&0x000000ff;
                        //								if(xx>=0x80){xx-=0x80;g_tmpoxpos-=xx;}else{g_tmpoxpos+=xx;}
                        //								if(yy>=0x80){yy-=0x80;g_tmpoypos-=yy;}else{g_tmpoypos+=yy;}
                        //							}
                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                        f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

                        if (f == 0)
                        { //读设定中压脚修正量
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_CONVERT;
                            pnoindex++;
                            return;
                        }
                        tmp_PfootOffVal = YREG[SYS_PFOOTSET];
                        pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //计算jog_point之前的中压脚高度变化量
                        tmp_PfootOffVal += pfootval;
                        Send_Pfoot(1, MENU_CONVERT, 1); //压脚升
                        //printf("g_patd.feed_sum = %d  g_jog_point = %d g_tmpoxpos = %d g_tmpoypos = %d  g_patd.pat_add = %d\r\n",g_patd.feed_sum,g_jog_point,g_tmpoxpos,g_tmpoypos,g_patd.pat_add);
                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT, 0, 0); //移动到当前坐标
                        Wait_Move_End();                                           //等待平台移动结束

                        f = Write_Cmd(C_PFOOTACTION, tmp_PfootOffVal, 0);
                        Read_Status();
                        //for(ii=0;ii<RDTIMES;ii++){
                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));		// 读状态
                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                        //	DelayMs(WAITTIME);
                        //}
                        dis_updata1 = 1;
                        dis_updata2 = 1;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //兄弟
                        g_savetimes++;
                        RecovrSREG(g_savetimes, 1);
                        //							g_jog_point = 0;
                        //							Go_Home(MENU_CONVERT,0);
                        AnaPattern(0, PAT_SEW, (F_FILELEN));
                        Send_Pfoot(1, MENU_CONVERT, 1); //压脚升
                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &tmpx, &tmpy, &dx, &dy, &speed, &Borpooft, &Borpfdis);
                        Comm_Move(tmpx + dx, tmpy + dy, 50, MENU_CONVERT, 1, 1);
                        Wait_Move_End(); //等待平台移动结束

                        dis_updata1 = 1;
                        dis_updata2 = 1;
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

            if (keyon == KEY_CONVERT_SCRNSVR) //不需要显示的按键
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
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if (Timerms > 1000)
        {
            Timerms = 0;
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_CONVERT);
        }
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_CONVERT);
        }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************
        // /[方式按钮刷新*begin]**********************************************************

        // /[方式按钮刷新*begin]**********************************************************
        if (dis_updata)
        {
            dis_updata = 0;
            f = KEY_CONVERT_ORGSUB + tmp_convert_mode;
            for (i = KEY_CONVERT_ORGSUB; i <= KEY_CONVERT_PINANG; i++)
            {
                if (enMethod[i - KEY_CONVERT_ORGSUB] == 0)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[1], keybmpind[i].icon[1]);
                }
                else
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (i == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (i == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                }
                Draw16x16Str(keyd[i].xpis - SAFE_DIST7 +
                                 (keyd[i].xsize + SAFE_DIST10 - 8 * strlen((char *)(MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax))) / 2,
                             keyd[i].ypis + keyd[i].ysize + SAFE_DIST3,
                             MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax,
                             SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[0]); //画出方框的文字
            }
        }
        // /[方式按钮刷新*end]**********************************************************

        // /[花样显示刷新*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;

            AnaPattern(0, F_FILETYPE, (F_FILELEN));
            // DrawFillRect(keyd[KEY_CONVERT_MAGETOUCH].xpis, keyd[KEY_CONVERT_MAGETOUCH].ypis,
            //              keyd[KEY_CONVERT_MAGETOUCH].xsize, keyd[KEY_CONVERT_MAGETOUCH].ysize, keybmpind[KEY_CONVERT_MAGETOUCH].icon[0]);
            // Draw_Pattern(PAT_CONVENT, 0, CONVAS_ORG_PX6, CONVAS_ORG_PY6, F_FILETYPE, (F_FILELEN), 0, 0, NULL); // 显示花样
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(PATRULES_CONVERT_MAIN, 0, patConvas.conOrgX, patConvas.conOrgY, F_FILETYPE, F_FILELEN, 0, 0, NULL);

            if (g_savetimes == 0)
            {
                BmpFillRect(keyd[KEY_CONVERT_UNDO].xpis, keyd[KEY_CONVERT_UNDO].ypis,
                            keyd[KEY_CONVERT_UNDO].xsize, keyd[KEY_CONVERT_UNDO].ysize);
            }
            else
            {
                DrawButton(keyd[KEY_CONVERT_UNDO], (U16 *)(ADD_ICONBMP), keybmpind[KEY_CONVERT_UNDO].border[0], keybmpind[KEY_CONVERT_UNDO].icon[0]);
            }
            if ((F_FILETYPE == PAT_MIT) ? g_savetimes >= g_Tsavetimes : g_savetimes >= g_Tsavetimes)
            {
                BmpFillRect(keyd[KEY_CONVERT_REDO].xpis, keyd[KEY_CONVERT_REDO].ypis,
                            keyd[KEY_CONVERT_REDO].xsize, keyd[KEY_CONVERT_REDO].ysize);
            }
            else
            {
                DrawButton(keyd[KEY_CONVERT_REDO], (U16 *)(ADD_ICONBMP), keybmpind[KEY_CONVERT_REDO].border[0], keybmpind[KEY_CONVERT_REDO].icon[0]);
            }
#if EN_SEWCHECK
            if (F_FILETYPE == PAT_SEW)
            {
                if (sew_Patcheck((U16 *)SREG, F_FILELEN) == 0)
                {
                    BmpFillRect(448, 560, 80, 80);
                    Draw24X24FStr(448, 560, "Error", Color_Red);
                    DrawFHEX(526, 560, g_sewcheck.flag, 2, Color_Blue);
                    DrawMFU32Z(448, 588, g_sewcheck.len, 6, Color_Blue);
                    DrawMFU32Z(448, 616, g_sewcheck.len1, 6, Color_Red);
                }
            }
#endif
        }
        // /[花样显示刷新*end]**********************************************************

        if (dis_updata2)
        {
            dis_updata2 = 0;
            //画出方框显示
            for (i = RECT_CONVERT_XM_TIT; i <= RECT_CONVERT_PINS_STR; i++)
            {
                if (i == RECT_CONVERT_XM_TIT || i == RECT_CONVERT_YM_TIT || i == RECT_CONVERT_XI_TIT ||
                    i == RECT_CONVERT_YI_TIT || i == RECT_CONVERT_PINS_TIT)
                {
                    DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    Draw16x16Str(rect[i].xpis + (rect[i].xsize - 8 * strlen((char *)(MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax))) / 2,
                                 rect[i].ypis + (rect[i].ysize - 16) / 2,
                                 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
                                 SheetPage, rect[i].index[0], rect[i].colorT[0]); //画出方框的文字
                }
                else
                {
                    DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                }
            }
            DrawFS32P(rect[RECT_CONVERT_XM_STR].xpis + SAFE_DIST4,
                      rect[RECT_CONVERT_XM_STR].ypis + SAFE_DIST4,
                      g_patd.patmX, 5, 1, rect[RECT_CONVERT_XM_STR].colorT[0]);
            DrawFS32P(rect[RECT_CONVERT_XI_STR].xpis + SAFE_DIST4,
                      rect[RECT_CONVERT_XI_STR].ypis + SAFE_DIST4,
                      g_patd.patiX, 5, 1, rect[RECT_CONVERT_XI_STR].colorT[0]);
            DrawFS32P(rect[RECT_CONVERT_YM_STR].xpis + SAFE_DIST4,
                      rect[RECT_CONVERT_YM_STR].ypis + SAFE_DIST4,
                      g_patd.patmY, 5, 1, rect[RECT_CONVERT_YM_STR].colorT[0]);
            DrawFS32P(rect[RECT_CONVERT_YI_STR].xpis + SAFE_DIST4,
                      rect[RECT_CONVERT_YI_STR].ypis + SAFE_DIST4,
                      g_patd.patiY, 5, 1, rect[RECT_CONVERT_YI_STR].colorT[0]);

            DrawFU32(rect[RECT_CONVERT_PINS_STR].xpis + SAFE_DIST4,
                     rect[RECT_CONVERT_PINS_STR].ypis + (rect[RECT_CONVERT_PINS_STR].ysize - 16) / 2,
                     g_patd.feed_sum, 5, rect[RECT_CONVERT_PINS_STR].colorT[0]);
        }

        // /[记录打板开始时中压脚高度	*begin]**********************************************************
        if (g_zhouDown)
        {
            g_zhouDown = 0;
            Write_Cmd(C_LDOWMPF, 0, 0); //记录打板开始时中压脚高度
            Read_Status();
        }
        // /[记录打板开始时中压脚高度	*end]**********************************************************

        // /[页面切换*begin]**********************************************************
        if (pno != MENU_CONVERT)
        {
            plastno = MENU_CONVERT;
            break;
        }
        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

/*******************************************************/
#define tmp_parameter g_publicbuf[61]
#define tmp_selectmode g_publicbuf[62]

#define TLFNADD_GENERAL 0
#define TLFNLEN_GENERAL 29
#define TLFNADD_ORGSUB (TLFNADD_GENERAL + TLFNLEN_GENERAL)
#define TLFNLEN_ORGSUB 9
#define TLFNADD_TACKING (TLFNADD_ORGSUB + TLFNLEN_ORGSUB)
#define TLFNLEN_TACKING 3
#define TLFNADD_ZIG (TLFNADD_TACKING + TLFNLEN_TACKING)
#define TLFNLEN_ZIG 3
#define TLFNADD_SCALE (TLFNADD_ZIG + TLFNLEN_ZIG)
#define TLFNLEN_SCALE 21
#define TLFNADD_MIRROR (TLFNADD_SCALE + TLFNLEN_SCALE)
#define TLFNLEN_MIRROR 10
#define TLFNADD_ROTATE (TLFNADD_MIRROR + TLFNLEN_MIRROR)
#define TLFNLEN_ROTATE 15
#define TLFNADD_OFFSET (TLFNADD_ROTATE + TLFNLEN_ROTATE)
#define TLFNLEN_OFFSET 14
#define TLFNADD_OFFSMULT (TLFNADD_OFFSET + TLFNLEN_OFFSET)
#define TLFNLEN_OFFSMULT 20
#define TLFNADD_CHANGE (TLFNADD_OFFSMULT + TLFNLEN_OFFSMULT)
#define TLFNLEN_CHANGET 6
#define TLFNADD_PINANG (TLFNADD_CHANGE + TLFNLEN_CHANGET)
#define TLFNLEN_PINANG 8

#define KEY_CONVERT1_SCRNSVR 1 //1 屏保
#define KEY_CONVERT1_BACK 2    //2 返回
#define KEY_CONVERT1_ENTER 3   //3 确认
#define KEY_CONVERT1_VIEW 4    //4 图像预览键
#define KEY_CONVERT1_PFOOT 5   //5 抬压脚
#define KEY_CONVERT1_CANCEL 6  //6 取消

#define KEY_CONVERT1_ZOUT 7 //7 缩小
#define KEY_CONVERT1_ZIN 8  //8 放大
#define KEY_CONVERT1_IMG 9  //9 画布移动
#define KEY_CONVERT1_TOP 10 //10 吸附点

#define KEY_CONVERT1_MUL 11  //11 左上
#define KEY_CONVERT1_MUM 12  //12 上
#define KEY_CONVERT1_MUR 13  //13 右上
#define KEY_CONVERT1_MML 14  //14 左
#define KEY_CONVERT1_MMSP 15 //15 压框移速
#define KEY_CONVERT1_MMR 16  //16 右
#define KEY_CONVERT1_MDL 17  //17 左下
#define KEY_CONVERT1_MDM 18  //18 下
#define KEY_CONVERT1_MDR 19  //19 右下

#define KEY_CONVERT1_DUP 20   //20 +1
#define KEY_CONVERT1_DDOWN 21 //21 -1
#define KEY_CONVERT1_0 22     //22 0
#define KEY_CONVERT1_1 23     //23 1
#define KEY_CONVERT1_2 24     //24 2
#define KEY_CONVERT1_3 25     //25 3
#define KEY_CONVERT1_4 26     //26 4
#define KEY_CONVERT1_5 27     //27 5
#define KEY_CONVERT1_6 28     //28 6
#define KEY_CONVERT1_7 29     //29 7
#define KEY_CONVERT1_8 30     //30 8
#define KEY_CONVERT1_9 31     //31 9

#define KEY_CONVERT1_0ORGSUBOF 32 //32 原点修正关闭
#define KEY_CONVERT1_0ORGSUBON 33 //33 原点修正开启

#define KEY_CONVERT1_3SSTITCH 34  //34 方法-针数固定
#define KEY_CONVERT1_3SPITCH 35   //35 方法-针距固定
#define KEY_CONVERT1_3XSCALE 36   //36 数据-X轴缩放倍率
#define KEY_CONVERT1_3YSCALE 37   //37 数据-Y轴缩放倍率
#define KEY_CONVERT1_3PITCH 38    //38 数据-针距设定
#define KEY_CONVERT1_3ANG 39      //39 数据 - 兄弟-角度修正
#define KEY_CONVERT1_3INPUTBS 40  //40 基准点-输入点
#define KEY_CONVERT1_3CENTERBS 41 //41 基准点-花样中心
#define KEY_CONVERT1_3HOMEBS 42   //42 基准点-原点

#define KEY_CONVERT1_4MIRRORDEL 43  //43 源数据-删除原图
#define KEY_CONVERT1_4MIRRORHOLD 44 //44 源数据-保留原图
#define KEY_CONVERT1_4XMIRROR 45    //45 方法-垂直对称
#define KEY_CONVERT1_4YMIRROR 46    //46 方法-水平对称
#define KEY_CONVERT1_4XYMIRROR 47   //47 方法-中心

#define KEY_CONVERT1_5LROTATE 48  //48 方向-左
#define KEY_CONVERT1_5RROTATE 49  //49 方向-右
#define KEY_CONVERT1_5INPUTBS 50  //50 基准点-输入点
#define KEY_CONVERT1_5CENTERBS 51 //51 基准点-花样中心
#define KEY_CONVERT1_5HOMEBS 52   //52 基准点-原点

#define KEY_CONVERT1_6PREV 53  //53 上一段
#define KEY_CONVERT1_6NEXT 54  //54 下一段
#define KEY_CONVERT1_6DIRL 55  //55 方向-左
#define KEY_CONVERT1_6DIRR 56  //56 方向-右
#define KEY_CONVERT1_6DIS 57   //57 数据-距离
#define KEY_CONVERT1_6PITCH 58 //58 数据-针距

#define KEY_CONVERT1_7PREV 59     //59 上一段
#define KEY_CONVERT1_7NEXT 60     //60 下一段
#define KEY_CONVERT1_7FDSYMULT 61 //61 方法-1
#define KEY_CONVERT1_7FDREMULT 62 //62 方法-2
#define KEY_CONVERT1_7SWSYMULT 63 //63 方法-3
#define KEY_CONVERT1_7SWREMULT 64 //64 方法-4
#define KEY_CONVERT1_7DIRL 65     //65 方向-左
#define KEY_CONVERT1_7DIRR 66     //66 方向-右
#define KEY_CONVERT1_7SELDIST 67  //67 数据-距离
#define KEY_CONVERT1_7SELTIME 68  //68 数据-次数
#define KEY_CONVERT1_7PITCH 69    //69 数据-针距

#define KEY_CONVERT1_8PREV 70 //70 上一段
#define KEY_CONVERT1_8NEXT 71 //71 下一段

#define KEY_CONVERT1_9ANG 72 //72 数据 - 兄弟-角度修正

#define RECT_CONVERT1_X_TIT 1
#define RECT_CONVERT1_X_STR 2
#define RECT_CONVERT1_Y_TIT 3
#define RECT_CONVERT1_Y_STR 4
#define RECT_CONVERT1_ST_TIT 5
#define RECT_CONVERT1_ST_STR 6
#define RECT_CONVERT1_EN_TIT 7
#define RECT_CONVERT1_EN_STR 8

#define RECT_CONVERT1_0METHOD 9 //原点修正-方法框

#define RECT_CONVERT1_3METHOD1 10   //缩放-方法框1
#define RECT_CONVERT1_3METHOD2 11   //缩放-方法框2
#define RECT_CONVERT1_3EDIT1_TIT 12 //缩放-数据框1
#define RECT_CONVERT1_3EDIT1_STR 13 //缩放-数据框1
#define RECT_CONVERT1_3EDIT2_TIT 14 //缩放-数据框2
#define RECT_CONVERT1_3EDIT2_STR 15 //缩放-数据框2
#define RECT_CONVERT1_3EDIT3_TIT 16 //缩放-数据框3
#define RECT_CONVERT1_3EDIT3_STR 17 //缩放-数据框3
#define RECT_CONVERT1_3EDIT4_TIT 18 //缩放-数据框4
#define RECT_CONVERT1_3EDIT4_STR 19 //缩放-数据框4

#define RECT_CONVERT1_4METHOD1 20 //镜像-方法框1
#define RECT_CONVERT1_4METHOD2 21 //镜像-方法框2

#define RECT_CONVERT1_5METHOD1 22   //旋转-方法框1
#define RECT_CONVERT1_5METHOD2 23   //旋转-方法框2
#define RECT_CONVERT1_5EDIT1_TIT 24 //旋转-数据框1
#define RECT_CONVERT1_5EDIT1_STR 25 //旋转-数据框1

#define RECT_CONVERT1_6EDIT1_TIT 26 //多重缝-方向
#define RECT_CONVERT1_6EDIT1_STR 27
#define RECT_CONVERT1_6EDIT1_BMP 28
#define RECT_CONVERT1_6EDIT2_TIT 29 //多重缝-方法
#define RECT_CONVERT1_6EDIT2_STR 30
#define RECT_CONVERT1_6EDIT2_BMP 31
#define RECT_CONVERT1_6EDIT3_TIT 32 //多重缝-距离
#define RECT_CONVERT1_6EDIT3_STR 33
#define RECT_CONVERT1_6EDIT4_TIT 34 //多重缝-次数
#define RECT_CONVERT1_6EDIT4_STR 35
#define RECT_CONVERT1_6EDITNO_TIT 36 //多重缝-编号
#define RECT_CONVERT1_6EDITNO_STR 37

#define RECT_CONVERT1_6METHOD 38  //离边缝-方法框
#define RECT_CONVERT1_6DIS_TIT 39 //离边缝-距离
#define RECT_CONVERT1_6DIS_STR 40
#define RECT_CONVERT1_6PITCH_TIT 41 //离边缝-针距
#define RECT_CONVERT1_6PITCH_STR 42
#define RECT_CONVERT1_6NO_TIT 43 //离边缝-编号
#define RECT_CONVERT1_6NO_STR 44

#define RECT_CONVERT1_7METHOD1 45  //多重缝-方法框1
#define RECT_CONVERT1_7METHOD2 46  //多重缝-方法框2
#define RECT_CONVERT1_7DIST_TIT 47 //多重缝-距离
#define RECT_CONVERT1_7DIST_STR 48
#define RECT_CONVERT1_7TIME_TIT 49 //多重缝-次数
#define RECT_CONVERT1_7TIME_STR 50
#define RECT_CONVERT1_7PITCH_TIT 51 //多重缝-针距
#define RECT_CONVERT1_7PITCH_STR 52
#define RECT_CONVERT1_7NO_TIT 53 //多重缝-编号
#define RECT_CONVERT1_7NO_STR 54

#define RECT_CONVERT1_8EDITNO_TIT 55 //首位变换-编号
#define RECT_CONVERT1_8EDITNO_STR 56

#define RECT_CONVERT1_9ANG_TIT 57 //针角度
#define RECT_CONVERT1_9ANG_STR 58

#define RECT_CONVERT1_HINT 59    //确认变换提示
#define RECT_CONVERT1_MESSAGE 60 //变换提示信息

void Menu_Convert1(U32 para)
{
#if 1
    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息

    /****按键相关参数end****/

    /****标志位相关参数begin****/
    U32 i;
    U8 flag = 0;         //获取段信息
    U8 dis_pos = 0;      //显示实际坐标
    U8 dis_updata = 1;   //界面显示用的UI初始化 0x01:所有显示刷新;0x02:方法1按钮刷新;0x04:方法2按钮刷新;0x08:edit按钮刷新;
                         //0x10:擦除全部;0x20:方法1擦除;0x40:方法2擦除;0x80:edit擦除
    U8 dis_updata1 = 1;  //界面显示用的数据初始化
    U8 dis_updata2 = 0;  //更新花样显示
    U8 dis_updata3 = 0;  //界面的edit数据刷新
    U8 dis_updata15 = 0; //刷新X,Y数据
    U16 pfootf = 0;
    S16 x, y;
    U16 patMin = 0, patMax = 0; //离边缝、多重缝段数
    S16 tmpxpos, tmpypos;
    U32 ii, dat; //n,,dat1
    U16 xx = 0, yy = 0;
    S16 mvxx, mvyy;
    U8 ll = 0, f = 0;
    U8 tmp_nummovflag = 0xff;
    U32 tmp_feedc = 0, tmp_feedsum = 0;
    U32 tmp_pat_add; //tmp_feedc,
    U16 tmp_draw_add = 0, tmp_line_add = 0;
    U8 deletedisf = 0; //取消键显示
    U8 pfootdisf = 0;  //压脚键显示
    U8 enterdisf = 0;  //确认键显示
    U8 patdisf = 0;    //花样显示
    U8 imgdisf = 0;    //图像预览键显示
    U16 Borcmd;
    S8 Borpooft, Borpfdis;
    S16 Convedis = 0;
    U8 speed;
    S16 dx = 0, dy = 0 /*, tdx = 0, tdy = 0*/;
    U16 ConAng = 30;
    S16 absposx = 0xffff, absposy = 0xffff;
    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    S16 nearpx, nearpy, onearpx = 0, onearpy = 0;
    U8 getNearflag = 0;
    U8 pressKey = 0;
    U8 tonear = 0, otonear = 0;

    S8 mult1 = 1, mult2 = 1, mult3 = 1, tmuly = 1;
    S32 scale = 0, scale1 = 0, vscale = 0;
    U8 bResetCaonvas = 0;
    S16 xorg = 0, yorg = 0;
    U16 ORG_X = CONVAS_ORG_PX9, ORG_Y = CONVAS_ORG_PY9;
    S16 xo = ORG_X;
    S16 yo = ORG_Y;
    S32 posx1, posy1;
    U8 vflag = 0;
    U16 tjog_point = g_jog_point;
    U16 title = 0;

    U8 editflag = 0, oldeditflag = 0; //输入框选中标志位
    U8 patRulesTmp = PATRULES_CONVERT1_MOVE;
    /****标志位相关参数end***/

    // const U8 keynum = {KEY_MODIADD_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 确认
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},  //4 图像预览
        {TYPE_X4, TYPE_Y4, KEY_X1, KEY_Y1},  //5 抬压脚
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //6 取消
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},  //7 缩小
        {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},  //8 放大

        {CONVAS_PX9, CONVAS_PY9, CONVAS_X9, CONVAS_Y9}, //9 画布移动
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},             //10 吸附点

        {TYPE_X36, TYPE_Y36, KEY_X1, KEY_Y1},                           //11 左上
        {TYPE_X36 + KEY_X1, TYPE_Y36, KEY_X1, KEY_Y1},                  //12 上
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36, KEY_X1, KEY_Y1},              //13 右上
        {TYPE_X36, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},                  //14 左
        {TYPE_X36 + KEY_X1, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},         //15 压框速度
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},     //16 右
        {TYPE_X36, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1},              //17 左下
        {TYPE_X36 + KEY_X1, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1},     //18 下
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1}, //19 右下

        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2}, //20 +1
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},       //21 -1
        {TYPE_X37, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},                             //22 0
        {TYPE_X37, TYPE_Y36, KEY_X2, KEY_Y2},                                                         //23 1
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36, KEY_X2, KEY_Y2},                                   //24 2
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36, KEY_X2, KEY_Y2},                             //25 3
        {TYPE_X37, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},                                   //26 4
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},             //27 5
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},       //28 6
        {TYPE_X37, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},                             //29 7
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},       //30 8
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2}, //31 9

        {TYPE_X5, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},          //32 原点修正关闭
        {TYPE_X5 + KEY_X6, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //33 原点修正开启

        {TYPE_X14 + SAFE_DIST1 + KEY_X6, TYPE_Y31, KEY_X6, KEY_Y6},                                          //34 方法-针数固定
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 4, TYPE_Y31, KEY_X6, KEY_Y6},                                      //35 方法-针距固定
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260, TYPE_SIZE14, TYPE_SIZE5},                                 //36 数据-X轴缩放倍率
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5},       //37 数据-Y轴缩放倍率
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5}, //38 数据-针距设定
        // {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 3, TYPE_SIZE14, TYPE_SIZE5}, //39 数据 - 兄弟-角度修正
        {0, 0, 0, 0},                                                       //39 数据 - 兄弟-角度修正
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 1 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //40 基准点-输入点
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //41 基准点-花样中心
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 9 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //42 基准点-原点

        {TYPE_X14 + SAFE_DIST1 + KEY_X6, TYPE_Y31, KEY_X6, KEY_Y6},         //43 源数据-删除原图
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 4, TYPE_Y31, KEY_X6, KEY_Y6},     //44 源数据-保留原图
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 1 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //45 方法-垂直对称
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //46 方法-水平对称
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 9 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //47 方法-中心

        {TYPE_X14 + SAFE_DIST1 + KEY_X6, TYPE_Y31, KEY_X6, KEY_Y6},         //48 方向-左
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 4, TYPE_Y31, KEY_X6, KEY_Y6},     //49 方向-右
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 1 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //50 基准点-输入点
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //51 基准点-花样中心
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 9 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //52 基准点-原点

        {744, 255 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},                                              //54 下一段
        {744, 255, KEY_X2, KEY_Y2},                                                                    //53 上一段
        {TYPE_X14 + SAFE_DIST1 + KEY_X6, TYPE_Y31, KEY_X6, KEY_Y6},                                    //55 方向-左
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 4, TYPE_Y31, KEY_X6, KEY_Y6},                                //56 方向-右
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 160, TYPE_SIZE14, TYPE_SIZE5},                           //57 数据-距离
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 160 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5}, //58 数据-针距

        {744, 255 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},                                                    //54 下一段
        {744, 255, KEY_X2, KEY_Y2},                                                                          //53 上一段
        {TYPE_X14 + SAFE_DIST1, TYPE_Y31, KEY_X6, KEY_Y6},                                                   //61 方法-1
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5 / 3, TYPE_Y31, KEY_X6, KEY_Y6},                                  //62 方法-2
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 10 / 3, TYPE_Y31, KEY_X6, KEY_Y6},                                 //63 方法-3
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5, TYPE_Y31, KEY_X6, KEY_Y6},                                      //64 方法-4
        {TYPE_X14 + SAFE_DIST1 + KEY_X6, TYPE_Y32, KEY_X6, KEY_Y6},                                          //65 方向-左
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 4, TYPE_Y32, KEY_X6, KEY_Y6},                                      //66 方向-右
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260, TYPE_SIZE14, TYPE_SIZE5},                                 //67 数据-距离
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5},       //68 数据-次数
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5}, //69 数据-针距

        {744, 255 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2}, //71 下一段
        {744, 255, KEY_X2, KEY_Y2},                       //70 上一段

        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5}, //72 数据 - 兄弟-角度修正

    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {TLFNADD_GENERAL, 0, 0, 0}, {TLFNADD_GENERAL}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {TLFNADD_GENERAL + 1, 0, 0, 0}, {TLFNADD_GENERAL + 1}}, //1 屏保
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {TLFNADD_GENERAL + 2, 0, 0, 0}, {0}},              //2 返回
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {TLFNADD_GENERAL + 3, 0, 0, 0}, {0}},              //3 确认
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {11, 13, 12}, {TLFNADD_GENERAL + 4, 0, 0, 0}, {0}},          //4 图像预览

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {17, 19, 18, 20}, {TLFNADD_GENERAL + 6, TLFNADD_GENERAL + 5, 0, 0}, {0}}, //5 抬压脚
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {361, 362}, {TLFNADD_GENERAL + 7, 0, 0, 0}, {0}},                         //6 取消
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {74}, {TLFNADD_GENERAL + 20, 0, 0, 0}, {0}},                              //7 缩小
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {75}, {TLFNADD_GENERAL + 21, 0, 0, 0}, {0}},                              //8 放大

        {{CONVAS_COLOR_B}, {CONVAS_COLOR_T}, {CONVAS_COLOR_B}, {TLFNADD_GENERAL + 21, 0, 0, 0}, {0}},                 //9 画布移动
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Silver}, {364}, {TLFNADD_GENERAL + 22, 0, 0, 0}, {0}}, //10 吸附点

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {55}, {TLFNADD_GENERAL + 8, 0, 0, 0}, {0}},      //11 左上
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {56}, {TLFNADD_GENERAL + 9, 0, 0, 0}, {0}},      //12 上
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {57}, {TLFNADD_GENERAL + 10, 0, 0, 0}, {0}},     //13 右上
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {58}, {TLFNADD_GENERAL + 11, 0, 0, 0}, {0}},     //14 左
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {51, 52}, {TLFNADD_GENERAL + 12, 0, 0, 0}, {0}}, //15 压框速度
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {59}, {TLFNADD_GENERAL + 13, 0, 0, 0}, {0}},     //16 右
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {60}, {TLFNADD_GENERAL + 14, 0, 0, 0}, {0}},     //17 左下
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {61}, {TLFNADD_GENERAL + 15, 0, 0, 0}, {0}},     //18 下
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {62}, {TLFNADD_GENERAL + 16, 0, 0, 0}, {0}},     //19 右下

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200}, {TLFNADD_GENERAL + 17, 0, 0, 0}, {0}}, //20 +1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202}, {TLFNADD_GENERAL + 18, 0, 0, 0}, {0}}, //21 -1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {91, 101}, {0, 0, 0, 0}, {0}},                     //22 0
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {92, 102}, {0, 0, 0, 0}, {0}},                     //23 1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {93, 103}, {0, 0, 0, 0}, {0}},                     //24 2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {94, 104}, {0, 0, 0, 0}, {0}},                     //25 3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {95, 105}, {0, 0, 0, 0}, {0}},                     //26 4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {96, 106}, {0, 0, 0, 0}, {0}},                     //27 5
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {97, 107}, {0, 0, 0, 0}, {0}},                     //28 6
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {98, 108}, {0, 0, 0, 0}, {0}},                     //29 7
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {99, 109}, {0, 0, 0, 0}, {0}},                     //30 8
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {100, 110}, {0, 0, 0, 0}, {0}},                    //31 9

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {594, 0, 595}, {TLFNADD_ORGSUB + 2, 0, 0, 0}, {0}},   //32 原点修正关闭
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {591, 593, 592}, {TLFNADD_ORGSUB + 3, 0, 0, 0}, {0}}, //33 原点修正开启

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {631, 0, 632}, {TLFNADD_SCALE + 2, 0, 0, 0}, {0}},                           //34 方法-针数固定
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {633, 0, 634}, {TLFNADD_SCALE + 3, 0, 0, 0}, {0}},                           //35 方法-针距固定
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_SCALE + 18, 0, 0, 0}, {0}}, //36 数据-X轴缩放倍率
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_SCALE + 19, 0, 0, 0}, {0}}, //37 数据-Y轴缩放倍率
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_SCALE + 6, 0, 0, 0}, {0}},  //38 数据-针距设定
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_SCALE + 7, 0, 0, 0}, {0}},  //39 数据 - 兄弟-角度修正
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {635, 0, 636}, {TLFNADD_SCALE + 8, 0, 0, 0}, {0}},                           //40 基准点-输入点
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {637, 0, 638}, {TLFNADD_SCALE + 9, 0, 0, 0}, {0}},                           //41 基准点-花样中心
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {639, 0, 640}, {TLFNADD_SCALE + 10, 0, 0, 0}, {0}},                          //42 基准点-原点

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {641, 0, 642}, {TLFNADD_MIRROR + 2, 0, 0, 0}, {0}}, //43 源数据-删除原图
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {604, 0, 605}, {TLFNADD_MIRROR + 3, 0, 0, 0}, {0}}, //44 源数据-保留原图
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {643, 0, 644}, {TLFNADD_MIRROR + 4, 0, 0, 0}, {0}}, //45 方法-垂直对称
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {645, 0, 646}, {TLFNADD_MIRROR + 5, 0, 0, 0}, {0}}, //46 方法-水平对称
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {647, 0, 648}, {TLFNADD_MIRROR + 6, 0, 0, 0}, {0}}, //47 方法-中心

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {649, 0, 650}, {TLFNADD_ROTATE + 2, 0, 0, 0}, {0}}, //48 方向-左
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {651, 0, 652}, {TLFNADD_ROTATE + 3, 0, 0, 0}, {0}}, //49 方向-右
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {635, 0, 636}, {TLFNADD_ROTATE + 4, 0, 0, 0}, {0}}, //50 基准点-输入点
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {637, 0, 638}, {TLFNADD_ROTATE + 5, 0, 0, 0}, {0}}, //51 基准点-花样中心
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {639, 0, 640}, {TLFNADD_ROTATE + 6, 0, 0, 0}, {0}}, //52 基准点-原点

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202, 0}, {TLFNADD_OFFSET + 3, 0, 0, 0}, {0}},                          //54 下一段
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200, 0}, {TLFNADD_OFFSET + 2, 0, 0, 0}, {0}},                          //53 上一段
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {653, 0, 654}, {TLFNADD_OFFSET + 4, 0, 0, 0}, {0}},                          //55 方向-左
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {655, 0, 656}, {TLFNADD_OFFSET + 5, 0, 0, 0}, {0}},                          //56 方向-右
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_OFFSET + 6, 0, 0, 0}, {0}}, //57 数据-距离
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_OFFSET + 7, 0, 0, 0}, {0}}, //58 数据-针距

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202, 0}, {TLFNADD_OFFSMULT + 3, 0, 0, 0}, {0}},                           //54 下一段
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200, 0}, {TLFNADD_OFFSMULT + 2, 0, 0, 0}, {0}},                           //53 上一段
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {422, 424, 423}, {TLFNADD_OFFSMULT + 4, 0, 0, 0}, {0}},                         //61 方法-1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {425, 427, 426}, {TLFNADD_OFFSMULT + 5, 0, 0, 0}, {0}},                         //62 方法-2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {431, 433, 432}, {TLFNADD_OFFSMULT + 6, 0, 0, 0}, {0}},                         //63 方法-3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {434, 436, 435}, {TLFNADD_OFFSMULT + 7, 0, 0, 0}, {0}},                         //64 方法-4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {653, 0, 654}, {TLFNADD_OFFSMULT + 8, 0, 0, 0}, {0}},                           //65 方向-左
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {655, 0, 656}, {TLFNADD_OFFSMULT + 9, 0, 0, 0}, {0}},                           //66 方向-右
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_OFFSMULT + 10, 0, 0, 0}, {0}}, //67 数据-距离
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_OFFSMULT + 11, 0, 0, 0}, {0}}, //68 数据-次数
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_OFFSMULT + 12, 0, 0, 0}, {0}}, //69 数据-针距

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202}, {TLFNADD_CHANGE + 3, 0, 0, 0}, {0}}, //71 下一段
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200}, {TLFNADD_CHANGE + 2, 0, 0, 0}, {0}}, //70 上一段

        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {TLFNADD_SCALE + 7, 0, 0, 0}, {0}}, //72 数据 - 兄弟-角度修正

    };

    const U16 rectTitList[10][4] = {
        //0:X; 1:Y; 2:起缝点标题;3:结束点标题;
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //原点修正
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //前后倒缝
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //z字缝
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //缩放
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //镜像
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //旋转
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //离边缝
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //多重缝
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //首尾对调
        {TLFNADD_GENERAL + 23, TLFNADD_GENERAL + 24, TLFNADD_GENERAL + 27, 0}, //针角度调整

    };

    const U16 rectDirBmpList[2][2] = {
        {653, 655}, //离边缝方向
        {437, 439}, //多重缝方向
    };

    const U16 rectMetBmpList[2][4] = {
        {428},                //离边缝方法
        {422, 425, 431, 434}, //多重缝方法
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_GENERAL}},

        {TYPE_X17, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {TLFNADD_GENERAL + 23}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {TLFNADD_GENERAL + 24}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X17, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {TLFNADD_GENERAL + 25}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {TLFNADD_GENERAL + 26}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {292, 396, 120, 80, {Color_Black}, {Color_Black}, {TLFNADD_ORGSUB + 7}}, //原点修正-方法框

        {TYPE_X14, TYPE_Y3, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_SCALE + 11}},                                                         //缩放-方法框1
        {TYPE_X14, TYPE_Y3 + TYPE_SIZE28 + SAFE_DIST6, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_SCALE + 14}},                              //缩放-方法框2
        {TYPE_X14, 260, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_SCALE + 18}},                                                            //缩放-数据框1
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},                                 //缩放-数据框1
        {TYPE_X14, 260 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_SCALE + 19}},                                  //缩放-数据框2
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},       //缩放-数据框2
        {TYPE_X14, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_SCALE + 6}},                             //缩放-数据框3
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //缩放-数据框3
        {TYPE_X14, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 3, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_SCALE + 7}},                             //缩放-数据框4
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 3, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}}, //缩放-数据框4

        {TYPE_X14, TYPE_Y3, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_MIRROR + 7}},                            //镜像-方法框1
        {TYPE_X14, TYPE_Y3 + TYPE_SIZE28 + SAFE_DIST6, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_MIRROR + 8}}, //镜像-方法框2

        {TYPE_X14, TYPE_Y3, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_ROTATE + 7}},                             //旋转-方法框1
        {TYPE_X14, TYPE_Y3 + TYPE_SIZE28 + SAFE_DIST6, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_ROTATE + 12}}, //旋转-方法框2
        {TYPE_X14, 260, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_ROTATE + 10}},                               //旋转-数据框1
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},     //旋转-数据框2

        {TYPE_X9, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 16}}, //多重缝-方向
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y2, TYPE_SIZE21, TYPE_SIZE21, {Color_Black}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 18}}, //多重缝-方法
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y10, TYPE_SIZE21, TYPE_SIZE21, {Color_Black}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y10 + 60, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 13}}, //多重缝-距离
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y10 + 60, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y10 + 60 * 2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 14}}, //多重缝-次数
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y10 + 60 * 2, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y10 + 60 * 2 + 70, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 17}}, //多重缝-编号
        {TYPE_X9 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y10 + 60 * 2 + 70, TYPE_SIZE8, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},

        {TYPE_X14, TYPE_Y3, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_OFFSET + 11}}, //离边缝-方法框
        {TYPE_X14, 160, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSET + 8}},     //离边缝-距离
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 160, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},
        {TYPE_X14, 160 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSET + 10}}, //离边缝-针距
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 160 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},
        {TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSET + 12}}, //离边缝-编号
        {TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},

        {TYPE_X14, TYPE_Y3, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_OFFSMULT + 18}},                         //多重缝-方法框1
        {TYPE_X14, TYPE_Y3 + TYPE_SIZE28 + SAFE_DIST6, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black}, {TLFNADD_SCALE + 16}}, //多重缝-方法框2
        {TYPE_X14, 260, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 13}},                            //多重缝-距离
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},
        {TYPE_X14, 260 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 14}}, //多重缝-次数
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},
        {TYPE_X14, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 15}}, //多重缝-针距
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},
        {TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_OFFSMULT + 17}}, //多重缝-编号
        {TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},

        {TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_CHANGE + 4}}, //首位变换-编号
        {TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},

        {TYPE_X14, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 3, TYPE_SIZE17, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_SCALE + 7}}, //针角度
        {TYPE_X14 + TYPE_SIZE17 + SAFE_DIST5, 260 + (TYPE_SIZE5 + SAFE_DIST5) * 3, TYPE_SIZE14, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black}, {0}},

        {TYPE_X4, TYPE_Y4, TYPE_SIZE23, TYPE_SIZE9, {Color_Blue}, {Color_White}, {0}}, //确认变换
        {500, TYPE_Y2, 300, 350, {Color_White}, {Color_Black}, {0}},                   //变换信息
    };

    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_CONVERT1); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(patRulesTmp);

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************
    if (F_FILETYPE == PAT_MIT)
    {              //三菱
        mult1 = 1; //g_dxpos,g_dypos
        mult2 = -1;
        tmuly = 1;
        tmp_feedsum = g_patd.feed_sum;
        tmp_draw_add = g_patd.draw_add;
        tmp_line_add = g_patd.line_add;
        tmp_pat_add = g_patd.pat_add;

        if ((g_num_mov_flag == 1) || (para == 0))
        {
            pfootdisf = 1;
        }
        if ((para < CONV_OFFSET) || (g_num_mov_flag == 2))
        {
            enterdisf = 1;
        }

        if ((plastno != MENU_SCRNSVR) && (plastno != MENU_MOIMAGE))
        {
            if ((g_selectmode > 1) && (para < CONV_OFFSET))
                g_selectmode = 0;
            if (((para == CONV_SCALE) || (para == CONV_ROTATE)) && (g_datum_mark == 0))
            {
                g_tmpoxpos = 0;
                g_tmpoypos = 0;
                for (ii = 0; ii < g_jog_point; ii++)
                {
                    dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                    if ((dat == 0x1) || (dat == 0x21) || (dat == 0x41) || (dat == 0x61) || (dat == 0x3) || (dat == 0x1b))
                    {
                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                        xx = dat & 0x000000ff;
                        yy = ((U32)(dat >> 8)) & 0x000000ff;
                        if (xx >= 0x80)
                        {
                            xx -= 0x80;
                            g_tmpoxpos -= xx;
                        }
                        else
                        {
                            g_tmpoxpos += xx;
                        }
                        if (yy >= 0x80)
                        {
                            yy -= 0x80;
                            g_tmpoypos -= yy;
                        }
                        else
                        {
                            g_tmpoypos += yy;
                        }
                    }
                }
                x = g_tmpoxpos + g_dxpos;
                y = g_tmpoypos + g_dypos;
                Comm_Move(x, y, 50, MENU_CONVERT1, 0, 0);
            }
            if (para == 3)
            {
                if (editflag > 1)
                    editflag = 0;
                if (g_datum_mark > 2)
                    g_datum_mark = 0;
                if ((g_convert_xscale < 10) || (g_convert_xscale > 4000))
                    g_convert_xscale = 1000;
                if ((g_convert_yscale < 10) || (g_convert_yscale > 4000))
                    g_convert_yscale = 1000;
            }

            g_publicbuf[60] = plastno;
        }
        else
        {
            plastno = g_publicbuf[60];
        }
        tmp_selectmode = g_selectmode;
    }
    else if (F_FILETYPE == PAT_SEW)
    { //兄弟
#if MOVEUNIT == 1
        mult1 = 1;
        mult3 = 2;
#else
        mult1 = 2;
#endif

        mult2 = 1;
        tmuly = -1;
        tmp_feedsum = g_patd.feed_sum;
        if ((g_selectmode > 1) && (para < 6 || para > 7))
            g_selectmode = 0;

        if (para == 3)
        { //放大、缩小
            enterdisf = 1;

            if (editflag > 1)
                editflag = 0;
            if (g_datum_mark > 2)
                g_datum_mark = 0;
            if ((g_convert_xscale < 10) || (g_convert_xscale > 4000))
                g_convert_xscale = 1000;
            if ((g_convert_yscale < 10) || (g_convert_yscale > 4000))
                g_convert_yscale = 1000;
            if ((g_convert_pindis < 1) || (g_convert_pindis > 127))
                g_convert_pindis = 30;

            if (g_datum_mark == 0)
            {
                //查找当前针坐标
                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, &dx, &dy, &speed, &Borpooft, &Borpfdis);
                g_tmpoxpos += dx;
                g_tmpoypos += dy;
            }
        }
        else if (para == 4)
        { //图像对称
            enterdisf = 1;
        }
        else if (para == 5)
        { //图像旋转
            enterdisf = 1;

            if (g_datum_mark == 0)
            {
                //查找当前针坐标
                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, &dx, &dy, &speed, &Borpooft, &Borpfdis);
                g_tmpoxpos += dx;
                g_tmpoypos += dy;
            }
        }
        else if ((para == 6) || (para == 7))
        { //辅助缝、多重逢
            if (g_num_mov_flag == 2)
            {
                enterdisf = 1;
            }
        }
        else if (para == 8)
        { //首尾交换
            //			enterdisf = 1;
        }
        tmp_selectmode = g_selectmode;
    }

    if (para == 0 && (g_dxpos != 0 || g_dypos != 0))
    {
        if (g_dxpos < 0)
        {
            g_minX += g_dxpos;
            g_maxX -= g_dxpos;
        }
        else
        {
            g_minX -= g_dxpos;
            g_maxX += g_dxpos;
        }

        if (g_dypos < 0)
        {
            g_minY += g_dypos;
            g_maxY -= g_dypos;
        }
        else
        {
            g_minY -= g_dypos;
            g_maxY += g_dypos;
        }
    }
    xo = ORG_X;
    yo = ORG_Y;
    scale = pat_getScale(patRule.h_patConvasIndex, patRulesTmp);
    vscale = pat_getFinalScale(scale, scale1);

    if ((P_TYA == 0) || (P_TYA > 15))
        P_TYA = 1;
    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************

    if (para == CONV_ORGSUB && (F_FILETYPE == PAT_MIT)) //	原点修正
    {
        title = TLFNADD_ORGSUB;
        editflag = 0;
        patdisf = 2;
        dis_pos = 1;
    }
    else if (para == CONV_TACKING || para == CONV_ZIG)
    {
    }
    else if (para == CONV_SCALE)
    {
        title = TLFNADD_SCALE;
        if (g_num_mov_flag == 1)
        {
            imgdisf = 0;
            patdisf = 3;
            dis_pos = 1;
        }
        else if (g_num_mov_flag == 2)
        {
            imgdisf = 1;
            patdisf = 0;
            dis_pos = 0;
        }
        else if (g_num_mov_flag == 3)
        {
            imgdisf = 0;
            patdisf = 2;
            dis_pos = 0;
        }
    }
    else if (para == CONV_MIRROR)
    {
        title = TLFNADD_MIRROR;
        imgdisf = 1;
        patdisf = 0;
        dis_pos = 0;
    }
    else if (para == CONV_ROTATE)
    {
        title = TLFNADD_ROTATE;
        if (g_num_mov_flag == 1)
        {
            imgdisf = 0;
            patdisf = 3;
            dis_pos = 1;
        }
        else if (g_num_mov_flag == 2)
        {
            imgdisf = 1;
            patdisf = 0;
            dis_pos = 0;
        }
        else if (g_num_mov_flag == 3)
        {
            imgdisf = 0;
            patdisf = 2;
            dis_pos = 0;
        }
    }
    else if (para == CONV_OFFSET)
    {
        title = TLFNADD_OFFSET;
        if (g_num_mov_flag == 0)
        {
            imgdisf = 0;
            patdisf = 2;
            dis_pos = 0;
        }
        else if (g_num_mov_flag == 2)
        {
            imgdisf = 1;
            patdisf = 0;
            dis_pos = 0;
        }
    }
    else if (para == CONV_OFFSMULT)
    {
        title = TLFNADD_OFFSMULT;
        if (g_num_mov_flag == 0)
        {
            imgdisf = 0;
            patdisf = 2;
            dis_pos = 0;
        }
        else if (g_num_mov_flag == 2)
        {
            imgdisf = 1;
            patdisf = 0;
            dis_pos = 0;
        }
    }
    else if (para == CONV_CHANGE)
    {
        title = TLFNADD_CHANGE;
        imgdisf = 0;
        patdisf = 2;
        dis_pos = 0;
    }
    else if (para == CONV_PINANG)
    {
        title = TLFNADD_PINANG;
        imgdisf = 1;
        patdisf = 0;
    }

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                 SheetPage, title, rect[0].colorT[0]); //标题

    //画出按键显示

    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************

    ViewPage(usepageno); //设置显示缓存
    if ((P_TYA == 0) || (P_TYA > 15))
        P_TYA = 1;

    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_CANCEL + 1, 0);
        if (keydata == 0 && patdisf)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_IMG + 1, KEY_CONVERT1_ZOUT);
        }
        if (keydata == 0 && patdisf && tonear)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_TOP + 1, KEY_CONVERT1_TOP);
        }
        if (keydata == 0)
        {
            if (g_num_mov_flag == 1)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_MDR + 1, KEY_CONVERT1_MUL);
            }
            else if (g_num_mov_flag == 2)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_9 + 1, KEY_CONVERT1_DUP);
                if ((para == CONV_ROTATE) && (tmp_selectmode == 5))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
            }
        }
        if (keydata == KEY_CONVERT1_PFOOT && pfootdisf == 0)
        {
            keydata = 0;
            keyolddata = 0;
        }
        if (keydata == KEY_CONVERT1_ENTER && enterdisf == 0)
        {
            keydata = 0;
            keyolddata = 0;
        }
        if (keydata == KEY_CONVERT1_CANCEL && deletedisf == 0)
        {
            keydata = 0;
            keyolddata = 0;
        }
        if (keydata == KEY_CONVERT1_VIEW && imgdisf == 0)
        {
            keydata = 0;
            keyolddata = 0;
        }
        if ((keydata == KEY_CONVERT1_IMG || keydata == KEY_CONVERT1_ZOUT || keydata == KEY_CONVERT1_ZIN) && patdisf == 0)
        {
            keydata = 0;
            keyolddata = 0;
        }

        if (keydata == 0)
        {
            switch (para)
            {
            case CONV_ORGSUB:
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_0ORGSUBON + 1, KEY_CONVERT1_0ORGSUBOF);
                break;
            case CONV_SCALE:
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_3HOMEBS + 1, KEY_CONVERT1_3SSTITCH); //30
                if (keydata == 0)
                {
                    keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_9ANG + 1, KEY_CONVERT1_9ANG);
                }
                if ((keydata == KEY_CONVERT1_3PITCH) && ((F_FILETYPE == PAT_MIT) || ((F_FILETYPE == PAT_SEW) && (tmp_selectmode == 0))))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                else if (g_num_mov_flag != 2)
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                else if (((F_FILETYPE == PAT_MIT) || (tmp_selectmode == 0)) && (keydata == KEY_CONVERT1_3ANG))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                break;
            case CONV_MIRROR:
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_4XYMIRROR + 1, KEY_CONVERT1_4MIRRORDEL); //38
                break;
            case CONV_ROTATE:                                                                           //图形旋转
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_5HOMEBS + 1, KEY_CONVERT1_5LROTATE); //43
                if (g_num_mov_flag != 2)
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                break;
            case CONV_OFFSET: //图形离边
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_6PITCH + 1, KEY_CONVERT1_6PREV);
                if (keydata == 0)
                {
                    keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_9ANG + 1, KEY_CONVERT1_9ANG);
                }
                if (((keydata == KEY_CONVERT1_6PREV) && ((g_selectmode < 2) || (g_num_mov_flag))) ||
                    ((keydata == KEY_CONVERT1_6NEXT) && ((g_selectmode >= tmp_selectmode) || (g_num_mov_flag))) ||
                    ((keydata > KEY_CONVERT1_6NEXT) && (g_num_mov_flag != 2)))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                if ((F_FILETYPE == PAT_MIT) && ((keydata == KEY_CONVERT1_6DIS) || (keydata == KEY_CONVERT1_6PITCH) ||
                                                (keydata == KEY_CONVERT1_9ANG)))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                break;
            case CONV_OFFSMULT: //图形多重缝
                keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_7PITCH + 1, KEY_CONVERT1_7PREV);
                if (keydata == 0)
                {
                    keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_9ANG + 1, KEY_CONVERT1_9ANG);
                }
                if (((keydata == KEY_CONVERT1_7PREV) && ((g_selectmode < 2) || (g_num_mov_flag))) ||
                    ((keydata == KEY_CONVERT1_7NEXT) && ((g_selectmode >= tmp_selectmode) || (g_num_mov_flag))) ||
                    ((keydata > KEY_CONVERT1_7NEXT) && (g_num_mov_flag != 2)))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                if ((F_FILETYPE == PAT_MIT) && ((keydata == KEY_CONVERT1_7PITCH) || (keydata == KEY_CONVERT1_9ANG)))
                {
                    keydata = 0;
                    keyolddata = 0;
                }
                break;
            case CONV_CHANGE: //首尾变换
                if (F_FILETYPE == PAT_SEW)
                {
                    keydata = get_keydata((KEYPAD *)keyd, KEY_CONVERT1_8NEXT + 1, KEY_CONVERT1_8PREV); //66
                    if (((keydata == KEY_CONVERT1_8PREV) && (g_selectmode < 2)) ||
                        ((keydata == KEY_CONVERT1_8NEXT) && (g_selectmode >= tmp_selectmode)))
                    {
                        keydata = 0;
                        keyolddata = 0;
                    }
                }
                break;
            }
        }

        // /[按键键值返回预处理*begin]**********************************************************

        //特定情况下不生效按键预处理
        //无效按键

        if (keydata == 0xff) //按键松开后的键值预处理
        {
            if ((keyon >= KEY_CONVERT1_MUL && keyon <= KEY_CONVERT1_MDR && keyon != KEY_CONVERT1_MMSP) ||
                (keyon >= KEY_CONVERT1_DUP && keyon <= KEY_CONVERT1_DDOWN) ||
                (keyon >= KEY_CONVERT1_ZOUT && keyon <= KEY_CONVERT1_IMG))
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
                pressKey = 1;
                if (keyon == KEY_CONVERT1_SCRNSVR || keyon == KEY_CONVERT1_IMG) //无按键特效
                {
                }
                //非常规按键-//绘制颜色文字按键
                else if (keyon == KEY_CONVERT1_3XSCALE || keyon == KEY_CONVERT1_3YSCALE ||
                         keyon == KEY_CONVERT1_3PITCH || keyon == KEY_CONVERT1_3ANG ||
                         keyon == KEY_CONVERT1_6DIS || keyon == KEY_CONVERT1_6PITCH ||
                         keyon == KEY_CONVERT1_7SELDIST || keyon == KEY_CONVERT1_7SELTIME || keyon == KEY_CONVERT1_7PITCH ||
                         keyon == KEY_CONVERT1_9ANG)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //按键防抖次数
                ll = 1;
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
                    case KEY_CONVERT1_PFOOT:
                        DrawFMenuStr((pfootf == 0) ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
                        break;
                    case KEY_CONVERT1_0:
                    case KEY_CONVERT1_1:
                    case KEY_CONVERT1_2:
                    case KEY_CONVERT1_3:
                    case KEY_CONVERT1_4:
                    case KEY_CONVERT1_5:
                    case KEY_CONVERT1_6:
                    case KEY_CONVERT1_7:
                    case KEY_CONVERT1_8:
                    case KEY_CONVERT1_9:
                        BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                        DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_CONVERT1_0, 1, rect[0].colorT[0]);
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
                if ((keyon >= KEY_CONVERT1_MUL && keyon <= KEY_CONVERT1_MDR && keyon != KEY_CONVERT1_MMSP) ||
                    (keyon >= KEY_CONVERT1_DUP && keyon <= KEY_CONVERT1_DDOWN) ||
                    (keyon >= KEY_CONVERT1_ZOUT && keyon <= KEY_CONVERT1_IMG))
                {
                    keyoff = keyon;
                }
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
            pressKey = 1;
            if ((keyon >= KEY_CONVERT1_MUL) && (keyon <= KEY_CONVERT1_MDR) && (keyon != KEY_CONVERT1_MMSP)) // 方向键  7 -15
            {
                if ((!(S_CLAMP)) && (P_OST == 0))
                {
                    pno = MENU_RWHELP;
                    hleppara = 13; //bpnohp = MENU_CONVERT1;
                }
                else
                {
                    if ((g_dxpos == 0) && (g_dypos == 0))
                    {
                        if ((P_TYA == 0) || (P_TYA > 15))
                            P_TYA = 1;
                    }

                    getNearflag = 1;
                    tmpxpos = g_dxpos;
                    tmpypos = g_dypos;

                    if ((keyoff == KEY_CONVERT1_MUR) || (keyoff == KEY_CONVERT1_MMR) || (keyoff == KEY_CONVERT1_MDR))
                    {
                        g_dxpos += ll;
                    }
                    else if ((keyoff == KEY_CONVERT1_MUL) || (keyoff == KEY_CONVERT1_MML) || (keyoff == KEY_CONVERT1_MDL))
                    {
                        g_dxpos -= ll;
                    }
                    if ((keyoff == KEY_CONVERT1_MUL) || (keyoff == KEY_CONVERT1_MUM) || (keyoff == KEY_CONVERT1_MUR))
                    {
                        g_dypos += ll;
                    }
                    else if ((keyoff == KEY_CONVERT1_MDL) || (keyoff == KEY_CONVERT1_MDM) || (keyoff == KEY_CONVERT1_MDR))
                    {
                        g_dypos -= ll;
                    }

                    if (para == CONV_ORGSUB)
                    { //原点修正
                        if (g_dxpos > 127)
                            g_dxpos = 127;
                        if (g_dxpos < -127)
                            g_dxpos = -127;
                        if (g_dypos > 127)
                            g_dypos = 127;
                        if (g_dypos < -127)
                            g_dypos = -127;
                        f = Comm_Move(g_tmpoxpos + g_dxpos, g_tmpoypos + g_dypos, ll, MENU_CONVERT1, ((F_FILETYPE == PAT_MIT) ? 0 : 1), 0);
                    }
                    else
                    {
                        mvxx = g_tmpoxpos + mult1 * g_dxpos;
                        mvyy = g_tmpoypos + mult1 * g_dypos;
                        if (mvxx < mult1 * mult3 * g_Mac.max_xl)
                            g_dxpos = (mult1 * mult3 * g_Mac.max_xl - g_tmpoxpos) / mult1;
                        if (mvxx > mult1 * mult3 * g_Mac.max_xr)
                            g_dxpos = (mult1 * mult3 * g_Mac.max_xr - g_tmpoxpos) / mult1;
                        if (mvyy > mult1 * mult3 * g_Mac.max_yu)
                            g_dypos = (mult1 * mult3 * g_Mac.max_yu - g_tmpoypos) / mult1;
                        if (mvyy < mult1 * mult3 * g_Mac.max_yd)
                            g_dypos = (mult1 * mult3 * g_Mac.max_yd - g_tmpoypos) / mult1;

#if MOVEUNIT == 1
                        if (g_curPTtype == 2)
                        {
                            f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll / 2, MENU_CONVERT1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                        }
                        else
                        {
                            f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll, MENU_CONVERT1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                        }
#else
                        f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll, MENU_CONVERT1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
#endif
                    }
                    if (f == 0)
                    {
                        g_dxpos = tmpxpos;
                        g_dypos = tmpypos;
                    }
                    if (ll < P_TYA * 8)
                        ll += P_TYA;
                    else
                        ll += P_TYA * 2;
                    dat = 20 + P_TYA * 5;
                    if (ll > dat)
                        ll = dat;
                    for (ii = 0; ii < RDTIMES; ii++)
                    {
                        Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                        if ((DREG[D_CMDSTATUS]) == 100)
                            break;
                        DelayMs(6);
                    }

                    dis_updata15 = 1;
                    vflag = 0;
                    if (g_tmpoxpos + mult1 * g_dxpos > g_maxX || g_tmpoxpos + mult1 * g_dxpos < g_minX || mult2 * (g_tmpoypos + mult1 * g_dypos) > g_maxY || mult2 * (g_tmpoypos + mult1 * g_dypos) < g_minY)
                    {
                        if (g_tmpoxpos + mult1 * g_dxpos > g_maxX)
                        {
                            g_maxX = g_tmpoxpos + mult1 * g_dxpos;
                        }
                        if (g_tmpoxpos + mult1 * g_dxpos < g_minX)
                        {
                            g_minX = g_tmpoxpos + mult1 * g_dxpos;
                        }
                        if (mult2 * (g_tmpoypos + mult1 * g_dypos) > g_maxY)
                        {
                            g_maxY = mult2 * (g_tmpoypos + mult1 * g_dypos);
                        }
                        if (mult2 * (g_tmpoypos + mult1 * g_dypos) < g_minY)
                        {
                            g_minY = mult2 * (g_tmpoypos + mult1 * g_dypos);
                        }
                        dis_updata2 = 1;
                        scale = pat_getScale(patRule.h_patConvasIndex, patRulesTmp);
                        vscale = pat_getFinalScale(scale, scale1);
                        onearpx = 0;
                        onearpy = 0;
                    }
                    else
                    {
                        posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
                        posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
                        if (((posx1 < keyd[KEY_CONVERT1_IMG].xpis) || (posx1 > keyd[KEY_CONVERT1_IMG].xpis + keyd[KEY_CONVERT1_IMG].xsize) || (posy1 < keyd[KEY_CONVERT1_IMG].ypis) || (posy1 > keyd[KEY_CONVERT1_IMG].ypis + keyd[KEY_CONVERT1_IMG].ysize)))
                        { //超出显示范围
                            dis_updata2 = 1;
                        }
                        else
                        {
                            RecoveData2();
                            ReadPointData2(posx1 - 4, posy1 - 4);
                            // ViewPoint(posx1, posy1, Color_Red, 1);
                            Draw_SmallCrossLimit(posx1, posy1, COLOR_CROSS, patRule.h_patConvasIndex);
                        }
                    }
                }
            }
            else if ((keyoff >= KEY_CONVERT1_DUP) && (keyoff <= KEY_CONVERT1_9)) // 数字键  16 - 27
            {
                if (para == CONV_SCALE) //放大缩小
                {

                    if (editflag < 2) //缩放
                    {
                        if (keyoff == KEY_CONVERT1_DUP)
                        {
                            tmp_parameter++;
                        }
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter > 10)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 1000) * 10 + keyoff - KEY_CONVERT1_0;
                            if (tmp_parameter > 4000)
                                tmp_parameter = tmp_parameter % 1000;
                        }
                        if (tmp_parameter > 4000)
                            tmp_parameter = 4000;

                        // DrawFillRect(xx, yy, 65, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 5, 1, Color_White);
                        dis_updata3 = 1;
                    }
                    else if (editflag == 2) //针距
                    {
                        if (keyoff == KEY_CONVERT1_DUP)
                            tmp_parameter++;
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 100) * 10 + (keyoff - KEY_CONVERT1_0);
                            if (tmp_parameter > 127)
                                tmp_parameter = tmp_parameter % 100;
                        }
                        if (tmp_parameter > 127)
                            tmp_parameter = 127;
                        // DrawFillRect(xx, yy, 65, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                        dis_updata3 = 1;
                    }
                    else if (editflag == 3) //修正角度
                    {
                        if (keyoff == KEY_CONVERT1_DUP)
                            tmp_parameter++;
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter > 20)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 10) * 10 + (keyoff - KEY_CONVERT1_0);
                            if (tmp_parameter > 60)
                                tmp_parameter = tmp_parameter % 10;
                        }
                        if (tmp_parameter > 60)
                            tmp_parameter = 60;
                        // DrawFillRect(xx, yy, 32, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 2, 0, Color_White);
                        dis_updata3 = 1;
                    }
                }
                else if (para == CONV_ROTATE) //图型翻转
                {
                    if (keyoff == KEY_CONVERT1_DUP)
                    {
                        tmp_parameter++;
                    }
                    else if (keyoff == KEY_CONVERT1_DDOWN)
                    {
                        tmp_parameter--;
                    }
                    else
                    {
                        tmp_parameter = (tmp_parameter % 1000) * 10 + keyoff - KEY_CONVERT1_0;
                        if (tmp_parameter > 1800)
                            tmp_parameter = tmp_parameter % 1000;
                    }
                    if (tmp_parameter > 1800)
                        tmp_parameter = 1800;
                    if (tmp_parameter < 1)
                        tmp_parameter = 1;

                    // DrawFillRect(xx, yy, 65, 24, Color_DimGray);
                    // DrawMFS32P(xx, yy, tmp_parameter, 5, 1, Color_White);
                    dis_updata3 = 1;
                }
                else if (para == CONV_OFFSET) //离边
                {
                    if (F_FILETYPE == PAT_MIT)
                    { //三菱
                        if (keyoff == KEY_CONVERT1_DUP)
                        {
                            tmp_parameter++;
                        }
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter > 0)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 100) * 10 + keyoff - KEY_CONVERT1_0;
                            if (tmp_parameter > 200)
                                tmp_parameter = tmp_parameter % 100;
                        }
                        if (tmp_parameter > 200)
                            tmp_parameter = 200;

                        // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                        dis_updata3 = 1;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //兄弟
                        if (editflag == 0)
                        { //距离
                            if (keyoff == KEY_CONVERT1_DUP)
                            {
                                tmp_parameter++;
                            }
                            else if (keyoff == KEY_CONVERT1_DDOWN)
                            {
                                if (tmp_parameter)
                                    tmp_parameter--;
                            }
                            else
                            {
                                tmp_parameter = (tmp_parameter % 100) * 10 + keyoff - KEY_CONVERT1_0;
                                if (tmp_parameter > 200)
                                    tmp_parameter = tmp_parameter % 100;
                            }
                            if (tmp_parameter > 200)
                                tmp_parameter = 200;
                            //if(tmp_parameter<1)tmp_parameter = 1;

                            // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                            // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                            dis_updata3 = 1;
                        }
                        else if (editflag == 1)
                        { //针距
                            if (keyoff == KEY_CONVERT1_DUP)
                                tmp_parameter++;
                            else if (keyoff == KEY_CONVERT1_DDOWN)
                            {
                                if (tmp_parameter)
                                    tmp_parameter--;
                            }
                            else
                            {
                                tmp_parameter = (tmp_parameter % 100) * 10 + (keyoff - KEY_CONVERT1_0);
                                if (tmp_parameter > 127)
                                    tmp_parameter = tmp_parameter % 100;
                            }
                            if (tmp_parameter > 127)
                                tmp_parameter = 127;

                            // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                            // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                            dis_updata3 = 1;
                        }
                        else if (editflag == 2)
                        { //修正角度
                            if (keyoff == KEY_CONVERT1_DUP)
                                tmp_parameter++;
                            else if (keyoff == KEY_CONVERT1_DDOWN)
                            {
                                if (tmp_parameter > 20)
                                    tmp_parameter--;
                            }
                            else
                            {
                                tmp_parameter = (tmp_parameter % 10) * 10 + (keyoff - KEY_CONVERT1_0);
                                if (tmp_parameter > 60)
                                    tmp_parameter = tmp_parameter % 10;
                            }
                            if (tmp_parameter > 60)
                                tmp_parameter = 60;
                            // DrawFillRect(xx, yy, 32, 24, Color_DimGray);
                            // DrawMFS32P(xx, yy, tmp_parameter, 2, 0, Color_White);
                            dis_updata3 = 1;
                        }
                    }
                }
                else if (para == CONV_OFFSMULT) //多重逢
                {
                    if (editflag == 0)
                    { //距离
                        if (keyoff == KEY_CONVERT1_DUP)
                        {
                            tmp_parameter++;
                        }
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter > 0)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 100) * 10 + keyoff - KEY_CONVERT1_0;
                            if (tmp_parameter > 200)
                                tmp_parameter = tmp_parameter % 100;
                        }
                        if (tmp_parameter > 200)
                            tmp_parameter = 200;
                        // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                        dis_updata3 = 1;
                    }
                    else if (editflag == 1)
                    { //次数
                        if (keyoff == KEY_CONVERT1_DUP)
                            tmp_parameter++;
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                            tmp_parameter--;
                        else
                        {
                            tmp_parameter = keyoff - KEY_CONVERT1_0;
                        }
                        if (tmp_parameter > 9)
                            tmp_parameter = 9;
                        if (tmp_parameter < 2)
                            tmp_parameter = 2;
                        // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 1, 0, Color_White);
                        dis_updata3 = 1;
                    }
                    else if (editflag == 2)
                    { //针距
                        if (keyoff == KEY_CONVERT1_DUP)
                            tmp_parameter++;
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 100) * 10 + (keyoff - KEY_CONVERT1_0);
                            if (tmp_parameter > 127)
                                tmp_parameter = tmp_parameter % 100;
                        }
                        if (tmp_parameter > 127)
                            tmp_parameter = 127;
                        if (tmp_parameter < 1)
                            tmp_parameter = 1;
                        // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                        dis_updata3 = 1;
                    }
                    else if (editflag == 3)
                    { //修正角度
                        if (keyoff == KEY_CONVERT1_DUP)
                            tmp_parameter++;
                        else if (keyoff == KEY_CONVERT1_DDOWN)
                        {
                            if (tmp_parameter > 20)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 10) * 10 + (keyoff - KEY_CONVERT1_0);
                            if (tmp_parameter > 60)
                                tmp_parameter = tmp_parameter % 10;
                        }
                        if (tmp_parameter > 60)
                            tmp_parameter = 60;
                        // DrawFillRect(xx, yy, 32, 24, Color_DimGray);
                        // DrawMFS32P(xx, yy, tmp_parameter, 2, 0, Color_White);
                        dis_updata3 = 1;
                    }
                }
            }
            else if ((keyoff >= KEY_CONVERT1_0ORGSUBOF) && (keyoff <= KEY_CONVERT1_0ORGSUBON)) //原点修正
            {                                                                                  // 0  28 - 29
                dis_updata = 0x01;
                tmp_selectmode = keyoff - KEY_CONVERT1_0ORGSUBOF;
                if (tmp_selectmode)
                    g_num_mov_flag = 1;
                else
                    g_num_mov_flag = 0;
            }
            else if ((keyoff >= KEY_CONVERT1_3SSTITCH) && (keyoff <= KEY_CONVERT1_3SPITCH)) //缩放
            {                                                                               // 3	30 - 31
                dis_updata = 0x02;
                tmp_selectmode = keyoff - KEY_CONVERT1_3SSTITCH;
            }
            else if ((keyoff >= KEY_CONVERT1_3XSCALE) && (keyoff <= KEY_CONVERT1_3PITCH)) //缩放
            {                                                                             // 3	32 - 34
                if (editflag != (keyoff - KEY_CONVERT1_3XSCALE))
                {
                    dis_updata = 0x08;
                    oldeditflag = editflag;
                    editflag = keyoff - KEY_CONVERT1_3XSCALE;
                    if (editflag == 0) //X方向缩放比率
                    {
                        if (oldeditflag == 1)
                        {
                            if (tmp_parameter < 10)
                                tmp_parameter = 1000;
                            g_convert_yscale = tmp_parameter;
                        }
                        else if (oldeditflag == 2)
                        {
                            if (tmp_parameter > 127)
                                tmp_parameter = 127;
                            g_convert_pindis = tmp_parameter;
                        }
                        else if (oldeditflag == 3)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_convert_xscale;
                    }
                    else if (editflag == 1) //Y方向缩放比率
                    {

                        if (oldeditflag == 0)
                        {
                            if (tmp_parameter < 10)
                                tmp_parameter = 1000;
                            g_convert_xscale = tmp_parameter;
                        }
                        else if (oldeditflag == 2)
                        {
                            if (tmp_parameter > 127)
                                tmp_parameter = 127;
                            g_convert_pindis = tmp_parameter;
                        }
                        else if (oldeditflag == 3)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_convert_yscale;
                    }
                    else if (editflag == 2) //针距
                    {

                        if (oldeditflag == 0)
                        {
                            if (tmp_parameter < 10)
                                tmp_parameter = 1000;
                            g_convert_xscale = tmp_parameter;
                        }
                        else if (oldeditflag == 1)
                        {
                            if (tmp_parameter < 10)
                                tmp_parameter = 1000;
                            g_convert_yscale = tmp_parameter;
                        }
                        else if (oldeditflag == 3)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_convert_pindis;
                    }
                }
            }
            else if ((keyoff >= KEY_CONVERT1_3INPUTBS) && (keyoff <= KEY_CONVERT1_3HOMEBS)) //缩放
            {                                                                               // 3	35 - 37
                dis_updata = 0x04;
                g_datum_mark = keyoff - KEY_CONVERT1_3INPUTBS;
            }
            else if ((keyoff >= KEY_CONVERT1_4MIRRORDEL) && (keyoff <= KEY_CONVERT1_4MIRRORHOLD)) //镜像
            {                                                                                     // 4	38 - 40
                tmp_selectmode = keyoff - KEY_CONVERT1_4MIRRORDEL;
                dis_updata = 0x02;
            }
            else if ((keyoff >= KEY_CONVERT1_4XMIRROR) && (keyoff <= KEY_CONVERT1_4XYMIRROR)) //镜像
            {                                                                                 // 4	 40 - 43
                dis_updata = 0x04;
                tmp_parameter = keyoff - KEY_CONVERT1_4XMIRROR;
            }
            else if ((keyoff >= KEY_CONVERT1_5LROTATE) && (keyoff <= KEY_CONVERT1_5RROTATE)) //旋转
            {                                                                                // 5     43 -  44
                dis_updata = 0x02;
                tmp_selectmode = keyoff - KEY_CONVERT1_5LROTATE;
            }
            else if ((keyoff >= KEY_CONVERT1_5INPUTBS) && (keyoff <= KEY_CONVERT1_5HOMEBS)) //旋转
            {                                                                               // 5     45 - 	47
                dis_updata = 0x04;
                g_datum_mark = keyoff - KEY_CONVERT1_5INPUTBS;
            }
            else if ((keyoff >= KEY_CONVERT1_6DIRL) && (keyoff <= KEY_CONVERT1_6DIRR)) //离边缝
            {                                                                          // 6   50 - 51
                dis_updata = 0x02;
                g_direction = keyoff - KEY_CONVERT1_6DIRL;
            }
            else if ((keyoff >= KEY_CONVERT1_6DIS) && (keyoff <= KEY_CONVERT1_6PITCH)) //离边缝
            {                                                                          // 6   52 - 53
                if (editflag != (keyoff - KEY_CONVERT1_6DIS))
                {
                    dis_updata = 0x08;
                    oldeditflag = editflag;
                    editflag = keyoff - KEY_CONVERT1_6DIS;
                    if (editflag == 0) //距离
                    {
                        if (oldeditflag == 1)
                        {
                            if (tmp_parameter > 127)
                                tmp_parameter = 127;
                            if (tmp_parameter == 0)
                                tmp_parameter = 1;
                            g_convert_pindis = tmp_parameter;
                        }
                        else if (oldeditflag == 2)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = Convedis;
                    }
                    else if (editflag == 1) //针距
                    {
                        if (oldeditflag == 0)
                        {
                            if (tmp_parameter > 200)
                                tmp_parameter = 200;
                            Convedis = tmp_parameter;
                        }
                        else if (oldeditflag == 2)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_convert_pindis;
                    }
                }
            }
            else if ((keyoff >= KEY_CONVERT1_7FDSYMULT) && (keyoff <= KEY_CONVERT1_7SWREMULT)) //多重缝
            {                                                                                  // 7   56 - 59
                dis_updata = 0x02;
                g_para_offset = keyoff - KEY_CONVERT1_7FDSYMULT;
                if (editflag == 0)
                { //距离
                    g_distance = tmp_parameter;
                }
                else if (editflag == 1)
                { //重缝次数
                    g_times = tmp_parameter;
                }
                else if (editflag == 2)
                { //针距
                    g_convert_pindis = tmp_parameter;
                }
                else if (editflag == 3)
                { //修正角度
                    ConAng = tmp_parameter;
                }
            }
            else if ((keyoff >= KEY_CONVERT1_7DIRL) && (keyoff <= KEY_CONVERT1_7DIRR)) //多重缝
            {                                                                          // 7	60 - 61
                dis_updata = 0x04;
                g_direction = keyoff - KEY_CONVERT1_7DIRL;
                if (editflag == 0)
                { //距离
                    g_distance = tmp_parameter;
                }
                else if (editflag == 1)
                { //重缝次数
                    g_times = tmp_parameter;
                }
                else if (editflag == 2)
                { //针距
                    g_convert_pindis = tmp_parameter;
                }
                else if (editflag == 3)
                { //修正角度
                    ConAng = tmp_parameter;
                }
            }
            else if ((keyoff >= KEY_CONVERT1_7SELDIST) && (keyoff <= KEY_CONVERT1_7PITCH))
            { // 7	62 - 64
                if (editflag != (keyoff - KEY_CONVERT1_7SELDIST))
                {
                    dis_updata = 0x08;
                    oldeditflag = editflag;
                    editflag = keyoff - KEY_CONVERT1_7SELDIST;
                    if (editflag == 0) //距离
                    {
                        if (oldeditflag == 1)
                        {
                            if (tmp_parameter < 2)
                                tmp_parameter = 2;
                            if (tmp_parameter > 9)
                                tmp_parameter = 9;
                            g_times = tmp_parameter;
                        }
                        else if (oldeditflag == 2)
                        {
                            if (tmp_parameter > 127)
                                tmp_parameter = 127;
                            if (tmp_parameter < 1)
                                tmp_parameter = 1;
                            g_convert_pindis = tmp_parameter;
                        }
                        else if (oldeditflag == 3)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_distance;
                    }
                    else if (editflag == 1) //重缝次数
                    {

                        if (oldeditflag == 0)
                        {
                            if (tmp_parameter > 200)
                                tmp_parameter = 200;
                            g_distance = tmp_parameter;
                        }
                        else if (oldeditflag == 2)
                        {
                            if (tmp_parameter > 127)
                                tmp_parameter = 127;
                            if (tmp_parameter < 1)
                                tmp_parameter = 1;
                            g_convert_pindis = tmp_parameter;
                        }
                        else if (oldeditflag == 3)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_times;
                    }
                    else if (editflag == 2) //针距
                    {
                        if (oldeditflag == 0)
                        {
                            if (tmp_parameter > 200)
                                tmp_parameter = 200;
                            g_distance = tmp_parameter;
                        }
                        else if (oldeditflag == 1)
                        {
                            if (tmp_parameter < 2)
                                tmp_parameter = 2;
                            if (tmp_parameter > 9)
                                tmp_parameter = 9;
                            g_times = tmp_parameter;
                        }
                        else if (oldeditflag == 3)
                        {
                            if (tmp_parameter > 60)
                                tmp_parameter = 30;
                            if (tmp_parameter < 20)
                                tmp_parameter = 30;
                            ConAng = tmp_parameter;
                        }
                        tmp_parameter = g_convert_pindis;
                    }
                }
            }
            else
            {
                switch (keyoff)
                {

                case KEY_CONVERT1_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    g_distance = tmp_parameter;
                    break;
                case KEY_CONVERT1_BACK:
                    Send_Pfoot(1, MENU_CONVERT1, 1);
                    if ((para == CONV_ORGSUB) && (F_FILETYPE == PAT_MIT)) //原点位置修正   兄弟款暂不支持
                    {
                        if (P_FHP)
                        {
                            Comm_RelationMove(-g_dxpos, -g_dypos, MENU_CONVERT1, 0, 0);
                        }
                        else
                        {
                            Go_Home(MENU_CONVERT1, 0); //0
                        }
                        g_dxpos = 0;
                        g_dypos = 0;
                        pno = MENU_CONVERT;
                        pnoindex--;
                        SREG[3] = g_tSREG3;
                    }
                    else if (((para == CONV_SCALE) || (para == CONV_ROTATE)) && (g_num_mov_flag == 1)) //放大缩小,旋转
                    {
                        g_dxpos = 0;
                        g_dypos = 0;
                        g_para1 += 25;
                        pno = MENU_MODIFYADD;
                        poldno[pnoindex] = MENU_CONVERT1;
                        pnoindex++;
                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                        Wait_Move_End(); //等待平台移动结束
                    }
                    else if (((para == CONV_SCALE) || (para == CONV_ROTATE)) && (g_num_mov_flag == 3)) //放大缩小,旋转
                    {
                        if (g_datum_mark)
                        {
                            g_num_mov_flag = 2;
                            if (para == CONV_SCALE)
                            {
                                if (editflag == 0)
                                {
                                    tmp_parameter = g_convert_xscale;
                                }
                                else
                                {
                                    tmp_parameter = g_convert_xscale;
                                }
                            }
                            else
                            {
                                tmp_parameter = g_parameter;
                                g_dxpos = 0;
                                g_dypos = 0;
                            }
                        }
                        else
                        {
                            g_num_mov_flag = 1;
                            if (para == CONV_SCALE)
                            {

                                xo = ORG_X;
                                yo = ORG_Y;
                                scale1 = 0;
                                vflag = 0;
                                scale = pat_getScale(patRule.h_patConvasIndex, patRulesTmp);
                                vscale = pat_getFinalScale(scale, scale1);
                            }

                            absposx = 0xffff;
                            absposy = 0xffff;
                            dis_pos = 1;
                            dis_updata2 = 1;
                        }
                        dis_updata = 0x11;
                        dis_updata1 = 1;
                        pfootf = ~(U16)(S_PFOOT);
                    }
                    else if (((para == CONV_OFFSET) || (para == CONV_OFFSMULT)) && (g_num_mov_flag == 2))
                    {
                        g_num_mov_flag = 0;
                        dis_updata = 0x11;
                        flag = 1;
                        if (F_FILETYPE == PAT_MIT)
                        {
                            tmp_feedsum = g_patd.feed_sum;
                            tmp_draw_add = g_patd.draw_add;
                            tmp_line_add = g_patd.line_add;
                            tmp_pat_add = g_patd.pat_add;
                        }
                    }
                    else
                    {
                        pno = MENU_CONVERT;
                        pnoindex--;
                    }
                    break;
                case KEY_CONVERT1_ENTER:
                    Send_Pfoot(1, MENU_CONVERT1, 1);
                    if ((para == CONV_ORGSUB) && (F_FILETYPE == PAT_MIT)) //原点位置修正   兄弟款暂不支持
                    {
                        g_selectmode = tmp_selectmode;
                        dat = twopoint_dis(g_dxpos, g_dypos);
                        if (0) //dat>127
                        {
                            pno = MENU_INHELP;
                            hleppara = 7;
                            poldno[pnoindex] = MENU_CONVERT1;
                            pnoindex++;
                        }
                        else
                        {
                            pno = MENU_CONVERT;
                            pnoindex--;
                            if (g_selectmode && g_dxpos != 0 && g_dypos != 0)
                                SREG[3] = 7;
                            else
                                SREG[3] = 6;
                            SREG[6] = g_dxpos;
                            SREG[7] = g_dypos;
                            if (P_FHP)
                            {
                                Comm_RelationMove(-g_dxpos, -g_dypos, MENU_CONVERT1, 0, 0);
                            }
                            Go_Home(MENU_CONVERT1, 0); //0

                            g_modifyflag |= 0x80;

                            g_convert_orgf = 1;
                        }
                    }
                    else if (para == CONV_SCALE) //放大缩小
                    {
                        if (g_num_mov_flag == 1)
                        { // 方向移动键输入
                            g_num_mov_flag = 3;
                            dis_updata = 0x11;
                            dis_updata2 = 1;
                        }
                        else if (g_num_mov_flag == 2) // 数字键输入
                        {
                            g_selectmode = tmp_selectmode;

                            if (F_FILETYPE == PAT_MIT) //三菱
                            {
                                if (tmp_parameter < 10)
                                    tmp_parameter = 1000;
                                if (editflag == 0)
                                    g_convert_xscale = tmp_parameter;
                                else
                                    g_convert_yscale = tmp_parameter;
                                ///////////////////////判断是否有圆或圆弧,如果有,应保持x和y方向缩放率一样
                                while (tmp_draw_add < tmp_line_add)
                                {
                                    dat = SREG[tmp_draw_add] & 0x1fd;
                                    if ((dat == 0xcc) || (dat == 0xd0)) // 圆 圆弧
                                    {
                                        if (g_convert_xscale != g_convert_yscale)
                                        {
                                            if (g_convert_xscale < g_convert_yscale)
                                                g_convert_yscale = g_convert_xscale;
                                            else
                                                g_convert_xscale = g_convert_yscale;
                                        }
                                        break;
                                    }
                                    tmp_draw_add += 4;
                                }
                            }
                            else if (F_FILETYPE == PAT_SEW) //兄弟
                            {
                                if (editflag == 0)
                                {
                                    if (tmp_parameter < 10)
                                        tmp_parameter = 1000;
                                    g_convert_xscale = tmp_parameter;
                                }
                                else if (editflag == 1)
                                {
                                    if (tmp_parameter < 10)
                                        tmp_parameter = 1000;
                                    g_convert_yscale = tmp_parameter;
                                }
                                else if (editflag == 2)
                                {
                                    if (tmp_parameter > 127)
                                        tmp_parameter = 127;
                                    if (tmp_parameter == 0)
                                        tmp_parameter = 1;
                                    g_convert_pindis = tmp_parameter;
                                }
                                else if (editflag == 3)
                                {
                                    if (tmp_parameter > 60)
                                        tmp_parameter = 30;
                                    if (tmp_parameter < 20)
                                        tmp_parameter = 30;
                                    ConAng = tmp_parameter;
                                }
                            }

                            ///////////////////////////////////////////////////////////
                            if (g_datum_mark == 0) // 基准点
                            {
                                g_num_mov_flag = 1;
                                g_para1 = para + 25; //28
                                pno = MENU_MODIFYADD;
                                poldno[pnoindex] = MENU_CONVERT1;
                                pnoindex++;
                            }
                            else
                            {
                                dis_updata = 0x11;
                                g_num_mov_flag = 3;
                                if (g_datum_mark == 2)
                                { //机械原点
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                }
                                else //花样中心
                                {
                                    if (F_FILETYPE == PAT_MIT)
                                    {
                                        Mit_Draw_Centre((S32 *)(&g_dxpos), (S32 *)(&g_dypos));
                                    }
                                    else if (F_FILETYPE == PAT_SEW)
                                    {
                                        g_dxpos = (g_patd.patmX + g_patd.patiX) / 2;
                                        g_dypos = (g_patd.patmY + g_patd.patiY) / 2;
                                    }
                                }
                            }
                        }
                        else
                        { //生成花样数据

                            if (F_FILETYPE == PAT_MIT)
                            {
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    if (g_datum_mark == 0)
                                    {
                                        g_tdxpos = g_dxpos;
                                        g_tdypos = g_dypos;
                                    }
                                    else
                                    {
                                        g_tdxpos = 0;
                                        g_tdypos = 0;
                                    }
                                    if (g_tdxpos != 0 || g_tdypos != 0)
                                    {
                                        Comm_RelationMove(-g_tdxpos, -g_tdypos, MENU_CONVERT1, 0, 0);
                                    }
                                    //printf("- g_dxpos  = %d  g_dypos = %d  g_datum_mark = %d\r\n",g_dxpos,g_dypos,g_datum_mark);
                                    g_patd.pat_add = 52000;
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }
                                f = Mit_Convert_Data(para);
                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    if (g_jog_point > g_patd.feed_sum)
                                        g_jog_point = 0;
                                    if (P_FHP)
                                    {
                                    }
                                    else
                                    {
                                        g_patd.pat_add = 52000;
                                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 0, 0); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }

                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            {
                                if (P_FHP)
                                { //按变换前花样回原点
                                    if (g_datum_mark == 0)
                                    {
                                        g_tdxpos = mult1 * g_dxpos;
                                        g_tdypos = mult1 * g_dypos;
                                    }
                                    else
                                    {
                                        g_tdxpos = 0;
                                        g_tdypos = 0;
                                    }
                                    if (g_tdxpos != 0 || g_tdypos != 0)
                                    {
                                        Comm_RelationMove(-g_tdxpos, -g_tdypos, MENU_CONVERT1, 1, 0);
                                    }

                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }
                                if (g_datum_mark == 0)
                                { // 基准点
                                    f = Bor_ConZoom(tmp_selectmode + 1, g_convert_xscale, g_convert_yscale, g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, g_convert_pindis, ConAng, g_patd.pat_len);
                                }
                                else
                                {
                                    f = Bor_ConZoom(tmp_selectmode + 1, g_convert_xscale, g_convert_yscale, g_dxpos, g_dypos, g_convert_pindis, ConAng, g_patd.pat_len);
                                }

                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }

                                if (f == 1)
                                { //成功
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);

                                    if (P_FHP)
                                    {
                                    }
                                    else
                                    {
                                        if (g_jog_point > g_patd.feed_sum)
                                            g_jog_point = 0;
                                        bor_getPointXY(g_jog_point, g_patd.pat_len, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 1, 1); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }

                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                                else if (f == 2)
                                { //图像超范围
                                    //										g_modifyflag = 1;
                                    //										g_Tsavetimes = g_savetimes;
                                    //										g_Tsavetimes++;
                                    //										g_savetimes = g_Tsavetimes;
                                    //										BackupSREG(g_Tsavetimes,0,g_patd.pat_len,1);
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, F_FILETYPE, (F_FILELEN));
                                    //g_jog_point =0;
                                    //Comm_Move(0,0,50,MENU_CONVERT1,1,1);	//移动到原点
                                    //Wait_Move_End();//等待平台移动结束
                                    pnoindex = 1;
                                    pno = MENU_RWHELP;
                                    hleppara = 65;
                                    poldno[pnoindex] = MENU_CONVERT;
                                    pnoindex++;
                                }
                                else
                                {
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, F_FILETYPE, (F_FILELEN));
                                    pnoindex = 1;
                                    pno = MENU_RWHELP;
                                    hleppara = 20;
                                    poldno[pnoindex] = MENU_CONVERT;
                                    pnoindex++;
                                }
                            }
                        }
                    }
                    else if (para == CONV_MIRROR) //图形对称
                    {
                        g_selectmode = tmp_selectmode;
                        g_parameter = tmp_parameter;
                        if (F_FILETYPE == PAT_MIT)
                        { //三菱
                            if (P_FHP)
                            { //先按照原花园回到原点
                                g_patd.pat_add = 52000;
                                tjog_point = g_jog_point;
                                Go_Home(MENU_CONVERT1, 1);
                                g_jog_point = tjog_point;
                            }
                            f = Mit_Convert_Data(para);

                            if (P_FHP)
                            {
                                g_jog_point = 0;
                            }
                            if (f == 1)
                            {
                                if (g_jog_point > g_patd.feed_sum)
                                    g_jog_point = 0;
                                if (P_FHP)
                                {
                                }
                                else
                                {
                                    g_patd.pat_add = 52000;
                                    mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                    Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                    Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 0, 0); //移动到当前坐标
                                    Wait_Move_End();                                            //等待平台移动结束
                                }
                                pno = MENU_INHELP;
                                hleppara = 6;
                                poldno[pnoindex] = MENU_CONVERT1;
                                pnoindex++;
                            }
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        { //兄弟
                            if (P_FHP)
                            { //按变换前花样回原点
                                tjog_point = g_jog_point;
                                Go_Home(MENU_CONVERT1, 1);
                                g_jog_point = tjog_point;
                            }
                            f = Bor_ConMirr(tmp_selectmode + 1, tmp_parameter + 1, g_patd.pat_len);

                            if (P_FHP)
                            {
                                g_jog_point = 0;
                            }
                            if (f == 1)
                            {
                                g_modifyflag = 1;
                                g_Tsavetimes = g_savetimes;
                                g_Tsavetimes++;
                                g_savetimes = g_Tsavetimes;
                                BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                if (P_FHP)
                                {
                                }
                                else
                                {
                                    if (g_jog_point > g_patd.feed_sum)
                                        g_jog_point = 0;
                                    bor_getPointXY(g_jog_point, g_patd.pat_len, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                    Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                    Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 1, 1); //移动到当前坐标
                                    Wait_Move_End();                                            //等待平台移动结束
                                }
                                pno = MENU_INHELP;
                                hleppara = 6;
                                poldno[pnoindex] = MENU_CONVERT1;
                                pnoindex++;
                                ;
                            }
                            else if (f == 2)
                            { //图像超范围
                                //										g_modifyflag = 1;
                                //										g_Tsavetimes = g_savetimes;
                                //										g_Tsavetimes++;
                                //										g_savetimes = g_Tsavetimes;
                                //										BackupSREG(g_Tsavetimes,0,g_patd.pat_len,1);
                                RecovrSREG(g_savetimes, 1);
                                AnaPattern(0, F_FILETYPE, (F_FILELEN));
                                //g_jog_point =0;
                                //Comm_Move(0,0,50,MENU_CONVERT1,1,1);	//移动到原点
                                //Wait_Move_End();//等待平台移动结束
                                pno = MENU_RWHELP;
                                hleppara = 65;
                                poldno[pnoindex] = MENU_CONVERT1;
                                pnoindex++;
                                //Go_Home(MENU_CONVERT1,0);
                            }
                        }
                    }
                    else if (para == CONV_ROTATE) //旋转
                    {
                        if (g_num_mov_flag == 1)
                        {
                            g_num_mov_flag = 3;
                            dis_updata = 0x11;
                            dis_updata2 = 1;
                        }
                        else if (g_num_mov_flag == 2)
                        {
                            g_selectmode = tmp_selectmode;
                            g_parameter = tmp_parameter;
                            if (g_datum_mark == 0)
                            { //基准点
                                g_num_mov_flag = 1;
                                g_para1 = para + 25;
                                pno = MENU_MODIFYADD;
                                poldno[pnoindex] = MENU_CONVERT1;
                                pnoindex++;
                            }
                            else
                            {
                                g_num_mov_flag = 3;
                                dis_updata = 0x11;
                                if (g_datum_mark == 2)
                                { //机械原点
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                }
                                else
                                { //花样中心
                                    if (F_FILETYPE == PAT_MIT)
                                    { //三菱
                                        Mit_Draw_Centre((S32 *)(&g_dxpos), (S32 *)(&g_dypos));
                                    }
                                    else if (F_FILETYPE == PAT_SEW)
                                    { //兄弟
                                        g_dxpos = (g_patd.patmX + g_patd.patiX) / 2;
                                        g_dypos = (g_patd.patmY + g_patd.patiY) / 2;
                                    }
                                }
                            }
                        }
                        else
                        { //生成花样
                            if (F_FILETYPE == PAT_MIT)
                            { //三菱
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    if (g_datum_mark == 0)
                                    {
                                        g_tdxpos = g_dxpos;
                                        g_tdypos = g_dypos;
                                    }
                                    else
                                    {
                                        g_tdxpos = 0;
                                        g_tdypos = 0;
                                    }
                                    if (g_tdxpos != 0 || g_tdypos != 0)
                                    {
                                        Comm_RelationMove(-g_tdxpos, -g_tdypos, MENU_CONVERT1, 0, 0);
                                    }

                                    g_patd.pat_add = 52000;
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }
                                f = Mit_Convert_Data(para);
                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    if (g_jog_point > g_patd.feed_sum)
                                        g_jog_point = 0;
                                    if (P_FHP)
                                    {
                                    }
                                    else
                                    {
                                        g_patd.pat_add = 52000;
                                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 0, 0); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }

                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            { //兄弟
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    //printf("- g_dxpos  = %d  g_dypos = %d  g_datum_mark = %d\r\n",g_dxpos,g_dypos,g_datum_mark);
                                    if (g_datum_mark == 0)
                                    {
                                        g_tdxpos = mult1 * g_dxpos;
                                        g_tdypos = mult1 * g_dypos;
                                    }
                                    else
                                    {
                                        g_tdxpos = 0;
                                        g_tdypos = 0;
                                    }
                                    if (g_tdxpos != 0 || g_tdypos != 0)
                                    {
                                        Comm_RelationMove(-g_tdxpos, -g_tdypos, MENU_CONVERT1, 1, 0);
                                    }

                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }

                                if (g_datum_mark == 0)
                                { // 基准点
                                    f = Bor_ConSpin(tmp_selectmode + 1, g_parameter, g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, g_patd.pat_len);
                                }
                                else
                                {
                                    f = Bor_ConSpin(tmp_selectmode + 1, g_parameter, g_dxpos, g_dypos, g_patd.pat_len);
                                }
                                //printf("Bor_ConSpin = %d \r\n",f);
                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                    if (P_FHP)
                                    { //按变换前花样回原点
                                    }
                                    else
                                    {
                                        if (g_jog_point > g_patd.feed_sum)
                                            g_jog_point = 0;
                                        bor_getPointXY(g_jog_point, g_patd.pat_len, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 1, 1); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }
                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                    ;
                                }
                                else if (f == 2)
                                { //图像超范围
                                    //										g_modifyflag = 1;
                                    //										g_Tsavetimes = g_savetimes;
                                    //										g_Tsavetimes++;
                                    //										g_savetimes = g_Tsavetimes;
                                    //										BackupSREG(g_Tsavetimes,0,g_patd.pat_len,1);
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, F_FILETYPE, (F_FILELEN));

                                    pno = MENU_RWHELP;
                                    hleppara = 65;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                    //g_dxpos =0;
                                    //g_dypos =0;
                                    //g_jog_point =0;
                                    //Comm_Move(0,0,50,MENU_CONVERT1,1,1);	//移动到原点
                                    //Wait_Move_End();//等待平台移动结束
                                }
                                else
                                {
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, PAT_SEW, (F_FILELEN));
                                    pno = MENU_RWHELP;
                                    hleppara = 20;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                            }
                        }
                    }
                    else if (para == CONV_OFFSET) //辅助缝
                    {
                        if (g_num_mov_flag == 0)
                        {
                            g_num_mov_flag = 2;
                            dis_updata = 0x11;
                            g_direction = g_array4[g_selectmode * 2][0];
                            g_distance = g_array4[g_selectmode * 2][1];
                            tmp_parameter = g_distance;
                        }
                        else if (g_num_mov_flag == 2) //生成花样
                        {
                            if (F_FILETYPE == PAT_MIT)
                            { //三菱
                                g_distance = tmp_parameter;
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    g_patd.pat_add = 52000;
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }

                                f = Mit_Convert_Data(para);
                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    if (g_jog_point > g_patd.feed_sum)
                                        g_jog_point = 0;
                                    if (P_FHP)
                                    {
                                    }
                                    else
                                    {
                                        g_patd.pat_add = 52000;
                                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 0, 0); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }

                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            { //兄弟
                                if (editflag == 0)
                                {
                                    if (tmp_parameter > 200)
                                        tmp_parameter = 200;
                                    Convedis = tmp_parameter;
                                }
                                else if (editflag == 1)
                                {
                                    if (tmp_parameter > 127)
                                        tmp_parameter = 127;
                                    if (tmp_parameter == 0)
                                        tmp_parameter = 1;
                                    g_convert_pindis = tmp_parameter;
                                }
                                else if (editflag == 2)
                                {
                                    if (tmp_parameter > 60)
                                        tmp_parameter = 30;
                                    if (tmp_parameter < 20)
                                        tmp_parameter = 30;
                                    ConAng = tmp_parameter;
                                }
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }
                                //printf("selectmode = %d direction = %d tmp_selectmode = %d  tmp_parameter = %d \r\n",selectmode,direction,tmp_selectmode,tmp_parameter);
                                f = Bor_ConveAss(g_direction + 1, g_selectmode, Convedis, g_convert_pindis, ConAng, F_FILELEN, &g_patd.pat_len);

                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                    if (P_FHP)
                                    { //按变换前花样回原点
                                    }
                                    else
                                    {
                                        if (g_jog_point > g_patd.feed_sum)
                                            g_jog_point = 0;
                                        bor_getPointXY(g_jog_point, g_patd.pat_len, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 1, 1); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }
                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                                else if (f == 2)
                                { //图像超范围
                                    //										g_modifyflag = 1;
                                    //										g_Tsavetimes = g_savetimes;
                                    //										g_Tsavetimes++;
                                    //										g_savetimes = g_Tsavetimes;
                                    //										BackupSREG(g_Tsavetimes,0,g_patd.pat_len,1);
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, F_FILETYPE, (F_FILELEN));
                                    pnoindex = 1;
                                    pno = MENU_RWHELP;
                                    hleppara = 65;
                                    poldno[pnoindex] = MENU_CONVERT;
                                    pnoindex++;
                                    //g_jog_point =0;
                                    //Comm_Move(0,0,50,MENU_CONVERT1,1,1);	//移动到原点
                                    //Wait_Move_End();//等待平台移动结束
                                }
                                else
                                {
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, PAT_SEW, (F_FILELEN));
                                    pnoindex = 1;
                                    pno = MENU_RWHELP;
                                    hleppara = 20;
                                    poldno[pnoindex] = MENU_CONVERT;
                                    pnoindex++;
                                }
                            }
                        }
                    }
                    else if (para == CONV_OFFSMULT) //多重缝
                    {
                        if (g_num_mov_flag == 0)
                        {
                            g_num_mov_flag = 2;
                            dis_updata = 0x11;
                            g_para_offset = g_array4[g_selectmode * 3][0];
                            g_direction = g_array4[g_selectmode * 3 + 1][0];
                            g_distance = g_array4[g_selectmode * 3 + 1][1];
                            if (F_FILETYPE == PAT_MIT)
                            { //三菱
                                g_times = g_array4[g_selectmode * 3][1];
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            { //兄弟
                                g_times = 2;
                            }
                            tmp_parameter = g_distance;
                        }
                        else if (g_num_mov_flag == 2)
                        { //生成花样
                            if (F_FILETYPE == PAT_MIT)
                            { //三菱
                                if (editflag == 0)
                                {
                                    if (tmp_parameter > 200)
                                        tmp_parameter = 200;
                                    g_distance = tmp_parameter;
                                }
                                else if (editflag == 1)
                                {
                                    if (tmp_parameter > 9)
                                        tmp_parameter = 9;
                                    if (tmp_parameter < 2)
                                        tmp_parameter = 2;
                                    g_times = tmp_parameter;
                                }
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    g_patd.pat_add = 52000;
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }

                                f = Mit_Convert_Data(para);

                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    if (g_jog_point > g_patd.feed_sum)
                                        g_jog_point = 0;
                                    if (P_FHP)
                                    {
                                    }
                                    else
                                    {
                                        g_patd.pat_add = 52000;
                                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 0, 0); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }

                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            { //兄弟
                                if (editflag == 0)
                                {
                                    if (tmp_parameter > 200)
                                        tmp_parameter = 200;
                                    g_distance = tmp_parameter;
                                }
                                else if (editflag == 1)
                                {
                                    if (tmp_parameter > 9)
                                        tmp_parameter = 9;
                                    if (tmp_parameter < 2)
                                        tmp_parameter = 2;
                                    g_times = tmp_parameter;
                                }
                                else if (editflag == 2)
                                {
                                    if (tmp_parameter > 127)
                                        tmp_parameter = 127;
                                    if (tmp_parameter == 0)
                                        tmp_parameter = 1;
                                    g_convert_pindis = tmp_parameter;
                                }
                                else if (editflag == 3)
                                {
                                    if (tmp_parameter > 60)
                                        tmp_parameter = 30;
                                    if (tmp_parameter < 20)
                                        tmp_parameter = 30;
                                    ConAng = tmp_parameter;
                                }
                                //printf("para_offset = %d selectmode = %d direction = %d  distance = %d  times = %d\r\n",para_offset,selectmode,direction,distance,times);
                                //printf("distance = %d convert_pindis = %d times = %d \r\n",distance,convert_pindis,times);
                                if (P_FHP)
                                { //先按照原花园回到原点
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }
                                f = Bor_ConveRepe(g_para_offset + 1, g_direction + 1, g_selectmode, g_distance, g_times, g_convert_pindis, ConAng, F_FILELEN);
                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                if (f == 1)
                                {
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                    if (P_FHP)
                                    { //按变换前花样回原点
                                    }
                                    else
                                    {
                                        if (g_jog_point > g_patd.feed_sum)
                                            g_jog_point = 0;
                                        bor_getPointXY(g_jog_point, g_patd.pat_len, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                        Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 1, 1); //移动到当前坐标
                                        Wait_Move_End();                                            //等待平台移动结束
                                    }
                                    pno = MENU_INHELP;
                                    hleppara = 6;
                                    poldno[pnoindex] = MENU_CONVERT1;
                                    pnoindex++;
                                }
                                else if (f == 2)
                                { //图像超范围
                                    //										g_modifyflag = 1;
                                    //										g_Tsavetimes = g_savetimes;
                                    //										g_Tsavetimes++;
                                    //										g_savetimes = g_Tsavetimes;
                                    //										BackupSREG(g_Tsavetimes,0,g_patd.pat_len,1);
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, F_FILETYPE, (F_FILELEN));

                                    //if(g_jog_point>g_patd.feed_sum )g_jog_point = 0;
                                    //g_jog_point =0;
                                    //bor_getPointXY(g_jog_point,g_patd.pat_len,(S16*)&g_tmpoxpos,(S16*)&g_tmpoypos);
                                    //Send_Pfoot(1,MENU_CONVERT1,1);							//压脚升
                                    //Comm_Move(g_tmpoxpos,g_tmpoypos,50,MENU_CONVERT1,1,1);	//移动到当前坐标
                                    //Wait_Move_End();//等待平台移动结束
                                    //Go_Home(MENU_CONVERT1,1);	//0
                                    pnoindex = 1;
                                    pno = MENU_RWHELP;
                                    hleppara = 65;
                                    poldno[pnoindex] = MENU_CONVERT;
                                    pnoindex++;
                                }
                                else
                                {
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, PAT_SEW, (F_FILELEN));
                                    pnoindex = 1;
                                    pno = MENU_RWHELP;
                                    hleppara = 20;
                                    poldno[pnoindex] = MENU_CONVERT;
                                    pnoindex++;
                                }
                            }
                        }
                    }
                    else if (para == CONV_CHANGE && F_FILETYPE == PAT_SEW)
                    { //首尾交换
                        if (P_FHP)
                        { //先按照原花园回到原点
                            tjog_point = g_jog_point;
                            Go_Home(MENU_CONVERT1, 1);
                            g_jog_point = tjog_point;
                        }
                        f = Bor_ConveStartEnd(g_array5[g_selectmode][0], F_FILELEN);
                        if (P_FHP)
                        {
                            g_jog_point = 0;
                        }
                        if (f == 1)
                        {
                            g_modifyflag = 1;
                            g_Tsavetimes = g_savetimes;
                            g_Tsavetimes++;
                            g_savetimes = g_Tsavetimes;
                            BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                            if (P_FHP)
                            { //按变换前花样回原点
                            }
                            else
                            {
                                if (g_jog_point > g_patd.feed_sum)
                                    g_jog_point = 0;
                                bor_getPointXY(g_jog_point, g_patd.pat_len, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                                Send_Pfoot(1, MENU_CONVERT1, 1);                            //压脚升
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT1, 0, 0); //移动到当前坐标
                                Wait_Move_End();                                            //等待平台移动结束
                            }
                            pno = MENU_INHELP;
                            hleppara = 6;
                            poldno[pnoindex] = MENU_CONVERT1;
                            pnoindex++;
                        }
                        else
                        {
                            RecovrSREG(g_savetimes, 1);
                            AnaPattern(0, PAT_SEW, (F_FILELEN));
                            pnoindex = 1;
                            pno = MENU_RWHELP;
                            hleppara = 20;
                            poldno[pnoindex] = MENU_CONVERT;
                            pnoindex++;
                        }
                    }
                    break;
                // case KEY_CONVERT1_PAT:
                //     pno = MENU_MOIMAGE;
                //     poldno[pnoindex] = MENU_CONVERT1;
                //     pnoindex++;
                //     g_selectmode = tmp_selectmode;
                //     g_distance = tmp_parameter;
                //     if (para == 0 || para == 3 || para == 5)
                //     { //原点修正,图形放大,图形旋转
                //         g_imgpara = 100;
                //     }
                //     break;
                case KEY_CONVERT1_VIEW:
                    // AnaPattern(0, g_GreatPatType, g_CtPatLen);
                    pno = MENU_MOIMAGE;
                    g_imgpara = MOIMAGE_CVIEW; //新增
                    poldno[pnoindex++] = MENU_CONVERT1;
                    break;
                case KEY_CONVERT1_PFOOT:
                    Send_Pfoot(0, MENU_CONVERT1, 0);
                    //if(pfootf)pfootf = 0;else pfootf = 1;
                    break;
                case KEY_CONVERT1_CANCEL:
                    if (g_num_mov_flag == 1)
                    {
                        if (g_dxpos != 0 || g_dypos != 0)
                        {
                            g_dxpos = 0;
                            g_dypos = 0;

                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_CONVERT, ((F_FILETYPE == PAT_MIT) ? 0 : 1), 0);
                            dis_updata2 = 1;
                            dis_updata15 = 1;
                        }
                    }
                    else
                    {
                        if (para == 3)
                        {
                            if (editflag < 2)
                            {
                                tmp_parameter = 10;
                                // DrawFillRect(xx, yy, 65, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 5, 1, Color_White);
                                dis_updata3 = 1;
                            }
                            else if (editflag == 2)
                            {
                                tmp_parameter = 1;
                                // DrawFillRect(xx, yy, 65, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                                dis_updata3 = 1;
                            }
                            else if (editflag == 3)
                            {
                                tmp_parameter = 30;
                                // DrawFillRect(xx, yy, 32, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 2, 0, Color_White);
                                dis_updata3 = 1;
                            }
                        }
                        else if (para == 5)
                        {
                            tmp_parameter = 1;
                            // DrawFillRect(xx, yy, 65, 24, Color_DimGray);
                            // DrawMFS32P(xx, yy, tmp_parameter, 5, 1, Color_White);
                            dis_updata3 = 1;
                        }
                        else if (para == 6)
                        {
                            if (F_FILETYPE == PAT_MIT)
                            {
                                tmp_parameter = 0;
                                // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                                dis_updata3 = 1;
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            {
                                if (editflag == 0)
                                {
                                    tmp_parameter = 0;
                                    // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                                    // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                                    dis_updata3 = 1;
                                }
                                else if (editflag == 1)
                                {
                                    tmp_parameter = 1;
                                    // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                                    // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                                    dis_updata3 = 1;
                                }
                                else if (editflag == 2)
                                {
                                    tmp_parameter = 30;
                                    // DrawFillRect(xx, yy, 32, 24, Color_DimGray);
                                    // DrawMFS32P(xx, yy, tmp_parameter, 2, 0, Color_White);
                                    dis_updata3 = 1;
                                }
                            }
                        }
                        else if (para == 7)
                        {
                            if (editflag == 0)
                            {
                                tmp_parameter = 0;
                                // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                                dis_updata3 = 1;
                            }
                            else if (editflag == 1)
                            {
                                tmp_parameter = 2;
                                // DrawFillRect(xx, yy, 12, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 1, 0, Color_White);
                                dis_updata3 = 1;
                            }
                            if (editflag == 2)
                            {
                                tmp_parameter = 1;
                                // DrawFillRect(xx, yy, 52, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 4, 1, Color_White);
                                dis_updata3 = 1;
                            }
                            else if (editflag == 3)
                            {
                                tmp_parameter = 30;
                                // DrawFillRect(xx, yy, 32, 24, Color_DimGray);
                                // DrawMFS32P(xx, yy, tmp_parameter, 2, 0, Color_White);
                                dis_updata3 = 1;
                            }
                        }
                    }
                    break;
                case KEY_CONVERT1_MMSP:

                    break;
                case KEY_CONVERT1_6PREV:
                case KEY_CONVERT1_7PREV:
                case KEY_CONVERT1_8PREV:
                    if (g_selectmode > 1)
                    {
                        g_selectmode--;
                        dis_updata = 0x01;
                    }
                    break;
                case KEY_CONVERT1_6NEXT:
                case KEY_CONVERT1_7NEXT:
                case KEY_CONVERT1_8NEXT:

                    if (g_selectmode < tmp_selectmode)
                    {
                        g_selectmode++;
                        dis_updata = 0x01;
                    }
                    break;
                case KEY_CONVERT1_DUP:

                    break;
                case KEY_CONVERT1_DDOWN:

                    break;
                case KEY_CONVERT1_9ANG:
                    if (para == 3)
                    { //放大缩小
                        if (editflag != 3)
                        {
                            oldeditflag = editflag;
                            editflag = 3;
                            if (oldeditflag == 0)
                            {
                                if (tmp_parameter < 10)
                                    tmp_parameter = 1000;
                                g_convert_xscale = tmp_parameter;
                            }
                            else if (oldeditflag == 1)
                            {
                                if (tmp_parameter < 10)
                                    tmp_parameter = 1000;
                                g_convert_yscale = tmp_parameter;
                            }
                            else if (oldeditflag == 2)
                            {
                                if (tmp_parameter > 127)
                                    tmp_parameter = 127;
                                g_convert_pindis = tmp_parameter;
                            }
                        }
                    }
                    else if (para == 6)
                    { //离边
                        if (editflag != 2)
                        {
                            oldeditflag = editflag;
                            editflag = 2;
                            if (oldeditflag == 0)
                            {
                                if (tmp_parameter > 200)
                                    tmp_parameter = 200;
                                Convedis = tmp_parameter;
                            }
                            else if (oldeditflag == 1)
                            {
                                if (tmp_parameter > 127)
                                    tmp_parameter = 127;
                                if (tmp_parameter == 0)
                                    tmp_parameter = 1;
                                g_convert_pindis = tmp_parameter;
                            }
                        }
                    }
                    else if (para == 7)
                    { //多重缝
                        if (editflag != 3)
                        {
                            oldeditflag = editflag;
                            editflag = 3;
                            if (oldeditflag == 0)
                            {
                                if (tmp_parameter > 200)
                                    tmp_parameter = 200;
                                g_distance = tmp_parameter;
                            }
                            else if (oldeditflag == 1)
                            {
                                if (tmp_parameter < 2)
                                    tmp_parameter = 2;
                                if (tmp_parameter > 9)
                                    tmp_parameter = 9;
                                g_times = tmp_parameter;
                            }
                            else if (oldeditflag == 2)
                            {
                                if (tmp_parameter > 127)
                                    tmp_parameter = 127;
                                if (tmp_parameter < 1)
                                    tmp_parameter = 1;
                                g_convert_pindis = tmp_parameter;
                            }
                        }
                    }

                    tmp_parameter = ConAng;
                    dis_updata = 0x08;
                    break;

                case KEY_CONVERT1_ZOUT: //图形缩小
                    if (scale1)
                    {
                        scale1 -= 50;
                        dis_updata2 = 2;
                    }
                    else
                    {
                        if (bResetCaonvas)
                        {
                            bResetCaonvas = 0;
                            xo = patConvas.conOrgX;
                            yo = patConvas.conOrgY;
                            scale1 = 0;
                            vflag = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            dis_updata2 = 2;
                        }
                    }
                    break;

                case KEY_CONVERT1_ZIN: //图形放大
                    scale1 += 50;
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_CONVERT1_IMG: //图形移动
                    if (beeponf > 1)
                    {
                        if ((absInt(keyxpct - okeyxpct) <= 10) && (absInt(keyypct - okeyypct) <= 10) && (absInt(keyxpct - stkeyxpct) <= 10) && (absInt(keyypct - stkeyypct) <= 10) && (absInt(okeyxpct - stkeyxpct) <= 10) && (absInt(okeyypct - stkeyypct) <= 10) && (beeponf == 15))
                        {
                            xo = ORG_X;
                            yo = ORG_Y;
                            scale1 = 0;
                            vflag = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            Beep_On(BELLT);
                        }
                        else
                        {
                            xo += (keyxpct - okeyxpct);
                            yo += (keyypct - okeyypct);
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            if (vflag == 1)
                                vflag = 2;
                        }
                    }
                    else
                    {
                        okeyxpct = keyxpct;
                        okeyypct = keyypct;
                        stkeyxpct = keyxpct;
                        stkeyypct = keyypct;
                    }

                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_CONVERT1_TOP:
                    tmpxpos = g_dxpos;
                    tmpypos = g_dypos;

                    g_dxpos = (nearpx - g_tmpoxpos) / mult1;
                    g_dypos = (nearpy - g_tmpoypos) / mult1;

                    f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, 50, MENU_MODIFY1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);

                    if (f == 0)
                    {
                        g_dxpos = tmpxpos;
                        g_dypos = tmpypos;
                    }

                    dis_updata15 = 1;

                    posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
                    posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
                    if (((posx1 < keyd[KEY_CONVERT1_IMG].xpis) || (posx1 > keyd[KEY_CONVERT1_IMG].xpis + keyd[KEY_CONVERT1_IMG].xsize) || (posy1 < keyd[KEY_CONVERT1_IMG].ypis) || (posy1 > keyd[KEY_CONVERT1_IMG].ypis + keyd[KEY_CONVERT1_IMG].ysize)))
                    { //超出显示范围
                        dis_updata2 = 1;
                    }
                    else
                    {
                        RecoveData2();
                        ReadPointData2(posx1 - 4, posy1 - 4);
                        // ViewPoint(posx1, posy1, COLOR_CROSS, 1);
                        Draw_SmallCrossLimit(posx1, posy1, COLOR_CROSS, patRule.h_patConvasIndex);
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
            if (keyon == KEY_CONVERT1_SCRNSVR || keyon == KEY_CONVERT1_IMG) //不需要显示的按键
            {
            }
            //非常规按键-//绘制颜色文字按键
            else if (keyon == KEY_CONVERT1_3XSCALE || keyon == KEY_CONVERT1_3YSCALE ||
                     keyon == KEY_CONVERT1_3PITCH || keyon == KEY_CONVERT1_3ANG ||
                     keyon == KEY_CONVERT1_6DIS || keyon == KEY_CONVERT1_6PITCH ||
                     keyon == KEY_CONVERT1_7SELDIST || keyon == KEY_CONVERT1_7SELTIME || keyon == KEY_CONVERT1_7PITCH ||
                     keyon == KEY_CONVERT1_9ANG)
            {
                DrawButtonOnZoomInRes(keyd[keyon]);
            }
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            pressKey = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                         SheetPage, title, rect[0].colorT[0]); //标题
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_CONVERT1);
        }
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_CONVERT1);
        }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************

        // /[吸附按钮刷新*begin]**********************************************************
        if (getNearflag && pressKey == 0)
        {

            S32 topPosX = 0, topPosY = 0; //吸附点坐标-屏幕坐标系
            if (getNearflag == 1)
            {
                tonear = getNearPointPos(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, &nearpx, &nearpy);

                topPosX = (S32)(nearpx * (vscale) / 1000 + xorg);
                topPosY = (S32)(-nearpy * (vscale) / 1000 + yorg);
            }

            if (tonear != otonear)
            {
                if (tonear)
                {

                    DrawButton(keyd[KEY_CONVERT1_TOP], (U16 *)(ADD_ICONBMP),
                               keybmpind[KEY_CONVERT1_TOP].border[0], keybmpind[KEY_CONVERT1_TOP].icon[0]);
                    DrawFChar(keyd[KEY_CONVERT1_TOP].xpis + SAFE_DIST4, keyd[KEY_CONVERT1_TOP].ypis + SAFE_DIST9,
                              "X", keybmpind[KEY_CONVERT1_TOP].textColor[0]);
                    DrawFChar(keyd[KEY_CONVERT1_TOP].xpis + SAFE_DIST4, keyd[KEY_CONVERT1_TOP].ypis + SAFE_DIST9 + TYPE_SIZE26,
                              "Y", keybmpind[KEY_CONVERT1_TOP].textColor[0]);

                    Timer_ms1 = 0;

                    // onearpx = nearpx;
                    // onearpy = nearpy;
                }
                else
                {
                    BmpFillRect(keyd[KEY_CONVERT1_TOP].xpis, keyd[KEY_CONVERT1_TOP].ypis, keyd[KEY_CONVERT1_TOP].xsize, keyd[KEY_CONVERT1_TOP].ysize);
                    pat_setPatRule(patRulesTmp);
                    pat_clearConvas(patRule.h_patConvasIndex);
                    pat_drawPattern(patRulesTmp, scale1, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
                    nearpx = 0;
                    nearpy = 0;
                }
                otonear = tonear;
            }

            if (tonear)
            {
                if (onearpx != nearpx || onearpy != nearpy)
                {

                    DrawFillRect(keyd[KEY_CONVERT1_TOP].xpis + SAFE_DIST4 + TYPE_SIZE1,
                                 keyd[KEY_CONVERT1_TOP].ypis + SAFE_DIST4 + TYPE_SIZE4,
                                 (keyd[KEY_CONVERT1_TOP].xsize - SAFE_DIST4) - (SAFE_DIST4 + TYPE_SIZE1),
                                 (keyd[KEY_CONVERT1_TOP].ysize - SAFE_DIST4) - (SAFE_DIST4 + TYPE_SIZE4),
                                 keybmpind[KEY_CONVERT1_TOP].textColor[1]);

                    onearpx = nearpx;
                    onearpy = nearpy;
                    pat_setPatRule(patRulesTmp);
                    pat_clearConvas(patRule.h_patConvasIndex);
                    pat_drawPattern(patRulesTmp, scale1, xo, yo, F_FILETYPE, F_FILELEN, topPosX, topPosY, NULL);
                }

#if MOVEUNIT == 1
                if (g_curPTtype == 2)
                {
                    DrawMFS32P(504, 44, nearpx * 5 / mult1, 7, 2, Color_Black);
                    DrawMFS32P(504, 68, nearpy * 5 / mult1, 7, 2, Color_Black);
                }
                else
                {
                    DrawMFS32P(504, 44, nearpx / mult1, 6, 1, Color_Black);
                    DrawMFS32P(504, 68, nearpy / mult1, 6, 1, Color_Black);
                }
#else
                DrawFS32P(keyd[KEY_CONVERT1_TOP].xpis + SAFE_DIST4 + TYPE_SIZE1, keyd[KEY_CONVERT1_TOP].ypis + SAFE_DIST9,
                          nearpx / mult1, 5, 1, keybmpind[KEY_CONVERT1_TOP].textColor[0]);
                DrawFS32P(keyd[KEY_CONVERT1_TOP].xpis + SAFE_DIST4 + TYPE_SIZE1, keyd[KEY_CONVERT1_TOP].ypis + SAFE_DIST9 + TYPE_SIZE26,
                          nearpy / mult1, 5, 1, keybmpind[KEY_CONVERT1_TOP].textColor[0]);
#endif

                //ViewPoint(posx1,posy1,COLOR_CROSS,1);
            }
            getNearflag = 0;
        }
        // /[吸附按钮刷新*end]**********************************************************

        // /[界面显示用的数据初始化*begin]********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
            if (para == CONV_ORGSUB)
            {
                dis_updata2 = 1;

                g_tmpoxpos = 0;
                g_tmpoypos = 0;
                x = g_tmpoxpos + g_dxpos;
                y = g_tmpoypos + g_dypos;
                Comm_Move(x, y, 50, MENU_CONVERT1, 0, 0);
                if (g_selectmode)
                {
                    g_num_mov_flag = 1;
                    tmp_selectmode = 1;
                }
                else
                {
                    g_num_mov_flag = 0;
                    tmp_selectmode = 0;
                }
            }
            else if (para == CONV_SCALE)
            { //放大缩小
                if (g_num_mov_flag == 1)
                {
                }
                else if (g_num_mov_flag == 2)
                {
                    if (F_FILETYPE == PAT_MIT)
                    { //三菱
                        if (editflag == 0)
                        {
                            tmp_parameter = g_convert_xscale;
                        }
                        else
                        {
                            tmp_parameter = g_convert_yscale;
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //兄弟
                        if (editflag == 0)
                        {
                            tmp_parameter = g_convert_xscale;
                        }
                        else if (editflag == 1)
                        {
                            tmp_parameter = g_convert_yscale;
                        }
                        else if (editflag == 2)
                        {
                            tmp_parameter = g_convert_pindis;
                        }
                        else if (editflag == 3)
                        {
                            tmp_parameter = ConAng;
                        }
                    }
                }
            }
            else if (para == CONV_MIRROR)
            { //对称
                if (g_parameter > 2)
                    g_parameter = 0;
                tmp_parameter = g_parameter;
            }
            else if (para == CONV_ROTATE)
            { // 旋转

                if (g_datum_mark > 2)
                    g_datum_mark = 0;
                if ((g_parameter == 0) || (g_parameter > 1800))
                    g_parameter = 1;
                if (g_num_mov_flag == 1)
                {
                }
                else if (g_num_mov_flag == 2)
                {
                    tmp_parameter = g_parameter;
                }
            }
            else if (para == CONV_OFFSET)
            { // 补助缝
                flag = 1;
                if ((g_selectmode == 0) && (tmp_selectmode))
                    g_selectmode = 1;
                else if (g_selectmode > tmp_selectmode)
                {
                    if (tmp_selectmode)
                        g_selectmode = 1;
                    else
                        g_selectmode = 0;
                }
                if (g_direction > 1)
                    g_direction = 0;
                if (g_distance > 200)
                    g_distance = 0;
                tmp_parameter = g_distance;
            }
            else if (para == CONV_OFFSMULT)
            { // 多重缝
                flag = 1;
                if ((g_selectmode == 0) && (tmp_selectmode))
                    g_selectmode = 1;
                else if (g_selectmode > tmp_selectmode)
                {
                    if (tmp_selectmode)
                        g_selectmode = 1;
                    else
                        g_selectmode = 0;
                }
                if (g_para_offset > 3)
                    g_para_offset = 0;
                if (g_direction > 1)
                    g_direction = 0;
                if (g_distance > 200)
                    g_distance = 0;
                if ((g_times < 2) || (g_times > 9))
                    g_times = 2;
                tmp_parameter = g_distance;
                if (editflag > 1)
                    editflag = 0;
            }
            else if (para == CONV_CHANGE)
            {
                dis_updata2 = 1;
            }
        }
        // /[界面显示用的数据初始化*end]********************************************************

        // /[界面显示用的UI初始化*begin]********************************************************
        if (dis_updata)
        {
            if (dis_updata & 0x10)
            {
                BmpFillRect(0, TITLE_YLEN, LCD_WIDTH, LCD_HEIGHT - TITLE_YLEN);
            }
            if (dis_updata & 0x01)
            {
                dis_updata2 = 1;
            }
            if (g_num_mov_flag != tmp_nummovflag)
            {
                tmp_nummovflag = g_num_mov_flag;
                BmpFillRect(keyd[KEY_CONVERT1_MUL].xpis, keyd[KEY_CONVERT1_MUL].ypis,
                            keyd[KEY_CONVERT1_MUL].xsize * 3,
                            keyd[KEY_CONVERT1_MUL].ysize * 3);
                BmpFillRect(keyd[KEY_CONVERT1_1].xpis, keyd[KEY_CONVERT1_1].ypis,
                            keyd[KEY_CONVERT1_1].xsize * 3 + SAFE_DIST5 * 2,
                            keyd[KEY_CONVERT1_1].ysize * 4 + SAFE_DIST5 * 3);
                deletedisf = 0;
                pfootdisf = 0;
                if (g_num_mov_flag == 1)
                {
                    BmpFillRect(keyd[KEY_CONVERT1_1].xpis, keyd[KEY_CONVERT1_1].ypis,
                                keyd[KEY_CONVERT1_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_CONVERT1_1].ysize * 4 + SAFE_DIST5 * 3);
                    for (i = KEY_CONVERT1_MUL; i <= KEY_CONVERT1_MDR; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);

                        if (i == KEY_CONVERT1_MMSP)
                            DrawColor_Gray1(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
                    }
                    deletedisf = 1;
                    pfootdisf = 1;
                }
                else if (g_num_mov_flag == 2)
                {
                    BmpFillRect(keyd[KEY_CONVERT1_MUL].xpis, keyd[KEY_CONVERT1_MUL].ypis,
                                keyd[KEY_CONVERT1_MUL].xsize * 3,
                                keyd[KEY_CONVERT1_MUL].ysize * 3);
                    for (i = KEY_CONVERT1_DUP; i <= KEY_CONVERT1_9; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                    deletedisf = 1;
                    pfootdisf = 0;
                }
            }
            if (dis_updata & 0x01)
            {

                if (pfootdisf)
                {
                    pfootf = S_PFOOT;
                    DrawButton(keyd[KEY_CONVERT1_PFOOT], (U16 *)(ADD_ICONBMP),
                               keybmpind[KEY_CONVERT1_PFOOT].border[0],
                               (pfootf == 0) ? keybmpind[KEY_CONVERT1_PFOOT].icon[0] : keybmpind[KEY_CONVERT1_PFOOT].icon[1]);
                }
                else
                {
                    BmpFillRect(keyd[KEY_CONVERT1_PFOOT].xpis, keyd[KEY_CONVERT1_PFOOT].ypis, keyd[KEY_CONVERT1_PFOOT].xsize, keyd[KEY_CONVERT1_PFOOT].ysize);
                }
            }

            if (para == CONV_ORGSUB) //原点修正
            {
                if ((dis_updata & 0x01))
                {
                    DrawRect(rect[RECT_CONVERT1_0METHOD].xpis, rect[RECT_CONVERT1_0METHOD].ypis,
                             rect[RECT_CONVERT1_0METHOD].xsize, rect[RECT_CONVERT1_0METHOD].ysize, rect[RECT_CONVERT1_0METHOD].colorB[0]);
                    Draw16x16Str(rect[RECT_CONVERT1_0METHOD].xpis + SAFE_DIST3, rect[RECT_CONVERT1_0METHOD].ypis + SAFE_DIST3,
                                 (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_0METHOD].index[0] * TitleLengthMax)),
                                 SheetPage, rect[RECT_CONVERT1_0METHOD].index[0], rect[RECT_CONVERT1_0METHOD].colorT[0]); //画出方框的文字
                }
                if ((dis_updata & 0x01) || (dis_updata & 0x02))
                {
                    for (i = KEY_CONVERT1_0ORGSUBOF; i <= KEY_CONVERT1_0ORGSUBON; i++)
                    {
                        f = i - KEY_CONVERT1_0ORGSUBOF;
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                   (tmp_selectmode == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                    }
                }
            }
            else if (para == CONV_SCALE) //放大缩小
            {

                if (g_num_mov_flag == 1)
                {
                    imgdisf = 0;
                    patdisf = 3;
                    dis_pos = 1;
                    tmp_feedc = g_jog_point;
                }
                else if (g_num_mov_flag == 2)
                {
                    imgdisf = 1;
                    patdisf = 0;
                    dis_pos = 0;
                    if ((F_FILETYPE == PAT_MIT) || (tmp_selectmode == 0))
                    {
                        BmpFillRect(keyd[KEY_CONVERT1_3PITCH].xpis, keyd[KEY_CONVERT1_3PITCH].ypis,
                                    rect[RECT_CONVERT1_3EDIT3_TIT].xsize + rect[RECT_CONVERT1_3EDIT3_STR].xsize * 2,
                                    keyd[KEY_CONVERT1_3PITCH].ysize);
                        BmpFillRect(keyd[KEY_CONVERT1_9ANG].xpis, keyd[KEY_CONVERT1_9ANG].ypis,
                                    rect[RECT_CONVERT1_9ANG_TIT].xsize + rect[RECT_CONVERT1_9ANG_STR].xsize * 2,
                                    keyd[KEY_CONVERT1_9ANG].ysize);
                        if ((editflag == 2) || (editflag == 3))
                        {
                            editflag = 0;
                            tmp_parameter = g_convert_xscale;
                        }
                    }
                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_3METHOD1].xpis, rect[RECT_CONVERT1_3METHOD1].ypis,
                                 rect[RECT_CONVERT1_3METHOD1].xsize, rect[RECT_CONVERT1_3METHOD1].ysize, rect[RECT_CONVERT1_3METHOD1].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_3METHOD1].xpis + SAFE_DIST3, rect[RECT_CONVERT1_3METHOD1].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_3METHOD1].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_3METHOD1].index[0], rect[RECT_CONVERT1_3METHOD1].colorT[0]); //输入方法
                    }
                    if ((dis_updata & 0x01) || (dis_updata & 0x02))
                    {
                        for (i = KEY_CONVERT1_3SSTITCH; i <= KEY_CONVERT1_3SPITCH; i++)
                        {
                            f = i - KEY_CONVERT1_3SSTITCH;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (tmp_selectmode == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }

                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_3METHOD2].xpis, rect[RECT_CONVERT1_3METHOD2].ypis,
                                 rect[RECT_CONVERT1_3METHOD2].xsize, rect[RECT_CONVERT1_3METHOD2].ysize, rect[RECT_CONVERT1_3METHOD2].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_3METHOD2].xpis + SAFE_DIST3, rect[RECT_CONVERT1_3METHOD2].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_3METHOD2].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_3METHOD2].index[0], rect[RECT_CONVERT1_3METHOD2].colorT[0]); //设定中心点
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x04))
                    {
                        for (i = KEY_CONVERT1_3INPUTBS; i <= KEY_CONVERT1_3HOMEBS; i++)
                        {
                            f = i - KEY_CONVERT1_3INPUTBS;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (g_datum_mark == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (g_datum_mark == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x08))
                    {

                        for (i = 0; i < (((F_FILETYPE == PAT_MIT) || (tmp_selectmode == 0)) ? 2 : 3); i++)
                        {
                            //绘制文本框
                            f = KEY_CONVERT1_3XSCALE + i;
                            DrawButtonEdit(keyd[f],
                                           (editflag == i) ? keybmpind[f].textColor[0] : keybmpind[f].icon[0],
                                           (editflag == i) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                            //绘制文本框标题
                            f = RECT_CONVERT1_3EDIT1_TIT + i * 2;
                            DrawEditTit(rect[f], rect[f].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        }

                        DrawF16Str(rect[RECT_CONVERT1_3EDIT1_STR].xpis + rect[RECT_CONVERT1_3EDIT1_STR].xsize + SAFE_DIST5,
                                   rect[RECT_CONVERT1_3EDIT1_STR].ypis + (rect[RECT_CONVERT1_3EDIT1_STR].ysize - 16) / 2,
                                   "(1.0%-400.0%)", rect[RECT_CONVERT1_3EDIT1_STR].colorT[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_3EDIT1_STR].xpis + (rect[RECT_CONVERT1_3EDIT1_STR].xsize - 5 * 13) / 2,
                                   rect[RECT_CONVERT1_3EDIT1_STR].ypis + (rect[RECT_CONVERT1_3EDIT1_STR].ysize - 24) / 2,
                                   (editflag == 0) ? tmp_parameter : g_convert_xscale, 5, 1,
                                   (editflag == 0) ? rect[RECT_CONVERT1_3EDIT1_STR].colorT[0] : rect[RECT_CONVERT1_3EDIT1_STR].colorT[0]);

                        DrawF16Str(rect[RECT_CONVERT1_3EDIT2_STR].xpis + rect[RECT_CONVERT1_3EDIT2_STR].xsize + SAFE_DIST5,
                                   rect[RECT_CONVERT1_3EDIT2_STR].ypis + (rect[RECT_CONVERT1_3EDIT2_STR].ysize - 16) / 2,
                                   "(1.0%-400.0%)", rect[RECT_CONVERT1_3EDIT2_STR].colorT[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_3EDIT2_STR].xpis + (rect[RECT_CONVERT1_3EDIT2_STR].xsize - 5 * 13) / 2,
                                   rect[RECT_CONVERT1_3EDIT2_STR].ypis + (rect[RECT_CONVERT1_3EDIT2_STR].ysize - 24) / 2,
                                   (editflag == 1) ? tmp_parameter : g_convert_yscale, 5, 1,
                                   (editflag == 1) ? rect[RECT_CONVERT1_3EDIT2_STR].colorT[0] : rect[RECT_CONVERT1_3EDIT2_STR].colorT[0]);

                        if ((F_FILETYPE == PAT_SEW) && (tmp_selectmode == 1))
                        {

                            DrawF16Str(rect[RECT_CONVERT1_3EDIT3_STR].xpis + rect[RECT_CONVERT1_3EDIT3_STR].xsize + SAFE_DIST5,
                                       rect[RECT_CONVERT1_3EDIT3_STR].ypis + (rect[RECT_CONVERT1_3EDIT3_STR].ysize - 16) / 2,
                                       "(0.1-12.7)", rect[RECT_CONVERT1_3EDIT3_STR].colorT[0]);
                            DrawMFS32P(rect[RECT_CONVERT1_3EDIT3_STR].xpis + (rect[RECT_CONVERT1_3EDIT3_STR].xsize - 4 * 13) / 2,
                                       rect[RECT_CONVERT1_3EDIT3_STR].ypis + (rect[RECT_CONVERT1_3EDIT3_STR].ysize - 24) / 2,
                                       (editflag == 2) ? tmp_parameter : g_convert_pindis, 4, 1,
                                       (editflag == 2) ? rect[RECT_CONVERT1_3EDIT3_STR].colorT[0] : rect[RECT_CONVERT1_3EDIT3_STR].colorT[0]);

                            //绘制文本框
                            DrawButtonEdit(keyd[KEY_CONVERT1_9ANG],
                                           (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].textColor[0] : keybmpind[KEY_CONVERT1_9ANG].icon[0],
                                           (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].icon[1] : keybmpind[KEY_CONVERT1_9ANG].icon[0]);
                            //绘制文本框标题
                            DrawEditTit(rect[RECT_CONVERT1_9ANG_TIT], rect[RECT_CONVERT1_9ANG_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);

                            DrawF16Str(rect[RECT_CONVERT1_9ANG_STR].xpis + rect[RECT_CONVERT1_9ANG_STR].xsize + SAFE_DIST5,
                                       rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 16) / 2,
                                       "(20-60)", rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                            DrawMFS32P(rect[RECT_CONVERT1_9ANG_STR].xpis + (rect[RECT_CONVERT1_9ANG_STR].xsize - 2 * 13) / 2,
                                       rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 24) / 2,
                                       (editflag == 3) ? tmp_parameter : ConAng, 2, 0,
                                       (editflag == 3) ? rect[RECT_CONVERT1_9ANG_STR].colorT[0] : rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                        }
                    }
                }
                else if (g_num_mov_flag == 3)
                {
                    imgdisf = 0;
                    patdisf = 2;
                    dis_pos = 0;
                    if ((dis_updata & 0x01))
                    {
                        DrawFillRect(rect[RECT_CONVERT1_HINT].xpis, rect[RECT_CONVERT1_HINT].ypis,
                                     rect[RECT_CONVERT1_HINT].xsize, rect[RECT_CONVERT1_HINT].ysize, rect[RECT_CONVERT1_HINT].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_HINT].xpis + SAFE_DIST4, rect[RECT_CONVERT1_HINT].ypis + SAFE_DIST4,
                                     (MEM_TITLE_ADD + (title + 1) * TitleLengthMax), SheetPage, (title + 1),
                                     rect[RECT_CONVERT1_HINT].colorT[0]);

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis,
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 11) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 11), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //输入方法
                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16, rect[RECT_CONVERT1_MESSAGE].ypis,
                                     (MEM_TITLE_ADD + (title + 12 + tmp_selectmode) * TitleLengthMax),
                                     SheetPage, (title + 12 + tmp_selectmode), rect[RECT_CONVERT1_MESSAGE].colorT[0]);

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis + (SAFE_DIST5 + TYPE_SIZE5),
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 18) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 18), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //水平比率
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16, rect[RECT_CONVERT1_MESSAGE].ypis + (SAFE_DIST5 + TYPE_SIZE5),
                                   g_convert_xscale, 5, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16 + 6 * 13,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + (SAFE_DIST5 + TYPE_SIZE5), "%", rect[RECT_CONVERT1_MESSAGE].colorT[0]);

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis + 2 * (SAFE_DIST5 + TYPE_SIZE5),
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 19) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 19), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //垂直比率
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16, rect[RECT_CONVERT1_MESSAGE].ypis + 2 * (SAFE_DIST5 + TYPE_SIZE5),
                                   g_convert_yscale, 5, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16 + 6 * 13,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 2 * (SAFE_DIST5 + TYPE_SIZE5), "%", rect[RECT_CONVERT1_MESSAGE].colorT[0]);

                        if ((F_FILETYPE == PAT_SEW) && (tmp_selectmode == 1))
                        {
                            Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5,
                                         rect[RECT_CONVERT1_MESSAGE].ypis + 3 * (SAFE_DIST5 + TYPE_SIZE5),
                                         (MEM_TITLE_ADD + (TLFNADD_SCALE + 16) * TitleLengthMax),
                                         SheetPage, (TLFNADD_SCALE + 16), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //针距
                            DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16, rect[RECT_CONVERT1_MESSAGE].ypis + 3 * (SAFE_DIST5 + TYPE_SIZE5),
                                       g_convert_pindis, 4, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                            Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16 + 13 * 5,
                                          rect[RECT_CONVERT1_MESSAGE].ypis + 3 * (SAFE_DIST5 + TYPE_SIZE5),
                                          "mm", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        }

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis + 4 * (SAFE_DIST5 + TYPE_SIZE5),
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 15) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 15), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //中心点
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 4 * (SAFE_DIST5 + TYPE_SIZE5), "X:", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16 + 13 * 3,
                                   rect[RECT_CONVERT1_MESSAGE].ypis + 4 * (SAFE_DIST5 + TYPE_SIZE5),
                                   g_dxpos, 7, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 5 * (SAFE_DIST5 + TYPE_SIZE5), "Y:", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE16 + 13 * 3,
                                   rect[RECT_CONVERT1_MESSAGE].ypis + 5 * (SAFE_DIST5 + TYPE_SIZE5),
                                   g_dypos, 7, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        if (g_datum_mark == 0)
                        {
                            Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE8 + 13 * 9,
                                          rect[RECT_CONVERT1_MESSAGE].ypis + 6 * (SAFE_DIST5 + TYPE_SIZE5), "/", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                            DrawMFU32(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE8 + 13 * 3,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 6 * (SAFE_DIST5 + TYPE_SIZE5),
                                      g_jog_point, 5, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                            DrawMFU32(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE8 + 13 * 11,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 6 * (SAFE_DIST5 + TYPE_SIZE5),
                                      tmp_feedsum, 5, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        }
                    }
                }
            }
            else if (para == CONV_MIRROR) //对称
            {
                imgdisf = 1;
                patdisf = 0;
                dis_pos = 0;
                if ((dis_updata & 0x01))
                {
                    DrawRect(rect[RECT_CONVERT1_4METHOD1].xpis, rect[RECT_CONVERT1_4METHOD1].ypis,
                             rect[RECT_CONVERT1_4METHOD1].xsize, rect[RECT_CONVERT1_4METHOD1].ysize, rect[RECT_CONVERT1_4METHOD1].colorB[0]);
                    Draw24x24Str(rect[RECT_CONVERT1_4METHOD1].xpis + SAFE_DIST3, rect[RECT_CONVERT1_4METHOD1].ypis + SAFE_DIST3,
                                 (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_4METHOD1].index[0] * TitleLengthMax)),
                                 SheetPage, rect[RECT_CONVERT1_4METHOD1].index[0], rect[RECT_CONVERT1_4METHOD1].colorT[0]); //镜像来源
                }

                if ((dis_updata & 0x01) || (dis_updata & 0x02))
                {
                    for (i = KEY_CONVERT1_4MIRRORDEL; i <= KEY_CONVERT1_4MIRRORHOLD; i++)
                    {
                        f = i - KEY_CONVERT1_4MIRRORDEL;
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                   (tmp_selectmode == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                    }
                }

                if ((dis_updata & 0x01))
                {
                    DrawRect(rect[RECT_CONVERT1_4METHOD2].xpis, rect[RECT_CONVERT1_4METHOD2].ypis,
                             rect[RECT_CONVERT1_4METHOD2].xsize, rect[RECT_CONVERT1_4METHOD2].ysize, rect[RECT_CONVERT1_4METHOD2].colorB[0]);
                    Draw24x24Str(rect[RECT_CONVERT1_4METHOD2].xpis + SAFE_DIST3, rect[RECT_CONVERT1_4METHOD2].ypis + SAFE_DIST3,
                                 (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_4METHOD2].index[0] * TitleLengthMax)),
                                 SheetPage, rect[RECT_CONVERT1_4METHOD2].index[0], rect[RECT_CONVERT1_4METHOD2].colorT[0]); //输入方法
                }

                if ((dis_updata & 0x01) || (dis_updata & 0x04))
                {
                    for (i = KEY_CONVERT1_4XMIRROR; i <= KEY_CONVERT1_4XYMIRROR; i++)
                    {
                        f = i - KEY_CONVERT1_4XMIRROR;
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (tmp_parameter == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                   (tmp_parameter == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                    }
                }

                if ((dis_updata & 0x01))
                {
                    DrawFillRect(rect[RECT_CONVERT1_HINT].xpis, rect[RECT_CONVERT1_HINT].ypis,
                                 rect[RECT_CONVERT1_HINT].xsize, rect[RECT_CONVERT1_HINT].ysize, rect[RECT_CONVERT1_HINT].colorB[0]);
                    Draw24x24Str(rect[RECT_CONVERT1_HINT].xpis + SAFE_DIST4, rect[RECT_CONVERT1_HINT].ypis + SAFE_DIST4,
                                 (MEM_TITLE_ADD + (title + 1) * TitleLengthMax), SheetPage, (title + 1),
                                 rect[RECT_CONVERT1_HINT].colorT[0]);
                }
            }
            else if (para == CONV_ROTATE) //旋转
            {
                if (g_num_mov_flag == 1)
                {
                    imgdisf = 0;
                    patdisf = 3;
                    dis_pos = 1;
                    tmp_feedc = g_jog_point;
                }
                else if (g_num_mov_flag == 2)
                {
                    imgdisf = 1;
                    patdisf = 0;
                    dis_pos = 0;

                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_5METHOD1].xpis, rect[RECT_CONVERT1_5METHOD1].ypis,
                                 rect[RECT_CONVERT1_5METHOD1].xsize, rect[RECT_CONVERT1_5METHOD1].ysize, rect[RECT_CONVERT1_5METHOD1].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_5METHOD1].xpis + SAFE_DIST3, rect[RECT_CONVERT1_5METHOD1].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_5METHOD1].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_5METHOD1].index[0], rect[RECT_CONVERT1_5METHOD1].colorT[0]); //方向
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x02))
                    {
                        for (i = KEY_CONVERT1_5LROTATE; i <= KEY_CONVERT1_5RROTATE; i++)
                        {
                            f = i - KEY_CONVERT1_5LROTATE;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (tmp_selectmode == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }

                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_5METHOD2].xpis, rect[RECT_CONVERT1_5METHOD2].ypis,
                                 rect[RECT_CONVERT1_5METHOD2].xsize, rect[RECT_CONVERT1_5METHOD2].ysize, rect[RECT_CONVERT1_5METHOD2].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_5METHOD2].xpis + SAFE_DIST3, rect[RECT_CONVERT1_5METHOD2].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_5METHOD2].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_5METHOD2].index[0], rect[RECT_CONVERT1_5METHOD2].colorT[0]); //设定中心点
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x04))
                    {
                        for (i = KEY_CONVERT1_5INPUTBS; i <= KEY_CONVERT1_5HOMEBS; i++)
                        {
                            f = i - KEY_CONVERT1_5INPUTBS;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (g_datum_mark == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (g_datum_mark == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x08))
                    {
                        DrawEdit(rect[RECT_CONVERT1_5EDIT1_STR], rect[RECT_CONVERT1_5EDIT1_STR].colorT[0], rect[RECT_CONVERT1_5EDIT1_STR].colorB[1]);
                        DrawEditTit(rect[RECT_CONVERT1_5EDIT1_TIT], rect[RECT_CONVERT1_5EDIT1_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawMFS32P(rect[RECT_CONVERT1_5EDIT1_STR].xpis + (rect[RECT_CONVERT1_5EDIT1_STR].xsize - 5 * 13) / 2,
                                   rect[RECT_CONVERT1_5EDIT1_STR].ypis + (rect[RECT_CONVERT1_5EDIT1_STR].ysize - 24) / 2,
                                   tmp_parameter, 5, 1, rect[RECT_CONVERT1_5EDIT1_STR].colorT[0]);
                        DrawF16Str(rect[RECT_CONVERT1_5EDIT1_STR].xpis + rect[RECT_CONVERT1_5EDIT1_STR].xsize + SAFE_DIST5,
                                   rect[RECT_CONVERT1_5EDIT1_STR].ypis + (rect[RECT_CONVERT1_5EDIT1_STR].ysize - 16) / 2,
                                   "(0.1-180.0)", rect[RECT_CONVERT1_5EDIT1_STR].colorT[0]);
                    }
                }
                else if (g_num_mov_flag == 3)
                {
                    imgdisf = 0;
                    patdisf = 2;
                    dis_pos = 0;
                    if ((dis_updata & 0x01))
                    {
                        DrawFillRect(rect[RECT_CONVERT1_HINT].xpis, rect[RECT_CONVERT1_HINT].ypis,
                                     rect[RECT_CONVERT1_HINT].xsize, rect[RECT_CONVERT1_HINT].ysize, rect[RECT_CONVERT1_HINT].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_HINT].xpis + SAFE_DIST4, rect[RECT_CONVERT1_HINT].ypis + SAFE_DIST4,
                                     (MEM_TITLE_ADD + (title + 1) * TitleLengthMax), SheetPage, (title + 1),
                                     rect[RECT_CONVERT1_HINT].colorT[0]);

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis,
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 11) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 11), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //方向
                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17, rect[RECT_CONVERT1_MESSAGE].ypis,
                                     (MEM_TITLE_ADD + (title + 8 + tmp_selectmode) * TitleLengthMax),
                                     SheetPage, (title + 8 + tmp_selectmode), rect[RECT_CONVERT1_MESSAGE].colorT[0]);

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis + (SAFE_DIST5 + TYPE_SIZE5),
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 18) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 18), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //角度
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17, rect[RECT_CONVERT1_MESSAGE].ypis + (SAFE_DIST5 + TYPE_SIZE5),
                                   g_parameter, 5, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17 + 6 * 13,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + (SAFE_DIST5 + TYPE_SIZE5), "°", rect[RECT_CONVERT1_MESSAGE].colorT[0]);

                        Draw24x24Str(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5, rect[RECT_CONVERT1_MESSAGE].ypis + 4 * (SAFE_DIST5 + TYPE_SIZE5),
                                     (MEM_TITLE_ADD + (TLFNADD_SCALE + 15) * TitleLengthMax),
                                     SheetPage, (TLFNADD_SCALE + 15), rect[RECT_CONVERT1_MESSAGE].colorT[0]); //中心点
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 4 * (SAFE_DIST5 + TYPE_SIZE5), "X:", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17 + 13 * 3,
                                   rect[RECT_CONVERT1_MESSAGE].ypis + 4 * (SAFE_DIST5 + TYPE_SIZE5),
                                   g_dxpos, 7, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 5 * (SAFE_DIST5 + TYPE_SIZE5), "Y:", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE17 + 13 * 3,
                                   rect[RECT_CONVERT1_MESSAGE].ypis + 5 * (SAFE_DIST5 + TYPE_SIZE5),
                                   g_dypos, 7, 1, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        if (g_datum_mark == 0)
                        {
                            Draw24X24FStr(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE8 + 13 * 9,
                                          rect[RECT_CONVERT1_MESSAGE].ypis + 6 * (SAFE_DIST5 + TYPE_SIZE5), "/", rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                            DrawMFU32(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE8 + 13 * 3,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 6 * (SAFE_DIST5 + TYPE_SIZE5),
                                      g_jog_point, 5, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                            DrawMFU32(rect[RECT_CONVERT1_MESSAGE].xpis + SAFE_DIST5 + TYPE_SIZE8 + 13 * 11,
                                      rect[RECT_CONVERT1_MESSAGE].ypis + 6 * (SAFE_DIST5 + TYPE_SIZE5),
                                      tmp_feedsum, 5, rect[RECT_CONVERT1_MESSAGE].colorT[0]);
                        }
                    }
                }
            }
            else if (para == CONV_OFFSET) //离边缝
            {

                if (g_num_mov_flag == 0)
                {
                    imgdisf = 0;
                    patdisf = 2;
                    dis_pos = 0;
                    if (flag)
                    {
                        flag = 0;
                        tmp_selectmode = 0;
                        g_array4[tmp_selectmode][0] = 0;
                        g_array4[tmp_selectmode][1] = 0;
                        g_array4[tmp_selectmode + 1][0] = 0;
                        g_array4[tmp_selectmode + 1][1] = 0;

                        if (F_FILETYPE == PAT_MIT)
                        { //三菱
                            while (tmp_draw_add < tmp_line_add)
                            {
                                dat = SREG[tmp_draw_add] & 0xfc00;
                                if (dat == 0x1800)
                                {
                                    while (tmp_draw_add < tmp_line_add)
                                    {
                                        tmp_draw_add += 4;
                                        dat = SREG[tmp_draw_add];
                                        if (dat == 0x2001)
                                            break;
                                    }
                                    tmp_selectmode++;
                                    dat = SREG[tmp_draw_add + 1];
                                    if (dat >= 0x8000)
                                    {
                                        g_array4[tmp_selectmode * 2][0] = 0;
                                        g_array4[tmp_selectmode * 2][1] = 0x10000 - dat;
                                    }
                                    else
                                    {
                                        g_array4[tmp_selectmode * 2][0] = 1;
                                        g_array4[tmp_selectmode * 2][1] = dat;
                                    }
                                    g_array4[tmp_selectmode * 2 + 1][0] = tmp_draw_add;
                                }
                                tmp_draw_add += 4;
                            }
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        { //兄弟
                            Bor_ConCalAssArea((F_FILELEN), 0);
                            tmp_selectmode = g_point_num - 1;
                            g_distance = 0;
                        }

                        if ((g_selectmode == 0) && (tmp_selectmode))
                            g_selectmode = 1;
                        else if (g_selectmode > tmp_selectmode)
                        {
                            if (tmp_selectmode)
                                g_selectmode = 1;
                            else
                                g_selectmode = 0;
                        }
                        if (g_direction > 1)
                            g_direction = 0;
                        if (g_distance > 200)
                            g_distance = 0;
                        tmp_parameter = g_distance;

                        if (tmp_selectmode)
                        {
                            enterdisf = 1;
                            patMin = 1;
                            patMax = tmp_selectmode;
                        }
                        else
                        {
                            enterdisf = 0;
                            patMin = 0;
                            patMax = 0;
                        }
                    }

                    if ((dis_updata & 0x01))
                    {
                        //方向
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT1_TIT], rect[RECT_CONVERT1_6EDIT1_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        // DrawEdit(rect[RECT_CONVERT1_6EDIT1_STR],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0]);
                        DrawDottedRect(rect[RECT_CONVERT1_6EDIT1_BMP].xpis, rect[RECT_CONVERT1_6EDIT1_BMP].ypis,
                                       rect[RECT_CONVERT1_6EDIT1_BMP].xsize, rect[RECT_CONVERT1_6EDIT1_BMP].ysize, rect[RECT_CONVERT1_6EDIT1_BMP].colorT[0]);
                        DrawFPBMPBTN1(rect[RECT_CONVERT1_6EDIT1_BMP].xpis + 1, rect[RECT_CONVERT1_6EDIT1_BMP].ypis + 1, TYPE_SIZE8, TYPE_SIZE8,
                                      (g_array4[g_selectmode * 2][0]) ? rectDirBmpList[para - CONV_OFFSET][1] : rectDirBmpList[para - CONV_OFFSET][0]);
                        //方法
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT2_TIT], rect[RECT_CONVERT1_6EDIT2_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        // DrawEdit(rect[RECT_CONVERT1_6EDIT1_STR],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0]);
                        DrawDottedRect(rect[RECT_CONVERT1_6EDIT2_BMP].xpis, rect[RECT_CONVERT1_6EDIT2_BMP].ypis,
                                       rect[RECT_CONVERT1_6EDIT2_BMP].xsize, rect[RECT_CONVERT1_6EDIT2_BMP].ysize, rect[RECT_CONVERT1_6EDIT2_BMP].colorT[0]);
                        DrawFPBMPBTN1(rect[RECT_CONVERT1_6EDIT2_BMP].xpis + 1, rect[RECT_CONVERT1_6EDIT2_BMP].ypis + 1, TYPE_SIZE8, TYPE_SIZE8,
                                      rectMetBmpList[para - CONV_OFFSET][0]);

                        //距离
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT3_TIT], rect[RECT_CONVERT1_6EDIT3_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawEdit(rect[RECT_CONVERT1_6EDIT3_STR], rect[RECT_CONVERT1_6EDIT3_STR].colorB[0], rect[RECT_CONVERT1_6EDIT3_STR].colorB[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_6EDIT3_STR].xpis + (rect[RECT_CONVERT1_6EDIT3_STR].xsize - SAFE_DIST5 - 6 * 13) / 2,
                                   rect[RECT_CONVERT1_6EDIT3_STR].ypis + (rect[RECT_CONVERT1_6EDIT3_STR].ysize - 24) / 2,
                                   g_array4[g_selectmode * 2][1], 4, 1, rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_6EDIT3_STR].xpis + rect[RECT_CONVERT1_6EDIT3_STR].xsize - SAFE_DIST5 - 2 * 13,
                                      rect[RECT_CONVERT1_6EDIT3_STR].ypis + (rect[RECT_CONVERT1_6EDIT3_STR].ysize - 24) / 2,
                                      "mm", rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);

                        //编号
                        DrawEditTit(rect[RECT_CONVERT1_6EDITNO_TIT], rect[RECT_CONVERT1_6EDITNO_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawEdit(rect[RECT_CONVERT1_6EDITNO_STR], rect[RECT_CONVERT1_6EDITNO_STR].colorT[0], rect[RECT_CONVERT1_6EDITNO_STR].colorB[1]);
                        DrawMFS32P(rect[RECT_CONVERT1_6EDITNO_STR].xpis + (rect[RECT_CONVERT1_6EDITNO_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_6EDITNO_STR].ypis + (rect[RECT_CONVERT1_6EDITNO_STR].ysize - 24) / 2,
                                   g_selectmode, 2, 0, rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);
                        DrawF16Str(rect[RECT_CONVERT1_6EDITNO_STR].xpis,
                                   rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                                   "(  -  )", rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);

                        DrawFU32Z(rect[RECT_CONVERT1_6EDITNO_STR].xpis + 8,
                                  rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                                  patMin, 2, rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);
                        DrawFU32Z(rect[RECT_CONVERT1_6EDITNO_STR].xpis + 4 * 8,
                                  rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                                  patMax, 2, rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);

                        DrawButton(keyd[KEY_CONVERT1_6PREV], (U16 *)(ADD_ICONBMP),
                                   (g_selectmode == patMin) ? keybmpind[KEY_CONVERT1_6PREV].border[1] : keybmpind[KEY_CONVERT1_6PREV].border[0],
                                   (g_selectmode == patMin) ? keybmpind[KEY_CONVERT1_6PREV].icon[1] : keybmpind[KEY_CONVERT1_6PREV].icon[0]);
                        DrawButton(keyd[KEY_CONVERT1_6NEXT], (U16 *)(ADD_ICONBMP),
                                   (g_selectmode == patMax) ? keybmpind[KEY_CONVERT1_6NEXT].border[1] : keybmpind[KEY_CONVERT1_6NEXT].border[0],
                                   (g_selectmode == patMax) ? keybmpind[KEY_CONVERT1_6NEXT].icon[1] : keybmpind[KEY_CONVERT1_6NEXT].icon[0]);
                    }
                }
                else if (g_num_mov_flag == 2)
                {
                    imgdisf = 1;
                    patdisf = 0;
                    dis_pos = 0;
                    if ((dis_updata & 0x01))
                    {
                        DrawFillRect(rect[RECT_CONVERT1_HINT].xpis, rect[RECT_CONVERT1_HINT].ypis,
                                     rect[RECT_CONVERT1_HINT].xsize, rect[RECT_CONVERT1_HINT].ysize, rect[RECT_CONVERT1_HINT].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_HINT].xpis + SAFE_DIST4, rect[RECT_CONVERT1_HINT].ypis + SAFE_DIST4,
                                     (MEM_TITLE_ADD + (title + 1) * TitleLengthMax), SheetPage, (title + 1),
                                     rect[RECT_CONVERT1_HINT].colorT[0]);
                    }
                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_6METHOD].xpis, rect[RECT_CONVERT1_6METHOD].ypis,
                                 rect[RECT_CONVERT1_6METHOD].xsize, rect[RECT_CONVERT1_6METHOD].ysize, rect[RECT_CONVERT1_6METHOD].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_6METHOD].xpis + SAFE_DIST3, rect[RECT_CONVERT1_6METHOD].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_6METHOD].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_6METHOD].index[0], rect[RECT_CONVERT1_6METHOD].colorT[0]); //方向
                    }
                    if ((dis_updata & 0x01) || (dis_updata & 0x02))
                    {
                        for (i = KEY_CONVERT1_6DIRL; i <= KEY_CONVERT1_6DIRR; i++)
                        {
                            f = i - KEY_CONVERT1_6DIRL;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (g_direction == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (g_direction == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }

                    if ((dis_updata & 0x01))
                    {
                        DrawEdit(rect[RECT_CONVERT1_6NO_STR], rect[RECT_CONVERT1_6NO_STR].colorB[0], rect[RECT_CONVERT1_6NO_STR].colorB[0]); //编号
                        DrawEditTit(rect[RECT_CONVERT1_6NO_TIT], rect[RECT_CONVERT1_6NO_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawMFS32P(rect[RECT_CONVERT1_6NO_STR].xpis + (rect[RECT_CONVERT1_6NO_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_6NO_STR].ypis + (rect[RECT_CONVERT1_6NO_STR].ysize - 24) / 2,
                                   g_selectmode, 2, 0, rect[RECT_CONVERT1_6NO_STR].colorT[0]);
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x08))
                    {
                        for (i = 0; i < ((F_FILETYPE == PAT_MIT) ? 1 : 2); i++) //距离、针距
                        {
                            //绘制文本框
                            f = KEY_CONVERT1_6DIS + i;
                            DrawButtonEdit(keyd[f],
                                           (editflag == i) ? keybmpind[f].textColor[0] : keybmpind[f].icon[0],
                                           (editflag == i) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                            //绘制文本框标题
                            f = RECT_CONVERT1_6DIS_TIT + i * 2;
                            DrawEditTit(rect[f], rect[f].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        }
                        //距离
                        DrawMFS32P(rect[RECT_CONVERT1_6DIS_STR].xpis + (rect[RECT_CONVERT1_6DIS_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                                   rect[RECT_CONVERT1_6DIS_STR].ypis + (rect[RECT_CONVERT1_6DIS_STR].ysize - 24) / 2,
                                   (editflag == 0) ? tmp_parameter : Convedis, 4, 1, rect[RECT_CONVERT1_6DIS_STR].colorT[0]);
                        DrawF16Str(rect[RECT_CONVERT1_6DIS_STR].xpis + rect[RECT_CONVERT1_6DIS_STR].xsize + SAFE_DIST5,
                                   rect[RECT_CONVERT1_6DIS_STR].ypis + (rect[RECT_CONVERT1_6DIS_STR].ysize - 16) / 2,
                                   "(0.0-20.0)mm", rect[RECT_CONVERT1_6DIS_STR].colorT[0]);

                        if (F_FILETYPE == PAT_SEW)
                        {
                            //针距
                            DrawMFS32P(rect[RECT_CONVERT1_6DIS_STR].xpis + (rect[RECT_CONVERT1_6DIS_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                                       rect[RECT_CONVERT1_6DIS_STR].ypis + (rect[RECT_CONVERT1_6DIS_STR].ysize - 24) / 2,
                                       (editflag == 1) ? tmp_parameter : g_convert_pindis, 4, 1, rect[RECT_CONVERT1_6DIS_STR].colorT[0]);
                            DrawF16Str(rect[RECT_CONVERT1_6DIS_STR].xpis + rect[RECT_CONVERT1_6DIS_STR].xsize + SAFE_DIST5,
                                       rect[RECT_CONVERT1_6DIS_STR].ypis + (rect[RECT_CONVERT1_6DIS_STR].ysize - 16) / 2,
                                       "(0.0-12.7)mm", rect[RECT_CONVERT1_6DIS_STR].colorT[0]);

                            //绘制文本框-针角度
                            DrawButtonEdit(keyd[KEY_CONVERT1_9ANG],
                                           (editflag == 2) ? keybmpind[KEY_CONVERT1_9ANG].textColor[0] : keybmpind[KEY_CONVERT1_9ANG].icon[0],
                                           (editflag == 2) ? keybmpind[KEY_CONVERT1_9ANG].icon[1] : keybmpind[KEY_CONVERT1_9ANG].icon[0]);
                            //绘制文本框标题-针角度
                            DrawEditTit(rect[RECT_CONVERT1_9ANG_TIT], rect[RECT_CONVERT1_9ANG_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);

                            DrawF16Str(rect[RECT_CONVERT1_9ANG_STR].xpis + rect[RECT_CONVERT1_9ANG_STR].xsize + SAFE_DIST5,
                                       rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 16) / 2,
                                       "(20-60)", rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                            DrawMFS32P(rect[RECT_CONVERT1_9ANG_STR].xpis + (rect[RECT_CONVERT1_9ANG_STR].xsize - 2 * 13) / 2,
                                       rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 24) / 2,
                                       (editflag == 2) ? tmp_parameter : ConAng, 2, 0,
                                       (editflag == 2) ? rect[RECT_CONVERT1_9ANG_STR].colorT[0] : rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                        }
                    }
                }
            }
            else if (para == CONV_OFFSMULT) //多重缝
            {
                if (g_num_mov_flag == 0)
                {
                    imgdisf = 0;
                    patdisf = 2;
                    dis_pos = 0;
                    if (flag)
                    {
                        flag = 0;
                        tmp_selectmode = 0;
                        g_array4[tmp_selectmode][0] = 0;
                        g_array4[tmp_selectmode][1] = 0;
                        g_array4[tmp_selectmode + 1][0] = 0;
                        g_array4[tmp_selectmode + 1][1] = 0;
                        g_array4[tmp_selectmode + 2][0] = 0;
                        g_array4[tmp_selectmode + 2][1] = 0;
                        if (F_FILETYPE == PAT_MIT)
                        { //三菱
                            while (tmp_draw_add < tmp_line_add)
                            {
                                dat = SREG[tmp_draw_add] & 0xfc00;
                                if ((dat == 0x0800) || (dat == 0x8800) || (dat == 0x1000) || (dat == 0x9000))
                                {
                                    if (dat == 0x1000)
                                    {
                                        g_array4[tmp_selectmode * 3 + 3][0] = 1;
                                    }
                                    else if (dat == 0x8800)
                                    {
                                        g_array4[tmp_selectmode * 3 + 3][0] = 2;
                                    }
                                    else if (dat == 0x9000)
                                    {
                                        g_array4[tmp_selectmode * 3 + 3][0] = 3;
                                    }
                                    else
                                    {
                                        g_array4[tmp_selectmode * 3 + 3][0] = 0;
                                    }
                                    while (tmp_draw_add < tmp_line_add)
                                    {
                                        tmp_draw_add += 4;
                                        dat = SREG[tmp_draw_add];
                                        if (dat == 0x2001)
                                            break;
                                    }
                                    tmp_selectmode++;
                                    dat = SREG[tmp_draw_add + 1];
                                    if (dat >= 0x8000)
                                    {
                                        g_array4[tmp_selectmode * 3 + 1][0] = 0;
                                        g_array4[tmp_selectmode * 3 + 1][1] = 0x10000 - dat;
                                    }
                                    else
                                    {
                                        g_array4[tmp_selectmode * 3 + 1][0] = 1;
                                        g_array4[tmp_selectmode * 3 + 1][1] = dat;
                                    }
                                    g_array4[tmp_selectmode * 3][1] = SREG[tmp_draw_add + 2] & 0x00ff;
                                    g_array4[tmp_selectmode * 3 + 2][0] = tmp_draw_add;
                                }
                                tmp_draw_add += 4;
                            }
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        { //兄弟
                            Bor_ConCalAssArea((F_FILELEN), 1);
                            tmp_selectmode = g_point_num - 1;
                            g_distance = 0;
                        }
                        //tmp_selectmode = 4;//-------------------------------------------

                        if ((g_selectmode == 0) && (tmp_selectmode))
                            g_selectmode = 1;
                        else if (g_selectmode > tmp_selectmode)
                        {
                            if (tmp_selectmode)
                                g_selectmode = 1;
                            else
                                g_selectmode = 0;
                        }
                        if (g_para_offset > 3)
                            g_para_offset = 0;
                        if (g_direction > 1)
                            g_direction = 0;
                        if (g_distance > 200)
                            g_distance = 0;
                        if ((g_times < 2) || (g_times > 9))
                            g_times = 2;
                        tmp_parameter = g_distance;
                        if (editflag > 1)
                            editflag = 0;
                        if (tmp_selectmode)
                        {
                            enterdisf = 1;
                            patMin = 1;
                            patMax = tmp_selectmode;
                        }
                        else
                        {
                            enterdisf = 0;
                            patMin = 0;
                            patMax = 0;
                        }
                    }

                    if ((dis_updata & 0x01))
                    {
                        //方向
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT1_TIT], rect[RECT_CONVERT1_6EDIT1_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        // DrawEdit(rect[RECT_CONVERT1_6EDIT1_STR],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0]);
                        DrawDottedRect(rect[RECT_CONVERT1_6EDIT1_BMP].xpis, rect[RECT_CONVERT1_6EDIT1_BMP].ypis,
                                       rect[RECT_CONVERT1_6EDIT1_BMP].xsize, rect[RECT_CONVERT1_6EDIT1_BMP].ysize, rect[RECT_CONVERT1_6EDIT1_BMP].colorT[0]);
                        DrawFPBMPBTN1(rect[RECT_CONVERT1_6EDIT1_BMP].xpis + 1, rect[RECT_CONVERT1_6EDIT1_BMP].ypis + 1, TYPE_SIZE8, TYPE_SIZE8,
                                      (g_array4[g_selectmode * 3 + 1][0]) ? rectDirBmpList[para - CONV_OFFSET][1] : rectDirBmpList[para - CONV_OFFSET][0]);
                        //方法
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT2_TIT], rect[RECT_CONVERT1_6EDIT2_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        // DrawEdit(rect[RECT_CONVERT1_6EDIT1_STR],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0],rect[RECT_CONVERT1_6EDIT1_STR].colorB[0]);
                        DrawDottedRect(rect[RECT_CONVERT1_6EDIT2_BMP].xpis, rect[RECT_CONVERT1_6EDIT2_BMP].ypis,
                                       rect[RECT_CONVERT1_6EDIT2_BMP].xsize, rect[RECT_CONVERT1_6EDIT2_BMP].ysize, rect[RECT_CONVERT1_6EDIT2_BMP].colorT[0]);
                        if (g_array4[g_selectmode * 3][0] < 2)
                        {
                            DrawFPBMPBTN1(rect[RECT_CONVERT1_6EDIT2_BMP].xpis + 1, rect[RECT_CONVERT1_6EDIT2_BMP].ypis + 1, TYPE_SIZE8, TYPE_SIZE8,
                                          g_array4[g_selectmode * 3][0] == 0 ? rectMetBmpList[para - CONV_OFFSET][0] : rectMetBmpList[para - CONV_OFFSET][1]);
                        }
                        else
                        {
                            if (g_array4[g_selectmode * 3][0] > 3)
                                DrawFPBMPBTN1(rect[RECT_CONVERT1_6EDIT2_BMP].xpis + 1, rect[RECT_CONVERT1_6EDIT2_BMP].ypis + 1, TYPE_SIZE8, TYPE_SIZE8,
                                              rectMetBmpList[para - CONV_OFFSET][0]);
                            else
                                DrawFPBMPBTN1(rect[RECT_CONVERT1_6EDIT2_BMP].xpis + 1, rect[RECT_CONVERT1_6EDIT2_BMP].ypis + 1, TYPE_SIZE8, TYPE_SIZE8,
                                              g_array4[g_selectmode * 3][0] == 2 ? rectMetBmpList[para - CONV_OFFSET][2] : rectMetBmpList[para - CONV_OFFSET][3]);
                        }

                        //距离
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT3_TIT], rect[RECT_CONVERT1_6EDIT3_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawEdit(rect[RECT_CONVERT1_6EDIT3_STR], rect[RECT_CONVERT1_6EDIT3_STR].colorB[0], rect[RECT_CONVERT1_6EDIT3_STR].colorB[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_6EDIT3_STR].xpis + (rect[RECT_CONVERT1_6EDIT3_STR].xsize - SAFE_DIST5 - 6 * 13) / 2,
                                   rect[RECT_CONVERT1_6EDIT3_STR].ypis + (rect[RECT_CONVERT1_6EDIT3_STR].ysize - 24) / 2,
                                   g_array4[g_selectmode * 3 + 1][1], 4, 1, rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);
                        Draw24X24FStr(rect[RECT_CONVERT1_6EDIT3_STR].xpis + rect[RECT_CONVERT1_6EDIT3_STR].xsize - SAFE_DIST5 - 2 * 13,
                                      rect[RECT_CONVERT1_6EDIT3_STR].ypis + (rect[RECT_CONVERT1_6EDIT3_STR].ysize - 24) / 2,
                                      "mm", rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);
                        //次数
                        DrawEditTit(rect[RECT_CONVERT1_6EDIT4_TIT], rect[RECT_CONVERT1_6EDIT4_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawEdit(rect[RECT_CONVERT1_6EDIT4_STR], rect[RECT_CONVERT1_6EDIT4_STR].colorB[0], rect[RECT_CONVERT1_6EDIT4_STR].colorB[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_6EDIT4_STR].xpis + (rect[RECT_CONVERT1_6EDIT4_STR].xsize - SAFE_DIST5 - 1 * 13) / 2,
                                   rect[RECT_CONVERT1_6EDIT4_STR].ypis + (rect[RECT_CONVERT1_6EDIT4_STR].ysize - 24) / 2,
                                   g_array4[g_selectmode * 3][1], 1, 0, rect[RECT_CONVERT1_6EDIT4_STR].colorT[0]);

                        //编号
                        DrawEditTit(rect[RECT_CONVERT1_6EDITNO_TIT], rect[RECT_CONVERT1_6EDITNO_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawEdit(rect[RECT_CONVERT1_6EDITNO_STR], rect[RECT_CONVERT1_6EDITNO_STR].colorT[0], rect[RECT_CONVERT1_6EDITNO_STR].colorB[1]);
                        DrawMFS32P(rect[RECT_CONVERT1_6EDITNO_STR].xpis + (rect[RECT_CONVERT1_6EDITNO_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_6EDITNO_STR].ypis + (rect[RECT_CONVERT1_6EDITNO_STR].ysize - 24) / 2,
                                   g_selectmode, 2, 0, rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);
                        DrawF16Str(rect[RECT_CONVERT1_6EDITNO_STR].xpis,
                                   rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                                   "(  -  )", rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);

                        DrawFU32Z(rect[RECT_CONVERT1_6EDITNO_STR].xpis + 8,
                                  rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                                  patMin, 2, rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);
                        DrawFU32Z(rect[RECT_CONVERT1_6EDITNO_STR].xpis + 4 * 8,
                                  rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                                  patMax, 2, rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);

                        DrawButton(keyd[KEY_CONVERT1_6PREV], (U16 *)(ADD_ICONBMP),
                                   (g_selectmode == patMin) ? keybmpind[KEY_CONVERT1_6PREV].border[1] : keybmpind[KEY_CONVERT1_6PREV].border[0],
                                   (g_selectmode == patMin) ? keybmpind[KEY_CONVERT1_6PREV].icon[1] : keybmpind[KEY_CONVERT1_6PREV].icon[0]);
                        DrawButton(keyd[KEY_CONVERT1_6NEXT], (U16 *)(ADD_ICONBMP),
                                   (g_selectmode == patMax) ? keybmpind[KEY_CONVERT1_6NEXT].border[1] : keybmpind[KEY_CONVERT1_6NEXT].border[0],
                                   (g_selectmode == patMax) ? keybmpind[KEY_CONVERT1_6NEXT].icon[1] : keybmpind[KEY_CONVERT1_6NEXT].icon[0]);
                    }
                }
                else if (g_num_mov_flag == 2)
                {
                    imgdisf = 1;
                    patdisf = 0;
                    dis_pos = 0;
                    if ((dis_updata & 0x01))
                    {
                        DrawFillRect(rect[RECT_CONVERT1_HINT].xpis, rect[RECT_CONVERT1_HINT].ypis,
                                     rect[RECT_CONVERT1_HINT].xsize, rect[RECT_CONVERT1_HINT].ysize, rect[RECT_CONVERT1_HINT].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_HINT].xpis + SAFE_DIST4, rect[RECT_CONVERT1_HINT].ypis + SAFE_DIST4,
                                     (MEM_TITLE_ADD + (title + 1) * TitleLengthMax), SheetPage, (title + 1),
                                     rect[RECT_CONVERT1_HINT].colorT[0]);
                    }
                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_7METHOD1].xpis, rect[RECT_CONVERT1_7METHOD1].ypis,
                                 rect[RECT_CONVERT1_7METHOD1].xsize, rect[RECT_CONVERT1_7METHOD1].ysize, rect[RECT_CONVERT1_7METHOD1].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_7METHOD1].xpis + SAFE_DIST3, rect[RECT_CONVERT1_7METHOD1].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_7METHOD1].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_7METHOD1].index[0], rect[RECT_CONVERT1_7METHOD1].colorT[0]); //方法
                    }
                    if ((dis_updata & 0x01) || (dis_updata & 0x02))
                    {
                        for (i = KEY_CONVERT1_7FDSYMULT; i <= KEY_CONVERT1_7SWREMULT; i++)
                        {
                            f = i - KEY_CONVERT1_7FDSYMULT;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (g_para_offset == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (g_para_offset == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }
                    if ((dis_updata & 0x01))
                    {
                        DrawRect(rect[RECT_CONVERT1_7METHOD2].xpis, rect[RECT_CONVERT1_7METHOD2].ypis,
                                 rect[RECT_CONVERT1_7METHOD2].xsize, rect[RECT_CONVERT1_7METHOD2].ysize, rect[RECT_CONVERT1_7METHOD2].colorB[0]);
                        Draw24x24Str(rect[RECT_CONVERT1_7METHOD2].xpis + SAFE_DIST3, rect[RECT_CONVERT1_7METHOD2].ypis + SAFE_DIST3,
                                     (MEM_TITLE_ADD + (U32)(rect[RECT_CONVERT1_7METHOD2].index[0] * TitleLengthMax)),
                                     SheetPage, rect[RECT_CONVERT1_7METHOD2].index[0], rect[RECT_CONVERT1_7METHOD2].colorT[0]); //方向
                    }
                    if ((dis_updata & 0x01) || (dis_updata & 0x04))
                    {
                        for (i = KEY_CONVERT1_7DIRL; i <= KEY_CONVERT1_7DIRR; i++)
                        {
                            f = i - KEY_CONVERT1_7DIRL;
                            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                       (g_direction == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                       (g_direction == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                        }
                    }

                    if ((dis_updata & 0x01))
                    {
                        DrawEdit(rect[RECT_CONVERT1_7NO_STR], rect[RECT_CONVERT1_7NO_STR].colorB[0], rect[RECT_CONVERT1_7NO_STR].colorB[0]); //编号
                        DrawEditTit(rect[RECT_CONVERT1_7NO_TIT], rect[RECT_CONVERT1_7NO_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        DrawMFS32P(rect[RECT_CONVERT1_7NO_STR].xpis + (rect[RECT_CONVERT1_7NO_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_7NO_STR].ypis + (rect[RECT_CONVERT1_7NO_STR].ysize - 24) / 2,
                                   g_selectmode, 2, 0, rect[RECT_CONVERT1_7NO_STR].colorT[0]);
                    }

                    if ((dis_updata & 0x01) || (dis_updata & 0x08))
                    {
                        for (i = 0; i < ((F_FILETYPE == PAT_MIT) ? 2 : 3); i++) //距离、次数、针距
                        {
                            //绘制文本框
                            f = KEY_CONVERT1_7SELDIST + i;
                            DrawButtonEdit(keyd[f],
                                           (editflag == i) ? keybmpind[f].textColor[0] : keybmpind[f].icon[0],
                                           (editflag == i) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                            //绘制文本框标题
                            f = RECT_CONVERT1_7DIST_TIT + i * 2;
                            DrawEditTit(rect[f], rect[f].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                        }
                        //距离
                        DrawMFS32P(rect[RECT_CONVERT1_7DIST_STR].xpis + (rect[RECT_CONVERT1_7DIST_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                                   rect[RECT_CONVERT1_7DIST_STR].ypis + (rect[RECT_CONVERT1_7DIST_STR].ysize - 24) / 2,
                                   (editflag == 0) ? tmp_parameter : g_distance, 4, 1, rect[RECT_CONVERT1_7DIST_STR].colorT[0]);
                        DrawF16Str(rect[RECT_CONVERT1_7DIST_STR].xpis + rect[RECT_CONVERT1_7DIST_STR].xsize + SAFE_DIST5,
                                   rect[RECT_CONVERT1_7DIST_STR].ypis + (rect[RECT_CONVERT1_7DIST_STR].ysize - 16) / 2,
                                   "(0.0-20.0)mm", rect[RECT_CONVERT1_7DIST_STR].colorT[0]);
                        //次数
                        DrawMFS32P(rect[RECT_CONVERT1_7TIME_STR].xpis + (rect[RECT_CONVERT1_7TIME_STR].xsize - SAFE_DIST5 - 1 * 13) / 2,
                                   rect[RECT_CONVERT1_7TIME_STR].ypis + (rect[RECT_CONVERT1_7TIME_STR].ysize - 24) / 2,
                                   (editflag == 1) ? tmp_parameter : g_times, 1, 0, rect[RECT_CONVERT1_7TIME_STR].colorT[0]);
                        DrawF16Str(rect[RECT_CONVERT1_7TIME_STR].xpis + rect[RECT_CONVERT1_7TIME_STR].xsize + SAFE_DIST5,
                                   rect[RECT_CONVERT1_7TIME_STR].ypis + (rect[RECT_CONVERT1_7TIME_STR].ysize - 16) / 2,
                                   "(2-9)", rect[RECT_CONVERT1_7TIME_STR].colorT[0]);

                        if (F_FILETYPE == PAT_SEW)
                        {
                            //针距
                            DrawMFS32P(rect[RECT_CONVERT1_7PITCH_STR].xpis + (rect[RECT_CONVERT1_7PITCH_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                                       rect[RECT_CONVERT1_7PITCH_STR].ypis + (rect[RECT_CONVERT1_7PITCH_STR].ysize - 24) / 2,
                                       (editflag == 2) ? tmp_parameter : g_convert_pindis, 4, 1, rect[RECT_CONVERT1_7PITCH_STR].colorT[0]);
                            DrawF16Str(rect[RECT_CONVERT1_7PITCH_STR].xpis + rect[RECT_CONVERT1_7PITCH_STR].xsize + SAFE_DIST5,
                                       rect[RECT_CONVERT1_7PITCH_STR].ypis + (rect[RECT_CONVERT1_7PITCH_STR].ysize - 16) / 2,
                                       "(0.0-12.7)mm", rect[RECT_CONVERT1_7PITCH_STR].colorT[0]);

                            //绘制文本框-针角度
                            DrawButtonEdit(keyd[KEY_CONVERT1_9ANG],
                                           (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].textColor[0] : keybmpind[KEY_CONVERT1_9ANG].icon[0],
                                           (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].icon[1] : keybmpind[KEY_CONVERT1_9ANG].icon[0]);
                            //绘制文本框标题-针角度
                            DrawEditTit(rect[RECT_CONVERT1_9ANG_TIT], rect[RECT_CONVERT1_9ANG_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);

                            DrawF16Str(rect[RECT_CONVERT1_9ANG_STR].xpis + rect[RECT_CONVERT1_9ANG_STR].xsize + SAFE_DIST5,
                                       rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 16) / 2,
                                       "(20-60)", rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                            DrawMFS32P(rect[RECT_CONVERT1_9ANG_STR].xpis + (rect[RECT_CONVERT1_9ANG_STR].xsize - 2 * 13) / 2,
                                       rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 24) / 2,
                                       (editflag == 3) ? tmp_parameter : ConAng, 2, 0,
                                       (editflag == 3) ? rect[RECT_CONVERT1_9ANG_STR].colorT[0] : rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                        }
                    }
                }
            }
            else if (para == CONV_CHANGE && F_FILETYPE == PAT_SEW) //首尾变换
            {
                imgdisf = 0;
                patdisf = 2;
                dis_pos = 0;
                Bor_ConCalAssArea((F_FILELEN), 0);
                tmp_selectmode = g_point_num - 1;
                // Draw24x24Str(64, 112, (MEM_TITLE_ADD + 86 * TitleLengthMax), SheetPage, 86, Color_Black); // NO.

                if ((g_selectmode == 0) && (tmp_selectmode))
                    g_selectmode = 1;
                else if (g_selectmode > tmp_selectmode)
                {
                    if (tmp_selectmode)
                        g_selectmode = 1;
                    else
                        g_selectmode = 0;
                }

                if (tmp_selectmode)
                {
                    enterdisf = 1;
                    patMin = 1;
                    patMax = tmp_selectmode;
                }
                else
                {
                    enterdisf = 0;
                    patMin = 0;
                    patMax = 0;
                }

                if ((dis_updata & 0x01))
                {
                    DrawFillRect(rect[RECT_CONVERT1_HINT].xpis, rect[RECT_CONVERT1_HINT].ypis,
                                 rect[RECT_CONVERT1_HINT].xsize, rect[RECT_CONVERT1_HINT].ysize, rect[RECT_CONVERT1_HINT].colorB[0]);
                    Draw24x24Str(rect[RECT_CONVERT1_HINT].xpis + SAFE_DIST4, rect[RECT_CONVERT1_HINT].ypis + SAFE_DIST4,
                                 (MEM_TITLE_ADD + (title + 1) * TitleLengthMax), SheetPage, (title + 1),
                                 rect[RECT_CONVERT1_HINT].colorT[0]);
                }

                if ((dis_updata & 0x01))
                {
                    //编号
                    DrawEditTit(rect[RECT_CONVERT1_6EDITNO_TIT], rect[RECT_CONVERT1_6EDITNO_TIT].index[0], SheetPage, CENTER, STR24X12, SAFE_DIST3);
                    DrawEdit(rect[RECT_CONVERT1_6EDITNO_STR], rect[RECT_CONVERT1_6EDITNO_STR].colorT[0], rect[RECT_CONVERT1_6EDITNO_STR].colorB[1]);
                    DrawMFS32P(rect[RECT_CONVERT1_6EDITNO_STR].xpis + (rect[RECT_CONVERT1_6EDITNO_STR].xsize - 2 * 13) / 2,
                               rect[RECT_CONVERT1_6EDITNO_STR].ypis + (rect[RECT_CONVERT1_6EDITNO_STR].ysize - 24) / 2,
                               g_selectmode, 2, 0, rect[RECT_CONVERT1_6EDIT3_STR].colorT[0]);
                    DrawF16Str(rect[RECT_CONVERT1_6EDITNO_STR].xpis,
                               rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                               "(  -  )", rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);

                    DrawFU32Z(rect[RECT_CONVERT1_6EDITNO_STR].xpis + 8,
                              rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                              patMin, 2, rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);
                    DrawFU32Z(rect[RECT_CONVERT1_6EDITNO_STR].xpis + 4 * 8,
                              rect[RECT_CONVERT1_6EDITNO_STR].ypis + rect[RECT_CONVERT1_6EDITNO_STR].ysize + SAFE_DIST5,
                              patMax, 2, rect[RECT_CONVERT1_6EDITNO_STR].colorT[0]);

                    DrawButton(keyd[KEY_CONVERT1_6PREV], (U16 *)(ADD_ICONBMP),
                               (g_selectmode == patMin) ? keybmpind[KEY_CONVERT1_6PREV].border[1] : keybmpind[KEY_CONVERT1_6PREV].border[0],
                               (g_selectmode == patMin) ? keybmpind[KEY_CONVERT1_6PREV].icon[1] : keybmpind[KEY_CONVERT1_6PREV].icon[0]);
                    DrawButton(keyd[KEY_CONVERT1_6NEXT], (U16 *)(ADD_ICONBMP),
                               (g_selectmode == patMax) ? keybmpind[KEY_CONVERT1_6NEXT].border[1] : keybmpind[KEY_CONVERT1_6NEXT].border[0],
                               (g_selectmode == patMax) ? keybmpind[KEY_CONVERT1_6NEXT].icon[1] : keybmpind[KEY_CONVERT1_6NEXT].icon[0]);
                }
            }

            if (dis_updata & 0x01)
            {
                if (deletedisf)
                {
                    DrawButton(keyd[KEY_CONVERT1_CANCEL], (U16 *)(ADD_ICONBMP),
                               keybmpind[KEY_CONVERT1_CANCEL].border[0], keybmpind[KEY_CONVERT1_CANCEL].icon[0]);
                }
                else
                {
                    BmpFillRect(keyd[KEY_CONVERT1_CANCEL].xpis, keyd[KEY_CONVERT1_CANCEL].ypis, keyd[KEY_CONVERT1_CANCEL].xsize, keyd[KEY_CONVERT1_CANCEL].ysize);
                }

                if (imgdisf)
                {
                    DrawButton(keyd[KEY_CONVERT1_VIEW], (U16 *)(ADD_ICONBMP),
                               keybmpind[KEY_CONVERT1_VIEW].border[0], keybmpind[KEY_CONVERT1_VIEW].icon[0]);
                }
                else
                {
                    BmpFillRect(keyd[KEY_CONVERT1_VIEW].xpis, keyd[KEY_CONVERT1_VIEW].ypis, keyd[KEY_CONVERT1_VIEW].xsize, keyd[KEY_CONVERT1_VIEW].ysize);
                }

                if (enterdisf)
                {
                    DrawButton(keyd[KEY_CONVERT1_ENTER], (U16 *)(ADD_ICONBMP),
                               keybmpind[KEY_CONVERT1_ENTER].border[0], keybmpind[KEY_CONVERT1_ENTER].icon[0]);
                }
                else
                {
                    BmpFillRect(keyd[KEY_CONVERT1_ENTER].xpis, keyd[KEY_CONVERT1_ENTER].ypis, keyd[KEY_CONVERT1_ENTER].xsize, keyd[KEY_CONVERT1_ENTER].ysize);
                }

                for (i = 2; i <= KEY_CONVERT1_ZIN; i++)
                {
                    if (((i == KEY_CONVERT1_VIEW) && (imgdisf == 0)) ||
                        ((i == KEY_CONVERT1_PFOOT) && (pfootdisf == 0)) ||
                        ((i == KEY_CONVERT1_CANCEL) && (deletedisf == 0)) ||
                        ((i == KEY_CONVERT1_ENTER) && (enterdisf == 0)) ||
                        ((i == KEY_CONVERT1_ZOUT) && (patdisf == 0)) ||
                        ((i == KEY_CONVERT1_ZIN) && (patdisf == 0)))

                    {
                    }
                    else
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                }
            }

            if (patdisf)
            {
                for (i = RECT_CONVERT1_X_TIT; i <= RECT_CONVERT1_EN_STR; i++)
                {
                    BmpFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize);
                }
                for (i = RECT_CONVERT1_X_TIT; i <= RECT_CONVERT1_X_STR + (patdisf - 1) * 2; i++)
                {
                    if ((i - RECT_CONVERT1_X_TIT) % 2 == 0)
                    {
                        DrawEditTit(rect[i], rectTitList[para][i / 2], SheetPage, LEFT, STR16X8, SAFE_DIST3);
                    }
                    else
                    {
                        DrawFillRect(rect[i].xpis, rect[i].ypis,
                                     rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    }
                }

                if (patdisf > 2)
                {
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
                        DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
                        DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
                    }
#else
                    DrawFS32P(rect[RECT_CONVERT1_X_STR].xpis + SAFE_DIST3, rect[RECT_CONVERT1_X_STR].ypis + SAFE_DIST4,
                              g_dxpos, 6, 1, rect[RECT_CONVERT1_X_STR].colorT[0]);
                    DrawFS32P(rect[RECT_CONVERT1_Y_STR].xpis + SAFE_DIST3, rect[RECT_CONVERT1_Y_STR].ypis + SAFE_DIST4,
                              g_dypos, 6, 1, rect[RECT_CONVERT1_Y_STR].colorT[0]);
#endif
                }
                else if (patdisf == 2)
                {
                    DrawFS32P(rect[RECT_CONVERT1_X_STR].xpis + SAFE_DIST3, rect[RECT_CONVERT1_X_STR].ypis + SAFE_DIST4,
                              g_patd.patmX - g_patd.patiX, 6, 1, rect[RECT_CONVERT1_X_STR].colorT[0]);
                    DrawFS32P(rect[RECT_CONVERT1_Y_STR].xpis + SAFE_DIST3, rect[RECT_CONVERT1_Y_STR].ypis + SAFE_DIST4,
                              g_patd.patmY - g_patd.patiY, 6, 1, rect[RECT_CONVERT1_Y_STR].colorT[0]);
                }

                if (patdisf >= 3)
                {
                    DrawFU32(rect[RECT_CONVERT1_ST_STR].xpis + SAFE_DIST2, rect[RECT_CONVERT1_ST_STR].ypis + SAFE_DIST4,
                             tmp_feedc, 5, rect[RECT_CONVERT1_ST_STR].colorT[0]);
                    DrawF16Str(rect[RECT_CONVERT1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_CONVERT1_ST_STR].ypis + SAFE_DIST4,
                               "/", rect[RECT_CONVERT1_ST_STR].colorT[0]);
                    DrawFU32(rect[RECT_CONVERT1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_CONVERT1_ST_STR].ypis + SAFE_DIST4,
                             tmp_feedsum, 5, rect[RECT_CONVERT1_ST_STR].colorT[0]);
                }
            }
            dis_updata = 0;
        }
        // /[界面显示用的UI初始化*end]********************************************************

        // /[更新花样显示*begin]**********************************************************
        if (dis_updata2 && patdisf)
        {
            dis_updata2 = 0;

            // vscale = scale + scale1;
            vscale = pat_getFinalScale(scale, scale1);
            xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
            yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;

            posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
            posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
            if ((((posx1 < keyd[KEY_CONVERT1_IMG].xpis) || (posx1 > keyd[KEY_CONVERT1_IMG].xpis + keyd[KEY_CONVERT1_IMG].xsize) || (posy1 < keyd[KEY_CONVERT1_IMG].ypis) || (posy1 > keyd[KEY_CONVERT1_IMG].ypis + keyd[KEY_CONVERT1_IMG].ysize)) && (vflag == 0)) || vflag == 1)
            {
                //printf("*--------------------------*\r\n");
                xorg = ORG_X;
                yorg = ORG_Y;
                xorg += (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg += tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;

                xo = xorg + (xorg - (S32)((g_tmpoxpos + mult1 * g_dxpos) * vscale / 1000 + xorg));
                yo = yorg + (yorg - (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * vscale / 1000 + yorg));

                xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
                vflag = 1;
            }
            // DrawFillRect(keyd[KEY_CONVERT1_IMG].xpis, keyd[KEY_CONVERT1_IMG].ypis, keyd[KEY_CONVERT1_IMG].xsize, keyd[KEY_CONVERT1_IMG].ysize,
            //              keybmpind[KEY_CONVERT1_IMG].icon[0]);
            // newDrawPat(i, scale1, xo, yo, 0, 0);

            if (para == CONV_ORGSUB)
            {
                i = 0;
                patRulesTmp = PATRULES_CONVERT1_MOVE_ORG;
            }
            else if (para == CONV_SCALE)
            {
                if (g_num_mov_flag == 2)
                {
                    i = 21;
                    patRulesTmp = PATRULES_CONVERT1_NUM;
                }
                else if (g_num_mov_flag == 3)
                {
                    i = 22;
                    patRulesTmp = PATRULES_CONVERT1_ENSURE;
                }
                else if (g_num_mov_flag == 1)
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
                else
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
            }
            else if (para == CONV_MIRROR)
            {
                i = 0;
                patRulesTmp = PATRULES_CONVERT1_MOVE;
            }
            else if (para == CONV_ROTATE)
            {
                if (g_num_mov_flag == 2)
                {
                    i = 21;
                    patRulesTmp = PATRULES_CONVERT1_NUM;
                }
                else if (g_num_mov_flag == 3)
                {
                    i = 22;
                    patRulesTmp = PATRULES_CONVERT1_ENSURE;
                }
                else if (g_num_mov_flag == 1)
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
                else
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
            }
            else if (para == CONV_OFFSET)
            {
                if (g_num_mov_flag == 0)
                {
                    i = 23;
                    patRulesTmp = PATRULES_CONVERT1_OFFSET;
                }
                else if (g_num_mov_flag == 1)
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
                else
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
            }
            else if (para == CONV_OFFSMULT)
            {
                if (g_num_mov_flag == 0)
                {
                    i = 24;
                    patRulesTmp = PATRULES_CONVERT1_OFFSMULT;
                }
                else if (g_num_mov_flag == 1)
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
                else
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
            }
            else if (para == CONV_CHANGE)
            {
                if (g_num_mov_flag == 0)
                {
                    i = 24;
                    patRulesTmp = PATRULES_CONVERT1_OFFSET;
                }
                else if (g_num_mov_flag == 1)
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
                else
                {
                    i = 0;
                    patRulesTmp = PATRULES_CONVERT1_MOVE;
                }
            }
            else
            {
                i = 0;
                patRulesTmp = PATRULES_CONVERT1_MOVE;
            }
            pat_setPatRule(patRulesTmp);
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(patRulesTmp, scale1, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
        }
        // /[更新花样显示*end]**********************************************************

        // /[界面的edit数据刷新*begin]**********************************************************
        if (dis_updata3)
        {
            if (para == CONV_ORGSUB) //原点修正
            {
            }
            else if (para == CONV_SCALE) //放大缩小
            {
                if (g_num_mov_flag == 2)
                {
                    for (i = 0; i < (((F_FILETYPE == PAT_MIT) || (tmp_selectmode == 0)) ? 2 : 3); i++)
                    {
                        //绘制文本框
                        f = KEY_CONVERT1_3XSCALE + i;
                        DrawButtonEdit(keyd[f],
                                       (editflag == i) ? keybmpind[f].textColor[0] : keybmpind[f].icon[0],
                                       (editflag == i) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                    }

                    DrawMFS32P(rect[RECT_CONVERT1_3EDIT1_STR].xpis + (rect[RECT_CONVERT1_3EDIT1_STR].xsize - 5 * 13) / 2,
                               rect[RECT_CONVERT1_3EDIT1_STR].ypis + (rect[RECT_CONVERT1_3EDIT1_STR].ysize - 24) / 2,
                               (editflag == 0) ? tmp_parameter : g_convert_xscale, 5, 1,
                               (editflag == 0) ? rect[RECT_CONVERT1_3EDIT1_STR].colorT[0] : rect[RECT_CONVERT1_3EDIT1_STR].colorT[0]);

                    DrawMFS32P(rect[RECT_CONVERT1_3EDIT2_STR].xpis + (rect[RECT_CONVERT1_3EDIT2_STR].xsize - 5 * 13) / 2,
                               rect[RECT_CONVERT1_3EDIT2_STR].ypis + (rect[RECT_CONVERT1_3EDIT2_STR].ysize - 24) / 2,
                               (editflag == 1) ? tmp_parameter : g_convert_yscale, 5, 1,
                               (editflag == 1) ? rect[RECT_CONVERT1_3EDIT2_STR].colorT[0] : rect[RECT_CONVERT1_3EDIT2_STR].colorT[0]);

                    if ((F_FILETYPE == PAT_SEW) && (tmp_selectmode == 1))
                    {

                        DrawMFS32P(rect[RECT_CONVERT1_3EDIT3_STR].xpis + (rect[RECT_CONVERT1_3EDIT3_STR].xsize - 4 * 13) / 2,
                                   rect[RECT_CONVERT1_3EDIT3_STR].ypis + (rect[RECT_CONVERT1_3EDIT3_STR].ysize - 24) / 2,
                                   (editflag == 2) ? tmp_parameter : g_convert_pindis, 4, 1,
                                   (editflag == 2) ? rect[RECT_CONVERT1_3EDIT3_STR].colorT[0] : rect[RECT_CONVERT1_3EDIT3_STR].colorT[0]);
                        //绘制文本框
                        DrawButtonEdit(keyd[KEY_CONVERT1_9ANG],
                                       (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].textColor[0] : keybmpind[KEY_CONVERT1_9ANG].icon[0],
                                       (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].icon[1] : keybmpind[KEY_CONVERT1_9ANG].icon[0]);
                        DrawMFS32P(rect[RECT_CONVERT1_9ANG_STR].xpis + (rect[RECT_CONVERT1_9ANG_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 24) / 2,
                                   (editflag == 3) ? tmp_parameter : ConAng, 2, 0,
                                   (editflag == 3) ? rect[RECT_CONVERT1_9ANG_STR].colorT[0] : rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                    }
                }
            }
            else if (para == CONV_MIRROR) //对称
            {
            }
            else if (para == CONV_ROTATE) //旋转
            {
                if (g_num_mov_flag == 2)
                {

                    DrawEdit(rect[RECT_CONVERT1_5EDIT1_STR], rect[RECT_CONVERT1_5EDIT1_STR].colorT[0], rect[RECT_CONVERT1_5EDIT1_STR].colorB[1]);
                    DrawMFS32P(rect[RECT_CONVERT1_5EDIT1_STR].xpis + (rect[RECT_CONVERT1_5EDIT1_STR].xsize - 5 * 13) / 2,
                               rect[RECT_CONVERT1_5EDIT1_STR].ypis + (rect[RECT_CONVERT1_5EDIT1_STR].ysize - 24) / 2,
                               tmp_parameter, 5, 1, rect[RECT_CONVERT1_5EDIT1_STR].colorT[0]);
                }
            }
            else if (para == CONV_OFFSET) //离边缝
            {
                if (g_num_mov_flag == 2)
                {

                    for (i = 0; i < ((F_FILETYPE == PAT_MIT) ? 1 : 2); i++) //距离、针距
                    {
                        //绘制文本框
                        f = KEY_CONVERT1_6DIS + i;
                        DrawButtonEdit(keyd[f],
                                       (editflag == i) ? keybmpind[f].textColor[0] : keybmpind[f].icon[0],
                                       (editflag == i) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                    }
                    //距离
                    DrawMFS32P(rect[RECT_CONVERT1_6DIS_STR].xpis + (rect[RECT_CONVERT1_6DIS_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                               rect[RECT_CONVERT1_6DIS_STR].ypis + (rect[RECT_CONVERT1_6DIS_STR].ysize - 24) / 2,
                               (editflag == 0) ? tmp_parameter : Convedis, 4, 1, rect[RECT_CONVERT1_6DIS_STR].colorT[0]);

                    if (F_FILETYPE == PAT_SEW)
                    {
                        //针距
                        DrawMFS32P(rect[RECT_CONVERT1_6DIS_STR].xpis + (rect[RECT_CONVERT1_6DIS_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                                   rect[RECT_CONVERT1_6DIS_STR].ypis + (rect[RECT_CONVERT1_6DIS_STR].ysize - 24) / 2,
                                   (editflag == 1) ? tmp_parameter : g_convert_pindis, 4, 1, rect[RECT_CONVERT1_6DIS_STR].colorT[0]);
                        //绘制文本框-针角度
                        DrawButtonEdit(keyd[KEY_CONVERT1_9ANG],

                                       (editflag == 2) ? keybmpind[KEY_CONVERT1_9ANG].textColor[0] : keybmpind[KEY_CONVERT1_9ANG].icon[0],
                                       (editflag == 2) ? keybmpind[KEY_CONVERT1_9ANG].icon[1] : keybmpind[KEY_CONVERT1_9ANG].icon[0]);

                        DrawMFS32P(rect[RECT_CONVERT1_9ANG_STR].xpis + (rect[RECT_CONVERT1_9ANG_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 24) / 2,
                                   (editflag == 2) ? tmp_parameter : ConAng, 2, 0,
                                   (editflag == 2) ? rect[RECT_CONVERT1_9ANG_STR].colorT[0] : rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                    }
                }
            }
            else if (para == CONV_OFFSMULT) //多重缝
            {
                if (g_num_mov_flag == 2)
                {

                    for (i = 0; i < ((F_FILETYPE == PAT_MIT) ? 2 : 3); i++) //距离、次数、针距
                    {
                        //绘制文本框
                        f = KEY_CONVERT1_7SELDIST + i;
                        DrawButtonEdit(keyd[f],
                                       (editflag == i) ? keybmpind[f].textColor[0] : keybmpind[f].icon[0],
                                       (editflag == i) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                    }
                    //距离
                    DrawMFS32P(rect[RECT_CONVERT1_7DIST_STR].xpis + (rect[RECT_CONVERT1_7DIST_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                               rect[RECT_CONVERT1_7DIST_STR].ypis + (rect[RECT_CONVERT1_7DIST_STR].ysize - 24) / 2,
                               (editflag == 0) ? tmp_parameter : g_distance, 4, 1, rect[RECT_CONVERT1_7DIST_STR].colorT[0]);

                    //次数
                    DrawMFS32P(rect[RECT_CONVERT1_7TIME_STR].xpis + (rect[RECT_CONVERT1_7TIME_STR].xsize - SAFE_DIST5 - 1 * 13) / 2,
                               rect[RECT_CONVERT1_7TIME_STR].ypis + (rect[RECT_CONVERT1_7TIME_STR].ysize - 24) / 2,
                               (editflag == 1) ? tmp_parameter : g_times, 1, 0, rect[RECT_CONVERT1_7TIME_STR].colorT[0]);

                    if (F_FILETYPE == PAT_SEW)
                    {
                        //针距
                        DrawMFS32P(rect[RECT_CONVERT1_7PITCH_STR].xpis + (rect[RECT_CONVERT1_7PITCH_STR].xsize - SAFE_DIST5 - 4 * 13) / 2,
                                   rect[RECT_CONVERT1_7PITCH_STR].ypis + (rect[RECT_CONVERT1_7PITCH_STR].ysize - 24) / 2,
                                   (editflag == 2) ? tmp_parameter : g_convert_pindis, 4, 1, rect[RECT_CONVERT1_7PITCH_STR].colorT[0]);

                        //绘制文本框-针角度
                        DrawButtonEdit(keyd[KEY_CONVERT1_9ANG],
                                       (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].textColor[0] : keybmpind[KEY_CONVERT1_9ANG].icon[0],
                                       (editflag == 3) ? keybmpind[KEY_CONVERT1_9ANG].icon[1] : keybmpind[KEY_CONVERT1_9ANG].icon[0]);

                        DrawMFS32P(rect[RECT_CONVERT1_9ANG_STR].xpis + (rect[RECT_CONVERT1_9ANG_STR].xsize - 2 * 13) / 2,
                                   rect[RECT_CONVERT1_9ANG_STR].ypis + (rect[RECT_CONVERT1_9ANG_STR].ysize - 24) / 2,
                                   (editflag == 3) ? tmp_parameter : ConAng, 2, 0,
                                   (editflag == 3) ? rect[RECT_CONVERT1_9ANG_STR].colorT[0] : rect[RECT_CONVERT1_9ANG_STR].colorT[0]);
                    }
                }
            }
            else if (para == CONV_CHANGE && F_FILETYPE == PAT_SEW) //首尾变换
            {
            }

            dis_updata3 = 0;
        }
        // /[界面的edit数据刷新*end]**********************************************************

        // /[刷新相对坐标信息*begin]**********************************************************
        if (dis_updata15)
        {
            dis_updata15 = 0;
            DrawFillRect(rect[RECT_CONVERT1_X_STR].xpis,
                         rect[RECT_CONVERT1_X_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_CONVERT1_X_STR].ysize,
                         rect[RECT_CONVERT1_X_STR].colorB[0]);
            DrawFS32P(rect[RECT_CONVERT1_X_STR].xpis + SAFE_DIST3, rect[RECT_CONVERT1_X_STR].ypis + SAFE_DIST4,
                      g_dxpos, 6, 1, rect[RECT_CONVERT1_X_STR].colorT[0]);
            DrawFillRect(rect[RECT_CONVERT1_Y_STR].xpis,
                         rect[RECT_CONVERT1_Y_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_CONVERT1_Y_STR].ysize,
                         rect[RECT_CONVERT1_Y_STR].colorB[0]);
            DrawFS32P(rect[RECT_CONVERT1_Y_STR].xpis + SAFE_DIST3, rect[RECT_CONVERT1_Y_STR].ypis + SAFE_DIST4,
                      g_dypos, 6, 1, rect[RECT_CONVERT1_Y_STR].colorT[0]);
        }
        // /[刷新相对坐标信息*end]**********************************************************

        // /[定时200ms信息处理*begin]**********************************************************
        if (Timerms > 200 && dis_pos)
        {
            Timerms = 0;
            if (Read_xyposi())
            {

                if (absposx != g_xabspos)
                {
                    absposx = g_xabspos;
                    DrawFillRect(rect[RECT_CONVERT1_X_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_CONVERT1_X_STR].ypis,
                                 rect[RECT_CONVERT1_X_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_CONVERT1_X_STR].ysize,
                                 rect[RECT_CONVERT1_X_STR].colorB[0]);
                    DrawF16Str(rect[RECT_CONVERT1_X_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_CONVERT1_X_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_CONVERT1_X_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos, 6, 1, Color_Black);
                    }
#else

                    DrawFS32P(rect[RECT_CONVERT1_X_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_CONVERT1_X_STR].ypis + SAFE_DIST4,
                              g_xabspos, 6, 1, rect[RECT_CONVERT1_X_STR].colorT[0]);
#endif
                }
                if (absposy != g_yabspos)
                {
                    absposy = g_yabspos;
                    DrawFillRect(rect[RECT_CONVERT1_Y_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_CONVERT1_Y_STR].ypis,
                                 rect[RECT_CONVERT1_Y_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_CONVERT1_Y_STR].ysize,
                                 rect[RECT_CONVERT1_Y_STR].colorB[0]);
                    DrawF16Str(rect[RECT_CONVERT1_Y_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_CONVERT1_Y_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_CONVERT1_Y_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos, 6, 1, Color_Black);
                    }
#else
                    DrawFS32P(rect[RECT_CONVERT1_Y_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_CONVERT1_Y_STR].ypis + SAFE_DIST4,
                              g_yabspos, 6, 1, rect[RECT_CONVERT1_Y_STR].colorT[0]);
#endif
                }
            }
        }
        // /[定时200ms信息处理*end]**********************************************************

        // /[中压脚按钮刷新*begin]**********************************************************
        if (pfootf != (U16)(S_PFOOT))
        {
            pfootf = (U16)(S_PFOOT);
            DrawButton(keyd[KEY_CONVERT1_PFOOT], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_CONVERT1_PFOOT].border[0],
                       (pfootf == 0) ? keybmpind[KEY_CONVERT1_PFOOT].icon[0] : keybmpind[KEY_CONVERT1_PFOOT].icon[1]);
        }
        // /[中压脚按钮刷新	*end]**********************************************************

        // /[页面切换*begin]**********************************************************
        if (pno != MENU_CONVERT1)
        {
            plastno = MENU_CONVERT1;
            g_temp_pno = pno;
            break;
        }
        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
#endif
}
