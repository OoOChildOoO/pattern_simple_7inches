#include "exfunction.h"
#include "utility.h"

/*-----------------------------------------------
//������: U8 geteWeekDay(U16 y,U8 m,U8 d)
//����  : ��ķ����ɭ���㹫ʽ���������ж����ڼ�  
--------------------------------------*/
U8 getWeekDay(U16 y,U8 m,U8 d)
{
	U8 week; //0-6 ->��һ-����
	if(m==1||m==2){
		m+=12;y--;
	}   
    week = (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;  
	return week;

}

/*-----------------------------------------------
//������:U8 isContain_U16(U16 *buff,U16 len,U16 dat)
//����  :�ж������Ƿ����ָ����ֵ
--------------------------------------*/
U8 isContain_U16(U16 *buff,U16 len,U16 dat)
{
	U16 i;
	wdt();
	for(i=0;i<len;i++){
		if(buff[i]==dat){
			return 1;
		}
	}
	return 0;
}


