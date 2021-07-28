/*
 * @Author: your name
 * @Date: 2021-05-08 13:43:33
 * @LastEditTime: 2021-05-08 14:00:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\Mit_funtion.h
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
//*  文   件  名:  Mit_funtion.h							*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年08月22日 							*
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

#ifndef _MIT_FUNCTION_H
#define _MIT_FUNCTION_H


#include "stm32cpu.h"
#include <string.h>

U16 getFiles(U16 *buf,U16 len);
U16 setBit16(U16 dat,U16 bit);
U16 clrBit16(U16 dat,U16 bit);
U8 getNumDigits(U32 nums);
// U8 isContain_U16(U16 *buff,U16 len,U16 dat);
void setBufDat16(U16 *buf,U16 len,U16 dat);
void setBufDat8(U8 *buf,U16 len,U8 dat);
void DisRange(U16 xpos,U16 ypos,U16 XRange,U16 YRange);
void removeStrByIndex(U8 *buf,U16 bufsize,U16 index);
int getCharIndexOnstr(char *str,char c,U16 stIndex);
int getCharLastIndexOnstr(char *str,char c);
U8 isContainFileByIREG(U16* ireg,U16 fileno);
U16 getFilesNo(U16 *buf,U16 len,U16 Index);
U16 getFilesNo1(U16 *buf,U16 inno);
U8* getFileNameByIREG(U16* ireg,U16 fileno);
void initMyProgress1(U16 maxval,U16 background);
void SetMyProgress1(U16 val);


#endif
