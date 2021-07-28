//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Mit_menu_files.c							*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision5 V5.21					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*


#include "Mit_menu_files.h"
#include "Mit_funtion.h"
#include "exfunction.h"
#include "Patfun.h"
#include <string.h>
#include "function.h"
#include "ScrollBar.h"
#include "menu.h"
#include "color.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "communication.h"
#include "viewPattern.h"

volatile U8  g_disFileMethod;	//��ʾ�ļ���ʽ 0-�б�ģʽ 1-ͼ��ģʽ
volatile U16 g_SelFileNo;
volatile U16 g_selfiles[MAXFILENO];	//ѡ���ļ�
volatile U16 g_editfiles[MAXFILENO];//�༭�ļ�
volatile U16 g_btndisable;			//���ð���
volatile U8 g_checkMode;			//��ѡ/��ѡģʽ
volatile U8 g_selALL;	//ȫѡ
volatile U8 g_from_memory;
volatile U8 g_to_memory;
volatile U8 g_clipboard;	//���а�
volatile U8 g_savePathTimes;
char* g_path;
char* g_fromPath;
char* g_savePath;
volatile U8 g_newname[64];
volatile U8 g_oldname[64];

extern uint8_t getDiskFiles(char* path,U16 *indexReg,uint8_t ftype);
extern void Select_ExtMemory(uint8_t usbsd);
extern uint8_t ExtMemory_Status(uint8_t usbsd);
extern uint8_t readDiskPatFileByName(char* path,char *fname,U16 *sreg);
extern uint8_t newDiskFolder(char* path,char *name);
extern uint8_t renameDiskFile(char* path,char *oldName,char *newName);

extern volatile U8 g_disk_memory;
extern volatile U8 g_patname[64];
extern volatile U8  g_init_flag;
extern PATHEAD	 g_patd;
extern volatile U16 g_selectdisno;

#if 0
#define		KEY_FILE_SCRNSVR		1
#define		KEY_FILE_BACK			2
#define		KEY_FILE_MEM			3
#define		KEY_FILE_ML				4
#define		KEY_FILE_MR				5
#define		KEY_FILE_UDIR			6
#define		KEY_FILE_LIST			7
#define 	KEY_FILE_ICON			8
#define		KEY_FILE_ADDFOLDER		9
#define		KEY_FILE_EDITNAME		10
#define		KEY_FILE_DEL			11
#define		KEY_FILE_COPY			12
#define		KEY_FILE_CUT			13
#define		KEY_FILE_PASTE			14
#define		KEY_FILE_EDITFILE		15
#define		KEY_FILE_CHECK			16
#define		KEY_FILE_IMPORT			17
#define		KEY_FILE_EXPORT			18
#define		KEY_FILE_FUP			19
#define		KEY_FILE_FDN			20
#define 	KEY_FILE_SLIDER			21
#define 	KEY_FILE_LIST1			22
#define		KEY_FILE_LIST2			23
#define		KEY_FILE_LIST3			24
#define 	KEY_FILE_LIST4			25
#define 	KEY_FILE_LIST5			26
#define		KEY_FILE_LIST6			27
#define		KEY_FILE_LIST7			28
#define 	KEY_FILE_LIST8			29
#define 	KEY_FILE_LIST9			30

#define 	KEY_FILE_FSORT			31
#define 	KEY_FILE_SALL			32

#define 	KEY_FILE_ICON1			33
#define		KEY_FILE_ICON2			34
#define		KEY_FILE_ICON3			35
#define 	KEY_FILE_ICON4			36
#define 	KEY_FILE_ICON5			37
#define		KEY_FILE_ICON6			38
#define 	KEY_FILE_ICON7			39
#define 	KEY_FILE_ICON8			40
#define 	KEY_FILE_ICON9			41

#define 	KEY_FILE_INMEM			42
#define 	KEY_FILE_USB			43
#define 	KEY_FILE_FORMAT			44

//------------------------------------------
// Function   : void Mit_Menu_File(void)		
// Description: �ļ�����				
//------------------------------------------
void Mit_Menu_File(void)
{	
	U8  beeponf=0;
	U16 delay=0;
	U32 keyon=0,keyoff=0,keydata=0;
	U32 i,j;
	S32 ii;
	U8 diskstatus =0;
	S16 disFilebase =0,odisFilebase =1000;
//	U16 slidivlen = 0,oslidivlen =0,slidlen = 0;
	U16 sheet = 0;
	U8 f=0;
	U8 pagmax = 9;
	U8 contentUpdata1 =1,contentUpdata2 =0;
	U8 SelNo =0,oSelNo =0;
	U8 Sort = 0;
	U8 disupdata0 = 1,disupdata1 =1,disupdata2 =1,disupdata3 =1,disupdata4 =1,disupdata5 =1,disupdata6 =1,disupdata7 =0,/*disupdata8 =0,*/disupdata9 =0; 
	U8 disupdata10 =0,disupdata11 =0,disupdata12 =0,disupdata13=1,disupdata14 =1; 
	U16 totalfiles =0;
	U32 dataadd[9];
	U8 tdisname[65] ={0},*tname,namelen =0;
	U8 seldisking =0;
	U16 FileNO =0;
	U16 SelFileNo,oSelFileNo ;
	U8 odisk_memory = g_disk_memory;
	U8 fileType = 0;
	U32 fileSize;
	U16 tmplen;
	U32 num1 ;
	U32 num2 ;
	U8 MemIn[3] ={1,0,1};
	U8 oMemIn[3] ={1,0,1};
	U16 selnums =0,oselnums =0;
	U16 errtimes =0;
	U8 fileStep;	//�ļ��㼶
	U16 tsavePathTimes;
	U8 conflag = 1;
	//U8 readflag =0;	// 0-����Ҫ���¶�����,1-��Ҫ���¶�����
	ScrollBar tScBar ={546,172,KEYX17,316,2,2,Color_White,Color_Gray};
	const U8 BorName[12] = {"ISMS0XXX.SEW"};
	
#if USEMULTCOLOR	//��ɫ
	const U8 keynum = {33};
//	const KEYPAD keyd[] = {{0,0,0,0},{0,0,128,32},{520,735,KEYX1,KEYY1},{4,36,64,40},{448,36,KEYX17,KEYY18},{498,36,KEYX17,KEYY18},{548,36,KEYX17,KEYY18},{100,698,KEYX17,KEYX17},{100,748,KEYX17,KEYX17},
//						   {160,698,KEYX17,KEYX17},{210,698,KEYX17,KEYX17},{260,698,KEYX17,KEYX17},{310,698,KEYX17,KEYX17},{360,698,KEYX17,KEYX17},{160,748,KEYX17,KEYX17},{210,748,KEYX17,KEYX17},{260,748,KEYX17,KEYX17},{310,748,KEYX17,KEYX17},{360,748,KEYX17,KEYX17},
//							{666,124,KEYX18,KEYX18},{666,312,KEYX18,KEYX18},{670,172,36,108},{4,124,660,44},{4,170,660,44},{4,216,660,44},{4,262,660,44},{4,308,660,44},{47,78,619,40},{4,78,40,40}};
	const KEYPAD keyd[] = {{0,0,0,0},{0,0,128,32},{1,735,KEYX1,KEYY1},{4,36,64,40},{450,36,KEYX17,KEYY18},{499,36,KEYX17,KEYY18},{548,36,KEYX17,KEYY18},{152,684,KEYX21,KEYY21},{152,740,KEYX21,KEYY21},
						   {334,684,KEYX21,KEYX21},{277,684,KEYX21,KEYX21},{391,684,KEYX21,KEYX21},{448,684,KEYX21,KEYX21},{220,740,KEYX21,KEYX21},{277,740,KEYX21,KEYX21},{334,740,KEYX21,KEYX21},{220,684,KEYX21,KEYX21},{391,740,KEYX21,KEYX21},{448,740,KEYX21,KEYX21},
							{546,124,KEYX17,KEYX17},{546,488,KEYX17,KEYX17},{548,174,44,312},{4,124,538,44},{4,170,538,44},{4,216,538,44},{4,262,538,44},{4,308,538,44},{4,354,538,44},{4,400,538,44},{4,446,538,44},{4,492,538,44},{47,78,549,40},{4,78,40,40}};
	
	const U16 keybmpind[] = {0,0,1190,0,611,612,609,613,615,721,723,725,727,729,731,733,735,739,737,587,585};	
	
	const U8 keynum1 = {9};
	const KEYPAD keyd1[]={{5,124,178,136},{185,124,178,136},{365,124,178,136},{5,262,178,136},{185,262,178,136},{365,262,178,136},{5,400,178,136},{185,400,178,136},{365,400,178,136}};

	const U8 keynum2 = {3};
	const KEYPAD keyd2[]={{4,78,260,KEYX18},{4,120,260,KEYX18},{4,178,260,KEYX18}};
	
#else
	const U8 keynum = {33};
//	const KEYPAD keyd[] = {{0,0,0,0},{0,0,128,32},{520,735,KEYX1,KEYY1},{4,36,64,40},{448,36,KEYX17,KEYY18},{498,36,KEYX17,KEYY18},{548,36,KEYX17,KEYY18},{100,698,KEYX17,KEYX17},{100,748,KEYX17,KEYX17},
//						   {160,698,KEYX17,KEYX17},{210,698,KEYX17,KEYX17},{260,698,KEYX17,KEYX17},{310,698,KEYX17,KEYX17},{360,698,KEYX17,KEYX17},{160,748,KEYX17,KEYX17},{210,748,KEYX17,KEYX17},{260,748,KEYX17,KEYX17},{310,748,KEYX17,KEYX17},{360,748,KEYX17,KEYX17},
//							{666,124,KEYX18,KEYX18},{666,312,KEYX18,KEYX18},{670,172,36,108},{4,124,660,44},{4,170,660,44},{4,216,660,44},{4,262,660,44},{4,308,660,44},{47,78,619,40},{4,78,40,40}};
	const KEYPAD keyd[] = {{0,0,0,0},{0,0,128,32},{1,735,KEYX1,KEYY1},{4,36,64,40},{448,36,KEYX17,KEYY18},{498,36,KEYX17,KEYY18},{548,36,KEYX17,KEYY18},{152,682,KEYX21,KEYY21},{152,740,KEYX21,KEYY21},
						   {336,682,KEYX21,KEYX21},{278,682,KEYX21,KEYX21},{394,682,KEYX21,KEYX21},{452,682,KEYX21,KEYX21},{220,740,KEYX21,KEYX21},{278,740,KEYX21,KEYX21},{336,740,KEYX21,KEYX21},{220,682,KEYX21,KEYX21},{394,740,KEYX21,KEYX21},{452,740,KEYX21,KEYX21},
							{546,124,KEYX17,KEYX17},{546,488,KEYX17,KEYX17},{548,174,44,312},{4,124,538,44},{4,170,538,44},{4,216,538,44},{4,262,538,44},{4,308,538,44},{4,354,538,44},{4,400,538,44},{4,446,538,44},{4,492,538,44},{47,78,549,40},{4,78,40,40}};
	
	const U16 keybmpind[] = {0,0,1190,0,611,612,609,613,615,721,723,725,727,729,731,733,735,739,737,1089,1087};	
	
	const U8 keynum1 = {9};
	const KEYPAD keyd1[]={{5,124,178,136},{185,124,178,136},{365,124,178,136},{5,262,178,136},{185,262,178,136},{365,262,178,136},{5,400,178,136},{185,400,178,136},{365,400,178,136}};

	const U8 keynum2 = {3};
	const KEYPAD keyd2[]={{4,78,260,KEYX18},{4,120,260,KEYX18},{4,178,260,KEYX18}};

#endif	


		
	SetLanguage(MENU_FTFILES);
	InitScreen();	
	
	ScrollBarInit(tScBar);
	ScrollBarSetMindiv(6);//��С�ֶ�ֵ,6�����ص�
	Draw24x24Str(TITLE_XPOS_BOR,4,MEM_TITLE_ADD,SheetPage,0,Color_White);	//�ļ�ϵͳ
#if USEMULTCOLOR	//��ɫ
	DrawRect(71,36,374,40,Color_Black);
	DrawRect(150,682,356,116,Color_Black);
	DrawDFrame(0,120, 598, 542, 1,Color_Gray);	
	for(i=2;i<KEY_FILE_SLIDER;i++){	
		if(i<KEY_FILE_LIST || i>KEY_FILE_EXPORT || i==KEY_FILE_BACK){
			DrawButton2(keyd[i],Color_White1,keybmpind[i],i==KEY_FILE_BACK?1:0);
		}		
	} 	
#else
	DrawRect(71,36,374,40,Color_Black);
	DrawRect(150,680,360,118,Color_Black);
	DrawDFrame(0,120, 598, 542, 1,Color_Gray);	
	for(i=2;i<KEY_FILE_SLIDER;i++){	
		if(i<KEY_FILE_LIST || i>KEY_FILE_EXPORT || i==KEY_FILE_BACK){
			DrawButton2(keyd[i],(i<KEY_FILE_FUP)?Color_White1:Color_Gray,keybmpind[i],i==KEY_FILE_BACK?1:0);
		}		
	} 
#endif

	


//	slidlen = keyd[KEY_FILE_SLIDER].ysize;
	SelFileNo = g_SelFileNo;
	oSelFileNo = SelFileNo;
	
	//if(g_disFileMethod) pagmax = 8;else pagmax = 9;
	selnums = getFiles((U16*)g_selfiles,MAXFILENO);	
	fileStep = getPathStep(g_path); 
	tsavePathTimes = getSavePathTimes(g_savePath);
//	DrawFillRect(420,78,95,40,Color_Gray);
//	Draw24x24Str(424,86,MEM_TITLE_ADD+2*TitleLengthMax,SheetPage,2,Color_White);	//����
//	
//	DrawFillRect(518,78,148,40,Color_Gray);
//	Draw24x24Str(522,86,MEM_TITLE_ADD+3*TitleLengthMax,SheetPage,3,Color_White);	//����

	DrawFillRect(keyd[KEY_FILE_FSORT].xpis,keyd[KEY_FILE_FSORT].ypis,keyd[KEY_FILE_FSORT].xsize,keyd[KEY_FILE_FSORT].ysize,Color_SkyBlue);	
	Draw24x24Str(keyd[KEY_FILE_FSORT].xpis+16,keyd[KEY_FILE_FSORT].ypis+8,MEM_TITLE_ADD+1*TitleLengthMax,SheetPage,1,Color_White);	//�ļ���
	
	DrawFillRect(keyd[KEY_FILE_SALL].xpis,keyd[KEY_FILE_SALL].ypis,keyd[KEY_FILE_SALL].xsize,keyd[KEY_FILE_SALL].ysize,Color_Gray);	
	DrawRect(keyd[KEY_FILE_SALL].xpis+8,keyd[KEY_FILE_SALL].ypis+8,24,24,Color_Black);	
	
	DrawFillRect(keyd[KEY_FILE_MEM].xpis+16,keyd[KEY_FILE_MEM].ypis+4,32,32,mCOLOR?Color_Silver:Color_White1);
	DrawFPBMPBTN(keyd[KEY_FILE_MEM].xpis+16,keyd[KEY_FILE_MEM].ypis+4,32,32,g_disk_memory==MEM_IN?617:/*(g_disk_memory==MEM_DSP?1294:*/g_disk_memory+715, 0xffff);
	
	Timer_dms = 0;
	ViewPage(usepageno);
		
	
	while(1){		
		keydata = get_keydata((KEYPAD *)keyd,keynum,0);
		if((keydata>=KEY_FILE_LIST1)&&(keydata<=KEY_FILE_LIST9)&&(g_disFileMethod==1))	{
			keydata = 0; keyolddata = 0;
			keydata = get_keydata((KEYPAD *)keyd1,keynum1,KEY_FILE_ICON1);
		}
		
		if(seldisking){		//����ѡ��
			if((keydata==KEY_FILE_SALL)||(keydata==KEY_FILE_FSORT)||((keydata>=KEY_FILE_LIST1)&&(keydata<=KEY_FILE_LIST4))
				||(keydata==KEY_FILE_ICON1)||(keydata==KEY_FILE_ICON2)||(keydata==KEY_FILE_ICON5)||(keydata==KEY_FILE_ICON6)		
			){
				keydata = 0; keyolddata = 0;
				keydata = get_keydata((KEYPAD *)keyd2,keynum2,KEY_FILE_INMEM);
			}
			
			if(((keydata>KEY_FILE_UDIR)&&(keydata<KEY_FILE_INMEM))||(keydata==KEY_FILE_BACK)||((keydata==KEY_FILE_USB)&&(MemIn[1]==0))){keydata =0;}
		}
		
		if(keydata>=KEY_FILE_LIST&& keydata<=KEY_FILE_EXPORT){	
			if(g_btndisable&(0x1<<(keydata-KEY_FILE_LIST))){keydata = 0;}				
		}else if(keydata==KEY_FILE_SALL && g_checkMode ==0) {keydata = 0;
		}else if(keydata==KEY_FILE_UDIR && fileStep == 0){keydata = 0;
		}else if((g_savePathTimes==0 && keydata==KEY_FILE_ML)||(g_savePathTimes>=tsavePathTimes && keydata==KEY_FILE_MR)){keydata = 0;}
			
		if(keydata==0xff){
			if((keyon==KEY_FILE_FUP)||(keyon==KEY_FILE_FDN)||(keyon==KEY_FILE_SLIDER)){}
			else if((keyon)&&(beeponf)){keyoff = keyon;}
			keydata = 0;
		}else if(keydata>USESCREENSAVER){
			if(keyon==0){
				keyon = keydata;
//			#if Debug
//				printf("Mit_Menu_File	keyon = %d seldisking = %d\r\n",keyon,seldisking); 
//			#endif
				if(keyon==KEY_FILE_SCRNSVR);
				else{
					if(keydata<KEY_FILE_SLIDER){
						if((keydata>=KEY_FILE_MEM)&&(keydata<=KEY_FILE_EXPORT)){
							DrawButtonOn(keyd[keydata],KEY_White);
							if(keydata==KEY_FILE_CHECK && g_checkMode==1){
								DrawFMenuStr(22);
							}else{								
								DrawFMenuStr(keydata-1);
							}							
						}else if((keydata>=KEY_FILE_FUP)&&(keydata<=KEY_FILE_FDN)){
							DrawButtonOn(keyd[keydata],KEY_Gray);
						}else if(keydata==KEY_FILE_BACK){
							DrawButtonOn(keyd[keydata],KEY_White);
						}else{
							DrawButtonOn(keyd[keydata],KEY_Silver);
						}
					}else if((keydata>=KEY_FILE_INMEM) &&(keydata<=KEY_FILE_FORMAT)){
						FillRectColorChang(keyd2[keydata-KEY_FILE_INMEM].xpis,keyd2[keydata-KEY_FILE_INMEM].ypis,keyd2[keydata-KEY_FILE_INMEM].xsize,keyd2[keydata-KEY_FILE_INMEM].ysize,Color_White,Color_LBlue);
					}
				}
				Times = 0;
				delay = KEYDT;
			}else if(keyon!=keydata){
				keydata = 0;
			}

			if(Times>delay){
				if(beeponf==0){
					beeponf=1;if(P_KBC==0)Beep_On(BELLT);					
					delay = KEYFT;
				}else if(beeponf==1){
					beeponf=2;
					delay = KEYNT;
				}
				Times = 0;
				if((keyon==KEY_FILE_FUP)||(keyon==KEY_FILE_FDN)||(keyon==KEY_FILE_SLIDER)){
					keyoff = keyon;
				}
			}
		}else{
			delay = KEYDT;
		}
		
		if(keyoff){	
		
			if(((keyoff>=KEY_FILE_LIST1)&&(keyoff<=KEY_FILE_LIST9)) ||((keyoff>=KEY_FILE_ICON1)&&(keyoff<=KEY_FILE_ICON9)) ){
				oSelNo = SelNo;
				if(g_disFileMethod==0){
					SelNo = keyoff- KEY_FILE_LIST1;
				}else{
					SelNo = keyoff- KEY_FILE_ICON1;
				}				
				if(SelNo+disFilebase<totalfiles){				
					if(g_checkMode){
						SelFileNo = dataadd[SelNo]/FileIndexSize+MINFILENO;
						oSelFileNo = SelFileNo; 
						oselnums = getFiles((U16*)g_selfiles,MAXFILENO);					
						if(g_selfiles[dataadd[SelNo]/FileIndexSize] !=0xffff){
							g_selfiles[dataadd[SelNo]/FileIndexSize] = 0xffff;
							if(g_disFileMethod==0){
								DrawFillRect(keyd[SelNo+KEY_FILE_LIST1].xpis+9,keyd[SelNo+KEY_FILE_LIST1].ypis+9,22,22,RGB(250,250,250));	
								FillRectColorChang(keyd[SelNo+KEY_FILE_LIST1].xpis,keyd[SelNo+KEY_FILE_LIST1].ypis,keyd[SelNo+KEY_FILE_LIST1].xsize,keyd[SelNo+KEY_FILE_LIST1].ysize,Color_LBlue,Color_White);	
							}else{
								DrawFillRect(keyd1[SelNo].xpis+5,keyd1[SelNo].ypis+109,22,22,RGB(250,250,250));	
								FillRectColorChang(keyd1[SelNo].xpis,keyd1[SelNo].ypis,keyd1[SelNo].xsize,keyd1[SelNo].ysize,Color_LBlue,Color_White);								
							}
						}else{
							g_selfiles[dataadd[SelNo]/FileIndexSize] =  SelFileNo;
							if(g_disFileMethod==0){
								DrawFPBMPBTN(keyd[SelNo+KEY_FILE_LIST1].xpis+9,keyd[SelNo+KEY_FILE_LIST1].ypis+9,22,22,596, 0xffff);
								FillRectColorChang(keyd[SelNo+KEY_FILE_LIST1].xpis,keyd[SelNo+KEY_FILE_LIST1].ypis,keyd[SelNo+KEY_FILE_LIST1].xsize,keyd[SelNo+KEY_FILE_LIST1].ysize,Color_White,Color_LBlue);							
							}else{
								DrawFPBMPBTN(keyd1[SelNo].xpis+5,keyd1[SelNo].ypis+109,22,22,596, 0xffff);
								FillRectColorChang(keyd1[SelNo].xpis,keyd1[SelNo].ypis,keyd1[SelNo].xsize,keyd1[SelNo].ysize,Color_White,Color_LBlue);														
							}
						}	
						selnums = getFiles((U16*)g_selfiles,MAXFILENO);	
						if(oselnums!=selnums){
							if(oselnums==0 || selnums==0){
								if(oselnums==0){
									if(g_btndisable != clrBit16(g_btndisable,(1<<4)|(1<<5)|(1<<6))){	//ʹ��5/6/7����
										g_btndisable = clrBit16(g_btndisable,(1<<4)|(1<<5)|(1<<6));
										disupdata0 = 1;
									}									
									if(g_disk_memory==MEM_IN/* || g_disk_memory==MEM_DSP*/){	//�ڴ�/�����
										if(g_btndisable != clrBit16(g_btndisable,(1<<11))){	//ʹ��12����
											g_btndisable = clrBit16(g_btndisable,(1<<11));
											disupdata0 = 1;
										}
									}else{	//�ⲿ�豸
										if(g_btndisable != clrBit16(g_btndisable,(1<<10))){	//ʹ��11����
											g_btndisable = clrBit16(g_btndisable,(1<<10));
											disupdata0 = 1;
										}
									}
								}else{
									if(setBit16(g_btndisable,(1<<4)|(1<<5)|(1<<6)) != g_btndisable){	//����5/6/7����
										g_btndisable = setBit16(g_btndisable,(1<<4)|(1<<5)|(1<<6));
										disupdata0 = 1;
									}
									if(g_disk_memory==MEM_IN /*|| g_disk_memory==MEM_DSP*/){	//�ڴ�
										if(setBit16(g_btndisable,(1<<11)) != g_btndisable){	//����12����
											g_btndisable = setBit16(g_btndisable,(1<<11));
											disupdata0 = 1;
										}									
									}else{	//�ⲿ�豸
										if(setBit16(g_btndisable,(1<<10)) != g_btndisable){	//����11����
											g_btndisable = setBit16(g_btndisable,(1<<10));
											disupdata0 = 1;
										}
									}
								}
								//g_clipboard = 0;
								disupdata0 = 1;
							}
						}
						disupdata11 = 1;
					}else{	
						fileType = getFileTypeByIREG((U16*)IREG2,dataadd[SelNo]/FileIndexSize+MINFILENO);

						if(fileType==FILE_MITE || fileType==FILE_MITG || fileType==FILE_SEW/* || fileType==FILE_EMB*/){	//����/�ֵܸ�ʽ
							contentUpdata2 =1;								
							if(g_btndisable != clrBit16(g_btndisable,(1<<3)|(1<<4)|(1<<5)|(1<<6))){	//ʹ��4/5/6/7����
								g_btndisable = clrBit16(g_btndisable,(1<<3)|(1<<4)|(1<<5)|(1<<6));
								disupdata0 = 1;
							}		
							if(g_disk_memory==MEM_IN){	//�ڴ�
								if(g_btndisable != clrBit16(g_btndisable,(1<<11))){	//ʹ��12����
									g_btndisable = clrBit16(g_btndisable,(1<<11));
									disupdata0 = 1;
								}
							}else{	//�ⲿ�豸
								if(g_btndisable != clrBit16(g_btndisable,(1<<10))){	//ʹ��11����
									g_btndisable = clrBit16(g_btndisable,(1<<10));
									disupdata0 = 1;
								}
							}							
						}else{
							SelFileNo = dataadd[SelNo]/FileIndexSize+MINFILENO;
							if(fileType==FILE_FOLDER && SelFileNo == oSelFileNo){	//�����ļ���
								setBufDat8((U8*)g_patname,64,0);
								tname = (U8*)(&IREG2[dataadd[SelNo]+FileIndexNamepox]);
								namelen =0;
								while((*(tname+namelen)!=0)&&(namelen<63)){
									g_patname[namelen] = *(tname+namelen);
									namelen++;
								}	
								strcat(g_path,"/");								
								strcat(g_path,(char*)g_patname); 	
								getDiskFiles(g_path,(U16*)IREG2,GET_ALLFILE);
								SelFileNo = 0xffff;
								oSelFileNo = 0xffff; 
								contentUpdata1 =1;
								disupdata4 =1;
								disupdata1 = 1;
								g_btndisable = 0xdf8;	//����4/5/6/7/8/9/11/12����
								disupdata0 = 1;
								if(g_clipboard){
									g_btndisable&= ~(1<<7);
								}
								disupdata7 =1;
								fileStep = getPathStep(g_path);
								disFilebase =0;
								disupdata13 = 1;
								g_savePathTimes++;
								tsavePathTimes = g_savePathTimes;
								savePath(g_savePath,g_path,g_savePathTimes,g_disk_memory);	
								disupdata14 = 1;								
							}else{
								disupdata4 = 1;	
								if(g_btndisable != clrBit16(g_btndisable,(1<<3)|(1<<4)|(1<<5)|(1<<6))){	//ʹ��4/5/6/7����
									g_btndisable = clrBit16(g_btndisable,(1<<3)|(1<<4)|(1<<5)|(1<<6));
									disupdata0 = 1;
								}							
							}
						}
												

					}
					
				}else{
					SelNo = oSelNo;
				}														
			}else{
				switch(keyoff){
					case KEY_FILE_SCRNSVR:
						pno	= MENU_SCRNSVR;
						break;
					
					case KEY_FILE_BACK:
						if(g_disk_memory!=MEM_IN){	//�ڴ�
							g_disk_memory = MEM_IN; 
							f = ReadFileIndex(C_RDMEMPATLIST,MENU_FTFILES);
							if(f==0){
								pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;break;
							}
						}
						//�ض����һ���Ļ���
						//printf("g_selectdisno = %d \r\n",g_selectdisno);
						if(isContainFileByIREG((U16*)IREG2,g_selectdisno)==0){	//ԭ��������ɾ��
							g_selectdisno = get_ExitPatNO((U16*)IREG2,0);					
						}						
						if(Write_Cmd(C_READMEMORYPAT,g_selectdisno,0)==0){
							pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;break;
						}	
						if(Read_Status()==0){
							pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;break;
						}else if(DREG[D_CMDSTATUS]!=100){
							pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;break;
						}	
						
						i = Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&HREG[0]));				// ����ǰ�����ļ���Ϣ
							
						if(i==0){
							pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;break;
						}
						if(g_selectdisno!=HREG[0]){
							pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;break;
						}
						i = (g_selectdisno-MINFILENO)*FileIndexSize;
						i = (((U32)(IREG2[i+1])<<16)+IREG2[i])/2;						
						j = Read_D(D_Address[PTDATA],i,(U16 *)(&SREG[0]));							// ����ǰ�����ļ�

						if(j==0){
							pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;break;
						}
								
						pno	= MENU_FUNCTION;pnoindex = 1;	
						break;
					
					case KEY_FILE_MEM:						
						if(seldisking==0){
							backupViewPage(usepageno);
							DrawColor_Gray1(0,78,LCD_width,LCD_height-78);				//ͼ�α��(0,78,LCD_width,LCD_height-78);				//ͼ�α��
							seldisking = 1;
							disupdata9 = 1;
						}else{
							recoverViewPage(usepageno);
							seldisking =0;
						}							
						break;
					
					case KEY_FILE_ML:
					case KEY_FILE_MR:
						if(seldisking)	break;
					
						if(keyoff==KEY_FILE_ML){
							if(g_savePathTimes>0) g_savePathTimes--;
							else break;
						}else if(keyoff==KEY_FILE_MR){
							if(g_savePathTimes<tsavePathTimes)	g_savePathTimes++;
							else break;
						}
						
						if(getSavePath(g_savePath,g_path,g_savePathTimes,(U8*)&g_disk_memory)){
							getDiskFiles(g_path,(U16*)IREG2,GET_ALLFILE);
							SelFileNo = 0xffff;
							oSelFileNo = 0xffff; 
							contentUpdata1 =1;
							disupdata4 =1;
							disupdata1 = 1;
							g_btndisable = 0xdf8;	//����4/5/6/7/8/9/11/12����
							disupdata0 = 1;
							if(g_clipboard){
								g_btndisable&= ~(1<<7);
							}
							disupdata7 =1;
							fileStep = getPathStep(g_path);
							disFilebase =0;
							disupdata13 = 1;							
						}						
						disupdata14 = 1;

						break;
												
					case KEY_FILE_UDIR:
						if(seldisking)	break;
						if(fileStep){
							setPathToLast(g_path);
							getDiskFiles(g_path,(U16*)IREG2,GET_ALLFILE);
							SelFileNo = 0xffff;
							oSelFileNo = 0xffff; 
							contentUpdata1 =1;
							disupdata4 =1;
							disupdata1 = 1;
							g_btndisable = 0xdf8;	//����4/5/6/7/8/9/11/12����
							disupdata0 = 1;
							if(g_clipboard){
								g_btndisable&= ~(1<<7);
							}
							disupdata7 =1;
							fileStep = getPathStep(g_path);
							disFilebase =0;
							disupdata13 = 1;
							if(g_savePathTimes){
								g_savePathTimes--;
								disupdata14 = 1;
							}
						}						
						break;
						
					case KEY_FILE_FUP:
						if(disFilebase>0){
							if(g_disFileMethod==1){	//ͼ��
								disFilebase -=pagmax;
								if(disFilebase<0)	disFilebase =0;
							}else{	//�б�
								disFilebase--;
							}
							
							disupdata2 =1;		
						}
						break;
					
					case KEY_FILE_FDN:					
						if((disFilebase<totalfiles-pagmax)&&(totalfiles>pagmax)){	
							if(g_disFileMethod==0){ //�б�
								disFilebase++;	
							}else{		//ͼ��
								disFilebase +=pagmax;
							}							
							disupdata2 =1;		
						}												
						break;	

					case KEY_FILE_SLIDER:
						//disFilebase = keyypct/ScrollBargetDiv();
						
						if(g_disFileMethod==0){ //�б�
							disFilebase = keyypct*(ScrollBarGetMaxVaule()-ScrollBarGetCirVaule())/keyd[KEY_FILE_SLIDER].ysize;
							if(keyypct>(disFilebase+1)*ScrollBargetDiv()) disFilebase++;
							if((disFilebase>totalfiles-pagmax) || (keyypct ==keyd[KEY_FILE_SLIDER].ysize)){								
								disFilebase = totalfiles>pagmax?totalfiles-pagmax:0;
							}	
						}else{ //ͼ��
							disFilebase = keyypct*ScrollBargetIYsize()/ScrollBargetDiv()/keyd[KEY_FILE_SLIDER].ysize;
							disFilebase *=pagmax;
							if(disFilebase>totalfiles) disFilebase -= pagmax;
						}
						if(disFilebase != odisFilebase){
							disupdata2 =1;
						}
						break;
						
					case KEY_FILE_LIST:		//�б�ģʽ
						if(g_disFileMethod !=0){
							g_disFileMethod = 0;
							disupdata2 =1;	
							disupdata3 =1;
							disupdata7 =1;
							//pagmax = 9;
							DrawFillRect(4,124,538,414,Color_Silver);
						}
						break;

					case KEY_FILE_ICON:		//ͼ��ģʽ
						if(g_disFileMethod!=1){
							g_disFileMethod = 1;
							disupdata2 =1;	
							disupdata3 =1;
							disupdata7 =1;
							BmpFillRect(4,124,538,414);
						}
						break;
					
					case KEY_FILE_ADDFOLDER:	//����ļ���
						g_para0 = 1;
						pnoindex = 2;pno = MENU_FTKEYBOARD;poldno[pnoindex++] = MENU_FTFILES;
						break;
					case KEY_FILE_EDITNAME: //������
						fileType = getFileTypeByIREG((U16*)IREG2,SelFileNo);
						setBufDat8((U8*)g_patname,64,0);
						namelen =0;
						if(g_disk_memory == MEM_IN){
							if(fileType!=FILE_SEW){
								tname = (U8*)(&IREG2[(SelFileNo-MINFILENO)*FileIndexSize+FileIndexNamepox]);
								
								while((*(tname+namelen)!=0)&&(namelen<63)){
									g_patname[namelen] = *(tname+namelen);
									namelen++;
								}						
							}

							g_para0 = 0;
							if(fileType==FILE_MITE || fileType==FILE_MITG ){	//�����ʽ,��׺Ϊ�������
								if(namelen>60) break;	//�ļ���̫��
								g_patname[namelen++] = '.';
								g_patname[namelen++] = SelFileNo/100+'0';
								g_patname[namelen++] = SelFileNo/10%10+'0';
								g_patname[namelen++] = SelFileNo%10+'0';
							}else{
								g_patname[namelen++] = 'I';
								g_patname[namelen++] = 'S';
								g_patname[namelen++] = 'M';
								g_patname[namelen++] = 'S';
								g_patname[namelen++] = '0';
								g_patname[namelen++] = SelFileNo/100+'0';
								g_patname[namelen++] = SelFileNo/10%10+'0';
								g_patname[namelen++] = SelFileNo%10+'0';								
								g_patname[namelen++] = '.';
	
								g_patname[namelen++] = 'S';
								g_patname[namelen++] = 'E';
								g_patname[namelen++] = 'W';									
							}							
						}else{
							tname = (U8*)(&IREG2[(SelFileNo-MINFILENO)*FileIndexSize+FileIndexNamepox]);
							
							while((*(tname+namelen)!=0)&&(namelen<63)){
								g_patname[namelen] = *(tname+namelen);
								namelen++;
							}	
							if(fileType==FILE_FOLDER){
								g_para0 = 2;
							}else{
								g_para0 = 3;
							}												
						}										
						pnoindex = 2;pno = MENU_FTKEYBOARD;poldno[pnoindex++] = MENU_FTFILES;
						break;
					
					case KEY_FILE_DEL:	//ɾ��					
						if(g_checkMode){	
							for(i=0;i<MAXFILENO;i++){
								g_editfiles[i] = g_selfiles[i];
							}
						}else{
							setBufDat16((U16*)g_editfiles,MAXFILENO,0xffff);
							g_editfiles[dataadd[SelNo]/FileIndexSize] =  SelFileNo;
						}
						pnoindex = 2;hleppara  =170;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;
						break;	
					case KEY_FILE_COPY:	//����
					case KEY_FILE_CUT:	//����
						g_fromPath[0] =0;
						g_from_memory = g_disk_memory;
						if(g_checkMode){
							for(i=0;i<MAXFILENO;i++){
								g_editfiles[i] = g_selfiles[i];
							}												
						}else{
							setBufDat16((U16*)g_editfiles,MAXFILENO,0xffff);
							g_editfiles[dataadd[SelNo]/FileIndexSize] =  SelFileNo;						
						}
						copyPath(g_path,g_fromPath);
						g_btndisable = setBit16(g_btndisable,(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<10)|(1<<11));//����4/5/6/7/11/12
						g_btndisable = clrBit16(g_btndisable,(1<<7));//ʹ��8
						disupdata0 = 1;
						g_checkMode =0;
						if(g_disFileMethod==0){//�б�ģʽ
							disupdata7 =1;	
							disupdata4 = 1;
							SelFileNo = 0xffff;
							oSelFileNo = 0xffff;
						}
						setBufDat16((U16*)g_selfiles,MAXFILENO,0xffff);
						selnums = 0;
						g_selALL = 0;						
						disupdata6 = 1;
						disupdata10 = 1;
						if(keyoff==KEY_FILE_COPY){
							g_clipboard = 1;
						}else{
							g_clipboard = 2;
						}
						
						break;
					case KEY_FILE_PASTE:	//ճ��
						
						if(g_clipboard==1){	//����
							g_to_memory = g_disk_memory;							
							pnoindex = 2;hleppara = 173;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;					
						}else{			//����
							g_to_memory = g_disk_memory;
//							#if Debug
//							printf("g_to_memory = %d,  g_from_memory = %d ,g_fromPath = %s  ,g_path = %s ,strEqual = %d",g_to_memory,g_from_memory,g_fromPath,g_path,strEqual(g_fromPath,g_path));
//							#endif							
							if(g_to_memory==g_from_memory&&(g_from_memory==MEM_IN  || strEqual(g_fromPath,g_path))){
								pnoindex = 2;hleppara = 172;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;											
							}else{
								pnoindex = 2;hleppara = 174;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;					
							}
						}
				
						break;
					
					case KEY_FILE_CHECK:	//��ѡ/��ѡ
						if(g_checkMode){
							g_checkMode =0;
						}else {
							g_checkMode = 1;
						}
						disupdata7 =1;	
	
						setBufDat16((U16*)g_selfiles,MAXFILENO,0xffff);
						selnums = 0;
						g_selALL = 0;	
						
						g_btndisable = setBit16(g_btndisable,(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<10)|(1<<11));//����4/5/6/7/11/12
						SelFileNo = 0xffff;
						oSelFileNo = 0xffff;
						disupdata0 = 1;
						disupdata4 = 1;						
						disupdata6 = 1;
						disupdata10 = 1;
						break;

					case KEY_FILE_IMPORT:	//�����ļ�  
						if(g_disk_memory==MEM_IN)	break;
						g_from_memory = g_disk_memory;	
						copyPath(g_path,g_fromPath);
						g_to_memory = MEM_IN;
						if(g_checkMode){
							for(i=0;i<MAXFILENO;i++){
								g_editfiles[i] = g_selfiles[i];
							}												
						}else{
							setBufDat16((U16*)g_editfiles,MAXFILENO,0xffff);
							g_editfiles[dataadd[SelNo]/FileIndexSize] =  SelFileNo;						
						}
						pnoindex = 2;hleppara = 179;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;			
					break;
					
					case KEY_FILE_EXPORT:	//�����ļ�   g_from_memory
						diskstatus = getDiskStatus();							
						if(diskstatus==0){	
							pnoindex = 2;hleppara = 95;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;	break;
						}
						g_from_memory = g_disk_memory;		
						g_to_memory = MEM_USB;
						pnoindex = 2;hleppara = 178;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTFILES;				
										
						if(g_checkMode){
							for(i=0;i<MAXFILENO;i++){
								g_editfiles[i] = g_selfiles[i];
							}												
						}else{
							setBufDat16((U16*)g_editfiles,MAXFILENO,0xffff);
							g_editfiles[dataadd[SelNo]/FileIndexSize] =  SelFileNo;						
						}
						break;
						
					case KEY_FILE_FSORT:
						if(Sort) Sort =0;
						else Sort =1;
						disupdata5 =1;
						disupdata7 =1;	
						break;	
					
					case KEY_FILE_SALL:
						if(g_selALL){
							g_selALL =0;
							setBufDat16((U16*)g_selfiles,MAXFILENO,0xffff);
							g_btndisable = setBit16(g_btndisable,(1<<4)|(1<<5)|(1<<6)|(1<<10)|(1<<11));//����5/6/7/11/12
						}else{							
							g_selALL =1;
							for(i=0;i<MAXIREG2;i+=FileIndexSize){
								if((((IREG2[i+1]<<16)|IREG2[i])>0)&&(((IREG2[i+1]<<16)|IREG2[i])<=2*MAXSREG)){
									g_selfiles[i/FileIndexSize] = i/FileIndexSize+MINFILENO;;
								}
							}
							g_btndisable = clrBit16(g_btndisable,(1<<4)|(1<<5)|(1<<6));//ʹ��5/6/7
							if(g_disk_memory==MEM_IN){	
								g_btndisable = clrBit16(g_btndisable,(1<<11));//ʹ��12
							}else{
								g_btndisable = clrBit16(g_btndisable,(1<<10));//ʹ��11
							}
						} 
						disupdata0 = 1;
						disupdata6 =1;
						disupdata11 =1;
						disupdata12 =1;
						break;	


					
					case KEY_FILE_INMEM:	
					case KEY_FILE_USB:
						if(keyoff==KEY_FILE_INMEM){
							g_disk_memory = MEM_IN;
						}else{
							g_disk_memory = MEM_USB;
						}
						
						recoverViewPage(usepageno);
						DrawButtonOff2(keyd[KEY_FILE_MEM],Color_White1);
						seldisking =0;
						if(g_disk_memory!=odisk_memory){
							if(g_selALL)	g_selALL = 0;
							if(g_checkMode){g_checkMode =0;disupdata6 =1;}
							SelFileNo = 0xffff;
							oSelFileNo = 0xffff;
							g_path[0] = 0;
							disupdata1 = 1;
							disupdata4 =1;
							disupdata7 =1;
							if(g_disk_memory == MEM_IN || g_disk_memory == MEM_DSP ){
								g_btndisable = 0xdfc; //����3/4/5/6/7/8/9/11/12����
							}else{
								g_btndisable = 0xdf8;	//����4/5/6/7/8/9/11/12����
							}
							disupdata0 = 1;
							if(g_clipboard){
								g_btndisable&= ~(1<<7);
							}
							g_savePathTimes = 0;
							tsavePathTimes = 0;
							savePath(g_savePath,g_path,g_savePathTimes,g_disk_memory);	
							disupdata14 = 1;						
						}						
						break;

					
					case KEY_FILE_FORMAT:
						recoverViewPage(usepageno);
						DrawButtonOff2(keyd[KEY_FILE_MEM],Color_White1);
						seldisking =0;

						pnoindex = 2;poldno[pnoindex++] = MENU_FTFILES;pno = MENU_RWHELP;hleppara  = (g_disk_memory==MEM_IN?24:82) ;
						break;
					
					default:break;
				}			
			}
			keyoff = 0;
		}
		if((keyon)&&(keydata==0)){
			if(keyon==KEY_FILE_SCRNSVR);
			else{
				if(keyon<KEY_FILE_SLIDER){
					if((keyon>=KEY_FILE_MEM)&&(keyon<=KEY_FILE_EXPORT)){
						BmpFillRect(TITLE_XPOS_BOR,4,270-TITLE_XPOS_BOR,24);
						DrawButtonOff2(keyd[keyon],Color_White1);
					}else if((keyon>=KEY_FILE_FUP)&&(keyon<=KEY_FILE_FDN)){
						DrawButtonOff2(keyd[keyon],mCOLOR?Color_White1:Color_Gray);
					}else if(keyon==KEY_FILE_BACK){
						DrawButtonOff2(keyd[keyon],Color_White1);
					}else{
						DrawButtonOff2(keyd[keyon],mCOLOR?Color_White1:Color_Gray);
					}					
				}else if((keyon>=KEY_FILE_INMEM) &&(keyon<=KEY_FILE_FORMAT)&&(seldisking)){
					FillRectColorChang(keyd2[keyon-KEY_FILE_INMEM].xpis,keyd2[keyon-KEY_FILE_INMEM].ypis,keyd2[keyon-KEY_FILE_INMEM].xsize,keyd2[keyon-KEY_FILE_INMEM].ysize,Color_LBlue,Color_White);
				}

				BmpFillRect(TITLE_XPOS_MIT,4,LCD_width-TITLE_XPOS_MIT,24);
				Draw24x24Str(TITLE_XPOS_MIT,4,MEM_TITLE_ADD,SheetPage,0,Color_White);			
				
			}
			keyon = 0;
			beeponf = 0;
		}

			
		if((odisk_memory!=g_disk_memory) && (seldisking==0)){
			odisk_memory =g_disk_memory;
			DrawFillRect(keyd[KEY_FILE_MEM].xpis+16,keyd[KEY_FILE_MEM].ypis+4,32,32,mCOLOR?Color_Silver:Color_White1);
			DrawFPBMPBTN(keyd[KEY_FILE_MEM].xpis+16,keyd[KEY_FILE_MEM].ypis+4,32,32,g_disk_memory==MEM_IN?617:/*(g_disk_memory==MEM_DSP?1294:*/g_disk_memory+715, 0xffff);
			
			fileStep = getPathStep(g_path);
			disFilebase =0;
			disupdata13 = 1;			
			contentUpdata1 =1;
			if(g_disFileMethod==0){				
				for(i=0;i<pagmax;i++)DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_White);									
			}else{
				for(i=0;i<pagmax;i++)DrawFillRect(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_White);								
			}
			disupdata7 =1;
			DrawFillRect(4,538,592,120,Color_White);			
			DrawRect(410,546,170,104,Color_Black);
			
			conflag = 1;
			//�����ļ�����
			if(g_disk_memory==MEM_IN){	//�ڴ�
				f = ReadFileIndex(C_RDMEMPATLIST,MENU_FTFILES);
				if(f==0){
					pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;
				}
			}else if(g_disk_memory==MEM_USB){	//USB
				if(ExtMemory_Status(USBTF)!=0){
					pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;
					f = ReadFileIndex(C_RDMEMPATLIST,MENU_FTFILES);
				}else{
					Select_ExtMemory(USBTF);
					getDiskFiles(g_path,(U16*)IREG2,GET_ALLFILE);			
				}			
			}

		}
		
		if(contentUpdata1){
			contentUpdata1 = 0;
			totalfiles = getFileNumsByIREG((U16*)IREG2,GET_ALLFILE);
			disupdata2 =1;
		}
		
		if(disupdata0){
			disupdata0 = 0;
//		#if Debug
//			printf("disupdata0 \r\n");
//		#endif
			for(i=KEY_FILE_LIST;i<=KEY_FILE_EXPORT;i++){
				if(i==KEY_FILE_LIST || i ==KEY_FILE_ICON){
					f = KEY_FILE_LIST+g_disFileMethod;
					DrawButton2(keyd[i],Color_White1,(i==f)?keybmpind[i]+1:keybmpind[i],1);
				}else{
					if(g_btndisable&(1<<(i-KEY_FILE_LIST))){
						DrawButton2(keyd[i],Color_Gray,keybmpind[i]+1,1);	
					}else{					
						DrawButton2(keyd[i],Color_White1,keybmpind[i],1);
					}				
				}
			}
			//DelayMs(5000);
			
		}
		//��ʾ�ļ�·��
		if(disupdata1){	
			disupdata1 = 0;
			showFilePath(72,37,g_disk_memory,g_path,0);
		}
		
		if(disupdata5){
			disupdata5 = 0;
			DrawFillRect(508,87,24,24,Color_SkyBlue);	
			if(Sort){		//����
				DrawFPBMPBTN(508,87,24,24,719, 0xffff);
			}else{	//����
				DrawFPBMPBTN(508,87,24,24,720, 0xffff);
			}
			disupdata2 =1;
		}
		
		if(disupdata6){
//		#if Debug
//			printf("disupdata6 \r\n");
//		#endif
			disupdata6 = 0;
			if(g_checkMode==0){
				DrawFillRect(keyd[KEY_FILE_SALL].xpis+9,keyd[KEY_FILE_SALL].ypis+9,22,22,Color_Silver);
			}else{
				DrawFillRect(keyd[KEY_FILE_SALL].xpis+9,keyd[KEY_FILE_SALL].ypis+9,22,22,RGB(250,250,250));	
				if(g_selALL){
					DrawFPBMPBTN(keyd[KEY_FILE_SALL].xpis+9,keyd[KEY_FILE_SALL].ypis+9,22,22,596, 0xffff);
				}		
			}
			//DelayMs(5000);
//			DrawFillRect(keyd[KEY_FILE_SALL].xpis,keyd[KEY_FILE_SALL].ypis,keyd[KEY_FILE_SALL].xsize,keyd[KEY_FILE_SALL].ysize,Color_Gray);	
		}
		if(disupdata2){
			disupdata2 = 0;
//		#if Debug
//			printf("disupdata2 totalfiles = %d  pagmax = %d \r\n",totalfiles,pagmax);
//		#endif					
			if(g_disFileMethod==0){	//�б�ģʽ	
				ScrollBarSetMaxVaule(totalfiles);					
				ScrollBarSetCirVaule(pagmax); 				
				if(totalfiles<=pagmax){
					disFilebase = 0;										
				}else{										
					if(disFilebase>(totalfiles-pagmax)){
						disFilebase = totalfiles-pagmax;					
					}					
				}	
				disupdata7 = 1;
				ScrollBarSetVaule(pagmax+disFilebase);
				ScrollBarUpdateDiv();
				ScrollBarOnDraw();				
			}else{	//ͼ��ģʽ
				ScrollBarSetMaxVaule(totalfiles/pagmax);					
				ScrollBarSetCirVaule(0);
				if(totalfiles<=pagmax){	
					disFilebase = 0;				
				}else{
					//if(disFilebase%pagmax) disFilebase /=pagmax;	
				}
			//	printf("disFilebase = %d  odisFilebase = %d \r\n",disFilebase,odisFilebase);
				for(j=0;j<pagmax;j++) dataadd[j]  = 0xffffffff;	
				disupdata7 = 1;
				ScrollBarSetVaule(disFilebase/pagmax);
				ScrollBarUpdateDiv();
				ScrollBarOnDraw();					
			}

			odisFilebase = disFilebase;
//			oslidivlen = slidivlen;
			
			wdt();
			
			if(Sort==0){	//����
				for(j=0,i=0;(i<MAXIREG2)&&(j<disFilebase);i+=FileIndexSize){
					if(((IREG2[i+1]<<16)|IREG2[i])!=0xffffffff){j++;}
				}
				
				for(j=0;(i<MAXIREG2)&&(j<(totalfiles<pagmax?totalfiles:pagmax));i+=FileIndexSize){
					if(((IREG2[i+1]<<16)|IREG2[i])!=0xffffffff){dataadd[j] = i;j++;}			
				}
			}else{	//����
				for(j=0,i=(MAXFILENO-1)*FileIndexSize;(i>0)&&(j<disFilebase);i-=FileIndexSize){
					if(((IREG2[i+1]<<16)|IREG2[i])!=0xffffffff){j++;}
				}	
				ii = (S32)i;				
				for(j=0;(ii>=0)&&(j<(totalfiles<pagmax?totalfiles:pagmax));ii-=FileIndexSize){
					if(((IREG2[ii+1]<<16)|IREG2[ii])!=0xffffffff){
					dataadd[j] = ii;j++;
//					#if Debug
//						printf("i= %d no = %d j = %d totalfiles = %d pagmax = %d\r\n",ii,ii/FileIndexSize+MINFILENO,j,totalfiles,pagmax);
//					#endif
					}			
				}			
			}
			
			for(;j<pagmax;j++) dataadd[j]  = 0xffffffff;

//			#if Debug
//			printf("totalfiles = %d slidivlen = %d exFiles = %d \r\n",totalfiles,slidivlen,disFilebase);
//			for(j=0;j<pagmax;j++) printf("dataadd[%d] = 0x%0x \r\n",j,dataadd[j]);	
//			
//			#endif		
		}
		
		if(contentUpdata2){
			contentUpdata2 =0;
			SelFileNo =  dataadd[SelNo]/FileIndexSize+MINFILENO;
			conflag = 1;
			if(g_disk_memory==MEM_IN){	//�ڴ�				
				if(Write_Cmd(C_READMEMORYPAT,SelFileNo,0)==0){
					pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;conflag =0;
				}
				if(conflag){
					if(Read_Status()==0){
						pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;conflag =0;
					}else if(DREG[D_CMDSTATUS]!=100){
						pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;conflag =0;
					}else{
						i = Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&HREG[0]));				// ����ǰ�����ļ���Ϣ
						if(i==0){
							pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;conflag =0;
						}
						if(conflag){
							if(SelFileNo!=HREG[0]){
								pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;conflag =0;
							}						
						}
						if(conflag){
							i = (SelFileNo-MINFILENO)*FileIndexSize;
							i = (((U32)(IREG2[i+1])<<16)+IREG2[i])/2;
							j = Read_D(D_Address[PTDATA],i,(U16 *)(&SREG[0]));							// ����ǰ�����ļ�	
							if(j==0){
								pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;conflag =0;
							}else{				
								for(;i<MAXSREG;i++)	SREG[i] = 0;
								g_init_flag = 1 ;
								if(HREG[8]==1) fileType = FILE_MITE;
								else if(HREG[8]==2)fileType = FILE_SEW;
								else fileType = FILE_EMB;
								
								disupdata4 =1;	//��ʾ����	
							}								
						}
					}				
				}
						
			}else{	//�ⲿ�洢��
				HREG[0] = SelFileNo;
				Select_ExtMemory(USBTF);
				if(readDiskPatFileByName(g_path,(char*)(&IREG2[dataadd[SelNo]+FileIndexNamepox]),(U16*)SREG)==0){				
					g_init_flag = 1 ;
					if(HREG[8]==1) fileType = FILE_MITE;
					else if(HREG[8]==2)fileType = FILE_SEW;
					else fileType = FILE_EMB;
					
					disupdata4 =1;	//��ʾ����	
				}else{
					pnoindex=2;pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;conflag =0;	//���ݳ���
				}	

				//readDiskPatFileByName(g_path,(U8*)(&IREG[dataadd[i]+FileIndexNamepox]));
		
			}		
			//#if Debug
			//printf("contentUpdata2 SelFileNo = %d  oSelFileNo = %d SelNo = %d  disFilebase = %d  totalfiles = %d \r\n",SelFileNo,oSelFileNo,SelNo,disFilebase,totalfiles);
			//#endif

		}

		
		if(disupdata7){		//��ʾ�ļ���
			disupdata7 = 0;
			
//			#if Debug
//			printf("disupdata7  SelFileNo = %d ****\r\n",SelFileNo);
//			#endif
			SelNo =0;oSelNo =0;
			
			if(g_disFileMethod==0){	//�б�ģʽ
				for(i=0;i<(totalfiles<pagmax?totalfiles:pagmax);i++){
					wdt();
					if(dataadd[i]<MAXIREG2){					
						FileNO = dataadd[i]/FileIndexSize+MINFILENO;
						if(FileNO==SelFileNo){oSelNo = i;SelNo =i;} 
						DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,FileNO==SelFileNo?Color_LBlue:Color_White);					
						fileType = getFileTypeByIREG((U16*)IREG2,FileNO);
//						#if Debug
//						printf("fileType = %d \r\n",fileType);
//						#endif
						mymemset(tdisname,0,sizeof(tdisname));
						if(g_disk_memory==MEM_IN){	//�ڴ��ļ�
							if(fileType==FILE_MITE || fileType==FILE_MITG ){	//�����ʽ
								//DrawFFilename(12,392,(char *)(&g_patd.fname),64,Color_Black,30,2,NULL);		// ��ʾ�ļ���
								tname = (U8*)(&IREG2[dataadd[i]+FileIndexNamepox]);
								namelen =0;

								while((*(tname+namelen)!=0)/*&&(*(tname+namelen)!=' ') */&&(namelen<38)){
									tdisname[namelen] = *(tname+namelen);
									namelen++;
								}						
								if(namelen<34){
									tdisname[namelen++] = '.';
									tdisname[namelen++] = FileNO/100+0x30;
									tdisname[namelen++] = FileNO/10%10+0x30;
									tdisname[namelen++] = FileNO%10+0x30;							
								}																	
							}else if(fileType==FILE_SEW){	//�ֵܸ�ʽ
								for(namelen=0;namelen<sizeof(BorName);namelen++){
									tdisname[namelen] = BorName[namelen];
								}
								tdisname[5] = FileNO/100+0x30;
								tdisname[6] = FileNO/10%10+0x30;
								tdisname[7] = FileNO%10+0x30;						
							}
						}else{	//�ⲿ�洢�豸
							tname = (U8*)(&IREG2[dataadd[i]+FileIndexNamepox]);
							namelen =0;
							while((*(tname+namelen)!=0)/*&&(*(tname+namelen)!=' ') */&&(namelen<34)){
								tdisname[namelen] = *(tname+namelen);
								namelen++;
							}	
							if(fileType==FILE_FOLDER){	//�ļ���
								DrawFPBMPBTN(keyd[i+KEY_FILE_LIST1].xpis+40,keyd[i+KEY_FILE_LIST1].ypis+10,24,20,923, 0xffff);		
							}
						}
						if(g_checkMode){
							DrawRect(keyd[i+KEY_FILE_LIST1].xpis+8,keyd[i+KEY_FILE_LIST1].ypis+8,24,24,Color_Black);
							DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis+9,keyd[i+KEY_FILE_LIST1].ypis+9,22,22,RGB(250,250,250));							
							if(isContain_U16((U16*)g_selfiles,MAXFILENO,FileNO)){
								DrawFPBMPBTN(keyd[i+KEY_FILE_LIST1].xpis+9,keyd[i+KEY_FILE_LIST1].ypis+9,22,22,596, 0xffff);	
								FillRectColorChang(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_White,Color_LBlue);								
							}else{					
								FillRectColorChang(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_LBlue,Color_White);	
							}						
						}
						Draw24x24StrSinLineLimt(keyd[i+KEY_FILE_LIST1].xpis+40 +(fileType==FILE_FOLDER?36:0),keyd[i+KEY_FILE_LIST1].ypis+10,(U16*)tdisname,sheet,0,48-(fileType==FILE_FOLDER?4:0),Color_Black);
						//Draw24X24FStr(),(char *)tdisname,Color_Black);				
					}else{
						DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_White);	
					}
				}
				for(;i<pagmax;i++){
					DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_White);	
				}	
			}else{	//ͼ��ģʽ
				for(i=0;i<(totalfiles<pagmax?totalfiles:pagmax);i++){
					wdt();
					mymemset(tdisname,0,sizeof(tdisname));
					if(dataadd[i]<MAXIREG2){					
						FileNO = dataadd[i]/FileIndexSize+MINFILENO;	
						if(FileNO==SelFileNo){oSelNo = i;SelNo =i;} 
						DrawFillRect(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,FileNO==SelFileNo?Color_LBlue:Color_White);	
						fileType = getFileTypeByIREG((U16*)IREG2,FileNO);
						
						if(g_disk_memory==MEM_IN){	//�ڴ��ļ�
							if(fileType==FILE_MITE || fileType==FILE_MITG ){	//�����ʽ
								tname = (U8*)(&IREG2[dataadd[i]+FileIndexNamepox]);
								namelen =0;

								while((*(tname+namelen)!=0)&&(*(tname+namelen)!=' ') &&(namelen<14)){
									tdisname[namelen] = *(tname+namelen);
									namelen++;
								}	
								tdisname[namelen++] = '.';
								tdisname[namelen++] = FileNO/100+0x30;
								tdisname[namelen++] = FileNO/10%10+0x30;
								tdisname[namelen++] = FileNO%10+0x30;	
							}else if(fileType==FILE_SEW){
								for(namelen=0;namelen<sizeof(BorName);namelen++){
									tdisname[namelen] = BorName[namelen];
								}
								tdisname[5] = FileNO/100+0x30;
								tdisname[6] = FileNO/10%10+0x30;
								tdisname[7] = FileNO%10+0x30;						
							}
						}else{
							tname = (U8*)(&IREG2[dataadd[i]+FileIndexNamepox]);
							namelen =0;
							while((*(tname+namelen)!=0)&&(*(tname+namelen)!=' ') &&(namelen<18)){
								tdisname[namelen] = *(tname+namelen);
								namelen++;
							}															
						}
						
						if(g_checkMode){
							DrawRect(keyd1[i].xpis+4,keyd1[i].ypis+108,24,24,Color_Black);
							DrawFillRect(keyd1[i].xpis+5,keyd1[i].ypis+109,22,22,RGB(250,250,250));							
							if(isContain_U16((U16*)g_selfiles,MAXFILENO,FileNO)){
								DrawFPBMPBTN(keyd1[i].xpis+5,keyd1[i].ypis+109,22,22,596, 0xffff);	
								FillRectColorChang(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_White,Color_LBlue);								
							}else{					
								FillRectColorChang(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_LBlue,Color_White);	
							}	
						}					
						//Draw24X24FStr(keyd1[i].xpis+3,keyd1[i].ypis+88,(char *)tdisname,Color_Black);		//��ʾ�ļ���
						DrawCenterStrArea1(keyd1[i].xpis+30,keyd1[i].ypis+106,keyd1[i].xsize-34,28,0,0,(char *)tdisname,Color_Black);		
						
						//��ȡ�������� 
						if(g_disk_memory==MEM_IN){	//�ڴ�
							if(fileType==FILE_MITE || fileType==FILE_MITG ||fileType==FILE_SEW /*|| fileType==FILE_EMB*/){
								if((FileNO>=MINFILENO)&&(FileNO<=MAXFILENO)){
									if(g_disk_memory==MEM_IN){
										if(Write_Cmd(C_READMEMORYPAT,FileNO,0)==0){
											pnoindex =2;pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
										}	
										if(Read_Status()==0){
											pnoindex =2;pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
										}else if(DREG[D_CMDSTATUS]!=100){
											pnoindex =2;pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
										}
										f = Read_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&HREG[0]));				// ����ǰ�����ļ���Ϣ
										
										if(f==0){
											pnoindex =2;pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
										}
										if(FileNO!=HREG[0]){
											pnoindex =2;pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
										}
										ii = (FileNO-MINFILENO)*FileIndexSize;
										ii = (((U32)(IREG2[ii+1])<<16)+IREG2[ii])/2;

										j = Read_D(D_Address[PTDATA],ii,(U16 *)(&SREG[0]));							// ����ǰ�����ļ�							
										if(j==0){
											pnoindex =2;pno = MENU_RWHELP;hleppara = 15;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
										}else{				
											for(;ii<MAXSREG;ii++)	SREG[ii] = 0;
											g_init_flag = 1 ;
										}								
									}
								}
								AnaPattern((S_2HD?1:0),HREG[8],((HREG[5]<<16)|HREG[4]));							
							}						
						}else{
							if(fileType==FILE_MITE || fileType==FILE_MITG ||fileType==FILE_SEW /*|| fileType==FILE_EMB*/){
								Select_ExtMemory(USBTF);	
								if(readDiskPatFileByName(g_path,(char*)(&IREG2[dataadd[i]+FileIndexNamepox]),(U16*)SREG)==0){
									g_init_flag = 1 ;
								}else{
									pnoindex =2;pno = MENU_RWHELP;hleppara = 16;poldno[pnoindex++] = MENU_FTFILES;continue;//�����ȡ��һ������
								}
								AnaPattern((S_2HD?1:0),HREG[8],((HREG[5]<<16)|HREG[4]));	
							}							
						}


						//��ʾͼ��
						DrawRect(keyd1[i].xpis+4,keyd1[i].ypis+6,170,100,Color_Gray);
						DrawFillRect(keyd1[i].xpis+5,keyd1[i].ypis+7,168,98,Color_White1);
						if(fileType==FILE_MITE || fileType==FILE_MITG || fileType==FILE_SEW /*|| fileType==FILE_EMB*/){
							Mit_Draw_Pattern(14, 0,keyd1[i].xpis+15,keyd1[i].ypis+12,HREG[8],((HREG[5]<<16)|HREG[4]),0,0,NULL);						
						}else{
							DrawFPBMPBTN(keyd1[i].xpis+57,keyd1[i].ypis+20,64,56,fileType==FILE_FOLDER?952:953, 0xffff);	
						}
					}else{
						DrawFillRect(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_White);	
					}
				}
				for(;i<pagmax;i++){
					DrawFillRect(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_White);	
				}				
			}
//			#if Debug
//			//DelayMs(5000);
//			printf("disupdata7 end ****\r\n");
//			#endif			
		}
		
		if(disupdata10){
			disupdata10 = 0;
//		#if Debug
//			printf("disupdata0 \r\n");
//		#endif
			if(g_disFileMethod==0){	//�б�ģʽ
				for(i=0;i<(totalfiles<pagmax?totalfiles:pagmax);i++){
					if(g_checkMode){
						DrawRect(keyd[i+KEY_FILE_LIST1].xpis+8,keyd[i+KEY_FILE_LIST1].ypis+8,24,24,Color_Black);	
						DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis+9,keyd[i+KEY_FILE_LIST1].ypis+9,22,22,RGB(250,250,250));			
					}else{
						DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis+8,keyd[i+KEY_FILE_LIST1].ypis+8,24,24,Color_White);
					}
					//DelayMs(2000);
				}
			}else{	//ͼ��ģʽ
				for(i=0;i<(totalfiles<pagmax?totalfiles:pagmax);i++){
					if(g_checkMode){
						DrawRect(keyd1[i].xpis+4,keyd1[i].ypis+108,24,24,Color_Black);	
						DrawFillRect(keyd1[i].xpis+5,keyd1[i].ypis+109,22,22,RGB(250,250,250));			
					}else{
						DrawFillRect(keyd1[i].xpis+4,keyd1[i].ypis+108,24,24,Color_White);
					}
				}			
			}
			//DelayMs(5000);
		}		
		if(disupdata3){
			disupdata3 =0;
			f = KEY_FILE_LIST+g_disFileMethod;
			for(i=KEY_FILE_LIST;i<=KEY_FILE_ICON;i++){									
				DrawButton2(keyd[i],Color_White1,(i==f)?keybmpind[i]+1:keybmpind[i],1);
			} 
		}
		
		if(SelFileNo!=oSelFileNo){	//ͼ��ģʽ��ѡ�����仯
			if(g_disFileMethod==0){	//�б�ģʽ
				FillRectColorChang(keyd[oSelNo+KEY_FILE_LIST1].xpis,keyd[oSelNo+KEY_FILE_LIST1].ypis,keyd[oSelNo+KEY_FILE_LIST1].xsize,keyd[oSelNo+KEY_FILE_LIST1].ysize,Color_LBlue,Color_White);	
				FillRectColorChang(keyd[SelNo+KEY_FILE_LIST1].xpis,keyd[SelNo+KEY_FILE_LIST1].ypis,keyd[SelNo+KEY_FILE_LIST1].xsize,keyd[SelNo+KEY_FILE_LIST1].ysize,Color_White,Color_LBlue);
			}else{		//ͼ��ģʽ
				FillRectColorChang(keyd1[oSelNo].xpis,keyd1[oSelNo].ypis,keyd1[oSelNo].xsize,keyd1[oSelNo].ysize,Color_LBlue,Color_White);	
				FillRectColorChang(keyd1[SelNo].xpis,keyd1[SelNo].ypis,keyd1[SelNo].xsize,keyd1[SelNo].ysize,Color_White,Color_LBlue);				
			} 
			oSelFileNo = SelFileNo;
		}
		if(disupdata4){		//�б�ģʽ�� ��ʾ����
			disupdata4 =0;
//			#if Debug
//			printf("disupdata4 ----");
//			#endif
			DrawFillRect(4,538,592,120,Color_White);			
			DrawRect(410,546,170,104,Color_Black);
			if(g_checkMode){
				selnums = getFiles((U16*)g_selfiles,MAXFILENO);	
				DrawMFU32(12, 546, selnums, getNumDigits(selnums),Color_Black);
				Draw24x24Str(14+getNumDigits(selnums)*13,546,MEM_TITLE_ADD+23*TitleLengthMax,SheetPage,23,Color_Black);		//x���ļ�ѡ��
				DrawFPBMPBTN(447,558,96,80,922, 0xffff);
			}else{
				if(SelFileNo ==0xffff){
					if(fileStep==0){						
						tmplen = Draw24x24Str(12,546,MEM_TITLE_ADD+(g_disk_memory+18)*TitleLengthMax,SheetPage,(g_disk_memory+18),Color_Black);			
					}else{
						mymemset(tdisname,0,sizeof(tdisname));
						//tmplen = Draw24X24FStr(12,546,getFolderName((char*)g_path),Color_Black);
						for(i=0;i<26;i++) tdisname[i] = g_path[i];
						tmplen = Draw24X24FStr(12,546,getFolderName((char*)tdisname),Color_Black);
						//tmplen = getFileNamaLen((U8*)getFolderName((char*)g_path));
					}
					Draw24X24FStr(20+tmplen*13,546,"/",Color_Black); 
					DrawMFU32ZR(36+tmplen*13,546,totalfiles,4,Color_Black);
					
					DrawFPBMPBTN(447,558,96,80,921, 0xffff);
				}else{
					if(g_disk_memory==MEM_IN){	//�ڴ�,�����
						if(fileType==FILE_SEW || fileType==FILE_MITE || fileType==FILE_MITG /*|| fileType==FILE_EMB */){
							AnaPattern((S_2HD?1:0),HREG[8],((HREG[5]<<16)|HREG[4]));										
							Draw24X24FStr(12,546,"Program NO.",Color_Black);
							DrawMFU32(155, 546, g_patd.fno, 3, Color_Black);
							DrawFFilename1(12,576,(char *)(&g_patd.fname),64,Color_Black,24,2,NULL);		// ��ʾ�ļ���
							DisRange(12,602,fileType==FILE_SEW?((g_patd.patmX-g_patd.patiX)*10/2):((g_patd.patmX-g_patd.patiX)*10),fileType==FILE_SEW?((g_patd.patmY-g_patd.patiY)*10/2):((g_patd.patmY-g_patd.patiY)*10));	//��ʾ��Χ
							DrawMFS32P(258,602, g_patd.feed_sum,5, 0,Color_Black);		//��ʾ������
							Draw24X24FStr(330,602,"sti",Color_Black);
							
							if(g_patd.pat_len<1024){
								DrawMFU32(12, 628, g_patd.pat_len, getNumDigits( g_patd.pat_len),Color_Black);		
								Draw24X24FStr(12+getNumDigits( g_patd.pat_len)*13+4,628,"B",Color_Black);
							}else{
								num1 =  g_patd.pat_len/1024;
								num2 = ( g_patd.pat_len*100/1024)%100;
	//							if(num2>99){	//����2λС��
	//								num2 /=10;
	//							}
								DrawMFU32(12, 628, num1, getNumDigits(num1),Color_Black);	
								Draw24X24FStr(12+getNumDigits(num1)*13,628,".",Color_Black);
								DrawMFU32(12+(getNumDigits(num1)+1)*13, 628, num2, getNumDigits(num2),Color_Black);				
								Draw24X24FStr(12+(getNumDigits(num1)+1+getNumDigits(num2))*13+4,628,"KB",Color_Black);
							}
							
							Mit_Draw_Pattern(13, 0,0,0,HREG[8],g_patd.pat_len,0,0,NULL);
						}
					}else{
						mymemset(tdisname,0,sizeof(tdisname));
						tname = (U8*)(&IREG2[dataadd[SelNo]+FileIndexNamepox]);
						namelen =0;

						while((*(tname+namelen)!=0)/*&&(*(tname+namelen)!=' ') */&&(namelen<64)){
							tdisname[namelen] = *(tname+namelen);
							namelen++;
						}
//						#if Debug
//						printf("tdisname = %s fileType = %d dataadd[SelNo] = %d fileType = %d \r\n",tdisname,fileType,dataadd[SelNo],fileType);
//						#endif
						Draw24x24StrSinLineLimt(12,546,(U16*)tdisname,sheet,0,24,Color_Black);	//��ʾ�ļ���
									
						if(fileType==FILE_SEW || fileType==FILE_MITE || fileType==FILE_MITG /*|| fileType==FILE_EMB*/){
							AnaPattern((S_2HD?1:0),HREG[8],((HREG[5]<<16)|HREG[4]));										
							DrawFFilename1(12,576,(char *)(&g_patd.fname),64,Color_Black,24,2,NULL);		// ��ʾ������
							DisRange(12,602,fileType==FILE_SEW?(g_patd.patmX-g_patd.patiX)*10/2:(g_patd.patmX-g_patd.patiX)*10,fileType==FILE_SEW?(g_patd.patmY-g_patd.patiY)*10/2:(g_patd.patmY-g_patd.patiY)*10);	//��ʾ��Χ
							DrawMFS32P(258,602, g_patd.feed_sum,5, 0,Color_Black);		//��ʾ������
							Draw24X24FStr(330,602,"sti",Color_Black);
													
							if(g_patd.pat_len<1024){
								DrawMFU32(12, 628, g_patd.pat_len, getNumDigits(g_patd.pat_len),Color_Black);		
								Draw24X24FStr(12+getNumDigits(g_patd.pat_len)*13+4,628,"B",Color_Black);
							}else{
								num1 = g_patd.pat_len/1024;
								num2 = (g_patd.pat_len*100/1024)%100;
	//							if(num2>99){	//����2λС��
	//								num2 /=10;
	//							}
								DrawMFU32(12, 628, num1, getNumDigits(num1),Color_Black);	
								Draw24X24FStr(12+getNumDigits(num1)*13,628,".",Color_Black);
								DrawMFU32(12+(getNumDigits(num1)+1)*13, 628, num2, getNumDigits(num2),Color_Black);				
								Draw24X24FStr(12+(getNumDigits(num1)+1+getNumDigits(num2))*13+4,628,"KB",Color_Black);
							}
							
							Mit_Draw_Pattern(13, 0,0,0,HREG[8],g_patd.pat_len,0,0,NULL);						
						}else if(fileType==FILE_FOLDER){	//�ļ���
							//DrawFFilename1(12,380,(char *)(&IREG[dataadd[SelNo]+FileIndexNamepox]),64,Color_Black,30,2,NULL);		// ��ʾ�ļ���				
							DrawFPBMPBTN(447,558,96,80,921, 0xffff);				
						}else {		//�����ļ�
							fileSize = (IREG2[dataadd[SelNo]+1]<<16)|IREG2[dataadd[SelNo]];
							if(fileSize<1024){
								DrawMFU32(12, 628, fileSize, getNumDigits(fileSize),Color_Black);		
								Draw24X24FStr(12+getNumDigits(fileSize)*13+4,628,"B",Color_Black);
							}else if(fileSize<1024*1024){
								num1 = fileSize/1024;
								num2 = (fileSize*100/1024)%100;
								DrawMFU32(12, 628, num1, getNumDigits(num1),Color_Black);	
								Draw24X24FStr(12+getNumDigits(num1)*13,628,".",Color_Black);
								DrawMFU32(12+(getNumDigits(num1)+1)*13, 628, num2, getNumDigits(num2),Color_Black);				
								Draw24X24FStr(12+(getNumDigits(num1)+1+getNumDigits(num2))*13+4,628,"KB",Color_Black);
							}else{
								num1 = fileSize/1024/1024;
								num2 = (fileSize*100/1024/1024)%100;
								DrawMFU32(12, 628, num1, getNumDigits(num1),Color_Black);	
								Draw24X24FStr(12+getNumDigits(num1)*13,628,".",Color_Black);
								DrawMFU32(12+(getNumDigits(num1)+1)*13, 628, num2, getNumDigits(num2),Color_Black);				
								Draw24X24FStr(12+(getNumDigits(num1)+1+getNumDigits(num2))*13+4,628,"MB",Color_Black);
							}
							//DrawFFilename1(12,380,(char *)(&IREG[dataadd[SelNo]+FileIndexNamepox]),64,Color_Black,30,2,NULL);		// ��ʾ�ļ���				
							DrawFPBMPBTN(447,558,96,80,922, 0xffff);				
						}					
					
					}											
					oSelFileNo = SelFileNo;												
				}			
			}


//			#if Debug
//			//DelayMs(5000);
//			printf("disupdata4 end \r\n" );
//			#endif			
		}
		
		if(disupdata9){
			disupdata9 = 0;
			BmpFillRect(4,76,262,144);
			
//			if(g_clipboard&&g_from_memory==MEM_IN) MemIn[1] =0; else MemIn[1] = 1;	//�ڴ治�ܸ��Ƶ������
//			if(g_clipboard&&g_from_memory==MEM_DSP) MemIn[0] =0; else MemIn[0] = 1;	//��������ܸ��Ƶ��ڴ�
			for(i=0;i<keynum2;i++){
				DrawFillRect(keyd2[i].xpis,keyd2[i].ypis,keyd2[i].xsize,keyd2[i].ysize,((i==g_disk_memory&&i==0)||(i==1&&g_disk_memory==MEM_USB))?Color_SkyBlue:MemIn[i]==1?Color_White:Color_DarkGray);	
				DrawFPBMPBTN(keyd2[i].xpis+4,keyd2[i].ypis+4,32,32,i==0?617:i+716, 0xffff);
				Draw24x24Str(keyd2[i].xpis+48,keyd2[i].ypis+12,MEM_TITLE_ADD+(18+i+(i>0?1:0))*TitleLengthMax,SheetPage,18+i+(i>0?1:0),((i==0&&i==g_disk_memory)||(i==1&&g_disk_memory==MEM_USB))?Color_White1:Color_Black);	
			}
		}
			
		if(disupdata11){
			disupdata11 = 0;
			selnums = getFiles((U16*)g_selfiles,MAXFILENO);	

			DrawFillRect(12,546,260,24,Color_White);
			DrawMFU32(12, 546, selnums, getNumDigits(selnums),Color_Black);
			Draw24x24Str(14+getNumDigits(selnums)*13,546,MEM_TITLE_ADD+23*TitleLengthMax,SheetPage,23,Color_Black);		//x���ļ�ѡ��

		}
		if(disupdata12){
			disupdata12 = 0;
//		#if Debug
//			printf("disupdata12  \r\n");
//		#endif
			if(g_disFileMethod==0){	//�б�ģʽ
				for(i=0;i<(totalfiles<pagmax?totalfiles:pagmax);i++){
					DrawFillRect(keyd[i+KEY_FILE_LIST1].xpis+9,keyd[i+KEY_FILE_LIST1].ypis+9,22,22,RGB(250,250,250));	
					if(g_selALL){
						DrawFPBMPBTN(keyd[i+KEY_FILE_LIST1].xpis+9,keyd[i+KEY_FILE_LIST1].ypis+9,22,22,596, 0xffff);	
						FillRectColorChang(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_White,Color_LBlue);								
					}else{
						FillRectColorChang(keyd[i+KEY_FILE_LIST1].xpis,keyd[i+KEY_FILE_LIST1].ypis,keyd[i+KEY_FILE_LIST1].xsize,keyd[i+KEY_FILE_LIST1].ysize,Color_LBlue,Color_White);	
					}
				}			
			}else{	//ͼ��ģʽ
				for(i=0;i<(totalfiles<pagmax?totalfiles:pagmax);i++){
					DrawFillRect(keyd1[i].xpis+5,keyd1[i].ypis+109,22,22,RGB(250,250,250));
					if(g_selALL){
						DrawFPBMPBTN(keyd1[i].xpis+5,keyd1[i].ypis+109,22,22,596, 0xffff);
						FillRectColorChang(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_White,Color_LBlue);
					}else{
						FillRectColorChang(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_LBlue,Color_White);
					}					
				}
			
			}

		}
		
		if(disupdata13){
			disupdata13 = 0;
//		#if Debug
//			printf("disupdata13 fileStep = %d \r\n",fileStep);
//		#endif
			if(fileStep==0){
				DrawColor_Gray1(keyd[KEY_FILE_UDIR].xpis,keyd[KEY_FILE_UDIR].ypis,keyd[KEY_FILE_UDIR].xsize,keyd[KEY_FILE_UDIR].ysize);	
			}else{
				DrawButton1(keyd[KEY_FILE_UDIR],Color_White1,keybmpind[KEY_FILE_UDIR]);			
			}
		}
		
		if(disupdata14){
			disupdata14 = 0;
			if(g_savePathTimes<tsavePathTimes){
				DrawButton1(keyd[KEY_FILE_MR],Color_White1,keybmpind[KEY_FILE_MR]);					
			}else{
				DrawColor_Gray1(keyd[KEY_FILE_MR].xpis,keyd[KEY_FILE_MR].ypis,keyd[KEY_FILE_MR].xsize,keyd[KEY_FILE_MR].ysize);			
			}	
			if(g_savePathTimes){
				DrawButton1(keyd[KEY_FILE_ML],Color_White1,keybmpind[KEY_FILE_ML]);		
			}else{
				DrawColor_Gray1(keyd[KEY_FILE_ML].xpis,keyd[KEY_FILE_ML].ypis,keyd[KEY_FILE_ML].xsize,keyd[KEY_FILE_ML].ysize);			
			}
		}
		if((Timer_dms>1000)){	//��ѯ�ⲿ�洢�豸
			Timer_dms = 0;	
			errtimes++;
			
			diskstatus = getDiskStatus();
			if(diskstatus&0x02){
				MemIn[1] =1;
			}else{
				MemIn[1] =0;
				if(g_disk_memory==MEM_USB){
					g_disk_memory=MEM_IN;disupdata4 =1;oSelFileNo=0xffff;SelFileNo=0xffff;
					g_btndisable = setBit16(g_btndisable,(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<10)|(1<<11));//����3/4/5/6/7/11/12
					if(g_from_memory==MEM_USB && g_clipboard){
						setBufDat16((U16*)g_editfiles,MAXFILENO,0xffff);
						g_btndisable = setBit16(g_btndisable,(1<<7));//����8
						g_clipboard = 0;
					}
					disupdata0 = 1;
					g_path[0] = 0;	
					g_savePathTimes = 0;	
					tsavePathTimes = 0;
					savePath(g_savePath,g_path,g_savePathTimes,g_disk_memory);	
					disupdata14 = 1;
					disupdata1 = 1;
				//	printf("---------- seldisking = %d \r\n",seldisking);
					if(seldisking){
						ReadFileIndex(C_RDMEMPATLIST,MENU_FTFILES);
						recoverViewPage(usepageno);
						pnoindex=2;pno = MENU_RWHELP;hleppara = 95;poldno[pnoindex++] = MENU_FTFILES;				
					}
				}
			}
			
			if((seldisking==1)&&((MemIn[1]!=oMemIn[1]))){
				disupdata9 =1;			
				oMemIn[1] = MemIn[1];
			}
		}							
		
		if(pno != MENU_FTFILES){g_SelFileNo = SelFileNo;plastno = MENU_FTFILES; break;}
		wdt();
	}
}



#define		KEY_KEYBOARD_SCRNSVR		1
#define		KEY_KEYBOARD_NO				2
#define		KEY_KEYBOARD_TEXT			3
#define		KEY_KEYBOARD_BACK			4
#define		KEY_KEYBOARD_ENTER			5

#define		KEY_KEYBOARD1_0				6
#define		KEY_KEYBOARD1_1				7
#define		KEY_KEYBOARD1_2				8
#define		KEY_KEYBOARD1_3				9
#define		KEY_KEYBOARD1_4				10
#define		KEY_KEYBOARD1_5				11
#define		KEY_KEYBOARD1_6				12
#define		KEY_KEYBOARD1_7				13
#define		KEY_KEYBOARD1_8				14
#define		KEY_KEYBOARD1_9				15
#define		KEY_KEYBOARD1_DEL			16
#define		KEY_KEYBOARD1_UP			17
#define		KEY_KEYBOARD1_DN			18

#define		KEY_KEYBOARD2_0				19
#define		KEY_KEYBOARD2_1				20
#define		KEY_KEYBOARD2_2				21
#define		KEY_KEYBOARD2_3				22
#define		KEY_KEYBOARD2_4				23
#define		KEY_KEYBOARD2_5				24
#define		KEY_KEYBOARD2_6				25
#define		KEY_KEYBOARD2_7				26
#define		KEY_KEYBOARD2_8				27
#define		KEY_KEYBOARD2_9				28
#define		KEY_KEYBOARD2_DEL			29
#define		KEY_KEYBOARD2_123			30
#define		KEY_KEYBOARD2_TOL			31
#define		KEY_KEYBOARD2_TOR			32
#define		KEY_KEYBOARD2_CHAT1			33
#define		KEY_KEYBOARD2_CHAT2			34
#define		KEY_KEYBOARD2_SPACE			35
#define		KEY_KEYBOARD2_SHIFT			36
#define		KEY_KEYBOARD2_A				37
#define		KEY_KEYBOARD2_B				38
#define		KEY_KEYBOARD2_C				39
#define		KEY_KEYBOARD2_D				40
#define		KEY_KEYBOARD2_E				41
#define		KEY_KEYBOARD2_F				42
#define		KEY_KEYBOARD2_G				43
#define		KEY_KEYBOARD2_H				44
#define		KEY_KEYBOARD2_I				45
#define		KEY_KEYBOARD2_J				46
#define		KEY_KEYBOARD2_K				47
#define		KEY_KEYBOARD2_L				48
#define		KEY_KEYBOARD2_M				49
#define		KEY_KEYBOARD2_N				50
#define		KEY_KEYBOARD2_O				51
#define		KEY_KEYBOARD2_P				52
#define		KEY_KEYBOARD2_Q				53
#define		KEY_KEYBOARD2_R				54
#define		KEY_KEYBOARD2_S				55
#define		KEY_KEYBOARD2_T				56
#define		KEY_KEYBOARD2_U				57
#define		KEY_KEYBOARD2_V				58
#define		KEY_KEYBOARD2_W				59
#define		KEY_KEYBOARD2_X				60
#define		KEY_KEYBOARD2_Y				61
#define		KEY_KEYBOARD2_Z				62

#define		KEY_KEYBOARDEX_1			63

#define		KEY_KEYBOARD3_SEL1			64
#define		KEY_KEYBOARD3_SEL2			65
#define		KEY_KEYBOARD3_SEL3			66
#define		KEY_KEYBOARD3_SEL4			67
#define		KEY_KEYBOARD3_SEL5			68
#define		KEY_KEYBOARD3_SEL6			69
#define		KEY_KEYBOARD3_SEL7			70
#define		KEY_KEYBOARD3_SEL8			71
#define		KEY_KEYBOARD3_SEL9			72
#define		KEY_KEYBOARD3_SEL10			73
#define		KEY_KEYBOARD3_SELPRES		74
#define		KEY_KEYBOARD3_SELPREV		75
#define		KEY_KEYBOARD3_SELNEXT		76

//------------------------------------------
// Function   : void Bor_Menu_KEYBOARD(U16 inpara)		
// Description: ���������	
// ���� :
//		0-�ļ�����->������(�����ļ�)
//		1-�ļ�����->�½��ļ���
//		2-�ļ�����->�ļ���������
//		3-�ļ�����->�ļ�������
//------------------------------------------
void Mit_Menu_Keyboard(U16 inpara)
{	
	U8  beeponf=0;
	U16 delay=0;
	U32 keyon=0,keyoff=0,keydata=0;
	U32 i;
	U8 disupdata1 =0,disupdata2 =0,disupdata3 =0,disupdata4 =0,disupdata5 =0; 
	U8 disupdata6 =0,disupdata7 =0,disupdata8 = 0; 
//	U16 multiple = 100;
//	U8 MakeType = 0,oMakeType = 0;
//	U8 DoType = 0,oDoType = 0;
//	U8 maxsel =0;
	U8 diskstatus;
	U8 UCase =0;	 //0:��д��ĸ  1��Сд��ĸ   ����������
	U8  pingyingbuf[7];
	U16 tmphzadd=0;
	U16 tmppysel[11];
	U8 sign = 0;	//0-��ĸ 1-����
	U8 zh = 0;	//����
	U8 inputType =0;	//�������� 0-Program NO 1-Comment
	U16 settingNo =0;
	U16 maxno =0,minno =0;
	U8 tlen =0,namelen=0,index = 0,oindex =0;
	U8 fileType =0;
	U8 nameMax =0;
	U32 addr;
	U8 res;
	KEYPAD *textKey = NULL;
	int charIndex = 0;
	char patname[64] = {0};
	const U8 keynum = {6};
	KEYPAD keyd[] = {{0,0,0,0},{0,0,128,32},{16,63,568,36},{16,129,568,36},{1,735,KEYX1,KEYY1},{535,735,KEYX1,KEYY1}};
	const U16 keybmpind[] = {0,0,0,0,1190,70};	
	
	const U8 keynum1 = {13};
	const KEYPAD keyd1[] = {{132,414,86,72},{132,340,86,72},{220,340,86,72},{308,340,86,72},{132,266,86,72},{220,266,86,72},{308,266,86,72},{132,192,86,72},{220,192,86,72},{308,192,86,72},
							{396,192,82,56},{478,268,54,56},{478,350,54,56}};
//	const U16 keybmpind1[]={745,746,747,748,749,750,751,752,753,754,755,756,757};
		
	const U8 keynum2 = {44};
//	const KEYPAD keyd2[] = {{643,184,70,56},{4,184,70,56},{75,184,70,56},{146,184,70,56},{217,184,70,56},{288,184,70,56},{359,184,70,56},{430,184,70,56},{501,184,70,56},{572,184,70,56},
//							{714,184,82,56},{4,300,116,56},{517,416,70,56},{588,416,70,56},{626,358,70,56},{697,358,70,56},{306,416,210,56},
//							{4,242,82,56},{87,242,70,56},{158,242,70,56},{229,242,70,56},{300,242,70,56},{371,242,70,56},{442,242,70,56},{513,242,70,56},{584,242,70,56},{655,242,70,56},{726,242,70,56},
//							{121,300,70,56},{192,300,70,56},{263,300,70,56},{334,300,70,56},{405,300,70,56},{476,300,70,56},{547,300,70,56},{618,300,70,56},{689,300,70,56},
//							{129,358,70,56},{200,358,70,56},{271,358,70,56},{342,358,70,56},{413,358,70,56},{484,358,70,56},{555,358,70,56},							
//							}; 
							//0				 1				2				3				4				5				6				7
	const KEYPAD t1keyd2[] = {{16,192,70,56},{87,192,70,56},{158,192,70,56},{229,192,70,56},{300,192,70,56},{371,192,70,56},{442,192,70,56},{513,192,70,56},
							//8				9			  del			 123				<-				->				  ,				  .				space				A/a
							{8,250,70,56},{79,250,70,56},{506,250,82,56},{300,250,116,56},{403,485,70,56},{474,485,70,56},{150,250,70,56},{221,250,70,56},{192,485,210,56},{420,250,82,56},
							//a				 b 				c				d				e				f				g				h				i				j
							{16,371,70,56},{302,428,70,56},{160,428,70,56},{158,371,70,56},{156,314,70,56},{229,371,70,56},{300,371,70,56},{371,371,70,56},{511,314,70,56},{442,371,70,56},
							//k				l				m				n				o				p				q				r				s
							{513,371,70,56},{121,485,70,56},{444,428,70,56},{373,428,70,56},{515,428,70,56},{50,485,70,56},{14,314,70,56},{227,314,70,56},{87,371,70,56},
							//t				u				v				w				x				y				z
							{298,314,70,56},{440,314,70,56},{231,428,70,56},{85,314,70,56},{89,428,70,56},{369,314,70,56},{18,428,70,56},														
							}; 	
							//0					1			2				3				4				5				6				7
	const KEYPAD t2keyd2[] = {{16,192,70,56},{87,192,70,56},{158,192,70,56},{229,192,70,56},{300,192,70,56},{371,192,70,56},{442,192,70,56},{513,192,70,56},
							//8				9			  del				123				<-				->											 	 space				A/a
							{8,250,70,56},{79,250,70,56},{506,250,82,56},{300,250,116,56},{403,485,70,56},{474,485,70,56},{150,250,70,56},{221,250,70,56},{192,485,210,56},{420,250,82,56},
							//!				 " 				#				$				&				'				(				)				*				
							{14,314,70,56},{85,314,70,56},{156,314,70,56},{227,314,70,56},{298,314,70,56},{369,314,70,56},{440,314,70,56},{511,314,70,56},{515,428,70,56},
							//+				,				-				.				/				:				;				=				?				@
							{16,371,70,56},{87,371,70,56},{158,371,70,56},{229,371,70,56},{300,371,70,56},{371,371,70,56},{442,371,70,56},{513,371,70,56},{121,485,70,56},{50,485,70,56},
							//[				]				_				`				{				}				~
							{18,428,70,56},{89,428,70,56},{160,428,70,56},{231,428,70,56},{302,428,70,56},{373,428,70,56},{444,428,70,56},														
						}; 
	
						
	const U16 keybmpind2[] ={755,769,770,771,798,799};	
	KEYPAD *keyd2;
	const U16 keybmpind_shift[] ={768,800,59};	
	
	
	const U8 keynum3 = {13};
	const KEYPAD keyd3[] = {{98,196,45,48},{144,196,45,48},{190,196,45,48},{236,196,45,48},{282,196,45,48},{328,196,45,48},{374,196,45,48},{420,196,45,48},{466,196,45,48},{512,196,45,48},
							{8,196,82,48},{8,250,64,48},{74,250,64,48}};	
	
							
	pingyingbuf[6] = 0;
	for(i=0;i<6;i++){
		pingyingbuf[i] = 0x20;
	}
	for(i=0;i<10;i++){
		tmppysel[i] = 0x2020;
	}
//	U16 color =0;
	InitScreen();	
#if	USEMULTCOLOR ==0	
	DrawFillRect(0,32,LCD_width,LCD_height-32,Color_Silver);
#endif
//	#if Debug
//	printf("Bor_Menu_Keyboard(%d)\r\n",inpara);
//	#endif
	keyd2 = (KEYPAD*)t1keyd2;
	
	if(inpara==0){	//�ļ�����->������(�����ļ�)
		SetLanguage(MENU_FTFILES);
		settingNo = g_patd.fno;
		maxno = MAXFILENO;
		minno = MINFILENO;
		nameMax = 43;
		DrawFillRect(0,33,LCD_width,147,Color_Gray);
		fileType = getFileTypeByIREG((U16*)IREG2,g_patd.fno);
		Draw24x24Str(16,37,MEM_TITLE_ADD+28*TitleLengthMax,SheetPage,28,Color_White);	//�����
		if(fileType==FILE_SEW || fileType==FILE_EMB ){
			Draw24x24Str(16,103,MEM_TITLE_ADD+27*TitleLengthMax,SheetPage,27,Color_White);	//ע��	
		}else{
			Draw24x24Str(16,103,MEM_TITLE_ADD+26*TitleLengthMax,SheetPage,26,Color_White);	//�ļ���
		}
		DrawFillRect(16,63,568,36,Color_White);		
		DrawFillRect(16,129,568,36,Color_White);
		
		for(i=0;i<nameMax;i++){
			if(g_patd.fname[i]==0)	break;
			patname[i] = g_patd.fname[i]; 
			namelen++;
		}
		disupdata1 = 1;
		disupdata2 = 1;
		disupdata5 = 1;		
		textKey = &keyd[KEY_KEYBOARD_TEXT];
	}else if(inpara==1 || inpara==2 || inpara ==3){	//�ļ�����->�½��ļ���/�ļ�������
		SetLanguage(MENU_FTFILES);
		DrawFillRect(0,33,LCD_width,147,Color_Gray);
		nameMax = 43;	
		if(inpara==2){	//�ļ���������
			for(i=0;i<nameMax;i++){	
				if(g_patname[i]==0)	break;
				patname[i] = g_patname[i]; 
				namelen++;
			}
			disupdata6 = 1;
		}else if(inpara==3){	//�����ļ�������
			charIndex = getCharLastIndexOnstr((char*)g_patname,'.');		
			if(charIndex==-1){	//���ļ��޺�׺��
				for(i=0;i<nameMax;i++){	
					if(g_patname[i]==0)	break;
					patname[i] = g_patname[i]; 
					namelen++;
				}			
			}else{
				for(i=0;i<charIndex;i++){	
					patname[i] = g_patname[i]; 
					namelen++;
				}			
			}
			disupdata6 = 1;
		}
		Draw24x24Str(16,50,MEM_TITLE_ADD+26*TitleLengthMax,SheetPage,26,Color_White);	//�ļ���
		DrawFillRect(16,88,568,64,Color_LBlue);	
		inputType = 1;	
		keyd[KEY_KEYBOARD_TEXT].xpis = 16;
		keyd[KEY_KEYBOARD_TEXT].ypis = 102;
		disupdata3 = 1;
		disupdata4 = 1;		
		textKey = &keyd[KEY_KEYBOARD_TEXT];
	}
	
	index = namelen;
	oindex = index;
	
	DrawButton2(keyd[KEY_KEYBOARD_BACK],Color_White1,keybmpind[KEY_KEYBOARD_BACK],1);
	DrawButton2(keyd[KEY_KEYBOARD_ENTER],Color_White1,keybmpind[KEY_KEYBOARD_ENTER],1);	
	
	ViewPage(usepageno);
	initViewStatus();	
	while(1){		
		keydata = get_keydata((KEYPAD *)keyd,keynum,0);
		if((inputType==0)&&(keydata==0)){
			keydata = get_keydata((KEYPAD *)keyd1,keynum1,KEY_KEYBOARD1_0);
		}else if((inputType==1)&&(keydata==0)){
			keydata = get_keydata((KEYPAD *)keyd2,keynum2,KEY_KEYBOARD2_0);
			if(UCase>2&&((keydata>=KEY_KEYBOARD2_0&&keydata<=KEY_KEYBOARD2_9) ||(keydata>=KEY_KEYBOARD2_CHAT1&&keydata<=KEY_KEYBOARD2_CHAT2))){
				keydata = 0;keyolddata = 0;
				keydata = get_keydata((KEYPAD *)keyd3,keynum3,KEY_KEYBOARD3_SEL1);
			}				
		}

		if((inpara==1) &&(keydata==KEY_KEYBOARD_NO||keydata==KEY_KEYBOARD_TEXT)){keydata =0;}
		if((sign==1)&&((keydata==KEY_KEYBOARD2_CHAT1)||(keydata==KEY_KEYBOARD2_CHAT2))){keydata =0;}

		if(keydata==0xff){
			if((keyon==KEY_KEYBOARD1_UP)||(keyon==KEY_KEYBOARD1_DN)||(keyon==KEY_KEYBOARD1_DEL)||(keyon==KEY_KEYBOARD2_DEL)||(keyon==KEY_KEYBOARD2_TOL)||(keyon==KEY_KEYBOARD2_TOR)){}
			else if((keyon)&&(beeponf)){keyoff = keyon;}
			keydata = 0;
		}else if(keydata>USESCREENSAVER){
			if(keyon==0){
				keyon = keydata;
//				#if Debug
//				printf("Bor_Menu_KEYBOARD	keyon = %d \r\n",keyon); 
//				#endif
				if(keyon==KEY_KEYBOARD_SCRNSVR);
				else{
					if((keydata>KEY_KEYBOARD_TEXT)&&(keydata<KEY_KEYBOARD1_0)){
						DrawButtonOn(keyd[keydata],KEY_Gray);
					}else if((keydata>=KEY_KEYBOARD1_0)&&(keydata<KEY_KEYBOARD2_0)){
						DrawButtonOn(keyd1[keydata-KEY_KEYBOARD1_0],KEY_White);
					}else if((keydata>=KEY_KEYBOARD2_0)&&(keydata<KEY_KEYBOARDEX_1)){
						DrawButtonOn(keyd2[keydata-KEY_KEYBOARD2_0],KEY_White);
					}else if(keyon>=KEY_KEYBOARD3_SEL1&&keyon<=KEY_KEYBOARD3_SELNEXT){
						if(keyon<KEY_KEYBOARD3_SELPRES){
							DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_KeyBlue);
							Draw24X24FPY(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+10,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+4, (char *)(&tmppysel[keyon-KEY_KEYBOARD3_SEL1]), 2 , Color_White);
						}else if(keyon==KEY_KEYBOARD3_SELPRES){
							DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_KeyBlue);
							Draw24X24FStr(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+2,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+8,(char *)(pingyingbuf),Color_White);
						}else if(keyon==KEY_KEYBOARD3_SELPREV){
							DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_KeyBlue);
							Draw24X24FStr(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+8,"<-",Color_White);
						}else if(keyon==KEY_KEYBOARD3_SELNEXT){
							DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_KeyBlue);
							Draw24X24FStr(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+8,"->",Color_White);
						}				
					
					}
				}
				Times = 0;
				delay = KEYDT;
			}else if(keyon!=keydata){
				keydata = 0;
			}

			if(Times>delay){
				if(beeponf==0){
					beeponf=1;if(P_KBC==0)Beep_On(BELLT);					
					delay = KEYFT;
				}else if(beeponf==1){
					beeponf=2;
					delay = KEYNT;
				}
				
				Times = 0;
				if((keyon==KEY_KEYBOARD1_UP)||(keyon==KEY_KEYBOARD1_DN)||(keyon==KEY_KEYBOARD1_DEL)||(keyon==KEY_KEYBOARD2_DEL)||(keyon==KEY_KEYBOARD2_TOL)||(keyon==KEY_KEYBOARD2_TOR)){
					keyoff = keyon;
				}
			}
		}else{
			delay = KEYDT;
		}
		
		if(keyoff){						
			if((keyoff>=KEY_KEYBOARD1_0) && (keyoff<=KEY_KEYBOARD1_9)){
				if(inpara==0 ){//�ļ�������-�����
					settingNo = (settingNo%100)*10 + keyoff-KEY_KEYBOARD1_0;
				}				
				disupdata5 = 1;
			}else if((keyoff>=KEY_KEYBOARD2_0) && (keyoff<=KEY_KEYBOARD2_9)){
				if(index==namelen || namelen==0){	//�����������
					if(namelen<nameMax){
						patname[namelen] = keyoff - KEY_KEYBOARD2_0 +'0';
						namelen ++;	
						index = namelen;
						disupdata6 = 1;
					}							
				}else{
					zh = 0;
					oindex = index;
					if(patname[index]&0x80 && patname[index+1]&0x80){	//����
						patname[index+1] = '0';
						removeStrByIndex((U8*)patname,sizeof(patname),index+1); 
						namelen--;
						DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);	
						zh = 1;	
					}
					DrawFillRect(textKey->xpis+8+index*13,textKey->ypis+4,zh?26:13,30,Color_KeyBlue);					
					patname[index] = keyoff - KEY_KEYBOARD2_0 +'0';
					
					disupdata7 = 1;
				}
			}else if((keyoff>=KEY_KEYBOARD2_A) && (keyoff<=KEY_KEYBOARD2_Z)){
				if(sign==0){	//��ĸ����
					if(UCase<2){
						if(index==namelen || namelen==0){	//���������ĸ
							if(namelen<nameMax){
								patname[namelen] = keyoff - KEY_KEYBOARD2_A +(UCase==0?'A':'a');
								namelen ++;	
								index = namelen;
								disupdata6 = 1;
							}							
						}else{
							zh = 0;
							oindex = index;
							if(patname[index]&0x80 && patname[index+1]&0x80){	//����
								patname[index+1] = '0';
								removeStrByIndex((U8*)patname,sizeof(patname),index+1); 
								namelen--;
								DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);	
								zh = 1;	
							}
							DrawFillRect(textKey->xpis+8+index*13,textKey->ypis+4,zh?26:13,30,Color_KeyBlue);
							patname[index] = keyoff - KEY_KEYBOARD2_A +(UCase==0?'A':'a');
							disupdata7 = 1;
						}	
					}else if((UCase<8)){	//����
						pingyingbuf[UCase-2] = keyoff - KEY_KEYBOARD2_A + 0x61;
						UCase ++;
						disupdata8 = 1;
						tmphzadd = FinePYFirstAdd(pingyingbuf,0x0);
						ReadHzData(tmppysel,10,tmphzadd);				
					}						
				}else{		//��������
					if(index==namelen || namelen==0){	//���������ĸ
						if(namelen<nameMax && keyoff<KEY_KEYBOARD2_X){
							if(keyoff>=KEY_KEYBOARD2_A && keyoff<=KEY_KEYBOARD2_D){
								patname[namelen] = keyoff - KEY_KEYBOARD2_A +'!';
							}if(keyoff>=KEY_KEYBOARD2_E && keyoff<=KEY_KEYBOARD2_N){
								patname[namelen] = keyoff - KEY_KEYBOARD2_E +'&';
							}else if(keyoff>=KEY_KEYBOARD2_O && keyoff<=KEY_KEYBOARD2_P){
								patname[namelen] = keyoff - KEY_KEYBOARD2_O +':';
							}else if(keyoff==KEY_KEYBOARD2_Q){
								patname[namelen] = '=';
							}else if(keyoff>=KEY_KEYBOARD2_R && keyoff<=KEY_KEYBOARD2_S){
								patname[namelen] = keyoff - KEY_KEYBOARD2_R +'?';
							}else if(keyoff==KEY_KEYBOARD2_T){
								patname[namelen] = '[';
							}else if(keyoff==KEY_KEYBOARD2_U){
								patname[namelen] = ']';
							}else if(keyoff>=KEY_KEYBOARD2_V && keyoff<=KEY_KEYBOARD2_W){
								patname[namelen] = keyoff - KEY_KEYBOARD2_V +'_';
							}else if(keyoff==KEY_KEYBOARD2_X){
								//patname[namelen] = 123;
							}else if(keyoff>=KEY_KEYBOARD2_Y && keyoff<=KEY_KEYBOARD2_Z){
								//patname[namelen] = keyoff - KEY_KEYBOARD2_Y +125;
							}						
							namelen ++;	
							index = namelen;
							disupdata6 = 1;
						}							
					}else{
						if( keyoff<KEY_KEYBOARD2_X){
	
							oindex = index;
							zh = 0;
							if(patname[index]&0x80 && patname[index+1]&0x80){	//����
								patname[index+1] = '0';
								removeStrByIndex((U8*)patname,sizeof(patname),index+1); 
								namelen--;
								DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);	
								zh = 1;	
							}
							
							if(keyoff>=KEY_KEYBOARD2_A && keyoff<=KEY_KEYBOARD2_D){
								patname[index] = keyoff - KEY_KEYBOARD2_A +'!';
							}if(keyoff>=KEY_KEYBOARD2_E && keyoff<=KEY_KEYBOARD2_N){
								patname[index] = keyoff - KEY_KEYBOARD2_E +'&';
							}else if(keyoff>=KEY_KEYBOARD2_O && keyoff<=KEY_KEYBOARD2_P){
								patname[index] = keyoff - KEY_KEYBOARD2_O +':';
							}else if(keyoff==KEY_KEYBOARD2_Q){
								patname[index] = '=';
							}else if(keyoff>=KEY_KEYBOARD2_R && keyoff<=KEY_KEYBOARD2_S){
								patname[index] = keyoff - KEY_KEYBOARD2_R +'?';
							}else if(keyoff==KEY_KEYBOARD2_T){
								patname[index] = '[';
							}else if(keyoff==KEY_KEYBOARD2_U){
								patname[index] = ']';
							}else if(keyoff>=KEY_KEYBOARD2_V && keyoff<=KEY_KEYBOARD2_W){
								patname[index] = keyoff - KEY_KEYBOARD2_V +'_';
							}else if(keyoff==KEY_KEYBOARD2_X){
								//patname[index] = 123;
							}else if(keyoff>=KEY_KEYBOARD2_Y && keyoff<=KEY_KEYBOARD2_Z){
								//patname[index] = keyoff - KEY_KEYBOARD2_Y +125;
							}							
							DrawFillRect(textKey->xpis+8+index*13,textKey->ypis+4,zh?26:13,30,Color_KeyBlue);
							disupdata7 = 1;							
						}
					}
				}
			}else if(keyoff==KEY_KEYBOARD2_CHAT1 || keyoff==KEY_KEYBOARD2_CHAT2 || keyoff == KEY_KEYBOARD2_SPACE){
				if((index==namelen || namelen==0)&&(UCase<=2)){	//���������ĸ
					if(namelen<nameMax){
						switch(keyoff){
							case KEY_KEYBOARD2_CHAT1: patname[namelen] = ',';break;
							case KEY_KEYBOARD2_CHAT2: patname[namelen] = '.';break;
							case KEY_KEYBOARD2_SPACE: patname[namelen] = ' ';break;
						}
						namelen ++;	
						index = namelen;
						disupdata6 = 1;
					}					
				}else if(UCase<=2){

					zh = 0;
					oindex = index;
					if(patname[index]&0x80 && patname[index+1]&0x80){	//����
						patname[index+1] = '0';
						removeStrByIndex((U8*)patname,sizeof(patname),index+1); 
						namelen--;
						DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);	
						zh = 1;	
					}
					switch(keyoff){
						case KEY_KEYBOARD2_CHAT1: patname[index] = ',';break;
						case KEY_KEYBOARD2_CHAT2: patname[index] = '.';break;
						case KEY_KEYBOARD2_SPACE: patname[index] = ' ';break;
					}					
					DrawFillRect(textKey->xpis+8+index*13,textKey->ypis+4,zh?26:13,30,Color_KeyBlue);	
					disupdata7 = 1;
				}			
			}else if((keyoff >=KEY_KEYBOARD3_SEL1)&&(keyoff<=KEY_KEYBOARD3_SEL10)){
				if((tmppysel[keyoff - KEY_KEYBOARD3_SEL1]&0x80)&&((tmppysel[keyoff - KEY_KEYBOARD3_SEL1]>>8)&0x80)){	//��Ч�����ַ��ű���
					if(index==namelen || namelen==0){	//���������ĸ
						if(namelen<nameMax-1){							
							patname[namelen++]= tmppysel[keyoff - KEY_KEYBOARD3_SEL1];							
							patname[namelen++] = tmppysel[keyoff - KEY_KEYBOARD3_SEL1]>>8;											

							index = namelen;
							disupdata6 = 1;
						}					
					}else{ //�޸���ĸ
						zh = 0;
						oindex = index;
						if(patname[index]&0x80 && patname[index+1]&0x80){	//����
							patname[index] = tmppysel[keyoff - KEY_KEYBOARD3_SEL1];	
							patname[index+1] = tmppysel[keyoff - KEY_KEYBOARD3_SEL1]>>8;	
							DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);	
							zh = 1;	
						}else if(namelen<nameMax){
							for(i=namelen-1;i>index;i--){
								patname[i+1] = patname[i];
							}
							patname[index] = tmppysel[keyoff - KEY_KEYBOARD3_SEL1];	
							patname[index+1] = tmppysel[keyoff - KEY_KEYBOARD3_SEL1]>>8;	
							DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);	
							namelen++;
							zh = 1;							
						}
						DrawFillRect(textKey->xpis+8+index*13,textKey->ypis+4,zh?26:13,30,Color_KeyBlue);	
						disupdata7 = 1;						
					}
					while(UCase>2){
						UCase --;
						pingyingbuf[UCase-2] = 0x20;						
					}
					disupdata8 = 1;
					tmphzadd = FinePYFirstAdd(pingyingbuf,0x0);
					ReadHzData(tmppysel,10,tmphzadd);
				}
			}else{
				switch(keyoff){
					case KEY_KEYBOARD_SCRNSVR:
						pno	= MENU_SCRNSVR;
						break;
					
					case KEY_KEYBOARD_NO:
						if(inputType!=0){
							inputType =0;
							disupdata1 =1;
							disupdata2 = 1;
						}						
						break;

					case KEY_KEYBOARD_TEXT:
						if(inputType!=1){
							inputType =1;
							disupdata1 =1;
							disupdata3 =1;
							disupdata4 = 1;
						}						
						break;
						
					case KEY_KEYBOARD_BACK:
						 pno = poldno[--pnoindex];
						break;	

					case KEY_KEYBOARD_ENTER:
						if(inpara==0){	//�����ļ�������
							if(g_disk_memory== MEM_IN){	//�ڴ��ļ�
								if(settingNo<MINFILENO || settingNo>MAXFILENO){
									hleppara = 56;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}
								setBufDat8((U8*)g_patname,sizeof(g_patname),0);
								for(i=0;i<namelen;i++){
									g_patname[i] = patname[i];
								}
								
								if(isContainFileByIREG((U16*)IREG2,settingNo) && settingNo != g_patd.fno){
									g_newname[0] = ((settingNo % 1000)/100) +'0';
									g_newname[1] = ((settingNo % 100)/10) +'0';
									g_newname[2] = (settingNo %10) +'0';										

									hleppara = 167;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}
								Mit_DealSaveData(fileType==FILE_SEW?PAT_SEW:PAT_MIT,g_patd.pat_len);
								HREG[0] = settingNo;
								HREG[8] = fileType==FILE_SEW?PAT_SEW:PAT_MIT;
								if(fileType==FILE_SEW){
									setBufDat8((U8*)g_patname,sizeof(g_patname),0);
									g_patname[0] = 'I';
									g_patname[1] = 'S';
									g_patname[2] = 'M';
									g_patname[3] = 'S';
									g_patname[4] = '0';
									g_patname[5] = ((settingNo % 1000)/100) +'0';
									g_patname[6] = ((settingNo % 100)/10) +'0';
									g_patname[7] = (settingNo %10) +'0';
									g_patname[8] = '.';
									g_patname[9] = 'S';
									g_patname[10] = 'E';
									g_patname[11] = 'W';									
								}
								for(i=0;i<32;i++){	 
									if( g_patname[2*i]==0)  break;
									HREG[32+i] = (g_patname[2*i+1]<<8)| g_patname[2*i];
								}
								//+----	 ɾ����ǰ��Ż���---------							
								DREG[D_COMMAND] = C_DELMEMORYPAT;			// ɾ������
								DREG[D_CMDSTATUS] = 0;
								DREG[D_CMDPARA1]  = g_patd.fno;
								res = Write_D(D_Address[DISP]+D_COMMAND,3,(U16 *)(&DREG[D_COMMAND]));
								if(res==0){
									hleppara = 15;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}
								if(Read_Status()==0){
									hleppara = 15;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}
								if(DREG[D_CMDSTATUS]!=100){
									hleppara = 16;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}
								//�����ļ�����
								addr = (g_patd.fno - MINFILENO) * PTINDEXREDLEN;
								for(i=0;i<FileIndexSize;i++){
									IREG2[addr+i] = 0xffff;
								}								
								
								//----- �����±�Ż���
								g_patd.fno = HREG[0];
								res = Write_D(D_Address[PTDATA],(g_patd.pat_len>>1),(U16 *)(&SREG[0]));//���ͻ�������
								if(res==0){
									hleppara = 15;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}	
								res = Write_D(D_Address[HEAD],D_Length[HEAD],(U16 *)(&HREG[0]));
								if(res==0){
									hleppara = 15;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}	
								if(Write_Cmd(C_WRITEMEMORYPAT,HREG[0],0)==0){
									hleppara = 15;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;
								}
								//�����ļ�����
								addr = (HREG[0] - MINFILENO) * PTINDEXREDLEN;
								IREG2[addr+PTINDEXLENADD+0] = g_patd.pat_len;
								IREG2[addr+PTINDEXLENADD+1] = g_patd.pat_len>>16;
								IREG2[addr+PTINDEXENCADD+0] = 0;
								IREG2[addr+PTINDEXENCADD+1] = 0;
								for(i=0;i<32;i++){
									IREG2[addr+PTINDEXNAMEADD+i] = HREG[32+i];
								}
								if(HREG[8]==PAT_MIT){	
									IREG2[addr+PTINDEXNAMEADD+31] &= 0x00ff;
									IREG2[addr+PTINDEXNAMEADD+31] |= 0x0100;
								}else{	
									IREG2[addr+PTINDEXNAMEADD+31] &= 0x00ff;
								}								
								pnoindex--;	pno = poldno[pnoindex];							
							}
							initFunctionBtn(1);
							
						}else if(inpara==1 || inpara ==2 || inpara==3){	//�½��ļ��� /�ļ�������
							if(namelen==0){
								hleppara = 168;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;																						
							}
							diskstatus = getDiskStatus();
							if(g_disk_memory == MEM_USB){
								if((diskstatus&0x02) !=0x02){
									hleppara = 95;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;										
								}
								Select_ExtMemory(USBTF);								
							}
							if(inpara==1 ){
								res = newDiskFolder(g_path,patname);
							}else{
								if(inpara==3 && charIndex !=-1){	//�����ļ���׺��
									i = 0;
									while((*(g_patname+charIndex+i)!=0) && (namelen<64) && (i+charIndex<64)){
										patname[namelen++] = *(g_patname+charIndex+i);
										i++;
									}
								}
								res = renameDiskFile(g_path,(char*)g_patname,patname); 
								if(res!=0){
									hleppara = 169;pno = MENU_RWHELP;poldno[pnoindex++] = MENU_FTKEYBOARD;break;	
								}
							}							
							getDiskFiles(g_path,(U16*)IREG2,GET_ALLFILE);	
							pnoindex--;	pno = poldno[pnoindex];
							initFunctionBtn(inpara==1?0:1);
						}
						break;	
					case KEY_KEYBOARD1_UP:
						if(settingNo<maxno){							
							disupdata5 =1;
							settingNo++;							
						}else if(settingNo>maxno){
							
							disupdata5 =1;
							settingNo = maxno;
						}
						break;
					
					case KEY_KEYBOARD1_DN:
						if(settingNo>minno){							
							disupdata5 = 1;
							settingNo --;							
						}
						break;
						
					case KEY_KEYBOARD1_DEL:
						if(settingNo){
							settingNo /=10;
							disupdata5 = 1;
						}
						break;
						
					case KEY_KEYBOARD2_SHIFT:	//��Сдת��
						if(sign==0){
							if(UCase>2){
								disupdata8 = 1;
								while(UCase>2){
									UCase --;
									pingyingbuf[UCase-2] = 0x20;
								}
							}
						
							UCase++;
							if(UCase>2) UCase =0;
							disupdata4 = 1;							
						}
						break;
						
					case KEY_KEYBOARD2_123:
						if(sign==0){
							sign = 1;
							if(UCase>2){
								disupdata8 = 1;
								while(UCase>2){
									UCase --;
									pingyingbuf[UCase-2] = 0x20;
								}
							}								
						}else{
							sign = 0;
						}
						disupdata4 = 1;		
						break;
					
					case KEY_KEYBOARD2_TOL:
						if(index){
							oindex = index;						
							if(patname[index-2]&0x80 &&patname[index-1]&0x80 && index>1){	//����
								index -=2;
							}else{
								index--;								
							}					
							disupdata7 =1;							
						}
						break;
					case KEY_KEYBOARD2_TOR:
						if(index<namelen){
							oindex = index;
							if(patname[index]&0x80 &&patname[index+1]&0x80 && index<namelen-1){	//����
								index +=2;
							}else{
								index++;
							}										
							disupdata7 =1;
						}
						break;
					case KEY_KEYBOARD2_DEL:
						if(UCase>2){
							UCase --;
							pingyingbuf[UCase-2] = 0x20;
							disupdata8 = 1;
							tmphzadd = FinePYFirstAdd(pingyingbuf,0x0);
							ReadHzData(tmppysel,10,tmphzadd);						
						}else{
							if(namelen>0){
								if(index==namelen){
									if(patname[index-1]&0x80 &&patname[index-2]&0x80){	//����
										index --;
									}
									removeStrByIndex((U8*)patname,sizeof(patname),index-1);  
									tlen = getStrLen((U8*)patname);	//��ȡɾ�����ַ�������
									DrawFillRect(textKey->xpis+8+(namelen-(namelen-tlen))*13,textKey->ypis+8,(namelen-tlen)*13,24,Color_LBlue);	
								
								}else{
									removeStrByIndex((U8*)patname,sizeof(patname),index);  
									tlen = getStrLen((U8*)patname);	//��ȡɾ�����ַ�������
									DrawFillRect(textKey->xpis+8,textKey->ypis+4,textKey->xsize-8,30,Color_LBlue);							
								}
								namelen = tlen;		
								index = namelen;	
								disupdata6 =1;								
							}
						}						
						break;
						
				case KEY_KEYBOARDEX_1:
					break;
				
				case KEY_KEYBOARD3_SELPRES:
					tmphzadd = FinePYFirstAdd(pingyingbuf,0x0);
					ReadHzData(tmppysel,10,tmphzadd);
					disupdata8 = 1;
					break;
				case KEY_KEYBOARD3_SELPREV:
					if(tmphzadd>10)tmphzadd-=10;else tmphzadd=1;
					ReadHzData(tmppysel,10,tmphzadd);
					disupdata8 = 1;
					break;
				case KEY_KEYBOARD3_SELNEXT:
					if((tmphzadd+10)<=HZLIBSIZE)tmphzadd += 10;
					ReadHzData(tmppysel,10,tmphzadd);
					disupdata8 = 1;
					break;					
				}			
			}

			keyoff = 0;
		}
		if((keyon)&&(keydata==0)){
			if(keyon==KEY_KEYBOARD_SCRNSVR);
			else{
				if(keyon==KEY_KEYBOARD_BACK || keyon==KEY_KEYBOARD_ENTER){
					DrawButtonOff2(keyd[keyon],Color_White1);
				}else if((keyon>=KEY_KEYBOARD1_0)&&(keyon<KEY_KEYBOARD2_0)){
					DrawButtonOff2(keyd1[keyon-KEY_KEYBOARD1_0],Color_White1);
				}else if((keyon>=KEY_KEYBOARD2_0)&&(keyon<KEY_KEYBOARDEX_1)){
					DrawButtonOff2(keyd2[keyon-KEY_KEYBOARD2_0],Color_White1);
				}else if(keyon>=KEY_KEYBOARD3_SEL1&&keyon<=KEY_KEYBOARD3_SELNEXT){
					if(keyon<=KEY_KEYBOARD3_SEL10){
						DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
						DrawFU32(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+32,(keyon-KEY_KEYBOARD3_SEL1),1,Color_White);
						Draw24X24FPY(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+10,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+4, (char *)(&tmppysel[keyon-KEY_KEYBOARD3_SEL1]), 2 , Color_Green);
					}else if(keyon==KEY_KEYBOARD3_SELPRES){
						DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
						Draw24X24FStr(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+2,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+8,(char *)(pingyingbuf),Color_White);
					}else if(keyon==KEY_KEYBOARD3_SELPREV){
						DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
						Draw24X24FStr(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+8,"<-",Color_White);
					}else if(keyon==KEY_KEYBOARD3_SELNEXT){
						DrawFillRect(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis,keyd3[keyon-KEY_KEYBOARD3_SEL1].xsize,keyd3[keyon-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
						Draw24X24FStr(keyd3[keyon-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[keyon-KEY_KEYBOARD3_SEL1].ypis+8,"->",Color_White);
					}
					
				}				
			}
			keyon = 0;
			beeponf = 0;
		}

		if(disupdata1){
//			#if Debug
//			printf("disupdata1 \r\n");
//			#endif
			disupdata1 =0;
			for(i=0;i<2;i++){
				if(i== inputType){
					FillRectColorChang(keyd[i+KEY_KEYBOARD_NO].xpis,keyd[i+KEY_KEYBOARD_NO].ypis,keyd[i+KEY_KEYBOARD_NO].xsize,keyd[i+KEY_KEYBOARD_NO].ysize,Color_White,Color_LBlue);
				}else{
					FillRectColorChang(keyd[i+KEY_KEYBOARD_NO].xpis,keyd[i+KEY_KEYBOARD_NO].ypis,keyd[i+KEY_KEYBOARD_NO].xsize,keyd[i+KEY_KEYBOARD_NO].ysize,Color_LBlue,Color_White);
					FillRectColorChang(keyd[i+KEY_KEYBOARD_NO].xpis,keyd[i+KEY_KEYBOARD_NO].ypis,keyd[i+KEY_KEYBOARD_NO].xsize,keyd[i+KEY_KEYBOARD_NO].ysize,Color_KeyBlue,Color_White);
					oindex= namelen;
					index= namelen;
				}
			}
			DrawMFU32Z(keyd[KEY_KEYBOARD_NO].xpis+8,keyd[KEY_KEYBOARD_NO].ypis+8,settingNo, 3, Color_Black);		
			Draw24X24FStr(textKey->xpis+8,textKey->ypis+8,patname,Color_Black);
			
			//printf("disupdata1 name:%s \r\n",patname);
			//Draw24X24FStr(keyd[KEY_KEYBOARD_TEXT].xpis+8,keyd[KEY_KEYBOARD_TEXT].ypis+8,"[]-/{}~11",Color_Black);
		}
		
		if(disupdata2){
//			#if Debug
//			printf("disupdata2 \r\n");
//			#endif
			disupdata2 = 0;
			//BmpFillRect(0,184,LCD_width,296);
			//DrawButton1(keyd[KEY_KEYBOARD_BACK],Color_Gray,keybmpind[KEY_KEYBOARD_BACK]);
			//DrawButton1(keyd[KEY_KEYBOARD_ENTER],Color_GrassGreen,keybmpind[KEY_KEYBOARD_ENTER]);	
			if(UCase>2){
				while(UCase>2){
					UCase --;
					pingyingbuf[UCase-2] = 0x20;
				}
			#if USEMULTCOLOR
				BmpFillRect(8,192,580,56);
				BmpFillRect(8,250,283,56);
			#else
				DrawFillRect(8,192,580,56,Color_Silver);
				DrawFillRect(8,250,283,56,Color_Silver);
			#endif
			}					
			
			for(i=0;i<keynum2;i++){
			#if	USEMULTCOLOR 
				BmpFillRect(keyd2[i].xpis,keyd2[i].ypis,keyd2[i].xsize,keyd2[i].ysize);
			#else
				DrawFillRect(keyd2[i].xpis,keyd2[i].ypis,keyd2[i].xsize,keyd2[i].ysize,Color_Silver);
			#endif
			}
			
			for(i=0;i<keynum1;i++){	
				DrawButton2(keyd1[i],Color_White1,745+i,i<11?0:1);
			} 
		}
		
		if(disupdata3){
//			#if Debug
//			printf("disupdata3 \r\n");
//			#endif
			disupdata3 =0;
			//BmpFillRect(0,184,LCD_width,296);
			//DrawButton1(keyd[KEY_KEYBOARD_BACK],Color_Gray,keybmpind[KEY_KEYBOARD_BACK]);
			//DrawButton1(keyd[KEY_KEYBOARD_ENTER],Color_GrassGreen,keybmpind[KEY_KEYBOARD_ENTER]);
			for(i=0;i<keynum1;i++){
			#if	USEMULTCOLOR 
				BmpFillRect(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize);
			#else
				DrawFillRect(keyd1[i].xpis,keyd1[i].ypis,keyd1[i].xsize,keyd1[i].ysize,Color_Silver);
			#endif
			}
			for(i=0;i<KEY_KEYBOARD2_SHIFT-KEY_KEYBOARD2_0;i++){	
				if(i<=KEY_KEYBOARD2_9-KEY_KEYBOARD2_0){
					DrawButton1(keyd2[i],Color_White1,i+758);
				}else if(i<=KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0){
					DrawButton1(keyd2[i],Color_White1,keybmpind2[i+KEY_KEYBOARD2_0-KEY_KEYBOARD2_DEL]);
				}else{				
				#if USEMULTCOLOR
					DrawButton2(keyd2[i],Color_White1,0,1);
				#else
					DrawFillRect(keyd2[i].xpis,keyd2[i].ypis,keyd2[i].xsize,keyd2[i].ysize,Color_White1);
				#endif
				}				
			} 
		}
		
		if(disupdata4){
//			#if Debug
//			printf("disupdata4 \r\n");
//			#endif
			disupdata4 =0;		
			DrawButton1(keyd2[KEY_KEYBOARD2_SHIFT-KEY_KEYBOARD2_0],Color_White1,keybmpind_shift[UCase]);
			if(sign==0){			
				keyd2 = (KEYPAD*)t1keyd2;
				DrawButton1(keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0],Color_White1,798);
				DrawButton1(keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0],Color_White1,799);
				DrawButton1(keyd2[KEY_KEYBOARD2_123-KEY_KEYBOARD2_0],Color_White1,769);
				for(i=0;i<=KEY_KEYBOARD2_Z-KEY_KEYBOARD2_A;i++){
					DrawButton1(keyd2[KEY_KEYBOARD2_A-KEY_KEYBOARD2_0+i],Color_White1,UCase?772+i:801+i);
				}		
			}else{
				keyd2 = (KEYPAD*)t2keyd2;
				DrawButton1(keyd2[KEY_KEYBOARD2_123-KEY_KEYBOARD2_0],Color_White1,925);
				//BmpFillRect(keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].xpis,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].ypis,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].xsize,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].ysize);
				//BmpFillRect(keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].xpis,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].ypis,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].xsize,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].ysize);
			#if USEMULTCOLOR
				BmpFillRect(keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].xpis,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].ypis,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].xsize,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].ysize);
				BmpFillRect(keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].xpis,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].ypis,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].xsize,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].ysize);
			#else
				DrawFillRect(keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].xpis,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].ypis,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].xsize,keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0].ysize,Color_Silver);
				DrawFillRect(keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].xpis,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].ypis,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].xsize,keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0].ysize,Color_Silver);
			#endif
				for(i=0;i<=KEY_KEYBOARD2_Z-KEY_KEYBOARD2_A;i++){
					DrawButton1(keyd2[KEY_KEYBOARD2_A-KEY_KEYBOARD2_0+i],Color_White1,926+i);				
				}				
			}
		}
		
		if(disupdata5){
//			#if Debug
//			printf("disupdata5 \r\n");
//			#endif
			disupdata5 = 0;
			if(inpara==0){//�ļ�������-����� 
				FillRectColorChang(keyd[KEY_KEYBOARD_NO].xpis+8,keyd[KEY_KEYBOARD_NO].ypis+8,40,24,Color_Black,Color_LBlue);	
				DrawMFU32Z(keyd[KEY_KEYBOARD_NO].xpis+8,keyd[KEY_KEYBOARD_NO].ypis+8,settingNo, 3, Color_Black);
			}
		}
		
		if(disupdata6){
//			#if Debug
//			printf("disupdata6 \r\n");
//			#endif
			disupdata6 = 0;
			//FillRectColorChang(keyd[KEY_KEYBOARD_TEXT].xpis+8,keyd[KEY_KEYBOARD_TEXT].ypis+8,keyd[KEY_KEYBOARD_TEXT].xsize-16,24,Color_Black,Color_LBlue);	
			Draw24X24FStr(textKey->xpis+8,textKey->ypis+8,patname,Color_Black);				
			
		}
		
		if(disupdata7){
//			#if Debug
//			printf("disupdata7 \r\n");
//			#endif
			disupdata7 = 0;
			if(oindex<namelen){
				if((patname[oindex]&0x80 &&patname[oindex+1]&0x80) || zh){	//����
					FillRectColorChang(textKey->xpis+8+oindex*13,textKey->ypis+4,2*13,30,Color_KeyBlue,Color_LBlue);	
				}else{
					FillRectColorChang(textKey->xpis+8+oindex*13,textKey->ypis+4,13,30,Color_KeyBlue,Color_LBlue);	
				}
			}
			Draw24X24FStr(textKey->xpis+8,textKey->ypis+8,patname,Color_Black);

//			#if Debug
//			printf("disupdata7 oindex = %d index = %d  namelen = %d patname = %s\r\n",oindex,index,namelen,patname);
//			#endif
			
			if(index<namelen){
				if(patname[index]&0x80 &&patname[index+1]&0x80){	//����
					FillRectColorChang(textKey->xpis+8+index*13,textKey->ypis+4,2*13,30,Color_LBlue,Color_KeyBlue);
					FillRectColorChang(textKey->xpis+8+index*13,textKey->ypis+4,2*13,30,Color_Black,Color_White);						
				}else{
					FillRectColorChang(textKey->xpis+8+index*13,textKey->ypis+4,13,30,Color_LBlue,Color_KeyBlue);	
					FillRectColorChang(textKey->xpis+8+index*13,textKey->ypis+4,13,30,Color_Black,Color_White);	
				}			
			}
		}

		
		if(disupdata8){
			disupdata8 =0;
		#if USEMULTCOLOR
			BmpFillRect(8,192,580,56);
			BmpFillRect(8,250,283,56);
		#else
			DrawFillRect(8,192,580,56,Color_Silver);
			DrawFillRect(8,250,283,56,Color_Silver);
		#endif
			if(UCase>2){

				DrawFillRect(keyd3[KEY_KEYBOARD3_SELPRES-KEY_KEYBOARD3_SEL1].xpis,keyd3[KEY_KEYBOARD3_SELPRES-KEY_KEYBOARD3_SEL1].ypis,keyd3[KEY_KEYBOARD3_SELPRES-KEY_KEYBOARD3_SEL1].xsize,keyd3[KEY_KEYBOARD3_SELPRES-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
				Draw24X24FStr(keyd3[KEY_KEYBOARD3_SELPRES-KEY_KEYBOARD3_SEL1].xpis+2,keyd3[KEY_KEYBOARD3_SELPRES-KEY_KEYBOARD3_SEL1].ypis+8,(char *)(pingyingbuf),Color_White);
				
				for(i=0;i<10;i++){
					DrawFillRect(keyd3[i].xpis,keyd3[i].ypis,keyd3[i].xsize,keyd3[i].ysize,Color_DarkSlateBlue);
					DrawFU32(keyd3[i].xpis+19,keyd3[i].ypis+32,i,1,Color_White);
					Draw24X24FPY(keyd3[i].xpis+10,keyd3[i].ypis+4, (char *)(&tmppysel[i]), 2 , Color_Green);					
				}

				DrawFillRect(keyd3[KEY_KEYBOARD3_SELPREV-KEY_KEYBOARD3_SEL1].xpis,keyd3[KEY_KEYBOARD3_SELPREV-KEY_KEYBOARD3_SEL1].ypis,keyd3[KEY_KEYBOARD3_SELPREV-KEY_KEYBOARD3_SEL1].xsize,keyd3[KEY_KEYBOARD3_SELPREV-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
				Draw24X24FStr(keyd3[KEY_KEYBOARD3_SELPREV-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[KEY_KEYBOARD3_SELPREV-KEY_KEYBOARD3_SEL1].ypis+8,"<-",Color_White);
				
				DrawFillRect(keyd3[KEY_KEYBOARD3_SELNEXT-KEY_KEYBOARD3_SEL1].xpis,keyd3[KEY_KEYBOARD3_SELNEXT-KEY_KEYBOARD3_SEL1].ypis,keyd3[KEY_KEYBOARD3_SELNEXT-KEY_KEYBOARD3_SEL1].xsize,keyd3[KEY_KEYBOARD3_SELNEXT-KEY_KEYBOARD3_SEL1].ysize,Color_DarkSlateBlue);
				Draw24X24FStr(keyd3[KEY_KEYBOARD3_SELNEXT-KEY_KEYBOARD3_SEL1].xpis+19,keyd3[KEY_KEYBOARD3_SELNEXT-KEY_KEYBOARD3_SEL1].ypis+8,"->",Color_White);
			}else{
				for(i=0;i<=KEY_KEYBOARD2_9-KEY_KEYBOARD2_0;i++){	
					DrawButton1(keyd2[i],Color_White1,i+758);			
				} 	
				DrawButton1(keyd2[KEY_KEYBOARD2_CHAT1-KEY_KEYBOARD2_0],Color_White1,798);
				DrawButton1(keyd2[KEY_KEYBOARD2_CHAT2-KEY_KEYBOARD2_0],Color_White1,799);				
			}		
		}
		
		
		if(pno != MENU_FTKEYBOARD){plastno = MENU_FTKEYBOARD; break;}
		wdt();
	}
}
#endif
//------------------------------------------
// Function   : void initFunctionBtn()		
// Description: ��ʼ������״̬			
//------------------------------------------
void initFunctionBtn(U8 clr)
{
	g_SelFileNo = 0xffff;
	g_btndisable = 0xdfc;  //����3/4/5/6/7/8/9/11/12����
	g_checkMode = 0;
	g_selALL = 0;
	if(g_clipboard && clr==0){
		g_btndisable = clrBit16(g_btndisable,(1<<7));//ʹ��8
	}else{
		g_clipboard = 0;
	}
		
	if(g_disk_memory!=MEM_IN && g_disk_memory!=MEM_DSP){
		g_btndisable = clrBit16(g_btndisable,(1<<2));//ʹ��3
	}	
}

//------------------------------------------
// Function   : void Bor_Menu_File(void)		
// Description: ��ʾ�ļ�·��			
//------------------------------------------
void showFilePath(U16 xpos,U16 ypos,U8 disk,char *path,U8 flag)
{
	char disPath[50] = {0};	//�����ʾ27���ַ�
	char root[8] ={0};	//��Ŀ¼
	U16 addr =0;
	U16 plen = getStrLen((U8*)path);
	U16 rlen = 0;
	U16 i;
	U8 zh =0;
	U8 vmax =28;
	if(flag==1) vmax = 32;
	switch(disk){
		case MEM_IN:rlen = 4;strcat(root,"RAM:");break;
		case MEM_USB:rlen = 7;strcat(root,"U-disk:");break;
		default:break;
	}
	if(plen+rlen <vmax){//��ʾ�̷���·��
		strcat(disPath,root);
		strcat(disPath,path);
	}else{	//�̷���.���,·������ʾ���32�ַ�
		setBufDat8((U8*)root,8,0);
		rlen = 1;
		root[0] = '.';
		if(plen+rlen <vmax){
			strcat(disPath,root);
			strcat(disPath,path);
		}else{
			addr = plen-(vmax-2);
			//�жϽ�ȡ��ʼ�ַ��Ƿ��ǰ������
			for(i=0;i<addr;i++){
				if(*(path+i) &0x80){
					if(zh==0) zh =1;
					else zh =0;
				}
			}
			if((*(path+addr)&0x80) && zh){	//��ʼ�ַ�Ϊ�������,����ʾ
				addr++;
			}						
			strcat(disPath,root);
			strcat(disPath,path+addr);
		}
	}
	if(flag==1)DrawFillRect(xpos,ypos,436,38,Color_White);
	else DrawFillRect(xpos,ypos,372,38,Color_White);

	Draw24X24FStr(xpos+5,ypos+10,disPath,Color_Black);
	wdt();	
	
}

//------------------------------------------
// Function   : U8 getPathStep(char* path)		
// Description: ��ȡĿ¼����			
//------------------------------------------
U8 getPathStep(char* path)
{
	U8 step = 0;
	U16 i =0;	
	while(*(path+i)!=0){
		if(*(path+i) == '/') step++;
		i++;
	}
	wdt();
	return step;
}

//------------------------------------------
// Function   : void setPathToLast(char* path)	
// Description: ����·��Ϊ��һ��·��		
//------------------------------------------
void setPathToLast(char* path)
{
	U16 tstep = getPathStep(path);
	U16 step = 0;
	U16 i = 0;
	while(*(path+i)!=0){
		if(*(path+i) == '/') step++;
		if(step==tstep){
			*(path+i) = 0;
			break;
		}
		i++;
	}
	wdt();
}

//------------------------------------------
// Function   : void copyPath(char *spath,char *dpath)		
// Description: ����·��	
//------------------------------------------
void copyPath(char *spath,char *dpath)
{
	U16 i =0;
	while(*(spath+i)!=0){
		*(dpath+i)= *(spath+i);
		i++;
	}
	*(dpath+i) = 0;
	wdt();
}

//------------------------------------------
// Function   : U8 getSavePathTimes(char* spath)		
// Description: ��ȡ����·������		
//------------------------------------------
U8 getSavePathTimes(char* spath)
{
	return (U8)*spath;
}

//------------------------------------------
// Function   : U8 getFolderName(char* spath)	
// Description: ��ȡ��ǰ�ļ�������	
//------------------------------------------
char* getFolderName(char* spath)
{
	char *p = spath;
	U16 i=0;
	while(*(spath+i)!=0){
		if(*(spath+i)=='/') p = spath+i+1; 
		i++;
	}
	return p;
}


//------------------------------------------
// Function   : void savePath(char* spath,U8 time,U8 disk)	
// Description: �����ļ�·��	
//------------------------------------------
void savePath(char* spath,char *path,U8 time,U8 disk)
{
	U16 addr =1;
	U8 t = 0;
	U16 i = 0;
//	printf("savePath time = %d \r\n",time);
	if(time ==0 )	return;
	*spath = time;
	
	//�ҵ���һ�α�������ݵ�ַ
	while((t<time-1) && (*(spath+addr)!=0)){
		if(*(spath+addr)=='>'){
			t++;
		}
		addr++;
	}
	spath[addr++] = '<';	//������ʼ��־
	spath[addr++] = time;	//�������
	spath[addr++] = disk;	//�����������
	while(*(path+i)!=0){
		spath[addr++] = *(path+i);	//�����ļ�·��
		i++;
	}
	spath[addr++] = '>';	//���������־		
}

//------------------------------------------
// Function   : char *getSavePath(char* spath,U8 time,U8 *disk)
// Description: ��ȡ�����ļ�·��	
//------------------------------------------
U8 getSavePath(char* spath,char *path,U8 time,U8 *disk)
{
	U8 t = 0;
	U16 addr =1;
	U16 i =0;
	//U8 tdisk =0;
//	printf("getSavePath time = %d \r\n",time);
	if(time==0){
		*path = 0;
		return 1;
	}
	//�ҵ���������ݵ�ַ
	while((t<time) && (*(spath+addr)!=0)){
		if(*(spath+addr)=='<'){
			t++;
		}
		addr++;
	}
	t = spath[addr++];
	if(time != t){
		return 0;
	}
	*disk = spath[addr++];
	while(spath[addr]!=0){	
		if(spath[addr]=='>')	break;
		*(path+i) = spath[addr];	//�����ļ�·��
		addr++;
		i++;
	}
	*(path+i) = 0;	
	
	wdt();
	return 1;
}

//-----��ȡ�ļ�������--------
U8 getFileNamaLen(U8 *name)
{
	U8 len = 0;
	while(*(name+len)) len++;
	if(len>64)	len = 0;	//�ļ������Ȳ��ܴ���64
	wdt();
	return len;
}


/*-----------------------------------------------
//������:void backupViewPage(U8 page)
//����  :������ʾҳ��
--------------------------------------*/
void backupViewPage(U8 page)
{
	U8 savePage;
	if(page==1)savePage = 2;else savePage = 1;
	PageWritern(savePage,page);		
	WritePage(savePage);
	ViewPage(savePage);
}

/*-----------------------------------------------
//������:void recoverViewPage(U8 page)
//����  :�ָ���ʾҳ��
--------------------------------------*/
void recoverViewPage(U8 page)
{
	WritePage(page);
	ViewPage(page);
}

