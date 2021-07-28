/*
 * @Author: your name
 * @Date: 2021-07-08 08:47:22
 * @LastEditTime: 2021-07-09 13:43:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \projecte:\WJ\JK\Program\MingLing\project\STM32F429_7inches\Lib\inc\uart.h
 */
//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  uart.h									*
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

#ifndef __UART_H   
#define __UART_H   

#include "stm32cpu.h"

////#define		UART_ADD		1					// �ӻ���ַ
//#define		UART_ADD		5					// �ӻ���ַ
#define 	RDTIMES			400					// ���������ݵĴ���
#define 	COMWDNUM		125					// ÿ��ͨѶ��д���������






void UART1_Init(U32 baud, UARTMODE set);//U��ͨѶ
void UART1_SendByte(U8 dat);
void UART1_DMASendBuf(U16 len, U8 *dat);
void UART1_ISendBuf(U16 len, U8 *dat);

void UART2_Init(U32 baud, UARTMODE set);//����λ��ͨѶ
void UART2_SendByte(U8 dat);
void UART2_DMASendBuf(U16 len, U8 *dat);
void UART2_ISendBuf(U16 len, U8 *dat);

extern volatile U8 uart1_rbuf[];	// ���ڽ��յ�buf   	 
extern volatile U8 uart1_tbuf[];	// ���ڷ��͵�buf
extern volatile U8 uart1_num;		// ���͸���
extern volatile U32 uart1_tc;		// ���ռ����ʱms

extern volatile U8 uart2_rbuf[];	// ���ڽ��յ�buf   	 
extern volatile U8 uart2_tbuf[];	// ���ڷ��͵�buf
extern volatile U8 uart2_num;		// ���͸���
extern volatile U32 uart2_tc;		// ���ռ����ʱms

extern volatile U32 com_err;//�쳣���ܴ���
extern volatile U8 receivef;//�ȴ����ܱ�־λ 0:���� 1:ʹ��

extern U8 CRC16(U8 *puchMsg, U16 usDataLen, U8 flag);
extern U8 const auchCRCHi[];
extern U8 const auchCRCLo[];
#endif
