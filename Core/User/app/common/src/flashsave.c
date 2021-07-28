//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  flashsave.c								*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision4 V4.21					*
//*  编程													*
//*															*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#include "flashsave.h"
#include "register.h" 
#include "w25qxx.h"
#include "utility.h"

//volatile U16 SYS_PTINDEXF;
extern volatile U16 buf[2048];

volatile U16 SYS_TMPUSEC =0;	//暂存flash花样数据区循环使用的SEC号
volatile U16 SYS_PTINDEXF; //0-不读取flash中emb文件索引

//------------------------------------------------------
// Function   : nextsec()
// Description: 返回下一个SEC
//------------------------------------------------------
U32 nextsec(U32 sec, U32 sno, U32 fno)
{
	U32 page, p, rf, rs;
	U32 nsec=0;

	if(sec<FAT_FILE) return(FALSE);	// SEC要大于数据区
	page = sec/512;									// SEC*4/2048
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_SEC+page);// 读入SEC page
	p = (sec%512)*4;								// 定位SEC的位置
	rf = buf[p];									// 文件编号
	rs = buf[p+1];									// SEC顺序号
	if((rf==fno)&&(rs==sno)){						// check
		nsec = buf[p+3];							// 下一个SEC
	}
	return(nsec);	
} 

//------------------------------------------------------
// Function   : findsec()
// Description: 搜索新的SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 findsec(U32 sec0)
{
	U32 oldpage, page, p;
	U16 rf;
	U32 j, sec, nsec=0;

	if(sec0<FAT_FILE){sec = FAT_FILE;sec0 = FAT_FILE;}	// 数据区第一个SEC
	else if(sec0>=FAT_TOTAL){sec = FAT_FILE;sec0 = FAT_FILE;}
	else  sec =  sec0;
	oldpage = 0xffff;
	j = FAT_FILE;
	while(j<FAT_TOTAL){
		page = sec/512;						// SEC*4/2048
		if(oldpage!=page){W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_SEC+page);}// 读入SEC page
		oldpage = page;
		p = (sec%512)*4;					// 定位SEC的位置
		rf = buf[p];						// 文件编号
		if(rf==0xffff){ nsec = sec; break;}	// 空的
		sec++;
		//if(sec>=FAT_TOTAL) { nsec = 0xffff; break;}	// 满了
		if(sec>=FAT_TOTAL) { sec = FAT_FILE;}	// 满了
		j++;
	}
	return(nsec);
}	

//------------------------------------------------------
// Function   : sevesec()
// Description: 保存SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 sevesec(U32 sec, U32 psec, U32 nsec, U32 secno, U32 fno)
{
	U32 page, p;

	if((sec<FAT_FILE)||(sec>=FAT_TOTAL)) return(FALSE);	// SEC要大于数据区
	page = sec/512;									// SEC*4/2048
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf, FAT_SEC+page);	// 读入SEC page
	p = (sec%512)*4;								// 定位SEC的位置
	buf[p+0]=fno;									// 文件编号
	buf[p+1]=secno;									// SEC顺序号
	buf[p+2]=psec;									// 上一个SEC
	buf[p+3]=nsec; 									// 下一个SEC
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf, FAT_SEC+page);	// 写入SEC page
	return(TRUE);	
}  

//------------------------------------------------------
// Function   : delsec()
// Description: 删除SEC的一个fno
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 delsec(U32 sec)
{
	U32 i, page, p;
	U32 nsec=0;

	if((sec<FAT_FILE)||(sec>=FAT_TOTAL)) return(FALSE);		// SEC要大于数据区
	page = sec/512;										// SEC*4/2048
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_SEC+page);	// 读入SEC page
	p = (sec%512)*4;									// 定位SEC的位置
	nsec = buf[p+3];									// 下一个SEC
	for(i=0;i<4;i++) buf[p+i]=0xffff;					// 清空
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf, FAT_SEC+page);		// 写入SEC page
	return(nsec);	
} 

//------------------------------------------------------
// Function   : read_stsec()
// Description: 读一个花样的开始SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 read_stsec(U32 fno)
{
	U32 sec, pno, page, p;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);		//保存位置大于600或小于920
	pno = fno-MINFILENO;									// pno = 0 - 320
	page = pno/2048;											// page = 0 - 1
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_STSEC+page);		// 读入SEC page
	p = pno%2048;
	sec = buf[p];											// 数据区第一个SEC
	return(sec);	
} 

//------------------------------------------------------
// Function   : save_stsec()
// Description: 保存一个花样的开始SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 save_stsec(U32 ssec, U32 fno)
{
	U32 pno, page, p;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);		// 保存位置大于600或小于920
	pno = fno-MINFILENO;
	page=pno/2048;
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_STSEC+page);	// 读入SEC page
	p = pno%2048;
	buf[p] = ssec;
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf,FAT_STSEC+page);	// 写入SEC page
	return(TRUE);	
}  

//------------------------------------------------------
// Function   : del_stsec()
// Description: 删除一个花样的开始SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 del_stsec(U32 fno)
{
	U32 pno, page, p;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);		// 保存位置大于600或小于920
	pno = fno-MINFILENO;
	page = pno/2048;
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf, FAT_STSEC+page);	// 读入SEC page
	p = pno%2048;
	buf[p] = 0xffff;
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf, FAT_STSEC+page);	// 写入SEC page
	return(TRUE);	
}  


//------------------------------------------------------
// Function   : readFlashIndex()
// Description: FLASH的FAT文件表读到RAM的PT_INDEX中(读索引表)
// Input	  :	
// Output	  :
//------------------------------------------------------
void readFlashIndex(U16 *ireg)
{
	U32 i, j, p =0;
	U32 n;

	for(i=0;i<MAXIREG2;i++) ireg[i] = 0xffff;
	wdt();
	j = FAT_TABLE;
	n = PTINDEXREDLEN*SUMFILENO;
	while(j<FAT_STSEC){
		W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf, j);
		j ++;
		for(i=0; i<2048; i++){
			ireg[p]=buf[i];	
			p++;
			if(p>=n)break;
		}
		if(p>=n)break;
	}
	SYS_PTINDEXF = 0;
}		

//------------------------------------------------------
// Function   : writeFlashIndex()
// Description: RAM的PT_INDEX写入FLASH的FAT_TABLE文件表中(写索引表)
// Input	  :	
// Output	  :
//------------------------------------------------------
void writeFlashIndex(U16 *ireg)
{
	U32  f =0;
	U32 i, j, p =0;
	U32 n;
	U16 tmpbuf[2048];
	j = FAT_TABLE;
	n = PTINDEXREDLEN*SUMFILENO;
	while(j<FAT_STSEC){
		f = 1;
		W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)tmpbuf, j);
		for(i=0; i<2048; i++){		  	
			buf[i] = ireg[p];		  	
			if(buf[i]!=tmpbuf[i])f = 0;
			p ++;
			if(p>=n)break;
		}
		if(f==0){W25QXX_Write_One_Sector(W25Q3_DAT,(U8 *)buf, j);}
		j ++;
		if(p>=n)break;
	}
}	


//------------------------------------------------------
// Function   : PT_DregToFlash()
// Description: PT SRAM数据写入FLASH(保存花样)
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 PT_SaveToFlash(U32 fno,U16 *sreg,U16* ireg)
{
	U32 i, j, p;
	U32 ind, pno, len;
	U32 ssec, sec, psec, nsec, secno;
	U8 name[14] = "ISMS0001.EMB";
	U8 *pname;

//#if Debug
//	printf("PT_SaveToFlash fno = %d SYS_PTINDEXF = %d \r\n",fno,SYS_PTINDEXF);
//#endif	
	if((fno<MINFILENO)||(fno>MAXFILENO)){return(FALSE);}	// 保存位置大于600或小于920		//Beep_On(500);
	if(SYS_PTINDEXF)readFlashIndex(ireg);					// 更新PT_INDEX
	PT_Del(2, fno,ireg);											// 只删除fno的flash	 
	pno = fno-MINFILENO;									// 求偏移值
	ssec = findsec(SYS_TMPUSEC);							// 检索空的SEC		
	if(ssec<FAT_FILE){return(FALSE);}		// SEC要大于数据区
	
	//---文件索引部分 

	ind = pno*PTINDEXREDLEN;					// 检索表定位
	name[5] = fno/100+'0';
	name[6] = ((fno/10)%10)+'0';
	name[7] = (fno%10)+'0';
	
	ireg[ind]   = fno;	
	ireg[ind+PTINDEXSECADD]    = ssec;						// 开始SEC
	ireg[ind+PTINDEXLENADD+0]  = F_FILELENL;
	ireg[ind+PTINDEXLENADD+1]  = F_FILELENH;
	ireg[ind+PTINDEXNAMEADD+31] &= 0x0300;	//emb
	//	printf("ind+PTINDEXLENADD+31 = %d \r\n",ind+PTINDEXLENADD+31);
	//ireg[ind+PTINDEXLENADD+31] |= (FILE_EMB<<8);
	pname = (U8*)&ireg[ind+PTINDEXNAMEADD];	
	for(i=0;i<14;i++){
		pname[i] = name[i];
	}
	len = ireg[ind+PTINDEXLENADD+0]+(((U32)ireg[ind+PTINDEXLENADD+1])<<16);		// 文件长
	//printf("len = %d F_FILELENL = %04x  F_FILELENH = %04x \r\n",len,F_FILELENL,F_FILELENH);
	//for(i=0;i<len/2;i++){
	//	printf("sreg[%d] = %04x \r\n",i,sreg[i]);
	//}
	j=0; p=0;
	psec=0; sec = ssec, secno=0;

	while(j<len){
		//UART3_SendByte(sec>>8);UART3_SendByte(sec);
		//printf("sec=%d \r\n",sec);
		//DelayMs(1);
		if(sec<FAT_FILE){return(FALSE);	}// SEC要大于数据区	UART2_SendByte(3);UART2_SendByte(l);
		if(sec>=FAT_TOTAL){return(FALSE);	}
		for(i=0; i<2048; i++){
			buf[i]=sreg[p];
			j += 2;
			if(j>=len)break;
			p++;
		}
		for(i=i+2;i<2048;i++)buf[i]=0xffff;
		W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf,sec);
		secno++;
		SYS_TMPUSEC = sec + 1;
		if(j<len) nsec = findsec(sec+1);
		else nsec = 0xffff;
		sevesec(sec, psec, nsec, secno, fno);
		psec = sec;
		sec = nsec;
		wdt();
	}
	//for(;p<(DREG_PT_DATA+DREG_PTDATALEN);p++)DREG[p]=0;
	save_stsec(ssec, fno);				
	writeFlashIndex(ireg);	// 保存新的PT_INDEX
	SYS_PTINDEXF = 0;
	return(TRUE);
}
		
//------------------------------------------------------
// Function   : U32 PT_ReadFromFlash(U32 fno)
// Description: PT FLASH数据读入SRAM(读取花样)
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 PT_ReadFromFlash(U32 fno,U16 *sreg,U16 *ireg)
{
	U32 i, j, p;
	U32 ind, pno, len;
	U32 sec, secno;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);	// 保存位置大于600或小于920
	if(SYS_PTINDEXF)readFlashIndex(ireg);				// 更新PT_INDEX
	pno = fno-MINFILENO;								// 求偏移值
	ind = pno*PTINDEXREDLEN;				// 检索表定位
	if(ireg[ind+PTINDEXNOADD]==0xffff) return(FALSE);	// 保存数据是空的
	len = ireg[ind+PTINDEXLENADD+0]+(((U32)ireg[ind+PTINDEXLENADD+1])<<16);
	//printf("PT_ReadFromFlash len = %d \r\n",len);
	sec = read_stsec(fno);								// 开始SEC
	j=0; secno=0; p=0; 
	while(j<len){
		if(sec<FAT_FILE) return(FALSE);	// SEC要大于数据区
		if(sec>=(FAT_TOTAL)){return(FALSE);}
		W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf, sec);
		for(i=0; i<2048; i++){
			sreg[p]=buf[i];
			p++;j+=2;
			if(j>=len)break;
		}
		secno++;
		sec = nextsec(sec,secno, fno);
		wdt();
	}
//	printf("p = %d ",p);
//	for(i=0;i<p;i++){
//		printf("sreg[%d] = %04x \r\n",i,sreg[i]);
//	}
	for(;p<(MAXSREG);p++)sreg[p]=0;	
	SYS_PTINDEXF = 0;
	return(TRUE);
}

//------------------------------------------------------
// Function   : PT_Del()
// Description: 删除一个花样数据 
// Input	  :	t=1 dreg, t=2 flash, t=3 all
// Output	  :
//------------------------------------------------------
U32 PT_Del(U32 t, U32 fno,U16 *ireg)
{
	U32  j,i;
	U32  len;
	U32 sec, nsec;
	U32 ind, pno;
	
	if((fno<MINFILENO)||(fno>MAXFILENO)){return(FALSE);}	// 保存位置大于600或小于920
	pno = fno-MINFILENO;									// 求偏移值
	ind = pno*PTINDEXREDLEN;				// 检索表定位
	
	if(t&0x02){									// 删除FLASH部分
		sec = read_stsec(fno);					// 开始SEC
		j = FAT_FILE;
		len = FAT_TOTAL;						// 总扇区数
		while(j<len){
			if(sec<FAT_FILE || sec>=FAT_TOTAL){return(FALSE);}	// SEC大于数据区
//			printf("sec=%d \r\n",sec);
			nsec=delsec(sec);					// 删除一个SEC返回下一个SEC
			if(nsec==0xffff) break;				// 如果结束退出
			else sec=nsec;						// 否则继续
		}
		del_stsec(fno);
	}
	
	if(t&0x01){
		for(i=0;i<PTINDEXREDLEN;i++) ireg[ind+i]=0xffff;
		writeFlashIndex(ireg);		
	}
	return(TRUE);
}  


void teststsec(void)
{
//	U32 i;
//	U8 buf[4096] = {11,12,13,14,15,16,17,18,19,20}; 
//	W25QXX_Read(W25Q3_DAT,buf,0,4096); 
//	for(i=0;i<4096;i++){
//		//printf("%02x ",buf[i]);	
//		if((i+1)%16==0) printf("\r\n");
//	}
}
//void testsec(void)
//{
//	U32 i,j,n;
//	
//	n = (FAT_FILE - FAT_SEC) >> FAT_SECSIZE;
//	for(j=0;j<n;j++){
//		Read_One_Sector(buf, (FAT_SEC>>FAT_SECSIZE)+j);
//		for(i=0;i<2048;i++){
//			UART3_SendByte(buf[i]>>8);UART3_SendByte(buf[i]);	
//		}
//	}
//}




		
