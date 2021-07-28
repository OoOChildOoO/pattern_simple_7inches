/*
 * @Author: your name
 * @Date: 2021-07-08 08:47:22
 * @LastEditTime: 2021-07-09 13:43:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \projecte:\WJ\JK\Program\MingLing\project\STM32F429_7inches\Lib\inc\uart.h
 */
//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  uart.h									*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision4 V4.74					*
//*  编程													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#ifndef __UART_H   
#define __UART_H   

#include "stm32cpu.h"

////#define		UART_ADD		1					// 从机地址
//#define		UART_ADD		5					// 从机地址
#define 	RDTIMES			400					// 连续读数据的次数
#define 	COMWDNUM		125					// 每次通讯读写的最大字数






void UART1_Init(U32 baud, UARTMODE set);//U盘通讯
void UART1_SendByte(U8 dat);
void UART1_DMASendBuf(U16 len, U8 *dat);
void UART1_ISendBuf(U16 len, U8 *dat);

void UART2_Init(U32 baud, UARTMODE set);//上下位机通讯
void UART2_SendByte(U8 dat);
void UART2_DMASendBuf(U16 len, U8 *dat);
void UART2_ISendBuf(U16 len, U8 *dat);

extern volatile U8 uart1_rbuf[];	// 正在接收的buf   	 
extern volatile U8 uart1_tbuf[];	// 正在发送的buf
extern volatile U8 uart1_num;		// 发送个数
extern volatile U32 uart1_tc;		// 接收间隔计时ms

extern volatile U8 uart2_rbuf[];	// 正在接收的buf   	 
extern volatile U8 uart2_tbuf[];	// 正在发送的buf
extern volatile U8 uart2_num;		// 发送个数
extern volatile U32 uart2_tc;		// 接收间隔计时ms

extern volatile U32 com_err;//异常接受次数
extern volatile U8 receivef;//等待接受标志位 0:禁用 1:使能

extern U8 CRC16(U8 *puchMsg, U16 usDataLen, U8 flag);
extern U8 const auchCRCHi[];
extern U8 const auchCRCLo[];
#endif
