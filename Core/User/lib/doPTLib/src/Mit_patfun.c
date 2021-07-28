/*
 * @Author: your name
 * @Date: 2021-05-07 11:31:06
 * @LastEditTime: 2021-05-25 14:34:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\src\menu_create.c
 */
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "function.h"
#include "uart.h"
#include "color.h"
#include "communication.h"
#include "ds2460.h"
#include "disk_oper.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"
#include "Patfun.h"
#include "viewPattern.h"
#include "Toast.h"

extern U16 SheetPage;
extern volatile U32 g_temp_pno;
extern volatile U8 g_init_flag;
extern volatile U8 g_modifyflag;
extern MachineInfo g_Mac;
extern volatile U8 g_ImageDis;
extern volatile U8 g_NoEndCode;
extern volatile S16 g_pf_absdis;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos;
extern volatile S16 g_tmpox, g_tmpoy;
extern volatile U16 g_savetimes;
extern volatile U16 g_Tsavetimes;
extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U32 g_jog_point;
extern volatile S8 g_pf_Set_Offest; //修改
extern volatile U8 g_reorigin;
extern volatile U16 g_selectdisno;
extern volatile U8 coldstart;
extern volatile U8 g_code_para;
extern volatile U16 g_tmp_PfootValSet;
extern volatile U8 g_disppicflag;
extern volatile U8 g_MboardUSB;
extern volatile U8 g_disk_memory;
extern volatile U8 g_UdZhouFlag;
extern volatile U16 g_array4[386][2];
extern volatile U16 g_array5[630][2];
extern volatile U8 g_tExCode;
extern volatile U8 g_ExCode;
extern volatile U8 g_curExCode;
extern volatile U8 g_savemodify;
extern volatile U8 g_BorSTflag;
extern volatile U8 g_ntohome;
extern volatile U8 g_needtohome;
extern volatile U8 g_endtype;
extern volatile S16 g_maxX;
extern volatile S16 g_minX;
extern volatile S16 g_maxY;
extern volatile S16 g_minY;
extern volatile U16 g_curPinAng;     //当前状态针角度值
extern volatile U16 g_pointPinAng;   //针数对应角度值
extern volatile U16 g_curSetPara1;   //当前状态设置一参数
extern volatile U16 g_pointSetPara1; //针数对应设置二参数值
extern volatile U16 g_curSetPara2;   //当前状态设置二参数
extern volatile U16 g_pointSetPara2; //针数对应设置二参数值
extern volatile U8 g_modedat;
extern volatile U16 g_mode1dat[3];
extern volatile S16 g_mode2dat[5];
extern const U16 draw_code[][2];
extern const U16 pat_code[];
extern volatile U8 g_BorAddFuns; //插入附加功能 每一个位代表一个功能
extern volatile U8 g_BorDelFuns; //删除附加功能 每一个位代表一个功能
extern volatile U8 g_DoTrim;     // 删除剪线标志 1是删除, 0 是无删除
extern volatile U16 g_Ptya;      //寸动调速值	2010.4.6
extern volatile U16 g_point_num; //控制点数
extern volatile U8 g_jogfunction;
extern volatile U8 g_jogfunction1;
extern volatile U32 g_jognumber;
extern volatile U8 g_input_method;    //输入模式 0-空送 1-点 2-直线 3-折线 4-圆 5-圆弧 6-曲线
extern volatile U8 g_para_offset;     //多重缝 0-无多重缝 1-同向空送多重缝 2-逆向空送多重缝 3-离边 4-同向缝纫多重缝 5-逆向缝纫多重缝
extern volatile U8 g_direction;       //多重缝方向
extern volatile U8 g_distance;        //多重缝距离
extern volatile U8 g_times;           //多重缝次数
extern volatile U8 g_zig_zag;         //Z字缝
extern volatile U8 g_zig_width;       //Z缝宽度
extern volatile U8 g_zig_pitch;       //Z缝针距
extern volatile U8 g_zig_direction;   //Z缝方向
extern volatile U8 g_bt_mode;         //倒缝
extern volatile U8 g_repeat;          //重叠缝次数
extern volatile U8 g_r_stitch;        //重叠缝针数
extern volatile U8 g_srt_bt;          //前端倒缝次数
extern volatile U8 g_s_stitch;        //前端倒缝针数
extern volatile U8 g_ent_bt;          //后端倒缝次数
extern volatile U8 g_e_stitch;        //后端倒缝针数
extern volatile U32 g_ct_status_flag; //打板状态标志
extern volatile U8 g_ct_insert_flag;  //打板插入标志
extern volatile U8 g_feed_flag;       //打板空送标志
extern volatile U32 g_feedc;          //当前针
extern volatile U32 g_feedsum;        //总针
extern volatile U32 g_feedpat;
extern volatile U8 g_code_mode;
extern volatile U8 g_feed_sew;                                    // 前一针的空送或缝纫标志
extern volatile S16 g_dxpos, g_dypos;                             //x,y坐标变化值
extern volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //x,y坐标实际值
extern volatile S16 g_disxpos, g_disypos;                         //x,y坐标实际值
extern volatile S16 g_tmpoxpos, g_tmpoypos;
extern volatile S16 g_temp_oxpos, g_temp_oypos;
extern volatile U8 g_dopfoot;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern volatile U16 g_temp_draw_p;
extern volatile U16 g_temp_line_p;
extern volatile U32 g_temp_pat_p;
extern volatile U16 g_line_sum;
extern volatile U8 g_editflag;
extern volatile S8 g_lf_dis_modify;
extern volatile S8 g_pf_dis_modify;
extern volatile U8 g_pf_modify_mode;
extern volatile U8 g_oldspeed;
extern volatile U8 g_speed; // 打版时选择的速度级别H MH ML L		20151211
extern volatile U8 g_pitch;
extern volatile U8 g_lineforceValSetNow; // 当前缝纫面线张力值
extern volatile U8 g_mitdotack;
extern volatile U8 g_originf; // 花样结束点位置标志 0：不在原点位置 1: 在原点位置
extern volatile U8 g_num_mov_key;
extern volatile S8 g_pf_dis;     //中压脚修正值 offest
extern volatile S16 g_pf_absval; //中压脚当前绝对值(包括基准值)
extern volatile U8 g_pf_mode;    //中压脚	1:有设置 0：无设置
extern volatile S16 g_pf_bdis;
extern volatile U16 g_pinAng;
extern volatile U8 g_zrota;
extern volatile U8 g_jogmode;
extern OLDPAT1 g_dispat1;
//volatile U8  g_isBAT;	//打板跳缝标志
extern volatile U8 g_lineforceValSet; // 面线张力基准值
extern volatile U8 g_fbmac;
extern volatile U8 g_blkInset;
extern volatile U8 g_nolimt;
extern volatile U32 g_blkInsetlen;
extern volatile U16 g_ctAddpins;
extern volatile U8 g_selectmode;
extern volatile U16 g_temp_point_num;
extern volatile U8 g_ModToCt;
extern volatile U8 g_modifyflag1;
extern SEW_CHECK g_sewcheck;
extern volatile U8 g_zhouDown;
extern volatile S16 g_modcordflag;

extern const unsigned int g_array1[];
extern volatile U16 g_array2[307][2];
extern volatile S32 g_array3[2][2];

extern volatile U8 g_inserflag, g_inserflag1;     //区段插入标志
extern volatile U32 g_inserDadrr0, g_inserDadrr1; //区段插入绘图数据起始结束地址
//------------------------------------------
// Function   : U8 new_Create_Data(U8 mode)
// Description: 生成加工数据
//------------------------------------------
U8 new_Create_Data(U8 mode)
{
    U32 x, y;
    S32 x0, y0, x1, y1, k1, k2;
    U32 ii, jj, kk;
    U32 d /*, d1, n*/, pn, l, dat = 0;
    U32 add = 0, add1 = 0, add2 = 0, add3 = 0, add4 = 0;
    U16 tmp_line_sum = 0;
    U8 occlude, cutf; //多边形是否封闭标志

    U8 tfeed_flag = 0 /*, tpara_offset = 0*/;//修改，添加
    U16 tpoint_num = 0, tfeedsum = 0, tfeedc = 0, tfeed = 0;//修改，添加

    COORDINATE POSI;
    U8 insert = g_inserflag1;
    //U16 time0pins = 0;
    if (g_para_offset > 5)
        g_para_offset = 0;

    if (g_point_num == 1)
    {
        g_array4[0][0] = 0;
        g_temp_oxpos = g_oxpos;
        g_temp_oypos = g_oypos;
        g_temp_draw_p = g_draw_p;
        g_temp_line_p = g_line_p;
        g_temp_pat_p = g_pat_p;
        if ((g_feed_flag) && (g_input_method != 1) && (mode == 1))
        {
            if ((g_bt_mode == 1) && (g_s_stitch > 0))
            {
                SREG[g_draw_p++] = 0x2401;
                SREG[g_draw_p++] = ((U16)(g_s_stitch << 8)) + (g_srt_bt + 1) * 2;
                SREG[g_draw_p++] = 0;
                SREG[g_draw_p++] = 0;
            }
        }
    }
    if (mode == 1)
    {
        SREG[g_draw_p + 2] = g_dxpos;
        SREG[g_draw_p + 3] = g_dypos;
    }
    else if ((mode == 2) && (g_input_method > 2))
    {
        g_temp_oxpos = g_oxpos;
        g_temp_oypos = g_oypos;
    }

    wdt();
    if ((g_input_method == 1) || (g_input_method == 2))
    { //空送/点/直线
        tfeed_flag = g_feed_flag;
        tpoint_num = g_point_num;
        // tpara_offset = g_para_offset;
        tfeedsum = g_patd.feed_sum;
        //if((!g_feed_flag)&&SREG[52000+g_feedc*2]==0x02){	//剪线
        //	g_feedc++;
        //}
        tfeedc = g_feedc;
        g_jog_stpoint = g_feedc;
        g_jog_enpoint = g_feedc;

        g_array4[g_point_num][0] = g_dxpos; //记录控制点的绝对坐标
        g_array4[g_point_num][1] = g_dypos;
        g_array5[g_point_num][0] = g_dxpos; //记录控制点相对坐标
        g_array5[g_point_num][1] = g_dypos;

        //printf("SRWG[%d] = %04x  \r\n",52000+g_feedc*2,SREG[52000+g_feedc*2]);
        //printf("*g_init_flag = %d  g_draw_p = %d  g_line_p = %d g_pat_p = %d\r\n",g_init_flag,g_draw_p,g_line_p,g_pat_p);
        //printf("*g_init_flag = %d  g_patd.draw_add  = %d  g_patd.line_add = %d g_patd.pat_add  = %d\r\n",g_init_flag,g_patd.draw_add ,g_patd.line_add,g_patd.pat_add );
        //
        //printf("0 g_feed_flag = %d  g_input_method = %d  g_point_num = %d \r\n",g_feed_flag,g_input_method,g_point_num);
        //printf("--------------------------------------\r\n");
        //printf("-0 g_patd.feed_sum = %d \r\n",g_patd.feed_sum);
        g_nolimt = 1;
        mit_CtToMod();
        //printf("-1 g_patd.feed_sum = %d \r\n",g_patd.feed_sum);
        AnaPattern(0, PAT_MIT, 0);
        //printf("-2 g_patd.feed_sum = %d \r\n",g_patd.feed_sum);
        g_feed_flag = tfeed_flag;
        g_point_num = tpoint_num;
        //g_para_offset = tpara_offset;
        //printf("1 g_feed_flag = %d  g_input_method = %d  g_point_num =%d g_para_offset = %d\r\n",g_feed_flag,g_input_method,g_point_num,g_para_offset);
        //printf("1 g_bt_mode = %d  g_s_stitch = %d  b_srt_bt = %d  g_e_stitch = %d  g_ent_bt = %d \r\n",g_bt_mode,g_s_stitch,g_srt_bt,g_e_stitch,g_ent_bt);
        //printf("1 g_zig_zag = %d  g_zig_direction = %d  g_zig_pitch = %d g_zig_width =%d \r\n",g_zig_zag,g_zig_direction,g_zig_pitch,g_zig_width);
        if (!g_feed_flag)
        { //空送
            g_selectmode = 5;
        }
        else if (g_input_method == 1)
        { //点
            g_selectmode = 6;
        }
        else if (g_input_method == 2)
        { //直线
            g_selectmode = 0;
        }
        g_point_num++;
        g_temp_point_num = g_point_num;
        //printf("-0- g_patd.feed_sum = %d  tfeedsum = %d  g_ctAddpins = %d  g_feedc = %d tfeedc =%d\r\n",g_patd.feed_sum,tfeedsum,g_ctAddpins,g_feedc,tfeedc);
        tfeedsum = g_patd.feed_sum;
        Mit_Modify_Data(20);
        //AnaPattern(0,0,0);
        //g_ctAddpins = g_patd.feed_sum-tfeedsum;
        //printf("f = %d g_ct_insert_flag = %d  g_ctAddpins = %d \r\n",f,g_ct_insert_flag,g_ctAddpins);
        g_nolimt = 0;

        mit_ModToCt();
        AnaPattern(0, PAT_MIT, 0);

        g_feed_flag = 1;

        //printf("-- g_patd.feed_sum = %d  tfeedsum = %d  g_ctAddpins = %d  g_feedc = %d tfeedc =%d\r\n",g_patd.feed_sum,tfeedsum,g_ctAddpins,g_feedc,tfeedc);
        g_feedsum = g_patd.feed_sum;
        g_feedc = tfeedc + g_ctAddpins;
        tfeedc = g_feedc;
    }
    else if ((g_input_method == 4) || (g_input_method == 5))
    { //圆 圆弧
        if (g_DoTrim == 1)
        {
            g_feed_sew = 1;
            g_DoTrim = 0;
        }
        if (mode == 1)
        {
            if (g_input_method == 4)
            {
                SREG[g_draw_p] = g_array1[g_para_offset] + 0x1cc;
            }
            else
            {
                SREG[g_draw_p] = g_array1[g_para_offset] + 0x1d0;
            }
            if (g_zig_zag == 1)
                SREG[g_draw_p] += 0x2;
            SREG[g_draw_p + 1] = ((U16)(g_pitch) << 8) + (U16)(g_speed);
            g_array4[0][0] = g_point_num;
            g_array4[g_point_num][0] = SREG[g_draw_p + 2];
            g_array4[g_point_num][1] = SREG[g_draw_p + 3];
            g_draw_p += 4;
            g_point_num++;
        }
        if (g_point_num == 3)
        {
            if (mode == 1)
            {
                SREG[g_draw_p - 4] -= 0x100;
            }
            else if ((mode == 2) && (g_para_offset))
            {
                add = 1;
                for (kk = 0; kk < g_array4[0][1]; kk++)
                {
                    dat = g_array4[add][1];
                    add += dat;
                }
                wdt();
                g_array4[add][0] = g_draw_p - 4;
                if (g_para_offset == 3)
                    g_array4[add][1] = 2;
                else
                    g_array4[add][1] = g_times + 1;
            }

            x0 = (S16)(SREG[g_draw_p - 6]);
            y0 = (S16)(SREG[g_draw_p - 5]);
            x1 = (S16)(SREG[g_draw_p - 2]);
            y1 = (S16)(SREG[g_draw_p - 1]);

            x1 = x0 + x1;
            y1 = y0 + y1;
            occlude = 0; //圆 圆弧的方向,0:顺时针;1:逆时针.
            if (((x0 == 0) && (y0 == 0)) || ((x1 == 0) && (y1 == 0)))
            {
                return (0);
            }
            else if (((x0 == 0) && (x1 == 0)) || ((y0 == 0) && (y1 == 0)))
            {
                return (0);
            }
            else if (x0 == 0)
            {
                if (y0 > 0)
                {
                    if (x1 < 0)
                        occlude = 1;
                }
                else
                {
                    if (x1 > 0)
                        occlude = 1;
                }
            }
            else
            {
                y0 = y0 * x1 / x0;
                if (y0 == y1)
                {
                    return (0);
                }
                if (x0 > 0)
                {
                    if (y0 < y1)
                        occlude = 1;
                }
                else
                {
                    if (y0 > y1)
                        occlude = 1;
                }
            }

            x0 = (S16)(SREG[g_draw_p - 6]);
            y0 = (S16)(SREG[g_draw_p - 5]);
            x1 = (S16)(SREG[g_draw_p - 2]);
            y1 = (S16)(SREG[g_draw_p - 1]);

            x1 = x0 + x1;
            y1 = y0 + y1;

            circle_center(x0, y0, x1, y1, &POSI);
            g_array3[0][0] = -POSI.x;
            g_array3[0][1] = -POSI.y;

            x0 = x0 + g_array3[0][0];
            y0 = y0 + g_array3[0][1];
            x1 = x1 + g_array3[0][0];
            y1 = y1 + g_array3[0][1];

            d = twopoint_dis(g_array3[0][0], g_array3[0][1]);
            if (g_para_offset == 3)
            {
                l = 1;
            }
            else if (g_para_offset > 0)
            {
                l = g_times;
            }
            else
            {
                l = 1;
            }

            g_array3[1][0] = g_temp_oxpos - g_array3[0][0];
            g_array3[1][1] = g_temp_oypos - g_array3[0][1];
            for (ii = 0; ii < l; ii++)
            {
                wdt();
                if (((occlude == 0) && (g_direction == 0)) || ((occlude == 1) && (g_direction == 1)))
                {
                    if (g_para_offset == 3)
                        k1 = d + g_distance;
                    else
                        k1 = d + g_distance * ii;
                }
                else
                {
                    if (g_para_offset == 3)
                        k1 = (S32)(d) - (S32)(g_distance);
                    else
                        k1 = (S32)(d) - (S32)(g_distance * ii);
                }

                g_array2[0][0] = (S32)(g_array3[0][0] * k1 / (S32)(d));
                g_array2[0][1] = (S32)(g_array3[0][1] * k1 / (S32)(d));
                g_array2[1][0] = (S32)(x0 * k1 / (S32)(d));
                g_array2[1][1] = (S32)(y0 * k1 / (S32)(d));
                g_array2[2][0] = (S32)(x1 * k1 / (S32)(d));
                g_array2[2][1] = (S32)(y1 * k1 / (S32)(d));
                //	printf("--g_array2[0][0] = %d g_array2[0][1] = %d g_array2[1][0] = %d g_array2[1][1] = %d g_array2[2][0] = %d g_array2[2][1] = %d\r\n",(S16)(g_array2[0][0]),
                //	(S16)(g_array2[0][1]),(S16)(g_array2[1][0]),(S16)(g_array2[1][1]),(S16)(g_array2[2][0]),(S16)(g_array2[2][1]));
                k1 = g_array3[1][0] - g_oxpos;
                k2 = g_array3[1][1] - g_oypos;
                if (((ii % 2) == 1) && ((g_para_offset == 2) || (g_para_offset == 5)))
                {
                    if (g_input_method == 4)
                    {
                        g_array2[3][0] = g_array2[1][0];
                        g_array2[3][1] = g_array2[1][1];
                        g_array2[1][0] = g_array2[2][0];
                        g_array2[1][1] = g_array2[2][1];
                        g_array2[2][0] = g_array2[3][0];
                        g_array2[2][1] = g_array2[3][1];
                    }
                    else
                    {
                        g_array2[3][0] = g_array2[0][0];
                        g_array2[3][1] = g_array2[0][1];
                        g_array2[0][0] = g_array2[2][0];
                        g_array2[0][1] = g_array2[2][1];
                        g_array2[2][0] = g_array2[3][0];
                        g_array2[2][1] = g_array2[3][1];
                    }
                    if (g_zig_direction == 0)
                        g_zig_direction = 2;
                    else if (g_zig_direction == 2)
                        g_zig_direction = 0;
                }
                k1 += (S16)(g_array2[0][0]);
                k2 += (S16)(g_array2[0][1]);
                pn = 1;
                cutf = 0;
                if ((k1 != 0) || (k2 != 0))
                {
                    if ((g_para_offset == 4) || (g_para_offset == 5))
                    {
                        dat = g_zig_zag;
                        g_zig_zag = 0;
                        pn = Mit_do_line(k1, k2, mode, 0);
                        if (insert)
                        {
                            g_ctAddpins += Mit_do_line(k1, k2, mode, 1);
                        }
                        if (pn == 0)
                        {
                            goto errorf;
                        }
                        g_zig_zag = (U8)(dat);
                    }
                    else
                    {
                        dat = (SREG[2] + 0x40) / 2;
                        dat = dat + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                        tmp_line_sum = g_line_sum;
                        if (g_pat_p > dat)
                        {
                            if ((g_para_offset == 3) && (SREG[g_draw_p - 12] == 0x88))
                            {
                                k1 = k1 + (S16)(SREG[g_draw_p - 10]);
                                k2 = k2 + (S16)(SREG[g_draw_p - 9]);
                                g_oxpos = g_oxpos - (S16)(SREG[g_draw_p - 10]);
                                g_oypos = g_oypos - (S16)(SREG[g_draw_p - 9]);
                                g_pat_p -= 2;
                                g_line_p--;
                                g_line_sum--;
                                while (SREG[g_pat_p - 2] == 0x3)
                                {
                                    g_pat_p -= 2;
                                    g_line_p--;
                                }
                            }
                            else if ((g_para_offset == 3) && (SREG[g_draw_p - 12] == 0x2401) && (SREG[g_draw_p - 16] == 0x88))
                            {
                                k1 = k1 + (S16)(SREG[g_draw_p - 14]);
                                k2 = k2 + (S16)(SREG[g_draw_p - 13]);
                                g_oxpos = g_oxpos - (S16)(SREG[g_draw_p - 14]);
                                g_oypos = g_oypos - (S16)(SREG[g_draw_p - 13]);
                                g_pat_p -= 2;
                                g_line_p--;
                                g_line_sum--;
                                while (SREG[g_pat_p - 2] == 0x3)
                                {
                                    g_pat_p -= 2;
                                    g_line_p--;
                                }
                            }
                            else if ((SREG[g_pat_p - 2] != 0x2) && (SREG[g_pat_p - 2] != 0x6))
                            { //add on 2009.6.8
                                g_line_sum;
                                SREG[g_line_p++] = g_line_sum;
                                SREG[g_pat_p++] = 0x2;
                                SREG[g_pat_p++] = 0;
                                cutf = 1;
                            }
                        }
                        else
                        {
                            dat = 0;
                        }
                        pn = Mit_do_feed(k1, k2, mode);
                        if (insert)
                        {
                            g_ctAddpins += pn;
                        }
                        g_line_sum = tmp_line_sum;
                        if (pn == 0)
                        {
                            goto errorf;
                        }
                    }
                }
                wdt();

                if ((g_para_offset) && (mode == 2))
                {
                    g_array4[++add][0] = pn - 1 + cutf;
                }
                cutf = 0;
                if ((g_bt_mode == 1) && (g_s_stitch > 0))
                {
                    if ((ii == 0) || (g_para_offset == 1) || (g_para_offset == 2))
                    {
                        if ((g_srt_bt == 0) || (g_srt_bt == 2))
                        {
                            dat = (SREG[2] + 0x40) / 2;
                            dat = dat + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4); //2013.3.27
                            if (g_pat_p > dat)
                            {
                                if ((SREG[g_pat_p - 2] != 0x2) && (SREG[g_pat_p - 2] != 0x1b) && (SREG[g_pat_p - 2] != 0x07))
                                { ///////////////////////////////////
                                    if (((SREG[g_pat_p - 2] & 0x00ff) == 0x1) || ((SREG[g_pat_p - 2] & 0x00ff) == 0x21) || ((SREG[g_pat_p - 2] & 0x00ff) == 0x41) || ((SREG[g_pat_p - 2] & 0x00ff) == 0x61))
                                    {
                                        if (g_feed_sew)
                                        {
                                            SREG[g_line_p++] = g_line_sum;
                                            SREG[g_pat_p++] = 0x2;
                                            SREG[g_pat_p++] = 0;
                                            cutf = 1;
                                        }
                                    }
                                }
                            }
                            add3 = g_pat_p;
                            add4 = g_line_p;
                            for (jj = 0; jj < g_s_stitch; jj++)
                            {
                                SREG[g_line_p++] = g_line_sum;
                            }
                            g_pat_p += (g_s_stitch * 2);
                        }
                        dat = g_s_stitch * (g_srt_bt + 1);
                        for (kk = 0; kk < dat; kk++)
                        {
                            SREG[g_line_p++] = g_line_sum + 0x4000;
                        }
                        add1 = g_pat_p;
                        g_pat_p = g_pat_p + dat * 2;
                        add2 = g_pat_p;
                    }
                }
                else if ((g_bt_mode == 2) && (g_r_stitch > 0))
                {
                    add1 = g_pat_p;
                }

                pn = new_do_arc(mode);
                if (insert && pn)
                {
                    g_ctAddpins += pn - 1;
                }
                if (pn == 0)
                {
                    goto errorf;
                }
                else if (mode == 2)
                {
                    if (g_para_offset)
                        g_array4[add][1] = pn - 1;
                }
                if (((ii % 2) == 1) && ((g_para_offset == 2) || (g_para_offset == 5)))
                {
                    if (g_zig_direction == 0)
                        g_zig_direction = 2;
                    else if (g_zig_direction == 2)
                        g_zig_direction = 0;
                }
                if ((ii == 0) || (g_para_offset == 1) || (g_para_offset == 2))
                {
                    if ((g_bt_mode == 1) && (g_s_stitch > 0))
                    {
                        k1 = 0;
                        k2 = 0;
                        if ((g_srt_bt == 0) || (g_srt_bt == 2))
                        {
                            for (kk = 0; kk < g_s_stitch; kk++)
                            {
                                SREG[add3++] = 0x3;
                                dat = SREG[add2 + kk * 2 + 1];
                                SREG[add3++] = dat;
                                x = (U8)(dat);
                                y = (U8)((dat >> 8));
                                if (x >= 0x80)
                                {
                                    x = x - 0x80;
                                    k1 = k1 - (S32)(x);
                                }
                                else
                                {
                                    k1 = k1 + (S32)(x);
                                }
                                if (y >= 0x80)
                                {
                                    y = y - 0x80;
                                    k2 = k2 - (S32)(y);
                                }
                                else
                                {
                                    k2 = k2 + (S32)(y);
                                }
                            }
                            SREG[add3 - 2] = 0x1b;
                            add3 -= (g_s_stitch * 2);
                        }
                        Mit_do_srtbt(add1, add2);

                        dat = (SREG[2] + 0x40) / 2;
                        dat = dat + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                        if ((add3 > dat) && ((g_srt_bt == 0) || (g_srt_bt == 2)))
                        {
                            if (SREG[add3 - 2] == 0x1b)
                            {
                                dat = SREG[add3 - 1];
                                x = (U8)(dat);
                                y = (U8)((dat >> 8));
                                if (x >= 0x80)
                                {
                                    x = x - 0x80;
                                    k1 = k1 - (S32)(x);
                                }
                                else
                                {
                                    k1 = k1 + (S32)(x);
                                }
                                if (y >= 0x80)
                                {
                                    y = y - 0x80;
                                    k2 = k2 - (S32)(y);
                                }
                                else
                                {
                                    k2 = k2 + (S32)(y);
                                }
                                add3 -= 2;
                                add4--;
                                while (SREG[add3 - 2] == 0x3)
                                {
                                    dat = SREG[add3 - 1];
                                    x = (U8)(dat);
                                    y = (U8)((dat >> 8));
                                    if (x >= 0x80)
                                    {
                                        x = x - 0x80;
                                        k1 = k1 - (S32)(x);
                                    }
                                    else
                                    {
                                        k1 = k1 + (S32)(x);
                                    }
                                    if (y >= 0x80)
                                    {
                                        y = y - 0x80;
                                        k2 = k2 - (S32)(y);
                                    }
                                    else
                                    {
                                        k2 = k2 + (S32)(y);
                                    }
                                    add3 -= 2;
                                    add4--;
                                }
                                if (SREG[add3 - 2] == 0x2)
                                    cutf++;
                                g_line_sum = SREG[add4];
                            }
                        }
                        if ((k1 != 0) || (k2 != 0))
                        {
                            dat = (g_pat_p - add1) / 2;
                            g_line_p = add4;
                            g_pat_p = add3;
                            pn = Mit_do_feed(k1, k2, 3);
                            if (insert)
                            {
                                g_ctAddpins += pn;
                            }
                            if (pn == 0)
                            {
                                goto errorf;
                            }
                            else if (mode == 2)
                            {
                                if (g_para_offset)
                                    g_array4[add][0] = pn - 1 + cutf;
                            }

                            add3 = add1;
                            add4 = g_line_p + ((add1 - g_pat_p) / 2);
                            for (kk = 0; kk < dat; kk++)
                            {
                                SREG[g_line_p++] = SREG[add4++];
                                SREG[g_pat_p++] = SREG[add3++];
                                SREG[g_pat_p++] = SREG[add3++];
                            }
                        }
                        dat = (SREG[2] + 0x40) / 2;
                        add1 = dat + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                        add1 = dat + ((add2 - add1) / 2);
                        g_line_sum = SREG[g_line_p - 1] & 0x7fff;
                        if ((g_para_offset) && (mode == 2))
                        {
                            g_array4[add][1] += (g_s_stitch * (g_srt_bt + 1));
                        }
                    }
                }
                if ((g_bt_mode == 2) && (g_r_stitch > 0) && (g_input_method == 4))
                {
                    Mit_do_repeat(add1, g_line_sum);
                    if ((g_para_offset) && (mode == 2))
                    {
                        g_array4[add][1] += (g_r_stitch * (g_repeat + 1));
                    }
                }
                else if ((g_para_offset == 1) || (g_para_offset == 2) || ((ii + 1) == l))
                {
                    if ((g_bt_mode == 1) && (g_e_stitch > 0))
                    {
                        Mit_do_endbt(g_line_sum);
                        if ((g_para_offset) && (mode == 2))
                        {
                            g_array4[add][1] += (g_e_stitch * (g_ent_bt + 1));
                        }
                    }
                }
            }
            wdt();
            if (mode == 1)
            {
                if (g_para_offset > 0)
                {
                    SREG[g_draw_p++] = 0x2001;
                    if (g_direction == 0)
                        dat = 0x10000 - g_distance;
                    else
                        dat = g_distance;
                    SREG[g_draw_p++] = (U16)(dat);
                    if (g_para_offset == 3)
                        SREG[g_draw_p++] = 0;
                    else
                        SREG[g_draw_p++] = g_times;
                    SREG[g_draw_p++] = 0;
                }
                else if (g_zig_zag)
                {
                    SREG[g_draw_p++] = 0x2001;
                    SREG[g_draw_p++] = 0;
                    SREG[g_draw_p++] = 0;
                    SREG[g_draw_p++] = 0;
                }
                if (g_zig_zag)
                {
                    if (g_zig_direction == 0)
                        dat = 0x100;
                    else if (g_zig_direction == 1)
                        dat = 0x200;
                    else if (g_zig_direction == 2)
                        dat = 0;
                    SREG[g_draw_p - 2] += dat;
                    SREG[g_draw_p - 1] = (((U16)(g_zig_pitch)) << 8) + g_zig_width;
                }
                if ((g_bt_mode == 1) && (g_e_stitch > 0))
                {
                    SREG[g_draw_p++] = 0x2501;
                    SREG[g_draw_p++] = ((U16)(g_e_stitch << 8)) + (g_ent_bt + 1) * 2;
                    SREG[g_draw_p++] = 0;
                    SREG[g_draw_p++] = 0;
                }
                else if ((g_bt_mode == 2) && (g_r_stitch > 0))
                {
                    SREG[g_draw_p++] = 0x2601;
                    SREG[g_draw_p++] = ((U16)(g_r_stitch << 8)) + (g_repeat + 1) * 2;
                    SREG[g_draw_p++] = 0;
                    SREG[g_draw_p++] = 0;
                }

                if (g_dopfoot)
                {
                    SREG[g_draw_p++] = 0x2a01;
                    SREG[g_draw_p++] = (U8)g_pf_dis;
                    SREG[g_draw_p++] = 0;
                    SREG[g_draw_p++] = 0;
                    g_dopfoot = 0;
                }
                g_ct_status_flag = 0;
            }
            else if (mode == 2)
            {
                if (g_para_offset > 0)
                    g_array4[0][1]++;
            }

            g_line_sum++;
            g_point_num = 1;
            g_array4[0][0] = 0;
        }
        else if (g_point_num > 3)
        {
            goto errorf;
        }
    }

    g_dxpos = 0;
    g_dypos = 0;
    if ((g_pf_mode == 0) && (g_pf_dis != 0))
        g_pf_dis = 0;

    if ((g_feed_flag) && (g_point_num == 1) && (g_input_method > 0))
    {
        g_para_offset = 0; //多重缝
        g_zig_zag = 0;     //Z字缝
        g_bt_mode = 0;     //倒缝
    }
    g_feed_flag = 1;
    if (mode == 1)
    {
        g_feedc = g_line_p - (SREG[2] + 0x40) / 2;
        g_feedsum = g_feedc;

        if (pno == MENU_CURSOR)
        {
            dis_data((g_feed_flag == 0) ? 0 : 0x5a, 0);
        }
    }

    g_modifyflag |= 0x01;

    if (g_dopfoot == 1)
        g_dopfoot = 0;

    wdt();

    return (1);

errorf:

    if (g_dopfoot == 1)
    {
        g_dopfoot = 0;
        g_pf_mode = 1;
    }
    if (mode == 1)
    {
        hleppara = 2;
        pno = MENU_INHELP;
        poldno[pnoindex] = MENU_CURSOR;
        pnoindex++;
    }
    wdt();
    return (0);
}

//------------------------------------------
// Function   : U32 new_do_arc(U8 mode)
// Description: 生成圆/圆弧数据
//------------------------------------------
U32 new_do_arc(U8 mode)
{
    U32 x, y;
    S32 x0, y0, x1, y1;
    U32 ii;
    U32 d, d1, n, dat, ang;
    double dat0;
    U8 dir = 0, quadrant; //0:顺时针;1:逆时针.
    S32 dat3;
    U16 dat4;
    U32 temp_add1;

    temp_add1 = g_pat_p;

    wdt();
    //****判断圆弧的方向和三点是否在同一直线上****
    x0 = (S16)(g_array2[0][0]);
    y0 = (S16)(g_array2[0][1]);
    x1 = (S16)(g_array2[1][0]);
    y1 = (S16)(g_array2[1][1]);
    x1 = x1 - x0;
    y1 = y1 - y0;
    x = x1;
    y = y1;
    x1 = (S16)(g_array2[2][0]);
    y1 = (S16)(g_array2[2][1]);
    x1 = x1 - x0;
    y1 = y1 - y0;
    x0 = x;
    y0 = y;
    if (((x0 == 0) && (y0 == 0)) || ((x1 == 0) && (y1 == 0)))
    {
        return (0);
    }
    else if (((x0 == 0) && (x1 == 0)) || ((y0 == 0) && (y1 == 0)))
    {
        return (0);
    }
    else if (x0 == 0)
    {
        if (y0 > 0)
        {
            if (x1 < 0)
                dir = 1;
        }
        else
        {
            if (x1 > 0)
                dir = 1;
        }
    }
    else
    {
        y0 = y0 * x1 / x0;
        if (y0 == y1)
            return (0);
        if (x0 > 0)
        {
            if (y0 < y1)
                dir = 1;
        }
        else
        {
            if (y0 > y1)
                dir = 1;
        }
    }
    //****判断圆弧的方向和三点是否在同一直线上****
    //***************计算圆弧的弧长***************
    x0 = (S16)(g_array2[0][0]);
    y0 = (S16)(g_array2[0][1]);
    x1 = (S16)(g_array2[2][0]);
    y1 = (S16)(g_array2[2][1]);

    if (x0 == 0)
    {
        if (y0 > 0)
            ang = 15708;
        else
            ang = 47124;
    }
    else
    {
        dat0 = (double)(y0) / x0;
        dat0 = atan(dat0);
        if (x0 > 0)
        {
            if (dat0 < 0)
                dat0 = dat0 + PI * 2;
        }
        else
        {
            dat0 = dat0 + PI;
        }
        dat0 = dat0 * 10000;
        ang = dat0;
    }
    if (g_input_method == 5)
    {
        if (x1 == 0)
        {
            if (y1 > 0)
                dat0 = 1.5708;
            else
                dat0 = 4.7124;
        }
        else
        {
            dat0 = (double)(y1) / x1;
            dat0 = atan(dat0);
            if (x1 > 0)
            {
                if (dat0 < 0)
                    dat0 = dat0 + PI * 2;
            }
            else
            {
                dat0 = dat0 + PI;
            }
            if (dat0 == 0)
                dat0 = PI * 2;
        }
        x1 = dat0 * 10000;
        x1 -= ang;
        if (x1 < 0)
            x1 = 62832 + x1;
        if (dir == 0)
            x1 = 62832 - x1;
        dat0 = (double)(x1) / 10000;
    }
    else
    {
        dat0 = PI * 2;
    }
    d = twopoint_dis(x0, y0);
    d = dat0 * d;
    //***************计算圆弧的弧长***************
    if (g_zig_zag)
    {
        n = getpoint_sum2(d, g_zig_pitch, 0, 0);
        if ((mode == 1) || (mode == 2))
        {
            d = twopoint_dis(x0, y0);
            d *= 10;
            dat = dat0 * 10000;
            ii = (U16)(g_zig_width)*10;
            if (g_zig_direction == 0)
            {
                if (dir == 0)
                    d1 = d + ii;
                else
                {
                    if (d > ii)
                        d1 = d - ii;
                    else
                        d1 = 0;
                }
            }
            else if (g_zig_direction == 2)
            {
                if (dir == 1)
                    d1 = d + ii;
                else
                {
                    if (d > ii)
                        d1 = d - ii;
                    else
                        d1 = 0;
                }
            }
            else
            {
                d = d + ii / 2;
                if (d > ii)
                    d1 = d - ii;
                else
                    d1 = 0;

                x1 = dat / n;
                x1 = (x1 + 2) / 4;
                if (dir)
                    x1 = (S32)(ang) + x1;
                else
                    x1 = (S32)(ang)-x1;
                x1 = x1 % 62832;
                if (x1 < 0)
                    x1 = x1 + 62832;
                if (x1 >= 47124)
                {
                    dat0 = x1 - 62832;
                    quadrant = 4;
                }
                else if (x1 >= 31416)
                {
                    dat0 = x1 - 31416;
                    quadrant = 3;
                }
                else if (x1 > 15708)
                {
                    dat0 = x1 - 31416;
                    quadrant = 2;
                }
                else
                {
                    dat0 = x1;
                    quadrant = 1;
                }
                if (abs(abs(dat0) - 15708) < 80)
                {
                    x1 = 0;
                    y1 = d / 10;
                    //if(dat0<0) y1 = -y1;
                    if ((quadrant == 3) || (quadrant == 4))
                        y1 = -y1;
                }
                else
                {
                    y = d * d;
                    x1 = dat0;
                    dat0 = dat0 / 10000;
                    dat0 = tan(dat0);
                    dat0 = dat0 * dat0 + 1;
                    x = y / dat0;
                    x1 = sqr(x);
                    if ((quadrant == 2) || (quadrant == 3))
                        x1 = -x1;
                    x = x1 * x1;
                    y1 = sqr(y - x);
                    if ((quadrant == 3) || (quadrant == 4))
                        y1 = -y1;
                    if (x1 >= 0)
                        x1 += 5;
                    else
                        x1 -= 5;
                    if (y1 >= 0)
                        y1 += 5;
                    else
                        y1 -= 5;
                    x1 /= 10;
                    y1 /= 10;
                }
                x0 = x1 - x0;
                y0 = y1 - y0;
                if (x0 < 0)
                {
                    x = (-x0);
                    x += 0x80;
                }
                else
                    x = x0;
                if (y0 < 0)
                {
                    y = (-y0);
                    y += 0x80;
                }
                else
                    y = y0;
                g_oxpos += (S16)(x0);
                g_oypos += (S16)(y0);
                if (((g_oxpos < g_Mac.max_xl - 5) || (g_oxpos > g_Mac.max_xr + 5) || (g_oypos > g_Mac.max_yu + 5) || (g_oypos < g_Mac.max_yd - 5)) && (g_nolimt == 0))
                { /*error_test(111);*/
                    return (0);
                }
                x = (U16)(((y << 8) & 0xff00) + ((x)&0xff));
                SREG[g_line_p++] = g_line_sum;
                SREG[g_pat_p++] = 0x1 + g_speed;
                SREG[g_pat_p++] = (U16)(x);
                x0 = x1;
                y0 = y1;
            }

            n *= 2;
            for (ii = 1; ii < n; ii++)
            {
                x1 = dat * ii / n;
                if (dir)
                    x1 = (S32)(ang) + x1;
                else
                    x1 = (S32)(ang)-x1;
                x1 = x1 % 62832;
                if (x1 < 0)
                    x1 = x1 + 62832;
                if (x1 >= 47124)
                {
                    dat0 = x1 - 62832;
                    quadrant = 4;
                }
                else if (x1 >= 31416)
                {
                    dat0 = x1 - 31416;
                    quadrant = 3;
                }
                else if (x1 > 15708)
                {
                    dat0 = x1 - 31416;
                    quadrant = 2;
                }
                else
                {
                    dat0 = x1;
                    quadrant = 1;
                }
                if (abs(abs(dat0) - 15708) < 100)
                {
                    x1 = 0;
                    if ((ii % 2) == 1)
                        y1 = d1 / 10;
                    else
                        y1 = d / 10;
                    //if(dat0<0) y1 = -y1;
                    if ((quadrant == 3) || (quadrant == 4))
                        y1 = -y1;
                }
                else
                {
                    if ((ii % 2) == 1)
                        y = d1 * d1;
                    else
                        y = d * d;
                    x1 = dat0;
                    dat0 = dat0 / 10000;
                    dat0 = tan(dat0);
                    dat0 = dat0 * dat0 + 1;
                    x = y / dat0;
                    x1 = sqr(x);
                    if ((quadrant == 2) || (quadrant == 3))
                        x1 = -x1;
                    x = x1 * x1;
                    y1 = sqr(y - x);
                    if ((quadrant == 3) || (quadrant == 4))
                        y1 = -y1;
                    if (x1 >= 0)
                        x1 += 5;
                    else
                        x1 -= 5; //2011.1.13
                    if (y1 >= 0)
                        y1 += 5;
                    else
                        y1 -= 5; //2011.1.13
                    x1 /= 10;
                    y1 /= 10;
                }
                x0 = x1 - x0;
                y0 = y1 - y0;
                if (x0 < 0)
                {
                    x = (-x0);
                    x += 0x80;
                }
                else
                    x = x0;
                if (y0 < 0)
                {
                    y = (-y0);
                    y += 0x80;
                }
                else
                    y = y0;
                g_oxpos += (S16)(x0);
                g_oypos += (S16)(y0);
                if (((g_oxpos < g_Mac.max_xl - 5) || (g_oxpos > g_Mac.max_xr + 5) || (g_oypos > g_Mac.max_yu + 5) || (g_oypos < g_Mac.max_yd - 5)) && (g_nolimt == 0))
                { /*error_test(222);*/
                    return (0);
                }
                x = (U16)(((y << 8) & 0xff00) + ((x)&0xff));
                SREG[g_line_p++] = g_line_sum;
                SREG[g_pat_p++] = 0x1 + g_speed;
                SREG[g_pat_p++] = (U16)(x);
                x0 = x1;
                y0 = y1;

                wdt();
            }
            if (g_input_method == 5)
            {
                x1 = (S16)(g_array2[2][0]);
                y1 = (S16)(g_array2[2][1]);
            }
            else
            {
                x1 = (S16)(g_array2[0][0]);
                y1 = (S16)(g_array2[0][1]);
            }
            x0 = x1 - x0;
            y0 = y1 - y0;
            if (x0 < 0)
            {
                x = (-x0);
                x += 0x80;
            }
            else
                x = x0;
            if (y0 < 0)
            {
                y = (-y0);
                y += 0x80;
            }
            else
                y = y0;
            g_oxpos += (S16)(x0);
            g_oypos += (S16)(y0);
            if (((g_oxpos < g_Mac.max_xl - 5) || (g_oxpos > g_Mac.max_xr + 5) || (g_oypos > g_Mac.max_yu + 5) || (g_oypos < g_Mac.max_yd - 5)) && (g_nolimt == 0))
            { /*error_test(333);*/
                return (0);
            }
            dat = (U16)(((y << 8) & 0xff00) + ((x)&0xff));
            SREG[g_line_p++] = g_line_sum;
            SREG[g_pat_p++] = 0x1 + g_speed;
            SREG[g_pat_p++] = (U16)(dat);
        }
        else
        {
            n = n * 2;
        }
        if (g_zig_direction == 1)
            n++;
    }
    else
    {
        n = getpoint_sum2(d, g_pitch, 0, 0);

        //	#if Debug4
        //
        //	printf("g_pitch = %d  d = %d \r\n",g_pitch,d);
        //	#endif
        if ((mode == 1) || (mode == 2))
        {
            d = twopoint_dis(x0, y0);
            d *= 10;

            dat = dat0 * 10000;
            for (ii = 1; ii < n; ii++)
            {
                x1 = dat * ii / n;
                if (dir)
                    x1 = (S32)(ang) + x1;
                else
                    x1 = (S32)(ang)-x1;
                x1 = x1 % 62832;
                if (x1 < 0)
                    x1 = x1 + 62832;
                if (x1 >= 47124)
                {
                    dat0 = x1 - 62832;
                    quadrant = 4;
                }
                else if (x1 >= 31416)
                {
                    dat0 = x1 - 31416;
                    quadrant = 3;
                }
                else if (x1 > 15708)
                {
                    dat0 = x1 - 31416;
                    quadrant = 2;
                }
                else
                {
                    dat0 = x1;
                    quadrant = 1;
                }

                dat3 = x1; //存圆弧角度		2009.7.8

                //printf("abs(abs(dat0)-15708) = %d \r\n",abs(abs(dat0)-15708));
                if (abs(abs(dat0) - 15708) < 10)
                { //2018-8-23 80改为10
                    x1 = 0;
                    y1 = d / 10;
                    if ((quadrant == 3) || (quadrant == 4))
                        y1 = -y1;
                }
                else
                {
                    //printf("-----88---\r\n");
                    y = d * d;
                    x1 = dat0;
                    dat0 = dat0 / 10000;
                    if (((dat3 > 7854) && (dat3 < 23562)) || ((dat3 > 39270) && (dat3 < 54978)))
                    {
                        dat0 = tan(dat0);
                        dat0 = dat0 * dat0 + 1;
                        x = y / dat0;
                        x1 = sqr(x);
                        if ((quadrant == 2) || (quadrant == 3))
                            x1 = -x1;
                        x = x1 * x1;
                        y1 = sqr(y - x);
                        if ((quadrant == 3) || (quadrant == 4))
                            y1 = -y1;
                    }
                    else
                    {
                        dat0 = tan(dat0);
                        dat0 = 1.0 / dat0;
                        dat0 = dat0 * dat0 + 1;
                        x = y / dat0;
                        y1 = sqr(x);
                        if ((quadrant == 3) || (quadrant == 4))
                            y1 = -y1;
                        x = y1 * y1;
                        x1 = sqr(y - x);
                        if ((quadrant == 2) || (quadrant == 3))
                            x1 = -x1;
                    }
                    if (x1 >= 0)
                        x1 += 5;
                    else
                        x1 -= 5;
                    if (y1 >= 0)
                        y1 += 5;
                    else
                        y1 -= 5;
                    x1 /= 10;
                    y1 /= 10;
                }
                //	printf("x1 = %d,y2 = %d,x0 = %d,y0 = %d ",x1,y1,x0,y0);
                x0 = x1 - x0;
                y0 = y1 - y0;
                if (x0 < 0)
                {
                    x = (-x0);
                    x += 0x80;
                }
                else
                    x = x0;
                if (y0 < 0)
                {
                    y = (-y0);
                    y += 0x80;
                }
                else
                    y = y0;
                g_oxpos += (S16)(x0);
                g_oypos += (S16)(y0);
                if (((g_oxpos < g_Mac.max_xl - 5) || (g_oxpos > g_Mac.max_xr + 5) || (g_oypos > g_Mac.max_yu + 5) || (g_oypos < g_Mac.max_yd - 5)) && (g_nolimt == 0))
                {
                    return (0);
                }
                x = (U16)(((y << 8) & 0xff00) + ((x)&0xff));
                //printf("--- x = %d \r\n",x);
                if (x != 0)
                { //2018-6-25 添加 防止生成针距为0的数据
                    SREG[g_line_p++] = g_line_sum;
                    SREG[g_pat_p++] = 0x1 + g_speed;
                    SREG[g_pat_p++] = (U16)(x);
                }

                x0 = x1;
                y0 = y1;

                wdt();
            }
            if (g_input_method == 5)
            {
                x1 = (S16)(g_array2[2][0]);
                y1 = (S16)(g_array2[2][1]);
            }
            else
            {
                x1 = (S16)(g_array2[0][0]);
                y1 = (S16)(g_array2[0][1]);
            }
            x0 = x1 - x0;
            y0 = y1 - y0;
            if (x0 < 0)
            {
                x = (-x0);
                x += 0x80;
            }
            else
                x = x0;
            if (y0 < 0)
            {
                y = (-y0);
                y += 0x80;
            }
            else
                y = y0;
            g_oxpos += (S16)(x0);
            g_oypos += (S16)(y0);
            if (((g_oxpos < g_Mac.max_xl - 5) || (g_oxpos > g_Mac.max_xr + 5) || (g_oypos > g_Mac.max_yu + 5) || (g_oypos < g_Mac.max_yd - 5)) && (g_nolimt == 0))
            { /*error_test(555);*/
                return (0);
            }
            dat = (U16)(((y << 8) & 0xff00) + ((x)&0xff));
            //printf("--- dat = %d \r\n",dat);
            if (dat != 0)
            { //2018-6-25 添加 防止生成针距为0的数据
                SREG[g_line_p++] = g_line_sum;
                SREG[g_pat_p++] = 0x1 + g_speed;
                SREG[g_pat_p++] = (U16)(dat);
            }
        }
    }
    n = n + 1;
    if ((g_pf_mode) && ((g_bt_mode == 2) || (g_s_stitch == 0) || (g_bt_mode == 0)))
    {

        for (ii = 0; ii < n; ii++)
        {
            dat4 = SREG[temp_add1 + 2 * ii] & 0x00ff;
            if ((dat4 == 0x61) || (dat4 == 0x41) || (dat4 == 0x21) || (dat4 == 0x1))
            {
                SREG[temp_add1 + 2 * ii] = dat4 + ((U16)(g_pf_dis) << 8);
                break;
            }
        }
        g_pf_mode = 0;
        g_dopfoot = 1;
    }

    g_feed_sew = 1; // 缝纫标志
    wdt();

    return (n);
}
