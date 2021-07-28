//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  register.h								*
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


#ifndef __REGISTER_H 
#define __REGISTER_H


#include "stm32cpu.h"

//---------------------------------------------------   
//	通用寄存器空间分配 D_REG U16
//---------------------------------------------------

//#define 	DREG_PMDX			0			// 64      0 - 63  X轴驱动交换数据用		1 
//#define 	DREG_PMDY			64			// 64     64 - 127 X轴驱动交换数据用		2
//#define 	DREG_PMDZ			128			// 64    128 - 191 CPUH交换数据用			3 
//#define 	DREG_PMDU			192			// 64    192 - 255 CPUA交换数据用			4
//#define 	DREG_PMDV			256			// 64    256 - 319 CPUH交换数据用			5 
//#define 	DREG_PMDW			320			// 64    320 - 383 CPUA交换数据用			6
//#define 	DREG_SVNM			384			// 128   384 - 511 CPUB交换数据用			7
//#define 	DREG_VIDEO			512			// 128   512 - 639 视觉处理交换数据用		8
//#define 	DREG_DISP			640			// 64    640 - 703 打版器交换数据用			9
//#define 	DREG_NETA			704			// 64    704 - 767 联网接口A交换数据用		10
//#define 	DREG_NETB			768			// 64    768 - 831 联网接口B交换数据用		11
//#define 	DREG_PBUF			832			// 1024  832 - 1855 公共数据交换用			12
//#define 	DREG_ERROR			1856		// 256  1856 - 2111 ERROR数据用				13
//#define 	DREG_MSSG			2112		// 192  2112 - 2303 系统运行信息记录		14
//#define 	DREG_SYS			2304		// 192  2304 - 2495 系统数据用				15 
//#define 	DREG_PARA			2496		// 1024 2496 - 3519 参数用					16
//#define 	DREG_PT_INDEX		3520		// 3200 3520 - 6719 320*10花样文件列表		17
//#define 	DREG_PT_HEAD		6720		// 64   6720 - 6783 当前花样文件的信息		18
//#define 	DREG_PT_DATA		6784		// 92072 6784 - 98855 花样数据				19 		  SRAM=256K(98856=96.6k)
//#define 	DREG_PT_DATA		6784		// 140072 6784 - 146855 花样数据SRAM=256K(146856=143.4k)
//#define 	DREG_BUFFLEN		98856		// 
//#define 	DREG_PT_DATA2		98856		//
//
//#define 	DREG_USERPW			98856		// 192 98856 - 99047 用户名及密码
//#define 	DREG_IOSETP			99048		// 192 99048 - 99239 IO设置
//#define 	DREG_SRVPARA		99240		// 256 99240 - 99495 伺服参数
//#define 	DREG_PT_INDEX2		99496		// 42000 99496 - 141496 花样文件列表新 42*1000	 (用300：12600)
//
//

		
#define 	MAXEEPREG			50			//
#define 	MAXDREG				64			// D内存最大寄存器	状态数据+文件名数据		9
#define 	MAXEREG				256			// E内存最大寄存器	错误信息				13
#define 	MAXMREG				192			// M内存最大寄存器　系统运行信息数据		14
#define 	MAXYREG				192			// Y内存最大寄存器	系统信息				15
#define 	MAXPREG				1024		// P内存最大寄存器	参数数据				16
//#define 	MAXIREG				3200		// I内存最大寄存器	花样文件索引表			17
#define 	MAXHREG				64			// H内存最大寄存器	当前花样文件信息		18
#define 	MAXSREG				128000		// S内存最大寄存器	花样数据				19
#define  	MAXDIR				3200		// 文件目录最大个数
#define 	MAXOREG				64			// O内存最大寄存器	操作面板交换数据
#define 	MAXWREG				1024		// W内存最大寄存器 ip地址数据和主机名称
#define 	MAXIOREG			384			// IO设置寄存器 I:3*12	O:11*12  168+168 ->384	
#define 	MAXTSREG			40000		// TMP_SREG
#define 	MAXTIAPBUFF			1024		// IAPBUFF  81920
#define		MAXIREG2			999*36/*42000*/		// I2内存最大寄存器	花样文件索引表新        
#define   	MAXSREG2			128000		//128000->92072(主板花样大小92072,打板器暂时不改,EMB文件需要较大)
#define  	MAXCOORD			20000		  //花样坐标暂存
#define 	MAXUREG				192			//用户信息
#define 	MAXVREG				640			//伺服参数
#define 	MAXGREG				1280		//组合图形数据
#define 	MAXLREG				24			//电子夹线器数据

#define 	ADDEEPREG	0xc1000000				//
#define 	ADDDREG		ADDEEPREG+(MAXEEPREG*2)	// D内存最大寄存器	状态数据+文件名数据		9
#define 	ADDEREG		ADDDREG+(MAXDREG*2)		// E内存最大寄存器	错误信息				13
#define 	ADDMREG		ADDEREG+(MAXEREG*2)		// M内存最大寄存器　系统运行信息数据		14
#define 	ADDYREG		ADDMREG+(MAXMREG*2)		// Y内存最大寄存器	系统信息				15
#define 	ADDPREG		ADDYREG+(MAXYREG*2)		// P内存最大寄存器	参数数据				16
//#define 	ADDIREG		ADDPREG+(MAXPREG*2)		// I内存最大寄存器	花样文件索引表			17
#define 	ADDHREG		ADDPREG+(MAXPREG*2)		// H内存最大寄存器	当前花样文件信息		18
#define 	ADDSREG		ADDHREG+(MAXHREG*2)		// S内存最大寄存器	花样数据				19
#define 	ADDIOREG	ADDSREG+(MAXSREG*2)		// IO内存最大寄存器	IO设置寄存器	   
#define 	ADDTSREG	ADDIOREG+((MAXIOREG)*2)	// TMP_SREG
#define 	ADDIAPBUFF	ADDTSREG+((MAXTSREG)*2)	// IAPBUFF
#define 	ADDIREG2	ADDIAPBUFF+((MAXTIAPBUFF)*2)	//I内存最大寄存器	花样文件索引表
//#define		ADDDIR_DREG2   ADDIREG2+((MAXIREG2)*2)	 	// 文件目录2
#define 	ADDNEWSREG	 ADDIREG2+((MAXIREG2)*2)//ADDDIR_DREG2+((MAXDIR2)*2)
#define		ADDUREG		ADDNEWSREG+(MAXSREG2*2)
#define		ADDVREG		ADDUREG+(MAXUREG*2)
#define		ADDGREG		ADDVREG+(MAXVREG*2)
#define		ADDLREG		ADDGREG+(MAXGREG*2)
#define		ADDPATCRRRD	 ADDLREG+(MAXLREG*2)
#define		ADDPATCRRRD1	ADDPATCRRRD+(MAXCOORD*4)
#define		ADDRINFPOINT	ADDPATCRRRD1+(MAXCOORD*4)

//#define		ADDUREG		ADDRINFPOINT+(MAXCOORD*2)
//#define		ADDVREG		ADDUREG+(MAXUREG*2)
//#define		ADDGREG		ADDVREG+(MAXVREG*2)

//备份数据存在 blank1

#define 	AVI_VIDEO_BUF_SIZE    1024*60		//定义avi解码时,视频buf大小.
#define 	AVI_AUDIO_BUF_SIZE    12		//定义avi解码时,音频buf大小.
#define		MAXBLKSREG		128000		//备份花样数据寄存器
#define		MAXBLKHREG		64			//备份花样文件信息
#define		MAXBLKIREG		MAXIREG2		//备份花样文件索引信息 8C7C
#define 	BLKINDSIZE		6			//索引信息大小
#define 	MAXBLIND		512
#define   	MAXBLKBUF		0x3B8984	//备份数据缓冲  0x3C1600
							
#define		ADDAVIVIDEO		0xc0800000	//备份花样数据
#define		ADDAVIAUDIO		ADDAVIVIDEO+(AVI_VIDEO_BUF_SIZE)		
#define		ADDBLKSREG		ADDAVIAUDIO+(AVI_AUDIO_BUF_SIZE)
#define		ADDBLKHREG		ADDBLKSREG+(MAXBLKSREG*2)		//备份花样数据
#define		ADDBLKIREG		ADDBLKHREG+(MAXBLKHREG*2)		//备份花样数据
	
#define 	ADDBLKINDEX		ADDBLKIREG+(MAXBLKIREG*2)							//备份数据索引表地址  
#define		ADDBLKBUF	 	ADDBLKINDEX+(MAXBLIND*2)		//备份数据缓冲


#define		SST_SREG_BACKUP			ADDBLKBUF 		//SST上暂存修改花样的针数据，在覆盖原备份花样数据前使用
#define		SST_SREG_PARA_BACKUP	ADDBLKBUF+0xD0000

#define		SST_SREG_SADD			ADDBLKBUF+0x100000		// SST上用于修改花样时备份数据的内存空间启始地址
#define		SST_SREG_EADD			SST_SREG_SADD+0xEFFFF		// SST上用于修改花样时备份数据的内存空间结束地址(960K)
#define		SST_SREG_MAXS			0x00078000


#define		ADDWAVE0			0xc0800000	//波形显示点数据备份地址	
#define		MAXWAVE0_X			0x3CA50
#define		MAXWAVE0_Y			0x3CA50
#define		ADDWAVE0_X			ADDWAVE0
#define		ADDWAVE0_Y			ADDWAVE0+MAXWAVE0_X


#define 	ADDWAVE1			ADDWAVE0+MAXWAVE0_X+MAXWAVE0_Y	//波形数据
#define 	MAXWAVE1			0x400000-MAXWAVE0_X-MAXWAVE0_Y

//#define		ADDWAVE0			0xc0800000	//波形显示点数据备份地址


//#define 	ADDWAVE1			0xc0800000	//波形数据
//#define 	MAXWAVE1			0x400000

//#define		SST_SREG_SADD			0xC0900000		// SST上用于修改花样时备份数据的内存空间启始地址
//#define		SST_SREG_EADD			0xC09EFFFF		// SST上用于修改花样时备份数据的内存空间结束地址(960K)
//#define		SST_SREG_MAXS			0x00078000


//#define		SST_SREG_BACKUP			0xC0800000 		//SST上暂存修改花样的针数据，在覆盖原备份花样数据前使用
//#define		SST_SREG_PARA_BACKUP	0xC08D0000


/******************* 3、寄存器读写模块序号 **************************/
#define		PMDX				1			// X轴驱动交换数据用
#define 	PMDY				2			// Y轴驱动交换数据用
#define 	PMDZ				3			// 中压脚驱动交换数据用
#define 	PMDU				4			// U轴驱动交换数据用
#define 	PMDV				5			// V轴驱动交换数据用
#define 	PMDW				6			// W轴驱动交换数据用
#define 	SVNM				7			// 主轴驱动交换数据用
#define 	VIDEO				8			// 视觉处理交换数据用
#define 	DISP				9			// 打版器交换数据用
#define 	NETA				10			// 联网接口A交换数据用
#define 	NETB				11			// 联网接口B交换数据用
#define 	PAGEBUF				12			// 公共数据交换用
#define 	ERRO				13			// 错误信息
#define 	MSSG				14			// 系统运行信息记录
#define 	SYS					15			// 系统状态信息
#define 	PARA				16			// 参数数据
#define 	INDEX				17			// 花样文件索引表
#define 	HEAD				18			// 当前花样文件信息
#define 	PTDATA				19			// 当前花样文件数据
#define 	USERLOG				20			// 用户名密码
#define 	IOPARA				21			// IO测试
#define 	SRVPARA				22			// 伺服参数
#define 	INDEX2				23			// 花样文件索引表
#define 	GROUP				24			// 图形组合
#define 	LFPARA				25			// 电子夹线器参数
/******************* 3、寄存器读写模块序号 **************************/

/******************* 3、寄存器读写模块起始地址 **************************/
#define 	D_PMDX				0			// X轴驱动交换数据起始地址
#define 	D_PMDY				64			// Y轴驱动交换数据起始地址
#define 	D_PMDZ				128			// 中压脚驱动交换数据起始地址
#define 	D_PMDU				192			// U轴驱动交换数据用起始地址
#define 	D_PMDV				256			// V轴驱动交换数据用起始地址
#define 	D_PMDW				320			// W轴驱动交换数据用起始地址
#define 	D_SVNM				384			// 主轴驱动交换数据用起始地址
#define 	D_VIDEO				512			// 视觉处理交换数据用起始地址
#define 	D_DISP				640			// 打版器交换数据用起始地址
#define 	D_NETA				704			// 联网接口A交换数据用起始地址
#define 	D_NETB				768			// 联网接口B交换数据用起始地址
#define 	D_PAGEBUF			832			// 公共数据交换用起始地址
#define 	D_ERRO				1856		// 错误信息起始地址
#define 	D_MSSG				2112		// 系统运行信息记录起始地址
#define 	D_SYS				2304		// 系统状态信息起始地址
#define 	D_PARA				2496		// 参数数据起始地址
#define 	D_INDEX				3520		// 花样文件索引表起始地址
#define 	D_HEAD				6720		// 当前花样文件信息起始地址
#define 	D_PTDATA			6784		// 当前花样文件数据起始地址
/******************* 3、寄存器读写模块起始地址 **************************/

#define     IOREGHEAD           0
#define 	IOREGCHECK          1
#define 	INSETREGI			2//12*2*3
#define 	OUTSETREGI			74//11*2*12
#define 	IOSETREGI			338

#define 	INSETREGLEN			3
#define 	OUTSETREGLEN		11

#define 	WAITTIME1		15 	
#define 	WAITTIME		10 					// 读数据延时
#define 	WAITTIME2		30 					// 读主板U盘数据延时
#define 	WAITTIME3		1 					// 语音播放间隔时间

#define		FORTIMS1		100					//循环查询主板U盘次数

#define 	WAIT_T		500 					// 读数据延时
#define 	RD_T		10000					// 连续读数据的次数

#define 	RD_T_DISK		240000

#define 	FREE			0
#define 	RSUCCEED		1
#define 	WSUCCEED		2
#define 	ERROR			3
#define 	OVERTIME		4
#define 	BUSY			5

//#define NOMEMORY    0
//#define USB     	1
//#define SD     	 	2
//#define FDC     	3

#define USB_FLAG     0x01
#define SD_FLAG      0x02
#define FDC_FLAG     0x04



/************* 15系统状态信息（区域15）起始寄存器D2304至D2495共192个寄存器 ***********/
// 系统信息寄存器 begin
#define		SYS_WP			0					// 512以下保护位.第一次写入54321后可写,上电后自动保护
#define		SYS_ID1			1					// ID1
#define		SYS_ID2			2					// ID2
#define		SYS_HVER		3					// 硬件版本号
#define		SYS_SVER		4					// 软件版本号
#define		SYS_WT1			5					// 更改时间
#define		SYS_WT2			6					// 更改时间
#define		SYS_KEY1		7					// 密钥1
#define		SYS_KEY2		8					// 密钥2
#define		SYS_MS			9					// bit0-7秒0-59, bit8-15分钟0-59，十进制4bit为一个数字
#define		SYS_WH			10					// bit0-7小时0-24,bit8-15星期0-6，十进制4bit为一个数字	D7=12/24
#define		SYS_MD			11					// bit0-7日1-31,bit8-15月1-12，十进制4bit为一个数字
#define		SYS_Y			12					// bit0-7年0-99,十进制4bit为一个数字
#define		SYS_RETURN_MD			13			// bit0-7日1-31,bit8-15月1-12，16进制		
#define		SYS_RETURN_Y			14			// bit0-7年0-99, BIT8 : 1 为有效期 0 为试用期
#define		ID_CODE1		15					// ID 版本号高位
#define		ID_CODE2		16					// ID 版本号低位

#define		SYS_WIFI_VER			18		//WIFI版本号
#define		SYS_FLASH_VER			19		//FLASH版本号
#define		SYS_MACHINE_TYPE			20		// 机型

#define		SYS_MAINBOARD_VER			21		//主板版本号
#define		SYS_MAINBOARD_CODE1			22		//主板编号低位
#define		SYS_MAINBOARD_CODE2			23		//主板编号高位

#define		SYS_OPERA_VER				24		//伺服板版本号
#define		SYS_OPERA_CODE1			25		//操作板编号低位
#define		SYS_OPERA_CODE2			26		//操作板编号高位
#define		SYS_DSP_VER				27		//打板器版本号


#define		SYS_IP_CODE1			30		//ip号高两位
#define		SYS_IP_CODE2			31		//ip号低两位
#define		SYS_MACHINE_NAME1		32		//机器名
#define		SYS_MACHINE_NAME2		33		
#define		SYS_MACHINE_NAME3		34		
#define		SYS_MACHINE_NAME4		35

#define		SYS_MACHINE_TYPEC1		36		// 机型前面字母
#define		SYS_MACHINE_TYPEC2		37		// 机型后面字母


#define		SYS_PEDAL_V				53	//踏板电压
#define		SYS_PEDAL_VMIN			56	//踏板电压最小值
#define		SYS_PEDAL_VMAX			57	//踏板电压最大值
#define		SYS_CAR_OUTI			54	//车头板输出电流
#define		SYS_ARA_I				58	//报警值
#define		SYS_ARA_S				59	//报警开关

#define		SYS_TEMPERATURE			139	//温度

#define		FGPARA_ADDR				236448	//折袋参数设置地址

// 系统信息寄存器 end


// 系统参数修正		 begin
#define		SYS_PARA1CHKSUM	         60 	// 针杆旋转修正参数校验         
#define		SYS_PARA1X1		         61 	// 针杆旋转修正参数_90度修正坐标X 
#define		SYS_PARA1Y1		         62 	// 针杆旋转修正参数_90度修正坐标Y 
#define		SYS_PARA1X2		         63 	// 针杆旋转修正参数_180度修正坐标X
#define		SYS_PARA1Y2		         64 	// 针杆旋转修正参数_180度修正坐标Y
#define		SYS_PARA1X3		         65 	// 针杆旋转修正参数_270度修正坐标X
#define		SYS_PARA1Y3		         66 	// 针杆旋转修正参数_270度修正坐标Y
#define		SYS_PARA1XR		         67 	// 针杆旋转修正参数_圆心坐标X     
#define		SYS_PARA1YR		         68 	// 针杆旋转修正参数_圆心坐标Y     
#define		SYS_PARA1RD		         69 	// 针杆旋转修正参数_半径          
#define		SYS_PARA1ANG	         70 	// 针杆旋转修正参数_0度位置角度   
#define		SYS_PARA2CHKSUM	         71		// 机械原点修正参数校验           
#define		SYS_PARA2XORG	         72 	// 机械原点修正参数_修正坐标X     
#define		SYS_PARA2YORG	         73 	// 机械原点修正参数_修正坐标Y  
#define		SYS_PARA2PFOOT         	 74 	// 中压脚参数_修正   
#define		SYS_PARA3CHKSUM	         75  	// 正反转间隙修正参数校验       
#define		SYS_PARA3XZADJ	         76  	// 正反转间隙修正参数_修正坐标X+
#define		SYS_PARA3XFADJ	         77  	// 正反转间隙修正参数_修正坐标X-
#define		SYS_PARA3YZADJ	         78  	// 正反转间隙修正参数_修正坐标Y+
#define		SYS_PARA3YFADJ	         79 	 // 正反转间隙修正参数_修正坐标Y-
#define		SYS_PARA2XDORG	         81 	// 机械原点D轴修正参数_修正坐标X     
#define		SYS_PARA2YDORG	         82 	// 机械原点D轴修正参数_修正坐标Y  

#define		SYS_PARA3MODE	         84 	// 正反转间隙修正参数_修改模式
#define		SYS_PARA3SPEED	         85 	// 正反转间隙修正参数_修改速度

#define		SYS_PARA4X	        	 87 	// 视觉坐标系X坐标
#define		SYS_PARA4Y       	 	 88 	// 视觉坐标系Y坐标

#define		SYS_TSTEPPER			98					// 步进电机连接类型
#define		SYS_FIXTURE				99				// 夹具状态 0-松开 1-夹紧 2-自动	
// 系统参数修正		 end

// 状态寄存器
#define		SYS_MSTU		100					// 主轴运行状态			r		0停止 1加速 2平速 3减速
// 参数设置
#define		SYS_MREN		101					// 每转脉冲		r	w	主轴编码器一周脉冲数
#define		SYS_MRCP		102					// 每转脉冲		r	w	主轴每转应送脉冲数
#define		SYS_MANG		103					//       	r	w	主轴当前角度 0-360
#define		SYS_MRST		104					// 				r	w	主轴CHZ复位角脉冲数=P_MS8*SYS_MREN/360 0-4000
#define		SYS_MENS		105					// 				r	w	主轴停止开始角 0-360
#define		SYS_MENT		106					// 				r	w	主轴停止角	   0-360
#define		SYS_MENE		107					// 				r	w	主轴停止终止角 0-360
// 主轴命令寄存器
#define		SYS_MSTP		108					// 停止命令		r	w	0:停止
#define		SYS_MRUN		109					// 运行命令		r	w	0:相对运行1绝对2复位8CW 9CCW
#define		SYS_MDIS		110					// 运行角度		r	w	度
#define		SYS_MLSP		111					// 起步速度		r	w	rpm
#define		SYS_MHSP		112					// 最高速度		r	w	rpm
#define		SYS_MSSP		113					// 停止速度		r	w	rpm
#define		SYS_MUSP		114					// 加速度		r	w	rpm
#define		SYS_MDSP		115					// 减速度		r	w	rpm

#define		SYS_XENCODER				119				//X轴编码器
#define		SYS_YENCODER				126				//Y轴编码器
#define		SYS_ZENCODER				127				//Z轴编码器

#define 	SYS_PFOOTNOW		120				//当前中压脚高度		2009.7.16
#define 	SYS_PFOOTSET		121				//设定中压脚基准高度	2009.7.16

#define 	SYS_LFORCESET		124				//面线张力基准值	2013.4.1
#define 	SYS_LFORCENOW		125				//面线张力当前值	2013.4.1

#define 	SYS_TEST1D			130				//程序测试用1
#define 	SYS_TEST2D			131				//程序测试用2	
#define 	SYS_UDZHOU			132				//上下轴分离机型标志位
#define 	SYS_DNANGLE			133				//主轴下轴当前角度 0-360

#define    	SYS_LISTTYPE		136  //主板内存索引类型  0：主板内存 1：主板U盘  3：网络



#define		SYS_IOCONCT		144					// io模块连接
#define		SYS_SPEED1		143					// 当前速度(兄弟系统)
#define		SYS_STATUS1		144					// 第二状态寄存器


#define		SYS_INLEVEL			146	// 输入口电平信号
#define		SYS_OUTLEVEL		152// 输出口电平信号

#define		SYS_EXINPUT			158	// 扩展输入口
#define		SYS_EXOUTPUT		159// 扩展输出口


#define		SYS_INPUTL		160	// 输入口当前状态低位
#define		SYS_INPUTH		161// 输入口当前状态高位
#define		SYS_OUTPUTL	162	// 输出口当前状态低位
#define		SYS_OUTPUTH	163	// 输出口当前状态高位

#define		SYS_CHANGENEEDLE	164	// 换针类型
#define		SYS_RUN_TIMER		165		//加工时间
#define		SYS_STOP_TIMER		166		//停止时间

#define		SYS_GROUP_TYPE		167		//图形组合类型  1-40 OR 100  使用组合图形  其他使用正常图形
#define		SYS_GROUP_NO		168		//组合图形编号
#define		SYS_GROUP_NUMS		169		//组合图形花样数据

#define		SYS_WINDSP		172					// 卷线速度
#define		SYS_WINDTS		173					// 卷线时间
#define		SYS_MAX_X		174					// 机型最大移动X
#define		SYS_MAX_Y		175					// 机型最大移动Y
#define		SYS_MAX_XL		176					// 当前X最大左极限
#define		SYS_MAX_XR		177					// 当前X最大右极限
#define		SYS_MAX_YU		178					// 当前Y最大上极限
#define		SYS_MAX_YD		179					// 当前Y最大下极限
#define		SYS_ENABLE		180					// 禁止状态
#define		SYS_STATUS		181					// 各种状态   
#define		SYS_SPEED		182					// 当前速度
#define		SYS_TEMPER		139					// 温度			183->139			2017-4-18
#define		SYS_POWERV		140					// 电压			184->140			2017-4-18
#define		SYS_UPCOUNT		183					// 加算当前值	185->[183,184]		2017-4-18
#define		SYS_DNCOUNT		185					// 减算当前值	186->[185,186]		2017-4-18
#define		SYS_TSERVO		187					// 私服连接类型
#define		SYS_FSIZE		188					// 内存总容量
#define		SYS_FUSEED		189					// 已使用容量
#define		SYS_SUMFILE		190					// 已使用针数
#define		SYS_COMEN		191					// 通讯允许

#define		C_IAPCOMM				212					//iap升级命令

//系统状态信息 D_STATUS  SYS_STATUS 各种状态寄存器1
#define		S_RUN				YREG[SYS_STATUS]&bit0		// 运行/停止
#define			S_RUNSET()		YREG[SYS_STATUS]|=bit0	// 
#define			S_RUNCLR()		YREG[SYS_STATUS]&=~bit0	// 
#define		S_HOME				YREG[SYS_STATUS]&bit1		// 已回原点/非原点
#define			S_HOMESET()		YREG[SYS_STATUS]|=bit1	// 
#define			S_HOMECLR()		YREG[SYS_STATUS]&=~bit1	//
#define		S_NEWHOME			YREG[SYS_STATUS]&bit2		// 已回原点/非原点
#define			S_NEWHOMESET()	YREG[SYS_STATUS]|=bit2	// 
#define			S_NEWHOMECLR()	YREG[SYS_STATUS]&=~bit2	// 
#define		S_CLAMP				YREG[SYS_STATUS]&bit3		// 外压脚上升/下降
#define			S_CLAMPSET()	YREG[SYS_STATUS]|=bit3	// 
#define			S_CLAMPCLR()	YREG[SYS_STATUS]&=~bit3	// 
#define		S_PFOOT				YREG[SYS_STATUS]&bit4		// 中压脚上升/下降
#define			S_PFOOTSET()	YREG[SYS_STATUS]|=bit4	// 
#define			S_PFOOTCLR()	YREG[SYS_STATUS]&=~bit4	// 
#define		S_NEEDLE			YREG[SYS_STATUS]&bit5		// 针上升/下降
#define			S_NEEDLESET()	YREG[SYS_STATUS]|=bit5	// 
#define			S_NEEDLECLR()	YREG[SYS_STATUS]&=~bit5	//
#define		S_LIMIT				YREG[SYS_STATUS]&bit6		// 坐标超出加工范围
#define			S_LIMITSET()	YREG[SYS_STATUS]|=bit6	// 
#define			S_LIMITCLR()	YREG[SYS_STATUS]&=~bit6	//
#define		S_2HD				YREG[SYS_STATUS]&bit7		// 次原点位置
#define			S_2HDSET()		YREG[SYS_STATUS]|=bit7	// 
#define			S_2HDCLR()		YREG[SYS_STATUS]&=~bit7	//  
#define		S_UPDATEPARA		YREG[SYS_STATUS]&bit8		// 参数更新
#define			S_UPDATEPARASET()	YREG[SYS_STATUS]|=bit8	// 
#define			S_UPDATEPARACLR()	YREG[SYS_STATUS]&=~bit8	//
#define		S_WIND				YREG[SYS_STATUS]&bit9		// 卷线
#define			S_WINDSET()		YREG[SYS_STATUS]|=bit9	// 
#define			S_WINDCLR()		YREG[SYS_STATUS]&=~bit9	//
#define		S_SHIELDMSG			YREG[SYS_STATUS]&bit11		// 屏蔽报警警告提示信息
#define			S_SHIELDMSGSET() 	YREG[SYS_STATUS]|=bit11// 
#define			S_SHIELDMSGCLR() 	YREG[SYS_STATUS]&=~bit11// 
#define		S_2HOME				YREG[SYS_STATUS]&bit13		
#define			S_2HOMESET()		YREG[SYS_STATUS]|=bit13	// 
#define			S_2HOMECLR()		YREG[SYS_STATUS]&=~bit13	//
#define		S_LEDF				YREG[SYS_STATUS]&bit14		// LED
#define			S_LEDFSET()		YREG[SYS_STATUS]|=bit14	// 
#define			S_LEDFCLR()		YREG[SYS_STATUS]&=~bit14//
#define		S_ERROR				YREG[SYS_STATUS]&bit15		// 错误
#define			S_ERRORSET()	YREG[SYS_STATUS]|=bit15	// 
#define			S_ERRORCLR()	YREG[SYS_STATUS]&=~bit15//

#define		S_MCLAMP			YREG[SYS_STATUS1]&bit1		// 小压脚上升/下降	
#define		S_MCLAMPSET()		YREG[SYS_STATUS1]|=bit1	 
#define		S_MCLAMPCLR()		YREG[SYS_STATUS1]&=~bit1

#define		S_BMOLD				YREG[SYS_STATUS1]&bit2		// 袋模伸缩
#define		S_BMOLDSET()		YREG[SYS_STATUS1]|=bit2	 
#define		S_BMOLDCLR()		YREG[SYS_STATUS1]&=~bit2

#define		S_FPLATE			YREG[SYS_STATUS1]&bit3		// 送料板上升/下降	
#define		S_FPLATESET()		YREG[SYS_STATUS1]|=bit3	 
#define		S_FPLATECLR()		YREG[SYS_STATUS1]&=~bit3

#define		S_DEMOE				YREG[SYS_STATUS1]&bit4		// 演示
#define		S_DEMOSET()			YREG[SYS_STATUS1]|=bit4	 
#define		S_DEMOCLR()			YREG[SYS_STATUS1]&=~bit4
// D_ENABLE 系统使能状态信息
#define		E_RUN				YREG[SYS_ENABLE]&bit0		// 运行
#define			E_RUNEN()		YREG[SYS_ENABLE]|=bit0	// 
#define			E_RUNDIS()		YREG[SYS_ENABLE]&=~bit0	// 
#define		E_HOME				YREG[SYS_ENABLE]&bit1		// 回原点
#define			E_HOMEEN()		YREG[SYS_ENABLE]|=bit1	// 
#define			E_HOMEDIS()		YREG[SYS_ENABLE]&=~bit1	// 
#define		E_CLAMP				YREG[SYS_ENABLE]&bit2		// 大压脚
#define			E_CLAMPEN()		YREG[SYS_ENABLE]|=bit2	// 
#define			E_CLAMPDIS()	YREG[SYS_ENABLE]&=~bit2	// 
#define		E_PFOOT				YREG[SYS_ENABLE]&bit3		// 中压脚
#define			E_PFOOTEN()		YREG[SYS_ENABLE]|=bit3	// 
#define			E_PFOOTDIS()	YREG[SYS_ENABLE]&=~bit3	// 
#define		E_NEEDLE			YREG[SYS_ENABLE]&bit4		// 针
#define			E_NEEDLEEN()	YREG[SYS_ENABLE]|=bit4	// 
#define			E_NEEDLEDIS()	YREG[SYS_ENABLE]&=~bit4	// 
#define		E_WIND				YREG[SYS_ENABLE]&bit5		// 卷线 禁止运行
#define			E_WINDEN()		YREG[SYS_ENABLE]|=bit5	// 
#define			E_WINDDIS()		YREG[SYS_ENABLE]&=~bit5	//

#define		E_IOTEST			YREG[SYS_ENABLE]&bit6		// 输入输出测试 禁止运行+++++
#define			E_IOTESTEN()	YREG[SYS_ENABLE]|=bit6	// 
#define			E_IOTESTDIS()	YREG[SYS_ENABLE]&=~bit6	//

#define		E_WORKSHOP			YREG[SYS_ENABLE]&bit7		// 联网操作 禁止运行+++++
#define			E_WORKSHOPEN()	YREG[SYS_ENABLE]|=bit7	// 
#define			E_WORKSHOPDIS()	YREG[SYS_ENABLE]&=~bit7//
/************* 15系统状态信息（区域15）起始寄存器D2304至D2495共192个寄存器 ***********/



/*************************** 9 打版器交换数据寄存器 ********************************/
// 9 打版器交换数据寄存器
//#define		D_STATUS		0					// 各种状态
//#define		D_ENABLE		1					// 禁止状态

#define		D_MODE			2						// 模式
#define		D_RUN			3						// 运行状态
#define		D_RUNEN			4						// 运行允许
//#define		D_SPEED			5						// 速度			0-9
//#define		D_TEMPER		6						// 温度			-50-100
//#define		D_POWERV		7						// 电源电压		0-300
#define		D_MSPEED		8						// 主轴转速		0-3000
#define		D_UPCOUNT1		9						// UPCOUNT		0-9999
//#define		D_UPCOUNT2		10						// UPCOUNT		0-9999
#define		D_ERRWAR		10						// 主板发来的报警 警告信息
#define		D_DNCOUNT1		11						// DNCOUNT		0-9999
#define		D_DNCOUNT2		12						// DNCOUNT		0-9999
#define		D_MERR1			13						// M轴故障1
#define		D_MERR2			14						// M轴故障1
#define		D_DERR1			15						// D轴故障1
#define		D_XERR1			16						// X轴故障1
#define		D_YERR1			17						// Y轴故障1
#define		D_FILESTU		18						// 花样文件状态
#define		D_FILEPOS		19						// 花样文件当前显示位置

#define		D_XERR2			58						// 步进X轴故障1
#define		D_YERR2			59						// 步进Y轴故障1
#define		D_ZERR2			60						// 步进Z轴故障1
#define		D_UERR2			61						// 步进U轴故障1
#define		D_VERR2			62						// 步进V轴故障1

#define		D_COMMAND		20						// 指令
#define		D_CMDSTATUS		21						// 指令状态
#define		D_CMDPARA1		22						// 指令参数1
#define		D_CMDPARA2		23						// 指令参数2
#define		D_CMDPARA3		24						// 指令参数3
#define		D_CMDPARA4		25						// 指令参数4
#define		D_CMDPARA5		26						// 指令参数5
#define		D_CMDPARA6		27						// 指令参数6
#define		D_CMDPARA7		28						// 指令参数7
#define		D_CMDPARA8		29						// 指令参数8

#define		D_MS			30					// bit0-7秒0-59, bit8-15分钟0-59，十进制4bit为一个数字
#define		D_WH			31					// bit0-7小时0-24,bit8-15星期0-6，十进制4bit为一个数字	D7=12/24
#define		D_MD			32					// bit0-7日1-31,bit8-15月1-12，十进制4bit为一个数字
#define		D_Y				33					// bit0-7年0-99,十进制4bit为一个数字

#define		D_PGROP			54				//快速设定参数
//写flash暂存寄存器
#define		PAG_COPY_FINO			0

// 9 打版器交换数据寄存器之下发指令  D_COMMAND  D20 COMMAND    

/*#define		C_RUN_DEN		1						// 禁止运行
#define		C_HOME_DEN		2						// 禁止回原点
#define		C_CLAMP_DEN		3						// 禁止大压脚指令
#define		C_GOHOME		4						// 回原点指令
#define		C_PFOOT			5						// 中压脚上升下降指令
#define		C_NEEDLE		6						// 针上升下降指令
#define		C_NEWHOME		7						// 次原点指令
#define		C_SPEEDUP		8						// 加速指令
#define		C_SPEEDDN		9						// 减速指令
#define		C_JOGUP			10						// 步走加指令
#define		C_JOGDN			11						// 步走减指令*/

#define		C_YABANDOWN				15				//压板下降
#define		C_COMNECTION_OK		16				//打版器已连接
#define		C_YABANUP				17				//压板抬起
#define		C_GOHOME_PRESS_ONT		18			// 回原点指令,压板不动作
#define		C_NEWHOME_PRESS_ONT		19				//次原点,压板不动作
#define		C_GOHOME		20						// 回原点指令,压板动作
#define		C_NEWHOME		21						// 次原点指令,压板动作
#define		C_PFOOTUP		22						// 中压脚上升指令
#define		C_PFOOTDN		23						// 中压脚下降指令
#define		C_NEEDLEUP		24						// 针上升指令
//#define		C_NEEDLEDN		25						// 针下降指令
#define		C_ENCODE_ANGLE		25					// 编码器角度
#define		C_SPEEDUP		26						// 加速指令
#define		C_SPEEDDN		27						// 减速指令
#define		C_JOGUP			28						// 步走加(相对)指令
#define		C_JOGDN			29						// 步走减(相对)指令
#define		C_ABSJOGUP		30						// 步走加(绝对)指令
#define		C_ABSJOGDN		31						// 步走减(绝对)指令
#define		C_ABSLTNSSMOVE		32						// 绝对移动指令
#define		C_RELATIONMOVE		33						// 相对移动指令
#define		C_READPOSI		34						// 读取当前位置
#define		C_RDCMDSTATUS	35						// 读取指令执行情况
#define		C_RDCMDSTATUS	35						// 读取指令执行情况
#define		C_WRITESTATUS	36						// 写状态寄存器
#define		C_XYMOFFLINE	37						// xym轴脱机指令
#define		C_WRITE_VER		38						// 写打板器版本号
#define		C_NEEDLE_UP		39					// 针上升
#define		C_JOGUPCON		133						// 持续步走加(相对)指令
#define		C_JOGDNCON		134						// 持续步走减(相对)指令
#define		C_JOGSTOP		135						// 暂停步进走

#define		C_SPEEDUP1		136						// 加速指令(兄弟系统)
#define		C_SPEEDDN1		137						// 减速指令(兄弟系统)

#define		C_SPEEDDN1		137						// 减速指令(兄弟系统)

#define		C_RDMEMPATLIST		40						// 读内存中的花样文件索引表
#define		C_RDFLAPATLIST		41						// 读优盘中的花样文件索引表
#define		C_RDMEMNAMELIST	42						// 重读内存中的文件名
#define		C_RDFLANAMELIST	43						// 重读优盘中的文件名
#define		C_READMEMORYPAT	44						// 读内存中花样文件
#define		C_READFLASHPAT		45						// 读优盘中花样文件
#define		C_DELMEMORYPAT		46						// 删除内存中花样文件
#define		C_DELFLASHPAT		47						// 删除优盘中花样文件
#define		C_WRITEMEMORYPAT	48					// 写内存中花样文件
#define		C_WRITEFLASHPAT	49						// 写优盘中花样文件
#define		C_UPDATEPARA	50						// 将参数写到主机并保存到内存-保存为默认值
#define		C_READFSIZE		51						// 读内存使用信息

#define		C_READMEMNUMS	57						// 读内存文件数
#define		C_PATUPDATE		58						// 更新内存花样
//////////////////2011.7.21 增加了当前加工图形段号数加减指令
#define		C_UPSREG			52						//up
#define		C_DOWNSREG			53						//dn
//////////////////////////////////////////////////////////

#define		C_ZD			59

#define		C_COPYMEMORYPAT		60						// 复制内存中花样文件到优盘
#define		C_COPYFLASHPAT			61						// 复制优盘中花样文件到内存
#define		C_BACKUPPARA			62						// 备份主板参数表到优盘
#define		C_RECOVERYPARA			63						// 从优盘恢复参数表到主板
#define		C_BACKUPPROG			64						// 备份主板控制程序到优盘
#define		C_RECOVERYPROG			65						// 从优盘恢复控制程序到主板
#define     C_DISK_STATUS			66						//读外部存储器状态标志 100: 无 101: usb  102:sd  103: 软驱	
#define		C_PFOOTACTION			67						//控制中压脚的高度，在中压脚上升时用	2009.7.31	

#define		C_PFEST				68			//保存中压脚活动量设定值
#define		C_PFEMOV			69			//中压脚电机控制

#define		C_IOOUTPUT_CHECK		70				//io口输出测试
#define		C_EXCODE_OUTPUT			71				//附加功能输出
#define		C_NEEDLE_UTTER_ANGLE		72			//针绝对角度
#define		C_NEEDLE_COM_ANGLE		73			//针相对角度
#define 	C_SET_TIMER			74					//设置时间
#define 	C_XYTEST 			75	    //测试XY轴
#define 	C_CUT 				76		//剪线

#define		C_READBARCODE			77						//读条形码
#define		C_SETBARCODE			78						//写条形码

#define		C_SETLFORCE			80						//设置面线张力基准值
#define		C_WORKSHOP_EN			81						//已经联网IP指令
#define		C_RDWORKSHOPLIST		82					//读远程主机花样文件索引指令
#define		C_RWDWORKSHOPFILE		83					//读写远程花样文件指令
#define		C_SETIP					84					//修改ip指令
#define		C_RECEIVE_FILE			85					//确认接收远程文件指令
#define		C_LEDONOFF				86					//LED灯开关

#define		C_ORGMODIFY				87					//原点修正
#define		C_GAPMODIFY				88				   //正反间隙修正
#define		C_PINMODIFY				89				   //针杆旋转修正
#define		C_DORGMODIFY			91					//D轴原点修正
#define		C_VIEWMODIFY			141					//视觉坐标系设置

#define		C_BCLAMP				142			//大压板
#define		C_MCLAMP				143			//小压板
#define		C_ECLAMP				144			//伸缩板
#define		C_PCLAMP				145			//袋形板

#define		C_THERAD				90				   //穿线模式
#define		C_QUIT_THERAD			132				   //退出穿线模式

#define		C_ABSLTNSSMOVE_BOR		92						// 绝对移动指令（兄弟款）
#define		C_RELATIONMOVE_BOR		93						// 相对移动指令（兄弟款）

#define		C_SAVEPW				94				//保存密码
#define		C_USRLOG				95				//用户登录

#define		C_SAVEIO				96				//保存IO参数

#define		C_READSERVO				97				//读取伺服参数
#define		C_SAVESERVO				98				//保存伺服参数
#define		C_RECOVESERVO			99				//恢复伺服参数

#define		C_READSTEPPER			138				//读取步进电机参数
#define		C_SAVESTEPPER			139				//保存步进电机参数
#define		C_RECOVESTEPPER			140				//恢复步进电机参数

#define 	C_FIXTURE 				124				//夹具
#define		C_YABANDN				125				//压板压下(参数1:0-上升 1-下降)
#define		C_PFSTEP				127				//一段二段压板行程

#define		C_DOWNLOAD_NETCODE		197					//远程升级包
#define		C_DOWNLOAD_FLASH		198					//升级打版器flash程序

#define		C_SYSRECOVER			199						//系统参数还原备份
#define		C_FORMATMEMORY			200						// 格式化（初始化）内存
#define		C_FORMATFLASH			201						// 格式化磁盘

#define		C_SETPASSSWORD			202						//发送使用权限密码
#define		C_MCPURESTART			230						//主板重启

#define		C_READGROUP				100				//读取组合图形数据
#define		C_SAVEGROUP				101				//保存组合图形数据
#define		C_OPENGROUP				102				//打开组合图形数据
#define		C_DELGROUP				103				//删除组合图形数据
#define		C_SWITCHGROUP			104				//切换组合图形数据

#define		C_CLRAIARM				105				//清除报警
#define		C_CLRRUNRECORD			106				//清除运行记录

#define		C_REINPUTSETTING		107				//还原IO输入设定
#define		C_REOUTPUTSETTING		108				//还原IO输出设定

#define		C_SETPINS				112				//设置当前移动针数
#define		C_CHANGENEEDLE			113				//换针

#define		C_REFRESH_ERRRECORD		114				//刷新错误记录
#define		C_REFRESH_RUNRECORD		115				//刷新运行记录

#define		C_ANALOG_SET			116				//模拟量设置

#define		C_READ_RUNRECORD		117				//读运作记录

#define		C_READ_WIFI				118				//读wifi信息
#define		C_WRITE_WIFI			119				//写wifi信息

#define		C_NEWORD_CHAT			121				//网络会话
#define		C_SAVE_MPROG			126				//保存快速修改参数

#define		C_TRIM_OUT				128			//剪线输出

#define		C_PIN_ANG				129			//针角度
#define		C_SET_PARA1				130			//设置参数一
#define		C_SET_PARA2				131			//设置参数一

#define		C_FB_STEP0				145			//袋模伸缩
#define		C_FB_STEP1				146			//折边器下压
#define		C_FB_STEP2				147			//折料一
#define		C_FB_STEP3				148			//折料二
#define		C_FB_STEP4				149			//折料三
#define		C_FB_STEP5				150			//折料四
#define		C_FB_STEP6				151			//整体台
#define		C_FB_MODE				152			//模式
#define		C_FB_PSAVE				153			//参数保存
#define		C_FB_RESET				154			//参数恢复-回原点
#define		C_FB_STEP0_1			155			//折边器伸缩,插入(0,1)
#define		C_FB_CHMOD				156			//换模气缸
#define		C_FB_MV					157			//移动

#define		C_FB_ZL					158			//折料
#define		C_FB_QL					159			//取料

#define		C_FB_RSETP				160			//参数设置-参数恢复

#define		C_FB_EXSTEP1			161			//放料
#define		C_FB_EXSTEP2			162			//取料
#define		C_FB_EXSTEP3			163			//回到起点
#define		C_FB_EXSTEP4			164			//送料板升降
#define		C_FB_DEMO				165			//演示

#define		C_LF_SET				166			//设定电子夹线器
#define		C_MACHINE_SET			167			//机器信息(参数1(0-读取当前参数,1-设置默认参数,2-保存参数))

#define		C_WAVES					168			//波形查看:[参数1;(0-停止;1-单次;2-连续)] [参数2:(周期)] [参数3:通道1] [参数4:通道2]

#define		C_LDOWMPF				169			//折边器打板修改机头下降

/*************************** 9 打版器交换数据寄存器 ********************************/






// PREG
//---------------------------------------------------
//  参数寄存器定义
//---------------------------------------------------
	
// 拔线(Wiper)														单位		Min		Max		Std
#define	P_WIP	PREG[(U16)(4)*0+3]		// 拔线的ON/OFF													ON(1)
#define	P_W1	PREG[(U16)(4)*1+3]		// 拔线输出(W)的开始输出时间		msec		0		998		30
#define	P_W2	PREG[(U16)(4)*2+3]		// 拔线输出(W)的输出时间			msec		0		998		40
// 慢速起缝(Slow start)
#define	P_S		PREG[(U16)(4)*3+3]		// 慢速起缝速度						s/min		0		2299	200
#define	P_SL	PREG[(U16)(4)*4+3]		// 慢速起缝的设定												OF(0)
#define	P_SLN	PREG[(U16)(4)*5+3]		// 慢速起缝的针数					sti.		0		5		2
// 外压脚(Clamp)
#define	P_OST	PREG[(U16)(4)*6+3]		// 外压脚没降下也可/不可动转									OF(0)
#define	P_RPT	PREG[(U16)(4)*7+3]		// 重复缝制的设定												OF(0)
#define	P_RP1	PREG[(U16)(4)*8+3]		// 压脚1的重复缝制设定											ON(1)
#define	P_RP2	PREG[(U16)(4)*9+3]		// 压脚2的重复缝制设定											ON(1)
#define	P_RP3	PREG[(U16)(4)*10+3]		// 压脚3的重复缝制设定											ON(1)
#define	P_RP4	PREG[(U16)(4)*11+3]		// 压脚4的重复缝制设定											ON(1)
#define	P_RP5	PREG[(U16)(4)*12+3]		// 压脚5的重复缝制设定											ON(1)
#define	P_RP6	PREG[(U16)(4)*13+3]		// 压脚6的重复缝制设定											ON(1)
#define	P_RP7	PREG[(U16)(4)*14+3]		// 压脚7的重复缝制设定											ON(1)
#define	P_RP8	PREG[(U16)(4)*15+3]		// 压脚8的重复缝制设定											ON(1)
#define	P_WHY	PREG[(U16)(4)*16+3]		// 压脚动作的优先顺序设定										OF(0)
#define	P_FSR	PREG[(U16)(4)*17+3]		// 压脚步进动作的解除											OF(0)
#define	P_1PD	PREG[(U16)(4)*18+3]		// 单踏板操作的设定												OF(0)
#define	P_1T	PREG[(U16)(4)*19+3]		// 单踏板操作的延迟时间设定			x10msec		0		498		0
#define	P_1A	PREG[(U16)(4)*20+3]		// 压脚1延迟输出ON时间设定			x10msec		0		999		0
#define	P_2A	PREG[(U16)(4)*21+3]		// 压脚2延迟输出ON时间设定			x10msec		0		999		0
#define	P_3A	PREG[(U16)(4)*22+3]		// 压脚3延迟输出ON时间设定			x10msec		0		999		0
#define	P_4A	PREG[(U16)(4)*23+3]		// 压脚4延迟输出ON时间设定			x10msec		0		999		0
#define	P_5A	PREG[(U16)(4)*24+3]		// 压脚5延迟输出ON时间设定			x10msec		0		999		0
#define	P_6A	PREG[(U16)(4)*25+3]		// 压脚6延迟输出ON时间设定			x10msec		0		999		0
#define	P_7A	PREG[(U16)(4)*26+3]		// 压脚7延迟输出ON时间设定			x10msec		0		999		0
#define	P_8A	PREG[(U16)(4)*27+3]		// 压脚8延迟输出ON时间设定			x10msec		0		999		0
#define	P_1B	PREG[(U16)(4)*28+3]		// 压脚1延迟输出OFF时间设定			x10msec		0		999		0
#define	P_2B	PREG[(U16)(4)*29+3]		// 压脚2延迟输出OFF时间设定			x10msec		0		999		0
#define	P_3B	PREG[(U16)(4)*30+3]		// 压脚3延迟输出OFF时间设定			x10msec		0		999		0
#define	P_4B	PREG[(U16)(4)*31+3]		// 压脚4延迟输出OFF时间设定			x10msec		0		999		0
#define	P_5B	PREG[(U16)(4)*32+3]		// 压脚5延迟输出OFF时间设定			x10msec		0		999		0
#define	P_6B	PREG[(U16)(4)*33+3]		// 压脚6延迟输出OFF时间设定			x10msec		0		999		0
#define	P_7B	PREG[(U16)(4)*34+3]		// 压脚7延迟输出OFF时间设定			x10msec		0		999		0
#define	P_8B	PREG[(U16)(4)*35+3]		// 压脚8延迟输出OFF时间设定			x10msec		0		999		0
#define	P_OFB	PREG[(U16)(4)*36+3]		// 压脚动作分割数量的设定										NO(0)
#define	P_FN	PREG[(U16)(4)*37+3]		// 有效压脚动作数量的设定						1		8		1
#define	P_CF	PREG[(U16)(4)*38+3]		// 压脚动作连动的设定											OF(0)
#define	P_F21N	PREG[(U16)(4)*39+3]		// 压脚动作分割时有效压脚数OF1-OF4				1		4		1
#define	P_F22N	PREG[(U16)(4)*40+3]		// 压脚动作分割时有效压脚数OF5-OF8				1		4		1
#define	P_CF1	PREG[(U16)(4)*41+3]		// 压脚动作分割时的连动设定OF1-OF4								OF(0)
#define	P_CF2	PREG[(U16)(4)*42+3]		// 压脚动作分割时的连动设定OF5-OF8								OF(0)
#define	P_F4BN	PREG[(U16)(4)*43+3]		// 有效压脚分组数量的设定						1		4		1
#define	P_F4SN	PREG[(U16)(4)*44+3]		// 分组步进数量的设定							1		4		1
#define	P_AF2	PREG[(U16)(4)*45+3]		// 空压两段动作压脚的选择										OF(0)
#define	P_OPR	PREG[(U16)(4)*46+3]		//+读入资料受中压脚位置限制										OF(0)
// 缝制范围(Sewing area)
#define	P_ALC	PREG[(U16)(4)*47+3]		// 缝制范围限制的解除											OF(0)
#define	P_XL	PREG[(U16)(4)*48+3]		// X轴左侧范围限制的设定			mm			0		254		101
#define	P_XR	PREG[(U16)(4)*49+3]		// X轴右侧范围限制的设定			mm			0		254		101
#define	P_YU	PREG[(U16)(4)*50+3]		// Y轴后侧范围限制的设定			mm			0		254		51
#define	P_YD	PREG[(U16)(4)*51+3]		// Y轴前侧范围限制的设定			mm			0		254		51
// 针位置(Needle position)
#define	P_RU	PREG[(U16)(4)*52+3]		// 切线后针逆转上升机能											OF(0)
#define	P_R8	PREG[(U16)(4)*53+3]		// RU的逆转角度的设定				deg.		0		360		30
#define	P_U8	PREG[(U16)(4)*54+3]		// 针上停位置的停止角度				deg.		0		360		14
#define	P_D8	PREG[(U16)(4)*55+3]		// 针下停位置的停止角度				deg.		0		360		50
// 断线检知(Thread trimming sensor)
#define	P_S1	PREG[(U16)(4)*56+3]		// 断线检测的有效/无效
#define	P_S2	PREG[(U16)(4)*57+3]		// 起缝后断线检测的无效针数
#define	P_S3	PREG[(U16)(4)*58+3]		// 缝制途中断线检测的元效针数
#define	P_B		PREG[(U16)(4)*59+3]		// 断线检测动作速度的设定
#define	P_TST	PREG[(U16)(4)*60+3]		// 断线检测时的切线有/无
// 原点(Home position)
#define	P_HPM	PREG[(U16)(4)*61+3]		// 中途暂停时的原点复归方法										--(0) --/JS/JE/JC
#define	P_HPC	PREG[(U16)(4)*62+3]		// 打开电源时原点复归动作的禁止									ON(1)
#define	P_HPF	PREG[(U16)(4)*63+3]		// 自动原点复归动作的禁止										OF(0)
#define	P_HPK	PREG[(U16)(4)*64+3]		// 复归键的设定													--(0) --/2/2L/NO
#define	P_2HS	PREG[(U16)(4)*65+3]		// 到达第2原点时停止的方法										ST(0) ST/SW/NS
#define	P_FHP	PREG[(U16)(4)*66+3]		// 任意原点的使用												OF(0)
#define	P_CHS	PREG[(U16)(4)*67+3]		// 花样资料读出时的原点補正选择									ON(1)
#define	P_SHX	PREG[(U16)(4)*68+3]		// X轴范围限制的原点复归										OF(0)
#define	P_SHY	PREG[(U16)(4)*69+3]		// Y轴范围限制的原点复归										OF(0)
#define	P_HPS	PREG[(U16)(4)*70+3]		// 上电时原点复归时间内,外压脚状态的设定						OF(0)
// 中途暂停(Halt)
#define	P_STF	PREG[(U16)(4)*71+3]		// 紧急暂停按钮按下时空送的停止状态								CP(0) CP/ST
#define	P_STN	PREG[(U16)(4)*72+3]		// 紧急暂停按钮按下时针的停止状态								UP(1) DN/UP/RN
#define	P_STS	PREG[(U16)(4)*73+3]		// 紧急暂停按钮按下时各种输出的保护								AL(0) AL/FU/OF
#define	P_ST2	PREG[(U16)(4)*74+3]		// 按第二次紧急暂停按钮时的动作									ST(2) UT/UP/ST
#define	P_STP	PREG[(U16)(4)*75+3]		// 按紧急暂停按钮时电源的操作									--(0) --/PD
#define	P_STD	PREG[(U16)(4)*76+3]		// 按中途暂停功能停止时外压脚的动作								FU(0) FU/FD
#define	P_STL	PREG[(U16)(4)*77+3]		// 中途暂停后再起缝时同位置的防止								ON(1)
// 计数(Counter)
#define	P_CUP	PREG[(U16)(4)*78+3]		// 加算UP计数功能												ED(1) --/ED/ST/CY
#define	P_CDN	PREG[(U16)(4)*79+3]		// 减算DN计数功能												ED(1) --/ED/ST/CY
#define	P_UCM	PREG[(U16)(4)*80+3]		// 加算UP计数设定值的消除方法									--(0) --/RE/IT/CL
#define	P_DCM	PREG[(U16)(4)*81+3]		// 减算UP计数设定值的消除方法									--(0) --/RE/IT/CL
#define	P_PCM	PREG[(U16)(4)*82+3]		// 重开电源时计数值的消除方法									--(0) --/IT
#define	P_CN	PREG[(U16)(4)*83+3]		// 针数的运算功能												OF(0)
#define	P_UCC	PREG[(U16)(4)*84+3]		// 加算UP计数值的修正禁止										OF(0)
#define	P_DCC	PREG[(U16)(4)*85+3]		// 减算DN计数值的修正禁止										OF(0)
#define	P_USC	PREG[(U16)(4)*86+3]		// 依加算UP计数值而缝制作业的设定								OF(0)
#define	P_DSC	PREG[(U16)(4)*87+3]		// 依减算DN计数值而缝制作业的设定								OF(0)
// 刹车(Brake)
#define	P_WBK	PREG[(U16)(4)*88+3]		// 微弱刹车的设定												OF(0)
#define	P_BKM	PREG[(U16)(4)*89+3]		// 微弱刹车的功能设定											E(0)  E/H
#define	P_BNR	PREG[(U16)(4)*90+3]		// 微弱刹车的声音设定											OF(0)
#define	P_BKT	PREG[(U16)(4)*91+3]		// 刹车时间的设定					x10msec		0		99		14
#define	P_PW	PREG[(U16)(4)*92+3]		// 中压脚动作时的微弱刹车时间		msec		0		254		50
// 中压脚(Presser foot)
#define	P_PD	PREG[(U16)(4)*93+3]		// 中压脚时间1(下降时)的设定		x10msec		0		498		36
#define	P_PU	PREG[(U16)(4)*94+3]		// 中压脚时间2(上升时)的设定		x10msec		0		498		16
#define	P_UW	PREG[(U16)(4)*95+3]		// 中压脚时间3(上升时)的设定		x10msec		0		498		10
#define	P_BPF	PREG[(U16)(4)*96+3]		// 假缝后的空送移动时中压脚的设定								OF(0)
#define	P_PLP	PREG[(U16)(4)*97+3]		// 中压脚行程设定				毫米	1,	0,	150
#define	P_ZU8	PREG[(U16)(4)*98+3]		// 中压脚行上升角度				度		1,	0,	360
#define	P_ZD8	PREG[(U16)(4)*99+3]		// 中压脚行下降角度				度		1,	0,	360
#define	P_ZTM	PREG[(U16)(4)*100+3]		// 输入时中压脚同步设定			逻辑		0,	0,	1
// 捲线(Bobbin winding)
#define	P_W		PREG[(U16)(4)*101+3]		// 捲线速度的设定					s/min		0		2299	1500
#define	P_WSM	PREG[(U16)(4)*102+3]		// 捲线动作的操作设定											NO(0)
#define	P_WT	PREG[(U16)(4)*103+3]		// 捲线的动作时间					sec			1		498		30
// 送布方式(Feed method)
#define	P_WET	PREG[(U16)(4)*104+3]		// 外压脚的重量选择												L(0)  L/M/H
#define	P_WEL	PREG[(U16)(4)*105+3]		// 外压脚的重量(WET=L)时的设定值				0		99		20
#define	P_WEM	PREG[(U16)(4)*106+3]		// 外压脚的重量(WET=M)时的设定值				0		99		26
#define	P_WEH	PREG[(U16)(4)*107+3]		// 外压脚的重量(WET=H)时的设定值				0		99		35
#define	P_THI	PREG[(U16)(4)*108+3]		// 缝物厚度的选择												L(0)  L/M/H
#define	P_TL	PREG[(U16)(4)*109+3]		// 缝物厚度(THI=L)时的设定值					0		60		0
#define	P_TM	PREG[(U16)(4)*110+3]		// 缝物厚度(THI=M)时的设定值					0		60		32
#define	P_TH	PREG[(U16)(4)*111+3]		// 缝物厚度(THI=H)时的设定值					0		60		44
#define	P_FED	PREG[(U16)(4)*112+3]		// 送布方式的设定												ID(1) IT/ID/S
#define	P_FEM	PREG[(U16)(4)*113+3]		// 空送动作的选择												S(1)  K/S
#define	P_FES	PREG[(U16)(4)*114+3]		// 平顺空送的设定								0		9		9
#define	P_FSM	PREG[(U16)(4)*115+3]		// 平顺空送的设定												ON(1)
#define	P_TYA	PREG[(U16)(4)*116+3]		// 打版时箭印最大移动量(倍数)的设定	time		1		15		1
// 缝制速度(Sewing speed)
#define	P_H		PREG[(U16)(4)*117+3]		// 最高速度							s/min		0		2299	2299
#define	P_L		PREG[(U16)(4)*118+3]		// 最低速度							s/min		0		399		200
#define	P_M		PREG[(U16)(4)*119+3]		// 中高速度MD1						s/min		0		2299	1500
#define	P_N		PREG[(U16)(4)*120+3]		// 中低速度MD2						s/min		0		2299	800
// 切线时间(Thread trimming timing)
#define	P_LTM	PREG[(U16)(4)*121+3]		// 切线输出(T)的输出种类										T1(0) T1/T2/T3/T4/T5
#define	P_LLM	PREG[(U16)(4)*122+3]		// 放线输出(L)的输出种类										L1(0) L1/L2/L3/L4/L5
#define	P_T1	PREG[(U16)(4)*123+3]		// 切线输出(T)开始输出的时间/角度	msec		0		998		50
#define	P_T2	PREG[(U16)(4)*124+3]		// 切线输出(T)输出的时间/结束角度	msec		0		998		0
#define	P_L1	PREG[(U16)(4)*125+3]		// 放线输出(L)开始输出的时间/角度	msec		0		998		196
#define	P_L2	PREG[(U16)(4)*126+3]		// 放线输出(L)输出的时间/结束角度	msec		0		998		0
#define	P_T		PREG[(U16)(4)*127+3]		// 切线速度							s/min		0		399		200
#define	P_ETC	PREG[(U16)(4)*128+3]		// 依针上升键使针上升时解除切线动作								OF(0)
#define	P_TRM	PREG[(U16)(4)*129+3]		// 切线动作的ON/OFF												ON(1)
#define	P_LP	PREG[(U16)(4)*130+3]		// 中压脚上升松线器输出时		x10msec		0		998		0
#define	P_LSP	PREG[(U16)(4)*131+3]		// 一般松线器电磁阀的使用情况		x10msec		0		998		0
// 步骤(Step)
#define	P_STE	PREG[(U16)(4)*132+3]		// 设定桌面转换时的步骤要点删除设定								OF(0)
#define	P_STO	PREG[(U16)(4)*133+3]		// 针杆自动旋转的ON/OFF												OF(0)
#define	P_SUU	PREG[(U16)(4)*134+3]		// 解释度的执行行数					line		1		192		2
#define	P_SP1	PREG[(U16)(4)*135+3]		// 程序运行方式 版本1					line		1		192		2
#define	P_SKF	PREG[(U16)(4)*136+3]		// 当IOF输入时,程序完全中止				line		1		192		2
// 寸动(Jog)
#define	P_JGM	PREG[(U16)(4)*137+3]		// 动键的功能设定												--(0) --/AL/HA
#define	P_UJC	PREG[(U16)(4)*138+3]		// 针停在上停位置以外时的寸动键的设定							OF(0)
#define	P_JGS	PREG[(U16)(4)*139+3]		// 连续移送时暂停代码的处理							OF(0)
// 送布角度(Feed angle)
#define	P_S8	PREG[(U16)(4)*140+3]		// 开始送布的角度设定				deg.		25		198		94
#define	P_E8	PREG[(U16)(4)*141+3]		// 停止送布的角度设定				deg.		100		356		344
#define	P_T8	PREG[(U16)(4)*142+3]		// 低速时停止送布的角度设定			deg.		150		356		324
#define	P_8D	PREG[(U16)(4)*143+3]		// 显示针下停位置的角度				deg.
// 液晶显示器(LCD panel)
#define	P_KBC	PREG[(U16)(4)*144+3]		// 打版器的呜声解除												OF(0)
#define	P_LCD	PREG[(U16)(4)*145+3]		// 打版器的连接													ON(1)
#define	P_LOF	PREG[(U16)(4)*146+3]		// 打版器的省电功能												ON(1)
#define	P_LNN	PREG[(U16)(4)*147+3]		// 打版器的显示颜色												ON(1)
#define	P_LHP	PREG[(U16)(4)*148+3]		// 按键说明的显示												OF(1)
#define	P_KNO	PREG[(U16)(4)*149+3]		// 标准模组的背景选择							1		3		1
#define	P_CTR	PREG[(U16)(4)*150+3]		// 按键敏感度									0		10		0
#define	P_CNR	PREG[(U16)(4)*151+3]		// 打版器画面的对比								0		100		50
#define	P_R		PREG[(U16)(4)*152+3]		// 打版器的省电亮灯时间				/60min		0		9998	300
// 输入信号(Input signal)
#define	P_I1	PREG[(U16)(4)*153+3]		// I1输入的功能选择
#define	P_I1L	PREG[(U16)(4)*154+3]		// I1输入的电平转换
#define	P_I1M	PREG[(U16)(4)*155+3]		// I1输入的动作选择
// 输出信号(Output signal)
#define	P_O1	PREG[(U16)(4)*156+3]		// O1输出的功能选择
#define	P_O1L	PREG[(U16)(4)*157+3]		// O1输出的电平转换
#define	P_O1C	PREG[(U16)(4)*158+3]		// O1输出的断续功能
#define	P_O1M	PREG[(U16)(4)*159+3]		// O1输出的动作选择1
#define	P_O1D	PREG[(U16)(4)*160+3]		// O1输出的ON延迟设定
#define	P_O1E	PREG[(U16)(4)*161+3]		// O1输出的动作选择2
#define	P_O_1A	PREG[(U16)(4)*162+3]		// O1输出到ON的延迟时间/脉波输出的OFF时间设定
#define	P_O_1B	PREG[(U16)(4)*163+3]		// O1输出的ON时间/脉波的ON时间设定
#define	P_O_1T	PREG[(U16)(4)*164+3]		// O1输出的全波输出时间设定
#define	P_O_1C	PREG[(U16)(4)*165+3]		// O1输出的断续效率的设定
// 其它(Other)
#define	P_GA	PREG[(U16)(4)*166+3]		// 高/低放大功能设定											L(0) L/LL/H
#define	P_MIL	PREG[(U16)(4)*167+3]		// 铣床功能的ON/OFF												OF(0)
#define	P_APC	PREG[(U16)(4)*168+3]		// 依端子台的输入指定缝制资料功能								OF(0)
#define	P_POF	PREG[(U16)(4)*169+3]		// 图形分离加工选择												OF(0)
#define	P_ASC	PREG[(U16)(4)*170+3]		// 依外部的输入指定最高速度限制的功能							OF(0)
#define	P_AER	PREG[(U16)(4)*171+3]		// 错误资料出力功能												OF(0)
#define	P_PFL	PREG[(U16)(4)*172+3]		// 内部记忆体禁止书写删除设定									OF(0)
#define	P_RN	PREG[(U16)(4)*173+3]		// 连续送布时的最大缝目长度
#define	P_LH	PREG[(U16)(4)*174+3]		// 连续送布时的最大缝目长度
#define	P_LCK	PREG[(U16)(4)*175+3]		// 禁止有控制箱的设定面板转换模组
#define	P_PTC	PREG[(U16)(4)*176+3]		// 从缝制资料变更设定桌面的编号									OF(0)
#define	P_SCP	PREG[(U16)(4)*177+3]		// 便捷功能键的交换												OF(0)
#define	P_DFR	PREG[(U16)(4)*178+3]		// 从磁碟片读出的资料要改写或添加的方法							OF(0)
#define	P_RT	PREG[(U16)(4)*179+3]		// 设定动力被解除延时到达							OF(0)
#define	P_SPC	PREG[(U16)(4)*180+3]		// 序列通讯							OF(0)
#define	P_APT	PREG[(U16)(4)*181+3]		// 图形时序由外接埠选择							OF(0)
#define	P_LAN	PREG[(U16)(4)*182+3]		// 语言选择							OF(0)
#define	P_VOI	PREG[(U16)(4)*183+3]		// 语音设定							OF(0)
#define	P_E_1	PREG[(U16)(4)*184+3]		// 错误信息1
#define	P_E_2	PREG[(U16)(4)*185+3]		// 错误信息2
#define	P_E_3	PREG[(U16)(4)*186+3]		// 错误信息3
#define	P_E_4	PREG[(U16)(4)*187+3]		// 错误信息4
#define	P_E_I1	PREG[(U16)(4)*188+3]		// I1输入信号显示
#define	P_E_UP	PREG[(U16)(4)*189+3]		// UP输入信号显示
#define	P_E_DN	PREG[(U16)(4)*190+3]		// DN输入信号显示
#define	P_E_XH	PREG[(U16)(4)*191+3]		// XH输入信号显示
#define	P_E_YH	PREG[(U16)(4)*192+3]		// YH输入信号显示
#define	P_E_ECA	PREG[(U16)(4)*193+3]		// ECA输入信号显示
#define	P_E_ECB	PREG[(U16)(4)*194+3]		// ECB输入信号显示
#define	P_E_CRS	PREG[(U16)(4)*195+3]		// CRS输入信号显示
#define	P_E_O1O	PREG[(U16)(4)*196+3]		// 输出
#define	P_E_O1D	PREG[(U16)(4)*197+3]		// 输出
#define	P_E_P	PREG[(U16)(4)*198+3]		// 显示累积通电时间
#define	P_E_M	PREG[(U16)(4)*199+3]		// 显示累积马达转动的时间
#define	P_E_DV	PREG[(U16)(4)*200+3]		// 显示设定桌面的版本号
#define	P_E_T	PREG[(U16)(4)*201+3]		// 显示机种
#define	P_E_BV	PREG[(U16)(4)*202+3]		// 显示系统ROM的版本号
#define	P_E_SV	PREG[(U16)(4)*203+3]		// 显示系统软件的版本号
// 特殊(Special)
#define	P_LHH	PREG[(U16)(4)*204+3]		// High speed H upper limit setting			x100s/min		0		99		23
#define	P_LHL	PREG[(U16)(4)*205+3]		// High speed H lower limit setting			x100s/min		0		99		0
#define	P_LLH	PREG[(U16)(4)*206+3]		// Low speed L upper limit setting			x100s/min		0		99		4
#define	P_LLL	PREG[(U16)(4)*207+3]		// LOw speed H lower limit setting			x100s/min		0		99		0
#define	P_DSL	PREG[(U16)(4)*208+3]		// Speed limit setting														OF(0)
#define	P_FSL	PREG[(U16)(4)*209+3]		// Speed limit cancel when continuos feed									ON(1)
#define	P_STM	PREG[(U16)(4)*210+3]		// Needle stop mode selection												OF(0)
#define	P_KU	PREG[(U16)(4)*211+3]		// Selection of UP signal													OF(0)
#define	P_K8	PREG[(U16)(4)*212+3]		// UP position angle from down position		deg.			0		360		270
#define	P_E8C	PREG[(U16)(4)*213+3]		// Detector error cancel mode												OF(0)
#define	P_MSP	PREG[(U16)(4)*214+3]		// Max. speed for main motor												36(0)
#define	P_PL	PREG[(U16)(4)*215+3]		// Sewing machine shaft/motor shaft speed setting selection					OF(0)
#define	P_MR	PREG[(U16)(4)*216+3]		// Motor pulley diameter					mm				0		254		60
#define	P_SR	PREG[(U16)(4)*217+3]		// Sewing machine pulley diameter			mm				0		254		70
#define	P_ACT	PREG[(U16)(4)*218+3]		// Acceleration time simple setting 		msec			0		99		14
#define	P_DCT	PREG[(U16)(4)*219+3]		// 减速时间简易设定											OF(0)
#define	P_CCW	PREG[(U16)(4)*220+3]		// Main motor rotating direction											OF(0)
#define	P_L8	PREG[(U16)(4)*221+3]		// Setting ofr down angle at low speed		deg.			0		360		28
#define	P_PPD	PREG[(U16)(4)*222+3]		// PMD power down select													ON(1)
#define	P_HXC	PREG[(U16)(4)*223+3]		// X motor CW/CCW															ON(1)
#define	P_HXS	PREG[(U16)(4)*224+3]		// Stepping motor rotation direction at home positioning					ON(1)
#define	P_HYC	PREG[(U16)(4)*225+3]		// Y motor CW/CCW															OF(0)
#define	P_HYS	PREG[(U16)(4)*226+3]		// Stepping motor rotation direction at home positioning					ON(1)
#define	P_F		PREG[(U16)(4)*227+3]		// Feed speed setting										0		65535	80
#define	P_XHP	PREG[(U16)(4)*228+3]		// X axis pulse setting for home positioning				0		255		37
#define	P_XH1	PREG[(U16)(4)*229+3]		// X axis pulse setting for home positioning(high speed)	0		65535	160
#define	P_XL1	PREG[(U16)(4)*230+3]		// X axis pulse setting for home positioning(low speed)		0		65535	2048
#define	P_XT	PREG[(U16)(4)*231+3]		// Lowest movement amount when X axis return to home position	0	254		20
#define	P_YHP	PREG[(U16)(4)*232+3]		// Y axis pulse setting for home positioning				0		255		37
#define	P_YH1	PREG[(U16)(4)*233+3]		// Y axis pulse setting for home positioning(high speed)	0		65535	160
#define	P_YL1	PREG[(U16)(4)*234+3]		// Y axis pulse setting for home positioning(low speed)		0		65535	2048
#define	P_YT	PREG[(U16)(4)*235+3]		// Lowest movement amount when Y axis return to home position	0	254		20
#define	P_L3	PREG[(U16)(4)*236+3]		// Pulse motor start time when X smooth feed is low			0		9998	640
#define	P_H3	PREG[(U16)(4)*237+3]		// Pulse motor start time when X smooth feed is high		0		9998	175
#define	P_X3	PREG[(U16)(4)*238+3]		// Change value of smooth feed of X							0		9998	1
#define	P_L4	PREG[(U16)(4)*239+3]		// Pulse motor start time when Y smooth feed is low			0		9998	640
#define	P_H4	PREG[(U16)(4)*240+3]		// Pulse motor start time when Y smooth feed is high		0		9998	175
#define	P_Y4	PREG[(U16)(4)*241+3]		// Change value of smooth feed of Y							0		9998	1
#define	P_HME	PREG[(U16)(4)*242+3]		// 原点扩充功能							0		9998	1
#define	P_MOS	PREG[(U16)(4)*243+3]		// 故障输出讯号被改变							0		9998	1
#define	P_MS8	PREG[(U16)(4)*244+3]		// 主轴角度偏差值							0		9998	1







//---------------------------------------------------
//  PT_HEAD寄存器定义 64   1792 - 1855 当前花样文件的信息 
//---------------------------------------------------
//#define	HREG_IND_NO		DREG[DREG_PT_HEAD+0]	// 当前花样文件列表位置600-920 ffff空
//#define	HREG_START			DREG[DREG_PT_HEAD+1]	// 当前花样文件花样数据开始位置
//#define	HREG_READ_PTDATA	DREG[DREG_PT_HEAD+2]	// 主CPU正在读花样数据
//#define	HREG_WRITE_PTDATA	DREG[DREG_PT_HEAD+3]	// 主CPU正在写花样数据

//#define	HREG_SPPED		DREG[DREG_PT_HEAD+10]	// 运行速度

#define	HREG_SNO		20	// 记存体编号
#define	HREG_FNO		21	// 文件编号			01-00
#define	HREG_FN1		22	// 文件名1			20-21
#define	HREG_FN2		23	// 文件名2			22-23
#define	HREG_FN3		24	// 文件名3			24-25
#define	HREG_FN4		25	// 文件名4			26-27
#define	HREG_FEED_C		26	// 当前步行位置
#define	HREG_FEED_SUM	27	// 花样总针数		09-08/4
#define	HREG_FEED_PAT	28	// 花样缝针数		09-08/4-(pat中不含01,21,41,61指令)
#define	HREG_DRAW_ADD	29	// 绘图起始位置		48h
#define	HREG_LINE_ADD	30	// 线条起始位置		05-04+40h
#define	HREG_PAT_ADD	31	// 针起始位置		05-04+09-08/2+40h
#define	HREG_PATM_X		32	// 花样最大X值 S16
#define	HREG_PATM_Y		33	// 花样最大Y值 S16
#define	HREG_PATI_X		34	// 花样最小X值 S16
#define	HREG_PATI_Y		35	// 花样最小Y值 S16
#define	HREG_SCALE1		36	// 比例 S32
#define	HREG_SCALE2		37	// 比例 S32
#define	HREG_ORG_F		38	// 原点置位
#define	HREG_VER1		39	// 版本char			3C-3F
#define	HREG_VER2		40	// 版本char			3C-3F
#define	HREG_MAX_X		41	// 机型最大移动X
#define	HREG_MAX_Y		42	// 机型最大移动Y

#define	F_IND_NO		HREG[0]		// 当前花样文件列表位置600-920 ffff空								用
#define	F_START			HREG[1]		// 当前花样文件花样数据开始位置										用
#define	F_READ_PTDATA	HREG[2]		// 主CPU正在读花样数据												用
#define	F_WRITE_PTDATA	HREG[3]		// 主CPU正在写花样数据												用
#define	F_FILELENL		HREG[4]		// 当前花样总长度低位												用
#define	F_FILELENH		HREG[5]		// 当前花样总长度高位												用
#define F_FILELEN      (HREG[5] << 16 | HREG[4])// 当前花样总长度

#define	F_ENDCODE		HREG[7]		// 结束代码
#define	F_FILETYPE		HREG[8]		// 文件类型 1：三菱	2：兄弟
#define	F_GOHOMEFLAG	HREG[9]		// 运行速度
#define	F_PT_SPPED		HREG[10]	// 运行速度

#define	F_PT_PARTSUM	HREG[11]	// 当前花样中的子花样数												用
#define	F_PT_PARTNUM	HREG[12]	// 当前花样中正在运行的子花样号										用
#define	F_PT_STPATNUM	HREG[13]	// 当前花样中正在运行的子花样的启始针编号
#define	F_PT_ENPATNUM	HREG[14]	// 当前花样中正在运行的子花样的结束针编号
#define	F_PT_STPATXPOS	HREG[15]	// 当前花样中正在运行的子花样的启始针位置地址X轴
#define	F_PT_STPATYPOS	HREG[16]	// 当前花样中正在运行的子花样的启始针位置地址Y轴
#define	F_PT_STPATPFPOS	HREG[17]	// 当前花样中正在运行的子花样的启始针位置地址中压脚
#define	F_PT_PAT2HDPOS	HREG[18]	// 当前花样中次原点位置
#define	F_PT_SUBSTATUS	HREG[19]	// 当前花样中正在运行的子花样的运行状态

#define	F_PT_SNO		HREG[20]	// 记存体编号
#define	F_PT_FNO		HREG[21]	// 文件编号			01-00
#define	F_PT_FN1		HREG[32]	// 文件名1			20-21
#define	F_PT_FN2		HREG[33]	// 文件名2			22-23
#define	F_PT_FN3		HREG[34]	// 文件名3			24-25
#define	F_PT_FN4		HREG[35]	// 文件名4			26-27
#define	F_PT_FN5		HREG[36]	// 文件名5
#define	F_PT_FN6		HREG[37]	// 文件名6
#define	F_PT_FN7		HREG[38]	// 文件名7
#define	F_PT_FN8		HREG[39]	// 文件名8
#define	F_PT_FN9		HREG[40]	// 文件名9
#define	F_PT_FN10		HREG[41]	// 文件名10
#define	F_PT_FN11		HREG[42]	// 文件名11
#define	F_PT_FN12		HREG[43]	// 文件名12
#define	F_PT_FN13		HREG[44]	// 文件名13
#define	F_PT_FN14		HREG[45]	// 文件名14
#define	F_PT_FN15		HREG[46]	// 文件名15
#define	F_PT_FN16		HREG[47]	// 文件名16
#define	F_PT_FN17		HREG[48]	// 文件名17
#define	F_PT_FN18		HREG[49]	// 文件名18
#define	F_PT_FN19		HREG[50]	// 文件名19
#define	F_PT_FN20		HREG[51]	// 文件名20
#define	F_PT_FN21		HREG[52]	// 文件名21
#define	F_PT_FN22		HREG[53]	// 文件名22
#define	F_PT_FN23		HREG[54]	// 文件名23
#define	F_PT_FN24		HREG[55]	// 文件名24
#define	F_PT_FN25		HREG[56]	// 文件名25
#define	F_PT_FN26		HREG[57]	// 文件名26
#define	F_PT_FN27		HREG[58]	// 文件名27
#define	F_PT_FN28		HREG[59]	// 文件名28
#define	F_PT_FN29		HREG[60]	// 文件名29
#define	F_PT_FN30		HREG[61]	// 文件名30
#define	F_PT_FN31		HREG[62]	// 文件名31
#define	F_PT_FN32		HREG[63]	// 文件名32

#define	F_PT_PATBYTESL	HREG[24]	// 针数据字节数
#define	F_PT_PATBYTESH	HREG[25]	// 针数据字节数
#define	F_PT_FEED_C		HREG[26]	// 当前步行位置														用
#define	F_PT_FEED_SUM	HREG[27]	// 花样总针数		09-08/4
#define	F_PT_FEED_PAT	HREG[28]	// 花样缝针数		09-08/4-(pat中不含01,21,41,61指令)
#define	F_PT_DRAW_ADD	HREG[29]	// 绘图起始位置		48h
#define	F_PT_LINE_ADD	HREG[30]	// 线条起始位置		05-04+40h
#define	F_PT_PAT_ADD	HREG[31]	// 针起始位置		05-04+09-08/2+40h


//-----线张力寄存器
#define		CLFDAT		0	//当前车缝压力值
#define		CLFNO		1	//当前车缝段号
#define		LFNOMAX		2	//当前花样有几段压力值
#define		LFNO0		3	//电子夹线压力寄存器被第几段使用
#define		LFNO1		4
#define		LFNO2		5
#define		LFNO3		6
#define		LFNO4		7
#define		LFNO5		8
#define		LFNO6		9
#define		LFNO7		10
#define		LFNO8		11
#define		LFNO9		12

#define		LFDAT0		13	//电子夹线压力值寄存器
#define		LFDAT1		14
#define		LFDAT2		15
#define		LFDAT3		16
#define		LFDAT4		17
#define		LFDAT5		18
#define		LFDAT6		19
#define		LFDAT7		20
#define		LFDAT8		21
#define		LFDAT9		22


#define		upcount_set_l		PREG[1018]		// 241*4+0		1020->[1018,1019]   2017-4-18
#define		upcount_set_h		PREG[1019]		// 241*4+0		1020->[1018,1019]   2017-4-18
#define		upcount_now_l		YREG[SYS_UPCOUNT]				//185->[183,184]
#define		upcount_now_h		YREG[SYS_UPCOUNT+1]	
#define		dncount_set_l		PREG[1020]		// 241*4+1		1021->[1020,1021]
#define		dncount_set_h		PREG[1021]		// 241*4+1		1021->[1020,1021]
#define		dncount_now_l		YREG[SYS_DNCOUNT]				//186->[185,186]		2017-4-18
#define		dncount_now_h		YREG[SYS_DNCOUNT+1]


#define		upcntonoff_flag		PREG[1022]		// 241*4+2
#define		dncntonoff_flag		PREG[1023]		// 241*4+3

//---------------说明：操作版通讯需将DREG_DISP改为DREG_OPRA------------------------------------------------------
#define		OPRA_OUTCND			OREG[19]						// 指令(主板发给操作版)(需要操作版在空闲时循环读该寄存器，读时将其清零)
#define		OPRA_COMMAND		OREG[20]						// 指令(操作版发给主板)
#define		OPRA_CMDSTATUS		OREG[21]						// 指令状态
#define		OPRA_CMDPARA1		OREG[22]						// 指令参数1
#define		OPRA_CMDPARA2		OREG[23]						// 指令参数2

#define		OPRA_DISPCOMMAND	OREG[25]						// 指令(打板器发指令给操作板（操作板被屏蔽）)
#define		OPRA_DISPCMDSTATUS	OREG[26]						// 指令状态
#define		OPRA_DISPCMDPARA1	OREG[27]						// 指令参数1
#define		OPRA_DISPCMDPARA2	OREG[28]						// 指令参数2
//---------------说明：操作版通讯需将DREG_DISP改为DREG_OPRA------------------------------------------------------

#define 	C_FREE				0			// 通讯空闲
#define 	C_BUSY				1			// 通讯忙
#define 	C_ERROR				2			// 通讯错误
#define 	C_LIMIT				3			// 通讯错误
#define 	C_NOFLASH   		10			// 没有优盘	
#define 	C_NOFILE			11			// 没有文件	
#define 	C_EORFILE			12			// 文件数据已坏	
#define 	C_CLETIME			99			// 清除主机等待时间	
#define 	C_SUCCEED			100			// 指令执行完成（成功）

#define		sreg_total		HREG[11]		// 
#define		sreg_now		HREG[12]

//参数修正寄存器
//#define	SYS_PARA1CHKSUM	DREG[DREG_SYS+60]	// 针杆旋转修正参数校验
//#define	SYS_PARA1X1		DREG[DREG_SYS+61]	// 针杆旋转修正参数_90度修正坐标X
//#define	SYS_PARA1Y1		DREG[DREG_SYS+62]	// 针杆旋转修正参数_90度修正坐标Y
//#define	SYS_PARA1X2		DREG[DREG_SYS+63]	// 针杆旋转修正参数_180度修正坐标X
//#define	SYS_PARA1Y2		DREG[DREG_SYS+64]	// 针杆旋转修正参数_180度修正坐标Y
//#define	SYS_PARA1X3		DREG[DREG_SYS+65]	// 针杆旋转修正参数_270度修正坐标X
//#define	SYS_PARA1Y3		DREG[DREG_SYS+66]	// 针杆旋转修正参数_270度修正坐标Y
//#define	SYS_PARA1XR		DREG[DREG_SYS+67]	// 针杆旋转修正参数_圆心坐标X
//#define	SYS_PARA1YR		DREG[DREG_SYS+68]	// 针杆旋转修正参数_圆心坐标Y
//#define	SYS_PARA1RD		DREG[DREG_SYS+69]	// 针杆旋转修正参数_半径
//#define	SYS_PARA1ANG	DREG[DREG_SYS+70]	// 针杆旋转修正参数_0度位置角度
//#define	SYS_PARA2CHKSUM	DREG[DREG_SYS+71]	// 机械原点修正参数校验
//#define	SYS_PARA2XORG	DREG[DREG_SYS+72]	// 机械原点修正参数_修正坐标X
//#define	SYS_PARA2YORG	DREG[DREG_SYS+73]	// 机械原点修正参数_修正坐标Y
//#define	SYS_PARA3CHKSUM	DREG[DREG_SYS+74]	// 正反转间隙修正参数校验
//#define	SYS_PARA3XZADJ	DREG[DREG_SYS+75]	// 正反转间隙修正参数_修正坐标X+
//#define	SYS_PARA3XFADJ	DREG[DREG_SYS+76]	// 正反转间隙修正参数_修正坐标X-
//#define	SYS_PARA3YZADJ	DREG[DREG_SYS+77]	// 正反转间隙修正参数_修正坐标Y+
//#define	SYS_PARA3YFADJ	DREG[DREG_SYS+78]	// 正反转间隙修正参数_修正坐标Y-

extern const volatile S16 PROMG_DATA[PROMGMAX][3];

extern volatile U16 EEPREG[];
extern volatile U16 DREG[MAXDREG];		// 状态数据
extern volatile U16 MREG[MAXMREG];		// 命令数据
extern volatile U16 SREG[MAXSREG];		// 花样数据
extern volatile U16 EREG[MAXEREG];		// 错误信息
extern volatile U16 YREG[MAXYREG];		// 系统数据
extern volatile U16 PREG[MAXPREG];		// 参数数据
extern volatile U16 HREG[MAXHREG];		// 当前花样文件信息
//extern volatile U16 IREG[MAXIREG];		// 花样文件索引表
extern volatile U16 IREG2[MAXIREG2];		// 花样文件索引表2 
extern volatile U16 NEWSREG[MAXSREG2];
extern volatile S16 PATCRRRD[MAXCOORD][2];
extern volatile S16 PATCRRRD1[MAXCOORD][2];		// 暂存新花样绝对坐标缓冲
extern volatile U16 INPOINTPOS[MAXCOORD];		// 拐点位置
extern volatile U16 UREG[MAXUREG];
extern volatile U16 VREG[MAXVREG];
extern volatile U16 GREG[MAXGREG];      // 组合花样列表
extern volatile U16 LREG[MAXLREG];
extern volatile U16 BLKSREG[MAXBLKSREG];
extern volatile U16 BLKHREG[MAXBLKHREG];
extern volatile U16 BLKIREG[MAXBLKIREG];
extern volatile U16 BLKIND[MAXBLIND];						// 备份数据缓冲索引	
extern volatile U16 BLKBUF[MAXBLKBUF];	// 备份数据缓冲
extern volatile U8 AVI_VIDEO_BUF[AVI_VIDEO_BUF_SIZE];
extern volatile U8 AVI_AUDIO_BUF[AVI_AUDIO_BUF_SIZE];

extern volatile U16 OREG[MAXOREG];
//extern volatile U16 DIR_DREG[MAXDIR];
extern volatile U16 WREG[MAXWREG];
extern volatile U16 IOREG[MAXIOREG];
extern volatile U16 TMP_SREG[MAXTSREG];
extern volatile U16 IAPBuff[MAXTIAPBUFF];

//extern volatile U16 WAVEREG[MAXWAVE];

extern void Reg_Init(void);		
//extern U32 BlkAddr[100];


#define 	bit0			0x00000001
#define 	bit1			0x00000002
#define 	bit2			0x00000004
#define 	bit3			0x00000008
#define 	bit4			0x00000010
#define 	bit5			0x00000020
#define 	bit6			0x00000040
#define 	bit7			0x00000080
#define 	bit8			0x00000100
#define 	bit9			0x00000200
#define 	bit10			0x00000400
#define 	bit11			0x00000800
#define 	bit12			0x00001000
#define 	bit13			0x00002000
#define 	bit14			0x00004000
#define 	bit15			0x00008000
#define 	bit16			0x00010000
#define 	bit17			0x00020000
#define 	bit18			0x00040000
#define 	bit19			0x00080000
#define 	bit20			0x00100000
#define 	bit21			0x00200000
#define 	bit22			0x00400000
#define 	bit23			0x00800000
#define 	bit24			0x01000000
#define 	bit25			0x02000000
#define 	bit26			0x04000000
#define 	bit27			0x08000000
#define 	bit28			0x10000000
#define 	bit29			0x20000000
#define 	bit30			0x40000000
#define 	bit31			0x80000000

#endif
