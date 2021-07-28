//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  main.c									*
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
#include "main.h"
#include "utility.h"
#include "lcddisp.h"
#include "register.h"
#include "menu.h"
#include "color.h"
#include "communication.h"
#include "touch.h"
#include "vs1003.h"
#include "w25qxx.h"
#include "S39vf160.h"
#include "aviplayer.h"
#include "ds2460.h"

U16 SYSDAT[0x40] __attribute__((at(0x20000000)));

U8 CPUID[12];
U32 SYSSETID;

#define TLFNADD_TOUCHSET 224

const U16 draw_code[][2] = {{0, 0}, {0x501, 0}, {0xb01, 0}, {0xc01, 0}, {0xd01, 0}, {0x1d01, 0}, {0x1d01, 1}, {0x1d01, 2}, {0x1d01, 3}, {0x401, 0}, {0x1401, 0}, {0x201, 0}, {0x701, 0}, {0x601, 0}, {0, 0}, {0x1c01, 0}, {0x1c01, 3}, {0x1c01, 1}, {0x1c01, 2}};
const U16 pat_code[] = {0, 0x5, 0xb, 0xc, 0xd, 0x1d, 0x11d, 0x21d, 0x31d, 0x4, 0x14, 0x2, 0x7, 0x6, 0, 0x1c, 0x031c, 0x011c, 0x021c};

//const U16 PC_bmp[16] = {470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485};	//ָ���ͼƬ
SystemTime g_SysTimes;
ProInfo g_ProInfo;//flash���ݰ汾��

#if USEMULTCOLOR //��ɫ
const U8 mCOLOR = 1;
#else
const U8 mCOLOR = 0;
#endif

volatile U8 g_usbmount;	   //USB���ر��
volatile U8 g_fbmac = 0;   //�۱���
volatile U8 g_disk_memory; //��ʼ���洢����־ 0-FLASHl;1-U��;2-SD
volatile U8 buttonflag[6]; // �����趨�����Ƿ���ʾ��־- ���½ǵļ�������ʹ��
volatile U16 g_Fileno;		//�����ļ����
volatile U8 g_GroupName[9];	  //��ϻ���������
volatile U8 g_GroupType;	  //1-���ģʽ;2-�Ӻ�ģʽ
volatile U8 g_ImgMove;		  //ͼ���϶����ܣ�����
volatile U8 g_NoReadPatf = 0; //����ȡ������־  1-����ȡ�������� 2-ǿ���ض�һ�λ�������
volatile U16 g_draw_p;//��ͼ��ʼ��ַ
volatile U16 g_line_p;//������ʼ��ַ
volatile U32 g_pat_p;//�����ʼ��ַ
volatile U16 g_line_sum;//��������
volatile U8 g_BarcodeFlag[MAXFILENO - MINFILENO];
volatile U8 g_jogfunction;//��������-���빦�����
volatile U8 g_jogfunction1;
volatile U16 *g_CodeNameAdd; //���ӹ�������
volatile U8 g_sysType;		 //0-M 1-H
volatile U8 g_runstepf;		//��Ļ������ý���� �ڲ��������
select_id_message g_SelectNo[SELECTNO_MAX]; //��ѡ�еĻ����ļ��˵���Ϣ
volatile U8 g_checkGroup;					//���ͼ�μ��
volatile U16 g_Groupsel1;
volatile U16 g_Combinepat[GroupMax][6]; //���ͼ��ѡ����-����
volatile U8 g_GroupSave;				//�������ͼ�� 0-������  1-���浽�ڴ�  2-���浽U�� 3-���浽SD�� 4-���浽Ĭ��λ��
volatile U8 g_GroupCover;				//���ͼ�θ��Ǳ�־
volatile U8 g_GroupDoingNO = 0;				//���ͼ�����ڲ������
volatile U8 g_GroupMenth;				//��ǰ���ͼ�β�������  0-  1-����  2-��ȡ   3-�����ǰ���ͼ��
volatile U8 g_GroupPatNums;				//��ȡ����ļ���
volatile U8 g_tmpGroName[10];
volatile U8 *g_tGname;
volatile U8 g_KeyLockStaus = 1;		//��������־λ

volatile U16 g_MachineTy;

volatile S16 g_PinModParaANG; // �����ת��������_0��λ�ýǶ�

volatile S16 g_PinModChkSum;  // �����ת��������У��ֵ
volatile S16 g_PinModParaX1;  // �����ת��������_90����������X
volatile S16 g_PinModParaY1;  // �����ת��������_180����������Y
volatile S16 g_PinModParaX2;  // �����ת��������_180����������X
volatile S16 g_PinModParaY2;  // �����ת��������_180����������Y
volatile S16 g_PinModParaX3;  // �����ת��������_270����������X
volatile S16 g_PinModParaY3;  // �����ת��������_270����������Y
volatile S16 g_PinModParaXR;  // �����ת��������_Բ������X
volatile S16 g_PinModParaYR;  // �����ת��������_Բ������Y
volatile S16 g_PinModParaRD;  // �����ת��������_�뾶
volatile S16 g_PinModParaANG; // �����ת��������_0��λ�ýǶ�

volatile S16 g_ViewModParaX; //�Ӿ�����ϵƫ��X
volatile S16 g_ViewModParaY; //�Ӿ�����ϵƫ��Y

volatile S16 g_OrgModChkSum; // ��еԭ����������У��
volatile S16 g_OrgModParaX;	 // ��еԭ����������_��������X
volatile S16 g_OrgModParaY;	 // ��еԭ����������_��������Y
volatile S16 g_OrgModParaZ;	 // ��еԭ����������_��������Z

volatile S16 g_OrgModParaXD; // ��еԭ��D����������_��������X
volatile S16 g_OrgModParaYD; // ��еԭ��D����������_��������Y
volatile U8 g_ModHome = 0;	 //0-����M��ԭ�� 1-����D��ԭ��

volatile S16 g_GapModChkSum;  // ����ת��϶��������У��
volatile S16 g_GapModPosadjX; //����ת��϶��������_��������X+
volatile S16 g_GapModNegadjX; //����ת��϶��������_��������X-
volatile S16 g_GapModPosadjY; //����ת��϶��������_��������Y+
volatile S16 g_GapModNegadjY; //����ת��϶��������_��������Y-

volatile S16 g_upDISP;	 // ��������� - 0:������ 1:����
volatile S16 g_upFLASH;	 // ��������� FLASH  - 0:������ 1:���� 2:AUTO
volatile S16 g_upMBOARD; // �������� - 0:������ 1:����
volatile S16 g_upWIFI;	 // ����wifiģ�� - 0:������ 1:���� 2:AUTO

volatile OVLP g_autoOvlp; //����׷���ص������

volatile U8 g_ExCode = 0; //��ʾ���ӹ������״̬
volatile U8 g_tExCode = 0;
volatile U8 g_curExCode = 0;
volatile U8 g_CodeEn = 1;//�޸� ���� ��ʾ����ʹ�ܣ�Ԥ��������Ч��

volatile U8 g_ImgPoint = 0;//�綯��־λ��1��֧�ִ綯, ѡ�е�֧��
volatile U8 g_ImgMove = 0;//�����ƶ���־λ 1��֧�ֿ����ƶ�
volatile U16 g_LPoint; //��λ��
volatile U8 g_datum_mark;

volatile S16 g_disxpos, g_disypos; //x,y����ʵ��ֵ
volatile S16 g_dxpos, g_dypos;	   //x,y����仯ֵ
volatile S16 g_tdxpos, g_tdypos;   //x,y����仯ֵ����
volatile S16 g_tmpox = 0, g_tmpoy = 0;
volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //ԭ������-��������ϵ
volatile S16 g_tmpoxpos, g_tmpoypos;//��¼����ƶ�ʱ����ǰ�������
volatile S16 g_temp_oxpos, g_temp_oypos;

volatile U32 g_feedc; //��ǰ��
volatile U32 g_max_feedsum;//��ת���������
volatile U32 g_min_feedsum;;//��ת����С����
volatile U32 g_jog_stpoint;//��ת��ʼ��
volatile U32 g_jog_enpoint;//��ת������
volatile U32 g_jog_point;//��ǰ��ת��
volatile U16 g_point_num; //���Ƶ���
volatile U8 g_sta_end_flag;//��ת��ʼ��������־λ 0��δѡ����㣬δѡ���յ㣬1��ѡ����㣬δѡ���յ�
volatile U8 g_jogmode;//ѡ�ν����ѡ�������׼ģʽ 0:JOG_ON (ÿ�����ѡ�е���������)1:JOG_OFF(ÿ��Ϊ�����������)
volatile S16 g_maxX;
volatile S16 g_minX;
volatile S16 g_maxY;
volatile S16 g_minY;
volatile U32 g_sewPTlen; //sew��������(������������)��С8b

volatile U8 g_BorAddFuns; //���븽�ӹ��� ÿһ��λ����һ������
volatile U8 g_BorDelFuns; //ɾ�����ӹ��� ÿһ��λ����һ������
volatile U16 g_pinAng;
volatile U16 g_curPinAng = 0;	  //��ǰ״̬��Ƕ�ֵ
volatile U16 g_pointPinAng = 0;	  //������Ӧ�Ƕ�ֵ
volatile U16 g_curSetPara1 = 0;	  //��ǰ״̬����һ����
volatile U16 g_pointSetPara1 = 0; //������Ӧ���ö�����ֵ
volatile U16 g_curSetPara2 = 0;	  //��ǰ״̬���ö�����
volatile U16 g_pointSetPara2 = 0; //������Ӧ���ö�����ֵ
volatile U16 g_AddRela;			  //�����ʽתΪ�����׷��

volatile S16 g_pf_bdis;
volatile S8 g_pf_dis;
volatile U8 g_speed;
volatile U8 g_oldspeed;
volatile U8 g_input_method; //����ģʽ 0-���� 1-�� 2-ֱ�� 3-���� 4-Բ 5-Բ�� 6-����
volatile U8 g_para_offset;	//���ط� 0-�޶��ط� 1-ͬ����Ͷ��ط� 2-������Ͷ��ط� 3-��� 4-ͬ����Ҷ��ط� 5-������Ҷ��ط�
volatile U8 g_direction;	//���ط췽��
volatile U8 g_distance;		//���ط����
volatile U8 g_times;		//���ط����

volatile U16 g_temp_draw_p;
volatile U16 g_temp_line_p;
volatile U32 g_temp_pat_p;
volatile U16 g_temp_point_num;
volatile U8 g_MboardUSB; //����U�̴洢�������־ 0��δ����  1������

volatile U16 g_Ptya;		   //�綯����ֵ	2010.4.6
volatile U8 g_pitch = 30;	   // ���ʱ�������0-12.7		20151211
volatile U32 g_ct_status_flag; //���״̬��־
volatile U8 g_ct_insert_flag;  //�������־ 0:�޲��룬1:�ɲ��� 2:���ڲ���
volatile U8 g_feed_flag;	   //�����ͱ�־
volatile U32 g_feedsum;		   //����
volatile U32 g_feedpat;			
volatile U8 g_bt_mode;	//����ģʽ
volatile U8 g_repeat;	//�ص������
volatile U8 g_r_stitch; //�ص�������
volatile U8 g_srt_bt;	//ǰ�˵������
volatile U8 g_s_stitch; //ǰ�˵�������
volatile U8 g_ent_bt;	//��˵������
volatile U8 g_e_stitch; //��˵�������
volatile U8 g_zrota;

volatile U8 g_zig_zag;		 //Z�ַ�
volatile U8 g_zig_width;	 //Z����
volatile U8 g_zig_pitch;	 //Z�����
volatile U8 g_zig_direction; //Z�췽��

volatile U8 g_dopfoot = 0;
volatile U8 g_DoTrim;
volatile U8 g_lineforceValSetNow; // ��ǰ������������ֵ
volatile U8 g_lineforceValSet;	  // ����������׼ֵ
volatile U8 g_DelAll_flag = 0, g_DelAll_flag1 = 0;
volatile U16 g_EndbtNum = 0;
volatile U16 g_btNumber = 0;
volatile U8 g_ModifyDrawFlag = 0;
volatile U8 g_ModifyMode = 0xff;
volatile U16 g_savetimes; //�������ݴ���
volatile U16 g_Tsavetimes;
volatile U8 g_code_mode;
volatile U8 g_feed_sew; // ǰһ��Ŀ��ͻ���ұ�־
volatile U8 g_originf;	// ����������λ�ñ�־ 0������ԭ��λ�� 1: ��ԭ��λ��
volatile S8 g_lf_dis_modify;
volatile S8 g_pf_dis_modify;
volatile U8 g_mitdotack;

volatile U8 g_patname[64];//���뻨��������
volatile U16 g_pattno;
volatile U32 g_modaddr[30] = {0};
volatile U8 g_modify_mode;
volatile U8 g_modify_method;
volatile U8 g_modify_gohome; //0: ֱ��ʽ���ͻ�ԭ�� 1: ���ʽ���ͻ�ԭ��
volatile U8 g_modify_code;
volatile U8 g_modifyflag;
volatile U8 g_modifyflag1;
volatile U8 g_modifyf;

volatile U8 g_overflag11 = 0; // �ж��޸�ʱ�Ƿ��Ѿ����ڴ洢��Χ�ı�־
volatile U8 g_num_mov_flag; //0 :������ 1:�������� 2:�������� 3:����ɽ���
volatile U16 g_stitch_number = 1;

volatile S16 g_pf_absval;  //��ѹ�ŵ�ǰ����ֵ(������׼ֵ)
volatile U8 g_pf_mode = 0; //��ѹ��	1:������ 0��������
volatile U8 g_pf_modify_mode;
volatile U16 g_pitch_parameter = 30; // �����޸Ĺ������

volatile U16 g_convert_xscale;//�任-����-X����
volatile U16 g_convert_yscale;//�任-����-Y����
volatile U16 g_convert_pindis;//�任-����-���

volatile U16 g_parameter;
volatile U8 g_selectmode;

volatile U16 g_mode1dat[3] = {0};
volatile S16 g_mode2dat[5] = {0};
volatile U8 g_modedat = 0;

volatile U8 g_num_mov_key;//0:�α�;1:����
volatile U8 g_editflag;
volatile U8 g_reorigin; // �Զ�����ԭ������ 0�����Զ�����ԭ�� 1���Զ�����ԭ��

volatile U8 g_wifiname[21] = {0};
volatile U8 g_nameidex = 0;
volatile U8 g_ssid[20] = {0};
volatile U8 g_ssididex = 0;
volatile U8 g_password[40] = {0};
volatile U8 g_passwordidex = 0;
volatile U8 g_mac[6] = {0};
volatile U32 g_id = 0;

volatile U8 g_blkInset;
volatile U8 g_nolimt = 0;
volatile U32 g_blkInsetlen;
volatile U16 g_ctAddpins = 0;
volatile U8 g_ModToCt = 0; //ͼ���޸�תͼ����ƽ���(���β���)

volatile U8 g_NoUpdataoldno = 0; //	1-�����¾ɻ������

volatile S16 g_progParas[PROMGMAX]; //����������趨

//volatile char  usbflag;
//volatile U32 buff[4096];
extern volatile U8 g_NoReadPatf;
extern volatile U8 g_NO_ReagIOf; //����ȡIO������־
extern volatile U8 gEnverify;	 //ʹ�ܼ���У�鹦��
extern volatile U32 gverTim, gverTimes;
extern volatile U32 mp3dver;
extern volatile U16 g_Fileno;
extern volatile U8 g_runstepf;
extern volatile U8 g_init_flag;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern const U16 PARA_DATA[];
extern volatile U32 g_light_c;
extern volatile U8 g_GroupName[9];
extern volatile U8 g_GroupType;
extern volatile U8 gPeessVal;
extern BOOTSCR g_bootscr;
extern volatile U8 g_upinfo[0x20];

extern void strcopy(U8 *src, U8 *des, U16 len);
extern void BackupIREG(void);
MachineInfo g_Mac; //��еXY��������Ϣ
volatile U8 g_netSelectMac;
U8 gBatType; //������� 1������  2���ֵ�
U32 gpos, list;
volatile U8 g_usersave;								//��ǰ��¼״̬���û����
volatile U8 usepageno;								// �����ò�������ǰ�õ�ַ0��ʾ������ʱд��ַ1��д����ٻ�����ַ1��ʾ(�ڵ�ַ0���ַ1��ѭ��ʹ��)
volatile U16 pno;									//��ǰ��Ҫ��ʾ��ҳ�����
volatile U16 plastno;								//��һ����ʾҳ�����
volatile U16 pnoindex, tmppnoindex;					//���浱ǰҳ��������
volatile U16 poldno[10];							//������һ��ҳ����
volatile U16 hleppara, g_para0, g_para1, g_imgpara; //ʵ��ҳ����ת�����͵ı�־λ
volatile U32 g_temp_pno = 0xffffffff;
volatile U32 g_pno_temp = 0;
volatile U8 key_on_flag1;	   //�������ܰ������±�־
volatile U8 g_data_flash;	   //�������ݸ���
volatile U8 coldstart;		   //��������������־
volatile U8 g_UdZhouFlag = 0;  //�������־ 1:0x5a;2:0xa5;3:0x55
volatile U16 g_selectdisno;	   //��ǰѡ�еĻ������
volatile U16 g_oldselectdisno; //ǰһ��ѡ�еĻ������
volatile U8 g_uperr = 0;	   //����ʧ�ܱ�־λ
volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos, g_wabspos;
volatile S16 g_pf_absdis;
volatile U16 g_limtdang;
volatile U8 usbflag = 0;
volatile U16 g_fno = 1;				//�������
volatile U16 g_Fmax = 1;		//�����ļ�������� min��1
volatile U8 g_Funtype;			  //��������  1:���  2:�任   (��Ե��빦��)
volatile U8 g_Restorepropara = 0; //��ԭ�޸Ĳ��� 0:δ��ԭ  1:���û�ԭ��־λ  2:���û�ԭ�ɹ���־λ
volatile U16 totalfiles;
volatile U32 g_recv_netdata = 0; //�����������ݱ�־
volatile U32 g_bmplen = 0;		 //bmpͼƬ�ܴ�С
volatile U8 g_logdir = 0;		 //logo��gif��ʼ��ʾ��ַ
volatile U8 gplayLogoAvi = 0;
const unsigned int g_array1[] = {0, 0x800, 0x1000, 0x1800, 0x8800, 0x9000};
volatile U16 g_array2[307][2];
volatile S32 g_array3[2][2];  //��¼��ǰ��ľ�������-��������ϵ
volatile U16 g_array4[386][2];//��¼���Ƶ�ľ�������-��������ϵ
volatile U16 g_array5[630][2];//��¼���Ƶ�ľ�������-��������ϵ
volatile U16 g_array6[302];		// ���������޸�ʱ���浱ǰ��(�ж��ط�)����ÿ����ֵ�������
volatile S16 g_array10[630][2]; //������������ʱ���������,����ͼ�η�Χ��ʾ
//volatile S32 g_array10[630][2];	//������������ʱ���������,����ͼ�η�Χ��ʾ
//#ifdef SYSTEM2
//volatile S32 g_array10[630][2];
//#else
//volatile S16 g_array10[630][2];
//#endif
volatile S16 g_array11[13][2]; //�ݴ����߶��ط�ÿ�ص�����������ͼ����ʾ
volatile U16 g_array12[120];
volatile S32 g_array33[2][2];
volatile U16 g_temp_array2[307][2];
volatile U16 g_publicbuf[3000];// 0-256:PREG�ĵ�ǰֵ;256-512:PREG�ĵ�ǰ�޸�;600-856:PREG�޸Ĺ��Ĳ������
volatile U32 g_tmp_list[480][3]; //�������������ݲ���
volatile U8 g_changeSys;		 //�л�ϵͳ
volatile U8 g_SysErr;			 //ϵͳ����
volatile U8 g_zhouDown = 0;		 //��ͷ�½�
THDTSN g_tdtensiond;			 // ���Ӽ���������
PRFOOT g_pfootd;				 // ��ѹ�Ų���
PATHEAD g_patd;					 // �����ļ�ͷ��������
OLDPAT g_dispat;				 //��һ�룿��
PassWordPara g_user;      //Ȩ�޵�¼�ȼ�
volatile U16 g_usblogin;  //USB�����¼��Ϣ
volatile U8 usepageno;	  // �����ò�������ǰ�õ�ַ0��ʾ������ʱд��ַ1��д����ٻ�����ַ1��ʾ(�ڵ�ַ0���ַ1��ѭ��ʹ��)
volatile ERRMSG g_errMsg; // ������Ϣ��¼

void Menu_Start(void);
void Menu_DesktopInit(void);
void Menu_StReadData(void);
void CheckTouchPara(void);
void Init_SysIfo(void);
extern U8 isUpInfoData(U8 *dat);
extern volatile U8 g_vs1003err;
//------------------------------------------
// Function   : int main(void)
// Description: ������
//------------------------------------------
int main(void)
{
	U32 i;
	//	U16 flasherr = 0;
	//	U16 mp3err = 0;
	//	U8 defver[3] = {0x30,0x30,0x30};		//����ʧ��ʱ���汾������Ϊ0
	g_usbmount = 0;
	g_usblogin = 0;
	gplayLogoAvi = 0;
	g_changeSys = 0;
	g_SysErr = 0;
	system_init();

	Reg_Init();
	wdt();
	if ((RCC->CSR & 0x08000000) || (SYSDAT[0] == 0x5a))
	{ // �ϵ縴λ
	}
	else if ((RCC->CSR & 0x04000000) || (SYSDAT[0] == 0xa5))
	{ // �ⲿ��λ
	}
	else
	{ // �����ת
	}
	RCC->CSR = 0x01000000; //״̬����
	SYSDAT[0] = 0;		   //��λ��־
	SYSDAT[4] = 0;

	coldstart = 1;
	//	Read_D(D_Address[PARA]+(4)*151+3,1,(U16 *)(&P_CNR));
	//	if(P_CNR<10||P_CNR>100) P_CNR = 40;
	SetLcdPwm(1, P_CNR);

	getBootScreenMxg();
	Menu_Start();
	initflashadd(); //��ʼ��flash���ݵ����У��洢��SDRAM��

	//�����ʷ������Ϣ
	g_errMsg.max = 0;
	for (i = 0; i < ERRMSGMAX; i++)
	{
		g_errMsg.err[i] = 0xffff;
	}
	SETTFCS(); //TF��ʹ�ܱ�־λ-��λ
	CLRTFCS();	//TF��ʹ�ܱ�־λ-����

	//��̲�����ʼ��
	for (i = 0; i < PROMGMAX; i++)
	{
		g_progParas[i] = 0;
	}

#if USEMULTCOLOR //��ɫ
	g_fbmac = 1; //�۱���	 ����
#else
	Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 1, (U16 *)(&YREG[SYS_MACHINE_TYPE]));
	g_fbmac = YREG[SYS_MACHINE_TYPE] == 10040 ? 1 : 0;
#endif



	systemApp();
}

// ��ȡ�û���Ϣ
void readUsrMsg()
{
	U32 i;
	U16 checksum;
	U8 errflag = 1;
	Read_D(D_Address[USERLOG], 123, (U16 *)(&UREG)); //��ȡ�û���

	checksum = 0;
	for (i = 0; i < 123; i++)//�ܹ�246�ֽ�
	{
		if (UREG[i] != 0)
			errflag = 0; //����ȫΪ0 ���û���Ϣ����
		if (i > 2)
		{
			checksum += UREG[i];
		}
	}


	if ((checksum == UREG[2]) && (errflag == 0))
	{
		strcopy((U8 *)(&UREG), (U8 *)&(g_user), 4);//��ǰ�û�״̬
		strcopy((U8 *)(&UREG[3]), (U8 *)((U8 *)(&(g_user)) + 4), 240);//�û��������û�����
	}
	else//У�鲻ͨ����ʹ��ϵͳĬ���û�
	{ 
		g_user.newuser = 0;
		g_user.newmode = 0;
		g_user.olduser = 0;
		g_user.oldmode = 0;
		g_user.users[1].name[0] = 0x55;
		g_user.users[1].name[1] = 0x53;
		g_user.users[1].name[2] = 0x45;
		g_user.users[1].name[3] = 0x52;
		g_user.users[1].name[4] = 0x31;
		g_user.users[1].name[5] = 0x00;
		g_user.users[1].password[0] = 0x31;
		g_user.users[1].password[1] = 0x30;
		g_user.users[1].password[2] = 0x31;
		g_user.users[1].password[3] = 0x30;
		g_user.users[1].password[4] = 0x00;
		g_user.users[2].name[0] = 0x55;
		g_user.users[2].name[1] = 0x53;
		g_user.users[2].name[2] = 0x45;
		g_user.users[2].name[3] = 0x52;
		g_user.users[2].name[4] = 0x32;
		g_user.users[2].name[5] = 0x00;
		g_user.users[2].password[0] = 0x32;
		g_user.users[2].password[1] = 0x30;
		g_user.users[2].password[2] = 0x32;
		g_user.users[2].password[3] = 0x30;
		g_user.users[2].password[4] = 0x00;
		g_user.users[3].name[0] = 0x55;
		g_user.users[3].name[1] = 0x53;
		g_user.users[3].name[2] = 0x45;
		g_user.users[3].name[3] = 0x52;
		g_user.users[3].name[4] = 0x33;
		g_user.users[3].name[5] = 0x00;
		g_user.users[3].password[0] = 0x33;
		g_user.users[3].password[1] = 0x30;
		g_user.users[3].password[2] = 0x33;
		g_user.users[3].password[3] = 0x30;
		g_user.users[3].password[4] = 0x00;
		g_user.users[4].name[0] = 0x55;
		g_user.users[4].name[1] = 0x53;
		g_user.users[4].name[2] = 0x45;
		g_user.users[4].name[3] = 0x52;
		g_user.users[4].name[4] = 0x34;
		g_user.users[4].name[5] = 0x00;
		g_user.users[4].password[0] = 0x34;
		g_user.users[4].password[1] = 0x30;
		g_user.users[4].password[2] = 0x34;
		g_user.users[4].password[3] = 0x30;
		g_user.users[4].password[4] = 0x00;
		g_user.users[5].name[0] = 0x55;
		g_user.users[5].name[1] = 0x53;
		g_user.users[5].name[2] = 0x45;
		g_user.users[5].name[3] = 0x52;
		g_user.users[5].name[4] = 0x35;
		g_user.users[5].name[5] = 0x00;
		g_user.users[5].password[0] = 0x35;
		g_user.users[5].password[1] = 0x30;
		g_user.users[5].password[2] = 0x35;
		g_user.users[5].password[3] = 0x30;
		g_user.users[5].password[4] = 0x00;
		g_user.users[6].name[0] = 0x55;
		g_user.users[6].name[1] = 0x53;
		g_user.users[6].name[2] = 0x45;
		g_user.users[6].name[3] = 0x52;
		g_user.users[6].name[4] = 0x36;
		g_user.users[6].name[5] = 0x00;
		g_user.users[6].password[0] = 0x36;
		g_user.users[6].password[1] = 0x30;
		g_user.users[6].password[2] = 0x36;
		g_user.users[6].password[3] = 0x30;
		g_user.users[6].password[4] = 0x00;
		g_user.users[7].name[0] = 0x55;
		g_user.users[7].name[1] = 0x53;
		g_user.users[7].name[2] = 0x45;
		g_user.users[7].name[3] = 0x52;
		g_user.users[7].name[4] = 0x37;
		g_user.users[7].name[5] = 0x00;
		g_user.users[7].password[0] = 0x37;
		g_user.users[7].password[1] = 0x30;
		g_user.users[7].password[2] = 0x37;
		g_user.users[7].password[3] = 0x30;
		g_user.users[7].password[4] = 0x00;
		g_user.users[8].name[0] = 0x55;
		g_user.users[8].name[1] = 0x53;
		g_user.users[8].name[2] = 0x45;
		g_user.users[8].name[3] = 0x52;
		g_user.users[8].name[4] = 0x38;
		g_user.users[8].name[5] = 0x00;
		g_user.users[8].password[0] = 0x38;
		g_user.users[8].password[1] = 0x30;
		g_user.users[8].password[2] = 0x38;
		g_user.users[8].password[3] = 0x30;
		g_user.users[8].password[4] = 0x00;
		g_user.users[9].name[0] = 0x55;
		g_user.users[9].name[1] = 0x53;
		g_user.users[9].name[2] = 0x45;
		g_user.users[9].name[3] = 0x52;
		g_user.users[9].name[4] = 0x39;
		g_user.users[9].name[5] = 0x00;
		g_user.users[9].password[0] = 0x39;
		g_user.users[9].password[1] = 0x30;
		g_user.users[9].password[2] = 0x39;
		g_user.users[9].password[3] = 0x30;
		g_user.users[9].password[4] = 0x00;
	}


	if ((g_usblogin & 0xff00) == 0xa500) //USB��¼�û�
	{
		g_user.newuser = g_usblogin & 0xff;
		if (g_user.newuser > 9)
		{
			g_user.newuser = 0;
		}
		g_user.olduser = g_user.newuser;
		g_user.newmode = 0;
		g_user.oldmode = 0;
	}
	userLoninTime = 0;
}

//��ʼ��ϵͳ��Ϣ
void Init_SysIfo(void)
{
	readUsrMsg();

	//Read_D(D_Address[IOPARA],338,(U16 *)(&IOREG));		//��ȡIO����

	//		if(Write_Cmd(C_READGROUP, 0, 0)==0){		//���Ͷ�ȡ���ͼ��ָ��
	//			pno = MENU_RWHELP;hleppara = 16;
	//		}
	//		if(Read_Status()==0){
	//			pno = MENU_RWHELP;hleppara = 15;
	//		}else if(DREG[D_CMDSTATUS]!=100){
	//			#if Debug
	//			printf("DREG[D_CMDSTATUS] = %d \r\n",DREG[D_CMDSTATUS]);
	//			#endif
	//			pno = MENU_RWHELP;hleppara = 15;
	//		}
	Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //��ȡ���ͼ������

	g_GroupName[0] = GREG[GroupPNameAddr] & 0xff;
	g_GroupName[1] = (GREG[GroupPNameAddr] >> 8) & 0xff;
	g_GroupName[2] = GREG[GroupPNameAddr + 1] & 0xff;
	g_GroupName[3] = (GREG[GroupPNameAddr + 1] >> 8) & 0xff;
	g_GroupName[4] = GREG[GroupPNameAddr + 2] & 0xff;
	g_GroupName[5] = (GREG[GroupPNameAddr + 2] >> 8) & 0xff;
	g_GroupName[6] = GREG[GroupPNameAddr + 3] & 0xff;
	g_GroupName[7] = (GREG[GroupPNameAddr + 3] >> 8) & 0xff;

	g_GroupName[8] = 0;

	Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE])); //
	if (((YREG[SYS_GROUP_TYPE] > 0) && (YREG[SYS_GROUP_TYPE] < 41)) || (YREG[SYS_GROUP_TYPE] == 100))
	{
		g_GroupType = (U8)GREG[GroupPTypeAddr];
	}
	else
	{
		g_GroupType = 0;
	}
}

//���������У��
void checkProgParas(S16 *paras, S16 min, S16 max, S16 def)
{
	if ((*paras) < min || (*paras) > max)
	{
		(*paras) = def;
	}
}
//��ʼ��������趨����
void initProgParas(void)
{
	U16 i;
	ReadPromgParas((U16 *)g_progParas);
	for (i = 0; i < PROMGMAX; i++)
	{
		checkProgParas((S16 *)&g_progParas[i], PROMG_DATA[i][0], PROMG_DATA[i][1], PROMG_DATA[i][2]);
	}
}
extern volatile U32 BufAdd;

void displayBootScreen(U8 mode, U16 flag)
{

#if LCD_TYPE == LCD_7cun

	uint32_t i, j;
	uint32_t DisplayAddr = LCD_VRAM_BASE_ADDR + 1 * DISP_PAGE_SIZE;
	uint16_t *p16;
	uint32_t f;

	gplayLogoAvi = 1;
	if (mode == 3)
	{ //�û��Զ��忪������ avi
		f = Check_AviFile((char *)"logo.avi");
		if (f)
		{			  //��Ƶ��������
			mode = 1; //���ͼƬ��������
			g_bootscr.mode = 1;
		}
	}
	switch (mode)
	{
	case 0: //���⿪������
		//�޸�begin
		if (flag == 0x0)
		{
			DrawFBMP2(80, LCD_HEIGHT / 2 - 112 / 2, 130, 112, MEM_BMPLOGO_MLSTR1, Color_Red);
			DrawFBMP2(350, LCD_HEIGHT / 2 - 268 / 2, 373, 268, MEM_BMPLOGO_HSTR3, Color_Black);
		}
		else
		{
			DrawFillRect(80, LCD_HEIGHT / 2 - 112 / 2, 130, 112, Color_Red);
			DrawFillRect(350, LCD_HEIGHT / 2 - 268 / 2, 373, 268, Color_Black);
		}
		//�޸�end
		break;

	case 1: //���ͼƬ��������
		if (flag == 0x0)
		{
			DrawBMP(36, 100, 528, 300, MEM_DESKTOPBMP);
			DrawFBMP2(36, 460, 528, 100, MEM_BMPLOGO_HSTR4, Color_Red);
		}
		break;

	case 2: //�û��Զ��忪������
		if (g_bootscr.size > 0 && g_bootscr.size <= LCD_height * LCD_width * 2)
		{
			W25QXX_Read(W25Q1_DAT, (U8 *)(Bank1ADDR), W25QBSDATADDR, g_bootscr.size);
			p16 = (U16 *)(Bank1ADDR);
			for (j = 0; j < LCD_height; j++)
			{
				for (i = 0; i < LCD_width; i++)
					*((U16 *)(DisplayAddr + i * XOFFSET + j + j)) = *p16++;
			}
		}
		break;

	case 3: //�û��Զ��忪������ avi

		g_logorun = 0;

		//		SETWCS1();									//ȡ��Ƭѡ
		//		SETWCS2();									//ȡ��Ƭѡ
		//		SETWCS3();									//ȡ��Ƭѡ
		//		SETTFCS();									//ȡ��Ƭѡ
		//
		//		Tfindelay = 0;
		//		TF_Init();
		//		i =0;
		//		while((Tfindelay!=SDTFRDY) && (i<5)){
		//			TF_Init();
		//			wdt();
		//			i++;
		//		}
		//		if(i<5){


		while (1)
		{
			if (Avi_Play(0, 1, 0))
				break;
		}
		//		}

		break;
	}

	gplayLogoAvi = 0;

#endif
}

//------------------------------------------
// Function   : void Menu_Start(void)
// Description: ��������
//------------------------------------------
void Menu_Start(void)
{
	U32 i;
	U32 *padd;

	//U8 modemax;
	//U8 f;
	U8 upflag = 0;
	U8 Info[0x20] = {0};
	U8 check1 = 0, check2 = 0;
	U8 tcheck1 = 0, tcheck2 = 0;

	W25QXX_Read(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
	check1 = Info[0x1e];
	check2 = Info[0x1f];
	tcheck1 = Info[0];
	tcheck2 = Info[0];
	for (i = 1; i < 0x1e; i++)
	{
		tcheck1 += Info[i];
		tcheck2 ^= Info[i];
	}
	upflag = 1;
	if (check1 == tcheck1 && check2 == tcheck2)
	{
		if (isUpInfoData(Info))
		{
			upflag = (Info[FL_ADDR] == '0' ? 0 : 1);
		}
		else
		{
			//printf("???? err \r\n");
		}
	}
	else
	{
		//printf("check1 = %d tcheck1 = %d check2 = %d tcheck2 = %d\r\n",check1,tcheck1,check2,tcheck2);
	}
	if (upflag == 0x0)//��ȡ����/ͼƬ/�ļ�������
	{
		W25QXX_Read(W25Q1_DAT, (U8 *)(SDRAMFLASHADD + FLASHINDEXADDR + 36), FLASHINDEXADDR + 36, 12 * BINFILE_ALL); //���ļ�������
		padd = (U32 *)(SDRAMFLASHADD + FLASHINDEXADDR + 36); //ÿ����ַ32λ = 8λ * 4
		for (i = 1; i <= BINFILE_ALL; i++)
		{
			tmpflashadd[i] = (*padd) + SDRAMFLASHADD;
			padd += 3;
		}

		W25QXX_Read(W25Q1_DAT, (U8 *)MEM_DESKTOPBMP, tmpflashadd[2] - SDRAMFLASHADD, tmpflashadd[3] - tmpflashadd[2]);
		W25QXX_Read(W25Q1_DAT, (U8 *)MEM_BMPLOGO_MLSTR1, tmpflashadd[143] - SDRAMFLASHADD, tmpflashadd[144] - tmpflashadd[143]);
		W25QXX_Read(W25Q1_DAT, (U8 *)MEM_BMPLOGO_HSTR3, tmpflashadd[145] - SDRAMFLASHADD, tmpflashadd[146] - tmpflashadd[145]);
	}

	EEPREG[0] = 1;
	WritePage(1);

	CLR_Screen(COLOR_BG);
	displayBootScreen(g_bootscr.mode, upflag);

	ViewPage(1);
}

U16 *getLogoAddr(U16 machine)
{
	//	const U16* p[]={MEM_BMPLOGO_1010,MEM_BMPLOGO_1510,MEM_BMPLOGO_2010,MEM_BMPLOGO_2516,MEM_BMPLOGO_3020,MEM_BMPLOGO_4030,
	//					MEM_BMPLOGO_5040,MEM_BMPLOGO_5050,MEM_BMPLOGO_6030,MEM_BMPLOGO_6050,MEM_BMPLOGO_10050,MEM_BMPLOGO_6040,
	//					MEM_BMPLOGO_311,MEM_BMPLOGO_326,MEM_BMPLOGO_342};

	U16 *addr = MEM_BMPLOGO_1010;

	switch (machine)
	{
	case 1010:
		addr = MEM_BMPLOGO_1010;
		break;
	case 1510:
		addr = MEM_BMPLOGO_1510;
		break;
	case 2010:
		addr = MEM_BMPLOGO_2010;
		break;
	case 2516:
		addr = MEM_BMPLOGO_2516;
		break;
	case 3020:
		addr = MEM_BMPLOGO_3020;
		break;
	case 4030:
		addr = MEM_BMPLOGO_4030;
		break;
	case 5040:
		addr = MEM_BMPLOGO_5040;
		break;
	case 5050:
		addr = MEM_BMPLOGO_5050;
		break;
	case 6030:
		addr = MEM_BMPLOGO_6030;
		break;
	case 6050:
		addr = MEM_BMPLOGO_6050;
		break;
	case 10050:
		addr = MEM_BMPLOGO_10050;
		break;
	case 6040:
		addr = MEM_BMPLOGO_6040;
		break;
	case 311:
		addr = MEM_BMPLOGO_311;
		break;
	case 326:
		addr = MEM_BMPLOGO_326;
		break;
	case 342:
		addr = MEM_BMPLOGO_342;
		break;
	}
	return addr;
}
//------------------------------------------
// Function   : void Menu_DesktopInit(void)
// Description: ���滭���ʼ��
//------------------------------------------
void Menu_DesktopInit(void)
{
	WritePage(VMAXPAG);

#ifdef SYSTEM1
	//	#if DIS_LOGO==0
	//	DrawFillRect(0,0,36,32,Color_White);
	//	g_logdir = 0;
	//	DrawFBMP2(15,13,6,6,MEM_BMPLOGO_MR1,Color_Red);
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,LCD_width-36,32,Color_Navy);
	//	DrawFBMP2(36,0,92,32,getLogoAddr(YREG[SYS_MACHINE_TYPE]),Color_Gainsboro);
	//	#endif

	//	#if DIS_LOGO==1
	//	DrawFillRect(0,0,36,32,Color_White);
	//	DrawFBMP2(9,0,14,23,MEM_BMPLOGO_ML1,Color_Red);
	//	DrawFBMP2(0,23,22,9,MEM_BMPLOGO_ML2,Color_Red);
	//	DrawFBMP2(18,15,18,17,MEM_BMPLOGO_ML3,Color_Red);
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,LCD_width-36,32,Color_Navy);
	//	DrawFBMP2(36,0,92,32,MEM_BMPLOGO_MLKH,Color_Gainsboro);
	//	#endif

	//	#if DIS_LOGO==2
	//	DrawFillRect(0,0,36,32,Color_White);
	//	g_logdir = 0;
	//	DrawFBMP2(15,13,6,6,MEM_BMPLOGO_MR1,Color_Red);
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,LCD_width-36,32,Color_Navy);
	//	DrawFBMP2(36,0,92,32,getLogoAddr(YREG[SYS_MACHINE_TYPE]),Color_Gainsboro);
	//	#endif
	//
	//	if(P_KNO==4){DrawFillRect(0,32,LCD_width,LCD_height-32,Color_SkyBlue);}
	//	else if(P_KNO==3){DrawFillRect(0,32,LCD_width,LCD_height-32,Color_Gainsboro);}
	//	else if(P_KNO==2){DrawFillRect(0,32,LCD_width,LCD_height-32,Color_White);}
	//	else{DrawFillRect(0,32,LCD_width,LCD_height-32,Color_Silver);}
	//	WritePage(usepageno);
	DrawFillRect(0, 0, TITLE_XLEN, TITLE_YLEN, COLOR_TITLE_BG);

	DrawFillRect(0, 0, TIT_LOGO_XLEN, TIT_LOGO_YLEN, COLOR_TITLE_STR);
	//	#if DIS_LOGO==0
	//	g_logdir = 0;
	//	DrawFBMP2(15,13,6,6,MEM_BMPLOGO_MR1,Color_Red);
	//	#endif
	//
	//	#if DIS_LOGO==1
	//	DrawFBMP2(9,0,14,23,MEM_BMPLOGO_ML1,Color_Red);
	//	DrawFBMP2(0,23,22,9,MEM_BMPLOGO_ML2,Color_Red);
	//	DrawFBMP2(18,15,18,17,MEM_BMPLOGO_ML3,Color_Red);
	//	#endif
	if (g_bootscr.vmlk)//���Ͻ�logo
	{
		DrawFBMP2(9, 0, 14, 23, MEM_BMPLOGO_ML1, Color_Red);
		DrawFBMP2(0, 23, 22, 9, MEM_BMPLOGO_ML2, Color_Red);
		DrawFBMP2(18, 15, 18, 17, MEM_BMPLOGO_ML3, Color_Red);
	}
	else
	{
		DrawFBMP2(15, 13, 6, 6, MEM_BMPLOGO_MR1, Color_Red);
	}

	//ƽ�̱���ɫ
	DrawFillRect(0, TITLE_YLEN, LCD_WIDTH, LCD_HEIGHT - TITLE_YLEN, COLOR_BG); //����ɫ
	//#if USEMULTCOLOR
	//	if (P_KNO == 4)
	//	{
	//		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_SkyBlue);//����ɫ
	//	}
	//	else if (P_KNO == 3)
	//	{
	//		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	//	}
	//	else if (P_KNO == 2)
	//	{
	//		DrawFillRect(0, 32, LCD_width, LCD_height - 32, RGB(58, 110, 165));//��ɫ
	//	}
	//	else
	//	{
	//		DrawBMP(0, 32, LCD_width, LCD_height - 32, MEM_STARTWLBMP);//����ͼƬ��ʽ
	//	}
	//#else
	//	DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	//#endif

#endif

	// #if USEMULTCOLOR
	// 	if (P_KNO == 4)
	// 	{
	// 		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_SkyBlue);
	// 	}
	// 	else if (P_KNO == 3)
	// 	{
	// 		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	// 	}
	// 	else if (P_KNO == 2)
	// 	{
	// 		DrawFillRect(0, 32, LCD_width, LCD_height - 32, RGB(58, 110, 165));
	// 	}
	// 	else
	// 	{
	// 		DrawBMP(0, 32, LCD_width, LCD_height - 32, MEM_STARTWLBMP);
	// 	}
	// #else
	// 	DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	// #endif


	WritePage(usepageno);
}

//------------------------------------------
// Function   : void Menu_StReadData(void)
// Description: ������ȡ��������
//------------------------------------------
void Menu_StReadData(void)
{
	U8 f = 0;
	U32 i, j = 0;
	U8 re = 0;
	U16 DSP_V = DISP_VER + 4000;
	U16 FLASH_V = flashver;

	Read_D(D_Address[SYS] + SYS_SPEED, 5, (U16 *)(&YREG[SYS_SPEED])); //��ȡ�ٶȣ����㡢����ֵ
	Timer_ms = 0;
	while (1)
	{
		if (f == 0) //0����ȡ�������״̬��ϵͳ�汾
		{
			YREG[SYS_COMEN] = 0;
			i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
			if ((i) && (YREG[SYS_COMEN] == 0x5a5a))
			{
				//�޸� begin
				//Write_Cmd1(C_COMNECTION_OK,1,2);
				Write_Cmd1(C_COMNECTION_OK, 0, 0);//���ӳɹ�
				Read_D(D_Address[SYS] + SYS_SPEED1, 1, (U16 *)(&YREG[SYS_SPEED1]));
				//�޸� end
				Write_Cmd3(C_WRITE_VER, SYSSETID >> 16, SYSSETID & 0xffff, DSP_V, FLASH_V);
				Read_Status();

				f = 1;//ϵͳ���ֳɹ��������DREG��YREG�����ݽ���
			}
		}

		if (Touchfshflag == 1)
			Timer_ms = 5000;
		if ((g_data_flash == 0) && (f < 10))
		{
			if (f == 1) //1����ȡ������
			{
				i = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // ��ȡ����ֵ
				if (i == 0) //��ȡʧ�ܣ�����ΪĬ��ֵ
				{
					for (j = 0; j < 1024; j++)
						PREG[j] = PARA_DATA[j];
				}
				else
				{
				}
				if (P_R < 60)
					P_R = 60;
				f++;
			}
			if ((Timer_ms > 500) && (f == 2)) //2����ȡ��������
			{

				//��ȡ�ļ���
				for (i = 0; i < MAXIREG2; i++)
				{
					IREG2[i] = 0xffff;
				}

				if (Write_Cmd(C_READMEMNUMS, 0, 0) == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[1] = MENU_P1;
					pnoindex = 2;
					break;
				}
				if (Read_Status() == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[1] = MENU_P1;
					pnoindex = 2;
					break;
				}
				else if (DREG[D_CMDSTATUS] < 100)
				{
					pno = MENU_RWHELP;
					hleppara = 39;
					poldno[1] = MENU_P1;
					pnoindex = 2;
					break;
				}

				Read_D(D_Address[PAGEBUF], (DREG[D_CMDSTATUS] - 100), (U16 *)(&IAPBuff[0]));

				for (i = 0; i < (DREG[D_CMDSTATUS] - 100); i++)
				{
					re = Read_D(D_Address[INDEX2] + (IAPBuff[i] - MINFILENO) * FileIndexSize, FileIndexSize, (U16 *)(&IREG2[(IAPBuff[i] - MINFILENO) * FileIndexSize]));
					if (re == 0)
						break;
				}
				if (re)
				{
					BackupIREG();
					for (j = 0, i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
					{
						if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) < 0xffffffff))
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
					for (i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
					{
						IREG2[i] = 0xffff;
					}
					g_Fmax = 1;
					g_fno = 1;
					j = 0;
				}
				f++;
			}
			if ((Timer_ms > 1000) && (f == 3)) //3����ȡ������Ϣ
			{
				if (j)//��һ������������ȡ�ɹ�
				{
					i = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&HREG[0])); // ����ǰ�����ļ���Ϣ
					if (i)
					{
						j = HREG[0];
						g_Fileno = HREG[0];

						if ((j >= MINFILENO) && (j <= MAXFILENO))
						{
							i = (j - MINFILENO) * FileIndexSize;
							j = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
							i = 0; // ���������ʧ�ܣ���ʼ��SREG�Ĵ���
							if ((j >= 0x40) && (j <= MAXSREG))
							{
								i = Read_D(D_Address[PTDATA], j, (U16 *)(&SREG[0])); // ����ǰ��������
								if (i)
								{
									coldstart = 0;
									g_init_flag = 1;
									g_selectdisno = HREG[0];
									g_NoReadPatf = 1;
									g_oldselectdisno = g_selectdisno;
									g_data_flash = 1;
									for (; j < MAXSREG; j++)
									{
										SREG[j] = 0;
									}
								}
							}
						}
					}
				}
				if (g_data_flash == 0)
				{
					f = 10;
				}
			}
		}
		wdt();
		if (Timer_ms > 4800)
		{

			if (f == 0) //0: ����ͨѶ����,��Ҫ��ʼ�������Ĵ���
			{
				for (j = 0; j < 1024; j++)
					PREG[j] = PARA_DATA[j];
				for (i = 0; i < MAXIREG2; i++)
				{
					IREG2[i] = 0xffff;
				}
				for (i = 0; i < MAXSREG; i++)
					SREG[i] = 0;
				for (i = 0; i < MAXHREG; i++)
					HREG[i] = 0;
				SREG[0] = 0;	//�ļ���
				SREG[2] = 0x10; //����������ʼλ����SREG[6000]λ��
				SREG[3] = 6;
				SREG[4] = 4;	   //ÿ�����ݵ������ݳ����Ϊ80000
				SREG[12] = 0x3e8;  //������С����100mm
				SREG[14] = 0x3e8;  //������С���100mm
				SREG[16] = 0x2020; //�ļ���
				SREG[17] = 0x2020;
				SREG[18] = 0x2020;
				SREG[19] = 0x2020;
				SREG[30] = 0x3045; //�汾
				SREG[31] = 0x3536;
				SREG[0x24] = 0;

				plastno = MENU_P1;
				pnoindex = 1;
				poldno[0] = MENU_P1;
				poldno[pnoindex] = pno;
				pnoindex++;
				pno = MENU_RWHELP;
				hleppara = 60;
				//pno = MENU_RWHELP;hleppara = 60;
				//InitScreen();ViewPage(usepageno);
			}
			break;
		}
	}
}

U8 CheckTouch()
{
	U8 i, cnt = 0;
	for (i = 0; i < 10; i++)
	{
		if (Touchfshflag == 1)
			cnt++;
		DelayMs(10);
		if (cnt > 3)
			break;
	}
	if (cnt >= 3)
		return 1;
	else
		return 0;
}

//------------------------------------------
// Function   : void CheckTouchPara(void)
// Description: ������У�����
//------------------------------------------
void CheckTouchPara(void)
{
	U16 i;
	U16 vpx = 108, vpx1 = 118, vpx2 = 360;
	U8 vnum = 40;

	//	P_LAN = 1;
	if (CheckTouch() == 1)
	{
		DelayMs(60);
		if (CheckTouch() == 1)
		{
			WritePage(usepageno);
			ViewPage(usepageno);
			CLR_Screen(Color_Black);
			if (P_LAN == 0)
			{
				DrawFMultiLStr24(vpx, 120, (U16 *)("�ȴ����ֵ�����0���ɿ���,���봥��У׼!"), vnum, 0, Color_White);
			}
			else
			{
				Draw24X24FStr(vpx1, 120, "Wait the num down to 0 after releasing", Color_White);
				Draw24X24FStr(vpx1, 152, "releasing the hand into touch-screen", Color_White);
				Draw24X24FStr(vpx1, 184, "screen calibration!", Color_White);
			}
			Timer_ms = 0;
			i = 0;
			DrawLFU32(vpx2, 216, 3, 1, Color_White);
			while (Timer_ms < 3000)
			{
				wdt();
				if (i != (Timer_ms / 1000))
				{
					i++;
					DrawFillRect(vpx2, 216, 32, 32, Color_Black);
					DrawLFU32(vpx2, 216, 3 - i, 1, Color_White);
				}
				if (CheckTouch() != 1)
				{
					DelayMs(40);
					if (CheckTouch() != 1)
					{
						Timer_ms = 0;
						break;
					}
				}
			}
			if (Timer_ms >= 3000)
			{
				DrawFillRect(vpx2, 216, 32, 32, Color_Black);
				DrawLFU32(vpx2, 216, 0, 1, Color_White);
				Timer_ms = 0;
				while (Timer_ms < 10000)
				{
					wdt();
					if (CheckTouch() != 1)
					{
						DelayMs(40);
						if (CheckTouch() != 1)
						{
							InitScreen();
							//�޸� begin
							SetLanguage(MENU_TOUCHCHECK);
							Draw24x24Str(TITLE_XPOS_MIT, 4, MEM_TITLE_ADD + (TLFNADD_TOUCHSET)*TitleLengthMax, SheetPage, TLFNADD_TOUCHSET, Color_White);
							//�޸� end

							DrawFillRect(0, TITLE_YLEN, LCD_width, LCD_height - TITLE_YLEN, COLOR_BG);
							ViewPage(usepageno);
							ChkTouch(0);
							Timer_ms = 0;
							break;
						}
					}
				}
				if (Timer_ms >= 10000)
				{
					CLR_Screen(Color_Black);
					if (P_LAN == 0)
					{
						DrawFMultiLStr24(vpx, 120, (U16 *)("������������,�뼰ʱ�����������!"), vnum, 0, Color_White);
					}
					else
					{
						Draw24X24FStr(vpx1, 120, "Touchscreen detect faults,", Color_White);
						Draw24X24FStr(vpx1, 152, "please check or replace parts!", Color_White);
					}
					while (1)
					{
						wdt();
					}
				}
			}
		}
	}
}
//------------------------------------------
// Function   : void Rotary_Logo(U8 ll)
// Description: LOGO��̬��ʾ
//------------------------------------------
void Rotary_Logo(U8 ll)
{
	//�޸� begin
	if (g_bootscr.vmlk)
	{
		U8 n;
		U8 i = 0, j = 0;
		U16 color[4] = {Color_Red, RGB(231, 0, 0), RGB(207, 0, 0), RGB(191, 0, 0)};
		U16 coldat[4][3] = {{Color_Red, Color_Red, Color_Red}, {Color_Blue, Color_Yellow, Color_Green}, {Color_Yellow, Color_Green, Color_Blue}, {Color_Green, Color_Blue, Color_Yellow}};

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 3; j++)
			{
				coldat[i][j] = color[i];
			}
		}
		n = ll - 1;
		n = n % 4;
		DrawFBMP2(9, 0, 14, 23, MEM_BMPLOGO_ML1, coldat[n][0]);
		DrawFBMP2(0, 23, 22, 9, MEM_BMPLOGO_ML2, coldat[n][1]);
		DrawFBMP2(18, 15, 18, 17, MEM_BMPLOGO_ML3, coldat[n][2]);
	}
	else
	{
		DrawFillRect(0, 0, 36, 32, Color_White);
		DrawFBMP2(15, 13, 6, 6, MEM_BMPLOGO_MR1, Color_Red);
		if (ll > 1)
			DrawFBMP2(12, 10, 12, 12, MEM_BMPLOGO_MR2, Color_Red);
		if (ll > 2)
			DrawFBMP2(9, 7, 18, 18, MEM_BMPLOGO_MR3, Color_Red);
		if (ll > 3)
		{
			DrawFBMP2(6, 4, 24, 24, MEM_BMPLOGO_MR4, Color_Red);
			g_logdir = 1;
		};
	}
	//�޸� end
}

//------------------------------------------
// Function   : void DisAnimation(U8 ll)
// Description: ��ʾ���� GIFͼ-���ϴ������
//------------------------------------------
void DisAnimation(U8 ll)
{
	U8 n;
	if (ll > 99)
	{
		n = ll - 100;
		if (n > 67)
			n = 0;
		DrawFillRect(50, 166, 128, 128,Color_Gray);
		DrawFPBMPNEW(50, 166, 128, 128, MEM_BMPGIF, n);
	}
}

//-----------------------------------------------------------------------
#define KEY_SCRNSVR_BACK 1
//------------------------------------------
// Function   : void Menu_Scrnsvr(U8 tt)
// Description: ��������
//------------------------------------------
void Menu_Scrnsvr(U8 tt)
{
	U8 beeponf = 0;
	U32 keyon = 0, keyoff = 0, keydata = 0;

	const U8 keynum = {2};
#if LCD_TYPE == LCD_5cun
	const KEYPAD keyd[] = {{0, 0, 0, 0}, {1, 415, KEYX1, KEYY1}};
#endif
#if LCD_TYPE == LCD_7cun
	const KEYPAD keyd[] = {{0, 0, 0, 0}, {1, 415, KEYX1, KEYY1}};
#endif
#if LCD_TYPE == LCD_8cun
	const KEYPAD keyd[] = {{0, 0, 0, 0}, {1, 735, KEYX1, KEYY1}};
#endif

	InitScreen();
	Draw24X24FStr(TITLE_XPOS, 4, "����", Color_White);
	DrawRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, Color_White);
	Draw24X24FStr(keyd[1].xpis + 7, keyd[1].ypis + 20, "����", Color_White);

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
		else if (keydata)
		{
			if (keyon == 0)
			{
				keyon = keydata;
				DrawRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, Color_Blue);
				Draw24X24FStr(keyd[1].xpis + 7, keyd[1].ypis + 20, "����", Color_Blue);
				switch (keydata)
				{
				case KEY_SCRNSVR_BACK:
					break;
				}
				Times = 0;
			}
			else if (keyon != keydata)
			{
				keydata = 0;
			}
			if (Times > 600)
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (P_KBC == 0)
						Beep_On(70);
				}
			}
		}
		else
		{
		}
		if (keyoff)
		{
			switch (keyoff)
			{
			case KEY_SCRNSVR_BACK:
				pno = plastno;
				break;
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0))
		{
			DrawRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, Color_White);
			Draw24X24FStr(keyd[1].xpis + 7, keyd[1].ypis + 20, "����", Color_White);
			switch (keyon)
			{
			case KEY_SCRNSVR_BACK:
				break;
			}
			keyon = 0;
			beeponf = 0;
		}

		if (pno != MENU_SCRNSVR)
		{
			plastno = MENU_SCRNSVR;
			break;
		}
		wdt();
	}
}
