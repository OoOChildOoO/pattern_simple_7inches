//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  utility.c								*
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

#include "utility.h"
#include "uart.h"
#include "main.h"
#include "fsmc.h"
#include "ltdc.h"
#include "sdio.h"
#include "spi.h"
#include "w25qxx.h"
#include "tfcard.h"
#include "vs1003.h"
#include "touch.h"
#include "i2c.h"
#include "register.h"
#include "communication.h"
#include "menu.h"
#include "Toast.h"
#include "disk_oper.h"
#include "gttouch.h"

volatile U8 keydelay;
volatile U8 keyloopms;
volatile U8 keyolddata;
volatile U8 keyerrtimes;
volatile U16 keyxmin, keyxmax, keyymin, keyymax;
volatile U16 keyxpct, keyypct;

volatile U8 sst39vf6401writeflag;
volatile U16 commtimes;
volatile U16 commtime1ms;
volatile U32 Sdindelay;
volatile U32 Tfindelay;
volatile U32 Timerms; //TIM定时时钟，一般为1s
volatile U32 Timer_ms;
volatile U32 Timer_ms1;
volatile U32 Timerms1;		//计算播放语音间隔时间
volatile U32 Timer_tms;		//时间显示
volatile U32 Timer_dms;		//磁盘显示
volatile U32 userLoninTime; //用户登录在线时间 5分钟后自动离线
volatile U32 time;
volatile U32 Times; //按键扫描防抖次数
volatile U32 DelayNms;
volatile U32 bell_count;
volatile U32 delaytimes;
volatile U32 g_light_c;
volatile U8 g_light_f = 0;
volatile U8 g_logorun;	   //动态logo标志
volatile U8 g_logorunflat; //动态logo及时处理标志位
volatile U8 ioinnum = 0, ioflag = 0;

volatile U8 gEnverify;			 //使能加密校验功能
volatile U32 gverTim, gverTimes; //系统运行校验时间,校验时间计数器
volatile U8 gvererr;			 //校验失败次数  超过5次失败系统报错
volatile U32 BINND;				 // 输入状态
volatile U32 BIND;				 // 输入状态
volatile U32 BINDO;				 // 输入状态
volatile U32 in_p = 0;			 // 每次读取输入计数
volatile U32 in_buf[IND_T];		 // 每次读取输入存放

volatile U16 g_keyBellT = 30; // 按键时蜂鸣器发声时间 //原30
volatile U16 g_allBellT = 30; // 报警时蜂鸣器发声时间 //原30

volatile U8 g_vs1003err = 0; //MPE3解码错误信息
extern volatile U8 uart1_sum;
extern volatile U8 uart2_sum;
extern volatile uint8_t diskoperatef;
extern volatile U16 g_usblogin;
U32 inio;
U32 indh;
U32 indl;

U16 ikeyPress(void);
U8 wakenglag = 0, wakenglag1 = 0;
extern void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group);

extern volatile S16 g_progParas[PROMGMAX];
//---------------------------------------------------
// Function   : system_init
// Description: 初始化，IO设定中断设定
//---------------------------------------------------
void system_init(void)
{
	U32 i;
	UARTMODE uartx_set;

	RCC->CR = 0x00000083;	//xxxxxx 0 0 xxxx 0 0 0 0 xxxxxxxx 10000 0 1 1
	RCC->CFGR = 0x00009400; // AHB = SYSCLK(180M)	APB2(H) = AHB / 2	APB1(L) = AHB / 4
	RCC->CIR = 0x00000000;
	RCC->CR = 0x00010083;	   // PLLCFGR:0000 QQQQ 0 S(1) 0000 PP 0 NNNNNNNNN MMMMMM					(8M:0x08405a08(8M/8*360/2)=180)
	RCC->PLLCFGR = 0x08406008; // = (Fin/M)*N/P = (12M / 12) * 240 = 240MHz / 2 = 120MHz   / 5 = 48MHz	(8M:0x08406008(8M/8*384/2)=192)(8M:0x05401e04(8M/4*120/2))(12M:0x05401e06(12M/6*120/2))(12M:0x05403c0c(12M/12*240/2))
	for (i = 0; i < 10000; i++)
	{
		if (RCC->CR & 0x00020000)
			break; //等待HSE使能稳定
	}
	RCC->CR = 0x01030083; //xxxxxx 0 1 xxxx 0 0 0 0 xxxxxxxx 10000 0 1 1

	FLASH->ACR &= 0x00000000; //	FLASH->ACR|=1<<8;		//指令预取使能.
	FLASH->ACR |= 0x00000005; //	FLASH->ACR|=1<<9;		//指令cache使能.
	FLASH->ACR &= 0xFFFFFFFF; //	FLASH->ACR|=1<<10;		//数据cache使能.
	FLASH->ACR |= 0x00000700; //	FLASH->ACR|=5<<0;		//5个CPU等待周期.

	for (i = 0; i < 10000; i++)
	{
		if (RCC->CR & 0x02000000)
			break; //等待PLL输出稳定
	}
	RCC->CFGR = 0x00009402; //xxxxxx 000 0 0100 0 1 00 000 100 0000 00 10
	for (i = 0; i < 10000; i++)
	{
		if ((RCC->CFGR & 0x0000000c) == 0x00000008)
			break; //等待切换PLL输出稳定
	}
	//RCC->PLLI2SCFGR = 0x24003000;
	//RCC->PLLSAICFGR = 0x24003000;
	//RCC->DCKCFGR    = 0x00000000;		// TIMPRE(24) = 0	定时器时钟为APB两倍

	RCC->AHB1RSTR = 0xffffffff;
	RCC->AHB2RSTR = 0xffffffff;
	RCC->AHB3RSTR = 0xffffffff;
	RCC->APB1RSTR = 0xffffffff;
	RCC->APB2RSTR = 0xffffffff;
	RCC->AHB1RSTR = 0x00000000;	 // xx 0 xxx 0 xx 0 0 xxxxxxxx 0 xxx 0 0 0 0 0 0 0 0 0			OTGHS ETH DMA2 DMA1 CRC GI GH GG GF GE GD GC GB GPIOA
	RCC->AHB2RSTR = 0x00000000;	 // xxxxxxxxxxxxxxxxxxxxxxxx 0 0 0 0 xxx 0						OTGFS RNG HASH CRYP DCMI
	RCC->AHB3RSTR = 0x00000000;	 // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 0							FSMC
	RCC->APB1RSTR = 0x00000000;	 // xx 0 0 x 0 0 x 0 0 0 0 0 0 0 x 0 0 xx 0 xx 0 0 0 0 0 0 0 0 0	DA PWR CAN2 CAN1 I2C3 I2C2 I2C1 UT5 UT4 UT3 UT2 SPI3 SPI2 WDG T14 T13 T12 T7 T6 T5 T4 T3 T2
	RCC->APB2RSTR = 0x00000000;	 // xxxxx 0 xxxxxxx 0 0 0 x 0 x 0 0 xx 0 xx 0 0 xx 0 0			LTDC T11 T10 T9 SYSCF SPI1 SDIO ADC UT6 UT1 T8 T1
	RCC->AHB1ENR = 0x007001ff;	 // x 0 0 0 0 0 0 xx 1 1 1 x 0 xxxxx 0 xxx 1 1 1 1 1 1 1 1 1		OTGHSU OTGHS ETHP ETHR ETHT ETH DMA2 DMA1 CCM BKP CRC GI GH GG GF GE GD GC GB GPIOA
	RCC->AHB2ENR = 0x000000c0;	 // xxxxxxxxxxxxxxxxxxxxxxxx 1 1 0 0 xxx 0						OTGFS RNG HASH CRYP DCMI
	RCC->AHB3ENR = 0x00000001;	 // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 1							FSMC																								(192MHz)
	RCC->APB1ENR = 0x0422c1b0;	 // 0 0 0 0 x 1 0 x 0 0 1 0 0 0 1 x 1 1 xx 0 xx 1 1 0 1 1 0 0 0 0UT8 UT7	DA PWR CAN2 CAN1 I2C3 I2C2 I2C1 UT5 UT4 UT3 UT2 SPI3 SPI2 WDG T14 T13 T12 T7 T6 T5 T4 T3 T2	(48MHz)
	RCC->APB2ENR = 0x04005810;	 // xxxxx 1 xxxx 0 0 x 0 0 0 x 1 0 1 1 0 0 0 xx 0 1 xx 0 0		LTDC SPI6 SPI5 T11 T10 T9 SYSCF SPI4 SPI1 SDIO ADC3 ADC2 ADC1 UT6 UT1 T8 T1							(96MHz)
	RCC->AHB1LPENR = 0x7e6791ff; // x 0 0 0 0 0 0 xx 0 0 xx 0 0 0 0 xx 0 xxx 0 0 0 0 0 0 0 0 0	OTGHSU OTGHS ETHP ETHR ETHT ETH DMA2 DMA1 BKP SRAM2 SRAM1 FLITF CRC GI GH GG GF GE GD GC GB GPIOA
	RCC->AHB2LPENR = 0x000000F1; // xxxxxxxxxxxxxxxxxxxxxxxx 0 0 0 0 xxx 0						OTGFS RNG HASH CRYP DCMI
	RCC->AHB3LPENR = 0x00000001; // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 0							FSMC																								(180MHz)
	RCC->APB1LPENR = 0x36fec9ff; // xx 0 0 0 0 0 x 0 0 0 0 0 0 0 x 0 0 xx 0 xx 0 0 0 0 0 0 0 0 0	DA PWR RESVED CAN2 CAN1 I2C3 I2C2 I2C1 UT5 UT4 UT3 UT2 SPI3 SPI2 WDG T14 T13 T12 T7 T6 T5 T4 T3 T2	(45MHz)
	RCC->APB2LPENR = 0x00075f33; // xxxxxxxxxxxxx 0 0 0 x 0 x 0 0 0 0 0 xx 0 0 xx 0 0			T11 T10 T9 SYSCF SPI1 SDIO ADC3 ADC2 ADC1 UT6 UT1 T8 T1												(90MHz)

	SYSCFG->MEMRMP = 0x00000000;
	SYSCFG->CMPCR = 0x00000001;
	SYSCFG->EXTICR[0] = 0x00000000; // 0:PA 1:PB 2:PC 3:PD 4:PE 5:PF 6:PG 7:PH 8:PI
	SYSCFG->EXTICR[1] = 0x00000000;
	SYSCFG->EXTICR[2] = 0x00000000;
	SYSCFG->EXTICR[3] = 0x00000600;
// MODER	00: 输入	01：输出	10：复用	11: 模拟
// OTYPER	 0: 推挽	 1：开漏
// OSPEEDR	00:	2MHz	01: 25MHz	10: 50MHz	11: 100MHz
// PUPDR	00: 不拉	01: 上拉	10: 下拉	11: 保留
// AFR		0000: AF0	0001: AF1	......		1111: AF15
#if 0 
	GPIOA->BSRRH  = 0xffff;			// 输出0
	GPIOB->BSRRH  = 0xffff;
	GPIOC->BSRRH  = 0xffff;
	GPIOD->BSRRH  = 0xffff;			
	GPIOE->BSRRH  = 0xffff;
	GPIOF->BSRRH  = 0xffff;
	GPIOG->BSRRH  = 0xffff;
	GPIOH->BSRRH  = 0xffff;
	GPIOI->BSRRH  = 0xffff;	
	GPIOI->BSRRL  = 0x0800;	
	GPIOD->BSRRL  = 0x0008;
#else //支持久编译器
	GPIOA->BSRR |= (uint32_t)0xffff << 16; // 输出0
	GPIOB->BSRR |= (uint32_t)0xffff << 16;
	GPIOC->BSRR |= (uint32_t)0xffff << 16;
	GPIOD->BSRR |= (uint32_t)0xffff << 16;
	GPIOE->BSRR |= (uint32_t)0xffff << 16;
	GPIOF->BSRR |= (uint32_t)0xffff << 16;
	GPIOG->BSRR |= (uint32_t)0xffff << 16;
	GPIOH->BSRR |= (uint32_t)0xffff << 16;
	GPIOI->BSRR |= (uint32_t)0xffff << 16;

	GPIOI->BSRR &= (uint32_t)0xffff0000;
	GPIOI->BSRR |= 0x0800;

	GPIOD->BSRR &= (uint32_t)0xffff0000;
	GPIOD->BSRR |= 0x0008;
#endif

	//GPIOA->MODER  = 0x2aa1a9a5;			// MODER	00: 输入	01：输出	10：复用	11: 模拟	关闭SW:0x02a90faa	使用SW:0x2aa1a9a5
	GPIOA->MODER = 0x6aa1a9a5; // MODER	00: 输入	01：输出	10：复用	11: 模拟	关闭SW:0x02a90faa	使用SW:0x2aa1a9a5
	GPIOB->MODER = 0xaa6a6a95;
	GPIOC->MODER = 0x02aa00a2;
	GPIOD->MODER = 0xaaaa8a6a;
	GPIOE->MODER = 0xaaaa9a8a;
	GPIOF->MODER = 0xaaaa0aaa;
	GPIOG->MODER = 0x85a2aaaa;
	GPIOH->MODER = 0xaaa95100;
	GPIOI->MODER = 0x0068aa2a;
	GPIOA->OTYPER = 0x00000000; // OTYPER	 0: 推挽	 1：开漏
	GPIOB->OTYPER = 0x00000300;
	GPIOC->OTYPER = 0x00000000;
	GPIOD->OTYPER = 0x00000000;
	GPIOE->OTYPER = 0x00000000;
	GPIOF->OTYPER = 0x00000000;
	GPIOG->OTYPER = 0x00000000;
	GPIOH->OTYPER = 0x00000000;
	GPIOI->OTYPER = 0x00000000;
	GPIOA->OSPEEDR = 0xffe5fdf5; // OSPEEDR	00:	2MHz	01: 25MHz	10: 50MHz	11: 100MHz
	GPIOB->OSPEEDR = 0xff7f6fd5;
	GPIOC->OSPEEDR = 0x57ff55f7;
	GPIOD->OSPEEDR = 0xffffcf7f;
	GPIOE->OSPEEDR = 0xffffdfdf;
	GPIOF->OSPEEDR = 0xfff51fff;
	GPIOG->OSPEEDR = 0xd5f3ffff;
	GPIOH->OSPEEDR = 0xfffd5500;
	GPIOI->OSPEEDR = 0x007dff7f;
	GPIOA->PUPDR = 0x54155555; // PUPDR	00: 不拉	01: 上拉	10: 下拉	11: 保留 PA11 PA12:USB		0x14155555
	GPIOB->PUPDR = 0x55505555;
	GPIOC->PUPDR = 0x55555555;
	GPIOD->PUPDR = 0x55554555;
	GPIOE->PUPDR = 0x55555555;
	GPIOF->PUPDR = 0x55550555;
	GPIOG->PUPDR = 0x55515555;
	GPIOH->PUPDR = 0x55555510;
	GPIOI->PUPDR = 0x00555555;
	GPIOA->AFR[0] = 0x55507700; // AFR		0000: AF0	0001: AF1	......		1111: AF15
	GPIOB->AFR[0] = 0x07666000;
	GPIOC->AFR[0] = 0x0000cc0c;
	GPIOD->AFR[0] = 0xc0cc0ccc;
	GPIOE->AFR[0] = 0xc0ccc0cc;
	GPIOF->AFR[0] = 0x00cccccc;
	GPIOG->AFR[0] = 0xeecccccc;
	GPIOH->AFR[0] = 0x00000000;
	GPIOI->AFR[0] = 0xeeee0eee;
	GPIOA->AFR[1] = 0x000aa700; // PA9:7 串口一		PA9:10 USBVBUS
	GPIOB->AFR[1] = 0x55990544;
	GPIOC->AFR[1] = 0x000ccccc;
	GPIOD->AFR[1] = 0xcccccccc;
	GPIOE->AFR[1] = 0xcccccccc;
	GPIOF->AFR[1] = 0xccccce99;
	GPIOG->AFR[1] = 0xc000ec0c;
	GPIOH->AFR[1] = 0xeeeeeee0;
	GPIOI->AFR[1] = 0x00000ee0;
	SETWDI();
	SETUSBLED();
	SETSDLED();
	SETLCDONOFF();
	SETMCIPWR();
	SETWCS1();
	SETWCS2();
	SETWCS3();
	SETTFCS();
	SETLTCS();
	//SETLTRST();
	CLRLTRST();
	SETT_SSEL();
	SETVSXCS();
	SETVSRST();
	SETVSXDCS();
	SETUSBFSEN();

	EXTI->IMR = 0x00004000; // DREQ:PG14
	EXTI->EMR = 0x00000000;
	EXTI->RTSR = 0x00004000; // 上升沿
	EXTI->FTSR = 0x00000000; // 下降沿
#ifdef UPDATEADD
	SCB->VTOR = UPGRADEADDR;
#else
	SCB->VTOR = 0x08000000;
#endif
	SCB->AIRCR = 0x05fa0500;	// 0x05faxxxx
	NVIC->ISER[0] = 0x80036000; //10000000 00000011 01100000 00000000  31:I2C1V 37:U1 38:U2 55:T7 67:OTGFS 35:SPI1 36:SPI2 51:SPI3 49:SDIO 54:T6
	NVIC->ISER[1] = 0x80ca0178; //10000000 11001010 00000001 01111000  13:DM12 14:DM13 16:DM15 17:DM16 68:DM25 70:DM27 40:ET1510 63:CAN2TX 64:CAN2RX0 65:CAN2RX1
	NVIC->ISER[2] = 0x0000005b; //00000000 00000000 00000000 01011011
	NVIC->ICER[0] = 0x7ffc9fff; // 40          55	 67	 31 49	 35 36 51   63 64 65  37 38   17 70	   16 68     13 14 -15(系统定时器)         54
	NVIC->ICER[1] = 0x7f35fe87; //00     1  2  3	14   5       6          7        28       9        10        11				     312 13	14 15
	NVIC->ICER[2] = 0xffffffa4; //
	NVIC->ISPR[0] = 0x00000000;
	NVIC->ISPR[1] = 0x00000000; // 0:WDG 1:PVD 2:TAMP 3:RTC 4:FLASH 5:RCC 6:ET0 7:ET1 8:ET2 9:ET3 10:ET4 11:DM10 12:DM11 13:DM12 14:DM13 15:DM14
	NVIC->ICPR[0] = 0x00000000; // 16:DM15 17:DM16 18:AD 19:USBHPCANTX 20:USBLPCANRX 21:CNARX1 22:CANSCE 23:ET95 24:T1B 25:T1U 26:T1T 27:T1C 28:T2
	NVIC->ICPR[1] = 0x00000000; // 29:T3 30:T4 31:I2C1V 32:I2C1E 33:I2C2V 34:I2C2E 35:SPI1 36:SPI2 37:U1 38:U2 39:U3 40:ET1510 41:RTCA 42:USB 43:T8B
	NVIC->IABR[0] = 0x00000000; // 44:T8U 45:T8T 46:T8C 47:DM17 48:FSMC 49:SDIO 50:T5 51:SPI3 52:U4 53:U5 54:T6 55:T7 56:DM20 57:DM21 58:DM22 59:DM23
	NVIC->IABR[1] = 0x00000000; // 60:DM24 61:ETH 62:ETHW 63:CAN2TX 64:CAN2RX0 65:CAN2RX1 66:CAN2SCE 67:OTGFS 68:DM25 69:DM26 70:DM27 71:U6
	NVIC->IP[0] = 0x00;			// WWDG
	NVIC->IP[1] = 0x00;			// PVD
	NVIC->IP[2] = 0x00;			// TAMP
	NVIC->IP[3] = 0x00;			// RTC
	NVIC->IP[4] = 0x00;			// FLASH
	NVIC->IP[5] = 0x00;			// RCC
	NVIC->IP[6] = 0x00;			// EXTI0
	NVIC->IP[7] = 0x00;			// EXTI1
	NVIC->IP[8] = 0x00;			// EXTI2
	NVIC->IP[9] = 0x00;			// EXTI3
	NVIC->IP[10] = 0x00;		// EXTI4
	NVIC->IP[11] = 0x00;		// DMA1_Stream0
	NVIC->IP[12] = 0x00;		// DMA1_Stream1
	NVIC->IP[13] = 0xb0;		// DMA1_Stream2
	NVIC->IP[14] = 0xb0;		// DMA1_Stream3
	NVIC->IP[15] = 0x00;		// DMA1_Stream4
	NVIC->IP[16] = 0xa0;		// DMA1_Stream5
	NVIC->IP[17] = 0x90;		// DMA1_Stream6
	NVIC->IP[18] = 0x00;		// ADC
	NVIC->IP[19] = 0x00;		// CAN1_TX
	NVIC->IP[20] = 0x00;		// CAN1_RX0
	NVIC->IP[21] = 0x00;		// CAN1_RX1
	NVIC->IP[22] = 0x00;		// CAN1_SCE
	NVIC->IP[23] = 0x00;		// EXTI9_5
	NVIC->IP[24] = 0x00;		// TIM1_BRK_TIM9
	NVIC->IP[25] = 0x00;		// TIM1_UP_TIM10
	NVIC->IP[26] = 0x00;		// TIM1_TRG_COM_TIM11
	NVIC->IP[27] = 0x00;		// TIM1_CC
	NVIC->IP[28] = 0x00;		// TIM2
	NVIC->IP[29] = 0x00;		// TIM3
	NVIC->IP[30] = 0x00;		// TIM4
	NVIC->IP[31] = 0x50;		// I2C1_EV
	NVIC->IP[32] = 0x00;		// I2C1_ER
	NVIC->IP[33] = 0x00;		// I2C2_EV
	NVIC->IP[34] = 0x00;		// I2C2_ER
	NVIC->IP[35] = 0x60;		// SPI1
	NVIC->IP[36] = 0x60;		// SPI2
	NVIC->IP[37] = 0x80;		// USART1
	NVIC->IP[38] = 0x80;		// USART2	// 0x80		0x20
	NVIC->IP[39] = 0x00;		// USART3
	NVIC->IP[40] = 0x10;		// EXTI15_10
	NVIC->IP[41] = 0x00;		// RTC_Alarm
	NVIC->IP[42] = 0x00;		// OTG_FS_WKUP
	NVIC->IP[43] = 0x00;		// TIM8_BRK_TIM12
	NVIC->IP[44] = 0x00;		// TIM8_UP_TIM13
	NVIC->IP[45] = 0x00;		// TIM8_TRG_COM_TIM14
	NVIC->IP[46] = 0x00;		// TIM8_CC
	NVIC->IP[47] = 0x00;		// DMA1_Stream7
	NVIC->IP[48] = 0x00;		// FSMC
	NVIC->IP[49] = 0x10;		// SDIO		0x50
	NVIC->IP[50] = 0x00;		// TIM5
	NVIC->IP[51] = 0x60;		// SPI3
	NVIC->IP[52] = 0x00;		// UART4
	NVIC->IP[53] = 0x00;		// UART5
	NVIC->IP[54] = 0xf0;		// TIM6
	NVIC->IP[55] = 0x30;		// TIM7
	NVIC->IP[56] = 0x00;		// DMA2_Stream0
	NVIC->IP[57] = 0x00;		// DMA2_Stream1
	NVIC->IP[58] = 0x00;		// DMA2_Stream2
	NVIC->IP[59] = 0x00;		// DMA2_Stream3
	NVIC->IP[60] = 0x00;		// DMA2_Stream4
	NVIC->IP[61] = 0x00;		// ETH
	NVIC->IP[62] = 0x00;		// ETH_WKUP
	NVIC->IP[63] = 0x70;		// CAN2_TX
	NVIC->IP[64] = 0x70;		// CAN2_RX0
	NVIC->IP[65] = 0x70;		// CAN2_RX1
	NVIC->IP[66] = 0x00;		// CAN2_SCE
	NVIC->IP[67] = 0x40;		// OTG_FS	40
	NVIC->IP[68] = 0xa0;		// DMA2_Stream5
	NVIC->IP[69] = 0x00;		// DMA2_Stream6
	NVIC->IP[70] = 0x90;		// DMA2_Stream7
	NVIC->IP[71] = 0x00;		// USART6
	NVIC->IP[72] = 0x00;		// I2C3_EV
	NVIC->IP[73] = 0x00;		// I2C3_ER
	NVIC->IP[74] = 0x00;		// OTG_HS_EP1_OUT
	NVIC->IP[75] = 0x00;		// OTG_HS_EP1_IN
	NVIC->IP[76] = 0x00;		// OTG_HS_WKUP
	NVIC->IP[77] = 0x00;		// OTG_HS
	NVIC->IP[78] = 0x00;		// DCMI
	NVIC->IP[79] = 0x00;		// CRYP
	NVIC->IP[80] = 0x00;		// HASH_RNG

	SPI2_Init(SP_187500HZ);
	SCB->SHP[11] = 0xb0;   // 最低优先级
	SysTick->LOAD = 24000; // 192MHz / 8 = 24MHz / 24000 = 1ms
	SysTick->VAL = 0;
	SysTick->CTRL = 0x3; // 0xx:外部时钟(clk/8)	1xx:内部时钟(clk)	x1x:使能中断	xx1:使能系统时钟定时器

	DelayMs(60);
	g_vs1003err = 0;
	if ((GPIOG->IDR & INDDATA) == 0)
	{
		DelayMs(60);
		if ((GPIOG->IDR & INDDATA) == 0)
		{
			g_vs1003err = 1;
		}
	}
	uartx_set.datab = 0x0000;	   // 0x0000:8位数据长 0x1000:9位数据长
	uartx_set.stopb = 0x0000;	   // 0x0000:1位停止 0x1000:0.5位停止 0x2000:2位停止 0x3000:1.5位停止
	uartx_set.parity = 0x0000;	   // 0x0000:无校验 0x0400:偶校验 0x0600:奇校验
	UART1_Init(115200, uartx_set); // 115200  57600  56000  43000  38400  19200  9600
	UART2_Init(781250, uartx_set); // 115200  57600  56000  43000  38400  19200  9600	打版器通讯	  950400	 781250		390625
								   //	UART2_Init(115200, uartx_set);
	I2C1_Init(400000);
	Fsmc_Init();
	SDRAM_Init();
	Time13_Init();
	Time14_Init();
	Time6_Init();
	LTDC_Init();
	SPI1_Init(SP_12MHZ); // W25Q128_1	W25Q128_2	TF卡	LT8900
	//SPI2_Init(SP_187500HZ);			// TSC2046
	SPI3_Init(SP_750000HZ); // VS1003B
	W25QXX_Init();
	//printf("g_vs1003err = %d \r\n",g_vs1003err);
	if (g_vs1003err == 0)
	{
		VS1003B_Init();
	}

	TF_Init(); // 存放视频文件用,机器宣传 教学 说明书等(没插卡时该函数等待时间比较长,需将其放在开机画面显示时执行)
	USB_Init();
	keyxmin = 0;
	keyxmax = 0;
	keyymin = 0;
	keyymax = 0;
	sst39vf6401writeflag = 0;
	GT9xx_Init();
}

//------------------------------------------
// Function   : OTG_FS_IRQHandler
// Description: This function handles USB-On-The-Go FS global interrupt request
//------------------------------------------
//void OTG_FS_IRQHandler(void)
//{
//	USBH_OTG_ISR_Handler(&USB_OTG_Core);
//}

//---------------------------------------------------
// Function   : Time6_Init()
// Description: 定时器初始化
//---------------------------------------------------
void Time6_Init(void)
{
	Sdindelay = 0;
	TIM6->CR1 = 0x0000;
	TIM6->CR2 = 0x0000;
	TIM6->SMCR = 0x0000;
	TIM6->DIER = 0x0001;
	TIM6->EGR = 0x0001;
	TIM6->PSC = 0x0007;
	TIM6->RCR = 0x0000;
	TIM6->ARR = 12000;
	TIM6->CR1 = 0x0001;
}

//---------------------------------------------------
// Function   : Time13_Init()
// Description: PWM输出 LED灯
//---------------------------------------------------
void Time13_Init(void)
{
	TIM13->CR1 = 0x0080;
	TIM13->CR2 = 0x0000;
	TIM13->SMCR = 0x0000;
	TIM13->DIER = 0x0000;
	TIM13->SR = 0x0000;
	TIM13->CCMR1 = 0x7c7c;
	TIM13->CCMR2 = 0x7c7c;
	TIM13->CCER = 0x0001;
	TIM13->CNT = 0;
	TIM13->PSC = 7;
	TIM13->ARR = TLCLK / 1000;
	TIM13->RCR = 0x0000;
	TIM13->CCR1 = 0;
	TIM13->CCR2 = 0;
	TIM13->CCR3 = 0;
	TIM13->CCR4 = 0;
	TIM13->BDTR = 0x8000;
	TIM13->DCR = 0;
	TIM13->DMAR = 0;
	TIM13->EGR = 0x0001;
	TIM13->CR1 = 0x0081;
}

//---------------------------------------------------
// Function   : Time14_Init()
// Description: PWM输出 LCD背光
//---------------------------------------------------
void Time14_Init(void)
{
	TIM14->CR1 = 0x0080;
	TIM14->CR2 = 0x0000;
	TIM14->SMCR = 0x0000;
	TIM14->DIER = 0x0000;
	TIM14->SR = 0x0000;
	TIM14->CCMR1 = 0x7c7c;
	TIM14->CCMR2 = 0x7c7c;
	TIM14->CCER = 0x0003;
	TIM14->CNT = 0;
	TIM14->PSC = 7;
	TIM14->ARR = TLCLK / 300;
	TIM14->RCR = 0x0000;
	TIM14->CCR1 = 0;
	TIM14->CCR2 = 0;
	TIM14->CCR3 = 0;
	TIM14->CCR4 = 0;
	TIM14->BDTR = 0x8000;
	TIM14->DCR = 0;
	TIM14->DMAR = 0;
	TIM14->EGR = 0x0001;
	TIM14->CR1 = 0x0081;
}

//---------------------------------------------------
// Function   : SetLedPwm(U32 pwm)
// Description: 设置LEDPWM占空比
//---------------------------------------------------
void SetLedPwm(U32 pwm)
{
	TIM13->CCR1 = TLCLK / 100000 * pwm;
}

//---------------------------------------------------
// Function   : SetLcdPwm(U8 lcdoff,U32 pwm)
// Description: 设置LCDPWM占空比
//---------------------------------------------------
void SetLcdPwm(U8 lcdoff, U32 pwm)
{
	U8 i;

	if (pwm > 99)
		i = 200;
	else if (pwm > 91)
		i = (pwm - 91) * 4 + 164;
	else if (pwm > 70)
		i = (pwm - 70) * 3 + 101;
	else if (pwm > 40)
		i = (pwm - 40) * 2 + 41;
	else
		i = pwm + 5;
	if (lcdoff == 0)
		TIM14->CCR1 = 0;
	else
		TIM14->CCR1 = TIM14->ARR / 200 * i;
}

//---------------------------------------------------
// Function   : Beep_On()
// Description: 主板上的蜂鸣器开
//---------------------------------------------------
void Beep_On(U32 t)
{
	//return ;
	bell_count = t;
	SETNBELL();
}

//------------------------------------------
// Function   : U8 asctonum(U8 asc)
// Description: ASC码转数字
//------------------------------------------
U8 asctonum(U8 asc)
{
	const U8 atondata[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};

	if ((asc > 0x60) && (asc < 0x67))
	{
		asc -= 0x20;
	}
	if ((asc > 0x2f) && (asc < 0x47))
	{
		return (atondata[asc - 0x30]);
	}
	return (0);
}

//------------------------------------------
// Function   : U8 numtoasc(U8 asc)
// Description: 数字转ASC码
//------------------------------------------
U8 numtoasc(U8 num)
{
	const U8 ntoadata[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46};

	if (num < 16)
	{
		return (ntoadata[num]);
	}
	return (0);
}

//---------------------------------------------------
// Function   : DelayMs()
// Description: ms延时
//---------------------------------------------------
void DelayMs(unsigned int n)
{
	DelayNms = n;
	DelayNms = n;
	while (DelayNms)
		wdt();
}

//---------------------------------------------------
// Function   : DelayUs()
// Description: us延时
//---------------------------------------------------
void DelayUs(unsigned int n)
{
	U32 i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < 45; j++)
			;
	}
}

//---------------------------------------------------
// Function   : wdt()
// Description: 喂狗
//---------------------------------------------------
void wdt(void)
{
	if (WWDI)
	{
		CLRWDI();
	}
	else
	{
		SETWDI();
	}
}

//---------------------------------------------------
// Function   : EXTI0_IRQHandler
// Description: IO0
//---------------------------------------------------
void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & 0x0001)
	{ // Px0
		EXTI->PR = 0x0001;
	}
}

//---------------------------------------------------
// Function   : EXTI1_IRQHandler
// Description: IO1
//---------------------------------------------------
void EXTI1_IRQHandler(void)
{
	if (EXTI->PR & 0x0002)
	{ // Px1
		EXTI->PR = 0x0002;
	}
}

//---------------------------------------------------
// Function   : EXTI2_IRQHandler
// Description: IO2
//---------------------------------------------------
void EXTI2_IRQHandler(void)
{
	if (EXTI->PR & 0x0004)
	{ // Px2
		EXTI->PR = 0x0004;
	}
}

static U8 tcnt = 0;
//---------------------------------------------------
// Function   : EXTI3_IRQHandler
// Description: IO3
//---------------------------------------------------
void EXTI3_IRQHandler(void)
{
	if (EXTI->PR & 0x0008)
	{ // Px3
		EXTI->PR = 0x0008;
		if (gtTouch)
		{
			if ((tcnt % 3) == 0)
			{
				GT9xx_ScanIRQ();
			}
			tcnt++;
		}
	}
}

//---------------------------------------------------
// Function   : EXTI4_IRQHandler
// Description: IO4
//---------------------------------------------------
void EXTI4_IRQHandler(void)
{
	if (EXTI->PR & 0x0010)
	{ // Px4
		EXTI->PR = 0x0010;
	}
}

//---------------------------------------------------
// Function   : EXTI9_5_IRQHandler
// Description: IO5 IO6 IO7 IO8 IO9
//---------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
	if (EXTI->PR & 0x0020)
	{ // Px5
		EXTI->PR = 0x0020;
	}
	if (EXTI->PR & 0x0040)
	{ // Px6
		EXTI->PR = 0x0040;
	}
	if (EXTI->PR & 0x0080)
	{ // Px7
		EXTI->PR = 0x0080;
	}
	if (EXTI->PR & 0x0100)
	{ // Px8
		EXTI->PR = 0x0100;
	}
	if (EXTI->PR & 0x0200)
	{ // Px9
		EXTI->PR = 0x0200;
	}
}

//---------------------------------------------------
// Function   : EXTI15_10_IRQHandler
// Description: IO10 IO11 IO12 IO13 IO14 IO15
//---------------------------------------------------
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & 0x0400)
	{ // Px10
		EXTI->PR = 0x0400;
	}
	if (EXTI->PR & 0x0800)
	{ // Px11
		EXTI->PR = 0x0800;
	}
	if (EXTI->PR & 0x1000)
	{ // Px12
		EXTI->PR = 0x1000;
	}
	if (EXTI->PR & 0x2000)
	{ // Px13
		EXTI->PR = 0x2000;
	}
	if (EXTI->PR & 0x4000)
	{ // Px14		PG14:DREQ
		EXTI->PR = 0x4000;
		VS1003B_WriteDAT();
	}
	if (EXTI->PR & 0x8000)
	{ // Px15
		EXTI->PR = 0x8000;
	}
}

//---------------------------------------------------
// Function   : void TIM6_DAC_IRQHandler(void)
// Description: 定时器6中断 最低优先级(U盘 SD卡检测,初始化用)
//---------------------------------------------------
void TIM6_DAC_IRQHandler(void)
{
	if (TIM6->SR & 0x0001)
	{
		TIM6->SR = 0x0000;

		if (WWMCIINSN)
		{
			if (Sdindelay == 0)
			{
				CLRMCIPWR();
			}
			if (Sdindelay < 35)
			{
				Sdindelay++;
				if (Sdindelay > 29)
				{
					if (SD_Init() == 0)
					{
						if (SDCardInfo.CardCapacity != 0)
						{
							Sdindelay = 35;
						}
						else
						{
							SD_ReSet();
							DelayMs(10);
							if (Sdindelay == 35)
								SETMCIPWR();
						}
					}
					else
					{
						DelayMs(10);
						if (Sdindelay == 35)
							SETMCIPWR();
					}
				}
			}
		}
		else
		{
			if (Sdindelay)
			{
				if (Sdindelay == 35)
					SD_ReSet();
				Sdindelay = 0;
				SETMCIPWR();
			}
		}
		if (diskoperatef == 0)
			USB_Process();
		commtime1ms++; // 300ms定时读取主板状态

		if (gtTouch == 0)
		{ //电阻屏屏保激活
			if (WWT_PENIRQN)
			{
				if (g_light_f)
				{
					g_light_f = 0;
					if ((TIM14->CCR1 == 0) && (P_LOF))
					{
						keyolddata = 0xff;
						SetLcdPwm(1, P_CNR);
					}
				}
			}
			else if (g_light_f == 0)
			{
				g_light_f = 1;
				g_light_c = P_R * 1000;
			}
			else if (g_light_c)
			{
				if (g_logorun < 100)
					g_light_c--; // 报警画面不关背光
				if ((g_light_c == 0) && (P_LOF))
				{
					SetLcdPwm(0, 0); // LCD关
				}
			}
		}
		else
		{ //电容屏屏保激活
			if (GT9xx_Scan())
			{
				if (g_light_f)
				{
					g_light_f = 0;
					if ((TIM14->CCR1 == 0) && (P_LOF))
					{
						wakenglag = 1;
						SetLcdPwm(1, P_CNR);
					}
				}
			}
			else if (g_light_f == 0)
			{
				g_light_f = 1;
				g_light_c = P_R * 1000;
			}
			else if (g_light_c)
			{
				if (g_logorun < 100)
					g_light_c--; // 报警画面不关背光
				if ((g_light_c == 0) && (P_LOF))
				{
					SetLcdPwm(0, 0); // LCD关
				}
			}
		}

		//if(commtime1ms>300)ErrorMsg(pno);
		//if(WSDLED)CLRSDLED();else{SETSDLED();}
	}
}

//---------------------------------------------------
// Function   : void SysTick_Handler(void)
// Description: SysTick_Handler系统时钟中断
//---------------------------------------------------
void SysTick_Handler(void)
{
	if (WNBELL)
	{
		if (bell_count)
			bell_count--;
		else
			CLRNBELL();
	}
	Timer_tms++;
	Timer_dms++;
	Timerms++;	// 暂未使用
	Timer_ms++; // 各函数中一般计时用
	Timer_ms1++;
	time++;		 // 本函数自用
	Times++;	 // 触摸延时用
	commtimes++; // 串口通讯接收等待时间
	if (DelayNms)
	{
		DelayNms--;
	}
	if (delaytimes)
		delaytimes--;
	if (Timerms1 < 0xffffffff)
		Timerms1++; //计算语音播放时间间隔

	if (gEnverify == 0)
	{
		if ((gverTim > (60 * 60 * 1000)) && (gverTim < (4 * 60 * 60 * 1000)))
		{
			gEnverify = 1;
		}
	}
	else
	{
		gverTimes++; //计算系统校验间隔时间
	}

	userLoninTime++;
	if (userLoninTime > 1000 * 60 * 30 && g_user.newmode == 1)
	{ //30分钟自动退出
		if (g_user.olduser > 0 && g_user.olduser < 6)
		{
			g_user.newuser = g_user.olduser;
			g_user.newmode = g_user.oldmode;
		}
		else
		{
			g_user.newuser = 0;
			g_user.newmode = 0;
		}

		g_user.olduser = 0;
		g_user.oldmode = 0;

		if (Write_Cmd3(C_USRLOG, g_user.newuser, g_user.newmode, g_user.olduser, g_user.oldmode) == 0)
		{ //发送登录指令
		}
		Read_Status();

		userLoninTime = 0;
	}

	g_Toast.curTime++;
	if (g_Toast.curTime > g_Toast.viewTime)
	{
		ToastHide();
	}
	if ((g_logorun < 10) && ((time % 1000) == 0))
	{
		g_logorunflat = 1;
	}
	if ((g_logorun > 99) && ((time % 150)) == 0)
	{
		g_logorunflat = 1;
	}
	if (WLEDFLAG)
	{
		if (TIM13->CCR1 < (TLCLK / 1000))
		{
			TIM13->CCR1 += 6;
		}
		else
		{
			CLRLEDFLAG();
		}
	}
	else
	{
		if (TIM13->CCR1 > 6)
		{
			TIM13->CCR1 -= 6;
		}
		else
		{
			SETLEDFLAG();
		}
	}
	if (diskoperatef == 0)
	{ //&&(sst39vf6401writeflag==0)){

		if (g_logorunflat)
		{
			g_logorunflat = 0;
			if ((g_logorun) && (g_logorun < 10))
			{
				Rotary_Logo(g_logorun);
				g_logorun++;
				if (g_logorun > 4)
					g_logorun = 1;
			}
			else if (g_logorun > 99)
			{
				DisAnimation(g_logorun);
				g_logorun++;
				if (g_logorun > 167)
					g_logorun = 100;
			}
		}

		// 输入滤波
		if (in_p >= IND_T)
			in_p = 0;
		in_buf[in_p] = (WINKEYD) + (WINCDATA) + (WINBDATA) + ((WINADATA) >> 5) + ((WINDDATA) >> 2) + ((WINEDATA) << 7);
		indh = 0xffffffff;
		indl = 0;
		for (inio = 0; inio < IND_T; inio++)
		{
			indh &= in_buf[inio];
			indl |= in_buf[inio];
		}
		inio = indh & indl;
		BIND |= inio;
		inio = indh | indl;
		BIND &= inio;
		BINND = ~BIND;
		in_p++;

		if (WWT_PENIRQN)
		{
			keydelay++;
			if (keydelay == 10)
			{
				keydelay = 60;
			}
			else if (keydelay > 60)
			{
				keydelay = 11;
				Read_TchData(0);
			}
			else if (Touchfshflag == 0xff)
			{
				Touchfshflag = 0;
				keydelay = 50;
			}
		}
		else
		{
			keydelay = 0;
			Touchfshflag = 0;
		}
	}
}
//------------------------------------------
// Function   : void resetKeyData(void)
// Description: 重置当前按键扫描的信息
//------------------------------------------
void resetKeyData(void)
{
	keyolddata = 0;
}

//------------------------------------------
// Function   : U16 get_keydata(KEYPAD *keyd,U8 keynum)
// Description: 按键服务程序
// note : 按键松开:0xff，无按键:0x00，屏保：0x01，其他按键: 0x02--0xFE
//------------------------------------------
U16 get_keydata(KEYPAD *keyd, U8 keynum, U8 basenum)
{
	U8 i;
	U16 xopis, yopis;
	if (gtTouch == 0)
	{ //没有使用电容触摸屏
		if ((keydelay > 10) && (Touchfshflag == 1))
		{ // 有按键,每50ms扫描一次
			//keyloopms = 0;
			if (Touch_AutoGet_XY())
			{ // 扫描到有效键值
				if (keyolddata)
				{ // 第n次扫描，n>=2
					if ((Touch_x > keyxmin) && (Touch_x < keyxmax) && (Touch_y > keyymin) && (Touch_y < keyymax))
					{ // 扫描到键值与上一次一样
						// i = keyolddata - basenum;//修改
						i = keyolddata;
						if (Touch_x < keyd[i].xpis)
							keyxpct = 0;
						else
							keyxpct = Touch_x - keyd[i].xpis;
						if (keyxpct > keyd[i].xsize)
							keyxpct = keyd[i].xsize;
						if (Touch_y < keyd[i].ypis)
							keyypct = 0;
						else
							keyypct = Touch_y - keyd[i].ypis;
						if (keyypct > keyd[i].ysize)
							keyypct = keyd[i].ysize;
						return (keyolddata);
					}
					else
					{
						// for (i = 0; i < keynum; i++)//修改
						for (i = 0 + basenum; i < keynum; i++)
						{
							xopis = keyd[i].xpis + keyd[i].xsize;
							yopis = keyd[i].ypis + keyd[i].ysize;
							if ((Touch_x > keyd[i].xpis) && (Touch_x < xopis) && (Touch_y > keyd[i].ypis) && (Touch_y < yopis))
							{
								break;
							} // + 1 keyolddata = i;return(keyolddata);
						}
						// if (i == keynum)//修改
						if (i == keynum)
						{ // 没有有效应用按键
							if (keyerrtimes > 3)
							{
								keyolddata = 0;
								keyerrtimes = 0;
								return (0);
							}
							else
							{
								keyerrtimes++;
								keyloopms = 22;
								return (keyolddata);
							}
						}
						else
						{ // 同一次按键中扫描到新的键值
							// keyolddata = i + basenum;//修改
							keyolddata = i;
							keyerrtimes = 0;
							keyxpct = Touch_x - keyd[i].xpis;
							keyypct = Touch_y - keyd[i].ypis;
							if (keyd[i].xpis > 20)
								keyxmin = keyd[i].xpis - 20;
							else
								keyxmin = 0;
							keyxmax = keyd[i].xpis + 20 + keyd[i].xsize;
							if (keyxmax > LCD_width - 1)
								keyxmax = LCD_width - 1;
							if (keyd[i].ypis > 20)
								keyymin = keyd[i].ypis - 20;
							else
								keyymin = 0;
							keyymax = keyd[i].ypis + 20 + keyd[i].ysize;
							if (keyymax > LCD_height - 1)
								keyymax = LCD_height - 1;
							return (keyolddata);
						}
					}
				}
				else
				{ // 第一次扫描
					// for (i = 0; i < keynum; i++)//修改
					for (i = 0 + basenum; i < keynum; i++)
					{
						xopis = keyd[i].xpis + keyd[i].xsize;
						yopis = keyd[i].ypis + keyd[i].ysize;
						if ((Touch_x > keyd[i].xpis) && (Touch_x < xopis) && (Touch_y > keyd[i].ypis) && (Touch_y < yopis))
						{
							break;
						} // + 1 keyolddata = i;return(keyolddata);
					}
					// if (i == keynum)//修改
					if (i == keynum)
					{ // 没有有效应用按键
						return (0);
					}
					else
					{ // 第一次扫描到有效铵
						// keyolddata = i + basenum;//修改
						keyolddata = i;
						keyerrtimes = 0;
						keyxpct = Touch_x - keyd[i].xpis;
						keyypct = Touch_y - keyd[i].ypis;

						if (keyd[i].xpis > 20)
							keyxmin = keyd[i].xpis - 20;
						else
							keyxmin = 0;
						keyxmax = keyd[i].xpis + 20 + keyd[i].xsize;
						if (keyxmax > LCD_width - 1)
							keyxmax = LCD_width - 1;
						if (keyd[i].ypis > 20)
							keyymin = keyd[i].ypis - 20;
						else
							keyymin = 0;
						keyymax = keyd[i].ypis + 20 + keyd[i].ysize;
						if (keyymax > LCD_height - 1)
							keyymax = LCD_height - 1;

						return (keyolddata);
					}
				}
			}
			else
			{ // 没有扫描到稳定键值，返回上一次键值
				//if(keyolddata)return(0xff);else return(0);
				return (keyolddata);
			}
		}
		else if (keydelay == 0)
		{ // 无按键
			if (keyolddata)
			{ // 按键松开
				keyolddata = 0;
				keyerrtimes = 0;
				return (0xff);
			}
			else
			{ // 没有按键
				return (0);
			}
		}
		else
		{ // 每50ms扫描一次按键，扫描延时期间返回上一次的扫描值
			return (keyolddata);
		}
	}
	else
	{ //电容触摸屏
		if (GT9xx_Scan())
		{ // 扫描到有效键值
			//printf("keyolddata = %d \r\n",keyolddata);
			if (wakenglag)
				return 0;
			if (keyolddata)
			{ // 第n次扫描，n>=2
				if ((gtTouch_x > keyxmin) && (gtTouch_x < keyxmax) && (gtTouch_y > keyymin) && (gtTouch_y < keyymax))
				{ // 扫描到键值与上一次一样
					// i = keyolddata - basenum;//修改
					i = keyolddata;
					if (gtTouch_x < keyd[i].xpis)
						keyxpct = 0;
					else
						keyxpct = gtTouch_x - keyd[i].xpis;
					if (keyxpct > keyd[i].xsize)
						keyxpct = keyd[i].xsize;
					if (gtTouch_y < keyd[i].ypis)
						keyypct = 0;
					else
						keyypct = gtTouch_y - keyd[i].ypis;
					if (keyypct > keyd[i].ysize)
						keyypct = keyd[i].ysize;
					return (keyolddata);
				}
				else
				{
					// for (i = 0; i < keynum; i++)//修改
					for (i = 0 + basenum; i < keynum; i++)
					{
						xopis = keyd[i].xpis + keyd[i].xsize;
						yopis = keyd[i].ypis + keyd[i].ysize;
						if ((gtTouch_x > keyd[i].xpis) && (gtTouch_x < xopis) && (gtTouch_y > keyd[i].ypis) && (gtTouch_y < yopis))
						{
							break;
						} // + 1 keyolddata = i;return(keyolddata);
					}
					if (i == keynum)
					{ // 没有有效应用按键
						if (keyerrtimes > 3)
						{
							keyolddata = 0;
							keyerrtimes = 0;
							return (0);
						}
						else
						{
							keyerrtimes++;
							keyloopms = 22;
							return (keyolddata);
						}
					}
					else
					{ // 同一次按键中扫描到新的键值
						// keyolddata = i + basenum;
						keyolddata = i;
						keyerrtimes = 0;
						keyxpct = gtTouch_x - keyd[i].xpis;
						keyypct = gtTouch_y - keyd[i].ypis;
						if (keyd[i].xpis > 20)
							keyxmin = keyd[i].xpis - 20;
						else
							keyxmin = 0;
						keyxmax = keyd[i].xpis + 20 + keyd[i].xsize;
						if (keyxmax > LCD_width - 1)
							keyxmax = LCD_width - 1;
						if (keyd[i].ypis > 20)
							keyymin = keyd[i].ypis - 20;
						else
							keyymin = 0;
						keyymax = keyd[i].ypis + 20 + keyd[i].ysize;
						if (keyymax > LCD_height - 1)
							keyymax = LCD_height - 1;
						return (keyolddata);
					}
				}
			}
			else
			{ // 第一次扫描
				// for (i = 0; i < keynum; i++)//修改
				for (i = 0 + basenum; i < keynum; i++)
				{
					xopis = keyd[i].xpis + keyd[i].xsize;
					yopis = keyd[i].ypis + keyd[i].ysize;
					//printf("gtTouch_x = %d gtTouch_y = %d  keyd[%d].xpis = %d xopis = %d keyd[%d].ypis = %d yopis = %d \r\n" ,gtTouch_x,gtTouch_y,i,keyd[i].xpis,xopis,i,keyd[i].ypis,yopis);
					if ((gtTouch_x > keyd[i].xpis) && (gtTouch_x < xopis) && (gtTouch_y > keyd[i].ypis) && (gtTouch_y < yopis))
					{
						break;
					} // + 1 keyolddata = i;return(keyolddata);
				}
				//printf("i = %d keynum = %d \r\n",i,keynum);
				if (i == keynum)
				{ // 没有有效应用按键
					return (0);
				}
				else
				{ // 第一次扫描到有效铵
					// keyolddata = i + basenum;
					keyolddata = i;
					keyerrtimes = 0;
					keyxpct = gtTouch_x - keyd[i].xpis;
					keyypct = gtTouch_y - keyd[i].ypis;
					if (keyd[i].xpis > 20)
						keyxmin = keyd[i].xpis - 20;
					else
						keyxmin = 0;
					keyxmax = keyd[i].xpis + 20 + keyd[i].xsize;
					if (keyxmax > LCD_width - 1)
						keyxmax = LCD_width - 1;
					if (keyd[i].ypis > 20)
						keyymin = keyd[i].ypis - 20;
					else
						keyymin = 0;
					keyymax = keyd[i].ypis + 20 + keyd[i].ysize;
					if (keyymax > LCD_height - 1)
						keyymax = LCD_height - 1;
					return (keyolddata);
				}
			}
		}
		else if (keyolddata)
		{ // 按键松开
			keyolddata = 0;
			keyerrtimes = 0;
			wakenglag = 0;
			return (0xff);
		}
		else
		{ // 没有按键
			wakenglag = 0;
			return (0);
		}
	}
}

//#if Debug
//U8 DebLog[64] ={0};
//void DebugLog(U8 *log,U8 newfile)
//{
//	U8 *p,res;
//	U8 FileType;
//	U16 len =0;
//	U16 bytesWritten;
//	if(newfile) FileType = FA_CREATE_ALWAYS;	//创建信新文件
//	else FileType = FA_OPEN_ALWAYS;
//	while((*(log+len++)) &&(len<64));
////	printf("len = %d \r\n",len);
//	f_mkdir("0:/BebugLog");
//	if(f_open(&file,"0:/BebugLog/BebugLog.txt",FileType | FA_WRITE) == FR_OK){
//		p = (U8 *)log;
//		if(newfile==0)	f_lseek(&file,file.fsize-1);  //
//		while(len){
//			if(HCD_IsDeviceConnected(&USB_OTG_Core)==0){break;}
//
//			if(len>511)res= f_write (&file, p, 512, (void *)&bytesWritten);
//			else res= f_write (&file, p, len, (void *)&bytesWritten);
//
//			if((bytesWritten == 0) || (res != FR_OK)){
//				break;
//			}
//			len -= bytesWritten;
//			p += bytesWritten;
//			wdt();
//		}
//	}
//	f_close(&file);
//}
//#endif
