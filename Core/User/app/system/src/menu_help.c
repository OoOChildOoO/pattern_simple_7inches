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
// Description: ��������
//------------------------------------------
void Menu_HeSupportConnect(U8 para)
{
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U16 i;

    const U8 keynum = {KEY_SPTCNT_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                    //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1}, //2 ����
    };
    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},           //1 ����
        {{KEY_Silver}, {0}, {72, 0, 0, 0}, {2, 0, 0, 0}, {0}}, //2 ����

    };
    const RECTPAD rect[RECT_SPTCNT_MAX] = {
        //����
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
        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            //���������������ܵİ���
            if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
            {
                keyon = keydata;
                if (keyon == KEY_SPTCNT_SCRNSVR) //�ް�����Ч
                {
                }
                //�ǳ��水��-//������ɫ���ְ���
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]); //���水��
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
                    switch (keyon) //�������±�����ʾ����ʾ
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
            if (keyon == KEY_SPTCNT_SCRNSVR) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ��P1
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
