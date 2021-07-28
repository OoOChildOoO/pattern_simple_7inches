//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  Patfun.c 								*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2017年09月14日 							*
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
#include <math.h>	
#include <stdlib.h>	
#include "Patfun.h"

extern volatile S32 g_array3[2][2];
extern volatile S32 g_array33[2][2];
//------------------------------------------
// Function   : int sqr(U64 num)			
// Description: 求平方根							
//------------------------------------------
int sqr(U64 num)
{
	int Result=0;
	Result = sqrt(num);
	return(Result);
}

//------------------------------------------
// Function   : U32 twopoint_dis(S32 xx,S32 yy)			
// Description: 求两点距离							
//------------------------------------------
U32 twopoint_dis(S32 xx,S32 yy)
{
	U64 x,y;
	U32 d;
	U64 temp;
	
	wdt();
	if((xx==0)&&(yy==0)){
		return(0);
	}else{
		x = abs(xx);
		y = abs(yy);
		temp = (U64)((U64)(x*x)+(U64)(y*y));
		//printf("twopoint_dis xx = %d yy = %d x = %lld y = %lld temp = %lld  %lld \r\n",xx,yy,x,y,temp,(U64)((U64)(x*x)+(U64)(y*y)));
		d = (U32)(sqr(temp*100));
		d = (d + 5) / 10;
		
		return(d);
	}
}

//------------------------------------------
// Function   : U32 twopoint_dis1(S32 xx,S32 yy)			
// Description: 求两点距离							
//------------------------------------------
U32 twopoint_dis1(S32 xx,S32 yy)
{
	U64 x,y;
	U32 d;
	U64 temp;
	wdt();
	if((xx==0)&&(yy==0)){	 ///////////////////add on 2009.6.4
		return(0);
		////////////////////////////
	}else{
		x = abs(xx);
		y = abs(yy);
		temp = (U64)((U64)(x*x)+(U64)(y*y));
		d = (U32)(sqrt(temp*100));
		d = (d + 5) / 10;
		return(d);
	}
}

//------------------------------------------
// Function   : U32 twopoint_dis_zag(S32 xx,S32 yy)			
// Description: 求两点距离							
//------------------------------------------
U32 twopoint_dis_zag(S32 xx,S32 yy)
{
	U32 x,y;
	U32 d;
	U64 tem;
	
	x = abs(xx);
	y = abs(yy);
	tem = (U32)(x*x)+(U32)(y*y);
	tem *= 100;
	d = sqr(tem);
	d = (d + 5) / 10;
	return(d);
}

//------------------------------------------
// Function   : U32 getpoint_sum(U32 dis,U8 temp_pitch,U8 extention)			
// Description: 计算针数							
//------------------------------------------
U32 getpoint_sum(U32 dis,U8 temp_pitch,U8 extention,U8 flag)
{
	U32 n,dat;
	
	wdt();
	n = dis/ temp_pitch;
	dat = dis % temp_pitch;
	if(dat!=0){
		if(n==0)n = 1;
		else if((extention*n)<dat) n ++;
	}
	dat = dis  / n;
	if(flag==1){	//SEW 需x2
		if(dat>(127*2)) n ++;
	}else{
		if(dat>127) n ++;
	}
	
	return(n);
}

//------------------------------------------
// Function   : U32 getpoint_sum2(U32 dis,U8 temp_pitch,U8 extention)			
// Description: 计算针数							
//------------------------------------------
U32 getpoint_sum2(U32 dis,U8 temp_pitch,U8 extention,U8 flag)
{
	U32 n,dat,dat1;
	U32 dat2;
	wdt();
	dat = 0xffffffff;
	n = dis/ temp_pitch;
	if(n!=0)
		dat = dis * 10 / n;

	dat2 = dis % temp_pitch;
	if(dat2 != 0){
		dat = dat - temp_pitch*10;
		dat1 = dis * 10 / (n+1);
		dat1 = temp_pitch*10 - dat1;
		if(dat>=dat1){
			if(dat>extention*10) n ++;
		}else if(temp_pitch>=120){
			n ++;	
		}
	}
	dat = dis * 10 / n;
	if(flag==1){	//SEW 需x2
		if(dat>(1270*2)) n ++;
	}else{
		if(dat>1270) n ++;
	}
	
	return(n);
}

//------------------------------------------
// Function   : void verticalline_pos(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)			
// Description: 求直线外一点坐标							
//------------------------------------------
void verticalline_pos(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)
{
	S32 x,y,x0,y0;
	U32 d;
	
	d = twopoint_dis(xx,yy);
	if(d==0)d = 1;
	if(f){
		x0 = -xx;
		y0 = -yy;
	}else{
		x0 = xx;
		y0 = yy;
	}
	x0 = x0 * 10;
	y0 = y0 * 10;
	x = y0*(S32)(dis)/(S32)(d);
	y = x0*(S32)(dis)/(S32)(d);
	if(f){
		if(dir==0){
			y = -y;
		}else{
			x = -x;
		}
	}else{
		if(dir){
			y = -y;
		}else{
			x = -x;
		}

	}
	if(x>=0)g_array3[0][0] = (x + 5) / 10;else g_array3[0][0] = (x - 5) / 10;
	if(y>=0)g_array3[0][1] = (y + 5) / 10;else g_array3[0][1] = (y - 5) / 10;
}

//------------------------------------------
// Function   : void verticalline_pos1(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)			
// Description: 求直线外一点坐标							
//------------------------------------------
void verticalline_pos1(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)
{
	S32 x,y,x0,y0;
	U32 d;
	
	d = twopoint_dis1(xx,yy);
	if(d==0)d = 10;
	if(f){
		x0 = -xx;
		y0 = -yy;
	}else{
		x0 = xx;
		y0 = yy;
	}
	x0 = x0 * 10;
	y0 = y0 * 10;
	x = y0*(S32)(dis)/(S32)(d);
	y = x0*(S32)(dis)/(S32)(d);
	if(f){
		if(dir==0){
			y = -y;
		}else{
			x = -x;
		}
	}else{
		if(dir){
			y = -y;
		}else{
			x = -x;
		}
	}
	if(x>=0)g_array3[0][0] = (x + 5) / 10;else g_array3[0][0] = (x - 5) / 10;
	if(y>=0)g_array3[0][1] = (y + 5) / 10;else g_array3[0][1] = (y - 5) / 10;
}


//------------------------------------------
// Function   : void verticalline_pos2(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)			
// Description:  求直线外一点坐标							
//------------------------------------------
void verticalline_pos2(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)
{
	S32 x,y;
	U32 d;
	S32 x0,y0;
	U64 temp;
	
	if((xx==0)&&(yy==0)){	 
		d = 0;	
	}else{
		temp = (U64)(xx*xx)+(U64)(yy*yy);		
		d = (U32)(sqrt(temp*100));				
	}

	if(d==0)d = 10;
	if(f){
		x0 = -xx;
		y0 = -yy;
	}else{
		x0 = xx;
		y0 = yy;
	}
	x0 = x0 * 1000;
	y0 = y0 * 1000;
	x = y0*(S32)(dis)/(S32)(d);
	y = x0*(S32)(dis)/(S32)(d);
	if(f){
		if(dir==0){
			y = -y;
		}else{
			x = -x;
		}
	}else{
		if(dir){
			y = -y;
		}else{
			x = -x;
		}

	}
	g_array33[0][0] = x;
	g_array33[0][1] = y;
}

//------------------------------------------
// Function   : void circle_center(S32 x0,S32 y0,S32 x1,S32 y1,COORDINATE *posi)		
// Description: 计算圆心							
//------------------------------------------
void circle_center(S32 x0,S32 y0,S32 x1,S32 y1,COORDINATE *posi)
{	
	S32 k1,k2;
	wdt();
	//printf("circle_center x0 =%d  y0 = %d  x1 =%d  y1 = %d\r\n",x0,y0,x1,y1);
	g_array3[0][0] = x0*5;
	g_array3[0][1] = y0*5;
	if(y0!=0){k1=x0*(S32)(100)*(-1)/y0;if(k1==0x7fffffff)k1--;}else k1 = 0x7fffffff;
	//printf("k1 = %d  g_array3[0][0] = %d g_array3[0][1] = %d  g_array3[1][0] = %d g_array3[1][1] = %d\r\n",k1,g_array3[0][0],g_array3[0][1],g_array3[1][0],g_array3[1][1]);

	g_array3[1][0] = x1*5;
	g_array3[1][1] = y1*5;
	if(y1!=0){k2=x1*(S32)(100)*(-1)/y1;if(k2==0x7fffffff)k2--;}else k2 = 0x7fffffff;
	//printf("k1 = %d k2 = %d g_array3[0][0] = %d g_array3[0][1] = %d  g_array3[1][0] = %d g_array3[1][1] = %d\r\n",k1,k2,g_array3[0][0],g_array3[0][1],g_array3[1][0],g_array3[1][1]);

	lines_intersection(k1,k2);
	
	//printf("g_array3[0][0] = %d g_array3[0][1] = %d  g_array3[1][0] = %d g_array3[1][1] = %d\r\n",g_array3[0][0],g_array3[0][1],g_array3[1][0],g_array3[1][1]);
	if(g_array3[0][0]>=0)g_array3[0][0]+=5;else g_array3[0][0]-=5;
	if(g_array3[0][1]>=0)g_array3[0][1]+=5;else g_array3[0][1]-=5;
	posi->x = g_array3[0][0] / 10;
	posi->y = g_array3[0][1] / 10;
	wdt();
}

//------------------------------------------
// Function   : void circle_center(S32 x0,S32 y0,S32 x1,S32 y1,COORDINATE *posi)		
// Description: 计算圆心							
//------------------------------------------
//void circle_center1(S32 x0,S32 y0,S32 x1,S32 y1,COORDINATE *posi)
//{	
//	S64 k1,k2;
//	wdt();
//	printf("circle_center x0 =%d  y0 = %d  x1 =%d  y1 = %d\r\n",x0,y0,x1,y1);
//	g_array3[0][0] = x0*5;
//	g_array3[0][1] = y0*5;
//	if(y0!=0){k1=x0*(S64)(100)*(-1)/y0;if(k1==0x7fffffff)k1--;}else k1 = 0x7fffffff;
//	printf("k1 = %d k2 = %d g_array3[0][0] = %d g_array3[0][1] = %d  g_array3[1][0] = %d g_array3[1][1] = %d\r\n",k1,k2,g_array3[0][0],g_array3[0][1],g_array3[1][0],g_array3[1][1]);

//	g_array3[1][0] = x1*5;
//	g_array3[1][1] = y1*5;
//	if(y1!=0){k2=x1*(S32)(100)*(-1)/y1;if(k2==0x7fffffff)k2--;}else k2 = 0x7fffffff;
//	printf("k1 = %d k2 = %d g_array3[0][0] = %d g_array3[0][1] = %d  g_array3[1][0] = %d g_array3[1][1] = %d\r\n",k1,k2,g_array3[0][0],g_array3[0][1],g_array3[1][0],g_array3[1][1]);

//	lines_intersection(k1,k2);
//	
//	printf("g_array3[0][0] = %d g_array3[0][1] = %d  g_array3[1][0] = %d g_array3[1][1] = %d\r\n",g_array3[0][0],g_array3[0][1],g_array3[1][0],g_array3[1][1]);
//	if(g_array3[0][0]>=0)g_array3[0][0]+=5;else g_array3[0][0]-=5;
//	if(g_array3[0][1]>=0)g_array3[0][1]+=5;else g_array3[0][1]-=5;
//	posi->x = g_array3[0][0] / 10;
//	posi->y = g_array3[0][1] / 10;
//	wdt();
//}

//------------------------------------------
// Function   : U8 lines_intersection(S32 k1,S32 k2)		
// Description: 求两直线交点坐标							
//------------------------------------------
U8 lines_intersection(S32 k1,S32 k2)
{
	S64 dat1,dat2;
	wdt();
	if(k1==k2){
		if((g_array3[0][0]!=g_array3[1][0])||(g_array3[0][1]!=g_array3[1][1])){
			return(1);
		}
	}else{
		if((g_array3[0][0]==g_array3[1][0])&&(g_array3[0][1]==g_array3[1][1])){
			return(1);
		}else if(k1==0x7fffffff){
			dat1 = (g_array3[0][0]-g_array3[1][0])*k2/100;
			g_array3[0][1] = dat1 + g_array3[1][1];
		}else if(k2==0x7fffffff){
			dat1 = (g_array3[1][0]-g_array3[0][0])*k1/100;
			g_array3[0][1] = dat1 + g_array3[0][1];
			g_array3[0][0] = g_array3[1][0];
		}else{
			dat1 = k1 * g_array3[0][0];					//1
			dat2 = k2 * g_array3[1][0];					//2
			//printf("0 dat1 = %lld  dat2 = %lld \r\n",dat1,dat2);
			dat1 = dat1 - dat2;							//3
			dat2 = (g_array3[1][1] - g_array3[0][1]) * 100;	//4
			//printf("1 dat1 = %lld  dat2 = %lld \r\n",dat1,dat2);
			dat1 = (dat1 + dat2) * 10;					//5
			dat2 = k1 - k2;								//6
			//printf("2 dat1 = %lld  dat2 = %lld \r\n",dat1,dat2);
			dat1 = dat1 / dat2;							//7
			dat2 = (dat1-g_array3[1][0]*10)*k2/100;		//8
			//printf("3 dat1 = %lld  dat2 = %lld \r\n",dat1,dat2);
			dat2 = dat2 + g_array3[1][1]*10;				//9
			if(dat1>=0)dat1+=5;else dat1-=5;
			if(dat2>=0)dat2+=5;else dat2-=5;
			//printf("4 dat1 = %lld  dat2 = %lld \r\n",dat1,dat2);
			g_array3[0][0] = dat1/10;						//x
			g_array3[0][1] = dat2/10;						//y
		}
	}
	return(1);
}

//------------------------------------------
// Function   : U8 lines_intersection1(S32 k1,S32 k2)			
// Description: 求两直线交点坐标							
//------------------------------------------
U8 lines_intersection1(S32 k1,S32 k2)
{
	S64 dat1,dat2;

	dat1 = g_array33[0][0];						//x
	dat2 = g_array33[0][1];						//y	

	if(k1==k2){
		if((g_array33[0][0]!=g_array33[1][0])||(g_array33[0][1]!=g_array33[1][1])){	}
	}else{		
		if((g_array33[0][0]==g_array33[1][0])&&(g_array33[0][1]==g_array33[1][1])){

		}else if(k1==0x7fffffff){
			dat1 = (g_array33[0][0]-g_array33[1][0])*k2;
			if(dat1>=0)dat1+=5;else dat1-=5;
			dat1 = dat1/10;				

			g_array33[0][1] = dat1 + g_array33[1][1];
			dat1 = g_array33[0][0];						//x
			dat2 = g_array33[0][1];						//y	
		}else if(k2==0x7fffffff){
			dat1 = (g_array33[1][0]-g_array33[0][0])*k1;
			if(dat1>=0)dat1+=5;else dat1-=5;
			dat1 = dat1/10;						//x

			g_array33[0][1] = dat1 + g_array33[0][1];
			g_array33[0][0] = g_array33[1][0];
			dat1 = g_array33[0][0];						//x
			dat2 = g_array33[0][1];						//y	
		}else{
			dat1 = k1 * g_array33[0][0];					//1
			dat2 = k2 * g_array33[1][0];					//2
			dat1 = dat1 - dat2;							//3
			dat2 = (g_array33[1][1] - g_array33[0][1]) * 10;	//4			
			dat1 = (dat1 + dat2);					//5
			dat2 = k1 - k2;								//6
			dat1 = dat1 / dat2;							//7
			dat2 = (dat1-g_array33[1][0])*k2;			
			if(dat2>=0)dat2+=5;else dat2-=5;
			dat2 = dat2/10;						//x

			dat2 = dat2 + g_array33[1][1];				//9													
		}		
	}
	if(dat1>=0)dat1+=50;else dat1-=50;		 //四舍五入
	if(dat2>=0)dat2+=50;else dat2-=50;
	g_array33[0][0] = dat1/100;						//x
	g_array33[0][1] = dat2/100;						//y
			
	return(1);
}

//------------------------------------------
// Function   : void verticalline_pos_zag(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)			
// Description: 求直线外一点坐标							
//------------------------------------------
void verticalline_pos_zag(U8 dir,U32 dis,S32 xx,S32 yy,U8 f)
{
	S32 x,y,x0,y0;
	U32 d;
	
	d = twopoint_dis_zag(xx,yy);
	if(d==0)d = 100;
	if(f){
		x0 = -xx;
		y0 = -yy;
	}else{
		x0 = xx;
		y0 = yy;
	}
	x0 = x0 * 10;
	y0 = y0 * 10;
	x = y0*(S32)(dis)/(S32)(d);
	y = x0*(S32)(dis)/(S32)(d);
	if(f){
		if(dir==0){
			y = -y;
		}else{
			x = -x;
		}
	}else{
		if(dir){
			y = -y;
		}else{
			x = -x;
		}
	}
	if(x>=0)g_array3[0][0] = (x + 5) / 10;else g_array3[0][0] = (x - 5) / 10;
	if(y>=0)g_array3[0][1] = (y + 5) / 10;else g_array3[0][1] = (y - 5) / 10;
}

