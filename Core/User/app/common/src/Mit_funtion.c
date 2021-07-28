//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F767IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  Mit_funtion.c							*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��08��22�� 							*
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



#include "Mit_funtion.h"
#include "function.h"
#include "utility.h"
#include "color.h"
#include "lcddisp.h"

U16 g_MyProgressMax1;
U16 g_background;
//------------------------------------------
// Function   : U16 getFiles(U16 *buf,U16 len)
// Description: ��ȡ�ļ�����(��������ֵ��Ϊ0xffff��Ϊ��¼�ļ����)			
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
//������:U16 setBit16(U16 dat,U16 bit)
//����  :��ָ��λ��1
//------------------------------------------*/
U16 setBit16(U16 dat,U16 bit)
{
	U16 res = dat|bit;
	return res;
}

/*-----------------------------------------------
//������:U16 clrBit16(U16 dat,U16 bit)
//����  :��ָ��λ��0
//------------------------------------------*/
U16 clrBit16(U16 dat,U16 bit)
{
	U16 res = dat&(~bit);
	return res;
}

/*-----------------------------------------------
//������:U8 getNumDigits(U32 nums)
//����  :��ȡʮ������ֵλ��
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
////������:U8 isContain_U16(U16 *buff,U16 len,U16 dat)
////����  :�ж������Ƿ����ָ����ֵ
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
//������:void setBufDat16(U16 *buf,U16 len,U16 dat)
//����  :�����趨ָ����ֵ
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
//������:void setBufDat8(U8 *buf,U16 len,U8 dat)
//����  :�����趨ָ����ֵ
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
//������:void removeStrByIndex8(U8 *buf,U16 bufsize,U16 index)
//����  :ɾ���ַ���ָ����������
//------------------------------------------*/
void removeStrByIndex(U8 *buf,U16 bufsize,U16 index)
{
	U16 i;
	U8 zh =0;	//����
	if(index>=bufsize) return;
	if(buf[index]&0x80 && buf[index+1]&0x80){
		if(index+1<bufsize){
			zh = 1;
		}	
	}
	if(zh){	//����
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
//������:int getcharIndexOnstr(char *str,char c,U16 stIndex)
//����  :��ָ��λ�û�ȡָ���ַ����ַ�����һ�γ��ֵ�����
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
//������:int getCharLastIndexOnstr(char *str,char c)
//����  :��ȡָ���ַ����ַ������һ�γ��ֵ�����
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
//������:U8 isContainFileByIREG(U16 fileno)
//����  :�ж��Ƿ����ָ����ŵ��ļ�
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
// Description: ��ȡ�ļ�����(��������ֵ��Ϊ0xffff��Ϊ��¼�ļ����)			
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
// Description: ��ȡ�����ļ����		
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
//������:U8* getFileNameByIREG(U16 fileno)
//����  :��ȡ�ļ�����
//------------------------------------------*/
U8* getFileNameByIREG(U16* ireg,U16 fileno)
{
//	U8 type = 0;
	U32 addr = (fileno-MINFILENO)*FileIndexSize;

	return (U8 *)(&ireg[addr+FileIndexNamepox]);
}

/*-----------------------------------------------
//������:void initMyProgress(U16 maxval)
//����  :��ʼ��������1
//------------------------------------------*/
void initMyProgress1(U16 maxval,U16 background)
{
#if USEMULTCOLOR	//����
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
//������:void SetMyProgress1(U16 val)
//����  :���ý�����1
//------------------------------------------*/
void SetMyProgress1(U16 val)
{
	U16 dx;
#if USEMULTCOLOR	//����
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


