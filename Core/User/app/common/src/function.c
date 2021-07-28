//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  function.c								*
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
#include <math.h>
#include <stdlib.h>
#include "main.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "function.h"
#include "uart.h"
#include "communication.h"
#include "color.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"

#define bpno1 plastno

U8 g_old_minute;
U8 g_diskStatus; //磁盘状态 0:无连接 bit0-SD bit1-U盘
volatile U8 home_f = 0;
volatile U32 FLASHADD_P;
volatile U32 g_patxaddr, g_patyaddr; //记录兄弟款花样起始坐标存放位置
volatile U8 g_backSREG_f;			 // 备份SERG数据标志
volatile U8 g_backHREG_f;			 // 备份HERG数据标志
volatile U8 g_backIREG_f;			 // 备份IERG数据标志
volatile U32 g_backSREGlen;

volatile U16 g_feedct;		 //当前步行位置
volatile U8 g_init_flag;	 //花样绘制过程数据初始化标志位  1：保存,生成完整的花样;2:绘制，创建花样;3:恢复，复原原来的绘制数据
volatile U16 g_patd_fno = 0; //选中的花样编号
volatile U8 g_BorSTflag;	 //标记控制平台移动到词语次原点位置

// volatile U8 chCodeNow = 0; //当前针点的代码 新增

extern uint8_t ExtMemory_Status(uint8_t usbsd);
extern const U16 draw_code[][2];
extern const unsigned char ZKEY[][10];

extern MachineInfo g_Mac;
extern volatile U8 g_BarcodeFlag[MAXFILENO - MINFILENO];
extern volatile U8 g_disk_memory;
extern volatile U32 delaytimes;
extern volatile U8 g_init_flag;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern volatile U8 g_jogmode;
extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U16 g_point_num;
extern volatile S16 g_temp_oxpos, g_temp_oypos;
extern volatile S16 g_dxpos, g_dypos;							  //x,y坐标变化值
extern volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //x,y坐标实际值
extern volatile S16 g_disxpos, g_disypos;						  //x,y坐标实际值
extern volatile U8 DrawLimtf;
extern volatile U16 Drawmx0;
extern volatile U16 Drawmy0;
extern volatile U16 Drawmx1;
extern volatile U16 Drawmy1;
extern volatile U16 g_feedct;
extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U32 g_jog_point;
extern volatile U8 g_sta_end_flag;
extern volatile U8 g_datum_mark;
extern volatile U8 g_netSelectMac;
extern volatile U8 g_disk_memory;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos, g_wabspos;
extern volatile S16 g_pf_absdis;
extern volatile U16 *g_CodeNameAdd;
extern volatile U8 g_BorSTflag;
extern volatile U8 g_UdZhouFlag;
extern volatile S16 g_PinModParaANG; // 针杆旋转修正参数_0度位置角度
extern volatile U8 g_ExCode;
extern volatile U8 g_curExCode;
extern volatile U8 g_jogfunction;
extern volatile U8 g_jogfunction1;
extern volatile U8 g_speed;
extern volatile U8 g_oldspeed;
extern volatile U8 g_pitch;
extern volatile U8 g_num_mov_key;
extern volatile U32 g_temp_pno;
extern volatile U8 g_editflag;
extern volatile U32 g_feedsum;
extern volatile S16 g_pf_bdis;
extern volatile S8 g_pf_dis;
extern volatile U8 g_lineforceValSetNow;
extern volatile U16 g_parameter;
extern volatile U8 g_fbmac;
extern volatile U16 g_curPTtype;
extern volatile U32 g_sewPTlen;

extern volatile S16 g_PinModChkSum;	 // 针杆旋转修正参数校验值
extern volatile S16 g_PinModParaX1;	 // 针杆旋转修正参数_90度修正坐标X
extern volatile S16 g_PinModParaY1;	 // 针杆旋转修正参数_180度修正坐标Y
extern volatile S16 g_PinModParaX2;	 // 针杆旋转修正参数_180度修正坐标X
extern volatile S16 g_PinModParaY2;	 // 针杆旋转修正参数_180度修正坐标Y
extern volatile S16 g_PinModParaX3;	 // 针杆旋转修正参数_270度修正坐标X
extern volatile S16 g_PinModParaY3;	 // 针杆旋转修正参数_270度修正坐标Y
extern volatile S16 g_PinModParaXR;	 // 针杆旋转修正参数_圆心坐标X
extern volatile S16 g_PinModParaYR;	 // 针杆旋转修正参数_圆心坐标Y
extern volatile S16 g_PinModParaRD;	 // 针杆旋转修正参数_半径
extern volatile S16 g_PinModParaANG; // 针杆旋转修正参数_0度位置角度

extern volatile S16 g_OrgModChkSum; // 机械原点修正参数校验
extern volatile S16 g_OrgModParaX;	// 机械原点修正参数_修正坐标X
extern volatile S16 g_OrgModParaY;	// 机械原点修正参数_修正坐标Y
extern volatile S16 g_OrgModParaZ;	// 机械原点修正参数_修正坐标Z

extern volatile S16 g_OrgModParaXD; // 机械原点D轴修正参数_修正坐标X
extern volatile S16 g_OrgModParaYD; // 机械原点D轴修正参数_修正坐标Y

extern volatile S16 g_GapModChkSum;	 // 正反转间隙修正参数校验
extern volatile S16 g_GapModPosadjX; //正反转间隙修正参数_修正坐标X+
extern volatile S16 g_GapModNegadjX; //正反转间隙修正参数_修正坐标X-
extern volatile S16 g_GapModPosadjY; //正反转间隙修正参数_修正坐标Y+
extern volatile S16 g_GapModNegadjY; //正反转间隙修正参数_修正坐标Y-

extern volatile S16 g_ViewModParaX; //视觉坐标系偏移X
extern volatile S16 g_ViewModParaY; //视觉坐标系偏移Y

extern const unsigned int array1[];
extern volatile S32 g_array3[2][2];
extern volatile U16 g_array4[386][2];
extern volatile U16 g_array5[630][2];
extern volatile S16 g_array10[630][2];

extern const U16 pat_code[];

extern volatile U8 g_ImgPoint;
extern volatile U32 g_max_feedsum;
extern volatile U32 g_min_feedsum;
extern volatile S16 g_maxX;
extern volatile S16 g_minX;
extern volatile S16 g_maxY;
extern volatile S16 g_minY;
extern volatile U16 g_LPoint;
extern volatile U32 g_feedc;
extern volatile U16 g_splsum; // 当前修改的花样中的曲线条数
extern volatile U32 g_splpatadd;

extern volatile U8 g_MachineType;

extern void ViewRunPoint(U16 x, U16 y, U32 color, U8 linId);
extern void C2_ViewPoint(U16 x, U16 y, U32 color, U8 linId);
extern void G_ViewPoint(U16 x, U16 y, U32 color, U8 linId);

extern volatile U16 g_stpins;

extern volatile U8 g_usbmount;
extern int USB_USR_MSC_FATFSINIT(void);
extern int USBH_USR_MSC_Application(void);
/*-----------------------------------------------
//函数名: S16 abs(S16 dat)
//功能  : 去绝对值
--------------------------------------*/
int absInt(int dat)
{
	return (dat < 0) ? (-dat) : dat;
}

/*-----------------------------------------------
//函数名: static S16 abs(S16 dat)
//功能  : 去绝对值
--------------------------------------*/
S32 abs32(S32 dat)
{
	return (dat < 0) ? (-dat) : dat;
}

/*-----------------------------------------------
//函数名: void mymemset(void *s,uint8_t c,uint32_t count)
//功能  : 设置内存
//输入  :*s:内存首地址
		  c :要设置的值
		  count:需要设置的内存大小(字节为单位)
//输出  : *
//返回值: 无
//-----------------------------------------------*/
void mymemset(void *s, uint8_t c, uint32_t count)
{
	uint8_t *xs = s;
	while (count--)
		*xs++ = c;
}

//U16数据设置
void mysetU16(U16 *inbt, U16 dat, U32 len)
{
	while (len--)
	{
		*inbt = dat;
		inbt++;
	}
}

void copyU16(U16 *tar, U16 *sr, U32 len)
{
	U32 i = 0;
	while (i < len)
	{
		*(tar + i) = *(sr + i);
		i++;
	}
}
//-数据移动
void moveBuff(U8 *buff, S32 dlen, U32 baseAddr, U32 maxbuff)
{
	U32 i;
	//#if Debug
	//	printf("moveBuff() dlen = %d  maxbuff = %d baseAddr = %d \r\n",dlen,maxbuff,baseAddr);
	//#endif
	if (dlen == 0)
		return;
	if (dlen > 0)
	{ //后移
		for (i = 0; i < (maxbuff - baseAddr); i++)
		{
			buff[maxbuff - 1 - i + dlen] = buff[maxbuff - 1 - i];
			//#if Debug
			//	if(dlen==4)printf("buff[%d] = %02x \r\n",maxbuff-1-i+dlen,buff[maxbuff-1-i+dlen]);
			//#endif
		}
	}
	else
	{ //前移
		for (i = 0; i < (maxbuff - baseAddr); i++)
		{
			buff[baseAddr + i + dlen] = buff[baseAddr + i];
			//#if Debug
			//	printf("buff[%d] = %02x \r\n",baseAddr+i+dlen,buff[baseAddr+i]);
			//#endif
		}
	}
}

//------------------------------------------
// Function   : U8 DataCheckSum(U16 *data,U32 len)
// Description: 计算较验和
//				U8 Type:0-三菱款   1-兄弟格式
//------------------------------------------
U8 DataCheckSum(U16 *data, U32 len, U8 Type)
{
	//U32 i;
	U16 checksum = 0;

	//	#if Debug
	//	printf("DataCheckSum Type = %d  len = %d  \r\n",Type,len);
	//	#endif
	if (len > MAXSREG)
		return 0;
	wdt();
	if (Type == 0)
	{ //三菱
		if (data[0x20] != 0)
		{
			checksum = get_checkSum((U16 *)data, len, PAT_MIT);
			if (checksum == data[0x20])
				return 1;
			else
				return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (Type == 1)
	{ //兄弟
		if (data[0x05] != 0)
		{
			checksum = get_checkSum((U16 *)data, len, PAT_SEW);
			if (checksum == data[0x05])
				return 1;
			else
				return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------
//函数名:uint8_t strEqual(char *str1,char *str2)
//功能  :判断字符串是否相等
//------------------------------------------*/
uint8_t strEqual(char *str1, char *str2)
{
	uint16_t len1 = getStrLen((uint8_t *)str1);
	uint16_t len2 = getStrLen((uint8_t *)str2);
	//#if Debug
	//	printf("str1 = %s  ; str2 = %s \r\n" ,str1,str2);
	//#endif
	if (len1 != len2)
	{
		return 0;
	}
	while (*str1 != 0)
	{
		if (*str1 != *str2)
			return 0;
		str1++;
		str2++;
	}
	return 1;
}

//------------------------------------------
// Function   : U8 CheckDrawScale(U8 tt)
// Description: 检查读出的花样数据是否超范围
// U8 PatType :花样类型  0-三菱款    1-兄弟款
// U32 Patlen :花样长度 针对兄弟款
//------------------------------------------
U8 CheckDrawScale(U8 tt, U8 PatType, U32 Patlen, U8 upmac)
{
	U32 ii, k, n;
	U8 ch;
	U16 dat;
	S16 x1 = 0, y1 = 0;
	U8 f;
	U32 nums, i;
	U8 endflag = 0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};

#if Debug
	printf("CheckDrawScale()   PatType = %d  \r\n ", PatType);
#endif

	if (upmac)
	{
		f = Read_D(D_SYS + SYS_MAX_XL, 4, (U16 *)(&YREG[SYS_MAX_XL]));
		if (f)
		{
			g_Mac.max_xl = (S16)(YREG[SYS_MAX_XL]);
			g_Mac.max_xr = (S16)(YREG[SYS_MAX_XR]);
			g_Mac.max_yu = (S16)(YREG[SYS_MAX_YU]);
			g_Mac.max_yd = (S16)(YREG[SYS_MAX_YD]);
			if ((g_Mac.max_xl < (-20000)) || (g_Mac.max_xl > 0))
				g_Mac.max_xl = -20000;
			if ((g_Mac.max_xr > 20000) || (g_Mac.max_xr < 0))
				g_Mac.max_xr = 20000;
			if ((g_Mac.max_yu > 10000) || (g_Mac.max_yu < 0))
				g_Mac.max_yu = 10000;
			if ((g_Mac.max_yd < (-10000)) || (g_Mac.max_yd > 0))
				g_Mac.max_yd = -10000;
		}
		else
		{
			g_Mac.max_xl = -1000;
			g_Mac.max_xr = 1000;
			g_Mac.max_yu = 500;
			g_Mac.max_yd = -500;
		}
	}

	if (PatType == PAT_MIT)
	{																			   //三菱花样
		ii = (SREG[2] + 0x40 + (((U32)(SREG[5]) << 16) + SREG[4]) / 2) / 2;		   //针数据的起始位置
		k = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3; //花样数据总数

		if (tt)
		{
			if (DataCheckSum((U16 *)(&SREG[0]), k, 0) == 0)
			{
				return 0xee;
			}
		}

		//读取中压脚基准值
		Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
		g_pfootd.base = YREG[SYS_PFOOTSET];
		Mit_CalPFlimt((U16 *)&SREG[ii], k, &g_patd.PFmax, &g_patd.PFmin); //计算中压脚极限
		if ((g_patd.PFmax + g_pfootd.base > PF_ABS_MAX) || (g_patd.PFmax + g_pfootd.base < PF_ABS_MIN) || (g_patd.PFmin + g_pfootd.base > PF_ABS_MAX) || (g_patd.PFmin + g_pfootd.base < PF_ABS_MIN))
		{
			return (2); //中压脚超范围
		}

		for (n = ii; n < k; n += 2)//计算花样范围
		{ 
			dat = SREG[n] & 0x00ff;
			if (dat == 0x1e)
				return 0xfe;
			if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
			{
				dat = SREG[n + 1];
				ch = (U8)(dat);
				if (ch >= 0x80)
				{
					ch -= 0x80;
					x1 -= (S16)(ch);
				}
				else
				{
					x1 += ch;
				}
				ch = (U8)((dat >> 8));
				if (ch >= 0x80)
				{
					ch -= 0x80;
					y1 -= (S16)(ch);
				}
				else
				{
					y1 += ch;
				}

				if ((x1 < g_Mac.max_xl - 5) || (x1 > g_Mac.max_xr + 5) || (y1 > g_Mac.max_yu + 5) || (y1 < g_Mac.max_yd - 5))
				{
					return (0);
				}
			}
		}
	}
	else if (PatType == PAT_SEW)
	{ //兄弟花样

		f = Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		if (f == 0)
			return 0xee;

		nums = Patlen / 2;
		if (tt)
		{
			if (DataCheckSum((U16 *)(&SREG[0]), nums, 1) == 0)
			{
				return 0xee;
			}
		}

		//读取中压脚基准值
		Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
		g_pfootd.base = YREG[SYS_PFOOTSET];
		Bor_CalPFlimt((U16 *)&SREG, Patlen, &g_patd.PFmax, &g_patd.PFmin); //计算中压脚极限
																		   //		#if Debug
																		   //		printf("g_patd.PFmax = %d  g_patd.PFmin = %d \r\n",g_patd.PFmax,g_patd.PFmin);
																		   //		#endif
		if ((g_patd.PFmax + g_pfootd.base > PF_ABS_MAX) || (g_patd.PFmax + g_pfootd.base < PF_ABS_MIN) || (g_patd.PFmin + g_pfootd.base > PF_ABS_MAX) || (g_patd.PFmin + g_pfootd.base < PF_ABS_MIN))
		{
			return (2); //中压脚超范围
		}

		Bor_CalPatLim((U16 *)SREG, Patlen, &g_patd.patmX, &g_patd.patiX, &g_patd.patmY, &g_patd.patiY, &g_patd.feed_sum);
		//printf("g_patd.patmX = %d  g_patd.patiX = %d g_patd.patmY = %d g_patd.patiY = %d \r\n",g_patd.patmX,g_patd.patiX,g_patd.patmY,g_patd.patiY);
		if ((g_patd.patmX < (2 * (g_Mac.max_xl - 5))) || (g_patd.patmX > (2 * (g_Mac.max_xr + 5))) || (g_patd.patiX < (2 * (g_Mac.max_xl - 5))) || (g_patd.patiX > (2 * (g_Mac.max_xr + 5))) || (g_patd.patmY > (2 * (g_Mac.max_yu + 5))) || (g_patd.patmY < (2 * (g_Mac.max_yd - 5))) || (g_patd.patiY > (2 * (g_Mac.max_yu + 5))) || (g_patd.patiY < (2 * (g_Mac.max_yd - 5))))
		{
			//printf("***********\r\n");
			return (0);
		}
		//printf("------------\r\n");
		//针数据段	判断有无结束指令
		for (i = 0; i < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
			if (f)
			{
				if (sewIcmd.cmd == 0xff00)
				{
					endflag = 1;
					break;
				}
				i += sewIcmd.off;
			}
			else
			{
				i++;
			}
		}
		if (endflag == 0)
			return 0xfe;
	}
	return (1);
}
//------------------------------------------
// Function   : U8 CheckDrawData(U32 pno11)
// Description: 判断图形是否超范围,或数据错误
//------------------------------------------
U8 CheckDrawData(U32 pno11, U8 flag)
{
	U16 f;

	f = CheckDrawScale(1, F_FILETYPE, (F_FILELEN), 1);
//	#if Debug
//	printf("CheckDrawScale ret = %d \r\n",f);
//	#endif
#ifdef SYSTEM0

	if (f == 0xee)
	{ // 数据出错
		pno = MENU_RWHELP;
		hleppara = 64;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
	else if (f == 0xfe)
	{ // 没有结束指令
		pno = MENU_RWHELP;
		hleppara = 74;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
	else if (f == 0)
	{ // 显示图形超出范围
		pno = MENU_RWHELP;
		hleppara = 65;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
	else if (f == 2)
	{ // 中压脚行程超范围
		pno = MENU_RWHELP;
		hleppara = 140;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}

	f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

	if (f == 0)
	{ //读当前中压脚修正量
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}

	if ((/*pfootsetOffest+ */ (S16)(YREG[SYS_PFOOTSET]) > PF_ABS_MAX) || (/*pfootsetOffest+*/ (S16)(YREG[SYS_PFOOTSET]) < PF_ABS_MIN))
	{ //检查中压脚活动量是否超范围
		pno = MENU_RWHELP;
		hleppara = 67;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
#endif

	//#ifdef SYSTEM1

	if (f == 0xee)
	{ // 数据出错
		pno = MENU_RWHELP;
		hleppara = 64;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
	else if (f == 0xfe)
	{ // 没有结束指令
		if (flag == 0)
		{
			pno = MENU_RWHELP;
			hleppara = 74;
			poldno[pnoindex] = pno11;
			pnoindex++;
			return 0;
		}
	}
	else if (f == 0)
	{ // 显示图形超出范围
		pno = MENU_RWHELP;
		hleppara = 65;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
	else if (f == 2)
	{ // 中压脚行程超范围
		pno = MENU_RWHELP;
		hleppara = 140;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}

	f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

	if (f == 0)
	{ //读当前中压脚修正量
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}

	if ((/*pfootsetOffest+ */ (S16)(YREG[SYS_PFOOTSET]) > PF_ABS_MAX) || (/*pfootsetOffest+*/ (S16)(YREG[SYS_PFOOTSET]) < PF_ABS_MIN))
	{ //检查中压脚活动量是否超范围
		pno = MENU_RWHELP;
		hleppara = 67;
		poldno[pnoindex] = pno11;
		pnoindex++;
		return 0;
	}
	//#endif

#ifdef SYSTEM2
	if (f == 0xee)
	{ // 数据出错
		pno = MENU_BOR_HELP;
		hleppara = 53;
		poldno[pnoindex++] = pno11;
		return 0;
	}
	else if (f == 0xfe)
	{ // 没有结束指令
		pno = MENU_BOR_HELP;
		hleppara = 54;
		poldno[pnoindex++] = pno11;
		return 0;
	}
	else if (f == 0)
	{ // 显示图形超出范围
		pno = MENU_BOR_HELP;
		hleppara = 55;
		poldno[pnoindex++] = pno11;
		return 0;
	}
	else if (f == 2)
	{ // 中压脚行程超范围
		pno = MENU_BOR_HELP;
		hleppara = 56;
		poldno[pnoindex++] = pno11;
		return 0;
	}
	f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
	if (f == 0)
	{ //读当前中压脚修正量
		pno = MENU_BOR_HELP;
		hleppara = 15;
		poldno[pnoindex++] = pno11;
		return 0;
	}
#endif

	return 1;
}

//------------------------------------------
// Function   : void BarcodeCompare(void)
// Description: 判断内存索引表是否有条形码相同的花样，并作相应记录
//------------------------------------------
void BarcodeCompare(void)
{
	U8 f = 1;
	U16 i, j;
	U32 barC1, barC2;
	U8 l = 0;

	for (i = 0; i < PatFilesMAX; i++)
	{
		g_BarcodeFlag[i] = 0;
	}
	for (i = 0; i < PatFilesMAX; i++)
	{
		if ((g_BarcodeFlag[i] != 0) || ((IREG2[i * FileIndexSize] == 0xffff) && (IREG2[i * FileIndexSize + 1] == 0xffff)) || ((IREG2[i * FileIndexSize] == 0) && (IREG2[i * FileIndexSize + 1] == 0)))
		{
			continue;
		}
		barC1 = ((U32)(IREG2[i * FileIndexSize + FileIndexBarpox] + 1) << 16) + (U32)IREG2[i * FileIndexSize + FileIndexBarpox];
		if ((barC1 == 0) || (barC1 > 999999999))
		{
			continue;
		}
		for (j = i + 1; j < PatFilesMAX; j++)
		{
			barC2 = ((U32)(IREG2[i * FileIndexSize + FileIndexBarpox] + 1) << 16) + (U32)IREG2[i * FileIndexSize + FileIndexBarpox];
			if (barC1 == barC2)
			{
				if (g_BarcodeFlag[j] == 0)
				{
					g_BarcodeFlag[i] = f;
					g_BarcodeFlag[j] = f;
					l = 1;
				}
			}
		}
		if (l == 1)
		{
			l = 0;
			f++;
		}
	}
}

//------------------------------------------
// Function   : U16 FinePYFirstAdd(U8 *inabc, U16 c)
// Description: 拼音输入法，查找输入拼音对应汉字的首地址
//------------------------------------------
U16 FinePYFirstAdd(U8 *inabc, U16 c)
{
	U8 abc[6];
	U8 a, b;
	U16 dat;
	U32 add;
	U32 minadd, maxadd;

	minadd = ADD_PYINDEXDATA + 8;
	maxadd = ADD_PYMBDATA;
	for (add = 0; add < 6; add++)
	{
		if ((inabc[add] > 0x60) && (inabc[add] < 0x7b))
		{
			abc[add] = inabc[add] - 0x20;
		}
		else
		{
			for (; add < 6; add++)
				abc[add] = 0;
		}
	}
	//	BmpFillRect(640,160,48,32);
	//	DrawLFHEX(640,160,abc[0],2,Color_Blue);
	//	BmpFillRect(688,160,48,32);
	//	DrawLFHEX(688,160,abc[1],2,Color_Blue);
	//	BmpFillRect(640,192,48,32);
	//	DrawLFHEX(640,192,abc[2],2,Color_Blue);
	//	BmpFillRect(688,192,48,32);
	//	DrawLFHEX(688,192,abc[3],2,Color_Blue);
	//	BmpFillRect(640,224,48,32);
	//	DrawLFHEX(640,224,abc[4],2,Color_Blue);
	//	BmpFillRect(688,224,48,32);
	//	DrawLFHEX(688,224,abc[5],2,Color_Blue);

	for (add = minadd; add < maxadd; add += 8)
	{
		dat = *((U16 *)add);
		a = dat;
		b = dat >> 8;
		if ((a > 0x40) && (a < 0x7b) && (abc[0] >= a))
		{
			if (abc[0] == a)
			{ //
				if (abc[1] == 0)
				{
					dat = *((U16 *)(add + 6));
					return (dat);
				}
				else
				{
					if ((b > 0x40) && (b < 0x7b) && (abc[1] >= b))
					{
						if (abc[1] == b)
						{ //
							if (abc[2] == 0)
							{
								dat = *((U16 *)(add + 6));
								return (dat);
							}
							else
							{
								dat = *((U16 *)(add + 2));
								a = dat;
								b = dat >> 8;
								if ((a > 0x40) && (a < 0x7b) && (abc[2] >= a))
								{
									if (abc[2] == a)
									{ //
										if (abc[3] == 0)
										{
											dat = *((U16 *)(add + 6));
											return (dat);
										}
										else
										{
											if ((b > 0x40) && (b < 0x7b) && (abc[3] >= b))
											{
												if (abc[3] == b)
												{ //
													if (abc[4] == 0)
													{
														dat = *((U16 *)(add + 6));
														return (dat);
													}
													else
													{
														dat = *((U16 *)(add + 4));
														a = dat;
														b = dat >> 8;
														if ((a > 0x40) && (a < 0x7b) && (abc[4] >= a))
														{
															if (abc[4] == a)
															{ //
																if (abc[5] == 0)
																{
																	dat = *((U16 *)(add + 6));
																	return (dat);
																}
																else
																{
																	if ((b > 0x40) && (b < 0x7b) && (abc[5] >= b))
																	{
																		if (abc[5] == b)
																		{ //
																			dat = *((U16 *)(add + 6));
																			return (dat);
																		}
																	}
																	else
																	{
																		if (b != 0)
																			return (0);
																	}
																}
															}
														}
														else
														{
															if (a != 0)
																return (0);
														}
													}
												}
											}
											else
											{
												if (b != 0)
													return (0);
											}
										}
									}
								}
								else
								{
									if (a != 0)
										return (0);
								}
							}
						}
					}
					else
					{
						if (b != 0)
							return (0);
					}
				}
			}
		}
		else
		{
			if (a != 0)
				return (0);
		}
	}
	return (0);
}

//------------------------------------------
// Function   : U16 ReadHzData(U16 *hzbuf, U8 len, U16 add)
// Description: 拼音输入法，查找输入拼音对应汉字的首地址
//------------------------------------------
U16 ReadHzData(U16 *hzbuf, U8 len, U16 add)
{
	U8 i;
	U16 *padd;

	if ((add == 0) || (add >= HZLIBSIZE))
	{
		for (i = 0; i < len; i++)
		{
			hzbuf[i] = 0x2020;
		}
	}
	else
	{
		padd = MEM_PYMBDATA;
		for (i = 0; i < len; i++)
		{
			if (add >= HZLIBSIZE)
			{
				for (; i < len; i++)
				{
					hzbuf[i] = 0x2020;
				}
			}
			hzbuf[i] = *(padd + add);
			add++;
		}
	}
	return (0);
}

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------
/*-----------------------------------------------
//函数名:void strcopy(U8 *src,U8 *des,U16 len)
//功能  :指定字节数据复制
//------------------------------------------*/
void strcopy(U8 *src, U8 *des, U16 len)
{
	while (len--)
	{
		*des++ = *src++;
	}
}

/*-----------------------------------------------
//函数名:U16 getStrLen(U8 *buf)
//功能  :获取字符串长度
//------------------------------------------*/
U16 getStrLen(U8 *buf)
{
	U16 len = 0;
	while (*(buf + len) != 0)
		len++;
	return len;
}

/*-----------------------------------------------
//函数名: U8 getDiskStatus()
//功能  : 获取磁盘状态(不支持主板U盘)
--------------------------------------*/
U8 getDiskStatus(void)
{
	U8 status = 0;
#if LCD_TYPE == LCD_5cun //5寸显示屏有sd卡
	if (ExtMemory_Status(SD) == 0)
	{ //sd卡
		status |= 0x01;
	}
#endif
	if (ExtMemory_Status(USBTF) == 0)
	{ //U 盘
		status |= 0x02;
		if (g_usbmount == 0)
		{
			USB_USR_MSC_FATFSINIT();
			//USBH_USR_MSC_Application();
		}
	}

	return status;
}

/*-----------------------------------------------
//函数名: U8 getDiskStatus1()
//功能  : 获取磁盘状态(不支持主板U盘)
--------------------------------------*/
U8 getDiskStatus1(void)
{
	return g_diskStatus;
}

/*-----------------------------------------------
//函数名:void setDiskStatus(U8 status)
//功能  :设置磁盘状态
--------------------------------------*/
void setDiskStatus(U8 status)
{
	if (status > 3)
		status = 0;
	g_diskStatus = status;
}

/*-----------------------------------------------
//函数名:void clrShowTime()
//功能  : 
--------------------------------------*/
void clrShowTime(void)
{
	g_old_minute = 0xff;
}

/*-----------------------------------------------
//函数名:U16 getFileNumsByIREG(void)
//功能  :获取文件数量
//------------------------------------------*/
U16 getFileNumsByIREG(U16 *iReg, U8 ftype)
{
	U16 totalfiles = 0;
	U32 i;
	U8 type;
	for (i = 0; i < MAXIREG2; i += FileIndexSize)
	{
		if (((iReg[i + 1] << 16) | iReg[i]) != 0xffffffff)
		{
			type = ((iReg[i + FileIndexSize - 1] & 0xff00) >> 8);
			switch (ftype)
			{
			case GET_ALLFILE:
				totalfiles++;
				break;
			case GET_SEQFILE:
				if (type == FILE_SEQ)
					totalfiles++;
				break;
			case GET_PATFILE:
				if (type == FILE_MITE || type == FILE_MITG || type == FILE_SEW || type == FILE_EMB)
					totalfiles++;
				break;
			case GET_EMBFILE:
				if (type == FILE_EMB)
					totalfiles++;
				break;
			case GET_MITFILE:
				if (type == FILE_MITE || type == FILE_MITG)
					totalfiles++;
				break;
			case GET_SEWFILE:
				if (type == FILE_SEW)
					totalfiles++;
				break;
			case GET_GROFILE:
				if (type == FILE_GPKMC || type == FILE_GPKMD)
					totalfiles++;
				break;
			case GET_KMCFILE:
				if (type == FILE_GPKMC)
					totalfiles++;
				break;
			case GET_KMDFILE:
				if (type == FILE_GPKMD)
					totalfiles++;
				break;
			case GET_MACFILE:
				if (type == FILE_MACP)
					totalfiles++;
				break;
			}
			if ((g_disk_memory == MEM_USB) && (ftype != GET_ALLFILE && type == FILE_FOLDER))
			{ //外部存储器,文件夹也算
				totalfiles++;
			}
		}
	}
	wdt();
	return totalfiles;
}

/*-----------------------------------------------
//函数名:U16 getFileNumsByIREG(void)
//功能  :获取文件数量
//------------------------------------------*/
U16 getFileNumsByReg(U16 *reg, U8 ftype)
{
	U16 totalfiles = 0;
	U32 i;
	//	U8 type;
	U16 add = GroupComAddr;
	switch (ftype)
	{
	case GET_KMCFILE: //GREG寄存器组合图
		for (totalfiles = 0, i = 0; i < GroupMax; i++)
		{ //组合模式、接合模式最多20组  每组30寄存器
			if ((reg[i * GroupPatMax + add + GroupPNoAddr] >= MINFILENO) && (reg[i * GroupPatMax + add + GroupPNoAddr] <= MAXFILENO))
				totalfiles++;
		}
		break;
	}
	wdt();
	return totalfiles;
}

/*-----------------------------------------------
//函数名:U8 getFileTypeByIREG(U16 fileno)
//功能  :获取文件类型
//返回:
//------------------------------------------*/
U8 getFileTypeByIREG(U16 *iReg, U16 fileno)
{
	U8 type = 0;
	U32 addr = (fileno - MINFILENO) * FileIndexSize;
	type = ((iReg[addr + FileIndexSize - 1] & 0xff00) >> 8);
	//	printf("ireg[%d] = %04x \r\n",addr+FileIndexSize-1,iReg[addr+FileIndexSize-1]);
	return type;
}

/*-----------------------------------------------
//函数名:setKeyPos(KEYPAD *key,U16 xpos,U16 ypos,U16 xsize,U16 ysize)
//功能  :设置按键位置和大小
//------------------------------------------*/
void setKeyPos(KEYPAD *key, U16 xpos, U16 ypos, U16 xsize, U16 ysize)
{
	key->xpis = xpos;
	key->ypis = ypos;
	key->xsize = xsize;
	key->ysize = ysize;
}

//------------------------------------------
// Function   : void BackupSHREG(U8 Type)
// Description: 备份SREG->BLKSREG	HREG->BLKHREG
//	U8 Type:备份类型
//------------------------------------------
void BackupSHREG(U8 Type)
{
	U32 i = 0;
	//	printf("BackupSHREG Type = %d \r\n",Type);
	if (Type & 0x01)
	{
		for (i = 0; i < g_backSREGlen; i++)
		{
			BLKSREG[i] = SREG[i];
		}
		g_backSREG_f = 0;
	}
	if (Type & 0x02)
	{
		for (i = 0; i < MAXHREG; i++)
		{
			BLKHREG[i] = HREG[i];
		}
		g_backHREG_f = 0;
	}
}

//------------------------------------------
// Function   : void RestoreSHREG(U8 Type)
// Description: 还原 BLKSREG->SREG	BLKHREG->HREG
//	U8 Type:备份类型
//------------------------------------------
void RestoreSHREG(U8 Type)
{
	U32 i = 0;
	if (Type & 0x01)
	{
		for (i = 0; i < g_backSREGlen; i++)
		{
			SREG[i] = BLKSREG[i];
		}
		for (; i < MAXSREG; i++)
		{
			SREG[i] = 0;
		}
	}
	if (Type & 0x02)
	{
		for (i = 0; i < MAXHREG; i++)
		{
			HREG[i] = BLKHREG[i];
		}
	}
}

//------------------------------------------
// Function   : void BackupSHREG(U8 Type)
// Description: 备份IREG->BLKIREG
//	U8 Type:备份类型
//------------------------------------------
void BackupIREG(void)
{
	U32 i = 0;
	//printf("BackupIREG\r\n");
	wdt();
	for (i = 0; i < MAXIREG2; i++)
	{
		BLKIREG[i] = IREG2[i];
	}
	g_backIREG_f = 0;
}

//------------------------------------------
// Function   : void RestoreSHREG(U8 Type)
// Description: 还原 BLKIREG->IREG
//	U8 Type:备份类型
//------------------------------------------
void RestoreIREG(void)
{
	U32 i = 0;
	wdt();
	for (i = 0; i < MAXIREG2; i++)
	{
		IREG2[i] = BLKIREG[i];
	}
}

U32 g_mitBlen = 0;	//备份数据程度
U16 g_mitBfeed = 0; //备份针数
U16 g_mitBdraw = 0;
U16 g_mitBline = 0;
U16 g_mitBpat = 0;
U32 g_addr = 0;
U16 g_tsplsum = 0;
U32 g_tsplpatadd = 0;
void mitBackupSREG(void)
{
	U32 i;
	g_mitBfeed = 0; //备份针数
	g_mitBdraw = g_patd.draw_add;
	g_mitBline = g_patd.line_add;
	g_mitBpat = g_patd.pat_add;
	g_addr = FLASHADD_P;
	g_tsplsum = g_splsum;
	g_tsplpatadd = g_splpatadd;
	for (i = 0; i < MAXSREG; i++)
	{
		BLKSREG[i] = SREG[i];
	}
}

void mitRecovrSREG(void)
{
	U32 i;
	g_mitBfeed = 0; //备份针数
	g_patd.draw_add = g_mitBdraw;
	g_patd.line_add = g_mitBline;
	g_patd.pat_add = g_mitBpat;
	FLASHADD_P = g_addr;
	g_splsum = g_tsplsum;
	g_splpatadd = g_tsplpatadd;
	for (i = 0; i < MAXSREG; i++)
	{
		SREG[i] = BLKSREG[i];
	}
}
//------------------------------------------/
// Function   :void BackupSREG(U8 block,U8 loop,U32 len,u8 flag)
// Description: 备份花样数据
// U8 block：备份数据块
// U8 loop:循环备份
// U32 len：备份长度
// u8 flag: 0 - 三菱   1 - 兄弟
//------------------------------------------/
void BackupSREG(U8 block, U8 loop, U32 len, u8 flag)
{
	U32 i;
	//	U8 j;
	U32 addr;

	if (block <= 0)
		block = 0;
	// 	#if Debug
	//	printf("BackupSREG(%d ,%d) \r\n",block,flag);
	//	#endif
	if (flag == 0)
	{
		len = (g_patd.pat_add - g_patd.draw_add) + g_patd.feed_sum * 2 + 4;

		BLKIND[block * BLKINDSIZE] = block;					   //备份块数
		BLKIND[block * BLKINDSIZE + 1] = g_patd.feed_sum;	   //备份总针数
		BLKIND[block * BLKINDSIZE + 2] = (len >> 16) & 0xffff; //备份长度
		BLKIND[block * BLKINDSIZE + 3] = (len)&0xffff;

		if (block == 0)
		{ //备份地址
			BLKIND[block * BLKINDSIZE + 4] = 0;
			BLKIND[block * BLKINDSIZE + 5] = 0;
		}
		else
		{
			BLKIND[block * BLKINDSIZE + 4] = ((((BLKIND[(block - 1) * BLKINDSIZE + 4] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 5]) + ((BLKIND[(block - 1) * BLKINDSIZE + 2] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 3])) >> 16) & 0xffff;
			BLKIND[block * BLKINDSIZE + 5] = ((((BLKIND[(block - 1) * BLKINDSIZE + 4] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 5]) + ((BLKIND[(block - 1) * BLKINDSIZE + 2] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 3]))) & 0xffff;
		}

		addr = (BLKIND[block * BLKINDSIZE + 4] << 16) | BLKIND[block * BLKINDSIZE + 5];

		BLKBUF[addr++] = g_patd.draw_add;
		BLKBUF[addr++] = g_patd.line_add;
		BLKBUF[addr++] = g_patd.pat_add;
		BLKBUF[addr++] = g_patd.feed_sum;

		//		len1 = patd.line_add  - patd.draw_add;
		for (i = 0; i < (g_patd.line_add - g_patd.draw_add); i++)
		{
			BLKBUF[addr++] = SREG[g_patd.draw_add + i];
		}
		for (i = 0; i < (g_patd.pat_add - g_patd.line_add); i++)
		{
			BLKBUF[addr++] = SREG[32000 + i];
		}
		for (i = 0; i < g_patd.feed_sum * 2; i++)
		{
			BLKBUF[addr++] = SREG[52000 + i];
		}
	}
	else
	{
		BLKIND[block * BLKINDSIZE] = block;					   //备份块数
		BLKIND[block * BLKINDSIZE + 1] = g_patd.feed_sum;	   //备份总针数
		BLKIND[block * BLKINDSIZE + 2] = (len >> 16) & 0xffff; //备份长度
		BLKIND[block * BLKINDSIZE + 3] = (len)&0xffff;
		if (block == 0)
		{ //备份地址
			BLKIND[block * BLKINDSIZE + 4] = 0;
			BLKIND[block * BLKINDSIZE + 5] = 0;
		}
		else
		{
			BLKIND[block * BLKINDSIZE + 4] = ((((BLKIND[(block - 1) * BLKINDSIZE + 4] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 5]) + ((BLKIND[(block - 1) * BLKINDSIZE + 2] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 3])) >> 16) & 0xffff;
			BLKIND[block * BLKINDSIZE + 5] = ((((BLKIND[(block - 1) * BLKINDSIZE + 4] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 5]) + ((BLKIND[(block - 1) * BLKINDSIZE + 2] << 16) | BLKIND[(block - 1) * BLKINDSIZE + 3]))) & 0xffff;
		}

		addr = (BLKIND[block * BLKINDSIZE + 4] << 16) | BLKIND[block * BLKINDSIZE + 5];
		for (i = 0; i < len / 2; i++)
		{
			BLKBUF[addr + i] = SREG[i];
		}
	}
}

//------------------------------------------/
// Function   : void RecovrSREG(U8 block,u8 flag)
// Description: 花样数据恢复
// U8 block：恢复数据块
// u8 flag: 0- 三菱款  1- 兄弟款
//------------------------------------------/
void RecovrSREG(U8 block, u8 flag)
{
	U32 i;
	U32 len, addr;
	//	#if Debug
	//	printf("RecovrSREG(%d ,%d) \r\n",block,flag);
	//	#endif
	if (block <= 0)
		block = 0;
	if (flag == 0)
	{
		for (i = g_patd.draw_add; i < MAXSREG; i++)
		{
			SREG[i] = 0;
		}
		addr = (BLKIND[block * BLKINDSIZE + 4] << 16) | BLKIND[block * BLKINDSIZE + 5];

		g_patd.draw_add = BLKBUF[addr++];
		g_patd.line_add = BLKBUF[addr++];
		g_patd.pat_add = BLKBUF[addr++];
		g_patd.feed_sum = BLKBUF[addr++];

		for (i = 0; i < (g_patd.line_add - g_patd.draw_add); i++)
		{
			SREG[g_patd.draw_add + i] = BLKBUF[addr++];
		}
		for (i = 0; i < (g_patd.pat_add - g_patd.line_add); i++)
		{
			SREG[g_patd.line_add + i] = BLKBUF[addr++];
		}
		for (i = 0; i < (g_patd.feed_sum * 2); i++)
		{
			SREG[g_patd.pat_add + i] = BLKBUF[addr++];
		}
	}
	else
	{
		HREG[27] = BLKIND[block * BLKINDSIZE + 1];
		HREG[5] = BLKIND[block * BLKINDSIZE + 2];
		HREG[4] = BLKIND[block * BLKINDSIZE + 3];

		len = (F_FILELEN) / 2;
		addr = (BLKIND[block * BLKINDSIZE + 4] << 16) | BLKIND[block * BLKINDSIZE + 5];
		for (i = 0; i < len; i++)
		{
			SREG[i] = BLKBUF[addr + i];
		}
	}
}

//------------------------------------------/
// Function   : AnaPattern					/
// Description: 分析花样数据参数			/
//				中心点164,120 宽230x166		/
//				比例1dot:1mm				/
// mode: 0 有次原点时显示整个图形			/
// 		 1 有次原点时只显示次原点后面的图形	/
// Type :花样类型 0-三菱款； 1—兄弟款      2016-12-5新增
// U32 Patlen:花样数据总长 针对兄弟款   2016-12-16
//------------------------------------------/
void AnaPattern(U8 mode, U8 Type, U32 Patlen)
{
	U8 ch;
	U32 i, j;
	S32 x, y;
	S32 xtmp, ytmp;
	U8 f, ii;
	U16 checksum;
	U32 DrawLen = 0;
	ISEWADD sewInfo = {0};
	//	U32 pinAddr,pinlen;
	//	U16 areaMax,ctrlMax,pinMax;
	wdt();

	//	#if Debug
	//	printf("AnaPattern Type = %d Patlen = %d init_flag = %d\r\n",Type,Patlen,g_init_flag) ;
	//	#endif

	g_patd.endcode = 0;
	g_patd.year = 0;
	g_patd.month = 0;
	g_patd.day = 0;
	g_patd.hour = 0;
	g_patd.min = 0;
	g_patd.sec = 0;
	if (Type == PAT_MIT)
	{ //三菱款
		if (((((U32)(SREG[5]) << 16) + SREG[4]) > 80000) || (SREG[2] < 0x20) || (SREG[2] > 64000))
		{ //2013.3.27
			//for(i=0;i<MAXSREG;i++) SREG[i] = 0;
			mysetU16((U16 *)SREG, 0, MAXSREG);
			SREG[0] = 0;	//文件号
			SREG[2] = 0x10; //线条数据启始位置在SREG[6000]位置
			SREG[3] = 6;
			SREG[4] = 4;	   //每组数据的针数据长最大为6000,即针数据的启始位置在SREG[3000]位置
			SREG[12] = 0x3e8;  //花样最小长度100mm
			SREG[14] = 0x3e8;  //花样最小宽度100mm
							   //	SREG[16]= 0x454e;		//文件名
							   //	SREG[17]= 0x2057;
							   //	SREG[18]= 0x2020;
							   //	SREG[19]= 0x2020;
							   //			SREG[16]= 0;		//文件名
							   //			SREG[17]= 0;
							   //			SREG[18]= 0;
							   //			SREG[19]= 0;
			SREG[30] = 0x3045; //版本
			SREG[31] = 0x3536;
			SREG[0x24] = 0;
			g_draw_p = 0x28; //绘图数据当前位置指针
			g_line_p = 0x29; //线条数据当前位置指针
			g_pat_p = 0x2b;	 //针数据当前位置指针
			g_init_flag = 0;

			//	DrawMFS32P(580,180, 1,3,0,Color_Black);
			//}else{
			//	DrawMFS32P(580,180, 2,3,0,Color_Black);
		}
		g_patd.ver[0] = (U8)(SREG[0x1e] & 0xff);
		g_patd.ver[1] = (U8)(SREG[0x1e] >> 8);
		g_patd.ver[2] = (U8)(SREG[0x1f] & 0xff);
		g_patd.ver[3] = (U8)(SREG[0x1f] >> 8);
		g_patd.ver[4] = 0;
		// 文件编号 	01-00
		g_patd.pat_len = Patlen;
		//g_patd.fno = SREG[0];
		g_patd.fno = F_IND_NO;

		// 文件名		20-2F
		/*		patd.fname[0] = (U8)(SREG[0x10]&0xff);
		patd.fname[1] = (U8)(SREG[0x10]>>8);
		patd.fname[2] = (U8)(SREG[0x11]&0xff);
		patd.fname[3] = (U8)(SREG[0x11]>>8);
		patd.fname[4] = (U8)(SREG[0x12]&0xff);
		patd.fname[5] = (U8)(SREG[0x12]>>8);
		patd.fname[6] = (U8)(SREG[0x13]&0xff);
		patd.fname[7] = (U8)(SREG[0x13]>>8);
		patd.fname[8] = 0; */

		for (ii = 0; ii < 64; ii++)
		{
			g_patd.fname[ii] = 0;
		}
		wdt();

		i = 0;
		for (ii = 0; ii < 32; ii++)
		{
			if ((HREG[32 + ii] & 0xff) == 0)
				break;
			if ((((HREG[32 + ii] & 0xff) >= 0x30) && ((HREG[32 + ii] & 0xff) <= 0x39)) || (((HREG[32 + ii] & 0xff) >= 0x41) && ((HREG[32 + ii] & 0xff) <= 0x5a)) ||
				(((HREG[32 + ii] & 0xff) >= 0x61) && ((HREG[32 + ii] & 0xff) <= 0x7a)) || ((HREG[32 + ii] & 0xff) > 0x7f) || ((HREG[32 + ii] & 0xff) == 0x5f))
			{
				g_patd.fname[i++] = (HREG[32 + ii]) & 0xff;
			}
			if (((HREG[32 + ii] >> 8) & 0xff) == 0)
			{
				break;
			}

			if (((((HREG[32 + ii] >> 8) & 0xff) >= 0x30) && (((HREG[32 + ii] >> 8) & 0xff) <= 0x39)) || ((((HREG[32 + ii] >> 8) & 0xff) >= 0x41) && (((HREG[32 + ii] >> 8) & 0xff) <= 0x5a)) ||
				((((HREG[32 + ii] >> 8) & 0xff) >= 0x61) && (((HREG[32 + ii] >> 8) & 0xff) <= 0x7a)) || (((HREG[32 + ii] >> 8) & 0xff) > 0x7f) || (((HREG[32 + ii] >> 8) & 0xff) == 0x5f))
			{
				g_patd.fname[i++] = (HREG[32 + ii] >> 8) & 0xff;
			}
		}
		g_patd.fname[63] = 0;

		// 最大移动XY	19-18 1D-1C
		g_patd.maxX = SREG[0x0c];
		g_patd.maxY = SREG[0x0e];
		// 绘图起始位置	48H
		g_patd.draw_add = 0x24;
		// 线条起始位置	05-04+40H
		g_patd.line_add = (SREG[2] + 0x40) / 2;
		// 针起始位置	05-04+40H+09-08/2
		g_patd.pat_add = (SREG[2] + 0x40 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 2)) / 2; //2013.3.27
		// 花样总针数	09-08/4
		if (g_init_flag == 1)
		{
			g_patd.feed_sum = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
			g_draw_p = g_patd.line_add;
			g_line_p = g_patd.pat_add;
			g_pat_p = g_patd.pat_add + g_patd.feed_sum * 2;
		}
		else
		{
			g_patd.feed_sum = g_line_p - g_patd.line_add;
			//printf("g_patd.pat_add = %d  g_patd.feed_sum = %d \r\n",g_patd.pat_add,g_patd.feed_sum);
		}
		// 花样缝针数	09-08/4-非移动指令
		g_patd.p_2hd = 0xffff;
		if (mode == 1)
		{
			j = g_patd.pat_add;
			for (i = 0; i < g_patd.feed_sum; i++)
			{
				if (SREG[j] == 0x6)
				{
					g_patd.p_2hd = i;
					break;
				}
				j += 2;
			}
		}
		wdt(); // 喂狗	2010.3.27
		g_patd.X2hd = 0;
		g_patd.Y2hd = 0;
		if (g_patd.p_2hd != 0xffff)
		{
			j = g_patd.pat_add;
			for (i = 0; i < g_patd.p_2hd; i++)
			{
				ch = (U8)(SREG[j] & 0xff);
				if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61) || (ch == 0x3) || (ch == 0x1b))
				{
					ch = (U8)(SREG[j + 1] & 0xff);
					if (ch >= 0x80)
						xtmp = (ch - 0x80) * (-1);
					else
						xtmp = ch;
					ch = (SREG[j + 1] >> 8) & 0xff;
					if (ch >= 0x80)
						ytmp = (ch - 0x80) * (-1);
					else
						ytmp = ch;
					g_patd.X2hd += xtmp;
					g_patd.Y2hd -= ytmp;
				}
				j += 2;
				wdt();
			}
		}
		x = 0;
		y = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			x = (S16)(SREG[6]);
			y = -(S16)(SREG[7]);
		}
#endif
		x += g_patd.X2hd;
		y += g_patd.Y2hd;
		g_patd.patmX = 0;
		g_patd.patiX = 0;
		g_patd.patmY = 0;
		g_patd.patiY = 0;
		if (x > g_patd.patmX)
			g_patd.patmX = x;
		if (y > g_patd.patmY)
			g_patd.patmY = y;
		if (x < g_patd.patiX)
			g_patd.patiX = x;
		if (y < g_patd.patiY)
			g_patd.patiY = y;
		g_patd.feed_pat = 0;
		j = g_patd.pat_add;

		for (i = 0; i < g_patd.feed_sum; i++)
		{
			ch = (U8)(SREG[j] & 0xff);
			if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61))
			{
				g_patd.feed_pat++;
			}
			if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61) || (ch == 0x3) || (ch == 0x1b))
			{
				if ((g_patd.p_2hd == 0xffff) || (g_patd.p_2hd < i))
				{
					ch = (U8)(SREG[j + 1] & 0xff);
					if (ch >= 0x80)
						xtmp = (ch - 0x80) * (-1);
					else
						xtmp = ch;
					ch = (SREG[j + 1] >> 8) & 0xff;
					if (ch >= 0x80)
						ytmp = (ch - 0x80) * (-1);
					else
						ytmp = ch;
					x += xtmp;
					y -= ytmp;
					if (x > g_patd.patmX)
						g_patd.patmX = x;
					if (y > g_patd.patmY)
						g_patd.patmY = y;
					if (x < g_patd.patiX)
						g_patd.patiX = x;
					if (y < g_patd.patiY)
						g_patd.patiY = y;
					//printf("x = %d y = %d \r\n",x,y);
				}
			}
			j += 2;
			wdt(); // 喂狗	2010.3.27
		}
		g_patd.scale = 100;

		//---计算缝纫数据总长
		g_patd.sew_len = 0;
		j = g_patd.pat_add;
		for (i = 0; i < g_patd.feed_sum; i++)
		{
			ch = (U8)(SREG[j] & 0xff);
			if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61))
			{
				ch = (U8)(SREG[j + 1] & 0xff);
				if (ch >= 0x80)
					xtmp = (ch - 0x80) * (-1);
				else
					xtmp = ch;
				ch = (SREG[j + 1] >> 8) & 0xff;
				if (ch >= 0x80)
					ytmp = (ch - 0x80) * (-1);
				else
					ytmp = ch;
				g_patd.sew_len += sqrt(xtmp * xtmp + ytmp * ytmp);
			}
			j += 2;
			wdt();
		}
		//g_patd.sew_len = sqrt(g_patd.sew_len);
	}
	else if (Type == PAT_SEW)
	{ //SEW
		g_sewPTlen = 0;
		f = Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		if (f == 0)
			return;
		g_patd.pat_len = Patlen;
		g_patd.fno = F_IND_NO;
		g_patd.feed_pat = HREG[28];
		g_sewPTlen = sewInfo.ptsize;

		g_patd.endcode = SREG[sewInfo.ecodeaddr] >> 8;
		g_patd.year = ((SREG[sewInfo.timeaddr] & 0xff) << 8) + (SREG[sewInfo.timeaddr] >> 8);
		g_patd.month = SREG[sewInfo.timeaddr + 1] >> 8;
		g_patd.day = SREG[sewInfo.timeaddr + 2] >> 8;
		;
		g_patd.hour = SREG[sewInfo.timeaddr + 3] >> 8;
		;
		g_patd.min = SREG[sewInfo.timeaddr + 4] >> 8;
		;

		//初始坐标区域
		g_patxaddr = sewInfo.orgaddr;
		g_patyaddr = sewInfo.orgaddr + 1;
		g_patd.orgX = ((SREG[sewInfo.orgaddr] & 0xff) << 8) | ((SREG[sewInfo.orgaddr] >> 8 & 0xff));
		g_patd.orgY = ((SREG[sewInfo.orgaddr + 1] & 0xff) << 8) | ((SREG[sewInfo.orgaddr + 1] >> 8 & 0xff));
		//由花样数据计算花样范围 以针数据计算结果为实际范围
		Bor_CalPatLim((U16 *)SREG, Patlen, &g_patd.patmX, &g_patd.patiX, &g_patd.patmY, &g_patd.patiY, &g_patd.feed_sum); //花样范围使用针数据计算值
		if (g_patd.patmX < 0)
			g_patd.patmX = 0;
		if (g_patd.patiX > 0)
			g_patd.patiX = 0;
		if (g_patd.patmY < 0)
			g_patd.patmY = 0;
		if (g_patd.patiY > 0)
			g_patd.patiY = 0;

		SREG[sewInfo.limaddr] = (((g_patd.patmX) & 0xff) << 8) | ((((g_patd.patmX) >> 8) & 0xff));
		SREG[sewInfo.limaddr + 1] = (((g_patd.patiX) & 0xff) << 8) | ((((g_patd.patiX) >> 8) & 0xff));
		SREG[sewInfo.limaddr + 2] = (((g_patd.patmY) & 0xff) << 8) | ((((g_patd.patmY) >> 8) & 0xff));
		SREG[sewInfo.limaddr + 3] = (((g_patd.patiY) & 0xff) << 8) | ((((g_patd.patiY) >> 8) & 0xff));

		//---计算缝纫数据总长
		g_patd.sew_len = Sew_GetSewLen((U16 *)SREG, Patlen);
		//		#if Debug
		//		printf("g_patd.sew_len = %d \r\n",g_patd.sew_len);
		//		#endif

		checksum = get_checkSum((U16 *)SREG, DrawLen, PAT_SEW);
		//for(n=0;n<DrawLen;n++){
		//	if(n != 5) checksum +=SREG[n];
		//}
		SREG[5] = checksum;

		//花样名称
		for (i = 0; i < 64; i++)
		{
			g_patd.fname[i] = 0;
		}
		for (i = 0; i < 32; i++)
		{
			if ((SREG[i + sewInfo.nameaddr] & 0xff) == 0)
				break;
			g_patd.fname[2 * i] = (U8)(SREG[i + sewInfo.nameaddr] & 0xff);

			if (((SREG[i + sewInfo.nameaddr] >> 8) & 0xff) == 0)
				break;
			g_patd.fname[2 * i + 1] = (U8)((SREG[i + sewInfo.nameaddr] >> 8) & 0xff);
		}
		//删除不支持的符号
		for (i = 0, j = 0; i < 64; i++)
		{
			if (((g_patd.fname[i] >= 0x30) && (g_patd.fname[i] <= 0x39)) || ((g_patd.fname[i] >= 0x41) && (g_patd.fname[i] <= 0x5a)) ||
				((g_patd.fname[i] >= 0x61) && (g_patd.fname[i] <= 0x7a)) || (g_patd.fname[i] > 0x7f) || (g_patd.fname[i] == 0x5f))
			{
				g_patd.fname[j] = g_patd.fname[i];
				j++;
			}
		}
		for (; j < 64; j++)
		{
			g_patd.fname[j] = 0;
		}
		g_patd.fname[63] = 0;

		g_patd.scale = 100;
	}
	else if (Type == PAT_EMB)
	{ //EMB

		//#ifndef SYSTEM1
		//		f = emb_getMacTypeByPT((U8*)SREG,0xdc);
		//
		//		if(f==0xff)	return;
		//		g_patd.macType = f;
		//		emb_getComment((U8*)SREG,0x54,(U8*)&g_patd.fname);
		//		emb_getOrgs((U8*)SREG,g_patd.macType,&g_patd.orgX,&g_patd.orgY);
		//		emb_getLimit((U8*)SREG,g_patd.macType,&g_patd.patiX,&g_patd.patmX,&g_patd.patiY,&g_patd.patmY);
		//		//printf("g_patd.fname = %s f = %d  \r\n",g_patd.fname,f);
		//		g_MachineType = g_patd.macType ;
		//		emb_Analy((U8*)SREG,&areaMax,&ctrlMax,&pinMax);
		//		g_patd.feed_sum = pinMax;
		//		//HREG[28] = pinMax;
		//		pinAddr = (SREG[PA_ADDR+1]<<16) + SREG[PA_ADDR];
		//		pinlen = (SREG[PL_ADDR+1]<<16) + SREG[PL_ADDR];
		//		g_patd.pat_len = pinAddr + pinlen;
		//		g_patd.fno = F_IND_NO;
		//		g_patd.scale = 100;
		//#endif
	}
}

//------------------------------------------/
// Function   : AnaPattern	2				/
// Description: 分析三菱花样数据参数		/
//------------------------------------------/
void AnaPattern2(U8 mode)
{
	U8 ch;
	U32 i, j;
	S32 x, y;
	S32 xtmp, ytmp;

	U16 jj;
	S32 temp_x1 = 0, temp_y1 = 0;
	U8 kk = 0;

	if (((((U32)(SREG[5]) << 16) + SREG[4]) > 80000) || (SREG[2] < 0x20) || (SREG[2] > 64000))
	{ //2013.3.27
		//for(i=0;i<MAXSREG;i++) SREG[i] = 0;
		mysetU16((U16 *)SREG, 0, MAXSREG);
		SREG[0] = 0;	//文件号
		SREG[2] = 0x10; //线条数据启始位置在SREG[6000]位置
		SREG[3] = 6;
		SREG[4] = 4;	   //每组数据的针数据长最大为80000
		SREG[12] = 0x3e8;  //花样最小长度100mm
		SREG[14] = 0x3e8;  //花样最小宽度100mm
		SREG[16] = 0x454e; //文件名
		SREG[17] = 0x2057;
		SREG[18] = 0x2020;
		SREG[19] = 0x2020;
		SREG[30] = 0x3045; //版本
		SREG[31] = 0x3536;
		SREG[0x24] = 0;
		g_draw_p = 0x28; //绘图数据当前位置指针
		g_line_p = 0x29; //线条数据当前位置指针
		g_pat_p = 0x2b;	 //针数据当前位置指针
		g_init_flag = 0;
	}
	g_patd.ver[0] = (U8)(SREG[0x1e] & 0xff);
	g_patd.ver[1] = (U8)(SREG[0x1e] >> 8);
	g_patd.ver[2] = (U8)(SREG[0x1f] & 0xff);
	g_patd.ver[3] = (U8)(SREG[0x1f] >> 8);
	g_patd.ver[4] = 0;
	// 文件编号 	01-00
	g_patd.fno = SREG[0];
	// 文件名		20-2F
	g_patd.fname[0] = (U8)(SREG[0x10] & 0xff);
	g_patd.fname[1] = (U8)(SREG[0x10] >> 8);
	g_patd.fname[2] = (U8)(SREG[0x11] & 0xff);
	g_patd.fname[3] = (U8)(SREG[0x11] >> 8);
	g_patd.fname[4] = (U8)(SREG[0x12] & 0xff);
	g_patd.fname[5] = (U8)(SREG[0x12] >> 8);
	g_patd.fname[6] = (U8)(SREG[0x13] & 0xff);
	g_patd.fname[7] = (U8)(SREG[0x13] >> 8);
	g_patd.fname[8] = 0;
	// 最大移动XY	19-18 1D-1C
	g_patd.maxX = SREG[0x0c];
	g_patd.maxY = SREG[0x0e];

	g_patd.draw_add = 0x24;					// 绘图起始位置	48H
	g_patd.line_add = (SREG[2] + 0x40) / 2; // 线条起始位置	05-04+40H
	//patd.pat_add = (SREG[2]+0x40+SREG[4]/2)/2;
	g_patd.pat_add = (SREG[2] + 0x40 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 2)) / 2; //// 针起始位置	05-04+40H+09-08/2 2013.3.27
	// 花样总针数	09-08/4
	if (g_init_flag == 1)
	{
		//patd.feed_sum = SREG[4]/4;
		g_patd.feed_sum = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4); //2013.3.27
		g_draw_p = g_patd.line_add;
		g_line_p = g_patd.pat_add;
		g_pat_p = g_patd.pat_add + g_patd.feed_sum * 2;
	}
	else
	{
		g_patd.feed_sum = g_line_p - g_patd.line_add;
	}
	// 花样缝针数	09-08/4-非移动指令
	g_patd.p_2hd = 0xffff;
	if (mode == 1)
	{
		j = g_patd.pat_add;
		for (i = 0; i < g_patd.feed_sum; i++)
		{
			if (SREG[j] == 0x6)
			{
				g_patd.p_2hd = i;
				break;
			}
			j += 2;
		}
	}
	g_patd.X2hd = 0;
	g_patd.Y2hd = 0;
	if (g_patd.p_2hd != 0xffff)
	{
		j = g_patd.pat_add;
		for (i = 0; i < g_patd.p_2hd; i++)
		{
			ch = (U8)(SREG[j] & 0xff);
			if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61) || (ch == 0x3) || (ch == 0x1b))
			{
				ch = (U8)(SREG[j + 1] & 0xff);
				if (ch >= 0x80)
					xtmp = (ch - 0x80) * (-1);
				else
					xtmp = ch;
				ch = (SREG[j + 1] >> 8) & 0xff;
				if (ch >= 0x80)
					ytmp = (ch - 0x80) * (-1);
				else
					ytmp = ch;
				g_patd.X2hd += xtmp;
				g_patd.Y2hd -= ytmp;
			}
			j += 2;
		}
	}
	wdt(); // 喂狗
	x = 0;
	y = 0;
#if PATRULES_ORG_OFFSET
	if (SREG[3] == 7)
	{
		x = (S16)(SREG[6]);
		y = -(S16)(SREG[7]);
	}
#endif

	x += g_patd.X2hd;
	y += g_patd.Y2hd;
	g_patd.patmX = 0;
	g_patd.patiX = 0;
	g_patd.patmY = 0;
	g_patd.patiY = 0;
	if (x > g_patd.patmX)
		g_patd.patmX = x;
	if (y > g_patd.patmY)
		g_patd.patmY = y;
	if (x < g_patd.patiX)
		g_patd.patiX = x;
	if (y < g_patd.patiY)
		g_patd.patiY = y;
	g_patd.feed_pat = 0;
	j = g_patd.pat_add;

	jj = 1; ////////////////////////////////////add on 2009.6.7
	for (i = 0; i < g_patd.feed_sum; i++)
	{
		wdt(); // 喂狗	2010.3.27
		ch = (U8)(SREG[j] & 0xff);
		if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61))
		{
			g_patd.feed_pat++;
		}
		if ((ch == 0x01) || (ch == 0x21) || (ch == 0x41) || (ch == 0x61) || (ch == 0x3) || (ch == 0x1b))
		{
			if ((g_patd.p_2hd == 0xffff) || (g_patd.p_2hd < i))
			{
				ch = (U8)(SREG[j + 1] & 0xff);
				if (ch >= 0x80)
					xtmp = (ch - 0x80) * (-1);
				else
					xtmp = ch;
				ch = (SREG[j + 1] >> 8) & 0xff;
				if (ch >= 0x80)
					ytmp = (ch - 0x80) * (-1);
				else
					ytmp = ch;
				x += xtmp;
				y -= ytmp;
				/////////////////////////////Add on 2009.2.10
				if (mode == 5)
				{
					if (g_jogmode)
					{
						if (j == g_patd.pat_add + g_jog_stpoint * 2)
						{
							temp_x1 = x;
							temp_y1 = y;
							for (jj = 1; jj < g_point_num; jj++)
							{
								temp_x1 += g_array10[jj][0];
								temp_y1 -= g_array10[jj][1];

								if (temp_x1 > g_patd.patmX)
									g_patd.patmX = temp_x1;
								if (temp_y1 > g_patd.patmY)
									g_patd.patmY = temp_y1;
								if (temp_x1 < g_patd.patiX)
									g_patd.patiX = temp_x1;
								if (temp_y1 < g_patd.patiY)
									g_patd.patiY = temp_y1;
							}
							temp_x1 += g_dxpos;
							temp_y1 -= g_dypos;
						}
					}
					else
					{
						if ((jj <= g_point_num) /*||(bpno1 != MENU_MODIFYADD)*/)
						{
							if (j == g_patd.pat_add + g_array10[300 + jj][0] * 2)
							{
								temp_x1 = x; ////////////++++++++++++++
								temp_y1 = y; ///////////++++++++++++++++

								if (jj == g_point_num)
								{
									temp_x1 += g_dxpos;
									temp_y1 -= g_dypos;
								}
								else
								{
									temp_x1 += g_array10[jj][0];
									temp_y1 -= g_array10[jj][1];
								}
								if (temp_x1 > g_patd.patmX)
									g_patd.patmX = temp_x1;
								if (temp_y1 > g_patd.patmY)
									g_patd.patmY = temp_y1;
								if (temp_x1 < g_patd.patiX)
									g_patd.patiX = temp_x1;
								if (temp_y1 < g_patd.patiY)
									g_patd.patiY = temp_y1;

								jj++;

								if (j == (g_patd.pat_add + g_array10[300 + jj][0] * 2))
								{
									kk = 1;
								} //2013.5.24
							}
						}
					}
				}
				//////////////////////////////////////////////////////
				if (x > g_patd.patmX)
					g_patd.patmX = x;
				if (y > g_patd.patmY)
					g_patd.patmY = y;
				if (x < g_patd.patiX)
					g_patd.patiX = x;
				if (y < g_patd.patiY)
					g_patd.patiY = y;

				if (kk == 1)
				{
					j -= 2;
					x -= xtmp;
					y += ytmp;
					kk = 0;
				}
			}
		}
		j += 2;
	}
	if ((mode == 5) && (g_jogmode == 1))
	{
		if (temp_x1 > g_patd.patmX)
			g_patd.patmX = temp_x1;
		if (temp_y1 > g_patd.patmY)
			g_patd.patmY = temp_y1;
		if (temp_x1 < g_patd.patiX)
			g_patd.patiX = temp_x1;
		if (temp_y1 < g_patd.patiY)
			g_patd.patiY = temp_y1;
	}
	if (mode != 5)
		g_patd.scale = 100;
}

#define DISP_PARA_X 1 //x缩放因子
#define DISP_PARA_Y 1 //y缩放因子

//------------------------------------------
// Function   : void GetDrawPatPos(U32 feedc,S32 scale,S16 ox1,S16 oy1,U8 Type,U32 Patlen,U8 flag)
// Description: 显示针位置
// U16 feedc  :  当前针数
//	U8 Type	  :  类型 0-三菱  1-兄弟
// U32 Patlen:花样数据总长 针对兄弟款   2016-12-16
//------------------------------------------
void GetDrawPatPos(U32 feedc, S32 scale, S16 *ox, S16 *oy, U8 Type, U32 Patlen, U8 flag)
{
	U8 ch, f;
	U32 i, j;
	S32 x, y;
	S16 xend = 0, yend = 0;
	S32 xtmp = 0, ytmp = 0;
	U32 tmpfeed = 0;
	S16 xorg = 0, yorg = 0;
	S32 tmpScale;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	//	#if Debug
	//	printf("GetDrawPatPos feedc = %d Type = %d  flag = %d \r\n",feedc,Type,flag);
	//	#endif

	if (Type == PAT_SEW)
	{
		scale /= 2;
	}
	tmpScale = scale;
	scale = 0;

	if (flag == 0)
	{
		xtmp = IMAGE_X3 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y3 * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;

		xorg = CONVAS_ORG_PX3;
		yorg = CONVAS_ORG_PY3;
	}
	else if (flag == 1)
	{
		xtmp = 49000 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = 33200 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY));

		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;

		xorg = 384;
		yorg = 304;
	}
	else if (flag == 2)
	{
		xorg = 260;
		yorg = 258;
		//DrawFillRect(84,114,352,288,Color_Silver);
		xtmp = 34400 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = 28000 / (g_maxY - g_minY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}

	*ox = xorg;
	*oy = yorg;

	scale += tmpScale;
	if (scale < 10)
		scale = 10;

	if (Type == PAT_MIT)
	{ //三菱款
		if (flag == 2)
		{
			xorg += (g_maxX + g_minX) / 2 * scale / 1000;
			yorg += (g_maxY + g_minY) / 2 * scale / 1000;
		}
		else
		{
			xorg += (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
			yorg += (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;
		}

		*ox = xorg;
		*oy = yorg;
		if (feedc)
		{
			x = 0;
			y = 0;
#if PATRULES_ORG_OFFSET
			if (SREG[3] == 7)
			{
				x = (S16)(SREG[6]);
				y = -(S16)(SREG[7]);
			}
#endif

			j = g_patd.pat_add + 1;
			for (i = 0; i < feedc; i++)
			{
				if (SREG[j - 1] != 0x3a)
				{
					ch = (U8)(SREG[j] & 0xff);
					if (ch >= 0x80)
					{
						ch -= 0x80;
						x -= ch;
					}
					else
					{
						x += ch;
					}
					ch = (SREG[j] >> 8) & 0xff;
					if (ch >= 0x80)
					{
						ch -= 0x80;
						y += ch;
					}
					else
					{
						y -= ch;
					}
				}
				j += 2;
			}
			x = (S32)(x * scale / 1000 + xorg);
			y = (S32)(y * scale / 1000 + yorg);

			*ox = xorg + (xorg - x);
			*oy = yorg + (yorg - y);
		}
	}
	else if (Type == PAT_SEW)
	{ // 兄弟款

		f = Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		if (f == 0)
			return;
		if (flag == 2)
		{
			xorg += (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
			yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;
		}
		else
		{
			xorg += (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
			yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;
		}

		if (feedc)
		{
			xend = g_patd.orgX;
			yend = g_patd.orgY;

			for (i = 0; (i < (sewInfo.ptsize / 2)) && (tmpfeed < feedc);)
			{
				f = Bor_DrawCmd((U16 *)(&SREG[i + sewInfo.pataddr]), &sewIcmd);
				if (f)
				{
					if (sewIcmd.cmd == 0xff00)
					{
						break;
					}
					tmpfeed++;
					if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
					{
						xend += sewIcmd.dx;
						yend += sewIcmd.dy;
					}
					i += sewIcmd.off;
				}
				else
					i++;
			}

			x = (U32)(xorg + xend * scale / 1000);
			y = (U32)(yorg - yend * scale / 1000);
			//			xend = xst;
			//			yend = yst;

			//			 xend = xst + xtmp;
			//			 yend = yst + ytmp;

			//			 DrawLine(xorg + xst*scale/1000,yorg - yst*scale/1000,xorg + xend*scale/1000,yorg - yend*scale/1000,sewColor);

			*ox = xorg + (xorg - x);
			*oy = yorg + (yorg - y);
			//			#if Debug
			//			printf("x = %d  y = %d  *ox = %d *oy = %d xorg = %d yorg = %d---\r\n",x,y,*ox,*oy,xorg,yorg);
			//			#endif
		}
		else if (g_BorSTflag)
		{
			x = (U32)(xorg + g_patd.orgX * scale / 1000);
			y = (U32)(yorg - g_patd.orgY * scale / 1000);
			*ox = xorg + (xorg - x);
			*oy = yorg + (yorg - y);
		}
	}
}

//绘制大十字点
void DrawBigPoint(U16 x, U16 y, U16 color)
{
	U8 i;
	for (i = 0; i < 3; i++)
	{
		DrawHLine(x - 3, y - 1 + i, 7, color);
		DrawVLine(x - 1 + i, y - 3, 7, color);
	}
	wdt();
}
//绘制大矩形点
void DrawBigRectPoint(U16 x, U16 y, U16 color)
{
	U8 i;
	for (i = 0; i < 5; i++)
	{
		DrawHLine(x - 2, y - 2 + i, 5, color); //绘制针位点
	}
	wdt();
}

void newDrawPat(U32 vn, S32 scale, S16 ox1, S16 oy1, U16 tx, U16 ty)
{
	U8 colorf = 0; //,cw
	U32 i, j, n;
	U16 tmpfeed = 0, ptmpfeed = 0;
	U16 dat, dat1; //,dat2
	U32 pat_add_p;
	U8 mdx, mdy;
	S16 ox, oy, x, y;
	S32 xtmp, ytmp;
	S32 tmpScale = 0;
	S32 x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	U16 k = 0;
	S16 k1 = 0, k2 = 0;
	S16 xorg = 0, yorg = 0;
	//	S32 tmpScale=0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	//	U8 tmpISdat =0;
	U8 f;

	S32 xst = 0, xend = 0, yst = 0, yend = 0;
	U16 sewColor = COLOR_LINE_SEW;
	U8 mut = 1;
	S16 dxp = 10, dyp = 10; //定位点误差10个像素点
	U32 code, dat3;			//,dat2
	U16 code1;
	S16 xx, yy;
	//	if(F_FILETYPE==2){	//sew
	//		scale /=2;
	//	} 16,104,568,316
	//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19

	if (vn == 1 || vn == 2 || vn == 6 || vn == 8 || vn == 9 || vn == 12)
	{
		tmpScale = scale;
		scale = 0;
	}
	g_patd.scale = scale;
	g_dispat.feedc = 0;
	// 最大比例
	if (g_maxX == g_minX)
		g_maxX++;
	if (g_maxY == g_minY)
		g_maxY++;

	wdt();

	if (vn == 0 || vn == 5 || vn == 7 || vn == 11 || vn == 1 ||
		vn == 2 || vn == 6 || vn == 8 || vn == 9 || vn == 12 ||
		(vn == 3 || vn == 4 || (vn >= 13 && vn <= 16)) || (vn >= 17 && vn <= 25))
	{
		tmpScale = scale;
		scale = 0;
	}
	g_patd.scale = scale;
	g_dispat.feedc = 0;
	// 最大比例
	if (g_maxX == g_minX)
		g_maxX++;
	if (g_maxY == g_minY)
		g_maxY++;

	wdt();
	if (F_FILETYPE == PAT_SEW)
	{ //sew
#if MOVEUNIT == 0
		mut = 2;
#endif
	}

	if (vn == 0 || vn == 5 || vn == 7 || vn == 10 || vn == 11 || vn == 26) // 修改画面1坐标移动、数字输入
	{
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y9 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
	}
	else if (vn == 1 || vn == 6 || vn == 8 || vn == 9 || vn == 12) //花样修改界面-挪动压框位置//放大查看花样
	{
		xorg = CONVAS_ORG_PX8;
		yorg = CONVAS_ORG_PY8;
		colorf = 1;
		DrawFillRect(CONVAS_PX8, CONVAS_PY8, CONVAS_X8, CONVAS_Y8, CONVAS_COLOR_B);
		xtmp = IMAGE_X8 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y8 * 100 / (g_maxY - g_minY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX8, CONVAS_PY8, CONVAS_X8 + CONVAS_PX8, CONVAS_Y8 + CONVAS_PY8, 1);
	}
	else if (vn == 2) //寸动显示图形
	{
		xorg = 260;
		yorg = 258;
		colorf = 1;
		//DrawFillRect(84,114,352,288,COLOR_CONVAS);
		xtmp = 34400 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = 28000 / (g_maxY - g_minY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(84, 114, 436, 398, 1);
	}
	else if (vn == 3 || vn == 13 || vn == 15 || vn == 4 || vn == 14 || vn == 16) //花样修改界面-选择针点
	{
		xorg = CONVAS_ORG_PX7;
		yorg = CONVAS_ORG_PY7;
		colorf = 0;

		DrawFillRect(CONVAS_PX7, CONVAS_PY7, CONVAS_X7, CONVAS_Y7, CONVAS_COLOR_B);
		xtmp = IMAGE_X7 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y7 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX7, CONVAS_PY7, CONVAS_X7 + CONVAS_PX7, CONVAS_Y7 + CONVAS_PY7, 1);
	}
	else if (vn == 17 || vn == 18 || vn == 19) //打板界面
	{
		//xorg = 300;
		//yorg = 255;
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X5 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y5 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX5, CONVAS_PY5, CONVAS_PX5 + CONVAS_X5, CONVAS_PY5 + CONVAS_Y5, 1);
		DrawHLine(CONVAS_PX5, CONVAS_ORG_PY5, CONVAS_X5, CONVAS_COLOR_T);
		DrawVLine(CONVAS_ORG_PX5, CONVAS_PY5, CONVAS_Y5, CONVAS_COLOR_T);
	}
	else if (vn == 20) //花样修改-速度修正
	{
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y9 * 100 / (g_maxY - g_minY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
	}
	else if (vn == 21 || vn == 22)
	{
		//xorg = 300+ox1;
		//yorg = 198+oy1;
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y9 * 100 / (g_maxY - g_minY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
	}
	else if (vn == 23)
	{
		//xorg = 300+ox1;
		//yorg = 230+oy1;
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y9 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
	}
	else if (vn == 24)
	{
		//xorg = 300+ox1;
		//yorg = 198+oy1;
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y9 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
	}
	else if (vn == 25) //花样修改-添加、删除代码
	{
		xorg = ox1;
		yorg = oy1;
		colorf = 0;
		xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = IMAGE_Y9 * 100 / (g_maxY - g_minY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
	}

	//获取画布原点
	if ((ox1 != xorg) || (oy1 != yorg))
	{
		if (vn == 1 || vn == 2 || vn == 6 || vn == 20 || vn == 25 || vn == 26 || (vn == 3 || vn == 4 || (vn >= 13 && vn <= 16)))
		{
			xorg = ox1;
			yorg = oy1;
		}
	}
	scale += tmpScale;
	if (scale < 10)
		scale = 10;

	if ((g_patd.scale == 0) || (g_patd.scale > scale))
	{
		g_patd.scale = scale;
	}
	else
	{
		scale = g_patd.scale;
	}

	if (F_FILETYPE == PAT_MIT)
	{												  //mit
		xorg -= (g_maxX + g_minX) * scale / 2 / 1000; //获取原点坐标
		yorg -= (g_maxY + g_minY) * scale / 2 / 1000;

		//if(vn>=17&&vn<=19){
		//	if((ox1!=xorg)||(oy1!=yorg)){
		//		xorg = ox1;
		//		yorg = oy1;
		//	}
		//}
		if (vn == 1 || vn == 6 || vn == 8 || vn == 9 || vn == 12)
		{
			DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_ORG, 0);
		}
		else
		{
			DrawBigPoint(xorg, yorg, COLOR_ORG);
		}

		if (vn == 23 || vn == 24)
		{
			U32 line_add_p = g_patd.line_add;
			//U32 temp_line_add = g_patd.line_add;
			//U32 temp_pat_add = g_patd.pat_add;
			U32 temp_line_add = g_draw_p; // 线条起始地址，用于限制绘图数据的范围
			U32 temp_pat_add = g_line_p;  // 针起始地址，用于限制线条数据的范围
			U8 sameline_f;
			U8 nn = 0;
			ox = 0;
			oy = 0;
			g_array5[0][0] = 0;

			pat_add_p = g_patd.pat_add;
			j = g_patd.draw_add;
			//printf("j = %d  temp_line_add = %d  g_init_flag = %d \r\n",j,temp_line_add,g_init_flag);
			while (j < temp_line_add)
			{
				wdt(); // 喂狗
				code = SREG[j];
				code1 = SREG[j + 1];
				dat = code & 0x3ff; ////////////////////////////////////////////+++++++++++++++++++

				if ((code & 0xfe00) == 0x1800)
				{ // 记录多重缝、补助缝的启点位置及编号
					if (vn == 23)
					{
						xx = ox;
						yy = oy;
						if ((xx == 0) && (yy == 0))
							xx = 1;
					}
					else
					{
						xx = 0;
						yy = 0;
					}
				}
				else if (vn == 24 && (code & 0x9800))
				{
					xx = ox;
					yy = oy;
					if ((xx == 0) && (yy == 0))
						xx = 1;
				}
				else
				{
					xx = 0;
					yy = 0;
				}
				//printf("ox = %d  oy = %d dat = %04x ",ox,oy,dat);
				switch (dat)
				{
				case 0x0088: // 移送
					dat = (SREG[line_add_p] & 0x3fff);
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);
						if (dat != dat1)
							break;
						k++;
					}
					n = k - line_add_p; //针数据的个数
					line_add_p = k;
					x = 0;
					y = 0;
					//printf("n = %d  pat_add_p = %d \r\n",n,pat_add_p);
					for (k = 0; k < n; k++)
					{
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						//printf("SREG[%d] = %04x SREG[%d] = %04x x = %d  y = %d \r\n",pat_add_p,SREG[pat_add_p],pat_add_p+1,SREG[pat_add_p+1],x,y);
						pat_add_p += 2;
					}
					//printf("x = %d y = %d ox = %d oy = %d ",x,y,ox,oy);
					x = ox + x;
					y = oy - y;
					//printf("x = %d y = %d ",x,y);
					ox = x;
					oy = y;
					xx = 0;
					yy = 0;
					break;
				case 0x00c4: // 点
					if (SREG[j + 2] > 0x8000)
						xtmp = (0x10000 - SREG[j + 2]) * (-1);
					else
						xtmp = SREG[j + 2];
					if (SREG[j + 3] > 0x8000)
						ytmp = (0x10000 - SREG[j + 3]) * (-1);
					else
						ytmp = SREG[j + 3];
					x = ox + xtmp;
					y = oy - ytmp;
					line_add_p++;
					pat_add_p += 2;
					ox = x;
					oy = y;
					xx = 0;
					yy = 0;
					break;
				case 0x00c8: // 直线
				case 0x00ca: // 直线_Z字缝
				case 0x01ce: // 园_Z字缝
				case 0x01d2: // 园弧_Z字缝
				case 0x00d6: // 曲线结束_Z字缝
					dat = (SREG[line_add_p] & 0x3fff);
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);
						if (dat != dat1)
							break;
						k++;
					}
					n = k - line_add_p;
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat1 = SREG[pat_add_p];
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						dat = SREG[pat_add_p + 2];
						if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
						{
							dat = SREG[pat_add_p + 3];
							x2 = (U8)(dat);
							y2 = (U8)((U16)(dat) >> 8);
							if (((abs((S32)(x1)-x2)) < 2) && ((abs((S32)(y1)-y2)) < 2))
							{
								if ((k + 1) < n)
									sameline_f = 1;
								else
									sameline_f = 0;
							}
							else
							{
								sameline_f = 0;
							}
						}
						else
						{
							sameline_f = 0;
						}
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						if (sameline_f == 0)
						{
							x = ox + x;
							y = oy - y;
							//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
							//x2 = (S32)(x*scale/1000+xorg);    y2 = (S32)(y*scale/1000+yorg);
							//if((dat1==0x3)||(dat1==0x1b));//DrawLine(x1, y1, x2, y2,Color_DimGray);
							//else DrawLine(x1, y1, x2, y2,sewColor);
							ox = x;
							oy = y;
							x = 0;
							y = 0;
						}
						pat_add_p += 2;
					}
					break;
				case 0x00ce: // 园结束_Z字缝
				case 0x00d2: // 园弧结束_Z字缝
				case 0x01d6: // 曲线_Z字缝
				case 0x02d6: // 曲线_Z字缝
					xx = 0;
					yy = 0;
					break;
				case 0x01cc: // 园
				case 0x01d0: // 园弧
					dat1 = (SREG[j + 4]) & 0x1ff;
					if (dat != (dat1 + 0x100))
						break;
					/////////////////////////////////////////  2011.4.11
					dat3 = code & 0xfe00;
					if (dat3 == 0x800)
						dat3 = 1;
					else if (dat3 == 0x1000)
						dat3 = 2;
					else if (dat3 == 0x1800)
						dat3 = 3;
					else if (dat3 == 0x8800)
						dat3 = 4;
					else if (dat3 == 0x9000)
						dat3 = 5;
					else
						dat3 = 0;

					dat = (SREG[line_add_p] & 0x3fff);
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);
						if (dat != dat1)
							break;
						k++;
					}
					n = k - line_add_p;
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat1 = SREG[pat_add_p];
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						dat = SREG[pat_add_p + 2];
						if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
						{
							dat = SREG[pat_add_p + 3];
							x2 = (U8)(dat);
							y2 = (U8)((U16)(dat) >> 8);
							sameline_f = 0;
						}
						else
						{
							sameline_f = 0;
						}
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						if (sameline_f == 0)
						{
							x = ox + x;
							y = oy - y;
							//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
							//x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
							//if((dat1==0x3)||(dat1==0x1b));//DrawLine(x1, y1, x2, y2,Color_DimGray);
							//else DrawLine(x1, y1, x2, y2,sewColor);
							ox = x;
							oy = y;
							x = 0;
							y = 0;
						}
						pat_add_p += 2;
					}
					break;
				case 0x00cc: // 园结束
				case 0x00d0: // 园弧结束
					xx = 0;
					yy = 0;
					break;
				case 0x01d4: // 曲线
				case 0x00d4:
				case 0x02d4:
					if (((SREG[j - 4] != 0x01d4) && (dat != 0x01d4)) || (dat == 0x01d4))
					{ //2009.6.16 ,修改了单独0x0d4,0x2d4开头不能正常显示的问题
						n = 1;
						if (dat != 0xd4)
						{
							while ((j + 4) < temp_line_add)
							{
								dat = (SREG[j + 4]) & 0x3fd;
								if ((dat != 0x1d4) && (dat != 0x2d4))
								{
									break;
								}
								n++;
								j += 4;
							}
							n++;
							j += 4;
						}

						if ((SREG[j] & 0x1fc) != 0xd4)
							break;

						dat = (SREG[line_add_p] & 0x3fff);
						k = line_add_p;
						while (k < temp_pat_add)
						{
							dat1 = (SREG[k] & 0x3fff);
							if (dat != dat1)
								break;
							k++;
						}
						n = k - line_add_p;
						line_add_p = k;
						x = 0;
						y = 0;
						for (k = 0; k < n; k++)
						{
							dat1 = SREG[pat_add_p];
							dat = SREG[pat_add_p + 1];
							x1 = (U8)(dat);
							y1 = (U8)((U16)(dat) >> 8);
							dat = SREG[pat_add_p + 2];
							if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
							{
								dat = SREG[pat_add_p + 3];
								x2 = (U8)(dat);
								y2 = (U8)((U16)(dat) >> 8);
								sameline_f = 0;
							}
							else
							{
								sameline_f = 0;
							}
							if (x1 >= 0x80)
							{
								x1 -= 0x80;
								x -= x1;
							}
							else
								x += x1;
							if (y1 >= 0x80)
							{
								y1 -= 0x80;
								y -= y1;
							}
							else
								y += y1;
							if (sameline_f == 0)
							{
								x = ox + x;
								y = oy - y;
								//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
								//x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
								//if((dat1==0x3)||(dat1==0x1b));
								//else DrawLine(x1, y1, x2, y2,sewColor);
								ox = x;
								oy = y;
								x = 0;
								y = 0;
							}
							pat_add_p += 2;
						}
					}
					else
					{
						xx = 0;
						yy = 0;
					}
					break;
				case 0x00d8: // 多边形
				case 0x01d8: // 多边形结束
				case 0x00da: // 多边形_Z字缝
				case 0x01da: // 多边形结束_Z字缝
					i = 1;
					while ((j + 4) < temp_line_add)
					{
						dat = (SREG[j + 4]) & 0xfc;
						if (dat != 0xd8)
						{
							break;
						}
						i++;
						j += 4;
					}
					dat = (SREG[line_add_p] & 0x3fff);
					dat += i;
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);

						if (dat <= dat1)
							break;
						k++;
					}
					n = k - line_add_p;
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat1 = SREG[pat_add_p];
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						dat = SREG[pat_add_p + 2];
						if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
						{
							dat = SREG[pat_add_p + 3];
							x2 = (U8)(dat);
							y2 = (U8)((U16)(dat) >> 8);
							if (((abs((S32)(x1)-x2)) < 2) && ((abs((S32)(y1)-y2)) < 2))
							{
								if ((k + 1) < n)
									sameline_f = 1;
								else
									sameline_f = 0;
							}
							else
							{
								sameline_f = 0;
							}
						}
						else
						{
							sameline_f = 0;
						}
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						if (sameline_f == 0)
						{
							x = ox + x;
							y = oy - y;
							//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
							//x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
							//if((dat1==0x3)||(dat1==0x1b));//DrawLine(x1, y1, x2, y2,Color_DimGray);
							//else DrawLine(x1, y1, x2, y2,sewColor);
							ox = x;
							oy = y;
							x = 0;
							y = 0;
						}
						pat_add_p += 2;
					}
					break;

				default:
					xx = 0;
					yy = 0;
					break;
				}
				wdt(); // 喂狗	2010.3.27
				//printf("code = %04x \r\n",code);
				switch (code)
				{
				case 0x0401: // 针上停
				case 0x1401: // 针下停
				case 0x0601: // 次原点
				case 0x0701: // 跳缝
				case 0x1c01: // 伸延=物料厚 压板重 移送速度 重启动
				case 0x3a01: // 面线张力  2013.4.8
				case 0x0201: // 剪线
				case 0x0501: // 功能一
				case 0x0b01: // 功能二
				case 0x0c01: // 功能三
				case 0x0d01: // 功能四
				case 0x3c01: // 针角度
				case 0x3d01: // 参数一
				case 0x3e01: // 参数二
					line_add_p++;
					pat_add_p += 2;
					break;	 // ?
				case 0x1d01: // 功能五 六 七 八
					if ((code1 == 0x03) && (g_UdZhouFlag == 3))
					{ //换针
						if (sewColor == COLOR_LINE_SEW)
							sewColor = COLOR_LINE_CHANGE;
						else
							sewColor = COLOR_LINE_SEW;
					}
					line_add_p++;
					pat_add_p += 2;
					break; // ?

				case 0x1e01: // 结束
				case 0x1f01: // 结束
					ox = x;
					oy = y;
					break;
				default:
					break;
				}

				j += 4;
				//printf("xx = %d   yy = %d ox = %d  oy = %d\r\n",xx,yy,ox,oy);
				if ((xx != 0) || (yy != 0))
				{ // 记录多重缝、补助缝的启点位置及编号
					nn++;
					g_array5[nn][0] = xx;
					g_array5[nn][1] = yy;
					g_array5[0][0] = nn;
				}
			}
		}

		// /[计算有无次原点]******************************************************
		x = 0;
		y = 0;
		ox = 0;
		oy = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			ox = (S16)(SREG[6]);
			oy = -(S16)(SREG[7]);
			if (ox != 0 || oy != 0)
			{
				x1 = (S32)(xorg);
				y1 = (S32)(yorg);
				x2 = (S32)(ox * scale / 1000 + xorg);
				y2 = (S32)(oy * scale / 1000 + yorg);
				DrawLine(x1, y1, x2, y2, COLOR_LINE_H2H2); // 原点到次原点Color_Orange
			}
		}
#endif

		// /[绘制针点与缝纫线]******************************************************
		pat_add_p = g_patd.pat_add;
		for (i = 0; i < g_patd.feed_sum; i++)
		{
			dat1 = SREG[pat_add_p];
			dat = SREG[pat_add_p + 1];
			if ((dat1 & 0xff) == 0x1 || (dat1 & 0xff) == 0x21 || (dat1 & 0xff) == 0x41 || (dat1 & 0xff) == 0x61 || (dat1 & 0xff) == 0x3 || (dat1 & 0xff) == 0x1b)
			{
				mdx = (U8)(dat);
				mdy = (U8)(dat >> 8);
			}
			else
			{
				mdx = 0;
				mdy = 0;
			}

			if (mdx >= 0x80)
			{
				mdx -= 0x80;
				x -= mdx;
			}
			else
				x += mdx;
			if (mdy >= 0x80)
			{
				mdy -= 0x80;
				y -= mdy;
			}
			else
				y += mdy;
			x = ox + x;
			y = oy - y;
			x1 = (S32)(ox * scale / 1000 + xorg);
			y1 = (S32)(oy * scale / 1000 + yorg);
			x2 = (S32)(x * scale / 1000 + xorg);
			y2 = (S32)(y * scale / 1000 + yorg);
			//if(vn==5)printf("i = %d, g_jog_point = %d  g_parameter = %d \r\n",i,g_jog_point,g_parameter);
			if ((vn == 5 || vn == 26 || vn == 20) && i >= g_jog_point && i < (g_jog_point + g_parameter))
			{
				DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH); //修改 RED
			}
			else if ((vn == 7 || vn == 11 || vn == 15 || vn == 10) && (i >= g_jog_stpoint && i < g_jog_enpoint))
			{
				DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH); //修改 RED
			}
			else if ((vn == 3 || vn == 4 || vn == 13 || vn == 14 || vn == 15 || vn == 16) &&
					 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
					  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
			{
				DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH); //修改 RED
			}
			else
			{
				if (((dat1 & 0xff) == 0x3) || ((dat1 & 0xff) == 0x1b))
					DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY); //空送
				else
					DrawLine(x1, y1, x2, y2, sewColor); //缝纫
			}

			if ((SREG[pat_add_p] == 0x031d) && (g_UdZhouFlag == 3)) //换针
			{
				if (sewColor == COLOR_LINE_SEW)
					sewColor = COLOR_LINE_CHANGE;
				else
					sewColor = COLOR_LINE_SEW;
			}

			if (((dat1 & 0xff) == 0x1 || (dat1 & 0xff) == 0x21 || (dat1 & 0xff) == 0x41 || (dat1 & 0xff) == 0x61))
			{
				// DrawHLine(x1 - 1, y1, 3, Color_Black); //绘制针点
				// DrawVLine(x1, y1 - 1, 3, Color_Black);

				// DrawHLine(x2 - 1, y2, 3, Color_Black); //绘制针点
				// DrawVLine(x2, y2 - 1, 3, Color_Black);

				// DrawPoint(x1, y1, COLOR_PIN_SEW);
				// DrawPoint(x2, y2, COLOR_PIN_SEW);
				if ((vn == 5 || vn == 26 || vn == 20) && i >= g_jog_point && i < (g_jog_point + g_parameter))
				{
					DrawPoint(x1, y1, COLOR_PIN_FINISH);
					DrawPoint(x2, y2, COLOR_PIN_FINISH);
				}
				else if ((vn == 7 || vn == 11 || vn == 15 || vn == 10) && (i >= g_jog_stpoint && i < g_jog_enpoint))
				{
					DrawPoint(x1, y1, COLOR_PIN_FINISH);
					DrawPoint(x2, y2, COLOR_PIN_FINISH);
				}
				else if ((vn == 3 || vn == 4 || vn == 13 || vn == 14 || vn == 15 || vn == 16) &&
						 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
						  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
				{
					DrawPoint(x1, y1, COLOR_PIN_FINISH);
					DrawPoint(x2, y2, COLOR_PIN_FINISH);
				}
				else
				{
					DrawPoint(x1, y1, COLOR_PIN_SEW);
					DrawPoint(x2, y2, COLOR_PIN_SEW);
				}
			}

			// /[绘制控制点]***************************************
			if (vn == 15 || vn == 16 || vn == 10) //修改-区段修正的的控制点
			{
				for (k = 1; k < g_point_num; k++)
				{
					if (i == g_array5[300 + k][0])
					{
						k1 = (S16)(g_array5[k][0]);
						k2 = (S16)(g_array5[k][1]);
						x2 = (S32)((ox + k1) * scale / 1000 + xorg);
						y2 = (S32)((oy - k2) * scale / 1000 + yorg);
						DrawSerialNumber2(x2, y2, k, colorf, COLOR_CTRL);
						//DrawSerialNumber2(x2,y2,k,colorf,RGB(224,87,38));
						// DrawBigPoint(x2, y2, (k == (g_point_num - 1)) ? COLOR_SELECT_P : COLOR_SELECT_E);
					}
				}
			}
			else if (vn == 9) //修改2-区段修正的的控制点
			{
				for (k = 1; k < g_point_num; k++)
				{
					if (i == g_array5[300 + k][0])
					{
						k1 = (S16)(g_array5[k][0]);
						k2 = (S16)(g_array5[k][1]);
						x2 = (S32)((ox + k1) * scale / 1000 + xorg);
						y2 = (S32)((oy - k2) * scale / 1000 + yorg);
						DrawSerialNumber2(x2, y2, k, colorf, COLOR_CTRL);
					}
				}
			}
			if (vn == 2)
			{
				if ((((abs(x1 - tx) * abs(x1 - tx) + abs(y1 - ty) * abs(y1 - ty)) < (dxp * dxp + dyp * dyp))) && (i >= g_min_feedsum) && (i <= g_max_feedsum))
				{
					dxp = abs(x1 - tx);
					dyp = abs(y1 - ty);
					g_LPoint = i;
				}
			}
			ox = x;
			oy = y;
			x = 0;
			y = 0;

			pat_add_p += 2;
		}

		if ((vn == 23 || vn == 24) && g_array5[0][0]) //变换-修改多重缝段
		{
			for (i = 1; i <= g_array5[0][0]; i++)
			{
				x1 = (S16)(g_array5[i][0]);
				y1 = (S16)(g_array5[i][1]);
				x1 = (S32)(x1 * scale / 1000 + xorg);
				y1 = (S32)(y1 * scale / 1000 + yorg);
				//DrawSerialNumber2(x1,y1,i,colorf,RGB(224,87,38));
				DrawSerialNumber2(x1, y1, i, 1, COLOR_CTRL);
			}
		}

		//绘制指令代码
		if (vn == 0 || vn == 3 || vn == 4 || vn == 5 || vn == 7 ||
			vn == 13 || vn == 14 || vn == 11 || vn == 15 || vn == 10 ||
			vn == 26 || vn == 21 || vn == 22 || vn == 23 || vn == 24 || vn == 17 ||
			vn == 20 || vn == 25)
		{
			ox = 0;
			oy = 0;
#if PATRULES_ORG_OFFSET
			if (SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}
#endif
			x = 0;
			y = 0;
			i = g_patd.pat_add;
			for (k = 0; k < g_patd.feed_sum; k++)
			{
				dat1 = SREG[i] & 0xff;
				//printf("dat1 = %02x \r\n",dat1);
				if ((dat1 == 0x1) || (dat1 == 0x21) || (dat1 == 0x41) || (dat1 == 0x61) || (dat1 == 0x3) || (dat1 == 0x1b))
				{
					dat = SREG[i + 1];
					x1 = (U8)(dat);
					y1 = (U8)((U16)(dat) >> 8);
					if (x1 >= 0x80)
					{
						x1 -= 0x80;
						x -= x1;
					}
					else
						x += x1;
					if (y1 >= 0x80)
					{
						y1 -= 0x80;
						y -= y1;
					}
					else
						y += y1;
					x = ox + x;
					y = oy - y;

					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					ox = x;
					oy = y;
					x = 0;
					y = 0;
					//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);

					//printf("x2 = %d,y2 = %d  x = %d,y = %d \r\n",x2,y2,x,y);
					if (dat1 == 0x1 || dat1 == 0x21 || dat1 == 0x41 || dat1 == 0x61)
					{
						dat1 = (SREG[i] >> 8) & 0xff;
						if (dat1 != 0)
						{
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_P);
						}
					}
				}
				else
				{
					//printf("dat1 = %02x \r\n",dat1);
					switch (dat1)
					{
					case 0x06:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_H2);
						break;
					case 0x3a:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_T);
						break;
					case 0x04:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_U);
						break;
					case 0x14:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_D);
						break;
					case 0x07:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_J);
						break;
					case 0x05:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F1);
						break;
					case 0x0b:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F2);
						break;
					case 0x0c:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F3);
						break;
					case 0x0d:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F4);
						break;
					case 0x1d:
						dat1 = (SREG[i] >> 8) & 0xff;
						switch (dat1)
						{
						case 0x00:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F5);
							break;
						case 0x01:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F6);
							break;
						case 0x02:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F7);
							break;
						case 0x03:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F8);
							break;
						}
						break;
					case 0x02:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_C);
						break;
					}
				}
				wdt();
				i += 2;
			}
		}
	}
	else if (F_FILETYPE == PAT_SEW)
	{ //sew
		f = Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);
		if (f == 0)
			return;
		//printf("xorg = %d  yorg = %d  g_maxX = %d g_minX = %d  g_maxY = %d  g_minY = %d\r\n",xorg,yorg,g_maxX,g_minX,g_maxY,g_minY);
		xorg -= (g_maxX + g_minX) * scale / 2 / 1000;
		yorg += (g_maxY + g_minY) * scale / 2 / 1000;
		//printf("-xorg = %d  yorg = %d  scale = %d\r\n",xorg,yorg,scale);

		//if(vn>=17&&vn<=19){
		//	if((ox1!=xorg)||(oy1!=yorg)){
		//		xorg = ox1;
		//		yorg = oy1;
		//	}
		//}
		if (vn == 1 || vn == 6 || vn == 8 || vn == 9 || vn == 12)
		{
			DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_ORG, 0);
		}
		else
		{
			DrawBigPoint(xorg, yorg, COLOR_ORG);
		}

		// /[原点到次原点]*****************
		xst = g_patd.orgX;
		yst = g_patd.orgY;
		xend = xst;
		yend = yst;
		if ((xorg != (xorg + xst * scale / 1000)) || (yorg != (yorg - yst * scale / 1000)))
		{
			DrawLine(xorg, yorg, xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_LINE_H2H2);
		}
		// /[针段数据]*****************
		for (i = 0; i < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
			if (f)
			{
				if (vn == 2)
				{
					if ((((abs(xend - tx) * abs(xend - tx) + abs(yend - ty) * abs(yend - ty)) < (dxp * dxp + dyp * dyp))) && (tmpfeed >= g_min_feedsum) && (tmpfeed <= g_max_feedsum))
					{
						dxp = abs(xend - tx);
						dyp = abs(yend - ty);
						g_LPoint = tmpfeed;
					}
				}
				tmpfeed++;
				xend = xst;
				yend = yst;
				if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
				{
					if (vn == 5 || vn == 20)
					{
						if (tmpfeed > g_jog_point)
							ptmpfeed++;
					}
					//printf("tmpfeed = %d g_jog_point = %d ptmpfeed = %d g_parameter = %d \r\n",tmpfeed,g_jog_point,ptmpfeed,g_parameter);
					switch ((sewIcmd.cmd & 0xf000))
					{
					case 0x2000: //空送
						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;
						if ((vn == 5 || vn == 20) && tmpfeed > g_jog_point && ptmpfeed <= g_parameter)
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else if (vn == 26 && tmpfeed > g_jog_point && tmpfeed <= (g_jog_point + g_parameter))
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else if ((vn == 7 || vn == 11 || vn == 15 || vn == 10) &&
								 (tmpfeed > g_jog_stpoint && tmpfeed <= g_jog_enpoint))
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else if ((vn == 3 || vn == 4 || vn == 13 || vn == 14 || vn == 15 || vn == 16) &&
								 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
								  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
						{

							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_EMPTY);
						}
						xst = xend;
						yst = yend;
						break;

					case 0x6000: //缝纫
						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;
						if ((vn == 5 || vn == 20) && tmpfeed > g_jog_point && ptmpfeed <= g_parameter)
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else if (vn == 26 && tmpfeed > g_jog_point && tmpfeed <= (g_jog_point + g_parameter))
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else if ((vn == 7 || vn == 11 || vn == 15 || vn == 10) &&
								 (tmpfeed > g_jog_stpoint && tmpfeed <= g_jog_enpoint))
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else if ((vn == 3 || vn == 4 || vn == 13 || vn == 14 || vn == 15 || vn == 16) &&
								 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
								  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH); //修改 RED
						}
						else
						{
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, sewColor); //修改 sewColor
						}

						//							if(vn==1 || vn==2 || vn==3 || vn ==4 || vn ==5 || vn==6){	//绘制针点
						// DrawHLine(xorg + xst * scale / 1000 - 1, yorg - yst * scale / 1000, 3, Color_Black);
						// DrawVLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000 - 1, 3, Color_Black);

						// DrawHLine(xorg + xend * scale / 1000 - 1, yorg - yend * scale / 1000, 3, Color_Black);
						// DrawVLine(xorg + xend * scale / 1000, yorg - yend * scale / 1000 - 1, 3, Color_Black);

						// DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_SEW);
						// DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_SEW);
						//							}

						if ((vn == 5 || vn == 20) && tmpfeed > g_jog_point && ptmpfeed <= g_parameter)
						{
							DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_FINISH);
							DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_FINISH);
						}
						else if (vn == 26 && tmpfeed > g_jog_point && tmpfeed <= (g_jog_point + g_parameter))
						{
							DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_FINISH);
							DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_FINISH);
						}
						else if ((vn == 7 || vn == 11 || vn == 15 || vn == 10) &&
								 (tmpfeed > g_jog_stpoint && tmpfeed <= g_jog_enpoint))
						{
							DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_FINISH);
							DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_FINISH);
						}
						else if ((vn == 3 || vn == 4 || vn == 13 || vn == 14 || vn == 15 || vn == 16) &&
								 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
								  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
						{
							DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_FINISH);
							DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_FINISH);
						}
						else
						{
							DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_SEW);
							DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_SEW);
						}
						xst = xend;
						yst = yend;
						break;
					}
				}
				else if ((sewIcmd.cmd & 0xff00) == 0xfc00)
				{ //附加功能
					if (((sewIcmd.cmd & 0xff80) == 0xfc80) && (g_UdZhouFlag == 3))
					{ //换针
						if (sewColor == COLOR_LINE_SEW)
							sewColor = COLOR_LINE_CHANGE;
						else
							sewColor = COLOR_LINE_SEW;
					}
				}
				i += sewIcmd.off;
			}
			else
			{
				i++;
			}
		}

		if (vn == 23 || vn == 24)
		{
			for (k = 1; k < g_point_num; k++)
			{
				k1 = (S16)(g_array10[k][0]);
				k2 = (S16)(g_array10[k][1]);
				x2 = (S32)(xorg + k1 * scale / 1000);
				y2 = (S32)(yorg - k2 * scale / 1000);
				//DrawSerialNumber2(x2,y2,k,colorf,RGB(224,87,38));
				DrawSerialNumber2(x2, y2, k, 1, COLOR_CTRL);
			}
		}
		//printf("g_LPoint = %d \r\n",g_LPoint);
		//绘制指令代码
		if (vn == 0 || vn == 3 || vn == 4 || vn == 5 || vn == 7 ||
			vn == 13 || vn == 14 || vn == 11 || vn == 15 || vn == 10 ||
			vn == 26 || vn == 21 || vn == 22 || vn == 23 || vn == 24 || vn == 17 ||
			vn == 20 || vn == 25)
		{
			xst = g_patd.orgX;
			yst = g_patd.orgY;
			xend = xst;
			yend = yst;
			DrawFPBMPBTN2(xorg - 7, yorg - 7, 14, 14, BMP_H);
			if (g_patd.orgX != 0 || g_patd.orgY != 0)
			{
				DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_H2);
			}
			for (i = 0; i < (sewInfo.ptsize / 2);)
			{
				f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
				if (f)
				{
					xend = xst;
					yend = yst;
					//				#if Debug
					//				printf("borDrawCmd %04x %d %d %d \r\n",DrawPatCmd,xtmp,ytmp,DrawOffest);
					//				#endif
					if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
					{
						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;
						xst = xend;
						yst = yend;
					}
					else
					{
						//printf("sewIcmd.cmd  = %04x \r\n",sewIcmd.cmd);
						switch (sewIcmd.cmd >> 8)
						{
						case 0xf0:
							DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_P);
							break;
						case 0xf1:
							DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_T);
							break;
						case 0xfe: //上下暂停
							DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, (sewIcmd.cmd & 0xff) ? BMP_U : BMP_D);
							break;
						case 0xf4:
							DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_J);
							break;
						case 0xfd:
							DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_C);
							break;
						case 0xfC: //功能
							k1 = 0;
							for (k = 0; k < 8; k++)
							{
								if ((sewIcmd.cmd >> k) & 0x01)
								{
									k1++;
								}
							}
							if (k1 > 1)
							{
								DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_FX);
							}
							else
							{
								switch (sewIcmd.cmd & 0xff)
								{
								case 0x01:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F1);
									break;
								case 0x02:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F2);
									break;
								case 0x04:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F3);
									break;
								case 0x08:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F4);
									break;
								case 0x10:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F5);
									break;
								case 0x20:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F6);
									break;
								case 0x40:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F7);
									break;
								case 0x80:
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F8);
									break;
								}
							}
							break;
						}
					}
					i += sewIcmd.off;
				}
				else
					i++;
			}
		}
	}

	if (vn == 0 || vn == 26) //修改
	{
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_DeepPink);
		ViewRunPoint(x1, y1, COLOR_SELECT_P, 0); //修改 Color_DeepPink 1
		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		ReadPointData2(x1 - 4, y1 - 4);
		ViewRunPoint(x1, y1, COLOR_SELECT_E, 0); //修改 Color_Red 1
												 //DrawBigPoint(x1,y1,Color_Red);
	}
	else if (vn == 1)
	{
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 3, colorf, COLOR_CODE_C, 0); //Color_DeepPink

		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 3, colorf, COLOR_CROSS_BEF, 0); //Color_Red
	}
	else if (vn == 2)
	{
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		ReadPointData2(x1 - 2, y1 - 2);
		// DrawHLine(x1 - 2, y1, 5, Color_Red); //绘制针位点
		// DrawVLine(x1, y1 - 2, 5, Color_Red);
		DrawCross7x7(x1, y1, COLOR_CROSS);

		if (g_LPoint != g_jog_point)
		{
			getFeedXY(g_LPoint, &x, &y);
			x1 = (S32)(x * scale / 1000 + xorg);
			y1 = (S32)(-y * scale / 1000 + yorg);
			ReadPointData3(x1 - 2, y1 - 2);
			// for (i = 0; i < 5; i++)
			// {
			// 	DrawHLine(x1 - 2, y1 - 2 + i, 5, Color_DeepPink); //绘制针位点
			// }
			// DrawBigRectPoint(x1, y1, COLOR_PAT_PIN);
			DrawBigPoint(x1, y1, COLOR_SELECT_E);
		}
	}
	else if (vn == 3 || vn == 4 || vn == 13 || vn == 14)
	{
		if (vn == 13 || vn == 14)
		{
			getFeedXY(g_jog_stpoint, &x, &y);
			x1 = (S32)(x * scale / 1000 + xorg);
			y1 = (S32)(-y * scale / 1000 + yorg);
			// DrawBigPoint(x1, y1, Color_DeepPink);
			DrawBigPoint(x1, y1, COLOR_SELECT_E);
		}
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//printf("g_jog_point = %d x = %d  y = %d scale = %d  x1 = %d  y1 = %d xorg =%d yorg = %d \r\n" ,g_jog_point,x,y,scale,x1,y1,xorg,yorg);
		ReadPointData2(x1 - 3, y1 - 3);
		// DrawBigPoint(x1, y1, Color_Red); //绘制针位点
		DrawBigPoint(x1, y1, COLOR_SELECT_P); //绘制针位点
	}
	else if (vn == 5)
	{ //针删除
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_Red);
		// ViewRunPoint(x1, y1, Color_Red, 1);
		ReadPointData2(x1 - 3, y1 - 3);
		ViewRunPoint(x1, y1, COLOR_SELECT_P, 1);
		//		getFeedXY(g_jog_point+g_publicbuf[51],&x,&y);
		//		x1 = (S32)(x*scale/1000+xorg); y1 = (S32)(-y*scale/1000+yorg);
		//		ReadPointData2(x1-4,y1-4);
		//		//DrawBigPoint(x1,y1,Color_DeepPink);
		//		ViewRunPoint(x1,y1,Color_DeepPink,1);
	}
	else if (vn == 6)
	{
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		// DrawSerialNumber(x1, y1, 3, colorf, Color_Red, 0);
		DrawSerialNumber(x1, y1, 3, colorf, COLOR_CODE_C, 0);

		getFeedXY(g_jog_point + g_publicbuf[51], &x, &y);
		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		// DrawSerialNumber(x1, y1, 3, colorf, Color_DeepPink, 0);
		DrawSerialNumber(x1, y1, 3, colorf, COLOR_CROSS_BEF, 0);
	}
	else if (vn == 7)
	{
		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_DeepPink);
		// ViewRunPoint(x1, y1, Color_DeepPink, 1);
		ViewRunPoint(x1, y1, COLOR_SELECT_E, 1);

		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_Red);
		// ViewRunPoint(x1, y1, Color_Red, 1);
		ViewRunPoint(x1, y1, COLOR_SELECT_E, 1);

		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		ReadPointData2(x1 - 4, y1 - 4);
		//DrawBigPoint(x1,y1,Color_Red);
		// ViewRunPoint(x1, y1, Color_Red, 1);
		ViewRunPoint(x1, y1, COLOR_SELECT_P, 1);
	}
	else if (vn == 8)
	{
		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 1, colorf, COLOR_CROSS_BEF, 0);

		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 2, colorf, COLOR_CROSS_BEF, 0);

		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 3, colorf, COLOR_CODE_C, 0);
	}
	else if (vn == 9)
	{
		// if (HREG[8 == 2])
		if (F_FILETYPE == PAT_SEW) //修改
		{
			for (k = 1; k < g_point_num; k++)
			{
				k1 = (S16)(g_array10[k][0]);
				k2 = (S16)(g_array10[k][1]);
				x2 = (S32)(xorg + k1 * scale / 1000);
				y2 = (S32)(yorg - k2 * scale / 1000);
				DrawSerialNumber2(x2, y2, k, colorf, COLOR_CTRL);
			}
		}

		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 1, colorf, COLOR_CROSS_BEF, 0);

		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 2, colorf, COLOR_CROSS_BEF, 0);

		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 3, colorf, COLOR_CODE_C, 0);
	}
	else if (vn == 15 || vn == 16)
	{
		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawBigPoint(x1, y1, COLOR_SELECT_E);

		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawBigPoint(x1, y1, COLOR_SELECT_E);
		if (F_FILETYPE == PAT_SEW)
		{ //sew
			getFeedXY(g_jog_point, &x, &y);
			for (k = 1; k < g_point_num; k++)
			{
				k1 = (S16)(g_array10[k][0]);
				k2 = (S16)(g_array10[k][1]);
				x2 = (S32)(xorg + k1 * scale / 1000);
				y2 = (S32)(yorg - k2 * scale / 1000);
				DrawBigPoint(x2, y2, COLOR_SELECT_E);
			}
		}
		getFeedXY(g_jog_point, &x, &y);
		x2 = (S32)(x * scale / 1000 + xorg);
		y2 = (S32)(-y * scale / 1000 + yorg);
		ReadPointData2(x1 - 3, y1 - 3);
		DrawBigPoint(x1, y1, COLOR_SELECT_P); //绘制针位点
	}
	else if (vn == 10)
	{
		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_DeepPink);
		ViewRunPoint(x1, y1, COLOR_SELECT_E, 1);

		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawBigPoint(x1, y1, COLOR_SELECT_E);

		if (F_FILETYPE == PAT_SEW)
		{ //sew
			for (k = 1; k < g_point_num; k++)
			{
				k1 = (S16)(g_array10[k][0]);
				k2 = (S16)(g_array10[k][1]);
				x2 = (S32)(xorg + k1 * scale / 1000);
				y2 = (S32)(yorg - k2 * scale / 1000);
				DrawBigPoint(x2, y2, COLOR_SELECT_E);
				ViewRunPoint(x2, y2, COLOR_SELECT_E, 1);
			}
		}
		getFeedXY(g_jog_point, &x, &y);
		x2 = (S32)(xorg + (x + mut * g_dxpos) * scale / 1000);
		y2 = (S32)(yorg - (y + mut * g_dypos) * scale / 1000);
		ReadPointData2(x2 - 4, y2 - 4);
		//DrawBigPoint(x2,y2,Color_Red);
		ViewRunPoint(x2, y2, COLOR_SELECT_P, 1);
	}
	else if (vn == 11)
	{
		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_Red);
		ViewRunPoint(x1, y1, COLOR_SELECT_E, 1);

		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_DeepPink);
		ViewRunPoint(x1, y1, COLOR_SELECT_E, 1);
		if (F_FILETYPE == PAT_MIT)
		{ //mit
			k1 = 0;
			k2 = 0;
			for (k = 1; k < g_point_num; k++)
			{
				k1 += (S16)(g_array5[k][0]);
				k2 += (S16)(g_array5[k][1]);
				x2 = (S32)((x + k1) * scale / 1000 + xorg);
				y2 = (S32)(-(y + k2) * scale / 1000 + yorg);
				//DrawBigPoint(x2,y2,Color_Red);
				ViewRunPoint(x2, y2, COLOR_SELECT_P, 1);
			}
			x1 = (S32)((x + k1 + g_dxpos) * scale / 1000 + xorg);
			y1 = (S32)(-(y + k2 + g_dypos) * scale / 1000 + yorg);
		}
		else if (F_FILETYPE == PAT_SEW)
		{ //sew
			k1 = x;
			k2 = y;
			for (k = 1; k < g_point_num; k++)
			{
				k1 = (S16)(g_array10[k][0]);
				k2 = (S16)(g_array10[k][1]);
				x2 = (S32)(xorg + k1 * scale / 1000);
				y2 = (S32)(yorg - k2 * scale / 1000);
				//DrawBigPoint(x2,y2,Color_Red);
				ViewRunPoint(x2, y2, COLOR_SELECT_P, 1);
			}
			k1 += mut * g_dxpos;
			k2 += mut * g_dypos;
			x1 = (S32)(k1 * scale / 1000 + xorg);
			y1 = (S32)(-k2 * scale / 1000 + yorg);
		}
		ReadPointData2(x1 - 4, y1 - 4);
		//DrawBigPoint(x1,y1,Color_Red);
		ViewRunPoint(x1, y1, COLOR_SELECT_P, 1);
	}
	else if (vn == 12)
	{

		getFeedXY(g_jog_enpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 2, colorf, COLOR_CROSS_BEF, 0);
		// DrawSerialNumber(x1, y1, 2, colorf, Color_Red, 0);

		getFeedXY(g_jog_stpoint, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		DrawSerialNumber(x1, y1, 1, colorf, COLOR_CROSS_BEF, 0);
		// DrawSerialNumber(x1, y1, 1, colorf, Color_Red, 0);
		if (F_FILETYPE == PAT_MIT)
		{ //mit
			k1 = 0;
			k2 = 0;
			for (k = 1; k < g_point_num; k++)
			{
				k1 += (S16)(g_array5[k][0]);
				k2 += (S16)(g_array5[k][1]);
				x2 = (S32)((x + k1) * scale / 1000 + xorg);
				y2 = (S32)(-(y + k2) * scale / 1000 + yorg);
				DrawSerialNumber2(x2, y2, k, colorf, COLOR_CROSS_BEF);
				// DrawSerialNumber2(x2, y2, k, colorf, RGB(224, 87, 38));
			}
			x1 = (S32)((x + k1 + g_dxpos) * scale / 1000 + xorg);
			y1 = (S32)(-(y + k2 + g_dypos) * scale / 1000 + yorg);
			DrawSerialNumber(x1, y1, 3, colorf, COLOR_CODE_C, 0);
			// DrawSerialNumber(x1, y1, 3, colorf, RGB(224, 87, 38), 0);
		}
		else if (F_FILETYPE == PAT_SEW)
		{ //sew
			k1 = x;
			k2 = y;
			for (k = 1; k < g_point_num; k++)
			{
				k1 = (S16)(g_array10[k][0]);
				k2 = (S16)(g_array10[k][1]);
				x2 = (S32)(xorg + k1 * scale / 1000);
				y2 = (S32)(yorg - k2 * scale / 1000);
				DrawSerialNumber2(x2, y2, k, colorf, COLOR_CROSS_BEF);
				// DrawSerialNumber2(x2, y2, k, colorf, RGB(224, 87, 38));
			}

			k1 += 2 * g_dxpos;
			k2 += 2 * g_dypos;
			x2 = (S32)(xorg + k1 * scale / 1000);
			y2 = (S32)(yorg - k2 * scale / 1000);
			DrawSerialNumber(x2, y2, 3, colorf, COLOR_CODE_C, 0);
			// DrawSerialNumber(x2, y2, 3, colorf, RGB(224, 87, 38), 0);
		}
	}
	else if (vn == 17) //创建花样-多点输入-移动的光标超过显示范围
	{
		//getFeedXY1(&x,&y);
		getFeedXY(g_feedc, &x, &y);
		//printf("g_feedc = %d  x = %d y = %d \r\n ",g_feedc,x,y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		//DrawBigPoint(x1,y1,Color_DeepPink);
		DrawCross7x7(x1, y1, COLOR_CROSS);

		if (F_FILETYPE == PAT_MIT)
		{ //mit
			for (i = 1; i < g_point_num; i++)
			{
				x += (S16)g_array4[i][0];
				y += (S16)g_array4[i][1];
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				//DrawBigPoint(x1,y1,Color_Red);
				DrawCross7x7(x1, y1, COLOR_CROSS_BEF);
			}
		}
		else if (F_FILETYPE == PAT_SEW)
		{ //sew
			for (i = 1; i < g_point_num; i++)
			{
				x += mut * (S16)g_array5[i][0];
				y += mut * (S16)g_array5[i][1];
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				//DrawBigPoint(x1,y1,Color_Red);
				DrawCross7x7(x1, y1, COLOR_CROSS_BEF);
			}
		}
		x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
		y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
		ReadPointData2(x1 - 4, y1 - 4);
		//DrawBigPoint(x1,y1,Color_Red);	//绘制针位点
		DrawCross7x7(x1, y1, COLOR_CROSS_BEF);
	}
	else if (vn == 18) //创建花样-花样插入
	{
		getFeedXY(g_feedc, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		ReadPointData2(x1 - 4, y1 - 4);
		//DrawBigPoint(x1,y1,Color_Red);	//绘制针位点
		DrawCross7x7(x1, y1, COLOR_CROSS);
	}
	else if (vn == 19) //创建花样-单点输入
	{
		getFeedXY1(&x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		ReadPointData2(x1 - 4, y1 - 4);
		//DrawBigPoint(x1,y1,Color_Red);	//绘制针位点
		// ViewRunPoint(x1, y1, Color_Red, 2);
		DrawCross7x7(x1, y1, COLOR_CROSS);
	}
	else if (vn == 20 || vn == 25) //修改界面-速度修正、删除代码、添加代码
	{
		getFeedXY(g_jog_point, &x, &y);
		x1 = (S32)(x * scale / 1000 + xorg);
		y1 = (S32)(-y * scale / 1000 + yorg);
		ReadPointData2(x1 - 3, y1 - 3);
		// DrawBigPoint(x1, y1, Color_Red); //绘制针位点
		DrawBigPoint(x1, y1, COLOR_SELECT_P); //绘制针位点
	}

	SetDrawXY(0, 0, LCD_width, LCD_height, 0);
}

//-- 清除定位点
void clrLPoint(void)
{
	RecoveData3();
}
//-------获取指定针的xy坐标
void getFeedXY(U16 feed, S16 *outx, S16 *outy)
{
	U32 i;
	S16 x, y;
	U32 pat_add_p;
	U16 dat, dat1; //,dat2
	S16 x1 = 0, y1 = 0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	U8 f;
	U16 tmpfeed = 0;

	*outx = 0;
	*outy = 0;
	//printf("getFeedXY F_FILETYPE = %d \r\n",F_FILETYPE);
	if (F_FILETYPE == PAT_MIT)
	{ //mit
		x = 0;
		y = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			x = (S16)(SREG[6]);
			y = (S16)(SREG[7]);
		}
#endif

		*outx = x;
		*outy = y;
		pat_add_p = g_patd.pat_add;
		for (i = 0; i < feed /*g_patd.feed_sum*/; i++)
		{
			dat1 = SREG[pat_add_p];
			dat = SREG[pat_add_p + 1];
			if ((dat1 & 0xff) == 0x1 || (dat1 & 0xff) == 0x21 || (dat1 & 0xff) == 0x41 || (dat1 & 0xff) == 0x61 || (dat1 & 0xff) == 0x3 || (dat1 & 0xff) == 0x1b)
			{
				x1 = (U8)(dat);
				y1 = (U8)((U16)(dat) >> 8);
			}
			else
			{
				x1 = 0;
				y1 = 0;
			}

			//if(i==feed || (feed==g_patd.feed_sum && i==feed-1)){
			//	*outx = x;
			//	*outy = y;
			//	break;
			//}
			if (x1 >= 0x80)
			{
				x1 -= 0x80;
				x -= x1;
			}
			else
				x += x1;
			if (y1 >= 0x80)
			{
				y1 -= 0x80;
				y -= y1;
			}
			else
				y += y1;
			pat_add_p += 2;

			*outx = x;
			*outy = y;
		}
	}
	else if (F_FILETYPE == PAT_SEW)
	{
		Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);
		*outx = g_patd.orgX;
		*outy = g_patd.orgY;
		x = g_patd.orgX;
		y = g_patd.orgY;
		//针数据段
		for (i = 0; i < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
			if (f)
			{
				if (tmpfeed == feed)
				{
					*outx = x;
					*outy = y;
					break;
				}
				x += sewIcmd.dx;
				y += sewIcmd.dy;
				tmpfeed++;
				//if(tmpfeed==feed && feed==g_patd.feed_sum){
				*outx = x;
				*outy = y;
				//	break;
				//}
				i += sewIcmd.off;
			}
			else
			{
				i++;
			}
		}

		if (g_BorSTflag == 0 && feed == 0)
		{
			*outx = 0;
			*outy = 0;
		}
	}

	//printf("getFeedXY(%d,%d,%d) \r\n",feed,*outx,*outy);
}

//打板获取终点坐标
void getFeedXY1(S16 *outx, S16 *outy)
{
	U32 i;
	S16 x, y;
	U32 pat_add_p;
	U16 dat, dat1; //,dat2
	S16 x1 = 0, y1 = 0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	U8 f;
	U16 tmpfeed = 0;

	*outx = 0;
	*outy = 0;
	if (F_FILETYPE == PAT_MIT)
	{ //mit
		x = 0;
		y = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			x = (S16)(SREG[6]);
			y = (S16)(SREG[7]);
		}
#endif
		*outx = x;
		*outy = y;
		pat_add_p = g_patd.pat_add;
		for (i = 0; i < g_patd.feed_sum; i++)
		{
			dat1 = SREG[pat_add_p];
			dat = SREG[pat_add_p + 1];
			if ((dat1 & 0xff) == 0x1 || (dat1 & 0xff) == 0x21 || (dat1 & 0xff) == 0x41 || (dat1 & 0xff) == 0x61 || (dat1 & 0xff) == 0x3 || (dat1 & 0xff) == 0x1b)
			{
				x1 = (U8)(dat);
				y1 = (U8)((U16)(dat) >> 8);
			}
			else
			{
				x1 = 0;
				y1 = 0;
			}
			if (x1 >= 0x80)
			{
				x1 -= 0x80;
				x -= x1;
			}
			else
				x += x1;
			if (y1 >= 0x80)
			{
				y1 -= 0x80;
				y -= y1;
			}
			else
				y += y1;
			*outx = x;
			*outy = y;
			pat_add_p += 2;
		}
	}
	else if (F_FILETYPE == PAT_SEW)
	{ //mit
		Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);
		*outx = g_patd.orgX;
		*outy = g_patd.orgY;
		x = g_patd.orgX;
		y = g_patd.orgY;
		//针数据段
		for (i = 0; i < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
			if (f)
			{
				x += sewIcmd.dx;
				y += sewIcmd.dy;
				*outx = x;
				*outy = y;
				tmpfeed++;
				i += sewIcmd.off;
			}
			else
			{
				i++;
			}
		}
	}

	//printf("getFeedXY(%d,%d,%d) \r\n",feed,*outx,*outy);
}

U8 borGetDiscode(U16 Cmd)
{
	U8 coden = 0;
	//	printf("Cmd = %04x \r\n",Cmd);
	if ((Cmd & 0xf000) == 0x2000)
	{ //空送
		coden = 0;
	}
	else if ((Cmd & 0xf000) == 0x6000)
	{ //缝纫
		coden = 1;
	}
	else
	{
		switch (Cmd & 0xff00)
		{
		case 0xf000: //中压脚调整指令
			coden = 26;
			break;

		case 0xf100: //一级松线度指令
			coden = 27;
			break;

		case 0xf300: //暂停
			if ((Cmd & 0xfff0) == 0xf340)
			{ //暂停开
				coden = 28;
			}
			else if ((Cmd & 0xfff0) == 0xf300)
			{ //暂停关
				coden = 29;
			}
			break;

		case 0xfb00: //减速指令
					 //				if(Cmd == 0xfb00)  coden = 21;
					 //				else if(Cmd == 0xfb01)  coden = 22;
					 //				else if(Cmd == 0xfb02)  coden = 23;
					 //				else if(Cmd == 0xfb03)  coden = 24;
			coden = (Cmd & 0xff) + 21;
			break;

		case 0xfc00: //备选代码1指令
			if (Cmd == 0xfC01)
			{
				coden = 3;
			} //功能一
			else if (Cmd == 0xfC02)
			{
				coden = 4;
			} //功能二
			else if (Cmd == 0xfC04)
			{
				coden = 5;
			} //功能三
			else if (Cmd == 0xfC08)
			{
				coden = 6;
			} //功能四
			else if (Cmd == 0xfC10)
			{
				coden = 7;
			} //功能五
			else if (Cmd == 0xfC20)
			{
				coden = 8;
			} //功能六
			else if (Cmd == 0xfC40)
			{
				coden = 9;
			} //功能七
			else if (Cmd == 0xfC80)
			{
				if (g_UdZhouFlag == 3)
					coden = 32;
				else
					coden = 10;
			} //换针 功能八
			else
			{
				coden = 31;
			} //功能X
			break;

		case 0xfd00: //剪线指令
			coden = 13;
			break;

		case 0xf400: //跳缝指令
			coden = 14;
			break;

		case 0xfe00: //上停针指令
			if (Cmd == 0xfe00)
				coden = 11;
			else if (Cmd == 0xfe01)
				coden = 12;
			break;

		case 0xec00: //针跟随
			coden = 33;
			break;

		case 0xea00: //参数一
			coden = 34;
			break;

		case 0xeb00: //参数二
			coden = 35;
			break;

		case 0xee00: //缝纫速度
			coden = 36;
			break;

		case 0xff00: //针数据结束指令
			coden = 2;
			break;
		default:
			coden = 2;
			break;
		}
	}

	return coden;
}

//读内存文件索引
//返回值：0失败 1：成功
int ReadFileIndex(U16 cmd, U16 pold)
{
	U8 f;
	U32 i;
	U16 ReaFiles = 0;

#ifdef SYSTEM0
	if (Write_Cmd(cmd, (cmd == C_RDWORKSHOPLIST) ? g_netSelectMac : 0, 0) == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	if (Read_Status() == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		pno = MENU_RWHELP;
		hleppara = 39;
		poldno[1] = pold;
		pnoindex = 2; //return 0;
	}
	for (i = 0; i < MAXIREG2; i++)
	{
		IREG2[i] = 0xffff;
	}

	if (Write_Cmd(C_READMEMNUMS, 0, 0) == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	if (Read_Status() == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		pno = MENU_RWHELP;
		hleppara = 39;
		poldno[1] = pold;
		pnoindex = 2; //return 0;
	}
	ReaFiles = DREG[D_CMDSTATUS] - 100;
	Read_D(D_Address[PAGEBUF], ReaFiles, (U16 *)(&IAPBuff[0]));

	for (i = 0; i < ReaFiles; i++)
	{
		f = Read_D(D_Address[INDEX2] + (IAPBuff[i] - MINFILENO) * FileIndexSize, FileIndexSize, (U16 *)(&IREG2[(IAPBuff[i] - MINFILENO) * FileIndexSize]));
		if (f == 0)
			return 0;
	}

	return 1;

#endif

	//#ifdef SYSTEM1
	if (Write_Cmd(cmd, (cmd == C_RDWORKSHOPLIST) ? g_netSelectMac : 0, 0) == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	if (Read_Status() == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		pno = MENU_RWHELP;
		hleppara = 39;
		poldno[1] = pold;
		pnoindex = 2; //return 0;
	}
	for (i = 0; i < MAXIREG2; i++)
	{
		IREG2[i] = 0xffff;
	}

	if (Write_Cmd(C_READMEMNUMS, 0, 0) == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	if (Read_Status() == 0)
	{
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[1] = pold;
		pnoindex = 2;
		return 0;
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		pno = MENU_RWHELP;
		hleppara = 39;
		poldno[1] = pold;
		pnoindex = 2; //return 0;
	}
	ReaFiles = DREG[D_CMDSTATUS] - 100;
	Read_D(D_Address[PAGEBUF], ReaFiles, (U16 *)(&IAPBuff[0]));

	for (i = 0; i < ReaFiles; i++)
	{
		f = Read_D(D_Address[INDEX2] + (IAPBuff[i] - MINFILENO) * FileIndexSize, FileIndexSize, (U16 *)(&IREG2[(IAPBuff[i] - MINFILENO) * FileIndexSize]));
		if (f == 0)
			return 0;
	}

	return 1;

	//#endif

#ifdef SYSTEM2
	if (Write_Cmd(cmd, (cmd == C_RDWORKSHOPLIST) ? g_netSelectMac : 0, 0) == 0)
	{
		return 0;
	}
	if (Read_Status() == 0)
	{
		return 0;
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		return 0;
	}
	for (i = 0; i < MAXIREG2; i++)
	{
		IREG2[i] = 0xffff;
	}

	if (Write_Cmd(C_READMEMNUMS, 0, 0) == 0)
	{
		return 0;
	}
	if (Read_Status() == 0)
	{
		return 0;
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		return 0;
	}
	ReaFiles = DREG[D_CMDSTATUS] - 100;
	Read_D(D_Address[PAGEBUF], ReaFiles, (U16 *)(&IAPBuff[0]));

	for (i = 0; i < ReaFiles; i++)
	{
		f = Read_D(D_Address[INDEX2] + (IAPBuff[i] - MINFILENO) * FileIndexSize, FileIndexSize, (U16 *)(&IREG2[(IAPBuff[i] - MINFILENO) * FileIndexSize]));
		if (f == 0)
			return 0;
	}

	return 1;

#endif
}

//------------------------------------------
// Function   : U8 ReadMemorryList(U32 pno20)
// Description: 如选中读优盘,在退出返回时要读回内存索引表,读成功返回1，否则为0
//------------------------------------------
U8 ReadMemorryList(U32 pno20)
{
	U8 f;

	if (g_disk_memory)
	{ //如选中读优盘,在退出返回时要读回内存索引表
		/*	if(Write_Cmd(C_RDMEMPATLIST,0,0)==0){
			pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex] = pno20;pnoindex ++;return 0;
		}
		if(Read_Status()==0){
			pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex] = pno20;pnoindex ++;return 0;
		}else if(DREG[D_CMDSTATUS]<100){
			pno = MENU_RWHELP;hleppara = 39;poldno[pnoindex] = pno20;pnoindex ++;return 0;
		}
		f = Read_D(D_Address[INDEX2],D_Length[INDEX2],(U16 *)(&IREG2[0]));*/
		f = ReadFileIndex(C_RDMEMPATLIST, pno20);
		if (f == 0)
		{
#ifdef SYSTEM0
			pno = MENU_RWHELP;
			hleppara = 15;
			poldno[pnoindex] = pno20;
			pnoindex++;
			return 0;
#endif

#ifdef SYSTEM1
			pno = MENU_RWHELP;
			hleppara = 15;
			poldno[pnoindex] = pno20;
			pnoindex++;
			return 0;
#endif

#ifdef SYSTEM2
			pno = MENU_BOR_HELP;
			hleppara = 15;
			poldno[pnoindex++] = pno20;
			return 0;
#endif
		}
		else
		{
			g_disk_memory = 0;
			BarcodeCompare();
		}
	}
	return 1;
}

// U32 Patlen:花样数据总长 针对兄弟款   2016-12-16
U8 CheckSREG(U8 wrmode, U8 patType, U32 Patlen)
{
	U16 SREG_sub = 0;
	U32 nval;

	//	#if Debug
	//	printf("patType = %d  lenth = %d   \r\n",patType,Patlen);
	//	#endif
	//nval = ((SREG[2]+0x40)/2)+(SREG[4]/4)*3;	//SREG花样个数
	if (patType == 0)
	{																				  //三菱
		nval = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3; //2013.3.7

		if (nval > MAXSREG)
			return FALSE;

		SREG_sub = get_checkSum((U16 *)SREG, nval, PAT_MIT);
		//for(i=0;i<nval;i++){
		//	if(i!=0x20) SREG_sub+= SREG[i];wdt();
		//}
		if (wrmode == 1)
		{ //读磁盘花样校验
			if (SREG[0x20] == 0)
			{
				SREG[0x20] = SREG_sub;
				return TRUE;
			}
			else
			{
				if (SREG[0x20] == SREG_sub)
					return TRUE;
			}
		}
		else if (wrmode == 2)
		{ //读主机花样校验
			if (SREG[0x20] != SREG_sub)
				return FALSE;
			else
				return TRUE;
		}
	}
	else if (patType == 1)
	{ //兄弟
		nval = Patlen / 2;
		SREG_sub = get_checkSum((U16 *)SREG, nval, PAT_SEW);
		//for(i=0;i<nval;i++){
		//	if(i!=0x05) SREG_sub+= SREG[i];wdt();
		//}
		if (wrmode == 1)
		{ //读磁盘花样校验
			if (SREG[0x05] == 0)
			{
				SREG[0x05] = SREG_sub;
				return TRUE;
			}
			else
			{
				if (SREG[0x05] == SREG_sub)
					return TRUE;
			}
		}
		else if (wrmode == 2)
		{ //读主机花样校验
			if (SREG[0x05] != SREG_sub)
				return FALSE;
			else
				return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------
// Function   : U8 MemoryDrv_Opera(U16 dispcmd,U16 PARA1)
// Description: SD卡\USB设备的相关操作
//------------------------------------------
U8 MemoryDrv_Opera(U16 dispcmd, U16 PARA1)
{
	U8 OPRA_STATUS = C_NOFLASH;
	//	U8 result=0;
	//	U8 s1, s2;
	//	U16 i,j,val;
	//	U32 lenth;
	//	U16 file_meg[10];
	//	U16 count;
	//	U32 ii;
	//	U8 ReSREGcount;	 //SREG校验错误时重复次数
	//
	//	char fileN[8];
	//	U8 k;
	//
	////	SPI_SetSpeed(SPI_SPEED_HIGH);
	//	switch(dispcmd){
	//		case C_RDFLAPATLIST:	// 读优盘中的花样文件索引表
	//			//打板器切换磁盘方式
	//			if((PARA1==USB)&&(PARA1!=Memorymode)){	//USB
	//				if((Memory_flag&USB_FLAG)==USB_FLAG){
	//					Memorymode=NOMEMORY;
	//					USB_error=0;
	//					Memory_flag&=~USB_FLAG;
	//					Memory_Checkin();
	//				}else{
	//					OPRA_STATUS =C_NOFLASH;// 没有优盘
	//					break;
	//				}
	//			}else if((PARA1==SD)&&(PARA1!=Memorymode)){	//SD
	//				if((Memory_flag&SD_FLAG)==SD_FLAG){
	//					Memorymode=NOMEMORY;
	//					SD_error=0;
	//					Memory_flag&=~SD_FLAG;
	//					Memory_Checkin();
	//				}else{
	//					OPRA_STATUS =C_NOFLASH;// 没有优盘
	//					break;
	//				}
	//			}
	//
	//			if(dir_cnt==0){
	//				for(i=0;i<MAXDIR;i++)
	//					DIR_DREG[i]=0xffff;
	//			}
	//			//读优盘或sd的索引表
	//			for(i = 0;i < MAXIREG;i++)
	//				IREG[i] = DIR_DREG[i];
	//			OPRA_STATUS = C_SUCCEED;	// 读成功	100
	//			break;
	//		case C_READFLASHPAT:	// 读优盘中花样文件
	//			if((PARA1>=600)&&(PARA1<=920)){
	//				i=(PARA1 - 600)*10;
	//				if((PARA1==DIR_DREG[i])&&(DIR_DREG[i]>=600)&&(DIR_DREG[i]<=920)){
	//					ReSREGcount=0;
	//					while(ReSREGcount<READMEMORYCOUNT){
	//						ReadFileRaw(PARA1);
	//						SREG[0]= PARA1;
	//						for(k=0;k<4;k++){
	//							fileN[2*k] = (char)(DIR_DREG[i+1+k] & 0x00ff);
	//							if(fileN[2*k] > 96)
	//								fileN[2*k] -= 32;		//转为大写
	//
	//							fileN[2*k+1] = (char)(DIR_DREG[i+1+k] >> 8);
	//							if(fileN[2*k+1] > 96)
	//								fileN[2*k+1] -= 32;		//转为大写
	//
	//							DIR_DREG[i+1+k]=(U16)(fileN[2*k+1]<<8)|fileN[2*k];
	//						}
	//							for(k =0;k<4;k++)
	//								SREG[0x10+k] = DIR_DREG[i+1+k];
	//
	//							SREG[0x20] = 0;
	//
	//						if(CheckSREG(1)==TRUE){
	//							break;
	//						}
	//						ReSREGcount+=1;
	//						wdt();
	//					}
	//					if(ReSREGcount>=READMEMORYCOUNT){
	//						OPRA_STATUS = C_EORFILE;	 // 文件数据已坏
	//						break;
	//					}
	//					OPRA_STATUS = C_SUCCEED;
	//					break;
	//				}
	//			}
	//
	//			OPRA_STATUS = C_NOFILE;	 //无此花样
	//			break;
	//		case C_DELFLASHPAT:		// 删除优盘中花样文件
	//			if((PARA1>=600)&&(PARA1<=920)){
	//				i=(PARA1 - 600)*10;
	//				if((PARA1==DIR_DREG[i])&&(DIR_DREG[i]>=600)&&(DIR_DREG[i]<=920)){
	//					result=deletefile(PARA1);
	//					if(result==F_OK){
	//						OPRA_STATUS = C_SUCCEED;	 // 读成功	100
	//					}
	//					else OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//					SearchavailabDir();	//重读DIR
	//					break;
	//				}
	//			}
	//			OPRA_STATUS = C_NOFILE;	 //无此花样
	//			break;
	//		case C_WRITEFLASHPAT:	// 写优盘中花样文件
	//			if((PARA1>=600)&&(PARA1<=920)){
	//				ReSREGcount=0;
	//				ReadCurTime();//读当前主机时间
	//				file_meg[0]=SREG[0];
	//				file_meg[1]=SREG[16];
	//				file_meg[2]=SREG[17];
	//				file_meg[3]=SREG[18];
	//				file_meg[4]=SREG[19];
	//				file_meg[5]=0xffff;
	//				file_meg[6]=0xffff;
	//				file_meg[7]=0xffff;
	//				ii=((SREG[2]+0x40)/2)+((((U32)(SREG[5])<<16)+SREG[4])/4)*3;
	//				file_meg[8]=(U16)(ii);
	//				file_meg[9]=(U16)(ii>>16);
	//				//////////////////////////////////////////////
	//				SREG[0]=0;	 //清除文件号
	//				SREG[0x20]=0;		//清校验位
	//				fcopydat(file_meg,(U16 *)(&SREG[0]),ii);
	//				SearchavailabDir();	//重读DIR
	//				SREG[0]=file_meg[0]; //恢复文件号
	//				OPRA_STATUS = C_SUCCEED;	 // 读成功	100
	//			}else OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//
	//			break;
	//
	//		case C_COPYMEMORYPAT:	// 复制内存中花样文件到优盘
	//			DREG[D_COMMAND] = C_RDMEMPATLIST;
	//			DREG[D_CMDSTATUS] = 0;
	//			result = Write_D(D_Address[DISP]+D_COMMAND,2,(U16 *)(&DREG[D_COMMAND]));
	//			for(i=0;i<(RDTIMES);i++){
	//				result = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
	//				if((DREG[D_CMDSTATUS]))break;
	//				DelayMs(WAITTIME);
	//			}
	//			Read_D(D_Address[INDEX],D_Length[INDEX],(U16 *)(&IREG[0]));
	//
	//			count=0;
	//
	//			if((PARA1>0)&&(PARA1<100)){	   //复制的花样数目
	//				ReadCurTime();//读当前主机时间
	//				for(val=0;val<PARA1;val++){
	//					wdt();
	//					if((SelectNoFino[val]>=600)&&(SelectNoFino[val]<=920)){
	//						i=(SelectNoFino[val] - 600)*10;
	//						if((IREG[i]>=600)&&(IREG[i]<=920)){
	//							for(j=0;j<10;j++)
	//								file_meg[j]=IREG[i+j];
	//
	//							DREG[D_COMMAND] = C_READMEMORYPAT;
	//							DREG[D_CMDSTATUS] = 0;
	//							DREG[D_CMDPARA1] = file_meg[0];
	//							result = Write_D(D_Address[DISP]+D_COMMAND,3,(U16 *)(&DREG[D_COMMAND]));
	//							if(result){
	//								for(i=0;i<(RDTIMES+RDTIMES);i++){
	//									result = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
	//									if((DREG[D_CMDSTATUS]))break;
	//									DelayMs(WAITTIME);
	//								}
	//							}
	//							if((DREG[D_CMDSTATUS]!=100)){
	//								OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//								SearchavailabDir();	//重读DIR
	//								break;
	//							}
	//
	//							Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&F_IND_NO));				// 读当前花样文件信息
	//							if(F_IND_NO==file_meg[0]){
	//								ReSREGcount=0;
	//								while(ReSREGcount<READFLASHCOUNT){
	//									result = Read_D(D_Address[PTDATA],(((U32)(file_meg[9])<<16)+(U32)(file_meg[8]))/2,(U16 *)(&SREG[0]));
	//									if(CheckSREG(2)==TRUE)
	//									{
	//										break;
	//									}
	//									ReSREGcount+=1;
	//									wdt();
	//								}
	//								if(ReSREGcount>=READFLASHCOUNT){
	//									if(result) continue;
	//								}
	//
	//								if(result){
	//									SREG[0]=0;	 //清除文件号
	//									SREG[0x20]=0;		//清校验位
	//									fcopydat(file_meg,(U16 *)(&SREG[0]),(((U32)(file_meg[9])<<16)+(U32)(file_meg[8]))/2);
	//									SREG[0]=file_meg[0]; //恢复文件号
	//									DelayMs(1);
	//								}else{
	//									OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//									SearchavailabDir();	//重读DIR
	//									break;
	//								}
	//							}else{
	//								OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//								SearchavailabDir();	//重读DIR
	//								break;
	//							}
	//						}
	//					}
	//				}
	//				ReMemorymode=100;//无意义（只做有效磁盘类型显示）
	//				if(val>=PARA1){
	//					OPRA_STATUS = C_SUCCEED;			 // 读成功	100
	//					SearchavailabDir();	//重读DIR
	//				}
	//			}else if(PARA1==100){  //全部复制
	//				ReadCurTime();//读当前主机时间
	//
	//				for(val=0;val<MAXIREG;){
	//					if((IREG[val]>=600)&&(IREG[val]<=920)) count++;
	//					val+=10;
	//				}
	//
	//
	//				count=0;
	//				for(val=0;val<MAXIREG;){
	//					wdt();
	//					if((IREG[val]>=600)&&(IREG[val]<=920)){
	//						for(j=0;j<10;j++)
	//							file_meg[j]=IREG[val+j];
	//
	//							DREG[D_COMMAND] = C_READMEMORYPAT;
	//							DREG[D_CMDSTATUS] = 0;
	//							DREG[D_CMDPARA1] = file_meg[0];
	//							result = Write_D(D_Address[DISP]+D_COMMAND,3,(U16 *)(&DREG[D_COMMAND]));
	//							if(result){
	//								for(i=0;i<(RDTIMES+RDTIMES);i++){
	//									result = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
	//									if((DREG[D_CMDSTATUS]))break;
	//									DelayMs(WAITTIME);
	//								}
	//							}
	//							if((DREG[D_CMDSTATUS]!=100)){
	//								OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//								SearchavailabDir();	//重读DIR
	//								break;
	//							}
	//
	//							Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&F_IND_NO));				// 读当前花样文件信息
	//							if(F_IND_NO==file_meg[0]){
	//								ReSREGcount=0;
	//								while(ReSREGcount<READFLASHCOUNT){
	//									result = Read_D(D_Address[PTDATA],(((U32)(file_meg[9])<<16)+(U32)(file_meg[8]))/2,(U16 *)(&SREG[0]));//2013.3.7
	//									if(CheckSREG(2)==TRUE) break;
	//									ReSREGcount+=1;
	//									wdt();
	//								}
	//								if(ReSREGcount>=READFLASHCOUNT){
	//									if(result){val+=10; continue;}
	//								}
	//
	//								if(result){
	//									SREG[0]=0;	 //清除文件号
	//									SREG[0x20]=0;		//清校验位
	//									fcopydat(file_meg,(U16 *)(&SREG[0]),(((U32)(file_meg[9])<<16)+(U32)(file_meg[8]))/2);//2013.3.7
	//									SREG[0]=IREG[i]; //恢复文件号
	//								}else{
	//									OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//									SearchavailabDir();	//重读DIR
	//
	//									break;
	//								}
	//							}else{
	//								OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//								SearchavailabDir();	//重读DIR
	//								break;
	//							}
	//						}
	//						val+=10;
	//					}
	//					if(val>=MAXIREG){
	//						OPRA_STATUS = C_SUCCEED;			 // 读成功	100
	//						SearchavailabDir();	//重读DIR
	//					}
	//				}
	//				ReMemorymode=100;//无意义（只做有效磁盘类型显示）
	//
	//				break;
	//			case C_COPYFLASHPAT:	// 复制优盘中花样文件到内存
	//				count=0;
	//				if((PARA1>0)&&(PARA1<100)){
	//					for(val=0;val<PARA1;val++){
	//						wdt();
	//						if((SelectNoFino[val]>600)&&(SelectNoFino[val]<=920)){
	//							i=(SelectNoFino[val] - 600)*10;
	//							if((DIR_DREG[i]>600)&&(DIR_DREG[i]<=920)){	//600无效
	//								ReSREGcount=0;
	//								while(ReSREGcount<READMEMORYCOUNT){
	//									ReadFileRaw(DIR_DREG[i]);
	//									SREG[0]= DIR_DREG[i];
	//									for(k=0;k<4;k++){
	//										fileN[2*k] = (char)(DIR_DREG[i+1+k] & 0x00ff);
	//										if(fileN[2*k] > 96)
	//											fileN[2*k] -= 32;		//转为大写
	//
	//										fileN[2*k+1] = (char)(DIR_DREG[i+1+k] >> 8);
	//										if(fileN[2*k+1] > 96)
	//											fileN[2*k+1] -= 32;		//转为大写
	//
	//										DIR_DREG[i+1+k]=(U16)(fileN[2*k+1]<<8)|fileN[2*k];
	//
	//									}
	//
	//
	//									for(k =0;k<4;k++)
	//										SREG[0x10+k] = DIR_DREG[i+1+k];
	//
	//									SREG[0x20] = 0;
	//
	//									if(CheckSREG(1)==TRUE) break;
	//									ReSREGcount+=1;
	//									wdt();
	//								}
	//								if(ReSREGcount>=READMEMORYCOUNT){
	//									if(result) continue;
	//								}
	//								lenth = ((SREG[2]+0x40)/2)+((((U32)(SREG[5])<<16)+SREG[4])/4)*3;
	//
	//							  	ReSREGcount=0;
	//								while(ReSREGcount<READFLASHCOUNT){
	//									result=Write_D(D_Address[PTDATA],lenth,(U16 *)(&SREG[0])); // 发送当前花样文件
	//									if(result==0){ //通讯错误
	//										OPRA_DISPCMDSTATUS = C_ERROR;			 // 操作失败	2
	//										ReSREGcount=12;//错误跳出
	//										break;
	//									}
	//									DREG[D_COMMAND] = C_WRITEMEMORYPAT;						// 保存花样文件
	//									DREG[D_CMDSTATUS] = 0;
	//									DREG[D_CMDPARA1]  = SREG[0];
	//									result = Write_D(D_Address[DISP]+D_COMMAND,3,(U16 *)(&DREG[D_COMMAND]));			// 发送指令
	//									if(result){
	//										for(i=0;i<(RDTIMES+RDTIMES);i++){
	//											result = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
	//											if((DREG[D_CMDSTATUS])||(result == 0))break;
	//											DelayMs(WAITTIME);
	//										}
	//									}
	//									if((DREG[D_CMDSTATUS]!=100)){
	//										if(DREG[D_CMDSTATUS]==12){	 //主机返回错误重新发数据
	//											ReSREGcount+=1;
	//											continue;
	//										}
	//										OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//										ReSREGcount=12;//错误跳出
	//										break;
	//									}else break;
	//								}
	//								if(ReSREGcount==12) break;//错误跳出
	//							}
	//						}
	//					}
	//					ReMemorymode=100;//无意义（只做有效磁盘类型显示）
	//					if(val>=PARA1){
	//						OPRA_STATUS = C_SUCCEED;			 // 读成功	100
	//					}
	//				}else if(PARA1==100){  //全部复制
	//					for(val=0;val<MAXDIR;){
	//						if((DIR_DREG[val]>=600)&&(DIR_DREG[val]<=920)) count++;
	//						val+=10;
	//					}
	//
	//					count=0;
	//					for(val=0;val<MAXDIR;){
	//						wdt();
	//						if((DIR_DREG[val]>600)&&(DIR_DREG[val]<=920)){	//600无效
	//							ReSREGcount=0;
	//							while(ReSREGcount<READMEMORYCOUNT){
	//								ReadFileRaw(DIR_DREG[val]);
	//								SREG[0]= DIR_DREG[val];
	//								for(k=0;k<4;k++){
	//									fileN[2*k] = (char)(DIR_DREG[val+1+k] & 0x00ff);
	//									if(fileN[2*k] > 96)
	//										fileN[2*k] -= 32;		//转为大写
	//
	//									fileN[2*k+1] = (char)(DIR_DREG[val+1+k] >> 8);
	//									if(fileN[2*k+1] > 96)
	//										fileN[2*k+1] -= 32;		//转为大写
	//
	//									DIR_DREG[val+1+k]=(U16)(fileN[2*k+1]<<8)|fileN[2*k];
	//								}
	//
	//								for(k =0;k<4;k++)
	//									SREG[0x10+k] = DIR_DREG[val+1+k];
	//
	//								SREG[0x20] = 0;
	//
	//								if(CheckSREG(1)==TRUE) break;
	//								ReSREGcount+=1;
	//								wdt();
	//							}
	//							if(ReSREGcount>=READMEMORYCOUNT){
	//								if(result){val+=10; continue;}
	//							}
	//							lenth = ((SREG[2]+0x40)/2)+((((U32)(SREG[5])<<16)+SREG[4])/4)*3;
	//
	//							ReSREGcount=0;
	//							while(ReSREGcount<READFLASHCOUNT){
	//								result=Write_D(D_Address[PTDATA],lenth,(U16 *)(&SREG[0])); // 发送当前花样文件
	//								if(result==0){ //通讯错误
	//									OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//									ReSREGcount=12;//错误跳出
	//									break;
	//								}
	//								DREG[D_COMMAND] = C_WRITEMEMORYPAT;						// 保存花样文件
	//								DREG[D_CMDSTATUS] = 0;
	//								DREG[D_CMDPARA1]  = SREG[0];
	//								result = Write_D(D_Address[DISP]+D_COMMAND,3,(U16 *)(&DREG[D_COMMAND]));			// 发送指令
	//								if(result){
	//									for(i=0;i<(RDTIMES+RDTIMES);i++){
	//										result = Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// 读状态
	//										if((DREG[D_CMDSTATUS])||(result == 0))break;
	//										DelayMs(WAITTIME);
	//									}
	//								}
	//								if((DREG[D_CMDSTATUS]!=100)){
	//									if(DREG[D_CMDSTATUS]==12){	 //主机返回错误重新发数据
	//
	//										ReSREGcount+=1;
	//										continue;
	//									}
	//									OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//									ReSREGcount=12;//错误跳出
	//									break;
	//								}else break;
	//							}
	//							if(ReSREGcount==12) break;//错误跳出
	//						}
	//						val+=10;
	//					}
	//					if(val>=MAXDIR){
	//						OPRA_STATUS = C_SUCCEED;			 // 读成功	100
	//					}
	//				}
	//				ReMemorymode=100;//无意义（只做有效磁盘类型显示）
	//				break;
	//			case C_BACKUPPARA:		// 备份主板参数表到优盘
	//				result=Read_D(D_Address[PARA],D_Length[PARA],(U16 *)(&PREG[0]));	// 从FLASH读取参数值
	//				if(result){
	//					ReadCurTime();//读当前主机时间
	//					file_meg[0]=999;	//标记
	//					s1 = 'S', s2 = 'Y';	file_meg[1] = (U16)s2<<8 | s1;	 //文件名SYS_PATA.999
	//					s1 = 'S', s2 = '_';	file_meg[2] = (U16)s2<<8 | s1;
	//					s1 = 'P', s2 = 'A';	file_meg[3] = (U16)s2<<8 | s1;
	//					s1 = 'T', s2 = 'A';	file_meg[4] = (U16)s2<<8 | s1;
	//					file_meg[5]=0xffff;
	//					file_meg[6]=0xffff;
	//					file_meg[7]=0xffff;
	//					file_meg[8]=(U16)D_Length[PARA]; //文件长度
	//					file_meg[9]=(U16)(D_Length[PARA]>>16); //文件长度
	//					////////////////////////////////////////////////////
	//					fcopydat(file_meg,(U16 *)(&PREG[0]),D_Length[PARA]);
	//					OPRA_STATUS = C_SUCCEED;	 		 // 读成功	100
	//				}else OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//				SearchavailabDir();
	//				break;
	//			case C_RECOVERYPARA:	// 从优盘恢复参数表到主板
	//				result=ReadFileRaw(999);	//从SD卡读取参数值
	//				if(result==F_OK){
	//					result=Write_D(D_Address[PARA],D_Length[PARA],(U16 *)(&PREG[0]));
	//					if(result) OPRA_STATUS = C_SUCCEED;	 		 // 读成功	100
	//					else OPRA_STATUS = C_ERROR;			 // 操作失败	2
	//				}else OPRA_STATUS = C_NOFILE;			 //无此花样
	//				break;
	//			case C_BACKUPPROG:		// 备份主板控制程序到优盘
	//				break;
	//			case C_RECOVERYPROG:	// 从优盘恢复控制程序到主板
	//				break;
	//			case C_FORMATFLASH:		// 格式化（初始化）优盘
	//				SDFormat();
	//				OPRA_STATUS = C_SUCCEED;	 		 // 读成功	100
	//				SearchavailabDir();
	//				break;
	//			default: break;
	//		}
	//	SPI_SetSpeed(SPI_SPEED_MIDDLE);
	return OPRA_STATUS;
}

//------------------------------------------
// Function   : U8 ReadPatList(U8 tt)
// Description:
//------------------------------------------
U8 ReadPatList(U8 tt, U16 pold)
{
	U8 f;
	U16 i;

	if (tt == 2)
	{
		//		f = MemoryDrv_Opera(C_RDFLAPATLIST,SD);
		//		if(f == C_NOFLASH){
		//			return 0;
		//		}else if(f == C_SUCCEED){
		//			return 1;
		//		}
	}
	else
	{
		if (tt == 1)
			DREG[D_COMMAND] = C_RDFLAPATLIST;
		else if (tt == 3)
			DREG[D_COMMAND] = C_RDWORKSHOPLIST;
		else
			DREG[D_COMMAND] = C_RDMEMPATLIST;

		DREG[D_CMDSTATUS] = 0;
		if (tt == 3)
			DREG[D_CMDPARA1] = g_netSelectMac;

		f = Write_D(D_Address[DISP] + D_COMMAND, (tt == 3) ? 3 : 2, (U16 *)(&DREG[D_COMMAND]));
		if (f == 0)
		{
			return 0;
		}
		for (i = 0; i < (RDTIMES); i++)
		{
			f = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
			if ((DREG[D_CMDSTATUS]))
				break;
			DelayMs(WAITTIME);
		}
		ReadFileIndex(DREG[D_COMMAND], pold);
		//Read_D(D_Address[INDEX2],D_Length[INDEX2],(U16 *)(&IREG2[0]));
		if (tt == 0)
			BarcodeCompare();
	}
	return 1;
}

//------------------------------------------
// Function   : U16 SaveSplPatData(void)
// Description: 修改时备份花样数据中绘图、曲线针数据
//------------------------------------------
//U16 SaveSplPatData(void)
//{
//
//	U32 add,add1,dat,dat1;
//	U32 n,l;
//	U32 i;

//	U16 k;
//
//	wdt();				// 喂狗
//	l = 0;
//
//	add = patd.draw_add;
//	add1= patd.line_add;
////	printf("patd.draw_add = %d  patd.line_add = %d patd.pat_add = %d patd.feed_sum = %d\r\n",patd.draw_add,patd.line_add,patd.pat_add,patd.feed_sum);
//	while(add<patd.line_add){
//		dat = SREG[add];    									//绘图数据
//		if(dat==0x201){											//缝纫结束
//			add1++;
//		}else if((dat==0x501)||(dat==0xb01)||(dat==0xc01)||(dat==0xd01)||(dat==0x1d01)){	//功能一-八
//			add1++;
//		}else if((dat==0x401)||(dat==0x1401)||(dat==0x601)||(dat==0x701)||(dat==0x1c01)||(dat==0x3a01)){	//上、下停针 次原点 跳缝 伸延 张力
//			add1++;
//		}else if((dat==0x1f01)||(dat==0x1e01)){					//结束
//			add1++;
//		}else{
//			dat = dat&0x3fd;

//			if(dat==0x88){										//移送
//				dat = SREG[add1];								//线数据
//				for(;add1<patd.pat_add;add1++){
//					if(SREG[add1]!=dat)break;
//				}
//			}else if(dat==0xc4){								//点
//				add1++;
//			}else if(dat==0xc8){								//直线
//				dat = (SREG[add1])&0x3fff;
//				for(;add1<patd.pat_add;add1++){
//					dat1 = (SREG[add1])&0x3fff;
//					if(dat1!=dat)break;
//				}
//			}else if((dat==0x1cc)||(dat==0x1d0)){				//圆 圆弧
//				add = add + 4;
//				dat = (SREG[add1])&0x3fff;
//				for(;add1<patd.pat_add;add1++){
//					dat1 = (SREG[add1])&0x3fff;
//					if(dat1!=dat)break;
//				}
//			}else if((dat==0xd4)||(dat==0x1d4)||(dat==0x2d4)){	//曲线
//				dat = (SREG[add])&0x3fd;
//				if((dat==0x1d4)||(dat==0x2d4)){
//					for(add+=4;add<patd.line_add;add+=4){
//						dat = (SREG[add])&0x3fd;
//						if((dat!=0x1d4)&&(dat!=0x2d4))break;
//					}
//					if(add==patd.line_add)add -= 4;
//				}
//				dat = (SREG[add1])&0x3fff;
//				n = 0;
//				tmp_list[l][1] = add1-patd.line_add;
//				for(;add1<patd.pat_add;add1++){
//					dat1 = (SREG[add1])&0x3fff;
//					if(dat1!=dat)break;
//					n++;
//				}
//				tmp_list[l][0] = n;
//				l++;
//			}else if((dat==0x0d8)||(dat==0x1d8)){				//多边形
//				dat = (SREG[add])&0x1fd;
//				if((dat==0xd8)||(dat==0x1d8)){
//					if(dat==0xd8){
//						n = 0;
//					}else{
//						n = 1;
//						for(add+=4;add<patd.line_add;add+=4){
//							dat = (SREG[add])&0x1fd;
//							if(dat!=0x1d8)break;
//							n++;
//						}
//						if(add==patd.line_add)add -= 4;
//					}
//					if((SREG[30]&0x00ff) == MLK_G){
//						dat = ((SREG[add1])&0x3fff)+n;
//						for(;add1<patd.pat_add;add1++){
//							dat1 = (SREG[add1])&0x3fff;
//							if(dat1>dat)break;
//						}
//					}else{
//						k=0;
//						dat = ((SREG[add1])&0x3fff);
//						for(;add1<patd.pat_add;add1++){
//							dat1 = (SREG[add1])&0x3fff;
//							if(dat1!=dat){
//							   	k++;
//							   	if(k > n){
//							   		break;
//							   	}else{
//									dat = dat1;
//							   	}
//							}
//						}
//					}
//				}
//			}
//		}
//		add = add + 4;

//		wdt();				// 喂狗
//	}
//
//
//	if(l==0){
//		splpatadd = 0;
//		return(0);
//	}

//	if((FLASHADD_P>SST_SREG_EADD)||(FLASHADD_P<SST_SREG_SADD)){
//		FLASHADD_P = SST_SREG_SADD;
//	}

//	SplPatDataSAdd = (U32)(FLASHADD_P);
//
//	dat = (U32)(FLASHADD_P) & 0x00000fff;
//	add = (U32)(FLASHADD_P) & 0xfffff000;
//	if(dat==0){
//		modifyf = 1;
//		if((add>=0x80000000)&&(add<0x81000000))Erase_One_Sector ((add&0xffffff)>>12);
//	}

//	dat = 0x1000 - dat;

//	dat1 = 0;
//	for(n=0;n<l;n++){
//		dat1 += (tmp_list[n][0]*4);
//	}
//	dat1 = dat1 + l * 8 + 4;
//	if(dat >= dat1){
//		i = 0;
//	}else{
//	   	dat1 = dat1 - dat;
//	   	i = dat1 >> 12;
//	   	if((dat1 % 0x1000) != 0)
//	   		i++;
//	}
//	for(;i>0;i--){
//		add = add + 0x1000;
//		if(add>SST_SREG_EADD)add = SST_SREG_SADD;
//		modifyf = 1;
//		if((add>=0x80000000)&&(add<0x81000000))Erase_One_Sector ((add&0xffffff)>>12);
//	}
//

//	splpatadd = (U32)FLASHADD_P;

//	splpatfinoadd = splpatadd;
//

//	splpatfinoNum = l;

//	add = splpatadd + l * 8;
//	if(add>SST_SREG_EADD)add-=(SST_SREG_MAXS * 2);
//	for(n=0;n<l;n++){
//		add1 = patd.pat_add + tmp_list[n][1] * 2;
//		SaveBakData(add);									// FLASHADD_P 曲线针数据起始地址低位
//		SaveBakData(add>>16);								// FLASHADD_P 曲线针数据起始地址高位
//		SaveBakData(tmp_list[n][0]);						// FLASHADD_P 曲线针数据个数
//		SaveBakData(SREG[add1]);							// FLASHADD_P 曲线针数据速度指令(0x6100 0x4100 0x2100 0x0100)
//		dat1 = add1 + tmp_list[n][0] * 2;
//		for(;add1<dat1;add1+=1){							// for(add1++;add1<dat1;add1+=2){ 改为针数据4个字节一起备份
//			Program_One_Word1((U16)(SREG[add1]),add);		// add_sta +
//			add += 2;
//			if(add>SST_SREG_EADD)add = SST_SREG_SADD;
//		}
//	}
//	FLASHADD_P = add;

//	dat = (U32)(FLASHADD_P) & 0x00000fff;
//	if(dat!=0){
//	   dat = ((U32)(FLASHADD_P) & 0xfffff000) + 0x1000;
//	   if(dat>SST_SREG_EADD) dat=SST_SREG_SADD;
//	   FLASHADD_P = dat;
//	}
//	SplPatDataEAdd = (U32)(FLASHADD_P);
//	overflag11=0;
//	wdt();				// 喂狗
//	return(1);
//}

//------------------------------------------
// Function   : void CailImageRange(COORDINATE2 *POS,U16 x0,U16 y0,U16 x1,U16 y1)
// Description:
//------------------------------------------
void CailImageRange(COORDINATE2 *POS, U16 x0, U16 y0, U16 x1, U16 y1)
{
	U16 i, j;
	U32 color;

	//	x0 *= DISP_PARA_X;y0 *= DISP_PARA_Y;
	//	x1 *= DISP_PARA_X;y1 *= DISP_PARA_Y;

	POS->x0 = x1;
	POS->y0 = y1;
	POS->x1 = x0;
	POS->y1 = y0;
	wdt();
	for (i = x0; i < x1; i++)
	{
		for (j = y0; j < y1; j++)
		{
			color = ReadPixel(i, j + 1);
			if (color != 0xDEFBDEFB)
			{
				if (i < POS->x0)
					POS->x0 = i;
				if (j < POS->y0)
					POS->y0 = j;
				if (i > POS->x1)
					POS->x1 = i;
				if (j > POS->y1)
					POS->y1 = j;
			}
		}
	}
}

//------------------------------------------
// Function   : void SaveBakData(U16 dat)
// Description: 保存备份数据
//------------------------------------------
void SaveBakData(U16 dat)
{
	//	#if Debug
	//	printf("SaveBakData  FLASHADD_P = %x dat= %04x \r\n",FLASHADD_P,dat);
	//	#endif
	*(U16 *)FLASHADD_P = dat;
	FLASHADD_P += 2;
	if (FLASHADD_P > SST_SREG_EADD)
		FLASHADD_P = SST_SREG_SADD;
}

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//------------------------------------------
// Function   :
// Description:
//------------------------------------------

//	DisTestData(100,90,x1,y1,0,Color_Black);DisTestData(100,110,x2,y2,0,Color_Black);DisTestData(100,130,x3,y3,0,Color_Black);

//输出附加功能或换轴
U8 OutputExcode(U8 excode, U16 tmppo)
{
	U8 NeedleType = 0;
	U8 f;
	U16 errpo = 0;
	//	#if Debug
	//		printf("OutputExcode1 excode = %02x \r\n",excode);
	//	#endif

#ifdef SYSTEM0
	errpo = MENU_RWHELP;
#endif

#ifdef SYSTEM1
	errpo = MENU_RWHELP;
#endif

#ifdef SYSTEM2
	errpo = MENU_BOR_HELP;
#endif

	//--- 2018-7-4
	g_curExCode ^= excode;
	//excode = g_curExCode^g_ExCode;
	//--------
	if (excode != 0)
	{
		Wait_Move_End(); //等待平台移动结束

		if (g_fbmac && (excode & 0x01))
		{ //折边器,缩板
			if (Write_Cmd(C_ECLAMP, 0, 0) == 0)
			{ //伸缩板
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			if (Read1_Status(10) == 0)
			{
				pno = errpo;
				hleppara = 15;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			else if (DREG[D_CMDSTATUS] != 100)
			{
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			g_ExCode ^= 0x01;
			excode &= 0xfe; //功能一不输出
		}

		if (g_jogfunction || g_jogfunction1)
		{ //输出附加功能

			if ((g_UdZhouFlag == 3) && (excode & 0x80))
			{ //换针
				if ((g_ExCode ^ 0x80) & 0x80)
				{ //D轴
					NeedleType = 1;
				}

				if (Write_Cmd(C_CHANGENEEDLE, NeedleType, 0) == 0)
				{ //换针指令
					pno = errpo;
					hleppara = 16;
					poldno[pnoindex] = tmppo;
					pnoindex++;
					return 0;
				}
				if (Read_Status() == 0)
				{
					pno = errpo;
					hleppara = 15;
					poldno[pnoindex] = tmppo;
					pnoindex++;
					return 0;
				}
				else if (DREG[D_CMDSTATUS] != 100)
				{
					pno = errpo;
					hleppara = 16;
					poldno[pnoindex] = tmppo;
					pnoindex++;
					return 0;
				}
				g_ExCode ^= 0x80;
				excode &= 0x7f; //功能八不输出
			}

			if (excode != 0)
			{
				f = Write_Cmd(C_EXCODE_OUTPUT, excode, 0);
				if (f == 0)
				{
					pno = errpo;
					hleppara = 16;
					poldno[pnoindex] = tmppo;
					pnoindex++;
					return 0;
				}
				if (Read_Status() == 0)
				{
					pno = errpo;
					hleppara = 15;
					poldno[pnoindex] = tmppo;
					pnoindex++;
					return 0;
				}
				else if (DREG[D_CMDSTATUS] != 100)
				{
					pno = errpo;
					hleppara = 16;
					poldno[pnoindex] = tmppo;
					pnoindex++;
					return 0;
				}
				g_ExCode ^= excode; //记录输出附加功能后的状态
			}
		}
		else if ((g_UdZhouFlag == 3) && (excode & 0x80))
		{ //换针
			if ((g_ExCode ^ 0x80) & 0x80)
			{ //D轴
				NeedleType = 1;
			}
			if (Write_Cmd(C_CHANGENEEDLE, NeedleType, 0) == 0)
			{ //换针指令
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex] = tmppo;
				pnoindex++;
				return 0;
			}
			if (Read_Status() == 0)
			{
				pno = errpo;
				hleppara = 15;
				poldno[pnoindex] = tmppo;
				pnoindex++;
				return 0;
			}
			else if (DREG[D_CMDSTATUS] != 100)
			{
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex] = tmppo;
				pnoindex++;
				return 0;
			}
			g_ExCode ^= 0x80;
		}
	}
	return 1;
}

//输出附加功能
U8 OutputExcode1(U8 excode, U16 tmppo, U8 swi)
{
	U8 f;
	U16 errpo = 0;
	//	#if Debug
	//		printf("OutputExcode1 excode = %02x \r\n",excode);
	//	#endif

#ifdef SYSTEM0
	errpo = MENU_RWHELP;
#endif

#ifdef SYSTEM1
	errpo = MENU_RWHELP;
#endif

#ifdef SYSTEM2
	errpo = MENU_BOR_HELP;
#endif

	//--- 2018-7-4
	if (swi == 0)
		g_curExCode ^= excode;
	//excode = g_curExCode^g_ExCode;
	//--------
	if (excode != 0)
	{
		Wait_Move_End(); //等待平台移动结束

		if (g_fbmac && (excode & 0x01))//折边器,缩板
		{ 
			if (Write_Cmd(C_ECLAMP, 0, 0) == 0)//伸缩板
			{ 
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			if (Read1_Status(10) == 0)
			{
				pno = errpo;
				hleppara = 15;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			else if (DREG[D_CMDSTATUS] != 100)
			{
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			g_ExCode ^= 0x01;
			excode &= 0xfe; //功能一不输出
		}

		if (g_jogfunction || g_jogfunction1 || swi)
		{ //输出附加功能
			if (excode != 0)
			{
				f = Write_Cmd(C_EXCODE_OUTPUT, excode, 0);
				if (f == 0)
				{
					pno = errpo;
					hleppara = 16;
					poldno[pnoindex++] = tmppo;
					return 0;
				}
				if (Read_Status() == 0)
				{
					pno = errpo;
					hleppara = 15;
					poldno[pnoindex++] = tmppo;
					return 0;
				}
				else if (DREG[D_CMDSTATUS] != 100)
				{
					pno = errpo;
					hleppara = 16;
					poldno[pnoindex++] = tmppo;
					return 0;
				}
				g_ExCode ^= excode; //记录输出附加功能后的状态
			}
		}
		else if ((g_UdZhouFlag == 3) && (excode & 0x80))
		{ //换针
			f = Write_Cmd(C_EXCODE_OUTPUT, 0x80, 0);
			if (f == 0)
			{
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			if (Read_Status() == 0)
			{
				pno = errpo;
				hleppara = 15;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			else if (DREG[D_CMDSTATUS] != 100)
			{
				pno = errpo;
				hleppara = 16;
				poldno[pnoindex++] = tmppo;
				return 0;
			}
			g_ExCode ^= 0x80; //记录输出附加功能后的状态
		}

		//	#if Debug
		//		printf("OutputExcode g_ExCode = %02x ",g_ExCode);
		//		for(f=0;f<8;f++){
		//			if(g_ExCode&(0x80>>f)) printf("1");
		//			else printf("0");
		//		}
		//		for(f=0;f<8;f++){
		//			if(g_ExCode&(0x80>>f)) printf(" %d",8-f);
		//		}
		//		printf("\r\n");
		//	#endif
	}
	return 1;
}

//循花样轨迹回到原点位置
U8 pathTohome(U32 tmp_pno)
{
	U8 x, y;
	S16 dx, dy, txpos, typos;
	U16 Borcmd;
	U8 speed;
	S8 Borpooft, Borpfdis;
	U32 dat1 = 0;
	U16 dat = 0;
	U8 f, movfeed = 0, excode;

	if (P_FHP == 0)
		return 1;

	if (F_FILETYPE == PAT_MIT)
	{ //三菱
		while (g_jog_point > 0)
		{
			dat1 = g_patd.pat_add + g_jog_point * 2;
			dat = SREG[dat1 - 2] & 0x00ff;
			//			#if Debug
			//			printf("dat = %02x SREG[%d] = %04x \r\n",dat,dat1-2,SREG[dat1-2]);
			//			#endif
			if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
			{
				if ((dat == 0x1b) || (dat == 0x3))
				{
					movfeed = 1;
				}
				dat = SREG[dat1 - 1];
				x = dat & 0x000000ff;
				y = ((U32)(dat >> 8)) & 0x000000ff;
				if (x >= 0x80)
				{
					x -= 0x80;
					dx = x;
				}
				else
				{
					dx = -x;
				}
				if (y >= 0x80)
				{
					y -= 0x80;
					dy = y;
				}
				else
				{
					dy = -y;
				}
				f = Comm_RelationMove(dx, dy, tmp_pno, 0, movfeed);
				movfeed = 0;
				if (f == 0)
				{
					return 0;
				}
			}
			else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
			{ //附加功能
				excode = 0;
				if (dat == 0x05)
					excode = 0x01; //功能1
				else if (dat == 0x0b)
					excode = 0x02; //功能2
				else if (dat == 0x0c)
					excode = 0x04; //功能3
				else if (dat == 0x0d)
					excode = 0x04; //功能4
				else
				{ //功能5-8
					excode = (SREG[dat1 - 2] & 0xff00) >> 8;
					if (excode > 3)
						excode = 0;
					excode = (1 << (excode + 4));
				}
				if (excode != 0)
				{
					f = OutputExcode1(excode, tmp_pno, 0);
					if (f == 0)
					{
						return 0;
					}
				}
			}
			g_jog_point--;

			//---有错误则退出-----------
			Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
			if (S_ERROR)
			{
				Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
				if (DREG[D_ERRWAR])
				{
					return 1;
				}
			}
		}
	}
	else if (F_FILETYPE == PAT_SEW)
	{ //兄弟
		if (g_jog_point > 0)
		{
			g_BorSTflag = 1;
		}
		while (g_jog_point > 0)
		{
			Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &txpos, &typos, (S16 *)&dx, (S16 *)&dy, &speed, &Borpooft, &Borpfdis);

			if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
			{					 //空送,剪线
				Wait_Move_End(); //等待平台移动结束

				if ((Borcmd & 0xf000) == 0x2000)
				{
					movfeed = 1;
				}
			}
			else if ((Borcmd & 0xf000) == 0x6000)
			{ //缝纫
			}
			else if ((Borcmd & 0xff00) == 0xf000)
			{ //中压脚调整
			}
			else if ((Borcmd & 0xff00) == 0xfc00)
			{ //附加功能
				excode = Borcmd & 0xff;
				if (excode != 0)
				{
					f = OutputExcode1(excode, tmp_pno, 0);
					if (f == 0)
					{
						return 0;
					}
				}
			}

			f = Comm_RelationMove(-dx, -dy, tmp_pno, 1, movfeed);
			movfeed = 0;
			if (f == 0)
			{
				return 0;
			}

			g_jog_point--;

			//---有错误则退出-----------
			Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
			if (S_ERROR)
			{
				Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
				if (DREG[D_ERRWAR])
				{
					return 1;
				}
			}
		}
		//		#if Debug
		//		printf("txpos = %d  typos = %d \r\n",txpos,typos);
		//		#endif
		if (g_BorSTflag)
		{
			Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &txpos, &typos, (S16 *)&dx, (S16 *)&dy, &speed, &Borpooft, &Borpfdis);
			Comm_RelationMove(-txpos, -typos, tmp_pno, 1, 1);
			g_BorSTflag = 0;
		}
	}
	return 1;
}

//------------------------------------------
// Function   : U8 Go_Home(U32 tmp_pno)
// Description: 回原点
// flag:0-发送回原点指令 1-先循花样轨迹回到原点,后发送回原点指令 2-循花样轨迹回到原点
//------------------------------------------
U8 Go_Home(U32 tmp_pno, U8 flag)
{
	U8 re = 0;
	U8 f = 0;
	U16 i;

#if Debug
	printf("Go_Home P_FHP = %d g_jog_point = %d tmp_pno = %d flag = %d \r\n", P_FHP, g_jog_point, tmp_pno, flag);
#endif
	if (P_FHP && flag)
	{ //先循花样轨迹回到原点,后发送回原点指令

		//	DelayMs(5000);	//测试用
		f = pathTohome(tmp_pno);
		if (f == 0)
			return 0;
		if (f == 1 && S_ERROR)
			return 1;
		if (flag == 2)
		{
			return 1;
		}
	}
	g_jog_point = 0;

	if (home_f > 1)
		home_f = 0;
	else if ((P_HPK == 1) && (delaytimes == 0))
		home_f = 0;
	if ((tmp_pno != MENU_P1) && (tmp_pno != MENU_P2))
		home_f = 1;

	if ((P_HPK == 1) && (home_f == 0))
	{
		home_f = 1;
		delaytimes = 2000;
		if ((pno != MENU_RWHELP) && (pno != MENU_INHELP))
		{
			pno = MENU_RWHELP;
			hleppara = 18;
			poldno[pnoindex] = tmp_pno;
			pnoindex++;
		}
	}
	else if ((P_HPK == 2) && (home_f == 0))
	{
		home_f = 1;
		DREG[D_COMMAND] = C_XYMOFFLINE;
		DREG[D_CMDSTATUS] = 0;
		DREG[D_CMDPARA1] = 0x11;
		Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND]));
		if ((pno != MENU_RWHELP) && (pno != MENU_INHELP))
		{
			pno = MENU_RWHELP;
			hleppara = 18;
			poldno[pnoindex] = tmp_pno;
			pnoindex++;
		}
	}
	else
	{
		home_f = 0;
#if PATRULES_ORG_OFFSET
		if ((SREG[3] == 7) && (F_FILETYPE == PAT_MIT))
		{
			if ((tmp_pno == MENU_P1) || (tmp_pno == MENU_P2))
				DREG[D_COMMAND] = C_NEWHOME; //C_GOHOME;
			else
				DREG[D_COMMAND] = C_NEWHOME_PRESS_ONT;

			DREG[D_CMDSTATUS] = 0;
			DREG[D_CMDPARA1] = SREG[6];
			DREG[D_CMDPARA2] = SREG[7];
			re = Write_D(D_Address[DISP] + D_COMMAND, 4, (U16 *)(&DREG[D_COMMAND]));
		}
		else
		{
			if ((tmp_pno == MENU_P1) || (tmp_pno == MENU_P2))
				DREG[D_COMMAND] = C_GOHOME;
			else
				DREG[D_COMMAND] = C_GOHOME_PRESS_ONT;

			DREG[D_CMDSTATUS] = 0;
			re = Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
		}
#else
		if ((tmp_pno == MENU_P1) || (tmp_pno == MENU_P2))
			DREG[D_COMMAND] = C_GOHOME;
		else
			DREG[D_COMMAND] = C_GOHOME_PRESS_ONT;

		DREG[D_CMDSTATUS] = 0;
		re = Write_D(D_Address[DISP] + D_COMMAND, 2, (U16 *)(&DREG[D_COMMAND]));
#endif
		if (re == 0)
		{
		}
		else
		{
			for (i = 0; i < 4 * RDTIMES; i++)
			{
				re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS]));
				if ((re == 0) || (DREG[D_CMDSTATUS]))
					break;
				DelayMs(WAITTIME);
			}
		}
	}

	Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));

	return (re);

	//	return 0;
}

//------------------------------------------
// Function   : U8 Send_Pfoot(U8 mode,U32 tmp_pno)
// Description: 压脚升降
//------------------------------------------
U8 Send_Pfoot(U8 mode, U32 tmp_pno, U8 flag)
{
	U16 f;
	U16 para2 = 0;
	Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
	if (mode == 0)
	{ // 反转
		if (S_PFOOT)
			f = C_PFOOTUP;
		else
			f = C_PFOOTDN;
	}
	else if ((mode == 1) && (S_PFOOT))
	{ // 上升
		f = C_PFOOTUP;
	}
	else if ((mode == 2) && ((S_PFOOT) == 0))
	{ // 下降
		f = C_PFOOTDN;
	}
	else
	{
		f = 0;
	}
	if (flag)
		para2 = 0x5a;
	if (f)
	{
		if (Write_Cmd1(f, 0, para2) == 0)
		{
			if (DREG[D_CMDSTATUS] == 0)
			{
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex] = tmp_pno;
				pnoindex++;
			}
		}
		f = Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
		if (f == 0)
		{
			pno = MENU_RWHELP;
			hleppara = 15;
			poldno[pnoindex] = tmp_pno;
			pnoindex++;
		}
	}

	return (f);
}

//------------------------------------------
// Function   : U8 NeddleUp(U32 tmp_pno)
// Description:
//------------------------------------------
#define NEEDLE_UP_DW 0x02000000
U8 NeddleUp(U32 tmp_pno)
{
	//U16 i;
	//U32 InputP;
	U8 status = 1;

	//	i = Read_D(D_Address[SYS]+SYS_INPUTL,1,(U16 *)(&YREG[SYS_INPUTL]));// 输入口当前状态低位
	//	if(i==0){
	//		pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex] = tmp_pno;pnoindex++;status = 0;
	//	}
	//
	//	i = Read_D(D_Address[SYS]+SYS_INPUTH,1,(U16 *)(&YREG[SYS_INPUTH]));//输入口当前状态高位
	//	if(i==0){
	//		pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex] = tmp_pno;pnoindex++;status = 0;
	//	}
	//	InputP = ((U32)(YREG[SYS_INPUTH]) << 16) + (U32)(YREG[SYS_INPUTL]);
	//
	//	if((InputP & NEEDLE_UP_DW)){
	//		pno = MENU_RWHELP;hleppara = 94;poldno[pnoindex] = tmp_pno;pnoindex++;status = 0;
	//	}
	status = (U8)(S_NEEDLE);
	//printf("status = %0x  \r\n",status);
#ifdef SYSTEM0
	if (status == 0)
	{ //0 低  0x20高
		pno = MENU_RWHELP;
		hleppara = 94;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
	}
	else
	{
		status = 1;
	}
#endif

#ifdef SYSTEM1
	if (status == 0)
	{ //0 低  0x20高
		pno = MENU_RWHELP;
		hleppara = 94;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
	}
	else
	{
		status = 1;
	}
#endif

#ifdef SYSTEM2
	if (status == 0)
	{ //0 低  0x20高
		pno = MENU_BOR_HELP;
		hleppara = 71;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
	}
	else
	{
		status = 1;
	}
#endif

	return status;
}

//------------------------------------------
// Function   : U8 Comm_Move(S16 x,S16 y,U8 para,U32 tmp_pno)
// U8 borflag : 1-兄弟款  2017-2-10
// Description: 移动坐标(绝对坐标）
//------------------------------------------
U8 Comm_Move(S16 x, S16 y, U8 para, U32 tmp_pno, U8 borflag, U8 movfeed)
{
	U8 re;
	U16 i;
	U16 movtype;
//#if Debug
//printf("Comm_Move \r\n");
//#endif
#ifdef SYSTEM0
	if ((!(S_CLAMP)) && (P_OST == 0))
	{
		pno = MENU_RWHELP;
		hleppara = 13;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
#endif

#ifdef SYSTEM1
	if ((!(S_CLAMP)) && (P_OST == 0))
	{
		pno = MENU_RWHELP;
		hleppara = 13;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
#endif

#ifdef SYSTEM2
	if ((!(S_CLAMP)) && (P_OST == 0))
	{
		pno = MENU_BOR_HELP;
		hleppara = 26;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
#endif

	if (borflag == 1)
	{
		DREG[D_COMMAND] = C_ABSLTNSSMOVE_BOR;
	}
	else
	{
		DREG[D_COMMAND] = C_ABSLTNSSMOVE;
	}

	if (movfeed == 1)
	{
		movtype = 0x5aa5;
	}
	else
	{
		movtype = 0;
	}
	DREG[D_CMDSTATUS] = para;
	DREG[D_CMDPARA1] = x;
	DREG[D_CMDPARA2] = y;
	DREG[D_CMDPARA3] = movtype;

	//	DrawFillRect(560,80,240,40,Color_White);
	//	DrawFU32(560,80, DREG[D_COMMAND],5,Color_Blue);
	//	DrawFU32(610,80, DREG[D_CMDSTATUS],5,Color_Blue);
	//	DrawFU32(660,80, DREG[D_CMDPARA1],5,Color_Blue);
	//	DrawFU32(710,80, DREG[D_CMDPARA2],5,Color_Blue);
	//	DrawFU32(760,80, DREG[D_CMDPARA3],5,Color_Blue);
	//	DrawFS32P(660, 100, (S16)DREG[D_CMDPARA1],5,0,Color_Blue);
	//	DrawFS32P(710, 100, (S16)DREG[D_CMDPARA2],5,0,Color_Blue);

	re = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // 发送指令

	if (re == 0)
	{
#ifdef SYSTEM0
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif

#ifdef SYSTEM1
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif

#ifdef SYSTEM2
		pno = MENU_BOR_HELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif
	}

	for (i = 0; i < RDTIMES * 3; i++)
	{
		re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
		if (DREG[D_CMDSTATUS] == 100)
			break;
		DelayMs(WAITTIME);
	}

	//	DrawFU32(560,80, DREG[D_COMMAND],5,Color_Red);
	//	DrawFU32(610,100, DREG[D_CMDSTATUS],5,Color_Red);
	//	DrawFU32(660,80, DREG[D_CMDPARA1],5,Color_Red);
	//	DrawFU32(710,80, DREG[D_CMDPARA2],5,Color_Red);
	//	DrawFU32(760,80, DREG[D_CMDPARA3],5,Color_Red);
	//
	//	DrawFS32P(660, 100, (S16)DREG[D_CMDPARA1],5,0,Color_Red);
	//	DrawFS32P(710, 100, (S16)DREG[D_CMDPARA2],5,0,Color_Red);
	//	#if Debug
	//	printf("i = %d re = %d  DREG[D_CMDSTATUS] = %d\r\n",i,re,DREG[D_CMDSTATUS]);
	//	#endif
	return (re);
}

//------------------------------------------
// Function   : U8 Comm_Move(S16 x,S16 y,U8 para,U32 tmp_pno)
// U8 borflag : 1-兄弟款  2017-2-10
// Description: 移动坐标(绝对坐标）
//------------------------------------------
U8 Comm_Move1(S16 x, S16 y, U8 para, U32 tmp_pno, U8 borflag, U8 movfeed)
{
	U8 re = 1;
	//	U16 i;
	U16 movtype;
//#if Debug
//printf("Comm_Move \r\n");
//#endif
#ifdef SYSTEM0
	if ((!(S_CLAMP)) && (P_OST == 0))
	{
		pno = MENU_RWHELP;
		hleppara = 13;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
#endif

#ifdef SYSTEM1
	if ((!(S_CLAMP)) && (P_OST == 0))
	{
		pno = MENU_RWHELP;
		hleppara = 13;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
#endif

#ifdef SYSTEM2
	if ((!(S_CLAMP)) && (P_OST == 0))
	{
		pno = MENU_BOR_HELP;
		hleppara = 26;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
#endif

	if (borflag == 1)
	{
		DREG[D_COMMAND] = C_ABSLTNSSMOVE_BOR;
	}
	else
	{
		DREG[D_COMMAND] = C_ABSLTNSSMOVE;
	}

	if (movfeed == 1)
	{
		movtype = 0x5aa5;
	}
	else
	{
		movtype = 0;
	}
	DREG[D_CMDSTATUS] = para;
	DREG[D_CMDPARA1] = x;
	DREG[D_CMDPARA2] = y;
	DREG[D_CMDPARA3] = movtype;
	re = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // 发送指令
	if (re == 0)
	{
#ifdef SYSTEM0
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif

#ifdef SYSTEM1
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif

#ifdef SYSTEM2
		pno = MENU_BOR_HELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif
	}

	//	#if Debug
	//	printf("i = %d re = %d  DREG[D_CMDSTATUS] = %d\r\n",i,re,DREG[D_CMDSTATUS]);
	//	#endif
	return (re);
}

void Read_Move1(U32 tmp_pno)
{
	U8 re;
	U16 i;
	for (i = 0; i < RDTIMES * 10; i++)
	{
		re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
		if (DREG[D_CMDSTATUS] == 100)
			break;
		DelayMs(WAITTIME);
	}
	if (re == 0)
	{
#ifdef SYSTEM0
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif

#ifdef SYSTEM1
		pno = MENU_RWHELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif

#ifdef SYSTEM2
		pno = MENU_BOR_HELP;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
#endif
	}
}
//------------------------------------------
// Function   : U8 Comm_RelationMove(S16 x,S16 y,U32 tmp_pno)
// U8 borflag : 1-兄弟款  2017-2-10
// Description: 移动坐标(相对坐标）
//------------------------------------------
U8 Comm_RelationMove(S16 x, S16 y, U32 tmp_pno, U8 borflag, U8 movfeed)
{
	U8 re;
	U16 i;
	U16 movtype;

	U32 errpno = MENU_RWHELP;

	if (borflag == 1)
	{
		DREG[D_COMMAND] = C_RELATIONMOVE_BOR;
	}
	else
	{
		DREG[D_COMMAND] = C_RELATIONMOVE;
	}

	//	#if Debug
	//	printf("Comm_RelationMove x = %d y = %d borflag = %d \r\n",x,y,borflag);
	//	#endif
	if (movfeed == 1)
	{
		movtype = 0x5aa5;
	}
	else
	{
		movtype = 0;
	}
	DREG[D_CMDSTATUS] = 0;
	DREG[D_CMDPARA1] = x;
	DREG[D_CMDPARA2] = y;
	DREG[D_CMDPARA3] = movtype;
	re = Write_D(D_Address[DISP] + D_COMMAND, 5, (U16 *)(&DREG[D_COMMAND])); // 发送指令
	if (re == 0)
	{
		pno = errpno;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}

	for (i = 0; i < RDTIMES; i++)
	{
		re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
		if ((re == 0) || (DREG[D_CMDSTATUS] == 100))
			break;
		DelayMs(WAITTIME);
	}
	if ((re == 0) || (DREG[D_CMDSTATUS] == 0))
	{
		pno = errpno;
		hleppara = 15;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}
	else if (DREG[D_CMDSTATUS] < 100)
	{
		pno = errpno;
		hleppara = 16;
		poldno[pnoindex] = tmp_pno;
		pnoindex++;
		return (0);
	}

	return (1);
}

//------------------------------------------
// Function   : void Wait_Move_End(void)
// Description: 等待平台结束
//------------------------------------------
void Wait_Move_End(void)
{
	U16 times = 0;
	while (times < 2000)
	{ //等待平台移动结束
		if (Write_Cmd1(C_RDCMDSTATUS, 0, 0))
			break;
		DelayMs(WAITTIME);
		times++;
	}
}

//------------------------------------------
// Function   : U8 Read_xyposi(void)
// Description: 读取XY轴绝对坐标
//------------------------------------------
U8 Read_xyposi(void)
{
#if MOVEUNIT == 1
	if (Write_Cmd1(C_READPOSI, g_curPTtype == 2 ? 0x5a : 0, 0))
	{
		if (Read_D(D_Address[DISP] + D_CMDPARA1, 7, (U16 *)(&DREG[D_CMDPARA1])))
		{
			g_xabspos = (S16)(DREG[D_CMDPARA1]);
			g_yabspos = (S16)(DREG[D_CMDPARA2]);
			g_pf_absdis = (S16)(DREG[D_CMDPARA3]);
			g_uabspos = (S16)(DREG[D_CMDPARA4]);
			g_vabspos = (S16)(DREG[D_CMDPARA5]);
			g_wabspos = (S16)(DREG[D_CMDPARA7]);
			return (1);
		}
	}
#else
	if (Write_Cmd1(C_READPOSI, 0, 0))
	{
		if (Read_D(D_Address[DISP] + D_CMDPARA1, 7, (U16 *)(&DREG[D_CMDPARA1])))
		{
#if PATRULES_ORG_OFFSET
			S16 ox = 0, oy = 0;
			if (SREG[3] == 7 && F_FILETYPE == PAT_MIT) //新增-修改原点位置
			{
				ox = (S16)(SREG[6]);
				oy = (S16)(SREG[7]);
			}
			g_xabspos = ox + (S16)(DREG[D_CMDPARA1]);
			g_yabspos = oy + (S16)(DREG[D_CMDPARA2]);
#else
			g_xabspos = (S16)(DREG[D_CMDPARA1]);
			g_yabspos = (S16)(DREG[D_CMDPARA2]);
#endif
			g_pf_absdis = (S16)(DREG[D_CMDPARA3]);
			g_uabspos = (S16)(DREG[D_CMDPARA4]);
			g_vabspos = (S16)(DREG[D_CMDPARA5]);
			g_wabspos = (S16)(DREG[D_CMDPARA7]);
			return (1);
		}
	}
#endif
	return (0);
}

//------------------------------------------
// Function   : U8 Read_ModifyPara(void)
// Description: 读取修正参数
//------------------------------------------
U8 Read_ModifyPara(void)
{

	Read_D(D_Address[SYS] + SYS_PARA4X, 3, (U16 *)(&YREG[SYS_PARA4X]));
	g_ViewModParaX = (S16)(YREG[SYS_PARA4X]);
	g_ViewModParaY = (S16)(YREG[SYS_PARA4Y]);

	if (Read_D(D_Address[SYS] + SYS_PARA1CHKSUM, 20, (U16 *)(&YREG[SYS_PARA1CHKSUM])))
	{
		g_PinModChkSum = (S16)(YREG[SYS_PARA1CHKSUM]);
		g_PinModParaX1 = (S16)(YREG[SYS_PARA1X1]);
		g_PinModParaY1 = (S16)(YREG[SYS_PARA1Y1]);
		g_PinModParaX2 = (S16)(YREG[SYS_PARA1X2]);
		g_PinModParaY2 = (S16)(YREG[SYS_PARA1Y2]);
		g_PinModParaX3 = (S16)(YREG[SYS_PARA1X3]);
		g_PinModParaY3 = (S16)(YREG[SYS_PARA1Y3]);
		g_PinModParaXR = (S16)(YREG[SYS_PARA1XR]);
		g_PinModParaYR = (S16)(YREG[SYS_PARA1YR]);
		g_PinModParaRD = (S16)(YREG[SYS_PARA1RD]);
		g_PinModParaANG = (S16)(YREG[SYS_PARA1ANG]);

		g_OrgModChkSum = (S16)(YREG[SYS_PARA2CHKSUM]);
		g_OrgModParaX = (S16)(YREG[SYS_PARA2XORG]);
		g_OrgModParaY = (S16)(YREG[SYS_PARA2YORG]);
		g_OrgModParaZ = (S16)(YREG[SYS_PARA2PFOOT]);

		g_GapModChkSum = (S16)(YREG[SYS_PARA3CHKSUM]);
		g_GapModPosadjX = (S16)(YREG[SYS_PARA3XZADJ]);
		g_GapModNegadjX = (S16)(YREG[SYS_PARA3XFADJ]);
		g_GapModPosadjY = (S16)(YREG[SYS_PARA3YZADJ]);
		g_GapModNegadjY = (S16)(YREG[SYS_PARA3YFADJ]);

		if (g_UdZhouFlag == 3)
		{
			Read_D(D_Address[SYS] + SYS_PARA2XDORG, 2, (U16 *)(&YREG[SYS_PARA2XDORG]));
			g_OrgModParaXD = (S16)(YREG[SYS_PARA2XDORG]);
			g_OrgModParaYD = (S16)(YREG[SYS_PARA2YDORG]);
		}

		return (1);
	}

	return (0);
}

//------------------------------------------
// Function   : U8 CopyFileCom(U16 cmd,U16* p,U8 cnt)
// Description: 向主机发送复制文件命令
//------------------------------------------
U8 CopyFileCom(U16 cmd, U16 *p, U8 cnt)
{
	U8 i, flag = 0;
	U8 result = 0;
	U16 j;
	U16 tmp = 0;
	if (cnt <= 10)
	{ //当cnt 为100 时,表示复制全部文件
		for (i = PAG_COPY_FINO; i < cnt; i++)
		{
			DREG[i] = *p++;
		}
		Write_D(D_Address[PAGEBUF] + PAG_COPY_FINO, cnt, (U16 *)(&DREG[PAG_COPY_FINO]));
	}

	if (Write_Cmd(cmd, cnt, 0))
	{
		for (j = 0; j < RDTIMES; i++)
		{
			result = Read_D(D_Address[DISP] + D_CMDSTATUS, 3, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
			if ((DREG[D_CMDSTATUS] == 100) && (result == 1))
			{
				return (result);
			}
			else if (DREG[D_CMDSTATUS] == 0)
			{ //D_CMDSTATUS,
				if (tmp != (DREG[D_CMDPARA2] + 1))
				{
					if (P_LAN == 2)
					{ //越南语
						DrawFillRect(460, 256, 38, 24, Color_Blue);
						DrawMFU32Z(460, 256, DREG[D_CMDPARA2] + 1, 3, Color_Red);
					}
					else
					{
						DrawFillRect(280, 256, 38, 24, Color_Blue);
						DrawMFU32Z(280, 256, DREG[D_CMDPARA2] + 1, 3, Color_Red);
					}
					//DrawFillRect(280,256,38,24,Color_Blue);
					//DrawMFU32Z(280, 256,DREG[D_CMDPARA2]+1, 3, Color_Red);
					tmp = DREG[D_CMDPARA2] + 1;
				}

				if ((flag == 0) && (DREG[D_CMDPARA2] != 0) && (cmd == C_COPYFLASHPAT))
				{
					flag = 1;
					//DrawFillRect(548,256,38,24,Color_Blue);
					//DrawMFU32Z(548, 256, DREG[D_CMDPARA1], 3, Color_Red);
					if (P_LAN == 2)
					{ //越南语
						DrawFillRect(206, 290, 38, 24, Color_Blue);
						DrawMFU32Z(206, 290, DREG[D_CMDPARA1], 3, Color_Red);
					}
					else
					{
						DrawFillRect(548, 256, 38, 24, Color_Blue);
						DrawMFU32Z(548, 256, DREG[D_CMDPARA1], 3, Color_Red);
					}
				}
			}
			DelayMs(WAITTIME);
		}
	}

	return (result);
}

//------------------------------------------
// Function   : U8 Read_DCMDSTATUS(U32 Waitime)
// Description: 向主机发送复制文件命令
//------------------------------------------
U8 Read_DCMDSTATUS(U32 Waitime)
{
	U8 re = 0;
	U32 iii;

	for (iii = 0; iii < Waitime; iii++)
	{
		re = Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
		if ((DREG[D_CMDSTATUS]))
			break;
		DelayMs(WAITTIME);
	}

	return re;
}

//------------------------------------------
// Function   : void Go_Stpoint(U32 tmp_pno)
// Description: 返回启始点
//------------------------------------------
void Go_Stpoint(U32 tmp_pno, U8 flag)
{
	U8 x, y;
	U32 dat, dat1, i;
	S16 tmp_PfootOffVal;
	S8 pfootval;
	U8 f;
	U8 Up = 1;
	U8 pfootf;
	S8 Borpooft = 0, Borpfdis = 0;
	U8 speed;
	U16 Borcmd = 0x2000;
	S16 dx, dy;

	if (flag == 0)
	{ //三菱
		if (g_jog_point > 0)
		{
			f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

			if (f == 0)
			{ //读设定中压脚修正量
#ifdef SYSTEM0
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex] = tmp_pno;
				pnoindex++;
				return; //bpnohp = tmp_pno;
#endif

#ifdef SYSTEM1
				pno = MENU_RWHELP;
				hleppara = 15;
				poldno[pnoindex] = tmp_pno;
				pnoindex++;
				return; //bpnohp = tmp_pno;
#endif

#ifdef SYSTEM2
				pno = MENU_BOR_HELP;
				hleppara = 15;
				poldno[pnoindex++] = tmp_pno;
				return; //bpnohp = tmp_pno;
#endif
			}
			tmp_PfootOffVal = YREG[SYS_PFOOTSET];
			pfootval = Mit_CailPadOffestPF(g_jog_stpoint, 0, 0); //计算jog_point之前的中压脚高度变化量
			tmp_PfootOffVal += pfootval;

			Send_Pfoot(1, tmp_pno, 1); //压脚升
			f = Write_Cmd(C_PFOOTACTION, tmp_PfootOffVal, 0);
			for (i = 0; i < RDTIMES; i++)
			{
				Read_D(D_Address[DISP] + D_CMDSTATUS, 1, (U16 *)(&DREG[D_CMDSTATUS])); // 读状态
				if ((DREG[D_CMDSTATUS]) == 100)
					break;
				DelayMs(WAITTIME);
			}
		}

		while (g_jog_point > g_jog_stpoint)
		{
			dat1 = g_patd.pat_add + g_jog_point * 2;
			dat = SREG[dat1 - 2] & 0x00ff;
			if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
			{
				dat = SREG[dat1 - 1];
				x = dat & 0x000000ff;
				y = ((U32)(dat >> 8)) & 0x000000ff;
				if (x >= 0x80)
				{
					x -= 0x80;
					g_dxpos = x;
				}
				else
				{
					g_dxpos = -x;
				}
				if (y >= 0x80)
				{
					y -= 0x80;
					g_dypos = y;
				}
				else
				{
					g_dypos = -y;
				}
				f = Comm_RelationMove(g_dxpos, g_dypos, tmp_pno, 0, 0);
				if (f == 0)
				{
					break;
				}
			}
			g_jog_point--;
		}
	}
	else if (flag == 1)
	{ //兄弟
		pfootf = (U8)(S_PFOOT);
		if (pfootf == 0x10)
			Up = 1; //下面
		else
			Up = 0;
		while (g_jog_point > g_jog_stpoint)
		{
			g_jog_point--;
			Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_temp_oxpos, (S16 *)&g_temp_oypos, &dx, &dy, &speed, &Borpooft, &Borpfdis);

			if (Up == 1)
			{
				Up = 0;
				Send_Pfoot(1, tmp_pno, 1); //压脚升
			}
			g_temp_oxpos += dx;
			g_temp_oypos += dy;
			Comm_Move(g_temp_oxpos, g_temp_oypos, 50, tmp_pno, 1, 0);
		}
	}
}

#define RECT_DISDATA_X_STR 0	 //2 X坐标-内容
#define RECT_DISDATA_Y_STR 1	 //4 Y坐标-内容
#define RECT_DISDATA_XX_STR 2	 //6 X挪动值-内容
#define RECT_DISDATA_XY_STR 3	 //8 Y挪动值-内容
#define RECT_DISDATA_PITCH_STR 4 //10 针距-内容
#define RECT_DISDATA_SP_STR 5	 //12 速度-内容
#define RECT_DISDATA_CODE_STR 6	 //14 代码-内容
#define RECT_DISDATA_PINS_STR 7	 //16 针数-内容
#define RECT_DISDATA_P_STR 8	 //18 P-内容
#define RECT_DISDATA_LF_STR 9	 //20 松线-内容
#define RECT_DISDATA_PF_STR 10	 //22 压脚高度-内容
#define RECT_DISDATA_XPF_STR 11	 //24 压脚高度绝对值-内容

void dis_data(U16 dat, U8 div)
{
	U8 spd = 0, feed_f;
	U8 ii;
	U16 *padd;

	S8 pfoot = 0;
	S16 dx, dy;
	U16 psheet;
	static U8 tmp_spd, tmp_feedf;
	static U16 tmp_feedc, tmp_feedsum;

	const RECTPAD rect[] = {
		//方框
		{TYPE_X1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE14, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}},  //2 X坐标-内容
		{TYPE_X1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_Y29, TYPE_SIZE14, TYPE_SIZE4, {Color_White, Color_DeepSkyBlue}, {Color_Black, Color_Black}, {0}}, //4 Y坐标-内容

		{TYPE_X28 + TYPE_SIZE9 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},  //6 X挪动值-内容
		{TYPE_X28 + TYPE_SIZE9 + SAFE_DIST3, TYPE_Y29, TYPE_SIZE9, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //8 Y挪动值-内容

		{TYPE_X29 + TYPE_SIZE21 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},	//10 针距-内容
		{TYPE_X29 + TYPE_SIZE21 + SAFE_DIST3, TYPE_Y29, TYPE_SIZE9, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //12 速度-内容

		{TYPE_X30 + TYPE_SIZE21 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE14, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},	 //14 代码-内容
		{TYPE_X30 + TYPE_SIZE21 + SAFE_DIST3, TYPE_Y29, TYPE_SIZE14, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //16 针数-内容

		{TYPE_X31 + TYPE_SIZE9 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE6, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},  //18 P-内容
		{TYPE_X31 + TYPE_SIZE9 + SAFE_DIST3, TYPE_Y29, TYPE_SIZE6, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //20 松线-内容

		{TYPE_X32 + TYPE_SIZE9 + SAFE_DIST3, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},  //22 压脚高度-内容
		{TYPE_X32 + TYPE_SIZE9 + SAFE_DIST3, TYPE_Y29, TYPE_SIZE9, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //24 压脚高度绝对值-内容
	};

#if MOVEUNIT == 1
	const U8 vimut = 5;
#endif

	if (P_LAN)
	{
		padd = MEM_TITLE_CODENAME_E;
		psheet = 34;
	}
	else
	{
		padd = MEM_TITLE_CODENAME;
		psheet = 34;
	}
	if (g_temp_pno != pno)
	{
		g_temp_pno = pno;
		tmp_spd = 0xff;
		tmp_feedf = 0xff;
		tmp_feedc = 0xffff;
		tmp_feedsum = 0xffff;
	}
	if (div == 1)
	{
#if MOVEUNIT == 1
		dx = g_dxpos;
		dy = g_dypos;
#else
		dx = g_dxpos / 2;
		dy = g_dypos / 2;
#endif
	}
	else
	{
		dx = g_dxpos;
		dy = g_dypos;
	}

#if MOVEUNIT == 1
	DrawFillRect(26, 34, 58, 40, mCOLOR ? Color_Silver : Color_White);
	DrawFillRect(118, 34, 58, 40, mCOLOR ? Color_Silver : Color_White);
	if (g_curPTtype == 2)
	{

		if (g_num_mov_key)
		{
			if (g_editflag)
			{
				DrawFS32P(26, 36, dx * vimut, 7, 2, Color_Black);
				DrawFillRect(26, 54, 58, 20, Color_Black);
				DrawFS32P(26, 56, dy * vimut, 7, 2, Color_White);
			}
			else
			{
				DrawFillRect(26, 34, 58, 20, Color_Black);
				DrawFS32P(26, 36, dx * vimut, 7, 2, Color_White);
				DrawFS32P(26, 56, dy * vimut, 7, 2, Color_Black);
			}
		}
		else
		{
			DrawFS32P(26, 36, dx * vimut, 7, 2, Color_Black);
			DrawFS32P(26, 56, dy * vimut, 7, 2, Color_Black);
		}
		//xy坐标绝对直
		//DrawFillRect(118,34,50,40,mCOLOR?Color_Silver:Color_White);
		DrawFS32P(118, 36, g_xabspos * vimut, 7, 2, Color_Black);
		DrawFS32P(118, 56, g_yabspos * vimut, 7, 2, Color_Black);
	}
	else
	{
		//DrawFillRect(26,34,50,40,mCOLOR?Color_Silver:Color_White);
		if (g_num_mov_key)
		{
			if (g_editflag)
			{
				DrawFS32P(26, 36, dx, 6, 1, Color_Black);
				DrawFillRect(26, 54, 50, 20, Color_Black);
				DrawFS32P(26, 56, dy, 6, 1, Color_White);
			}
			else
			{
				DrawFillRect(26, 34, 50, 20, Color_Black);
				DrawFS32P(26, 36, dx, 6, 1, Color_White);
				DrawFS32P(26, 56, dy, 6, 1, Color_Black);
			}
		}
		else
		{
			DrawFS32P(26, 36, dx, 6, 1, Color_Black);
			DrawFS32P(26, 56, dy, 6, 1, Color_Black);
		}
		//xy坐标绝对直
		//DrawFillRect(118,34,50,40,mCOLOR?Color_Silver:Color_White);
		DrawFS32P(118, 36, g_xabspos, 6, 1, Color_Black);
		DrawFS32P(118, 56, g_yabspos, 6, 1, Color_Black);
	}

#else
	// DrawFillRect(26, 34, 50, 40, mCOLOR ? Color_Silver : Color_White);
	if (g_num_mov_key)
	{
		if (g_editflag)
		{
			// DrawFS32P(26, 36, dx, 6, 1, Color_Black);
			// DrawFillRect(26, 54, 50, 20, Color_Black);
			// DrawFS32P(26, 56, dy, 6, 1, Color_White);
			DrawFillRect(rect[RECT_DISDATA_X_STR].xpis, rect[RECT_DISDATA_X_STR].ypis,
						 rect[RECT_DISDATA_X_STR].xsize, rect[RECT_DISDATA_X_STR].ysize, rect[RECT_DISDATA_X_STR].colorB[0]);
			DrawFS32P(rect[RECT_DISDATA_X_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_X_STR].ypis + SAFE_DIST4, dx, 6, 1, rect[0].colorT[0]);

			DrawFillRect(rect[RECT_DISDATA_Y_STR].xpis, rect[RECT_DISDATA_Y_STR].ypis,
						 rect[RECT_DISDATA_Y_STR].xsize, rect[RECT_DISDATA_Y_STR].ysize, rect[RECT_DISDATA_Y_STR].colorB[1]);
			DrawDottedRect(rect[RECT_DISDATA_Y_STR].xpis, rect[RECT_DISDATA_Y_STR].ypis,
						   rect[RECT_DISDATA_Y_STR].xsize, rect[RECT_DISDATA_Y_STR].ysize, rect[RECT_DISDATA_Y_STR].colorT[1]);
			DrawFS32P(rect[RECT_DISDATA_Y_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_Y_STR].ypis + SAFE_DIST4,
					  dy, 6, 1, rect[RECT_DISDATA_Y_STR].colorT[1]);
		}
		else
		{
			// DrawFillRect(26, 34, 50, 20, Color_Black);
			// DrawFS32P(26, 36, dx, 6, 1, Color_White);
			// DrawFS32P(26, 56, dy, 6, 1, Color_Black);
			DrawFillRect(rect[RECT_DISDATA_X_STR].xpis, rect[RECT_DISDATA_X_STR].ypis,
						 rect[RECT_DISDATA_X_STR].xsize, rect[RECT_DISDATA_X_STR].ysize, rect[RECT_DISDATA_X_STR].colorB[1]);
			DrawDottedRect(rect[RECT_DISDATA_X_STR].xpis, rect[RECT_DISDATA_X_STR].ypis,
						   rect[RECT_DISDATA_X_STR].xsize, rect[RECT_DISDATA_X_STR].ysize, rect[RECT_DISDATA_X_STR].colorT[1]);
			DrawFS32P(rect[RECT_DISDATA_X_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_X_STR].ypis + SAFE_DIST4,
					  dx, 6, 1, rect[RECT_DISDATA_X_STR].colorT[1]);

			DrawFillRect(rect[RECT_DISDATA_Y_STR].xpis, rect[RECT_DISDATA_Y_STR].ypis,
						 rect[RECT_DISDATA_Y_STR].xsize, rect[RECT_DISDATA_Y_STR].ysize, rect[RECT_DISDATA_Y_STR].colorB[0]);
			DrawFS32P(rect[RECT_DISDATA_Y_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_Y_STR].ypis + SAFE_DIST4,
					  dy, 6, 1, rect[RECT_DISDATA_Y_STR].colorT[0]);
		}
	}
	else
	{
		// DrawFS32P(26, 36, dx, 6, 1, Color_Black);
		// DrawFS32P(26, 56, dy, 6, 1, Color_Black);
		DrawFillRect(rect[RECT_DISDATA_X_STR].xpis, rect[RECT_DISDATA_X_STR].ypis,
					 rect[RECT_DISDATA_X_STR].xsize, rect[RECT_DISDATA_X_STR].ysize, rect[RECT_DISDATA_X_STR].colorB[0]);
		DrawFS32P(rect[RECT_DISDATA_X_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_X_STR].ypis + SAFE_DIST4,
				  dx, 6, 1, rect[RECT_DISDATA_X_STR].colorT[0]);
		DrawFillRect(rect[RECT_DISDATA_Y_STR].xpis, rect[RECT_DISDATA_Y_STR].ypis,
					 rect[RECT_DISDATA_Y_STR].xsize, rect[RECT_DISDATA_Y_STR].ysize, rect[RECT_DISDATA_Y_STR].colorB[0]);
		DrawFS32P(rect[RECT_DISDATA_Y_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_Y_STR].ypis + SAFE_DIST4,
				  dy, 6, 1, rect[RECT_DISDATA_Y_STR].colorT[0]);
	}
	//xy坐标绝对直
	// DrawFillRect(118, 34, 50, 40, mCOLOR ? Color_Silver : Color_White);
	// DrawFS32P(118, 36, g_xabspos, 6, 1, Color_Black);
	// DrawFS32P(118, 56, g_yabspos, 6, 1, Color_Black);

	DrawFillRect(rect[RECT_DISDATA_XX_STR].xpis, rect[RECT_DISDATA_XX_STR].ypis,
				 rect[RECT_DISDATA_XX_STR].xsize, rect[RECT_DISDATA_XX_STR].ysize, rect[RECT_DISDATA_XX_STR].colorB[0]);
	DrawFS32P(rect[RECT_DISDATA_XX_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_XX_STR].ypis + SAFE_DIST4,
			  g_xabspos, 6, 1, rect[RECT_DISDATA_XX_STR].colorT[0]);
	DrawFillRect(rect[RECT_DISDATA_XY_STR].xpis, rect[RECT_DISDATA_XY_STR].ypis,
				 rect[RECT_DISDATA_XY_STR].xsize, rect[RECT_DISDATA_XY_STR].ysize, rect[RECT_DISDATA_XY_STR].colorB[0]);
	DrawFS32P(rect[RECT_DISDATA_XY_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_XY_STR].ypis + SAFE_DIST4,
			  g_yabspos, 6, 1, rect[RECT_DISDATA_XY_STR].colorT[0]);
#endif

	spd = g_speed;
	if ((dat == 0xff) || (dat == 0x1e) || (dat == 0x1f))
	{
		feed_f = 2;
	}
	else if ((dat == 0) || (dat == 0x3) || (dat == 0x1b))
	{
		feed_f = 0;
		spd = 0;
	}
	else if (((dat & 0x00ff) == 0x1) || ((dat & 0x00ff) == 0x21) || ((dat & 0x00ff) == 0x41) || ((dat & 0x00ff) == 0x61))
	{
		spd = (dat & 0x00ff) - 1;
		feed_f = 1;
		if (g_feedc < g_feedsum)
		{
			if ((dat & 0xff00) != 0)
			{
				pfoot = (S8)(dat >> 8);
			}
		}
	}
	else
	{
		for (ii = 1; ii < 19; ii++)
		{
			if (dat == pat_code[ii])
				break;
		}
		if (ii < 19)
			feed_f = ii + 2;
		else if (dat == 0x3a)
			feed_f = 25; // 线张力
		else if (dat == 0xf0)
			feed_f = 26; // 中压脚
		else if (dat == 0xf3)
			feed_f = 28; // 暂停开
		else if (dat == 0xfb)
			feed_f = 30; // 减速
		else if (dat == 0xfc)
			feed_f = 31; // 功能X
		else if ((dat & 0xff) == 0x3c)
			feed_f = 33; // 针角度
		else if ((dat & 0xff) == 0x3d)
			feed_f = 34; // 功能一
		else if ((dat & 0xff) == 0x3e)
			feed_f = 35; // 功能二
		else if ((dat & 0xff) == 0xee)
			feed_f = 36; // 速度
		else
			feed_f = 1;

		if ((feed_f == 10) && (g_UdZhouFlag == 3))
		{ //换针
			feed_f = 32;
		}
	}

	if (tmp_feedf != feed_f)
	{
		tmp_feedf = feed_f;
		// DrawFillRect(326, 36, 64, 16, mCOLOR ? Color_Silver : Color_White);
		// Draw16x16Str(326, 36, (padd + ((U32)(TitleLengthMax)) * feed_f), psheet, feed_f, Color_Black);
		DrawFillRect(rect[RECT_DISDATA_CODE_STR].xpis, rect[RECT_DISDATA_CODE_STR].ypis,
					 rect[RECT_DISDATA_CODE_STR].xsize, rect[RECT_DISDATA_CODE_STR].ysize, rect[RECT_DISDATA_CODE_STR].colorB[0]);
		Draw16x16Str(rect[RECT_DISDATA_CODE_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_CODE_STR].ypis + SAFE_DIST4,
					 (padd + ((U32)(TitleLengthMax)) * feed_f), psheet, feed_f, rect[RECT_DISDATA_CODE_STR].colorT[0]);
	}
	if (feed_f == 0)
	{
		// DrawFillRect(536, 56, 32, 16, mCOLOR ? Color_Silver : Color_White);
		// DrawFS32P(536, 56, 120, 4, 1, Color_Black);

		DrawFillRect(rect[RECT_DISDATA_PITCH_STR].xpis, rect[RECT_DISDATA_PITCH_STR].ypis,
					 rect[RECT_DISDATA_PITCH_STR].xsize, rect[RECT_DISDATA_PITCH_STR].ysize, rect[RECT_DISDATA_PITCH_STR].colorB[0]);
		DrawFS32P(rect[RECT_DISDATA_PITCH_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_PITCH_STR].ypis + SAFE_DIST4,
				  g_pitch, 4, 1, rect[RECT_DISDATA_PITCH_STR].colorT[0]);
	}
	else
	{
		// DrawFillRect(536, 56, 32, 16, mCOLOR ? Color_Silver : Color_White);
		// DrawFS32P(536, 56, g_pitch, 4, 1, Color_Black);
		DrawFillRect(rect[RECT_DISDATA_PITCH_STR].xpis, rect[RECT_DISDATA_PITCH_STR].ypis,
					 rect[RECT_DISDATA_PITCH_STR].xsize, rect[RECT_DISDATA_PITCH_STR].ysize, rect[RECT_DISDATA_PITCH_STR].colorB[0]);
		DrawFS32P(rect[RECT_DISDATA_PITCH_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_PITCH_STR].ypis + SAFE_DIST4,
				  g_pitch, 4, 1, rect[RECT_DISDATA_PITCH_STR].colorT[0]);
	}

	if (F_FILETYPE == PAT_SEW)
	{
		spd = (div == 1 ? g_oldspeed : g_speed);
		pfoot = (feed_f == 26 ? g_pf_bdis : 0);
	}

	if (spd == 0x60)
		spd = 21;
	else if (spd == 0x40)
		spd = 22;
	else if (spd == 0x20)
		spd = 23;
	else
		spd = 24;

	if (tmp_spd != spd)
	{
		tmp_spd = spd;
		// DrawFillRect(326, 56, 64, 16, mCOLOR ? Color_Silver : Color_White);
		// Draw16x16Str(326, 56, (padd + ((U32)(TitleLengthMax)) * spd), psheet, spd, Color_Black);

		DrawFillRect(rect[RECT_DISDATA_SP_STR].xpis, rect[RECT_DISDATA_SP_STR].ypis,
					 rect[RECT_DISDATA_SP_STR].xsize, rect[RECT_DISDATA_SP_STR].ysize, rect[RECT_DISDATA_SP_STR].colorB[0]);
		Draw16x16Str(rect[RECT_DISDATA_SP_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_SP_STR].ypis + SAFE_DIST4,
					 (padd + ((U32)(TitleLengthMax)) * spd), psheet, spd, rect[RECT_DISDATA_SP_STR].colorT[0]);
	}
	if (tmp_feedc != g_feedc)
	{
		tmp_feedc = g_feedc;
		// DrawFillRect(488, 36, 48, 16, mCOLOR ? Color_Silver : Color_White);
		// DrawF16Str(528, 36, "/", Color_Red);
		// DrawFU32(488, 36, g_feedc, 5, Color_Red);

		DrawFillRect(rect[RECT_DISDATA_PINS_STR].xpis, rect[RECT_DISDATA_PINS_STR].ypis,
					 SAFE_DIST2 + 9 * 5, rect[RECT_DISDATA_PINS_STR].ysize, rect[RECT_DISDATA_PINS_STR].colorB[0]);
		DrawF16Str(rect[RECT_DISDATA_PINS_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_DISDATA_PINS_STR].ypis + SAFE_DIST4,
				   "/", rect[RECT_DISDATA_PINS_STR].colorT[0]);
		DrawFU32(rect[RECT_DISDATA_PINS_STR].xpis + SAFE_DIST2, rect[RECT_DISDATA_PINS_STR].ypis + SAFE_DIST4,
				 g_feedc, 5, rect[RECT_DISDATA_PINS_STR].colorT[0]);
	}
	if (tmp_feedsum != g_feedsum)
	{
		tmp_feedsum = g_feedsum;
		// DrawFillRect(536, 36, 40, 16, mCOLOR ? Color_Silver : Color_White);
		// DrawFU32(536, 36, g_feedsum, 5, Color_Red);

		DrawFillRect(rect[RECT_DISDATA_PINS_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_DISDATA_PINS_STR].ypis,
					 rect[RECT_DISDATA_PINS_STR].xsize - (SAFE_DIST2 + 9 * 6), rect[RECT_DISDATA_PINS_STR].ysize, rect[RECT_DISDATA_PINS_STR].colorB[0]);
		DrawFU32(rect[RECT_DISDATA_PINS_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_DISDATA_PINS_STR].ypis + SAFE_DIST4,
				 g_feedsum, 5, rect[RECT_DISDATA_PINS_STR].colorT[0]);
	}

	if (PFOOTBT)
	{
		// DrawFillRect(226, 36, 32, 16, mCOLOR ? Color_Silver : Color_White);
		DrawFillRect(rect[RECT_DISDATA_PF_STR].xpis, rect[RECT_DISDATA_PF_STR].ypis,
					 rect[RECT_DISDATA_PF_STR].xsize, rect[RECT_DISDATA_PF_STR].ysize, rect[RECT_DISDATA_PF_STR].colorB[0]);
		if (g_feedc == g_feedsum)
		{ //显示中压脚修正值
			// DrawF16Str(226, 36, g_pf_dis < 0 ? "-" : "+", Color_Black);
			// DrawFS32P(234, 36, abs(g_pf_dis), 3, 1, Color_Black);
			DrawF16Str(rect[RECT_DISDATA_PF_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_PF_STR].ypis + SAFE_DIST4,
					   g_pf_dis < 0 ? "-" : "+", rect[RECT_DISDATA_PF_STR].colorT[0]);
			DrawFS32P(rect[RECT_DISDATA_PF_STR].xpis + SAFE_DIST3 + SAFE_DIST6, rect[RECT_DISDATA_PF_STR].ypis + SAFE_DIST4,
					  abs(g_pf_dis), 3, 1, rect[RECT_DISDATA_PF_STR].colorT[0]);
		}
		else
		{
			// DrawFS32P(226, 36, pfoot, 4, 1, Color_Black);
			DrawFS32P(rect[RECT_DISDATA_PF_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_PF_STR].ypis + SAFE_DIST4,
					  pfoot, 4, 1, rect[RECT_DISDATA_PF_STR].colorT[0]);
		}
		// DrawFillRect(226, 56, 32, 16, mCOLOR ? Color_Silver : Color_White);
		// DrawFS32P(226, 56, g_pf_absdis, 4, 1, Color_Black);
		DrawFillRect(rect[RECT_DISDATA_XPF_STR].xpis, rect[RECT_DISDATA_XPF_STR].ypis,
					 rect[RECT_DISDATA_XPF_STR].xsize, rect[RECT_DISDATA_XPF_STR].ysize, rect[RECT_DISDATA_XPF_STR].colorB[0]);
		DrawFS32P(rect[RECT_DISDATA_XPF_STR].xpis + SAFE_DIST3 + TYPE_SIZE20, rect[RECT_DISDATA_XPF_STR].ypis + SAFE_DIST4,
				  g_pf_absdis, 4, 1, rect[RECT_DISDATA_XPF_STR].colorT[0]);
	}
	if (TENSIONBT)
	{
		// DrawFillRect(446, 56, 24, 16, mCOLOR ? Color_Silver : Color_White);
		// DrawFS32P(446, 56, g_lineforceValSetNow, 3, 0, Color_Black);
		DrawFillRect(rect[RECT_DISDATA_LF_STR].xpis, rect[RECT_DISDATA_LF_STR].ypis,
					 rect[RECT_DISDATA_LF_STR].xsize, rect[RECT_DISDATA_LF_STR].ysize, rect[RECT_DISDATA_LF_STR].colorB[0]);
		DrawFS32P(rect[RECT_DISDATA_LF_STR].xpis + SAFE_DIST3, rect[RECT_DISDATA_LF_STR].ypis + SAFE_DIST4,
				  g_lineforceValSetNow, 3, 0, rect[RECT_DISDATA_LF_STR].colorT[0]);
	}
	DrawFillRect(rect[RECT_DISDATA_P_STR].xpis, rect[RECT_DISDATA_P_STR].ypis,
				 rect[RECT_DISDATA_P_STR].xsize, rect[RECT_DISDATA_P_STR].ysize, rect[RECT_DISDATA_P_STR].colorB[0]);
	DrawFS32P(rect[RECT_DISDATA_P_STR].xpis + SAFE_DIST4, rect[RECT_DISDATA_P_STR].ypis + SAFE_DIST4,
			  g_point_num, 3, 0, rect[RECT_DISDATA_P_STR].colorT[0]);
}

//------------------------------------------
// Function   : U8 CailDate(U8 type,S8 val,S8 year,S8 mon)
// Description: 判断设置的日期是否正确,如正确返回1 ,否则返0
//------------------------------------------
U8 CailDate(U8 type, S8 val, S8 year, S8 mon)
{

	switch (type)
	{
	case 0:
		if (val >= 0 && val <= 99)
			return 1;
		else
			break;
	case 1:
		if (val > 0 && val < 13)
			return 1;
		else
			break;
	case 3:
		if (val >= 0 && val < 24)
			return 1;
		else
			break;
	case 4:
		if (val >= 0 && val < 60)
			return 1;
		else
			break;
	case 2:
		if ((mon == 1) || (mon == 3) || (mon == 5) || (mon == 7) || (mon == 8) || (mon == 10) || (mon == 12))
		{
			if (val > 0 && val < 32)
				return 1;
		}
		else if ((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11))
		{
			if (val > 0 && val < 31)
				return 1;
		}
		else if (mon == 2)
		{
			if ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0)))
			{ //是否是闰年
				if (val > 0 && val < 30)
					return 1;
			}
			else
			{
				if (val > 0 && val < 29)
					return 1;
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

//------------------------------------------
// Function   : U8 WriteTimer(U16 cmd,U8 t[5])
// Description:
//------------------------------------------
U8 WriteTimer(U16 cmd, U8 t[5])
{
	U8 re;
	U8 timer[5];

	for (re = 0; re < 5; re++)
		timer[re] = Hex_2BCD(t[re]);

	DREG[D_Y] = (U16)timer[0];
	DREG[D_MD] = (((U16)timer[1]) << 8) + (U16)timer[2];
	DREG[D_WH] = (U16)(timer[3] | 0x80);
	DREG[D_MS] = (((U16)timer[4]) << 8);
	re = Write_D(D_Address[DISP] + D_MS, 4, (U16 *)(&DREG[D_MS]));

	DREG[D_COMMAND] = cmd;
	DREG[D_CMDSTATUS] = 0;
	DREG[D_CMDPARA1] = 1;
	DREG[D_CMDPARA2] = 2;
	re = Write_D(D_Address[DISP] + D_COMMAND, 4, (U16 *)(&DREG[D_COMMAND]));

	return (re);
}

U32 Cail_Yneedle(S16 angle, U16 maxy, U16 miny)
{
	U32 y;
	if ((angle >= 0) && (angle <= 180))
	{
		y = maxy - (angle >> 1);
	}
	else
	{
		y = (angle >> 1) + miny;
	}
	return y;
}

U8 checkusername(U8 *inputname, U8 len)
{
	U8 i, j;

	if ((len == 0) || (len > 10))
		return (0);
	for (i = 1; i < 10; i++)
	{
		for (j = 0; j < len; j++)
		{
			if ((g_user.users[i].name[j] != inputname[j]) || (g_user.users[i].name[j] == 0))
				break;
		}
		if ((j >= len) && (g_user.users[i].name[len] == 0))
			return (i);
	}
	return (0);
}

//生成授权密码
//U8 *srcword:输入密码
//u8 len：密码长度
//返回值：32为新密码
U32 MakeAutPassword(U8 *srcword, U8 mainBoardType, u8 len)
{
	U8 i, j, k;
	U32 PassWord, PassWord1;
	U8 tmp;
	U8 password[10];

	PassWord = 0;
	for (i = 0; i < len; i++)
	{
		*(password + i) = *(srcword + i);
	}

	for (i = 0; i < len; i++)
	{ //先将输入字符串改为数字
		*(password + i) = *(password + i) - 0x30;
	}
	for (; i < 10; i++)
	{
		*(password + i) = 0;
	}
	for (i = 0; i < 5; i++)
	{ //前后5个字节对换
		tmp = password[i];
		password[i] = password[i + 5];
		password[i + 5] = tmp;
	}

	for (j = 0; j < len; j++)
	{
		if (j < 5)
			k = j + 5;
		else
			k = j - 5;

		if (k != 0)
		{
			password[k] = ZKEY[mainBoardType][password[k]]; //数字查表加密
		}
		PassWord1 = 1;
		for (i = 9; i > k; i--)
		{
			PassWord1 *= 10;
		}
		PassWord += password[k] * PassWord1;
	}

	//	for(;i<10;i++){		//清除密码
	//		*(password+i) = 0;
	//	}

	return PassWord;
}

U8 checkuserpassword(U8 *inputpassword, U8 index, U8 len)
{
	U8 j;

	if ((len == 0) || (len > 10))
		return (0);
	if ((index == 0) || (index > 10))
		return (0);
	for (j = 0; j < len; j++)
	{
		if ((g_user.users[index].password[j] != inputpassword[j]) || (g_user.users[index].password[j] == 0))
			break;
	}
	if ((j >= len) && (g_user.users[index].password[len] == 0))
		return (index);

	return (0);
}

#define RUN_RECORD_MAXLIST 18

//获取指定天的信息
//U32 disDate:指定的第几天
//U8 *Msg:记录信息
//U16 *Index:记录信息索引
//U16 *maxpage:当天最大显示页数
//*startAddr,*endAddr:记录当天信息的起始结束地址
//U16 *date:当天的日期
void getDateRunRecord(U32 disDate, U8 *Msg, U16 *Index, U16 *maxpage, U32 *startAddr, U32 *endAddr, U16 *date)
{
	U32 tStAddr = (disDate - 1) * 4;
	U32 tEndAddr = (disDate)*4;
	U32 tstartAddr = 0, tendAddr = 0;
	U16 records = 0;
	U32 i;
	U16 tdate = 0;
	tstartAddr = (Index[tStAddr + 3] << 16) | Index[tStAddr + 2];
	tendAddr = (Index[tEndAddr + 3] << 16) | Index[tEndAddr + 2];
	tdate = (Msg[tstartAddr + 1] << 8) | Msg[tstartAddr + 2];

	i = tstartAddr;
	wdt();
	while (i < tendAddr)
	{
		if (Msg[i] == 0xa5)
		{ //日期
			i += 3;
		}
		if ((Msg[i] & 0xfc) == 0xa8)
		{ //小时
			i += 3;
		}
		i += 3; //秒 记录一条加工信息
		records++;
	}

	*startAddr = tstartAddr;
	*endAddr = tendAddr;
	*date = tdate;
	*maxpage = records / RUN_RECORD_MAXLIST + (records % RUN_RECORD_MAXLIST ? 1 : 0);
	if (*maxpage == 0)
	{
		(*maxpage) = 1;
	}
}

//根据用户编号获取用户名称
void getUsrName(U8 No, U8 *name)
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		name[i] = g_user.users[No].name[i]; //0x20;
	}
}

//------------------------------------------
// Function   :U8 getPT_ZoomPara(U8 PT_type,U16 *Xscal,U16 *Yscal,S16 *Xoffset,S16 *Yoffset,U32 Patlen)
// Description: 获取图形放大缩小,平移移动参数
//  U8  PT_type:  花样类型
//	U16 Xscal,U16 Yscal: X,Y轴放大比例
//  S16 xmove,S16 ymove: x,y平行移动距离
//	U32 Patlen :花样长度	8b
//	返回值：0-成功, 1-失败(针距超12.7), 2-失败(图形超加工范围)
U8 getPT_ZoomPara(U8 PT_type, S16 *Xscal, S16 *Yscal, S16 *Xoffset, S16 *Yoffset, U32 Patlen)
{
	ISEWADD sewInfo = {0};
	if (PT_type == PAT_MIT)
	{
		*Xscal = 1000;
		*Yscal = 1000;
		*Xoffset = 0;
		*Yoffset = 0;
	}
	else if (PT_type == PAT_SEW)
	{
		Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		*Xscal = (S16)SREG[sewInfo.orgaddr - 3];
		*Yscal = (S16)SREG[sewInfo.orgaddr - 2];
		if (*Xscal < 10)
			*Xscal = 1000;
		if (*Yscal < 10)
			*Yscal = 1000;

		*Xoffset = (S16)SREG[sewInfo.pataddr - 2];
		*Yoffset = (S16)SREG[sewInfo.pataddr - 1];

		*Xoffset /= 2;
		*Yoffset /= 2;
	}
	return 0;
}

//------------------------------------------
// Function   : U8 PT_Zoom(U8 PT_type,U16 Xscal,U16 Yscal,S16 xorgp,S16 yorgp,U32 Patlen)
// Description: 图形放大缩小,平移移动
//  U8  PT_type:  花样类型
//	U16 Xscal,U16 Yscal: X,Y轴放大比例
//	S16 xorgp,S16 yorgp: X,Y中心点
//  S16 xmove,S16 ymove: x,y平行移动距离
//	U32 Patlen :花样长度	8b
//	返回值：0-成功, 1-失败(针距超12.7), 2-失败(图形超加工范围)
U8 PT_Zoom(U8 PT_type, S16 Xscal, S16 Yscal, S16 xorgp, S16 yorgp, S16 xmove, S16 ymove, U32 Patlen)
{
	U32 i, ii, j;
	U32 len1 = 0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	S16 xst = 0, xend = 0, yst = 0, yend = 0;
	S16 dx, dy;
	S16 lastPx, laxtPy, tmpOrgX, tmpOrgY, OrgX, OrgY;
	U8 f;
	U32 offset;
	U16 maxPitch;
	S16 Xposmax, Xposmin, Yposmax, Yposmin;
	U16 checksum;
	S16 oXoff, oYoff;
	for (i = 0; i < MAXSREG; i++)
		NEWSREG[i] = 0;

	wdt();

	//#if Debug
	//	printf("PT_Zoom(%d, %d, %d, %d, %d, %d, %d, %d) \r\n",PT_type,Xscal,Yscal,xorgp,yorgp,xmove,ymove,Patlen);
	//#endif
	xmove *= 2;
	ymove *= 2;
	if (PT_type == PAT_MIT)
	{
		Mit_To_Bor(&Patlen); //转成SEW格式
		F_FILETYPE = PAT_SEW;
		PT_type = PAT_SEW;
	}
	if (PT_type == PAT_SEW)
	{
		Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);

		OrgX = ((SREG[sewInfo.orgaddr] & 0xff) << 8) | ((SREG[sewInfo.orgaddr] >> 8 & 0xff));
		OrgY = ((SREG[sewInfo.orgaddr + 1] & 0xff) << 8) | ((SREG[sewInfo.orgaddr + 1] >> 8 & 0xff));

		for (i = 0; i < sewInfo.pataddr; i++)
			NEWSREG[i] = SREG[i];
		offset = i;
		wdt();

		xst = OrgX;
		yst = OrgY;
		xend = OrgX;
		yend = OrgY;
		tmpOrgX = OrgX;
		tmpOrgY = OrgY;

		tmpOrgX -= xorgp;
		tmpOrgY -= yorgp;
		tmpOrgX = tmpOrgX * ((S32)Xscal) / 1000;
		tmpOrgY = tmpOrgY * ((S32)Yscal) / 1000;
		tmpOrgX += xorgp;
		tmpOrgY += yorgp;

		lastPx = tmpOrgX;
		laxtPy = tmpOrgY;

		SREG[sewInfo.orgaddr] = (((tmpOrgX)&0xff) << 8) | (((tmpOrgX) >> 8) & 0xff);
		SREG[sewInfo.orgaddr + 1] = (((tmpOrgY)&0xff) << 8) | (((tmpOrgY) >> 8) & 0xff);

		i = sewInfo.pataddr;
		for (j = 0; j < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + j], &sewIcmd);
			if (f)
			{
				if ((sewIcmd.cmd & 0xf000) == 0x2000)
				{ //空送
					xend = xst + sewIcmd.dx;
					yend = yst + sewIcmd.dy;

					xst = xend;
					yst = yend;

					xend -= xorgp;
					yend -= yorgp;

					xend = xend * ((S32)Xscal) / 1000;
					yend = yend * ((S32)Yscal) / 1000;

					xend += xorgp;
					yend += yorgp;

					dx = xend - lastPx;
					dy = yend - laxtPy;

					if ((dx >= 0) && (dy >= 0))
					{ //X+    Y+
						NEWSREG[offset++] = (((dx >> 8) & 0xff) << 8) | 0x22;
						NEWSREG[offset++] = ((dx & 0xff) << 8) | 0x00;
						NEWSREG[offset++] = (((dy >> 8) & 0xff) << 8) | 0x0A;
						NEWSREG[offset++] = ((dy & 0xff) << 8) | 0x08;
					}
					else if ((dx >= 0) && (dy < 0))
					{ //X+    Y-
						NEWSREG[offset++] = (((dx >> 8) & 0xff) << 8) | 0x22;
						NEWSREG[offset++] = ((dx & 0xff) << 8) | 0x00;
						NEWSREG[offset++] = ((((-dy) >> 8) & 0xff) << 8) | 0x0E;
						NEWSREG[offset++] = (((-dy) & 0xff) << 8) | 0x0C;
					}
					else if ((dx < 0) && (dy >= 0))
					{ //X-    Y+
						NEWSREG[offset++] = ((((-dx) >> 8) & 0xff) << 8) | 0x26;
						NEWSREG[offset++] = (((-dx) & 0xff) << 8) | 0x04;
						NEWSREG[offset++] = (((dy >> 8) & 0xff) << 8) | 0x0A;
						NEWSREG[offset++] = ((dy & 0xff) << 8) | 0x08;
					}
					else if ((dx < 0) && (dy < 0))
					{ //X-    Y-
						NEWSREG[offset++] = ((((-dx) >> 8) & 0xff) << 8) | 0x26;
						NEWSREG[offset++] = (((-dx) & 0xff) << 8) | 0x04;
						NEWSREG[offset++] = ((((-dy) >> 8) & 0xff) << 8) | 0x0E;
						NEWSREG[offset++] = (((-dy) & 0xff) << 8) | 0x0C;
					}

					lastPx = xend;
					laxtPy = yend;
				}
				else if ((sewIcmd.cmd & 0xf000) == 0x6000)
				{ //缝纫
					xend = xst + sewIcmd.dx;
					yend = yst + sewIcmd.dy;

					xst = xend;
					yst = yend;

					xend -= xorgp;
					yend -= yorgp;
					xend = xend * ((S32)Xscal) / 1000;
					yend = yend * ((S32)Yscal) / 1000;
					xend += xorgp;
					yend += yorgp;

					dx = xend - lastPx;
					dy = yend - laxtPy;

					if ((dx >= 0) && (dy >= 0))
					{ //X+    Y+
						NEWSREG[offset++] = (dx << 8) | 0x60;
						NEWSREG[offset++] = (dy << 8) | 0x48;
					}
					else if ((dx >= 0) && (dy < 0))
					{ //X+    Y-
						NEWSREG[offset++] = (dx << 8) | 0x60;
						NEWSREG[offset++] = ((-dy) << 8) | 0x4c;
					}
					else if ((dx < 0) && (dy >= 0))
					{ //X-    Y+
						NEWSREG[offset++] = ((-dx) << 8) | 0x64;
						NEWSREG[offset++] = (dy << 8) | 0x48;
					}
					else if ((dx < 0) && (dy < 0))
					{ //X-    Y-
						NEWSREG[offset++] = ((-dx) << 8) | 0x64;
						NEWSREG[offset++] = ((-dy) << 8) | 0x4c;
					}
					lastPx = xend;
					laxtPy = yend;
				}
				else
				{
					for (ii = 0; ii < sewIcmd.off; ii++)
					{
						NEWSREG[offset++] = SREG[i + j + ii];
					}
				}
				j += sewIcmd.off;
			}
			else
				j++;
		}

		wdt();

		len1 = (offset - sewInfo.pataddr) * 2;

		for (i = sewInfo.pataddr + sewInfo.ptsize / 2; i < (Patlen / 2); i++)
		{
			NEWSREG[offset++] = SREG[i];
		}
		wdt();
		SREG[sewInfo.orgaddr] = (((OrgX)&0xff) << 8) | (((OrgX) >> 8) & 0xff);
		SREG[sewInfo.orgaddr + 1] = (((OrgY)&0xff) << 8) | (((OrgY) >> 8) & 0xff);

		Bor_AnaInfo((U16 *)NEWSREG, offset * 2, &sewInfo);

		oXoff = (S16)SREG[sewInfo.pataddr - 2];
		oYoff = (S16)SREG[sewInfo.pataddr - 1];
		tmpOrgX += (xmove - oXoff);
		tmpOrgY += (ymove - oYoff);

		NEWSREG[sewInfo.orgaddr] = (((tmpOrgX)&0xff) << 8) | (((tmpOrgX) >> 8) & 0xff);
		NEWSREG[sewInfo.orgaddr + 1] = (((tmpOrgY)&0xff) << 8) | (((tmpOrgY) >> 8) & 0xff);

		NEWSREG[sewInfo.lenaddr] = (((len1 >> 16) & 0xff) << 8) | ((len1 >> 24) & 0xff);
		NEWSREG[sewInfo.lenaddr + 1] = (((len1 >> 0) & 0xff) << 8) | ((len1 >> 8) & 0xff);

		NEWSREG[sewInfo.orgaddr - 3] = Xscal;
		NEWSREG[sewInfo.orgaddr - 2] = Yscal;

		NEWSREG[sewInfo.pataddr - 2] = xmove;
		NEWSREG[sewInfo.pataddr - 1] = ymove;

		//判断花样
		maxPitch = get_MaxPitch(PT_type, (U16 *)NEWSREG, offset * 2);
		if (maxPitch > Pindismax * Pindismax)
			return 1;

		Bor_CalPatLim((U16 *)NEWSREG, offset * 2, &Xposmax, &Xposmin, &Yposmax, &Yposmin, &g_patd.feed_sum);

		//printf("Xposmax = %d  Xposmin = %d Yposmax = %d Yposmin = %d offset = %d \r\n",Xposmax,Xposmin,Yposmax,Yposmin,offset);
		//printf("g_Mac.max_xr = %d  g_Mac.max_xl = %d g_Mac.max_yu = %d g_Mac.max_yd = %d \r\n",g_Mac.max_xr,g_Mac.max_xl,g_Mac.max_yu,g_Mac.max_yd);
		if ((Xposmax > 2 * (g_Mac.max_xr + 1)) || (Xposmin < 2 * (g_Mac.max_xl - 1)))
			return 2;
		if ((Yposmax > 2 * (g_Mac.max_yu + 1)) || (Yposmin < 2 * (g_Mac.max_yd - 1)))
			return 2;

		g_patd.orgX = tmpOrgX;
		g_patd.orgY = tmpOrgY;
		g_patd.patmX = Xposmax;
		g_patd.patmX = Xposmin;
		g_patd.patmY = Yposmax;
		g_patd.patiY = Yposmin;

		for (i = 0; i < offset; i++)
		{
			SREG[i] = NEWSREG[i];
		}
		for (; i < MAXSREG; i++)
			SREG[i] = 0;

		checksum = 0;
		for (i = 0; i < offset; i++)
		{
			if (i != 5)
				checksum += SREG[i];
		}
		SREG[5] = checksum;

		HREG[4] = (U16)((offset * 2) & 0xffff); //文件长度
		HREG[5] = (U16)(((offset * 2) >> 16) & 0xffff);
		g_patd.pat_len = offset * 2;
	}
	return 0;
}

//------------------------------------------
// Function   : void get_ScaleCenter(U8 PT_Type,U8 mode,S16 *xCenter,S16 *yCenter)
// Description: 获取图形缩放中心
//  U8  PT_type:  花样类型
//	S16 *xCenter,S16 *yCenter: X,Y中心点
// mode:获取模式
//	U32 Patlen :花样长度	8b
//	返回值：0-成功, 1-失败(针距超12.7), 2-失败(图形超加工范围)
void get_ScaleCenter(U8 PT_type, U8 mode, S16 *xCenter, S16 *yCenter, U32 Patlen)
{
	S16 Xposmax, Xposmin, Yposmax, Yposmin;
	if (PT_type == PAT_SEW)
	{
		Bor_CalPatLim1((U16 *)SREG, Patlen, &Xposmax, &Xposmin, &Yposmax, &Yposmin);
	}
	else if (PT_type == PAT_MIT)
	{
		Xposmax = g_patd.patmX;
		Xposmin = g_patd.patiX;
		Yposmax = g_patd.patmY;
		Yposmin = g_patd.patiY;
	}
	if (mode == 0 || mode == 1 || mode == 2)
	{ //图案中心,图案中心前,图案中心后设置为机械原点
		*xCenter = 0;
		*yCenter = 0;
	}
	else if (mode == 3)
	{ //缝纫区域左后角
		*xCenter = Xposmin;
		*yCenter = Yposmin;
	}
	else if (mode == 4)
	{ //缝纫区域左前角
		*xCenter = Xposmin;
		*yCenter = Yposmax;
	}
	else if (mode == 5)
	{ //缝纫区域右后角
		*xCenter = Xposmax;
		*yCenter = Yposmin;
	}
	else if (mode == 6)
	{ //缝纫区域右前角
		*xCenter = Xposmax;
		*yCenter = Yposmax;
	}
	else if (mode == 7)
	{ //缝纫区域中心
		*xCenter = (Xposmax + Xposmin) / 2;
		*yCenter = (Yposmax + Yposmin) / 2;
	}
	else if (mode == 8)
	{ //缝纫区域开始点
		*xCenter = g_patd.orgX;
		*yCenter = g_patd.orgY;
	}
}
//------------------------------------------
// Function   : U16 get_MaxPitch(U8 PT_Type,U16 *reg,U32 Patlen)
// Description: 获取花样中最大针距
//  U8  PT_type:  花样类型
//	U32 Patlen :花样长度	8b
//	返回值：最大针距平方值
U16 get_MaxPitch(U8 PT_type, U16 *reg, U32 Patlen)
{
	U32 i, j;
	//	S16 xst = 0,xend = 0,yst = 0,yend = 0;
	U16 pitch = 0, tpitch = 0;
	U8 f;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};

	if (PT_type == PAT_SEW)
	{
		Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		i = sewInfo.pataddr;
		for (j = 0; j < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + j], &sewIcmd);
			if (f)
			{
				if ((sewIcmd.cmd & 0xf000) == 0x6000)
				{ //缝纫
					tpitch = sewIcmd.dx * sewIcmd.dx + sewIcmd.dy * sewIcmd.dy;
					if (tpitch > pitch)
						pitch = tpitch;
				}
				j += sewIcmd.off;
			}
			else
				j++;
		}
		wdt();
	}

	return pitch;
}

//计算组合图形保存编号
//U8 *NO:编号值
//U8 Type:类型 1-组合  2-结合
void CalGroSaveNo(U8 *NO, U8 Type)
{
	U8 i;
	U16 baddr = (Type == 1) ? GroupComAddr : GroupConAddr;
	*NO = 1;
	for (i = 0; i < GroupMax; i++)
	{
		if ((GREG[baddr + i * GroupPatMax + GroupPNoAddr] >= MINFILENO) && (GREG[baddr + i * GroupPatMax + GroupPNoAddr] <= MAXFILENO))
		{
			(*NO)++;
		}
		else
		{
			break;
		}
	}
}

//----------------------------------
// Function   : void getFeedPos(U16 inFeed,U16 inType,S16 *outx,S16 *outy)
// Description: 获取指定针的实际xy坐标
void getFeedPos(U16 inFeed, U16 inType, S16 *outx, S16 *outy)
{
	U32 i = 0, j = 0;
	U8 ch, f;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	S16 x = 0, y = 0;

	*outx = 0;
	*outy = 0;
	if (inType == PAT_MIT)
	{	
		x = 0;
		y = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			x = (S16)(SREG[6]);
			y = -(S16)(SREG[7]);
		}
#endif
		*outx = x;
		*outy = y;
		if (inFeed)
		{
			j = g_patd.pat_add + 1;
			for (i = 0; i < inFeed; i++)
			{
				if ((SREG[j - 1] != 0x3a) && ((SREG[j - 1] & 0xff) != 0x1c) && ((SREG[j - 1] & 0xff) != 0x3c))
				{
					ch = (U8)(SREG[j] & 0xff);
					if (ch >= 0x80)
					{
						ch -= 0x80;
						x -= ch;
					}
					else
					{
						x += ch;
					}
					ch = (SREG[j] >> 8) & 0xff;
					if (ch >= 0x80)
					{
						ch -= 0x80;
						y += ch;
					}
					else
					{
						y -= ch;
					}
				}
				j += 2;
			}
			*outx = x;
			*outy = y;
		}
	}
	else if (inType == PAT_SEW)
	{
		f = Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);
		if (f == 0)
			return;
		x = g_patd.orgX;
		y = g_patd.orgY;

		if (inFeed)
		{
			for (i = 0; (i < (sewInfo.ptsize / 2)) && (j < inFeed);)
			{
				f = Bor_DrawCmd((U16 *)(&SREG[i + sewInfo.pataddr]), &sewIcmd);
				if (f)
				{
					if (sewIcmd.cmd == 0xff00)
					{
						break;
					}
					j++;
					if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
					{
						x += sewIcmd.dx;
						y += sewIcmd.dy;
					}
					i += sewIcmd.off;
				}
				else
					i++;
			}
			*outx = x;
			*outy = y;

			//printf("inFeed = %d x = %d y = %d \r\n",inFeed,x,y);
		}
		else if (g_BorSTflag)
		{
			*outx = g_patd.orgX;
			*outy = g_patd.orgY;
		}
		//printf("inFeed = %d x = %d y = %d \r\n",inFeed,*outx,*outy);
	}
	else if (inType == PAT_EMB)
	{
	}

	wdt();
}

//----------------------------------
// Function   : void viewFeedCode(U16 inX,U16 inY,U16 inFeed,U16 inType)
// Description: 指定位置显示指定针的代码指令
// text:0:24*24字符，其他:16*16字符
//----------------------------------
void viewFeedCode(U16 inX, U16 inY, U16 inFeed, U16 inType, U8 f, U8 text)
{
	U32 j;
	U8 ii, x, y;
	U8 dat, coden = 0;
	U16 cname;
	U32 i;
	static U16 tmpfeed = 0;
	static U32 pataddr = 0;
	static ISEWADD sewInfo = {0};
	static ISEWCMD sewIcmd = {0};
	U8 res = 1;
	U16 tmpS_2HOME;
	S32 dlen = 0;

	tmpS_2HOME = (U16)(S_2HOME);

	if (P_LAN)
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME_E;
	}
	else
	{
		g_CodeNameAdd = MEM_TITLE_CODENAME;
	}
	g_dxpos = 0;
	g_dypos = 0;
	if (inType == PAT_MIT)
	{
		j = g_patd.pat_add + 2 * inFeed;
		cname = SREG[j];
		dat = cname;
		if ((dat == 0xff) || (dat == 0x1e) || (dat == 0x1f))
		{
			coden = 2;
		}
		else if (dat == 0)
		{
			coden = 2; //feed_flag;
		}
		else if (dat == 0x3)
		{
			coden = 0;
			cname = SREG[j + 1];
			x = cname & 0xff;
			y = (cname >> 8) & 0xff;
			if (x >= 0x80)
			{
				x -= 0x80;
				g_dxpos = -x;
			}
			else
			{
				g_dxpos = x;
			}
			if (y >= 0x80)
			{
				y -= 0x80;
				g_dypos = -y;
			}
			else
			{
				g_dypos = y;
			}
		}
		else if (dat == 0x1b)
		{
			coden = 16;
			cname = SREG[j + 1];
			x = cname & 0xff;
			y = (cname >> 8) & 0xff;
			if (x >= 0x80)
			{
				x -= 0x80;
				g_dxpos = -x;
			}
			else
			{
				g_dxpos = x;
			}
			if (y >= 0x80)
			{
				y -= 0x80;
				g_dypos = -y;
			}
			else
			{
				g_dypos = y;
			}
		}
		else if (((dat & 0x00ff) == 0x1) || ((dat & 0x00ff) == 0x21) || ((dat & 0x00ff) == 0x41) || ((dat & 0x00ff) == 0x61))
		{
			coden = 1;
			cname = SREG[j + 1];
			x = cname & 0xff;
			y = (cname >> 8) & 0xff;
			if (x >= 0x80)
			{
				x -= 0x80;
				g_dxpos = -x;
			}
			else
			{
				g_dxpos = x;
			}
			if (y >= 0x80)
			{
				y -= 0x80;
				g_dypos = -y;
			}
			else
			{
				g_dypos = y;
			}
		}
		else if ((dat & 0x00ff) == 0x3c)
		{
			coden = 33; //针跟随
		}
		else if ((dat & 0x00ff) == 0x3d)
		{
			coden = 34; //参数一
		}
		else if ((dat & 0x00ff) == 0x3e)
		{
			coden = 35; //参数二
		}
		else
		{
			if (dat == 0x1d || dat == 0x1c)
			{ //功能5,6,7,8
				for (ii = 1; ii < 19; ii++)
				{
					if (cname == pat_code[ii])
						break;
				}
			}
			else
			{
				for (ii = 1; ii < 19; ii++)
				{
					if (dat == pat_code[ii])
						break;
				}
			}
			if ((g_UdZhouFlag == 3) && (cname == 0x31d))
				coden = 32; //换针
			else if (ii < 19)
				coden = ii + 2;
			else if (dat == 0x3a)
				coden = 25;
			else
				coden = 1;
		}
	}
	else if (inType == PAT_SEW)
	{

		if ((SREG[0] != 0x4942) || (SREG[1] != 0x004C) || (SREG[2] != 0x1323) || (SREG[3] != 0xC800))
		{
			res = 0;
		}
		if (res)
		{
			if (inFeed == 0 || f)
			{
				res = Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);
				sewIcmd.off = 0;
				tmpfeed = 0;
				pataddr = sewInfo.pataddr;
			}
		}

		if (res)
		{
			//printf("tmpfeed = %d  feedc = %d patstaddr = %d \r\n",tmpfeed,feedc,patstaddr);
			if (tmpfeed > inFeed)
			{
				i = 1;
				tmpfeed--;
				sewInfo.pataddr -= sewIcmd.off;
				while (sewInfo.pataddr - i > 0)
				{
					res = Bor_DrawCmd((U16 *)&SREG[sewInfo.pataddr - i], &sewIcmd);
					if (res)
					{
						g_dxpos = sewIcmd.dx / 2;
						g_dypos = sewIcmd.dy / 2;
						coden = borGetDiscode(sewIcmd.cmd);
						break;
					}
					else
						i++;
				}
			}
			else
			{
				dlen = sewInfo.pataddr - pataddr;
				if (dlen > 0)
					dlen *= 2;
				else
					dlen = 0;
				for (i = 0; (i < ((sewInfo.ptsize - dlen) / 2)) && tmpfeed <= inFeed;)
				{
					res = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
					if (res)
					{
						g_dxpos = sewIcmd.dx / 2;
						g_dypos = sewIcmd.dy / 2;

						coden = borGetDiscode(sewIcmd.cmd);
						i += sewIcmd.off;
						if (tmpfeed == inFeed)
						{
							sewInfo.pataddr += i;
						}
						tmpfeed++;
					}
					else
						i++;
					//
				}
				if (sewInfo.ptsize == dlen)
				{
					coden = 2; //针结束
				}
				//printf("i =  %d %d-%d = %d \r\n",i,len,dlen,len-dlen);
				//			tmpfeed = feedc;
			}
		}

		if ((tmpS_2HOME != 0x2000) && (inFeed == 0) && ((g_patd.orgX != 0) || (g_patd.orgY != 0)))
		{
			g_dxpos = g_patd.orgX / 2;
			g_dypos = g_patd.orgY / 2;
			coden = 0;
		}
	}
	else if (inType == PAT_EMB)
	{
	}

	// chCodeNow = coden;

	if (text == 0)
		Draw24X24FPY(inX, inY, (char *)(g_CodeNameAdd + ((U32)(TitleLengthMax)) * coden), 8, Color_Black);
	//Draw24X24FFStr(inX, inY,(U16 *)(g_CodeNameAdd+((U32)(TitleLengthMax))*coden),Color_Black);
	else
	{
		DrawFFStrN(inX + (64 - strlen((char *)(g_CodeNameAdd + ((U32)(TitleLengthMax)) * coden)) * 8) / 2, inY, (U16 *)(g_CodeNameAdd + ((U32)(TitleLengthMax)) * coden), 8, text == 2 ? Color_White : Color_Black);
	}
}

/*-----------------------------------------------
//函数名:U16 get_ExitPatNO(U16 *reg,U8 flag)
//功能  :获取已有的花样编号
//------------------------------------------*/
U16 get_ExitPatNO(U16 *reg, U8 flag)
{
	U16 i;
	U16 no = 0;
	U32 len;

	for (i = 0; i < MAXIREG2; i += FileIndexSize)
	{
		len = (((U32)(reg[i + 1]) << 16) + reg[i]) / 2;
		if (len > 0 && len <= MAXSREG)
		{
			no = i / FileIndexSize + MINFILENO;
			if (flag == 0)
				break; //查找到最小编号
		}
	}

	if (no < MINFILENO || no > MAXFILENO)
		no = 0;

	return no;
}

/*-----------------------------------------------
//函数名:U16 get_EmptyPatNo(U16 *reg,U16 sno)
//功能  :从指定编号开始查找空闲的花样编号
//------------------------------------------*/
U16 get_EmptyPatNo(U16 *reg, U16 sno)
{
	U32 i, endaddr;
	U16 outno;
	if (sno < MINFILENO || sno > MAXFILENO)
	{
		sno = MINFILENO;
	}
	outno = sno;
	i = (sno - MINFILENO) * FileIndexSize;

	for (; i < MAXIREG2; i += FileIndexSize)
	{
		if ((reg[i] == 0xffff) && (reg[i + 1] == 0xffff))
			break;
	}
	wdt();

	if (i >= MAXIREG2)
	{
		i = (2 - MINFILENO) * FileIndexSize;
		endaddr = (sno - MINFILENO) * FileIndexSize;

		for (; i < endaddr; i += FileIndexSize)
		{
			if ((reg[i] == 0xffff) && (reg[i + 1] == 0xffff))
				break;
		}
		if (i >= endaddr)
			i = MAXIREG2;
	}
	if (i >= MAXIREG2)
		i = FileIndexSize;
	outno = i / FileIndexSize + MINFILENO;

	return outno;
}

/*-----------------------------------------------
//函数名:U16 get_ExitPatNO(U16 *reg,U8 flag)
//功能  :获取已有的花样编号
//------------------------------------------*/
U8 myStrCompare(char *str1, char *str2, U8 ignoreCase)
{
	U16 len1 = getStrLen((U8 *)str1);
	U16 len2 = getStrLen((U8 *)str2);
	U16 i = 0;
	if (len1 != len2)
		return 0;

	if (ignoreCase)
	{ //忽略大小写
		while (i < len1)
		{
			if (*(str1 + i) != *(str2 + i))
			{
				if (*(str1 + i) > *(str2 + i))
				{
					if (*(str1 + i) - 32 != *(str2 + i))
						return 0;
				}
				else
				{
					if (*(str1 + i) + 32 != *(str2 + i))
						return 0;
				}
			}
			i++;
		}
	}
	else
	{
		while (i < len1)
		{
			if (*(str1 + i) != *(str2 + i))
				return 0;
			i++;
		}
	}

	wdt();
	return 1;
}
