/*
 * @Author: your name
 * @Date: 2021-05-12 13:18:25
 * @LastEditTime: 2021-07-27 17:00:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\src\menu_function.c
 */

#include <string.h>
#include "menu_function.h"
#include "main.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "uart.h"
#include "function.h"
#include "ds2460.h"
#include "communication.h"
#include "color.h"
#include "disk_oper.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"
#include "viewPattern.h"
#include "Mit_menu_files.h"
#include "Mit_funtion.h"
#include "menu_p.h"

//#include "..\..\Lib\usb\app_inc\usbh_usr.h"

extern void readUsrMsg(void);
extern U8 getBootScreenMxg(void);
extern U8 anyWriteScreeData(void);
extern U8 WriteScreeData(U16 psx, U16 psy, U16 pslen);
extern U8 TF_Init(void);
// uint8_t Check_AviFile(char *filename);

extern volatile U8 gplayLogoAvi;

extern U16 SheetPage; //多国语言Sheet表页码，从0开始
extern MachineInfo g_Mac;
extern volatile U16 g_Fileno;
extern volatile U8 g_backSREG_f; // 备份SERG数据标志
extern volatile U8 g_backHREG_f; // 备份HERG数据标志
extern volatile U32 g_backSREGlen;
extern volatile U8 g_MboardUSB;
extern volatile U8 g_disk_memory;
extern volatile U8 g_UdZhouFlag;
extern volatile U8 g_NoReadPatf;
extern volatile U16 g_selectdisno;
extern volatile U16 g_oldselectdisno;
extern volatile U8 gPeessVal;
extern volatile U8 g_uperr;
extern volatile U8 g_usersave;
extern volatile U8 g_netSelectMac;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos, g_wabspos;
extern volatile U8 g_NoUpdataoldno; //不更新旧花样编号
extern volatile S16 g_upDISP;       // 升级打板器
extern volatile S16 g_upFLASH;      // 升级打板器 FLASH
extern volatile S16 g_upMBOARD;     // 升级主板
extern volatile S16 g_upWIFI;       // 升级wifi模块
extern volatile U8 g_checkGroup;    //组合图形检查
extern volatile U16 g_Groupsel1;
extern volatile U16 g_Combinepat[GroupMax][6]; //组合图形选择花样
extern volatile U8 g_GroupSave;                //保存组合图形 0-不保存  1-保存到内存  2-保存到U盘 3-保存到SD卡 4-保存到默认位置
extern volatile U8 g_GroupCover;               //组合图形覆盖标志
extern volatile U8 g_GroupDoingNO;             //组合图形正在操作编号
extern volatile U8 g_GroupMenth;               //当前组合图形操作类型  0-  1-保存  2-读取   3-清除当前组合图型
extern volatile U8 g_GroupPatNums;             //读取组合文件数
extern volatile U8 g_GroupName[9];
extern volatile U8 g_GroupType;
extern volatile U8 g_tmpGroName[10];
extern volatile U8 g_runstepf;
extern select_id_message g_SelectNo[SELECTNO_MAX]; //被选中的花样文件菜单信息
extern volatile U8 g_ModHome;                      //0-修正M轴原点 1-修正D轴原点
extern volatile U8 g_KeyLockStaus;
extern volatile U16 g_MachineTy;
extern ProInfo g_ProInfo;
extern volatile U8 g_wifiname[21];
extern volatile U8 g_nameidex;
extern volatile U8 g_ssid[20];
extern volatile U8 g_ssididex;
extern volatile U8 g_password[40];
extern volatile U8 g_passwordidex;
extern volatile U8 g_mac[6];
extern volatile U32 g_id;
extern BOOTSCR g_bootscr;

extern volatile U8 g_Funtype;
extern volatile U8 g_GreatPatType;
extern volatile U16 g_temp_clr_flag;
extern volatile U8 g_GreatPatType; //生成花样的类型
extern volatile U8 g_code_mode;
extern volatile U8 g_modifyflag;
extern volatile U8 g_editflag;
extern volatile U8 g_ntohome;
extern volatile U8 g_enctTackCode;

extern volatile U16 g_array6[302];
extern volatile U16 g_array12[120];

extern volatile S16 g_PinModParaANG; // 针杆旋转修正参数_0度位置角度

extern volatile S16 g_PinModChkSum;  // 针杆旋转修正参数校验值
extern volatile S16 g_PinModParaX1;  // 针杆旋转修正参数_90度修正坐标X
extern volatile S16 g_PinModParaY1;  // 针杆旋转修正参数_180度修正坐标Y
extern volatile S16 g_PinModParaX2;  // 针杆旋转修正参数_180度修正坐标X
extern volatile S16 g_PinModParaY2;  // 针杆旋转修正参数_180度修正坐标Y
extern volatile S16 g_PinModParaX3;  // 针杆旋转修正参数_270度修正坐标X
extern volatile S16 g_PinModParaY3;  // 针杆旋转修正参数_270度修正坐标Y
extern volatile S16 g_PinModParaXR;  // 针杆旋转修正参数_圆心坐标X
extern volatile S16 g_PinModParaYR;  // 针杆旋转修正参数_圆心坐标Y
extern volatile S16 g_PinModParaRD;  // 针杆旋转修正参数_半径
extern volatile S16 g_PinModParaANG; // 针杆旋转修正参数_0度位置角度

extern volatile S16 g_OrgModChkSum; // 机械原点修正参数校验
extern volatile S16 g_OrgModParaX;  // 机械原点修正参数_修正坐标X
extern volatile S16 g_OrgModParaY;  // 机械原点修正参数_修正坐标Y
extern volatile S16 g_OrgModParaXD; // 机械原点D轴修正参数_修正坐标X
extern volatile S16 g_OrgModParaYD; // 机械原点D轴修正参数_修正坐标Y

extern volatile S16 g_GapModChkSum;  // 正反转间隙修正参数校验
extern volatile S16 g_GapModPosadjX; //正反转间隙修正参数_修正坐标X+
extern volatile S16 g_GapModNegadjX; //正反转间隙修正参数_修正坐标X-
extern volatile S16 g_GapModPosadjY; //正反转间隙修正参数_修正坐标Y+
extern volatile S16 g_GapModNegadjY; //正反转间隙修正参数_修正坐标Y-

extern volatile S16 g_ViewModParaX; //视觉坐标系偏移X
extern volatile S16 g_ViewModParaY; //视觉坐标系偏移Y

extern volatile U8 g_disppicflag;    //显示标志位
extern volatile U8 g_namenumberflag; //读写界面的名称/序号切换

#define TLFNADD_FUN_MAIN 0
#define TLFNLEN_FUN_MAIN 20
#define TLFNADD_FUN_COPY (TLFNADD_FUN_MAIN + TLFNLEN_FUN_MAIN)
#define TLFNLEN_FUN_COPY 26
#define TLFNADD_FUN_NEEDLE (TLFNADD_FUN_COPY + TLFNLEN_FUN_COPY)
#define TLFNLEN_FUN_NEEDLE 23
#define TLFNADD_FUN_FORMART (TLFNADD_FUN_NEEDLE + TLFNLEN_FUN_NEEDLE)
#define TLFNLEN_FUN_FORMART 6
#define TLFNADD_FUN_GROUP (TLFNADD_FUN_FORMART + TLFNLEN_FUN_FORMART)
#define TLFNLEN_FUN_GROUP 34
#define TLFNADD_FUN_READMEM (TLFNADD_FUN_GROUP + TLFNLEN_FUN_GROUP)
#define TLFNLEN_FUN_READMEM 23
#define TLFNADD_FUN_READCOM (TLFNADD_FUN_READMEM + TLFNLEN_FUN_READMEM)
#define TLFNLEN_FUN_READCOM 29
#define TLFNADD_FUN_WRITECOM (TLFNADD_FUN_READCOM + TLFNLEN_FUN_READCOM)
#define TLFNLEN_FUN_WRITECOM 53
#define TLFNADD_FUN_VER (TLFNADD_FUN_WRITECOM + TLFNLEN_FUN_WRITECOM)
#define TLFNLEN_FUN_VER 14
#define TLFNADD_FUN_SCREEN (TLFNADD_FUN_VER + TLFNLEN_FUN_VER)
#define TLFNLEN_FUN_SCREEN 43
#define TLFNADD_FUN_RESTORE (TLFNADD_FUN_SCREEN + TLFNLEN_FUN_SCREEN)
#define TLFNLEN_FUN_RESTORE 36
#define TLFNADD_FUN_BACKUP (TLFNADD_FUN_RESTORE + TLFNLEN_FUN_RESTORE)
#define TLFNLEN_FUN_BACKUP 12
#define TLFNADD_FUN_USER (TLFNADD_FUN_BACKUP + TLFNLEN_FUN_BACKUP)
#define TLFNLEN_FUN_USER 48
#define TLFNADD_FUN_ERRRECORD (TLFNADD_FUN_USER + TLFNLEN_FUN_USER)
#define TLFNLEN_FUN_ERRRECORD 11
#define TLFNADD_FUN_RUNRECORD (TLFNADD_FUN_ERRRECORD + TLFNLEN_FUN_ERRRECORD)
#define TLFNLEN_FUN_RUNRECORD 31
#define TLFNADD_FUN_NET_M (TLFNADD_FUN_RUNRECORD + TLFNLEN_FUN_RUNRECORD)
#define TLFNLEN_FUN_NET_M 10
#define TLFNADD_FUN_NET_NAME (TLFNADD_FUN_NET_M + TLFNLEN_FUN_NET_M)
#define TLFNLEN_FUN_NET_NAME 50
#define TLFNADD_FUN_NET_LOCAL (TLFNADD_FUN_NET_NAME + TLFNLEN_FUN_NET_NAME)
#define TLFNLEN_FUN_NET_LOCAL 17
#define TLFNADD_FUN_NET_CLOUD (TLFNADD_FUN_NET_LOCAL + TLFNLEN_FUN_NET_LOCAL)
#define TLFNLEN_FUN_NET_CLOUD 20
#define TLFNADD_FUN_RESET (TLFNADD_FUN_NET_CLOUD + TLFNLEN_FUN_NET_CLOUD)
#define TLFNLEN_FUN_RESET 5
#define TLFNADD_FUN_PARAMODIY (TLFNADD_FUN_RESET + TLFNLEN_FUN_RESET)
#define TLFNLEN_FUN_PARAMODIY 40
#define TLFNADD_FUN_SERVO (TLFNADD_FUN_PARAMODIY + TLFNLEN_FUN_PARAMODIY)
#define TLFNLEN_FUN_SERVO 46
#define TLFNADD_FUN_STEPPER (TLFNADD_FUN_PARAMODIY + TLFNLEN_FUN_PARAMODIY)
#define TLFNLEN_FUN_STEPPER 28
#define TLFNADD_FUN_OTHER1 (TLFNADD_FUN_STEPPER + TLFNLEN_FUN_STEPPER)
#define TLFNLEN_FUN_OTHER1 5
#define TLFNADD_FUN_FBAG (TLFNADD_FUN_OTHER1 + TLFNLEN_FUN_OTHER1)
#define TLFNLEN_FUN_FBAG 53
#define TLFNADD_FUN_BOOTSET (TLFNADD_FUN_FBAG + TLFNLEN_FUN_FBAG)
#define TLFNLEN_FUN_BOOTSET 17
#define TLFNADD_FUN_OTHER2 (TLFNADD_FUN_BOOTSET + TLFNLEN_FUN_BOOTSET)
#define TLFNLEN_FUN_OTHER2 43
//----------------------------------------------------------------

#define KEY_FUNCTION_SCRNSVR 1
#define KEY_FUNCTION_BACK 2
#define KEY_FUNCTION_COPY 3       //花样拷贝
#define KEY_FUNCTION_GROUP 4      //组合花样
#define KEY_FUNCTION_FORMAT 5     //格式化存储器
#define KEY_FUNCTION_SYSRESTORE 6 //读取设定
#define KEY_FUNCTION_SYSBACKUP 7  //备份设定
#define KEY_FUNCTION_SYSVERSION 8 //版本号读取
#define KEY_FUNCTION_ERRRECORD 9  //故障信息记录
#define KEY_FUNCTION_RUNRECORD 10 //运行信息记录
#define KEY_FUNCTION_NETWORD 11   //网络服务
#define KEY_FUNCTION_RESET 12     //一键还原
#define KEY_FUNCTION_USER 13      //用户登录
#define KEY_FUNCTION_NEEDLE 14    //针调整
#define KEY_FUNCTION_PARAMODIY 15 //原点机械相关参数设置
#define KEY_FUNCTION_SERVO 16     //伺服参数设置
#define KEY_FUNCTION_STEPPER 17   //步进参数设置
#define KEY_FUNCTION_SCREENSET 18 //屏幕设置
#define KEY_FUNCTION_FILEMAG 19   //文件系统
#define KEY_FUNCTION_MAX 19
//------------------------------------------
// Function   : void Menu_Function(void)
// Description: 功能画面
//------------------------------------------
void Menu_Function(void)
{
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U32 i;
    U8 j, f, groupno, Chinesenum;
    U8 needlef = 0xff;
    U8 selEnalbe[17] = {1, 1, 1, 1, 1,
                        1, 0, 0, 0, 0,
                        1, 0, 0, 0, 0,
                        1, 0};
    // U8 selEnalbe[17] = {1, 1, 1, 1, 1,
    //                     1, 1, 1, 0, 1,
    //                     1, 1, 1, 0, 0,
    //                     1, 1};
    const U8 keynum = {KEY_FUNCTION_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                                                                                 //1 屏保
        {TYPE_X1, TYPE_Y4, KEYX1, KEYY1},                                                                //2 返回
        {TYPE_X18, TYPE_Y3, KEY_X15, KEY_Y15},                                                           //3
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 1, TYPE_Y3, KEY_X15, KEY_Y15},                              //4
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 2, TYPE_Y3, KEY_X15, KEY_Y15},                              //5
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 3, TYPE_Y3, KEY_X15, KEY_Y15},                              //6
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 4, TYPE_Y3, KEY_X15, KEY_Y15},                              //7
        {TYPE_X18, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 1, KEY_X15, KEY_Y15},                              //8
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 1, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 1, KEY_X15, KEY_Y15}, //9
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 2, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 1, KEY_X15, KEY_Y15}, //10
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 3, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 1, KEY_X15, KEY_Y15}, //11
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 4, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 1, KEY_X15, KEY_Y15}, //12
        {TYPE_X18, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 2, KEY_X15, KEY_Y15},                              //13
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 1, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 2, KEY_X15, KEY_Y15}, //14
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 2, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 2, KEY_X15, KEY_Y15}, //15
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 3, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 2, KEY_X15, KEY_Y15}, //16
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 4, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 2, KEY_X15, KEY_Y15}, //17
        {TYPE_X18, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 3, KEY_X15, KEY_Y15},                              //18
        {TYPE_X18 + (KEY_X15 + SAFE_DIST9) * 1, TYPE_Y3 + (KEY_Y15 + SAFE_DIST9) * 3, KEY_X15, KEY_Y15}, //19

    };
    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_MAIN + 1, 0, 0, 0}, {0}},         //1 	屏保
        {{KEY_Silver}, {0}, {241, 242}, {TLFNADD_FUN_MAIN + 12, 0, 0, 0}, {0}}, //2	返回

        {{KEY_Silver}, {0}, {671}, {TLFNADD_FUN_MAIN + 3, 0, 0, 0}, {0}},  //花样拷贝
        {{KEY_Silver}, {0}, {674}, {TLFNADD_FUN_MAIN + 6, 0, 0, 0}, {0}},  //组合花样
        {{KEY_Silver}, {0}, {673}, {TLFNADD_FUN_MAIN + 5, 0, 0, 0}, {0}},  //格式化存储器
        {{KEY_Silver}, {0}, {677}, {TLFNADD_FUN_MAIN + 9, 0, 0, 0}, {0}},  //读取设定
        {{KEY_Silver}, {0}, {678}, {TLFNADD_FUN_MAIN + 10, 0, 0, 0}, {0}}, //备份设定
        {{KEY_Silver}, {0}, {675}, {TLFNADD_FUN_MAIN + 7, 0, 0, 0}, {0}},  //版本号读取
        {{KEY_Silver}, {0}, {680}, {TLFNADD_FUN_MAIN + 12, 0, 0, 0}, {0}}, //故障信息记录
        {{KEY_Silver}, {0}, {681}, {TLFNADD_FUN_MAIN + 13, 0, 0, 0}, {0}}, //运行信息记录
        {{KEY_Silver}, {0}, {687}, {TLFNADD_FUN_MAIN + 14, 0, 0, 0}, {0}}, //网络服务
        {{KEY_Silver}, {0}, {682}, {TLFNADD_FUN_MAIN + 15, 0, 0, 0}, {0}}, //一键还原
        {{KEY_Silver}, {0}, {679}, {TLFNADD_FUN_MAIN + 11, 0, 0, 0}, {0}}, //用户登录
        {{KEY_Silver}, {0}, {672}, {TLFNADD_FUN_MAIN + 4, 0, 0, 0}, {0}},  //针调整
        {{KEY_Silver}, {0}, {683}, {TLFNADD_FUN_MAIN + 16, 0, 0, 0}, {0}}, //原点机械相关参数设置
        {{KEY_Silver}, {0}, {684}, {TLFNADD_FUN_MAIN + 17, 0, 0, 0}, {0}}, //伺服参数设置
        {{KEY_Silver}, {0}, {685}, {TLFNADD_FUN_MAIN + 18, 0, 0, 0}, {0}}, //步进参数设置
        {{KEY_Silver}, {0}, {676}, {TLFNADD_FUN_MAIN + 8, 0, 0, 0}, {0}},  //屏幕设置
        {{KEY_Silver}, {0}, {686}, {TLFNADD_FUN_MAIN + 19, 0, 0, 0}, {0}}, //文件系统
    };
    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FUN_MAIN}},
    };
    SetLanguage(MENU_FUNCTION);
    InitScreen();
    initViewStatus();
    updateViewStatus();

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //P1

    //按钮绘制
    DrawButton(keyd[KEY_FUNCTION_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_FUNCTION_BACK].border[0], keybmpind[KEY_FUNCTION_BACK].icon[0]);
    for (i = KEY_FUNCTION_COPY; i <= KEY_FUNCTION_SCREENSET; i++)
    {
        f = i - KEY_FUNCTION_COPY;
        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        if (selEnalbe[f] == 0)
        {
            DrawColor_Gray1(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
        }
    }

    Read_D(D_Address[SYS] + SYS_WIFI_VER, 1, (U16 *)(&YREG[SYS_WIFI_VER])); //读系WIFI版本号
    if (YREG[SYS_WIFI_VER] != 0)
    {                 //标记为wifi模式
        g_upWIFI = 1; //自动选择升级wifi模式
    }
    else
    {
        g_upWIFI = 0;
    }

    poldno[2] = 0;
    ViewPage(usepageno);
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, keynum, 0);
        if (keydata >= KEY_FUNCTION_COPY && keydata <= KEY_FUNCTION_SCREENSET)
        {
            f = keydata - KEY_FUNCTION_COPY;
            if (selEnalbe[f] == 0)
            {
                keydata = 0;
                resetKeyData();
            }
        }
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

            if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
            {
                keyon = keydata;
                if (keyon == KEY_FUNCTION_SCRNSVR) //无按键特效
                {
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
        else //按键没有按下
        {
            key_on_flag1 = 0;
        }

        if (keyoff)
        {
            switch (keyoff)
            {
            case KEY_FUNCTION_SCRNSVR:
                pno = MENU_SCRNSVR;
                break;
            case KEY_FUNCTION_BACK:
                pno = poldno[--pnoindex];
                break;
            case KEY_FUNCTION_COPY:
                pno = MENU_FTCOPY;
                poldno[pnoindex++] = MENU_FUNCTION;
                for (i = 0; i < DISSUM; i++)
                {
                    g_SelectNo[i].Select_No = 0xffff;
                    g_SelectNo[i].Fileno = 0xffff;
                }
                g_Fileno = F_IND_NO;
                g_backSREG_f = 1;
                g_backHREG_f = 1;
                g_backSREGlen = (F_FILELEN) / 2;
                break;
            case KEY_FUNCTION_NEEDLE:
                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                needlef = (U8)S_NEEDLE; //0x20针在上  0 针在下
                if (needlef != 0x20)
                {
                    //						ErrorMsg(MENU_FUNCTION);break;
                    pno = MENU_RWHELP;
                    hleppara = 94;
                    poldno[pnoindex++] = MENU_FUNCTION;
                    break;
                }
                pno = MENU_FTNEEDLE;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_FORMAT:
                pno = MENU_FTFORMAT;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_GROUP:
                if (P_POF == 1)
                {
                    pno = MENU_RWHELP;
                    hleppara = 117;
                    break;
                }
                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                if (1) /*(S_HOME)||(S_NEWHOME)||(S_2HOME)*/
                {      //回到原点
                    pno = MENU_FTGROUP;
                    poldno[1] = MENU_FUNCTION;
                    pnoindex = 2;
                    g_checkGroup = 0;
                    g_Groupsel1 = 0;
                    g_GroupSave = 1;
                    g_GroupMenth = 0;
                    g_GroupCover = 0;
                    for (i = 0; i < GroupMax; i++) //清除数据
                    {
                        for (j = 0; j < 6; j++)
                        {
                            if (j == 0)
                            {
                                g_Combinepat[i][j] = 0xffff; //图形编号
                            }
                            else
                            {
                                g_Combinepat[i][j] = 0;
                            }
                        }
                    }

                    for (groupno = 0, i = 0; i < GREG[GroupPNumAddr]; i++, groupno++)
                    {
                        //内存里无该花样，跳过
                        while ((IREG2[(GREG[GroupPNoAddr + i] - 1) * FileIndexSize + 1] == 0xffff) && (IREG2[(GREG[GroupPNoAddr + i] - 1) * FileIndexSize] == 0xffff))
                        {
                            i++;
                        }
                        if (i >= GREG[GroupPNumAddr])
                            break;
                        g_Combinepat[groupno][0] = GREG[GroupPNoAddr + i]; //图形花样编号
                        for (j = 0; j < 4; j++)
                        { //图形花样名称,只显示前面8个字符
                            g_Combinepat[groupno][j + 1] = IREG2[(GREG[GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j];
                        }
                        Chinesenum = 0;
                        for (j = 0; j < 4; j++)
                        {
                            if (g_Combinepat[i][j + 1] & 0x80)
                                Chinesenum++;
                            if ((g_Combinepat[i][j + 1] >> 8) & 0x80)
                                Chinesenum++;
                        }
                        if (Chinesenum % 2)
                        { //最后一个字符为中文，显示不完全时不显示该字符
                            g_Combinepat[i][4] &= 0xff;
                        }
                    }
                    g_GroupName[0] = GREG[GroupPNameAddr] & 0xff;
                    g_GroupName[1] = (GREG[GroupPNameAddr] >> 8) & 0xff;
                    g_GroupName[2] = GREG[GroupPNameAddr + 1] & 0xff;
                    g_GroupName[3] = (GREG[GroupPNameAddr + 1] >> 8) & 0xff;
                    g_GroupName[4] = GREG[GroupPNameAddr + 2] & 0xff;
                    g_GroupName[5] = (GREG[GroupPNameAddr + 2] >> 8) & 0xff;
                    g_GroupName[6] = GREG[GroupPNameAddr + 3] & 0xff;
                    g_GroupName[7] = (GREG[GroupPNameAddr + 3] >> 8) & 0xff;
                    g_GroupType = (U8)GREG[GroupPTypeAddr];

                    g_Fileno = F_IND_NO;
                    g_backSREG_f = 1;
                    g_backHREG_f = 1;
                    g_backSREGlen = (F_FILELEN) / 2;
                }
                else
                {
                    pno = MENU_RWHELP;
                    hleppara = 17;
                }

                break;
            case KEY_FUNCTION_SYSVERSION:
                pno = MENU_FTSYSVERSION;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_SCREENSET:
                pno = MENU_FTSCREENSET;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_SYSRESTORE:
                //pno	= MENU_FTSYSRESTORE;
                if ((g_user.newuser > 5) && (g_user.newuser < 10))
                {
                    pno = MENU_FTSYSRESTORE;
                }
                else
                {
                    poldno[2] = MENU_FTSYSRESTORE;
                    pno = MENU_FTUSER;
                }
                poldno[1] = MENU_FUNCTION;
                pnoindex = 2;

                g_upDISP = 1;   //升级打板器
                g_upFLASH = 2;  //自动选择升级FLASH模式
                g_upMBOARD = 1; //升级主板
                break;
            case KEY_FUNCTION_SYSBACKUP:
                pno = MENU_FTSYSBACKUP;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_USER:
                pno = MENU_FTUSER;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_ERRRECORD:
                pno = MENU_FTERRRECORD;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_RUNRECORD:
                pno = MENU_FTRUNRECORD;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            // case KEY_FUNCTION_NETWORD:
            //     g_Fileno = F_IND_NO;
            //     g_backSREG_f = 1;
            //     g_backHREG_f = 1;
            //     g_backSREGlen = (F_FILELEN) / 2;

            //     pno = MENU_FTNETWORD;
            //     poldno[1] = MENU_FUNCTION;
            //     pnoindex = 2;
            //     break;
            case KEY_FUNCTION_RESET:
                pno = MENU_FTRESET;
                poldno[pnoindex++] = MENU_FUNCTION;
                break;
            case KEY_FUNCTION_PARAMODIY:
                g_ModHome = 0;
                if ((g_user.newuser > 5) && (g_user.newuser < 10))
                {
                    pno = MENU_PARAMODIFY;
                }
                else
                {
                    poldno[2] = MENU_PARAMODIFY;
                    pno = MENU_FTUSER;
                }
                poldno[1] = MENU_FUNCTION;
                pnoindex = 2;

                //					pno	= MENU_PARAMODIFY;
                //					poldno[1] = MENU_FUNCTION;pnoindex = 2;
                break;

                //             case KEY_FUNCTION_SERVO:
                // #if Debug
                //                 pno = MENU_FTSERVO;
                //                 poldno[1] = MENU_FUNCTION;
                //                 pnoindex = 2;
                // #else
                //                 if ((g_user.newuser > 7) && (g_user.newuser < 10))
                //                 {
                //                     pno = MENU_FTSERVO;
                //                 }
                //                 else
                //                 {
                //                     poldno[2] = MENU_FTSERVO;
                //                     pno = MENU_FTUSER;
                //                 }
                //                 poldno[1] = MENU_FUNCTION;
                //                 pnoindex = 2;
                // #endif
                //                 break;
            default:
                break;
            }
            g_runstepf = 0;
            keyoff = 0;
        }

        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FUNCTION_SCRNSVR) //不需要显示的按键
            {
            }
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                         SheetPage, rect[0].index[0], rect[0].colorT[0]); //标题
        }

        if (pno != MENU_FUNCTION)
        {
            plastno = MENU_FUNCTION;
            break;
        }
        updateViewStatus();
        wdt();
    }
}

//------------------------------------------------------------------------------
#define KEY_FTCOPY_SCRNSVR 1 //1 屏保
#define KEY_FTCOPY_BACK 2    //2 返回
#define KEY_FTCOPY_ENTER 3   //3 确认
#define KEY_FTCOPY_MEN 4     //4 内存
#define KEY_FTCOPY_USB 5     //5 USB
#define KEY_FTCOPY_SD 6      //6 SD
#define KEY_FTCOPY_VIEWPIC 7 //7 显示当前花样信息
#define KEY_FTCOPY_PREV 8    //8 上一页
#define KEY_FTCOPY_NEXT 9    //9 下一页
#define KEY_FTCOPY_ALL 10    //10 全选
#define KEY_FTCOPY_SEL1 11   //11 选框1
#define KEY_FTCOPY_SEL2 12   //12 选框2
#define KEY_FTCOPY_SEL3 13   //13 选框3
#define KEY_FTCOPY_SEL4 14   //14 选框4
#define KEY_FTCOPY_SEL5 15   //15 选框5
#define KEY_FTCOPY_SEL6 16   //16 选框6
#define KEY_FTCOPY_SEL7 17   //17 选框7
#define KEY_FTCOPY_SEL8 18   //18 选框8
#define KEY_FTCOPY_SEL9 19   //19 选框9
#define KEY_FTCOPY_SEL10 20  //20 选框10
#define KEY_FTCOPY_SEL11 21  //21 选框11
#define KEY_FTCOPY_SEL12 22  //22 选框12

#define KEY_FTCOPY_MAX 23

#define RECT_FTCOPY_SEL1 1   //1 选框1
#define RECT_FTCOPY_SEL2 2   //2 选框2
#define RECT_FTCOPY_SEL3 3   //3 选框3
#define RECT_FTCOPY_SEL4 4   //4 选框4
#define RECT_FTCOPY_SEL5 5   //5 选框5
#define RECT_FTCOPY_SEL6 6   //6 选框6
#define RECT_FTCOPY_SEL7 7   //7 选框7
#define RECT_FTCOPY_SEL8 8   //8 选框8
#define RECT_FTCOPY_SEL9 9   //9 选框9
#define RECT_FTCOPY_SEL10 10 //10 选框10
#define RECT_FTCOPY_SEL11 11 //11 选框11
#define RECT_FTCOPY_SEL12 12 //12 选框12
#define RECT_FTCOPY_PAGE 13  //13 换页提示
#define RECT_FTCOPY_MOD 14   //14 模组按键框

#define RECT_FTCOPY_IMG1 15  //15 选框1
#define RECT_FTCOPY_IMG2 16  //16 选框2
#define RECT_FTCOPY_IMG3 17  //17 选框3
#define RECT_FTCOPY_IMG4 18  //18 选框4
#define RECT_FTCOPY_IMG5 19  //19 选框5
#define RECT_FTCOPY_IMG6 20  //20 选框6
#define RECT_FTCOPY_IMG7 21  //21 选框7
#define RECT_FTCOPY_IMG8 22  //22 选框8
#define RECT_FTCOPY_IMG9 23  //23 选框9
#define RECT_FTCOPY_IMG10 24 //24 选框10
#define RECT_FTCOPY_IMG11 25 //25 选框11
#define RECT_FTCOPY_IMG12 26 //26 选框12

#define RECT_FTCOPY_MAX 27

#define BMP_FTCOPY_SEL1 1   //1 选框1
#define BMP_FTCOPY_SEL2 2   //2 选框2
#define BMP_FTCOPY_SEL3 3   //3 选框3
#define BMP_FTCOPY_SEL4 4   //4 选框4
#define BMP_FTCOPY_SEL5 5   //5 选框5
#define BMP_FTCOPY_SEL6 6   //6 选框6
#define BMP_FTCOPY_SEL7 7   //7 选框7
#define BMP_FTCOPY_SEL8 8   //8 选框8
#define BMP_FTCOPY_SEL9 9   //9 选框9
#define BMP_FTCOPY_SEL10 10 //10 选框10
#define BMP_FTCOPY_SEL11 11 //11 选框11
#define BMP_FTCOPY_SEL12 12 //12 选框12

#define BMP_FTCOPY_MAX 13
//------------------------------------------
// Function   : void Menu_FTCOPY(void)
// Description: 读取花样画面
//------------------------------------------
void Menu_FtCopy(void)
{

    U32 i, j, f;

    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
    /****按键相关参数end****/

    /****花样序号，花样名称，花样列表相关参数begin****/
    //	U16 static tmpFno;																 //当前花样信息HREG[0]文件位置(即序号),花样序号缓存
    U16 tmpselectdisno;                                                              //选中的花样序号缓存
    U16 tmp_s_2hd = (U16)(S_2HD);                                                    //次原点位置缓存
    tydPatternList PatternList = {1, 0, 0, 12, 1, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}}; //花样列表参数
    /****花样序号，花样名称，花样列表相关参数end****/

    /****花样显示相关参数begin****/
    S32 Scale = 0; //tscale：图像显示需要调整的缩放比，tscale1：最终的总缩放比，Scale：固定的缩放比

    const S16 orgListX[] = {CONVAS_ORG_PX2_1, CONVAS_ORG_PX2_2, CONVAS_ORG_PX2_3, CONVAS_ORG_PX2_4, CONVAS_ORG_PX2_5,
                            CONVAS_ORG_PX2_6, CONVAS_ORG_PX2_7, CONVAS_ORG_PX2_8, CONVAS_ORG_PX2_9, CONVAS_ORG_PX2_10,
                            CONVAS_ORG_PX2_11, CONVAS_ORG_PX2_12};
    const S16 orgListY[] = {CONVAS_ORG_PY2_1, CONVAS_ORG_PY2_2, CONVAS_ORG_PY2_3, CONVAS_ORG_PY2_4, CONVAS_ORG_PY2_5,
                            CONVAS_ORG_PY2_6, CONVAS_ORG_PY2_7, CONVAS_ORG_PY2_8, CONVAS_ORG_PY2_9, CONVAS_ORG_PY2_10,
                            CONVAS_ORG_PY2_11, CONVAS_ORG_PY2_12};
    /****花样显示相关参数end****/

    /****标志位相关参数begin****/
    U8 resetflag;
    U8 SDFlag = 0, SDFlag1 = 0, USBFlag = 0, USBFlag1 = 0;
    U8 StorType = 0;

    static U8 errorf = 0;
    U8 /*refresh0 = 1, refresh1 = 1,*/ refresh2 = 0, refresh4 = 1, refresh5 = 1;
    U8 /*disupdata1 = 1, disupdata2 = 0,*/ dis_updata3 = 0; //disupdata1:索引更新标志 disupdata2：当前文件所在索引位置更新标志  disupdata3：花样数据更新标志
    /****标志位相关参数end***/

    const U8 keynum = {KEY_FTCOPY_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                                             //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                          //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                                         //3 确认
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},                                         //4 内存
        {TYPE_X10 - KEY_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                //5 USB
        {TYPE_X10 - KEY_X1 * 2, TYPE_Y4, KEY_X1, KEY_Y1},                            //6 SD
        {TYPE_X11, TYPE_Y3, KEY_X1, KEY_Y1},                                         //7 显示当前花样信息
        {TYPE_X11, TYPE_Y14, KEY_X1, KEY_Y1},                                        //8 上一页
        {TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5 * 2 + TYPE_SIZE4, KEY_X1, KEY_Y1}, //9 下一页
        {TYPE_X10 - KEY_X1 * 3 - SAFE_DIST1, TYPE_Y4, KEY_X1, KEY_Y1},               //10 全选
        {CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //14 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //15 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //16 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //17 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //18 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //19 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //20 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //21 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //22 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},           //23 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},           //24 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},           //25 选框12
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {1, 0, 0, 0}, {0}},                                                    //1 屏保
        {{KEY_Silver}, {0}, {72}, {TLFNADD_FUN_COPY + 1, 0, 0, 0}, {0}},                                //2 返回
        {{KEY_Silver}, {0}, {71}, {TLFNADD_FUN_COPY + 2, 0, 0, 0}, {0}},                                //3 确认
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {257, 258, 259}, {TLFNADD_FUN_COPY + 6, 0, 0, 0}, {0}}, //4 内存
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {251, 252, 253}, {TLFNADD_FUN_COPY + 7, 0, 0, 0}, {0}}, //5 USB
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {254, 255, 256}, {TLFNADD_FUN_COPY + 8, 0, 0, 0}, {0}}, //6 SD
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {11, 12, 13}, {TLFNADD_FUN_COPY + 19, 0, 0, 0}, {0}},   //7 显示当前花样信息
        {{KEY_Silver}, {0}, {219, 220}, {TLFNADD_FUN_COPY + 4, 0, 0, 0}, {0}},                          //8 上一页
        {{KEY_Silver}, {0}, {221, 222}, {TLFNADD_FUN_COPY + 5, 0, 0, 0}, {0}},                          //9 下一页
        {{KEY_Silver}, {0}, {246}, {TLFNADD_FUN_COPY + 3, 0, 0, 0}, {0}},                               //10 全选

        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 9, 0, 0, 0}, {0}},  //11 选框1
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 10, 0, 0, 0}, {0}}, //12 选框2
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 11, 0, 0, 0}, {0}}, //13 选框3
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 12, 0, 0, 0}, {0}}, //14 选框4
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 13, 0, 0, 0}, {0}}, //15 选框5
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 14, 0, 0, 0}, {0}}, //16 选框6
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 15, 0, 0, 0}, {0}}, //17 选框7
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 16, 0, 0, 0}, {0}}, //18 选框8
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 17, 0, 0, 0}, {0}}, //19 选框9
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 18, 0, 0, 0}, {0}}, //20 选框10
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 20, 0, 0, 0}, {0}}, //21 选框11
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_COPY + 21, 0, 0, 0}, {0}}, //22 选框12
    };

    const RECTPAD rect[RECT_FTCOPY_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_COPY}},

        {CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //1 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //2 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //3 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //4 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //5 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //6 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //7 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //8 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //9 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                       //10 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                       //11 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                       //12 选框12
        {TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                          //13 换页提示
        {TYPE_X10 - KEY_X1 * 2 - SAFE_DIST4, TYPE_Y4 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //14 模组按键框
        {CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //15 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //16 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //17 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //18 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //19 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //20 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //21 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //22 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //23 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                    //24 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                    //25 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                    //26 选框12
    };

    const BMPPAD disBmp[BMP_FTCOPY_MAX] = {
        //icon
        {0, 0, 0, 0, {0}},

        {CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //1 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //2 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //3 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //4 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //5 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //6 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //7 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //8 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //9 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //10 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //11 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //12 选框12

    };
    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_READ);

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************

    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //读取模式
    //画出方框
    for (i = 1; i < RECT_FTCOPY_MAX; i++)
    {
        if (i <= RECT_FTCOPY_SEL12)
        {
            BmpFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize);
        }
        else
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
        }
    }
    //画出按键显示
    for (i = 2; i < KEY_FTCOPY_MAX; i++)
    {

        if (i == KEY_FTCOPY_VIEWPIC) //无效按键
        {
            continue;
        }
        //不需要显示的按键
        else if (i >= KEY_FTCOPY_SEL1 && i <= KEY_FTCOPY_SEL12) //非常规按钮
        {
            // DrawButtonDisPat(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
        else //常规按钮
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    //画出BMP显示
    for (i = 1; i < BMP_FTCOPY_MAX; i++)
    {
        DrawFPBMPBTN1(disBmp[i].xpis, disBmp[i].ypis, disBmp[i].xsize, disBmp[i].ysize, disBmp[i].icon[0]);
    }
    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************
    g_disk_memory = 0;
    if (ExtMemory_Status(USBTF) == 0)
    {
        USBFlag = 1;
        USBFlag1 = 1;
    }
    resetflag = g_disk_memory + 1;
    ViewPage(usepageno); //设置显示缓存

    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    tmpselectdisno = g_oldselectdisno;
    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0);          //获取键值
        if (keyon >= KEY_FTCOPY_SEL1 && keyon <= KEY_FTCOPY_SEL12) //非常规按键
        {
            f = keydata - KEY_FTCOPY_SEL1;
            if (PatternList.selectEnable[f] == 0)
                keydata = 0;
        }
        // /[按键键值返回预处理*begin]**********************************************************
        //按键松开后的键值预处理
        if (keydata == KEY_FTCOPY_VIEWPIC) //无效按键
        {
            keydata = 0;
        }

        //特定情况下不生效按键预处理

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
            if (((keydata == KEY_FTCOPY_USB) && (USBFlag == 0)) || ((keydata == KEY_FTCOPY_SD) && (SDFlag == 0)))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
                {
                    keyon = keydata;
                    if (keyon == KEY_FTCOPY_SCRNSVR ||
                        (keyon >= KEY_FTCOPY_SEL1 && keyon <= KEY_FTCOPY_SEL12)) //无按键特效
                    {
                    }
                    //非常规按键
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
                    //带有长按连续功能按键的功能
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
            if (keyoff >= KEY_FTCOPY_SEL1 && keyoff <= KEY_FTCOPY_SEL12)
            {

                PatternList.noInPageNow = keyoff - KEY_FTCOPY_SEL1;
                PatternList.bFlashSelectFlag = 1;
            }
            else
            {

                switch (keyoff)
                {
                case KEY_FTCOPY_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_FTCOPY_BACK:
                    if ((ReadMemorryList(MENU_FTCOPY)) == 0) //如选中读优盘,在退出返回时要读回内存索引表
                    {
                        break;
                    }
                    // pno = MENU_FUNCTION;
                    // pnoindex = 1;
                    pno = poldno[--pnoindex];
                    break;
                case KEY_FTCOPY_ENTER:

                    StorType = 0;
                    StorType = (SDFlag << 1) | USBFlag;

                    if (StorType == 0x03)
                    { //USB && SD
                        if (g_disk_memory)
                        {
                            hleppara = 32;
                        }
                        else
                        {
                            hleppara = 84;
                            g_para0 = 31;
                        }
                    }
                    else if ((StorType == 0x02) || (StorType == 0x01))
                    { //USB || SD
                        if (g_disk_memory)
                        {
                            hleppara = 32;
                        }
                        else
                        {
                            hleppara = 31;
                            g_disk_memory = StorType;
                        }
                    }
                    else
                    {
                        hleppara = 95;
                    }

                    // poldno[2] = MENU_FTCOPY;
                    // pnoindex = 3;
                    pno = MENU_RWHELP;
                    poldno[pnoindex] = MENU_FTCOPY;
                    pnoindex++;

                    break;
                case KEY_FTCOPY_PREV:
                    // if (fno > 1)
                    // {
                    //     fno--;
                    //     dis_updata = 1;
                    // }
                    if (PatternList.pageNow > 1)
                    {
                        PatternList.pageNow--;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_FTCOPY_NEXT:
                    // if (fno < Fmax)
                    // {
                    //     fno++;
                    //     dis_updata = 1;
                    // }
                    if (PatternList.pageNow < PatternList.pageNum)
                    {
                        PatternList.pageNow++;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_FTCOPY_MEN:
                    f = ReadFileIndex(C_RDMEMPATLIST, MENU_FTCOPY);
                    if (f == 0)
                    {
                        // pno = MENU_RWHELP;
                        // poldno[2] = MENU_FTCOPY;
                        // pnoindex = 3;
                        hleppara = 15;

                        pno = MENU_RWHELP;
                        poldno[pnoindex] = MENU_FTCOPY;
                        pnoindex++;
                        break;
                    }
                    else
                    {
                        // dis_updata = 1;
                        PatternList.bflashAllFlag = 1;
                        PatternList.bFlashPageFlag = 1;
                        refresh4 = 1;
                        g_disk_memory = 0;
                        resetflag &= 0x7f;
                        if (resetflag != 1)
                            resetflag |= 0x80;
                        //	DrawBMP(0,32,33,33,(U32 *)(MEM_ICON_64x64+200*4356));
                    }
                    break;
                case KEY_FTCOPY_USB:
                    if (ExtMemory_Status(USBTF) != 0)
                    {
                        // pno = MENU_RWHELP;
                        // poldno[2] = MENU_FTCOPY;
                        // pnoindex = 3;
                        hleppara = 95;

                        pno = MENU_RWHELP;
                        poldno[pnoindex] = MENU_FTCOPY;
                        pnoindex++;
                        break;
                    }
                    Select_ExtMemory(USBTF);
                    if (Explore_Disk("0:/", 1))
                    {
                        hleppara = 40;
                        pno = MENU_RWHELP;
                        poldno[pnoindex++] = MENU_FTCOPY;
                        ReadMemorryList(MENU_FTCOPY);
                        break;
                    }
                    // dis_updata = 1;
                    PatternList.bflashAllFlag = 1;
                    PatternList.bFlashPageFlag = 1;
                    g_disk_memory = 1;
                    refresh4 = 1;
                    resetflag &= 0x7f;
                    if (resetflag != 2)
                        resetflag |= 0x80;
                    //}else if(g_MboardUSB == 1){   //使用主板U盘
                    //	f = ReadFileIndex(C_RDFLAPATLIST,MENU_FTCOPY);
                    //	if(f==0){
                    //		pno = MENU_RWHELP;hleppara = 15;poldno[2] = MENU_FTCOPY;pnoindex = 3;break;
                    //	}else{
                    //		dis_updata = 1;
                    //		g_disk_memory = 1;
                    //		resetflag &= 0x7f;
                    //		if(resetflag!=2)resetflag |= 0x80;
                    //	}
                    //}
                    break;
                case KEY_FTCOPY_SD:
                    //if(ExtMemory_Status(SD)!=0){
                    //	pno = MENU_RWHELP;hleppara = 95;poldno[2] = MENU_FTCOPY;pnoindex = 3;break;
                    //}
                    //Select_ExtMemory(SD);
                    //if(Explore_Disk ("1:/", 1)){
                    //	pno = MENU_RWHELP;hleppara = 40;poldno[2] = MENU_FTCOPY;pnoindex = 3;break;
                    //}
                    //dis_updata = 1;
                    //g_disk_memory = 2;
                    //resetflag &= 0x7f;
                    //if(resetflag!=3)resetflag |= 0x80;
                    break;
                case KEY_FTCOPY_ALL:
                    StorType = 0;
                    StorType = (SDFlag << 1) | USBFlag;

                    if (StorType == 0x03)
                    { //USB && SD
                        if (g_disk_memory)
                        {
                            hleppara = 38;
                        }
                        else
                        {
                            hleppara = 84;
                            g_para0 = 37;
                        }
                    }
                    else if ((StorType == 0x02) || (StorType == 0x01))
                    { //USB || SD
                        if (g_disk_memory)
                        {
                            hleppara = 38;
                        }
                        else
                        {
                            hleppara = 37;
                            g_disk_memory = StorType;
                        }
                    }
                    else
                    {
                        hleppara = 95;
                    }
                    // pno = MENU_RWHELP;
                    // poldno[2] = MENU_FTCOPY;
                    // pnoindex = 3;

                    pno = MENU_RWHELP;
                    poldno[pnoindex++] = MENU_FTCOPY;
                    break;
                }
            }
            keyoff = 0;
        }
        // /[按键松开键值处理*end]**********************************************************

        // /[按键弹起显示信息处理*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {

            if (keyon == KEY_FTCOPY_SCRNSVR ||
                (keyon >= KEY_FTCOPY_SEL1 && keyon <= KEY_FTCOPY_SEL12)) //无按键特效
            {
            }
            //非常规按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示复制模式
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
            if (g_disk_memory == 1) //使用U盘
            {
                //if(g_MboardUSB==1){		//使用主板U盘
                //	Write_Cmd(C_DISK_STATUS,0,0);
                //	for(i=0;i<FORTIMS1;i++){
                //		Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));
                //		if((DREG[D_CMDSTATUS] >= 100)||(DREG[D_CMDSTATUS] == 10)){
                //			break;
                //		}
                //		DelayMs(WAITTIME2);
                //	}
                //	if((DREG[D_CMDSTATUS] != 102) && (DREG[D_CMDSTATUS] != 101)){
                //		USBFlag =0;
                //		if((ReadMemorryList(MENU_READ)) == 0){//如选中读优盘,在退出返回时要读回内存索引表
                //			return;
                //		}
                //		g_selectdisno = g_Fileno;
                //		Write_Cmd(C_READMEMORYPAT,g_Fileno,0);
                //		RestoreSHREG(0x03);
                //		pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_READ;pnoindex = 2;//这里有点问题
                //	}else{
                //		USBFlag =1;
                //	}
                //}else{
                if (ExtMemory_Status(USBTF) != 0) //使用打板器U盘
                {
                    USBFlag = 0;
                    hleppara = 95;
                    pno = MENU_RWHELP;
                    poldno[pnoindex] = MENU_FTCOPY;
                    pnoindex++;
                    if ((ReadMemorryList(MENU_FTCOPY)) == 0)
                    { // 如选中读优盘,在退出返回时要读回内存索引表
                        return;
                    }
                    g_selectdisno = g_Fileno;
                    Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                    RestoreSHREG(0x03);
                }
                else
                {
                    USBFlag = 1;
                }
                //}
                //查询一下SD卡插入情况
                //if(ExtMemory_Status(SD)==0){
                //	SDFlag =1;
                //}else{
                //	SDFlag =0;
                //}
            }
            else if (g_disk_memory == 2) // 使用打板器SD
            {
                //	if(ExtMemory_Status(SD)!=0){
                //		SDFlag = 0;
                //		pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_READ;pnoindex = 2;
                //		if((ReadMemorryList(MENU_READ)) == 0){		// 如选中读优盘,在退出返回时要读回内存索引表
                //			return;
                //		}
                //		Write_Cmd(C_READMEMORYPAT,g_Fileno,0);
                //		g_selectdisno = g_Fileno;
                //		RestoreSHREG(0x03);
                //	}else{
                //		SDFlag = 1;
                //	}
                //
                //	//查询一下U盘插入情况
                //	Write_Cmd(C_DISK_STATUS,0,0);	//获取主板U盘插入状态
                //	for(i=0;i<FORTIMS1;i++){
                //		Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));
                //		if((DREG[D_CMDSTATUS] >= 100)||(DREG[D_CMDSTATUS] == 10)){
                //			break;
                //		}
                //		DelayMs(WAITTIME2);
                //	}
                //	if(DREG[D_CMDSTATUS] == 102 || DREG[D_CMDSTATUS] == 101){
                //		USBFlag =1;
                //	}else{
                //		USBFlag =0;
                //	}
                //
                //	if(USBFlag || (ExtMemory_Status(USBTF)==0)) {
                //		USBFlag =1;
                //	}else{
                //		USBFlag =0;
                //	}
            }
            else //使用主板内存
            {
                //查询一下U盘插入情况
                //Write_Cmd(C_DISK_STATUS,0,0);	//获取主板U盘插入状态
                //for(i=0;i<FORTIMS1;i++){
                //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));
                //	if((DREG[D_CMDSTATUS] >= 100)||(DREG[D_CMDSTATUS] == 10)){
                //		break;
                //	}
                //	DelayMs(WAITTIME2);
                //}
                //if(DREG[D_CMDSTATUS] == 102 || DREG[D_CMDSTATUS] == 101){
                //	USBFlag =1;
                //}else{
                //	USBFlag =0;
                //}

                if ((ExtMemory_Status(USBTF) == 0))
                {
                    USBFlag = 1;
                }
                else
                {
                    USBFlag = 0;
                }

                //查询一下SD卡插入情况
                //if(ExtMemory_Status(SD)==0){
                //	SDFlag =1;
                //}else{
                //	SDFlag =0;
                //}
            }
            if (SDFlag != SDFlag1)
            {
                SDFlag1 = SDFlag;
                refresh4 = 1;
            }
            if (USBFlag != USBFlag1)
            {
                USBFlag1 = USBFlag;
                refresh4 = 1;
            }
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[更新花样数据*begin]**********************************************************
        if (dis_updata3)
        {
            U16 chFileNoTemp = 0, cntTemp = 0;
            dis_updata3 = 0;
            memset((U8 *)SREG, 0, sizeof(SREG));
            errorf = 0;
            for (cntTemp = 0; cntTemp < PatternList.disNumDiv + 1; cntTemp++)
            {

                if (cntTemp == PatternList.disNumDiv)
                {
                    chFileNoTemp = g_Fileno;
                }
                else
                {
                    chFileNoTemp = PatternList.dataAdd[cntTemp] / FileIndexSize + MINFILENO;
                }
                if (PatternList.dataAdd[cntTemp] == 0xffff)
                {
                    continue;
                }
                while (errorf < 3 && (chFileNoTemp >= MINFILENO && chFileNoTemp <= MAXFILENO))
                {
                    if (cntTemp == PatternList.disNumDiv) //读回内存中的原数据
                    {
                        i = C_READMEMORYPAT; // 从记忆体读花样文件
                        goto READ_FLASH_ORG;
                    }
                    else if ((g_disk_memory == 0) /*||((g_MboardUSB == 1)&&(g_disk_memory == 1)) */) //从记忆体、主板U盘读花样文件
                    {
                        if (g_disk_memory == 0)
                            i = C_READMEMORYPAT; // 从记忆体读花样文件
                        else
                            i = C_READFLASHPAT; // 从U盘读花样文件
                    READ_FLASH_ORG:
                        if (Write_Cmd(i, chFileNoTemp, 0) == 0)
                        {

                            hleppara = 15;
                            // pno = MENU_RWHELP;
                            // poldno[1] = MENU_READ;
                            // pnoindex = 2;

                            pno = MENU_RWHELP;
                            poldno[pnoindex] = MENU_FTCOPY;
                            pnoindex++;
                            break;
                        }
                        else
                        {
                            if (Read_Status() == 0)
                            {
                                hleppara = 15;
                                // pno = MENU_RWHELP;
                                // poldno[1] = MENU_READ;
                                // pnoindex = 2;

                                pno = MENU_RWHELP;
                                poldno[pnoindex] = MENU_FTCOPY;
                                pnoindex++;
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 98)
                            {
                                //if(g_disk_memory){
                                //	pno = MENU_RWHELP;hleppara = 2;poldno[1] = MENU_READ;pnoindex = 2;break;
                                //}
                            }
                            else if (DREG[D_CMDSTATUS] == 99)
                            {
                                //if(g_disk_memory==0){
                                hleppara = 3;
                                // pno = MENU_RWHELP;
                                // poldno[1] = MENU_READ;
                                // pnoindex = 2;

                                pno = MENU_RWHELP;
                                poldno[pnoindex] = MENU_FTCOPY;
                                pnoindex++;
                                break;
                                //}else{
                                //	pno = MENU_RWHELP;hleppara = 4;poldno[1] = MENU_READ;pnoindex = 2;break;
                                //}
                            }
                            else if (DREG[D_CMDSTATUS] < 98)
                            {
                                hleppara = 16;
                                // pno = MENU_RWHELP;
                                // poldno[1] = MENU_READ;
                                // pnoindex = 2;

                                pno = MENU_RWHELP;
                                poldno[pnoindex] = MENU_FTCOPY;
                                pnoindex++;
                                break;
                            }
                            else
                            {
                                f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                                if (f == 0)
                                {
                                    hleppara = 15;
                                    // pno = MENU_RWHELP;
                                    // poldno[1] = MENU_READ;
                                    // pnoindex = 2;

                                    pno = MENU_RWHELP;
                                    poldno[pnoindex] = MENU_FTCOPY;
                                    pnoindex++;
                                    break;
                                }
                                else
                                {
                                    if (chFileNoTemp != F_IND_NO)
                                    {
                                        hleppara = 16;
                                        // pno = MENU_RWHELP;
                                        // poldno[1] = MENU_READ;
                                        // pnoindex = 2;

                                        pno = MENU_RWHELP;
                                        poldno[pnoindex] = MENU_FTCOPY;
                                        pnoindex++;
                                        break;
                                    }
                                    else
                                    {
                                        i = (chFileNoTemp - MINFILENO) * FileIndexSize;
                                        i = (((U32)(IREG2[i + 1]) << 16) + IREG2[i]) / 2;
                                        f = Read_D(D_Address[PTDATA], i, (U16 *)(&SREG[0]));
                                        if (f == 0)
                                        {
                                            hleppara = 15;
                                            // pno = MENU_RWHELP;
                                            // poldno[1] = MENU_READ;
                                            // pnoindex = 2;

                                            pno = MENU_RWHELP;
                                            poldno[pnoindex] = MENU_FTCOPY;
                                            pnoindex++;
                                            break;
                                        }
                                        else
                                        {
                                            if ((DataCheckSum((U16 *)(&SREG[0]), i, ((((IREG2[(chFileNoTemp - MINFILENO) * FileIndexSize + FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1))) == 0)
                                            { //数据校验失败
                                                errorf++;
                                            }
                                            else
                                            {
                                                for (; i < MAXSREG; i++)
                                                    SREG[i] = 0;
                                                //                                                if (g_disppicflag)
                                                refresh5 = 1;
                                                g_init_flag = 1;
                                                g_data_flash = 1; // 读取花样数据
                                                g_selectdisno = F_IND_NO;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else //读取打板器存储器设备花样
                    {
                        F_IND_NO = chFileNoTemp;
                        /*if(g_disk_memory==1)*/ Select_ExtMemory(USBTF);
                        //else Select_ExtMemory(SD);

                        if (Read_PatFile(chFileNoTemp) == 0)
                        {
                            //                            if (g_disppicflag)
                            refresh5 = 1;
                            g_init_flag = 1;
                            g_data_flash = 1;

                            i = (F_FILELENH << 16) + F_FILELENL;
                            g_selectdisno = F_IND_NO;
                        }
                        else
                        {
                            errorf++;
                        }
                        break;
                    }
                }
                if (errorf == 3)
                {
                    hleppara = 64;
                    errorf = 0;
                    pno = MENU_RWHELP;
                    poldno[pnoindex] = MENU_FTCOPY;
                    pnoindex++;

                    break; //修改增加
                }

                wdt();
                DelayMs(20);
                // /[绘制花样与相关信息*begin]******************************************************************************
                if (refresh5)
                {
                    refresh5 = 0;
                    if (cntTemp == PatternList.disNumDiv)
                    {
                        break;
                    }

                    if (S_2HD) //根据次原点信息分析显示花样的数据
                        AnaPattern(1, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
                    else
                        AnaPattern(0, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
                    pat_setDrawXY(patRule.h_patConvasIndex, rect[cntTemp + RECT_FTCOPY_IMG1].xpis, rect[cntTemp + RECT_FTCOPY_IMG1].ypis);
                    pat_drawPattern(PATRULES_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, F_FILELEN, 0, 0, NULL);
                }
                // /[绘制花样与相关信息*end]******************************************************************************
                wdt();
            }
        }
        // /[更新花样数据*end]**********************************************************

        // /[内存按键处理*begin]**********************************************************
        if (refresh4) // SD卡 内存按键选中处理
        {
            U8 chSelectMem = 0;
            refresh4 = 0;
            chSelectMem = KEY_FTCOPY_MEN + g_disk_memory;

            for (i = KEY_FTCOPY_MEN; i <= KEY_FTCOPY_SD; i++)
            {
                if (i == KEY_FTCOPY_USB)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
                               i == chSelectMem ? keybmpind[i].icon[1] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
                else if (i == KEY_FTCOPY_SD)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
                               i == chSelectMem ? keybmpind[i].icon[1] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
                else if (i == KEY_FTCOPY_MEN)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : keybmpind[i].border[0],
                               i == chSelectMem ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
            }
        }
        // /[内存按键处理*end]**********************************************************

        // /[更新花样显示列表*begin]**********************************************************
        if (PatternList.bflashAllFlag) //索引信息更新
        {
            PatternList.bflashAllFlag = 0;
            PatternList.bFlashPageFlag = 1;

            PatternList.selectNoNow = 0;
            PatternList.patternNum = 0;
            for (i = 0; i < MAXIREG2; i += FileIndexSize) //遍历花样存储空间，获取 最大合花样数量、当前花样序号
            {
                if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
                    (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                {
                    PatternList.patternNum++;
                    if ((i / FileIndexSize + MINFILENO) <= g_selectdisno)
                        PatternList.selectNoNow++;
                }
            }

            //确定 当前选中项、当前选中页、页码总数
            PatternList.noInPageNow = (PatternList.selectNoNow - 1) % PatternList.disNumDiv;
            PatternList.noInPageLast = PatternList.noInPageNow;
            PatternList.pageNow = (PatternList.selectNoNow - 1) / PatternList.disNumDiv + 1;
            PatternList.pageLast = PatternList.pageNow;
            PatternList.pageNum = (PatternList.patternNum - 1) / PatternList.disNumDiv + 1;

            if (PatternList.pageNum == 0) //
                PatternList.pageNum = 1;

            if (PatternList.pageNow == 0)
                PatternList.pageNow = 1;
            else if (PatternList.pageNow > PatternList.pageNum)
                PatternList.pageNow = PatternList.pageNum;

            if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;

            for (i = 0; i < DISSUM; i++)
            {
                g_SelectNo[i].Fileno = 0xffff;
                g_SelectNo[i].Select_No = 0xffff;
            }
        }

        if (PatternList.bFlashSelectFlag) //索引选中项更新
        {
            PatternList.bFlashSelectFlag = 0;

            if (PatternList.dataAdd[PatternList.noInPageNow] == 0xFFFF) //选中空
            {
                PatternList.noInPageNow = PatternList.noInPageLast;
                return;
            }
            else
            {
                PatternList.noInPageLast = PatternList.noInPageNow;
            }

            if (resetflag & 0x80) //选中的是USB
            {
                resetflag = g_disk_memory + 1;
                for (i = 0; i < PatternList.disNumDiv; i++)
                {
                    g_SelectNo[i].Select_No = 0xffff;
                    g_SelectNo[i].Fileno = 0xffff;
                }
            }
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                if (g_SelectNo[i].Fileno == (PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO)) //文件已被选中
                {
                    g_SelectNo[i].Fileno = 0xffff;    //清标志
                    g_SelectNo[i].Select_No = 0xffff; //取消选择
                    break;
                }
            }
            if (i == PatternList.disNumDiv) //文件未被选中
            {
                for (i = 0; i < PatternList.disNumDiv; i++)
                {
                    if (g_SelectNo[i].Fileno == 0xffff)
                    {                                                                                                      //此空间可以存信息,选择按键
                        g_SelectNo[i].Fileno = (PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO); //置标志
                        g_SelectNo[i].Select_No = PatternList.noInPageNow + MINFILENO + (PatternList.pageNow - 1) * PatternList.disNumDiv;
                        break;
                    }
                }
            }

            tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO; //转化为花样序号disno

            if (tmpselectdisno != 0xffff &&
                tmpselectdisno != g_selectdisno)
            {
                PatternList.selectNoNow = PatternList.noInPageNow + MINFILENO + (PatternList.pageNow - 1) * PatternList.disNumDiv;
                PatternList.selectNoLast = PatternList.selectNoNow;
                PatternList.pageLast = PatternList.pageNow;

                // g_selectdisno = tmpselectdisno;//只是勾选，没有确认
                // g_oldselectdisno = g_selectdisno;
            }

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                if (g_SelectNo[i].Fileno != 0xffff) //此空间可以存信息,选择按键
                {
                    if (g_SelectNo[i].Select_No >= (PatternList.pageNow - 1) * PatternList.disNumDiv + MINFILENO &&
                        g_SelectNo[i].Select_No < PatternList.pageNow * PatternList.disNumDiv + MINFILENO)
                    {
                        PatternList.selectList[g_SelectNo[i].Select_No - ((PatternList.pageNow - 1) * PatternList.disNumDiv + MINFILENO)] = 1;
                    }
                }
            }
            refresh2 = 1;
        }

        if (PatternList.bFlashPageFlag) //索引换页更新
        {

            U32 numA = 0; //当前页的花样序号基数
            // U32 numB = 0; //当前页的花样序号偏移数
            U32 addA = 0; //当前页的花样序号基地址
            U32 addB = 0; //当前页的花样序号偏移地址
            U32 patternLen = 0;
            U8 patternType = 0;
            U16 disColor = 0;
            PatternList.bFlashPageFlag = 0;
            refresh2 = 1;
            dis_updata3 = 1;

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                if (g_SelectNo[i].Fileno != 0xffff) //此空间可以存信息,选择按键
                {
                    if (g_SelectNo[i].Select_No >= (PatternList.pageNow - 1) * PatternList.disNumDiv + MINFILENO &&
                        g_SelectNo[i].Select_No < PatternList.pageNow * PatternList.disNumDiv + MINFILENO)
                    {
                        PatternList.selectList[g_SelectNo[i].Select_No - ((PatternList.pageNow - 1) * PatternList.disNumDiv + MINFILENO)] = 1;
                    }
                }
            }

            //显示页码
            DrawFillRect(rect[RECT_FTCOPY_PAGE].xpis, rect[RECT_FTCOPY_PAGE].ypis,
                         rect[RECT_FTCOPY_PAGE].xsize, rect[RECT_FTCOPY_PAGE].ysize, rect[RECT_FTCOPY_PAGE].colorB[0]);
            DrawFU32(rect[RECT_FTCOPY_PAGE].xpis + SAFE_DIST2, rect[RECT_FTCOPY_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNow, 3, rect[RECT_FTCOPY_PAGE].colorT[0]);
            DrawF16Str(rect[RECT_FTCOPY_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_FTCOPY_PAGE].ypis + SAFE_DIST4,
                       "/", rect[RECT_FTCOPY_PAGE].colorT[0]);
            DrawFU32(rect[RECT_FTCOPY_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_FTCOPY_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNum, 3, rect[RECT_FTCOPY_PAGE].colorT[0]);

            if (PatternList.patternNum == 0)
                PatternList.noInPageNow = 0xff;
            else if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;

            numA = (PatternList.pageNow - 1) * PatternList.disNumDiv;
            if (numA > PatternList.patternNum)
                numA = PatternList.patternNum;
            // numB = PatternList.patternNum % PatternList.disNumDiv;
            // if ((PatternList.noInPageNow >= numB) && (PatternList.pageNow == PatternList.pageNum) && (PatternList.patternNum))
            // 	PatternList.noInPageNow = 0;

            if (numA) //计算当前页的基地址
            {
                for (j = 0, addA = 0; addA < MAXIREG2; addA += FileIndexSize)
                {
                    if ((((IREG2[addA + 1] << 16) | IREG2[addA]) > 0) &&
                        (((IREG2[addA + 1] << 16) | IREG2[addA]) <= 2 * MAXSREG))
                    {
                        j++;
                        if (j > numA)
                            break;
                    }
                }
            }
            addB = addA;
            if (numA < PatternList.patternNum) //计算当前页的各个文件的偏移地址
            {
                for (j = 0; addB < MAXIREG2; addB += FileIndexSize)
                {
                    if ((((IREG2[addB + 1] << 16) | IREG2[addB]) > 0) &&
                        (((IREG2[addB + 1] << 16) | IREG2[addB]) <= 2 * MAXSREG))
                    {
                        PatternList.dataAdd[j] = addB;
                        j++;
                        if (((numA + j) >= PatternList.patternNum) || (j >= PatternList.disNumDiv))
                            break;
                    }
                }
                for (; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }
            else
            {
                for (j = 0; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }
            //绘制按钮信息
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                DrawFillRect(rect[i + RECT_FTCOPY_IMG1].xpis, rect[i + RECT_FTCOPY_IMG1].ypis,
                             rect[i + RECT_FTCOPY_IMG1].xsize, rect[i + RECT_FTCOPY_IMG1].ysize,
                             rect[i + RECT_FTCOPY_IMG1].colorB[0]);

                BmpFillRect(rect[i + RECT_FTCOPY_SEL1].xpis, rect[i + RECT_FTCOPY_SEL1].ypis,
                            rect[i + RECT_FTCOPY_SEL1].xsize + SAFE_DIST6, rect[i + RECT_FTCOPY_SEL1].ysize);

                patternLen = ((IREG2[(PatternList.dataAdd[i]) + 1] << 16) | IREG2[(PatternList.dataAdd[i])]);
                patternType = ((IREG2[(PatternList.dataAdd[i]) + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
                disColor = ((PatternList.dataAdd[i] < MAXIREG2) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
                               ? ((patternType != 0) ? MitFontColor : BorFontColor)
                               : keybmpind[i + RECT_FTCOPY_SEL1].textColor[0];
                DrawFFileno_16x8(rect[i + RECT_FTCOPY_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3, rect[i + RECT_FTCOPY_SEL1].ypis + SAFE_DIST1,
                                 (PatternList.dataAdd[i] >= MAXIREG2) ? 0 : PatternList.dataAdd[i] / FileIndexSize + MINFILENO, disColor);
                DrawF16Str(rect[i + RECT_FTCOPY_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 3,
                           rect[i + RECT_FTCOPY_SEL1].ypis + SAFE_DIST1,
                           "@", rect[i + RECT_FTCOPY_SEL1].colorT[0]);

                if (PatternList.dataAdd[i] >= MAXIREG2)
                {
                    DrawFFilename(rect[i + RECT_FTCOPY_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
                                  rect[i + RECT_FTCOPY_SEL1].ypis + SAFE_DIST3,
                                  "NO__DATA", strlen("NO__DATA"), NO_DATAColor, 6, 2, NULL);
                    PatternList.selectEnable[i] = 0;
                }
                else
                {
                    DrawFFilename(rect[i + RECT_FTCOPY_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
                                  rect[i + RECT_FTCOPY_SEL1].ypis + SAFE_DIST3,
                                  (char *)(&IREG2[PatternList.dataAdd[i] + FileIndexNamepox]),
                                  64, disColor, 6, 2, NULL);
                    PatternList.selectEnable[i] = 1;
                }
            }
            wdt();
        }
        // /[更新花样显示列表*end]**********************************************************

        // /[文件选中BMP处理*begin]**********************************************************
        if (refresh2) // 文件编号选中处理
        {
            refresh2 = 0;
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                BmpFillRect(disBmp[i + 1].xpis, disBmp[i + 1].ypis,
                            disBmp[i + 1].xsize, disBmp[i + 1].ysize);
                DrawFPBMPBTN1(disBmp[i + 1].xpis, disBmp[i + 1].ypis,
                              disBmp[i + 1].xsize, disBmp[i + 1].ysize,
                              PatternList.selectList[i] ? disBmp[i + 1].icon[1] : disBmp[i + 1].icon[0]);
            }
        }
        // /[文件选中BMP处理*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        if (key_on_flag1)
            ErrorMsg(MENU_FTCOPY);
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_FTCOPY);
            if (Timer_ms >= 30000) //30720
            {
                Timer_ms = 0;
            }
        }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************

        // /[读取更新花样列表*begin]**********************************************************
        // if (g_updataIREG)
        // {
        // 	g_updataIREG = 0;
        // 	ReadPatList(0, pno); //更新内存索引
        // }
        // /[读取更新花样列表*end]**********************************************************

        // /[页面切换*begin]**********************************************************
        if ((g_backSREG_f == 1) || (g_backHREG_f == 1))
        {
            BackupSHREG((g_backHREG_f << 1) | g_backSREG_f);
        }

        if (pno != MENU_FTCOPY)
        {
            plastno = MENU_FTCOPY;
            break;
        }

        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

//**************************************
#define KEY_FTFORMAT_SCRNSVR 1 //1 屏保
#define KEY_FTFORMAT_BACK 2    //2 返回
#define KEY_FTFORMAT_ENTER 3   //3 确认
#define KEY_FTFORMAT_MEM 4     //4 flash
#define KEY_FTFORMAT_USB 5     //5 USB
#define KEY_FTFORMAT_SD 6      //6 SD
#define KEY_FTFORMAT_MAX 7

#define RECT_FTFORMAT_MOD 1
#define RECT_FTFORMAT_MAX 2

void Menu_FtFormat(void)
{

    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
                                            /****按键相关参数end****/

    /****标志位相关参数begin****/
    U32 i;
    U8 f;
    U8 dis_updata = 1;
    U8 SDFlag = 0, SDFlag1 = 0, USBFlag = 0, USBFlag1 = 0;
    /****标志位相关参数end***/
    const U8 keynum = {KEY_FTFORMAT_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 确认
        {TYPE_X7, TYPE_Y19, KEY_X1, KEY_Y1}, //4 内存
        {TYPE_X6, TYPE_Y19, KEY_X1, KEY_Y1}, //5 USB
        {TYPE_X5, TYPE_Y19, KEY_X1, KEY_Y1}, //6 SD
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_FORMART, 0, 0, 0}, {0}},

        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_FORMART + 1, 0, 0, 0}, {0}},                                 //1 屏保
        {{KEY_Silver}, {0}, {72, 88}, {TLFNADD_FUN_FORMART + 2, 0, 0, 0}, {0}},                            //2 返回
        {{KEY_Silver}, {0}, {71, 87}, {TLFNADD_FUN_FORMART + 3, 0, 0, 0}, {0}},                            //3 确认
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {257, 259, 258}, {TLFNADD_FUN_FORMART + 4, 0, 0, 0}, {0}}, //4 内存
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {251, 253, 252}, {TLFNADD_FUN_FORMART + 5, 0, 0, 0}, {0}}, //5 USB
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {254, 256, 255}, {TLFNADD_FUN_FORMART + 6, 0, 0, 0}, {0}}, //6 SD
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_FORMART}},

        {TYPE_X5 - SAFE_DIST4, TYPE_Y19 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1 * 4, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //3 模组按键框

    };

    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应40.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************

    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //穿线
    //画出方框
    //画出按键显示
    for (i = 2; i <= KEY_FTFORMAT_ENTER; i++)
    {
        //无效按键
        //绘制颜色文字按键
        //常规按钮
        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
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

        if (keydata == 0xff) //按键松开后的键值预处理
        {
            //带有连续按键功能的按键
            if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > 0) //按键按下的键值预处理    与提示性信息处理
        {
            //特定情况下不生效按键
            if (((keydata == KEY_FTFORMAT_USB) && (USBFlag == 0)) || ((keydata == KEY_FTFORMAT_SD) && (SDFlag == 0)))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
                {
                    keyon = keydata;
                    if (keyon == KEY_FTFORMAT_SCRNSVR) //无按键特效
                    {
                    }
                    //非常规按键-//绘制颜色文字按键
                    else //常规按键
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
                    }
                    else if (beeponf < 20)
                    {
                        beeponf++;
                    }

                    //带有长按连续功能按键的功能
                    Times = 0;
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
            switch (keyoff)
            {
            case KEY_FTFORMAT_SCRNSVR:
                pno = MENU_SCRNSVR;
                break;
            case KEY_FTFORMAT_BACK:
                pno = MENU_FUNCTION;
                pnoindex = 1;
                break;
            case KEY_FTFORMAT_ENTER:
                if (g_disk_memory == 2)
                { //格式化sd
                    hleppara = 25;
                }
                else if (g_disk_memory == 1)
                { //格式化usb
                    hleppara = 82;
                }
                else
                { //格式化内存
                    hleppara = 24;
                }
                pno = MENU_RWHELP;
                poldno[2] = MENU_FTFORMAT;
                pnoindex = 3;
                break;
            case KEY_FTFORMAT_MEM:
                dis_updata = 1;
                g_disk_memory = 0;
                break;
            case KEY_FTFORMAT_USB:
                if (ExtMemory_Status(USBTF) != 0)
                {
                    pno = MENU_RWHELP;
                    hleppara = 95;
                    poldno[2] = MENU_FTFORMAT;
                    pnoindex = 3;
                    break;
                }

                dis_updata = 1;
                g_disk_memory = 1;
                break;
            case KEY_FTFORMAT_SD:

                break;
            }
            keyoff = 0;
        }
        // /[按键松开键值处理*end]**********************************************************

        // /[按键弹起显示信息处理*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FTFORMAT_SCRNSVR) //不需要显示的按键
            {
            }
            //非常规按键-//绘制颜色文字按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;

            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回穿线
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        { //查询外部存储设备
            if (g_disk_memory == 1)
            { //使用U盘
                if (ExtMemory_Status(USBTF) != 0)
                {
                    USBFlag = 0;
                    pno = MENU_RWHELP;
                    hleppara = 95;
                    poldno[1] = MENU_FTFORMAT;
                    pnoindex = 2;
                }
            }
            else if (g_disk_memory == 2)
            { // 使用打板器SD
            }
            else
            { //使用主板内存

                if ((ExtMemory_Status(USBTF) == 0))
                {
                    USBFlag = 1;
                }
                else
                {
                    USBFlag = 0;
                }
            }
            if (SDFlag != SDFlag1)
            {
                SDFlag1 = SDFlag;
                dis_updata = 1;
            }
            if (USBFlag != USBFlag1)
            {
                USBFlag1 = USBFlag;
                dis_updata = 1;
            }
            Timerms = 0;
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        if (key_on_flag1)
            ErrorMsg(MENU_FTFORMAT);
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[内存按钮切换*begin]**********************************************************
        if (dis_updata)
        {
            dis_updata = 0;
            DrawFillRect(rect[RECT_FTFORMAT_MOD].xpis, rect[RECT_FTFORMAT_MOD].ypis,
                         rect[RECT_FTFORMAT_MOD].xsize, rect[RECT_FTFORMAT_MOD].ysize, rect[RECT_FTFORMAT_MOD].colorB[0]);
            f = KEY_FTFORMAT_MEM + g_disk_memory;
            for (i = KEY_FTFORMAT_MEM; i <= KEY_FTFORMAT_SD; i++)
            {
                if (i == KEY_FTFORMAT_USB)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (f == i) ? keybmpind[i].border[2] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[1]),
                               (f == i) ? keybmpind[i].icon[2] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[1]));
                }
                else if (i == KEY_FTFORMAT_SD)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (f == i) ? keybmpind[i].border[2] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[1]),
                               (f == i) ? keybmpind[i].icon[2] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[1]));
                }
                else
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                }
            }
        }
        // /[内存按钮切换*end]**********************************************************

        // /[页面切换*begin]**********************************************************

        if (pno != MENU_FTFORMAT)
        {
            plastno = MENU_FTFORMAT;
            break;
        }
        wdt();
        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

//------------------------------------------
#define KEY_FTGROUP_SCRNSVR 1 //1 屏保
#define KEY_FTGROUP_BACK 2    //2 返回
#define KEY_FTGROUP_ENTER 3   //3 确认

#define KEY_FTGROUP_READMEM 4 //4 从内存读取
#define KEY_FTGROUP_DEL 5     //5 删除一个
#define KEY_FTGROUP_REDO 6    //6 删除全部
#define KEY_FTGROUP_READ 7    //7 读取组合花样
#define KEY_FTGROUP_WRITE 8   //8 写入组合花样

#define KEY_READ_VIEWPIC 9  //9 显示当前花样信息
#define KEY_FTGROUP_PREV 10 //10 上一页
#define KEY_FTGROUP_NEXT 11 //11 下一页

#define KEY_FTGROUP_SEL1 12
#define KEY_FTGROUP_SEL2 13
#define KEY_FTGROUP_SEL3 14
#define KEY_FTGROUP_SEL4 15
#define KEY_FTGROUP_SEL5 16
#define KEY_FTGROUP_SEL6 17
#define KEY_FTGROUP_SEL7 18
#define KEY_FTGROUP_SEL8 19
#define KEY_FTGROUP_SEL9 20
#define KEY_FTGROUP_SEL10 21
#define KEY_FTGROUP_MAX 22

#define BMP_FTGROUP_1 1   //1 箭头提示
#define BMP_FTGROUP_2 2   //2 箭头提示
#define BMP_FTGROUP_3 3   //3 箭头提示
#define BMP_FTGROUP_4 4   //4 箭头提示
#define BMP_FTGROUP_5 5   //5 箭头提示
#define BMP_FTGROUP_6 6   //6 箭头提示
#define BMP_FTGROUP_7 7   //7 箭头提示
#define BMP_FTGROUP_8 8   //8 箭头提示
#define BMP_FTGROUP_9 9   //9 箭头提示
#define BMP_FTGROUP_10 10 //10 箭头提示
#define BMP_READ_MAX 11

#define RECT_FTGROUP_PAGE 1     //1 换页提示
#define RECT_FTGROUP_NAME_TIT 2 //组合花样名称
#define RECT_FTGROUP_NAME_STR 3 //3 组合花样名称
#define RECT_FTGROUP_NAME1 4    //4 花样名称-1
#define RECT_FTGROUP_NUM1 5     //5 花样序号-1
#define RECT_FTGROUP_NAME2 6    //6 花样名称-2
#define RECT_FTGROUP_NUM2 7     //7 花样序号-2
#define RECT_FTGROUP_NAME3 8    //8 花样名称-3
#define RECT_FTGROUP_NUM3 9     //9 花样序号-3
#define RECT_FTGROUP_NAME4 10   //10 花样名称-4
#define RECT_FTGROUP_NUM4 11    //11 花样序号-4
#define RECT_FTGROUP_NAME5 12   //12 花样名称-5
#define RECT_FTGROUP_NUM5 13    //13 花样序号-5
#define RECT_FTGROUP_NAME6 14   //14 花样名称-6
#define RECT_FTGROUP_NUM6 15    //15 花样序号-6
#define RECT_FTGROUP_NAME7 16   //16 花样名称-7
#define RECT_FTGROUP_NUM7 17    //17 花样序号-7
#define RECT_FTGROUP_NAME8 18   //18 花样名称-8
#define RECT_FTGROUP_NUM8 19    //19 花样序号-8
#define RECT_FTGROUP_NAME9 20   //20 花样名称-9
#define RECT_FTGROUP_NUM9 21    //21 花样序号-9
#define RECT_FTGROUP_NAME10 22  //22 花样名称-10
#define RECT_FTGROUP_NUM10 23   //23 花样序号-10
#define RECT_FTGROUP_MAX 24

U16 g_patfno;
//------------------------------------------
// Function   : void Menu_FtGroup(void)
// Description: 功能画面_图形组合
//------------------------------------------
void Menu_FtGroup(void)
{
    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
                                            /****按键相关参数end****/
    U32 i, j, f;
    U8 dis_updata1 = 1;                                                            //按钮状态更新
    tydPatternList pageList = {1, 0, 0, 10, 0, 20, 2, 1, 1, 0, 0, 0, 0, {0}, {0}}; //花样列表参数

    const U8 keynum = {KEY_FTGROUP_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 确认
        {TYPE_X8, TYPE_Y4, KEY_X1, KEY_Y1},  //4 从内存读取
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //5 删除一个
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},  //6 删除全部
        {TYPE_X6, TYPE_Y4, KEY_X1, KEY_Y1},  //7 读取组合花样
        {TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},  //8 写入组合花样

        {TYPE_X11, TYPE_Y3, KEY_X1, KEY_Y1},                                         //9 显示当前花样信息
        {TYPE_X11, TYPE_Y14, KEY_X1, KEY_Y1},                                        //10 上一页
        {TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5 * 2 + TYPE_SIZE4, KEY_X1, KEY_Y1}, //11 下一页

        {TYPE_X40, TYPE_Y40, KEY_X4, KEY_Y4},
        {TYPE_X41, TYPE_Y40, KEY_X4, KEY_Y4},
        {TYPE_X42, TYPE_Y40, KEY_X4, KEY_Y4},
        {TYPE_X43, TYPE_Y40, KEY_X4, KEY_Y4},
        {TYPE_X44, TYPE_Y40, KEY_X4, KEY_Y4},
        {TYPE_X40, TYPE_Y41, KEY_X4, KEY_Y4},
        {TYPE_X41, TYPE_Y41, KEY_X4, KEY_Y4},
        {TYPE_X42, TYPE_Y41, KEY_X4, KEY_Y4},
        {TYPE_X43, TYPE_Y41, KEY_X4, KEY_Y4},
        {TYPE_X44, TYPE_Y41, KEY_X4, KEY_Y4},
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_GROUP + 0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_GROUP + 1, 0, 0, 0}, {0}},                               //1 屏保
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {TLFNADD_FUN_GROUP + 2, 0, 0, 0}, {0}},      //2 返回
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {TLFNADD_FUN_GROUP + 3, 0, 0, 0}, {0}},      //3 确认
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {272, 273, 0}, {TLFNADD_FUN_GROUP + 4, 0, 0, 0}, {0}}, //4 从内存读取
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {111, 112, 0}, {TLFNADD_FUN_GROUP + 5, 0, 0, 0}, {0}}, //5 删除一个
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {244, 0, 0}, {TLFNADD_FUN_GROUP + 6, 0, 0, 0}, {0}},   //6 删除全部
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {247, 0, 0}, {TLFNADD_FUN_GROUP + 7, 0, 0, 0}, {0}},   //7 读取组合花样
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {280, 281, 0}, {TLFNADD_FUN_GROUP + 8, 0, 0, 0}, {0}}, //8 写入组合花样

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {11, 12, 13}, {TLFNADD_FUN_GROUP + 9, 0, 0, 0}, {0}}, //9 显示当前花样信息
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {219, 220}, {TLFNADD_FUN_GROUP + 10, 0, 0, 0}, {0}},  //10 上一页
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {221, 222}, {TLFNADD_FUN_GROUP + 11, 0, 0, 0}, {0}},  //11 下一页

        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 13, 0, 0, 0}, {0}}, //14 选框1
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 14, 0, 0, 0}, {0}}, //15 选框2
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 15, 0, 0, 0}, {0}}, //16 选框3
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 16, 0, 0, 0}, {0}}, //17 选框4
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 17, 0, 0, 0}, {0}}, //18 选框5
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 18, 0, 0, 0}, {0}}, //19 选框6
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 19, 0, 0, 0}, {0}}, //20 选框7
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 20, 0, 0, 0}, {0}}, //21 选框8
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 21, 0, 0, 0}, {0}}, //22 选框9
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_GROUP + 22, 0, 0, 0}, {0}}, //23 选框10
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_GROUP}},

        {TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //1 换页提示

        {215, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_GROUP + 33}},                                     //组合花样名称
        {215 + SAFE_DIST5 + TYPE_SIZE9, TYPE_Y3, TYPE_SIZE23, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //3 组合花样名称

        {TYPE_X40, TYPE_Y40 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X40, TYPE_Y40, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X41, TYPE_Y40 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X41, TYPE_Y40, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X42, TYPE_Y40 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X42, TYPE_Y40, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X43, TYPE_Y40 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X43, TYPE_Y40, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X44, TYPE_Y40 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X44, TYPE_Y40, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X40, TYPE_Y41 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X40, TYPE_Y41, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X41, TYPE_Y41 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X41, TYPE_Y41, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X42, TYPE_Y41 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X42, TYPE_Y41, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X43, TYPE_Y41 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X43, TYPE_Y41, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_X44, TYPE_Y41 + SAFE_DIST6 + KEY_Y4, KEY_X4, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X44, TYPE_Y41, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
    };

    const BMPPAD disBmp[] = {
        //icon
        {0, 0, 0, 0, {0}},

        {TYPE_X40 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y42, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //1
        {TYPE_X41 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y42, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //2
        {TYPE_X42 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y42, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //3
        {TYPE_X43 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y42, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //4
        {TYPE_X44 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y42, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //5
        {TYPE_X40 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y43, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //6
        {TYPE_X41 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y43, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //7
        {TYPE_X42 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y43, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //8
        {TYPE_X43 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y43, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //9
        {TYPE_X44 - SAFE_DIST5 - TYPE_SIZE5, TYPE_Y43, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //10
    };

    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************
    if (g_Groupsel1 > GroupMax)
        g_Groupsel1 = 0;

    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

    //画出方框
    DrawEditTit(rect[RECT_FTGROUP_NAME_TIT], rect[RECT_FTGROUP_NAME_TIT].index[0], SheetPage, LEFT, STR24X12, SAFE_DIST4);
    DrawFillRect(rect[RECT_FTGROUP_NAME_STR].xpis, rect[RECT_FTGROUP_NAME_STR].ypis,
                 rect[RECT_FTGROUP_NAME_STR].xsize, rect[RECT_FTGROUP_NAME_STR].ysize, rect[RECT_FTGROUP_NAME_STR].colorB[0]);
    Draw24X24FStr(rect[RECT_FTGROUP_NAME_STR].xpis + SAFE_DIST4, rect[RECT_FTGROUP_NAME_STR].ypis + (rect[RECT_FTGROUP_NAME_STR].ysize - 24) / 2,
                  (char *)g_GroupName, rect[RECT_FTGROUP_NAME_STR].colorT[0]);
    //画出按键显示
    for (i = 2; i <= KEY_FTGROUP_NEXT; i++)
    {
        if (i == KEY_READ_VIEWPIC)
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

    Timer_ms = 30720;
    Timerms = 0;
    Timer_ms1 = 0;

    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值

        // /[按键键值返回预处理*begin]**********************************************************
        if (keydata == KEY_READ_VIEWPIC)
        {
            keydata = 0;
        }
        //特定情况下不生效按键预处理

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
            if (g_checkGroup && (keydata >= KEY_FTGROUP_ENTER) && (keydata <= KEY_READ_VIEWPIC))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
                {
                    keyon = keydata;
                    if (keyon == KEY_FTGROUP_SCRNSVR) //无按键特效
                    {
                    }
                    //非常规按键
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
                        case KEY_FTGROUP_SEL1:
                        case KEY_FTGROUP_SEL2:
                        case KEY_FTGROUP_SEL3:
                        case KEY_FTGROUP_SEL4:
                        case KEY_FTGROUP_SEL5:
                        case KEY_FTGROUP_SEL6:
                        case KEY_FTGROUP_SEL7:
                        case KEY_FTGROUP_SEL8:
                        case KEY_FTGROUP_SEL9:
                        case KEY_FTGROUP_SEL10:
                            DrawFMenuStr(keybmpind[keyon].titleStr[0] + (pageList.pageNow - 1) * pageList.disNumDiv);
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
                    //带有长按连续功能按键的功能
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
            if ((keyoff >= KEY_FTGROUP_SEL1) && (keyoff <= KEY_FTGROUP_SEL10))
            {
                g_Groupsel1 = keyoff - KEY_FTGROUP_SEL1 + (pageList.pageNow - 1) * pageList.disNumDiv;
                pageList.bFlashSelectFlag = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTGROUP_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_FTGROUP_BACK:
                    if (g_checkGroup == 0)
                    {
                        Write_Cmd(C_READMEMORYPAT, g_Fileno, 0); //还原进来是的花样
                        RestoreSHREG(0x03);
                        g_NoReadPatf = 1;                      //标记 进入P1界面不需要重新读取花样数据
                        if (Write_Cmd(C_READGROUP, 0, 0) == 0) //发送读取组合图形指令
                        {
                            pno = MENU_RWHELP;
                            hleppara = 16;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                        }
                        Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                        Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE])); // 读组合类型
                        if (((YREG[SYS_GROUP_TYPE] > 0) && (YREG[SYS_GROUP_TYPE] < 41)) || (YREG[SYS_GROUP_TYPE] == 100))
                        {
                            g_GroupType = (U8)GREG[GroupPTypeAddr];
                        }
                        else
                        {
                            g_GroupType = 0;
                        }
                    }
                    pnoindex--;
                    pno = poldno[pnoindex];
                    break;

                case KEY_FTGROUP_ENTER:
                    for (i = 0; i < GroupPatMax; i++) //清0
                    {
                        GREG[i] = 0;
                    }

                    for (i = 0; i < GroupMax; i++)
                    {
                        if (g_Combinepat[i][0] == 0xffff)
                            break;
                        GREG[i] = g_Combinepat[i][0]; //图形花样编号
                    }

                    if (i == 0)
                    {
                        if (g_GroupMenth == 1) //保存模式-提示无花样
                        {
                            pno = MENU_RWHELP;
                            hleppara = 101;
                            poldno[pnoindex] = MENU_FTGROUP;
                            pnoindex++;
                            break;
                        }
                        else
                        {
                            g_GroupMenth = 3; //无文件，转为清除模式
                        }
                    }

                    if (g_GroupMenth == 1) //保存组合图形到内存
                    {
                        GREG[GroupPNameAddr] = (g_GroupName[1] << 8) | g_GroupName[0];
                        GREG[GroupPNameAddr + 1] = (g_GroupName[3] << 8) | g_GroupName[2];
                        GREG[GroupPNameAddr + 2] = (g_GroupName[5] << 8) | g_GroupName[4];
                        GREG[GroupPNameAddr + 3] = (g_GroupName[7] << 8) | g_GroupName[6];
                        GREG[GroupPNumAddr] = i;
                        GREG[GroupPTypeAddr] = g_GroupType;
                        if (g_GroupSave == 1) //保存到内存
                        {

                            if (Write_D(D_Address[GROUP], GroupPatMax, (U16 *)(&GREG[0])) == 0)
                            { //发送参数
                                pno = MENU_RWHELP;
                                hleppara = 16;
                                break;
                            }

                            if (Write_Cmd(C_SAVEGROUP, g_GroupType, g_GroupDoingNO) == 0) //发送保存组合文件指令
                            {
                                pno = MENU_RWHELP;
                                hleppara = 16;
                                break;
                            }
                            if (Read_Status() == 0)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] != 100)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                                break;
                            }

                            if (g_GroupCover == 1)
                            {
                                g_GroupCover = 0;
                            }
                        }
                        else if ((g_GroupSave == 2) || (g_GroupSave == 3)) //保存到磁盘
                        {
                            pno = MENU_RWHELP;
                            hleppara = 110;
                            poldno[pnoindex] = MENU_FTGROUP;
                            pnoindex++;
                            break;
                        }
                    }
                    else if (g_GroupMenth == 2) //读取混合图形
                    {
                        if (Write_Cmd(C_OPENGROUP, g_GroupType, g_GroupDoingNO) == 0) //发送打开组合文件指令
                        {
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {

                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                    }
                    else if (g_GroupMenth == 3) //清除当前组合图形
                    {
                        if (Write_Cmd(C_OPENGROUP, 3, 0) == 0) //发送关闭组合文件指令
                        {
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {

                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                    }
                    else //保存到默认位置
                    {
                        if ((g_GroupType != 1) && (g_GroupType != 2))
                            g_GroupType = 1;

                        GREG[GroupPNameAddr] = (g_GroupName[1] << 8) | g_GroupName[0];
                        GREG[GroupPNameAddr + 1] = (g_GroupName[3] << 8) | g_GroupName[2];
                        GREG[GroupPNameAddr + 2] = (g_GroupName[5] << 8) | g_GroupName[4];
                        GREG[GroupPNameAddr + 3] = (g_GroupName[7] << 8) | g_GroupName[6];
                        GREG[GroupPNumAddr] = i;
                        GREG[GroupPTypeAddr] = g_GroupType;

                        if (Write_D(D_Address[GROUP], GroupPatMax, (U16 *)(&GREG[0])) == 0) //发送参数
                        {
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }

                        if (Write_Cmd(C_SAVEGROUP, 3, g_GroupType) == 0) //发送保存组合文件指令
                        {
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                    }
                    if (Write_Cmd(C_READGROUP, 0, 0) == 0) //发送读取组合图形指令
                    {
                        pno = MENU_RWHELP;
                        hleppara = 16;
                        break;
                    }
                    if (Read_Status() == 0)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 15;
                        break;
                    }
                    else if (DREG[D_CMDSTATUS] != 100)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 15;
                        break;
                    }
                    Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                    Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE])); // 读取组合花样系统参数
                    if (((YREG[SYS_GROUP_TYPE] > 0) && (YREG[SYS_GROUP_TYPE] < 41)) || (YREG[SYS_GROUP_TYPE] == 100))
                    {
                        g_GroupType = (U8)GREG[GroupPTypeAddr];
                    }
                    else
                    {
                        g_GroupType = 0;
                    }

                    i = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                    if (i == 0)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 15;
                        break;
                    }
                    g_selectdisno = F_IND_NO;
                    g_oldselectdisno = F_IND_NO;

                    Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // 读当前花样文件
                    g_NoReadPatf = 1;                                          //标记 进入P1界面不需要重新读取花样数据
                    pnoindex = 0;
                    pno = MENU_P1;
                    break;

                case KEY_FTGROUP_READMEM:
                    if ((g_Combinepat[g_Groupsel1 - 1][0] == 0xffff) && (g_Groupsel1 > 0))
                    { //选定前没有数据，先填前面
                        while ((g_Combinepat[g_Groupsel1 - 1][0] == 0xffff) && (g_Groupsel1 > 0))
                        {
                            g_Groupsel1--;
                        }
                    }

                    pno = MENU_FTGROUPREAD1;
                    poldno[pnoindex] = MENU_FTGROUP;
                    pnoindex++;
                    g_patfno = F_IND_NO;
                    g_disppicflag = 1;
                    break;

                case KEY_FTGROUP_DEL:                           //清除选中的花样资料
                    if (g_Combinepat[g_Groupsel1][0] == 0xffff) //当前选中及其后面没有数据
                    {
                    }
                    else if ((g_Combinepat[g_Groupsel1 + 1][0] != 0xffff) && ((g_Groupsel1 + 1) < 20))
                    { //当前选中及其后面都有数据，删除当前数据，后面数据前移
                        for (i = g_Groupsel1 + 1; i < 20; i++)
                        {
                            if (g_Combinepat[i][0] == 0xffff)
                                break;
                            for (j = 0; j < 6; j++)
                            {
                                g_Combinepat[i - 1][j] = g_Combinepat[i][j];
                            }
                        }
                        for (j = 0; j < 6; j++)
                        {
                            if (j == 0)
                            {
                                g_Combinepat[i - 1][j] = 0xffff;
                            }
                            else
                            {
                                g_Combinepat[i - 1][j] = 0;
                            }
                        }
                    }
                    else
                    { //当前选中有数据其后没有数据，删除当前数据
                        for (j = 0; j < 6; j++)
                        {
                            if (j == 0)
                            {
                                g_Combinepat[g_Groupsel1][j] = 0xffff;
                            }
                            else
                            {
                                g_Combinepat[g_Groupsel1][j] = 0;
                            }
                        }
                    }
                    dis_updata1 = 1;
                    break;

                case KEY_FTGROUP_REDO:
                    for (i = 0; i < GroupMax; i++)
                    {
                        if (g_Combinepat[i][0] == 0xffff)
                            break;
                    }
                    if ((i > 0) || (g_GroupType == 1) || (g_GroupType == 2))
                    { //在混合状态
                        pno = MENU_RWHELP;
                        hleppara = 103;
                        poldno[pnoindex] = MENU_FTGROUP;
                        pnoindex++;
                        break;
                    }
                    break;

                case KEY_FTGROUP_READ:
                    for (i = 0; i < GroupMax; i++)
                    {
                        if (g_Combinepat[i][0] == 0xffff)
                            break;
                    }
                    if (i > 0)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 104;
                        poldno[pnoindex] = MENU_FTGROUP;
                        pnoindex++;
                        break;
                    }
                    if ((g_GroupType != 1) && (g_GroupType != 2)) //当前写入类型不是组合和接合，设置为组合
                    {
                        g_GroupType = 1;
                    }

                    pno = MENU_FTGROUPREAD2;
                    poldno[pnoindex] = MENU_FTGROUP;
                    pnoindex++;
                    break;

                case KEY_FTGROUP_WRITE:

                    for (i = 0; i < GroupMax; i++)
                    {
                        if (g_Combinepat[i][0] == 0xffff)
                            break;
                    }
                    if (i == 0)
                    {
                        //保存模式-提示无花样
                        pno = MENU_RWHELP;
                        hleppara = 101;
                        poldno[pnoindex] = MENU_FTGROUP;
                        pnoindex++;
                        break;
                    }

                    if ((g_GroupType != 1) && (g_GroupType != 2)) //当前写入类型不是组合和接合，设置为组合
                    {
                        g_GroupType = 1;
                    }
                    pno = MENU_FTGROUPWRITE;
                    poldno[pnoindex] = MENU_FTGROUP;
                    pnoindex++;
                    break;
                case KEY_FTGROUP_PREV:
                    if (pageList.pageNow > 1)
                    {
                        pageList.pageNow--;
                        pageList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_FTGROUP_NEXT:
                    if (pageList.pageNow < pageList.pageNum)
                    {
                        pageList.pageNow++;
                        pageList.bFlashPageFlag = 1;
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

            if (keyon == KEY_FTGROUP_SCRNSVR) //无按键特效
            {
            }
            //非常规按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回读取模式
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
        }
        if (g_GroupMenth == 3) //修改
        {
            g_GroupMenth = 0;
            if (Write_Cmd(C_OPENGROUP, 3, 0) == 0) //发送关闭组合文件指令
            {
                pno = MENU_RWHELP;
                hleppara = 16;
            }
            if (Read_Status() == 0)
            {
                pno = MENU_RWHELP;
                hleppara = 15;
            }
            else if (DREG[D_CMDSTATUS] != 100)
            {

                pno = MENU_RWHELP;
                hleppara = 15;
            }
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[更新花样显示列表*begin]**********************************************************
        if (pageList.bflashAllFlag) //索引信息更新
        {
            pageList.bflashAllFlag = 0;
            pageList.bFlashPageFlag = 1;

            pageList.patternNum = 20;
            pageList.selectNoNow = 1;
            for (i = 0; i < pageList.patternNum; i++)
            {
                if (g_Combinepat[i][0] == 0xffff)
                {
                    pageList.selectNoNow = i + 1;
                    break;
                }
            }
            pageList.selectNoLast = pageList.selectNoNow;

            //确定 当前选中项、当前选中页、页码总数
            pageList.noInPageNow = (pageList.selectNoNow - 1) % pageList.disNumDiv;
            pageList.noInPageLast = pageList.noInPageNow;
            pageList.pageNow = (pageList.selectNoNow - 1) / pageList.disNumDiv + 1;
            pageList.pageLast = pageList.pageNow;
            pageList.pageNum = (pageList.patternNum - 1) / pageList.disNumDiv + 1;

            if (pageList.pageNum == 0)
                pageList.pageNum = 1;

            if (pageList.pageNow == 0)
                pageList.pageNow = 1;
            else if (pageList.pageNow > pageList.pageNum)
                pageList.pageNow = pageList.pageNum;

            if (pageList.noInPageNow >= pageList.disNumDiv)
                pageList.noInPageNow = 0;
        }

        if (pageList.bFlashSelectFlag) //索引选中项更新
        {
            pageList.bFlashSelectFlag = 0;
            pageList.selectNoNow = g_Groupsel1 + 1;
            pageList.selectNoLast = pageList.selectNoNow;
            pageList.pageLast = pageList.pageNow;

            pageList.noInPageNow = (pageList.selectNoNow - 1) % pageList.disNumDiv;
            pageList.noInPageLast = pageList.noInPageNow;

            for (i = 0; i < pageList.disNumDiv; i++)
            {
                pageList.selectList[i] = 0;
            }
            if (pageList.pageNow == pageList.pageLast)
            {
                pageList.selectList[pageList.noInPageNow] = 1;
            }
            dis_updata1 = 1;
        }

        if (pageList.bFlashPageFlag) //索引换页更新
        {
            pageList.bFlashPageFlag = 0;
            dis_updata1 = 1;

            //显示页码
            DrawFillRect(rect[RECT_FTGROUP_PAGE].xpis, rect[RECT_FTGROUP_PAGE].ypis,
                         rect[RECT_FTGROUP_PAGE].xsize, rect[RECT_FTGROUP_PAGE].ysize, rect[RECT_FTGROUP_PAGE].colorB[0]);
            DrawFU32(rect[RECT_FTGROUP_PAGE].xpis + SAFE_DIST2, rect[RECT_FTGROUP_PAGE].ypis + SAFE_DIST4,
                     pageList.pageNow, 3, rect[RECT_FTGROUP_PAGE].colorT[0]);
            DrawF16Str(rect[RECT_FTGROUP_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_FTGROUP_PAGE].ypis + SAFE_DIST4,
                       "/", rect[RECT_FTGROUP_PAGE].colorT[0]);
            DrawFU32(rect[RECT_FTGROUP_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_FTGROUP_PAGE].ypis + SAFE_DIST4,
                     pageList.pageNum, 3, rect[RECT_FTGROUP_PAGE].colorT[0]);
            for (i = 0; i < pageList.disNumDiv; i++)
            {
                f = i + 1;
                BmpFillRect(disBmp[f].xpis, disBmp[f].ypis, disBmp[f].xsize, disBmp[f].ysize);
                if (i == 0)
                {
                    if (pageList.pageNow != 1)
                    {
                        DrawFPBMPBTN(disBmp[f].xpis, disBmp[f].ypis, disBmp[f].xsize, disBmp[f].ysize, disBmp[f].icon[0], Color_White);
                    }
                }
                else if (i % 5 == 0)
                {
                    DrawFPBMPBTN(disBmp[f].xpis, disBmp[f].ypis, disBmp[f].xsize, disBmp[f].ysize, disBmp[f].icon[1], Color_White);
                }
                else
                {
                    DrawFPBMPBTN(disBmp[f].xpis, disBmp[f].ypis, disBmp[f].xsize, disBmp[f].ysize, disBmp[f].icon[0], Color_White);
                }
            }

            if (pageList.patternNum == 0)
                pageList.noInPageNow = 0xff;
            else if (pageList.noInPageNow >= pageList.disNumDiv)
                pageList.noInPageNow = 0;

            for (i = 0; i < pageList.disNumDiv; i++)
            {
                pageList.selectList[i] = 0;
            }
            if (pageList.pageNow == pageList.pageLast)
            {
                pageList.selectList[pageList.noInPageNow] = 1;
            }
            wdt();
        }
        // /[更新花样显示列表*end]**********************************************************

        if (dis_updata1)
        {
            dis_updata1 = 0;
            for (i = 0; i < pageList.disNumDiv; i++)
            {
                f = i + KEY_FTGROUP_SEL1;
                DrawButtonSingleColor(keyd[f],
                                      (pageList.selectList[i] == 1) ? keybmpind[f].border[1] : keybmpind[f].border[0],
                                      (pageList.selectList[i] == 1) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                f = i * 2 + RECT_FTGROUP_NAME1;
                DrawEdit(rect[f],
                         (pageList.selectList[i] == 1) ? rect[f].colorT[1] : rect[f].colorB[0],
                         (pageList.selectList[i] == 1) ? rect[f].colorB[1] : rect[f].colorB[0]);
                //显示编号名称
                j = i + (pageList.pageNow - 1) * pageList.disNumDiv;
                if (g_Combinepat[j][0] == 0xffff)
                {
                    f = i + KEY_FTGROUP_SEL1;
                    Draw24X24FStr(keyd[f].xpis + (keyd[f].xsize - 13 * 3) / 2, keyd[f].ypis + (keyd[f].ysize - 24) / 2,
                                  "***", (pageList.selectList[i] == 1) ? keybmpind[f].textColor[1] : keybmpind[f].textColor[0]);
                    f = i * 2 + RECT_FTGROUP_NAME1;
                    DrawF16Str(rect[f].xpis + (rect[f].xsize - 9 * 8) / 2, rect[f].ypis + (rect[f].ysize - 16) / 2,
                               "********", (pageList.selectList[i] == 1) ? rect[f].colorT[1] : rect[f].colorT[0]);
                }
                else
                {
                    f = i + KEY_FTGROUP_SEL1;
                    DrawMFU32(keyd[f].xpis + (keyd[f].xsize - 13 * 3) / 2, keyd[f].ypis + (keyd[f].ysize - 24) / 2,
                              g_Combinepat[j][0], 3,
                              (pageList.selectList[i] == 1) ? keybmpind[f].textColor[1] : keybmpind[f].textColor[0]);
                    f = i * 2 + RECT_FTGROUP_NAME1;
                    DrawF16Str(rect[f].xpis + (rect[f].xsize - 9 * (strlen((char *)&g_Combinepat[j][1]))) / 2, rect[f].ypis + (rect[f].ysize - 16) / 2,
                               (char *)&g_Combinepat[j][1],
                               (pageList.selectList[i] == 1) ? rect[f].colorT[1] : rect[f].colorT[0]);
                }
            }
        }
        //备份花样数据
        if ((g_backSREG_f == 1) || (g_backHREG_f == 1))
        {
            BackupSHREG((g_backHREG_f << 1) | g_backSREG_f);
        }
        ErrorMsg(MENU_FTGROUP);
        if (pno != MENU_FTGROUP)
        {
            plastno = MENU_FTGROUP;
            break;
        }
        wdt();
    }
}

//------------------------------------------------------------------------------
#define KEY_FTREADMEM_SCRNSVR 1 //1 屏保
#define KEY_FTREADMEM_BACK 2    //2 返回
#define KEY_FTREADMEM_OK 3      //3 确认
#define KEY_FTREADMEM_FLASH 4   //4 内存
#define KEY_FTREADMEM_USB 5     //5 USB
#define KEY_FTREADMEM_SD 6      //6 SD
#define KEY_FTREADMEM_VIEWPIC 7 //7 显示当前花样信息
#define KEY_FTREADMEM_PREV 8    //8 上一页
#define KEY_FTREADMEM_NEXT 9    //9 下一页
#define KEY_FTREADMEM_SEL1 10   //10 选框1
#define KEY_FTREADMEM_SEL2 11   //11 选框2
#define KEY_FTREADMEM_SEL3 12   //12 选框3
#define KEY_FTREADMEM_SEL4 13   //13 选框4
#define KEY_FTREADMEM_SEL5 14   //14 选框5
#define KEY_FTREADMEM_SEL6 15   //15 选框6
#define KEY_FTREADMEM_SEL7 16   //16 选框7
#define KEY_FTREADMEM_SEL8 17   //17 选框8
#define KEY_FTREADMEM_SEL9 18   //18 选框9
#define KEY_FTREADMEM_SEL10 19  //19 选框10
#define KEY_FTREADMEM_SEL11 20  //20 选框11
#define KEY_FTREADMEM_SEL12 21  //21 选框12
#define KEY_FTREADMEM_DREAD 22  //22 跳转读取

#define KEY_FTREADMEM_MAX 23

#define RECT_FTREADMEM_SEL1 1   //1 选框1
#define RECT_FTREADMEM_SEL2 2   //2 选框2
#define RECT_FTREADMEM_SEL3 3   //3 选框3
#define RECT_FTREADMEM_SEL4 4   //4 选框4
#define RECT_FTREADMEM_SEL5 5   //5 选框5
#define RECT_FTREADMEM_SEL6 6   //6 选框6
#define RECT_FTREADMEM_SEL7 7   //7 选框7
#define RECT_FTREADMEM_SEL8 8   //8 选框8
#define RECT_FTREADMEM_SEL9 9   //9 选框9
#define RECT_FTREADMEM_SEL10 10 //10 选框10
#define RECT_FTREADMEM_SEL11 11 //11 选框11
#define RECT_FTREADMEM_SEL12 12 //12 选框12
#define RECT_FTREADMEM_PAGE 13  //13 换页提示
#define RECT_FTREADMEM_MOD 14   //14 模组按键框

#define RECT_FTREADMEM_IMG1 15  //15 选框1
#define RECT_FTREADMEM_IMG2 16  //16 选框2
#define RECT_FTREADMEM_IMG3 17  //17 选框3
#define RECT_FTREADMEM_IMG4 18  //18 选框4
#define RECT_FTREADMEM_IMG5 19  //19 选框5
#define RECT_FTREADMEM_IMG6 20  //20 选框6
#define RECT_FTREADMEM_IMG7 21  //21 选框7
#define RECT_FTREADMEM_IMG8 22  //22 选框8
#define RECT_FTREADMEM_IMG9 23  //23 选框9
#define RECT_FTREADMEM_IMG10 24 //24 选框10
#define RECT_FTREADMEM_IMG11 25 //25 选框11
#define RECT_FTREADMEM_IMG12 26 //26 选框12

#define RECT_FTREADMEM_MAX 27

#define BMP_FTREADMEM_SEL1 1   //1 选框1
#define BMP_FTREADMEM_SEL2 2   //2 选框2
#define BMP_FTREADMEM_SEL3 3   //3 选框3
#define BMP_FTREADMEM_SEL4 4   //4 选框4
#define BMP_FTREADMEM_SEL5 5   //5 选框5
#define BMP_FTREADMEM_SEL6 6   //6 选框6
#define BMP_FTREADMEM_SEL7 7   //7 选框7
#define BMP_FTREADMEM_SEL8 8   //8 选框8
#define BMP_FTREADMEM_SEL9 9   //9 选框9
#define BMP_FTREADMEM_SEL10 10 //10 选框10
#define BMP_FTREADMEM_SEL11 11 //11 选框11
#define BMP_FTREADMEM_SEL12 12 //12 选框12

#define BMP_FTREADMEM_MAX 13
//------------------------------------------
// Function   : void MENU_FTGROUPREAD1(void)
// Description: 读取花样画面
//------------------------------------------
void Menu_FtReadMem(void)
{
    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
    /****按键相关参数end****/

    /****花样序号，花样名称，花样列表相关参数begin****/
    //	U16 static tmpFno;																 //当前花样信息HREG[0]文件位置(即序号),花样序号缓存
    U16 tmpselectdisno;                                                              //选中的花样序号缓存
    U16 tmp_s_2hd = (U16)(S_2HD);                                                    //次原点位置缓存
    tydPatternList PatternList = {1, 0, 0, 12, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}}; //花样列表参数
    /****花样序号，花样名称，花样列表相关参数end****/

    /****花样显示相关参数begin****/
    S32 Scale = 0; //tscale：图像显示需要调整的缩放比，tscale1：最终的总缩放比，Scale：固定的缩放比
    const S16 orgListX[] = {CONVAS_ORG_PX2_1, CONVAS_ORG_PX2_2, CONVAS_ORG_PX2_3, CONVAS_ORG_PX2_4, CONVAS_ORG_PX2_5,
                            CONVAS_ORG_PX2_6, CONVAS_ORG_PX2_7, CONVAS_ORG_PX2_8, CONVAS_ORG_PX2_9, CONVAS_ORG_PX2_10,
                            CONVAS_ORG_PX2_11, CONVAS_ORG_PX2_12};
    const S16 orgListY[] = {CONVAS_ORG_PY2_1, CONVAS_ORG_PY2_2, CONVAS_ORG_PY2_3, CONVAS_ORG_PY2_4, CONVAS_ORG_PY2_5,
                            CONVAS_ORG_PY2_6, CONVAS_ORG_PY2_7, CONVAS_ORG_PY2_8, CONVAS_ORG_PY2_9, CONVAS_ORG_PY2_10,
                            CONVAS_ORG_PY2_11, CONVAS_ORG_PY2_12};
    /****花样显示相关参数end****/

    /****标志位相关参数begin****/
    U32 i, j;
    U8 f = 0;
    U8 Chinesenum;
    U8 endfile = 0;

    U8 SDFlag = 0 /*, SDFlag1 = 0*/, USBFlag = 0 /*, USBFlag1 = 0*/;

    U8 /*refresh0 = 1, refresh1 = 1,*/ refresh2 = 0, refresh4 = 0, refresh5 = 1; //refresh2更新文件选中编号， refresh4更新内存选择按钮，refresh5更新花样显示图案
    U8 /*disupdata1 = 1, disupdata2 = 0,*/ dis_updata3 = 0;                      //disupdata1:索引更新标志 disupdata2：当前文件所在索引位置更新标志  disupdata3：花样数据更新标志

    /****标志位相关参数end***/

    const U8 keynum = {KEY_FTREADMEM_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                                             //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                          //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                                         //3 确认
        {0, 0, 0, 0},                                                                //4 内存
        {0, 0, 0, 0},                                                                //5 USB
        {0, 0, 0, 0},                                                                //6 SD
        {TYPE_X11, TYPE_Y3, KEY_X1, KEY_Y1},                                         //7 显示当前花样信息
        {TYPE_X11, TYPE_Y14, KEY_X1, KEY_Y1},                                        //8 上一页
        {TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5 * 2 + TYPE_SIZE4, KEY_X1, KEY_Y1}, //9 下一页
        {CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //10 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //11 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //12 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //13 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //14 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //15 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //16 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //17 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},             //18 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},           //19 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},           //20 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},           //21 选框12
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},                                         //22 跳转读取
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_READMEM, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_READMEM + 1, 0, 0, 0}, {0}},                              //1 屏保
        {{KEY_Silver}, {0}, {72, 88}, {TLFNADD_FUN_READMEM + 2, 0, 0, 0}, {0}},                         //2 返回
        {{KEY_Silver}, {0}, {71, 87}, {TLFNADD_FUN_READMEM + 3, 0, 0, 0}, {0}},                         //3 确认
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {257, 258, 259}, {TLFNADD_FUN_READMEM, 0, 0, 0}, {0}},  //4 内存
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {251, 252, 253}, {TLFNADD_FUN_READMEM, 0, 0, 0}, {0}},  //5 USB
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {254, 255, 256}, {TLFNADD_FUN_READMEM, 0, 0, 0}, {0}},  //6 SD
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {11, 12, 13}, {TLFNADD_FUN_READMEM + 4, 0, 0, 0}, {0}}, //7 显示当前花样信息
        {{KEY_Silver}, {0}, {219, 220}, {TLFNADD_FUN_READMEM + 5, 0, 0, 0}, {0}},                       //8 上一页
        {{KEY_Silver}, {0}, {221, 222}, {TLFNADD_FUN_READMEM + 6, 0, 0, 0}, {0}},                       //9 下一页
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 7, 0, 0, 0}, {0}},                   //10 选框1
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 8, 0, 0, 0}, {0}},                   //11 选框2
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 9, 0, 0, 0}, {0}},                   //12 选框3
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 10, 0, 0, 0}, {0}},                  //13 选框4
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 11, 0, 0, 0}, {0}},                  //14 选框5
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 12, 0, 0, 0}, {0}},                  //15 选框6
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 13, 0, 0, 0}, {0}},                  //16 选框7
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 14, 0, 0, 0}, {0}},                  //17 选框8
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 15, 0, 0, 0}, {0}},                  //18 选框9
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 16, 0, 0, 0}, {0}},                  //19 选框10
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 17, 0, 0, 0}, {0}},                  //20 选框11
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 18, 0, 0, 0}, {0}},                  //21 选框12
        {{KEY_Silver}, {0}, {243}, {TLFNADD_FUN_READMEM + 20, 0, 0, 0}, {0}},                           //22 跳转读取
    };

    const RECTPAD rect[RECT_FTREADMEM_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_READMEM}},

        {CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //1 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //2 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //3 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //4 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //5 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //6 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //7 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //8 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                         //9 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                       //10 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                       //11 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                       //12 选框12
        {TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                          //13 换页提示
        {TYPE_X10 - KEY_X1 * 2 - SAFE_DIST4, TYPE_Y4 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //14 模组按键框
        {CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //15 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //16 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //17 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //18 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //19 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //20 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //21 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //22 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                      //23 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                    //24 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                    //25 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                                    //26 选框12

    };

    const BMPPAD disBmp[BMP_FTREADMEM_MAX] = {
        //icon
        {0, 0, 0, 0, {0}},

        {CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //1 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //2 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //3 选框3
        {CONVAS_PX2_4, CONVAS_PY2_4 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //4 选框4
        {CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //5 选框5
        {CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //6 选框6
        {CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //7 选框7
        {CONVAS_PX2_8, CONVAS_PY2_8 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //8 选框8
        {CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //9 选框9
        {CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //10 选框10
        {CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //11 选框11
        {CONVAS_PX2_12, CONVAS_PY2_12 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //12 选框12

    };
    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_READ);

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************

    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //读取模式
    //画出方框
    for (i = 1; i < RECT_FTREADMEM_MAX; i++)
    {
        if (i < RECT_FTREADMEM_PAGE)
        {
            BmpFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize);
        }
        else if (i != RECT_FTREADMEM_MOD)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
        }
    }
    //画出按键显示
    for (i = 2; i < KEY_FTREADMEM_MAX; i++)
    {
        //无效按键

        //不需要显示的按键
        if (i >= KEY_FTREADMEM_SEL1 && i <= KEY_FTREADMEM_SEL12) //非常规按钮
        {
            // DrawButtonDisPat(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
        else if (i >= KEY_FTREADMEM_FLASH && i <= KEY_FTREADMEM_SD)
        {
        }
        else //常规按钮
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    //画出BMP显示
    for (i = 1; i < BMP_FTREADMEM_MAX; i++)
    {
        DrawFPBMPBTN1(disBmp[i].xpis, disBmp[i].ypis, disBmp[i].xsize, disBmp[i].ysize, disBmp[i].icon[0]);
    }
    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************

    ViewPage(usepageno); //设置显示缓存
    if (ExtMemory_Status(USBTF) == 0)
    {
        USBFlag = 1;
        // USBFlag1 = 1;
    }
    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    tmpselectdisno = g_patfno;
    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值
        if (keydata >= KEY_FTREADMEM_FLASH && keydata <= KEY_FTREADMEM_SD)
        {
            keydata = 0;
        }
        if (keydata >= KEY_FTREADMEM_SEL1 && keydata <= KEY_FTREADMEM_SEL12) //非常规按键
        {
            f = keydata - KEY_FTREADMEM_SEL1;
            if (PatternList.selectEnable[f] == 0)
                keydata = 0;
        }
        // /[按键键值返回预处理*begin]**********************************************************

        //特定情况下不生效按键预处理

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
            if (((keydata == KEY_FTREADMEM_USB) && (USBFlag == 0)) || ((keydata == KEY_FTREADMEM_SD) && (SDFlag == 0)))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
                {
                    keyon = keydata;
                    if (keyon == KEY_FTREADMEM_SCRNSVR ||
                        (keyon >= KEY_FTREADMEM_SEL1 && keyon <= KEY_FTREADMEM_SEL12)) //无按键特效
                    {
                    }
                    //非常规按键
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
                    //带有长按连续功能按键的功能
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
            if (keyoff >= KEY_FTREADMEM_SEL1 && keyoff <= KEY_FTREADMEM_SEL12)
            {

                PatternList.noInPageNow = keyoff - KEY_FTREADMEM_SEL1;
                PatternList.bFlashSelectFlag = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTREADMEM_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;

                case KEY_FTREADMEM_BACK:
                    pnoindex--;
                    pno = poldno[pnoindex];
                    break;

                case KEY_FTREADMEM_OK:
                    if (g_Combinepat[g_Groupsel1][0] == 0xffff || g_Combinepat[g_Groupsel1][0] == 0) //修改-当前选择编号无文件
                    {
                        endfile = 1;
                    }
                    else //在当前位置插入文件，其他后移
                    {
                        endfile = 0;
                        i = GroupMax - 2;
                        while (i >= g_Groupsel1)
                        {
                            for (j = 0; j < 6; j++)
                            {
                                g_Combinepat[i + 1][j] = g_Combinepat[i][j];
                            }
                            if (i == 0)
                                break;
                            i--;
                        }
                    }

                    g_Combinepat[g_Groupsel1][0] = tmpselectdisno; //编号
                    for (i = 0; i < 4; i++)                        //提取前面8个名称字符
                    {
                        g_Combinepat[g_Groupsel1][i + 1] = IREG2[PatternList.dataAdd[PatternList.noInPageNow] + FileIndexNamepox + i];
                    }
                    Chinesenum = 0;
                    for (j = 0; j < 4; j++)
                    {
                        if (g_Combinepat[g_Groupsel1][j + 1] & 0x80)
                            Chinesenum++;
                        if ((g_Combinepat[g_Groupsel1][j + 1] >> 8) & 0x80)
                            Chinesenum++;
                    }
                    if (Chinesenum % 2) //最后一个字符为中文，显示不完全时不显示该字符
                    {
                        g_Combinepat[g_Groupsel1][4] &= 0xff;
                    }

                    if (endfile)
                    { //当前选择编号无文件 插入文件后位置加一
                        g_Groupsel1++;
                    }

                    if (g_GroupMenth == 2) //如果是读取组合图形时，修改了数据，则为默认方式
                    {
                        g_GroupMenth = 0;
                    }
                    pnoindex--;
                    pno = poldno[pnoindex];
                    break;

                case KEY_FTREADMEM_PREV:
                    if (PatternList.pageNow > 1)
                    {
                        PatternList.pageNow--;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_FTREADMEM_NEXT:
                    if (PatternList.pageNow < PatternList.pageNum)
                    {
                        PatternList.pageNow++;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;

                case KEY_FTREADMEM_DREAD:
                    pno = MENU_FTDIRREAD;
                    poldno[pnoindex] = MENU_FTGROUPREAD1;
                    pnoindex++;
                    break;
                }
            }
            keyoff = 0;
        }
        // /[按键松开键值处理*end]**********************************************************

        // /[按键弹起显示信息处理*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {

            if (keyon == KEY_FTREADMEM_SCRNSVR ||
                (keyon >= KEY_FTREADMEM_SEL1 && keyon <= KEY_FTREADMEM_SEL12)) //无按键特效
            {
            }
            //非常规按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回读取模式
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[更新花样数据,并显示*begin]**********************************************************
        if (dis_updata3)
        {
            U16 chFileNoTemp = 0, cntTemp = 0;
            dis_updata3 = 0;
            for (cntTemp = 0; cntTemp < PatternList.disNumDiv; cntTemp++)
            {
                if (PatternList.dataAdd[cntTemp] == 0xffff)
                {
                    continue;
                }
                chFileNoTemp = PatternList.dataAdd[cntTemp] / FileIndexSize + MINFILENO;
                f = fun_getPatData(chFileNoTemp, g_disk_memory, MENU_FTGROUPREAD1);
                if (f)
                {
                    if (g_disppicflag)
                        refresh5 = 1;
                    g_init_flag = 1;
                    g_data_flash = 1;
                    // g_selectdisno = F_IND_NO;
                }
                else
                {
                    continue;
                }

                wdt();
                DelayMs(20);
                // /[绘制花样与相关信息*begin]******************************************************************************
                if (refresh5)
                {
                    refresh5 = 0;
                    if (cntTemp == PatternList.disNumDiv)
                    {
                        break;
                    }

                    if (S_2HD) //根据次原点信息分析显示花样的数据
                        AnaPattern(1, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
                    else
                        AnaPattern(0, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));

                    // SetDrawXY(rect[cntTemp + RECT_FTREADMEM_IMG1].xpis, rect[cntTemp + RECT_FTREADMEM_IMG1].ypis,
                    //           rect[cntTemp + RECT_FTREADMEM_IMG1].xpis + CONVAS_X2, rect[cntTemp + RECT_FTREADMEM_IMG1].ypis + CONVAS_Y2, 1);
                    // Draw_Pattern(PAT_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, ((F_FILELENH << 16) | F_FILELENL), 0, 0, NULL); // 显示花样 - 三菱款
                    // SetDrawXY(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);

                    pat_setDrawXY(patRule.h_patConvasIndex, rect[cntTemp + RECT_FTREADMEM_IMG1].xpis, rect[cntTemp + RECT_FTREADMEM_IMG1].ypis);
                    pat_drawPattern(PATRULES_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, F_FILELEN, 0, 0, NULL);
                }
                // /[绘制花样与相关信息*end]******************************************************************************
                wdt();
            }
        }
        // /[更新花样数据*end]**********************************************************

        // /[内存按键处理*begin]**********************************************************
        if (refresh4) // SD卡 内存按键选中处理
        {
            U8 chSelectMem = 0;
            refresh4 = 0;
            chSelectMem = KEY_FTREADMEM_FLASH + g_disk_memory;

            for (i = KEY_FTREADMEM_FLASH; i <= KEY_FTREADMEM_SD; i++)
            {
                if (i == KEY_FTREADMEM_USB)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
                               i == chSelectMem ? keybmpind[i].icon[1] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
                else if (i == KEY_FTREADMEM_SD)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
                               i == chSelectMem ? keybmpind[i].icon[1] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
                else if (i == KEY_FTREADMEM_FLASH)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : keybmpind[i].border[0],
                               i == chSelectMem ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
            }
        }
        // /[内存按键处理*end]**********************************************************

        // /[更新花样显示列表*begin]**********************************************************
        if (PatternList.bflashAllFlag) //索引信息更新
        {
            PatternList.bflashAllFlag = 0;
            PatternList.bFlashPageFlag = 1;
            if (g_disk_memory == 0)
            {
                tmpselectdisno = g_Fileno;
            }
            else
            {
                tmpselectdisno = MAXFILENO;
            }
            PatternList.selectNoNow = 0;
            PatternList.patternNum = 0;
            for (i = 0; i < MAXIREG2; i += FileIndexSize) //遍历花样存储空间，获取 最大合花样数量、当前花样序号
            {
                if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
                    (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                {
                    PatternList.patternNum++;
                    if ((i / FileIndexSize + MINFILENO) <= tmpselectdisno)
                        PatternList.selectNoNow++;
                }
            }

            //确定 当前选中项、当前选中页、页码总数
            PatternList.noInPageNow = (PatternList.selectNoNow - 1) % PatternList.disNumDiv;
            PatternList.noInPageLast = PatternList.noInPageNow;
            PatternList.pageNow = (PatternList.selectNoNow - 1) / PatternList.disNumDiv + 1;
            PatternList.pageLast = PatternList.pageNow;
            PatternList.pageNum = (PatternList.patternNum - 1) / PatternList.disNumDiv + 1;

            if (PatternList.pageNum == 0) //
                PatternList.pageNum = 1;

            if (PatternList.pageNow == 0)
                PatternList.pageNow = 1;
            else if (PatternList.pageNow > PatternList.pageNum)
                PatternList.pageNow = PatternList.pageNum;

            if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;
        }

        if (PatternList.bFlashSelectFlag) //索引选中项更新
        {
            PatternList.bFlashSelectFlag = 0;

            if (PatternList.dataAdd[PatternList.noInPageNow] == 0xFFFF) //选中空
            {
                PatternList.noInPageNow = PatternList.noInPageLast;
                return;
            }
            else
            {
                PatternList.noInPageLast = PatternList.noInPageNow;
            }

            tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO; //转化为花样序号disno

            if (tmpselectdisno != 0xffff &&
                tmpselectdisno != g_selectdisno)
            {
                PatternList.selectNoNow = PatternList.noInPageNow + MINFILENO + (PatternList.pageNow - 1) * PatternList.disNumDiv;
                PatternList.selectNoLast = PatternList.selectNoNow;
                PatternList.pageLast = PatternList.pageNow;

                // g_selectdisno = tmpselectdisno;//只是勾选，没有确认
                // g_oldselectdisno = g_selectdisno;
            }

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }
            if (PatternList.pageNow == PatternList.pageLast)
            {
                PatternList.selectList[PatternList.noInPageNow] = 1;
            }
            // patlen = (((U32)(IREG2[PatternList.dataAdd[PatternList.noInPageNow] + 1]) << 16) + IREG2[PatternList.dataAdd[PatternList.noInPageNow]]) / 2;

            refresh2 = 1;
        }

        if (PatternList.bFlashPageFlag) //索引换页更新
        {

            U32 numA = 0; //当前页的花样序号基数
            // U32 numB = 0; //当前页的花样序号偏移数
            U32 addA = 0; //当前页的花样序号基地址
            U32 addB = 0; //当前页的花样序号偏移地址
            U32 patternLen = 0;
            U8 patternType = 0;
            U16 disColor = 0;
            PatternList.bFlashPageFlag = 0;
            refresh2 = 1;
            dis_updata3 = 1;

            //显示页码
            DrawFillRect(rect[RECT_FTREADMEM_PAGE].xpis, rect[RECT_FTREADMEM_PAGE].ypis,
                         rect[RECT_FTREADMEM_PAGE].xsize, rect[RECT_FTREADMEM_PAGE].ysize, rect[RECT_FTREADMEM_PAGE].colorB[0]);
            DrawFU32(rect[RECT_FTREADMEM_PAGE].xpis + SAFE_DIST2, rect[RECT_FTREADMEM_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNow, 3, rect[RECT_FTREADMEM_PAGE].colorT[0]);
            DrawF16Str(rect[RECT_FTREADMEM_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_FTREADMEM_PAGE].ypis + SAFE_DIST4,
                       "/", rect[RECT_FTREADMEM_PAGE].colorT[0]);
            DrawFU32(rect[RECT_FTREADMEM_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_FTREADMEM_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNum, 3, rect[RECT_FTREADMEM_PAGE].colorT[0]);

            if (PatternList.patternNum == 0)
                PatternList.noInPageNow = 0xff;
            else if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;

            numA = (PatternList.pageNow - 1) * PatternList.disNumDiv;
            if (numA > PatternList.patternNum)
                numA = PatternList.patternNum;
            // numB = PatternList.patternNum % PatternList.disNumDiv;
            // if ((PatternList.noInPageNow >= numB) && (PatternList.pageNow == PatternList.pageNum) && (PatternList.patternNum))
            // 	PatternList.noInPageNow = 0;

            if (numA) //计算当前页的基地址
            {
                for (j = 0, addA = 0; addA < MAXIREG2; addA += FileIndexSize)
                {
                    if ((((IREG2[addA + 1] << 16) | IREG2[addA]) > 0) &&
                        (((IREG2[addA + 1] << 16) | IREG2[addA]) <= 2 * MAXSREG))
                    {
                        j++;
                        if (j > numA)
                            break;
                    }
                }
            }
            addB = addA;
            if (numA < PatternList.patternNum) //计算当前页的各个文件的偏移地址
            {
                for (j = 0; addB < MAXIREG2; addB += FileIndexSize)
                {
                    if ((((IREG2[addB + 1] << 16) | IREG2[addB]) > 0) &&
                        (((IREG2[addB + 1] << 16) | IREG2[addB]) <= 2 * MAXSREG))
                    {
                        PatternList.dataAdd[j] = addB;
                        j++;
                        if (((numA + j) >= PatternList.patternNum) || (j >= PatternList.disNumDiv))
                            break;
                    }
                }
                for (; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }
            else
            {
                for (j = 0; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }
            if (PatternList.pageNow == PatternList.pageLast)
            {
                PatternList.selectList[PatternList.noInPageNow] = 1;
                tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO; //转化为花样序号disno
                // patlen = (((U32)(IREG2[PatternList.dataAdd[PatternList.noInPageNow] + 1]) << 16) + IREG2[PatternList.dataAdd[PatternList.noInPageNow]]) / 2;
            }

            //绘制按钮信息
            for (i = 0; i < PatternList.disNumDiv; i++)
            {

                DrawFillRect(rect[i + RECT_FTREADMEM_IMG1].xpis, rect[i + RECT_FTREADMEM_IMG1].ypis,
                             rect[i + RECT_FTREADMEM_IMG1].xsize, rect[i + RECT_FTREADMEM_IMG1].ysize,
                             rect[i + RECT_FTREADMEM_IMG1].colorB[0]);

                BmpFillRect(rect[i + RECT_FTREADMEM_SEL1].xpis, rect[i + RECT_FTREADMEM_SEL1].ypis,
                            rect[i + RECT_FTREADMEM_SEL1].xsize + SAFE_DIST6, rect[i + RECT_FTREADMEM_SEL1].ysize);

                patternLen = ((IREG2[(PatternList.dataAdd[i]) + 1] << 16) | IREG2[(PatternList.dataAdd[i])]);
                patternType = ((IREG2[(PatternList.dataAdd[i]) + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
                disColor = ((PatternList.dataAdd[i] < MAXIREG2) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
                               ? ((patternType != 0) ? MitFontColor : BorFontColor)
                               : keybmpind[i + RECT_FTREADMEM_SEL1].textColor[0];
                DrawFFileno_16x8(rect[i + RECT_FTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3, rect[i + RECT_FTREADMEM_SEL1].ypis + SAFE_DIST1,
                                 (PatternList.dataAdd[i] >= MAXIREG2) ? 0 : PatternList.dataAdd[i] / FileIndexSize + MINFILENO, disColor);
                DrawF16Str(rect[i + RECT_FTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 3,
                           rect[i + RECT_FTREADMEM_SEL1].ypis + SAFE_DIST1,
                           "@", rect[i + RECT_FTREADMEM_SEL1].colorT[0]);

                if (PatternList.dataAdd[i] >= MAXIREG2)
                {
                    DrawFFilename(rect[i + RECT_FTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
                                  rect[i + RECT_FTREADMEM_SEL1].ypis + SAFE_DIST4,
                                  "NO__DATA",
                                  strlen("NO__DATA"), NO_DATAColor, 6, 0, NULL);
                    PatternList.selectEnable[i] = 0;
                }
                else
                {
                    DrawFFilename(rect[i + RECT_FTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
                                  rect[i + RECT_FTREADMEM_SEL1].ypis + SAFE_DIST4,
                                  (char *)(&IREG2[PatternList.dataAdd[i] + FileIndexNamepox]),
                                  64, disColor, 6, 0, NULL);
                    PatternList.selectEnable[i] = 1;
                }
            }
            wdt();
        }
        // /[更新花样显示列表*end]**********************************************************

        // /[文件选中BMP处理*begin]**********************************************************
        if (refresh2) // 文件编号选中处理
        {
            refresh2 = 0;
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                BmpFillRect(disBmp[i + 1].xpis, disBmp[i + 1].ypis,
                            disBmp[i + 1].xsize, disBmp[i + 1].ysize);
                DrawFPBMPBTN1(disBmp[i + 1].xpis, disBmp[i + 1].ypis,
                              disBmp[i + 1].xsize, disBmp[i + 1].ysize,
                              PatternList.selectList[i] ? disBmp[i + 1].icon[1] : disBmp[i + 1].icon[0]);
            }
        }
        // /[文件选中BMP处理*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        if (key_on_flag1)
            ErrorMsg(MENU_FTGROUPREAD1);
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_FTGROUPREAD1);
            if (Timer_ms >= 30000) //30720
            {
                Timer_ms = 0;
            }
        }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************

        // /[页面切换*begin]**********************************************************
        if (pno != MENU_FTGROUPREAD1)
        {
            plastno = MENU_FTGROUPREAD1;
            break;
        }

        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

/*********************************************/

#define KEY_DIRFTREADMEM_SCRNSVR 1 //1 屏保
#define KEY_DIRFTREADMEM_BACK 2    //2 返回
#define KEY_DIRFTREADMEM_OK 3      //3 确认
#define KEY_DIRFTREADMEM_PREV 4    //4 上一页
#define KEY_DIRFTREADMEM_NEXT 5    //5 下一页
#define KEY_DIRFTREADMEM_SEL1 6    //6 选框1
#define KEY_DIRFTREADMEM_SEL2 7    //7 选框2
#define KEY_DIRFTREADMEM_SEL3 8    //8 选框3
#define KEY_DIRFTREADMEM_SEL4 9    //9 选框4
#define KEY_DIRFTREADMEM_SEL5 10   //10 选框5
#define KEY_DIRFTREADMEM_SEL6 11   //11 选框6
#define KEY_DIRFTREADMEM_SEL7 12   //12 选框7
#define KEY_DIRFTREADMEM_SEL8 13   //13 选框8
#define KEY_DIRFTREADMEM_SEL9 14   //14 选框9
#define KEY_DIRFTREADMEM_CLEAN 15  //15 清除一位
#define KEY_DIRFTREADMEM_0 16      //16 0
#define KEY_DIRFTREADMEM_1 17      //17 1
#define KEY_DIRFTREADMEM_2 18      //18 2
#define KEY_DIRFTREADMEM_3 19      //19 3
#define KEY_DIRFTREADMEM_4 20      //20 4
#define KEY_DIRFTREADMEM_5 21      //21 5
#define KEY_DIRFTREADMEM_6 22      //22 6
#define KEY_DIRFTREADMEM_7 23      //23 7
#define KEY_DIRFTREADMEM_8 24      //24 8
#define KEY_DIRFTREADMEM_9 25      //25 9
#define KEY_DIRFTREADMEM_MAX 26

#define RECT_DIRFTREADMEM_SEL1 1     //1 选框1
#define RECT_DIRFTREADMEM_SEL2 2     //2 选框2
#define RECT_DIRFTREADMEM_SEL3 3     //3 选框3
#define RECT_DIRFTREADMEM_SEL4 4     //4 选框4
#define RECT_DIRFTREADMEM_SEL5 5     //5 选框5
#define RECT_DIRFTREADMEM_SEL6 6     //6 选框6
#define RECT_DIRFTREADMEM_SEL7 7     //7 选框7
#define RECT_DIRFTREADMEM_SEL8 8     //8 选框8
#define RECT_DIRFTREADMEM_SEL9 9     //9 选框9
#define RECT_DIRFTREADMEM_PAGE 10    //10 换页提示
#define RECT_DIRFTREADMEM_NUM_TIT 11 //11 按钮标题
#define RECT_DIRFTREADMEM_NUM_STR 12 //12 按钮内容
#define RECT_DIRFTREADMEM_IMG1 13    //13 选框1
#define RECT_DIRFTREADMEM_IMG2 14    //14 选框2
#define RECT_DIRFTREADMEM_IMG3 15    //15 选框3
#define RECT_DIRFTREADMEM_IMG4 16    //16 选框4
#define RECT_DIRFTREADMEM_IMG5 17    //17 选框5
#define RECT_DIRFTREADMEM_IMG6 18    //18 选框6
#define RECT_DIRFTREADMEM_IMG7 19    //19 选框7
#define RECT_DIRFTREADMEM_IMG8 20    //20 选框8
#define RECT_DIRFTREADMEM_IMG9 21    //21 选框9

#define RECT_DIRFTREADMEM_MAX 22

#define BMP_DIRFTREADMEM_SEL1 1 //1 选框1
#define BMP_DIRFTREADMEM_SEL2 2 //2 选框2
#define BMP_DIRFTREADMEM_SEL3 3 //3 选框3
#define BMP_DIRFTREADMEM_SEL4 4 //4 选框4
#define BMP_DIRFTREADMEM_SEL5 5 //5 选框5
#define BMP_DIRFTREADMEM_SEL6 6 //6 选框6
#define BMP_DIRFTREADMEM_SEL7 7 //7 选框7
#define BMP_DIRFTREADMEM_SEL8 8 //8 选框8
#define BMP_DIRFTREADMEM_SEL9 9 //9 选框9

#define BMP_DIRFTREADMEM_MAX 10
//------------------------------------------
// Function   : void Menu_FtDirReadMem(void)
// Description: 直接读取花样画面
//------------------------------------------
void Menu_FtDirReadMem(void)
{

    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
    /****按键相关参数end****/

    /****花样序号，花样名称，花样列表相关参数begin****/

    U16 tmp_s_2hd = (U16)(S_2HD);                                                   //次原点位置缓存
    tydPatternList PatternList = {1, 0, 0, 9, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}}; //花样列表参数
    /****花样序号，花样名称，花样列表相关参数end****/

    /****花样显示相关参数begin****/
    S32 Scale = 0; //tscale：图像显示需要调整的缩放比，tscale1：最终的总缩放比，Scale：固定的缩放比

    const S16 orgListX[] = {CONVAS_ORG_PX2_1, CONVAS_ORG_PX2_2, CONVAS_ORG_PX2_3, CONVAS_ORG_PX2_5,
                            CONVAS_ORG_PX2_6, CONVAS_ORG_PX2_7, CONVAS_ORG_PX2_9, CONVAS_ORG_PX2_10,
                            CONVAS_ORG_PX2_11};
    const S16 orgListY[] = {CONVAS_ORG_PY2_1, CONVAS_ORG_PY2_2, CONVAS_ORG_PY2_3, CONVAS_ORG_PY2_5,
                            CONVAS_ORG_PY2_6, CONVAS_ORG_PY2_7, CONVAS_ORG_PY2_9, CONVAS_ORG_PY2_10,
                            CONVAS_ORG_PY2_11};
    /****花样显示相关参数end****/

    /****标志位相关参数begin****/
    U32 i, j; //,ii,jj,ofesst,s
    U8 f = 0;

    U16 patno = 0; //显示花样筛选数字输入
    U8 disall = 1; //显示所有花样标志
    U8 Chinesenum;
    U8 endfile = 0;

    U8 refresh2 = 0, refresh5 = 1;
    U8 dis_updata1 = 0, dis_updata3 = 0; //disupdata1:索引更新标志 disupdata2：当前文件所在索引位置更新标志  disupdata3：花样数据更新标志
    /****标志位相关参数end***/

    const U8 keynum = {KEY_DIRFTREADMEM_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                                   //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                //2 返回
        {TYPE_X11, TYPE_Y3, KEY_X1, KEY_Y1},                               //3 确认
        {331, TYPE_Y4, KEY_X1, KEY_Y1},                                    //4 上一页
        {331 + (KEY_X1 + SAFE_DIST5) * 2, TYPE_Y4, KEY_X1, KEY_Y1},        //5 下一页
        {CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //6 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //7 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //8 选框3
        {CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //9 选框4
        {CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //10 选框5
        {CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //11 选框6
        {CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //12 选框7
        {CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5}, //13 选框8
        {CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5}, //14 选框9

        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},       //15 清除一位
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},                             //16 0
        {TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},                                                         //17 1
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13, KEY_X2, KEY_Y2},                                   //18 2
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13, KEY_X2, KEY_Y2},                             //19 3
        {TYPE_X15, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},                                   //20 4
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},             //21 5
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},       //22 6
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},                             //23 7
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},       //24 8
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //25 9

    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_READMEM, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_READMEM + 1, 0, 0, 0}, {0}},             //1 屏保
        {{KEY_Silver}, {0}, {72}, {TLFNADD_FUN_READMEM + 2, 0, 0, 0}, {0}},            //2 返回
        {{KEY_Silver}, {0}, {71}, {TLFNADD_FUN_READMEM + 3, 0, 0, 0}, {0}},            //3 确认
        {{KEY_Silver}, {0}, {223, 224}, {TLFNADD_FUN_READMEM + 5, 0, 0, 0}, {0}},      //8 上一页
        {{KEY_Silver}, {0}, {225, 226}, {TLFNADD_FUN_READMEM + 6, 0, 0, 0}, {0}},      //9 下一页
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 7, 0, 0, 0}, {0}},  //14 选框1
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 8, 0, 0, 0}, {0}},  //15 选框2
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 9, 0, 0, 0}, {0}},  //16 选框3
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 10, 0, 0, 0}, {0}}, //17 选框4
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 11, 0, 0, 0}, {0}}, //18 选框5
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 12, 0, 0, 0}, {0}}, //19 选框6
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 13, 0, 0, 0}, {0}}, //20 选框7
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 14, 0, 0, 0}, {0}}, //21 选框8
        {{Color_White}, {0}, {Color_Black}, {TLFNADD_FUN_READMEM + 15, 0, 0, 0}, {0}}, //22 选框9

        {{KEY_Silver}, {0}, {113}, {TLFNADD_FUN_READMEM + 21, 0, 0, 0}, {0}}, //8 输入清除
        {{KEY_Silver}, {0}, {91}, {0, 0, 0, 0}, {0}},                         //11 0
        {{KEY_Silver}, {0}, {92}, {0, 0, 0, 0}, {0}},                         //12 1
        {{KEY_Silver}, {0}, {93}, {0, 0, 0, 0}, {0}},                         //13 2
        {{KEY_Silver}, {0}, {94}, {0, 0, 0, 0}, {0}},                         //14 3
        {{KEY_Silver}, {0}, {95}, {0, 0, 0, 0}, {0}},                         //15 4
        {{KEY_Silver}, {0}, {96}, {0, 0, 0, 0}, {0}},                         //16 5
        {{KEY_Silver}, {0}, {97}, {0, 0, 0, 0}, {0}},                         //17 6
        {{KEY_Silver}, {0}, {98}, {0, 0, 0, 0}, {0}},                         //18 7
        {{KEY_Silver}, {0}, {99}, {0, 0, 0, 0}, {0}},                         //19 8
        {{KEY_Silver}, {0}, {100}, {0, 0, 0, 0}, {0}},                        //20 9

    };

    const RECTPAD rect[RECT_DIRFTREADMEM_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_READMEM + 0}},

        {CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //1 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //2 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //3 选框3
        {CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //4 选框4
        {CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //5 选框5
        {CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //6 选框6
        {CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                      //7 选框7
        {CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                    //8 选框8
        {CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                    //9 选框9
        {331 + KEY_X1 + SAFE_DIST5, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //10 换页提示
        {TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_READMEM + 22}},                 //11 序号标题
        {TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}},         //12 序号内容
        {CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //13 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //14 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //15 选框3
        {CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //16 选框4
        {CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //17 选框5
        {CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //18 选框6
        {CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                   //19 选框7
        {CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                 //20 选框8
        {CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},                                 //21 选框9
    };

    const BMPPAD disBmp[BMP_DIRFTREADMEM_MAX] = {
        //icon
        {0, 0, 0, 0, {0}},

        {CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //1 选框1
        {CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //2 选框2
        {CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //3 选框3
        {CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //4 选框4
        {CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //5 选框5
        {CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //6 选框6
        {CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},   //7 选框7
        {CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //8 选框8
        {CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //9 选框9

    };
    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_READ);

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************

    g_disppicflag = 1;
    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //读取模式
    //画出方框
    for (i = 1; i < RECT_DIRFTREADMEM_MAX; i++)
    {
        if (i <= RECT_DIRFTREADMEM_SEL9)
        {
            BmpFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize);
        }
        else
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_DIRFTREADMEM_NUM_STR)
            {
                DrawDottedRect(rect[RECT_DIRFTREADMEM_NUM_STR].xpis, rect[RECT_DIRFTREADMEM_NUM_STR].ypis,
                               rect[RECT_DIRFTREADMEM_NUM_STR].xsize, rect[RECT_DIRFTREADMEM_NUM_STR].ysize,
                               rect[RECT_DIRFTREADMEM_NUM_STR].colorT[0]);
            }
            else if (i == RECT_DIRFTREADMEM_NUM_TIT)
            {
                Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST4,
                             MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //读取模式
            }
        }
    }
    //画出按键显示
    for (i = 2; i < KEY_DIRFTREADMEM_MAX; i++)
    {
        //无效按键

        //不需要显示的按键
        if (i == KEY_DIRFTREADMEM_OK) //非常规按钮
        {
        }
        else if (i >= KEY_DIRFTREADMEM_SEL1 && i <= KEY_DIRFTREADMEM_SEL9) //非常规按钮
        {
            // DrawButtonDisPat(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
        else //常规按钮
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    //画出BMP显示
    for (i = 1; i < BMP_DIRFTREADMEM_MAX; i++)
    {
        DrawFPBMPBTN1(disBmp[i].xpis, disBmp[i].ypis, disBmp[i].xsize, disBmp[i].ysize, disBmp[i].icon[0]);
    }
    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************

    ViewPage(usepageno); //设置显示缓存

    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    //	tmpselectdisno = g_oldselectdisno;
    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0);                     //获取键值
        if (keyon >= KEY_DIRFTREADMEM_SEL1 && keyon <= KEY_DIRFTREADMEM_SEL9) //非常规按键
        {
            f = keydata - KEY_DIRFTREADMEM_SEL1;
            if (PatternList.selectEnable[f] == 0)
                keydata = 0;
        }
        // /[按键键值返回预处理*begin]**********************************************************

        //特定情况下不生效按键预处理

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

            if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
            {
                keyon = keydata;
                if (keyon == KEY_DIRFTREADMEM_SCRNSVR || keyon == KEY_DIRFTREADMEM_OK ||
                    (keyon >= KEY_DIRFTREADMEM_SEL1 && keyon <= KEY_DIRFTREADMEM_SEL9)) //无按键特效
                {
                }
                //非常规按键
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
                    case KEY_DIRFTREADMEM_0:
                    case KEY_DIRFTREADMEM_1:
                    case KEY_DIRFTREADMEM_2:
                    case KEY_DIRFTREADMEM_3:
                    case KEY_DIRFTREADMEM_4:
                    case KEY_DIRFTREADMEM_5:
                    case KEY_DIRFTREADMEM_6:
                    case KEY_DIRFTREADMEM_7:
                    case KEY_DIRFTREADMEM_8:
                    case KEY_DIRFTREADMEM_9:
                        BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                        DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_DIRFTREADMEM_0, 1, rect[0].colorT[0]);
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
                //带有长按连续功能按键的功能
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
            if ((keyoff >= KEY_DIRFTREADMEM_0) && (keyoff <= KEY_DIRFTREADMEM_9))
            {
                patno = (patno % 100) * 10 + keyoff - KEY_DIRFTREADMEM_0;
                disall = 0;
                dis_updata1 = 1;
            }
            else if (keyoff >= KEY_DIRFTREADMEM_SEL1 && keyoff <= KEY_DIRFTREADMEM_SEL9)
            {

                if (PatternList.dataAdd[keyoff - KEY_DIRFTREADMEM_SEL1] != 0xffff)
                {
                    U16 tmpselectdisno = 0;
                    tmpselectdisno = PatternList.dataAdd[keyoff - KEY_DIRFTREADMEM_SEL1] / FileIndexSize + MINFILENO;
                    if (g_Combinepat[g_Groupsel1][0] == 0xffff || g_Combinepat[g_Groupsel1][0] == 0) //修改-当前选择编号无文件
                    {
                        endfile = 1;
                    }
                    else //在当前位置插入文件，其他后移
                    {
                        endfile = 0;
                        i = GroupMax - 2;
                        while (i >= g_Groupsel1)
                        {
                            for (j = 0; j < 6; j++)
                            {
                                g_Combinepat[i + 1][j] = g_Combinepat[i][j];
                            }
                            if (i == 0)
                                break;
                            i--;
                        }
                    }

                    g_Combinepat[g_Groupsel1][0] = tmpselectdisno; //编号
                    for (i = 0; i < 4; i++)                        //提取前面8个名称字符
                    {
                        g_Combinepat[g_Groupsel1][i + 1] = IREG2[PatternList.dataAdd[PatternList.noInPageNow] + FileIndexNamepox + i];
                    }
                    Chinesenum = 0;
                    for (j = 0; j < 4; j++)
                    {
                        if (g_Combinepat[g_Groupsel1][j + 1] & 0x80)
                            Chinesenum++;
                        if ((g_Combinepat[g_Groupsel1][j + 1] >> 8) & 0x80)
                            Chinesenum++;
                    }
                    if (Chinesenum % 2) //最后一个字符为中文，显示不完全时不显示该字符
                    {
                        g_Combinepat[g_Groupsel1][4] &= 0xff;
                    }

                    if (endfile)
                    { //当前选择编号无文件 插入文件后位置加一
                        g_Groupsel1++;
                    }

                    if (g_GroupMenth == 2)
                    { //如果是读取组合图形时，修改了数据，则为默认方式
                        g_GroupMenth = 0;
                    }
                    pnoindex--;
                    pno = poldno[pnoindex];
                    if (pno == MENU_FTGROUPREAD1)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    break;
                }
            }
            else
            {
                switch (keyoff)
                {
                case KEY_DIRFTREADMEM_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_DIRFTREADMEM_BACK:
                    pnoindex--;
                    pno = poldno[pnoindex];
                    break;

                case KEY_DIRFTREADMEM_CLEAN:
                    if (patno)
                    {
                        patno /= 10;
                        dis_updata1 = 1;
                    }
                    else if (disall == 0)
                    {
                        disall = 1;
                        dis_updata1 = 1;
                    }
                    break;

                case KEY_DIRFTREADMEM_PREV:
                    if (PatternList.pageNow > 1)
                    {
                        PatternList.pageNow--;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_DIRFTREADMEM_NEXT:
                    if (PatternList.pageNow < PatternList.pageNum)
                    {
                        PatternList.pageNow++;
                        PatternList.bFlashPageFlag = 1;
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

            if (keyon == KEY_DIRFTREADMEM_SCRNSVR || keyon == KEY_DIRFTREADMEM_OK ||
                (keyon >= KEY_DIRFTREADMEM_SEL1 && keyon <= KEY_DIRFTREADMEM_SEL9)) //无按键特效
            {
            }
            //非常规按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回读取模式
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[更新花样数据*begin]**********************************************************
        if (dis_updata3)
        {
            U16 chFileNoTemp = 0, cntTemp = 0;
            dis_updata3 = 0;

            for (cntTemp = 0; cntTemp < PatternList.disNumDiv; cntTemp++)
            {

                if (PatternList.dataAdd[cntTemp] == 0xffff)
                {
                    continue;
                }
                chFileNoTemp = PatternList.dataAdd[cntTemp] / FileIndexSize + MINFILENO;
                f = fun_getPatData(chFileNoTemp, g_disk_memory, MENU_FTDIRREAD);
                if (f)
                {
                    if (g_disppicflag)
                        refresh5 = 1;
                    g_init_flag = 1;
                    g_data_flash = 1;
                    // g_selectdisno = F_IND_NO;
                }
                else
                {
                    continue;
                }

                wdt();
                DelayMs(20);
                // /[绘制花样与相关信息*begin]******************************************************************************
                if (refresh5)
                {
                    refresh5 = 0;
                    if (cntTemp == PatternList.disNumDiv)
                    {
                        break;
                    }

                    if (S_2HD) //根据次原点信息分析显示花样的数据
                        AnaPattern(1, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
                    else
                        AnaPattern(0, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));

                    // SetDrawXY(rect[cntTemp + RECT_DIRFTREADMEM_IMG1].xpis, rect[cntTemp + RECT_DIRFTREADMEM_IMG1].ypis,
                    //           rect[cntTemp + RECT_DIRFTREADMEM_IMG1].xpis + CONVAS_X2, rect[cntTemp + RECT_DIRFTREADMEM_IMG1].ypis + CONVAS_Y2, 1);
                    // Draw_Pattern(PAT_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, ((F_FILELENH << 16) | F_FILELENL), 0, 0, NULL); // 显示花样 - 三菱款
                    // SetDrawXY(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);
                    pat_setDrawXY(patRule.h_patConvasIndex, rect[cntTemp + RECT_DIRFTREADMEM_IMG1].xpis, rect[cntTemp + RECT_DIRFTREADMEM_IMG1].ypis);
                    pat_drawPattern(PATRULES_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, F_FILELEN, 0, 0, NULL);
                }
                // /[绘制花样与相关信息*end]******************************************************************************
                wdt();
            }
        }
        // /[更新花样数据*end]**********************************************************

        // /[更新花样显示列表*begin]**********************************************************
        if (PatternList.bflashAllFlag) //索引信息更新
        {
            PatternList.bflashAllFlag = 0;
            PatternList.bFlashPageFlag = 1;

            PatternList.selectNoNow = 0;
            PatternList.patternNum = 0;
            for (i = 0; i < MAXIREG2; i += FileIndexSize) //遍历花样存储空间，获取 最大合花样数量、当前花样序号
            {
                // if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
                // 	(((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                // {
                // 	PatternList.patternNum++;
                // 	if ((i / FileIndexSize + MINFILENO) <= g_selectdisno)
                // 		PatternList.selectNoNow++;

                if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
                    (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                {
                    U16 tmpPatNo = 0;
                    tmpPatNo = i / FileIndexSize + MINFILENO;
                    if (disall) //显示全部
                    {
                        PatternList.patternNum++;
                    }
                    else
                    {
                        if (isViewNo(tmpPatNo, patno))
                        {
                            PatternList.patternNum++;
                        }
                    }
                }
            }

            //确定 当前选中项、当前选中页、页码总数
            PatternList.noInPageNow = (PatternList.selectNoNow - 1) % PatternList.disNumDiv;
            PatternList.noInPageLast = PatternList.noInPageNow;
            PatternList.pageNow = (PatternList.selectNoNow - 1) / PatternList.disNumDiv + 1;
            PatternList.pageLast = PatternList.pageNow;
            PatternList.pageNum = (PatternList.patternNum - 1) / PatternList.disNumDiv + 1;

            if (PatternList.pageNum == 0)
                PatternList.pageNum = 1;

            if (PatternList.pageNow == 0)
                PatternList.pageNow = 1;
            else if (PatternList.pageNow > PatternList.pageNum)
                PatternList.pageNow = PatternList.pageNum;

            if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;
        }

        if (PatternList.bFlashPageFlag) //索引换页更新
        {

            U32 numA = 0; //当前页的花样序号基数
            // U32 numB = 0; //当前页的花样序号偏移数
            U32 addA = 0; //当前页的花样序号基地址
            U32 addB = 0; //当前页的花样序号偏移地址
            U32 patternLen = 0;
            U8 patternType = 0;
            U16 disColor = 0;
            PatternList.bFlashPageFlag = 0;
            refresh2 = 1;
            dis_updata3 = 1;

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }

            //显示页码
            DrawFillRect(rect[RECT_DIRFTREADMEM_PAGE].xpis, rect[RECT_DIRFTREADMEM_PAGE].ypis,
                         rect[RECT_DIRFTREADMEM_PAGE].xsize, rect[RECT_DIRFTREADMEM_PAGE].ysize, rect[RECT_DIRFTREADMEM_PAGE].colorB[0]);
            DrawFU32(rect[RECT_DIRFTREADMEM_PAGE].xpis + SAFE_DIST2, rect[RECT_DIRFTREADMEM_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNow, 3, rect[RECT_DIRFTREADMEM_PAGE].colorT[0]);
            DrawF16Str(rect[RECT_DIRFTREADMEM_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_DIRFTREADMEM_PAGE].ypis + SAFE_DIST4,
                       "/", rect[RECT_DIRFTREADMEM_PAGE].colorT[0]);
            DrawFU32(rect[RECT_DIRFTREADMEM_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_DIRFTREADMEM_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNum, 3, rect[RECT_DIRFTREADMEM_PAGE].colorT[0]);

            if (PatternList.patternNum == 0)
                PatternList.noInPageNow = 0xff;
            else if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;

            numA = (PatternList.pageNow - 1) * PatternList.disNumDiv;
            if (numA > PatternList.patternNum)
                numA = PatternList.patternNum;
            // numB = PatternList.patternNum % PatternList.disNumDiv;
            // if ((PatternList.noInPageNow >= numB) && (PatternList.pageNow == PatternList.pageNum) && (PatternList.patternNum))
            // 	PatternList.noInPageNow = 0;

            if (numA) //计算当前页的基地址
            {
                for (j = 0, addA = 0; addA < MAXIREG2; addA += FileIndexSize)
                {
                    if ((((IREG2[addA + 1] << 16) | IREG2[addA]) > 0) &&
                        (((IREG2[addA + 1] << 16) | IREG2[addA]) <= 2 * MAXSREG))
                    {
                        U16 tmpPatNo = 0;
                        tmpPatNo = addA / FileIndexSize + MINFILENO;
                        if (disall) //显示全部
                        {
                            j++;
                            if (j > numA)
                                break;
                        }
                        else
                        {
                            if (isViewNo(tmpPatNo, patno))
                            {
                                j++;
                                if (j > numA)
                                    break;
                            }
                        }
                    }
                }
            }
            addB = addA;
            if (numA < PatternList.patternNum) //计算当前页的各个文件的偏移地址
            {
                for (j = 0; addB < MAXIREG2; addB += FileIndexSize)
                {
                    if ((((IREG2[addB + 1] << 16) | IREG2[addB]) > 0) &&
                        (((IREG2[addB + 1] << 16) | IREG2[addB]) <= 2 * MAXSREG))
                    {

                        U16 tmpPatNo = 0;
                        tmpPatNo = addB / FileIndexSize + MINFILENO;
                        if (disall) //显示全部
                        {
                            PatternList.dataAdd[j] = addB;
                            j++;
                            if (((numA + j) >= PatternList.patternNum) || (j >= PatternList.disNumDiv))
                                break;
                        }
                        else
                        {
                            if (isViewNo(tmpPatNo, patno))
                            {
                                PatternList.dataAdd[j] = addB;
                                j++;
                                if (((numA + j) >= PatternList.patternNum) || (j >= PatternList.disNumDiv))
                                    break;
                            }
                        }
                    }
                }
                for (; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }
            else
            {
                for (j = 0; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }
            //绘制按钮信息
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                DrawFillRect(rect[i + RECT_DIRFTREADMEM_IMG1].xpis, rect[i + RECT_DIRFTREADMEM_IMG1].ypis,
                             rect[i + RECT_DIRFTREADMEM_IMG1].xsize, rect[i + RECT_DIRFTREADMEM_IMG1].ysize,
                             rect[i + RECT_DIRFTREADMEM_IMG1].colorB[0]);

                BmpFillRect(rect[i + RECT_DIRFTREADMEM_SEL1].xpis, rect[i + RECT_DIRFTREADMEM_SEL1].ypis,
                            rect[i + RECT_DIRFTREADMEM_SEL1].xsize + SAFE_DIST6, rect[i + RECT_DIRFTREADMEM_SEL1].ysize);

                patternLen = ((IREG2[(PatternList.dataAdd[i]) + 1] << 16) | IREG2[(PatternList.dataAdd[i])]);
                patternType = ((IREG2[(PatternList.dataAdd[i]) + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
                disColor = ((PatternList.dataAdd[i] < MAXIREG2) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
                               ? ((patternType != 0) ? MitFontColor : BorFontColor)
                               : keybmpind[i + RECT_DIRFTREADMEM_SEL1].textColor[0];
                DrawFFileno_16x8(rect[i + RECT_DIRFTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3, rect[i + RECT_DIRFTREADMEM_SEL1].ypis + SAFE_DIST1,
                                 (PatternList.dataAdd[i] >= MAXIREG2) ? 0 : PatternList.dataAdd[i] / FileIndexSize + MINFILENO, disColor);
                DrawF16Str(rect[i + RECT_DIRFTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 3,
                           rect[i + RECT_DIRFTREADMEM_SEL1].ypis + SAFE_DIST1,
                           "@", rect[i + RECT_DIRFTREADMEM_SEL1].colorT[0]);

                if (PatternList.dataAdd[i] >= MAXIREG2)
                {
                    DrawFFilename(rect[i + RECT_DIRFTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
                                  rect[i + RECT_DIRFTREADMEM_SEL1].ypis + SAFE_DIST4,
                                  "NO__DATA", strlen("NO__DATA"), NO_DATAColor, 6, 0, NULL);
                    PatternList.selectEnable[i] = 0;
                }
                else
                {
                    DrawFFilename(rect[i + RECT_DIRFTREADMEM_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
                                  rect[i + RECT_DIRFTREADMEM_SEL1].ypis + SAFE_DIST4,
                                  (char *)(&IREG2[PatternList.dataAdd[i] + FileIndexNamepox]),
                                  64, disColor, 6, 0, NULL);
                    PatternList.selectEnable[i] = 1;
                }
            }
            wdt();
        }
        // /[更新花样显示列表*end]**********************************************************

        // /[文件选中BMP处理*begin]**********************************************************
        if (refresh2) // 文件编号选中处理
        {
            refresh2 = 0;
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                BmpFillRect(disBmp[i + 1].xpis, disBmp[i + 1].ypis,
                            disBmp[i + 1].xsize, disBmp[i + 1].ysize);
                DrawFPBMPBTN1(disBmp[i + 1].xpis, disBmp[i + 1].ypis,
                              disBmp[i + 1].xsize, disBmp[i + 1].ysize,
                              PatternList.selectList[i] ? disBmp[i + 1].icon[1] : disBmp[i + 1].icon[0]);
            }
        }
        // /[文件选中BMP处理*end]**********************************************************

        // /[数字更新*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
            PatternList.bflashAllFlag = 1;
            DrawFillRect(rect[RECT_DIRFTREADMEM_NUM_STR].xpis, rect[RECT_DIRFTREADMEM_NUM_STR].ypis,
                         rect[RECT_DIRFTREADMEM_NUM_STR].xsize, rect[RECT_DIRFTREADMEM_NUM_STR].ysize,
                         rect[RECT_DIRFTREADMEM_NUM_STR].colorB[0]);
            DrawDottedRect(rect[RECT_DIRFTREADMEM_NUM_STR].xpis, rect[RECT_DIRFTREADMEM_NUM_STR].ypis,
                           rect[RECT_DIRFTREADMEM_NUM_STR].xsize, rect[RECT_DIRFTREADMEM_NUM_STR].ysize,
                           rect[RECT_DIRFTREADMEM_NUM_STR].colorT[0]);
            if (disall == 0)
            {
                DrawMFU32Z(rect[RECT_DIRFTREADMEM_NUM_STR].xpis + (rect[RECT_DIRFTREADMEM_NUM_STR].xsize - 12 * 3) / 2,
                           rect[RECT_DIRFTREADMEM_NUM_STR].ypis + SAFE_DIST4, patno, 3, rect[RECT_DIRFTREADMEM_NUM_STR].colorT[0]);
            }
        }
        // /[数字更新*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        // if (key_on_flag1)
        // 	ErrorMsg(MENU_READ);
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        // if (beeponf == 0)
        // {
        // 	ErrorMsg(MENU_READ);
        // 	if (Timer_ms >= 30000) //30720
        // 	{
        // 		Timer_ms = 0;
        // 	}
        // }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************

        // /[页面切换*begin]**********************************************************

        if (pno != MENU_FTDIRREAD)
        {
            plastno = MENU_FTDIRREAD;
            break;
        }

        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

//------------------------------------------
#define KEY_FTREADCOM_SCRNSVR 1  //1 屏保
#define KEY_FTREADCOM_BACK 2     //2 返回
#define KEY_FTREADCOM_ENTER 3    //3 确认
#define KEY_FTREADCOM_MEM 4      //4 FLASH
#define KEY_FTREADCOM_USB 5      //5 USB
#define KEY_FTREADCOM_SD 6       //6 SD
#define KEY_FTREADCOM_COMBINE 7  //7 组合模式
#define KEY_FTREADCOM_CONECT 8   //8 接合模式
#define KEY_FTREADCOM_DEL 9      //9 删除
#define KEY_FTREADCOM_PREV 10    //10 上一页
#define KEY_FTREADCOM_NEXT 11    //11 下一页
#define KEY_FTREADCOM_SEL1 12    //12 选择-1
#define KEY_FTREADCOM_SEL2 13    //13 选择-2
#define KEY_FTREADCOM_SEL3 14    //14 选择-3
#define KEY_FTREADCOM_SEL4 15    //15 选择-4
#define KEY_FTREADCOM_SEL5 16    //16 选择-5
#define KEY_FTREADCOM_SEL6 17    //17 选择-6
#define KEY_FTREADCOM_SEL7 18    //18 选择-7
#define KEY_FTREADCOM_SEL8 19    //19 选择-8
#define KEY_FTREADCOM_SEL9 20    //20 选择-9
#define KEY_FTREADCOM_IMGPREV 21 //21 预览图像-上页
#define KEY_FTREADCOM_IMGNEXT 22 //22 预览图像-下页
#define KEY_FTREADCOM_IMGBAR 23  //23 预览图像-拖拽
#define KEY_FTREADCOM_MAX 24

#define RECT_FTREADCOM_PAGE 1   //1 换页提示
#define RECT_FTREADCOM_MOD 2    //2 内存选择
#define RECT_FTREADCOM_IMGBAR 3 //3 图像拖拽条
#define RECT_FTREADCOM_NUM1 4   //4 花样序号-1
#define RECT_FTREADCOM_NAME1 5  //5 花样名称-1
#define RECT_FTREADCOM_NUM2 6   //6 花样序号-2
#define RECT_FTREADCOM_NAME2 7  //7 花样名称-2
#define RECT_FTREADCOM_NUM3 8   //8 花样序号-3
#define RECT_FTREADCOM_NAME3 9  //9 花样名称-3
#define RECT_FTREADCOM_NUM4 10  //10 花样序号-4
#define RECT_FTREADCOM_NAME4 11 //11 花样名称-4
#define RECT_FTREADCOM_NUM5 12  //12 花样序号-5
#define RECT_FTREADCOM_NAME5 13 //13 花样名称-5
#define RECT_FTREADCOM_MAX 14

#define BMP_FTREADCOM_SEL1 1 //1 选框1
#define BMP_FTREADCOM_SEL2 2 //2 选框2
#define BMP_FTREADCOM_SEL3 3 //3 选框3
#define BMP_FTREADCOM_SEL4 4 //4 选框4
#define BMP_FTREADCOM_SEL5 5 //5 选框5
#define BMP_FTREADCOM_MAX 6

//------------------------------------------
// Function   : void Menu_FtReadCombine(void)
// Description: 功能画面_图形组合	读组合花样
//------------------------------------------
void Menu_FtReadCombine(void)
{
    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
    /****按键相关参数end****/
    tydPatternList PatternList = {1, 0, 0, 9, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //组合花样列表参数
    /****标志位相关参数begin****/
    U32 i, j; //,ii,jj,ofesst,s
    U8 f = 0;
    static U8 CombineType = 1;
    U8 disname[9] = {0};
    U8 Chinesenum = 0;
    U16 add;
    U8 SDFlag = 0, SDFlag1 = 0, USBFlag = 0, USBFlag1 = 0;
    U8 comPageMax = 0, comPageNow = 0, comSum = 0, comPageDiv = 5;
    U8 groupno;

    U8 dis_updata2 = 0, dis_updata3 = 1, dis_updata4 = 1, dis_updata5 = 1, dis_updata6 = 1;

    /****标志位相关参数end***/

    const U8 keynum = {KEY_FTREADCOM_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                                                                          //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                                                       //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                                                                      //3 确认
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},                                                                      //4 内存
        {TYPE_X10 - KEY_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                                             //5 USB
        {TYPE_X10 - KEY_X1 * 2, TYPE_Y4, KEY_X1, KEY_Y1},                                                         //6 SD
        {0, 0, 0, 0},                                                                                             //7 组合模式
        {0, 0, 0, 0},                                                                                             //8 接合模式
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},                                                                       //9 删除
        {TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},                                                                       //10 上一页
        {TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},                                                                       //11 下一页
        {TYPE_X2, 190, TYPE_SIZE18, TYPE_SIZE8},                                                                  //12 选择-1
        {TYPE_X2 + TYPE_SIZE18 + SAFE_DIST8, 190, TYPE_SIZE18, TYPE_SIZE8},                                       //13 选择-2
        {TYPE_X2 + (TYPE_SIZE18 + SAFE_DIST8) * 2, 190, TYPE_SIZE18, TYPE_SIZE8},                                 //14 选择-3
        {TYPE_X2, 190 + TYPE_SIZE8 + SAFE_DIST8, TYPE_SIZE18, TYPE_SIZE8},                                        //15 选择-4
        {TYPE_X2 + TYPE_SIZE18 + SAFE_DIST8, 190 + TYPE_SIZE8 + SAFE_DIST8, TYPE_SIZE18, TYPE_SIZE8},             //16 选择-5
        {TYPE_X2 + (TYPE_SIZE18 + SAFE_DIST8) * 2, 190 + TYPE_SIZE8 + SAFE_DIST8, TYPE_SIZE18, TYPE_SIZE8},       //17 选择-6
        {TYPE_X2, 190 + (TYPE_SIZE8 + SAFE_DIST8) * 2, TYPE_SIZE18, TYPE_SIZE8},                                  //18 选择-7
        {TYPE_X2 + TYPE_SIZE18 + SAFE_DIST8, 190 + (TYPE_SIZE8 + SAFE_DIST8) * 2, TYPE_SIZE18, TYPE_SIZE8},       //19 选择-8
        {TYPE_X2 + (TYPE_SIZE18 + SAFE_DIST8) * 2, 190 + (TYPE_SIZE8 + SAFE_DIST8) * 2, TYPE_SIZE18, TYPE_SIZE8}, //20 选择-9
        {TYPE_X18, TYPE_Y2, 350, TYPE_SIZE15},                                                                    //21 预览图像-上一页
        {TYPE_X18 + 350, TYPE_Y2, 350, TYPE_SIZE15},                                                              //22 预览图像-下一页
        {TYPE_X18, TYPE_Y2, 700, TYPE_SIZE15},                                                                    //23 预览图像-拖拽
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_READCOM, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_READCOM + 1, 0, 0, 0}, {0}},                                 //1 屏保
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {TLFNADD_FUN_READCOM + 2, 0, 0, 0}, {0}},        //2 返回
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {TLFNADD_FUN_READCOM + 3, 0, 0, 0}, {0}},        //3 确认
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {257, 259, 258}, {TLFNADD_FUN_READCOM + 5, 0, 0, 0}, {0}}, //4 内存
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {251, 253, 252}, {TLFNADD_FUN_READCOM + 6, 0, 0, 0}, {0}}, //5 USB
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {254, 256, 255}, {TLFNADD_FUN_READCOM + 7, 0, 0, 0}, {0}}, //6 SD
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {11, 13, 12}, {TLFNADD_FUN_READCOM + 8, 0, 0, 0}, {0}},    //7 组合模式
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {11, 13, 12}, {TLFNADD_FUN_READCOM + 9, 0, 0, 0}, {0}},    //8 接合模式

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {111, 112}, {TLFNADD_FUN_READCOM + 10, 0, 0, 0}, {0}},                          //9 删除
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {223, 224}, {TLFNADD_FUN_READCOM + 11, 0, 0, 0}, {0}},                          //10 上一页
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {225, 226}, {TLFNADD_FUN_READCOM + 12, 0, 0, 0}, {0}},                          //11 下一页
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 13, 0, 0, 0}, {0}}, //12 选框1
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 14, 0, 0, 0}, {0}}, //13 选框2
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 15, 0, 0, 0}, {0}}, //14 选框3
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 16, 0, 0, 0}, {0}}, //15 选框4
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 17, 0, 0, 0}, {0}}, //16 选框5
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 18, 0, 0, 0}, {0}}, //17 选框6
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 19, 0, 0, 0}, {0}}, //18 选框7
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 20, 0, 0, 0}, {0}}, //19 选框8
        {{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue}, {TLFNADD_FUN_READCOM + 21, 0, 0, 0}, {0}}, //20 选框9

        {{Color_Black}, {Color_Black}, {Color_Silver}, {TLFNADD_FUN_READCOM + TLFNLEN_FUN_READCOM - 1, 0, 0, 0}, {0}}, //21 预览图像-上一页
        {{Color_Black}, {Color_Black}, {Color_Silver}, {TLFNADD_FUN_READCOM + TLFNLEN_FUN_READCOM - 1, 0, 0, 0}, {0}}, //22 预览图像-下一页
        {{Color_Black}, {Color_Black}, {Color_Silver}, {TLFNADD_FUN_READCOM + TLFNLEN_FUN_READCOM - 1, 0, 0, 0}, {0}}, //23 预览图像-拖拽
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_READCOM}},

        {TYPE_X6, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                      //1 换页提示
        {TYPE_X10 - KEY_X1 * 2 - SAFE_DIST4, TYPE_Y4 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //2 模组按键框
        {TYPE_X18, TYPE_Y2, 700, TYPE_SIZE15, {Color_Silver}, {Color_Black}, {0}},                                                                  //3 图像拖拽条

        {TYPE_X18 + SAFE_DIST7, TYPE_Y2 + SAFE_DIST8, KEY_X4, TYPE_SIZE8, {Color_White}, {Color_Black}, {0}},                                                                        //4 花样序号-1
        {TYPE_X18 + SAFE_DIST7, TYPE_Y2 + SAFE_DIST8 + TYPE_SIZE8 + SAFE_DIST6, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                              //5 花样名称-1
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2), TYPE_Y2 + SAFE_DIST8, KEY_X4, TYPE_SIZE8, {Color_White}, {Color_Black}, {0}},                               //6 花样序号-2
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2), TYPE_Y2 + SAFE_DIST8 + TYPE_SIZE8 + SAFE_DIST6, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},     //7 花样名称-2
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 2, TYPE_Y2 + SAFE_DIST8, KEY_X4, TYPE_SIZE8, {Color_White}, {Color_Black}, {0}},                           //8 花样序号-3
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 2, TYPE_Y2 + SAFE_DIST8 + TYPE_SIZE8 + SAFE_DIST6, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //9 花样名称-3
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 3, TYPE_Y2 + SAFE_DIST8, KEY_X4, TYPE_SIZE8, {Color_White}, {Color_Black}, {0}},                           //10 花样序号-4
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 3, TYPE_Y2 + SAFE_DIST8 + TYPE_SIZE8 + SAFE_DIST6, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //11 花样名称-4
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 4, TYPE_Y2 + SAFE_DIST8, KEY_X4, TYPE_SIZE8, {Color_White}, {Color_Black}, {0}},                           //12 花样序号-5
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 4, TYPE_Y2 + SAFE_DIST8 + TYPE_SIZE8 + SAFE_DIST6, KEY_X4, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //13 花样名称-5
    };

    const BMPPAD disBmp[] = {
        //icon
        {0, 0, 0, 0, {0}},

        {TYPE_X18 + SAFE_DIST7 - SAFE_DIST5 - TYPE_SIZE5, 85, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}},                                              //1
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) - SAFE_DIST5 - TYPE_SIZE5, 85, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}},     //2
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 2 - SAFE_DIST5 - TYPE_SIZE5, 85, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //3
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 3 - SAFE_DIST5 - TYPE_SIZE5, 85, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //4
        {TYPE_X18 + SAFE_DIST7 + (KEY_X4 + TYPE_SIZE5 + SAFE_DIST5 * 2) * 4 - SAFE_DIST5 - TYPE_SIZE5, 85, TYPE_SIZE5, TYPE_SIZE5, {785, 786, 0}}, //5

    };

    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();

    // /[显示相关标志位信息读取*begin]**********************************************************
    if (g_checkGroup == 0)
    {
        CombineType = g_GroupType;
        g_disk_memory = 0;
    }

    if (g_checkGroup == 0)
    {
        CombineType = g_GroupType;
        g_disk_memory = 0;
    }
    add = (CombineType == 1) ? GroupComAddr : GroupConAddr;
    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
    //画出方框
    //画出按键显示
    for (i = 2; i <= KEY_FTREADCOM_NEXT; i++)
    {
        //无效按键
        if ((g_GroupCover == 1) && ((i >= KEY_FTREADCOM_MEM && i <= KEY_FTREADCOM_SD) || (i == KEY_FTREADCOM_DEL))) //不需要显示的按键
        {
        }
        else if (i == KEY_FTREADCOM_COMBINE || i == KEY_FTREADCOM_CONECT)
        {
        }
        else if ((i >= KEY_FTREADCOM_MEM && i <= KEY_FTREADCOM_SD))
        {
        }
        else //常规按钮
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************
    if (ExtMemory_Status(USBTF) == 0)
    {
        USBFlag = 1;
        USBFlag1 = 1;
    }
    g_disk_memory = 0;   //默认主板内存
    ViewPage(usepageno); //设置显示缓存

    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值
        if (keydata == KEY_FTREADCOM_COMBINE || keydata == KEY_FTREADCOM_CONECT)
        {
            keydata = 0;
        }
        if (keydata == KEY_FTREADCOM_IMGBAR)
        {
            keydata = 0;
        }
        if ((keydata >= KEY_FTREADCOM_SEL1) && (keydata <= KEY_FTREADCOM_SEL9))
        {
            f = keydata - KEY_FTREADCOM_SEL1;
            if (PatternList.selectEnable[f] == 0)
                keydata = 0;
        }
        // /[按键键值返回预处理*begin]**********************************************************
        //特定情况下不生效按键预处理

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
            if (((keydata == KEY_FTREADCOM_USB) && (USBFlag == 0)) || ((keydata == KEY_FTREADCOM_SD) && (SDFlag == 0)))
            {
                keydata = 0;
            }
            else if ((keydata == KEY_FTREADCOM_DEL) && ((g_GroupCover == 1)))
            {
                keydata = 0;
            }
            else if ((keydata >= KEY_FTREADCOM_USB) && (keydata <= KEY_FTREADCOM_SD) && (g_GroupCover == 1))
            {
                keydata = 0;
            }
            else if ((((keydata == KEY_FTREADCOM_COMBINE) && (CombineType == 2)) ||
                      ((keydata == KEY_FTREADCOM_CONECT) && (CombineType == 1))) &&
                     (g_GroupCover == 1))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
                {
                    keyon = keydata;
                    if (keyon == KEY_FTREADCOM_SCRNSVR || keyon == KEY_FTREADCOM_IMGPREV || keyon == KEY_FTREADCOM_IMGNEXT ||
                        keyon == KEY_FTREADCOM_IMGBAR) //无按键特效
                    {
                    }
                    //非常规按键
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
                    //带有长按连续功能按键的功能
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
            if ((keyoff >= KEY_FTREADCOM_SEL1) && (keyoff <= KEY_FTREADCOM_SEL9))
            {
                PatternList.noInPageNow = keyoff - KEY_FTREADCOM_SEL1;
                PatternList.bFlashSelectFlag = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTREADCOM_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;

                case KEY_FTREADCOM_BACK:
                    if (g_disk_memory != 0)
                    { //当前不是内存文件，读回内存组合文件
                        if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                        { //发送读取组合图形指令
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                        ReadMemorryList(MENU_FTGROUPREAD2); //读回内存文件索引
                        g_disk_memory = 0;
                    }

                    g_checkGroup = 0;

                    for (i = 0; i < GroupMax; i++) //清除数据
                    {
                        for (j = 0; j < 6; j++)
                        {
                            if (j == 0)
                            {
                                g_Combinepat[i][j] = 0xffff; //图形编号
                            }
                            else
                            {
                                g_Combinepat[i][j] = 0;
                            }
                        }
                    }

                    //显示回当前组合文件
                    for (groupno = 0, i = 0; i < GREG[GroupPNumAddr]; i++, groupno++)
                    {
                        while ((IREG2[(GREG[GroupPNoAddr + i] - 1) * FileIndexSize + 1] == 0xffff) && (IREG2[(GREG[GroupPNoAddr + i] - 1) * FileIndexSize] == 0xffff))
                        { //内存里无该花样，跳过
                            GREG[GroupPNoAddr + i] = GREG[GroupPNoAddr + i + 1];
                            i++;
                        }
                        if (i >= GREG[GroupPNumAddr])
                            break;
                        g_Combinepat[groupno][0] = GREG[GroupPNoAddr + i]; //图形花样编号
                        for (j = 0; j < 4; j++)
                        { //图形花样名称,只显示前面8个字符
                            g_Combinepat[groupno][j + 1] = IREG2[(GREG[GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j];
                        }
                        Chinesenum = 0;
                        for (j = 0; j < 4; j++)
                        {
                            if (g_Combinepat[i][j + 1] & 0x80)
                                Chinesenum++;
                            if ((g_Combinepat[i][j + 1] >> 8) & 0x80)
                                Chinesenum++;
                        }
                        if (Chinesenum % 2)
                        { //最后一个字符为中文，显示不完全时不显示该字符
                            g_Combinepat[i][4] &= 0xff;
                        }
                    }
                    g_GroupName[0] = GREG[GroupPNameAddr] & 0xff;
                    g_GroupName[1] = (GREG[GroupPNameAddr] >> 8) & 0xff;
                    g_GroupName[2] = GREG[GroupPNameAddr + 1] & 0xff;
                    g_GroupName[3] = (GREG[GroupPNameAddr + 1] >> 8) & 0xff;
                    g_GroupName[4] = GREG[GroupPNameAddr + 2] & 0xff;
                    g_GroupName[5] = (GREG[GroupPNameAddr + 2] >> 8) & 0xff;
                    g_GroupName[6] = GREG[GroupPNameAddr + 3] & 0xff;
                    g_GroupName[7] = (GREG[GroupPNameAddr + 3] >> 8) & 0xff;

                    pnoindex--;
                    pno = poldno[pnoindex];
                    break;

                case KEY_FTREADCOM_ENTER:
                    if (g_GroupCover != 1)
                    {
                        U8 bErrorPat = 0;
                        for (i = 0; i < GroupMax; i++)
                        { //清除数据
                            for (j = 0; j < 6; j++)
                            {
                                if (j == 0)
                                {
                                    g_Combinepat[i][j] = 0xffff; //图形编号
                                }
                                else
                                {
                                    g_Combinepat[i][j] = 0;
                                }
                            }
                        }

                        if (PatternList.dataAdd[PatternList.noInPageNow] > MAXGREG) //选择出错
                        {

                            if (g_disk_memory != 0)
                            { //当前不是内存文件，读回内存组合文件
                                if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                                { //发送读取组合图形指令
                                    pno = MENU_RWHELP;
                                    hleppara = 16;
                                    break;
                                }
                                if (Read_Status() == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] != 100)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    break;
                                }
                                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                                ReadMemorryList(MENU_FTGROUPREAD2); //读回内存文件索引
                                g_disk_memory = 0;
                            }

                            //显示回当前组合文件
                            for (groupno = 0, i = 0; i < GREG[GroupPNumAddr]; i++, groupno++)
                            {
                                while ((IREG2[(GREG[GroupPNoAddr + i] - 1) * FileIndexSize + 1] == 0xffff) &&
                                       (IREG2[(GREG[GroupPNoAddr + i] - 1) * FileIndexSize] == 0xffff))
                                { //内存里无该花样，跳过
                                    i++;
                                    bErrorPat++;
                                }
                                if (i >= GREG[GroupPNumAddr])
                                    break;
                                g_Combinepat[groupno][0] = GREG[GroupPNoAddr + i]; //图形花样编号
                                for (j = 0; j < 4; j++)
                                { //图形花样名称,只显示前面8个字符
                                    g_Combinepat[groupno][j + 1] = IREG2[(GREG[GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j];
                                }

                                Chinesenum = 0;
                                for (j = 0; j < 4; j++)
                                {
                                    if (g_Combinepat[i][j + 1] & 0x80)
                                        Chinesenum++;
                                    if ((g_Combinepat[i][j + 1] >> 8) & 0x80)
                                        Chinesenum++;
                                }
                                if (Chinesenum % 2)
                                { //最后一个字符为中文，显示不完全时不显示该字符
                                    g_Combinepat[i][4] &= 0xff;
                                }
                            }

                            g_GroupName[0] = GREG[GroupPNameAddr] & 0xff;
                            g_GroupName[1] = (GREG[GroupPNameAddr] >> 8) & 0xff;
                            g_GroupName[2] = GREG[GroupPNameAddr + 1] & 0xff;
                            g_GroupName[3] = (GREG[GroupPNameAddr + 1] >> 8) & 0xff;
                            g_GroupName[4] = GREG[GroupPNameAddr + 2] & 0xff;
                            g_GroupName[5] = (GREG[GroupPNameAddr + 2] >> 8) & 0xff;
                            g_GroupName[6] = GREG[GroupPNameAddr + 3] & 0xff;
                            g_GroupName[7] = (GREG[GroupPNameAddr + 3] >> 8) & 0xff;

                            g_checkGroup = 0;
                        }
                        else
                        {

                            for (groupno = 0, i = 0; i < GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNumAddr]; i++, groupno++)
                            {
                                while ((IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - 1) * FileIndexSize + 1] == 0xffff) &&
                                       (IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - 1) * FileIndexSize] == 0xffff))
                                { //内存里无该花样，跳过
                                    i++;
                                    bErrorPat++;
                                }
                                if (i >= GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNumAddr])
                                    break;
                                g_Combinepat[groupno][0] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i]; //组合花样子花样序号
                                for (j = 0; j < 4; j++)
                                { //图形花样名称,只显示前面8个字符
                                    g_Combinepat[groupno][j + 1] = IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j];
                                }
                            }

                            g_GroupName[0] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr] & 0xff;
                            g_GroupName[1] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr] >> 8) & 0xff;
                            g_GroupName[2] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 1] & 0xff;
                            g_GroupName[3] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 1] >> 8) & 0xff;
                            g_GroupName[4] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 2] & 0xff;
                            g_GroupName[5] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 2] >> 8) & 0xff;
                            g_GroupName[6] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 3] & 0xff;
                            g_GroupName[7] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 3] >> 8) & 0xff;

                            g_GroupType = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPTypeAddr];
                            g_GroupPatNums = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNumAddr];
                            g_GroupDoingNO = (PatternList.dataAdd[PatternList.noInPageNow] - add) / GroupPatMax + 1;

                            if (g_disk_memory == 0) //读内存组合文件
                            {
                                g_checkGroup = 0;
                                // if (bErrorPat == 0)
                                //     g_GroupMenth = 2; //花样组中花样均正常，则读取
                                // else
                                //     g_GroupMenth = 0; //不正常则保存
                                g_GroupMenth = 0; //都改为保存模式
                            }
                            else
                            {
                                pno = MENU_RWHELP;
                                hleppara = 111;
                                poldno[pnoindex] = MENU_FTGROUPREAD2;
                                pnoindex++;
                                g_checkGroup = 1;
                                break;
                            }
                        }
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else //盖写时，只取选择的编号
                    {
                        g_GroupDoingNO = (PatternList.dataAdd[PatternList.noInPageNow] - add) / GroupPatMax + 1;
                        g_checkGroup = 0;
                        pnoindex -= 2;
                        pno = poldno[pnoindex];

                        //修改
                        pnoindex++;
                        for (i = 0; i < GroupPatMax; i++) //清0
                        {
                            GREG[i] = 0;
                        }

                        for (i = 0; i < GroupMax; i++)
                        {
                            if (g_Combinepat[i][0] == 0xffff)
                                break;
                            GREG[i] = g_Combinepat[i][0]; //图形花样编号
                        }

                        GREG[GroupPNameAddr] = (g_GroupName[1] << 8) | g_GroupName[0];
                        GREG[GroupPNameAddr + 1] = (g_GroupName[3] << 8) | g_GroupName[2];
                        GREG[GroupPNameAddr + 2] = (g_GroupName[5] << 8) | g_GroupName[4];
                        GREG[GroupPNameAddr + 3] = (g_GroupName[7] << 8) | g_GroupName[6];
                        GREG[GroupPNumAddr] = i;
                        GREG[GroupPTypeAddr] = g_GroupType;
                        if (g_GroupSave == 1) //保存到内存
                        {
                            if (Write_D(D_Address[GROUP], GroupPatMax, (U16 *)(&GREG[0])) == 0)
                            { //发送参数
                                pno = MENU_RWHELP;
                                hleppara = 16;
                            }

                            if (Write_Cmd(C_SAVEGROUP, g_GroupType, g_GroupDoingNO) == 0) //发送保存组合文件指令
                            {
                                pno = MENU_RWHELP;
                                hleppara = 16;
                            }
                            if (Read_Status() == 0)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                            }
                            else if (DREG[D_CMDSTATUS] != 100)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                            }

                            if (g_GroupCover == 1)
                            {
                                g_GroupCover = 0;
                            }
                        }
                        else if ((g_GroupSave == 2) || (g_GroupSave == 3)) //保存到磁盘
                        {
                            pno = MENU_RWHELP;
                            hleppara = 110;
                        }
                    }

                    break;

                case KEY_FTREADCOM_MEM:
                    if (g_disk_memory != 0)
                    {
                        if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                        { //发送读取组合图形指令
                            g_checkGroup = 0;
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            poldno[pnoindex] = MENU_FTGROUPREAD2;
                            pnoindex++;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            g_checkGroup = 0;
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_FTGROUPREAD2;
                            pnoindex++;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            g_checkGroup = 0;
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_FTGROUPREAD2;
                            pnoindex++;
                            break;
                        }
                        Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                        ReadMemorryList(MENU_FTGROUPREAD2);
                    }
                    g_disk_memory = 0;
                    PatternList.bflashAllFlag = 1;
                    dis_updata3 = 1;
                    break;

                case KEY_FTREADCOM_USB:

                    if (ExtMemory_Status(USBTF) != 0)
                    {
                        g_checkGroup = 0;
                        pno = MENU_RWHELP;
                        hleppara = 95;
                        poldno[pnoindex] = MENU_FTGROUPREAD2;
                        pnoindex++;
                        break;
                    }
                    Select_ExtMemory(USBTF);
                    if (Explore_Disk_Group("0:/", 1))
                    {
                        g_checkGroup = 0;
                        pno = MENU_RWHELP;
                        hleppara = 40;
                        poldno[pnoindex] = MENU_FTGROUPREAD2;
                        pnoindex++;
                        //重新读回内存组合文件信息
                        if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                        { //发送读取组合图形指令
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                        if ((ReadMemorryList(MENU_FTGROUPREAD2)) == 0) // 如选中读优盘,在退出返回时要读回内存索引表
                        {
                            return;
                        }
                        break;
                    }

                    if (Explore_Disk("0:/", 1))
                    {
                        g_checkGroup = 0;
                        pno = MENU_RWHELP;
                        hleppara = 40;
                        poldno[pnoindex] = MENU_FTGROUPREAD2;
                        pnoindex++;
                        //重新读回内存组合文件信息
                        if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                        { //发送读取组合图形指令
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            //#if Debug
                            //printf("DREG[D_CMDSTATUS] = %d \r\n",DREG[D_CMDSTATUS]);
                            //#endif
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            break;
                        }
                        Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                        if ((ReadMemorryList(MENU_FTGROUPREAD2)) == 0)
                        { // 如选中读优盘,在退出返回时要读回内存索引表
                            return;
                        }
                        break;
                    }

                    g_disk_memory = 1;
                    PatternList.bflashAllFlag = 1;
                    dis_updata3 = 1;
                    break;

                case KEY_FTREADCOM_SD:
                    break;

                case KEY_FTREADCOM_COMBINE:
                    if (g_GroupCover != 1)
                    {
                        CombineType = 1;
                        add = GroupComAddr;
                        PatternList.bflashAllFlag = 1;
                        dis_updata2 = 1;
                    }
                    break;

                case KEY_FTREADCOM_CONECT:
                    if (g_GroupCover != 1)
                    {
                        CombineType = 2;
                        add = GroupConAddr;
                        PatternList.bflashAllFlag = 1;
                        dis_updata2 = 1;
                    }
                    break;

                case KEY_FTREADCOM_DEL:
                    g_GroupType = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPTypeAddr];
                    g_GroupDoingNO = (PatternList.dataAdd[PatternList.noInPageNow] - add) / GroupPatMax + 1;

                    g_checkGroup = 1;

                    g_tmpGroName[0] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr] & 0xff;
                    g_tmpGroName[1] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr] >> 8) & 0xff;
                    g_tmpGroName[2] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 1] & 0xff;
                    g_tmpGroName[3] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 1] >> 8) & 0xff;
                    g_tmpGroName[4] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 2] & 0xff;
                    g_tmpGroName[5] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 2] >> 8) & 0xff;
                    g_tmpGroName[6] = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 3] & 0xff;
                    g_tmpGroName[7] = (GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNameAddr + 3] >> 8) & 0xff;
                    g_tmpGroName[8] = 0;

                    pno = MENU_RWHELP;
                    poldno[pnoindex] = MENU_FTGROUPREAD2;
                    pnoindex++;
                    if (g_disk_memory == 0)
                    {
                        hleppara = 105;
                    }
                    else
                    {
                        hleppara = 107;
                    }

                    break;
                case KEY_FTREADCOM_PREV:
                    if (PatternList.pageNow > 1)
                    {
                        PatternList.pageNow--;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_FTREADCOM_NEXT:
                    if (PatternList.pageNow < PatternList.pageNum)
                    {
                        PatternList.pageNow++;
                        PatternList.bFlashPageFlag = 1;
                    }
                    break;
                case KEY_FTREADCOM_IMGPREV:
                    if (comPageNow > 1)
                    {
                        comPageNow--;
                        dis_updata5 = 1;
                    }
                    break;
                case KEY_FTREADCOM_IMGNEXT:
                    if (comPageNow < comPageMax)
                    {
                        comPageNow++;
                        dis_updata5 = 1;
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

            if (keyon == KEY_FTREADCOM_SCRNSVR || keyon == KEY_FTREADCOM_IMGPREV || keyon == KEY_FTREADCOM_IMGNEXT ||
                keyon == KEY_FTREADCOM_IMGBAR) //无按键特效
            {
            }
            //非常规按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回读取模式
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        {
            if (g_disk_memory == 1) //使用U盘
            {
                if (ExtMemory_Status(USBTF) != 0)
                {
                    USBFlag = 0;
                    pno = MENU_RWHELP;
                    hleppara = 95;
                    poldno[pnoindex] = MENU_FTGROUPREAD2;
                    pnoindex++;
                    //重新读回内存组合文件信息
                    if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                    { //发送读取组合图形指令
                        pno = MENU_RWHELP;
                        hleppara = 16;
                        break;
                    }
                    if (Read_Status() == 0)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 15;
                        break;
                    }
                    else if (DREG[D_CMDSTATUS] != 100)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 15;
                        break;
                    }
                    Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据
                    if ((ReadMemorryList(MENU_FTGROUPREAD2)) == 0)
                    { // 如选中读优盘,在退出返回时要读回内存索引表
                        return;
                    }
                }
                else
                {
                    USBFlag = 1;
                }
            }
            else if (g_disk_memory == 2) // 使用打板器SD
            {
            }
            else //使用主板内存
            {
                //查询一下U盘插入情况
                if (ExtMemory_Status(USBTF) == 0)
                {
                    USBFlag = 1;
                }
                else
                {
                    USBFlag = 0;
                }
            }
            if (SDFlag != SDFlag1)
            {
                SDFlag1 = SDFlag;
                dis_updata3 = 1;
            }
            if (USBFlag != USBFlag1)
            {
                USBFlag1 = USBFlag;
                dis_updata3 = 1;
            }
            Timerms = 0;
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[更新花样显示列表*begin]**********************************************************
        if (PatternList.bflashAllFlag) //索引信息更新
        {
            PatternList.bflashAllFlag = 0;
            PatternList.bFlashPageFlag = 1;

            for (PatternList.patternNum = 0, i = 0; i < GroupMax; i++) //组合模式、接合模式最多20组  每组30寄存器
            {
                if ((GREG[i * GroupPatMax + add + GroupPNoAddr] >= MINFILENO) && (GREG[i * GroupPatMax + add + GroupPNoAddr] <= MAXFILENO))
                    PatternList.patternNum++;
            }

            if (PatternList.patternNum)
            {
                PatternList.selectNoNow = 1;
            }
            else
            {
                PatternList.selectNoNow = 0;
            }
            PatternList.selectNoLast = PatternList.selectNoNow;

            //确定 当前选中项、当前选中页、页码总数
            PatternList.noInPageNow = (PatternList.selectNoNow - 1) % PatternList.disNumDiv;
            PatternList.noInPageLast = PatternList.noInPageNow;
            PatternList.pageNow = (PatternList.selectNoNow - 1) / PatternList.disNumDiv + 1;
            PatternList.pageLast = PatternList.pageNow;
            PatternList.pageNum = (PatternList.patternNum - 1) / PatternList.disNumDiv + 1;

            if (PatternList.pageNum == 0)
                PatternList.pageNum = 1;

            if (PatternList.pageNow == 0)
                PatternList.pageNow = 1;
            else if (PatternList.pageNow > PatternList.pageNum)
                PatternList.pageNow = PatternList.pageNum;

            if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;
        }

        if (PatternList.bFlashSelectFlag) //索引选中项更新
        {
            PatternList.bFlashSelectFlag = 0;

            if (PatternList.dataAdd[PatternList.noInPageNow] == 0xFFFF || PatternList.dataAdd[PatternList.noInPageNow] >= MAXGREG) //选中空
            {
                PatternList.noInPageNow = PatternList.noInPageLast;
                return;
            }
            else
            {
                PatternList.noInPageLast = PatternList.noInPageNow;
                comSum = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNumAddr];
                comPageMax = (comSum - 1) / comPageDiv + 1;
                comPageNow = 1;
            }

            PatternList.selectNoNow = PatternList.noInPageNow + 1 + (PatternList.pageNow - 1) * PatternList.disNumDiv;
            PatternList.selectNoLast = PatternList.selectNoNow;
            PatternList.pageLast = PatternList.pageNow;

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }
            if (PatternList.pageNow == PatternList.pageLast)
            {
                PatternList.selectList[PatternList.noInPageNow] = 1;
            }
            dis_updata5 = 1;
            dis_updata6 = 1;
        }

        if (PatternList.bFlashPageFlag) //索引换页更新
        {
            U32 numA = 0; //当前页的组合花样序号基数
            // U32 numB = 0; //当前页的组合花花样序号偏移数
            U32 addA = 0; //当前页的组合花样序号基地址
            U32 addB = 0; //当前页的组合花样序号偏移地址
            PatternList.bFlashPageFlag = 0;

            //显示页码
            DrawFillRect(rect[RECT_FTREADCOM_PAGE].xpis, rect[RECT_FTREADCOM_PAGE].ypis,
                         rect[RECT_FTREADCOM_PAGE].xsize, rect[RECT_FTREADCOM_PAGE].ysize, rect[RECT_FTREADCOM_PAGE].colorB[0]);
            DrawFU32(rect[RECT_FTREADCOM_PAGE].xpis + SAFE_DIST2, rect[RECT_FTREADCOM_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNow, 3, rect[RECT_FTREADCOM_PAGE].colorT[0]);
            DrawF16Str(rect[RECT_FTREADCOM_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_FTREADCOM_PAGE].ypis + SAFE_DIST4,
                       "/", rect[RECT_FTREADCOM_PAGE].colorT[0]);
            DrawFU32(rect[RECT_FTREADCOM_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_FTREADCOM_PAGE].ypis + SAFE_DIST4,
                     PatternList.pageNum, 3, rect[RECT_FTREADCOM_PAGE].colorT[0]);

            if (PatternList.patternNum == 0)
                PatternList.noInPageNow = 0xff;
            else if (PatternList.noInPageNow >= PatternList.disNumDiv)
                PatternList.noInPageNow = 0;

            numA = (PatternList.pageNow - 1) * PatternList.disNumDiv;
            if (numA > PatternList.patternNum)
                numA = PatternList.patternNum;
            // numB = PatternList.patternNum % PatternList.disNumDiv;
            // if ((PatternList.noInPageNow >= numB) && (PatternList.pageNow == PatternList.pageNum) && (PatternList.patternNum))
            // 	PatternList.noInPageNow = 0;

            if (numA) //计算当前页的基地址
            {
                for (j = 0, addA = 0; addA < GroupMax; addA++)
                {
                    if ((GREG[addA * GroupPatMax + add + GroupPNoAddr] >= MINFILENO) &&
                        (GREG[addA * GroupPatMax + add + GroupPNoAddr] <= MAXFILENO)) //判断每组花样的第一个花样序号是否为真
                    {
                        j++;
                        if (j > numA)
                            break;
                    }
                }
            }
            addB = addA;
            if (numA < PatternList.patternNum) //计算当前页的各个文件的偏移地址
            {
                for (j = 0; addB < GroupMax; addB++)
                {
                    if ((GREG[addB * GroupPatMax + add + GroupPNoAddr] >= MINFILENO) &&
                        (GREG[addB * GroupPatMax + add + GroupPNoAddr] <= MAXFILENO)) //判断每组花样的第一个花样序号是否为真
                    {
                        PatternList.dataAdd[j] = addB * GroupPatMax + add; //记录当前组合花样的首地址
                        j++;
                        if (((numA + j) >= PatternList.patternNum) || (j >= PatternList.disNumDiv))
                            break;
                    }
                }

                for (; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }
            else
            {
                for (j = 0; j < PatternList.disNumDiv; j++) //当前页的剩余文件地址写空
                    PatternList.dataAdd[j] = 0xffff;
            }

            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                PatternList.selectList[i] = 0;
            }
            if (PatternList.pageNow == PatternList.pageLast)
            {
                PatternList.selectList[PatternList.noInPageNow] = 1;
                comSum = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNumAddr];
                comPageMax = (comSum - 1) / comPageDiv + 1;
                comPageNow = 1;
            }
            dis_updata6 = 1;

            wdt();
        }
        // /[更新花样显示列表*end]**********************************************************

        // /[绘制组合花样按钮*begin]**********************************************************
        if (dis_updata6)
        {
            dis_updata6 = 0;
            //绘制按钮信息
            for (i = 0; i < PatternList.disNumDiv; i++)
            {
                BmpFillRect(keyd[i + KEY_FTREADCOM_SEL1].xpis, keyd[i + KEY_FTREADCOM_SEL1].ypis,
                            keyd[i + KEY_FTREADCOM_SEL1].xsize, keyd[i + KEY_FTREADCOM_SEL1].ysize);
                if (PatternList.dataAdd[i] < MAXGREG)
                {

                    DrawButtonSingleColor(keyd[i + KEY_FTREADCOM_SEL1],
                                          PatternList.selectList[i] ? keybmpind[i + KEY_FTREADCOM_SEL1].border[1] : keybmpind[i + KEY_FTREADCOM_SEL1].border[0],
                                          PatternList.selectList[i] ? keybmpind[i + KEY_FTREADCOM_SEL1].icon[1] : keybmpind[i + KEY_FTREADCOM_SEL1].icon[0]);
                    for (j = 0; j < 9; j++)
                        disname[j] = 0;
                    for (j = 0; j < 4; j++)
                    {
                        if (GREG[PatternList.dataAdd[i] + GroupPNameAddr + j] == 0)
                            break;
                        disname[2 * j] = (GREG[PatternList.dataAdd[i] + GroupPNameAddr + j]) & 0xff;
                        disname[2 * j + 1] = (GREG[PatternList.dataAdd[i] + GroupPNameAddr + j] >> 8) & 0xff;
                    }
                    Draw24X24FStr(keyd[i + KEY_FTREADCOM_SEL1].xpis + (keyd[i + KEY_FTREADCOM_SEL1].xsize - 13 * strlen((char *)disname)) / 2,
                                  keyd[i + KEY_FTREADCOM_SEL1].ypis + (keyd[i + KEY_FTREADCOM_SEL1].ysize - 24) / 2,
                                  (char *)disname,
                                  PatternList.selectList[i] ? keybmpind[i + KEY_FTREADCOM_SEL1].textColor[1] : keybmpind[i + KEY_FTREADCOM_SEL1].textColor[0]);
                    PatternList.selectEnable[i] = 1;
                }
                else
                {
                    PatternList.selectEnable[i] = 0;
                }
            }
        }
        // /[绘制组合花样按钮*end]**********************************************************

        // /[组合类型选中处理*begin]**********************************************************
        if (dis_updata2)
        {
            dis_updata2 = 0;
            for (i = KEY_FTREADCOM_COMBINE; i <= KEY_FTREADCOM_CONECT; i++)
            {
                f = (CombineType - 1 + KEY_FTREADCOM_COMBINE);
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                           (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
            }
        }
        // /[组合类型选中处理*end]**********************************************************

        // /[内存按键选中处理*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;
            if (g_GroupCover == 1)
            {
                BmpFillRect(rect[RECT_FTREADCOM_MOD].xpis, rect[RECT_FTREADCOM_MOD].ypis,
                            rect[RECT_FTREADCOM_MOD].xsize, rect[RECT_FTREADCOM_MOD].ysize);
            }
            else
            {
                DrawFillRect(rect[RECT_FTREADCOM_MOD].xpis, rect[RECT_FTREADCOM_MOD].ypis,
                             rect[RECT_FTREADCOM_MOD].xsize, rect[RECT_FTREADCOM_MOD].ysize, rect[RECT_FTREADCOM_MOD].colorB[0]);
                f = KEY_FTREADCOM_MEM + g_disk_memory;
                for (i = KEY_FTREADCOM_MEM; i <= KEY_FTREADCOM_SD; i++)
                {
                    if (i == KEY_FTREADCOM_USB)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (f == i) ? keybmpind[i].border[2] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[1]),
                                   (f == i) ? keybmpind[i].icon[2] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[1]));
                    }
                    else if (i == KEY_FTREADCOM_SD)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (f == i) ? keybmpind[i].border[2] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[1]),
                                   (f == i) ? keybmpind[i].icon[2] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[1]));
                    }
                    else
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                   (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                    }
                }
            }
        }
        // /[内存按键选中处理*end]**********************************************************

        // /[清除按键*begin]**********************************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;
            if (g_GroupCover == 1)
            {
                BmpFillRect(keyd[KEY_FTREADCOM_DEL].xpis, keyd[KEY_FTREADCOM_DEL].ypis, keyd[KEY_FTREADCOM_DEL].xsize, keyd[KEY_FTREADCOM_DEL].ysize);
            }
            else
            {
                DrawButton(keyd[KEY_FTREADCOM_DEL], (U16 *)(ADD_ICONBMP),
                           (PatternList.selectNoNow == 0) ? keybmpind[KEY_FTREADCOM_DEL].border[1] : keybmpind[KEY_FTREADCOM_DEL].border[0],
                           (PatternList.selectNoNow == 0) ? keybmpind[KEY_FTREADCOM_DEL].icon[1] : keybmpind[KEY_FTREADCOM_DEL].icon[0]);
            }
        }
        // /[清除按键*end]**********************************************************

        // /[绘制预览*begin]**********************************************************
        if (dis_updata5)
        {
            dis_updata5 = 0;
            DrawFillRect(keyd[KEY_FTREADCOM_IMGBAR].xpis, keyd[KEY_FTREADCOM_IMGBAR].ypis,
                         keyd[KEY_FTREADCOM_IMGBAR].xsize, keyd[KEY_FTREADCOM_IMGBAR].ysize, keybmpind[KEY_FTREADCOM_IMGBAR].icon[0]);
            DrawRect(keyd[KEY_FTREADCOM_IMGBAR].xpis, keyd[KEY_FTREADCOM_IMGBAR].ypis,
                     keyd[KEY_FTREADCOM_IMGBAR].xsize, keyd[KEY_FTREADCOM_IMGBAR].ysize, keybmpind[KEY_FTREADCOM_IMGBAR].border[0]);

            if (PatternList.pageLast == PatternList.pageNow)
            {
                if (PatternList.dataAdd[PatternList.noInPageNow] < MAXGREG)
                {
                    U32 tmpNo = 0;
                    U8 disNumSum = 0;
                    U8 disNumSt = 0, disNumEd = 0;
                    for (i = 0; i < comSum; i++)
                    {
                        tmpNo = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i]; //图形花样编号
                        if (tmpNo >= MINFILENO && tmpNo <= MAXFILENO)
                        {
                            disNumSum++;
                        }
                    }
                    disNumSt = (comPageNow - 1) * comPageDiv;
                    disNumEd = (comPageNow * comPageDiv > disNumSum) ? disNumSum : comPageNow * comPageDiv;
                    for (i = 0; i < disNumSum; i++)
                    {
                        if (i < disNumSt || i >= disNumEd)
                        {
                            continue;
                        }
                        tmpNo = GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i]; //图形花样编号
                        if (tmpNo >= MINFILENO && tmpNo <= MAXFILENO)
                        {
                            if ((IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - 1) * FileIndexSize + 1] == 0xffff) &&
                                (IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - 1) * FileIndexSize] == 0xffff))
                            { //内存里无该花样，文件名不现实
                                strcpy((char *)disname, "no data");
                            }
                            else
                            {
                                for (j = 0; j < 4; j++)
                                { //图形花样名称,只显示前面8个字符
                                    disname[2 * j] = IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j] & 0xff;
                                    disname[2 * j + 1] = (IREG2[(GREG[PatternList.dataAdd[PatternList.noInPageNow] + GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j] >> 8) & 0xff;
                                }
                                Chinesenum = 0;
                                for (j = 0; j < 8; j++)
                                {
                                    if (disname[j] & 0x80)
                                        Chinesenum++;
                                }
                                if (Chinesenum % 2 && j > 7)
                                { //最后一个字符为中文，显示不完全时不显示该字符
                                    disname[8] = 0;
                                }
                            }
                        }

                        f = i % comPageDiv;
                        DrawFillRect(rect[f * 2 + RECT_FTREADCOM_NUM1].xpis, rect[f * 2 + RECT_FTREADCOM_NUM1].ypis,
                                     rect[f * 2 + RECT_FTREADCOM_NUM1].xsize, rect[f * 2 + RECT_FTREADCOM_NUM1].ysize, rect[f * 2 + RECT_FTREADCOM_NUM1].colorB[0]);
                        DrawFillRect(rect[f * 2 + RECT_FTREADCOM_NAME1].xpis, rect[f * 2 + RECT_FTREADCOM_NAME1].ypis,
                                     rect[f * 2 + RECT_FTREADCOM_NAME1].xsize, rect[f * 2 + RECT_FTREADCOM_NAME1].ysize, rect[f * 2 + RECT_FTREADCOM_NAME1].colorB[0]);

                        DrawMFU32(rect[f * 2 + RECT_FTREADCOM_NUM1].xpis + (rect[f * 2 + RECT_FTREADCOM_NUM1].xsize - 3 * 13) / 2,
                                  rect[f * 2 + RECT_FTREADCOM_NUM1].ypis + (rect[f * 2 + RECT_FTREADCOM_NUM1].ysize - 24) / 2,
                                  tmpNo, 3, rect[f * 2 + RECT_FTREADCOM_NUM1].colorT[0]);
                        DrawFStr(rect[f * 2 + RECT_FTREADCOM_NAME1].xpis + (rect[f * 2 + RECT_FTREADCOM_NAME1].xsize - 13 * strlen((char *)disname)) / 2,
                                 rect[f * 2 + RECT_FTREADCOM_NAME1].ypis + (rect[f * 2 + RECT_FTREADCOM_NAME1].ysize - 16) / 2,
                                 (char *)disname, rect[f * 2 + RECT_FTREADCOM_NAME1].colorT[0]);

                        if (f != 0)
                        {
                            f += 1;
                            DrawFPBMPBTN(disBmp[f].xpis, disBmp[f].ypis, disBmp[f].xsize, disBmp[f].ysize, disBmp[f].icon[0], Color_White);
                        }
                    }
                }
            }
        }
        // /[绘制预览*end]**********************************************************
        ErrorMsg(MENU_FTGROUPREAD2);
        if (pno != MENU_FTGROUPREAD2)
        {
            plastno = MENU_FTGROUPREAD2;
            break;
        }
        wdt();
    }
}

//------------------------------------------------------------------------------
#define KEY_FTWRITECOM_SCRNSVR 1  //1 屏保
#define KEY_FTWRITECOM_BACK 2     //2 返回
#define KEY_FTWRITECOM_OK 3       //3 确认
#define KEY_FTWRITECOM_FLASH 4    //4 内存
#define KEY_FTWRITECOM_USB 5      //5 USB
#define KEY_FTWRITECOM_SD 6       //6 SD
#define KEY_FTWRITECOM_CAPACITY 7 //7 剩余容量
#define KEY_FTWRITECOM_DEL 8      //8 删除一位
#define KEY_FTWRITECOM_ABCPY 9    //9 输入切换
#define KEY_FTWRITECOM_0 10       //10 0
#define KEY_FTWRITECOM_1 11       //11 1
#define KEY_FTWRITECOM_2 12       //12 2
#define KEY_FTWRITECOM_3 13       //13 3
#define KEY_FTWRITECOM_4 14       //14 4
#define KEY_FTWRITECOM_5 15       //15 5
#define KEY_FTWRITECOM_6 16       //16 6
#define KEY_FTWRITECOM_7 17       //17 7
#define KEY_FTWRITECOM_8 18       //18 8
#define KEY_FTWRITECOM_9 19       //19 9
#define KEY_FTWRITECOM_A 20       //20 A
#define KEY_FTWRITECOM_B 21       //21 B
#define KEY_FTWRITECOM_C 22       //22 C
#define KEY_FTWRITECOM_D 23       //23 D
#define KEY_FTWRITECOM_E 24       //24 E
#define KEY_FTWRITECOM_F 25       //25 F
#define KEY_FTWRITECOM_G 26       //26 G
#define KEY_FTWRITECOM_H 27       //27 H
#define KEY_FTWRITECOM_I 28       //28 I
#define KEY_FTWRITECOM_J 29       //29 J
#define KEY_FTWRITECOM_K 30       //30 K
#define KEY_FTWRITECOM_L 31       //31 L
#define KEY_FTWRITECOM_M 32       //32 M
#define KEY_FTWRITECOM_N 33       //33 N
#define KEY_FTWRITECOM_O 34       //34 O
#define KEY_FTWRITECOM_P 35       //35 P
#define KEY_FTWRITECOM_Q 36       //36 Q
#define KEY_FTWRITECOM_R 37       //37 R
#define KEY_FTWRITECOM_S 38       //38 S
#define KEY_FTWRITECOM_T 39       //39 T
#define KEY_FTWRITECOM_U 40       //40 U
#define KEY_FTWRITECOM_V 41       //41 V
#define KEY_FTWRITECOM_W 42       //42 W
#define KEY_FTWRITECOM_X 43       //43 X
#define KEY_FTWRITECOM_Y 44       //44 Y
#define KEY_FTWRITECOM_Z 45       //45 Z
#define KEY_FTWRITECOM__ 46       //46 _
#define KEY_FTWRITECOM_NAME 47    //47 名称
#define KEY_FTWRITECOM_NUM 48     //48 数字
#define KEY_FTWRITECOM_SEL1 49    //49 中文选择 1
#define KEY_FTWRITECOM_SEL2 50    //50 中文选择 2
#define KEY_FTWRITECOM_SEL3 51    //51 中文选择 3
#define KEY_FTWRITECOM_SEL4 52    //52 中文选择 4
#define KEY_FTWRITECOM_SEL5 53    //53 中文选择 5
#define KEY_FTWRITECOM_SEL6 54    //54 中文选择 6
#define KEY_FTWRITECOM_SEL7 55    //55 中文选择 7
#define KEY_FTWRITECOM_SEL8 56    //56 中文选择 8
#define KEY_FTWRITECOM_SEL9 57    //57 中文选择 9
#define KEY_FTWRITECOM_SEL10 58   //58 中文选择 10
#define KEY_FTWRITECOM_SELPRES 59 //59 中文 复位
#define KEY_FTWRITECOM_SELPREV 60 //60 中文 前一页
#define KEY_FTWRITECOM_SELNEXT 61 //61 中文 后一页

#define KEY_FTWRITECOM_MAX 62

#define RECT_FTWRITECOM_NAME_TIT 1 //1 名称标题
#define RECT_FTWRITECOM_NUM_TIT 2  //2 序号标题
#define RECT_FTWRITECOM_MOD 3      //3 模组按键框

#define RECT_FTWRITECOM_MAX 4

#define FTWRITE_NAME_LENGTH 8
//------------------------------------------
// Function   : void Menu_FtWriteCombine(void)
// Description: 写存花样画面
//------------------------------------------
void Menu_FtWriteCombine(void)
{

    U32 i, f;

    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
    /****按键相关参数end****/

    /****花样序号，花样名称，花样列表相关参数begin****/
    //	U16 static tmpFno;																 //当前花样信息HREG[0]文件位置(即序号),花样序号缓存
    // U16 tmpselectdisno; //选中的花样序号缓存
    /****花样序号，花样名称，花样列表相关参数end****/

    /****标志位相关参数begin****/

    U8 SDFlag = 0, SDFlag1 = 0, USBFlag = 0, USBFlag1 = 0;

    U8 pinyinflag = 0; //0:大写字母  1：小写字母   2：中文  3-其他：刷新中文选择
    U8 pingyingbuf[7];
    U16 tmppysel[11];

    U8 CombineType = 1;
    U8 SetName[FTWRITE_NAME_LENGTH + 1] = {0};
    U8 nameidex = 0;

    // U8 dis_updata1 = 1; //dis_updata1:重绘保存类型按钮
    U8 refresh1 = 1, /*refresh2 = 0,*/ refresh3 = 1, refresh4 = 0, refresh5 = 0, refresh6 = 0;
    //refresh1 名称数字刷新,refresh2 重新显示A-Z,refresh3 刷新内存三按钮,refresh4 名称刷新,refresh5 数字刷新,refresh6 中文选框列表刷新
    /****标志位相关参数end***/

    const U8 keynum = {KEY_FTWRITECOM_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                                  //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                               //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                              //3 确认
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},                              //4 内存
        {TYPE_X10 - KEY_X1, TYPE_Y4, KEY_X1, KEY_Y1},                     //5 USB
        {TYPE_X10 - KEY_X1 * 2, TYPE_Y4, KEY_X1, KEY_Y1},                 //6 SD
        {TYPE_X10 - KEY_X1 * 3 - SAFE_DIST1, TYPE_Y4, KEY_X1, KEY_Y1},    //7 剩余容量
        {TYPE_X23, TYPE_Y19, KEY_X5, KEY_Y5},                             //8 删除一位
        {TYPE_X24, TYPE_Y19, KEY_X5, KEY_Y5},                             //9 输入切换
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y18, KEY_X2, KEY_Y2}, //10 0
        {TYPE_X16, TYPE_Y18, KEY_X2, KEY_Y2},                             //11 1
        {TYPE_X16 + KEY_X2 + SAFE_DIST2, TYPE_Y18, KEY_X2, KEY_Y2},       //12 2
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y18, KEY_X2, KEY_Y2}, //13 3
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y18, KEY_X2, KEY_Y2}, //14 4
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y18, KEY_X2, KEY_Y2}, //15 5
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y18, KEY_X2, KEY_Y2}, //16 6
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y18, KEY_X2, KEY_Y2}, //17 7
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y18, KEY_X2, KEY_Y2}, //18 8
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y18, KEY_X2, KEY_Y2}, //19 9

        {TYPE_X20, TYPE_Y21, KEY_X2, KEY_Y2},                             //20 A
        {TYPE_X21 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y22, KEY_X2, KEY_Y2}, //21 B
        {TYPE_X21 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y22, KEY_X2, KEY_Y2}, //22 C
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y21, KEY_X2, KEY_Y2}, //23 D
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y20, KEY_X2, KEY_Y2}, //24 E
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y21, KEY_X2, KEY_Y2}, //25 F
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y21, KEY_X2, KEY_Y2}, //26 G
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y21, KEY_X2, KEY_Y2}, //27 H
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y20, KEY_X2, KEY_Y2}, //28 I
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y21, KEY_X2, KEY_Y2}, //29 J
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y21, KEY_X2, KEY_Y2}, //30 K
        {TYPE_X20 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y21, KEY_X2, KEY_Y2}, //31 L
        {TYPE_X21 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y22, KEY_X2, KEY_Y2}, //32 M
        {TYPE_X21 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y22, KEY_X2, KEY_Y2}, //33 N
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y20, KEY_X2, KEY_Y2}, //34 O
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y20, KEY_X2, KEY_Y2}, //35 P
        {TYPE_X16, TYPE_Y20, KEY_X2, KEY_Y2},                             //36 Q
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y20, KEY_X2, KEY_Y2}, //37 R
        {TYPE_X20 + KEY_X2 + SAFE_DIST2, TYPE_Y21, KEY_X2, KEY_Y2},       //38 S
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y20, KEY_X2, KEY_Y2}, //39 T
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y20, KEY_X2, KEY_Y2}, //40 U
        {TYPE_X21 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y22, KEY_X2, KEY_Y2}, //41 V
        {TYPE_X16 + KEY_X2 + SAFE_DIST2, TYPE_Y20, KEY_X2, KEY_Y2},       //42 W
        {TYPE_X21 + KEY_X2 + SAFE_DIST2, TYPE_Y22, KEY_X2, KEY_Y2},       //43 X
        {TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y20, KEY_X2, KEY_Y2}, //44 Y
        {TYPE_X21, TYPE_Y22, KEY_X2, KEY_Y2},                             //45 Z
        {TYPE_X16, TYPE_Y19, KEY_X2, KEY_Y2},                             //46 _

        {TYPE_X22, TYPE_Y2, TYPE_SIZE23, TYPE_SIZE5},  //47 名称
        {TYPE_X22, TYPE_Y16, TYPE_SIZE23, TYPE_SIZE5}, //48 数字

        {TYPE_X22, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},                                   //49 中文选择 1
        {TYPE_X22 + TYPE_SIZE27 + SAFE_DIST2, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},        //50 中文选择 2
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 2, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //51 中文选择 3
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 3, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //52 中文选择 4
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 4, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //53 中文选择 5
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 5, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //54 中文选择 6
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 6, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //55 中文选择 7
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 7, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //56 中文选择 8
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 8, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //57 中文选择 9
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 9, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},  //58 中文选择 10
        {TYPE_X16, TYPE_Y17, TYPE_SIZE11, TYPE_SIZE7},                                   //59 中文 复位
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 10, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7}, //60 中文 前一页
        {TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 11, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7}, //61 中文 后一页
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_WRITECOM, 0, 0, 0}, {0}},

        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_WRITECOM + 1, 0, 0, 0}, {0}},                                                             //1 屏保
        {{KEY_Silver}, {0}, {72, 88}, {TLFNADD_FUN_WRITECOM + 2, 0, 0, 0}, {0}},                                                        //2 返回
        {{KEY_Silver}, {0}, {71, 87}, {TLFNADD_FUN_WRITECOM + 3, 0, 0, 0}, {0}},                                                        //3 确认
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {257, 258, 259}, {TLFNADD_FUN_WRITECOM + 4, 0, 0, 0}, {0}},                             //4 内存
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {251, 252, 253}, {TLFNADD_FUN_WRITECOM + 5, 0, 0, 0}, {0}},                             //5 USB
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {254, 255, 256}, {TLFNADD_FUN_WRITECOM + 6, 0, 0, 0}, {0}},                             //6 SD
        {{KEY_Silver}, {0}, {73}, {TLFNADD_FUN_WRITECOM, 0, 0, 0}, {0}},                                                                //7 剩余容量
        {{KEY_Silver}, {0}, {189}, {TLFNADD_FUN_WRITECOM + 8, 0, 0, 0}, {0}},                                                           //8 删除一位
        {{KEY_Silver}, {0}, {147, 148, 149}, {TLFNADD_FUN_WRITECOM + 69, TLFNADD_FUN_WRITECOM + 9, TLFNADD_FUN_WRITECOM + 62, 0}, {0}}, //9 输入切换
        {{KEY_Silver, KEY_Gray}, {0}, {91, 101}, {TLFNADD_FUN_WRITECOM + 10, 0, 0, 0}, {0}},                                            //10 0
        {{KEY_Silver, KEY_Gray}, {0}, {92, 102}, {TLFNADD_FUN_WRITECOM + 11, 0, 0, 0}, {0}},                                            //11 1
        {{KEY_Silver, KEY_Gray}, {0}, {93, 103}, {TLFNADD_FUN_WRITECOM + 12, 0, 0, 0}, {0}},                                            //12 2
        {{KEY_Silver, KEY_Gray}, {0}, {94, 104}, {TLFNADD_FUN_WRITECOM + 13, 0, 0, 0}, {0}},                                            //13 3
        {{KEY_Silver, KEY_Gray}, {0}, {95, 105}, {TLFNADD_FUN_WRITECOM + 14, 0, 0, 0}, {0}},                                            //14 4
        {{KEY_Silver, KEY_Gray}, {0}, {96, 106}, {TLFNADD_FUN_WRITECOM + 15, 0, 0, 0}, {0}},                                            //15 5
        {{KEY_Silver, KEY_Gray}, {0}, {97, 107}, {TLFNADD_FUN_WRITECOM + 16, 0, 0, 0}, {0}},                                            //16 6
        {{KEY_Silver, KEY_Gray}, {0}, {98, 108}, {TLFNADD_FUN_WRITECOM + 17, 0, 0, 0}, {0}},                                            //17 7
        {{KEY_Silver, KEY_Gray}, {0}, {99, 109}, {TLFNADD_FUN_WRITECOM + 18, 0, 0, 0}, {0}},                                            //18 8
        {{KEY_Silver, KEY_Gray}, {0}, {100, 110}, {TLFNADD_FUN_WRITECOM + 19, 0, 0, 0}, {0}},                                           //19 9

        {{KEY_Silver}, {0}, {121}, {TLFNADD_FUN_WRITECOM + 20, 0, 0, 0}, {0}}, //20 A
        {{KEY_Silver}, {0}, {122}, {TLFNADD_FUN_WRITECOM + 21, 0, 0, 0}, {0}}, //21 B
        {{KEY_Silver}, {0}, {123}, {TLFNADD_FUN_WRITECOM + 22, 0, 0, 0}, {0}}, //22 C
        {{KEY_Silver}, {0}, {124}, {TLFNADD_FUN_WRITECOM + 23, 0, 0, 0}, {0}}, //23 D
        {{KEY_Silver}, {0}, {125}, {TLFNADD_FUN_WRITECOM + 24, 0, 0, 0}, {0}}, //24 E
        {{KEY_Silver}, {0}, {126}, {TLFNADD_FUN_WRITECOM + 25, 0, 0, 0}, {0}}, //25 F
        {{KEY_Silver}, {0}, {127}, {TLFNADD_FUN_WRITECOM + 26, 0, 0, 0}, {0}}, //26 G
        {{KEY_Silver}, {0}, {128}, {TLFNADD_FUN_WRITECOM + 27, 0, 0, 0}, {0}}, //27 H
        {{KEY_Silver}, {0}, {129}, {TLFNADD_FUN_WRITECOM + 28, 0, 0, 0}, {0}}, //28 I
        {{KEY_Silver}, {0}, {130}, {TLFNADD_FUN_WRITECOM + 29, 0, 0, 0}, {0}}, //29 J
        {{KEY_Silver}, {0}, {131}, {TLFNADD_FUN_WRITECOM + 30, 0, 0, 0}, {0}}, //30 K
        {{KEY_Silver}, {0}, {132}, {TLFNADD_FUN_WRITECOM + 31, 0, 0, 0}, {0}}, //31 L
        {{KEY_Silver}, {0}, {133}, {TLFNADD_FUN_WRITECOM + 32, 0, 0, 0}, {0}}, //32 M
        {{KEY_Silver}, {0}, {134}, {TLFNADD_FUN_WRITECOM + 33, 0, 0, 0}, {0}}, //33 N
        {{KEY_Silver}, {0}, {135}, {TLFNADD_FUN_WRITECOM + 34, 0, 0, 0}, {0}}, //34 O
        {{KEY_Silver}, {0}, {136}, {TLFNADD_FUN_WRITECOM + 35, 0, 0, 0}, {0}}, //35 P
        {{KEY_Silver}, {0}, {137}, {TLFNADD_FUN_WRITECOM + 36, 0, 0, 0}, {0}}, //36 Q
        {{KEY_Silver}, {0}, {138}, {TLFNADD_FUN_WRITECOM + 37, 0, 0, 0}, {0}}, //37 R
        {{KEY_Silver}, {0}, {139}, {TLFNADD_FUN_WRITECOM + 38, 0, 0, 0}, {0}}, //38 S
        {{KEY_Silver}, {0}, {140}, {TLFNADD_FUN_WRITECOM + 39, 0, 0, 0}, {0}}, //39 T
        {{KEY_Silver}, {0}, {141}, {TLFNADD_FUN_WRITECOM + 40, 0, 0, 0}, {0}}, //40 U
        {{KEY_Silver}, {0}, {142}, {TLFNADD_FUN_WRITECOM + 41, 0, 0, 0}, {0}}, //41 V
        {{KEY_Silver}, {0}, {143}, {TLFNADD_FUN_WRITECOM + 42, 0, 0, 0}, {0}}, //42 W
        {{KEY_Silver}, {0}, {144}, {TLFNADD_FUN_WRITECOM + 43, 0, 0, 0}, {0}}, //43 X
        {{KEY_Silver}, {0}, {145}, {TLFNADD_FUN_WRITECOM + 44, 0, 0, 0}, {0}}, //44 Y
        {{KEY_Silver}, {0}, {146}, {TLFNADD_FUN_WRITECOM + 45, 0, 0, 0}, {0}}, //45 Z
        {{KEY_Silver}, {0}, {173}, {TLFNADD_FUN_WRITECOM + 46, 0, 0, 0}, {0}}, //46 _

        {{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {TLFNADD_FUN_WRITECOM + 48}, {0}}, //47 名称
        {{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue}, {TLFNADD_FUN_WRITECOM}, {0}},      //48 数字

        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //49 中文选择 1
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //50 中文选择 2
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //51 中文选择 3
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //52 中文选择 4
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //53 中文选择 5
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //54 中文选择 6
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //55 中文选择 7
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //56 中文选择 8
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //57 中文选择 9
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //58 中文选择 10
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //59 中文 复位
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //60 中文 前一页
        {{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {TLFNADD_FUN_WRITECOM}, {0}}, //61 中文 后一页
    };

    const RECTPAD rect[RECT_FTWRITECOM_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {TLFNADD_FUN_WRITECOM}},

        {TYPE_X16, TYPE_Y2, TYPE_SIZE11, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_WRITECOM + 48}},                                  //1 名称标题
        {TYPE_X16, TYPE_Y16, TYPE_SIZE11, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_WRITECOM}},                                      //2 序号标题
        {TYPE_X10 - KEY_X1 * 2 - SAFE_DIST4, TYPE_Y4 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //3 模组按键框

    };

    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_FUNCTION); //设置当前文字显示雷人在P1页面，对应41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[显示相关标志位信息读取*begin]**********************************************************
    g_namenumberflag = 0;
    strcopy((U8 *)g_GroupName, SetName, 8);
    CombineType = g_GroupType;
    while (SetName[nameidex] != 0)
    {
        nameidex++;
    }

    // /[显示相关标志位信息读取*end]**********************************************************

    // /[绘制初始显示页面*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //写存模式
    //画出方框
    for (i = 1; i < RECT_FTWRITECOM_MAX; i++)
    {
        if (i == RECT_FTWRITECOM_NUM_TIT)
        {
        }
        else
        {

            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_FTWRITECOM_NAME_TIT)
            {
                Draw24x24Str(rect[i].xpis + (rect[i].xsize - 24 * 2) / 2,
                             rect[i].ypis + SAFE_DIST4,
                             MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
                             SheetPage, rect[i].index[0], rect[i].colorT[0]);
            }
        }
    }
    //画出按键显示
    for (i = 2; i < KEY_FTWRITECOM_MAX; i++)
    {
        //无效按键
        if (i >= KEY_FTWRITECOM_SEL1 && i <= KEY_FTWRITECOM_SELNEXT) //不需要显示的按键
        {
            continue;
        }
        else if (i == KEY_FTWRITECOM_NUM)
        {
        }
        else if (i == KEY_FTWRITECOM_NAME) //非常规按钮
        {
            DrawButton3(keyd[i], keybmpind[i].icon[0], 0);
        }
        else if (i == KEY_FTWRITECOM_CAPACITY)
        {
        }
        else if (i == KEY_FTWRITECOM_ABCPY)
        {
        }
        else //常规按钮
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[绘制初始显示页面*end]**********************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************
    if (ExtMemory_Status(USBTF) == 0)
    {
        USBFlag = 1;
        USBFlag1 = 1;
    }
    g_disk_memory = 0;   //默认主板内存
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
        if (keydata >= KEY_FTWRITECOM_SEL1 && keydata <= KEY_FTWRITECOM_SELNEXT && (pinyinflag <= 2))
        {
            keydata = 0;
        }
        else if (keydata == KEY_FTWRITECOM_NAME || keydata == KEY_FTWRITECOM_NAME)
        {
            keydata = 0;
        }
        else if (keydata == KEY_FTWRITECOM_ABCPY)
        {
            keydata = 0;
        }
        else if (keydata == KEY_FTWRITECOM_CAPACITY)
        {
            keydata = 0;
        }

        if (keydata == 0xff) //按键松开后的键值预处理
        {
            //带有连续按键功能的按键
            if (keyon == KEY_FTWRITECOM_DEL)
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
            if ((g_disk_memory != 0) && (keydata == KEY_FTWRITECOM_CAPACITY))
            {
                keydata = 0;
            }
            else if (((keydata == KEY_FTWRITECOM_USB) && (USBFlag == 0)) || ((keydata == KEY_FTWRITECOM_SD) && (SDFlag == 0)))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时)
                {
                    keyon = keydata;
                    if (keyon == KEY_FTWRITECOM_SCRNSVR) //无按键特效
                    {
                    }
                    else if (keyon >= KEY_FTWRITECOM_SEL1 && keyon <= KEY_FTWRITECOM_SELNEXT)
                    {
                        DrawFillRect(keyd[keyon].xpis, keyd[keyon].ypis,
                                     keyd[keyon].xsize, keyd[keyon].ysize, keybmpind[keyon].icon[1]);
                        if (keyon == KEY_FTWRITECOM_SELPRES)
                        {
                            Draw24X24FStr(keyd[keyon].xpis + SAFE_DIST3, keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
                                          (char *)(pingyingbuf), keybmpind[keyon].textColor[2]);
                        }
                        else if (keyon == KEY_FTWRITECOM_SELPREV)
                        {
                            Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
                                          keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
                                          "<", keybmpind[keyon].textColor[2]);
                        }
                        else if (keyon == KEY_FTWRITECOM_SELNEXT)
                        {

                            Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
                                          keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
                                          ">", keybmpind[keyon].textColor[2]);
                        }
                        else if ((keyon >= KEY_FTWRITECOM_SEL1) && (keyon <= KEY_FTWRITECOM_SEL10))
                        {
                            Draw24X24FPY(keyd[keyon].xpis + (keyd[keyon].xsize - 24) / 2,
                                         keyd[keyon].ypis + (keyd[keyon].ysize - 24) / 2,
                                         (char *)(&tmppysel[keyon - KEY_FTWRITECOM_SEL1]), 2, keybmpind[keyon].textColor[2]);
                        }
                    }
                    else if (keyon == KEY_FTWRITECOM_NUM || keyon == KEY_FTWRITECOM_NAME) //非常规按键
                    {
                        DrawButtonOnZoomIn(keyd[keyon], 2);
                    }
                    else
                    {
                        DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                    }

                    if ((keyon >= KEY_FTWRITECOM_0 && keyon <= KEY_FTWRITECOM_9) && pinyinflag > 2)
                    {
                        DrawFillRect(keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xpis, keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ypis,
                                     keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xsize, keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ysize, keybmpind[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].icon[1]);

                        Draw24X24FPY(keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xpis + (keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xsize - 24) / 2,
                                     keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ypis + (keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ysize - 24) / 2,
                                     (char *)(&tmppysel[keyon - KEY_FTWRITECOM_0]), 2, keybmpind[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].textColor[2]);
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
                        case KEY_FTWRITECOM_ABCPY:
                            if (pinyinflag == 0)
                            {
                                DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                            }
                            else if (pinyinflag == 1)
                            {
                                DrawFMenuStr(keybmpind[keyon].titleStr[1]);
                            }
                            else if (pinyinflag == 2)
                            {
                                DrawFMenuStr(keybmpind[keyon].titleStr[2]);
                            }
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

                        if (keyon == KEY_FTWRITECOM_DEL) //带有长按连续功能按键的功能
                        {
                            delay = KEYLT;
                        }
                        else
                        {
                            delay = KEYNT;
                        }
                    }
                    else if (beeponf < 20)
                    {
                        beeponf++;
                    }
                    if (keyon == KEY_FTWRITECOM_DEL) //带有长按连续功能按键的功能
                    {
                        keyoff = keyon;
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
            if ((keyoff >= KEY_FTWRITECOM_0) && (keyoff <= KEY_FTWRITECOM_9))
            { //10 - 19  数字键
                if (nameidex < FTWRITE_NAME_LENGTH)
                {
                    SetName[nameidex] = keyoff - KEY_FTWRITECOM_0 + 0x30;
                    nameidex++;
                    refresh1 = 1;
                }
            }
            else if ((keyoff >= KEY_FTWRITECOM_A) && (keyoff <= KEY_FTWRITECOM__))
            { // 20 -16  字母
                if (nameidex < FTWRITE_NAME_LENGTH)
                {
                    if (keyoff == KEY_FTWRITECOM__)
                    {
                        SetName[nameidex] = 0x5f;
                    }
                    else
                    {
                        SetName[nameidex] = keyoff - KEY_FTWRITECOM_A + 0x41;
                    }
                    nameidex++;
                    refresh1 = 1;
                }
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTWRITECOM_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;

                case KEY_FTWRITECOM_BACK:
                    pnoindex--;
                    pno = poldno[pnoindex];
                    break;

                case KEY_FTWRITECOM_OK:
                    if (nameidex == 0) //无输入文件名
                    {
                        pno = MENU_RWHELP;
                        hleppara = 100;
                        poldno[pnoindex] = MENU_FTGROUPWRITE;
                        pnoindex++;
                    }
                    else
                    {
                        strcopy(SetName, (U8 *)g_GroupName, 8);
                        g_GroupSave = g_disk_memory + 1;
                        g_GroupMenth = 1; //标记为保存类型
                        g_GroupType = CombineType;

                        if (g_GroupSave == 1)
                        {
                            CalGroSaveNo((U8 *)&g_GroupDoingNO, g_GroupType); //计算保存编号
                            if (g_GroupDoingNO > GroupMax)                    //编号大于20,要覆盖原来组合文件
                            {
                                g_GroupCover = 1;
                                pno = MENU_FTGROUPREAD2;
                                poldno[pnoindex] = MENU_FTGROUPWRITE;
                                pnoindex++;
                                break;
                            }
                            else
                            {
                                pnoindex--;
                                pno = poldno[pnoindex];
                            }
                        }
                        else //保存到U盘 ,SD卡
                        {
                            //修改
                            // pno = MENU_RWHELP;
                            // hleppara = 109;
                            // poldno[pnoindex] = MENU_FTGROUPWRITE;
                            // pnoindex++;
                        }
                        //修改
                        if (g_GroupMenth == 1)
                        {
                            g_GroupMenth = 0;
                            for (i = 0; i < GroupPatMax; i++) //清0
                            {
                                GREG[i] = 0;
                            }

                            for (i = 0; i < GroupMax; i++)
                            {
                                if (g_Combinepat[i][0] == 0xffff)
                                    break;
                                GREG[i] = g_Combinepat[i][0]; //图形花样编号
                            }

                            GREG[GroupPNameAddr] = (g_GroupName[1] << 8) | g_GroupName[0];
                            GREG[GroupPNameAddr + 1] = (g_GroupName[3] << 8) | g_GroupName[2];
                            GREG[GroupPNameAddr + 2] = (g_GroupName[5] << 8) | g_GroupName[4];
                            GREG[GroupPNameAddr + 3] = (g_GroupName[7] << 8) | g_GroupName[6];
                            GREG[GroupPNumAddr] = i;
                            GREG[GroupPTypeAddr] = g_GroupType;
                            if (g_GroupSave == 1) //保存到内存
                            {
                                if (Write_D(D_Address[GROUP], GroupPatMax, (U16 *)(&GREG[0])) == 0)
                                { //发送参数
                                    pno = MENU_RWHELP;
                                    hleppara = 16;
                                }

                                if (Write_Cmd(C_SAVEGROUP, g_GroupType, g_GroupDoingNO) == 0) //发送保存组合文件指令
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 16;
                                }
                                if (Read_Status() == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                }
                                else if (DREG[D_CMDSTATUS] != 100)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                }

                                if (g_GroupCover == 1)
                                {
                                    g_GroupCover = 0;
                                }
                            }
                            else if ((g_GroupSave == 2) || (g_GroupSave == 3)) //保存到磁盘
                            {
                                pno = MENU_RWHELP;
                                hleppara = 110;
                                pnoindex--;
                                // poldno[pnoindex] = MENU_FTGROUPWRITE;
                                // pnoindex++;
                            }
                        }
                    }
                    break;

                case KEY_FTWRITECOM_FLASH:
                    g_disk_memory = 0;
                    refresh3 = 1;
                    break;

                case KEY_FTWRITECOM_USB:
                    if (ExtMemory_Status(USBTF) != 0)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 95;
                        poldno[pnoindex] = MENU_FTGROUPWRITE;
                        pnoindex++;
                        break;
                    }
                    g_disk_memory = 1;
                    refresh3 = 1;
                    break;

                case KEY_FTWRITECOM_SD:
                    break;

                case KEY_FTWRITECOM_DEL:
                    if (nameidex > 0)
                    {
                        SetName[nameidex - 1] = 0;
                        nameidex--;
                        refresh1 = 1;
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
            if (keyon == KEY_FTWRITECOM_SCRNSVR) //无按键特效
            {
            }
            else if (keyon >= KEY_FTWRITECOM_SEL1 && keyon <= KEY_FTWRITECOM_SELNEXT)
            {
                DrawFillRect(keyd[keyon].xpis, keyd[keyon].ypis,
                             keyd[keyon].xsize, keyd[keyon].ysize, keybmpind[keyon].icon[0]);
                if (keyon == KEY_FTWRITECOM_SELPRES)
                {
                    Draw24X24FStr(keyd[keyon].xpis + SAFE_DIST3, keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
                                  (char *)(pingyingbuf), keybmpind[keyon].textColor[0]);
                }
                else if (keyon == KEY_FTWRITECOM_SELPREV)
                {
                    Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
                                  keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
                                  "<", keybmpind[keyon].textColor[0]);
                }
                else if (keyon == KEY_FTWRITECOM_SELNEXT)
                {

                    Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
                                  keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
                                  ">", keybmpind[keyon].textColor[0]);
                }
                else if ((keyon >= KEY_FTWRITECOM_SEL1) && (keyon <= KEY_FTWRITECOM_SEL10))
                {
                    Draw24X24FPY(keyd[keyon].xpis + (keyd[keyon].xsize - 24) / 2,
                                 keyd[keyon].ypis + SAFE_DIST3,
                                 (char *)(&tmppysel[keyon - KEY_FTWRITECOM_SEL1]), 2, keybmpind[keyon].textColor[1]);
                    DrawFU32(keyd[keyon].xpis + (keyd[keyon].xsize - 8) / 2,
                             keyd[keyon].ypis + keyd[keyon].ysize - 16 + SAFE_DIST2,
                             (keyon - KEY_FTWRITECOM_SEL1), 1, keybmpind[keyon].textColor[0]);
                }
            }
            else if (keyon == KEY_FTWRITECOM_NUM || keyon == KEY_FTWRITECOM_NAME) //非常规按键
            {
                DrawButtonOnZoomInRes(keyd[keyon]);
            }
            else
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            if ((keyon >= KEY_FTWRITECOM_0 && keyon <= KEY_FTWRITECOM_9) && pinyinflag > 2)
            {
                DrawFillRect(keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xpis, keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ypis,
                             keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xsize, keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ysize, keybmpind[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].icon[0]);

                Draw24X24FPY(keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xpis + (keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xsize - 24) / 2,
                             keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ypis + SAFE_DIST2,
                             (char *)(&tmppysel[keyon - KEY_FTWRITECOM_0]), 2, keybmpind[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].textColor[1]);
                DrawFU32(keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xpis + (keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].xsize - 24) / 2,
                         keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ypis + keyd[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].ysize - 12 + SAFE_DIST2,
                         (keyon - KEY_FTWRITECOM_0), 1, keybmpind[keyon + KEY_FTWRITECOM_SEL1 - KEY_FTWRITECOM_0].textColor[0]);
            }

            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //显示回写存模式
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
            if (g_disk_memory == 1)
            { //使用U盘
                if (ExtMemory_Status(USBTF) != 0)
                {
                    USBFlag = 0;
                    pno = MENU_RWHELP;
                    hleppara = 95;
                    poldno[pnoindex] = MENU_FTGROUPWRITE;
                    pnoindex++;
                    break;
                }
                else
                {
                    USBFlag = 1;
                }
            }
            else if (g_disk_memory == 2)
            { // 使用打板器SD
            }
            else
            { //使用主板内存
                //查询一下U盘插入情况
                if (ExtMemory_Status(USBTF) == 0)
                {
                    USBFlag = 1;
                }
                else
                {
                    USBFlag = 0;
                }
            }
            if (SDFlag != SDFlag1)
            {
                SDFlag1 = SDFlag;
                refresh3 = 1;
            }
            if (USBFlag != USBFlag1)
            {
                USBFlag1 = USBFlag;
                refresh3 = 1;
            }
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[内存按键处理*begin]**********************************************************
        if (refresh3)
        {
            U8 chSelectMem = 0;
            refresh3 = 0;
            chSelectMem = KEY_FTWRITECOM_FLASH + g_disk_memory;

            for (i = KEY_FTWRITECOM_FLASH; i <= KEY_FTWRITECOM_SD; i++)
            {
                if (i == KEY_FTWRITECOM_USB)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
                               i == chSelectMem ? keybmpind[i].icon[1] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
                else if (i == KEY_FTWRITECOM_SD)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
                               i == chSelectMem ? keybmpind[i].icon[1] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
                else if (i == KEY_FTWRITECOM_FLASH)
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               i == chSelectMem ? keybmpind[i].border[1] : keybmpind[i].border[0],
                               i == chSelectMem ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
            }
            refresh4 = 1;
            refresh5 = 0;
        }
        // /[内存按键处理*end]**********************************************************

        // /[名称数字按钮刷新*begin]**********************************************************
        if (refresh1)
        {
            refresh1 = 0;
            refresh4 = 1;
            refresh5 = 0;
        }
        if (refresh4)
        {
            refresh4 = 0;
            f = (g_namenumberflag + KEY_FTWRITECOM_NAME);
            DrawButtonEdit(keyd[KEY_FTWRITECOM_NAME],
                           (KEY_FTWRITECOM_NAME == f) ? keybmpind[KEY_FTWRITECOM_NAME].textColor[1] : keybmpind[KEY_FTWRITECOM_NAME].icon[0],
                           (KEY_FTWRITECOM_NAME == f) ? keybmpind[KEY_FTWRITECOM_NAME].icon[1] : keybmpind[KEY_FTWRITECOM_NAME].icon[0]);

            // DrawFFilename(keyd[KEY_FTWRITECOM_NAME].xpis + SAFE_DIST4, keyd[KEY_FTWRITECOM_NAME].ypis + SAFE_DIST4,
            // 			  (char *)&g_patname, 64, keybmpind[KEY_FTWRITECOM_NAME].textColor[0], 20, 0, NULL);
            Draw24X24FStr(keyd[KEY_FTWRITECOM_NAME].xpis + SAFE_DIST4, keyd[KEY_FTWRITECOM_NAME].ypis + SAFE_DIST4,
                          (char *)SetName, keybmpind[KEY_FTWRITECOM_NAME].textColor[0]);
        }
        if (refresh5)
        {
            refresh5 = 0;
            f = (g_namenumberflag + KEY_FTWRITECOM_NAME);
            DrawButtonEdit(keyd[KEY_FTWRITECOM_NAME],
                           (KEY_FTWRITECOM_NUM == f) ? keybmpind[KEY_FTWRITECOM_NUM].textColor[1] : keybmpind[KEY_FTWRITECOM_NUM].icon[0],
                           (KEY_FTWRITECOM_NUM == f) ? keybmpind[KEY_FTWRITECOM_NUM].icon[1] : keybmpind[KEY_FTWRITECOM_NUM].icon[0]);
            // DrawFASC12(keyd[KEY_FTWRITECOM_NUM].xpis + SAFE_DIST4, keyd[KEY_FTWRITECOM_NUM].ypis + SAFE_DIST4,
            // 		   (char *)g_patnumber, keybmpind[KEY_FTWRITECOM_NUM].textColor[0]);
        }
        // /[名称数字按钮刷新*end]**********************************************************

        // /[汉字选择标识*begin]**********************************************************
        if (refresh6)
        {
            refresh6 = 0;

            if (pinyinflag > 2)
            {

                BmpFillRect(keyd[KEY_FTWRITECOM_SELPRES].xpis, keyd[KEY_FTWRITECOM_SELPRES].ypis,
                            keyd[KEY_FTWRITECOM_SELNEXT].xpis + keyd[KEY_FTWRITECOM_SELNEXT].xsize - keyd[KEY_FTWRITECOM_SELPRES].xpis, keyd[KEY_FTWRITECOM_SELPRES].ysize);
                for (i = KEY_FTWRITECOM_SEL1; i <= KEY_FTWRITECOM_SELNEXT; i++)
                {
                    DrawFillRect(keyd[i].xpis, keyd[i].ypis,
                                 keyd[i].xsize, keyd[i].ysize, keybmpind[i].icon[0]);
                    if (i == KEY_FTWRITECOM_SELPRES)
                    {
                        Draw24X24FStr(keyd[i].xpis + SAFE_DIST3, keyd[i].ypis + (keyd[i].ysize - 12) / 2,
                                      (char *)(pingyingbuf), keybmpind[i].textColor[0]);
                    }
                    else if (i == KEY_FTWRITECOM_SELPREV)
                    {
                        Draw24X24FStr(keyd[i].xpis + (keyd[i].xsize - 12) / 2,
                                      keyd[i].ypis + (keyd[i].ysize - 12) / 2,
                                      "<", keybmpind[i].textColor[0]);
                    }
                    else if (i == KEY_FTWRITECOM_SELNEXT)
                    {

                        Draw24X24FStr(keyd[i].xpis + (keyd[i].xsize - 12) / 2,
                                      keyd[i].ypis + (keyd[i].ysize - 12) / 2,
                                      ">", keybmpind[i].textColor[0]);
                    }
                    else if ((i >= KEY_FTWRITECOM_SEL1) && (i <= KEY_FTWRITECOM_SEL10))
                    {
                        Draw24X24FPY(keyd[i].xpis + (keyd[i].xsize - 24) / 2,
                                     keyd[i].ypis + SAFE_DIST3,
                                     (char *)(&tmppysel[i - KEY_FTWRITECOM_SEL1]), 2, keybmpind[i].textColor[1]);
                        DrawFU32(keyd[i].xpis + (keyd[i].xsize - 8) / 2,
                                 keyd[i].ypis + keyd[i].ysize - 16 + SAFE_DIST2,
                                 (i - KEY_FTWRITECOM_SEL1), 1, keybmpind[i].textColor[0]);
                    }
                }
            }
            else
            {
                BmpFillRect(keyd[KEY_FTWRITECOM_SELPRES].xpis, keyd[KEY_FTWRITECOM_SELPRES].ypis,
                            keyd[KEY_FTWRITECOM_SELNEXT].xpis + keyd[KEY_FTWRITECOM_SELNEXT].xsize - keyd[KEY_FTWRITECOM_SELPRES].xpis, keyd[KEY_FTWRITECOM_SELPRES].ysize);
                refresh5 = 0;
            }
        }
        // /[汉字选择标识*end]**********************************************************

        // /[连续功能按键信息处理时，及时检测报警信息*begin]**********************************************************
        if (key_on_flag1)
            ErrorMsg(MENU_FTGROUPWRITE);
        // /[连续功能按键信息处理时，及时检测报警信息*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_FTGROUPWRITE);
            if (Timer_ms >= 30000) //30720
            {
                Timer_ms = 0;
            }
        }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************

        // /[页面切换*begin]**********************************************************

        if (pno != MENU_FTGROUPWRITE)
        {
            plastno = MENU_FTGROUPWRITE;
            break;
        }

        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

//---------------------------------------------------------
#define KEY_FTUSER_SCRNSVR 1    //1 屏保
#define KEY_FTUSER_BACK 2       //2 返回
#define KEY_FTUSER_ENTER 3      //3 确认
#define KEY_FTUSER_QUIT 4       //4 退出登录
#define KEY_FTUSER_EDIT 5       //修改密码
#define KEY_FTUSER_OPERATOR 6   //操作员
#define KEY_FTUSER_MANAGER 7    //管理员
#define KEY_FTUSER_MANUFACTOR 8 //设备商
#define KEY_FTUSER_AUTHORIZAT 9 //授权
#define KEY_FTUSER_UNQUIT 10    //不退出
#define KEY_FTUSER_DELAY 11     //延迟自动退出
#define KEY_FTUSER_ONETIME 12   //自动退出
#define KEY_FTUSER_DEL 13       // 清除
#define KEY_FTUSER_0 14
#define KEY_FTUSER_1 15
#define KEY_FTUSER_2 16
#define KEY_FTUSER_3 17
#define KEY_FTUSER_4 18
#define KEY_FTUSER_5 19
#define KEY_FTUSER_6 20
#define KEY_FTUSER_7 21
#define KEY_FTUSER_8 22
#define KEY_FTUSER_9 23
#define KEY_FTUSER_A 24
#define KEY_FTUSER_B 25
#define KEY_FTUSER_C 26
#define KEY_FTUSER_D 27
#define KEY_FTUSER_E 28
#define KEY_FTUSER_F 29
#define KEY_FTUSER_G 30
#define KEY_FTUSER_H 31
#define KEY_FTUSER_I 32
#define KEY_FTUSER_J 33
#define KEY_FTUSER_K 34
#define KEY_FTUSER_L 35
#define KEY_FTUSER_M 36
#define KEY_FTUSER_N 37
#define KEY_FTUSER_O 38
#define KEY_FTUSER_P 39
#define KEY_FTUSER_Q 40
#define KEY_FTUSER_R 41
#define KEY_FTUSER_S 42
#define KEY_FTUSER_T 43
#define KEY_FTUSER_U 44
#define KEY_FTUSER_V 45
#define KEY_FTUSER_W 46
#define KEY_FTUSER_X 47
#define KEY_FTUSER_Y 48
#define KEY_FTUSER_Z 49
#define KEY_FTUSER_SELNAME 50     //用户名
#define KEY_FTUSER_SELPASSWORD 51 //密码

#define KEY_FTUSER_SELUSER1 52
#define KEY_FTUSER_SELUSER2 53
#define KEY_FTUSER_SELUSER3 54
#define KEY_FTUSER_SELUSER4 55
#define KEY_FTUSER_SELUSER5 56
#define KEY_FTUSER_SELUSER6 57
#define KEY_FTUSER_SELUSER7 58
#define KEY_FTUSER_SELUSER8 59
#define KEY_FTUSER_SELUSER9 60
#define KEY_FTUSER_SELUSER10 61
#define KEY_FTUSER_MAX 62

#define RECT_FTUSER_STA_TIT 1
#define RECT_FTUSER_STA_STR 2
#define RECT_FTUSER_USER_TIT 3
#define RECT_FTUSER_USER_STR 4
#define RECT_FTUSER_PW_TIT 5
#define RECT_FTUSER_PW_STR 6

#define RECT_FTUSER_OPER_ALL 7
#define RECT_FTUSER_MANA_ALL 8
#define RECT_FTUSER_MANU_ALL 9

//------------------------------------------
// Function   : void Menu_FtUser(void)
// Description: 功能画面_用户登录
//------------------------------------------
void Menu_FtUser(void)
{
#if 1
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U32 i, j;
    U8 backmode = 0; //回退模式 0:通过功能模组界面指定按钮进入此界面;1:其他页面跳转到此页面
    U8 f;
    U8 runflag = 0, oldRunFlag = 0; //0:选择修改哪组用户名和密码;1:用户登录;2:输入原密码;3:输入新密码;4:再次输入新密码
    U8 dis_updata = 1,
       dis_updata1 = 1,  //操作员等级按钮切换刷新
        dis_updata2 = 1, //登出设置按钮切换刷新
        dis_updata3 = 1, //切换用户名输入与密码输入刷新
        dis_updata4 = 0, //用户名输入/密码输入数据刷新
        dis_updata5 = 0, //显示A-Z
        dis_updata6 = 1, //修改按钮刷新
        dis_updata7 = 0, //刷新修改用户选择按钮
        dis_updata8 = 1, //用户登录状态刷新
        dis_updata9 = 0; //切换用户
    U8 selclassflag,     //操作员等级 0:操作员;1:技术员;2:厂商;3:授权码.
        seloutmodeflag,  //登出设置 0:不登出;1:延迟时间登出;2:仅当前操作
        selmpflag,       //用户名/密码选框 0:用户名;1:密码.
        tmpselclassflag;
    U8 tmpname[11], tmppassword[11], bakpassword[11], dispassword[11];
    U8 selnamelen, selpasswordelen;                      //用户名长度,密码长度
    U8 selusers = 0;                                     //可选账户长度
    U8 usrno = 0;                                        //用户编号
    U8 statusnum, oldstatusnum = 0, tmpstatusnum = 0xff; //用户登录状态
    //0:未登录;1:USER :;2:用户名输入错误;3:密码输入错误;4:请输入原密码;5:请输入新密码;
    //6:请再次输入新密码;7:请以管理员登录;8:设备使用无限制;9:年.月.日 停机;10:修改成功.
    U32 MainBoardCode; // 主板编号
    U16 status_flag;   //按钮禁止限制 0:修改密码 1:输入框 2:数字 3:字母 6:0 7:1 8:2 9:3 10:4 11:5 12:6 13:7 14:8 15:9
    U16 checksum = 0;
    U8 Restf = 0,    //万能密码登录-0:未登录;1:登录中
        Setingf = 0; //密码设置状态-1:密码设置中;0:未在设置密码
    U16 PassWord[2] = {0};
    U32 tmpPassWord;
    U8 MainBoardType;
    U8 temp;
    // U8 author = 0; //授权码标志位

    // const U8 RestName[] = {"0000000000"}; //
    // const U8 RestPASS[] = {"0000000000"};
    // const U8 RestName[] = {"GZSKDZKJ01"}; //
    // const U8 RestPASS[] = {"SK29800085"};
    const U8 RestName[] = {"JACK210101"}; //
    const U8 RestPASS[] = {"JACK210101"};

    const U16 keylitd[] = {0xffc0, 0xff40, 0xfec0, 0xfdc0, 0xfbc0, 0xf7c0, 0xe040, 0xd040, 0x8040, 0x4040, 0x0040};
    // const U8 keynum = {KEY_FTUSER_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                                            //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                         //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                        //3 确认
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},                        //4 退出登录
        {LCD_WIDTH - SAFE_DIST1 - KEY_X6, TYPE_Y2, KEY_X6, KEY_Y6}, //5 修改密码
        {568, 176, KEY_X6, KEY_Y6},                                 //6 操作员
        {568 + KEY_X6, 176, KEY_X6, KEY_Y6},                        //7 管理员
        {568 + KEY_X6 * 2, 176, KEY_X6, KEY_Y6},                    //8 设备商
        {568 + KEY_X6 * 3, 176, KEY_X6, KEY_Y6},                    //9 授权
        {TYPE_X47, 176, KEY_X6, KEY_Y6},                            //10 不退出
        {TYPE_X47 + KEY_X6, 176, KEY_X6, KEY_Y6},                   //11 延迟自动退出
        {TYPE_X47 + KEY_X6 * 2, 176, KEY_X6, KEY_Y6},               //12 自动退出

        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 10, TYPE_Y95, KEY_X5, KEY_Y2}, //13 清除
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y47, KEY_X2, KEY_Y2},  //14 0
        {TYPE_X47, TYPE_Y47, KEY_X2, KEY_Y2},                              //15 1
        {TYPE_X47 + KEY_X2 + SAFE_DIST2, TYPE_Y47, KEY_X2, KEY_Y2},        //16 2
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y47, KEY_X2, KEY_Y2},  //17 3
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y47, KEY_X2, KEY_Y2},  //18 4
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y47, KEY_X2, KEY_Y2},  //19 5
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y47, KEY_X2, KEY_Y2},  //20 6
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y47, KEY_X2, KEY_Y2},  //21 7
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y47, KEY_X2, KEY_Y2},  //22 8
        {TYPE_X47 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y47, KEY_X2, KEY_Y2},  //23 9

        {TYPE_X49, TYPE_Y49, KEY_X2, KEY_Y2},                             //24 A
        {TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y50, KEY_X2, KEY_Y2}, //25 B
        {TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y50, KEY_X2, KEY_Y2}, //26 C
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y49, KEY_X2, KEY_Y2}, //27 D
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y48, KEY_X2, KEY_Y2}, //28 E
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y49, KEY_X2, KEY_Y2}, //29 F
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y49, KEY_X2, KEY_Y2}, //30 G
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y49, KEY_X2, KEY_Y2}, //31 H
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y48, KEY_X2, KEY_Y2}, //32 I
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y49, KEY_X2, KEY_Y2}, //33 J
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y49, KEY_X2, KEY_Y2}, //34 K
        {TYPE_X49 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y49, KEY_X2, KEY_Y2}, //35 L
        {TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y50, KEY_X2, KEY_Y2}, //36 M
        {TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y50, KEY_X2, KEY_Y2}, //37 N
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y48, KEY_X2, KEY_Y2}, //38 O
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y48, KEY_X2, KEY_Y2}, //39 P
        {TYPE_X48, TYPE_Y48, KEY_X2, KEY_Y2},                             //40 Q
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y48, KEY_X2, KEY_Y2}, //41 R
        {TYPE_X49 + KEY_X2 + SAFE_DIST2, TYPE_Y49, KEY_X2, KEY_Y2},       //42 S
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y48, KEY_X2, KEY_Y2}, //43 T
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y48, KEY_X2, KEY_Y2}, //44 U
        {TYPE_X50 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y50, KEY_X2, KEY_Y2}, //45 V
        {TYPE_X48 + KEY_X2 + SAFE_DIST2, TYPE_Y48, KEY_X2, KEY_Y2},       //46 W
        {TYPE_X50 + KEY_X2 + SAFE_DIST2, TYPE_Y50, KEY_X2, KEY_Y2},       //47 X
        {TYPE_X48 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y48, KEY_X2, KEY_Y2}, //48 Y
        {TYPE_X50, TYPE_Y50, KEY_X2, KEY_Y2},                             //49 Z

        {255, 88, TYPE_SIZE23, TYPE_SIZE5},  //50 用户名
        {255, 124, TYPE_SIZE23, TYPE_SIZE5}, //51 密码

        {230, TYPE_Y3, KEY_X13, KEY_Y13},
        {480, TYPE_Y3, KEY_X13, KEY_Y13},
        {230, TYPE_Y3 + (KEY_Y13 + SAFE_DIST6), KEY_X13, KEY_Y13},
        {480, TYPE_Y3 + (KEY_Y13 + SAFE_DIST6), KEY_X13, KEY_Y13},
        {230, TYPE_Y3 + (KEY_Y13 + SAFE_DIST6) * 2, KEY_X13, KEY_Y13},
        {480, TYPE_Y3 + (KEY_Y13 + SAFE_DIST6) * 2, KEY_X13, KEY_Y13},
        {230, 220, KEY_X13, KEY_Y13},
        {480, 220, KEY_X13, KEY_Y13},
        {230, 290, KEY_X13, KEY_Y13},
        {480, 290, KEY_X13, KEY_Y13},

    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},

        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_USER, 0, 0, 0}, {0}},      //1 屏保
        {{KEY_Silver}, {0}, {72}, {TLFNADD_FUN_USER + 1, 0, 0, 0}, {0}}, //2 返回
        {{KEY_Silver}, {0}, {71}, {TLFNADD_FUN_USER + 2, 0, 0, 0}, {0}}, //3 确认

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {784}, {TLFNADD_FUN_USER + 3, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {781, 782}, {TLFNADD_FUN_USER + 5, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {773, 0, 774}, {TLFNADD_FUN_USER + 6, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {775, 0, 776}, {TLFNADD_FUN_USER + 7, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {777, 0, 778}, {TLFNADD_FUN_USER + 8, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {779, 0, 780}, {TLFNADD_FUN_USER + 9, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {767, 0, 768}, {TLFNADD_FUN_USER + 10, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {769, 0, 770}, {TLFNADD_FUN_USER + 11, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {771, 0, 772}, {TLFNADD_FUN_USER + 12, 0, 0, 0}, {0}},

        {{KEY_Silver}, {0}, {189}, {8, 0, 0, 0}, {0}},                 //8 删除一位
        {{KEY_Silver, KEY_Gray}, {0}, {91, 101}, {10, 0, 0, 0}, {0}},  //10 0
        {{KEY_Silver, KEY_Gray}, {0}, {92, 102}, {11, 0, 0, 0}, {0}},  //11 1
        {{KEY_Silver, KEY_Gray}, {0}, {93, 103}, {12, 0, 0, 0}, {0}},  //12 2
        {{KEY_Silver, KEY_Gray}, {0}, {94, 104}, {13, 0, 0, 0}, {0}},  //13 3
        {{KEY_Silver, KEY_Gray}, {0}, {95, 105}, {14, 0, 0, 0}, {0}},  //14 4
        {{KEY_Silver, KEY_Gray}, {0}, {96, 106}, {15, 0, 0, 0}, {0}},  //15 5
        {{KEY_Silver, KEY_Gray}, {0}, {97, 107}, {16, 0, 0, 0}, {0}},  //16 6
        {{KEY_Silver, KEY_Gray}, {0}, {98, 108}, {17, 0, 0, 0}, {0}},  //17 7
        {{KEY_Silver, KEY_Gray}, {0}, {99, 109}, {18, 0, 0, 0}, {0}},  //18 8
        {{KEY_Silver, KEY_Gray}, {0}, {100, 110}, {19, 0, 0, 0}, {0}}, //19 9

        {{KEY_Silver}, {0}, {121}, {20, 0, 0, 0}, {0}}, //20 A
        {{KEY_Silver}, {0}, {122}, {21, 0, 0, 0}, {0}}, //21 B
        {{KEY_Silver}, {0}, {123}, {22, 0, 0, 0}, {0}}, //22 C
        {{KEY_Silver}, {0}, {124}, {23, 0, 0, 0}, {0}}, //23 D
        {{KEY_Silver}, {0}, {125}, {24, 0, 0, 0}, {0}}, //24 E
        {{KEY_Silver}, {0}, {126}, {25, 0, 0, 0}, {0}}, //25 F
        {{KEY_Silver}, {0}, {127}, {26, 0, 0, 0}, {0}}, //26 G
        {{KEY_Silver}, {0}, {128}, {27, 0, 0, 0}, {0}}, //27 H
        {{KEY_Silver}, {0}, {129}, {28, 0, 0, 0}, {0}}, //28 I
        {{KEY_Silver}, {0}, {130}, {29, 0, 0, 0}, {0}}, //29 J
        {{KEY_Silver}, {0}, {131}, {30, 0, 0, 0}, {0}}, //30 K
        {{KEY_Silver}, {0}, {132}, {31, 0, 0, 0}, {0}}, //31 L
        {{KEY_Silver}, {0}, {133}, {32, 0, 0, 0}, {0}}, //32 M
        {{KEY_Silver}, {0}, {134}, {33, 0, 0, 0}, {0}}, //33 N
        {{KEY_Silver}, {0}, {135}, {34, 0, 0, 0}, {0}}, //34 O
        {{KEY_Silver}, {0}, {136}, {35, 0, 0, 0}, {0}}, //35 P
        {{KEY_Silver}, {0}, {137}, {36, 0, 0, 0}, {0}}, //36 Q
        {{KEY_Silver}, {0}, {138}, {37, 0, 0, 0}, {0}}, //37 R
        {{KEY_Silver}, {0}, {139}, {38, 0, 0, 0}, {0}}, //38 S
        {{KEY_Silver}, {0}, {140}, {39, 0, 0, 0}, {0}}, //39 T
        {{KEY_Silver}, {0}, {141}, {40, 0, 0, 0}, {0}}, //40 U
        {{KEY_Silver}, {0}, {142}, {41, 0, 0, 0}, {0}}, //41 V
        {{KEY_Silver}, {0}, {143}, {42, 0, 0, 0}, {0}}, //42 W
        {{KEY_Silver}, {0}, {144}, {43, 0, 0, 0}, {0}}, //43 X
        {{KEY_Silver}, {0}, {145}, {44, 0, 0, 0}, {0}}, //44 Y
        {{KEY_Silver}, {0}, {146}, {45, 0, 0, 0}, {0}}, //45 Z

        {{Color_Black}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black}, {TLFNADD_FUN_USER + 26, TLFNADD_FUN_USER + 28}, {0}},
        {{Color_Black}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black}, {TLFNADD_FUN_USER + 27}, {0}},

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 15}, {TLFNADD_FUN_USER + 15}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 16}, {TLFNADD_FUN_USER + 16}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 17}, {TLFNADD_FUN_USER + 17}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 18}, {TLFNADD_FUN_USER + 18}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 19}, {TLFNADD_FUN_USER + 19}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 20}, {TLFNADD_FUN_USER + 20}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 21}, {TLFNADD_FUN_USER + 21}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 22}, {TLFNADD_FUN_USER + 22}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 23}, {TLFNADD_FUN_USER + 23}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Black, Color_White}, {KEY_Silver, KEY_Gray, KEY_Blue}, {TLFNADD_FUN_USER + 24}, {TLFNADD_FUN_USER + 24}},
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FUN_USER}},

        {150, 40, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_USER + 25}},
        {255, 40, TYPE_SIZE23, TYPE_SIZE5, {Color_Black}, {Color_Black}, {TLFNADD_FUN_USER + 34}},
        {150, 88, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_USER + 26, TLFNADD_FUN_USER + 28}},
        {255, 88, TYPE_SIZE23, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue, Color_Black}, {Color_Black, Color_Black}, {0}},
        {150, 124, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_USER + 27}},
        {255, 124, TYPE_SIZE23, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},

        {100, 40, 600, 160, {Color_Black}, {Color_Black}, {TLFNADD_FUN_USER + 29}},
        {100, 210, 600, 60, {Color_Black}, {Color_Black}, {TLFNADD_FUN_USER + 30}},
        {100, 280, 600, 60, {Color_Black}, {Color_Black}, {TLFNADD_FUN_USER + 31}},
    };
    SetLanguage(MENU_FUNCTION);
    readUsrMsg();

#if 0
    //来自参数修改界面
    if ((plastno == MENU_PROG) || (poldno[2] == MENU_FTSERVO || (poldno[2] == MENU_FTSTEPPER) || (plastno == MENU_DIFFERENT)))
    {
        if ((g_usersave > 7) && (g_usersave < 10))
            j = g_usersave;
        else
            j = 8;
        if ((g_user.newuser) && (g_user.newuser < 10))
            statusnum = 1;
        else
            statusnum = 0;
        selclassflag = 2;
        seloutmodeflag = 2;
        backmode = 1;
    }
    //来自IO测试界面与功能模组界面
    else if ((plastno == MENU_IOTEST) || (plastno == MENU_FTSCREENSET) ||
             (poldno[2] == MENU_FTSYSRESTORE) || (poldno[2] == MENU_PARAMODIFY) || (poldno[2] == MENU_FTRESET))
    {
        if ((g_usersave > 5) && (g_usersave < 10))
            j = g_usersave;
        else
            j = 6;
        if ((g_user.newuser) && (g_user.newuser < 10))
            statusnum = 1;
        else
            statusnum = 0;
        selclassflag = 1;
        seloutmodeflag = 2;
        backmode = 1;
    }
    else if ((g_user.newuser) && (g_user.newuser < 10))
    {
        j = g_user.newuser;
        statusnum = 1;
        if (j < 6)
            selclassflag = 0;
        else if (j < 8)
            selclassflag = 1;
        else
            selclassflag = 2;
        seloutmodeflag = g_user.newmode;
    }
    else
    {
        g_user.newuser = 0;
        if ((g_usersave > 0) && (g_usersave < 10))
            j = g_usersave;
        else
            j = 1;
        statusnum = 0;
        selclassflag = 0;
        seloutmodeflag = 0;
    }
#else
    //来自参数修改界面
    if ((plastno == MENU_PROG) || (plastno == MENU_DIFFERENT) ||
        (poldno[pnoindex - 1] == MENU_PROG) || (poldno[pnoindex - 1] == MENU_DIFFERENT))
    {
        if ((g_usersave > 7) && (g_usersave < 10))
            j = g_usersave;
        else
            j = 8;
        if ((g_user.newuser) && (g_user.newuser < 10))
            statusnum = 1;
        else
            statusnum = 0;
        selclassflag = 2;
        seloutmodeflag = 2;
        backmode = 1;
    }
    //来自IO测试界面与功能模组界面
    else if ((plastno == MENU_OUTPUTSTATUS) || (plastno == MENU_INPUTSTATUS) ||
             (poldno[pnoindex - 1] == MENU_OUTPUTSET) || (poldno[pnoindex - 1] == MENU_INPUTSET))
    {
        if ((g_usersave > 5) && (g_usersave < 10))
            j = g_usersave;
        else
            j = 6;
        if ((g_user.newuser) && (g_user.newuser < 10))
            statusnum = 1;
        else
            statusnum = 0;
        selclassflag = 1;
        seloutmodeflag = 2;
        backmode = 1;
    }
    else if ((g_user.newuser) && (g_user.newuser < 10))
    {
        j = g_user.newuser;
        statusnum = 1;
        if (j < 6)
            selclassflag = 0;
        else if (j < 8)
            selclassflag = 1;
        else
            selclassflag = 2;
        seloutmodeflag = g_user.newmode;
    }
    else
    {
        g_user.newuser = 0;
        if ((g_usersave > 0) && (g_usersave < 10))
            j = g_usersave;
        else
            j = 1;
        statusnum = 0;
        selclassflag = 0;
        seloutmodeflag = 0;
    }

#endif
    runflag = 1;
    selmpflag = 1;

    status_flag = 1;
    tmpselclassflag = selclassflag;
    selnamelen = 0;

    usrno = j;

    for (f = 0, i = 0; i < 10; i++)
    {
        if ((g_user.users[j].name[i] < 0x30) || (g_user.users[j].name[i] > 0x5A) || ((g_user.users[j].name[i] > 0x39) && (g_user.users[j].name[i] < 0x41)))
            g_user.users[j].name[i] = 0;
        tmpname[i] = g_user.users[j].name[i]; //0x20;
        tmppassword[i] = 0;
        dispassword[i] = 0x2a;
        if ((tmpname[i] == 0) && (f == 0))
        {
            selnamelen = i;
            f = 1;
        }
    }
    if ((selnamelen == 0) && (tmpname[0]))
        selnamelen = 10;
    tmpname[10] = 0;
    tmppassword[10] = 0;
    bakpassword[10] = 0;
    dispassword[10] = 0;
    selpasswordelen = 0;
    dispassword[0] = 0;

    //	YREG[SYS_MAINBOARD_CODE2] = 0x23d4;YREG[SYS_MAINBOARD_CODE1] = 0xf941;	 //601160001;//
    Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 7, (U16 *)(&YREG[SYS_MACHINE_TYPE])); //读系统版本,编号信息
    MainBoardCode = ((U32)(YREG[SYS_MAINBOARD_CODE2]) << 16) + (U32)(YREG[SYS_MAINBOARD_CODE1]);
    MainBoardType = MainBoardCode % 10;

    //读系统时间
    Read_D(D_Address[SYS] + SYS_MS, 4, (U16 *)(&YREG[SYS_MS]));

    g_SysTimes.day = BCD_2Hex((U8)(YREG[SYS_MD] & 0x00ff));
    g_SysTimes.month = BCD_2Hex((U8)(YREG[SYS_MD] >> 8));
    g_SysTimes.year = BCD_2Hex((U8)(YREG[SYS_Y] & 0x00ff));
    Timer_ms = 0;

    while (1)
    {
        if (dis_updata)
        {
            dis_updata = 0;

            if (oldRunFlag != runflag)
            {
                SetLanguage(MENU_FUNCTION);
                InitScreen();
                initViewStatus();
                updateViewStatus();
                Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
            }
            if (runflag)
            {
                dis_updata1 = 1;
                dis_updata2 = 1;
                dis_updata3 = 1;
                dis_updata4 = 0;
                dis_updata5 = 0;
                dis_updata6 = 1;
                dis_updata7 = 0;
                dis_updata8 = 1;
                dis_updata9 = 0;
                for (i = KEY_FTUSER_BACK; i <= KEY_FTUSER_9; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }
                if (backmode == 1)
                {
                    BmpFillRect(keyd[KEY_FTUSER_AUTHORIZAT].xpis, keyd[KEY_FTUSER_AUTHORIZAT].ypis, keyd[KEY_FTUSER_AUTHORIZAT].xsize, keyd[KEY_FTUSER_AUTHORIZAT].ysize);
                    status_flag |= 0x01;
                }
                for (i = RECT_FTUSER_STA_TIT; i <= RECT_FTUSER_PW_STR; i += 2)
                {
                    DrawEditTit(rect[i], rect[i].index[0], SheetPage, LEFT, STR24X12, SAFE_DIST4);
                }
                DrawRect(rect[RECT_FTUSER_STA_STR].xpis, rect[RECT_FTUSER_STA_STR].ypis, rect[RECT_FTUSER_STA_STR].xsize, rect[RECT_FTUSER_STA_STR].ysize, rect[RECT_FTUSER_STA_STR].colorB[0]);
                if (selclassflag != 3)
                    dis_updata5 = 1;
            }
            else if (runflag == 0) //按钮进入后的界面
            {

                for (i = KEY_FTUSER_BACK; i <= KEY_FTUSER_ENTER; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }

                for (i = RECT_FTUSER_OPER_ALL; i <= RECT_FTUSER_MANU_ALL; i++)
                {
                    DrawRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST4,
                                 (MEM_TITLE_ADD + (U32)rect[i].index[0] * TitleLengthMax), SheetPage, rect[i].index[0], rect[i].colorT[0]);
                }

                dis_updata7 = 1;
                (Restf) ? (selusers = 10) : (selusers = g_user.newuser);
                status_flag &= 0x3f;
                status_flag |= keylitd[selusers];
                if (g_user.newuser == 9) //用户9可以修改用户8
                {
                    status_flag &= ~keylitd[7];
                }
            }

            if (oldRunFlag != runflag)
            {
                oldRunFlag = runflag;
                ViewPage(usepageno);
            }
        }

        keydata = get_keydata((KEYPAD *)keyd, KEY_FTUSER_ENTER + 1, 0);
        if (keydata == 0)
        {
            if (runflag)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_FTUSER_SELPASSWORD + 1, KEY_FTUSER_QUIT);
                if (((keydata == KEY_FTUSER_EDIT) && (status_flag & 0x1)) ||
                    (((keydata == KEY_FTUSER_SELNAME) || (keydata == KEY_FTUSER_SELPASSWORD)) && (status_flag & 0x2)) ||
                    ((keydata >= KEY_FTUSER_DEL) && (keydata <= KEY_FTUSER_9) && (status_flag & 0x4)) ||
                    ((keydata >= KEY_FTUSER_A) && (keydata <= KEY_FTUSER_Z) && (status_flag & 0x8)))
                {
                    keydata = 0;
                    resetKeyData();
                }
                if ((Setingf == 1) && (keydata >= KEY_FTUSER_OPERATOR) && (keydata <= KEY_FTUSER_AUTHORIZAT))
                {
                    keydata = 0;
                    resetKeyData();
                }
                if (backmode == 1 && keydata == KEY_FTUSER_AUTHORIZAT)
                {
                    keydata = 0;
                    resetKeyData();
                }
            }
            else if (runflag == 0)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_FTUSER_SELUSER10 + 1, KEY_FTUSER_SELUSER1);
                if ((keydata >= KEY_FTUSER_SELUSER1) && (keydata <= KEY_FTUSER_SELUSER10))
                {
                    U32 tmp;
                    tmp = 0x40;
                    tmp <<= (keydata - KEY_FTUSER_SELUSER1);
                    if (status_flag & tmp)
                    {
                        keydata = 0;
                        resetKeyData();
                    }
                }
            }
        }

        if (keydata == 0xff)
        {
            if (keyon == KEY_FTUSER_DEL)
            {
            }
            else if ((keyon) && (beeponf))
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > 1)
        {
            if (keyon == 0)
            {
                keyon = keydata;

                if (keyon == KEY_FTUSER_SCRNSVR)
                {
                }
                else if (keyon == KEY_FTUSER_SELNAME || keyon == KEY_FTUSER_SELPASSWORD)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
                }
                else
                {
                    DrawButtonOn(keyd[keydata], keybmpind[keydata].border[0]); //常规按键特效
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
                    if (g_keyBellT) //按键时蜂鸣器发声时间
                        Beep_On(g_keyBellT);
                    if (keyon >= KEY_FTUSER_DEL && keyon <= KEY_FTUSER_Z)
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
                Times = 0;
                if (keyon == KEY_FTUSER_DEL)
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
            if ((keyoff >= KEY_FTUSER_OPERATOR) && (keyoff <= KEY_FTUSER_AUTHORIZAT))
            {

                // if (keyoff == KEY_FTUSER_AUTHORIZAT)
                //     author = 1;
                // else
                //     author = 0;

                if (backmode == 0)
                {
                    selclassflag = keyoff - KEY_FTUSER_OPERATOR;
                    dis_updata1 = 1;
                    dis_updata9 = 1;
                    if (tmpselclassflag != selclassflag)
                    {
                        if ((selclassflag == 3) || (tmpselclassflag == 3))
                            dis_updata3 = 1;
                        if (((selclassflag == 1) || (selclassflag == 2)) && (seloutmodeflag == 0))
                        {
                            seloutmodeflag = 2;
                            dis_updata2 = 1;
                        }
                    }
                }
                else
                {
                    if (keyoff != KEY_FTUSER_AUTHORIZAT) //不切换到授权模式
                    {
                        selclassflag = keyoff - KEY_FTUSER_OPERATOR;
                        dis_updata1 = 1;
                        dis_updata9 = 1;
                    }
                }
            }
            else if ((keyoff >= KEY_FTUSER_UNQUIT) && (keyoff <= KEY_FTUSER_ONETIME))
            { // 11 -13
                seloutmodeflag = keyoff - KEY_FTUSER_UNQUIT;
                dis_updata2 = 1;
            }
            else if ((keyoff >= KEY_FTUSER_0) && (keyoff <= KEY_FTUSER_Z)) //14 - 49
            {
                if (selmpflag == 0)
                {
                    if (selnamelen < 10)
                    {
                        if (keyoff < KEY_FTUSER_A)
                            tmpname[selnamelen] = keyoff - KEY_FTUSER_0 + 0x30;
                        else
                            tmpname[selnamelen] = keyoff - KEY_FTUSER_A + 0x41;
                        selnamelen++;
                        dis_updata4 = 1;
                    }
                }
                else
                {
                    if (selpasswordelen < 10)
                    {
                        if (keyoff < KEY_FTUSER_A)
                            tmppassword[selpasswordelen] = keyoff - KEY_FTUSER_0 + 0x30;
                        else
                            tmppassword[selpasswordelen] = keyoff - KEY_FTUSER_A + 0x41;
                        dispassword[selpasswordelen] = 0x2a;
                        selpasswordelen++;
                        dispassword[selpasswordelen] = 0;
                        dis_updata4 = 1;
                    }
                }
                if (tmpstatusnum == 10)
                {
                    statusnum = 1;
                    dis_updata8 = 1;
                }
            }
            else if ((keyoff >= KEY_FTUSER_SELUSER1) && (keyoff <= KEY_FTUSER_SELUSER10))
            { //52 -61
                selusers = keyoff - KEY_FTUSER_SELUSER1;
                dis_updata7 = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTUSER_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_FTUSER_BACK:
                    if (runflag == 1) //
                    {
                        if (backmode)
                        {
                            pno = plastno;
                            if (plastno == MENU_FTSCREENSET)
                                g_runstepf = 0;
                        }
                        else
                        {
                            pno = MENU_FUNCTION;
                            if (Restf)
                                g_user.newuser = 0;
                        }
                        pnoindex = 1;
                    }
                    else if ((runflag >= 2) && (runflag <= 4))
                    {
                        if (((g_user.newuser) && (g_user.newuser < 10)) || (Restf))
                        {
                            j = g_user.newuser;
                            statusnum = 1;
                        }
                        else
                        {
                            g_user.newuser = 0;
                            j = 0;
                            statusnum = 0;
                        }
                        runflag = 1;
                        if (Restf == 0)
                        {
                            Setingf = 0;
                        }
                        dis_updata = 1;
                        dis_updata8 = 1;
                        selnamelen = 0;
                        selpasswordelen = 0;
                        for (f = 0, i = 0; i < 10; i++)
                        {
                            if ((g_user.users[selusers].name[i] < 0x30) || (g_user.users[selusers].name[i] > 0x5A) || ((g_user.users[selusers].name[i] > 0x39) && (g_user.users[selusers].name[i] < 0x41)))
                                g_user.users[selusers].name[i] = 0;
                            tmpname[i] = g_user.users[selusers].name[i];
                            tmppassword[i] = 0;
                            dispassword[i] = 0x2a;
                            if ((tmpname[i] == 0) && (f == 0))
                            {
                                selnamelen = i;
                                f = 1;
                            }
                        }
                        if ((selnamelen == 0) && (tmpname[0]))
                            selnamelen = 10;
                        dispassword[0] = 0x00;
                        selmpflag = 1;
                        status_flag &= (Restf ? 0xfffe : 0xfffd);
                    }
                    else
                    {
                        runflag = 1;
                        dis_updata = 1;
                        statusnum = 1;
                        tmpstatusnum = 0;
                    }

                    break;
                case KEY_FTUSER_QUIT:
                    if (runflag == 1)
                    {
                        if ((g_user.newuser > 5) && (g_user.olduser))
                        {
                            g_user.newuser = g_user.olduser;
                            g_user.newmode = g_user.oldmode;
                            statusnum = 1;
                            tmpstatusnum = 2;
                            dis_updata8 = 1;
                        }
                        else
                        {
                            g_user.newuser = 0;
                            g_user.newmode = 0;
                            statusnum = 0;
                            tmpstatusnum = 1;
                            dis_updata8 = 1;
                        }
                        if (Write_Cmd3(C_USRLOG, g_user.newuser, g_user.newmode, g_user.olduser, g_user.oldmode) == 0) //发送登录指令
                        {
                            // pno = MENU_C2_HELP;
                            // hleppara = 16;
                            // break;
                        }
                        Read_Status();

                        g_user.olduser = 0;
                        g_user.oldmode = 0;
                        status_flag |= 0x1;
                        dis_updata6 = 1;
                    }
                    Restf = 0;
                    Setingf = 0;
                    break;
                case KEY_FTUSER_ENTER:

                    if (Setingf == 0)
                    {
                        for (i = 0; i < 10; i++)
                        {
                            if ((tmpname[i] != RestName[i]) || (tmppassword[i] != RestPASS[i]))
                                break;
                        }
                        if (i == 10)
                        {
                            Restf = 1; //使用万能密码
                        }
                        else if (Restf == 1)
                            Restf = 2;
                    }

                    if ((Setingf == 0) && (Restf == 1) && (runflag == 1)) //使用万能密码
                    {
                        dis_updata1 = 1;
                        dis_updata2 = 1;
                        dis_updata6 = 1;
                        dis_updata8 = 1;
                        status_flag &= 0xfffe;
                        statusnum = 1;
                        tmpstatusnum = 0;
                        seloutmodeflag = 2;
                        g_user.newmode = seloutmodeflag;

                        for (i = 0; i < 10; i++)
                        {
                            tmpname[i] = RestName[i];
                            dispassword[i] = 0;
                        }
                        selnamelen = 10;
                        selpasswordelen = 0;
                        DrawButtonEdit(keyd[KEY_FTUSER_SELPASSWORD], keybmpind[KEY_FTUSER_SELPASSWORD].border[1], keybmpind[KEY_FTUSER_SELPASSWORD].icon[1]);
                    }
                    else
                    {
                        if (runflag == 0) // 选择修改哪组用户名和密码
                        {
                            if (selusers == g_user.newuser) // 修改已经登录用户需要确认密码
                            {
                                runflag = 2;
                                Setingf = 1; //正在设置密码
                                statusnum = 4;
                                selmpflag = 1;
                                status_flag |= 0x2;
                            }
                            else // 修改下一级用户不用确认密码
                            {
                                runflag = 3;
                                Setingf = 1; //正在设置密码
                                statusnum = 5;
                                selmpflag = 1;
                                status_flag &= 0xfffd;
                            }
                            dis_updata = 1;
                            // dis_updata8 = 1;
                            selnamelen = 0;
                            selpasswordelen = 0;
                            for (f = 0, i = 0; i < 10; i++)
                            {
                                if ((g_user.users[selusers].name[i] < 0x30) ||
                                    (g_user.users[selusers].name[i] > 0x5A) ||
                                    ((g_user.users[selusers].name[i] > 0x39) && (g_user.users[selusers].name[i] < 0x41)))
                                    g_user.users[selusers].name[i] = 0;
                                tmpname[i] = g_user.users[selusers].name[i];
                                tmppassword[i] = 0;
                                dispassword[i] = 0x2a;
                                if ((tmpname[i] == 0) && (f == 0))
                                {
                                    selnamelen = i;
                                    f = 1;
                                }
                            }
                            if ((selnamelen == 0) && (tmpname[0]))
                                selnamelen = 10;
                            dispassword[0] = 0x00;
                        }
                        else if (runflag == 2) // 输入原密码
                        {
                            if (checkuserpassword(tmppassword, selusers, selpasswordelen) == 0) // 密码错
                            {
                                statusnum = 3;
                                dis_updata8 = 1;
                            }
                            else
                            {
                                runflag = 3;
                                Setingf = 1; //正在设置密码
                                statusnum = 5;
                                dis_updata3 = 1;
                                dis_updata8 = 1;
                                for (i = 0; i < selpasswordelen; i++)
                                {
                                    tmppassword[i] = 0;
                                }
                                dispassword[selpasswordelen] = 0x2a;
                                dispassword[0] = 0x00;
                                selpasswordelen = 0;
                                selmpflag = 1;
                                status_flag &= 0xfffd;
                            }
                        }
                        else if (runflag == 3) // 输入新密码
                        {
                            if ((selpasswordelen == 0) || (selpasswordelen > 9)) // 密码错
                            {
                                statusnum = 3;
                                dis_updata8 = 1;
                            }
                            else
                            {
                                runflag = 4;
                                Setingf = 1; //正在设置密码
                                statusnum = 6;
                                dis_updata3 = 1;
                                dis_updata8 = 1;
                                for (i = 0; i < selpasswordelen; i++)
                                {
                                    bakpassword[i] = tmppassword[i];
                                    tmppassword[i] = 0;
                                }
                                for (; i < 10; i++)
                                {
                                    bakpassword[i] = 0;
                                }
                                dispassword[selpasswordelen] = 0x2a;
                                dispassword[0] = 0x00;
                                selpasswordelen = 0;
                                selmpflag = 1;
                                status_flag |= 0x2;
                            }
                        }
                        else if (runflag == 4) // 再次输入新密码
                        {
                            if ((selpasswordelen == 0) || (selpasswordelen > 9)) // 密码错
                            {
                                statusnum = 3;
                                dis_updata8 = 1;
                            }
                            else
                            {
                                for (i = 0; i < selpasswordelen; i++)
                                {
                                    if (bakpassword[i] != tmppassword[i])
                                        break;
                                }
                                if ((i >= selpasswordelen) && (bakpassword[selpasswordelen] == 0)) // 用户名和密码修改成功
                                {
                                    for (i = 0; i < 10; i++)
                                    {
                                        g_user.users[selusers].name[i] = tmpname[i];
                                        g_user.users[selusers].password[i] = bakpassword[i];
                                    }

                                    strcopy((U8 *)&(g_user), (U8 *)(&UREG), 4); //将新密码数据发送到主板
                                    strcopy((U8 *)((U8 *)(&(g_user)) + 4), (U8 *)(&UREG[3]), 240);
                                    checksum = 0;
                                    for (i = 3; i < 123; i++)
                                    {
                                        checksum += UREG[i];
                                    }
                                    UREG[2] = checksum;
                                    if (Write_D(D_Address[USERLOG], 123, (U16 *)(&UREG[0])) == 0)
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 16;
                                        break;
                                    }

                                    if (Write_Cmd(C_SAVEPW, 0, 0) == 0) //发送保存指令
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 16;
                                        break;
                                    }
                                    if (Read_Status() == 0)
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 15;
                                        break;
                                    }
                                    else if (DREG[D_CMDSTATUS] != 100)
                                    {

                                        pno = MENU_RWHELP;
                                        hleppara = 16;
                                        break;
                                    }

                                    statusnum = 10;
                                    dis_updata8 = 1;
                                    dis_updata3 = 1;
                                    dis_updata1 = 1;
                                    runflag = 1;
                                    //Restf =0;
                                    Setingf = 0; //取消正在设置密码
                                    status_flag &= (Restf ? 0xfffe : 0xfffd);
                                }
                                else
                                {
                                    statusnum = 3;
                                    dis_updata8 = 1;
                                }
                            }
                        }
                        else if (runflag == 1) //用户登录
                        {
                            if (selclassflag == 3) // 输入授权密码
                            {
                                tmpPassWord = MakeAutPassword(tmppassword, MainBoardType, selpasswordelen);
                                PassWord[0] = tmpPassWord & 0xffff;
                                PassWord[1] = (tmpPassWord >> 16) & 0xffff;

                                if (Write_Cmd(C_SETPASSSWORD, PassWord[0], PassWord[1]) == 0) //发送密码到主板
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    break;
                                }
                                if (Read_Status() == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] == 100)
                                {
                                    f = 1;
                                }
                                else if (DREG[D_CMDSTATUS] == 4) // 密码错
                                {
                                    f = 0;
                                }
                                else //操作失败
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 16;
                                    break;
                                }

                                if (f == 0) // 密码错
                                {
                                    statusnum = 3;
                                    dis_updata8 = 1;
                                }
                                else
                                {
                                    Read_D(D_Address[SYS] + SYS_RETURN_MD, 2, (U16 *)(&YREG[SYS_RETURN_MD]));
                                    if ((YREG[SYS_RETURN_MD] == 0x9999) && (YREG[SYS_RETURN_Y] == 0x9999)) // 使用无限制
                                    {
                                        statusnum = 8;
                                    }
                                    else
                                    {
                                        statusnum = 9;
                                    }
                                    for (i = 0; i < 10; i++) //清除密码
                                    {
                                        tmppassword[i] = 0;
                                    }
                                    selpasswordelen = 0;
                                    dis_updata8 = 1;
                                    DrawButtonEdit(keyd[KEY_FTUSER_SELPASSWORD], keybmpind[KEY_FTUSER_SELPASSWORD].border[1], keybmpind[KEY_FTUSER_SELPASSWORD].icon[1]);
                                }
                            }
                            else // 用户登录
                            {
                                f = checkusername(tmpname, selnamelen);

                                if (f == 0) // 用户名错
                                {
                                    statusnum = 2;
                                    dis_updata8 = 1;
                                }
                                else
                                {
                                    if (checkuserpassword(tmppassword, f, selpasswordelen) == 0) // 密码错
                                    {
                                        statusnum = 3;
                                        dis_updata8 = 1;
                                    }
                                    else
                                    {
                                        if (Write_Cmd(C_USRLOG, f, seloutmodeflag) == 0) //发送登录指令
                                            // {
                                            //     pno = MENU_RWHELP;
                                            //     hleppara = 16;
                                            //     break;
                                            // }
                                            Read_Status();
                                        // if (Read_Status() == 0)
                                        // {
                                        //     pno = MENU_RWHELP;
                                        //     hleppara = 15;
                                        //     break;
                                        // }
                                        // else if (DREG[D_CMDSTATUS] != 100)
                                        // {

                                        //     pno = MENU_RWHELP;
                                        //     hleppara = 16;
                                        //     break;
                                        // }

                                        if ((g_user.newuser > 0) && (g_user.newuser < 6) && (f > 5))
                                        {
                                            g_user.olduser = g_user.newuser;
                                            g_user.oldmode = g_user.newmode;
                                        }
                                        else
                                        {
                                            g_user.olduser = 0;
                                            g_user.oldmode = 0;
                                        }
                                        g_user.newuser = f;
                                        g_user.newmode = seloutmodeflag;
                                        if (f < 6)
                                        {
                                            selclassflag = 0;
                                            seloutmodeflag = 0;
                                            dis_updata2 = 1;
                                        }
                                        else if (f < 8)
                                        {
                                            selclassflag = 1;
                                        }
                                        else
                                        {
                                            selclassflag = 2;
                                        }
                                        Setingf = 0;
                                        Restf = 0; //登录成功，切换用户编号，不使用万能用户
                                        dis_updata1 = 1;
                                        status_flag &= 0xfffe;
                                        dis_updata6 = 1;
                                        statusnum = 1;
                                        tmpstatusnum = 0;
                                        dis_updata8 = 1;
                                        g_usersave = f;
                                        SaveUserNo(g_usersave); //保存用户编号
                                        for (i = 0; i < selpasswordelen; i++)
                                        {
                                            tmppassword[i] = 0;
                                        }
                                        dispassword[selpasswordelen] = 0x2a;
                                        dispassword[0] = 0x00;
                                        selpasswordelen = 0;
                                        dis_updata3 = 1;
                                        userLoninTime = 0;
                                        if (backmode)
                                        {
                                            if ((plastno == MENU_PROG) || (poldno[2] == MENU_FTSERVO) || (poldno[2] == MENU_FTSTEPPER))
                                            {
                                                if (f > 7)
                                                {
                                                    pno = poldno[2];
                                                }
                                            }
                                            else if (plastno == MENU_DIFFERENT)
                                            {
                                                if (f > 7)
                                                {
                                                    pno = MENU_PROPARA;
                                                }
                                            }
                                            else
                                            {
                                                if (f > 5)
                                                {
                                                    pno = poldno[2];
                                                }
                                            }
                                            if (g_user.newmode == 2) //登录一次后自动退出
                                            {

                                                if (g_user.olduser > 0 && g_user.olduser < 6)
                                                {
                                                    g_user.newuser = g_user.olduser;
                                                    g_user.newmode = g_user.oldmode;
                                                }
                                                else
                                                {
                                                    g_user.newuser = 0;
                                                    g_user.newmode = 0;
                                                }

                                                g_user.olduser = 0;
                                                g_user.oldmode = 0;

                                                if (Write_Cmd3(C_USRLOG, g_user.newuser, g_user.newmode, g_user.olduser, g_user.oldmode) == 0) //发送登录指令
                                                {
                                                    //	pno = MENU_C2_HELP;hleppara = 16;break;
                                                }
                                                Read_Status();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                case KEY_FTUSER_DEL:
                    if (selmpflag == 0)
                    {
                        if (selnamelen)
                        {
                            selnamelen--;
                            tmpname[selnamelen] = 0;
                            dis_updata4 = 1;
                        }
                    }
                    else
                    {
                        if (selpasswordelen)
                        {
                            dispassword[selpasswordelen] = 0x2a;
                            selpasswordelen--;
                            tmppassword[selpasswordelen] = 0;
                            dispassword[selpasswordelen] = 0;
                            dis_updata4 = 1;
                        }
                        if (tmpstatusnum == 3)
                        {
                            statusnum = oldstatusnum;
                            dis_updata8 = 1;
                        }
                    }
                    if (tmpstatusnum == 10)
                    {
                        statusnum = 1;
                        dis_updata8 = 1;
                    }
                    break;
                case KEY_FTUSER_EDIT:
                    if (((g_user.newuser) && (g_user.newuser < 10)) || (Restf))
                    {
                        runflag = 0;
                        dis_updata = 1;
                    }
                    break;
                case KEY_FTUSER_SELNAME:
                    selmpflag = 0;
                    dis_updata3 = 1;
                    if (tmpstatusnum == 10)
                    {
                        statusnum = 1;
                        dis_updata8 = 1;
                    }
                    break;
                case KEY_FTUSER_SELPASSWORD:
                    selmpflag = 1;
                    dis_updata3 = 1;
                    if (tmpstatusnum == 10)
                    {
                        statusnum = 1;
                        dis_updata8 = 1;
                    }
                    break;
                }
            }
            keyoff = 0;
        }
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FTUSER_SCRNSVR) //不需要显示的按键
            {
            }
            else if (keyon == KEY_FTUSER_SELNAME || keyon == KEY_FTUSER_SELPASSWORD)
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
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                         SheetPage, rect[0].index[0], rect[0].colorT[0]); //显示回花样设计
        }

        // [操作员等级按钮切换刷新*begin]****************************
        if (dis_updata1)
        {
            U8 classLen = 0;
            dis_updata1 = 0;

            if (backmode == 1)
            {
                classLen = 3;
            }
            else
            {
                classLen = 4;
            }
            if (Setingf == 1) //正在设置密码,不允许切换用户
            {
                for (i = KEY_FTUSER_OPERATOR; i < KEY_FTUSER_OPERATOR + classLen; i++)
                {
                    DrawColor_Gray1(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
                }
            }
            else
            {
                f = KEY_FTUSER_OPERATOR + selclassflag;
                for (i = KEY_FTUSER_OPERATOR; i < KEY_FTUSER_OPERATOR + classLen; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (i == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (i == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                }
            }
        }
        // [操作员等级按钮切换刷新*end]****************************

        // [登出设置按钮切换刷新*begin]*************************
        if (dis_updata2)
        {
            dis_updata2 = 0;

            f = KEY_FTUSER_UNQUIT + seloutmodeflag;
            for (i = KEY_FTUSER_UNQUIT; i <= KEY_FTUSER_ONETIME; i++)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           (i == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                           (i == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
            }
        }
        // [登出设置按钮切换刷新*end]*************************

        // [切换用户名输入与密码输入刷新*begin]*************************
        if (dis_updata3)
        {
            dis_updata3 = 0;

            if (selclassflag == 3)
            {

                DrawEditTit(rect[RECT_FTUSER_USER_TIT], rect[RECT_FTUSER_USER_TIT].index[1], SheetPage, LEFT, STR24X12, SAFE_DIST4);
                BmpFillRect(rect[RECT_FTUSER_USER_STR].xpis, rect[RECT_FTUSER_USER_STR].ypis, rect[RECT_FTUSER_USER_STR].xsize, rect[RECT_FTUSER_USER_STR].ysize);
                DrawRect(rect[RECT_FTUSER_USER_STR].xpis, rect[RECT_FTUSER_USER_STR].ypis, rect[RECT_FTUSER_USER_STR].xsize, rect[RECT_FTUSER_USER_STR].ysize, rect[RECT_FTUSER_USER_STR].colorB[2]);
                DrawMFU32(rect[RECT_FTUSER_USER_STR].xpis + SAFE_DIST4, rect[RECT_FTUSER_USER_STR].ypis + rect[RECT_FTUSER_USER_STR].ysize / 2 - 12,
                          MainBoardCode, 9, rect[RECT_FTUSER_USER_STR].colorT[0]);

                DrawButtonEdit(keyd[KEY_FTUSER_SELPASSWORD], keybmpind[KEY_FTUSER_SELPASSWORD].border[0], keybmpind[KEY_FTUSER_SELPASSWORD].icon[1]);
                DrawFASC12(keyd[KEY_FTUSER_SELPASSWORD].xpis + SAFE_DIST4, keyd[KEY_FTUSER_SELPASSWORD].ypis + keyd[KEY_FTUSER_SELPASSWORD].ysize / 2 - 12,
                           (char *)dispassword, keybmpind[KEY_FTUSER_SELPASSWORD].textColor[0]);

                for (i = KEY_FTUSER_A; i <= KEY_FTUSER_Z; i++)
                {
                    BmpFillRect(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
                }

                // if (seloutmodeflag)// 使用无限制
                // {
                //     statusnum = 8;
                // }
                // else
                // {
                //     statusnum = 9;
                // }
                selmpflag = 1;
                status_flag |= 0x0B;
                dis_updata8 = 1;
            }
            else
            {
                U8 *tmpP;
                f = KEY_FTUSER_SELNAME + selmpflag;
                for (i = KEY_FTUSER_SELNAME; i <= KEY_FTUSER_SELPASSWORD; i++)
                {
                    if (i == KEY_FTUSER_SELNAME)
                    {
                        tmpP = &tmpname[0];
                    }
                    else if (i == KEY_FTUSER_SELPASSWORD)
                    {
                        tmpP = &dispassword[0];
                    }
                    DrawButtonEdit(keyd[i],
                                   (i == f) ? keybmpind[i].border[0] : keybmpind[i].icon[0],
                                   (i == f) ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
                    DrawFASC12(keyd[i].xpis + SAFE_DIST4, keyd[i].ypis + keyd[i].ysize / 2 - 12,
                               (char *)tmpP, keybmpind[i].textColor[0]);
                }
                if (runflag == 1)
                    status_flag &= 0xfff0; //开放按钮
                else
                    status_flag &= 0xfff2; //禁止用户/密码切换
                if (((g_user.newuser == 0) || (backmode)) && (Restf == 0))
                    status_flag |= 0x01; //禁止修改密码
            }

            if (tmpselclassflag != selclassflag)
            {
                if (tmpselclassflag == 3)
                {
                    dis_updata5 = 1;
                    DrawEditTit(rect[RECT_FTUSER_USER_TIT], rect[RECT_FTUSER_USER_TIT].index[0], SheetPage, LEFT, STR24X12, SAFE_DIST4);
                }
                else if (selclassflag == 3) // 输入授权密码
                {
                    DrawEditTit(rect[RECT_FTUSER_USER_TIT], rect[RECT_FTUSER_USER_TIT].index[1], SheetPage, LEFT, STR24X12, SAFE_DIST4);
                }
            }
            tmpselclassflag = selclassflag;
            dis_updata6 = 1;
        }
        // [切换用户名输入与密码输入刷新*end]*************************

        // [用户名输入/密码输入*begin]*************************
        if (dis_updata4)
        {
            dis_updata4 = 0;

            if (selmpflag == 0)
            {
                DrawButtonEdit(keyd[KEY_FTUSER_SELNAME], keybmpind[KEY_FTUSER_SELNAME].border[0], keybmpind[KEY_FTUSER_SELNAME].icon[1]);
                DrawFASC12(keyd[KEY_FTUSER_SELNAME].xpis + SAFE_DIST4, keyd[KEY_FTUSER_SELNAME].ypis + keyd[KEY_FTUSER_SELNAME].ysize / 2 - 12,
                           (char *)tmpname, keybmpind[KEY_FTUSER_SELNAME].textColor[0]);
            }
            else
            {
                DrawButtonEdit(keyd[KEY_FTUSER_SELPASSWORD], keybmpind[KEY_FTUSER_SELPASSWORD].border[0], keybmpind[KEY_FTUSER_SELPASSWORD].icon[1]);
                DrawFASC12(keyd[KEY_FTUSER_SELPASSWORD].xpis + SAFE_DIST4, keyd[KEY_FTUSER_SELPASSWORD].ypis + keyd[KEY_FTUSER_SELPASSWORD].ysize / 2 - 12,
                           (char *)dispassword, keybmpind[KEY_FTUSER_SELPASSWORD].textColor[0]);
            }
        }
        // [用户名输入/密码输入*end]*************************

        // [显示A-Z*begin]*************************
        if (dis_updata5)
        {
            dis_updata5 = 0;

            for (i = KEY_FTUSER_A; i <= KEY_FTUSER_Z; i++)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
            }
        }
        // [显示A-Z*end]*************************

        // [修改按钮刷新*begin]*************************
        if (dis_updata6)
        {
            dis_updata6 = 0;
            DrawButton(keyd[KEY_FTUSER_EDIT], (U16 *)(ADD_ICONBMP),
                       (status_flag & 0x1) ? keybmpind[KEY_FTUSER_EDIT].border[1] : keybmpind[KEY_FTUSER_EDIT].border[0],
                       (status_flag & 0x1) ? keybmpind[KEY_FTUSER_EDIT].icon[1] : keybmpind[KEY_FTUSER_EDIT].icon[0]);
        }
        // [修改按钮刷新*end]*************************

        // [刷新修改用户选择按钮*begin]*************************
        if (dis_updata7)
        {
            dis_updata7 = 0;

            j = 0x40;
            for (i = KEY_FTUSER_SELUSER1; i <= KEY_FTUSER_SELUSER10; i++)
            {
                f = i - KEY_FTUSER_SELUSER1;
                if ((f == selusers) && (selusers))
                {
                    DrawButtonSingleColor(keyd[i], keybmpind[i].border[2], keybmpind[i].icon[2]);
                    DrawMFU32Z(keyd[i].xpis + SAFE_DIST6, keyd[i].ypis + keyd[i].ysize / 2 - 12, f, 1, keybmpind[i].textColor[2]);
                    Draw24X24FStr(keyd[i].xpis + SAFE_DIST6 + 12, keyd[i].ypis + keyd[i].ysize / 2 - 12, ":", keybmpind[i].textColor[2]);
                    Draw24X24FStr(keyd[i].xpis + SAFE_DIST6 + 12 * 2, keyd[i].ypis + keyd[i].ysize / 2 - 12,
                                  (char *)(&g_user.users[f].name[0]), keybmpind[i].textColor[2]);
                }
                else
                {
                    DrawButtonSingleColor(keyd[i],
                                          (status_flag & j) ? keybmpind[i].border[1] : keybmpind[i].border[0],
                                          (status_flag & j) ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
                    DrawMFU32Z(keyd[i].xpis + SAFE_DIST6, keyd[i].ypis + keyd[i].ysize / 2 - 12, f, 1,
                               (status_flag & j) ? keybmpind[i].textColor[1] : keybmpind[i].textColor[0]);
                    Draw24X24FStr(keyd[i].xpis + SAFE_DIST6 + 12, keyd[i].ypis + keyd[i].ysize / 2 - 12, ":",
                                  (status_flag & j) ? keybmpind[i].textColor[1] : keybmpind[i].textColor[0]);
                    if ((status_flag & j) == 0)
                    {
                        Draw24X24FStr(keyd[i].xpis + SAFE_DIST6 + 12 * 2, keyd[i].ypis + keyd[i].ysize / 2 - 12,
                                      (char *)(&g_user.users[f].name[0]), (status_flag & j) ? keybmpind[i].textColor[1] : keybmpind[i].textColor[0]);
                    }
                }
                j <<= 1;
            }
        }
        // [刷新修改用户选择按钮*end]*************************

        // [用户登录状态刷新*begin]*************************
        if (dis_updata8)
        {
            dis_updata8 = 0;

            if (statusnum != tmpstatusnum)
            {
                if (statusnum == 3)
                    oldstatusnum = tmpstatusnum;
                BmpFillRect(rect[RECT_FTUSER_STA_STR].xpis + 1, rect[RECT_FTUSER_STA_STR].ypis + 1, rect[RECT_FTUSER_STA_STR].xsize - 2, rect[RECT_FTUSER_STA_STR].ysize - 2);
                Draw24x24Str(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                             MEM_TITLE_ADD + (rect[RECT_FTUSER_STA_STR].index[0] + statusnum) * TitleLengthMax,
                             SheetPage, rect[RECT_FTUSER_STA_STR].index[0] + statusnum, rect[RECT_FTUSER_STA_STR].colorT[0]);

                if (statusnum == 1)
                {
                    if (Restf)
                    {
                        Draw24X24FStr(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4 + 13 * 6, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                                      (char *)RestName, rect[RECT_FTUSER_STA_STR].colorT[0]);
                    }
                    else
                    {
                        DrawMFU32Z(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4 + 13 * 4, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                                   g_user.newuser, 1, rect[RECT_FTUSER_STA_STR].colorT[0]);
                        Draw24X24FStr(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4 + 13 * 6, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                                      (char *)(&g_user.users[g_user.newuser].name[0]), rect[RECT_FTUSER_STA_STR].colorT[0]);
                    }
                }
                else if (statusnum == 9)
                {
                    temp = (U8)(YREG[SYS_RETURN_Y]);
                    temp = BCD_2Hex(temp);
                    DrawMFU32Z(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                               2000 + temp, 4, rect[RECT_FTUSER_STA_STR].colorT[0]); //年

                    temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                    temp = BCD_2Hex(temp);
                    DrawMFU32Z(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4 + 5 * 13, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                               temp, 2, rect[RECT_FTUSER_STA_STR].colorT[0]); //月

                    temp = (U8)(YREG[SYS_RETURN_MD]);
                    temp = BCD_2Hex(temp);
                    DrawMFU32Z(rect[RECT_FTUSER_STA_STR].xpis + SAFE_DIST4 + 8 * 13, rect[RECT_FTUSER_STA_STR].ypis + rect[RECT_FTUSER_STA_STR].ysize / 2 - 12,
                               temp, 2, rect[RECT_FTUSER_STA_STR].colorT[0]); //日
                }
                tmpstatusnum = statusnum;
            }
        }
        // [用户登录状态刷新*end]*************************

        // [切换用户*begin]*************************
        if (dis_updata9)
        {
            dis_updata9 = 0;
            if (selclassflag == 0) //级别0
            {
                if ((usrno < 1) || (usrno > 4))
                {
                    usrno = 1;
                }
                else
                {
                    usrno++;
                }
            }
            else if (selclassflag == 1) //级别2
            {
                if (usrno != 6)
                {
                    usrno = 6;
                }
                else
                {
                    usrno++;
                }
            }
            else if (selclassflag == 2) //级别3
            {
                if (usrno != 8)
                {
                    usrno = 8;
                }
                else
                {
                    usrno++;
                }
            }

            if (selclassflag < 3)
            {
                for (i = 0; i < 10; i++)
                {
                    if ((g_user.users[usrno].name[i] < 0x30) || (g_user.users[usrno].name[i] > 0x5A) || ((g_user.users[usrno].name[i] > 0x39) && (g_user.users[usrno].name[i] < 0x41)))
                        g_user.users[usrno].name[i] = 0;
                    tmpname[i] = g_user.users[usrno].name[i]; //0x20;
                    tmppassword[i] = 0;
                    dispassword[i] = 0;
                }
                for (i = 0; i < 10; i++)
                {
                    if (tmpname[i] == 0)
                        break;
                }
                selnamelen = i;
                selpasswordelen = 0;

                selmpflag = 1;

                DrawButtonEdit(keyd[KEY_FTUSER_SELNAME], keybmpind[KEY_FTUSER_SELNAME].icon[0], keybmpind[KEY_FTUSER_SELNAME].icon[0]);
                DrawFASC12(keyd[KEY_FTUSER_SELNAME].xpis + SAFE_DIST4, keyd[KEY_FTUSER_SELNAME].ypis + keyd[KEY_FTUSER_SELNAME].ysize / 2 - 12,
                           (char *)tmpname, keybmpind[KEY_FTUSER_SELNAME].textColor[0]);

                DrawButtonEdit(keyd[KEY_FTUSER_SELPASSWORD], keybmpind[KEY_FTUSER_SELPASSWORD].border[0], keybmpind[KEY_FTUSER_SELPASSWORD].icon[1]);
                DrawFASC12(keyd[KEY_FTUSER_SELPASSWORD].xpis + SAFE_DIST4, keyd[KEY_FTUSER_SELPASSWORD].ypis + keyd[KEY_FTUSER_SELPASSWORD].ysize / 2 - 12,
                           (char *)dispassword, keybmpind[KEY_FTUSER_SELPASSWORD].textColor[0]);
            }
        }
        // [切换用户*end]*************************

        if (Timer_ms > 3000)
        {
            Timer_ms = 0;
            dis_updata8 = 1;
        }

        if (pno != MENU_FTUSER)
        {
            plastno = MENU_FTUSER;
            break;
        }
        wdt();
    }
#endif
}

//-------------------------------------------------
#define KEY_FTSYSVERSION_SCRNSVR 1
#define KEY_FTSYSVERSION_BACK 2
#define KEY_FTSYSVERSION_MAX 3

#define RECT_FTSYSVERSION_ITEM1_TIT 1
#define RECT_FTSYSVERSION_ITEM1_STR 2
#define RECT_FTSYSVERSION_ITEM2_TIT 3
#define RECT_FTSYSVERSION_ITEM2_STR 4
#define RECT_FTSYSVERSION_ITEM3_TIT 5
#define RECT_FTSYSVERSION_ITEM3_STR 6
#define RECT_FTSYSVERSION_ITEM4_TIT 7
#define RECT_FTSYSVERSION_ITEM4_STR 8
#define RECT_FTSYSVERSION_ITEM5_TIT 9
#define RECT_FTSYSVERSION_ITEM5_STR 10
#define RECT_FTSYSVERSION_ITEM6_TIT 11
#define RECT_FTSYSVERSION_ITEM6_STR 12
#define RECT_FTSYSVERSION_ITEM7_TIT 13
#define RECT_FTSYSVERSION_ITEM7_STR 14
#define RECT_FTSYSVERSION_ITEM8_TIT 15
#define RECT_FTSYSVERSION_ITEM8_STR 16
#define RECT_FTSYSVERSION_ITEM9_TIT 17
#define RECT_FTSYSVERSION_ITEM9_STR 18

#define RECT_FTSYSVERSION_ITEM_MAX 9
//------------------------------------------
// Function   : void Menu_FtSysversion(void)
// Description: 功能画面_版本
//------------------------------------------
void Menu_FtSysversion(void)
{
#if 1
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U8 f;
    U32 i;
    U16 MachineType;       //机型
    U16 MainBoardVer;      //主板版本号
    U32 MainBoardCode;     //主板编号
    U32 MainTim;           //主板发布时间
    U16 LCDVer = DISP_VER; //打版器版本号
    U16 LCDTim = PUB_DATE; //打版器时间
    U16 SERVER;            //伺服驱动版本号
    //	U16 OperaVer;				//操作板版本号
    //	U32 OperaCode;				//操作板编号
    U8 enable[RECT_FTSYSVERSION_ITEM_MAX] = {1, 1, 1, 1, 1, 1, 1, 1, 0}; //使能
    const U8 keynum = {KEY_FTSYSVERSION_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                    //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1}, //2 返回
    };
    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {TLFNADD_FUN_VER, 0, 0, 0}, {0}},      //1 屏保
        {{KEY_Silver}, {0}, {72}, {TLFNADD_FUN_VER + 1, 0, 0, 0}, {0}}, //2 返回
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FUN_VER}},
        {140, TYPE_Y2, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 2, TLFNADD_FUN_VER + 3}},                                           //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1), TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 4}},                                    //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1), TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                             //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 2, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 5}},                                //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 2, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 3, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 6}},                                //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 3, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 4, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 7}},                                //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 4, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 5, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 11}},                               //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 5, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 6, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 12}},                               //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 6, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 7, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue, Color_Gray}, {Color_White, Color_Gray}, {TLFNADD_FUN_VER + 10}},       //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 7, TYPE_SIZE22, TYPE_SIZE5, {Color_White, Color_Gray}, {Color_Black, Color_Gray}, {0}}, //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 8, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue, Color_Gray}, {Color_White, Color_Gray}, {TLFNADD_FUN_VER + 13}},       //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 8, TYPE_SIZE22, TYPE_SIZE5, {Color_White, Color_Gray}, {Color_Black, Color_Gray}, {0}}, //2 数据框
    };

    SetLanguage(MENU_FUNCTION);
    InitScreen();
    initViewStatus();
    updateViewStatus();

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
                 SheetPage, rect[0].index[0], rect[0].colorT[0]);

    for (i = 2; i < KEY_FTSYSVERSION_MAX; i++)
    {
        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
    }

    Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 7, (U16 *)(&YREG[SYS_MACHINE_TYPE])); //读系统版本,编号信息
    MachineType = YREG[SYS_MACHINE_TYPE];
    MainBoardVer = YREG[SYS_MAINBOARD_VER];
    MainBoardCode = ((U32)(YREG[SYS_MAINBOARD_CODE2]) << 16) + (U32)(YREG[SYS_MAINBOARD_CODE1]);
    SERVER = YREG[SYS_OPERA_VER];

    Read_D(D_Address[SYS] + SYS_WT1, 2, (U16 *)(&YREG[SYS_WT1])); //读系统版本,编号信息
    MainTim = ((U32)(YREG[SYS_WT2]) << 16) + (U32)(YREG[SYS_WT1]);

    for (i = 0; i < RECT_FTSYSVERSION_ITEM_MAX; ++i)
    {
        f = i * 2 + RECT_FTSYSVERSION_ITEM1_STR;
        DrawFillRect(rect[f].xpis, rect[f].ypis, rect[f].xsize, rect[f].ysize, rect[f].colorB[0]);
    }

    if ((g_UdZhouFlag == 1) || (g_UdZhouFlag == 2)) //TESTKEYF 0x5a  0xa5
    {
        Draw24X24FStr(rect[RECT_FTSYSVERSION_ITEM1_STR].xpis + SAFE_DIST4,
                      rect[RECT_FTSYSVERSION_ITEM1_STR].ypis + rect[RECT_FTSYSVERSION_ITEM1_STR].ysize / 2 - 12, "T", rect[RECT_FTSYSVERSION_ITEM1_STR].colorT[0]);
        DrawMFU32Z(rect[RECT_FTSYSVERSION_ITEM1_STR].xpis + SAFE_DIST4 + 24, rect[RECT_FTSYSVERSION_ITEM1_STR].ypis + rect[RECT_FTSYSVERSION_ITEM1_STR].ysize / 2 - 12,
                   MachineType, 5, rect[RECT_FTSYSVERSION_ITEM1_STR].colorT[0]);
    }
    else if (g_UdZhouFlag == 3) //	0x55
    {
        Draw24X24FStr(rect[RECT_FTSYSVERSION_ITEM1_STR].xpis + SAFE_DIST4,
                      rect[RECT_FTSYSVERSION_ITEM1_STR].ypis + rect[RECT_FTSYSVERSION_ITEM1_STR].ysize / 2 - 12, "D", rect[RECT_FTSYSVERSION_ITEM1_STR].colorT[0]);
        DrawMFU32Z(rect[RECT_FTSYSVERSION_ITEM1_STR].xpis + SAFE_DIST4 + 24, rect[RECT_FTSYSVERSION_ITEM1_STR].ypis + rect[RECT_FTSYSVERSION_ITEM1_STR].ysize / 2 - 12,
                   MachineType, 5, rect[RECT_FTSYSVERSION_ITEM1_STR].colorT[0]);
    }
    else
    {
        DrawMFU32Z(rect[RECT_FTSYSVERSION_ITEM1_STR].xpis + SAFE_DIST4, rect[RECT_FTSYSVERSION_ITEM1_STR].ypis + rect[RECT_FTSYSVERSION_ITEM1_STR].ysize / 2 - 12,
                   MachineType, 5, rect[RECT_FTSYSVERSION_ITEM1_STR].colorT[0]);
    }
    //主板版本号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM2_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM2_STR].ypis + rect[RECT_FTSYSVERSION_ITEM2_STR].ysize / 2 - 12,
              MainBoardVer, 4, rect[RECT_FTSYSVERSION_ITEM2_STR].colorT[0]);
    //主板发布时间
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM2_STR].xpis + SAFE_DIST4 + 13 * 6,
              rect[RECT_FTSYSVERSION_ITEM2_STR].ypis + rect[RECT_FTSYSVERSION_ITEM2_STR].ysize / 2 - 12,
              MainTim, 4, rect[RECT_FTSYSVERSION_ITEM2_STR].colorT[0]);
    //主板编号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM3_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM3_STR].ypis + rect[RECT_FTSYSVERSION_ITEM3_STR].ysize / 2 - 12,
              MainBoardCode, 9, rect[RECT_FTSYSVERSION_ITEM3_STR].colorT[0]);

    //打版器版本号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM4_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM4_STR].ypis + rect[RECT_FTSYSVERSION_ITEM4_STR].ysize / 2 - 12,
              LCDVer, 4, rect[RECT_FTSYSVERSION_ITEM4_STR].colorT[0]);
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM4_STR].xpis + SAFE_DIST4 + 13 * 6,
              rect[RECT_FTSYSVERSION_ITEM4_STR].ypis + rect[RECT_FTSYSVERSION_ITEM4_STR].ysize / 2 - 12,
              LCDTim, 4, rect[RECT_FTSYSVERSION_ITEM4_STR].colorT[0]);

    //打版器编号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM5_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM5_STR].ypis + rect[RECT_FTSYSVERSION_ITEM5_STR].ysize / 2 - 12,
              SYSSETID, 9, rect[RECT_FTSYSVERSION_ITEM5_STR].colorT[0]);

    //伺服版本号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM6_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM6_STR].ypis + rect[RECT_FTSYSVERSION_ITEM6_STR].ysize / 2 - 12,
              SERVER, 4, rect[RECT_FTSYSVERSION_ITEM6_STR].colorT[0]);

    //FLASH 版本号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM7_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM7_STR].ypis + rect[RECT_FTSYSVERSION_ITEM7_STR].ysize / 2 - 12,
              flashver, 4, rect[RECT_FTSYSVERSION_ITEM7_STR].colorT[0]);

    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM7_STR].xpis + SAFE_DIST4 + 13 * 6,
              rect[RECT_FTSYSVERSION_ITEM7_STR].ypis + rect[RECT_FTSYSVERSION_ITEM7_STR].ysize / 2 - 12,
              flashdate, 8, rect[RECT_FTSYSVERSION_ITEM7_STR].colorT[0]);

    //MP3 版本号
    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM8_STR].xpis + SAFE_DIST4,
              rect[RECT_FTSYSVERSION_ITEM8_STR].ypis + rect[RECT_FTSYSVERSION_ITEM8_STR].ysize / 2 - 12,
              mp3dver, 4, rect[RECT_FTSYSVERSION_ITEM8_STR].colorT[0]);

    DrawMFU32(rect[RECT_FTSYSVERSION_ITEM8_STR].xpis + SAFE_DIST4 + 13 * 6,
              rect[RECT_FTSYSVERSION_ITEM8_STR].ypis + rect[RECT_FTSYSVERSION_ITEM8_STR].ysize / 2 - 12,
              mp3ddate, 8, rect[RECT_FTSYSVERSION_ITEM8_STR].colorT[0]);

    //读系WIFI版本号
    Read_D(D_Address[SYS] + SYS_WIFI_VER, 1, (U16 *)(&YREG[SYS_WIFI_VER]));
    if (YREG[SYS_WIFI_VER] != 0)
    {
        DrawMFU32(rect[RECT_FTSYSVERSION_ITEM9_STR].xpis + SAFE_DIST4,
                  rect[RECT_FTSYSVERSION_ITEM9_STR].ypis + rect[RECT_FTSYSVERSION_ITEM9_STR].ysize / 2 - 12,
                  YREG[SYS_WIFI_VER], 4, rect[RECT_FTSYSVERSION_ITEM9_STR].colorT[0]);
    }

    for (i = 0; i < RECT_FTSYSVERSION_ITEM_MAX; i++)
    {
        f = i * 2 + RECT_FTSYSVERSION_ITEM1_TIT;
        if (enable[i])
        {
            DrawEditTit(rect[f], rect[f].index[0], SheetPage, LEFT, STR24X12, SAFE_DIST4);
        }
        else
        {
            DrawFillRect(rect[f].xpis, rect[f].ypis, rect[f].xsize, rect[f].ysize, rect[f].colorB[1]);
            Draw24x24Str(rect[f].xpis + SAFE_DIST4,
                         rect[f].ypis + (rect[f].ysize - 24) / 2,
                         (MEM_TITLE_ADD + (U32)(rect[f].index[0] * TitleLengthMax)),
                         SheetPage, rect[f].index[0], rect[f].colorT[0]); //画出方框的文字
        }
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
                if (keyon == KEY_FTSYSVERSION_SCRNSVR)
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

        if (keyoff)
        {
            switch (keyoff)
            {
            case KEY_FTSYSVERSION_SCRNSVR:
                pno = MENU_SCRNSVR;
                break;
            case KEY_FTSYSVERSION_BACK:
                // pno = MENU_FUNCTION;
                // pnoindex = 1;
                pno = poldno[--pnoindex];
                break;
            }

            keyoff = 0;
        }

        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FTSYSVERSION_SCRNSVR) //不需要显示的按键
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

        if (pno != MENU_FTSYSVERSION)
        {
            plastno = MENU_FTSYSVERSION;
            break;
        }
        wdt();
    }
#endif
}

//---------------------------------------------------
#define KEY_FTSCREENSET_SCRNSVR 1          //1 屏保
#define KEY_FTSCREENSET_BACK 2             //2 返回
#define KEY_FTSCREENSET_ENTER 3            //3 确认
#define KEY_FTSCREENSET_SETTIME 4          //4 设置时间
#define KEY_FTSCREENSET_CALIBRATION 5      //5 触摸校准设置
#define KEY_FTSCREENSET_BOOTSET 6          //6 开机画面设置
#define KEY_FTSCREENSET_BACKLIGHTUP 7      //7 屏保时间+
#define KEY_FTSCREENSET_BACKLIGHTDN 8      //8 屏保时间-
#define KEY_FTSCREENSET_BACKLIGHTINVALID 9 //9 屏保时间有效
#define KEY_FTSCREENSET_BACKLIGHTVALID 10  //10 屏保时间无效
#define KEY_FTSCREENSET_KEYLOCKINVALID 11  //11 按键锁有效
#define KEY_FTSCREENSET_KEYLOCKVALID 12    //12 按键锁无效
#define KEY_FTSCREENSET_VOICEINVALID 13    //13 语音有效
#define KEY_FTSCREENSET_VOICEVALID 14      //14 语音无效
#define KEY_FTSCREENSET_LCDCOLUP 15        //15 LCD背景色+
#define KEY_FTSCREENSET_LCDCOLDOWN 16      //16 LCD背景色-
#define KEY_FTSCREENSET_BRIGHTNESSUP 17    //17 背光亮度+
#define KEY_FTSCREENSET_BRIGHTNESSDN 18    //18 背光亮度-
#define KEY_FTSCREENSET_KEYSENLUP 19       //19 按键灵敏度+
#define KEY_FTSCREENSET_KEYSENLDOWN 20     //20 按键灵敏度-

#define KEY_FTSCREENSET1_BACK 21  //20 返回
#define KEY_FTSCREENSET1_ENTER 22 //21 确认
#define KEY_FTSCREENSET1_UP 23    //22 +1
#define KEY_FTSCREENSET1_DOWNN 24 //23 -1
#define KEY_FTSCREENSET1_0 25
#define KEY_FTSCREENSET1_1 26
#define KEY_FTSCREENSET1_2 27
#define KEY_FTSCREENSET1_3 28
#define KEY_FTSCREENSET1_4 29
#define KEY_FTSCREENSET1_5 30
#define KEY_FTSCREENSET1_6 31
#define KEY_FTSCREENSET1_7 32
#define KEY_FTSCREENSET1_8 33
#define KEY_FTSCREENSET1_9 34
#define KEY_FTSCREENSET1_YEAR 35  //35 年
#define KEY_FTSCREENSET1_MONTH 36 //36 月
#define KEY_FTSCREENSET1_DAY 37   //37 日
#define KEY_FTSCREENSET1_HOUR 38  //38 时
#define KEY_FTSCREENSET1_MIN 39   //39 分
// #define KEY_FTSCREENSET_CHSYSTEM 38 //38 选择系统
// #define KEY_FTSCREENSET_BOOTSET 39 //触摸按键设置

#define RECT_FTSCREENSET_ITEM1_TIT 1
#define RECT_FTSCREENSET_ITEM1_PARA 2
#define RECT_FTSCREENSET_ITEM2_TIT 3
#define RECT_FTSCREENSET_ITEM2_PARA 4
#define RECT_FTSCREENSET_ITEM3_TIT 5
#define RECT_FTSCREENSET_ITEM3_PARA 6
#define RECT_FTSCREENSET_ITEM4_TIT 7
#define RECT_FTSCREENSET_ITEM4_PARA 8
#define RECT_FTSCREENSET_ITEM5_TIT 9
#define RECT_FTSCREENSET_ITEM5_PARA 10
#define RECT_FTSCREENSET_ITEM6_TIT 11
#define RECT_FTSCREENSET_ITEM6_PARA 12

#define RECT_FTSCREENSET1_TIMITEM1_ALL 13
#define RECT_FTSCREENSET1_TIMITEM1_TIT 14
#define RECT_FTSCREENSET1_TIMITEM1_STR 15
#define RECT_FTSCREENSET1_TIMITEM1_YEAR 16
#define RECT_FTSCREENSET1_TIMITEM1_MONTH 17
#define RECT_FTSCREENSET1_TIMITEM1_DAY 18
#define RECT_FTSCREENSET1_TIMITEM1_HOUR 19
#define RECT_FTSCREENSET1_TIMITEM1_MIN 20
#define RECT_FTSCREENSET1_TIMITEM2_ALL 21
#define RECT_FTSCREENSET1_TIMITEM2_TIT 22
#define RECT_FTSCREENSET1_TIMITEM2_STR 23
#define RECT_FTSCREENSET1_TIMITEM2_YEAR 24
#define RECT_FTSCREENSET1_TIMITEM2_MONTH 25
#define RECT_FTSCREENSET1_TIMITEM2_DAY 26
#define RECT_FTSCREENSET1_TIMITEM2_HOUR 27
#define RECT_FTSCREENSET1_TIMITEM2_MIN 28

//------------------------------------------
// Function   : void Menu_FtScreenset(void)
// Description: 功能画面_显示设置
//------------------------------------------
void Menu_FtScreenset(void)
{
#if 1
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U32 i;
    U8 oldRunstepf = 0xff; //界面序号缓存
    U8 dis_updata = 1,
       dis_updata1 = 0,  //绘制自动关闭背光的参数数值
        dis_updata2 = 0, //绘制按键锁开关按钮
        dis_updata3 = 0, //显示触摸灵敏度数值
        dis_updata4 = 0, //绘制语音开关按钮
        dis_updata5 = 0, //显示背景颜色数值
        dis_updata6 = 0, //显示屏幕亮度数值
        dis_updata7 = 0, //显示时间设置数值
        dis_updata8 = 0; //显示屏保时间数值

    U16 BackLightStaus = P_LOF;            //屏保开关
    U16 BackLightVal = P_R / 60;           //屏保时间
    U16 LcdContrastVal = P_CNR;            //屏幕亮度
    U16 LcdContrastVal_Old = P_CNR;        //屏幕亮度
    U8 temp_KeyLockStaus = g_KeyLockStaus; //按键锁标志
    U16 KeyHelpStaus = P_LHP;              //按键说明显示功能
    U16 VoiceStaus = P_VOI;                //语音功能开关
    U16 ScreenColorVal = P_KNO;            //背景寒色选择
    S16 KeySenVal = gPeessVal;             //按键灵敏度设置
    //U16 ScreenColorStaus = P_LNN;
    U8 f = 0;
    U8 old_minute = 0;
    S8 temp_timer[5]; //时间设置缓存,年,月,日,时,分
    S8 Timer;         //当前设置时间的数据输入缓存
    U8 select_no = 0; //时间设置选择框
    // U32 tmptimes = 0;

    U8 bootSet = 0;
    U8 initSrc = 0;
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                             //1	屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},          //2 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},         //3 确认
        {TYPE_X4, TYPE_Y4, KEY_X16, KEY_Y16},        //4 时间设置
        {TYPE_X6, TYPE_Y4, KEY_X16, KEY_Y16},        //5 触摸校准
        {TYPE_X8, TYPE_Y4, KEY_X16, KEY_Y16},        //5 开机画面设置
        {392, TYPE_Y2 + SAFE_DIST4, KEY_X2, KEY_Y2}, //6 屏保时间+
        {524, TYPE_Y2 + SAFE_DIST4, KEY_X2, KEY_Y2}, //7 屏保时间-

        {608, TYPE_Y2 + SAFE_DIST15, KEY_X11, KEY_Y11},                                            //8 屏保时间有效
        {608 + KEY_X11, TYPE_Y2 + SAFE_DIST15, KEY_X11, KEY_Y11},                                  //9 屏保时间无效
        {608, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) + SAFE_DIST15, KEY_X11, KEY_Y11},               //10 按键锁有效
        {608 + KEY_X11, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) + SAFE_DIST15, KEY_X11, KEY_Y11},     //11 按键锁无效
        {608, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) * 2 + SAFE_DIST15, KEY_X11, KEY_Y11},           //12 语音有效
        {608 + KEY_X11, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) * 2 + SAFE_DIST15, KEY_X11, KEY_Y11}, //13 语音无效

        {392, TYPE_Y2 + SAFE_DIST4 + (TYPE_SIZE35 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //14 LCD背景色+
        {524, TYPE_Y2 + SAFE_DIST4 + (TYPE_SIZE35 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //15 LCD背景色-
        {392, TYPE_Y2 + SAFE_DIST4 + (TYPE_SIZE35 + SAFE_DIST5) * 4, KEY_X2, KEY_Y2}, //16 背光亮度+
        {524, TYPE_Y2 + SAFE_DIST4 + (TYPE_SIZE35 + SAFE_DIST5) * 4, KEY_X2, KEY_Y2}, //17 背光亮度-
        {392, TYPE_Y2 + SAFE_DIST4 + (TYPE_SIZE35 + SAFE_DIST5) * 5, KEY_X2, KEY_Y2}, //18 按键灵敏度+
        {524, TYPE_Y2 + SAFE_DIST4 + (TYPE_SIZE35 + SAFE_DIST5) * 5, KEY_X2, KEY_Y2}, //19 按键灵敏度-

        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //20 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //21 确认
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},
        {TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13, KEY_X2, KEY_Y2},
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13, KEY_X2, KEY_Y2},
        {TYPE_X15, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},

        {TYPE_X18 + SAFE_DIST10, TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE10, TYPE_SIZE5},                                                                           //34 年
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8), TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE5},                                               //35 月
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5), TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE5},                   //36 日
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5) + SAFE_DIST14, TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE5},     //37 时
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5) * 2 + SAFE_DIST14, TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE5}, //38 分

    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},                       //1 屏保
        {{KEY_Silver}, {0}, {72}, {TLFNADD_FUN_SCREEN + 1, 0, 0, 0}, {0}}, //2 返回
        {{KEY_Silver}, {0}, {71}, {TLFNADD_FUN_SCREEN + 2, 0, 0, 0}, {0}}, //3 确认

        {{KEY_Silver}, {0}, {698}, {TLFNADD_FUN_SCREEN + 3, 0, 0, 0}, {0}}, //4 时间设置
        {{KEY_Silver}, {0}, {699}, {TLFNADD_FUN_SCREEN + 4, 0, 0, 0}, {0}}, //5 触摸校准
        {{KEY_Silver}, {0}, {700}, {TLFNADD_FUN_SCREEN + 4, 0, 0, 0}, {0}}, //5 开机画面设置

        {{KEY_Silver}, {0}, {199}, {TLFNADD_FUN_SCREEN + 5, 0, 0, 0}, {0}},                                         //6 屏保时间+
        {{KEY_Silver}, {0}, {201}, {TLFNADD_FUN_SCREEN + 6, 0, 0, 0}, {0}},                                         //7 屏保时间-
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {861, 0, 862}, {TLFNADD_FUN_SCREEN + 7}, {TLFNADD_FUN_SCREEN + 7}}, //8 屏保时间有效
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {863, 0, 864}, {TLFNADD_FUN_SCREEN + 8}, {TLFNADD_FUN_SCREEN + 8}}, //9 屏保时间无效
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {861, 0, 862}, {TLFNADD_FUN_SCREEN + 7}, {TLFNADD_FUN_SCREEN + 7}}, //10 按键锁有效
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {863, 0, 864}, {TLFNADD_FUN_SCREEN + 8}, {TLFNADD_FUN_SCREEN + 8}}, //11 按键锁无效
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {861, 0, 862}, {TLFNADD_FUN_SCREEN + 7}, {TLFNADD_FUN_SCREEN + 7}}, //12 语音有效
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {863, 0, 864}, {TLFNADD_FUN_SCREEN + 8}, {TLFNADD_FUN_SCREEN + 8}}, //13 语音无效
        {{KEY_Silver}, {0}, {199}, {TLFNADD_FUN_SCREEN + 5, 0, 0, 0}, {0}},                                         //14 LCD背景色+
        {{KEY_Silver}, {0}, {201}, {TLFNADD_FUN_SCREEN + 6, 0, 0, 0}, {0}},                                         //15 LCD背景色-
        {{KEY_Silver}, {0}, {199}, {TLFNADD_FUN_SCREEN + 5, 0, 0, 0}, {0}},                                         //16 背光亮度+
        {{KEY_Silver}, {0}, {201}, {TLFNADD_FUN_SCREEN + 6, 0, 0, 0}, {0}},                                         //17 背光亮度-
        {{KEY_Silver}, {0}, {199}, {TLFNADD_FUN_SCREEN + 5, 0, 0, 0}, {0}},                                         //18 按键灵敏度+
        {{KEY_Silver}, {0}, {201}, {TLFNADD_FUN_SCREEN + 6, 0, 0, 0}, {0}},                                         //19 按键灵敏度-

        {{KEY_Silver}, {0}, {72}, {TLFNADD_FUN_SCREEN + 1, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {71}, {TLFNADD_FUN_SCREEN + 2, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {199}, {TLFNADD_FUN_SCREEN + 41, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {201}, {TLFNADD_FUN_SCREEN + 42, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {91}, {10, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {92}, {11, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {93}, {12, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {94}, {13, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {95}, {14, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {96}, {15, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {97}, {16, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {98}, {17, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {99}, {18, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {100}, {19, 0, 0, 0}, {0}},

        {{Color_White, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, 0}, {TLFNADD_FUN_SCREEN + 29}, {0}}, //年
        {{Color_White, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, 0}, {TLFNADD_FUN_SCREEN + 30}, {0}}, //月
        {{Color_White, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, 0}, {TLFNADD_FUN_SCREEN + 31}, {0}}, //日
        {{Color_White, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, 0}, {TLFNADD_FUN_SCREEN + 32}, {0}}, //时
        {{Color_White, Color_Black}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, 0}, {TLFNADD_FUN_SCREEN + 33}, {0}}, //分

    };

    const RECTPAD rect[] = {
        {0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FUN_SCREEN, TLFNADD_FUN_SCREEN + 26, TLFNADD_FUN_SCREEN + 34}},
        {TYPE_X18, TYPE_Y2, TYPE_SIZE34, TYPE_SIZE35, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 20}},
        {TYPE_X18 + 400, TYPE_Y2 + SAFE_DIST15, TYPE_SIZE9, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X18, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5), TYPE_SIZE34, TYPE_SIZE35, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 21}},
        {TYPE_X18 + 400, TYPE_Y2 + SAFE_DIST15 + (TYPE_SIZE35 + SAFE_DIST5), TYPE_SIZE9, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X18, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) * 2, TYPE_SIZE34, TYPE_SIZE35, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 22}},
        {TYPE_X18 + 400, TYPE_Y2 + SAFE_DIST15 + (TYPE_SIZE35 + SAFE_DIST5) * 2, TYPE_SIZE9, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X18, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) * 3, TYPE_SIZE34, TYPE_SIZE35, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 23}},
        {TYPE_X18 + 400, TYPE_Y2 + SAFE_DIST15 + (TYPE_SIZE35 + SAFE_DIST5) * 3, TYPE_SIZE9, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X18, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) * 4, TYPE_SIZE34, TYPE_SIZE35, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 24}},
        {TYPE_X18 + 400, TYPE_Y2 + SAFE_DIST15 + (TYPE_SIZE35 + SAFE_DIST5) * 4, TYPE_SIZE9, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},
        {TYPE_X18, TYPE_Y2 + (TYPE_SIZE35 + SAFE_DIST5) * 5, TYPE_SIZE34, TYPE_SIZE35, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 25}},
        {TYPE_X18 + 400, TYPE_Y2 + SAFE_DIST15 + (TYPE_SIZE35 + SAFE_DIST5) * 5, TYPE_SIZE9, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},

        {TYPE_X18 - 1, TYPE_Y10 - 1, TYPE_SIZE25 + 2, TYPE_SIZE11 + 2, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 27}},
        {TYPE_X18 + 1, TYPE_Y10 + 1, TYPE_SIZE25 - 2, TYPE_SIZE6, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_SCREEN + 27}},
        {TYPE_X18 + 1, TYPE_Y10 + 1 + TYPE_SIZE6, TYPE_SIZE25 - 2, TYPE_SIZE6, {Color_Silver}, {Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST5, TYPE_Y10 + 2 + TYPE_SIZE6, TYPE_SIZE10, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST5 + (TYPE_SIZE10 + SAFE_DIST8), TYPE_Y10 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST5 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5), TYPE_Y10 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST5 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5) + SAFE_DIST14, TYPE_Y10 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST5 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5) * 2 + SAFE_DIST14, TYPE_Y10 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},

        {TYPE_X18 - 1, TYPE_Y36 - 1, TYPE_SIZE25 + 2, TYPE_SIZE11 + 2, {Color_Black}, {Color_Black}, {TLFNADD_FUN_SCREEN + 28}},
        {TYPE_X18 + 1, TYPE_Y36 + 1, TYPE_SIZE25 - 2, TYPE_SIZE6, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_SCREEN + 28}},
        {TYPE_X18 + 1, TYPE_Y36 + 1 + TYPE_SIZE6, TYPE_SIZE25 - 2, TYPE_SIZE6, {Color_Silver}, {Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST10, TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE10, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8), TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5), TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5) + SAFE_DIST14, TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},
        {TYPE_X18 + SAFE_DIST10 + (TYPE_SIZE10 + SAFE_DIST8) + (TYPE_SIZE8 + TYPE_SIZE5) * 2 + SAFE_DIST14, TYPE_Y36 + 2 + TYPE_SIZE6, TYPE_SIZE8, TYPE_SIZE6, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},

    };
    if ((g_user.newuser > 7) && (g_user.newuser < 10))
    {
        bootSet = 1;
    }

    SetLanguage(MENU_FUNCTION);

    while (1)
    {
        if (dis_updata)
        {
            dis_updata = 0;

            if (oldRunstepf != g_runstepf)
            {
                oldRunstepf = g_runstepf;
                SetLanguage(MENU_FUNCTION);
                InitScreen();
                initViewStatus();
                updateViewStatus();
                Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[g_runstepf] * TitleLengthMax, SheetPage, rect[0].index[g_runstepf], rect[0].colorT[0]);
                ViewPage(usepageno);
            }

            if (g_runstepf == 0)
            {
                dis_updata1 = 1;
                dis_updata2 = 1;
                dis_updata3 = 1;
                dis_updata4 = 1;
                dis_updata5 = 1;
                dis_updata6 = 1;
                for (i = RECT_FTSCREENSET_ITEM1_TIT; i < RECT_FTSCREENSET_ITEM6_PARA; i += 2)
                {
                    DrawRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    Draw24x24Str(rect[i].xpis + SAFE_DIST5,
                                 rect[i].ypis + (rect[i].ysize - 24) / 2,
                                 (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                                 SheetPage, rect[i].index[0], rect[i].colorT[0]);
                }
                for (i = KEY_FTSCREENSET_BACK; i <= KEY_FTSCREENSET_CALIBRATION; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }
                if (bootSet)
                {
                    DrawButton(keyd[KEY_FTSCREENSET_BOOTSET], (U16 *)(ADD_ICONBMP), keybmpind[KEY_FTSCREENSET_BOOTSET].border[0], keybmpind[KEY_FTSCREENSET_BOOTSET].icon[0]);
                }

                for (i = KEY_FTSCREENSET_BACKLIGHTINVALID; i <= KEY_FTSCREENSET_KEYSENLDOWN; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }
            }
            else if (g_runstepf == 1) //按钮进入后的界面
            {
                dis_updata7 = 2;
                for (i = KEY_FTSCREENSET1_BACK; i <= KEY_FTSCREENSET1_9; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }
                for (i = RECT_FTSCREENSET1_TIMITEM2_ALL; i <= RECT_FTSCREENSET1_TIMITEM2_MIN; i += 8)
                {
                    f = i;
                    DrawRect(rect[f].xpis, rect[f].ypis, rect[f].xsize, rect[f].ysize, rect[f].colorB[0]);
                    f = i + 1;
                    DrawEditTit(rect[f], rect[f].index[0], SheetPage, LEFT, STR24X12, SAFE_DIST5);
                    f = i + 2;
                    BmpFillRect(rect[f].xpis, rect[f].ypis, rect[f].xsize, rect[f].ysize);
                    f = i + 3;
                    Draw24X24FStr(rect[f].xpis + rect[f].xsize + SAFE_DIST6, rect[f].ypis + rect[f].ysize / 2 - 12, ".", rect[f].colorT[0]);
                    f = i + 4;
                    Draw24X24FStr(rect[f].xpis + rect[f].xsize + SAFE_DIST6, rect[f].ypis + rect[f].ysize / 2 - 12, ".", rect[f].colorT[0]);
                    f = i + 6;
                    Draw24X24FStr(rect[f].xpis + rect[f].xsize + SAFE_DIST6, rect[f].ypis + rect[f].ysize / 2 - 12, ":", rect[f].colorT[0]);
                }
                Timer_ms = 30000;
                old_minute = 0xff;
            }
            else if (g_runstepf == 2)
            {
                DrawFillRect(0, TITLE_YLEN, LCD_WIDTH, LCD_HEIGHT - TITLE_YLEN, COLOR_BG);
                ChkTouch(TLFNADD_FUN_SCREEN);
                if ((poldno[1] == MENU_P1) && (pnoindex == 1))
                {
                    pno = MENU_P1;
                    pnoindex = 0;
                }
                else
                {
                    g_runstepf = 0;
                    dis_updata = 1;
                    if (g_user.newmode == 2)
                    {
                        g_user.newuser = g_user.olduser;
                        g_user.newmode = g_user.oldmode;
                        g_user.olduser = 0;
                        g_user.oldmode = 0;
                    }
                }
            }
        }

        if (g_runstepf == 0)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_FTSCREENSET_KEYSENLDOWN + 1, 0);
            if (keydata == KEY_FTSCREENSET_BOOTSET && bootSet == 0)
            {
                keydata = 0;
                resetKeyData();
            }
        }
        else if (g_runstepf == 1)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_FTSCREENSET1_MIN + 1, KEY_FTSCREENSET1_BACK);
        }

        //键值为按键松开时
        if (keydata == 0xff)
        {
            if ((keyon == KEY_FTSCREENSET_BACKLIGHTUP) || (keyon == KEY_FTSCREENSET_BACKLIGHTDN) ||
                (keyon == KEY_FTSCREENSET_LCDCOLUP) || (keyon == KEY_FTSCREENSET_LCDCOLDOWN) ||
                (keyon == KEY_FTSCREENSET_BRIGHTNESSUP) || (keyon == KEY_FTSCREENSET_BRIGHTNESSDN) ||
                (keyon == KEY_FTSCREENSET_KEYSENLUP || keyon == KEY_FTSCREENSET_KEYSENLDOWN) ||
                (keyon == KEY_FTSCREENSET1_UP) || (keyon == KEY_FTSCREENSET1_DOWNN))
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
                if (keyon == KEY_FTSCREENSET_SCRNSVR)
                {
                }
                else if (keyon >= KEY_FTSCREENSET1_YEAR && keyon <= KEY_FTSCREENSET1_MIN)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
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
                    case KEY_FTSCREENSET1_0:
                    case KEY_FTSCREENSET1_1:
                    case KEY_FTSCREENSET1_2:
                    case KEY_FTSCREENSET1_3:
                    case KEY_FTSCREENSET1_4:
                    case KEY_FTSCREENSET1_5:
                    case KEY_FTSCREENSET1_6:
                    case KEY_FTSCREENSET1_7:
                    case KEY_FTSCREENSET1_8:
                    case KEY_FTSCREENSET1_9:
                        DrawFMenuStrP(MENU_WRITE, keyon + NUMKEYPOS - KEY_FTSCREENSET1_0);
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
                if ((keyon == KEY_FTSCREENSET_BACKLIGHTUP) || (keyon == KEY_FTSCREENSET_BACKLIGHTDN) ||
                    (keyon == KEY_FTSCREENSET_LCDCOLUP) || (keyon == KEY_FTSCREENSET_LCDCOLDOWN) ||
                    (keyon == KEY_FTSCREENSET_BRIGHTNESSUP) || (keyon == KEY_FTSCREENSET_BRIGHTNESSDN) ||
                    (keyon == KEY_FTSCREENSET_KEYSENLUP || keyon == KEY_FTSCREENSET_KEYSENLDOWN) ||
                    (keyon == KEY_FTSCREENSET1_UP) || (keyon == KEY_FTSCREENSET1_DOWNN))
                {
                    keyoff = keyon;
                }
            }
        }
        else
        {
            key_on_flag1 = 0;
        }

        if (keyoff) //按键松开处理
        {
            if ((keyoff >= KEY_FTSCREENSET1_UP) && (keyoff <= KEY_FTSCREENSET1_9))
            {
                Timer = temp_timer[select_no];
                if (keyoff == KEY_FTSCREENSET1_UP)
                {
                    Timer++;
                    if (CailDate(select_no, Timer, temp_timer[0], temp_timer[1]) == 0)
                        Timer--;
                }
                else if (keyoff == KEY_FTSCREENSET1_DOWNN)
                {
                    Timer--;
                    if (CailDate(select_no, Timer, temp_timer[0], temp_timer[1]) == 0)
                        Timer++;
                }
                else
                {
                    Timer = (Timer % 10) * 10 + (keyoff - KEY_FTSCREENSET1_0);
                    if (CailDate(select_no, Timer, temp_timer[0], temp_timer[1]) == 0)
                    {
                        Timer = Timer % 10;
                        if (CailDate(select_no, Timer, temp_timer[0], temp_timer[1]) == 0)
                        {
                            if ((select_no == 1) || (select_no == 2))
                                Timer = 1;
                            else
                                Timer = 0;
                        }
                    }
                }
                temp_timer[select_no] = Timer;
                dis_updata7 = 1;
            }
            else if ((keyoff >= KEY_FTSCREENSET1_YEAR) && (keyoff <= KEY_FTSCREENSET1_MIN))
            {
                select_no = keyoff - KEY_FTSCREENSET1_YEAR;
                dis_updata7 = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTSCREENSET_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_FTSCREENSET_BACK:
                    pno = poldno[--pnoindex];
                    SetLcdPwm(1, P_CNR); //调节液晶屏对比度
                    break;
                case KEY_FTSCREENSET_BOOTSET:
                    pno = MENU_FTSCREENBOOTSET;
                    poldno[pnoindex++] = MENU_FTSCREENSET;
                    break;
                case KEY_FTSCREENSET_ENTER:
                    P_R = BackLightVal * 60;            //单位:秒
                    P_KNO = ScreenColorVal;             // Y
                    P_CNR = LcdContrastVal;             // Y
                    P_LOF = BackLightStaus;             // Y
                    g_KeyLockStaus = temp_KeyLockStaus; // Y
                    P_LHP = KeyHelpStaus;               // Y
                    P_VOI = VoiceStaus;                 // Y
                    P_CTR = (U16)KeySenVal;

                    SetLcdPwm(1, P_CNR); //调节液晶屏对比度
                    SaveBackLight(P_CNR);

                    for (i = 0; i < 7; i++)
                    {
                        Write_D((D_Address[PARA] + (146 + i) * 4 + 3), 1, (U16 *)&PREG[(146 + i) * 4 + 3]); //发送参数到主机
                    }
                    Write_D((D_Address[PARA] + 183 * 4 + 3), 1, (U16 *)&PREG[183 * 4 + 3]); //发送参数到主机
                    if (LcdContrastVal_Old != LcdContrastVal)
                    {
                        //SPI_SetSpeed(SPI_SPEED_HIGH);
                        //sysmemorydrive = W25Q1;
                        //EraseSector( BACKLIGHT_VAL_ADDR / 0x1000 );
                        //DelayMs(1000);
                        //WriteData( BACKLIGHT_VAL_ADDR, 4, (U32 *)(&P_CNR) );
                        //DelayMs(50);
                        //SPI_SetSpeed(SPI_SPEED_MIDDLE);
                    }
                    if (Write_Cmd(C_UPDATEPARA, 0, 0) == 0)
                    {
                        pno = MENU_RWHELP;
                        poldno[pnoindex++] = MENU_FTSCREENSET;
                        hleppara = 15;
                        break;
                    }
                    else
                    {
                        pno = poldno[--pnoindex];
                    }
                    gPeessVal = (U8)KeySenVal;
                    if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL)
                        gPeessVal = PEESS_DEfVAL;
                    WritePessVal(gPeessVal);
                    initSrc = 1;
                    break;
                case KEY_FTSCREENSET_SETTIME:
                    g_runstepf = 1;
                    dis_updata = 1;
                    break;
                case KEY_FTSCREENSET_CALIBRATION:
                    g_runstepf = 2;
                    if ((g_user.newuser > 5) && (g_user.newuser < 10))
                    {
                        dis_updata = 1;
                    }
                    else
                    {
                        pno = MENU_FTUSER;
                        poldno[pnoindex] = MENU_FTSCREENSET;
                    }
                    break;
                case KEY_FTSCREENSET_LCDCOLUP:
                    ScreenColorVal++;
                    if (ScreenColorVal > 4)
                        ScreenColorVal = 1;
                    dis_updata5 = 1;
                    break;
                case KEY_FTSCREENSET_LCDCOLDOWN:
                    if (ScreenColorVal < 2)
                        ScreenColorVal = 4;
                    else
                        ScreenColorVal--;
                    dis_updata5 = 1;
                    break;
                case KEY_FTSCREENSET_BRIGHTNESSUP:
                    if (LcdContrastVal < 99)
                        LcdContrastVal++;
                    SetLcdPwm(1, LcdContrastVal); //调节液晶屏对比度
                    dis_updata6 = 1;
                    break;
                case KEY_FTSCREENSET_BRIGHTNESSDN:
                    if (LcdContrastVal)
                        LcdContrastVal--;
                    SetLcdPwm(1, LcdContrastVal); //调节液晶屏对比度
                    dis_updata6 = 1;
                    break;
                case KEY_FTSCREENSET_BACKLIGHTUP:
                    if (BackLightVal < 9)
                    {
                        dis_updata8 = 1;
                        BackLightVal++;
                    }
                    else
                    {
                        BackLightVal = 9;
                    }
                    break;
                case KEY_FTSCREENSET_BACKLIGHTDN:
                    if (BackLightVal > 1)
                    {
                        dis_updata8 = 1;
                        BackLightVal--;
                    }
                    else
                    {
                        BackLightVal = 1;
                    }
                    break;
                case KEY_FTSCREENSET_BACKLIGHTVALID:
                    BackLightStaus = 0; // 自动关背光
                    dis_updata1 = 1;
                    break;
                case KEY_FTSCREENSET_BACKLIGHTINVALID:
                    BackLightStaus = 1;
                    dis_updata1 = 1;
                    break;
                case KEY_FTSCREENSET_KEYLOCKVALID:
                    temp_KeyLockStaus = 0; // 按键锁
                    dis_updata2 = 1;
                    break;
                case KEY_FTSCREENSET_KEYLOCKINVALID:
                    temp_KeyLockStaus = 1;
                    dis_updata2 = 1;
                    break;
                case KEY_FTSCREENSET_KEYSENLUP: //按键敏感度
                    KeySenVal++;
                    if (KeySenVal > PEESS_MAXVAL)
                        KeySenVal = PEESS_MINVAL;
                    dis_updata3 = 1;
                    break;
                case KEY_FTSCREENSET_KEYSENLDOWN:
                    KeySenVal--;
                    if (KeySenVal < PEESS_MINVAL)
                        KeySenVal = PEESS_MAXVAL;
                    dis_updata3 = 1;
                    break;
                case KEY_FTSCREENSET_VOICEVALID:
                    VoiceStaus = 0; // 语音提示开关
                    dis_updata4 = 1;
                    break;
                case KEY_FTSCREENSET_VOICEINVALID:
                    VoiceStaus = 1;
                    dis_updata4 = 1;
                    break;
                case KEY_FTSCREENSET1_BACK:
                    g_runstepf = 0;
                    dis_updata = 1;
                    break;
                case KEY_FTSCREENSET1_ENTER:
                    old_minute = 0xff;
                    Timer_ms = 29000;
                    //发通讯命令,修改系统时间
                    WriteTimer(C_SET_TIMER, (U8 *)(&temp_timer));
                    for (i = 0; i < RDTIMES; i++)
                    {
                        Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                        if (DREG[D_CMDSTATUS] == 100)                                          //成功
                        {
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] == 4) //没有权限设置时间
                        {
                            pno = MENU_RWHELP;
                            hleppara = 59;
                            poldno[pnoindex++] = MENU_FTSCREENSET;
                            break;
                        }
                        DelayMs(WAITTIME);
                    }
                    break;

                    // case KEY_FTSCREENSET_CHSYSTEM:
                    //     pno = MENU_RWHELP;
                    //     hleppara = 162;
                    //     poldno[pnoindex++] = MENU_FTSCREENSET;
                    //     break;
                }
            }
            keyoff = 0;
        }
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FTSCREENSET_SCRNSVR) //不需要显示的按键
            {
            }
            else if (keyon >= KEY_FTSCREENSET1_YEAR && keyon <= KEY_FTSCREENSET1_MIN)
            {
                DrawButtonOnZoomIn(keyd[keyon], 2);
            }
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

        // [绘制自动关闭背光的参数数值*begin]*****************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
            DrawButton(keyd[KEY_FTSCREENSET_BACKLIGHTINVALID], (U16 *)(ADD_ICONBMP),
                       (BackLightStaus == 1) ? keybmpind[KEY_FTSCREENSET_BACKLIGHTINVALID].border[2] : keybmpind[KEY_FTSCREENSET_BACKLIGHTINVALID].border[0],
                       (BackLightStaus == 1) ? keybmpind[KEY_FTSCREENSET_BACKLIGHTINVALID].icon[2] : keybmpind[KEY_FTSCREENSET_BACKLIGHTINVALID].icon[0]);
            DrawButton(keyd[KEY_FTSCREENSET_BACKLIGHTVALID], (U16 *)(ADD_ICONBMP),
                       (BackLightStaus == 0) ? keybmpind[KEY_FTSCREENSET_BACKLIGHTVALID].border[2] : keybmpind[KEY_FTSCREENSET_BACKLIGHTVALID].border[0],
                       (BackLightStaus == 0) ? keybmpind[KEY_FTSCREENSET_BACKLIGHTVALID].icon[2] : keybmpind[KEY_FTSCREENSET_BACKLIGHTVALID].icon[0]);
            if (BackLightStaus)
            {
                for (i = KEY_FTSCREENSET_BACKLIGHTUP; i <= KEY_FTSCREENSET_BACKLIGHTDN; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }
                dis_updata8 = 1;
            }
            else
            {
                BmpFillRect(keyd[KEY_FTSCREENSET_BACKLIGHTUP].xpis, keyd[KEY_FTSCREENSET_BACKLIGHTUP].ypis, keyd[KEY_FTSCREENSET_BACKLIGHTUP].xsize, keyd[KEY_FTSCREENSET_BACKLIGHTUP].ysize);
                BmpFillRect(keyd[KEY_FTSCREENSET_BACKLIGHTDN].xpis, keyd[KEY_FTSCREENSET_BACKLIGHTDN].ypis, keyd[KEY_FTSCREENSET_BACKLIGHTDN].xsize, keyd[KEY_FTSCREENSET_BACKLIGHTDN].ysize);
                BmpFillRect(rect[RECT_FTSCREENSET_ITEM1_PARA].xpis, rect[RECT_FTSCREENSET_ITEM1_PARA].ypis, rect[RECT_FTSCREENSET_ITEM1_PARA].xsize, rect[RECT_FTSCREENSET_ITEM1_PARA].ysize);
            }
        }
        // [绘制自动关闭背光的参数数值*end]*****************************

        // [绘制按键锁开关按钮*begin]**********************************
        if (dis_updata2)
        {
            dis_updata2 = 0;
            DrawButton(keyd[KEY_FTSCREENSET_KEYLOCKVALID], (U16 *)(ADD_ICONBMP),
                       (temp_KeyLockStaus == 0) ? keybmpind[KEY_FTSCREENSET_KEYLOCKVALID].border[2] : keybmpind[KEY_FTSCREENSET_KEYLOCKVALID].border[0],
                       (temp_KeyLockStaus == 0) ? keybmpind[KEY_FTSCREENSET_KEYLOCKVALID].icon[2] : keybmpind[KEY_FTSCREENSET_KEYLOCKVALID].icon[0]);
            DrawButton(keyd[KEY_FTSCREENSET_KEYLOCKINVALID], (U16 *)(ADD_ICONBMP),
                       (temp_KeyLockStaus == 1) ? keybmpind[KEY_FTSCREENSET_KEYLOCKINVALID].border[2] : keybmpind[KEY_FTSCREENSET_KEYLOCKINVALID].border[0],
                       (temp_KeyLockStaus == 1) ? keybmpind[KEY_FTSCREENSET_KEYLOCKINVALID].icon[2] : keybmpind[KEY_FTSCREENSET_KEYLOCKINVALID].icon[0]);
        }
        // [绘制按键锁开关按钮*end]************************************

        // [显示触摸灵敏度数值*begin]*************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;
            DrawFillRect(rect[RECT_FTSCREENSET_ITEM6_PARA].xpis, rect[RECT_FTSCREENSET_ITEM6_PARA].ypis,
                         rect[RECT_FTSCREENSET_ITEM6_PARA].xsize, rect[RECT_FTSCREENSET_ITEM6_PARA].ysize, rect[RECT_FTSCREENSET_ITEM6_PARA].colorB[0]);
            DrawMFU32(rect[RECT_FTSCREENSET_ITEM6_PARA].xpis + rect[RECT_FTSCREENSET_ITEM6_PARA].xsize / 2 - 12,
                      rect[RECT_FTSCREENSET_ITEM6_PARA].ypis + rect[RECT_FTSCREENSET_ITEM6_PARA].ysize / 2 - 12,
                      KeySenVal, 2, rect[RECT_FTSCREENSET_ITEM6_PARA].colorT[0]);
        }
        // [显示触摸灵敏度数值*end]*************************************

        // [绘制语音开关按钮*begin]***************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;
            DrawButton(keyd[KEY_FTSCREENSET_VOICEVALID], (U16 *)(ADD_ICONBMP),
                       (VoiceStaus == 0) ? keybmpind[KEY_FTSCREENSET_VOICEVALID].border[2] : keybmpind[KEY_FTSCREENSET_VOICEVALID].border[0],
                       (VoiceStaus == 0) ? keybmpind[KEY_FTSCREENSET_VOICEVALID].icon[2] : keybmpind[KEY_FTSCREENSET_VOICEVALID].icon[0]);
            DrawButton(keyd[KEY_FTSCREENSET_VOICEINVALID], (U16 *)(ADD_ICONBMP),
                       (VoiceStaus == 1) ? keybmpind[KEY_FTSCREENSET_VOICEINVALID].border[2] : keybmpind[KEY_FTSCREENSET_VOICEINVALID].border[0],
                       (VoiceStaus == 1) ? keybmpind[KEY_FTSCREENSET_VOICEINVALID].icon[2] : keybmpind[KEY_FTSCREENSET_VOICEINVALID].icon[0]);
        }
        // [绘制语音开关按钮*end]***************************************

        // [显示背景颜色数值*begin]***************************************
        if (dis_updata5)
        {
            dis_updata5 = 0;
            DrawFillRect(rect[RECT_FTSCREENSET_ITEM4_PARA].xpis, rect[RECT_FTSCREENSET_ITEM4_PARA].ypis,
                         rect[RECT_FTSCREENSET_ITEM4_PARA].xsize, rect[RECT_FTSCREENSET_ITEM4_PARA].ysize, rect[RECT_FTSCREENSET_ITEM4_PARA].colorB[0]);
            DrawMFU32(rect[RECT_FTSCREENSET_ITEM4_PARA].xpis + rect[RECT_FTSCREENSET_ITEM4_PARA].xsize / 2 - 6,
                      rect[RECT_FTSCREENSET_ITEM4_PARA].ypis + rect[RECT_FTSCREENSET_ITEM4_PARA].ysize / 2 - 12,
                      ScreenColorVal, 1, rect[RECT_FTSCREENSET_ITEM4_PARA].colorT[0]);
        }
        // [显示背景颜色数值*end]***************************************

        // [显示屏幕亮度数值*begin]***************************************
        if (dis_updata6)
        {
            dis_updata6 = 0;
            DrawFillRect(rect[RECT_FTSCREENSET_ITEM5_PARA].xpis, rect[RECT_FTSCREENSET_ITEM5_PARA].ypis,
                         rect[RECT_FTSCREENSET_ITEM5_PARA].xsize, rect[RECT_FTSCREENSET_ITEM5_PARA].ysize, rect[RECT_FTSCREENSET_ITEM5_PARA].colorB[0]);
            DrawMFU32(rect[RECT_FTSCREENSET_ITEM5_PARA].xpis + rect[RECT_FTSCREENSET_ITEM5_PARA].xsize / 2 - 12,
                      rect[RECT_FTSCREENSET_ITEM5_PARA].ypis + rect[RECT_FTSCREENSET_ITEM5_PARA].ysize / 2 - 12,
                      LcdContrastVal, 2, rect[RECT_FTSCREENSET_ITEM5_PARA].colorT[0]);
        }
        // [显示屏幕亮度数值*begin]***************************************

        // [显示屏保时间数值*begin]***************************************
        if (dis_updata8)
        {
            dis_updata8 = 0;
            if (BackLightStaus)
            {
                DrawFillRect(rect[RECT_FTSCREENSET_ITEM1_PARA].xpis, rect[RECT_FTSCREENSET_ITEM1_PARA].ypis,
                             rect[RECT_FTSCREENSET_ITEM1_PARA].xsize, rect[RECT_FTSCREENSET_ITEM1_PARA].ysize, rect[RECT_FTSCREENSET_ITEM1_PARA].colorB[0]);
                DrawMFU32(rect[RECT_FTSCREENSET_ITEM1_PARA].xpis + SAFE_DIST8, rect[RECT_FTSCREENSET_ITEM1_PARA].ypis + rect[RECT_FTSCREENSET_ITEM1_PARA].ysize / 2 - 12,
                          BackLightVal, 1, rect[RECT_FTSCREENSET_ITEM1_PARA].colorT[0]);
                Draw24X24FStr(rect[RECT_FTSCREENSET_ITEM1_PARA].xpis + rect[RECT_FTSCREENSET_ITEM1_PARA].xsize - SAFE_DIST8,
                              rect[RECT_FTSCREENSET_ITEM1_PARA].ypis + rect[RECT_FTSCREENSET_ITEM1_PARA].ysize / 2 - 12,
                              "M", rect[RECT_FTSCREENSET_ITEM1_PARA].colorT[0]);
            }
        }
        // [显示屏保时间数值*begin]***************************************

#if 0
        if (g_runstepf == 1)
        {
            if (Timer_ms >= 30000)
            {
                Timer_ms = 0;

                //读系统时间
                Read_D(D_Address[SYS] + SYS_MS, 4, (U16 *)(&YREG[SYS_MS])); // 读秒分
                g_SysTimes.minute = BCD_2Hex((U8)(YREG[SYS_MS] >> 8));

                if (old_minute != g_SysTimes.minute)
                { //每分钟刷新一次时间
                    old_minute = g_SysTimes.minute;

                    g_SysTimes.hour = BCD_2Hex((U8)(YREG[SYS_WH] & 0x007f));
                    g_SysTimes.day = BCD_2Hex((U8)(YREG[SYS_MD] & 0x00ff));
                    g_SysTimes.month = BCD_2Hex((U8)(YREG[SYS_MD] >> 8));
                    g_SysTimes.year = BCD_2Hex((U8)(YREG[SYS_Y] & 0x00ff));

                    //刷新显示当前时间
                    BmpFillRect(16, 128, 64, 32);
                    DrawMFU32(19, 128, 2000 + g_SysTimes.year, 4, Color_Black); //年
                    BmpFillRect(96, 128, 32, 32);
                    DrawMFU32(99, 128, g_SysTimes.month, 2, Color_Black); //月
                    BmpFillRect(144, 128, 32, 32);
                    DrawMFU32(147, 128, g_SysTimes.day, 2, Color_Black); //日
                    BmpFillRect(224, 128, 32, 32);
                    DrawMFU32(227, 128, g_SysTimes.hour, 2, Color_Black); //时
                    BmpFillRect(272, 128, 32, 32);
                    DrawMFU32(275, 128, g_SysTimes.minute, 2, Color_Black); //分
                }
            }
            i = Timer_ms / 1000;
            if (tmptimes != i)
            {
                tmptimes = i;
                if (f)
                {
                    f = 0;
                    Draw24X24FStr(256, 128, ":", Color_Black);
                }
                else
                {
                    f = 1;
                    BmpFillRect(256, 128, 13, 24);
                }
            }
        }
#endif
        // [显示时间设置数值 * begin]************************************
        if (dis_updata7)
        {
            if (dis_updata7 == 2)
            {
                temp_timer[0] = g_SysTimes.year;
                if (temp_timer[0] > 99)
                {
                    temp_timer[0] = 0;
                }
                temp_timer[1] = g_SysTimes.month;
                temp_timer[2] = g_SysTimes.day;
                temp_timer[3] = g_SysTimes.hour;
                temp_timer[4] = g_SysTimes.minute;
            }
            for (i = 0; i < 5; i++)
            {
                f = KEY_FTSCREENSET1_YEAR + i;
                DrawButtonEdit(keyd[f],
                               (i == select_no) ? keybmpind[f].border[1] : keybmpind[f].border[0],
                               (i == select_no) ? keybmpind[f].icon[1] : keybmpind[f].icon[0]);
                if (i == 0)
                {
                    DrawMFU32(keyd[f].xpis + keyd[f].xsize / 2 - 24, keyd[f].ypis + keyd[f].ysize / 2 - 12, 2000 + temp_timer[i], 4, (i == select_no) ? keybmpind[f].textColor[1] : keybmpind[f].textColor[0]);
                }
                else
                {
                    DrawMFU32(keyd[f].xpis + keyd[f].xsize / 2 - 12, keyd[f].ypis + keyd[f].ysize / 2 - 12, temp_timer[i], 2, (i == select_no) ? keybmpind[f].textColor[1] : keybmpind[f].textColor[0]);
                }
            }
            dis_updata7 = 0;
        }
        // [显示时间设置数值 * end]************************************

        if (pno != MENU_FTSCREENSET)
        {
#if USEMULTCOLOR
            if (initSrc)
                Menu_DesktopInit();
#endif
            plastno = MENU_FTSCREENSET;
            break;
        }
        wdt();
    }
#endif
}

//-------------------------------------------------------
#define KEY_FTBOOTSCR_SCRNSVR 1
#define KEY_FTBOOTSCR_BACK 2
#define KEY_FTBOOTSCR_ENTER 3
#define KEY_FTBOOTSCR_MODE 4
#define KEY_FTBOOTSCR_INPORT 5
#define KEY_FTBOOTSCR_VMLK 6
#define KEY_FTBOOTSCR_NVMLK 7
//------------------------------------------
// Function   : void Mit_Menu_FtBootScreen(void)
// Description: 功能画面_设置开机画面
//------------------------------------------
void Menu_FtBootScreen(void)
{
#if 0
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U32 i;
    U16 vnums;
    U8 dis_updata = 1;
    U8 btmode = 0, btmax = 1; //0-默认 1-自定义
    U8 btani = 0;             //0-tf卡无开机动画 1-tf卡有开机动画
    U8 vmlk = 0, ovmlk = 0xff;
    const U8 keynum = {8};
    const KEYPAD keyd[] = {{0, 0, 0, 0}, {0, 0, 128, 32},
     {1, 735, KEYX1, KEYY1}, {535, 735, KEYX1, KEYY1}, 
     {10, 180, KEYX1, KEYY1}, {10, 400, KEYX1, KEYY1},
      {100, 250, 220, 56}, {370, 250, 220, 56}};
    const U16 keybmpind[] = {0, 0, 1190, 70, 1262, 133};

    SetLanguage(MENU_FUNCTION);
    InitScreen();
    Draw24x24Str(TITLE_XPOS_MIT, 4, MEM_TITLE_ADD + TLFNADD_BOOTSET * TitleLengthMax, SheetPage, TLFNADD_BOOTSET, Color_White);
    //printf("Mit_Menu_FtBootScreen \r\n");
    gplayLogoAvi = 1;
    btmax = getBootScreenMxg() + 1;
    //btmax = 3;
    btani = Check_AviFile((char *)"logo.avi") == 0 ? 1 : 0;
    //printf("btani = %d \r\n",btani);
    //SETWCS1();									//取消片选
    //SETWCS2();									//取消片选
    //SETWCS3();									//取消片选
    //SETTFCS();									//取消片选

    //btani =  Check_AviFile((char*)"logo.avi");
    //printf("btani = %d \r\n",btani);
    gplayLogoAvi = 0;

    btmode = g_bootscr.mode;
    vmlk = g_bootscr.vmlk;
    DrawDFrame(4, 140, 592, 180, 1, Color_Gray);
    DrawDFrame(4, 360, 592, 120, 1, Color_Gray);
    Draw24x24Str(10, 146, MEM_TITLE_ADD + (TLFNADD_BOOTSET + 9) * TitleLengthMax, SheetPage, TLFNADD_BOOTSET + 9, Color_Black);           //选择开机画面
    vnums = Draw24x24Str(10, 366, MEM_TITLE_ADD + (TLFNADD_BOOTSET + 10) * TitleLengthMax, SheetPage, TLFNADD_BOOTSET + 10, Color_Black); //导入开机画面
    Draw24X24FStr(10 + vnums * 13 + 4, 366, "root:\\logo.jpg", Color_Red);
    for (i = 2; i < KEY_FTBOOTSCR_VMLK; i++)
    {
        DrawButton2(keyd[i], Color_White1, keybmpind[i], 1);
    }
    Draw24x24MulStr(130, 254, MEM_TITLE_ADD + (TLFNADD_BOOTSET + 11) * TitleLengthMax, 14, SheetPage, (TLFNADD_BOOTSET + 11), 2, Color_Black); //显示名菱信息
    Draw24x24MulStr(400, 254, MEM_TITLE_ADD + (TLFNADD_BOOTSET + 12) * TitleLengthMax, 14, SheetPage, (TLFNADD_BOOTSET + 12), 2, Color_Black); //屏蔽名菱信息

    ViewPage(usepageno);
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, keynum, 0);
        if (keydata == 0xff)
        {
            if ((keyon) && (beeponf))
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > 1)
        {
            if (keyon == 0)
            {
                keyon = keydata;
                if (keyon == KEY_FTBOOTSCR_SCRNSVR)
                    ;
                else
                {
                    if (keyon < KEY_FTBOOTSCR_VMLK)
                        DrawButtonOn(keyd[keydata], KEY_Silver);
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
                    if (keyon < KEY_FTBOOTSCR_VMLK)
                        DrawFMenuStr(keyon + TLFNADD_BOOTSET);
                }
                Times = 0;
            }
        }
        if (keyoff)
        {
            switch (keyoff)
            {
            case KEY_FTBOOTSCR_SCRNSVR:
                pno = MENU_SCRNSVR;
                break;
            case KEY_FTBOOTSCR_BACK:
                pno = poldno[--pnoindex];
                break;

            case KEY_FTBOOTSCR_ENTER:
                if (btmode != g_bootscr.mode || vmlk != g_bootscr.vmlk)
                {
                    g_bootscr.mode = btmode;
                    g_bootscr.vmlk = vmlk;
                    //printf("g_bootscr.mode = %d btmode = %d \r\n",g_bootscr.mode,btmode);
                    hleppara = 183;
                    pno = MENU_RWHELP;
                    poldno[pnoindex++] = MENU_FTSCREENBOOTSET;
                    break;
                }
                pno = poldno[--pnoindex];
                break;

            case KEY_FTBOOTSCR_MODE:
                if (btmax == 0)
                    break;
                if (btmode < (btmax + btani))
                {
                    if (btmode < btmax)
                        btmode++;
                    else
                        btmode = 3; //开机动画
                }
                else
                {
                    btmode = 0;
                }
                if (btmode)
                    vmlk = 0;
                else
                    vmlk = 1;

                dis_updata = 1;
                break;

            case KEY_FTBOOTSCR_INPORT:
                if (ExtMemory_Status(USBTF))
                { //查询打板器U盘
                    hleppara = 95;
                    pno = MENU_RWHELP;
                    poldno[pnoindex++] = MENU_FTSCREENBOOTSET;
                    break;
                }

                if (anyWriteScreeData() == 0)
                {
                    hleppara = 182;
                    pno = MENU_RWHELP;
                    poldno[pnoindex++] = MENU_FTSCREENBOOTSET;
                    break;
                }

                //if(btmax==0){
                WriteScreeData(100, 410, 436);
                if (btmode == 0)
                {
                    btmode = 1;
                    dis_updata = 1;
                }
                //hleppara = 183;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTSCREENBOOTSET;break;
                //}

                break;

            case KEY_FTBOOTSCR_VMLK:
                vmlk = 1;
                break;

            case KEY_FTBOOTSCR_NVMLK:
                vmlk = 0;
                break;
            }
            keyoff = 0;
        }
        if ((keyon) && (keydata == 0))
        {
            if (keyon == KEY_FTBOOTSCR_SCRNSVR)
                ;
            else if (keyon < KEY_FTBOOTSCR_VMLK)
            {
                DrawButtonOff2(keyd[keyon], Color_White1);
                BmpFillRect(TITLE_XPOS_MIT, 4, TITLE_XLEN - TITLE_XPOS_MIT, 24);
                Draw24x24Str(TITLE_XPOS_MIT, 4, MEM_TITLE_ADD + TLFNADD_BOOTSET * TitleLengthMax, SheetPage, TLFNADD_BOOTSET, Color_White);
            }
            keyon = 0;
            beeponf = 0;
        }

        if (dis_updata)
        {
            dis_updata = 0;
            BmpFillRect(100, 210, 480, 24);
            Draw24x24Str(100, 210, MEM_TITLE_ADD + (TLFNADD_BOOTSET + btmode + 13) * TitleLengthMax, SheetPage, (TLFNADD_BOOTSET + btmode + 13), Color_Blue);
        }

        if (vmlk != ovmlk)
        {
            if (vmlk)
            {
                DrawFPBMPBTN1(keyd[KEY_FTBOOTSCR_VMLK].xpis, keyd[KEY_FTBOOTSCR_VMLK].ypis + 6, 20, 20, 1069);
                DrawFPBMPBTN1(keyd[KEY_FTBOOTSCR_NVMLK].xpis, keyd[KEY_FTBOOTSCR_NVMLK].ypis + 6, 20, 20, 1068);
            }
            else
            {
                DrawFPBMPBTN1(keyd[KEY_FTBOOTSCR_VMLK].xpis, keyd[KEY_FTBOOTSCR_VMLK].ypis + 6, 20, 20, 1068);
                DrawFPBMPBTN1(keyd[KEY_FTBOOTSCR_NVMLK].xpis, keyd[KEY_FTBOOTSCR_NVMLK].ypis + 6, 20, 20, 1069);
            }
            ovmlk = vmlk;
        }
        if (pno != MENU_FTSCREENBOOTSET)
        {
            plastno = MENU_FTSCREENBOOTSET;
            break;
        }
        wdt();
    }
#endif
}

//-------------------------------------------------
#define KEY_FTSYSBACKUP_SCRNSVR 1
#define KEY_FTSYSBACKUP_BACK 2
#define KEY_FTSYSBACKUP_PARA 3
#define KEY_FTSYSBACKUP_IOSETTING 4
#define KEY_FTSYSBACKUP_RECORD 5
#define KEY_FTSYSBACKUP_MAX 6

// #define KEY_FTSYSBACKUP_SYSTEM 3

//------------------------------------------
// Function   : void Menu_FtSysbackup(void)
// Description: 功能画面_系统备份
// 说明：备份数据只保存在打板器的SD卡和U盘  暂不支持主板存储设备
//------------------------------------------
void Menu_FtSysbackup(void)
{
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U32 i, f;
    U8 SDFlag = 0, USBFlag = 0;
    U8 StorType = 0;
    U8 tmpLength = 0;
    const U8 limitLength = 16;

    const U8 keynum = {KEY_FTSYSBACKUP_MAX}; //
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                     //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 返回
        {TYPE_X90, TYPE_Y6, KEY_X9, KEY_Y9}, //3 参数选项1
        {TYPE_X91, TYPE_Y6, KEY_X9, KEY_Y9}, //4 参数选项2
        {TYPE_X92, TYPE_Y6, KEY_X9, KEY_Y9}, //5 参数选项3

    };
    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},                           //1 屏保
        {{KEY_Silver}, {0}, {72, 88}, {TLFNADD_FUN_BACKUP + 1, 0, 0, 0}, {0}}, //2 返回

        {{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_FUN_BACKUP + 3, 0, 0, 0}, {TLFNADD_FUN_BACKUP + 8}},  //3 选项1 参数备份
        {{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_FUN_BACKUP + 4, 0, 0, 0}, {TLFNADD_FUN_BACKUP + 9}},  //4 选项2 IO设定备份
        {{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_FUN_BACKUP + 5, 0, 0, 0}, {TLFNADD_FUN_BACKUP + 10}}, //5 选项3 运行信息备份

    };
    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FUN_BACKUP}},
    };
    SetLanguage(MENU_FUNCTION);
    InitScreen(); //清屏
    initViewStatus();
    updateViewStatus();

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0], SheetPage, rect[0].index[0], rect[0].colorT[0]);

    DrawButton(keyd[KEY_FTSYSBACKUP_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_FTSYSBACKUP_BACK].border[0], keybmpind[KEY_FTSYSBACKUP_BACK].icon[0]);

    for (i = 0; i < 3; i++)
    {
        f = KEY_FTSYSBACKUP_PARA + i;
        DrawButtonSingleColor(keyd[f], keybmpind[f].border[0], keybmpind[f].icon[0]);
        tmpLength = strlen((char *)(MEM_TITLE_ADD + keybmpind[f].nameStr[0] * TitleLengthMax));
        if (tmpLength > limitLength)
        {
            Draw24x24MulStr(keyd[f].xpis + keyd[f].xsize / 2 - limitLength * 12 / 2,
                            keyd[f].ypis + keyd[f].ysize / 2 - 50 / 2, (MEM_TITLE_ADD + (keybmpind[f].nameStr[0] * TitleLengthMax)),
                            limitLength, SheetPage, keybmpind[f].nameStr[0], 2, keybmpind[f].textColor[0]);
        }
        else
        {
            Draw24x24MulStr(keyd[f].xpis + keyd[f].xsize / 2 - tmpLength * 12 / 2,
                            keyd[f].ypis + keyd[f].ysize / 2 - 24 / 2, (MEM_TITLE_ADD + (keybmpind[f].nameStr[0] * TitleLengthMax)),
                            limitLength, SheetPage, keybmpind[f].nameStr[0], 2, keybmpind[f].textColor[0]);
        }
    }

    //查询外设接入情况
    //if(ExtMemory_Status(SD)==0)  {	SDFlag =1;}
    if (ExtMemory_Status(USBTF) == 0)
    {
        USBFlag = 1;
    }
    Timerms = 0;

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
                if (keyon == KEY_FTSYSBACKUP_SCRNSVR)
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
            case KEY_FTSYSBACKUP_SCRNSVR:
                pno = MENU_SCRNSVR;
                break;
            case KEY_FTSYSBACKUP_BACK:
                pno = poldno[--pnoindex];
                break;

            case KEY_FTSYSBACKUP_PARA:
                Read_D(D_Address[PARA], MAXPREG, (U16 *)(&PREG[0])); // 读取参数值
                StorType = 0;
                StorType = (SDFlag << 1) | USBFlag;

                if (StorType == 0x03) //USB && SD
                {
                    hleppara = 84;
                    g_para0 = 126;
                }
                else if ((StorType == 0x02) || (StorType == 0x01))
                { //SD	|| USB
                    if (StorType == 0x02)
                    {
                        hleppara = 127;
                    }
                    else
                    {
                        hleppara = 126;
                    }
                    g_disk_memory = StorType;
                }
                else
                {
                    hleppara = 95;
                }
                pno = MENU_RWHELP;
                poldno[pnoindex++] = MENU_FTSYSBACKUP;
                break;
            case KEY_FTSYSBACKUP_IOSETTING:
                Read_D(D_Address[IOPARA], 338, (U16 *)(&IOREG)); //读取IO参数
                StorType = 0;
                StorType = (SDFlag << 1) | USBFlag;

                if (StorType == 0x03) //USB && SD
                {
                    hleppara = 84;
                    g_para0 = 129;
                }
                else if ((StorType == 0x02) || (StorType == 0x01))
                { //SD	|| USB
                    if (StorType == 0x02)
                    {
                        hleppara = 130;
                    }
                    else
                    {
                        hleppara = 129;
                    }
                    g_disk_memory = StorType;
                }
                else
                {
                    hleppara = 95;
                }
                pno = MENU_RWHELP;
                poldno[pnoindex++] = MENU_FTSYSBACKUP;
                break;

            case KEY_FTSYSBACKUP_RECORD:
                StorType = 0;
                StorType = (SDFlag << 1) | USBFlag;

                if (StorType == 0x03) //USB && SD
                {
                    hleppara = 84;
                    g_para0 = 151;
                }
                else if ((StorType == 0x02) || (StorType == 0x01)) //SD	|| USB
                {
                    if (StorType == 0x02)
                    {
                        hleppara = 152;
                    }
                    else
                    {
                        hleppara = 151;
                    }
                    g_disk_memory = StorType;
                }
                else
                {
                    hleppara = 95;
                }
                pno = MENU_RWHELP;
                poldno[pnoindex++] = MENU_FTSYSBACKUP;
                break;
            }

            keyoff = 0;
        }
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FTSYSBACKUP_SCRNSVR) //不需要显示的按键
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

        if (Timerms > 1000) //每隔1s 查询外部存储设备
        {
            if (ExtMemory_Status(USBTF) == 0)
            {
                USBFlag = 1;
            }
            else
            {
                USBFlag = 0;
            }

            Timerms = 0;
        }

        ErrorMsg(MENU_FTSYSBACKUP);
        if (pno != MENU_FTSYSBACKUP)
        {
            plastno = MENU_FTSYSBACKUP;
            break;
        }
        wdt();
    }
}

//-------------------------------------------------------
#define KEY_FTSYSRESTORE_SCRNSVR 1
#define KEY_FTSYSRESTORE_BACK 2
#define KEY_FTSYSRESTORE_SYSTEM 3
#define KEY_FTSYSRESTORE_PARA 4
#define KEY_FTSYSRESTORE_IOSETTING 5

#define KEY_FTSYSRESTORE1_BACK 6
#define KEY_FTSYSRESTORE1_ENTER 7
#define KEY_FTSYSRESTORE1_DISP 8
#define KEY_FTSYSRESTORE1_FLASH 9
#define KEY_FTSYSRESTORE1_MBOARD 10
#define KEY_FTSYSRESTORE1_WIFI 11
#define KEY_FTSYSRESTORE1_2010 12
#define KEY_FTSYSRESTORE1_2516 13
#define KEY_FTSYSRESTORE1_3020 14
#define KEY_FTSYSRESTORE1_4030 15
#define KEY_FTSYSRESTORE1_4030P 16
#define KEY_FTSYSRESTORE1_4030B 17
#define KEY_FTSYSRESTORE1_6040 18
#define KEY_FTSYSRESTORE1_6050 19
#define KEY_FTSYSRESTORE1_10050 20
#define KEY_FTSYSRESTORE1_311 21
#define KEY_FTSYSRESTORE1_326 22
#define KEY_FTSYSRESTORE1_342 23
#define KEY_FTSYSRESTORE1_4030RR 24
#define KEY_FTSYSRESTORE1_6440D 25
#define KEY_FTSYSRESTORE1_1510 26
#define KEY_FTSYSRESTORE1_OTHER 27

#define RECT_FTSYSRESTORE1_ITEM1_TIT 1
#define RECT_FTSYSRESTORE1_ITEM1_STR 2
#define RECT_FTSYSRESTORE1_ITEM2_TIT 3
#define RECT_FTSYSRESTORE1_ITEM2_STR 4
#define RECT_FTSYSRESTORE1_ITEM3_TIT 5
#define RECT_FTSYSRESTORE1_ITEM3_STR 6
#define RECT_FTSYSRESTORE1_ITEM4_TIT 7
#define RECT_FTSYSRESTORE1_ITEM4_STR 8
#define RECT_FTSYSRESTORE1_ITEM5_TIT 9
#define RECT_FTSYSRESTORE1_ITEM5_STR 10
#define RECT_FTSYSRESTORE1_ITEM6_TIT 11
#define RECT_FTSYSRESTORE1_ITEM6_STR 12
#define RECT_FTSYSRESTORE1_ITEM7_TIT 13
#define RECT_FTSYSRESTORE1_ITEM7_STR 14
#define RECT_FTSYSRESTORE1_ITEM8_TIT 15
#define RECT_FTSYSRESTORE1_ITEM8_STR 16
#define RECT_FTSYSRESTORE1_ITEM9_TIT 17
#define RECT_FTSYSRESTORE1_ITEM9_STR 18

#define RECT_FTSYSRESTORE1_ITEM_MAX 9

//------------------------------------------
// Function   : void Menu_FtSysrestore(void)
// Description: 功能画面_系统还原升级
//------------------------------------------
void Menu_FtSysrestore(void)
{
#if 1
    U8 beeponf = 0, delay = KEYDT;
    U32 keyon = 0, keyoff = 0, keydata = 0;
    U8 runflag = 0, oldRunFlag = 0xFF;
    U8 dis_updata = 1,   //初始化
        dis_updata1 = 0, //系统按钮显示
        dis_updata2 = 0; //选择升级方式
    U8 f, select;
    U32 i;
    U8 StorType = 0; //外部存储器状态;1：U盘 2：SD卡
    // const U16 CurVer = ((g_ProInfo.Data[0] & 0xff) << 8) | ((g_ProInfo.Data[0] >> 8) & 0xff);
    // const U16 CurVerY = ((g_ProInfo.Data[1] & 0xff) << 8) | ((g_ProInfo.Data[1] >> 8) & 0xff);
    // const U8 CurVerM = g_ProInfo.Data[2] & 0xff;
    // const U8 CurVerD = (g_ProInfo.Data[2] >> 8) & 0xff;

    const U16 mbtype[] = {2010, 2516, 3020, 4030, 4030, 4030, 6040, 6050, 10050, 311, 326, 342, 4030, 6040, 1510, 0};
    const U16 mbtypeAddr[] = {1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; //要改

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},
        {0, 0, 128, 32},                     //1 屏保
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 返回
        {TYPE_X90, TYPE_Y6, KEY_X9, KEY_Y9}, //3 参数选项1
        {TYPE_X91, TYPE_Y6, KEY_X9, KEY_Y9}, //4 参数选项2
        {TYPE_X92, TYPE_Y6, KEY_X9, KEY_Y9}, //5 参数选项3

        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                  //6 返回
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                 //7 确认
        {TYPE_X12 - KEY_X17 * 2, TYPE_Y4, KEY_X17, KEY_Y17}, //8 升级屏幕
        {TYPE_X12 - KEY_X17, TYPE_Y4, KEY_X17, KEY_Y17},     //9 升级内存
        {TYPE_X12, TYPE_Y4, KEY_X17, KEY_Y17},               //10 升级主板
        {TYPE_X12 + KEY_X17, TYPE_Y4, KEY_X17, KEY_Y17},     //11 升级WIFI

    };
    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},                            //1 屏保
        {{KEY_Silver}, {0}, {72, 88}, {TLFNADD_FUN_RESTORE + 1, 0, 0, 0}, {0}}, //2 返回

        {{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_FUN_RESTORE + 23, 0, 0, 0}, {TLFNADD_FUN_RESTORE + 23}}, //3 选项1 参数备份
        {{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_FUN_RESTORE + 24, 0, 0, 0}, {TLFNADD_FUN_RESTORE + 24}}, //4 选项2 IO设定备份
        {{KEY_Silver}, {Color_Blue}, {KEY_Silver}, {TLFNADD_FUN_RESTORE + 25, 0, 0, 0}, {TLFNADD_FUN_RESTORE + 25}}, //5 选项3 运行信息备份

        {{KEY_Silver}, {0}, {72, 88}, {TLFNADD_FUN_RESTORE + 5, 0, 0, 0}, {0}},                                                                            //6 返回
        {{KEY_Silver}, {0}, {71, 87}, {TLFNADD_FUN_RESTORE + 6, 0, 0, 0}, {0}},                                                                            //7 确认
        {{KEY_Silver, KEY_Silver}, {0}, {701, 702}, {TLFNADD_FUN_RESTORE + 26, TLFNADD_FUN_RESTORE + 27}, {0}},                                            //8 打板器升级
        {{KEY_Silver, KEY_Silver, KEY_Silver}, {0}, {704, 705, 706}, {TLFNADD_FUN_RESTORE + 28, TLFNADD_FUN_RESTORE + 29, TLFNADD_FUN_RESTORE + 30}, {0}}, //9 FLASH升级
        {{KEY_Silver, KEY_Silver}, {0}, {707, 708}, {TLFNADD_FUN_RESTORE + 31, TLFNADD_FUN_RESTORE + 32}, {0}},                                            //10 主板升级
        {{KEY_Silver, KEY_Silver, KEY_Silver}, {0}, {710, 711, 712}, {TLFNADD_FUN_RESTORE + 33, TLFNADD_FUN_RESTORE + 34, TLFNADD_FUN_RESTORE + 35}, {0}}, //11 wifi程序升级
    };

    const RECTPAD rect[] = {
        //方框
        {0, 0, 0, 0, {COLOR_TITLE_BG}, {COLOR_TITLE_STR}, {TLFNADD_FUN_RESTORE}},
        {140, TYPE_Y2, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 2, TLFNADD_FUN_VER + 3}},                                           //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                                                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1), TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 4}},                                    //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1), TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                             //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 2, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 5}},                                //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 2, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 3, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 6}},                                //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 3, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 4, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 7}},                                //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 4, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 5, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 11}},                               //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 5, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 6, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {TLFNADD_FUN_VER + 12}},                               //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 6, TYPE_SIZE22, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},                         //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 7, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue, Color_Gray}, {Color_White, Color_Gray}, {TLFNADD_FUN_VER + 10}},       //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 7, TYPE_SIZE22, TYPE_SIZE5, {Color_White, Color_Gray}, {Color_Black, Color_Gray}, {0}}, //2 数据框
        {140, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 8, TYPE_SIZE18, TYPE_SIZE5, {Color_KeyBlue, Color_Gray}, {Color_White, Color_Gray}, {TLFNADD_FUN_VER + 13}},       //1 标题框
        {140 + TYPE_SIZE18 + SAFE_DIST8, TYPE_Y2 + (TYPE_SIZE5 + SAFE_DIST1) * 8, TYPE_SIZE22, TYPE_SIZE5, {Color_White, Color_Gray}, {Color_Black, Color_Gray}, {0}}, //2 数据框
    };

    SetLanguage(MENU_FUNCTION);

    if (g_uperr == 1)
    {
        runflag = 1;
    }
    Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 1, (U16 *)(&YREG[SYS_MACHINE_TYPE])); //读系统版本,编号信息
    Read_D(D_SYS + SYS_UDZHOU, 1, (U16 *)(&YREG[SYS_UDZHOU]));

    for (select = 3, i = 0; i < 16; i++)
    {
        if (YREG[SYS_MACHINE_TYPE] == mbtype[i])
        {
            select = i;
            if (mbtype[i] == 4030)
            {
                if (YREG[SYS_UDZHOU] == 0xa5 || YREG[SYS_UDZHOU] == 0x5a)
                    select = 12; //4030RR
                else if (YREG[SYS_UDZHOU] == 1)
                    select = 4; //4030P
                else if (YREG[SYS_UDZHOU] == 2)
                    select = 5; //4030B
            }
            else if (mbtype[i] == 6040)
            {
                if (YREG[SYS_UDZHOU] == 0x55)
                    select = 13; //6040D
            }
            break;
        }
    }
    if (i == 16)
    {               //没有对应型号
        select = 8; //选择10050机器
    }
    while (1)
    {
        if (dis_updata)
        {
            dis_updata = 0;

            if (oldRunFlag != runflag)
            {
                SetLanguage(MENU_FUNCTION);
                InitScreen();
                initViewStatus();
                updateViewStatus();
                Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

                if (runflag == 0)
                {
                    DrawButton(keyd[KEY_FTSYSRESTORE_BACK], (U16 *)(ADD_ICONBMP), keybmpind[KEY_FTSYSRESTORE_BACK].border[0], keybmpind[KEY_FTSYSRESTORE_BACK].icon[0]);
                    for (i = 0; i < 3; i++)
                    {
                        f = KEY_FTSYSRESTORE_SYSTEM + i;

                        DrawButtonSingleColor(keyd[f], keybmpind[f].border[0], keybmpind[f].icon[0]);
                        Draw24x24MulStr(keyd[f].xpis + (keyd[f].xsize - strlen((char *)(MEM_TITLE_ADD + keybmpind[f].nameStr[0] * TitleLengthMax)) * 12) / 2,
                                        keyd[f].ypis + (keyd[f].ysize - 24) / 2, (MEM_TITLE_ADD + (keybmpind[f].nameStr[0] * TitleLengthMax)),
                                        16, SheetPage, keybmpind[f].nameStr[0], 2, keybmpind[f].textColor[0]);
                    }
                }
                else if (runflag == 1)
                {
                    U16 MachineType;                                                     //机型
                    U16 MainBoardVer;                                                    //主板版本号
                    U32 MainBoardCode;                                                   //主板编号
                    U32 MainTim;                                                         //主板发布时间
                    U16 LCDVer = DISP_VER;                                               //打版器版本号
                    U16 LCDTim = PUB_DATE;                                               //打版器时间
                    U16 SERVER;                                                          //伺服驱动版本号
                    U8 enable[RECT_FTSYSVERSION_ITEM_MAX] = {1, 1, 1, 1, 1, 1, 1, 1, 0}; //使能

                    Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 7, (U16 *)(&YREG[SYS_MACHINE_TYPE])); //读系统版本,编号信息
                    MachineType = YREG[SYS_MACHINE_TYPE];
                    MainBoardVer = YREG[SYS_MAINBOARD_VER];
                    MainBoardCode = ((U32)(YREG[SYS_MAINBOARD_CODE2]) << 16) + (U32)(YREG[SYS_MAINBOARD_CODE1]);
                    SERVER = YREG[SYS_OPERA_VER];

                    Read_D(D_Address[SYS] + SYS_WT1, 2, (U16 *)(&YREG[SYS_WT1])); //读系统版本,编号信息
                    MainTim = ((U32)(YREG[SYS_WT2]) << 16) + (U32)(YREG[SYS_WT1]);

                    for (i = 0; i < RECT_FTSYSRESTORE1_ITEM_MAX; ++i)
                    {
                        f = i * 2 + RECT_FTSYSRESTORE1_ITEM1_STR;
                        DrawFillRect(rect[f].xpis, rect[f].ypis, rect[f].xsize, rect[f].ysize, rect[f].colorB[0]);
                    }

                    if ((g_UdZhouFlag == 1) || (g_UdZhouFlag == 2)) //TESTKEYF 0x5a  0xa5
                    {
                        Draw24X24FStr(rect[RECT_FTSYSRESTORE1_ITEM1_STR].xpis + SAFE_DIST4,
                                      rect[RECT_FTSYSRESTORE1_ITEM1_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM1_STR].ysize / 2 - 12, "T", rect[RECT_FTSYSRESTORE1_ITEM1_STR].colorT[0]);
                        DrawMFU32Z(rect[RECT_FTSYSRESTORE1_ITEM1_STR].xpis + SAFE_DIST4 + 24, rect[RECT_FTSYSRESTORE1_ITEM1_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM1_STR].ysize / 2 - 12,
                                   MachineType, 5, rect[RECT_FTSYSRESTORE1_ITEM1_STR].colorT[0]);
                    }
                    else if (g_UdZhouFlag == 3) //	0x55
                    {
                        Draw24X24FStr(rect[RECT_FTSYSRESTORE1_ITEM1_STR].xpis + SAFE_DIST4,
                                      rect[RECT_FTSYSRESTORE1_ITEM1_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM1_STR].ysize / 2 - 12, "D", rect[RECT_FTSYSRESTORE1_ITEM1_STR].colorT[0]);
                        DrawMFU32Z(rect[RECT_FTSYSRESTORE1_ITEM1_STR].xpis + SAFE_DIST4 + 24, rect[RECT_FTSYSRESTORE1_ITEM1_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM1_STR].ysize / 2 - 12,
                                   MachineType, 5, rect[RECT_FTSYSRESTORE1_ITEM1_STR].colorT[0]);
                    }
                    else
                    {
                        DrawMFU32Z(rect[RECT_FTSYSRESTORE1_ITEM1_STR].xpis + SAFE_DIST4, rect[RECT_FTSYSRESTORE1_ITEM1_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM1_STR].ysize / 2 - 12,
                                   MachineType, 5, rect[RECT_FTSYSRESTORE1_ITEM1_STR].colorT[0]);
                    }
                    //主板版本号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM2_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM2_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM2_STR].ysize / 2 - 12,
                              MainBoardVer, 4, rect[RECT_FTSYSRESTORE1_ITEM2_STR].colorT[0]);
                    //主板发布时间
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM2_STR].xpis + SAFE_DIST4 + 13 * 6,
                              rect[RECT_FTSYSRESTORE1_ITEM2_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM2_STR].ysize / 2 - 12,
                              MainTim, 4, rect[RECT_FTSYSRESTORE1_ITEM2_STR].colorT[0]);
                    //主板编号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM3_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM3_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM3_STR].ysize / 2 - 12,
                              MainBoardCode, 9, rect[RECT_FTSYSRESTORE1_ITEM3_STR].colorT[0]);

                    //打版器版本号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM4_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM4_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM4_STR].ysize / 2 - 12,
                              LCDVer, 4, rect[RECT_FTSYSRESTORE1_ITEM4_STR].colorT[0]);
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM4_STR].xpis + SAFE_DIST4 + 13 * 6,
                              rect[RECT_FTSYSRESTORE1_ITEM4_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM4_STR].ysize / 2 - 12,
                              LCDTim, 4, rect[RECT_FTSYSRESTORE1_ITEM4_STR].colorT[0]);

                    //打版器编号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM5_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM5_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM5_STR].ysize / 2 - 12,
                              SYSSETID, 9, rect[RECT_FTSYSRESTORE1_ITEM5_STR].colorT[0]);

                    //伺服版本号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM6_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM6_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM6_STR].ysize / 2 - 12,
                              SERVER, 4, rect[RECT_FTSYSRESTORE1_ITEM6_STR].colorT[0]);

                    //FLASH 版本号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM7_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM7_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM7_STR].ysize / 2 - 12,
                              flashver, 4, rect[RECT_FTSYSRESTORE1_ITEM7_STR].colorT[0]);

                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM7_STR].xpis + SAFE_DIST4 + 13 * 6,
                              rect[RECT_FTSYSRESTORE1_ITEM7_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM7_STR].ysize / 2 - 12,
                              flashdate, 8, rect[RECT_FTSYSRESTORE1_ITEM7_STR].colorT[0]);

                    //MP3 版本号
                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM8_STR].xpis + SAFE_DIST4,
                              rect[RECT_FTSYSRESTORE1_ITEM8_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM8_STR].ysize / 2 - 12,
                              mp3dver, 4, rect[RECT_FTSYSRESTORE1_ITEM8_STR].colorT[0]);

                    DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM8_STR].xpis + SAFE_DIST4 + 13 * 6,
                              rect[RECT_FTSYSRESTORE1_ITEM8_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM8_STR].ysize / 2 - 12,
                              mp3ddate, 8, rect[RECT_FTSYSRESTORE1_ITEM8_STR].colorT[0]);

                    //读系WIFI版本号
                    Read_D(D_Address[SYS] + SYS_WIFI_VER, 1, (U16 *)(&YREG[SYS_WIFI_VER]));
                    if (YREG[SYS_WIFI_VER] != 0)
                    {
                        DrawMFU32(rect[RECT_FTSYSRESTORE1_ITEM9_STR].xpis + SAFE_DIST4,
                                  rect[RECT_FTSYSRESTORE1_ITEM9_STR].ypis + rect[RECT_FTSYSRESTORE1_ITEM9_STR].ysize / 2 - 12,
                                  YREG[SYS_WIFI_VER], 4, rect[RECT_FTSYSRESTORE1_ITEM9_STR].colorT[0]);
                    }

                    for (i = 0; i < RECT_FTSYSRESTORE1_ITEM_MAX; i++)
                    {
                        f = i * 2 + RECT_FTSYSRESTORE1_ITEM1_TIT;
                        if (enable[i])
                        {
                            DrawEditTit(rect[f], rect[f].index[0], SheetPage, LEFT, STR24X12, SAFE_DIST4);
                        }
                        else
                        {
                            DrawFillRect(rect[f].xpis, rect[f].ypis, rect[f].xsize, rect[f].ysize, rect[f].colorB[1]);
                            Draw24x24Str(rect[f].xpis + SAFE_DIST4,
                                         rect[f].ypis + (rect[f].ysize - 24) / 2,
                                         (MEM_TITLE_ADD + (U32)(rect[f].index[0] * TitleLengthMax)),
                                         SheetPage, rect[f].index[0], rect[f].colorT[0]); //画出方框的文字
                        }
                    }

                    for (i = KEY_FTSYSRESTORE1_BACK; i <= KEY_FTSYSRESTORE1_ENTER; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                    dis_updata1 = 1;
                    dis_updata2 = 1;
                }
            }

            if (oldRunFlag != runflag)
            {
                oldRunFlag = runflag;
                ViewPage(usepageno);
            }
        }

        keydata = get_keydata((KEYPAD *)keyd, KEY_FTSYSRESTORE_SCRNSVR + 1, 0);
        if (keydata == 0)
        {
            if (runflag == 0)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_FTSYSRESTORE_IOSETTING + 1, KEY_FTSYSRESTORE_BACK);
            }
            else
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_FTSYSRESTORE1_WIFI + 1, KEY_FTSYSRESTORE1_BACK);
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
                if (keyon == KEY_FTSYSRESTORE_SCRNSVR)
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
        if (keyoff)
        {
            if ((keyoff >= KEY_FTSYSRESTORE1_2010) && (keyoff <= KEY_FTSYSRESTORE1_OTHER))
            {
                select = keyoff - KEY_FTSYSRESTORE1_2010;
                dis_updata1 = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_FTSYSRESTORE_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_FTSYSRESTORE_BACK:
                    pno = poldno[--pnoindex];
                    if (g_user.newmode == 2)
                    {
                        g_user.newuser = g_user.olduser;
                        g_user.newmode = g_user.oldmode;
                        g_user.olduser = 0;
                        g_user.oldmode = 0;
                    }
                    break;
                case KEY_FTSYSRESTORE_SYSTEM:
                    runflag = 1;
                    dis_updata = 1;
                    break;
                case KEY_FTSYSRESTORE_PARA: //参数还原   还原数据在打板器U盘或SD卡  暂不支持主板设备
                    StorType = 0;
                    //if(ExtMemory_Status(SD)==0){		//查询是否插有SD卡
                    //	StorType |= 0x02;
                    //}

                    if (ExtMemory_Status(USBTF) == 0)
                    { //查询打板器U盘
                        StorType |= 0x01;
                    }

                    if (StorType == 0x03)
                    { //USB && SD
                        hleppara = 84;
                        g_para0 = 132;
                    }
                    else if ((StorType == 0x02) || (StorType == 0x01))
                    { //SD	|| USB
                        hleppara = 132;
                        g_disk_memory = StorType;
                    }
                    else
                    {
                        hleppara = 95;
                    }
                    pno = MENU_RWHELP;
                    poldno[pnoindex++] = MENU_FTSYSRESTORE;

                    break;
                case KEY_FTSYSRESTORE_IOSETTING: //IO设定参数还原   还原数据在打板器U盘或SD卡  暂不支持主板设备
                    StorType = 0;
                    //if(ExtMemory_Status(SD)==0){		//查询是否插有SD卡
                    //	StorType |= 0x02;
                    //}

                    if (ExtMemory_Status(USBTF) == 0)
                    { //查询打板器U盘
                        StorType |= 0x01;
                    }

                    if (StorType == 0x03)
                    { //USB && SD
                        hleppara = 84;
                        g_para0 = 134;
                    }
                    else if ((StorType == 0x02) || (StorType == 0x01))
                    { //SD	|| USB
                        hleppara = 134;
                        g_disk_memory = StorType;
                    }
                    else
                    {
                        hleppara = 95;
                    }
                    pno = MENU_RWHELP;
                    poldno[pnoindex++] = MENU_FTSYSRESTORE;
                    break;
                case KEY_FTSYSRESTORE1_BACK:
                    runflag = 0;
                    dis_updata = 1;
                    break;
                case KEY_FTSYSRESTORE1_ENTER:

                    if (g_upDISP == 0 && g_upFLASH == 0 && g_upMBOARD == 0 && g_upWIFI == 0) //没有选择升级设备
                    {
                        g_upDISP = 1; //设置为默认状态
                        g_upFLASH = 2;
                        g_upMBOARD = 1;
                        if (YREG[SYS_WIFI_VER] != 0)
                        {                 //标记为wifi模式
                            g_upWIFI = 1; //自动选择升级wifi模式
                        }
                        else
                        {
                            g_upWIFI = 0;
                        }
                        pno = MENU_RWHELP;
                        hleppara = 150;
                        poldno[pnoindex++] = MENU_FTSYSRESTORE;
                        break;
                    }
                    StorType = 0;
                    //if(ExtMemory_Status(SD)==0){		//查询是否插有SD卡
                    //	StorType |= 0x02;
                    //}
                    //
                    //Write_Cmd(C_DISK_STATUS,0,0);   //查询主板U盘插入情况
                    //for(i=0;i<FORTIMS1;i++){
                    //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                    //	if((DREG[D_CMDSTATUS] >= 100)||(DREG[D_CMDSTATUS] == 10)){
                    //		break;
                    //	}
                    //	DelayMs(WAITTIME2);
                    //}
                    //if(DREG[D_CMDSTATUS] == 102 || DREG[D_CMDSTATUS] == 101){		//主板有U盘
                    //	StorType |= 0x01;
                    //}
                    //
                    if (ExtMemory_Status(USBTF) == 0) //查询打板器U盘
                    {
                        StorType |= 0x01;
                    }

                    if (StorType == 0x03)
                    {
                        pno = MENU_RWHELP;
                        hleppara = 85;
                        poldno[pnoindex++] = MENU_FTSYSRESTORE;
                    }
                    else if (StorType == 0x02)
                    {
                        g_disk_memory = 2;
                        pno = MENU_RWHELP;
                        hleppara = 68;
                        poldno[pnoindex++] = MENU_FTSYSRESTORE;
                    }
                    else if (StorType == 0x01)
                    {
                        g_disk_memory = 1;
                        pno = MENU_RWHELP;
                        hleppara = 68;
                        poldno[pnoindex++] = MENU_FTSYSRESTORE;
                    }
                    else
                    {
                        pno = MENU_RWHELP;
                        hleppara = 95;
                        poldno[pnoindex++] = MENU_FTSYSRESTORE;
                    }

                    g_MachineTy = mbtypeAddr[select]; //mtypedata[select];
                    break;

                case KEY_FTSYSRESTORE1_DISP:
                    g_upDISP++;
                    if (g_upDISP > 1)
                        g_upDISP = 0;
                    dis_updata2 = 1;
                    break;
                case KEY_FTSYSRESTORE1_FLASH:
                    g_upFLASH++;
                    if (g_upFLASH > 2)
                        g_upFLASH = 0;
                    dis_updata2 = 1;
                    break;

                case KEY_FTSYSRESTORE1_MBOARD:
                    g_upMBOARD++;
                    if (g_upMBOARD > 1)
                        g_upMBOARD = 0;
                    dis_updata2 = 1;
                    break;

                case KEY_FTSYSRESTORE1_WIFI:
                    if (YREG[SYS_WIFI_VER] != 0)
                    {
                        g_upWIFI++;
                    }
                    else
                    {
                        g_upWIFI = 0;
                    }
                    if (g_upWIFI > 1)
                        g_upWIFI = 0;
                    dis_updata2 = 1;
                    break;
                }
            }
            keyoff = 0;
        }
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {
            if (keyon == KEY_FTSYSRESTORE_SCRNSVR) //不需要显示的按键
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
            dis_updata1 = 0;

            // f = 2 + select;
            // for (i = 2; i < keynum1 - 4 - 1; i++)
            // {

            //     DrawButton2(keyd1[i], (i == f) ? Color_KeyBlue : (g_user.newuser >= 8 ? Color_White1 : Color_Gray), 0, 1);
            //     Draw24x24Str(keyd1[i].xpis + ((i < 10) ? 28 : 15), keyd1[i].ypis + 20, MEM_TITLE_ADD + ((U32)(TLFNADD_SYSRESTORE + 5 + i)) * TitleLengthMax, SheetPage, TLFNADD_SYSRESTORE + 5 + i, (i == f) ? Color_White : (g_user.newuser >= 8 ? (mCOLOR ? Color_Blue : Color_Black) : Color_Black));
            // }
        }

        if (dis_updata2)
        {
            dis_updata2 = 0;
            DrawButton(keyd[KEY_FTSYSRESTORE1_DISP], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_FTSYSRESTORE1_DISP].border[g_upDISP],
                       keybmpind[KEY_FTSYSRESTORE1_DISP].icon[g_upDISP]);
            DrawButton(keyd[KEY_FTSYSRESTORE1_FLASH], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_FTSYSRESTORE1_FLASH].border[g_upFLASH],
                       keybmpind[KEY_FTSYSRESTORE1_FLASH].icon[g_upFLASH]);
            DrawButton(keyd[KEY_FTSYSRESTORE1_MBOARD], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_FTSYSRESTORE1_MBOARD].border[g_upMBOARD],
                       keybmpind[KEY_FTSYSRESTORE1_MBOARD].icon[g_upMBOARD]);
            DrawButton(keyd[KEY_FTSYSRESTORE1_WIFI], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_FTSYSRESTORE1_WIFI].border[g_upWIFI],
                       keybmpind[KEY_FTSYSRESTORE1_WIFI].icon[g_upWIFI]);
        }
        ErrorMsg(MENU_FTSYSRESTORE);
        if (pno != MENU_FTSYSRESTORE)
        {
            plastno = MENU_FTSYSRESTORE;
            break;
        }
        wdt();
    }
#endif
}
