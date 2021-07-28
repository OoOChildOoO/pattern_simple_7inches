//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F207VG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  flashsave.c								*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.21					*
//*  ���													*
//*															*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*

#include "flashsave.h"
#include "register.h" 
#include "w25qxx.h"
#include "utility.h"

//volatile U16 SYS_PTINDEXF;
extern volatile U16 buf[2048];

volatile U16 SYS_TMPUSEC =0;	//�ݴ�flash����������ѭ��ʹ�õ�SEC��
volatile U16 SYS_PTINDEXF; //0-����ȡflash��emb�ļ�����

//------------------------------------------------------
// Function   : nextsec()
// Description: ������һ��SEC
//------------------------------------------------------
U32 nextsec(U32 sec, U32 sno, U32 fno)
{
	U32 page, p, rf, rs;
	U32 nsec=0;

	if(sec<FAT_FILE) return(FALSE);	// SECҪ����������
	page = sec/512;									// SEC*4/2048
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_SEC+page);// ����SEC page
	p = (sec%512)*4;								// ��λSEC��λ��
	rf = buf[p];									// �ļ����
	rs = buf[p+1];									// SEC˳���
	if((rf==fno)&&(rs==sno)){						// check
		nsec = buf[p+3];							// ��һ��SEC
	}
	return(nsec);	
} 

//------------------------------------------------------
// Function   : findsec()
// Description: �����µ�SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 findsec(U32 sec0)
{
	U32 oldpage, page, p;
	U16 rf;
	U32 j, sec, nsec=0;

	if(sec0<FAT_FILE){sec = FAT_FILE;sec0 = FAT_FILE;}	// ��������һ��SEC
	else if(sec0>=FAT_TOTAL){sec = FAT_FILE;sec0 = FAT_FILE;}
	else  sec =  sec0;
	oldpage = 0xffff;
	j = FAT_FILE;
	while(j<FAT_TOTAL){
		page = sec/512;						// SEC*4/2048
		if(oldpage!=page){W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_SEC+page);}// ����SEC page
		oldpage = page;
		p = (sec%512)*4;					// ��λSEC��λ��
		rf = buf[p];						// �ļ����
		if(rf==0xffff){ nsec = sec; break;}	// �յ�
		sec++;
		//if(sec>=FAT_TOTAL) { nsec = 0xffff; break;}	// ����
		if(sec>=FAT_TOTAL) { sec = FAT_FILE;}	// ����
		j++;
	}
	return(nsec);
}	

//------------------------------------------------------
// Function   : sevesec()
// Description: ����SEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 sevesec(U32 sec, U32 psec, U32 nsec, U32 secno, U32 fno)
{
	U32 page, p;

	if((sec<FAT_FILE)||(sec>=FAT_TOTAL)) return(FALSE);	// SECҪ����������
	page = sec/512;									// SEC*4/2048
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf, FAT_SEC+page);	// ����SEC page
	p = (sec%512)*4;								// ��λSEC��λ��
	buf[p+0]=fno;									// �ļ����
	buf[p+1]=secno;									// SEC˳���
	buf[p+2]=psec;									// ��һ��SEC
	buf[p+3]=nsec; 									// ��һ��SEC
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf, FAT_SEC+page);	// д��SEC page
	return(TRUE);	
}  

//------------------------------------------------------
// Function   : delsec()
// Description: ɾ��SEC��һ��fno
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 delsec(U32 sec)
{
	U32 i, page, p;
	U32 nsec=0;

	if((sec<FAT_FILE)||(sec>=FAT_TOTAL)) return(FALSE);		// SECҪ����������
	page = sec/512;										// SEC*4/2048
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_SEC+page);	// ����SEC page
	p = (sec%512)*4;									// ��λSEC��λ��
	nsec = buf[p+3];									// ��һ��SEC
	for(i=0;i<4;i++) buf[p+i]=0xffff;					// ���
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf, FAT_SEC+page);		// д��SEC page
	return(nsec);	
} 

//------------------------------------------------------
// Function   : read_stsec()
// Description: ��һ�������Ŀ�ʼSEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 read_stsec(U32 fno)
{
	U32 sec, pno, page, p;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);		//����λ�ô���600��С��920
	pno = fno-MINFILENO;									// pno = 0 - 320
	page = pno/2048;											// page = 0 - 1
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_STSEC+page);		// ����SEC page
	p = pno%2048;
	sec = buf[p];											// ��������һ��SEC
	return(sec);	
} 

//------------------------------------------------------
// Function   : save_stsec()
// Description: ����һ�������Ŀ�ʼSEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 save_stsec(U32 ssec, U32 fno)
{
	U32 pno, page, p;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);		// ����λ�ô���600��С��920
	pno = fno-MINFILENO;
	page=pno/2048;
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf,FAT_STSEC+page);	// ����SEC page
	p = pno%2048;
	buf[p] = ssec;
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf,FAT_STSEC+page);	// д��SEC page
	return(TRUE);	
}  

//------------------------------------------------------
// Function   : del_stsec()
// Description: ɾ��һ�������Ŀ�ʼSEC
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 del_stsec(U32 fno)
{
	U32 pno, page, p;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);		// ����λ�ô���600��С��920
	pno = fno-MINFILENO;
	page = pno/2048;
	W25QXX_Read_One_Sector(W25Q3_DAT,(U8*)buf, FAT_STSEC+page);	// ����SEC page
	p = pno%2048;
	buf[p] = 0xffff;
	W25QXX_Write_One_Sector(W25Q3_DAT,(U8*)buf, FAT_STSEC+page);	// д��SEC page
	return(TRUE);	
}  


//------------------------------------------------------
// Function   : readFlashIndex()
// Description: FLASH��FAT�ļ������RAM��PT_INDEX��(��������)
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
// Description: RAM��PT_INDEXд��FLASH��FAT_TABLE�ļ�����(д������)
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
// Description: PT SRAM����д��FLASH(���滨��)
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
	if((fno<MINFILENO)||(fno>MAXFILENO)){return(FALSE);}	// ����λ�ô���600��С��920		//Beep_On(500);
	if(SYS_PTINDEXF)readFlashIndex(ireg);					// ����PT_INDEX
	PT_Del(2, fno,ireg);											// ֻɾ��fno��flash	 
	pno = fno-MINFILENO;									// ��ƫ��ֵ
	ssec = findsec(SYS_TMPUSEC);							// �����յ�SEC		
	if(ssec<FAT_FILE){return(FALSE);}		// SECҪ����������
	
	//---�ļ��������� 

	ind = pno*PTINDEXREDLEN;					// ������λ
	name[5] = fno/100+'0';
	name[6] = ((fno/10)%10)+'0';
	name[7] = (fno%10)+'0';
	
	ireg[ind]   = fno;	
	ireg[ind+PTINDEXSECADD]    = ssec;						// ��ʼSEC
	ireg[ind+PTINDEXLENADD+0]  = F_FILELENL;
	ireg[ind+PTINDEXLENADD+1]  = F_FILELENH;
	ireg[ind+PTINDEXNAMEADD+31] &= 0x0300;	//emb
	//	printf("ind+PTINDEXLENADD+31 = %d \r\n",ind+PTINDEXLENADD+31);
	//ireg[ind+PTINDEXLENADD+31] |= (FILE_EMB<<8);
	pname = (U8*)&ireg[ind+PTINDEXNAMEADD];	
	for(i=0;i<14;i++){
		pname[i] = name[i];
	}
	len = ireg[ind+PTINDEXLENADD+0]+(((U32)ireg[ind+PTINDEXLENADD+1])<<16);		// �ļ���
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
		if(sec<FAT_FILE){return(FALSE);	}// SECҪ����������	UART2_SendByte(3);UART2_SendByte(l);
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
	writeFlashIndex(ireg);	// �����µ�PT_INDEX
	SYS_PTINDEXF = 0;
	return(TRUE);
}
		
//------------------------------------------------------
// Function   : U32 PT_ReadFromFlash(U32 fno)
// Description: PT FLASH���ݶ���SRAM(��ȡ����)
// Input	  :	
// Output	  :
//------------------------------------------------------
U32 PT_ReadFromFlash(U32 fno,U16 *sreg,U16 *ireg)
{
	U32 i, j, p;
	U32 ind, pno, len;
	U32 sec, secno;

	if((fno<MINFILENO)||(fno>MAXFILENO)) return(FALSE);	// ����λ�ô���600��С��920
	if(SYS_PTINDEXF)readFlashIndex(ireg);				// ����PT_INDEX
	pno = fno-MINFILENO;								// ��ƫ��ֵ
	ind = pno*PTINDEXREDLEN;				// ������λ
	if(ireg[ind+PTINDEXNOADD]==0xffff) return(FALSE);	// ���������ǿյ�
	len = ireg[ind+PTINDEXLENADD+0]+(((U32)ireg[ind+PTINDEXLENADD+1])<<16);
	//printf("PT_ReadFromFlash len = %d \r\n",len);
	sec = read_stsec(fno);								// ��ʼSEC
	j=0; secno=0; p=0; 
	while(j<len){
		if(sec<FAT_FILE) return(FALSE);	// SECҪ����������
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
// Description: ɾ��һ���������� 
// Input	  :	t=1 dreg, t=2 flash, t=3 all
// Output	  :
//------------------------------------------------------
U32 PT_Del(U32 t, U32 fno,U16 *ireg)
{
	U32  j,i;
	U32  len;
	U32 sec, nsec;
	U32 ind, pno;
	
	if((fno<MINFILENO)||(fno>MAXFILENO)){return(FALSE);}	// ����λ�ô���600��С��920
	pno = fno-MINFILENO;									// ��ƫ��ֵ
	ind = pno*PTINDEXREDLEN;				// ������λ
	
	if(t&0x02){									// ɾ��FLASH����
		sec = read_stsec(fno);					// ��ʼSEC
		j = FAT_FILE;
		len = FAT_TOTAL;						// ��������
		while(j<len){
			if(sec<FAT_FILE || sec>=FAT_TOTAL){return(FALSE);}	// SEC����������
//			printf("sec=%d \r\n",sec);
			nsec=delsec(sec);					// ɾ��һ��SEC������һ��SEC
			if(nsec==0xffff) break;				// ��������˳�
			else sec=nsec;						// �������
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




		
