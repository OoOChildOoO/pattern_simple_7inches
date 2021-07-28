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
extern volatile U8 g_backSREG_f; // ����SERG���ݱ�־
extern volatile U8 g_backHREG_f; // ����HERG���ݱ�־
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
extern volatile S16 g_upDISP;   // ���������
extern volatile S16 g_upFLASH;  // ��������� FLASH
extern volatile S16 g_upMBOARD; // ��������
extern volatile S16 g_upWIFI;   // ����wifiģ��
extern MachineInfo g_Mac;
extern ProInfo g_ProInfo;
extern volatile U8 g_UdZhouFlag;
extern volatile U8 g_savemodify;
extern volatile U8 g_updataIREG;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos;
extern volatile U8 g_needtohome;
extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U8 g_ExCode; //��ʾ���ӹ������״̬
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
volatile U8 g_NO_ReagIOf = 0; //����ȡIO������־
extern volatile U8 g_needtohome;
extern volatile U8 g_upinfo[0x20];
extern volatile U8 g_patname[64];

extern volatile U8 g_disFileMethod; //��ʾ�ļ���ʽ 0-�б�ģʽ 1-ͼ��ģʽ
extern volatile U16 g_SelFileNo;
extern volatile U16 g_selfiles[MAXFILENO];  //ѡ���ļ�
extern volatile U16 g_editfiles[MAXFILENO]; //�༭�ļ�
extern volatile U16 g_btndisable;           //���ð���
extern volatile U8 g_checkMode;             //��ѡ/��ѡģʽ
extern volatile U8 g_selALL;                //ȫѡ
extern volatile U8 g_from_memory;
extern volatile U8 g_to_memory;
extern volatile U8 g_clipboard; //���а�
extern volatile U8 g_savePathTimes;
extern char *g_path;
extern char *g_fromPath;
extern char *g_savePath;
extern volatile U8 g_newname[64];
extern volatile U8 g_oldname[64];

extern volatile ERRMSG g_errMsg; // ������Ϣ��¼
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

#define FileIndexSize 36   //ÿ֡�ļ���Ϣ��С
#define FileIndexNamepox 4 //ÿ֡�ļ�ÿ֡�ļ������ʼλ��
#define PatFilesMAX 999    //�����ļ������ֵ  1~999

#define F_ERROR 0   // dir/file operation failed
#define F_OK 1      // dir/file operation successfull
#define F_SST32OK 2 // �������SST3201���ݳɹ�
#define DOWNLOAD_LEN (160 * 1024)
#define MAXFLASHADD 0x400 / 2
#define FLASH_VER 0x000003fc
#define W25Q1 2

#define SST_IAP_FLASADD 0x00700000 //ƫ�Ƶ�ַ
#define IAP_FLAG_ADD 0x00600000    /////////////	һ����64k�ֽ�
#define IAP_LEN_ADD 0x00610000     //4k�ֽ� ����
#define IAP_FHFLAG_ADD 0x00620000  //4k�ֽڽ���

#define FLAG_HARDREST 0x00001234
#define IAP_UPDATE_FLAG 0x12345678
#define IAP_FHFLAG 0x07654321
#pragma pack(1)
typedef struct
{
    U16 Type;      // �ͺ�
    U32 Dadd;      // ��Ƭ��������ʼ��ַ
    U32 Len;       // ����
    U16 Crc16;     // crc16
    U16 Sumchk;    // �ۼ�У��
    U16 Norchk;    // ���У��
    U32 Other;     // ����
    U32 FileInAdd; //�����ڱ��ļ��ĵ�ַ
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
                    linemaxdisplay, Err_MSG1_Sheet1, msgIndex, l1, vTestColor); //δ����ŵ�Ƭ
    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + g_ERROR_LEN * msgIndex + g_ERROR_MSG),
                    linemaxdisplay, Err_MSG1_Sheet2, msgIndex, l2, vTestColor); //�����ŵ�Ƭ
}

#define NEEDLE_UP_DW 0x02000000

#define KEY_RWHELP_EXIT 1   //1 ����
#define KEY_RWHELP_ENTER 2  //2 ȷ��
#define KEY_RWHELP_ORG 3    //3 ��ԭ��
#define KEY_RWHELP_NEEDLE 4 //4 ���
#define KEY_RWHELP_USB 5    //5 USB
#define KEY_RWHELP_SD 6     //6 SD
#define KEY_RWHELP_HERROR 7 //7 �쳣����ȷ��

#define KEY_RWHELP_MAX 8

#define RECT_RWHELP_BG 1   //1 ����
#define RECT_RWHELP_BMP1 2 //2 ͼƬ1
#define RECT_RWHELP_BMP2 3 //3 ͼƬ2
#define RECT_RWHELP_STR1 4 //4 ��������1
#define RECT_RWHELP_STR2 5 //5 ��������2

#define RECT_RWHELP_MAX 6

#define RECT_RWHELP2_BG 1          //1 ����
#define RECT_RWHELP2_TIT 2         //2 ����
#define RECT_RWHELP2_PINSUM_TIT 3  //3 �ܹ�����-����
#define RECT_RWHELP2_PINSUM_STR 4  //4 �ܹ�����-��ֵ
#define RECT_RWHELP2_PINUSE_TIT 5  //5 ��������-����
#define RECT_RWHELP2_PINUSE_STR 6  //6 ��������-��ֵ
#define RECT_RWHELP2_PATSUM_TIT 7  //7 �ܹ�����-����
#define RECT_RWHELP2_PATSUM_STR 8  //8 �ܹ�����-��ֵ
#define RECT_RWHELP2_PATUSE_TIT 9  //9 ��������-����
#define RECT_RWHELP2_PATUSE_STR 10 //10 ��������-��ֵ

#define RECT_RWHELP2_MAX 11

// note: para��0������������޸� 1���� 2:������޸� 3�������޸����޸�
void Menu_RwHelp(U16 para)
{
    //    U16 times50ms = 0xffff;

    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
                                            /****������ز���end****/
    /****��־λ��ز���begin****/
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
    U16 backuplen = 0; //ϵͳ�������ݳ���
    U8 ReSREGcount;    //SREGУ�����ʱ�ظ�����
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

    U32 ErrCode = 0;    //M���Ῠ���������
    U32 ErrCode2 = 0;   //D���Ῠ���������
    U32 ErrCode3 = 0;   //X���Ῠ���������
    U32 ErrCode4 = 0;   //Y���Ῠ���������
    U32 ErrCode5 = 0;   //����X���Ῠ���������
    U32 ErrCode6 = 0;   //����Y���Ῠ���������
    U32 ErrCode7 = 0;   //����Z���Ῠ���������
    U32 ErrCode8 = 0;   //����U���Ῠ���������
    U32 ErrCode9 = 0;   //����V���Ῠ���������
    U8 ExErrDis = 0xff; //���Ῠ����ʾ����
    U16 filecount = 0;
    U8 err_base = 93;
    const U8 err_base1 = 126; //�����������
    const U8 err_base2 = 133; //���������׼��������
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
    /****��־λ��ز���end***/

    const U8 keynum = {KEY_RWHELP_MAX};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {TYPE_X33, TYPE_Y30, KEY_X1, KEY_Y1},                                     //1 ����
        {TYPE_X34, TYPE_Y30, KEY_X1, KEY_Y1},                                    //2 ȷ��
        {TYPE_X34, TYPE_Y30, KEY_X1, KEY_Y1},                                    //3 ��ԭ��
        {TYPE_X33, TYPE_Y30, KEY_X1, KEY_Y1},                                     //4 ���
        {TYPE_SIZE24 - KEY_X1, TYPE_Y7, KEY_X1, KEY_Y1},               //5 USB
        {TYPE_SIZE24 + KEY_X1, TYPE_Y7, KEY_X1, KEY_Y1},               //6 SD
        {TYPE_X18 + SAFE_DIST6, TYPE_Y2 + SAFE_DIST6, KEY_X1, KEY_Y1}, //7 �쳣����ȷ��
    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {72, 0, 0, Color_Gray}, {0, 0, 0, 0}, {0}},  //1 ����
        {{KEY_Silver}, {0}, {71, 70, 0, Color_Gray}, {0, 0, 0, 0}, {0}}, ///2 ȷ��
        {{KEY_Silver}, {0}, {3, 0, 0, Color_Gray}, {0, 0, 0, 0}, {0}},   //3 ��ԭ��

        {{KEY_Silver}, {0}, {21, 23, 0, Color_Gray}, {0, 0, 0, 0}, {0}},     //4 ���
        {{KEY_Silver}, {0}, {251, 252, 253, Color_Gray}, {0, 0, 0, 0}, {0}}, //5 USB
        {{KEY_Silver}, {0}, {254, 255, 256, Color_Gray}, {0, 0, 0, 0}, {0}}, //6 SD
        {{KEY_Silver}, {0}, {0}, {7, 0, 0, Color_Gray}, {0}},                //7 �쳣����ȷ��

    };

    const RECTPAD rect[RECT_RWHELP_MAX] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X18, TYPE_Y2, 700, TYPE_SIZE24, {Color_Gray, Color_Black}, {Color_White}, {0}},                                        //1 ����
        {TYPE_X18 + SAFE_DIST6, TYPE_Y2 + SAFE_DIST6, TYPE_SIZE12, TYPE_SIZE12, {Color_Blue}, {Color_White}, {68}},                  //2 ͼƬ1
        {TYPE_X18 + SAFE_DIST6, TYPE_Y2 + SAFE_DIST6 * 3 + TYPE_SIZE13, TYPE_SIZE12, TYPE_SIZE12, {Color_Blue}, {Color_White}, {0}}, //3 ͼƬ2
        {TYPE_X19, TYPE_Y2 + SAFE_DIST6, 560, TYPE_SIZE13, {Color_Blue}, {Color_White}, {0}},                                        //4 ��������1
        {TYPE_X19, TYPE_Y2 + SAFE_DIST6 * 3 + TYPE_SIZE13, 560, 160, {Color_Blue}, {Color_White}, {0}},                              //5 ��������2
    };
    const RECTPAD rect2[RECT_RWHELP2_MAX] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
        {TYPE_X18, TYPE_Y2, 700, TYPE_SIZE24, {Color_Gray, Color_Black}, {Color_White}, {0}},                                     //1 ����
        {164, TYPE_Y6, 472, TYPE_SIZE7, {Color_MidSkyBlue}, {Color_Black}, {7}},                                                 //2 ����
        {164, TYPE_Y6 + TYPE_SIZE7 + SAFE_DIST5, TYPE_SIZE24, TYPE_SIZE7, {Color_KeyBlue}, {Color_White}, {8}},                   //3 �ܹ�����-����
        {164 + TYPE_SIZE24, TYPE_Y6 + TYPE_SIZE7 + SAFE_DIST5, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}},       //4 �ܹ�����-��ֵ
        {164, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 2, TYPE_SIZE24, TYPE_SIZE7, {Color_LightGray}, {Color_Black}, {10}},          //5 ��������-����
        {164 + TYPE_SIZE24, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 2, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}}, //6 ��������-��ֵ
        {164, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 3, TYPE_SIZE24, TYPE_SIZE7, {Color_KeyBlue}, {Color_White}, {12}},            //7 �ܹ�����-����
        {164 + TYPE_SIZE24, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 3, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}}, //8 �ܹ�����-��ֵ
        {164, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 4, TYPE_SIZE24, TYPE_SIZE7, {Color_LightGray}, {Color_Black}, {13}},          //9 ��������-����
        {164 + TYPE_SIZE24, TYPE_Y6 + (TYPE_SIZE7 + SAFE_DIST5) * 4, TYPE_SIZE10, TYPE_SIZE7, {Color_White}, {Color_Black}, {0}}, //10 ��������-��ֵ

    };
    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_RWHELP); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
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
        SetLcdPwm(1, P_CNR); //������
        g_light_c = P_R * 1000;
    }
    if (para == 10)
    {
        if (g_para0 == 2)//�����棬��������
        { 
            tmpcmd = C_PATUPDATE;
        }
        else
        {
            tmpcmd = C_WRITEMEMORYPAT; //���滨����������
        }
    }
    else if (para == 81)
    {
        tmpcmd = C_WRITEFLASHPAT; //���滨����U��
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

    if (para != 9) //����ʣ��洢����
    {
        DrawFPBMPNEW(rect[RECT_RWHELP_BMP1].xpis, rect[RECT_RWHELP_BMP1].ypis,
                     64, 58, MEM_BMPGIF, rect[RECT_RWHELP_BMP1].index[0]);
        g_logorun = 100;
        ViewPage(usepageno);
    }
    // else //���
    // {
    //     ViewPage(usepageno);
    // }

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
    temp_para = para;
    otemp_para = temp_para;
    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

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
            case 0: //��ʾɾ���������ļ�-ȷ�Ͻ���	
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
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 230), linemaxdisplay, Err_MSG1_Sheet1, 230, l2, vTestColor); //����ɾ���ڲ���������ķ���������?
                btn_en = 3;
                break;

            case 7: //ɾ�������廨��-�ȴ�����	(0->7)

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

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 228), linemaxdisplay, Err_MSG1_Sheet1, 228, l2, vTestColor); //�ڲ�����������ɾ������������...

                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);

                btn_en = 0;

                DREG[D_COMMAND] = C_DELMEMORYPAT; // ɾ�������ļ�
                DREG[D_CMDSTATUS] = 0;
                DREG[D_CMDPARA1] = g_selectnumber;
                f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND])); // ����ָ��
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
                else if (DREG[D_CMDSTATUS] == 100)//ִ�гɹ�
                { 
                    dat = g_selectnumber - MINFILENO;
                    i = dat * FileIndexSize;
                    dat = i + FileIndexSize;
                    for (; i < dat; i++)
                        IREG2[i] = 0xffff; //���ɾ��������������

                    g_init_flag = 1;
                    pnoindex = 0;
                    pno = poldno[0]; //������Ŀ¼
                    if (g_selectnumber != g_Fileno)
                    { //ɾ���Ļ������ǵ�ǰʹ�û���
                        Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                        RestoreSHREG(0x03);
                    }
                    else
                    { //ɾ���Ļ����ǵ�ǰʹ�û���
                        for (i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
                        {
                            if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
                            { //����ǰ������Ϊ�ڴ��б����С�Ļ���
                                g_selectdisno = i / FileIndexSize + MINFILENO;
                                g_oldselectdisno = g_selectdisno;
                                break;
                            }
                        }
                        if (i >= D_Length[INDEX2])
                        { //���ļ�ȫ��ɾ�����
                            for (i = 0; i < MAXSREG; i++)
                                SREG[i] = 0;
                            g_selectdisno = 0;
                            g_oldselectdisno = 0;
                            F_IND_NO = 0;
                            g_NoReadPatf = 1; //���������治���ض���������
                            break;
                        }

                        if (Write_Cmd(C_READMEMORYPAT, g_selectdisno, 0) == 0)
                        { // �������ļ�
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
                        f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ�����ļ�
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
                        g_NoReadPatf = 1; //���������治���ض���������
                    }
                }
                break;

            case 1:  //��ʾɾ��SD���ļ�
            case 78: //��ʾɾ��U���ļ�
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

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 211), linemaxdisplay, Err_MSG1_Sheet1, 211, l2, vTestColor); //����ɾ���ŵ�Ƭ�ϵķ���������?
                btn_en = 3;
                break; //�Ƿ�Ӵ���ɾ������

            case 2:
                ViewMsg(200);
                btn_en = 2;
                //PatF1();
                break; //���̲��ڴŵ�����

            case 3:
                ViewMsg(223);
                btn_en = 2;
                //PatF1();
                break;

            case 4:
                ViewMsg(204);
                btn_en = 2;
                //PatF1();
                break; //�������Ҳ�������

            case 5: //���ڴ滨��	ֱ�ӷ��������棬��Ϊ�ڶ�ȡ���������Ѿ������û���
                ViewMsg(229);
                if ((g_selectnumber >= MINFILENO) && (g_selectnumber <= MAXFILENO))
                {
                    g_selectdisno = g_selectnumber;
                    g_oldselectdisno = g_selectdisno;

                    if (g_NoReadPatf == 2)
                    { //���¶�ȡ��������
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
                        f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                        f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ�����ļ�
                        if (f == 0)
                        {
                            temp_para = 15;
                            dis_updata = 1;
                            break;
                        }
                        else
                        {
                            if ((DataCheckSum((U16 *)(&SREG[0]), len, ((F_FILETYPE == PAT_MIT) ? 0 : 1))) == 0)
                            { //����У��ʧ��
                                temp_para = 64;
                                dis_updata = 1;
                                break;
                            }
                        }
                    }
                }
                g_NoReadPatf = 1; //���������治���ض���������
                g_init_flag = 1;

                pnoindex = 0;
                pno = poldno[0];
                break;

            case 6:  //��SD������
            case 80: //��U�̻���
                ViewMsg(209);
                g_selectdisno = g_selectnumber;
                if (g_NoReadPatf == 2)
                { //��ȡ��������
                    if (g_disk_memory == 1)
                    {
                        if (g_MboardUSB == 1)
                        { //����USB
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
                            f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                        { //�����USB
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
                //У�黨������
                if (F_FILETYPE == PAT_MIT)
                { // �����
                    i = len >> 1;
                    checksum = 0;
                    SREG[0] = g_selectnumber;
                    for (i = 0; i < (len >> 1); i++)
                    { //����У���
                        if (i != 0x20)
                            checksum += SREG[i];
                    }
                    SREG[0x20] = checksum;
                }
                else
                { // �ֵ�
                    checksum = 0;
                    for (i = 0; i < (len >> 1); i++)
                    { //����У���
                        if (i != 0x05)
                            checksum += SREG[i];
                    }
                    SREG[0x05] = checksum;
                }

                n = (g_selectnumber - MINFILENO) * FileIndexSize;
                if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= (2 * MAXSREG)))
                { //������������ͬ��Ż���
                    temp_para = 12;
                    dis_updata = 1; //�Ƿ񸲸�ԭ�ļ�
                }
                else
                {
                    tmpcmd = C_WRITEMEMORYPAT; //���滨������
                    temp_para = 10;
                    dis_updata = 1; //д�������ݵ��ڴ�
                }
                break;

            case 8:  // SD��(1->8)
            case 79: //U��(78->79)

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

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 208), linemaxdisplay, Err_MSG1_Sheet1, 208, l2, vTestColor); //�ŵ�Ƭ����ɾ������������...

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

            case 9: //�鿴������ʣ������

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

                // DrawMFS32P(480, 156, SUMFILENO, 5, 0, Color_Black);                            //�ܹ�д������
                // DrawMFS32P(160, 212, dat1, 3, 0, Color_Black);                                 //��ʹ�������ٷֱ�
                // DrawMFS32P(480, 268, YREG[SYS_SUMFILE], 5, 0, Color_Black);                    //��ռ������
                // DrawMFS32P(480, 324, (SUMFILENO - YREG[SYS_SUMFILE]), 5, 0, Color_Black);      //ʣ���������
                // DrawMFS32P(480, 380, YREG[SYS_FSIZE], 5, 0, Color_Black);                      //������������
                // DrawMFS32P(160, 436, dat2, 3, 0, Color_Black);                                 //��ʹ�ü�����ٷֱ�
                // DrawMFS32P(480, 492, YREG[SYS_FUSEED], 5, 0, Color_Black);                     //��ʹ�ü�����
                // DrawMFS32P(480, 548, (YREG[SYS_FSIZE] - YREG[SYS_FUSEED]), 5, 0, Color_Black); //δʹ�ü����� YREG[SYS_COMEN]

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
                           SUMFILENO, 5, 0, rect2[RECT_RWHELP2_PINSUM_STR].colorT[0]); //�ܹ�д������
                DrawMFS32P(rect2[RECT_RWHELP2_PINUSE_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PINUSE_STR].ypis + SAFE_DIST1,
                           YREG[SYS_SUMFILE], 5, 0, rect2[RECT_RWHELP2_PINUSE_STR].colorT[0]); //��ռ������
                DrawMFS32P(rect2[RECT_RWHELP2_PATSUM_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PATSUM_STR].ypis + SAFE_DIST1,
                           YREG[SYS_FSIZE], 5, 0, rect2[RECT_RWHELP2_PATSUM_STR].colorT[0]); //������������
                DrawMFS32P(rect2[RECT_RWHELP2_PATUSE_STR].xpis + SAFE_DIST4,
                           rect2[RECT_RWHELP2_PATUSE_STR].ypis + SAFE_DIST1,
                           YREG[SYS_FUSEED], 5, 0, rect2[RECT_RWHELP2_PATUSE_STR].colorT[0]); //��ʹ�ü�����

                ViewPage(usepageno);
                btn_en = 2;
                break;

            case 10: //д�������ڴ�
                ViewMsg(227);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                if ((g_Funtype == 1) && (F_FILETYPE == PAT_SEW))//������ɵ��ֵܿ������
                { 
                    Bor_2deal();
                }
                CheckDrawScale(0, F_FILETYPE, F_FILELEN, 0); //����һ�·�Χ����
                len = (((U32)(F_FILELENH)) << 16) + F_FILELENL;
                f = Write_D(D_Address[PTDATA], (len >> 1), (U16 *)(&SREG[0])); //���ͻ�������
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    errorf = 0;
                    break;
                }
                f = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //����ͷ�ļ���Ϣ
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
                    g_NoReadPatf = 1; //���������治���ض���������
                    //ReadPatList(0,pno);		//�����ڴ�����
                    //g_updataIREG = 1;
                    if (tmpcmd == C_WRITEMEMORYPAT)
                    { //�����ļ�����
                        addr = (F_IND_NO - MINFILENO) * PTINDEXREDLEN;
                        IREG2[addr + PTINDEXLENADD + 0] = len;
                        IREG2[addr + PTINDEXLENADD + 1] = len >> 16;
                        IREG2[addr + PTINDEXENCADD + 0] = 0;
                        IREG2[addr + PTINDEXENCADD + 1] = 0;
                        for (i = 0; i < 32; i++)
                        {
                            IREG2[addr + PTINDEXNAMEADD + i] = HREG[32 + i];
                        }
                        if (F_FILETYPE == PAT_MIT)//����
                        { 
                            IREG2[addr + PTINDEXNAMEADD + 31] &= 0x00ff;
                            IREG2[addr + PTINDEXNAMEADD + 31] |= 0x0100;
                        }
                        else//�ֵ�
                        { 
                            IREG2[addr + PTINDEXNAMEADD + 31] &= 0x00ff;
                        }
                    }
                }
                else if (DREG[D_CMDSTATUS] == 12)
                { //ͨѶ�����ط�
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
                { //ͨѶ������3�Σ�����
                    errorf = 0;
                    temp_para = 15;
                    dis_updata = 1;
                }
                btn_en = 0;
                break;

            case 12: //��ʾ���Ǵ��ڵ��ļ�
                ViewMsg(231);
                btn_en = 3;
                break;

            case 11: //д������ SD��
            case 81: //д������ U��
                ViewMsg(207);
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);

                if ((g_Funtype == 1) && (F_FILETYPE == PAT_SEW))
                { //������ɵ��ֵܿ������
                    Bor_2deal();
                }
                //�����U��,SD��
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
                        HREG[4] = len & 0xffff; //�ļ�����
                        HREG[5] = (len >> 16) & 0xffff;
                    }

                    if (Write_PatFile(F_IND_NO, g_exwriteType) == 0)
                    {
                        errorf = 0;
                        pnoindex = 0;
                        pno = poldno[0];
                        //ReadPatList(0,pno);		//�����ڴ�����
                        //g_updataIREG = 1;
                        RestoreIREG(); //�ָ��ڴ�����
                        Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
                        RestoreSHREG(0x03);
                        break;
                    }
                    else
                        errorf++;
                }

                if (errorf >= 3)
                { //ͨѶ������3�Σ�����
                    errorf = 0;
                    temp_para = 15;
                    dis_updata = 1;
                }
                btn_en = 0;
                break;

            case 13: //ѹ��δ����
                ViewMsg(3);
                ComErrMessage = 1;
                btn_en = 3;
                DREG[D_ERRWAR] = 0x8003;
                Write_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR])); //���ʹ����������
                S_ERRORSET();
                Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS); //���ʹ����־������
                break;

            case 14: //��ǰ����δ����
                ViewMsg(22);
                btn_en = 3;
                break; 
            case 15: //ͨѶ����
                ViewMsg(23);
                btn_en = 2;
                DelayMs(500);
                i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
                if ((i) && (YREG[SYS_COMEN] == 0x5a5a))//-������ص���һ��ҳ��
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

            case 16: //����ʧ��-������ص���ǰҳ��
                ViewMsg(24);
                btn_en = 2;
                break;

            case 17: //����ԭ��
                ViewMsg(7);
                ComErrMessage = 1;
                btn_en = 2;
                //S_ERRORSET();
                //Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS); //���ʹ����������
                break;

            case 18:
                ViewMsg(25);
                btn_en = 4;
                if (P_HPK == 1)
                    btn_en += 2;
                break; //��ԭ��,�ڶ��ΰ���ԭ��

            case 19: //���յ����
                ViewMsg(26);
                btn_en = 1;
                break;

            case 20: //���곬���ӹ���Χ
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
                    { //����
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

            case 21: //  ���󾯸���Ϣ��ʾ
                wdt();
                btn_en = 1;
                err_btn_en = 1;
                tmp_error = DREG[D_ERRWAR];
                ComErrMessage = 1;

                if ((tmp_error & 0x8000) == 0)
                { //E
                    dat = tmp_error;
                    if (dat == 3)
                    { //���������

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
                    { //���Ӱ屨��
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
                            { //����X
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
                            { //����Y
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
                            { //����Z
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
                            { //����U
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
                            { //����V
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
                        { //Զ������
                            Timerms = 0;
                            btn_en = 3;
                        }
                        else
                        { //��Զ�̻���
                            U8 haspat = 0;
                            BackupSHREG(0x02);
                            Write_Cmd(C_RECEIVE_FILE, 0, 0); //����ȷ�Ͻ���Զ���ļ�ָ��
                            for (i = 0; i < RDTIMES * 8; i++)
                            {
                                f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                                if (DREG[D_CMDSTATUS])
                                    break;
                                DelayMs(WAITTIME);
                            }
                            if (DREG[D_CMDSTATUS] == 100)
                            {
                                f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                            { //�ڴ��д��ڴ˻���
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

                                //									f = Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&F_IND_NO));	// ����ǰ�����ļ���Ϣ

                                Write_Cmd(C_RECEIVE_FILE, 0, 0); //����ȷ�Ͻ���Զ���ļ�ָ��
                                for (i = 0; i < RDTIMES * 8; i++)
                                {
                                    f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                                    if (DREG[D_CMDSTATUS])
                                        break;
                                    DelayMs(WAITTIME);
                                }
                                if (DREG[D_CMDSTATUS] == 100)
                                {
                                    f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
                                    if (f)
                                    {
                                        len = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
                                        f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ�����ļ�
                                        if (f == 0)
                                        {
                                            temp_para = 15;
                                            dis_updata = 1;
                                        }
                                        else
                                        {
                                            if (Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0) == 0)
                                            { //���滨�����ڴ�
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
                                            //													Write_D(D_Address[PTDATA]+0x20,1,(U16 *)(&SREG[0x20])); // ���͵�ǰ�����ļ�У��ֵ
                                            //												}else{
                                            //													checksum = 0;
                                            //													for(i=0;i<len;i++){
                                            //														if(i!=0x05){checksum +=SREG[i];}
                                            //													}
                                            //													SREG[0x05] = checksum;
                                            //													Write_D(D_Address[PTDATA]+0x05,1,(U16 *)(&SREG[0x05])); // ���͵�ǰ�����ļ�У��ֵ
                                            //												}
                                            ReadFileIndex(C_RDMEMPATLIST, plastno); //�����ļ�����
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

            case 22: //  ������Ϣ��ʾ
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
                break; //���곬���ӹ���Χ

            case 24: //��ʽ���ڴ�
                ViewMsg(235);
                btn_en = 3;
                break;

            case 26: //��ʽ���ڴ�  24->26
                ViewMsg(disPosStr_X + 12 * 5);
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                Write_Cmd(C_FORMATMEMORY, 1, 2); // ��ʼ��
                for (i = 0; i < RDTIMES * 50; i++)
                {
                    f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                    if ((DREG[D_CMDSTATUS]))
                        break;
                    DelayMs(WAITTIME);
                }

                if (DREG[D_CMDSTATUS] == 100)
                {                                                                   //��ʽ���ɹ�
                    f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��ȡ����ֵ
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
                        n = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
                        if (n)
                        {
                            j = F_IND_NO;
                            if ((j >= MINFILENO) && (j <= MAXFILENO))
                            {
                                n = (j - MINFILENO) * FileIndexSize;
                                len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                                n = 0; // ���������ʧ�ܣ���ʼ��SREG�Ĵ���
                                if ((len > 0) && (len <= MAXSREG))
                                {
                                    n = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ��������
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
                { //��ʽ��ʧ��
                    temp_para = 28;
                    dis_updata = 1;
                    break;
                }
                break;

            case 25: //��ʽ��SD
            case 82: //��ʽ��usb
                ViewMsg(236);
                btn_en = 3;
                break;

            case 27: //��ʽ�� SD��   25->27
            case 83: //��ʽ��U�� 81->83
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
                g_disk_memory = MEM_IN; //�ڴ�
                pno = MENU_FUNCTION;
                pnoindex = 1;
                ReadFileIndex(C_RDMEMPATLIST, pno);
                break;

            case 28:                                                                                                                                 //��ʽ��ʧ��
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 29), linemaxdisplay, Err_MSG1_Sheet1, 29, l1, vTestColor); //��ʽ��ʧ��!
                if ((g_disk_memory == 0) /*||((g_disk_memory==1)&&(g_MboardUSB==1))*/)
                {                                                                                                                                                    //����洢�豸
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 29 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 29, l2, vTestColor); //����ϵͳ�豸ͨѶ��֮�������!
                }
                btn_en = 2;
                break;

            case 31: //����ѡ��������  ���ڴ渴�Ƶ�����
                     //�����
                if (ExtMemory_Status(USBTF) == 0)
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 33), linemaxdisplay, Err_MSG1_Sheet1, 33, l1, vTestColor);               //�Ƿ���Ը���ָ����ͼ�����ϵ�U��?
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 315 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 315, l2, vTestColor); //��ͬ������������ϻᱻ����������,ԭ�е����ϻ���ʧ
                    btn_en = 3;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //�޴洢�豸!
                    btn_en = 2;
                }
                break;

            case 33:                                                                                                                                   //  ���ڴ渴��ָ���ļ�������	(84->31->33)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 301), linemaxdisplay, Err_MSG1_Sheet1, 301, l1, vTestColor); //���ڴ��ڲ������帴�ƻ������ϵ����̴洢����...
                                                                                                                                                       //					Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*301+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,301,2,Color_White);	//���Ժ�
                if (P_LAN == 2)//��ʾ��
                {                                                                                                                       //Խ����
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X + 25 * 4, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
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
                    { //�˿ռ���Դ���Ϣ,ѡ�񰴼�
                        SelectNoFino[SelectNoCnt] = g_SelectNo[i].Fileno;
                        SelectNoCnt++;
                    }
                }
                if (P_LAN == 2)//�ܹ��ļ���
                {                                                            //Խ����
                    DrawMFU32Z(206, vaddr2 + 34, SelectNoCnt, 3, Color_Red); //���޸�
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, SelectNoCnt, 3, Color_Red);
                }
                //TO �����U�� SD
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
                                    result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                                    if ((DREG[D_CMDSTATUS]))
                                        break;
                                    DelayMs(WAITTIME);
                                }
                            }

                            Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                                    { //������ʽ
                                        len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                        for (i = 0; i < len; i++)
                                        { //����У���
                                            if (i != 0x20)
                                                checksum += SREG[i];
                                        }
                                        SREG[0x20] = checksum;
                                    }
                                    else
                                    { //�ֵܸ�ʽ
                                        len = ((file_meg[1] << 16) | file_meg[0]) / 2;
                                        for (i = 0; i < len; i++)
                                        { //����У���
                                            if (i != 0x05)
                                                checksum += SREG[i];
                                        }
                                        SREG[0x05] = checksum;
                                    }

                                    if (P_LAN == 2)//��ǰִ�е��ļ���
                                    { //Խ����
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
                                        Delete_PatFile(F_IND_NO);   //��ɾ��ԭ�ļ� ��û�ж�ɾ��һ��
                                        Write_PatFile(F_IND_NO, 0); //д�뻨������
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

            case 37: //����ȫ���������ⲿ������
                //�����
                if (ExtMemory_Status(USBTF) == 0)
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 35), linemaxdisplay, Err_MSG1_Sheet1, 35, l1, vTestColor);               //�Ƿ���Ը������е�ͼ�����ϵ�U��?
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 316 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 316, l2, vTestColor); //��ͬ������������ϻᱻ����������,ԭ�е����ϻ���ʧ
                    btn_en = 3;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //�޴洢�豸!
                    btn_en = 2;
                }
                break;

            case 35:                                                                                                                                   //���ڴ渴��ȫ�����ϵ�����	(84->37->35)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 301), linemaxdisplay, Err_MSG1_Sheet1, 301, l1, vTestColor); //���ڴ��ڲ������帴�ƻ������ϵ����̴洢����...
                                                                                                                                                       //					Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*301+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,301,2,Color_White);	//���Ժ�
                if (P_LAN == 2)
                {                                                                                                                       //Խ����
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X + 25 * 4, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
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
                { //Խ����
                    DrawMFU32Z(206, vaddr2 + 34, filecount, 3, Color_Red);
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, filecount, 3, Color_Red);
                }
                //if((g_MboardUSB == 1)&&(g_disk_memory == 1)){	//TO ������U��
                //	CopyFileCom(C_COPYMEMORYPAT, SelectNoFino, 100);
                //}else{	//TO �����U�� SD
                filecount = 0;
                for (n = 0; n < MAXIREG2; n += FileIndexSize)
                {
                    wdt();
                    if ((((IREG2[n + 1] << 16) | IREG2[n]) > 0) && (((IREG2[n + 1] << 16) | IREG2[n]) <= 2 * MAXSREG))
                    {
                        filecount++;
                        if (P_LAN == 2)
                        { //Խ����
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
                                result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                                if ((DREG[D_CMDSTATUS]))
                                    break;
                                DelayMs(WAITTIME);
                            }
                        }
                        if ((DREG[D_CMDSTATUS] != 100))
                        {
                            continue;
                        }

                        Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                                { //������ʽ
                                    len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                    for (i = 0; i < len; i++)
                                    { //����У���
                                        if (i != 0x20)
                                            checksum += SREG[i];
                                    }
                                    SREG[0x20] = checksum;
                                }
                                else
                                { //�ֵܸ�ʽ
                                    len = ((file_meg[1] << 16) | file_meg[0]) / 2;
                                    for (i = 0; i < len; i++)
                                    { //����У���
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
                                    Delete_PatFile(F_IND_NO);   //��ɾ��ԭ�ļ� ��û�ж�ɾ��һ��
                                    Write_PatFile(F_IND_NO, 0); //д�뻨������
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

            case 32: //�Ӵ��̸��Ƶ�ָ�����������ڴ�
                ViewMsg(315);
                btn_en = 3;
                break;

            case 34:                                                                                                                                   //�Ӵ��̸���ѡ���ļ����ڴ�	(32->34)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 300), linemaxdisplay, Err_MSG1_Sheet1, 300, l1, vTestColor); // ���ڴӴ��̴洢�����ƻ������ϵ��ڲ���������...

                if (P_LAN == 2)
                {                                                                                                                       //Խ����
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X + 25 * 4, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
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
                    { //�˿ռ���Դ���Ϣ,ѡ�񰴼�
                        SelectNoFino[SelectNoCnt] = g_SelectNo[i].Fileno;
                        SelectNoCnt++;
                    }
                }
                if (P_LAN == 2)
                { //Խ����
                    DrawMFU32Z(206, vaddr2 + 34, SelectNoCnt, 3, Color_Red);
                }
                else
                {
                    DrawMFU32Z(disPosStr_X + 25 * 5, vaddr2 + 34, SelectNoCnt, 3, Color_Red);
                }
                //if(( g_MboardUSB == 1)&&(g_disk_memory == 1)){	//From ������U��
                //	CopyFileCom(C_COPYFLASHPAT, SelectNoFino, SelectNoCnt);
                //}else{		//From �������U��/SD
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
                        { //Խ����
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
                                    result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // ���͵�ǰ�����ļ�
                                    if (result == 0)
                                    { //ͨѶ����
                                        break;
                                    }
                                    result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //���͵�ǰ�ļ��б�
                                    if (result == 0)
                                    { //ͨѶ����
                                        break;
                                    }
                                    result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //���ͱ��滨������
                                    if (result == 0)
                                    { //ͨѶ����
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
                                    { //�ɹ�
                                        break;
                                    }
                                    else if (DREG[D_CMDSTATUS] == 12)
                                    { //ͨѶ�����ط�
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
                Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO));              // ����ǰ�����ļ���Ϣ
                Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ�����ļ�

                pno = MENU_FUNCTION;
                pnoindex = 1;
                ReadFileIndex(C_RDMEMPATLIST, MENU_FUNCTION);
                break;

            case 38: //�Ӵ��̸������л������ϵ��ڴ�
                ViewMsg(316);
                btn_en = 3;
                break;

            case 36:                                                                                                                                   //�Ӵ��̸����������ϵ��ڴ�	(38->36)
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 300), linemaxdisplay, Err_MSG1_Sheet1, 300, l1, vTestColor); // ���ڴӴ��̴洢�����ƻ������ϵ��ڲ���������...
                                                                                                                                                       //					Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*300+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,300,2,Color_White);
                                                                                                                                                       //					Draw24x24Str(511,256,RwHelp_TITLE_ADD+17*RwHelpTitleLengthMax,RwHelp_Sheet,17,Color_White);		// ��  ��
                if (P_LAN == 2)
                {                                                                                                                       //Խ����
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);      //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
                }
                else
                {
                    Draw24x24Str(disPosStr_X, vaddr2, RwHelp_TITLE_ADD + 16 * RwHelpTitleLengthMax, RwHelp_Sheet, 16, vTestColor);               //���ڸ��Ƶ�  ��ͼ������
                    Draw24x24Str(disPosStr_X + 25 * 5, vaddr2 + 34, RwHelp_TITLE_ADD + 17 * RwHelpTitleLengthMax, RwHelp_Sheet, 17, vTestColor); // ��  ��
                }
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);

                //if(( g_MboardUSB == 1)&&(g_disk_memory == 1)){	//From ������U��
                //	CopyFileCom(C_COPYFLASHPAT, SelectNoFino, 100);
                //}else{		//From �������U��/SD
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
                //DrawMFU32Z(548, 256, filecount, 3, Color_Red);	//�ļ�����
                if (P_LAN == 2)
                { //Խ����
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
                        { //Խ����
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
                            result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // ���͵�ǰ�����ļ�
                            if (result == 0)
                            { //ͨѶ����
                                break;
                            }
                            result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //���͵�ǰ�ļ��б�
                            if (result == 0)
                            { //ͨѶ����
                                break;
                            }
                            result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //���ͱ��滨������
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
                            { //�ɹ�
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 12)
                            { //ͨѶ�����ط�
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
                Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO));              // ����ǰ�����ļ���Ϣ
                Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ�����ļ�

                pno = MENU_FUNCTION;
                pnoindex = 1;
                ReadFileIndex(C_RDMEMPATLIST, MENU_FUNCTION);
                break;

            case 39:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 36), linemaxdisplay, Err_MSG1_Sheet1, 36, l1, vTestColor); //�ڲ����������ļ�
                btn_en = 2;
                break;

            case 40:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //�������ļ�
                btn_en = 2;
                break;

            case 41:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 38), linemaxdisplay, Err_MSG1_Sheet1, 38, l1, vTestColor); //	�Ƿ񱸷ݹ����趨������?
                btn_en = 3;
                break; //�Ƿ񱸷��趨��?

            case 42:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 39), linemaxdisplay, Err_MSG1_Sheet1, 39, l1, vTestColor); //�Ƿ�ԭ�����趨�������?
                btn_en = 3;
                break; //�Ƿ�ԭ�趨��?

            case 43:
                ViewMsg(45);
                //���ڱ����趨��
                btn_en = 0;
                result = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��FLASH��ȡ����ֵ
                if (result)
                {
                }
                else
                    f = C_ERROR; // ����ʧ��	2
                //Write_D(D_Address[OPRA]+26,1,(U16 *)(&OPRA_DISPCMDSTATUS));

                //						SearchavailabDir();
                //						f = MemoryDrv_Opera(C_BACKUPPARA,0);       ///���޸ģ�������������������������������������������������������������������������������������������������������������������
                //
                if (f == C_SUCCEED)
                { //�ɹ�
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
                //���ڻ�ԭ�趨��

                if (f == C_SUCCEED)
                {                                                                   //�ɹ�
                    f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��ȡ����ֵ
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 47), linemaxdisplay, Err_MSG1_Sheet1, 47, l1, vTestColor); //�Ƿ񱸷ݿ��Ƴ��򵽴���?
                btn_en = 3;
                break; //�Ƿ񱸷ݿ��Ʊ�?

            case 46:
                ViewMsg(48);
                //���ڱ��ݿ��Ʊ�
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 49), linemaxdisplay, Err_MSG1_Sheet1, 49, l1, vTestColor); //�Ƿ�ԭ���Ƴ���?
                btn_en = 3;
                break; //�Ƿ�ԭ���Ʊ�?

            case 48:
                ViewMsg(78);
                //���ڻ�ԭ���Ʊ�
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 79), linemaxdisplay, Err_MSG1_Sheet1, 79, l1, vTestColor); //�Ƿ񱸷ݹ����趨��Ϳ��Ƴ��򵽴���?
                btn_en = 3;
                break; //�Ƿ񱸷��趨��Ϳ��Ƴ���?

            case 50:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 80), linemaxdisplay, Err_MSG1_Sheet1, 80, l1, vTestColor); //�Ƿ�ԭ�趨��Ϳ��Ƴ���?
                btn_en = 3;
                break;

            case 51:
                ViewMsg(81);
                //���ڱ����趨��Ϳ��Ƴ���
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                Write_Cmd(C_BACKUPPARA, 1, 2);
                DelayMs(8000);
                //for(i=0;i<RDTIMES;i++){
                //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                //	if((DREG[D_CMDSTATUS]))break;
                //	DelayMs(WAITTIME);
                //}
                Read_Status();
                if (DREG[D_CMDSTATUS] == 100)
                { //�ɹ�
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
                //���ڻ�ԭ�趨��Ϳ��Ƴ���
                btn_en = 0;
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                Write_Cmd(C_RECOVERYPARA, 1, 2);
                DelayMs(3000);
                Read_Status();
                //for(i=0;i<RDTIMES;i++){
                //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                //	if((DREG[D_CMDSTATUS]))break;
                //	DelayMs(WAITTIME);
                //}

                if (DREG[D_CMDSTATUS] == 100)
                {                                                                   //�ɹ�
                    f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��ȡ����ֵ
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
                break; //���뵵�����벻��

            case 57:
                Read_D(D_Address[SYS] + SYS_RETURN_MD, 2, (U16 *)(&YREG[SYS_RETURN_MD]));
                if ((YREG[SYS_RETURN_MD] == 0x9999) && (YREG[SYS_RETURN_Y] == 0x9999))
                {
                    Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 399 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 399, l2, vTestColor); //ʹ������������!
                }
                else
                {
                    if ((YREG[SYS_RETURN_Y] & 0x0100))
                    {                                                                                                                                                      //������
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 398 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 398, l2, vTestColor); //������Ч�ڵ�    ��  ��  ��
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 398), linemaxdisplay, Err_MSG1_Sheet1, 398, l2, vTestColor); //ʹ����Ч�ڵ�    ��  ��  ��
                    }
                    if (P_LAN) //���޸�
                    {
                        if ((YREG[SYS_RETURN_Y] & 0x0100))
                        { //������
                            temp = (U8)(YREG[SYS_RETURN_Y]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(112, vaddr1, 32, 16, bgColor);
                            DrawFU32(112, vaddr1, 2000 + temp, 4, vTestColor); //��

                            temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(152, vaddr1, 16, 16, bgColor);
                            DrawFU32(152, vaddr1, temp, 2, vTestColor); //��

                            temp = (U8)(YREG[SYS_RETURN_MD]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(176, vaddr1, 16, 16, bgColor);
                            DrawFU32(176, vaddr1, temp, 2, vTestColor); //��
                        }
                        else
                        {
                            temp = (U8)(YREG[SYS_RETURN_Y]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(232, vaddr1, 32, 16, bgColor);
                            DrawFU32(232, vaddr1, 2000 + temp, 4, vTestColor); //��

                            temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(disPosStr_X + 12 * 8, vaddr1, 16, 16, bgColor);
                            DrawFU32(disPosStr_X + 12 * 8, vaddr1, temp, 2, vTestColor); //��

                            temp = (U8)(YREG[SYS_RETURN_MD]);
                            temp = BCD_2Hex(temp);
                            DrawFillRect(296, vaddr1, 16, 16, bgColor);
                            DrawFU32(296, vaddr1, temp, 2, vTestColor); //��
                        }
                    }
                    else
                    {
                        temp = (U8)(YREG[SYS_RETURN_Y]);
                        temp = BCD_2Hex(temp);
                        DrawFillRect(168, vaddr1, 32, 16, bgColor);
                        DrawFU32(168, vaddr1, 2000 + temp, 4, vTestColor); //��

                        temp = (U8)(YREG[SYS_RETURN_MD] >> 8);
                        temp = BCD_2Hex(temp);
                        DrawFillRect(216, vaddr1, 16, 16, bgColor);
                        DrawFU32(216, vaddr1, temp, 2, vTestColor); //��

                        temp = (U8)(YREG[SYS_RETURN_MD]);
                        temp = BCD_2Hex(temp);
                        DrawFillRect(248, vaddr1, 16, 16, bgColor);
                        DrawFU32(248, vaddr1, temp, 2, vTestColor); //��
                    }
                }
                Draw24x24MulStr(disPosStr_X, vaddr1 + 28, (uint *)(USE_ERROR_MSG + ERROR_LEN * 399), linemaxdisplay, Err_MSG1_Sheet1, 399, 2, vTestColor); //	��������ɹ�!
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
                if ((g_light_c == 0) && (P_LOF == 1))// ��ͨѶ���Ͼ�����ʾ״̬��Ҫ������
                { 
                    ViewPage(usepageno);
                    SetLcdPwm(1, P_CNR); //������
                    g_light_c = P_R * 1000;
                    DelayMs(10);
                }
                ViewMsg(84);
                DelayMs(200);
                i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));

                if ((i) && (YREG[SYS_COMEN] == 0x5a5a))
                { //���ͨѶ��������ֱ���˳�
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
                break; //ͨѶ����

            case 61:                                                                                                                                 //����ϵͳ����
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 85), linemaxdisplay, Err_MSG1_Sheet1, 85, l1, vTestColor); //�Ƿ񱸷�ϵͳ���õ��ڴ�?
                btn_en = 3;
                break;

            case 62:                                                                                                                                 //�ָ��ڴ�
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 86), linemaxdisplay, Err_MSG1_Sheet1, 86, l1, vTestColor); //�Ƿ�ԭϵͳ����?
                btn_en = 3;
                break;

            case 63:                                                                                                                                 //�ָ�����
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 87), linemaxdisplay, Err_MSG1_Sheet1, 87, l1, vTestColor); //�Ƿ�ԭϵͳ��������?
                btn_en = 3;
                break;

            case 64:
                ViewMsg(88);
                btn_en = 2;
                break;

            case 65: //ͼ�γ�����Χ����,��������
                ViewMsg(89);
                btn_en = 2;
                break;

            case 66: //ָ������λ��
                ViewMsg(128);
                btn_en = 2;
                break;

            case 67:
                ViewMsg(90);
                btn_en = 2; //��ѹ�ų����г̷�Χ
                break;

            case 68:

                if (ExtMemory_Status(USBTF) == 0)
                {
                    Select_ExtMemory(USBTF);
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 91), linemaxdisplay, Err_MSG1_Sheet1, 91, l1, vTestColor); //�Ƿ�����ϵͳ																		btn_en = 3;
                    btn_en = 3;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l2, vTestColor); //�޴洢�豸
                    btn_en = 2;
                }

                break;

            case 69:
                upflag = 0;
                f = 0; // �ⲿ�洢����Ч

                j = 0;
                if (ExtMemory_Status(USB) != 0)
                {
                    f = 1;
                }

#ifdef OLD_BOOT
                if (f)
                {                                                                                                                                        // δ����
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //�޴洢�豸
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
                    { //Hϵͳ���������,���ݵ�flash
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // ���ڸ�������...
                        if (copy_dispprogram(j, 4, bgColor, 0, 1) == TRUE)
                        {
                            W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //д�����ļ���Ϣ
                            upflag = 1;
                        }
                    }

                    if ((ii & 0x80000) && (g_upWIFI))
                    { //wifi
                        errCnt = 0;
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 455), linemaxdisplay, Err_MSG1_Sheet1, 455, l1, vTestColor); //��������WIFIģ��...
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
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 93), linemaxdisplay, Err_MSG1_Sheet1, 93, 2, vTestColor); //������������...
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
                    { //����FLASH

                        errFlag = 0;
                        errCnt = 0;
                        g_logorun = 0;
                        while (errCnt < 3)
                        {
                            DrawFillRect(140, 190, 444, 96, bgColor);
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 95), linemaxdisplay, Err_MSG1_Sheet1, 95, 2, vTestColor); //�������������FLASH
                            SETWCS1();                                                                                                                          //ȡ��Ƭѡ
                            SETWCS2();                                                                                                                          //ȡ��Ƭѡ
                            SETTFCS();
                            errFlag = 0;
                            UpdataF = 0xffff;

                            if (update_flash(j, 4, bgColor, 0) == TRUE)
                            { // ����FLASH
#ifndef USESST39VFXX
                                //д�����ݺ��ض�У�飬���ɹ�����3��
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
                                    W25QXX_Write(W25Q1_DAT, (U8 *)(&UpdataF), W25QBACIFOADDR + 16, 2); //����FLASH���³ɹ���־
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
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 116), linemaxdisplay, Err_MSG1_Sheet1, 116, 2, vTestColor); //������������������ļ�...

                        if (update_mp3(j, 4, bgColor, 0) == TRUE)
                        { // ������������
                            f |= 0x04;
                        }
                    }

                    //�������ݴ����M-����ϵͳ����
                    if ((ii & 0x00000001) && (g_upDISP))
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 92), linemaxdisplay, Err_MSG1_Sheet1, 92, 2, vTestColor); // �������������...
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6);                                                                //д�����ļ���Ϣ
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
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // û�������ļ�
                        btn_en = 2;
                    }
                    else
                    {
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6); //д�����ļ���Ϣ
                        DelayMs(800);
                        if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                        {                                          // ѡ������U��
                            Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // ����������
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
                {                                                                                                                                        // δ����
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //�޴洢�豸
                    btn_en = 2;
                }
                else
                {
                    DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                    DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                    // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                    // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                    ii = AnalyseUpdateList(j, 0);

                    if ((ii & 0x00000002) && (g_upDISP)) //Hϵͳ���������,���ݵ�flash
                    {
                        DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                     rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // ���ڸ�������...
                        if (copy_dispprogram(j, 4, bgColor, 0, 1) == TRUE)
                        {
                            W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //д�����ļ���Ϣ
                            upflag = 1;
                        }
                    }

                    if ((ii & 0x80000) && (g_upWIFI))//wifi
                    { 
                        errCnt = 0;
                        // DrawFillRect(140, 190, 444, 96, bgColor);
                        DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                     rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 455), linemaxdisplay, Err_MSG1_Sheet1, 455, l1, vTestColor); //��������WIFIģ��...
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
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 93), linemaxdisplay, Err_MSG1_Sheet1, 93, 2, vTestColor); //������������...
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

                    if ((ii & 0x00000004) && (g_upFLASH))//����FLASH
                    { 

                        errFlag = 0;
                        errCnt = 0;
                        g_logorun = 0;
                        while (errCnt < 3)
                        {
                            // DrawFillRect(140, 190, 444, 96, bgColor);
                            DrawFillRect(rect[RECT_RWHELP_STR1].xpis, rect[RECT_RWHELP_STR1].ypis,
                                         rect[RECT_RWHELP_STR1].xsize, rect[RECT_RWHELP_STR1].ysize, rect[RECT_RWHELP_STR1].colorB[0]);
                            Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 95), linemaxdisplay, Err_MSG1_Sheet1, 95, 2, vTestColor); //�������������FLASH
                            SETWCS1();                                                                                                                          //ȡ��Ƭѡ
                            SETWCS2();                                                                                                                          //ȡ��Ƭѡ
                            SETTFCS();
                            errFlag = 0;

                            if (update_flash(j, 4, bgColor, 0) == TRUE)
                            { // ����FLASH
#ifndef USESST39VFXX
                                //д�����ݺ��ض�У�飬���ɹ�����3��
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
                                    g_upinfo[FL_ADDR] = '0'; //��ǲ���Ҫ����flash
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
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 116), linemaxdisplay, Err_MSG1_Sheet1, 116, 2, vTestColor); //������������������ļ�...

                        if (update_mp3(j, 4, bgColor, 0) == TRUE)
                        { // ������������
                            f |= 0x04;
                        }
                    }

                    //�������ݴ����M-����ϵͳ����
                    if ((ii & 0x00000001) && (g_upDISP))
                    {
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 92), linemaxdisplay, Err_MSG1_Sheet1, 92, 2, vTestColor); // �������������...
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QMIFOADDR + 18, 6);                                                                  //д�����ļ���Ϣ
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
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // û�������ļ�
                        btn_en = 2;
                    }
                    else
                    {
                        W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QMIFOADDR + 18, 6); //д�����ļ���Ϣ
                        creatUpInfoData(Info);
                        W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
                        DelayMs(800);
                        if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                        {                                          // ѡ������U��
                            Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // ����������
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 96), linemaxdisplay, Err_MSG1_Sheet1, 96, l1, vTestColor); //����У�����,����������!
                btn_en = 2;
                break;

            case 73:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 97), linemaxdisplay, Err_MSG1_Sheet1, 97, l1, vTestColor); //����ͼ������,������������!
                btn_en = 2;

                Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ

                m = F_IND_NO;
                if ((m >= MINFILENO) && (m <= MAXFILENO))
                {
                    n = (m - MINFILENO) * PTINDEXREDLEN + PTINDEXLENADD;
                    len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                    n = 0; // ���������ʧ�ܣ���ʼ��SREG�Ĵ���
                    if ((len >= 0x40) && (len <= MAXSREG))
                    {
                        n = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ��������
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 98), linemaxdisplay, Err_MSG1_Sheet1, 98, l1, vTestColor); //û���ҵ�����ָ��,������������!
                btn_en = 2;
                break;

            case 75: //�Ƿ�У׼������
                ViewMsg(99);
                btn_en = 3;
                break;

            case 76:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, l1, vTestColor); //�������ļ�
                btn_en = 2;
                break;

            case 77:                                                                                                                                 //���ļ�
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 36), linemaxdisplay, Err_MSG1_Sheet1, 36, l1, vTestColor); //�ڲ����������ļ�
                btn_en = 2;
                break;

            case 84: //ѡ����Ӧ�����Ĵ��̽���
            case 85:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 30), linemaxdisplay, Err_MSG1_Sheet1, 30, l1, vTestColor); //��ѡ��Ҫ�����Ĵ洢��.
                btn_en = 0;
                break;

            case 86:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 108), linemaxdisplay, Err_MSG1_Sheet1, 108, l1, vTestColor); //�Ƿ��Զ�д��������?
                btn_en = 3;                                                                                                                            //�Ƿ��Զ�д��������
                break;

            case 87:
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 108 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 108, l2, vTestColor); //����д��������,���Ժ�...
                btn_en = 0;                                                                                                                                        //д��������
                break;

            case 88:
                ViewMsg(52);
                btn_en = 1; //����ֵ̫��
                break;

            case 89:
                ViewMsg(53);
                btn_en = 1; //����ֵ̫С
                break;

            case 90: //��Զ�̻���
                ViewMsg(113);
                Read_D(D_Address[SYS] + SYS_LISTTYPE, 1, (U16 *)(&YREG[SYS_LISTTYPE])); // �������ڴ���������
                btn_en = 0;

                while (errorf < 3)
                {
                    DREG[D_COMMAND] = C_RWDWORKSHOPFILE; // �������ļ�
                    DREG[D_CMDSTATUS] = 0;
                    DREG[D_CMDPARA1] = g_array6[0]; //�ļ����
                    DREG[D_CMDPARA2] = 1;
                    DREG[D_CMDPARA3] = g_netSelectMac;                                      //ip
                    f = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // ����ָ��
                    if (f == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    f = Read1_Status(8);
                    //for(i=0;i<(RDTIMES*8);i++){
                    //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
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

                    f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                    f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ�����ļ�
                    if (f == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    else
                    {
                        if ((DataCheckSum((U16 *)(&SREG[0]), len, (F_FILETYPE == PAT_MIT ? 0 : 1)) == 0))
                        { //����У��ʧ��
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
                { //ͨѶ������3�Σ�����
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

            case 91: //дԶ���ļ�
                ViewMsg(114);
                btn_en = 0;

                m = F_IND_NO;

                DREG[D_COMMAND] = C_RWDWORKSHOPFILE; // д�����ļ�
                DREG[D_CMDSTATUS] = 0;
                DREG[D_CMDPARA1] = g_array6[0]; //�ļ����
                DREG[D_CMDPARA2] = 2;
                DREG[D_CMDPARA3] = g_netSelectMac;                                      //ip
                f = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // ����ָ��
                if (f == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }
                f = Read1_Status(8);
                //for(i=0;i<(RDTIMES*8);i++){
                //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
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
                break; //����ֵ����

            case 94:
                ViewMsg(412);
                ComErrMessage = 1;
                btn_en = 1;
                //S_ERRORSET();Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS); //���ʹ����������
                break; //��˲������λ

            case 95:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 32), linemaxdisplay, Err_MSG1_Sheet1, 32, l1, vTestColor); //�޴洢�豸!
                btn_en = 2;
                break;

            case 96:
                DrawFBMP2(100, 168, 500, 128, (U16 *)msgerr, vTestColor); //ϵͳ����ʧ�ܣ�����������
                btn_en = 3;
                break;

            case 97:
                Draw24x24Str(disPosStr_X, vaddr1, RwHelp_TITLE_ADD + 18 * RwHelpTitleLengthMax, RwHelp_Sheet, 18, vTestColor); // ID ����
                btn_en = 0;
                break;

            case 98:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_PROG_MSG7 + ERROR_LEN * 83), linemaxdisplay, Err_MSG2_Sheet1, 83, l1, vTestColor); //  ת��Ϊ�����ʽ������������8000,�޷����ת��
                btn_en = 2;
                break;

            case 99: //������쳣
                ViewMsg(8);
                btn_en = 2;
                break;

            case 100: //����뱣������
                Draw24x24Str(disPosStr_X, vaddr1, RwHelp_TITLE_ADD + 19 * RwHelpTitleLengthMax, RwHelp_Sheet, 19, vTestColor);
                btn_en = 1;
                break;

            case 101://��ѡ��ϲ���������
                Draw24x24Str(disPosStr_X, vaddr1, RwHelp_TITLE_ADD + 20 * RwHelpTitleLengthMax, RwHelp_Sheet, 20, vTestColor); 
                btn_en = 1;
                break;

            case 102: //���ܲ����ϲ�ͼ��
                ViewMsg(413);
                btn_en = 1;
                break;

            case 103://���������ǰ���ͼ����?
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 415), linemaxdisplay, Err_MSG1_Sheet1, 415, l1, vTestColor); 
                btn_en = 3;
                break;

            case 104: //�޷���ȡ���ͼ��
                ViewMsg(414);
                btn_en = 1;
                break;

            case 105://����ɾ���ڲ���������ĺϲ�ͼ����?
                //GroupType = GREG[dataadd[selectno1]+GroupPTypeAddr];
                DrawRect(400, vaddr1, 174, 40, vTestColor); //���޸�
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 416), linemaxdisplay, Err_MSG1_Sheet1, 416, l2, vTestColor); 
                btn_en = 3;
                break;

            case 106://����ɾ���ڲ���������ĺϲ�ͼ��
                DrawRect(400, vaddr1, 174, 40, vTestColor);
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 417), linemaxdisplay, Err_MSG1_Sheet1, 417, l2, vTestColor); 

                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;
                if (Write_Cmd(C_DELGROUP, g_GroupType, g_GroupDoingNO) == 0)
                { //����ɾ������ļ�ָ��
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
                { //���Ͷ�ȡ���ͼ��ָ��
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
                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //��ȡ���ͼ������

                for (i = 0; i < GroupMax; i++)
                { //�������
                    for (j = 0; j < 6; j++)
                    {
                        if (j == 0)
                        {
                            g_Combinepat[i][j] = 0xffff; //ͼ�α��
                        }
                        else
                        {
                            g_Combinepat[i][j] = 0;
                        }
                    }
                }

                //��ʾ�ص�ǰ����ļ�
                for (i = 0; i < GREG[GroupPNumAddr]; i++)
                {
                    g_Combinepat[i][0] = GREG[GroupPNoAddr + i]; //ͼ�λ������
                    for (j = 0; j < 4; j++)
                    { //ͼ�λ�������,ֻ��ʾǰ��8���ַ�
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

            case 107: //����ɾ���ŵ�Ƭ�ϵĺϲ�ͼ����?          
                                        
                DrawRect(400, vaddr1, 174, 40, vTestColor); //���޸�
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 420), linemaxdisplay, Err_MSG1_Sheet1, 420, l2, vTestColor); //����ɾ���ŵ�Ƭ�ϵĺϲ�ͼ����?
                btn_en = 3;
                break;

            case 108://����ɾ���ŵ�Ƭ�ϵĺϲ�ͼ��
                DrawRect(400, vaddr1, 174, 40, vTestColor); //���޸�
                Draw32X32FStr(410, vaddr1 + 4, (char *)g_tmpGroName, vTestColor);

                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 421), linemaxdisplay, Err_MSG1_Sheet1, 26, l2, vTestColor); 
                //Draw24x24MulStr(disPosStr_X,256,(uint *)(USE_ERROR_MSG+ERROR_LEN*421+ERROR_MSG),linemaxdisplay,Err_MSG1_Sheet2,36,2,Color_White);//���Ժ�

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

            case 109://���ƺϲ�ͼ�ε��ŵ�Ƭ��?
                ViewMsg(422);
                btn_en = 3;
                break;

            case 110:
                ViewMsg(207);
                //�����廨��д���ŵ�Ƭ��
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
                                    result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                                    if ((DREG[D_CMDSTATUS]))
                                        break;
                                    DelayMs(WAITTIME);
                                }
                            }

                            Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                                    { //������ʽ
                                        len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                        checksum = 0;

                                        for (cnt = 0; cnt < len; cnt++)
                                        { //����У���
                                            if (cnt != 0x20)
                                                checksum += SREG[cnt];
                                        }
                                        SREG[0x20] = checksum;
                                    }
                                    else
                                    { //�ֵܸ�ʽ
                                        len = ((file_meg[1] << 16) | file_meg[0]) / 2;
                                        checksum = 0;
                                        for (cnt = 0; cnt < len; cnt++)
                                        { //����У���
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
                                        Delete_PatFile(F_IND_NO); //��ɾ��ԭ�ļ� ��û�ж�ɾ��һ��
                                        Write_PatFile(F_IND_NO, 0);
                                    }
                                }
                            }
                        }
                    }
                }

                //д����ļ�
                Delete_GroupFile((U8 *)g_GroupName, g_GroupType); //��ɾ��Դ�ļ�
                Write_GroupFile((U8 *)g_GroupName, g_GroupType);

                //�����ļ���Ĭ��λ��
                if (Write_D(D_Address[GROUP], GroupPatMax, (U16 *)(&GREG[0])) == 0)
                { //���Ͳ���
                    break;
                }
                if (Write_Cmd(C_SAVEGROUP, 3, g_GroupType) == 0)
                { //���ͱ�������ļ�ָ��
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

                //���¶�ȡ�������
                if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                { //���Ͷ�ȡ���ͼ��ָ��
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
                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //��ȡ���ͼ������

                Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE])); // �������ڴ���������
                if (((YREG[SYS_GROUP_TYPE] > 0) && (YREG[SYS_GROUP_TYPE] < 41)) || (YREG[SYS_GROUP_TYPE] == 100))
                {
                    g_GroupType = (U8)GREG[GroupPTypeAddr];
                }
                else
                {
                    g_GroupType = 0;
                }

                i = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
                if (i == 0)
                {
                    temp_para = 15;
                    dis_updata = 1;
                    break;
                }

                Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ�����ļ�
                g_NoReadPatf = 1;                                                        //��� ����P1���治��Ҫ���¶�ȡ��������
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

                //д�����ļ����ڴ�
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
                                result = Write_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ���͵�ǰ�����ļ�
                                if (result == 0)
                                {                //ͨѶ����
                                    f = C_ERROR; // ����ʧ��	2
                                    break;
                                }

                                result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //���͵�ǰ�ļ��б�
                                if (result == 0)
                                { //ͨѶ����
                                    break;
                                }
                                result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //���ͱ��滨������
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
                                { //ͨѶ�����ط�
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

                //�����ڴ��ļ�
                if (Write_Cmd(C_READGROUP, 0, 0) == 0)
                { //���Ͷ�ȡ���ͼ��ָ��
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
                Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //��ȡ���ͼ������

                ReadMemorryList(MENU_FTGROUPREAD2); //�����ڴ��ļ�����

                btn_en = 0;
                g_GroupMenth = 4; //���ļ����浽Ĭ��λ��
                g_checkGroup = 0;
                pnoindex--;
                pno = MENU_FTGROUP;

                break;

            case 113: //�Ҳ���ͼ������
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 425), linemaxdisplay, Err_MSG1_Sheet1, 425, l1, vTestColor);
                btn_en = 1;
                break;

            case 114: //ͨ�Ź��ϣ������ŷ������ʺ�����
                ViewMsg(431);
                btn_en = 1;
                break;

            case 117:
                ViewMsg(418);
                btn_en = 1;
                break;

            case 118: //ʹ�����ͼ��,�������ȡ����
                ViewMsg(419);
                btn_en = 1;
                break;

            case 119: //ͼ�γ���Χ
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
                break; //�Ҳ���TF��

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

            case 133: //��ԭϵͳ����
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
                {                                                               //����
                    Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); //���޸�
                    DrawFillRect(140, 190, 444, 96, bgColor);
                    DrawFillRect(140, 304, 444, 160, bgColor);
                    if (f == 0xff)
                    {                                                                                                                                          //У�鲻�ɹ�
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 429), linemaxdisplay, Err_MSG1_Sheet1, 429, l1, vTestColor); //�ļ���ʽ����
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //�������ļ�
                    }
                    btn_en = 1;
                }
                else
                { //�ɹ�
                    Write_D(D_Address[PARA], backuplen, (U16 *)&PREG[0]);
                    f = Write_Cmd(C_UPDATEPARA, 0, 0);

                    if (f)
                    { //�ɹ�
                        if (Read_Status() == 0)
                        {
                            f = 0; //ʧ��
                        }
                        else if (DREG[D_CMDSTATUS] == 100)
                        {
                            f = 1; //�ɹ�
                        }
                        else
                        {
                            f = 0; //ʧ��
                        }
                    }
                    if (f == 0)
                    {                                                               //ͨ��ʧ��
                        Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); //���޸�
                        DrawFillRect(140, 190, 444, 96, bgColor);
                        DrawFillRect(140, 304, 444, 160, bgColor);

                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23), linemaxdisplay, Err_MSG1_Sheet1, 23, l1, vTestColor);             //ͨ��ʧ��
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 23, l2, vTestColor); //����ͨѶ���Ƿ�����������?

                        btn_en = 1;
                    }
                    else
                    {
                        gPeessVal = P_CTR;
                        if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL)
                            gPeessVal = PEESS_DEfVAL;
                        WritePessVal(gPeessVal);
                        SetLcdPwm(1, P_CNR); //����Һ�����Աȶ�
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
                {                                                    //����
                    Read_D(D_Address[IOPARA], 338, (U16 *)(&IOREG)); //��ȡIO����
                    DrawFillRect(140, 190, 444, 96, bgColor);        //���޸�
                    DrawFillRect(140, 304, 444, 160, bgColor);
                    ;
                    if (f == 0xff)
                    {                                                                                                                                          //У�鲻�ɹ�
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 429), linemaxdisplay, Err_MSG1_Sheet1, 429, l1, vTestColor); //�ļ���ʽ����
                    }
                    else
                    {
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //�������ļ�
                    }
                    btn_en = 1;
                }
                else
                { //�ɹ�
                    Write_D(D_Address[IOPARA], backuplen, (U16 *)(&IOREG[0]));
                    f = Write_Cmd(C_SAVEIO, 0, 0);

                    if (f)
                    { //�ɹ�
                        if (Read_Status() == 0)
                        {
                            f = 0; //ʧ��
                        }
                        else if (DREG[D_CMDSTATUS] == 100)
                        {
                            f = 1; //�ɹ�
                        }
                        else
                        {
                            f = 0; //ʧ��
                        }
                    }
                    if (f == 0)
                    {                                                    //ͨ��ʧ��
                        Read_D(D_Address[IOPARA], 338, (U16 *)(&IOREG)); //��ȡIO����
                        DrawFillRect(140, 190, 444, 96, bgColor);        //���޸�
                        DrawFillRect(140, 304, 444, 160, bgColor);
                        Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23), linemaxdisplay, Err_MSG1_Sheet1, 23, l1, vTestColor);
                        Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 23 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 23, l2, vTestColor); //ͨ��ʧ��

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

            case 136:                                                                                                                               //IO �����趨 �ָ�����
            case 137:                                                                                                                               //IO ����趨 �ָ�����
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 87), linemaxdisplay, 82, 2, Err_MSG1_Sheet1, vTestColor); //�Ƿ�ԭϵͳ��������?
                g_NO_ReagIOf = 1;
                btn_en = 3;
                break;

            case 139: //������ͼ�γ���Χ
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 27), linemaxdisplay, Err_MSG1_Sheet1, 27, l1, vTestColor);   //�����������!
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 444), linemaxdisplay, Err_MSG1_Sheet1, 444, l2, vTestColor); //��ѹ���г̳��������ɵ���Χ 0-8mm!
                btn_en = 1;
                break;

            case 144:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 446), linemaxdisplay, Err_MSG1_Sheet1, 446, l1, vTestColor); //ѡ��ͣ���Ϊ����/������,��֧�ֵ���任
                btn_en = 1;
                break;

            case 145:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 447), linemaxdisplay, Err_MSG1_Sheet1, 447, l1, vTestColor); //ѡ��ͣ���Ϊ����/������,��֧��Z�ַ�任
                btn_en = 1;
                break;

            case 146:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 110), linemaxdisplay, Err_MSG1_Sheet1, 110, l1, vTestColor); //���곬�����ٸ�!
                btn_en = 1;
                break;

            case 147:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 448), linemaxdisplay, Err_MSG1_Sheet1, 448, l1, vTestColor); //�����̼��쳣
                btn_en = 1;
                break;

            case 148:
                ViewMsg(25);
                btn_en = 6;
                break;

            case 149:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 449), linemaxdisplay, Err_MSG1_Sheet1, 449, l1, vTestColor); //����/������ʾ��Ϣ������.
                btn_en = 1;
                break;

            case 150:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 450), linemaxdisplay, Err_MSG1_Sheet1, 450, l1, vTestColor); //��ѡ�������豸
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 456), linemaxdisplay, Err_MSG1_Sheet1, 456, l1, vTestColor); //���յ�Զ�̻����ļ�
                Draw24X24FStr(520, vaddr1, "NO.", Color_Red);                                                                                          //���޸�
                DrawMFU32(559, 176, F_IND_NO, 3, Color_Red);
                Draw24x24MulStr(disPosStr_X, vaddr2, (uint *)(USE_ERROR_MSG + ERROR_LEN * 456 + ERROR_MSG), linemaxdisplay, Err_MSG1_Sheet2, 456, l2, vTestColor); //�Ƿ�鿴
                btn_en = 3;
                if ((DREG[D_ERRWAR]))
                {
                    S_ERRORCLR();
                    Write_Cmd(C_WRITESTATUS, YREG[SYS_STATUS], SYS_STATUS);
                }
                break;

            case 157: //����ԭ��
            case 158: //����ԭ��
            case 159: //����ԭ��
            case 160: //����ԭ��
                ViewMsg(7);
                ComErrMessage = 1;
                //S_ERRORSET();Write_Cmd(C_WRITESTATUS,YREG[SYS_STATUS],SYS_STATUS); //���ʹ����������
                btn_en = 6;
                break;

            case 161: //ͨ�Ź��ϣ����鲽����������ʺ�����
                ViewMsg(462);
                btn_en = 1;
                break;

            case 162:                                                                                                                                  //ͨ�Ź��ϣ����鲽����������ʺ�����
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 464), linemaxdisplay, Err_MSG1_Sheet1, 464, l1, vTestColor); //�Ƿ��л���H-����ϵͳ
                btn_en = 3;
                break;

            case 163: //�л���H-����ϵͳ
            case 166:
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                f = 0;
                DrawFillRect(140, 190, 444, 96, bgColor);                                                                                             //���޸�
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // ���ڸ�������...
#ifdef OLD_BOOT
                if (update1_dispprogram(4, bgColor, 1) == TRUE)
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6); //д�����ļ���Ϣ
                    f |= 0x08;
                }

                if (f == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // û�������ļ�
                    btn_en = 2;
                }
                else
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6); //д�����ļ���Ϣ
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
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //д�����ļ���Ϣ
                    f |= 0x08;
                }

                if (f == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);                                                                                             //���޸�
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // û�������ļ�
                    btn_en = 2;
                }
                else
                {
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QHIFOADDR + 18, 6); //д�����ļ���Ϣ
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 467), linemaxdisplay, Err_MSG1_Sheet1, 467, l1, vTestColor); //û��M-����ϵͳ�����ļ�,�Ƿ�����ΪH-����ϵͳ
                btn_en = 3;
                break;

            case 165:
#ifdef OLD_BOOT
                if (upflag == 0)
                {
                    DrawFillRect(140, 190, 444, 96, bgColor);
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // û�������ļ�
                    btn_en = 2;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // ���ڸ�������...
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QBACIFOADDR + 18, 6);                                                                  //д�����ļ���Ϣ
                    DelayMs(800);
                    if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                    {                                          // ѡ������U��
                        Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // ����������
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
                    DrawFillRect(140, 190, 444, 96, bgColor);                                                                                             //���޸�
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 107), linemaxdisplay, Err_MSG1_Sheet1, 107, 2, vTestColor); // û�������ļ�
                    btn_en = 2;
                }
                else
                {
                    Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 463), linemaxdisplay, Err_MSG1_Sheet1, 463, 2, vTestColor); // ���ڸ�������...
                    W25QXX_Write(W25Q1_DAT, (U8 *)(&g_ProInfo), W25QMIFOADDR + 18, 6);                                                                    //д�����ļ���Ϣ
                    creatUpInfoData(Info);
                    W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
                    DelayMs(800);
                    if (((g_disk_memory == 1) && (g_MboardUSB == 1)) || (((g_upMBOARD == 0) && (g_upWIFI == 1))))
                    {                                          // ѡ������U��
                        Write_Cmd1(C_MCPURESTART, 0x5a, 0xa5); // ����������
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
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (18) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 18, l1, vTestColor);                    //����
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (18) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 18, l2, vTestColor); //�˳�����ļ��Ѵ���,�Ƿ����ִ��?
                btn_en = 3;
                break;

            case 168:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (15) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 15, l1, vTestColor);                    //����
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (15) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 15, l2, vTestColor); //�ļ�������Ϊ��
                btn_en = 3;
                break;

            case 169:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (19) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 19, l1, vTestColor);                    //������ʧ��
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (19) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 19, l2, vTestColor); //��ǰĿ¼�Ѵ��ڴ������ļ�!
                btn_en = 2;
                break;

            case 170:
                //��ʾɾ���������ļ�
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (2) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 2, l1, vTestColor);                    //ɾ��
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (2) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 2, l2, vTestColor); //ȷ��Ҫɾ�����ļ�/�ļ�����?
                btn_en = 3;
                break;

            case 171:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (8) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 8, l1, vTestColor);                    //ɾ��
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (8) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 8, l2, vTestColor); //����ɾ���ļ�,���Ե�
                btn_en = 0;
                files = getFiles((U16 *)g_editfiles, MAXFILENO);

                setBufDat16((U16 *)g_selfiles, MAXFILENO, 0xffff);

                if (g_disk_memory == MEM_IN)
                { //���� �ڴ洢���ʹ����һ��ɾ��
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
                    f = ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]); //��ȡ�ļ�����
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
                Draw24x24MulStr(disPosStr_X, vaddr1, Toast_TITLE_ADD + (3) * ToastTitleLengthMax, linemaxdisplay, Toast_Sheet, 3, l1, vTestColor); //���к�ճ�����ļ�����ͬ.
                btn_en = 2;
                break;

            case 173: //ճ��
            case 174: //ճ��
                files = getFiles((U16 *)g_editfiles, MAXFILENO);
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (10) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 10, l1, vTestColor);                    //ճ��
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (10) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 10, l2, vTestColor); //ȷ��Ҫ�����Ѵ��ڵ��ļ�/�ļ�����
                btn_en = 3;
                break;

            case 175: //����
            case 176: //����
            case 177: //����
                DrawFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, keybmpind[1].icon[3]);
                DrawFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize, keybmpind[2].icon[3]);
                // BmpFillRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize);
                // BmpFillRect(keyd[2].xpis, keyd[2].ypis, keyd[2].xsize, keyd[2].ysize);
                btn_en = 0;

                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (11) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 11, l1, vTestColor); //����
                files = getFiles((U16 *)g_editfiles, MAXFILENO);
                initFunctionBtn(temp_para == 176 ? 1 : 0);
                setBufDat16((U16 *)g_selfiles, MAXFILENO, 0xffff);
                initMyProgress1(files, bgColor);

                if (g_from_memory == MEM_IN)
                { //����Դ���ڴ�
                    if (g_to_memory != MEM_IN)
                    {
                        if (temp_para != 176)
                        {
                            f = ReadFileIndex(C_RDMEMPATLIST, poldno[pnoindex - 1]); //�ȶ����ڴ��ļ�����
                            if (f == 0)
                            {
                                temp_para = 15;
                                dis_updata = 1;
                                break;
                            }
                        }

                        for (i = 0; i < files; i++)
                        {
                            DrawFillRect(140, vaddr2, 444, 56, bgColor); //���޸�
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
                                    result = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // ��״̬
                                    if ((DREG[D_CMDSTATUS]))
                                        break;
                                    DelayMs(WAITTIME);
                                }
                            }

                            Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
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
                                    { //������ʽ
                                        len = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
                                        for (j = 0; j < len; j++)
                                        { //����У���
                                            if (j != 0x20)
                                                checksum += SREG[j];
                                        }
                                        SREG[0x20] = checksum;
                                    }
                                    else
                                    { //�ֵܸ�ʽ
                                        len = (F_FILELEN) / 2;
                                        for (j = 0; j < len; j++)
                                        { //����У���
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
                                        { //����,�ļ�·��ΪĬ��
                                            writeDiskPatFile((char *)defFilePart, fileNo);
                                        }
                                        else
                                        {
                                            writeDiskPatFile(g_path, fileNo);
                                        }
                                        if (temp_para == 176)
                                        { //����//ɾ���ڴ���ԭ���ļ�
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
                { //����Դ���ⲿ�洢��
                    flag = 0;
                    if (ExtMemory_Status(USBTF) == 0)
                    {
                        flag = 1;
                        Select_ExtMemory(USBTF);
                    }
                    if (flag == 0)
                        break;

                    if (g_from_memory == g_to_memory && strEqual(g_fromPath, g_path))
                    { //����Դ�ͱ���·����ͬ
                        btn_en = 0;
                        SetMyProgress1(files);
                        pnoindex--;
                        pno = poldno[pnoindex];
                        break;
                    }
                    getDiskFiles(g_fromPath, (U16 *)IREG2, GET_ALLFILE); //��ȡ�ļ�Դ����
                    //#if Debug
                    //printf("---files = %d \r\n",files);
                    //#endif

                    for (i = 0; i < files; i++)
                    {
                        DrawFillRect(140, vaddr2, 444, 56, bgColor); //���޸�
                        SetMyProgress1(i);
                        DrawMFU32Z(528 - 13 * (2 * getNumDigits(files) + 1), vaddr2, i + 1, getNumDigits(files), vTestColor);
                        DrawMFU32Z(528 - 13 * getNumDigits(files), vaddr2, files, getNumDigits(files), vTestColor);
                        Draw24X24FStr(528 - 13 * (getNumDigits(files) + 1), vaddr2, "/", vTestColor);
                        fileNo = getFilesNo((U16 *)g_editfiles, MAXFILENO, i);
                        fileNo1 = getFilesNo1((U16 *)IREG2, fileNo);
                        fileType = getFileTypeByIREG((U16 *)IREG2, fileNo);
                        fname = getFileNameByIREG((U16 *)IREG2, fileNo);
                        if (fname) //��warming
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
                        { //���������ļ�
                            if (g_to_memory == MEM_IN)
                                continue; //�ڴ治����,�����������

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
                        { //�����ļ����ڴ�
                            ReSREGcount = 0;
                            while (ReSREGcount < 3)
                            {
                                if (readDiskPatFileByName(g_fromPath, (char *)(&IREG2[(fileNo - MINFILENO) * FileIndexSize + FileIndexNamepox]), (U16 *)SREG) != 0)
                                { //��ȡ����ʧ��
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
                                result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // ���͵�ǰ�����ļ�
                                if (result == 0)
                                { //ͨѶ����
                                    break;
                                }
                                result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //���͵�ǰ�ļ��б�
                                if (result == 0)
                                { //ͨѶ����
                                    break;
                                }
                                result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //���ͱ��滨������
                                if (result == 0)
                                { //ͨѶ����
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
                                { //�ɹ�
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] == 12)
                                { //ͨѶ�����ط�
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
                        { //����
                            deleteDiskFile(g_fromPath, (char *)disName);
                        }
                    }
                    SetMyProgress1(files);
                }

                //���¸��ƺ���ļ�����
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
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (14) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 14, l1, vTestColor);                    //����
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (14) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 14, l2, vTestColor); //ȷ��Ҫ�����Ѵ��ڵ��ļ�/�ļ�����
                btn_en = 3;
                break;

            case 179:
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (21) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 21, l1, vTestColor);                    //����
                Draw24x24MulStr(disPosStr_X, vaddr2, Msg3_TITLE_ADD + (21) * Msg3TitleLengthMax + Msg3TitleOffest, linemaxdisplay, MSG3_Sheet1, 21, l2, vTestColor); //ȷ��Ҫ�����Ѵ��ڵ��ļ�/�ļ�����
                btn_en = 3;

                break;

            case 180:                                                                                                                             //����
                Draw24x24MulStr(disPosStr_X, vaddr1, Msg3_TITLE_ADD + (11) * Msg3TitleLengthMax, linemaxdisplay, MSG3_Sheet, 11, l1, vTestColor); //����
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

                getDiskFiles(g_fromPath, (U16 *)IREG2, GET_ALLFILE); //��ȡ�ļ�Դ����

                for (i = 0; i < files; i++)
                {
                    DrawFillRect(140, vaddr2, 444, 56, bgColor); //���޸�
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
                        g_to_memory = MEM_IN; //���뵽�ڴ�

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
                        g_to_memory = MEM_IN; //���뵽�ڴ�

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
                    {             //���������ļ�
                        continue; //�����ļ�������
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
                    { //�����ļ����ڴ�
                        ReSREGcount = 0;
                        while (ReSREGcount < 3)
                        {
                            if (readDiskPatFileByName(g_fromPath, (char *)(&IREG2[(fileNo - MINFILENO) * FileIndexSize + FileIndexNamepox]), (U16 *)SREG) != 0)
                            { //��ȡ����ʧ��
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
                            result = Write_D(D_Address[PTDATA], len / 2, (U16 *)(&SREG[0])); // ���͵�ǰ�����ļ�
                            if (result == 0)
                            { //ͨѶ����
                                break;
                            }
                            result = Write_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //���͵�ǰ�ļ��б�
                            if (result == 0)
                            { //ͨѶ����
                                break;
                            }
                            result = Write_Cmd(C_WRITEMEMORYPAT, F_IND_NO, 0); //���ͱ��滨������
                            if (result == 0)
                            { //ͨѶ����
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
                            { //�ɹ�
                                break;
                            }
                            else if (DREG[D_CMDSTATUS] == 12)
                            { //ͨѶ�����ط�
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

                //���¸��ƺ���ļ�����
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
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 37), linemaxdisplay, Err_MSG1_Sheet1, 37, l1, vTestColor); //�������ļ�
                btn_en = 2;
                //PatF1();
                break;

            case 183:
                ViewMsg(470);
                btn_en = 3;
                break;

            case 184:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 471), linemaxdisplay, Err_MSG1_Sheet1, 471, l1, vTestColor); //"�й��ܶ���ִ����,��رպ����˳�",
                btn_en = 2;
                //PatF1();
                break;

            case 185:
                Draw24x24MulStr(disPosStr_X, vaddr1, (uint *)(USE_ERROR_MSG + ERROR_LEN * 472), linemaxdisplay, Err_MSG1_Sheet1, 472, l1, vTestColor); //"�й��ܶ���ִ����,��رջ򰴸�λ�������˳�",
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
                //if(btn_en==10){	//��˲������λ��
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

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
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
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            //���������������ܵİ���
            if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata >= USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����

            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
            {
                keyon = keydata;
                if (keyon == KEY_RWHELP_HERROR) //�ް�����Ч
                {
                }
                //�ǳ��水��-//������ɫ���ְ���
                else //���水��
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }

                Times = 0;
                delay = KEYDT; //������������
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
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
                    if (g_keyBellT) //����ʱ����������ʱ��
                        if (keyon != KEY_RWHELP_HERROR)
                            Beep_On(g_keyBellT);
                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }
                //���г����������ܰ����Ĺ���
                Times = 0; //��ǰ
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {

            switch (keyoff)
            {
            case KEY_RWHELP_EXIT:
                if (temp_para == 12)
                {
                    tmpcmd = C_PATUPDATE; //���»�������
                    temp_para = 10;
                    dis_updata = 1;
                }
                else if ((temp_para == 13) || (temp_para == 17) || (temp_para == 60))
                { //ѹ��
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
                { //�Ӵ��̸��ƻ������ڴ�
                    ReadFileIndex(C_RDMEMPATLIST, MENU_FTCOPY);
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if ((temp_para == 141) || (temp_para == 142))
                { //�Ӵ��̸��ƻ������ڴ�
                    for (i = 0; i < MAXSREG; i++)
                    {
                        SREG[i] = 0;
                    }
                    Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ��������
                    pno = plastno;
                }
                else if (temp_para == 156)
                { //�ָ�ԭ����������
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
                    pno = poldno[pnoindex]; //�ص�������
                }
                else if (temp_para == 164)
                {
                    temp_para = 165;
                    dis_updata = 1;
                }
                else if (temp_para == 183)
                { //����ϵͳ
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
                { //ɾ���ڴ滨��
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
                { //ɾ��U�̻���
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
                { //ɾ��SD������
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
                            tmpcmd = C_WRITEMEMORYPAT; //���滨��������������
                            temp_para = 10;
                        }
                        else if (g_disk_memory == 1)
                        {
                            //if(g_MboardUSB==1){		//����U��
                            //	tmpcmd = C_WRITEFLASHPAT;	//���滨����U��
                            //}else{
                            Select_ExtMemory(USBTF);
                            Delete_PatFile(F_IND_NO);
                            //}
                            temp_para = 81;
                        }
                        else if (g_disk_memory == 2)
                        { //���滨����SD��
                            //	Select_ExtMemory(SD);
                            //	Delete_PatFile(F_IND_NO);
                            temp_para = 11;
                        }
                    }
                    else
                    {
                        tmpcmd = C_WRITEMEMORYPAT; //���滨������
                        temp_para = 10;
                    }
                    g_para0 = 0;
                    dis_updata = 1;
                }
                else if (temp_para == 13)
                { //ѹ��
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

                        f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ

                        Write_Cmd(C_RECEIVE_FILE, 0, 0); //����ȷ�Ͻ���Զ���ļ�ָ��
                        Read1_Status(8);
                        //for(i=0;i<RDTIMES*8;i++){
                        //	f = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                        //	if(DREG[D_CMDSTATUS])break;
                        //	DelayMs(WAITTIME);
                        //}
                        if (DREG[D_CMDSTATUS] == 100)
                        {
                            f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
                            if (f)
                            {
                                len = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
                                f = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ�����ļ�
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
                                    //											//Write_D(D_Address[PTDATA]+0x20,1,(U16 *)(&SREG[0x20])); // ���͵�ǰ�����ļ�У��ֵ
                                    //										}else{
                                    //											checksum = 0;
                                    //											for(i=0;i<len;i++){
                                    //												if(i!=0x05){checksum +=SREG[i];}
                                    //											}
                                    //											SREG[0x05] = checksum;
                                    //											//Write_D(D_Address[PTDATA]+0x05,1,(U16 *)(&SREG[0x05])); // ���͵�ǰ�����ļ�У��ֵ
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
                    { //Զ������Timerms
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
                { //ȷ����ʽ���ڴ�
                    temp_para = 26;
                    dis_updata = 1;
                }
                else if (temp_para == 25)
                { //ȷ����ʽ��sd
                    temp_para = 27;
                    dis_updata = 1;
                }
                else if (temp_para == 82)
                { //ȷ����ʽ��usb
                    temp_para = 83;
                    dis_updata = 1;
                }
                else if (temp_para == 31)
                { // ���ڴ渴�����ϵ�����
                    temp_para = 33;
                    dis_updata = 1;
                }
                else if (temp_para == 32)
                { // �Ӵ��̸������ϵ��ڴ�
                    temp_para = 34;
                    dis_updata = 1;
                }
                else if (temp_para == 37)
                { // ���ڴ渴���������ϵ�����
                    temp_para = 35;
                    dis_updata = 1;
                }
                else if (temp_para == 38)
                { // �Ӵ��̸����������ϵ��ڴ�
                    temp_para = 36;
                    dis_updata = 1;
                }
                else if (temp_para == 41)
                { //�����趨������
                    temp_para = 43;
                    dis_updata = 1;
                }
                else if (temp_para == 42)
                { //��ԭ�趨��
                    temp_para = 44;
                    dis_updata = 1;
                }
                else if (temp_para == 45)
                { //���ݿ��Ʊ�����
                    temp_para = 46;
                    dis_updata = 1;
                }
                else if (temp_para == 47)
                { //��ԭ���ݿ��Ʊ�
                    temp_para = 48;
                    dis_updata = 1;
                }
                else if (temp_para == 49)
                { //��������
                    temp_para = 51;
                    dis_updata = 1;
                }
                else if (temp_para == 50)
                { //��ԭ����
                    temp_para = 52;
                    dis_updata = 1;
                }
                else if (temp_para == 55)
                {
                    Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
                    m = F_IND_NO;
                    if ((m >= MINFILENO) && (m <= MAXFILENO))
                    {
                        n = (m - MINFILENO) * FileIndexSize;
                        len = (((U32)(IREG2[n + 1]) << 16) + IREG2[n]) / 2;
                        n = 0; // ���������ʧ�ܣ���ʼ��SREG�Ĵ���
                        if ((len >= 0x40) && (len <= MAXSREG))
                        {
                            n = Read_D(D_Address[PTDATA], len, (U16 *)(&SREG[0])); // ����ǰ��������
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
                { //����ϵͳ�������ڴ�
                    Write_Cmd(C_SYSRECOVER, 5, 6);
                    Read_Status();
                    //for(i=0;i<RDTIMES;i++){
                    //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
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
                { //��ԭ����ϵͳ����
                    Write_Cmd(C_SYSRECOVER, 3, 4);
                    Read_Status();
                    //for(i=0;i<RDTIMES;i++){
                    //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                    //	if((DREG[D_CMDSTATUS]))break;
                    //	DelayMs(WAITTIME);
                    //}
                    if ((DREG[D_CMDSTATUS]) == 100)
                    {
                        f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��ȡ����ֵ
                        if (f)
                        {
                        }
                        //����Һ�����Աȶ�
                        SetLcdPwm(1, P_CNR);
                        SaveBackLight(P_CNR);
                        //							DelayMs(1000);
                        ///WriteData( BACKLIGHT_VAL_ADDR, 4,(U32 *)(&P_CNR) );	//Ҫ��Ҫ��Ҫ��
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
                { //������ԭ
                    Write_Cmd(C_SYSRECOVER, 1, 2);
                    if (Read_Status() == 0)
                    {
                        temp_para = 15;
                        dis_updata = 1;
                        break;
                    }
                    if ((DREG[D_CMDSTATUS]) == 100)
                    {
                        f = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��ȡ����ֵ
                        if (f)
                        {
                        }
                        //����Һ�����Աȶ�
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
                else if (temp_para == 68)//����ϵͳ
                { 
                    temp_para = 69;
                    dis_updata = 1;
                }
                else if (temp_para == 75)
                {
                    //	pno = MENU_CHKTOUCH;	//���봥��У׼ҳ��
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
                { //����ϲ�ͼ��
                    for (i = 0; i < GroupMax; i++)
                    { //�������
                        for (j = 0; j < 6; j++)
                        {
                            if (j == 0)
                            {
                                g_Combinepat[i][j] = 0xffff; //ͼ�α��
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
                    g_GroupMenth = 3; //��������ǰ����ļ�
                    g_Groupsel1 = 0;  //�ص���һ��ѡ��
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if (temp_para == 105)//ɾ��������ϲ�ͼ��
                { 
                    temp_para = 106;
                    dis_updata = 1;
                }
                else if (temp_para == 107)//ɾ���ŵ�Ƭ�Ϻϲ�ͼ��
                { 
                    temp_para = 108;
                    dis_updata = 1;
                }
                else if (temp_para == 109)//д�ϲ�ͼ�ε�����
                { 
                    pnoindex--;
                    pno = MENU_FTGROUP;
                }
                else if (temp_para == 111)//�����̺ϲ�ͼ��
                { 
                    temp_para = 112;
                    dis_updata = 1;
                }
                else if (temp_para == 126)//����ϵͳ������U��
                { 
                    temp_para = 128;
                    dis_updata = 1;
                    g_disk_memory = 1;
                }
                else if (temp_para == 127)//����ϵͳ������SD��
                { 
                    temp_para = 128;
                    dis_updata = 1;
                    g_disk_memory = 2;
                }
                else if (temp_para == 129)
                { //����IO�趨������U��
                    temp_para = 131;
                    dis_updata = 1;
                    g_disk_memory = 1;
                }
                else if (temp_para == 130)
                { //����IO�趨������SD��
                    temp_para = 131;
                    dis_updata = 1;
                    g_disk_memory = 2;
                }
                else if (temp_para == 132)
                { //��ԭϵͳ����
                    temp_para = 133;
                    dis_updata = 1;
                }
                else if (temp_para == 134)
                { //��ԭIO�趨����
                    temp_para = 135;
                    dis_updata = 1;
                }
                else if (temp_para == 136)
                { //��ԭIO �����趨����
                    if (Write_Cmd(C_REINPUTSETTING, 0, 0) == 0)
                    { //���ͻ�ԭ�趨ָ��
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
                    g_NO_ReagIOf = 0; //�ض�IO����
                    pnoindex--;
                    pno = poldno[pnoindex];
                }
                else if (temp_para == 137)
                { //��ԭIO ����趨����
                    if (Write_Cmd(C_REOUTPUTSETTING, 0, 0) == 0)
                    { //���ͻ�ԭ�趨ָ��
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
                    g_NO_ReagIOf = 0; //�ض�IO����
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
                { //���ݼ�¼��Ϣ��U��
                    temp_para = 153;
                    dis_updata = 1;
                    g_disk_memory = 1;
                }
                else if (temp_para == 152)
                { //���ݼ�¼��Ϣ��SD��
                    temp_para = 153;
                    dis_updata = 1;
                    g_disk_memory = 2;
                }
                else if (temp_para == 162)
                { //�л���H-����ϵͳ
                    temp_para = 163;
                    dis_updata = 1;
                }
                else if (temp_para == 164)
                {
                    temp_para = 166;
                    dis_updata = 1;
                }
                else if (temp_para == 167)
                { //������
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
                        //+----	 ɾ����ǰ��Ż���---------
                        DREG[D_COMMAND] = C_DELMEMORYPAT; // ɾ������
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
                        //�����ļ�����
                        addr = (g_patd.fno - MINFILENO) * PTINDEXREDLEN;
                        for (i = 0; i < FileIndexSize; i++)
                        {
                            IREG2[addr + i] = 0xffff;
                        }

                        //----- �����±�Ż���
                        g_patd.fno = F_IND_NO;
                        f = Write_D(D_Address[PTDATA], (g_patd.pat_len >> 1), (U16 *)(&SREG[0])); //���ͻ�������
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
                        //�����ļ�����
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
                { //�ļ�����-ɾ��
                    temp_para = 171;
                    dis_updata = 1;
                }
                else if (temp_para == 173)
                { //����ճ��
                    temp_para = 175;
                    dis_updata = 1;
                }
                else if (temp_para == 174)
                { //����ճ��
                    temp_para = 176;
                    dis_updata = 1;
                }
                else if (temp_para == 178)
                { //����
                    temp_para = 177;
                    dis_updata = 1;
                }
                else if (temp_para == 179)
                { //����
                    temp_para = 180;
                    dis_updata = 1;
                    break;
                }
                else if (temp_para == 183)
                { //����ϵͳ
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
                            g_IsLocal = 1; //���ʹ�ÿ��ٶ�λ����
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
                //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
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

            case KEY_RWHELP_USB: //ѡ��U�̴洢�豸
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
                { // ϵͳ���� 85
                    temp_para = 68;
                }
                dis_updata = 1;
                break;
            case KEY_RWHELP_SD: //ѡ��SD���洢�豸
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
                { // ϵͳ���� 85
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
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {

            if (keyon == KEY_RWHELP_HERROR) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else //���水��
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
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[��ʱ1s��Ϣ����*begin]**********************************************************
        if (Timerms > 10000)
        {
            if ((temp_para == 21) && (dat == 391)) //10s �޲����Զ�Զ������
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
        // /[��ʱ1s��Ϣ����*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************
        if (Timer_ms > 300)
        {
            Timer_ms = 0;
            if ((ComErrMessage == 1) && (temp_para == 60))
            {
                i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
                if ((i) && (YREG[SYS_COMEN] == 0x5a5a))//���ͨѶ��������ֱ���˳�
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
                            if (dat1 || dat2) //��warming
                            {
                            }
                            DrawMFS32P(rect2[RECT_RWHELP2_PINSUM_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PINSUM_STR].ypis + SAFE_DIST1,
                                       SUMFILENO, 5, 0, rect2[RECT_RWHELP2_PINSUM_STR].colorT[0]); //�ܹ�д������
                            DrawMFS32P(rect2[RECT_RWHELP2_PINUSE_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PINUSE_STR].ypis + SAFE_DIST1,
                                       YREG[SYS_SUMFILE], 5, 0, rect2[RECT_RWHELP2_PINUSE_STR].colorT[0]); //��ռ������
                            DrawMFS32P(rect2[RECT_RWHELP2_PATSUM_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PATSUM_STR].ypis + SAFE_DIST1,
                                       YREG[SYS_FSIZE], 5, 0, rect2[RECT_RWHELP2_PATSUM_STR].colorT[0]); //������������
                            DrawMFS32P(rect2[RECT_RWHELP2_PATUSE_STR].xpis + SAFE_DIST4,
                                       rect2[RECT_RWHELP2_PATUSE_STR].ypis + SAFE_DIST1,
                                       YREG[SYS_FUSEED], 5, 0, rect2[RECT_RWHELP2_PATUSE_STR].colorT[0]); //��ʹ�ü�����

                            // DrawMFS32P(480, 156, SUMFILENO, 5, 0, Color_Black); //�ܹ�д������
                            // // DrawMFS32P(160, 212, dat1, 3, 0, Color_Black);                                 //��ʹ�������ٷֱ�
                            // DrawMFS32P(480, 268, YREG[SYS_SUMFILE], 5, 0, Color_Black); //��ռ������
                            // // DrawMFS32P(480, 324, (SUMFILENO - YREG[SYS_SUMFILE]), 5, 0, Color_Black);      //ʣ���������

                            // DrawMFS32P(480, 380, YREG[SYS_FSIZE], 5, 0, Color_Black); //������������
                            // // DrawMFS32P(160, 436, dat2, 3, 0, Color_Black);                                 //��ʹ�ü�����ٷֱ�
                            // DrawMFS32P(480, 492, YREG[SYS_FUSEED], 5, 0, Color_Black); //��ʹ�ü�����
                            // // DrawMFS32P(480, 548, (YREG[SYS_FSIZE] - YREG[SYS_FUSEED]), 5, 0, Color_Black); //δʹ�ü�����
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

        // /[��ʾ�����������*begin]**********************************************************
        // if (disflag)
        // {
        //     ViewPage(usepageno);
        //     disflag = 0;
        // }
        // /[��ʾ�����������*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_RWHELP)
        {
            plastno = MENU_RWHELP;
            break;
        }
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}
