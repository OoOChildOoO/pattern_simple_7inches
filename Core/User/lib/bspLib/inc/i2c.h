//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  i2c.h									*
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

#ifndef __I2C_H 	 
#define __I2C_H

#include "stm32cpu.h"



extern volatile U8 	i2cdelaytime,i2c1delaytime;
void I2C1_Init(U32 i2csp);
	  

#define	S_SB			0x00000001		// 已发送开始位
#define	S_ADDR			0x00000002		// 已发送地址/地址匹配
#define	S_ENDADD		0x00000002		// 地址发送结束
#define	S_BTF			0x00000004		// 字节传输完成
#define	S_ADD10			0x00000008		// 主模式下10寻址第一地址已发送
#define	S_STOPF			0x00000010		// STOP检测
#define	S_RxNE			0x00000040		// 接收到数据
#define	S_TxE			0x00000080		// 数据已发送
#define	S_BERR			0x00000100		// 总线错误
#define	S_ARLO			0x00000200		// 仲裁丢失
#define	S_AF			0x00000400		// 确认失败
#define	S_OVR			0x00000800		// 过载
#define	S_PECERR		0x00001000		// PEC校验错误
#define	S_TO			0x00004000		// 超时错误
#define	S_SMB			0x00008000		// SMB
#define	S_MSL			0x00010000		// 主机/从机模式
#define	S_BUSY			0x00020000		// 总线忙
#define	S_TRA			0x00040000		// 数据发送/接收标志
#define	S_GCAL			0x00100000		// 检测到广播地址
#define	S_ADSL			0x04000000		// 接受的地址匹配


#define	I2C_OFF			0x0000			// I2C关闭
#define	I2C_EN			0x0001			// I2C使能
#define	I2C_START		0x0101			// 发送启始位
#define	I2C_STOP		0x0201			// 发送停止位
#define	I2C_ACK			0x0401			// 发送确认位
#define	I2C_STARTACK	0x0501			// 发送启始确认位
#define	I2C_RESET		0x8001			// 总线复位



#define	DS2460			0xAA			// 定义DS2460器件地址

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


