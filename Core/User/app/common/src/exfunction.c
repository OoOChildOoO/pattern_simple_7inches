#include "exfunction.h"
#include "utility.h"

/*-----------------------------------------------
//函数名: U8 geteWeekDay(U16 y,U8 m,U8 d)
//功能  : 基姆拉尔森计算公式根据日期判断星期几  
--------------------------------------*/
U8 getWeekDay(U16 y,U8 m,U8 d)
{
	U8 week; //0-6 ->周一-周日
	if(m==1||m==2){
		m+=12;y--;
	}   
    week = (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;  
	return week;

}

/*-----------------------------------------------
//函数名:U8 isContain_U16(U16 *buff,U16 len,U16 dat)
//功能  :判断数组是否包含指定数值
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


