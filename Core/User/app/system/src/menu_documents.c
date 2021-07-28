/*
 * @Author: your name
 * @Date: 2021-05-07 19:34:06
 * @LastEditTime: 2021-07-27 14:54:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\src\menu_documents.c
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "menu_documents.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "uart.h"
#include "main.h"
#include "function.h"
#include "S39vf160.h"
#include "color.h"
#include "communication.h"
#include "disk_oper.h"
#include "w25qxx.h"
#include "spi.h"
#include "tfcard.h"
#include "ds2460.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"
#include "Patfun.h"
#include "Mit_funtion.h"
#include "exfunction.h"
#include "Mit_menu_files.h"
#include "msgpic.h"

#define TLFNADD_RWHELP 0
#define TLFNLEN_RWHELP 21 //0
#define TLFNADD_RWHELP1 (TLFNADD_RWHELP + TLFNLEN_RWHELP)
#define TLFNLEN_RWHELP1 11

extern volatile U32 g_light_c;
extern volatile U32 delaytimes;
extern volatile U8 g_NoReadPatf;
extern volatile U8 g_init_flag;
extern volatile U16 g_oldselectdisno;
extern volatile U16 g_selectdisno;
extern volatile U16 g_selectnumber;
extern volatile U8 g_disk_memory;
extern volatile U8 g_distmpName[64];
extern volatile U16 g_Fileno;
extern volatile U8 g_MboardUSB;
extern saveINFO g_savePatInfo;
extern select_id_message g_SelectNo[SELECTNO_MAX];
extern volatile U16 g_Combinepat[GroupMax][6];
extern volatile U8 g_GroupName[9];
extern volatile U8 g_GroupType;
extern volatile U8 g_GroupMenth;
extern volatile U16 g_Groupsel1;
extern volatile U8 g_GroupSave;
extern volatile U8 g_GroupPatNums;
extern volatile U8 g_GroupDoingNO;
extern volatile U8 g_checkGroup;
extern volatile U8 g_tmpGroName[10];
extern volatile U16 g_MachineTy;
extern volatile U8 gPeessVal;
extern volatile U8 g_netSelectMac;
extern volatile U16 g_array6[302];
extern volatile U8 g_NoUpdataoldno;
extern volatile U8 g_backSREG_f; // 备份SERG数据标志
extern volatile U8 g_backHREG_f; // 备份HERG数据标志
extern volatile U32 g_backSREGlen;
extern volatile U32 g_recv_netdata;
extern volatile U8 g_Restorepropara;
extern volatile U8 g_NO_ReagIOf;
extern volatile U8 coldstart;
extern volatile S16 g_dxpos, g_dypos;
extern volatile S16 g_tmpoxpos, g_tmpoypos;
extern volatile U8 g_Funtype;
extern volatile U8 g_exwriteType;
extern volatile U8 g_BorSTflag;
extern volatile U32 g_max_feedsum;
extern volatile U32 g_min_feedsum;
extern volatile U32 g_jog_point;
extern volatile U8 g_IsLocal;
extern volatile U8 g_ImgMove;
extern volatile S16 g_upDISP;   // 升级打板器
extern volatile S16 g_upFLASH;  // 升级打板器 FLASH
extern volatile S16 g_upMBOARD; // 升级主板
extern volatile S16 g_upWIFI;   // 升级wifi模块
extern MachineInfo g_Mac;
extern ProInfo g_ProInfo;
extern volatile U8 g_UdZhouFlag;
extern volatile U8 g_savemodify;
extern volatile U8 g_updataIREG;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos;
extern volatile U8 g_needtohome;
extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U8 g_ExCode; //表示附加功能输出状态
extern volatile U8 g_tExCode;
extern volatile U8 g_curExCode;
extern volatile U8 g_endtype;
extern volatile U8 g_cttohome;
extern volatile U16 g_curPinAng;
extern volatile U16 g_curSetPara1;
extern volatile U16 g_curSetPara2;
extern volatile U16 g_pointPinAng;
extern volatile U16 g_pointSetPara1;
extern volatile U16 g_pointSetPara2;
volatile U8 g_NO_ReagIOf = 0; //不读取IO参数标志
extern volatile U8 g_needtohome;
extern volatile U8 g_upinfo[0x20];
extern volatile U8 g_patname[64];

extern volatile U8 g_disFileMethod; //显示文件方式 0-列表模式 1-图标模式
extern volatile U16 g_SelFileNo;
extern volatile U16 g_selfiles[MAXFILENO];  //选择文件
extern volatile U16 g_editfiles[MAXFILENO]; //编辑文件
extern volatile U16 g_btndisable;           //禁用按键
extern volatile U8 g_checkMode;             //复选/单选模式
extern volatile U8 g_selALL;                //全选
extern volatile U8 g_from_memory;
extern volatile U8 g_to_memory;
extern volatile U8 g_clipboard; //剪切板
extern volatile U8 g_savePathTimes;
extern char *g_path;
extern char *g_fromPath;
extern char *g_savePath;
extern volatile U8 g_newname[64];
extern volatile U8 g_oldname[64];

extern volatile ERRMSG g_errMsg; // 报警信息记录
extern BOOTSCR g_bootscr;

extern U8 setBootScreenMode(U16 mode, U16 vmlk);
extern void Bor_SeeCreataddr(U32 *staddr, U32 *Patlen, U8 Type);
extern U32 AnalyseUpdateList(U8 selextm, U8 netflag);
extern U8 update_mcpuprogram(U8 selextm, U8 seltype, U8 flag, U16 bkcolor);
extern U8 update_flash(U8 selextm, U8 flag, U16 bkcolor, U8 netflag);
extern U8 update_mp3(U8 selextm, U8 flag, U16 bkcolor, U8 netflag);
extern U8 update_dispprogram(U8 selextm, U8 flag, U16 bkcolor, U8 netflag, U8 copySys);
extern U8 copy_dispprogram(U8 selextm, U8 flag, U16 bkcolor, U8 netflag, U8 copySys);
extern U8 update1_dispprogram(U8 flag, U16 bkcolor, U8 copySys);
extern void (*theUboot)(void);
extern void creatUpInfoData(U8 *outDat);
extern uint8_t deleteDiskFile(char *path, char *name);
extern uint8_t getDiskFiles(char *path, U16 *indexReg, uint8_t ftype);
extern void initFunctionBtn(U8 clr);
extern uint8_t writeDiskPatFile(char *path, uint16_t patnum);
extern uint8_t readDiskPatFileByName(char *path, char *fname, U16 *sreg);
extern uint8_t copyDiskPatFile(char *spath, uint8_t sdisk, char *dpath, uint8_t ddisk, char *name, uint8_t mode);

#define FileIndexSize 36   //每帧文件信息大小
#define FileIndexNamepox 4 //每帧文件每帧文件编号起始位置
#define PatFilesMAX 999    //花样文件数最大值  1~999

#define F_ERROR 0   // dir/file operation failed
#define F_OK 1      // dir/file operation successfull
#define F_SST32OK 2 // 读打板器SST3201数据成功
#define DOWNLOAD_LEN (160 * 1024)
#define MAXFLASHADD 0x400 / 2
#define FLASH_VER 0x000003fc
#define W25Q1 2

#define SST_IAP_FLASADD 0x00700000 //偏移地址
#define IAP_FLAG_ADD 0x00600000    /////////////	一个块64k字节
#define IAP_LEN_ADD 0x00610000     //4k字节 长度
#define IAP_FHFLAG_ADD 0x00620000  //4k字节结束

#define FLAG_HARDREST 0x00001234
#define IAP_UPDATE_FLAG 0x12345678
#define IAP_FHFLAG 0x07654321
#pragma pack(1)
typedef struct
{
    U16 Type;      // 型号
    U32 Dadd;      // 单片机下载起始地址
    U32 Len;       // 长度
    U16 Crc16;     // crc16
    U16 Sumchk;    // 累加校验
    U16 Norchk;    // 异或校验
    U32 Other;     // 其他
    U32 FileInAdd; //数据在本文件的地址
} UpdateFilePara;
UpdateFilePara UpdateFileCode[3];

unsigned short SST32_secloop = 0;
unsigned long SST32_tmpcluster = 0;
unsigned long SST32_tmpsector = 0;
unsigned long SST32_FilePosition = 0;
//unsigned short SST32_secloop_old=0;
//unsigned long  SST32_tmpcluster_old=0;
//unsigned long  SST32_tmpsector_old=0;
//unsigned long  SST32_FilePosition_old=0;
unsigned long SST32FrameNum = 0;
U8 updateflash = 0;
U16 *add;
U32 FlashDownLoad = 1;
U32 p;
U16 *TEMP;
volatile U8 sysmemorydrive = W25Q1;

#define NewType 0

U8 lencount(U8 *IN)
{
    U8 result = 0;
    while (*IN != 0)
    {
        result++;
        IN++;
    }
    return result;
}

U32 g_ERROR_LEN;
U32 g_ERROR_MSG;
void ViewMsg(U16 msgIndex)
{
    const U16 vTestColor = Color_White;
    const U8 l1 = 3, l2 = 5;
    const U16 vaddr1 = TYPE_Y2 + SAFE_DIST6 + SAFE_DIST4,
              vaddr2 = TYPE_Y2 + SAFE_DIST6 * 3 + TYPE_SIZE13 + SAFE_DIST4; //198,312
    const U16 disPosStr_X = TYPE_X19 + SAFE_DIST4;
    const U8 linemaxdisplay = 44; //32
    const U16 Err_MSG1_Sheet1 = 48;
    const U16 Err_MSG1_Sheet2 = 49;

    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + g_ERROR_LEN * msgIndex),
                    linemaxdisplay, Err_MSG1_Sheet1, msgIndex, l1, vTestColor); //未插入磁碟片
    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + g_ERROR_LEN * msgIndex + g_ERROR_MSG),
                    linemaxdisplay, Err_MSG1_Sheet2, msgIndex, l2, vTestColor); //请插入磁碟片
}

#define NEEDLE_UP_DW 0x02000000

#define KEY_RWHELP_EXIT 1   //1 返回
#define KEY_RWHELP_ENTER 2  //2 确认
#define KEY_RWHELP_ORG 3    //3 回原点
#define KEY_RWHELP_NEEDLE 4 //4 针杆
#define KEY_RWHELP_USB 5    //5 USB
#define KEY_RWHELP_SD 6     //6 SD
#define KEY_RWHELP_HERROR 7 //7 异常报警确认

#define KEY_RWHELP_MAX 8

#define RECT_RWHELP_BG 1   //1 背景
#define RECT_RWHELP_BMP1 2 //2 图片1
#define RECT_RWHELP_BMP2 3 //3 图片2
#define RECT_RWHELP_STR1 4 //4 文字索引1
#define RECT_RWHELP_STR2 5 //5 文字索引2

#define RECT_RWHELP_MAX 6

#define RECT_RWHELP2_BG 1          //1 背景
#define RECT_RWHELP2_TIT 2         //2 标题
#define RECT_RWHELP2_PINSUM_TIT 3  //3 总共针数-标题
#define RECT_RWHELP2_PINSUM_STR 4  //4 总共针数-数值
#define RECT_RWHELP2_PINUSE_TIT 5  //5 已用针数-标题
#define RECT_RWHELP2_PINUSE_STR 6  //6 已用针数-数值
#define RECT_RWHELP2_PATSUM_TIT 7  //7 总共容量-标题
#define RECT_RWHELP2_PATSUM_STR 8  //8 总共容量-数值
#define RECT_RWHELP2_PATUSE_TIT 9  //9 已用容量-标题
#define RECT_RWHELP2_PATUSE_STR 10 //10 已用容量-数值

#define RECT_RWHELP2_MAX 11

// note: para：0：主界面进入修改 1：无 2:打板中修改 3：花样修改中修改
void Menu_RwHelp(U16 para)
{
    //    U16 times50ms = 0xffff;

    /****按键相关参数begin****/
    U8 beeponf = 0;                         //蜂鸣器 启动标志位
    U16 delay = 0;                          //按键执行延迟的设定时间，与Times进行比较
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon按键按下，keyoff按键松开，keydata当前按键信息
                                            /****按键相关参数end****/
    /****标志位相关参数begin****/
    // U8 disflag = 1;

    U8 val = 0;
    U32 temp_para = 0, otemp_para;
    U16 dat = 0, dat1, dat2;
    U32 i = 0, len = 0; //,new_i
    U16 tmp_fsize = 0, tmp_fuseed = 0, tmp_sumfile = 0;
    U16 tmp_error = 0; // = DREG[D_ERRWAR];
    U8 btn_en = 0, err_btn_en = 0;
    U8 dis_updata = 1;
    U8 f = 0, flag = 0;
    U16 j = 0, n = 0, m; //,jj
    U16 SelectNoFino[DISSUM] = {0};
    U8 SelectNoCnt = 0;
    U16 file_meg[FileIndexSize];
    U16 tmpcmd = 0, checksum = 0;
    U32 ii;
    U8 errorf = 0, result = 0;
    U16 backuplen = 0; //系统备份数据长度
    U8 ReSREGcount;    //SREG校验错误时重复次数
    U16 *Msg3_TITLE_ADD;
    U32 Msg3TitleLengthMax;
    U32 Msg3TitleOffest;
    U16 MSG3_Sheet = 0, MSG3_Sheet1 = 0;
    U16 *RwHelp_TITLE_ADD;
    U32 RwHelpTitleLengthMax;
    U16 RwHelp_Sheet = 8;
    U16 *Toast_TITLE_ADD;
    U32 ToastTitleLengthMax;
    U16 Toast_Sheet = 0;
    const U8 BorName[12] = {"ISMS0XXX.SEW"};

    const U16 vTestColor = Color_White;
    const U16 bgColor = Color_Blue;

    U32 ErrCode = 0;    //M主轴卡死错误代码
    U32 ErrCode2 = 0;   //D主轴卡死错误代码
    U32 ErrCode3 = 0;   //X主轴卡死错误代码
    U32 ErrCode4 = 0;   //Y主轴卡死错误代码
    U32 ErrCode5 = 0;   //步进X主轴卡死错误代码
    U32 ErrCode6 = 0;   //步进Y主轴卡死错误代码
    U32 ErrCode7 = 0;   //步进Z主轴卡死错误代码
    U32 ErrCode8 = 0;   //步进U主轴卡死错误代码
    U32 ErrCode9 = 0;   //步进V主轴卡死错误代码
    U8 ExErrDis = 0xff; //主轴卡死显示内容
    U16 filecount = 0;
    U8 err_base = 93;
    const U8 err_base1 = 126; //步进电机报警
    const U8 err_base2 = 133; //贴袋机气缸检测器故障
    U8 temp;
    U16 Err_MSG1_Sheet1 = 48;
    U16 Err_MSG1_Sheet2 = 49;
    U16 Err_MSG2_Sheet1 = 50;
    U16 Err_MSG2_Sheet2 = 51;
    const U8 linemaxdisplay = 44; //32
    U32 ERROR_MSG;
    U32 ERROR_LEN;
    U16 PINERR = 0;
    U8 errCnt = 0;
    U8 errFlag = 0;
    U16 checkbuff[12];
    U32 maxadd;
    U16 *checkadd = NULL;
    U32 listlen = 0;
    U16 tmpchecksum = 0;
    U32 *padd = NULL;
    U16 *dadd;
    U16 datasum, tmpdatasum;
    U32 l, cnti, cntj;
    U8 upflag = 0;
    U16 files, fileNo = MINFILENO, fileNo1 = MINFILENO;
    U8 fileType;
    U8 disName[68] = {0};
    U8 namelen = 0;
    U16 viewTextIndex = 0;
    U8 *p8, *fname;
    const U8 l1 = 3, l2 = 5;
    const U16 vaddr1 = TYPE_Y2 + SAFE_DIST6 + SAFE_DIST4,
              vaddr2 = TYPE_Y2 + SAFE_DIST6 * 3 + TYPE_SIZE13 + SAFE_DIST4; //198,312
    const U16 disPosStr_X = TYPE_X19 + SAFE_DIST4;
    char tmpb[2] = {0x00, 0x00};
    uint8_t defFilePart[50] = {"BROTHER/ISM/ISMDB00"};

#ifdef OLD_BOOT
    U16 UpdataF = 0xfff;
#else
    U8 Info[0x20];
#endif
    /****标志位相关参数end***/

    const U8 keynum = {KEY_RWHELP_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {TYPE_X33, TYPE_Y30, KEY_X1, KEY_Y1},                                     //1 返回
        {TYPE_X34, TYPE_Y30, KEY_X1, KEY_Y1},                                    //2 确认
        {TYPE_X34, TYPE_Y30, KEY_X1, KEY_Y1},                                    //3 回原点
        {TYPE_X33, TYPE_Y30, KEY_X1, KEY_Y1},                                     //4 针杆
        {TYPE_SIZE24 - KEY_X1, TYPE_Y7, KEY_X1, KEY_Y1},               //5 USB
        {TYPE_SIZE24 + KEY_X1, TYPE_Y7, KEY_X1, KEY_Y1},               //6 SD
        {TYPE_X18 + SAFE_DIST6, TYPE_Y2 + SAFE_DIST6, KEY_X1, KEY_Y1}, //7 异常报警确认
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {72, 0, 0, Color_Gray}, {0, 0, 0, 0}, {0}},  //1 返回
        {{KEY_Silver}, {0}, {71, 70, 0, Color_Gray}, {0, 0, 0, 0}, {0}}, ///2 确认
        {{KEY_Silver}, {0}, {3, 0, 0, Color_Gray}, {0, 0, 0, 0}, {0}},   //3 回原点

        {{KEY_Silver}, {0}, {21, 23, 0, Color_Gray}, {0, 0, 0, 0}, {0}},     //4 针杆
        {{KEY_Silver}, {0}, {251, 252, 253, Color_Gray}, {0, 0, 0, 0}, {0}}, //5 USB
        {{KEY_Silver}, {0}, {254, 255, 256, Color_Gray}, {0, 0, 0, 0}, {0}}, //6 SD
        {{KEY_Silver}, {0}, {0}, {7, 0, 0, Color_Gray}, {0}},                //7 异常报警确认

    };

    const RECTPAD rect[RECT_RWHELP_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X18, TYPE_Y2, 700, TYPE_SIZE24, {Color_Gray, Color_Black}, {Color_White}, {0}},                                        //1 背景
        {TYPE_X18 + SAFE_DIST6, TYPE_Y2 + SAFE_DIST6, TYPE_SIZE12, TYPE_SIZE12, {Color_Blue}, {Color_White}, {68}},                  //2 图片1
        {TYPE_X18 + SAFE_DIST6, TYPE_Y2 + SAFE_DIST6 * 3 + TYPE_SIZE13, TYPE_SIZE12, TYPE_SIZE12, {Color_Blue}, {Color_White}, {0}}, //3 图片2
        {TYPE_X19, TYPE_Y2 + SAFE_DIST6, 560, TYPE_SIZE13, {Color_Blue}, {Color_White}, {0}},                                        //4 文字索引1
        {TYPE_X19, TYPE_Y2 + SAFE_DIST6 * 3 + TYPE_SIZE13, 560, 160, {Color_Blue}, {Color_White}, {0}},                              //5 文字索引2
    };
    const RECTPAD rect2[RECT_RWHELP2_MAX] = {
        //方框
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
        {TYPE_X18, TYPE_Y2, 700, TYPE_SIZE24, {Color_Gray, Color_Black}, {Color_White}, {0}},                                     //1 背景
        {164, TYPE_Y6, 472, TYPE_SIZE7, {Color_MidSkyBlue}, {Color_Black}, {7}},                                                 //2 标题
        {164, TYPE_Y6 + TYPE_SIZE7 + SAFE_DIST5, TYPE_SIZE24, TYPE_SIZE7, {Color_KeyBlue}, {Color_White}, {8}},                   //3 总共针数-标题
        {164 + TYPE_SIZE24, TYPE_Y6 + TYPE_SIZE7 + SAFE_DIST5, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}},       //4 总共针数-数值
        {164, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 2, TYPE_SIZE24, TYPE_SIZE7, {Color_LightGray}, {Color_Black}, {10}},          //5 已用针数-标题
        {164 + TYPE_SIZE24, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 2, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}}, //6 已用针数-数值
        {164, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 3, TYPE_SIZE24, TYPE_SIZE7, {Color_KeyBlue}, {Color_White}, {12}},            //7 总共容量-标题
        {164 + TYPE_SIZE24, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 3, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}}, //8 总共容量-数值
        {164, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 4, TYPE_SIZE24, TYPE_SIZE7, {Color_LightGray}, {Color_Black}, {13}},          //9 已用容量-标题
        {164 + TYPE_SIZE24, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 4, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}}, //10 已用容量-数值

    };
    // /[语言显示相关文件路径设置与页面初始化*begin]************************************************
    SetLanguage(MENU_RWHELP); //设置当前文字显示雷人在P1页面，对应41.bin
    if (P_RT > 99)
        P_RT = 0;
    tmpb[0] = (P_RT / 10) % 10 + 0x30;
    tmpb[1] = P_RT % 10 + 0x30;
    defFilePart[17] = tmpb[0];
    defFilePart[18] = tmpb[1];
    if (P_LAN)
    {
        Msg3_TITLE_ADD = (U16 *)ADD_BOR_MSG3_E;
        Msg3TitleLengthMax = 125;
        Msg3TitleOffest = 25;

        Toast_TITLE_ADD = ((U16 *)(tmpflashadd[136]));
        ToastTitleLengthMax = 61;

        UADD_ERROR_MSG = ADD_ERROR_MSG1_E;
        UADD_PROG_MSG7 = ADD_ERROR_MSG2_E;
        RwHelp_TITLE_ADD = MEM_TITLE_RWHELP_E;
        RwHelpTitleLengthMax = TIT_MAXLENGTH_E;
        ERROR_MSG = 40;  //450 * TIT_MAXLENGTH_E;
        ERROR_LEN = 115; //TIT_MAXLENGTH_E;
    }
    else
    {
        Msg3_TITLE_ADD = (U16 *)ADD_BOR_MSG3;
        Msg3TitleLengthMax = 85;
        Msg3TitleOffest = 10;

        Toast_TITLE_ADD = ((U16 *)(tmpflashadd[86]));
        ToastTitleLengthMax = 31;

        UADD_ERROR_MSG = ADD_ERROR_MSG1;
        UADD_PROG_MSG7 = ADD_ERROR_MSG2;
        RwHelp_TITLE_ADD = MEM_TITLE_RWHELP;
        RwHelpTitleLengthMax = TIT_MAXLENGTH;
        ERROR_MSG = 24; //30;
        ERROR_LEN = 74; //105;
    }

    g_ERROR_LEN = ERROR_LEN;
    g_ERROR_MSG = ERROR_MSG;
    if ((g_light_c == 0) && (P_LOF == 1))
    {
        ViewPage(usepageno);
        WritePage(usepageno);
        SetLcdPwm(1, P_CNR); //开背光
        g_light_c = P_R * 1000;
    }
    if (para == 10)
    {
        if (g_para0 == 2)//不保存，更新数据
        { 
            tmpcmd = C_PATUPDATE;
        }
        else
        {
            tmpcmd = C_WRITEMEMORYPAT; //保存花样到记忆体
        }
    }
    else if (para == 81)
    {
        tmpcmd = C_WRITEFLASHPAT; //保存花样到U盘
    }
    // if (para != 9)
    // {
    //     g_logorun = 50;

    //     DrawColor_Gray1(0, 0, LCD_WIDTH, LCD_HEIGHT);
    //     DrawDFrame(rect[RECT_RWHELP_BG].xpis + SAFE_DIST2, rect[RECT_RWHELP_BG].ypis + SAFE_DIST2,
    //                rect[RECT_RWHELP_BG].xsize, rect[RECT_RWHELP_BG].ysize, SAFE_DIST3, rect[RECT_RWHELP_BG].colorB[1]);
    //     DrawFillRect(rect[RECT_RWHELP_BG].xpis, rect[RECT_RWHELP_BG].ypis,
    //                  rect[RECT_RWHELP_BG].xsize, rect[RECT_RWHELP_BG].ysize, rect[RECT_RWHELP_BG].colorB[0]);
    // }
    // else
    // {

    //     DrawColor_Gray1(0, 0, LCD_WIDTH, LCD_HEIGHT);
    //     DrawDFrame(rect[RECT_RWHELP_BG].xpis + SAFE_DIST2, rect[RECT_RWHELP_BG].ypis + SAFE_DIST2,
    //                rect[RECT_RWHELP_BG].xsize, rect[RECT_RWHELP_BG].ysize, SAFE_DIST3, rect[RECT_RWHELP_BG].colorB[1]);
    //     DrawFillRect(rect[RECT_RWHELP_BG].xpis, rect[RECT_RWHELP_BG].ypis,
    //                  rect[RECT_RWHELP_BG].xsize, rect[RECT_RWHELP_BG].ysize, rect[RECT_RWHELP_BG].colorB[0]);
    // }
    DrawColor_Gray1(0, 0, LCD_WIDTH, LCD_HEIGHT);
    DrawDFrame(rect[RECT_RWHELP_BG].xpis + SAFE_DIST2, rect[RECT_RWHELP_BG].ypis + SAFE_DIST2,
               rect[RECT_RWHELP_BG].xsize, rect[RECT_RWHELP_BG].ysize, SAFE_DIST3, rect[RECT_RWHELP_BG].colorB[1]);
    DrawFillRect(rect[RECT_RWHELP_BG].xpis, rect[RECT_RWHELP_BG].ypis,
                 rect[RECT_RWHELP_BG].xsize, rect[RECT_RWHELP_BG].ysize, rect[RECT_RWHELP_BG].colorB[0]);

    if (para != 9) //不是剩余存储界面
    {
        DrawFPBMPNEW(rect[RECT_RWHELP_BMP1].xpis, rect[RECT_RWHELP_BMP1].ypis,
                     64, 58, MEM_BMPGIF, rect[RECT_RWHELP_BMP1].index[0]);
        g_logorun = 100;
        ViewPage(usepageno);
    }
    // else //添加
    // {
    //     ViewPage(usepageno);
    // }

    // /[语言显示相关文件路径设置与页面初始化*end]************************************************

    // /[运行状态信息与需要执行的数据信息读取*begin]**********************************************************
    temp_para = para;
    otemp_para = temp_para;
    // /[运行状态信息与需要执行的数据信息读取*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        if (dis_updata)
        {
            wdt();
            dis_updata = 0;
            if (temp_para != 9)
            {
                if ((temp_para == 21) && (((tmp_error - 0x8000) >= 402) && ((tmp_error - 0x8000) <= 409)) && (P_LAN))
                {
                    // DrawFillRect(140, 190, 444, 96, bgColor);
                    // DrawFillRect(140, 304, 444, 160, bgColor);

                    DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                 rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize,
                                 rect[RECT_RWHELP_STR1].colorB[0]);
                    DrawFillRect(rect[RECT_RWHELP_STR2].xpis, rect[RECT_RWHELP_STR2].ypis,
                                 rect[RECT_RWHELP_STR2].xsize, rect[RECT_RWHELP_STR2].ysize,
                                 rect[RECT_RWHELP_STR2].colorB[0]);
                }
                else if (temp_para == 96)
                {
                    // DrawFillRect(100, 168, 500, 128, bgColor);

                    DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                 rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize,
                                 rect[RECT_RWHELP_STR1].colorB[0]);
                }
                else
                {
                    // DrawFillRect(140, 190, 444, 96, bgColor);
                    // DrawFillRect(140, 304, 444, 160, bgColor);
                    DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                 rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize,
                                 rect[RECT_RWHELP_STR1].colorB[0]);
                    DrawFillRect(rect[RECT_RWHELP_STR2].xpis, rect[RECT_RWHELP_STR2].ypis,
                                 rect[RECT_RWHELP_STR2].xsize, rect[RECT_RWHELP_STR2].ysize,
                                 rect[RECT_RWHELP_STR2].colorB[0]);
                }
            }

            switch (temp_para)
            {
            case 0: //提示删除记忆体文件-确认界面	
                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE22, TYPE_SIZE5, vTestColor);
                DrawFFilename(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2,
                              rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2 + SAFE_DIST4,
                              (char *)g_distmpName, 64, vTestColor, 24, 2, NULL);
                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 + TYPE_SIZE22 + SAFE_DIST6,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE9, TYPE_SIZE5, vTestColor);
                DrawFFileno(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2 + TYPE_SIZE22 + SAFE_DIST6,
                            rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                            g_selectnumber, vTestColor);
                // DrawRect(200, vaddr1, 300, 44, vTestColor);
                // DrawFFilename(208, vaddr1 + 10, (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                // DrawRect(510, vaddr1, 64, 44, vTestColor);
                // DrawFFileno(516, vaddr1 + 8, g_selectnumber, vTestColor);
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 230), linemaxdisplay, Err_MSG1_Sheet1, 230, l2, vTestColor); //可以删除内部记忆体里的缝纫资料吗?
                btn_en = 3;
                break;

            case 7: //删除记忆体花样-等待界面	(0->7)

                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE22, TYPE_SIZE5, vTestColor);
                DrawFFilename(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2,
                              rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                              (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 + TYPE_SIZE22 + SAFE_DIST6,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE9, TYPE_SIZE5, vTestColor);
                DrawFFileno(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2 + TYPE_SIZE22 + SAFE_DIST6,
                            rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                            g_selectnumber, vTestColor);
                // DrawRect(200, vaddr1, 300, 44, vTestColor);
                // DrawRect(510, vaddr1, 64, 44, vTestColor);
                // DrawFFilename(208, vaddr1 + 10, (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                // DrawFFileno(516, vaddr1 + 8, g_selectnumber, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 228), linemaxdisplay, Err_MSG1_Sheet1, 228, l2, vTestColor); //内部记忆体正在删除缝纫资料中...

                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);

                btn_en = 0;

                DREG[D_COMMAND] = C_DELMEMORYPAT; // 删除花样文件
                DREG[D_CMDSTATUS] = 0;
                DREG[D_CMDPARA1] = g_selectnumber;
                f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND])); // 发送指令
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                if (Read_Status() == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                else if (DREG[D_CMDSTATUS] == 100)//执行成功
                { 
                    dat = g_selectnumber - MINFILENO;
                    i = dat * FileIndexSize;
                    dat = i + FileIndexSize;
                    for (; i < dat; i++)
                        IREG2[i] = 0xffff; //清除删除花样的索引表

                    g_init_flag = 1;
                    pnoindex = 0;
                    pno = poldno[0]; //返回主目录
                    if (g_selectnumber != g_Fileno)
                    { //删除的花样不是当前使用花样
                        Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                        RestoreSHREG(0x03);
                    }
                    else
                    { //删除的花样是当前使用花样
                        for (i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
                        {
                            if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                            { //将当前花样改为内存中编号最小的花样
                                g_selectdisno = i / FileIndexSize + MINFILENO;
                                g_oldselectdisno = g_selectdisno;
                                break;
                            }
                        }
                        if (i >= D_Length[INDEX2])
                        { //将文件全部删除完成
                            for (i = 0; i < MAXSREG; i++)
                                SREG[i] = 0;
                            g_selectdisno = 0;
                            g_oldselectdisno = 0;
                            F_IND_NO = 0;
                            g_NoReadPatf = 1; //返回主界面不需重读花样数据
                            break;
                        }

                        if (Write_Cmd(C_READMEMORYPAT, g_selectdisno, 0) == 0)
                        { // 读花样文件
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] == 98)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] == 99)
                        {
                            temp_para = 3;
                            dis_updata = 1;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] < 98)
                        {
                            temp_para = 16;
                            dis_updata = 1;
                            break;
                        }

                        i = (g_selectdisno - MINFILENO) * FileIndexSize;
                        len = (((U32)(IREG2[i + 1]) << 16) + IREG2[i]) / 2;
                        f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样文件
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        else
                        {
                            for (i = len; i < MAXSREG; i++)
                                SREG[i] = 0;
                        }
                        g_NoReadPatf = 1; //返回主界面不需重读花样数据
                    }
                }
                break;

            case 1:  //提示删除SD卡文件
            case 78: //提示删除U盘文件
                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE22, TYPE_SIZE5, vTestColor);
                DrawFFilename(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2,
                              rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                              (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 + TYPE_SIZE22 + SAFE_DIST6,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE9, TYPE_SIZE5, vTestColor);
                DrawFFileno(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2 + TYPE_SIZE22 + SAFE_DIST6,
                            rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                            g_selectnumber, vTestColor);
                // DrawRect(200, vaddr1, 300, 44, vTestColor);
                // DrawRect(510, vaddr1, 64, 44, vTestColor);
                // DrawFFilename(208, vaddr1 + 10, (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                // DrawFFileno(516, vaddr1 + 8, g_selectnumber, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 211), linemaxdisplay, Err_MSG1_Sheet1, 211, l2, vTestColor); //可以删除磁碟片上的缝纫资料吗?
                btn_en = 3;
                break; //是否从磁盘删除资料

            case 2:
                ViewMsg(200);
                btn_en = 2;
                //PatF1();
                break; //磁盘不在磁碟机内

            case 3:
                ViewMsg(223);
                btn_en = 2;
                //PatF1();
                break;

            case 4:
                ViewMsg(204);
                btn_en = 2;
                //PatF1();
                break; //磁盘上找不到资料

            case 5: //读内存花样	直接返回主界面，因为在读取花样画面已经读出该花样
                ViewMsg(229);
                if ((g_selectnumber >= MINFILENO) && (g_selectnumber <= MAXFILENO))
                {
                    g_selectdisno = g_selectnumber;
                    g_oldselectdisno = g_selectdisno;

                    if (g_NoReadPatf == 2)
                    { //重新读取花样数据
                        if (Write_Cmd(C_READMEMORYPAT, g_selectnumber, 0) == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] == 98)
                        {
                            if (temp_para == 80)
                            {
                                temp_para = 2;
                                dis_updata = 1;
                            }
                            else
                            {
                                pnoindex = 0;
                                pno = poldno[0];
                            }
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] == 99)
                        {
                            if (temp_para == 5)
                            {
                                temp_para = 3;
                                dis_updata = 1;
                            }
                            else
                            {
                                temp_para = 4;
                                dis_updata = 1;
                            }
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] < 98)
                        {
                            temp_para = 16;
                            dis_updata = 1;
                            break;
                        }
                        f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (g_selectnumber != F_IND_NO)
                        {
                            temp_para = 16;
                            dis_updata = 1;
                            break;
                        }
                        n = (g_selectnumber - MINFILENO) * FileIndexSize;
                        len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                        f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样文件
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        else
                        {
                            if ((DataCheckSum((U16 *)(&SREG[0]), len, ((F_FILETYPE == PAT_MIT) ? 0 : 1))) == 0)
                            { //数据校验失败
                                temp_para = 64;
                                dis_updata = 1;
                                break;
                            }
                        }
                    }
                }
                g_NoReadPatf = 1; //返回主界面不需重读花样数据
                g_init_flag = 1;

                pnoindex = 0;
                pno = poldno[0];
                break;

            case 6:  //读SD卡花样
            case 80: //读U盘花样
                ViewMsg(209);
                g_selectdisno = g_selectnumber;
                if (g_NoReadPatf == 2)
                { //读取花样数据
                    if (g_disk_memory == 1)
                    {
                        if (g_MboardUSB == 1)
                        { //主板USB
                            if (Write_Cmd(C_READFLASHPAT, g_selectnumber, 0) == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                            if (Read_Status() == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                            if (DREG[D_CMDSTATUS] != 100)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                            f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                            if (f == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                            if (g_selectnumber != F_IND_NO)
                            {
                                temp_para = 16;
                                dis_updata = 1;
                                break;
                            }
                            len = (((U32)(F_FILELENH)) << 16) + F_FILELENL;
                            f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0]));
                            if (f == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                        }
                        else
                        { //打板器USB
                            Select_ExtMemory(USBTF);
                            Read_PatFile(g_selectnumber);
                        }
                    }
                    else
                    {
                        Select_ExtMemory(SD);
                        Read_PatFile(g_selectnumber);
                    }
                }
                len = (((U32)(F_FILELENH)) << 16) + F_FILELENL;
                //校验花样数据
                if (F_FILETYPE == PAT_MIT)
                { // 三菱款
                    i = len >> 1;
                    checksum = 0;
                    SREG[0] = g_selectnumber;
                    for (i = 0; i < (len >> 1); i++)
                    { //计算校验和
                        if (i != 0x20)
                            checksum += SREG[i];
                    }
                    SREG[0x20] = checksum;
                }
                else
                { // 兄弟
                    checksum = 0;
                    for (i = 0; i < (len >> 1); i++)
                    { //计算校验和
                        if (i != 0x05)
                            checksum += SREG[i];
                    }
                    SREG[0x05] = checksum;
                }

                n = (g_selectnumber - MINFILENO) * FileIndexSize;
                if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= (2 * MAXSREG)))
                { //记忆体内有相同编号花样
                    temp_para = 12;
                    dis_updata = 1; //是否覆盖原文件
                }
                else
                {
                    tmpcmd = C_WRITEMEMORYPAT; //保存花样命令
                    temp_para = 10;
                    dis_updata = 1; //写花样数据到内存
                }
                break;

            case 8:  // SD卡(1->8)
            case 79: //U盘(78->79)

                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE22, TYPE_SIZE5, vTestColor);
                DrawFFilename(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2,
                              rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                              (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                DrawRect(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 + TYPE_SIZE22 + SAFE_DIST6,
                         rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                         TYPE_SIZE9, TYPE_SIZE5, vTestColor);
                DrawFFileno(rect[RECT_RWHELP_STR1].xpis + SAFE_DIST4 * 2 + TYPE_SIZE22 + SAFE_DIST6,
                            rect[RECT_RWHELP_STR1].ypis + (rect[RECT_RWHELP_STR1].ysize - TYPE_SIZE5) / 2,
                            g_selectnumber, vTestColor);
                // DrawRect(200, vaddr1, 300, 44, vTestColor);
                // DrawRect(510, vaddr1, 64, 44, vTestColor);
                // DrawFFilename(208, vaddr1 + 10, (char *)g_distmpName, 64, vTestColor, 17, 2, NULL);
                // DrawFFileno(516, vaddr1 + 8, g_selectnumber, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 208), linemaxdisplay, Err_MSG1_Sheet1, 208, l2, vTestColor); //磁碟片正在删除缝纫资料中...

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                btn_en = 0;

                Select_ExtMemory(USBTF);

                if (Delete_PatFile(g_selectnumber))
                {
                    temp_para = 16;
                    dis_updata = 1;
                    break;
                }

                Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                RestoreSHREG(0x03);
                pnoindex = 0;
                pno = poldno[0];
                break;

            case 9: //查看记忆体剩除容量

                // for (i = 0; i < 9; i++)
                // {
                //     if (i == 0)
                //     {
                //     }
                //     else if (i % 2)
                //     {
                //         DrawFillRect(32, 92 + 56 * i, 440, 40, mCOLOR ? Color_Blue : Color_Black);
                //     }
                //     else if (i == 2 || i == 6)
                //     {
                //         DrawRect(144, 92 + 56 * i, 328, 40, Color_Black);
                //     }
                //     else
                //     {
                //         DrawRect(32, 92 + 56 * i, 440, 40, Color_Black);
                //     }

                //     Draw24x24Str(40, 100 + 56 * i, RwHelp_TITLE_ADD + (7 + (i == 6 ? 2 : (i < 6 ? i : i - 1))) * RwHelpTitleLengthMax, RwHelp_Sheet, 7 + (i == 6 ? 2 : (i < 6 ? i : i - 1)), (i % 2 == 0) ? Color_Black : Color_White);
                // }

                if (YREG[SYS_SUMFILE] > SUMFILENO)
                    YREG[SYS_SUMFILE] = 0;
                if (YREG[SYS_FUSEED] > YREG[SYS_FSIZE])
                    YREG[SYS_FUSEED] = 0;
                dat1 = YREG[SYS_SUMFILE] * 100 / SUMFILENO;
                if (YREG[SYS_FSIZE])
                    dat2 = YREG[SYS_FUSEED] * 100 / YREG[SYS_FSIZE];
                else
                    dat2 = 0;
                tmp_fsize = YREG[SYS_FSIZE];
                tmp_fuseed = YREG[SYS_FUSEED];
                tmp_sumfile = YREG[SYS_SUMFILE];

                // DrawMFS32P(480, 156, SUMFILENO, 5, 0, Color_Black);                            //总共写入针数
                // DrawMFS32P(160, 212, dat1, 3, 0, Color_Black);                                 //已使用针数百分比
                // DrawMFS32P(480, 268, YREG[SYS_SUMFILE], 5, 0, Color_Black);                    //已占用针数
                // DrawMFS32P(480, 324, (SUMFILENO - YREG[SYS_SUMFILE]), 5, 0, Color_Black);      //剩余可用针数
                // DrawMFS32P(480, 380, YREG[SYS_FSIZE], 5, 0, Color_Black);                      //记忆体总容量
                // DrawMFS32P(160, 436, dat2, 3, 0, Color_Black);                                 //已使用记忆体百分比
                // DrawMFS32P(480, 492, YREG[SYS_FUSEED], 5, 0, Color_Black);                     //已使用记忆体
                // DrawMFS32P(480, 548, (YREG[SYS_FSIZE] - YREG[SYS_FUSEED]), 5, 0, Color_Black); //未使用记忆体 YREG[SYS_COMEN]

                for (i = 2; i < RECT_RWHELP2_MAX; i++)
                {
                    DrawFillRect(rect2[i].xpis, rect2[i].ypis, rect2[i].xsize, rect2[i].ysize, rect2[i].colorB[0]);
                    Draw24x24Str(rect2[i].xpis + SAFE_DIST4,
                                 rect2[i].ypis + SAFE_DIST1,
                                 RwHelp_TITLE_ADD + rect2[i].index[0] * RwHelpTitleLengthMax,
                                 RwHelp_Sheet, rect2[i].index[0], rect2[i].colorT[0]);
                }

                DrawMFS32P(rect2[RECT_RWHELP2_PINSUM_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PINSUM_STR].ypis + SAFE_DIST1,
                           SUMFILENO, 5, 0, rect2[RECT_RWHELP2_PINSUM_STR].colorT[0]); //总共写入针数
                DrawMFS32P(rect2[RECT_RWHELP2_PINUSE_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PINUSE_STR].ypis + SAFE_DIST1,
                           YREG[SYS_SUMFILE], 5, 0, rect2[RECT_RWHELP2_PINUSE_STR].colorT[0]); //已占用针数
                DrawMFS32P(rect2[RECT_RWHELP2_PATSUM_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PATSUM_STR].ypis + SAFE_DIST1,
                           YREG[SYS_FSIZE], 5, 0, rect2[RECT_RWHELP2_PATSUM_STR].colorT[0]); //记忆体总容量
                DrawMFS32P(rect2[RECT_RWHELP2_PATUSE_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PATUSE_STR].ypis + SAFE_DIST1,
                           YREG[SYS_FUSEED], 5, 0, rect2[RECT_RWHELP2_PATUSE_STR].colorT[0]); //已使用记忆体

                ViewPage(usepageno);
                btn_en = 2;
                break;

            case 10: //写花样到内存
                ViewMsg(227);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                if ((g_Funtype == 1) && (F_FILETYPE == PAT_SEW))//打板生成的兄弟款花样数据
                { 
                    Bor_2deal();
                }
                CheckDrawScale(0, F_FILETYPE, F_FILELEN, 0); //更新一下范围数据
                len = (((U32)(F_FILELENH)) << 16) + F_FILELENL;
                f = Write_D(D_Address[PTDATA], (len >> 1), (U16 *)(&SREG[0])); //发送花样数据
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    errorf = 0;
                    break;
                }
                f = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //发送头文件信息
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    errorf = 0;
                    break;
                }

                if (Write_Cmd(tmpcmd, F_IND_NO, 0) == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    errorf = 0;
                    break;
                }
                if (Read_Status() == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    errorf = 0;
                }
                else if (DREG[D_CMDSTATUS] == 98)
                {
                    temp_para = 2;
                    dis_updata = 1;
                    errorf = 0;
                }
                else if (DREG[D_CMDSTATUS] == 100)
                {
                    U32 addr = 0;
                    pnoindex = 0;
                    pno = poldno[0];
                    g_selectdisno = F_IND_NO;
                    if (g_para0 == 2)
                    {
                        g_oldselectdisno = g_savePatInfo.No;
                    }
                    else if (tmpcmd == C_WRITEMEMORYPAT)
                    {
                        g_oldselectdisno = g_selectdisno;
                    }
                    g_disk_memory = 0;
                    g_NoReadPatf = 1; //返回主界面不需重读花样数据
                    //ReadPatList(0,pno);		//更新内存索引
                    //g_updataIREG = 1;
                    if (tmpcmd == C_WRITEMEMORYPAT)
                    { //更新文件索引
                        addr = (F_IND_NO - MINFILENO) * PTINDEXREDLEN;
                        IREG2[addr + PTINDEXLENADD + 0] = len;
                        IREG2[addr + PTINDEXLENADD + 1] = len >> 16;
                        IREG2[addr + PTINDEXENCADD + 0] = 0;
                        IREG2[addr + PTINDEXENCADD + 1] = 0;
                        for (i = 0; i < 32; i++)
                        {
                            IREG2[addr + PTINDEXNAMEADD + i] = HREG[32 + i];
                        }
                        if (F_FILETYPE == PAT_MIT)//三菱
                        { 
                            IREG2[addr + PTINDEXNAMEADD + 31] &= 0x00ff;
                            IREG2[addr + PTINDEXNAMEADD + 31] |= 0x0100;
                        }
                        else//兄弟
                        { 
                            IREG2[addr + PTINDEXNAMEADD + 31] &= 0x00ff;
                        }
                    }
                }
                else if (DREG[D_CMDSTATUS] == 12)
                { //通讯出错，重发
                    errorf++;
                    dis_updata = 1;
                }
                else
                {
                    temp_para = 16;
                    dis_updata = 1;
                    errorf = 0;
                }
                if (errorf >= 3)
                { //通讯出错超过3次，报警
                    errorf = 0;
                    temp_para = 15;
                    dis_updata = 1;
                }
                btn_en = 0;
                break;

            case 12: //提示复盖存在的文件
                ViewMsg(231);
                btn_en = 3;
                break;

            case 11: //写花样到 SD卡
            case 81: //写花样到 U盘
                ViewMsg(207);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);

                if ((g_Funtype == 1) && (F_FILETYPE == PAT_SEW))
                { //打板生成的兄弟款花样数据
                    Bor_2deal();
                }
                //打板器U盘,SD卡
                for (i = 0; i < 3; i++)
                {
                    //if(temp_para==81){
                    Select_ExtMemory(USBTF);
                    //}else{
                    //	Select_ExtMemory(SD);
                    //}

                    if (g_exwriteType == 1)
                    {
                        len = F_FILELEN;
                        Bor_TO_Juki(&len);
                        HREG[4] = len & 0xffff; //文件长度
                        HREG[5] = (len >> 16) & 0xffff;
                    }

                    if (Write_PatFile(F_IND_NO, g_exwriteType) == 0)
                    {
                        errorf = 0;
                        pnoindex = 0;
                        pno = poldno[0];
                        //ReadPatList(0,pno);		//更新内存索引
                        //g_updataIREG = 1;
                        RestoreIREG(); //恢复内存索引
                        Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                        RestoreSHREG(0x03);
                        break;
                    }
                    else
                        errorf++;
                }

                if (errorf >= 3)
                { //通讯出错超过3次，报警
                    errorf = 0;
                    temp_para = 15;
                    dis_updata = 1;
                }
                btn_en = 0;
                break;

            case 13: //压板未放下
                ViewMsg(3);
                ComErrMessage = 1;
                btn_en = 3;
                DREG[D_ERRWAR] = 0x8003;
                Write_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR])); //发送错误码给主板
                S_ERRORSET();
                Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS); //发送错误标志给主板
                break;

            case 14: //当前花样未保存
                ViewMsg(22);
                btn_en = 3;
                break; 
            case 15: //通讯错误
                ViewMsg(23);
                btn_en = 2;
                DelayMs(500);
                i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
                if ((i) && (YREG[SYS_COMEN] == 0x5a5a))//-正常后回到上一个页面
                { 
                    pnoindex--;
                    pno = poldno[pnoindex];
                    btn_en = 0;
                    ComErrMessage = 0;
                }
                else
                {
                    ComErrMessage = 1;
                    btn_en = 2;
                }
                break; 

            case 16: //操作失败-正常后回到当前页面
                ViewMsg(24);
                btn_en = 2;
                break;

            case 17: //不在原点
                ViewMsg(7);
                ComErrMessage = 1;
                btn_en = 2;
                //S_ERRORSET();
                //Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS); //发送错误码给主板
                break;

            case 18:
                ViewMsg(25);
                btn_en = 4;
                if (P_HPK == 1)
                    btn_en += 2;
                break; //回原点,第二次按回原点

            case 19: //启终点错误
                ViewMsg(26);
                btn_en = 1;
                break;

            case 20: //坐标超出加工范围
                ViewMsg(27);
                btn_en = 1;
                DREG[D_COMMAND] = C_READPOSI;
                DREG[D_CMDSTATUS] = 0;
                f = Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
                if (f == 0)
                {
                    dis_updata = 1;
                    temp_para = 15;
                    break;
                }
                for (i = 0; i < RDTIMES; i++)
                {
                    f = Read_D(D_Address[DISP] + D_CMDSTATUS, 3, (U16 *)(&DREG[D_CMDSTATUS]));
                    if ((f == 0) || (DREG[D_CMDSTATUS]))
                        break;
                    DelayMs(WAITTIME);
                }
                if ((f == 0) && (DREG[D_CMDSTATUS] == 0))
                {
                    dis_updata = 1;
                    temp_para = 15;
                    break;
                }
                else if (DREG[D_CMDSTATUS] == 100)
                {
                    g_dxpos = (S16)(DREG[D_CMDPARA1]);
                    g_dypos = (S16)(DREG[D_CMDPARA2]);

                    if (F_FILETYPE == PAT_MIT)
                    { //三菱
                        g_dxpos -= g_tmpoxpos;
                        g_dypos -= g_tmpoypos;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    {
                        g_dxpos -= (g_tmpoxpos / 2);
                        g_dypos -= (g_tmpoypos / 2);
                    }
                }
                break;

            case 21: //  错误警告信息显示
                wdt();
                btn_en = 1;
                err_btn_en = 1;
                tmp_error = DREG[D_ERRWAR];
                ComErrMessage = 1;

                if ((tmp_error & 0x8000) == 0)
                { //E
                    dat = tmp_error;
                    if (dat == 3)
                    { //主轴机卡死

                        Read_D(D_Address[DISP] + D_DNCOUNT1, 5, (U16 *)(&DREG[D_DNCOUNT1]));
                        ErrCode = (DREG[D_DNCOUNT1 + 2] << 16) | DREG[D_DNCOUNT1 + 3];
                        ErrCode2 = DREG[D_DERR1];

                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_PROG_MSG7 + ERROR_LEN * dat), linemaxdisplay, Err_MSG2_Sheet1, dat, l1, vTestColor);
                        if ((ErrCode == 0) && (ErrCode2 == 0))
                        {
                            Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_PROG_MSG7 + ERROR_LEN * dat + ERROR_MSG), linemaxdisplay, Err_MSG2_Sheet2, dat, l2, vTestColor);
                        }
                        else
                        {
                            U16 yy = vaddr2;
                            if (ErrCode != 0)
                            { //M
                                yy += 32;
                                Draw24X24FFStr(disPosStr_X, vaddr2, (U16 *)"M-Err", Color_Red);
                                DrawMFHEX(disPosStr_X + 12 * 5, vaddr2, ErrCode, 3, Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode)
                                {
                                case 0x090:
                                    ExErrDis = 0;
                                    break;
                                case 0x0a0:
                                case 0x0a1:
                                    ExErrDis = 1;
                                    break;
                                case 0x0a2:
                                case 0x0a3:
                                case 0x0a4:
                                    ExErrDis = 2;
                                    break;
                                case 0x0a5:
                                    ExErrDis = 3;
                                    break;
                                case 0x0a6:
                                    ExErrDis = 4;
                                    break;
                                case 0x0b0:
                                    ExErrDis = 5;
                                    break;
                                case 0x0c0:
                                    ExErrDis = 6;
                                    break;
                                case 0x0d0:
                                    ExErrDis = 7;
                                    break;
                                case 0x0d1:
                                    ExErrDis = 8;
                                    break;
                                case 0x0e0:
                                    ExErrDis = 9;
                                    break;
                                case 0x0e1:
                                    ExErrDis = 10;
                                    break;
                                case 0x0f0:
                                    ExErrDis = 11;
                                    break;
                                case 0x100:
                                    ExErrDis = 12;
                                    break;
                                case 0x120:
                                    ExErrDis = 13;
                                    break;
                                case 0x150:
                                    ExErrDis = 14;
                                    break;
                                case 0x151:
                                    ExErrDis = 15;
                                    break;
                                case 0x152:
                                    ExErrDis = 16;
                                    break;
                                case 0x170:
                                    ExErrDis = 17;
                                    break;
                                case 0x180:
                                    ExErrDis = 18;
                                    break;
                                case 0x190:
                                    ExErrDis = 19;
                                    break;
                                case 0x1a0:
                                    ExErrDis = 20;
                                    break;
                                case 0x1a1:
                                    ExErrDis = 21;
                                    break;
                                case 0x1b0:
                                    ExErrDis = 22;
                                    break;
                                case 0x210:
                                    ExErrDis = 23;
                                    break;
                                case 0x211:
                                    ExErrDis = 24;
                                    break;
                                case 0x212:
                                    ExErrDis = 25;
                                    break;
                                case 0x240:
                                    ExErrDis = 26;
                                    break;
                                case 0x260:
                                    ExErrDis = 27;
                                    break;
                                case 0x270:
                                    ExErrDis = 28;
                                    break;
                                case 0x271:
                                    ExErrDis = 29;
                                    break;
                                case 0x272:
                                    ExErrDis = 30;
                                    break;
                                case 0x570:
                                    ExErrDis = 31;
                                    break;
                                case 0x5f0:
                                    ExErrDis = 32;
                                    break;
                                default:
                                    break;
                                }
                                if (ExErrDis < 33)
                                {
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, vaddr2, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base + ExErrDis, l2, vTestColor);
                                }
                            }

                            if (ErrCode2 != 0)
                            { //D
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"D-Err", Color_Red);
                                DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode2, 3, Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode2)
                                {
                                case 0x090:
                                    ExErrDis = 0;
                                    break;
                                case 0x0a0:
                                case 0x0a1:
                                    ExErrDis = 1;
                                    break;
                                case 0x0a2:
                                case 0x0a3:
                                case 0x0a4:
                                    ExErrDis = 2;
                                    break;
                                case 0x0a5:
                                    ExErrDis = 3;
                                    break;
                                case 0x0a6:
                                    ExErrDis = 4;
                                    break;
                                case 0x0b0:
                                    ExErrDis = 5;
                                    break;
                                case 0x0c0:
                                    ExErrDis = 6;
                                    break;
                                case 0x0d0:
                                    ExErrDis = 7;
                                    break;
                                case 0x0d1:
                                    ExErrDis = 8;
                                    break;
                                case 0x0e0:
                                    ExErrDis = 9;
                                    break;
                                case 0x0e1:
                                    ExErrDis = 10;
                                    break;
                                case 0x0f0:
                                    ExErrDis = 11;
                                    break;
                                case 0x100:
                                    ExErrDis = 12;
                                    break;
                                case 0x120:
                                    ExErrDis = 13;
                                    break;
                                case 0x150:
                                    ExErrDis = 14;
                                    break;
                                case 0x151:
                                    ExErrDis = 15;
                                    break;
                                case 0x152:
                                    ExErrDis = 16;
                                    break;
                                case 0x170:
                                    ExErrDis = 17;
                                    break;
                                case 0x180:
                                    ExErrDis = 18;
                                    break;
                                case 0x190:
                                    ExErrDis = 19;
                                    break;
                                case 0x1a0:
                                    ExErrDis = 20;
                                    break;
                                case 0x1a1:
                                    ExErrDis = 21;
                                    break;
                                case 0x1b0:
                                    ExErrDis = 22;
                                    break;
                                case 0x210:
                                    ExErrDis = 23;
                                    break;
                                case 0x211:
                                    ExErrDis = 24;
                                    break;
                                case 0x212:
                                    ExErrDis = 25;
                                    break;
                                case 0x240:
                                    ExErrDis = 26;
                                    break;
                                case 0x260:
                                    ExErrDis = 27;
                                    break;
                                case 0x270:
                                    ExErrDis = 28;
                                    break;
                                case 0x271:
                                    ExErrDis = 29;
                                    break;
                                case 0x272:
                                    ExErrDis = 30;
                                    break;
                                case 0x570:
                                    ExErrDis = 31;
                                    break;
                                case 0x5f0:
                                    ExErrDis = 32;
                                    break;
                                default:
                                    break;
                                }
                                if (ExErrDis < 33)
                                {
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base + ExErrDis, l2, vTestColor);
                                }
                            }
                        }
                    }
                    else if (dat == 70)
                    { //电子板报警
                        Read_D(D_Address[DISP] + D_XERR1, 2, (U16 *)(&DREG[D_XERR1]));
                        ErrCode3 = DREG[D_XERR1];
                        ErrCode4 = DREG[D_YERR1];

                        Read_D(D_Address[DISP] + D_XERR2, 5, (U16 *)(&DREG[D_XERR2]));
                        ErrCode5 = DREG[D_XERR2];
                        ErrCode6 = DREG[D_YERR2];
                        ErrCode7 = DREG[D_ZERR2];
                        ErrCode8 = DREG[D_UERR2];
                        ErrCode9 = DREG[D_VERR2];

                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_PROG_MSG7 + ERROR_LEN * dat), linemaxdisplay, Err_MSG2_Sheet1, dat, l1, vTestColor);
                        if ((ErrCode3 == 0) && (ErrCode4 == 0) && (ErrCode5 == 0) && (ErrCode6 == 0) && (ErrCode7 == 0) && (ErrCode8 == 0) && (ErrCode9 == 0))
                        {
                            Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_PROG_MSG7 + ERROR_LEN * dat + ERROR_MSG), linemaxdisplay, Err_MSG2_Sheet2, dat, l2, vTestColor);
                        }
                        else
                        {
                            U16 yy = vaddr2;
                            if (ErrCode3 != 0)
                            { //X
                                yy += 32;
                                Draw24X24FFStr(disPosStr_X, vaddr2, (U16 *)"X-Err", Color_Red);
                                DrawMFHEX(disPosStr_X + 12 * 5, vaddr2, ErrCode3, 3, Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode3)
                                {
                                case 0x090:
                                    ExErrDis = 0;
                                    break;
                                case 0x0a0:
                                case 0x0a1:
                                    ExErrDis = 1;
                                    break;
                                case 0x0a2:
                                case 0x0a3:
                                case 0x0a4:
                                    ExErrDis = 2;
                                    break;
                                case 0x0a5:
                                    ExErrDis = 3;
                                    break;
                                case 0x0a6:
                                    ExErrDis = 4;
                                    break;
                                case 0x0b0:
                                    ExErrDis = 5;
                                    break;
                                case 0x0c0:
                                    ExErrDis = 6;
                                    break;
                                case 0x0d0:
                                    ExErrDis = 7;
                                    break;
                                case 0x0d1:
                                    ExErrDis = 8;
                                    break;
                                case 0x0e0:
                                    ExErrDis = 9;
                                    break;
                                case 0x0e1:
                                    ExErrDis = 10;
                                    break;
                                case 0x0f0:
                                    ExErrDis = 11;
                                    break;
                                case 0x100:
                                    ExErrDis = 12;
                                    break;
                                case 0x120:
                                    ExErrDis = 13;
                                    break;
                                case 0x150:
                                    ExErrDis = 14;
                                    break;
                                case 0x151:
                                    ExErrDis = 15;
                                    break;
                                case 0x152:
                                    ExErrDis = 16;
                                    break;
                                case 0x170:
                                    ExErrDis = 17;
                                    break;
                                case 0x180:
                                    ExErrDis = 18;
                                    break;
                                case 0x190:
                                    ExErrDis = 19;
                                    break;
                                case 0x1a0:
                                    ExErrDis = 20;
                                    break;
                                case 0x1a1:
                                    ExErrDis = 21;
                                    break;
                                case 0x1b0:
                                    ExErrDis = 22;
                                    break;
                                case 0x210:
                                    ExErrDis = 23;
                                    break;
                                case 0x211:
                                    ExErrDis = 24;
                                    break;
                                case 0x212:
                                    ExErrDis = 25;
                                    break;
                                case 0x240:
                                    ExErrDis = 26;
                                    break;
                                case 0x260:
                                    ExErrDis = 27;
                                    break;
                                case 0x270:
                                    ExErrDis = 28;
                                    break;
                                case 0x271:
                                    ExErrDis = 29;
                                    break;
                                case 0x272:
                                    ExErrDis = 30;
                                    break;
                                case 0x570:
                                    ExErrDis = 31;
                                    break;
                                case 0x5f0:
                                    ExErrDis = 32;
                                    break;
                                default:
                                    break;
                                }
                                if (ExErrDis < 33)
                                {
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, vaddr2, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base + ExErrDis, l2, vTestColor);
                                }
                            }

                            if (ErrCode4 != 0)
                            { //Y
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"Y-Err", Color_Red);
                                DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode4, 3, Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode4)
                                {
                                case 0x090:
                                    ExErrDis = 0;
                                    break;
                                case 0x0a0:
                                case 0x0a1:
                                    ExErrDis = 1;
                                    break;
                                case 0x0a2:
                                case 0x0a3:
                                case 0x0a4:
                                    ExErrDis = 2;
                                    break;
                                case 0x0a5:
                                    ExErrDis = 3;
                                    break;
                                case 0x0a6:
                                    ExErrDis = 4;
                                    break;
                                case 0x0b0:
                                    ExErrDis = 5;
                                    break;
                                case 0x0c0:
                                    ExErrDis = 6;
                                    break;
                                case 0x0d0:
                                    ExErrDis = 7;
                                    break;
                                case 0x0d1:
                                    ExErrDis = 8;
                                    break;
                                case 0x0e0:
                                    ExErrDis = 9;
                                    break;
                                case 0x0e1:
                                    ExErrDis = 10;
                                    break;
                                case 0x0f0:
                                    ExErrDis = 11;
                                    break;
                                case 0x100:
                                    ExErrDis = 12;
                                    break;
                                case 0x120:
                                    ExErrDis = 13;
                                    break;
                                case 0x150:
                                    ExErrDis = 14;
                                    break;
                                case 0x151:
                                    ExErrDis = 15;
                                    break;
                                case 0x152:
                                    ExErrDis = 16;
                                    break;
                                case 0x170:
                                    ExErrDis = 17;
                                    break;
                                case 0x180:
                                    ExErrDis = 18;
                                    break;
                                case 0x190:
                                    ExErrDis = 19;
                                    break;
                                case 0x1a0:
                                    ExErrDis = 20;
                                    break;
                                case 0x1a1:
                                    ExErrDis = 21;
                                    break;
                                case 0x1b0:
                                    ExErrDis = 22;
                                    break;
                                case 0x210:
                                    ExErrDis = 23;
                                    break;
                                case 0x211:
                                    ExErrDis = 24;
                                    break;
                                case 0x212:
                                    ExErrDis = 25;
                                    break;
                                case 0x240:
                                    ExErrDis = 26;
                                    break;
                                case 0x260:
                                    ExErrDis = 27;
                                    break;
                                case 0x270:
                                    ExErrDis = 28;
                                    break;
                                case 0x271:
                                    ExErrDis = 29;
                                    break;
                                case 0x272:
                                    ExErrDis = 30;
                                    break;
                                case 0x570:
                                    ExErrDis = 31;
                                    break;
                                case 0x5f0:
                                    ExErrDis = 32;
                                    break;
                                default:
                                    break;
                                }
                                if (ExErrDis < 33)
                                {
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base + ExErrDis, l2, vTestColor);
                                }
                                yy += 32;
                            }

                            if (ErrCode5 != 0)
                            { //步进X
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"X-Err", Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode5)
                                {
                                case 1:
                                    ExErrDis = 0;
                                    break;
                                case 2:
                                    ExErrDis = 1;
                                    break;
                                case 64:
                                    ExErrDis = 2;
                                    break;
                                case 128:
                                    ExErrDis = 3;
                                    break;
                                case 512:
                                    ExErrDis = 4;
                                    break;
                                case 256:
                                    ExErrDis = 5;
                                    break;
                                case 32:
                                    ExErrDis = 6;
                                    break;
                                }
                                if (ExErrDis < 7)
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ExErrDis + 1, 3, Color_Red);
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base1 + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base1 + ExErrDis, l2, vTestColor);
                                }
                                else
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode5, 3, vTestColor);
                                }
                                yy += 32;
                            }

                            if (ErrCode6 != 0)
                            { //步进Y
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"Y-Err", Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode6)
                                {
                                case 1:
                                    ExErrDis = 0;
                                    break;
                                case 2:
                                    ExErrDis = 1;
                                    break;
                                case 64:
                                    ExErrDis = 2;
                                    break;
                                case 128:
                                    ExErrDis = 3;
                                    break;
                                case 512:
                                    ExErrDis = 4;
                                    break;
                                case 256:
                                    ExErrDis = 5;
                                    break;
                                case 32:
                                    ExErrDis = 6;
                                    break;
                                }
                                if (ExErrDis < 7)
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ExErrDis + 1, 3, Color_Red);
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base1 + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base1 + ExErrDis, l2, vTestColor);
                                }
                                else
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode6, 3, vTestColor);
                                }
                                yy += 32;
                            }

                            if (ErrCode7 != 0)
                            { //步进Z
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"Z-Err", Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode7)
                                {
                                case 1:
                                    ExErrDis = 0;
                                    break;
                                case 2:
                                    ExErrDis = 1;
                                    break;
                                case 64:
                                    ExErrDis = 2;
                                    break;
                                case 128:
                                    ExErrDis = 3;
                                    break;
                                case 512:
                                    ExErrDis = 4;
                                    break;
                                case 256:
                                    ExErrDis = 5;
                                    break;
                                case 32:
                                    ExErrDis = 6;
                                    break;
                                }
                                if (ExErrDis < 7)
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ExErrDis + 1, 3, Color_Red);
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base1 + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base1 + ExErrDis, l2, vTestColor);
                                }
                                else
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode7, 3, vTestColor);
                                }
                                yy += 32;
                            }

                            if (ErrCode8 != 0)
                            { //步进U
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"U-Err", Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode8)
                                {
                                case 1:
                                    ExErrDis = 0;
                                    break;
                                case 2:
                                    ExErrDis = 1;
                                    break;
                                case 64:
                                    ExErrDis = 2;
                                    break;
                                case 128:
                                    ExErrDis = 3;
                                    break;
                                case 512:
                                    ExErrDis = 4;
                                    break;
                                case 256:
                                    ExErrDis = 5;
                                    break;
                                case 32:
                                    ExErrDis = 6;
                                    break;
                                }
                                if (ExErrDis < 7)
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ExErrDis + 1, 3, Color_Red);
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base1 + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base1 + ExErrDis, l2, vTestColor);
                                }
                                else
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode8, 3, vTestColor);
                                }
                                yy += 32;
                            }

                            if (ErrCode9 != 0)
                            { //步进V
                                Draw24X24FFStr(disPosStr_X, yy, (U16 *)"V-Err", Color_Red);
                                ExErrDis = 0xff;
                                switch (ErrCode9)
                                {
                                case 1:
                                    ExErrDis = 0;
                                    break;
                                case 2:
                                    ExErrDis = 1;
                                    break;
                                case 64:
                                    ExErrDis = 2;
                                    break;
                                case 128:
                                    ExErrDis = 3;
                                    break;
                                case 512:
                                    ExErrDis = 4;
                                    break;
                                case 256:
                                    ExErrDis = 5;
                                    break;
                                case 32:
                                    ExErrDis = 6;
                                    break;
                                }
                                if (ExErrDis < 7)
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ExErrDis + 1, 3, Color_Red);
                                    Draw24x24MulStr(disPosStr_X + 12 * 8, yy, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base1 + ExErrDis)), linemaxdisplay - 8, Err_MSG2_Sheet1, err_base1 + ExErrDis, l2, vTestColor);
                                }
                                else
                                {
                                    DrawMFHEX(disPosStr_X + 12 * 5, yy, ErrCode9, 3, vTestColor);
                                }
                                yy += 32;
                            }
                        }
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_PROG_MSG7 + ERROR_LEN * dat), linemaxdisplay, Err_MSG2_Sheet1, dat, l1, vTestColor);
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_PROG_MSG7 + ERROR_LEN * dat + ERROR_MSG), linemaxdisplay, Err_MSG2_Sheet2, dat, l2, vTestColor);
                    }

                    ///DelayMs(4500);
                    ///Bell_On(ALARMSOUND);
                }
                else
                { // M
                    dat = tmp_error - 0x8000;
                    if ((dat >= 402) && (dat <= 409) && (P_LAN))
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * dat), linemaxdisplay, Err_MSG1_Sheet1, dat, l1, vTestColor);
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * dat + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, dat, l2, vTestColor);
                    }
                    else
                    {
                        if (dat == 388)
                        {
                            pno = MENU_RWHELP1;
                            if (poldno[pnoindex - 1] == MENU_RWHELP)
                                pnoindex--;
                            break;
                        }
                        if (dat != 13)
                        {
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * dat), linemaxdisplay, Err_MSG1_Sheet1, dat, l1, vTestColor);
                            if (dat == 385)
                            {
                                Read_D(D_Address[DISP] + 19, 1, (U16 *)(&DREG[19]));
                                if (DREG[19] > 0 && DREG[19] < 15)
                                {
                                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_PROG_MSG7 + ERROR_LEN * (err_base2 + DREG[19] - 1)), linemaxdisplay, Err_MSG2_Sheet1, err_base2 + DREG[19] - 1, l2, vTestColor);
                                }
                                else
                                {
                                    Draw24X24FFStr(disPosStr_X, vaddr2, (U16 *)"Err-", Color_Red);
                                    DrawMFHEX(disPosStr_X + 12 * 5, vaddr2, DREG[19], 3, Color_Red);
                                }
                            }
                            else
                            {
                                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * dat + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, dat, l2, vTestColor);
                            }
                        }
                    }
                    if ((dat == 1) || (dat == 117))
                    {
                        btn_en = 10;
                        PINERR = DREG[D_ERRWAR];
                    }
                    else if (dat == 25)
                    {
                        btn_en = 4;
                    }
                    else if (dat == 13 || dat == 391)
                    {
                        if (dat == 391)
                        { //远程升级
                            Timerms = 0;
                            btn_en = 3;
                        }
                        else
                        { //读远程花样
                            U8 haspat = 0;
                            BackupSHREG(0x02);
                            Write_Cmd(C_RECEIVE_FILE, 0, 0); //发送确认接收远程文件指令
                            for (i = 0; i < RDTIMES * 8; i++)
                            {
                                f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                                if (DREG[D_CMDSTATUS])
                                    break;
                                DelayMs(WAITTIME);
                            }
                            if (DREG[D_CMDSTATUS] == 100)
                            {
                                f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                                for (i = 0; i < MAXIREG2; i += FileIndexSize)
                                {
                                    if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                                    {
                                        if ((i / FileIndexSize + MINFILENO) == F_IND_NO)
                                        {
                                            haspat = 1;
                                            break;
                                        }
                                    }
                                }
                            }
                            RestoreSHREG(0x02);
                            if (haspat == 1)
                            { //内存中存在此花样
                                btn_en = 3;
                                Write_Cmd(C_READMEMORYPAT, F_IND_NO, 0);
                                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * dat), linemaxdisplay, Err_MSG1_Sheet1, dat, l1, vTestColor);
                                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * dat + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, dat, l2, vTestColor);
                            }
                            else
                            {
                                g_NoUpdataoldno = 1;
                                g_NoReadPatf = 1;
                                g_backSREG_f = 1;
                                g_backHREG_f = 1;
                                g_Fileno = F_IND_NO;
                                g_backSREGlen = (F_FILELEN) / 2;
                                BackupSHREG((g_backHREG_f << 1) | g_backSREG_f);

                                //									f = Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&F_IND_NO));	// 读当前花样文件信息

                                Write_Cmd(C_RECEIVE_FILE, 0, 0); //发送确认接收远程文件指令
                                for (i = 0; i < RDTIMES * 8; i++)
                                {
                                    f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                                    if (DREG[D_CMDSTATUS])
                                        break;
                                    DelayMs(WAITTIME);
                                }
                                if (DREG[D_CMDSTATUS] == 100)
                                {
                                    f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                                    if (f)
                                    {
                                        len = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
                                        f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样文件
                                        if (f == 0)
                                        {
                                            temp_para = 15;
                                            dis_updata = 1;
                                        }
                                        else
                                        {
                                            if (Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0) == 0)
                                            { //保存花样到内存
                                                temp_para = 15;
                                                dis_updata = 1;
                                                break;
                                            }
                                            if (Read_Status() == 0 || DREG[D_CMDSTATUS] != 100)
                                            {
                                                temp_para = 15;
                                                dis_updata = 1;
                                                break;
                                            }
                                            btn_en = 0;
                                            temp_para = 156;
                                            dis_updata = 1;
                                            //												if(F_FILETYPE==1){
                                            //													checksum = 0;
                                            //													for(i=0;i<len;i++){
                                            //														if(i!=0x20){checksum +=SREG[i];}
                                            //													}
                                            //													SREG[0x20] = checksum;
                                            //													Write_D(D_Address[PTDATA]+0x20,1,(U16 *)(&SREG[0x20])); // 发送当前花样文件校验值
                                            //												}else{
                                            //													checksum = 0;
                                            //													for(i=0;i<len;i++){
                                            //														if(i!=0x05){checksum +=SREG[i];}
                                            //													}
                                            //													SREG[0x05] = checksum;
                                            //													Write_D(D_Address[PTDATA]+0x05,1,(U16 *)(&SREG[0x05])); // 发送当前花样文件校验值
                                            //												}
                                            ReadFileIndex(C_RDMEMPATLIST, plastno); //更新文件索引
                                        }
                                    }
                                    else
                                    {
                                        temp_para = 15;
                                        dis_updata = 1;
                                    }
                                }
                                else
                                {
                                    temp_para = 15;
                                    dis_updata = 1;
                                }
                            }
                        }
                    }
                    //						else if(dat == 392){
                    //							btn_en = 0;
                    //							pno = MENU_FTCHAT;
                    //						}
                }
                //#if Debug
                //printf("tmp_error = %04x dat = %d btn_en = %d \r\n",tmp_error,dat,btn_en);
                //#endif
                break;

            case 22: //  错误信息显示
                btn_en = 1;
                tmp_error = EREG[1];
                dat = (EREG[1] & 0xfff0) >> 4;
                viewTextIndex = 0;
                if (dat == 7)
                { //112
                    viewTextIndex = 14;
                }
                else if (dat == 8)
                { //128
                    viewTextIndex = 15;
                }
                else if (dat == 9)
                { //144
                    viewTextIndex = 74;
                }
                else if (dat == 10)
                { //160
                    viewTextIndex = 60;
                }
                else if (dat == 11)
                { //176
                    viewTextIndex = 128;
                }
                else if (dat == 12)
                { //192
                    viewTextIndex = 250;
                }

                if (viewTextIndex)
                {
                    ViewMsg(viewTextIndex);
                }
                break;

            case 23:
                ViewMsg(27);
                btn_en = 1;
                break; //坐标超出加工范围

            case 24: //格式化内存
                ViewMsg(235);
                btn_en = 3;
                break;

            case 26: //格式化内存  24->26
                ViewMsg(disPosStr_X + 12 * 5);
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                Write_Cmd(C_FORMATMEMORY, 1, 2); // 初始化
                for (i = 0; i < RDTIMES * 50; i++)
                {
                    f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                    if ((DREG[D_CMDSTATUS]))
                        break;
                    DelayMs(WAITTIME);
                }

                if (DREG[D_CMDSTATUS] == 100)
                {                                                                   //格式化成功
                    f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 读取参数值
                    n = ReadFileIndex(C_RDMEMPATLIST, MENU_FTFORMAT);

                    if (n)
                    {
                        for (j = 0, n = 0; n < D_Length[INDEX2]; n += FileIndexSize)
                        {
                            if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                                j++;
                        }
                        g_Fmax = j / 10;
                        if ((g_Fmax == 0) || (j % 10))
                            g_Fmax++;
                        if (g_Fmax < g_fno)
                            g_fno = 1;
                    }
                    else
                    {
                        for (n = 0; n < D_Length[INDEX2]; n += FileIndexSize)
                        {
                            IREG2[n] = 0xffff;
                        }
                        g_Fmax = 1;
                        g_fno = 1;
                        j = 0;
                    }

                    if (j)
                    {
                        n = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                        if (n)
                        {
                            j = F_IND_NO;
                            if ((j >= MINFILENO) && (j <= MAXFILENO))
                            {
                                n = (j - MINFILENO) * FileIndexSize;
                                len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                                n = 0; // 如果读花样失败，初始化SREG寄存器
                                if ((len > 0) && (len <= MAXSREG))
                                {
                                    n = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样数据
                                    if (n)
                                    {
                                        g_init_flag = 1;
                                        g_selectdisno = F_IND_NO;
                                        g_oldselectdisno = F_IND_NO;
                                    }
                                }
                            }
                        }
                    }
                    pno = MENU_FUNCTION;
                    pnoindex = 1;
                }
                else
                { //格式化失败
                    temp_para = 28;
                    dis_updata = 1;
                    break;
                }
                break;

            case 25: //格式化SD
            case 82: //格式化usb
                ViewMsg(236);
                btn_en = 3;
                break;

            case 27: //格式化 SD卡   25->27
            case 83: //格式化U盘 81->83
                ViewMsg(205);

                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                Select_ExtMemory(USBTF);
                if (Format_Disk() != 0)
                {
                    temp_para = 28;
                    dis_updata = 1;
                    break;
                }
                //	}
                //}
                g_disk_memory = MEM_IN; //内存
                pno = MENU_FUNCTION;
                pnoindex = 1;
                ReadFileIndex(C_RDMEMPATLIST, pno);
                break;

            case 28:                                                                                                                                 //格式化失败
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 29), linemaxdisplay, Err_MSG1_Sheet1, 29, l1, vTestColor); //格式化失败!
                if ((g_disk_memory == 0) /*||((g_disk_memory==1)&&(g_MboardUSB==1))*/)
                {                                                                                                                                                    //主板存储设备
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 29 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 29, l2, vTestColor); //请检查系统设备通讯线之间的连接!
                }
                btn_en = 2;
                break;

            case 31: //复制选择花样资料  从内存复制到磁盘
                     //打板器
                if (ExtMemory_Status(USBTF) == 0)
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 33), linemaxdisplay, Err_MSG1_Sheet1, 33, l1, vTestColor);               //是否可以复制指定的图型资料到U盘?
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 315 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 315, l2, vTestColor); //相同编号上如有资料会被盖上新资料,原有的资料会消失
                    btn_en = 3;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //无存储设备!
                    btn_en = 2;
                }
                break;

            case 33:                                                                                                                                   //  从内存复制指定文件到磁盘	(84->31->33)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 301), linemaxdisplay, Err_MSG1_Sheet1, 301, l1, vTestColor); //正在从内部记忆体复制花样资料到磁盘存储器中...
                                                                                                                                                       //					Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*301+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,301,2,Color_White);	//请稍候
                if (P_LAN == 2)//提示语
                {                                                                                                                       //越南语
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X + 25 * 4, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }

                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                SelectNoCnt = 0;
                for (i = 0; i < DISSUM; i++)
                {
                    SelectNoFino[i] = 0xffff;
                }
                for (i = 0; i < DISSUM; i++)
                {
                    if (g_SelectNo[i].Fileno != 0xffff)
                    { //此空间可以存信息,选择按键
                        SelectNoFino[SelectNoCnt] = g_SelectNo[i].Fileno;
                        SelectNoCnt++;
                    }
                }
                if (P_LAN == 2)//总共文件数
                {                                                            //越南语
                    DrawMFU32Z(206, vaddr2 + 34, SelectNoCnt, 3, Color_Red); //待修改
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, SelectNoCnt, 3, Color_Red);
                }
                //TO 打板器U盘 SD
                for (val = 0; val < SelectNoCnt; val++)
                {
                    wdt();
                    if ((SelectNoFino[val] >= MINFILENO) && (SelectNoFino[val] <= MAXFILENO))
                    {
                        n = (SelectNoFino[val] - MINFILENO) * FileIndexSize;
                        if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                        {
                            for (j = 0; j < FileIndexSize; j++)
                                file_meg[j] = IREG2[n + j];

                            result = Write_Cmd(C_READMEMORYPAT, n / FileIndexSize + MINFILENO, 0);
                            if (result)
                            {
                                for (i = 0; i < (RDTIMES + RDTIMES); i++)
                                {
                                    result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                                    if ((DREG[D_CMDSTATUS]))
                                        break;
                                    DelayMs(WAITTIME);
                                }
                            }

                            Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                            if (F_IND_NO == (n / FileIndexSize + MINFILENO))
                            {
                                ReSREGcount = 0;
                                while (ReSREGcount < 3)
                                {
                                    result = Read_D(D_Address[PTDATA], (((U32)(file_meg[1]) << 16) + (U32)(file_meg[0])) / 2, (U16 *)(&SREG[0]));
                                    if (CheckSREG(2, ((((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1), ((file_meg[1] << 16) | file_meg[0])) == TRUE)
                                        break;
                                    ReSREGcount += 1;
                                    wdt();
                                }
                                if (ReSREGcount >= 3)
                                {
                                    if (result)
                                        continue;
                                }
                                if (result)
                                {
                                    checksum = 0;
                                    if (((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0)
                                    { //三菱款格式
                                        len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                        for (i = 0; i < len; i++)
                                        { //计算校验和
                                            if (i != 0x20)
                                                checksum += SREG[i];
                                        }
                                        SREG[0x20] = checksum;
                                    }
                                    else
                                    { //兄弟格式
                                        len = ((file_meg[1] << 16) | file_meg[0]) / 2;
                                        for (i = 0; i < len; i++)
                                        { //计算校验和
                                            if (i != 0x05)
                                                checksum += SREG[i];
                                        }
                                        SREG[0x05] = checksum;
                                    }

                                    if (P_LAN == 2)//当前执行的文件数
                                    { //越南语
                                        DrawFillRect(460, vaddr2, 38, 24, bgColor);
                                        DrawMFU32Z(460, vaddr2, val + 1, 3, Color_Red);
                                    }
                                    else
                                    {
                                        DrawFillRect(disPosStr_X + 25 * 5, vaddr2, TYPE_SIZE6, TYPE_SIZE4, bgColor);
                                        DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2, val + 1, 3, Color_Red);
                                    }
                                    flag = 0;
                                    //TO USB
                                    if (ExtMemory_Status(USBTF) == 0)
                                    {
                                        flag = 1;
                                        Select_ExtMemory(USBTF);
                                    }
                                    //}else{	//TO SD
                                    //	if(ExtMemory_Status(SD)==0){flag=1;Select_ExtMemory(SD);}
                                    //}
                                    if (flag)
                                    {
                                        Delete_PatFile(F_IND_NO);   //先删除原文件 有没有都删除一下
                                        Write_PatFile(F_IND_NO, 0); //写入花样数据
                                    }
                                }
                            }
                        }
                    }
                }

                Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                RestoreSHREG(0x03);
                pno = MENU_FUNCTION;
                pnoindex = 1;
                break;

            case 37: //复制全部花样到外部储存器
                //打板器
                if (ExtMemory_Status(USBTF) == 0)
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 35), linemaxdisplay, Err_MSG1_Sheet1, 35, l1, vTestColor);               //是否可以复制所有的图型资料到U盘?
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 316 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 316, l2, vTestColor); //相同编号上如有资料会被盖上新资料,原有的资料会消失
                    btn_en = 3;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //无存储设备!
                    btn_en = 2;
                }
                break;

            case 35:                                                                                                                                   //从内存复制全部资料到磁盘	(84->37->35)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 301), linemaxdisplay, Err_MSG1_Sheet1, 301, l1, vTestColor); //正在从内部记忆体复制花样资料到磁盘存储器中...
                                                                                                                                                       //					Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*301+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,301,2,Color_White);	//请稍候
                if (P_LAN == 2)
                {                                                                                                                       //越南语
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X + 25 * 4, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }
                btn_en = 0;

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                filecount = 0;
                for (i = 0; i < MAXIREG2;)
                {
                    if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                        filecount++;
                    i += FileIndexSize; //2013.3.5
                }
                if (P_LAN == 2)
                { //越南语
                    DrawMFU32Z(206, vaddr2 + 34, filecount, 3, Color_Red);
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, filecount, 3, Color_Red);
                }
                //if((g_MboardUSB == 1)&&(g_disk_memory == 1)){	//TO 主板上U盘
                //	CopyFileCom(C_COPYMEMORYPAT, SelectNoFino, 100);
                //}else{	//TO 打板器U盘 SD
                filecount = 0;
                for (n = 0; n < MAXIREG2; n += FileIndexSize)
                {
                    wdt();
                    if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                    {
                        filecount++;
                        if (P_LAN == 2)
                        { //越南语
                            DrawFillRect(460, vaddr2, 38, 24, bgColor);
                            DrawMFU32Z(460, vaddr2, filecount, 3, Color_Red);
                        }
                        else
                        {
                            DrawFillRect(disPosStr_X + 25 * 5, vaddr2, TYPE_SIZE6, TYPE_SIZE4, bgColor);
                            DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2, filecount, 3, Color_Red);
                        }
                        for (j = 0; j < FileIndexSize; j++)
                            file_meg[j] = IREG2[n + j];
                        //DelayMs(1000);
                        result = Write_Cmd(C_READMEMORYPAT, n / FileIndexSize + MINFILENO, 0);
                        if (result)
                        {
                            for (i = 0; i < (RDTIMES + RDTIMES); i++)
                            {
                                result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                                if ((DREG[D_CMDSTATUS]))
                                    break;
                                DelayMs(WAITTIME);
                            }
                        }
                        if ((DREG[D_CMDSTATUS] != 100))
                        {
                            continue;
                        }

                        Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                        if (F_IND_NO == (n / FileIndexSize + MINFILENO))
                        {
                            ReSREGcount = 0;
                            while (ReSREGcount < 3)
                            {
                                result = Read_D(D_Address[PTDATA], (((U32)(file_meg[1]) << 16) + (U32)(file_meg[0])) / 2, (U16 *)(&SREG[0]));
                                if (CheckSREG(2, ((((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1), ((file_meg[1] << 16) | file_meg[0])) == TRUE)
                                    break;
                                ReSREGcount += 1;
                                wdt();
                            }
                            if (ReSREGcount >= 3)
                            {
                                if (result)
                                    continue;
                            }
                            if (result)
                            {
                                checksum = 0;
                                if (((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0)
                                { //三菱款格式
                                    len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                    for (i = 0; i < len; i++)
                                    { //计算校验和
                                        if (i != 0x20)
                                            checksum += SREG[i];
                                    }
                                    SREG[0x20] = checksum;
                                }
                                else
                                { //兄弟格式
                                    len = ((file_meg[1] << 16) | file_meg[0]) / 2;
                                    for (i = 0; i < len; i++)
                                    { //计算校验和
                                        if (i != 0x05)
                                            checksum += SREG[i];
                                    }
                                    SREG[0x05] = checksum;
                                }

                                flag = 0;
                                //if(g_disk_memory==1){	//TO USB
                                if (ExtMemory_Status(USBTF) == 0)
                                {
                                    flag = 1;
                                    Select_ExtMemory(USBTF);
                                }
                                //}else{	//TO SD
                                //	if(ExtMemory_Status(SD)==0){flag=1;Select_ExtMemory(SD);}
                                //}
                                if (flag)
                                {
                                    Delete_PatFile(F_IND_NO);   //先删除原文件 有没有都删除一下
                                    Write_PatFile(F_IND_NO, 0); //写入花样数据
                                }
                            }
                        }
                    }
                }

                Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                RestoreSHREG(0x03);
                pno = MENU_FUNCTION;
                pnoindex = 1;
                break;

            case 32: //从磁盘复制到指定花样资料内存
                ViewMsg(315);
                btn_en = 3;
                break;

            case 34:                                                                                                                                   //从磁盘复制选定文件到内存	(32->34)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 300), linemaxdisplay, Err_MSG1_Sheet1, 300, l1, vTestColor); // 正在从磁盘存储器复制花样资料到内部记忆体中...

                if (P_LAN == 2)
                {                                                                                                                       //越南语
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X + 25 * 4, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }

                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                SelectNoCnt = 0;
                for (i = 0; i < DISSUM; i++)
                {
                    SelectNoFino[i] = 0xffff;
                }
                for (i = 0; i < DISSUM; i++)
                {
                    if (g_SelectNo[i].Fileno != 0xffff)
                    { //此空间可以存信息,选择按键
                        SelectNoFino[SelectNoCnt] = g_SelectNo[i].Fileno;
                        SelectNoCnt++;
                    }
                }
                if (P_LAN == 2)
                { //越南语
                    DrawMFU32Z(206, vaddr2 + 34, SelectNoCnt, 3, Color_Red);
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, SelectNoCnt, 3, Color_Red);
                }
                //if(( g_MboardUSB == 1)&&(g_disk_memory == 1)){	//From 主板上U盘
                //	CopyFileCom(C_COPYFLASHPAT, SelectNoFino, SelectNoCnt);
                //}else{		//From 打板器上U盘/SD
                /*if(g_disk_memory==1)*/ {
                    Select_ExtMemory(USBTF);
                    Explore_Disk("0:/", 1);
                }
                //else{Select_ExtMemory(SD);Explore_Disk ("1:/", 1);}

                if ((SelectNoCnt > 0) && (SelectNoCnt < 100))
                {
                    for (val = 0; val < SelectNoCnt; val++)
                    {

                        if (P_LAN == 2)
                        { //越南语
                            DrawFillRect(460, vaddr2, 38, 24, bgColor);
                            DrawMFU32Z(460, vaddr2, val, 3, Color_Red);
                        }
                        else
                        {
                            DrawFillRect(disPosStr_X + 25 * 5, vaddr2, TYPE_SIZE6, TYPE_SIZE4, bgColor);
                            DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2, val, 3, Color_Red);
                        }

                        wdt();
                        if ((SelectNoFino[val] >= MINFILENO) && (SelectNoFino[val] <= MAXFILENO))
                        {
                            n = (SelectNoFino[val] - MINFILENO) * FileIndexSize;
                            if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                            {
                                for (j = 0; j < FileIndexSize; j++)
                                    file_meg[j] = IREG2[n + j];
                                ReSREGcount = 0;
                                while (ReSREGcount < 3)
                                {
                                    Read_PatFile(SelectNoFino[val]);
                                    len = (F_FILELENH << 16) + F_FILELENL;
                                    if (CheckSREG(1, ((((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1), ((F_FILELENH << 16) | F_FILELENL)) == TRUE)
                                        break;
                                    ReSREGcount += 1;
                                    wdt();
                                }
                                if (ReSREGcount >= 3)
                                {
                                    continue;
                                }

                                ReSREGcount = 0;
                                while (ReSREGcount < 5)
                                {
                                    result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // 发送当前花样文件
                                    if (result == 0)
                                    { //通讯错误
                                        break;
                                    }
                                    result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //发送当前文件列表
                                    if (result == 0)
                                    { //通讯错误
                                        break;
                                    }
                                    result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //发送保存花样命令
                                    if (result == 0)
                                    { //通讯错误
                                        break;
                                    }
                                    if (Read_Status() == 0)
                                    {
                                        break;
                                    }
                                    else if (DREG[D_CMDSTATUS] == 98)
                                    {
                                        ReSREGcount++;
                                    }
                                    else if (DREG[D_CMDSTATUS] == 100)
                                    { //成功
                                        break;
                                    }
                                    else if (DREG[D_CMDSTATUS] == 12)
                                    { //通讯出错，重发
                                        ReSREGcount++;
                                    }
                                    else
                                    {
                                        ReSREGcount++;
                                    }
                                }
                            }
                        }
                    }
                }

                Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                Read_Status();
                Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO));              // 读当前花样文件信息
                Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // 读当前花样文件

                pno = MENU_FUNCTION;
                pnoindex = 1;
                ReadFileIndex(C_RDMEMPATLIST, MENU_FUNCTION);
                break;

            case 38: //从磁盘复制所有花样资料到内存
                ViewMsg(316);
                btn_en = 3;
                break;

            case 36:                                                                                                                                   //从磁盘复制所有资料到内存	(38->36)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 300), linemaxdisplay, Err_MSG1_Sheet1, 300, l1, vTestColor); // 正在从磁盘存储器复制花样资料到内部记忆体中...
                                                                                                                                                       //					Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*300+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,300,2,Color_White);
                                                                                                                                                       //					Draw24x24Str(511,256,RwHelp_TITLE_ADD+17*RwHelpTitleLengthMax,RwHelp_Sheet,17,Color_White);		// 总  个
                if (P_LAN == 2)
                {                                                                                                                       //越南语
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //正在复制第  个图型资料
                    Draw24x24Str(disPosStr_X + 25 * 5, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // 总  个
                }
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                //if(( g_MboardUSB == 1)&&(g_disk_memory == 1)){	//From 主板上U盘
                //	CopyFileCom(C_COPYFLASHPAT, SelectNoFino, 100);
                //}else{		//From 打板器上U盘/SD
                /*if(g_disk_memory==1)*/ {
                    Select_ExtMemory(USBTF);
                    Explore_Disk("0:/", 1);
                }
                //else{Select_ExtMemory(SD);Explore_Disk ("1:/", 1);}

                filecount = 0;
                for (i = 0; i < MAXIREG2; i += FileIndexSize)
                {
                    if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                        filecount++;
                }
                //DrawMFU32Z(548, 256, filecount, 3, Color_Red);	//文件总数
                if (P_LAN == 2)
                { //越南语
                    DrawMFU32Z(206, vaddr2 + 34, filecount, 3, Color_Red);
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, filecount, 3, Color_Red);
                }
                filecount = 0;

                for (n = 0; n < MAXIREG2; n += FileIndexSize)
                {
                    wdt();
                    if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                    {
                        filecount++;
                        //								DrawFillRect(280,256,38,24,Color_Blue);
                        //								DrawMFU32Z(280, 256,filecount, 3, Color_Red);
                        if (P_LAN == 2)
                        { //越南语
                            DrawFillRect(460, vaddr2, 38, 24, bgColor);
                            DrawMFU32Z(460, vaddr2, filecount, 3, Color_Red);
                        }
                        else
                        {
                            DrawFillRect(disPosStr_X + 25 * 5, vaddr2, TYPE_SIZE6, TYPE_SIZE4, bgColor);
                            DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2, filecount, 3, Color_Red);
                        }
                        for (j = 0; j < FileIndexSize; j++)
                            file_meg[j] = IREG2[n + j];

                        ReSREGcount = 0;
                        while (ReSREGcount < 3)
                        {
                            Read_PatFile(n / FileIndexSize + MINFILENO);
                            len = (F_FILELENH << 16) + F_FILELENL;
                            if (CheckSREG(1, ((((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1), ((F_FILELENH << 16) | F_FILELENL)) == TRUE)
                                break;
                            ReSREGcount += 1;
                            wdt();
                        }
                        if (ReSREGcount >= 3)
                        {
                            continue;
                        }

                        ReSREGcount = 0;
                        while (ReSREGcount < 5)
                        {
                            result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // 发送当前花样文件
                            if (result == 0)
                            { //通讯错误
                                break;
                            }
                            result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //发送当前文件列表
                            if (result == 0)
                            { //通讯错误
                                break;
                            }
                            result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //发送保存花样命令
                            if (result == 0)
                            {
                                break;
                            }
                            if (Read_Status() == 0)
                            {
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 98)
                            {
                                ReSREGcount++;
                            }
                            else if (DREG[D_CMDSTATUS] == 100)
                            { //成功
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 12)
                            { //通讯出错，重发
                                ReSREGcount++;
                            }
                            else
                            {
                                ReSREGcount++;
                            }
                        }
                    }
                }

                Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                Read_Status();
                Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO));              // 读当前花样文件信息
                Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // 读当前花样文件

                pno = MENU_FUNCTION;
                pnoindex = 1;
                ReadFileIndex(C_RDMEMPATLIST, MENU_FUNCTION);
                break;

            case 39:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 36), linemaxdisplay, Err_MSG1_Sheet1, 36, l1, vTestColor); //内部记忆体无文件
                btn_en = 2;
                break;

            case 40:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //磁盘无文件
                btn_en = 2;
                break;

            case 41:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 38), linemaxdisplay, Err_MSG1_Sheet1, 38, l1, vTestColor); //	是否备份功能设定表到磁盘?
                btn_en = 3;
                break; //是否备份设定表?

            case 42:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 39), linemaxdisplay, Err_MSG1_Sheet1, 39, l1, vTestColor); //是否还原功能设定表的内容?
                btn_en = 3;
                break; //是否还原设定表?

            case 43:
                ViewMsg(45);
                //正在备份设定表
                btn_en = 0;
                result = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 从FLASH读取参数值
                if (result)
                {
                }
                else
                    f = C_ERROR; // 操作失败	2
                //Write_D(D_Address[OPRA]+26,1,(U16 *)(&OPRA_DISPCMDSTATUS));

                //						SearchavailabDir();
                //						f = MemoryDrv_Opera(C_BACKUPPARA,0);       ///待修改！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
                //
                if (f == C_SUCCEED)
                { //成功
                    pno = MENU_FUNCTION;
                    pnoindex = 1;
                }
                else
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                break;

            case 44:
                ViewMsg(46);
                btn_en = 0;
                //正在还原设定表

                if (f == C_SUCCEED)
                {                                                                   //成功
                    f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 读取参数值
                    pno = MENU_FUNCTION;
                    pnoindex = 1;
                }
                else
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                break;

            case 45:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 47), linemaxdisplay, Err_MSG1_Sheet1, 47, l1, vTestColor); //是否备份控制程序到磁盘?
                btn_en = 3;
                break; //是否备份控制表?

            case 46:
                ViewMsg(48);
                //正在备份控制表
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                DelayMs(3000);
                pno = MENU_FUNCTION;
                pnoindex = 1;
                break;

            case 47:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 49), linemaxdisplay, Err_MSG1_Sheet1, 49, l1, vTestColor); //是否还原控制程序?
                btn_en = 3;
                break; //是否还原控制表?

            case 48:
                ViewMsg(78);
                //正在还原控制表
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                DelayMs(3000);
                pno = MENU_FUNCTION;
                pnoindex = 1;
                break;

            case 49:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 79), linemaxdisplay, Err_MSG1_Sheet1, 79, l1, vTestColor); //是否备份功能设定表和控制程序到磁盘?
                btn_en = 3;
                break; //是否备份设定表和控制程序?

            case 50:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 80), linemaxdisplay, Err_MSG1_Sheet1, 80, l1, vTestColor); //是否还原设定表和控制程序?
                btn_en = 3;
                break;

            case 51:
                ViewMsg(81);
                //正在备份设定表和控制程序
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                Write_Cmd(C_BACKUPPARA, 1, 2);
                DelayMs(8000);
                //for(i=0;i<RDTIMES;i++){
                //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                //	if((DREG[D_CMDSTATUS]))break;
                //	DelayMs(WAITTIME);
                //}
                Read_Status();
                if (DREG[D_CMDSTATUS] == 100)
                { //成功
                    pno = MENU_FUNCTION;
                    pnoindex = 1;
                }
                else
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                break;

            case 52:
                ViewMsg(82);
                //正在还原设定表和控制程序
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                Write_Cmd(C_RECOVERYPARA, 1, 2);
                DelayMs(3000);
                Read_Status();
                //for(i=0;i<RDTIMES;i++){
                //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                //	if((DREG[D_CMDSTATUS]))break;
                //	DelayMs(WAITTIME);
                //}

                if (DREG[D_CMDSTATUS] == 100)
                {                                                                   //成功
                    f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 读取参数值
                    pno = MENU_FUNCTION;
                    pnoindex = 1;
                }
                else
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                break;

            case 54:
                ViewMsg(124);
                btn_en = 1;
                break;

            case 55:
                ViewMsg(83);
                btn_en = 1;
                break;

            case 56:
                ViewMsg(247);
                btn_en = 1;
                break; //输入档案号码不正

            case 57:
                Read_D(D_Address[SYS] + SYS_RETURN_MD, 2, (U16 *)(&YREG[SYS_RETURN_MD]));
                if ((YREG[SYS_RETURN_MD] == 0x9999) && (YREG[SYS_RETURN_Y] == 0x9999))
                {
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 399 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 399, l2, vTestColor); //使用期限无限制!
                }
                else
                {
                    if ((YREG[SYS_RETURN_Y] & 0x0100))
                    {                                                                                                                                                      //试用期
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 398 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 398, l2, vTestColor); //试用有效期到    年  月  日
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 398), linemaxdisplay, Err_MSG1_Sheet1, 398, l2, vTestColor); //使用有效期到    年  月  日
                    }
                    if (P_LAN) //待修改
                    {
                        if ((YREG[SYS_RETURN_Y] & 0x0100))
                        { //试用期
                            temp = (U8)(YREG[SYS_RETURN_Y]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(112, vaddr1, 32, 16, bgColor);
                            DrawFU32(112, vaddr1, 2000 + temp, 4, vTestColor); //年

                            temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(152, vaddr1, 16, 16, bgColor);
                            DrawFU32(152, vaddr1, temp, 2, vTestColor); //月

                            temp = (U8)(YREG[SYS_RETURN_MD]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(176, vaddr1, 16, 16, bgColor);
                            DrawFU32(176, vaddr1, temp, 2, vTestColor); //日
                        }
                        else
                        {
                            temp = (U8)(YREG[SYS_RETURN_Y]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(232, vaddr1, 32, 16, bgColor);
                            DrawFU32(232, vaddr1, 2000 + temp, 4, vTestColor); //年

                            temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(disPosStr_X + 12 * 8, vaddr1, 16, 16, bgColor);
                            DrawFU32(disPosStr_X + 12 * 8, vaddr1, temp, 2, vTestColor); //月

                            temp = (U8)(YREG[SYS_RETURN_MD]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(296, vaddr1, 16, 16, bgColor);
                            DrawFU32(296, vaddr1, temp, 2, vTestColor); //日
                        }
                    }
                    else
                    {
                        temp = (U8)(YREG[SYS_RETURN_Y]);
                        temp = BCD_2Hex(temp);
                        DrawFillRect(168, vaddr1, 32, 16, bgColor);
                        DrawFU32(168, vaddr1, 2000 + temp, 4, vTestColor); //年

                        temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                        temp = BCD_2Hex(temp);
                        DrawFillRect(216, vaddr1, 16, 16, bgColor);
                        DrawFU32(216, vaddr1, temp, 2, vTestColor); //月

                        temp = (U8)(YREG[SYS_RETURN_MD]);
                        temp = BCD_2Hex(temp);
                        DrawFillRect(248, vaddr1, 16, 16, bgColor);
                        DrawFU32(248, vaddr1, temp, 2, vTestColor); //日
                    }
                }
                Draw24x24MulStr(disPosStr_X, vaddr1 + 28, (uint *)(USE_ERROR_MSG + ERROR_LEN * 399), linemaxdisplay, Err_MSG1_Sheet1, 399, 2, vTestColor); //	密码输入成功!
                btn_en = 1;
                break;

            case 58:
                ViewMsg(302);
                btn_en = 1;
                break;

            case 59:
                ViewMsg(397);
                btn_en = 1;
                break;

            case 60:
                if ((g_light_c == 0) && (P_LOF == 1))// 在通讯故障警告提示状态下要开背光
                { 
                    ViewPage(usepageno);
                    SetLcdPwm(1, P_CNR); //开背光
                    g_light_c = P_R * 1000;
                    DelayMs(10);
                }
                ViewMsg(84);
                DelayMs(200);
                i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));

                if ((i) && (YREG[SYS_COMEN] == 0x5a5a))
                { //如果通讯正常，就直接退出
                    pnoindex--;
                    pno = poldno[pnoindex];
                    btn_en = 0;
                    ComErrMessage = 0;
                }
                else
                {
                    ComErrMessage = 1;
                    btn_en = 2;
                }
                break; //通讯故障

            case 61:                                                                                                                                 //备份系统参数
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 85), linemaxdisplay, Err_MSG1_Sheet1, 85, l1, vTestColor); //是否备份系统设置到内存?
                btn_en = 3;
                break;

            case 62:                                                                                                                                 //恢复内存
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 86), linemaxdisplay, Err_MSG1_Sheet1, 86, l1, vTestColor); //是否还原系统设置?
                btn_en = 3;
                break;

            case 63:                                                                                                                                 //恢复出厂
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 87), linemaxdisplay, Err_MSG1_Sheet1, 87, l1, vTestColor); //是否还原系统出厂设置?
                btn_en = 3;
                break;

            case 64:
                ViewMsg(88);
                btn_en = 2;
                break;

            case 65: //图形超出范围限制,更换机型
                ViewMsg(89);
                btn_en = 2;
                break;

            case 66: //指定错误位置
                ViewMsg(128);
                btn_en = 2;
                break;

            case 67:
                ViewMsg(90);
                btn_en = 2; //中压脚超出行程范围
                break;

            case 68:

                if (ExtMemory_Status(USBTF) == 0)
                {
                    Select_ExtMemory(USBTF);
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 91), linemaxdisplay, Err_MSG1_Sheet1, 91, l1, vTestColor); //是否升级系统																		btn_en = 3;
                    btn_en = 3;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l2, vTestColor); //无存储设备
                    btn_en = 2;
                }

                break;

            case 69:
                upflag = 0;
                f = 0; // 外部存储器有效

                j = 0;
                if (ExtMemory_Status(USB) != 0)
                {
                    f = 1;
                }

#ifdef OLD_BOOT
                if (f)
                {                                                                                                                                        // 未连接
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //无存储设备
                    btn_en = 2;
                }
                else
                {
                    BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                    BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                    ii = AnalyseUpdateList(j, 0);
                    //						#if Debug
                    //						printf("-------- ii = %x g_upWIFI = %d g_upFLASH = %d----------\r\n",ii,g_upWIFI,g_upFLASH);
                    //						#endif

                    if ((ii & 0x00000002) && (g_upDISP))
                    { //H系统打扮器程序,备份到flash
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // 正在更新数据...
                        if (copy_dispprogram(j, 4, bgColor, 0, 1) == TRUE)
                        {
                            W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //写升级文件信息
                            upflag = 1;
                        }
                    }

                    if ((ii & 0x80000) && (g_upWIFI))
                    { //wifi
                        errCnt = 0;
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 455), linemaxdisplay, Err_MSG1_Sheet1, 455, l1, vTestColor); //正在升级WIFI模块...
                        while (errCnt < 3)
                        {
                            if (update_mcpuprogram(j, 15, 4, bgColor) == TRUE)
                            {
                                f |= 0x10;
                                if (g_MboardUSB == 1)
                                {
                                    DelayMs(2000);
                                }
                                errCnt = 3;
                                break;
                            }
                            else
                            {
                                errCnt++;
                            }
                        }
                        //DelayMs(2000);
                    }

                    if (g_MachineTy < 16)
                    {
                        i = 0x00000010;
                        i <<= g_MachineTy;
                        if ((ii & i) && (g_upMBOARD == 1))
                        {
                            errCnt = 0;
                            DrawFillRect(140, 190, 444, 96, bgColor);
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 93), linemaxdisplay, Err_MSG1_Sheet1, 93, 2, vTestColor); //正在升级主板...
                            while (errCnt < 3)
                            {
                                if (update_mcpuprogram(j, g_MachineTy, 4, bgColor) == TRUE)
                                {
                                    f |= 0x01;
                                    if (g_MboardUSB == 1)
                                    {
                                        DelayMs(2000);
                                    }
                                    errCnt = 3;
                                    break;
                                }
                                else
                                {
                                    errCnt++;
                                }
                            }
                        }
                    }

                    //						#if Debug
                    //						printf("ii = %0x g_upFLASH = %d \r\n",ii,g_upFLASH);
                    //						#endif
                    if ((ii & 0x00000004) && (g_upFLASH))
                    { //升级FLASH

                        errFlag = 0;
                        errCnt = 0;
                        g_logorun = 0;
                        while (errCnt < 3)
                        {
                            DrawFillRect(140, 190, 444, 96, bgColor);
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 95), linemaxdisplay, Err_MSG1_Sheet1, 95, 2, vTestColor); //正在升级打板器FLASH
                            SETWCS1();                                                                                                                          //取消片选
                            SETWCS2();                                                                                                                          //取消片选
                            SETTFCS();
                            errFlag = 0;
                            UpdataF = 0xffff;

                            if (update_flash(j, 4, bgColor, 0) == TRUE)
                            { // 升级FLASH
#ifndef USESST39VFXX
                                //写入数据后重读校验，不成功继续3次
                                W25QXX_Read(W25Q1_DAT, (U8 *)checkbuff, FLASHINDEXADDR + 24, 12);
                                maxadd = (((U32)checkbuff[1]) << 16) + checkbuff[0];
                                //									#if Debug
                                //									printf("maxadd = %08x: SDRAMFLASHADD = %8x errCnt = %d\r\n",maxadd,SDRAMFLASHADD,errCnt);
                                //									#endif
                                if ((maxadd > FLASHINDEXADDR) && (maxadd < 0x800000))
                                {
                                    W25QXX_Read(W25Q1_DAT, (U8 *)Bank1ADDR, 0, maxadd);

                                    checkadd = (U16 *)(Bank1ADDR + FLASHINDEXADDR + 28);
                                    listlen = ((*(checkadd + 1)) << 16) + (*checkadd);
                                    tmpchecksum = (*(checkadd + 2));

                                    checksum = 0;
                                    checkadd = (U16 *)(Bank1ADDR + FLASHINDEXADDR + 36);
                                    padd = (U32 *)(Bank1ADDR + FLASHINDEXADDR + 36);

                                    listlen -= FLASHINDEXADDR;
                                    listlen /= 12;
                                    listlen -= 3;
                                    //#if Debug
                                    //printf("000listlen = %d \r\n",listlen);
                                    //#endif
                                }

                                //#if Debug
                                //printf("--listlen = %d \r\n",listlen);
                                //#endif
                                for (cnti = 1; cnti <= listlen; cnti++)
                                {
                                    for (cntj = 0; cntj < 5; cntj++)
                                    {
                                        checksum += (*checkadd);
                                        checkadd++;
                                    }
                                    tmpflashadd[cnti] = (*padd);
                                    if (tmpflashadd[cnti] >= maxadd)
                                    {
                                        tmpflashadd[cnti] = 0;
                                        flashdataflag[cnti] = FALSE;
                                    }
                                    else
                                    {
                                        flashdataflag[cnti] = TRUE;
                                    }
                                    tmpflashadd[cnti] += Bank1ADDR;
                                    if (flashdataflag[cnti] == TRUE)
                                    {
                                        dadd = (U16 *)(tmpflashadd[cnti]);

                                        l = (*(padd + 1));
                                        tmpdatasum = (*(padd + 2));
                                        datasum = 0;
                                        for (cntj = 0; cntj < l; cntj++)
                                        {
                                            datasum += *dadd;
                                            dadd++;
                                        }
                                        if (datasum != tmpdatasum)
                                        {
                                            flashdataflag[cnti] = FALSE;
                                            errFlag = 1;
                                            //#if Debug
                                            //printf("flashdataflag[%d] = %d  datasum = %x  tmpdatasum = %x\r\n",cnti,flashdataflag[cnti],datasum,tmpdatasum);
                                            //#endif
                                        }
                                    }
                                    padd += 3;
                                    checkadd++;
                                }
                                if ((errFlag == 0) && ((checksum != tmpchecksum)))
                                {
                                    errFlag = 1;
                                }
                                if (errFlag == 0)
                                {
                                    //UpdataF = 0x55;
                                    W25QXX_Write(W25Q1_DAT, (U8 *)(&UpdataF), W25QBACIFOADDR + 16, 2); //设置FLASH跟新成功标志
                                    f |= 0x02;
                                    break;
                                }
                                else
                                {
                                    DelayMs(200);
                                    SPI1_Init(SP_12MHZ);
                                    W25QXX_Init();
                                    TF_Init();
                                    errCnt++;
                                }
#endif
                            }
                            else
                            {
                                //printf("update_flash err \r\n");
                                break;
                            }
                        }
                        g_logorun = 100;
                    }

                    if (ii & 0x00000008)
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 116), linemaxdisplay, Err_MSG1_Sheet1, 116, 2, vTestColor); //正在升级打板器语音文件...

                        if (update_mp3(j, 4, bgColor, 0) == TRUE)
                        { // 升级语音数据
                            f |= 0x04;
                        }
                    }

                    //升级备份打扮器M-操作系统程序
                    if ((ii & 0x00000001) && (g_upDISP))
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 92), linemaxdisplay, Err_MSG1_Sheet1, 92, 2, vTestColor); // 正在升级打版器...
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6);                                                                //写升级文件信息
                        if (update_dispprogram(j, 4, bgColor, 0, 0) == TRUE)
                        {
                            f |= 0x08;
                        }
                    }
                    else if (upflag)
                    {
                        upflag = f;
                        temp_para = 164;
                        dis_updata = 1;
                        break;
                    }

                    if (f == 0)
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // 没有升级文件
                        btn_en = 2;
                    }
                    else
                    {
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6); //写升级文件信息
                        DelayMs(800);
                        if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                        {                                          // 选择主板U盘
                            Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // 主板重启动
                        }
                        EXTI->IMR = 0x00000000;
                        EXTI->EMR = 0x00000000;

                        RCC->AHB1RSTR = 0xffffffff;
                        RCC->AHB2RSTR = 0xffffffff;
                        RCC->AHB3RSTR = 0xffffffff;
                        RCC->APB1RSTR = 0xffffffff;
                        RCC->APB2RSTR = 0xffffffff;
                        theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                        MSR_MSP(*(U32 *)(UPGRADEADDR));
                        theUboot();
                    }
                }
#else
                if (f)
                {                                                                                                                                        // 未连接
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //无存储设备
                    btn_en = 2;
                }
                else
                {
                    DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                    DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                    // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                    // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                    ii = AnalyseUpdateList(j, 0);

                    if ((ii & 0x00000002) && (g_upDISP)) //H系统打扮器程序,备份到flash
                    {
                        DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                     rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // 正在更新数据...
                        if (copy_dispprogram(j, 4, bgColor, 0, 1) == TRUE)
                        {
                            W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //写升级文件信息
                            upflag = 1;
                        }
                    }

                    if ((ii & 0x80000) && (g_upWIFI))//wifi
                    { 
                        errCnt = 0;
                        // DrawFillRect(140, 190, 444, 96, bgColor);
                        DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                     rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 455), linemaxdisplay, Err_MSG1_Sheet1, 455, l1, vTestColor); //正在升级WIFI模块...
                        while (errCnt < 3)
                        {
                            if (update_mcpuprogram(j, 15, 4, bgColor) == TRUE)
                            {
                                f |= 0x10;
                                if (g_MboardUSB == 1)
                                {
                                    DelayMs(2000);
                                }
                                errCnt = 3;
                                break;
                            }
                            else
                            {
                                errCnt++;
                            }
                        }
                    }

                    if (g_MachineTy < 16)
                    {
                        i = 0x00000010;
                        i <<= g_MachineTy;
                        if ((ii & i) && (g_upMBOARD == 1))
                        {
                            errCnt = 0;
                            // DrawFillRect(140, 190, 444, 96, bgColor);
                            DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                         rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 93), linemaxdisplay, Err_MSG1_Sheet1, 93, 2, vTestColor); //正在升级主板...
                            while (errCnt < 3)
                            {
                                if (update_mcpuprogram(j, g_MachineTy, 4, bgColor) == TRUE)
                                {
                                    f |= 0x01;
                                    if (g_MboardUSB == 1)
                                    {
                                        DelayMs(2000);
                                    }
                                    errCnt = 3;
                                    break;
                                }
                                else
                                {
                                    errCnt++;
                                }
                            }
                        }
                    }

                    if ((ii & 0x00000004) && (g_upFLASH))//升级FLASH
                    { 

                        errFlag = 0;
                        errCnt = 0;
                        g_logorun = 0;
                        while (errCnt < 3)
                        {
                            // DrawFillRect(140, 190, 444, 96, bgColor);
                            DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                         rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 95), linemaxdisplay, Err_MSG1_Sheet1, 95, 2, vTestColor); //正在升级打板器FLASH
                            SETWCS1();                                                                                                                          //取消片选
                            SETWCS2();                                                                                                                          //取消片选
                            SETTFCS();
                            errFlag = 0;

                            if (update_flash(j, 4, bgColor, 0) == TRUE)
                            { // 升级FLASH
#ifndef USESST39VFXX
                                //写入数据后重读校验，不成功继续3次
                                W25QXX_Read(W25Q1_DAT, (U8 *)checkbuff, FLASHINDEXADDR + 24, 12);
                                maxadd = (((U32)checkbuff[1]) << 16) + checkbuff[0];
                                //									#if Debug
                                //									printf("maxadd = %08x: SDRAMFLASHADD = %8x errCnt = %d\r\n",maxadd,SDRAMFLASHADD,errCnt);
                                //									#endif
                                if ((maxadd > FLASHINDEXADDR) && (maxadd < 0x800000))
                                {
                                    W25QXX_Read(W25Q1_DAT, (U8 *)Bank1ADDR, 0, maxadd);

                                    checkadd = (U16 *)(Bank1ADDR + FLASHINDEXADDR + 28);
                                    listlen = ((*(checkadd + 1)) << 16) + (*checkadd);
                                    tmpchecksum = (*(checkadd + 2));

                                    checksum = 0;
                                    checkadd = (U16 *)(Bank1ADDR + FLASHINDEXADDR + 36);
                                    padd = (U32 *)(Bank1ADDR + FLASHINDEXADDR + 36);

                                    listlen -= FLASHINDEXADDR;
                                    listlen /= 12;
                                    listlen -= 3;
                                }


                                for (cnti = 1; cnti <= listlen; cnti++)
                                {
                                    for (cntj = 0; cntj < 5; cntj++)
                                    {
                                        checksum += (*checkadd);
                                        checkadd++;
                                    }
                                    tmpflashadd[cnti] = (*padd);
                                    if (tmpflashadd[cnti] >= maxadd)
                                    {
                                        tmpflashadd[cnti] = 0;
                                        flashdataflag[cnti] = FALSE;
                                    }
                                    else
                                    {
                                        flashdataflag[cnti] = TRUE;
                                    }
                                    tmpflashadd[cnti] += Bank1ADDR;
                                    if (flashdataflag[cnti] == TRUE)
                                    {
                                        dadd = (U16 *)(tmpflashadd[cnti]);

                                        l = (*(padd + 1));
                                        tmpdatasum = (*(padd + 2));
                                        datasum = 0;
                                        for (cntj = 0; cntj < l; cntj++)
                                        {
                                            datasum += *dadd;
                                            dadd++;
                                        }
                                        if (datasum != tmpdatasum)
                                        {
                                            flashdataflag[cnti] = FALSE;
                                            errFlag = 1;
                                            //#if Debug
                                            //printf("flashdataflag[%d] = %d  datasum = %x  tmpdatasum = %x\r\n",cnti,flashdataflag[cnti],datasum,tmpdatasum);
                                            //#endif
                                        }
                                    }
                                    padd += 3;
                                    checkadd++;
                                }
                                if ((errFlag == 0) && ((checksum != tmpchecksum)))
                                {
                                    errFlag = 1;
                                }
                                if (errFlag == 0)
                                {
                                    g_upinfo[FL_ADDR] = '0'; //标记不需要升级flash
                                    creatUpInfoData(Info);
                                    W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
                                    f |= 0x02;
                                    break;
                                }
                                else
                                {
                                    DelayMs(200);
                                    SPI1_Init(SP_12MHZ);
                                    W25QXX_Init();
                                    TF_Init();
                                    errCnt++;
                                }
#endif
                            }
                            else
                            {
                                //printf("update_flash err \r\n");
                                break;
                            }
                        }
                        g_logorun = 100;
                    }

                    if (ii & 0x00000008)
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 116), linemaxdisplay, Err_MSG1_Sheet1, 116, 2, vTestColor); //正在升级打板器语音文件...

                        if (update_mp3(j, 4, bgColor, 0) == TRUE)
                        { // 升级语音数据
                            f |= 0x04;
                        }
                    }

                    //升级备份打扮器M-操作系统程序
                    if ((ii & 0x00000001) && (g_upDISP))
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 92), linemaxdisplay, Err_MSG1_Sheet1, 92, 2, vTestColor); // 正在升级打版器...
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QMIFOADDR + 18, 6);                                                                  //写升级文件信息
                        if (update_dispprogram(j, 4, bgColor, 0, 0) == TRUE)
                        {
                            f |= 0x08;
                        }
                    }
                    else if (upflag)
                    {
                        upflag = f;
                        temp_para = 164;
                        dis_updata = 1;
                        break;
                    }

                    if (f == 0)
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // 没有升级文件
                        btn_en = 2;
                    }
                    else
                    {
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QMIFOADDR + 18, 6); //写升级文件信息
                        creatUpInfoData(Info);
                        W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
                        DelayMs(800);
                        if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                        {                                          // 选择主板U盘
                            Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // 主板重启动
                        }
                        EXTI->IMR = 0x00000000;
                        EXTI->EMR = 0x00000000;

                        RCC->AHB1RSTR = 0xffffffff;
                        RCC->AHB2RSTR = 0xffffffff;
                        RCC->AHB3RSTR = 0xffffffff;
                        RCC->APB1RSTR = 0xffffffff;
                        RCC->APB2RSTR = 0xffffffff;
                        theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                        MSR_MSP(*(U32 *)(UPGRADEADDR));
                        theUboot();
                    }
                }
#endif
                break;

            case 72:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 96), linemaxdisplay, Err_MSG1_Sheet1, 96, l1, vTestColor); //数据校验错误,请重新升级!
                btn_en = 2;
                break;

            case 73:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 97), linemaxdisplay, Err_MSG1_Sheet1, 97, l1, vTestColor); //花样图形已损坏,不能正常操作!
                btn_en = 2;

                Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息

                m = F_IND_NO;
                if ((m >= MINFILENO) && (m <= MAXFILENO))
                {
                    n = (m - MINFILENO) * PTINDEXREDLEN + PTINDEXLENADD;
                    len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                    n = 0; // 如果读花样失败，初始化SREG寄存器
                    if ((len >= 0x40) && (len <= MAXSREG))
                    {
                        n = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样数据
                        if (n)
                        {
                            coldstart = 0;
                            g_init_flag = 1;
                            g_selectdisno = SREG[0];
                            g_data_flash = 1;
                        }
                    }
                }
                break;

            case 74:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 98), linemaxdisplay, Err_MSG1_Sheet1, 98, l1, vTestColor); //没有找到结束指令,不能正常操作!
                btn_en = 2;
                break;

            case 75: //是否校准触摸屏
                ViewMsg(99);
                btn_en = 3;
                break;

            case 76:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, l1, vTestColor); //无升级文件
                btn_en = 2;
                break;

            case 77:                                                                                                                                 //无文件
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 36), linemaxdisplay, Err_MSG1_Sheet1, 36, l1, vTestColor); //内部记忆体无文件
                btn_en = 2;
                break;

            case 84: //选择相应操作的磁盘介质
            case 85:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 30), linemaxdisplay, Err_MSG1_Sheet1, 30, l1, vTestColor); //请选择要操作的存储器.
                btn_en = 0;
                break;

            case 86:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 108), linemaxdisplay, Err_MSG1_Sheet1, 108, l1, vTestColor); //是否自动写入条形码?
                btn_en = 3;                                                                                                                            //是否自动写入条形码
                break;

            case 87:
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 108 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 108, l2, vTestColor); //正在写入条形码,请稍候...
                btn_en = 0;                                                                                                                                        //写入条形码
                break;

            case 88:
                ViewMsg(52);
                btn_en = 1; //设置值太大
                break;

            case 89:
                ViewMsg(53);
                btn_en = 1; //设置值太小
                break;

            case 90: //读远程花样
                ViewMsg(113);
                Read_D(D_Address[SYS] + SYS_LISTTYPE, 1, (U16 *)(&YREG[SYS_LISTTYPE])); // 读主板内存索引类型
                btn_en = 0;

                while (errorf < 3)
                {
                    DREG[D_COMMAND] = C_RWDWORKSHOPFILE; // 读花样文件
                    DREG[D_CMDSTATUS] = 0;
                    DREG[D_CMDPARA1] = g_array6[0]; //文件编号
                    DREG[D_CMDPARA2] = 1;
                    DREG[D_CMDPARA3] = g_netSelectMac;                                      //ip
                    f = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // 发送指令
                    if (f == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    f = Read1_Status(8);
                    //for(i=0;i<(RDTIMES*8);i++){
                    //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                    //	if((DREG[D_CMDSTATUS]))break;
                    //	DelayMs(WAITTIME);
                    //}
                    if ((f == 0) || (DREG[D_CMDSTATUS] == 0))
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    else if (DREG[D_CMDSTATUS] == 98)
                    {
                    }
                    else if (DREG[D_CMDSTATUS] == 99)
                    {
                        temp_para = 3;
                        dis_updata = 1;
                        break;
                    }
                    else if (DREG[D_CMDSTATUS] < 98)
                    {
                        temp_para = 16;
                        dis_updata = 1;
                        break;
                    }

                    f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                    if (f == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    if (g_array6[0] != F_IND_NO)
                    {
                        temp_para = 16;
                        dis_updata = 1;
                        break;
                    }
                    len = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
                    f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样文件
                    if (f == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    else
                    {
                        if ((DataCheckSum((U16 *)(&SREG[0]), len, (F_FILETYPE == PAT_MIT ? 0 : 1)) == 0))
                        { //数据校验失败
                            errorf++;
                            i = 0;
                        }
                        else
                        {
                            errorf = 0;
                            E_RUNDIS();
                            E_WORKSHOPDIS();
                            f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
                            if (f == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                            }
                            break;
                        }
                    }
                }
                if (errorf == 3)
                { //通讯出错超过3次，报警
                    errorf = 0;
                    temp_para = 64;
                    dis_updata = 1;
                }
                else
                {
                    pno = MENU_WRITE1;
                    g_NoUpdataoldno = 1;
                }
                break;

            case 91: //写远程文件
                ViewMsg(114);
                btn_en = 0;

                m = F_IND_NO;

                DREG[D_COMMAND] = C_RWDWORKSHOPFILE; // 写花样文件
                DREG[D_CMDSTATUS] = 0;
                DREG[D_CMDPARA1] = g_array6[0]; //文件编号
                DREG[D_CMDPARA2] = 2;
                DREG[D_CMDPARA3] = g_netSelectMac;                                      //ip
                f = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // 发送指令
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                f = Read1_Status(8);
                //for(i=0;i<(RDTIMES*8);i++){
                //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                //	if((DREG[D_CMDSTATUS]))break;
                //	DelayMs(WAITTIME);
                //}
                if ((f == 0) || (DREG[D_CMDSTATUS] == 0))
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                else if (DREG[D_CMDSTATUS] == 98)
                {
                }
                else if (DREG[D_CMDSTATUS] == 99)
                {
                    temp_para = 3;
                    dis_updata = 1;
                    break;
                }
                else if (DREG[D_CMDSTATUS] < 98)
                {
                    temp_para = 16;
                    dis_updata = 1;
                    break;
                }

                E_RUNDIS();
                E_WORKSHOPDIS();
                f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }

                g_array6[0] = m;
                pnoindex--;
                pno = poldno[pnoindex];
                break;

            case 92:
                ViewMsg(51);
                btn_en = 1;
                break; //设置值错误

            case 94:
                ViewMsg(412);
                ComErrMessage = 1;
                btn_en = 1;
                //S_ERRORSET();Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS); //发送错误码给主板
                break; //针杆不在最高位

            case 95:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //无存储设备!
                btn_en = 2;
                break;

            case 96:
                DrawFBMP2(100, 168, 500, 128, (U16 *)msgerr, vTestColor); //系统升级失败，请重新升级
                btn_en = 3;
                break;

            case 97:
                Draw24x24Str(disPosStr_X, vaddr1, RwHelp_TITLE_ADD + 18 * RwHelpTitleLengthMax, RwHelp_Sheet, 18, vTestColor); // ID 错误
                btn_en = 0;
                break;

            case 98:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_PROG_MSG7 + ERROR_LEN * 83), linemaxdisplay, Err_MSG2_Sheet1, 83, l1, vTestColor); //  转换为三菱格式部件数量超过8000,无法完成转换
                btn_en = 2;
                break;

            case 99: //检出器异常
                ViewMsg(8);
                btn_en = 2;
                break;

            case 100: //请出入保存名字
                Draw24x24Str(disPosStr_X, vaddr1, RwHelp_TITLE_ADD + 19 * RwHelpTitleLengthMax, RwHelp_Sheet, 19, vTestColor);
                btn_en = 1;
                break;

            case 101://请选择合并花样资料
                Draw24x24Str(disPosStr_X, vaddr1, RwHelp_TITLE_ADD + 20 * RwHelpTitleLengthMax, RwHelp_Sheet, 20, vTestColor); 
                btn_en = 1;
                break;

            case 102: //不能产生合并图形
                ViewMsg(413);
                btn_en = 1;
                break;

            case 103://可以清除当前混合图形吗?
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 415), linemaxdisplay, Err_MSG1_Sheet1, 415, l1, vTestColor); 
                btn_en = 3;
                break;

            case 104: //无法读取混合图形
                ViewMsg(414);
                btn_en = 1;
                break;

            case 105://可以删除内部记忆体里的合并图形吗?
                //GroupType = GREG[dataadd[selectno1]+GroupPTypeAddr];
                DrawRect(400, vaddr1, 174, 40, vTestColor); //待修改
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 416), linemaxdisplay, Err_MSG1_Sheet1, 416, l2, vTestColor); 
                btn_en = 3;
                break;

            case 106://正在删除内部记忆体里的合并图形
                DrawRect(400, vaddr1, 174, 40, vTestColor);
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 417), linemaxdisplay, Err_MSG1_Sheet1, 417, l2, vTestColor); 

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;
                if (Write_Cmd(C_DELGROUP, g_GroupType, g_GroupDoingNO) == 0)
                { //发送删除组合文件指令
                    temp_para = 16;
                    dis_updata = 1;
                    break;
                }
                if (Read_Status() == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                else if (DREG[D_CMDSTATUS] != 100)
                {
                    //#if Debug
                    //	printf("DREG[D_CMDSTATUS] = %d \r\n",DREG[D_CMDSTATUS] );
                    //#endif
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }

                if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                { //发送读取组合图形指令
                    temp_para = 16;
                    dis_updata = 1;
                    break;
                }
                if (Read_Status() == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                else if (DREG[D_CMDSTATUS] != 100)
                {

                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

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

                //显示回当前组合文件
                for (i = 0; i < GREG[GroupPNumAddr]; i++)
                {
                    g_Combinepat[i][0] = GREG[GroupPNoAddr + i]; //图形花样编号
                    for (j = 0; j < 4; j++)
                    { //图形花样名称,只显示前面8个字符
                        g_Combinepat[i][j + 1] = IREG2[(GREG[GroupPNoAddr + i] - MINFILENO) * FileIndexSize + FileIndexNamepox + j];
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

            case 107: //可以删除磁碟片上的合并图形吗?          
                                        
                DrawRect(400, vaddr1, 174, 40, vTestColor); //待修改
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 420), linemaxdisplay, Err_MSG1_Sheet1, 420, l2, vTestColor); //可以删除磁碟片上的合并图形吗?
                btn_en = 3;
                break;

            case 108://正在删除磁碟片上的合并图形
                DrawRect(400, vaddr1, 174, 40, vTestColor); //待修改
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 421), linemaxdisplay, Err_MSG1_Sheet1, 26, l2, vTestColor); 
                //Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*421+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,36,2,Color_White);//请稍候

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                if (g_disk_memory == 1)
                    Select_ExtMemory(USBTF);
                else
                    Select_ExtMemory(SD);
                if (Delete_GroupFile((U8 *)g_tmpGroName, g_GroupType))
                {
                    temp_para = 16;
                    dis_updata = 1;
                    break;
                }
                else
                {
                    if (g_disk_memory == 1)
                    {
                        Explore_Disk_Group("0:/", 1);
                    }
                    else
                    {
                        Explore_Disk_Group("1:/", 1);
                    }
                }
                pnoindex--;
                pno = poldno[pnoindex];

                break;

            case 109://复制合并图形到磁碟片上?
                ViewMsg(422);
                btn_en = 3;
                break;

            case 110:
                ViewMsg(207);
                //将主板花样写到磁碟片上
                for (i = 0; i < GREG[GroupPNumAddr]; i++)
                {
                    if ((GREG[i] >= MINFILENO) && (GREG[i] <= MAXFILENO))
                    {
                        U32 cnt;
                        n = (GREG[i] - MINFILENO) * FileIndexSize; //2013.3.5
                        if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                        {
                            for (j = 0; j < FileIndexSize; j++)
                                file_meg[j] = IREG2[n + j];
                            result = Write_Cmd(C_READMEMORYPAT, n / FileIndexSize + MINFILENO, 0);
                            if (result)
                            {
                                for (cnt = 0; cnt < (RDTIMES + RDTIMES); cnt++)
                                {
                                    result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                                    if ((DREG[D_CMDSTATUS]))
                                        break;
                                    DelayMs(WAITTIME);
                                }
                            }

                            Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                            if (F_IND_NO == (n / FileIndexSize + MINFILENO))
                            {
                                ReSREGcount = 0;
                                while (ReSREGcount < 3)
                                {
                                    result = Read_D(D_Address[PTDATA], (((U32)(file_meg[1]) << 16) + (U32)(file_meg[0])) / 2, (U16 *)(&SREG[0]));
                                    if (CheckSREG(2, ((((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1), ((file_meg[1] << 16) | file_meg[0])) == TRUE)
                                        break;
                                    ReSREGcount += 1;
                                    wdt();
                                }
                                if (ReSREGcount >= 3)
                                {
                                    if (result)
                                        continue;
                                }

                                if (result)
                                {
                                    if (((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0)
                                    { //三菱款格式
                                        len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                        checksum = 0;

                                        for (cnt = 0; cnt < len; cnt++)
                                        { //计算校验和
                                            if (cnt != 0x20)
                                                checksum += SREG[cnt];
                                        }
                                        SREG[0x20] = checksum;
                                    }
                                    else
                                    { //兄弟格式
                                        len = ((file_meg[1] << 16) | file_meg[0]) / 2;
                                        checksum = 0;
                                        for (cnt = 0; cnt < len; cnt++)
                                        { //计算校验和
                                            if (cnt != 0x05)
                                                checksum += SREG[cnt];
                                        }
                                        SREG[0x05] = checksum;
                                    }

                                    flag = 0;
                                    if (g_GroupSave == 2)
                                    {
                                        if (ExtMemory_Status(USBTF) == 0)
                                        {
                                            flag = 1;
                                            Select_ExtMemory(USBTF);
                                        }
                                    }
                                    else
                                    {
                                        if (ExtMemory_Status(SD) == 0)
                                        {
                                            flag = 1;
                                            Select_ExtMemory(SD);
                                        }
                                    }
                                    if (flag)
                                    {
                                        Delete_PatFile(F_IND_NO); //先删除原文件 有没有都删除一下
                                        Write_PatFile(F_IND_NO, 0);
                                    }
                                }
                            }
                        }
                    }
                }

                //写组合文件
                Delete_GroupFile((U8 *)g_GroupName, g_GroupType); //先删除源文件
                Write_GroupFile((U8 *)g_GroupName, g_GroupType);

                //保存文件到默认位置
                if (Write_D(D_Address[GROUP], GroupPatMax, (U16 *)(&GREG[0])) == 0)
                { //发送参数
                    break;
                }
                if (Write_Cmd(C_SAVEGROUP, 3, g_GroupType) == 0)
                { //发送保存组合文件指令
                    break;
                }
                if (Read_Status() == 0)
                {
                    break;
                }
                else if (DREG[D_CMDSTATUS] != 100)
                {
                    break;
                }

                //重新读取组合数据
                if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                { //发送读取组合图形指令
                    break;
                }
                if (Read_Status() == 0)
                {
                    break;
                }
                else if (DREG[D_CMDSTATUS] != 100)
                {
                    break;
                }
                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE])); // 读主板内存索引类型
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
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }

                Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // 读当前花样文件
                g_NoReadPatf = 1;                                                        //标记 进入P1界面不需要重新读取花样数据
                pnoindex = 0;
                pno = MENU_P1;
                break;

            case 111:
                ViewMsg(423);
                btn_en = 3;
                break;

            case 112:
                ViewMsg(209);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                Select_ExtMemory(USBTF);

                //写磁盘文件到内存
                for (i = 0; i < g_GroupPatNums; i++)
                {
                    if ((GREG[i + (g_GroupDoingNO - 1) * GroupPatMax + ((g_GroupType == 1) ? GroupComAddr : GroupConAddr)] >= MINFILENO) && (GREG[i + (g_GroupDoingNO - 1) * GroupPatMax + ((g_GroupType == 1) ? GroupComAddr : GroupConAddr)] <= MAXFILENO))
                    {
                        U16 FileNo = GREG[i + (g_GroupDoingNO - 1) * GroupPatMax + ((g_GroupType == 1) ? GroupComAddr : GroupConAddr)];
                        n = (FileNo - MINFILENO) * FileIndexSize;
                        if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                        {
                            for (j = 0; j < FileIndexSize; j++)
                                file_meg[j] = IREG2[n + j];

                            ReSREGcount = 0;
                            while (ReSREGcount < 3)
                            {
                                Read_PatFile(FileNo);
                                len = (F_FILELENH << 16) + F_FILELENL;
                                if (CheckSREG(1, ((((file_meg[FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1), ((file_meg[1] << 16) | file_meg[0])) == TRUE)
                                    break;
                                ReSREGcount += 1;
                                wdt();
                            }
                            if (ReSREGcount >= 3)
                            {
                                continue;
                            }
                            ReSREGcount = 0;
                            while (ReSREGcount < 5)
                            {
                                result = Write_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 发送当前花样文件
                                if (result == 0)
                                {                //通讯错误
                                    f = C_ERROR; // 操作失败	2
                                    break;
                                }

                                result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //发送当前文件列表
                                if (result == 0)
                                { //通讯错误
                                    break;
                                }
                                result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //发送保存花样命令
                                if (result == 0)
                                {
                                    ReSREGcount = 5;
                                    break;
                                }
                                if (Read_Status() == 0)
                                {
                                    ReSREGcount = 5;
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] == 98)
                                {
                                    ReSREGcount++;
                                }
                                else if (DREG[D_CMDSTATUS] == 100)
                                {
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] == 12)
                                { //通讯出错，重发
                                    ReSREGcount++;
                                }
                                else
                                {
                                    ReSREGcount++;
                                }
                            }
                        }
                    }
                }

                //读回内存文件
                if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                { //发送读取组合图形指令
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                if (Read_Status() == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                else if (DREG[D_CMDSTATUS] != 100)
                {

                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

                ReadMemorryList(MENU_FTGROUPREAD2); //读回内存文件索引

                btn_en = 0;
                g_GroupMenth = 4; //将文件保存到默认位置
                g_checkGroup = 0;
                pnoindex--;
                pno = MENU_FTGROUP;

                break;

            case 113: //找不到图形资料
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 425), linemaxdisplay, Err_MSG1_Sheet1, 425, l1, vTestColor);
                btn_en = 1;
                break;

            case 114: //通信故障，请检查伺服连接适合正常
                ViewMsg(431);
                btn_en = 1;
                break;

            case 117:
                ViewMsg(418);
                btn_en = 1;
                break;

            case 118: //使用组合图形,不容许读取操作
                ViewMsg(419);
                btn_en = 1;
                break;

            case 119: //图形超范围
                ViewMsg(27);
                btn_en = 1;
                break;

            case 120:
                ViewMsg(424);
                btn_en = 1;
                break;

            case 121:
                ViewMsg(426);
                btn_en = 1;
                break; //找不到TF卡

            case 122:
                ViewMsg(427);
                btn_en = 1;
                break;

            case 123:
                ViewMsg(428);
                btn_en = 1;
                break;

            case 124:
                ViewMsg(429);
                btn_en = 1;
                break;

            case 125:
                ViewMsg(430);
                btn_en = 1;
                break;

            case 126:
                ViewMsg(432);
                btn_en = 3;
                break;

            case 127:
                ViewMsg(433);
                btn_en = 3;
                break;

            case 128:
                ViewMsg(434);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                Select_ExtMemory(USBTF);

                Write_SysBackupkFile((U16 *)(&PREG[0]), MAXPREG, 1);
                pnoindex = 1;
                pno = MENU_FUNCTION;
                ;
                break;

            case 129:
                ViewMsg(435);
                btn_en = 3;
                break;

            case 130:
                ViewMsg(436);
                btn_en = 3;
                break;

            case 131:
                ViewMsg(437);
                BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;
                Select_ExtMemory(USBTF);

                Write_SysBackupkFile((U16 *)(&IOREG[0]), 338, 2);
                pnoindex = 1;
                pno = MENU_FUNCTION;
                ;
                break;

            case 132:
                ViewMsg(438);
                btn_en = 3;
                break;

            case 133: //还原系统参数
                ViewMsg(440);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;
                /*if(g_disk_memory==1)*/ Select_ExtMemory(USBTF);
                //else  Select_ExtMemory(SD);

                f = Read_SysBackupkFile((U16 *)&PREG[0], &backuplen, 1);
                if (f)
                {                                                               //出错
                    Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); //待修改
                    DrawFillRect(140, 190, 444, 96, bgColor);
                    DrawFillRect(140, 304, 444, 160, bgColor);
                    if (f == 0xff)
                    {                                                                                                                                          //校验不成功
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 429), linemaxdisplay, Err_MSG1_Sheet1, 429, l1, vTestColor); //文件格式错误
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //磁盘无文件
                    }
                    btn_en = 1;
                }
                else
                { //成功
                    Write_D(D_Address[PARA], backuplen, (U16 *)&PREG[0]);
                    f = Write_Cmd(C_UPDATEPARA, 0, 0);

                    if (f)
                    { //成功
                        if (Read_Status() == 0)
                        {
                            f = 0; //失败
                        }
                        else if (DREG[D_CMDSTATUS] == 100)
                        {
                            f = 1; //成功
                        }
                        else
                        {
                            f = 0; //失败
                        }
                    }
                    if (f == 0)
                    {                                                               //通信失败
                        Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); //待修改
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        DrawFillRect(140, 304, 444, 160, bgColor);

                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23), linemaxdisplay, Err_MSG1_Sheet1, 23, l1, vTestColor);             //通信失败
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 23, l2, vTestColor); //请检查通讯线是否与主机连接?

                        btn_en = 1;
                    }
                    else
                    {
                        gPeessVal = P_CTR;
                        if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL)
                            gPeessVal = PEESS_DEfVAL;
                        WritePessVal(gPeessVal);
                        SetLcdPwm(1, P_CNR); //调节液晶屏对比度
                        SaveBackLight(P_CNR);
                        pnoindex = 1;
                        pno = MENU_FUNCTION;
                        ;
                    }
                }

                break;

            case 134:
                ViewMsg(439);
                btn_en = 3;
                break;

            case 135:
                ViewMsg(441);
                /*if(g_disk_memory==1)*/ Select_ExtMemory(USBTF);
                //else  Select_ExtMemory(SD);

                f = Read_SysBackupkFile((U16 *)&IOREG[0], &backuplen, 2);
                if (f)
                {                                                    //出错
                    Read_D(D_Address[IOPARA], 338, (U16 *)(&IOREG)); //读取IO参数
                    DrawFillRect(140, 190, 444, 96, bgColor);        //待修改
                    DrawFillRect(140, 304, 444, 160, bgColor);
                    ;
                    if (f == 0xff)
                    {                                                                                                                                          //校验不成功
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 429), linemaxdisplay, Err_MSG1_Sheet1, 429, l1, vTestColor); //文件格式错误
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //磁盘无文件
                    }
                    btn_en = 1;
                }
                else
                { //成功
                    Write_D(D_Address[IOPARA], backuplen, (U16 *)(&IOREG[0]));
                    f = Write_Cmd(C_SAVEIO, 0, 0);

                    if (f)
                    { //成功
                        if (Read_Status() == 0)
                        {
                            f = 0; //失败
                        }
                        else if (DREG[D_CMDSTATUS] == 100)
                        {
                            f = 1; //成功
                        }
                        else
                        {
                            f = 0; //失败
                        }
                    }
                    if (f == 0)
                    {                                                    //通信失败
                        Read_D(D_Address[IOPARA], 338, (U16 *)(&IOREG)); //读取IO参数
                        DrawFillRect(140, 190, 444, 96, bgColor);        //待修改
                        DrawFillRect(140, 304, 444, 160, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23), linemaxdisplay, Err_MSG1_Sheet1, 23, l1, vTestColor);
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 23, l2, vTestColor); //通信失败

                        btn_en = 1;
                    }
                    else
                    {
                        pnoindex = 1;
                        pno = MENU_FUNCTION;
                        ;
                    }
                }
                break;

            case 136:                                                                                                                               //IO 输入设定 恢复出厂
            case 137:                                                                                                                               //IO 输出设定 恢复出厂
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 87), linemaxdisplay, 82, 2, Err_MSG1_Sheet1, vTestColor); //是否还原系统出厂设置?
                g_NO_ReagIOf = 1;
                btn_en = 3;
                break;

            case 139: //打板进入图形超范围
                ViewMsg(443);
                btn_en = 2;
                break;

            case 140:
                ViewMsg(444);
                btn_en = 2;
                break;

            case 141:
            case 142:
                ViewMsg(445);
                btn_en = 3;
                break;

            case 143:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 27), linemaxdisplay, Err_MSG1_Sheet1, 27, l1, vTestColor);   //输入坐标错误!
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 444), linemaxdisplay, Err_MSG1_Sheet1, 444, l2, vTestColor); //中压脚行程超出超出可调范围 0-8mm!
                btn_en = 1;
                break;

            case 144:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 446), linemaxdisplay, Err_MSG1_Sheet1, 446, l1, vTestColor); //选择停针点为空送/点数据,不支持倒缝变换
                btn_en = 1;
                break;

            case 145:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 447), linemaxdisplay, Err_MSG1_Sheet1, 447, l1, vTestColor); //选择停针点为空送/点数据,不支持Z字缝变换
                btn_en = 1;
                break;

            case 146:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 110), linemaxdisplay, Err_MSG1_Sheet1, 110, l1, vTestColor); //坐标超过三百个!
                btn_en = 1;
                break;

            case 147:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 448), linemaxdisplay, Err_MSG1_Sheet1, 448, l1, vTestColor); //语音固件异常
                btn_en = 1;
                break;

            case 148:
                ViewMsg(25);
                btn_en = 6;
                break;

            case 149:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 449), linemaxdisplay, Err_MSG1_Sheet1, 449, l1, vTestColor); //报警/警告提示信息已屏蔽.
                btn_en = 1;
                break;

            case 150:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 450), linemaxdisplay, Err_MSG1_Sheet1, 450, l1, vTestColor); //请选择升级设备
                btn_en = 1;
                break;

            case 151:
                ViewMsg(451);
                btn_en = 3;
                break;
            case 152:
                ViewMsg(452);
                btn_en = 3;
                break;

            case 153:
                ViewMsg(453);

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;
                /*if(g_disk_memory==1)*/ Select_ExtMemory(USBTF);
                //else  Select_ExtMemory(SD);
                Write_RecordFile();
                pnoindex = 1;
                pno = MENU_FUNCTION;
                ;

                break;

            case 154:
                ViewMsg(454);
                btn_en = 2;
                break;

            case 155:

                break;

            case 156:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 456), linemaxdisplay, Err_MSG1_Sheet1, 456, l1, vTestColor); //已收到远程花样文件
                Draw24X24FStr(520, vaddr1, "NO.", Color_Red);                                                                                          //待修改
                DrawMFU32(559, 176, F_IND_NO, 3, Color_Red);
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 456 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 456, l2, vTestColor); //是否查看
                btn_en = 3;
                if ((DREG[D_ERRWAR]))
                {
                    S_ERRORCLR();
                    Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                }
                break;

            case 157: //不在原点
            case 158: //不在原点
            case 159: //不在原点
            case 160: //不在原点
                ViewMsg(7);
                ComErrMessage = 1;
                //S_ERRORSET();Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS); //发送错误码给主板
                btn_en = 6;
                break;

            case 161: //通信故障，请检查步进电机连接适合正常
                ViewMsg(462);
                btn_en = 1;
                break;

            case 162:                                                                                                                                  //通信故障，请检查步进电机连接适合正常
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 464), linemaxdisplay, Err_MSG1_Sheet1, 464, l1, vTestColor); //是否切换到H-操作系统
                btn_en = 3;
                break;

            case 163: //切换到H-操作系统
            case 166:
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                f = 0;
                DrawFillRect(140, 190, 444, 96, bgColor);                                                                                             //待修改
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // 正在更新数据...
#ifdef OLD_BOOT
                if (update1_dispprogram(4, bgColor, 1) == TRUE)
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6); //写升级文件信息
                    f |= 0x08;
                }

                if (f == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // 没有升级文件
                    btn_en = 2;
                }
                else
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6); //写升级文件信息
                    DelayMs(800);

                    EXTI->IMR = 0x00000000;
                    EXTI->EMR = 0x00000000;

                    RCC->AHB1RSTR = 0xffffffff;
                    RCC->AHB2RSTR = 0xffffffff;
                    RCC->AHB3RSTR = 0xffffffff;
                    RCC->APB1RSTR = 0xffffffff;
                    RCC->APB2RSTR = 0xffffffff;
                    theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                    MSR_MSP(*(U32 *)(UPGRADEADDR));
                    theUboot();
                }
#else
                if (update1_dispprogram(4, bgColor, 1) == TRUE)
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //写升级文件信息
                    f |= 0x08;
                }

                if (f == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);                                                                                             //待修改
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // 没有升级文件
                    btn_en = 2;
                }
                else
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //写升级文件信息
                    DelayMs(800);

                    EXTI->IMR = 0x00000000;
                    EXTI->EMR = 0x00000000;

                    RCC->AHB1RSTR = 0xffffffff;
                    RCC->AHB2RSTR = 0xffffffff;
                    RCC->AHB3RSTR = 0xffffffff;
                    RCC->APB1RSTR = 0xffffffff;
                    RCC->APB2RSTR = 0xffffffff;
                    theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                    MSR_MSP(*(U32 *)(UPGRADEADDR));
                    theUboot();
                }
#endif
                break;

            case 164:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 467), linemaxdisplay, Err_MSG1_Sheet1, 467, l1, vTestColor); //没有M-操作系统升级文件,是否升级为H-操作系统
                btn_en = 3;
                break;

            case 165:
#ifdef OLD_BOOT
                if (upflag == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // 没有升级文件
                    btn_en = 2;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // 正在更新数据...
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6);                                                                  //写升级文件信息
                    DelayMs(800);
                    if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                    {                                          // 选择主板U盘
                        Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // 主板重启动
                    }
                    EXTI->IMR = 0x00000000;
                    EXTI->EMR = 0x00000000;

                    RCC->AHB1RSTR = 0xffffffff;
                    RCC->AHB2RSTR = 0xffffffff;
                    RCC->AHB3RSTR = 0xffffffff;
                    RCC->APB1RSTR = 0xffffffff;
                    RCC->APB2RSTR = 0xffffffff;
                    theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                    MSR_MSP(*(U32 *)(UPGRADEADDR));
                    theUboot();
                }
#else
                if (upflag == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);                                                                                             //待修改
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // 没有升级文件
                    btn_en = 2;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // 正在更新数据...
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QMIFOADDR + 18, 6);                                                                    //写升级文件信息
                    creatUpInfoData(Info);
                    W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
                    DelayMs(800);
                    if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                    {                                          // 选择主板U盘
                        Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // 主板重启动
                    }
                    EXTI->IMR = 0x00000000;
                    EXTI->EMR = 0x00000000;

                    RCC->AHB1RSTR = 0xffffffff;
                    RCC->AHB2RSTR = 0xffffffff;
                    RCC->AHB3RSTR = 0xffffffff;
                    RCC->APB1RSTR = 0xffffffff;
                    RCC->APB2RSTR = 0xffffffff;
                    theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                    MSR_MSP(*(U32 *)(UPGRADEADDR));
                    theUboot();
                }
#endif
                break;

            case 167:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (18) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 18, l1, vTestColor);                    //覆盖
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (18) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 18, l2, vTestColor); //此程序号文件已存在,是否继续执行?
                btn_en = 3;
                break;

            case 168:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (15) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 15, l1, vTestColor);                    //错误
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (15) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 15, l2, vTestColor); //文件名不能为空
                btn_en = 3;
                break;

            case 169:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (19) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 19, l1, vTestColor);                    //重命名失败
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (19) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 19, l2, vTestColor); //当前目录已存在此名称文件!
                btn_en = 2;
                break;

            case 170:
                //提示删除记忆体文件
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (2) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 2, l1, vTestColor);                    //删除
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (2) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 2, l2, vTestColor); //确认要删除的文件/文件夹吗?
                btn_en = 3;
                break;

            case 171:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (8) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 8, l1, vTestColor);                    //删除
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (8) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 8, l2, vTestColor); //正在删除文件,请稍等
                btn_en = 0;
                files = getFiles((U16 *)g_editfiles, MAXFILENO);

                setBufDat16((U16 *)g_selfiles, MAXFILENO, 0xffff);

                if (g_disk_memory == MEM_IN)
                { //主板 内存储器和打板器一起删除
                    for (i = 0; i < files; i++)
                    {
                        f = Write_Cmd(C_DELMEMORYPAT, getFilesNo((U16 *)g_editfiles, MAXFILENO, i), 0);
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (Read_Status() == 0 || DREG[D_CMDSTATUS] != 100)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                    }
                    f = ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]); //读取文件索引
                    if (f == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                }
                else
                {
                    for (i = 0; i < files; i++)
                    {
                        fileNo = getFilesNo((U16 *)g_editfiles, MAXFILENO, i);
                        fileType = getFileTypeByIREG((U16 *)IREG2, fileNo);
                        setBufDat8(disName, sizeof(disName), 0);

                        p8 = getFileNameByIREG((U16 *)IREG2, fileNo);
                        for (namelen = 0; namelen < 64; namelen++)
                        {
                            if (*(p8 + namelen) == 0)
                                break;
                            disName[namelen] = *(p8 + namelen);
                        }
                        deleteDiskFile(g_path, (char *)disName);
                    }
                    getDiskFiles(g_path, (U16 *)IREG2, GET_ALLFILE);
                }

                initFunctionBtn(1);
                pnoindex--;
                pno = poldno[pnoindex];
                break;

            case 172:
                Draw24x24MulStr(disPosStr_X, vaddr1, Toast_TITLE_ADD + (3) * ToastTitleLengthMax, linemaxdisplay, Toast_Sheet, 3, l1, vTestColor); //剪切和粘贴的文件夹相同.
                btn_en = 2;
                break;

            case 173: //粘贴
            case 174: //粘贴
                files = getFiles((U16 *)g_editfiles, MAXFILENO);
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (10) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 10, l1, vTestColor);                    //粘贴
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (10) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 10, l2, vTestColor); //确定要覆盖已存在的文件/文件夹吗
                btn_en = 3;
                break;

            case 175: //复制
            case 176: //剪切
            case 177: //导出
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (11) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 11, l1, vTestColor); //复制
                files = getFiles((U16 *)g_editfiles, MAXFILENO);
                initFunctionBtn(temp_para == 176 ? 1 : 0);
                setBufDat16((U16 *)g_selfiles, MAXFILENO, 0xffff);
                initMyProgress1(files, bgColor);

                if (g_from_memory == MEM_IN)
                { //复制源是内存
                    if (g_to_memory != MEM_IN)
                    {
                        if (temp_para != 176)
                        {
                            f = ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]); //先读回内存文件索引
                            if (f == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                        }

                        for (i = 0; i < files; i++)
                        {
                            DrawFillRect(140, vaddr2, 444, 56, bgColor); //待修改
                            SetMyProgress1(i);
                            DrawMFU32Z(528 - 13 * (2 * getNumDigits(files) + 1), vaddr2, i + 1, getNumDigits(files), vTestColor);
                            DrawMFU32Z(528 - 13 * getNumDigits(files), vaddr2, files, getNumDigits(files), vTestColor);
                            Draw24X24FStr(528 - 13 * (getNumDigits(files) + 1), vaddr2, "/", vTestColor);
                            fileNo = getFilesNo((U16 *)g_editfiles, MAXFILENO, i);
                            fileType = getFileTypeByIREG((U16 *)IREG2, fileNo);
                            //#if Debug
                            //printf("i =  %d files = %d fileNo = %d  fileType = %d \r\n",i,files,fileNo,fileType);
                            //#endif
                            setBufDat8(disName, sizeof(disName), 0);
                            if (fileType == FILE_SEW)
                            {
                                for (namelen = 0; namelen < sizeof(BorName); namelen++)
                                {
                                    disName[namelen] = BorName[namelen];
                                }
                                disName[5] = fileNo / 100 + 0x30;
                                disName[6] = fileNo / 10 % 10 + 0x30;
                                disName[7] = fileNo % 10 + 0x30;
                            }
                            else if (fileType == FILE_MITE || fileType == FILE_MITG)
                            {
                                p8 = getFileNameByIREG((U16 *)IREG2, fileNo);
                                for (namelen = 0; namelen < 63; namelen++)
                                {
                                    if (*(p8 + namelen) == 0)
                                        break;
                                    disName[namelen] = *(p8 + namelen);
                                }
                                disName[namelen++] = '.';
                                disName[namelen++] = fileNo / 100 + 0x30;
                                disName[namelen++] = fileNo / 10 % 10 + 0x30;
                                disName[namelen++] = fileNo % 10 + 0x30;
                            }
                            for (; namelen < 68; namelen++)
                            {
                                disName[namelen] = 0;
                            }
                            Draw24X24FStrByLine(disPosStr_X, vaddr2, (char *)disName, linemaxdisplay, 0, vTestColor);

                            result = Write_Cmd(C_READMEMORYPAT, fileNo, 0);
                            if (result)
                            {
                                for (j = 0; j < (RDTIMES + RDTIMES); j++)
                                {
                                    result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
                                    if ((DREG[D_CMDSTATUS]))
                                        break;
                                    DelayMs(WAITTIME);
                                }
                            }

                            Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                            if (F_IND_NO == fileNo)
                            {
                                ReSREGcount = 0;
                                while (ReSREGcount < 3)
                                {
                                    result = Read_D(D_Address[PTDATA], (((U32)(HREG[5]) << 16) + (U32)(HREG[4])) / 2, (U16 *)(&SREG[0]));
                                    if (CheckSREG(2, ((fileType == FILE_MITE || fileType == FILE_MITG) ? 0 : 1), (F_FILELEN)) == TRUE)
                                        break;
                                    ReSREGcount += 1;
                                    wdt();
                                }
                                if (ReSREGcount >= 3)
                                {
                                    if (result)
                                        continue;
                                }
                                if (result)
                                {
                                    checksum = 0;
                                    if (fileType == FILE_MITE || fileType == FILE_MITG)
                                    { //三菱款格式
                                        len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                        for (j = 0; j < len; j++)
                                        { //计算校验和
                                            if (j != 0x20)
                                                checksum += SREG[j];
                                        }
                                        SREG[0x20] = checksum;
                                    }
                                    else
                                    { //兄弟格式
                                        len = (F_FILELEN) / 2;
                                        for (j = 0; j < len; j++)
                                        { //计算校验和
                                            if (j != 0x05)
                                                checksum += SREG[j];
                                        }
                                        SREG[0x05] = checksum;
                                    }

                                    flag = 0;

                                    if (ExtMemory_Status(USBTF) == 0)
                                    {
                                        flag = 1;
                                        Select_ExtMemory(USBTF);
                                    }
                                    if (flag)
                                    {
                                        if (temp_para == 177)
                                        { //导出,文件路径为默认
                                            writeDiskPatFile((char *)defFilePart, fileNo);
                                        }
                                        else
                                        {
                                            writeDiskPatFile(g_path, fileNo);
                                        }
                                        if (temp_para == 176)
                                        { //剪切//删除内存器原来文件
                                            f = Write_Cmd(C_DELMEMORYPAT, fileNo, 0);
                                            Read_Status();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    SetMyProgress1(files);
                }
                else
                { //复制源是外部存储器
                    flag = 0;
                    if (ExtMemory_Status(USBTF) == 0)
                    {
                        flag = 1;
                        Select_ExtMemory(USBTF);
                    }
                    if (flag == 0)
                        break;

                    if (g_from_memory == g_to_memory && strEqual(g_fromPath, g_path))
                    { //复制源和保存路径相同
                        btn_en = 0;
                        SetMyProgress1(files);
                        pnoindex--;
                        pno = poldno[pnoindex];
                        break;
                    }
                    getDiskFiles(g_fromPath, (U16 *)IREG2, GET_ALLFILE); //获取文件源索引
                    //#if Debug
                    //printf("---files = %d \r\n",files);
                    //#endif

                    for (i = 0; i < files; i++)
                    {
                        DrawFillRect(140, vaddr2, 444, 56, bgColor); //待修改
                        SetMyProgress1(i);
                        DrawMFU32Z(528 - 13 * (2 * getNumDigits(files) + 1), vaddr2, i + 1, getNumDigits(files), vTestColor);
                        DrawMFU32Z(528 - 13 * getNumDigits(files), vaddr2, files, getNumDigits(files), vTestColor);
                        Draw24X24FStr(528 - 13 * (getNumDigits(files) + 1), vaddr2, "/", vTestColor);
                        fileNo = getFilesNo((U16 *)g_editfiles, MAXFILENO, i);
                        fileNo1 = getFilesNo1((U16 *)IREG2, fileNo);
                        fileType = getFileTypeByIREG((U16 *)IREG2, fileNo);
                        fname = getFileNameByIREG((U16 *)IREG2, fileNo);
                        if (fname) //防warming
                        {
                        }
                        //#if Debug
                        //printf("i =  %d files = %d fileNo = %d  fileNo1 = %d  fileType = %d \r\n",i,files,fileNo,fileNo1,fileType);
                        //printf("fname = %s \r\n",fname);
                        //#endif
                        setBufDat8(disName, sizeof(disName), 0);
                        if (fileType == FILE_SEW)
                        {
                            p8 = getFileNameByIREG((U16 *)IREG2, fileNo);
                            for (namelen = 0; namelen < (63); namelen++)
                            {
                                if (*(p8 + namelen) == 0)
                                    break;
                                disName[namelen] = *(p8 + namelen);
                            }
                        }
                        else if (fileType == FILE_MITE || fileType == FILE_MITG)
                        {
                            p8 = getFileNameByIREG((U16 *)IREG2, fileNo);
                            for (namelen = 0; namelen < (63 - 4); namelen++)
                            {
                                if (*(p8 + namelen) == 0 || *(p8 + namelen) == '.')
                                    break;
                                disName[namelen] = *(p8 + namelen);
                            }
                            disName[namelen++] = '.';
                            disName[namelen++] = fileNo1 / 100 + 0x30;
                            disName[namelen++] = fileNo1 / 10 % 10 + 0x30;
                            disName[namelen++] = fileNo1 % 10 + 0x30;
                        }
                        else
                        { //其他类型文件
                            if (g_to_memory == MEM_IN)
                                continue; //内存不保存,打板器不保存

                            p8 = getFileNameByIREG((U16 *)IREG2, fileNo);
                            for (namelen = 0; namelen < 63; namelen++)
                            {
                                if (*(p8 + namelen) == 0)
                                    break;
                                disName[namelen] = *(p8 + namelen);
                            }
                        }
                        for (; namelen < 68; namelen++)
                        {
                            disName[namelen] = 0;
                        }
                        Draw24X24FStrByLine(disPosStr_X, vaddr2, (char *)disName, linemaxdisplay, 0, vTestColor);

                        flag = 0;

                        if (ExtMemory_Status(USBTF) == 0)
                        {
                            flag = 1;
                            Select_ExtMemory(USBTF);
                        }

                        if (flag == 0)
                            break;
                        //DelayMs(5000);
                        if (g_to_memory == MEM_IN)
                        { //保存文件到内存
                            ReSREGcount = 0;
                            while (ReSREGcount < 3)
                            {
                                if (readDiskPatFileByName(g_fromPath, (char *)(&IREG2[(fileNo - MINFILENO) * FileIndexSize + FileIndexNamepox]), (U16 *)SREG) != 0)
                                { //读取花样失败
                                    //printf("---erro---\r\n");
                                    ReSREGcount = 3;
                                    break;
                                }
                                len = (F_FILELENH << 16) + F_FILELENL;
                                if (CheckSREG(1, ((fileType == FILE_MITE || fileType == FILE_MITG) ? 0 : 1), ((F_FILELENH << 16) | F_FILELENL)) == TRUE)
                                    break;
                                ReSREGcount += 1;
                                wdt();
                            }
                            //printf("-* ReSREGcount = %d *-\r\n",ReSREGcount);
                            if (ReSREGcount >= 3)
                            {
                                continue;
                            }

                            ReSREGcount = 0;
                            F_IND_NO = fileNo1;
                            while (ReSREGcount < 5)
                            {
                                result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // 发送当前花样文件
                                if (result == 0)
                                { //通讯错误
                                    break;
                                }
                                result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //发送当前文件列表
                                if (result == 0)
                                { //通讯错误
                                    break;
                                }
                                result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //发送保存花样命令
                                if (result == 0)
                                { //通讯错误
                                    break;
                                }
                                if (Read_Status() == 0)
                                {
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] == 98)
                                {
                                    ReSREGcount++;
                                }
                                else if (DREG[D_CMDSTATUS] == 100)
                                { //成功
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] == 12)
                                { //通讯出错，重发
                                    ReSREGcount++;
                                }
                                else
                                {
                                    ReSREGcount++;
                                }
                            }
                        }
                        else
                        {
                            //printf("disName :%s \r\n",disName);
                            copyDiskPatFile(g_fromPath, g_from_memory, g_path, g_to_memory, (char *)disName, 0);
                        }
                        if (temp_para == 176)
                        { //剪切
                            deleteDiskFile(g_fromPath, (char *)disName);
                        }
                    }
                    SetMyProgress1(files);
                }

                //更新复制后的文件索引
                if (g_disk_memory == MEM_IN)
                {
                    ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]);
                }
                else if (g_disk_memory == MEM_USB)
                { //USB
                    if (ExtMemory_Status(USBTF) != 0)
                    {
                        f = ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]);
                        g_disk_memory = MEM_IN;
                        g_disk_memory = MEM_IN;
                        temp_para = 95;
                        dis_updata = 1;
                        break;
                    }
                    else
                    {
                        Select_ExtMemory(USBTF);
                        getDiskFiles(g_path, (U16 *)IREG2, GET_ALLFILE);
                    }
                }

                initFunctionBtn(0);
                pnoindex--;
                pno = poldno[pnoindex];
                break;

            case 178:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (14) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 14, l1, vTestColor);                    //导出
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (14) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 14, l2, vTestColor); //确定要覆盖已存在的文件/文件夹吗
                btn_en = 3;
                break;

            case 179:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (21) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 21, l1, vTestColor);                    //导入
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (21) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 21, l2, vTestColor); //确定要覆盖已存在的文件/文件夹吗
                btn_en = 3;

                break;

            case 180:                                                                                                                             //导入
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (11) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 11, l1, vTestColor); //复制
                files = getFiles((U16 *)g_editfiles, MAXFILENO);
                initFunctionBtn(0);
                setBufDat16((U16 *)g_selfiles, MAXFILENO, 0xffff);
                initMyProgress1(files, bgColor);

                flag = 0;
                if (ExtMemory_Status(USBTF) == 0)
                {
                    flag = 1;
                    Select_ExtMemory(USBTF);
                }
                if (flag == 0)
                    break;

                getDiskFiles(g_fromPath, (U16 *)IREG2, GET_ALLFILE); //获取文件源索引

                for (i = 0; i < files; i++)
                {
                    DrawFillRect(140, vaddr2, 444, 56, bgColor); //待修改
                    SetMyProgress1(i);
                    DrawMFU32Z(528 - 13 * (2 * getNumDigits(files) + 1), vaddr2, i + 1, getNumDigits(files), vTestColor);
                    DrawMFU32Z(528 - 13 * getNumDigits(files), vaddr2, files, getNumDigits(files), vTestColor);
                    Draw24X24FStr(528 - 13 * (getNumDigits(files) + 1), vaddr2, "/", vTestColor);
                    fileNo = getFilesNo((U16 *)g_editfiles, MAXFILENO, i);
                    fileNo1 = getFilesNo1((U16 *)IREG2, fileNo);
                    fileType = getFileTypeByIREG((U16 *)IREG2, fileNo);

                    //#if Debug
                    //printf("i =  %d files = %d fileNo = %d  fileType = %d \r\n",i,files,fileNo,fileType);
                    //#endif
                    setBufDat8(disName, sizeof(disName), 0);
                    if (fileType == FILE_SEW)
                    {
                        g_to_memory = MEM_IN; //导入到内存

                        for (namelen = 0; namelen < sizeof(BorName); namelen++)
                        {
                            disName[namelen] = BorName[namelen];
                        }
                        disName[5] = fileNo1 / 100 + 0x30;
                        disName[6] = fileNo1 / 10 % 10 + 0x30;
                        disName[7] = fileNo1 % 10 + 0x30;
                    }
                    else if (fileType == FILE_MITE || fileType == FILE_MITG)
                    {
                        g_to_memory = MEM_IN; //导入到内存

                        p8 = getFileNameByIREG((U16 *)IREG2, fileNo);
                        for (namelen = 0; namelen < (63 - 4); namelen++)
                        {
                            if (*(p8 + namelen) == 0 || *(p8 + namelen) == '.')
                                break;
                            disName[namelen] = *(p8 + namelen);
                        }
                        disName[namelen++] = '.';
                        disName[namelen++] = fileNo1 / 100 + 0x30;
                        disName[namelen++] = fileNo1 / 10 % 10 + 0x30;
                        disName[namelen++] = fileNo1 % 10 + 0x30;
                    }
                    else
                    {             //其他类型文件
                        continue; //其他文件不导入
                    }
                    for (; namelen < 68; namelen++)
                    {
                        disName[namelen] = 0;
                    }
                    Draw24X24FStrByLine(disPosStr_X, vaddr2, (char *)disName, linemaxdisplay, 0, vTestColor);

                    flag = 0;

                    if (ExtMemory_Status(USBTF) == 0)
                    {
                        flag = 1;
                        Select_ExtMemory(USBTF);
                    }

                    if (flag == 0)
                        break;
                    //DelayMs(5000);
                    if (g_to_memory == MEM_IN)
                    { //保存文件到内存
                        ReSREGcount = 0;
                        while (ReSREGcount < 3)
                        {
                            if (readDiskPatFileByName(g_fromPath, (char *)(&IREG2[(fileNo - MINFILENO) * FileIndexSize + FileIndexNamepox]), (U16 *)SREG) != 0)
                            { //读取花样失败
                                ReSREGcount = 3;
                                break;
                            }
                            len = (F_FILELENH << 16) + F_FILELENL;
                            if (CheckSREG(1, ((fileType == FILE_MITE || fileType == FILE_MITG) ? 0 : 1), ((F_FILELENH << 16) | F_FILELENL)) == TRUE)
                                break;
                            ReSREGcount += 1;
                            wdt();
                        }
                        if (ReSREGcount >= 3)
                        {
                            continue;
                        }

                        ReSREGcount = 0;
                        F_IND_NO = fileNo1;
                        while (ReSREGcount < 5)
                        {
                            result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // 发送当前花样文件
                            if (result == 0)
                            { //通讯错误
                                break;
                            }
                            result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //发送当前文件列表
                            if (result == 0)
                            { //通讯错误
                                break;
                            }
                            result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //发送保存花样命令
                            if (result == 0)
                            { //通讯错误
                                break;
                            }
                            if (Read_Status() == 0)
                            {
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 98)
                            {
                                ReSREGcount++;
                            }
                            else if (DREG[D_CMDSTATUS] == 100)
                            { //成功
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 12)
                            { //通讯出错，重发
                                ReSREGcount++;
                            }
                            else
                            {
                                ReSREGcount++;
                            }
                        }
                    }
                }
                SetMyProgress1(files);

                //更新复制后的文件索引
                if (ExtMemory_Status(USBTF) != 0)
                {
                    f = ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]);
                    g_disk_memory = MEM_IN;
                    temp_para = 95;
                    dis_updata = 1;
                    break;
                }
                else
                {
                    Select_ExtMemory(USBTF);
                    getDiskFiles(g_path, (U16 *)IREG2, GET_ALLFILE);
                }
                initFunctionBtn(0);
                pnoindex--;
                pno = poldno[pnoindex];
                btn_en = 0;
                break;

            case 181:
                ViewMsg(468);
                btn_en = 2;
                break;

            case 182:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //磁盘无文件
                btn_en = 2;
                //PatF1();
                break;

            case 183:
                ViewMsg(470);
                btn_en = 3;
                break;

            case 184:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 471), linemaxdisplay, Err_MSG1_Sheet1, 471, l1, vTestColor); //"有功能动作执行中,请关闭后再退出",
                btn_en = 2;
                //PatF1();
                break;

            case 185:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 472), linemaxdisplay, Err_MSG1_Sheet1, 472, l1, vTestColor); //"有功能动作执行中,请关闭或按复位键后再退出",
                btn_en = 2;
                //PatF1();
                break;
            default:
                temp_para = 0;
                dis_updata = 1;
                break;
            }

            wdt();

            if (btn_en & 0x1) //2 ENTER = 1;
            {
                //
                // if(temp_para==13){
                //	DrawButton2(keyd[2],Color_White1,67,1);
                // }else{
                //	DrawButton2(keyd[2],Color_White1,70,1);
                // }
                // DrawButton2(keyd[2], Color_White1, temp_para == 13 ? 67 : 70, 1);
                DrawButton(keyd[KEY_RWHELP_ENTER], (U16 *)(ADD_ICONBMP),
                           keybmpind[KEY_RWHELP_ENTER].border[0],
                           temp_para == 13 ? keybmpind[KEY_RWHELP_ENTER].icon[1] : keybmpind[KEY_RWHELP_ENTER].icon[0]);
            }
            else if (btn_en & 0x4) //3	 ORG = 4;
            {
                // DrawButton2(keyd[3], Color_White1, 1150, 1);
                DrawButton(keyd[KEY_RWHELP_ORG], (U16 *)(ADD_ICONBMP),
                           keybmpind[KEY_RWHELP_ORG].border[0],
                           keybmpind[KEY_RWHELP_ORG].icon[0]);
            }
            else if (btn_en & 0x8) //4	 NEEDLE = 8;
            {
                // DrawButton2(keyd[4], Color_White1, 1151, 1);
                DrawButton(keyd[KEY_RWHELP_NEEDLE], (U16 *)(ADD_ICONBMP),
                           keybmpind[KEY_RWHELP_NEEDLE].border[0],
                           keybmpind[KEY_RWHELP_NEEDLE].icon[0]);
            }
            else if (temp_para != 9)
            {
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
            }

            if (btn_en & 0x2) //1 EXIT  = 2;
            {
                //if(btn_en==10){	//针杆不在最高位置
                //	DrawButton2(keyd[2],Color_White1,70,1);
                //}else{
                //	DrawButton2(keyd[1],Color_White1,1190,1);
                //}
                // DrawButton2(btn_en == 10 ? keyd[2] : keyd[1], Color_White1, btn_en == 10 ? 70 : 1190, 1);
                DrawButton(btn_en == 10 ? keyd[KEY_RWHELP_ENTER] : keyd[KEY_RWHELP_EXIT], (U16 *)(ADD_ICONBMP),
                           keybmpind[KEY_RWHELP_EXIT].border[0],
                           btn_en == 10 ? keybmpind[KEY_RWHELP_ENTER].icon[0] : keybmpind[KEY_RWHELP_EXIT].icon[0]);
            }
            else
            {
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
            }

            if ((temp_para == 84) || (temp_para == 85))
            {
                // for (i = 0; i < 2; i++)
                // {
                //     // DrawButton2(keyd[5 + i], Color_White1, i == 0 ? 104 : 1191, 1);
                // }
                for (i = KEY_RWHELP_USB; i <= KEY_RWHELP_SD; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               keybmpind[i].border[0],
                               keybmpind[i].icon[0]);
                }
            }
        }

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //获取键值

        // /[按键键值返回预处理*begin]**********************************************************

        //特定情况下不生效按键预处理
        if (keydata == 1)
        {
            if (btn_en == 10)
                keydata = 4;
            else if ((btn_en & 0x2) == 0)
                keydata = 0;
        }
        else if (keydata == 2)
        {
            if (btn_en == 10)
                keydata = 2;
            else if (btn_en & 0x1)
                keydata = 2;
            else if (btn_en & 0x4)
                keydata = 3;
            else if (btn_en & 0x8)
                keydata = 4;
            else
                keydata = 0;
        }
        else if (keydata == 7)
        {
            if (err_btn_en == 0)
                keydata = 0;
        }
        else if (keydata != 0xff)
        {
            if ((temp_para != 84) && (temp_para != 85))
                keydata = 0;
        }
        //无效按键

        if (keydata == 0xff) //按键松开后的键值预处理
        {
            //带有连续按键功能的按键
            if ((keyon) && (beeponf)) //降按键按下时的键值传给按键松开键值
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata >= USESCREENSAVER) //按键按下的键值预处理    与提示性信息处理
        {
            //特定情况下不生效按键

            if (keyon == 0) //按键按下键值赋值与按键按下效果显示(当前没有按键按下时
            {
                keyon = keydata;
                if (keyon == KEY_RWHELP_HERROR) //无按键特效
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
                    // if (keyon == KEY_RWHELP_HERROR)
                    // {
                    //     if (Times > 10000)
                    //     {
                    //         beeponf = 1;
                    //         Beep_On(200);
                    //     }
                    // }
                    // else if (Times > 10)
                    // {
                    //     beeponf = 1;
                    //     Beep_On(70);
                    // }
                    beeponf = 1;
                    if (g_keyBellT) //按键时蜂鸣器发声时间
                        if (keyon != KEY_RWHELP_HERROR)
                            Beep_On(g_keyBellT);
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
            case KEY_RWHELP_EXIT:
                if (temp_para == 12)
                {
                    tmpcmd = C_PATUPDATE; //更新花样命令
                    temp_para = 10;
                    dis_updata = 1;
                }
                else if ((temp_para == 13) || (temp_para == 17) || (temp_para == 60))
                { //压板
                    ComErrMessage = 0;
                    if (temp_para == 13)
                    {
                        S_ERRORCLR();
                        Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                    }
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if ((temp_para == 21) && (dat == 13))
                {
                    if ((DREG[D_ERRWAR]))
                    {
                        S_ERRORCLR();
                        Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                    }
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if ((temp_para == 32) || (temp_para == 38))
                { //从磁盘复制花样到内存
                    ReadFileIndex(C_RDMEMPATLIST, MENU_FTCOPY);
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if ((temp_para == 141) || (temp_para == 142))
                { //从磁盘复制花样到内存
                    for (i = 0; i < MAXSREG; i++)
                    {
                        SREG[i] = 0;
                    }
                    Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // 读当前花样数据
                    pno = plastno;
                }
                else if (temp_para == 156)
                { //恢复原来花样数据
                    //#if Debug
                    //printf("temp_para = 156  g_Fileno = %d \r\n",g_Fileno);
                    //#endif
                    Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                    RestoreSHREG(0x03);
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if (temp_para == 157)
                {
                    RestoreSHREG(0x03);
                    pnoindex -= 2;
                    pno = poldno[pnoindex]; //回到主界面
                }
                else if (temp_para == 164)
                {
                    temp_para = 165;
                    dis_updata = 1;
                }
                else if (temp_para == 183)
                { //重启系统
                    setBootScreenMode(g_bootscr.mode, g_bootscr.vmlk);
                    pnoindex -= 2;
                    pno = poldno[pnoindex];
                }
                else
                {
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                delaytimes = 0;

                break;

            case KEY_RWHELP_ENTER:

                if (temp_para == 0)
                { //删除内存花样
                    if ((g_selectnumber < MINFILENO) || (g_selectnumber > MAXFILENO))
                    {
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }
                        break;
                    }
                    temp_para = 7;
                    dis_updata = 1;
                }
                else if (temp_para == 1)
                { //删除U盘花样
                    if ((g_selectnumber < MINFILENO) || (g_selectnumber > MAXFILENO))
                    {
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }
                        break;
                    }
                    temp_para = 8;
                    dis_updata = 1;
                }
                else if (temp_para == 78)
                { //删除SD卡花样
                    if ((g_selectnumber < MINFILENO) || (g_selectnumber > MAXFILENO))
                    {
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }
                        break;
                    }
                    temp_para = 79;
                    dis_updata = 1;
                }
                else if (temp_para == 12)
                {
                    if (g_para0 == 12)
                    {
                        if (g_disk_memory == 0)
                        {
                            tmpcmd = C_WRITEMEMORYPAT; //保存花样到记忆体命令
                            temp_para = 10;
                        }
                        else if (g_disk_memory == 1)
                        {
                            //if(g_MboardUSB==1){		//主板U盘
                            //	tmpcmd = C_WRITEFLASHPAT;	//保存花样到U盘
                            //}else{
                            Select_ExtMemory(USBTF);
                            Delete_PatFile(F_IND_NO);
                            //}
                            temp_para = 81;
                        }
                        else if (g_disk_memory == 2)
                        { //保存花样到SD卡
                            //	Select_ExtMemory(SD);
                            //	Delete_PatFile(F_IND_NO);
                            temp_para = 11;
                        }
                    }
                    else
                    {
                        tmpcmd = C_WRITEMEMORYPAT; //保存花样命令
                        temp_para = 10;
                    }
                    g_para0 = 0;
                    dis_updata = 1;
                }
                else if (temp_para == 13)
                { //压板
                    //printf("Write_Cmd(%d,0,0) \r\n",C_YABANDOWN);
                    Write_Cmd(C_YABANDOWN, 0, 0);
                    //if(ComErrMessage)	ComErrMessage = 0;
                    //S_ERRORCLR();Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS);
                    //if(pnoindex){pnoindex --;pno = poldno[pnoindex];}else{pno = plastno;}
                }
                else if (temp_para == 18)
                {
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                }
                else if (temp_para == 21)
                {
                    if (dat == 13)
                    {
                        g_NoUpdataoldno = 1;
                        g_backSREG_f = 1;
                        g_backHREG_f = 1;
                        g_Fileno = F_IND_NO;
                        g_backSREGlen = (F_FILELEN) / 2;
                        BackupSHREG((g_backHREG_f << 1) | g_backSREG_f);

                        f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息

                        Write_Cmd(C_RECEIVE_FILE, 0, 0); //发送确认接收远程文件指令
                        Read1_Status(8);
                        //for(i=0;i<RDTIMES*8;i++){
                        //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                        //	if(DREG[D_CMDSTATUS])break;
                        //	DelayMs(WAITTIME);
                        //}
                        if (DREG[D_CMDSTATUS] == 100)
                        {
                            f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                            if (f)
                            {
                                len = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
                                f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样文件
                                if (f == 0)
                                {
                                    temp_para = 15;
                                    dis_updata = 1;
                                }
                                else
                                {
                                    //										if(F_FILETYPE==1){
                                    //											checksum = 0;
                                    //											for(i=0;i<len;i++){
                                    //												if(i!=0x20){checksum +=SREG[i];}
                                    //											}
                                    //											SREG[0x20] = checksum;
                                    //											//Write_D(D_Address[PTDATA]+0x20,1,(U16 *)(&SREG[0x20])); // 发送当前花样文件校验值
                                    //										}else{
                                    //											checksum = 0;
                                    //											for(i=0;i<len;i++){
                                    //												if(i!=0x05){checksum +=SREG[i];}
                                    //											}
                                    //											SREG[0x05] = checksum;
                                    //											//Write_D(D_Address[PTDATA]+0x05,1,(U16 *)(&SREG[0x05])); // 发送当前花样文件校验值
                                    //										}
                                    pno = MENU_WRITE1;
                                    g_recv_netdata = 1;
                                    ViewPage(usepageno);
                                }
                            }
                            else
                            {
                                temp_para = 15;
                                dis_updata = 1;
                            }
                        }
                        else
                        {
                            temp_para = 16;
                            dis_updata = 1;
                        }
                    }
                    else if (dat == 1)
                    {
                        //	S_NEEDLESET();
                        //							if(pnoindex){pnoindex --;pno = poldno[pnoindex];}else{pno = plastno;}
                    }
                    else if (dat == 391)
                    { //远程升级Timerms
                        temp_para = 155;
                        dis_updata = 1;
                    }
                    if ((DREG[D_ERRWAR]))
                    {
                        S_ERRORCLR();
                        Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                    }
                }
                else if (temp_para == 24)
                { //确定格式化内存
                    temp_para = 26;
                    dis_updata = 1;
                }
                else if (temp_para == 25)
                { //确定格式化sd
                    temp_para = 27;
                    dis_updata = 1;
                }
                else if (temp_para == 82)
                { //确定格式化usb
                    temp_para = 83;
                    dis_updata = 1;
                }
                else if (temp_para == 31)
                { // 从内存复制资料到磁盘
                    temp_para = 33;
                    dis_updata = 1;
                }
                else if (temp_para == 32)
                { // 从磁盘复制资料到内存
                    temp_para = 34;
                    dis_updata = 1;
                }
                else if (temp_para == 37)
                { // 从内存复制所有资料到磁盘
                    temp_para = 35;
                    dis_updata = 1;
                }
                else if (temp_para == 38)
                { // 从磁盘复制所有资料到内存
                    temp_para = 36;
                    dis_updata = 1;
                }
                else if (temp_para == 41)
                { //备份设定表到磁盘
                    temp_para = 43;
                    dis_updata = 1;
                }
                else if (temp_para == 42)
                { //还原设定表
                    temp_para = 44;
                    dis_updata = 1;
                }
                else if (temp_para == 45)
                { //备份控制表到磁盘
                    temp_para = 46;
                    dis_updata = 1;
                }
                else if (temp_para == 47)
                { //还原备份控制表
                    temp_para = 48;
                    dis_updata = 1;
                }
                else if (temp_para == 49)
                { //备份所有
                    temp_para = 51;
                    dis_updata = 1;
                }
                else if (temp_para == 50)
                { //还原所有
                    temp_para = 52;
                    dis_updata = 1;
                }
                else if (temp_para == 55)
                {
                    Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // 读当前花样文件信息
                    m = F_IND_NO;
                    if ((m >= MINFILENO) && (m <= MAXFILENO))
                    {
                        n = (m - MINFILENO) * FileIndexSize;
                        len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                        n = 0; // 如果读花样失败，初始化SREG寄存器
                        if ((len >= 0x40) && (len <= MAXSREG))
                        {
                            n = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // 读当前花样数据
                            if (n)
                            {
                                coldstart = 0;
                                g_init_flag = 1;
                                g_selectdisno = F_IND_NO;
                                g_data_flash = 1;
                            }
                        }
                    }
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if (temp_para == 61)
                { //备份系统参数到内存
                    Write_Cmd(C_SYSRECOVER, 5, 6);
                    Read_Status();
                    //for(i=0;i<RDTIMES;i++){
                    //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                    //	if((DREG[D_CMDSTATUS]))break;
                    //	DelayMs(WAITTIME);
                    //}
                    if ((DREG[D_CMDSTATUS]) == 100)
                    {
                        pno = MENU_FUNCTION;
                    }
                    else
                    {
                        temp_para = 15;
                        dis_updata = 1;
                    }
                }
                else if (temp_para == 62)
                { //还原备份系统设置
                    Write_Cmd(C_SYSRECOVER, 3, 4);
                    Read_Status();
                    //for(i=0;i<RDTIMES;i++){
                    //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                    //	if((DREG[D_CMDSTATUS]))break;
                    //	DelayMs(WAITTIME);
                    //}
                    if ((DREG[D_CMDSTATUS]) == 100)
                    {
                        f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 读取参数值
                        if (f)
                        {
                        }
                        //调节液晶屏对比度
                        SetLcdPwm(1, P_CNR);
                        SaveBackLight(P_CNR);
                        //							DelayMs(1000);
                        ///WriteData( BACKLIGHT_VAL_ADDR, 4,(U32 *)(&P_CNR) );	//要改要改要改
                        DelayMs(50);

                        gPeessVal = P_CTR;
                        if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL)
                        {
                            gPeessVal = ReadPessVal();
                        }
                        else
                        {
                            WritePessVal(gPeessVal);
                        }

                        pno = MENU_FUNCTION;
                    }
                    else
                    {
                        temp_para = 15;
                        dis_updata = 1;
                    }
                }
                else if (temp_para == 63)
                { //参数还原
                    Write_Cmd(C_SYSRECOVER, 1, 2);
                    if (Read_Status() == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    if ((DREG[D_CMDSTATUS]) == 100)
                    {
                        f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 读取参数值
                        if (f)
                        {
                        }
                        //调节液晶屏对比度
                        SetLcdPwm(1, P_CNR);
                        SaveBackLight(P_CNR);
                        //							DelayMs(1000);
                        DelayMs(50);
                        gPeessVal = P_CTR;
                        if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL)
                        {
                            gPeessVal = ReadPessVal();
                        }
                        else
                        {
                            WritePessVal(gPeessVal);
                        }

                        if (g_Restorepropara == 1)
                        {
                            pno = MENU_DIFFERENT;
                            g_Restorepropara = 2;
                        }
                        else
                        {
                            pno = MENU_FUNCTION;
                        }
                    }
                    else
                    {
                        temp_para = 15;
                        dis_updata = 1;
                    }
                }
                else if (temp_para == 68)//升级系统
                { 
                    temp_para = 69;
                    dis_updata = 1;
                }
                else if (temp_para == 75)
                {
                    //	pno = MENU_CHKTOUCH;	//进入触屏校准页面
                    //	bpno = MENU_P1;
                }
                else if (temp_para == 86)
                {
                    temp_para = 87;
                    dis_updata = 1;
                }
                else if (temp_para == 96)
                {
                    poldno[1] = MENU_FTSYSRESTORE;
                    pnoindex = 1;
                    poldno[0] = MENU_P1;
                    pno = MENU_FTSYSRESTORE;
                }
                else if (temp_para == 103)
                { //清除合并图形
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
                    for (i = 0; i < 8; i++)
                    {
                        g_GroupName[i] = 0;
                    }
                    g_GroupMenth = 3; //标记清除当前组合文件
                    g_Groupsel1 = 0;  //回到第一个选择
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if (temp_para == 105)//删除记忆体合并图形
                { 
                    temp_para = 106;
                    dis_updata = 1;
                }
                else if (temp_para == 107)//删除磁碟片上合并图形
                { 
                    temp_para = 108;
                    dis_updata = 1;
                }
                else if (temp_para == 109)//写合并图形到磁盘
                { 
                    pnoindex--;
                    pno = MENU_FTGROUP;
                }
                else if (temp_para == 111)//读磁盘合并图形
                { 
                    temp_para = 112;
                    dis_updata = 1;
                }
                else if (temp_para == 126)//备份系统参数到U盘
                { 
                    temp_para = 128;
                    dis_updata = 1;
                    g_disk_memory = 1;
                }
                else if (temp_para == 127)//备份系统参数到SD卡
                { 
                    temp_para = 128;
                    dis_updata = 1;
                    g_disk_memory = 2;
                }
                else if (temp_para == 129)
                { //备份IO设定参数到U盘
                    temp_para = 131;
                    dis_updata = 1;
                    g_disk_memory = 1;
                }
                else if (temp_para == 130)
                { //备份IO设定参数到SD卡
                    temp_para = 131;
                    dis_updata = 1;
                    g_disk_memory = 2;
                }
                else if (temp_para == 132)
                { //还原系统参数
                    temp_para = 133;
                    dis_updata = 1;
                }
                else if (temp_para == 134)
                { //还原IO设定参数
                    temp_para = 135;
                    dis_updata = 1;
                }
                else if (temp_para == 136)
                { //还原IO 输入设定参数
                    if (Write_Cmd(C_REINPUTSETTING, 0, 0) == 0)
                    { //发送还原设定指令
                        temp_para = 16;
                        dis_updata = 1;
                        break;
                    }
                    if (Read_Status() == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    else if (DREG[D_CMDSTATUS] != 100)
                    {
                        temp_para = 16;
                        dis_updata = 1;
                        break;
                    }
                    g_NO_ReagIOf = 0; //重读IO参数
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if (temp_para == 137)
                { //还原IO 输出设定参数
                    if (Write_Cmd(C_REOUTPUTSETTING, 0, 0) == 0)
                    { //发送还原设定指令
                        temp_para = 16;
                        dis_updata = 1;
                        break;
                    }
                    if (Read_Status() == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    else if (DREG[D_CMDSTATUS] != 100)
                    {
                        temp_para = 16;
                        dis_updata = 1;
                        break;
                    }
                    g_NO_ReagIOf = 0; //重读IO参数
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if (temp_para == 141)
                {
                    pno = MENU_MODIFY;
                }
                else if (temp_para == 142)
                {
                    pno = MENU_CONVERT;
                }
                else if (temp_para == 151)
                { //备份记录信息到U盘
                    temp_para = 153;
                    dis_updata = 1;
                    g_disk_memory = 1;
                }
                else if (temp_para == 152)
                { //备份记录信息到SD卡
                    temp_para = 153;
                    dis_updata = 1;
                    g_disk_memory = 2;
                }
                else if (temp_para == 162)
                { //切换到H-操作系统
                    temp_para = 163;
                    dis_updata = 1;
                }
                else if (temp_para == 164)
                {
                    temp_para = 166;
                    dis_updata = 1;
                }
                else if (temp_para == 167)
                { //重命名
                    if (g_disk_memory == MEM_IN)
                    {
                        U16 no = (g_newname[0] - '0') * 100 + (g_newname[1] - '0') * 10 + (g_newname[2] - '0');
                        U8 fileType = getFileTypeByIREG((U16 *)IREG2, g_patd.fno);
                        U32 addr = 0;
                        Mit_DealSaveData(fileType == FILE_SEW ? PAT_SEW : PAT_MIT, g_patd.pat_len);

                        F_IND_NO = no;
                        F_FILETYPE = fileType == FILE_SEW ? PAT_SEW : PAT_MIT;
                        if (fileType == FILE_SEW)
                        {
                            setBufDat8((U8 *)g_patname, sizeof(g_patname), 0);
                            g_patname[0] = 'I';
                            g_patname[1] = 'S';
                            g_patname[2] = 'M';
                            g_patname[3] = 'S';
                            g_patname[4] = '0';
                            g_patname[5] = ((no % 1000) / 100) + '0';
                            g_patname[6] = ((no % 100) / 10) + '0';
                            g_patname[7] = (no % 10) + '0';
                            g_patname[8] = '.';
                            g_patname[9] = 'S';
                            g_patname[10] = 'E';
                            g_patname[11] = 'W';
                        }
                        for (i = 0; i < 32; i++)
                        {
                            if (g_patname[2 * i] == 0)
                                break;
                            HREG[32 + i] = (g_patname[2 * i + 1] << 8) | g_patname[2 * i];
                        }
                        //+----	 删除当前编号花样---------
                        DREG[D_COMMAND] = C_DELMEMORYPAT; // 删除花样
                        DREG[D_CMDSTATUS] = 0;
                        DREG[D_CMDPARA1] = g_patd.fno;
                        f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND]));
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (Read_Status() == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (DREG[D_CMDSTATUS] != 100)
                        {
                            temp_para = 16;
                            dis_updata = 1;
                            break;
                        }
                        //更新文件索引
                        addr = (g_patd.fno - MINFILENO) * PTINDEXREDLEN;
                        for (i = 0; i < FileIndexSize; i++)
                        {
                            IREG2[addr + i] = 0xffff;
                        }

                        //----- 保存新编号花样
                        g_patd.fno = F_IND_NO;
                        f = Write_D(D_Address[PTDATA], (g_patd.pat_len >> 1), (U16 *)(&SREG[0])); //发送花样数据
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        f = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO));
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        if (Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0) == 0)
                        {
                            temp_para = 16;
                            dis_updata = 1;
                            break;
                        }
                        //更新文件索引
                        addr = (F_IND_NO - MINFILENO) * PTINDEXREDLEN;
                        IREG2[addr + PTINDEXLENADD + 0] = g_patd.pat_len;
                        IREG2[addr + PTINDEXLENADD + 1] = g_patd.pat_len >> 16;
                        IREG2[addr + PTINDEXENCADD + 0] = 0;
                        IREG2[addr + PTINDEXENCADD + 1] = 0;
                        for (i = 0; i < 32; i++)
                        {
                            IREG2[addr + PTINDEXNAMEADD + i] = HREG[32 + i];
                        }
                        if (F_FILETYPE == PAT_MIT)
                        {
                            IREG2[addr + PTINDEXNAMEADD + 31] &= 0x00ff;
                            IREG2[addr + PTINDEXNAMEADD + 31] |= 0x0100;
                        }
                        else
                        {
                            IREG2[addr + PTINDEXNAMEADD + 31] &= 0x00ff;
                        }
                        pno = MENU_FTFILES;
                        pnoindex = 2;
                        break;
                    }
                }
                else if (temp_para == 170)
                { //文件管理-删除
                    temp_para = 171;
                    dis_updata = 1;
                }
                else if (temp_para == 173)
                { //复制粘贴
                    temp_para = 175;
                    dis_updata = 1;
                }
                else if (temp_para == 174)
                { //剪切粘贴
                    temp_para = 176;
                    dis_updata = 1;
                }
                else if (temp_para == 178)
                { //导出
                    temp_para = 177;
                    dis_updata = 1;
                }
                else if (temp_para == 179)
                { //导入
                    temp_para = 180;
                    dis_updata = 1;
                    break;
                }
                else if (temp_para == 183)
                { //重启系统
                    setBootScreenMode(g_bootscr.mode, g_bootscr.vmlk);

                    EXTI->IMR = 0x00000000;
                    EXTI->EMR = 0x00000000;

                    RCC->AHB1RSTR = 0xffffffff;
                    RCC->AHB2RSTR = 0xffffffff;
                    RCC->AHB3RSTR = 0xffffffff;
                    RCC->APB1RSTR = 0xffffffff;
                    RCC->APB2RSTR = 0xffffffff;
                    theUboot = (void (*)(void))(*((U32 *)(UPGRADEADDR + 4)));
                    MSR_MSP(*(U32 *)(UPGRADEADDR));
                    theUboot();
                    break;
                }
                else
                {
                    if ((temp_para == 13) || (temp_para == 94))
                    {
                        if (ComErrMessage)
                            ComErrMessage = 0;
                    }
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                break;

            case KEY_RWHELP_ORG:
                if (temp_para == 158)
                {
                    f = Go_Home(MENU_RWHELP, 1); //0
                    if (f == 0 && S_ERROR && P_FHP)
                    {
                        Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                        temp_para = 21;
                        dis_updata = 1;
                        poldno[pnoindex++] = pno;
                        break;
                    }
                }
                else if (temp_para == 159)
                {
                    if (P_FHP)
                        f = Comm_Move(0, 0, 50, MENU_RWHELP, 0, 0);
                    else
                        f = Go_Home(MENU_RWHELP, 0); //0
                }
                else
                {
                    f = Go_Home(MENU_RWHELP, 0); //0
                }

                if (f)
                {
                    if (S_ERROR)
                    {
                        if (temp_para == 148)
                        {
                            Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                            temp_para = 21;
                            dis_updata = 1;
                            break;
                        }
                        else if (temp_para == 157 || temp_para == 158 || temp_para == 159)
                        {
                            Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                            temp_para = 21;
                            dis_updata = 1;
                            poldno[pnoindex++] = pno;
                            break;
                        }
                        else
                        {
                            S_ERRORCLR();
                            Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                        }
                    }
                    else
                    {
                        if (temp_para == 148)
                        {
                            g_BorSTflag = 0;
                            g_max_feedsum = g_patd.feed_sum;
                            g_min_feedsum = 0;
                            g_jog_point = 0;
                            g_IsLocal = 1; //标记使用快速定位功能
                            g_ImgMove = 1;
                            g_curPinAng = 0;
                            g_curSetPara1 = 0;
                            g_curSetPara2 = 0;
                            g_pointPinAng = 0;
                            g_pointSetPara1 = 0;
                            g_pointSetPara2 = 0;
                            
                            pno = MENU_MOIMAGE;
                            // g_imgpara = 0;
                            pnoindex--;
                            poldno[pnoindex] = MENU_P1;
                            pnoindex++;
                            break;
                        }
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }

                        if (temp_para == 158)
                        {
                            g_jog_point = 0;
                            g_jog_stpoint = 0;
                            g_jog_enpoint = 0;
                            g_BorSTflag = 0;
                            g_needtohome = 0;
                            g_ExCode = 0;
                            g_curExCode = 0;
                        }
                        else if (temp_para == 159)
                        {
                            pno = MENU_CURSOR;
                        }
                    }
                }
                break;

            case KEY_RWHELP_NEEDLE:

                if ((DREG[D_ERRWAR]))
                {
                    S_ERRORCLR();
                    Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                }

                Write_Cmd(C_NEEDLE_UP, 0, 0);
                Read_Status();
                //for(i=0;i<RDTIMES;i++){
                //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
                //	if((DREG[D_CMDSTATUS]))break;
                //	DelayMs(WAITTIME);
                //}
                if ((DREG[D_CMDSTATUS]) == 100)
                {
                    if (g_savemodify == 1)
                    {
                        //g_savemodify = 0;
                        pno = MENU_INHELP;
                        hleppara = 0;
                        pnoindex -= 2;
                        poldno[pnoindex] = MENU_MODIFY;
                        pnoindex++;
                    }
                    else
                    {
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }
                    }
                }
                else
                {
                    temp_para = 15;
                    dis_updata = 1;
                }
                break;

            case KEY_RWHELP_USB: //选择U盘存储设备
                g_disk_memory = 1;
                if (temp_para == 84)
                {
                    if (g_para0 == 134)
                    {
                        temp_para = 134;
                    }
                    else if (g_para0 == 132)
                    {
                        temp_para = 132;
                    }
                    else if (g_para0 == 129)
                    {
                        temp_para = 129;
                    }
                    else if (g_para0 == 126)
                    {
                        temp_para = 126;
                    }
                    else if (g_para0 == 151)
                    {
                        temp_para = 151;
                    }
                    else if (g_para0 == 31)
                    {
                        temp_para = 31;
                    }
                    else
                    {
                        temp_para = 37;
                    }
                }
                else
                { // 系统升级 85
                    temp_para = 68;
                }
                dis_updata = 1;
                break;
            case KEY_RWHELP_SD: //选择SD卡存储设备
                g_disk_memory = 2;
                if (temp_para == 84)
                {
                    if (g_para0 == 134)
                    {
                        temp_para = 134;
                    }
                    else if (g_para0 == 132)
                    {
                        temp_para = 132;
                    }
                    else if (g_para0 == 129)
                    {
                        temp_para = 130;
                    }
                    else if (g_para0 == 126)
                    {
                        temp_para = 127;
                    }
                    else if (g_para0 == 151)
                    {
                        temp_para = 152;
                    }
                    else if (g_para0 == 31)
                    {
                        temp_para = 31;
                    }
                    else
                    {
                        temp_para = 37;
                    }
                }
                else
                { // 系统升级 85
                    temp_para = 68;
                }
                dis_updata = 1;
                break;

            case KEY_RWHELP_HERROR:
                if (isContain_U16((U16 *)g_errMsg.err, ERRMSGMAX, DREG[D_ERRWAR]) == 0)
                {
                    if (g_errMsg.max < ERRMSGMAX)
                    {
                        g_errMsg.err[g_errMsg.max] = DREG[D_ERRWAR];
                    }
                    g_errMsg.max++;
                }
                if ((DREG[D_ERRWAR]))
                {
                    S_ERRORCLR();
                    Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                }
                if (pnoindex)
                {
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else
                {
                    pno = plastno;
                }
                //printf("KEY_RWHELP_HERROR \r\n");
                break;
            }

            keyoff = 0;
        }
        // /[按键松开键值处理*end]**********************************************************

        // /[按键弹起显示信息处理*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //按键弹起显示信息处理
        {

            if (keyon == KEY_RWHELP_HERROR) //不需要显示的按键
            {
            }
            //非常规按键-//绘制颜色文字按键
            else //常规按键
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            switch (keyon)
            {
            case KEY_RWHELP_EXIT:
                break;
            case KEY_RWHELP_ENTER:
                break;
            case KEY_RWHELP_ORG:
                break;
            case KEY_RWHELP_NEEDLE:
                break;
            case KEY_RWHELP_USB:
                break;
            case KEY_RWHELP_SD:
                break;
            }
            keyon = 0;
            beeponf = 0;
        }
        // /[按键弹起显示信息处理*end]**********************************************************

        // /[定时1s信息处理*begin]**********************************************************
        if (Timerms > 10000)
        {
            if ((temp_para == 21) && (dat == 391)) //10s 无操作自动远程升级
            {
                temp_para = 155;
                dis_updata = 1;
                Timerms = 0;
                if ((DREG[D_ERRWAR]))
                {
                    S_ERRORCLR();
                    Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                }
            }
        }
        // /[定时1s信息处理*end]**********************************************************

        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*begin]**********************************************************
        if (beeponf == 0)
        {
        }
        // /[蜂鸣器未响信息处理,包含跳针信息，花样更新*end]**********************************************************
        if (Timer_ms > 300)
        {
            Timer_ms = 0;
            if ((ComErrMessage == 1) && (temp_para == 60))
            {
                i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
                if ((i) && (YREG[SYS_COMEN] == 0x5a5a))//如果通讯正常，就直接退出
                { 
                    ComErrMessage = 0;
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
            }
            else if (temp_para == 94)
            {
                U8 needlef = 0xff;
                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                needlef = (U8)S_NEEDLE;
                if (0x20 == needlef)
                {
                    if (ComErrMessage)
                        ComErrMessage = 0;
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
            }
            else
            {
                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                DelayMs(1);
                Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));

                if ((temp_para == 21) && (dat == 13) && (pno == MENU_WRITE1))
                {
                }
                else if ((temp_para == 21) && (!(S_ERROR) /*&&(0x20==(U8)(S_NEEDLE))*/))
                {
                    if (ComErrMessage)
                        ComErrMessage = 0;
                    if (g_savemodify == 1 && g_cttohome != 1)
                    {
                        //	Beep_On(2000);
                        //g_savemodify = 0;
                        pno = MENU_INHELP;
                        hleppara = 0;
                        pnoindex -= 2;
                        poldno[pnoindex] = MENU_MODIFY;
                        pnoindex++;
                    }
                    else
                    {
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }
                        if (pno == MENU_RWHELP)
                        {
                            temp_para = otemp_para;
                            dis_updata = 1;
                        }
                    }
                }
                else if ((temp_para == 13) && (S_CLAMP))
                { //!(S_ERROR)
                    if (ComErrMessage)
                        ComErrMessage = 0;
                    S_ERRORCLR();
                    Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if ((temp_para == 17) && (S_RUN))
                {
                    if (ComErrMessage)
                        ComErrMessage = 0;
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if ((temp_para == 21) && (DREG[D_ERRWAR] == 0x8001) && (g_UdZhouFlag != 3) && (0x20 == (U8)(S_NEEDLE)))
                {
                    if ((DREG[D_ERRWAR]))
                    {
                        S_ERRORCLR();
                        Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                    }
                    if (ComErrMessage)
                        ComErrMessage = 0;
                    if (pnoindex)
                    {
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }
                    else
                    {
                        pno = plastno;
                    }
                }
                else if ((temp_para == 21) && (g_UdZhouFlag == 3) && ((PINERR == 0x8001) || (PINERR == 0x8075)))
                {
                    if (DREG[D_ERRWAR] != PINERR)
                    {
                        if (pnoindex)
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                        }
                        else
                        {
                            pno = plastno;
                        }
                    }
                }
                else if ((temp_para == 21) && (S_ERROR))
                {
                }

                if (temp_para == 9)
                {
                    Read_D(D_Address[DISP] + D_CMDSTATUS, 2, (U16 *)(&DREG[D_CMDSTATUS]));
                    if (DREG[D_CMDSTATUS] == 100)
                    {
                        Read_D(D_SYS + SYS_FSIZE, 3, (U16 *)(&YREG[SYS_FSIZE]));
                        if ((tmp_fsize != YREG[SYS_FSIZE]) || (tmp_fuseed != YREG[SYS_FUSEED]) || (tmp_sumfile != YREG[SYS_SUMFILE]))
                        {
                            if (YREG[SYS_SUMFILE] > SUMFILENO)
                                YREG[SYS_SUMFILE] = 0;
                            if (YREG[SYS_FUSEED] > YREG[SYS_FSIZE])
                                YREG[SYS_FUSEED] = 0;
                            dat1 = YREG[SYS_SUMFILE] * 100 / SUMFILENO;
                            if (YREG[SYS_FSIZE])
                                dat2 = YREG[SYS_FUSEED] * 100 / YREG[SYS_FSIZE];
                            else
                                dat2 = 0;
                            tmp_fsize = YREG[SYS_FSIZE];
                            tmp_fuseed = YREG[SYS_FUSEED];
                            tmp_sumfile = YREG[SYS_SUMFILE];

                            for (i = 1; i < RECT_RWHELP2_MAX; i++)
                            {
                                if (i == RECT_RWHELP2_PINSUM_STR ||
                                    i == RECT_RWHELP2_PINUSE_STR ||
                                    i == RECT_RWHELP2_PATSUM_STR ||
                                    i == RECT_RWHELP2_PATUSE_STR)
                                {
                                    DrawFillRect(rect2[i].xpis, rect2[i].ypis, rect2[i].xsize, rect2[i].ysize, rect2[i].colorB[0]);
                                }
                            }
                            if (dat1 || dat2) //防warming
                            {
                            }
                            DrawMFS32P(rect2[RECT_RWHELP2_PINSUM_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PINSUM_STR].ypis + SAFE_DIST1,
                                       SUMFILENO, 5, 0, rect2[RECT_RWHELP2_PINSUM_STR].colorT[0]); //总共写入针数
                            DrawMFS32P(rect2[RECT_RWHELP2_PINUSE_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PINUSE_STR].ypis + SAFE_DIST1,
                                       YREG[SYS_SUMFILE], 5, 0, rect2[RECT_RWHELP2_PINUSE_STR].colorT[0]); //已占用针数
                            DrawMFS32P(rect2[RECT_RWHELP2_PATSUM_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PATSUM_STR].ypis + SAFE_DIST1,
                                       YREG[SYS_FSIZE], 5, 0, rect2[RECT_RWHELP2_PATSUM_STR].colorT[0]); //记忆体总容量
                            DrawMFS32P(rect2[RECT_RWHELP2_PATUSE_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PATUSE_STR].ypis + SAFE_DIST1,
                                       YREG[SYS_FUSEED], 5, 0, rect2[RECT_RWHELP2_PATUSE_STR].colorT[0]); //已使用记忆体

                            // DrawMFS32P(480, 156, SUMFILENO, 5, 0, Color_Black); //总共写入针数
                            // // DrawMFS32P(160, 212, dat1, 3, 0, Color_Black);                                 //已使用针数百分比
                            // DrawMFS32P(480, 268, YREG[SYS_SUMFILE], 5, 0, Color_Black); //已占用针数
                            // // DrawMFS32P(480, 324, (SUMFILENO - YREG[SYS_SUMFILE]), 5, 0, Color_Black);      //剩余可用针数

                            // DrawMFS32P(480, 380, YREG[SYS_FSIZE], 5, 0, Color_Black); //记忆体总容量
                            // // DrawMFS32P(160, 436, dat2, 3, 0, Color_Black);                                 //已使用记忆体百分比
                            // DrawMFS32P(480, 492, YREG[SYS_FUSEED], 5, 0, Color_Black); //已使用记忆体
                            // // DrawMFS32P(480, 548, (YREG[SYS_FSIZE] - YREG[SYS_FUSEED]), 5, 0, Color_Black); //未使用记忆体
                        }
                    }
                }
            }
        }

        if ((tmp_error != DREG[D_ERRWAR]) && (temp_para == 21))
        {
            dis_updata = 1;
            tmp_error = DREG[D_ERRWAR];
        }
        if ((temp_para == 18) && (P_HPK == 1) && (delaytimes == 0))
        {
            if (pnoindex)
            {
                pnoindex--;
                pno = poldno[pnoindex];
            }
            else
            {
                pno = plastno;
            }
        }

        // /[显示缓存更新设置*begin]**********************************************************
        // if (disflag)
        // {
        //     ViewPage(usepageno);
        //     disflag = 0;
        // }
        // /[显示缓存更新设置*end]**********************************************************

        // /[页面切换*begin]**********************************************************
        if (pno != MENU_RWHELP)
        {
            plastno = MENU_RWHELP;
            break;
        }
        // /[页面切换*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}
