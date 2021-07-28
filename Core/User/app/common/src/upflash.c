#include "upflash.h"
#include "iap.h"
#include "lcddisp.h"
#include <string.h>	
#include "w25qxx.h"


extern void DrawProgressbar(U32 x,U32 y,U32 dx,U32 dy,U8 percent,U16 color,U16 bkcolor);
extern uint8_t ExtMemory_Status(uint8_t usbsd);
extern void Select_ExtMemory(uint8_t usbsd);
extern uint8_t Read_ProgramFile(uint8_t list,uint8_t num,uint32_t *sumbyte,PROGRESSBAR pgbar);
extern U8 asctonum(U8 asc);
extern void creatUpInfoData(U8 *outDat);
extern void wdt(void);

extern volatile U8 g_upinfo[0x20];
//------------------------------------------
// Function   : U8 update_flash(U8 selextm,U8 flag,U16 bkcolor)		
// Description: 从升级包中读取数据升级FLASH		bkcolor:进度条底色
//------------------------------------------
U8 update_flash(U8 flag,U16 bkcolor)
{
	U8  f=0;
	U32 i,j,n,m,l;
	U32 len,add,sdramadd,num;
	PROGRESSBAR pgbar;
	const U8 Check_Buff[] = {"GZSK_FLASH_V"};
	U8 Info[0x20];		
//	U32 iflashver;
//	U8 *buff;
	U8 selextm = USBTF;
	
	//printf(" update_flash flag = %d \r\n",flag);
	if(flag==1){
		pgbar.mode = 0x5a;
		pgbar.color = 0xf800;		// Color_Red
		pgbar.bkcolor = bkcolor;
		pgbar.px = 150;
		pgbar.py = 212;				
		pgbar.pdx = 322;				
		pgbar.pdy = 36;
		pgbar.str = 0;				
		pgbar.end = 0;	
		DrawProgressbar(pgbar.px,pgbar.py,pgbar.pdx,pgbar.pdy,pgbar.str,pgbar.color,pgbar.bkcolor);
	}else{
		pgbar.mode = 0;		
	}	
	if(selextm<3){				// selextm:0_USB 1_SD 2_TF
		if(ExtMemory_Status(selextm)!=0)return(FALSE);
		Select_ExtMemory(selextm);
		f = Read_ProgramFile(2,0,&len,pgbar);

	}

	if(f==0){																		// 打开升级包,判断是否有FLASH数据,只读数据长度,暂不读数据				
		if(strncmp((char *)(IAPDATA_ADD),(char*)Check_Buff,12)!=0)return(FALSE);		// 判断升级数据中标志位,如果标志位不对直接退出
		
//		buff = (U8 *)(IAPDATA_ADD);
//		iflashver = ((U16)asctonum(buff[12]))*100 + ((U16)asctonum(buff[13]/*>>8*/))*10 + asctonum(buff[14]);
		


		if(selextm<3){l = (flashcheck.hexlen + (DISPUSBRDLEN - 1)) / DISPUSBRDLEN;}	// 计算分几次读数据,每次可读8M Byte
		else{l = (flashcheck.hexlen + (MCPUUSBRDLEN - 1)) / MCPUUSBRDLEN;}			// 主板USB,每次发指令读160K,缓冲区最大可连续读50次
		if(l==0)return(FALSE);
		m = 100 / l;
		add = FLASHINDEXADDR;
		
		
		for(i=0;i<l;i++){
						
			pgbar.str = i*m;
			pgbar.end = pgbar.str + (m>>1);
				
			if(selextm<3){		// selextm:0_USB 1_SD 2_TF			
				f = Read_ProgramFile(2,(i+1),&len,pgbar);
			}	
			//printf("f = %d \r\n",f);			
			if(f!=0)return(FALSE);
			sdramadd = IAPDATA_ADD;
			pgbar.str = pgbar.end;
			pgbar.end += (m>>1);
			n = (len + (100*1024-1)) / (100*1024);				// 为了刷进度条,每次写100K Byte

			g_upinfo[FL_ADDR] = '1';	//标记需要升级flash
			creatUpInfoData(Info);
			W25QXX_Write(W25Q1_DAT,Info,W25QUPIFOADDR,0x20);
		
			//printf("pgbar.str = %d  pgbar.end = %d  n = %d flag = %d\r\n",pgbar.str,pgbar.end,n,flag);
			for(j=0;j<n;){
				if(len>(100*1024))num = 100*1024;else num = len;
				wdt();
				W25QXX_Write(W25Q1_DAT,(U8 *)(sdramadd),add,num);
			
				len -= num;
				add += num;
				sdramadd += num;
				j++;
				if(flag){
					num = (m>>1)*j/n;if(num>pgbar.end)num = pgbar.end;
					DrawProgressbar(pgbar.px,pgbar.py,pgbar.pdx,pgbar.pdy,pgbar.str+num,pgbar.color,pgbar.bkcolor);								
				}
			}
		}	
		if(flag){DrawProgressbar(pgbar.px,pgbar.py,pgbar.pdx,pgbar.pdy,100,pgbar.color,pgbar.bkcolor);}
		 	
		return(TRUE);
	}
	return(FALSE);
}


