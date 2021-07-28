//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  aviplayer.h								*
//*  ��   ��  ��:  ZHAO										*
//*  �� �� �� ��:  2017��6��07��							*
//*  ����޸�����: 											*
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

#ifndef _AVI_PLAYER_H
#define _AVI_PLAYER_H

#include "stm32cpu.h"
#include "stdlib.h"
#include "tfcard.h"
#include "diskint.h"
#include "vs1003.h" 
#include "spi.h"

#define  T_vids	 0x6463
#define  T_auds	 0x7762

#define 	AVI_VIDEO_BUF_SIZE    1024*60		//����avi����ʱ,��Ƶbuf��С.
#define 	AVI_AUDIO_BUF_SIZE    12		//����avi����ʱ,��Ƶbuf��С.
#define		MAXIREG2			999*36/*42000*/		// I2�ڴ����Ĵ���	�����ļ���������     


#define RIFF_ID		0x52494646		
#define AVI_ID		0x41564920		
#define LIST_ID		0x4c495354		
#define hdrl_ID		0x6864726c
#define avih_ID		0x61766968
#define strl_ID		0x7374726c
#define strh_ID		0x73747268
#define strf_ID		0x73747266
#define movi_ID		0x6d6f7669


#define	 MAKEWORD(ptr)	(WORD)(((WORD)*((BYTE*)(ptr))<<8)|(WORD)*(BYTE*)((ptr)+1))
#define  MAKEDWORD(ptr)	(DWORD)(((WORD)*(BYTE*)(ptr)|(((WORD)*(BYTE*)(ptr+1))<<8)|\
						(((WORD)*(BYTE*)(ptr+2))<<16)|(((WORD)*(BYTE*)(ptr+3))<<24))) 
						
typedef struct
{	
	DWORD  RIFFchunksize;
	DWORD  LISTchunksize;
	DWORD  avihsize;
	DWORD  strlsize;
	DWORD  strhsize;
}AVI_TypeDef;

typedef struct
{	
	DWORD  SecPerFrame;	//��ʾÿ֡��ʱ��
	DWORD  MaxByteSec; 	//������ݴ�����
	DWORD  ChunkBase; 	//�鳤��Ϊ��ֵ�ı�����ͨ��Ϊ2048
	DWORD  SpecProp;	  //AVI�ļ�����������
	DWORD  TotalFrame;	//�ļ���֡��
	DWORD  InitFrames;  //��ʼ����ǰ�ĳ�ʼ��֡��
	DWORD  Streams;		  //����������������
	DWORD  RefBufSize;  //����ʹ�õĻ�������С��ͨ��Ϊ�洢һ֡ͼ����ͬ����Ƶ����֮��
	DWORD  Width;		    //ͼ���
	DWORD  Height;		  //ͼ���
	DWORD  Reserved;	  //����
}avih_TypeDef;


typedef struct
{	
	uint8_t StreamType[4];//���������࣬vids(0x73646976):��Ƶ,auds:��Ƶ
	uint8_t Handler[4];	  //���ݽ�ѹ���������������
	DWORD   StreamFlag;   //����������
	WORD    Priority;	    //�����������ŵ����ȼ�
	WORD    Language;	    //��Ƶ�����Դ���
	DWORD   InitFrames;   //��ʼ����ǰ�ĳ�ʼ��֡��
	DWORD   Scale;		    //��Ƶÿ֡�Ĵ�С����Ƶ�Ĳ�����
	DWORD   Rate; 		    //Scale/Rate=ÿ�������
	DWORD   Start;		    //��������ʼ���ŵ�λ�ã���λΪScale
	DWORD   Length;		    //������������������λΪScale
 	DWORD   RefBufSize;   //����ʹ�õĻ�������С
	DWORD   Quality;		  //��ѹ������������ֵԽ������Խ��
	DWORD   SampleSize;	  //��Ƶ��������С
	struct{				
	  short int  Left;
		short int  Top;
		short int  Right;
		short int  Bottom;
	}Frame;				        //��Ƶ֡��ռ�ľ��� 
}strh_TypeDef;

typedef struct tagBMPHEADER
{
	DWORD  	Size;		      //���ṹ��ռ���ֽ���
	long   	Width;		    //ͼ���
	long   	Height;		    //ͼ���
	WORD   	Planes;		    //ƽ����������Ϊ1
	WORD   	BitCount;	    //����λ��
	BYTE  	Compression[4];	//ѹ�����ͣ�MJPEG
	DWORD  	SizeImage;	  //ͼ���С
	long   	XpixPerMeter; //ˮƽ�ֱ���
	long   	YpixPerMeter; //��ֱ�ֱ���
	DWORD  	ClrUsed;		  //ʵ��ʹ���˵�ɫ���е���ɫ��,ѹ����ʽ�в�ʹ��
	DWORD  	ClrImportant; //��Ҫ����ɫ
}BMPHEADER;             //�ýṹռ��40�ֽ�


typedef struct tagRGBQUAD 
{
	BYTE  rgbBlue;	      //��ɫ������(ֵ��ΧΪ0-255)
	BYTE  rgbGreen;       //��ɫ������(ֵ��ΧΪ0-255)
	BYTE  rgbRed; 	      //��ɫ������(ֵ��ΧΪ0-255)
	BYTE  rgbReserved;    //����������Ϊ0
}RGBQUAD;               //��ɫ��

typedef struct tagBITMAPINFO 
{
	BMPHEADER  bmiHeader;  //λͼ��Ϣͷ
	RGBQUAD    bmColors[1];//��ɫ��
}BITMAPINFO;  

typedef struct 
{
	WORD  FormatTag;
	WORD  Channels;	  //������
	DWORD SampleRate; //������
	DWORD BaudRate;   //������
	WORD  BlockAlign; //���ݿ�����־
	WORD  Size;       //�ýṹ��С
}WAVEFORMAT;

extern volatile U8 AVI_VIDEO_BUF[AVI_VIDEO_BUF_SIZE];
extern volatile U8 AVI_AUDIO_BUF[AVI_AUDIO_BUF_SIZE]; 
extern volatile U16 IREG2[MAXIREG2];		// �����ļ�������2 


uint8_t Find_BookFile(void);
uint8_t Read_BookData(uint16_t fileNo);
void DrawBook(uint8_t page,uint16_t curPage,uint8_t DrawON);
void Book_Play(int8_t scale,uint16_t posx,uint16_t posy,uint16_t CurPage,uint8_t Zoomflag,uint8_t DrawON);
//void UpdataPageNo(uint16_t CurPage);


uint8_t Find_AviFile(void);
uint8_t Check_AviFile(char *filename);//����ֵ 0��OK 1��ERR
void DrawAviProgress(void);
uint8_t Avi_Play(uint8_t exdis,uint8_t playf,uint8_t movef);//����ֵ 0��RUNING 1��COMPLETE
uint8_t AVI_Parser(uint8_t *buffer);//��ôʹ�ã���
uint32_t ReadUnit(uint8_t *buffer,uint8_t index,uint8_t Bytes,uint8_t Format);
uint8_t Avih_Parser(uint8_t *buffer);//��ôʹ�ã���
uint8_t Strl_Parser(uint8_t *buffer);//��ôʹ�ã���
uint8_t Strf_Parser(uint8_t *buffer);//��ôʹ�ã���
uint16_t Search_Movi(uint8_t* buffer);//��ôʹ�ã���
uint16_t Search_Fram(uint8_t* buffer);//��ôʹ�ã���
uint16_t Search_Auds(uint8_t* buffer);//��ôʹ�ã���

U8 getBootScreenMxg(void);//��ôʹ�ã���
U8 setBootScreenMode(U16 mode,U16 vmlk);//��ôʹ�ã���
U8 anyWriteScreeData(void);//��ôʹ�ã���
U8 WriteScreeData(U16 psx,U16 psy,U16 pslen);//��ôʹ�ã���
#endif
