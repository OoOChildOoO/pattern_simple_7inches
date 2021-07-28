//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  viewPattern.c							*
//*  创   建  人:  OUYANGQINGJIANG							*
//*  创 建 日 期:  2015年10月01日							*
//*  最后修改日期: 2016年03月01日 							*
//*---------------------------------------------------------*
//*  功能描述: 												*
//*---------------------------------------------------------*
//*  硬件描述  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  软件描述: 开发软件Keil Vision4 V4.74					*
//*  编程													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  版本修订: 												*
//*---------------------------------------------------------*

#define bpno1 plastno

#include <math.h>
#include <stdlib.h>
#include "function.h"
#include "viewPattern.h"
#include "Bor_patfun.h"
#include "color.h"
#include "utility.h"
#include "lcddisp.h"
#include "register.h"
#include "menu.h"

extern PATHEAD g_patd;
extern OLDPAT g_dispat;
extern volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //x,y坐标实际值
extern volatile S16 g_dxpos, g_dypos;
extern volatile S16 g_disxpos, g_disypos;
extern volatile U16 g_point_num;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern volatile U16 g_line_sum;
extern volatile U8 g_UdZhouFlag;
extern volatile U8 g_init_flag;
extern volatile U16 hleppara, g_para0, g_para1, g_imgpara;
extern volatile U16 g_feedct;
extern volatile U32 g_feedc; //当前针
extern volatile U32 g_max_feedsum;
extern volatile U32 g_min_feedsum;
extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U32 g_jog_point;
extern volatile U8 g_sta_end_flag;
extern volatile U8 g_jogmode;
extern volatile U8 g_datum_mark;
extern volatile U8 g_ImgPoint;
extern volatile U8 g_BorSTflag;
extern volatile U8 g_CodeEn;
extern volatile U16 g_parameter;
extern volatile U8 g_selectmode;

extern volatile S16 g_maxX;
extern volatile S16 g_minX;
extern volatile S16 g_maxY;
extern volatile S16 g_minY;

extern volatile S32 g_array3[2][2];
extern volatile U16 g_array4[386][2];
extern volatile U16 g_array5[630][2];
extern volatile S16 g_array10[630][2];

//------------------------------------------
//	 Function   :  Draw_Pattern
// 	Description:  显示花样绘图数据
//				 中心点164,120 宽230x166
//				 比例1dot:1mm scale=100
//	note :
//  U32 vn: 不同页面显示不同的花样风格
//  S32 scale: 缩放比
//	S16 ox1, S16 oy1 : 花样显示区域中心点
// 	U8 PatType：花样类型  0-三菱款    1-兄弟款   2016-11-30
// 	U32 Patlen:花样数据总长 针对兄弟款   2016-12-16
//  U32 *infeed:花样针数
//  U16 inxpos, U16 inypos:新输入点
//------------------------------------------/
void Draw_Pattern(U32 vn, S32 scale, S16 ox1, S16 oy1, U8 PatType, U32 Patlen, U16 inxpos, U16 inypos, U32 *infeed)
{
	U8 ch, sameline_f, colorf = 0; //,cw
	U8 nn = 0;
	U32 i, j, k = 0, m, n /*, di, pi, pj*/;

	U32 code, dat, dat1, dat3; //,dat2

	// U16 code1;
	U32 pat_add_p, line_add_p, temp_line_add, temp_pat_add;
	S32 tmpxpos = 0, tmpypos = 0, stxpos = 0, stypos = 0, enxpos = 0, enypos = 0;
	S16 xx, yy;
	S32 ox, oy, /*ox0, oy0,*/ x, y;
	S32 k1 = 0, k2 = 0;
	S16 xtmp, ytmp /*,xtmp1, ytmp1*/;
	S32 x1 = 0, y1 = 0, x2 = 0, y2 = 0, tmpx2 = 0, tmpy2 = 0, tmpx3 = 0, tmpy3 = 0, tmpxS = 0, tmpyS = 0, tmpxE = 0, tmpyE = 0;
	U8 disP = 0, disP1 = 0, disS = 0, disE = 0;
	S16 xorg = 0, yorg = 0;
	U8 Mtype = 0; //1为原三菱G版的花样数据，0为MLK的数据
	S32 tmpScale = 0;
	S32 xpoint = 0, ypoint = 0, txpoint = 0, typoint = 0, xpoint1 = 0, ypoint1 = 0;
	S16 disxpoint = LCD_width, disypoint = LCD_height;
	U32 tmpfeed = 0, tmpfeed1 = 0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	//	U8 tmpISdat =0;
	U8 f;
	S32 xst = 0, xend = 0, yst = 0, yend = 0;
	U16 sewColor = COLOR_LINE_SEW;
	// U16 color;
	// U8 pres = 0, dres = 0;
	//   S16 StPoix,StPoiy,StPoix1,StPoiy1;
	//   S8 tmpBorpooft;
	S16 tmpxmax = 0, tmpxmin = 0, tmpymax = 0, tmpymin = 0;
	U32 drawAddr = (SREG[DA_ADDR + 1] << 16) | SREG[DA_ADDR]; //绘图数据地址
	U32 drawLen = (SREG[DL_ADDR + 1] << 16) | SREG[DL_ADDR];
	U32 pinAddr = (SREG[PA_ADDR + 1] << 16) | SREG[PA_ADDR]; //针数据起始地址

	// U16 tpinsLen;
	// EMBDINFO dinfo;
	// EMBPINFO pinfo;
	// U8 *p8;

	if ((inxpos == 0) && (inypos == 0))
	{ //不更新输入点
		disxpoint = 0;
		disypoint = 0;
	}

	if ((vn == PAT_P1) || (vn == 100) || (vn == PAT_VIEW_CREATE) ||
		(vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3) ||
		(vn == PAT_VIEW_CONVENT) || (vn == PAT_P1_ZOOM))
	{
		tmpScale = scale;
		scale = 0;
	}
	g_patd.scale = scale;
	g_dispat.feedc = 0;
	// 最大比例
	if (g_patd.patmX == g_patd.patiX)
		g_patd.patmX++;
	if (g_patd.patmY == g_patd.patiY)
		g_patd.patmY++;

	wdt();

	if (vn == PAT_P1 || vn == 100) // 主画面
	{
		xorg = CONVAS_ORG_PX1;
		yorg = CONVAS_ORG_PY1;

		DrawFillRect(CONVAS_PX1, CONVAS_PY1,
					 CONVAS_X1, CONVAS_Y1, CONVAS_COLOR1);
		SetDrawXY(CONVAS_PX1, CONVAS_PY1, CONVAS_PX1 + CONVAS_X1, CONVAS_PY1 + CONVAS_Y1, 1);

		xtmp = IMAGE_X1 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y1 * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY)); //-16  修改

		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}
	else if (vn == PAT_MODIFY || vn == PAT_CONVENT)
	{ // 修改画面显示图形
		xorg = CONVAS_ORG_PX6;
		yorg = CONVAS_ORG_PY6;
		colorf = 0;
		xtmp = IMAGE_X6 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y6 * 100 / (g_patd.patmY - g_patd.patiY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}

	else if (vn == PAT_READ)
	{ // 读取花样查看图形
		xorg = ox1;
		yorg = oy1;

		xtmp = IMAGE_X2 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y2 * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY)); //-16  修改

		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}
	else if ((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY3))
	{
		xorg = CONVAS_ORG_PX3;
		yorg = CONVAS_ORG_PY3;
		DrawFillRect(CONVAS_PX3, CONVAS_PY3, CONVAS_X3, CONVAS_Y3, CONVAS_COLOR_B);
		if (PatType == PAT_MIT)
		{ //三菱
			if (vn == PAT_VIEW_MODIFY3)
			{
				// for (i = 1; i <= g_point_num; i++)
				// {
				// 	if ((g_array10[i][0] != 0) || (g_array10[i][1] != 0))
				// 		break;
				// }
				// if (i <= g_point_num)
				// {
				// 	AnaPattern2(5);
				// }
				// else
				// {
				// 	if ((g_dxpos > 0x1F) || (g_dypos > 0x1F))
				// 		AnaPattern2(5);
				// }
			}
			xtmp = IMAGE_X3 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
			ytmp = IMAGE_Y3 * 100 / (g_patd.patmY - g_patd.patiY);
			if (xtmp > ytmp)
				scale = ytmp * 10;
			else
				scale = xtmp * 10;
		}
		else if (PatType == PAT_SEW)
		{ //兄弟
			if (vn == PAT_VIEW_MODIFY3)
			{
				// tmpxmax = g_patd.patmX;
				// tmpxmin = g_patd.patiX;
				// tmpymax = g_patd.patmY;
				// tmpymin = g_patd.patiY;
				// for (k = 1; k < g_point_num; k++)
				// {
				// 	k1 = (S16)(g_array10[k][0]);
				// 	k2 = (S16)(g_array10[k][1]);
				// 	if (k1 > tmpxmax)
				// 		tmpxmax = k1;
				// 	if (k1 < tmpxmin)
				// 		tmpxmin = k1;
				// 	if (k2 > tmpymax)
				// 		tmpymax = k2;
				// 	if (k2 < tmpymin)
				// 		tmpymin = k2;
				// }
				// if (g_disxpos > tmpxmax)
				// 	tmpxmax = g_disxpos;
				// if (g_disxpos < tmpxmin)
				// 	tmpxmin = g_disxpos;
				// if (g_disypos > tmpymax)
				// 	tmpymax = g_disypos;
				// if (g_disypos < tmpymin)
				// 	tmpymin = g_disypos;

				// xtmp = IMAGE_X3 * 100 / ((tmpxmax - tmpxmin) < 2 ? 2 : (tmpxmax - tmpxmin));
				// ytmp = IMAGE_Y3 * 100 / (tmpymax - tmpymin);
				xtmp = IMAGE_X3 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
				ytmp = IMAGE_Y3 * 100 / (g_patd.patmY - g_patd.patiY);
			}
			else
			{
				xtmp = IMAGE_X3 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
				ytmp = IMAGE_Y3 * 100 / (g_patd.patmY - g_patd.patiY);
			}
			if (xtmp > ytmp)
				scale = ytmp * 10;
			else
				scale = xtmp * 10;
		}
		SetDrawXY(CONVAS_PX3, CONVAS_PY3, CONVAS_PX3 + CONVAS_X3, CONVAS_PY3 + CONVAS_Y3, 1);
	}
	else if ((vn == PAT_VIEW_MODIFY2))
	{
		xorg = CONVAS_ORG_PX4;
		yorg = CONVAS_ORG_PY4;
		DrawFillRect(CONVAS_PX4, CONVAS_PY4, CONVAS_X4, CONVAS_Y4, CONVAS_COLOR_B);
		if (PatType == PAT_MIT)
		{ //三菱
			if (vn == PAT_VIEW_MODIFY3)
			{
				// for (i = 1; i <= g_point_num; i++)
				// {
				// 	if ((g_array10[i][0] != 0) || (g_array10[i][1] != 0))
				// 		break;
				// }
				// if (i <= g_point_num)
				// {
				// 	AnaPattern2(5);
				// }
				// else
				// {
				// 	if ((g_dxpos > 0x1F) || (g_dypos > 0x1F))
				// 		AnaPattern2(5);
				// }
			}
			xtmp = IMAGE_X4 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
			ytmp = IMAGE_Y4 * 100 / (g_patd.patmY - g_patd.patiY);
			if (xtmp > ytmp)
				scale = ytmp * 10;
			else
				scale = xtmp * 10;
		}
		else if (PatType == PAT_SEW)
		{ //兄弟
			if (vn == PAT_VIEW_MODIFY3)
			{
				// tmpxmax = g_patd.patmX;
				// tmpxmin = g_patd.patiX;
				// tmpymax = g_patd.patmY;
				// tmpymin = g_patd.patiY;
				// for (k = 1; k < g_point_num; k++)
				// {
				// 	k1 = (S16)(g_array10[k][0]);
				// 	k2 = (S16)(g_array10[k][1]);
				// 	if (k1 > tmpxmax)
				// 		tmpxmax = k1;
				// 	if (k1 < tmpxmin)
				// 		tmpxmin = k1;
				// 	if (k2 > tmpymax)
				// 		tmpymax = k2;
				// 	if (k2 < tmpymin)
				// 		tmpymin = k2;
				// }
				// if (g_disxpos > tmpxmax)
				// 	tmpxmax = g_disxpos;
				// if (g_disxpos < tmpxmin)
				// 	tmpxmin = g_disxpos;
				// if (g_disypos > tmpymax)
				// 	tmpymax = g_disypos;
				// if (g_disypos < tmpymin)
				// 	tmpymin = g_disypos;

				// xtmp = IMAGE_X4 * 100 / ((tmpxmax - tmpxmin) < 2 ? 2 : (tmpxmax - tmpxmin));
				// ytmp = IMAGE_Y4 * 100 / (tmpymax - tmpymin);
				xtmp = IMAGE_X4 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
				ytmp = IMAGE_Y4 * 100 / (g_patd.patmY - g_patd.patiY);
			}
			else
			{
				xtmp = IMAGE_X4 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
				ytmp = IMAGE_Y4 * 100 / (g_patd.patmY - g_patd.patiY);
			}
			if (xtmp > ytmp)
				scale = ytmp * 10;
			else
				scale = xtmp * 10;
		}
		SetDrawXY(CONVAS_PX4, CONVAS_PY4, CONVAS_PX4 + CONVAS_X4, CONVAS_PY4 + CONVAS_Y4, 1);
	}
	else if ((vn == PAT_VIEW_CREATE) || (vn == PAT_VIEW_CONVENT))
	{
		xorg = CONVAS_ORG_PX4;
		yorg = CONVAS_ORG_PY4;
		DrawFillRect(CONVAS_PX4, CONVAS_PY4, CONVAS_X4, CONVAS_Y4, CONVAS_COLOR4);

		if (PatType == PAT_MIT)
		{ //三菱
			if (vn == PAT_VIEW_CREATE)
			{
				// x = g_oxpos;
				// y = -g_oypos;
				// if (SREG[3] == 7)
				// {
				// 	x += (S16)(SREG[6]);
				// 	y -= (S16)(SREG[7]);
				// }

				// if (g_array4[0][0] > 301)
				// 	g_array4[0][0] = 0;
				// n = g_array4[0][0] + 1;
				// for (k = 0; k < n; k++)
				// {
				// 	if (k == g_array4[0][0])
				// 	{
				// 		x += g_dxpos;
				// 		y -= g_dypos;
				// 	}
				// 	else
				// 	{
				// 		x += (S16)(g_array4[k + 1][0]);
				// 		y -= (S16)(g_array4[k + 1][1]);
				// 	}
				// 	if (x > g_patd.patmX)
				// 		g_patd.patmX = x;
				// 	if (y > g_patd.patmY)
				// 		g_patd.patmY = y;
				// 	if (x < g_patd.patiX)
				// 		g_patd.patiX = x;
				// 	if (y < g_patd.patiY)
				// 		g_patd.patiY = y;
				// }
			}

			xtmp = IMAGE_X4 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
			ytmp = IMAGE_Y4 * 100 / (g_patd.patmY - g_patd.patiY);
			if (xtmp > ytmp)
				scale = ytmp * 10;
			else
				scale = xtmp * 10;
		}
		else if (PatType == PAT_SEW)
		{ //兄弟
			if (vn == PAT_VIEW_CREATE)
			{ //打板中查看花样
				// tmpxmax = g_patd.patmX;
				// tmpxmin = g_patd.patiX;
				// tmpymax = g_patd.patmY;
				// tmpymin = g_patd.patiY;

				// if (g_array4[0][0] > 301)
				// 	g_array4[0][0] = 0;
				// n = g_array4[0][0] + 1;
				// for (k = 0; k < n; k++)
				// {
				// 	if (k == g_array4[0][0])
				// 	{
				// 		k1 = g_disxpos;
				// 		k2 = g_disypos;
				// 	}
				// 	else
				// 	{
				// 		k1 = (S16)(g_array4[k + 1][0]);
				// 		k2 = (S16)(g_array4[k + 1][1]);
				// 	}
				// 	if (k1 > tmpxmax)
				// 		tmpxmax = k1;
				// 	if (k1 < tmpxmin)
				// 		tmpxmin = k1;
				// 	if (k2 > tmpymax)
				// 		tmpymax = k2;
				// 	if (k2 < tmpymin)
				// 		tmpymin = k2;
				// }
				// xtmp = IMAGE_X4 * 100 / ((tmpxmax - tmpxmin) < 2 ? 2 : (tmpxmax - tmpxmin));
				// ytmp = IMAGE_Y4 * 100 / (tmpymax - tmpymin);
				xtmp = IMAGE_X4 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
				ytmp = IMAGE_Y4 * 100 / (g_patd.patmY - g_patd.patiY);
			}
			else
			{
				xtmp = IMAGE_X4 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
				ytmp = IMAGE_Y4 * 100 / (g_patd.patmY - g_patd.patiY);
			}
			if (xtmp > ytmp)
				scale = ytmp * 10;
			else
				scale = xtmp * 10;
		}

		SetDrawXY(CONVAS_PX4, CONVAS_PY4, CONVAS_PX4 + CONVAS_X4, CONVAS_PY4 + CONVAS_Y4, 1);
	}
	else if (vn == PAT_CREATE)
	{ // 打版中显示图形

		xorg = CONVAS_ORG_PX5;
		yorg = CONVAS_ORG_PY5;
		colorf = 0;
		xtmp = IMAGE_X5 * 100 / (g_patd.patmX - g_patd.patiX);
		ytmp = IMAGE_Y5 * 100 / (g_patd.patmY - g_patd.patiY);
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}
	wdt();

	//修改 begin
	if ((ox1 != xorg) || (oy1 != yorg))
	{
		if ((vn == PAT_P1) || (vn == 100) || (vn == PAT_VIEW_CREATE) ||
			(vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3) ||
			(vn == PAT_VIEW_CONVENT) || (vn == PAT_P1_ZOOM))
		{
			xorg = ox1;
			yorg = oy1;
		}
	}
	//修改 end

	scale += tmpScale;
	if (scale < 10)
		scale = 10;

	if ((g_patd.scale == 0) || (g_patd.scale > scale))
	{
		g_patd.scale = scale;
	}
	else
	{
		scale = g_patd.scale;
	}

	if (PatType == PAT_MIT)
	{																		  //三菱款
		if (vn == PAT_P1 || vn == 100 || vn == PAT_P1_ZOOM || vn == PAT_READ) //P1画面全部用针数据显示
		{
			xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
			yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;
			// DrawHLine(xorg - 1, yorg, 3, Color_Green);
			// DrawVLine(xorg, yorg - 1, 3, Color_Green);//修改
			DrawPoint(xorg, yorg, COLOR_ORG);

			// /[缝纫针点与移送针点]*************************
			i = g_patd.pat_add;
			ox = 0;
			oy = 0;
			x = 0;
			y = 0;
			for (k = 0; k < g_patd.feed_sum; k++)
			{
				dat1 = SREG[i] & 0xff;

				//缝纫针点与移送针点
				if ((dat1 == 0x1) || (dat1 == 0x21) || (dat1 == 0x41) ||
					(dat1 == 0x61) || (dat1 == 0x3) || (dat1 == 0x1b))
				{
					dat = SREG[i + 1];
					x1 = (U8)(dat);
					y1 = (U8)((U16)(dat) >> 8);
					if (x1 >= 0x80)
					{
						x1 -= 0x80;
						x -= x1;
					}
					else
						x += x1;
					if (y1 >= 0x80)
					{
						y1 -= 0x80;
						y -= y1;
					}
					else
						y += y1;
					x = ox + x;
					y = oy - y;
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);

					if (k < HREG[HREG_FEED_C])
					{
						// DrawLine(x1, y1, x2, y2, Color_Red);//修改
						DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH); //修改
					}
					else
					{
						if ((dat1 == 0x3) || (dat1 == 0x1b))
						{
							// DrawLine(x1, y1, x2, y2, Color_DimGray); //修改
							DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY); //修改
						}
						else
						{
							// DrawLine(x1, y1, x2, y2, Color_Blue); //修改
							DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW); //修改
						}
					}

					if (vn == PAT_P1 || vn == 100 || vn == PAT_P1_ZOOM)
					{
						if ((dat1 == 0x1 || dat1 == 0x21 || dat1 == 0x41 || dat1 == 0x61))
						{
							if (k < HREG[HREG_FEED_C])
							{
								// DrawHLine(x2 - 1, y2, 3, Color_Red); //绘制针点
								// DrawVLine(x2, y2 - 1, 3, Color_Red);//修改
								DrawPoint(x2, y2, COLOR_PIN_FINISH);
							}
							else
							{
								//	// DrawHLine(x1-1, y1, 3,Color_Black);		//绘制针点
								//	// DrawVLine(x1, y1-1, 3,Color_Black);

								DrawPoint(x1, y1, COLOR_PIN_SEW);
							}
						}
					}

					ox = x;
					oy = y;
					x = 0;
					y = 0;
				}

				i += 2;
			}

			// /[重绘运行过的针点]*************************
			if (vn == PAT_P1 || vn == 100 || vn == PAT_P1_ZOOM)
			{

				//--将运行点重新花一次
				//if(SREG[3]==7){
				//	ox = (S16)(SREG[6]); oy = -(S16)(SREG[7]);
				//}else{
				ox = 0;
				oy = 0;
				//}
				x = 0;
				y = 0;
				i = g_patd.pat_add;
				for (k = 0; k < HREG[HREG_FEED_C]; k++)
				{
					dat1 = SREG[i] & 0xff;
					if ((dat1 == 0x1) || (dat1 == 0x21) || (dat1 == 0x41) ||
						(dat1 == 0x61) || (dat1 == 0x3) || (dat1 == 0x1b))
					{
						dat = SREG[i + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						x = ox + x;
						y = oy - y;
						x1 = (S32)(ox * scale / 1000 + xorg);
						y1 = (S32)(oy * scale / 1000 + yorg);
						x2 = (S32)(x * scale / 1000 + xorg);
						y2 = (S32)(y * scale / 1000 + yorg);

						//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
						//x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
						// DrawLine(x1, y1, x2, y2,Color_Red);
						DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);

						//					if((dat1==0x1 || dat1==0x21 || dat1==0x41 || dat1==0x61 )){
						if ((vn == 100) && (dat1 == 0x1 || dat1 == 0x21 || dat1 == 0x41 || dat1 == 0x61))
						{ //修改

							// DrawHLine(x2 - 1, y2, 3, Color_Red); //绘制针点
							// DrawVLine(x2, y2 - 1, 3, Color_Red);//修改
							DrawPoint(x2, y2, COLOR_PIN_FINISH);
						}
						ox = x;
						oy = y;
						x = 0;
						y = 0;
					}

					i += 2;
				}
			}

			// /[绘制附加功能]*************************
			if (vn == PAT_P1 || vn == PAT_P1_ZOOM)
			{

				//if(SREG[3]==7){
				//	ox = (S16)(SREG[6]); oy = -(S16)(SREG[7]);
				//}else{
				ox = 0;
				oy = 0;
				//}
				x = 0;
				y = 0;
				i = g_patd.pat_add;

				DrawFPBMPBTN2(xorg - 7, yorg - 7, 14, 14, BMP_H);

				for (k = 0; k < g_patd.feed_sum; k++)
				{
					dat1 = SREG[i] & 0xff;
					if ((dat1 == 0x1) || (dat1 == 0x21) || (dat1 == 0x41) ||
						(dat1 == 0x61) || (dat1 == 0x3) || (dat1 == 0x1b))
					{
						dat = SREG[i + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						x = ox + x;
						y = oy - y;

						x2 = (S32)(x * scale / 1000 + xorg);
						y2 = (S32)(y * scale / 1000 + yorg);
						ox = x;
						oy = y;
						x = 0;
						y = 0;
						//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);

						if (dat1 == 0x1 || dat1 == 0x21 || dat1 == 0x41 || dat1 == 0x61)
						{
							dat1 = (SREG[i] >> 8) & 0xff;
							if (dat1 != 0)
							{
								if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
									DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_P);
							}
						}
					}
					else
					{
						//printf("dat1 = %02x \r\n",dat1);
						switch (dat1)
						{
						case 0x06:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_H2);
							break;
						case 0x3a:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_T);
							break;
						case 0x04:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_U);
							break;
						case 0x14:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_D);
							break;
						case 0x07:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_J);
							break;
						case 0x05:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F1);
							break;
						case 0x0b:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F2);
							break;
						case 0x0c:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F3);
							break;
						case 0x0d:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F4);
							break;
						case 0x1d:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
							{
								dat1 = (SREG[i] >> 8) & 0xff;
								switch (dat1)
								{
								case 0x00:
									DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F5);
									break;
								case 0x01:
									DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F6);
									break;
								case 0x02:
									DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F7);
									break;
								case 0x03:
									DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F8);
									break;
								}
							}

							break;
						case 0x02:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_C);
							break;
						}
					}
					wdt();
					i += 2;
				}
			}

			SetDrawXY(0, 0, LCD_width, LCD_height, 0);
			return;
		}

#if 1
		//指令代码暂不绘制
		x1 = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;

		if (((SREG[30] & 0x00ff) == 0x45) && (SREG[31] != 0x3536))
		{ //从原三菱E机器读回的数据
			wdt();
			SREG[30] = (SREG[30] & 0xff00) | 0x0054;
			x1 = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
			m = 0;

			for (y1 = 0; y1 < x1; y1++)
			{
				if (y1 != 0x20)
				{
					m += SREG[y1];
				}
			}
			SREG[0x20] = m;
			wdt();
		}

		if ((SREG[30] & 0x00ff) != MLK_G)
		{
			Mtype = 1;
		}

		//////////////////////////////////////////////////

		if (vn == PAT_READ || vn == PAT_P1 || vn == 100 || vn == PAT_P1_ZOOM)
		{
			i = ((SREG[2] + 0x40) / 2) + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4) * 3;
			if (DataCheckSum((U16 *)(&SREG[0]), i, 0) == 0)
			{
				for (m = 0; m < MAXSREG; m++)
					SREG[m] = 0;
				SREG[0] = 0;			//文件号
				SREG[2] = 64000 - 0x40; //线条数据启始位置在SREG[32000]位置
				SREG[3] = 6;
				SREG[4] = 0x3880; //每组数据的针数据长最大为80000
				SREG[5] = 1;
				SREG[12] = 0x3e8;  //花样最小长度100mm
				SREG[14] = 0x3e8;  //花样最小宽度100mm
				SREG[16] = 0x454e; //文件名
				SREG[17] = 0x2057;
				SREG[18] = 0x2020;
				SREG[19] = 0x2020;
				SREG[30] = 0x3045; //版本
				SREG[31] = 0x3536;
				g_draw_p = 0x24;  //绘图数据当前位置指针
				g_line_p = 32000; //线条数据当前位置指针
				g_pat_p = 52000;  //针数据当前位置指针
			}
		}
		wdt(); // 喂狗	2010.3.27

		xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
		yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;

		g_patd.orgX = xorg;
		g_patd.orgY = yorg;

		sameline_f = 0;
		ox = 0;
		oy = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			ox = (S16)(SREG[6]);
			oy = -(S16)(SREG[7]);
		}
#endif

		if ((g_patd.p_2hd < g_patd.feed_sum) && (vn == PAT_P1_ZOOM || vn == PAT_P1))
		{
			i = g_patd.p_2hd + 1;
			j = g_patd.pat_add + i * 2;
			ox += g_patd.X2hd;
			oy += g_patd.Y2hd;
			x = g_patd.X2hd;
			y = g_patd.Y2hd;
		}
		else
		{
			i = 0;
			j = g_patd.pat_add;
			x = 0;
			y = 0;
		}

		// if (vn == PAT_P1_ZOOM)
		// {
		// 	DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_CODE_C, 0);
		// }

#if 0
		if (Mtype == 1)
		{
			pat_add_p = g_patd.pat_add;
			j = 0;

			// /[绘制结束点到起点的线]**************************
			if ((SREG[3] == 7) || ((g_patd.p_2hd < g_patd.feed_sum) && (vn == PAT_P1_ZOOM || vn == PAT_P1)))
			{
				if (SREG[3] == 7)
				{
					ox = (S16)(SREG[6]);
					oy = -(S16)(SREG[7]);
				}
				else
				{
					ox = 0;
					oy = 0;
				}
				x1 = (S32)(xorg); // 起点
				y1 = (S32)(yorg);
				if ((g_patd.p_2hd < g_patd.feed_sum) && (vn == PAT_P1_ZOOM || vn == PAT_P1))
				{
					x2 = (S32)((ox + g_patd.X2hd) * scale / 1000 + xorg);
					y2 = (S32)((oy + g_patd.Y2hd) * scale / 1000 + yorg); // 终点
				}
				else
				{
					x2 = (S32)(ox * scale / 1000 + xorg);
					y2 = (S32)(oy * scale / 1000 + yorg);
				}
#if 0 
				DrawLine(x1, y1, x2, y2, COLOR_LINE_ED2H); //结束点回到起点
#endif
			}
			else
			{
				ox = 0;
				oy = 0;
			}

			// /[绘制缝纫、空送连线]**************************
			x = 0;
			y = 0;
			if ((g_patd.p_2hd < g_patd.feed_sum) && (vn < 2))
			{
				pat_add_p += ((g_patd.p_2hd + 1) * 2);
				j = g_patd.p_2hd + 1;
				ox += g_patd.X2hd;
				oy += g_patd.Y2hd;
				x = g_patd.X2hd;
				y = g_patd.Y2hd;
			}

			x = 0;
			y = 0;
			for (k = 0; k < g_patd.feed_sum; k++)
			{
				dat1 = SREG[pat_add_p];
				dat = SREG[pat_add_p + 1];
				x1 = (U8)(dat);
				y1 = (U8)((U16)(dat) >> 8);
				if (x1 >= 0x80)
				{
					x1 -= 0x80;
					x -= x1;
				}
				else
					x += x1;
				if (y1 >= 0x80)
				{
					y1 -= 0x80;
					y -= y1;
				}
				else
					y += y1;
				x = ox + x;
				y = oy - y;
				x1 = (S32)(ox * scale / 1000 + xorg);
				y1 = (S32)(oy * scale / 1000 + yorg);
				x2 = (S32)(x * scale / 1000 + xorg);
				y2 = (S32)(y * scale / 1000 + yorg);

				if ((dat1 == 0x3) || (dat1 == 0x1b))
				{
#if 0 
					DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY); //空送

#endif
				}
				else
				{
#if 0
					// DrawLine(x1, y1, x2, y2, sewColor); //缝纫
					DrawLine(x1, y1, x2, y2, sewColor); //缝纫
#endif
				}

				if ((SREG[pat_add_p] == 0x031d) && (g_UdZhouFlag == 3)) //换针
				{
					if (sewColor == COLOR_LINE_SEW)
						sewColor = COLOR_LINE_CHANGE;
					else
						sewColor = COLOR_LINE_SEW;
				}
				ox = x;
				oy = y;
				x = 0;
				y = 0;

				pat_add_p += 2;
			}
		}
#endif
		if (Mtype == 0)
		{
			for (; i < g_patd.feed_sum; i++) // 以下为空送线条显示
			{
				if (((SREG[j] == 0x3) || (SREG[j] == 0x1b)) && (sameline_f == 0))
				{
					sameline_f = 1;
					k1 = ox;
					k2 = oy;
				}
				if (((SREG[j] & 0x00ff) == 0x1) || ((SREG[j] & 0x00ff) == 0x21) || ((SREG[j] & 0x00ff) == 0x41) ||
					((SREG[j] & 0x00ff) == 0x61) || ((SREG[j] & 0x00ff) == 0x3) || ((SREG[j] & 0x00ff) == 0x1b))
				{
					ch = (U8)(SREG[j + 1] & 0xff);
					if (ch >= 0x80)
						xtmp = (ch - 0x80) * (-1);
					else
						xtmp = ch;
					ch = (SREG[j + 1] >> 8) & 0xff;
					if (ch >= 0x80)
						ytmp = (ch - 0x80) * (-1);
					else
						ytmp = ch;
					x = ox + xtmp;
					y = oy - ytmp;
					if ((sameline_f == 1) && (SREG[j] == 0x1b)) //移送，空松
					{
						sameline_f = 0;
						x1 = (S32)(k1 * scale / 1000 + xorg);
						y1 = (S32)(k2 * scale / 1000 + yorg);
						x2 = (S32)(x * scale / 1000 + xorg);
						y2 = (S32)(y * scale / 1000 + yorg);
#if 0
						DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY);
#endif
					}
					ox = x;
					oy = y;
				}
				j += 2;
				wdt(); // 喂狗
			}

			j = g_patd.draw_add;
			line_add_p = g_patd.line_add;
			pat_add_p = g_patd.pat_add;
			if (g_init_flag == 1)
			{									 // 完整的花样数据
				temp_line_add = g_patd.line_add; // 线条起始地址，用于限制绘图数据的范围
				temp_pat_add = g_patd.pat_add;	 // 针起始地址，用于限制线条数据的范围
			}
			else
			{							  // 入力菜单中，正在输入花样数据
				temp_line_add = g_draw_p; // 线条起始地址，用于限制绘图数据的范围
				temp_pat_add = g_line_p;  // 针起始地址，用于限制线条数据的范围
			}
			wdt(); // 喂狗

			// /[绘制结束点到起点的线]**************************
			if ((SREG[3] == 7) || ((g_patd.p_2hd < g_patd.feed_sum) && (vn == PAT_P1 || vn == PAT_P1_ZOOM)))
			{
				ox = 0;
				oy = 0;
#if PATRULES_ORG_OFFSET
				if (SREG[3] == 7)
				{
					ox = (S16)(SREG[6]);
					oy = -(S16)(SREG[7]);
				}
#endif

				x1 = (S32)(xorg);
				y1 = (S32)(yorg);
				if ((g_patd.p_2hd < g_patd.feed_sum) && (vn < 2))
				{
					x2 = (S32)((ox + g_patd.X2hd) * scale / 1000 + xorg);
					y2 = (S32)((oy + g_patd.Y2hd) * scale / 1000 + yorg);
				}
				else
				{
					x2 = (S32)(ox * scale / 1000 + xorg);
					y2 = (S32)(oy * scale / 1000 + yorg);
				}
#if 0
				DrawLine(x1, y1, x2, y2, COLOR_LINE_ED2H); //原点到次原点
#endif
			}
			else
			{
				ox = 0;
				oy = 0; // 起点
			}

			// /[绘制缝纫、空送连线]**************************
			x = 0;
			y = 0; // 终点
			if ((g_patd.p_2hd < g_patd.feed_sum) && (vn == PAT_P1 || vn == PAT_P1_ZOOM))
			{
				for (; j < temp_line_add; j += 4)
				{
					if (SREG[j] == 0x601)
					{
						j += 4;
						break;
					}
				}
				line_add_p += (g_patd.p_2hd + 1);
				pat_add_p += ((g_patd.p_2hd + 1) * 2);
				ox += g_patd.X2hd;
				oy += g_patd.Y2hd;
				x = g_patd.X2hd;
				y = g_patd.Y2hd;
			}

			g_array3[0][0] = ox;
			g_array3[0][1] = oy;
			if ((vn == PAT_VIEW_CONVENT) && ((g_para1 == 6) || (g_para1 == 7)))
				g_array5[0][0] = 0;
			while (j < temp_line_add)
			{
				wdt(); // 喂狗
				code = SREG[j];
				// code1 = SREG[j + 1];
				dat = code & 0x3ff; ////////////////////////////////////////////+++++++++++++++++++

				if ((vn == PAT_VIEW_CONVENT) && ((code & 0xfe00) == 0x1800))
				{ // 记录多重缝、补助缝的启点位置及编号
					if (g_para1 == 6)
					{
						xx = ox;
						yy = oy;
						if ((xx == 0) && (yy == 0))
							xx = 1;
					}
					else
					{
						xx = 0;
						yy = 0;
					}
				}
				else if ((vn == PAT_VIEW_CONVENT) && (g_para1 == 7) && (code & 0x9800))
				{
					xx = ox;
					yy = oy;
					if ((xx == 0) && (yy == 0))
						xx = 1;
				}
				else
				{
					xx = 0;
					yy = 0;
				}

				switch (dat)
				{
				case 0x0088: // 移送
					if (SREG[j + 2] > 0x8000)
						xtmp = (0x10000 - SREG[j + 2]) * (-1);
					else
						xtmp = SREG[j + 2];
					if (SREG[j + 3] > 0x8000)
						ytmp = (0x10000 - SREG[j + 3]) * (-1);
					else
						ytmp = SREG[j + 3];
					g_array3[0][0] += xtmp;
					g_array3[0][1] -= ytmp;
					dat = (SREG[line_add_p] & 0x3fff);
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);
						if (dat != dat1)
							break;
						k++;
					}
					n = k - line_add_p; //针数据的个数
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						pat_add_p += 2;
					}
					x = ox + x;
					y = oy - y;
					ox = x;
					oy = y;
					xx = 0;
					yy = 0;
					break;
				case 0x00c4: // 点
					if (SREG[j + 2] > 0x8000)
						xtmp = (0x10000 - SREG[j + 2]) * (-1);
					else
						xtmp = SREG[j + 2];
					if (SREG[j + 3] > 0x8000)
						ytmp = (0x10000 - SREG[j + 3]) * (-1);
					else
						ytmp = SREG[j + 3];
					x = ox + xtmp;
					y = oy - ytmp;
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
#if 0
// DrawLine(x1, y1, x2, y2, sewColor);

					DrawLine(x1, y1, x2, y2, sewColor);

#endif
					line_add_p++;
					pat_add_p += 2;
					ox = x;
					oy = y;
					g_array3[0][0] = ox;
					g_array3[0][1] = oy;
					xx = 0;
					yy = 0;
					break;
				case 0x00c8: // 直线
				case 0x00ca: // 直线_Z字缝
				case 0x01ce: // 园_Z字缝
				case 0x01d2: // 园弧_Z字缝
				case 0x00d6: // 曲线结束_Z字缝
					dat = (SREG[line_add_p] & 0x3fff);
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);
						if (dat != dat1)
							break;
						k++;
					}
					n = k - line_add_p;
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat1 = SREG[pat_add_p];
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						dat = SREG[pat_add_p + 2];
						if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
						{
							dat = SREG[pat_add_p + 3];
							x2 = (U8)(dat);
							y2 = (U8)((U16)(dat) >> 8);
							if (((abs((S32)(x1)-x2)) < 2) && ((abs((S32)(y1)-y2)) < 2))
							{
								if ((k + 1) < n)
									sameline_f = 1;
								else
									sameline_f = 0;
							}
							else
							{
								sameline_f = 0;
							}
						}
						else
						{
							sameline_f = 0;
						}
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						if (sameline_f == 0)
						{
							x = ox + x;
							y = oy - y;
							x1 = (S32)(ox * scale / 1000 + xorg);
							y1 = (S32)(oy * scale / 1000 + yorg);
							x2 = (S32)(x * scale / 1000 + xorg);
							y2 = (S32)(y * scale / 1000 + yorg);
							if ((dat1 == 0x3) || (dat1 == 0x1b))
							{
								//DrawLine(x1, y1, x2, y2,Color_DimGray);
							}
							else
							{
#if 0
								// DrawLine(x1, y1, x2, y2, sewColor);
								DrawLine(x1, y1, x2, y2, sewColor);

#endif
							}
							ox = x;
							oy = y;
							x = 0;
							y = 0;
						}
						pat_add_p += 2;
					}
					g_array3[0][0] = ox;
					g_array3[0][1] = oy;
					break;
				case 0x00ce: // 园结束_Z字缝
				case 0x00d2: // 园弧结束_Z字缝
				case 0x01d6: // 曲线_Z字缝
				case 0x02d6: // 曲线_Z字缝
					xx = 0;
					yy = 0;
					break;
				case 0x01cc: // 园
				case 0x01d0: // 园弧
					dat1 = (SREG[j + 4]) & 0x1ff;
					if (dat != (dat1 + 0x100))
						break;
					/////////////////////////////////////////  2011.4.11
					dat3 = code & 0xfe00;
					if (dat3 == 0x800)
						dat3 = 1;
					else if (dat3 == 0x1000)
						dat3 = 2;
					else if (dat3 == 0x1800)
						dat3 = 3;
					else if (dat3 == 0x8800)
						dat3 = 4;
					else if (dat3 == 0x9000)
						dat3 = 5;
					else
						dat3 = 0;

					//三菱G版的数据
					dat = (SREG[line_add_p] & 0x3fff);
					k = line_add_p;
					while (k < temp_pat_add)
					{
						dat1 = (SREG[k] & 0x3fff);
						if (dat != dat1)
							break;
						k++;
					}
					n = k - line_add_p;
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat1 = SREG[pat_add_p];
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						dat = SREG[pat_add_p + 2];
						if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
						{
							dat = SREG[pat_add_p + 3];
							x2 = (U8)(dat);
							y2 = (U8)((U16)(dat) >> 8);
							sameline_f = 0;
						}
						else
						{
							sameline_f = 0;
						}
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						if (sameline_f == 0)
						{
							x = ox + x;
							y = oy - y;
							x1 = (S32)(ox * scale / 1000 + xorg);
							y1 = (S32)(oy * scale / 1000 + yorg);
							x2 = (S32)(x * scale / 1000 + xorg);
							y2 = (S32)(y * scale / 1000 + yorg);
							if ((dat1 == 0x3) || (dat1 == 0x1b)) //空送
							{
							}
							else
							{
#if 0
								// DrawLine(x1, y1, x2, y2, sewColor);
								DrawLine(x1, y1, x2, y2, sewColor);
#endif
							}
							ox = x;
							oy = y;
							x = 0;
							y = 0;
						}
						pat_add_p += 2;
					}

					g_array3[0][0] = ox;
					g_array3[0][1] = oy;
					break;
				case 0x00cc: // 园结束
				case 0x00d0: // 园弧结束
					xx = 0;
					yy = 0;
					break;
				case 0x01d4: // 曲线
				case 0x00d4:
				case 0x02d4:
					if (Mtype == 1) //三菱G版的数据
					{
					}
					else
					{
						if (((SREG[j - 4] != 0x01d4) && (dat != 0x01d4)) || (dat == 0x01d4))
						{ //2009.6.16 ,修改了单独0x0d4,0x2d4开头不能正常显示的问题
							n = 1;
							if (dat != 0xd4)
							{
								while ((j + 4) < temp_line_add)
								{
									dat = (SREG[j + 4]) & 0x3fd;
									if ((dat != 0x1d4) && (dat != 0x2d4))
									{
										break;
									}
									n++;
									j += 4;
								}
								n++;
								j += 4;
							}

							if ((SREG[j] & 0x1fc) != 0xd4)
								break;

							dat = (SREG[line_add_p] & 0x3fff);
							k = line_add_p;
							while (k < temp_pat_add)
							{
								dat1 = (SREG[k] & 0x3fff);
								if (dat != dat1)
									break;
								k++;
							}
							n = k - line_add_p;
							line_add_p = k;
							x = 0;
							y = 0;
							for (k = 0; k < n; k++)
							{
								dat1 = SREG[pat_add_p];
								dat = SREG[pat_add_p + 1];
								x1 = (U8)(dat);
								y1 = (U8)((U16)(dat) >> 8);
								dat = SREG[pat_add_p + 2];
								if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
								{
									dat = SREG[pat_add_p + 3];
									x2 = (U8)(dat);
									y2 = (U8)((U16)(dat) >> 8);
									sameline_f = 0;
								}
								else
								{
									sameline_f = 0;
								}
								if (x1 >= 0x80)
								{
									x1 -= 0x80;
									x -= x1;
								}
								else
									x += x1;
								if (y1 >= 0x80)
								{
									y1 -= 0x80;
									y -= y1;
								}
								else
									y += y1;
								if (sameline_f == 0)
								{
									x = ox + x;
									y = oy - y;
									x1 = (S32)(ox * scale / 1000 + xorg);
									y1 = (S32)(oy * scale / 1000 + yorg);
									x2 = (S32)(x * scale / 1000 + xorg);
									y2 = (S32)(y * scale / 1000 + yorg);
									if ((dat1 == 0x3) || (dat1 == 0x1b))
									{
									}
									else
									{
#if 0
										// DrawLine(x1, y1, x2, y2, sewColor);
										DrawLine(x1, y1, x2, y2, sewColor);
#endif
									}
									ox = x;
									oy = y;
									x = 0;
									y = 0;
								}
								pat_add_p += 2;
							}
							g_array3[0][0] = ox;
							g_array3[0][1] = oy;
						}
						else
						{
							xx = 0;
							yy = 0;
						}
					}
					break;
				case 0x00d8:		// 多边形
				case 0x01d8:		// 多边形结束
				case 0x00da:		// 多边形_Z字缝
				case 0x01da:		// 多边形结束_Z字缝
					if (Mtype == 1) ///////////////////////三菱G版的数据
					{
					}
					else
					{
						i = 1;
						while ((j + 4) < temp_line_add)
						{
							dat = (SREG[j + 4]) & 0xfc;
							if (dat != 0xd8)
							{
								break;
							}
							i++;
							j += 4;
						}
						dat = (SREG[line_add_p] & 0x3fff);
						dat += i;
						k = line_add_p;
						while (k < temp_pat_add)
						{
							dat1 = (SREG[k] & 0x3fff);

							if (dat <= dat1)
								break;
							k++;
						}
					}
					n = k - line_add_p;
					line_add_p = k;
					x = 0;
					y = 0;
					for (k = 0; k < n; k++)
					{
						dat1 = SREG[pat_add_p];
						dat = SREG[pat_add_p + 1];
						x1 = (U8)(dat);
						y1 = (U8)((U16)(dat) >> 8);
						dat = SREG[pat_add_p + 2];
						if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
						{
							dat = SREG[pat_add_p + 3];
							x2 = (U8)(dat);
							y2 = (U8)((U16)(dat) >> 8);
							if (((abs((S32)(x1)-x2)) < 2) && ((abs((S32)(y1)-y2)) < 2))
							{
								if ((k + 1) < n)
									sameline_f = 1;
								else
									sameline_f = 0;
							}
							else
							{
								sameline_f = 0;
							}
						}
						else
						{
							sameline_f = 0;
						}
						if (x1 >= 0x80)
						{
							x1 -= 0x80;
							x -= x1;
						}
						else
							x += x1;
						if (y1 >= 0x80)
						{
							y1 -= 0x80;
							y -= y1;
						}
						else
							y += y1;
						if (sameline_f == 0)
						{
							x = ox + x;
							y = oy - y;
							x1 = (S32)(ox * scale / 1000 + xorg);
							y1 = (S32)(oy * scale / 1000 + yorg);
							x2 = (S32)(x * scale / 1000 + xorg);
							y2 = (S32)(y * scale / 1000 + yorg);
							if ((dat1 == 0x3) || (dat1 == 0x1b))
							{
							}
							else
							{
#if 0
								// DrawLine(x1, y1, x2, y2, sewColor);
								DrawLine(x1, y1, x2, y2, sewColor);
#endif
							}
							ox = x;
							oy = y;
							x = 0;
							y = 0;
						}
						pat_add_p += 2;
					}
					g_array3[0][0] = ox;
					g_array3[0][1] = oy;
					break;
				default:
					xx = 0;
					yy = 0;
					break;
				}
				wdt(); // 喂狗	2010.3.27
				//printf("code = %04x \r\n",code);
				switch (code)
				{
				case 0x0401: // 针上停
				case 0x1401: // 针下停
				case 0x0601: // 次原点
				case 0x0701: // 跳缝
				case 0x1c01: // 伸延=物料厚 压板重 移送速度 重启动
				case 0x3a01: // 面线张力  2013.4.8
				case 0x0201: // 剪线
				case 0x0501: // 功能一
				case 0x0b01: // 功能二
				case 0x0c01: // 功能三
				case 0x0d01: // 功能四
				case 0x3c01: // 针角度
				case 0x3d01: // 参数一
				case 0x3e01: // 参数二
					line_add_p++;
					pat_add_p += 2;
					break;	 // ?
				case 0x1d01: // 功能五 六 七 八
					// if ((code1 == 0x03) && (g_UdZhouFlag == 3))
					// { //换针
					// 	if (sewColor == Color_Blue)
					// 		sewColor = Color_Dsew;
					// 	else
					// 		sewColor = Color_Blue;
					// }
					line_add_p++;
					pat_add_p += 2;
					break; // ?

				case 0x1e01: // 结束
				case 0x1f01: // 结束
					ox = x;
					oy = y;
					break;
				default:
					break;
				}
				j += 4;
				if ((xx != 0) || (yy != 0)) // 记录多重缝、补助缝的启点位置及编号
				{
					nn++;
					g_array5[nn][0] = xx;
					g_array5[nn][1] = yy;
					g_array5[0][0] = nn;
				}
			}
		}

		// 针位绘图***********************
		sameline_f = 0;
		ox = 0;
		oy = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			ox = (S16)(SREG[6]);
			oy = -(S16)(SREG[7]);
		}
#endif

		if ((g_patd.p_2hd < g_patd.feed_sum) && (vn == 1))
		{
			i = g_patd.p_2hd + 1;
			j = g_patd.pat_add + i * 2;
			ox += g_patd.X2hd;
			oy += g_patd.Y2hd;
			x = g_patd.X2hd;
			y = g_patd.Y2hd;
		}
		else
		{
			i = 0;
			j = g_patd.pat_add;
			x = 0;
			y = 0;
		}

		// /[绘制已完成缝纫的点与线]*****************
		for (; i < g_patd.feed_sum; i++)
		{
			dat = SREG[j] & 0x00ff;
			if ((dat == 0x1) || (dat == 0x21) || (dat == 0x41) || (dat == 0x61) || (dat == 0x3) || (dat == 0x1b))
			{
				ch = (U8)(SREG[j + 1] & 0xff);
				if (ch >= 0x80)
					xtmp = (ch - 0x80) * (-1);
				else
					xtmp = ch;
				ch = (SREG[j + 1] >> 8) & 0xff;
				if (ch >= 0x80)
					ytmp = (ch - 0x80) * (-1);
				else
					ytmp = ch;
				x = ox + xtmp;
				y = oy - ytmp;
#if 0
				if ((vn == PAT_P1 || vn == 100) && i < HREG[HREG_FEED_C])
				{
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
				}
				else if ((vn == PAT_VIEW_MODIFY2 || vn == PAT_VIEW_MODIFY3) &&
						 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
						  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
				{
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
				}
#else
				if ((vn == PAT_P1 || vn == 100) && i < HREG[HREG_FEED_C])
				{
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
				}
				else if ((vn == PAT_VIEW_MODIFY2 || vn == PAT_VIEW_MODIFY3) &&
						 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
						  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
				{
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
				}
				else
				{
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					if (((dat & 0xff) == 0x3) || ((dat & 0xff) == 0x1b))
						DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY); //空送
					else
						DrawLine(x1, y1, x2, y2, sewColor); //缝纫
				}
#endif

				// if ((((vn) && (vn < PAT_READ)) || vn == 100) && (dat != 0x3) && (dat != 0x1b)) //不为空送处，绘制针点
				if (((vn)) && (dat != 0x3) && (dat != 0x1b)) //不为空送处，绘制针点
				{
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					// if (vn == 100 && i <= HREG[HREG_FEED_C])
					// {
					// 	ViewRunPoint(x2, y2, COLOR_PIN_FINISH, 0);
					// }
					// else
					// {
					// 	DrawPoint(x2, y2, COLOR_PIN_SEW);
					// }
					if (i <= HREG[HREG_FEED_C])
					{
						if (vn == 100)
						{
							ViewRunPoint(x2, y2, COLOR_PIN_FINISH, 0);
							ViewRunPoint(x1, y1, COLOR_PIN_FINISH, 0);
						}
						else
						{
							DrawPoint(x2, y2, COLOR_PIN_FINISH);
							DrawPoint(x1, y1, COLOR_PIN_FINISH);
						}
					}
					else if ((vn == PAT_VIEW_MODIFY2 || vn == PAT_VIEW_MODIFY3) &&
							 ((i >= g_jog_stpoint && i < g_jog_enpoint) ||
							  (i >= g_jog_stpoint && i < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
					{
						DrawPoint(x2, y2, COLOR_PIN_FINISH);
						DrawPoint(x1, y1, COLOR_PIN_FINISH);
					}
					else
					{
						DrawPoint(x2, y2, COLOR_PIN_SEW);
						DrawPoint(x1, y1, COLOR_PIN_SEW);
					}
				}
				ox = x;
				oy = y;
			}
			j += 2;
			wdt(); // 喂狗
		}

		// /[绘制指令代码]***********************
		if (((vn == PAT_VIEW_MODIFY1 || vn == PAT_MODIFY || vn == PAT_CONVENT ||
			  vn == PAT_VIEW_MODIFY2 || vn == PAT_VIEW_MODIFY3 || vn == PAT_VIEW_CREATE ||
			  vn == PAT_VIEW_CONVENT) &&
			 g_CodeEn == 1))
		{
			DrawFPBMPBTN2(xorg - CODE_BMP_HX, yorg - CODE_BMP_HY, CODE_BMP_X, CODE_BMP_Y, BMP_H);
			ox = 0;
			oy = 0;
#if PATRULES_ORG_OFFSET
			if (SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}
#endif
			x = 0;
			y = 0;
			i = g_patd.pat_add;
			for (k = 0; k < g_patd.feed_sum; k++)
			{
				dat1 = SREG[i] & 0xff;
				if ((dat1 == 0x1) || (dat1 == 0x21) || (dat1 == 0x41) || (dat1 == 0x61) || (dat1 == 0x3) || (dat1 == 0x1b))
				{
					dat = SREG[i + 1];
					x1 = (U8)(dat);
					y1 = (U8)((U16)(dat) >> 8);
					if (x1 >= 0x80)
					{
						x1 -= 0x80;
						x -= x1;
					}
					else
						x += x1;
					if (y1 >= 0x80)
					{
						y1 -= 0x80;
						y -= y1;
					}
					else
						y += y1;
					x = ox + x;
					y = oy - y;

					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					ox = x;
					oy = y;
					x = 0;
					y = 0;
					//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
					if (dat1 == 0x1 || dat1 == 0x21 || dat1 == 0x41 || dat1 == 0x61)
					{
						dat1 = (SREG[i] >> 8) & 0xff;
						if (dat1 != 0)
						{
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_P);
						}
					}
				}
				else
				{
					switch (dat1)
					{
					case 0x06:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_H2);
						break;
					case 0x3a:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_T);
						break;
					case 0x04:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_U);
						break;
					case 0x14:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_D);
						break;
					case 0x07:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_J);
						break;
					case 0x05:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F1);
						break;
					case 0x0b:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F2);
						break;
					case 0x0c:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F3);
						break;
					case 0x0d:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F4);
						break;
					case 0x1d:
						dat1 = (SREG[i] >> 8) & 0xff;
						switch (dat1)
						{
						case 0x00:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F5);
							break;
						case 0x01:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F6);
							break;
						case 0x02:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F7);
							break;
						case 0x03:
							DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_F8);
							break;
						}
						break;
					case 0x02:
						DrawFPBMPBTN2(x2 - 7, y2 - 7, 14, 14, BMP_C);
						break;
					}
				}
				wdt();
				i += 2;
			}
		}

		// /[显示调整的相关代码]*****************
		if ((((vn == PAT_VIEW_CREATE) || (vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3) ||
			  (vn == PAT_VIEW_CONVENT) || (vn == PAT_CREATE)) &&
			 g_CodeEn == 1) ||
			((vn == PAT_P1_ZOOM) && (g_feedct)))
		{
			if (vn == PAT_VIEW_CREATE)
			{ // 入力菜单中调用显示
				x = g_oxpos;
				y = -g_oypos;
				xx = 0;
				yy = 0;
#if PATRULES_ORG_OFFSET
				if (SREG[3] == 7)
				{
					xx = (S16)(SREG[6]);
					yy = -(S16)(SREG[7]);
					xx = (S32)(xx * scale / 1000);
					yy = (S32)(yy * scale / 1000);
					x += (S16)(SREG[6]);
					y -= (S16)(SREG[7]);
				}
#endif
				ox = x;
				oy = y;
				n = g_array4[0][0] + 1;
				for (k = 0; k < n; k++)
				{
					if (k == g_array4[0][0])
					{
						x += g_dxpos;
						y -= g_dypos;
					}
					else
					{
						x += (S16)(g_array4[k + 1][0]);
						y -= (S16)(g_array4[k + 1][1]);
					}
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);
					DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW);
					ox = x;
					oy = y;
				}
				if ((g_patd.feed_sum) && (x == 0) && (y == 0))
				{
					DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_ORG, 0);
					DrawSerialNumber(xorg + xx, yorg + yy, 3, colorf, COLOR_CODE_C, 0);
				}
				else
				{
					DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_ORG, 0);
					if ((x != 0) || (y != 0))
						DrawSerialNumber(x2, y2, 3, colorf, COLOR_CODE_C, 0); //RGB(224, 87, 38)
				}
			}
			else
			{ // 修改、变换菜单中调用显示	vn=6区段移动 vn=7区段修改 vn=5其他修改、变换
				xx = 0;
				yy = 0;
				ox = 0;
				oy = 0;
#if PATRULES_ORG_OFFSET
				if (SREG[3] == 7)
				{
					xx = (S16)(SREG[6]);
					yy = -(S16)(SREG[7]);
					xx = (S32)(xx * scale / 1000);
					yy = (S32)(yy * scale / 1000);
					ox = (S16)(SREG[6]);
					oy = -(S16)(SREG[7]);
				}
#endif

				DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_ORG, 0);
				if ((vn == PAT_VIEW_MODIFY1) && (0 == g_jog_point))
				{
					DrawSerialNumber(xorg + xx, yorg + yy, 3, colorf, COLOR_CODE_C, 0);
				}
				else if (((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (((0 == g_jog_stpoint) && (g_sta_end_flag)) || ((0 == g_jog_point) && (g_sta_end_flag == 0))))
				{
					DrawSerialNumber(xorg + xx, yorg + yy, 1, colorf, COLOR_CODE_C, 0);
				}
				else if (((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (((0 == g_jog_point) && (g_sta_end_flag) == 1) || ((0 == g_jog_enpoint) && (g_sta_end_flag == 4))))
				{
					DrawSerialNumber(xorg + xx, yorg + yy, 2, colorf, COLOR_CODE_C, 0);
				}

				// /[记录点]*****************
				x = 0;
				y = 0;
				k1 = 0;
				k2 = 0;
				j = g_patd.pat_add;
				for (i = 1; i <= g_patd.feed_sum; i++)
				{
					if (((SREG[j] & 0x00ff) == 0x1) || ((SREG[j] & 0x00ff) == 0x21) || ((SREG[j] & 0x00ff) == 0x41) || ((SREG[j] & 0x00ff) == 0x61) || ((SREG[j] & 0x00ff) == 0x3) || ((SREG[j] & 0x00ff) == 0x1b))
					{
						ch = (U8)(SREG[j + 1] & 0xff);
						if (ch >= 0x80)
							xtmp = (ch - 0x80) * (-1);
						else
							xtmp = ch;
						ch = (SREG[j + 1] >> 8) & 0xff;
						if (ch >= 0x80)
							ytmp = (ch - 0x80) * (-1);
						else
							ytmp = ch;
						x = ox + xtmp;
						y = oy - ytmp;
						if (vn == PAT_P1_ZOOM)
						{
							//if(i==g_feedct){
							//	x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
							//		DrawSerialNumber(x2,y2,3,colorf,RGB(224,87,38),0);
							//	}
						}
						else if (vn == PAT_VIEW_MODIFY1)
						{	// 修改 变换菜单中单个选择点时调用显示
							//	if(i==g_jog_point){
							//				x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
							//					DrawSerialNumber(x2,y2,3,colorf,RGB(224,87,38));
							//			}
						}
						else if ((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3))
						{ // 修改菜单中两个选择点(区段)时调用显示	 //////////////////////////////???????????????????
							if (i == g_jog_stpoint)
							{
								//								x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
								//								if((vn==7)&&(g_jogmode==1)){
								//									k1 = x;k2 = y;
								//								}
								//								DrawSerialNumber(x2,y2,1,colorf,RGB(224,87,38));
							}
							else if (i == g_jog_enpoint)
							{
								//								x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
								//								DrawSerialNumber(x2,y2,2,colorf,RGB(224,87,38));
							}
							else if (i == g_jog_point)
							{
								//								x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
								//								DrawSerialNumber(x2,y2,3,colorf,RGB(224,87,38));
							}
							if ((vn == PAT_VIEW_MODIFY3) && (g_jogmode == 0))
							{
								for (k = 1; k < g_point_num; k++)
								{
									if (i == g_array5[300 + k][0])
									{
										k1 = (S16)(g_array5[k][0]);
										k2 = (S16)(g_array5[k][1]);
										x2 = (S32)((x + k1) * scale / 1000 + xorg);
										y2 = (S32)((y - k2) * scale / 1000 + yorg);
										DrawSerialNumber2(x2, y2, k, colorf, COLOR_CTRL);
									}
								}
								if (i == g_jog_point)
								{ // array5[15+point_num][1]	 (point_num==1)&&
									if (bpno1 != MENU_MODIFYADD)
									{
										k1 = g_dxpos;
										k2 = g_dypos;
									}
									else
									{
										k1 = 0;
										k2 = 0;
									}
									x2 = (S32)((x + k1) * scale / 1000 + xorg);
									y2 = (S32)((y - k2) * scale / 1000 + yorg);
									tmpx3 = x2;
									tmpy3 = y2;
									disP1 = 1;
									//	DrawSerialNumber(x2,y2,3,colorf,RGB(224,87,38),0);
								}
							}
						}
						else if (vn == PAT_VIEW_CONVENT)
						{
							if (((g_para1 == 3) || (g_para1 == 5)) && (g_datum_mark == 0))
							{
								if (i == g_jog_point)
								{
									k1 = g_dxpos;
									k2 = g_dypos;
									x2 = (S32)((x + k1) * scale / 1000 + xorg);
									y2 = (S32)((y - k2) * scale / 1000 + yorg);
									DrawSerialNumber(x2, y2, 3, colorf, COLOR_CODE_C, 0);
								}
							}
						}
						ox = x;
						oy = y;
					}

					if (i == g_jog_stpoint)
					{ //记录开始点
						if ((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3))
						{
							disS = 1;
							tmpxS = (S32)(x * scale / 1000 + xorg);
							tmpyS = (S32)(y * scale / 1000 + yorg);
							if ((vn == PAT_VIEW_MODIFY3) && (g_jogmode == 1))
							{
								k1 = x;
								k2 = y;
							}
						}
					}

					if (i == g_jog_enpoint)
					{ //记录结束点
						if ((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3))
						{
							disE = 1;
							tmpxE = (S32)(x * scale / 1000 + xorg);
							tmpyE = (S32)(y * scale / 1000 + yorg);
						}
					}
					if (i == g_jog_point)
					{ //记录当前点位置
						if ((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3))
						{
							disP = 1;
							tmpx2 = (S32)(x * scale / 1000 + xorg);
							tmpy2 = (S32)(y * scale / 1000 + yorg);
						}
					}

					if (i == g_feedct)
					{ //记录当前点位置
						if (vn == PAT_P1_ZOOM)
						{
							disP = 1;
							tmpx2 = (S32)(x * scale / 1000 + xorg);
							tmpy2 = (S32)(y * scale / 1000 + yorg);
						}
					}

					//记录选中点
					if (((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (g_ImgPoint == 1))
					{
						txpoint = (S32)(x * scale / 1000 + xorg);
						typoint = (S32)(y * scale / 1000 + yorg);
						if ((i) == *infeed)
						{
							xpoint1 = txpoint;
							ypoint1 = typoint;
						}
						if ((((abs(txpoint - inxpos) * abs(txpoint - inxpos) + abs(typoint - inypos) * abs(typoint - inypos)) < (disxpoint * disxpoint + disypoint * disypoint))) && (i >= g_min_feedsum) && (i <= g_max_feedsum))
						{
							xpoint = txpoint;
							ypoint = typoint;
							disxpoint = abs(xpoint - inxpos);
							disypoint = abs(ypoint - inypos);
							tmpfeed1 = i;
						}
					}
					j += 2;
					wdt(); // 喂狗
				}
				// /[显示选中点]*****************
				if (((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (g_ImgPoint == 1))
				{

					if ((abs(xpoint - inxpos) < 10) && (abs(ypoint - inypos) < 10) &&
						((inxpos != 0) || (inypos != 0)) && (tmpfeed1 >= g_min_feedsum) && (tmpfeed1 <= g_max_feedsum))
					{

						DrawBigPoint(xpoint, ypoint, COLOR_SELECT_P);
						*infeed = tmpfeed1;
					}
					else
					{

						DrawBigPoint(xpoint1, ypoint1, COLOR_SELECT_P);
					}
				}

				if ((vn == PAT_VIEW_MODIFY3) && (g_jogmode == 1))
				{
					for (k = 1; k < g_point_num; k++)
					{
						k1 += (S16)(g_array5[k][0]);
						k2 -= (S16)(g_array5[k][1]);
						x2 = (S32)(k1 * scale / 1000 + xorg);
						y2 = (S32)(k2 * scale / 1000 + yorg);
						DrawSerialNumber2(x2, y2, k, colorf, COLOR_CTRL);
					}
					// if ((g_dxpos != 0) || (g_dypos != 0))
					// {
					// 	k1 += g_dxpos;
					// 	k2 -= g_dypos;
					// 	x2 = (S32)(k1 * scale / 1000 + xorg);
					// 	y2 = (S32)(k2 * scale / 1000 + yorg);
					// 	DrawSerialNumber(x2, y2, 3, colorf, COLOR_CTRL, 0);
					// }
				}
				else if ((vn == PAT_VIEW_CONVENT) && ((g_para1 == 3) || (g_para1 == 5)) && (g_datum_mark))
				{
					k1 += g_dxpos;
					k2 -= g_dypos;
					x2 = (S32)(k1 * scale / 1000 + xorg);
					y2 = (S32)(k2 * scale / 1000 + yorg);
					DrawSerialNumber(x2, y2, 3, colorf, COLOR_CODE_C, 0);
				}

				if (disS)
				{ //显示起始点位置
					DrawSerialNumber(tmpxS, tmpyS, 1, colorf, COLOR_CODE_C, 0);
				}
				if (disE)
				{ //显示结束点位置
					DrawSerialNumber(tmpxE, tmpyE, 2, colorf, COLOR_CODE_C, 0);
				}
				if (disP)
				{ //显示当前点位置
					DrawSerialNumber(tmpx2, tmpy2, 3, colorf, COLOR_CODE_C, ((g_ImgPoint == 1) || (vn == PAT_P1_ZOOM)) ? 1 : 0);
					if (disP1)
					{
						if ((tmpx2 == tmpx3) && (tmpy2 == tmpy3))
							disP1 = 0;
					}
				}
				//修改 盖写H字符
				if ((vn == PAT_VIEW_MODIFY1) && (0 == g_jog_point))
				{
					DrawSerialNumber(xorg + xx, yorg + yy, 0, colorf, COLOR_ORG, 0);
				}

				if (disP1)
				{
					DrawSerialNumber(tmpx3, tmpy3, 3, colorf, COLOR_CODE_C, 0);
				}
			}
		}
		else
		{
			// DrawHLine(xorg - 1, yorg, 3, Color_Green);
			// DrawVLine(xorg, yorg - 1, 3, Color_Green);
			DrawPoint(xorg, yorg, COLOR_ORG);
		}
		if ((vn == PAT_VIEW_CONVENT) && ((g_para1 == 6) || (g_para1 == 7)) && (g_array5[0][0]))
		{
			for (i = 1; i <= g_array5[0][0]; i++)
			{
				xx = (S16)(g_array5[i][0]);
				yy = (S16)(g_array5[i][1]);
				xx = (S32)(xx * scale / 1000 + xorg);
				yy = (S32)(yy * scale / 1000 + yorg);
				DrawSerialNumber2(xx, yy, i, colorf, COLOR_CTRL);
			}
		}
		SetDrawXY(0, 0, LCD_width, LCD_height, 0);

#endif
	}
	else if (PatType == PAT_SEW)
	{ //兄弟款

		f = Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		if (f == 0)
			return; //测试修改

		if ((vn == PAT_VIEW_CREATE) || (vn == PAT_VIEW_MODIFY3))
		{
			xorg -= (tmpxmax + tmpxmin) / 2 * scale / 1000;
			yorg += (tmpymax + tmpymin) / 2 * scale / 1000;
		}
		else
		{
			xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
			yorg += (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;
		}

		DrawPoint(xorg, yorg, COLOR_ORG);

		// if ((vn == PAT_VIEW_CREATE) || (vn == PAT_P1_ZOOM)||
		// 	(vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3) || (vn == PAT_VIEW_CONVENT))
		// {
		// 	DrawSerialNumber(xorg, yorg, 0, colorf, COLOR_ORG, 0);
		// }

		// /[绘制原点到次原点************************]
		xst = g_patd.orgX;
		yst = g_patd.orgY;
		xend = xst;
		yend = yst;
		// PutPixel1(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PAT_PIN_FINISH);
		if ((xorg != (xorg + xst * scale / 1000)) || (yorg != (yorg - yst * scale / 1000)))
		{
			//次原点
			if ((vn == PAT_P1 || vn == 100) && (HREG[HREG_FEED_C] || g_BorSTflag))
			{
				// DrawLine(xorg, yorg, xorg + xst * scale / 1000, yorg - yst * scale / 1000, Color_Red);
				DrawLine(xorg, yorg, xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_LINE_H2H2);
			}
			else
			{
				// DrawLine(xorg, yorg, xorg + xst * scale / 1000, yorg - yst * scale / 1000, Color_Orange);
				DrawLine(xorg, yorg, xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_LINE_H2H2);
			}
		}

		// /[针数据段****************************]
		for (i = 0; i < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
			if (f)
			{
				tmpfeed++;
				xend = xst;
				yend = yst;

				if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
				{
					switch ((sewIcmd.cmd & 0xf000))
					{
					case 0x2000: //空送
					{
						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;
						if ((vn == PAT_P1 || vn == 100) && tmpfeed <= HREG[HREG_FEED_C])
						{
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, Color_Red);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
						}
						else if ((vn == PAT_VIEW_MODIFY2 || vn == PAT_VIEW_MODIFY3) &&
								 ((tmpfeed > g_jog_stpoint && tmpfeed <= g_jog_enpoint) ||
								  (tmpfeed >= g_jog_stpoint && tmpfeed < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))))
						{
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, Color_Red);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
						}
						else
						{
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, Color_DimGray);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_EMPTY);
						}

						xst = xend;
						yst = yend;
					}
					break;

					case 0x6000: //缝纫
					{
						xend = xst;
						yend = yst;

						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;

						if ((vn == PAT_P1 || vn == 100) && tmpfeed <= HREG[HREG_FEED_C])
						{
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, Color_Red);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
						}
						else if ((vn == PAT_VIEW_MODIFY2 || vn == PAT_VIEW_MODIFY3) &&
								 ((tmpfeed > g_jog_stpoint && tmpfeed <= g_jog_enpoint) ||
								  (tmpfeed >= g_jog_stpoint && tmpfeed < g_jog_point && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4))))) //新增
						{
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, Color_Red);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
						}
						else
						{
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, sewColor);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, sewColor);
						}
						//printf("vposx = %d vposy = %d\r\n",xorg + xend*scale/1000,yorg - yend*scale/1000);
						if ((vn == PAT_P1) || (vn == 1 || vn == 15) ||
							(vn == 2) || (vn == 3) || (vn == 4) || (vn == 5) ||
							(vn == 6) || (vn == 7) || (vn == 8) || (vn == 18) || (vn == 9) || (vn == 100))
						{
							if ((vn == PAT_P1 || vn == 100) && tmpfeed <= HREG[HREG_FEED_C])
							{
								// ViewRunPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, Color_Red, 0);
								ViewRunPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_FINISH, 0);
								// SetDrawXY(104, 126, 546, 398, 1);
								// SetDrawXY(CONVAS_PX1, CONVAS_PY1, CONVAS_PX1 + CONVAS_X1, CONVAS_PY1 + CONVAS_Y1, 1);
							}
							else
							{
								// DrawHLine(xorg + xst * scale / 1000 - 1, yorg - yst * scale / 1000, 3, Color_Black); //绘制针位点
								// DrawVLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000 - 1, 3, Color_Black);

								// DrawHLine(xorg + xend * scale / 1000 - 1, yorg - yend * scale / 1000, 3, Color_Black); //绘制针位点
								// DrawVLine(xorg + xend * scale / 1000, yorg - yend * scale / 1000 - 1, 3, Color_Black);

								DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_SEW); //绘制针位点

								DrawPoint(xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_PIN_SEW); //绘制针位点
							}
						}
						//									 }
						xst = xend;
						yst = yend;
					}
					break;
					}
				}
				else if ((sewIcmd.cmd & 0xff00) == 0xfc00)
				{																   //附加功能
					if (((sewIcmd.cmd & 0xff80) == 0xfc80) && (g_UdZhouFlag == 3)) //换针
					{
						if (sewColor == COLOR_LINE_SEW)
							sewColor = COLOR_LINE_CHANGE;
						else
							sewColor = COLOR_LINE_SEW;
					}
				}
				if (((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (g_ImgPoint == 1))
				{
					txpoint = xorg + xend * scale / 1000;
					typoint = yorg - yend * scale / 1000;
					if ((tmpfeed) == *infeed)
					{
						xpoint1 = txpoint;
						ypoint1 = typoint;
					}

					if ((((abs(txpoint - inxpos) * abs(txpoint - inxpos) + abs(typoint - inypos) * abs(typoint - inypos)) < (disxpoint * disxpoint + disypoint * disypoint))) && (tmpfeed >= g_min_feedsum) && (tmpfeed <= g_max_feedsum))
					{
						xpoint = txpoint;
						ypoint = typoint;
						disxpoint = abs(xpoint - inxpos);
						disypoint = abs(ypoint - inypos);
						tmpfeed1 = tmpfeed;
					}
				}
				i += sewIcmd.off;
				if (vn == PAT_P1_ZOOM)
				{
					if ((tmpfeed == HREG[HREG_FEED_C]) && (HREG[HREG_FEED_C] != 0))
					{
						tmpxpos = xend;
						tmpypos = yend;
						if (tmpfeed >= g_patd.feed_sum)
						{
							tmpxpos = g_patd.orgX;
							tmpypos = g_patd.orgY;
						}
					}
				}
				else if ((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3))
				{
					if (tmpfeed == g_jog_point)
					{
						tmpxpos = xend;
						tmpypos = yend;
					}

					if (((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && ((g_sta_end_flag == 1) || (g_sta_end_flag == 4)))
					{
						if (tmpfeed == g_jog_stpoint)
						{
							stxpos = xend;
							stypos = yend;
						}
						if ((tmpfeed == g_jog_enpoint) && (g_jog_enpoint > g_jog_stpoint))
						{
							enxpos = xend;
							enypos = yend;
						}
					}
				}
			}
			else
				i++;
		}

		// /[将运行点重新花一次]**********************
		if (vn == PAT_P1 || vn == 100 || vn == PAT_P1_ZOOM)
		{

			xst = g_patd.orgX;
			yst = g_patd.orgY;
			xend = xst;
			yend = yst;
			if ((HREG[HREG_FEED_C] || g_BorSTflag))
			{
				// DrawLine(xorg, yorg, xorg + xst * scale / 1000, yorg - yst * scale / 1000, Color_Red);
				DrawLine(xorg, yorg, xorg + xst * scale / 1000,
						 yorg - yst * scale / 1000, COLOR_LINE_FINISH);
			}

			tmpfeed = 0;
			for (i = 0; i < (sewInfo.ptsize / 2);)
			{
				f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
				if (f)
				{
					tmpfeed++;
					if (tmpfeed > HREG[HREG_FEED_C])
						break;
					xend = xst;
					yend = yst;
					//				#if Debug
					//				printf("borDrawCmd %04x %d %d %d \r\n",DrawPatCmd,xtmp,ytmp,DrawOffest);
					//				#endif
					if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
					{
						switch ((sewIcmd.cmd & 0xf000))
						{
						case 0x2000: //空送
						{
							xend = xst + sewIcmd.dx;
							yend = yst + sewIcmd.dy;
							xst = xend;
							yst = yend;
						}
						break;

						case 0x6000: //缝纫
						{
							xend = xst;
							yend = yst;

							xend = xst + sewIcmd.dx;
							yend = yst + sewIcmd.dy;
							// DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000, xorg + xend * scale / 1000, yorg - yend * scale / 1000, Color_Red);
							DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
									 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
							xst = xend;
							yst = yend;
						}
						break;
						}
					}

					i += sewIcmd.off;
				}
				else
					i++;
			}
		}

		// /[绘制指令代码]**********************
		if (vn == PAT_P1 || vn == 100 || vn == PAT_P1_ZOOM ||
			((vn == PAT_VIEW_MODIFY1 || vn == PAT_VIEW_MODIFY2 || vn == PAT_MODIFY ||
			  vn == PAT_CONVENT || vn == PAT_VIEW_CREATE) &&
			 g_CodeEn == 1))
		{

			xst = g_patd.orgX;
			yst = g_patd.orgY;
			xend = xst;
			yend = yst;
			DrawFPBMPBTN2(xorg - 7, yorg - 7, 14, 14, BMP_H);
			if (g_patd.orgX != 0 || g_patd.orgY != 0)
			{
				DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_H2);
			}

			for (i = 0; i < (sewInfo.ptsize / 2);)
			{
				f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
				if (f)
				{
					xend = xst;
					yend = yst;

					if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
					{
						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;
						xst = xend;
						yst = yend;
					}
					else
					{
						switch (sewIcmd.cmd >> 8)
						{
						case 0xf0:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_P);
							break;
						case 0xf1:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_T);
							break;
						case 0xfe: //上下暂停
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, (sewIcmd.cmd & 0xff) ? BMP_U : BMP_D);
							break;
						case 0xf4:
							if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
								DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_J);
							break;
						case 0xfd:
							DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_C);
							break;
						case 0xfC: //功能
							k1 = 0;
							for (k = 0; k < 8; k++)
							{
								if ((sewIcmd.cmd >> k) & 0x01)
								{
									k1++;
								}
							}
							if (k1 > 1)
							{
								if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
									DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_FX);
							}
							else
							{
								switch (sewIcmd.cmd & 0xff)
								{
								case 0x01:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F1);
									break;
								case 0x02:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F2);
									break;
								case 0x04:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F3);
									break;
								case 0x08:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F4);
									break;
								case 0x10:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F5);
									break;
								case 0x20:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F6);
									break;
								case 0x40:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F7);
									break;
								case 0x80:
									if (vn != PAT_P1 && vn != PAT_P1_ZOOM)
										DrawFPBMPBTN2(xorg + xend * scale / 1000 - 7, yorg - yend * scale / 1000 - 7, 14, 14, BMP_F8);
									break;
								}
							}
							break;
						}
					}

					i += sewIcmd.off;
				}
				else
					i++;
			}
		}

#if 1
		if (((vn == PAT_VIEW_MODIFY1) || (vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) &&
			(g_ImgPoint == 1))
		{

			if ((abs(xpoint - inxpos) < 10) && (abs(ypoint - inypos) < 10) &&
				((inxpos != 0) || (inypos != 0)) &&
				(tmpfeed1 >= g_min_feedsum) && (tmpfeed1 <= g_max_feedsum))
			{

				DrawBigPoint(xpoint, ypoint, COLOR_SELECT_P);
				*infeed = tmpfeed1;
			}
			else
			{

				DrawBigPoint(xpoint1, ypoint1, COLOR_SELECT_P);
			}
		}
		// if (vn == PAT_P1_ZOOM)
		// {
		// 	U16 tmpS_2HOME = (U16)(S_2HOME);
		// 	if (HREG[HREG_FEED_C] != 0)
		// 	{
		// 		DrawSerialNumber(xorg + tmpxpos * scale / 1000 - 1, yorg - tmpypos * scale / 1000, 3, colorf, COLOR_CODE2, 1);
		// 	}
		// 	else if (tmpS_2HOME == 0x2000)
		// 	{
		// 		DrawSerialNumber(xorg + g_patd.orgX * scale / 1000 - 1, yorg - g_patd.orgY * scale / 1000, 3, colorf, COLOR_CODE2, 1);
		// 	}
		// }
		if ((vn == PAT_VIEW_MODIFY1 || vn == PAT_VIEW_MODIFY2 || vn == PAT_MODIFY ||
			 vn == PAT_CONVENT || vn == PAT_VIEW_CREATE) &&
			g_CodeEn == 1)
		{
			if (vn == PAT_VIEW_MODIFY1)
			{
				if (g_jog_point != 0)
				{
					DrawSerialNumber(xorg + tmpxpos * scale / 1000 - 1, yorg - tmpypos * scale / 1000,
									 3, colorf, COLOR_CODE_C, (g_ImgPoint == 1) ? 1 : 0);
				}
				else if (g_BorSTflag)
				{
					DrawSerialNumber(xorg + g_patd.orgX * scale / 1000 - 1, yorg - g_patd.orgY * scale / 1000,
									 3, colorf, COLOR_CODE_C, (g_ImgPoint == 1) ? 1 : 0);
				}
				//			else{
				//				DrawSerialNumber(xorg,yorg,0,colorf,Color_Green);	//在机械原点
				//			}
			}

			if (((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (g_sta_end_flag != 0))
			{
				DrawSerialNumber(xorg + stxpos * scale / 1000 - 1, yorg - stypos * scale / 1000,
								 1, colorf, COLOR_CODE_C, 0);
				if (g_jog_enpoint > g_jog_stpoint)
				{
					DrawSerialNumber(xorg + enxpos * scale / 1000 - 1, yorg - enypos * scale / 1000,
									 2, colorf, COLOR_CODE_C, 0);
				}
			}

			if (((vn == PAT_VIEW_MODIFY2) || (vn == PAT_VIEW_MODIFY3)) && (g_jog_point != 0))
			{
				DrawSerialNumber(xorg + tmpxpos * scale / 1000 - 1, yorg - tmpypos * scale / 1000,
								 3, colorf, COLOR_CODE_C, (g_ImgPoint == 1) ? 1 : 0);
			}

			if (vn == PAT_VIEW_CREATE)
			{
				if (g_array4[0][0] > 301)
					g_array4[0][0] = 0;
				n = g_array4[0][0] + 1;
				for (k = 0; k < n; k++)
				{
					if (k == g_array4[0][0])
					{ //最后一个坐标点
						if (k == 0)
						{
							DrawLine(xorg + xend * scale / 1000,
									 yorg - yend * scale / 1000, xorg + g_disxpos * scale / 1000,
									 yorg - g_disypos * scale / 1000, COLOR_LINE_SEW);
						}
						else
						{
							DrawLine(xorg + ((S16)(g_array4[k][0])) * scale / 1000,
									 yorg - ((S16)(g_array4[k][1])) * scale / 1000,
									 xorg + g_disxpos * scale / 1000, yorg - g_disypos * scale / 1000, COLOR_LINE_SEW);
						}
					}
					else
					{
						if (k == 0)
						{
							DrawLine(xorg + xend * scale / 1000,
									 yorg - yend * scale / 1000,
									 xorg + ((S16)(g_array4[k + 1][0])) * scale / 1000,
									 yorg - ((S16)(g_array4[k + 1][1])) * scale / 1000, COLOR_LINE_SEW);
						}
						else
						{
							DrawLine(xorg + ((S16)(g_array4[k][0])) * scale / 1000,
									 yorg - ((S16)(g_array4[k][1])) * scale / 1000,
									 xorg + ((S16)(g_array4[k + 1][0])) * scale / 1000,
									 yorg - ((S16)(g_array4[k + 1][1])) * scale / 1000, COLOR_LINE_SEW);
						}
					}
				}
				DrawSerialNumber(xorg + g_disxpos * scale / 1000 - 1, yorg - g_disypos * scale / 1000,
								 3, colorf, COLOR_CODE_C, 0);
			}
			//		if((vn == 8)&&((gsetxpos!=0)||(gsetypos!=0)))
			//		{
			//			x2 = (S32)(xorg+gsetxpos*scale/1000);
			//			y2 = (S32)(yorg-gsetypos*scale/1000);
			//			DrawSerialNumber(x2,y2,3,colorf,RGB(224,87,38));
			//		}

			if ((vn == PAT_VIEW_MODIFY3) /*&&(jogmode==1)*/ || (vn == PAT_VIEW_CONVENT))
			{
				for (k = 1; k < g_point_num; k++)
				{
					k1 = (S16)(g_array10[k][0]);
					k2 = (S16)(g_array10[k][1]);
					x2 = (S32)(xorg + k1 * scale / 1000);
					y2 = (S32)(yorg - k2 * scale / 1000);
					DrawSerialNumber2(x2, y2, k, colorf, COLOR_CTRL);
				}
				if (((g_dxpos != 0) || (g_dypos != 0)) && (MENU_MODIFYADD != plastno))
				{
					x2 = (S32)(xorg + g_disxpos * scale / 1000);
					y2 = (S32)(yorg - g_disypos * scale / 1000);
					DrawSerialNumber(x2, y2, 3, colorf, COLOR_CTRL, 0);
				}
			}
		}

#endif
		SetDrawXY(0, 0, LCD_width, LCD_height, 0);
	}
	else if (PatType == PAT_EMB)
	{ //EMB

		SetDrawXY(0, 0, LCD_width, LCD_height, 0);
	}
}

//------------------------------------------/
// Function   : void ViewPoint(U16 x, U16 y, U32 color, U8 linId)
// Description: 绘制指定颜色的针点
// linId ：限制图像显示范围的组
//------------------------------------------/
void ViewRunPoint(U16 x, U16 y, U32 color, U8 linId)
{
	//U8 i;

	switch (linId)
	{
	case 0:
		// SetDrawXY(104, 126, 546, 398, 1);//修改
		SetDrawXY(CONVAS_PX1, CONVAS_PY1, CONVAS_PX1 + CONVAS_X1, CONVAS_PY1 + CONVAS_Y1, 1);
		break;
	case 1:
		SetDrawXY(CONVAS_PX5, CONVAS_PY5, CONVAS_PX5 + CONVAS_X5, CONVAS_PY5 + CONVAS_Y5, 1);
		break;

	case 2:
		SetDrawXY(CONVAS_PX5, CONVAS_PY5, CONVAS_PX5 + CONVAS_X5, CONVAS_PY5 + CONVAS_Y5, 1);
		break;
	}
	// DrawHLine(x - 1, y, 3, color);
	// DrawVLine(x, y - 1, 3, color);
	DrawPoint(x, y, color);

	SetDrawXY(0, 0, LCD_width, LCD_height, 0);
	wdt();
}
//------------------------------------------/
// Function   : void ViewPoint(U16 x, U16 y, U32 color, U8 linId)
// Description: 显示原点十字标识 与限制图像显示范围
// linId ：限制图像显示范围的组
//------------------------------------------/
void ViewPoint(U16 x, U16 y, U32 color, U8 linId)
{
	//printf("Mit_ViewPoint(%d,%d,%d)\r\n",x,y,linId);
	switch (linId)
	{
	case 0:
		// SetDrawXY(104, 126, 446, 398, 1);
		SetDrawXY(CONVAS_PX1, CONVAS_PY1, CONVAS_PX1 + CONVAS_X1, CONVAS_PY1 + CONVAS_Y1, 1);
		break;

	case 1:
		SetDrawXY(CONVAS_PX9, CONVAS_PY9, CONVAS_PX9 + CONVAS_X9, CONVAS_PY9 + CONVAS_Y9, 1);
		break;

	case 2:
		// SetDrawXY(81, 113, 442, 404, 1);
		SetDrawXY(CONVAS_PX5, CONVAS_PY5, CONVAS_PX5 + CONVAS_X5, CONVAS_PY5 + CONVAS_Y5, 1);
		//SetDrawXY(0,0,LCD_width,LCD_height,0);
		break;
	}
	// DrawHLine(x - 4, y, 9, color);
	// DrawVLine(x, y - 4, 9, color);
	DrawCross7x7(x, y, color);

	SetDrawXY(0, 0, LCD_width, LCD_height, 0);
	wdt();
}
//------------------------------------------
// Function   : void ViewPoint1(U16 feedc)
// Description: 显示针位置
// U16 feedc  :  当前针数
//	U8 Type	  :  类型 0-三菱  1-兄弟
// U32 Patlen:花样数据总长 针对兄弟款   2016-12-16
//------------------------------------------
U8 ViewPoint1(U32 feedc, S32 scale, S16 ox1, S16 oy1, U8 Type, U32 Patlen, U8 flag)
{
	U8 ch, f;
	U32 i, j;
	S32 x, y;
	S32 xtmp, ytmp;
	S16 xend = 0, yend = 0;
	U32 tmpfeed = 0;
	S16 xorg = 0, yorg = 0;
	S32 tmpScale;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};

	U16 xl = 0, xr = 0, yu = 0, yd = 0;
	U8 onpaint = 0;

	if (Type == PAT_SEW)
	{
		scale /= 2;
	}
	tmpScale = scale;
	scale = 0;
	if (flag == 0)
	{
		xtmp = IMAGE_X3 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y3 * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY));
		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;

		xorg = CONVAS_ORG_PX3;
		yorg = CONVAS_ORG_PY3;
		xl = CONVAS_PX3;
		xr = CONVAS_PX3 + CONVAS_X3;
		yu = CONVAS_PY3;
		yd = CONVAS_PY3 + CONVAS_Y3;
		SetDrawXY(xl, yu, xr, yd, 1);
	}
	else if (flag == 1)
	{

		xtmp = 47600 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = 31800 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY));

		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;

		xorg = 384;
		yorg = 304;
		xl = 137;
		xr = 631;
		yu = 137;
		yd = 472;
		SetDrawXY(xl, yu, xr, yd, 1);
	}

	if ((ox1 != xorg) || (oy1 != yorg))
	{
		xorg = ox1;
		yorg = oy1;
	}

	scale += tmpScale;
	if (scale < 10)
		scale = 10;

	if (Type == PAT_MIT)
	{ //三菱款
		xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
		yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;

		RecoveData1();
		if (feedc)
		{
			x = 0;
			y = 0;
#if PATRULES_ORG_OFFSET
			if (SREG[3] == 7)
			{
				x = (S16)(SREG[6]);
				y = -(S16)(SREG[7]);
			}
#endif
			j = g_patd.pat_add + 1;
			for (i = 0; i < feedc; i++)
			{
				if (SREG[j - 1] != 0x3a)
				{
					ch = (U8)(SREG[j] & 0xff);
					if (ch >= 0x80)
					{
						ch -= 0x80;
						x -= ch;
					}
					else
					{
						x += ch;
					}
					ch = (SREG[j] >> 8) & 0xff;
					if (ch >= 0x80)
					{
						ch -= 0x80;
						y += ch;
					}
					else
					{
						y -= ch;
					}
				}
				j += 2;
			}
			x = (S32)(x * scale / 1000 + xorg);
			y = (S32)(y * scale / 1000 + yorg);
			DrawSerialNumber(x, y, 3, 1, COLOR_CODE_C, 1);
			if (x > xr - 9 || x < xl + 9 || y > yd - 9 || y < yu + 9)
				onpaint = 1;
		}
		else
		{
			x = (S32)(xorg);
			y = (S32)(yorg);
			if (x > xr - 9 || x < xl + 9 || y > yd - 9 || y < yu + 9)
				onpaint = 1;
		}
	}
	else if (Type == PAT_SEW)
	{ // 兄弟款

		f = Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		if (f == 0)
			return 0;

		xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
		yorg += (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;

		if (feedc)
		{
			xend = g_patd.orgX;
			yend = g_patd.orgY;

			for (i = 0; (i < (sewInfo.ptsize / 2)) && (tmpfeed < feedc);)
			{
				f = Bor_DrawCmd((U16 *)(&SREG[i + sewInfo.pataddr]), &sewIcmd);
				if (f)
				{
					if (sewIcmd.cmd == 0xff00)
					{
						break;
					}
					tmpfeed++;
					if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
					{
						xend += sewIcmd.dx;
						yend += sewIcmd.dy;
					}
					i += sewIcmd.off;
				}
				else
					i++;
			}

			x = (U32)(xorg + xend * scale / 1000);
			y = (U32)(yorg - yend * scale / 1000);
			RecoveData1();
			DrawSerialNumber(x, y, 3, 1, COLOR_CODE_C, 1);
			if (x > xr - 9 || x < xl + 9 || y > yd - 9 || y < yu + 9)
				onpaint = 1;
		}
		else if (g_BorSTflag)
		{
			x = (U32)(xorg + g_patd.orgX * scale / 1000);
			y = (U32)(yorg - g_patd.orgY * scale / 1000);

			RecoveData1();
			DrawSerialNumber(x, y, 3, 1, COLOR_CODE_C, 1);
			if (x > xr - 9 || x < xl + 9 || y > yd - 9 || y < yu + 9)
				onpaint = 1;
		}
	}
	SetDrawXY(0, 0, LCD_width, LCD_height, 0);
	return onpaint;
}

//------------------------------------------/
// Function   : Mit_ViewMutPoint				/
// Description: 局部重绘运行过的针点与超过的针点		/
// U8 PatType：花样类型  0-三菱款    1-兄弟款   2016-11-30
// U32 Patlen:花样数据总长 针对兄弟款   2016-12-16
//------------------------------------------/
void ViewMutPoint(U32 vn, S32 scale, S16 ox1, S16 oy1, U8 PatType, U32 Patlen, U16 infeed)
{

	U8 ch; //,cw
	U32 i, j;
	U32 dat; //,dat2
	S32 ox, oy, /*ox0, oy0,*/ x, y;
	S32 xtmp, ytmp;
	S32 x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	S16 xorg = 0, yorg = 0;
	S32 tmpScale = 0;
	U32 tmpfeed = 0;
	ISEWADD sewInfo = {0};
	ISEWCMD sewIcmd = {0};
	//	U8 tmpISdat =0;
	U8 f;
	S32 xst = 0, xend = 0, yst = 0, yend = 0;

	if (vn == PAT_P1 || vn == 1)
	{
		tmpScale = scale;
		scale = 0;
	}
	g_patd.scale = scale;
	g_dispat.feedc = 0;
	// 最大比例
	if (g_patd.patmX == g_patd.patiX)
		g_patd.patmX++;
	if (g_patd.patmY == g_patd.patiY)
		g_patd.patmY++;

	wdt();

	if (vn == PAT_P1 || vn == 100) // 主画面
	{

		xorg = CONVAS_ORG_PX1;

		yorg = CONVAS_ORG_PY1;

		// DrawFillRect(CONVAS_PX1, CONVAS_PY1,
		// 			 CONVAS_X1, CONVAS_Y1, Color_Black); //比主画面窄一点，以免覆盖黑色边框
		SetDrawXY(CONVAS_PX1, CONVAS_PY1, CONVAS_PX1 + CONVAS_X1, CONVAS_PY1 + CONVAS_Y1, 1);
		xtmp = IMAGE_X1 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y1 * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY)); //-16  修改

		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}
	if (vn == PAT_VIEW_P1) // 预览跳针
	{
		xorg = CONVAS_ORG_PX3;

		yorg = CONVAS_ORG_PY3;

		// DrawFillRect(CONVAS_PX3, CONVAS_PY3,
		// 			 CONVAS_X3, CONVAS_Y3, Color_Black); //比主画面窄一点，以免覆盖黑色边框
		SetDrawXY(CONVAS_PX3, CONVAS_PY3, CONVAS_PX3 + CONVAS_X3, CONVAS_PY3 + CONVAS_Y3, 1);
		xtmp = IMAGE_X3 * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = IMAGE_Y3 * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY)); //-16  修改

		if (xtmp > ytmp)
			scale = ytmp * 10;
		else
			scale = xtmp * 10;
	}
	wdt();

	if ((ox1 != xorg) || (oy1 != yorg))
	{
		if (vn == PAT_P1 || vn == 1)
		{
			xorg = ox1;
			yorg = oy1;
		}
	}
	scale += tmpScale;
	if (scale < 10)
		scale = 10;

	if ((g_patd.scale == 0) || (g_patd.scale > scale))
	{
		g_patd.scale = scale;
	}
	else
	{
		scale = g_patd.scale;
	}

	if (PatType == PAT_MIT)
	{ //三菱款

		xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
		yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000; // 针位绘图

		DrawPoint(xorg, yorg, COLOR_ORG);
		ox = 0;
		oy = 0;
#if PATRULES_ORG_OFFSET
		if (SREG[3] == 7)
		{
			ox = (S16)(SREG[6]);
			oy = -(S16)(SREG[7]);
		}
#endif

		i = 0;
		j = g_patd.pat_add;
		x = 0;
		y = 0;

		if (infeed <= HREG[HREG_FEED_C])
		{
			for (; i < HREG[HREG_FEED_C]; i++)
			{
				//if(i>=HREG[HREG_FEED_C]) return;
				dat = SREG[j] & 0x00ff;
				if ((dat == 0x1) || (dat == 0x21) || (dat == 0x41) || (dat == 0x61) || (dat == 0x3) || (dat == 0x1b))
				{
					ch = (U8)(SREG[j + 1] & 0xff);
					if (ch >= 0x80)
						xtmp = (ch - 0x80) * (-1);
					else
						xtmp = ch;
					ch = (SREG[j + 1] >> 8) & 0xff;
					if (ch >= 0x80)
						ytmp = (ch - 0x80) * (-1);
					else
						ytmp = ch;

					x = ox + xtmp;
					y = oy - ytmp;
					if ((vn == PAT_P1 || vn == PAT_P1_ZOOM) && (i >= infeed && i < HREG[HREG_FEED_C]))
					{
						x1 = (S32)(ox * scale / 1000 + xorg);
						y1 = (S32)(oy * scale / 1000 + yorg);
						x2 = (S32)(x * scale / 1000 + xorg);
						y2 = (S32)(y * scale / 1000 + yorg);
						if ((dat != 0x3) && (dat != 0x1b))
						{
							DrawPoint(x2, y2, COLOR_PIN_FINISH);
							DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
						}
						else
						{
							DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
						}
					}
					// if ((vn == PAT_P1_ZOOM) && (dat != 0x3) && (dat != 0x1b))
					// {
					// 	x1 = (S32)(ox * scale / 1000 + xorg);
					// 	y1 = (S32)(oy * scale / 1000 + yorg);
					// 	x2 = (S32)(x * scale / 1000 + xorg);
					// 	y2 = (S32)(y * scale / 1000 + yorg);
					// 	if (i >= infeed && i <= HREG[HREG_FEED_C])
					// 	{
					// 		// ViewRunPoint(x2, y2, COLOR_PAT_PIN_FINISH, 0);
					// 		// SetDrawXY(104, 126, 446, 398, 1);//修改
					// 		SetDrawXY(CONVAS_PX1, CONVAS_PY1, CONVAS_PX1 + CONVAS_X1, CONVAS_PY1 + CONVAS_Y1, 1);
					// 		DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
					// 	}
					// }
					ox = x;
					oy = y;
				}
				j += 2;
				wdt(); // 喂狗
			}
		}
		else
		{
			for (; i < infeed; i++)
			{
				//if(i>=HREG[HREG_FEED_C]) return;
				dat = SREG[j] & 0x00ff;
				if ((dat == 0x1) || (dat == 0x21) || (dat == 0x41) || (dat == 0x61) || (dat == 0x3) || (dat == 0x1b))
				{
					ch = (U8)(SREG[j + 1] & 0xff);
					if (ch >= 0x80)
						xtmp = (ch - 0x80) * (-1);
					else
						xtmp = ch;
					ch = (SREG[j + 1] >> 8) & 0xff;
					if (ch >= 0x80)
						ytmp = (ch - 0x80) * (-1);
					else
						ytmp = ch;

					x = ox + xtmp;
					y = oy - ytmp;
					if ((vn == PAT_P1 || vn == PAT_P1_ZOOM) && (i < infeed && i >= HREG[HREG_FEED_C]))
					{
						x1 = (S32)(ox * scale / 1000 + xorg);
						y1 = (S32)(oy * scale / 1000 + yorg);
						x2 = (S32)(x * scale / 1000 + xorg);
						y2 = (S32)(y * scale / 1000 + yorg);
						if ((dat != 0x3) && (dat != 0x1b))
						{
							DrawPoint(x2, y2, COLOR_PIN_SEW);
							DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW);
						}
						else
						{
							DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY);
						}
					}
					ox = x;
					oy = y;
				}
				j += 2;
				wdt(); // 喂狗
			}
		}
	}
	else if (PatType == PAT_SEW)
	{ //兄弟款

		f = Bor_AnaInfo((U16 *)SREG, Patlen, &sewInfo);
		if (f == 0)
			return;

		xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000;
		yorg += (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;

		// DrawHLine(xorg - 1, yorg, 3, Color_Green);
		// DrawVLine(xorg, yorg - 1, 3, Color_Green);
		DrawPoint(xorg, yorg, COLOR_ORG);

		//坐标起点
		xst = g_patd.orgX;
		yst = g_patd.orgY;
		xend = xst;
		yend = yst;

		//针数据段
		for (i = 0; i < (sewInfo.ptsize / 2);)
		{
			f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
			if (f)
			{
				tmpfeed++;
				xend = xst;
				yend = yst;
				//				#if Debug
				//				printf("borDrawCmd %04x %d %d %d \r\n",DrawPatCmd,xtmp,ytmp,DrawOffest);
				//				#endif
				if (tmpfeed > HREG[HREG_FEED_C])
					return;
				if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
				{
					switch ((sewIcmd.cmd & 0xf000))
					{
					case 0x2000: //空送
					{
						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;
						if (vn == PAT_P1 || vn == 100)
						{
							if (tmpfeed >= infeed && tmpfeed <= HREG[HREG_FEED_C])
							{
								DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
										 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
							}
							else if (tmpfeed <= infeed && tmpfeed >= HREG[HREG_FEED_C])
							{
								DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
										 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_EMPTY);
							}
						}

						xst = xend;
						yst = yend;
					}
					break;

					case 0x6000: //缝纫
					{
						xend = xst;
						yend = yst;

						xend = xst + sewIcmd.dx;
						yend = yst + sewIcmd.dy;

						if (vn == PAT_P1 || vn == 100)
						{
							if (tmpfeed >= infeed && tmpfeed <= HREG[HREG_FEED_C])
							{
								DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
										 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_FINISH);
								// ViewRunPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
								// 			 COLOR_CROSS, 0);
								DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_FINISH);
							}
							else if (tmpfeed <= infeed && tmpfeed >= HREG[HREG_FEED_C])
							{
								DrawLine(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
										 xorg + xend * scale / 1000, yorg - yend * scale / 1000, COLOR_LINE_SEW);
								// ViewRunPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000,
								// 			 COLOR_CROSS, 0);
								DrawPoint(xorg + xst * scale / 1000, yorg - yst * scale / 1000, COLOR_PIN_SEW);
							}
						}

						xst = xend;
						yst = yend;
					}
					break;
					}
				}

				i += sewIcmd.off;
			}
			else
			{
				i++;
			}
		}
	}
	SetDrawXY(0, 0, LCD_width, LCD_height, 0);
}

//------------------------------------------/
// Function   : S32 pat_GetScale(U16 id)			/
// Description: 获取指定画布大小下，显示图案的最佳缩放比
//------------------------------------------/
void pat_clearConvas(U8 id)
{
	U16 xPos = 0;
	U16 yPos = 0;
	U16 xSize = 0;
	U16 ySize = 0;
	switch (id)
	{
	case CONVAS1:
		xPos = CONVAS_PX1;
		yPos = CONVAS_PY1;
		xSize = CONVAS_X1;
		ySize = CONVAS_Y1;
		break;
	case CONVAS2:
		//xPos = CONVAS_PX2;
		//yPos = CONVAS_PY2;
		xSize = CONVAS_X2;
		ySize = CONVAS_Y2;
		break;
	case CONVAS3:
		xPos = CONVAS_PX3;
		yPos = CONVAS_PY3;
		xSize = CONVAS_X3;
		ySize = CONVAS_Y3;
		break;
	case CONVAS4:
		xPos = CONVAS_PX4;
		yPos = CONVAS_PY4;
		xSize = CONVAS_X4;
		ySize = CONVAS_Y4;
		break;
	case CONVAS5:
		xPos = CONVAS_PX5;
		yPos = CONVAS_PY5;
		xSize = CONVAS_X5;
		ySize = CONVAS_Y5;
		break;
	case CONVAS6:
		xPos = CONVAS_PX6;
		yPos = CONVAS_PY6;
		xSize = CONVAS_X6;
		ySize = CONVAS_Y6;
		break;
	case CONVAS7:
		xPos = CONVAS_PX7;
		yPos = CONVAS_PY7;
		xSize = CONVAS_X7;
		ySize = CONVAS_Y7;
		break;
	case CONVAS8:
		xPos = CONVAS_PX8;
		yPos = CONVAS_PY8;
		xSize = CONVAS_X8;
		ySize = CONVAS_Y8;
		break;
	case CONVAS9:
		xPos = CONVAS_PX9;
		yPos = CONVAS_PY9;
		xSize = CONVAS_X9;
		ySize = CONVAS_Y9;
		break;
	default:
		break;
	}
	if (id != CONVAS2)
	{
		DrawFillRect(xPos, yPos, xSize, ySize, COLOR_CONVAS);
	}

	wdt();
}

//------------------------------------------/
// Function   : S32 pat_GetScale(U16 id)			/
// Description: 获取指定画布大小下，显示图案的最佳缩放比
//------------------------------------------/
S32 pat_getScale(U8 id, U8 rulesIndex)
{
	S32 scale = 0;
	S16 xtmp = 0, ytmp = 0;
	if (g_maxX == g_minX)
		g_maxX++;
	if (g_maxY == g_minY)
		g_maxY++;

	switch (id)
	{
	case CONVAS1:
		xtmp = IMAGE_X1;
		ytmp = IMAGE_Y1;
		break;
	case CONVAS2:
		xtmp = IMAGE_X2;
		ytmp = IMAGE_Y2;
		break;
	case CONVAS3:
		xtmp = IMAGE_X3;
		ytmp = IMAGE_Y3;
		break;
	case CONVAS4:
		xtmp = IMAGE_X4;
		ytmp = IMAGE_Y4;
		break;
	case CONVAS5:
		xtmp = IMAGE_X5;
		ytmp = IMAGE_Y5;
		break;
	case CONVAS6:
		xtmp = IMAGE_X6;
		ytmp = IMAGE_Y6;
		break;
	case CONVAS7:
		xtmp = IMAGE_X7;
		ytmp = IMAGE_Y7;
		break;
	case CONVAS8:
		xtmp = IMAGE_X8;
		ytmp = IMAGE_Y8;
		break;
	case CONVAS9:
		xtmp = IMAGE_X9;
		ytmp = IMAGE_Y9;
		break;
	default:
		break;
	}

	switch (rulesIndex)
	{
	case PATRULES_P1:
	case PATRULES_READ:
	case PATRULES_VIEW_DETAIL:
	case PATRULES_JOG_DETAIL:
	case PATRULES_JOG_X_N:
	case PATRULES_JOG_A_B:
	case PATRULES_JOG_A_X:
	case PATRULES_MODIFY_MAIN:
	case PATRULES_CONVERT_MAIN:
		xtmp = xtmp * 100 / ((g_patd.patmX - g_patd.patiX) < 2 ? 2 : (g_patd.patmX - g_patd.patiX));
		ytmp = ytmp * 100 / ((g_patd.patmY - g_patd.patiY) < 2 ? 2 : (g_patd.patmY - g_patd.patiY));
		break;
	case PATRULES_CREATE1:
	case PATRULES_CREATE2:
	case PATRULES_CREATE3:
	case PATRULES_VIEW_SIMPLE:
	case PATRULES_MODIFY_ADD:
	case PATRULES_MODIFY_ADD1:
	case PATRULES_MODIFY_ADD2:
	case PATRULES_MODIFY_ADD3:
	case PATRULES_MODIFY1_SINGLE:
	case PATRULES_MODIFY1_SINGLE_A_B:
	case PATRULES_MODIFY1_MUL_A_B0:
	case PATRULES_MODIFY1_MUL_A_B1:
	case PATRULES_MODIFY1_MUL_A_X:
	case PATRULES_MODIFY2_MUL_A_X:
	case PATRULES_MODIFY3_ENSURE:
	case PATRULES_CONVERT1_MOVE:
	case PATRULES_CONVERT1_NUM:
	case PATRULES_CONVERT1_ENSURE:
	case PATRULES_CONVERT1_OFFSET:
	case PATRULES_CONVERT1_OFFSMULT:
		xtmp = xtmp * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
		ytmp = ytmp * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
		break;

	default:
		break;
	}

	if (xtmp > ytmp)
		scale = ytmp * 10;
	else
		scale = xtmp * 10;

	wdt();
	return scale;
}

S32 pat_getFinalScale(S32 imgScale, S32 zoomScale)
{
	S32 finalScale = 0;
	finalScale = imgScale + zoomScale; //最佳显示倍率 + 输入的缩放倍率
	if (finalScale < 10)			   //最小倍率
		finalScale = 10;
	// if ((g_patd.scale == 0) || (g_patd.scale > finalScale))
	// {
	// 	g_patd.scale = finalScale;
	// }
	// else
	// {
	// 	finalScale = g_patd.scale;
	// }
	return finalScale;
}

//------------------------------------------/
// Function   : void pat_setDrawXY(U8 id,U16 posX,U16 posY)			/
// Description: 设置绘制花样的显示范围
//------------------------------------------/
void pat_setDrawXY(U8 id, U16 posX, U16 posY)
{
	U16 xPos = 0;
	U16 yPos = 0;
	U16 xSize = 0;
	U16 ySize = 0;
	switch (id)
	{
	case CONVAS1:
		xPos = CONVAS_PX1;
		yPos = CONVAS_PY1;
		xSize = CONVAS_X1;
		ySize = CONVAS_Y1;
		break;
	case CONVAS2:
		xPos = posX;
		yPos = posY;
		xSize = CONVAS_X2;
		ySize = CONVAS_Y2;
		break;
	case CONVAS3:
		xPos = CONVAS_PX3;
		yPos = CONVAS_PY3;
		xSize = CONVAS_X3;
		ySize = CONVAS_Y3;
		break;
	case CONVAS4:
		xPos = CONVAS_PX4;
		yPos = CONVAS_PY4;
		xSize = CONVAS_X4;
		ySize = CONVAS_Y4;
		break;
	case CONVAS5:
		xPos = CONVAS_PX5;
		yPos = CONVAS_PY5;
		xSize = CONVAS_X5;
		ySize = CONVAS_Y5;
		break;
	case CONVAS6:
		xPos = CONVAS_PX6;
		yPos = CONVAS_PY6;
		xSize = CONVAS_X6;
		ySize = CONVAS_Y6;
		break;
	case CONVAS7:
		xPos = CONVAS_PX7;
		yPos = CONVAS_PY7;
		xSize = CONVAS_X7;
		ySize = CONVAS_Y7;
		break;
	case CONVAS8:
		xPos = CONVAS_PX8;
		yPos = CONVAS_PY8;
		xSize = CONVAS_X8;
		ySize = CONVAS_Y8;
		break;
	case CONVAS9:
		xPos = CONVAS_PX9;
		yPos = CONVAS_PY9;
		xSize = CONVAS_X9;
		ySize = CONVAS_Y9;
		break;
	default:
		break;
	}
	SetDrawXY(xPos, yPos, xPos + xSize, yPos + ySize, 1);

	wdt();
}

//------------------------------------------/
// Function   : void pat_resetDrawXY(void)		/
// Description: 复位绘制花样的显示范围
//------------------------------------------/
void pat_resetDrawXY(void)
{
	SetDrawXY(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);
}

tydstConvasNature patConvas = {0, 0, 0, 0, 0, 0};
tydstPatRule patRule = {PM_BASE, CONVAS1, DISABLE, DISABLE, DISABLE, ENABLE, ENABLE, ENABLE, ENABLE, DISABLE, DISABLE, DISABLE, DISABLE, 0, DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, DISABLE, 0, 0, 0, 0, 0, 0, {0xff}};
void pat_resetPatRule(void)
{
	ResetData1();
	ResetData2();
	patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
	patRule.h_patConvasIndex = CONVAS1; //高-画布序号
	patRule.h_subline = DISABLE;		//高-辅助线
	patRule.h_imgZoom = DISABLE;		//高-图像缩放
	patRule.h_imgDrag = DISABLE;		//高-图像拖拽
	patRule.h_line = ENABLE;			//高-线段
	patRule.h_point = ENABLE;			//高-针点
	patRule.h_org = ENABLE;				//高-原点
	patRule.h_code = ENABLE;			//高-代码
	patRule.h_exCode = DISABLE;			//高-其他显示代码
	patRule.h_orgOffset = DISABLE;

	patRule.l_line_udZhou = DISABLE; //低-高-分离轴
	patRule.l_line_speed = DISABLE;	 //低-低-速度

	patRule.l_org_M_XL = 0; //低-互斥-0:小点;1:大十字

	patRule.l_ex_select = DISABLE;	   //低-共存-选段
	patRule.l_ex_pat = DISABLE;		   //低-共存-针段
	patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
	patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
	patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
	patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

	patRule.l_ex_select_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
	patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
	patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
	patRule.l_ex_ctrl_type = 0;
	patRule.l_ex_cursor_mode = 0x00;
	patRule.l_ex_cursor_type = 0;

	patRule.l_code.code = 0xff; //低-共存-显示代码
}
void pat_setPatRule(U8 type)
{
	switch (type)
	{
	case PATRULES_P1:
		patRule.h_patMode = PM_SEWING; //高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样

		patRule.h_patConvasIndex = CONVAS1; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = DISABLE;		//高-图像缩放
		patRule.h_imgDrag = DISABLE;		//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = DISABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 1;

		PAT_CODE_ALL = 0;
		PAT_CODE_H = 1;
		PAT_CODE_H2 = 1;
		PAT_CODE_C = 1;
		break;
	case PATRULES_READ:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS2; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = DISABLE;		//高-图像缩放
		patRule.h_imgDrag = DISABLE;		//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = DISABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = DISABLE;			//高-代码
		patRule.h_exCode = DISABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = DISABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x01; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_VIEW_DETAIL:
		patRule.h_patMode = PM_BASE;					//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS3;				//高-画布序号
		patRule.h_subline = DISABLE;					//高-辅助线
		patRule.h_imgZoom = ENABLE;						//高-图像缩放
		patRule.h_imgDrag = ENABLE;						//高-图像拖拽
		patRule.h_line = ENABLE;						//高-线段
		patRule.h_point = ENABLE;						//高-针点
		patRule.h_org = ENABLE;							//高-原点
		patRule.h_code = (g_CodeEn) ? ENABLE : DISABLE; //高-代码
		patRule.h_exCode = DISABLE;						//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示
		patRule.h_orgOffset = DISABLE;

		patRule.l_ex_select_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_JOG_DETAIL:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS3; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	  //低-共存-选段
		patRule.l_ex_pat = DISABLE;		  //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	  //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	  //低-共存-光标
		patRule.l_ex_selectNear = ENABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	  //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x01; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_JOG_X_N:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS3; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	  //低-共存-选段
		patRule.l_ex_pat = DISABLE;		  //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	  //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	  //低-共存-光标
		patRule.l_ex_selectNear = ENABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	  //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x01; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x01; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_JOG_A_B:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS3; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	  //低-共存-选段
		patRule.l_ex_pat = DISABLE;		  //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	  //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	  //低-共存-光标
		patRule.l_ex_selectNear = ENABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	  //低-共存-附近点高亮提示
		patRule.h_orgOffset = DISABLE;

		patRule.l_ex_select_mode = 0x01; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x02; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_JOG_A_X:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS3; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	  //低-共存-选段
		patRule.l_ex_pat = DISABLE;		  //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	  //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	  //低-共存-光标
		patRule.l_ex_selectNear = ENABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	  //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x01; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x04; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_VIEW_SIMPLE:
		patRule.h_patMode = PM_BASE;					//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS4;				//高-画布序号
		patRule.h_subline = DISABLE;					//高-辅助线
		patRule.h_imgZoom = ENABLE;						//高-图像缩放
		patRule.h_imgDrag = ENABLE;						//高-图像拖拽
		patRule.h_line = ENABLE;						//高-线段
		patRule.h_point = ENABLE;						//高-针点
		patRule.h_org = ENABLE;							//高-原点
		patRule.h_code = (g_CodeEn) ? ENABLE : DISABLE; //高-代码
		patRule.h_exCode = ENABLE;						//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = ENABLE;		   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x01;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 1;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_MODIFY_ADD:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS7; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x04; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY_ADD1:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS7; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x04; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY_ADD2:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS7; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x04; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY_ADD3:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS7; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = ENABLE;		   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02:显示十字光标
		patRule.l_ex_select_type = 0x04; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x01;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 1;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY1_SINGLE:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x00; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 7;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY1_SINGLE_A_B:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x02; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 8;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY1_MUL_A_B0:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = ENABLE;		   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x02; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x04;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 9;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 9;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY1_MUL_A_B1:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = ENABLE;		   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x02; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x04;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 10;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 10;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY1_MUL_A_X:
	case PATRULES_MODIFY2_MUL_A_X:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0x02; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0x01; //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY3_ENSURE:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = ENABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X01; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;	 //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0X00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_CREATE1:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS5; //高-画布序号
		patRule.h_subline = ENABLE;			//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0;  //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;  //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 4;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_CREATE2:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS5; //高-画布序号
		patRule.h_subline = ENABLE;			//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = ENABLE;		   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0;  //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;  //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x04; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 5;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 5;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_CREATE3:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS5; //高-画布序号
		patRule.h_subline = ENABLE;			//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0;  //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;  //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 6;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_MODIFY_MAIN:
	case PATRULES_CONVERT_MAIN:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS6; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = DISABLE;		//高-图像缩放
		patRule.h_imgDrag = DISABLE;		//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = DISABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x01; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0;  //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;  //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_CONVERT1_MOVE:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X01; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;	 //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 2;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_CONVERT1_MOVE_ORG:
		patRule.h_patMode = PM_PATTING;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = ENABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = ENABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = ENABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X01; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;	 //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x04;
		patRule.l_ex_cursor_type = 2;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		PAT_CODE_H = 0;
		break;
	case PATRULES_CONVERT1_NUM:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = DISABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;	 //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_CONVERT1_ENSURE:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = DISABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = ENABLE;		   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;	 //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x01;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 2;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;
	case PATRULES_CONVERT1_OFFSET:
	case PATRULES_CONVERT1_OFFSMULT:
		patRule.h_patMode = PM_BASE;		//高-0x00:基本显示;0x01:缝纫中(0-x);0x02:创建花样
		patRule.h_patConvasIndex = CONVAS9; //高-画布序号
		patRule.h_subline = DISABLE;		//高-辅助线
		patRule.h_imgZoom = ENABLE;			//高-图像缩放
		patRule.h_imgDrag = ENABLE;			//高-图像拖拽
		patRule.h_line = ENABLE;			//高-线段
		patRule.h_point = ENABLE;			//高-针点
		patRule.h_org = ENABLE;				//高-原点
		patRule.h_code = ENABLE;			//高-代码
		patRule.h_exCode = ENABLE;			//高-其他显示代码
		patRule.h_orgOffset = DISABLE;

		patRule.l_line_udZhou = DISABLE; //低-高-分离轴
		patRule.l_line_speed = ENABLE;	 //低-低-速度

		patRule.l_org_M_XL = 0x02; //低-0x01:小点;0x02:大十字

		patRule.l_ex_select = DISABLE;	   //低-共存-选段
		patRule.l_ex_pat = ENABLE;		   //低-共存-针段
		patRule.l_ex_ctrl = DISABLE;	   //低-共存-绘制控制点
		patRule.l_ex_cursor = DISABLE;	   //低-共存-光标
		patRule.l_ex_selectNear = DISABLE; //低-共存-附近点选择吸附
		patRule.l_ex_topNear = DISABLE;	   //低-共存-附近点高亮提示

		patRule.l_ex_select_mode = 0X00; //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_select_type = 0;	 //0x01:选中段(x-x+n);0x02:选中段(a-b);0x04:选中段(a-b||a-x);
		patRule.l_ex_ctrl_mode = 0x00;	 //0x01:显示代码 、0x02：显示十字光标
		patRule.l_ex_ctrl_type = 0;
		patRule.l_ex_cursor_mode = 0x00;
		patRule.l_ex_cursor_type = 0;

		PAT_CODE_ALL = 0xff; //低-共存-显示代码
		break;

	default:
		pat_resetPatRule();
		break;
	}

	//[获取画布参数属性]*******************
	switch (patRule.h_patConvasIndex)
	{
	case CONVAS1:
		patConvas.conPosX = CONVAS_PX1;
		patConvas.conPosY = CONVAS_PY1;
		patConvas.conSizeX = CONVAS_X1;
		patConvas.conSizeY = CONVAS_Y1;
		patConvas.conOrgX = CONVAS_ORG_PX1;
		patConvas.conOrgY = CONVAS_ORG_PY1;
		break;
	case CONVAS2:
		patConvas.conPosX = CONVAS_PX2_1;
		patConvas.conPosY = CONVAS_PY2_1;
		patConvas.conSizeX = CONVAS_X2;
		patConvas.conSizeY = CONVAS_Y2;
		patConvas.conOrgX = CONVAS_ORG_PX2_1;
		patConvas.conOrgY = CONVAS_ORG_PY2_1;
		break;
	case CONVAS3:
		patConvas.conPosX = CONVAS_PX3;
		patConvas.conPosY = CONVAS_PY3;
		patConvas.conSizeX = CONVAS_X3;
		patConvas.conSizeY = CONVAS_Y3;
		patConvas.conOrgX = CONVAS_ORG_PX3;
		patConvas.conOrgY = CONVAS_ORG_PY3;
		break;
	case CONVAS4:
		patConvas.conPosX = CONVAS_PX4;
		patConvas.conPosY = CONVAS_PY4;
		patConvas.conSizeX = CONVAS_X4;
		patConvas.conSizeY = CONVAS_Y4;
		patConvas.conOrgX = CONVAS_ORG_PX4;
		patConvas.conOrgY = CONVAS_ORG_PY4;
		break;
	case CONVAS5:
		patConvas.conPosX = CONVAS_PX5;
		patConvas.conPosY = CONVAS_PY5;
		patConvas.conSizeX = CONVAS_X5;
		patConvas.conSizeY = CONVAS_Y5;
		patConvas.conOrgX = CONVAS_ORG_PX5;
		patConvas.conOrgY = CONVAS_ORG_PY5;
		break;
	case CONVAS6:
		patConvas.conPosX = CONVAS_PX6;
		patConvas.conPosY = CONVAS_PY6;
		patConvas.conSizeX = CONVAS_X6;
		patConvas.conSizeY = CONVAS_Y6;
		patConvas.conOrgX = CONVAS_ORG_PX6;
		patConvas.conOrgY = CONVAS_ORG_PY6;
		break;
	case CONVAS7:
		patConvas.conPosX = CONVAS_PX7;
		patConvas.conPosY = CONVAS_PY7;
		patConvas.conSizeX = CONVAS_X7;
		patConvas.conSizeY = CONVAS_Y7;
		patConvas.conOrgX = CONVAS_ORG_PX7;
		patConvas.conOrgY = CONVAS_ORG_PY7;
		break;
	case CONVAS8:
		patConvas.conPosX = CONVAS_PX8;
		patConvas.conPosY = CONVAS_PY8;
		patConvas.conSizeX = CONVAS_X8;
		patConvas.conSizeY = CONVAS_Y8;
		patConvas.conOrgX = CONVAS_ORG_PX8;
		patConvas.conOrgY = CONVAS_ORG_PY8;
		break;
	case CONVAS9:
		patConvas.conPosX = CONVAS_PX9;
		patConvas.conPosY = CONVAS_PY9;
		patConvas.conSizeX = CONVAS_X9;
		patConvas.conSizeY = CONVAS_Y9;
		patConvas.conOrgX = CONVAS_ORG_PX9;
		patConvas.conOrgY = CONVAS_ORG_PY9;
		break;
	default:
		break;
	}
}

//------------------------------------------/
// Function   : void pat_drawPattern(tydenConvas vn, S32 scale, S16 ox1, S16 oy1, U8 PatType, U32 Patlen, U16 inxpos, U16 inypos, U32 *infeed)
// Description: 绘制花样显示
// note: tydenConvas vn:输入画布模式;
//		 S32 scale:输入缩放倍率;
//       S16 ox1, S16 oy1:输入原点坐标-屏幕坐标系
//		 U8 PatType:输入花样类型
//		 U32 Patlen:输入花样长度
//		 U16 inxpos, U16 inypos:输入选中点坐标/输入坐标吸附点的坐标 - 屏幕坐标系
//		 U32 *infeed:返回选中点的序号
//------------------------------------------/
void pat_drawPattern(U8 rulesIndex, S32 scale, S16 ox1, S16 oy1, U8 PatType, U32 Patlen, U16 inxpos, U16 inypos, U32 *infeed)
{
#if 1
	// U8 colorf = 0; //,cw
	U32 i;
	U16 k = 0;
	U16 dat, dat1;

	U8 mdx, mdy;						//存储花样帧数据坐标的缓存
	S16 ox, oy, x, y;					//ox,oy当前点位置-花样坐标系;x,y下一点位置-花样坐标系
	S32 x1 = 0, y1 = 0, x2 = 0, y2 = 0; //x1,y1当前点位置-屏幕坐标系;x2,y2下一点位置-屏幕坐标系
	S16 xorg = 0, yorg = 0;				//实际中心点坐标-屏幕坐标(花样中心点 + 拖拽导致的偏移量)
	S32 tmpScale = 0;					//记录输入缩放倍率缓存
	S16 k1 = 0, k2 = 0;					//缓存
	U8 mut = 1;							//移送分辨率
	// S8 tmuly = 1;						//不同花样的坐标系方向

	S16 dxp = 10, dyp = 10; //定位点误差10个像素点

	S32 tmpx2 = 0, tmpy2 = 0, tmpxS = 0, tmpyS = 0, tmpxE = 0, tmpyE = 0;
	U8 disP = 0, disS = 0, disE = 0;
	S32 xpoint = 0, ypoint = 0, txpoint = 0, typoint = 0, xpoint1 = 0, ypoint1 = 0; //绘制选中附近点的缓存
	U32 tmpfeed1 = 0;																//记录符合附近点规范的点的点数缓存

	S16 disxpoint = LCD_WIDTH, disypoint = LCD_HEIGHT; //输入选中点坐标
	U32 tmpPatAdd = 0;

	// tmuly = (F_FILETYPE == PAT_SEW) ? -1 : 1;
	///[计算最佳显示倍率,与设置绘图范围*begin]*********************************************
	if (patRule.h_imgZoom == ENABLE) //支持缩放，记录输入倍率
	{
		tmpScale = scale;
		scale = 0;
	}
	g_patd.scale = scale;
	g_dispat.feedc = 0;
	// 最大比例
	if (g_patd.patmX == g_patd.patiX)
		g_patd.patmX++;
	if (g_patd.patmY == g_patd.patiY)
		g_patd.patmY++;

	wdt();

	switch (rulesIndex)
	{
	case PATRULES_P1:
	case PATRULES_VIEW_DETAIL:
	case PATRULES_JOG_DETAIL:
	case PATRULES_JOG_X_N:
	case PATRULES_JOG_A_B:
	case PATRULES_JOG_A_X:
	case PATRULES_VIEW_SIMPLE:
	case PATRULES_CREATE1:
	case PATRULES_CREATE2:
	case PATRULES_CREATE3:
	case PATRULES_MODIFY_MAIN:
	case PATRULES_MODIFY_ADD:
	case PATRULES_MODIFY_ADD1:
	case PATRULES_MODIFY_ADD2:
	case PATRULES_MODIFY_ADD3:
	case PATRULES_MODIFY1_SINGLE:
	case PATRULES_MODIFY1_SINGLE_A_B:
	case PATRULES_MODIFY1_MUL_A_B0:
	case PATRULES_MODIFY1_MUL_A_B1:
	case PATRULES_MODIFY1_MUL_A_X:
	case PATRULES_MODIFY2_MUL_A_X:
	case PATRULES_MODIFY3_ENSURE:
	case PATRULES_CONVERT_MAIN:
	case PATRULES_CONVERT1_MOVE:
	case PATRULES_CONVERT1_NUM:
	case PATRULES_CONVERT1_ENSURE:
	case PATRULES_CONVERT1_OFFSET:
	case PATRULES_CONVERT1_OFFSMULT:
		xorg = ox1;
		yorg = oy1;
		// colorf = 0;
		scale = pat_getScale(patRule.h_patConvasIndex, rulesIndex);
		pat_setDrawXY(patRule.h_patConvasIndex, 0, 0);
		break;
	case PATRULES_READ:
		xorg = ox1;
		yorg = oy1;
		// colorf = 0;
		scale = pat_getScale(patRule.h_patConvasIndex, rulesIndex);
		break;
	default:
		break;
	}

	scale = pat_getFinalScale(scale, tmpScale); //最佳显示倍率 + 输入的缩放倍率
	if ((g_patd.scale == 0) || (g_patd.scale > scale))
	{
		g_patd.scale = scale;
	}
	else
	{
		scale = g_patd.scale;
	}
	///[计算最佳显示倍率,与设置绘图范围*end]*********************************************

	///[使能图像拖动*begin]*********************************************
	if (patRule.h_imgDrag == ENABLE)
	{
		if ((ox1 != xorg) || (oy1 != yorg))
		{

			xorg = ox1;
			yorg = oy1;
		}
	}
	///[使能图像拖动*end]*********************************************

	///[绘制辅助线*begin]*********************************************
	if (patRule.h_subline == ENABLE)
	{
		DrawHLine(patConvas.conPosX, patConvas.conOrgY, patConvas.conSizeX, COLOR_SUBLINE);
		DrawVLine(patConvas.conOrgX, patConvas.conPosY, patConvas.conSizeY, COLOR_SUBLINE);
	}
	///[绘制辅助线*end]*********************************************

	///[不同花样类型的有所不同的绘制方法]*********************************************
	if (PatType == PAT_MIT)
	{
		///[花样数据异常处理*begin]*********************************************
		if (rulesIndex == PATRULES_P1)
		{
			U32 patSREGLen = 0;
			U16 checkSREG = 0;
			patSREGLen = ((SREG[0x02] + 0x40) / 2) + ((((U32)(SREG[0x05]) << 16) + SREG[0x04]) / 4) * 3; //花样数据总长度

			if (((SREG[0x1E] & 0x00ff) == 0x45) && (SREG[0x1F] != 0x3536)) //读取版本号
			{
				wdt();
				SREG[0x1E] = (SREG[0x1E] & 0xff00) | 0x0054;
				patSREGLen = ((SREG[0x02] + 0x40) / 2) + ((((U32)(SREG[0x05]) << 16) + SREG[0x04]) / 4) * 3;
				checkSREG = 0;

				for (i = 0; i < patSREGLen; y1++)
				{
					if (i != 0x20)
					{
						checkSREG += SREG[i];
					}
				}
				SREG[0x20] = checkSREG;
				wdt();
			}

			patSREGLen = ((SREG[0x02] + 0x40) / 2) + ((((U32)(SREG[0x05]) << 16) + SREG[0x04]) / 4) * 3; //花样数据总长度
			if (DataCheckSum((U16 *)(&SREG[0]), patSREGLen, 0) == 0)
			{
				for (i = 0; i < MAXSREG; i++)
					SREG[i] = 0;
				SREG[0] = 0;			//文件号
				SREG[2] = 64000 - 0x40; //线条数据启始位置在SREG[32000]位置
				SREG[3] = 6;
				SREG[4] = 0x3880; //每组数据的针数据长最大为80000
				SREG[5] = 1;
				SREG[12] = 0x3e8;  //花样最小长度100mm
				SREG[14] = 0x3e8;  //花样最小宽度100mm
				SREG[16] = 0x454e; //文件名
				SREG[17] = 0x2057;
				SREG[18] = 0x2020;
				SREG[19] = 0x2020;
				SREG[30] = 0x3045; //版本
				SREG[31] = 0x3536;
				g_draw_p = 0x24;  //绘图数据当前位置指针
				g_line_p = 32000; //线条数据当前位置指针
				g_pat_p = 52000;  //针数据当前位置指针
			}
			wdt(); // 喂狗
		}
		///[花样数据异常处理*end]*********************************************

		///[获取图像中心坐标*begin]*********************************************
		switch (rulesIndex)
		{
		case PATRULES_CREATE1:
		case PATRULES_CREATE2:
		case PATRULES_CREATE3:
		case PATRULES_CONVERT1_MOVE:
		case PATRULES_CONVERT1_NUM:
		case PATRULES_CONVERT1_ENSURE:
		case PATRULES_CONVERT1_OFFSET:
		case PATRULES_CONVERT1_OFFSMULT:
		case PATRULES_MODIFY_ADD:
		case PATRULES_MODIFY_ADD1:
		case PATRULES_MODIFY_ADD2:
		case PATRULES_MODIFY_ADD3:
		case PATRULES_MODIFY1_SINGLE:
		case PATRULES_MODIFY1_SINGLE_A_B:
		case PATRULES_MODIFY1_MUL_A_B0:
		case PATRULES_MODIFY1_MUL_A_B1:
		case PATRULES_MODIFY1_MUL_A_X:
		case PATRULES_MODIFY2_MUL_A_X:
		case PATRULES_MODIFY3_ENSURE:
			xorg -= (g_maxX + g_minX) * scale / 2 / 1000; //原点-屏幕坐标系
			yorg -= (g_maxY + g_minY) * scale / 2 / 1000;
			break;
		default:
			xorg -= (g_patd.patmX + g_patd.patiX) / 2 * scale / 1000; //原点-屏幕坐标系
			yorg -= (g_patd.patmY + g_patd.patiY) / 2 * scale / 1000;
			break;
		}

		///[获取图像中心坐标*end]*********************************************

		// /[绘制线 *begin]*************************
		if (patRule.h_line == ENABLE)
		{

			tmpPatAdd = g_patd.pat_add;

			ox = 0;
			oy = 0;
			x = 0;
			y = 0;

			// /[绘制原点的连线]*************************
			if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}

			for (k = 0; k < g_patd.feed_sum; k++)
			{

				dat1 = SREG[tmpPatAdd];
				dat = SREG[tmpPatAdd + 1];
				if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) ||
					(dat1 == PAT_FM_H) || (dat1 == PAT_FM_EMPTY) || (dat1 == PAT_FM_EMP_ED) || (dat1 == PAT_FM_END) || (dat1 == PAT_FM_FEND))
				{
					mdx = (U8)(dat);
					mdy = (U8)(dat >> 8);
				}
				else
				{
					mdx = 0;
					mdy = 0;
				}

				if (mdx >= 0x80)
				{
					mdx -= 0x80;
					x -= mdx;
				}
				else
					x += mdx;
				if (mdy >= 0x80)
				{
					mdy -= 0x80;
					y -= mdy;
				}
				else
					y += mdy;
				x = ox + x;
				y = oy - y;
				x1 = (S32)(ox * scale / 1000 + xorg);
				y1 = (S32)(oy * scale / 1000 + yorg);
				x2 = (S32)(x * scale / 1000 + xorg);
				y2 = (S32)(y * scale / 1000 + yorg);

				if ((dat1 == PAT_FM_END) || (dat1 == PAT_FM_FEND))
				{
					DrawLine(x1, y1, xorg, yorg, COLOR_LINE_EMPTY);
				}

				if ((dat1 == PAT_FM_EMPTY) || (dat1 == PAT_FM_EMP_ED))
				{
					DrawLine(x1, y1, x2, y2, COLOR_LINE_EMPTY);
				}
				else
				{
					if (patRule.l_line_udZhou == ENABLE)
					{
						DrawLine(x1, y1, x2, y2, COLOR_LINE_CHANGE);
					}
					else
					{
						if (patRule.l_line_speed == ENABLE)
						{
							if ((dat1 == PAT_FM_L))
							{
								DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW_L);
							}
							else if ((dat1 == PAT_FM_MD2))
							{
								DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW_MD2);
							}
							else if ((dat1 == PAT_FM_MD1))
							{
								DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW_MD1);
							}
							else if ((dat1 == PAT_FM_H))
							{
								DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW_H);
							}
						}
						else
						{
							DrawLine(x1, y1, x2, y2, COLOR_LINE_SEW);
						}
					}
				}

				if ((patRule.h_patMode & PM_SEWING))
				{
					if (k < HREG[HREG_FEED_C])
					{
						DrawLine(x1, y1, x2, y2, COLOR_LINE_FINISH);
					}
				}
				else if ((patRule.h_patMode & PM_PATTING))
				{
				}

				if ((patRule.l_ex_select_type & 0x01))
				{
					if (k >= g_jog_point && k < (g_jog_point + g_parameter))
					{
						DrawLine(x1, y1, x2, y2, COLOR_LINE_SELECT);
					}
				}
				else if ((patRule.l_ex_select_type & 0x02))
				{
					if (k >= g_jog_stpoint && k < g_jog_enpoint)
					{
						DrawLine(x1, y1, x2, y2, COLOR_LINE_SELECT);
					}
				}
				else if ((patRule.l_ex_select_type & 0x04))
				{
					if ((k >= g_jog_stpoint && k < g_jog_enpoint) ||
						(k >= g_jog_stpoint && k < g_jog_point &&
						 ((g_sta_end_flag == 1) || (g_sta_end_flag == 4))))
					{
						DrawLine(x1, y1, x2, y2, COLOR_LINE_SELECT);
					}
				}

				ox = x;
				oy = y;
				x = 0;
				y = 0;
				wdt();
				tmpPatAdd += 2;
			}
		}
		// /[绘制线 *end]*************************

		// /[绘制针点 *begin]*************************
		if (patRule.h_point == ENABLE)
		{
			U16 datLast = 0;
			tmpPatAdd = g_patd.pat_add;

			ox = 0;
			oy = 0;
			x = 0;
			y = 0;
			if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}

			for (k = 0; k < g_patd.feed_sum; k++)
			{
				dat1 = SREG[tmpPatAdd];
				dat = SREG[tmpPatAdd + 1];
				if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) ||
					(dat1 == PAT_FM_H) || (dat1 == PAT_FM_EMPTY) || (dat1 == PAT_FM_EMP_ED) || (dat1 == PAT_FM_END) || (dat1 == PAT_FM_FEND))
				{
					mdx = (U8)(dat);
					mdy = (U8)(dat >> 8);
				}
				else
				{
					mdx = 0;
					mdy = 0;
				}

				if (mdx >= 0x80)
				{
					mdx -= 0x80;
					x -= mdx;
				}
				else
					x += mdx;
				if (mdy >= 0x80)
				{
					mdy -= 0x80;
					y -= mdy;
				}
				else
					y += mdy;
				x = ox + x;
				y = oy - y;
				x1 = (S32)(ox * scale / 1000 + xorg);
				y1 = (S32)(oy * scale / 1000 + yorg);
				x2 = (S32)(x * scale / 1000 + xorg);
				y2 = (S32)(y * scale / 1000 + yorg);

				if ((dat1 == PAT_FM_L || dat1 == PAT_FM_MD2 || dat1 == PAT_FM_MD1 || dat1 == PAT_FM_H))
				{
					Draw_FullPoint(x1, y1, 1, COLOR_PIN_SEW);

					if ((patRule.h_patMode & PM_SEWING))
					{
						if (k < HREG[HREG_FEED_C])
						{
							Draw_FullPoint(x1, y1, 1, COLOR_PIN_FINISH);
						}
					}
					else if ((patRule.h_patMode & PM_PATTING))
					{
					}

					if ((patRule.l_ex_select_type & 0x01))
					{
						if (k >= g_jog_point && k < (g_jog_point + g_parameter))
						{
							Draw_FullPoint(x1, y1, 1, COLOR_PIN_SELECT);
						}
					}
					else if ((patRule.l_ex_select_type & 0x02))
					{
						if (k >= g_jog_stpoint && k < g_jog_enpoint)
						{
							Draw_FullPoint(x1, y1, 1, COLOR_PIN_SELECT);
						}
					}
					else if ((patRule.l_ex_select_type & 0x04))
					{
						if ((k >= g_jog_stpoint && k < g_jog_enpoint) ||
							(k >= g_jog_stpoint && k < g_jog_point &&
							 ((g_sta_end_flag == 1) || (g_sta_end_flag == 4))))
						{
							Draw_FullPoint(x1, y1, 1, COLOR_PIN_SELECT);
						}
					}
				}
				else if ((datLast == PAT_FM_L || datLast == PAT_FM_MD2 || datLast == PAT_FM_MD1 || datLast == PAT_FM_H))
				{
					Draw_FullPoint(x1, y1, 1, COLOR_PIN_SEW);

					if ((patRule.h_patMode & PM_SEWING))
					{
						if (k < HREG[HREG_FEED_C])
						{
							Draw_FullPoint(x1, y1, 1, COLOR_PIN_FINISH);
						}
					}
					else if ((patRule.h_patMode & PM_PATTING))
					{
					}
				}
				ox = x;
				oy = y;
				x = 0;
				y = 0;
				datLast = dat1;
				wdt();
				tmpPatAdd += 2;
			}
		}
		// /[绘制针点 *end]*************************

		// /[绘制原点 *begin]*************************
		if (patRule.h_org == ENABLE)
		{
			// /[绘制原点针点]*************************
			if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}
			else
			{
				ox = 0;
				oy = 0;
			}
			x2 = (S32)(ox * scale / 1000 + xorg);
			y2 = (S32)(oy * scale / 1000 + yorg);
			if (patRule.l_org_M_XL == 0x01)
			{
				Draw_FullPoint(x2, y2, 1, COLOR_ORG);
			}
			else if (patRule.l_org_M_XL == 0x02)
			{
				Draw_BigCross(x2, y2, COLOR_ORG);
			}
		}
		// /[绘制原点 *end]*************************

		// /[绘制基本代码 *begin]*************************
		if (patRule.h_code == ENABLE)
		{

			tmpPatAdd = g_patd.pat_add;
			ox = 0;
			oy = 0;
			x = 0;
			y = 0;

			// /[绘制原点针点]*************************
			if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}
			x2 = (S32)(ox * scale / 1000 + xorg);
			y2 = (S32)(oy * scale / 1000 + yorg);
			Draw_Code_BMP(x2, y2, 14, 14, BMP_H);

			for (k = 0; k < g_patd.feed_sum; k++)
			{
				dat1 = SREG[tmpPatAdd];
				dat = SREG[tmpPatAdd + 1];
				if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) ||
					(dat1 == PAT_FM_H) || (dat1 == PAT_FM_EMPTY) || (dat1 == PAT_FM_EMP_ED) || (dat1 == PAT_FM_END) || (dat1 == PAT_FM_FEND))
				{
					mdx = (U8)(dat);
					mdy = (U8)(dat >> 8);

					if (mdx >= 0x80)
					{
						mdx -= 0x80;
						x -= mdx;
					}
					else
						x += mdx;
					if (mdy >= 0x80)
					{
						mdy -= 0x80;
						y -= mdy;
					}
					else
						y += mdy;
					x = ox + x;
					y = oy - y;
					x1 = (S32)(ox * scale / 1000 + xorg);
					y1 = (S32)(oy * scale / 1000 + yorg);
					x2 = (S32)(x * scale / 1000 + xorg);
					y2 = (S32)(y * scale / 1000 + yorg);

					ox = x;
					oy = y;
					x = 0;
					y = 0;

					// if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) || (dat1 == PAT_FM_H))
					// {
					// 	dat1 = (SREG[tmpPatAdd] >> 8) & 0xff;
					// 	if (dat1 != 0)
					// 	{
					// 		Draw_Code_BMP(x2, y2, 14, 14, BMP_P);
					// 	}
					// }
				}
				else
				{
					U16 tmpCodeBMP = 0;
					mdx = 0;
					mdy = 0;
					switch (dat1)
					{
					case PAT_FM_H2:
						tmpCodeBMP = BMP_H2;
						break;
					case PAT_FM_T:
						tmpCodeBMP = BMP_T;
						break;
					case PAT_FM_U:
						tmpCodeBMP = BMP_U;
						break;
					case PAT_FM_D:
						tmpCodeBMP = BMP_D;
						break;
					case PAT_FM_J:
						tmpCodeBMP = BMP_J;
						break;
					case PAT_FM_C:
						tmpCodeBMP = BMP_C;
						break;
					case PAT_FM_P:
						tmpCodeBMP = BMP_P;
						break;
					case PAT_FM_F1:
						tmpCodeBMP = BMP_F1;
						break;
					case PAT_FM_F2:
						tmpCodeBMP = BMP_F2;
						break;
					case PAT_FM_F3:
						tmpCodeBMP = BMP_F3;
						break;
					case PAT_FM_F4:
						tmpCodeBMP = BMP_F4;
						break;
					case PAT_FM_F5:
						tmpCodeBMP = BMP_F5;
						break;
					case PAT_FM_F6:
						tmpCodeBMP = BMP_F6;
						break;
					case PAT_FM_F7:
						tmpCodeBMP = BMP_F7;
						break;
					case PAT_FM_F8:
						tmpCodeBMP = BMP_F8;
						break;
					}
					Draw_Code_BMP(x2, y2, 14, 14, tmpCodeBMP);
				}
				wdt();
				tmpPatAdd += 2;
			}
		}
		// /[绘制基本代码 *end]*************************

		// /[其他显示提示信息 *begin]*************************
		if (patRule.h_exCode == ENABLE)
		{
			///[显示多重缝段标号*begin]*************
			if (patRule.l_ex_pat == ENABLE && (rulesIndex == PATRULES_CONVERT1_OFFSET || rulesIndex == PATRULES_CONVERT1_OFFSMULT))
			{
				U32 draw_add_p = g_patd.draw_add; // 绘图起始地址
				U32 line_add_p = g_patd.line_add; // 线条起始地址
				U32 pat_add_p = g_patd.pat_add;	  //针起始地址
				U32 temp_draw_edAdd = g_draw_p;	  // 绘图终止地址
				U32 temp_line_edAdd = g_line_p;	  // 线条终止地址
				U16 lineNumTmp = 0, lineNum = 0;
				S16 tmpX = 0, tmpY = 0;
				U8 sameline_f;
				U32 code, dat3; //,dat2
				// U16 code1;
				S16 xx, yy;
				U8 partNum = 0;
				S16 oxTmp, oyTmp;
				ox = 0;
				oy = 0;
				if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
				{
					ox = (S16)(SREG[6]);
					oy = -(S16)(SREG[7]);
				}
				oxTmp = ox;
				oyTmp = oy;
				g_array5[0][0] = 0;
				while (draw_add_p < temp_draw_edAdd)
				{
					wdt(); // 喂狗
					code = SREG[draw_add_p];
					// code1 = SREG[draw_add_p + 1];

					dat = code & 0x3ff; //根据绘图格式,此与操作过后得到的dat均为绘制线条相关的指令

					//[多重缝段号记录*begin]***************************************************
					if ((code & 0x9800))
					{
						if (rulesIndex == PATRULES_CONVERT1_OFFSET)
						{
							if ((code & 0x9800) == 0x1800)
							{
								xx = ox;
								yy = oy;
								if ((xx == oxTmp) && (yy == oyTmp))
									xx = 1;
							}
							else
							{
								xx = 0;
								yy = 0;
							}
						}
						else if (rulesIndex == PATRULES_CONVERT1_OFFSMULT)
						{
							if ((code & 0x9800) == 0x0800 || (code & 0x9800) == 0x8800 || (code & 0x9800) == 0x1000 || (code & 0x9800) == 0x9000)
							{
								xx = ox;
								yy = oy;
								if ((xx == oxTmp) && (yy == oyTmp))
									xx = 1;
							}
							else
							{
								xx = 0;
								yy = 0;
							}
						}
					}
					else
					{
						xx = 0;
						yy = 0;
					}
					//[多重缝段号记录*end]***************************************************

					switch (dat)
					{
					case 0x0088: // 移送
						dat = (SREG[line_add_p] & 0x3fff);
						lineNumTmp = line_add_p;
						while (lineNumTmp < temp_line_edAdd)
						{
							dat1 = (SREG[lineNumTmp] & 0x3fff);
							if (dat != dat1)
								break;
							lineNumTmp++;
						}
						lineNum = lineNumTmp - line_add_p; //针数据的个数
						line_add_p = lineNumTmp;
						x = 0;
						y = 0;

						for (lineNumTmp = 0; lineNumTmp < lineNum; lineNumTmp++)
						{
							dat = SREG[pat_add_p + 1];
							x1 = (U8)(dat);
							y1 = (U8)((U16)(dat) >> 8);
							if (x1 >= 0x80)
							{
								x1 -= 0x80;
								x -= x1;
							}
							else
								x += x1;
							if (y1 >= 0x80)
							{
								y1 -= 0x80;
								y -= y1;
							}
							else
								y += y1;
							pat_add_p += 2;
						}
						x = ox + x;
						y = oy - y;
						ox = x;
						oy = y;
						xx = 0;
						yy = 0;
						break;
					case 0x00c4: // 点
						if (SREG[draw_add_p + 2] > 0x8000)
							tmpX = (0x10000 - SREG[draw_add_p + 2]) * (-1);
						else
							tmpX = SREG[draw_add_p + 2];
						if (SREG[draw_add_p + 3] > 0x8000)
							tmpY = (0x10000 - SREG[draw_add_p + 3]) * (-1);
						else
							tmpY = SREG[draw_add_p + 3];
						x = ox + tmpX;
						y = oy - tmpY;
						line_add_p++;
						pat_add_p += 2;
						ox = x;
						oy = y;
						xx = 0;
						yy = 0;
						break;
					case 0x00c8: // 直线
					case 0x00ca: // 直线_Z字缝
					case 0x01ce: // 园_Z字缝
					case 0x01d2: // 园弧_Z字缝
					case 0x00d6: // 曲线结束_Z字缝
						dat = (SREG[line_add_p] & 0x3fff);
						lineNumTmp = line_add_p;
						while (lineNumTmp < temp_line_edAdd)
						{
							dat1 = (SREG[lineNumTmp] & 0x3fff);
							if (dat != dat1)
								break;
							lineNumTmp++;
						}
						lineNum = lineNumTmp - line_add_p;
						line_add_p = lineNumTmp;
						x = 0;
						y = 0;
						for (lineNumTmp = 0; lineNumTmp < lineNum; lineNumTmp++)
						{
							dat1 = SREG[pat_add_p];
							dat = SREG[pat_add_p + 1];
							x1 = (U8)(dat);
							y1 = (U8)((U16)(dat) >> 8);
							dat = SREG[pat_add_p + 2];
							if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
							{
								dat = SREG[pat_add_p + 3];
								x2 = (U8)(dat);
								y2 = (U8)((U16)(dat) >> 8);
								if (((abs((S32)(x1)-x2)) < 2) && ((abs((S32)(y1)-y2)) < 2))
								{
									if ((lineNumTmp + 1) < lineNum)
										sameline_f = 1;
									else
										sameline_f = 0;
								}
								else
								{
									sameline_f = 0;
								}
							}
							else
							{
								sameline_f = 0;
							}
							if (x1 >= 0x80)
							{
								x1 -= 0x80;
								x -= x1;
							}
							else
								x += x1;
							if (y1 >= 0x80)
							{
								y1 -= 0x80;
								y -= y1;
							}
							else
								y += y1;
							if (sameline_f == 0)
							{
								x = ox + x;
								y = oy - y;
								if ((g_array5[0][0] + 1 == g_selectmode) && ((xx != 0) || (yy != 0))) //绘制选中的区段
								{
									x1 = (S32)(ox * scale / 1000 + xorg);
									y1 = (S32)(oy * scale / 1000 + yorg);
									x2 = (S32)(x * scale / 1000 + xorg);
									y2 = (S32)(y * scale / 1000 + yorg);
									// if ((dat1 == 0x3) || (dat1 == 0x1b))
									// 	;
									// else
									DrawLine(x1, y1, x2, y2, COLOR_LINE_SELECT);
								}
								ox = x;
								oy = y;
								x = 0;
								y = 0;
							}
							pat_add_p += 2;
						}
						break;
					case 0x00ce: // 园结束_Z字缝
					case 0x00d2: // 园弧结束_Z字缝
					case 0x01d6: // 曲线_Z字缝
					case 0x02d6: // 曲线_Z字缝
						xx = 0;
						yy = 0;
						break;
					case 0x01cc: // 园
					case 0x01d0: // 园弧
						dat1 = (SREG[draw_add_p + 4]) & 0x1ff;
						if (dat != (dat1 + 0x100))
							break;
						dat3 = code & 0xfe00;
						if (dat3 == 0x800)
							dat3 = 1;
						else if (dat3 == 0x1000)
							dat3 = 2;
						else if (dat3 == 0x1800)
							dat3 = 3;
						else if (dat3 == 0x8800)
							dat3 = 4;
						else if (dat3 == 0x9000)
							dat3 = 5;
						else
							dat3 = 0;

						dat = (SREG[line_add_p] & 0x3fff);
						lineNumTmp = line_add_p;
						while (lineNumTmp < temp_line_edAdd)
						{
							dat1 = (SREG[lineNumTmp] & 0x3fff);
							if (dat != dat1)
								break;
							lineNumTmp++;
						}
						lineNum = lineNumTmp - line_add_p;
						line_add_p = lineNumTmp;
						x = 0;
						y = 0;
						for (lineNumTmp = 0; lineNumTmp < lineNum; lineNumTmp++)
						{
							dat1 = SREG[pat_add_p];
							dat = SREG[pat_add_p + 1];
							x1 = (U8)(dat);
							y1 = (U8)((U16)(dat) >> 8);
							dat = SREG[pat_add_p + 2];
							if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
							{
								dat = SREG[pat_add_p + 3];
								x2 = (U8)(dat);
								y2 = (U8)((U16)(dat) >> 8);
								sameline_f = 0;
							}
							else
							{
								sameline_f = 0;
							}
							if (x1 >= 0x80)
							{
								x1 -= 0x80;
								x -= x1;
							}
							else
								x += x1;
							if (y1 >= 0x80)
							{
								y1 -= 0x80;
								y -= y1;
							}
							else
								y += y1;
							if (sameline_f == 0)
							{
								x = ox + x;
								y = oy - y;
								if ((g_array5[0][0] + 1 == g_selectmode) && ((xx != 0) || (yy != 0))) //绘制选中的区段
								{
									x1 = (S32)(ox * scale / 1000 + xorg);
									y1 = (S32)(oy * scale / 1000 + yorg);
									x2 = (S32)(x * scale / 1000 + xorg);
									y2 = (S32)(y * scale / 1000 + yorg);
									// if ((dat1 == 0x3) || (dat1 == 0x1b))
									// 	;
									// else
									DrawLine(x1, y1, x2, y2, COLOR_LINE_SELECT);
								}
								ox = x;
								oy = y;
								x = 0;
								y = 0;
							}
							pat_add_p += 2;
						}
						break;
					case 0x00cc: // 园结束
					case 0x00d0: // 园弧结束
						xx = 0;
						yy = 0;
						break;
					case 0x01d4: // 曲线
					case 0x00d4:
					case 0x02d4:
						if (((SREG[draw_add_p - 4] != 0x01d4) && (dat != 0x01d4)) || (dat == 0x01d4)) //修改了单独0x0d4,0x2d4开头不能正常显示的问题
						{
							lineNum = 1;
							if (dat != 0xd4)
							{
								while ((draw_add_p + 4) < temp_draw_edAdd)
								{
									dat = (SREG[draw_add_p + 4]) & 0x3fd;
									if ((dat != 0x1d4) && (dat != 0x2d4))
									{
										break;
									}
									lineNum++;
									draw_add_p += 4;
								}
								lineNum++;
								draw_add_p += 4;
							}

							if ((SREG[draw_add_p] & 0x1fc) != 0xd4)
								break;

							dat = (SREG[line_add_p] & 0x3fff);
							lineNumTmp = line_add_p;
							while (lineNumTmp < temp_line_edAdd)
							{
								dat1 = (SREG[lineNumTmp] & 0x3fff);
								if (dat != dat1)
									break;
								lineNumTmp++;
							}
							lineNum = lineNumTmp - line_add_p;
							line_add_p = lineNumTmp;
							x = 0;
							y = 0;
							for (lineNumTmp = 0; lineNumTmp < lineNum; lineNumTmp++)
							{
								dat1 = SREG[pat_add_p];
								dat = SREG[pat_add_p + 1];
								x1 = (U8)(dat);
								y1 = (U8)((U16)(dat) >> 8);
								dat = SREG[pat_add_p + 2];
								if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
								{
									dat = SREG[pat_add_p + 3];
									x2 = (U8)(dat);
									y2 = (U8)((U16)(dat) >> 8);
									sameline_f = 0;
								}
								else
								{
									sameline_f = 0;
								}
								if (x1 >= 0x80)
								{
									x1 -= 0x80;
									x -= x1;
								}
								else
									x += x1;
								if (y1 >= 0x80)
								{
									y1 -= 0x80;
									y -= y1;
								}
								else
									y += y1;
								if (sameline_f == 0)
								{
									x = ox + x;
									y = oy - y;
									if ((g_array5[0][0] + 1 == g_selectmode) && ((xx != 0) || (yy != 0))) //绘制选中的区段
									{
										x1 = (S32)(ox * scale / 1000 + xorg);
										y1 = (S32)(oy * scale / 1000 + yorg);
										x2 = (S32)(x * scale / 1000 + xorg);
										y2 = (S32)(y * scale / 1000 + yorg);
										// if ((dat1 == 0x3) || (dat1 == 0x1b))
										// 	;
										// else

										DrawLine(x1, y1, x2, y2, COLOR_LINE_SELECT);
									}

									ox = x;
									oy = y;
									x = 0;
									y = 0;
								}
								pat_add_p += 2;
							}
						}
						else
						{
							xx = 0;
							yy = 0;
						}
						break;
					case 0x00d8: // 多边形
					case 0x01d8: // 多边形结束
					case 0x00da: // 多边形_Z字缝
					case 0x01da: // 多边形结束_Z字缝
						i = 1;
						while ((draw_add_p + 4) < temp_draw_edAdd)
						{
							dat = (SREG[draw_add_p + 4]) & 0xfc;
							if (dat != 0xd8)
							{
								break;
							}
							i++;
							draw_add_p += 4;
						}
						dat = (SREG[line_add_p] & 0x3fff);
						dat += i;
						lineNumTmp = line_add_p;
						while (lineNumTmp < temp_line_edAdd)
						{
							dat1 = (SREG[lineNumTmp] & 0x3fff);

							if (dat <= dat1)
								break;
							lineNumTmp++;
						}
						lineNum = lineNumTmp - line_add_p;
						line_add_p = lineNumTmp;
						x = 0;
						y = 0;
						for (lineNumTmp = 0; lineNumTmp < lineNum; lineNumTmp++)
						{
							dat1 = SREG[pat_add_p];
							dat = SREG[pat_add_p + 1];
							x1 = (U8)(dat);
							y1 = (U8)((U16)(dat) >> 8);
							dat = SREG[pat_add_p + 2];
							if ((dat == dat1) || ((dat1 == 0x3) && (dat == 0x1b)))
							{
								dat = SREG[pat_add_p + 3];
								x2 = (U8)(dat);
								y2 = (U8)((U16)(dat) >> 8);
								if (((abs((S32)(x1)-x2)) < 2) && ((abs((S32)(y1)-y2)) < 2))
								{
									if ((lineNumTmp + 1) < lineNum)
										sameline_f = 1;
									else
										sameline_f = 0;
								}
								else
								{
									sameline_f = 0;
								}
							}
							else
							{
								sameline_f = 0;
							}
							if (x1 >= 0x80)
							{
								x1 -= 0x80;
								x -= x1;
							}
							else
								x += x1;
							if (y1 >= 0x80)
							{
								y1 -= 0x80;
								y -= y1;
							}
							else
								y += y1;
							if (sameline_f == 0)
							{
								x = ox + x;
								y = oy - y;
								//x1 = (S32)(ox*scale/1000+xorg); y1 = (S32)(oy*scale/1000+yorg);
								//x2 = (S32)(x*scale/1000+xorg); y2 = (S32)(y*scale/1000+yorg);
								//if((dat1==0x3)||(dat1==0x1b));//DrawLine(x1, y1, x2, y2,Color_DimGray);
								//else DrawLine(x1, y1, x2, y2,sewColor);
								ox = x;
								oy = y;
								x = 0;
								y = 0;
							}
							pat_add_p += 2;
						}
						break;

					default:
						xx = 0;
						yy = 0;
						break;
					}
					wdt(); // 喂狗	2010.3.27
					switch (code)
					{
					case 0x0401: // 针上停
					case 0x1401: // 针下停
					case 0x0601: // 次原点
					case 0x0701: // 跳缝
					case 0x1c01: // 伸延=物料厚 压板重 移送速度 重启动
					case 0x3a01: // 面线张力  2013.4.8
					case 0x0201: // 剪线
					case 0x0501: // 功能一
					case 0x0b01: // 功能二
					case 0x0c01: // 功能三
					case 0x0d01: // 功能四
					case 0x3c01: // 针角度
					case 0x3d01: // 参数一
					case 0x3e01: // 参数二
						line_add_p++;
						pat_add_p += 2;
						break;
					case 0x1d01: // 功能五 六 七 八
						// if ((code1 == 0x03) && (g_UdZhouFlag == 3))
						// { //换针
						// 	if (sewColor == COLOR_LINE_SEW)
						// 		sewColor = COLOR_LINE_CHANGE;
						// 	else
						// 		sewColor = COLOR_LINE_SEW;
						// }
						line_add_p++;
						pat_add_p += 2;
						break;

					case 0x1e01: // 结束
					case 0x1f01: // 结束
						ox = x;
						oy = y;
						break;
					default:
						break;
					}

					draw_add_p += 4;

					if ((xx != 0) || (yy != 0)) // 记录多重缝、补助缝的启点位置及编号
					{
						partNum++;
						g_array5[partNum][0] = xx;
						g_array5[partNum][1] = yy;
						g_array5[0][0] = partNum;
					}
				}
				///[重新盖写指令代码]************************************
				if (patRule.h_code == ENABLE)
				{

					tmpPatAdd = g_patd.pat_add;
					ox = 0;
					oy = 0;
					x = 0;
					y = 0;

					// /[绘制原点针点]*************************
					if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
					{
						ox = (S16)(SREG[6]);
						oy = -(S16)(SREG[7]);
					}
					x2 = (S32)(ox * scale / 1000 + xorg);
					y2 = (S32)(oy * scale / 1000 + yorg);
					Draw_Code_BMP(x2, y2, 14, 14, BMP_H);

					for (k = 0; k < g_patd.feed_sum; k++)
					{
						dat1 = SREG[tmpPatAdd];
						dat = SREG[tmpPatAdd + 1];
						if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) ||
							(dat1 == PAT_FM_H) || (dat1 == PAT_FM_EMPTY) || (dat1 == PAT_FM_EMP_ED) || (dat1 == PAT_FM_END) || (dat1 == PAT_FM_FEND))
						{
							mdx = (U8)(dat);
							mdy = (U8)(dat >> 8);

							if (mdx >= 0x80)
							{
								mdx -= 0x80;
								x -= mdx;
							}
							else
								x += mdx;
							if (mdy >= 0x80)
							{
								mdy -= 0x80;
								y -= mdy;
							}
							else
								y += mdy;
							x = ox + x;
							y = oy - y;
							x1 = (S32)(ox * scale / 1000 + xorg);
							y1 = (S32)(oy * scale / 1000 + yorg);
							x2 = (S32)(x * scale / 1000 + xorg);
							y2 = (S32)(y * scale / 1000 + yorg);

							ox = x;
							oy = y;
							x = 0;
							y = 0;

							// if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) || (dat1 == PAT_FM_H))
							// {
							// 	dat1 = (SREG[tmpPatAdd] >> 8) & 0xff;
							// 	if (dat1 != 0)
							// 	{
							// 		Draw_Code_BMP(x2, y2, 14, 14, BMP_P);
							// 	}
							// }
						}
						else
						{
							U16 tmpCodeBMP = 0;
							mdx = 0;
							mdy = 0;
							switch (dat1)
							{
							case PAT_FM_H2:
								tmpCodeBMP = BMP_H2;
								break;
							case PAT_FM_T:
								tmpCodeBMP = BMP_T;
								break;
							case PAT_FM_U:
								tmpCodeBMP = BMP_U;
								break;
							case PAT_FM_D:
								tmpCodeBMP = BMP_D;
								break;
							case PAT_FM_J:
								tmpCodeBMP = BMP_J;
								break;
							case PAT_FM_C:
								tmpCodeBMP = BMP_C;
								break;
							case PAT_FM_P:
								tmpCodeBMP = BMP_P;
								break;
							case PAT_FM_F1:
								tmpCodeBMP = BMP_F1;
								break;
							case PAT_FM_F2:
								tmpCodeBMP = BMP_F2;
								break;
							case PAT_FM_F3:
								tmpCodeBMP = BMP_F3;
								break;
							case PAT_FM_F4:
								tmpCodeBMP = BMP_F4;
								break;
							case PAT_FM_F5:
								tmpCodeBMP = BMP_F5;
								break;
							case PAT_FM_F6:
								tmpCodeBMP = BMP_F6;
								break;
							case PAT_FM_F7:
								tmpCodeBMP = BMP_F7;
								break;
							case PAT_FM_F8:
								tmpCodeBMP = BMP_F8;
								break;
							}
							Draw_Code_BMP(x2, y2, 14, 14, tmpCodeBMP);
						}
						wdt();
						tmpPatAdd += 2;
					}
				}
				///[绘制多段缝-离边缝控制段序号]************************************
				if (g_array5[0][0])
				{
					U32 colortmp = 0;
					for (i = 1; i <= g_array5[0][0]; i++)
					{
						x1 = (S16)(g_array5[i][0]);
						y1 = (S16)(g_array5[i][1]);
						x1 = (S32)(x1 * scale / 1000 + xorg);
						y1 = (S32)(y1 * scale / 1000 + yorg);
						if (i == g_selectmode)
						{
							colortmp = COLOR_PIN_SELECT;
						}
						else
						{
							colortmp = COLOR_PART_N;
						}
						Draw_Code_Num(x1, y1, i, colortmp);
					}
				}
			}
			///[显示多重缝段标号*end]*************

			///[选择触摸周围点使用,初始化参数*begin]*************
			if (patRule.l_ex_selectNear == ENABLE && patRule.l_ex_topNear == DISABLE)
			{
				if ((inxpos == 0) && (inypos == 0)) //不更新输入点
				{
					disxpoint = 0;
					disypoint = 0;
				}
			}
			///[选择触摸周围点使用,初始化参数*end]*************

			tmpPatAdd = g_patd.pat_add;

			ox = 0;
			oy = 0;
			x = 0;
			y = 0;

			if (patRule.h_orgOffset == ENABLE && SREG[3] == 7)
			{
				ox = (S16)(SREG[6]);
				oy = -(S16)(SREG[7]);
			}

			x2 = (S32)(ox * scale / 1000 + xorg);
			y2 = (S32)(oy * scale / 1000 + yorg);
			///[获取选中区段点-原点显示*begin]*******************
			if ((patRule.l_ex_select == ENABLE))
			{
				if ((patRule.l_ex_select_mode & 0x02))
				{
					Draw_BigCross(x2, y2, COLOR_ORG);
				}
				else if ((patRule.l_ex_select_mode & 0x01))
				{
					Draw_Code_HSEP(x2, y2, PAT_CODEH, COLOR_ORG);
				}
			}
			///[获取选中区段点-原点显示*end]*******************

			for (k = 0; k < g_patd.feed_sum; k++)
			{

				dat1 = SREG[tmpPatAdd];
				dat = SREG[tmpPatAdd + 1];
				if ((dat1 == PAT_FM_L) || (dat1 == PAT_FM_MD2) || (dat1 == PAT_FM_MD1) ||
					(dat1 == PAT_FM_H) || (dat1 == PAT_FM_EMPTY) || (dat1 == PAT_FM_EMP_ED) || (dat1 == PAT_FM_END) || (dat1 == PAT_FM_FEND))
				{
					mdx = (U8)(dat);
					mdy = (U8)(dat >> 8);
				}
				else
				{
					mdx = 0;
					mdy = 0;
				}

				if (mdx >= 0x80)
				{
					mdx -= 0x80;
					x -= mdx;
				}
				else
					x += mdx;
				if (mdy >= 0x80)
				{
					mdy -= 0x80;
					y -= mdy;
				}
				else
					y += mdy;
				x = ox + x;
				y = oy - y;
				x1 = (S32)(ox * scale / 1000 + xorg);
				y1 = (S32)(oy * scale / 1000 + yorg);
				x2 = (S32)(x * scale / 1000 + xorg);
				y2 = (S32)(y * scale / 1000 + yorg);

				// /[绘制相对点控制点-非绘图模式下*begin]*****************
				if (patRule.l_ex_ctrl == ENABLE) //绘制控制点
				{
					if (patRule.l_ex_ctrl_type == 1) //修改花样ADD-段修改-显示多点控制
					{
						if (g_jogmode == 0)
						{
							for (i = 1; i < g_point_num; i++)
							{
								if (k == g_array5[300 + i][0])
								{
									k1 = (S16)(g_array5[i][0]);
									k2 = (S16)(g_array5[i][1]);
									x2 = (S32)((ox + k1) * scale / 1000 + xorg);
									y2 = (S32)((oy - k2) * scale / 1000 + yorg);
									if ((patRule.l_ex_ctrl_mode & 0x01))
										Draw_Code_Num(x2, y2, i, COLOR_CTRL);
									else if ((patRule.l_ex_ctrl_mode & 0x02))
										Draw_BigCross(x2, y2, COLOR_CTRL);
									else if ((patRule.l_ex_ctrl_mode & 0x04))
										Draw_SmallCross(x2, y2, COLOR_CTRL);
								}
							}
						}
						else if (g_jogmode == 1)
						{
							if ((g_jogmode == 1))
							{
								if (k == g_jog_stpoint)
								{
									k1 = ox;
									k2 = oy;
									for (i = 1; i < g_point_num; i++)
									{
										k1 += (S16)(g_array5[i][0]);
										k2 -= (S16)(g_array5[i][1]);
										x2 = (S32)(k1 * scale / 1000 + xorg);
										y2 = (S32)(k2 * scale / 1000 + yorg);
										if ((patRule.l_ex_ctrl_mode & 0x01))
											Draw_Code_Num(x2, y2, i, COLOR_CTRL);
										else if ((patRule.l_ex_ctrl_mode & 0x02))
											Draw_BigCross(x2, y2, COLOR_CTRL);
										else if ((patRule.l_ex_ctrl_mode & 0x04))
											Draw_SmallCross(x2, y2, COLOR_CTRL);
									}
								}
							}
						}
					}
					else if (patRule.l_ex_ctrl_type == 2) //变换- 缩放，旋转-选择基准点
					{
						if ((g_datum_mark == 0))
						{
							if (k == g_jog_point)
							{
								x2 = (S32)((ox + g_dxpos) * scale / 1000 + xorg);
								y2 = (S32)((oy - g_dypos) * scale / 1000 + yorg);
								Draw_Code_HSEP(x2, y2, PAT_CODEP, COLOR_CODE_C);
							}
						}
					}
				}
				// /[绘制相对点控制点-非绘图模式下*end]*****************

				// /[获取选中区段点的位置*begin]*****************
				if ((patRule.l_ex_select == ENABLE))
				{
					if (k == g_jog_enpoint && k != 0 && patRule.l_ex_select_type != 0x00) //记录结束点-结束点不为0
					{
						disE = 1;
						tmpxE = (S32)(ox * scale / 1000 + xorg);
						tmpyE = (S32)(oy * scale / 1000 + yorg);
					}
					if (k == g_jog_stpoint && patRule.l_ex_select_type != 0x00) //记录开始点
					{
						disS = 1;
						tmpxS = (S32)(ox * scale / 1000 + xorg);
						tmpyS = (S32)(oy * scale / 1000 + yorg);
					}
					if (k == g_jog_point) //记录当前点位置
					{
						disP = 1;
						tmpx2 = (S32)(ox * scale / 1000 + xorg);
						tmpy2 = (S32)(oy * scale / 1000 + yorg);
					}
				}
				// /[获取选中区段点的位置*begin]*****************

				// /[选择触摸周围点使用，获取点位置*begin]*****************
				if (patRule.l_ex_selectNear == ENABLE && patRule.l_ex_topNear == DISABLE)
				{
					if ((g_ImgPoint == 1)) // /[记录选中点]*****************
					{
						txpoint = (S32)(ox * scale / 1000 + xorg);
						typoint = (S32)(oy * scale / 1000 + yorg);
						if ((k) == *infeed) //记录原来的点的位置
						{
							xpoint1 = txpoint;
							ypoint1 = typoint;
						}
						if ((((abs(txpoint - inxpos) * abs(txpoint - inxpos) + abs(typoint - inypos) * abs(typoint - inypos)) < (disxpoint * disxpoint + disypoint * disypoint))) &&
							(k >= g_min_feedsum) && (k <= g_max_feedsum)) //选中点合规时,记录当前选中点的位置
						{
							xpoint = txpoint;
							ypoint = typoint;
							disxpoint = abs(xpoint - inxpos); //获得了新的，缩小后的范围(可以优化到直接与dxp进行比较)
							disypoint = abs(ypoint - inypos);
							tmpfeed1 = k;
						}
					}
				}
				// /[选择触摸周围点使用，获取点位置*end]*****************

				ox = x;
				oy = y;
				x = 0;
				y = 0;
				wdt();
				tmpPatAdd += 2;
			}

			// /[选择触摸周围点使用，绘制点*begin]*****************
			if (patRule.l_ex_selectNear == ENABLE && patRule.l_ex_topNear == DISABLE)
			{
				if ((g_ImgPoint == 1))
				{
					if ((disxpoint < dxp) && (disypoint < dyp) &&
						((inxpos != 0) || (inypos != 0)) && (tmpfeed1 >= g_min_feedsum) && (tmpfeed1 <= g_max_feedsum))
					{
						*infeed = tmpfeed1;
						xpoint1 = xpoint;
						ypoint1 = ypoint;
					}
					Draw_BigCross(xpoint1, ypoint1, COLOR_SELECT_P);
				}
			}
			// /[选择触摸周围点使用，绘制点*end]*****************

			// /[显示选中区段点的位置*begin]*****************
			if ((patRule.l_ex_select == ENABLE))
			{
				if ((patRule.l_ex_select_mode & 0x02))
				{
					if (disE) //显示结束点位置
					{
						Draw_BigCross(tmpxE, tmpyE, COLOR_SELECT_E);
					}
					if (disS) //显示起始点位置
					{
						Draw_BigCross(tmpxS, tmpyS, COLOR_SELECT_S);
					}
					if (disP) //显示当前点位置
					{
						ReadPointData2(tmpx2 - 3, tmpy2 - 3);
						ReadPointData1(tmpx2 - 3, tmpy2 - 3);
						Draw_BigCross(tmpx2, tmpy2, COLOR_SELECT_P);
					}
				}
				if ((patRule.l_ex_select_mode & 0x01))
				{
					if (disE) //显示结束点位置
					{
						Draw_Code_HSEP(tmpxE, tmpyE, PAT_CODEE, COLOR_SELECT_E);
					}
					if (disS) //显示起始点位置
					{
						Draw_Code_HSEP(tmpxS, tmpyS, PAT_CODES, COLOR_SELECT_S);
					}
					if (disP) //显示当前点位置
					{
						ReadPointData2(tmpx2 - 7, tmpy2 - 7);
						ReadPointData1(tmpx2 - 7, tmpy2 - 7);
						Draw_Code_HSEP(tmpx2, tmpy2, PAT_CODEP, COLOR_SELECT_P);
					}
				}
			}
			// /[显示选中区段点的位置*end]*****************
		}
		// /[其他显示提示信息 *end]*************************
	}

	// /[提示光标周围点使用，绘制点*begin]*************************
	if (patRule.h_exCode == ENABLE && patRule.l_ex_topNear == ENABLE && patRule.l_ex_selectNear == DISABLE)
	{
		if ((inxpos != xorg || inypos != yorg) &&
			(inxpos > patConvas.conPosX && inxpos < patConvas.conPosX + patConvas.conSizeX) &&
			(inypos > patConvas.conPosY && inypos < patConvas.conPosY + patConvas.conSizeY))
		{
			Draw_BigCross(inxpos, inypos, COLOR_SELECT_TOP);
		}
	}
	// /[提示光标周围点使用，绘制点*end]*************************

	// /[绘制当前移动光标(绘图模式下)与移动光标间的控制点 *begin]*************************
	if (patRule.h_exCode == ENABLE && patRule.l_ex_cursor == ENABLE)
	{
		if ((patRule.h_patMode & PM_PATTING))
		{
			if (patRule.l_ex_cursor_type == 4)
			{
				getFeedXY1(&x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 5)
			{

				getFeedXY(g_feedc, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);

				Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);

				if (patRule.l_ex_ctrl == ENABLE && patRule.l_ex_ctrl_type == 5)
				{
					if (F_FILETYPE == PAT_MIT && g_point_num > 1)
					{
						for (i = 1; i < g_point_num; i++)
						{
							x += (S16)g_array4[i][0];
							y += (S16)g_array4[i][1];
							x1 = (S32)(x * scale / 1000 + xorg);
							y1 = (S32)(-y * scale / 1000 + yorg);
							if (patRule.l_ex_ctrl_mode == 0x01)
							{
								Draw_Code_Num(x1, y1, i, COLOR_CTRL);
							}
							else
							{
								Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);
							}
						}
					}
				}
				x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
				y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 6)
			{
				getFeedXY(g_feedc, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 2)
			{
				getFeedXY(g_jog_point, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);

				x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
				y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 7)
			{
				getFeedXY(g_jog_point, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);

				x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
				y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 8)
			{

				// getFeedXY(g_jog_stpoint, &x, &y);
				// x1 = (S32)(x * scale / 1000 + xorg);
				// y1 = (S32)(-y * scale / 1000 + yorg);
				// Draw_SmallCross(x1, y1, COLOR_SELECT_S);

				// getFeedXY(g_jog_enpoint, &x, &y);
				// x1 = (S32)(x * scale / 1000 + xorg);
				// y1 = (S32)(-y * scale / 1000 + yorg);
				// Draw_SmallCross(x1, y1, COLOR_SELECT_E);

				getFeedXY(g_jog_point, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);

				x1 = (S32)((x + mut * g_dxpos) * scale / 1000 + xorg);
				y1 = (S32)(-(y + mut * g_dypos) * scale / 1000 + yorg);
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 9)
			{

				// getFeedXY(g_jog_enpoint, &x, &y);
				// x1 = (S32)(x * scale / 1000 + xorg);
				// y1 = (S32)(-y * scale / 1000 + yorg);
				// Draw_SmallCross(x1, y1, COLOR_SELECT_E);

				// getFeedXY(g_jog_stpoint, &x, &y);
				// x1 = (S32)(x * scale / 1000 + xorg);
				// y1 = (S32)(-y * scale / 1000 + yorg);
				// Draw_SmallCross(x1, y1, COLOR_SELECT_S);

				getFeedXY(g_jog_point, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);

				if (patRule.l_ex_ctrl == ENABLE && patRule.l_ex_ctrl_type == 9)
				{
					if (F_FILETYPE == PAT_MIT)
					{
						k1 = 0;
						k2 = 0;
						for (k = 1; k < g_point_num; k++)
						{
							k1 = (S16)(g_array5[k][0]);
							k2 = (S16)(g_array5[k][1]);
							x2 = (S32)((x + k1) * scale / 1000 + xorg);
							y2 = (S32)(-(y + k2) * scale / 1000 + yorg);
							Draw_SmallCross(x2, y2, COLOR_CTRL);
						}
					}
					else if (F_FILETYPE == PAT_SEW)
					{
						for (k = 1; k < g_point_num; k++)
						{
							k1 = (S16)(g_array10[k][0]);
							k2 = (S16)(g_array10[k][1]);
							x2 = (S32)((k1)*scale / 1000 + xorg);
							y2 = (S32)(-(k2)*scale / 1000 + yorg);
							Draw_SmallCross(x2, y2, COLOR_CTRL);
						}
					}
				}
				x2 = (S32)(xorg + (x + mut * g_dxpos) * scale / 1000);
				y2 = (S32)(yorg - (y + mut * g_dypos) * scale / 1000);
				ReadPointData2(x2 - 4, y2 - 4);
				Draw_SmallCross(x2, y2, COLOR_CROSS);
			}
			else if (patRule.l_ex_cursor_type == 10)
			{

				// getFeedXY(g_jog_enpoint, &x, &y);
				// x1 = (S32)(x * scale / 1000 + xorg);
				// y1 = (S32)(-y * scale / 1000 + yorg);
				// Draw_SmallCross(x1, y1, COLOR_SELECT_E);

				getFeedXY(g_jog_stpoint, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				Draw_SmallCross(x1, y1, COLOR_CROSS_BEF);

				if (patRule.l_ex_ctrl == ENABLE && patRule.l_ex_ctrl_type == 10)
				{
					if (F_FILETYPE == PAT_MIT)
					{
						k1 = 0;
						k2 = 0;
						for (k = 1; k < g_point_num; k++)
						{
							k1 += (S16)(g_array5[k][0]);
							k2 += (S16)(g_array5[k][1]);
							x2 = (S32)((x + k1) * scale / 1000 + xorg);
							y2 = (S32)(-(y + k2) * scale / 1000 + yorg);
							Draw_SmallCross(x2, y2, COLOR_CTRL);
						}
						x1 = (S32)((x + k1 + g_dxpos) * scale / 1000 + xorg);
						y1 = (S32)(-(y + k2 + g_dypos) * scale / 1000 + yorg);
					}
					else if (F_FILETYPE == PAT_SEW)
					{
						k1 = x;
						k2 = y;
						for (k = 1; k < g_point_num; k++)
						{
							k1 = (S16)(g_array10[k][0]);
							k2 = (S16)(g_array10[k][1]);
							x2 = (S32)((k1)*scale / 1000 + xorg);
							y2 = (S32)(-(k2)*scale / 1000 + yorg);
							Draw_SmallCross(x2, y2, COLOR_CTRL);
						}
						k1 += mut * g_dxpos;
						k2 += mut * g_dypos;
						x1 = (S32)(k1 * scale / 1000 + xorg);
						y1 = (S32)(-k2 * scale / 1000 + yorg);
					}
				}
				ReadPointData2(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
		}
		else if ((patRule.h_patMode & PM_SEWING))
		{
			if (patRule.l_ex_cursor_type == 1)
			{
				getFeedXY(F_PT_FEED_C, &x, &y);
				x1 = (S32)(x * scale / 1000 + xorg);
				y1 = (S32)(-y * scale / 1000 + yorg);
				ReadPointData1(x1 - 4, y1 - 4);
				Draw_SmallCross(x1, y1, COLOR_CROSS);
			}
		}
	}
	// /[绘制当前移动光标(绘图模式下)与移动光标间的控制点 *end]*************************
	pat_resetDrawXY();
#endif
}
