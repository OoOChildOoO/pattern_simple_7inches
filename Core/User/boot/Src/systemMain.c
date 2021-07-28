//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  system2Main.c							*
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
//*															*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
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

extern BOOTSCR g_bootscr; //����boot�����Ϣ
extern volatile U8 g_vs1003err;
extern volatile U8 g_uperr; //����ʧ��
extern volatile U32 mp3dver, mp3ddate;
extern volatile U8 g_disk_memory; //�洢��־ѡ��
extern volatile U8 gPeessVal;	  //����������
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

/******************������*begin*****************/
volatile U8 g_enctTackCode = 0; //�����Զ�׷���ص���ʹ��
volatile U8 g_needtohome = 0;	//��Ҫ�ֶ���ԭ��
volatile U8 g_readcovnflag;
volatile U8 g_readcovnflag1;
volatile U16 g_saveflag;
volatile S16 g_temp_PfootOffVal;
/******************������*end*****************/

/*-----------------------------------------------
//������: void Run_Bor_APP(void)
//����  : ϵͳ�������
//����  : 	
//���  : 
//����ֵ: ��
//-----------------------------------------------*/
void systemApp(void)
{
	U16 i;
	U16 flasherr = 0;				   //flash�����쳣��־
	U16 mp3err = 0;					   //MP3�����쳣��־
	U8 defver[3] = {0x30, 0x30, 0x30}; //����ʧ��ʱ���汾������Ϊ0
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

	if (g_bootscr.mode != 3) //��������ȴ�
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
	if (gPeessVal > PEESS_MAXVAL || gPeessVal < PEESS_MINVAL) //���屣��ѹ��ֵ����,��ȡ���������ֵ
	{
		gPeessVal = ReadPessVal();
	}
	Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 7, (U16 *)(&YREG[SYS_MACHINE_TYPE])); //��ϵͳ�汾,�����Ϣ

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
		g_draw_p = 0x24;  //��ͼ���ݵ�ǰλ��ָ��
		g_line_p = 32000; //�������ݵ�ǰλ��ָ��
		g_pat_p = 52000;  //�����ݵ�ǰλ��ָ��
		g_init_flag = 0;
		g_data_flash = 1;
	}
	g_Mac.max_xl = -2000;
	g_Mac.max_xr = 2000;
	g_Mac.max_yu = 1000;
	g_Mac.max_yd = -1000;
	Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS])); //��ȡϵͳ״̬

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

	CheckTouchPara(); //������У�����
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

	if (flasherr) // Flash ���²��ɹ�
	{
		W25QXX_Write(W25Q1_DAT, (U8 *)defver, FLASHINDEXADDR + 12, 3);
		flashver = 0;
		pno = MENU_RWHELP;
		hleppara = 96;
		poldno[0] = tmp_po;
		g_uperr = 1;
		pnoindex = 1 /*InitScreen();ViewPage(usepageno)*/;
	}
	if (mp3err) // mp3 ���²��ɹ�
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

	if (flashver == 0) //flash����
	{
		pno = MENU_RWHELP;
		hleppara = 96;
		poldno[0] = tmp_po;
		g_uperr = 1;
		pnoindex = 1;
	}

	Read_D(D_Address[SYS] + SYS_MAINBOARD_VER, 1, (U16 *)(&YREG[SYS_MAINBOARD_VER])); //��ϵͳ�汾,�����Ϣ
	MainBoardVer = YREG[SYS_MAINBOARD_VER];

	g_SysErr = ((MainBoardVer / 1000 == 2) ? 0 : 1);
	g_SysErr = 0;

	g_selectdisno = HREG[0];

	//P_R = 1 * 60;	//����ʱ��һ����
	//pno = MENU_C2_FPROG;
	//pno = MENU_G_HELP;hleppara = 147;
	while (1)
	{
		//printf("pno = %d \r\n",pno);
		EEPREG[1] = pno;
		switch (pno)
		{

		case MENU_P1: //����������
			Menu_P1();
			break;
		case MENU_P2:
			Menu_P2(); // ���������(�λ���)
			break;
		case MENU_MENU:
			Menu_Menu(); // �˵�����
			break;
		case MENU_THREAD: //����ģʽ
			Menu_Thread();
			break;
		case MENU_COUNTER:
			Menu_Counter(g_para0); // ������
			break;
		case MENU_READ:
			Menu_Read(); // ��ȡ��������
			break;
		case MENU_DIRREAD: //ֱ�Ӷ�ȡĿ¼
			Menu_DirRead();
			break;
		case MENU_WRITE:
			Menu_Write(); // ���滨������
			break;
		case MENU_WRITE1:
			Menu_Write1(); // ����ģʽ
			break;
		case MENU_WIND:
			Menu_Wind(); // ������ʾ�˵�
			break;
		case MENU_SKIPJOG:
			Menu_SkipJog(); // ��Ծ�綯����
			break;
		case MENU_PFOOT:
			Menu_CtPfootSet(g_para0); // ��ѹ�����á��޸Ľ���
			break;
		case MENU_LINEFORCE:
			Menu_Ctlineforce(g_para0); // �����������ò˵�
			break;
		case MENU_HELP:
			Menu_HeSupportConnect(g_para1); //��������
			break;

		case MENU_RWHELP:
			if ((hleppara == 21) && (DREG[D_ERRWAR] - 0x8000 == 388))
			{
				pno = MENU_RWHELP1;
				// Menu_RwHelp1();	// ��д��������
			}
			else
			{
				Menu_RwHelp(hleppara); // ��д��������
			}
			break;
		case MENU_INPUT:
			Menu_CtInput(); // �������뻭��
			break;
		case MENU_CURSOR:
			Menu_CtCursor(); // ��������
			break;
		case MENU_INMENU:
			Menu_CtInputMenu(); // �˵�����
			break;
		case MENU_PITCH:
			Menu_Ctpitch(); // ����޸�
			break;
		case MENU_METHOD:
			Menu_CtMethod(); // ���뷽�����û���
			break;
		case MENU_MULTI:
			Menu_Ctmultiple(); // ���ط��趨����
			break;
		case MENU_ZIGZAG:
			Menu_Ctzigzag(); // Z�ַ��趨����
			break;
		case MENU_TACKING:
			Menu_Cttacking(); // �������ò˵�
			break;
		case MENU_TACKING1:
			Menu_CtTack1(); // �������ò˵�
			break;
		case MENU_CODE:
			Menu_CtCode(); // ���븨�����û���
			break;
		case MENU_EXCODE:
			Menu_CtExCode(); // �����������仭��1
			break;
		case MENU_INHELP:
			Menu_CtInHelp(hleppara); // ��������˵�
			break;
		case MENU_CALLFILE:
			Menu_CtCallFile(); // �����û���
			break;
		case MENU_DIRCALLFILE:
			Menu_DirCallFile();
			break;

		case MENU_MODIFY:
			Menu_Modify(); // �����޸Ļ���
			break;
		case MENU_MODIFYADD:
			Menu_ModiAdd(g_para1); // �綯��������
			break;
		case MENU_MODIFY1:
			if (g_para1 == 5 && g_num_mov_flag == 2)
			{
				Menu_ModifyPatChangeMethod(g_para1);
			}
			else
			{
				Menu_Modify1(g_para1); // ��Ŀ����
			}
			break;
		case MENU_MODIFY2:
			Menu_Modify2(0); // �ٶ�ָ������
			break;
		case MENU_MODIFY3:
			Menu_Modify3(g_modify_code); // ���ӹ�������
			break;
		case MENU_MOIMAGE: // �޸�ģ���е�ͼ������

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
			Menu_Convert(); // �����任����
			break;
		case MENU_CONVERT1:
			Menu_Convert1(g_para1); // �����任�˵�
			break;

		case MENU_PROG: //��ʽģ��
			Menu_Prog();
			break;
		case MENU_PROFRAME: //��ʽģ������˵�
			Menu_ProFrame();
			break;
		case MENU_PROPARA: //��ʽģ���������
			Menu_ProPara();
			break;
		case MENU_DIFFERENT: //��ʽģ���������
			Menu_ProDifferent();
			break;
		case MENU_FPROG: //���ٳ�ʽģ���������
			Menu_FastProg();
			break;

		case MENU_IOTEST:
			Menu_Iotest(); // IO���Ի���
			break;
		case MENU_INPUTSTATUS:
			Menu_Inputstatus(); // �����ź�״̬��ʾ
			break;
		case MENU_OUTPUTSTATUS:
			Menu_Outputstatus(); // ����ź�״̬��ʾ
			break;
		case MENU_SENSORSTATUS:
			Menu_Sensorstatus(); // ������ź�״̬��ʾ
			break;
		case MENU_INPUTSET:
			Menu_Inputset(); // �����ź�����
			break;
		case MENU_OUTPUTSET:
			Menu_Outputset(); // ����ź�����
			break;
		case MENU_ANALOGSET: //ģ������������趨
			Menu_AnalogSet();
			break;

		case MENU_FUNCTION:
			Menu_Function(); // ���ܻ���
			break;
		case MENU_FTCOPY:
			Menu_FtCopy(); // ͼ�ο���
			break;
			//   case MENU_FTNEEDLE:
			// 	Menu_FtNeedle();		// ��ͣ������
			// 	break;
		case MENU_FTFORMAT:
			Menu_FtFormat(); // ��ʽ��
			break;
		case MENU_FTGROUP:
			Menu_FtGroup(); // ͼ�����
			break;
		case MENU_FTGROUPREAD1: //ͼ����϶��ڴ滨��
			Menu_FtReadMem();
			break;
		case MENU_FTDIRREAD: //ͼ����϶��ڴ滨��-ֱ�Ӷ�ȡ����
			Menu_FtDirReadMem();
			break;
		case MENU_FTGROUPREAD2: //ͼ����϶���ϻ���
			Menu_FtReadCombine();
			break;
		case MENU_FTGROUPWRITE: //ͼ����ϱ�����ϻ���
			Menu_FtWriteCombine();
			break;
		case MENU_FTSYSVERSION:
			Menu_FtSysversion(); // �汾
			break;
		case MENU_FTSCREENSET:
			Menu_FtScreenset(); // ��ʾ����
			break;
		case MENU_FTSCREENBOOTSET:
			Menu_FtBootScreen();
			break;
		case MENU_FTSYSRESTORE:
			Menu_FtSysrestore(); // ϵͳ��ԭ����
			break;
		case MENU_FTSYSBACKUP:
			Menu_FtSysbackup(); // ϵͳ����
			break;
		case MENU_FTUSER:
			Menu_FtUser(); // �û���¼
			break;
			//   case MENU_FTERRRECORD:
			// 	Menu_FtErrrecord();		// ������Ϣ��¼
			// 	break;
			//   case MENU_FTRUNRECORD:
			// 	Menu_FtRunrecord();		// ������Ϣ��¼
			// 	break;
			//   case MENU_FTNETWORD:
			// 	Menu_FtNetword();		// ���繤����
			// 	break;
			//   case MENU_FTCHAT:
			// 	Menu_FtNewChat();		//��������
			// 	break;
			//   case	MENU_FTSETIP:		//���û���IP������
			// 	Menu_FtSetIP(g_para1);
			// 	break;
			//   case MENU_FTNETRW:		//���Ͷ�ȡԶ���ļ�
			// 	Menu_FtNetwordRW(g_para1,g_netSelectMac);
			// 	break;
			//   case MENU_FTRESET:
			// 	Menu_FtReset();			// һ����ԭ
			// 	break;
			// case MENU_PARAMODIFY:       //������������
			// 	Menu_FtParaCorr();
			// 	break;
			//   case MENU_FTSERVO:		//�ŷ�����
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
