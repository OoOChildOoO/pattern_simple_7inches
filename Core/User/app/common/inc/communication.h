//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  communication.h							*
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
//*															*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*


#ifndef __COMMUNICATION_H 	 
#define __COMMUNICATION_H

#include "stm32cpu.h"



extern volatile U8 ComErrMessage;
extern volatile U16 commtimes;
extern volatile U16 commtime1ms;
extern volatile U16 commtime10ms;

typedef union _RTEMP{
      volatile U32 ruart;
	  volatile U8 rbyte[4]; 
}RTEMP;











U8 Write_D(U32 Dn,U32 len,U16 *add);
U8 Read_D(U32 Dn,U32 len,U16 *add);
U8 Read_D_WAVE(U32 Dn,U32 len);
U8 Read_Dat(U32 Dn,U32 len,U16 *add);
U8 Write_DS(U8 comm, U32 Dn, U16 len, U16 *p);
U8 Read_DS(U8 comm, U32 Dn, U16 len, U16 *p);
U8 Write_Cmd(U16 cmd,U16 para1,U16 para2);
U8 Write_Cmd2(U16 cmd,U16 para1,U16 para2,U16 para3);
U8 Write_Cmd3(U16 cmd,U16 para1,U16 para2,U16 para3,U16 para4);
U16 Read_Status(void);
U16 Read1_Status(U8 times);
U8 Write_Cmd1(U16 cmd,U16 para1,U16 para2);
U8 Write1_Cmd1(U16 cmd,U16 para1,U16 para2,U8 wtime);
void  ErrorMsg(U32 tmpbpon);

U32 AnalyseUpdateList(U8 selextm,U8 netflag);

extern const U32 D_Length[];
extern const U32 D_Address[];
extern const U8 D_Command[];


extern U8 CRC16(U8 *puchMsg, U16 usDataLen, U8 flag);
extern U8 const auchCRCHi[];
extern U8 const auchCRCLo[];



#endif


