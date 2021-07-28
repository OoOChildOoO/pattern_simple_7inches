/*
 * @Author: your name
 * @Date: 2021-05-06 08:44:44
 * @LastEditTime: 2021-05-06 14:52:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\src\menu_help.c
 */
#include "menu_help.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "color.h"
#include "communication.h"
#include "aviplayer.h"

//--------------------------------------------
#define KEY_SPTCNT_SCRNSVR 1
#define KEY_SPTCNT_BACK 2

#define KEY_SPTCNT_MAX 3

#define RECT_SPTCNT_MAX 1
//------------------------------------------
// Function   : void Menu_HeSupportConnect(U8 para)
// Description: 技术连线
//------------------------------------------
void Menu_HeSupportConnect(U8 para)
{
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U16 i;

    const U8 keynum = {KEY_SPTCNT_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                    //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1}, //2 返回
    };
    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},           //1 屏保
        {{KEY_Silver}, {0}, {72, 0, 0, 0}, {2, 0, 0, 0}, {0}}, //2 返回

    };
    const RECTPAD rect[RECT_SPTCNT_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {3}},
    };

    SetLanguage(MENU_HELP);
    InitScreen();
    Draw24x24Str(TITLE_XPOS, SAFE_DIST4, (MEM_TITLE_ADD + (para + rect[0].index[0]) * TitleLengthMax), SheetPage, rect[0].index[0], rect[0].colorT[0]);

    for (i = 2; i < KEY_SPTCNT_MAX; i++)
    {
        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
    }

    //    if (g_bootscr.vmlk)
    //    {
    Draw24x24Str(TYPE_X12 - (11 * 24 / 2), TYPE_Y10,
                 (MEM_TITLE_ADD + 9 * TitleLengthMax), SheetPage, 9, Color_Black);
    Draw24x24Str(TYPE_X12 - (6 * 24 / 2), TYPE_Y10 + SAFE_DIST11,
                 (MEM_TITLE_ADD + 10 * TitleLengthMax), SheetPage, 10, Color_Black);
    Draw24x24Str(TYPE_X12 - (16 * 12 / 2), TYPE_Y10 + SAFE_DIST11 * 2,
                 (MEM_TITLE_ADD + 11 * TitleLengthMax), SheetPage, 11, Color_Black);
    Draw24x24Str(TYPE_X12 - (14 * 12 / 2), TYPE_Y10 + SAFE_DIST11 * 3,
                 (MEM_TITLE_ADD + 12 * TitleLengthMax), SheetPage, 12, Color_Black);
    Draw24x24Str(TYPE_X12 - (22 * 12 / 2), TYPE_Y10 + SAFE_DIST11 * 4,
                 (MEM_TITLE_ADD + 13 * TitleLengthMax), SheetPage, 13, Color_Black);
    //    }

    ViewPage(usepageno);
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, keynum, 0);
        if (keydata == 0xff) //按键松开后的键值预处理
        {
            //带有连续按键功能的按键
            if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //按键按下的键值预处理    与提示性信息处理
        {
            //特定情况下不生效按键
            if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
            {
                keyon = keydata;
                if (keyon == KEY_SPTCNT_SCRNSVR) //无按键特效
                {
                }
                //非常规按键-//绘制颜色文字按键
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]); //常规按键
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
                Times = 0;
            }
        }
        if (keyoff)
        {
            switch (keyoff)
            {
            case KEY_SPTCNT_SCRNSVR:
                pno = MENU_SCRNSVR;
                break;
            case KEY_SPTCNT_BACK:
                pno = poldno[0];
                pnoindex = 0;
                break;
            }
            keyoff = 0;
        }
        if ((keyon) && (keydata == 0))
        {
            if (keyon == KEY_SPTCNT_SCRNSVR) //不需要显示的按键
            {
            }
            //非常规按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回P1
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, (MEM_TITLE_ADD + (para + rect[0].index[0]) * TitleLengthMax),
                         SheetPage, rect[0].index[0], rect[0].colorT[0]);

            keyon = 0;
            beeponf = 0;
        }
        if (pno != MENU_HELP)
        {
            plastno = MENU_HELP;
            break;
        }
        //if(pno != MENU_SPTCNT){plastno = MENU_SPTCNT; break;}
        wdt();
    }
}
