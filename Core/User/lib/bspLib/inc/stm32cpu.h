//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  stm32cpu.h								*
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

// SYSTEM0 - 640x480 ��ʾ��
// SYSTEM1 - 800x600 ��ʾ�� (������)
// SYSTEM2 - 800x600 ��ʾ�� (�ֵ�H����)

#ifndef __STM32CPU_H
#define __STM32CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx.h"

#define Debug 0	 //1:�������Դ�ӡ
#define Debug1 0 //1:�������Դ�ӡ
#define Debug2 0 //1:�������Դ�ӡ
#define Debug3 0 //1:�������Դ�ӡ
#define Debug4 0 //1:�������Դ�ӡ

#define EN_SEWCHECK 0 //0 ��У�����sew�ļ�

#define PROMGMAX 30 //��̲��������(U16) ������ds2460
/*--Flash �汾˵��
--640x480��ʾ��flash�汾 0-500
--800x600��ʾ��flash�汾500-1000
--800x480��ʾ��flash�汾1000-1500
*/

#define UPDAT_MB 0	//�л�ϵͳ��������
#define RDTIMES 400 // ���������ݵĴ���
//#define		W25Q128_MAX		0x1000000			// W25q128���� B

#define UPSFTRESET 1 //1-һ����ԭ������Ҫ�����¼ 0-����Ҫ

#define USEMULTCOLOR 1 //0-�ڰ� 1-��ɫ

#define SPLSHAPE_MIN 0
#define SPLSHAPE_MAX 359
#define SPLSHAPE_BAX 10
//#define		SPLSHAPE_MIN	-9
//#define		SPLSHAPE_MAX	20
//#define		SPLSHAPE_BAX	10
// SPLSHAPE_BAX+SPLSHAPE_MIN>0

#define PI 3.14156

#define Pindismax 127 * 2 //�ֵܿ�����

//#define DIS_LOGO		0		//���̱�logo
#define DIS_LOGO 1 //�����̱�logo
//#define DIS_LOGO		2		//��������avi logo.aiv

#define DIS_MODE 0 //��ʾģʽ �綯,���,�޸Ļ��治��ʾͼ��
//#define DIS_MODE	1	//��ʾģʽ �綯,���,�޸Ļ�����ʾͼ��

#define P1_MODE 0 //P1 ����ͨ����ʾģʽ
//#define P1_MODE		1	//P1 ���������ʾģʽ

//-----���в���->���η���
#define ADDR_OFF 10 //����ʵ����Ч���ݴӵ�10���Ĵ�����ʼ

//-----Һ����---------
#define LCD_5cun 0 //5���� 640*480
#define LCD_8cun 1 //8���� 800*600
#define LCD_7cun 2 //7���� 800*480

#define HORIZONTAL 0 //����
#define VERTICAL 1	 //����

/*********************ϵͳ����-bigin************************/

#define MOVEUNIT 0 //�ƶ���λ0-0.1;1-0.05

#define MP3_SADDR 0

#define DISSUM 16 //ÿҳ��ʾ���ļ���Ŀ
#define SELECTNO_MAX 16

#define VMAXPAG 3 //0-3�Դ�����л�ҳ��
//#define	DISSRC_TYPE		1	//������
//#define	DISSRC_TYPE		2	//������
#define DISSRC_TYPE 3 //������+������

#define LCD_TYPE LCD_7cun

#define VIEWTYPE HORIZONTAL
#define LCD_WIDTH 800  //��ʾ�����
#define LCD_HEIGHT 480 //��ʾ���߶�

#define LCD_width 800  //��ʾ�����
#define LCD_height 480 //��ʾ���߶�

#define LCD_TIME_WIDTH_LIMIT TIT_DISK_XPOS //����ʱ����ʾ����ʾλ�� ����

#define UPGRADEADDR 0x08020600 //������ַ
#define UPGRADESIZE 0xdfa00	   //������ַ

#define DISP_VER 2021 // ���������汾 1017
#define PUB_DATE 126  // ���������ʱ�� ��-��

#define W25QUPIFOADDR 0xe00000 // W25Q�洢����������Ϣ��ʼ��ַ

#define W25QMIFOADDR 0xe00020 // W25Q�洢M-����ϵͳ����������Ϣ��ʼ��ַ
#define W25QMDATADDR 0xe00040 // W25Q�洢M-����ϵͳ��������������ʼ��ַ

#define W25QHIFOADDR 0xf00000 // W25Q�洢H-����ϵͳ����������Ϣ��ʼ��ַ
#define W25QHDATADDR 0xf00020 // W25Q�洢H-����ϵͳ��������������ʼ��ַ

/*********************ϵͳ����-end************************/

#define PLK_G 0x47
#define PLK_ER 0x54
#define MLK_G 0x45

//EMB ��ʽ��ַ
#define DA_ADDR 0x11
#define DL_ADDR 0x13
#define PA_ADDR 0x19
#define PL_ADDR 0x1b
#define NA_ADDR 0x54
#define ORG_ADDR 0x112

#define EMB_AREAMAX 1024	  //����ͼ���ݶ���
#define EMB_CTRLMAX 1024 * 20 //�����Ƶ���

//---------��������
#define K_TOL 1	 //��
#define K_TOR 2	 //��
#define K_TOU 3	 //��
#define K_TOD 4	 //��
#define K_TOLU 5 //����
#define K_TORU 6 //����
#define K_TOLD 7 //����
#define K_TORD 8 //����
#define K_MENU 9 //�˵���
#define K_OK 10	 //ȷ����
#define K_CAL 11 //ȡ����
#define K_E 12	 //����

//---�ŷ�������ʾ����
//						  | | | | | | | |
#define EnDisP0 0x0000000000f67ff  //ѡ��P0��ʾ�˿�  25λ
#define EnDisP1 0x00000026000effff //ѡ��P1��ʾ�˿�  40λ
#define EnDisP2 0x0000000000c140fe //ѡ��P2��ʾ�˿�  30λ
#define EnDisP3 0x00000000013f7fff //ѡ��P3��ʾ�˿�  30λ
#define EnDisP4 0x0000f8ff80003c1f //ѡ��P4��ʾ�˿�  50λ
#define EnDisP5 0x0000000910102347 //ѡ��P5��ʾ�˿�  40λ
#define EnDisP6 0x0000000200704bf8 //ѡ��P6��ʾ�˿�  40λ
#define EnDisP7 0x0000000000018000 //ѡ��P7��ʾ�˿�  50λ

//---  ��������----
#define MEM_IN 0
//#define	MEM_SD		1
#define MEM_DSP 1
#define MEM_USB 2
#define MEM_TF 3

#define LOOPMAX 20	//ѭ���������ֵ
#define CFILEMAX 20 //ѭ�������ļ������ֵ

#define CreatBlkMax 12 //�ֵܿ��屸����Ϣ��С
#define f00000_1 0.00001

#define PEESS_MAXVAL 100 //ѹ����ֵ(��ֵԽСѹ��Խ��)1	2
#define PEESS_MINVAL 1
#define PEESS_DEfVAL 4 //ѹ����Ĭ��ֵ

#define TolLangs 16 //������� ֧�������������ֵ

//------------�洢�豸------------------------------------
#define USB 0
#define SD 1
#define TF 2
#define SDTFRDY 35
#define USBTF USB //USB//TF

//Unicode �ֿⷶΧ 24x24
#define Unicode0_Min 0
#define Unicode0_Max 0x24f
#define Unicode1_Min 0x1e00
#define Unicode1_Max 0x1eff
#define Unicode2_Min 0x1000
#define Unicode2_Max 0x109f

#define Mit_line_staddr 32000 //���������������ʼ��ַ
#define Mit_pat_staddr 52000  //�������������ʼ��ַ
//#define USESST39VFXX						// �򿪸ö���ʱʹ�ò���flash SST39VF6401B,����ʹ�ô���flash W25Q128

#define FLASH_BASEAdd 0x2800 // ����������ʼλ��
#define FILE_ALL 424		 // һ�������ļ����ļ�����
#define DIR_SUM 600			 // ������3���ļ���,ÿ���ļ������Ŀ¼��600(���һ��ֻ��80),ÿ��Ŀ¼8���ֽ�

#define FLASHINDEXADDR 0x00003000 // flashǰ����������ʼ��ַ
#define FLASHDATAADDR 0x000a0000  // flashǰ����������ʼ��ַ
#define BINFILE_ALL 178			  // BIN�ļ�����
#define SST39VFADD 0x60000000	  //
#define SDRAMFLASHADD 0xc1800000  // ��ʾ���� ͼƬ �ֿ�����ݻ�����(��SDRAM���Ŀ�0xc1800000 - 0xc1ffffff)
#define Bank1ADDR 0xc0800000	  //

#define UP_ADDR 7//volatile U8 g_upinfo[0x20]
#define OS_ADDR 12
#define FL_ADDR 16
#define MP_ADDR 22

//#ifdef OLD_BOOT
//	#define W25QMIFOADDR		0xd00000		// W25Q�洢M-����ϵͳ����������Ϣ��ʼ��ַ
//	#define W25QMDATADDR		0xd00020		// W25Q�洢M-����ϵͳ��������������ʼ��ַ

//	#define W25QHIFOADDR		0xe00000		// W25Q�洢H-����ϵͳ����������Ϣ��ʼ��ַ
//	#define W25QHDATADDR		0xe00020		// W25Q�洢H-����ϵͳ��������������ʼ��ַ

//	#define W25QBACIFOADDR		0xf00000		// W25Q�洢����������Ϣ��ʼ��ַ
//	#define W25QBACDATADDR		0xf00020		// W25Q�洢��������������ʼ��ַ

//#else
//	#define W25QUPIFOADDR		0xe00000		// W25Q�洢����������Ϣ��ʼ��ַ

//	#define W25QMIFOADDR		0xe00020		// W25Q�洢M-����ϵͳ����������Ϣ��ʼ��ַ
//	#define W25QMDATADDR		0xe00040		// W25Q�洢M-����ϵͳ��������������ʼ��ַ

//	#define W25QHIFOADDR		0xf00000		// W25Q�洢H-����ϵͳ����������Ϣ��ʼ��ַ
//	#define W25QHDATADDR		0xf00020		// W25Q�洢H-����ϵͳ��������������ʼ��ַ
//#endif

//�������浼�� �ļ�λ��:U��(boot/boot.jpg) �ļ���С600x800 ��������
#define W25QBSIFOADDR 0xc00000 // W25Q�洢����������Ϣ
#define W25QBSDATADDR 0xc00020 // W25Q�洢������������

#define DISPUSBRDLEN 0x800000 // �������USB SD,һ�ζ�8MB
#define MCPUUSBRDLEN 0x7d0000 // ������USB,һ�ζ�160KB*50=8000KB

#define USEPRINTFDEBUG // ��Ҫʹ��printf��ӡ������Ϣʱ����

#define USESCREENSAVER 1 // 0��ʹ����������		1����ʹ����������

#define ENREADPROTECT 0 // 0 :��ֹ������   1����������������
#define PROTECTLEVEL2 0 // 1 :ʹ�ö���������2   ����������������1

#define FileIndexSize 36   //ÿ֡�ļ���Ϣ��С
#define FileIndexBarpox 2  //ÿ֡�ļ�ÿ֡�ļ���������ʼλ��
#define FileIndexNamepox 4 //ÿ֡�ļ�ÿ֡�ļ�������ʼλ�� ����ϵͳΪ4
#define PatFilesMAX 999	   //�����ļ������ֵ  1~999

//------------------��ѹ��----------------------------------
#define PF_ABS_MAX 80 //��ѹ�����ֵ
#define PF_ABS_MIN 0  //��ѹ����Сֵ

//---------------���Ӽ�����-------------------------------
#define LF_ABS_MAX 100 //���Ӽ��������ֵ
#define LF_ABS_MIN 20  //���Ӽ�������Сֵ

//�ļ�����
#define FILE_SEW 0
#define FILE_MITE 1
#define FILE_MITG 2
#define FILE_EMB 3
#define FILE_OTHER 4
#define FILE_FOLDER 5
#define FILE_SEQ 6
#define FILE_GPKMC 7
#define FILE_GPKMD 8
#define FILE_MACP 9
//��������
#define PAT_MIT 1 //����
#define PAT_SEW 2 //�ֵ�
#define PAT_EMB 3

#define GET_ALLFILE 0
#define GET_SEQFILE 1
#define GET_PATFILE 2  //mit+sew+emb
#define GET_EMBFILE 3  //EMB
#define GET_MITFILE 4  //mit
#define GET_SEWFILE 5  //sew
#define GET_GROFILE 6  //���ͼ��
#define GET_KMCFILE 7  //���ͼ .kmc
#define GET_KMDFILE 8  //�Ӻ��� .kmd
#define GET_FOLDER 9   //��ȡ�ļ���
#define GET_MACFILE 10 //��ȡ��������.mac

#define T_UNKNOW 0 //δ֪����
#define T_MITE 1   //����E��
#define T_MITG 2   //����G��
#define T_SEWNO 3  //SEW�б��
#define T_SEW 4	   //SEW�ޱ��
#define T_SEWENO 5 //SEW�б��E��
#define T_SEQ 6	   //SEO�ļ�
#define T_EMBNO 7  //EMB�ļ��б��
#define T_EMB 8	   //EMB�ļ��ޱ��
#define T_KMC 9	   //���ͼ.kmc�ļ�
#define T_KMD 10   //�Ӻ���.kmd�ļ�
#define T_MAC 10   //��������.mac�ļ�

//��������
#define BAS311HN 0
#define BAS326H 1
#define BAS326H484 2
#define BAS341H 3
#define BAS342H 4
#define BAS34XH 5
#define UNKNOWN 6

#define MitFontColor Color_Blue		 //�����������ʾ��ɫ
#define BorFontColor Color_Coral	 //�ֵܿ�������ʾ��ɫ ԭRGB(177, 0, 3)
#define EmbFontColor Color_Red		 //�ֵܿ�EMB������ʾ��ɫ
#define NO_DATAColor Color_SlateBlue //���ļ���ʾ��ɫ

#define MINFILENO 1		 // �ļ���д��С���
#define MAXFILENO 999	 // �ļ���д�����
#define SUMFILENO 999	 // �ļ���д���ļ���
#define PTINDEXNOADD 0	 // �ļ��������ļ����λ��		0
#define PTINDEXNAMEADD 4 // �ļ��������ļ���λ��			32
#define PTINDEXSECADD 0	 // �ļ��������ļ���ʼ����λ��	0
#define PTINDEXENCADD 2	 // �ļ��������ļ�������λ��		2
#define PTINDEXLENADD 0	 // �ļ��������ļ�����λ��		0
#define PTINDEXREDLEN 36 // �ļ�������ÿ��Ŀ¼��Ĵ�����

#define GroupPatMax 30	  //�ϲ�����ÿ��Ĵ�����С
#define GroupPNoAddr 0	  //�ϲ��������������ʼ��ַ
#define GroupPNameAddr 20 //�ϲ�����������ʼ��ַ
#define GroupPNumAddr 28  //�ϲ�������¼����������ʼ��ַ
#define GroupPTypeAddr 29 //�ϲ�������¼���������ʼ��ַ

#define GroupMax 20		 //�ϲ�ģʽ�������
#define GroupComAddr 60	 //���ģʽ������ʼ��ַ
#define GroupConAddr 660 //�Ӻ�ģʽ������ʼ��ַ

//�ֵ��ļ�ϵͳ�����ļ���Ϣ(������ BLKBUF�Ĵ�����)
#define CUR_PATH_ADDR 0 //��ǰ·��
#define CUR_PATH_MAX 512
#define FROM_PATH_ADDR CUR_PATH_ADDR + CUR_PATH_MAX //���Ƽ�����Դ·��
#define FROM_PATH_MAX 512
#define SAVE_PATH_ADDR FROM_PATH_MAX + FROM_PATH_MAX //������������ļ�·��
#define SAVE_PATH_MAX 0xfff

#define FALSE 0
#define TRUE 1
//#define WDTC()	{SETWDI();SETWDI();SETWDI();CLRWDI();}
#define WDTC()    \
	{             \
		SETWDI(); \
		CLRWDI(); \
		SETWDI(); \
		CLRWDI(); \
	}

//------------------�������Ͷ���----------------------------------
#define U8 unsigned char	   // �޷��� 8λ���ͱ���
#define u8 unsigned char	   // �޷��� 8λ���ͱ���
#define S8 signed char		   // �з��� 8λ���ͱ���
#define s8 signed char		   // �з��� 8λ���ͱ���
#define U16 unsigned short	   // �޷���16λ���ͱ���
#define u16 unsigned short	   // �޷���16λ���ͱ���
#define S16 signed short	   // �з���16λ���ͱ���
#define s16 signed short	   // �з���16λ���ͱ���
#define U32 unsigned int	   // �޷���32λ���ͱ���
#define u32 unsigned int	   // �޷���32λ���ͱ���
#define S32 signed int		   // �з���32λ���ͱ���
#define s32 signed int		   // �з���32λ���ͱ���
#define U64 unsigned long long // �޷���64λ���ͱ���
#define u64 unsigned long long // �޷���64λ���ͱ���
#define S64 signed long long   // �з���64λ���ͱ���
#define s64 signed long long   // �з���64λ���ͱ���
#define F32 float			   // �����ȸ�������32λ���ȣ�
#define f32 float			   // �����ȸ�������32λ���ȣ�
#define F64 double			   // ˫���ȸ�������64λ���ȣ�
#define f64 double			   // ˫���ȸ�������64λ���ȣ�
#define uint unsigned short	   // �޷���16λ���ͱ���
#define uint16 unsigned short  // �޷���16λ���ͱ���

#define FOSC ((U32)8000000)	   // �ⲿ����Ƶ��
#define FPLL ((U32)192000000)  // PLL���Ƶ��
#define FCLK ((U32)192000000)  // �ں�����Ƶ��
#define FHCLK ((U32)192000000) // �ڲ���������Ƶ��
#define FPCLK1 ((U32)48000000) // ��Ƶ��1Ƶ�� ��ʱ��2 3 4 ����2 3ʹ��
#define FPCLK2 ((U32)96000000) // ��Ƶ��2Ƶ�� ��ʱ��1 ����1 ADCʹ��
#define FUSART1 ((U32)6000000) // ����1ʱ��Ƶ��=96MHz / 16
#define FUSARTx ((U32)3000000) // ����nʱ��Ƶ��=48MHz / 16(n=2 3 4 5)
#define THCLK ((U32)12000000)  // ��ʱ��1 8 9 10 11 ʱ��Ƶ�� 			��Ƶ��2Ƶ�� / 16 = 6M
#define TLCLK ((U32)12000000)  // ��ʱ��2 3 4 5 6 7 12 13 14 ʱ��Ƶ�� 	��Ƶ��1Ƶ�� / 8  = 6M

#define SYSID 212020101	 // ϵͳ��� 32bit
#define SYSKEY 12345678	 // ����		32bit
#define SYSKEY 12345678	 // ����		32bit
#define SYSDATE 20121202 // ����		32bit

#define SYSTYPE 3020 // ϵͳ�ͺ�
#define SYSVER 4000	 // �汾		8bit   2012.12.02

extern volatile U32 BIND, BINND;
extern volatile U32 statusf;
extern U32 Timer_lsn;
extern U32 Timer_plg;
extern volatile U8 tempout;

typedef struct
{
	U8 flag;  // ���
	U32 len;  // ��������۳���
	U32 len1; // ������ǰ����
} SEW_CHECK;

typedef struct
{
	U16 datab;	// ���ݳ���
	U16 stopb;	// ֹͣλ��
	U16 parity; // ��żУ��λ
} UARTMODE;

typedef struct
{
	U16 xpis; // ˮƽ���� 0-19
	U16 ypis; // ��ֱ���� 0-14
	U16 xsize;
	U16 ysize;
} KEYPAD;

typedef struct
{
	U16 xpis; // ˮƽ���� 0-19
	U16 ypis; // ��ֱ���� 0-14
	U16 xsize;
	U16 ysize;
	U16 colorB[4]; // �����ɫ ��   �����ɫ
	U16 colorT[4]; // ������ɫ ���� ����ɫ
	U16 index[4];  // �������� ���� ������ɫ
} RECTPAD2;

typedef struct
{
	U16 xpis; // ˮƽ���� 0-19
	U16 ypis; // ��ֱ���� 0-14
	U16 xsize;
	U16 ysize;
	U16 colorB[3]; // �����ɫ ��   �����ɫ
	U16 colorT[2]; // ������ɫ ���� ����ɫ
	U16 index[3];  // �������� ���� ������ɫ
} RECTPAD;

typedef struct
{
	U16 xpis; // ˮƽ���� 0-19
	U16 ypis; // ��ֱ���� 0-14
	U16 xsize;
	U16 ysize;
	U16 icon[4]; //������bmpͼƬ��Ż��߰�����ɫ
	U16 filter; //������ɫ
} BMPPAD;

typedef struct
{
	U16 border[3];	  //�����߿���ɫ
	U16 textColor[3]; //���ְ�ť������ɫ
	U16 icon[4];	  //������bmpͼƬ��Ż��߰�����ɫ
	U16 titleStr[4];  // TIT��ʾ������ʾ��������
	U16 nameStr[4];	  // ��ť������ʾ����
} tydDisSrcIndex;

typedef struct
{
	U16 bflashAllFlag : 1;	  //���±�־λ
	U16 bFlashPageFlag : 1;	  //��ҳ��־
	u16 bFlashSelectFlag : 1; //ѡ����Ÿ���
	U16 disNumDiv;			  //ÿҳ��ʾ����
	U16 selectMode;			  //0:��ѡ��1:��ѡ

	U16 patternNum; //�ܻ����� 1-999
	U16 pageNum;	//��ҳ�� 1-999/div
	U16 pageNow;	//��ǰ����ҳ 1-999/div
	U16 pageLast;	//ѡ�еĻ�������ҳ 1-999/div

	U16 noInPageNow;  //��ǰѡ����ţ���Ե�ǰҳ   0-div
	U16 noInPageLast; //��һ����ţ���Ե�ǰҳ   0-div
	U16 selectNoNow;  //��ǰѡ�����,����ܻ����� 1-999
	U16 selectNoLast; //��һ�����,����ܻ����� 1-999

	U8 selectList[12]; //��ѡģʽ����ǰҳѡ���б�
	U32 dataAdd[12];   //һҳ�Ļ����ļ���Ϣ�׵�ַ
	U8 selectEnable[12]; //��ťʹ��
} tydPatternList;


#define PAT_CODEH 0
#define PAT_CODES 1
#define PAT_CODEE 2
#define PAT_CODEP 3

#define CONVAS1 1 //������
#define CONVAS2 2 //��ȡ�����б����-��ҳ��
#define CONVAS3 3 //ͼƬ������Ϣ���������
#define CONVAS4 4 //ͼƬ��Ԥ��
#define CONVAS5 5 //���������
#define CONVAS6 6 //�޸ġ��任������
#define CONVAS7 7 //�޸ġ��任ѡ�ν���
#define CONVAS8 8 //�޸ġ��任���ƽ���-��
#define CONVAS9 9 //�޸ġ��任���ƽ���-С

#define PM_BASE 0x00	//������ʾ
#define PM_SEWING 0x01	//������
#define PM_PATTING 0x02 //������
// #define PM_SEL_PART1 0x04 //ѡ�ж�(x-x+n)
// #define PM_SEL_PART2 0x08 //ѡ�ж�(a-b)
// #define PM_SEL_PART3 0x10 //ѡ�ж�(a-b||a-x)

typedef struct
{
	U8 codeH : 1;  //ԭ��ͼƬ
	U8 codeH2 : 1; //��ԭ��ͼƬ
	U8 codeP : 1;  //��ѹ��ͼƬ
	U8 codeU : 1;  //����ͣͼƬ
	U8 codeD : 1;  //����ͣͼƬ
	U8 codeJ : 1;  //����ͼƬ
	U8 codeC : 1;  //����ͼƬ
	U8 codeT : 1;  //����ͼƬ
	U8 codeF1 : 1; //����1ͼƬ
	U8 codeF2 : 1; //����2ͼƬ
	U8 codeF3 : 1; //����3ͼƬ
	U8 codeF4 : 1; //����4ͼƬ
	U8 codeF5 : 1; //����5ͼƬ
	U8 codeF6 : 1; //����6ͼƬ
	U8 codeF7 : 1; //����7ͼƬ
	U8 codeF8 : 1; //����8ͼƬ
	U8 codeFX : 1; //����9ͼƬ
} tydunPatCodeChild;

typedef union
{
	U32 code; //����
	tydunPatCodeChild codeChild;
} tydunPatCode;

typedef struct
{
	U8 h_patMode;				   //��-0x00:������ʾ;0x01:������(0-x);0x02:��������
								   //0x04:ѡ������(x-x+n);0x08:ѡ������(a-b);0x10:ѡ������(a-b||a-x);
	U8 h_patConvasIndex;		   //��-�������
	FunctionalState h_subline : 1; //��-������
	FunctionalState h_imgZoom : 1; //��-ͼ������
	FunctionalState h_imgDrag : 1; //��-ͼ����ק
	FunctionalState h_line : 1;	   //��-�߶�
	FunctionalState h_point : 1;   //��-���
	FunctionalState h_org : 1;	   //��-ԭ��
	FunctionalState h_code : 1;	   //��-����
	FunctionalState h_exCode : 1;  //��-������ʾ����
	FunctionalState h_orgOffset : 1;  //��-ԭ��ƫ����ʾ��־λ

	FunctionalState l_line_udZhou : 1; //��-��-������
	FunctionalState l_line_speed : 1;  //��-��-�ٶ�

	U8 l_org_M_XL; //��-0x01:С��;0x02:��ʮ��

	FunctionalState l_ex_select : 1;	 //(��ʾѡ�е��߶�ʹ��)��-����-ѡ������ H/S/E/P/+
	FunctionalState l_ex_pat : 1;		 //(��ʾ��ͼ���߶�ʹ��)��-����-�����δ���/11/22/33/ �任����-��ʾ���
	FunctionalState l_ex_ctrl : 1;		 //(���ƿ��Ƶ�ʹ��-�������)��-����-���Ƶ�/1/2/3/4/+/
	FunctionalState l_ex_cursor : 1;	 //(���Ƶ�ǰ�������ʹ��-��������)��-����-��ǰ���
	FunctionalState l_ex_selectNear : 1; //(ѡ������Χ��ʹ��)��l_ex_topNear����-������ѡ������
	FunctionalState l_ex_topNear : 1;	 //(��ʾ�����Χ��ʹ��)��l_ex_selectNear����-�����������ʾ

	U8 l_ex_select_mode; //0x01:��ʾ����;0x02����ʾ��ʮ�ֹ��;0x04:��ʾСʮ�ֹ��
	U8 l_ex_select_type; //0x00: ��P�� 0x01:ѡ�ж�(x-x+n);0x02:ѡ�ж�(a-b);0x04:ѡ�ж�(a-b||a-x);
	U8 l_ex_ctrl_mode;	 //0x01:��ʾ����;0x02����ʾ��ʮ�ֹ��
	U8 l_ex_ctrl_type;	 //1.�޸Ļ���ADD-���޸�-��ʾ������;2:�任����-����/��ת-��ʾ��׼��;3:
						 //4:���ƻ���-��������;5:���ƻ���-�������;6:���ƻ���-��������
						 //7:�޸Ļ���MODIFY1-��������;8:�޸Ļ���MODIFY1-A-B��������;
						 //9:�޸Ļ���MODIFY1-A-B�������g_jogmode=0 10:�޸Ļ���MODIFY1-A-B�������g_jogmode=1

	U8 l_ex_cursor_mode; //0x01:��ʾ����;0x02:��ʾ��ʮ�ֹ��;0x04:��ʾСʮ�ֹ��
	U8 l_ex_cursor_type; //1:������;2:�任����-��������;
						 //4:���ƻ���-��������;5:���ƻ���-�������;6:���ƻ���-��������
						 //7:�޸Ļ���MODIFY1-��������;8:�޸Ļ���MODIFY1-A-B��������;
						 //9:�޸Ļ���MODIFY1-A-B�������g_jogmode=0 10:�޸Ļ���MODIFY1-A-B�������g_jogmode=1

	tydunPatCode l_code; //��-����-��ʾ����
} tydstPatRule;

extern tydstPatRule patRule;
#define PAT_CODE_ALL patRule.l_code.code
#define PAT_CODE_H patRule.l_code.codeChild.codeH
#define PAT_CODE_H2 patRule.l_code.codeChild.codeH2
#define PAT_CODE_P patRule.l_code.codeChild.codeP
#define PAT_CODE_U patRule.l_code.codeChild.codeU
#define PAT_CODE_D patRule.l_code.codeChild.codeD
#define PAT_CODE_J patRule.l_code.codeChild.codeJ
#define PAT_CODE_C patRule.l_code.codeChild.codeC
#define PAT_CODE_T patRule.l_code.codeChild.codeT
#define PAT_CODE_F1 patRule.l_code.codeChild.codeF1
#define PAT_CODE_F2 patRule.l_code.codeChild.codeF2
#define PAT_CODE_F3 patRule.l_code.codeChild.codeF3
#define PAT_CODE_F4 patRule.l_code.codeChild.codeF4
#define PAT_CODE_F5 patRule.l_code.codeChild.codeF5
#define PAT_CODE_F6 patRule.l_code.codeChild.codeF6
#define PAT_CODE_F7 patRule.l_code.codeChild.codeF7
#define PAT_CODE_F8 patRule.l_code.codeChild.codeF8
#define PAT_CODE_FX patRule.l_code.codeChild.codeFX

#define PATRULES_ORG_OFFSET 0  //ԭ��ƫ��ʹ�ܱ�־

#define PATRULES_P1 1		   //������
#define PATRULES_READ 2		   //������ȡ����
#define PATRULES_VIEW_DETAIL 3 //����ֱ��Ԥ��-������ϸ��Ϣ

#define PATRULES_JOG_DETAIL 4 //��������Ԥ��-����ʵ������Ȳ���
#define PATRULES_JOG_X_N 5	  //��������Ԥ��-����ʵ������Ȳ���
#define PATRULES_JOG_A_B 6	  //��������Ԥ��-����ʵ������Ȳ���
#define PATRULES_JOG_A_X 7	  //��������Ԥ��-����ʵ������Ȳ���

#define PATRULES_VIEW_SIMPLE 8 //��������Ԥ��-��������ϸ��Ϣ

#define PATRULES_CREATE1 9	//��������-��������
#define PATRULES_CREATE2 10 //��������-�������-�ƶ��Ĺ�곬����ʾ��Χ
#define PATRULES_CREATE3 11 //��������-��������

#define PATRULES_MODIFY_MAIN 12 //�޸Ļ���-������
#define PATRULES_MODIFY_ADD 13	//�޸Ļ���-ѡ�����
#define PATRULES_MODIFY_ADD1 14 //�޸Ļ���-һ�����Ƶ�
#define PATRULES_MODIFY_ADD2 15 //�޸Ļ���-�������Ƶ�
#define PATRULES_MODIFY_ADD3 16 //�޸Ļ���-������Ƶ�

#define PATRULES_MODIFY1_SINGLE 17	   //�޸Ļ���MODIFY1-��������
#define PATRULES_MODIFY1_SINGLE_A_B 18 //�޸Ļ���MODIFY1-A-B��������
#define PATRULES_MODIFY1_MUL_A_B0 19   //�޸Ļ���MODIFY1-A-B�������g_jogmode=0
#define PATRULES_MODIFY1_MUL_A_B1 20   //�޸Ļ���MODIFY1-A-B�������g_jogmode=1
#define PATRULES_MODIFY1_MUL_A_X 21	   //�޸Ļ���-���ƶ��-X-X+N

#define PATRULES_MODIFY2_MUL_A_X 22 //�޸Ļ���-���ƶ��-X-X+N

#define PATRULES_MODIFY3_ENSURE 23	  //�޸Ļ���-ȷ������

#define PATRULES_CONVERT_MAIN 24	  //�任����-������
#define PATRULES_CONVERT1_MOVE 25	  //�任����-��������
#define PATRULES_CONVERT1_MOVE_ORG 26	  //�任����-��������-ԭ��ƫ������
#define PATRULES_CONVERT1_NUM 27	  //�任����-��������
#define PATRULES_CONVERT1_ENSURE 28	  //�任����-ȷ������
#define PATRULES_CONVERT1_OFFSET 29	  //�任����-��߷����ʾ
#define PATRULES_CONVERT1_OFFSMULT 30 //�任����-���ط����ʾ

typedef struct
{
	U16 conPosX;
	U16 conPosY;
	U16 conSizeX;
	U16 conSizeY;
	U16 conOrgX;
	U16 conOrgY;
	// U16 imgSizeX;
	// U16 imgSizeY;
} tydstConvasNature;

typedef struct
{
	S16 x;
	S16 y;
} Point;

typedef struct
{
	U16 check_sum;
	U16 check_xor;
} Embcheck;

typedef struct __UserPW
{
	U8 name[12];
	U8 password[12];
} UserPW;

typedef struct __PassWordPara
{
	U8 newuser;
	U8 newmode;
	U8 olduser;
	U8 oldmode;
	UserPW users[10];
} PassWordPara;

typedef struct __SystemTime
{
	U8 year;
	U8 month;
	U8 day;
	U8 hour;
	U8 minute;
} SystemTime;

typedef struct _saveINFO
{
	U16 No;	  // �������
	U32 len;  //��������
	U16 Type; //��������
} saveINFO;

typedef struct _MachineInfo
{
	volatile S16 max_xl;
	volatile S16 max_xr;
	volatile S16 max_yu;
	volatile S16 max_yd;
} MachineInfo;

typedef struct
{
	U8 base;
	U8 oldbase;
	U8 val;
	U8 set;
	U8 max;
	U8 min;
} THDTSN;

typedef struct
{
	S8 base;
	S8 oldbase;
	S8 val;
	S8 set;
	S8 max;
	S8 min;
} PRFOOT;

typedef struct //׷���ص������
{
	S16 stposx; //���ο�ʼ������ֵ
	S16 stposy;
	S16 enposx; //���ν���������ֵ
	S16 enposy;
	U8 occlude; //���η�ձ�� 1-��� 0-�Ƿ��
	U8 retimes; //�ص������
	U8 evepins; //ÿ���ص�������
	U8 evecnts;
	S16 repdis[9][2];
	//	S16 repdis1[9][2];	//��һ��XY��������
	//	S16 repdis2[9][2];	//�ڶ���XY��������
	//	S16 repdis3[9][2];	//������XY��������
	//	S16 repdis4[9][2];	//������XY��������

} OVLP;

typedef struct
{
	U16 sno;		// �Ǵ�����
	U16 fno;		// �ļ����			01-00
	char fname[65]; // �ļ���			20-27

	U32 feed_c;	  // ��ǰ����λ��			//2013.3.27
	U16 feed_sum; // ����������		09-08/4   //2013.3.27
	U16 feed_pat; // ����������		09-08/4-(pat�в���01,21,41,61ָ��)  //2013.3.27
	U16 draw_add; // ��ͼ��ʼλ��		48h
	U16 line_add; // ������ʼλ��		05-04+40h
	U32 pat_add;  // ����ʼλ��		2013.3.27

	S16 patmX; // �������Xֵ
	S16 patmY; // �������Yֵ
	S16 patiX; // ������СXֵ
	S16 patiY; // ������СYֵ
	S32 scale; // ����
	U16 p_2hd; // ��ԭ����λ
	U16 org_f; // ԭ����λ

	S16 X2hd; // ��ԭ��λ��X����
	S16 Y2hd; // ��ԭ��λ��Y����
	S16 orgX; // ԭ��λ��X����
	S16 orgY; // ԭ��λ��Y����

	char ver[5]; // �汾				3C-3F
	U16 maxX;	 // ��������ƶ�X
	U16 maxY;	 // ��������ƶ�Y

	S8 PFmax;	 //��ѹ�����ֵ
	S8 PFmin;	 //��ѹ����Сֵ
	U32 pat_len; //�������ݳ��� 8b
	S32 sew_len; //���������ܳ�

	U8 macType; //�������� ���EMB��ʽ
	U8 endcode; //��������
	U16 year;	//��
	U8 month;	//��
	U8 day;		//��
	U8 hour;	//ʱ
	U8 min;		//��
	U8 sec;		//��
} PATHEAD;

typedef struct
{
	S16 ox; //
	S16 oy; //

	U16 feedc; //
	U16 ppc[9];
} OLDPAT;

typedef struct
{
	U32 ptsize;	   //�������ݴ�СB
	U32 lenaddr;   //���������ݳ��ȴ�ŵ�ַ
	U32 pataddr;   //�������������ʼ��ַ
	U32 limaddr;   //����������޴�ŵ�ַ
	U32 orgaddr;   //���������������ŵ�ַ
	U32 ecodeaddr; //����������������ŵ�ַ
	U32 nameaddr;  //����������ƴ�ŵ�ַ
	U32 timeaddr;  //�����������ʱ���ŵ�ַ
} ISEWADD;

typedef struct
{
	U16 cmd; //���ҵ��Ĵ������
	S16 dx;	 //X����仯��
	S16 dy;	 //Y����仯��
	U8 off;	 //���δ�����������ܳ� 2B
} ISEWCMD;

typedef struct
{
	U32 ox; //
	U32 oy; //
	U8 IsDat;
	U16 ppc[18][18];
} OLDPAT1;

typedef struct
{
	S32 x;
	S32 y;
} COORDINATE;

typedef struct
{
	S16 x0;
	S16 y0;
	S16 x1;
	S16 y1;
} COORDINATE2;

typedef struct
{
	U16 flag;	   //��� 0x5a5a
	U16 mode;	   //��ǰʹ�ÿ�������
	U32 size;	   //���������СU16
	U16 vmlk;	   //��ʾmlk��Ϣ
	U16 checksum;  //������������У���
	U16 checksum1; //����������ϢͷУ���
} BOOTSCR;

#define MAXZONE 30 //���������
typedef struct
{
	U8 cmd;			   // ��ͼ����ָ��
	U32 dlen;		   // ���λ�ͼ���ݳ���
	U16 plen;		   // �����ݳ���
	U8 zone;		   // ������
	U8 type;		   // ��������
	U8 ztype[MAXZONE]; // ÿ����������
	U8 ctls[MAXZONE];  // ÿ�����ο��Ƶ���
	U8 plink;		   // ���ӷ�ʽ
	U16 ptich;		   //	���
} EMBDINFO;			   //emb������ʽ��ͼ������Ϣ

typedef struct
{
	U8 cmd;	  // ������ָ��
	U8 cmd1;  // ������ָ��
	U16 pins; // ����
	U8 code;  // ���ӹ��� 1-����ͣ,2-����ͣ,3-����,4-��ѹ��,5-����,6-�����ٶ�,7-���ӹ���,8-��ͣ��,9-��ͣ��
	U16 cval; // ����ֵ
} EMBPINFO;	  //emb������ʽ��ͼ������Ϣ

typedef struct
{
	U8 cmd;		   // ������ָ��
	S16 px;		   // x�������ֵ
	S16 py;		   // y�������ֵ
	S16 dx;		   // ����һ��X�仯ֵ
	S16 dy;		   // ����һ��y�仯ֵ
	U8 code;	   // ���ӹ���
	U8 code1;	   // ���ӹ���1
	U8 codeval[7]; // codeֵ 0-�Ϸ�;1-ѹ��;2-����;3-�����ٶ�;4-����;5-ѡ�����;6-��ͣ
	U16 area;	   //������ͼ���ݺ�
	U8 conect;	   //�������ӷ�ʽ
} EMBPINS;		   //emb����Ϣ

typedef struct
{
	U16 drawPins; //��ͼ��������Ϣ
	U32 newLen;	  //�����ݶ�
} EMBNEWPINS;

#pragma pack(1)
typedef struct _select_id_message
{
	U16 Select_No; // �����ļ���
	U16 Fileno;	   // ѡ���б�����
} select_id_message;

typedef struct __ProInfo //��ǰ����汾
{
	U16 Data[3];
} ProInfo;

typedef struct __RunRecord //���м�¼
{
	U8 usrNo;	 //�û����
	U16 patNo;	 //�������
	U8 hour;	 //Сʱ
	U8 min;		 //����
	U8 sec;		 //��
	U8 timeuint; //��λ
	U8 times;	 //�ӹ�ʱ��
} RunRecord;

typedef struct _PATPOINT
{
	U16 oposx;
	U16 oposy;
	U16 ocolor[40];

	U16 posx;
	U16 posy;
	//	U16	color[9];
} PATPOINT;

typedef struct _CREATEPT
{
	U8 Type;	  //����
	U8 L_Type;	  //����
	U8 P_dis;	  //���
	U8 C_mode;	  //����ģʽ
	U8 C_menthod; //���ӷ�ʽ
	U8 T_dir;	  //��ת����
} CREATEPT;

typedef struct
{
	S16 posx; // x��������
	S16 posy; // y��������
	S16 dx;	  // x��һ�������
	S16 dy;	  // y��һ�������
	U8 speed; // �ٶ�
	U8 abspf; // ��ѹ�ž���ֵ
} PINMSG;	  //�����Ϣ

#define ERRMSGMAX 10 //��౨����Ϣ����
typedef struct _ERRMSG
{
	U16 max;			//��ͬ������Ϣ����
	U16 err[ERRMSGMAX]; //���治ͬ������Ϣ���
} ERRMSG;

//typedef struct _WAVESDAT{
//	U32 raddr;	//��ȡ��ַ
//	U16 rlen;	//����
//	U32 saddr;	//�����ַ
//	U16 sdat[120];	//��������
//	U16 v[3];
//	//U16 c[2];
//}WAVESDAT;

#define FALSES 0
#define TRUES 1

// 8  4  2  1	//
// 3  2	 1	0	H000x
// 7  6	 5	4	H00x0
// 11 10 9  8	H0x00
// 15 14 13 12	Hx000

//#define KEYIN0			0x2000			// PC 13
//#define KEYIN1			0x4000			// PC 14
//#define KEYIN2			0x8000			// PC 15
//#define KEYIN3			0x0002			// PC 1
//#define KEYIN4			0x0040			// PC 6
//#define KEYIN5			0x0080			// PC 7
//#define KEYIN6			0x0010			// PC 4
//#define KEYIN7			0x0020			// PC 5

#define KEYIN0 0x0002 // PC 1 ->S5
#define KEYIN1 0x8000 // PC 15 ->S4
#define KEYIN2 0x4000 // PC 14 ->S1
#define KEYIN3 0x2000 // PC 13 ->S2
#define KEYIN4 0x0080 // PC 7 ->S3
#define KEYIN5 0x0040 // PC 6 ->S6
#define KEYIN6 0x0020 // PC 5 ->S7
#define KEYIN7 0x0010 // PC 4

#define MCIDAT0 0x0100 // PC 8
#define MCIDAT1 0x0200 // PC 9
#define MCIDAT2 0x0400 // PC 10
#define MCIDAT3 0x0800 // PC 11
#define MCICLK 0x1000  // PC 12
#define MCICMD 0x0004  // PD 2
#define MCIPWR 0x0008  // PD 3
#define MCIINS 0x0008  // PI 3
#define INKEYD 0xe0f2  // PC 1 4 5 6 7 13 14 15
#define INADATA 0x0020 // PH 5
#define INBDATA 0x0004 // PE 2
#define INCDATA 0x0108 // PI 8 3
#define INDDATA 0x4000 // PG 14

#define CANRD 0x1000  // PB 12
#define CANTD 0x2000  // PB 13
#define I2CSCL 0x0100 // PB 8
#define I2CSDA 0x0200 // PB 9
#define TXD1 0x0040	  // PB 6
#define RXD1 0x0400	  // PA 10
#define TXD2 0x0004	  // PA 2
#define RXD2 0x0008	  // PA 3

#define SCK 0x0020	  // PA 5
#define MISO 0x0040	  // PA 6
#define MOSI 0x0080	  // PA 7
#define WCS1 0x0010	  // PH 4
#define WCS2 0x0001	  // PB 0
#define WCS3 0x8000	  // PA 15
#define TFCS 0x0002	  // PB 1
#define LTCS 0x0010	  // PA 4
#define LTRST 0x0004  // PB 2
#define LTPKTF 0x0020 // PH 5

#define T_SCK 0x0400	// PB 10
#define T_MISO 0x4000	// PB 14
#define T_MOSI 0x8000	// PB 15
#define T_SSEL 0x0800	// PB 11
#define T_BUSY 0x0100	// PI 8
#define T_PENIRQ 0x0004 // PE 2

#define C_INT 0x0008 // PH 3

#define VSCLK 0x0008  // PB 3
#define VSMISO 0x0010 // PB 4
#define VSMOSI 0x0020 // PB 5
#define VSXCS 0x0080  // PB 7
#define VSRST 0x1000  // PG 12
#define VSXDCS 0x2000 // PG 13
#define DREQ 0x4000	  // PG 14

#define USBVBUS 0x0200	// PA 9
#define USBFSEN 0x0100	// PA 8
#define USBFSDM 0x0800	// PA 11
#define USBFSDP 0x1000	// PA 12
#define WDI 0x0001		// PA 0
#define NBELL 0x0002	// PA 1
#define USBLED 0x0100	// PH 8
#define SDLED 0x0040	// PE 6
#define LCDONOFF 0x0800 // PI 11
#define LCDPWM 0x0200	// PF 9
#define LEDPWM 0x0100	// PF 8
#define TEST1 0x0040	// PH 6
#define TEST2 0x0080	// PH 7
#define IOMODE 0x0040	// PF 6
#define TFINS 0x0004	// PH 2

#define WKEYIN0 GPIOC->IDR &KEYIN0 // PC 1 ->S5
#define WKEYIN1 GPIOC->IDR &KEYIN1 // PC 15 ->S4
#define WKEYIN2 GPIOC->IDR &KEYIN2 // PC 14 ->S1
#define WKEYIN3 GPIOC->IDR &KEYIN3 // PC 13 ->S2
#define WKEYIN4 GPIOC->IDR &KEYIN4 // PC 7 ->S3
#define WKEYIN5 GPIOC->IDR &KEYIN5 // PC 6 ->S6
#define WKEYIN6 GPIOC->IDR &KEYIN6 // PC 5 ->S7
#define WKEYIN7 GPIOC->IDR &KEYIN7 // PC 4

#define WMCIDAT0 GPIOC->IDR &MCIDAT0 // PC 8
#define WMCIDAT1 GPIOC->IDR &MCIDAT1 // PC 9
#define WMCIDAT2 GPIOC->IDR &MCIDAT2 // PC 10
#define WMCIDAT3 GPIOC->IDR &MCIDAT3 // PC 11
#define WMCICLK GPIOC->IDR &MCICLK	 // PC 12
#define WMCICMD GPIOD->IDR &MCICMD	 // PD 2
#define WMCIPWR GPIOD->ODR &MCIPWR	 // PD 3
#define WMCIINS GPIOI->IDR &MCIINS	 // PI 3
#define WINKEYD GPIOC->IDR &INKEYD	 // PC 1 4 5 6 7 13 14 15	Y
#define WINADATA GPIOH->IDR &INADATA // PH 5						>>5	= 0
#define WINBDATA GPIOE->IDR &INBDATA // PE 2						Y
#define WINCDATA GPIOI->IDR &INCDATA // PI 8 3					Y
#define WINDDATA GPIOG->IDR &INDDATA // PG 14					>>2	= 12
#define WINEDATA GPIOH->IDR &TFINS	 // PH 2						<<7 = 9

#define WCANRD GPIOB->IDR &CANRD   // PB 12
#define WCANTD GPIOB->ODR &CANTD   // PB 13
#define WI2CSCL GPIOB->ODR &I2CSCL // PB 8
#define WI2CSDA GPIOB->ODR &I2CSDA // PB 9
#define WTXD1 GPIOB->ODR &TXD1	   // PB 6
#define WRXD1 GPIOA->IDR &RXD1	   // PA 10
#define WTXD2 GPIOA->ODR &TXD2	   // PA 2
#define WRXD2 GPIOA->IDR &RXD2	   // PA 3

#define WSCK GPIOA->ODR &SCK	   // PA 5
#define WMISO GPIOA->IDR &MISO	   // PA 6
#define WMOSI GPIOA->ODR &MOSI	   // PA 7
#define WWCS1 GPIOH->ODR &WCS1	   // PH 4
#define WWCS2 GPIOB->ODR &WCS2	   // PB 0
#define WTFCS GPIOB->ODR &TFCS	   // PB 1
#define WLTCS GPIOA->ODR &LTCS	   // PA 4
#define WLTRST GPIOB->ODR &LTRST   // PB 2
#define WLTPKTF GPIOH->IDR &LTPKTF // PH 5

#define WT_SCK GPIOB->ODR &T_SCK	   // PB 10
#define WT_MISO GPIOB->IDR &T_MISO	   // PB 14
#define WT_MOSI GPIOB->ODR &T_MOSI	   // PB 15
#define WT_SSEL GPIOB->ODR &T_SSEL	   // PB 11
#define WT_BUSY GPIOI->IDR &T_BUSY	   // PI 8
#define WT_PENIRQ GPIOE->IDR &T_PENIRQ // PE 2

#define WVSCLK GPIOB->ODR &VSCLK   // PB 3
#define WVSMISO GPIOB->IDR &VSMISO // PB 4
#define WVSMOSI GPIOB->ODR &VSMOSI // PB 5
#define WVSXCS GPIOB->ODR &VSXCS   // PB 7
#define WVSRST GPIOG->ODR &VSRST   // PG 12
#define WVSXDCS GPIOG->ODR &VSXDCS // PG 13
#define WDREQ GPIOG->IDR &DREQ	   // PG 14

#define WUSBVBUS GPIOA->IDR &USBVBUS   // PA 9
#define WUSBFSEN GPIOA->ODR &USBFSEN   // PA 8
#define WUSBFSDM GPIOA->IDR &USBFSDM   // PA 11
#define WUSBFSDP GPIOA->IDR &USBFSDP   // PA 12
#define WWDI GPIOA->ODR &WDI		   // PA 0
#define WNBELL GPIOA->ODR &NBELL	   // PA 1
#define WUSBLED GPIOH->ODR &USBLED	   // PH 8
#define WSDLED GPIOE->ODR &SDLED	   // PE 6
#define WLCDONOFF GPIOI->ODR &LCDONOFF // PI 11
#define WLCDPWM GPIOF->ODR &LCDPWM	   // PF 9
#define WLEDPWM GPIOF->ODR &LEDPWM	   // PF 8
#define WTEST1 GPIOH->ODR &TEST1	   // PH 6
#define WTEST2 GPIOH->ODR &TEST2	   // PH 7
#define WIOMODE GPIOF->IDR &IOMODE	   // PF 6
#define WTFINS GPIOH->IDR &TFINS	   // PH 2

#define WWKEYIN0 BIND &KEYIN0		// PC 13
#define WWKEYIN1 BIND &KEYIN1		// PC 14
#define WWKEYIN2 BIND &KEYIN2		// PC 15
#define WWKEYIN3 BIND &KEYIN3		// PC 1
#define WWKEYIN4 BIND &KEYIN4		// PC 6
#define WWKEYIN5 BIND &KEYIN5		// PC 7
#define WWKEYIN6 BIND &KEYIN6		// PC 4
#define WWKEYIN7 BIND &KEYIN7		// PC 5
#define WWLTPKTF BIND &bit0			// PH 5
#define WWT_PENIRQ BIND &T_PENIRQ	// PE 2
#define WWT_BUSY BIND &T_BUSY		// PI 8
#define WWMCIINS BIND &MCIINS		// PI 3
#define WWDREQ BIND &bit12			// PG 14
#define WWTFINS BIND &bit9			// PH 2
#define WWKEYIN0N BINND &KEYIN0		// PC 13
#define WWKEYIN1N BINND &KEYIN1		// PC 14
#define WWKEYIN2N BINND &KEYIN2		// PC 15
#define WWKEYIN3N BINND &KEYIN3		// PC 1
#define WWKEYIN4N BINND &KEYIN4		// PC 6
#define WWKEYIN5N BINND &KEYIN5		// PC 7
#define WWKEYIN6N BINND &KEYIN6		// PC 4
#define WWKEYIN7N BINND &KEYIN7		// PC 5
#define WWLTPKTFN BINND &bit0		// PH 5
#define WWT_PENIRQN BINND &T_PENIRQ // PE 2
#define WWT_BUSYN BINND &T_BUSY		// PI 8
#define WWMCIINSN BINND &MCIINS		// PI 3
#define WWDREQN BINND &bit12		// PG 14
#define WWTFINSN BINND &bit9		// PH 2

#if 0
#define SETMCIPWR() GPIOD->BSRRL = MCIPWR // PD 3
#define SETCANTD() GPIOB->BSRRL = CANTD	  // PB 13
#define SETI2CSCL() GPIOB->BSRRL = I2CSCL // PB 8
#define SETI2CSDA() GPIOB->BSRRL = I2CSDA // PB 9
#define SETTXD1() GPIOB->BSRRL = TXD1	  // PB 6
#define SETTXD2() GPIOA->BSRRL = TXD2	  // PA 2

#define SETSCK() GPIOA->BSRRL = SCK		// PA 5
#define SETMOSI() GPIOA->BSRRL = MOSI	// PA 7
#define SETWCS1() GPIOH->BSRRL = WCS1	// PH 4
#define SETWCS2() GPIOB->BSRRL = WCS2	// PB 0
#define SETWCS3() GPIOA->BSRRL = WCS3	// PA 15
#define SETTFCS() GPIOB->BSRRL = TFCS	// PB 1
#define SETLTCS() GPIOA->BSRRL = LTCS	// PA 4
#define SETLTRST() GPIOB->BSRRL = LTRST // PB 2

#define SETT_SCK() GPIOB->BSRRL = T_SCK	  // PB 10
#define SETT_MOSI() GPIOB->BSRRL = T_MOSI // PB 15
#define SETT_SSEL() GPIOB->BSRRL = T_SSEL // PB 11

#define SETVSCLK() GPIOB->BSRRL = VSCLK	  // PB 3
#define SETVSMOSI() GPIOB->BSRRL = VSMOSI // PB 5
#define SETVSXCS() GPIOB->BSRRL = VSXCS	  // PB 7
#define SETVSRST() GPIOG->BSRRL = VSRST	  // PG 12
#define SETVSXDCS() GPIOG->BSRRL = VSXDCS // PG 13

#define SETUSBFSEN() GPIOA->BSRRL = USBFSEN	  // PA 8
#define SETWDI() GPIOA->BSRRL = WDI			  // PA 0
#define SETNBELL() GPIOA->BSRRL = NBELL		  // PA 1
#define SETUSBLED() GPIOH->BSRRL = USBLED	  // PH 8
#define SETSDLED() GPIOE->BSRRL = SDLED		  // PE 6
#define SETLCDONOFF() GPIOI->BSRRL = LCDONOFF // PI 11
#define SETLCDPWM() GPIOF->BSRRL = LCDPWM	  // PF 9
#define SETLEDPWM() GPIOF->BSRRL = LEDPWM	  // PF 8
#define SETTEST1() GPIOH->BSRRL = TEST1		  // PH 6
#define SETTEST2() GPIOH->BSRRL = TEST2		  // PH 7

#define CLRMCIPWR() GPIOD->BSRRH = MCIPWR // PD 3
#define CLRCANTD() GPIOB->BSRRL = CANTD	  // PB 13
#define CLRI2CSCL() GPIOB->BSRRL = I2CSCL // PB 8
#define CLRI2CSDA() GPIOB->BSRRL = I2CSDA // PB 9
#define CLRTXD1() GPIOB->BSRRL = TXD1	  // PB 6
#define CLRTXD2() GPIOA->BSRRL = TXD2	  // PA 2

#define CLRSCK() GPIOA->BSRRH = SCK		// PA 5
#define CLRMOSI() GPIOA->BSRRH = MOSI	// PA 7
#define CLRWCS1() GPIOH->BSRRH = WCS1	// PH 4
#define CLRWCS2() GPIOB->BSRRH = WCS2	// PB 0
#define CLRWCS3() GPIOA->BSRRH = WCS3	// PA 15
#define CLRTFCS() GPIOB->BSRRH = TFCS	// PB 1
#define CLRLTCS() GPIOA->BSRRH = LTCS	// PA 4
#define CLRLTRST() GPIOB->BSRRH = LTRST // PB 2

#define CLRT_SCK() GPIOB->BSRRH = T_SCK	  // PB 10
#define CLRT_MOSI() GPIOB->BSRRH = T_MOSI // PB 15
#define CLRT_SSEL() GPIOB->BSRRH = T_SSEL // PB 11

#define CLRVSCLK() GPIOB->BSRRH = VSCLK	  // PB 3
#define CLRVSMOSI() GPIOB->BSRRH = VSMOSI // PB 5
#define CLRVSXCS() GPIOB->BSRRH = VSXCS	  // PB 7
#define CLRVSRST() GPIOG->BSRRH = VSRST	  // PG 12
#define CLRVSXDCS() GPIOG->BSRRH = VSXDCS // PG 13

#define CLRUSBFSEN() GPIOA->BSRRH = USBFSEN	  // PA 8
#define CLRWDI() GPIOA->BSRRH = WDI			  // PA 0
#define CLRNBELL() GPIOA->BSRRH = NBELL		  // PA 1
#define CLRUSBLED() GPIOH->BSRRH = USBLED	  // PH 8
#define CLRSDLED() GPIOE->BSRRH = SDLED		  // PE 6
#define CLRLCDONOFF() GPIOI->BSRRH = LCDONOFF // PI 11
#define CLRLCDPWM() GPIOF->BSRRH = LCDPWM	  // PF 9
#define CLRLEDPWM() GPIOF->BSRRH = LEDPWM	  // PF 8
#define CLRTEST1() GPIOH->BSRRH = TEST1		  // PH 6
#define CLRTEST2() GPIOH->BSRRH = TEST2		  // PH 7

#else //֧�־ɱ�����

#define SETMCIPWR()            \
	GPIOD->BSRR &= 0xFFFF0000; \
	GPIOD->BSRR |= MCIPWR // PD 3
#define SETCANTD()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= CANTD // PB 13
#define SETI2CSCL()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSCL // PB 8
#define SETI2CSDA()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSDA // PB 9
#define SETTXD1()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= TXD1 // PB 6
#define SETTXD2()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= TXD2 // PA 2

#define SETSCK()               \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= SCK // PA 5
#define SETMOSI()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= MOSI // PA 7
#define SETWCS1()              \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= WCS1 // PH 4
#define SETWCS2()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= WCS2 // PB 0
#define SETWCS3()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= WCS3 // PA 15
#define SETTFCS()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= TFCS // PB 1
#define SETLTCS()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= LTCS // PA 4
#define SETLTRST()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= LTRST // PB 2

#define SETT_SCK()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= T_SCK // PB 10
#define SETT_MOSI()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= T_MOSI // PB 15
#define SETT_SSEL()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= T_SSEL // PB 11

#define SETVSCLK()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= VSCLK // PB 3
#define SETVSMOSI()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= VSMOSI // PB 5
#define SETVSXCS()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= VSXCS // PB 7
#define SETVSRST()             \
	GPIOG->BSRR &= 0xFFFF0000; \
	GPIOG->BSRR |= VSRST // PG 12
#define SETVSXDCS()            \
	GPIOG->BSRR &= 0xFFFF0000; \
	GPIOG->BSRR |= VSXDCS // PG 13

#define SETUSBFSEN()           \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= USBFSEN // PA 8
#define SETWDI()               \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= WDI // PA 0
#define SETNBELL()             \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= NBELL // PA 1
#define SETUSBLED()            \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= USBLED // PH 8
#define SETSDLED()             \
	GPIOE->BSRR &= 0xFFFF0000; \
	GPIOE->BSRR |= SDLED // PE 6
#define SETLCDONOFF()          \
	GPIOI->BSRR &= 0xFFFF0000; \
	GPIOI->BSRR |= LCDONOFF // PI 11
#define SETLCDPWM()            \
	GPIOF->BSRR &= 0xFFFF0000; \
	GPIOF->BSRR |= LCDPWM // PF 9
#define SETLEDPWM()            \
	GPIOF->BSRR &= 0xFFFF0000; \
	GPIOF->BSRR |= LEDPWM // PF 8
#define SETTEST1()             \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= TEST1 // PH 6
#define SETTEST2()             \
	GPIOH->BSRR &= 0xFFFF0000; \
	GPIOH->BSRR |= TEST2 // PH 7

#define CLRMCIPWR()            \
	GPIOD->BSRR &= 0x0000FFFF; \
	GPIOD->BSRR |= MCIPWR << 16 // PD 3
#define CLRCANTD()             \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= CANTD // PB 13
#define CLRI2CSCL()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSCL // PB 8
#define CLRI2CSDA()            \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= I2CSDA // PB 9
#define CLRTXD1()              \
	GPIOB->BSRR &= 0xFFFF0000; \
	GPIOB->BSRR |= TXD1 // PB 6
#define CLRTXD2()              \
	GPIOA->BSRR &= 0xFFFF0000; \
	GPIOA->BSRR |= TXD2 // PA 2

#define CLRSCK()               \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= SCK << 16 // PA 5
#define CLRMOSI()              \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= MOSI << 16 // PA 7
#define CLRWCS1()              \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= WCS1 << 16 // PH 4
#define CLRWCS2()              \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= WCS2 << 16 // PB 0
#define CLRWCS3()              \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= WCS3 << 16 // PA 15
#define CLRTFCS()              \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= TFCS << 16 // PB 1
#define CLRLTCS()              \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= LTCS << 16 // PA 4
#define CLRLTRST()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= LTRST << 16 // PB 2

#define CLRT_SCK()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= T_SCK << 16 // PB 10
#define CLRT_MOSI()            \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= T_MOSI << 16 // PB 15
#define CLRT_SSEL()            \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= T_SSEL << 16 // PB 11

#define CLRVSCLK()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= VSCLK << 16 // PB 3
#define CLRVSMOSI()            \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= VSMOSI << 16 // PB 5
#define CLRVSXCS()             \
	GPIOB->BSRR &= 0x0000FFFF; \
	GPIOB->BSRR |= VSXCS << 16 // PB 7
#define CLRVSRST()             \
	GPIOG->BSRR &= 0x0000FFFF; \
	GPIOG->BSRR |= VSRST << 16 // PG 12
#define CLRVSXDCS()            \
	GPIOG->BSRR &= 0x0000FFFF; \
	GPIOG->BSRR |= VSXDCS << 16 // PG 13

#define CLRUSBFSEN()           \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= USBFSEN << 16 // PA 8
#define CLRWDI()               \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= WDI << 16 // PA 0
#define CLRNBELL()             \
	GPIOA->BSRR &= 0x0000FFFF; \
	GPIOA->BSRR |= NBELL << 16 // PA 1
#define CLRUSBLED()            \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= USBLED << 16 // PH 8
#define CLRSDLED()             \
	GPIOE->BSRR &= 0x0000FFFF; \
	GPIOE->BSRR |= SDLED << 16 // PE 6
#define CLRLCDONOFF()          \
	GPIOI->BSRR &= 0x0000FFFF; \
	GPIOI->BSRR |= LCDONOFF << 16 // PI 11
#define CLRLCDPWM()            \
	GPIOF->BSRR &= 0x0000FFFF; \
	GPIOF->BSRR |= LCDPWM << 16 // PF 9
#define CLRLEDPWM()            \
	GPIOF->BSRR &= 0x0000FFFF; \
	GPIOF->BSRR |= LEDPWM << 16 // PF 8
#define CLRTEST1()             \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= TEST1 << 16 // PH 6
#define CLRTEST2()             \
	GPIOH->BSRR &= 0x0000FFFF; \
	GPIOH->BSRR |= TEST2 << 16 // PH 7

#endif

#define bit0 0x00000001
#define bit1 0x00000002
#define bit2 0x00000004
#define bit3 0x00000008
#define bit4 0x00000010
#define bit5 0x00000020
#define bit6 0x00000040
#define bit7 0x00000080
#define bit8 0x00000100
#define bit9 0x00000200
#define bit10 0x00000400
#define bit11 0x00000800
#define bit12 0x00001000
#define bit13 0x00002000
#define bit14 0x00004000
#define bit15 0x00008000
#define bit16 0x00010000
#define bit17 0x00020000
#define bit18 0x00040000
#define bit19 0x00080000
#define bit20 0x00100000
#define bit21 0x00200000
#define bit22 0x00400000
#define bit23 0x00800000
#define bit24 0x01000000
#define bit25 0x02000000
#define bit26 0x04000000
#define bit27 0x08000000
#define bit28 0x10000000
#define bit29 0x20000000
#define bit30 0x40000000
#define bit31 0x80000000

#endif
