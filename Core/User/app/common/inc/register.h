//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  register.h								*
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


#ifndef __REGISTER_H 
#define __REGISTER_H


#include "stm32cpu.h"

//---------------------------------------------------   
//	ͨ�üĴ����ռ���� D_REG U16
//---------------------------------------------------

//#define 	DREG_PMDX			0			// 64      0 - 63  X����������������		1 
//#define 	DREG_PMDY			64			// 64     64 - 127 X����������������		2
//#define 	DREG_PMDZ			128			// 64    128 - 191 CPUH����������			3 
//#define 	DREG_PMDU			192			// 64    192 - 255 CPUA����������			4
//#define 	DREG_PMDV			256			// 64    256 - 319 CPUH����������			5 
//#define 	DREG_PMDW			320			// 64    320 - 383 CPUA����������			6
//#define 	DREG_SVNM			384			// 128   384 - 511 CPUB����������			7
//#define 	DREG_VIDEO			512			// 128   512 - 639 �Ӿ�������������		8
//#define 	DREG_DISP			640			// 64    640 - 703 ���������������			9
//#define 	DREG_NETA			704			// 64    704 - 767 �����ӿ�A����������		10
//#define 	DREG_NETB			768			// 64    768 - 831 �����ӿ�B����������		11
//#define 	DREG_PBUF			832			// 1024  832 - 1855 �������ݽ�����			12
//#define 	DREG_ERROR			1856		// 256  1856 - 2111 ERROR������				13
//#define 	DREG_MSSG			2112		// 192  2112 - 2303 ϵͳ������Ϣ��¼		14
//#define 	DREG_SYS			2304		// 192  2304 - 2495 ϵͳ������				15 
//#define 	DREG_PARA			2496		// 1024 2496 - 3519 ������					16
//#define 	DREG_PT_INDEX		3520		// 3200 3520 - 6719 320*10�����ļ��б�		17
//#define 	DREG_PT_HEAD		6720		// 64   6720 - 6783 ��ǰ�����ļ�����Ϣ		18
//#define 	DREG_PT_DATA		6784		// 92072 6784 - 98855 ��������				19 		  SRAM=256K(98856=96.6k)
//#define 	DREG_PT_DATA		6784		// 140072 6784 - 146855 ��������SRAM=256K(146856=143.4k)
//#define 	DREG_BUFFLEN		98856		// 
//#define 	DREG_PT_DATA2		98856		//
//
//#define 	DREG_USERPW			98856		// 192 98856 - 99047 �û���������
//#define 	DREG_IOSETP			99048		// 192 99048 - 99239 IO����
//#define 	DREG_SRVPARA		99240		// 256 99240 - 99495 �ŷ�����
//#define 	DREG_PT_INDEX2		99496		// 42000 99496 - 141496 �����ļ��б��� 42*1000	 (��300��12600)
//
//

		
#define 	MAXEEPREG			50			//
#define 	MAXDREG				64			// D�ڴ����Ĵ���	״̬����+�ļ�������		9
#define 	MAXEREG				256			// E�ڴ����Ĵ���	������Ϣ				13
#define 	MAXMREG				192			// M�ڴ����Ĵ�����ϵͳ������Ϣ����		14
#define 	MAXYREG				192			// Y�ڴ����Ĵ���	ϵͳ��Ϣ				15
#define 	MAXPREG				1024		// P�ڴ����Ĵ���	��������				16
//#define 	MAXIREG				3200		// I�ڴ����Ĵ���	�����ļ�������			17
#define 	MAXHREG				64			// H�ڴ����Ĵ���	��ǰ�����ļ���Ϣ		18
#define 	MAXSREG				128000		// S�ڴ����Ĵ���	��������				19
#define  	MAXDIR				3200		// �ļ�Ŀ¼������
#define 	MAXOREG				64			// O�ڴ����Ĵ���	������彻������
#define 	MAXWREG				1024		// W�ڴ����Ĵ��� ip��ַ���ݺ���������
#define 	MAXIOREG			384			// IO���üĴ��� I:3*12	O:11*12  168+168 ->384	
#define 	MAXTSREG			40000		// TMP_SREG
#define 	MAXTIAPBUFF			1024		// IAPBUFF  81920
#define		MAXIREG2			999*36/*42000*/		// I2�ڴ����Ĵ���	�����ļ���������        
#define   	MAXSREG2			128000		//128000->92072(���廨����С92072,�������ʱ����,EMB�ļ���Ҫ�ϴ�)
#define  	MAXCOORD			20000		  //���������ݴ�
#define 	MAXUREG				192			//�û���Ϣ
#define 	MAXVREG				640			//�ŷ�����
#define 	MAXGREG				1280		//���ͼ������
#define 	MAXLREG				24			//���Ӽ���������

#define 	ADDEEPREG	0xc1000000				//
#define 	ADDDREG		ADDEEPREG+(MAXEEPREG*2)	// D�ڴ����Ĵ���	״̬����+�ļ�������		9
#define 	ADDEREG		ADDDREG+(MAXDREG*2)		// E�ڴ����Ĵ���	������Ϣ				13
#define 	ADDMREG		ADDEREG+(MAXEREG*2)		// M�ڴ����Ĵ�����ϵͳ������Ϣ����		14
#define 	ADDYREG		ADDMREG+(MAXMREG*2)		// Y�ڴ����Ĵ���	ϵͳ��Ϣ				15
#define 	ADDPREG		ADDYREG+(MAXYREG*2)		// P�ڴ����Ĵ���	��������				16
//#define 	ADDIREG		ADDPREG+(MAXPREG*2)		// I�ڴ����Ĵ���	�����ļ�������			17
#define 	ADDHREG		ADDPREG+(MAXPREG*2)		// H�ڴ����Ĵ���	��ǰ�����ļ���Ϣ		18
#define 	ADDSREG		ADDHREG+(MAXHREG*2)		// S�ڴ����Ĵ���	��������				19
#define 	ADDIOREG	ADDSREG+(MAXSREG*2)		// IO�ڴ����Ĵ���	IO���üĴ���	   
#define 	ADDTSREG	ADDIOREG+((MAXIOREG)*2)	// TMP_SREG
#define 	ADDIAPBUFF	ADDTSREG+((MAXTSREG)*2)	// IAPBUFF
#define 	ADDIREG2	ADDIAPBUFF+((MAXTIAPBUFF)*2)	//I�ڴ����Ĵ���	�����ļ�������
//#define		ADDDIR_DREG2   ADDIREG2+((MAXIREG2)*2)	 	// �ļ�Ŀ¼2
#define 	ADDNEWSREG	 ADDIREG2+((MAXIREG2)*2)//ADDDIR_DREG2+((MAXDIR2)*2)
#define		ADDUREG		ADDNEWSREG+(MAXSREG2*2)
#define		ADDVREG		ADDUREG+(MAXUREG*2)
#define		ADDGREG		ADDVREG+(MAXVREG*2)
#define		ADDLREG		ADDGREG+(MAXGREG*2)
#define		ADDPATCRRRD	 ADDLREG+(MAXLREG*2)
#define		ADDPATCRRRD1	ADDPATCRRRD+(MAXCOORD*4)
#define		ADDRINFPOINT	ADDPATCRRRD1+(MAXCOORD*4)

//#define		ADDUREG		ADDRINFPOINT+(MAXCOORD*2)
//#define		ADDVREG		ADDUREG+(MAXUREG*2)
//#define		ADDGREG		ADDVREG+(MAXVREG*2)

//�������ݴ��� blank1

#define 	AVI_VIDEO_BUF_SIZE    1024*60		//����avi����ʱ,��Ƶbuf��С.
#define 	AVI_AUDIO_BUF_SIZE    12		//����avi����ʱ,��Ƶbuf��С.
#define		MAXBLKSREG		128000		//���ݻ������ݼĴ���
#define		MAXBLKHREG		64			//���ݻ����ļ���Ϣ
#define		MAXBLKIREG		MAXIREG2		//���ݻ����ļ�������Ϣ 8C7C
#define 	BLKINDSIZE		6			//������Ϣ��С
#define 	MAXBLIND		512
#define   	MAXBLKBUF		0x3B8984	//�������ݻ���  0x3C1600
							
#define		ADDAVIVIDEO		0xc0800000	//���ݻ�������
#define		ADDAVIAUDIO		ADDAVIVIDEO+(AVI_VIDEO_BUF_SIZE)		
#define		ADDBLKSREG		ADDAVIAUDIO+(AVI_AUDIO_BUF_SIZE)
#define		ADDBLKHREG		ADDBLKSREG+(MAXBLKSREG*2)		//���ݻ�������
#define		ADDBLKIREG		ADDBLKHREG+(MAXBLKHREG*2)		//���ݻ�������
	
#define 	ADDBLKINDEX		ADDBLKIREG+(MAXBLKIREG*2)							//���������������ַ  
#define		ADDBLKBUF	 	ADDBLKINDEX+(MAXBLIND*2)		//�������ݻ���


#define		SST_SREG_BACKUP			ADDBLKBUF 		//SST���ݴ��޸Ļ����������ݣ��ڸ���ԭ���ݻ�������ǰʹ��
#define		SST_SREG_PARA_BACKUP	ADDBLKBUF+0xD0000

#define		SST_SREG_SADD			ADDBLKBUF+0x100000		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ���ʼ��ַ
#define		SST_SREG_EADD			SST_SREG_SADD+0xEFFFF		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ������ַ(960K)
#define		SST_SREG_MAXS			0x00078000


#define		ADDWAVE0			0xc0800000	//������ʾ�����ݱ��ݵ�ַ	
#define		MAXWAVE0_X			0x3CA50
#define		MAXWAVE0_Y			0x3CA50
#define		ADDWAVE0_X			ADDWAVE0
#define		ADDWAVE0_Y			ADDWAVE0+MAXWAVE0_X


#define 	ADDWAVE1			ADDWAVE0+MAXWAVE0_X+MAXWAVE0_Y	//��������
#define 	MAXWAVE1			0x400000-MAXWAVE0_X-MAXWAVE0_Y

//#define		ADDWAVE0			0xc0800000	//������ʾ�����ݱ��ݵ�ַ


//#define 	ADDWAVE1			0xc0800000	//��������
//#define 	MAXWAVE1			0x400000

//#define		SST_SREG_SADD			0xC0900000		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ���ʼ��ַ
//#define		SST_SREG_EADD			0xC09EFFFF		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ������ַ(960K)
//#define		SST_SREG_MAXS			0x00078000


//#define		SST_SREG_BACKUP			0xC0800000 		//SST���ݴ��޸Ļ����������ݣ��ڸ���ԭ���ݻ�������ǰʹ��
//#define		SST_SREG_PARA_BACKUP	0xC08D0000


/******************* 3���Ĵ�����дģ����� **************************/
#define		PMDX				1			// X����������������
#define 	PMDY				2			// Y����������������
#define 	PMDZ				3			// ��ѹ����������������
#define 	PMDU				4			// U����������������
#define 	PMDV				5			// V����������������
#define 	PMDW				6			// W����������������
#define 	SVNM				7			// ������������������
#define 	VIDEO				8			// �Ӿ�������������
#define 	DISP				9			// ���������������
#define 	NETA				10			// �����ӿ�A����������
#define 	NETB				11			// �����ӿ�B����������
#define 	PAGEBUF				12			// �������ݽ�����
#define 	ERRO				13			// ������Ϣ
#define 	MSSG				14			// ϵͳ������Ϣ��¼
#define 	SYS					15			// ϵͳ״̬��Ϣ
#define 	PARA				16			// ��������
#define 	INDEX				17			// �����ļ�������
#define 	HEAD				18			// ��ǰ�����ļ���Ϣ
#define 	PTDATA				19			// ��ǰ�����ļ�����
#define 	USERLOG				20			// �û�������
#define 	IOPARA				21			// IO����
#define 	SRVPARA				22			// �ŷ�����
#define 	INDEX2				23			// �����ļ�������
#define 	GROUP				24			// ͼ�����
#define 	LFPARA				25			// ���Ӽ���������
/******************* 3���Ĵ�����дģ����� **************************/

/******************* 3���Ĵ�����дģ����ʼ��ַ **************************/
#define 	D_PMDX				0			// X����������������ʼ��ַ
#define 	D_PMDY				64			// Y����������������ʼ��ַ
#define 	D_PMDZ				128			// ��ѹ����������������ʼ��ַ
#define 	D_PMDU				192			// U������������������ʼ��ַ
#define 	D_PMDV				256			// V������������������ʼ��ַ
#define 	D_PMDW				320			// W������������������ʼ��ַ
#define 	D_SVNM				384			// ��������������������ʼ��ַ
#define 	D_VIDEO				512			// �Ӿ���������������ʼ��ַ
#define 	D_DISP				640			// �����������������ʼ��ַ
#define 	D_NETA				704			// �����ӿ�A������������ʼ��ַ
#define 	D_NETB				768			// �����ӿ�B������������ʼ��ַ
#define 	D_PAGEBUF			832			// �������ݽ�������ʼ��ַ
#define 	D_ERRO				1856		// ������Ϣ��ʼ��ַ
#define 	D_MSSG				2112		// ϵͳ������Ϣ��¼��ʼ��ַ
#define 	D_SYS				2304		// ϵͳ״̬��Ϣ��ʼ��ַ
#define 	D_PARA				2496		// ����������ʼ��ַ
#define 	D_INDEX				3520		// �����ļ���������ʼ��ַ
#define 	D_HEAD				6720		// ��ǰ�����ļ���Ϣ��ʼ��ַ
#define 	D_PTDATA			6784		// ��ǰ�����ļ�������ʼ��ַ
/******************* 3���Ĵ�����дģ����ʼ��ַ **************************/

#define     IOREGHEAD           0
#define 	IOREGCHECK          1
#define 	INSETREGI			2//12*2*3
#define 	OUTSETREGI			74//11*2*12
#define 	IOSETREGI			338

#define 	INSETREGLEN			3
#define 	OUTSETREGLEN		11

#define 	WAITTIME1		15 	
#define 	WAITTIME		10 					// ��������ʱ
#define 	WAITTIME2		30 					// ������U��������ʱ
#define 	WAITTIME3		1 					// �������ż��ʱ��

#define		FORTIMS1		100					//ѭ����ѯ����U�̴���

#define 	WAIT_T		500 					// ��������ʱ
#define 	RD_T		10000					// ���������ݵĴ���

#define 	RD_T_DISK		240000

#define 	FREE			0
#define 	RSUCCEED		1
#define 	WSUCCEED		2
#define 	ERROR			3
#define 	OVERTIME		4
#define 	BUSY			5

//#define NOMEMORY    0
//#define USB     	1
//#define SD     	 	2
//#define FDC     	3

#define USB_FLAG     0x01
#define SD_FLAG      0x02
#define FDC_FLAG     0x04



/************* 15ϵͳ״̬��Ϣ������15����ʼ�Ĵ���D2304��D2495��192���Ĵ��� ***********/
// ϵͳ��Ϣ�Ĵ��� begin
#define		SYS_WP			0					// 512���±���λ.��һ��д��54321���д,�ϵ���Զ�����
#define		SYS_ID1			1					// ID1
#define		SYS_ID2			2					// ID2
#define		SYS_HVER		3					// Ӳ���汾��
#define		SYS_SVER		4					// ����汾��
#define		SYS_WT1			5					// ����ʱ��
#define		SYS_WT2			6					// ����ʱ��
#define		SYS_KEY1		7					// ��Կ1
#define		SYS_KEY2		8					// ��Կ2
#define		SYS_MS			9					// bit0-7��0-59, bit8-15����0-59��ʮ����4bitΪһ������
#define		SYS_WH			10					// bit0-7Сʱ0-24,bit8-15����0-6��ʮ����4bitΪһ������	D7=12/24
#define		SYS_MD			11					// bit0-7��1-31,bit8-15��1-12��ʮ����4bitΪһ������
#define		SYS_Y			12					// bit0-7��0-99,ʮ����4bitΪһ������
#define		SYS_RETURN_MD			13			// bit0-7��1-31,bit8-15��1-12��16����		
#define		SYS_RETURN_Y			14			// bit0-7��0-99, BIT8 : 1 Ϊ��Ч�� 0 Ϊ������
#define		ID_CODE1		15					// ID �汾�Ÿ�λ
#define		ID_CODE2		16					// ID �汾�ŵ�λ

#define		SYS_WIFI_VER			18		//WIFI�汾��
#define		SYS_FLASH_VER			19		//FLASH�汾��
#define		SYS_MACHINE_TYPE			20		// ����

#define		SYS_MAINBOARD_VER			21		//����汾��
#define		SYS_MAINBOARD_CODE1			22		//�����ŵ�λ
#define		SYS_MAINBOARD_CODE2			23		//�����Ÿ�λ

#define		SYS_OPERA_VER				24		//�ŷ���汾��
#define		SYS_OPERA_CODE1			25		//�������ŵ�λ
#define		SYS_OPERA_CODE2			26		//�������Ÿ�λ
#define		SYS_DSP_VER				27		//������汾��


#define		SYS_IP_CODE1			30		//ip�Ÿ���λ
#define		SYS_IP_CODE2			31		//ip�ŵ���λ
#define		SYS_MACHINE_NAME1		32		//������
#define		SYS_MACHINE_NAME2		33		
#define		SYS_MACHINE_NAME3		34		
#define		SYS_MACHINE_NAME4		35

#define		SYS_MACHINE_TYPEC1		36		// ����ǰ����ĸ
#define		SYS_MACHINE_TYPEC2		37		// ���ͺ�����ĸ


#define		SYS_PEDAL_V				53	//̤���ѹ
#define		SYS_PEDAL_VMIN			56	//̤���ѹ��Сֵ
#define		SYS_PEDAL_VMAX			57	//̤���ѹ���ֵ
#define		SYS_CAR_OUTI			54	//��ͷ���������
#define		SYS_ARA_I				58	//����ֵ
#define		SYS_ARA_S				59	//��������

#define		SYS_TEMPERATURE			139	//�¶�

#define		FGPARA_ADDR				236448	//�۴��������õ�ַ

// ϵͳ��Ϣ�Ĵ��� end


// ϵͳ��������		 begin
#define		SYS_PARA1CHKSUM	         60 	// �����ת��������У��         
#define		SYS_PARA1X1		         61 	// �����ת��������_90����������X 
#define		SYS_PARA1Y1		         62 	// �����ת��������_90����������Y 
#define		SYS_PARA1X2		         63 	// �����ת��������_180����������X
#define		SYS_PARA1Y2		         64 	// �����ת��������_180����������Y
#define		SYS_PARA1X3		         65 	// �����ת��������_270����������X
#define		SYS_PARA1Y3		         66 	// �����ת��������_270����������Y
#define		SYS_PARA1XR		         67 	// �����ת��������_Բ������X     
#define		SYS_PARA1YR		         68 	// �����ת��������_Բ������Y     
#define		SYS_PARA1RD		         69 	// �����ת��������_�뾶          
#define		SYS_PARA1ANG	         70 	// �����ת��������_0��λ�ýǶ�   
#define		SYS_PARA2CHKSUM	         71		// ��еԭ����������У��           
#define		SYS_PARA2XORG	         72 	// ��еԭ����������_��������X     
#define		SYS_PARA2YORG	         73 	// ��еԭ����������_��������Y  
#define		SYS_PARA2PFOOT         	 74 	// ��ѹ�Ų���_����   
#define		SYS_PARA3CHKSUM	         75  	// ����ת��϶��������У��       
#define		SYS_PARA3XZADJ	         76  	// ����ת��϶��������_��������X+
#define		SYS_PARA3XFADJ	         77  	// ����ת��϶��������_��������X-
#define		SYS_PARA3YZADJ	         78  	// ����ת��϶��������_��������Y+
#define		SYS_PARA3YFADJ	         79 	 // ����ת��϶��������_��������Y-
#define		SYS_PARA2XDORG	         81 	// ��еԭ��D����������_��������X     
#define		SYS_PARA2YDORG	         82 	// ��еԭ��D����������_��������Y  

#define		SYS_PARA3MODE	         84 	// ����ת��϶��������_�޸�ģʽ
#define		SYS_PARA3SPEED	         85 	// ����ת��϶��������_�޸��ٶ�

#define		SYS_PARA4X	        	 87 	// �Ӿ�����ϵX����
#define		SYS_PARA4Y       	 	 88 	// �Ӿ�����ϵY����

#define		SYS_TSTEPPER			98					// ���������������
#define		SYS_FIXTURE				99				// �о�״̬ 0-�ɿ� 1-�н� 2-�Զ�	
// ϵͳ��������		 end

// ״̬�Ĵ���
#define		SYS_MSTU		100					// ��������״̬			r		0ֹͣ 1���� 2ƽ�� 3����
// ��������
#define		SYS_MREN		101					// ÿת����		r	w	���������һ��������
#define		SYS_MRCP		102					// ÿת����		r	w	����ÿתӦ��������
#define		SYS_MANG		103					//       	r	w	���ᵱǰ�Ƕ� 0-360
#define		SYS_MRST		104					// 				r	w	����CHZ��λ��������=P_MS8*SYS_MREN/360 0-4000
#define		SYS_MENS		105					// 				r	w	����ֹͣ��ʼ�� 0-360
#define		SYS_MENT		106					// 				r	w	����ֹͣ��	   0-360
#define		SYS_MENE		107					// 				r	w	����ֹͣ��ֹ�� 0-360
// ��������Ĵ���
#define		SYS_MSTP		108					// ֹͣ����		r	w	0:ֹͣ
#define		SYS_MRUN		109					// ��������		r	w	0:�������1����2��λ8CW 9CCW
#define		SYS_MDIS		110					// ���нǶ�		r	w	��
#define		SYS_MLSP		111					// ���ٶ�		r	w	rpm
#define		SYS_MHSP		112					// ����ٶ�		r	w	rpm
#define		SYS_MSSP		113					// ֹͣ�ٶ�		r	w	rpm
#define		SYS_MUSP		114					// ���ٶ�		r	w	rpm
#define		SYS_MDSP		115					// ���ٶ�		r	w	rpm

#define		SYS_XENCODER				119				//X�������
#define		SYS_YENCODER				126				//Y�������
#define		SYS_ZENCODER				127				//Z�������

#define 	SYS_PFOOTNOW		120				//��ǰ��ѹ�Ÿ߶�		2009.7.16
#define 	SYS_PFOOTSET		121				//�趨��ѹ�Ż�׼�߶�	2009.7.16

#define 	SYS_LFORCESET		124				//����������׼ֵ	2013.4.1
#define 	SYS_LFORCENOW		125				//����������ǰֵ	2013.4.1

#define 	SYS_TEST1D			130				//���������1
#define 	SYS_TEST2D			131				//���������2	
#define 	SYS_UDZHOU			132				//�����������ͱ�־λ
#define 	SYS_DNANGLE			133				//�������ᵱǰ�Ƕ� 0-360

#define    	SYS_LISTTYPE		136  //�����ڴ���������  0�������ڴ� 1������U��  3������



#define		SYS_IOCONCT		144					// ioģ������
#define		SYS_SPEED1		143					// ��ǰ�ٶ�(�ֵ�ϵͳ)
#define		SYS_STATUS1		144					// �ڶ�״̬�Ĵ���


#define		SYS_INLEVEL			146	// ����ڵ�ƽ�ź�
#define		SYS_OUTLEVEL		152// ����ڵ�ƽ�ź�

#define		SYS_EXINPUT			158	// ��չ�����
#define		SYS_EXOUTPUT		159// ��չ�����


#define		SYS_INPUTL		160	// ����ڵ�ǰ״̬��λ
#define		SYS_INPUTH		161// ����ڵ�ǰ״̬��λ
#define		SYS_OUTPUTL	162	// ����ڵ�ǰ״̬��λ
#define		SYS_OUTPUTH	163	// ����ڵ�ǰ״̬��λ

#define		SYS_CHANGENEEDLE	164	// ��������
#define		SYS_RUN_TIMER		165		//�ӹ�ʱ��
#define		SYS_STOP_TIMER		166		//ֹͣʱ��

#define		SYS_GROUP_TYPE		167		//ͼ���������  1-40 OR 100  ʹ�����ͼ��  ����ʹ������ͼ��
#define		SYS_GROUP_NO		168		//���ͼ�α��
#define		SYS_GROUP_NUMS		169		//���ͼ�λ�������

#define		SYS_WINDSP		172					// �����ٶ�
#define		SYS_WINDTS		173					// ����ʱ��
#define		SYS_MAX_X		174					// ��������ƶ�X
#define		SYS_MAX_Y		175					// ��������ƶ�Y
#define		SYS_MAX_XL		176					// ��ǰX�������
#define		SYS_MAX_XR		177					// ��ǰX����Ҽ���
#define		SYS_MAX_YU		178					// ��ǰY����ϼ���
#define		SYS_MAX_YD		179					// ��ǰY����¼���
#define		SYS_ENABLE		180					// ��ֹ״̬
#define		SYS_STATUS		181					// ����״̬   
#define		SYS_SPEED		182					// ��ǰ�ٶ�
#define		SYS_TEMPER		139					// �¶�			183->139			2017-4-18
#define		SYS_POWERV		140					// ��ѹ			184->140			2017-4-18
#define		SYS_UPCOUNT		183					// ���㵱ǰֵ	185->[183,184]		2017-4-18
#define		SYS_DNCOUNT		185					// ���㵱ǰֵ	186->[185,186]		2017-4-18
#define		SYS_TSERVO		187					// ˽����������
#define		SYS_FSIZE		188					// �ڴ�������
#define		SYS_FUSEED		189					// ��ʹ������
#define		SYS_SUMFILE		190					// ��ʹ������
#define		SYS_COMEN		191					// ͨѶ����

#define		C_IAPCOMM				212					//iap��������

//ϵͳ״̬��Ϣ D_STATUS  SYS_STATUS ����״̬�Ĵ���1
#define		S_RUN				YREG[SYS_STATUS]&bit0		// ����/ֹͣ
#define			S_RUNSET()		YREG[SYS_STATUS]|=bit0	// 
#define			S_RUNCLR()		YREG[SYS_STATUS]&=~bit0	// 
#define		S_HOME				YREG[SYS_STATUS]&bit1		// �ѻ�ԭ��/��ԭ��
#define			S_HOMESET()		YREG[SYS_STATUS]|=bit1	// 
#define			S_HOMECLR()		YREG[SYS_STATUS]&=~bit1	//
#define		S_NEWHOME			YREG[SYS_STATUS]&bit2		// �ѻ�ԭ��/��ԭ��
#define			S_NEWHOMESET()	YREG[SYS_STATUS]|=bit2	// 
#define			S_NEWHOMECLR()	YREG[SYS_STATUS]&=~bit2	// 
#define		S_CLAMP				YREG[SYS_STATUS]&bit3		// ��ѹ������/�½�
#define			S_CLAMPSET()	YREG[SYS_STATUS]|=bit3	// 
#define			S_CLAMPCLR()	YREG[SYS_STATUS]&=~bit3	// 
#define		S_PFOOT				YREG[SYS_STATUS]&bit4		// ��ѹ������/�½�
#define			S_PFOOTSET()	YREG[SYS_STATUS]|=bit4	// 
#define			S_PFOOTCLR()	YREG[SYS_STATUS]&=~bit4	// 
#define		S_NEEDLE			YREG[SYS_STATUS]&bit5		// ������/�½�
#define			S_NEEDLESET()	YREG[SYS_STATUS]|=bit5	// 
#define			S_NEEDLECLR()	YREG[SYS_STATUS]&=~bit5	//
#define		S_LIMIT				YREG[SYS_STATUS]&bit6		// ���곬���ӹ���Χ
#define			S_LIMITSET()	YREG[SYS_STATUS]|=bit6	// 
#define			S_LIMITCLR()	YREG[SYS_STATUS]&=~bit6	//
#define		S_2HD				YREG[SYS_STATUS]&bit7		// ��ԭ��λ��
#define			S_2HDSET()		YREG[SYS_STATUS]|=bit7	// 
#define			S_2HDCLR()		YREG[SYS_STATUS]&=~bit7	//  
#define		S_UPDATEPARA		YREG[SYS_STATUS]&bit8		// ��������
#define			S_UPDATEPARASET()	YREG[SYS_STATUS]|=bit8	// 
#define			S_UPDATEPARACLR()	YREG[SYS_STATUS]&=~bit8	//
#define		S_WIND				YREG[SYS_STATUS]&bit9		// ����
#define			S_WINDSET()		YREG[SYS_STATUS]|=bit9	// 
#define			S_WINDCLR()		YREG[SYS_STATUS]&=~bit9	//
#define		S_SHIELDMSG			YREG[SYS_STATUS]&bit11		// ���α���������ʾ��Ϣ
#define			S_SHIELDMSGSET() 	YREG[SYS_STATUS]|=bit11// 
#define			S_SHIELDMSGCLR() 	YREG[SYS_STATUS]&=~bit11// 
#define		S_2HOME				YREG[SYS_STATUS]&bit13		
#define			S_2HOMESET()		YREG[SYS_STATUS]|=bit13	// 
#define			S_2HOMECLR()		YREG[SYS_STATUS]&=~bit13	//
#define		S_LEDF				YREG[SYS_STATUS]&bit14		// LED
#define			S_LEDFSET()		YREG[SYS_STATUS]|=bit14	// 
#define			S_LEDFCLR()		YREG[SYS_STATUS]&=~bit14//
#define		S_ERROR				YREG[SYS_STATUS]&bit15		// ����
#define			S_ERRORSET()	YREG[SYS_STATUS]|=bit15	// 
#define			S_ERRORCLR()	YREG[SYS_STATUS]&=~bit15//

#define		S_MCLAMP			YREG[SYS_STATUS1]&bit1		// Сѹ������/�½�	
#define		S_MCLAMPSET()		YREG[SYS_STATUS1]|=bit1	 
#define		S_MCLAMPCLR()		YREG[SYS_STATUS1]&=~bit1

#define		S_BMOLD				YREG[SYS_STATUS1]&bit2		// ��ģ����
#define		S_BMOLDSET()		YREG[SYS_STATUS1]|=bit2	 
#define		S_BMOLDCLR()		YREG[SYS_STATUS1]&=~bit2

#define		S_FPLATE			YREG[SYS_STATUS1]&bit3		// ���ϰ�����/�½�	
#define		S_FPLATESET()		YREG[SYS_STATUS1]|=bit3	 
#define		S_FPLATECLR()		YREG[SYS_STATUS1]&=~bit3

#define		S_DEMOE				YREG[SYS_STATUS1]&bit4		// ��ʾ
#define		S_DEMOSET()			YREG[SYS_STATUS1]|=bit4	 
#define		S_DEMOCLR()			YREG[SYS_STATUS1]&=~bit4
// D_ENABLE ϵͳʹ��״̬��Ϣ
#define		E_RUN				YREG[SYS_ENABLE]&bit0		// ����
#define			E_RUNEN()		YREG[SYS_ENABLE]|=bit0	// 
#define			E_RUNDIS()		YREG[SYS_ENABLE]&=~bit0	// 
#define		E_HOME				YREG[SYS_ENABLE]&bit1		// ��ԭ��
#define			E_HOMEEN()		YREG[SYS_ENABLE]|=bit1	// 
#define			E_HOMEDIS()		YREG[SYS_ENABLE]&=~bit1	// 
#define		E_CLAMP				YREG[SYS_ENABLE]&bit2		// ��ѹ��
#define			E_CLAMPEN()		YREG[SYS_ENABLE]|=bit2	// 
#define			E_CLAMPDIS()	YREG[SYS_ENABLE]&=~bit2	// 
#define		E_PFOOT				YREG[SYS_ENABLE]&bit3		// ��ѹ��
#define			E_PFOOTEN()		YREG[SYS_ENABLE]|=bit3	// 
#define			E_PFOOTDIS()	YREG[SYS_ENABLE]&=~bit3	// 
#define		E_NEEDLE			YREG[SYS_ENABLE]&bit4		// ��
#define			E_NEEDLEEN()	YREG[SYS_ENABLE]|=bit4	// 
#define			E_NEEDLEDIS()	YREG[SYS_ENABLE]&=~bit4	// 
#define		E_WIND				YREG[SYS_ENABLE]&bit5		// ���� ��ֹ����
#define			E_WINDEN()		YREG[SYS_ENABLE]|=bit5	// 
#define			E_WINDDIS()		YREG[SYS_ENABLE]&=~bit5	//

#define		E_IOTEST			YREG[SYS_ENABLE]&bit6		// ����������� ��ֹ����+++++
#define			E_IOTESTEN()	YREG[SYS_ENABLE]|=bit6	// 
#define			E_IOTESTDIS()	YREG[SYS_ENABLE]&=~bit6	//

#define		E_WORKSHOP			YREG[SYS_ENABLE]&bit7		// �������� ��ֹ����+++++
#define			E_WORKSHOPEN()	YREG[SYS_ENABLE]|=bit7	// 
#define			E_WORKSHOPDIS()	YREG[SYS_ENABLE]&=~bit7//
/************* 15ϵͳ״̬��Ϣ������15����ʼ�Ĵ���D2304��D2495��192���Ĵ��� ***********/



/*************************** 9 ������������ݼĴ��� ********************************/
// 9 ������������ݼĴ���
//#define		D_STATUS		0					// ����״̬
//#define		D_ENABLE		1					// ��ֹ״̬

#define		D_MODE			2						// ģʽ
#define		D_RUN			3						// ����״̬
#define		D_RUNEN			4						// ��������
//#define		D_SPEED			5						// �ٶ�			0-9
//#define		D_TEMPER		6						// �¶�			-50-100
//#define		D_POWERV		7						// ��Դ��ѹ		0-300
#define		D_MSPEED		8						// ����ת��		0-3000
#define		D_UPCOUNT1		9						// UPCOUNT		0-9999
//#define		D_UPCOUNT2		10						// UPCOUNT		0-9999
#define		D_ERRWAR		10						// ���巢���ı��� ������Ϣ
#define		D_DNCOUNT1		11						// DNCOUNT		0-9999
#define		D_DNCOUNT2		12						// DNCOUNT		0-9999
#define		D_MERR1			13						// M�����1
#define		D_MERR2			14						// M�����1
#define		D_DERR1			15						// D�����1
#define		D_XERR1			16						// X�����1
#define		D_YERR1			17						// Y�����1
#define		D_FILESTU		18						// �����ļ�״̬
#define		D_FILEPOS		19						// �����ļ���ǰ��ʾλ��

#define		D_XERR2			58						// ����X�����1
#define		D_YERR2			59						// ����Y�����1
#define		D_ZERR2			60						// ����Z�����1
#define		D_UERR2			61						// ����U�����1
#define		D_VERR2			62						// ����V�����1

#define		D_COMMAND		20						// ָ��
#define		D_CMDSTATUS		21						// ָ��״̬
#define		D_CMDPARA1		22						// ָ�����1
#define		D_CMDPARA2		23						// ָ�����2
#define		D_CMDPARA3		24						// ָ�����3
#define		D_CMDPARA4		25						// ָ�����4
#define		D_CMDPARA5		26						// ָ�����5
#define		D_CMDPARA6		27						// ָ�����6
#define		D_CMDPARA7		28						// ָ�����7
#define		D_CMDPARA8		29						// ָ�����8

#define		D_MS			30					// bit0-7��0-59, bit8-15����0-59��ʮ����4bitΪһ������
#define		D_WH			31					// bit0-7Сʱ0-24,bit8-15����0-6��ʮ����4bitΪһ������	D7=12/24
#define		D_MD			32					// bit0-7��1-31,bit8-15��1-12��ʮ����4bitΪһ������
#define		D_Y				33					// bit0-7��0-99,ʮ����4bitΪһ������

#define		D_PGROP			54				//�����趨����
//дflash�ݴ�Ĵ���
#define		PAG_COPY_FINO			0

// 9 ������������ݼĴ���֮�·�ָ��  D_COMMAND  D20 COMMAND    

/*#define		C_RUN_DEN		1						// ��ֹ����
#define		C_HOME_DEN		2						// ��ֹ��ԭ��
#define		C_CLAMP_DEN		3						// ��ֹ��ѹ��ָ��
#define		C_GOHOME		4						// ��ԭ��ָ��
#define		C_PFOOT			5						// ��ѹ�������½�ָ��
#define		C_NEEDLE		6						// �������½�ָ��
#define		C_NEWHOME		7						// ��ԭ��ָ��
#define		C_SPEEDUP		8						// ����ָ��
#define		C_SPEEDDN		9						// ����ָ��
#define		C_JOGUP			10						// ���߼�ָ��
#define		C_JOGDN			11						// ���߼�ָ��*/

#define		C_YABANDOWN				15				//ѹ���½�
#define		C_COMNECTION_OK		16				//�����������
#define		C_YABANUP				17				//ѹ��̧��
#define		C_GOHOME_PRESS_ONT		18			// ��ԭ��ָ��,ѹ�岻����
#define		C_NEWHOME_PRESS_ONT		19				//��ԭ��,ѹ�岻����
#define		C_GOHOME		20						// ��ԭ��ָ��,ѹ�嶯��
#define		C_NEWHOME		21						// ��ԭ��ָ��,ѹ�嶯��
#define		C_PFOOTUP		22						// ��ѹ������ָ��
#define		C_PFOOTDN		23						// ��ѹ���½�ָ��
#define		C_NEEDLEUP		24						// ������ָ��
//#define		C_NEEDLEDN		25						// ���½�ָ��
#define		C_ENCODE_ANGLE		25					// �������Ƕ�
#define		C_SPEEDUP		26						// ����ָ��
#define		C_SPEEDDN		27						// ����ָ��
#define		C_JOGUP			28						// ���߼�(���)ָ��
#define		C_JOGDN			29						// ���߼�(���)ָ��
#define		C_ABSJOGUP		30						// ���߼�(����)ָ��
#define		C_ABSJOGDN		31						// ���߼�(����)ָ��
#define		C_ABSLTNSSMOVE		32						// �����ƶ�ָ��
#define		C_RELATIONMOVE		33						// ����ƶ�ָ��
#define		C_READPOSI		34						// ��ȡ��ǰλ��
#define		C_RDCMDSTATUS	35						// ��ȡָ��ִ�����
#define		C_RDCMDSTATUS	35						// ��ȡָ��ִ�����
#define		C_WRITESTATUS	36						// д״̬�Ĵ���
#define		C_XYMOFFLINE	37						// xym���ѻ�ָ��
#define		C_WRITE_VER		38						// д������汾��
#define		C_NEEDLE_UP		39					// ������
#define		C_JOGUPCON		133						// �������߼�(���)ָ��
#define		C_JOGDNCON		134						// �������߼�(���)ָ��
#define		C_JOGSTOP		135						// ��ͣ������

#define		C_SPEEDUP1		136						// ����ָ��(�ֵ�ϵͳ)
#define		C_SPEEDDN1		137						// ����ָ��(�ֵ�ϵͳ)

#define		C_SPEEDDN1		137						// ����ָ��(�ֵ�ϵͳ)

#define		C_RDMEMPATLIST		40						// ���ڴ��еĻ����ļ�������
#define		C_RDFLAPATLIST		41						// �������еĻ����ļ�������
#define		C_RDMEMNAMELIST	42						// �ض��ڴ��е��ļ���
#define		C_RDFLANAMELIST	43						// �ض������е��ļ���
#define		C_READMEMORYPAT	44						// ���ڴ��л����ļ�
#define		C_READFLASHPAT		45						// �������л����ļ�
#define		C_DELMEMORYPAT		46						// ɾ���ڴ��л����ļ�
#define		C_DELFLASHPAT		47						// ɾ�������л����ļ�
#define		C_WRITEMEMORYPAT	48					// д�ڴ��л����ļ�
#define		C_WRITEFLASHPAT	49						// д�����л����ļ�
#define		C_UPDATEPARA	50						// ������д�����������浽�ڴ�-����ΪĬ��ֵ
#define		C_READFSIZE		51						// ���ڴ�ʹ����Ϣ

#define		C_READMEMNUMS	57						// ���ڴ��ļ���
#define		C_PATUPDATE		58						// �����ڴ滨��
//////////////////2011.7.21 �����˵�ǰ�ӹ�ͼ�ζκ����Ӽ�ָ��
#define		C_UPSREG			52						//up
#define		C_DOWNSREG			53						//dn
//////////////////////////////////////////////////////////

#define		C_ZD			59

#define		C_COPYMEMORYPAT		60						// �����ڴ��л����ļ�������
#define		C_COPYFLASHPAT			61						// ���������л����ļ����ڴ�
#define		C_BACKUPPARA			62						// �����������������
#define		C_RECOVERYPARA			63						// �����ָ̻�����������
#define		C_BACKUPPROG			64						// ����������Ƴ�������
#define		C_RECOVERYPROG			65						// �����ָ̻����Ƴ�������
#define     C_DISK_STATUS			66						//���ⲿ�洢��״̬��־ 100: �� 101: usb  102:sd  103: ����	
#define		C_PFOOTACTION			67						//������ѹ�ŵĸ߶ȣ�����ѹ������ʱ��	2009.7.31	

#define		C_PFEST				68			//������ѹ�Ż���趨ֵ
#define		C_PFEMOV			69			//��ѹ�ŵ������

#define		C_IOOUTPUT_CHECK		70				//io���������
#define		C_EXCODE_OUTPUT			71				//���ӹ������
#define		C_NEEDLE_UTTER_ANGLE		72			//����ԽǶ�
#define		C_NEEDLE_COM_ANGLE		73			//����ԽǶ�
#define 	C_SET_TIMER			74					//����ʱ��
#define 	C_XYTEST 			75	    //����XY��
#define 	C_CUT 				76		//����

#define		C_READBARCODE			77						//��������
#define		C_SETBARCODE			78						//д������

#define		C_SETLFORCE			80						//��������������׼ֵ
#define		C_WORKSHOP_EN			81						//�Ѿ�����IPָ��
#define		C_RDWORKSHOPLIST		82					//��Զ�����������ļ�����ָ��
#define		C_RWDWORKSHOPFILE		83					//��дԶ�̻����ļ�ָ��
#define		C_SETIP					84					//�޸�ipָ��
#define		C_RECEIVE_FILE			85					//ȷ�Ͻ���Զ���ļ�ָ��
#define		C_LEDONOFF				86					//LED�ƿ���

#define		C_ORGMODIFY				87					//ԭ������
#define		C_GAPMODIFY				88				   //������϶����
#define		C_PINMODIFY				89				   //�����ת����
#define		C_DORGMODIFY			91					//D��ԭ������
#define		C_VIEWMODIFY			141					//�Ӿ�����ϵ����

#define		C_BCLAMP				142			//��ѹ��
#define		C_MCLAMP				143			//Сѹ��
#define		C_ECLAMP				144			//������
#define		C_PCLAMP				145			//���ΰ�

#define		C_THERAD				90				   //����ģʽ
#define		C_QUIT_THERAD			132				   //�˳�����ģʽ

#define		C_ABSLTNSSMOVE_BOR		92						// �����ƶ�ָ��ֵܿ
#define		C_RELATIONMOVE_BOR		93						// ����ƶ�ָ��ֵܿ

#define		C_SAVEPW				94				//��������
#define		C_USRLOG				95				//�û���¼

#define		C_SAVEIO				96				//����IO����

#define		C_READSERVO				97				//��ȡ�ŷ�����
#define		C_SAVESERVO				98				//�����ŷ�����
#define		C_RECOVESERVO			99				//�ָ��ŷ�����

#define		C_READSTEPPER			138				//��ȡ�����������
#define		C_SAVESTEPPER			139				//���沽���������
#define		C_RECOVESTEPPER			140				//�ָ������������

#define 	C_FIXTURE 				124				//�о�
#define		C_YABANDN				125				//ѹ��ѹ��(����1:0-���� 1-�½�)
#define		C_PFSTEP				127				//һ�ζ���ѹ���г�

#define		C_DOWNLOAD_NETCODE		197					//Զ��������
#define		C_DOWNLOAD_FLASH		198					//���������flash����

#define		C_SYSRECOVER			199						//ϵͳ������ԭ����
#define		C_FORMATMEMORY			200						// ��ʽ������ʼ�����ڴ�
#define		C_FORMATFLASH			201						// ��ʽ������

#define		C_SETPASSSWORD			202						//����ʹ��Ȩ������
#define		C_MCPURESTART			230						//��������

#define		C_READGROUP				100				//��ȡ���ͼ������
#define		C_SAVEGROUP				101				//�������ͼ������
#define		C_OPENGROUP				102				//�����ͼ������
#define		C_DELGROUP				103				//ɾ�����ͼ������
#define		C_SWITCHGROUP			104				//�л����ͼ������

#define		C_CLRAIARM				105				//�������
#define		C_CLRRUNRECORD			106				//������м�¼

#define		C_REINPUTSETTING		107				//��ԭIO�����趨
#define		C_REOUTPUTSETTING		108				//��ԭIO����趨

#define		C_SETPINS				112				//���õ�ǰ�ƶ�����
#define		C_CHANGENEEDLE			113				//����

#define		C_REFRESH_ERRRECORD		114				//ˢ�´����¼
#define		C_REFRESH_RUNRECORD		115				//ˢ�����м�¼

#define		C_ANALOG_SET			116				//ģ��������

#define		C_READ_RUNRECORD		117				//��������¼

#define		C_READ_WIFI				118				//��wifi��Ϣ
#define		C_WRITE_WIFI			119				//дwifi��Ϣ

#define		C_NEWORD_CHAT			121				//����Ự
#define		C_SAVE_MPROG			126				//��������޸Ĳ���

#define		C_TRIM_OUT				128			//�������

#define		C_PIN_ANG				129			//��Ƕ�
#define		C_SET_PARA1				130			//���ò���һ
#define		C_SET_PARA2				131			//���ò���һ

#define		C_FB_STEP0				145			//��ģ����
#define		C_FB_STEP1				146			//�۱�����ѹ
#define		C_FB_STEP2				147			//����һ
#define		C_FB_STEP3				148			//���϶�
#define		C_FB_STEP4				149			//������
#define		C_FB_STEP5				150			//������
#define		C_FB_STEP6				151			//����̨
#define		C_FB_MODE				152			//ģʽ
#define		C_FB_PSAVE				153			//��������
#define		C_FB_RESET				154			//�����ָ�-��ԭ��
#define		C_FB_STEP0_1			155			//�۱�������,����(0,1)
#define		C_FB_CHMOD				156			//��ģ����
#define		C_FB_MV					157			//�ƶ�

#define		C_FB_ZL					158			//����
#define		C_FB_QL					159			//ȡ��

#define		C_FB_RSETP				160			//��������-�����ָ�

#define		C_FB_EXSTEP1			161			//����
#define		C_FB_EXSTEP2			162			//ȡ��
#define		C_FB_EXSTEP3			163			//�ص����
#define		C_FB_EXSTEP4			164			//���ϰ�����
#define		C_FB_DEMO				165			//��ʾ

#define		C_LF_SET				166			//�趨���Ӽ�����
#define		C_MACHINE_SET			167			//������Ϣ(����1(0-��ȡ��ǰ����,1-����Ĭ�ϲ���,2-�������))

#define		C_WAVES					168			//���β鿴:[����1;(0-ֹͣ;1-����;2-����)] [����2:(����)] [����3:ͨ��1] [����4:ͨ��2]

#define		C_LDOWMPF				169			//�۱�������޸Ļ�ͷ�½�

/*************************** 9 ������������ݼĴ��� ********************************/






// PREG
//---------------------------------------------------
//  �����Ĵ�������
//---------------------------------------------------
	
// ����(Wiper)														��λ		Min		Max		Std
#define	P_WIP	PREG[(U16)(4)*0+3]		// ���ߵ�ON/OFF													ON(1)
#define	P_W1	PREG[(U16)(4)*1+3]		// �������(W)�Ŀ�ʼ���ʱ��		msec		0		998		30
#define	P_W2	PREG[(U16)(4)*2+3]		// �������(W)�����ʱ��			msec		0		998		40
// �������(Slow start)
#define	P_S		PREG[(U16)(4)*3+3]		// ��������ٶ�						s/min		0		2299	200
#define	P_SL	PREG[(U16)(4)*4+3]		// ���������趨												OF(0)
#define	P_SLN	PREG[(U16)(4)*5+3]		// ������������					sti.		0		5		2
// ��ѹ��(Clamp)
#define	P_OST	PREG[(U16)(4)*6+3]		// ��ѹ��û����Ҳ��/���ɶ�ת									OF(0)
#define	P_RPT	PREG[(U16)(4)*7+3]		// �ظ����Ƶ��趨												OF(0)
#define	P_RP1	PREG[(U16)(4)*8+3]		// ѹ��1���ظ������趨											ON(1)
#define	P_RP2	PREG[(U16)(4)*9+3]		// ѹ��2���ظ������趨											ON(1)
#define	P_RP3	PREG[(U16)(4)*10+3]		// ѹ��3���ظ������趨											ON(1)
#define	P_RP4	PREG[(U16)(4)*11+3]		// ѹ��4���ظ������趨											ON(1)
#define	P_RP5	PREG[(U16)(4)*12+3]		// ѹ��5���ظ������趨											ON(1)
#define	P_RP6	PREG[(U16)(4)*13+3]		// ѹ��6���ظ������趨											ON(1)
#define	P_RP7	PREG[(U16)(4)*14+3]		// ѹ��7���ظ������趨											ON(1)
#define	P_RP8	PREG[(U16)(4)*15+3]		// ѹ��8���ظ������趨											ON(1)
#define	P_WHY	PREG[(U16)(4)*16+3]		// ѹ�Ŷ���������˳���趨										OF(0)
#define	P_FSR	PREG[(U16)(4)*17+3]		// ѹ�Ų��������Ľ��											OF(0)
#define	P_1PD	PREG[(U16)(4)*18+3]		// ��̤��������趨												OF(0)
#define	P_1T	PREG[(U16)(4)*19+3]		// ��̤��������ӳ�ʱ���趨			x10msec		0		498		0
#define	P_1A	PREG[(U16)(4)*20+3]		// ѹ��1�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_2A	PREG[(U16)(4)*21+3]		// ѹ��2�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_3A	PREG[(U16)(4)*22+3]		// ѹ��3�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_4A	PREG[(U16)(4)*23+3]		// ѹ��4�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_5A	PREG[(U16)(4)*24+3]		// ѹ��5�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_6A	PREG[(U16)(4)*25+3]		// ѹ��6�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_7A	PREG[(U16)(4)*26+3]		// ѹ��7�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_8A	PREG[(U16)(4)*27+3]		// ѹ��8�ӳ����ONʱ���趨			x10msec		0		999		0
#define	P_1B	PREG[(U16)(4)*28+3]		// ѹ��1�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_2B	PREG[(U16)(4)*29+3]		// ѹ��2�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_3B	PREG[(U16)(4)*30+3]		// ѹ��3�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_4B	PREG[(U16)(4)*31+3]		// ѹ��4�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_5B	PREG[(U16)(4)*32+3]		// ѹ��5�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_6B	PREG[(U16)(4)*33+3]		// ѹ��6�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_7B	PREG[(U16)(4)*34+3]		// ѹ��7�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_8B	PREG[(U16)(4)*35+3]		// ѹ��8�ӳ����OFFʱ���趨			x10msec		0		999		0
#define	P_OFB	PREG[(U16)(4)*36+3]		// ѹ�Ŷ����ָ��������趨										NO(0)
#define	P_FN	PREG[(U16)(4)*37+3]		// ��Чѹ�Ŷ����������趨						1		8		1
#define	P_CF	PREG[(U16)(4)*38+3]		// ѹ�Ŷ����������趨											OF(0)
#define	P_F21N	PREG[(U16)(4)*39+3]		// ѹ�Ŷ����ָ�ʱ��Чѹ����OF1-OF4				1		4		1
#define	P_F22N	PREG[(U16)(4)*40+3]		// ѹ�Ŷ����ָ�ʱ��Чѹ����OF5-OF8				1		4		1
#define	P_CF1	PREG[(U16)(4)*41+3]		// ѹ�Ŷ����ָ�ʱ�������趨OF1-OF4								OF(0)
#define	P_CF2	PREG[(U16)(4)*42+3]		// ѹ�Ŷ����ָ�ʱ�������趨OF5-OF8								OF(0)
#define	P_F4BN	PREG[(U16)(4)*43+3]		// ��Чѹ�ŷ����������趨						1		4		1
#define	P_F4SN	PREG[(U16)(4)*44+3]		// ���鲽���������趨							1		4		1
#define	P_AF2	PREG[(U16)(4)*45+3]		// ��ѹ���ζ���ѹ�ŵ�ѡ��										OF(0)
#define	P_OPR	PREG[(U16)(4)*46+3]		//+������������ѹ��λ������										OF(0)
// ���Ʒ�Χ(Sewing area)
#define	P_ALC	PREG[(U16)(4)*47+3]		// ���Ʒ�Χ���ƵĽ��											OF(0)
#define	P_XL	PREG[(U16)(4)*48+3]		// X����෶Χ���Ƶ��趨			mm			0		254		101
#define	P_XR	PREG[(U16)(4)*49+3]		// X���Ҳ෶Χ���Ƶ��趨			mm			0		254		101
#define	P_YU	PREG[(U16)(4)*50+3]		// Y���෶Χ���Ƶ��趨			mm			0		254		51
#define	P_YD	PREG[(U16)(4)*51+3]		// Y��ǰ�෶Χ���Ƶ��趨			mm			0		254		51
// ��λ��(Needle position)
#define	P_RU	PREG[(U16)(4)*52+3]		// ���ߺ�����ת��������											OF(0)
#define	P_R8	PREG[(U16)(4)*53+3]		// RU����ת�Ƕȵ��趨				deg.		0		360		30
#define	P_U8	PREG[(U16)(4)*54+3]		// ����ͣλ�õ�ֹͣ�Ƕ�				deg.		0		360		14
#define	P_D8	PREG[(U16)(4)*55+3]		// ����ͣλ�õ�ֹͣ�Ƕ�				deg.		0		360		50
// ���߼�֪(Thread trimming sensor)
#define	P_S1	PREG[(U16)(4)*56+3]		// ���߼�����Ч/��Ч
#define	P_S2	PREG[(U16)(4)*57+3]		// ������߼�����Ч����
#define	P_S3	PREG[(U16)(4)*58+3]		// ����;�ж��߼���ԪЧ����
#define	P_B		PREG[(U16)(4)*59+3]		// ���߼�⶯���ٶȵ��趨
#define	P_TST	PREG[(U16)(4)*60+3]		// ���߼��ʱ��������/��
// ԭ��(Home position)
#define	P_HPM	PREG[(U16)(4)*61+3]		// ��;��ͣʱ��ԭ�㸴�鷽��										--(0) --/JS/JE/JC
#define	P_HPC	PREG[(U16)(4)*62+3]		// �򿪵�Դʱԭ�㸴�鶯���Ľ�ֹ									ON(1)
#define	P_HPF	PREG[(U16)(4)*63+3]		// �Զ�ԭ�㸴�鶯���Ľ�ֹ										OF(0)
#define	P_HPK	PREG[(U16)(4)*64+3]		// ��������趨													--(0) --/2/2L/NO
#define	P_2HS	PREG[(U16)(4)*65+3]		// �����2ԭ��ʱֹͣ�ķ���										ST(0) ST/SW/NS
#define	P_FHP	PREG[(U16)(4)*66+3]		// ����ԭ���ʹ��												OF(0)
#define	P_CHS	PREG[(U16)(4)*67+3]		// �������϶���ʱ��ԭ���a��ѡ��									ON(1)
#define	P_SHX	PREG[(U16)(4)*68+3]		// X�᷶Χ���Ƶ�ԭ�㸴��										OF(0)
#define	P_SHY	PREG[(U16)(4)*69+3]		// Y�᷶Χ���Ƶ�ԭ�㸴��										OF(0)
#define	P_HPS	PREG[(U16)(4)*70+3]		// �ϵ�ʱԭ�㸴��ʱ����,��ѹ��״̬���趨						OF(0)
// ��;��ͣ(Halt)
#define	P_STF	PREG[(U16)(4)*71+3]		// ������ͣ��ť����ʱ���͵�ֹͣ״̬								CP(0) CP/ST
#define	P_STN	PREG[(U16)(4)*72+3]		// ������ͣ��ť����ʱ���ֹͣ״̬								UP(1) DN/UP/RN
#define	P_STS	PREG[(U16)(4)*73+3]		// ������ͣ��ť����ʱ��������ı���								AL(0) AL/FU/OF
#define	P_ST2	PREG[(U16)(4)*74+3]		// ���ڶ��ν�����ͣ��ťʱ�Ķ���									ST(2) UT/UP/ST
#define	P_STP	PREG[(U16)(4)*75+3]		// ��������ͣ��ťʱ��Դ�Ĳ���									--(0) --/PD
#define	P_STD	PREG[(U16)(4)*76+3]		// ����;��ͣ����ֹͣʱ��ѹ�ŵĶ���								FU(0) FU/FD
#define	P_STL	PREG[(U16)(4)*77+3]		// ��;��ͣ�������ʱͬλ�õķ�ֹ								ON(1)
// ����(Counter)
#define	P_CUP	PREG[(U16)(4)*78+3]		// ����UP��������												ED(1) --/ED/ST/CY
#define	P_CDN	PREG[(U16)(4)*79+3]		// ����DN��������												ED(1) --/ED/ST/CY
#define	P_UCM	PREG[(U16)(4)*80+3]		// ����UP�����趨ֵ����������									--(0) --/RE/IT/CL
#define	P_DCM	PREG[(U16)(4)*81+3]		// ����UP�����趨ֵ����������									--(0) --/RE/IT/CL
#define	P_PCM	PREG[(U16)(4)*82+3]		// �ؿ���Դʱ����ֵ����������									--(0) --/IT
#define	P_CN	PREG[(U16)(4)*83+3]		// ���������㹦��												OF(0)
#define	P_UCC	PREG[(U16)(4)*84+3]		// ����UP����ֵ��������ֹ										OF(0)
#define	P_DCC	PREG[(U16)(4)*85+3]		// ����DN����ֵ��������ֹ										OF(0)
#define	P_USC	PREG[(U16)(4)*86+3]		// ������UP����ֵ��������ҵ���趨								OF(0)
#define	P_DSC	PREG[(U16)(4)*87+3]		// ������DN����ֵ��������ҵ���趨								OF(0)
// ɲ��(Brake)
#define	P_WBK	PREG[(U16)(4)*88+3]		// ΢��ɲ�����趨												OF(0)
#define	P_BKM	PREG[(U16)(4)*89+3]		// ΢��ɲ���Ĺ����趨											E(0)  E/H
#define	P_BNR	PREG[(U16)(4)*90+3]		// ΢��ɲ���������趨											OF(0)
#define	P_BKT	PREG[(U16)(4)*91+3]		// ɲ��ʱ����趨					x10msec		0		99		14
#define	P_PW	PREG[(U16)(4)*92+3]		// ��ѹ�Ŷ���ʱ��΢��ɲ��ʱ��		msec		0		254		50
// ��ѹ��(Presser foot)
#define	P_PD	PREG[(U16)(4)*93+3]		// ��ѹ��ʱ��1(�½�ʱ)���趨		x10msec		0		498		36
#define	P_PU	PREG[(U16)(4)*94+3]		// ��ѹ��ʱ��2(����ʱ)���趨		x10msec		0		498		16
#define	P_UW	PREG[(U16)(4)*95+3]		// ��ѹ��ʱ��3(����ʱ)���趨		x10msec		0		498		10
#define	P_BPF	PREG[(U16)(4)*96+3]		// �ٷ��Ŀ����ƶ�ʱ��ѹ�ŵ��趨								OF(0)
#define	P_PLP	PREG[(U16)(4)*97+3]		// ��ѹ���г��趨				����	1,	0,	150
#define	P_ZU8	PREG[(U16)(4)*98+3]		// ��ѹ���������Ƕ�				��		1,	0,	360
#define	P_ZD8	PREG[(U16)(4)*99+3]		// ��ѹ�����½��Ƕ�				��		1,	0,	360
#define	P_ZTM	PREG[(U16)(4)*100+3]		// ����ʱ��ѹ��ͬ���趨			�߼�		0,	0,	1
// ����(Bobbin winding)
#define	P_W		PREG[(U16)(4)*101+3]		// �����ٶȵ��趨					s/min		0		2299	1500
#define	P_WSM	PREG[(U16)(4)*102+3]		// ���߶����Ĳ����趨											NO(0)
#define	P_WT	PREG[(U16)(4)*103+3]		// ���ߵĶ���ʱ��					sec			1		498		30
// �Ͳ���ʽ(Feed method)
#define	P_WET	PREG[(U16)(4)*104+3]		// ��ѹ�ŵ�����ѡ��												L(0)  L/M/H
#define	P_WEL	PREG[(U16)(4)*105+3]		// ��ѹ�ŵ�����(WET=L)ʱ���趨ֵ				0		99		20
#define	P_WEM	PREG[(U16)(4)*106+3]		// ��ѹ�ŵ�����(WET=M)ʱ���趨ֵ				0		99		26
#define	P_WEH	PREG[(U16)(4)*107+3]		// ��ѹ�ŵ�����(WET=H)ʱ���趨ֵ				0		99		35
#define	P_THI	PREG[(U16)(4)*108+3]		// �����ȵ�ѡ��												L(0)  L/M/H
#define	P_TL	PREG[(U16)(4)*109+3]		// ������(THI=L)ʱ���趨ֵ					0		60		0
#define	P_TM	PREG[(U16)(4)*110+3]		// ������(THI=M)ʱ���趨ֵ					0		60		32
#define	P_TH	PREG[(U16)(4)*111+3]		// ������(THI=H)ʱ���趨ֵ					0		60		44
#define	P_FED	PREG[(U16)(4)*112+3]		// �Ͳ���ʽ���趨												ID(1) IT/ID/S
#define	P_FEM	PREG[(U16)(4)*113+3]		// ���Ͷ�����ѡ��												S(1)  K/S
#define	P_FES	PREG[(U16)(4)*114+3]		// ƽ˳���͵��趨								0		9		9
#define	P_FSM	PREG[(U16)(4)*115+3]		// ƽ˳���͵��趨												ON(1)
#define	P_TYA	PREG[(U16)(4)*116+3]		// ���ʱ��ӡ����ƶ���(����)���趨	time		1		15		1
// �����ٶ�(Sewing speed)
#define	P_H		PREG[(U16)(4)*117+3]		// ����ٶ�							s/min		0		2299	2299
#define	P_L		PREG[(U16)(4)*118+3]		// ����ٶ�							s/min		0		399		200
#define	P_M		PREG[(U16)(4)*119+3]		// �и��ٶ�MD1						s/min		0		2299	1500
#define	P_N		PREG[(U16)(4)*120+3]		// �е��ٶ�MD2						s/min		0		2299	800
// ����ʱ��(Thread trimming timing)
#define	P_LTM	PREG[(U16)(4)*121+3]		// �������(T)���������										T1(0) T1/T2/T3/T4/T5
#define	P_LLM	PREG[(U16)(4)*122+3]		// �������(L)���������										L1(0) L1/L2/L3/L4/L5
#define	P_T1	PREG[(U16)(4)*123+3]		// �������(T)��ʼ�����ʱ��/�Ƕ�	msec		0		998		50
#define	P_T2	PREG[(U16)(4)*124+3]		// �������(T)�����ʱ��/�����Ƕ�	msec		0		998		0
#define	P_L1	PREG[(U16)(4)*125+3]		// �������(L)��ʼ�����ʱ��/�Ƕ�	msec		0		998		196
#define	P_L2	PREG[(U16)(4)*126+3]		// �������(L)�����ʱ��/�����Ƕ�	msec		0		998		0
#define	P_T		PREG[(U16)(4)*127+3]		// �����ٶ�							s/min		0		399		200
#define	P_ETC	PREG[(U16)(4)*128+3]		// ����������ʹ������ʱ������߶���								OF(0)
#define	P_TRM	PREG[(U16)(4)*129+3]		// ���߶�����ON/OFF												ON(1)
#define	P_LP	PREG[(U16)(4)*130+3]		// ��ѹ���������������ʱ		x10msec		0		998		0
#define	P_LSP	PREG[(U16)(4)*131+3]		// һ����������ŷ���ʹ�����		x10msec		0		998		0
// ����(Step)
#define	P_STE	PREG[(U16)(4)*132+3]		// �趨����ת��ʱ�Ĳ���Ҫ��ɾ���趨								OF(0)
#define	P_STO	PREG[(U16)(4)*133+3]		// ����Զ���ת��ON/OFF												OF(0)
#define	P_SUU	PREG[(U16)(4)*134+3]		// ���Ͷȵ�ִ������					line		1		192		2
#define	P_SP1	PREG[(U16)(4)*135+3]		// �������з�ʽ �汾1					line		1		192		2
#define	P_SKF	PREG[(U16)(4)*136+3]		// ��IOF����ʱ,������ȫ��ֹ				line		1		192		2
// �綯(Jog)
#define	P_JGM	PREG[(U16)(4)*137+3]		// �����Ĺ����趨												--(0) --/AL/HA
#define	P_UJC	PREG[(U16)(4)*138+3]		// ��ͣ����ͣλ������ʱ�Ĵ綯�����趨							OF(0)
#define	P_JGS	PREG[(U16)(4)*139+3]		// ��������ʱ��ͣ����Ĵ���							OF(0)
// �Ͳ��Ƕ�(Feed angle)
#define	P_S8	PREG[(U16)(4)*140+3]		// ��ʼ�Ͳ��ĽǶ��趨				deg.		25		198		94
#define	P_E8	PREG[(U16)(4)*141+3]		// ֹͣ�Ͳ��ĽǶ��趨				deg.		100		356		344
#define	P_T8	PREG[(U16)(4)*142+3]		// ����ʱֹͣ�Ͳ��ĽǶ��趨			deg.		150		356		324
#define	P_8D	PREG[(U16)(4)*143+3]		// ��ʾ����ͣλ�õĽǶ�				deg.
// Һ����ʾ��(LCD panel)
#define	P_KBC	PREG[(U16)(4)*144+3]		// ��������������												OF(0)
#define	P_LCD	PREG[(U16)(4)*145+3]		// �����������													ON(1)
#define	P_LOF	PREG[(U16)(4)*146+3]		// �������ʡ�繦��												ON(1)
#define	P_LNN	PREG[(U16)(4)*147+3]		// ���������ʾ��ɫ												ON(1)
#define	P_LHP	PREG[(U16)(4)*148+3]		// ����˵������ʾ												OF(1)
#define	P_KNO	PREG[(U16)(4)*149+3]		// ��׼ģ��ı���ѡ��							1		3		1
#define	P_CTR	PREG[(U16)(4)*150+3]		// �������ж�									0		10		0
#define	P_CNR	PREG[(U16)(4)*151+3]		// ���������ĶԱ�								0		100		50
#define	P_R		PREG[(U16)(4)*152+3]		// �������ʡ������ʱ��				/60min		0		9998	300
// �����ź�(Input signal)
#define	P_I1	PREG[(U16)(4)*153+3]		// I1����Ĺ���ѡ��
#define	P_I1L	PREG[(U16)(4)*154+3]		// I1����ĵ�ƽת��
#define	P_I1M	PREG[(U16)(4)*155+3]		// I1����Ķ���ѡ��
// ����ź�(Output signal)
#define	P_O1	PREG[(U16)(4)*156+3]		// O1����Ĺ���ѡ��
#define	P_O1L	PREG[(U16)(4)*157+3]		// O1����ĵ�ƽת��
#define	P_O1C	PREG[(U16)(4)*158+3]		// O1����Ķ�������
#define	P_O1M	PREG[(U16)(4)*159+3]		// O1����Ķ���ѡ��1
#define	P_O1D	PREG[(U16)(4)*160+3]		// O1�����ON�ӳ��趨
#define	P_O1E	PREG[(U16)(4)*161+3]		// O1����Ķ���ѡ��2
#define	P_O_1A	PREG[(U16)(4)*162+3]		// O1�����ON���ӳ�ʱ��/���������OFFʱ���趨
#define	P_O_1B	PREG[(U16)(4)*163+3]		// O1�����ONʱ��/������ONʱ���趨
#define	P_O_1T	PREG[(U16)(4)*164+3]		// O1�����ȫ�����ʱ���趨
#define	P_O_1C	PREG[(U16)(4)*165+3]		// O1����Ķ���Ч�ʵ��趨
// ����(Other)
#define	P_GA	PREG[(U16)(4)*166+3]		// ��/�ͷŴ����趨											L(0) L/LL/H
#define	P_MIL	PREG[(U16)(4)*167+3]		// ϳ�����ܵ�ON/OFF												OF(0)
#define	P_APC	PREG[(U16)(4)*168+3]		// ������̨������ָ���������Ϲ���								OF(0)
#define	P_POF	PREG[(U16)(4)*169+3]		// ͼ�η���ӹ�ѡ��												OF(0)
#define	P_ASC	PREG[(U16)(4)*170+3]		// ���ⲿ������ָ������ٶ����ƵĹ���							OF(0)
#define	P_AER	PREG[(U16)(4)*171+3]		// �������ϳ�������												OF(0)
#define	P_PFL	PREG[(U16)(4)*172+3]		// �ڲ��������ֹ��дɾ���趨									OF(0)
#define	P_RN	PREG[(U16)(4)*173+3]		// �����Ͳ�ʱ������Ŀ����
#define	P_LH	PREG[(U16)(4)*174+3]		// �����Ͳ�ʱ������Ŀ����
#define	P_LCK	PREG[(U16)(4)*175+3]		// ��ֹ�п�������趨���ת��ģ��
#define	P_PTC	PREG[(U16)(4)*176+3]		// �ӷ������ϱ���趨����ı��									OF(0)
#define	P_SCP	PREG[(U16)(4)*177+3]		// ��ݹ��ܼ��Ľ���												OF(0)
#define	P_DFR	PREG[(U16)(4)*178+3]		// �Ӵŵ�Ƭ����������Ҫ��д����ӵķ���							OF(0)
#define	P_RT	PREG[(U16)(4)*179+3]		// �趨�����������ʱ����							OF(0)
#define	P_SPC	PREG[(U16)(4)*180+3]		// ����ͨѶ							OF(0)
#define	P_APT	PREG[(U16)(4)*181+3]		// ͼ��ʱ������Ӳ�ѡ��							OF(0)
#define	P_LAN	PREG[(U16)(4)*182+3]		// ����ѡ��							OF(0)
#define	P_VOI	PREG[(U16)(4)*183+3]		// �����趨							OF(0)
#define	P_E_1	PREG[(U16)(4)*184+3]		// ������Ϣ1
#define	P_E_2	PREG[(U16)(4)*185+3]		// ������Ϣ2
#define	P_E_3	PREG[(U16)(4)*186+3]		// ������Ϣ3
#define	P_E_4	PREG[(U16)(4)*187+3]		// ������Ϣ4
#define	P_E_I1	PREG[(U16)(4)*188+3]		// I1�����ź���ʾ
#define	P_E_UP	PREG[(U16)(4)*189+3]		// UP�����ź���ʾ
#define	P_E_DN	PREG[(U16)(4)*190+3]		// DN�����ź���ʾ
#define	P_E_XH	PREG[(U16)(4)*191+3]		// XH�����ź���ʾ
#define	P_E_YH	PREG[(U16)(4)*192+3]		// YH�����ź���ʾ
#define	P_E_ECA	PREG[(U16)(4)*193+3]		// ECA�����ź���ʾ
#define	P_E_ECB	PREG[(U16)(4)*194+3]		// ECB�����ź���ʾ
#define	P_E_CRS	PREG[(U16)(4)*195+3]		// CRS�����ź���ʾ
#define	P_E_O1O	PREG[(U16)(4)*196+3]		// ���
#define	P_E_O1D	PREG[(U16)(4)*197+3]		// ���
#define	P_E_P	PREG[(U16)(4)*198+3]		// ��ʾ�ۻ�ͨ��ʱ��
#define	P_E_M	PREG[(U16)(4)*199+3]		// ��ʾ�ۻ����ת����ʱ��
#define	P_E_DV	PREG[(U16)(4)*200+3]		// ��ʾ�趨����İ汾��
#define	P_E_T	PREG[(U16)(4)*201+3]		// ��ʾ����
#define	P_E_BV	PREG[(U16)(4)*202+3]		// ��ʾϵͳROM�İ汾��
#define	P_E_SV	PREG[(U16)(4)*203+3]		// ��ʾϵͳ����İ汾��
// ����(Special)
#define	P_LHH	PREG[(U16)(4)*204+3]		// High speed H upper limit setting			x100s/min		0		99		23
#define	P_LHL	PREG[(U16)(4)*205+3]		// High speed H lower limit setting			x100s/min		0		99		0
#define	P_LLH	PREG[(U16)(4)*206+3]		// Low speed L upper limit setting			x100s/min		0		99		4
#define	P_LLL	PREG[(U16)(4)*207+3]		// LOw speed H lower limit setting			x100s/min		0		99		0
#define	P_DSL	PREG[(U16)(4)*208+3]		// Speed limit setting														OF(0)
#define	P_FSL	PREG[(U16)(4)*209+3]		// Speed limit cancel when continuos feed									ON(1)
#define	P_STM	PREG[(U16)(4)*210+3]		// Needle stop mode selection												OF(0)
#define	P_KU	PREG[(U16)(4)*211+3]		// Selection of UP signal													OF(0)
#define	P_K8	PREG[(U16)(4)*212+3]		// UP position angle from down position		deg.			0		360		270
#define	P_E8C	PREG[(U16)(4)*213+3]		// Detector error cancel mode												OF(0)
#define	P_MSP	PREG[(U16)(4)*214+3]		// Max. speed for main motor												36(0)
#define	P_PL	PREG[(U16)(4)*215+3]		// Sewing machine shaft/motor shaft speed setting selection					OF(0)
#define	P_MR	PREG[(U16)(4)*216+3]		// Motor pulley diameter					mm				0		254		60
#define	P_SR	PREG[(U16)(4)*217+3]		// Sewing machine pulley diameter			mm				0		254		70
#define	P_ACT	PREG[(U16)(4)*218+3]		// Acceleration time simple setting 		msec			0		99		14
#define	P_DCT	PREG[(U16)(4)*219+3]		// ����ʱ������趨											OF(0)
#define	P_CCW	PREG[(U16)(4)*220+3]		// Main motor rotating direction											OF(0)
#define	P_L8	PREG[(U16)(4)*221+3]		// Setting ofr down angle at low speed		deg.			0		360		28
#define	P_PPD	PREG[(U16)(4)*222+3]		// PMD power down select													ON(1)
#define	P_HXC	PREG[(U16)(4)*223+3]		// X motor CW/CCW															ON(1)
#define	P_HXS	PREG[(U16)(4)*224+3]		// Stepping motor rotation direction at home positioning					ON(1)
#define	P_HYC	PREG[(U16)(4)*225+3]		// Y motor CW/CCW															OF(0)
#define	P_HYS	PREG[(U16)(4)*226+3]		// Stepping motor rotation direction at home positioning					ON(1)
#define	P_F		PREG[(U16)(4)*227+3]		// Feed speed setting										0		65535	80
#define	P_XHP	PREG[(U16)(4)*228+3]		// X axis pulse setting for home positioning				0		255		37
#define	P_XH1	PREG[(U16)(4)*229+3]		// X axis pulse setting for home positioning(high speed)	0		65535	160
#define	P_XL1	PREG[(U16)(4)*230+3]		// X axis pulse setting for home positioning(low speed)		0		65535	2048
#define	P_XT	PREG[(U16)(4)*231+3]		// Lowest movement amount when X axis return to home position	0	254		20
#define	P_YHP	PREG[(U16)(4)*232+3]		// Y axis pulse setting for home positioning				0		255		37
#define	P_YH1	PREG[(U16)(4)*233+3]		// Y axis pulse setting for home positioning(high speed)	0		65535	160
#define	P_YL1	PREG[(U16)(4)*234+3]		// Y axis pulse setting for home positioning(low speed)		0		65535	2048
#define	P_YT	PREG[(U16)(4)*235+3]		// Lowest movement amount when Y axis return to home position	0	254		20
#define	P_L3	PREG[(U16)(4)*236+3]		// Pulse motor start time when X smooth feed is low			0		9998	640
#define	P_H3	PREG[(U16)(4)*237+3]		// Pulse motor start time when X smooth feed is high		0		9998	175
#define	P_X3	PREG[(U16)(4)*238+3]		// Change value of smooth feed of X							0		9998	1
#define	P_L4	PREG[(U16)(4)*239+3]		// Pulse motor start time when Y smooth feed is low			0		9998	640
#define	P_H4	PREG[(U16)(4)*240+3]		// Pulse motor start time when Y smooth feed is high		0		9998	175
#define	P_Y4	PREG[(U16)(4)*241+3]		// Change value of smooth feed of Y							0		9998	1
#define	P_HME	PREG[(U16)(4)*242+3]		// ԭ�����书��							0		9998	1
#define	P_MOS	PREG[(U16)(4)*243+3]		// �������Ѷ�ű��ı�							0		9998	1
#define	P_MS8	PREG[(U16)(4)*244+3]		// ����Ƕ�ƫ��ֵ							0		9998	1







//---------------------------------------------------
//  PT_HEAD�Ĵ������� 64   1792 - 1855 ��ǰ�����ļ�����Ϣ 
//---------------------------------------------------
//#define	HREG_IND_NO		DREG[DREG_PT_HEAD+0]	// ��ǰ�����ļ��б�λ��600-920 ffff��
//#define	HREG_START			DREG[DREG_PT_HEAD+1]	// ��ǰ�����ļ��������ݿ�ʼλ��
//#define	HREG_READ_PTDATA	DREG[DREG_PT_HEAD+2]	// ��CPU���ڶ���������
//#define	HREG_WRITE_PTDATA	DREG[DREG_PT_HEAD+3]	// ��CPU����д��������

//#define	HREG_SPPED		DREG[DREG_PT_HEAD+10]	// �����ٶ�

#define	HREG_SNO		20	// �Ǵ�����
#define	HREG_FNO		21	// �ļ����			01-00
#define	HREG_FN1		22	// �ļ���1			20-21
#define	HREG_FN2		23	// �ļ���2			22-23
#define	HREG_FN3		24	// �ļ���3			24-25
#define	HREG_FN4		25	// �ļ���4			26-27
#define	HREG_FEED_C		26	// ��ǰ����λ��
#define	HREG_FEED_SUM	27	// ����������		09-08/4
#define	HREG_FEED_PAT	28	// ����������		09-08/4-(pat�в���01,21,41,61ָ��)
#define	HREG_DRAW_ADD	29	// ��ͼ��ʼλ��		48h
#define	HREG_LINE_ADD	30	// ������ʼλ��		05-04+40h
#define	HREG_PAT_ADD	31	// ����ʼλ��		05-04+09-08/2+40h
#define	HREG_PATM_X		32	// �������Xֵ S16
#define	HREG_PATM_Y		33	// �������Yֵ S16
#define	HREG_PATI_X		34	// ������СXֵ S16
#define	HREG_PATI_Y		35	// ������СYֵ S16
#define	HREG_SCALE1		36	// ���� S32
#define	HREG_SCALE2		37	// ���� S32
#define	HREG_ORG_F		38	// ԭ����λ
#define	HREG_VER1		39	// �汾char			3C-3F
#define	HREG_VER2		40	// �汾char			3C-3F
#define	HREG_MAX_X		41	// ��������ƶ�X
#define	HREG_MAX_Y		42	// ��������ƶ�Y

#define	F_IND_NO		HREG[0]		// ��ǰ�����ļ��б�λ��600-920 ffff��								��
#define	F_START			HREG[1]		// ��ǰ�����ļ��������ݿ�ʼλ��										��
#define	F_READ_PTDATA	HREG[2]		// ��CPU���ڶ���������												��
#define	F_WRITE_PTDATA	HREG[3]		// ��CPU����д��������												��
#define	F_FILELENL		HREG[4]		// ��ǰ�����ܳ��ȵ�λ												��
#define	F_FILELENH		HREG[5]		// ��ǰ�����ܳ��ȸ�λ												��
#define F_FILELEN      (HREG[5] << 16 | HREG[4])// ��ǰ�����ܳ���

#define	F_ENDCODE		HREG[7]		// ��������
#define	F_FILETYPE		HREG[8]		// �ļ����� 1������	2���ֵ�
#define	F_GOHOMEFLAG	HREG[9]		// �����ٶ�
#define	F_PT_SPPED		HREG[10]	// �����ٶ�

#define	F_PT_PARTSUM	HREG[11]	// ��ǰ�����е��ӻ�����												��
#define	F_PT_PARTNUM	HREG[12]	// ��ǰ�������������е��ӻ�����										��
#define	F_PT_STPATNUM	HREG[13]	// ��ǰ�������������е��ӻ�������ʼ����
#define	F_PT_ENPATNUM	HREG[14]	// ��ǰ�������������е��ӻ����Ľ�������
#define	F_PT_STPATXPOS	HREG[15]	// ��ǰ�������������е��ӻ�������ʼ��λ�õ�ַX��
#define	F_PT_STPATYPOS	HREG[16]	// ��ǰ�������������е��ӻ�������ʼ��λ�õ�ַY��
#define	F_PT_STPATPFPOS	HREG[17]	// ��ǰ�������������е��ӻ�������ʼ��λ�õ�ַ��ѹ��
#define	F_PT_PAT2HDPOS	HREG[18]	// ��ǰ�����д�ԭ��λ��
#define	F_PT_SUBSTATUS	HREG[19]	// ��ǰ�������������е��ӻ���������״̬

#define	F_PT_SNO		HREG[20]	// �Ǵ�����
#define	F_PT_FNO		HREG[21]	// �ļ����			01-00
#define	F_PT_FN1		HREG[32]	// �ļ���1			20-21
#define	F_PT_FN2		HREG[33]	// �ļ���2			22-23
#define	F_PT_FN3		HREG[34]	// �ļ���3			24-25
#define	F_PT_FN4		HREG[35]	// �ļ���4			26-27
#define	F_PT_FN5		HREG[36]	// �ļ���5
#define	F_PT_FN6		HREG[37]	// �ļ���6
#define	F_PT_FN7		HREG[38]	// �ļ���7
#define	F_PT_FN8		HREG[39]	// �ļ���8
#define	F_PT_FN9		HREG[40]	// �ļ���9
#define	F_PT_FN10		HREG[41]	// �ļ���10
#define	F_PT_FN11		HREG[42]	// �ļ���11
#define	F_PT_FN12		HREG[43]	// �ļ���12
#define	F_PT_FN13		HREG[44]	// �ļ���13
#define	F_PT_FN14		HREG[45]	// �ļ���14
#define	F_PT_FN15		HREG[46]	// �ļ���15
#define	F_PT_FN16		HREG[47]	// �ļ���16
#define	F_PT_FN17		HREG[48]	// �ļ���17
#define	F_PT_FN18		HREG[49]	// �ļ���18
#define	F_PT_FN19		HREG[50]	// �ļ���19
#define	F_PT_FN20		HREG[51]	// �ļ���20
#define	F_PT_FN21		HREG[52]	// �ļ���21
#define	F_PT_FN22		HREG[53]	// �ļ���22
#define	F_PT_FN23		HREG[54]	// �ļ���23
#define	F_PT_FN24		HREG[55]	// �ļ���24
#define	F_PT_FN25		HREG[56]	// �ļ���25
#define	F_PT_FN26		HREG[57]	// �ļ���26
#define	F_PT_FN27		HREG[58]	// �ļ���27
#define	F_PT_FN28		HREG[59]	// �ļ���28
#define	F_PT_FN29		HREG[60]	// �ļ���29
#define	F_PT_FN30		HREG[61]	// �ļ���30
#define	F_PT_FN31		HREG[62]	// �ļ���31
#define	F_PT_FN32		HREG[63]	// �ļ���32

#define	F_PT_PATBYTESL	HREG[24]	// �������ֽ���
#define	F_PT_PATBYTESH	HREG[25]	// �������ֽ���
#define	F_PT_FEED_C		HREG[26]	// ��ǰ����λ��														��
#define	F_PT_FEED_SUM	HREG[27]	// ����������		09-08/4
#define	F_PT_FEED_PAT	HREG[28]	// ����������		09-08/4-(pat�в���01,21,41,61ָ��)
#define	F_PT_DRAW_ADD	HREG[29]	// ��ͼ��ʼλ��		48h
#define	F_PT_LINE_ADD	HREG[30]	// ������ʼλ��		05-04+40h
#define	F_PT_PAT_ADD	HREG[31]	// ����ʼλ��		05-04+09-08/2+40h


//-----�������Ĵ���
#define		CLFDAT		0	//��ǰ����ѹ��ֵ
#define		CLFNO		1	//��ǰ����κ�
#define		LFNOMAX		2	//��ǰ�����м���ѹ��ֵ
#define		LFNO0		3	//���Ӽ���ѹ���Ĵ������ڼ���ʹ��
#define		LFNO1		4
#define		LFNO2		5
#define		LFNO3		6
#define		LFNO4		7
#define		LFNO5		8
#define		LFNO6		9
#define		LFNO7		10
#define		LFNO8		11
#define		LFNO9		12

#define		LFDAT0		13	//���Ӽ���ѹ��ֵ�Ĵ���
#define		LFDAT1		14
#define		LFDAT2		15
#define		LFDAT3		16
#define		LFDAT4		17
#define		LFDAT5		18
#define		LFDAT6		19
#define		LFDAT7		20
#define		LFDAT8		21
#define		LFDAT9		22


#define		upcount_set_l		PREG[1018]		// 241*4+0		1020->[1018,1019]   2017-4-18
#define		upcount_set_h		PREG[1019]		// 241*4+0		1020->[1018,1019]   2017-4-18
#define		upcount_now_l		YREG[SYS_UPCOUNT]				//185->[183,184]
#define		upcount_now_h		YREG[SYS_UPCOUNT+1]	
#define		dncount_set_l		PREG[1020]		// 241*4+1		1021->[1020,1021]
#define		dncount_set_h		PREG[1021]		// 241*4+1		1021->[1020,1021]
#define		dncount_now_l		YREG[SYS_DNCOUNT]				//186->[185,186]		2017-4-18
#define		dncount_now_h		YREG[SYS_DNCOUNT+1]


#define		upcntonoff_flag		PREG[1022]		// 241*4+2
#define		dncntonoff_flag		PREG[1023]		// 241*4+3

//---------------˵����������ͨѶ�轫DREG_DISP��ΪDREG_OPRA------------------------------------------------------
#define		OPRA_OUTCND			OREG[19]						// ָ��(���巢��������)(��Ҫ�������ڿ���ʱѭ�����üĴ�������ʱ��������)
#define		OPRA_COMMAND		OREG[20]						// ָ��(�����淢������)
#define		OPRA_CMDSTATUS		OREG[21]						// ָ��״̬
#define		OPRA_CMDPARA1		OREG[22]						// ָ�����1
#define		OPRA_CMDPARA2		OREG[23]						// ָ�����2

#define		OPRA_DISPCOMMAND	OREG[25]						// ָ��(�������ָ��������壨�����屻���Σ�)
#define		OPRA_DISPCMDSTATUS	OREG[26]						// ָ��״̬
#define		OPRA_DISPCMDPARA1	OREG[27]						// ָ�����1
#define		OPRA_DISPCMDPARA2	OREG[28]						// ָ�����2
//---------------˵����������ͨѶ�轫DREG_DISP��ΪDREG_OPRA------------------------------------------------------

#define 	C_FREE				0			// ͨѶ����
#define 	C_BUSY				1			// ͨѶæ
#define 	C_ERROR				2			// ͨѶ����
#define 	C_LIMIT				3			// ͨѶ����
#define 	C_NOFLASH   		10			// û������	
#define 	C_NOFILE			11			// û���ļ�	
#define 	C_EORFILE			12			// �ļ������ѻ�	
#define 	C_CLETIME			99			// ��������ȴ�ʱ��	
#define 	C_SUCCEED			100			// ָ��ִ����ɣ��ɹ���

#define		sreg_total		HREG[11]		// 
#define		sreg_now		HREG[12]

//���������Ĵ���
//#define	SYS_PARA1CHKSUM	DREG[DREG_SYS+60]	// �����ת��������У��
//#define	SYS_PARA1X1		DREG[DREG_SYS+61]	// �����ת��������_90����������X
//#define	SYS_PARA1Y1		DREG[DREG_SYS+62]	// �����ת��������_90����������Y
//#define	SYS_PARA1X2		DREG[DREG_SYS+63]	// �����ת��������_180����������X
//#define	SYS_PARA1Y2		DREG[DREG_SYS+64]	// �����ת��������_180����������Y
//#define	SYS_PARA1X3		DREG[DREG_SYS+65]	// �����ת��������_270����������X
//#define	SYS_PARA1Y3		DREG[DREG_SYS+66]	// �����ת��������_270����������Y
//#define	SYS_PARA1XR		DREG[DREG_SYS+67]	// �����ת��������_Բ������X
//#define	SYS_PARA1YR		DREG[DREG_SYS+68]	// �����ת��������_Բ������Y
//#define	SYS_PARA1RD		DREG[DREG_SYS+69]	// �����ת��������_�뾶
//#define	SYS_PARA1ANG	DREG[DREG_SYS+70]	// �����ת��������_0��λ�ýǶ�
//#define	SYS_PARA2CHKSUM	DREG[DREG_SYS+71]	// ��еԭ����������У��
//#define	SYS_PARA2XORG	DREG[DREG_SYS+72]	// ��еԭ����������_��������X
//#define	SYS_PARA2YORG	DREG[DREG_SYS+73]	// ��еԭ����������_��������Y
//#define	SYS_PARA3CHKSUM	DREG[DREG_SYS+74]	// ����ת��϶��������У��
//#define	SYS_PARA3XZADJ	DREG[DREG_SYS+75]	// ����ת��϶��������_��������X+
//#define	SYS_PARA3XFADJ	DREG[DREG_SYS+76]	// ����ת��϶��������_��������X-
//#define	SYS_PARA3YZADJ	DREG[DREG_SYS+77]	// ����ת��϶��������_��������Y+
//#define	SYS_PARA3YFADJ	DREG[DREG_SYS+78]	// ����ת��϶��������_��������Y-

extern const volatile S16 PROMG_DATA[PROMGMAX][3];

extern volatile U16 EEPREG[];
extern volatile U16 DREG[MAXDREG];		// ״̬����
extern volatile U16 MREG[MAXMREG];		// ��������
extern volatile U16 SREG[MAXSREG];		// ��������
extern volatile U16 EREG[MAXEREG];		// ������Ϣ
extern volatile U16 YREG[MAXYREG];		// ϵͳ����
extern volatile U16 PREG[MAXPREG];		// ��������
extern volatile U16 HREG[MAXHREG];		// ��ǰ�����ļ���Ϣ
//extern volatile U16 IREG[MAXIREG];		// �����ļ�������
extern volatile U16 IREG2[MAXIREG2];		// �����ļ�������2 
extern volatile U16 NEWSREG[MAXSREG2];
extern volatile S16 PATCRRRD[MAXCOORD][2];
extern volatile S16 PATCRRRD1[MAXCOORD][2];		// �ݴ��»����������껺��
extern volatile U16 INPOINTPOS[MAXCOORD];		// �յ�λ��
extern volatile U16 UREG[MAXUREG];
extern volatile U16 VREG[MAXVREG];
extern volatile U16 GREG[MAXGREG];      // ��ϻ����б�
extern volatile U16 LREG[MAXLREG];
extern volatile U16 BLKSREG[MAXBLKSREG];
extern volatile U16 BLKHREG[MAXBLKHREG];
extern volatile U16 BLKIREG[MAXBLKIREG];
extern volatile U16 BLKIND[MAXBLIND];						// �������ݻ�������	
extern volatile U16 BLKBUF[MAXBLKBUF];	// �������ݻ���
extern volatile U8 AVI_VIDEO_BUF[AVI_VIDEO_BUF_SIZE];
extern volatile U8 AVI_AUDIO_BUF[AVI_AUDIO_BUF_SIZE];

extern volatile U16 OREG[MAXOREG];
//extern volatile U16 DIR_DREG[MAXDIR];
extern volatile U16 WREG[MAXWREG];
extern volatile U16 IOREG[MAXIOREG];
extern volatile U16 TMP_SREG[MAXTSREG];
extern volatile U16 IAPBuff[MAXTIAPBUFF];

//extern volatile U16 WAVEREG[MAXWAVE];

extern void Reg_Init(void);		
//extern U32 BlkAddr[100];


#define 	bit0			0x00000001
#define 	bit1			0x00000002
#define 	bit2			0x00000004
#define 	bit3			0x00000008
#define 	bit4			0x00000010
#define 	bit5			0x00000020
#define 	bit6			0x00000040
#define 	bit7			0x00000080
#define 	bit8			0x00000100
#define 	bit9			0x00000200
#define 	bit10			0x00000400
#define 	bit11			0x00000800
#define 	bit12			0x00001000
#define 	bit13			0x00002000
#define 	bit14			0x00004000
#define 	bit15			0x00008000
#define 	bit16			0x00010000
#define 	bit17			0x00020000
#define 	bit18			0x00040000
#define 	bit19			0x00080000
#define 	bit20			0x00100000
#define 	bit21			0x00200000
#define 	bit22			0x00400000
#define 	bit23			0x00800000
#define 	bit24			0x01000000
#define 	bit25			0x02000000
#define 	bit26			0x04000000
#define 	bit27			0x08000000
#define 	bit28			0x10000000
#define 	bit29			0x20000000
#define 	bit30			0x40000000
#define 	bit31			0x80000000

#endif
