//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  main.c									*
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
//*															*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#include "systemMain.h"
#include "main.h"
#include "utility.h"
#include "lcddisp.h"
#include "register.h"
#include "menu.h"
#include "color.h"
#include "communication.h"
#include "touch.h"
#include "vs1003.h"
#include "w25qxx.h"
#include "S39vf160.h"
#include "aviplayer.h"
#include "ds2460.h"

U16 SYSDAT[0x40] __attribute__((at(0x20000000)));

U8 CPUID[12];
U32 SYSSETID;

#define TLFNADD_TOUCHSET 224

const U16 draw_code[][2] = {{0, 0}, {0x501, 0}, {0xb01, 0}, {0xc01, 0}, {0xd01, 0}, {0x1d01, 0}, {0x1d01, 1}, {0x1d01, 2}, {0x1d01, 3}, {0x401, 0}, {0x1401, 0}, {0x201, 0}, {0x701, 0}, {0x601, 0}, {0, 0}, {0x1c01, 0}, {0x1c01, 3}, {0x1c01, 1}, {0x1c01, 2}};
const U16 pat_code[] = {0, 0x5, 0xb, 0xc, 0xd, 0x1d, 0x11d, 0x21d, 0x31d, 0x4, 0x14, 0x2, 0x7, 0x6, 0, 0x1c, 0x031c, 0x011c, 0x021c};

//const U16 PC_bmp[16] = {470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485};	//指令功能图片
SystemTime g_SysTimes;
ProInfo g_ProInfo;//flash内容版本号

#if USEMULTCOLOR //彩色
const U8 mCOLOR = 1;
#else
const U8 mCOLOR = 0;
#endif

volatile U8 g_usbmount;	   //USB挂载标记
volatile U8 g_fbmac = 0;   //折边器
volatile U8 g_disk_memory; //初始化存储器标志 0-FLASHl;1-U盘;2-SD
volatile U8 buttonflag[6]; // 用于设定按键是否显示标志- 右下角的几个按键使用
volatile U16 g_Fileno;		//花样文件序号
volatile U8 g_GroupName[9];	  //组合花样的名称
volatile U8 g_GroupType;	  //1-组合模式;2-接合模式
volatile U8 g_ImgMove;		  //图像拖动功能？？？
volatile U8 g_NoReadPatf = 0; //不读取花样标志  1-不读取花样数据 2-强制重读一次花样数据
volatile U16 g_draw_p;//绘图起始地址
volatile U16 g_line_p;//线条起始地址
volatile U32 g_pat_p;//针点起始地址
volatile U16 g_line_sum;//线条总数
volatile U8 g_BarcodeFlag[MAXFILENO - MINFILENO];
volatile U8 g_jogfunction;//跳针设置-跳针功能输出
volatile U8 g_jogfunction1;
volatile U16 *g_CodeNameAdd; //附加功能名称
volatile U8 g_sysType;		 //0-M 1-H
volatile U8 g_runstepf;		//屏幕相关设置界面的 内部界面序号
select_id_message g_SelectNo[SELECTNO_MAX]; //被选中的花样文件菜单信息
volatile U8 g_checkGroup;					//组合图形检查
volatile U16 g_Groupsel1;
volatile U16 g_Combinepat[GroupMax][6]; //组合图形选择花样-缓存
volatile U8 g_GroupSave;				//保存组合图形 0-不保存  1-保存到内存  2-保存到U盘 3-保存到SD卡 4-保存到默认位置
volatile U8 g_GroupCover;				//组合图形覆盖标志
volatile U8 g_GroupDoingNO = 0;				//组合图形正在操作编号
volatile U8 g_GroupMenth;				//当前组合图形操作类型  0-  1-保存  2-读取   3-清除当前组合图型
volatile U8 g_GroupPatNums;				//读取组合文件数
volatile U8 g_tmpGroName[10];
volatile U8 *g_tGname;
volatile U8 g_KeyLockStaus = 1;		//按键锁标志位

volatile U16 g_MachineTy;

volatile S16 g_PinModParaANG; // 针杆旋转修正参数_0度位置角度

volatile S16 g_PinModChkSum;  // 针杆旋转修正参数校验值
volatile S16 g_PinModParaX1;  // 针杆旋转修正参数_90度修正坐标X
volatile S16 g_PinModParaY1;  // 针杆旋转修正参数_180度修正坐标Y
volatile S16 g_PinModParaX2;  // 针杆旋转修正参数_180度修正坐标X
volatile S16 g_PinModParaY2;  // 针杆旋转修正参数_180度修正坐标Y
volatile S16 g_PinModParaX3;  // 针杆旋转修正参数_270度修正坐标X
volatile S16 g_PinModParaY3;  // 针杆旋转修正参数_270度修正坐标Y
volatile S16 g_PinModParaXR;  // 针杆旋转修正参数_圆心坐标X
volatile S16 g_PinModParaYR;  // 针杆旋转修正参数_圆心坐标Y
volatile S16 g_PinModParaRD;  // 针杆旋转修正参数_半径
volatile S16 g_PinModParaANG; // 针杆旋转修正参数_0度位置角度

volatile S16 g_ViewModParaX; //视觉坐标系偏移X
volatile S16 g_ViewModParaY; //视觉坐标系偏移Y

volatile S16 g_OrgModChkSum; // 机械原点修正参数校验
volatile S16 g_OrgModParaX;	 // 机械原点修正参数_修正坐标X
volatile S16 g_OrgModParaY;	 // 机械原点修正参数_修正坐标Y
volatile S16 g_OrgModParaZ;	 // 机械原点修正参数_修正坐标Z

volatile S16 g_OrgModParaXD; // 机械原点D轴修正参数_修正坐标X
volatile S16 g_OrgModParaYD; // 机械原点D轴修正参数_修正坐标Y
volatile U8 g_ModHome = 0;	 //0-修正M轴原点 1-修正D轴原点

volatile S16 g_GapModChkSum;  // 正反转间隙修正参数校验
volatile S16 g_GapModPosadjX; //正反转间隙修正参数_修正坐标X+
volatile S16 g_GapModNegadjX; //正反转间隙修正参数_修正坐标X-
volatile S16 g_GapModPosadjY; //正反转间隙修正参数_修正坐标Y+
volatile S16 g_GapModNegadjY; //正反转间隙修正参数_修正坐标Y-

volatile S16 g_upDISP;	 // 升级打板器 - 0:不升级 1:升级
volatile S16 g_upFLASH;	 // 升级打板器 FLASH  - 0:不升级 1:升级 2:AUTO
volatile S16 g_upMBOARD; // 升级主板 - 0:不升级 1:升级
volatile S16 g_upWIFI;	 // 升级wifi模块 - 0:不升级 1:升级 2:AUTO

volatile OVLP g_autoOvlp; //区段追加重叠缝参数

volatile U8 g_ExCode = 0; //表示附加功能输出状态
volatile U8 g_tExCode = 0;
volatile U8 g_curExCode = 0;
volatile U8 g_CodeEn = 1;//修改 新增 显示代码使能（预览功能有效）

volatile U8 g_ImgPoint = 0;//寸动标志位，1：支持寸动, 选中点支持
volatile U8 g_ImgMove = 0;//快速移动标志位 1：支持快速移动
volatile U16 g_LPoint; //定位点
volatile U8 g_datum_mark;

volatile S16 g_disxpos, g_disypos; //x,y坐标实际值
volatile S16 g_dxpos, g_dypos;	   //x,y坐标变化值
volatile S16 g_tdxpos, g_tdypos;   //x,y坐标变化值缓存
volatile S16 g_tmpox = 0, g_tmpoy = 0;
volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //原点坐标-花样坐标系
volatile S16 g_tmpoxpos, g_tmpoypos;//记录光标移动时，当前点的坐标
volatile S16 g_temp_oxpos, g_temp_oypos;

volatile U32 g_feedc; //当前针
volatile U32 g_max_feedsum;//跳转点最大数量
volatile U32 g_min_feedsum;;//跳转点最小数量
volatile U32 g_jog_stpoint;//跳转起始点
volatile U32 g_jog_enpoint;//跳转结束点
volatile U32 g_jog_point;//当前跳转点
volatile U16 g_point_num; //控制点数
volatile U8 g_sta_end_flag;//跳转开始，结束标志位 0：未选择起点，未选择终点，1：选中起点，未选择终点
volatile U8 g_jogmode;//选段界面的选择跳针基准模式 0:JOG_ON (每点根据选中点的相对坐标)1:JOG_OFF(每点为起点的相对坐标)
volatile S16 g_maxX;
volatile S16 g_minX;
volatile S16 g_maxY;
volatile S16 g_minY;
volatile U32 g_sewPTlen; //sew花样数据(花样数据区段)大小8b

volatile U8 g_BorAddFuns; //插入附加功能 每一个位代表一个功能
volatile U8 g_BorDelFuns; //删除附加功能 每一个位代表一个功能
volatile U16 g_pinAng;
volatile U16 g_curPinAng = 0;	  //当前状态针角度值
volatile U16 g_pointPinAng = 0;	  //针数对应角度值
volatile U16 g_curSetPara1 = 0;	  //当前状态设置一参数
volatile U16 g_pointSetPara1 = 0; //针数对应设置二参数值
volatile U16 g_curSetPara2 = 0;	  //当前状态设置二参数
volatile U16 g_pointSetPara2 = 0; //针数对应设置二参数值
volatile U16 g_AddRela;			  //三菱格式转为针相对追加

volatile S16 g_pf_bdis;
volatile S8 g_pf_dis;
volatile U8 g_speed;
volatile U8 g_oldspeed;
volatile U8 g_input_method; //输入模式 0-空送 1-点 2-直线 3-折线 4-圆 5-圆弧 6-曲线
volatile U8 g_para_offset;	//多重缝 0-无多重缝 1-同向空送多重缝 2-逆向空送多重缝 3-离边 4-同向缝纫多重缝 5-逆向缝纫多重缝
volatile U8 g_direction;	//多重缝方向
volatile U8 g_distance;		//多重缝距离
volatile U8 g_times;		//多重缝次数

volatile U16 g_temp_draw_p;
volatile U16 g_temp_line_p;
volatile U32 g_temp_pat_p;
volatile U16 g_temp_point_num;
volatile U8 g_MboardUSB; //主板U盘存储器插入标志 0：未插入  1：插入

volatile U16 g_Ptya;		   //寸动调速值	2010.4.6
volatile U8 g_pitch = 30;	   // 打版时输入针距0-12.7		20151211
volatile U32 g_ct_status_flag; //打板状态标志
volatile U8 g_ct_insert_flag;  //打板插入标志 0:无插入，1:可插入 2:正在插入
volatile U8 g_feed_flag;	   //打板空送标志
volatile U32 g_feedsum;		   //总针
volatile U32 g_feedpat;			
volatile U8 g_bt_mode;	//倒缝模式
volatile U8 g_repeat;	//重叠缝次数
volatile U8 g_r_stitch; //重叠缝针数
volatile U8 g_srt_bt;	//前端倒缝次数
volatile U8 g_s_stitch; //前端倒缝针数
volatile U8 g_ent_bt;	//后端倒缝次数
volatile U8 g_e_stitch; //后端倒缝针数
volatile U8 g_zrota;

volatile U8 g_zig_zag;		 //Z字缝
volatile U8 g_zig_width;	 //Z缝宽度
volatile U8 g_zig_pitch;	 //Z缝针距
volatile U8 g_zig_direction; //Z缝方向

volatile U8 g_dopfoot = 0;
volatile U8 g_DoTrim;
volatile U8 g_lineforceValSetNow; // 当前缝纫面线张力值
volatile U8 g_lineforceValSet;	  // 面线张力基准值
volatile U8 g_DelAll_flag = 0, g_DelAll_flag1 = 0;
volatile U16 g_EndbtNum = 0;
volatile U16 g_btNumber = 0;
volatile U8 g_ModifyDrawFlag = 0;
volatile U8 g_ModifyMode = 0xff;
volatile U16 g_savetimes; //备份数据次数
volatile U16 g_Tsavetimes;
volatile U8 g_code_mode;
volatile U8 g_feed_sew; // 前一针的空送或缝纫标志
volatile U8 g_originf;	// 花样结束点位置标志 0：不在原点位置 1: 在原点位置
volatile S8 g_lf_dis_modify;
volatile S8 g_pf_dis_modify;
volatile U8 g_mitdotack;

volatile U8 g_patname[64];//输入花样的名称
volatile U16 g_pattno;
volatile U32 g_modaddr[30] = {0};
volatile U8 g_modify_mode;
volatile U8 g_modify_method;
volatile U8 g_modify_gohome; //0: 直接式空送回原点 1: 添加式空送回原点
volatile U8 g_modify_code;
volatile U8 g_modifyflag;
volatile U8 g_modifyflag1;
volatile U8 g_modifyf;

volatile U8 g_overflag11 = 0; // 判断修改时是否已经大于存储范围的标志
volatile U8 g_num_mov_flag; //0 :无输入 1:方向输入 2:数字输入 3:待完成界面
volatile U16 g_stitch_number = 1;

volatile S16 g_pf_absval;  //中压脚当前绝对值(包括基准值)
volatile U8 g_pf_mode = 0; //中压脚	1:有设置 0：无设置
volatile U8 g_pf_modify_mode;
volatile U16 g_pitch_parameter = 30; // 保存修改过的针距

volatile U16 g_convert_xscale;//变换-缩放-X倍率
volatile U16 g_convert_yscale;//变换-缩放-Y倍率
volatile U16 g_convert_pindis;//变换-缩放-针距

volatile U16 g_parameter;
volatile U8 g_selectmode;

volatile U16 g_mode1dat[3] = {0};
volatile S16 g_mode2dat[5] = {0};
volatile U8 g_modedat = 0;

volatile U8 g_num_mov_key;//0:游标;1:数字
volatile U8 g_editflag;
volatile U8 g_reorigin; // 自动返回原点命令 0：不自动反回原点 1：自动反回原点

volatile U8 g_wifiname[21] = {0};
volatile U8 g_nameidex = 0;
volatile U8 g_ssid[20] = {0};
volatile U8 g_ssididex = 0;
volatile U8 g_password[40] = {0};
volatile U8 g_passwordidex = 0;
volatile U8 g_mac[6] = {0};
volatile U32 g_id = 0;

volatile U8 g_blkInset;
volatile U8 g_nolimt = 0;
volatile U32 g_blkInsetlen;
volatile U16 g_ctAddpins = 0;
volatile U8 g_ModToCt = 0; //图形修改转图形设计界面(区段插入)

volatile U8 g_NoUpdataoldno = 0; //	1-不更新旧花样编号

volatile S16 g_progParas[PROMGMAX]; //编程器参数设定

//volatile char  usbflag;
//volatile U32 buff[4096];
extern volatile U8 g_NoReadPatf;
extern volatile U8 g_NO_ReagIOf; //不读取IO参数标志
extern volatile U8 gEnverify;	 //使能加密校验功能
extern volatile U32 gverTim, gverTimes;
extern volatile U32 mp3dver;
extern volatile U16 g_Fileno;
extern volatile U8 g_runstepf;
extern volatile U8 g_init_flag;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern const U16 PARA_DATA[];
extern volatile U32 g_light_c;
extern volatile U8 g_GroupName[9];
extern volatile U8 g_GroupType;
extern volatile U8 gPeessVal;
extern BOOTSCR g_bootscr;
extern volatile U8 g_upinfo[0x20];

extern void strcopy(U8 *src, U8 *des, U16 len);
extern void BackupIREG(void);
MachineInfo g_Mac; //机械XY轴限制信息
volatile U8 g_netSelectMac;
U8 gBatType; //打板类型 1：三菱  2：兄弟
U32 gpos, list;
volatile U8 g_usersave;								//当前登录状态的用户编号
volatile U8 usepageno;								// 换屏用参数，当前用地址0显示，换屏时写地址1，写完后再换到地址1显示(在地址0与地址1中循环使用)
volatile U16 pno;									//当前需要显示的页码序号
volatile U16 plastno;								//上一个显示页码序号
volatile U16 pnoindex, tmppnoindex;					//缓存当前页面组索引
volatile U16 poldno[10];							//缓存上一个页面组
volatile U16 hleppara, g_para0, g_para1, g_imgpara; //实现页面跳转子类型的标志位
volatile U32 g_temp_pno = 0xffffffff;
volatile U32 g_pno_temp = 0;
volatile U8 key_on_flag1;	   //连续功能按键按下标志
volatile U8 g_data_flash;	   //花样数据更新
volatile U8 coldstart;		   //冷启动热启动标志
volatile U8 g_UdZhouFlag = 0;  //分离轴标志 1:0x5a;2:0xa5;3:0x55
volatile U16 g_selectdisno;	   //当前选中的花样序号
volatile U16 g_oldselectdisno; //前一个选中的花样序号
volatile U8 g_uperr = 0;	   //升级失败标志位
volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos, g_wabspos;
volatile S16 g_pf_absdis;
volatile U16 g_limtdang;
volatile U8 usbflag = 0;
volatile U16 g_fno = 1;				//花样编号
volatile U16 g_Fmax = 1;		//花样文件最大数据 min：1
volatile U8 g_Funtype;			  //功能类型  1:打板  2:变换   (针对倒针功能)
volatile U8 g_Restorepropara = 0; //还原修改参数 0:未还原  1:设置还原标志位  2:设置还原成功标志位
volatile U16 totalfiles;
volatile U32 g_recv_netdata = 0; //接收网络数据标志
volatile U32 g_bmplen = 0;		 //bmp图片总大小
volatile U8 g_logdir = 0;		 //logo的gif初始显示地址
volatile U8 gplayLogoAvi = 0;
const unsigned int g_array1[] = {0, 0x800, 0x1000, 0x1800, 0x8800, 0x9000};
volatile U16 g_array2[307][2];
volatile S32 g_array3[2][2];  //记录当前点的绝对坐标-花样坐标系
volatile U16 g_array4[386][2];//记录控制点的绝对坐标-花样坐标系
volatile U16 g_array5[630][2];//记录控制点的绝对坐标-花样坐标系
volatile U16 g_array6[302];		// 用于曲线修改时保存当前重(有多重缝)曲线每个型值点间针数
volatile S16 g_array10[630][2]; //保存区段修正时输入点数据,用于图形范围显示
//volatile S32 g_array10[630][2];	//保存区段修正时输入点数据,用于图形范围显示
//#ifdef SYSTEM2
//volatile S32 g_array10[630][2];
//#else
//volatile S16 g_array10[630][2];
//#endif
volatile S16 g_array11[13][2]; //暂存曲线多重缝每重的针数，用于图形显示
volatile U16 g_array12[120];
volatile S32 g_array33[2][2];
volatile U16 g_temp_array2[307][2];
volatile U16 g_publicbuf[3000];// 0-256:PREG的当前值;256-512:PREG的当前修改;600-856:PREG修改过的参数序号
volatile U32 g_tmp_list[480][3]; //保存曲线针数据参数
volatile U8 g_changeSys;		 //切换系统
volatile U8 g_SysErr;			 //系统错误
volatile U8 g_zhouDown = 0;		 //机头下降
THDTSN g_tdtensiond;			 // 电子夹线器参数
PRFOOT g_pfootd;				 // 中压脚参数
PATHEAD g_patd;					 // 花样文件头部构造体
OLDPAT g_dispat;				 //上一针？？
PassWordPara g_user;      //权限登录等级
volatile U16 g_usblogin;  //USB管理登录信息
volatile U8 usepageno;	  // 换屏用参数，当前用地址0显示，换屏时写地址1，写完后再换到地址1显示(在地址0与地址1中循环使用)
volatile ERRMSG g_errMsg; // 报警信息记录

void Menu_Start(void);
void Menu_DesktopInit(void);
void Menu_StReadData(void);
void CheckTouchPara(void);
void Init_SysIfo(void);
extern U8 isUpInfoData(U8 *dat);
extern volatile U8 g_vs1003err;
//------------------------------------------
// Function   : int main(void)
// Description: 主程序
//------------------------------------------
int main(void)
{
	U32 i;
	//	U16 flasherr = 0;
	//	U16 mp3err = 0;
	//	U8 defver[3] = {0x30,0x30,0x30};		//更新失败时将版本号设置为0
	g_usbmount = 0;
	g_usblogin = 0;
	gplayLogoAvi = 0;
	g_changeSys = 0;
	g_SysErr = 0;
	system_init();

	Reg_Init();
	wdt();
	if ((RCC->CSR & 0x08000000) || (SYSDAT[0] == 0x5a))
	{ // 上电复位
	}
	else if ((RCC->CSR & 0x04000000) || (SYSDAT[0] == 0xa5))
	{ // 外部复位
	}
	else
	{ // 软件跳转
	}
	RCC->CSR = 0x01000000; //状态控制
	SYSDAT[0] = 0;		   //复位标志
	SYSDAT[4] = 0;

	coldstart = 1;
	//	Read_D(D_Address[PARA]+(4)*151+3,1,(U16 *)(&P_CNR));
	//	if(P_CNR<10||P_CNR>100) P_CNR = 40;
	SetLcdPwm(1, P_CNR);

	getBootScreenMxg();
	Menu_Start();
	initflashadd(); //初始化flash数据的所有，存储于SDRAM中

	//清除历史报警信息
	g_errMsg.max = 0;
	for (i = 0; i < ERRMSGMAX; i++)
	{
		g_errMsg.err[i] = 0xffff;
	}
	SETTFCS(); //TF卡使能标志位-置位
	CLRTFCS();	//TF卡使能标志位-清零

	//编程参数初始化
	for (i = 0; i < PROMGMAX; i++)
	{
		g_progParas[i] = 0;
	}

#if USEMULTCOLOR //彩色
	g_fbmac = 1; //折边器	 ？？
#else
	Read_D(D_Address[SYS] + SYS_MACHINE_TYPE, 1, (U16 *)(&YREG[SYS_MACHINE_TYPE]));
	g_fbmac = YREG[SYS_MACHINE_TYPE] == 10040 ? 1 : 0;
#endif



	systemApp();
}

// 读取用户信息
void readUsrMsg()
{
	U32 i;
	U16 checksum;
	U8 errflag = 1;
	Read_D(D_Address[USERLOG], 123, (U16 *)(&UREG)); //读取用户名

	checksum = 0;
	for (i = 0; i < 123; i++)//总共246字节
	{
		if (UREG[i] != 0)
			errflag = 0; //数据全为0 ，用户信息错误
		if (i > 2)
		{
			checksum += UREG[i];
		}
	}


	if ((checksum == UREG[2]) && (errflag == 0))
	{
		strcopy((U8 *)(&UREG), (U8 *)&(g_user), 4);//当前用户状态
		strcopy((U8 *)(&UREG[3]), (U8 *)((U8 *)(&(g_user)) + 4), 240);//用户密码与用户名组
	}
	else//校验不通过，使用系统默认用户
	{ 
		g_user.newuser = 0;
		g_user.newmode = 0;
		g_user.olduser = 0;
		g_user.oldmode = 0;
		g_user.users[1].name[0] = 0x55;
		g_user.users[1].name[1] = 0x53;
		g_user.users[1].name[2] = 0x45;
		g_user.users[1].name[3] = 0x52;
		g_user.users[1].name[4] = 0x31;
		g_user.users[1].name[5] = 0x00;
		g_user.users[1].password[0] = 0x31;
		g_user.users[1].password[1] = 0x30;
		g_user.users[1].password[2] = 0x31;
		g_user.users[1].password[3] = 0x30;
		g_user.users[1].password[4] = 0x00;
		g_user.users[2].name[0] = 0x55;
		g_user.users[2].name[1] = 0x53;
		g_user.users[2].name[2] = 0x45;
		g_user.users[2].name[3] = 0x52;
		g_user.users[2].name[4] = 0x32;
		g_user.users[2].name[5] = 0x00;
		g_user.users[2].password[0] = 0x32;
		g_user.users[2].password[1] = 0x30;
		g_user.users[2].password[2] = 0x32;
		g_user.users[2].password[3] = 0x30;
		g_user.users[2].password[4] = 0x00;
		g_user.users[3].name[0] = 0x55;
		g_user.users[3].name[1] = 0x53;
		g_user.users[3].name[2] = 0x45;
		g_user.users[3].name[3] = 0x52;
		g_user.users[3].name[4] = 0x33;
		g_user.users[3].name[5] = 0x00;
		g_user.users[3].password[0] = 0x33;
		g_user.users[3].password[1] = 0x30;
		g_user.users[3].password[2] = 0x33;
		g_user.users[3].password[3] = 0x30;
		g_user.users[3].password[4] = 0x00;
		g_user.users[4].name[0] = 0x55;
		g_user.users[4].name[1] = 0x53;
		g_user.users[4].name[2] = 0x45;
		g_user.users[4].name[3] = 0x52;
		g_user.users[4].name[4] = 0x34;
		g_user.users[4].name[5] = 0x00;
		g_user.users[4].password[0] = 0x34;
		g_user.users[4].password[1] = 0x30;
		g_user.users[4].password[2] = 0x34;
		g_user.users[4].password[3] = 0x30;
		g_user.users[4].password[4] = 0x00;
		g_user.users[5].name[0] = 0x55;
		g_user.users[5].name[1] = 0x53;
		g_user.users[5].name[2] = 0x45;
		g_user.users[5].name[3] = 0x52;
		g_user.users[5].name[4] = 0x35;
		g_user.users[5].name[5] = 0x00;
		g_user.users[5].password[0] = 0x35;
		g_user.users[5].password[1] = 0x30;
		g_user.users[5].password[2] = 0x35;
		g_user.users[5].password[3] = 0x30;
		g_user.users[5].password[4] = 0x00;
		g_user.users[6].name[0] = 0x55;
		g_user.users[6].name[1] = 0x53;
		g_user.users[6].name[2] = 0x45;
		g_user.users[6].name[3] = 0x52;
		g_user.users[6].name[4] = 0x36;
		g_user.users[6].name[5] = 0x00;
		g_user.users[6].password[0] = 0x36;
		g_user.users[6].password[1] = 0x30;
		g_user.users[6].password[2] = 0x36;
		g_user.users[6].password[3] = 0x30;
		g_user.users[6].password[4] = 0x00;
		g_user.users[7].name[0] = 0x55;
		g_user.users[7].name[1] = 0x53;
		g_user.users[7].name[2] = 0x45;
		g_user.users[7].name[3] = 0x52;
		g_user.users[7].name[4] = 0x37;
		g_user.users[7].name[5] = 0x00;
		g_user.users[7].password[0] = 0x37;
		g_user.users[7].password[1] = 0x30;
		g_user.users[7].password[2] = 0x37;
		g_user.users[7].password[3] = 0x30;
		g_user.users[7].password[4] = 0x00;
		g_user.users[8].name[0] = 0x55;
		g_user.users[8].name[1] = 0x53;
		g_user.users[8].name[2] = 0x45;
		g_user.users[8].name[3] = 0x52;
		g_user.users[8].name[4] = 0x38;
		g_user.users[8].name[5] = 0x00;
		g_user.users[8].password[0] = 0x38;
		g_user.users[8].password[1] = 0x30;
		g_user.users[8].password[2] = 0x38;
		g_user.users[8].password[3] = 0x30;
		g_user.users[8].password[4] = 0x00;
		g_user.users[9].name[0] = 0x55;
		g_user.users[9].name[1] = 0x53;
		g_user.users[9].name[2] = 0x45;
		g_user.users[9].name[3] = 0x52;
		g_user.users[9].name[4] = 0x39;
		g_user.users[9].name[5] = 0x00;
		g_user.users[9].password[0] = 0x39;
		g_user.users[9].password[1] = 0x30;
		g_user.users[9].password[2] = 0x39;
		g_user.users[9].password[3] = 0x30;
		g_user.users[9].password[4] = 0x00;
	}


	if ((g_usblogin & 0xff00) == 0xa500) //USB登录用户
	{
		g_user.newuser = g_usblogin & 0xff;
		if (g_user.newuser > 9)
		{
			g_user.newuser = 0;
		}
		g_user.olduser = g_user.newuser;
		g_user.newmode = 0;
		g_user.oldmode = 0;
	}
	userLoninTime = 0;
}

//初始化系统信息
void Init_SysIfo(void)
{
	readUsrMsg();

	//Read_D(D_Address[IOPARA],338,(U16 *)(&IOREG));		//读取IO参数

	//		if(Write_Cmd(C_READGROUP, 0, 0)==0){		//发送读取组合图形指令
	//			pno = MENU_RWHELP;hleppara = 16;
	//		}
	//		if(Read_Status()==0){
	//			pno = MENU_RWHELP;hleppara = 15;
	//		}else if(DREG[D_CMDSTATUS]!=100){
	//			#if Debug
	//			printf("DREG[D_CMDSTATUS] = %d \r\n",DREG[D_CMDSTATUS]);
	//			#endif
	//			pno = MENU_RWHELP;hleppara = 15;
	//		}
	Read_D(D_Address[GROUP], MAXGREG, (U16 *)(&GREG)); //读取组合图形数据

	g_GroupName[0] = GREG[GroupPNameAddr] & 0xff;
	g_GroupName[1] = (GREG[GroupPNameAddr] >> 8) & 0xff;
	g_GroupName[2] = GREG[GroupPNameAddr + 1] & 0xff;
	g_GroupName[3] = (GREG[GroupPNameAddr + 1] >> 8) & 0xff;
	g_GroupName[4] = GREG[GroupPNameAddr + 2] & 0xff;
	g_GroupName[5] = (GREG[GroupPNameAddr + 2] >> 8) & 0xff;
	g_GroupName[6] = GREG[GroupPNameAddr + 3] & 0xff;
	g_GroupName[7] = (GREG[GroupPNameAddr + 3] >> 8) & 0xff;

	g_GroupName[8] = 0;

	Read_D(D_Address[SYS] + SYS_GROUP_TYPE, 3, (U16 *)(&YREG[SYS_GROUP_TYPE])); //
	if (((YREG[SYS_GROUP_TYPE] > 0) && (YREG[SYS_GROUP_TYPE] < 41)) || (YREG[SYS_GROUP_TYPE] == 100))
	{
		g_GroupType = (U8)GREG[GroupPTypeAddr];
	}
	else
	{
		g_GroupType = 0;
	}
}

//编程器参数校验
void checkProgParas(S16 *paras, S16 min, S16 max, S16 def)
{
	if ((*paras) < min || (*paras) > max)
	{
		(*paras) = def;
	}
}
//初始化编程器设定参数
void initProgParas(void)
{
	U16 i;
	ReadPromgParas((U16 *)g_progParas);
	for (i = 0; i < PROMGMAX; i++)
	{
		checkProgParas((S16 *)&g_progParas[i], PROMG_DATA[i][0], PROMG_DATA[i][1], PROMG_DATA[i][2]);
	}
}
extern volatile U32 BufAdd;

void displayBootScreen(U8 mode, U16 flag)
{

#if LCD_TYPE == LCD_7cun

	uint32_t i, j;
	uint32_t DisplayAddr = LCD_VRAM_BASE_ADDR + 1 * DISP_PAGE_SIZE;
	uint16_t *p16;
	uint32_t f;

	gplayLogoAvi = 1;
	if (mode == 3)
	{ //用户自定义开机动画 avi
		f = Check_AviFile((char *)"logo.avi");
		if (f)
		{			  //视频动画错误
			mode = 1; //缝机图片开机画面
			g_bootscr.mode = 1;
		}
	}
	switch (mode)
	{
	case 0: //名菱开机画面
		//修改begin
		if (flag == 0x0)
		{
			DrawFBMP2(80, LCD_HEIGHT / 2 - 112 / 2, 130, 112, MEM_BMPLOGO_MLSTR1, Color_Red);
			DrawFBMP2(350, LCD_HEIGHT / 2 - 268 / 2, 373, 268, MEM_BMPLOGO_HSTR3, Color_Black);
		}
		else
		{
			DrawFillRect(80, LCD_HEIGHT / 2 - 112 / 2, 130, 112, Color_Red);
			DrawFillRect(350, LCD_HEIGHT / 2 - 268 / 2, 373, 268, Color_Black);
		}
		//修改end
		break;

	case 1: //缝机图片开机画面
		if (flag == 0x0)
		{
			DrawBMP(36, 100, 528, 300, MEM_DESKTOPBMP);
			DrawFBMP2(36, 460, 528, 100, MEM_BMPLOGO_HSTR4, Color_Red);
		}
		break;

	case 2: //用户自定义开机画面
		if (g_bootscr.size > 0 && g_bootscr.size <= LCD_height * LCD_width * 2)
		{
			W25QXX_Read(W25Q1_DAT, (U8 *)(Bank1ADDR), W25QBSDATADDR, g_bootscr.size);
			p16 = (U16 *)(Bank1ADDR);
			for (j = 0; j < LCD_height; j++)
			{
				for (i = 0; i < LCD_width; i++)
					*((U16 *)(DisplayAddr + i * XOFFSET + j + j)) = *p16++;
			}
		}
		break;

	case 3: //用户自定义开机动画 avi

		g_logorun = 0;

		//		SETWCS1();									//取消片选
		//		SETWCS2();									//取消片选
		//		SETWCS3();									//取消片选
		//		SETTFCS();									//取消片选
		//
		//		Tfindelay = 0;
		//		TF_Init();
		//		i =0;
		//		while((Tfindelay!=SDTFRDY) && (i<5)){
		//			TF_Init();
		//			wdt();
		//			i++;
		//		}
		//		if(i<5){


		while (1)
		{
			if (Avi_Play(0, 1, 0))
				break;
		}
		//		}

		break;
	}

	gplayLogoAvi = 0;

#endif
}

//------------------------------------------
// Function   : void Menu_Start(void)
// Description: 开机画面
//------------------------------------------
void Menu_Start(void)
{
	U32 i;
	U32 *padd;

	//U8 modemax;
	//U8 f;
	U8 upflag = 0;
	U8 Info[0x20] = {0};
	U8 check1 = 0, check2 = 0;
	U8 tcheck1 = 0, tcheck2 = 0;

	W25QXX_Read(W25Q1_DAT, Info, W25QUPIFOADDR, 0x20);
	check1 = Info[0x1e];
	check2 = Info[0x1f];
	tcheck1 = Info[0];
	tcheck2 = Info[0];
	for (i = 1; i < 0x1e; i++)
	{
		tcheck1 += Info[i];
		tcheck2 ^= Info[i];
	}
	upflag = 1;
	if (check1 == tcheck1 && check2 == tcheck2)
	{
		if (isUpInfoData(Info))
		{
			upflag = (Info[FL_ADDR] == '0' ? 0 : 1);
		}
		else
		{
			//printf("???? err \r\n");
		}
	}
	else
	{
		//printf("check1 = %d tcheck1 = %d check2 = %d tcheck2 = %d\r\n",check1,tcheck1,check2,tcheck2);
	}
	if (upflag == 0x0)//读取文字/图片/文件索引表
	{
		W25QXX_Read(W25Q1_DAT, (U8 *)(SDRAMFLASHADD + FLASHINDEXADDR + 36), FLASHINDEXADDR + 36, 12 * BINFILE_ALL); //读文件索引表
		padd = (U32 *)(SDRAMFLASHADD + FLASHINDEXADDR + 36); //每个地址32位 = 8位 * 4
		for (i = 1; i <= BINFILE_ALL; i++)
		{
			tmpflashadd[i] = (*padd) + SDRAMFLASHADD;
			padd += 3;
		}

		W25QXX_Read(W25Q1_DAT, (U8 *)MEM_DESKTOPBMP, tmpflashadd[2] - SDRAMFLASHADD, tmpflashadd[3] - tmpflashadd[2]);
		W25QXX_Read(W25Q1_DAT, (U8 *)MEM_BMPLOGO_MLSTR1, tmpflashadd[143] - SDRAMFLASHADD, tmpflashadd[144] - tmpflashadd[143]);
		W25QXX_Read(W25Q1_DAT, (U8 *)MEM_BMPLOGO_HSTR3, tmpflashadd[145] - SDRAMFLASHADD, tmpflashadd[146] - tmpflashadd[145]);
	}

	EEPREG[0] = 1;
	WritePage(1);

	CLR_Screen(COLOR_BG);
	displayBootScreen(g_bootscr.mode, upflag);

	ViewPage(1);
}

U16 *getLogoAddr(U16 machine)
{
	//	const U16* p[]={MEM_BMPLOGO_1010,MEM_BMPLOGO_1510,MEM_BMPLOGO_2010,MEM_BMPLOGO_2516,MEM_BMPLOGO_3020,MEM_BMPLOGO_4030,
	//					MEM_BMPLOGO_5040,MEM_BMPLOGO_5050,MEM_BMPLOGO_6030,MEM_BMPLOGO_6050,MEM_BMPLOGO_10050,MEM_BMPLOGO_6040,
	//					MEM_BMPLOGO_311,MEM_BMPLOGO_326,MEM_BMPLOGO_342};

	U16 *addr = MEM_BMPLOGO_1010;

	switch (machine)
	{
	case 1010:
		addr = MEM_BMPLOGO_1010;
		break;
	case 1510:
		addr = MEM_BMPLOGO_1510;
		break;
	case 2010:
		addr = MEM_BMPLOGO_2010;
		break;
	case 2516:
		addr = MEM_BMPLOGO_2516;
		break;
	case 3020:
		addr = MEM_BMPLOGO_3020;
		break;
	case 4030:
		addr = MEM_BMPLOGO_4030;
		break;
	case 5040:
		addr = MEM_BMPLOGO_5040;
		break;
	case 5050:
		addr = MEM_BMPLOGO_5050;
		break;
	case 6030:
		addr = MEM_BMPLOGO_6030;
		break;
	case 6050:
		addr = MEM_BMPLOGO_6050;
		break;
	case 10050:
		addr = MEM_BMPLOGO_10050;
		break;
	case 6040:
		addr = MEM_BMPLOGO_6040;
		break;
	case 311:
		addr = MEM_BMPLOGO_311;
		break;
	case 326:
		addr = MEM_BMPLOGO_326;
		break;
	case 342:
		addr = MEM_BMPLOGO_342;
		break;
	}
	return addr;
}
//------------------------------------------
// Function   : void Menu_DesktopInit(void)
// Description: 桌面画面初始化
//------------------------------------------
void Menu_DesktopInit(void)
{
	WritePage(VMAXPAG);

#ifdef SYSTEM1
	//	#if DIS_LOGO==0
	//	DrawFillRect(0,0,36,32,Color_White);
	//	g_logdir = 0;
	//	DrawFBMP2(15,13,6,6,MEM_BMPLOGO_MR1,Color_Red);
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,LCD_width-36,32,Color_Navy);
	//	DrawFBMP2(36,0,92,32,getLogoAddr(YREG[SYS_MACHINE_TYPE]),Color_Gainsboro);
	//	#endif

	//	#if DIS_LOGO==1
	//	DrawFillRect(0,0,36,32,Color_White);
	//	DrawFBMP2(9,0,14,23,MEM_BMPLOGO_ML1,Color_Red);
	//	DrawFBMP2(0,23,22,9,MEM_BMPLOGO_ML2,Color_Red);
	//	DrawFBMP2(18,15,18,17,MEM_BMPLOGO_ML3,Color_Red);
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,LCD_width-36,32,Color_Navy);
	//	DrawFBMP2(36,0,92,32,MEM_BMPLOGO_MLKH,Color_Gainsboro);
	//	#endif

	//	#if DIS_LOGO==2
	//	DrawFillRect(0,0,36,32,Color_White);
	//	g_logdir = 0;
	//	DrawFBMP2(15,13,6,6,MEM_BMPLOGO_MR1,Color_Red);
	//	DrawFillRect(36,0,92,32,Color_Orange);
	//	DrawFillRect(128,0,LCD_width-36,32,Color_Navy);
	//	DrawFBMP2(36,0,92,32,getLogoAddr(YREG[SYS_MACHINE_TYPE]),Color_Gainsboro);
	//	#endif
	//
	//	if(P_KNO==4){DrawFillRect(0,32,LCD_width,LCD_height-32,Color_SkyBlue);}
	//	else if(P_KNO==3){DrawFillRect(0,32,LCD_width,LCD_height-32,Color_Gainsboro);}
	//	else if(P_KNO==2){DrawFillRect(0,32,LCD_width,LCD_height-32,Color_White);}
	//	else{DrawFillRect(0,32,LCD_width,LCD_height-32,Color_Silver);}
	//	WritePage(usepageno);
	DrawFillRect(0, 0, TITLE_XLEN, TITLE_YLEN, COLOR_TITLE_BG);

	DrawFillRect(0, 0, TIT_LOGO_XLEN, TIT_LOGO_YLEN, COLOR_TITLE_STR);
	//	#if DIS_LOGO==0
	//	g_logdir = 0;
	//	DrawFBMP2(15,13,6,6,MEM_BMPLOGO_MR1,Color_Red);
	//	#endif
	//
	//	#if DIS_LOGO==1
	//	DrawFBMP2(9,0,14,23,MEM_BMPLOGO_ML1,Color_Red);
	//	DrawFBMP2(0,23,22,9,MEM_BMPLOGO_ML2,Color_Red);
	//	DrawFBMP2(18,15,18,17,MEM_BMPLOGO_ML3,Color_Red);
	//	#endif
	if (g_bootscr.vmlk)//左上角logo
	{
		DrawFBMP2(9, 0, 14, 23, MEM_BMPLOGO_ML1, Color_Red);
		DrawFBMP2(0, 23, 22, 9, MEM_BMPLOGO_ML2, Color_Red);
		DrawFBMP2(18, 15, 18, 17, MEM_BMPLOGO_ML3, Color_Red);
	}
	else
	{
		DrawFBMP2(15, 13, 6, 6, MEM_BMPLOGO_MR1, Color_Red);
	}

	//平铺背景色
	DrawFillRect(0, TITLE_YLEN, LCD_WIDTH, LCD_HEIGHT - TITLE_YLEN, COLOR_BG); //天蓝色
	//#if USEMULTCOLOR
	//	if (P_KNO == 4)
	//	{
	//		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_SkyBlue);//天蓝色
	//	}
	//	else if (P_KNO == 3)
	//	{
	//		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	//	}
	//	else if (P_KNO == 2)
	//	{
	//		DrawFillRect(0, 32, LCD_width, LCD_height - 32, RGB(58, 110, 165));//青色
	//	}
	//	else
	//	{
	//		DrawBMP(0, 32, LCD_width, LCD_height - 32, MEM_STARTWLBMP);//背景图片形式
	//	}
	//#else
	//	DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	//#endif

#endif

	// #if USEMULTCOLOR
	// 	if (P_KNO == 4)
	// 	{
	// 		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_SkyBlue);
	// 	}
	// 	else if (P_KNO == 3)
	// 	{
	// 		DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	// 	}
	// 	else if (P_KNO == 2)
	// 	{
	// 		DrawFillRect(0, 32, LCD_width, LCD_height - 32, RGB(58, 110, 165));
	// 	}
	// 	else
	// 	{
	// 		DrawBMP(0, 32, LCD_width, LCD_height - 32, MEM_STARTWLBMP);
	// 	}
	// #else
	// 	DrawFillRect(0, 32, LCD_width, LCD_height - 32, Color_Silver);
	// #endif


	WritePage(usepageno);
}

//------------------------------------------
// Function   : void Menu_StReadData(void)
// Description: 开机读取主板数据
//------------------------------------------
void Menu_StReadData(void)
{
	U8 f = 0;
	U32 i, j = 0;
	U8 re = 0;
	U16 DSP_V = DISP_VER + 4000;
	U16 FLASH_V = flashver;

	Read_D(D_Address[SYS] + SYS_SPEED, 5, (U16 *)(&YREG[SYS_SPEED])); //读取速度，加算、减算值
	Timer_ms = 0;
	while (1)
	{
		if (f == 0) //0：读取打板器的状态与系统版本
		{
			YREG[SYS_COMEN] = 0;
			i = Read_D(D_SYS + SYS_COMEN, 1, (U16 *)(&YREG[SYS_COMEN]));
			if ((i) && (YREG[SYS_COMEN] == 0x5a5a))
			{
				//修改 begin
				//Write_Cmd1(C_COMNECTION_OK,1,2);
				Write_Cmd1(C_COMNECTION_OK, 0, 0);//链接成功
				Read_D(D_Address[SYS] + SYS_SPEED1, 1, (U16 *)(&YREG[SYS_SPEED1]));
				//修改 end
				Write_Cmd3(C_WRITE_VER, SYSSETID >> 16, SYSSETID & 0xffff, DSP_V, FLASH_V);
				Read_Status();

				f = 1;//系统握手成功，完成了DREG与YREG的数据交换
			}
		}

		if (Touchfshflag == 1)
			Timer_ms = 5000;
		if ((g_data_flash == 0) && (f < 10))
		{
			if (f == 1) //1：读取参数表
			{
				i = Read_D(D_Address[PARA], D_Length[PARA], (U16 *)(&PREG[0])); // 读取参数值
				if (i == 0) //读取失败，设置为默认值
				{
					for (j = 0; j < 1024; j++)
						PREG[j] = PARA_DATA[j];
				}
				else
				{
				}
				if (P_R < 60)
					P_R = 60;
				f++;
			}
			if ((Timer_ms > 500) && (f == 2)) //2：读取花样索引
			{

				//获取文件数
				for (i = 0; i < MAXIREG2; i++)
				{
					IREG2[i] = 0xffff;
				}

				if (Write_Cmd(C_READMEMNUMS, 0, 0) == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[1] = MENU_P1;
					pnoindex = 2;
					break;
				}
				if (Read_Status() == 0)
				{
					pno = MENU_RWHELP;
					hleppara = 15;
					poldno[1] = MENU_P1;
					pnoindex = 2;
					break;
				}
				else if (DREG[D_CMDSTATUS] < 100)
				{
					pno = MENU_RWHELP;
					hleppara = 39;
					poldno[1] = MENU_P1;
					pnoindex = 2;
					break;
				}

				Read_D(D_Address[PAGEBUF], (DREG[D_CMDSTATUS] - 100), (U16 *)(&IAPBuff[0]));

				for (i = 0; i < (DREG[D_CMDSTATUS] - 100); i++)
				{
					re = Read_D(D_Address[INDEX2] + (IAPBuff[i] - MINFILENO) * FileIndexSize, FileIndexSize, (U16 *)(&IREG2[(IAPBuff[i] - MINFILENO) * FileIndexSize]));
					if (re == 0)
						break;
				}
				if (re)
				{
					BackupIREG();
					for (j = 0, i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
					{
						if ((((IREG2[i + 1] << 16) | IREG2[i]) > 0) && (((IREG2[i + 1] << 16) | IREG2[i]) < 0xffffffff))
							j++;
					}
					g_Fmax = j / 10;
					if ((g_Fmax == 0) || (j % 10))
						g_Fmax++;
					if (g_Fmax < g_fno)
						g_fno = 1;
				}
				else
				{
					for (i = 0; i < D_Length[INDEX2]; i += FileIndexSize)
					{
						IREG2[i] = 0xffff;
					}
					g_Fmax = 1;
					g_fno = 1;
					j = 0;
				}
				f++;
			}
			if ((Timer_ms > 1000) && (f == 3)) //3：读取花样信息
			{
				if (j)//上一步花样索引读取成功
				{
					i = Read_D(D_Address[HEAD], D_Length[HEAD], (U16 *)(&HREG[0])); // 读当前花样文件信息
					if (i)
					{
						j = HREG[0];
						g_Fileno = HREG[0];

						if ((j >= MINFILENO) && (j <= MAXFILENO))
						{
							i = (j - MINFILENO) * FileIndexSize;
							j = (((U32)(HREG[5]) << 16) + HREG[4]) / 2;
							i = 0; // 如果读花样失败，初始化SREG寄存器
							if ((j >= 0x40) && (j <= MAXSREG))
							{
								i = Read_D(D_Address[PTDATA], j, (U16 *)(&SREG[0])); // 读当前花样数据
								if (i)
								{
									coldstart = 0;
									g_init_flag = 1;
									g_selectdisno = HREG[0];
									g_NoReadPatf = 1;
									g_oldselectdisno = g_selectdisno;
									g_data_flash = 1;
									for (; j < MAXSREG; j++)
									{
										SREG[j] = 0;
									}
								}
							}
						}
					}
				}
				if (g_data_flash == 0)
				{
					f = 10;
				}
			}
		}
		wdt();
		if (Timer_ms > 4800)
		{

			if (f == 0) //0: 开机通讯故障,需要初始化各个寄存器
			{
				for (j = 0; j < 1024; j++)
					PREG[j] = PARA_DATA[j];
				for (i = 0; i < MAXIREG2; i++)
				{
					IREG2[i] = 0xffff;
				}
				for (i = 0; i < MAXSREG; i++)
					SREG[i] = 0;
				for (i = 0; i < MAXHREG; i++)
					HREG[i] = 0;
				SREG[0] = 0;	//文件号
				SREG[2] = 0x10; //线条数据启始位置在SREG[6000]位置
				SREG[3] = 6;
				SREG[4] = 4;	   //每组数据的针数据长最大为80000
				SREG[12] = 0x3e8;  //花样最小长度100mm
				SREG[14] = 0x3e8;  //花样最小宽度100mm
				SREG[16] = 0x2020; //文件名
				SREG[17] = 0x2020;
				SREG[18] = 0x2020;
				SREG[19] = 0x2020;
				SREG[30] = 0x3045; //版本
				SREG[31] = 0x3536;
				SREG[0x24] = 0;

				plastno = MENU_P1;
				pnoindex = 1;
				poldno[0] = MENU_P1;
				poldno[pnoindex] = pno;
				pnoindex++;
				pno = MENU_RWHELP;
				hleppara = 60;
				//pno = MENU_RWHELP;hleppara = 60;
				//InitScreen();ViewPage(usepageno);
			}
			break;
		}
	}
}

U8 CheckTouch()
{
	U8 i, cnt = 0;
	for (i = 0; i < 10; i++)
	{
		if (Touchfshflag == 1)
			cnt++;
		DelayMs(10);
		if (cnt > 3)
			break;
	}
	if (cnt >= 3)
		return 1;
	else
		return 0;
}

//------------------------------------------
// Function   : void CheckTouchPara(void)
// Description: 触摸屏校正检测
//------------------------------------------
void CheckTouchPara(void)
{
	U16 i;
	U16 vpx = 108, vpx1 = 118, vpx2 = 360;
	U8 vnum = 40;

	//	P_LAN = 1;
	if (CheckTouch() == 1)
	{
		DelayMs(60);
		if (CheckTouch() == 1)
		{
			WritePage(usepageno);
			ViewPage(usepageno);
			CLR_Screen(Color_Black);
			if (P_LAN == 0)
			{
				DrawFMultiLStr24(vpx, 120, (U16 *)("等待数字倒数到0后松开手,进入触屏校准!"), vnum, 0, Color_White);
			}
			else
			{
				Draw24X24FStr(vpx1, 120, "Wait the num down to 0 after releasing", Color_White);
				Draw24X24FStr(vpx1, 152, "releasing the hand into touch-screen", Color_White);
				Draw24X24FStr(vpx1, 184, "screen calibration!", Color_White);
			}
			Timer_ms = 0;
			i = 0;
			DrawLFU32(vpx2, 216, 3, 1, Color_White);
			while (Timer_ms < 3000)
			{
				wdt();
				if (i != (Timer_ms / 1000))
				{
					i++;
					DrawFillRect(vpx2, 216, 32, 32, Color_Black);
					DrawLFU32(vpx2, 216, 3 - i, 1, Color_White);
				}
				if (CheckTouch() != 1)
				{
					DelayMs(40);
					if (CheckTouch() != 1)
					{
						Timer_ms = 0;
						break;
					}
				}
			}
			if (Timer_ms >= 3000)
			{
				DrawFillRect(vpx2, 216, 32, 32, Color_Black);
				DrawLFU32(vpx2, 216, 0, 1, Color_White);
				Timer_ms = 0;
				while (Timer_ms < 10000)
				{
					wdt();
					if (CheckTouch() != 1)
					{
						DelayMs(40);
						if (CheckTouch() != 1)
						{
							InitScreen();
							//修改 begin
							SetLanguage(MENU_TOUCHCHECK);
							Draw24x24Str(TITLE_XPOS_MIT, 4, MEM_TITLE_ADD + (TLFNADD_TOUCHSET)*TitleLengthMax, SheetPage, TLFNADD_TOUCHSET, Color_White);
							//修改 end

							DrawFillRect(0, TITLE_YLEN, LCD_width, LCD_height - TITLE_YLEN, COLOR_BG);
							ViewPage(usepageno);
							ChkTouch(0);
							Timer_ms = 0;
							break;
						}
					}
				}
				if (Timer_ms >= 10000)
				{
					CLR_Screen(Color_Black);
					if (P_LAN == 0)
					{
						DrawFMultiLStr24(vpx, 120, (U16 *)("触摸屏检测故障,请及时检查或更换部件!"), vnum, 0, Color_White);
					}
					else
					{
						Draw24X24FStr(vpx1, 120, "Touchscreen detect faults,", Color_White);
						Draw24X24FStr(vpx1, 152, "please check or replace parts!", Color_White);
					}
					while (1)
					{
						wdt();
					}
				}
			}
		}
	}
}
//------------------------------------------
// Function   : void Rotary_Logo(U8 ll)
// Description: LOGO动态显示
//------------------------------------------
void Rotary_Logo(U8 ll)
{
	//修改 begin
	if (g_bootscr.vmlk)
	{
		U8 n;
		U8 i = 0, j = 0;
		U16 color[4] = {Color_Red, RGB(231, 0, 0), RGB(207, 0, 0), RGB(191, 0, 0)};
		U16 coldat[4][3] = {{Color_Red, Color_Red, Color_Red}, {Color_Blue, Color_Yellow, Color_Green}, {Color_Yellow, Color_Green, Color_Blue}, {Color_Green, Color_Blue, Color_Yellow}};

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 3; j++)
			{
				coldat[i][j] = color[i];
			}
		}
		n = ll - 1;
		n = n % 4;
		DrawFBMP2(9, 0, 14, 23, MEM_BMPLOGO_ML1, coldat[n][0]);
		DrawFBMP2(0, 23, 22, 9, MEM_BMPLOGO_ML2, coldat[n][1]);
		DrawFBMP2(18, 15, 18, 17, MEM_BMPLOGO_ML3, coldat[n][2]);
	}
	else
	{
		DrawFillRect(0, 0, 36, 32, Color_White);
		DrawFBMP2(15, 13, 6, 6, MEM_BMPLOGO_MR1, Color_Red);
		if (ll > 1)
			DrawFBMP2(12, 10, 12, 12, MEM_BMPLOGO_MR2, Color_Red);
		if (ll > 2)
			DrawFBMP2(9, 7, 18, 18, MEM_BMPLOGO_MR3, Color_Red);
		if (ll > 3)
		{
			DrawFBMP2(6, 4, 24, 24, MEM_BMPLOGO_MR4, Color_Red);
			g_logdir = 1;
		};
	}
	//修改 end
}

//------------------------------------------
// Function   : void DisAnimation(U8 ll)
// Description: 显示动画 GIF图-故障代码界面
//------------------------------------------
void DisAnimation(U8 ll)
{
	U8 n;
	if (ll > 99)
	{
		n = ll - 100;
		if (n > 67)
			n = 0;
		DrawFillRect(50, 166, 128, 128,Color_Gray);
		DrawFPBMPNEW(50, 166, 128, 128, MEM_BMPGIF, n);
	}
}

//-----------------------------------------------------------------------
#define KEY_SCRNSVR_BACK 1
//------------------------------------------
// Function   : void Menu_Scrnsvr(U8 tt)
// Description: 屏保画面
//------------------------------------------
void Menu_Scrnsvr(U8 tt)
{
	U8 beeponf = 0;
	U32 keyon = 0, keyoff = 0, keydata = 0;

	const U8 keynum = {2};
#if LCD_TYPE == LCD_5cun
	const KEYPAD keyd[] = {{0, 0, 0, 0}, {1, 415, KEYX1, KEYY1}};
#endif
#if LCD_TYPE == LCD_7cun
	const KEYPAD keyd[] = {{0, 0, 0, 0}, {1, 415, KEYX1, KEYY1}};
#endif
#if LCD_TYPE == LCD_8cun
	const KEYPAD keyd[] = {{0, 0, 0, 0}, {1, 735, KEYX1, KEYY1}};
#endif

	InitScreen();
	Draw24X24FStr(TITLE_XPOS, 4, "屏保", Color_White);
	DrawRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, Color_White);
	Draw24X24FStr(keyd[1].xpis + 7, keyd[1].ypis + 20, "返回", Color_White);

	ViewPage(usepageno);
	while (1)
	{
		keydata = get_keydata((KEYPAD *)keyd, keynum, 0);
		if (keydata == 0xff)
		{
			if ((keyon) && (beeponf))
			{
				keyoff = keyon;
			}
			keydata = 0;
		}
		else if (keydata)
		{
			if (keyon == 0)
			{
				keyon = keydata;
				DrawRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, Color_Blue);
				Draw24X24FStr(keyd[1].xpis + 7, keyd[1].ypis + 20, "返回", Color_Blue);
				switch (keydata)
				{
				case KEY_SCRNSVR_BACK:
					break;
				}
				Times = 0;
			}
			else if (keyon != keydata)
			{
				keydata = 0;
			}
			if (Times > 600)
			{
				if (beeponf == 0)
				{
					beeponf = 1;
					if (P_KBC == 0)
						Beep_On(70);
				}
			}
		}
		else
		{
		}
		if (keyoff)
		{
			switch (keyoff)
			{
			case KEY_SCRNSVR_BACK:
				pno = plastno;
				break;
			}
			keyoff = 0;
		}
		if ((keyon) && (keydata == 0))
		{
			DrawRect(keyd[1].xpis, keyd[1].ypis, keyd[1].xsize, keyd[1].ysize, Color_White);
			Draw24X24FStr(keyd[1].xpis + 7, keyd[1].ypis + 20, "返回", Color_White);
			switch (keyon)
			{
			case KEY_SCRNSVR_BACK:
				break;
			}
			keyon = 0;
			beeponf = 0;
		}

		if (pno != MENU_SCRNSVR)
		{
			plastno = MENU_SCRNSVR;
			break;
		}
		wdt();
	}
}
