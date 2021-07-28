//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  menu_p.c									*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.74					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "menu_p.h"
#include "menu_modify.h"
#include "menu.h"
#include "touch.h"
#include "title.h"
#include "utility.h"
#include "register.h"
#include "main.h"
#include "function.h"
#include "uart.h"
#include "color.h"
#include "communication.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"
#include "disk_oper.h"
#include "tfcard.h"
#include "upflash.h"
#include "w25qxx.h"
#include "spi.h"
#include "exfunction.h"
#include "viewPattern.h"
#include "ds2460.h"

extern U16 SheetPage; //�������Sheet��ҳ�룬��0��ʼ
extern volatile U16 g_limtdang;
extern volatile U8 g_uperr;
extern volatile U8 g_GroupName[9];
extern volatile U8 g_GroupType;
// extern volatile U8 chCodeNow; //����
extern volatile U16 g_oldselectdisno;
extern volatile U16 g_selectdisno;
extern volatile S16 g_temp_PfootOffVal;
extern volatile U8 coldstart; //��������������־
extern volatile U8 g_prochange;
extern volatile U16 g_line_sum;
extern volatile U32 g_feedc;
extern volatile U32 g_feedsum;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern volatile U8 g_Funtype;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos;
extern volatile S16 g_pf_absdis;
extern volatile U16 g_temp_clr_flag;
extern volatile U8 g_code_mode;
extern volatile U8 g_editflag;
extern volatile U16 g_temp_draw_p;
extern volatile U16 g_temp_line_p;
extern volatile U32 g_temp_pat_p;
extern volatile S16 g_dxpos, g_dypos; //x,y����仯ֵ
extern volatile S16 g_tdxpos, g_tdypos;
extern volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //x,y����ʵ��ֵ
extern volatile S16 g_disxpos, g_disypos;						  //x,y����ʵ��ֵ
extern volatile U8 g_feed_sew;
extern volatile U8 g_DoTrim;
extern volatile U8 gEnverify; //ʹ�ܼ���У�鹦��
extern volatile U32 gverTim, gverTimes;
extern volatile U8 gvererr;
extern volatile U8 g_backSREG_f; // ����SERG���ݱ�־
extern volatile U8 g_backHREG_f; // ����HERG���ݱ�־
extern volatile U32 g_backSREGlen;
extern volatile U8 g_jogonflag;
extern volatile U8 g_jogmode;
extern volatile U8 g_jogmethod;
extern volatile U8 g_jogfunction;
extern volatile U32 g_jognumber;
extern volatile U32 g_recv_netdata;
extern volatile U8 g_home_2flag;				// �ڶ�ԭ���־
extern volatile S16 g_home_2xpos, g_home_2ypos; // �ڶ�ԭ��λ������
extern volatile U16 g_point_num;
extern volatile U8 g_feed_flag;
extern volatile U32 g_ct_status_flag;
extern volatile U32 g_CtPatLen;
extern volatile U8 g_oldspeed;
extern volatile U32 g_Ctendaddr, g_Ctstaddr;
extern volatile U16 g_savetimes;
extern volatile U16 g_Tsavetimes;
extern volatile U16 g_saveflag;
extern volatile S16 g_pf_absval;
extern volatile S8 g_pf_dis;
extern volatile U8 g_lineforceValSetNow;
extern volatile U16 g_Ctsavetimes;
extern volatile U32 g_jog_point;
extern volatile U8 g_reorigin;
extern volatile U8 g_originf;
extern volatile U16 g_splsum;
extern volatile U8 g_convert_orgf;
extern volatile U16 g_array4[386][2];
extern volatile U8 g_ImgMove;
extern volatile U8 g_BorSTflag;
extern volatile U32 g_max_feedsum;
extern volatile U32 g_min_feedsum;
extern volatile U8 g_UdZhouFlag;
extern volatile U8 g_ExCode;
extern volatile U8 g_curExCode;
extern volatile U8 g_backIREG_f;
extern volatile U32 g_modaddr[30];
extern volatile U8 g_jogfunction1;
extern volatile U8 g_needtohome;
extern volatile U8 g_GreatPatType;
extern volatile U16 g_curPinAng;
extern volatile U16 g_curSetPara1;
extern volatile U16 g_curSetPara2;
extern volatile U16 g_pointPinAng;
extern volatile U16 g_pointSetPara1;
extern volatile U16 g_pointSetPara2;
extern volatile U8 g_BarcodeFlag[MAXFILENO - MINFILENO];
extern volatile U8 g_patname[64];
extern volatile U8 buttonflag[6]; // �����趨�����Ƿ���ʾ��־
extern volatile U16 *g_CodeNameAdd;
extern volatile U8 g_MboardUSB;		//����U�̴洢�������־ 0��δ����  1������
extern volatile U8 g_NoReadPatf;	//����ȡ������־  1-����ȡ�������� 2-ǿ���ض�һ�λ�������
extern volatile U8 g_NoUpdataoldno; //	1-�����¾ɻ������
extern SystemTime g_SysTimes;
extern volatile U8 g_disk_memory;
extern volatile U8 g_modifyf;
extern volatile U8 g_modifyflag;
extern volatile U16 g_Fileno; //�洢��������Ļ����ļ��ţ�
extern volatile U8 g_init_flag;
extern volatile U16 g_patd_fno; //ѡ�еĻ������
extern volatile U8 g_DelAll_flag1;
extern volatile U8 g_enctTackCode;
extern volatile U8 g_fbstep;
extern volatile U8 g_fbstep1;
extern volatile U8 g_fbztt;
extern U8 OutputExcode1(U8 excode, U16 tmppo, U8 swi);
//�޸� begin
extern volatile U8 g_readcovnflag1;
extern volatile U8 g_modifyflag1;
extern volatile U8 g_fbrunflag;
extern volatile U8 g_upinfo[0x20];

extern volatile U8 g_disFileMethod; //��ʾ�ļ���ʽ 0-�б�ģʽ 1-ͼ��ģʽ
extern volatile U16 g_SelFileNo;
extern volatile U16 g_selfiles[MAXFILENO];	//ѡ���ļ�
extern volatile U16 g_editfiles[MAXFILENO]; //�༭�ļ�
extern volatile U16 g_btndisable;			//���ð���
extern volatile U8 g_checkMode;				//��ѡ/��ѡģʽ
extern volatile U8 g_selALL;				//ȫѡ
extern volatile U8 g_from_memory;
extern volatile U8 g_to_memory;
extern volatile U8 g_clipboard; //���а�
extern volatile U8 g_savePathTimes;
extern char *g_path;
extern char *g_fromPath;
extern char *g_savePath;

extern volatile U8 g_SysErr;

extern volatile S16 g_modcordx;
extern volatile S16 g_modcordy;
extern volatile S16 g_modcordflag;

extern volatile U8 g_CodeEn;

extern volatile U16 g_curPTtype;
extern volatile U8 g_zhouDown;
extern volatile S16 g_modinpx;
extern volatile S16 g_modinpy;
extern volatile S16 g_modcordx;
extern volatile S16 g_modcordy;
extern volatile S16 g_modcordflag;
extern volatile S16 g_modinpoint;

//�޸� end
volatile U8 g_ImageDis = 0;
volatile U8 g_NoEndCode;
volatile U8 g_readH = 0;	 //��ȡ������־λ
volatile U16 g_selectnumber; //ѡ�����
volatile U8 g_save_ptype;
volatile U32 g_save_len;
volatile U8 g_ReadPatternFlag; //�������SD����U�̻�����Ϣ��־   1��ʹ�ô����
volatile U8 g_orgsub_f = 0;	   // ԭ�㲹����־,7��ʹ�� 6��ʧ��
//volatile S32 g_scale = 100;			// ͼ�����ű���
volatile U16 g_tmp_PfootValSet; // �ݴ���ѹ������ֵ
volatile S8 pf_dis_modify;		//��ѹ�����޸�ģ����޸�ֵ
volatile U8 pf_modify_mode = 0; //��ѹ�����޸�ģ����޸ı�־λ
volatile U8 g_updataIREG = 0;	//��ȡ���»����б��־
volatile U8 g_distmpName[64];	//��ǰ��������
volatile U8 g_disppicflag;		//��ʾ��־λ
volatile U8 g_namenumberflag;
volatile U8 g_patnumber[4]; //���뻨���������
volatile U8 g_patnameindex;
volatile U8 g_patnumberindex;
volatile U8 g_savemodify;
saveINFO g_savePatInfo;
volatile U8 g_uppattern_flash; //���´���� �ĵ�ǰ�������ݱ�־λ
volatile U8 g_enwritef;
volatile U8 g_exwriteType;
volatile U8 g_IsLocal = 0;	//���ʹ�ÿ��ٶ�λ����
volatile U8 g_errLocal = 0; //ͼ����ʾ�����쳣��־
volatile U8 g_ntohome = 0;
volatile U8 g_endtype = 0; //����������� 1-��� 2-�޸� 3-�任

extern volatile U32 FLASHADD_P;

U8 fun_backPattingData(U32 tmp_pno);

#define TLREADD_READ 0
#define TLRELEN_READ 24 //0
#define TLREADD_BAR (TLREADD_READ + TLRELEN_READ)
#define TLRELEN_BAR 11 //1
#define TLREADD_DIRREAD (TLREADD_BAR + TLRELEN_BAR)
#define TLRELEN_DIRREAD 16 //2

//---------------P1--------------------
#define KEY_P1_SCRNSVR 1  //����
#define KEY_P1_JOGUP 2	  //���� ǰһ��
#define KEY_P1_JOGDN 3	  //���� ��һ��
#define KEY_P1_F1 4		  //��һ������
#define KEY_P1_F2 5		  //��һ������
#define KEY_P1_F3 6		  //��ǰ������� ��ȷ�ϻ���  1
#define KEY_P1_F4 7		  //��ǰ������� ��ȷ�ϻ���  2
#define KEY_P1_F5 8		  //��ǰ������� ��ȷ�ϻ���  3
#define KEY_P1_F6 9		  //��ǰ������� ��ȷ�ϻ���  4
#define KEY_P1_F7 10	  //��ǰ������� ��ȷ�ϻ���  5
#define KEY_P1_CUT 11	  //����
#define KEY_P1_UP 12	  //����
#define KEY_P1_DOWN 13	  //����
#define KEY_P1_PFOOT 14	  //��ѹ���½�
#define KEY_P1_MENU 15	  //menu
#define KEY_P1_ORG 16	  //�ص�ԭ��
#define KEY_P1_SPFOOT 17  //��ѹ���趨
#define KEY_P1_POS 18	  //�붨λ����
#define KEY_P1_SLFORCE 19 //����
#define KEY_P1_FIXTURE 20 //�о߼�
#define KEY_P1_FBAG 21	  //�۴�
#define KEY_P1_NULL 22	  //Ԥ������λ��
#define KEY_P1_NEXT 23	  //��һҳ
#define KEY_P1_LED 24	  //��ͷ��
#define KEY_P1_PINUP 25	  //���½�����
#define KEY_P1_WIND 26	  //����
#define KEY_P1_THRED 27	  //����
#define KEY_P1_VIEW 28	  //��ʾͼ��
#define KEY_P1_IMG 29	  //ͼ���϶���  ��ʾ���ڡ�Ҫ��Ϊͼ����ʾ����
#define KEY_P1_OTHER1 30  //���ð��������ڸ����ڴ�

#define KEY_P1_MAX 31

#define RECT_P1_NAME_STR 1 //���Ƶİ׵׿�
#define RECT_P1_NUM_STR 2  //����İ׵׿�
#define RECT_P1_NAME_TIT 3 //���Ƶĺڵ׿�
#define RECT_P1_NUM_TIT 4  //����ĺڵ׿�
#define RECT_P1_IMG 5	   //������ʾ���ڣ���
#define RECT_P1_SPEED 6	   //�ٶ���ʾ���ڣ���
#define RECT_P1_PIN 7	   //��ǰ��������ʾ���ڣ���
#define RECT_P1_PROGRESS 8 //���ҽ����������
#define RECT_P1_GROUP 9	   //�����Ϣ����
#define RECT_P1_PAGE 10	   //ҳ��

#define RECT_P1_MAX 11

//--------------------------
#define INKEY_P1_BASE 36

#define KEY_P1_TOL INKEY_P1_BASE + K_TOL
#define KEY_P1_TOR INKEY_P1_BASE + K_TOR
#define KEY_P1_TOU INKEY_P1_BASE + K_TOU
#define KEY_P1_TOD INKEY_P1_BASE + K_TOD
#define KEY_P1_IMENU INKEY_P1_BASE + K_MENU

#define g_GroupTol YREG[SYS_GROUP_NUMS] //��ϻ������ӻ�����
#define g_GroupNO YREG[SYS_GROUP_NO]	//��ϻ����ĵ�ǰ�ӻ������

//------------------------------------------
// Function   : void Menu_P1(U8 tt)
// Description: ����׼Ŀ¼
//------------------------------------------
void Menu_P1(void)
{

	U32 i, j;
	U8 isHome, f /*,vflag=0*/;
	U16 times50ms = 0xffff;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****������ţ��������ƣ������б���ز���begin****/
	// U16 static tmpFno;																//��ǰ������ϢHREG[0]�ļ�λ��(�����),������Ż���
	U16 tmpselectdisno = g_selectdisno; //ѡ�еĻ�����Ż���
	U16 temp_feed = 0xffff;				//���ڻ������� ��ǰ������������
	// U16 temp_feed0 = 0xffff;														//���ڻ������� ��һ��ʱ����������������
	U16 tmpS_2HOME = 0, oldtmpS_2HOME = 0;												 //ϵͳ״̬����-�ص�ԭ��
	U16 tmp_s_2hd = (U16)(S_2HD);														 //��ԭ��λ�û���
	U8 pattern_flash = 0, data_flash = 1, jog_flash = 1;								 //pattern_flash����ͼ�����Ƹ��±�־λ ��data_flash�������ݸ��±�־λ  ��jog_flash�������ݸ��±�־λ
	U8 bflag = 0;																		 //0���������й����ߣ�1���ػ泬�����ֵ�������� 2���ػ��˻ص��ߣ�3���������й���������ߣ�
	U8 tmpGroupNO = g_GroupNO, lastGroupNO = g_GroupNO;									 //��ǰ���ģʽ��ţ���һ�����ģʽ���
	tydPatternList PatternList = {1, 0, 0, 5, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}, {0}}; //�����б����
	/****������ţ��������ƣ������б���ز���end****/

	/****������ʾ��ز���begin****/
	const S16 ORG_X = CONVAS_ORG_PX1, ORG_Y = CONVAS_ORG_PY1; //������ʾ�����Ļ������ĵ㣨LCDʵ�����꣩ ԭORG_X = 325, ORG_Y = 262
	S16 xo = ORG_X, yo = ORG_Y;								  //��������Դ�����ĵ㻨���������껺��
	// S16 xorg = ORG_X, yorg = ORG_Y;									  //����ͼ������ĵ����� ת���� ��Ļ�����ĵ�����
	S32 /*tscale = 1, tscale1 = 1,*/ Scale = 0; //tscale��ͼ��������ű��ʣ�tscale1�����յ������űȣ�Scale���ɵ����ű���
	/****������ʾ��ز���end****/

	/****��־λ��ز���begin****/
	//U8 threadflag = 0;   //����ģʽ��־  0������   1���������봩��ģʽ  2�����ڴ���ģʽ
	U8 tozero = 1; //��ԭ���־λ
	// U8 cutFlag = 0;		  //���߱�־
	U16 pfootf = 0xffff;  //ѹ�������½���־λ
	U16 needlef = 0xffff; //�������½�����
	U16 ledflag = 0xffff; //��ͷ�ƻ���
	U16 fixdat = 0;		  //�о߼����ݻ���
	// U8 fixtureFlag = 1;	  //�о�ͼ����±�־λ
	//	U8 enLF	=0;		//���߰�����־
	U8 enFBAG = 1;			//0-��ʹ��KEY_P1_FBAG ����  //�۴�����
	U16 fbmode = 0;			//�۴�ģʽ��Ϣ
	U16 fbstep, ofbstep;	//�۴�������Ϣ
	U8 speedFlag;			//�ٶ�������ʾ��־(1����λ/0��ʵ���ٶȣ�
	U16 tmp_speed = 0xffff; //tmp_speed �ٶȻ���,���ϵͳ�����뵱ǰ�Ƿ����
	/****��־λ��ز���end***/

	/****ϵͳ��Դ������������begin****/
	U8 errFlag = 0;		  //�쳣��־
	U8 errCnt = 0;		  //�쳣��������
	U16 checkbuff[12];	  //�ļ�У�黺��
	U32 maxadd;			  //����ַ
	U16 *checkadd = NULL; //У���ַָ��
	U32 listlen = 0;	  //�ļ�Ŀ¼����
	U16 tmpchecksum = 0;  //У���ָ��
	U32 *padd = NULL;
	U16 *dadd;
	U16 datasum, tmpdatasum;
	U32 l, cnti, cntj;
	U16 /*tmpcmd =0,*/ checksum = 0;
	U8 Info[0x20];
	/****ϵͳ��Դ������������end****/

	const U8 keynum = {KEY_P1_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},									   //1 ����
		{TYPE_X1, 117, KEY_X1, KEY_Y1},						   //2 ���� ǰһ��
		{TYPE_X1, 117 + TYPE_SIZE16 + KEY_Y1, KEY_X1, KEY_Y1}, //3 ���� ��һ��

		{656, TYPE_Y1 + 48 * 5, KEY_X2, KEY_Y2},					 //4 ��һ������
		{SAFE_BORDER_R1 - KEY_X2, TYPE_Y1 + 48 * 5, KEY_X2, KEY_Y2}, //5 ��һ������
		{TYPE_X10, TYPE_Y1, 136, TYPE_SIZE7},						 //6 ��ǰ������� ��ȷ�ϻ���  1
		{TYPE_X10, TYPE_Y1 + 48, 136, TYPE_SIZE7},					 //7 ��ǰ������� ��ȷ�ϻ���  2
		{TYPE_X10, TYPE_Y1 + 48 * 2, 136, TYPE_SIZE7},				 //8 ��ǰ������� ��ȷ�ϻ���  3
		{TYPE_X10, TYPE_Y1 + 48 * 3, 136, TYPE_SIZE7},				 //9 ��ǰ������� ��ȷ�ϻ���  4
		{TYPE_X10, TYPE_Y1 + 48 * 4, 136, TYPE_SIZE7},				 //10 ��ǰ������� ��ȷ�ϻ���  5

		{TYPE_X9, TYPE_Y1, KEY_X1, KEY_Y1},					  //11 ����
		{TYPE_X9, 124, KEY_X1, KEY_Y1},						  //12 ����
		{TYPE_X9, 124 + KEY_X1 + TYPE_SIZE5, KEY_X1, KEY_Y1}, //13 ����
		{TYPE_X9, 306, KEY_X1, KEY_Y1},						  //14 ��ѹ���½�

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1}, //15 menu
		{TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1}, //16 �ص�ԭ��
		{TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1}, //17 ��ѹ���趨
		{TYPE_X4, TYPE_Y4, KEY_X1, KEY_Y1}, //18 �붨λ����
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1}, //19 ����
		{TYPE_X6, TYPE_Y4, KEY_X1, KEY_Y1}, //20 �о߼�
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1}, //21 �۴�  //����ʵ����ʾ��Χ42600��25600
		{TYPE_X8, TYPE_Y4, KEY_X1, KEY_Y1}, //22 Ԥ������λ��
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1}, //23 ��һҳ

		{TYPE_X10, TYPE_Y5, KEY_X1, KEY_Y1},			//24 ��ͷ��
		{TYPE_X11, TYPE_Y5, KEY_X1, KEY_Y1},			//25 ���½�����
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},			//26 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},			//27 ����
		{CONVAS_PX1, CONVAS_PY1, CONVAS_X1, CONVAS_Y1}, //28 ��ʾͼ��
		{CONVAS_PX1, CONVAS_PY1, CONVAS_X1, CONVAS_Y1}, //29 ͼ���϶���  ��ʾ���ڡ�Ҫ��Ϊͼ����ʾ����
		{TYPE_X1, TYPE_Y1, KEY_X1, KEY_Y1},				//30 ���ð��������ڸ����ڴ�
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},   //1 ����
		{{KEY_Silver}, {0}, {191}, {5, 0, 0, 0}, {0}}, //2 ���� ǰһ��
		{{KEY_Silver}, {0}, {193}, {6, 0, 0, 0}, {0}}, //3 ���� ��һ��

		{{KEY_Silver}, {0}, {203}, {35, 0, 0, 0}, {0}},																	 //4 ��һ������
		{{KEY_Silver}, {0}, {205}, {36, 0, 0, 0}, {0}},																	 //5 ��һ������
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {9, 0, 0, 0}, {0}}, //6 ��ǰ������� ��ȷ�ϻ���  1
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {9, 0, 0, 0}, {0}}, //7 ��ǰ������� ��ȷ�ϻ���  2
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {9, 0, 0, 0}, {0}}, //8 ��ǰ������� ��ȷ�ϻ���  3
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {9, 0, 0, 0}, {0}}, //9 ��ǰ������� ��ȷ�ϻ���  4
		{{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {9, 0, 0, 0}, {0}}, //10 ��ǰ������� ��ȷ�ϻ���  5

		{{KEY_Silver}, {0}, {34}, {10, 0, 0, 0}, {0}},			 //11 ����
		{{KEY_Silver}, {0}, {41}, {11, 0, 0, 0}, {0}},			 //12 ����
		{{KEY_Silver}, {0}, {42}, {12, 0, 0, 0}, {0}},			 //13 ����
		{{KEY_Silver}, {0}, {17, 19}, {13, 14, 0, 0}, {0}},		 //14 ��ѹ���½�
		{{KEY_Silver}, {0}, {1}, {15, 0, 0, 0}, {0}},			 //15 menu
		{{KEY_Silver}, {0}, {3}, {16, 0, 0, 0}, {0}},			 //16 �ص�ԭ��
		{{KEY_Silver}, {0}, {14}, {17, 0, 0, 0}, {0}},			 //17 ��ѹ���趨
		{{KEY_Silver}, {0}, {37}, {18, 0, 0, 0}, {0}},			 //18 �붨λ����
		{{KEY_Silver}, {0}, {28}, {19, 0, 0, 0}, {0}},			 //19 ����
		{{KEY_Silver}, {0}, {25, 26, 27}, {20, 22, 21, 0}, {0}}, //20 �о߼�
		{{KEY_Silver}, {0}, {45}, {23, 0, 0, 0}, {0}},			 //21 �۴�  //����ʵ����ʾ��Χ42600��25600
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},			 //22 Ԥ������λ��
		{{KEY_Silver}, {0}, {39}, {26, 0, 0, 0}, {0}},			 //23 ��������

		{{KEY_Silver, KEY_Blue}, {0}, {9, 5}, {27, 28, 0, 0}, {0}}, //24 ��ͷ��
		{{KEY_Silver}, {0}, {21, 23}, {29, 30, 0, 0}, {0}},			//25 ���½�����
		{{KEY_Silver}, {0}, {36}, {31, 0, 0, 0}, {0}},				//26 ����
		{{KEY_Silver}, {0}, {35}, {32, 0, 0, 0}, {0}},				//27 ����
		{{KEY_Silver}, {0}, {0}, {33, 0, 0, 0}, {0}},				//28 ��ʾͼ��
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},				//29 ͼ���϶���  ��ʾ���ڡ�Ҫ��Ϊͼ����ʾ����
		{{KEY_Silver}, {0}, {0}, {34, 0, 0, 0}, {0}},				//30 ���ð��������ڸ����ڴ�
	};
	const RECTPAD rect[] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
		{TYPE_X2, TYPE_Y1, TYPE_SIZE10, TYPE_SIZE5, {Color_Black}, {Color_White}, {1}},									//1 ���Ƶİ׵׿�
		{424, TYPE_Y1, TYPE_SIZE10, TYPE_SIZE5, {Color_Black}, {Color_White}, {2}},										//2 ����İ׵׿�
		{154, TYPE_Y1, 268, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},												//3 ���Ƶĺڵ׿�
		{498, TYPE_Y1, TYPE_SIZE10, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},										//4 ����ĺڵ׿�
		{CONVAS_PX1, CONVAS_PY1, CONVAS_X1, CONVAS_Y1, {Color_Black}, {Color_White}, {0}},								//5 ������ʾ���ڣ���
		{TYPE_X9, 124 + KEY_X1, KEY_X1, TYPE_SIZE5, {Color_Black}, {Color_White}, {0}},									//6 �ٶ���ʾ���ڣ���
		{TYPE_X1, 117 + KEY_X1, KEY_X1, TYPE_SIZE16, {Color_Black}, {Color_White}, {0}},								//7 ��ǰ��������ʾ���ڣ���
		{TYPE_X2, 377, CONVAS_X1, TYPE_SIZE4, {Color_Orange}, {Color_White}, {Color_Black}},							//8 ���ҽ����������
		{TITLE_XPOS_GROUP, SAFE_DIST4, TIT_TIME_XPOS - TITLE_XPOS_GROUP, TYPE_SIZE4, {COLOR_TITLE_BG}, {Color_Red}, {39}}, //9 �����Ϣ���ޣ�����
		{TYPE_X10 + KEY_X2 + 1, TYPE_Y1 + 48 * 5, 38, KEY_Y2, {Color_White}, {Color_Black}, {0}},						//10 ҳ��׵ף�����
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_P1); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();
	pat_resetPatRule();
	pat_setPatRule(PATRULES_P1);
	pnoindex = 0;

	if (P_LAN)
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME_E;
	}
	else
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME;
	}
	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************

	Read_D(D_Address[SYS] + SYS_IOCONCT, 1, (U16 *)(&YREG[SYS_IOCONCT]));
	speedFlag = (YREG[SYS_IOCONCT] & 0x20) ? 1 : 0;

	/************�۴��������***************/
	//��ȡ�û�����_�ж��Ƿ�Ϊ�۴���
	Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 1, (U16 *)(&YREG[SYS_MACHINE_TYPE]));
	enFBAG = YREG[SYS_MACHINE_TYPE] == 10040 ? 1 : 0;

	Read_D(FGPARA_ADDR + 23, 1, &fbstep); //��ȡ��ǰ�۴�����

	//	enFBAG=0; //�޸�
	//fbstep = 0;
	ofbstep = fbstep;
	if (ofbstep) //��waring
	{
	}
	Read_D(FGPARA_ADDR, 1, &fbmode); //��ȡ��ǰ�۴�ģʽ
	if (fbmode > 4)
		fbmode = 0;

	//	//enFBAG = 1;
	//	if(enFBAG){
	//		//DrawButtonSys4(keyd[KEY_P1_FBAG],KEY_Silver1,29);
	//		DrawButton(keyd[KEY_P1_FBAG],(U16 *)(STARTWLBMPADD),KEY_Silver,29);//�۴�����
	////		SetLanguage(MENU_FUNCTION);
	////		DrawFMultiLStrR(592, 105,MEM_TITLE_ADD+TitleLengthMax*(632+fbmode), 16,1,Color_Black);
	////		DrawFMultiLStrR(592, 80,MEM_TITLE_ADD+TitleLengthMax*(TLFNADD_OTHER2+16+fbstep), 40,1,Color_Black);
	////		//DrawFMultiLStr(keyd[KEY_P1_FBAG].xpis+8, keyd[KEY_P1_FBAG].ypis+28,MEM_TITLE_ADD+TitleLengthMax*(632+fbmode), 6,2,Color_Red);
	////		SetLanguage(MENU_P1);
	//	}

	Read_D(D_Address[SYS] + SYS_UPCOUNT, 4, (U16 *)(&YREG[SYS_UPCOUNT]));
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //P1
	//��������
	for (i = 1; i < RECT_P1_MAX; i++)
	{
		if (i == RECT_P1_PROGRESS)
		{
			DrawProgressbar(rect[i].xpis, rect[i].ypis,
							rect[i].xsize, rect[i].ysize,
							g_feedct * 100 / g_patd.feed_sum, rect[i].colorB[0], rect[i].colorT[0]);
			DrawFU32(TYPE_X2 + CONVAS_X1 / 2, 377 + SAFE_DIST4, g_feedct * 100 / g_patd.feed_sum, 3, rect[i].index[0]);
			DrawF16Str(TYPE_X2 + CONVAS_X1 / 2 + 8 * 3, 377 + SAFE_DIST4, "%", rect[i].index[0]);
		}
		else if (i == RECT_P1_GROUP) //���ģʽ_�Ӻ�ģʽ��Ϣ
		{
			if (g_GroupType == 1 || g_GroupType == 2)
			{ //�����
				Draw24x24Str(rect[i].xpis, rect[i].ypis, MEM_TITLE_ADD + ((U32)rect[i].index[0] + g_GroupType) * TitleLengthMax,
							 SheetPage, rect[i].index[0] + g_GroupType, rect[i].colorT[0]); //���ģʽ���Ӻ�ģʽ

				Draw24X24FStr(rect[i].xpis + 12 * 9, rect[i].ypis, (char *)g_GroupName, rect[i].colorT[0]);
				DrawMFU32(rect[i].xpis + 12 * 18, rect[i].ypis, g_GroupNO, 2, rect[i].colorT[0]);
				Draw24X24FStr(rect[i].xpis + 12 * 18 + 12 * 2, rect[i].ypis, "/", rect[i].colorT[0]);
				DrawMFU32(rect[i].xpis + 12 * 18 + 12 * 3, rect[i].ypis, g_GroupTol, 2, rect[i].colorT[0]);
			}
		}
		else if (i == RECT_P1_NAME_STR || i == RECT_P1_NUM_STR) //���ƺ���
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
			Draw24x24Str(rect[i].xpis + (rect[i].xsize - 24 * 2) / 2,
						 rect[i].ypis + (rect[i].ysize - 24) / 2,
						 MEM_TITLE_ADD + ((U32)rect[i].index[0]) * TitleLengthMax,
						 SheetPage, rect[i].index[0], rect[i].colorT[0]);
		}
		else
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		}
	}
	//����������ʾ
	for (i = 2; i < KEY_P1_MAX; i++)
	{
		//	if((i==KEY_P1_SLFORCE&&enLF==0)||i==KEY_P1_SPFOOT ){}
		if (i == KEY_P1_NULL || (i == KEY_P1_FBAG && !enFBAG) || i == KEY_P1_FIXTURE) //��Ч����
		{
			continue;
		}
		else if (i == KEY_P1_IMG || i == KEY_P1_VIEW) //����Ҫ��ʾ�İ���
		{
			continue;
		}
		else if (i == KEY_P1_F3 || i == KEY_P1_F4 || i == KEY_P1_F5 || i == KEY_P1_F6 || i == KEY_P1_F7) //�ǳ��水ť
		{
			DrawButtonEdit(keyd[i], keybmpind[i].icon[0], keybmpind[i].icon[0]);
		}
		else //���水ť
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

	ViewPage(usepageno);									//������ʾ����
	E_RUNDIS();												//��ֹ����
	Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE); //��д״̬�Ĵ��� д�뾲ֹ״̬

#if 0
	if (g_Fileno != F_IND_NO)
	{
		U32 wIndexTemp = (g_Fileno - MINFILENO) * FileIndexSize;
		if ((((IREG2[wIndexTemp + 1] << 16) | IREG2[i]) <= 0) ||
			(((IREG2[wIndexTemp + 1] << 16) | IREG2[wIndexTemp]) > 2 * MAXSREG))
		{
			wIndexTemp = g_oldselectdisno;
			if ((((IREG2[wIndexTemp + 1] << 16) | IREG2[i]) <= 0) ||
				(((IREG2[wIndexTemp + 1] << 16) | IREG2[wIndexTemp]) > 2 * MAXSREG))
			{
				for (i = 0; i < MAXIREG2; i += FileIndexSize) //���������洢�ռ䣬��ȡ ���ϻ�����������ǰ�������
				{
					if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
						(((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
					{
						g_Fileno = i / FileIndexSize + MINFILENO;
						break;
					}
				}
			}
		}

		g_disk_memory = 0;
		fun_getPatData(g_Fileno, g_disk_memory, MENU_P1);
		g_selectdisno = g_Fileno;
		g_oldselectdisno = g_Fileno;
		tmpselectdisno = g_Fileno;
	}
	else
	{
		g_disk_memory = 0;
		fun_getPatData(F_IND_NO, g_disk_memory, MENU_P1);
		//��ȡ������Ϣ������begin
		Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
		g_readH = 0;

		// tmpFno = g_selectdisno;
		// if ((tmpFno != F_IND_NO) && (g_NoReadPatf != 1))
		// {
		// 	tmpFno = F_IND_NO;
		// 	Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ��������
		// 																			 // g_uppattern_flash = 1;
		// }

		// if ((tmpselectdisno != F_IND_NO) && (g_NoReadPatf != 1))
		// {
		// 	tmpselectdisno = F_IND_NO;
		// 	Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ��������
		// }
		if ((tmpselectdisno != F_IND_NO) || (g_NoReadPatf != 1))
		{
			tmpselectdisno = F_IND_NO;
			Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ��������
		}

		g_NoReadPatf = 0;
		g_selectdisno = g_Fileno;
		g_oldselectdisno = g_Fileno;
		tmpselectdisno = g_Fileno;
	}
#else
	g_disk_memory = 0;
	fun_getPatData(F_IND_NO, g_disk_memory, MENU_P1);
	//��ȡ������Ϣ������begin
	Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
	g_readH = 0;
	if ((tmpselectdisno != g_Fileno) || (g_NoReadPatf != 1))
	{
		tmpselectdisno = F_IND_NO;
		Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ��������
	}
	g_NoReadPatf = 0;
	g_Fileno = F_IND_NO;
	g_selectdisno = F_IND_NO;
	g_oldselectdisno = F_IND_NO;
	tmpselectdisno = F_IND_NO;
	// if (g_uppattern_flash == 1)
	// {
	// 	g_uppattern_flash = 0;
	// 	Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ��������
	// }
	//��ȡ������Ϣ������end
#endif

	if (S_RUN)
	{
		Read_D(D_Address[SYS] + SYS_SPEED, 1, (U16 *)(&YREG[SYS_SPEED])); //���й����У���ȡϵͳ��Ϣ_�ٶ�
	}

	Read_D(D_Address[SYS] + SYS_LISTTYPE, 1, (U16 *)(&YREG[SYS_LISTTYPE])); // �������ڴ���������
	if (YREG[SYS_LISTTYPE] != 0)
	{							 //��ǰ�ļ��������������ڴ� �����������ڴ�����
		ReadPatList(0, MENU_P1); //�ض��ڴ��еĻ�������
	}

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	tmpselectdisno = g_oldselectdisno;
	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ
		if (keydata == KEY_P1_F3 || keydata == KEY_P1_F4 || keydata == KEY_P1_F5 ||
			keydata == KEY_P1_F6 || keydata == KEY_P1_F7) //�ǳ��水��
		{
			f = keydata - KEY_P1_F3;
			if (PatternList.selectEnable[f] == 0)
				keydata = 0;
		}

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����
		if ((S_RUN) && (keydata)) //����״̬�£������١����١���һҳ�� ���� �ڰ����ɿ�ʱ ��Ч
		{
			//if((keydata<KEY_P1_UP)||((keydata>KEY_P1_DOWN)&&(keydata<KEY_P1_NEXT))){keydata = 0;}
			if ((keydata != KEY_P1_UP) && (keydata != KEY_P1_DOWN) && (keydata != KEY_P1_NEXT) && (keydata != 0xff))
			{
				keydata = 0;
			}
		}
		if ((keydata == KEY_P1_FBAG && enFBAG == 0) ||
			keydata == KEY_P1_FIXTURE ||
			keydata == KEY_P1_IMG || keydata == KEY_P1_NULL) //�۴�������Чʱ������ʧЧ
		{
			keydata = 0;
		}

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			if ((keyon == KEY_P1_JOGUP) || (keyon == KEY_P1_JOGDN) ||
				(keyon == KEY_P1_UP) || (keyon == KEY_P1_DOWN)) //���������������ܵİ���
			{
			}
			else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//	if(((keydata==KEY_P1_SLFORCE)&& (enLF==0))||
			//((keydata==KEY_P1_SPFOOT)&&((PFOOTBT==0)||(pfootf)))){keydata = 0;} //�޸�
			if (((keydata == KEY_P1_SLFORCE) && (TENSIONBT == 0)) ||
				((keydata == KEY_P1_SPFOOT) && ((PFOOTBT == 0) || (pfootf)))) //�ض�����²���Ч����
			{
				keydata = 0;
			}
			else
			{
				if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
				{
					keyon = keydata;
					if (keyon == KEY_P1_SCRNSVR || keyon == KEY_P1_IMG || keyon == KEY_P1_VIEW) //�ް�����Ч
					{
					}
					else if (keyon == KEY_P1_F3 || keyon == KEY_P1_F4 || keyon == KEY_P1_F5 ||
							 keyon == KEY_P1_F6 || keyon == KEY_P1_F7) //�ǳ��水��
					{
						DrawButtonOnZoomIn(keyd[keyon], 2);
					}
					else
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
						beeponf = 1;

						if (g_keyBellT) //����ʱ����������ʱ��
										// if (keyon != KEY_P1_CUT)
							Beep_On(g_keyBellT);

						switch (keyon) //�������±�����ʾ����ʾ
						{
						case KEY_P1_PFOOT:
							if (pfootf == 0)
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						case KEY_P1_FIXTURE:
							if (P_SCP)
							{
								// DrawFMenuStr(36);
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							}
							else
							{
								// DrawFMenuStr(fixdat == 0 ? 26 : fixdat + 32);
								DrawFMenuStr(fixdat == 0 ? keybmpind[keyon].titleStr[1] : keybmpind[keyon].titleStr[2]);
							}
							break;
						case KEY_P1_PINUP:
							if (needlef == 0x20)
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						case KEY_P1_LED:
							if (P_SCP)
							{
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							}
							else
							{
								DrawFMenuStr(ledflag ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
							}
							break;
						default:
							DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						}

						delay = KEYFT;
						if (keyon == KEY_P1_PFOOT)
						{
							delay = 800;
						}
						else if (keyon == KEY_P1_CUT)
						{
							delay = 800;
						}
					}
					else if (beeponf == 1) //�ڶ��ν���
					{
						beeponf = 2;
						if (((keyon == KEY_P1_JOGUP) || (keyon == KEY_P1_JOGDN)) && (F_FILETYPE == PAT_MIT))
							delay = DELAY_JOG1;
						else if (((keyon == KEY_P1_JOGUP) || (keyon == KEY_P1_JOGDN)) && (F_FILETYPE == PAT_SEW))
							delay = DELAY_JOG2;
						else
							delay = KEYNT;
						// if (keyon == KEY_P1_CUT)
						// {
						// 	cutFlag = 1;
						// 	Beep_On(BELLT);
						// }
					}
					else if (beeponf < 20)
					{
						beeponf++;
					}

					if ((keyon == KEY_P1_JOGUP) || (keyon == KEY_P1_JOGDN) ||
						(keyon == KEY_P1_UP) || (keyon == KEY_P1_DOWN)) //���г����������ܰ����Ĺ���
					{
						keyoff = keyon;
						key_on_flag1 = 1;
					}
					Times = 0; //��ǰ
				}
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
			case KEY_P1_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_P1_MENU:
				pno = MENU_MENU;
				break;
			case KEY_P1_JOGUP:
				if ((P_JGM == 1) && (delay != 220))
					break;
				if (g_feedct < g_patd.feed_sum)
				{
					if ((!(S_CLAMP)) && (P_OST == 0))
					{
						pno = MENU_RWHELP;
						hleppara = 13;
						break;
					}
					if (Write_Cmd(C_JOGUP, 0, 0) == 0)
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
					else if (DREG[D_CMDSTATUS] < 99)
					{
						pno = MENU_RWHELP;
						hleppara = 16; //break;
					}
					else if (DREG[D_CMDSTATUS] == 100)
					{
						g_feedct++;
					}
					jog_flash = 1;
				}
				break;

			case KEY_P1_JOGDN:
				if ((P_JGM == 1) && (delay != 220))
					break;
				//if(feedct>0)
				{
					if ((SREG[g_patd.pat_add + (g_feedct - 1) * 2] == 0x06) && F_FILETYPE == PAT_MIT)
					{
						Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
						if (S_PFOOT)
						{ //��ѹ��̧��
							if (Write_Cmd(C_PFOOTUP, 0, 0) == 0)
							{
								pno = MENU_RWHELP;
								hleppara = 15;
								break;
							}
							Read_Status();
						}
					}

					if ((!(S_CLAMP)) && (P_OST == 0))
					{
						pno = MENU_RWHELP;
						hleppara = 13;
						break;
					}
					if (Write_Cmd(C_JOGDN, 0, 0) == 0)
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
					else if (DREG[D_CMDSTATUS] < 99)
					{
						pno = MENU_RWHELP;
						hleppara = 16; //break;
					}
					else if (DREG[D_CMDSTATUS] == 100)
					{
						g_feedct--;
					}
					jog_flash = 1;
				}
				break;

			case KEY_P1_F1:
				if (PatternList.pageNow > MINFILENO)
				{
					PatternList.bFlashPageFlag = 1;
					PatternList.pageNow--;
				}
				break;
			case KEY_P1_F2:
				if (PatternList.pageNow < PatternList.pageNum)
				{
					PatternList.bFlashPageFlag = 1;
					PatternList.pageNow++;
				}
				break;
			case KEY_P1_F3:
				PatternList.noInPageNow = 0;
				PatternList.bFlashSelectFlag = 1;
				break;
			case KEY_P1_F4:
				PatternList.noInPageNow = 1;
				PatternList.bFlashSelectFlag = 1;
				break;
			case KEY_P1_F5:
				PatternList.noInPageNow = 2;
				PatternList.bFlashSelectFlag = 1;
				break;
			case KEY_P1_F6:
				PatternList.noInPageNow = 3;
				PatternList.bFlashSelectFlag = 1;
				break;
			case KEY_P1_F7:
				PatternList.noInPageNow = 4;
				PatternList.bFlashSelectFlag = 1;
				break;

			case KEY_P1_UP:
				if (YREG[SYS_SPEED] < 9)
				{
					if (Write_Cmd(C_SPEEDUP, 0, 0) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15; /*g_para1 = 15;*/
						break;
					}
					else if (DREG[D_CMDSTATUS] == 100)
					{
						//							YREG[SYS_SPEED] ++;
						Read_D(D_Address[SYS] + SYS_SPEED, 1, (U16 *)(&YREG[SYS_SPEED]));
					}
				}
				break;

			case KEY_P1_DOWN:
				if (YREG[SYS_SPEED] > 0)
				{
					if (Write_Cmd(C_SPEEDDN, 0, 0) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15; /*g_para1 = 15;*/
						break;		   //2018-8-29��
					}
					else if (DREG[D_CMDSTATUS] == 100)
					{
						//							YREG[SYS_SPEED] --;
						Read_D(D_Address[SYS] + SYS_SPEED, 1, (U16 *)(&YREG[SYS_SPEED]));
					}
				}
				break;

			case KEY_P1_PFOOT:
				Send_Pfoot(0, MENU_P1, 0);
				break;

			case KEY_P1_ORG:
				if (P_HPK != 3)
				{
					if (Go_Home(MENU_P1, 0))
					{
						g_feedct = 0;
					}
				}
				break;
			case KEY_P1_VIEW:
			case KEY_P1_POS:

				if ((CheckDrawData(MENU_P1, 0)) == 0)
				{ //ͼ�γ���Χ���ܽ��붨λ
					break;
				}
				g_modcordflag = 0;
				g_modcordx = 0;
				g_modcordy = 0;
				g_CodeEn = 1; //��ʾ����
				if (keyoff == KEY_P1_POS)
				{
					g_imgpara = MOIMAGE_SKIPJOG; //0
					if (P_HPF == 0x02)
					{
						if (!(S_CLAMP))//ѹ��̧��״̬
						{ 
							if (Write_Cmd(C_YABANDN, 1, 0) == 0)//ѹ���½�
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
							else if (DREG[D_CMDSTATUS] != 100)
							{
								pno = MENU_RWHELP;
								hleppara = 16;
								break;
							}
						}

						if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
						{
						}
						else
						{								  //����ԭ��
							f = Go_Home(MENU_MOIMAGE, 0); //0
							if (f == 0)
								break;
							isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
							if ((isHome & 0x02) || (isHome & 0x04))
							{
							}
							else
							{ //û�лص���еԭ��λ��
								break;
							}
						}
						f = Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
					}

					if ((!(S_CLAMP)) && (P_OST == 0))
					{
						pno = MENU_RWHELP;
						hleppara = 13;
						break;
					}
				}
				else if (keyoff == KEY_P1_VIEW)
				{
					g_imgpara = MOIMAGE_VIEW; //1
				}

				g_jogfunction1 = 1;
				if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
				{
				}
				else
				{ //����ԭ��
					g_errLocal = 0;
					pno = MENU_RWHELP;
					hleppara = 148;
					poldno[pnoindex] = MENU_P1;
					pnoindex++;
					break;
				}
				g_BorSTflag = 0;
				g_max_feedsum = g_patd.feed_sum;
				g_min_feedsum = 0;
				g_jog_point = 0;
				g_IsLocal = 1;
				g_ImgMove = 1;
				g_errLocal = 0;

				g_ExCode = 0;
				g_curExCode = 0;
				g_curPinAng = 0;
				g_curSetPara1 = 0;
				g_curSetPara2 = 0;
				g_pointPinAng = 0;
				g_pointSetPara1 = 0;
				g_pointSetPara2 = 0;
				pno = MENU_MOIMAGE;
				poldno[pnoindex] = MENU_P1;
				pnoindex++;
				break;

			case KEY_P1_SLFORCE:
				pno = MENU_LINEFORCE;
				g_para0 = 0;
				break;
			case KEY_P1_SPFOOT:
				pno = MENU_PFOOT;
				g_para0 = 1;
				break;
			case KEY_P1_NEXT:
				pno = MENU_P2;
				if (F_FILETYPE == PAT_MIT) //�����
				{
					g_orgsub_f = SREG[3];
				}
				break;

			case KEY_P1_CUT:
				if (1) //cutFlag
				{
					if (Write_Cmd(C_CUT, 0, 0) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						break;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
					}
				}
				break;

			case KEY_P1_LED:
				if (P_SCP)
				{
					if (Write_Cmd(C_MCLAMP, 0, 0) == 0)
					{	//Сѹ��
						//pno = MENU_C2_HELP;hleppara = 15;break;
					}
					if (Read_Status() == 0)
					{
						//pno = MENU_C2_HELP;hleppara = 15;break;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{
						//pno = MENU_C2_HELP;hleppara = 16;break;
					}
				}
				else
				{
					Write_Cmd1(C_LEDONOFF, 0, 0);
				}

				break;
			case KEY_P1_PINUP:
				Write_Cmd1(C_NEEDLEUP, 0, 0);
				break;
			case KEY_P1_FIXTURE:
				if (P_SCP)
				{
					if (Write_Cmd(C_BCLAMP, 0, 0) == 0)
					{	//��ѹ��
						//pno = MENU_C2_HELP;hleppara = 15;break;
					}
					if (Read_Status() == 0)
					{
						//pno = MENU_C2_HELP;hleppara = 15;break;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{
						//pno = MENU_C2_HELP;hleppara = 16;break;
					}
				}
				else
				{
					if (Write_Cmd(C_FIXTURE, 0, 0) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						break;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
					}
					// fixtureFlag = 1;
				}
				break;

			case KEY_P1_WIND:
				Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
				if ((S_HOME) || (S_NEWHOME) || (S_2HOME))
				{
					if (pfootf == 0)
					{
						Send_Pfoot(0, MENU_P1, 0);
					}
					pno = MENU_WIND;
					E_WINDEN();
					i = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
					if (i == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						break;
					}
				}
				else
				{
					pno = MENU_RWHELP;
					hleppara = 17;
				}
				break;

			case KEY_P1_THRED:
				pno = MENU_THREAD;
				E_RUNEN(); //��ֹ����
				Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
				break;

			case KEY_P1_OTHER1: //����Flash�ļ� ������
				//	if(ExtMemory_Status(USB)!=0){	break;	}
				//	ii = AnalyseUpdateList(0,0);
				AnalyseUpdateList(0, 0);

				errFlag = 0;
				errCnt = 0;
				g_logorun = 0;
				while (errCnt < 3)
				{
					SETWCS1(); //ȡ��Ƭѡ
					SETWCS2(); //ȡ��Ƭѡ
					SETTFCS();
					errFlag = 0;

					if (update_flash(1, Color_White) == TRUE)
					{ // ����FLASH
						//д�����ݺ��ض�У�飬���ɹ�����3��
						W25QXX_Read(W25Q1_DAT, (U8 *)checkbuff, FLASHINDEXADDR + 24, 12);
						maxadd = (((U32)checkbuff[1]) << 16) + checkbuff[0];
						//									#if Debug
						//printf("maxadd = %08x: SDRAMFLASHADD = %8x errCnt = %d\r\n",maxadd,SDRAMFLASHADD,errCnt);
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
						//	printf("--listlen = %d \r\n",listlen);
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
							//printf("tmpflashadd[%d] = %d \r\n",cnti,tmpflashadd[cnti]);
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

								//printf("datasum = %0x  tmpdatasum = %0x  \r\n",datasum,tmpdatasum);

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
													 //								creatUpInfoData(Info);
							W25QXX_Write(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
							f |= 0x02;

							// �ر������ж�
							__set_FAULTMASK(1);
							// ��λ
							NVIC_SystemReset();
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
					}
					else
					{
						//printf("update_flash err \r\n");
						break;
					}
				}
				break;
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			if (keyon == KEY_P1_SCRNSVR || keyon == KEY_P1_IMG || keyon == KEY_P1_VIEW) //����Ҫ��ʾ�İ���
			{
			}
			else if (keyon == KEY_P1_F3 || keyon == KEY_P1_F4 || keyon == KEY_P1_F5 ||
					 keyon == KEY_P1_F6 || keyon == KEY_P1_F7) //�ǳ��水��
			{
				DrawButtonOnZoomInRes(keyd[keyon]);
			}
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}
			keyon = 0;
			// cutFlag = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ��P1
			Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

			if (g_GroupType == 1 || g_GroupType == 2)
			{ //�����
				Draw24x24Str(rect[RECT_P1_GROUP].xpis, rect[RECT_P1_GROUP].ypis, MEM_TITLE_ADD + ((U32)rect[RECT_P1_GROUP].index[0] + g_GroupType) * TitleLengthMax,
							 SheetPage, rect[RECT_P1_GROUP].index[0] + g_GroupType, rect[RECT_P1_GROUP].colorT[0]); //���ģʽ���Ӻ�ģʽ

				Draw24X24FStr(rect[RECT_P1_GROUP].xpis + 12 * 9, rect[RECT_P1_GROUP].ypis, (char *)g_GroupName, rect[RECT_P1_GROUP].colorT[0]);
				DrawMFU32(rect[RECT_P1_GROUP].xpis + 12 * 18, rect[RECT_P1_GROUP].ypis, g_GroupNO, 2, rect[RECT_P1_GROUP].colorT[0]);
				Draw24X24FStr(rect[RECT_P1_GROUP].xpis + 12 * 18 + 12 * 2, rect[RECT_P1_GROUP].ypis, "/", rect[RECT_P1_GROUP].colorT[0]);
				DrawMFU32(rect[RECT_P1_GROUP].xpis + 12 * 18 + 12 * 3, rect[RECT_P1_GROUP].ypis, g_GroupTol, 2, rect[RECT_P1_GROUP].colorT[0]);
			}
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if ((Timerms > 1000))
		{
			Timerms = 0;
			if ((g_GroupType == 1) || (g_GroupType == 2)) // ��ȡ��ǰ���ģʽ����ϱ�ţ���ǰ���ڵĻ�����ţ���ǰ��Ļ�������
			{
				Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE]));
				if (lastGroupNO != g_GroupNO) //���ģʽ��
				{
					lastGroupNO = g_GroupNO;
					tmpGroupNO = g_GroupNO;

					//��ʾ��ǰ���
					BmpFillRect(rect[RECT_P1_GROUP].xpis + 12 * 18, rect[RECT_P1_GROUP].ypis, 24, 24);
					DrawMFU32(rect[RECT_P1_GROUP].xpis + 12 * 18, rect[RECT_P1_GROUP].ypis, tmpGroupNO, 2, rect[RECT_P1_GROUP].colorT[0]);
				}
//�޸�
#if 0 
				if (g_GroupType == 2) //�Ӻ�ģʽ��
				{
					Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
					if (tmpselectdisno != F_IND_NO)
					{
						Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ�����ļ�
						data_flash = 1;
					}
				}
#else
				Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
				if (tmpselectdisno != F_IND_NO)
				{
					Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ�����ļ�
					data_flash = 1;
				}
#endif
			}
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[���»�����ʾ�б�*begin]**********************************************************
		if (PatternList.bflashAllFlag) //������Ϣ����
		{
			PatternList.bflashAllFlag = 0;
			PatternList.bFlashPageFlag = 1;
			if (g_GroupType == 1 || g_GroupType == 2)
			{
				PatternList.selectNoNow = g_GroupNO;
				PatternList.patternNum = g_GroupTol;
			}
			else
			{
				PatternList.selectNoNow = 0;
				PatternList.patternNum = 0;
				for (i = 0; i < MAXIREG2; i += FileIndexSize) //���������洢�ռ䣬��ȡ ���ϻ�����������ǰ�������
				{
					if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
						(((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
					{
						PatternList.patternNum++;
						if ((i / FileIndexSize + MINFILENO) <= g_selectdisno)
							PatternList.selectNoNow++;
					}
				}
			}
			//ȷ�� ��ǰѡ�����ǰѡ��ҳ��ҳ������
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

		if (PatternList.bFlashSelectFlag) //����ѡ�������
		{
			PatternList.bFlashSelectFlag = 0;
			PatternList.bFlashPageFlag = 1;

			if (PatternList.dataAdd[PatternList.noInPageNow] == 0xFFFF) //ѡ�п�
			{
				PatternList.noInPageNow = PatternList.noInPageLast;
			}
			else
			{
				PatternList.noInPageLast = PatternList.noInPageNow;
			}
			if (g_GroupType == 1 || g_GroupType == 2) //���ģʽ��
			{
				tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow]; //ת��Ϊ�������disno
			}
			else
			{
				tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO; //ת��Ϊ�������disno
			}

			if (tmpselectdisno != 0xffff &&
				tmpselectdisno != g_selectdisno)
			{
				PatternList.selectNoNow = PatternList.noInPageNow + MINFILENO + (PatternList.pageNow - 1) * PatternList.disNumDiv;
				PatternList.selectNoLast = PatternList.selectNoNow;
				PatternList.pageLast = PatternList.pageNow;

				g_Fileno = tmpselectdisno;
				g_selectdisno = tmpselectdisno;
				g_oldselectdisno = tmpselectdisno;
				if (g_GroupType == 1 || g_GroupType == 2) //���ģʽ��
				{
					tmpGroupNO = PatternList.selectNoNow;
					g_GroupNO = tmpGroupNO;
					if (Write_Cmd(C_SWITCHGROUP, g_GroupNO, 0) == 0) //�����л����ͼ��ָ��
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
					// ��ȡ��ǰ���ģʽ����ϱ�ţ���ǰ���ڵĻ�����ţ���ǰ��Ļ�������
					Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE]));
					tmpGroupNO = g_GroupNO;
					lastGroupNO = g_GroupNO;

					Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
					Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0]));	 // ����ǰ�����ļ�
					BmpFillRect(rect[RECT_P1_GROUP].xpis + 12 * 18, rect[RECT_P1_GROUP].ypis, 24, 24);
					DrawMFU32(rect[RECT_P1_GROUP].xpis + 12 * 18, rect[RECT_P1_GROUP].ypis, tmpGroupNO, 2, rect[RECT_P1_GROUP].colorT[0]);
					data_flash = 1;
				}
				else
				{
					if (g_ReadPatternFlag == 1)
					{
						g_ReadPatternFlag = 0;
					}

					f = fun_getPatData(g_selectdisno, g_disk_memory, MENU_P1);
					if (f)
					{
						g_init_flag = 1;
						data_flash = 1;
					}
				}
			}
		}

		if (PatternList.bFlashPageFlag) //������ҳ����
		{

			U32 numA = 0; //��ǰҳ�Ļ�����Ż���
			// U32 numB = 0; //��ǰҳ�Ļ������ƫ����
			U32 addA = 0; //��ǰҳ�Ļ�����Ż���ַ
			U32 addB = 0; //��ǰҳ�Ļ������ƫ�Ƶ�ַ
			U32 patternLen = 0;
			U8 patternType = 0;
			U16 disColor = 0;
			PatternList.bFlashPageFlag = 0;

			//��ʾҳ��
			DrawFillRect(rect[RECT_P1_PAGE].xpis, rect[RECT_P1_PAGE].ypis,
						 rect[RECT_P1_PAGE].xsize, rect[RECT_P1_PAGE].ysize, rect[RECT_P1_PAGE].colorB[0]);
			DrawFU32(rect[RECT_P1_PAGE].xpis + SAFE_DIST4, rect[RECT_P1_PAGE].ypis + SAFE_DIST4,
					 PatternList.pageNow, 3, rect[RECT_P1_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_P1_PAGE].xpis + SAFE_DIST4, rect[RECT_P1_PAGE].ypis + SAFE_DIST7,
					   "---", rect[RECT_P1_PAGE].colorT[0]);
			DrawFU32(rect[RECT_P1_PAGE].xpis + SAFE_DIST4, rect[RECT_P1_PAGE].ypis + SAFE_DIST10,
					 PatternList.pageNum, 3, rect[RECT_P1_PAGE].colorT[0]);

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

			if (g_GroupType == 1 || g_GroupType == 2)
			{

				if (numA) //���㵱ǰҳ�Ļ���ַ
				{
					for (j = 0, addA = 0; addA < GroupMax; addA += 1)
					{
						if (GREG[addA] >= MINFILENO && GREG[addA] <= MAXFILENO)
						{
							j++;
							if (j > numA)
								break;
						}
					}
				}
				addB = addA;
				if (numA < PatternList.patternNum) //���㵱ǰҳ�ĸ����ļ���ƫ�Ƶ�ַ
				{
					for (j = 0; addB < GroupMax; addB += 1)
					{
						if (GREG[addA] >= MINFILENO && GREG[addA] <= MAXFILENO)
						{
							PatternList.dataAdd[j] = GREG[addB];
							j++;
							if (((numA + j) >= PatternList.patternNum) || (j >= PatternList.disNumDiv))
								break;
						}
					}
					for (; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
						PatternList.dataAdd[j] = 0xffff;
				}
				else
				{
					for (j = 0; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
						PatternList.dataAdd[j] = 0xffff;
				}
				//�������ģʽ�µİ�ť��Ϣ
				for (i = 0; i < PatternList.disNumDiv; i++)
				{
					if (PatternList.pageLast == PatternList.pageNow)
					{
						DrawButtonEdit(keyd[i + KEY_P1_F3],
									   (i == PatternList.noInPageNow) ? keybmpind[i + KEY_P1_F3].textColor[1] : keybmpind[i + KEY_P1_F3].icon[0],
									   (i == PatternList.noInPageNow) ? keybmpind[i + KEY_P1_F3].icon[1] : keybmpind[i + KEY_P1_F3].icon[0]);
					}
					else
					{
						DrawButtonEdit(keyd[i + KEY_P1_F3], keybmpind[i + KEY_P1_F3].icon[0], keybmpind[i + KEY_P1_F3].icon[0]);
					}
					patternLen = ((IREG2[(PatternList.dataAdd[i] - MINFILENO) * FileIndexSize + 1] << 16) | IREG2[(PatternList.dataAdd[i] - MINFILENO) * FileIndexSize]);
					patternType = ((IREG2[(PatternList.dataAdd[i] - MINFILENO) * FileIndexSize + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
					disColor = ((PatternList.dataAdd[i] <= MAXFILENO) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
								   ? ((patternType != 0) ? MitFontColor : BorFontColor)
								   : keybmpind[i + KEY_P1_F3].textColor[0];
					DrawFFileno_24x12(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4, keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
									  (PatternList.dataAdd[i] > MAXFILENO) ? 0 : PatternList.dataAdd[i], disColor);
					Draw24X24FStr(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4 + 13 * 3, keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
								  "@", keybmpind[i + KEY_P1_F3].textColor[0]);

					if ((PatternList.dataAdd[i] - MINFILENO) * FileIndexSize >= MAXIREG2)
					{
						DrawFFilename(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4 + 13 * 4,
									  keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
									  "NO__DATA", strlen("NO__DATA"), NO_DATAColor, 6, 0, NULL);
						PatternList.selectEnable[i] = 0;
					}
					else
					{
						DrawFFilename(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4 + 13 * 4,
									  keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
									  (char *)(&IREG2[(PatternList.dataAdd[i] - MINFILENO) * FileIndexSize + FileIndexNamepox]),
									  64, disColor, 6, 0, NULL);
						PatternList.selectEnable[i] = 1;
					}
				}
			}
			else
			{
				if (numA) //���㵱ǰҳ�Ļ���ַ
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
				if (numA < PatternList.patternNum) //���㵱ǰҳ�ĸ����ļ���ƫ�Ƶ�ַ
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
					for (; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
						PatternList.dataAdd[j] = 0xffff;
				}
				else
				{
					for (j = 0; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
						PatternList.dataAdd[j] = 0xffff;
				}
				//���ư�ť��Ϣ
				for (i = 0; i < PatternList.disNumDiv; i++)
				{
					if (PatternList.pageLast == PatternList.pageNow)
					{
						DrawButtonEdit(keyd[i + KEY_P1_F3],
									   (i == PatternList.noInPageNow) ? keybmpind[i + KEY_P1_F3].textColor[1] : keybmpind[i + KEY_P1_F3].icon[0],
									   (i == PatternList.noInPageNow) ? keybmpind[i + KEY_P1_F3].icon[1] : keybmpind[i + KEY_P1_F3].icon[0]);
					}
					else
					{
						DrawButtonEdit(keyd[i + KEY_P1_F3], keybmpind[i + KEY_P1_F3].icon[0], keybmpind[i + KEY_P1_F3].icon[0]);
					}

					patternLen = ((IREG2[(PatternList.dataAdd[i]) + 1] << 16) | IREG2[(PatternList.dataAdd[i])]);
					patternType = ((IREG2[(PatternList.dataAdd[i]) + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
					disColor = ((PatternList.dataAdd[i] < MAXIREG2) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
								   ? ((patternType != 0) ? MitFontColor : BorFontColor)
								   : keybmpind[i + KEY_P1_F3].textColor[0];
					DrawFFileno_24x12(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4, keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
									  (PatternList.dataAdd[i] >= MAXIREG2) ? 0 : PatternList.dataAdd[i] / FileIndexSize + MINFILENO, disColor);

					Draw24X24FStr(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4 + 13 * 3, keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
								  "@", keybmpind[i + KEY_P1_F3].textColor[0]);

					if (PatternList.dataAdd[i] >= MAXIREG2)
					{
						DrawFFilename(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4 + 13 * 4,
									  keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
									  "NO__DATA", strlen("NO__DATA"), NO_DATAColor, 6, 0, NULL);
						PatternList.selectEnable[i] = 0;
					}
					else
					{
						DrawFFilename(keyd[i + KEY_P1_F3].xpis + SAFE_DIST4 + 13 * 4,
									  keyd[i + KEY_P1_F3].ypis + SAFE_DIST1,
									  (char *)(&IREG2[PatternList.dataAdd[i] + FileIndexNamepox]),
									  64, disColor, 6, 0, NULL);
						PatternList.selectEnable[i] = 1;
					}
				}
			}
		}
		// /[���»�����ʾ�б�*end]**********************************************************

		// /[��ǰ�������ݸ���*bigin]**********************************************************
		if (data_flash)
		{
			data_flash = 0;
			if (S_2HD) //���ݴ�ԭ����Ϣ������ʾ����������
				AnaPattern(1, F_FILETYPE, (F_FILELEN));
			else
				AnaPattern(0, F_FILETYPE, (F_FILELEN));

			if (g_ReadPatternFlag == 1) //��ʾ������ڴ滨��
			{
				//ReadPatternFlag = 0;
			}
			else
			{
				pattern_flash = 1;
			}
			g_patd.fno = F_IND_NO;								  //��ȡ�������
			if ((g_patd.fno != g_patd_fno) || (g_NoEndCode == 1)) //��ȡ��ͬ�Ļ���ǰҪ���ж�ͼ���Ƿ񳬷�Χ
			{
				if (g_NoEndCode == 1)
					g_NoEndCode = 0;
				g_patd_fno = g_patd.fno;
				if ((CheckDrawData(MENU_P1, 0)) == 0) //ͼ�γ�������
				{
					//return;
				}
			}

			//�������ƣ�//����� Y�����
			DrawFillRect(rect[RECT_P1_NAME_TIT].xpis, rect[RECT_P1_NAME_TIT].ypis,
						 rect[RECT_P1_NAME_TIT].xsize, rect[RECT_P1_NAME_TIT].ysize, rect[RECT_P1_NAME_TIT].colorB[0]); // �ļ�����ɫ
			DrawFMultiLStr24LimtL(rect[RECT_P1_NAME_TIT].xpis + SAFE_DIST4, rect[RECT_P1_NAME_TIT].ypis + (rect[RECT_P1_NAME_TIT].ysize - 24) / 2,
								  (U16 *)(&g_patd.fname), 28, 1, rect[RECT_P1_NAME_TIT].colorT[0]); //����1�У������ʾ28���ַ�

			//������ţ�//����� Y�����
			DrawFillRect(rect[RECT_P1_NUM_TIT].xpis, rect[RECT_P1_NUM_TIT].ypis,
						 rect[RECT_P1_NUM_TIT].xsize, rect[RECT_P1_NUM_TIT].ysize, rect[RECT_P1_NUM_TIT].colorB[0]); // ��ŵ�ɫ

			DrawFFileno_24x12(rect[RECT_P1_NUM_TIT].xpis + (rect[RECT_P1_NUM_TIT].xsize - 13 * 3) / 2,
							  rect[RECT_P1_NUM_TIT].ypis + (rect[RECT_P1_NUM_TIT].ysize - 24) / 2,
							  g_patd.fno, rect[RECT_P1_NUM_TIT].colorT[0]);

			//���·��һ�����Ϣ�� //������//�����
			DrawFillRect(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + TYPE_SIZE5 * 2,
						 rect[RECT_P1_PIN].xsize, TYPE_SIZE5, rect[RECT_P1_PIN].colorB[0]);
			DrawMFU32(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + TYPE_SIZE5 * 2 + SAFE_DIST4,
					  g_patd.feed_pat, 5, rect[RECT_P1_PIN].colorT[0]);
			//���·��һ�����Ϣ�� //������//�����
			DrawFillRect(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + TYPE_SIZE5 * 3,
						 rect[RECT_P1_PIN].xsize, TYPE_SIZE5, rect[RECT_P1_PIN].colorB[0]);
			DrawMFU32(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + TYPE_SIZE5 * 3 + SAFE_DIST4,
					  g_patd.feed_sum, 5, rect[RECT_P1_PIN].colorT[0]); //������

			// tmuly = (F_FILETYPE == PAT_SEW) ? -1 : 1;

			// tscale = pat_getScale(CONVAS1);
		}
		// /[��ǰ�������ݸ���*end]**********************************************************

		// /[��ǰ����ͼ����ʾ*begin]**********************************************************
		if (pattern_flash)
		{
			pattern_flash = 0;

			tozero = 1;	   // ��ԭ�����ݸ���
			jog_flash = 1; // �������ݸ���

			if (S_RUN) //�˶������У���ȡ��ǰ���λ��
			{
				Read_D(D_Address[HEAD] + HREG_FEED_C, 1, (U16 *)(&HREG[HREG_FEED_C]));
			}
			// tscale1 = Scale + tscale;
			// if (tmpFno != F_IND_NO) //������Ϣ����ȵ�ʱ�򣬸��»���
			// 	tmpFno = F_IND_NO;

#if 0
			tscale1 = Scale + tscale;

			xorg = xo - (g_patd.patmX + g_patd.patiX) / 2 * tscale1 / 1000; //����ͼ�����ĵ������
			yorg = yo - tmuly * (g_patd.patmY + g_patd.patiY) / 2 * tscale1 / 1000;

			if (HREG[HREG_FEED_C] > g_patd.feed_sum) //��ǰ��λ�ô����ܵ�λ��
				HREG[HREG_FEED_C] = 0;

			getFeedPos(HREG[HREG_FEED_C], F_FILETYPE, &fpx, &fpy); //��ȡ��ǰ���λ��
			vposx = (S32)(fpx * tscale1 / 1000 + xorg);			   //���㵱ǰ������Ļ�е�λ��
			vposy = (S32)(tmuly * fpy * tscale1 / 1000 + yorg);

			// vposx0 = vposx;
			// vposy0 = vposy;
			Draw_Pattern(PAT_P1, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0, 0, NULL); // ��ʾ���� - �����
			ReadPointData1(vposx - 4, vposy - 4);													  //��ȡԭ����Χ������
			// ViewPoint(vposx, vposy, (xo == vposx && yo == vposy) ? COLOR_ORG : COLOR_CROSS, 0);		  //���Ƶ�ǰ���λ��
			ViewPoint(vposx, vposy, COLOR_CROSS, 0); //���Ƶ�ǰ���λ��
#else
			pat_clearConvas(patRule.h_patConvasIndex);
			pat_drawPattern(PATRULES_P1, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);

#endif
		}
		// /[��ǰ����ͼ����ʾ*end]******************************************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(MENU_P1);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			U32 wTimeTemp;

			ErrorMsg(MENU_P1);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}

			if (S_RUN)
			{

				wTimeTemp = Timer_ms >> 5;	//Timer_ms/2^5
				if (wTimeTemp != times50ms) //��ʱ������������
				{
					times50ms = wTimeTemp;
					jog_flash = 1;
				}
			}
			else
			{

				wTimeTemp = Timer_ms >> 8; //Timer_ms/2^8
				if (wTimeTemp != times50ms)
				{
					U32 wReadACK;
					U32 wPatternNo, wPatternNum;
					times50ms = wTimeTemp;
					jog_flash = 1;

					if (S_RUN)
					{
					}
					else
					{
						//��ʾ������ʾ
						Read_D(D_Address[SYS] + SYS_DNCOUNT, 2, (U16 *)(&YREG[SYS_DNCOUNT]));
						Read_D(D_Address[SYS] + SYS_UPCOUNT, 2, (U16 *)(&YREG[SYS_UPCOUNT]));

						Read_D(D_Address[SYS] + SYS_SPEED, 1, (U16 *)(&YREG[SYS_SPEED]));
						wReadACK = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //��ȡ64λ���ݣ���ȫ����HREG[]
						if (wReadACK)															//��ȡ�ɹ�
						{
							if (F_IND_NO != g_selectdisno)
							{
								wPatternNo = F_IND_NO;										//��ȡ��ǰ�������
								if ((wPatternNo >= MINFILENO) && (wPatternNo <= MAXFILENO)) //�����ȷ
								{
									wPatternNo = (wPatternNo - MINFILENO) * FileIndexSize;						  //��ȡ��ǰ�����������ڻ���������λ��
									wPatternNum = (((U32)(IREG2[wPatternNo + 1]) << 16) + IREG2[wPatternNo]) / 2; //��ȡ��ǰ�������ļ���

									if ((wPatternNum >= 0x40) && (wPatternNum <= MAXSREG)) //������С��Ϣ��ȷ
									{
										wReadACK = Read_D(D_Address[PTDATA], wPatternNum, (U16 *)(&SREG[0])); // ����ǰ��������

										if (wReadACK) //��ȡ�ɹ�
										{
											coldstart = 0;
											g_init_flag = 1;
											g_Fileno = F_IND_NO;
											g_selectdisno = F_IND_NO;
											tmpselectdisno = F_IND_NO;
											g_oldselectdisno = F_IND_NO;
											data_flash = 1;
											//vflag =0;
										}
									}
								}
							}
						}
						Read_D(D_Address[HEAD] + HREG_FEED_C, 1, (U16 *)(&HREG[HREG_FEED_C]));
						Read_D(D_Address[SYS] + SYS_RUN_TIMER, 2, (U16 *)(&YREG[SYS_RUN_TIMER]));
					}
				}
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[������Ϣ����*begin]**********************************************************
		if (jog_flash)
		{
			U32 wReadACK;
			jog_flash = 0;
			//			if(enFBAG){
			//				Read_D(FGPARA_ADDR+23,1,&fbstep);
			//				if(fbstep !=ofbstep){
			//					SetLanguage(MENU_FUNCTION);
			//				#if	USEMULTCOLOR	//��ɫ
			//					BmpFillRect(272,80,320,16);
			//				#else
			//					DrawFillRect(272,80,320,16,Color_White);
			//				#endif
			//					DrawFMultiLStrR(592, 80,MEM_TITLE_ADD+TitleLengthMax*(TLFNADD_OTHER2+16+fbstep), 40,1,Color_Black);
			//					//DrawFMultiLStr(keyd[KEY_P1_FBAG].xpis+8, keyd[KEY_P1_FBAG].ypis+28,MEM_TITLE_ADD+TitleLengthMax*(632+fbmode), 6,2,Color_Red);
			//					SetLanguage(MENU_P1);
			//				}
			//			}
			wReadACK = Read_D(D_Address[HEAD] + HREG_FEED_C, 1, (U16 *)(&HREG[HREG_FEED_C])); //��ȡ��ǰ������λ�� ��18 ������Ϣ����
			if (wReadACK)																	  //��ȡ�ɹ�
			{
				g_feedct = HREG[HREG_FEED_C];

				if (temp_feed != g_feedct)
				{
					//���ƽ�����
					BmpFillRect(rect[RECT_P1_PROGRESS].xpis, rect[RECT_P1_PROGRESS].ypis,
								rect[RECT_P1_PROGRESS].xsize, rect[RECT_P1_PROGRESS].ysize);
					DrawProgressbar(rect[RECT_P1_PROGRESS].xpis, rect[RECT_P1_PROGRESS].ypis,
									rect[RECT_P1_PROGRESS].xsize, rect[RECT_P1_PROGRESS].ysize,
									g_feedct * 100 / g_patd.feed_sum, rect[RECT_P1_PROGRESS].colorB[0], rect[RECT_P1_PROGRESS].colorT[0]);
					DrawFU32(TYPE_X2 + CONVAS_X1 / 2, 377 + SAFE_DIST4, g_feedct * 100 / g_patd.feed_sum, 3, rect[RECT_P1_PROGRESS].index[0]);
					DrawF16Str(TYPE_X2 + CONVAS_X1 / 2 + 8 * 3, 377 + SAFE_DIST4, "%", rect[RECT_P1_PROGRESS].index[0]);
				}
			}

			if (HREG[HREG_FEED_C] > g_patd.feed_sum) //��ǰ������λ���쳣�Ļ�������
				HREG[HREG_FEED_C] = 0;

			oldtmpS_2HOME = tmpS_2HOME;
			tmpS_2HOME = (U16)(S_2HOME);

			if ((temp_feed != HREG[HREG_FEED_C]) ||
				((temp_feed == 0) && (oldtmpS_2HOME != tmpS_2HOME)))
			{
				// temp_feed0 = temp_feed;
				if (HREG[HREG_FEED_C] < temp_feed) //����ʵ��������������������ˢ��
				{
					// temp_feed0 = temp_feed;
					bflag = (S_RUN ? 2 : (temp_feed - HREG[HREG_FEED_C] > 1 ? 2 : 1));
				}
				else if (HREG[HREG_FEED_C] - temp_feed > 1) //�����������ˢ��
				{
					// temp_feed0 = temp_feed;
					bflag = 3;
				}
				else if (temp_feed == 0)
					bflag = 2;
				else
					bflag = 0;

				temp_feed = HREG[HREG_FEED_C];
				if (temp_feed == 0)
				{
					if (tmpS_2HOME == 0x2000)
						g_BorSTflag = 1; //��ԭ��
					else
						g_BorSTflag = 0; //��еԭ��
				}

				//����룬�׶���
				DrawFillRect(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + TYPE_SIZE5 * 1,
							 rect[RECT_P1_PIN].xsize, TYPE_SIZE5, rect[RECT_P1_PIN].colorB[0]);
				DrawMFU32(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + TYPE_SIZE5 * 1 + SAFE_DIST4,
						  temp_feed, 5, rect[RECT_P1_PIN].colorT[0]); //��ǰ����Ϣλ��
				//���У��׶���
				DrawFillRect(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis,
							 rect[RECT_P1_PIN].xsize, TYPE_SIZE5, rect[RECT_P1_PIN].colorB[0]);
				viewFeedCode(rect[RECT_P1_PIN].xpis, rect[RECT_P1_PIN].ypis + SAFE_DIST1,
							 temp_feed, F_FILETYPE, tozero, 2); //��ʾ�������
				if (tozero)
					tozero = 0;

				// getFeedPos(temp_feed, F_FILETYPE, &fpx, &fpy);
				// vposx = (S32)(fpx * tscale1 / 1000 + xorg);
				// vposy = (S32)(tmuly * fpy * tscale1 / 1000 + yorg);

				if (temp_feed == 0 || bflag == 2) //��ǰ��ָ��ԭ�㣬�ػ滨��
				{
					// Draw_Pattern(PAT_P1, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0, 0, NULL);
					pat_clearConvas(patRule.h_patConvasIndex);
					pat_drawPattern(PATRULES_P1, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
				}
				else if (bflag == 3) //��ʾ���й��̵��������
				{
					RecoveData1();
					// ViewMutPoint(PAT_P1, Scale, xo, yo, F_FILETYPE, (F_FILELEN), temp_feed0);
					pat_drawPattern(PATRULES_P1, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
				}
				else if (bflag == 1) //���������ػ�
				{
					RecoveData1();
					// ViewMutPoint(PAT_P1, Scale, xo, yo, F_FILETYPE, (F_FILELEN), temp_feed0);
					pat_drawPattern(PATRULES_P1, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
				}
				else //������ɵ���
				{
					RecoveData1();
					// ViewMutPoint(PAT_P1, Scale, xo, yo, F_FILETYPE, (F_FILELEN), temp_feed0);
					pat_drawPattern(PATRULES_P1, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
				}
			}
		}
		// /[������Ϣ����*end]**********************************************************

		// /[�ص���ԭ��*begin]**********************************************************
		if (tmp_s_2hd != (U16)(S_2HD))
		{
			tmp_s_2hd = (U16)(S_2HD);
			if (S_2HD)
				AnaPattern(1, ((((IREG2[(g_selectdisno - MINFILENO) * FileIndexSize + FileIndexSize - 1] & 0xff00) >> 8) != 0) ? PAT_MIT : PAT_SEW),
						   (((IREG2[(g_selectdisno - MINFILENO) * FileIndexSize + 1]) << 16) | IREG2[(g_selectdisno - MINFILENO) * FileIndexSize]));
			else
				AnaPattern(0, ((((IREG2[(g_selectdisno - MINFILENO) * FileIndexSize + FileIndexSize - 1] & 0xff00) >> 8) != 0) ? PAT_MIT : PAT_SEW),
						   (((IREG2[(g_selectdisno - MINFILENO) * FileIndexSize + 1]) << 16) | IREG2[(g_selectdisno - MINFILENO) * FileIndexSize]));
			pattern_flash = 1;

			//DrawFillRect(51,39,226,162,Color_Silver);
		}
		// /[�ص���ԭ��*end]**********************************************************

		// /[ת����Ϣ���´���*begin]**********************************************************
		if (tmp_speed != YREG[SYS_SPEED])
		{
			//if(YREG[SYS_SPEED]>9)YREG[SYS_SPEED] = 0;
			tmp_speed = YREG[SYS_SPEED];

			// DrawFillRect(keyd[KEY_P1_MAX + 4].xpis + 4, keyd[KEY_P1_MAX + 4].ypis + 4, keyd[KEY_P1_MAX + 4].xsize - 8, keyd[KEY_P1_MAX + 4].ysize - 8, Color_Black);
			// if (speedFlag)
			// {
			// 	DrawMFU32Z(keyd[KEY_P1_MAX + 4].xpis + 8, keyd[KEY_P1_MAX + 4].ypis + 10, YREG[SYS_SPEED] * 100, 4, Color_White);
			// }
			// else
			// {
			// 	DrawLFU32Z(keyd[KEY_P1_MAX + 4].xpis + 22, keyd[KEY_P1_MAX + 4].ypis + 6, YREG[SYS_SPEED], 1, Color_White); // ��ʾ
			// }

			//XY�������ʾ
			DrawFillRect(rect[RECT_P1_SPEED].xpis, rect[RECT_P1_SPEED].ypis,
						 rect[RECT_P1_SPEED].xsize, rect[RECT_P1_SPEED].ysize, rect[RECT_P1_SPEED].colorB[0]);
			if (speedFlag)
			{
				DrawMFU32Z(rect[RECT_P1_SPEED].xpis + (rect[RECT_P1_SPEED].xsize - 12 * 4) / 2,
						   rect[RECT_P1_SPEED].ypis + (rect[RECT_P1_SPEED].ysize - 24) / 2,
						   YREG[SYS_SPEED] * 100, 4, rect[RECT_P1_SPEED].colorT[0]);
			}
			else
			{
				DrawMFU32Z(rect[RECT_P1_SPEED].xpis + (rect[RECT_P1_SPEED].xsize - 12) / 2,
						   rect[RECT_P1_SPEED].ypis + (rect[RECT_P1_SPEED].ysize - 24) / 2,
						   YREG[SYS_SPEED], 1, rect[RECT_P1_SPEED].colorT[0]); // ��ʾ
			}
		}
		// /[ת����Ϣ���´���*end]**********************************************************

		// /[ѹ�������½�ָ��*begin]**********************************************************
		if (pfootf != (U16)(S_PFOOT))
		{
			pfootf = (U16)(S_PFOOT);
			DrawButton(keyd[KEY_P1_PFOOT], (U16 *)(ADD_ICONBMP),
					   keybmpind[KEY_P1_PFOOT].border[0], (pfootf == 0) ? (keybmpind[KEY_P1_PFOOT].icon[0]) : (keybmpind[KEY_P1_PFOOT].icon[1])); //�޸�   ADD_ICONBMP
			DrawButton(keyd[KEY_P1_SPFOOT], (U16 *)(ADD_ICONBMP), keybmpind[KEY_P1_PFOOT].border[0], keybmpind[KEY_P1_SPFOOT].icon[0]);			  //�޸�   ADD_ICONBMP
			if (pfootf)
			{
				DrawColor_Gray1(keyd[KEY_P1_SPFOOT].xpis, keyd[KEY_P1_SPFOOT].ypis,
								keyd[KEY_P1_SPFOOT].xsize, keyd[KEY_P1_SPFOOT].ysize);
			}
		}
		// /[ѹ�������½�ָ��*end]**********************************************************

		// /[�о�ͼ�����*begin]**********************************************************
		// if (fixtureFlag)
		// {
		// 	fixtureFlag = 0;

		// 	if (P_SCP)
		// 	{
		// 		DrawButton(keyd[KEY_P1_FBAG], (U16 *)(STARTWLBMPADD), keybmpind[i].border[0], keybmpind[KEY_P1_FBAG].icon[0]);
		// 	}
		// 	else
		// 	{
		// 		Read_D(D_Address[SYS] + SYS_FIXTURE, 1, (U16 *)(&YREG[SYS_FIXTURE]));
		// 		fixdat = YREG[SYS_FIXTURE];
		// 		if (fixdat > 2)
		// 			fixdat = 0;
		// 		DrawButton(keyd[KEY_P1_FIXTURE], (U16 *)(STARTWLBMPADD), keybmpind[i].border[0], keybmpind[KEY_P1_FIXTURE].icon[0]);
		// 	}
		// }
		// /[�о�ͼ�����*end]**********************************************************

		// /[LEDͼ�����*begin]**********************************************************
		if (ledflag != (S_LEDF))
		{
			ledflag = S_LEDF; // 0x4000
			if (P_SCP)
			{
				DrawButton(keyd[KEY_P1_LED], (U16 *)(STARTWLBMPADD), keybmpind[KEY_P1_LED].border[0], keybmpind[KEY_P1_LED].icon[0]);
			}
			else
			{
				DrawButton(keyd[KEY_P1_LED], (U16 *)(STARTWLBMPADD),
						   (ledflag) ? keybmpind[KEY_P1_LED].border[1] : keybmpind[KEY_P1_LED].border[0],
						   (ledflag) ? keybmpind[KEY_P1_LED].icon[0] : keybmpind[KEY_P1_LED].icon[1]);
			}
		}
		// /[LEDͼ�����*end]**********************************************************

		// /[�������½�ͼ�����*begin]**********************************************************
		if (needlef != (U8)(S_NEEDLE))
		{
			needlef = (U8)S_NEEDLE; //0x20������  0 ������
			DrawButton(keyd[KEY_P1_PINUP], (U16 *)(ADD_ICONBMP), keybmpind[KEY_P1_PINUP].border[0],
					   (needlef == 0x20) ? keybmpind[KEY_P1_PINUP].icon[0] : keybmpind[KEY_P1_PINUP].icon[1]);
		}
		// /[�������½�ͼ�����*end]**********************************************************

		// /[��ȡ���»����б�*begin]**********************************************************
		if (g_updataIREG)
		{
			g_updataIREG = 0;
			ReadPatList(0, pno); //�����ڴ�����
		}
		// /[��ȡ���»����б�*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_P1)
		{
			plastno = MENU_P1;
			pnoindex = 0;
			poldno[pnoindex] = MENU_P1;
			pnoindex++;
			break;
		}

		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

#define KEY_P2_SCRNSVR 1
#define KEY_P2_UPC 2		//2 ����
#define KEY_P2_DOC 3		//3 ����
#define KEY_P2_INPUT 4		//4 ������
#define KEY_P2_OUTPUT 5		//5 ������
#define KEY_P2_FPROG 6		//6 �����޸�
#define KEY_P2_PINUP 7		//7 �������½�
#define KEY_P2_CUT 8		//8 ����
#define KEY_P2_PFOOT 9		//9 ��ѹ�������½�
#define KEY_P2_MENU 10		//10 �˵�
#define KEY_P2_ORG 11		//11 �ص�ԭ��
#define KEY_P2_SPFOOT 12	//12 ��ѹ���趨
#define KEY_P2_ORGSUB 13	//13 ԭ������
#define KEY_P2_SLFORCE 14	//14 ����
#define KEY_P2_FIXTURE 15	//15 �о�
#define KEY_P2_FBAG 16		//16 �۴�
#define KEY_P2_BACK 17		//17 ��һҳ
#define KEY_P2_WIND 18		//18 ����
#define KEY_P2_THRED 19		//19 ����
#define KEY_P2_MOVEUP 20	//20 ����
#define KEY_P2_MOVEDOWN 21	//21 ����
#define KEY_P2_MOVELEFT 22	//22 ����
#define KEY_P2_MOVERIGHT 23 //23 ����
#define KEY_P2_MOVESPEED 24 //24 ѹ���ٶ�

#define KEY_P2_MAX 25

#define RECT_P2_UPC 1		//�������ֿ�
#define RECT_P2_DOC 2		//�������ֿ�
#define RECT_P2_XPOS_TIT 3	//ѹ��X��λ��
#define RECT_P2_YPOS_TIT 4	//ѹ��Y��λ��
#define RECT_P2_ANGLE_TIT 5 //��Ƕ�
#define RECT_P2_XPOS_STR 6	//ѹ��X��λ��
#define RECT_P2_YPOS_STR 7	//ѹ��Y��λ��
#define RECT_P2_ANGLE_STR 8 //��Ƕ�
#define RECT_P2_GROUP 9		//�����Ϣ

#define RECT_P2_MAX 10

void Menu_P2(void)
{

	U32 i;
	U16 times50ms = 0xffff;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****������ţ��������ƣ������б���ز���begin****/
	// U16 static tmpFno;									 //��ǰ������ϢHREG[0]�ļ�λ��(�����),������Ż���
	// U16 tmpS_2HOME = 0, oldtmpS_2HOME = 0;				 //ϵͳ״̬����-�ص�ԭ��
	// U16 tmp_s_2hd = (U16)(S_2HD);						 //��ԭ��λ�û���
	//0���������й����ߣ�1���ػ泬�����ֵ�������� 2���ػ��˻ص��ߣ�3���������й���������ߣ�
	U8 tmpGroupNO = g_GroupNO, lastGroupNO = g_GroupNO; //��ǰ���ģʽ��ţ���һ�����ģʽ���
	/****������ţ��������ƣ������б���ز���end****/

	/****��־λ��ز���begin****/
	//U8 threadflag = 0;   //����ģʽ��־  0������   1���������봩��ģʽ  2�����ڴ���ģʽ
	//	S32 vpl,vpr,vpu,vpd;
	U32 tmp_upcount = 0xffffffff, tmp_dncount = 0xffffffff;
	//	U8 xmove =0,ymove =0;
	//	U8 tozero = 1;		  //��ԭ���־λ
	// U8 cutFlag = 0;		  //���߱�־
	U16 pfootf = 0xffff;  //ѹ�������½���־λ
	U16 needlef = 0xffff; //�������½�����
	U16 fixdat = 0;		  //�о߼����ݻ���
	// U8 fixtureFlag = 1;	  //�о�ͼ����±�־λ
	//	U8 enLF	=0;		//���߰�����־
	U8 flagOrgSub = 0; //ԭ���������ı�־λ
	// U8 NeedType = 0, oNeedType = 0, NeedType1 = 1; //�������
	U16 chPartNumNow = 0xfff; //��ǰ�ӹ��ӻ�����
	U8 flagPartNum = 0;		  //�ӻ�����
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_P2_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},						 //1 ����
		{240, TYPE_Y2, TYPE_SIZE17, TYPE_SIZE7}, //2 ����
		{410, TYPE_Y2, TYPE_SIZE17, TYPE_SIZE7}, //3 ����

		{TYPE_X1, 128, KEY_X1, KEY_Y1},								   //4 ������
		{TYPE_X1, 128 + KEY_Y1 + SAFE_DIST9, KEY_X1, KEY_Y1},		   //5 ������
		{TYPE_X1, 128 + KEY_Y1 * 2 + SAFE_DIST9 * 2, KEY_X1, KEY_Y1},  //6 �����޸�
		{TYPE_X11, 128, KEY_X1, KEY_Y1},							   //7 �������½�
		{TYPE_X11, 128 + KEY_Y1 + SAFE_DIST9, KEY_X1, KEY_Y1},		   //8 ����
		{TYPE_X11, 128 + KEY_Y1 * 2 + SAFE_DIST9 * 2, KEY_X1, KEY_Y1}, //9 ��ѹ�������½�

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //10 �˵�
		{TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},	 //11 �ص�ԭ��
		{TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},	 //12 ��ѹ���趨
		{TYPE_X4, TYPE_Y4, KEY_X2, KEY_Y2},	 //13 ԭ������
		{TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},	 //14 ����
		{TYPE_X6, TYPE_Y4, KEY_X1, KEY_Y1},	 //15 �о�
		{TYPE_X7, TYPE_Y4, KEY_X1, KEY_Y1},	 //16 �۴�
		{TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},	 //17 ��������
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //18 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //19 ����

		{TYPE_X12 - KEY_X1 / 2, TYPE_Y12 - KEY_Y1 - SAFE_DIST6, KEY_X1, KEY_Y1}, //20 ����
		{TYPE_X12 - KEY_X1 / 2, TYPE_Y12 + KEY_Y1 + SAFE_DIST6, KEY_X1, KEY_Y1}, //21 ����
		{TYPE_X12 - KEY_X1 * 3 / 2 - SAFE_DIST6, TYPE_Y12, KEY_X1, KEY_Y1},		 //22 ����
		{TYPE_X12 + KEY_X1 / 2 + SAFE_DIST6, TYPE_Y12, KEY_X1, KEY_Y1},			 //23 ����
		{TYPE_X12 - KEY_X1 / 2, TYPE_Y12, KEY_X1, KEY_Y1},						 //24 ѹ���ٶ�
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},														   //1 ����
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {4, 0, 0, 0}, {0}}, //2 ����
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {5, 0, 0, 0}, {0}}, //3 ����

		{{KEY_Silver}, {0}, {63}, {8, 0, 0, 0}, {0}},		//4 ������
		{{KEY_Silver}, {0}, {64}, {9, 0, 0, 0}, {0}},		//5 ������
		{{KEY_Silver}, {0}, {65}, {10, 0, 0, 0}, {0}},		//6 �����޸�
		{{KEY_Silver}, {0}, {21, 23}, {13, 14, 0, 0}, {0}}, //7 �������½�
		{{KEY_Silver}, {0}, {34}, {15, 0, 0, 0}, {0}},		//8 ����
		{{KEY_Silver}, {0}, {17, 19}, {16, 17, 0, 0}, {0}}, //9 ��ѹ�������½�
		{{KEY_Silver}, {0}, {1}, {18, 0, 0, 0}, {0}},		//10 �˵�

		{{KEY_Silver}, {0}, {3}, {10, 0, 0, 0}, {0}},			 //11 �ص�ԭ��
		{{KEY_Silver}, {0}, {14}, {19, 0, 0, 0}, {0}},			 //12 ��ѹ���趨
		{{KEY_Silver}, {0}, {591, 594}, {23, 22, 21, 0}, {0}},	 //13 ԭ������
		{{KEY_Silver}, {0}, {28}, {24, 14, 0, 0}, {0}},			 //14 ����
		{{KEY_Silver}, {0}, {25, 26, 27}, {25, 27, 26, 0}, {0}}, //15 �о�
		{{KEY_Silver}, {0}, {45}, {28, 0, 0, 0}, {0}},			 //16 �۴�
		{{KEY_Silver}, {0}, {40}, {31, 0, 0, 0}, {0}},			 //17 ��������
		{{KEY_Silver}, {0}, {36}, {32, 0, 0, 0}, {0}},			 //18 ����
		{{KEY_Silver}, {0}, {35}, {33, 0, 0, 0}, {0}},			 //19 ����
		{{KEY_Silver}, {0}, {56}, {34, 0, 0, 0}, {0}},			 //20 ����
		{{KEY_Silver}, {0}, {61}, {35, 0, 0, 0}, {0}},			 //21 ����
		{{KEY_Silver}, {0}, {58}, {36, 0, 0, 0}, {0}},			 //22 ����
		{{KEY_Silver}, {0}, {59}, {37, 0, 0, 0}, {0}},			 //23 ����
		{{KEY_Silver}, {Color_Black}, {51}, {38, 0, 0, 0}, {0}}, //24 ѹ���ٶ�

	};

	const RECTPAD rect[RECT_P2_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
		{240, 85, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},																		//1 �������ֿ�
		{410, 85, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},																		//2 �������ֿ�
		{TYPE_X1, TYPE_Y1, TYPE_SIZE8, TYPE_SIZE4, {Color_Black}, {Color_White}, {41}},																//3 ѹ��X��λ��
		{TYPE_X1, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE8, TYPE_SIZE4, {Color_Black}, {Color_White}, {42}},									//4 ѹ��Y��λ��
		{TYPE_X1, TYPE_Y1 + TYPE_SIZE4 * 2 + SAFE_DIST3 * 2, TYPE_SIZE8, TYPE_SIZE4, {Color_Black}, {Color_White}, {43}},							//5 ��Ƕ�
		{TYPE_X1 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE14, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},									//6 ѹ��X��λ��
		{TYPE_X1 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE14, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},			//7 ѹ��Y��λ��
		{TYPE_X1 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y1 + TYPE_SIZE4 * 2 + SAFE_DIST3 * 2, TYPE_SIZE14, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //8 ��Ƕ�
		{TITLE_XPOS_GROUP, SAFE_DIST4, TIT_TIME_XPOS - TITLE_XPOS_GROUP, TYPE_SIZE4, {COLOR_TITLE_BG}, {Color_Red}, {39}},								//9 �����Ϣ���ޣ�����
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_P2); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();
	pnoindex = 1;
	tmppnoindex = pnoindex;
	if (P_LAN)
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME_E;
	}
	else
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME;
	}

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************

	/************�۴��������***************/
	//��ȡ�û�����_�ж��Ƿ�Ϊ�۴���
	Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 1, (U16 *)(&YREG[SYS_MACHINE_TYPE]));

	Read_D(D_Address[SYS] + SYS_UPCOUNT, 4, (U16 *)(&YREG[SYS_UPCOUNT]));
	tmp_upcount = (upcount_now_h << 16 | upcount_now_l);
	tmp_dncount = (dncount_now_h << 16 | dncount_now_l);
	if (g_UdZhouFlag == 3)
	{
		// fixtureFlag = 1;
		Read_D(D_Address[SYS] + SYS_CHANGENEEDLE, 1, (U16 *)(&YREG[SYS_CHANGENEEDLE])); //
																						// NeedType = YREG[SYS_CHANGENEEDLE] & 0xff;
																						// NeedType1 = NeedType;
																						// oNeedType = (YREG[SYS_CHANGENEEDLE] >> 8) & 0xff;
	}
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //P2
	//��������
	for (i = 1; i < RECT_P2_MAX; i++)
	{
		if (i == RECT_P2_GROUP) //���ģʽ_�Ӻ�ģʽ��Ϣ
		{
			if (g_GroupType == 1 || g_GroupType == 2)
			{ //�����
				Draw24x24Str(rect[i].xpis, rect[i].ypis, MEM_TITLE_ADD + ((U32)rect[i].index[0] + g_GroupType) * TitleLengthMax,
							 SheetPage, rect[i].index[0] + g_GroupType, rect[i].colorT[0]); //���ģʽ���Ӻ�ģʽ

				Draw24X24FStr(rect[i].xpis + 12 * 9, rect[i].ypis, (char *)g_GroupName, rect[i].colorT[0]);
				DrawMFU32(rect[i].xpis + 12 * 18, rect[i].ypis, g_GroupNO, 2, rect[i].colorT[0]);
				Draw24X24FStr(rect[i].xpis + 12 * 18 + 12 * 2, rect[i].ypis, "/", rect[i].colorT[0]);
				DrawMFU32(rect[i].xpis + 12 * 18 + 12 * 3, rect[i].ypis, g_GroupTol, 2, rect[i].colorT[0]);
			}
		}
		else if (i == RECT_P2_XPOS_TIT || i == RECT_P2_YPOS_TIT || i == RECT_P2_ANGLE_TIT)
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);

			DrawFFStr(rect[i].xpis + (rect[i].xsize - 16 * 2) / 2,
					  rect[i].ypis + (rect[i].ysize - 16) / 2,
					  MEM_TITLE_ADD + ((U32)rect[i].index[0]) * TitleLengthMax,
					  rect[i].colorT[0]);
		}
		else
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		}
	}
	DrawMFU32(rect[RECT_P2_UPC].xpis + SAFE_DIST4,
			  rect[RECT_P2_UPC].ypis + SAFE_DIST4,
			  tmp_upcount, 5, rect[RECT_P2_UPC].colorT[0]);
	Draw24X24FStr(rect[RECT_P2_UPC].xpis + SAFE_DIST4 + 13 * 5,
				  rect[RECT_P2_UPC].ypis + SAFE_DIST4, "/", rect[RECT_P2_UPC].colorT[0]);
	DrawMFU32(rect[RECT_P2_UPC].xpis + SAFE_DIST4 + 13 * 6,
			  rect[RECT_P2_UPC].ypis + SAFE_DIST4,
			  (upcount_set_h << 16 | upcount_set_l), 5, rect[RECT_P2_UPC].colorT[0]); //��������

	DrawMFU32(rect[RECT_P2_DOC].xpis + SAFE_DIST4,
			  rect[RECT_P2_DOC].ypis + SAFE_DIST4,
			  tmp_dncount, 5, rect[RECT_P2_DOC].colorT[0]);
	Draw24X24FStr(rect[RECT_P2_DOC].xpis + SAFE_DIST4 + 13 * 5,
				  rect[RECT_P2_DOC].ypis + SAFE_DIST4, "/", rect[RECT_P2_DOC].colorT[0]);
	DrawMFU32(rect[RECT_P2_DOC].xpis + SAFE_DIST4 + 13 * 6,
			  rect[RECT_P2_DOC].ypis + SAFE_DIST4,
			  (dncount_set_h << 16 | dncount_set_l), 5, rect[RECT_P2_DOC].colorT[0]); //��������

	//����������ʾ
	for (i = 2; i < KEY_P2_MAX; i++)
	{
		if ((i == KEY_P2_FBAG) || i == KEY_P2_FIXTURE ||
			i == KEY_P2_ORGSUB || i == KEY_P2_SLFORCE) //��Ч����
		{
			continue;
		}
		else if (i == KEY_P2_UPC || i == KEY_P2_DOC) //������ɫ���ְ���
		{
			U32 tempCnt = 0;
			if (i == KEY_P2_UPC)
			{
				tempCnt = upcntonoff_flag;
			}
			else if (i == KEY_P2_DOC)
			{
				tempCnt = dncntonoff_flag;
			}
			DrawButtonSingleColor(keyd[i], tempCnt ? keybmpind[i].border[1] : keybmpind[i].border[0],
								  tempCnt ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
			Draw24X24FFStr(keyd[i].xpis + (keyd[i].xsize - 24 * 3) / 2,
						   keyd[i].ypis + (keyd[i].ysize - 24) / 2,
						   MEM_TITLE_ADD + ((U32)keybmpind[i].titleStr[0]) * TitleLengthMax,
						   tempCnt ? keybmpind[i].textColor[1] : keybmpind[i].textColor[0]);
		}
		else //���水ť
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
		if (i == KEY_P2_MOVESPEED)
		{
			DrawFU32Z(keyd[i].xpis + keyd[i].xsize / 2 - 4, keyd[i].ypis + SAFE_DIST1,
					  PROMG_DATA[11][2], 1, keybmpind[i].textColor[0]);
		}
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

	E_RUNDIS();												//��ֹ����
	Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE); //��д״̬�Ĵ��� д�뾲ֹ״̬
	ViewPage(usepageno);									//������ʾ����

	Read_D(D_Address[SYS] + SYS_LISTTYPE, 1, (U16 *)(&YREG[SYS_LISTTYPE])); // �������ڴ���������
	if (YREG[SYS_LISTTYPE] != 0)
	{ //��ǰ�ļ��������������ڴ� �����������ڴ�����
		ReadPatList(0, MENU_P2);
	}
	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����
		if ((S_RUN) && (keydata)) //����״̬�£������١����١���һҳ�� ���� �ڰ����ɿ�ʱ ��Ч
		{
			//if((keydata<KEY_P1_UP)||((keydata>KEY_P1_DOWN)&&(keydata<KEY_P1_NEXT))){keydata = 0;}
			if ((keydata != KEY_P2_BACK) && (keydata != 0xff))
			{
				keydata = 0;
			}
		}
		if ((keydata == KEY_P2_FBAG) || keydata == KEY_P2_FIXTURE ||
			keydata == KEY_P2_ORGSUB || keydata == KEY_P2_SLFORCE) //��Ч����
		{
			keydata = 0;
		}

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			if ((keyon == KEY_P2_MOVEUP) || (keyon == KEY_P2_MOVEDOWN) ||
				(keyon == KEY_P2_MOVELEFT) || (keyon == KEY_P2_MOVERIGHT)) //���������������ܵİ���
			{
			}
			else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			if (((keydata == KEY_P2_SLFORCE) && (TENSIONBT == 0)) ||
				((keydata == KEY_P2_SPFOOT) && ((PFOOTBT == 0) || (pfootf)))) //�ض�����²���Ч����
			{
				keydata = 0;
			}
			else
			{
				if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
				{
					keyon = keydata;
					if (keyon == KEY_P2_SCRNSVR) //�ް�����Ч
					{
					}
					// else if (keyon == KEY_P2_DOC) //�ǳ��水��-//������ɫ���ְ���
					// {
					// 	DrawButtonOn(keyd[keyon],
					// 				 dncntonoff_flag ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
					// }
					// else if (keyon == KEY_P2_UPC)
					// {
					// 	DrawButtonOn(keyd[keyon],
					// 				 upcntonoff_flag ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
					// }
					else
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
						beeponf = 1;

						if (g_keyBellT) //����ʱ����������ʱ��
										// if (keyon != KEY_P2_CUT)
							Beep_On(g_keyBellT);

						switch (keyon) //�������±�����ʾ����ʾ
						{
						case KEY_P2_PFOOT:
							if (pfootf == 0)
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						case KEY_P2_FIXTURE:
							if (P_SCP)
							{
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							}
							else
							{
								DrawFMenuStr(fixdat == 0 ? keybmpind[keyon].titleStr[1] : keybmpind[keyon].titleStr[2]);
							}
							break;
						case KEY_P2_PINUP:
							if (needlef == 0x20)
								DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;

						default:
							DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						}

						delay = KEYFT;
						if (keyon == KEY_P2_PFOOT)
						{
							delay = 800;
						}
						else if (keyon == KEY_P2_CUT)
						{
							delay = 800;
						}
					}
					else if (beeponf == 1) //�ڶ��ν���
					{
						beeponf = 2;
						if ((keyon == KEY_P2_MOVEUP) || (keyon == KEY_P2_MOVEDOWN) ||
							(keyon == KEY_P2_MOVELEFT) || (keyon == KEY_P2_MOVERIGHT))
						{
							delay = DELAY_MOVE;
						}
						else
							delay = KEYNT;
						// if (keyon == KEY_P2_CUT)
						// {
						// 	cutFlag = 1;
						// 	Beep_On(BELLT);
						// }
					}
					else if (beeponf < 20)
					{
						beeponf++;
					}

					if ((keyon == KEY_P2_MOVEUP) || (keyon == KEY_P2_MOVEDOWN) ||
						(keyon == KEY_P2_MOVELEFT) || (keyon == KEY_P2_MOVERIGHT)) //���г����������ܰ����Ĺ���
					{
						keyoff = keyon;
						key_on_flag1 = 1;
					}
					Times = 0; //��ǰ
				}
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
			case KEY_P2_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_P2_MENU:
				pno = MENU_MENU;
				break;
			case KEY_P2_UPC:
				pno = MENU_COUNTER;
				g_para0 = 0;
				break;
			case KEY_P2_DOC:
				pno = MENU_COUNTER;
				g_para0 = 1;
				break;
			case KEY_P2_PFOOT:
				Send_Pfoot(0, MENU_P1, 0);
				break;

			case KEY_P2_ORG:
				if (P_HPK != 3)
				{
					if (Go_Home(MENU_P2, 0))
					{
						g_feedct = 0;
					}
				}
				break;
			case KEY_P2_ORGSUB:
				if (g_orgsub_f == 6)
					g_orgsub_f = 7;
				else
					g_orgsub_f = 6;
				flagOrgSub = 1;
				break;
			case KEY_P2_SLFORCE:
				pno = MENU_LINEFORCE;
				g_para0 = 0;
				break;
			case KEY_P2_SPFOOT:
				pno = MENU_PFOOT;
				g_para0 = 1;
				break;
			case KEY_P2_BACK:
				pno = MENU_P1;
				if ((g_orgsub_f != SREG[3]) && (F_FILETYPE == PAT_MIT))
				{
					Write_D(D_Address[PTDATA] + 3, 1, (U16 *)(&SREG[3]));
				}
				break;

			case KEY_P2_CUT:
				if (1) //cutFlag
				{
					if (Write_Cmd(C_CUT, 0, 0) == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						break;
					}
					if (Read_Status() == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
					}
					else if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
					}
				}
				break;

			case KEY_P2_PINUP:
				Write_Cmd1(C_NEEDLEUP, 0, 0);
				break;
			case KEY_P2_FIXTURE:
				// if (P_SCP)
				// {
				// 	if (Write_Cmd(C_BCLAMP, 0, 0) == 0)
				// 	{	//��ѹ��
				// 		//pno = MENU_C2_HELP;hleppara = 15;break;
				// 	}
				// 	if (Read_Status() == 0)
				// 	{
				// 		//pno = MENU_C2_HELP;hleppara = 15;break;
				// 	}
				// 	else if (DREG[D_CMDSTATUS] != 100)
				// 	{
				// 		//pno = MENU_C2_HELP;hleppara = 16;break;
				// 	}
				// }
				// else
				// {
				// 	if (Write_Cmd(C_FIXTURE, 0, 0) == 0)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 15;
				// 		break;
				// 	}
				// 	if (Read_Status() == 0)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 15;
				// 	}
				// 	else if (DREG[D_CMDSTATUS] != 100)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 16;
				// 	}
				// 	fixtureFlag = 1;
				// }
				break;

			case KEY_P2_WIND:
				Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
				if ((S_HOME) || (S_NEWHOME) || (S_2HOME))
				{
					if (pfootf == 0)
					{
						Send_Pfoot(0, MENU_P1, 0);
					}
					pno = MENU_WIND;
					E_WINDEN();
					i = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
					if (i == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						break;
					}
				}
				else
				{
					pno = MENU_RWHELP;
					hleppara = 17;
				}
				break;

			case KEY_P2_THRED:
				pno = MENU_THREAD;

				E_RUNEN(); //��ֹ����
				Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
				break;

			case KEY_P2_FPROG:
				E_RUNEN();
				Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
				pno = MENU_FPROG;
				poldno[pnoindex++] = MENU_P2;
				break;
			case KEY_P2_INPUT:
				E_RUNEN();
				if (Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE) == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					break;
				}
				pno = MENU_INPUTSTATUS;
				poldno[pnoindex] = MENU_P2;
				pnoindex++;
				break;
			case KEY_P2_OUTPUT:
				E_RUNEN();
				if (Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE) == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					break;
				}
				pno = MENU_OUTPUTSTATUS;
				poldno[pnoindex] = MENU_P2;
				pnoindex++;
				break;
				// case KEY_P2_SREG_UP: //�޸�
				// 	if (Write_Cmd(C_UPSREG, 0, 0) == 0)
				// 	{ //���ͼӹ�ͼ�ζκ�����ָ��
				// 		pno = MENU_RWHELP;
				// 		hleppara = 16;
				// 		break;
				// 	}
				// 	if (Read_Status() == 0)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 15;
				// 		break;
				// 	}
				// 	else if (DREG[D_CMDSTATUS] != 100)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 16;
				// 		break;
				// 	}
				// 	if (Read_D(D_Address[HEAD] + 12, 1, (U16 *)(&HREG[12])) == 0)
				// 	{ //��ȡ��ǰ�ӹ��κ�
				// 		pno = MENU_RWHELP;
				// 		hleppara = 15;
				// 		break;
				// 	}
				// 	flagPartNum = 1;

				// 	break;

				// case KEY_P2_SREG_DN:
				// 	if (Write_Cmd(C_DOWNSREG, 0, 0) == 0)
				// 	{ //���ͼӹ�ͼ�ζκ�����ָ��
				// 		pno = MENU_RWHELP;
				// 		hleppara = 16;
				// 		break;
				// 	}
				// 	if (Read_Status() == 0)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 15;
				// 		break;
				// 	}
				// 	else if (DREG[D_CMDSTATUS] != 100)
				// 	{
				// 		pno = MENU_RWHELP;
				// 		hleppara = 16;
				// 		break;
				// 	}
				// 	if (Read_D(D_Address[HEAD] + 12, 1, (U16 *)(&HREG[12])) == 0)
				// 	{ //��ȡ��ǰ�ӹ��κ�
				// 		pno = MENU_RWHELP;
				// 		hleppara = 15;
				// 		break;
				// 	}
				// 	flagPartNum = 1;
				// 	break;
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			if (keyon == KEY_P2_SCRNSVR) //����Ҫ��ʾ�İ���
			{
			}
			// else if (keyon == KEY_P2_DOC) //�ǳ��水��-//������ɫ���ְ���
			// {
			// 	DrawButtonOff(keyd[keyon],
			// 				  dncntonoff_flag ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
			// }
			// else if (keyon == KEY_P2_UPC)
			// {
			// 	DrawButtonOff(keyd[keyon],
			// 				  upcntonoff_flag ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
			// }
			//�ǳ��水��-----��
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			// cutFlag = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ��P2
			Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);

			if (g_GroupType == 1 || g_GroupType == 2)
			{ //�����
				Draw24x24Str(rect[RECT_P2_GROUP].xpis, rect[RECT_P2_GROUP].ypis, MEM_TITLE_ADD + ((U32)rect[RECT_P2_GROUP].index[0] + g_GroupType) * TitleLengthMax,
							 SheetPage, rect[RECT_P2_GROUP].index[0] + g_GroupType, rect[RECT_P2_GROUP].colorT[0]); //���ģʽ���Ӻ�ģʽ

				Draw24X24FStr(rect[RECT_P2_GROUP].xpis + 12 * 9, rect[RECT_P2_GROUP].ypis, (char *)g_GroupName, rect[RECT_P2_GROUP].colorT[0]);
				DrawMFU32(rect[RECT_P2_GROUP].xpis + 12 * 18, rect[RECT_P2_GROUP].ypis, g_GroupNO, 2, rect[RECT_P2_GROUP].colorT[0]);
				Draw24X24FStr(rect[RECT_P2_GROUP].xpis + 12 * 18 + 12 * 2, rect[RECT_P2_GROUP].ypis, "/", rect[RECT_P2_GROUP].colorT[0]);
				DrawMFU32(rect[RECT_P2_GROUP].xpis + 12 * 18 + 12 * 3, rect[RECT_P2_GROUP].ypis, g_GroupTol, 2, rect[RECT_P2_GROUP].colorT[0]);
			}
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 1000)
		{
			Timerms = 0;

			if ((g_GroupType == 1) || (g_GroupType == 2))
			{ // ��ȡ��ǰ���ģʽ����ϱ�ţ���ǰ���ڵĻ�����ţ���ǰ��Ļ�������
				Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE]));
				if (lastGroupNO != g_GroupNO) //���ģʽ��
				{
					lastGroupNO = g_GroupNO;
					tmpGroupNO = g_GroupNO;

					//��ʾ��ǰ���
					BmpFillRect(rect[RECT_P2_GROUP].xpis + 120 + 120, rect[RECT_P2_GROUP].ypis, 28, 24);
					DrawMFU32(rect[RECT_P2_GROUP].xpis + 120 + 120, rect[RECT_P2_GROUP].ypis, tmpGroupNO, 2, rect[RECT_P2_GROUP].colorT[0]);
				}
				if (g_GroupType == 2) //�Ӻ�ģʽ��
				{
					Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ

					// if (tmpFno != F_IND_NO)										//ͼ�α仯
					// {
					// 	Read_D(D_Address[PTDATA], (F_FILELEN), (U16 *)(&SREG[0])); // ����ǰ�����ļ�

					// }
				}
			}

			Read_D(D_Address[SYS] + SYS_SPEED, 5, (U16 *)(&YREG[SYS_SPEED]));
			Read_D(D_Address[HEAD] + HREG_FEED_C, 1, (U16 *)(&HREG[HREG_FEED_C]));
			Read_D(D_Address[SYS] + SYS_RUN_TIMER, 2, (U16 *)(&YREG[SYS_RUN_TIMER]));
			if ((F_PT_PARTSUM > 1) && ((P_POF == 1) || (P_POF == 2))) //ͼ�η���ӹ���Ч
			{
				if (Read_D(D_Address[HEAD] + 12, 1, (U16 *)(&HREG[12])) == 0) //��ȡ��ǰ�ӹ��κ�
				{
					pno = MENU_RWHELP;
					hleppara = 15;
				}
				if (chPartNumNow != F_PT_PARTNUM)
					flagPartNum = 1;
			}
			if (g_UdZhouFlag == 3)
			{
				Read_D(D_Address[SYS] + SYS_CHANGENEEDLE, 1, (U16 *)(&YREG[SYS_CHANGENEEDLE]));
				// NeedType = YREG[SYS_CHANGENEEDLE] & 0xff;
				// oNeedType = (YREG[SYS_CHANGENEEDLE] >> 8) & 0xff;
				// if (NeedType != NeedType1)
				// {
				// 	NeedType1 = NeedType;
				// 	// fixtureFlag = 1;
				// }
			}
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(MENU_P2);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			U32 wTimeTemp;

			ErrorMsg(MENU_P2);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}

			if (S_RUN)
			{

				wTimeTemp = Timer_ms >> 5;	//Timer_ms/2^5
				if (wTimeTemp != times50ms) //��ʱ������������
				{
					times50ms = wTimeTemp;
				}
			}
			else
			{

				wTimeTemp = Timer_ms >> 8; //Timer_ms/2^8
				if (wTimeTemp != times50ms)
				{
					U32 wReadACK;
					U32 wPatternNo, wPatternNum;
					times50ms = wTimeTemp;

					if (S_RUN)
					{
					}
					else
					{
						//��ȡ�����������
						Read_D(D_Address[SYS] + SYS_DNCOUNT, 2, (U16 *)(&YREG[SYS_DNCOUNT]));
						Read_D(D_Address[SYS] + SYS_UPCOUNT, 2, (U16 *)(&YREG[SYS_UPCOUNT]));

						// Read_D(D_Address[SYS] + SYS_SPEED, 1, (U16 *)(&YREG[SYS_SPEED]));
						wReadACK = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); //��ȡ64λ���ݣ���ȫ����HREG[]
						if (wReadACK)															//��ȡ�ɹ�
						{
							if (F_IND_NO != g_selectdisno)
							{
								wPatternNo = F_IND_NO;							   //��ȡ��ǰ�������
								if ((wPatternNo >= MINFILENO) && (i <= MAXFILENO)) //�����ȷ
								{
									wPatternNo = (wPatternNo - MINFILENO) * FileIndexSize;						  //��ȡ��ǰ�����������ڻ���������λ��
									wPatternNum = (((U32)(IREG2[wPatternNo + 1]) << 16) + IREG2[wPatternNo]) / 2; //��ȡ��ǰ�������ļ���

									if ((wPatternNum >= 0x40) && (wPatternNum <= MAXSREG)) //������С��Ϣ��ȷ
									{
										wReadACK = Read_D(D_Address[PTDATA], wPatternNum, (U16 *)(&SREG[0])); // ����ǰ��������

										if (wReadACK) //��ȡ�ɹ�
										{
											coldstart = 0;
											g_init_flag = 1;
											g_selectdisno = F_IND_NO;
											g_oldselectdisno = g_selectdisno;
											g_data_flash = 1;
										}
									}
								}
							}
						}
						Read_D(D_Address[HEAD] + HREG_FEED_C, 1, (U16 *)(&HREG[HREG_FEED_C]));
						Read_D(D_Address[SYS] + SYS_RUN_TIMER, 2, (U16 *)(&YREG[SYS_RUN_TIMER]));
					}
				}
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[ԭ�����*begin]**********************************************************
		if (flagOrgSub)
		{
			flagOrgSub = 0;

			if (F_FILETYPE == PAT_MIT) //����
			{
				U16 chOffsetLen = 0;
				U16 chCheckSum = 0;
				g_data_flash = 1;
				if (g_orgsub_f == 7)
				{
					DrawButton(keyd[KEY_P2_ORGSUB], (U16 *)(ADD_ICONBMP),
							   keybmpind[KEY_P2_ORGSUB].border[0], keybmpind[KEY_P2_ORGSUB].icon[1]);

					//����X��ƫ������
					DrawFillRect(rect[RECT_P2_XPOS_STR].xpis + TYPE_SIZE21,
								 rect[RECT_P2_XPOS_STR].ypis,
								 rect[RECT_P2_XPOS_STR].xsize - TYPE_SIZE21,
								 rect[RECT_P2_XPOS_STR].ysize, rect[RECT_P2_XPOS_STR].colorB[0]);
					DrawF16Str(rect[RECT_P2_XPOS_STR].xpis + TYPE_SIZE21,
							   rect[RECT_P2_XPOS_STR].ypis + SAFE_DIST4,
							   "(", rect[RECT_P2_XPOS_STR].colorT[0]);
					DrawMFS32P(rect[RECT_P2_XPOS_STR].xpis + TYPE_SIZE21 + TYPE_SIZE20,
							   rect[RECT_P2_XPOS_STR].ypis + SAFE_DIST4,
							   (S16)(SREG[6]), 5, 1, rect[RECT_P2_XPOS_STR].colorT[0]);
					DrawF16Str(rect[RECT_P2_XPOS_STR].xpis + TYPE_SIZE21 + TYPE_SIZE21 + TYPE_SIZE20,
							   rect[RECT_P2_XPOS_STR].ypis + SAFE_DIST4,
							   ")", rect[RECT_P2_XPOS_STR].colorT[0]);
					//����Y��ƫ������
					DrawFillRect(rect[RECT_P2_YPOS_STR].xpis + TYPE_SIZE21,
								 rect[RECT_P2_YPOS_STR].ypis,
								 rect[RECT_P2_YPOS_STR].xsize - TYPE_SIZE21,
								 rect[RECT_P2_YPOS_STR].ysize, rect[RECT_P2_YPOS_STR].colorB[0]);
					DrawF16Str(rect[RECT_P2_YPOS_STR].xpis + TYPE_SIZE21,
							   rect[RECT_P2_YPOS_STR].ypis + SAFE_DIST4,
							   "(", rect[RECT_P2_YPOS_STR].colorT[0]);
					DrawMFS32P(rect[RECT_P2_YPOS_STR].xpis + TYPE_SIZE21 + TYPE_SIZE20,
							   rect[RECT_P2_YPOS_STR].ypis + SAFE_DIST4,
							   (S16)(SREG[7]), 5, 1, rect[RECT_P2_YPOS_STR].colorT[0]);
					DrawF16Str(rect[RECT_P2_YPOS_STR].xpis + TYPE_SIZE21 + TYPE_SIZE21 + TYPE_SIZE20,
							   rect[RECT_P2_YPOS_STR].ypis + SAFE_DIST4,
							   ")", rect[RECT_P2_YPOS_STR].colorT[0]);

					SREG[3] = 7;
					DREG[D_COMMAND] = C_NEWHOME; //C_GOHOME;
					DREG[D_CMDSTATUS] = 0;
					DREG[D_CMDPARA1] = SREG[6];
					DREG[D_CMDPARA2] = SREG[7];
					Write_D(D_Address[DISP] + D_COMMAND, 4, (U16 *)(&DREG[D_COMMAND]));
				}
				else if (g_orgsub_f == 6)
				{
					DrawButton(keyd[KEY_P2_ORGSUB], (U16 *)(ADD_ICONBMP),
							   keybmpind[KEY_P2_ORGSUB].border[0], keybmpind[KEY_P2_ORGSUB].icon[0]);
					//����X��ƫ������
					DrawFillRect(rect[RECT_P2_XPOS_STR].xpis + TYPE_SIZE21,
								 rect[RECT_P2_XPOS_STR].ypis,
								 rect[RECT_P2_XPOS_STR].xsize - TYPE_SIZE21,
								 rect[RECT_P2_XPOS_STR].ysize, rect[RECT_P2_XPOS_STR].colorB[0]);
					//����Y��ƫ������
					DrawFillRect(rect[RECT_P2_YPOS_STR].xpis + TYPE_SIZE21,
								 rect[RECT_P2_YPOS_STR].ypis,
								 rect[RECT_P2_YPOS_STR].xsize - TYPE_SIZE21,
								 rect[RECT_P2_YPOS_STR].ysize, rect[RECT_P2_YPOS_STR].colorB[0]);
					SREG[3] = 6;
					DREG[D_COMMAND] = C_GOHOME;
					DREG[D_CMDSTATUS] = 0;
					Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
				}

				chOffsetLen = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3; //2013.3.7
				for (i = 0; i < chOffsetLen; i++)
				{
					if (i != 0x20)
						chCheckSum += SREG[i];
				}
				SREG[0x20] = chCheckSum;

				Write_D(D_Address[PTDATA] + 3, 1, (U16 *)(&SREG[3]));
			}
		}
		// /[ԭ�����*end]**********************************************************

		if (flagPartNum)
		{
			flagPartNum = 0;
			chPartNumNow = F_PT_PARTNUM;
		}

		// /[����������ݸ���*begin]**********************************************************
		if (tmp_upcount != (upcount_now_h << 16 | upcount_now_l))
		{
			tmp_upcount = (upcount_now_h << 16 | upcount_now_l);
			DrawFillRect(rect[RECT_P2_UPC].xpis, rect[RECT_P2_UPC].ypis, rect[RECT_P2_UPC].xsize, rect[RECT_P2_UPC].ysize, rect[RECT_P2_UPC].colorB[0]);
			DrawMFU32(rect[RECT_P2_UPC].xpis + SAFE_DIST4,
					  rect[RECT_P2_UPC].ypis + SAFE_DIST4,
					  tmp_upcount, 5, rect[RECT_P2_UPC].colorT[0]);
			Draw24X24FStr(rect[RECT_P2_UPC].xpis + SAFE_DIST4 + 13 * 5,
						  rect[RECT_P2_UPC].ypis + SAFE_DIST4, "/", rect[RECT_P2_UPC].colorT[0]);
			DrawMFU32(rect[RECT_P2_UPC].xpis + SAFE_DIST4 + 13 * 6,
					  rect[RECT_P2_UPC].ypis + SAFE_DIST4,
					  (upcount_set_h << 16 | upcount_set_l), 5, rect[RECT_P2_UPC].colorT[0]); //��������
		}
		if (tmp_dncount != (dncount_now_h << 16 | dncount_now_l))
		{
			tmp_dncount = (dncount_now_h << 16 | dncount_now_l);
			DrawFillRect(rect[RECT_P2_DOC].xpis, rect[RECT_P2_DOC].ypis, rect[RECT_P2_DOC].xsize, rect[RECT_P2_DOC].ysize, rect[RECT_P2_UPC].colorB[0]);
			DrawMFU32(rect[RECT_P2_DOC].xpis + SAFE_DIST4,
					  rect[RECT_P2_DOC].ypis + SAFE_DIST4,
					  tmp_dncount, 5, rect[RECT_P2_DOC].colorT[0]);
			Draw24X24FStr(rect[RECT_P2_DOC].xpis + SAFE_DIST4 + 13 * 5,
						  rect[RECT_P2_DOC].ypis + SAFE_DIST4, "/", rect[RECT_P2_DOC].colorT[0]);
			DrawMFU32(rect[RECT_P2_DOC].xpis + SAFE_DIST4 + 13 * 6,
					  rect[RECT_P2_DOC].ypis + SAFE_DIST4,
					  (dncount_set_h << 16 | dncount_set_l), 5, rect[RECT_P2_DOC].colorT[0]); //��������
		}
		// /[����������ݸ���*end]**********************************************************

		// /[ѹ�������½�ָ��*begin]**********************************************************
		if (pfootf != (U16)(S_PFOOT))
		{
			pfootf = (U16)(S_PFOOT);
			DrawButton(keyd[KEY_P2_PFOOT], (U16 *)(ADD_ICONBMP),
					   keybmpind[KEY_P2_PFOOT].border[0],
					   (pfootf == 0) ? (keybmpind[KEY_P2_PFOOT].icon[0]) : (keybmpind[KEY_P2_PFOOT].icon[1])); //�޸�   ADD_ICONBMP
			DrawButton(keyd[KEY_P2_SPFOOT], (U16 *)(ADD_ICONBMP),
					   keybmpind[KEY_P2_SPFOOT].border[0],
					   keybmpind[KEY_P2_SPFOOT].icon[0]); //�޸�   ADD_ICONBMP
			if (pfootf)
			{
				DrawColor_Gray1(keyd[KEY_P2_SPFOOT].xpis, keyd[KEY_P2_SPFOOT].ypis,
								keyd[KEY_P2_SPFOOT].xsize, keyd[KEY_P2_SPFOOT].ysize);
			}
		}
		// /[ѹ�������½�ָ��*end]**********************************************************

		// /[�������½�ͼ�����*begin]**********************************************************
		if (needlef != (U8)(S_NEEDLE))
		{
			needlef = (U8)S_NEEDLE; //0x20������  0 ������
			DrawButton(keyd[KEY_P2_PINUP], (U16 *)(STARTWLBMPADD), keybmpind[KEY_P2_PINUP].border[0],
					   (needlef == 0x20) ? keybmpind[KEY_P2_PINUP].icon[0] : keybmpind[KEY_P2_PINUP].icon[1]);
		}
		// /[�������½�ͼ�����*end]**********************************************************

		// /[��ȡ���»����б�*begin]**********************************************************
		if (g_updataIREG)
		{
			g_updataIREG = 0;
			ReadPatList(0, pno); //�����ڴ�����
		}
		// /[��ȡ���»����б�*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_P2)
		{
			plastno = MENU_P2;
			pnoindex = 1;
			poldno[pnoindex] = MENU_P2;
			pnoindex++;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

#define KEY_MENU_BACK 1		//1 ����
#define KEY_MENU_READ 2		//2 ������
#define KEY_MENU_WRITE 3	//3 д����
#define KEY_MENU_INPUT 4	//4 ���
#define KEY_MENU_MODIFY 5	//5 �޸�
#define KEY_MENU_CONVERT 6	//6 �任
#define KEY_MENU_PROG 7		//7 ����
#define KEY_MENU_IOTEST 8	//8 ����
#define KEY_MENU_FUNCTION 9 //9 ����
#define KEY_MENU_HELP 10	//10 ����

#define KEY_MENU_MAX 11

#define RECT_MENU_TIT 1	 //1 ����
#define RECT_MENU_BASE 2 //2 ��ɫ

#define RECT_MENU_MAX 3

void Menu_Menu(void)
{

	U32 i;
	//	U16 times50ms = 0xffff;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/
	/****��־λ��ز���begin****/
	U8 isHome, f;
	U8 pferr = 0;
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_MENU_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},							  //1 ����
		{TYPE_X1, 343, KEY_X3, KEY_Y3},									  //2 ������
		{TYPE_X1 + KEY_X3 + SAFE_DIST2, 343, KEY_X3, KEY_Y3},			  //3 д����
		{TYPE_X1 + KEY_X3 * 2 + SAFE_DIST2 * 2, 343, KEY_X3, KEY_Y3},	  //4 ���
		{TYPE_X1 + KEY_X3 * 3 + SAFE_DIST2 * 3, 343, KEY_X3, KEY_Y3},	  //5 �޸�
		{TYPE_X1 + KEY_X3 * 4 + SAFE_DIST2 * 4, 343, KEY_X3, KEY_Y3},	  //6 �任
		{TYPE_X1, TYPE_Y4, KEY_X3, KEY_Y3},								  //7 ����
		{TYPE_X1 + KEY_X3 + SAFE_DIST2, TYPE_Y4, KEY_X3, KEY_Y3},		  //8 ����
		{TYPE_X1 + KEY_X3 * 2 + SAFE_DIST2 * 2, TYPE_Y4, KEY_X3, KEY_Y3}, //9 ����
		{TYPE_X1 + KEY_X3 * 3 + SAFE_DIST2 * 3, TYPE_Y4, KEY_X3, KEY_Y3}, //10 ����

	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {72}, {1, 0, 0, 0}, {0}},	//1 ����
		{{KEY_Silver}, {0}, {227}, {2, 0, 0, 0}, {0}},	//2 ������
		{{KEY_Silver}, {0}, {228}, {3, 0, 0, 0}, {0}},	//3 д����
		{{KEY_Silver}, {0}, {229}, {4, 0, 0, 0}, {0}},	//4 ���
		{{KEY_Silver}, {0}, {230}, {5, 0, 0, 0}, {0}},	//5 �޸�
		{{KEY_Silver}, {0}, {231}, {6, 0, 0, 0}, {0}},	//6 �任
		{{KEY_Silver}, {0}, {232}, {7, 0, 0, 0}, {0}},	//7 ����
		{{KEY_Silver}, {0}, {233}, {8, 0, 0, 0}, {0}},	//8 ����
		{{KEY_Silver}, {0}, {234}, {9, 0, 0, 0}, {0}},	//9 ����
		{{KEY_Silver}, {0}, {235}, {10, 0, 0, 0}, {0}}, //10 ����

	};

	const RECTPAD rect[RECT_MENU_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
		{TYPE_X13, 311, LCD_WIDTH, TYPE_SIZE5, {Color_DarkBlue}, {Color_White}, {0}}, //1 ����
		{TYPE_X13, 311 + TYPE_SIZE5, LCD_WIDTH, 137, {Color_Gray}, {0}, {0}},		  //2 ��ɫ

	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_MENU); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ40.bin
	// InitScreen();
	// initViewStatus();
	// pnoindex = 0;
	if (P_LAN)
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME_E;
	}
	else
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME;
	}

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************

	//��������
	for (i = 1; i < RECT_MENU_MAX; i++)
	{
		DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		if (i == RECT_MENU_TIT)
		{
			Draw24x24Str(rect[i].xpis + SAFE_DIST1, rect[i].ypis + SAFE_DIST4,
						 MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[i].colorT[0]);
		}
	}

	//����������ʾ
	for (i = 1; i < KEY_MENU_MAX; i++)
	{
		//��Ч����
		//������ɫ���ְ���
		//���水ť
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************
	g_BorAutoFeed.MaxFeed = 0;
	for (i = 0; i < AutoFeedMax; i++)
	{
		g_BorAutoFeed.Feed[i].NO = 0;
		g_BorAutoFeed.Feed[i].addr = 0;
	}
	g_Funtype = 0;
	g_DelAll_flag1 = 0;
	g_Funtype = 0;
	g_DelAll_flag1 = 0;
	g_temp_PfootOffVal = 0;
	g_uperr = 0;
	g_enwritef = 0;
	g_exwriteType = 0;
	g_ImgMove = 0;
	g_savemodify = 0;
	g_saveflag = 0;
	g_jogfunction1 = 0;
	g_ntohome = 0;
	g_needtohome = 0;
	g_endtype = 0;
	g_readcovnflag1 = 0;
	g_enctTackCode = 0;
	g_tdxpos = 0;
	g_tdypos = 0;
	//	g_ModToCt = 0;
	//	g_modifyflag1 = 0;
	//	g_zhouDown = 0;
	//	g_modcordflag = 0;
	//	g_modcordx = 0;
	//	g_modcordy = 0;

	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����
		if ((S_RUN) && (keydata)) //����״̬�£������١����١���һҳ�� ���� �ڰ����ɿ�ʱ ��Ч
		{
			if ((keydata != KEY_MENU_BACK) && (keydata != 0xff))
			{
				keydata = 0;
			}
		}

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			//���������������ܵİ���
			if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > 0) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//�ض�����²���Ч����

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
			{
				keyon = keydata;
				//�ް�����Ч
				//�ǳ��水��-//������ɫ���ְ���
				DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]); //���水��
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
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					default:
						DrawFMenuStrUser(rect[RECT_MENU_TIT].xpis,
										 rect[RECT_MENU_TIT].ypis,
										 keybmpind[keyon].titleStr[0],
										 rect[RECT_MENU_TIT].colorB[0], rect[RECT_MENU_TIT].colorT[0]);
						break;
					}

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
				Times = 0;
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
			case KEY_MENU_BACK:
				pnoindex--;
				pno = poldno[pnoindex];
				break;

			case KEY_MENU_READ:
			case KEY_MENU_WRITE:
				g_disppicflag = 1;
				g_namenumberflag = 0;
				if (g_GroupType == 1 || g_GroupType == 2)
				{
					pno = MENU_RWHELP;
					hleppara = 118;
					break;
				}
				g_backSREGlen = (F_FILELEN) / 2;
				g_backSREG_f = 1;
				g_backHREG_f = 1;
				g_Fileno = F_IND_NO;
				if (keyoff == KEY_MENU_READ)
				{
					pno = MENU_READ;
				}
				else
				{
					pno = MENU_WRITE;
					g_NoReadPatf = 1; //�˳�����Ŀ¼���������治��Ҫ��ȡ��������
					g_save_ptype = F_FILETYPE;
					g_save_len = F_FILELEN;
					g_savePatInfo.No = F_IND_NO;
					g_savePatInfo.len = F_FILELEN;
					g_savePatInfo.Type = F_FILETYPE;
					g_enwritef = 1;
					g_readH = 1;
				}

				break;

			case KEY_MENU_INPUT:
				if (g_GroupType == 1 || g_GroupType == 2)
				{ //ͼ�����״̬���ܴ��
					pno = MENU_RWHELP;
					hleppara = 118;
					break;
				}
				if (!(S_CLAMP) && P_HPF == 0x02)//ѹ��̧��״̬
				{ 
					if (Write_Cmd(C_YABANDN, 1, 0) == 0)//ѹ���½�
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
					else if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
				}
				if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
				{
				}
				else if (P_HPF == 0x02)
				{							   //����ԭ��,ͬʱ�趨�Զ���ԭ��
					f = Go_Home(MENU_MENU, 0); //0
					if (f == 0)
						return;
					isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
					if ((isHome & 0x02) || (isHome & 0x04))
					{
					}
					else
					{ //û�лص���еԭ��λ��
						return;
					}
					f = Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
				}

				g_Funtype = 1;
				g_GreatPatType = F_FILETYPE; //Ĭ�Ͻ���ʱ�Ļ�������
				pno = MENU_INPUT;

				if (g_temp_clr_flag == 1)
					g_temp_clr_flag = 0;
				g_code_mode = 9;
				g_modifyflag = 0;
				if (g_editflag)
					g_editflag = 0;
				poldno[2] = 0;

				if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
				{
				}
				else
				{
					g_backSREGlen = (F_FILELEN) / 2;
					BackupSHREG(0x03); //��������
					pno = MENU_RWHELP;
					hleppara = 157;
					poldno[pnoindex++] = MENU_INPUT;
				}
				g_ntohome = 1;
				g_enctTackCode = 1;
				break;

			case KEY_MENU_MODIFY:
				if (g_GroupType == 1 || g_GroupType == 2)
				{
					pno = MENU_RWHELP;
					hleppara = 118;
					break;
				}
				if (!(S_CLAMP) && P_HPF == 0x02)//ѹ��̧��״̬
				{ 
					if (Write_Cmd(C_YABANDN, 1, 0) == 0)
					{ //ѹ���½�
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
					else if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
				}
				if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
				{
				}
				else if (P_HPF == 0x02)
				{							   //����ԭ��,ͬʱ�趨�Զ���ԭ��
					f = Go_Home(MENU_MENU, 0); //0
					if (f == 0)
						return;
					isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
					if ((isHome & 0x02) || (isHome & 0x04))
					{
					}
					else
					{ //û�лص���еԭ��λ��
						return;
					}
					f = Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
				}

				//������ݻ���
				for (i = 0; i < MAXBLIND; i++)
				{
					BLKIND[i] = 0;
				}
				for (i = 0; i < MAXBLKBUF; i++)
				{
					BLKBUF[i] = 0;
				}
				g_backSREGlen = (F_FILELEN) / 2;
				BackupSHREG(0x03); //��������
				g_curPTtype = F_FILETYPE;

				if ((F_FILETYPE == PAT_MIT))
				{ //����
					f = CheckDrawScale(0, F_FILETYPE, 0, 1);
					if (0 == f)
					{ // ��ʾͼ�γ�����Χ
						pno = MENU_RWHELP;
						hleppara = 55;
						return;
					}
					else if (2 == f)
					{ //��ѹ���г̳���Χ
						pferr = 1;
					}

					if ((SREG[2] == 0x10) || (SREG[2] == 0))
					{ // �ڲ����������ļ�
						pno = MENU_RWHELP;
						hleppara = 77;
						return;
					}
					for (i = 0; i < 30; i++)
					{
						g_modaddr[i] = 0;
					}
					g_jog_point = HREG[HREG_FEED_C];
					g_reorigin = 0; // ���Զ�����ԭ��

					if ((SREG[30] & 0x00ff) != MLK_G)
					{
						Mit_Analyse_SplineData();
					}

					g_modifyf = 0;
					//FLASHADD_P = SST_SREG_SADD;
					//printf("g_patd.draw_add = %d  g_patd.line_add = %d \r\n",g_patd.draw_add,g_patd.line_add);
					mit_setRepConPins();
					Mit_SaveSplPatData();
					Mit_AnalyseDataCorrect(); //��黨�����ݵ���ȷ�Բ�У��
					f = Mit_CreateAllData();
					//printf("g_patd.draw_add = %d  g_patd.line_add = %d \r\n",g_patd.draw_add,g_patd.line_add);
					if (1 == Mit_ConvertMultiDraw())
					{
						f = Mit_CreateAllData();
					}

					g_Tsavetimes = 0;
					g_savetimes = 0;
					//BackupSREG(g_Tsavetimes,0,0,0);

					if (f == 0)
					{ // ��ʾͼ�γ�����Χ
						pno = MENU_RWHELP;
						hleppara = 55;
						return;
					}
					else if (f == 10)
					{
						pno = MENU_RWHELP;
						hleppara = 73;
						return;
					}
					else
					{
						g_zhouDown = 1;
						pno = MENU_MODIFY;
						g_savetimes = 0;
						tmppnoindex = pnoindex;
					}
					if ((g_oxpos != 0) || (g_oypos != 0))
						g_originf = 0;
					else
						g_originf = 1;
					g_modifyflag = 0;

					g_jogmode = 1;
					f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

					if (f == 0)
					{ //���趨��ѹ��������
						pno = MENU_RWHELP;
						hleppara = 15;
						return;
					}

					g_tmp_PfootValSet = YREG[SYS_PFOOTSET]; //�ݴ���ѹ������ֵ
					g_saveflag = 1;
				}
				else if ((F_FILETYPE == PAT_SEW))
				{ //�ֵ�
					f = CheckDrawScale(0, F_FILETYPE, (F_FILELEN), 1);
					if (0 == f)
					{ // ��ʾͼ�γ�����Χ
						pno = MENU_RWHELP;
						hleppara = 55;
						return;
					}
					else if (2 == f)
					{ //��ѹ���г̳���Χ
						pferr = 1;
					}
					g_modifyflag = 0;
					g_jogmode = 1;
					f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
					if (f == 0)
					{ //���趨��ѹ��������
						pno = MENU_RWHELP;
						hleppara = 15;
						return;
					}

					g_tmp_PfootValSet = YREG[SYS_PFOOTSET]; //�ݴ���ѹ������ֵ

					g_Tsavetimes = 0;
					g_savetimes = 0;
					BackupSREG(g_Tsavetimes, 0, F_FILELEN, 1);

					g_zhouDown = 1;
					pno = MENU_MODIFY;
					g_savetimes = 0;
					tmppnoindex = pnoindex;
				}
				Read_xyposi();
				g_jog_point = HREG[HREG_FEED_C];
				g_modinpx = g_xabspos;
				g_modinpy = g_yabspos;
				g_modcordflag = 1;
				g_modinpoint = g_jog_point;

				g_pfootd.oldbase = g_pfootd.base;
				g_tdtensiond.base = g_tdtensiond.oldbase;
				g_ExCode = 0;
				g_curExCode = 0;
				g_curPinAng = 0;
				g_curSetPara1 = 0;
				g_curSetPara2 = 0;
				g_pointPinAng = 0;
				g_pointSetPara1 = 0;
				g_pointSetPara2 = 0;
				g_readH = 1;
				//printf("g_modinpx = %d  g_modinpy = %d  g_jog_point = %d\r\n",g_modinpx,g_modinpy,g_jog_point);
				if (pferr)
				{
					pno = MENU_RWHELP;
					hleppara = 141;
					return;
				}
				break;

			case KEY_MENU_CONVERT:
				if (g_GroupType == 1 || g_GroupType == 2)
				{
					pno = MENU_RWHELP;
					hleppara = 118;
					break;
				}

				if (!(S_CLAMP) && P_HPF == 0x02)//ѹ��̧��״̬
				{ 
					if (Write_Cmd(C_YABANDN, 1, 0) == 0)//ѹ���½�
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
					else if (DREG[D_CMDSTATUS] != 100)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						break;
					}
				}

				if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
				{
				}
				else if (P_HPF == 0x02)
				{							   //����ԭ��,ͬʱ�趨�Զ���ԭ��
					f = Go_Home(MENU_MENU, 0); //0
					if (f == 0)
						return;
					isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
					if ((isHome & 0x02) || (isHome & 0x04))
					{
					}
					else
					{ //û�лص���еԭ��λ��
						return;
					}
					f = Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
				}
				g_Funtype = 2;
				g_convert_orgf = 0;
				//������ݻ���
				for (i = 0; i < MAXBLIND; i++)
				{
					BLKIND[i] = 0;
				}
				for (i = 0; i < MAXBLKBUF; i++)
				{
					BLKBUF[i] = 0;
				}
				g_backSREGlen = (F_FILELEN) / 2;
				BackupSHREG(0x03); //��������
				g_curPTtype = F_FILETYPE;

				if (F_FILETYPE == PAT_MIT)
				{ //����
					f = CheckDrawScale(0, F_FILETYPE, 0, 1);

					if (0 == f)
					{ // ��ʾͼ�γ�����Χ
						pno = MENU_RWHELP;
						hleppara = 55;
						return;
					}
					else if (2 == f)
					{ //��ѹ���г̳���Χ
						pferr = 1;
					}
					if ((SREG[2] == 0x10) || (SREG[2] == 0))
					{ // �ڲ����������ļ�
						pno = MENU_RWHELP;
						hleppara = 77;
						return;
					}

					g_jog_point = HREG[HREG_FEED_C];
					g_reorigin = 0; // ���Զ�����ԭ��

					if ((SREG[30] & 0x00ff) != MLK_G)
					{
						Mit_Analyse_SplineData();
					}

					g_modifyf = 0;
					g_splsum = 0;

					Mit_AnalyseDataCorrect();
					f = Mit_CreateAllData();

					g_Tsavetimes = 0;
					g_savetimes = 0;
					//BackupSREG(g_Tsavetimes,0,0,0);;

					if (f == 0)
					{ // ��ʾͼ�γ�����Χ
						pno = MENU_RWHELP;
						hleppara = 55;
						return;
					}
					else if (f == 10)
					{
						pno = MENU_RWHELP;
						hleppara = 73;
						return;
					}
					else
					{
						g_zhouDown = 1;
						pno = MENU_CONVERT;
						g_savetimes = 0;
					}

					if ((g_oxpos != 0) || (g_oypos != 0))
						g_originf = 0;
					else
						g_originf = 1;
					g_modifyflag = 0;
					g_saveflag = 1;
				}
				else if ((F_FILETYPE == PAT_SEW))
				{ //�ֵ�
					f = CheckDrawScale(0, F_FILETYPE, (F_FILELEN), 1);
					if (0 == f)
					{ // ��ʾͼ�γ�����Χ
						pno = MENU_RWHELP;
						hleppara = 55;
						return;
					}
					else if (2 == f)
					{ //��ѹ���г̳���Χ
						pferr = 1;
					}
					g_jog_point = HREG[HREG_FEED_C];
					g_modifyflag = 0;

					g_Tsavetimes = 0;
					g_savetimes = 0;
					BackupSREG(g_Tsavetimes, 0, F_FILELEN, 1);
					g_zhouDown = 1;
					pno = MENU_CONVERT;
					tmppnoindex = pnoindex;
				}
				if (pferr)
				{
					pno = MENU_RWHELP;
					hleppara = 142;
					return;
				}
				g_jog_point = 0;
				g_pfootd.oldbase = g_pfootd.base;
				g_tdtensiond.base = g_tdtensiond.oldbase;
				g_ExCode = 0;
				g_curExCode = 0;
				g_curPinAng = 0;
				g_curSetPara1 = 0;
				g_curSetPara2 = 0;
				g_pointPinAng = 0;
				g_pointSetPara1 = 0;
				g_pointSetPara2 = 0;
				g_readH = 1;

				if (((S_HOME) || (S_NEWHOME)) && (!(S_2HD)))
				{
				}
				else
				{
					pno = MENU_RWHELP;
					hleppara = 157;
					poldno[pnoindex++] = MENU_CONVERT;
				}
				break;

			case KEY_MENU_IOTEST:
				pno = MENU_IOTEST;
				E_RUNEN();
				f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
				if (f == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					break;
				}
				break;
			case KEY_MENU_FUNCTION:
				pno = MENU_FUNCTION;
				break;

			case KEY_MENU_PROG:
				pno = MENU_PROG;
				g_prochange = 0;
				for (i = 0; i < 255; i++)
					g_publicbuf[i] = PREG[i * PREG_LENGTH + PREG_DEFAULT];
				Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
				break;
			case KEY_MENU_HELP:
				pno = MENU_HELP;
				break;
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			//����Ҫ��ʾ�İ���
			//�ǳ��水��-//������ɫ���ְ���
			//���水��
			DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);

			keyon = 0;
			beeponf = 0;

			DrawFillRect(rect[RECT_MENU_TIT].xpis, rect[RECT_MENU_TIT].ypis,
						 rect[RECT_MENU_TIT].xsize, rect[RECT_MENU_TIT].ysize, rect[RECT_MENU_TIT].colorB[0]); //��ʾ�ء�Ŀ¼ģʽ��
			DrawFMenuStrUser(rect[RECT_MENU_TIT].xpis,
							 rect[RECT_MENU_TIT].ypis,
							 keybmpind[keyon].titleStr[0],
							 rect[RECT_MENU_TIT].colorB[0], rect[RECT_MENU_TIT].colorT[0]);
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 1000)
		{
			Timerms = 0;
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(poldno[0]);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			ErrorMsg(poldno[0]);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}
			if (S_RUN)
			{
				pno = poldno[0];
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[��ȡ���»����б�*begin]**********************************************************

		// /[��ȡ���»����б�*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_MENU)
		{
			plastno = MENU_MENU;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
	if (coldstart)
	{
		U16 chACK;
		chACK = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
		if ((chACK) && (YREG[SYS_COMEN] == 0x5a5a))
		{
			chACK = ReadFileIndex(C_RDMEMPATLIST, MENU_MENU);
			if (chACK)
			{
				U16 chPatternNum = 0;
				for (chPatternNum = 0, i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
				{

					if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
						(((IREG2[i + 1] << 16) | IREG2[i]) < 0xffffffff))
						chPatternNum++;
				}
				g_Fmax = chPatternNum / 10;
				if ((g_Fmax == 0) || (chPatternNum % 10))
					g_Fmax++;
				if (g_Fmax < g_fno)
					g_fno = 1;
				if (chPatternNum)
				{
					U16 chACK2;
					chACK2 = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
					if (chACK2)
					{
						U16 chPatNo, chPatHeadAdd;
						chPatNo = F_IND_NO;
						for (chPatHeadAdd = 0; chPatHeadAdd < D_Length[INDEX2]; chPatHeadAdd += FileIndexSize)
						{
							if ((chPatHeadAdd + MINFILENO) == chPatNo)
								break;
						}
						if (chPatHeadAdd < D_Length[INDEX2])
						{
							if ((chPatNo >= MINFILENO) && (chPatNo <= MAXFILENO))
							{
								U32 wPatData;
								chPatHeadAdd = (chPatNo - MINFILENO) * FileIndexSize;
								wPatData = (((U32)(IREG2[chPatHeadAdd + 1]) << 16) + IREG2[chPatHeadAdd]) / 2;
								chPatNo = 0; // ���������ʧ�ܣ���ʼ��SREG�Ĵ���
								if ((wPatData > 0) && (wPatData <= MAXSREG))
								{
									U16 chACK3;
									chACK3 = Read_D(D_Address[PTDATA], wPatData, (U16 *)(&SREG[0])); // ����ǰ��������
									if (chACK3)
									{
										coldstart = 0;
										g_init_flag = 1;
										g_selectdisno = F_IND_NO;
									}
								}
							}
							else
							{
								chPatHeadAdd = 0;
							}
						}
					}
				}
				else
				{
					coldstart = 0;
				}
			}
		}
		if (!chACK)
		{
			coldstart = 0;
			pno = MENU_RWHELP;
			hleppara = 15;
		}
	}
}

#define KEY_THREAD_SCRNSVR 1
#define KEY_THREAD_ENTER 2 //1 ȷ��

#define KEY_THREAD_MAX 3

#define RECT_THREAD_STR 1 //1 ������ʾ
#define RECT_THREAD_BMP 2 //2 ͼƬ��ʾ

#define RECT_THREAD_MAX 3

void Menu_Thread(void)
{

	U32 i;
	//	U16 times50ms = 0xffff;
	U32 ERROR_MSG;
	U32 ERROR_LEN;
	U16 linemaxdisplay = 26;
	U16 Err_MSG1_Sheet2 = 49;
	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****��־λ��ز���begin****/

	/****��־λ��ز���end***/

	const U8 keynum = {KEY_THREAD_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},					 //1 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //2 ȷ��
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},  //1 ����
		{{KEY_Silver}, {0}, {71}, {1, 0, 0, 0}, {0}}, //2 ȷ��

	};

	const RECTPAD rect[RECT_THREAD_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {12}},
		{SAFE_DIST9 + TYPE_SIZE24, TYPE_Y10, TYPE_SIZE23, TYPE_SIZE22, {Color_White}, {Color_Black}, {0}}, //1 ������ʾ
		{SAFE_DIST9, TYPE_Y10, TYPE_SIZE24, TYPE_SIZE22, {0}, {0}, {868}},								   //2 ͼƬ��ʾ
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_THREAD); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ40.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();

	if (P_LAN)
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME_E;
	}
	else
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME;
	}
	if (P_LAN)
	{
		UADD_ERROR_MSG = ADD_ERROR_MSG1_E;
		UADD_PROG_MSG7 = ADD_ERROR_MSG2_E;
		ERROR_MSG = 40;	 //450 * TIT_MAXLENGTH_E;
		ERROR_LEN = 115; //TIT_MAXLENGTH_E;
	}
	else
	{
		UADD_ERROR_MSG = ADD_ERROR_MSG1;
		UADD_PROG_MSG7 = ADD_ERROR_MSG2;

		ERROR_MSG = 24; //30;
		ERROR_LEN = 74; //105;
	}
	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //����
	//��������
	for (i = 1; i < RECT_THREAD_MAX; i++)
	{

		if (i == RECT_THREAD_BMP)
		{
			DrawFPBMPBTN1(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].index[0]);
		}
		else
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		}
		if (i == RECT_THREAD_STR)
		{
			Draw24x24MulStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST4,
							(uint *)(USE_ERROR_MSG + ERROR_LEN * (458) + ERROR_MSG),
							linemaxdisplay, Err_MSG1_Sheet2, 458, 4, rect[i].colorT[0]);
			Draw24x24MulStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (SAFE_DIST4 + 24) * 4,
							(uint *)(USE_ERROR_MSG + ERROR_LEN * (459) + ERROR_MSG),
							linemaxdisplay, Err_MSG1_Sheet2, 459, 4, rect[i].colorT[0]);
			Draw24x24MulStr(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (SAFE_DIST4 + 24) * 8,
							(uint *)(USE_ERROR_MSG + ERROR_LEN * (460) + ERROR_MSG),
							linemaxdisplay, Err_MSG1_Sheet2, 460, 4, rect[i].colorT[0]);
		}
	}

	//����������ʾ
	for (i = 2; i < KEY_THREAD_MAX; i++)
	{
		//��Ч����
		//������ɫ���ְ���
		//���水ť
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
	ViewPage(usepageno);				 //������ʾ����
	if (Write_Cmd1(C_THERAD, 0, 0) == 0) //���ͽ��봩��ģʽָ��
	{
		pno = MENU_RWHELP;
		hleppara = 16;
	}
	else
	{
		if (Read_Status() == 0)
		{
			pno = MENU_RWHELP;
			hleppara = 15;
		}
		else if (DREG[D_CMDSTATUS] != 100)
		{
			pno = MENU_RWHELP;
			hleppara = 16;
		}
	}
	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************
	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			//���������������ܵİ���
			if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > 0) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//�ض�����²���Ч����

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
			{
				keyon = keydata;
				if (keyon == KEY_THREAD_SCRNSVR) //�ް�����Ч
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
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}

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
				Times = 0;
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
			case KEY_THREAD_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_THREAD_ENTER: //��Ҫ���˳�����ģʽָ��
				if (Write_Cmd(C_QUIT_THERAD, 0, 0) == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[pnoindex++] = MENU_THREAD;
					break;
				}
				if (Read_Status() == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[pnoindex++] = MENU_THREAD;
					break;
				}
				else if (DREG[D_CMDSTATUS] != 100)
				{
					pno = MENU_RWHELP;
					hleppara = 16;
					poldno[pnoindex++] = MENU_THREAD;
					break;
				}
				pno = poldno[--pnoindex];
				break;
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{
			if (keyon == KEY_THREAD_SCRNSVR) //����Ҫ��ʾ�İ���
			{
			}
			//�ǳ��水��-//������ɫ���ְ���
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;

			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ�ش���
			Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 1000)
		{
			Timerms = 0;
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(poldno[0]);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			ErrorMsg(poldno[0]);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_THREAD)
		{
			plastno = MENU_THREAD;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

#define KEY_COUNT_SCRNSVR 1 //1 ����
#define KEY_COUNT_BACK 2	//2 ����
#define KEY_COUNT_ENTER 3	//3 ȷ��
#define KEY_COUNT_SET 4		//4 �趨ֵѡ��
#define KEY_COUNT_NOW 5		//5 ��ǰֵѡ��
#define KEY_COUNT_ONOFF 6	//6 ����������
#define KEY_COUNT_CLR 7		//7 ���������
#define KEY_COUNT_CLEAN 8	//8 �������
#define KEY_COUNT_UP 9		//9 +1
#define KEY_COUNT_DOWN 10	// 10 -1
#define KEY_COUNT_0 11		//11 0
#define KEY_COUNT_1 12		//12 1
#define KEY_COUNT_2 13		//13 2
#define KEY_COUNT_3 14		//14 3
#define KEY_COUNT_4 15		//15 4
#define KEY_COUNT_5 16		//16 5
#define KEY_COUNT_6 17		//17 6
#define KEY_COUNT_7 18		//18 7
#define KEY_COUNT_8 19		//19 8
#define KEY_COUNT_9 20		//20 9

#define KEY_COUNT_MAX 21

#define RECT_COUNT_SET 1 //1 �趨ֵ���ֿ�
#define RECT_COUNT_NOW 2 //2 ��ǰֵ���ֿ�

#define RECT_COUNT_MAX 3

void Menu_Counter(U16 para)
{

	U32 i;
	U16 times50ms = 0xffff;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****��־λ��ز���begin****/
	U8 page_flash = 1, page_flash1 = 1;
	U8 refresh1 = 1, refresh2 = 1;
	U32 cnt_now, cnt_set; //
	// U16 dx = 40;
	static U8 chSelectBtn = 0; //��ťѡ�б�־λ��0������ 1����ǰ
	static U8 upcntonoff_f = 0, dncntonoff_f = 0;
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_COUNT_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},																//1 ����
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},												//2 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},											//3 ȷ��
		{TYPE_X14, TYPE_Y13, KEY_X4, KEY_Y4},											//4 �趨ֵѡ��
		{TYPE_X14, TYPE_Y13 + TYPE_SIZE14, KEY_X4, KEY_Y4},								//5 ��ǰֵѡ��
		{TYPE_X14, TYPE_Y13 + TYPE_SIZE14 * 2 - (KEY_Y1 - KEY_Y4) / 2, KEY_X1, KEY_Y1}, //6 ����������
		{TYPE_X14 + SAFE_DIST6 + KEY_X4 + 125 - KEY_X1,
		 TYPE_Y13 + TYPE_SIZE14 * 2 - (KEY_Y1 - KEY_Y4) / 2, KEY_X1, KEY_Y1},						  //7 ���������
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 4, KEY_X2, KEY_Y2},							  //8 �������
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},		  //9 +1
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //10 -1
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},							  //11 0
		{TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},														  //12 1
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13, KEY_X2, KEY_Y2},									  //13 2
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13, KEY_X2, KEY_Y2},							  //14 3
		{TYPE_X15, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},									  //15 4
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},			  //16 5
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},		  //17 6
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},							  //18 7
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},		  //19 8
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //20 9
	};

	const tydDisSrcIndex keybmpind[] = {
		//title_counter
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},  //1 ����
		{{KEY_Silver}, {0}, {72}, {2, 0, 0, 0}, {0}}, //2 ����
		{{KEY_Silver}, {0}, {71}, {3, 0, 0, 0}, {0}}, //3 ȷ��

		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {4, 0, 0, 0}, {6}}, //4 �趨ֵѡ��
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {5, 0, 0, 0}, {7}}, //5 ��ǰֵѡ��
		{{KEY_Silver, KEY_Blue}, {0}, {78, 79, 80, 81}, {11, 12, 14, 15}, {0, 17}},							   //6 ����������
		{{KEY_Silver}, {0}, {76, 77}, {13, 16, 0, 0}, {0}},													   //7 ���������
		{{KEY_Silver}, {0}, {113}, {8, 0, 0, 0}, {0}},														   //8 �������
		{{KEY_Silver}, {0}, {199}, {9, 0, 0, 0}, {0}},														   //9 +1
		{{KEY_Silver}, {0}, {201}, {10, 0, 0, 0}, {0}},														   //10 -1

		{{KEY_Silver}, {0}, {91}, {18, 0, 0, 0}, {0}},	//11 0
		{{KEY_Silver}, {0}, {92}, {18, 0, 0, 0}, {0}},	//12 1
		{{KEY_Silver}, {0}, {93}, {18, 0, 0, 0}, {0}},	//13 2
		{{KEY_Silver}, {0}, {94}, {18, 0, 0, 0}, {0}},	//14 3
		{{KEY_Silver}, {0}, {95}, {18, 0, 0, 0}, {0}},	//15 4
		{{KEY_Silver}, {0}, {96}, {18, 0, 0, 0}, {0}},	//16 5
		{{KEY_Silver}, {0}, {97}, {18, 0, 0, 0}, {0}},	//17 6
		{{KEY_Silver}, {0}, {98}, {18, 0, 0, 0}, {0}},	//18 7
		{{KEY_Silver}, {0}, {99}, {18, 0, 0, 0}, {0}},	//19 8
		{{KEY_Silver}, {0}, {100}, {18, 0, 0, 0}, {0}}, //20 9

	};

	const RECTPAD rect[RECT_COUNT_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0, 17}},
		{TYPE_X14 + SAFE_DIST6 + KEY_X4, TYPE_Y13 + (KEY_Y4 - TYPE_SIZE5) / 2, TYPE_SIZE16, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},				  //1 �趨ֵ���ֿ�
		{TYPE_X14 + SAFE_DIST6 + KEY_X4, TYPE_Y13 + (KEY_Y4 - TYPE_SIZE5) / 2 + TYPE_SIZE14, TYPE_SIZE16, TYPE_SIZE5, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //2 ��ǰֵ���ֿ�
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_COUNTER); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
	// if (P_LAN == 2)
	// { //Խ����
	// 	dx = 12;
	// }
	if (para)
	{
		cnt_now = (dncount_now_h << 16 | dncount_now_l);
		cnt_set = (dncount_set_h << 16 | dncount_set_l);
		dncntonoff_f = dncntonoff_flag;
	}
	else
	{
		cnt_now = (upcount_now_h << 16 | upcount_now_l);
		cnt_set = (upcount_set_h << 16 | upcount_set_l);
		upcntonoff_f = upcntonoff_flag;
	}
	if ((P_UCC == 1) && (para == 0))
		chSelectBtn = 0;
	if ((P_DCC == 1) && (para == 1))
		chSelectBtn = 0;
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)((para) ? rect[0].index[1] : rect[0].index[0]) * TitleLengthMax,
				 SheetPage, (para) ? rect[0].index[1] : rect[0].index[0], rect[0].colorT[0]); //�������
	//��������
	for (i = 1; i < RECT_COUNT_MAX; i++)
	{
		if (i == RECT_COUNT_SET || i == RECT_COUNT_NOW)
		{
			if ((chSelectBtn == 0 && i == RECT_COUNT_SET) ||
				(chSelectBtn == 1 && i == RECT_COUNT_NOW))
			{
				DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[1]);
				DrawDottedRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorT[1]);
			}
			else
			{
				DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
			}
		}
		else
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		}
	}

	//����������ʾ
	for (i = 2; i < KEY_COUNT_MAX; i++)
	{
		//��Ч����
		if (i == KEY_COUNT_SET || i == KEY_COUNT_NOW) //������ɫ���ְ���
		{
			if ((chSelectBtn == 0 && i == KEY_COUNT_SET) ||
				(chSelectBtn == 1 && i == KEY_COUNT_NOW))
			{
				DrawButtonSingleColor(keyd[i], keybmpind[i].border[1], keybmpind[i].icon[1]);
				Draw24X24FFStr(keyd[i].xpis + (keyd[i].xsize - 24 * 3) / 2,
							   keyd[i].ypis + (keyd[i].ysize - 24) / 2,
							   MEM_TITLE_ADD + ((U32)keybmpind[i].nameStr[0]) * TitleLengthMax,
							   keybmpind[i].textColor[1]);
			}
			else
			{
				DrawButtonSingleColor(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
				Draw24X24FFStr(keyd[i].xpis + (keyd[i].xsize - 24 * 3) / 2,
							   keyd[i].ypis + (keyd[i].ysize - 24) / 2,
							   MEM_TITLE_ADD + ((U32)keybmpind[i].nameStr[0]) * TitleLengthMax,
							   keybmpind[i].textColor[0]);
			}
		}
		else if (i == KEY_COUNT_ONOFF)
		{
			U8 chSwitch = 0;
			if (para)
			{
				if (dncntonoff_f)
					chSwitch = 3;
				else
					chSwitch = 2;
			}
			else
			{
				if (upcntonoff_f)
					chSwitch = 1;
				else
					chSwitch = 0;
			}
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[chSwitch % 2],
					   keybmpind[i].icon[chSwitch]);
		}
		else if (i == KEY_COUNT_CLR)
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0],
					   keybmpind[i].icon[para]);
		}
		else //���水ť
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
	ViewPage(usepageno); //������ʾ����
	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����
		//��Ч����

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			if ((keyon == KEY_COUNT_UP) || (keyon == KEY_COUNT_DOWN)) //���������������ܵİ���
			{
			}
			else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//�ض�����²���Ч����

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
			{
				keyon = keydata;
				if (keyon == KEY_COUNT_SCRNSVR) //�ް�����Ч
				{
				}
				if (keyon == KEY_COUNT_SET || keyon == KEY_COUNT_NOW) //�ǳ��水��-//������ɫ���ְ���
				{
					if ((chSelectBtn == 0 && keyon == KEY_COUNT_SET) ||
						(chSelectBtn == 1 && keyon == KEY_COUNT_NOW))
					{
						DrawButtonOn(keyd[keyon], keybmpind[keyon].border[1]);
					}
					else
					{
						DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
					}
				}
				else if (keyon == KEY_COUNT_ONOFF)
				{
					U8 chSwitch = 0;
					if (para)
					{
						if (dncntonoff_f)
							chSwitch = 3;
						else
							chSwitch = 2;
					}
					else
					{
						if (upcntonoff_f)
							chSwitch = 1;
						else
							chSwitch = 0;
					}
					DrawButtonOn(keyd[keyon], keybmpind[keyon].border[chSwitch % 2]);
				}
				else
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
					U8 chSwitch = 0;
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					case KEY_COUNT_ONOFF:

						if (para)
						{
							if (dncntonoff_f)
								chSwitch = 3;
							else
								chSwitch = 2;
						}
						else
						{
							if (upcntonoff_f)
								chSwitch = 1;
							else
								chSwitch = 0;
						}
						DrawFMenuStr(keybmpind[keyon].titleStr[chSwitch]);
						break;
					case KEY_COUNT_CLR:
						DrawFMenuStr(keybmpind[keyon].titleStr[para]);
						break;
					case KEY_COUNT_0:
					case KEY_COUNT_1:
					case KEY_COUNT_2:
					case KEY_COUNT_3:
					case KEY_COUNT_4:
					case KEY_COUNT_5:
					case KEY_COUNT_6:
					case KEY_COUNT_7:
					case KEY_COUNT_8:
					case KEY_COUNT_9:
						BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
						DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_COUNT_0, 1, rect[0].colorT[0]);
						break;
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}

					delay = KEYFT;
				}
				else if (beeponf == 1) //�ڶ��ν���
				{
					beeponf = 2;
					if ((keyon == KEY_COUNT_UP) || (keyon == KEY_COUNT_DOWN)) //���г����������ܰ����Ĺ���
					{
						delay = DELAY_MOVE;
					}
					else
						delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}

				if ((keyon == KEY_COUNT_UP) || (keyon == KEY_COUNT_DOWN)) //���г����������ܰ����Ĺ���
				{
					keyoff = keyon;
					key_on_flag1 = 1;
				}
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
			if ((keyoff > KEY_COUNT_DOWN) && (keyoff < keynum))
			{ //����ֵ
				if (chSelectBtn == 0)
				{
					cnt_set = (cnt_set % 10000) * 10 + (keyoff - KEY_COUNT_0);
					refresh1 = 1;
				}
				else if (chSelectBtn == 1)
				{ //����ֵ
					cnt_now = (cnt_now % 10000) * 10 + (keyoff - KEY_COUNT_0);
					refresh2 = 1;
				}
			}
			else
			{
				switch (keyoff)
				{
				case KEY_COUNT_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_COUNT_BACK:
					pnoindex = 0;
					pno = MENU_P2;
					break;
				case KEY_COUNT_ENTER:
					if (cnt_now > cnt_set)
					{
						pno = MENU_RWHELP;
						hleppara = 16;
						poldno[pnoindex] = MENU_COUNTER;
						pnoindex++;
						break;
					}
					pno = MENU_P2;
					pnoindex = 0;
					if (para)
					{ //����
						if ((dncount_now_h << 16 | dncount_now_l) != cnt_now)
						{
							dncount_now_h = (cnt_now >> 16) & 0xffff;
							dncount_now_l = cnt_now & 0xffff;
							Write_Cmd2(C_WRITESTATUS, dncount_now_l, SYS_DNCOUNT, dncount_now_h);
						}
						if ((dncntonoff_flag != dncntonoff_f) || ((dncount_set_h << 16 | dncount_set_l) != cnt_set))
						{
							dncntonoff_flag = dncntonoff_f;
							dncount_set_h = (cnt_set >> 16) & 0xffff;
							dncount_set_l = cnt_set & 0xffff;
							Write_D((D_Address[PARA] + 1018), 6, (U16 *)&PREG[1018]);
							DREG[D_COMMAND] = C_UPDATEPARA;
							DREG[D_CMDSTATUS] = 0;
							Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
						}
					}
					else
					{ //����
						if ((upcount_now_h << 16 | upcount_now_l) != cnt_now)
						{

							upcount_now_h = (cnt_now >> 16) & 0xffff;
							upcount_now_l = cnt_now & 0xffff;
							Write_Cmd2(C_WRITESTATUS, upcount_now_l, SYS_UPCOUNT, upcount_now_h);
						}
						if ((upcntonoff_flag != upcntonoff_f) || ((upcount_set_h << 16 | upcount_set_l) != cnt_set))
						{
							upcntonoff_flag = upcntonoff_f;
							upcount_set_h = (cnt_set >> 16) & 0xffff;
							upcount_set_l = cnt_set & 0xffff;

							Write_D((D_Address[PARA] + 1018), 6, (U16 *)&PREG[1018]);
							DREG[D_COMMAND] = C_UPDATEPARA;
							DREG[D_CMDSTATUS] = 0;
							Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
						}
					}
					break;

				case KEY_COUNT_SET:
					chSelectBtn = 0;
					page_flash = 1;
					refresh1 = 1;
					refresh2 = 1;
					break;

				case KEY_COUNT_NOW:
					if (((P_UCC != 1) && (para == 0)) || ((P_DCC != 1) && (para == 1)))
					{
						chSelectBtn = 1;
						page_flash = 1;
						refresh1 = 1;
						refresh2 = 1;
					}
					else
					{
						EREG[1] = 112;
						pno = MENU_RWHELP;
						hleppara = 22;
						poldno[pnoindex] = MENU_COUNTER;
						pnoindex++;
					}
					break;

				case KEY_COUNT_ONOFF:
					if (para)
					{
						if (dncntonoff_f)
						{
							dncntonoff_f = 0; //��������Ч
						}
						else
						{
							dncntonoff_f = 1; //��������Ч
						}
					}
					else
					{
						if (upcntonoff_f)
						{
							upcntonoff_f = 0; //��������Ч
						}
						else
						{
							upcntonoff_f = 1; //��������Ч
						}
					}
					page_flash1 = 1;
					break;

				case KEY_COUNT_CLR:
					if (((P_UCC != 1) && (para == 0)) || ((P_DCC != 1) && (para == 1)))
					{
						if (para)
						{
							cnt_now = cnt_set;
						}
						else
						{
							cnt_now = 0;
						}
						refresh2 = 1;
					}
					else
					{
						EREG[1] = 112;
						pno = MENU_RWHELP;
						hleppara = 22;
						poldno[pnoindex] = MENU_COUNTER;
						pnoindex++;
					}
					break;

				case KEY_COUNT_CLEAN:
					if (para == 0)
					{
						if (chSelectBtn)
						{
							cnt_now = cnt_now / 10;
							refresh2 = 1;
						}
						else if (P_UCC != 1)
						{
							cnt_set = cnt_set / 10;
							refresh1 = 1;
						}
					}
					else
					{
						if (chSelectBtn)
						{
							cnt_now = cnt_now / 10;
							refresh2 = 1;
						}
						else if (P_DCC != 1)
						{
							cnt_set = cnt_set / 10;
							refresh1 = 1;
						}
					}
					break;

				case KEY_COUNT_UP:
					//�������ٰ����ּ�������
					if (chSelectBtn == 0)
					{
						if (cnt_set == 99999)
							break;
						cnt_set++;
						if (cnt_now == 99999)
							cnt_set = 99999;
						refresh1 = 1;
					}
					else if (chSelectBtn == 1)
					{
						if (cnt_now >= cnt_set)
							cnt_now = cnt_set;
						else
							cnt_now++;
						refresh2 = 1;
					}
					break;

				case KEY_COUNT_DOWN:
					//�������ٰ����ּ�������
					if (chSelectBtn == 0)
					{
						if (cnt_set == 0)
							break;
						cnt_set--;
						if (cnt_now == 99999)
							cnt_set = 99999;
						refresh1 = 1;
					}
					else if (chSelectBtn == 1)
					{
						if (cnt_now == 0)
							break;
						if (cnt_now > cnt_set)
							cnt_now = cnt_set;
						else
							cnt_now--;
						refresh2 = 1;
					}
					break;
				}
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			if (keyon == KEY_COUNT_SCRNSVR) //����Ҫ��ʾ�İ���
			{
			}
			else if (keyon == KEY_COUNT_SET || keyon == KEY_COUNT_NOW) //�ǳ��水��-//������ɫ���ְ���
			{
				for (i = KEY_COUNT_SET; i <= KEY_COUNT_NOW; i++)
				{
					if ((chSelectBtn == 0 && keyon == KEY_COUNT_SET) ||
						(chSelectBtn == 1 && keyon == KEY_COUNT_NOW))
					{
						DrawButtonOff(keyd[keyon], keybmpind[keyon].border[1]);
						Draw24X24FFStr(keyd[keyon].xpis + (keyd[keyon].xsize - 24 * 3) / 2,
									   keyd[keyon].ypis + (keyd[keyon].ysize - 24) / 2,
									   MEM_TITLE_ADD + ((U32)keybmpind[keyon].nameStr[0]) * TitleLengthMax,
									   keybmpind[keyon].textColor[1]);
					}
					else
					{
						DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
						Draw24X24FFStr(keyd[keyon].xpis + (keyd[keyon].xsize - 24 * 3) / 2,
									   keyd[keyon].ypis + (keyd[keyon].ysize - 24) / 2,
									   MEM_TITLE_ADD + ((U32)keybmpind[keyon].nameStr[0]) * TitleLengthMax,
									   keybmpind[keyon].textColor[0]);
					}
				}
			}
			else if (keyon == KEY_COUNT_ONOFF)
			{
				U8 chSwitch = 0;
				if (para)
				{
					if (dncntonoff_f)
						chSwitch = 3;
					else
						chSwitch = 2;
				}
				else
				{
					if (upcntonoff_f)
						chSwitch = 1;
					else
						chSwitch = 0;
				}
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[chSwitch % 2]);
			}
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)((para) ? rect[0].index[1] : rect[0].index[0]) * TitleLengthMax,
						 SheetPage, (para) ? rect[0].index[1] : rect[0].index[0], rect[0].colorT[0]); //��ʾ�ؼ������
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 1000)
		{
			Timerms = 0;
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(MENU_COUNTER);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			U32 wTimeTemp;

			ErrorMsg(MENU_COUNTER);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}

			if (S_RUN)
			{

				wTimeTemp = Timer_ms >> 5; //Timer_ms/2^5
				if (wTimeTemp != times50ms)
				{
					times50ms = wTimeTemp;
				}
			}
			else
			{
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[ˢ���趨�����ڵİ�ť*begin]**********************************************************
		if (page_flash)
		{
			page_flash = 0;

			for (i = KEY_COUNT_SET; i <= KEY_COUNT_NOW; i++)
			{
				if ((chSelectBtn == 0 && i == KEY_COUNT_SET) ||
					(chSelectBtn == 1 && i == KEY_COUNT_NOW))
				{
					DrawButtonSingleColor(keyd[i], keybmpind[i].border[1], keybmpind[i].icon[1]);
					Draw24X24FFStr(keyd[i].xpis + (keyd[i].xsize - 24 * 3) / 2,
								   keyd[i].ypis + (keyd[i].ysize - 24) / 2,
								   MEM_TITLE_ADD + ((U32)keybmpind[i].nameStr[0]) * TitleLengthMax,
								   keybmpind[i].textColor[1]);
				}
				else
				{
					DrawButtonSingleColor(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
					Draw24X24FFStr(keyd[i].xpis + (keyd[i].xsize - 24 * 3) / 2,
								   keyd[i].ypis + (keyd[i].ysize - 24) / 2,
								   MEM_TITLE_ADD + ((U32)keybmpind[i].nameStr[0]) * TitleLengthMax,
								   keybmpind[i].textColor[0]);
				}
			}
		}
		// /[ˢ���趨�����ڵİ�ť*end]**********************************************************

		// /[ˢ�¿��ذ�ť*begin]**********************************************************
		if (page_flash1)
		{
			U8 chSwitch = 0;
			if (para)
			{
				if (dncntonoff_f)
					chSwitch = 3;
				else
					chSwitch = 2;
			}
			else
			{
				if (upcntonoff_f)
					chSwitch = 1;
				else
					chSwitch = 0;
			}
			DrawButton(keyd[KEY_COUNT_ONOFF], (U16 *)(ADD_ICONBMP),
					   keybmpind[KEY_COUNT_ONOFF].border[chSwitch % 2],
					   keybmpind[KEY_COUNT_ONOFF].icon[chSwitch]);
			page_flash1 = 0;
		}
		// /[ˢ�¿��ذ�ť*end]**********************************************************

		// /[ˢ���趨����*begin]**********************************************************
		if (refresh1)
		{
			refresh1 = 0;
			DrawFillRect(rect[RECT_COUNT_SET].xpis, rect[RECT_COUNT_SET].ypis,
						 rect[RECT_COUNT_SET].xsize, rect[RECT_COUNT_SET].ysize,
						 chSelectBtn ? rect[RECT_COUNT_SET].colorB[0] : rect[RECT_COUNT_SET].colorB[1]);
			if (chSelectBtn == 0)
			{
				DrawDottedRect(rect[RECT_COUNT_SET].xpis, rect[RECT_COUNT_SET].ypis,
							   rect[RECT_COUNT_SET].xsize, rect[RECT_COUNT_SET].ysize,
							   rect[RECT_COUNT_SET].colorT[0]);
			}
			DrawMFU32(rect[RECT_COUNT_SET].xpis + (rect[RECT_COUNT_SET].xsize - 12 * 5) / 2,
					  rect[RECT_COUNT_SET].ypis + SAFE_DIST4,
					  cnt_set, 5, rect[RECT_COUNT_SET].colorT[0]);
		}
		// /[ˢ���趨����*end]**********************************************************

		// /[ˢ�µ�ǰ����*begin]**********************************************************
		if (refresh2)
		{
			refresh2 = 0;
			DrawFillRect(rect[RECT_COUNT_NOW].xpis, rect[RECT_COUNT_NOW].ypis,
						 rect[RECT_COUNT_NOW].xsize, rect[RECT_COUNT_NOW].ysize,
						 chSelectBtn ? rect[RECT_COUNT_NOW].colorB[1] : rect[RECT_COUNT_NOW].colorB[0]);
			if (chSelectBtn == 1)
			{
				DrawDottedRect(rect[RECT_COUNT_NOW].xpis, rect[RECT_COUNT_NOW].ypis,
							   rect[RECT_COUNT_NOW].xsize, rect[RECT_COUNT_NOW].ysize,
							   rect[RECT_COUNT_NOW].colorT[0]);
			}
			DrawMFU32(rect[RECT_COUNT_NOW].xpis + (rect[RECT_COUNT_NOW].xsize - 12 * 5) / 2,
					  rect[RECT_COUNT_NOW].ypis + SAFE_DIST4,
					  cnt_now, 5, rect[RECT_COUNT_NOW].colorT[0]);
		}
		// /[ˢ�µ�ǰ����*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_COUNTER)
		{
			plastno = MENU_COUNTER;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

#define KEY_WIND_SCRNSVR 1
#define KEY_WIND_ENTER 2 //1 ȷ��

#define KEY_WIND_MAX 3

#define RECT_WIND_SPEED_TLT 1	 //1 ����оת�ٱ���
#define RECT_WIND_SPEEDSET_TLT 2 //2 ����оת�ٱ���-�趨
#define RECT_WIND_SPEEDNOW_TLT 3 //3 ����оת�ٱ���-��ǰ
#define RECT_WIND_SPEEDSET_STR 4 //4 ����оת������-�趨
#define RECT_WIND_SPEEDNOW_STR 5 //5 ����оת������-�趨
#define RECT_WIND_MODE_TLT 6	 //6 ����о������ʽ����
#define RECT_WIND_MODESET_TLT 7	 //7 ����о������ʽ����-�趨
#define RECT_WIND_MODESET_STR 8	 //8 ����о������ʽ����-�趨
#define RECT_WIND_TIME_TLT 9	 //9 �����Զ�ֹͣ��ʱ����
#define RECT_WIND_TIMESET_TLT 10 //10 �����Զ�ֹͣ��ʱ����-�趨
#define RECT_WIND_TIMENOW_TLT 11 //11 �����Զ�ֹͣ��ʱ����-��ǰ
#define RECT_WIND_TIMESET_STR 12 //12 �����Զ�ֹͣ��ʱ����-�趨
#define RECT_WIND_TIMENOW_STR 13 //13 �����Զ�ֹͣ��ʱ����-�趨

#define RECT_WIND_MAX 14

void Menu_Wind(void)
{

	U32 i;
	//	U16 times50ms = 0xffff;
	U16 tmp_windsp, tmp_windts;
	U8 f;
	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****��־λ��ز���begin****/

	/****��־λ��ز���end***/

	const U8 keynum = {KEY_WIND_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32},					 //1 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //2 ȷ��
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},  //1 ����
		{{KEY_Silver}, {0}, {71}, {1, 0, 0, 0}, {0}}, //2 ȷ��

	};

	const RECTPAD rect[RECT_WIND_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
		{TYPE_X16, TYPE_Y3, 505, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {2}},																	//1 ����оת�ٱ���
		{TYPE_X16, TYPE_Y3 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE24, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {3}},								//2 ����оת�ٱ���-�趨
		{TYPE_X16, TYPE_Y3 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE24, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {4}},						//3 ����оת�ٱ���-��ǰ
		{TYPE_X16 + TYPE_SIZE24 + SAFE_DIST5, TYPE_Y3 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {5}},		//4 ����оת������-�趨
		{TYPE_X16 + TYPE_SIZE24 + SAFE_DIST5, TYPE_Y3 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {5}}, //5 ����оת������-�趨
		{TYPE_X16, 166, 505, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {6}},																		//6 ����о������ʽ����
		{TYPE_X16, 166 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE24, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {3}},									//7 ����о������ʽ����-�趨
		{TYPE_X16 + TYPE_SIZE24 + SAFE_DIST5, 166 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},			//8 ����о������ʽ����-�趨
		{TYPE_X16, 245, 505, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {7}},																		//9 �����Զ�ֹͣ��ʱ����
		{TYPE_X16, 245 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE24, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {8}},									//10 �����Զ�ֹͣ��ʱ����-�趨
		{TYPE_X16, 245 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE24, TYPE_SIZE5, {Color_LightGray}, {Color_Black}, {9}},							//11 �����Զ�ֹͣ��ʱ����-��ǰ
		{TYPE_X16 + TYPE_SIZE24 + SAFE_DIST5, 245 + TYPE_SIZE5 + SAFE_DIST5, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {10}},			//12 �����Զ�ֹͣ��ʱ����-�趨
		{TYPE_X16 + TYPE_SIZE24 + SAFE_DIST5, 245 + (TYPE_SIZE5 + SAFE_DIST5) * 2, TYPE_SIZE14, TYPE_SIZE5, {Color_White}, {Color_Black}, {10}},	//13 �����Զ�ֹͣ��ʱ����-�趨
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_WIND); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ40.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();

	if (P_LAN)
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME_E;
	}
	else
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME;
	}
	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
	Read_D(D_Address[SYS] + SYS_WINDSP, 2, (U16 *)(&YREG[SYS_WINDSP]));
	tmp_windsp = YREG[SYS_WINDSP];
	tmp_windts = YREG[SYS_WINDTS];
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
				 SheetPage, rect[0].index[0], rect[0].colorT[0]); //����
	//��������
	for (i = 1; i < RECT_WIND_MAX; i++)
	{

		DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		if (i == RECT_WIND_SPEED_TLT ||
			i == RECT_WIND_SPEEDSET_TLT ||
			i == RECT_WIND_SPEEDNOW_TLT ||
			i == RECT_WIND_MODE_TLT ||
			i == RECT_WIND_MODESET_TLT ||
			i == RECT_WIND_TIME_TLT ||
			i == RECT_WIND_TIMESET_TLT ||
			i == RECT_WIND_TIMENOW_TLT)
		{
			Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST4,
						 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
						 SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
		}
	}
	Draw24X24FStr(rect[RECT_WIND_SPEED_TLT].xpis + TYPE_SIZE24 + SAFE_DIST5 + (TYPE_SIZE14 - 12) / 2,
				  rect[RECT_WIND_SPEED_TLT].ypis + SAFE_DIST4,
				  "W", rect[RECT_WIND_SPEED_TLT].colorT[0]); //��λ
	Draw24x24Str(rect[RECT_WIND_SPEEDSET_STR].xpis + rect[RECT_WIND_SPEEDSET_STR].xsize + SAFE_DIST4,
				 rect[RECT_WIND_SPEEDSET_STR].ypis + SAFE_DIST4,
				 MEM_TITLE_ADD + rect[RECT_WIND_SPEEDSET_STR].index[0] * TitleLengthMax,
				 SheetPage, rect[RECT_WIND_SPEEDSET_STR].index[0], rect[RECT_WIND_SPEEDSET_STR].colorT[0]); //��λ
	Draw24x24Str(rect[RECT_WIND_SPEEDNOW_STR].xpis + rect[RECT_WIND_SPEEDNOW_STR].xsize + SAFE_DIST4,
				 rect[RECT_WIND_SPEEDNOW_STR].ypis + SAFE_DIST4,
				 MEM_TITLE_ADD + rect[RECT_WIND_SPEEDNOW_STR].index[0] * TitleLengthMax,
				 SheetPage, rect[RECT_WIND_SPEEDNOW_STR].index[0], rect[RECT_WIND_SPEEDNOW_STR].colorT[0]); //��λ

	Draw24X24FStr(rect[RECT_WIND_MODE_TLT].xpis + TYPE_SIZE24 + SAFE_DIST5 + (TYPE_SIZE14 - 12 * 3) / 2,
				  rect[RECT_WIND_MODE_TLT].ypis + SAFE_DIST4,
				  "WSM", rect[RECT_WIND_MODE_TLT].colorT[0]); //��λ

	Draw24X24FStr(rect[RECT_WIND_TIME_TLT].xpis + TYPE_SIZE24 + SAFE_DIST5 + (TYPE_SIZE14 - 12 * 2) / 2,
				  rect[RECT_WIND_TIME_TLT].ypis + SAFE_DIST4,
				  "WT", rect[RECT_WIND_TIME_TLT].colorT[0]); //��λ
	Draw24x24Str(rect[RECT_WIND_TIMESET_STR].xpis + rect[RECT_WIND_TIMESET_STR].xsize + SAFE_DIST4,
				 rect[RECT_WIND_TIMESET_STR].ypis + SAFE_DIST4,
				 MEM_TITLE_ADD + rect[RECT_WIND_TIMESET_STR].index[0] * TitleLengthMax,
				 SheetPage, rect[RECT_WIND_TIMESET_STR].index[0], rect[RECT_WIND_TIMESET_STR].colorT[0]); //��λ
	Draw24x24Str(rect[RECT_WIND_TIMENOW_STR].xpis + rect[RECT_WIND_TIMENOW_STR].xsize + SAFE_DIST4,
				 rect[RECT_WIND_TIMENOW_STR].ypis + SAFE_DIST4,
				 MEM_TITLE_ADD + rect[RECT_WIND_TIMENOW_STR].index[0] * TitleLengthMax,
				 SheetPage, rect[RECT_WIND_TIMENOW_STR].index[0], rect[RECT_WIND_TIMENOW_STR].colorT[0]); //��λ

	DrawMFU32(rect[RECT_WIND_SPEEDSET_STR].xpis + (rect[RECT_WIND_SPEEDSET_STR].xsize - 12 * 4) / 2,
			  rect[RECT_WIND_SPEEDSET_STR].ypis + SAFE_DIST4,
			  P_W, 4, rect[RECT_WIND_SPEEDSET_STR].colorT[0]); // ����о�ٶ��趨ֵ
	DrawMFU32(rect[RECT_WIND_SPEEDNOW_STR].xpis + (rect[RECT_WIND_SPEEDNOW_STR].xsize - 12 * 4) / 2,
			  rect[RECT_WIND_SPEEDNOW_STR].ypis + SAFE_DIST4,
			  tmp_windsp, 4, rect[RECT_WIND_SPEEDNOW_STR].colorT[0]); // ����о�ٶ�����ֵ

	if (P_WSM == 1)
		Draw24X24FStr(rect[RECT_WIND_MODESET_STR].xpis + (rect[RECT_WIND_MODESET_STR].xsize - 12 * 2) / 2,
					  rect[RECT_WIND_MODESET_STR].ypis + SAFE_DIST4,
					  "AL", rect[RECT_WIND_MODESET_STR].colorT[0]);
	else if (P_WSM == 2)
		Draw24X24FStr(rect[RECT_WIND_MODESET_STR].xpis + (rect[RECT_WIND_MODESET_STR].xsize - 12 * 1) / 2,
					  rect[RECT_WIND_MODESET_STR].ypis + SAFE_DIST4,
					  "T", rect[RECT_WIND_MODESET_STR].colorT[0]);
	else
		Draw24X24FStr(rect[RECT_WIND_MODESET_STR].xpis + (rect[RECT_WIND_MODESET_STR].xsize - 12 * 2) / 2,
					  rect[RECT_WIND_MODESET_STR].ypis + SAFE_DIST4,
					  "NO", rect[RECT_WIND_MODESET_STR].colorT[0]);

	DrawMFU32(rect[RECT_WIND_TIMESET_STR].xpis + (rect[RECT_WIND_TIMESET_STR].xsize - 12 * 4) / 2,
			  rect[RECT_WIND_TIMESET_STR].ypis + SAFE_DIST4,
			  P_WT, 4, rect[RECT_WIND_TIMESET_STR].colorT[0]); // ���ߵĶ���ʱ���趨

	DrawMFU32(rect[RECT_WIND_TIMENOW_STR].xpis + (rect[RECT_WIND_TIMENOW_STR].xsize - 12 * 4) / 2,
			  rect[RECT_WIND_TIMENOW_STR].ypis + SAFE_DIST4,
			  tmp_windts, 4, rect[RECT_WIND_TIMENOW_STR].colorT[0]); // ���ߵĶ���ʱ����ʾ

	//����������ʾ
	for (i = 2; i < KEY_WIND_MAX; i++)
	{
		//��Ч����
		//������ɫ���ְ���
		//���水ť
		DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
	ViewPage(usepageno); //������ʾ����

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************
	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			//���������������ܵİ���
			if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > 0) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//�ض�����²���Ч����

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
			{
				keyon = keydata;
				if (keyon == KEY_WIND_SCRNSVR) //�ް�����Ч
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
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}

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
				Times = 0;
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
			case KEY_WIND_SCRNSVR:
				pno = MENU_SCRNSVR;
				break;
			case KEY_WIND_ENTER:
				if (!(S_WIND))
				{
					DREG[D_COMMAND] = C_PFOOTUP; //ѹ������
					DREG[D_CMDSTATUS] = 0;
					DREG[D_CMDPARA1] = 0;
					f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND])); //add on 2009.7.31
					if (f == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						poldno[pnoindex] = MENU_WIND;
						pnoindex++;
						break;
					}
					Read_Status();
					//for(i=0;i<RDTIMES;i++){
					//	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
					//	if((DREG[D_CMDSTATUS]) == 100)break;
					//	DelayMs(WAITTIME);
					//}
					E_WINDDIS();
					f = Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
					if (f == 0)
					{
						pno = MENU_RWHELP;
						g_para1 = 15;
						poldno[pnoindex] = MENU_WIND;
						pnoindex++;
						break;
					}
					else
					{
						pnoindex--;
						pno = poldno[pnoindex];
						// pno = MENU_P2;
						// pnoindex = 0;
					}
				}
				break;
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{
			if (keyon == KEY_THREAD_SCRNSVR) //����Ҫ��ʾ�İ���
			{
			}
			//�ǳ��水��-//������ɫ���ְ���
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;

			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ʾ������
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 50)
		{
			Timerms = 0;
			Read_D(D_Address[SYS] + SYS_WINDSP, 2, (U16 *)(&YREG[SYS_WINDSP]));
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(poldno[0]);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			ErrorMsg(poldno[0]);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[���µ�ǰ�ٶ���ʱ��*begin]**********************************************************
		if (tmp_windsp != YREG[SYS_WINDSP]) // ����о�ٶ�����ֵ
		{
			tmp_windsp = YREG[SYS_WINDSP];
			DrawFillRect(rect[RECT_WIND_SPEEDNOW_STR].xpis, rect[RECT_WIND_SPEEDNOW_STR].ypis,
						 rect[RECT_WIND_SPEEDNOW_STR].xsize, rect[RECT_WIND_SPEEDNOW_STR].ysize,
						 rect[RECT_WIND_SPEEDNOW_STR].colorB[0]);
			DrawMFU32(rect[RECT_WIND_SPEEDNOW_STR].xpis + (rect[RECT_WIND_SPEEDNOW_STR].xsize - 12 * 4) / 2,
					  rect[RECT_WIND_SPEEDNOW_STR].ypis + SAFE_DIST4,
					  tmp_windsp, 4, rect[RECT_WIND_SPEEDNOW_STR].colorT[0]); // ����о�ٶ�����ֵ
		}
		if (tmp_windts != YREG[SYS_WINDTS]) // ����оʱ�䵱ǰֵ
		{
			tmp_windts = YREG[SYS_WINDTS];
			DrawFillRect(rect[RECT_WIND_TIMENOW_STR].xpis, rect[RECT_WIND_TIMENOW_STR].ypis,
						 rect[RECT_WIND_TIMENOW_STR].xsize, rect[RECT_WIND_TIMENOW_STR].ysize,
						 rect[RECT_WIND_TIMENOW_STR].colorB[0]);
			DrawMFU32(rect[RECT_WIND_TIMENOW_STR].xpis + (rect[RECT_WIND_TIMENOW_STR].xsize - 12 * 4) / 2,
					  rect[RECT_WIND_TIMENOW_STR].ypis + SAFE_DIST4,
					  tmp_windts, 4, rect[RECT_WIND_TIMENOW_STR].colorT[0]); // ����о�ٶ�����ֵ
		}
		// /[���µ�ǰ�ٶ���ʱ��*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_WIND)
		{
			plastno = MENU_WIND;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

//------------------------------------------------------------------------------
#define KEY_READ_SCRNSVR 1	//1 ����
#define KEY_READ_BACK 2		//2 ����
#define KEY_READ_OK 3		//3 ȷ��
#define KEY_READ_FLASH 4	//4 �ڴ�
#define KEY_READ_USB 5		//5 USB
#define KEY_READ_SD 6		//6 SD
#define KEY_READ_VIEWPIC 7	//7 ��ʾ��ǰ������Ϣ
#define KEY_READ_PREV 8		//8 ��һҳ
#define KEY_READ_NEXT 9		//9 ��һҳ
#define KEY_READ_BAR 10		//10 �ڴ�ʣ����
#define KEY_READ_COPY 11	//11 ����
#define KEY_READ_DELFILE 12 //12 ɾ��ѡ���ļ�
#define KEY_READ_DREAD 13	//13 ��ת��ȡ
#define KEY_READ_SEL1 14	//14 ѡ��1
#define KEY_READ_SEL2 15	//15 ѡ��2
#define KEY_READ_SEL3 16	//16 ѡ��3
#define KEY_READ_SEL4 17	//17 ѡ��4
#define KEY_READ_SEL5 18	//18 ѡ��5
#define KEY_READ_SEL6 19	//19 ѡ��6
#define KEY_READ_SEL7 20	//20 ѡ��7
#define KEY_READ_SEL8 21	//21 ѡ��8
#define KEY_READ_SEL9 22	//22 ѡ��9
#define KEY_READ_SEL10 23	//23 ѡ��10
#define KEY_READ_SEL11 24	//24 ѡ��11
#define KEY_READ_SEL12 25	//25 ѡ��12

#define KEY_READ_MAX 26

#define RECT_READ_SEL1 1   //1 ѡ��1
#define RECT_READ_SEL2 2   //2 ѡ��2
#define RECT_READ_SEL3 3   //3 ѡ��3
#define RECT_READ_SEL4 4   //4 ѡ��4
#define RECT_READ_SEL5 5   //5 ѡ��5
#define RECT_READ_SEL6 6   //6 ѡ��6
#define RECT_READ_SEL7 7   //7 ѡ��7
#define RECT_READ_SEL8 8   //8 ѡ��8
#define RECT_READ_SEL9 9   //9 ѡ��9
#define RECT_READ_SEL10 10 //10 ѡ��10
#define RECT_READ_SEL11 11 //11 ѡ��11
#define RECT_READ_SEL12 12 //12 ѡ��12
#define RECT_READ_PAGE 13  //13 ��ҳ��ʾ
#define RECT_READ_MOD 14   //14 ģ�鰴����

#define RECT_READ_IMG1 15  //15 ѡ��1
#define RECT_READ_IMG2 16  //16 ѡ��2
#define RECT_READ_IMG3 17  //17 ѡ��3
#define RECT_READ_IMG4 18  //18 ѡ��4
#define RECT_READ_IMG5 19  //19 ѡ��5
#define RECT_READ_IMG6 20  //20 ѡ��6
#define RECT_READ_IMG7 21  //21 ѡ��7
#define RECT_READ_IMG8 22  //22 ѡ��8
#define RECT_READ_IMG9 23  //23 ѡ��9
#define RECT_READ_IMG10 24 //24 ѡ��10
#define RECT_READ_IMG11 25 //25 ѡ��11
#define RECT_READ_IMG12 26 //26 ѡ��12

#define RECT_READ_MAX 27

#define BMP_READ_SEL1 1	  //1 ѡ��1
#define BMP_READ_SEL2 2	  //2 ѡ��2
#define BMP_READ_SEL3 3	  //3 ѡ��3
#define BMP_READ_SEL4 4	  //4 ѡ��4
#define BMP_READ_SEL5 5	  //5 ѡ��5
#define BMP_READ_SEL6 6	  //6 ѡ��6
#define BMP_READ_SEL7 7	  //7 ѡ��7
#define BMP_READ_SEL8 8	  //8 ѡ��8
#define BMP_READ_SEL9 9	  //9 ѡ��9
#define BMP_READ_SEL10 10 //10 ѡ��10
#define BMP_READ_SEL11 11 //11 ѡ��11
#define BMP_READ_SEL12 12 //12 ѡ��12

#define BMP_READ_MAX 13
//------------------------------------------
// Function   : void Menu_Read(void)
// Description: ��ȡ��������
//------------------------------------------
void Menu_Read(void)
{

	U32 i, j, f;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****������ţ��������ƣ������б���ز���begin****/
	//	U16 static tmpFno;																 //��ǰ������ϢHREG[0]�ļ�λ��(�����),������Ż���
	U16 tmpselectdisno;			  //ѡ�еĻ�����Ż���
								  //	U16 temp_feed = 0xffff;															 //���ڻ������� ��ǰ������������
								  //	U16 temp_feed0 = 0xffff;														 //���ڻ������� ��һ��ʱ����������������
								  //	U16 tmpS_2HOME = 0, oldtmpS_2HOME = 0;											 //ϵͳ״̬����-�ص�ԭ��
	U16 tmp_s_2hd = (U16)(S_2HD); //��ԭ��λ�û���
								  //	U8 pattern_flash = 0, data_flash = 1, jog_flash = 1;							 //pattern_flash����ͼ�����Ƹ��±�־λ ��data_flash�������ݸ��±�־λ  ��jog_flash�������ݸ��±�־λ
								  //	U8 bflag = 0;																	 //0���������й����ߣ�1���ػ泬�����ֵ�������� 2���ػ��˻ص��ߣ�3���������й���������ߣ�
	// U8 tmpGroupNO = g_GroupNO, lastGroupNO = g_GroupNO;								 //��ǰ���ģʽ��ţ���һ�����ģʽ���
	tydPatternList PatternList = {1, 0, 0, 12, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}}; //�����б����
	/****������ţ��������ƣ������б���ز���end****/

	/****������ʾ��ز���begin****/
	//	S16 ORG_X = 0, ORG_Y = 0;				//������ʾ�����Ļ������ĵ㣨LCDʵ�����꣩ ԭORG_X = 325, ORG_Y = 262
	// S16 xo = 0, yo = 0;							 //��������Դ�����ĵ㻨���������껺��
	//	S16 xorg = 0, yorg = 0;					//����ͼ������ĵ����� ת���� ��Ļ�����ĵ�����
	S32 /*tscale = 1 , tscale1 = 1, */ Scale = 0; //tscale��ͼ����ʾ��Ҫ���������űȣ�tscale1�����յ������űȣ�Scale���̶������ű�
	// S16 fpx, fpy;								 //ָ�����Ļ�����������
	//	S8 tmuly = 1;							//��������ֵܿ��Y�����ݱȣ��෴��ϵ
	//	S32 vposx0 = 0, vposy0 = 0;				//��һ����ָ������������� ת���� ��Ļ������
	//	S32 vposx, vposy;						//ָ������������� ת���� ��Ļ������
	// S16 xtmp, ytmp;								 //���ʼ��㻺��

	const S16 orgListX[] = {CONVAS_ORG_PX2_1, CONVAS_ORG_PX2_2, CONVAS_ORG_PX2_3, CONVAS_ORG_PX2_4, CONVAS_ORG_PX2_5,
							CONVAS_ORG_PX2_6, CONVAS_ORG_PX2_7, CONVAS_ORG_PX2_8, CONVAS_ORG_PX2_9, CONVAS_ORG_PX2_10,
							CONVAS_ORG_PX2_11, CONVAS_ORG_PX2_12};
	const S16 orgListY[] = {CONVAS_ORG_PY2_1, CONVAS_ORG_PY2_2, CONVAS_ORG_PY2_3, CONVAS_ORG_PY2_4, CONVAS_ORG_PY2_5,
							CONVAS_ORG_PY2_6, CONVAS_ORG_PY2_7, CONVAS_ORG_PY2_8, CONVAS_ORG_PY2_9, CONVAS_ORG_PY2_10,
							CONVAS_ORG_PY2_11, CONVAS_ORG_PY2_12};
	/****������ʾ��ز���end****/

	/****��־λ��ز���begin****/
	// const U8 dissum = 10;
	// U16 oldselectno = 0xff, tmp_fno = F_IND_NO;
	// static U16 selectno = 0;
	// U32 dataadd[FileIndexSize];

	// U32 keyon = 0, keyoff = 0, keydata = 0;
	// U32 i, j; //,ii,jj,ofesst,s
	// U32 dat = 0;
	// U8 Read_Barcode = 0;
	// U8 DisNameRows = 0;
	U8 SDFlag = 0, SDFlag1 = 0, USBFlag = 0, USBFlag1 = 0;
	U8 ReRead = 0; //�˳�ʱ���¶�ȡ�ڴ滨����־
	// U16 totalfiles = 0, curfilenum = 0;
	// U16 maxpage = 1;
	// static U16 selectpage = 1;

	U8 /*refresh0 = 1, refresh1 = 1,*/ refresh2 = 0, refresh4 = 1, refresh5 = 1; //refresh2�����ļ�ѡ�б�ţ� refresh4�����ڴ�ѡ��ť��refresh5���»�����ʾͼ��
	U8 /*disupdata1 = 1, disupdata2 = 0,*/ dis_updata3 = 0;						 //disupdata1:�������±�־ disupdata2����ǰ�ļ���������λ�ø��±�־  disupdata3�����»������ݣ�������
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_READ_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},

		{0, 0, 128, 32},															 //1 ����
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},											 //2 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},										 //3 ȷ��
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},										 //4 �ڴ�
		{TYPE_X10 - KEY_X1, TYPE_Y4, KEY_X1, KEY_Y1},								 //5 USB
		{TYPE_X10 - KEY_X1 * 2, TYPE_Y4, KEY_X1, KEY_Y1},							 //6 SD
		{TYPE_X11, TYPE_Y3, KEY_X1, KEY_Y1},										 //7 ��ʾ��ǰ������Ϣ
		{TYPE_X11, TYPE_Y14, KEY_X1, KEY_Y1},										 //8 ��һҳ
		{TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5 * 2 + TYPE_SIZE4, KEY_X1, KEY_Y1}, //9 ��һҳ
		{TYPE_X10 - KEY_X1 * 3 - SAFE_DIST1, TYPE_Y4, KEY_X1, KEY_Y1},				 //10 �ڴ�ʣ����
		{TYPE_X10 - KEY_X1 * 4 - SAFE_DIST1 * 2, TYPE_Y4, KEY_X1, KEY_Y1},			 //11 ����
		{TYPE_X10 - KEY_X1 * 5 - SAFE_DIST1 * 3, TYPE_Y4, KEY_X1, KEY_Y1},			 //12 ɾ��ѡ���ļ�
		{TYPE_X10 - KEY_X1 * 6 - SAFE_DIST1 * 4, TYPE_Y4, KEY_X1, KEY_Y1},			 //13 ��ת��ȡ
		{CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //14 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //15 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //16 ѡ��3
		{CONVAS_PX2_4, CONVAS_PY2_4, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //17 ѡ��4
		{CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //18 ѡ��5
		{CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //19 ѡ��6
		{CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //20 ѡ��7
		{CONVAS_PX2_8, CONVAS_PY2_8, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //21 ѡ��8
		{CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //22 ѡ��9
		{CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //23 ѡ��10
		{CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //24 ѡ��11
		{CONVAS_PX2_12, CONVAS_PY2_12, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},			 //25 ѡ��12
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},								 //1 ����
		{{KEY_Silver}, {0}, {241, 242}, {2, 0, 0, 0}, {0}},							 //2 ����
		{{KEY_Silver}, {0}, {71}, {3, 0, 0, 0}, {0}},								 //3 ȷ��
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {257, 258, 259}, {4, 0, 0, 0}, {0}}, //4 �ڴ�
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {251, 252, 253}, {5, 0, 0, 0}, {0}}, //5 USB
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {254, 255, 256}, {6, 0, 0, 0}, {0}}, //6 SD
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {11, 12, 13}, {7, 0, 0, 0}, {0}},	 //7 ��ʾ��ǰ������Ϣ
		{{KEY_Silver}, {0}, {219, 220}, {8, 0, 0, 0}, {0}},							 //8 ��һҳ
		{{KEY_Silver}, {0}, {221, 222}, {9, 0, 0, 0}, {0}},							 //9 ��һҳ
		{{KEY_Silver}, {0}, {73}, {11, 0, 0, 0}, {0}},								 //10 �ڴ�ʣ����
		{{KEY_Silver}, {0}, {245}, {12, 0, 0, 0}, {0}},								 //11 ����
		{{KEY_Silver}, {0}, {244}, {10, 0, 0, 0}, {0}},								 //12 ɾ��ѡ���ļ�
		{{KEY_Silver}, {0}, {243}, {13, 0, 0, 0}, {0}},								 //13 ��ת��ȡ
		{{Color_White}, {0}, {Color_Black}, {14, 0, 0, 0}, {0}},					 //14 ѡ��1
		{{Color_White}, {0}, {Color_Black}, {15, 0, 0, 0}, {0}},					 //15 ѡ��2
		{{Color_White}, {0}, {Color_Black}, {16, 0, 0, 0}, {0}},					 //16 ѡ��3
		{{Color_White}, {0}, {Color_Black}, {17, 0, 0, 0}, {0}},					 //17 ѡ��4
		{{Color_White}, {0}, {Color_Black}, {18, 0, 0, 0}, {0}},					 //18 ѡ��5
		{{Color_White}, {0}, {Color_Black}, {19, 0, 0, 0}, {0}},					 //19 ѡ��6
		{{Color_White}, {0}, {Color_Black}, {20, 0, 0, 0}, {0}},					 //20 ѡ��7
		{{Color_White}, {0}, {Color_Black}, {21, 0, 0, 0}, {0}},					 //21 ѡ��8
		{{Color_White}, {0}, {Color_Black}, {22, 0, 0, 0}, {0}},					 //22 ѡ��9
		{{Color_White}, {0}, {Color_Black}, {23, 0, 0, 0}, {0}},					 //23 ѡ��10
		{{Color_White}, {0}, {Color_Black}, {24, 0, 0, 0}, {0}},					 //24 ѡ��11
		{{Color_White}, {0}, {Color_Black}, {25, 0, 0, 0}, {0}},					 //25 ѡ��12
	};

	const RECTPAD rect[RECT_READ_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

		{CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//1 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//2 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//3 ѡ��3
		{CONVAS_PX2_4, CONVAS_PY2_4 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//4 ѡ��4
		{CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//5 ѡ��5
		{CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//6 ѡ��6
		{CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//7 ѡ��7
		{CONVAS_PX2_8, CONVAS_PY2_8 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//8 ѡ��8
		{CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},											//9 ѡ��9
		{CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},										//10 ѡ��10
		{CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},										//11 ѡ��11
		{CONVAS_PX2_12, CONVAS_PY2_12 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},										//12 ѡ��12
		{TYPE_X11, TYPE_Y14 + KEY_Y1 + SAFE_DIST5, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},											//13 ��ҳ��ʾ
		{TYPE_X10 - KEY_X1 * 2 - SAFE_DIST4, TYPE_Y4 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //14 ģ�鰴����
		{CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//15 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//16 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//17 ѡ��3
		{CONVAS_PX2_4, CONVAS_PY2_4, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//18 ѡ��4
		{CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//19 ѡ��5
		{CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//20 ѡ��6
		{CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//21 ѡ��7
		{CONVAS_PX2_8, CONVAS_PY2_8, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//22 ѡ��8
		{CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},														//23 ѡ��9
		{CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},													//24 ѡ��10
		{CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},													//25 ѡ��11
		{CONVAS_PX2_12, CONVAS_PY2_12, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},													//26 ѡ��12

	};

	const BMPPAD disBmp[BMP_READ_MAX] = {
		//icon
		{0, 0, 0, 0, {0}},

		{CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//1 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//2 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//3 ѡ��3
		{CONVAS_PX2_4, CONVAS_PY2_4 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//4 ѡ��4
		{CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//5 ѡ��5
		{CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//6 ѡ��6
		{CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//7 ѡ��7
		{CONVAS_PX2_8, CONVAS_PY2_8 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//8 ѡ��8
		{CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//9 ѡ��9
		{CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //10 ѡ��10
		{CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //11 ѡ��11
		{CONVAS_PX2_12, CONVAS_PY2_12 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //12 ѡ��12

	};
	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_READ); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();
	pat_resetPatRule();
	pat_setPatRule(PATRULES_READ);

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
	if (plastno != MENU_DIRREAD)
	{
		g_disk_memory = 0;
		Read_D(D_Address[SYS] + SYS_LISTTYPE, 1, (U16 *)(&YREG[SYS_LISTTYPE])); // �������ڴ���������
		if (YREG[SYS_LISTTYPE] != 0)											//��ǰ�ļ��������������ڴ� �����������ڴ�����
		{
			ReadPatList(0, MENU_READ);
		}
	}
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ȡģʽ
	//��������
	for (i = 1; i < RECT_READ_MAX; i++)
	{
		if (i < RECT_READ_PAGE)
		{
			BmpFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize);
		}
		else
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		}

		// if (i == RECT_READ_PAGE)
		// {
		// 	DrawRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
		// }
	}
	//����������ʾ
	for (i = 2; i < KEY_READ_MAX; i++)
	{
		//��Ч����

		//����Ҫ��ʾ�İ���

		if (i >= KEY_READ_SEL1 && i <= KEY_READ_SEL12) //�ǳ��水ť
		{
			// DrawButtonDisPat(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
		else //���水ť
		{

			if ((g_disk_memory != 0) && (i == KEY_READ_BAR))
			{
				continue;
			}
			else
			{
				DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
			}
		}
	}
	//����BMP��ʾ
	for (i = 1; i < BMP_READ_MAX; i++)
	{
		DrawFPBMPBTN1(disBmp[i].xpis, disBmp[i].ypis, disBmp[i].xsize, disBmp[i].ysize, disBmp[i].icon[0]);
	}
	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

	ViewPage(usepageno); //������ʾ����

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	// tmpselectdisno = g_oldselectdisno;
	tmpselectdisno = g_Fileno;
	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);	   //��ȡ��ֵ
		if (keydata >= KEY_READ_SEL1 && keydata <= KEY_READ_SEL12) //�ǳ��水��
		{
			f = keydata - KEY_READ_SEL1;
			if (PatternList.selectEnable[f] == 0)
				keydata = 0;
		}
		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����

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
			if ((g_disk_memory != 0) && (keydata == KEY_READ_BAR))
			{
				keydata = 0;
			}
			else if (((keydata == KEY_READ_USB) && (USBFlag == 0)) || ((keydata == KEY_READ_SD) && (SDFlag == 0)))
			{
				keydata = 0;
			}
			else
			{
				if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
				{
					keyon = keydata;
					if (keyon == KEY_READ_SCRNSVR ||
						(keyon >= KEY_READ_SEL1 && keyon <= KEY_READ_SEL12)) //�ް�����Ч
					{
					}
					//�ǳ��水��
					else
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
						beeponf = 1;

						if (g_keyBellT) //����ʱ����������ʱ��
							Beep_On(g_keyBellT);

						switch (keyon) //�������±�����ʾ����ʾ
						{
						default:
							DrawFMenuStr(keybmpind[keyon].titleStr[0]);
							break;
						}

						delay = KEYFT;
					}
					else if (beeponf == 1) //�ڶ��ν���
					{
						beeponf = 2;
						delay = KEYNT;
					}
					else if (beeponf < 20)
					{
						beeponf++;
					}
					//���г����������ܰ����Ĺ���
					Times = 0; //��ǰ
				}
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
			if (keyoff >= KEY_READ_SEL1 && keyoff <= KEY_READ_SEL12)
			{
				// dat = keyoff - KEY_READ_SEL1;

				// if (dataadd[dat] < MAXIREG2)
				// {
				// 	oldselectno = selectno;
				// 	selectno = dat;
				// 	if (oldselectno != selectno)
				// 		refresh2 = 1;

				// 	tmp_fno = dataadd[dat] / FileIndexSize + MINFILENO;

				// 	if (g_disppicflag)
				// 	{
				// 		dis_updata3 = 1;
				// 	}
				// }

				// memset((U8 *)g_distmpName, 0, sizeof(g_distmpName));	   //���ָ���ڴ������ֵ
				// for (i = FileIndexNamepox; i < 32 + FileIndexNamepox; i++) //��ȡ�����ļ�����
				// {
				// 	if (IREG2[(tmp_fno - MINFILENO) * FileIndexSize + i] == 0)
				// 		break;
				// 	g_distmpName[2 * i] = (IREG2[(tmp_fno - MINFILENO) * FileIndexSize + i]) & 0xff;
				// 	g_distmpName[2 * i + 1] = (IREG2[(tmp_fno - MINFILENO) * FileIndexSize + i] >> 8) & 0xff;
				// }
				PatternList.noInPageNow = keyoff - KEY_READ_SEL1;
				PatternList.bFlashSelectFlag = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_READ_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_READ_BACK:
					if (g_disk_memory)
						ReRead = 1;
					else
						ReRead = 0;

					if ((ReadMemorryList(MENU_READ)) == 0) //��ѡ�ж�U��,���˳�����ʱҪ�����ڴ�������
					{
						break;
					}
					pno = poldno[0];
					pnoindex = 0;

					if ((F_IND_NO != g_Fileno) || (ReRead == 1))
					{
						Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
						RestoreSHREG(0x03);
					}
					g_disppicflag = 0;
					break;

				case KEY_READ_OK:
					// tmp_fno = dataadd[selectno] / FileIndexSize + MINFILENO;
					if (tmpselectdisno != 0xffff)
					{
						g_selectnumber = tmpselectdisno;
						g_selectdisno = tmpselectdisno;
					}

					if (PatternList.pageNum == 0) //������ĿΪ0������
					{
						if (g_disk_memory)
						{
							ReadMemorryList(MENU_READ);
						}
						pno = MENU_RWHELP;
						poldno[pnoindex++] = MENU_READ;
						hleppara = 40;
						break;
					}

					// if (g_disppicflag)
					// {
					// 	//pat_f1=1;
					// }
					// else
					// {
					// 	g_NoReadPatf = 2;
					// }
					g_NoReadPatf = 2;
					if (g_disk_memory > 0)
					{
						if (g_disk_memory == 2)
						{
							hleppara = 6;
						}
						else
						{
							hleppara = 80;
						}
						ReadFileIndex(C_RDMEMPATLIST, MENU_READ);
					}
					else
					{
						hleppara = 5;
					}
					pno = MENU_RWHELP;
					poldno[1] = MENU_READ;
					pnoindex = 2;
					break;
				case KEY_READ_FLASH:
					f = ReadFileIndex(C_RDMEMPATLIST, MENU_READ);
					if (f == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						poldno[pnoindex++] = MENU_READ;
						break;
					}
					else
					{
						// disupdata1 = 1;//�޸�
						PatternList.bflashAllFlag = 1;
						PatternList.bFlashPageFlag = 1;
						refresh4 = 1;
						g_disk_memory = 0;
						// BarcodeCompare();
						DrawButton(keyd[KEY_READ_BAR], (U16 *)(ADD_ICONBMP), keybmpind[KEY_READ_BAR].border[0], keybmpind[KEY_READ_BAR].icon[0]);
					}
					break;

				case KEY_READ_USB:
					//Write_Cmd(C_DISK_STATUS,0,0);	//��ȡ����U�̲���״̬
					//for(i=0;i<FORTIMS1;i++){
					//	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));
					//	if((DREG[D_CMDSTATUS] >= 100)||(DREG[D_CMDSTATUS] == 10)){
					//		break;
					//	}
					//	DelayMs(WAITTIME2);
					//}
					//if(DREG[D_CMDSTATUS] == 102 || DREG[D_CMDSTATUS] == 101)
					// 	g_MboardUSB = 1;
					//else
					//	g_MboardUSB = 0;

					//if(g_MboardUSB == 0){   //ʹ�ô����U��
					wdt();
					if (ExtMemory_Status(USBTF) != 0)
					{
						pno = MENU_RWHELP;
						hleppara = 95;
						poldno[pnoindex++] = MENU_READ;
						// pnoindex = 2;
						ReadMemorryList(MENU_READ);
						break;
					}
					Select_ExtMemory(USBTF);
					if (Explore_Disk("0:/", 1))
					{
						pno = MENU_RWHELP;
						hleppara = 40;
						poldno[pnoindex++] = MENU_READ;
						// pnoindex = 2;
						ReadMemorryList(MENU_READ);
						break;
					}
					//}else if(g_MboardUSB == 1){   //ʹ������U��
					//	f = ReadFileIndex(C_RDFLAPATLIST,MENU_READ);
					//	if(f==0){
					//		pno = MENU_RWHELP;hleppara = 15;poldno[1] = MENU_READ;pnoindex = 2;break;
					//	}
					//}
					// disupdata1 = 1;//�޸�
					PatternList.bflashAllFlag = 1;
					PatternList.bFlashPageFlag = 1;
					g_disk_memory = 1;
					refresh4 = 1;
					BmpFillRect(keyd[KEY_READ_BAR].xpis, keyd[KEY_READ_BAR].ypis, keyd[KEY_READ_BAR].xsize, keyd[KEY_READ_BAR].ysize); //����ָ������

					break;

				case KEY_READ_SD:
					//if(ExtMemory_Status(SD)!=0){
					//	pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_READ;pnoindex = 2;
					//	ReadMemorryList(MENU_READ);
					//	break;
					//}
					//Select_ExtMemory(SD);
					//if(Explore_Disk ("1:/", 1)){
					//	pno = MENU_RWHELP;hleppara = 40;poldno[1] = MENU_READ;pnoindex = 2;
					//	ReadMemorryList(MENU_READ);
					//	break;
					//}
					//disupdata1 = 1;
					//PatternList.bFlashPageFlag = 1;
					//refresh4 = 1;
					//g_disk_memory = 2;
					//BmpFillRect(keyd[KEY_READ_BAR].xpis,keyd[KEY_READ_BAR].ypis,keyd[KEY_READ_BAR].xsize,keyd[KEY_READ_BAR].ysize);
					break;
				case KEY_READ_VIEWPIC:

					f = fun_getPatData(tmpselectdisno, g_disk_memory, MENU_READ);
					if (f)
					{
						if (g_disppicflag)
							refresh5 = 1;
						g_init_flag = 1;
						g_data_flash = 1;
					}
					else
					{
						break;
					}

					if (S_2HD) //���ݴ�ԭ����Ϣ������ʾ����������
						AnaPattern(1, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
					else
						AnaPattern(0, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));

					if ((CheckDrawData(MENU_READ, 0)) == 0)
					{ //ͼ�γ���Χ���ܽ��붨λ
						break;
					}
					g_modcordflag = 0;
					g_modcordx = 0;
					g_modcordy = 0;
					g_imgpara = MOIMAGE_VIEW; //1

					g_jogfunction1 = 1;

					g_BorSTflag = 0;
					g_max_feedsum = g_patd.feed_sum;
					g_min_feedsum = 0;
					g_jog_point = 0;
					g_IsLocal = 1;
					g_ImgMove = 1;
					g_errLocal = 0;
					g_CodeEn = 1; //��ʾ����
					g_ExCode = 0;
					g_curExCode = 0;
					g_curPinAng = 0;
					g_curSetPara1 = 0;
					g_curSetPara2 = 0;
					g_pointPinAng = 0;
					g_pointSetPara1 = 0;
					g_pointSetPara2 = 0;
					pno = MENU_MOIMAGE;
					poldno[pnoindex++] = MENU_READ;
					break;

				case KEY_READ_PREV:
					if (PatternList.pageNow > 1)
					{
						PatternList.pageNow--;
						PatternList.bFlashPageFlag = 1;
					}
					break;
				case KEY_READ_NEXT:
					if (PatternList.pageNow < PatternList.pageNum)
					{
						PatternList.pageNow++;
						PatternList.bFlashPageFlag = 1;
					}
					break;
				case KEY_READ_COPY:
					pno = MENU_FTCOPY;
					poldno[pnoindex++] = MENU_READ;
					break;
				case KEY_READ_DELFILE:

					if (tmpselectdisno != 0xffff)
					{
						g_selectnumber = tmpselectdisno;
						//	selectdisno = tmp_fno;
						memset((U8 *)g_distmpName, 0, sizeof(g_distmpName));
						for (i = 0; i < 32; i++)
						{
							if (IREG2[(tmpselectdisno - MINFILENO) * FileIndexSize + FileIndexNamepox + i] == 0)
								break;
							g_distmpName[2 * i] = (IREG2[(tmpselectdisno - MINFILENO) * FileIndexSize + FileIndexNamepox + i]) & 0xff;
							g_distmpName[2 * i + 1] = (IREG2[(tmpselectdisno - MINFILENO) * FileIndexSize + FileIndexNamepox + i] >> 8) & 0xff;
						}
					}

					pno = MENU_RWHELP;
					poldno[pnoindex++] = MENU_READ;
					if (g_disk_memory == 1)
						hleppara = 78; //usb
					else if (g_disk_memory == 2)
						hleppara = 1; //sd
					else
						hleppara = 0; //FLASH

					if ((ReadMemorryList(MENU_READ)) == 0) //��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
					{
						break;
					}
					break;

				case KEY_READ_BAR: //��ȡϵͳ�ڴ�
					hleppara = 9;
					pno = MENU_RWHELP;
					poldno[pnoindex++] == MENU_READ;
					// poldno[1] = MENU_READ;
					// pnoindex = 2;
					Write_Cmd(C_READFSIZE, 0, 0);
					break;
					// case KEY_READ_BARCODE:
					// 	dis_updata3 = 1;
					// 	Read_Barcode = 1;
					// 	break;

				case KEY_READ_DREAD:
					pno = MENU_DIRREAD;
					poldno[pnoindex++] = MENU_READ;
					break;
				}
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			if (keyon == KEY_READ_SCRNSVR ||
				(keyon >= KEY_READ_SEL1 && keyon <= KEY_READ_SEL12)) //�ް�����Ч
			{
			}
			//�ǳ��水��
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}
			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ�ض�ȡģʽ
			Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if ((Timerms > 1000))
		{
			Timerms = 0;
			if (g_disk_memory == 1) //ʹ��U��
			{
				//if(g_MboardUSB==1){		//ʹ������U��
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
				//		if((ReadMemorryList(MENU_READ)) == 0){//��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
				//			return;
				//		}
				//		g_selectdisno = g_Fileno;
				//		Write_Cmd(C_READMEMORYPAT,g_Fileno,0);
				//		RestoreSHREG(0x03);
				//		pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_READ;pnoindex = 2;//�����е�����
				//	}else{
				//		USBFlag =1;
				//	}
				//}else{
				if (ExtMemory_Status(USBTF) != 0) //ʹ�ô����U��
				{
					USBFlag = 0;
					hleppara = 95;
					pno = MENU_RWHELP;
					poldno[pnoindex] = MENU_READ;
					pnoindex++;
					if ((ReadMemorryList(MENU_READ)) == 0)
					{ // ��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
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
				//��ѯһ��SD���������
				//if(ExtMemory_Status(SD)==0){
				//	SDFlag =1;
				//}else{
				//	SDFlag =0;
				//}
			}
			else if (g_disk_memory == 2) // ʹ�ô����SD
			{
				//	if(ExtMemory_Status(SD)!=0){
				//		SDFlag = 0;
				//		pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_READ;pnoindex = 2;
				//		if((ReadMemorryList(MENU_READ)) == 0){		// ��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
				//			return;
				//		}
				//		Write_Cmd(C_READMEMORYPAT,g_Fileno,0);
				//		g_selectdisno = g_Fileno;
				//		RestoreSHREG(0x03);
				//	}else{
				//		SDFlag = 1;
				//	}
				//
				//	//��ѯһ��U�̲������
				//	Write_Cmd(C_DISK_STATUS,0,0);	//��ȡ����U�̲���״̬
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
			else //ʹ�������ڴ�
			{
				//��ѯһ��U�̲������
				//Write_Cmd(C_DISK_STATUS,0,0);	//��ȡ����U�̲���״̬
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

				//��ѯһ��SD���������
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
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[���»������ݣ�������*begin]**********************************************************
		if (dis_updata3)
		{
			U16 chFileNoTemp = 0, cntTemp = 0;
			dis_updata3 = 0;
			// memset((U8 *)SREG, 0, sizeof(SREG));
			for (cntTemp = 0; cntTemp < PatternList.disNumDiv; cntTemp++)
			{

				if (PatternList.dataAdd[cntTemp] == 0xffff)
				{
					continue;
				}
				chFileNoTemp = PatternList.dataAdd[cntTemp] / FileIndexSize + MINFILENO;
				f = fun_getPatData(chFileNoTemp, g_disk_memory, MENU_READ);
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
				// /[���ƻ����������Ϣ*begin]******************************************************************************
				if (refresh5)
				{
					refresh5 = 0;
					if (cntTemp == PatternList.disNumDiv)
					{
						break;
					}

					if (S_2HD) //���ݴ�ԭ����Ϣ������ʾ����������
						AnaPattern(1, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
					else
						AnaPattern(0, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));

					// SetDrawXY(rect[cntTemp + RECT_READ_IMG1].xpis, rect[cntTemp + RECT_READ_IMG1].ypis,
					// 		  rect[cntTemp + RECT_READ_IMG1].xpis + CONVAS_X2, rect[cntTemp + RECT_READ_IMG1].ypis + CONVAS_Y2, 1);
					// Draw_Pattern(PAT_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, ((F_FILELENH << 16) | F_FILELENL), 0, 0, NULL); // ��ʾ���� - �����
					pat_setDrawXY(patRule.h_patConvasIndex, rect[cntTemp + RECT_READ_IMG1].xpis, rect[cntTemp + RECT_READ_IMG1].ypis);
					pat_drawPattern(PATRULES_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, F_FILELEN, 0, 0, NULL);
					// SetDrawXY(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);
				}
				// /[���ƻ����������Ϣ*end]******************************************************************************
				wdt();
			}
		}
		// /[���»������ݣ�������*end]**********************************************************

		// /[�ڴ水������*begin]**********************************************************
		if (refresh4) // SD�� �ڴ水��ѡ�д���
		{
			U8 chSelectMem = 0;
			refresh4 = 0;
			chSelectMem = KEY_READ_FLASH + g_disk_memory;

			for (i = KEY_READ_FLASH; i <= KEY_READ_SD; i++)
			{
				if (i == KEY_READ_USB)
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
							   i == chSelectMem ? keybmpind[i].border[1] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
							   i == chSelectMem ? keybmpind[i].icon[1] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
				else if (i == KEY_READ_SD)
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
							   i == chSelectMem ? keybmpind[i].border[1] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
							   i == chSelectMem ? keybmpind[i].icon[1] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
				else if (i == KEY_READ_FLASH)
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
							   i == chSelectMem ? keybmpind[i].border[1] : keybmpind[i].border[0],
							   i == chSelectMem ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
			}
		}
		// /[�ڴ水������*end]**********************************************************

		// /[���»�����ʾ�б�*begin]**********************************************************
		if (PatternList.bflashAllFlag) //������Ϣ����
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
			for (i = 0; i < MAXIREG2; i += FileIndexSize) //���������洢�ռ䣬��ȡ ���ϻ�����������ǰ�������
			{
				if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
					(((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
				{
					PatternList.patternNum++;
					if ((i / FileIndexSize + MINFILENO) <= tmpselectdisno)
						PatternList.selectNoNow++;
				}
			}

			//ȷ�� ��ǰѡ�����ǰѡ��ҳ��ҳ������
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

		if (PatternList.bFlashSelectFlag) //����ѡ�������
		{
			PatternList.bFlashSelectFlag = 0;

			if (PatternList.dataAdd[PatternList.noInPageNow] == 0xFFFF) //ѡ�п�
			{
				PatternList.noInPageNow = PatternList.noInPageLast;
				return;
			}
			else
			{
				PatternList.noInPageLast = PatternList.noInPageNow;
			}

			tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO; //ת��Ϊ�������disno

			if (tmpselectdisno != 0xffff &&
				tmpselectdisno != g_selectdisno)
			{
				PatternList.selectNoNow = PatternList.noInPageNow + MINFILENO + (PatternList.pageNow - 1) * PatternList.disNumDiv;
				PatternList.selectNoLast = PatternList.selectNoNow;
				PatternList.pageLast = PatternList.pageNow;

				// g_selectdisno = tmpselectdisno;//ֻ�ǹ�ѡ��û��ȷ��
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
			refresh2 = 1;
		}

		if (PatternList.bFlashPageFlag) //������ҳ����
		{

			U32 numA = 0; //��ǰҳ�Ļ�����Ż���
			// U32 numB = 0; //��ǰҳ�Ļ������ƫ����
			U32 addA = 0; //��ǰҳ�Ļ�����Ż���ַ
			U32 addB = 0; //��ǰҳ�Ļ������ƫ�Ƶ�ַ
			U32 patternLen = 0;
			U8 patternType = 0;
			U16 disColor = 0;

			PatternList.bFlashPageFlag = 0;
			refresh2 = 1;
			dis_updata3 = 1;

			//��ʾҳ��
			DrawFillRect(rect[RECT_READ_PAGE].xpis, rect[RECT_READ_PAGE].ypis,
						 rect[RECT_READ_PAGE].xsize, rect[RECT_READ_PAGE].ysize, rect[RECT_READ_PAGE].colorB[0]);
			DrawFU32(rect[RECT_READ_PAGE].xpis + SAFE_DIST2, rect[RECT_READ_PAGE].ypis + SAFE_DIST4,
					 PatternList.pageNow, 3, rect[RECT_READ_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_READ_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_READ_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_READ_PAGE].colorT[0]);
			DrawFU32(rect[RECT_READ_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_READ_PAGE].ypis + SAFE_DIST4,
					 PatternList.pageNum, 3, rect[RECT_READ_PAGE].colorT[0]);

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

			if (numA) //���㵱ǰҳ�Ļ���ַ
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
			if (numA < PatternList.patternNum) //���㵱ǰҳ�ĸ����ļ���ƫ�Ƶ�ַ
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
				for (; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
					PatternList.dataAdd[j] = 0xffff;
			}
			else
			{
				for (j = 0; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
					PatternList.dataAdd[j] = 0xffff;
			}

			for (i = 0; i < PatternList.disNumDiv; i++)
			{
				PatternList.selectList[i] = 0;
			}
			if (PatternList.pageNow == PatternList.pageLast)
			{
				PatternList.selectList[PatternList.noInPageNow] = 1;
				tmpselectdisno = PatternList.dataAdd[PatternList.noInPageNow] / FileIndexSize + MINFILENO; //ת��Ϊ�������disno
			}

			//���ư�ť��Ϣ
			for (i = 0; i < PatternList.disNumDiv; i++)
			{
				// DrawFillRect(keyd[i + KEY_READ_SEL1].xpis + SAFE_DIST2, keyd[i + KEY_READ_SEL1].ypis + SAFE_DIST2,
				// 			 keyd[i + KEY_READ_SEL1].xsize - SAFE_DIST2 * 2, keyd[i + KEY_READ_SEL1].ysize - SAFE_DIST2 * 2,
				// 			 keybmpind[i + KEY_READ_SEL1].icon[0]);
				DrawFillRect(rect[i + RECT_READ_IMG1].xpis, rect[i + RECT_READ_IMG1].ypis,
							 rect[i + RECT_READ_IMG1].xsize, rect[i + RECT_READ_IMG1].ysize,
							 rect[i + RECT_READ_IMG1].colorB[0]);

				BmpFillRect(rect[i + RECT_READ_SEL1].xpis, rect[i + RECT_READ_SEL1].ypis,
							rect[i + RECT_READ_SEL1].xsize + SAFE_DIST6, rect[i + RECT_READ_SEL1].ysize);

				patternLen = ((IREG2[(PatternList.dataAdd[i]) + 1] << 16) | IREG2[(PatternList.dataAdd[i])]);
				patternType = ((IREG2[(PatternList.dataAdd[i]) + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
				disColor = ((PatternList.dataAdd[i] < MAXIREG2) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
							   ? ((patternType != 0) ? MitFontColor : BorFontColor)
							   : keybmpind[i + RECT_READ_SEL1].textColor[0];
				DrawFFileno_16x8(rect[i + RECT_READ_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3, rect[i + RECT_READ_SEL1].ypis + SAFE_DIST1,
								 (PatternList.dataAdd[i] >= MAXIREG2) ? 0 : PatternList.dataAdd[i] / FileIndexSize + MINFILENO, disColor);

				DrawF16Str(rect[i + RECT_READ_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 3,
						   rect[i + RECT_READ_SEL1].ypis + SAFE_DIST1,
						   "@", rect[i + RECT_READ_SEL1].colorT[0]);

				if (PatternList.dataAdd[i] >= MAXIREG2)
				{
					DrawFFilename(rect[i + RECT_READ_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
								  rect[i + RECT_READ_SEL1].ypis + SAFE_DIST4,
								  "NO__DATA",
								  strlen("NO__DATA"), NO_DATAColor, 6, 0, NULL);
					PatternList.selectEnable[i] = 0;
				}
				else
				{
					DrawFFilename(rect[i + RECT_READ_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
								  rect[i + RECT_READ_SEL1].ypis + SAFE_DIST4,
								  (char *)(&IREG2[PatternList.dataAdd[i] + FileIndexNamepox]),
								  64, disColor, 6, 0, NULL);
					PatternList.selectEnable[i] = 1;
				}
			}
			wdt();
		}
		// /[���»�����ʾ�б�*end]**********************************************************

		// /[�ļ�ѡ��BMP����*begin]**********************************************************
		if (refresh2) // �ļ����ѡ�д���
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
		// /[�ļ�ѡ��BMP����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(MENU_READ);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			ErrorMsg(MENU_READ);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[��ȡ���»����б�*begin]**********************************************************
		// if (g_updataIREG)
		// {
		// 	g_updataIREG = 0;
		// 	ReadPatList(0, pno); //�����ڴ�����
		// }
		// /[��ȡ���»����б�*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if ((g_backSREG_f == 1) || (g_backHREG_f == 1))
		{
			BackupSHREG((g_backHREG_f << 1) | g_backSREG_f);
		}

		if (pno != MENU_READ)
		{
			plastno = MENU_READ;
			break;
		}

		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

//------------------------------------------
// Function   : U8 isViewNo(U16 patno, U16 viewno)
// Description: �ж�patno�Ƿ���viewno��Ԫ�أ���return1����return0
//------------------------------------------
U8 isViewNo(U16 patno, U16 viewno)
{
	U8 pnob[3] = {0};
	U8 vnob[3] = {0};
	U8 len = 0;
	U8 i;
	if (viewno >= 100)
		len = 3;
	else if (viewno >= 10)
		len = 2;
	else
		len = 1;

	pnob[0] = patno / 100;
	pnob[1] = (patno % 100) / 10;
	pnob[2] = patno % 10;

	vnob[0] = viewno / 100;
	vnob[1] = (viewno % 100) / 10;
	vnob[2] = viewno % 10;

	if (len == 1) //��1λʱ���ж����������У��Ƿ��������
	{
		for (i = 0; i < 3; i++)
		{
			if (pnob[i] == vnob[2])
			{
				return 1;
			}
		}
	}
	else if (len == 2) //2λʱ���ж����������Ƿ��������
	{
		for (i = 0; i < 2; i++)
		{
			if (pnob[i] == vnob[1] && pnob[i + 1] == vnob[2])
			{
				return 1;
			}
		}
	}
	else if (len == 3) //3λʱ���ж�ȫ���ַ��Ƿ����
	{
		if (pnob[0] == vnob[0] && pnob[1] == vnob[1] && pnob[2] == vnob[2])
		{
			return 1;
		}
	}

	return 0;
}

#define KEY_DIRREAD_SCRNSVR 1 //1 ����
#define KEY_DIRREAD_BACK 2	  //2 ����
#define KEY_DIRREAD_OK 3	  //3 ȷ��
#define KEY_DIRREAD_PREV 4	  //4 ��һҳ
#define KEY_DIRREAD_NEXT 5	  //5 ��һҳ
#define KEY_DIRREAD_SEL1 6	  //6 ѡ��1
#define KEY_DIRREAD_SEL2 7	  //7 ѡ��2
#define KEY_DIRREAD_SEL3 8	  //8 ѡ��3
#define KEY_DIRREAD_SEL4 9	  //9 ѡ��4
#define KEY_DIRREAD_SEL5 10	  //10 ѡ��5
#define KEY_DIRREAD_SEL6 11	  //11 ѡ��6
#define KEY_DIRREAD_SEL7 12	  //12 ѡ��7
#define KEY_DIRREAD_SEL8 13	  //13 ѡ��8
#define KEY_DIRREAD_SEL9 14	  //14 ѡ��9
#define KEY_DIRREAD_CLEAN 15  //15 ���һλ
#define KEY_DIRREAD_0 16	  //16 0
#define KEY_DIRREAD_1 17	  //17 1
#define KEY_DIRREAD_2 18	  //18 2
#define KEY_DIRREAD_3 19	  //19 3
#define KEY_DIRREAD_4 20	  //20 4
#define KEY_DIRREAD_5 21	  //21 5
#define KEY_DIRREAD_6 22	  //22 6
#define KEY_DIRREAD_7 23	  //23 7
#define KEY_DIRREAD_8 24	  //24 8
#define KEY_DIRREAD_9 25	  //25 9

#define KEY_DIRREAD_MAX 26

#define RECT_DIRREAD_SEL1 1		//1 ѡ��1
#define RECT_DIRREAD_SEL2 2		//2 ѡ��2
#define RECT_DIRREAD_SEL3 3		//3 ѡ��3
#define RECT_DIRREAD_SEL4 4		//4 ѡ��4
#define RECT_DIRREAD_SEL5 5		//5 ѡ��5
#define RECT_DIRREAD_SEL6 6		//6 ѡ��6
#define RECT_DIRREAD_SEL7 7		//7 ѡ��7
#define RECT_DIRREAD_SEL8 8		//8 ѡ��8
#define RECT_DIRREAD_SEL9 9		//9 ѡ��9
#define RECT_DIRREAD_PAGE 10	//10 ��ҳ��ʾ
#define RECT_DIRREAD_NUM_TIT 11 //11 ��ť����
#define RECT_DIRREAD_NUM_STR 12 //12 ��ť����
#define RECT_DIRREAD_IMG1 13	//13 ѡ��1
#define RECT_DIRREAD_IMG2 14	//14 ѡ��2
#define RECT_DIRREAD_IMG3 15	//15 ѡ��3
#define RECT_DIRREAD_IMG4 16	//16 ѡ��4
#define RECT_DIRREAD_IMG5 17	//17 ѡ��5
#define RECT_DIRREAD_IMG6 18	//18 ѡ��6
#define RECT_DIRREAD_IMG7 19	//19 ѡ��7
#define RECT_DIRREAD_IMG8 20	//20 ѡ��8
#define RECT_DIRREAD_IMG9 21	//21 ѡ��9

#define RECT_DIRREAD_MAX 22

#define BMP_DIRREAD_SEL1 1 //1 ѡ��1
#define BMP_DIRREAD_SEL2 2 //2 ѡ��2
#define BMP_DIRREAD_SEL3 3 //3 ѡ��3
#define BMP_DIRREAD_SEL4 4 //4 ѡ��4
#define BMP_DIRREAD_SEL5 5 //5 ѡ��5
#define BMP_DIRREAD_SEL6 6 //6 ѡ��6
#define BMP_DIRREAD_SEL7 7 //7 ѡ��7
#define BMP_DIRREAD_SEL8 8 //8 ѡ��8
#define BMP_DIRREAD_SEL9 9 //9 ѡ��9

#define BMP_DIRREAD_MAX 10
//------------------------------------------
// Function   : void Menu_DirRead(void)
// Description: ֱ�Ӷ�ȡ��������
//------------------------------------------
void Menu_DirRead(void)
{

	U32 i, j, f;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****������ţ��������ƣ������б���ز���begin****/
	//	U16 static tmpFno;																 //��ǰ������ϢHREG[0]�ļ�λ��(�����),������Ż���
	//	U16 tmpselectdisno;			  //ѡ�еĻ�����Ż���
	//	U16 temp_feed = 0xffff;															 //���ڻ������� ��ǰ������������
	//	U16 temp_feed0 = 0xffff;														 //���ڻ������� ��һ��ʱ����������������
	//	U16 tmpS_2HOME = 0, oldtmpS_2HOME = 0;											 //ϵͳ״̬����-�ص�ԭ��
	U16 tmp_s_2hd = (U16)(S_2HD); //��ԭ��λ�û���
								  //	U8 pattern_flash = 0, data_flash = 1, jog_flash = 1;							 //pattern_flash����ͼ�����Ƹ��±�־λ ��data_flash�������ݸ��±�־λ  ��jog_flash�������ݸ��±�־λ
								  //	U8 bflag = 0;																	 //0���������й����ߣ�1���ػ泬�����ֵ�������� 2���ػ��˻ص��ߣ�3���������й���������ߣ�
	// U8 tmpGroupNO = g_GroupNO, lastGroupNO = g_GroupNO;								 //��ǰ���ģʽ��ţ���һ�����ģʽ���
	tydPatternList PatternList = {1, 0, 0, 9, 0, 0, 1, 1, 1, 0, 0, 0, 0, {0}, {0}}; //�����б����
	/****������ţ��������ƣ������б���ز���end****/

	/****������ʾ��ز���begin****/
	//	S16 ORG_X = 0, ORG_Y = 0;				//������ʾ�����Ļ������ĵ㣨LCDʵ�����꣩ ԭORG_X = 325, ORG_Y = 262
	// S16 xo = 0, yo = 0;							 //��������Դ�����ĵ㻨���������껺��
	//	S16 xorg = 0, yorg = 0;					//����ͼ������ĵ����� ת���� ��Ļ�����ĵ�����
	S32 /*tscale = 1 , tscale1 = 1, */ Scale = 0; //tscale��ͼ����ʾ��Ҫ���������űȣ�tscale1�����յ������űȣ�Scale���̶������ű�
	// S16 fpx, fpy;								 //ָ�����Ļ�����������
	//	S8 tmuly = 1;							//��������ֵܿ��Y�����ݱȣ��෴��ϵ
	//	S32 vposx0 = 0, vposy0 = 0;				//��һ����ָ������������� ת���� ��Ļ������
	//	S32 vposx, vposy;						//ָ������������� ת���� ��Ļ������
	// S16 xtmp, ytmp;								 //���ʼ��㻺��

	const S16 orgListX[] = {CONVAS_ORG_PX2_1, CONVAS_ORG_PX2_2, CONVAS_ORG_PX2_3, CONVAS_ORG_PX2_5,
							CONVAS_ORG_PX2_6, CONVAS_ORG_PX2_7, CONVAS_ORG_PX2_9, CONVAS_ORG_PX2_10,
							CONVAS_ORG_PX2_11};
	const S16 orgListY[] = {CONVAS_ORG_PY2_1, CONVAS_ORG_PY2_2, CONVAS_ORG_PY2_3, CONVAS_ORG_PY2_5,
							CONVAS_ORG_PY2_6, CONVAS_ORG_PY2_7, CONVAS_ORG_PY2_9, CONVAS_ORG_PY2_10,
							CONVAS_ORG_PY2_11};
	/****������ʾ��ز���end****/

	/****��־λ��ز���begin****/
	U16 patno = 0 /*, tmppatno = 0*/;
	U8 disall = 1;
	//	U16 totalfiles = 0, clrfiles = 0;
	// U8 maxpage = 0, selectpage = 0;
	// U8 vienos = 0;
	// U16 tvpno[10] = {0}; //��ǰҳ���
	//	U8 ReRead = 0; //�˳�ʱ���¶�ȡ�ڴ滨����־
	U8 /*dis_updata = 1,*/ dis_updata1 = 0 /*, dis_updata2 = 0*/;

	U8 refresh2 = 0 /*, refresh4 = 1*/, refresh5 = 1;
	U8 dis_updata3 = 0; //disupdata1:�������±�־ disupdata2����ǰ�ļ���������λ�ø��±�־  disupdata3���������ݸ��±�־
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_DIRREAD_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},

		{0, 0, 128, 32},												   //1 ����
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},								   //2 ����
		{TYPE_X11, TYPE_Y3, KEY_X1, KEY_Y1},							   //3 ȷ��
		{331, TYPE_Y4, KEY_X1, KEY_Y1},									   //4 ��һҳ
		{331 + (KEY_X1 + SAFE_DIST5) * 2, TYPE_Y4, KEY_X1, KEY_Y1},		   //5 ��һҳ
		{CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //6 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //7 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //8 ѡ��3
		{CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //9 ѡ��4
		{CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //10 ѡ��5
		{CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //11 ѡ��6
		{CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5},   //12 ѡ��7
		{CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5}, //13 ѡ��8
		{CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2 + TYPE_SIZE5}, //14 ѡ��9

		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},		  //15 ���һλ
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},							  //16 0
		{TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},														  //17 1
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13, KEY_X2, KEY_Y2},									  //18 2
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13, KEY_X2, KEY_Y2},							  //19 3
		{TYPE_X15, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},									  //20 4
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},			  //21 5
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},		  //22 6
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},							  //23 7
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},		  //24 8
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //25 9
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {44, 0, 0, 0}, {0}},			 //1 ����
		{{KEY_Silver}, {0}, {72}, {45, 0, 0, 0}, {0}},			 //2 ����
		{{KEY_Silver}, {0}, {71}, {3, 0, 0, 0}, {0}},			 //3 ȷ��
		{{KEY_Silver}, {0}, {223, 224}, {47, 0, 0, 0}, {0}},	 //8 ��һҳ
		{{KEY_Silver}, {0}, {225, 226}, {48, 0, 0, 0}, {0}},	 //9 ��һҳ
		{{Color_White}, {0}, {Color_Black}, {49, 0, 0, 0}, {0}}, //14 ѡ��1
		{{Color_White}, {0}, {Color_Black}, {50, 0, 0, 0}, {0}}, //15 ѡ��2
		{{Color_White}, {0}, {Color_Black}, {51, 0, 0, 0}, {0}}, //16 ѡ��3
		{{Color_White}, {0}, {Color_Black}, {52, 0, 0, 0}, {0}}, //17 ѡ��4
		{{Color_White}, {0}, {Color_Black}, {53, 0, 0, 0}, {0}}, //18 ѡ��5
		{{Color_White}, {0}, {Color_Black}, {54, 0, 0, 0}, {0}}, //19 ѡ��6
		{{Color_White}, {0}, {Color_Black}, {55, 0, 0, 0}, {0}}, //20 ѡ��7
		{{Color_White}, {0}, {Color_Black}, {56, 0, 0, 0}, {0}}, //21 ѡ��8
		{{Color_White}, {0}, {Color_Black}, {57, 0, 0, 0}, {0}}, //22 ѡ��9

		{{KEY_Silver}, {0}, {113}, {46, 0, 0, 0}, {0}}, //8 �������
		{{KEY_Silver}, {0}, {91}, {0, 0, 0, 0}, {0}},	//11 0
		{{KEY_Silver}, {0}, {92}, {0, 0, 0, 0}, {0}},	//12 1
		{{KEY_Silver}, {0}, {93}, {0, 0, 0, 0}, {0}},	//13 2
		{{KEY_Silver}, {0}, {94}, {0, 0, 0, 0}, {0}},	//14 3
		{{KEY_Silver}, {0}, {95}, {0, 0, 0, 0}, {0}},	//15 4
		{{KEY_Silver}, {0}, {96}, {0, 0, 0, 0}, {0}},	//16 5
		{{KEY_Silver}, {0}, {97}, {0, 0, 0, 0}, {0}},	//17 6
		{{KEY_Silver}, {0}, {98}, {0, 0, 0, 0}, {0}},	//18 7
		{{KEY_Silver}, {0}, {99}, {0, 0, 0, 0}, {0}},	//19 8
		{{KEY_Silver}, {0}, {100}, {0, 0, 0, 0}, {0}},	//20 9

	};

	const RECTPAD rect[RECT_DIRREAD_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {43}},

		{CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //1 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //2 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //3 ѡ��3
		{CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //4 ѡ��4
		{CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //5 ѡ��5
		{CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //6 ѡ��6
		{CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},						 //7 ѡ��7
		{CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},					 //8 ѡ��8
		{CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2, CONVAS_X2, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},					 //9 ѡ��9
		{331 + KEY_X1 + SAFE_DIST5, TYPE_Y4 + (KEY_Y1 - TYPE_SIZE4) / 2, KEY_X1, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //10 ��ҳ��ʾ
		{TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {59}},										 //11 ��ű���
		{TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}},		 //12 �������
		{CONVAS_PX2_1, CONVAS_PY2_1, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //13 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //14 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //15 ѡ��3
		{CONVAS_PX2_5, CONVAS_PY2_5, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //16 ѡ��4
		{CONVAS_PX2_6, CONVAS_PY2_6, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //17 ѡ��5
		{CONVAS_PX2_7, CONVAS_PY2_7, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //18 ѡ��6
		{CONVAS_PX2_9, CONVAS_PY2_9, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},									 //19 ѡ��7
		{CONVAS_PX2_10, CONVAS_PY2_10, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},								 //20 ѡ��8
		{CONVAS_PX2_11, CONVAS_PY2_11, CONVAS_X2, CONVAS_Y2, {Color_Black}, {Color_Black}, {0}},								 //21 ѡ��9
	};

	const BMPPAD disBmp[BMP_DIRREAD_MAX] = {
		//icon
		{0, 0, 0, 0, {0}},

		{CONVAS_PX2_1, CONVAS_PY2_1 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//1 ѡ��1
		{CONVAS_PX2_2, CONVAS_PY2_2 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//2 ѡ��2
		{CONVAS_PX2_3, CONVAS_PY2_3 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//3 ѡ��3
		{CONVAS_PX2_5, CONVAS_PY2_5 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//4 ѡ��4
		{CONVAS_PX2_6, CONVAS_PY2_6 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//5 ѡ��5
		{CONVAS_PX2_7, CONVAS_PY2_7 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//6 ѡ��6
		{CONVAS_PX2_9, CONVAS_PY2_9 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}},	//7 ѡ��7
		{CONVAS_PX2_10, CONVAS_PY2_10 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //8 ѡ��8
		{CONVAS_PX2_11, CONVAS_PY2_11 + CONVAS_Y2 + SAFE_DIST1, TYPE_SIZE26, TYPE_SIZE26, {836, 837, 838}}, //9 ѡ��9

	};
	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_READ); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();
	pat_resetPatRule();
	pat_setPatRule(PATRULES_READ);

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ȡģʽ
	//��������
	for (i = 1; i < RECT_DIRREAD_MAX; i++)
	{
		if (i <= RECT_DIRREAD_SEL9)
		{
			BmpFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize);
		}
		else
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
			if (i == RECT_DIRREAD_NUM_STR)
			{
				DrawDottedRect(rect[RECT_DIRREAD_NUM_STR].xpis, rect[RECT_DIRREAD_NUM_STR].ypis,
							   rect[RECT_DIRREAD_NUM_STR].xsize, rect[RECT_DIRREAD_NUM_STR].ysize,
							   rect[RECT_DIRREAD_NUM_STR].colorT[0]);
			}
			else if (i == RECT_DIRREAD_NUM_TIT)
			{
				Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST4,
							 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
							 SheetPage, rect[i].index[0], rect[i].colorT[0]); //��ȡģʽ
			}
		}
	}
	//����������ʾ
	for (i = 2; i < KEY_DIRREAD_MAX; i++)
	{
		//��Ч����

		//����Ҫ��ʾ�İ���
		if (i == KEY_DIRREAD_OK) //�ǳ��水ť
		{
		}
		else if (i >= KEY_DIRREAD_SEL1 && i <= KEY_DIRREAD_SEL9) //�ǳ��水ť
		{
			// DrawButtonDisPat(keyd[i], keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
		else //���水ť
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
	}
	//����BMP��ʾ
	for (i = 1; i < BMP_DIRREAD_MAX; i++)
	{
		DrawFPBMPBTN1(disBmp[i].xpis, disBmp[i].ypis, disBmp[i].xsize, disBmp[i].ysize, disBmp[i].icon[0]);
	}
	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

	ViewPage(usepageno); //������ʾ����

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	//	tmpselectdisno = g_oldselectdisno;
	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);			//��ȡ��ֵ
		if (keyon >= KEY_DIRREAD_SEL1 && keyon <= KEY_DIRREAD_SEL9) //�ǳ��水��
		{
			f = keydata - KEY_DIRREAD_SEL1;
			if (PatternList.selectEnable[f] == 0)
				keydata = 0;
		}
		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����

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

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
			{
				keyon = keydata;
				if (keyon == KEY_DIRREAD_SCRNSVR || keyon == KEY_DIRREAD_OK ||
					(keyon >= KEY_DIRREAD_SEL1 && keyon <= KEY_DIRREAD_SEL9)) //�ް�����Ч
				{
				}
				//�ǳ��水��
				else
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
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					case KEY_DIRREAD_0:
					case KEY_DIRREAD_1:
					case KEY_DIRREAD_2:
					case KEY_DIRREAD_3:
					case KEY_DIRREAD_4:
					case KEY_DIRREAD_5:
					case KEY_DIRREAD_6:
					case KEY_DIRREAD_7:
					case KEY_DIRREAD_8:
					case KEY_DIRREAD_9:
						BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
						DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_DIRREAD_0, 1, rect[0].colorT[0]);
						break;
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}

					delay = KEYFT;
				}
				else if (beeponf == 1) //�ڶ��ν���
				{
					beeponf = 2;
					delay = KEYNT;
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
			if ((keyoff >= KEY_DIRREAD_0) && (keyoff <= KEY_DIRREAD_9))
			{
				patno = (patno % 100) * 10 + keyoff - KEY_DIRREAD_0;
				disall = 0;
				dis_updata1 = 1;
			}
			else if (keyoff >= KEY_DIRREAD_SEL1 && keyoff <= KEY_DIRREAD_SEL9)
			{
				// PatternList.noInPageNow = keyoff - KEY_DIRREAD_SEL1;
				// PatternList.bFlashSelectFlag = 1;

				if (PatternList.dataAdd[keyoff - KEY_DIRREAD_SEL1] != 0xffff)
				{
					g_selectnumber = PatternList.dataAdd[keyoff - KEY_DIRREAD_SEL1] / FileIndexSize + MINFILENO;
					g_selectdisno = g_selectnumber;
					g_NoReadPatf = 2; //��Ҫ�ض���������
					if (g_disk_memory > 0)
					{
						if (g_disk_memory == 2)
						{
							hleppara = 6;
						}
						else
						{
							hleppara = 80;
						}
						ReadFileIndex(C_RDMEMPATLIST, MENU_READ);
					}
					else
					{
						hleppara = 5;
					}
					pno = MENU_RWHELP;
					poldno[pnoindex] = MENU_DIRREAD;
					pnoindex++;
				}
			}
			else
			{
				switch (keyoff)
				{
				case KEY_DIRREAD_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_DIRREAD_BACK:
					pnoindex--;
					pno = poldno[pnoindex];
					break;

				case KEY_DIRREAD_CLEAN:
					if (patno)
					{
						patno /= 10;
						dis_updata1 = 1;
					}
					else if (disall == 0)
					{
						disall = 1;
						dis_updata1 = 1;
						// PatternList.bflashAllFlag = 1;
						// DrawFillRect(194, 152, 58, 32, Color_Blue);
					}
					break;

				case KEY_DIRREAD_PREV:
					if (PatternList.pageNow > 1)
					{
						PatternList.pageNow--;
						PatternList.bFlashPageFlag = 1;
					}
					break;
				case KEY_DIRREAD_NEXT:
					if (PatternList.pageNow < PatternList.pageNum)
					{
						PatternList.pageNow++;
						PatternList.bFlashPageFlag = 1;
					}
					break;

					// case KEY_DIRREAD_PREV:
					// 	if (selectpage > 1)
					// 	{
					// 		selectpage--;
					// 		BmpFillRect(418, 556, 39, 24);
					// 		DrawMFU32(418, 556, selectpage, 3, Color_Black);
					// 		dis_updata2 = 1;
					// 	}
					// 	break;

					// case KEY_DIRREAD_NEXT:
					// 	if (selectpage < maxpage)
					// 	{
					// 		selectpage++;
					// 		BmpFillRect(418, 556, 39, 24);
					// 		DrawMFU32(418, 556, selectpage, 3, Color_Black);

					// 		dis_updata2 = 1;
					// 	}
					// 	break;
				}
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			if (keyon == KEY_DIRREAD_SCRNSVR || keyon == KEY_DIRREAD_OK ||
				(keyon >= KEY_DIRREAD_SEL1 && keyon <= KEY_DIRREAD_SEL9)) //�ް�����Ч
			{
			}
			//�ǳ��水��
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}
			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ�ض�ȡģʽ
			Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if ((Timerms > 1000))
		{
			Timerms = 0;
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[���»�������*begin]**********************************************************
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
				f = fun_getPatData(chFileNoTemp, g_disk_memory, MENU_DIRREAD);
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
				// /[���ƻ����������Ϣ*begin]******************************************************************************
				if (refresh5)
				{
					refresh5 = 0;
					if (cntTemp == PatternList.disNumDiv)
					{
						break;
					}

					if (S_2HD) //���ݴ�ԭ����Ϣ������ʾ����������
						AnaPattern(1, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));
					else
						AnaPattern(0, F_FILETYPE, (((F_FILELENH) << 16) | F_FILELENL));

					// SetDrawXY(rect[cntTemp + RECT_DIRREAD_IMG1].xpis, rect[cntTemp + RECT_DIRREAD_IMG1].ypis,
					// 		  rect[cntTemp + RECT_DIRREAD_IMG1].xpis + CONVAS_X2, rect[cntTemp + RECT_DIRREAD_IMG1].ypis + CONVAS_Y2, 1);

					// Draw_Pattern(PAT_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, ((F_FILELENH << 16) | F_FILELENL), 0, 0, NULL); // ��ʾ���� - �����
					// SetDrawXY(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);

					pat_setDrawXY(patRule.h_patConvasIndex, rect[cntTemp + RECT_DIRREAD_IMG1].xpis, rect[cntTemp + RECT_DIRREAD_IMG1].ypis);
					pat_drawPattern(PATRULES_READ, Scale, orgListX[cntTemp], orgListY[cntTemp], F_FILETYPE, F_FILELEN, 0, 0, NULL);
				}
				// /[���ƻ����������Ϣ*end]******************************************************************************
				wdt();
			}
		}
		// /[���»�������*end]**********************************************************

		// /[���»�����ʾ�б�*begin]**********************************************************
		if (PatternList.bflashAllFlag) //������Ϣ����
		{
			PatternList.bflashAllFlag = 0;
			PatternList.bFlashPageFlag = 1;

			PatternList.selectNoNow = 0;
			PatternList.patternNum = 0;
			for (i = 0; i < MAXIREG2; i += FileIndexSize) //���������洢�ռ䣬��ȡ ���ϻ�����������ǰ�������
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
					if (disall) //��ʾȫ��
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

			//ȷ�� ��ǰѡ�����ǰѡ��ҳ��ҳ������
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

		if (PatternList.bFlashPageFlag) //������ҳ����
		{

			U32 numA = 0; //��ǰҳ�Ļ�����Ż���
			// U32 numB = 0; //��ǰҳ�Ļ������ƫ����
			U32 addA = 0; //��ǰҳ�Ļ�����Ż���ַ
			U32 addB = 0; //��ǰҳ�Ļ������ƫ�Ƶ�ַ
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
			// if (PatternList.pageNow == PatternList.pageLast)
			// {
			// 	PatternList.selectList[PatternList.noInPageNow] = 1;
			// }

			// clrfiles = 0;
			// vienos = 0;
			// for (i = 0; i < PatternList.disNumDiv; i++)
			// {
			// 	// DrawFillRect(keyd[KEY_DIRREAD_SEL0 + i].xpis + 19, keyd[KEY_DIRREAD_SEL0 + i].ypis + 12, 58, 32, mCOLOR ? Color_Silver : Color_White1);
			// 	tvpno[i] = 0;
			// }
			// for (i = 0; i < MAXIREG2; i += FileIndexSize)
			// {
			// 	if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) &&
			// 		(((IREG2[i + 1] << 16) | IREG2[i]) <= 2 * MAXSREG))
			// 	{
			// 		U16 tmpPatNo = 0;
			// 		tmpPatNo = i / FileIndexSize + MINFILENO;
			// 		if (disall) //��ʾȫ��
			// 		{
			// 			clrfiles++;
			// 			if (clrfiles > ((selectpage - 1) * PatternList.disNumDiv))
			// 			{
			// 				// DrawLFU32(keyd[KEY_DIRREAD_SEL0 + vienos].xpis + 19, keyd[KEY_DIRREAD_SEL0 + vienos].ypis + 12, tmpPatNo, 3, Color_Black);
			// 				tvpno[vienos] = tmpPatNo;
			// 				vienos++;
			// 				if (vienos == PatternList.disNumDiv)
			// 					break;
			// 			}
			// 		}
			// 		else
			// 		{
			// 			if (isViewNo(tmpPatNo, patno))
			// 			{
			// 				clrfiles++;
			// 				if (clrfiles > ((selectpage - 1) * PatternList.disNumDiv))
			// 				{
			// 					// DrawLFU32(keyd[KEY_DIRREAD_SEL0 + vienos].xpis + 19, keyd[KEY_DIRREAD_SEL0 + vienos].ypis + 12, tmpPatNo, 3, Color_Black);
			// 					tvpno[vienos] = tmpPatNo;
			// 					vienos++;
			// 					if (vienos == PatternList.disNumDiv)
			// 						break;
			// 				}
			// 			}
			// 		}
			// 	}
			// }

			//��ʾҳ��
			DrawFillRect(rect[RECT_DIRREAD_PAGE].xpis, rect[RECT_DIRREAD_PAGE].ypis,
						 rect[RECT_DIRREAD_PAGE].xsize, rect[RECT_DIRREAD_PAGE].ysize, rect[RECT_DIRREAD_PAGE].colorB[0]);
			DrawFU32(rect[RECT_DIRREAD_PAGE].xpis + SAFE_DIST2, rect[RECT_DIRREAD_PAGE].ypis + SAFE_DIST4,
					 PatternList.pageNow, 3, rect[RECT_DIRREAD_PAGE].colorT[0]);
			DrawF16Str(rect[RECT_DIRREAD_PAGE].xpis + SAFE_DIST2 + 9 * 3, rect[RECT_DIRREAD_PAGE].ypis + SAFE_DIST4,
					   "/", rect[RECT_DIRREAD_PAGE].colorT[0]);
			DrawFU32(rect[RECT_DIRREAD_PAGE].xpis + SAFE_DIST2 + 9 * 4, rect[RECT_DIRREAD_PAGE].ypis + SAFE_DIST4,
					 PatternList.pageNum, 3, rect[RECT_DIRREAD_PAGE].colorT[0]);

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

			if (numA) //���㵱ǰҳ�Ļ���ַ
			{
				for (j = 0, addA = 0; addA < MAXIREG2; addA += FileIndexSize)
				{
					if ((((IREG2[addA + 1] << 16) | IREG2[addA]) > 0) &&
						(((IREG2[addA + 1] << 16) | IREG2[addA]) <= 2 * MAXSREG))
					{
						U16 tmpPatNo = 0;
						tmpPatNo = addA / FileIndexSize + MINFILENO;
						if (disall) //��ʾȫ��
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
			if (numA < PatternList.patternNum) //���㵱ǰҳ�ĸ����ļ���ƫ�Ƶ�ַ
			{
				for (j = 0; addB < MAXIREG2; addB += FileIndexSize)
				{
					if ((((IREG2[addB + 1] << 16) | IREG2[addB]) > 0) &&
						(((IREG2[addB + 1] << 16) | IREG2[addB]) <= 2 * MAXSREG))
					{

						U16 tmpPatNo = 0;
						tmpPatNo = addB / FileIndexSize + MINFILENO;
						if (disall) //��ʾȫ��
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
				for (; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
					PatternList.dataAdd[j] = 0xffff;
			}
			else
			{
				for (j = 0; j < PatternList.disNumDiv; j++) //��ǰҳ��ʣ���ļ���ַд��
					PatternList.dataAdd[j] = 0xffff;
			}
			//���ư�ť��Ϣ
			for (i = 0; i < PatternList.disNumDiv; i++)
			{
				// DrawFillRect(keyd[i + KEY_DIRREAD_SEL1].xpis + SAFE_DIST2, keyd[i + KEY_DIRREAD_SEL1].ypis + SAFE_DIST2,
				// 			 keyd[i + KEY_DIRREAD_SEL1].xsize - SAFE_DIST2 * 2, keyd[i + KEY_DIRREAD_SEL1].ysize - SAFE_DIST2 * 2,
				// 			 keybmpind[i + KEY_DIRREAD_SEL1].icon[0]);
				DrawFillRect(rect[i + RECT_DIRREAD_IMG1].xpis, rect[i + RECT_DIRREAD_IMG1].ypis,
							 rect[i + RECT_DIRREAD_IMG1].xsize, rect[i + RECT_DIRREAD_IMG1].ysize,
							 rect[i + RECT_DIRREAD_IMG1].colorB[0]);

				BmpFillRect(rect[i + RECT_DIRREAD_SEL1].xpis, rect[i + RECT_DIRREAD_SEL1].ypis,
							rect[i + RECT_DIRREAD_SEL1].xsize + SAFE_DIST6, rect[i + RECT_DIRREAD_SEL1].ysize);

				patternLen = ((IREG2[(PatternList.dataAdd[i]) + 1] << 16) | IREG2[(PatternList.dataAdd[i])]);
				patternType = ((IREG2[(PatternList.dataAdd[i]) + FileIndexSize - 1] & 0xff00) >> 8) & 0xff;
				disColor = ((PatternList.dataAdd[i] < MAXIREG2) && ((patternLen > 0) && (patternLen < 2 * MAXSREG)))
							   ? ((patternType != 0) ? MitFontColor : BorFontColor)
							   : keybmpind[i + RECT_DIRREAD_SEL1].textColor[0];
				DrawFFileno_16x8(rect[i + RECT_DIRREAD_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3, rect[i + RECT_DIRREAD_SEL1].ypis + SAFE_DIST1,
								 (PatternList.dataAdd[i] >= MAXIREG2) ? 0 : PatternList.dataAdd[i] / FileIndexSize + MINFILENO, disColor);

				DrawF16Str(rect[i + RECT_DIRREAD_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 3,
						   rect[i + RECT_DIRREAD_SEL1].ypis + SAFE_DIST1,
						   "@", rect[i + RECT_DIRREAD_SEL1].colorT[0]);

				if (PatternList.dataAdd[i] >= MAXIREG2)
				{
					DrawFFilename(rect[i + RECT_DIRREAD_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
								  rect[i + RECT_DIRREAD_SEL1].ypis + SAFE_DIST4,
								  "NO__DATA",
								  strlen("NO__DATA"), NO_DATAColor, 6, 0, NULL);
					PatternList.selectEnable[i] = 0;
				}
				else
				{
					DrawFFilename(rect[i + RECT_DIRREAD_SEL1].xpis + TYPE_SIZE26 + SAFE_DIST3 + 9 * 4,
								  rect[i + RECT_DIRREAD_SEL1].ypis + SAFE_DIST4,
								  (char *)(&IREG2[PatternList.dataAdd[i] + FileIndexNamepox]),
								  64, disColor, 6, 0, NULL);
					PatternList.selectEnable[i] = 1;
				}
			}
			wdt();
		}
		// /[���»�����ʾ�б�*end]**********************************************************

		// /[�ļ�ѡ��BMP����*begin]**********************************************************
		if (refresh2) // �ļ����ѡ�д���
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
		// /[�ļ�ѡ��BMP����*end]**********************************************************

		// /[���ָ���*begin]**********************************************************
		if (dis_updata1)
		{
			dis_updata1 = 0;
			PatternList.bflashAllFlag = 1;
			DrawFillRect(rect[RECT_DIRREAD_NUM_STR].xpis, rect[RECT_DIRREAD_NUM_STR].ypis,
						 rect[RECT_DIRREAD_NUM_STR].xsize, rect[RECT_DIRREAD_NUM_STR].ysize,
						 rect[RECT_DIRREAD_NUM_STR].colorB[0]);
			DrawDottedRect(rect[RECT_DIRREAD_NUM_STR].xpis, rect[RECT_DIRREAD_NUM_STR].ypis,
						   rect[RECT_DIRREAD_NUM_STR].xsize, rect[RECT_DIRREAD_NUM_STR].ysize,
						   rect[RECT_DIRREAD_NUM_STR].colorT[0]);
			if (disall == 0)
			{
				DrawMFU32Z(rect[RECT_DIRREAD_NUM_STR].xpis + (rect[RECT_DIRREAD_NUM_STR].xsize - 12 * 3) / 2,
						   rect[RECT_DIRREAD_NUM_STR].ypis + SAFE_DIST4, patno, 3, rect[RECT_DIRREAD_NUM_STR].colorT[0]);
			}
		}
		// /[���ָ���*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		// if (key_on_flag1)
		// 	ErrorMsg(MENU_READ);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		// if (beeponf == 0)
		// {
		// 	ErrorMsg(MENU_READ);
		// 	if (Timer_ms >= 30000) //30720
		// 	{
		// 		Timer_ms = 0;
		// 	}
		// }
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************

		if (pno != MENU_DIRREAD)
		{
			plastno = MENU_DIRREAD;
			break;
		}

		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

//------------------------------------------------------------------------------
#define KEY_WRITE_SCRNSVR 1	 //1 ����
#define KEY_WRITE_BACK 2	 //2 ����
#define KEY_WRITE_OK 3		 //3 ȷ��
#define KEY_WRITE_FLASH 4	 //4 �ڴ�
#define KEY_WRITE_USB 5		 //5 USB
#define KEY_WRITE_SD 6		 //6 SD
#define KEY_WRITE_CAPACITY 7 //7 ʣ������
#define KEY_WRITE_DEL 8		 //8 ɾ��һλ
#define KEY_WRITE_ABCPY 9	 //9 �����л�
#define KEY_WRITE_0 10		 //10 0
#define KEY_WRITE_1 11		 //11 1
#define KEY_WRITE_2 12		 //12 2
#define KEY_WRITE_3 13		 //13 3
#define KEY_WRITE_4 14		 //14 4
#define KEY_WRITE_5 15		 //15 5
#define KEY_WRITE_6 16		 //16 6
#define KEY_WRITE_7 17		 //17 7
#define KEY_WRITE_8 18		 //18 8
#define KEY_WRITE_9 19		 //19 9
#define KEY_WRITE_A 20		 //20 A
#define KEY_WRITE_B 21		 //21 B
#define KEY_WRITE_C 22		 //22 C
#define KEY_WRITE_D 23		 //23 D
#define KEY_WRITE_E 24		 //24 E
#define KEY_WRITE_F 25		 //25 F
#define KEY_WRITE_G 26		 //26 G
#define KEY_WRITE_H 27		 //27 H
#define KEY_WRITE_I 28		 //28 I
#define KEY_WRITE_J 29		 //29 J
#define KEY_WRITE_K 30		 //30 K
#define KEY_WRITE_L 31		 //31 L
#define KEY_WRITE_M 32		 //32 M
#define KEY_WRITE_N 33		 //33 N
#define KEY_WRITE_O 34		 //34 O
#define KEY_WRITE_P 35		 //35 P
#define KEY_WRITE_Q 36		 //36 Q
#define KEY_WRITE_R 37		 //37 R
#define KEY_WRITE_S 38		 //38 S
#define KEY_WRITE_T 39		 //39 T
#define KEY_WRITE_U 40		 //40 U
#define KEY_WRITE_V 41		 //41 V
#define KEY_WRITE_W 42		 //42 W
#define KEY_WRITE_X 43		 //43 X
#define KEY_WRITE_Y 44		 //44 Y
#define KEY_WRITE_Z 45		 //45 Z
#define KEY_WRITE__ 46		 //46 _
#define KEY_WRITE_NAME 47	 //47 ����
#define KEY_WRITE_NUM 48	 //48 ����
#define KEY_WRITE_SEL1 49	 //49 ����ѡ�� 1
#define KEY_WRITE_SEL2 50	 //50 ����ѡ�� 2
#define KEY_WRITE_SEL3 51	 //51 ����ѡ�� 3
#define KEY_WRITE_SEL4 52	 //52 ����ѡ�� 4
#define KEY_WRITE_SEL5 53	 //53 ����ѡ�� 5
#define KEY_WRITE_SEL6 54	 //54 ����ѡ�� 6
#define KEY_WRITE_SEL7 55	 //55 ����ѡ�� 7
#define KEY_WRITE_SEL8 56	 //56 ����ѡ�� 8
#define KEY_WRITE_SEL9 57	 //57 ����ѡ�� 9
#define KEY_WRITE_SEL10 58	 //58 ����ѡ�� 10
#define KEY_WRITE_SELPRES 59 //59 ���� ��λ
#define KEY_WRITE_SELPREV 60 //60 ���� ǰһҳ
#define KEY_WRITE_SELNEXT 61 //61 ���� ��һҳ

#define KEY_WRITE_MAX 62

#define RECT_WRITE_NAME_TIT 1 //1 ���Ʊ���
#define RECT_WRITE_NUM_TIT 2  //2 ��ű���
#define RECT_WRITE_MOD 3	  //3 ģ�鰴����

#define RECT_WRITE_MAX 4

#define SAVEADD_MIT 2
#define SAVEADD_SEW 100
//------------------------------------------
// Function   : void Menu_Write(void)
// Description: д�滨������
//------------------------------------------
void Menu_Write(void)
{

	U32 i, f;

	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****������ţ��������ƣ������б���ز���begin****/
	//	U16 static tmpFno;																 //��ǰ������ϢHREG[0]�ļ�λ��(�����),������Ż���
	// U16 tmpselectdisno; //ѡ�еĻ�����Ż���
	/****������ţ��������ƣ������б���ز���end****/

	/****��־λ��ز���begin****/

	U8 SDFlag = 0, SDFlag1 = 0, USBFlag = 0, USBFlag1 = 0;

	U8 pinyinflag = 0; //0:��д��ĸ  1��Сд��ĸ   2������  3-������ˢ������ѡ��
	U8 pingyingbuf[7];
	U16 tmphzadd = 0;
	U16 tmppysel[11];
	U8 SaveType = 1;

	U16 dat1 = 0;
	// U8 dis_updata1 = 1; //dis_updata1:�ػ汣�����Ͱ�ť
	U8 refresh1 = 1, /*refresh2 = 0,*/ refresh3 = 1, refresh4 = 0, refresh5 = 0, refresh6 = 0;
	//refresh1 ��������ˢ��,refresh2 ������ʾA-Z,refresh3 ˢ���ڴ�����ť,refresh4 ����ˢ��,refresh5 ����ˢ��,refresh6 ����ѡ���б�ˢ��
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_WRITE_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},

		{0, 0, 128, 32},												  //1 ����
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},								  //2 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},							  //3 ȷ��
		{TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},							  //4 �ڴ�
		{TYPE_X10 - KEY_X1, TYPE_Y4, KEY_X1, KEY_Y1},					  //5 USB
		{TYPE_X10 - KEY_X1 * 2, TYPE_Y4, KEY_X1, KEY_Y1},				  //6 SD
		{TYPE_X10 - KEY_X1 * 3 - SAFE_DIST1, TYPE_Y4, KEY_X1, KEY_Y1},	  //7 ʣ������
		{TYPE_X23, TYPE_Y19, KEY_X5, KEY_Y5},							  //8 ɾ��һλ
		{TYPE_X24, TYPE_Y19, KEY_X5, KEY_Y5},							  //9 �����л�
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 9, TYPE_Y18, KEY_X2, KEY_Y2}, //10 0
		{TYPE_X16, TYPE_Y18, KEY_X2, KEY_Y2},							  //11 1
		{TYPE_X16 + KEY_X2 + SAFE_DIST2, TYPE_Y18, KEY_X2, KEY_Y2},		  //12 2
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 2, TYPE_Y18, KEY_X2, KEY_Y2}, //13 3
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y18, KEY_X2, KEY_Y2}, //14 4
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y18, KEY_X2, KEY_Y2}, //15 5
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y18, KEY_X2, KEY_Y2}, //16 6
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y18, KEY_X2, KEY_Y2}, //17 7
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 7, TYPE_Y18, KEY_X2, KEY_Y2}, //18 8
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 8, TYPE_Y18, KEY_X2, KEY_Y2}, //19 9

		{TYPE_X20, TYPE_Y21, KEY_X2, KEY_Y2},							  //20 A
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
		{TYPE_X16, TYPE_Y20, KEY_X2, KEY_Y2},							  //36 Q
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y20, KEY_X2, KEY_Y2}, //37 R
		{TYPE_X20 + KEY_X2 + SAFE_DIST2, TYPE_Y21, KEY_X2, KEY_Y2},		  //38 S
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 4, TYPE_Y20, KEY_X2, KEY_Y2}, //39 T
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 6, TYPE_Y20, KEY_X2, KEY_Y2}, //40 U
		{TYPE_X21 + (KEY_X2 + SAFE_DIST2) * 3, TYPE_Y22, KEY_X2, KEY_Y2}, //41 V
		{TYPE_X16 + KEY_X2 + SAFE_DIST2, TYPE_Y20, KEY_X2, KEY_Y2},		  //42 W
		{TYPE_X21 + KEY_X2 + SAFE_DIST2, TYPE_Y22, KEY_X2, KEY_Y2},		  //43 X
		{TYPE_X16 + (KEY_X2 + SAFE_DIST2) * 5, TYPE_Y20, KEY_X2, KEY_Y2}, //44 Y
		{TYPE_X21, TYPE_Y22, KEY_X2, KEY_Y2},							  //45 Z
		{TYPE_X16, TYPE_Y19, KEY_X2, KEY_Y2},							  //46 _

		{TYPE_X22, TYPE_Y2, TYPE_SIZE23, TYPE_SIZE5},  //47 ����
		{TYPE_X22, TYPE_Y16, TYPE_SIZE23, TYPE_SIZE5}, //48 ����

		{TYPE_X22, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},									 //49 ����ѡ�� 1
		{TYPE_X22 + TYPE_SIZE27 + SAFE_DIST2, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},		 //50 ����ѡ�� 2
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 2, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //51 ����ѡ�� 3
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 3, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //52 ����ѡ�� 4
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 4, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //53 ����ѡ�� 5
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 5, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //54 ����ѡ�� 6
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 6, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //55 ����ѡ�� 7
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 7, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //56 ����ѡ�� 8
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 8, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //57 ����ѡ�� 9
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 9, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7},	 //58 ����ѡ�� 10
		{TYPE_X16, TYPE_Y17, TYPE_SIZE11, TYPE_SIZE7},									 //59 ���� ��λ
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 10, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7}, //60 ���� ǰһҳ
		{TYPE_X22 + (TYPE_SIZE27 + SAFE_DIST2) * 11, TYPE_Y17, TYPE_SIZE27, TYPE_SIZE7}, //61 ���� ��һҳ
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},

		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},								 //1 ����
		{{KEY_Silver}, {0}, {241, 242}, {2, 0, 0, 0}, {0}},							 //2 ����
		{{KEY_Silver}, {0}, {71}, {3, 0, 0, 0}, {0}},								 //3 ȷ��
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {257, 258, 259}, {4, 0, 0, 0}, {0}}, //4 �ڴ�
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {251, 252, 253}, {5, 0, 0, 0}, {0}}, //5 USB
		{{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {254, 255, 256}, {6, 0, 0, 0}, {0}}, //6 SD
		{{KEY_Silver}, {0}, {73}, {7, 0, 0, 0}, {0}},								 //7 ʣ������
		{{KEY_Silver}, {0}, {189}, {8, 0, 0, 0}, {0}},								 //8 ɾ��һλ
		{{KEY_Silver}, {0}, {147, 148, 149}, {69, 9, 62, 0}, {0}},					 //9 �����л�
		{{KEY_Silver, KEY_Gray}, {0}, {91, 101}, {10, 0, 0, 0}, {0}},				 //10 0
		{{KEY_Silver, KEY_Gray}, {0}, {92, 102}, {11, 0, 0, 0}, {0}},				 //11 1
		{{KEY_Silver, KEY_Gray}, {0}, {93, 103}, {12, 0, 0, 0}, {0}},				 //12 2
		{{KEY_Silver, KEY_Gray}, {0}, {94, 104}, {13, 0, 0, 0}, {0}},				 //13 3
		{{KEY_Silver, KEY_Gray}, {0}, {95, 105}, {14, 0, 0, 0}, {0}},				 //14 4
		{{KEY_Silver, KEY_Gray}, {0}, {96, 106}, {15, 0, 0, 0}, {0}},				 //15 5
		{{KEY_Silver, KEY_Gray}, {0}, {97, 107}, {16, 0, 0, 0}, {0}},				 //16 6
		{{KEY_Silver, KEY_Gray}, {0}, {98, 108}, {17, 0, 0, 0}, {0}},				 //17 7
		{{KEY_Silver, KEY_Gray}, {0}, {99, 109}, {18, 0, 0, 0}, {0}},				 //18 8
		{{KEY_Silver, KEY_Gray}, {0}, {100, 110}, {19, 0, 0, 0}, {0}},				 //19 9

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
		{{KEY_Silver}, {0}, {173}, {46, 0, 0, 0}, {0}}, //46 _

		{{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black}, {47}, {0}}, //47 ����
		{{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black}, {48}, {0}}, //48 ����

		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {49}, {0}}, //49 ����ѡ�� 1
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {50}, {0}}, //50 ����ѡ�� 2
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {51}, {0}}, //51 ����ѡ�� 3
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {52}, {0}}, //52 ����ѡ�� 4
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {53}, {0}}, //53 ����ѡ�� 5
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {54}, {0}}, //54 ����ѡ�� 6
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {55}, {0}}, //55 ����ѡ�� 7
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {56}, {0}}, //56 ����ѡ�� 8
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {57}, {0}}, //57 ����ѡ�� 9
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {58}, {0}}, //58 ����ѡ�� 10
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {59}, {0}}, //59 ���� ��λ
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {60}, {0}}, //60 ���� ǰһҳ
		{{KEY_Silver}, {Color_White, Color_Yellow, Color_White}, {Color_BlueViolet, Color_KeyBlue}, {61}, {0}}, //61 ���� ��һҳ
	};

	const RECTPAD rect[RECT_WRITE_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

		{TYPE_X16, TYPE_Y2, TYPE_SIZE11, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {64}},															//1 ���Ʊ���
		{TYPE_X16, TYPE_Y16, TYPE_SIZE11, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {65}},														//2 ��ű���
		{TYPE_X10 - KEY_X1 * 2 - SAFE_DIST4, TYPE_Y4 - SAFE_DIST4, KEY_X1 * 3 + SAFE_DIST1, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_Black}, {0}}, //3 ģ�鰴����

	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_WRITE); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
	g_disk_memory = 0; //Ĭ�������ڴ�
	pingyingbuf[6] = 0;
	for (i = 0; i < 6; i++)
	{
		pingyingbuf[i] = 0x20;
	}
	for (i = 0; i < 10; i++)
	{
		tmppysel[i] = 0x2020;
	}

	memset((U8 *)g_patname, 0, sizeof(g_patname));
	g_patnameindex = 0;
	g_patnumberindex = 0;

	for (i = 0; i < 32; i++)
	{

		if ((HREG[32 + i] & 0xff) == 0)
			break;
		if ((((HREG[32 + i] & 0xff) >= 0x30) && ((HREG[32 + i] & 0xff) <= 0x39)) || (((HREG[32 + i] & 0xff) >= 0x41) && ((HREG[32 + i] & 0xff) <= 0x5a)) ||
			(((HREG[32 + i] & 0xff) >= 0x61) && ((HREG[32 + i] & 0xff) <= 0x7a)) || ((HREG[32 + i] & 0xff) > 0x7f) ||
			((HREG[32 + i] & 0xff) == 0x5f))
		{
			g_patname[g_patnameindex++] = (HREG[32 + i]) & 0xff;
		}

		if (((HREG[32 + i] >> 8) & 0xff) == 0)
		{
			break;
		}

		if (((((HREG[32 + i] >> 8) & 0xff) >= 0x30) && (((HREG[32 + i] >> 8) & 0xff) <= 0x39)) || ((((HREG[32 + i] >> 8) & 0xff) >= 0x41) && (((HREG[32 + i] >> 8) & 0xff) <= 0x5a)) ||
			((((HREG[32 + i] >> 8) & 0xff) >= 0x61) && (((HREG[32 + i] >> 8) & 0xff) <= 0x7a)) || (((HREG[32 + i] >> 8) & 0xff) > 0x7f) ||
			(((HREG[32 + i] >> 8) & 0xff) == 0x5f))
		{
			g_patname[g_patnameindex++] = (HREG[32 + i] >> 8) & 0xff;
		}
	}
	g_patnumber[0] = F_IND_NO / 100 % 10;
	g_patnumber[1] = F_IND_NO / 10 % 10;
	g_patnumber[2] = F_IND_NO % 10;
	g_patnumberindex = 3;
	SaveType = F_FILETYPE; //���滨������Ϊ����ʱ����
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //д��ģʽ
	//��������
	for (i = 1; i < RECT_WRITE_MAX; i++)
	{

		DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		if (i == RECT_WRITE_NUM_TIT || i == RECT_WRITE_NAME_TIT)
		{
			Draw24x24Str(rect[i].xpis + (rect[i].xsize - 24 * 2) / 2,
						 rect[i].ypis + SAFE_DIST4,
						 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
						 SheetPage, rect[i].index[0], rect[i].colorT[0]);
		}
	}
	//����������ʾ
	for (i = 2; i < KEY_WRITE_MAX; i++)
	{
		//��Ч����
		if (i >= KEY_WRITE_SEL1 && i <= KEY_WRITE_SELNEXT) //����Ҫ��ʾ�İ���
		{
			continue;
		}
		else if (i == KEY_WRITE_NAME || i == KEY_WRITE_NUM) //�ǳ��水ť
		{
			DrawButton3(keyd[i], keybmpind[i].icon[0], 0);
		}
		else //���水ť
		{

			if ((g_disk_memory != 0) && (i == KEY_WRITE_CAPACITY))
			{
				continue;
			}
			else
			{
				DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
			}
		}
	}
	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
	if ((poldno[pnoindex - 1] == MENU_P1) || (poldno[pnoindex - 1] == MENU_P2)) //���������������ֹͣ��������
	{
		E_RUNDIS();												//��ֹ����
		Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE); //��д״̬�Ĵ��� д�뾲ֹ״̬
		//��ȡ��ǰ��ѹ�Ż�׼ֵ
		Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
		g_pfootd.base = YREG[SYS_PFOOTSET];

		Read_D(D_Address[SYS] + SYS_LFORCESET, 1, (U16 *)(&YREG[SYS_LFORCESET]));
		g_tdtensiond.base = YREG[SYS_LFORCESET];
	}
	if (YREG[SYS_LISTTYPE] != 0) //��ǰ�ļ��������������ڴ� �����������ڴ�����
	{
		ReadPatList(0, MENU_WRITE);
	}
	g_backIREG_f = 1;	 //�ս�����Ҫ����һ��IREG����
	ViewPage(usepageno); //������ʾ����

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����
		if (keydata >= KEY_WRITE_SEL1 && keydata <= KEY_WRITE_SELNEXT && (pinyinflag <= 2))
		{
			keydata = 0;
		}

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			//���������������ܵİ���
			if (keyon == KEY_WRITE_DEL)
			{
			}
			else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			if ((g_disk_memory != 0) && (keydata == KEY_WRITE_CAPACITY))
			{
				keydata = 0;
			}
			else if (((keydata == KEY_WRITE_USB) && (USBFlag == 0)) || ((keydata == KEY_WRITE_SD) && (SDFlag == 0)))
			{
				keydata = 0;
			}
			else
			{
				if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
				{
					keyon = keydata;
					if (keyon == KEY_WRITE_SCRNSVR) //�ް�����Ч
					{
					}
					else if (keyon >= KEY_WRITE_SEL1 && keyon <= KEY_WRITE_SELNEXT)
					{
						DrawFillRect(keyd[keyon].xpis, keyd[keyon].ypis,
									 keyd[keyon].xsize, keyd[keyon].ysize, keybmpind[keyon].icon[1]);
						if (keyon == KEY_WRITE_SELPRES)
						{
							Draw24X24FStr(keyd[keyon].xpis + SAFE_DIST3, keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
										  (char *)(pingyingbuf), keybmpind[keyon].textColor[2]);
						}
						else if (keyon == KEY_WRITE_SELPREV)
						{
							Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
										  keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
										  "<", keybmpind[keyon].textColor[2]);
						}
						else if (keyon == KEY_WRITE_SELNEXT)
						{

							Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
										  keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
										  ">", keybmpind[keyon].textColor[2]);
						}
						else if ((keyon >= KEY_WRITE_SEL1) && (keyon <= KEY_WRITE_SEL10))
						{
							Draw24X24FPY(keyd[keyon].xpis + (keyd[keyon].xsize - 24) / 2,
										 keyd[keyon].ypis + (keyd[keyon].ysize - 24) / 2,
										 (char *)(&tmppysel[keyon - KEY_WRITE_SEL1]), 2, keybmpind[keyon].textColor[2]);
						}
					}
					else if (keyon == KEY_WRITE_NUM || keyon == KEY_WRITE_NAME) //�ǳ��水��
					{
						DrawButtonOnZoomIn(keyd[keyon], 2);
					}
					else
					{
						DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
					}

					if ((keyon >= KEY_WRITE_0 && keyon <= KEY_WRITE_9) && pinyinflag > 2)
					{
						DrawFillRect(keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xpis, keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ypis,
									 keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xsize, keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ysize, keybmpind[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].icon[1]);

						Draw24X24FPY(keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xpis + (keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xsize - 24) / 2,
									 keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ypis + (keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ysize - 24) / 2,
									 (char *)(&tmppysel[keyon - KEY_WRITE_0]), 2, keybmpind[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].textColor[2]);
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
						beeponf = 1;

						if (g_keyBellT) //����ʱ����������ʱ��
							Beep_On(g_keyBellT);

						switch (keyon) //�������±�����ʾ����ʾ
						{
						case KEY_WRITE_ABCPY:
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
					else if (beeponf == 1) //�ڶ��ν���
					{
						beeponf = 2;

						if (keyon == KEY_WRITE_DEL) //���г����������ܰ����Ĺ���
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
					if (keyon == KEY_WRITE_DEL) //���г����������ܰ����Ĺ���
					{
						keyoff = keyon;
					}
					Times = 0; //��ǰ
				}
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
			if ((keyoff >= KEY_WRITE_0) && (keyoff <= KEY_WRITE_9))
			{						  //���ּ� 0 - 9
				if (g_namenumberflag) //ѡ������
				{
					if (g_patnumberindex < 3)
					{
						g_patnumber[g_patnumberindex] = keyoff - KEY_WRITE_0 + 0x30;
						g_patnumberindex++;
					}
					else
					{
						g_patnumber[0] = g_patnumber[1];
						g_patnumber[1] = g_patnumber[2];
						g_patnumber[2] = keyoff - KEY_WRITE_0 + 0x30;
					}
					refresh5 = 1;
				}
				else // ѡ������
				{
					if (pinyinflag <= 2)
					{
						if (g_patnameindex < 63) //����-���֣�Ӣ��
						{
							g_patname[g_patnameindex] = keyoff - KEY_WRITE_0 + 0x30;
							g_patnameindex++;
						}
						refresh4 = 1;
					}
					else //����-����
					{
						if (g_patnameindex < 62) //��Ҫ�޸�
						{
							//GB2312���뷶Χ��A1A1��FEFE�����к��ֱ��뷶Χ��B0A1-F7FE
							if ((tmppysel[keyoff - KEY_WRITE_0] & 0x80) && ((tmppysel[keyoff - KEY_WRITE_0] >> 8) & 0x80))
							{ //��Ч�����ַ��ű���
								g_patname[g_patnameindex] = tmppysel[keyoff - KEY_WRITE_0];
								g_patnameindex++;
								g_patname[g_patnameindex] = tmppysel[keyoff - KEY_WRITE_0] >> 8;
								g_patnameindex++;
							}

							while (pinyinflag > 2) //��������ѡ����ʾʱ
							{
								pinyinflag--;
								pingyingbuf[pinyinflag - 2] = 0x20; //���
							}
							refresh6 = 1;
							tmphzadd = FinePYFirstAdd(pingyingbuf, 0x0);
							ReadHzData(tmppysel, 10, tmphzadd);
						}
						refresh4 = 1;
					}
				}
			}
			else if ((keyoff >= KEY_WRITE_A) && (keyoff <= KEY_WRITE__))
			{							   // ����		��д��ĸ   �ַ�����
				if (g_namenumberflag == 0) //��������״̬�£���Ч
				{
					if (pinyinflag == 0)
					{
						if (g_patnameindex < 63)
						{
							if (keyoff == KEY_WRITE__)
								g_patname[g_patnameindex] = 0x5f;
							else
								g_patname[g_patnameindex] = keyoff - KEY_WRITE_A + 0x41; //��д
							g_patnameindex++;
						}
						refresh4 = 1;
					}
					else if (pinyinflag == 1)
					{ // ����		Сд��ĸ
						if (g_patnameindex < 63)
						{
							if (keyoff == KEY_WRITE__)
								g_patname[g_patnameindex] = 0x5f;
							else
								g_patname[g_patnameindex] = keyoff - KEY_WRITE_A + 0x61; //Сд
							g_patnameindex++;
						}
						refresh4 = 1;
					}
					else if ((pinyinflag < 8) && (keyoff < KEY_WRITE__))
					{
						pingyingbuf[pinyinflag - 2] = keyoff - KEY_WRITE_A + 0x61; //Сд
						pinyinflag++;
						refresh6 = 1;
						tmphzadd = FinePYFirstAdd(pingyingbuf, 0x0);
						ReadHzData(tmppysel, 10, tmphzadd);
					}
				}
			}
			else if ((keyoff >= KEY_WRITE_SEL1) && (keyoff < KEY_WRITE_MAX))
			{ //����     ����
				if ((keyoff >= KEY_WRITE_SEL1) && (keyoff <= KEY_WRITE_SEL10))
				{
					if (g_patnameindex < 62)
					{
						////GB2312���뷶Χ��A1A1��FEFE�����к��ֱ��뷶Χ��B0A1-F7FE
						if ((tmppysel[keyoff - KEY_WRITE_SEL1] & 0x80) && ((tmppysel[keyoff - KEY_WRITE_SEL1] >> 8) & 0x80))
						{ //��Ч�����ַ��ű���
							g_patname[g_patnameindex] = tmppysel[keyoff - KEY_WRITE_SEL1];
							g_patnameindex++;
							g_patname[g_patnameindex] = tmppysel[keyoff - KEY_WRITE_SEL1] >> 8;
							g_patnameindex++;
						}
						while (pinyinflag > 2)
						{
							pinyinflag--;
							pingyingbuf[pinyinflag - 2] = 0x20;
						}
						refresh6 = 1;
						tmphzadd = FinePYFirstAdd(pingyingbuf, 0x0);
						ReadHzData(tmppysel, 10, tmphzadd);
					}
					refresh4 = 1;
				}
				else
				{
					switch (keyoff)
					{
					case KEY_WRITE_SELPRES:
						tmphzadd = FinePYFirstAdd(pingyingbuf, 0x0);
						ReadHzData(tmppysel, 10, tmphzadd);
						refresh6 = 1;
						break;
					case KEY_WRITE_SELPREV:
						if (tmphzadd > 10)
							tmphzadd -= 10;
						else
							tmphzadd = 1;
						ReadHzData(tmppysel, 10, tmphzadd);
						refresh6 = 1;
						break;
					case KEY_WRITE_SELNEXT:
						if ((tmphzadd + 10) <= HZLIBSIZE)
							tmphzadd += 10;
						ReadHzData(tmppysel, 10, tmphzadd);
						refresh6 = 1;
						break;
					default:

						break;
					}
				}
			}
			else
			{
				switch (keyoff)
				{
				case KEY_READ_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_WRITE_BACK:
					if ((ReadMemorryList(MENU_WRITE)) == 0)
					{ //��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
						break;
					}
					if ((poldno[pnoindex - 2] != MENU_CURSOR) && (poldno[pnoindex - 2] != MENU_MODIFY) && (poldno[pnoindex - 2] != MENU_CONVERT))
					{
						RestoreSHREG(0x03);
					}
					pnoindex--;
					pno = poldno[pnoindex];
					break;

				case KEY_WRITE_OK:

					if (g_patnumberindex == 0)
					{
						g_patnumber[0] = 0;
						g_patnumber[1] = 0;
						g_patnumber[2] = 0;
					}
					else if (g_patnumberindex == 1)
					{
						g_patnumber[2] = g_patnumber[0] - 0x30;
						g_patnumber[1] = 0;
						g_patnumber[0] = 0;
					}
					else if (g_patnumberindex == 2)
					{
						g_patnumber[2] = g_patnumber[1] - 0x30;
						g_patnumber[1] = g_patnumber[0] - 0x30;
						g_patnumber[0] = 0;
					}
					else
					{
						g_patnumber[0] -= 0x30;
						g_patnumber[1] -= 0x30;
						g_patnumber[2] -= 0x30;
					}
					g_exwriteType = 0;
					SaveType = PAT_MIT; //ǿ�Ʊ���Ϊ�����ʽ
					if ((g_enwritef == 1) && (SaveType != F_FILETYPE))
					{ //����ת��
						switch (SaveType)
						{
						case PAT_MIT: //����Ϊ�����ʽ
							f = Bor_To_Mit(g_save_len);
							if (f == 0)
							{ //����������8000
								pno = MENU_RWHELP;
								hleppara = 98;
								return;
							}
							g_save_ptype = 1;
							g_save_len = (((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3) * 2;
							break;
						case PAT_SEW:
							g_save_ptype = 2;
							Mit_To_Bor((U32 *)&g_save_len);
							break; //����Ϊ�ֵܸ�ʽ

						case PAT_EMB: //����Ϊ�ػ���ʽ
							if (F_FILETYPE != 2)
							{ //�������ת��Ϊ�ֵܸ�ʽ
								g_save_ptype = 2;
								Mit_To_Bor((U32 *)&g_save_len);
							}
							g_exwriteType = 1;
							break;

						default:
							break;
						}
					}

					Mit_DealSaveData(g_save_ptype, g_save_len);

					dat1 = (U16)(g_patnumber[0]) * 100 + (U16)(g_patnumber[1]) * 10 + g_patnumber[2];
					if ((dat1 < MINFILENO) || (dat1 > MAXFILENO))
					{ //���뵵�����벻��ȷ
						pno = MENU_RWHELP;
						hleppara = 56;
						poldno[1] = MENU_WRITE;
						pnoindex = 2;
						break;
					}
					dat1 -= MINFILENO;
					dat1 *= FileIndexSize;

					for (i = 0; i < D_Length[HEAD]; i++)
					{
						HREG[i] = 0;
					}

					F_IND_NO = (U16)(g_patnumber[0]) * 100 + (U16)(g_patnumber[1]) * 10 + g_patnumber[2];
					HREG[5] = (g_save_len >> 16) & 0xffff;
					HREG[4] = g_save_len & 0xffff;
					F_FILETYPE = g_save_ptype;

					for (i = 0; i < 32; i++)
					{
						if (g_patname[2 * i] == 0)
							break;
						HREG[32 + i] = (g_patname[2 * i + 1] << 8) | g_patname[2 * i];
					}

					if ((IREG2[dat1] == 0xffff) && (IREG2[dat1 + 1] == 0xffff))
					{ //�����豸�޴˱��
						g_data_flash = 1;
						pno = MENU_RWHELP;
						poldno[1] = MENU_WRITE;
						pnoindex = 2;

						if (g_disk_memory == 2)
							hleppara = 11;
						else if (g_disk_memory == 1)
						{
							hleppara = 81;
						}
						else
						{
							hleppara = 10;
							g_para0 = 0;
						}
					}
					else
					{ //�����豸���д˱��
						pno = MENU_RWHELP;
						hleppara = 12;
						g_para0 = 12;
						poldno[1] = MENU_WRITE;
						pnoindex = 2;
					}
					break;
				case KEY_WRITE_NAME:
					refresh1 = 1;
					if (g_namenumberflag == 1)
					{
						g_namenumberflag = 0;
					}
					break;

				case KEY_WRITE_NUM:
					refresh1 = 1;
					if (g_namenumberflag == 0)
					{
						g_namenumberflag = 1;
					}
					break;
				case KEY_WRITE_FLASH:
					f = ReadFileIndex(C_RDMEMPATLIST, MENU_WRITE);
					if (f == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						poldno[1] = MENU_WRITE;
						pnoindex = 2;
						break;
					}
					else
					{
						refresh1 = 1;
						// refresh2 = 1;
						refresh3 = 1;
						g_namenumberflag = 0;
						g_disk_memory = 0;
						if (pinyinflag > 2)
						{
							pinyinflag = 2;
							refresh6 = 1;
							for (i = 0; i < 6; i++)
							{
								pingyingbuf[i] = 0x20;
							}
						}
						// ʣ������
						DrawButton(keyd[KEY_WRITE_CAPACITY], (U16 *)(ADD_ICONBMP),
								   keybmpind[KEY_WRITE_CAPACITY].border[0], keybmpind[KEY_WRITE_CAPACITY].icon[0]);
						if (SaveType > 2)
						{
							SaveType = F_FILETYPE;
							// dis_updata1 = 1;
						}
					}
					break;

				case KEY_WRITE_USB:
					//ʹ�ô������U��
					wdt();
					if (ExtMemory_Status(USBTF) != 0)
					{
						pno = MENU_RWHELP;
						hleppara = 95;
						poldno[1] = MENU_WRITE;
						pnoindex = 2;
						break;
					}
					Select_ExtMemory(USBTF);
					if (Explore_Disk("0:/", 1))
					{
						pno = MENU_RWHELP;
						hleppara = 40;
						poldno[1] = MENU_WRITE;
						pnoindex = 2;
						break;
					}

					refresh1 = 1;
					// refresh2 = 1;
					refresh3 = 1;
					g_namenumberflag = 0;
					g_disk_memory = 1;
					if (pinyinflag > 2)
					{
						pinyinflag = 2;
						refresh6 = 1;
						for (i = 0; i < 6; i++)
						{
							pingyingbuf[i] = 0x20;
						}
					}
					//FillRectColorChang(keyd[KEY_WRITE_CAPACITY].xpis,keyd[KEY_WRITE_CAPACITY].ypis,keyd[KEY_WRITE_CAPACITY].xsize,keyd[KEY_WRITE_CAPACITY].ysize,Color_White1,Color_Gray);
					BmpFillRect(keyd[KEY_WRITE_CAPACITY].xpis, keyd[KEY_WRITE_CAPACITY].ypis,
								keyd[KEY_WRITE_CAPACITY].xsize, keyd[KEY_WRITE_CAPACITY].ysize);
					if (SaveType > 3)
					{
						SaveType = F_FILETYPE;
						// dis_updata1 = 1;
					}
					break;

				case KEY_WRITE_SD:
					//if(ExtMemory_Status(SD)!=0){
					//	pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_WRITE;pnoindex = 2;break;
					//}
					//Select_ExtMemory(SD);
					//if(Explore_Disk ("1:/", 1)){
					//	pno = MENU_RWHELP;hleppara = 40;poldno[1] = MENU_WRITE;pnoindex = 2;break;
					//}
					//refresh1 = 1;
					//refresh2 = 1;
					//refresh3 = 1;
					//g_namenumberflag = 0;
					//
					//g_disk_memory = 2;
					//if(pinyinflag>2){
					//	pinyinflag = 2;
					//	refresh6 = 1;
					//	for(i=0;i<6;i++){
					//		pingyingbuf[i] = 0x20;
					//	}
					//}
					////FillRectColorChang(keyd[KEY_WRITE_CAPACITY].xpis,keyd[KEY_WRITE_CAPACITY].ypis,keyd[KEY_WRITE_CAPACITY].xsize,keyd[KEY_WRITE_CAPACITY].ysize,Color_White1,Color_Gray);
					//BmpFillRect(keyd[KEY_WRITE_CAPACITY].xpis,keyd[KEY_WRITE_CAPACITY].ypis,keyd[KEY_WRITE_CAPACITY].xsize,keyd[KEY_WRITE_CAPACITY].ysize);
					//if(SaveType>3){
					//	SaveType = F_FILETYPE;
					//	dis_updata1 =1;
					//}
					break;
				case KEY_WRITE_CAPACITY: //��ȡϵͳ�ڴ�
					hleppara = 9;
					pno = MENU_RWHELP;
					poldno[pnoindex] = MENU_WRITE;
					pnoindex++;
					Write_Cmd(C_READFSIZE, 0, 0);
					break;
				case KEY_WRITE_DEL:
					if (g_namenumberflag)
					{
						if (g_patnumberindex)
						{
							g_patnumberindex--;
							g_patnumber[g_patnumberindex] = 0x20;
							refresh5 = 1;
						}
					}
					else
					{
						if (pinyinflag < 3)
						{
							if (g_patnameindex)
							{
								if ((g_patname[g_patnameindex - 2] & 0x80) && (g_patname[g_patnameindex - 1] & 0x80))
								{ //����
									g_patnameindex--;
									g_patname[g_patnameindex] = 0;
								}
								g_patnameindex--;
								g_patname[g_patnameindex] = 0;
								refresh4 = 1;
							}
						}
						else
						{
							pinyinflag--;
							pingyingbuf[pinyinflag - 2] = 0x20;
							refresh6 = 1;
							tmphzadd = FinePYFirstAdd(pingyingbuf, 0x0);
							ReadHzData(tmppysel, 10, tmphzadd);
						}
					}
					break;

				case KEY_WRITE_ABCPY:
					if (pinyinflag > 2)
					{
						refresh6 = 1;
						while (pinyinflag > 2)
						{
							pinyinflag--;
							pingyingbuf[pinyinflag - 2] = 0x20;
						}
					}
					// refresh2 = 1;
					pinyinflag++;
					if (pinyinflag > 2)
						pinyinflag = 0;

					DrawButton(keyd[KEY_WRITE_ABCPY], (U16 *)(ADD_ICONBMP),
							   keybmpind[KEY_WRITE_ABCPY].border[0], keybmpind[KEY_WRITE_ABCPY].icon[pinyinflag]);
					break;
				}
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{
			if (keyon == KEY_WRITE_SCRNSVR) //�ް�����Ч
			{
			}
			else if (keyon >= KEY_WRITE_SEL1 && keyon <= KEY_WRITE_SELNEXT)
			{
				DrawFillRect(keyd[keyon].xpis, keyd[keyon].ypis,
							 keyd[keyon].xsize, keyd[keyon].ysize, keybmpind[keyon].icon[0]);
				if (keyon == KEY_WRITE_SELPRES)
				{
					Draw24X24FStr(keyd[keyon].xpis + SAFE_DIST3, keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
								  (char *)(pingyingbuf), keybmpind[keyon].textColor[0]);
				}
				else if (keyon == KEY_WRITE_SELPREV)
				{
					Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
								  keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
								  "<", keybmpind[keyon].textColor[0]);
				}
				else if (keyon == KEY_WRITE_SELNEXT)
				{

					Draw24X24FStr(keyd[keyon].xpis + (keyd[keyon].xsize - 12) / 2,
								  keyd[keyon].ypis + (keyd[keyon].ysize - 12) / 2,
								  ">", keybmpind[keyon].textColor[0]);
				}
				else if ((keyon >= KEY_WRITE_SEL1) && (keyon <= KEY_WRITE_SEL10))
				{
					Draw24X24FPY(keyd[keyon].xpis + (keyd[keyon].xsize - 24) / 2,
								 keyd[keyon].ypis + SAFE_DIST3,
								 (char *)(&tmppysel[keyon - KEY_WRITE_SEL1]), 2, keybmpind[keyon].textColor[1]);
					DrawFU32(keyd[keyon].xpis + (keyd[keyon].xsize - 8) / 2,
							 keyd[keyon].ypis + keyd[keyon].ysize - 16 + SAFE_DIST2,
							 (keyon - KEY_WRITE_SEL1), 1, keybmpind[keyon].textColor[0]);
				}
			}
			else if (keyon == KEY_WRITE_NUM || keyon == KEY_WRITE_NAME) //�ǳ��水��
			{
				DrawButtonOnZoomInRes(keyd[keyon]);
			}
			else
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			if ((keyon >= KEY_WRITE_0 && keyon <= KEY_WRITE_9) && pinyinflag > 2)
			{
				DrawFillRect(keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xpis, keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ypis,
							 keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xsize, keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ysize, keybmpind[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].icon[0]);

				Draw24X24FPY(keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xpis + (keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xsize - 24) / 2,
							 keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ypis + SAFE_DIST2,
							 (char *)(&tmppysel[keyon - KEY_WRITE_0]), 2, keybmpind[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].textColor[1]);
				DrawFU32(keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xpis + (keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].xsize - 24) / 2,
						 keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ypis + keyd[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].ysize - 12 + SAFE_DIST2,
						 (keyon - KEY_WRITE_0), 1, keybmpind[keyon + KEY_WRITE_SEL1 - KEY_WRITE_0].textColor[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ��д��ģʽ
			Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if ((Timerms > 1000))
		{
			Timerms = 0;
			if (g_disk_memory == 1)
			{ //ʹ��U��
				//if(g_MboardUSB==1){		//ʹ������U��
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
				//		if((ReadMemorryList(MENU_WRITE)) == 0){//��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
				//			return;
				//		}
				//		pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_WRITE;pnoindex = 2;
				//	}else{
				//		USBFlag =1;
				//	}
				//}else{	//ʹ�ô����U��
				if (ExtMemory_Status(USBTF) != 0)
				{
					USBFlag = 0;
					hleppara = 95;
					pno = MENU_RWHELP;
					poldno[pnoindex] = MENU_WRITE;
					pnoindex++;
					if ((ReadMemorryList(MENU_READ)) == 0)
					{ // ��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
						return;
					}
				}
				else
				{
					USBFlag = 1;
				}
				//}
				//��ѯһ��SD���������
				//if(ExtMemory_Status(SD)==0){
				//	SDFlag =1;
				//}else{
				//	SDFlag =0;
				//}
			}
			else if (g_disk_memory == 2)
			{	// ʹ�ô����SD
				//if(ExtMemory_Status(SD)!=0){
				//	SDFlag = 0;
				//	pno = MENU_RWHELP;hleppara = 95;poldno[1] = MENU_WRITE;pnoindex = 2;
				//	if((ReadMemorryList(MENU_WRITE)) == 0){		// ��ѡ�ж�����,���˳�����ʱҪ�����ڴ�������
				//		return;
				//	}
				//}else{
				//	SDFlag = 1;
				//}
				//
				////��ѯһ��U�̲������
				//Write_Cmd(C_DISK_STATUS,0,0);	//��ȡ����U�̲���״̬
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
				//
				//if(USBFlag || (ExtMemory_Status(USBTF)==0)) {
				//	USBFlag =1;
				//}else{
				//	USBFlag =0;
				//}
			}
			else
			{ //ʹ�������ڴ�
				//��ѯһ��U�̲������
				//Write_Cmd(C_DISK_STATUS,0,0);	//��ȡ����U�̲���״̬
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

				if (/*USBFlag || */ (ExtMemory_Status(USBTF) == 0))
				{
					USBFlag = 1;
				}
				else
				{
					USBFlag = 0;
				}

				//��ѯһ��SD���������
				//if(ExtMemory_Status(SD)==0){
				//	SDFlag =1;
				//}else{
				//	SDFlag =0;
				//}
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
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�ڴ水������*begin]**********************************************************
		if (refresh3)
		{
			U8 chSelectMem = 0;
			U32 chSaveAdd = 0;
			refresh3 = 0;
			chSelectMem = KEY_WRITE_FLASH + g_disk_memory;

			for (i = KEY_WRITE_FLASH; i <= KEY_WRITE_SD; i++)
			{
				if (i == KEY_WRITE_USB)
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
							   i == chSelectMem ? keybmpind[i].border[1] : (USBFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
							   i == chSelectMem ? keybmpind[i].icon[1] : (USBFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
				else if (i == KEY_WRITE_SD)
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
							   i == chSelectMem ? keybmpind[i].border[1] : (SDFlag == 1 ? keybmpind[i].border[0] : keybmpind[i].border[2]),
							   i == chSelectMem ? keybmpind[i].icon[1] : (SDFlag == 1 ? keybmpind[i].icon[0] : keybmpind[i].icon[2]));
				else if (i == KEY_WRITE_FLASH)
					DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
							   i == chSelectMem ? keybmpind[i].border[1] : keybmpind[i].border[0],
							   i == chSelectMem ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
			}

			g_patnumberindex = 3;

			if (g_save_ptype == PAT_MIT) //��100��600��ʼѰ�Ҵ洢�ռ��ַ
			{							 //�����
				chSaveAdd = (SAVEADD_MIT - MINFILENO) * FileIndexSize;
			}
			else if (g_save_ptype == PAT_SEW)//�ֵܿ�
			{ 
				chSaveAdd = (SAVEADD_SEW - MINFILENO) * FileIndexSize;
			}

			for (; chSaveAdd < MAXIREG2; chSaveAdd += FileIndexSize) //��ȡδ��ռ�õĻ�����ַ
			{
				if ((IREG2[chSaveAdd] == 0xffff) && (IREG2[chSaveAdd + 1] == 0xffff))
					break;
			}
			wdt();

			if (chSaveAdd >= MAXIREG2) //�޿ռ䣬��1��ʼѰ�Ҵ洢�ռ��ַ
			{
				U32 endaddr = 0;
				if (g_save_ptype == PAT_MIT)//�����
				{ 
					chSaveAdd = (2 - MINFILENO) * FileIndexSize;
					endaddr = (SAVEADD_MIT - MINFILENO) * FileIndexSize;
				}
				else if (g_save_ptype == PAT_SEW)//�ֵܿ�
				{ 
					chSaveAdd = (2 - MINFILENO) * FileIndexSize;
					endaddr = (SAVEADD_SEW - MINFILENO) * FileIndexSize;
				}
				for (; chSaveAdd < endaddr; chSaveAdd += FileIndexSize)
				{
					if ((IREG2[chSaveAdd] == 0xffff) && (IREG2[chSaveAdd + 1] == 0xffff))
						break;
				}

				if (chSaveAdd >= endaddr) //���Ҳ�Ҳ���������Ϊ����ַ
					chSaveAdd = MAXIREG2;
			}
			if (chSaveAdd >= MAXIREG2) //����Ϊ���1
				chSaveAdd = FileIndexSize;
			chSaveAdd = chSaveAdd / FileIndexSize + MINFILENO;

			//����ASCLL��ֵ
			g_patnumber[0] = (chSaveAdd / 100);
			g_patnumber[1] = (chSaveAdd / 10) % 10;
			g_patnumber[2] = chSaveAdd % 10;

			if (g_patnumber[0] == 0)
			{
				if (g_patnumber[1] == 0)
				{
					if (g_patnumber[2] == 0)
					{
						g_patnumber[0] = 0x20;
						g_patnumberindex = 0;
					}
					else
					{
						g_patnumber[0] = g_patnumber[2] + 0x30;
						g_patnumberindex = 1;
					}
					g_patnumber[1] = 0x20;
				}
				else if (g_patnumber[2] == 0)
				{
					g_patnumber[0] = g_patnumber[1] + 0x30;
					g_patnumber[1] = 0 + 0x30;
					g_patnumberindex = 2;
				}
				else
				{
					g_patnumber[0] = g_patnumber[1] + 0x30;
					g_patnumber[1] = g_patnumber[2] + 0x30;
					g_patnumberindex = 2;
				}
				g_patnumber[2] = 0x20;
			}
			else
			{
				g_patnumber[0] += 0x30;
				g_patnumber[1] += 0x30;
				g_patnumber[2] += 0x30;
				g_patnumberindex = 3;
			}

			refresh4 = 1;
			refresh5 = 1;
		}
		// /[�ڴ水������*end]**********************************************************

		// /[�������ְ�ťˢ��*begin]**********************************************************
		if (refresh1)
		{
			refresh1 = 0;
			refresh4 = 1;
			refresh5 = 1;
		}
		if (refresh4)
		{
			refresh4 = 0;

			if ((g_namenumberflag + KEY_WRITE_NAME) == KEY_WRITE_NAME)
			{
				DrawButton3(keyd[KEY_WRITE_NAME], keybmpind[KEY_WRITE_NAME].icon[1], 0);
				DrawDottedRect(keyd[KEY_WRITE_NAME].xpis, keyd[KEY_WRITE_NAME].ypis,
							   keyd[KEY_WRITE_NAME].xsize, keyd[KEY_WRITE_NAME].ysize,
							   keybmpind[KEY_WRITE_NAME].icon[2]);
			}
			else
			{
				DrawButton3(keyd[KEY_WRITE_NAME], keybmpind[KEY_WRITE_NAME].icon[0], 0);
			}
			DrawFFilename(keyd[KEY_WRITE_NAME].xpis + SAFE_DIST4, keyd[KEY_WRITE_NAME].ypis + SAFE_DIST4,
						  (char *)&g_patname, 64, keybmpind[KEY_WRITE_NAME].textColor[0], 20, 0, NULL);
			// DrawFMultiLStr24LimtL(keyd[KEY_WRITE_NAME].xpis + SAFE_DIST4, keyd[KEY_WRITE_NAME].ypis + (keyd[KEY_WRITE_NAME].ysize - 24) / 2,
			// 					  (U16 *)(&g_patname), 15, 1, keybmpind[KEY_WRITE_NAME].textColor[0]); //����1�У������ʾ28���ַ�
		}
		if (refresh5)
		{
			// U16 chNoTemp = (g_patnumber[0] - 0x30) * 100 + (g_patnumber[1] - 0x30) * 10 + (g_patnumber[2] - 0x30);
			refresh5 = 0;
			// DrawFillRect(keyd[KEY_WRITE_NUMBER].xpis, keyd[KEY_WRITE_NUMBER].ypis, keyd[KEY_WRITE_NUMBER].xsize, keyd[KEY_WRITE_NUMBER].ysize, (g_namenumberflag == 1) ? Color_KeyBlue : Color_White1);
			// DrawFASC3216(keyd[KEY_WRITE_NUMBER].xpis + 8, keyd[KEY_WRITE_NUMBER].ypis + 9, (char *)g_patnumber, (g_namenumberflag == 1) ? Color_White : Color_Black);
			if ((g_namenumberflag + KEY_WRITE_NAME) == KEY_WRITE_NUM)
			{
				DrawButton3(keyd[KEY_WRITE_NUM], keybmpind[KEY_WRITE_NUM].icon[1], 0);
				DrawDottedRect(keyd[KEY_WRITE_NUM].xpis, keyd[KEY_WRITE_NUM].ypis,
							   keyd[KEY_WRITE_NUM].xsize, keyd[KEY_WRITE_NUM].ysize,
							   keybmpind[KEY_WRITE_NUM].icon[2]);
			}
			else
			{
				DrawButton3(keyd[KEY_WRITE_NUM], keybmpind[KEY_WRITE_NUM].icon[0], 0);
			}
			DrawFASC12(keyd[KEY_WRITE_NUM].xpis + SAFE_DIST4, keyd[KEY_WRITE_NUM].ypis + SAFE_DIST4,
					   (char *)g_patnumber, keybmpind[KEY_WRITE_NUM].textColor[0]);
			// DrawFFileno_24x12(keyd[KEY_WRITE_NUM].xpis + SAFE_DIST4, keyd[KEY_WRITE_NUM].ypis + (keyd[KEY_WRITE_NUM].ysize - 24) / 2,
			// 				  chNoTemp, keybmpind[KEY_WRITE_NUM].textColor[0]);
		}
		// /[�������ְ�ťˢ��*end]**********************************************************

		// /[����ѡ���ʶ*begin]**********************************************************
		if (refresh6)
		{
			refresh6 = 0;

			if (pinyinflag > 2)
			{

				BmpFillRect(keyd[KEY_WRITE_SELPRES].xpis, keyd[KEY_WRITE_SELPRES].ypis,
							keyd[KEY_WRITE_SELNEXT].xpis + keyd[KEY_WRITE_SELNEXT].xsize - keyd[KEY_WRITE_SELPRES].xpis, keyd[KEY_WRITE_SELPRES].ysize);
				for (i = KEY_WRITE_SEL1; i <= KEY_WRITE_SELNEXT; i++)
				{
					DrawFillRect(keyd[i].xpis, keyd[i].ypis,
								 keyd[i].xsize, keyd[i].ysize, keybmpind[i].icon[0]);
					if (i == KEY_WRITE_SELPRES)
					{
						Draw24X24FStr(keyd[i].xpis + SAFE_DIST3, keyd[i].ypis + (keyd[i].ysize - 12) / 2,
									  (char *)(pingyingbuf), keybmpind[i].textColor[0]);
					}
					else if (i == KEY_WRITE_SELPREV)
					{
						Draw24X24FStr(keyd[i].xpis + (keyd[i].xsize - 12) / 2,
									  keyd[i].ypis + (keyd[i].ysize - 12) / 2,
									  "<", keybmpind[i].textColor[0]);
					}
					else if (i == KEY_WRITE_SELNEXT)
					{

						Draw24X24FStr(keyd[i].xpis + (keyd[i].xsize - 12) / 2,
									  keyd[i].ypis + (keyd[i].ysize - 12) / 2,
									  ">", keybmpind[i].textColor[0]);
					}
					else if ((i >= KEY_WRITE_SEL1) && (i <= KEY_WRITE_SEL10))
					{
						Draw24X24FPY(keyd[i].xpis + (keyd[i].xsize - 24) / 2,
									 keyd[i].ypis + SAFE_DIST3,
									 (char *)(&tmppysel[i - KEY_WRITE_SEL1]), 2, keybmpind[i].textColor[1]);
						DrawFU32(keyd[i].xpis + (keyd[i].xsize - 8) / 2,
								 keyd[i].ypis + keyd[i].ysize - 16 + SAFE_DIST2,
								 (i - KEY_WRITE_SEL1), 1, keybmpind[i].textColor[0]);
					}
				}
			}
			else
			{
				BmpFillRect(keyd[KEY_WRITE_SELPRES].xpis, keyd[KEY_WRITE_SELPRES].ypis,
							keyd[KEY_WRITE_SELNEXT].xpis + keyd[KEY_WRITE_SELNEXT].xsize - keyd[KEY_WRITE_SELPRES].xpis, keyd[KEY_WRITE_SELPRES].ysize);
				refresh5 = 1;
				// dis_updata1 = 1;
			}
		}
		// /[����ѡ���ʶ*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(MENU_WRITE);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			ErrorMsg(MENU_WRITE);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[��ȡ���»����б�*begin]**********************************************************
		// if (g_updataIREG)
		// {
		// 	g_updataIREG = 0;
		// 	ReadPatList(0, pno); //�����ڴ�����
		// }
		// /[��ȡ���»����б�*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		//���ݻ�������
		if ((g_backSREG_f == 1) || (g_backHREG_f == 1))
		{
			BackupSHREG((g_backHREG_f << 1) | g_backSREG_f);
		}
		if (g_backIREG_f)
		{
			BackupIREG();
		}

		if (pno != MENU_WRITE)
		{
			plastno = MENU_WRITE;
			break;
		}

		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

#define KEY_WRITE1_SCRNSVR 1 //1 ����
#define KEY_WRITE1_BACK 2	 //2 ����
#define KEY_WRITE1_OK 3		 //3 ȷ��
#define KEY_WRITE1_OVER 4	 //4 ��д
#define KEY_WRITE1_NEW 5	 //5 �½�
#define KEY_WRITE1_EXIT 6	 //6 �������˳�

#define KEY_WRITE1_MAX 7

#define RECT_WRITE1_NAME_TLT 1 //1 ����-����
#define RECT_WRITE1_NUM_TLT 2  //2 ����-����
#define RECT_WRITE1_NAME_STR 3 //3 ����-����
#define RECT_WRITE1_NUM_STR 4  //4 ����-����

#define RECT_WRITE1_MAX 5

void Menu_Write1(void)
{

	U16 i, j/*, dat*/;
	// U8 ch;
	// U32 n, ii;
	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****��־λ��ز���begin****/
	U8 tmpName[64] = {0};
	U8 oldWriteMode = 0, WriteMode = 1; //д��ģʽ 0����д   1���½�   2���������˳�
	// U16 tmpBorcmd;
	// S8 tmpBorpooft, Borpfdis;
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_WRITE1_MAX};
	const KEYPAD keyd[] = {
		{0, 0, 0, 0},
		{0, 0, 128, 32}, //1 ����

		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //2 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 ȷ��
		{275, 144, TYPE_SIZE19, KEY_Y3},	 //4 ��д
		{275, 238, TYPE_SIZE19, KEY_Y3},	 //5 �½�
		{275, 332, TYPE_SIZE19, KEY_Y3},	 //6 �������˳�
	};

	const tydDisSrcIndex keybmpind[] = {
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},  //1 ����
		{{KEY_Silver}, {0}, {72}, {2, 0, 0, 0}, {0}}, //2 ����
		{{KEY_Silver}, {0}, {71}, {3, 0, 0, 0}, {0}}, //3 ȷ��

		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {6, 0, 0, 0}, {9}},	//4 ��д
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {7, 0, 0, 0}, {10}}, //5 �½�
		{{KEY_Silver, KEY_Blue}, {Color_Black, Color_White}, {KEY_Silver, KEY_Blue, 0, 0}, {8, 0, 0, 0}, {11}}, //6 �������˳�

	};

	const RECTPAD rect[RECT_WRITE1_MAX] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
		{TYPE_X16, TYPE_Y2, TYPE_SIZE11, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {4}},	//1 ����-����
		{TYPE_X16, TYPE_Y16, TYPE_SIZE11, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {5}}, //2 ���-����
		{TYPE_X22, TYPE_Y2, TYPE_SIZE23, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},	//3 ����-����
		{TYPE_X22, TYPE_Y16, TYPE_SIZE23, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},	//4 ���-����
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_WRITE1); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ40.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
	g_savePatInfo.No = F_IND_NO;
	g_savePatInfo.len = F_FILELEN;
	g_savePatInfo.Type = F_FILETYPE;
	g_savePatInfo.Type = PAT_MIT;

	for (i = 0; i < 32; i++)
	{
		if ((i != 0) && (i != 4) && (i != 5) && (i != 8))
		{
			HREG[i] = 0;
		}
	}

	for (i = 0, j = 0; i < 32; i++)
	{
		if ((HREG[32 + i] & 0xff) == 0)
			break;
		if ((((HREG[32 + i] & 0xff) >= 0x30) && ((HREG[32 + i] & 0xff) <= 0x39)) || (((HREG[32 + i] & 0xff) >= 0x41) && ((HREG[32 + i] & 0xff) <= 0x5a)) ||
			(((HREG[32 + i] & 0xff) >= 0x61) && ((HREG[32 + i] & 0xff) <= 0x7a)) || ((HREG[32 + i] & 0xff) > 0x7f) ||
			((HREG[32 + i] & 0xff) == 0x5f))
		{
			tmpName[j++] = (HREG[32 + i]) & 0xff;
		}

		if (((HREG[32 + i] >> 8) & 0xff) == 0)
		{
			break;
		}

		if (((((HREG[32 + i] >> 8) & 0xff) >= 0x30) && (((HREG[32 + i] >> 8) & 0xff) <= 0x39)) || ((((HREG[32 + i] >> 8) & 0xff) >= 0x41) && (((HREG[32 + i] >> 8) & 0xff) <= 0x5a)) ||
			((((HREG[32 + i] >> 8) & 0xff) >= 0x61) && (((HREG[32 + i] >> 8) & 0xff) <= 0x7a)) || (((HREG[32 + i] >> 8) & 0xff) > 0x7f) ||
			(((HREG[32 + i] >> 8) & 0xff) == 0x5f))
		{
			tmpName[j++] = (HREG[32 + i] >> 8) & 0xff;
		}
	}
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
				 SheetPage, rect[0].index[0], rect[0].colorT[0]); //д��ģʽ
	//��������
	for (i = 1; i < RECT_WRITE1_MAX; i++)
	{
		DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
		if (i == RECT_WRITE1_NAME_TLT || i == RECT_WRITE1_NUM_TLT)
		{
			// Draw24x24Str(rect[i].xpis + (rect[i].xsize - strlen((char *)(MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax)) * 13) / 2,
			// 			 rect[i].ypis + (rect[i].ysize - 24) / 2,
			// 			 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
			// 			 SheetPage, rect[i].index[0], rect[i].colorT[0]);
			Draw24x24Str(rect[i].xpis + SAFE_DIST4,
						 rect[i].ypis + (rect[i].ysize - 24) / 2,
						 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
						 SheetPage, rect[i].index[0], rect[i].colorT[0]);
		}
	}
	DrawFFilename(rect[RECT_WRITE1_NAME_STR].xpis + SAFE_DIST4,
				  rect[RECT_WRITE1_NAME_STR].ypis + SAFE_DIST4,
				  (char *)&tmpName, 64, keybmpind[RECT_WRITE1_NAME_STR].textColor[0], 20, 0, NULL);
	DrawFFileno_24x12(rect[RECT_WRITE1_NUM_STR].xpis + SAFE_DIST4,
					  rect[RECT_WRITE1_NUM_STR].ypis + (rect[RECT_WRITE1_NUM_STR].ysize - 24) / 2,
					  g_savePatInfo.No, keybmpind[RECT_WRITE1_NUM_STR].textColor[0]);
	//����������ʾ
	for (i = 2; i < KEY_WRITE1_MAX; i++)
	{
		//��Ч����
		if (i >= KEY_WRITE1_OVER && i <= KEY_WRITE1_EXIT) //������ɫ���ְ���
		{
			U16 chKeyTemp = i - KEY_WRITE1_OVER;

			DrawButtonSingleColor(keyd[i], (chKeyTemp != WriteMode) ? keybmpind[i].border[0] : keybmpind[i].border[1],
								  (chKeyTemp != WriteMode) ? keybmpind[i].icon[0] : keybmpind[i].icon[1]);
			Draw24x24Str(keyd[i].xpis + (keyd[i].xsize - strlen((char *)(MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax)) * 13) / 2,
						 keyd[i].ypis + (keyd[i].ysize - 24) / 2,
						 MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax,
						 SheetPage, keybmpind[i].nameStr[0],
						 (chKeyTemp != WriteMode) ? keybmpind[i].textColor[0] : keybmpind[i].textColor[1]);
		}
		else //���水ť
		{
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
		}
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
	if (g_endtype == 1) //������
	{
		pnoindex = 3;
		poldno[pnoindex - 1] = MENU_CURSOR;
	}
	else if (g_endtype == 2) //�޸����
	{
		pnoindex = 2;
		poldno[pnoindex - 1] = MENU_MODIFY;
	}
	else if (g_endtype == 3) //�任���
	{
		pnoindex = 2;
		poldno[pnoindex - 1] = MENU_CONVERT;
	}
	ViewPage(usepageno); //������ʾ����

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************
	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			//���������������ܵİ���
			if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > 0) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//�ض�����²���Ч����

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
			{
				keyon = keydata;
				if (keyon == KEY_WRITE1_SCRNSVR) //�ް�����Ч
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
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}

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
				Times = 0;
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
			case KEY_WRITE1_SCRNSVR: //����
				pno = MENU_SCRNSVR;
				break;

			case KEY_WRITE1_BACK: //����
#if 0
				if ((poldno[pnoindex - 1] == MENU_MODIFY) || (poldno[pnoindex - 1] == MENU_CONVERT))
				{
					//	if(F_FILETYPE==1){
					//		AnalyseDataCorrect();								//��黨�����ݵ���ȷ�Բ�У��
					//		Createdata();

					//		if(1==ConvertMultiDraw()){
					//			Createdata();
					//			}
					//	}
				}
				else if (poldno[pnoindex - 1] == MENU_CURSOR) //�ָ���ͼ����
				{
					if (F_FILETYPE == PAT_MIT)
					{
						ii = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
						g_temp_line_p = (SREG[2] + 0x40) / 2;
						g_temp_pat_p = g_temp_line_p + ii;
						if ((g_temp_line_p <= 32000) && (g_temp_pat_p <= 52000) && (ii < 20000))
						{
							if ((S_CLAMP) || (P_OST == 1))
							{
								g_init_flag = 3;
								g_draw_p = g_temp_line_p;
								g_line_p = 32000 + ii;
								g_pat_p = 52000 + ii * 2;

								n = g_temp_pat_p + ii * 2;
								g_dxpos = 0;
								g_dypos = 0;
								g_oxpos = 0;
								g_oypos = 0;
#if PATRULES_ORG_OFFSET
								if (SREG[3] == 7) //����-�޸�ԭ��λ��
								{
									g_oxpos = (S16)(SREG[6]);
									g_oypos = (S16)(SREG[7]);
								}
#endif
								g_feed_sew = 0xff;
								for (; ii > 0; ii--)
								{
									SREG[--g_pat_p] = SREG[--n];
									dat = SREG[n];
									SREG[--g_pat_p] = SREG[--n];
									if (((SREG[n] & 0x00ff) == 0x61) || ((SREG[n] & 0x00ff) == 0x41) || ((SREG[n] & 0x00ff) == 0x21) || ((SREG[n] & 0x00ff) == 0x1) || ((SREG[n] & 0x00ff) == 0x1b) || ((SREG[n] & 0x00ff) == 0x3))
									{
										if (g_feed_sew == 0xff)
										{
											if ((SREG[n] == 0x3) || (SREG[n] == 0x1b))
												g_feed_sew = 0;
											else
												g_feed_sew = 1;
										}
										ch = (U8)(dat);
										if (ch >= 0x80)
										{
											ch = ch - 0x80;
											g_oxpos = g_oxpos - (S16)(ch);
										}
										else
										{
											g_oxpos = g_oxpos + (S16)(ch);
										}
										ch = (U8)((U16)(dat) >> 8);
										if (ch >= 0x80)
										{
											ch = ch - 0x80;
											g_oypos = g_oypos - (S16)(ch);
										}
										else
										{
											g_oypos = g_oypos + (S16)(ch);
										}
									}
									else if ((g_feed_sew == 0xff) && (SREG[n] == 0x2))
									{
										g_feed_sew = 0;
									}
								}
								if (g_feed_sew == 0xff)
									g_feed_sew = 0;
								Comm_Move(g_oxpos, g_oypos, 120, MENU_INPUT, 0, 0);
								ii = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
								for (; ii > 0; ii--)
								{
									SREG[--g_line_p] = SREG[--n];
								}
								for (ii = g_draw_p; ii < 32000; ii++)
								{
									SREG[ii] = 0;
								}

								g_line_p = 32000 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
								g_pat_p = 52000 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 2);
								SREG[2] = 64000 - 0x40;
								SREG[4] = 0x3880;
								SREG[5] = 1;
								///////////////////////////////
								g_temp_line_p = SREG[g_draw_p - 4];
								g_temp_pat_p = SREG[g_pat_p - 2];
								if (((g_temp_line_p == 0x1f01) && (g_temp_pat_p == 0x1f)) || ((g_temp_line_p == 0x1e01) && (g_temp_pat_p == 0x1e)))
								{
									g_draw_p -= 4;
									g_line_p -= 1;
									g_pat_p -= 2;
									SREG[g_draw_p] = 0;

									///////////////////////////////////////////////ADD ON 1.16

									g_temp_line_p = SREG[g_draw_p - 4];
									g_temp_pat_p = SREG[g_pat_p - 2];
									if ((g_temp_line_p == 0x0201) && (g_temp_pat_p == 0x02))
									{
										g_draw_p -= 4;
										g_line_p -= 1;
										g_pat_p -= 2;
										SREG[g_draw_p] = 0;
										g_DoTrim = 1; //ɾ�����߱�־
									}
								}
								g_line_sum = (U16)((SREG[g_line_p - 1]) & 0x3fff) + 1;
								g_feedc = g_line_p - (SREG[2] + 0x40) / 2;
								g_feedsum = g_feedc;
								for (ii = 0x24; ii < g_draw_p; ii += 4)
								{
									if (SREG[ii] == 0x601)
									{
										g_home_2flag = 1;
										g_home_2xpos = 0;
										g_home_2ypos = 0;
										for (; ii > 0x24; ii -= 4)
										{
											g_home_2xpos += (S16)(SREG[ii - 2]);
											g_home_2ypos += (S16)(SREG[ii - 1]);
										}
										break;
									}
								}
							}
							else
							{
								pno = MENU_RWHELP;
								hleppara = 13;
								break;
							}
						}
						else
						{
						}
					}
					else if (F_FILETYPE == PAT_SEW)
					{

						Bor_feedInfo(g_patd.feed_sum, g_CtPatLen, &tmpBorcmd, (S16 *)&g_oxpos, (S16 *)&g_oypos, (S16 *)&g_dxpos, (S16 *)&g_dxpos, (U8 *)&g_oldspeed, &tmpBorpooft, &Borpfdis); //���ҵ�ǰԭ��������������
						Comm_Move(g_oxpos + g_dxpos, g_oypos + g_dxpos, 120, MENU_CURSOR, 1, 1);																							   //�ƶ�����������������
						g_oxpos = g_oxpos + g_dxpos;
						g_oypos = g_oypos + g_dxpos;
						g_disxpos = g_oxpos;
						g_disypos = g_oypos;
						g_dxpos = 0;
						g_dxpos = 0;

						g_pf_absval = tmpBorpooft;
						g_pf_absval += g_pfootd.base;
						Bor_CtBefDatAny((U32 *)&g_Ctstaddr, (U32 *)&g_CtPatLen, (U8 *)&g_oldspeed, 1);
						g_feedsum = g_patd.feed_sum;
						g_feedc = g_patd.feed_sum;
						g_Ctendaddr = g_Ctstaddr;
						g_Ctsavetimes = 0;
						//	printf("g_Ctstaddr = %d  g_CtPatLen = %d \r\n",g_Ctstaddr,g_CtPatLen);
						for (ii = 0; ii < MAXBLKBUF; ii++)
						{
							BLKBUF[ii] = 0;
						}
						Bor_CreatBack((U16 *)&g_Ctsavetimes, g_Ctstaddr, g_Ctendaddr, g_oxpos, g_oypos, g_oldspeed, g_pf_absval - g_pfootd.base, g_pf_dis, g_lineforceValSetNow, g_patd.orgX, g_patd.orgY);
					}
					g_array4[0][0] = 0;
					g_point_num = 1;
					g_feed_flag = 0;
					g_ct_status_flag = 0;
					g_ct_status_flag |= (bit1 | bit4 | bit7 | bit9 | bit10);
				}
				else
				{
					if (g_recv_netdata)
					{
						g_recv_netdata = 0;
						Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
						RestoreSHREG(0x03);
					}
				}
				pnoindex--;
				pno = poldno[pnoindex];
#else
				if (fun_backPattingData(MENU_WRITE1) != 0)
				{
					pnoindex--;
					pno = poldno[pnoindex];
				}

#endif

				break;

			case KEY_WRITE1_OK: //ȷ��
				g_savemodify = 0;
				g_para0 = 0;
				if (WriteMode == 0) //��д
				{
					for (i = 0; i < 64; i++)
					{
						if (tmpName[i] == 0)
							break;
						g_patname[i] = tmpName[i];
					}

					Mit_DealSaveData(g_savePatInfo.Type, g_savePatInfo.len);

					for (i = 0; i < 32; i++)
					{
						if (g_patname[2 * i] == 0)
							break;
						HREG[32 + i] = (g_patname[2 * i + 1] << 8) | g_patname[2 * i];
					}
					pno = MENU_RWHELP;
					hleppara = 10;
				}
				else if (WriteMode == 1) //�����µ�
				{
					if (g_NoUpdataoldno == 1)//����Ҫ������һ�λ������
					{	
						//	gWfitePatInfo[0] =	oldselectdisno;
					}
					if (YREG[SYS_LISTTYPE] != 0) //��ǰ�ļ��������������ڴ� �����������ڴ�����
					{
						ReadPatList(0, MENU_WRITE1);
						Read_D(D_Address[SYS] + SYS_LISTTYPE, 1, (U16 *)(&YREG[SYS_LISTTYPE])); // �������ڴ���������
					}
					g_save_ptype = g_savePatInfo.Type;
					g_save_len = g_savePatInfo.len;
					pno = MENU_WRITE;
					poldno[pnoindex] = MENU_WRITE1;
					pnoindex++;
				}
				else if (WriteMode == 2) //���洢�˳�
				{
					for (i = 0; i < 64; i++)
					{
						if (tmpName[i] == 0)
							break;
						g_patname[i] = tmpName[i];
					}
					Mit_DealSaveData(g_savePatInfo.Type, g_savePatInfo.len);

					for (i = 0; i < 32; i++)
					{
						if (g_patname[2 * i] == 0)
							break;
						HREG[32 + i] = (g_patname[2 * i + 1] << 8) | g_patname[2 * i];
					}

					if (g_NoUpdataoldno != 1)
					{
						g_oldselectdisno = F_IND_NO;
					}
					else
					{
						g_savePatInfo.No = g_oldselectdisno;
					}

					g_NoUpdataoldno = 0;
					poldno[pnoindex++] = MENU_WRITE1; //����
					pno = MENU_RWHELP;
					hleppara = 10;
					g_para0 = 2;
				}

				DREG[D_COMMAND] = C_YABANUP;
				DREG[D_CMDSTATUS] = 0;
				Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
				Read_Status();
				g_NoReadPatf = 2; //�޸�
				break;

			case KEY_WRITE1_OVER:
				WriteMode = 0;
				break;

			case KEY_WRITE1_NEW:
				WriteMode = 1;
				break;

			case KEY_WRITE1_EXIT:
				WriteMode = 2;
				break;
			default:
				break;
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{
			if (keyon == KEY_WRITE1_SCRNSVR) //����Ҫ��ʾ�İ���
			{
			}
			//�ǳ��水��-//������ɫ���ְ���
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;

			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ʾ��д�����
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 1000)
		{
			Timerms = 0;
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
			ErrorMsg(poldno[0]);
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			ErrorMsg(poldno[0]);
			if (Timer_ms >= 30000) //30720
			{
				Timer_ms = 0;
			}
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[��ť����*begin]**********************************************************
		if (WriteMode != oldWriteMode)
		{

			U16 chKeyTemp = 0;
			for (i = KEY_WRITE1_OVER; i <= KEY_WRITE1_EXIT; i++)
			{
				chKeyTemp = i - KEY_WRITE1_OVER;

				DrawButtonSingleColor(keyd[i], (chKeyTemp != WriteMode) ? keybmpind[i].border[0] : keybmpind[i].border[1],
									  (chKeyTemp != WriteMode) ? keybmpind[i].icon[0] : keybmpind[i].icon[1]);
				Draw24x24Str(keyd[i].xpis + (keyd[i].xsize - strlen((char *)(MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax)) * 13) / 2,
							 keyd[i].ypis + (keyd[i].ysize - 24) / 2,
							 MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax,
							 SheetPage, keybmpind[i].nameStr[0],
							 (chKeyTemp != WriteMode) ? keybmpind[i].textColor[0] : keybmpind[i].textColor[1]);
			}
			oldWriteMode = WriteMode;
		}
		// /[��ť����*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_WRITE1)
		{
			plastno = MENU_WRITE1;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

/******************************************************/

#define KEY_SKIP_SCRNSVR 1 //1 ����
#define KEY_SKIP_BACK 2	   //2 ����
#define KEY_SKIP_ENTER 3   //3 ȷ��
#define KEY_SKIP_OFF 4	   //4 �ر�
#define KEY_SKIP_ON 5	   //5 ����
#define KEY_SKIP_PP 6	   //6 �㵽��
#define KEY_SKIP_PAT 7	   //7 ����
#define KEY_SKIP_NOUT 8	   //8 �����������
#define KEY_SKIP_OUT 9	   //9 ���������
#define KEY_SKIP_CLEAN 10  //10 ���һλ
#define KEY_SKIP_UP 11	   //11 +1
#define KEY_SKIP_DOWN 12   //12 -1
#define KEY_SKIP_0 13	   //13 0
#define KEY_SKIP_1 14	   //14 1
#define KEY_SKIP_2 15	   //15 2
#define KEY_SKIP_3 16	   //16 3
#define KEY_SKIP_4 17	   //17 4
#define KEY_SKIP_5 18	   //18 5
#define KEY_SKIP_6 19	   //19 6
#define KEY_SKIP_7 20	   //20 7
#define KEY_SKIP_8 21	   //21 8
#define KEY_SKIP_9 22	   //22 9

#define KEY_SKIP_MAX 23

#define RECT_SKIP_JOG 1	   //1 �ƶ�ʹ��
#define RECT_SKIP_METHOD 2 //2 ����
#define RECT_SKIP_OUT 3	   //3 �������

#define RECT_SKIP_PIN_TIT 4 //4 ��Ծ����-����
#define RECT_SKIP_PIN_STR 5 //5 ��Ծ����-����
#define RECT_SKIP_MAX 6

void Menu_SkipJog(void)
{
	/****������ز���begin****/
	U8 beeponf = 0;							//������ ������־λ
	U16 delay = 0;							//����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
	U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
	/****������ز���end****/

	/****��־λ��ز���begin****/
	U8 dis_updata1 = 1;
	U8 dis_updata2 = 1;
	U8 dis_updata3 = 1;
	U8 dis_updata5 = 1;
	U32 i;
	U8 f;
	U32 stitch;
	U8 temp_jogonflag, temp_jogmethod, temp_jogfunction;
	U8 excode;
	U32 wMaxFeedSum;
	U16 moddat = 0;
	/****��־λ��ز���end***/

	const U8 keynum = {KEY_SKIP_MAX};

	const KEYPAD keyd[] = {
		{0, 0, 0, 0},

		{0, 0, 128, 32},					 //1 ����
		{TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},	 //2 ����
		{TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 ȷ��

		{TYPE_X14 + SAFE_DIST1, 78, KEY_X1, KEY_Y1},			  //4 �ر�
		{TYPE_X14 + SAFE_DIST1 * 2 + KEY_X1, 78, KEY_X1, KEY_Y1}, //5 ����

		{TYPE_X14 + SAFE_DIST1, 188, KEY_X1, KEY_Y1},			   //6 �㵽��
		{TYPE_X14 + SAFE_DIST1 * 2 + KEY_X1, 188, KEY_X1, KEY_Y1}, //7 ����

		{TYPE_X14 + SAFE_DIST1, 298, KEY_X1, KEY_Y1},			   //8 �����������
		{TYPE_X14 + SAFE_DIST1 * 2 + KEY_X1, 298, KEY_X1, KEY_Y1}, //9 ���������

		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 4, KEY_X2, KEY_Y2},							  //10 ���1λ
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},		  //11 +1
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //12 -1
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},							  //13 0
		{TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},														  //14 1
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13, KEY_X2, KEY_Y2},									  //15 2
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13, KEY_X2, KEY_Y2},							  //16 3
		{TYPE_X15, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},									  //17 4
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},			  //18 5
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},		  //19 6
		{TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},							  //20 7
		{TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},		  //21 8
		{TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //22 9

	};

	const tydDisSrcIndex keybmpind[] = {
		//title_counter
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
		{{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},						  //1 ����
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {2, 0, 0, 0}, {0}}, //2 ����
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {3, 0, 0, 0}, {0}}, //3 ȷ��

		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {513, 0, 514}, {4, 0, 0, 0}, {0}}, //4 �ر�
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {511, 0, 512}, {5, 0, 0, 0}, {0}}, //5 ����
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {82, 0, 83}, {6, 0, 0, 0}, {0}},   //6 �㵽��
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {84, 0, 85}, {7, 0, 0, 0}, {0}},   //7 ����
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {5, 7, 6}, {8, 0, 0, 0}, {0}},	   //8 �����������
		{{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {8, 10, 9}, {9, 0, 0, 0}, {0}},	   //9 ���������

		{{KEY_Silver}, {0}, {113}, {10, 0, 0, 0}, {0}}, //10 ���1λ
		{{KEY_Silver}, {0}, {199}, {11, 0, 0, 0}, {0}}, //11 +1
		{{KEY_Silver}, {0}, {201}, {12, 0, 0, 0}, {0}}, //12 -1
		{{KEY_Silver}, {0}, {91}, {0, 0, 0, 0}, {0}},	//13 0
		{{KEY_Silver}, {0}, {92}, {0, 0, 0, 0}, {0}},	//14 1
		{{KEY_Silver}, {0}, {93}, {0, 0, 0, 0}, {0}},	//15 2
		{{KEY_Silver}, {0}, {94}, {0, 0, 0, 0}, {0}},	//16 3
		{{KEY_Silver}, {0}, {95}, {0, 0, 0, 0}, {0}},	//17 4
		{{KEY_Silver}, {0}, {96}, {0, 0, 0, 0}, {0}},	//18 5
		{{KEY_Silver}, {0}, {97}, {0, 0, 0, 0}, {0}},	//19 6
		{{KEY_Silver}, {0}, {98}, {0, 0, 0, 0}, {0}},	//20 7
		{{KEY_Silver}, {0}, {99}, {0, 0, 0, 0}, {0}},	//21 8
		{{KEY_Silver}, {0}, {100}, {0, 0, 0, 0}, {0}},	//22 9

	};

	const RECTPAD rect[] = {
		//����
		{0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},
		{TYPE_X14, TYPE_Y3, 152, TYPE_SIZE14, {Color_Black}, {Color_Black, Color_White}, {13}},									 //1 �ƶ�ʹ��
		{TYPE_X14, TYPE_Y3 + TYPE_SIZE14 + SAFE_DIST6, 152, TYPE_SIZE14, {Color_Black}, {Color_Black, Color_White}, {14}},		 //2 ����
		{TYPE_X14, TYPE_Y3 + (TYPE_SIZE14 + SAFE_DIST6) * 2, 152, TYPE_SIZE14, {Color_Black}, {Color_Black, Color_White}, {16}}, //3 �������

		{TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {15}},								 //4 ���-����
		{TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}}, //5 ���-����
	};

	// /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
	SetLanguage(MENU_SKIPJOG); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
	InitScreen();
	initViewStatus();
	updateViewStatus();

	// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

	// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
	if ((g_jognumber > 9999) || (g_jognumber < 2))
		g_jognumber = 10;
	if (g_jogonflag > 1)
		g_jogonflag = 0;
	if (g_jogmethod > 1)
		g_jogmethod = 0;
	if (g_jogfunction > 1)
		g_jogfunction = 0;
	stitch = g_jognumber;
	temp_jogonflag = g_jogonflag;
	temp_jogmethod = g_jogmethod;
	temp_jogfunction = g_jogfunction;

	wMaxFeedSum = g_patd.feed_sum;

	if (wMaxFeedSum < 100)
	{
		moddat = 10;
	}
	else if (wMaxFeedSum < 1000)
	{
		moddat = 100;
	}
	else if (wMaxFeedSum < 10000)
	{
		moddat = 1000;
	}
	else if (wMaxFeedSum < 100000)
	{
		moddat = 10000;
	}
	// /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

	// /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
	Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
				 SheetPage, rect[0].index[0], rect[0].colorT[0]); //�������

	//����������ʾ
	for (i = 1; i < RECT_SKIP_MAX; i++)
	{

		if (i >= RECT_SKIP_JOG && i <= RECT_SKIP_OUT)
		{
			DrawRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);

			Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST3,
						 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
						 SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
		}
		else if (i == RECT_SKIP_PIN_TIT)
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
			Draw24x24Str(rect[i].xpis + (rect[i].xsize - 24 * 2) / 2, rect[i].ypis + (rect[i].ysize - 24) / 2,
						 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
						 SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
		}
		else if (i == RECT_SKIP_PIN_STR)
		{
			DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
			DrawDottedRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
		}
	}
	DrawF16Str(rect[RECT_SKIP_PIN_STR].xpis,
			   rect[RECT_SKIP_PIN_STR].ypis + rect[RECT_SKIP_PIN_STR].ysize + SAFE_DIST4,
			   "(2-     )", rect[RECT_SKIP_PIN_STR].colorT[0]);

	DrawFU32(rect[RECT_SKIP_PIN_STR].xpis + 9 * 3,
			 rect[RECT_SKIP_PIN_STR].ypis + rect[RECT_SKIP_PIN_STR].ysize + SAFE_DIST4,
			 wMaxFeedSum, 5, rect[RECT_SKIP_PIN_STR].colorT[0]);
	//����������ʾ
	for (i = 2; i < KEY_SKIP_MAX; i++)
	{
		if ((i >= KEY_SKIP_OFF) && (i <= KEY_SKIP_OUT))
		{
		}
		else
			DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
	}

	// /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
	ViewPage(usepageno); //������ʾ����

	// /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

	Timer_ms = 30720; //0x7800
	Timerms = 0;
	Timer_ms1 = 0;
	while (1)
	{

		keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ

		// /[������ֵ����Ԥ����*begin]**********************************************************

		//�ض�����²���Ч����Ԥ����
		//��Ч����

		if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
		{
			if ((keyon == KEY_SKIP_UP) || (keyon == KEY_SKIP_DOWN))
			{
			}
			else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
		{
			//�ض�����²���Ч����

			if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
			{
				keyon = keydata;
				if (keyon == KEY_SKIP_SCRNSVR) //�ް�����Ч
				{
				}
				else
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
					beeponf = 1;

					if (g_keyBellT) //����ʱ����������ʱ��
						Beep_On(g_keyBellT);

					switch (keyon) //�������±�����ʾ����ʾ
					{
					case KEY_SKIP_0:
					case KEY_SKIP_1:
					case KEY_SKIP_2:
					case KEY_SKIP_3:
					case KEY_SKIP_4:
					case KEY_SKIP_5:
					case KEY_SKIP_6:
					case KEY_SKIP_7:
					case KEY_SKIP_8:
					case KEY_SKIP_9:
						BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
						DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_SKIP_0, 1, rect[0].colorT[0]);
						break;
					default:
						DrawFMenuStr(keybmpind[keyon].titleStr[0]);
						break;
					}
					delay = KEYFT;
				}
				else if (beeponf == 1) //�ڶ��ν���
				{
					beeponf = 2;
					//���г����������ܰ����Ĺ���

					delay = KEYNT;
				}
				else if (beeponf < 20)
				{
					beeponf++;
				}

				if ((keyon == KEY_SKIP_UP) || (keyon == KEY_SKIP_DOWN)) //���г����������ܰ����Ĺ���
				{
					keyoff = keyon;
					key_on_flag1 = 1;
				}
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
			if ((keyoff >= KEY_SKIP_0) && (keyoff <= KEY_SKIP_9))
			{
				stitch = (stitch % moddat) * 10 + keyoff - KEY_SKIP_0;
				if (stitch > wMaxFeedSum)
					stitch %= moddat;
				dis_updata5 = 1;
			}
			else
			{
				switch (keyoff)
				{
				case KEY_SKIP_SCRNSVR:
					pno = MENU_SCRNSVR;
					break;
				case KEY_SKIP_BACK:
					pnoindex--;
					pno = poldno[pnoindex];
					break;
				case KEY_SKIP_ENTER:
					pnoindex--;
					pno = poldno[pnoindex];
					g_jognumber = stitch;
					g_jogonflag = temp_jogonflag;
					g_jogmethod = temp_jogmethod;
					WriteJogmethod(g_jogmethod);
					excode = g_curExCode;
					if (g_UdZhouFlag == 3)
					{
						excode &= 0x7f; //����״̬���ܸı�
					}
					if (g_jogfunction != temp_jogfunction)
					{ //�������״̬�ı�
						OutputExcode1(excode, pno, 1);
					}
					g_jogfunction = temp_jogfunction;
					break;
				case KEY_SKIP_OFF:
					temp_jogonflag = 0;
					dis_updata1 = 1;
					break;
				case KEY_SKIP_ON:
					temp_jogonflag = 1;
					dis_updata1 = 1;
					break;
				case KEY_SKIP_PP:
					temp_jogmethod = 0;
					dis_updata2 = 1;
					break;
				case KEY_SKIP_PAT:
					temp_jogmethod = 1;
					dis_updata2 = 1;
					break;
				case KEY_SKIP_NOUT:
					temp_jogfunction = 0;
					dis_updata3 = 1;
					break;
				case KEY_SKIP_OUT:
					temp_jogfunction = 1;
					dis_updata3 = 1;
					break;
				case KEY_SKIP_CLEAN:
					stitch = 2;
					dis_updata5 = 1;
					break;
				case KEY_SKIP_UP:
					if (stitch < wMaxFeedSum)
						stitch++;
					dis_updata5 = 1;
					break;
				case KEY_SKIP_DOWN:
					if (stitch > 2)
						stitch--;
					dis_updata5 = 1;
					break;
				}
			}
			keyoff = 0;
		}
		// /[�����ɿ���ֵ����*end]**********************************************************

		// /[����������ʾ��Ϣ����*begin]**********************************************************
		if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
		{

			if (keyon == KEY_SKIP_SCRNSVR) //����Ҫ��ʾ�İ���
			{
			}
			else //���水��
			{
				DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
			}

			keyon = 0;
			beeponf = 0;
			BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
			Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
						 SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ʾ�ػ������
		}
		// /[����������ʾ��Ϣ����*end]**********************************************************

		// /[��ʱ1s��Ϣ����*begin]**********************************************************
		if (Timerms > 1000)
		{
			Timerms = 0;
		}
		// /[��ʱ1s��Ϣ����*end]**********************************************************

		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
		if (key_on_flag1)
		{
			// ErrorMsg(MENU_SKIPJOG);
		}
		// /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

		// /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
		if (beeponf == 0)
		{
			// ErrorMsg(MENU_SKIPJOG);
		}
		// /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

		// /[�������뿪�ذ�ť*begin]**********************************************************
		if (dis_updata1)
		{
			dis_updata1 = 0;

			f = KEY_SKIP_OFF;
			if (temp_jogonflag)
				f++;
			for (i = KEY_SKIP_OFF; i <= KEY_SKIP_ON; i++)
			{
				DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
						   (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
						   (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
			}
		}
		// /[�������뿪�ذ�ť*end]**********************************************************

		// /[�������뷽����ť*begin]**********************************************************
		if (dis_updata2)
		{
			dis_updata2 = 0;

			f = KEY_SKIP_PP;
			if (temp_jogmethod)
				f++;
			for (i = KEY_SKIP_PP; i <= KEY_SKIP_PAT; i++)
			{
				DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
						   (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
						   (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
			}
		}
		// /[�������뷽����ť*end]**********************************************************

		// /[�������빦�������ť*begin]**********************************************************
		if (dis_updata3)
		{
			dis_updata3 = 0;

			f = KEY_SKIP_NOUT;
			if (temp_jogfunction)
				f++;
			for (i = KEY_SKIP_NOUT; i <= KEY_SKIP_OUT; i++)
			{
				DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
						   (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
						   (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
			}
		}

		// /[�������빦�������ť*end]**********************************************************

		// /[�������ˢ��*begin]**********************************************************
		if (dis_updata5)
		{
			dis_updata5 = 0;
			DrawFillRect(rect[RECT_SKIP_PIN_STR].xpis + SAFE_DIST2, rect[RECT_SKIP_PIN_STR].ypis + SAFE_DIST2,
						 rect[RECT_SKIP_PIN_STR].xsize - 2 * SAFE_DIST2, rect[RECT_SKIP_PIN_STR].ysize - 2 * SAFE_DIST2,
						 rect[RECT_SKIP_PIN_STR].colorB[0]);
			DrawMFU32(rect[RECT_SKIP_PIN_STR].xpis + (rect[RECT_SKIP_PIN_STR].xsize - 4 * 13) / 2,
					  rect[RECT_SKIP_PIN_STR].ypis + (rect[RECT_SKIP_PIN_STR].ysize - 24) / 2,
					  stitch, 4, rect[RECT_SKIP_PIN_STR].colorT[0]);
		}
		// /[�������ˢ��*end]**********************************************************

		// /[ҳ���л�*begin]**********************************************************
		if (pno != MENU_SKIPJOG)
		{
			plastno = MENU_SKIPJOG;
			break;
		}
		// /[ҳ���л�*end]**********************************************************

		updateViewStatus();

		wdt();
	}
}

U8 fun_getPatData(U16 chFileNoTemp, U8 chDiskTemp, U16 chPageNo)
{
	U8 errorf = 0, f = 0;
	U32 i = 0;
	memset((U8 *)SREG, 0, sizeof(SREG));
	while (errorf < 3 && (chFileNoTemp >= MINFILENO && chFileNoTemp <= MAXFILENO))
	{
		if ((chDiskTemp == 0) /*||((g_MboardUSB == 1)&&(chDiskTemp == 1)) */) //�Ӽ����塢����U�̶������ļ�
		{
			if (chDiskTemp == 0)
				i = C_READMEMORYPAT; // �Ӽ�����������ļ�
			else
				i = C_READFLASHPAT; // ��U�̶������ļ�
			if (Write_Cmd(i, chFileNoTemp, 0) == 0)
			{
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex] = chPageNo;
				pnoindex++;
				break;
			}
			else
			{
				if (Read_Status() == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[pnoindex] = chPageNo;
					pnoindex++;
					break;
				}
				else if (DREG[D_CMDSTATUS] == 98)
				{
					//if(chDiskTemp){
					//	pno = MENU_RWHELP;hleppara = 2;poldno[1] = chPageNo;pnoindex = 2;break;
					//}
				}
				else if (DREG[D_CMDSTATUS] == 99)
				{
					//if(chDiskTemp==0){
					pno = MENU_RWHELP;
					hleppara = 3;
					poldno[pnoindex] = chPageNo;
					pnoindex++;
					break;
					//}else{
					//	pno = MENU_RWHELP;hleppara = 4;poldno[1] = chPageNo;pnoindex = 2;break;
					//}
				}
				else if (DREG[D_CMDSTATUS] < 98)
				{
					pno = MENU_RWHELP;
					hleppara = 16;
					poldno[pnoindex] = chPageNo;
					pnoindex++;
					break;
				}
				else
				{
					f = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&F_IND_NO)); // ����ǰ�����ļ���Ϣ
					if (f == 0)
					{
						pno = MENU_RWHELP;
						hleppara = 15;
						poldno[pnoindex] = chPageNo;
						pnoindex++;
						break;
					}
					else
					{
						if (chFileNoTemp != F_IND_NO)
						{
							pno = MENU_RWHELP;
							hleppara = 16;
							poldno[pnoindex] = chPageNo;
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
								pno = MENU_RWHELP;
								hleppara = 15;
								poldno[pnoindex] = chPageNo;
								pnoindex++;
								break;
							}
							else
							{
								if ((DataCheckSum((U16 *)(&SREG[0]), i,
												  ((((IREG2[(chFileNoTemp - MINFILENO) * FileIndexSize + FileIndexSize - 1] & 0xff00) >> 8) != 0) ? 0 : 1))) == 0)
								{ //����У��ʧ��
									errorf++;
								}
								else
								{
									for (; i < MAXSREG; i++)
										SREG[i] = 0;
									break;
								}
							}
						}
					}
				}
			}
		}
		else //��ȡ������洢���豸����
		{
			F_IND_NO = chFileNoTemp;
			/*if(chDiskTemp==1)*/ Select_ExtMemory(USBTF);
			//else Select_ExtMemory(SD);

			if (Read_PatFile(chFileNoTemp) == 0)
			{
				i = ((F_FILELENH << 16) + F_FILELENL) / 2;
				for (; i < MAXSREG; i++)
					SREG[i] = 0;
			}
			else
			{
				errorf++;
			}
			break;
		}
		wdt();
	}
	wdt();
	if (errorf == 3)
	{
		pno = MENU_RWHELP;
		hleppara = 64;
		poldno[pnoindex] = chPageNo;
		pnoindex++; //���ݳ���
		errorf = 0;
		return 0; //�޸�����
	}
	else
	{
		errorf = 0;
		return 1; //�޸�����
	}
}

U8 fun_backPattingData(U32 tmp_pno)
{
	U8 flag = 1;
	U16 dat;
	U8 ch;
	U32 n, ii;
	U16 tmpBorcmd;
	S8 tmpBorpooft, Borpfdis;

	if ((poldno[pnoindex - 1] == MENU_MODIFY) || (poldno[pnoindex - 1] == MENU_CONVERT))
	{
		//	if(F_FILETYPE==1){
		//		AnalyseDataCorrect();								//��黨�����ݵ���ȷ�Բ�У��
		//		Createdata();

		//		if(1==ConvertMultiDraw()){
		//			Createdata();
		//			}
		//	}
	}
	else if (poldno[pnoindex - 1] == MENU_CURSOR) //�ָ���ͼ����
	{
		if (F_FILETYPE == PAT_MIT)
		{
			ii = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
			g_temp_line_p = (SREG[2] + 0x40) / 2;
			g_temp_pat_p = g_temp_line_p + ii;
			if ((g_temp_line_p <= 32000) && (g_temp_pat_p <= 52000) && (ii < 20000))
			{
				if ((S_CLAMP) || (P_OST == 1))
				{
					g_init_flag = 3;
					g_draw_p = g_temp_line_p;
					g_line_p = 32000 + ii;
					g_pat_p = 52000 + ii * 2;

					n = g_temp_pat_p + ii * 2;
					g_dxpos = 0;
					g_dypos = 0;
					g_oxpos = 0;
					g_oypos = 0;
#if PATRULES_ORG_OFFSET
					if (SREG[3] == 7) //����-�޸�ԭ��λ��
					{
						g_oxpos = (S16)(SREG[6]);
						g_oypos = (S16)(SREG[7]);
					}
#endif
					g_feed_sew = 0xff;
					for (; ii > 0; ii--)
					{
						SREG[--g_pat_p] = SREG[--n];
						dat = SREG[n];
						SREG[--g_pat_p] = SREG[--n];
						if (((SREG[n] & 0x00ff) == 0x61) || ((SREG[n] & 0x00ff) == 0x41) || ((SREG[n] & 0x00ff) == 0x21) || ((SREG[n] & 0x00ff) == 0x1) || ((SREG[n] & 0x00ff) == 0x1b) || ((SREG[n] & 0x00ff) == 0x3))
						{
							if (g_feed_sew == 0xff)
							{
								if ((SREG[n] == 0x3) || (SREG[n] == 0x1b))
									g_feed_sew = 0;
								else
									g_feed_sew = 1;
							}
							ch = (U8)(dat);
							if (ch >= 0x80)
							{
								ch = ch - 0x80;
								g_oxpos = g_oxpos - (S16)(ch);
							}
							else
							{
								g_oxpos = g_oxpos + (S16)(ch);
							}
							ch = (U8)((U16)(dat) >> 8);
							if (ch >= 0x80)
							{
								ch = ch - 0x80;
								g_oypos = g_oypos - (S16)(ch);
							}
							else
							{
								g_oypos = g_oypos + (S16)(ch);
							}
						}
						else if ((g_feed_sew == 0xff) && (SREG[n] == 0x2))
						{
							g_feed_sew = 0;
						}
					}
					if (g_feed_sew == 0xff)
						g_feed_sew = 0;
					Comm_Move(g_oxpos, g_oypos, 120, MENU_INPUT, 0, 0);
					ii = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
					for (; ii > 0; ii--)
					{
						SREG[--g_line_p] = SREG[--n];
					}
					for (ii = g_draw_p; ii < 32000; ii++)
					{
						SREG[ii] = 0;
					}

					g_line_p = 32000 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
					g_pat_p = 52000 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 2);
					SREG[2] = 64000 - 0x40;
					SREG[4] = 0x3880;
					SREG[5] = 1;
					///////////////////////////////
					g_temp_line_p = SREG[g_draw_p - 4];
					g_temp_pat_p = SREG[g_pat_p - 2];
					if (((g_temp_line_p == 0x1f01) && (g_temp_pat_p == 0x1f)) || ((g_temp_line_p == 0x1e01) && (g_temp_pat_p == 0x1e)))
					{
						g_draw_p -= 4;
						g_line_p -= 1;
						g_pat_p -= 2;
						SREG[g_draw_p] = 0;

						///////////////////////////////////////////////ADD ON 1.16

						g_temp_line_p = SREG[g_draw_p - 4];
						g_temp_pat_p = SREG[g_pat_p - 2];
						if ((g_temp_line_p == 0x0201) && (g_temp_pat_p == 0x02))
						{
							g_draw_p -= 4;
							g_line_p -= 1;
							g_pat_p -= 2;
							SREG[g_draw_p] = 0;
							g_DoTrim = 1; //ɾ�����߱�־
						}
					}
					g_line_sum = (U16)((SREG[g_line_p - 1]) & 0x3fff) + 1;
					g_feedc = g_line_p - (SREG[2] + 0x40) / 2;
					g_feedsum = g_feedc;
					for (ii = 0x24; ii < g_draw_p; ii += 4)
					{
						if (SREG[ii] == 0x601)
						{
							g_home_2flag = 1;
							g_home_2xpos = 0;
							g_home_2ypos = 0;
							for (; ii > 0x24; ii -= 4)
							{
								g_home_2xpos += (S16)(SREG[ii - 2]);
								g_home_2ypos += (S16)(SREG[ii - 1]);
							}
							break;
						}
					}
				}
				else
				{

					flag = 0;
					hleppara = 13;
					poldno[pnoindex++] = tmp_pno;
					pno = MENU_RWHELP;
				}
			}
			else
			{
			}
		}
		else if (F_FILETYPE == PAT_SEW)
		{

			Bor_feedInfo(g_patd.feed_sum, g_CtPatLen, &tmpBorcmd, (S16 *)&g_oxpos, (S16 *)&g_oypos, (S16 *)&g_dxpos, (S16 *)&g_dxpos, (U8 *)&g_oldspeed, &tmpBorpooft, &Borpfdis); //���ҵ�ǰԭ��������������
			Comm_Move(g_oxpos + g_dxpos, g_oypos + g_dxpos, 120, MENU_CURSOR, 1, 1);																							   //�ƶ�����������������
			g_oxpos = g_oxpos + g_dxpos;
			g_oypos = g_oypos + g_dxpos;
			g_disxpos = g_oxpos;
			g_disypos = g_oypos;
			g_dxpos = 0;
			g_dxpos = 0;

			g_pf_absval = tmpBorpooft;
			g_pf_absval += g_pfootd.base;
			Bor_CtBefDatAny((U32 *)&g_Ctstaddr, (U32 *)&g_CtPatLen, (U8 *)&g_oldspeed, 1);
			g_feedsum = g_patd.feed_sum;
			g_feedc = g_patd.feed_sum;
			g_Ctendaddr = g_Ctstaddr;
			g_Ctsavetimes = 0;
			//	printf("g_Ctstaddr = %d  g_CtPatLen = %d \r\n",g_Ctstaddr,g_CtPatLen);
			for (ii = 0; ii < MAXBLKBUF; ii++)
			{
				BLKBUF[ii] = 0;
			}
			Bor_CreatBack((U16 *)&g_Ctsavetimes, g_Ctstaddr, g_Ctendaddr, g_oxpos, g_oypos, g_oldspeed, g_pf_absval - g_pfootd.base, g_pf_dis, g_lineforceValSetNow, g_patd.orgX, g_patd.orgY);
		}
		g_array4[0][0] = 0;
		g_point_num = 1;
		g_feed_flag = 0;
		g_ct_status_flag = 0;
		g_ct_status_flag |= (bit1 | bit4 | bit7 | bit9 | bit10);
	}
	else
	{
		if (g_recv_netdata)
		{
			g_recv_netdata = 0;
			Write_Cmd(C_READMEMORYPAT, g_Fileno, 0);
			RestoreSHREG(0x03);
		}
	}
	return flag;
}
