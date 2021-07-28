/*
 * @Author: your name
 * @Date: 2021-05-08 16:19:42
 * @LastEditTime: 2021-05-08 16:26:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\Mit_menu_files.h
 */
//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Mit_menu_files.h							*
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

#ifndef	_MIT_MENU_FILES_H
#define _MIT_MENU_FILES_H


#include "stm32cpu.h"


extern volatile U8  g_disFileMethod;	//��ʾ�ļ���ʽ 0-�б�ģʽ 1-ͼ��ģʽ
extern volatile U16 g_SelFileNo;
extern volatile U16 g_selfiles[MAXFILENO];	//ѡ���ļ�
extern volatile U16 g_editfiles[MAXFILENO];//�༭�ļ�
extern volatile U16 g_btndisable;			//���ð���
extern volatile U8 g_checkMode;			//��ѡ/��ѡģʽ
extern volatile U8 g_selALL;	//ȫѡ
extern volatile U8 g_from_memory;
extern volatile U8 g_to_memory;
extern volatile U8 g_clipboard;	//���а�
extern volatile U8 g_savePathTimes;
extern char* g_path;
extern char* g_fromPath;
extern char* g_savePath;
 
void Mit_Menu_File(void);
void Mit_Menu_Keyboard(U16 inpara);

void initFunctionBtn(U8 clr);
void showFilePath(U16 xpos,U16 ypos,U8 disk,char *path,U8 flag);
U8 getPathStep(char* path);
void setPathToLast(char* path);
void copyPath(char *spath,char *dpath);
U8 getSavePathTimes(char* spath);
char* getFolderName(char* spath);
void savePath(char* spath,char *path,U8 time,U8 disk);
U8 getSavePath(char* spath,char *path,U8 time,U8 *disk);
U8 getFileNamaLen(U8 *name);
void backupViewPage(U8 page);
void recoverViewPage(U8 page);
#endif


