//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  register.c								*
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

#include <absacc.h>
#include "register.h"
#include "menu.h"
#include "main.h"
#include "patdata.h"
#include "touch.h"
#include "ds2460.h"

//SDRAM:H57V2562GTR		16M x 16bit		0xc0000000 - 0xc1ffffff		���ĸ���
//bank0:0xc0000000 - 0xc07fffff		0x400000 x 16bit				�Դ�����
//bank1:0xc0800000 - 0xc0ffffff		0x400000 x 16bit
//bank2:0xc1000000 - 0xc17fffff		0x400000 x 16bit				��ͨѶ�üĴ���
//bank3:0xc1800000 - 0xc1ffffff		0x400000 x 16bit
//�Դ� SDRAM
//bank2
volatile U16 EEPREG[MAXEEPREG] __attribute__((at(ADDEEPREG))); //0xc1000000
volatile U16 DREG[MAXDREG] __attribute__((at(ADDDREG)));	   // ״̬����							9		c1000000
volatile U16 EREG[MAXEREG] __attribute__((at(ADDEREG)));	   // ������Ϣ							13		c1000080
volatile U16 MREG[MAXMREG] __attribute__((at(ADDMREG)));	   // ϵͳ������Ϣ����			14		c1000280
volatile U16 YREG[MAXYREG] __attribute__((at(ADDYREG)));	   // ϵͳ����							15		c1000400
volatile U16 PREG[MAXPREG] __attribute__((at(ADDPREG)));	   // ��������							16		c1000580
//volatile U16 IREG[MAXIREG] __attribute__((at(ADDIREG)));			// �����ļ�������					17		c1000d80
volatile U16 HREG[MAXHREG] __attribute__((at(ADDHREG)));		   // ��ǰ�����ļ���Ϣ					18		c1002680
volatile U16 SREG[MAXSREG] __attribute__((at(ADDSREG)));		   // ��������							19		c1002700
volatile U16 IOREG[MAXIOREG] __attribute__((at(ADDIOREG)));		   // IO��������						20		c102f608
volatile U16 TMP_SREG[MAXTSREG] __attribute__((at(ADDTSREG)));	   // TMP_SREG							21		c102f8a8
volatile U16 IAPBuff[MAXTIAPBUFF] __attribute__((at(ADDIAPBUFF))); //									22		c10394E8
volatile U16 IREG2[MAXIREG2] __attribute__((at(ADDIREG2)));		   // �����ļ�������2					23
//volatile U16 DIR_DREG2[MAXDIR2] __attribute__((at(ADDDIR_DREG2)));	// �ļ�Ŀ¼2������(������) 2560*2
volatile U16 NEWSREG[MAXSREG2] __attribute__((at(ADDNEWSREG))); // ��ת��Ϊ�ֵܿ������			19		c1002700
volatile S16 PATCRRRD[MAXCOORD][2] __at(ADDPATCRRRD);			// �ݴ��»����������껺��
volatile S16 PATCRRRD1[MAXCOORD][2] __at(ADDPATCRRRD1);			// �ݴ��»����������껺��
volatile U16 INPOINTPOS[MAXCOORD] __at(ADDRINFPOINT);			// �յ�λ��
volatile U16 UREG[MAXUREG] __at(ADDUREG);						// �û���Ϣ
volatile U16 VREG[MAXVREG] __at(ADDVREG);						// �ŷ�����
volatile U16 GREG[MAXGREG] __at(ADDGREG);						// ���ͼ������
volatile U16 LREG[MAXLREG] __at(ADDLREG);						// ���Ӽ���������
//bank1
volatile U8 AVI_VIDEO_BUF[AVI_VIDEO_BUF_SIZE] __attribute__((at(ADDAVIVIDEO))); // AVI��Ƶ������
volatile U8 AVI_AUDIO_BUF[AVI_AUDIO_BUF_SIZE] __attribute__((at(ADDAVIAUDIO))); // AVI��Ƶ������

volatile U16 BLKSREG[MAXBLKSREG] __attribute__((at(ADDBLKSREG))); // ���ݻ������ݻ�������
volatile U16 BLKHREG[MAXBLKHREG] __attribute__((at(ADDBLKHREG))); // ���ݻ����ļ���Ϣ
volatile U16 BLKIREG[MAXBLKIREG] __attribute__((at(ADDBLKIREG))); // ���ݻ����ļ�������Ϣ

volatile U16 BLKIND[MAXBLIND] __attribute__((at(ADDBLKINDEX))); // �������ݻ�������
volatile U16 BLKBUF[MAXBLKBUF] __attribute__((at(ADDBLKBUF)));	// �������ݻ���

//volatile U16 WAVEREG[MAXWAVE] __attribute__((at(ADDWAVE)));	// ���β鿴���ݻ���

volatile U16 WREG[MAXWREG]; // 2013.10.25

extern volatile U32 FLASHADD_P;
extern U8 gBatType; //������� 1������  2���ֵ�
extern volatile U8 g_usersave;

const U16 PARA_DATA[];
const U16 PARA_INSET[];
const U16 PARA_OUTSET[];
//------------------------------------------
// Function   : void Reg_Init(void)
// Description: ������ʼ��
//------------------------------------------
void Reg_Init(void)
{
	U32 i;

	for (i = 0; i < MAXDREG; i++)
		DREG[i] = 0;
	for (i = 0; i < MAXMREG; i++)
		MREG[i] = 0;
	for (i = 0; i < MAXSREG; i++)
		SREG[i] = 0;

	//	padd = (U16 *)(PAT_DATA);
	for (i = 0; i < 1024; i++)
		PREG[i] = PARA_DATA[i];
	//	n = sizeof(PAT_DATA);n >>= 1;
	//	for(i=0;i<n;i++){SREG[i] = *padd;padd++;}
	//	for(;i<MAXSREG;i++){SREG[i] = 0;}
	//	for(i=0;i<72;i++){IOREG2[i+INSETREGI] = PARA_INSET[i];}
	//	for(i=0;i<132;i++){IOREG[i+OUTSETREGI] = PARA_OUTSET[i];}
	//	for(i=0;i<168;i++){IOREG[i+IOSETREGI] = IOREG[i+INSETREGI];}
	for (i = 0; i < MAXMREG; i++)
	{
		MREG[i] = 0;
	}
	for (i = 0; i < MAXEREG; i++)
	{
		EREG[i] = 0;
	}
	ReadTouchPara(); // ��ȡ������У׼����
	ReadSysid();	 // ������������

	FLASHADD_P = SST_SREG_SADD;

	YREG[SYS_STATUS] = 0;
	YREG[SYS_ENABLE] = 0;
	YREG[SYS_UPCOUNT] = 0;
	YREG[SYS_DNCOUNT] = 0;

	YREG[SYS_INPUTH] = 0;
	YREG[SYS_INPUTL] = 0;
	YREG[SYS_OUTPUTH] = 0;
	YREG[SYS_OUTPUTL] = 0;

	g_SysTimes.year = 16;
	g_SysTimes.month = 1;
	g_SysTimes.day = 1;
	g_SysTimes.hour = 0;
	g_SysTimes.minute = 0;
	YREG[SYS_MS] = 0;
	YREG[SYS_WH] = 0;
	YREG[SYS_MD] = 0x101;
	YREG[SYS_Y] = 0x16;

	P_CNR = ReadBackLight();
	if ((P_CNR < 10) || (P_CNR > 100))
		P_CNR = 40;

	gBatType = ReadBatType();  //��ȡ�ϴδ������
	g_usersave = ReadUserNo(); //��ȡ�ϴ��û����
	//	ReadSeed();		//������һ�α�����������		???�޴˺���
}

const U16 PARA_DATA[] = {
	// 227*4= 908
	// ������()
	0, 1, 1, 1,			   //	P_WIP	PREG[0]			// ���ߵ�ON/OFF													ON(1)
	0, 998, 30, 30,		   //	P_W1	PREG[1]			// �������(W)�Ŀ�ʼ���ʱ��		msec		0		998		30
	0, 998, 40, 40,		   //	P_W2	PREG[2]			// �������(W)�����ʱ��			msec		0		998		40
						   // �������(Slow start)
	0, 2299, 200, 200,	   //	P_S		PREG[3]			// ��������ٶ�						s/min		0		2299	200
	0, 1, 0, 0,			   //	P_SL	PREG[4]			// ���������趨												OF(0)
	0, 5, 2, 2,			   //	P_SLN	PREG[5]			// ������������					sti.		0		5		2
						   // ��ѹ��(Clamp)
	0, 1, 0, 0,			   //	P_OST	PREG[6]			// ��ѹ��û����Ҳ��/���ɶ�ת									OF(0)
	0, 1, 0, 0,			   //	P_RPT	PREG[7]			// �ظ����Ƶ��趨												OF(0)
	0, 1, 1, 1,			   //	P_RP1	PREG[8]			// ѹ��1���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP2	PREG[9]			// ѹ��2���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP3	PREG[10]		// ѹ��3���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP4	PREG[11]		// ѹ��4���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP5	PREG[12]		// ѹ��5���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP6	PREG[13]		// ѹ��6���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP7	PREG[14]		// ѹ��7���ظ������趨											ON(1)
	0, 1, 1, 1,			   //	P_RP8	PREG[15]		// ѹ��8���ظ������趨											ON(1)
	0, 1, 0, 0,			   //	P_WHY	PREG[16]		// ѹ�Ŷ���������˳���趨										OF(0)
	0, 1, 0, 0,			   //	P_FSR	PREG[17]		// ѹ�Ų��������Ľ��											OF(0)
	0, 1, 0, 0,			   //	P_1PD	PREG[18]		// ��̤��������趨												OF(0)
	0, 498, 0, 0,		   //	P_1T	PREG[19]		// ��̤��������ӳ�ʱ���趨			x10msec		0		498		0
	0, 999, 300, 300,	   //	P_1A	PREG[20]		// ѹ��1�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_2A	PREG[21]		// ѹ��2�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_3A	PREG[22]		// ѹ��3�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_4A	PREG[23]		// ѹ��4�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_5A	PREG[24]		// ѹ��5�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_6A	PREG[25]		// ѹ��6�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_7A	PREG[26]		// ѹ��7�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_8A	PREG[27]		// ѹ��8�ӳ����ONʱ���趨			x10msec		0		999		0
	0, 999, 300, 300,	   //	P_1B	PREG[28]		// ѹ��1�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_2B	PREG[29]		// ѹ��2�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_3B	PREG[30]		// ѹ��3�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_4B	PREG[31]		// ѹ��4�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_5B	PREG[32]		// ѹ��5�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_6B	PREG[33]		// ѹ��6�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_7B	PREG[34]		// ѹ��7�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 999, 0, 0,		   //	P_8B	PREG[35]		// ѹ��8�ӳ����OFFʱ���趨			x10msec		0		999		0
	0, 2, 0, 0,			   //	P_OFB	PREG[36]		// ѹ�Ŷ����ָ��������趨										NO(0)
	1, 8, 1, 1,			   //	P_FN	PREG[37]		// ��Чѹ�Ŷ����������趨						1		8		1
	0, 1, 0, 0,			   //	P_CF	PREG[38]		// ѹ�Ŷ����������趨											OF(0)
	1, 4, 1, 1,			   //	P_F21N	PREG[39]		// ѹ�Ŷ����ָ�ʱ��Чѹ����OF1-OF4				1		4		1
	1, 4, 1, 1,			   //	P_F22N	PREG[40]		// ѹ�Ŷ����ָ�ʱ��Чѹ����OF5-OF8				1		4		1
	0, 1, 0, 0,			   //	P_CF1	PREG[41]		// ѹ�Ŷ����ָ�ʱ�������趨OF1-OF4								OF(0)
	0, 1, 0, 0,			   //	P_CF2	PREG[42]		// ѹ�Ŷ����ָ�ʱ�������趨OF5-OF8								OF(0)
	1, 4, 1, 1,			   //	P_F4BN	PREG[43]		// ��Чѹ�ŷ����������趨						1		4		1
	1, 4, 1, 1,			   //	P_F4SN	PREG[44]		// ���鲽���������趨							1		4		1
	0, 1, 0, 0,			   //	P_AF2	PREG[45]		// ��ѹ���ζ���ѹ�ŵ�ѡ��										OF(0)
	0, 1, 0, 0,			   //	P_OPR	PREG[46]		// ������������ѹ��λ������										OF(0)
						   // ���Ʒ�Χ(Sewing area)
	0, 1, 0, 0,			   //	P_ALC	PREG[47]		// ���Ʒ�Χ���ƵĽ��											OF(0)
	0, 205, 100, 100,	   //	P_XL	PREG[48]		// X����෶Χ���Ƶ��趨			mm			0		254		101
	0, 205, 100, 100,	   //	P_XR	PREG[49]		// X���Ҳ෶Χ���Ƶ��趨			mm			0		254		101
	0, 105, 50, 50,		   //	P_YU	PREG[50]		// Y���෶Χ���Ƶ��趨			mm			0		254		51
	0, 105, 50, 50,		   //	P_YD	PREG[51]		// Y��ǰ�෶Χ���Ƶ��趨			mm			0		254		51
						   // ��λ��(Needle position)
	0, 1, 0, 0,			   //	P_RU	PREG[52]		// ���ߺ�����ת��������											OF(0)
	0, 360, 30, 30,		   //	P_R8	PREG[53]		// RU����ת�Ƕȵ��趨				deg.		0		360		30
	0, 360, 14, 14,		   //	P_U8	PREG[54]		// ����ͣλ�õ�ֹͣ�Ƕ�				deg.		0		360		14
	0, 360, 50, 50,		   //	P_D8	PREG[55]		// ����ͣλ�õ�ֹͣ�Ƕ�				deg.		0		360		50
						   // ���߼�֪(Thread trimming sensor)
	0, 1, 0, 0,			   //	P_S1	PREG[56]		// ���߼�����Ч/��Ч
	0, 15, 3, 3,		   //	P_S2	PREG[57]		// ������߼�����Ч����
	0, 99, 2, 2,		   //	P_S3	PREG[58]		// ����;�ж��߼���ԪЧ����
	0, 2499, 300, 300,	   //	P_B		PREG[59]		// ���߼�⶯���ٶȵ��趨
	0, 1, 0, 0,			   //	P_TST	PREG[60]		// ���߼��ʱ��������/��
						   // ԭ��(Home position)
	0, 3, 0, 0,			   //	P_HPM	PREG[61]		// ��;��ͣʱ��ԭ�㸴�鷽��										--(0) --/JS/JE/JC
	0, 1, 1, 1,			   //	P_HPC	PREG[62]		// �򿪵�Դʱԭ�㸴�鶯���Ľ�ֹ									ON(1)
	0, 1, 1, 1,			   //	P_HPF	PREG[63]		// �Զ�ԭ�㸴�鶯���Ľ�ֹ										OF(0)		�Ĺ� �����ⲻͬ
	0, 3, 0, 0,			   //	P_HPK	PREG[64]		// ��������趨													--(0) --/2/2L/NO
	0, 2, 0, 0,			   //	P_2HS	PREG[65]		// �����2ԭ��ʱֹͣ�ķ���										ST(0) ST/SW/NS
	0, 1, 0, 0,			   //	P_FHP	PREG[66]		// ����ԭ���ʹ��												OF(0)
	0, 1, 1, 1,			   //	P_CHS	PREG[67]		// �������϶���ʱ��ԭ���a��ѡ��									ON(1)
	0, 1, 0, 0,			   //	P_SHX	PREG[68]		// X�᷶Χ���Ƶ�ԭ�㸴��										OF(0)
	0, 1, 0, 0,			   //	P_SHY	PREG[69]		// Y�᷶Χ���Ƶ�ԭ�㸴��										OF(0)
	0, 1, 0, 0,			   //	P_HPS	PREG[70]		// �ϵ�ʱԭ�㸴��ʱ����,��ѹ��״̬���趨						OF(0)
						   // ��;��ͣ(Halt)
	0, 1, 0, 0,			   //	P_STF	PREG[71]		// ������ͣ��ť����ʱ���͵�ֹͣ״̬								CP(0) CP/ST
	0, 2, 1, 1,			   //	P_STN	PREG[72]		// ������ͣ��ť����ʱ���ֹͣ״̬								UP(1) DN/UP/RN
	0, 2, 0, 0,			   //	P_STS	PREG[73]		// ������ͣ��ť����ʱ��������ı���								AL(0) AL/FU/OF
	0, 2, 2, 2,			   //	P_ST2	PREG[74]		// ���ڶ��ν�����ͣ��ťʱ�Ķ���									ST(2) UT/UP/ST
	0, 1, 0, 0,			   //	P_STP	PREG[75]		// ��������ͣ��ťʱ��Դ�Ĳ���									--(0) --/PD
	0, 1, 1, 1,			   //	P_STD	PREG[76]		// ����;��ͣ����ֹͣʱ��ѹ�ŵĶ���								FU(0) FU/FD	  �Ĺ� �����ⲻͬ
	0, 1, 1, 1,			   //	P_STL	PREG[77]		// ��;��ͣ�������ʱͬλ�õķ�ֹ								ON(1)
						   // ����(Counter)
	0, 3, 1, 1,			   //	P_CUP	PREG[78]		// ����UP��������												ED(1) --/ED/ST/CY
	0, 3, 1, 1,			   //	P_CDN	PREG[79]		// ����DN��������												ED(1) --/ED/ST/CY
	0, 3, 0, 0,			   //	P_UCM	PREG[80]		// ����UP�����趨ֵ����������									--(0) --/RE/IT/CL
	0, 3, 0, 0,			   //	P_DCM	PREG[81]		// ����UP�����趨ֵ����������									--(0) --/RE/IT/CL
	0, 1, 0, 0,			   //	P_PCM	PREG[82]		// �ؿ���Դʱ����ֵ����������									--(0) --/IT
	0, 1, 0, 0,			   //	P_CN	PREG[83]		// ���������㹦��												OF(0)
	0, 1, 0, 0,			   //	P_UCC	PREG[84]		// ����UP����ֵ��������ֹ										OF(0)
	0, 1, 0, 0,			   //	P_DCC	PREG[85]		// ����DN����ֵ��������ֹ										OF(0)
	0, 1, 0, 0,			   //	P_USC	PREG[86]		// ������UP����ֵ��������ҵ���趨								OF(0)
	0, 1, 0, 0,			   //	P_DSC	PREG[87]		// ������DN����ֵ��������ҵ���趨								OF(0)
						   // ɲ��(Brake)
	0, 1, 0, 0,			   //	P_WBK	PREG[88]		// ΢��ɲ�����趨												OF(0)
	0, 1, 0, 0,			   //	P_BKM	PREG[89]		// ΢��ɲ���Ĺ����趨											E(0)  E/H
	0, 1, 0, 0,			   //	P_BNR	PREG[90]		// ΢��ɲ���������趨											OF(0)
	0, 99, 14, 14,		   //	P_BKT	PREG[91]		// ɲ��ʱ����趨					x10msec		0		99		14
	0, 255, 50, 50,		   //	P_PW	PREG[92]		// ��ѹ�Ŷ���ʱ��΢��ɲ��ʱ��		msec		0		254		50
						   // ��ѹ��(Presser foot)
	0, 498, 2, 2,		   //	P_PD	PREG[93]		// ��ѹ��ʱ��1(�½�ʱ)���趨		x10msec		0		498		36
	0, 498, 2, 2,		   //	P_PU	PREG[94]		// ��ѹ��ʱ��2(����ʱ)���趨		x10msec		0		498		16
	0, 498, 10, 10,		   //	P_UW	PREG[95]		// ��ѹ��ʱ��3(����ʱ)���趨		x10msec		0		498		10
	0, 1, 0, 0,			   //	P_BPF	PREG[96]		// �ٷ��Ŀ����ƶ�ʱ��ѹ�ŵ��趨								OF(0)
	0, 150, 150, 150,	   //  P_PLP   PREG[97]		// ��ѹ���г��趨
	0, 360, 100, 100,	   //	P_ZU8	PREG[98]		// ��ѹ�������Ƕ�
	0, 360, 0, 0,		   //	P_Zd8	PREG[99]		// ��ѹ���Ͻ��Ƕ�
	0, 1, 1, 1,			   //	P_ZTM	PREG[100]		// ����ʱ��ѹ��ͬ���趨
						   // ����(Bobbin winding)
	200, 3000, 1500, 1500, //	P_W		PREG[101]		// �����ٶȵ��趨					s/min		0		2299	1500
	0, 2, 0, 0,			   //	P_WSM	PREG[102]		// ���߶����Ĳ����趨											NO(0)
	2, 498, 30, 30,		   //	P_WT	PREG[103]		// ���ߵĶ���ʱ��					sec			1		498		30
						   // �ƶ���ʽ(���Ϸ�ʽ)(Feed method)
	2, 2, 0, 0,			   //	P_WET	PREG[104]		// ��ѹ�ŵ�����ѡ��												L(0)  L/M/H
	0, 99, 20, 20,		   //	P_WEL	PREG[105]		// ��ѹ�ŵ�����(WET=L)ʱ���趨ֵ				0		99		20
	0, 99, 26, 26,		   //	P_WEM	PREG[106]		// ��ѹ�ŵ�����(WET=M)ʱ���趨ֵ				0		99		26
	0, 99, 35, 35,		   //	P_WEH	PREG[107]		// ��ѹ�ŵ�����(WET=H)ʱ���趨ֵ				0		99		35
	0, 2, 0, 0,			   //	P_THI	PREG[108]		// �����ȵ�ѡ��												L(0)  L/M/H
	0, 60, 0, 0,		   //	P_TL	PREG[109]		// ������(THI=L)ʱ���趨ֵ					0		60		0
	0, 60, 32, 32,		   //	P_TM	PREG[110]		// ������(THI=M)ʱ���趨ֵ					0		60		32
	0, 60, 44, 44,		   //	P_TH	PREG[111]		// ������(THI=H)ʱ���趨ֵ					0		60		44
	0, 2, 1, 1,			   //	P_FED	PREG[112]		// �Ͳ���ʽ���趨												ID(1) IT/ID/S
	0, 1, 1, 1,			   //	P_FEM	PREG[113]		// ���Ͷ�����ѡ��												S(1)  K/S
	0, 9, 4, 4,			   //	P_FES	PREG[114]		// ƽ˳���͵��趨								0		9		9
	0, 1, 1, 1,			   //	P_FSM	PREG[115]		// ƽ˳���͵��趨												ON(1)
	1, 15, 2, 2,		   //	P_TYA	PREG[116]		// ���ʱ��ӡ����ƶ���(����)���趨	time		1		15		1
						   // �����ٶ�(Sewing speed)
	0, 3000, 2800, 2800,   //	P_H		PREG[117]		// ����ٶ�							s/min		0		2299	2299
	0, 500, 200, 200,	   //	P_L		PREG[118]		// ����ٶ�							s/min		0		399		200
	0, 3000, 1500, 1500,   //	P_M		PREG[119]		// �и��ٶ�MD1						s/min		0		2299	1500
	0, 3000, 800, 800,	   //	P_N		PREG[120]		// �е��ٶ�MD2						s/min		0		2299	800
						   // ����ʱ��(Thread trimming timing)
	0, 4, 0, 0,			   //	P_LTM	PREG[121]		// �������(T)���������										T1(0) T1/T2/T3/T4/T5
	0, 4, 0, 0,			   //	P_LLM	PREG[122]		// �������(L)���������										L1(0) L1/L2/L3/L4/L5
	0, 360, 120, 120,	   //	P_T1	PREG[123]		// �������(T)��ʼ�����ʱ��/�Ƕ�	msec		0		998		50
	0, 360, 240, 240,	   //	P_T2	PREG[124]		// �������(T)�����ʱ��/�����Ƕ�	msec		0		998		0
	0, 998, 196, 196,	   //	P_L1	PREG[125]		// �������(L)��ʼ�����ʱ��/�Ƕ�	msec		0		998		196
	0, 998, 0, 0,		   //	P_L2	PREG[126]		// �������(L)�����ʱ��/�����Ƕ�	msec		0		998		0
	0, 399, 200, 200,	   //	P_T		PREG[127]		// �����ٶ�							s/min		0		399		200
	0, 1, 0, 0,			   //	P_ETC	PREG[128]		// ����������ʹ������ʱ������߶���								OF(0)
	0, 1, 1, 1,			   //	P_TRM	PREG[129]		// ���߶�����ON/OFF												ON(1)
	0, 998, 30, 30,		   //	P_LP	PREG[130]		// Setting for gension release outputs time length at presser foot rise		x10msec		0		998		0
	0, 1, 0, 0,			   //	P_LSP	PREG[131]		// һ����������ŷ���ʹ�����		x10msec		0		998		0
						   // ����(����)(Step)
	0, 1, 0, 0,			   //	P_STE	PREG[132]		// �趨����ת��ʱ�Ĳ���Ҫ��ɾ���趨								OF(0)
	0, 1, 0, 0,			   //	P_STO	PREG[133]		// ���趯����ON/OFF												OF(0)
	0, 359, 0, 0,		   //	P_SUU	PREG[134]		// ���Ͷȵ�ִ������					line		1		192		2
	0, 1, 0, 0,			   //	P_SP1	PREG[135]		// �������з�ʽ �汾1				line		1		192		2
	0, 1, 0, 0,			   //	P_SKF	PREG[136]		// ��IOF����ʱ,������ȫ��ֹ		line		1		192		2
						   // �ƶ�(�綯)(Jog)
	0, 2, 0, 0,			   //	P_JGM	PREG[137]		// �����Ĺ����趨												--(0) --/AL/HA
	0, 1, 0, 0,			   //	P_UJC	PREG[138]		// ��ͣ����ͣλ������ʱ�Ĵ綯�����趨							OF(0)
	0, 1, 0, 0,			   //	P_JGS	PREG[139]		// ��������ʱ��ͣ����Ĵ���							OF(0)
						   // �Ͳ��Ƕ�(Feed angle)
	20, 198, 94, 94,	   //	P_S8	PREG[140]		// ��ʼ�Ͳ��ĽǶ��趨				deg.		25		198		94
	200, 356, 344, 344,	   //	P_E8	PREG[141]		// ֹͣ�Ͳ��ĽǶ��趨				deg.		100		356		344
	300, 356, 324, 324,	   //	P_T8	PREG[142]		// ����ʱֹͣ�Ͳ��ĽǶ��趨			deg.		150		356		324
	0, 360, 0, 0,		   //	P_8D	PREG[143]		// ��ʾ����ͣλ�õĽǶ�				deg.
						   // Һ����ʾ��(LCD panel)
	0, 1, 0, 0,			   //	P_KBC	PREG[144]		// ��������������												OF(0)
	0, 1, 1, 1,			   //	P_LCD	PREG[145]		// �����������													ON(1)
	0, 1, 1, 1,			   //	P_LOF	PREG[146]		// �������ʡ�繦��												ON(1)
	0, 1, 1, 1,			   //	P_LNN	PREG[147]		// ���������ʾ��ɫ												ON(1)
	0, 1, 1, 1,			   //	P_LHP	PREG[148]		// ����˵������ʾ												OF(1)
	1, 3, 1, 1,			   //	P_KNO	PREG[149]		// ��׼ģ��ı���ѡ��							1		3		1
	0, 10, 0, 4,		   //	P_CTR	PREG[150]		// ���������Ĵ���ѡ��							0		10		0
	0, 99, 50, 50,		   //	P_CNR	PREG[151]		// ���������ĶԱ�								0		100		50
	60, 540, 120, 120,	   //	P_R		PREG[152]		// �������ʡ������ʱ��				/60min		0		9998	300
						   // �����ź�(Input signal)
	0, 1, 0, 0,			   //	P_I1	PREG[153]		// I1����Ĺ���ѡ��
	0, 1, 0, 0,			   //	P_I1L	PREG[154]		// I1����ĵ�ƽת��
	0, 1, 0, 0,			   //	P_I1M	PREG[155]		// I1����Ķ���ѡ��
						   // ����ź�(Output signal)
	0, 1, 0, 0,			   //	P_O1	PREG[156]		// O1����Ĺ���ѡ��
	0, 1, 0, 0,			   //	P_O1L	PREG[157]		// O1����ĵ�ƽת��
	0, 1, 0, 0,			   //	P_O1C	PREG[158]		// O1����Ķ�������
	0, 1, 0, 0,			   //	P_O1M	PREG[159]		// O1����Ķ���ѡ��1
	0, 1, 0, 0,			   //	P_O1D	PREG[160]		// O1�����ON�ӳ��趨
	0, 1, 0, 0,			   //	P_O1E	PREG[161]		// O1����Ķ���ѡ��2
	0, 1, 0, 0,			   //	P_O_1A	PREG[162]		// O1�����ON���ӳ�ʱ��/���������OFFʱ���趨
	0, 1, 0, 0,			   //	P_O_1B	PREG[163]		// O1�����ONʱ��/������ONʱ���趨
	0, 1, 0, 0,			   //	P_O_1T	PREG[164]		// O1�����ȫ�����ʱ���趨
	0, 1, 0, 0,			   //	P_O_1C	PREG[165]		// O1����Ķ���Ч�ʵ��趨
						   // ����(Other)
	0, 2, 0, 0,			   //	P_GA	PREG[166]		// ��/�ͷŴ����趨											L(0) L/LL/H
	0, 1, 0, 0,			   //	P_MIL	PREG[167]		// ϳ�����ܵ�ON/OFF												OF(0)
	0, 1, 0, 0,			   //	P_APC	PREG[168]		// ������̨������ָ���������Ϲ���								OF(0)
	0, 1, 0, 0,			   //	P_POF	PREG[169]		// ������̨������ָ����������ʱ�Ĳ���ֵ							6(0) 6/1
	0, 1, 0, 0,			   //	P_ASC	PREG[170]		// ���ⲿ������ָ������ٶ����ƵĹ���							OF(0)
	0, 1, 0, 0,			   //	P_AER	PREG[171]		// �������ϳ�������												OF(0)
	0, 1, 0, 0,			   //	P_PFL	PREG[172]		// �ڲ��������ֹ��дɾ���趨									OF(0)
	1, 127, 43, 43,		   //	P_RN	PREG[173]		// Setting of length of continuous feed sew in maximum	/10mm	1	127	43
	0, 127, 0, 0,		   //	P_LH	PREG[174]		// �����Ͳ�ʱ������Ŀ����
	0, 1, 0, 0,			   //	P_LCK	PREG[175]		// ��ֹ�п�������趨���ת��ģ��
	0, 1, 0, 0,			   //	P_PTC	PREG[176]		// �ӷ������ϱ���趨����ı��									OF(0)
	0, 1, 0, 0,			   //	P_SCP	PREG[177]		// ��ݹ��ܼ��Ľ���												OF(0)
	0, 1, 0, 0,			   //	P_DFR	PREG[178]		// �Ӵŵ�Ƭ����������Ҫ��д����ӵķ���							OF(0)
	0, 998, 0, 0,		   //	P_RT	PREG[179]		// �趨�����������ʱ����							OF(0)
	0, 1, 0, 0,			   //	P_SPC	PREG[180]		// ����ͨѶ							OF(0)
	0, 1, 0, 0,			   //	P_APT	PREG[181]		// ͼ��ʱ������Ӳ�ѡ��							OF(0)
	0, 2, 0, 0,			   //	P_LAN	PREG[182]		// ����ѡ��							OF(0)
	0, 1, 0, 0,			   //	P_VOI	PREG[183]		// �����趨							OF(0)
	0, 1, 0, 0,			   //	P_E_1	PREG[184]		// ������Ϣ1
	0, 1, 0, 0,			   //	P_E_2	PREG[185]		// ������Ϣ2
	0, 1, 0, 0,			   //	P_E_3	PREG[186]		// ������Ϣ3
	0, 1, 0, 0,			   //	P_E_4	PREG[187]		// ������Ϣ4
	0, 1, 0, 0,			   //	P_E_I1	PREG[188]		// I1�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_UP	PREG[189]		// UP�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_DN	PREG[190]		// DN�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_XH	PREG[191]		// XH�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_YH	PREG[192]		// YH�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_ECA	PREG[193]		// ECA�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_ECB	PREG[194]		// ECB�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_CRS	PREG[195]		// CRS�����ź���ʾ
	0, 1, 0, 0,			   //	P_E_O1O	PREG[196]		// ���
	0, 1, 0, 0,			   //	P_E_O1D	PREG[197]		// ���
	0, 1, 0, 0,			   //	P_E_P	PREG[198]		// ��ʾ�ۻ�ͨ��ʱ��
	0, 1, 0, 0,			   //	P_E_M	PREG[199]		// ��ʾ�ۻ����ת����ʱ��
	0, 1, 0, 0,			   //	P_E_DV	PREG[200]		// ��ʾ�趨����İ汾��
	0, 1, 0, 0,			   //	P_E_T	PREG[201]		// ��ʾ����
	0, 1, 0, 0,			   //	P_E_BV	PREG[202]		// ��ʾϵͳROM�İ汾��
	0, 1, 0, 0,			   //	P_E_SV	PREG[203]		// ��ʾϵͳ����İ汾��
						   // ����(Special)
	0, 99, 28, 28,		   //	P_LHH	PREG[204]		// High speed H upper limit setting			x100s/min		0		99		23
	0, 99, 0, 0,		   //	P_LHL	PREG[205]		// High speed H lower limit setting			x100s/min		0		99		0
	0, 99, 4, 4,		   //	P_LLH	PREG[206]		// Low speed L upper limit setting			x100s/min		0		99		4
	0, 99, 0, 0,		   //	P_LLL	PREG[207]		// LOw speed H lower limit setting			x100s/min		0		99		0
	0, 1, 0, 0,			   //	P_DSL	PREG[208]		// Speed limit setting														OF(0)
	0, 1, 1, 1,			   //	P_FSL	PREG[209]		// Speed limit cancel when continuos feed									ON(1)
	0, 1, 0, 0,			   //	P_STM	PREG[210]		// Needle stop mode selection												OF(0)
	0, 1, 0, 0,			   //	P_KU	PREG[211]		// Selection of UP signal													OF(0)
	0, 360, 270, 270,	   //	P_K8	PREG[212]		// UP position angle from down position		deg.			0		360		270
	0, 1, 0, 0,			   //	P_E8C	PREG[213]		// Detector error cancel mode												OF(0)
	0, 1, 0, 0,			   //	P_MSP	PREG[214]		// Max. speed for main motor												36(0)
	0, 1, 0, 0,			   //	P_PL	PREG[215]		// Sewing machine shaft/motor shaft speed setting selection					OF(0)
	0, 200, 100, 100,	   //	P_MR	PREG[216]		// Motor pulley diameter					mm				0		254		60	// �ò�����Ϊ���Ӽ���ѹ����������
	0, 300, 20, 20,		   //	P_SR	PREG[217]		// Sewing machine pulley diameter			mm				0		254		70	// �ò�����Ϊ�󴲺����ת�����͵���Сת���Ƕ� 	 1600 = 360�� 4.4444
	0, 99, 14, 14,		   //	P_ACT	PREG[218]		// Acceleration time simple setting 		msec			0		99		14
	0, 99, 0, 0,		   //	P_DCT	PREG[219]		// ����ʱ������趨											OF(0)
	0, 1, 0, 0,			   //	P_CCW	PREG[220]		// Main motor rotating direction											OF(0)
	0, 360, 28, 28,		   //	P_L8	PREG[221]		// Setting ofr down angle at low speed		deg.			0		360		28
	0, 2, 1, 1,			   //	P_PPD	PREG[222]		// XY���ԭ��˳������													ON(1)
	0, 1, 1, 1,			   //	P_HXC	PREG[223]		// X motor CW/CCW															ON(1)
	0, 1, 1, 1,			   //	P_HXS	PREG[224]		// Stepping motor rotation direction at home positioning					ON(1)
	0, 1, 0, 0,			   //	P_HYC	PREG[225]		// Y motor CW/CCW															OF(0)
	0, 1, 1, 1,			   //	P_HYS	PREG[226]		// Stepping motor rotation direction at home positioning					ON(1)
	0, 65535, 2500, 2000,  //	P_F		PREG[227]		// Feed speed setting										0		65535	80
	0, 255, 105, 105,	   //	P_XHP	PREG[228]		// X axis pulse setting for home positioning				0		255		37
	0, 65535, 3300, 3300,  //	P_XH1	PREG[229]		// X axis pulse setting for home positioning(high speed)	0		65535	160
	0, 65535, 300, 300,	   //	P_XL1	PREG[230]		// X axis pulse setting for home positioning(low speed)		0		65535	2048
	0, 255, 20, 20,		   //	P_XT	PREG[231]		// Lowest movement amount when X axis return to home position	0	254		20
	0, 255, 55, 55,		   //	P_YHP	PREG[232]		// Y axis pulse setting for home positioning				0		255		37
	0, 65535, 3300, 3300,  //	P_YH1	PREG[233]		// Y axis pulse setting for home positioning(high speed)	0		65535	160
	0, 65535, 300, 300,	   //	P_YL1	PREG[234]		// Y axis pulse setting for home positioning(low speed)		0		65535	2048
	0, 255, 20, 20,		   //	P_YT	PREG[235]		// Lowest movement amount when Y axis return to home position	0	254		20
	0, 9999, 640, 640,	   //	P_L3	PREG[236]		// Pulse motor start time when X smooth feed is low			0		9998	640
	0, 9999, 175, 175,	   //	P_H3	PREG[237]		// Pulse motor start time when X smooth feed is high		0		9998	175
	0, 9999, 1, 1,		   //	P_X3	PREG[238]		// Change value of smooth feed of X							0		9998	1
	0, 9999, 640, 640,	   //	P_L4	PREG[239]		// Pulse motor start time when Y smooth feed is low			0		9998	640
	0, 9999, 175, 175,	   //	P_H4	PREG[240]		// Pulse motor start time when Y smooth feed is high		0		9998	175
	0, 9999, 1, 1,		   //	P_Y4	PREG[241]		// Change value of smooth feed of Y							0		9998	1
	0, 1, 1, 1,			   //	P_HME	PREG[242]		// ԭ�����书��							0		9998	1
	0, 2, 1, 1,			   //	P_MOS	PREG[243]		// �������Ѷ�ű��ı�							0		9998	1
	0, 360, 1, 20,		   //	P_MS8	PREG[244]		// ����Ƕ�ƫ��ֵ							0		9998	1
	0, 0, 0, 0,			   //	P_	PREG[245]		// ����
	0, 0, 0, 0,			   //	P_	PREG[246]		// ����
	0, 0, 0, 0,			   //	P_	PREG[247]		// ����
	0, 0, 0, 0,			   //	P_	PREG[248]		// ����
	0, 0, 0, 0,			   //	P_	PREG[249]		// ����
	0, 0, 0, 0,			   //	P_	PREG[250]		// ����
	0, 0, 0, 0,			   //	P_	PREG[251]		// ����
	0, 0, 0, 0,			   //	P_	PREG[252]		// ����
	0, 0, 0, 0,			   //	P_	PREG[253]		// ����
	0, 0, 0, 0,			   //	P_	PREG[254]		// ����
	0, 0, 0, 0,			   //	P_	PREG[255]		// ����
};

const U16 PARA_INSET[] = { //���ݡ��߼�����ʽ
	1, 0, 0,			   // I1������ΪSRT
	15, 0, 0,			   // I2������ΪIF1
	16, 0, 0,			   // I3������ΪIF2
	6, 1, 0,			   // I4������ΪSTP
	80, 0, 0,			   // I5������ΪHES
	37, 0, 0,			   // I6������ΪTHS
	15, 0, 0,			   // I7������Ϊ
	1, 0, 0,			   // I8������Ϊ
	2, 0, 0,			   // I9������Ϊ
	0, 0, 0,			   // IA������Ϊ
	0, 0, 0,			   // IB������Ϊ
	0, 0, 0,			   // IC������Ϊ
	0, 0, 0,			   // IE1������Ϊ
	0, 0, 0,			   // IE2������Ϊ
	0, 0, 0,			   // IE3������Ϊ
	0, 0, 0,			   // IE4������Ϊ
	0, 0, 0,			   // IE5������Ϊ
	0, 0, 0,			   // IE6������Ϊ
	0, 0, 0,			   // IE7������Ϊ
	0, 0, 0,			   // IE8������Ϊ
	0, 0, 0,			   // IE9������Ϊ
	0, 0, 0,			   // IEA������Ϊ
	0, 0, 0,			   // IEB������Ϊ
	0, 0, 0,			   // IEC������Ϊ
	0x5a, 0xa5, 0x01, 0x26};

const U16 PARA_OUTSET[] = {				  //���ݡ��߼�����ʽ�����塢������ʱ���ر���ʱ���������رա�ȫ�������ӡ�����
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O1������ΪT
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O2������ΪW
	2, 0, 0, 8, 0, 0, 20, 50, 1500, 0, 0, // O3������ΪL
	60, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O4������ΪOF1
	61, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O5������ΪOF2
	10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O6������ΪCB
	44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O7������ΪFN1
	45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O8������ΪFN2
	46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // O9������ΪFN3
	47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OA������ΪFN4
	48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OB������Ϊ
	49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OC������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE1������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE2������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE3������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE4������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE5������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE6������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE7������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE8������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OE9������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OEA������ΪF
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OEB������Ϊ
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	  // OEC������Ϊ
	0xa5, 0x5a, 0x00, 0x88};

//������趨����
const volatile S16 PROMG_DATA[PROMGMAX][3] = {
	//��Сֵ - ���ֵ - Ĭ��ֵ
	{1, 5, 1},		//0-JOG����ģʽ
	{0, 1, 0},		//1-��������������
	{-999, 999, 0}, //2-X�������ֵ(mm)
	{-999, 999, 0}, //3-Y�������ֵ(mm)
	{0, 1, 0},		//4-���Ƶ�ȷ��ʱ��ѡ�����
	{0, 1, 0},		//5-����HOME����ENTER��
	{0, 1, 0},		//6-��Ǳ�ʾ
	{-999, 999, 0}, //7-���Xλ��(��꿪ʼ���ƶ���)(mm)
	{-999, 999, 0}, //8-���Yλ��(��꿪ʼ���ƶ���)(mm)
	{0, 1, 0},		//9-���������Զ�����
	{0, 1, 0},		//10-����,����������ʾ
	{1, 4, 2},		//11-����ƶ��ٶ�
};
