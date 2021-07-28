//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Mit_funtion.c							*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年08月22日 							*
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



#include "Mit_funtion.h"
#include "function.h"
#include "utility.h"
#include "color.h"
#include "lcddisp.h"

U16 g_MyProgressMax1;
U16 g_background;
//------------------------------------------
// Function   : U16 getFiles(U16 *buf,U16 len)
// Description: 获取文件数量(数据索引值不为0xffff即为记录文件编号)			
//------------------------------------------
U16 getFiles(U16 *buf,U16 len)
{
	U16	selnums = 0,i;
	for(i=0;i<len;i++){
		if(buf[i]!=0xffff){
			selnums++;
		}
	}
	wdt();
	return 	selnums;
}

/*-----------------------------------------------
//函数名:U16 setBit16(U16 dat,U16 bit)
//功能  :将指定位置1
//------------------------------------------*/
U16 setBit16(U16 dat,U16 bit)
{
	U16 res = dat|bit;
	return res;
}

/*-----------------------------------------------
//函数名:U16 clrBit16(U16 dat,U16 bit)
//功能  :将指定位清0
//------------------------------------------*/
U16 clrBit16(U16 dat,U16 bit)
{
	U16 res = dat&(~bit);
	return res;
}

/*-----------------------------------------------
//函数名:U8 getNumDigits(U32 nums)
//功能  :获取十进制数值位数
--------------------------------------*/
U8 getNumDigits(U32 nums)
{
	if(nums>999999999){
		return 10;
	}else if(nums>99999999){
		return 9;
	}else if(nums>9999999){
		return 8;
	}else if(nums>999999){
		return 7;
	}else if(nums>99999){
		return 6;
	}else if(nums>9999){
		return 5;
	}else if(nums>999){
		return 4;
	}else if(nums>99){
		return 3;
	}else if(nums>9){
		return 2;
	}else{
		return 1;
	}		
}

///*-----------------------------------------------
////函数名:U8 isContain_U16(U16 *buff,U16 len,U16 dat)
////功能  :判断数组是否包含指定数值
//--------------------------------------*/
//U8 isContain_U16(U16 *buff,U16 len,U16 dat)
//{
//	U16 i;
//	wdt();
//	for(i=0;i<len;i++){
//		if(buff[i]==dat){
//			return 1;
//		}
//	}
//	return 0;
//}

/*-----------------------------------------------
//函数名:void setBufDat16(U16 *buf,U16 len,U16 dat)
//功能  :数组设定指定数值
//------------------------------------------*/
void setBufDat16(U16 *buf,U16 len,U16 dat)
{
	U16 i;
	for(i=0;i<len;i++){
		buf[i]=dat;
	}
	wdt();
}

/*-----------------------------------------------
//函数名:void setBufDat8(U8 *buf,U16 len,U8 dat)
//功能  :数组设定指定数值
//------------------------------------------*/
void setBufDat8(U8 *buf,U16 len,U8 dat)
{
	U16 i;
	for(i=0;i<len;i++){
		buf[i]=dat;
	}
	wdt();
}

//----------------------------------
void DisRange(U16 xpos,U16 ypos,U16 XRange,U16 YRange)
{
//	U16 x1=0,x2=0,x3=0;

	U8 lenx = getNumDigits(XRange/100);
	U8 leny = getNumDigits(YRange/100);

	DrawMFU32(xpos, ypos, XRange/100, lenx,Color_Black);
	Draw24X24FStr(xpos+lenx*13,ypos,".",Color_Black);
	DrawMFU32(xpos+(lenx+1)*13, ypos, XRange%100, 2,Color_Black);
	xpos +=4;
	Draw24X24FStr(xpos+(lenx+1+2)*13,ypos,"X",Color_Black);	
	xpos +=4;
	
	DrawMFU32(xpos+(lenx+1+2+1)*13, ypos, YRange/100, leny,Color_Black);
	Draw24X24FStr(xpos+(lenx+1+2+1+leny)*13,ypos,".",Color_Black);
	DrawMFU32(xpos+(lenx+1+2+1+leny+1)*13, ypos, YRange%100, 2,Color_Black);
	Draw24X24FStr(xpos+(lenx+1+2+1+leny+1+2)*13,ypos,"(mm)",Color_Black);
		
}

/*-----------------------------------------------
//函数名:void removeStrByIndex8(U8 *buf,U16 bufsize,U16 index)
//功能  :删除字符串指定索引数据
//------------------------------------------*/
void removeStrByIndex(U8 *buf,U16 bufsize,U16 index)
{
	U16 i;
	U8 zh =0;	//中文
	if(index>=bufsize) return;
	if(buf[index]&0x80 && buf[index+1]&0x80){
		if(index+1<bufsize){
			zh = 1;
		}	
	}
	if(zh){	//中文
		if(index==bufsize-2){
			buf[index] = 0;
			buf[index+1] = 0;
		}else{
			for(i=index;i<bufsize-2;i+=2){
				buf[i] = buf[i+2];
				buf[i+1] = buf[i+3];
			}			
		}
	}else{
		if(index==bufsize-1){
			buf[index] = 0;
		}else{
			for(i=index;i<bufsize-1;i++){
				buf[i] = buf[i+1];
			}					
		} 		
	}
	wdt();
}

/*-----------------------------------------------
//函数名:int getcharIndexOnstr(char *str,char c,U16 stIndex)
//功能  :从指定位置获取指定字符在字符串第一次出现的索引
//------------------------------------------*/
int getCharIndexOnstr(char *str,char c,U16 stIndex)
{
	int i =0;
	while(*(str+i+stIndex)!=0){
		if(*(str+i+stIndex)==c){
			return i+stIndex;
		}
		i++;
	}
	wdt();
	return -1; 
}


/*-----------------------------------------------
//函数名:int getCharLastIndexOnstr(char *str,char c)
//功能  :获取指定字符在字符串最后一次出现的索引
//------------------------------------------*/
int getCharLastIndexOnstr(char *str,char c)
{
	int i =0;
	int index = -1;
	while(*(str+i)!=0){
		if(*(str+i)==c){
			index = i;
		}
		i++;
	}
	wdt();
	return index; 
}

/*-----------------------------------------------
//函数名:U8 isContainFileByIREG(U16 fileno)
//功能  :判断是否包含指定编号的文件
//------------------------------------------*/
U8 isContainFileByIREG(U16* ireg,U16 fileno)
{
	U32 addr = (fileno-MINFILENO)*FileIndexSize;
	if(fileno<MINFILENO || fileno>MAXFILENO) return 0;	
	if(ireg[addr]==0xffff && ireg[addr+1]==0xffff )
		return 0;
	else return 1;
}

//------------------------------------------
// Function   : U16 getFiles(U16 *buf,U16 len)
// Description: 获取文件数量(数据索引值不为0xffff即为记录文件编号)			
//------------------------------------------
U16 getFilesNo(U16 *buf,U16 len,U16 Index)
{
	U16	nums = 0,i;
	wdt();
	for(i=0;i<len;i++){
		if(buf[i]!=0xffff){
			if(nums==Index){
				return buf[i];
			}
			nums++;
		}
	}
	return 	0;
}

//------------------------------------------
// Function   : U16 getFilesNo1(U16 *buf,U16 inno)
// Description: 获取磁盘文件编号		
//------------------------------------------
U16 getFilesNo1(U16 *buf,U16 inno)
{
	U8 *p8,nlen =0,ftype;
	U8 name[64] = {0};
	U16 no;
	p8 = getFileNameByIREG(buf,inno);
	ftype = getFileTypeByIREG(buf,inno);		
	for(nlen=0;nlen<63;nlen++){
		if(*(p8+nlen)==0)	break;
		name[nlen] = *(p8+nlen);
	}
	wdt();
	if(nlen<4 || name[nlen-4]!='.') return 	inno;
	
	if(ftype==FILE_SEW || ftype==FILE_EMB){
		if((nlen==12)&&((name[5]>='0')&&(name[5]<='9'))&&((name[6]>='0')&&(name[6]<='9'))&&((name[7]>='0')&&(name[7]<='9'))){
			no = (name[5] - '0') * 100 + (name[6] - '0') * 10 + (name[7] - '0');
			if(no>=MINFILENO&&no<=MAXFILENO){
				return no;
			}
		}
	}else if(ftype==FILE_MITE || ftype==FILE_MITG){		
		if((name[nlen-3]>='0')&&(name[nlen-3]<='9')&&(name[nlen-2]>='0')&&(name[nlen-2]<='9')&&(name[nlen-1]>='0')&&(name[nlen-1]<='9')){
			no = (name[nlen-3] - '0') * 100 + (name[nlen-2] - '0') * 10 + (name[nlen-1] - '0');	
			if(no>=MINFILENO&&no<=MAXFILENO){
				return no;
			}		
		}
	}
//	#if Debug
//	printf("getFilesNo1 ftype = %d name = %s \r\n",ftype,name);
//	#endif
	
	return 	inno;
}
/*-----------------------------------------------
//函数名:U8* getFileNameByIREG(U16 fileno)
//功能  :获取文件名称
//------------------------------------------*/
U8* getFileNameByIREG(U16* ireg,U16 fileno)
{
//	U8 type = 0;
	U32 addr = (fileno-MINFILENO)*FileIndexSize;

	return (U8 *)(&ireg[addr+FileIndexNamepox]);
}

/*-----------------------------------------------
//函数名:void initMyProgress(U16 maxval)
//功能  :初始化进度条1
//------------------------------------------*/
void initMyProgress1(U16 maxval,U16 background)
{
#if USEMULTCOLOR	//彩屏
	const U16 color = Color_Red;
#else
	const U16 color = Color_SkyBlue;
#endif
	
	g_MyProgressMax1 = maxval;
	g_background = background;

	DrawRect(187,230,506,48,color);
	DrawRect(188,231,504,46,color);	

}
/*-----------------------------------------------
//函数名:void SetMyProgress1(U16 val)
//功能  :设置进度条1
//------------------------------------------*/
void SetMyProgress1(U16 val)
{
	U16 dx;
#if USEMULTCOLOR	//彩屏
	const U16 color = Color_Red;
#else
	const U16 color = Color_SkyBlue;
#endif
	
	if(val==g_MyProgressMax1){
		DrawFillRect(190,233,500,42,color);
	}else{
		dx =420*val/g_MyProgressMax1;
		DrawFillRect(190,233,dx,42,color);
	}	
	wdt();
}


