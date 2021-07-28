/*
 * @Author: your name
 * @Date: 2021-05-08 13:43:33
 * @LastEditTime: 2021-05-08 14:00:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\Mit_funtion.h
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
//*  ��   ��  ��:  Mit_funtion.h							*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��08��22�� 							*
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
