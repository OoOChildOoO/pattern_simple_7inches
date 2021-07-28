#include "usb_bsp.h"
	   


#define USE_ACCURATE_TIME
#define TIM_MSEC_DELAY       0x01
#define TIM_USEC_DELAY       0x02








#ifdef USE_ACCURATE_TIME 
static void BSP_SetTime(uint8_t Unit);
static void BSP_Delay(uint32_t nTime,uint8_t Unit);
static void USB_OTG_BSP_TimeInit ( void );
#endif

#define TIMX					TIM7
#define USB_OTG_BSP_TimerIRQ	TIM7_IRQHandler
#ifdef USE_ACCURATE_TIME 
__IO uint32_t BSP_delay = 0;
#endif
  
//USB OTG 底层IO初始化
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
//	RCC->AHB1ENR|=1<<0;    		//使能PORTA时钟	    
// 	RCC->AHB2ENR|=1<<7;    		//使能USB OTG时钟	
//	GPIO_Set(GPIOA,3<<11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);	//PA11/12复用功能输出	
//	GPIO_Set(GPIOA,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);	//PA15推挽输出	
//  	PCF8574_WriteBit(USB_PWR_IO,1);//开启USB HOST电源供电
//   	GPIO_AF_Set(GPIOA,11,10);	//PA11,AF10(USB)
// 	GPIO_AF_Set(GPIOA,12,10);	//PA12,AF10(USB)
	USB_OTG_BSP_TimeInit();
}

//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
//   	MY_NVIC_Init(0,3,OTG_FS_IRQn,2);	//优先级设置为抢占0,子优先级3，组2	 
}

//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_DisableInterrupt(void)
{ 
}
//USB OTG 端口供电设置(本例程未用到)
//pdev:USB OTG内核结构体指针
//state:0,断电;1,上电
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
}
//USB_OTG 端口供电IO配置(本例程未用到)
//pdev:USB OTG内核结构体指针
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{ 
} 
//USB_OTG us级延时函数
//本例程采用SYSTEM文件夹的delay.c里面的delay_us函数实现
//官方例程采用的是定时器2来实现的.
//usec:要延时的us数.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
  #ifdef USE_ACCURATE_TIME    
	BSP_Delay(usec,TIM_USEC_DELAY); 
  #else
	__IO uint32_t count = 0;
	const uint32_t utime = (120 * usec / 7);
	
	do{
		if ( ++count > utime ){
			return ;
		}
	}while (1);
  #endif 
}

//------------------------------------------
// Function   : _uDelay				
// Description: This function provides delay time in micro sec					
//------------------------------------------
#ifndef USE_ACCURATE_TIME
static void _uDelay (uint32_t usec)
{
	__IO uint32_t count = 0;
	const uint32_t utime = (120 * usec / 7);
	do{
		if ( ++count > utime ){
			return ;
		}
	}while (1);  
}
#endif
//USB_OTG ms级延时函数
//本例程采用SYSTEM文件夹的delay.c里面的delay_ms函数实现
//官方例程采用的是定时器2来实现的.
//msec:要延时的ms数.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
  #ifdef USE_ACCURATE_TIME  
	BSP_Delay(msec,TIM_MSEC_DELAY);   
  #else
	USB_OTG_BSP_uDelay(msec * 1000);   
  #endif  
}

//------------------------------------------
// Function   : USB_OTG_BSP_TimerIRQ				
// Description: Time base IRQ					
//------------------------------------------
void USB_OTG_BSP_TimerIRQ (void)
{
#ifdef USE_ACCURATE_TIME 
	if(TIMX->SR&0x0001){
		TIMX->SR = 0x0000;
		if (BSP_delay > 0x00){ 
			BSP_delay--;
		}else{
			TIMX->CR1 = 0x0000;	
		}
	}
#endif  
} 

#ifdef USE_ACCURATE_TIME 

//------------------------------------------
// Function   : BSP_Delay				
// Description: Delay routine based on TIM2					
//------------------------------------------
static void BSP_Delay(uint32_t nTime, uint8_t unit)
{  
	BSP_delay = nTime;
	BSP_SetTime(unit);  
	while(BSP_delay != 0);
	TIMX->CR1 = 0x0000;
}

//------------------------------------------
// Function   : BSP_SetTime				
// Description: Configures TIMX for delay routine based on TIMX					
//------------------------------------------
static void BSP_SetTime(uint8_t unit)
{
	TIMX->CR1 = 0x0000;
	TIMX->CR2 = 0x0000;
	TIMX->SMCR = 0x0000;
	TIMX->DIER = 0x0001;
	TIMX->EGR = 0x0001;
	TIMX->PSC = 0x0007;//5
	TIMX->RCR = 0x0000;
	if(unit == TIM_USEC_DELAY){//2  
		TIMX->ARR = 12;//9
	}else if(unit == TIM_MSEC_DELAY){//1
		TIMX->ARR = 12000;//9999
	}
	TIMX->CR1 = 0x0001; 
} 

#endif

static void USB_OTG_BSP_TimeInit ( void )
{
/*
  #ifdef USE_ACCURATE_TIME   
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// Set the Vector Table base address at 0x08000000 //
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	
	// Configure the Priority Group to 2 bits //
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// Enable the TIM2 gloabal Interrupt //
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  #endif
*/
// 以上在system_init(void)统一初始化 
}




















