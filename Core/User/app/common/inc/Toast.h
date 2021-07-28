//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Bor_Toast.h							*
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

#ifndef	_TOAST_H
#define	_TOAST_H


#include "stm32cpu.h"

#define	TOAST_MSG1		0	//�������ݳ����˿ɷ��뷶Χ,�ж��˴���.
#define	TOAST_MSG2		1	//���벻��ȷ
#define	TOAST_MSG3		2	//�����벻ƥ��
#define	TOAST_MSG4		3	//���к�ճ�����ļ�����ͬ.
#define	TOAST_MSG5		4	//�洢��δ����.
#define	TOAST_MSG6		5	//����ų�����Χ,������1-999��Χ�ڵ�ֵ!
#define	TOAST_MSG7		6	//ע�����ݲ���Ϊ��!		
#define	TOAST_MSG8		7	//�ļ�������Ϊ��!	
#define	TOAST_MSG9		8	//ͨѶ����,����ͨѶ���Ƿ�����������!	
#define	TOAST_MSG10		9	//����ʧ��!
#define	TOAST_MSG11		10	//����ļ����Ѵﵽ�������.
#define	TOAST_MSG12		11	//����ȡֵ����.
#define	TOAST_MSG13		12	//�벻�����λ,���ֶ����������λ
#define	TOAST_MSG14		13	//ѹ��δ����,��̤��(��ɫ)����	
#define	TOAST_MSG15		14	//��ر�ͼ�η��빦��
#define	TOAST_MSG16	 	15	//��ѡ��ϲ���������
#define	TOAST_MSG17	 	16	// �޷���ȡ���ͼ��,�밴������ѵ�ǰ�ϲ�ͼ�����		
#define	TOAST_MSG18	 	17	// ��û��Ȩ���޸�ϵͳʱ��,���볧����ϵ
#define	TOAST_MSG19	 	18	// ��ѡ�����ļ�
#define	TOAST_MSG20 	19	// �������,���곬���ӹ���Χ.
#define	TOAST_MSG21 	20	// ����ԭ��,�޷�����,�밴��ԭ���	
#define	TOAST_MSG22 	21	// ͨ�Ź���,�����ŷ������ʺ�����
#define	TOAST_MSG23 	22	// ѡ���ļ�����,������ѡ��
#define	TOAST_MSG24 	23	// ��ѡ�������豸
#define	TOAST_MSG25 	24	// �ļ���ʽ����,��ȡʧ��
#define	TOAST_MSG26 	25	// �밴ȷ������ɵ�ǰ����
#define	TOAST_MSG27 	26	// ����ֵ���ܴ����趨ֵ
#define	TOAST_MSG28 	27	// �趨ֵ����С�ڼ���ֵ
#define	TOAST_MSG29 	28	// �༭ѭ������ǰ,��ѡ���������
#define	TOAST_MSG30 	29	// �����ļ���ԭ�ļ���ͬ,������ѡ��
#define	TOAST_MSG31 	30	// �뽫��ѹ�Ž���
#define	TOAST_MSG32 	31	// ���������12.7,�ж��˴���.
#define	TOAST_MSG33 	32	// �ڴ��ļ����ܸ��Ƶ������.
#define	TOAST_MSG34 	33	// ������ļ����ܸ��Ƶ��ڴ�.
#define	TOAST_MSG35 	34	// ͨ�Ź���,���鲽����������Ƿ�����

#define	TOAST_MSG_MAX	34

#define	TOAST_SHOW_5S	5000	

typedef struct _TOAST
{
	U8 isShow;
	U16 viewTime;
	U16 curTime;
	U16 vIndex;
	U16 bColor;
	U16 tColor;
	U16 xpos;
	U16 ypos;
	U16 xdpos;
	U16 ydpos;	
} TOAST;


void ToastInit(void);
void ToastSetBackColor(U16 color);
void ToastSetTextColor(U16 color);
void ToastShow1(U16 xpos,U16 ypos,U16 xdpox,U16 vTime);
void ToastHide(void);

extern TOAST g_Toast;

#endif


