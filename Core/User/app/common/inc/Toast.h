//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Bor_Toast.h							*
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

#ifndef	_TOAST_H
#define	_TOAST_H


#include "stm32cpu.h"

#define	TOAST_MSG1		0	//缝纫数据超出了可缝针范围,中断了处理.
#define	TOAST_MSG2		1	//密码不正确
#define	TOAST_MSG3		2	//新密码不匹配
#define	TOAST_MSG4		3	//剪切和粘贴的文件夹相同.
#define	TOAST_MSG5		4	//存储器未连接.
#define	TOAST_MSG6		5	//程序号超出范围,请输入1-999范围内的值!
#define	TOAST_MSG7		6	//注释内容不能为空!		
#define	TOAST_MSG8		7	//文件名不能为空!	
#define	TOAST_MSG9		8	//通讯错误,请检查通讯线是否与主机连接!	
#define	TOAST_MSG10		9	//操作失败!
#define	TOAST_MSG11		10	//添加文件数已达到最大数量.
#define	TOAST_MSG12		11	//参数取值错误.
#define	TOAST_MSG13		12	//针不在最高位,请手动调整到最高位
#define	TOAST_MSG14		13	//压板未放下,请踏下(黑色)脚制	
#define	TOAST_MSG15		14	//请关闭图形分离功能
#define	TOAST_MSG16	 	15	//请选择合并花样资料
#define	TOAST_MSG17	 	16	// 无法读取混合图形,请按清除键把当前合并图形清除		
#define	TOAST_MSG18	 	17	// 您没有权限修改系统时间,请与厂家联系
#define	TOAST_MSG19	 	18	// 请选择发送文件
#define	TOAST_MSG20 	19	// 输入错误,坐标超出加工范围.
#define	TOAST_MSG21 	20	// 不在原点,无法操作,请按回原点键	
#define	TOAST_MSG22 	21	// 通信故障,请检查伺服连接适合正常
#define	TOAST_MSG23 	22	// 选择文件错误,请重新选择
#define	TOAST_MSG24 	23	// 请选择升级设备
#define	TOAST_MSG25 	24	// 文件格式错误,读取失败
#define	TOAST_MSG26 	25	// 请按确定键完成当前操作
#define	TOAST_MSG27 	26	// 计数值不能大于设定值
#define	TOAST_MSG28 	27	// 设定值不能小于计数值
#define	TOAST_MSG29 	28	// 编辑循环程序前,请选择缝制数据
#define	TOAST_MSG30 	29	// 复制文件和原文件相同,请重新选择
#define	TOAST_MSG31 	30	// 请将中压脚降下
#define	TOAST_MSG32 	31	// 最大针距大于12.7,中断了处理.
#define	TOAST_MSG33 	32	// 内存文件不能复制到打板器.
#define	TOAST_MSG34 	33	// 打板器文件不能复制到内存.
#define	TOAST_MSG35 	34	// 通信故障,请检查步进电机连接是否正常

#define	TOAST_MSG_MAX	34

#define	TOAST_SHOW_5S	5000	

typedef struct _TOAST
{
	U8 isShow;
	U16 viewTime;
	U16 curTime;
	U16 vIndex;
	U16 bColor;
	U16 tColor;
	U16 xpos;
	U16 ypos;
	U16 xdpos;
	U16 ydpos;	
} TOAST;


void ToastInit(void);
void ToastSetBackColor(U16 color);
void ToastSetTextColor(U16 color);
void ToastShow1(U16 xpos,U16 ypos,U16 xdpox,U16 vTime);
void ToastHide(void);

extern TOAST g_Toast;

#endif


