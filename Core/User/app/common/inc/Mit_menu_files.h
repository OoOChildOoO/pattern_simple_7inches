/*
 * @Author: your name
 * @Date: 2021-05-08 16:19:42
 * @LastEditTime: 2021-05-08 16:26:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\Mit_menu_files.h
 */
//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Mit_menu_files.h							*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision5 V5.21					*
//*  编程													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#ifndef	_MIT_MENU_FILES_H
#define _MIT_MENU_FILES_H


#include "stm32cpu.h"


extern volatile U8  g_disFileMethod;	//显示文件方式 0-列表模式 1-图标模式
extern volatile U16 g_SelFileNo;
extern volatile U16 g_selfiles[MAXFILENO];	//选择文件
extern volatile U16 g_editfiles[MAXFILENO];//编辑文件
extern volatile U16 g_btndisable;			//禁用按键
extern volatile U8 g_checkMode;			//复选/单选模式
extern volatile U8 g_selALL;	//全选
extern volatile U8 g_from_memory;
extern volatile U8 g_to_memory;
extern volatile U8 g_clipboard;	//剪切板
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


