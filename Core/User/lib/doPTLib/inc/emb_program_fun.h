//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Bor_program_fun.h						*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision5 V5.21					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*

#ifndef _EMB_PROGRAM_FUN_H
#define _EMB_PROGRAM_FUN_H

#ifndef SYSTEM1
#ifndef SYSTEM4
#ifndef SYSTEM5

#include "stm32cpu.h"

#define		LOG			0		//�߼�
#define		NUM			1		//�����޵�λ
#define		NUMU		2		//�����е�λ
#define		SIGSEL		3		//��ѡ
#define		CHECK		4		//��ѡ
#define		BACK		5		//����


//��ѡ�������
#define		STOPNEED	1	//��ֹͣ
#define		LOWSPEED	2	//����
#define		SUSPEND		3	//��ͣ

//��ѡ�����ֵ
#define		STNEED_MAX		2	//��ֹͣ
#define		LOWSPEED_MAX	5	//����
#define		CHOUTPUT_MAX	3	//���ѡ��
#define		SUSPEND_MAX		5	//��ͣ


#define		ADDM		0	//׷�ӷ���
#define 	MCURRM		1	//�ƶ���귽��
#define		LTYPE		2	//�ߵ�����
#define		SPITCH		3	//ֱ�߷����
#define		WPITCH		4	//���۷����
#define		WLWIDTH		5	//���۷�����
#define		WRWIDTH		6	//���۷��ҿ��
#define		WNEEDLES	7	//���۷�����
#define		SMODE		8	//ֱ�߷��γ�ģʽ
#define		SFMODE		9	//�ӹ̷쿪ʼģʽ	
#define		SFNEEDLE	10	//�ӹ̷쿪ʼ����	
#define		EFMODE		11	//�ӹ̷����ģʽ	
#define		EFNEEDLE	12	//�ӹ̷��������
#define		CONNECT		13	//��������
#define		ROTADIR		14	//��ת����
#define		MAXPITCH	15	//������
#define		INSBREAK	16	//�Ϸ���Զ�����
#define		ROTAANG		17	//��ת�Ƕ�
#define		ROTACEN		18	//��ת����
#define		SYMM		19	//�Գ�ģʽ
#define		COMDIR		20	//��������
#define		COMWIDTH	21	//�������
#define		COMCOPYS	22	//����������
#define		COMM		23	//��������������
#define		XRATIO		24	//x����
#define		YRATIO		25	//y����
#define		ZOOMCEN		26	//������������
#define		PINCM		27	//��������ģʽ
#define		LTYPE1		28	//�ߵ�����
#define		CORDER		29	//����˳����
#define		CCONT		30	//ѡ���������ɵ�
#define		ATTRIBUTE	31	//����
#define		PINSMV		32	//�����ƶ���
#define		DISCODE		33	//��ʾ/���ر��
#define		FREPATES	34	//�ӹ̷�����
#define		FNEEDLES	35	//�ӹ̷�����
#define		ADDNEEDM	36	//�뼣����׷�ӷ���

typedef struct _SETPARA{
	U8 no;
	U16 val;
	U16 min;
	U16 max;
	U16 img;
	U8 type;	// 0-�߼� 1-�����޵�λ 2-�����е�λ
}SETPARA;

//typedef struct
//{
//	U32		ox;				// 
//	U32 	oy;				// 
//	U8		IsDat;
//	U16		ppc[7][7];
//} OLDPAT2;

extern const SETPARA ADDM_D;		//׷�ӷ���	
extern const SETPARA MCURRM_D;	//�ƶ���귽�� 
extern const SETPARA LTYPE_D;  	//�ߵ�����
extern const SETPARA SPITCH_D;  //ֱ�߷����
extern const SETPARA WPITCH_D;	//���۷����
extern const SETPARA WLWIDTH_D;	//���۷�����
extern const SETPARA WRWIDTH_D;	//���۷��ҿ��
extern const SETPARA WNEEDLES_D;//���۷�����
extern const SETPARA SMODE_D;	//ֱ�߷��γ�ģʽ
extern const SETPARA SFMODE_D;	//�ӹ̷쿪ʼģʽ
extern const SETPARA SFMODE1_D;	//�ӹ̷쿪ʼģʽ,���ص���
extern const SETPARA SFNEEDLE_D; //�ӹ̷쿪ʼ����	
extern const SETPARA EFMODE_D;	//�ӹ̷����ģʽ
extern const SETPARA EFMODE1_D;	//�ӹ̷����ģʽ,���ص���
extern const SETPARA EFNEEDLE_D; //�ӹ̷��������	
extern const SETPARA CONNECT_D;	//��������
extern const SETPARA ROTADIR_D;	//��ת����
extern const SETPARA MAXPITCH_D;//������
extern const SETPARA INSBREAK_D;	//�Ϸ���Զ�����
extern const SETPARA ROTAANG_D;	//��ת�Ƕ�
extern const SETPARA ROTACEN_D;		//��ת����
extern const SETPARA SYMM_D;	//�Գ�ģʽ	
extern const SETPARA COMDIR_D;	//��������
extern const SETPARA COMWIDTH_D;//�������
extern const SETPARA COMCOPYS_D;	//����������
extern const SETPARA COMM_D;	//��������������
extern const SETPARA XRATIO_D;	//x����
extern const SETPARA YRATIO_D;	//y����
extern const SETPARA ZOOMCEN_D;	//������������
extern const SETPARA PINCM_D;	//��������ģʽ
extern const SETPARA LTYPE1_D;  //�ߵ�����
extern const SETPARA CORDER_D;	//����˳��������
extern const SETPARA CCONT_D;	//ѡ���������ɵ�
extern const SETPARA ATTRIBUTE_D;//����
extern const SETPARA PINSMV_D;	//�����ƶ���
extern const SETPARA DISCODE_D;	//��ʾ/���ر��
extern const SETPARA FREPATES_D;	//�ӹ̷�����
extern const SETPARA FNEEDLES_D;	//�ӹ̷�����
extern const SETPARA ADDNEEDM_D;	//�뼣����׷�ӷ���

extern volatile U16 g_ctmode;		//���ģʽ
extern volatile U16 g_addMethod;	//׷�ӷ���
extern volatile U16 g_moveCurrMethod;	//�ƶ���귽��
extern volatile U16 g_lineType;	//�ߵ�����
extern volatile U16 g_strPitch;	//ֱ�߷����
extern volatile U16 g_winPitch;	//���۷����
extern volatile U16 g_winLwidth;	//���۷�����
extern volatile U16 g_winRwidth;	//���۷��ҿ��
extern volatile U16 g_winNeedles;	//���۷�����
extern volatile U16 g_strMode;		//ֱ�߷��γ�ģʽ
extern volatile U16 g_stFirmMode;	//�ӹ̷쿪ʼģʽ	
extern volatile U16 g_stFirmNeedles;	//�ӹ̷쿪ʼ����	
extern volatile U16 g_endFirmMode;	//�ӹ̷����ģʽ	
extern volatile U16 g_endFirmNeedles;	//�ӹ̷��������
extern volatile U16 g_conConnect;		//��������
extern volatile U16 g_rotateDir;		//��ת����
extern volatile U16 g_maxPitch;		//������
extern volatile U16 g_inserBreak;		//�Ϸ���Զ�����
extern volatile U16 g_rotateAngle;		//��ת�Ƕ�
extern volatile U16 g_rotateCenter;	//��ת����
extern volatile U16 g_symMode;			//�Գ�ģʽ
extern volatile U16 g_compDir;			//��������
extern volatile U16 g_compWidth;		//�������
extern volatile U16 g_compCopys;		//����������
extern volatile U16 g_compModes;		//��������������
extern volatile U16 g_xRatio;			//x����
extern volatile U16 g_yRatio;			//y����
extern volatile U16 g_zoomCenter;		//������������
extern volatile U16 g_pinCreMode;		//��������ģʽ
extern volatile U16 g_lineType1;		//�ߵ�����1
extern volatile U16 g_codeOrder;		//����˳��������
extern volatile U16 g_chooseCont;		//ѡ���������ɵ�
extern volatile U16 g_pinMoves;		//�����ƶ���
extern volatile U16 g_attributes;		//����
extern volatile U16 g_displayCode;		//��ʾ/���ر��
extern volatile U16 g_firmRepeats;		//�ӹ̷�����
extern volatile U16 g_firmNeedlets;		//�ӹ̷�����
extern volatile U16 g_addNeedMethod;	//�뼣����׷�ӷ���
extern volatile U16 g_enableCode;		//ʹ�ܴ����־ һλһ������
extern SETPARA g_curSetPara[12];
extern volatile S16 g_xorg;			//�����xλ��
extern volatile S16 g_yorg;			//�����yλ��
extern volatile U8 g_hasStPoint;			//����������־
extern volatile U16 g_endCode;			//���������־
extern volatile U16 g_genLine;			//��ֱͨ��
extern volatile U16 g_isColse;					//���ͼ��,�ص����жϱ��
extern volatile U16 g_tattributes;		//���ҵ���ʱ����

#define		TOUPPER		0		//������һ��
#define		STONNEED	1		//����ֹͣ
#define		STUNNEED	2		//����ֹͣ
#define		PFVALID		3		//ѹ����Ч
#define		PFHEIGHT	4		//ѹ�Ÿ߶�
#define		LFVALID		5		//������Ч
#define		LFNO		6		//�������
#define		SPVALID		7		//�����ٶ���Ч
#define		SPEEDVAL	8		//�����ٶ�
#define		LOWSPEED0	9		//�������
#define		LOWSPEED1	10		//1200����
#define		LOWSPEED2	11		//800����
#define		LOWSPEED3	12		//600����
#define		LOWSPEED4	13		//400����
#define		CHOUTPUT1	14		//ѡ�����1
#define		CHOUTPUT2	15		//ѡ�����2
#define		CHOUTPUT3	16		//ѡ�����3
#define		SUSPEND1	17		//��ͣ1
#define		SUSPEND2	18		//��ͣ2
#define		SUSPEND3	19		//��ͣ3
#define		SUSPEND4	20		//��ͣ4
#define		SUSPEND5	21		//��ͣ5
#define		SUSPEND1A	22		//��ͣ1-��
#define		SUSPEND1B	23		//��ͣ1-��
#define		SUSPEND1C	24		//��ͣ1-���
#define		SUSPEND2A	25		//��ͣ2-��
#define		SUSPEND2B	26		//��ͣ2-��
#define		SUSPEND2C	27		//��ͣ2-���
#define		SUSPEND3A	28		//��ͣ3-��
#define		SUSPEND3B	29		//��ͣ3-��
#define		SUSPEND3C	30		//��ͣ3-���
#define		SUSPEND4A	31		//��ͣ4-��
#define		SUSPEND4B	32		//��ͣ4-��
#define		SUSPEND4C	33		//��ͣ4-���
#define		SUSPEND5A	34		//��ͣ5-��
#define		SUSPEND5B	35		//��ͣ5-��
#define		SUSPEND5C	36		//��ͣ5-���

extern const SETPARA TOUPPER_D;		//������һ��
extern const SETPARA STONNEED_D;	//����ֹͣ
extern const SETPARA STUNNEED_D;	//����ֹͣ
extern const SETPARA PFVALID_D;		//ѹ����Ч
extern SETPARA PFHEIGHT_D;	//ѹ�Ÿ߶�
extern const SETPARA LFVALID_D;		//������Ч
extern const SETPARA LFNO_D;		//�������
extern const SETPARA SPVALID_D;		//�����ٶ���Ч
extern const SETPARA SPEEDVAL_D;	//�����ٶ�
extern const SETPARA LOWSPEED0_D;	//�������
extern const SETPARA LOWSPEED1_D;	//1200����
extern const SETPARA LOWSPEED2_D;	//800����
extern const SETPARA LOWSPEED3_D;	//600����
extern const SETPARA LOWSPEED4_D;	//400����
extern const SETPARA CHOUTPUT1_D;	//ѡ�����1
extern const SETPARA CHOUTPUT2_D;	//ѡ�����2
extern const SETPARA CHOUTPUT3_D;	//ѡ�����3
extern const SETPARA SUSPEND1_D;	//��ͣ1
extern const SETPARA SUSPEND2_D;	//��ͣ2
extern const SETPARA SUSPEND3_D;	//��ͣ3
extern const SETPARA SUSPEND4_D;	//��ͣ4
extern const SETPARA SUSPEND5_D;	//��ͣ5 
extern const SETPARA SUSPEND1A_D;	//��ͣ1-��
extern const SETPARA SUSPEND1B_D;   //��ͣ1-��
extern const SETPARA SUSPEND1C_D;   //��ͣ1-���
extern const SETPARA SUSPEND2A_D;   //��ͣ2-��
extern const SETPARA SUSPEND2B_D;   //��ͣ2-��
extern const SETPARA SUSPEND2C_D;   //��ͣ2-���
extern const SETPARA SUSPEND3A_D;   //��ͣ3-��
extern const SETPARA SUSPEND3B_D;   //��ͣ3-��
extern const SETPARA SUSPEND3C_D;   //��ͣ3-���
extern const SETPARA SUSPEND4A_D;   //��ͣ4-��
extern const SETPARA SUSPEND4B_D;   //��ͣ4-��
extern const SETPARA SUSPEND4C_D;   //��ͣ4-���
extern const SETPARA SUSPEND5A_D;   //��ͣ1-��
extern const SETPARA SUSPEND5B_D;   //��ͣ1-��
extern const SETPARA SUSPEND5C_D;   //��ͣ1-���


extern volatile U16 g_needlsStop;		//����/����ֹͣ
extern volatile U16 g_pfValid;			//ѹ����Ч
extern volatile U16 g_pfHeight;			//ѹ�Ÿ߶�
extern volatile U16 g_lfValid;			//������Ч
extern volatile U16 g_lfNO;				//�������
extern volatile U16 g_speedValid;		//�����ٶ���Ч.
extern volatile U16 g_speeds;			//�����ٶ�
extern volatile U16 g_lowSpeed;			//����
extern volatile U16 g_chOutput1;		//ѡ�����1
extern volatile U16 g_chOutput2;		//ѡ�����2
extern volatile U16 g_chOutput3;		//ѡ�����3
extern volatile U16 g_suspend1;		//��ͣ1
extern volatile U16 g_suspend2;		//��ͣ2
extern volatile U16 g_suspend3;		//��ͣ3
extern volatile U16 g_suspend4;		//��ͣ4
extern volatile U16 g_suspend5;		//��ͣ5
extern volatile U16 g_suspend1S;		//��ͣ1״̬
extern volatile U16 g_suspend1N;		//��ͣ1���
extern volatile U16 g_suspend2S;		//��ͣ2״̬
extern volatile U16 g_suspend2N;		//��ͣ2���
extern volatile U16 g_suspend3S;		//��ͣ3״̬
extern volatile U16 g_suspend3N;		//��ͣ3���
extern volatile U16 g_suspend4S;		//��ͣ4״̬
extern volatile U16 g_suspend4N;		//��ͣ4���
extern volatile U16 g_suspend5S;		//��ͣ5״̬
extern volatile U16 g_suspend5N;		//��ͣ5���

extern SETPARA g_setcode[10];
extern volatile S16 g_areaX,g_areaY,g_areaDx,g_areaDy;	
extern volatile S16 g_rotCenX,g_rotCenY;	


void setValue(SETPARA *des,SETPARA src,U16 val,U8 flag);
void initSetPara(void);
U16 getTextAddr(U8 makeType);
U16 getSetTextAddr(SETPARA val);
char *getValUnit(SETPARA val,U8 flag);
U8 getValPoints(SETPARA val,U8 flag);
U16 getSetTextAddr1(SETPARA val);
U16 getSetTextAddr2(U16 val);
U16 getSetTextAddr3(U16 val);
U8 getDefCmds(U8 makeType,U8 doType);
U8 isCanModified(SETPARA data,U8 flag);
U16 getStep(SETPARA data,U8 flag,U16 maxstep,U8 ctime);
U8 alterSetValue(SETPARA data,U8 makeType,U8 doType,U8 index,U8 flag,U8 ctime,U8 *totalcmd);
U8 alterSetValue1(SETPARA data,U8 makeType,U8 doType,U8 index,U8 sdat,U8 *totalcmd);
U8 getDefCodes(U8 scode,U8 flag);
U16 getCodeTextAddr1(SETPARA data);
U8 isbtnChooese(SETPARA data);
U8 alterCodeValue(U8 *indat,U8 machineType,SETPARA data,U8 index,U8 flag,U8 ctime,U16 *pinNo);
U8 getSingleVal(SETPARA data);
U8 getSingleGroup(SETPARA data);
U8 getSinGroMac(U8 gno);
void setPointColor(U16 color);
void clrSelectArea(void);
void setSetectArea(U16 setno,U8 flag);
U8 isSelestArea(U16 setno);
void clrSelectCtrl(void);
void setSetectCtrl(U16 setno,U8 flag);
U8 isSelestCtrl(U16 setno);
U8 getConnetModes(U8 *indat,U16 areaNo);
void getXYpos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,U8 flag);
void getXYDrawPos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,U16 areaNo);
void getXYCtlPos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,U16 ctlNo);
void getXYPinPos(U8 *indat,U8 machineType,S16 *posx,S16 *posy,S16 *dx,S16 *dy,U8 *code,U16 *code1,U16 pinNo);
void getSetectAreaXY(U8 *indat,S16 *xt,S16 *yt,S16 *dx,S16 *dy);
void embSetBackupBlock(U16 block);
void embGetBackupBlock(U16 block);
U16 getMoveStep(U16 ll);
U8 pointIsOver(S32 xp,S32 yp,S32 xmin,S32 xmax,S32 ymin, S32 ymax);
U8 getVkeyType(U8 inno);
S32 getViewScale(U16 id,U8 flag);
void setViewScale(U16 multiple);
U8 canChangePara(U8 makeType,U8 doType,U8 changeNo);
U8 emb_areaIsClose(void);
U16 getPinNo(U8 *reg,U16 indat,U8 flag,U16 opinno);
U16 getArea(U8 *reg,U16 indat,U8 flag);
U16 getCtrl(U8 *reg,U16 indat,U8 flag);
#endif
#endif
#endif

#endif

