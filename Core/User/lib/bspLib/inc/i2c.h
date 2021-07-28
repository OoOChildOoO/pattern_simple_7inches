//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  i2c.h									*
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

#ifndef __I2C_H 	 
#define __I2C_H

#include "stm32cpu.h"



extern volatile U8 	i2cdelaytime,i2c1delaytime;
void I2C1_Init(U32 i2csp);
	  

#define	S_SB			0x00000001		// �ѷ��Ϳ�ʼλ
#define	S_ADDR			0x00000002		// �ѷ��͵�ַ/��ַƥ��
#define	S_ENDADD		0x00000002		// ��ַ���ͽ���
#define	S_BTF			0x00000004		// �ֽڴ������
#define	S_ADD10			0x00000008		// ��ģʽ��10Ѱַ��һ��ַ�ѷ���
#define	S_STOPF			0x00000010		// STOP���
#define	S_RxNE			0x00000040		// ���յ�����
#define	S_TxE			0x00000080		// �����ѷ���
#define	S_BERR			0x00000100		// ���ߴ���
#define	S_ARLO			0x00000200		// �ٲö�ʧ
#define	S_AF			0x00000400		// ȷ��ʧ��
#define	S_OVR			0x00000800		// ����
#define	S_PECERR		0x00001000		// PECУ�����
#define	S_TO			0x00004000		// ��ʱ����
#define	S_SMB			0x00008000		// SMB
#define	S_MSL			0x00010000		// ����/�ӻ�ģʽ
#define	S_BUSY			0x00020000		// ����æ
#define	S_TRA			0x00040000		// ���ݷ���/���ձ�־
#define	S_GCAL			0x00100000		// ��⵽�㲥��ַ
#define	S_ADSL			0x04000000		// ���ܵĵ�ַƥ��


#define	I2C_OFF			0x0000			// I2C�ر�
#define	I2C_EN			0x0001			// I2Cʹ��
#define	I2C_START		0x0101			// ������ʼλ
#define	I2C_STOP		0x0201			// ����ֹͣλ
#define	I2C_ACK			0x0401			// ����ȷ��λ
#define	I2C_STARTACK	0x0501			// ������ʼȷ��λ
#define	I2C_RESET		0x8001			// ���߸�λ



#define	DS2460			0xAA			// ����DS2460������ַ

#define	ONE_BYTE_SUBA	1
#define TWO_BYTE_SUBA	2
#define X_ADD_8_SUBA	3
extern U16 buf[2048];


extern void wdt(void);
extern void DelayMs(unsigned int n);
extern U8 DS2460_Read(U8 sla, U8 suba, U8 *s, U8 num);
extern U8 DS2460_Write(U8 sla, U8 suba, U8 *s, U8 num);

U8 GT9271_Read(U8 sla, U16 suba, U8 *s, U8 num);
U8 GT9271_Write(U8 sla, U16 suba, U8 *s, U8 num);

#endif


