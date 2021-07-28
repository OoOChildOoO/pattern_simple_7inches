/*
 * @Author: your name
 * @Date: 2021-05-14 09:27:53
 * @LastEditTime: 2021-07-14 16:32:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\src\menu_modify.c
 */

#include <math.h>
#include <stdlib.h>
#include "menu_modify.h"
#include "main.h"
#include "menu.h"
#include "touch.h"
#include "utility.h"
#include "register.h"
#include "function.h"
#include "uart.h"
#include "color.h"
#include "communication.h"
#include "Bor_patfun.h"
#include "Mit_patfun.h"
#include "viewPattern.h"
#include "Patfun.h"

#define DISABSAXIS
extern PATHEAD g_patd;
extern U16 SheetPage; //�������Sheet��ҳ�룬��0��ʼ
extern MachineInfo g_Mac;
extern volatile U32 g_temp_pno;
extern volatile U32 g_pno_temp;
extern volatile U8 g_modifyflag;
extern volatile S32 g_xabspos, g_yabspos, g_uabspos, g_vabspos;
extern volatile S16 g_dxpos, g_dypos;
extern volatile S16 g_tdxpos, g_tdypos;
extern volatile S16 g_tmpox, g_tmpoy;
extern volatile S16 g_disxpos, g_disypos;
extern volatile S16 g_tmpoxpos, g_tmpoypos;
extern volatile S16 g_temp_oxpos, g_temp_oypos;
extern volatile S16 g_oxpos, g_oypos, g_oxpos_temp, g_oypos_temp; //x,y����ʵ��ֵ
extern volatile U8 g_lineforceValSetNow;                          // ��ǰ������������ֵ
extern volatile U8 g_lineforceValSet;                             // ����������׼ֵ
extern volatile U8 g_jogmode;
extern volatile U8 g_jogonflag;
extern volatile U8 g_jogfunction;
extern volatile U32 g_jognumber;
extern volatile U8 g_jogmethod;
extern volatile U16 g_point_num; //���Ƶ���
extern volatile U8 g_oldspeed;
extern volatile U8 g_speed;
extern volatile S8 g_pf_dis;
extern volatile S16 g_pf_absval;
extern volatile S8 g_pf_moddis;
extern volatile S16 g_pf_absdis;
extern volatile U8 g_updata_pf_valflg;
extern volatile U8 g_bt_mode;       //����
extern volatile U8 g_repeat;        //�ص������
extern volatile U8 g_r_stitch;      //�ص�������
extern volatile U8 g_srt_bt;        //ǰ�˵������
extern volatile U8 g_s_stitch;      //ǰ�˵�������
extern volatile U8 g_ent_bt;        //��˵������
extern volatile U8 g_e_stitch;      //��˵�������
extern volatile U8 g_para_offset;   //���ط� 0-�޶��ط� 1-ͬ����Ͷ��ط� 2-������Ͷ��ط� 3-��� 4-ͬ����Ҷ��ط� 5-������Ҷ��ط�
extern volatile U8 g_direction;     //���ط췽��
extern volatile U8 g_distance;      //���ط����
extern volatile U8 g_times;         //���ط����
extern volatile U8 g_zig_zag;       //Z�ַ�
extern volatile U8 g_zig_width;     //Z����
extern volatile U8 g_zig_pitch;     //Z�����
extern volatile U8 g_zig_direction; //Z�췽��
extern volatile U8 g_feed_flag;
extern volatile U32 g_patxaddr, g_patyaddr; //��¼�ֵܿ����ʼ������λ��
extern volatile U8 g_code_mode;
extern volatile U8 g_BorAddFuns;
extern volatile U8 g_GreatPatType;
extern volatile U32 g_CtPatLen;
extern volatile U32 g_Ctendaddr, g_Ctstaddr;
extern volatile U16 g_draw_p;
extern volatile U16 g_line_p;
extern volatile U32 g_pat_p;
extern volatile U16 g_line_sum;
extern volatile U32 g_feedc;   //��ǰ��
extern volatile U32 g_feedsum; //����
extern volatile U8 g_init_flag;
extern volatile U16 g_temp_draw_p;
extern volatile U16 g_temp_line_p;
extern volatile U32 g_temp_pat_p;
extern volatile U8 g_feed_sew;
extern volatile U8 g_DoTrim;
extern volatile U8 g_home_2flag;                // �ڶ�ԭ���־
extern volatile S16 g_home_2xpos, g_home_2ypos; // �ڶ�ԭ��λ������
extern volatile U8 g_pf_mode;
extern volatile U16 g_Ctsavetimes;
extern volatile U16 g_array4[386][2];
extern volatile U16 g_array5[630][2];
extern volatile S16 g_array10[630][2]; //������������ʱ���������,����ͼ�η�Χ��ʾ
extern const U16 pat_code[];
extern volatile U8 g_IsPfcmd;
extern volatile U8 g_BorDelFuns;
extern volatile U8 g_UpdataBorDelFuns;
extern volatile U8 g_IsLocal;
extern volatile U8 g_errLocal;
extern volatile U8 g_UdZhouFlag;
extern volatile U8 g_modifyf;
extern volatile U32 g_list_oadd;
extern volatile U32 g_list_nadd;
extern volatile U8 g_endtype;
extern OLDPAT1 g_dispat2;
extern volatile S16 g_dxpos1, g_dypos1;
//extern volatile U8  g_isBAT;
extern volatile U16 g_pinAng;
extern volatile U8 g_code_para;
extern volatile U16 g_exType;
extern volatile U16 g_exFeedSp;    //������
extern volatile U16 g_exRestart;   //������
extern volatile U16 g_exThickness; //���Ϻ�
extern volatile U16 g_exClweight;  //ѹ����
extern volatile U8 g_ModToCt;
extern volatile U8 g_ct_insert_flag;
extern volatile U32 g_ct_status_flag;
extern volatile U8 g_modifyflag1;
extern volatile U16 g_curPTtype;
extern SEW_CHECK g_sewcheck;
extern volatile U8 g_zhouDown;

extern volatile U32 g_modaddr[30];
extern volatile U16 g_AddRela; //�����ʽתΪ�����׷��
extern volatile U8 g_DelAll_flag;
extern volatile U16 g_EndbtNum;
extern volatile U16 g_btNumber;
extern volatile U8 g_ModifyMode;
extern volatile U16 g_temp_point_num;
extern volatile U8 g_overflag11; // �ж��޸�ʱ�Ƿ��Ѿ����ڴ洢��Χ�ı�־
extern volatile U32 g_max_feedsum;
extern volatile U32 g_min_feedsum;
extern volatile U8 g_ImgPoint;
extern volatile U8 g_ImgMove;
extern volatile U8 g_ExCode; //��ʾ���ӹ������״̬
extern volatile U8 g_tExCode;
extern volatile U8 g_curExCode;
extern volatile U16 g_curPinAng;     //��ǰ״̬��Ƕ�ֵ
extern volatile U16 g_pointPinAng;   //������Ӧ�Ƕ�ֵ
extern volatile U16 g_curSetPara1;   //��ǰ״̬����һ����
extern volatile U16 g_pointSetPara1; //������Ӧ���ö�����ֵ
extern volatile U16 g_curSetPara2;   //��ǰ״̬���ö�����
extern volatile U16 g_pointSetPara2; //������Ӧ���ö�����ֵ
extern volatile S16 g_maxX;
extern volatile S16 g_minX;
extern volatile S16 g_maxY;
extern volatile S16 g_minY;
extern volatile U16 g_LPoint;   //��λ��
extern volatile U8 g_BorSTflag; //��ǿ���ƽ̨�ƶ��������ԭ��λ��
extern volatile U8 g_ModifyDrawFlag;
extern volatile U16 g_Tsavetimes;
extern volatile U16 g_savetimes; //�������ݴ���

extern volatile U32 g_jog_stpoint;
extern volatile U32 g_jog_enpoint;
extern volatile U32 g_jog_point;
extern volatile U8 g_sta_end_flag;
extern volatile U8 g_num_mov_flag;
extern volatile U16 g_parameter;
extern volatile U8 g_selectmode;
extern volatile U16 g_pitch_parameter; // �����޸Ĺ������
extern volatile U8 g_modify_mode;
extern volatile U8 g_modify_method;
extern volatile U8 g_modify_gohome; //0: ֱ��ʽ���ͻ�ԭ�� 1: ���ʽ���ͻ�ԭ��
extern volatile U8 g_modify_code;
extern volatile U8 g_reorigin; // �Զ�����ԭ������ 0�����Զ�����ԭ�� 1���Զ�����ԭ��
extern volatile S16 g_temp_PfootOffVal;
extern volatile U16 g_stitch_number;

extern OLDPAT1 g_dispat1;
extern OLDPAT1 g_dispat3;

extern volatile U8 g_CodeEn;

volatile S16 g_modinpx;
volatile S16 g_modinpy;
volatile S16 g_modinpoint;
volatile S16 g_modcordx;
volatile S16 g_modcordy;
volatile S16 g_modcordflag;

volatile U8 g_temp_modify_selectmode = 0;

extern volatile U16 g_saveflag;
extern volatile S8 g_pf_Set_Offest;
extern volatile U8 g_needtohome; //��Ҫ�ֶ���ԭ����ʾ��־λ
extern volatile U8 g_modzig;
extern void WriteJogmethod(U8 bl);

#define tmp_parameter g_publicbuf[51]
#define tmp_selectmode g_publicbuf[52]
#define tmp_zigwp g_publicbuf[53]
#define temp_stitch_number g_publicbuf[54]
#define tmp_speed g_publicbuf[55]
#define GoHome PREG[283]
/*
	�������������,��ѯ��ӽ�������Ļ�����λ��
*/
#define P_MINDIS 50
U8 getNearPointPos(s16 inx, S16 iny, S16 *outx, S16 *outy)
{
    U32 i;
    ISEWADD sewInfo = {0};
    ISEWCMD sewIcmd = {0};
    U16 dat;
    U8 xx, yy;
    S16 xpos = 0, ypos = 0;
    U16 tmindis;
    U8 f, res = 0;

    if (F_FILETYPE == PAT_MIT)
    {
#if PATRULES_ORG_OFFSET
        if (SREG[3] == 7) //����-�޸�ԭ��λ��
        {
            xpos = (S16)(SREG[6]);
            ypos = (S16)(SREG[7]);
        }
#endif
        tmindis = P_MINDIS * P_MINDIS;
        for (i = 0; i < g_patd.feed_sum; i++)
        {
            dat = SREG[g_patd.pat_add + i * 2] & 0x00ff;
            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
            {
                dat = SREG[g_patd.pat_add + i * 2 + 1];
                xx = dat & 0xff;
                yy = ((U32)(dat >> 8)) & 0xff;
                if (xx >= 0x80)
                {
                    xx -= 0x80;
                    xpos -= xx;
                }
                else
                {
                    xpos += xx;
                }
                if (yy >= 0x80)
                {
                    yy -= 0x80;
                    ypos -= yy;
                }
                else
                {
                    ypos += yy;
                }
                if ((inx - xpos) * (inx - xpos) + (iny - ypos) * (iny - ypos) < tmindis)
                {
                    *outx = xpos;
                    *outy = ypos;
                    tmindis = (inx - xpos) * (inx - xpos) + (iny - ypos) * (iny - ypos);
                    res = 1;
                }
            }
        }
    }
    else if (F_FILETYPE == PAT_SEW)
    {
        tmindis = 2 * P_MINDIS * 2 * P_MINDIS;
        f = Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);
        if (f == 0)
            return res;

        xpos = g_patd.orgX;
        ypos = g_patd.orgY;

        for (i = 0; i < g_patd.pat_len / 2;)
        {
            f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
            if (f)
            {
                if (sewIcmd.cmd == 0xff00)
                { //����
                    break;
                }
                else if (((sewIcmd.cmd & 0xf000) == 0x2000) || ((sewIcmd.cmd & 0xf000) == 0x6000))
                {
                    xpos += sewIcmd.dx;
                    ypos += sewIcmd.dy;
                    if ((inx - xpos) * (inx - xpos) + (iny - ypos) * (iny - ypos) < tmindis)
                    {
                        *outx = xpos;
                        *outy = ypos;
                        tmindis = (inx - xpos) * (inx - xpos) + (iny - ypos) * (iny - ypos);
                        res = 1;
                    }
                }
                i += sewIcmd.off;
            }
            else
                i++;
        }
    }

    return res;
}
//U8 gPatModflag;   //�����޸ı�־ 1����д  	 0��������
//U8 Stflag ;	 //�ֵܿ�  �綯 ���ƶ���־
//S16 gsetxpos,gsetypos;   //�ֵܿ� �������õ�x��y����

//--���͸��ӹ���ָ�� ��Ƕ�,����һ,���ö�
void sendExCodeCmd(void)
{
    if (g_curPinAng != g_pointPinAng)
    {

        //printf("g_curPinAng = %d  g_pointPinAng = %d \r\n",g_curPinAng,g_pointPinAng);

        Write_Cmd(C_PIN_ANG, g_pointPinAng, 0);
        Read_Status();
        g_curPinAng = g_pointPinAng;
        //Beep_On(200);
    }
    if (g_curSetPara1 != g_pointSetPara1)
    {

        //printf("g_curSetPara1 = %d  g_pointSetPara1 = %d \r\n",g_curSetPara1,g_pointSetPara1);

        Write_Cmd(C_SET_PARA1, g_pointSetPara1, 0);
        Read_Status();
        g_curSetPara1 = g_pointSetPara1;
        //Beep_On(200);
    }
    if (g_curSetPara2 != g_pointSetPara2)
    {

        //printf("g_curSetPara2 = %d  g_pointSetPara2 = %d \r\n",g_curSetPara2,g_pointSetPara2);

        Write_Cmd(C_SET_PARA2, g_pointSetPara2, 0);
        Read_Status();
        g_curSetPara2 = g_pointSetPara2;
        //Beep_On(200);
    }
}

S32 getScale(U16 id)
{
    S32 scale = 0;
    S16 xtmp = 0, ytmp = 0;
    if (g_maxX == g_minX)
        g_maxX++;
    if (g_maxY == g_minY)
        g_maxY++;
    if (id == 0)
    {
        xtmp = IMAGE_X9 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
        ytmp = IMAGE_Y9 * 100 / (g_maxY - g_minY);
    }
    else if (id == 1)
    {
        xtmp = IMAGE_X5 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
        ytmp = IMAGE_Y5 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));
    }
    else if (id == 2)
    {
        xtmp = 47200 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
        ytmp = 28700 / (g_maxY - g_minY);
    }

    if (xtmp > ytmp)
        scale = ytmp * 10;
    else
        scale = xtmp * 10;

    wdt();
    return scale;
}

//-----------------------------------------------------------
#define tmp_modify_mode g_publicbuf[41]
#define tmp_modify_method g_publicbuf[42]
#define tmp_gohome g_publicbuf[43]
#define tmp_modify_code g_publicbuf[44]

#define TLFNADD_MODIFY 0
#define TLFNLEN_MODIFY 57                       //0
#define TLFNADD_MODIFY_BN (TLFNADD_MODIFY + 40) //0

#define KEY_MODIFY_SCRNSVR 1     //1 ����
#define KEY_MODIFY_BACK 2        //2 ����
#define KEY_MODIFY_ENTER 3       //3 ȷ��
#define KEY_MODIFY_SKIPJOG 4     //4 ��������
#define KEY_MODIFY_UNDO 5        //5 ȡ��
#define KEY_MODIFY_REDO 6        //6 ����
#define KEY_MODIFY_SEWMODI 7     //7 ��Ŀ����ģʽ
#define KEY_MODIFY_SPDMODE 8     //8 �ٶ�����ģʽ
#define KEY_MODIFY_CODEMOD 9     //9 ��������ģʽ
#define KEY_MODIFY_MODIFYHOME 10 //10 ��ӿ��ͻ�ԭ��
#define KEY_MODIFY_DIRECTHOME 11 //11 ֱ�ӿ��ͻ�ԭ��

#define KEY_MODIFY_STARTPOSI 12 //12 ���λ���޸�
#define KEY_MODIFY_DELETEPAT 13 //13 ��ɾ��
#define KEY_MODIFY_INCREASEP 14 //14 �����
#define KEY_MODIFY_MOVEPAT 15   //15 ���޸�
#define KEY_MODIFY_SECTIONMV 16 //16 ���ƶ�
#define KEY_MODIFY_SECTIONMD 17 //17 ���޸�
#define KEY_MODIFY_PICTHMD 18   //18 ���
#define KEY_MODIFY_PFOOTMD 19   // 19 ѹ��
#define KEY_MODIFY_TENSIONMD 20 //20 ����
#define KEY_MODIFY_SECTINSR 21  //21 �β���

#define KEY_MODIFY_INCCODE 22 //22 ��Ӵ���
#define KEY_MODIFY_DELCODE 23 //23 ɾ������

#define KEY_MODIFY_MAGETOUCH 24 //24 �����ƶ�

#define KEY_MODIFY_MAX 25

#define RECT_MODIFY_TYPE 1      //1 ����
#define RECT_MODIFY_METHOD 2    //2 ����
#define RECT_MODIFY_HOME 3      //3 ��ԭ�㷽ʽ
#define RECT_MODIFY_XM_TIT 4    //4 X�����-����
#define RECT_MODIFY_XM_STR 5    //5 X�����-����
#define RECT_MODIFY_YM_TIT 6    //6 Y�����-����
#define RECT_MODIFY_YM_STR 7    //7 Y�����-����
#define RECT_MODIFY_XI_TIT 8    //8 X����С-����
#define RECT_MODIFY_XI_STR 9    //9 X����С-����
#define RECT_MODIFY_YI_TIT 10   //10 Y����С-����
#define RECT_MODIFY_YI_STR 11   //11 Y����С-����
#define RECT_MODIFY_PINS_TIT 12 //12 ����-����
#define RECT_MODIFY_PINS_STR 13 //13 ����-����

#define RECT_MODIFY_MAX 14

void Menu_Modify(void)
{

    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ

    /****������ز���end****/

    /****��־λ��ز���begin****/
    U32 i, ii, dat, dat1;
    U8 f;
    //	U8  xx,yy;
    U8 tmpmodifymode = 0xff;
    U8 dis_updata = 1,   //dis_updata:�����޸�
        dis_updata1 = 1, //dis_updata1:��ԭ�㷽ʽ�޸�
        dis_updata2 = 1, //dis_updata2:�����޸�
        dis_updata3 = 1, //dis_updata3:ˢ�»�����ʾ
        dis_updata4 = 1; //dis_updata4:������ʾ������Ϣ
    S8 pfootval;
    S16 tmp_PfootOffVal;
    S16 tmpx = 0, tmpy = 0, dx = 0, dy = 0;
    U8 speed;
    S8 Borpooft, Borpfdis;
    S8 lforceval = 0;
    U16 Borcmd;
    S8 tmpBorpooft;
    U8 tmp_pfoot = (U8)S_PFOOT;
    U32 add1, add2, dat2, kk, n;
    U8 x, y;
    U8 ch;
    U8 pfootf;
    U8 excode;
    U8 movfeed = 0;
    /****��־λ��ز���end***/

    const U8 keynum = {KEY_MODIFY_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 ����
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 ȷ��

        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //4 ��������
        {TYPE_X8, TYPE_Y4, KEY_X1, KEY_Y1},  //5 ȡ��
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},  //6 ����

        {38, 68, KEY_X6, KEY_Y6},                             //7 ��Ŀ����ģʽ
        {38 + (KEY_X6 + SAFE_DIST1), 68, KEY_X6, KEY_Y6},     //8 �ٶ�����ģʽ
        {38 + (KEY_X6 + SAFE_DIST1) * 2, 68, KEY_X6, KEY_Y6}, //9 ��������ģʽ

        {246 + (KEY_X6 + SAFE_DIST1), 68, KEY_X6, KEY_Y6}, //10 ��ӿ��ͻ�ԭ��
        {246, 68, KEY_X6, KEY_Y6},                         //11 ֱ�ӿ��ͻ�ԭ��

        {45, 168, KEY_X6, KEY_Y6},                                                      //12 ���λ���޸�
        {45 + (KEY_X6 + SAFE_DIST10), 168, KEY_X6, KEY_Y6},                             //13 ��ɾ��
        {45 + (KEY_X6 + SAFE_DIST10) * 2, 168, KEY_X6, KEY_Y6},                         //14 �����
        {45 + (KEY_X6 + SAFE_DIST10) * 3, 168, KEY_X6, KEY_Y6},                         //15 ���޸�
        {45, 168 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6},                              //16 ���ƶ�
        {45 + (KEY_X6 + SAFE_DIST10), 168 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6},     //17 ���޸�
        {45 + (KEY_X6 + SAFE_DIST10) * 3, 168 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6}, //18 ���
        {45, 168 + (KEY_Y6 + SAFE_DIST8) * 2, KEY_X6, KEY_Y6},                          //19 ѹ��
        {45 + (KEY_X6 + SAFE_DIST10), 168 + (KEY_Y6 + SAFE_DIST8) * 2, KEY_X6, KEY_Y6}, //20 ����
        {45 + (KEY_X6 + SAFE_DIST10) * 2, 168 + (KEY_Y6 + SAFE_DIST8), KEY_X6, KEY_Y6}, //21 �β���

        {45, 168, KEY_X6, KEY_Y6},                          //22 ��Ӵ���
        {45 + (KEY_X6 + SAFE_DIST10), 168, KEY_X6, KEY_Y6}, //23 ɾ������

        {CONVAS_PX6, CONVAS_PY6, CONVAS_X6, CONVAS_Y6}, //24 �����ƶ�
    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {1}},                          //1 ����
        {{KEY_Silver, KEY_Gray}, {0}, {241, 242, 72, 88}, {2, 0, 0, 0}, {0}}, //2 ����
        {{KEY_Silver, KEY_Gray}, {0}, {71, 87}, {3, 0, 0, 0}, {0}},           //3 ȷ��

        {{KEY_Silver, KEY_Gray}, {0}, {509, 0}, {4, 0, 0, 0}, {0}},                      //4 ��������
        {{KEY_Silver, KEY_Gray}, {0}, {661, 0}, {5, 0, 0, 0}, {TLFNADD_MODIFY_BN + 16}}, //5 ȡ��
        {{KEY_Silver, KEY_Gray}, {0}, {662, 0}, {6, 0, 0, 0}, {TLFNADD_MODIFY_BN + 17}}, //6 ����

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {521, 0, 522}, {7, 0, 0, 0}, {0}},   //7 ��Ŀ����ģʽ
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {369, 371, 370}, {8, 0, 0, 0}, {0}}, //8 �ٶ�����ģʽ
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {372, 374, 373}, {9, 0, 0, 0}, {0}}, //9 ��������ģʽ

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {525, 0, 526}, {11, 0, 0, 0}, {0}}, //10 ��ӿ��ͻ�ԭ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {523, 0, 524}, {10, 0, 0, 0}, {0}}, //11 ֱ�ӿ��ͻ�ԭ��

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {527, 0, 528}, {12, 0, 0, 0}, {TLFNADD_MODIFY_BN + 0}},   //12 ���λ���޸�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {529, 0, 530}, {13, 0, 0, 0}, {TLFNADD_MODIFY_BN + 1}},   //13 ��ɾ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {531, 0, 532}, {14, 0, 0, 0}, {TLFNADD_MODIFY_BN + 2}},   //14 �����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {533, 0, 534}, {15, 0, 0, 0}, {TLFNADD_MODIFY_BN + 3}},   //15 ���޸�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {535, 0, 536}, {16, 0, 0, 0}, {TLFNADD_MODIFY_BN + 4}},   //16 ���ƶ�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {537, 0, 538}, {17, 0, 0, 0}, {TLFNADD_MODIFY_BN + 5}},   //17 ���޸�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {386, 0, 387}, {18, 0, 0, 0}, {TLFNADD_MODIFY_BN + 7}},   //18 ���
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {354, 356, 355}, {19, 0, 0, 0}, {TLFNADD_MODIFY_BN + 8}}, //19 ѹ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {31, 33, 32}, {20, 0, 0, 0}, {TLFNADD_MODIFY_BN + 9}},    //20 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {539, 0, 540}, {24, 0, 0, 0}, {TLFNADD_MODIFY_BN + 6}},   //21 �β���

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {541, 0, 542}, {21, 0, 0, 0}, {TLFNADD_MODIFY_BN + 10}}, //22 ��Ӵ���
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {543, 0, 544}, {22, 0, 0, 0}, {TLFNADD_MODIFY_BN + 11}}, //23 ɾ������

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {CONVAS_COLOR_B, CONVAS_COLOR_T}, {0, 0, 0, 0}, {0}}, //24 �����ƶ�

    };

    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X35, TYPE_Y2, TYPE_SIZE18, TYPE_SIZE30, {Color_Black}, {Color_Black}, {25}},             //1 ����
        {TYPE_X35, 140, KEY_X6 * 4 + SAFE_DIST7 * 8, TYPE_SIZE31, {Color_Black}, {Color_Black}, {26}}, //2 ����
        {238, TYPE_Y2, KEY_X6 * 2 + SAFE_DIST1 * 3, TYPE_SIZE30, {Color_Black}, {Color_Black}, {27}},  //3 ��ԭ�㷽ʽ

        {CONVAS_PX6, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {28}},                                                  //4 X�����-����
        {CONVAS_PX6 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                           //5 X�����-����
        {510, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {30}},                                                         //6 Y�����-����
        {510 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                  //7 Y�����-����
        {CONVAS_PX6, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {29}},                        //8 X����С-����
        {CONVAS_PX6 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}}, //9 X����С-����
        {510, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {31}},                               //10 Y����С-����
        {510 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2 + TYPE_SIZE4 + SAFE_DIST5, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},        //11 Y����С-����
        {630, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {32}},                                                         //12 ����-����
        {630 + TYPE_SIZE8 + SAFE_DIST3, TYPE_Y2, TYPE_SIZE8, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},                                  //13 ����-����

    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MODIFY); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_MODIFY_MAIN);

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    g_ModToCt = 0;
    g_pno_temp = 0; //���޸�ģ��ͼ����־�ݴ���
    if (plastno != MENU_SCRNSVR)
    {
        if (g_modify_mode > 2)
            g_modify_mode = 0;
        if (g_modify_method > 9)
            g_modify_method = 0;
        if (g_modify_gohome > 1)
            g_modify_gohome = 0;
        if (g_modify_code > 1)
            g_modify_code = 0;
        tmp_modify_mode = g_modify_mode;
        tmp_modify_method = g_modify_method;
        tmp_gohome = g_modify_gohome;
        tmp_modify_code = g_modify_code;
    }
    if (g_modcordflag)
    {
        //mit_getPointXY(g_jog_point,52000,&tmpx,&tmpy);
        if (g_modinpoint == g_jog_point)
        {
            if (F_FILETYPE == PAT_MIT)
            {
                mit_getPointXY(g_jog_point, 52000, &tmpx, &tmpy);
                if (g_jog_point == 0 && (tmpx != g_modinpx || tmpy != g_modinpy))
                {
                    g_modinpoint = 0;
                    Send_Pfoot(1, MENU_MODIFY, 1); //ѹ����
                    Comm_Move(tmpx, tmpy, 50, MENU_MODIFY, 0, 0);
                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                }
                else
                {
                    g_modcordx = tmpx - g_modinpx;
                    g_modcordy = tmpy - g_modinpy;
                }
            }
            else if (F_FILETYPE == PAT_SEW)
            {

                if (g_jog_point == 0)
                {
                    Read_xyposi();
                    if ((g_xabspos == 0) && (g_yabspos == 0)) //��еԭ��
                    {
                        tmpx = 0;
                        tmpy = 0;
                    }
                    else if (g_jog_point == 0) //��ԭ��
                    {
                        tmpx = g_patd.orgX;
                        tmpy = g_patd.orgY;
                    }
                    if (tmpx != g_modinpx || tmpy != g_modinpy)
                    {
                        Send_Pfoot(1, MENU_MODIFY, 1); //ѹ����
                        g_modinpoint = 0;
                        Comm_Move(tmpx, tmpy, 50, MENU_MODIFY, 1, 0);
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                    }
                }
            }
        }
        tmpx = 0;
        tmpy = 0;
    }

    pnoindex = tmppnoindex;

    g_modzig = 0;
    pfootf = (U8)(S_PFOOT);
    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                 SheetPage, rect[0].index[0], rect[0].colorT[0]); //�������

    //����������ʾ
    for (i = 1; i < RECT_MODIFY_MAX; i++)
    {

        if (i == RECT_MODIFY_TYPE ||
            i == RECT_MODIFY_METHOD ||
            i == RECT_MODIFY_HOME)
        {
            DrawRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);

            Draw24x24Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + SAFE_DIST3,
                         MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
                         SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
        }
        // else if (i == RECT_MODIFY_XM_TIT || i == RECT_MODIFY_YM_TIT || i == RECT_MODIFY_XI_TIT ||
        //          i == RECT_MODIFY_YI_TIT || i == RECT_MODIFY_PINS_TIT)
        // {
        //     DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
        //     Draw16x16Str(rect[i].xpis + (rect[i].xsize - 8 * strlen((char *)(MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax))) / 2,
        //                  rect[i].ypis + (rect[i].ysize - 16) / 2,
        //                  MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
        //                  SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
        // }
        // else
        // {
        //     DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
        // }
    }
    // DrawFS32P(rect[RECT_MODIFY_XM_STR].xpis + SAFE_DIST4,
    //           rect[RECT_MODIFY_XM_STR].ypis + SAFE_DIST4,
    //           g_patd.patmX, 5, 1, rect[RECT_MODIFY_XM_STR].colorT[0]);
    // DrawFS32P(rect[RECT_MODIFY_XI_STR].xpis + SAFE_DIST4,
    //           rect[RECT_MODIFY_XI_STR].ypis + SAFE_DIST4,
    //           g_patd.patiX, 5, 1, rect[RECT_MODIFY_XI_STR].colorT[0]);
    // DrawFS32P(rect[RECT_MODIFY_YM_STR].xpis + SAFE_DIST4,
    //           rect[RECT_MODIFY_YM_STR].ypis + SAFE_DIST4,
    //           g_patd.patmY, 5, 1, rect[RECT_MODIFY_YM_STR].colorT[0]);
    // DrawFS32P(rect[RECT_MODIFY_YI_STR].xpis + SAFE_DIST4,
    //           rect[RECT_MODIFY_YI_STR].ypis + SAFE_DIST4,
    //           g_patd.patiY, 5, 1, rect[RECT_MODIFY_YI_STR].colorT[0]);

    // DrawFU32(rect[RECT_MODIFY_PINS_STR].xpis + SAFE_DIST4,
    //          rect[RECT_MODIFY_PINS_STR].ypis + (rect[RECT_MODIFY_PINS_STR].ysize - 16) / 2,
    //          g_patd.feed_sum, 5, rect[RECT_MODIFY_PINS_STR].colorT[0]);

    DrawFillRect(keyd[KEY_MODIFY_MAGETOUCH].xpis, keyd[KEY_MODIFY_MAGETOUCH].ypis,
                 keyd[KEY_MODIFY_MAGETOUCH].xsize, keyd[KEY_MODIFY_MAGETOUCH].ysize, keybmpind[KEY_MODIFY_MAGETOUCH].icon[0]);
    //����������ʾ
    for (i = 2; i <= KEY_MODIFY_REDO; i++)
    {
        if ((i == KEY_MODIFY_UNDO) && (g_savetimes == 0)) //��Ч����
        {
        }
        else if ((i == KEY_MODIFY_REDO) && (g_savetimes >= g_Tsavetimes))
        {
        }
        else if (i == KEY_MODIFY_BACK)
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0],
                       (tmppnoindex != 1) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
    ViewPage(usepageno); //������ʾ����

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        keydata = get_keydata((KEYPAD *)keyd, keynum, 0); //��ȡ��ֵ
        if (tmp_modify_mode == 0)
        {
            if (keydata >= KEY_MODIFY_INCCODE && keydata <= KEY_MODIFY_DELCODE)
            {
                keydata = 0;
            }
        }
        else if (tmp_modify_mode == 1)
        {
            if (keydata >= KEY_MODIFY_STARTPOSI && keydata <= KEY_MODIFY_SECTINSR)
            {
                keydata = 0;
            }
        }
        else if (tmp_modify_mode == 2)
        {
            if (keydata >= KEY_MODIFY_STARTPOSI && keydata <= KEY_MODIFY_DELETEPAT)
            {
                keydata += (KEY_MODIFY_INCCODE - KEY_MODIFY_STARTPOSI);
            }
            else if (keydata >= KEY_MODIFY_INCREASEP && keydata <= KEY_MODIFY_SECTINSR)
            {
                keydata = 0;
            }
        }

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if ((keydata == KEY_MODIFY_UNDO) && (g_savetimes == 0))
            {
                keydata = 0;
            }
            else if ((keydata == KEY_MODIFY_REDO) &&
                     ((F_FILETYPE == PAT_MIT) ? g_savetimes >= g_Tsavetimes : g_savetimes >= g_Tsavetimes))
            {
                keydata = 0;
            }
            else if ((keydata == KEY_MODIFY_PFOOTMD) && (PFOOTBT == 0))
            {
                keydata = 0;
            }
            else if ((keydata == KEY_MODIFY_TENSIONMD) && (TENSIONBT == 0))
            {
                keydata = 0;
            }
            else if (keydata == KEY_MODIFY_MAGETOUCH)
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
                {
                    keyon = keydata;
                    if (keyon == KEY_MODIFY_SCRNSVR) //�ް�����Ч
                    {
                    }
                    //�ǳ��水��-//������ɫ���ְ���
                    else
                    {
                        DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                    }
                    Times = 0;
                    delay = KEYDT; //������������
                }
                else if (keyon != keydata) //���а������£��������°�����Ϣ
                {
                    keydata = 0;
                }

                if (Times > delay) //��������
                {
                    if (beeponf == 0) //��������־λ //��һ�ν���
                    {
                        beeponf = 1;

                        if (g_keyBellT) //����ʱ����������ʱ��
                            Beep_On(g_keyBellT);

                        switch (keyon) //�������±�����ʾ����ʾ
                        {
                        default:
                            DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                            break;
                        }
                        delay = KEYFT;
                    }
                    else if (beeponf == 1) //�ڶ��ν���
                    {
                        beeponf = 2;
                        delay = KEYNT;
                    }
                    else if (beeponf < 20)
                    {
                        beeponf++;
                    }
                    Times = 0; //��ǰ
                }
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            if ((keyoff >= KEY_MODIFY_SEWMODI) && (keyoff <= KEY_MODIFY_CODEMOD))
            { //7 -  9
                tmp_modify_mode = keyoff - KEY_MODIFY_SEWMODI;
                dis_updata = 1;
                dis_updata2 = 1;
            }
            else if ((keyoff >= KEY_MODIFY_MODIFYHOME) && (keyoff <= KEY_MODIFY_DIRECTHOME))
            { //10 - 11
                tmp_gohome = keyoff - KEY_MODIFY_MODIFYHOME;
                dis_updata1 = 1;
            }
            else if ((keyoff >= KEY_MODIFY_STARTPOSI) && (keyoff <= KEY_MODIFY_SECTINSR))
            { //12 - 20
                tmp_modify_method = keyoff - KEY_MODIFY_STARTPOSI;
                dis_updata2 = 1;
            }
            else if ((keyoff >= KEY_MODIFY_INCCODE) && (keyoff <= KEY_MODIFY_DELCODE))
            { // 21 - 22
                tmp_modify_code = keyoff - KEY_MODIFY_INCCODE;
                dis_updata2 = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_MODIFY_SCRNSVR: //1
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_MODIFY_BACK:     //2
                    if (tmppnoindex == 1) // �����λ������
                    {
                        pno = MENU_INHELP;
                        hleppara = 0;
                        poldno[pnoindex] = MENU_MODIFY;
                        pnoindex++;
                        if (F_FILETYPE == PAT_SEW)
                        {
                            if (g_savetimes == 0 && g_modifyflag1 == 0)
                                g_modifyflag = 0;
                            //								if(g_modifyflag !=0)  gPatModflag = 1;	   //��дԭ������
                        }
                        else
                        {
                            if (g_savetimes == 0 && g_modifyflag1 == 0)
                                g_modifyflag = 0;
                            //								 if(modifyflag !=0)  gPatModflag = 1;	   //��дԭ������
                        }
                        g_endtype = 2;
                        g_modify_method = 0;
                    }
                    else // ��滭�����
                    {

                        if (g_GreatPatType == 1)
                        { //����
                            //�ϲ�Ϊ��������
                            add1 = SREG[g_draw_p - 4];
                            add2 = SREG[g_pat_p - 2];
                            if ((add1 != 0x1f01) && (add1 != 0x1e01) && (add2 != 0x1f) && (add2 != 0x1e))
                            {
                                SREG[g_draw_p++] = 0x1f01;
                                SREG[g_draw_p++] = 0;
                                SREG[g_draw_p++] = 0;
                                SREG[g_draw_p++] = 0;
                                SREG[g_line_p++] = g_line_sum++;
                                SREG[g_pat_p++] = 0x1f;
                                SREG[g_pat_p++] = 0;
                                g_feedc++;
                                g_feedsum++;
                            }
                            add1 = (SREG[2] + 0x40) / 2;
                            add2 = add1 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                            dat2 = g_line_p - add1;
                            g_line_p = g_draw_p;
                            for (ii = 0; ii < dat2; ii++)
                            {
                                SREG[g_line_p++] = SREG[add1 + ii];
                                kk++;
                            }
                            g_pat_p = g_line_p;
                            for (ii = 0; ii < dat2; ii++)
                            {
                                SREG[g_pat_p++] = SREG[add2++];
                                SREG[g_pat_p++] = SREG[add2++];
                            }
                            // ****************************************
                            for (add2 = g_pat_p; add2 < MAXSREG; add2++)
                            {
                                SREG[add2] = 0;
                            }
                            SREG[2] = g_draw_p * 2 - 0x40;
                            SREG[4] = (U16)(dat2 * 4);
                            SREG[5] = (U16)((dat2 * 4) >> 16);

                            ///////////////////////////////////////////������Ӽ�������׼�߶�
                            f = Read_D(D_Address[SYS] + SYS_LFORCESET, 1, (U16 *)(&YREG[SYS_LFORCESET]));
                            //�����Ӽ�������׼�߶�
                            if (f == 0)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                                poldno[pnoindex] = MENU_CURSOR;
                                pnoindex++;
                                break;
                            }
                            SREG[0x0a] = YREG[SYS_LFORCESET];
                            ////////////////////////////////////

                            f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
                            //���趨��ѹ��������
                            if (f == 0)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                                poldno[pnoindex] = MENU_MODIFY;
                                pnoindex++;
                                break;
                            }
                            SREG[0x19] = YREG[SYS_PFOOTSET] << 8;

                            g_init_flag = 1;

                            //�ѻ������ݲ��Ϊ�޸Ĵ���ʽ
                            for (i = 0; i < MAXBLKBUF; i++)
                            {
                                BLKBUF[i] = 0;
                            }
                            ii = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                            g_temp_line_p = (SREG[2] + 0x40) / 2;
                            g_temp_pat_p = g_temp_line_p + ii;
                            if ((g_temp_line_p <= 32000) && (g_temp_pat_p <= 52000) && (ii < 20000))
                            {

                                g_init_flag = 3;
                                g_draw_p = g_temp_line_p;
                                g_line_p = 32000 + ii;
                                g_pat_p = 52000 + ii * 2;

                                n = g_temp_pat_p + ii * 2;
                                g_dxpos = 0;
                                g_dypos = 0;
                                g_oxpos = 0;
                                g_oypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_oxpos = (S16)(SREG[6]);
                                    g_oypos = (S16)(SREG[7]);
                                }
#endif
                                g_feed_sew = 0xff;
                                g_tExCode = 0;
                                for (; ii > 0; ii--)
                                {
                                    SREG[--g_pat_p] = SREG[--n];
                                    dat = SREG[n];
                                    SREG[--g_pat_p] = SREG[--n];
                                    if (((SREG[n] & 0x00ff) == 0x61) || ((SREG[n] & 0x00ff) == 0x41) || ((SREG[n] & 0x00ff) == 0x21) || ((SREG[n] & 0x00ff) == 0x1) || ((SREG[n] & 0x00ff) == 0x1b) || ((SREG[n] & 0x00ff) == 0x3))
                                    {
                                        if (g_feed_sew == 0xff)
                                        {
                                            if ((SREG[n] == 0x3) || (SREG[n] == 0x1b))
                                                g_feed_sew = 0;
                                            else
                                                g_feed_sew = 1;
                                        }
                                        ch = (U8)(dat);
                                        if (ch >= 0x80)
                                        {
                                            ch = ch - 0x80;
                                            g_oxpos = g_oxpos - (S16)(ch);
                                        }
                                        else
                                        {
                                            g_oxpos = g_oxpos + (S16)(ch);
                                        }
                                        ch = (U8)((U16)(dat) >> 8);
                                        if (ch >= 0x80)
                                        {
                                            ch = ch - 0x80;
                                            g_oypos = g_oypos - (S16)(ch);
                                        }
                                        else
                                        {
                                            g_oypos = g_oypos + (S16)(ch);
                                        }
                                    }
                                    else if ((g_feed_sew == 0xff) && (SREG[n] == 0x2))
                                    {
                                        g_feed_sew = 0;
                                    }
                                    else if (SREG[n] == 0x05)
                                    { //����1
                                        g_tExCode ^= 0x01;
                                    }
                                    else if (SREG[n] == 0x0b)
                                    { //����2
                                        g_tExCode ^= 0x02;
                                    }
                                    else if (SREG[n] == 0x0c)
                                    { //����3
                                        g_tExCode ^= 0x04;
                                    }
                                    else if (SREG[n] == 0x0d)
                                    { //����4
                                        g_tExCode ^= 0x08;
                                    }
                                    else if (SREG[n] == 0x001d)
                                    { //����5
                                        g_tExCode ^= 0x10;
                                    }
                                    else if (SREG[n] == 0x011d)
                                    { //����6
                                        g_tExCode ^= 0x20;
                                    }
                                    else if (SREG[n] == 0x021d)
                                    { //����7
                                        g_tExCode ^= 0x40;
                                    }
                                    else if (SREG[n] == 0x031d)
                                    { //����8
                                        g_tExCode ^= 0x80;
                                    }
                                }
                                if (g_feed_sew == 0xff)
                                    g_feed_sew = 0;

                                if (tmp_pfoot != 0)
                                {                                  //ѹ��������
                                    Send_Pfoot(1, MENU_MODIFY, 1); //ѹ����
                                }
                                if (P_FHP == 0) //ֱ�ӻص�����������
                                {
                                    Comm_Move(g_oxpos, g_oypos, 120, MENU_MODIFY, 0, 0);
                                }

                                excode = g_ExCode ^ g_tExCode;
                                if (excode != 0)
                                {
                                    OutputExcode(excode, MENU_MODIFY);
                                }

                                ii = ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                                for (; ii > 0; ii--)
                                {
                                    SREG[--g_line_p] = SREG[--n];
                                }
                                for (ii = g_draw_p; ii < 32000; ii++)
                                {
                                    SREG[ii] = 0;
                                }

                                g_line_p = 32000 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 4);
                                g_pat_p = 52000 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 2);
                                SREG[2] = 64000 - 0x40;
                                SREG[4] = 0x3880;
                                SREG[5] = 1;
                                ///////////////////////////////
                                g_temp_line_p = SREG[g_draw_p - 4];
                                g_temp_pat_p = SREG[g_pat_p - 2];
                                if (((g_temp_line_p == 0x1f01) && (g_temp_pat_p == 0x1f)) || ((g_temp_line_p == 0x1e01) && (g_temp_pat_p == 0x1e)))
                                {
                                    g_draw_p -= 4;
                                    g_line_p -= 1;
                                    g_pat_p -= 2;
                                    SREG[g_draw_p] = 0;

                                    ///////////////////////////////////////////////ADD ON 1.16

                                    g_temp_line_p = SREG[g_draw_p - 4];
                                    g_temp_pat_p = SREG[g_pat_p - 2];
                                    if ((g_temp_line_p == 0x0201) && (g_temp_pat_p == 0x02))
                                    {
                                        g_draw_p -= 4;
                                        g_line_p -= 1;
                                        g_pat_p -= 2;
                                        SREG[g_draw_p] = 0;
                                        g_DoTrim = 1; //ɾ�����߱�־
                                    }
                                }
                                g_line_sum = (U16)((SREG[g_line_p - 1]) & 0x3fff) + 1;
                                g_feedc = g_line_p - (SREG[2] + 0x40) / 2;
                                g_feedsum = g_feedc;
                                for (ii = 0x24; ii < g_draw_p; ii += 4)
                                {
                                    if (SREG[ii] == 0x601)
                                    {
                                        g_home_2flag = 1;
                                        g_home_2xpos = 0;
                                        g_home_2ypos = 0;
                                        for (; ii > 0x24; ii -= 4)
                                        {
                                            g_home_2xpos += (S16)(SREG[ii - 2]);
                                            g_home_2ypos += (S16)(SREG[ii - 1]);
                                        }
                                        break;
                                    }
                                }
                            }

                            g_patd.draw_add = 0x24;
                            g_patd.line_add = (SREG[2] + 0x40) / 2;
                            g_patd.pat_add = (SREG[2] + 0x40 + ((((U32)(SREG[5]) << 16) + SREG[4]) / 2)) / 2;

                            g_lineforceValSet = g_tdtensiond.base;
                            lforceval = Mit_CailPadOffestLF(g_feedsum, 0);
                            g_lineforceValSetNow = g_lineforceValSet + lforceval;

                            wdt();
                            g_pf_absval = g_pfootd.base;
                            for (i = 52000; i < g_pat_p; i += 2)
                            {
                                dat = SREG[i] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                {
                                    if ((SREG[i] & 0xff00) != 0)
                                    {
                                        g_pf_absval += (S8)(SREG[i] >> 8);
                                    }
                                }
                            }

                            if (P_FHP)
                            { //ѭ�����켣�ص�������

                                while (g_jog_point < g_feedsum)
                                {
                                    dat1 = 52000 + g_jog_point * 2;
                                    dat = SREG[dat1] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        if ((dat == 0x1b) || (dat == 0x3))
                                            movfeed = 1;
                                        dat = SREG[dat1 + 1];

                                        x = dat & 0x000000ff;
                                        y = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (x >= 0x80)
                                        {
                                            x -= 0x80;
                                            dx = -x;
                                        }
                                        else
                                        {
                                            dx = x;
                                        }
                                        if (y >= 0x80)
                                        {
                                            y -= 0x80;
                                            dy = -y;
                                        }
                                        else
                                        {
                                            dy = y;
                                        }
                                        //	printf("x = %d y = %d dx = %d dy = %d \r\n",x,y,dx,dy);
                                        f = Comm_RelationMove(dx, dy, MENU_MODIFY, 0, movfeed);
                                        movfeed = 0;
                                        if (f == 0)
                                        {
                                            break;
                                        }
                                    }
                                    g_jog_point++;
                                }
                            }

                            g_pointPinAng = mit_getExCodeData(52000, 52000 + g_feedsum * 2, 0);
                            g_pointSetPara1 = mit_getExCodeData(52000, 52000 + g_feedsum * 2, 1);
                            g_pointSetPara2 = mit_getExCodeData(52000, 52000 + g_feedsum * 2, 2);

                            sendExCodeCmd();
                        }
                        else if (g_GreatPatType == 2)
                        { //�ֵ�
                            g_CtPatLen = F_FILELEN;
                            if (tmp_pfoot != 0)
                            {                                  //ѹ��������
                                Send_Pfoot(1, MENU_MODIFY, 1); //ѹ����
                            }
                            if (P_FHP)
                            {
                                if (g_jog_point == 0)
                                { //���ƶ�����ԭ��λ��
                                    Comm_Move(g_patd.orgX, g_patd.orgY, 50, MENU_MODIFY, 1, 0);
                                }
                                //Bor_feedInfo(g_jog_point+1,g_CtPatLen,&Borcmd,(S16*)&g_oxpos,(S16*)&g_oypos,(S16*)&g_dxpos,(S16*)&g_dypos,(U8*)&g_oldspeed,&tmpBorpooft,&Borpfdis);
                                while (g_jog_point < g_patd.feed_sum)
                                {
                                    Bor_feedInfo(g_jog_point + 1, g_CtPatLen, &Borcmd, (S16 *)&g_oxpos, (S16 *)&g_oypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &tmpBorpooft, &Borpfdis);
                                    if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                                    {                    //����,����
                                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                        if ((Borcmd & 0xf000) == 0x2000)
                                        {
                                            movfeed = 1;
                                        }
                                    }
                                    else if ((Borcmd & 0xf000) == 0x6000)
                                    { //����
                                    }
                                    else if ((Borcmd & 0xff00) == 0xf000)
                                    { //��ѹ�ŵ���
                                    }
                                    else if ((Borcmd & 0xff00) == 0xfc00)
                                    { //���ӹ���
                                        excode = Borcmd & 0xff;
                                        if (excode != 0)
                                        {
                                            f = OutputExcode1(excode, MENU_MODIFY, 0);
                                            if (f == 0)
                                            {
                                                break;
                                            }
                                        }
                                    }
                                    f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFY, 1, movfeed);
                                    movfeed = 0;
                                    if (f == 0)
                                    {
                                        break;
                                    }
                                    g_jog_point++;
                                }
                                if (g_jog_point < g_patd.feed_sum)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    poldno[pnoindex] = MENU_MODIFY;
                                    pnoindex++;
                                    break;
                                }
                                g_oxpos += g_dxpos;
                                g_oypos += g_dypos;
                                g_disxpos = g_oxpos;
                                g_disypos = g_oypos;
                                g_dxpos = 0;
                                g_dypos = 0;
                            }
                            else
                            {
                                Bor_feedInfo(g_patd.feed_sum, g_CtPatLen, &Borcmd, (S16 *)&g_oxpos, (S16 *)&g_oypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &tmpBorpooft, &Borpfdis); //���ҵ�ǰԭ��������������

                                f = Comm_Move(g_oxpos + g_dxpos, g_oypos + g_dypos, 120, MENU_MODIFY, 1, 0); //�ƶ�����������������
                                if (f == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    poldno[pnoindex] = MENU_MODIFY;
                                    pnoindex++;
                                    break;
                                }
                                g_oxpos += g_dxpos;
                                g_oypos += g_dypos;
                                g_disxpos = g_oxpos;
                                g_disypos = g_oypos;
                                g_dxpos = 0;
                                g_dypos = 0;

                                excode = g_ExCode ^ g_tExCode;
                                if (excode != 0)
                                {
                                    f = OutputExcode(excode, MENU_MODIFY);
                                    if (f == 0)
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 15;
                                        poldno[pnoindex] = MENU_MODIFY;
                                        pnoindex++;
                                        break;
                                    }
                                }
                                sendExCodeCmd();
                            }

                            Bor_CtBefDatAny((U32 *)&g_Ctstaddr, (U32 *)&g_CtPatLen, (U8 *)&g_oldspeed, 1);

                            g_point_num = 1;
                            g_array4[0][0] = 0;
                            g_Ctendaddr = g_Ctstaddr;
                            g_feedsum = g_patd.feed_sum;
                            g_feedc = g_patd.feed_sum;
                            g_pf_mode = 0;
                            g_Ctsavetimes = 0;
                            for (i = 0; i < MAXBLKBUF; i++)
                            {
                                BLKBUF[i] = 0;
                            }
                            g_lineforceValSetNow = Bor_CreatCalLF(g_Ctendaddr, g_CtPatLen); //�������������ֵ
                            Bor_CreatBack((U16 *)&g_Ctsavetimes, g_Ctstaddr, g_Ctendaddr, g_oxpos, g_oypos, g_oldspeed, g_pf_absval - g_pfootd.base, g_pf_dis, g_lineforceValSetNow, g_patd.orgX, g_patd.orgY);

                            Bor_CalPFval((U16 *)SREG, g_CtPatLen, g_Ctendaddr, (S16 *)&g_pf_absval);
                            g_pf_absval += g_pfootd.base;
                        }

                        Write_Cmd(C_PFOOTACTION, g_pf_absval, 0); //����ѹ�Ÿ߶�����Ϊ׷��һ��ĸ߶�
                        Read_Status();

                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����

                        g_maxX = g_patd.patmX;
                        g_minX = g_patd.patiX;
                        g_maxY = g_patd.patmY;
                        g_minY = g_patd.patiY;
                        //							g_isBAT = 0;	//��������־
                        pnoindex--;
                        pno = poldno[pnoindex];
                    }

                    break;
                case KEY_MODIFY_ENTER: //3
                    g_modify_mode = tmp_modify_mode;
                    g_modify_method = tmp_modify_method;
                    g_temp_modify_selectmode = g_modify_method;
                    g_modify_gohome = tmp_gohome;

                    if (g_modify_mode == 0)
                    { //��Ŀ����
                        g_jog_stpoint = 0;
                        g_jog_enpoint = 0;
                        g_para1 = g_modify_method;
                        g_sta_end_flag = 0; // modify_method=4 5 ʱ��
                        g_num_mov_flag = 0;
                        if (g_modify_method > 3)
                            g_parameter = 30;
                        if ((g_modify_method == 0) || (g_modify_method == 5))
                        {
                            g_dxpos = 0;
                            g_dypos = 0;
                            if (g_modify_method == 5)
                                g_num_mov_flag = 2;
                            pno = MENU_MODIFY1;
                            poldno[pnoindex] = MENU_MODIFY;
                            pnoindex++;
                        }
                        else
                        {
                            if (g_modify_method == 4)
                                g_num_mov_flag = 2;
                            pno = MENU_MODIFYADD;
                            poldno[pnoindex] = MENU_MODIFY;
                            pnoindex++;
                        }
                    }
                    else if (g_modify_mode == 1)
                    { //�ٶ�ָ������
                        g_para1 = 15;
                        pno = MENU_MODIFYADD;
                        poldno[pnoindex] = MENU_MODIFY;
                        pnoindex++;
                    }
                    else
                    { //������������
                        g_modify_code = tmp_modify_code;
                        if (g_modify_code == 0)
                            g_para1 = 20;
                        else
                            g_para1 = 21;
                        pno = MENU_MODIFYADD;
                        poldno[pnoindex] = MENU_MODIFY;
                        pnoindex++;
                    }
                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����

                    g_maxX = g_patd.patmX;
                    g_minX = g_patd.patiX;
                    g_maxY = g_patd.patmY;
                    g_minY = g_patd.patiY;
                    break;

                case KEY_MODIFY_SKIPJOG: //4
                    pno = MENU_SKIPJOG;
                    poldno[pnoindex] = MENU_MODIFY;
                    pnoindex++;
                    break;
                case KEY_MODIFY_UNDO: //5
                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        g_savetimes--;
                        g_list_nadd = Mit_ReadSregData(g_list_nadd);

                        g_reorigin = 0;
                        //RecovrSREG(g_savetimes,0);
                        Mit_CreateAllData();

                        // DrawFillRect(keyd[KEY_MODIFY_MAGETOUCH].xpis, keyd[KEY_MODIFY_MAGETOUCH].ypis, keyd[KEY_MODIFY_MAGETOUCH].xsize, keyd[KEY_MODIFY_MAGETOUCH].ysize, mCOLOR ? Color_Silver : Color_White);

                        dis_updata3 = 1;
                        dis_updata4 = 1;
                        AnaPattern(0, PAT_MIT, 0);

                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        //							g_tmpoxpos = 0;g_tmpoypos = 0;
                        //							for(ii=0;ii<g_jog_point;ii++){
                        //								dat = SREG[g_patd.pat_add+ii*2+1];
                        //								xx = dat&0x000000ff;
                        //								yy = ((U32)(dat>>8))&0x000000ff;
                        //								if(xx>=0x80){xx-=0x80;g_tmpoxpos-=xx;}else{g_tmpoxpos+=xx;}
                        //								if(yy>=0x80){yy-=0x80;g_tmpoypos-=yy;}else{g_tmpoypos+=yy;}
                        //							}
                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                        f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET])); //���趨��ѹ��������
                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            return;
                        }
                        tmp_PfootOffVal = YREG[SYS_PFOOTSET];
                        pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //����jog_point֮ǰ����ѹ�Ÿ߶ȱ仯��
                        tmp_PfootOffVal += pfootval;

                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                        if (S_PFOOT)
                        {                                //��ѹ��̧��
                            DREG[D_COMMAND] = C_PFOOTUP; //ѹ����
                            DREG[D_CMDSTATUS] = 0;
                            DREG[D_CMDPARA1] = 0;
                            f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND]));
                            if (f == 0)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                                break;
                            }
                            Read_Status();
                            //for(ii=0;ii<RDTIMES;ii++){
                            //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                            //	if((DREG[D_CMDSTATUS]) == 100)break;
                            //	DelayMs(WAITTIME);
                            //}
                        }

                        f = Write_Cmd(C_PFOOTACTION, tmp_PfootOffVal, 0);
                        Read_Status();
                        //for(ii=0;ii<RDTIMES;ii++){
                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));		// ��״̬
                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                        //	DelayMs(WAITTIME);
                        //}

                        f = Read_D(D_Address[SYS] + SYS_LFORCESET, 1, (U16 *)(&YREG[SYS_LFORCESET])); //������������׼ֵ
                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            return;
                        }
                        g_lineforceValSet = YREG[SYS_LFORCESET];
                        lforceval = Mit_CailPadOffestLF(g_jog_point, 0);
                        g_lineforceValSetNow = g_lineforceValSet + lforceval;

                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY, 0, 0);
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                        g_modcordflag = 0;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        g_savetimes--;
                        RecovrSREG(g_savetimes, 1);

                        dis_updata3 = 1;
                        AnaPattern(0, PAT_SEW, (F_FILELEN));
                        if (pfootf == 0x10)
                        {
                            Send_Pfoot(1, MENU_MODIFY, 1); //ѹ����
                        }

                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        if ((g_modify_method == 0) && (g_modify_mode == 0))
                        { //ԭ������
                            Comm_Move(0, 0, 50, MENU_MODIFY, 1, 1);
                            g_jog_point = 0;
                        }
                        else
                        {
                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &tmpx, &tmpy, &dx, &dy, &speed, &Borpooft, &Borpfdis);
                            Comm_Move(tmpx + dx, tmpy + dy, 50, MENU_MODIFY, 1, 1);
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                        }
                        g_modcordflag = 0;
                    }
                    break;

                case KEY_MODIFY_REDO: //6
                    if (F_FILETYPE == PAT_MIT)
                    { //����

                        g_savetimes++;
                        g_reorigin = 0;

                        if (g_savetimes < g_Tsavetimes)
                        {
                            g_list_nadd = g_modaddr[g_savetimes];
                            g_list_nadd = Mit_ReadSregData(g_list_nadd);
                        }
                        else
                        {
                            //break;
                            g_list_nadd = g_modaddr[g_savetimes - 1];
                            mitRecovrSREG();
                        }
                        Mit_CreateAllData();

                        // DrawFillRect(keyd[KEY_MODIFY_MAGETOUCH].xpis, keyd[KEY_MODIFY_MAGETOUCH].ypis, keyd[KEY_MODIFY_MAGETOUCH].xsize, keyd[KEY_MODIFY_MAGETOUCH].ysize, mCOLOR ? Color_Silver : Color_White);

                        dis_updata3 = 1;
                        dis_updata4 = 1;
                        AnaPattern(0, PAT_MIT, 0);
                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        //							g_tmpoxpos = 0;g_tmpoypos = 0;
                        //							for(ii=0;ii<g_jog_point;ii++){
                        //								dat = SREG[g_patd.pat_add+ii*2+1];
                        //								xx = dat&0x000000ff;
                        //								yy = ((U32)(dat>>8))&0x000000ff;
                        //								if(xx>=0x80){xx-=0x80;g_tmpoxpos-=xx;}else{g_tmpoxpos+=xx;}
                        //								if(yy>=0x80){yy-=0x80;g_tmpoypos-=yy;}else{g_tmpoypos+=yy;}
                        //							}
                        mit_getPointXY(g_jog_point, g_patd.pat_add, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos);
                        f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET])); //���趨��ѹ��������
                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            return;
                        }
                        tmp_PfootOffVal = YREG[SYS_PFOOTSET];
                        pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //����jog_point֮ǰ����ѹ�Ÿ߶ȱ仯��
                        tmp_PfootOffVal += pfootval;

                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                        if (S_PFOOT)
                        {                                //��ѹ��̧��
                            DREG[D_COMMAND] = C_PFOOTUP; //ѹ����
                            DREG[D_CMDSTATUS] = 0;
                            DREG[D_CMDPARA1] = 0;
                            f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND]));
                            if (f == 0)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 15;
                                break;
                            }
                            Read_Status();
                            //for(ii=0;ii<RDTIMES;ii++){
                            //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                            //	if((DREG[D_CMDSTATUS]) == 100)break;
                            //	DelayMs(WAITTIME);
                            //}
                        }
                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY, 0, 0);
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����

                        f = Write_Cmd(C_PFOOTACTION, tmp_PfootOffVal, 0);
                        Read_Status();
                        //for(ii=0;ii<RDTIMES;ii++){
                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));		// ��״̬
                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                        //	DelayMs(WAITTIME);
                        //}

                        f = Read_D(D_Address[SYS] + SYS_LFORCESET, 1, (U16 *)(&YREG[SYS_LFORCESET])); //������������׼ֵ
                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            return;
                        }
                        g_lineforceValSet = YREG[SYS_LFORCESET];
                        lforceval = Mit_CailPadOffestLF(g_jog_point, 0);
                        g_lineforceValSetNow = g_lineforceValSet + lforceval;

                        g_modcordflag = 0;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        g_savetimes++;
                        RecovrSREG(g_savetimes, 1);

                        dis_updata3 = 1;
                        AnaPattern(0, PAT_SEW, (F_FILELEN));
                        if (g_jog_point > g_patd.feed_sum)
                            g_jog_point = 0;
                        if (pfootf == 0x10)
                        {
                            Send_Pfoot(1, MENU_MODIFY, 1); //ѹ����
                        }
                        if ((g_modify_method == 0) && (g_modify_mode == 0))
                        { //ԭ������
                            Comm_Move(0, 0, 50, MENU_MODIFY, 1, 0);
                            g_jog_point = 0;
                        }
                        else
                        {
                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &tmpx, &tmpy, &dx, &dy, &speed, &Borpooft, &Borpfdis);
                            Comm_Move(tmpx + dx, tmpy + dy, 50, MENU_MODIFY, 1, 1);
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                        }
                        g_modcordflag = 0;
                    }
                    break;
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {

            if (keyon == KEY_MODIFY_SCRNSVR) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                         SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ʾ�ػ������
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[��ʱ1s��Ϣ����*begin]**********************************************************
        if (Timerms > 1000)
        {
            Timerms = 0;
        }
        // /[��ʱ1s��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_MODIFY);
        }
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MODIFY);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[���Ͱ���ˢ��*begin]**********************************************************
        if (dis_updata)
        {
            dis_updata = 0;
            f = KEY_MODIFY_SEWMODI + tmp_modify_mode;
            for (i = KEY_MODIFY_SEWMODI; i <= KEY_MODIFY_CODEMOD; i++)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                           (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
            }
        }
        // /[���Ͱ���ˢ��*end]**********************************************************

        // /[��ԭ�㷽ʽ����ˢ��*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;

            f = KEY_MODIFY_MODIFYHOME + tmp_gohome;
            for (i = KEY_MODIFY_MODIFYHOME; i <= KEY_MODIFY_DIRECTHOME; i++)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                           (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
            }
        }
        // /[��ԭ�㷽ʽ����ˢ��*end]**********************************************************

        // /[��ʽ��ťˢ��*begin]**********************************************************
        if (dis_updata2)
        {
            dis_updata2 = 0;

            if (tmpmodifymode != tmp_modify_mode)
            {
                BmpFillRect(rect[RECT_MODIFY_METHOD].xpis, rect[RECT_MODIFY_METHOD].ypis,
                            rect[RECT_MODIFY_METHOD].xsize, rect[RECT_MODIFY_METHOD].ysize);
                if (tmp_modify_mode != 1)
                {
                    DrawRect(rect[RECT_MODIFY_METHOD].xpis, rect[RECT_MODIFY_METHOD].ypis,
                             rect[RECT_MODIFY_METHOD].xsize, rect[RECT_MODIFY_METHOD].ysize, rect[RECT_MODIFY_METHOD].colorB[0]);
                    Draw24x24Str(rect[RECT_MODIFY_METHOD].xpis + SAFE_DIST4, rect[RECT_MODIFY_METHOD].ypis + SAFE_DIST3,
                                 MEM_TITLE_ADD + rect[RECT_MODIFY_METHOD].index[0] * TitleLengthMax,
                                 SheetPage, rect[RECT_MODIFY_METHOD].index[0], rect[RECT_MODIFY_METHOD].colorT[0]); //�������������
                }
                tmpmodifymode = tmp_modify_mode;
            }
            if (tmp_modify_mode == 0)
            {
                f = KEY_MODIFY_STARTPOSI + tmp_modify_method;
                for (i = KEY_MODIFY_STARTPOSI; i <= KEY_MODIFY_SECTINSR; i++)
                {
                    if (((i == KEY_MODIFY_PFOOTMD) && (PFOOTBT == 0)) ||
                        ((i == KEY_MODIFY_TENSIONMD) && (TENSIONBT == 0)))
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[1], keybmpind[i].icon[1]);
                    }
                    else
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (i == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                   (i == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                    }
                    Draw16x16Str(keyd[i].xpis - SAFE_DIST7 +
                                     (keyd[i].xsize + SAFE_DIST10 - 8 * strlen((char *)(MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax))) / 2,
                                 keyd[i].ypis + keyd[i].ysize + SAFE_DIST3,
                                 MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax,
                                 SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[0]); //�������������
                }
            }
            else if (tmp_modify_mode == 2)
            {
                f = KEY_MODIFY_INCCODE + tmp_modify_code;
                for (i = KEY_MODIFY_INCCODE; i <= KEY_MODIFY_DELCODE; i++)
                {

                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (i == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (i == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                    Draw16x16Str(keyd[i].xpis - SAFE_DIST7 +
                                     (keyd[i].xsize + SAFE_DIST10 - 8 * strlen((char *)(MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax))) / 2,
                                 keyd[i].ypis + keyd[i].ysize + SAFE_DIST3,
                                 MEM_TITLE_ADD + keybmpind[i].nameStr[0] * TitleLengthMax,
                                 SheetPage, keybmpind[i].nameStr[0], keybmpind[i].textColor[0]); //�������������
                }
            }
        }
        // /[��ʽ��ťˢ��*end]**********************************************************

        // /[������ʾˢ��*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;
            g_reorigin = 0;

            AnaPattern(0, F_FILETYPE, (F_FILELEN));
            // DrawFillRect(keyd[KEY_MODIFY_MAGETOUCH].xpis, keyd[KEY_MODIFY_MAGETOUCH].ypis,
            //              keyd[KEY_MODIFY_MAGETOUCH].xsize, keyd[KEY_MODIFY_MAGETOUCH].ysize, keybmpind[KEY_MODIFY_MAGETOUCH].icon[0]);
            // Draw_Pattern(PAT_MODIFY, 0, CONVAS_ORG_PX6, CONVAS_ORG_PY6, F_FILETYPE, (F_FILELEN), 0, 0, NULL); // ��ʾ����
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(PATRULES_MODIFY_MAIN, 0, patConvas.conOrgX, patConvas.conOrgY, F_FILETYPE, F_FILELEN, 0, 0, NULL);

            if (g_savetimes == 0)
            {
                BmpFillRect(keyd[KEY_MODIFY_UNDO].xpis, keyd[KEY_MODIFY_UNDO].ypis,
                            keyd[KEY_MODIFY_UNDO].xsize, keyd[KEY_MODIFY_UNDO].ysize);
            }
            else
            {
                DrawButton(keyd[KEY_MODIFY_UNDO], (U16 *)(ADD_ICONBMP), keybmpind[KEY_MODIFY_UNDO].border[0], keybmpind[KEY_MODIFY_UNDO].icon[0]);
            }
            if ((F_FILETYPE == PAT_MIT) ? g_savetimes >= g_Tsavetimes : g_savetimes >= g_Tsavetimes)
            {
                BmpFillRect(keyd[KEY_MODIFY_REDO].xpis, keyd[KEY_MODIFY_REDO].ypis,
                            keyd[KEY_MODIFY_REDO].xsize, keyd[KEY_MODIFY_REDO].ysize);
            }
            else
            {
                DrawButton(keyd[KEY_MODIFY_REDO], (U16 *)(ADD_ICONBMP), keybmpind[KEY_MODIFY_REDO].border[0], keybmpind[KEY_MODIFY_REDO].icon[0]);
            }
#if EN_SEWCHECK
            if (F_FILETYPE == PAT_SEW)
            {
                if (sew_Patcheck((U16 *)SREG, F_FILELEN) == 0)
                {
                    BmpFillRect(448, 560, 80, 80);
                    Draw24X24FStr(448, 560, "Error", Color_Red);
                    DrawFHEX(526, 560, g_sewcheck.flag, 2, Color_Blue);
                    DrawMFU32Z(448, 588, g_sewcheck.len, 6, Color_Blue);
                    DrawMFU32Z(448, 616, g_sewcheck.len1, 6, Color_Red);
                }
            }
#endif
        }
        // /[������ʾˢ��*end]**********************************************************

        if (dis_updata4)
        {
            dis_updata4 = 0;
            //����������ʾ
            for (i = RECT_MODIFY_XM_TIT; i <= RECT_MODIFY_PINS_STR; i++)
            {
                if (i == RECT_MODIFY_XM_TIT || i == RECT_MODIFY_YM_TIT || i == RECT_MODIFY_XI_TIT ||
                    i == RECT_MODIFY_YI_TIT || i == RECT_MODIFY_PINS_TIT)
                {
                    DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    Draw16x16Str(rect[i].xpis + (rect[i].xsize - 8 * strlen((char *)(MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax))) / 2,
                                 rect[i].ypis + (rect[i].ysize - 16) / 2,
                                 MEM_TITLE_ADD + rect[i].index[0] * TitleLengthMax,
                                 SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
                }
                else
                {
                    DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                }
            }
            DrawFS32P(rect[RECT_MODIFY_XM_STR].xpis + SAFE_DIST4,
                      rect[RECT_MODIFY_XM_STR].ypis + SAFE_DIST4,
                      g_patd.patmX, 5, 1, rect[RECT_MODIFY_XM_STR].colorT[0]);
            DrawFS32P(rect[RECT_MODIFY_XI_STR].xpis + SAFE_DIST4,
                      rect[RECT_MODIFY_XI_STR].ypis + SAFE_DIST4,
                      g_patd.patiX, 5, 1, rect[RECT_MODIFY_XI_STR].colorT[0]);
            DrawFS32P(rect[RECT_MODIFY_YM_STR].xpis + SAFE_DIST4,
                      rect[RECT_MODIFY_YM_STR].ypis + SAFE_DIST4,
                      g_patd.patmY, 5, 1, rect[RECT_MODIFY_YM_STR].colorT[0]);
            DrawFS32P(rect[RECT_MODIFY_YI_STR].xpis + SAFE_DIST4,
                      rect[RECT_MODIFY_YI_STR].ypis + SAFE_DIST4,
                      g_patd.patiY, 5, 1, rect[RECT_MODIFY_YI_STR].colorT[0]);

            DrawFU32(rect[RECT_MODIFY_PINS_STR].xpis + SAFE_DIST4,
                     rect[RECT_MODIFY_PINS_STR].ypis + (rect[RECT_MODIFY_PINS_STR].ysize - 16) / 2,
                     g_patd.feed_sum, 5, rect[RECT_MODIFY_PINS_STR].colorT[0]);
        }

        // /[��¼��忪ʼʱ��ѹ�Ÿ߶�	*begin]**********************************************************
        if (g_zhouDown)
        {
            g_zhouDown = 0;
            Write_Cmd(C_LDOWMPF, 0, 0); //��¼��忪ʼʱ��ѹ�Ÿ߶�
            Read_Status();
        }
        // /[��¼��忪ʼʱ��ѹ�Ÿ߶�	*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MODIFY)
        {
            plastno = MENU_MODIFY;
            g_pno_temp = MENU_MODIFY;
            break;
        } //pnoindex = 2;poldno[1] = MENU_MODIFY;
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

/************************************/
#define MODA_TEXT_ADDR 34
#define MODA_COM_ADDR MODA_TEXT_ADDR
#define MODA_COM_LEN 27
#define MODA_CODE_ADDR MODA_COM_ADDR + MODA_COM_LEN
#define MODA_CODE_LEN 33
#define MODA_KEY1_ADDR MODA_CODE_ADDR + MODA_CODE_LEN
#define MODA_KEY1_LEN 5

#define RECT_STATUS_PINS_TIT 1  //1 ����
#define RECT_STATUS_X_TIT 2     //2 X����
#define RECT_STATUS_Y_TIT 3     //3 Y����
#define RECT_STATUS_PF_TIT 4    //4 ��ѹ�Ÿ߶�
#define RECT_STATUS_CODE_TIT 5  //5 ���
#define RECT_STATUS_SPEED_TIT 6 //6 �ٶ�
#define RECT_STATUS_PITCH_TIT 7 //7 ����

#define RECT_STATUS_TIT_NUM 8

#define RECT_STATUS_STR 1 //8 ����

//---------------------------------------------------------------
// Function   : void clrAllStatus(RECTPAD backRect)
// Description: ���ѡ�е�ѡ����Ϣ��
// note: xpos��ypos����������
//---------------------------------------------------------------
void clrAllStatus(RECTPAD backRect)
{
    BmpFillRect(backRect.xpis, backRect.ypis - TYPE_SIZE4, backRect.xsize, backRect.ysize + TYPE_SIZE4);
}

//---------------------------------------------------------------
// Function   : void disStatusTit(U16 xpos, U16 ypos,U16 chTitleIndex)
// Description: �޸�ģ���ڣ�ѡ�ص�����
// note: xpos��ypos����������
//---------------------------------------------------------------
void disStatusTit(RECTPAD backRect, U16 chTitleIndex, U8 bEnableTit)
{
    U16 i;
    //�������
    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_White, Color_Black}, {Color_Black}, {0}},

        {SAFE_DIST3, SAFE_DIST3, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {23}},                  //1 ����
        {SAFE_DIST3, SAFE_DIST3 + TYPE_SIZE3, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {29}},     //2 X����
        {SAFE_DIST3, SAFE_DIST3 + TYPE_SIZE3 * 2, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {30}}, //3 Y����
        {SAFE_DIST3, SAFE_DIST3 + TYPE_SIZE3 * 3, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {31}}, //4 ��ѹ�Ÿ߶�
        {SAFE_DIST3, SAFE_DIST3 + TYPE_SIZE3 * 4, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {33}}, //5 ���
        {SAFE_DIST3, SAFE_DIST3 + TYPE_SIZE3 * 5, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {25}}, //6 �ٶ�
        {SAFE_DIST3, SAFE_DIST3 + TYPE_SIZE3 * 6, TYPE_SIZE27, TYPE_SIZE3, {Color_White}, {Color_Black}, {24}}, //7 ����
    };
    SetLanguage(MENU_MODIFYADD);
    BmpFillRect(backRect.xpis, backRect.ypis - TYPE_SIZE4, backRect.xsize, backRect.ysize + TYPE_SIZE4);
    Draw24x24Str(backRect.xpis, backRect.ypis - TYPE_SIZE4,
                 (MEM_TITLE_ADD + TitleLengthMax * chTitleIndex),
                 SheetPage, chTitleIndex, backRect.colorT[0]); // ����
    if (bEnableTit == 0)
    {
        return;
    }
    DrawFillRect(backRect.xpis, backRect.ypis, backRect.xsize, backRect.ysize, backRect.colorB[0]);
    DrawRect(backRect.xpis, backRect.ypis, backRect.xsize, backRect.ysize, backRect.colorB[1]);
    for (i = 1; i < RECT_STATUS_TIT_NUM; i++)
    {
        Draw16x16Str(backRect.xpis + rect[i].xpis, backRect.ypis + rect[i].ypis + (rect[i].ysize - 16) / 2,
                     (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                     SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
    }
}
//---------------------------------------------------------------
// Function   : disStatus(U16 xpos, U16 ypos, U16 dat, U16 vdat, S8 dlf, S8 dpf, U16 color)
// Description: �޸�ģ���ڣ�ѡ�ص�����
// note: xpos��ypos���������꣬ dat�������룬color:������ɫ
//---------------------------------------------------------------
void disStatus(U16 xpos, U16 ypos, U16 dat, U16 vdat, S8 dlf, S8 dpf, U16 color)
{
    U16 abspindis, i;
    U8 spd = 0, feed_f = 0;
    // S8 pfoot = 0;
    U16 *padd, psheet;

    //�������
    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_White, Color_Black}, {Color_Black}, {0}},
        {SAFE_DIST3 + TYPE_SIZE7, SAFE_DIST3, 105, TYPE_SIZE3, {Color_White}, {Color_Black, Color_Red}, {0}}, //8 ����
    };

    if (P_LAN)
    {
        padd = MEM_TITLE_CODENAME_E;
        psheet = 34;
    }
    else
    {
        padd = MEM_TITLE_CODENAME;
        psheet = 34;
    }
    for (i = 2; i < RECT_STATUS_TIT_NUM; i++)
    {
        DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * (i - 1),
                     rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
                     rect[RECT_STATUS_STR].colorB[0]);
    }
    //-- ����  --��һ��
    // DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis,
    //              9 * 5, rect[RECT_STATUS_STR].ysize,
    //              rect[RECT_STATUS_STR].colorB[0]);
    DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis,
                 ypos + rect[RECT_STATUS_STR].ypis,
                 9 * 6, rect[RECT_STATUS_STR].ysize,
                 rect[RECT_STATUS_STR].colorB[0]);
    DrawFU32(xpos + rect[RECT_STATUS_STR].xpis,
             ypos + rect[RECT_STATUS_STR].ypis + (rect[RECT_STATUS_STR].ysize - 16) / 2,
             g_jog_point, 5, color);

    DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 9 * 5,
               ypos + rect[RECT_STATUS_STR].ypis + (rect[RECT_STATUS_STR].ysize - 16) / 2,
               "/", rect[RECT_STATUS_STR].colorT[0]);

    // DrawFU32(xpos + rect[RECT_STATUS_STR].xpis + 9 * 6,
    //          ypos + rect[RECT_STATUS_STR].ypis + (rect[RECT_STATUS_STR].ysize - 16) / 2,
    //          g_feedsum, 5, rect[RECT_STATUS_STR].colorT[0]); //max_feedsum
    // DrawFillRect(xpos + 406, ypos, 65, 24, mCOLOR ? Color_Silver : Color_White);
    // DrawMFU32(xpos + 406, ypos, g_jog_point, 5, color);

    // for (i = 0; i < 2; i++)
    // {
    //     DrawFillRect(xpos + 43 + 151 * i, ypos + 60, 93, 24, mCOLOR ? Color_Silver : Color_White);
    //     DrawFillRect(xpos + 104 + 300 * i, ypos + 28, 130, 24, mCOLOR ? Color_Silver : Color_White);
    //     DrawFillRect(xpos + 504, ypos + 60 + i * 28, 60, 24, mCOLOR ? Color_Silver : Color_White);
    // }

    // DrawFillRect(xpos + 234, ypos + 28, 52, 24, mCOLOR ? Color_Silver : Color_White);
    if (F_FILETYPE == PAT_MIT)
    { //mit
        //-- x���� -- x�������-- ��2��
        //DrawFillRect(xpos+43,ypos+60,80,24,mCOLOR?Color_Silver:Color_White);
        // DrawMFS32P(xpos + 56, ypos + 60, g_dxpos, 5, 1, color);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 1 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  g_dxpos, 5, 1, rect[RECT_STATUS_STR].colorT[0]);

        //-- y���� -- y�������-- ��3��
        //DrawFillRect(xpos+194,ypos+60,80,24,mCOLOR?Color_Silver:Color_White);
        // DrawMFS32P(xpos + 207, ypos + 60, g_dypos, 5, 1, color);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  g_dypos, 5, 1, rect[RECT_STATUS_STR].colorT[0]);

        if ((dat == 0xff) || (dat == 0x1e) || (dat == 0x1f))
        { //����
            spd = g_speed;
            feed_f = 2;
        }
        else if (dat == 0)
        {
            spd = g_speed;
            feed_f = g_feed_flag;
        }
        else if (dat == 0x3)
        { //����
            spd = g_speed;
            feed_f = 0;
        }
        else if (dat == 0x1b)
        { //������
            spd = g_speed;
            feed_f = 16;
        }
        else if (((dat & 0x00ff) == 0x1) || ((dat & 0x00ff) == 0x21) || ((dat & 0x00ff) == 0x41) || ((dat & 0x00ff) == 0x61))
        {
            spd = (dat & 0x00ff) - 1;
            feed_f = 1;

            if ((dat & 0xff00) != 0)
            {
                // pfoot = (S8)(dat >> 8);
            }
        }
        else
        {
            spd = g_speed;
            for (i = 1; i < 19; i++)
            {
                if (dat == pat_code[i])
                    break;
            }
            if ((g_UdZhouFlag == 3) && (dat == 0x31d))
                feed_f = 32; //����
            else if (i < 19)
                feed_f = i + 2;
            else if (dat == 0x3a)
                feed_f = 25; // ������
            else if ((dat & 0xff) == 0x3c)
                feed_f = 33; // ��Ƕ�
            else if ((dat & 0xff) == 0x3d)
                feed_f = 34; // ����һ
            else if ((dat & 0xff) == 0x3e)
                feed_f = 35; // ������
            else
                feed_f = 1; //����
        }

        //printf("dat = %04x  feed_f = %d \r\n",dat,feed_f);
        if (spd == 0x60)
            spd = 21; //����
        else if (spd == 0x40)
            spd = 22; //�и�
        else if (spd == 0x20)
            spd = 23; //�е�
        else
            spd = 24; //����

        //DrawFillRect(xpos+234,ypos+28,52,24,mCOLOR?Color_Silver:Color_White);
        //-- ��ʾ������� --��7��
        if (feed_f == 33)
        {
            if ((dat >> 8) == 0xff)
            {
                // Draw24X24FStr(xpos + 234, ypos + 28, "AUTO", Color_Red);
                DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                           ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                           "AUTO", rect[RECT_STATUS_STR].colorT[0]);
            }
            else
            {
                // DrawMFS32P(xpos + 234, ypos + 28, (dat >> 8) * 2, 3, 0, Color_Red);
                DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                          ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                          (dat >> 8) * 2, 3, 0, rect[RECT_STATUS_STR].colorT[0]);
            }
        }
        else if (feed_f >= 17 && feed_f <= 20)
        { // ������,������,���Ϻ�,ѹ���� ����
            // DrawMFS32P(xpos + 234, ypos + 28, vdat, 3, 0, Color_Red);
            DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                      ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                      vdat, 3, 0, rect[RECT_STATUS_STR].colorT[0]);
        }
        else if (feed_f == 34 || feed_f == 35)
        {
            // DrawMFS32P(xpos + 234, ypos + 28, (dat >> 8), 3, 0, Color_Red);
            DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                      ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                      (dat >> 8), 3, 0, rect[RECT_STATUS_STR].colorT[0]);
        }
    }
    else if (F_FILETYPE == PAT_SEW)
    {   //sew
        //-- xy ����
#if MOVEUNIT == 1
        DrawMFS32P(xpos + 56, ypos + 60, g_dxpos * 5, 6, 2, color);
        DrawMFS32P(xpos + 207, ypos + 60, g_dypos * 5, 6, 2, color);
#else
        //DrawFillRect(xpos+43,ypos+60,80,24,mCOLOR?Color_Silver:Color_White);
        // DrawMFS32P(xpos + 56, ypos + 60, g_dxpos / 2, 5, 1, color);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  g_dxpos, 5, 1, rect[RECT_STATUS_STR].colorT[0]);

        //DrawFillRect(xpos+194,ypos+60,80,24,mCOLOR?Color_Silver:Color_White);
        // DrawMFS32P(xpos + 207, ypos + 60, g_dypos / 2, 5, 1, color);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  g_dypos, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
#endif

        if ((dat & 0xf000) == 0x2000)
        {
            feed_f = 0;
        } //����
        else if ((dat & 0xf000) == 0x6000)
        {
            feed_f = 1;
        } //����
        else if ((dat & 0xff00) == 0xf000)
        {
            feed_f = 26;
        } //��ѹ��
        else if ((dat & 0xff00) == 0xf100)
        {
            feed_f = 25;
        } //������,������
        else if ((dat & 0xfff0) == 0xf340)
        {
            feed_f = 28;
        } //��ͣ��
        else if ((dat & 0xfff0) == 0xf300)
        {
            feed_f = 29;
        } //��ͣ��
        else if ((dat & 0xff00) == 0xf400)
        {
            feed_f = 14;
        } //����
        else if ((dat & 0xff00) == 0xfb00)
        {
            feed_f = 30;
        } //���� (����)
        else if ((dat & 0xff00) == 0xfd00)
        {
            feed_f = 13;
        } //����
        else if ((dat & 0xffff) == 0xfe00)
        {
            feed_f = 11;
        } //����ͣ
        else if ((dat & 0xffff) == 0xfe01)
        {
            feed_f = 12;
        } //����ͣ
        else if ((dat & 0xff00) == 0xff00)
        {
            feed_f = 2;
        } //����
        else if ((dat & 0xff00) == 0xfC00)
        {
            if (dat == 0xfC01)
            {
                feed_f = 3;
            } //����һ
            else if (dat == 0xfC02)
            {
                feed_f = 4;
            } //���ܶ�
            else if (dat == 0xfC04)
            {
                feed_f = 5;
            } //������
            else if (dat == 0xfC08)
            {
                feed_f = 6;
            } //������
            else if (dat == 0xfC10)
            {
                feed_f = 7;
            } //������
            else if (dat == 0xfC20)
            {
                feed_f = 8;
            } //������
            else if (dat == 0xfC40)
            {
                feed_f = 9;
            } //������
            else if (dat == 0xfC80)
            {
                if (g_UdZhouFlag == 3)
                    feed_f = 32;
                else
                    feed_f = 10;
            } //���� ���ܰ�
            else
            {
                feed_f = 31;
            } //����X
        }
        else if (dat == 0x5050)
        { //��ԭ��
            feed_f = 15;
        }
        else if ((dat & 0xff00) == 0xec00)
        {
            feed_f = 33;
        } //��Ƕ�
        else if ((dat & 0xff00) == 0xea00)
        {
            feed_f = 34;
        } //����һ
        else if ((dat & 0xff00) == 0xeb00)
        {
            feed_f = 35;
        } //������
        else if ((dat & 0xff00) == 0xee00)
        {
            feed_f = 36;
        } //�ٶ�

        if (g_oldspeed == 0)
        {
            spd = 21;
        } //����
        else if (g_oldspeed == 0x01)
        {
            spd = 22;
        } //�и���
        else if (g_oldspeed == 0x02)
        {
            spd = 23;
        } //�е���
        else
        {
            spd = 24;
        } //����

        // pfoot = dpf;
        //DrawFillRect(xpos+234,ypos+28,52,24,mCOLOR?Color_Silver:Color_White);
        if (feed_f == 33)
        {
            if ((dat & 0xff) == 0xff)
            {
                // Draw24X24FStr(xpos + 234, ypos + 28, "AUTO", Color_Red);
                DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                           ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                           "AUTO", rect[RECT_STATUS_STR].colorT[0]);
            }
            else
            {
                // DrawMFS32P(xpos + 234, ypos + 28, (dat & 0xff) * 2, 3, 0, Color_Red);
                DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                          ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                          (dat & 0xff) * 2, 3, 0, rect[RECT_STATUS_STR].colorT[0]);
            }
        }
        else if (feed_f == 34 || feed_f == 35)
        {
            // DrawMFS32P(xpos + 234, ypos + 28, (dat & 0xff), 3, 0, Color_Red);
            DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 8 * 8,
                      ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                      (dat & 0xff), 3, 0, rect[RECT_STATUS_STR].colorT[0]);
        }
    }

    //-- ��ѹ�ű仯ֵ --��4��
    // DrawFillRect(xpos + 356, ypos + 60, 80, 24, mCOLOR ? Color_Silver : Color_White);
    // DrawMFS32P(xpos + 369, ypos + 60, pfoot, 4, 1, color);
    // DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 3,
    //              rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
    //              rect[RECT_STATUS_STR].colorB[0]);

    // DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 3 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
    //           pfoot, 4, 1, rect[RECT_STATUS_STR].colorT[0]);

    //-- ��������� -- ��
    //DrawFillRect(xpos+504,ypos+60,60,24,mCOLOR?Color_Silver:Color_White);
    // DrawMFS32P(xpos + 504, ypos + 60, dlf, 3, 0, color);

    //-- �����߾���ֵ -- ��
    //DrawFillRect(xpos+504,ypos+88,60,24,mCOLOR?Color_Silver:Color_White);
    // DrawMFS32P(xpos + 504, ypos + 88, g_lineforceValSetNow, 3, 0, color);

    //-- ��ʾ���� --��7��
    //DrawFillRect(xpos+104,ypos+28,130,24,mCOLOR?Color_Silver:Color_White);
    // Draw24x24Str(xpos + 104, ypos + 28, (padd + ((U32)(TitleLengthMax)) * feed_f), psheet, feed_f, color);
    // DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6,
    //              rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
    //              rect[RECT_STATUS_STR].colorB[0]);
    Draw16x16Str(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 6 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                 (padd + ((U32)(TitleLengthMax)) * feed_f),
                 psheet, feed_f, rect[RECT_STATUS_STR].colorT[0]); //�������������

    //-- �ٶ�  --��6��
    //DrawFillRect(xpos+404,ypos+28,130,24,mCOLOR?Color_Silver:Color_White);
    // Draw24x24Str(xpos + 404, ypos + 28, (padd + ((U32)(TitleLengthMax)) * spd), psheet, spd, color);
    // DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 5,
    //              rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
    //              rect[RECT_STATUS_STR].colorB[0]);
    Draw16x16Str(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 5 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                 (padd + ((U32)(TitleLengthMax)) * spd),
                 psheet, spd, rect[RECT_STATUS_STR].colorT[0]); //�������������

    //-- ���  --��5��
    //if((g_user.newuser>7)&&(g_user.newuser<10)){
    if (F_FILETYPE == PAT_SEW)
    {
        abspindis = sqrt((double)(g_dxpos * g_dxpos / 4) + (double)(g_dypos * g_dypos / 4));
    }
    else
    {
        abspindis = sqrt((double)(g_dxpos * g_dxpos) + (double)(g_dypos * g_dypos));
    }

    // DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 4,
    //              rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
    //              rect[RECT_STATUS_STR].colorB[0]);

    DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 4 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
              abspindis, 6, 1, rect[RECT_STATUS_STR].colorT[0]);
    //}
}
//-----------------------------------------
//-������ʾ�������ֵ
//----------------------------------------
void disStatusFeedSum(U16 xpos, U16 ypos, U32 wMaxFeedSum, U16 color)
{
    const RECTPAD rect[] = {
        //����
        {SAFE_DIST3 + TYPE_SIZE7, SAFE_DIST3, 105, TYPE_SIZE3, {Color_White}, {Color_Black, Color_Red}, {0}}, //8 ����
    };
    // g_feedsum = wMaxFeedSum;
    // disStatus(xpos, ypos, dat, vdat, dlf, dpf, color);
    DrawFillRect(xpos + rect[0].xpis + 9 * 6,
                 ypos + rect[0].ypis,
                 9 * 5, rect[0].ysize,
                 rect[0].colorB[0]);
    DrawFU32(xpos + rect[0].xpis + 9 * 6,
             ypos + rect[0].ypis + (rect[0].ysize - 16) / 2,
             wMaxFeedSum, 5, color); //max_feedsum
}
//-----------------------------------------
//-������ʾ��ǰ����
//----------------------------------------
void disStatusFeedNow(U16 xpos, U16 ypos, U32 wFeedNow, U16 color)
{
    const RECTPAD rect[] = {
        //����
        {SAFE_DIST3 + TYPE_SIZE7, SAFE_DIST3, 105, TYPE_SIZE3, {Color_White}, {Color_Black, Color_Red}, {0}}, //8 ����
    };
    // g_feedsum = wMaxFeedSum;
    // disStatus(xpos, ypos, dat, vdat, dlf, dpf, color);
    DrawFillRect(xpos + rect[0].xpis,
                 ypos + rect[0].ypis,
                 9 * 5, rect[0].ysize,
                 rect[0].colorB[0]);
    DrawFU32(xpos + rect[0].xpis,
             ypos + rect[0].ypis + (rect[0].ysize - 16) / 2,
             wFeedNow, 5, color); //wFeedNow
}
//-----------------------------------------
//-����ʾX/Y�ľ������� �仯�� �� ��ѹ��ֵ
//----------------------------------------
void disStatusUpdate(U16 xpos, U16 ypos, S32 dx, S32 dy, S32 abs_x, S32 abs_y, S8 dpf)
{
    S8 pfoot = dpf;

    //�������
    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_White, Color_Black}, {Color_Black}, {0}},
        {SAFE_DIST3 + TYPE_SIZE7, SAFE_DIST3, 105, TYPE_SIZE3, {Color_White}, {Color_Black, Color_Red}, {0}}, //8 ����
    };

    DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize,
                 rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
                 rect[RECT_STATUS_STR].colorB[0]);
    DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis,
                 ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2,
                 rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
                 rect[RECT_STATUS_STR].colorB[0]);
    DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis,
                 ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 3,
                 rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
                 rect[RECT_STATUS_STR].colorB[0]);
    //-- ����  --��һ��

    // DrawFillRect(xpos + 234, ypos + 28, 52, 24, mCOLOR ? Color_Silver : Color_White);
    if (F_FILETYPE == PAT_MIT)
    { //mit
        //-- x���� -- x��������-- ��2��
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 1 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  dx, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
        DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 9 * 5,
                   ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 1 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                   "(     )", rect[RECT_STATUS_STR].colorT[0]);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 9 * 6, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 1 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  abs_x, 5, 1, rect[RECT_STATUS_STR].colorT[0]);

        //-- y���� -- y��������-- ��3��
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  dy, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
        DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 9 * 5,
                   ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                   "(     )", rect[RECT_STATUS_STR].colorT[0]);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 9 * 6, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  abs_y, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
    }
    else if (F_FILETYPE == PAT_SEW)
    {   //sew
        //-- xy ����
#if MOVEUNIT == 1
        DrawMFS32P(xpos + 56, ypos + 60, g_dxpos * 5, 6, 2, color);
        DrawMFS32P(xpos + 207, ypos + 60, g_dypos * 5, 6, 2, color);
#else
        //DrawFillRect(xpos+43,ypos+60,80,24,mCOLOR?Color_Silver:Color_White);
        // DrawMFS32P(xpos + 56, ypos + 60, g_dxpos / 2, 5, 1, color);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  dx / 2, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
        DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 9 * 5,
                   ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 1 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                   "(     )", rect[RECT_STATUS_STR].colorT[0]);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 9 * 6, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  abs_x, 5, 1, rect[RECT_STATUS_STR].colorT[0]);

        //DrawFillRect(xpos+194,ypos+60,80,24,mCOLOR?Color_Silver:Color_White);
        // DrawMFS32P(xpos + 207, ypos + 60, g_dypos / 2, 5, 1, color);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  dy / 2, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
        DrawF16Str(xpos + rect[RECT_STATUS_STR].xpis + 9 * 5,
                   ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                   "(     )", rect[RECT_STATUS_STR].colorT[0]);
        DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis + 9 * 6, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 2 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
                  abs_y, 5, 1, rect[RECT_STATUS_STR].colorT[0]);
#endif
    }

    //-- ��ѹ�ű仯ֵ --��4��
    // DrawFillRect(xpos + 356, ypos + 60, 80, 24, mCOLOR ? Color_Silver : Color_White);
    // DrawMFS32P(xpos + 369, ypos + 60, pfoot, 4, 1, color);
    // DrawFillRect(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 3,
    //              rect[RECT_STATUS_STR].xsize, rect[RECT_STATUS_STR].ysize,
    //              rect[RECT_STATUS_STR].colorB[0]);
    DrawFS32P(xpos + rect[RECT_STATUS_STR].xpis, ypos + rect[RECT_STATUS_STR].ypis + rect[RECT_STATUS_STR].ysize * 3 + (rect[RECT_STATUS_STR].ysize - 16) / 2,
              pfoot, 4, 1, rect[RECT_STATUS_STR].colorT[0]);
}
/*****************************************************/
#define KEY_MODIADD_SCRNSVR 1 //1 ����
#define KEY_MODIADD_BACK 2    //2 ����
#define KEY_MODIADD_ENTER 3   //3 ȷ��
#define KEY_MODIADD_IMGO 4    //4 ��С
#define KEY_MODIADD_IMGI 5    //5 �Ŵ�
#define KEY_MODIADD_ORG 6     //6 ��ԭ��
#define KEY_MODIADD_FOOT 7    //7 ѹ������
#define KEY_MODIADD_PAT 8     //8 ����
#define KEY_MODIADD_IMG 9     //9 ͼ���϶�
#define KEY_MODIADD_MAX1 10

#define KEY_MODIADD_MUP 10   //10 �綯+
#define KEY_MODIADD_MDOWN 11 //11 �綯-
#define KEY_MODIADD_MAX2 2

#define KEY_MODIADD_JOGUP 12       //12 ����++
#define KEY_MODIADD_SKIPJOGUP 13   //13 ����+
#define KEY_MODIADD_SKIPJOGDOWN 14 //14 ����-
#define KEY_MODIADD_JOGDOWN 15     //15 ����--
#define KEY_MODIADD_MAX3 3

#define KEY_MODIADD_JOGON 16    //16 �綯������
#define KEY_MODIADD_JOGOFF 17   //17 �綯������
#define KEY_MODIADD_INPUTEND 18 //18 �������
#define KEY_MODIADD_MAX4 3

#define KEY_MODIADD_AUTO 19 //19 �������2
#define KEY_MODIADD_MAX5 1

// #define KEY_MODIADD_DELN 20     //20 ɾ��N��
// #define KEY_MODIADD_DELN_REL 21 //21 ɾ��N��-���
// #define KEY_MODIADD_DELALL 22   //22 ɾ��ȫ��
// #define KEY_MODIADD_MAX5 3

// #define KEY_MODIADD_SELN 23   //23 ѡ��N��
// #define KEY_MODIADD_SELALL 24 //24 ѡ��ȫ��
// #define KEY_MODIADD_MAX6 2

#define KEY_MODIADD_MAX 20

#define RECT_MODIADD_PIN1 1 //1 ѡ�����1
#define RECT_MODIADD_PIN2 2 //2 ѡ�����2
// #define RECT_MODIADD_METHOD1 3 //3 ����ѡ��1
// #define RECT_MODIADD_METHOD2 4 //4 ����ѡ��2
#define RECT_MODIADD_IN_TIT 3 //5 �����-����
#define RECT_MODIADD_IN_STR 4 //6 �����-����

#define RECT_MODIADD_MAX 5

void Menu_ModiAdd(U32 para)
{

    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ

    /****������ز���end****/

    /****��־λ��ز���begin****/
    U32 i;
    U8 f;
    U32 ii = 0, dat = 0, dat1 = 0;
    U16 vdat = 0;
    U32 tmp_feedsum = 0;
    U16 runf1 = 0, runf2 = 0;
    U32 max_feedsum = 0, min_feedsum = 0;
    U8 x, y;
    U8 dis_updata = 1,   //dis_updata:����ѡ�����������ʾ
        dis_updata1 = 0, //dis_updata1:��ʾJOG ON/OFF��ť
        dis_updata4 = 1; //dis_updata4:��ʾ����
    U8 uplock = 1;
    U8 Up = 0;
    U8 Dn = 1;
    S8 pfootval = 0;
    S16 PfootOffVal = 0, old_PfootOffVal = 0;
    S16 temp_PfVal;
    S8 lforceval = 0, dlforceval = 0;
    U8 pfootf = 0xff;
    U16 Borcmd = 0x2000;
    S8 Borpooft = 0, Borpfdis;
    S16 tmpxdat = 0, tmpydat = 0;
    S16 temp_oxpos1, temp_oypos1, temp_oxpos2, temp_oypos2, tempdx, tempdy;
    static U8 EnUPflag = 0;
    U8 movfeed = 0;
    //S32 xabsposold =0,yabsposold = 0,pf_absold=0;
    U8 excode = 0;
    U8 Stflag = 2; //0����еԭ��  1����ԭ��
    U8 isHome = 0, vflag = 0;
    U8 speed;
    U16 tmpstpin;
    //	U16 vxp =0,vyp =0;
    // U16 vxp1 = 0, vyp1 = 0;
    // U16 vxp2 = 0, vyp2 = 0;
    S16 absposx = 0xffff, absposy = 0xffff, abspf = 0xffff;
    U16 tBorcmd;
    S16 ttemp_oxpos1, ttemp_oypos1, tdxpos, tdypos;
    U8 toldspeed;
    S8 tBorpooft, tBorpfdis;
    U8 vpointf = 0;
    U16 vpoint = 0;
    S16 fpx, fpy;
    S32 vposx, vposy;
    S16 xorg = 0, yorg = 0;
    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    S32 scale = 0;
    S32 tscale = 0, tscale1 = 0;
    U8 bResetCaonvas = 0;
    S8 tmuly = 1;
    U8 lfnow;
    U16 tjog_point = 0;
    const S16 ORG_X = CONVAS_ORG_PX7, ORG_Y = CONVAS_ORG_PY7;
    S16 xo = ORG_X;
    S16 yo = ORG_Y;
    static U16 tdat, tdat1;
    // static U16 tlf;
    static S8 tdpf, tdlf;
    static S16 tpf, tdx, tdy, tpx, tpy;
    static U8 AutoDoSplit = 0;

    static U16 chRectTemp = 0; //����-ָ����Ļ���
    U8 patRulesTmp = PATRULES_MODIFY_ADD;
    /****��־λ��ز���end***/

    // const U8 keynum = {KEY_MODIADD_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},             //2 ����
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},            //3 ȷ��
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},             //4 ��С
        {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},             //5 �Ŵ�
        {TYPE_X4, TYPE_Y4, KEY_X1, KEY_Y1},             //6 ��ԭ��
        {TYPE_X5, TYPE_Y4, KEY_X1, KEY_Y1},             //7 ѹ������
        {TYPE_X6, TYPE_Y4, KEY_X1, KEY_Y1},             //8 ����
        {CONVAS_PX7, CONVAS_PY7, CONVAS_X7, CONVAS_Y7}, //9 ͼ���϶�

        {TYPE_X11, TYPE_Y7 + KEY_Y1 + SAFE_DIST1, KEY_X1, KEY_Y1},       //10 �綯+
        {TYPE_X11, TYPE_Y7 + (KEY_Y1 + SAFE_DIST1) * 2, KEY_X1, KEY_Y1}, //11 �綯-

        {TYPE_X11, TYPE_Y7, KEY_X1, KEY_Y1},                             //12 ����++
        {TYPE_X11, TYPE_Y7 + KEY_Y1 + SAFE_DIST1, KEY_X1, KEY_Y1},       //13 ����+
        {TYPE_X11, TYPE_Y7 + (KEY_Y1 + SAFE_DIST1) * 2, KEY_X1, KEY_Y1}, //14 ����-
        {TYPE_X11, TYPE_Y7 + (KEY_Y1 + SAFE_DIST1) * 3, KEY_X1, KEY_Y1}, //15 ����--

        {TYPE_X7, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6},          //16 �綯������
        {TYPE_X7 + KEY_X6, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6}, //17 �綯������
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},                                  //18 �������

        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1}, //19 �������2

        // {TYPE_X7, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6},              //20 ɾ��N��
        // {TYPE_X7 + KEY_X6, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6},     //21 ɾ��N��-���
        // {TYPE_X7 + KEY_X6 * 2, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6}, //22 ɾ��ȫ��

        // {TYPE_X7, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6},          //23 ѡ��N��
        // {TYPE_X7 + KEY_X6, TYPE_Y4 + (KEY_Y1 - KEY_Y6) / 2, KEY_X6, KEY_Y6}, //24 ѡ��ȫ��

    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {MODA_COM_ADDR, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {MODA_COM_ADDR + 1, 0, 0, 0}, {1}},                                         //1 ����
        {{KEY_Silver, KEY_Gray}, {0}, {72, 88}, {MODA_COM_ADDR + 2, MODA_COM_ADDR + 16, 0, 0}, {0}},         //2 ����
        {{KEY_Silver, KEY_Gray}, {0}, {71, 87}, {MODA_COM_ADDR + 3, MODA_COM_ADDR + 13, 0, 0}, {0}},         //3 ȷ��
        {{KEY_Silver, KEY_Gray}, {0}, {74, 0}, {MODA_COM_ADDR + 9, 0, 0, 0}, {0}},                           //4 ��С
        {{KEY_Silver, KEY_Gray}, {0}, {75, 0}, {MODA_COM_ADDR + 8, 0, 0, 0}, {0}},                           //5 �Ŵ�
        {{KEY_Silver, KEY_Gray}, {0}, {3, 4}, {MODA_COM_ADDR + 4, 0, 0, 0}, {0}},                            //6 ��ԭ��
        {{KEY_Silver, KEY_Gray}, {0}, {17, 19, 18, 20}, {MODA_COM_ADDR + 15, MODA_COM_ADDR + 5, 0, 0}, {0}}, //7 ѹ������
        {{KEY_Silver, KEY_Gray}, {0}, {37, 0}, {MODA_COM_ADDR + 20, 0, 0, 0}, {0}},                          //8 ����

        {{KEY_Silver}, {CONVAS_COLOR_T}, {CONVAS_COLOR_B}, {MODA_COM_ADDR + 14, 0, 0, 0}, {0}}, //24 �����ƶ�

        {{KEY_Silver, KEY_Gray}, {0}, {191, 192}, {MODA_COM_ADDR + 6, 0, 0, 0}, {0}}, //10 �綯+
        {{KEY_Silver, KEY_Gray}, {0}, {193, 194}, {MODA_COM_ADDR + 7, 0, 0, 0}, {0}}, //11 �綯-

        {{KEY_Silver, KEY_Gray}, {0}, {207, 208}, {MODA_COM_ADDR + 17, 0, 0, 0}, {0}}, //12 ����++
        {{KEY_Silver, KEY_Gray}, {0}, {191, 192}, {MODA_COM_ADDR + 6, 0, 0, 0}, {0}},  //13 ����+
        {{KEY_Silver, KEY_Gray}, {0}, {193, 194}, {MODA_COM_ADDR + 7, 0, 0, 0}, {0}},  //14 ����-
        {{KEY_Silver, KEY_Gray}, {0}, {209, 210}, {MODA_COM_ADDR + 18, 0, 0, 0}, {0}}, //15 ����--

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {515, 0, 516}, {MODA_COM_ADDR + 11, 0, 0, 0}, {0}}, //16 �綯������
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {517, 0, 518}, {MODA_COM_ADDR + 10, 0, 0, 0}, {0}}, //17 �綯������
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {519, 0, 520}, {MODA_COM_ADDR + 12, 0, 0, 0}, {0}}, //18 �������

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {519, 0, 520}, {MODA_COM_ADDR + 12, 0, 0, 0}, {0}}, //19 �������2

        // {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {555, 0, 556}, {MODA_COM_ADDR + 22, 0, 0, 0}, {0}}, //20 ɾ��N��
        // {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {557, 0, 558}, {MODA_COM_ADDR + 23, 0, 0, 0}, {0}}, //21 ɾ��N��-���
        // {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {559, 0, 560}, {MODA_COM_ADDR + 24, 0, 0, 0}, {0}}, //22 ɾ��ȫ��

        // {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {577, 0, 578}, {MODA_COM_ADDR + 25, 0, 0, 0}, {0}}, //23 ѡ��N��
        // {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {579, 0, 580}, {MODA_COM_ADDR + 26, 0, 0, 0}, {0}}, //24 ѡ��ȫ��

    };

    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {MODA_COM_ADDR}},
        {TYPE_X35, 65, TYPE_SIZE17, TYPE_SIZE17, {Color_White, Color_Black}, {Color_Black, Color_Blue}, {21, 18}},  //1 ѡ�����1
        {TYPE_X35, 245, TYPE_SIZE17, TYPE_SIZE17, {Color_White, Color_Black}, {Color_Black, Color_Blue}, {21, 19}}, //2 ѡ�����2
        // {TYPE_X7 - SAFE_DIST4, TYPE_Y4, KEY_X6 * 2 + SAFE_DIST1, KEY_Y1, {Color_Gray}, {Color_Black}, {0}},         //3 ����ѡ��
        // {TYPE_X7 - SAFE_DIST4, TYPE_Y4, KEY_X6 * 3 + SAFE_DIST1, KEY_Y1, {Color_Gray}, {Color_Black}, {0}},         //4 ����ѡ��2

        {TYPE_X35, 65 + TYPE_SIZE17 + SAFE_DIST4, TYPE_SIZE28, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {26}},                                                         //5 �����-����
        {TYPE_X35 + TYPE_SIZE14 + SAFE_DIST5, 65 + TYPE_SIZE17 + SAFE_DIST4, TYPE_SIZE21, TYPE_SIZE5, {Color_DeepSkyBlue, Color_Black}, {Color_Black, Color_Black}, {0}}, //6 �����-����
    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MODIFYADD); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(patRulesTmp);

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    pfootf = (U8)(S_PFOOT);
    if (pfootf == 0x10)
    { //����
        Up = 1;
        Dn = 0;
    }
    else
    {
        Up = 0;
        Dn = 1;
    }

    pfootf = 0xff;

    if (F_FILETYPE == PAT_SEW)
    {
        Read_xyposi();
        if ((g_xabspos == 0) && (g_yabspos == 0) && (g_jog_point == 0))
            Stflag = 0; //��еԭ��
        else if (g_jog_point == 0)
            Stflag = 1; //��ԭ��
    }

    //��ȡ��ǰ������ѹ�Ż�׼ֵ
    f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));
    if (f == 0)
    {
        pno = MENU_RWHELP;
        hleppara = 15;
        return;
    }
    g_pfootd.base = YREG[SYS_PFOOTSET];
    if ((g_pfootd.base > (PF_ABS_MAX - g_patd.PFmax)) || (g_pfootd.base < PF_ABS_MIN - g_patd.PFmin))
        g_pfootd.base = PF_ABS_MIN - g_patd.PFmin;

    f = Read_D(D_Address[SYS] + SYS_PFOOTSET, 1, (U16 *)(&YREG[SYS_PFOOTSET]));

    if (f == 0)
    { //���趨��ѹ��������
        pno = MENU_RWHELP;
        hleppara = 15;
        return;
    }
    PfootOffVal = YREG[SYS_PFOOTSET];

    f = Read_D(D_Address[SYS] + SYS_PFOOTNOW, 1, (U16 *)(&YREG[SYS_PFOOTNOW])); ///////////////////remove 2010.7.5

    if (f == 0)
    { //���趨��ѹ��������
        pno = MENU_RWHELP;
        hleppara = 15;
        return;
    }
    temp_PfVal = YREG[SYS_PFOOTNOW];
    old_PfootOffVal = temp_PfVal;

    f = Read_D(D_Address[SYS] + SYS_LFORCESET, 1, (U16 *)(&YREG[SYS_LFORCESET]));
    if (f == 0)
    { //������������׼ֵ
        pno = MENU_RWHELP;
        hleppara = 15;
        return;
    }
    g_lineforceValSet = YREG[SYS_LFORCESET];

    if (F_FILETYPE == PAT_MIT)
    { //����
        if ((para == MODIFY_PART_CHANGE) && (g_selectmode == 3))
        {
        }
        else
        {
            AutoDoSplit = 0;
        }

        lforceval = Mit_CailPadOffestLF(g_jog_point, 0);
        g_lineforceValSetNow = g_lineforceValSet + lforceval;

        if ((g_jog_stpoint > g_patd.feed_sum) || (g_jog_enpoint > g_patd.feed_sum))
        {
            g_jog_stpoint = 0;
            g_jog_enpoint = 0;
        }

        if (g_jog_point > g_patd.feed_sum)
        {
            g_jog_point = g_patd.feed_sum;
            if (g_jog_point > 0)
            {
                g_temp_oxpos = 0;
                g_temp_oypos = 0;
#if PATRULES_ORG_OFFSET
                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                {
                    g_temp_oxpos = (S16)(SREG[6]);
                    g_temp_oxpos = (S16)(SREG[7]);
                }
#endif
                dat1 = g_patd.pat_add;
                for (ii = 0; ii < g_jog_point; ii++)
                {
                    dat = SREG[dat1] & 0x00ff;
                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                    {
                        dat = SREG[dat1 + 1];
                        x = (U8)(dat);
                        y = (U8)((U16)(dat >> 8));
                        if (x >= 0x80)
                        {
                            x -= 0x80;
                            g_temp_oxpos -= (S16)(x);
                        }
                        else
                        {
                            g_temp_oxpos += (S16)(x);
                        }
                        if (y >= 0x80)
                        {
                            y -= 0x80;
                            g_temp_oypos -= (S16)(y);
                        }
                        else
                        {
                            g_temp_oypos += (S16)(y);
                        }
                    }
                    dat1 += 2;
                }
                DREG[D_COMMAND] = C_ABSLTNSSMOVE;
                DREG[D_CMDSTATUS] = 50;
                DREG[D_CMDPARA1] = g_temp_oxpos;
                DREG[D_CMDPARA2] = g_temp_oypos;
                Write_D(D_Address[DISP] + D_COMMAND, 4, (U16 *)(&DREG[D_COMMAND])); // ����ָ��
                g_modcordflag = 0;
            }
        }
        tmp_feedsum = g_patd.feed_sum;
        if ((g_sta_end_flag == 4) && (para == MODIFY_PART_CHANGE))
        {
            max_feedsum = g_jog_enpoint;
            if ((g_jogmode == 0) && (g_point_num > 1))
                min_feedsum = g_array5[299 + g_point_num][0];
            else
                min_feedsum = g_jog_stpoint;
        }
        else
        {
            max_feedsum = tmp_feedsum;
            if (((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE)) && (g_sta_end_flag == 1))
            {
                min_feedsum = g_jog_stpoint;
                g_jog_enpoint = g_jog_stpoint; //����
            }
            else
            {
                min_feedsum = 0;
                g_jog_enpoint = 0; //����
            }
        }

        if ((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE))
        {

            if ((g_sta_end_flag == 4) && (para == MODIFY_PART_CHANGE))
            {
                chRectTemp = RECT_MODIADD_PIN1;

                for (i = RECT_MODIADD_IN_TIT; i <= RECT_MODIADD_IN_STR; i++)
                {

                    DrawFillRect(rect[i].xpis, rect[i].ypis,
                                 rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    if (i == RECT_MODIADD_IN_TIT)
                    {
                        Draw24x24Str(rect[i].xpis + (rect[i].xsize - 13 * strlen((char *)((MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0])))) / 2,
                                     rect[i].ypis + (rect[i].ysize - 24) / 2,
                                     (MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0]),
                                     SheetPage, rect[i].index[0], rect[i].colorT[0]); // �����
                    }
                    else
                    {
                        DrawDottedRect(rect[i].xpis, rect[i].ypis,
                                       rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
                        DrawMFS32P(rect[i].xpis + (rect[i].xsize - 13 * 2) / 2,
                                   rect[i].ypis + (rect[i].ysize - 24) / 2, g_point_num, 2, 0, rect[i].colorT[0]);
                    }
                }
                DrawButton(keyd[KEY_MODIADD_INPUTEND], (U16 *)(ADD_ICONBMP),
                           keybmpind[KEY_MODIADD_INPUTEND].border[0], keybmpind[KEY_MODIADD_INPUTEND].icon[0]);
            }
            else if (g_sta_end_flag == 0)
            {
                chRectTemp = RECT_MODIADD_PIN1;
            }
            else
            {
                chRectTemp = RECT_MODIADD_PIN2;

#if 0
                Draw24X24FStr(vxp2, vyp2 + 60, "X: ", Color_Gray);
                Draw24X24FStr(vxp2 + 155, vyp2 + 60, "Y: ", Color_Gray);

                Draw24X24FStr(vxp2, vyp2 + 88, "XX:", Color_Gray);
                Draw24X24FStr(vxp2 + 155, vyp2 + 88, "XY:", Color_Gray);

                Draw24x24Str(vxp2, vyp2, (MEM_TITLE_ADD + TitleLengthMax * 21), SheetPage, 21, Color_Gray);       //�ƶ���
                Draw24x24Str(vxp2 + 300, vyp2, (MEM_TITLE_ADD + TitleLengthMax * 23), SheetPage, 23, Color_Gray); //����
                DrawRect(vxp2 + 404, vyp2 - 4, 69, 30, Color_Gray);
                Draw24X24FStr(vxp2 + 300, vyp2 + 60, "PF:", Color_Gray);
                Draw24X24FStr(vxp2 + 448, vyp2 + 60, "LF:", Color_Gray);

                Draw24x24Str(vxp2, vyp2 + 28, (MEM_TITLE_ADD + TitleLengthMax * 24), SheetPage, 24, Color_Gray);       //����
                Draw24x24Str(vxp2 + 300, vyp2 + 28, (MEM_TITLE_ADD + TitleLengthMax * 25), SheetPage, 25, Color_Gray); //�ٶ�

                Draw24X24FStr(vxp2 + 300, vyp2 + 88, "XPF:", Color_Gray);
                Draw24X24FStr(vxp2 + 448, vyp2 + 88, "XLF:", Color_Gray);

                Draw24X24FStr(vxp2 + 477, vyp2, "/", Color_Gray);
                DrawMFU32(vxp2 + 490, vyp2, max_feedsum, 5, Color_Gray);

                disStatus(vxp2, vyp2, tdat, tdat1, tdlf, tdpf, Color_Gray);

                DrawFillRect(vxp2 + 406, vyp2, 65, 24, mCOLOR ? Color_Silver : Color_White);
                DrawMFU32(vxp2 + 406, vyp2, g_jog_stpoint, 5, Color_Gray);

                DrawFillRect(vxp2 + 43, vyp2 + 60, 80, 24, mCOLOR ? Color_Silver : Color_White);
                DrawMFS32P(vxp2 + 43, vyp2 + 60, tdx / 2, 5, 1, Color_Gray);

                DrawFillRect(vxp2 + 194, vyp2 + 60, 80, 24, mCOLOR ? Color_Silver : Color_White);
                DrawMFS32P(vxp2 + 194, vyp2 + 60, tdy / 2, 5, 1, Color_Gray);

                DrawFillRect(vxp2 + 504, vyp2 + 88, 60, 24, mCOLOR ? Color_Silver : Color_White);
                DrawMFS32P(vxp2 + 504, vyp2 + 88, tlf, 3, 0, Color_Gray);

                DrawMFS32P(vxp1 + 43, vyp1, tpx, 5, 1, Color_Gray);
                DrawMFS32P(vxp1 + 194, vyp1, tpy, 5, 1, Color_Gray);
                DrawMFS32P(vxp2 + 369, vyp2 + 88, tpf, 4, 1, Color_Gray);

                DrawHLine(0, vyp2 + 124, LCD_width, Color_Black);
                DrawHLine(0, vyp2 + 125, LCD_width, Color_Black);
#else
                disStatusTit(rect[RECT_MODIADD_PIN1], rect[RECT_MODIADD_PIN1].index[1], 1);
                disStatus(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, tdat, tdat1, tdlf, tdpf, rect[RECT_MODIADD_PIN1].colorT[0]);
                disStatusFeedSum(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, max_feedsum, rect[RECT_MODIADD_PIN1].colorT[0]);
                disStatusFeedNow(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, g_jog_stpoint, rect[RECT_MODIADD_PIN1].colorT[0]);
                disStatusUpdate(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, tdx, tdy, tpx, tpy, tpf);
#endif
                // vyp1 += 140;
                // vyp2 += 140;
                if ((para == MODIFY_PART_CHANGE) && (g_selectmode > 0) && (g_selectmode < 6) && (g_selectmode != 4))
                    dis_updata1 = 1;

                if (g_jog_stpoint < tmp_feedsum)
                // if (g_jog_enpoint < tmp_feedsum)
                {
                    // g_jog_point = g_jog_enpoint;
                    dat1 = g_patd.pat_add + g_jog_stpoint * 2;
                    dat = SREG[dat1] & 0x00ff;
                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                    {
                        dat = SREG[dat1 + 1];
                        x = (U8)(dat);
                        y = (U8)((U16)(dat >> 8));
                        if (x >= 0x80)
                        {
                            x -= 0x80;
                            g_dxpos = -x;
                        }
                        else
                        {
                            g_dxpos = x;
                        }
                        if (y >= 0x80)
                        {
                            y -= 0x80;
                            g_dypos = -y;
                        }
                        else
                        {
                            g_dypos = y;
                        }
                        vdat = SREG[dat1 + 1];
                        dat = SREG[dat1];
                    }
                    else
                    {
                        g_dxpos = 0;
                        g_dypos = 0;
                        vdat = SREG[dat1 + 1];
                        dat = SREG[dat1];
                    }
                }
                else
                {
                    g_dxpos = 0;
                    g_dypos = 0;
                    if (tmp_feedsum)
                        dat = 0xff;
                    else
                        dat = 0x1b;
                }
                // disStatus(vxp2, vyp2, dat, vdat, dlforceval, 0, Color_Black);
                disStatusTit(rect[RECT_MODIADD_PIN2], rect[RECT_MODIADD_PIN2].index[1], 1);
                disStatus(rect[RECT_MODIADD_PIN2].xpis, rect[RECT_MODIADD_PIN2].ypis, dat, vdat, dlforceval, 0, rect[RECT_MODIADD_PIN2].colorT[1]);
            }
        }
        else
        {
            g_sta_end_flag = 0;

            chRectTemp = RECT_MODIADD_PIN1;
        }

        g_temp_oxpos = 0;
        g_temp_oypos = 0;
#if PATRULES_ORG_OFFSET
        if (SREG[3] == 7) //����-�޸�ԭ��λ��
        {
            g_temp_oxpos = (S16)(SREG[6]);
            g_temp_oypos = (S16)(SREG[7]);
        }
#endif
        dat1 = g_patd.pat_add + 1;
        if (g_jog_point > 0)
        {
            for (ii = 0; ii < g_jog_point; ii++)
            {
                dat = SREG[dat1 - 1] & 0x00ff;
                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                {
                    dat = SREG[dat1];
                    x = (U8)(dat);
                    y = (U8)((U16)(dat >> 8));
                    if (x >= 0x80)
                    {
                        x -= 0x80;
                        g_temp_oxpos -= (S16)(x);
                    }
                    else
                    {
                        g_temp_oxpos += (S16)(x);
                    }
                    if (y >= 0x80)
                    {
                        y -= 0x80;
                        g_temp_oypos -= (S16)(y);
                    }
                    else
                    {
                        g_temp_oypos += (S16)(y);
                    }
                }
                dat1 += 2;
            }
        }

        if (g_jog_point < tmp_feedsum)
        {
            dat = SREG[dat1 - 1] & 0x00ff;
            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
            {
                dat = SREG[dat1];
                x = (U8)(dat);
                y = (U8)((U16)(dat >> 8));
                if (x >= 0x80)
                {
                    x -= 0x80;
                    g_dxpos = -x;
                }
                else
                {
                    g_dxpos = x;
                }
                if (y >= 0x80)
                {
                    y -= 0x80;
                    g_dypos = -y;
                }
                else
                {
                    g_dypos = y;
                }
                dat = SREG[dat1 - 1];
                vdat = SREG[dat1];
            }
            else
            {
                g_dxpos = 0;
                g_dypos = 0;
                dat = SREG[dat1 - 1];
                vdat = SREG[dat1];
            }
        }
        else
        {
            g_dxpos = 0;
            g_dypos = 0;
            if (tmp_feedsum)
                dat = 0x1c;
            else
                dat = 0x1b;
        }

        g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 0);
        g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 1);
        g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 2);

        sendExCodeCmd();
    }
    else if (F_FILETYPE == PAT_SEW)
    { //�ֵ�
        AutoDoSplit = 0;
        Bor_CalLFval((U16 *)SREG, g_patd.pat_len, g_jog_point, (U8 *)&g_lineforceValSetNow, &dlforceval);
        lfnow = g_lineforceValSetNow;
        if ((g_jog_stpoint > g_patd.feed_sum) || (g_jog_enpoint > g_patd.feed_sum))
        {
            g_jog_stpoint = 0;
            g_jog_enpoint = 0;
        }
        if (g_jog_point > g_patd.feed_sum)
        {
            g_jog_point = g_patd.feed_sum;
            if (g_jog_point > 0)
            {
                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                Comm_Move(temp_oxpos1, temp_oypos1, 50, MENU_MODIFYADD, 1, 1);
                g_modcordflag = 0;
            }
        }
        tmp_feedsum = g_patd.feed_sum;
        min_feedsum = 0;

        if ((para == MODIFY_PIN_DEL) || (para == MODIFY_PIN_ADD) || (para == MODIFY_PIN_MOVE))
        { //��ɾ������׷�ӡ����޸�
            max_feedsum = tmp_feedsum;
            g_sta_end_flag = 0;
            chRectTemp = RECT_MODIADD_PIN1;
        }
        else if ((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE))
        { //�����ƶ��� ��������
            if ((g_sta_end_flag == 4) && (para == MODIFY_PART_CHANGE))
            {
                max_feedsum = g_jog_enpoint;
                if ((g_jogmode == 0) && (g_point_num > 1))
                    min_feedsum = g_array5[299 + g_point_num][0];
                else
                    min_feedsum = g_jog_stpoint;

                chRectTemp = RECT_MODIADD_PIN1;
                for (i = RECT_MODIADD_IN_TIT; i <= RECT_MODIADD_IN_STR; i++)
                {

                    DrawFillRect(rect[i].xpis, rect[i].ypis,
                                 rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    if (i == RECT_MODIADD_IN_TIT)
                    {
                        Draw24x24Str(rect[i].xpis + (rect[i].xsize - 13 * strlen((char *)((MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0])))) / 2,
                                     rect[i].ypis + (rect[i].ysize - 24) / 2,
                                     (MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0]),
                                     SheetPage, rect[i].index[0], rect[i].colorT[0]); // �����
                    }
                    else
                    {
                        DrawDottedRect(rect[i].xpis, rect[i].ypis,
                                       rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
                        DrawMFS32P(rect[i].xpis + (rect[i].xsize - 13 * 2) / 2,
                                   rect[i].ypis + (rect[i].ysize - 24) / 2, g_point_num, 2, 0, rect[i].colorT[0]);
                    }
                }
                DrawButton(keyd[KEY_MODIADD_INPUTEND], (U16 *)(ADD_ICONBMP),
                           keybmpind[KEY_MODIADD_INPUTEND].border[0], keybmpind[KEY_MODIADD_INPUTEND].icon[0]);
            }
            else if (g_sta_end_flag == 0)
            {
                max_feedsum = tmp_feedsum;
                chRectTemp = RECT_MODIADD_PIN1;
            }
            else
            {
                max_feedsum = tmp_feedsum;
                min_feedsum = g_jog_stpoint;
                g_jog_enpoint = g_jog_stpoint; //����

                chRectTemp = RECT_MODIADD_PIN2;

#if 0
                Draw24X24FStr(vxp2, vyp2 + 60, "X: ", Color_Gray);
                Draw24X24FStr(vxp2 + 155, vyp2 + 60, "Y: ", Color_Gray);

                Draw24X24FStr(vxp2, vyp2 + 88, "XX:", Color_Gray);
                Draw24X24FStr(vxp2 + 155, vyp2 + 88, "XY:", Color_Gray);

                Draw24x24Str(vxp2, vyp2, (MEM_TITLE_ADD + TitleLengthMax * 21), SheetPage, 21, Color_Gray);       //�ƶ���
                Draw24x24Str(vxp2 + 300, vyp2, (MEM_TITLE_ADD + TitleLengthMax * 23), SheetPage, 23, Color_Gray); //����
                DrawRect(vxp2 + 404, vyp2 - 4, 69, 30, Color_Gray);
                Draw24X24FStr(vxp2 + 300, vyp2 + 60, "PF:", Color_Gray);
                Draw24X24FStr(vxp2 + 448, vyp2 + 60, "LF:", Color_Gray);

                Draw24x24Str(vxp2, vyp2 + 28, (MEM_TITLE_ADD + TitleLengthMax * 24), SheetPage, 24, Color_Gray);       //����
                Draw24x24Str(vxp2 + 300, vyp2 + 28, (MEM_TITLE_ADD + TitleLengthMax * 25), SheetPage, 25, Color_Gray); //�ٶ�

                Draw24X24FStr(vxp2 + 300, vyp2 + 88, "XPF:", Color_Gray);
                Draw24X24FStr(vxp2 + 448, vyp2 + 88, "XLF:", Color_Gray);

                Draw24X24FStr(vxp2 + 477, vyp2, "/", Color_Gray);
                DrawMFU32(vxp2 + 490, vyp2, max_feedsum, 5, Color_Gray);

                disStatus(vxp2, vyp2, tdat, tdat1, tdlf, tdpf, Color_Gray);

                //for(i=0;i<2;i++){
                //	DrawHLine(0,vyp2+124+i, LCD_width, Color_Black);
                //	DrawFillRect(vxp2+43+151*i,vyp2+60,80,24,mCOLOR?Color_Silver:Color_White);
                //}

                DrawFillRect(vxp2 + 406, vyp2, 65, 24, mCOLOR ? Color_Silver : Color_White);
                DrawMFU32(vxp2 + 406, vyp2, g_jog_stpoint, 5, Color_Gray);

                DrawFillRect(vxp2 + 43, vyp2 + 60, 93, 24, mCOLOR ? Color_Silver : Color_White);
#if MOVEUNIT == 1
                DrawMFS32P(vxp2 + 43, vyp2 + 60, tdx * 5, 6, 2, Color_Gray);
#else
                DrawMFS32P(vxp2 + 43, vyp2 + 60, tdx / 2, 5, 1, Color_Gray);
#endif

                DrawFillRect(vxp2 + 194, vyp2 + 60, 93, 24, mCOLOR ? Color_Silver : Color_White);
#if MOVEUNIT == 1
                DrawMFS32P(vxp2 + 194, vyp2 + 60, tdy * 5, 5, 2, Color_Gray);
#else
                DrawMFS32P(vxp2 + 194, vyp2 + 60, tdy / 2, 5, 1, Color_Gray);
#endif

                DrawFillRect(vxp2 + 504, vyp2 + 88, 60, 24, mCOLOR ? Color_Silver : Color_White);
                DrawMFS32P(vxp2 + 504, vyp2 + 88, tlf, 3, 0, Color_Gray);

#if MOVEUNIT == 1
                DrawMFS32P(vxp1 + 43, vyp1, tpx * 5, 6, 2, Color_Gray);
                DrawMFS32P(vxp1 + 194, vyp1, tpy * 5, 6, 2, Color_Gray);
#else
                DrawMFS32P(vxp1 + 43, vyp1, tpx, 5, 1, Color_Gray);
                DrawMFS32P(vxp1 + 194, vyp1, tpy, 5, 1, Color_Gray);
#endif
                DrawMFS32P(vxp2 + 369, vyp2 + 88, tpf, 4, 1, Color_Gray);

                DrawHLine(0, vyp2 + 124, LCD_width, Color_Black);
                DrawHLine(0, vyp2 + 125, LCD_width, Color_Black);
#else

                disStatusTit(rect[RECT_MODIADD_PIN1], rect[RECT_MODIADD_PIN1].index[1], 1);
                disStatus(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, tdat, tdat1, tdlf, tdpf, rect[RECT_MODIADD_PIN1].colorT[0]);
                disStatusFeedSum(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, max_feedsum, rect[RECT_MODIADD_PIN1].colorT[0]);
                disStatusFeedNow(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, g_jog_stpoint, rect[RECT_MODIADD_PIN1].colorT[0]);
                disStatusUpdate(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis, tdx, tdy, tpx, tpy, tpf);
#endif

                if ((para == MODIFY_PART_CHANGE) && (g_selectmode > 0) && (g_selectmode < 6) && (g_selectmode != 4))
                    dis_updata1 = 1;

                Bor_feedInfo(g_jog_stpoint + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, Borpfdis, Color_Black);
                disStatusTit(rect[RECT_MODIADD_PIN2], rect[RECT_MODIADD_PIN2].index[1], 1);
                disStatus(rect[RECT_MODIADD_PIN2].xpis, rect[RECT_MODIADD_PIN2].ypis, dat, vdat, dlforceval, 0, rect[RECT_MODIADD_PIN2].colorT[1]);

                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                dat = Borcmd;
            }
        }
        else if ((para == MODIFY_PITCH) || (para == MODIFY_PF) || (para == MODIFY_LF) || (para == MODIFY_PART_ADD) || (para == MODIFY_SPEED) || (para == MODIFY_CODE_DEL) || (para == MODIFY_CODE_ADD) || //�޸Ļ��棨����޸ġ���ѹ�š����������β�/�ٶ��޸ġ���������ɾ�����������ܲ��룩
                 (para == CONVERT_TACKING) || (para == CONVERT_ZOOM) || (para == CONVERT_ROTATE) || (para == CONVERT_CHANGE) || (para == CONVERT_PINANG))
        { //�任���棨���졢�Ŵ���С����ת,��β����,��Ƕȣ�
            max_feedsum = tmp_feedsum;
            g_sta_end_flag = 0;
            chRectTemp = RECT_MODIADD_PIN1;
        }

        g_temp_oxpos = 0;
        g_temp_oypos = 0;
#if PATRULES_ORG_OFFSET
        if (SREG[3] == 7) //����-�޸�ԭ��λ��
        {
            g_temp_oxpos = (S16)(SREG[6]);
            g_temp_oxpos = (S16)(SREG[7]);
        }
#endif
        if (Stflag == 0)
        { //�ڻ�еԭ��λ��
            Borcmd = 0x2000;
            g_dxpos = g_patd.orgX;
            g_dypos = g_patd.orgY;
        }
        else if (Stflag == 1)
        {
            Borcmd = 0x5050; //��ԭ��
            g_dxpos = 0;
            g_dxpos = 0;
        }
        else if (g_jog_point < tmp_feedsum)
        {
            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, (S16 *)&temp_oxpos1, (S16 *)&temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
            Borpooft += g_pfootd.base;
        }
        else
        {
            g_dxpos = 0;
            g_dxpos = 0;
        }
        dat = Borcmd;
        g_BorSTflag = Stflag;
        Bor_feedInfo(g_jog_point, g_patd.pat_len, &tBorcmd, &ttemp_oxpos1, &ttemp_oypos1, (S16 *)&tdxpos, (S16 *)&tdypos, (U8 *)&toldspeed, &tBorpooft, &tBorpfdis);
        sendExCodeCmd();
    }

    if (g_jog_point == 0)
    {
        EnUPflag = 1;
    }
    tmuly = (F_FILETYPE == PAT_SEW) ? -1 : 1;

    tscale = pat_getScale(patRule.h_patConvasIndex, patRulesTmp);

    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                 SheetPage, rect[0].index[0], rect[0].colorT[0]); //ѡ������

    //����������ʾ
    // if ((para == MODIFY_PIN_DEL))
    // {
    //     DrawFillRect(rect[RECT_MODIADD_METHOD2].xpis, rect[RECT_MODIADD_METHOD2].ypis,
    //                  rect[RECT_MODIADD_METHOD2].xsize, rect[RECT_MODIADD_METHOD2].ysize, rect[RECT_MODIADD_METHOD2].colorB[0]);
    // }
    // else if ((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE) || (para == MODIFY_PITCH) || (para == MODIFY_SPEED))
    // {
    //     DrawFillRect(rect[RECT_MODIADD_METHOD1].xpis, rect[RECT_MODIADD_METHOD1].ypis,
    //                  rect[RECT_MODIADD_METHOD1].xsize, rect[RECT_MODIADD_METHOD1].ysize, rect[RECT_MODIADD_METHOD1].colorB[0]);
    // }

    //����������ʾ
    for (i = 2; i <= KEY_MODIADD_MDOWN; i++)
    {
        if (i == KEY_MODIADD_IMG)
        {
            DrawFillRect(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize, keybmpind[i].icon[0]);
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

    ViewPage(usepageno); //������ʾ����
    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_MAX1, 0); //��ȡ��ֵ
        if (keydata == 0)
        {
            if (g_jogonflag == 0)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1, KEY_MODIADD_MAX1);
            }
            else
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_MAX3 + KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1, KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1);
            }
        }

        if (keydata == 0)
        {
            // if ((para == MODIFY_PIN_DEL))
            // {
            //     keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_MAX5 + KEY_MODIADD_MAX4 + KEY_MODIADD_MAX3 + KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1,
            //                           KEY_MODIADD_MAX4 + KEY_MODIADD_MAX3 + KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1);
            // }
            // else if ((para == MODIFY_PITCH) || (para == MODIFY_SPEED))
            // {
            //     keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_MAX6 + KEY_MODIADD_MAX5 + KEY_MODIADD_MAX4 + KEY_MODIADD_MAX3 + KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1,
            //                           KEY_MODIADD_MAX5 + KEY_MODIADD_MAX4 + KEY_MODIADD_MAX3 + KEY_MODIADD_MAX2 + KEY_MODIADD_MAX1);
            // }
            // else
            if ((para == MODIFY_PART_CHANGE))
            {
                if ((g_selectmode > 0) && (g_selectmode < 6) && (g_selectmode != 4))
                {
                    keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_JOGOFF + 1,
                                          KEY_MODIADD_JOGON);
                }
                if ((g_sta_end_flag == 4) && (keydata == 0))
                {
                    keydata = get_keydata((KEYPAD *)keyd, KEY_MODIADD_AUTO + 1,
                                          KEY_MODIADD_INPUTEND);
                }
            }
        }

        if ((keydata == KEY_MODIADD_INPUTEND) && (g_sta_end_flag != 4))
        {
            if ((F_FILETYPE == PAT_MIT) && (g_selectmode == 3))
            {
                keydata = KEY_MODIADD_AUTO;
            }
        }

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon >= KEY_MODIADD_MUP) && (keyon <= KEY_MODIADD_MDOWN))
            {
            }
            else if ((keyon >= KEY_MODIADD_JOGUP) && (keyon <= KEY_MODIADD_JOGDOWN))
            {
            }
            else if (keyon == KEY_MODIADD_IMG)
            {
            }
            else if ((keyon >= KEY_MODIADD_IMGO) && (keyon <= KEY_MODIADD_IMGI))
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if (((keydata == KEY_MODIADD_JOGON) && (dis_updata1 == 0)) ||
                ((keydata == KEY_MODIADD_JOGOFF) && (dis_updata1 == 0)) ||
                ((keydata == KEY_MODIADD_INPUTEND) && (g_sta_end_flag != 4)))
            {
                keydata = 0;
            }
            else
            {
                if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
                {
                    keyon = keydata;
                    if (keyon == KEY_MODIADD_SCRNSVR || keyon == KEY_MODIADD_IMG) //�ް�����Ч
                    {
                    }
                    //�ǳ��水��-//������ɫ���ְ���
                    else
                    {
                        DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                    }
                    Times = 0;
                    delay = KEYDT; //������������
                }
                else if (keyon != keydata) //���а������£��������°�����Ϣ
                {
                    keydata = 0;
                }

                if (Times > delay) //��������
                {
                    if (beeponf == 0) //��������־λ //��һ�ν���
                    {
                        beeponf = 1;

                        if (g_keyBellT) //����ʱ����������ʱ��
                            Beep_On(g_keyBellT);

                        switch (keyon) //�������±�����ʾ����ʾ
                        {
                        case KEY_MODIADD_FOOT:
                            DrawFMenuStr((pfootf == 0) ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
                            break;
                        default:
                            DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                            break;
                        }
                        delay = KEYFT;
                    }
                    else if (beeponf == 1) //�ڶ��ν���
                    {
                        beeponf = 2;
                        delay = KEYNT;
                        if ((keyon >= KEY_MODIADD_MUP) && (keyon <= KEY_MODIADD_MDOWN))
                        {
                            delay = KEYMT;
                        }
                        else if ((keyon >= KEY_MODIADD_JOGUP) && (keyon <= KEY_MODIADD_JOGDOWN))
                        {
                            delay = KEYMT;
                        }
                    }
                    else if (beeponf < 20)
                    {
                        beeponf++;
                    }
                    Times = 0; //��ǰ
                    if ((keyon >= KEY_MODIADD_MUP) && (keyon <= KEY_MODIADD_MDOWN))
                    {
                        keyoff = keyon;
                    }
                    else if ((keyon >= KEY_MODIADD_JOGUP) && (keyon <= KEY_MODIADD_JOGDOWN))
                    {
                        keyoff = keyon;
                    }
                    else if (keyon == KEY_MODIADD_IMG)
                    {
                        keyoff = keyon;
                    }
                    else if ((keyon >= KEY_MODIADD_IMGO) && (keyon <= KEY_MODIADD_IMGI))
                    {
                        keyoff = keyon;
                    }
                }
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            if (g_needtohome && (keyoff != KEY_MODIADD_ORG) && (keyoff != KEY_MODIADD_JOGON) && (keyoff != KEY_MODIADD_JOGOFF))
            {
                pno = MENU_RWHELP;
                hleppara = 158;
                poldno[pnoindex] = MENU_MODIFYADD;
                pnoindex++;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_MODIADD_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_MODIADD_BACK:
                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        if ((para == MODIFY_PART_CHANGE) && (g_sta_end_flag == 4))
                        {
                            if ((!(S_CLAMP)) && (P_OST == 0))
                            {
                                pno = MENU_RWHELP;
                                hleppara = 13;
                                poldno[pnoindex++] = MENU_MODIFYADD;
                                break;
                            }

                            if (g_point_num > 1)
                            {
                                g_point_num--;
                                g_tmpoxpos = 0;
                                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_tmpoxpos = (S16)(SREG[6]);
                                    g_tmpoypos = (S16)(SREG[7]);
                                }
#endif
                                g_jog_point = g_array5[300 + g_point_num][0];
                                for (ii = 0; ii < g_jog_point; ii++)
                                {
                                    dat = SREG[g_patd.pat_add + ii * 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[g_patd.pat_add + ii * 2 + 1];
                                        dat &= 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoxpos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoxpos += dat;
                                        }
                                        dat = SREG[g_patd.pat_add + ii * 2 + 1];
                                        dat = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoypos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoypos += dat;
                                        }
                                    }
                                }

                                g_dxpos = (S16)(g_array5[g_point_num][0]);
                                g_dypos = (S16)(g_array5[g_point_num][1]);
                                Comm_Move(g_tmpoxpos + g_dxpos, g_tmpoypos + g_dypos, 50, MENU_MODIFYADD, 0, 0);
                                pno = MENU_MODIFY1;
                                pnoindex--;
                            }
                            else
                            {
                                g_sta_end_flag = 1;
                                chRectTemp = RECT_MODIADD_PIN2;
                                g_jog_point = g_jog_stpoint;
                                if (g_jog_enpoint > g_jog_stpoint)
                                    g_jog_enpoint = g_jog_stpoint;
                                g_tmpoxpos = 0;
                                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_tmpoxpos = (S16)(SREG[6]);
                                    g_tmpoypos = (S16)(SREG[7]);
                                }
#endif
                                for (ii = 0; ii < g_jog_point; ii++)
                                {
                                    dat = SREG[g_patd.pat_add + ii * 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[g_patd.pat_add + ii * 2 + 1];
                                        dat &= 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoxpos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoxpos += dat;
                                        }
                                        dat = SREG[g_patd.pat_add + ii * 2 + 1];
                                        dat = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoypos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoypos += dat;
                                        }
                                    }
                                    else if (dat == 0x3c)
                                    {
                                        g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                                    }
                                    else if (dat == 0x3d)
                                    {
                                        g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                                    }
                                    else if (dat == 0x3e)
                                    {
                                        g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                                    }
                                }
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFYADD, 0, 0);
                                pno = MENU_MODIFYADD;
                                g_temp_pno = 0xffffffff;

                                sendExCodeCmd();
                                goto loop1;
                            }
                        }
                        else
                        {
                            if (((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE)) && (g_sta_end_flag == 1))
                            { //����ѡ����ʼ��λ��
                                if ((!(S_CLAMP)) && (P_OST == 0))
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 13;
                                    poldno[pnoindex++] = MENU_MODIFYADD;
                                    break;
                                }

                                min_feedsum = 0;
                                g_jog_enpoint = 0;
                                g_sta_end_flag = 0;

                                clrAllStatus(rect[chRectTemp]);
                                chRectTemp = RECT_MODIADD_PIN1;

                                abspf = 0xffff;
                                absposx = 0xffff;
                                absposy = 0xffff;
                                dis_updata = 1;
                                dis_updata4 = 1;
                                if (g_jog_stpoint < tmp_feedsum)
                                {
                                    dat1 = g_patd.pat_add + g_jog_stpoint * 2;
                                    dat = SREG[dat1] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[dat1 + 1];
                                        x = (U8)(dat);
                                        y = (U8)((U16)(dat >> 8));
                                        if (x >= 0x80)
                                        {
                                            x -= 0x80;
                                            g_dxpos = -x;
                                        }
                                        else
                                        {
                                            g_dxpos = x;
                                        }
                                        if (y >= 0x80)
                                        {
                                            y -= 0x80;
                                            g_dypos = -y;
                                        }
                                        else
                                        {
                                            g_dypos = y;
                                        }
                                        dat = SREG[dat1];
                                        vdat = SREG[dat1 + 1];
                                    }
                                    else
                                    {
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                    }
                                }
                                else
                                {
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                    if (tmp_feedsum)
                                        dat = 0xff;
                                    else
                                        dat = 0x1b;
                                }
                                if (g_jog_point != g_jog_stpoint)
                                {
                                    Go_Stpoint(MENU_MODIFYADD, 0);
                                    g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 0);
                                    g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 1);
                                    g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 2);

                                    sendExCodeCmd();
                                }
                                g_jog_stpoint = 0;
                            }
                            else
                            {
                                pnoindex--;
                                pno = poldno[pnoindex];
                                if ((para == MODIFY_PART_CHANGE) && (pno == MENU_MODIFY1))
                                { // 20151227+++++++
                                    g_num_mov_flag = 2;
                                }
                                if ((para == CONVERT_ZOOM) || (para == CONVERT_ROTATE))
                                {
                                    g_para1 -= 25;
                                    g_num_mov_flag = 2;
                                    //bpno = MENU_CONVERT;
                                }
                            }
                        }
                        g_temp_pno = 0xffffffff;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        if (((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE)) && (g_sta_end_flag == 1))
                        { //����ѡ����ʼ��λ��
                            if ((!(S_CLAMP)) && (P_OST == 0))
                            {
                                pno = MENU_RWHELP;
                                hleppara = 13;
                                poldno[pnoindex++] = MENU_MODIFYADD;
                                break;
                            }
                            g_jog_enpoint = 0;
                            g_sta_end_flag = 0;

                            min_feedsum = 0;

                            clrAllStatus(rect[chRectTemp]);
                            chRectTemp = RECT_MODIADD_PIN1;

                            dis_updata = 1;
                            dis_updata4 = 1;
                            dis_updata = 1;
                            abspf = 0xffff;
                            absposx = 0xffff;
                            absposy = 0xffff;

                            if (g_jog_point != g_jog_stpoint)
                            {
                                Go_Stpoint(MENU_MODIFYADD, 1);
                                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                sendExCodeCmd();
                            }
                            g_jog_stpoint = 0;
                        }
                        else if ((para == MODIFY_PART_CHANGE) && (g_sta_end_flag == 4))
                        {
                            if ((!(S_CLAMP)) && (P_OST == 0))
                            {
                                pno = MENU_RWHELP;
                                hleppara = 13;
                                poldno[pnoindex++] = MENU_MODIFYADD;
                                break;
                            }

                            if (g_point_num > 1)
                            {
                                g_point_num--;
                                g_jog_point = g_array5[300 + g_point_num][0];
                                //								SeedPatpos(jog_point,(HREG[5]<<16|HREG[4]),&Borcmd,(S16*)&tmpoxpos,(S16*)&tmpoypos,&temp_oxpos1,&temp_oypos1,&Borpooft,0);
                                g_dxpos = (S16)(g_array5[g_point_num][0]);
                                g_dypos = (S16)(g_array5[g_point_num][1]);
                                Comm_Move(g_tmpoxpos + g_dxpos, g_tmpoypos + g_dypos, 50, MENU_MODIFYADD, 1, 0);
                                g_dxpos /= 2;
                                g_dypos /= 2;
                                pno = MENU_MODIFY1;
                                pnoindex--;
                            }
                            else
                            {
                                g_sta_end_flag = 1;
                                chRectTemp = RECT_MODIADD_PIN2;
                                //								RetstPoin(MENU_MODIFYADD);
                                //g_jog_point = g_jog_stpoint;
                                if (g_jog_enpoint > g_jog_stpoint)
                                    g_jog_enpoint = g_jog_stpoint;
                                pno = MENU_MODIFYADD;
                                g_temp_pno = 0xffffffff;
                                goto loop1;
                            }
                        }
                        else
                        {
                            pnoindex--;
                            pno = poldno[pnoindex];
                            if ((para == MODIFY_PART_CHANGE) && (pno == MENU_MODIFY1))
                            {
                                g_num_mov_flag = 2;
                            }
                            if ((para == CONVERT_ZOOM) || (para == CONVERT_ROTATE) || (para == CONVERT_CHANGE))
                            {
                                g_para1 -= 25;
                                if ((para == CONVERT_ZOOM) || (para == CONVERT_ROTATE))
                                {
                                    g_num_mov_flag = 2;
                                }
                            }
                        }
                    }

                    break;
                case KEY_MODIADD_ENTER:
                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        if ((para == MODIFY_PIN_DEL) || (para == MODIFY_PIN_ADD) || (para == MODIFY_PIN_MOVE) || (para == MODIFY_PITCH) || (para == MODIFY_PF) || (para == MODIFY_LF))
                        {
                            if (para == MODIFY_PF)
                            {
                                dat1 = g_patd.pat_add + g_jog_point * 2;
                                dat = SREG[dat1] & 0x00ff;
                                if (((dat != 0x61) && (dat != 0x41) && (dat != 0x21) && (dat != 0x1)))
                                { //ָ�ڴ�����λ������
                                    pno = MENU_RWHELP;
                                    hleppara = 66;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                }
                                else
                                {
                                    pno = MENU_PFOOT;
                                    g_para0 = 3;

                                    g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                                    g_pf_absval += g_pfootd.base;
                                    g_updata_pf_valflg = 1;
                                }
                            }
                            else if (para == MODIFY_LF)
                            {
                                dat1 = g_patd.pat_add + g_jog_point * 2;
                                dat = SREG[dat1] & 0x00ff;
                                if ((dat != 0x61) && (dat != 0x41) && (dat != 0x21) && (dat != 0x1))
                                { //ָ�ڴ�����λ������
                                    pno = MENU_RWHELP;
                                    hleppara = 66;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                }
                                else
                                {
                                    pno = MENU_LINEFORCE;
                                    g_para0 = 3;
                                }
                            }
                            else if (para == MODIFY_PIN_ADD)
                            {
                                dat1 = SREG[g_patd.pat_add + g_jog_point * 2] & 0x00ff;
                                if ((dat1 != 0x03) && (dat1 != 0x1b) && (dat1 != 0x01) && (dat1 != 0x21) && (dat1 != 0x41) && (dat1 != 0x61))
                                {
                                    g_AddRela = 1;
                                    if (g_jog_point == max_feedsum)
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 66;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                        break; //ָ���˴�����벽
                                    }
                                    else
                                    {
                                        pno = MENU_MODIFY1;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                    }
                                }
                                else
                                {
                                    g_AddRela = 0;
                                    pno = MENU_MODIFY1;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                }
                            }
                            else
                            {

                                pno = MENU_MODIFY1;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if (para == MODIFY_PART_ADD)
                        { //���β���

                            g_ModToCt = 1;

                            mit_ModToCt();
                            AnaPattern(0, PAT_MIT, 0);

                            g_feedc = g_jog_point;

                            if (g_feedc < g_patd.feed_sum)
                            {
                                g_ct_insert_flag = 3;
                            }
                            else
                            {
                                g_ct_insert_flag = 0;
                                g_feedc = g_patd.feed_sum;
                            }
                            if (g_ct_insert_flag)
                            {
                                g_ct_status_flag = 0x7cfdf;
                                g_ct_status_flag |= bit19;
                            }
                            g_feed_flag = 1;

                            g_point_num = 1;
                            g_array4[0][0] = 0;
                            g_feedsum = g_patd.feed_sum;
                            g_pf_mode = 0;

                            g_GreatPatType = 1;
                            for (i = 0; i < MAXBLKBUF; i++)
                            {
                                BLKBUF[i] = 0;
                            }

                            pno = MENU_CURSOR;
                            pnoindex = 2;
                        }
                        else if ((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE))
                        {
                            if (g_sta_end_flag == 0)
                            { // ����ѡ��������ʼ��(��ûѡ���յ�)
                                if (para == MODIFY_PART_MOVE)
                                {
                                    dat1 = SREG[g_patd.pat_add + g_jog_point * 2] & 0x00ff;
                                    if ((dat1 != 0x03) && (dat1 != 0x1b) && (dat1 != 0x01) && (dat1 != 0x21) && (dat1 != 0x41) && (dat1 != 0x61))
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 66;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                        break; //ָ���˴�����벽
                                    }
                                }
                                g_sta_end_flag = 1;
                                chRectTemp = RECT_MODIADD_PIN2;
                                g_jog_stpoint = g_jog_point;
                                min_feedsum = g_jog_stpoint;

                                tdat = dat;
                                tdat1 = vdat;
                                tdpf = Borpooft - old_PfootOffVal;
                                // tlf = g_lineforceValSet;
                                tdlf = dlforceval;
                                tpf = g_pf_absdis;
                                tdx = g_dxpos;
                                tdy = g_dypos;
                                tpx = g_xabspos;
                                tpy = g_yabspos;

                                abspf = 0xffff;
                                absposx = 0xffff;
                                absposy = 0xffff;
                                FillRectColorChang(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis,
                                                   rect[RECT_MODIADD_PIN1].xsize, rect[RECT_MODIADD_PIN1].ysize, rect[RECT_MODIADD_PIN1].colorT[1], Color_Black);

                                dis_updata = 1;
                                dis_updata4 = 1;
                                if (para == MODIFY_PART_CHANGE)
                                {
                                    if (((g_selectmode > 0) && (g_selectmode < 4)) || (g_selectmode == 5))
                                    {
                                        if (g_jogmode > 2)
                                            g_jogmode = 0;
                                        dis_updata1 = 1;
                                    }
                                }
                            }
                            else
                            {
                                if (g_sta_end_flag == 4)
                                {                                                 // ��ѡ�������յ�,����ѡ���޸�λ��
                                    g_array5[300 + g_point_num][0] = g_jog_point; //2013.5.24
                                    min_feedsum = g_jog_point;

                                    g_array10[300 + g_point_num][0] = g_jog_point;
                                }
                                else
                                {
                                    g_jog_enpoint = g_jog_point;
                                }

                                if ((para == MODIFY_PART_CHANGE) && (g_selectmode < 5))
                                {
                                    ii = g_patd.pat_add + g_jog_enpoint * 2;
                                    if ((SREG[ii - 2] == 0x1b) || (SREG[ii - 2] == 0x03) || (SREG[ii - 2] == 0x02))
                                    { // �����޸Ŀ���ָ���������һ��
                                        pno = MENU_RWHELP;
                                        hleppara = 54;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                        break;
                                    }
                                }

                                if ((g_sta_end_flag != 4) && (g_jog_stpoint < g_jog_enpoint))
                                {
                                    dat = SREG[g_patd.pat_add + (g_jog_point - 1) * 2] & 0x00ff;
                                    if ((dat != 0x03) && (dat != 0x1b) && (dat != 0x01) && (dat != 0x21) && (dat != 0x41) && (dat != 0x61))
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 66;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                        break; //ָ���˴�����벽
                                    }
                                    Go_Stpoint(MENU_MODIFYADD, 0);
                                    if (g_jog_point < tmp_feedsum)
                                    {
                                        dat = SREG[dat1 - 1] & 0x00ff;
                                        if ((dat == 0x1) || (dat == 0x21) || (dat == 0x41) || (dat == 0x61) || (dat == 0x3) || (dat == 0x1b))
                                        {
                                            dat = SREG[dat1];
                                            x = (U8)(dat);
                                            y = (U8)((U16)(dat >> 8));
                                            if (x >= 0x80)
                                            {
                                                x -= 0x80;
                                                g_dxpos = -x;
                                            }
                                            else
                                            {
                                                g_dxpos = x;
                                            }
                                            if (y >= 0x80)
                                            {
                                                y -= 0x80;
                                                g_dypos = -y;
                                            }
                                            else
                                            {
                                                g_dypos = y;
                                            }
                                            dat = SREG[dat1 - 1];
                                            vdat = SREG[dat1];
                                        }
                                        else
                                        {
                                            g_dxpos = 0;
                                            g_dypos = 0;
                                            dat = SREG[dat1 - 1];
                                            vdat = SREG[dat1];
                                        }
                                    }
                                    else
                                    {
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                        dat = 0xff;
                                    }
                                    g_temp_pno = 0xffffffff;
                                    //dis_data1(xx,yy,dat,g_jog_point,tmp_feedsum,dlforceval);
                                    // disStatus(vxp2, vyp2, dat, vdat, dlforceval, Borpooft - old_PfootOffVal, Color_Black);
                                    disStatus(rect[RECT_MODIADD_PIN2].xpis, rect[RECT_MODIADD_PIN2].ypis,
                                              dat, vdat, dlforceval, Borpooft - old_PfootOffVal, rect[RECT_MODIADD_PIN2].colorT[1]);
                                }

                                if ((g_jog_stpoint >= g_jog_point) && (g_sta_end_flag == 4))
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 19;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                    break;
                                }
                                else if (g_jog_stpoint >= g_jog_enpoint)
                                {
                                    g_jog_enpoint = 0;
                                    pno = MENU_RWHELP;
                                    hleppara = 19;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                    break;
                                }
                                else if (para == MODIFY_PART_MOVE)
                                {
                                    pno = MENU_MODIFY1;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                }
                                else
                                {
                                    if ((g_selectmode == 0) || (g_selectmode == 4))
                                    {
                                        g_imgpara = 200;
                                        pno = MENU_MOIMAGE;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                        g_tdxpos = 0;
                                        g_tdypos = 0;
                                    }
                                    else if (((g_selectmode > 0) && (g_selectmode < 4)) || (g_selectmode == 5))
                                    {
                                        if (g_sta_end_flag == 4)
                                        {
                                            if (g_jog_point == g_jog_enpoint)
                                            {
                                                pno = MENU_MOIMAGE;
                                                poldno[pnoindex] = MENU_MODIFYADD;
                                                pnoindex++; // �����ƶ���������һ���˵�ȷ����������
                                                if (g_selectmode == 2)
                                                {
                                                    g_array5[1][0] = 0;
                                                    g_array5[1][1] = 0;
                                                    if (g_jog_enpoint > g_jog_stpoint)
                                                    {
                                                        dat = g_jog_enpoint - g_jog_stpoint;
                                                        dat = dat / 2 + g_jog_stpoint;
                                                    }
                                                    else
                                                    {
                                                        dat = g_jog_stpoint - g_jog_enpoint;
                                                        dat = dat / 2 + g_jog_point;
                                                    }
                                                    g_array5[301][0] = dat;
                                                }
                                                g_imgpara = 201;
                                            }
                                            else
                                            {
                                                pno = MENU_MODIFY1;
                                                pnoindex--; // ������һ���˵�	JOGON
                                            }
                                        }
                                        else if (AutoDoSplit == 1)
                                        {
                                            S16 tmpdx = 0, tmpdy = 0;

                                            max_feedsum = g_jog_enpoint;
                                            min_feedsum = g_jog_stpoint;
                                            g_point_num = 1;

                                            wdt();
                                            for (i = 0; i < SPLINESUM_MAX; i++)
                                            {
                                                g_array10[i][0] = 0;
                                                g_array10[i][1] = 0;

                                                g_array5[i][0] = 0;
                                                g_array5[i][1] = 0;
                                            }
                                            wdt();

                                            while (min_feedsum < max_feedsum)
                                            {
                                                dat = SREG[g_patd.pat_add + min_feedsum * 2] & 0x00ff;
                                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                                {
                                                    dat = SREG[g_patd.pat_add + min_feedsum * 2 + 1];
                                                    dat &= 0x000000ff;
                                                    if (dat >= 0x80)
                                                    {
                                                        dat -= 0x80;
                                                        tmpdx = -dat;
                                                    }
                                                    else
                                                    {
                                                        tmpdx = dat;
                                                    }
                                                    dat = SREG[g_patd.pat_add + min_feedsum * 2 + 1];
                                                    dat = ((U32)(dat >> 8)) & 0x000000ff;
                                                    if (dat >= 0x80)
                                                    {
                                                        dat -= 0x80;
                                                        tmpdy = -dat;
                                                    }
                                                    else
                                                    {
                                                        tmpdy = dat;
                                                    }

                                                    g_array10[g_point_num][0] = tmpdx;
                                                    g_array10[g_point_num][1] = tmpdy;

                                                    g_array5[g_point_num][0] = tmpdx;
                                                    g_array5[g_point_num][1] = tmpdy;
                                                    g_point_num++;

                                                    if (g_point_num > SPLINESUM_MAX)
                                                    {
                                                        pno = MENU_RWHELP;
                                                        hleppara = 146;
                                                        poldno[pnoindex] = MENU_MODIFYADD;
                                                        pnoindex++;
                                                        break;
                                                    }
                                                }
                                                min_feedsum++;
                                            }
                                            //											g_sta_end_flag = 0;
                                            g_imgpara = 220;
                                            pno = MENU_MOIMAGE;
                                            poldno[pnoindex] = MENU_MODIFYADD;
                                            pnoindex++;
                                        }
                                        else if (g_jogmode == 0)
                                        {
                                            g_sta_end_flag = 4;
                                            max_feedsum = g_jog_enpoint;
                                            min_feedsum = g_jog_stpoint;
                                            //xx = 192;yy = 192;
                                            dis_updata = 1;
                                            dis_updata1 = 0;
                                            dis_updata4 = 1;
                                            g_point_num = 1;
                                            absposx = 0xffff;
                                            absposy = 0xffff;
                                            abspf = 0xffff;

                                            clrAllStatus(rect[RECT_MODIADD_PIN1]);
                                            clrAllStatus(rect[RECT_MODIADD_PIN2]);
                                            for (i = RECT_MODIADD_IN_TIT; i <= RECT_MODIADD_IN_STR; i++)
                                            {

                                                DrawFillRect(rect[i].xpis, rect[i].ypis,
                                                             rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                                                if (i == RECT_MODIADD_IN_TIT)
                                                {
                                                    Draw24x24Str(rect[i].xpis + (rect[i].xsize - 13 * strlen((char *)((MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0])))) / 2,
                                                                 rect[i].ypis + (rect[i].ysize - 24) / 2,
                                                                 (MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0]),
                                                                 SheetPage, rect[i].index[0], rect[i].colorT[0]); // �����
                                                }
                                                else
                                                {
                                                    DrawDottedRect(rect[i].xpis, rect[i].ypis,
                                                                   rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
                                                    DrawMFS32P(rect[i].xpis + (rect[i].xsize - 13 * 2) / 2,
                                                               rect[i].ypis + (rect[i].ysize - 24) / 2, g_point_num, 2, 0, rect[i].colorT[0]);
                                                }
                                            }
                                            BmpFillRect(keyd[KEY_MODIADD_JOGON].xpis, keyd[KEY_MODIADD_JOGON].ypis, keyd[KEY_MODIADD_JOGON].xsize, keyd[KEY_MODIADD_JOGON].ysize);
                                            BmpFillRect(keyd[KEY_MODIADD_JOGOFF].xpis, keyd[KEY_MODIADD_JOGOFF].ypis, keyd[KEY_MODIADD_JOGOFF].xsize, keyd[KEY_MODIADD_JOGOFF].ysize);
                                            DrawButton(keyd[KEY_MODIADD_INPUTEND], (U16 *)(ADD_ICONBMP),
                                                       keybmpind[KEY_MODIADD_INPUTEND].border[0], keybmpind[KEY_MODIADD_INPUTEND].icon[0]);
                                        }
                                        else
                                        {
                                            g_point_num = 1;
                                            pno = MENU_MODIFY1;
                                            pnoindex--; // ������һ���˵�	JOGOFF
                                        }
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                    }
                                }
                            }
                        }
                        else if (para == MODIFY_SPEED)
                        { //k>14,k<20�޸Ĳ˵��е��ٶ�����
                            pno = MENU_MODIFY2;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                        }
                        else if (para == MODIFY_CODE_ADD)
                        { //k=20 	   �޸Ĳ˵��еĴ�������  ����
                            g_dxpos1 = 0;
                            g_dypos1 = 0;
                            if (g_jog_point > 0)
                            {
                                dat1 = g_patd.pat_add + (g_jog_point - 1) * 2;
                                dat = SREG[dat1 + 1];
                                x = (U8)(dat);
                                y = (U8)((U16)(dat >> 8));
                                if (x >= 0x80)
                                {
                                    x -= 0x80;
                                    g_dxpos1 = -x;
                                }
                                else
                                {
                                    g_dxpos1 = x;
                                }
                                if (y >= 0x80)
                                {
                                    y -= 0x80;
                                    g_dypos1 = -y;
                                }
                                else
                                {
                                    g_dypos1 = y;
                                }
                            }

                            dat1 = g_patd.pat_add + g_jog_point * 2;
                            dat = SREG[dat1];
                            if (dat == 0x001c)
                            { //������
                                g_exType = 1;
                                g_exFeedSp = SREG[dat1 + 1] & 0xff;
                            }
                            else if (dat == 0x031c)
                            { //������
                                g_exType = 2;
                                g_exRestart = SREG[dat1 + 1] & 0xff;
                            }
                            else if (dat == 0x011c)
                            { //���Ϻ�
                                g_exType = 3;
                                g_exThickness = SREG[dat1 + 1] & 0xff;
                            }
                            else if (dat == 0x021c)
                            { //ѹ����
                                g_exType = 4;
                                g_exClweight = SREG[dat1 + 1] & 0xff;
                            }
                            else if ((dat & 0xff) == 0x3d)
                            { //����һ
                                g_exType = 5;
                                g_pointSetPara1 = (dat >> 8) & 0xff;
                            }
                            else if ((dat & 0xff) == 0x3e)
                            { //���ö�
                                g_exType = 6;
                                g_pointSetPara2 = (dat >> 8) & 0xff;
                            }
                            else if ((dat & 0xff) == 0x3c)
                            { //��Ƕ�
                                g_exType = 7;
                                g_pointPinAng = (dat >> 8) & 0xff;
                            }
                            else
                            { //��������ָ��
                                g_exType = 0;
                            }
                            pno = MENU_CODE;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                        }
                        else if (para == MODIFY_CODE_DEL)
                        { //k=21 	   �޸Ĳ˵��еĴ�������  ɾ��
                            dat1 = g_patd.pat_add + g_jog_point * 2;
                            dat = SREG[dat1];
                            if ((dat & 0xff) == 0x3c || (dat & 0xff) == 0x3d || (dat & 0xff) == 0x3e)
                            {
                                pno = MENU_MODIFY3;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                                break;
                            } //��Ƕ�,����һ,���ö�
                            for (dat1 = 1; dat1 < 19; dat1++)
                            {
                                if (dat == pat_code[dat1])
                                {
                                    if (dat1 != 14)
                                    {
                                        pno = MENU_MODIFY3;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                        break;
                                    }
                                }
                            }
                            if (dat1 >= 19)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 22;
                                EREG[1] = 176;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if (para == CONVERT_TACKING)
                        {
                            pno = MENU_TACKING;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                            if (g_bt_mode > 2)
                                g_bt_mode = 0; //����
                            if (g_repeat > 3)
                                g_repeat = 0;
                            if (g_r_stitch > 9)
                                g_r_stitch = 0;
                            if (g_srt_bt > 3)
                                g_srt_bt = 0;
                            if (g_s_stitch > 9)
                                g_s_stitch = 0;
                            if (g_ent_bt > 3)
                                g_ent_bt = 0;
                            if (g_e_stitch > 9)
                                g_e_stitch = 0;
                        }
                        else if (para == CONVERT_ZIGZAG)
                        {
                            pno = MENU_ZIGZAG;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                            if (g_zig_zag > 1)
                                g_zig_zag = 0; //Z�ַ�
                            if ((g_zig_width == 0) || (g_zig_width > 100))
                                g_zig_width = 10;
                            if ((g_zig_pitch < 5) || (g_zig_pitch > 100))
                                g_zig_pitch = 30;
                            if (g_zig_direction > 2)
                                g_zig_direction = 1;
                        }
                        else if ((para == CONVERT_ZOOM) || (para == CONVERT_ROTATE))
                        {
                            pno = MENU_CONVERT1;
                            pnoindex--;
                            g_para1 -= 25;
                        }
                        else if (para == CONVERT_PINANG)
                        { //��Ƕ�
                            dat1 = g_patd.pat_add + g_jog_point * 2;
                            dat = SREG[dat1];
                            if ((dat & 0xff) != 0x3c)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                g_dxpos1 = 0;
                                g_dypos1 = 0;
                                if (g_jog_point > 0)
                                {
                                    dat1 = g_patd.pat_add + (g_jog_point - 1) * 2;
                                    dat = SREG[dat1 + 1];
                                    x = (U8)(dat);
                                    y = (U8)((U16)(dat >> 8));
                                    if (x >= 0x80)
                                    {
                                        x -= 0x80;
                                        g_dxpos1 = -x;
                                    }
                                    else
                                    {
                                        g_dxpos1 = x;
                                    }
                                    if (y >= 0x80)
                                    {
                                        y -= 0x80;
                                        g_dypos1 = -y;
                                    }
                                    else
                                    {
                                        g_dypos1 = y;
                                    }
                                }
                                pno = MENU_EXCODE1;
                                poldno[pnoindex++] = MENU_MODIFYADD;
                            }
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        g_BorSTflag = Stflag;
                        if (para == MODIFY_PIN_DEL)
                        { //��ɾ��
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if ((Stflag == 0) || (((Borcmd & 0xf000) != 0x2000) && ((Borcmd & 0xf000) != 0x6000) && (Stflag != 1)))
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_MODIFY1;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if (para == MODIFY_PIN_ADD)
                        { //��׷��
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if (Stflag == 0 /*||(Borcmd==0xff00)||(((Borcmd&0xf000)!=0x2000)&&((Borcmd&0xf000)!=0x6000)&&(Stflag!=1))*/)
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_MODIFY1;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                                g_selectmode = 0; //Ĭ��ѡ��������
                            }
                        }
                        else if (para == MODIFY_PIN_MOVE)
                        { //��λ���޸�
                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if ((Stflag == 0) || (((Borcmd & 0xf000) != 0x2000) && ((Borcmd & 0xf000) != 0x6000) && (Stflag != 1)))
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_MODIFY1;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if (para == MODIFY_PITCH)
                        { //���
                            pno = MENU_MODIFY1;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                        }
                        else if ((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE))
                        { //�����ƶ��������޸�
                            if (g_sta_end_flag == 0)
                            { // ����ѡ��������ʼ��(��ûѡ���յ�)
                                if (Stflag == 0)
                                { //û�ƶ�����ԭ��ʱ�������޸�
                                    pno = MENU_RWHELP;
                                    hleppara = 66;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                }
                                else
                                {
                                    g_sta_end_flag = 1;
                                    g_jog_stpoint = g_jog_point;
                                    min_feedsum = g_jog_stpoint;

                                    tdat = Borcmd;
                                    tdpf = Borpooft - old_PfootOffVal;
                                    tdlf = dlforceval;
                                    // tlf = g_lineforceValSet;
                                    tpf = g_pf_absdis;
                                    tdx = g_dxpos;
                                    tdy = g_dypos;
                                    tpx = g_xabspos;
                                    tpy = g_yabspos;

                                    dis_updata = 1;
                                    dis_updata4 = 1;
                                    abspf = 0xffff;
                                    absposx = 0xffff;
                                    absposy = 0xffff;
                                    FillRectColorChang(rect[RECT_MODIADD_PIN1].xpis, rect[RECT_MODIADD_PIN1].ypis,
                                                       rect[RECT_MODIADD_PIN1].xsize, rect[RECT_MODIADD_PIN1].ysize, rect[RECT_MODIADD_PIN1].colorT[1], Color_Black);

                                    if (para == MODIFY_PART_CHANGE)
                                    {
                                        if (((g_selectmode > 0) && (g_selectmode < 4)) || (g_selectmode == 5))
                                        {
                                            if (g_jogmode > 2)
                                                g_jogmode = 0;
                                            dis_updata1 = 1;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (g_sta_end_flag == 4)
                                {                                                 // ��ѡ�������յ�,����ѡ���޸�λ��
                                    g_array5[300 + g_point_num][0] = g_jog_point; //2013.5.24
                                    min_feedsum = g_jog_point;

                                    g_array10[300 + g_point_num][0] = g_jog_point;
                                }
                                else
                                {
                                    g_jog_enpoint = g_jog_point;
                                }

                                if ((g_jog_stpoint >= g_jog_point) && (g_sta_end_flag == 4))
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 19;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                    break;
                                }
                                else if (g_jog_stpoint >= g_jog_enpoint)
                                {
                                    g_jog_enpoint = 0;
                                    pno = MENU_RWHELP;
                                    hleppara = 19;
                                    poldno[pnoindex] = MENU_MODIFYADD;
                                    pnoindex++;
                                    break;
                                }
                                else
                                {
                                    if (para == MODIFY_PART_MOVE)
                                    {
                                        if ((!(S_CLAMP)) && (P_OST == 0))
                                        {
                                            pno = MENU_RWHELP;
                                            hleppara = 13;
                                            poldno[pnoindex++] = MENU_MODIFYADD;
                                            break;
                                        }
                                        Go_Stpoint(MENU_MODIFYADD, 1); //�ص��������
                                        g_jog_point = g_jog_stpoint;
                                        pno = MENU_MODIFY1;
                                        poldno[pnoindex] = MENU_MODIFYADD;
                                        pnoindex++;
                                    }
                                    else if (para == MODIFY_PART_CHANGE)
                                    {
                                        if (g_selectmode == 0 || g_selectmode == 4)
                                        { //ֱ��,Z�ַ�
                                            if ((!(S_CLAMP)) && (P_OST == 0))
                                            {
                                                pno = MENU_RWHELP;
                                                hleppara = 13;
                                                poldno[pnoindex++] = MENU_MODIFYADD;
                                                break;
                                            }
                                            Go_Stpoint(MENU_MODIFYADD, 1); //�ص��������
                                            g_jog_point = g_jog_stpoint;
                                            g_imgpara = 200;
                                            pno = MENU_MOIMAGE;
                                            poldno[pnoindex] = MENU_MODIFYADD;
                                            pnoindex++;
                                            g_tdxpos = 0;
                                            g_tdypos = 0;
                                        }
                                        else if ((g_selectmode == 1) || (g_selectmode == 2) || (g_selectmode == 3) || (g_selectmode == 5))
                                        { //����  ��Բ�������� ������
                                            if (g_jogmode == 1)
                                            { //������ƶ�
                                                if ((!(S_CLAMP)) && (P_OST == 0))
                                                {
                                                    pno = MENU_RWHELP;
                                                    hleppara = 13;
                                                    poldno[pnoindex++] = MENU_MODIFYADD;
                                                    break;
                                                }
                                                Go_Stpoint(MENU_MODIFYADD, 1); //�ص��������
                                                g_point_num = 1;
                                                pno = MENU_MODIFY1;
                                                poldno[pnoindex] = MENU_MODIFYADD;
                                                pnoindex++;
                                            }
                                            else if (g_jogmode == 0)
                                            {
                                                if (g_sta_end_flag != 4)
                                                {
                                                    if ((!(S_CLAMP)) && (P_OST == 0))
                                                    {
                                                        pno = MENU_RWHELP;
                                                        hleppara = 13;
                                                        poldno[pnoindex++] = MENU_MODIFYADD;
                                                        break;
                                                    }
                                                    Go_Stpoint(MENU_MODIFYADD, 1); //�ص��������
                                                    g_sta_end_flag = 4;
                                                    max_feedsum = g_jog_enpoint;
                                                    min_feedsum = g_jog_stpoint;
                                                    //xx = 192;yy = 192;
                                                    dis_updata = 1;
                                                    dis_updata1 = 0;
                                                    dis_updata4 = 1;
                                                    g_point_num = 1;

                                                    absposx = 0xffff;
                                                    absposy = 0xffff;
                                                    abspf = 0xffff;

                                                    clrAllStatus(rect[RECT_MODIADD_PIN1]);
                                                    clrAllStatus(rect[RECT_MODIADD_PIN2]);
                                                    for (i = RECT_MODIADD_IN_TIT; i <= RECT_MODIADD_IN_STR; i++)
                                                    {

                                                        DrawFillRect(rect[i].xpis, rect[i].ypis,
                                                                     rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                                                        if (i == RECT_MODIADD_IN_TIT)
                                                        {
                                                            Draw24x24Str(rect[i].xpis + (rect[i].xsize - 13 * strlen((char *)((MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0])))) / 2,
                                                                         rect[i].ypis + (rect[i].ysize - 24) / 2,
                                                                         (MEM_TITLE_ADD + TitleLengthMax * rect[i].index[0]),
                                                                         SheetPage, rect[i].index[0], rect[i].colorT[0]); // �����
                                                        }
                                                        else
                                                        {
                                                            DrawDottedRect(rect[i].xpis, rect[i].ypis,
                                                                           rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
                                                            DrawMFS32P(rect[i].xpis + (rect[i].xsize - 13 * 2) / 2,
                                                                       rect[i].ypis + (rect[i].ysize - 24) / 2, g_point_num, 2, 0, rect[i].colorT[0]);
                                                        }
                                                    }
                                                    BmpFillRect(keyd[KEY_MODIADD_JOGON].xpis, keyd[KEY_MODIADD_JOGON].ypis, keyd[KEY_MODIADD_JOGON].xsize, keyd[KEY_MODIADD_JOGON].ysize);
                                                    BmpFillRect(keyd[KEY_MODIADD_JOGOFF].xpis, keyd[KEY_MODIADD_JOGOFF].ypis, keyd[KEY_MODIADD_JOGOFF].xsize, keyd[KEY_MODIADD_JOGOFF].ysize);
                                                    DrawButton(keyd[KEY_MODIADD_INPUTEND], (U16 *)(ADD_ICONBMP),
                                                               keybmpind[KEY_MODIADD_INPUTEND].border[0], keybmpind[KEY_MODIADD_INPUTEND].icon[0]);
                                                }
                                                else
                                                {
                                                    if (g_jog_point == g_jog_enpoint)
                                                    {
                                                        pno = MENU_MOIMAGE;
                                                        poldno[pnoindex] = MENU_MODIFYADD;
                                                        pnoindex++; // �����ƶ���������һ���˵�ȷ����������
                                                        g_imgpara = 201;
                                                    }
                                                    else
                                                    {
                                                        g_dxpos = 0;
                                                        g_dypos = 0;
                                                        pno = MENU_MODIFY1;
                                                        pnoindex--; // ������һ���˵�	JOGON
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (para == MODIFY_PF)
                        { //��ѹ��
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            //if(((Stflag!=2)&&(g_jog_point==0))||(((Borcmd&0xff00)!=0xf000) &&((Borcmd&0xf000)!=0x6000)&&(Borcmd!=0xf400))){
                            if (((Stflag != 2) && (g_jog_point == 0)) || ((Borcmd & 0xf000) == 0x2000) || Borcmd == 0xfd00 || Borcmd == 0xff00)
                            {
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                //								Bor_feedInfo(g_jog_point+1,g_patd.pat_len,&Borcmd,&temp_oxpos1,&temp_oypos1,&tmpxdat,&tmpydat,(U8*)&g_oldspeed,&Borpooft,&Borpfdis);
                                if ((Borcmd & 0xff00) == 0xf000)
                                {
                                    g_IsPfcmd = 1;
                                }
                                else
                                {
                                    g_IsPfcmd = 0;
                                }

                                g_pf_absval = Borpooft + g_pfootd.base;
                                g_pf_moddis = Borpfdis;
                                //#if Debug
                                //printf("Borpooft = %d 	Borpfdis = %d  g_IsPfcmd = %d\r\n",Borpooft,Borpfdis,g_IsPfcmd);
                                //#endif
                                g_updata_pf_valflg = 1;
                                pno = MENU_PFOOT;
                                g_para0 = 3;
                            }
                        }
                        else if (para == MODIFY_LF)
                        { //����
                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if (((Borcmd & 0xff00) != 0xf100) && ((Borcmd & 0xf000) != 0x6000))
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_LINEFORCE;
                                g_para0 = 3;
                            }
                        }
                        else if (para == MODIFY_PART_ADD) //���β���
                        {

                            g_ModToCt = 1;

                            g_CtPatLen = F_FILELEN;
                            Bor_CtBefDatAny((U32 *)&g_Ctstaddr, (U32 *)&g_CtPatLen, (U8 *)&g_oldspeed, 1);
                            g_Ctsavetimes = 0;
                            Bor_CreatBack((U16 *)&g_Ctsavetimes, g_Ctstaddr, g_Ctendaddr, g_oxpos, g_oypos, g_oldspeed, g_pf_absval - g_pfootd.base, g_pf_dis, g_lineforceValSetNow, g_patd.orgX, g_patd.orgY);
                            g_pf_absval += g_pfootd.base;

                            if (g_jog_point > g_patd.feed_sum)
                            {
                                g_jog_point = g_patd.feed_sum;
                            }
                            g_speed = g_oldspeed;
                            g_feedc = g_jog_point;

                            if (g_feedc < g_patd.feed_sum)
                            {
                                g_ct_insert_flag = 3;
                            }
                            else
                            {
                                g_ct_insert_flag = 0;
                            }
                            if (g_ct_insert_flag)
                            {
                                g_ct_status_flag = 0x7cfdf;
                                g_ct_status_flag |= bit19;
                            }
                            g_feed_flag = 1;

                            g_Ctendaddr = sew_getPinAddr((U16 *)SREG, g_CtPatLen, g_feedc, g_ct_insert_flag == 0 ? 1 : 0);

                            g_point_num = 1;
                            g_array4[0][0] = 0;
                            g_feedsum = g_patd.feed_sum;
                            g_pf_mode = 0;

                            g_GreatPatType = 2;
                            for (i = 0; i < MAXBLKBUF; i++)
                            {
                                BLKBUF[i] = 0;
                            }
                            //Bor_feedInfo(g_feedc,g_CtPatLen,&Borcmd,(S16*)&g_oxpos,(S16*)&g_oypos,(S16*)&g_dxpos,(S16*)&g_dypos,(U8*)&g_oldspeed,&Borpooft,&Borpfdis);
                            //if(g_oldspeed==0)g_oldspeed=0x60;
                            //else if(g_oldspeed==1)g_oldspeed=0x40;
                            //else if(g_oldspeed==2)g_oldspeed=0x20;
                            //else g_oldspeed=0x0;

                            pno = MENU_CURSOR;
                            pnoindex = 2;
                        }
                        else if (para == MODIFY_SPEED)
                        { //�ٶ��޸�															//k>14,k<20�޸Ĳ˵��е��ٶ�����
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if ((Stflag == 0) || (((Borcmd & 0xff00) != 0xfb00) && ((Borcmd & 0xf000) != 0x6000) && (Stflag != 1)))
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_MODIFY2;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if (para == MODIFY_CODE_ADD)
                        { //k=20 	   �޸Ĳ˵��еĴ�������  ����
                            pno = MENU_CODE;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos1, (S16 *)&g_dypos1, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            dat = Borcmd;

                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos1, (S16 *)&g_dypos1, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);

                            if ((dat & 0xff00) == 0xea00)
                            { //����һ
                                g_exType = 5;
                                g_pointSetPara1 = dat & 0xff;
                            }
                            else if ((dat & 0xff00) == 0xeb00)
                            { //���ö�
                                g_exType = 6;
                                g_pointSetPara2 = dat & 0xff;
                            }
                            else if ((dat & 0xff00) == 0xec00)
                            { //��Ƕ�
                                g_exType = 7;
                                g_pointPinAng = dat & 0xff;
                            }
                            else
                            {
                                g_exType = 0;
                            }
                        }
                        else if (para == MODIFY_CODE_DEL)
                        { //	 �޸Ĳ˵��еĴ�������  ɾ��
                            if (Borcmd != 0x5050)
                            { //���Ǵ�ԭ��
                                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, &tmpxdat, &tmpydat, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            }
                            if (((Borcmd & 0xff00) == 0xf000) || ((Borcmd & 0xff00) == 0xf100) || ((Borcmd & 0xff00) == 0xf300) || ((Borcmd & 0xff00) == 0xfb00) || ((Borcmd & 0xff00) == 0xfc00) ||
                                ((Borcmd & 0xff00) == 0xfd00) || ((Borcmd & 0xff00) == 0xfe00) || ((Borcmd & 0xff00) == 0xf400) || (Borcmd == 0x5050) || ((Borcmd & 0xff00) == 0xec00) || ((Borcmd & 0xff00) == 0xea00) || ((Borcmd & 0xff00) == 0xeb00))
                            {
                                g_UpdataBorDelFuns = 1;
                                pno = MENU_MODIFY3;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_RWHELP;
                                hleppara = 22;
                                EREG[1] = 176;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if (para == CONVERT_TACKING)
                        { //����
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &tmpxdat, &tmpydat, &temp_oxpos1, &temp_oypos1, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if (((Borcmd & 0xf000) != 0x6000) || ((g_jog_point == 0) && (Stflag != 2)))
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pno = MENU_TACKING;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                        }
                        else if ((para == CONVERT_ZOOM) || (para == CONVERT_ROTATE))
                        { //�Ŵ���С����ת
                            pno = MENU_CONVERT1;
                            pnoindex--;
                            g_para1 -= 25;
                        }
                        else if (para == CONVERT_CHANGE)
                        { //��β����
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &tmpxdat, &tmpydat, &temp_oxpos1, &temp_oypos1, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if (((Borcmd & 0xf000) != 0x6000) || ((g_jog_point == 0) && (Stflag != 2)))
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                pnoindex--;
                                pno = poldno[pnoindex];
                                g_para1 -= 25;

                                tmpstpin = Bor_getStPin(g_patd.pat_len, g_jog_point);

                                if (P_FHP)
                                { //�Ȱ���ԭ��԰�ص�ԭ��
                                    tjog_point = g_jog_point;
                                    Go_Home(MENU_CONVERT1, 1);
                                    g_jog_point = tjog_point;
                                }
                                f = Bor_ConveStartEnd(tmpstpin, g_patd.pat_len);

                                if (P_FHP)
                                {
                                    g_jog_point = 0;
                                }
                                else
                                {
                                    Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                                    g_tmpoxpos += g_dxpos;
                                    g_tmpoypos += g_dypos;
                                    Send_Pfoot(1, pno, 1);
                                    Comm_Move(g_tmpoxpos, g_tmpoypos, 80, pno, 1, 0);
                                }

                                if (f)
                                {
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                }
                                else
                                {
                                    RecovrSREG(g_savetimes, 1);
                                    AnaPattern(0, PAT_SEW, (F_FILELEN));
                                    pno = MENU_RWHELP;
                                    hleppara = 20;
                                    pnoindex++;
                                }
                                Wait_Move_End();
                            }
                        }
                        else if (para == CONVERT_PINANG)
                        { //��Ƕ�
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &tmpxdat, &tmpydat, &temp_oxpos1, &temp_oypos1, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            if ((Borcmd & 0xff00) != 0xec00)
                            { //ָ�ڴ�����λ������
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFYADD;
                                pnoindex++;
                            }
                            else
                            {
                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos1, (S16 *)&g_dypos1, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                pno = MENU_EXCODE1;
                                poldno[pnoindex++] = MENU_MODIFYADD;
                            }
                        }
                    }

                    break;
                case KEY_MODIADD_PAT:
                    g_ImgMove = 1;
                    pno = MENU_MOIMAGE;
                    poldno[pnoindex] = MENU_MODIFYADD;
                    pnoindex++;
                    if ((para == MODIFY_PART_MOVE) || (para == MODIFY_PART_CHANGE))
                    {
                        if ((para == MODIFY_PART_CHANGE) && (g_sta_end_flag == 4))
                        {
                            g_imgpara = 211;
                        }
                        else
                        {
                            g_imgpara = 210;
                        }
                    }

                    g_BorSTflag = Stflag;
                    g_max_feedsum = max_feedsum;
                    g_min_feedsum = min_feedsum;
                    break;

                case KEY_MODIADD_ORG:
                    dis_updata4 = 1;
                    g_BorSTflag = Stflag;
                    f = Go_Home(MENU_MODIFYADD, 1); //0
                    if (f == 0)
                        break;
                    EnUPflag = 1;

                    isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
                    if ((isHome & 0x02) || (isHome & 0x04))
                    {
                    }
                    else
                    {                     //û�лص���еԭ��λ��
                        g_needtohome = 1; //�綯ǰ��Ҫ��һ��ԭ��
                        break;
                    }
                    g_needtohome = 0;

                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        g_jog_point = 0;
                        if (g_sta_end_flag == 1)
                        {
                            g_jog_enpoint = 0;
                        }
                        else if (g_sta_end_flag != 4)
                        {
                            g_jog_stpoint = 0;
                        }
                        if (g_jog_point < tmp_feedsum)
                        {
                            dat = SREG[g_patd.pat_add] & 0x00ff;
                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                            {
                                dat = SREG[g_patd.pat_add + 1];
                                x = (U8)(dat);
                                y = (U8)((U16)(dat >> 8));
                                if (x >= 0x80)
                                {
                                    x -= 0x80;
                                    g_dxpos = -x;
                                }
                                else
                                {
                                    g_dxpos = x;
                                }
                                if (y >= 0x80)
                                {
                                    y -= 0x80;
                                    g_dypos = -y;
                                }
                                else
                                {
                                    g_dypos = y;
                                }
                                dat = SREG[g_patd.pat_add];
                                vdat = SREG[g_patd.pat_add + 1];
                            }
                            else
                            {
                                g_dxpos = 0;
                                g_dypos = 0;
                                dat = SREG[g_patd.pat_add];
                                vdat = SREG[g_patd.pat_add + 1];
                            }
                        }
                        else
                        {
                            g_dxpos = 0;
                            g_dypos = 0;
                            dat = 0xff;
                        }
                        g_temp_oxpos = 0;
                        g_temp_oypos = 0;
#if PATRULES_ORG_OFFSET
                        if (SREG[3] == 7) //����-�޸�ԭ��λ��
                        {
                            g_temp_oxpos = (S16)(SREG[6]);
                            g_temp_oxpos = (S16)(SREG[7]);
                        }
#endif
                        // disStatus(vxp2, vyp2, dat, vdat, dlforceval, 0, Color_Black);
                        //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                        if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        else if (g_sta_end_flag == 1)
                        {
                            chRectTemp = RECT_MODIADD_PIN2;
                        }
                        else
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, 0, rect[chRectTemp].colorT[1]);

                        vpointf = 1;
                        vpoint = g_jog_point;
                        f = Read_D(D_Address[SYS] + SYS_PFOOTNOW, 1, (U16 *)(&YREG[SYS_PFOOTNOW]));
                        //���趨��ѹ��������
                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_MODIFYADD;
                            pnoindex++;
                            break;
                        }
                        temp_PfVal = YREG[SYS_PFOOTNOW];
                        old_PfootOffVal = temp_PfVal;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        Stflag = 0;
                        g_BorSTflag = Stflag;
                        g_jog_point = 0;
                        Up = 0;
                        Dn = 1;
                        Borcmd = 0x2000;
                        g_dxpos = g_patd.orgX;
                        g_dypos = g_patd.orgY;
                        //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,0,g_dxpos,g_dypos,dlforceval);
                        // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, 0, Color_Black);
                        //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                        if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        else if (g_sta_end_flag == 1)
                        {
                            chRectTemp = RECT_MODIADD_PIN2;
                        }
                        else
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, 0, rect[chRectTemp].colorT[1]);

                        vpointf = 1;
                        vpoint = g_jog_point;
                    }
                    g_ExCode = 0;
                    g_curExCode = 0;
                    g_pointPinAng = 0;
                    g_pointSetPara1 = 0;
                    g_pointSetPara2 = 0;
                    g_modcordflag = 0;
                    break;
                case KEY_MODIADD_FOOT:
                    Send_Pfoot(0, MENU_MODIFYADD, 0);
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    //tmp_status = (U16)(S_PFOOT);

                    pfootf = 0xff;
                    break;
                case KEY_MODIADD_JOGON:
                    g_jogmode = 0;
                    AutoDoSplit = 0;
                    dis_updata1 = 1;
                    break;
                case KEY_MODIADD_JOGOFF:
                    g_jogmode = 1;
                    AutoDoSplit = 0;
                    dis_updata1 = 1;
                    break;
                case KEY_MODIADD_INPUTEND:
                    pno = MENU_MOIMAGE;
                    poldno[pnoindex] = MENU_MODIFYADD;
                    pnoindex++;
                    if (F_FILETYPE == PAT_MIT)
                    { //
                        if (g_selectmode == 2)
                        {
                            g_array5[1][0] = 0;
                            g_array5[1][1] = 0;
                            if (g_jog_enpoint > g_jog_stpoint)
                            {
                                dat = g_jog_enpoint - g_jog_stpoint;
                                dat = dat / 2 + g_jog_stpoint;
                            }
                            else
                            {
                                dat = g_jog_stpoint - g_jog_enpoint;
                                dat = dat / 2 + g_jog_point;
                            }
                            g_array5[301][0] = dat;
                        }
                    }

                    g_imgpara = 201;
                    break;
                case KEY_MODIADD_MUP:
                case KEY_MODIADD_JOGUP:

                    if ((!(S_CLAMP)) && (P_OST == 0))
                    {
                        pno = MENU_RWHELP;
                        hleppara = 13;
                        poldno[pnoindex] = MENU_MODIFYADD;
                        pnoindex++;
                        break;
                    }

                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        g_updata_pf_valflg = 1;

                        if (g_jog_point < max_feedsum)
                        {
                            dat1 = g_patd.pat_add + g_jog_point * 2;
                            if ((g_jog_point < tmp_feedsum) && (uplock == 1))
                            {
                                dat = SREG[dat1] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1))
                                {
                                    if (((SREG[dat1] & 0xff00) != 0))
                                    {
                                        pfootval = (S8)(SREG[dat1] >> 8);
                                        temp_PfVal = g_pfootd.base + Mit_CailPadOffestPF(g_jog_point, 0, 0) + pfootval;
                                    }
                                }
                                else if ((dat == 0x1b) || (dat == 0x03))
                                { //����
                                    EnUPflag = 1;
                                    if (Up)
                                    {
                                        Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        Up = 0;
                                        Dn = 1;
                                        break;
                                    }
                                }
                                g_temp_PfootOffVal = temp_PfVal;
                                uplock = 0;
                            }
                            if (((SREG[dat1] & 0x00ff) == 0x1) || ((SREG[dat1] & 0x00ff) == 0x21) || ((SREG[dat1] & 0x00ff) == 0x41) || ((SREG[dat1] & 0x00ff) == 0x61))
                            { //����
                                if ((old_PfootOffVal != temp_PfVal) || Dn)
                                {
                                    if ((Dn == 1) && ((EnUPflag == 1) || (old_PfootOffVal != temp_PfVal)))
                                    {
                                        Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        Up = 1;
                                        Dn = 0;
                                        break;
                                    }
                                    if (old_PfootOffVal != temp_PfVal)
                                    {
                                        old_PfootOffVal = temp_PfVal;

                                        if (pfootval > 0)
                                        {
                                            f = Write_Cmd(C_PFEMOV, 1, pfootval);
                                        }
                                        else
                                        {
                                            f = Write_Cmd(C_PFEMOV, 2, (-pfootval));
                                        }
                                        Read_Status();
                                        //for(i=0;i<RDTIMES;i++){
                                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                                        //	DelayMs(WAITTIME);
                                        //}
                                    }
                                }
                                EnUPflag = 0;
                            }
                            else if (((SREG[dat1] & 0x00ff) == 0x03) || ((SREG[dat1] & 0x00ff) == 0x1b))
                            { //����
                                EnUPflag = 1;
                                if (Up)
                                {
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                Up = 0;
                                Dn = 1;
                            }
                            else if (((SREG[dat1] & 0x00ff) == 0x07))
                            { //����
                                EnUPflag = 1;
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Up == 1)
                                { //ѹ��������,ѹ����
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                else
                                {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    Up = 0;
                                    Dn = 1;
                                    pfootf = 0xff;
                                }
                            }

                            dat = SREG[dat1] & 0x00ff;
                            //printf("---- dat = %04x \r\n",dat);
                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                            {
                                if ((dat == 0x1b) || (dat == 0x3))
                                {
                                    movfeed = 1;
                                }
                                dat = SREG[dat1 + 1];
                                x = dat & 0x000000ff;
                                y = ((U32)(dat >> 8)) & 0x000000ff;
                                if (x >= 0x80)
                                {
                                    x -= 0x80;
                                    g_dxpos = -x;
                                }
                                else
                                {
                                    g_dxpos = x;
                                }
                                if (y >= 0x80)
                                {
                                    y -= 0x80;
                                    g_dypos = -y;
                                }
                                else
                                {
                                    g_dypos = y;
                                }

                                // printf("g_modcordflag = %d  g_dxpos = %d g_dypos = %d g_modcordx = %d g_modcordy = %d \r\n", g_modcordflag, g_dxpos, g_dypos, g_modcordx, g_modcordy);
                                if (g_modcordflag)
                                {
                                    f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MODIFYADD, 0, movfeed);
                                }
                                else
                                {
                                    f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 0, movfeed);
                                }
                                g_modcordflag = 0;
                                movfeed = 0;
                                if (f == 0)
                                {
                                    break;
                                }
                            }
                            else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                            { //���ӹ���
                                excode = 0;
                                if (dat == 0x05)
                                    excode = 0x01; //����1
                                else if (dat == 0x0b)
                                    excode = 0x02; //����2
                                else if (dat == 0x0c)
                                    excode = 0x04; //����3
                                else if (dat == 0x0d)
                                    excode = 0x08; //����4
                                else
                                { //����5-8
                                    excode = (SREG[dat1] & 0xff00) >> 8;
                                    if (excode > 3)
                                        excode = 0;
                                    excode = (1 << (excode + 4));
                                }
                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                }
                            }
                            else if (dat == 0x3c)
                            {
                                g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                            }
                            else if (dat == 0x3d)
                            {
                                g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                            }
                            else if (dat == 0x3e)
                            {
                                g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                            }
                            sendExCodeCmd();

                            if (dat == 0x3a)
                            { // ��������ָ����㵱ǰ��������
                                lforceval = Mit_CailPadOffestLF(g_jog_point, 0);
                                g_lineforceValSetNow = g_lineforceValSet + lforceval + (S8)(SREG[dat1 + 1]);
                                //dlforceval = (S8)(SREG[dat1+1]);
                            }

                            g_jog_point++;
                            g_temp_oxpos += (S16)(g_dxpos);
                            g_temp_oypos += (S16)(g_dypos);
                            dlforceval = 0;
                            if (g_jog_point < tmp_feedsum)
                            {
                                dat = SREG[dat1 + 2] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                {
                                    if ((SREG[dat1 + 2] & 0xff00) != 0)
                                    {
                                        if (Up == 0)
                                        {
                                            pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //����jog_point֮ǰ����ѹ�Ÿ߶ȱ仯��
                                            temp_PfVal = PfootOffVal + pfootval + (S8)(SREG[dat1 + 2] >> 8);
                                            pfootval = (S8)(SREG[dat1 + 2] >> 8);
                                        }
                                        else
                                        {
                                            pfootval = (S8)(SREG[dat1 + 2] >> 8);
                                            temp_PfVal += pfootval;
                                        }
                                        g_temp_PfootOffVal = temp_PfVal;
                                    }
                                    dat = SREG[dat1 + 3];
                                    x = (U8)(dat);
                                    y = (U8)((U16)(dat >> 8));
                                    if (x >= 0x80)
                                    {
                                        x -= 0x80;
                                        g_dxpos = -x;
                                    }
                                    else
                                    {
                                        g_dxpos = x;
                                    }
                                    if (y >= 0x80)
                                    {
                                        y -= 0x80;
                                        g_dypos = -y;
                                    }
                                    else
                                    {
                                        g_dypos = y;
                                    }
                                    dat = SREG[dat1 + 2];
                                    vdat = SREG[dat1 + 3];
                                }
                                else if (dat == 0x3a)
                                {
                                    dlforceval = (S8)(SREG[dat1 + 3]);
                                }
                                else
                                {
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                    dat = SREG[dat1 + 2];
                                    vdat = SREG[dat1 + 3];
                                }
                            }
                            else
                            {
                                g_dxpos = 0;
                                g_dypos = 0;
                                dat = 0xff;
                            }

                            if (((dat & 0x00ff) == 0x1) || ((dat & 0x00ff) == 0x21) || ((dat & 0x00ff) == 0x41) || ((dat & 0x00ff) == 0x61))
                            {
                                g_pf_moddis = (S8)(dat >> 8);
                            }
                            else
                            {
                                g_pf_moddis = 0;
                            }

                            // disStatus(vxp2, vyp2, dat, vdat, dlforceval, Borpfdis, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);

                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, Borpfdis, rect[chRectTemp].colorT[1]);
                            vpointf = 1;
                            vpoint = g_jog_point;
                            dlforceval = 0;
                        }

                        g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                        g_pf_Set_Offest = 0;
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�

                        if (g_jog_point < max_feedsum)
                        {
                            if (Stflag == 0)
                            { //�ڻ�еԭ��λ��
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    EnUPflag = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                g_lineforceValSetNow = g_lineforceValSet;
                                g_curPinAng = 0;
                                g_curSetPara1 = 0;
                                g_curSetPara2 = 0;
                                g_modcordflag = 0;
                                g_temp_oxpos = g_patd.orgX;
                                g_temp_oypos = g_patd.orgY;

                                if (GoHome == 0)
                                {                                                                        //XYͬʱ�ƶ�
                                    Comm_RelationMove(g_temp_oxpos, g_temp_oypos, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                                }
                                else if (GoHome == 1)
                                {                                                             //X�Ȼ�ԭ��,Y���ԭ��
                                                                                              //���ƶ�Y,���ƶ�X
                                    Comm_RelationMove(0, g_temp_oypos, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                                    Wait_Move_End();                                          //�ȴ�ƽ̨�ƶ�����
                                    Comm_RelationMove(g_temp_oxpos, 0, MENU_MODIFYADD, 1, 1);
                                }
                                else
                                {
                                    //���ƶ�x,���ƶ�y
                                    Comm_RelationMove(g_temp_oxpos, 0, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                                    Wait_Move_End();                                          //�ȴ�ƽ̨�ƶ�����
                                    Comm_RelationMove(0, g_temp_oypos, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                                }

                                if (g_patd.orgX != 0 || g_patd.orgY != 0)
                                { //��ԭ��
                                    Stflag = 1;
                                    g_BorSTflag = Stflag;
                                    Borcmd = 0x5050; //��ԭ��
                                                     // dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,0,0,0,0);		//��ʾ��ԭ��
                                                     // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, Borpfdis, Color_Black);
                                    if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    else if (g_sta_end_flag == 1)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN2;
                                    }
                                    else
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, Borpfdis, rect[chRectTemp].colorT[1]);

                                    //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                    vpointf = 1;
                                    vpoint = g_jog_point;
                                }
                                else
                                { //�޴�ԭ�� ��ʾ��һ����������
                                    Stflag = 2;
                                    g_BorSTflag = Stflag;
                                    Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                    Borpooft += g_pfootd.base;
                                    if ((Borcmd & 0xff00) == 0xf100)
                                    { //����
                                        Bor_CalLFval((U16 *)SREG, g_patd.pat_len, g_jog_point + 1, (U8 *)&lfnow, &dlforceval);
                                    }
                                    //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                                    // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                    //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                    if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    else if (g_sta_end_flag == 1)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN2;
                                    }
                                    else
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);
                                    vpointf = 1;
                                    vpoint = g_jog_point;
                                    dlforceval = 0;
                                }
                            }
                            else if (Stflag == 1)
                            { //������ʾ��һ������ָ��
                                Stflag = 2;
                                g_BorSTflag = Stflag;
                                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                Borpooft += g_pfootd.base;
                                if ((Borcmd & 0xff00) == 0xf100)
                                { //����
                                    Bor_CalLFval((U16 *)SREG, g_patd.pat_len, g_jog_point + 1, (U8 *)&lfnow, &dlforceval);
                                }
                                //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                                // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);
                                vpointf = 1;
                                vpoint = g_jog_point;
                                dlforceval = 0;
                            }
                            else
                            {
                                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                Borpooft += g_pfootd.base;
                                if (Borpooft > PF_ABS_MAX)
                                    Borpooft = PF_ABS_MAX;
                                if (Borpooft < PF_ABS_MIN)
                                    Borpooft = PF_ABS_MIN;
                                if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                                { //����,����
                                    EnUPflag = 1;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    {
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        if ((Borcmd & 0xf000) == 0x2000)
                                        {
                                            break;
                                        }
                                    }
                                    if ((Borcmd & 0xf000) == 0x2000)
                                    {
                                        movfeed = 1;
                                    }
                                }
                                else if ((Borcmd & 0xf000) == 0x6000)
                                { //����
                                    if ((Dn == 1) && (EnUPflag == 1))
                                    {
                                        Up = 1;
                                        Dn = 0;
                                        Wait_Move_End();                  //�ȴ�ƽ̨�ƶ�����
                                        Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        break;
                                    }
                                    EnUPflag = 0;
                                }
                                else if ((Borcmd & 0xff00) == 0xf000)
                                {                    //��ѹ�ŵ���
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Dn == 1)
                                    {
                                        Up = 1;
                                        Dn = 0;
                                        Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        break;
                                    }

                                    if (Borpfdis > 0)
                                    { //��ѹ������
                                        f = Write_Cmd(C_PFEMOV, 1, Borpfdis);
                                    }
                                    else if (Borpfdis < 0)
                                    { //��ѹ���½�
                                        f = Write_Cmd(C_PFEMOV, 2, -Borpfdis);
                                    }
                                }
                                else if ((Borcmd & 0xff00) == 0xf400)
                                { //����
                                    EnUPflag = 1;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    { //ѹ��������,ѹ����
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                    }
                                    else
                                    {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                        Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                        Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        Up = 0;
                                        Dn = 1;
                                        pfootf = 0xff;
                                    }
                                }
                                else if ((Borcmd & 0xff00) == 0xfc00)
                                { //���ӹ���
                                    excode = Borcmd & 0xff;
                                    if (excode != 0)
                                    {
                                        OutputExcode1(excode, MENU_MODIFYADD, 0);
                                    }
                                }
                                else if ((Borcmd & 0xff00) == 0xf100)
                                { //����
                                    g_lineforceValSetNow = Borcmd & 0xff;
                                    if (g_lineforceValSetNow > 8)
                                        g_lineforceValSetNow = 8; //�������趨��Χ 20 -100
                                    g_lineforceValSetNow = (10 - g_lineforceValSetNow) * 10;
                                }
                                sendExCodeCmd();
                                g_jog_point++;
                                //Comm_Move(temp_oxpos1+g_dxpos,temp_oypos1+g_dypos,movfeed?80:50,MENU_MODIFYADD,1,movfeed);
                                Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 1, movfeed);
                                movfeed = 0;
                                //								//������ʾ��һ��ָ��
                                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                Borpooft += g_pfootd.base;
                                if ((Borcmd & 0xff00) == 0xf100)
                                { //����
                                    Bor_CalLFval((U16 *)SREG, g_patd.pat_len, g_jog_point + 1, (U8 *)&lfnow, &dlforceval);
                                }
                                // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);

                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                                vpointf = 1;
                                vpoint = g_jog_point;
                                dlforceval = 0;
                            }
                        }
                        else
                        { //�ص���еԭ��
                            g_lineforceValSetNow = g_lineforceValSet;
                            if (g_sta_end_flag == 0)
                            {
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    EnUPflag = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }

                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                //								if(Borpooft<0){		//ѹ�Ÿ߶ȸ�λ
                                //									f = Write_Cmd(C_PFEMOV, 1, -Borpfdis);
                                //								}else if(Borpooft>0){
                                //									f = Write_Cmd(C_PFEMOV, 2,Borpfdis);
                                //								}
                                Borpooft += g_pfootd.base;

                                f = Write_Cmd(C_PFOOTACTION, g_pfootd.base, 0); //ѹ�Ÿ߶�����Ϊ��ʼֵ
                                if ((g_patd.orgX != 0) || (g_patd.orgY != 0))
                                { //�ص���ԭ��

                                    Comm_RelationMove(g_patd.orgX - temp_oxpos1, g_patd.orgY - temp_oypos1, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��

                                    g_jog_point = 0;
                                    Stflag = 1;
                                    g_BorSTflag = Stflag;
                                    Borcmd = 0x5050;
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                }
                                else
                                { //�ص�ԭ��
                                    if (GoHome == 0)
                                    {                                                                            //XYͬʱ�ƶ�
                                        Comm_RelationMove(-(temp_oxpos1), -(temp_oypos1), MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                                    }
                                    else if (GoHome == 1)
                                    {
                                        Comm_RelationMove(-(temp_oxpos1), 0, MENU_MODIFYADD, 1, 1);
                                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                        Comm_RelationMove(0, -(temp_oypos1), MENU_MODIFYADD, 1, 1);
                                    }
                                    else
                                    { //Y�Ȼ�ԭ��,X���ԭ��
                                        Comm_RelationMove(0, -(temp_oypos1), MENU_MODIFYADD, 1, 1);
                                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                        Comm_RelationMove(-(temp_oxpos1), 0, MENU_MODIFYADD, 1, 1);
                                    }
                                    Stflag = 0;
                                    g_BorSTflag = Stflag;
                                    g_jog_point = 0;
                                    Borcmd = 0x2000;
                                    g_dxpos = g_patd.orgX;
                                    g_dypos = g_patd.orgY;
                                }

                                excode = g_ExCode;
                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                }

                                g_curPinAng = 0;
                                g_curSetPara1 = 0;
                                g_curSetPara2 = 0;
                                sendExCodeCmd();

                                // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, 0, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, 0, rect[chRectTemp].colorT[1]);
                                vpointf = 1;
                                vpoint = g_jog_point;
                            }
                        }
                    }

                    break;
                case KEY_MODIADD_MDOWN:
                case KEY_MODIADD_JOGDOWN:

                    if ((!(S_CLAMP)) && (P_OST == 0))
                    {
                        pno = MENU_RWHELP;
                        hleppara = 13;
                        poldno[pnoindex] = MENU_MODIFYADD;
                        pnoindex++;
                        break;
                    }
                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                        g_updata_pf_valflg = 1;

                        if (g_jog_point > min_feedsum)
                        {
                            dat1 = g_patd.pat_add + g_jog_point * 2;
                            if (dat1 > g_patd.pat_add)
                            {

                                dat = SREG[dat1 - 2] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1))
                                {
                                    if ((SREG[dat1 - 2] & 0xff00) != 0)
                                    {
                                        pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //����jog_point֮ǰ����ѹ�Ÿ߶ȱ仯��
                                        temp_PfVal = PfootOffVal + pfootval - (S8)(SREG[dat1 - 2] >> 8);
                                        pfootval = (S8)(SREG[dat1 - 2] >> 8);
                                    }
                                }
                                else if ((dat == 0x1b) || (dat == 0x03))
                                { //����
                                    EnUPflag = 1;
                                    if (Up)
                                    {
                                        Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        Up = 0;
                                        Dn = 1;
                                        break;
                                    }
                                }

                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x03))
                                {
                                    if ((SREG[dat1 - 2] & 0xff00) == 0)
                                    {
                                        g_temp_PfootOffVal = temp_PfVal;
                                        if (uplock == 0)
                                        {
                                            if (old_PfootOffVal != temp_PfVal)
                                            {
                                                temp_PfVal = old_PfootOffVal;
                                                g_temp_PfootOffVal = temp_PfVal;
                                            }
                                        }
                                    }
                                }

                                if (uplock == 0)
                                {
                                    uplock = 1;
                                }
                            }
                            if (((SREG[dat1 - 2] & 0x00ff) == 0x1) || ((SREG[dat1 - 2] & 0x00ff) == 0x21) || ((SREG[dat1 - 2] & 0x00ff) == 0x41) || ((SREG[dat1 - 2] & 0x00ff) == 0x61))
                            {

                                if ((old_PfootOffVal != temp_PfVal) || Dn)
                                {
                                    f = Write_Cmd(C_PFOOTACTION, old_PfootOffVal, 0);
                                    Read_Status();
                                    //for(i=0;i<RDTIMES;i++){
                                    //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                                    //	if((DREG[D_CMDSTATUS]) == 100)break;
                                    //	DelayMs(WAITTIME);
                                    //}
                                    if (Dn && ((old_PfootOffVal != temp_PfVal) || (EnUPflag == 1)))
                                    {
                                        Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�

                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        Up = 1;
                                        Dn = 0;
                                        if (EnUPflag == 1)
                                            break;
                                    }

                                    if (old_PfootOffVal != temp_PfVal)
                                    {
                                        old_PfootOffVal = temp_PfVal;
                                        if (pfootval < 0)
                                        {
                                            f = Write_Cmd(C_PFEMOV, 1, (-pfootval));
                                        }
                                        else
                                        {
                                            f = Write_Cmd(C_PFEMOV, 2, pfootval);
                                        }
                                        Read_Status();
                                        //for(i=0;i<RDTIMES;i++){
                                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                                        //	DelayMs(WAITTIME);
                                        //}
                                    }
                                }

                                EnUPflag = 0;
                            }
                            else if (((SREG[dat1 - 2] & 0x00ff) == 0x07))
                            { //����
                                EnUPflag = 1;
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Up == 1)
                                { //ѹ��������,ѹ����
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                else
                                {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    Up = 0;
                                    Dn = 1;
                                    pfootf = 0xff;
                                }
                            }

                            dat = SREG[dat1 - 2] & 0x00ff;
                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                            {
                                if ((dat == 0x1b) || (dat == 0x3))
                                {
                                    movfeed = 1;
                                }
                                dat = SREG[dat1 - 1];
                                x = dat & 0x000000ff;
                                y = ((U32)(dat >> 8)) & 0x000000ff;
                                if (x >= 0x80)
                                {
                                    x -= 0x80;
                                    g_dxpos = x;
                                }
                                else
                                {
                                    g_dxpos = -x;
                                }
                                if (y >= 0x80)
                                {
                                    y -= 0x80;
                                    g_dypos = y;
                                }
                                else
                                {
                                    g_dypos = -y;
                                }

                                if (g_modcordflag)
                                {
                                    f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MODIFYADD, 0, movfeed);
                                }
                                else
                                {
                                    f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 0, movfeed);
                                }
                                g_modcordflag = 0;
                                movfeed = 0;
                                if (f == 0)
                                {
                                    break;
                                }
                            }
                            else
                            {
                                g_dxpos = 0;
                                g_dypos = 0;

                                if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                { //���ӹ���
                                    excode = 0;
                                    if (dat == 0x05)
                                        excode = 0x01; //����1
                                    else if (dat == 0x0b)
                                        excode = 0x02; //����2
                                    else if (dat == 0x0c)
                                        excode = 0x04; //����3
                                    else if (dat == 0x0d)
                                        excode = 0x08; //����4
                                    else
                                    { //����5-8
                                        excode = (SREG[dat1 - 2] & 0xff00) >> 8;
                                        if (excode > 3)
                                            excode = 0;
                                        excode = (1 << (excode + 4));
                                    }
                                    if (excode != 0)
                                    {
                                        OutputExcode1(excode, MENU_MODIFYADD, 0);
                                    }
                                }
                                else if (dat == 0x3c)
                                {
                                    g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 0);
                                }
                                else if (dat == 0x3d)
                                {
                                    g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 1);
                                }
                                else if (dat == 0x3e)
                                {
                                    g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 2);
                                }

                                sendExCodeCmd();
                            }

                            if (dat == 0x3a)
                            { // ��������ָ����㵱ǰ��������
                                lforceval = Mit_CailPadOffestLF(g_jog_point, 0);
                                g_lineforceValSetNow = g_lineforceValSet + lforceval - (S8)(SREG[dat1 - 1]);
                                dlforceval = -(S8)(SREG[dat1 - 1]);
                            }
                            g_jog_point--;
                            g_temp_oxpos += (S16)(g_dxpos);
                            g_temp_oypos += (S16)(g_dypos);

                            g_dxpos = -g_dxpos;
                            g_dypos = -g_dypos;

                            dat = SREG[dat1 - 2];
                            vdat = SREG[dat1 - 1];
                            if (((dat & 0x00ff) == 0x1) || ((dat & 0x00ff) == 0x21) || ((dat & 0x00ff) == 0x41) || ((dat & 0x00ff) == 0x61))
                            {
                                if ((dat & 0xff00) != 0)
                                {
                                    g_pf_moddis = (S8)(dat >> 8);
                                }
                                else
                                {
                                    g_pf_moddis = 0;
                                }
                            }
                            else
                            {
                                g_pf_moddis = 0;
                            }

                            // disStatus(vxp2, vyp2, dat, vdat, dlforceval, Borpfdis, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, Borpfdis, rect[chRectTemp].colorT[1]);

                            vpointf = 1;
                            vpoint = g_jog_point;
                            dlforceval = 0;
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW) //�ֵ�
                    {
                        if (g_jog_point > min_feedsum)
                        {
                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            Borpooft += g_pfootd.base;
                            if (Borpooft > PF_ABS_MAX)
                                Borpooft = PF_ABS_MAX;
                            if (Borpooft < PF_ABS_MIN)
                                Borpooft = PF_ABS_MIN;

                            if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                            { //����,����
                                EnUPflag = 1;
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                    if ((Borcmd & 0xf000) == 0x2000)
                                    {
                                        break;
                                    }
                                }

                                if ((Borcmd & 0xf000) == 0x2000)
                                {
                                    movfeed = 1;
                                }
                            }
                            else if ((Borcmd & 0xf000) == 0x6000)
                            { //����
                                if ((Dn == 1) && (EnUPflag == 1))
                                {
                                    Up = 1;
                                    Dn = 0;
                                    Wait_Move_End();                  //�ȴ�ƽ̨�ƶ�����
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                    break;
                                }
                                EnUPflag = 0;
                            }
                            else if ((Borcmd & 0xff00) == 0xf000)
                            {                    //��ѹ�ŵ���
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Dn == 1)
                                {
                                    Up = 1;
                                    Dn = 0;
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                    break;
                                }
                                if (Borpfdis < 0)
                                { //��ѹ������
                                    f = Write_Cmd(C_PFEMOV, 1, -Borpfdis);
                                }
                                else if (Borpfdis > 0)
                                { //��ѹ���½�
                                    f = Write_Cmd(C_PFEMOV, 2, Borpfdis);
                                }
                            }
                            else if ((Borcmd & 0xff00) == 0xf400)
                            { //����
                                EnUPflag = 1;
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Up == 1)
                                { //ѹ��������,ѹ����
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                else
                                {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    Up = 0;
                                    Dn = 1;
                                    pfootf = 0xff;
                                }
                            }
                            else if ((Borcmd & 0xff00) == 0xfc00)
                            { //���ӹ���
                                excode = Borcmd & 0xff;
                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                }
                            }
                            else if ((Borcmd & 0xff00) == 0xec00 || (Borcmd & 0xff00) == 0xea00 || (Borcmd & 0xff00) == 0xeb00)
                            { //��Ƕ�,����һ,���ö�
                                if (g_jog_point)
                                    Bor_feedInfo(g_jog_point - 1, g_patd.pat_len, &tBorcmd, &ttemp_oxpos1, &ttemp_oypos1, (S16 *)&tdxpos, (S16 *)&tdypos, (U8 *)&toldspeed, &tBorpooft, &tBorpfdis);
                                else
                                {
                                    g_pointPinAng = 0;
                                    g_pointSetPara1 = 0;
                                    g_pointSetPara2 = 0;
                                }
                            }
                            else if ((Borcmd & 0xff00) == 0xf100)
                            { //����
                                Bor_CalLFval((U16 *)SREG, g_patd.pat_len, g_jog_point, (U8 *)&g_lineforceValSetNow, &dlforceval);
                            }
                            sendExCodeCmd();
                            g_jog_point--;
                            //Comm_Move(temp_oxpos1,temp_oypos1,movfeed?80:50,MENU_MODIFYADD,1,movfeed);
                            Comm_RelationMove(-g_dxpos, -g_dypos, MENU_MODIFYADD, 1, movfeed);
                            // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                            vpointf = 1;
                            vpoint = g_jog_point;
                            movfeed = 0;
                            dlforceval = 0;
                        }
                        else if ((Stflag == 2) && (g_sta_end_flag == 0) && ((g_patd.orgX != 0) || (g_patd.orgY != 0)))
                        { //��ԭ��
                            Stflag = 1;
                            g_BorSTflag = Stflag;
                            Borcmd = 0x5050; //��ԭ��

                            //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,0,0,0,0);
                            // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, 0, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, 0, rect[chRectTemp].colorT[1]);

                            vpointf = 1;
                            vpoint = g_jog_point;
                        }
                        else if ((g_sta_end_flag == 0) && ((Stflag == 1) || ((Stflag == 2) && (g_patd.orgX == 0) && (g_patd.orgY == 0))))
                        { //�ص���еԭ��
                            //Go_Home(MENU_MODIFYADD);
                            Stflag = 0;
                            g_BorSTflag = Stflag;
                            g_jog_point = 0;
                            Up = 0;
                            Dn = 1;
                            Borcmd = 0x2000;
                            g_dxpos = g_patd.orgX;
                            g_dypos = g_patd.orgY;
                            if (GoHome == 0)
                            {                                                                //XYͬʱ�ƶ�
                                Comm_RelationMove(-g_dxpos, -g_dypos, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                            }
                            else if (GoHome == 1)
                            {
                                Comm_RelationMove(-g_dxpos, 0, MENU_MODIFYADD, 1, 1);
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                Comm_RelationMove(0, -g_dypos, MENU_MODIFYADD, 1, 1);
                            }
                            else
                            { //Y�Ȼ�ԭ��,X���ԭ��
                                Comm_RelationMove(0, -g_dypos, MENU_MODIFYADD, 1, 1);
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                Comm_RelationMove(-g_dxpos, 0, MENU_MODIFYADD, 1, 1);
                            }
                            // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, 0, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, 0, rect[chRectTemp].colorT[1]);

                            vpointf = 1;
                            vpoint = g_jog_point;
                            g_curPinAng = 0;
                            g_curSetPara1 = 0;
                            g_curSetPara2 = 0;
                            sendExCodeCmd();
                        }
                        else if (g_sta_end_flag == 0)
                        { //�ص�������λ��
                            g_jog_point = max_feedsum;
                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis); //printf("dn Borcmd = %04x jog_point = %d  temp_oxpos = %d temp_oypos = %d\r\n",Borcmd,jog_point,temp_oxpos,temp_oypos);
                            Borpooft += g_pfootd.base;
                            Bor_CalLFval((U16 *)SREG, g_patd.pat_len, g_jog_point, (U8 *)&g_lineforceValSetNow, &dlforceval);
                            if (Borpooft > PF_ABS_MAX)
                                Borpooft = PF_ABS_MAX;
                            if (Borpooft < PF_ABS_MIN)
                                Borpooft = PF_ABS_MIN;
                            // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, 0, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);

                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, 0, rect[chRectTemp].colorT[1]);
                            vpointf = 1;
                            vpoint = g_jog_point;
                            Stflag = 2;
                            g_BorSTflag = Stflag;
                            dlforceval = 0;
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                            if (Up == 1)
                            {
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                            }
                            f = Write_Cmd(C_PFOOTACTION, Borpooft, 0); //ѹ�Ÿ߶�����Ϊ����ֵ
                            Borcmd = 0x2000;
                            if (GoHome == 0)
                            {                                                                                          //XYͬʱ�ƶ�
                                Comm_RelationMove(temp_oxpos1 + g_dxpos, temp_oypos1 + g_dypos, MENU_MODIFYADD, 1, 1); //�ƶ�����ԭ��λ��
                            }
                            else if (GoHome == 1)
                            { //X�Ȼ�ԭ��,Y���ԭ��
                                //���ƶ�Y,���ƶ�X
                                Comm_RelationMove(0, temp_oypos1 + g_dypos, MENU_MODIFYADD, 1, 1);
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                Comm_RelationMove(temp_oxpos1 + g_dxpos, 0, MENU_MODIFYADD, 1, 1);
                            }
                            else
                            {
                                //���ƶ�x,���ƶ�y
                                Comm_RelationMove(temp_oxpos1 + g_dxpos, 0, MENU_MODIFYADD, 1, 1);
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                Comm_RelationMove(0, temp_oypos1 + g_dypos, MENU_MODIFYADD, 1, 1);
                            }
                            excode = g_ExCode ^ g_tExCode;
                            if (excode != 0)
                            {
                                OutputExcode1(excode, MENU_MODIFYADD, 0);
                            }
                            sendExCodeCmd();
                        }
                    }

                    break;

                case KEY_MODIADD_SKIPJOGUP:
                    if ((!(S_CLAMP)) && (P_OST == 0))
                    {
                        pno = MENU_RWHELP;
                        hleppara = 13;
                        poldno[pnoindex] = MENU_MODIFYADD;
                        pnoindex++;
                        break;
                    }
                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        if (runf1)
                            runf1 = 0;
                        else if (runf2)
                            runf2 = 0;
                        else if (g_jog_point < max_feedsum)
                        {
                            if ((g_jog_point + g_jognumber) < max_feedsum)
                            {
                                runf1 = g_jog_point + g_jognumber;
                            }
                            else
                            {
                                runf1 = max_feedsum;
                            }
                            if (Up == 1)
                            {
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                            }
                            if (g_jogmethod == 0)
                            {
                                dat1 = g_patd.pat_add + g_jog_point * 2;
                                g_dxpos = 0;
                                g_dypos = 0;
                                excode = 0;
                                for (ii = g_jog_point; (ii < runf1) && (excode == 0); ii++)
                                {
                                    dat = SREG[dat1] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        if ((dat == 0x1b) || (dat == 0x3))
                                        {
                                            movfeed = 1;
                                        }
                                        dat = SREG[dat1 + 1];
                                        x = dat & 0x000000ff;
                                        y = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (x >= 0x80)
                                        {
                                            x -= 0x80;
                                            g_dxpos -= x;
                                        }
                                        else
                                        {
                                            g_dxpos += x;
                                        }
                                        if (y >= 0x80)
                                        {
                                            y -= 0x80;
                                            g_dypos -= y;
                                        }
                                        else
                                        {
                                            g_dypos += y;
                                        }
                                    }
                                    else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                    { //���ӹ���
                                        if (dat == 0x05)
                                            excode = 0x01; //����1
                                        else if (dat == 0x0b)
                                            excode = 0x02; //����2
                                        else if (dat == 0x0c)
                                            excode = 0x04; //����3
                                        else if (dat == 0x0d)
                                            excode = 0x08; //����4
                                        else
                                        { //����5-8
                                            excode = (SREG[dat1] & 0xff00) >> 8;
                                            if (excode > 3)
                                                excode = 0;
                                            excode = (1 << (excode + 4));
                                        }
                                        if (excode != 0)
                                        {
                                            if (g_jogfunction || ((g_UdZhouFlag == 3) && (excode & 0x80)))
                                            { //������ӹ���,����
                                            }
                                            else
                                            {
                                                excode = 0;
                                            }
                                        }
                                    }
                                    else if (dat == 0x3c)
                                    {
                                        g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                                    }
                                    else if (dat == 0x3d)
                                    {
                                        g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                                    }
                                    else if (dat == 0x3e)
                                    {
                                        g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                                    }
                                    dat1 += 2;
                                }
                                if (g_modcordflag)
                                {
                                    f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MODIFYADD, 0, movfeed);
                                }
                                else
                                {
                                    f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 0, movfeed);
                                }
                                g_modcordflag = 0;

                                movfeed = 0;
                                if (f == 0)
                                {
                                    break;
                                }
                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                    runf1 = ii;
                                }
                                sendExCodeCmd();
                                g_jog_point = runf1;
                                runf1 = 0;
                                g_temp_oxpos += (S16)(g_dxpos);
                                g_temp_oypos += (S16)(g_dypos);
                                if (g_jog_point < tmp_feedsum)
                                {
                                    dat = SREG[dat1] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[dat1 + 1];
                                        x = (U8)(dat);
                                        y = (U8)((U16)(dat >> 8));
                                        if (x >= 0x80)
                                        {
                                            x -= 0x80;
                                            g_dxpos = -x;
                                        }
                                        else
                                        {
                                            g_dxpos = x;
                                        }
                                        if (y >= 0x80)
                                        {
                                            y -= 0x80;
                                            g_dypos = -y;
                                        }
                                        else
                                        {
                                            g_dypos = y;
                                        }
                                        dat = SREG[dat1];
                                        vdat = SREG[dat1 + 1];
                                    }
                                    else
                                    {
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                        dat = SREG[dat1];
                                        vdat = SREG[dat1 + 1];
                                    }
                                }
                                else
                                {
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                    dat = 0xff;
                                }
                                // disStatus(vxp2, vyp2, dat, vdat, dlforceval, Borpfdis, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, Borpfdis, rect[chRectTemp].colorT[1]);
                                vpointf = 1;
                                vpoint = g_jog_point;
                                g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                                g_pf_absval += g_pfootd.base;
                                if (g_pf_absval != old_PfootOffVal)
                                {
                                    temp_PfVal = g_pf_absval;
                                    old_PfootOffVal = g_pf_absval;
                                    f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0); //������ѹ�Ÿ߶�
                                    Read_Status();
                                    pfootval = 0;
                                    Up = 0;
                                    Dn = 1;
                                    EnUPflag = 1;
                                }
                            }
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        if (runf1)
                            runf1 = 0;
                        else if (runf2)
                            runf2 = 0;
                        else if (g_jog_point < max_feedsum)
                        {
                            if (Stflag == 0)
                            {
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    EnUPflag = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                g_temp_oxpos = g_patd.orgX;
                                g_temp_oypos = g_patd.orgY;
                                g_ExCode = 0;
                                g_curExCode = 0;
                                g_curPinAng = 0;
                                g_curSetPara1 = 0;
                                g_curSetPara2 = 0;
                                if (GoHome == 0)
                                { //XYͬʱ��?
                                    Comm_RelationMove(g_temp_oxpos, g_temp_oypos, MENU_MODIFYADD, 1, 1);
                                }
                                else if (GoHome == 1)
                                { //X�Ȼ�ԭ��,Y���ԭ��
                                    //���ƶ�Y,���ƶ�X
                                    Comm_RelationMove(0, g_temp_oypos, MENU_MODIFYADD, 1, 1);
                                    Wait_Move_End();
                                    Comm_RelationMove(g_temp_oxpos, 0, MENU_MODIFYADD, 1, 1);
                                }
                                else
                                {
                                    //���ƶ�x,���ƶ�y
                                    Comm_RelationMove(g_temp_oxpos, 0, MENU_MODIFYADD, 1, 1);
                                    Wait_Move_End();
                                    Comm_RelationMove(0, g_temp_oypos, MENU_MODIFYADD, 1, 1);
                                }

                                if (g_patd.orgX != 0 || g_patd.orgY != 0)
                                { //��ԭ��
                                    Stflag = 1;
                                    g_BorSTflag = Stflag;
                                    Borcmd = 0x5050; //��ԭ��

                                    // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, 0, Color_Black);
                                    //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                    if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    else if (g_sta_end_flag == 1)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN2;
                                    }
                                    else
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, 0, rect[chRectTemp].colorT[1]);

                                    vpointf = 1;
                                    vpoint = g_jog_point;
                                }
                                else
                                { //�޴�ԭ�� ��ʾ��һ����������
                                    Stflag = 2;
                                    g_BorSTflag = Stflag;
                                    Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                    Borpooft += g_pfootd.base;

                                    //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                                    // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                    //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                    if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    else if (g_sta_end_flag == 1)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN2;
                                    }
                                    else
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                                    vpointf = 1;
                                    vpoint = g_jog_point;
                                }
                            }
                            else
                            {
                                if ((g_jog_point + g_jognumber) < max_feedsum)
                                {
                                    runf1 = g_jog_point + g_jognumber;
                                }
                                else
                                {
                                    runf1 = max_feedsum;
                                }
                                if (g_jogmethod == 0)
                                {
                                    if (Up == 1)
                                    {
                                        Up = 0;
                                        Dn = 1;
                                        Wait_Move_End();                  //�ȴ�ƽ̨�ƶ�����
                                        Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    }
                                    Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&tempdx, (S16 *)&tempdy, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                    temp_oxpos1 += tempdx;
                                    temp_oypos1 += tempdy;
                                    Bor_feedInfo(runf1, g_patd.pat_len, &Borcmd, &temp_oxpos2, &temp_oypos2, (S16 *)&tempdx, (S16 *)&tempdy, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                    temp_oxpos2 += tempdx;
                                    temp_oypos2 += tempdy;
                                    //dis_data2(xx,yy,runf1,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,temp_oxpos2-temp_oxpos1,temp_oypos2-temp_oypos1,dlforceval);
                                    // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                    //newDrawPoint(g_jogonflag?2:1,runf1,scale,xo,yo,0);
                                    if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    else if (g_sta_end_flag == 1)
                                    {
                                        chRectTemp = RECT_MODIADD_PIN2;
                                    }
                                    else
                                    {
                                        chRectTemp = RECT_MODIADD_PIN1;
                                    }
                                    disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                                    vpointf = 1;
                                    vpoint = runf1;
                                    f = Comm_RelationMove(temp_oxpos2 - temp_oxpos1, temp_oypos2 - temp_oypos1, MENU_MODIFYADD, 1, 0);
                                    if (f == 0)
                                    {
                                        break;
                                    }
                                    excode = g_ExCode ^ g_tExCode;
                                    if (excode != 0)
                                    {
                                        OutputExcode1(excode, MENU_MODIFYADD, 0);
                                    }
                                    sendExCodeCmd();
                                    g_jog_point = runf1;
                                    runf1 = 0;

                                    Borpooft += g_pfootd.base;
                                    if (Borpooft > PF_ABS_MAX)
                                        Borpooft = PF_ABS_MAX;
                                    if (Borpooft < PF_ABS_MIN)
                                        Borpooft = PF_ABS_MIN;
                                    f = Write_Cmd(C_PFOOTACTION, Borpooft, 0); //������ѹ�Ÿ߶�
                                    Read_Status();
                                }
                            }
                        }
                    }

                    break;
                case KEY_MODIADD_SKIPJOGDOWN:
                    if ((!(S_CLAMP)) && (P_OST == 0))
                    {
                        pno = MENU_RWHELP;
                        hleppara = 13;
                        poldno[pnoindex] = MENU_MODIFYADD;
                        pnoindex++;
                        break;
                    }

                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        if (runf1)
                            runf1 = 0;
                        else if (runf2)
                            runf2 = 0;
                        else if (g_jog_point > min_feedsum)
                        {
                            if ((g_jog_point - min_feedsum) > g_jognumber)
                            {
                                runf2 = g_jog_point - g_jognumber + 1;
                            }
                            else
                            {
                                runf2 = min_feedsum + 1;
                            }
                            if (Up == 1)
                            {
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                            }
                            if (g_jogmethod == 0)
                            {
                                dat1 = g_patd.pat_add + g_jog_point * 2;
                                g_dxpos = 0;
                                g_dypos = 0;
                                excode = 0;
                                for (ii = g_jog_point; (ii >= runf2) && (excode == 0); ii--)
                                {
                                    dat = SREG[dat1 - 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        if ((dat == 0x1b) || (dat == 0x3))
                                        {
                                            movfeed = 1;
                                        }
                                        dat = SREG[dat1 - 1];
                                        x = dat & 0x000000ff;
                                        y = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (x >= 0x80)
                                        {
                                            x -= 0x80;
                                            g_dxpos += x;
                                        }
                                        else
                                        {
                                            g_dxpos -= x;
                                        }
                                        if (y >= 0x80)
                                        {
                                            y -= 0x80;
                                            g_dypos += y;
                                        }
                                        else
                                        {
                                            g_dypos -= y;
                                        }
                                    }
                                    else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                    { //���ӹ���
                                        if (dat == 0x05)
                                            excode = 0x01; //����1
                                        else if (dat == 0x0b)
                                            excode = 0x02; //����2
                                        else if (dat == 0x0c)
                                            excode = 0x04; //����3
                                        else if (dat == 0x0d)
                                            excode = 0x08; //����4
                                        else
                                        { //����5-8
                                            excode = (SREG[dat1 - 2] & 0xff00) >> 8;
                                            if (excode > 3)
                                                excode = 0;
                                            excode = (1 << (excode + 4));
                                        }
                                        if (excode != 0)
                                        { //������ӹ��ܻ���
                                            if (g_jogfunction || ((g_UdZhouFlag == 3) && (excode & 0x80)))
                                            {
                                            }
                                            else
                                            {
                                                excode = 0;
                                            }
                                        }
                                    }
                                    dat1 -= 2;
                                }
                                if (g_modcordflag)
                                {
                                    f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MODIFYADD, 0, movfeed);
                                }
                                else
                                {
                                    f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 0, movfeed);
                                }
                                g_modcordflag = 0;
                                movfeed = 0;
                                if (f == 0)
                                {
                                    break;
                                }

                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                    runf2 = ii;
                                }
                                g_temp_oxpos += (S16)(g_dxpos);
                                g_temp_oypos += (S16)(g_dypos);

                                g_jog_point = runf2 - 1;
                                runf2 = 0;
                                g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 0);
                                g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 1);
                                g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 2);
                                sendExCodeCmd();
                                if (g_jog_point < tmp_feedsum)
                                {
                                    dat = SREG[dat1] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[dat1 + 1];
                                        x = (U8)(dat);
                                        y = (U8)((U16)(dat >> 8));
                                        if (x >= 0x80)
                                        {
                                            x -= 0x80;
                                            g_dxpos = -x;
                                        }
                                        else
                                        {
                                            g_dxpos = x;
                                        }
                                        if (y >= 0x80)
                                        {
                                            y -= 0x80;
                                            g_dypos = -y;
                                        }
                                        else
                                        {
                                            g_dypos = y;
                                        }
                                        dat = SREG[dat1];
                                        vdat = SREG[dat1 + 1];
                                    }
                                    else
                                    {
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                        dat = SREG[dat1];
                                        vdat = SREG[dat1 + 1];
                                    }
                                }
                                else
                                {
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                    dat = 0xff;
                                }
                                // disStatus(vxp2, vyp2, dat, vdat, dlforceval, Borpfdis, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, Borpfdis, rect[chRectTemp].colorT[1]);

                                vpointf = 1;
                                vpoint = g_jog_point;
                                g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                                g_pf_absval += g_pfootd.base;
                                if (g_pf_absval != old_PfootOffVal)
                                {
                                    temp_PfVal = g_pf_absval;
                                    old_PfootOffVal = g_pf_absval;
                                    f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0); //������ѹ�Ÿ߶�
                                    Read_Status();
                                    pfootval = 0;
                                    Up = 0;
                                    Dn = 1;
                                    EnUPflag = 1;
                                }
                            }
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    {
                        if (runf1)
                            runf1 = 0;
                        else if (runf2)
                            runf2 = 0;
                        else if (g_jog_point > min_feedsum)
                        {
                            if ((g_jog_point - min_feedsum) > g_jognumber)
                            {
                                runf2 = g_jog_point - g_jognumber + 1;
                            }
                            else
                            {
                                runf2 = min_feedsum + 1;
                            }
                            if (g_jogmethod == 0)
                            {
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    Wait_Move_End();                  //�ȴ�ƽ̨�ƶ�����
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }

                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&tempdx, (S16 *)&tempdy, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                temp_oxpos1 += tempdx;
                                temp_oypos1 += tempdy;
                                Bor_feedInfo(runf2 - 1, g_patd.pat_len, &Borcmd, &temp_oxpos2, &temp_oypos2, (S16 *)&tempdx, (S16 *)&tempdy, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                temp_oxpos2 += tempdx;
                                temp_oypos2 += tempdy;

                                //dis_data2(xx,yy,runf2,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,temp_oxpos2-temp_oxpos1,temp_oypos2-temp_oypos1,dlforceval);
                                // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,runf2,scale,xo,yo,0);
                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                                vpointf = 1;
                                vpoint = runf2;
                                f = Comm_RelationMove(temp_oxpos2 - temp_oxpos1, temp_oypos2 - temp_oypos1, MENU_MODIFYADD, 1, 0);
                                if (f == 0)
                                {
                                    break;
                                }

                                excode = g_ExCode ^ g_tExCode;
                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                }
                                sendExCodeCmd();
                                g_jog_point = runf2 - 1;
                                runf2 = 0;
                                Borpooft += g_pfootd.base;
                                if (Borpooft > PF_ABS_MAX)
                                    Borpooft = PF_ABS_MAX;
                                if (Borpooft < PF_ABS_MIN)
                                    Borpooft = PF_ABS_MIN;
                                f = Write_Cmd(C_PFOOTACTION, Borpooft, 0); //������ѹ�Ÿ߶�
                                Read_Status();
                            }
                        }
                    }

                    break;

                case KEY_MODIADD_AUTO:
                    AutoDoSplit = 1;
                    g_jogmode = 3;
                    dis_updata1 = 1;
                    break;

                case KEY_MODIADD_IMG:
                    if (beeponf > 1)
                    {
                        if ((absInt(keyxpct - okeyxpct) <= 10) && (absInt(keyypct - okeyypct) <= 10) && (absInt(keyxpct - stkeyxpct) <= 10) && (absInt(keyypct - stkeyypct) <= 10) && (absInt(okeyxpct - stkeyxpct) <= 10) && (absInt(okeyypct - stkeyypct) <= 10) && (beeponf == 15))
                        {
                            xo = ORG_X;
                            yo = ORG_Y;
                            scale = 0;
                            vflag = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            Beep_On(BELLT);
                        }
                        else
                        {
                            xo += (keyxpct - okeyxpct);
                            yo += (keyypct - okeyypct);
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            if (vflag == 1)
                                vflag = 2;
                        }
                    }
                    else
                    {
                        okeyxpct = keyxpct;
                        okeyypct = keyypct;
                        stkeyxpct = keyxpct;
                        stkeyypct = keyypct;
                    }
                    dis_updata4 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_MODIADD_IMGI:
                    scale += 50;
                    dis_updata4 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_MODIADD_IMGO:
                    if (scale)
                    {
                        scale -= 50;
                        dis_updata4 = 1;
                    }
                    else
                    {
                        if (bResetCaonvas)
                        {
                            bResetCaonvas = 0;
                            scale = 0;
                            xo = patConvas.conOrgX;
                            yo = patConvas.conOrgY;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            dis_updata4 = 2;
                        }
                    }

                    break;
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {

            if (keyon == KEY_MODIADD_SCRNSVR || keyon == KEY_MODIADD_IMG) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(rect[0].index[0]) * TitleLengthMax,
                         SheetPage, rect[0].index[0], rect[0].colorT[0]); //��ʾ�ػ������
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        if (runf1)
        {
            if (F_FILETYPE == PAT_MIT)
            { //����
                if (g_jog_point < max_feedsum)
                {
                    if (g_jog_point < runf1)
                    {
                        dat1 = g_patd.pat_add + g_jog_point * 2;
                        dat = SREG[dat1] & 0x00ff;
                        if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                        {
                            if ((dat == 0x1b) || (dat == 0x3))
                            {
                                movfeed = 1;
                            }

                            dat = SREG[dat1 + 1];
                            x = dat & 0x000000ff;
                            y = ((U32)(dat >> 8)) & 0x000000ff;
                            if (x >= 0x80)
                            {
                                x -= 0x80;
                                g_dxpos = -x;
                                g_temp_oxpos -= (S16)(x);
                            }
                            else
                            {
                                g_dxpos = x;
                                g_temp_oxpos += (S16)(x);
                            }
                            if (y >= 0x80)
                            {
                                y -= 0x80;
                                g_dypos = -y;
                                g_temp_oypos -= (S16)(y);
                            }
                            else
                            {
                                g_dypos = y;
                                g_temp_oypos += (S16)(y);
                            }
                            if (g_modcordflag)
                            {
                                f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MODIFYADD, 0, movfeed);
                            }
                            else
                            {
                                f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 0, movfeed);
                            }
                            g_modcordflag = 0;
                            movfeed = 0;
                            if (f == 0)
                            {
                                break;
                            }
                        }
                        else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                        { //���ӹ���
                            excode = 0;
                            if (dat == 0x05)
                                excode = 0x01; //����1
                            else if (dat == 0x0b)
                                excode = 0x02; //����2
                            else if (dat == 0x0c)
                                excode = 0x04; //����3
                            else if (dat == 0x0d)
                                excode = 0x08; //����4
                            else
                            { //����5-8
                                excode = (SREG[dat1] & 0xff00) >> 8;
                                if (excode > 3)
                                    excode = 0;
                                excode = (1 << (excode + 4));
                            }
                            if (excode != 0)
                            {
                                OutputExcode1(excode, MENU_MODIFYADD, 0);
                            }
                        }
                        else if (dat == 0x07)
                        { //����
                            EnUPflag = 1;
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                            if (Up == 1)
                            { //ѹ��������,ѹ����
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                            }
                            else
                            {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                Up = 0;
                                Dn = 1;
                                pfootf = 0xff;
                            }
                        }
                        else if (dat == 0x3c)
                        {
                            g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                        }
                        else if (dat == 0x3d)
                        {
                            g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                        }
                        else if (dat == 0x3e)
                        {
                            g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                        }
                        sendExCodeCmd();
                        g_jog_point++;
                        if (g_jog_point < tmp_feedsum)
                        {
                            dat = SREG[dat1 + 2] & 0x00ff;
                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                            {
                                dat = SREG[dat1 + 3];
                                x = (U8)(dat);
                                y = (U8)((U16)(dat >> 8));
                                if (x >= 0x80)
                                {
                                    x -= 0x80;
                                    g_dxpos = -x;
                                }
                                else
                                {
                                    g_dxpos = x;
                                }
                                if (y >= 0x80)
                                {
                                    y -= 0x80;
                                    g_dypos = -y;
                                }
                                else
                                {
                                    g_dypos = y;
                                }
                                dat = SREG[dat1 + 2];
                                vdat = SREG[dat1 + 3];
                                if ((dat & 0xff00) != 0)
                                {
                                    if (Up == 0)
                                    {
                                        pfootval = (S8)(SREG[dat1 + 2] >> 8);
                                        temp_PfVal = g_pfootd.base + Mit_CailPadOffestPF(g_jog_point, 0, 0) + pfootval;
                                    }
                                    else
                                    {
                                        pfootval = (S8)(SREG[dat1 + 2] >> 8);
                                        temp_PfVal += pfootval;
                                    }
                                    g_temp_PfootOffVal = temp_PfVal;
                                }
                            }
                            else
                            {
                                g_dxpos = 0;
                                g_dypos = 0;
                                dat = SREG[dat1 + 2];
                                vdat = SREG[dat1 + 3];
                            }
                        }
                        else
                        {
                            g_dxpos = 0;
                            g_dypos = 0;
                            dat = 0xff;
                        }
                        //dis_data1(xx,yy,dat,g_jog_point,tmp_feedsum,dlforceval);
                        // disStatus(vxp2, vyp2, dat, vdat, dlforceval, 0, Color_Black);
                        //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                        if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        else if (g_sta_end_flag == 1)
                        {
                            chRectTemp = RECT_MODIADD_PIN2;
                        }
                        else
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, 0, rect[chRectTemp].colorT[1]);

                        vpointf = 1;
                        vpoint = g_jog_point;
                        if (g_jog_point == runf1)
                        {
                            g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                            g_pf_absval += g_pfootd.base;
                            if (g_pf_absval != old_PfootOffVal)
                            {
                                temp_PfVal = g_pf_absval;
                                old_PfootOffVal = g_pf_absval;
                                f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0); //������ѹ�Ÿ߶�
                                Read_Status();
                                pfootval = 0;
                                Up = 0;
                                Dn = 1;
                                EnUPflag = 1;
                            }
                        }
                    }
                    else
                    {
                        runf1 = 0;
                    }
                }
                else
                {
                    runf1 = 0;
                }
            }
            else if (F_FILETYPE == PAT_SEW)
            { //�ֵ�
                if (g_jog_point < max_feedsum)
                {
                    if (g_jog_point < runf1)
                    {
                        if (Stflag == 0)
                        { //�ڻ�еԭ��λ��
                            if (Up == 1)
                            {
                                Up = 0;
                                Dn = 1;
                                EnUPflag = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                            }
                            g_ExCode = 0;
                            g_curExCode = 0;
                            g_curPinAng = 0;
                            g_curSetPara1 = 0;
                            g_curSetPara2 = 0;
                            g_temp_oxpos = g_patd.orgX;
                            g_temp_oypos = g_patd.orgY;
                            if (GoHome == 0)
                            { //XYͬʱ�ƶ�
                                Comm_RelationMove(g_temp_oxpos, g_temp_oypos, MENU_MODIFYADD, 1, 1);
                            }
                            else if (GoHome == 1)
                            { //X�Ȼ�ԭ��,Y���ԭ��
                                //���ƶ�Y,���ƶ�X
                                Comm_RelationMove(0, g_temp_oypos, MENU_MODIFYADD, 1, 1);
                                Wait_Move_End();
                                Comm_RelationMove(g_temp_oxpos, 0, MENU_MODIFYADD, 1, 1);
                            }
                            else
                            {
                                //���ƶ�x,���ƶ�y
                                Comm_RelationMove(g_temp_oxpos, 0, MENU_MODIFYADD, 1, 1);
                                Wait_Move_End();
                                Comm_RelationMove(0, g_temp_oypos, MENU_MODIFYADD, 1, 1);
                            }

                            if (g_patd.orgX != 0 || g_patd.orgY != 0)
                            { //��ԭ��
                                Stflag = 1;
                                g_BorSTflag = Stflag;
                                Borcmd = 0x5050; //��ԭ��

                                // dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,0,0,0,0);		//��ʾ��ԭ��
                                // disStatus(vxp2, vyp2, Borcmd, 0, 0, 0, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, 0, 0, rect[chRectTemp].colorT[1]);

                                vpointf = 1;
                                vpoint = g_jog_point;
                            }
                            else
                            { //�޴�ԭ�� ��ʾ��һ����������
                                Stflag = 2;
                                g_BorSTflag = Stflag;
                                Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                Borpooft += g_pfootd.base;
                                //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                                // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                                //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);

                                if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                else if (g_sta_end_flag == 1)
                                {
                                    chRectTemp = RECT_MODIADD_PIN2;
                                }
                                else
                                {
                                    chRectTemp = RECT_MODIADD_PIN1;
                                }
                                disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                                vpointf = 1;
                                vpoint = g_jog_point;
                            }
                        }
                        else if (Stflag == 1)
                        { //������ʾ��һ������ָ��
                            Stflag = 2;
                            g_BorSTflag = Stflag;
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            Borpooft += g_pfootd.base;
                            //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                            // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                            vpointf = 1;
                            vpoint = g_jog_point;
                        }
                        else
                        {
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            Borpooft += g_pfootd.base;
                            if (Borpooft > PF_ABS_MAX)
                                Borpooft = PF_ABS_MAX;
                            if (Borpooft < PF_ABS_MIN)
                                Borpooft = PF_ABS_MIN;

                            if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                            { //����,����
                                EnUPflag = 1;
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                    if ((Borcmd & 0xf000) == 0x2000)
                                    {
                                        continue;
                                    }
                                }
                                if ((Borcmd & 0xf000) == 0x2000)
                                {
                                    movfeed = 1;
                                }
                            }
                            else if ((Borcmd & 0xf000) == 0x6000)
                            { //����
                                if ((Dn == 1) && (EnUPflag == 1))
                                {
                                    Up = 1;
                                    Dn = 0;
                                    Wait_Move_End();                  //�ȴ�ƽ̨�ƶ�����
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                    continue;
                                }
                                EnUPflag = 0;
                            }
                            else if ((Borcmd & 0xff00) == 0xf000)
                            {                    //��ѹ�ŵ���
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Dn == 1)
                                {
                                    Up = 1;
                                    Dn = 0;
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                    continue;
                                }

                                if (Borpfdis > 0)
                                { //��ѹ������
                                    f = Write_Cmd(C_PFEMOV, 1, Borpfdis);
                                }
                                else if (Borpfdis < 0)
                                { //��ѹ���½�
                                    f = Write_Cmd(C_PFEMOV, 2, -Borpfdis);
                                }
                            }
                            else if ((Borcmd & 0xff00) == 0xf400)
                            { //����
                                EnUPflag = 1;
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                if (Up == 1)
                                { //ѹ��������,ѹ����
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                else
                                {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                    Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                    Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    Up = 0;
                                    Dn = 1;
                                    pfootf = 0xff;
                                }
                            }
                            else if ((Borcmd & 0xff00) == 0xfc00)
                            { //���ӹ���
                                excode = Borcmd & 0xff;
                                if (excode != 0)
                                {
                                    OutputExcode1(excode, MENU_MODIFYADD, 0);
                                }
                            }
                            sendExCodeCmd();
                            g_jog_point++;
                            Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 1, movfeed);
                            //Comm_Move(temp_oxpos1+g_dxpos,temp_oypos1+g_dypos,movfeed?80:50,MENU_MODIFYADD,1,movfeed);
                            movfeed = 0;
                            //								//������ʾ��һ��ָ��
                            Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                            Borpooft += g_pfootd.base;
                            //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                            // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                            //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            else if (g_sta_end_flag == 1)
                            {
                                chRectTemp = RECT_MODIADD_PIN2;
                            }
                            else
                            {
                                chRectTemp = RECT_MODIADD_PIN1;
                            }
                            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                            vpointf = 1;
                            vpoint = g_jog_point;
                        }
                    }
                    else
                    {
                        runf1 = 0;
                    }
                }
                else
                {
                    runf1 = 0;
                }
            }
        }
        else if (runf2)
        {
            if (F_FILETYPE == PAT_MIT)
            { //����
                if (g_jog_point > min_feedsum)
                {
                    if (g_jog_point >= runf2)
                    {
                        dat1 = g_patd.pat_add + g_jog_point * 2;
                        dat = SREG[dat1 - 2] & 0x00ff;
                        if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                        {
                            if ((dat == 0x1b) || (dat == 0x3))
                            {
                                movfeed = 1;
                            }
                            dat = SREG[dat1 - 1];
                            x = dat & 0x000000ff;
                            y = ((U32)(dat >> 8)) & 0x000000ff;
                            if (x >= 0x80)
                            {
                                x -= 0x80;
                                g_dxpos = x;
                                g_temp_oxpos += (S16)(x);
                            }
                            else
                            {
                                g_dxpos = -x;
                                g_temp_oxpos -= (S16)(x);
                            }
                            if (y >= 0x80)
                            {
                                y -= 0x80;
                                g_dypos = y;
                                g_temp_oypos += (S16)(y);
                            }
                            else
                            {
                                g_dypos = -y;
                                g_temp_oypos -= (S16)(y);
                            }
                            if (g_modcordflag)
                            {
                                f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MODIFYADD, 0, movfeed);
                            }
                            else
                            {
                                f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MODIFYADD, 0, movfeed);
                            }
                            g_modcordflag = 0;
                            movfeed = 0;
                            if (f == 0)
                            {
                                break;
                            }
                        }
                        else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                        { //���ӹ���
                            excode = 0;
                            if (dat == 0x05)
                                excode = 0x01; //����1
                            else if (dat == 0x0b)
                                excode = 0x02; //����2
                            else if (dat == 0x0c)
                                excode = 0x04; //����3
                            else if (dat == 0x0d)
                                excode = 0x08; //����4
                            else
                            { //����5-8
                                excode = (SREG[dat1 - 2] & 0xff00) >> 8;
                                if (excode > 3)
                                    excode = 0;
                                excode = (1 << (excode + 4));
                            }
                            if (excode != 0)
                            {
                                OutputExcode1(excode, MENU_MODIFYADD, 0);
                            }
                        }
                        else if (dat == 0x07)
                        { //����
                            EnUPflag = 1;
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                            if (Up == 1)
                            { //ѹ��������,ѹ����
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                            }
                            else
                            {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                Up = 0;
                                Dn = 1;
                                pfootf = 0xff;
                            }
                        }
                        else if (dat == 0x3c)
                        {
                            g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 0);
                        }
                        else if (dat == 0x3d)
                        {
                            g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 1);
                        }
                        else if (dat == 0x3e)
                        {
                            g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 2);
                        }
                        sendExCodeCmd();
                        g_jog_point--;
                        if (g_jog_point < tmp_feedsum)
                        {
                            dat = SREG[dat1 - 2] & 0x00ff;
                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                            {
                                dat = SREG[dat1 - 1];
                                x = (U8)(dat);
                                y = (U8)((U16)(dat >> 8));
                                if (x >= 0x80)
                                {
                                    x -= 0x80;
                                    g_dxpos = -x;
                                }
                                else
                                {
                                    g_dxpos = x;
                                }
                                if (y >= 0x80)
                                {
                                    y -= 0x80;
                                    g_dypos = -y;
                                }
                                else
                                {
                                    g_dypos = y;
                                }
                                dat = SREG[dat1 - 2];
                                vdat = SREG[dat1 - 1];
                            }
                            else
                            {
                                g_dxpos = 0;
                                g_dypos = 0;
                                dat = SREG[dat1 - 2];
                                vdat = SREG[dat1 - 1];
                            }
                        }
                        else
                        {
                            g_dxpos = 0;
                            g_dypos = 0;
                            dat = 0xff;
                        }
                        //dis_data1(xx,yy,dat,g_jog_point,tmp_feedsum,dlforceval);
                        // disStatus(vxp2, vyp2, dat, vdat, dlforceval, 0, Color_Black);
                        //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                        if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        else if (g_sta_end_flag == 1)
                        {
                            chRectTemp = RECT_MODIADD_PIN2;
                        }
                        else
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, 0, rect[chRectTemp].colorT[1]);

                        vpointf = 1;
                        vpoint = g_jog_point;
                        if (g_jog_point < runf2)
                        {
                            g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                            g_pf_absval += g_pfootd.base;
                            if (g_pf_absval != old_PfootOffVal)
                            {
                                temp_PfVal = g_pf_absval;
                                old_PfootOffVal = g_pf_absval;
                                f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0); //������ѹ�Ÿ߶�
                                Read_Status();
                                pfootval = 0;
                                Up = 0;
                                Dn = 1;
                                EnUPflag = 1;
                            }
                        }
                    }
                    else
                    {
                        runf2 = 0;
                    }
                }
                else
                {
                    runf2 = 0;
                }
            }
            else if (F_FILETYPE == PAT_SEW)
            { //�ֵ�
                if (g_jog_point > min_feedsum)
                {
                    if (g_jog_point >= runf2)
                    {
                        Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                        Borpooft += g_pfootd.base;
                        if (Borpooft > PF_ABS_MAX)
                            Borpooft = PF_ABS_MAX;
                        if (Borpooft < PF_ABS_MIN)
                            Borpooft = PF_ABS_MIN;

                        if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                        { //����,����
                            EnUPflag = 1;
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                            if (Up == 1)
                            {
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                                if ((Borcmd & 0xf000) == 0x2000)
                                {
                                    continue;
                                }
                            }

                            if ((Borcmd & 0xf000) == 0x2000)
                            {
                                movfeed = 1;
                            }
                        }
                        else if ((Borcmd & 0xf000) == 0x6000)
                        { //����
                            if ((Dn == 1) && (EnUPflag == 1))
                            {
                                Up = 1;
                                Dn = 0;
                                Wait_Move_End();                  //�ȴ�ƽ̨�ƶ�����
                                Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                                continue;
                            }
                            EnUPflag = 0;
                        }
                        else if ((Borcmd & 0xff00) == 0xf000)
                        {                    //��ѹ�ŵ���
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                            if (Dn == 1)
                            {
                                Up = 1;
                                Dn = 0;
                                Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                                continue;
                            }
                            if (Borpfdis < 0)
                            { //��ѹ������
                                f = Write_Cmd(C_PFEMOV, 1, -Borpfdis);
                            }
                            else if (Borpfdis > 0)
                            { //��ѹ���½�
                                f = Write_Cmd(C_PFEMOV, 2, Borpfdis);
                            }
                        }
                        else if ((Borcmd & 0xff00) == 0xf400)
                        { //����
                            EnUPflag = 1;
                            Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                            if (Up == 1)
                            { //ѹ��������,ѹ����
                                Up = 0;
                                Dn = 1;
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                pfootf = 0xff;
                            }
                            else
                            {                                     //ѹ��������,��ѹ�ŷ�����̧����
                                Send_Pfoot(2, MENU_MODIFYADD, 1); //ѹ�Ž�
                                Send_Pfoot(1, MENU_MODIFYADD, 1); //ѹ����
                                Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                Up = 0;
                                Dn = 1;
                                pfootf = 0xff;
                            }
                        }
                        else if ((Borcmd & 0xff00) == 0xfc00)
                        { //���ӹ���
                            excode = Borcmd & 0xff;
                            if (excode != 0)
                            {
                                OutputExcode1(excode, MENU_MODIFYADD, 0);
                            }
                        }
                        else if ((Borcmd & 0xff00) == 0xec00 || (Borcmd & 0xff00) == 0xea00 || (Borcmd & 0xff00) == 0xeb00)
                        { //???,/???,???
                            if (g_jog_point)
                                Bor_feedInfo(g_jog_point - 1, g_patd.pat_len, &tBorcmd, &ttemp_oxpos1, &ttemp_oypos1, (S16 *)&tdxpos, (S16 *)&tdypos, (U8 *)&toldspeed, &tBorpooft, &tBorpfdis);
                            else
                            {
                                g_pointPinAng = 0;
                                g_pointSetPara1 = 0;
                                g_pointSetPara2 = 0;
                            }
                        }
                        sendExCodeCmd();
                        g_jog_point--;
                        Comm_RelationMove(-g_dxpos, -g_dypos, MENU_MODIFYADD, 1, movfeed);
                        //	Comm_Move(temp_oxpos1,temp_oypos1,movfeed?80:50,MENU_MODIFYADD,1,movfeed);
                        movfeed = 0;
                        //dis_data2(xx,yy,g_jog_point,max_feedsum,Borcmd,((Borcmd&0xff00)==0xf000)?Borpfdis:0,g_dxpos,g_dypos,dlforceval);
                        // disStatus(vxp2, vyp2, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, Color_Black);
                        //newDrawPoint(g_jogonflag?2:1,g_jog_point,scale,xo,yo,0);
                        if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        else if (g_sta_end_flag == 1)
                        {
                            chRectTemp = RECT_MODIADD_PIN2;
                        }
                        else
                        {
                            chRectTemp = RECT_MODIADD_PIN1;
                        }
                        disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, Borcmd, 0, dlforceval, ((Borcmd & 0xff00) == 0xf000) ? Borpfdis : 0, rect[chRectTemp].colorT[1]);

                        vpointf = 1;
                        vpoint = g_jog_point;
                    }
                    else
                    {
                        runf2 = 0;
                    }
                }
                else
                {
                    runf2 = 0;
                }
            }
        }

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_MODIFYADD);
        }
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MODIFYADD);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[ˢ�����ѡ����Ϣ*begin]**********************************************************
        if (dis_updata)
        {

            dis_updata = 0;

            if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
            {
                chRectTemp = RECT_MODIADD_PIN1;
            }
            else if (g_sta_end_flag == 1)
            {
                chRectTemp = RECT_MODIADD_PIN2;
            }
            else
            {
                chRectTemp = RECT_MODIADD_PIN1;
            }
            clrAllStatus(rect[chRectTemp]);
            disStatusTit(rect[chRectTemp],
                         (g_sta_end_flag == 4) ? rect[chRectTemp].index[0] : rect[chRectTemp].index[1], 1);
            disStatus(rect[chRectTemp].xpis, rect[chRectTemp].ypis, dat, vdat, dlforceval, Borpooft - old_PfootOffVal, rect[chRectTemp].colorT[1]);
            disStatusFeedSum(rect[chRectTemp].xpis, rect[chRectTemp].ypis, max_feedsum, rect[chRectTemp].colorT[0]);
        }
        // /[ˢ�����ѡ����Ϣ*end]**********************************************************

        // /[ˢ�����뿪����������ϰ���*begin]**********************************************************
        if (dis_updata1 == 1)
        {
            dis_updata1 = 2;

            f = KEY_MODIADD_JOGON + g_jogmode;
            for (i = KEY_MODIADD_JOGON; i <= KEY_MODIADD_JOGOFF; i++)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           ((f == i) && (AutoDoSplit == 0)) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                           ((f == i) && (AutoDoSplit == 0)) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
            }
            if ((F_FILETYPE == PAT_MIT) && (para == MODIFY_PART_CHANGE) && (g_selectmode == 3))
            {
                DrawButton2(keyd[KEY_MODIADD_AUTO],
                            (AutoDoSplit == 1) ? keybmpind[KEY_MODIADD_AUTO].border[2] : keybmpind[KEY_MODIADD_AUTO].border[0],
                            (AutoDoSplit == 1) ? keybmpind[KEY_MODIADD_AUTO].icon[2] : keybmpind[KEY_MODIADD_AUTO].icon[0], 1);
            }
        }
        // /[ˢ�����뿪����������ϰ���*end]**********************************************************

        // /[ˢ�»�����ʾ*begin]**********************************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;
            if (g_jogonflag == 0)
            {
                patRulesTmp = PATRULES_MODIFY_ADD1;
                i = NPAT_ST_ON;
                if ((para == MODIFY_PART_MOVE || para == MODIFY_PART_CHANGE) && g_sta_end_flag == 1)
                {
                    i = NPAT_ED_ON;
                    patRulesTmp = PATRULES_MODIFY_ADD2;
                }
                else if (para == MODIFY_PART_CHANGE && g_sta_end_flag == 4)
                {
                    i = NPAT_MU_ON;
                    patRulesTmp = PATRULES_MODIFY_ADD3;
                }
            }
            else
            {
                i = NPAT_ST_OFF;
                patRulesTmp = PATRULES_MODIFY_ADD1;
                if ((para == MODIFY_PART_MOVE || para == MODIFY_PART_CHANGE) && g_sta_end_flag == 1)
                {
                    i = NPAT_ED_OFF;
                    patRulesTmp = PATRULES_MODIFY_ADD2;
                }
                else if (para == MODIFY_PART_CHANGE && g_sta_end_flag == 4)
                {
                    i = NPAT_MU_OFF;
                    patRulesTmp = PATRULES_MODIFY_ADD3;
                }
            }
            // tscale1 = scale + tscale;
            tscale1 = pat_getFinalScale(tscale, scale);

            xorg = xo - (g_maxX + g_minX) * tscale1 / 2 / 1000;
            yorg = yo - tmuly * (g_maxY + g_minY) * tscale1 / 2 / 1000;
            getFeedPos(g_jog_point, F_FILETYPE, &fpx, &fpy);
            vposx = (S32)(fpx * tscale1 / 1000 + xorg);
            vposy = (S32)(tmuly * fpy * tscale1 / 1000 + yorg);
            if ((((vposx < keyd[KEY_MODIADD_IMG].xpis) ||
                  (vposx > keyd[KEY_MODIADD_IMG].xpis + keyd[KEY_MODIADD_IMG].xsize) ||
                  (vposy < keyd[KEY_MODIADD_IMG].ypis) ||
                  (vposy > keyd[KEY_MODIADD_IMG].ypis + keyd[KEY_MODIADD_IMG].ysize)) &&
                 vflag == 0) ||
                vflag == 1)
            { //��㳬��ʾ��Χ
                //printf("*--------------------------*\r\n");
                xorg = ORG_X;
                yorg = ORG_Y;

                xorg += (g_maxX + g_minX) * tscale1 / 2 / 1000;
                yorg += tmuly * (g_maxY + g_minY) * tscale1 / 2 / 1000;

                xo = xorg + (xorg - (S32)(fpx * tscale1 / 1000 + xorg));
                yo = yorg + (yorg - (S32)(tmuly * fpy * tscale1 / 1000 + yorg));

                xorg = xo - (g_maxX + g_minX * tscale1) / 2 / 1000;
                yorg = yo - tmuly * (g_maxY + g_minY) * tscale1 / 2 / 1000;
                vposx = (S32)(fpx * tscale1 / 1000 + xorg);
                vposy = (S32)(tmuly * fpy * tscale1 / 1000 + yorg);
                vflag = 1;
            }
            // newDrawPat(i, scale, xo, yo, 0, 0);
            pat_setPatRule(patRulesTmp);
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(patRulesTmp, scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
        }
        // /[ˢ�»�����ʾ*end]**********************************************************

        // /[��ʱ200ms��Ϣ����*begin]**********************************************************
        if (Timerms > 200)
        {
            Timerms = 0;
            if (Read_xyposi())
            {
#if 0
                if (absposx != g_xabspos)
                {
                    absposx = g_xabspos;
                    DrawFillRect(vxp1 + 43, vyp1, 93, 24, mCOLOR ? Color_Silver : Color_White);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1 + 56, vyp1, g_xabspos * 5, 6, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1 + 56, vyp1, g_xabspos, 5, 1, Color_Black);
                    }
#else
                    DrawMFS32P(vxp1 + 56, vyp1, g_xabspos, 5, 1, Color_Black);
#endif
                }
                if (absposy != g_yabspos)
                {
                    absposy = g_yabspos;
                    DrawFillRect(vxp1 + 194, vyp1, 93, 24, mCOLOR ? Color_Silver : Color_White);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1 + 207, vyp1, g_yabspos * 5, 6, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1 + 207, vyp1, g_yabspos, 5, 1, Color_Black);
                    }
#else
                    DrawMFS32P(vxp1 + 207, vyp1, g_yabspos, 5, 1, Color_Black);
#endif
                }

                if (abspf != g_pf_absdis)
                {
                    abspf = g_pf_absdis;
                    DrawFillRect(vxp2 + 356, vyp2 + 88, 80, 24, mCOLOR ? Color_Silver : Color_White);
                    DrawMFS32P(vxp2 + 369, vyp2 + 88, g_pf_absdis, 4, 1, Color_Black);
                }
#else
                if (abspf != g_pf_absdis || absposx != g_xabspos || absposy != g_yabspos)
                {

                    if (g_sta_end_flag == 0 || g_sta_end_flag == 4)
                    {
                        chRectTemp = RECT_MODIADD_PIN1;
                    }
                    else if (g_sta_end_flag == 1)
                    {
                        chRectTemp = RECT_MODIADD_PIN2;
                    }
                    else
                    {
                        chRectTemp = RECT_MODIADD_PIN1;
                    }

                    disStatusUpdate(rect[chRectTemp].xpis, rect[chRectTemp].ypis, g_dxpos, g_dypos, g_xabspos, g_yabspos, g_pf_absdis);
                }

#endif
            }
        }
        // /[��ʱ200ms��Ϣ����*end]**********************************************************

        // /[��ѹ�Ű�ťˢ��*begin]**********************************************************
        if (pfootf != (U16)(S_PFOOT))
        {
            pfootf = (U16)(S_PFOOT);
            DrawButton(keyd[KEY_MODIADD_FOOT], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_MODIADD_FOOT].border[0],
                       (pfootf == 0) ? keybmpind[KEY_MODIADD_FOOT].icon[0] : keybmpind[KEY_MODIADD_FOOT].icon[1]);
            if (pfootf == 0x10)
            { //����
                Up = 1;
                Dn = 0;
            }
            else
            {
                Up = 0;
                Dn = 1;
            }
        }
        // /[��ѹ�Ű�ťˢ��	*end]**********************************************************

        // /[����ѡ�������ʮ��	*begin]**********************************************************
        if (vpointf)
        {
            vpointf = 0;
            getFeedPos(vpoint, F_FILETYPE, &fpx, &fpy);
            // tscale1 = scale + tscale;
            tscale1 = pat_getFinalScale(tscale, scale);
            vposx = (S32)(fpx * tscale1 / 1000 + xorg);
            vposy = (S32)(tmuly * fpy * tscale1 / 1000 + yorg);
            if (((vposx < keyd[KEY_MODIADD_IMG].xpis) || (vposx > keyd[KEY_MODIADD_IMG].xpis + keyd[KEY_MODIADD_IMG].xsize) ||
                 (vposy < keyd[KEY_MODIADD_IMG].ypis) || (vposy > keyd[KEY_MODIADD_IMG].ypis + keyd[KEY_MODIADD_IMG].ysize)))
            { //��㳬��ʾ��Χ
                if (g_jogonflag == 0)
                {
                    patRulesTmp = PATRULES_MODIFY_ADD1;
                    i = NPAT_ST_ON;
                    if ((para == MODIFY_PART_MOVE || para == MODIFY_PART_CHANGE) && g_sta_end_flag == 1)
                    {
                        i = NPAT_ED_ON;
                        patRulesTmp = PATRULES_MODIFY_ADD2;
                    }
                    else if (para == MODIFY_PART_CHANGE && g_sta_end_flag == 4)
                    {
                        i = NPAT_MU_ON;
                        patRulesTmp = PATRULES_MODIFY_ADD3;
                    }
                }
                else
                {
                    i = NPAT_ST_OFF;
                    patRulesTmp = PATRULES_MODIFY_ADD1;
                    if ((para == MODIFY_PART_MOVE || para == MODIFY_PART_CHANGE) && g_sta_end_flag == 1)
                    {
                        i = NPAT_ED_OFF;
                        patRulesTmp = PATRULES_MODIFY_ADD2;
                    }
                    else if (para == MODIFY_PART_CHANGE && g_sta_end_flag == 4)
                    {
                        i = NPAT_MU_OFF;
                        patRulesTmp = PATRULES_MODIFY_ADD3;
                    }
                }
                xorg = ORG_X;
                yorg = ORG_Y;

                xorg += (g_maxX + g_minX) * tscale1 / 2 / 1000;
                yorg += tmuly * (g_maxY + g_minY) * tscale1 / 2 / 1000;

                xo = xorg + (xorg - (S32)(fpx * tscale1 / 1000 + xorg));
                yo = yorg + (yorg - (S32)(tmuly * fpy * tscale1 / 1000 + yorg));

                xorg = xo - (g_maxX + g_minX) * tscale1 / 2 / 1000;
                yorg = yo - tmuly * (g_maxY + g_minY) * tscale1 / 2 / 1000;

                //printf("xorg =%d yorg = %d xo = %d yo = %d \r\n",xorg,yorg,xo,yo);
                vposx = (S32)(fpx * tscale1 / 1000 + xorg);
                vposy = (S32)(tmuly * fpy * tscale1 / 1000 + yorg);

                // newDrawPat(i, scale, xo, yo, 0, 0);
                pat_setPatRule(patRulesTmp);
                pat_clearConvas(patRule.h_patConvasIndex);
                pat_drawPattern(patRulesTmp, scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
            }
            else
            {
                RecoveData2();
                ReadPointData2(vposx - 3, vposy - 3);
                Draw_BigCrossLimit(vposx, vposy, COLOR_SELECT_P, patRule.h_patConvasIndex);
            }
        }
        // /[����ѡ�������ʮ��	*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MODIFYADD)
        {
            plastno = MENU_MODIFYADD;
            g_temp_pno = MENU_MODIFYADD;
            break;
        }
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
loop1:
    if ((pno != MENU_IMAGE) && (pno != MENU_MOIMAGE) && (pno != MENU_RWHELP) && (g_sta_end_flag != 4))
    {
        g_dxpos = 0;
        g_dypos = 0;
    }
}

/***************************************************/
#define AREA_ZIG 0 //0-�����޸����β�֧��Z��,1-֧��

#define MOD1_TEXT_ADDR 89
#define MOD1_COM_ADDR MOD1_TEXT_ADDR
#define MOD1_COM_LEN 18
#define MOD1_PARA0_ADDR MOD1_COM_ADDR + MOD1_COM_LEN
#define MOD1_PARA0_LEN 4
#define MOD1_NUM_ADDR MOD1_PARA0_ADDR + MOD1_PARA0_LEN
#define MOD1_NUM_LEN 2
#define MOD1_PARA1_ADDR MOD1_NUM_ADDR + MOD1_NUM_LEN
#define MOD1_PARA1_LEN 5
#define MOD1_PARA2_ADDR MOD1_PARA1_ADDR + MOD1_PARA1_LEN
#define MOD1_PARA2_LEN 5
#define MOD1_PARA3_ADDR MOD1_PARA2_ADDR + MOD1_PARA2_LEN
#define MOD1_PARA3_LEN 5
#define MOD1_PARA4_ADDR MOD1_PARA3_ADDR + MOD1_PARA3_LEN
#define MOD1_PARA4_LEN 8
#define MOD1_PARA5_ADDR MOD1_PARA4_ADDR + MOD1_PARA4_LEN
#define MOD1_PARA5_LEN 26
#define MOD1_PARA6_ADDR MOD1_PARA5_ADDR + MOD1_PARA5_LEN
#define MOD1_PARA6_LEN 9

#define KEY_MODIFY1_SCRNSVR 1 //1 ����
#define KEY_MODIFY1_BACK 2    //2 ����
#define KEY_MODIFY1_ENTER 3   //3 ȷ��
#define KEY_MODIFY1_PAT 4     //4 ����
#define KEY_MODIFY1_PFOOT 5   //5 ̧ѹ��
#define KEY_MODIFY1_CANCEL 6  //6 ȡ��

#define KEY_MODIFY1_ZOUT 7 //7 ��С
#define KEY_MODIFY1_ZIN 8  //8 �Ŵ�
#define KEY_MODIFY1_IMG 9  //9 �����ƶ�
#define KEY_MODIFY1_TOP 10 //10 ������

#define KEY_MODIFY1_MUL 11  //11 ����
#define KEY_MODIFY1_MUM 12  //12 ��
#define KEY_MODIFY1_MUR 13  //13 ����
#define KEY_MODIFY1_MML 14  //14 ��
#define KEY_MODIFY1_MMSP 15 //15 ѹ������
#define KEY_MODIFY1_MMR 16  //16 ��
#define KEY_MODIFY1_MDL 17  //17 ����
#define KEY_MODIFY1_MDM 18  //18 ��
#define KEY_MODIFY1_MDR 19  //19 ����

#define KEY_MODIFY1_DUP 20   //20 +1
#define KEY_MODIFY1_DDOWN 21 //21 -1
#define KEY_MODIFY1_0 22     //22 0
#define KEY_MODIFY1_1 23     //23 1
#define KEY_MODIFY1_2 24     //24 2
#define KEY_MODIFY1_3 25     //25 3
#define KEY_MODIFY1_4 26     //26 4
#define KEY_MODIFY1_5 27     //27 5
#define KEY_MODIFY1_6 28     //28 6
#define KEY_MODIFY1_7 29     //29 7
#define KEY_MODIFY1_8 30     //30 8
#define KEY_MODIFY1_9 31     //31 9

#define KEY_MODIFY1_MET1 32 //32 ����1
#define KEY_MODIFY1_MET2 33 //33 ����2
#define KEY_MODIFY1_MET3 34 //34 ����3
#define KEY_MODIFY1_MET4 35 //35 ����4

#define KEY_MODIFY1_INP1 36 //36 ����1
#define KEY_MODIFY1_INP2 37 //37 ����2

/*

#define KEY_MODIFY1_ORGM1 32 //32 ����λ������
#define KEY_MODIFY1_ORGM2 33 //33

#define KEY_MODIFY1_DELNABS 34 //34	��ɾ��
#define KEY_MODIFY1_DELNREL 35
#define KEY_MODIFY1_DELALL 36

#define KEY_MODIFY1_ADDABS 37 //37	��׷��
#define KEY_MODIFY1_ADDREL 38
#define KEY_MODIFY1_ADDSAME 39

#define KEY_MODIFY1_MOVEABS 40 //40	������
#define KEY_MODIFY1_MOVEREL 41
#define KEY_MODIFY1_MOVEBLOCK 42

#define KEY_MODIFY1_CHANGE 43 //43	�����ƶ�
#define KEY_MODIFY1_FIXED 44

#define KEY_MODIFY1_LINE 45 //45	��������
#define KEY_MODIFY1_POLYGON 46
#define KEY_MODIFY1_ARC 47
#define KEY_MODIFY1_CURVE 48
#define KEY_MODIFY1_ZIGZAG 49
#define KEY_MODIFY1_FEED 50
#define KEY_MODIFY1_SELWIDTH 51
#define KEY_MODIFY1_SELPITCH 52
#define KEY_MODIFY1_DIRL 53
#define KEY_MODIFY1_DIRC 54
#define KEY_MODIFY1_DIRR 55

#define KEY_MODIFY1_PITCHN 56 //56	�޸����
#define KEY_MODIFY1_PITCHALL 57
#define KEY_MODIFY1_STITCH 58
#define KEY_MODIFY1_PITCH 59
#define KEY_MODIFY1_ANG 60

#define KEY_MODIFY1_NOZIG 61
#define KEY_MODIFY1_ZIG 62
#define KEY_MODIFY1_ZIGWIDTH 63
#define KEY_MODIFY1_ZIGPITCH 64
#define KEY_MODIFY1_ZIGDIRL 65
#define KEY_MODIFY1_ZIGDIRC 66
#define KEY_MODIFY1_ZIGDIRR 67

#define KEY_MODIFY1_MAX 68
*/

#define RECT_MODIFY1_X_TIT 1
#define RECT_MODIFY1_X_STR 2
#define RECT_MODIFY1_Y_TIT 3
#define RECT_MODIFY1_Y_STR 4
#define RECT_MODIFY1_ST_TIT 5
#define RECT_MODIFY1_ST_STR 6
#define RECT_MODIFY1_EN_TIT 7
#define RECT_MODIFY1_EN_STR 8

#define RECT_MODIFY1_INP1_TIT 9
#define RECT_MODIFY1_INP2_TIT 10

#define RECT_MODIFY1_METHOD 11

void Menu_Modify1(U32 para)
{
#if 1
    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ

    /****������ز���end****/

    /****��־λ��ز���begin****/
    U32 i;
    U8 pfootf = 0xff;
    U8 dis_updata1 = 0; //��ʾ����(�����/���ּ�)
    U8 dis_updata2 = 0; //��ʾͼ��
    U8 dis_updata3 = 0; //����ģʽ������ʾ
    U8 dis_updata4 = 0; //edit����ˢ�� para == 1|2|3|4
    //U8 dis_updata5 = 0;  //��ʾz��ѡ�񰴼�
    U8 dis_updata6 = 0;  //��ʾ��Ŀ�����趨��ʾ��
    U8 dis_updata10 = 0; //edit1�������ˢ�� para == 5|6
    U8 dis_updata11 = 0; //deit2��������ˢ�� para == 5|6
    U8 dis_updata12 = 0; //Z�ַ��趨��Ϣˢ��
    U8 dis_updata13 = 0; //Z�ַ�����Ϣˢ��
    //U8 dis_updata14 = 0; //Z�ַ������Ϣˢ��

    U8 dis_updata15 = 0; //XY,����ˢ��
    U8 dis_updata16 = 0; //��������ģʽ����
    U8 dis_updata17 = 0; //����edit��ʾ para == 1|2|3|4
    U8 dis_updata18 = 0; //��ʾedit para == 1|2|3|4

    U16 Setang = 30;
    U8 dis_pos = 1; //��ʾʵ������
    U8 endir = 0;   //�����ʹ��
    U8 ennum = 0;   //���ּ�ʹ��
    U8 enmode = 1;  //ģʽѡ��ʹ��
    U8 enCanel = 1; //ʹ�������
    //U8 enzig = 0;   //�����޸�Z��
    U8 ll = 0, f = 0;
    U16 title = 0;
    U32 tmp_feedc = 0, tmp_feedsum = 0, tmp_pat_add = 0;
    U16 maxfeed = 0;
    U32 n, ii, dat, dat1;
    U16 tmp_draw_add = 0, tmp_line_add = 0;
    U16 xx = 0, yy = 0;
    S16 x, y;
    S16 xorg = 0, yorg = 0;
    S16 tmpxpos, tmpypos;
    S16 mvxx, mvyy;
    S32 scale = 0, scale1 = 0, vscale = 0;
    U8 bResetCaonvas = 0;
    //S16 dorgx=0,dorgy=0;
    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    S32 posx1, posy1;
    S16 absposx = 0xffff, absposy = 0xffff;
    S8 mult1 = 1, mult2 = 1, mult3 = 1, tmuly = 1;
    U16 moddat = 0;
    U16 Borcmd;
    S8 Borpooft, Borpfdis;
    U8 speed;
    U8 enZoom = 1;
    S16 dx = 0, dy = 0;
    const U16 ORG_X = CONVAS_ORG_PX9, ORG_Y = CONVAS_ORG_PY9;
    S16 xo = ORG_X;
    S16 yo = ORG_Y;
    U8 pressKey = 0;
    S16 nearpx, nearpy, onearpx = 0, onearpy = 0;
    U8 getNearflag = 0 /*,vnear =0*/;
    U8 tonear = 0, otonear = 0;
    U16 tmp_parameter1 /*,tmp_parameter2*/;

    U8 editflag = 0;      //�����ѡ�б�־λ
    U8 chSelInputMax = 0; //�����ѡ��������
    U8 chSelModeMax = 0;  //���������������

    U8 patRulesTmp = PATRULES_MODIFY1_SINGLE;
    /****��־λ��ز���end***/

    // const U8 keynum = {KEY_MODIADD_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 ����
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 ȷ��
        {0, 0, 0, 0},                        //4 ����
        {TYPE_X4, TYPE_Y4, KEY_X1, KEY_Y1},  //5 ̧ѹ��
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //6 ȡ��
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},  //7 ��С
        {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},  //8 �Ŵ�

        {CONVAS_PX9, CONVAS_PY9, CONVAS_X9, CONVAS_Y9}, //9 �����ƶ�
        {TYPE_X9, TYPE_Y4, KEY_X1, KEY_Y1},             //10 ������

        {TYPE_X36, TYPE_Y36, KEY_X1, KEY_Y1},                           //11 ����
        {TYPE_X36 + KEY_X1, TYPE_Y36, KEY_X1, KEY_Y1},                  //12 ��
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36, KEY_X1, KEY_Y1},              //13 ����
        {TYPE_X36, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},                  //14 ��
        {TYPE_X36 + KEY_X1, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},         //15 ѹ���ٶ�
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},     //16 ��
        {TYPE_X36, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1},              //17 ����
        {TYPE_X36 + KEY_X1, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1},     //18 ��
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1}, //19 ����

        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2}, //20 +1
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},       //21 -1
        {TYPE_X37, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},                             //22 0
        {TYPE_X37, TYPE_Y36, KEY_X2, KEY_Y2},                                                         //23 1
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36, KEY_X2, KEY_Y2},                                   //24 2
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36, KEY_X2, KEY_Y2},                             //25 3
        {TYPE_X37, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},                                   //26 4
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},             //27 5
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},       //28 6
        {TYPE_X37, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},                             //29 7
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},       //30 8
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2}, //31 9

        {TYPE_X5, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},              //32 ����1
        {TYPE_X5 + KEY_X6, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},     //33 ����2
        {TYPE_X5 + KEY_X6 * 2, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //34 ����3
        {TYPE_X5 + KEY_X6 * 3, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //35 ����4

        {TYPE_X38, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5},  //36 ����1
        {TYPE_X38, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5}, //37 ����2

    };

    const U16 keyBmpList[7][9] = {
        {551, 0, 552,
         553, 0, 554}, //�����ƶ�
        {555, 0, 556,
         557, 0, 558,
         559, 0, 560}, //��ɾ��
        {561, 0, 562,
         563, 0, 564,
         565, 0, 566}, //�����
        {567, 0, 568,
         569, 0, 570,
         571, 0, 572}, //������
        {573, 0, 574,
         575, 0, 576}, //���ƶ�
        {551, 0, 552,
         553, 0, 554}, //���޸�
        {577, 0, 578,
         579, 0, 580}, //���

    };
    const U16 keyTitList[7][4] = {
        {MOD1_PARA0_ADDR + 2, MOD1_PARA0_ADDR + 3},                      //�����ƶ�
        {MOD1_PARA1_ADDR + 1, MOD1_PARA1_ADDR + 2, MOD1_PARA1_ADDR + 3}, //��ɾ��
        {MOD1_PARA2_ADDR + 1, MOD1_PARA2_ADDR + 2, MOD1_PARA2_ADDR + 3}, //�����
        {MOD1_PARA3_ADDR + 1, MOD1_PARA3_ADDR + 2, MOD1_PARA3_ADDR + 3}, //������
        {MOD1_PARA4_ADDR + 5, MOD1_PARA4_ADDR + 6},                      //���ƶ�
        {MOD1_PARA5_ADDR, MOD1_PARA5_ADDR},                              //���޸�
        {MOD1_PARA6_ADDR + 1, MOD1_PARA6_ADDR + 2},                      //���
    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {1, 0, 0, 0}, {1}},      //1 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {2, 0, 0, 0}, {0}}, //2 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {3, 0, 0, 0}, {0}}, //3 ȷ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {37}, {4, 0, 0, 0}, {0}},     //4 ����

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {17, 19, 18, 20}, {25, 5, 0, 0}, {0}}, //5 ̧ѹ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {361, 362}, {6, 0, 0, 0}, {0}},        //6 ȡ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {74}, {86, 0, 0, 0}, {0}},             //7 ��С
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {75}, {85, 0, 0, 0}, {0}},             //8 �Ŵ�

        {{CONVAS_COLOR_B}, {CONVAS_COLOR_T}, {CONVAS_COLOR_B}, {MODA_COM_ADDR + 14, 0, 0, 0}, {0}}, //9 �����ƶ�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {Color_Black, Color_Silver}, {364}, {26, 0, 0, 0}, {0}}, //10 ������

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {55}, {7, 0, 0, 0}, {0}},      //11 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {56}, {8, 0, 0, 0}, {0}},      //12 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {57}, {9, 0, 0, 0}, {0}},      //13 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {58}, {10, 0, 0, 0}, {0}},     //14 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {51, 52}, {11, 0, 0, 0}, {0}}, //15 ѹ���ٶ�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {59}, {12, 0, 0, 0}, {0}},     //16 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {60}, {13, 0, 0, 0}, {0}},     //17 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {61}, {14, 0, 0, 0}, {0}},     //18 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {62}, {15, 0, 0, 0}, {0}},     //19 ����

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200}, {16, 0, 0, 0}, {0}}, //20 +1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202}, {17, 0, 0, 0}, {0}}, //21 -1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {91, 101}, {0, 0, 0, 0}, {0}},   //22 0
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {92, 102}, {0, 0, 0, 0}, {0}},   //23 1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {93, 103}, {0, 0, 0, 0}, {0}},   //24 2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {94, 104}, {0, 0, 0, 0}, {0}},   //25 3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {95, 105}, {0, 0, 0, 0}, {0}},   //26 4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {96, 106}, {0, 0, 0, 0}, {0}},   //27 5
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {97, 107}, {0, 0, 0, 0}, {0}},   //28 6
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {98, 108}, {0, 0, 0, 0}, {0}},   //29 7
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {99, 109}, {0, 0, 0, 0}, {0}},   //30 8
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {100, 110}, {0, 0, 0, 0}, {0}},  //31 9

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //32 ����1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //33 ����2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //34 ����3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //35 ����4

        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {Color_Black, 0, 0, 0}, {0}}, //36 ����1
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {Color_Black, 0, 0, 0}, {0}}, //37 ����2
    };

    const U16 rectTitList[7][4] = {
        //0:������⣬1:��������� 2:edit1���� 3:edit2����
        {1, 0, 0, 0}, //�����ƶ�
        {4, 0, 0, 0}, //��ɾ��
        {4, 0, 0, 0}, //�����
        {4, 0, 0, 0}, //������
        {3, 4, 7, 0}, //���ƶ�
        {0, 0, 0, 0}, //���޸�
        {8, 0, 6, 7}, //���
    };
    const U16 rectInTitList[] = {
        77, //����
        64, //���
        76, //�����
    };
    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X17, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {87}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {88}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {TYPE_X17, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {59, 68, 74}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {59, 69, 74}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {0}},
        {TYPE_X9, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {0}},

        {292, 396, 8, 80, {Color_Black}, {Color_Black}, {MOD1_TEXT_ADDR + 17}},

    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MODIFY1); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(patRulesTmp);

// /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

// /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
#if AREA_ZIG == 0
    g_modzig = 0;
#endif
    Read_xyposi(); //��ȡһ�µ�ǰλ��
    if (F_FILETYPE == PAT_MIT)
    {              //����
        mult1 = 1; //g_dxpos,g_dypos
        mult2 = -1;
        tmuly = 1;
        tmp_feedc = 0;
        tmp_feedsum = g_patd.feed_sum;
        tmp_draw_add = g_patd.draw_add;
        tmp_line_add = g_patd.line_add;
        tmp_pat_add = g_patd.pat_add;
        tmp_feedc = g_jog_point;
    }
    else if (F_FILETYPE == PAT_SEW)
    { //�ֵ�
#if MOVEUNIT == 1
        mult1 = 1;
        mult3 = 2;
#else
        mult1 = 2;
#endif
        mult2 = 1;
        tmuly = -1;
        tmp_feedc = 0;
        tmp_feedsum = g_patd.feed_sum;
        tmp_feedc = g_jog_point;
    }

    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************

    if (para == 0) //	����λ������
    {
        chSelModeMax = 2;
        chSelInputMax = 0;
        editflag = 0;
        title = MOD1_PARA0_ADDR;
        if (plastno != MENU_MOIMAGE)
            g_selectmode = 0; //Ĭ��ѡ���һ��
        if (g_selectmode > 1)
        {
            g_selectmode = 0;
        }

        tmp_selectmode = g_selectmode;
        if ((!(S_CLAMP)) && (P_OST == 0))
        {
            pno = MENU_RWHELP;
            hleppara = 13;
            return;
        }
        dis_updata1 = 1;
        dis_updata2 = 1;
        dis_updata3 = 1;
        endir = 1;

        for (i = 1; i <= RECT_MODIFY1_ST_STR; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis,
                         rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MODIFY1_X_TIT || i == RECT_MODIFY1_Y_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY1_ST_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((title + 1) * TitleLengthMax)),
                             SheetPage, (title + 1), rect[i].colorT[0]); //�������������
            }
        }

#if MOVEUNIT == 1
        if (g_curPTtype == 2)
        {
            DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
            DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
        }
        else
        {
            DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
            DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
        }
#else
        DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                  g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
        DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                  g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif

        // Draw24x24Str(16, 538, MEM_TITLE_ADD + (MOD1_COM_ADDR + 17) * TitleLengthMax, SheetPage, MOD1_COM_ADDR + 17, Color_Black);    //����
        // Draw24x24Str(152, 742, MEM_TITLE_ADD + (MOD1_PARA0_ADDR + 1) * TitleLengthMax, SheetPage, MOD1_PARA0_ADDR + 1, Color_Black); //�����

        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
        if (S_PFOOT)
        {
            Send_Pfoot(1, MENU_MODIFY1, 1); //ѹ����
        }
        if (F_FILETYPE == PAT_MIT)
        { //mit
            if (SREG[tmp_draw_add] == 0x88)
            {
                dat = SREG[tmp_line_add];
                n = 0;
                ii = tmp_line_add;
                while (ii < tmp_pat_add)
                {
                    dat1 = SREG[ii];
                    if (dat != dat1)
                    {
                        dat = SREG[tmp_pat_add + 2 * n] & 0x00ff;
                        if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1))
                        {
                            break;
                        }
                        else
                        {
                            dat = dat1;
                        }
                    }
                    ii++;
                    n++;
                }
                tmp_feedc = (U16)(n);
                g_tmpoxpos = 0;
                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                if (SREG[3] == 7)
                {
                    g_tmpoxpos = (S16)(SREG[6]);
                    g_tmpoypos = (S16)(SREG[7]);
                }
#endif
                for (ii = 0; ii < n; ii++)
                {
                    dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                    {
                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                        xx = dat & 0x000000ff;
                        yy = ((U32)(dat >> 8)) & 0x000000ff;
                        if (xx >= 0x80)
                        {
                            xx -= 0x80;
                            g_tmpoxpos -= xx;
                        }
                        else
                        {
                            g_tmpoxpos += xx;
                        }
                        if (yy >= 0x80)
                        {
                            yy -= 0x80;
                            g_tmpoypos -= yy;
                        }
                        else
                        {
                            g_tmpoypos += yy;
                        }
                    }
                }
                g_jog_point = tmp_feedc;
            }
            else
            {
                g_tmpoxpos = 0;
                g_tmpoypos = 0;
                tmp_feedc = 0;
                g_jog_point = 0;
            }
            g_tmpox = g_tmpoxpos;
            g_tmpoy = g_tmpoypos;
            x = g_tmpoxpos + g_dxpos;
            y = g_tmpoypos + g_dypos;
            Comm_Move(x, y, 50, MENU_MODIFY1, 0, 0);
        }
        else if (F_FILETYPE == PAT_SEW)
        {
            ISEWADD sewInfo = {0};
            ISEWCMD sewIcmd = {0};
            tmp_feedc = 0;
            Bor_AnaInfo((U16 *)SREG, g_patd.pat_len, &sewInfo);

            g_tmpoxpos = g_patd.orgX;
            g_tmpoypos = g_patd.orgY;
            tmp_feedc = 0;
            for (i = 0; (i < sewInfo.ptsize / 2);)
            {
                f = Bor_DrawCmd((U16 *)&SREG[i + sewInfo.pataddr], &sewIcmd);
                if (f)
                {
                    if ((sewIcmd.cmd & 0xf000) == 0x6000)
                    {

                        break;
                    }
                    tmp_feedc++;
                    g_tmpoxpos += sewIcmd.dx;
                    g_tmpoypos += sewIcmd.dy;
                    i += sewIcmd.off;
                }
                else
                    i++;
            }

            x = g_tmpoxpos + mult1 * g_dxpos;
            y = g_tmpoypos + mult1 * g_dypos;
            Comm_Move(x, y, 50, MENU_MODIFY, 1, 0);
            g_jog_point = tmp_feedc;
            g_BorSTflag = 1;
        }
        g_modcordflag = 0;

        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedc, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
    }
    else if (para == 1) //��ɾ��
    {
        chSelModeMax = 3;
        chSelInputMax = 1;
        editflag = 0;
        title = MOD1_PARA1_ADDR;
        if (plastno != MENU_MOIMAGE)
            g_selectmode = 0; //Ĭ��ѡ���һ��
        if (F_FILETYPE == PAT_MIT)
        { //mit
            if (g_selectmode > 2)
            {
                g_selectmode = 0;
            }
        }
        else if (F_FILETYPE == PAT_SEW)
        { //sew
            if (g_selectmode > 2)
            {
                g_selectmode = 0;
            }
        }
        tmp_selectmode = g_selectmode;

        for (i = 1; i <= RECT_MODIFY1_ST_STR; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis,
                         rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MODIFY1_X_TIT || i == RECT_MODIFY1_Y_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY1_ST_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((title + 4) * TitleLengthMax)),
                             SheetPage, (title + 4), rect[i].colorT[0]); //�������������
            }
        }

        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedc, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);

#if MOVEUNIT == 1
        if (g_curPTtype == 2)
        {
            DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
            DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
        }
        else
        {
            DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
            DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
        }
#else
        DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                  g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
        DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                  g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif

        // Draw24x24Str(16, 538, MEM_TITLE_ADD + (MOD1_COM_ADDR + 17) * TitleLengthMax, SheetPage, MOD1_COM_ADDR + 17, Color_Black);    //����
        // Draw24x24Str(152, 742, MEM_TITLE_ADD + (MOD1_PARA1_ADDR + 4) * TitleLengthMax, SheetPage, MOD1_PARA1_ADDR + 4, Color_Black); //��λ��

        for (i = 0; i < chSelInputMax; i++)
        {
            DrawFillRect(rect[i + RECT_MODIFY1_INP1_TIT].xpis, rect[i + RECT_MODIFY1_INP1_TIT].ypis,
                         rect[i + RECT_MODIFY1_INP1_TIT].xsize, rect[i + RECT_MODIFY1_INP1_TIT].ysize, rect[i + RECT_MODIFY1_INP1_TIT].colorB[0]);

            DrawButtonEdit(keyd[i + KEY_MODIFY1_INP1],
                           (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].textColor[0] : keybmpind[i + KEY_MODIFY1_INP1].icon[0],
                           (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].icon[1] : keybmpind[i + KEY_MODIFY1_INP1].icon[0]);
        }
        Draw24x24Str(rect[RECT_MODIFY1_INP1_TIT].xpis + (rect[RECT_MODIFY1_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[0]) * TitleLengthMax))) / 2,
                     rect[RECT_MODIFY1_INP1_TIT].ypis + (rect[RECT_MODIFY1_INP1_TIT].ysize - 24) / 2,
                     (MEM_TITLE_ADD + (U32)(rectInTitList[0] * TitleLengthMax)),
                     SheetPage, rectInTitList[0], rect[i].colorT[0]); //����

        maxfeed = tmp_feedsum - tmp_feedc - 1;
        DrawF16Str(keyd[KEY_MODIFY1_INP1].xpis, keyd[KEY_MODIFY1_INP1].ypis + keyd[KEY_MODIFY1_INP1].ysize + SAFE_DIST4,
                   "(1-     )", keybmpind[KEY_MODIFY1_INP1].textColor[0]);

        DrawFU32(keyd[KEY_MODIFY1_INP1].xpis + 3 * 9, keyd[KEY_MODIFY1_INP1].ypis + keyd[KEY_MODIFY1_INP1].ysize + SAFE_DIST4,
                 maxfeed, 5, keybmpind[KEY_MODIFY1_INP1].textColor[0]);

        if (tmp_parameter > maxfeed || tmp_parameter < 1)
        {
            tmp_parameter = 1;
        }
        DrawMFS32P(keyd[KEY_MODIFY1_INP1].xpis + (keyd[KEY_MODIFY1_INP1].xsize - 13 * 5) / 2,
                   keyd[KEY_MODIFY1_INP1].ypis + SAFE_DIST4,
                   tmp_parameter, 5, 0,
                   keybmpind[KEY_MODIFY1_INP1].textColor[0]);

        if (maxfeed < 100)
        {
            moddat = 10;
        }
        else if (maxfeed < 1000)
        {
            moddat = 100;
        }
        else if (maxfeed < 10000)
        {
            moddat = 1000;
        }
        else
        {
            moddat = 10000;
        }
        AnaPattern(0, F_FILETYPE, (F_FILELEN));
        dis_updata1 = 1;
        dis_updata2 = 1;
        dis_updata3 = 1;
        ennum = 1;

        g_tmpoxpos = mult1 * g_xabspos;
        g_tmpoypos = mult1 * g_yabspos;
    }
    else if (para == 2 || para == 3) //��׷��/������
    {
        if (para == 2)
        {
            chSelModeMax = 3;
            chSelInputMax = 0;
            editflag = 0;
            title = MOD1_PARA2_ADDR;
        }
        else
        {
            chSelModeMax = 2;
            chSelInputMax = 0;
            editflag = 0;
            title = MOD1_PARA3_ADDR;
        }

        if (plastno != MENU_MOIMAGE)
            g_selectmode = 0; //Ĭ��ѡ���һ��
        if (g_selectmode > 2)
        {
            g_selectmode = 0;
        }
        if (F_FILETYPE == PAT_MIT)
        { //mit
            if (para == 3 && g_selectmode > 1)
                g_selectmode = 0;
            g_tmpoxpos = 0;
            g_tmpoypos = 0;
            n = g_jog_point;
#if PATRULES_ORG_OFFSET
            if (SREG[3] == 7)
            {
                g_tmpoxpos = (S16)(SREG[6]);
                g_tmpoypos = (S16)(SREG[7]);
            }
#endif
            for (ii = 0; ii < n; ii++)
            {
                dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                {
                    dat = SREG[tmp_pat_add + ii * 2 + 1];
                    xx = dat & 0x000000ff;
                    yy = ((U32)(dat >> 8)) & 0x000000ff;
                    if (xx >= 0x80)
                    {
                        xx -= 0x80;
                        g_tmpoxpos -= xx;
                    }
                    else
                    {
                        g_tmpoxpos += xx;
                    }
                    if (yy >= 0x80)
                    {
                        yy -= 0x80;
                        g_tmpoypos -= yy;
                    }
                    else
                    {
                        g_tmpoypos += yy;
                    }
                }
            }
            g_tmpox = g_tmpoxpos;
            g_tmpoy = g_tmpoypos;
            x = g_tmpoxpos + g_dxpos;
            y = g_tmpoypos + g_dypos;

            if (g_xabspos != x || g_yabspos != y)
            {
                Comm_Move(x, y, 50, MENU_MODIFY1, 0, 0);
            }
            g_modcordflag = 0;
        }
        else if (F_FILETYPE == PAT_SEW)
        { //sew
            if (plastno != MENU_MOIMAGE)
            {
                Bor_feedInfo(tmp_feedc, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                g_tmpoxpos += g_dxpos;
                g_tmpoypos += g_dypos;
                g_dxpos = 0;
                g_dypos = 0;
            }
        }
        tmp_selectmode = g_selectmode;

        for (i = 1; i <= RECT_MODIFY1_ST_STR; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis,
                         rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MODIFY1_X_TIT || i == RECT_MODIFY1_Y_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY1_ST_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((title + 4) * TitleLengthMax)),
                             SheetPage, (title + 4), rect[i].colorT[0]); //�������������
            }
        }

        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedc, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);

#if MOVEUNIT == 1
        if (g_curPTtype == 2)
        {
            DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
            DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
        }
        else
        {
            DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
            DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
        }
#else
        DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                  g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
        DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                  g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif

        dis_updata1 = 1;
        dis_updata2 = 1;
        dis_updata3 = 1;
        endir = 1;
    }
    else if (para == 4) //�����ƶ�
    {
        chSelModeMax = 2;
        chSelInputMax = 1;
        editflag = 0;
        title = MOD1_PARA4_ADDR;
        if (plastno != MENU_MOIMAGE)
        {
            g_selectmode = 0;
            if ((g_parameter == 0) || (g_parameter > 127))
                g_parameter = 30;
        }
        tmp_parameter = g_parameter;
        if (g_selectmode > 1)
        {
            g_selectmode = 0;
        }
        tmp_selectmode = g_selectmode;
        if (F_FILETYPE == PAT_MIT)
        { //mit
            g_tmpoxpos = 0;
            g_tmpoypos = 0;
            n = g_jog_stpoint;
#if PATRULES_ORG_OFFSET
            if (SREG[3] == 7)
            {
                g_tmpoxpos = (S16)(SREG[6]);
                g_tmpoypos = (S16)(SREG[7]);
            }
#endif
            for (ii = 0; ii < n; ii++)
            {
                dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                {
                    dat = SREG[tmp_pat_add + ii * 2 + 1];
                    xx = dat & 0x000000ff;
                    yy = ((U32)(dat >> 8)) & 0x000000ff;
                    if (xx >= 0x80)
                    {
                        xx -= 0x80;
                        g_tmpoxpos -= xx;
                    }
                    else
                    {
                        g_tmpoxpos += xx;
                    }
                    if (yy >= 0x80)
                    {
                        yy -= 0x80;
                        g_tmpoypos -= yy;
                    }
                    else
                    {
                        g_tmpoypos += yy;
                    }
                }
            }
            g_tmpox = g_tmpoxpos;
            g_tmpoy = g_tmpoypos;
            x = g_tmpoxpos + g_dxpos;
            y = g_tmpoypos + g_dypos;
            //Comm_Move(x,y,50,MENU_MODIFY1,0,0);
            if (g_xabspos != x || g_yabspos != y)
            {
                Comm_Move(x, y, 50, MENU_MODIFY1, 0, 0);
            }
            g_modcordflag = 0;
        }
        else if (F_FILETYPE == PAT_SEW)
        { //sew
            if (plastno != MENU_MOIMAGE && plastno != MENU_RWHELP)
            {
                Bor_feedInfo(tmp_feedc, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                g_tmpoxpos += g_dxpos;
                g_tmpoypos += g_dypos;
                g_dxpos = 0;
                g_dypos = 0;
            }
        }

        for (i = 1; i <= RECT_MODIFY1_EN_STR; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis,
                         rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MODIFY1_X_TIT || i == RECT_MODIFY1_Y_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY1_ST_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((title + 3) * TitleLengthMax)),
                             SheetPage, (title + 3), rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY1_EN_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((title + 4) * TitleLengthMax)),
                             SheetPage, (title + 4), rect[i].colorT[0]); //�������������
            }
        }
        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 g_jog_stpoint, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);

        DrawFU32(rect[RECT_MODIFY1_EN_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_EN_STR].ypis + SAFE_DIST4,
                 g_jog_enpoint, 5, rect[RECT_MODIFY1_EN_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY1_EN_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_EN_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY1_EN_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY1_EN_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_EN_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY1_EN_STR].colorT[0]);

#if MOVEUNIT == 1
        if (g_curPTtype == 2)
        {
            DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
            DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
        }
        else
        {
            DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
            DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
        }
#else

        DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                  g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
        DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                  g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif

        if (g_num_mov_flag == 2)
        {
            dis_updata4 = 1;
            dis_pos = 1;
            enmode = 0;
            ennum = 1;
            endir = 0;
            dis_updata18 = 1;

            maxfeed = 127;
            moddat = 100;
        }
        else
        {

            dis_updata3 = 1;
            dis_pos = 1;
            enmode = 1;
            endir = 1;
            ennum = 0;
        }
        dis_updata1 = 1;
        dis_updata2 = 1;
    }
    else if (para == 5) //��������
    {

        if (g_selectmode > 5)
        {
            g_selectmode = 0;
        }
        tmp_selectmode = g_selectmode;

        if ((g_zig_width < 1) || (g_zig_width > 100))
            g_zig_width = 20;
        if ((g_zig_pitch < 5) || (g_zig_pitch > 100))
            g_zig_pitch = 30;
        if (g_zig_direction > 2)
            g_zig_direction = 0;

        if (F_FILETYPE == PAT_MIT)
        { //mit
            g_tmpoxpos = 0;
            g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
            if (SREG[3] == 7) //����-�޸�ԭ��λ��
            {
                g_tmpoxpos = (S16)(SREG[6]);
                g_tmpoypos = (S16)(SREG[7]);
            }
#endif
            n = g_jog_point;
            for (ii = 0; ii < n; ii++)
            {
                dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                {
                    dat = SREG[tmp_pat_add + ii * 2 + 1];
                    xx = dat & 0x000000ff;
                    yy = ((U32)(dat >> 8)) & 0x000000ff;
                    if (xx >= 0x80)
                    {
                        xx -= 0x80;
                        g_tmpoxpos -= xx;
                    }
                    else
                    {
                        g_tmpoxpos += xx;
                    }
                    if (yy >= 0x80)
                    {
                        yy -= 0x80;
                        g_tmpoypos -= yy;
                    }
                    else
                    {
                        g_tmpoypos += yy;
                    }
                }
            }
            g_tmpox = g_tmpoxpos;
            g_tmpoy = g_tmpoypos;
            x = g_tmpoxpos + g_dxpos;
            y = g_tmpoypos + g_dypos;
            if (g_xabspos != x || g_yabspos != y)
            {
                Comm_Move(x, y, 50, MENU_MODIFY1, 0, 0);
            }
            g_modcordflag = 0;
        }
        else if (F_FILETYPE == PAT_SEW && plastno != MENU_MOIMAGE)
        {
            dx = 0;
            dy = 0;
            Bor_feedInfo(tmp_feedc, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, &dx, &dy, &speed, &Borpooft, &Borpfdis);
            g_tmpoxpos += dx;
            g_tmpoypos += dy;
        }
        dis_updata1 = 1;
        if (g_num_mov_flag == 2)
        { //���ּ�
            title = MOD1_PARA5_ADDR;
            dis_pos = 0;
            enZoom = 0;
            ennum = 1;
            endir = 0;
            editflag = 0;
            maxfeed = 127;
            moddat = 100;
            dis_updata3 = 1;
            BmpFillRect(0, 34, LCD_width, 399);
            if (tmp_selectmode < 4)
            {
                dis_updata4 = 1;
            }
            else if (tmp_selectmode == 5)
            {
                dis_updata1 = 0;
                enCanel = 0;
            }
            if (tmp_selectmode == 4)
            {
                tmp_zigwp = g_zig_width;
            }
            if ((g_parameter == 0) || (g_parameter > 127))
                g_parameter = 30;
            tmp_parameter = g_parameter;
        }
        else
        { //�����
            chSelModeMax = 0;
            chSelInputMax = 1;
            editflag = 0;
            title = MOD1_PARA5_ADDR + 18 + g_selectmode;
            dis_pos = 1;
            ennum = 0;
            endir = 1;
            dis_updata2 = 1;
            dis_updata18 = 1;
            dis_updata4 = 1;

            for (i = 1; i <= RECT_MODIFY1_EN_STR; i++)
            {
                DrawFillRect(rect[i].xpis, rect[i].ypis,
                             rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                if (i == RECT_MODIFY1_X_TIT || i == RECT_MODIFY1_Y_TIT)
                {
                    Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                                 (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                                 SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
                }
                else if (i == RECT_MODIFY1_ST_TIT)
                {
                    Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                                 (MEM_TITLE_ADD + (U32)((MOD1_PARA4_ADDR + 3) * TitleLengthMax)),
                                 SheetPage, (MOD1_PARA4_ADDR + 3), rect[i].colorT[0]); //�������������
                }
                else if (i == RECT_MODIFY1_EN_TIT)
                {
                    Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                                 (MEM_TITLE_ADD + (U32)((MOD1_PARA4_ADDR + 4) * TitleLengthMax)),
                                 SheetPage, (MOD1_PARA4_ADDR + 4), rect[i].colorT[0]); //�������������
                }
            }
#if MOVEUNIT == 1
            if (g_curPTtype == 2)
            {
                DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
                DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
            }
            else
            {
                DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
                DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
            }
#else
            DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                      g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
            DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                      g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif

            // DrawMFU32(152, 770, g_jogmode == 1 ? tmp_feedc : g_array5[300 + g_point_num][0], 5, Color_Black);

            DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                     g_jog_stpoint, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
            DrawF16Str(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                       "/", rect[RECT_MODIFY1_ST_STR].colorT[0]);
            DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                     tmp_feedsum, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);

            DrawFU32(rect[RECT_MODIFY1_EN_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_EN_STR].ypis + SAFE_DIST4,
                     g_jog_enpoint, 5, rect[RECT_MODIFY1_EN_STR].colorT[0]);
            DrawF16Str(rect[RECT_MODIFY1_EN_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_EN_STR].ypis + SAFE_DIST4,
                       "/", rect[RECT_MODIFY1_EN_STR].colorT[0]);
            DrawFU32(rect[RECT_MODIFY1_EN_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_EN_STR].ypis + SAFE_DIST4,
                     tmp_feedsum, 5, rect[RECT_MODIFY1_EN_STR].colorT[0]);
        }
    }
    else if (para == 6) //��Ŀ����
    {

        chSelModeMax = 2;
        chSelInputMax = 2;
        editflag = 0;
        title = MOD1_PARA6_ADDR;

        dis_pos = 1;
        ennum = 1;
        endir = 0;
        if (plastno != MENU_MOIMAGE)
        {
            if ((g_stitch_number == 0) || (g_stitch_number > 84))
                g_stitch_number = 1;
            g_selectmode = 0;
            temp_stitch_number = g_stitch_number;
            tmp_parameter = g_pitch_parameter;
            editflag = 0;
        }
        if (tmp_selectmode && (editflag == 0))
        {
            editflag = 1;
        }
        tmp_selectmode = g_selectmode;
        maxfeed = 127;

        for (i = 1; i <= RECT_MODIFY1_ST_STR; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis,
                         rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MODIFY1_X_TIT || i == RECT_MODIFY1_Y_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY1_ST_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((title + 8) * TitleLengthMax)),
                             SheetPage, (title + 8), rect[i].colorT[0]); //�������������
            }
        }

        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedc, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY1_ST_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY1_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY1_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY1_ST_STR].colorT[0]);

#if MOVEUNIT == 1
        if (g_curPTtype == 2)
        {
            DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
            DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
        }
        else
        {
            DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
            DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
        }
#else
        DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                  g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
        DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                  g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif

        dis_updata1 = 1;
        dis_updata2 = 1;
        dis_updata3 = 1;
        g_tmpoxpos = mult1 * g_xabspos;
        g_tmpoypos = mult1 * g_yabspos;
    }

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                 SheetPage, title, rect[0].colorT[0]); //����

    scale = pat_getScale(patRule.h_patConvasIndex, patRulesTmp);
    vscale = pat_getFinalScale(scale, scale1);

    if (enCanel)
    {
        DrawButton(keyd[KEY_MODIFY1_CANCEL], (U16 *)(ADD_ICONBMP),
                   keybmpind[KEY_MODIFY1_CANCEL].border[0], keybmpind[KEY_MODIFY1_CANCEL].icon[0]);
    }

    if (enZoom)
    {
        for (i = KEY_MODIFY1_ZOUT; i <= KEY_MODIFY1_ZIN; i++)
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                       keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
        DrawButtonDisPat(keyd[KEY_MODIFY1_IMG], keybmpind[KEY_MODIFY1_IMG].border[0], keybmpind[KEY_MODIFY1_IMG].icon[0]);
    }

    //����������ʾ
    for (i = 2; i <= KEY_MODIFY1_PFOOT; i++)
    {
        if (i == KEY_MODIFY1_PAT)
        {
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

    ViewPage(usepageno); //������ʾ����
    if ((P_TYA == 0) || (P_TYA > 15))
        P_TYA = 1;

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_PFOOT + 1, 0);
        if (keydata == 0 && enCanel)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_CANCEL + 1, KEY_MODIFY1_CANCEL);
        }
        if (keydata == 0 && enZoom)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_IMG + 1, KEY_MODIFY1_ZOUT);
        }
        if (keydata == 0 && tonear)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_TOP + 1, KEY_MODIFY1_TOP);
        }
        if (keydata == 0 && endir)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_MDR + 1, KEY_MODIFY1_MUL);
        }
        if (keydata == 0 && ennum)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_9 + 1, KEY_MODIFY1_DUP);
        }

        if (keydata == 0 && enmode)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_MET1 + chSelModeMax, KEY_MODIFY1_MET1);
        }

        if (keydata == 0 && chSelInputMax > 1)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_INP1 + chSelInputMax, KEY_MODIFY1_INP1);
        }
        if (keydata == KEY_MODIFY1_MMSP)
        {
            keydata = 0;
        }

        // if ((keydata >= KEY_MODIFY1_SELWIDTH && keydata <= KEY_MODIFY1_DIRR && tmp_selectmode != 4) ||
        //     (keydata == KEY_MODIFY1_STITCH && tmp_selectmode))
        // {
        //     keydata = 0;
        // }
        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon >= KEY_MODIFY1_MUL && keyon <= KEY_MODIFY1_MDR && keyon != KEY_MODIFY1_MMSP) ||
                (keyon >= KEY_MODIFY1_DUP && keyon <= KEY_MODIFY1_DDOWN) ||
                (keyon >= KEY_MODIFY1_ZOUT && keyon <= KEY_MODIFY1_IMG))
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
            {
                keyon = keydata;
                pressKey = 1;
                if (keyon == KEY_MODIFY1_SCRNSVR || keyon == KEY_MODIFY1_IMG) //�ް�����Ч
                {
                }
                //�ǳ��水��-//������ɫ���ְ���
                else if (keyon == KEY_MODIFY1_INP1 || keyon == KEY_MODIFY1_INP2)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //������������
                ll = 1;
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
                {
                    beeponf = 1;

                    if (g_keyBellT) //����ʱ����������ʱ��
                        Beep_On(g_keyBellT);

                    switch (keyon) //�������±�����ʾ����ʾ
                    {
                    case KEY_MODIFY1_MET1:
                    case KEY_MODIFY1_MET2:
                    case KEY_MODIFY1_MET3:
                    case KEY_MODIFY1_MET4:
                        DrawFMenuStr(keyTitList[para][keyon - KEY_MODIFY1_MET1]);
                        break;
                    case KEY_MODIFY1_INP1:
                    case KEY_MODIFY1_INP2:
                        DrawFMenuStr(rectTitList[para][keyon - KEY_MODIFY1_INP1 + 2]);
                        break;
                    case KEY_MODIFY1_PFOOT:
                        DrawFMenuStr((pfootf == 0) ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
                        break;
                    case KEY_MODIFY1_0:
                    case KEY_MODIFY1_1:
                    case KEY_MODIFY1_2:
                    case KEY_MODIFY1_3:
                    case KEY_MODIFY1_4:
                    case KEY_MODIFY1_5:
                    case KEY_MODIFY1_6:
                    case KEY_MODIFY1_7:
                    case KEY_MODIFY1_8:
                    case KEY_MODIFY1_9:
                        BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                        DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_MODIFY1_0, 1, rect[0].colorT[0]);
                        break;
                    default:
                        DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                        break;
                    }
                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                    delay = KEYNT;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }
                Times = 0; //��ǰ
                if ((keyon >= KEY_MODIFY1_MUL && keyon <= KEY_MODIFY1_MDR && keyon != KEY_MODIFY1_MMSP) ||
                    (keyon >= KEY_MODIFY1_DUP && keyon <= KEY_MODIFY1_DDOWN) ||
                    (keyon >= KEY_MODIFY1_ZOUT && keyon <= KEY_MODIFY1_IMG))
                {
                    keyoff = keyon;
                }
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            pressKey = 1;
            if ((keyoff >= KEY_MODIFY1_MUL) && (keyoff <= KEY_MODIFY1_MDR) && (keyoff != KEY_MODIFY1_MMSP)) // �����
            {
                if ((!(S_CLAMP)) && (P_OST == 0))
                {
                    pno = MENU_RWHELP;
                    hleppara = 13;
                    poldno[pnoindex++] = MENU_MODIFY1;
                }
                else
                {
                    if ((g_dxpos == 0) && (g_dypos == 0))
                    {
                        if ((P_TYA == 0) || (P_TYA > 15))
                            P_TYA = 1;
                    }
                    getNearflag = 1;
                    tmpxpos = g_dxpos;
                    tmpypos = g_dypos;

                    if ((keyoff == KEY_MODIFY1_MUR) || (keyoff == KEY_MODIFY1_MMR) || (keyoff == KEY_MODIFY1_MDR))
                    {
                        g_dxpos += ll;
                    }
                    else if ((keyoff == KEY_MODIFY1_MUL) || (keyoff == KEY_MODIFY1_MML) || (keyoff == KEY_MODIFY1_MDL))
                    {
                        g_dxpos -= ll;
                    }
                    if ((keyoff == KEY_MODIFY1_MUL) || (keyoff == KEY_MODIFY1_MUM) || (keyoff == KEY_MODIFY1_MUR))
                    {
                        g_dypos += ll;
                    }
                    else if ((keyoff == KEY_MODIFY1_MDL) || (keyoff == KEY_MODIFY1_MDM) || (keyoff == KEY_MODIFY1_MDR))
                    {
                        g_dypos -= ll;
                    }

                    mvxx = g_tmpoxpos + mult1 * g_dxpos;
                    mvyy = g_tmpoypos + mult1 * g_dypos;
                    if (mvxx < mult1 * mult3 * g_Mac.max_xl)
                        g_dxpos = (mult1 * mult3 * g_Mac.max_xl - g_tmpoxpos) / mult1;
                    if (mvxx > mult1 * mult3 * g_Mac.max_xr)
                        g_dxpos = (mult1 * mult3 * g_Mac.max_xr - g_tmpoxpos) / mult1;
                    if (mvyy > mult1 * mult3 * g_Mac.max_yu)
                        g_dypos = (mult1 * mult3 * g_Mac.max_yu - g_tmpoypos) / mult1;
                    if (mvyy < mult1 * mult3 * g_Mac.max_yd)
                        g_dypos = (mult1 * mult3 * g_Mac.max_yd - g_tmpoypos) / mult1;

#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll / 2, MENU_MODIFY1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                    }
                    else
                    {
                        f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll, MENU_MODIFY1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                    }
#else
                    f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll, MENU_MODIFY1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
#endif

                    if (f == 0)
                    {
                        g_dxpos = tmpxpos;
                        g_dypos = tmpypos;
                    }
                    if (ll < P_TYA * 8)
                        ll += P_TYA;
                    else
                        ll += P_TYA * 2;
                    dat = 20 + P_TYA * 5;
                    if (ll > dat)
                        ll = dat;
                    dis_updata15 = 1;
                    if (g_tmpoxpos + mult1 * g_dxpos > g_maxX || g_tmpoxpos + mult1 * g_dxpos < g_minX || mult2 * (g_tmpoypos + mult1 * g_dypos) > g_maxY || mult2 * (g_tmpoypos + mult1 * g_dypos) < g_minY)
                    {
                        if (g_tmpoxpos + mult1 * g_dxpos > g_maxX)
                        {
                            g_maxX = g_tmpoxpos + mult1 * g_dxpos;
                        }
                        if (g_tmpoxpos + mult1 * g_dxpos < g_minX)
                        {
                            g_minX = g_tmpoxpos + mult1 * g_dxpos;
                        }
                        if (mult2 * (g_tmpoypos + mult1 * g_dypos) > g_maxY)
                        {
                            g_maxY = mult2 * (g_tmpoypos + mult1 * g_dypos);
                        }
                        if (mult2 * (g_tmpoypos + mult1 * g_dypos) < g_minY)
                        {
                            g_minY = mult2 * (g_tmpoypos + mult1 * g_dypos);
                        }
                        dis_updata2 = 1;
                        // dis_updata15 = 1;
                        scale = pat_getScale(patRule.h_patConvasIndex, patRulesTmp);
                        vscale = pat_getFinalScale(scale, scale1);
                        onearpx = 0;
                        onearpy = 0;
                    }
                    else
                    {
                        posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
                        posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
                        if (((posx1 < keyd[KEY_MODIFY1_IMG].xpis) ||
                             (posx1 > keyd[KEY_MODIFY1_IMG].xpis + keyd[KEY_MODIFY1_IMG].xsize) ||
                             (posy1 < keyd[KEY_MODIFY1_IMG].ypis) ||
                             (posy1 > keyd[KEY_MODIFY1_IMG].ypis + keyd[KEY_MODIFY1_IMG].ysize)))
                        { //������ʾ��Χ
                            dis_updata2 = 1;
                        }
                        else
                        {
                            RecoveData2();
                            ReadPointData2(posx1 - 4, posy1 - 4);
                            Draw_SmallCrossLimit(posx1, posy1, COLOR_CROSS, patRule.h_patConvasIndex);
                        }
                    }
                }
            }
            else if (keyoff >= KEY_MODIFY1_DUP && keyoff <= KEY_MODIFY1_9) //���ּ�
            {
                if ((para == 5) && (tmp_selectmode == 4 || (tmp_selectmode < 4 && g_modzig)))
                { //z�����
                    if (keyoff == KEY_MODIFY1_DUP)
                        tmp_zigwp++;
                    else if (keyoff == KEY_MODIFY1_DDOWN)
                    {
                        if (tmp_zigwp)
                            tmp_zigwp--;
                    }
                    else
                    {
                        tmp_zigwp = (tmp_zigwp % 100) * 10 + (keyoff - KEY_MODIFY1_0);
                        if (tmp_zigwp > 100)
                            tmp_zigwp = tmp_zigwp % 100;
                    }
                    if (tmp_zigwp > 100)
                        tmp_zigwp = 100;

                    if (editflag)
                    {
                        g_zig_pitch = (U8)(tmp_zigwp);
                        if (tmp_selectmode == 4)
                            dis_updata11 = 1;
                        else
                            dis_updata13 = 1;
                    }
                    else
                    {
                        g_zig_width = (U8)(tmp_zigwp);
                        if (tmp_selectmode == 4)
                            dis_updata10 = 1;
                        else
                            dis_updata12 = 1;
                    }
                }
                else if (para == 6)
                {                      //�޸����
                    if (editflag == 0) //����
                    {
                        if (keyoff == KEY_MODIFY1_DUP)
                            temp_stitch_number++;
                        else if (keyoff == KEY_MODIFY1_DDOWN)
                        {
                            if (temp_stitch_number)
                                temp_stitch_number--;
                        }
                        else
                        {
                            temp_stitch_number = (temp_stitch_number % 10) * 10 + (keyoff - KEY_MODIFY1_0);
                            if (temp_stitch_number > 84)
                                temp_stitch_number = (temp_stitch_number % 10);
                        }
                        if (temp_stitch_number > 84)
                            temp_stitch_number = 84;
                        dis_updata10 = 1;
                        dis_updata2 = 1;
                    }
                    else if (editflag == 1) //���
                    {
                        if (keyoff == KEY_MODIFY1_DUP)
                            tmp_parameter++;
                        else if (keyoff == KEY_MODIFY1_DDOWN)
                        {
                            if (tmp_parameter)
                                tmp_parameter--;
                        }
                        else
                        {
                            tmp_parameter = (tmp_parameter % 100) * 10 + (keyoff - KEY_MODIFY1_0);
                            if (tmp_parameter > 127)
                                tmp_parameter = tmp_parameter % 100;
                        }
                        if (tmp_parameter > 127)
                            tmp_parameter = 127;
                        dis_updata11 = 1;
                    }
                }
                else
                {
                    if (keyoff == KEY_MODIFY1_DUP)
                    {
                        if (tmp_parameter < maxfeed)
                        {
                            tmp_parameter++;
                            dis_updata4 = 1;
                        }
                    }
                    else if (keyoff == KEY_MODIFY1_DDOWN)
                    {
                        if (tmp_parameter > 1)
                        {
                            tmp_parameter--;
                            dis_updata4 = 1;
                        }
                    }
                    else
                    {
                        tmp_parameter = (tmp_parameter % moddat) * 10 + keyoff - KEY_MODIFY1_0;
                        if (tmp_parameter > maxfeed)
                            tmp_parameter %= moddat;
                        if (tmp_parameter < 1)
                            tmp_parameter = 1;
                        dis_updata4 = 1;
                    }

                    if (para == 1)
                    { //��ɾ��
                        dis_updata2 = 1;
                    }
                }

                //				if(scale1 !=0 || dorgx!=0 || dorgy!=0){	//ʵ��������ʾ
                //					scale1 = 0;
                //					dorgx = 0;
                //					dorgy = 0;
                //					dis_updata2 = 1;
                //				}
            }
            else if (keyoff >= KEY_MODIFY1_MET1 && keyoff <= KEY_MODIFY1_MET4)
            {
                switch (para)
                {
                case 0: //��������ģʽѡ��
                    tmp_selectmode = keyoff - KEY_MODIFY1_MET1;
                    dis_updata3 = 1;
                    break;
                case 1: //��ɾ��ģʽѡ��
                    tmp_selectmode = keyoff - KEY_MODIFY1_MET1;
                    dis_updata3 = 1;
                    dis_updata2 = 1;
                    if (tmp_selectmode == 2)
                    {
                        dis_updata17 = 1;
                        dis_updata1 = 1;
                        ennum = 0;
                        endir = 0;
                    }
                    else
                    {
                        dis_updata18 = 1;
                        dis_updata1 = 1;
                        ennum = 1;
                        endir = 0;
                    }
                    break;
                case 2: //��׷��ģʽ
                    tmp_selectmode = keyoff - KEY_MODIFY1_MET1;
                    dis_updata3 = 1;
                    break;
                case 3: //������ģʽ
                    tmp_selectmode = keyoff - KEY_MODIFY1_MET1;
                    dis_updata3 = 1;
                    break;
                case 4: //�����ƶ�ģʽ
                    tmp_selectmode = keyoff - KEY_MODIFY1_MET1;
                    dis_updata3 = 1;
                    break;
                case 5: //��������ģʽ
                    break;
                case 6: //�������ģʽѡ��
                    tmp_selectmode = keyoff - KEY_MODIFY1_MET1;
                    if (tmp_selectmode)
                    {
                        if (editflag == 0)
                            editflag = 1;
                        chSelInputMax = 1;
                    }
                    else
                    {
                        chSelInputMax = 2;
                    }

                    dis_updata3 = 1;
                    dis_updata2 = 1;
                    break;

                default:
                    break;
                }
            }
            else if (keyoff >= KEY_MODIFY1_INP1 && keyoff <= KEY_MODIFY1_INP2)
            {
                switch (para)
                {
                case 0: //��������ģʽѡ��
                    break;
                case 1:    //��ɾ��ģʽѡ��
                    break; //��׷��ģʽ
                case 2:
                    break;
                case 3: //������ģʽ
                    break;
                case 4: //�����ƶ�ģʽ
                    break;
                case 5: //��������ģʽ
                    break;
                case 6: //�������ģʽѡ��
                    editflag = keyoff - KEY_MODIFY1_INP1;
                    dis_updata6 = 1;
                    break;

                default:
                    break;
                }
            }
#if 0
            else if (keyoff >= KEY_MODIFY1_ORGM1 && keyoff <= KEY_MODIFY1_ORGM2)
            { //��������ģʽѡ��
                tmp_selectmode = keyoff - KEY_MODIFY1_ORGM1;
                dis_updata3 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_DELNABS && keyoff <= KEY_MODIFY1_DELALL)
            { //��ɾ��ģʽѡ��
                tmp_selectmode = keyoff - KEY_MODIFY1_DELNABS;
                dis_updata3 = 1;
                dis_updata2 = 1;
            }

            else if (keyoff >= KEY_MODIFY1_ADDABS && keyoff <= KEY_MODIFY1_ADDSAME)
            { //��׷��ģʽ
                tmp_selectmode = keyoff - KEY_MODIFY1_ADDABS;
                dis_updata3 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_MOVEABS && keyoff <= KEY_MODIFY1_MOVEBLOCK)
            { //������ģʽ
                tmp_selectmode = keyoff - KEY_MODIFY1_MOVEABS;
                dis_updata3 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_CHANGE && keyoff <= KEY_MODIFY1_FIXED)
            { //�����ƶ�ģʽ
                tmp_selectmode = keyoff - KEY_MODIFY1_CHANGE;
                dis_updata3 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_LINE && keyoff <= KEY_MODIFY1_FEED)
            { //��������ģʽ
                if (tmp_selectmode != keyoff - KEY_MODIFY1_LINE)
                {
                    if (tmp_selectmode == 4 || keyoff - KEY_MODIFY1_LINE == 4 || keyoff - KEY_MODIFY1_LINE == 5)
                    {
                        BmpFillRect(80, 144, 420, 224);
                    }
                    if (tmp_selectmode == 5)
                    {
                        dis_updata1 = 1;
                    }
                }
                tmp_selectmode = keyoff - KEY_MODIFY1_LINE;
                dis_updata3 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_STITCH && keyoff <= KEY_MODIFY1_ANG)
            { //���,�����л�
                editflag = keyoff - KEY_MODIFY1_STITCH;
                dis_updata6 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_PITCHN && keyoff <= KEY_MODIFY1_PITCHALL)
            { //�������ģʽѡ��
                tmp_selectmode = keyoff - KEY_MODIFY1_PITCHN;
                if (tmp_selectmode && (editflag == 0))
                {
                    editflag = 1;
                }
                dis_updata3 = 1;
                dis_updata2 = 1;
            }
            
            else if (keyoff >= KEY_MODIFY1_SELWIDTH && keyoff <= KEY_MODIFY1_SELPITCH)
            { //z�����ѡ��
                editflag = keyoff - KEY_MODIFY1_SELWIDTH;
                if (editflag == 0)
                {
                    tmp_zigwp = g_zig_width;
                }
                else
                {
                    tmp_zigwp = g_zig_pitch;
                }
                dis_updata5 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_DIRL && keyoff <= KEY_MODIFY1_DIRR)
            { //z�췽��
                g_zig_direction = keyon - KEY_MODIFY1_DIRL;
                dis_updata5 = 1;
            }

            else if (keyoff >= KEY_MODIFY1_NOZIG && keyoff <= KEY_MODIFY1_ZIG)
            {
                g_modzig = keyoff - KEY_MODIFY1_NOZIG;
                dis_updata14 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_ZIGWIDTH && keyoff <= KEY_MODIFY1_ZIGPITCH)
            { //z�����ѡ��
                editflag = keyoff - KEY_MODIFY1_ZIGWIDTH;
                if (editflag == 0)
                {
                    tmp_zigwp = g_zig_width;
                }
                else
                {
                    tmp_zigwp = g_zig_pitch;
                }
                dis_updata14 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_ZIGDIRL && keyoff <= KEY_MODIFY1_ZIGDIRR)
            { //z�췽��
                g_zig_direction = keyon - KEY_MODIFY1_ZIGDIRL;
                dis_updata14 = 1;
            }
#endif
            else
            {
                switch (keyoff)
                {
                case KEY_MODIFY1_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    g_parameter = tmp_parameter;
                    break;

                case KEY_MODIFY1_BACK:
                    Send_Pfoot(1, MENU_MODIFY1, 1);
                    if (F_FILETYPE == PAT_MIT)
                    {                  //mit
                        if (para == 0) //���������
                        {

                            g_dxpos = 0;
                            g_dypos = 0;
                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                        }
                        else if (para == 2 || para == 3 || (para == 4 && g_num_mov_flag == 2)) //��׷��,������,�����ƶ�
                        {
                            g_tmpoxpos = 0;
                            g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                            if (SREG[3] == 7) //����-�޸�ԭ��λ��
                            {
                                g_tmpoxpos = (S16)(SREG[6]);
                                g_tmpoypos = (S16)(SREG[7]);
                            }
#endif
                            for (ii = 0; ii < g_jog_point; ii++)
                            {
                                dat = SREG[g_patd.pat_add + ii * 2] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                {
                                    dat = SREG[g_patd.pat_add + ii * 2 + 1];
                                    dat &= 0x000000ff;
                                    if (dat >= 0x80)
                                    {
                                        dat -= 0x80;
                                        g_tmpoxpos -= dat;
                                    }
                                    else
                                    {
                                        g_tmpoxpos += dat;
                                    }
                                    dat = SREG[g_patd.pat_add + ii * 2 + 1];
                                    dat = ((U32)(dat >> 8)) & 0x000000ff;
                                    if (dat >= 0x80)
                                    {
                                        dat -= 0x80;
                                        g_tmpoypos -= dat;
                                    }
                                    else
                                    {
                                        g_tmpoypos += dat;
                                    }
                                }
                            }
                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                            g_modcordflag = 0;
                            g_tmpox = 0;
                            g_tmpoy = 0;
                        }
                        else if (para == 4) //�����ƶ�
                        {
                            if (g_num_mov_flag == 1)
                            {
                                g_num_mov_flag = 2;
                                dis_pos = 1;
                                enmode = 0;
                                ennum = 1;
                                endir = 0;

                                maxfeed = 127;
                                moddat = 100;

                                dis_updata1 = 1;
                                dis_updata16 = 1;
                                dis_updata18 = 1;
                                dis_updata4 = 1;
                                break;
                            }
                        }
                        else if (para == 5) //��������
                        {
                            if (g_num_mov_flag == 1)
                            {
                                g_dxpos = 0;
                                g_dypos = 0;

                                if (g_jogmode == 0)
                                {
                                    pno = MENU_MODIFYADD;
                                    poldno[pnoindex++] = MENU_MODIFY1;
                                    if (g_point_num >= 1)
                                    {
                                        g_tmpoxpos = 0;
                                        g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                        if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                        {
                                            g_tmpoxpos = (S16)(SREG[6]);
                                            g_tmpoypos = (S16)(SREG[7]);
                                        }
#endif
                                        for (ii = 0; ii < g_jog_point; ii++)
                                        {
                                            dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                            {
                                                dat = SREG[tmp_pat_add + ii * 2 + 1];
                                                dat &= 0x000000ff;
                                                if (dat >= 0x80)
                                                {
                                                    dat -= 0x80;
                                                    g_tmpoxpos -= dat;
                                                }
                                                else
                                                {
                                                    g_tmpoxpos += dat;
                                                }
                                                dat = SREG[tmp_pat_add + ii * 2 + 1];
                                                dat = ((U32)(dat >> 8)) & 0x000000ff;
                                                if (dat >= 0x80)
                                                {
                                                    dat -= 0x80;
                                                    g_tmpoypos -= dat;
                                                }
                                                else
                                                {
                                                    g_tmpoypos += dat;
                                                }
                                            }
                                        }
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                                        g_modcordflag = 0;
                                        if (g_jog_point < tmp_feedsum)
                                        {
                                            dat1 = tmp_pat_add + g_jog_point * 2;
                                            dat = SREG[dat1] & 0x00ff;
                                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                            {
                                                dat = SREG[dat1 + 1];
                                                x = (U8)(dat);
                                                y = (U8)((U16)(dat >> 8));
                                                if (x >= 0x80)
                                                {
                                                    x -= 0x80;
                                                    g_dxpos = -x;
                                                }
                                                else
                                                {
                                                    g_dxpos = x;
                                                }
                                                if (y >= 0x80)
                                                {
                                                    y -= 0x80;
                                                    g_dypos = -y;
                                                }
                                                else
                                                {
                                                    g_dypos = y;
                                                }
                                                dat = SREG[dat1];
                                            }
                                            else
                                            {
                                                g_dxpos = 0;
                                                g_dypos = 0;
                                            }
                                        }
                                        else
                                        {
                                            g_dxpos = 0;
                                            g_dypos = 0;
                                            if (tmp_feedsum)
                                                dat = 0xff;
                                            else
                                                dat = 0x1b;
                                        }
                                    }
                                }
                                else
                                {
                                    if (g_point_num >= 1)
                                    {
                                        g_point_num--;

                                        g_tmpoxpos = 0;
                                        g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                        if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                        {
                                            g_tmpoxpos = (S16)(SREG[6]);
                                            g_tmpoypos = (S16)(SREG[7]);
                                        }
#endif
                                        for (ii = 0; ii < g_jog_stpoint; ii++)
                                        {
                                            dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                            {
                                                dat = SREG[tmp_pat_add + ii * 2 + 1];
                                                dat &= 0x000000ff;
                                                if (dat >= 0x80)
                                                {
                                                    dat -= 0x80;
                                                    g_tmpoxpos -= dat;
                                                }
                                                else
                                                {
                                                    g_tmpoxpos += dat;
                                                }
                                                dat = SREG[tmp_pat_add + ii * 2 + 1];
                                                dat = ((U32)(dat >> 8)) & 0x000000ff;
                                                if (dat >= 0x80)
                                                {
                                                    dat -= 0x80;
                                                    g_tmpoypos -= dat;
                                                }
                                                else
                                                {
                                                    g_tmpoypos += dat;
                                                }
                                            }
                                        }
                                        for (ii = 1; ii < g_point_num; ii++)
                                        {
                                            g_tmpoxpos += (S16)(g_array5[ii][0]);
                                            g_tmpoypos += (S16)(g_array5[ii][1]);
                                        }

                                        if (g_point_num > 0)
                                        {
                                            g_dxpos = (S16)(g_array5[g_point_num][0]);
                                            g_dypos = (S16)(g_array5[g_point_num][1]);
                                        }
                                        else
                                        {
                                            g_dxpos = 0;
                                            g_dypos = 0;
                                        }
                                        Comm_Move(g_tmpoxpos + g_dxpos, g_tmpoypos + g_dypos, 50, MENU_MODIFY1, 0, 0);

                                        dis_updata2 = 1;
                                        dis_updata15 = 1;
                                        dis_updata18 = 1;

                                        if (g_point_num == 0)
                                        {
                                            if (g_jog_enpoint > g_jog_stpoint)
                                                g_jog_enpoint = g_jog_stpoint;
                                            //pnoindex--;pno = poldno[pnoindex];point_num=1;//bpno = MENU_MODIFY;	pno = bpno;
                                            pno = MENU_MODIFYADD;
                                            poldno[pnoindex] = MENU_MODIFY1;
                                            pnoindex++;
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    {                                            //sew
                        if (para == 0 || para == 2 || para == 3) //���������,��׷��
                        {
                            if ((!(S_CLAMP)) && (P_OST == 0))
                            {
                                pno = MENU_RWHELP;
                                hleppara = 13;
                                poldno[pnoindex++] = MENU_MODIFY1;
                                break;
                            }
                            g_dxpos = 0;
                            g_dypos = 0;
                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 1, 0);
                        }
                        else if (para == 4) //�����ƶ�
                        {
                            if ((!(S_CLAMP)) && (P_OST == 0))
                            {
                                pno = MENU_RWHELP;
                                hleppara = 13;
                                poldno[pnoindex++] = MENU_MODIFY1;
                                break;
                            }
                            if (g_num_mov_flag == 1)
                            {
                                g_num_mov_flag = 2;
                                dis_pos = 1;
                                enmode = 0;
                                ennum = 1;
                                endir = 0;

                                maxfeed = 127;
                                moddat = 100;

                                dis_updata1 = 1;
                                dis_updata16 = 1;
                                dis_updata18 = 1;
                                dis_updata4 = 1;
                                break;
                            }
                            else
                            {
                                if ((g_dxpos != 0) || (g_dypos != 0))
                                {
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                    Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY, 1, 0);
                                }
                                g_jog_enpoint = 0;
                            }
                        }
                        else if (para == 5)
                        { //��������
                            if (g_num_mov_flag == 1)
                            {
                                if (g_jogmode == 1)
                                {
                                    if (g_point_num >= 1)
                                    {
                                        if ((g_dxpos != 0) || (g_dypos != 0))
                                        {
                                            if ((!(S_CLAMP)) && (P_OST == 0))
                                            {
                                                pno = MENU_RWHELP;
                                                hleppara = 13;
                                                poldno[pnoindex++] = MENU_MODIFY1;
                                                break;
                                            }
                                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 1, 0);
                                        }
                                        g_point_num--;
                                        if (g_point_num > 0)
                                        {
                                            g_dxpos = (S16)(g_array5[g_point_num][0]);
                                            g_dypos = (S16)(g_array5[g_point_num][1]);
                                            g_tmpoxpos = (S16)(g_array10[g_point_num][0]) - g_dxpos;
                                            g_tmpoypos = (S16)(g_array10[g_point_num][1]) - g_dypos;
                                            g_dxpos /= mult1;
                                            g_dypos /= mult1;
                                        }
                                        else
                                        {
                                            g_dxpos = 0;
                                            g_dypos = 0;
                                        }

                                        dis_updata2 = 1;
                                        dis_updata15 = 1;
                                        dis_updata18 = 1;

                                        if (g_point_num == 0)
                                        {
                                            pnoindex--;
                                            pno = poldno[pnoindex];
                                            if (g_jog_enpoint > g_jog_stpoint)
                                                g_jog_enpoint = g_jog_stpoint;
                                        }
                                    }
                                }
                                else
                                {
                                    if (g_point_num >= 1)
                                    {
                                        if ((!(S_CLAMP)) && (P_OST == 0))
                                        {
                                            pno = MENU_RWHELP;
                                            hleppara = 13;
                                            poldno[pnoindex++] = MENU_MODIFY1;
                                            break;
                                        }
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY1, 1, 0);
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                    }
                                    pno = MENU_MODIFYADD;
                                    poldno[pnoindex] = MENU_MODIFY1;
                                    pnoindex++;
                                }
                                break;
                            }
                        }
                    }
                    pno = poldno[--pnoindex];
                    Wait_Move_End();
                    break;

                case KEY_MODIFY1_ENTER:
                    Send_Pfoot(1, MENU_MODIFY1, 1);
                    if (F_FILETYPE == PAT_MIT)
                    {                  //mit
                        if (para == 0) //���������
                        {
                            if ((g_dxpos != 0 || g_dypos != 0) || tmp_selectmode == 1)
                            {
                                pno = MENU_MODIFY;
                                pnoindex--;
                                g_jog_point = 0;
                                g_selectmode = tmp_selectmode;
                                f = Mit_Modify_Data(para);
                                if (f == 1)
                                {
                                    Comm_Move(0, 0, 50, MENU_MODIFY1, 0, 0);
                                }
                            }
                        }
                        else if (para == 1) //��ɾ��
                        {
                            pno = MENU_MODIFY;
                            pnoindex -= 2;
                            g_parameter = tmp_parameter;
                            g_selectmode = tmp_selectmode;
                            if (tmp_selectmode == 2)
                            {
                                g_DelAll_flag = 1;
                            }
                            Mit_Modify_Data(para);
                        }
                        else if (para == 2) //��׷��
                        {
                            pno = MENU_MODIFY;
                            pnoindex -= 2;
                            if (g_AddRela && tmp_selectmode == 0)
                                tmp_selectmode = 1; //��Ϊ���׷��
                            g_selectmode = tmp_selectmode;
                            f = Mit_Modify_Data(para);
                            if (f)
                            {
                                g_jog_point++;

                                g_tmpoxpos = 0;
                                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_tmpoxpos = (S16)(SREG[6]);
                                    g_tmpoypos = (S16)(SREG[7]);
                                }
#endif
                                for (ii = 0; ii < g_jog_point; ii++)
                                {
                                    dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat &= 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoxpos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoxpos += dat;
                                        }
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoypos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoypos += dat;
                                        }
                                    }
                                }
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                                g_modcordflag = 0;
                            }
                        }
                        else if (para == 3) //��λ������
                        {
                            pno = MENU_MODIFY;
                            pnoindex -= 2;
                            g_selectmode = tmp_selectmode;
                            f = Mit_Modify_Data(para);
                            if (f == 1)
                            {
                                g_tmpoxpos = 0;
                                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_tmpoxpos = (S16)(SREG[6]);
                                    g_tmpoypos = (S16)(SREG[7]);
                                }
#endif
                                for (ii = 0; ii < g_jog_point; ii++)
                                {
                                    dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat &= 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoxpos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoxpos += dat;
                                        }
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoypos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoypos += dat;
                                        }
                                    }
                                }
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                                g_modcordflag = 0;
                            }
                        }
                        else if (para == 4) //�����ƶ�
                        {
                            if (g_num_mov_flag == 2)
                            {
                                if (tmp_parameter > 127 || tmp_parameter < 1)
                                    tmp_parameter = 30;
                                g_num_mov_flag = 1;

                                g_parameter = tmp_parameter;
                                g_selectmode = tmp_selectmode;
                                if (g_jog_stpoint > g_jog_enpoint)
                                {
                                    dat = g_jog_stpoint;
                                    g_jog_stpoint = g_jog_enpoint;
                                    g_jog_enpoint = dat;
                                }
                                dis_updata1 = 1;
                                dis_updata3 = 1;
                                dis_updata17 = 1;

                                dis_pos = 1;
                                enmode = 1;
                                endir = 1;
                                ennum = 0;
                            }
                            else
                            {
                                g_selectmode = tmp_selectmode;
                                //printf("----g_selectmode = %d \r\n",g_selectmode);
                                pno = MENU_MODIFY;
                                pnoindex -= 2;
                                Mit_Modify_Data(para);
                                g_tmpoxpos = 0;
                                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_tmpoxpos = (S16)(SREG[6]);
                                    g_tmpoypos = (S16)(SREG[7]);
                                }
#endif
                                for (ii = 0; ii < g_jog_point; ii++)
                                {
                                    dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat &= 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoxpos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoxpos += dat;
                                        }
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoypos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoypos += dat;
                                        }
                                    }
                                }
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                                g_modcordflag = 0;
                            }
                        }
                        else if (para == 5) //��������
                        {
                            if (g_num_mov_flag == 2)
                            {
                                g_num_mov_flag = 1;
                                g_parameter = tmp_parameter;
                                g_selectmode = tmp_selectmode;
                                para = g_modify_method;
                                g_sta_end_flag = 0;
                                if (g_zig_pitch < 5)
                                    g_zig_pitch = 5;
                                if (g_zig_width < 1)
                                    g_zig_width = 1;
                                pno = MENU_MODIFYADD;
                                poldno[pnoindex++] = MENU_MODIFY1;
                            }
                            else
                            {
                                g_array10[g_point_num][0] = g_dxpos;
                                g_array10[g_point_num][1] = g_dypos;

                                g_array5[g_point_num][0] = g_dxpos;
                                g_array5[g_point_num][1] = g_dypos;

                                if (g_selectmode == 2)
                                {
                                    g_imgpara = 201; //205

                                    Comm_Move(g_tmpoxpos, g_tmpoypos, 120, MENU_MODIFYADD, 0, 0);
                                    pno = MENU_MOIMAGE;
                                    poldno[pnoindex] = MENU_MODIFY1;
                                    pnoindex++;
                                }
                                else if (g_jogmode == 0)
                                {
                                    g_point_num++;
                                    g_array5[300 + g_point_num][1] = g_array5[299 + g_point_num][0];
                                    pno = MENU_MODIFYADD;
                                    poldno[pnoindex] = MENU_MODIFY1;
                                    pnoindex++;
                                    if (g_point_num > SPLINESUM_MAX)
                                    {
                                        g_imgpara = 201; //205
                                        Go_Stpoint(MENU_MODIFY1, 0);
                                        pno = MENU_MOIMAGE;
                                    }
                                    else
                                    {
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                                    }
                                }
                                else
                                {
                                    if ((g_dxpos == 0) && (g_dypos == 0))
                                    {
                                        g_imgpara = 201; //205
                                        for (ii = g_point_num - 1; ii > 0; ii--)
                                        {
                                            Comm_RelationMove((-(S16)(g_array5[ii][0])), (-(S16)(g_array5[ii][1])), MENU_CURSOR, 0, 0);
                                        }
                                        /////////////////////////////////
                                        pno = MENU_MOIMAGE;
                                        poldno[pnoindex] = MENU_MODIFY1;
                                        pnoindex++;
                                    }
                                    else
                                    {
                                        g_point_num++;
                                        g_tmpoxpos += g_dxpos;
                                        g_tmpoypos += g_dypos;
                                        g_dxpos = 0;
                                        g_dypos = 0;

                                        dis_updata15 = 1;
                                        dis_updata4 = 1;
                                        dis_updata2 = 1;
                                        if (g_point_num > SPLINESUM_MAX)
                                        {
                                            g_imgpara = 201; //205
                                            for (ii = g_point_num - 1; ii > 0; ii--)
                                            {
                                                Comm_RelationMove((-(S16)(g_array5[ii][0])), (-(S16)(g_array5[ii][1])), MENU_CURSOR, 0, 0);
                                            }
                                            pno = MENU_MOIMAGE;
                                            poldno[pnoindex] = MENU_MODIFY1;
                                            pnoindex++;
                                        }
                                    }
                                }
                            }
                        }
                        else if (para == 6) //��Ŀ��������
                        {
                            g_parameter = tmp_parameter;

                            if (temp_stitch_number < 1)
                                temp_stitch_number = 1;
                            g_stitch_number = temp_stitch_number;
                            g_selectmode = tmp_selectmode;
                            pno = MENU_MODIFY;
                            pnoindex -= 2;

                            f = Mit_Modify_Data(para);
                            g_ModifyMode = 0xff;
                            g_btNumber = 0;
                            if (f == 1)
                            {
                                g_tmpoxpos = 0;
                                g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
                                if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                {
                                    g_tmpoxpos = (S16)(SREG[6]);
                                    g_tmpoypos = (S16)(SREG[7]);
                                }
#endif
                                for (ii = 0; ii < g_jog_point; ii++)
                                {
                                    dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat &= 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoxpos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoxpos += dat;
                                        }
                                        dat = SREG[tmp_pat_add + ii * 2 + 1];
                                        dat = ((U32)(dat >> 8)) & 0x000000ff;
                                        if (dat >= 0x80)
                                        {
                                            dat -= 0x80;
                                            g_tmpoypos -= dat;
                                        }
                                        else
                                        {
                                            g_tmpoypos += dat;
                                        }
                                    }
                                }
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 0, 0);
                                g_modcordflag = 0;
                            }
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //sew
                        if (para == 0)
                        { //���������
                            if ((g_dxpos != 0 || g_dypos != 0) || tmp_selectmode == 1)
                            {
                                g_selectmode = tmp_selectmode;
                                f = Bor_ModOrg(mult1 * g_dxpos, mult1 * g_dypos, g_tmpoxpos, g_tmpoypos, g_selectmode, g_patd.pat_len);
                                if (f == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 65;
                                    poldno[pnoindex] = MENU_MODIFY;
                                    pnoindex++;
                                }
                                else
                                {
                                    pno = MENU_MODIFY;
                                    pnoindex--;
                                }
                                g_jog_point = 0;
                                g_BorSTflag = 1;
                                g_modifyflag = 1;
                                g_Tsavetimes = g_savetimes;
                                g_Tsavetimes++;
                                g_savetimes = g_Tsavetimes;
                                BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                if (g_selectmode == 0)
                                {
                                    Comm_Move(0, 0, 50, MENU_MODIFY1, 1, 0);
                                    g_BorSTflag = 0;
                                }
                            }
                            //Comm_Move(0,0,50,MENU_MODIFY1,1,0);
                        }
                        else if (para == 1)
                        { //��ɾ��
                            f = Bor_ModDelPins(tmp_feedc + 1, tmp_parameter, tmp_selectmode + 1, g_patd.pat_len);
                            if (f == 2)
                            { //���곬����Χ
                                pno = MENU_RWHELP;
                                hleppara = 65;
                                poldno[pnoindex] = MENU_MODIFY;
                                pnoindex++;
                                Go_Home(MENU_MODIFY, 1); //0
                                g_jog_point = 0;
                            }
                            else
                            {
                                pno = MENU_MODIFY;
                                pnoindex -= 2;
                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                                g_tmpoxpos += g_dxpos;
                                g_tmpoypos += g_dypos;
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY, 1, 0);
                            }

                            tmp_feedsum = g_patd.feed_sum;
                            tmp_parameter = 1;
                            g_parameter = tmp_parameter;
                            g_selectmode = tmp_selectmode;

                            g_modifyflag = 1;
                            g_Tsavetimes = g_savetimes;
                            g_Tsavetimes++;
                            g_savetimes = g_Tsavetimes;
                            BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                        }
                        else if (para == 2)
                        { //��׷��
                            f = Bor_ModAddPin((U16 *)&tmp_feedc, mult1 * g_dxpos, mult1 * g_dypos, tmp_selectmode + 1, g_patd.pat_len);
                            if (f == 0)
                            { //���곬��Χ
                                hleppara = 4;
                                pno = MENU_INHELP;
                                pnoindex += 2;
                                poldno[pnoindex++] = MENU_MODIFY1;
                            }
                            else if (f == 2)
                            { //ͼ�γ���Χ
                                pno = MENU_RWHELP;
                                hleppara = 65;
                                poldno[pnoindex++] = MENU_MODIFY;
                                Go_Home(MENU_MODIFY, 1); //0
                                g_jog_point = 0;
                                g_modifyflag = 1;
                                g_Tsavetimes = g_savetimes;
                                g_Tsavetimes++;
                                g_savetimes = g_Tsavetimes;
                                BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                            }
                            else if (f == 4)
                            { //ָ��λ�ô���
                                pno = MENU_RWHELP;
                                hleppara = 66;
                                poldno[pnoindex] = MENU_MODIFY1;
                                pnoindex++;
                            }
                            else
                            {
                                g_jog_point = tmp_feedc;
                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                                g_tmpoxpos += g_dxpos;
                                g_tmpoypos += g_dypos;
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY1, 1, 0);
                                if (f == 1)
                                {
                                    g_modifyflag |= 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                }
                                pno = MENU_MODIFY;
                                pnoindex -= 2;
                            }
                        }
                        else if (para == 3)
                        { //���޸�
                            //#if Debug
                            //printf("tmp_selectmode = %d \r\n",tmp_selectmode);
                            //#endif
                            f = Bor_ModMoPin((U16 *)&tmp_feedc, mult1 * g_dxpos, mult1 * g_dypos, tmp_selectmode + 1, g_patd.pat_len);
                            if (f == 0)
                            { //���곬����Χ
                                hleppara = 4;
                                pno = MENU_INHELP;
                                poldno[pnoindex++] = MENU_MODIFY1;
                            }
                            else if (f == 2)
                            { //ͼ�γ�����Χ
                                pno = MENU_RWHELP;
                                hleppara = 65;
                                poldno[pnoindex++] = MENU_MODIFY;
                                Go_Home(MENU_MODIFY, 1); //0
                                g_jog_point = 0;
                                g_modifyflag = 1;
                                g_Tsavetimes = g_savetimes;
                                g_Tsavetimes++;
                                g_savetimes = g_Tsavetimes;
                                BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                            }
                            else
                            {
                                g_jog_point = tmp_feedc;

                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                                g_tmpoxpos += g_dxpos;
                                g_tmpoypos += g_dypos;
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY1, 1, 0);
                                if (f == 1)
                                {
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                }
                                pno = MENU_MODIFY;
                                pnoindex -= 2;
                            }
                        }
                        else if (para == 4)
                        { //�����ƶ�
                            if (g_num_mov_flag == 2)
                            {
                                if (tmp_parameter > 127 || tmp_parameter < 1)
                                    tmp_parameter = 30;
                                g_num_mov_flag = 1;

                                g_parameter = tmp_parameter;
                                g_selectmode = tmp_selectmode;
                                if (g_jog_stpoint > g_jog_enpoint)
                                {
                                    dat = g_jog_stpoint;
                                    g_jog_stpoint = g_jog_enpoint;
                                    g_jog_enpoint = dat;
                                }

                                dis_updata1 = 1;
                                dis_updata3 = 1;
                                dis_updata17 = 1;

                                dis_pos = 1;
                                enmode = 1;
                                endir = 1;
                                ennum = 0;
                            }
                            else if (g_num_mov_flag == 1)
                            {
                                f = Bor_ModMoveLine(g_jog_stpoint, g_jog_enpoint, mult1 * g_dxpos, mult1 * g_dypos, tmp_parameter, tmp_selectmode + 1, g_patd.pat_len);
                                if (f == 2)
                                { //ͼ�γ�����Χ
                                    pno = MENU_RWHELP;
                                    hleppara = 65;
                                    poldno[pnoindex] = MENU_MODIFY;
                                    pnoindex++;
                                    Go_Home(MENU_MODIFY, 1); //1
                                    g_jog_point = 0;
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                    break;
                                }
                                else
                                {
                                    pno = MENU_MODIFY;
                                    pnoindex -= 2;

                                    Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                                    g_tmpoxpos += g_dxpos;
                                    g_tmpoypos += g_dypos;
                                    Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY, 1, 0);
                                }
                                g_parameter = tmp_parameter;
                                if (f == 1)
                                {
                                    g_modifyflag = 1;
                                    g_Tsavetimes = g_savetimes;
                                    g_Tsavetimes++;
                                    g_savetimes = g_Tsavetimes;
                                    BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                }
                            }
                        }
                        else if (para == 5)
                        { //��������
                            if (g_num_mov_flag == 2)
                            {
                                g_num_mov_flag = 1;
                                g_parameter = tmp_parameter;
                                g_selectmode = tmp_selectmode;
                                para = g_modify_method;
                                g_sta_end_flag = 0;
                                if (g_zig_pitch < 5)
                                    g_zig_pitch = 5;
                                if (g_zig_width < 1)
                                    g_zig_width = 1;
                                pno = MENU_MODIFYADD;
                                poldno[pnoindex++] = MENU_MODIFY1;
                            }
                            else
                            {
                                if (g_selectmode == 2)
                                { //Բ��

                                    g_array10[g_point_num][0] = g_tmpoxpos + mult1 * g_dxpos;
                                    g_array10[g_point_num][1] = g_tmpoypos + mult1 * g_dypos;

                                    g_array5[g_point_num][0] = mult1 * g_dxpos;
                                    g_array5[g_point_num][1] = mult1 * g_dypos;

                                    g_point_num++;
                                    g_imgpara = 201; //205
                                    pno = MENU_MOIMAGE;
                                    poldno[pnoindex] = MENU_MODIFY1;
                                    pnoindex++;
                                    g_tdxpos = mult1 * g_dxpos;
                                    g_tdypos = mult1 * g_dypos;
                                }
                                else if ((g_jogmode == 0) && ((g_selectmode == 1) || (g_selectmode == 3) || (g_selectmode == 5)))
                                { //���ߡ����ߡ�����
                                    g_array10[g_point_num][0] = g_tmpoxpos + mult1 * g_dxpos;
                                    g_array10[g_point_num][1] = g_tmpoypos + mult1 * g_dypos;

                                    g_array5[g_point_num][0] = mult1 * g_dxpos;
                                    g_array5[g_point_num][1] = mult1 * g_dypos;
                                    g_array5[300 + g_point_num][0] = g_jog_point;

                                    g_point_num++;
                                    pno = MENU_MODIFYADD;
                                    poldno[pnoindex] = MENU_MODIFY1;
                                    pnoindex++;
                                    if (g_point_num > SPLINESUM_MAX)
                                    {
                                        g_imgpara = 201; //205
                                        Go_Stpoint(MENU_MODIFY1, 1);
                                        pno = MENU_MOIMAGE;
                                    }
                                    else
                                    {
                                        Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, 1, 0);
                                    }
                                    g_dxpos = 0;
                                    g_dypos = 0;
                                    g_tdxpos = mult1 * g_dxpos;
                                    g_tdypos = mult1 * g_dypos;
                                }
                                else if ((g_selectmode == 1) || (g_selectmode == 3) || (g_selectmode == 5))
                                { //���ߡ����ߡ�����
                                    if ((g_dxpos == 0) && (g_dypos == 0))
                                    {
                                        g_imgpara = 201; //205
                                        for (ii = g_point_num - 1; ii > 0; ii--)
                                        {
                                            Comm_RelationMove((-(S16)(g_array5[ii][0])), (-(S16)(g_array5[ii][1])), MENU_CURSOR, 1, 0);
                                        }
                                        g_tdxpos = mult1 * g_dxpos;
                                        g_tdypos = mult1 * g_dypos;
                                        pno = MENU_MOIMAGE;
                                        poldno[pnoindex] = MENU_MODIFY1;
                                        pnoindex++;
                                    }
                                    else
                                    {
                                        g_tmpoxpos += mult1 * g_dxpos;
                                        g_tmpoypos += mult1 * g_dypos;

                                        g_array10[g_point_num][0] = g_tmpoxpos;
                                        g_array10[g_point_num][1] = g_tmpoypos;

                                        g_array5[g_point_num][0] = mult1 * g_dxpos;
                                        g_array5[g_point_num][1] = mult1 * g_dypos;

                                        g_point_num++;
                                        g_dxpos = 0;
                                        g_dypos = 0;

                                        dis_updata15 = 1;
                                        dis_updata4 = 1;
                                        dis_updata2 = 1;

                                        if (g_point_num > SPLINESUM_MAX)
                                        {
                                            g_imgpara = 201; //205
                                            for (ii = g_point_num - 1; ii > 0; ii--)
                                            {
                                                Comm_RelationMove((-(S16)(g_array5[ii][0])), (-(S16)(g_array5[ii][1])), MENU_CURSOR, 1, 0);
                                            }
                                            pno = MENU_MOIMAGE;
                                            poldno[pnoindex] = MENU_MODIFY1;
                                            pnoindex++;
                                            g_tdxpos = mult1 * g_dxpos;
                                            g_tdypos = mult1 * g_dypos;
                                        }
                                    }
                                }
                            }
                        }
                        else if (para == 6)
                        { //����޸�
                            if (temp_stitch_number < 1)
                                temp_stitch_number = 1;
                            pno = MENU_MODIFY;
                            pnoindex -= 2;
                            if (Setang < 20)
                                Setang = 30;
                            if (Setang > 60)
                                Setang = 30;
                            if (tmp_parameter == 0)
                                tmp_parameter = 1;
                            Bor_ModMoPinSwitch(g_jog_point, temp_stitch_number, tmp_selectmode + 1, tmp_parameter, Setang, g_patd.pat_len);
                            g_modifyflag = 1;
                            g_Tsavetimes = g_savetimes;
                            g_Tsavetimes++;
                            g_savetimes = g_Tsavetimes;
                            BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);

                            Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                            g_tmpoxpos += g_dxpos;
                            g_tmpoypos += g_dypos;
                            Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY, 1, 0);
                        }
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                    }
                    break;

                    // case KEY_MODIFY1_PAT:
                    //     pno = MENU_MOIMAGE;
                    //     poldno[pnoindex] = MENU_MODIFY1;
                    //     pnoindex++;
                    //     g_parameter = tmp_parameter;
                    //     g_selectmode = tmp_selectmode;
                    //     if (para == 0 || para == 2 || para == 3 || (para == 5 && g_num_mov_flag == 2))
                    //     { //���������,��׷��,������,��������
                    //         g_imgpara = 100;
                    //     }
                    //     else if (para == 1)
                    //     { //��ɾ��
                    //         g_imgpara = 101;
                    //     }
                    //     else if (para == 4)
                    //     { //�����ƶ�
                    //         g_imgpara = 102;
                    //     }
                    //     else if (para == 5 && g_num_mov_flag == 1)
                    //     { //��������
                    //         if (g_jogmode == 0)
                    //         {
                    //             g_imgpara = 103;
                    //         }
                    //         else
                    //         {
                    //             g_imgpara = 104;
                    //         }
                    //     }
                    //     break;

                case KEY_MODIFY1_CANCEL:
                    if (para == 0 || para == 2 || para == 3)
                    { //���������,��׷��,������
                        if (g_dxpos != 0 || g_dypos != 0)
                        {
                            g_dxpos = 0;
                            g_dypos = 0;
                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, F_FILETYPE == PAT_MIT ? 0 : 1, 0);

                            //scale1 = 0;
                            //dorgx = 0;
                            //dorgy = 0;
                            dis_updata15 = 1;
                            dis_updata2 = 1;
                        }
                    }
                    else if (para == 1)
                    { //��ɾ��
                        if (tmp_parameter != 1)
                        {
                            tmp_parameter = 1;
                            dis_updata4 = 1;
                            if (tmp_selectmode != 2)
                            { //��ɾ��
                                dis_updata2 = 1;
                            }
                        }
                    }
                    else if (para == 4)
                    { //�����ƶ�
                        if (g_num_mov_flag == 2)
                        {
                            if (tmp_parameter != 1)
                            {
                                tmp_parameter = 1;
                                dis_updata4 = 1;
                            }
                        }
                        else if (g_num_mov_flag == 1)
                        {
                            g_dxpos = 0;
                            g_dypos = 0;
                            Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, F_FILETYPE == PAT_MIT ? 0 : 1, 0);

                            //scale1 = 0;
                            //dorgx = 0;
                            //dorgy = 0;
                            dis_updata15 = 1;
                            dis_updata2 = 1;
                        }
                    }
                    else if (para == 5)
                    { //��������
                        if (g_num_mov_flag == 2)
                        {
                            if (tmp_selectmode == 4)
                            {
                                if (editflag)
                                {
                                    tmp_zigwp = 0;
                                    g_zig_pitch = 0;
                                    dis_updata11 = 1;
                                }
                                else
                                {
                                    tmp_zigwp = 0;
                                    g_zig_width = 0;
                                    dis_updata10 = 1;
                                }
                            }
                            else
                            {
                                if (tmp_parameter != 1)
                                {
                                    tmp_parameter = 1;
                                    dis_updata4 = 1;
                                }
                            }
                        }
                        else
                        {
                            if (g_dxpos != 0 || g_dypos != 0)
                            {
                                g_dxpos = 0;
                                g_dypos = 0;
                                Comm_Move(g_tmpoxpos, g_tmpoypos, 50, MENU_MODIFY1, F_FILETYPE == PAT_MIT ? 0 : 1, 0);

                                //scale1 = 0;
                                //dorgx = 0;
                                //dorgy = 0;
                                dis_updata15 = 1;
                                dis_updata4 = 1;
                                dis_updata2 = 1;
                            }
                        }
                    }
                    else if (para == 6)
                    { //����޸�
                        if (editflag == 0)
                        {
                            temp_stitch_number = 0;
                            dis_updata10 = 1;
                            dis_updata2 = 1;
                        }
                        else if (editflag == 1)
                        {
                            tmp_parameter = 0;
                            dis_updata11 = 1;
                        }
                    }

                    break;

                case KEY_MODIFY1_PFOOT:
                    Send_Pfoot(0, MENU_MODIFY1, 0);
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    break;

                case KEY_MODIFY1_ZOUT: //ͼ����С
                    if (scale1)
                    {
                        scale1 -= 50;
                        dis_updata2 = 2;
                    }
                    else
                    {
                        if (bResetCaonvas)
                        {
                            bResetCaonvas = 0;
                            xo = patConvas.conOrgX;
                            yo = patConvas.conOrgY;
                            scale1 = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            dis_updata2 = 2;
                        }
                    }
                    break;

                case KEY_MODIFY1_ZIN: //ͼ�ηŴ�
                    scale1 += 50;
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_MODIFY1_IMG: //ͼ���ƶ�
                    if (beeponf > 1)
                    {
                        if ((absInt(keyxpct - okeyxpct) <= 10) && (absInt(keyypct - okeyypct) <= 10) && (absInt(keyxpct - stkeyxpct) <= 10) && (absInt(keyypct - stkeyypct) <= 10) && (absInt(okeyxpct - stkeyxpct) <= 10) && (absInt(okeyypct - stkeyypct) <= 10) && (beeponf == 15))
                        {
                            xo = ORG_X;
                            yo = ORG_Y;
                            scale1 = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            Beep_On(BELLT);
                        }
                        else
                        {
                            xo += (keyxpct - okeyxpct);
                            yo += (keyypct - okeyypct);
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                        }
                    }
                    else
                    {
                        okeyxpct = keyxpct;
                        okeyypct = keyypct;
                        stkeyxpct = keyxpct;
                        stkeyypct = keyypct;
                    }
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_MODIFY1_TOP:
                    tmpxpos = g_dxpos;
                    tmpypos = g_dypos;

                    g_dxpos = (nearpx - g_tmpoxpos) / mult1;
                    g_dypos = (nearpy - g_tmpoypos) / mult1;

                    //printf("")
                    f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, 50, MENU_MODIFY1, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);

                    if (f == 0)
                    {
                        g_dxpos = tmpxpos;
                        g_dypos = tmpypos;
                    }

                    dis_updata15 = 1;

                    posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
                    posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
                    if (((posx1 < keyd[KEY_MODIFY1_IMG].xpis) || (posx1 > keyd[KEY_MODIFY1_IMG].xpis + keyd[KEY_MODIFY1_IMG].xsize) || (posy1 < keyd[KEY_MODIFY1_IMG].ypis) || (posy1 > keyd[KEY_MODIFY1_IMG].ypis + keyd[KEY_MODIFY1_IMG].ysize)))
                    { //������ʾ��Χ
                        dis_updata2 = 1;
                    }
                    else
                    {
                        RecoveData2();
                        ReadPointData2(posx1 - 4, posy1 - 4);
                        Draw_SmallCrossLimit(posx1, posy1, COLOR_CROSS, patRule.h_patConvasIndex);
                    }
                    break;
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {
            if (keyon == KEY_MODIFY1_SCRNSVR || keyon == KEY_MODIFY1_IMG) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else if (keyon == KEY_MODIFY1_INP1 || keyon == KEY_MODIFY1_INP2)
            {
                DrawButtonOnZoomInRes(keyd[keyon]);
            }
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            pressKey = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                         SheetPage, title, rect[0].colorT[0]); //����
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_MODIFY1);
        }
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MODIFY1);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[������ťˢ��*begin]**********************************************************
        if (getNearflag && pressKey == 0)
        {
            S32 topPosX = 0, topPosY = 0; //����������-��Ļ����ϵ
            if (getNearflag == 1)
            {
                tonear = getNearPointPos(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, &nearpx, &nearpy);

                topPosX = (S32)(nearpx * (vscale) / 1000 + xorg);
                topPosY = (S32)(-nearpy * (vscale) / 1000 + yorg);
            }
            if (tonear != otonear)
            {
                if (tonear)
                {

                    DrawButton(keyd[KEY_MODIFY1_TOP], (U16 *)(ADD_ICONBMP),
                               keybmpind[KEY_MODIFY1_TOP].border[0], keybmpind[KEY_MODIFY1_TOP].icon[0]);
                    DrawFChar(keyd[KEY_MODIFY1_TOP].xpis + SAFE_DIST4, keyd[KEY_MODIFY1_TOP].ypis + SAFE_DIST9,
                              "X", keybmpind[KEY_MODIFY1_TOP].textColor[0]);
                    DrawFChar(keyd[KEY_MODIFY1_TOP].xpis + SAFE_DIST4, keyd[KEY_MODIFY1_TOP].ypis + SAFE_DIST9 + TYPE_SIZE26,
                              "Y", keybmpind[KEY_MODIFY1_TOP].textColor[0]);

                    Timer_ms1 = 0;

                    // onearpx = nearpx;
                    // onearpy = nearpy;
                }
                else
                {
                    BmpFillRect(keyd[KEY_MODIFY1_TOP].xpis, keyd[KEY_MODIFY1_TOP].ypis, keyd[KEY_MODIFY1_TOP].xsize, keyd[KEY_MODIFY1_TOP].ysize);
                    pat_setPatRule(patRulesTmp);
                    pat_clearConvas(patRule.h_patConvasIndex);
                    pat_drawPattern(patRulesTmp, scale1, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
                    nearpx = 0;
                    nearpy = 0;
                }
                otonear = tonear;
            }
            if (tonear)
            {
                if (onearpx != nearpx || onearpy != nearpy)
                {
                    //nearposx = (S32)((nearpx)*(vscale)/1000+xorg);
                    //nearposy = (S32)(mult2*tmuly*(nearpy)*(vscale)/1000+yorg);

                    DrawFillRect(keyd[KEY_MODIFY1_TOP].xpis + SAFE_DIST4 + TYPE_SIZE1,
                                 keyd[KEY_MODIFY1_TOP].ypis + SAFE_DIST4 + TYPE_SIZE4,
                                 (keyd[KEY_MODIFY1_TOP].xsize - SAFE_DIST4) - (SAFE_DIST4 + TYPE_SIZE1),
                                 (keyd[KEY_MODIFY1_TOP].ysize - SAFE_DIST4) - (SAFE_DIST4 + TYPE_SIZE4),
                                 keybmpind[KEY_MODIFY1_TOP].textColor[1]);

                    onearpx = nearpx;
                    onearpy = nearpy;
                    //����-����������
                    pat_setPatRule(patRulesTmp);
                    pat_clearConvas(patRule.h_patConvasIndex);
                    pat_drawPattern(patRulesTmp, scale1, xo, yo, F_FILETYPE, F_FILELEN, topPosX, topPosY, NULL);
                }

#if MOVEUNIT == 1
                if (g_curPTtype == 2)
                {
                    DrawMFS32P(504, 44, nearpx * 5 / mult1, 7, 2, Color_Black);
                    DrawMFS32P(504, 68, nearpy * 5 / mult1, 7, 2, Color_Black);
                }
                else
                {
                    DrawMFS32P(504, 44, nearpx / mult1, 6, 1, Color_Black);
                    DrawMFS32P(504, 68, nearpy / mult1, 6, 1, Color_Black);
                }
#else
                // DrawMFS32P(504, 44, nearpx / mult1, 6, 1, Color_Black);
                // DrawMFS32P(504, 68, nearpy / mult1, 6, 1, Color_Black);

                DrawFS32P(keyd[KEY_MODIFY1_TOP].xpis + SAFE_DIST4 + TYPE_SIZE1, keyd[KEY_MODIFY1_TOP].ypis + SAFE_DIST9,
                          nearpx / mult1, 5, 1, keybmpind[KEY_MODIFY1_TOP].textColor[0]);
                DrawFS32P(keyd[KEY_MODIFY1_TOP].xpis + SAFE_DIST4 + TYPE_SIZE1, keyd[KEY_MODIFY1_TOP].ypis + SAFE_DIST9 + TYPE_SIZE26,
                          nearpy / mult1, 5, 1, keybmpind[KEY_MODIFY1_TOP].textColor[0]);
#endif

                //ViewPoint(posx1,posy1,COLOR_CROSS,1);
            }
            getNearflag = 0;
        }
        // /[������ťˢ��*end]**********************************************************

        // /[���ְ�ť�뷽��ťˢ��*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
            if (para == 0 || para == 2 || para == 3) //���������/��׷��/������
            {
                if (ennum == 0 && endir == 1)
                {
                    BmpFillRect(keyd[KEY_MODIFY1_1].xpis, keyd[KEY_MODIFY1_1].ypis,
                                keyd[KEY_MODIFY1_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY1_1].ysize * 4 + SAFE_DIST5 * 3);

                    for (i = KEY_MODIFY1_MUL; i <= KEY_MODIFY1_MDR; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);

                        if (i == KEY_MODIFY1_MMSP)
                            DrawColor_Gray1(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
                    }
                }
                else
                {
                    BmpFillRect(keyd[KEY_MODIFY1_MUL].xpis, keyd[KEY_MODIFY1_MUL].ypis,
                                keyd[KEY_MODIFY1_MUL].xsize * 3,
                                keyd[KEY_MODIFY1_MUL].ysize * 3);
                }
            }
            else if (para == 1 || para == 6) //��ɾ��,/�������
            {
                if (ennum == 1 && endir == 0)
                {

                    BmpFillRect(keyd[KEY_MODIFY1_MUL].xpis, keyd[KEY_MODIFY1_MUL].ypis,
                                keyd[KEY_MODIFY1_MUL].xsize * 3,
                                keyd[KEY_MODIFY1_MUL].ysize * 3);
                    for (i = KEY_MODIFY1_DUP; i <= KEY_MODIFY1_9; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                }
                else
                {
                    BmpFillRect(keyd[KEY_MODIFY1_1].xpis, keyd[KEY_MODIFY1_1].ypis,
                                keyd[KEY_MODIFY1_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY1_1].ysize * 4 + SAFE_DIST5 * 3);
                }
            }
            else if (para == 4 || para == 5) //�����ƶ�/��������
            {
                if (ennum == 1 && endir == 0)
                {
                    BmpFillRect(keyd[KEY_MODIFY1_MUL].xpis, keyd[KEY_MODIFY1_MUL].ypis,
                                keyd[KEY_MODIFY1_MUL].xsize * 3,
                                keyd[KEY_MODIFY1_MUL].ysize * 3);
                    for (i = KEY_MODIFY1_DUP; i <= KEY_MODIFY1_9; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                }
                else if (ennum == 0 && endir == 1)
                {
                    BmpFillRect(keyd[KEY_MODIFY1_1].xpis, keyd[KEY_MODIFY1_1].ypis,
                                keyd[KEY_MODIFY1_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY1_1].ysize * 4 + SAFE_DIST5 * 3);
                    for (i = KEY_MODIFY1_MUL; i <= KEY_MODIFY1_MDR; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);

                        if (i == KEY_MODIFY1_MMSP)
                            DrawColor_Gray1(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
                    }
                }
                else
                {
                    BmpFillRect(keyd[KEY_MODIFY1_MUL].xpis, keyd[KEY_MODIFY1_MUL].ypis,
                                keyd[KEY_MODIFY1_MUL].xsize * 3,
                                keyd[KEY_MODIFY1_MUL].ysize * 3);
                    BmpFillRect(keyd[KEY_MODIFY1_1].xpis, keyd[KEY_MODIFY1_1].ypis,
                                keyd[KEY_MODIFY1_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY1_1].ysize * 4 + SAFE_DIST5 * 3);
                }
            }
        }
        // /[���ְ�ť�뷽��ťˢ��*end]**********************************************************

        // /[ˢ�»�����ʾ*begin]**********************************************************
        if (dis_updata2)
        {
            DrawFillRect(keyd[KEY_MODIFY1_IMG].xpis, keyd[KEY_MODIFY1_IMG].ypis,
                         keyd[KEY_MODIFY1_IMG].xsize, keyd[KEY_MODIFY1_IMG].ysize, CONVAS_COLOR_B);
            if (para == 0 || para == 2 || para == 3)
            { //���������,��׷��,������,
                i = 0;
                patRulesTmp = PATRULES_MODIFY1_SINGLE;
            }
            else if (para == 6)
            { //���
                i = 26;
                patRulesTmp = PATRULES_MODIFY1_MUL_A_X;
            }
            else if (para == 1)
            { //��ɾ��
                i = 5;
                patRulesTmp = PATRULES_MODIFY1_MUL_A_X;
            }
            else if (para == 5 && g_num_mov_flag == 1)
            { //��������
                if (g_jogmode == 0)
                {
                    i = 10;
                    patRulesTmp = PATRULES_MODIFY1_MUL_A_B0;
                }
                else
                {
                    i = 11;
                    patRulesTmp = PATRULES_MODIFY1_MUL_A_B1;
                }
            }
            else if (para == 4 || para == 5)
            { //�����ƶ�/��������
                i = 7;
                patRulesTmp = PATRULES_MODIFY1_SINGLE_A_B;
            }
            vscale = pat_getFinalScale(scale, scale1);
            xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
            yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
            posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
            posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
            if (((posx1 < keyd[KEY_MODIFY1_IMG].xpis) || (posx1 > keyd[KEY_MODIFY1_IMG].xpis + keyd[KEY_MODIFY1_IMG].xsize) || (posy1 < keyd[KEY_MODIFY1_IMG].ypis) || (posy1 > keyd[KEY_MODIFY1_IMG].ypis + keyd[KEY_MODIFY1_IMG].ysize)))
            { //������ʾ��Χ
                //printf("*--------------------------*\r\n");
                xorg = ORG_X;
                yorg = ORG_Y;
                xorg += (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg += tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;

                xo = xorg + (xorg - (S32)((g_tmpoxpos + mult1 * g_dxpos) * vscale / 1000 + xorg));
                yo = yorg + (yorg - (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * vscale / 1000 + yorg));

                xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
            }

            tmp_parameter1 = g_parameter;
            if (para == 1)
            {
                g_parameter = (tmp_selectmode == 2 ? g_patd.feed_sum : tmp_parameter);
            }
            else if (para == 6)
            {
                g_parameter = (tmp_selectmode == 1 ? g_patd.feed_sum : temp_stitch_number);
            }
            else
            {
                g_parameter = tmp_parameter;
            }
            pat_setPatRule(patRulesTmp);
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(patRulesTmp, scale1, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
            // newDrawPat(i, scale1, xo, yo, 0, 0);

            g_parameter = tmp_parameter1;
            // RecoveData2();
            // ReadPointData2(posx1 - 4, posy1 - 4);
            // ViewPoint(posx1, posy1, COLOR_SELECT_P, 1);
            dis_updata2 = 0;
        }
        // /[ˢ�»�����ʾ*end]**********************************************************

        // /[��������ģʽ����*begin]**********************************************************
        if (dis_updata16)
        {
            dis_updata16 = 0;
            BmpFillRect(rect[RECT_MODIFY1_METHOD].xpis, rect[RECT_MODIFY1_METHOD].ypis,
                        rect[RECT_MODIFY1_METHOD].xsize + keyd[KEY_MODIFY1_MET1].xsize * chSelModeMax, rect[RECT_MODIFY1_METHOD].ysize);
        }
        // /[��������ģʽ����*end]**********************************************************

        // /[����edit��ʾ para == 1|2|3|4*begin]**********************************************************
        if (dis_updata17)
        {
            dis_updata17 = 0;
            if (chSelInputMax == 1 || chSelInputMax == 0)
            {
                BmpFillRect(rect[RECT_MODIFY1_INP1_TIT].xpis, rect[RECT_MODIFY1_INP1_TIT].ypis,
                            rect[RECT_MODIFY1_INP1_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY1_INP1].xsize,
                            rect[RECT_MODIFY1_INP1_TIT].ysize + keyd[KEY_MODIFY1_INP1].ysize);
            }
            if (chSelInputMax == 2 || chSelInputMax == 0)
            {
                BmpFillRect(rect[RECT_MODIFY1_INP2_TIT].xpis, rect[RECT_MODIFY1_INP2_TIT].ypis,
                            rect[RECT_MODIFY1_INP2_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY1_INP2].xsize,
                            rect[RECT_MODIFY1_INP2_TIT].ysize + keyd[KEY_MODIFY1_INP2].ysize);
            }
        }
        // /[����edit��ʾ para == 1|2|3|4*end]**********************************************************

        // /[��ʾedit para == 1|2|3|4*begin]**********************************************************
        if (dis_updata18)
        {
            dis_updata18 = 0;
            if (para == 1)
            {
                dis_updata4 = 1;
                for (i = 0; i < chSelInputMax; i++)
                {
                    DrawFillRect(rect[i + RECT_MODIFY1_INP1_TIT].xpis, rect[i + RECT_MODIFY1_INP1_TIT].ypis,
                                 rect[i + RECT_MODIFY1_INP1_TIT].xsize, rect[i + RECT_MODIFY1_INP1_TIT].ysize, rect[i + RECT_MODIFY1_INP1_TIT].colorB[0]);

                    DrawButtonEdit(keyd[i + KEY_MODIFY1_INP1],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].textColor[0] : keybmpind[i + KEY_MODIFY1_INP1].icon[0],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].icon[1] : keybmpind[i + KEY_MODIFY1_INP1].icon[0]);
                }
                Draw24x24Str(rect[RECT_MODIFY1_INP1_TIT].xpis + (rect[RECT_MODIFY1_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[0]) * TitleLengthMax))) / 2,
                             rect[RECT_MODIFY1_INP1_TIT].ypis + (rect[RECT_MODIFY1_INP1_TIT].ysize - 24) / 2,
                             (MEM_TITLE_ADD + (U32)(rectInTitList[0] * TitleLengthMax)),
                             SheetPage, rectInTitList[0], rect[RECT_MODIFY1_INP1_TIT].colorT[0]); //����

                maxfeed = tmp_feedsum - tmp_feedc - 1;
                DrawF16Str(keyd[KEY_MODIFY1_INP1].xpis, keyd[KEY_MODIFY1_INP1].ypis + keyd[KEY_MODIFY1_INP1].ysize + SAFE_DIST4,
                           "(1-     )", keybmpind[KEY_MODIFY1_INP1].textColor[0]);

                DrawFU32(keyd[KEY_MODIFY1_INP1].xpis + 3 * 9, keyd[KEY_MODIFY1_INP1].ypis + keyd[KEY_MODIFY1_INP1].ysize + SAFE_DIST4,
                         maxfeed, 5, keybmpind[KEY_MODIFY1_INP1].textColor[0]);
            }
            else if (para == 4)
            {
                dis_updata4 = 1;
                for (i = 0; i < chSelInputMax; i++)
                {
                    DrawFillRect(rect[i + RECT_MODIFY1_INP1_TIT].xpis, rect[i + RECT_MODIFY1_INP1_TIT].ypis,
                                 rect[i + RECT_MODIFY1_INP1_TIT].xsize, rect[i + RECT_MODIFY1_INP1_TIT].ysize, rect[i + RECT_MODIFY1_INP1_TIT].colorB[0]);

                    DrawButtonEdit(keyd[i + KEY_MODIFY1_INP1],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].textColor[0] : keybmpind[i + KEY_MODIFY1_INP1].icon[0],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].icon[1] : keybmpind[i + KEY_MODIFY1_INP1].icon[0]);
                }
                Draw24x24Str(rect[RECT_MODIFY1_INP1_TIT].xpis + (rect[RECT_MODIFY1_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[1]) * TitleLengthMax))) / 2,
                             rect[RECT_MODIFY1_INP1_TIT].ypis + (rect[RECT_MODIFY1_INP1_TIT].ysize - 24) / 2,
                             (MEM_TITLE_ADD + (U32)(rectInTitList[1] * TitleLengthMax)),
                             SheetPage, rectInTitList[1], rect[RECT_MODIFY1_INP1_TIT].colorT[0]); //���

                DrawF16Str(keyd[KEY_MODIFY1_INP1].xpis, keyd[KEY_MODIFY1_INP1].ypis + keyd[KEY_MODIFY1_INP1].ysize + SAFE_DIST4,
                           "(1-12.7)", keybmpind[KEY_MODIFY1_INP1].textColor[0]);
            }
            else if (para == 5)
            {
                dis_updata4 = 1;
                for (i = 0; i < chSelInputMax; i++)
                {
                    DrawFillRect(rect[i + RECT_MODIFY1_INP1_TIT].xpis, rect[i + RECT_MODIFY1_INP1_TIT].ypis,
                                 rect[i + RECT_MODIFY1_INP1_TIT].xsize, rect[i + RECT_MODIFY1_INP1_TIT].ysize, rect[i + RECT_MODIFY1_INP1_TIT].colorB[0]);

                    DrawButtonEdit(keyd[i + KEY_MODIFY1_INP1],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].textColor[0] : keybmpind[i + KEY_MODIFY1_INP1].icon[0],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY1_INP1].icon[1] : keybmpind[i + KEY_MODIFY1_INP1].icon[0]);
                }
                Draw24x24Str(rect[RECT_MODIFY1_INP1_TIT].xpis + (rect[RECT_MODIFY1_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[2]) * TitleLengthMax))) / 2,
                             rect[RECT_MODIFY1_INP1_TIT].ypis + (rect[RECT_MODIFY1_INP1_TIT].ysize - 24) / 2,
                             (MEM_TITLE_ADD + (U32)(rectInTitList[2] * TitleLengthMax)),
                             SheetPage, rectInTitList[2], rect[RECT_MODIFY1_INP1_TIT].colorT[0]); //�����
            }
        }
        // /[��ʾedit para == 1|2|3|4*end]**********************************************************

        // /[����ģʽ������ʾ*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;

            BmpFillRect(rect[RECT_MODIFY1_METHOD].xpis, rect[RECT_MODIFY1_METHOD].ypis,
                        rect[RECT_MODIFY1_METHOD].xsize + chSelModeMax * keyd[KEY_MODIFY1_MET1].xsize,
                        rect[RECT_MODIFY1_METHOD].ysize);

            DrawRect(rect[RECT_MODIFY1_METHOD].xpis, rect[RECT_MODIFY1_METHOD].ypis,
                     rect[RECT_MODIFY1_METHOD].xsize + chSelModeMax * KEY_X6, rect[RECT_MODIFY1_METHOD].ysize, rect[RECT_MODIFY1_METHOD].colorB[0]);
            Draw16x16Str(rect[RECT_MODIFY1_METHOD].xpis + SAFE_DIST3, rect[RECT_MODIFY1_METHOD].ypis + SAFE_DIST3,
                         (MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY1_METHOD].index[0] * TitleLengthMax)),
                         SheetPage, rect[RECT_MODIFY1_METHOD].index[0], rect[RECT_MODIFY1_METHOD].colorT[0]); //�������������

            if (para == 0 || para == 1 || para == 2 || para == 3 || para == 4 || para == 6)
            {
                for (i = KEY_MODIFY1_MET1; i < KEY_MODIFY1_MET1 + chSelModeMax; i++)
                {
                    f = i - KEY_MODIFY1_MET1;
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (tmp_selectmode == f) ? keyBmpList[para][f * 3 + 2] : keyBmpList[para][f * 3 + 0]);
                }
            }
            else if (para == 5)
            {
                if (g_num_mov_flag == 2)
                {
                }
                else
                {
                    for (i = KEY_MODIFY1_MET1; i < KEY_MODIFY1_MET1 + chSelModeMax; i++)
                    {
                        f = i - KEY_MODIFY1_MET1;
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                                   (tmp_selectmode == f) ? keyBmpList[para][f * 3 + 2] : keyBmpList[para][f * 3 + 0]);
                    }
                }
            }
            if (para == 6)
            {
                dis_updata6 = 1;
            }
        }
        // /[����ģʽ������ʾ*end]**********************************************************

        // /[edit����ˢ�� para == 1|2|3|4*begin]**********************************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;

            DrawButtonEdit(keyd[KEY_MODIFY1_INP1],
                           keybmpind[KEY_MODIFY1_INP1].textColor[0], keybmpind[KEY_MODIFY1_INP1].icon[1]);
            if (para == 1) //��ɾ��
            {
                DrawMFS32P(keyd[KEY_MODIFY1_INP1].xpis + (keyd[KEY_MODIFY1_INP1].xsize - 12 * 5) / 2,
                           keyd[KEY_MODIFY1_INP1].ypis + SAFE_DIST4,
                           tmp_parameter, 5, 0, keybmpind[KEY_MODIFY1_INP1].textColor[0]);
            }
            else if (para == 4) //�����ƶ�-���
            {
                DrawMFS32P(keyd[KEY_MODIFY1_INP1].xpis + (keyd[KEY_MODIFY1_INP1].xsize - 12 * 4) / 2,
                           keyd[KEY_MODIFY1_INP1].ypis + SAFE_DIST4,
                           tmp_parameter, 4, 1, keybmpind[KEY_MODIFY1_INP1].textColor[0]);
            }
            else if (para == 5)
            {
                DrawMFS32P(keyd[KEY_MODIFY1_INP1].xpis + (keyd[KEY_MODIFY1_INP1].xsize - 13 * 4) / 2,
                           keyd[KEY_MODIFY1_INP1].ypis + SAFE_DIST4, g_point_num, 3, 0, keybmpind[KEY_MODIFY1_INP1].textColor[0]);
            }
        }
        // /[edit����ˢ�� para == 1|2|3|4*end]**********************************************************

        // /[��ʾ��Ŀ�����趨��ʾ��*begin]**********************************************************
        if (dis_updata6)
        {
            dis_updata6 = 0;
            if (chSelInputMax == 1)
            {
                i = KEY_MODIFY1_INP2;
                DrawButtonEdit(keyd[i],
                               (1 == editflag) ? keybmpind[i].textColor[0] : keybmpind[i].icon[0],
                               (1 == editflag) ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
            }
            else if (chSelInputMax == 2)
            {
                for (i = KEY_MODIFY1_INP1; i < KEY_MODIFY1_INP1 + chSelInputMax; i++)
                {
                    f = i - KEY_MODIFY1_INP1;
                    DrawButtonEdit(keyd[i],
                                   (f == editflag) ? keybmpind[i].textColor[0] : keybmpind[i].icon[0],
                                   (f == editflag) ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
                }
                for (i = RECT_MODIFY1_INP1_TIT; i < RECT_MODIFY1_INP1_TIT + chSelInputMax; i++)
                {
                    f = i - RECT_MODIFY1_INP1_TIT;
                    DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize,
                                 rect[i].colorB[0]);
                    Draw24x24Str(rect[i].xpis + (rect[i].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[f]) * TitleLengthMax))) / 2,
                                 rect[i].ypis + SAFE_DIST4,
                                 MEM_TITLE_ADD + (U32)(rectInTitList[f]) * TitleLengthMax,
                                 SheetPage, rectInTitList[f], rect[i].colorT[0]);
                }
                for (i = KEY_MODIFY1_INP1; i < KEY_MODIFY1_INP1 + chSelInputMax; i++)
                {
                    if (i == KEY_MODIFY1_INP1)
                    {
                        DrawF16Str(keyd[i].xpis, keyd[i].ypis + keyd[i].ysize + SAFE_DIST4,
                                   "(1-84)", keybmpind[i].textColor[0]);
                    }
                    else if (i == KEY_MODIFY1_INP2)
                    {
                        DrawF16Str(keyd[i].xpis, keyd[i].ypis + keyd[i].ysize + SAFE_DIST4,
                                   "(0.1-12.7)", keybmpind[i].textColor[0]);
                    }
                }
            }

            if (tmp_selectmode)
            {
                BmpFillRect(rect[RECT_MODIFY1_INP1_TIT].xpis, rect[RECT_MODIFY1_INP1_TIT].ypis,
                            rect[RECT_MODIFY1_INP1_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY1_INP1].xsize,
                            rect[RECT_MODIFY1_INP1_TIT].ysize + keyd[KEY_MODIFY1_INP1].ysize - SAFE_DIST6);
            }
            else
            {
                dis_updata10 = 1;
            }

            dis_updata11 = 1;
        }
        // /[��ʾ��Ŀ�����趨��ʾ��*end]**********************************************************

        // /[ˢ��edit1������*begin]**********************************************************
        if (dis_updata10)
        {
            dis_updata10 = 0;

            if (para == 6)
            {
                DrawButtonEdit(keyd[KEY_MODIFY1_INP1],
                               (editflag == 0) ? keybmpind[KEY_MODIFY1_INP1].textColor[0] : keybmpind[KEY_MODIFY1_INP1].icon[0],
                               (editflag == 0) ? keybmpind[KEY_MODIFY1_INP1].icon[1] : keybmpind[KEY_MODIFY1_INP1].icon[0]);

                DrawMFS32P(keyd[KEY_MODIFY1_INP1].xpis + (keyd[KEY_MODIFY1_INP1].xsize - 13 * 2) / 2,
                           keyd[KEY_MODIFY1_INP1].ypis + SAFE_DIST4,
                           temp_stitch_number, 2, 0,
                           keybmpind[KEY_MODIFY1_INP1].textColor[0]);
            }
        }
        // /[ˢ��edit1������*end]**********************************************************

        // /[ˢ��edit2������*begin]**********************************************************
        if (dis_updata11)
        {
            dis_updata11 = 0;

            if (para == 6)
            {
                DrawButtonEdit(keyd[KEY_MODIFY1_INP2],
                               (editflag == 1) ? keybmpind[KEY_MODIFY1_INP2].textColor[0] : keybmpind[KEY_MODIFY1_INP2].icon[0],
                               (editflag == 1) ? keybmpind[KEY_MODIFY1_INP2].icon[1] : keybmpind[KEY_MODIFY1_INP2].icon[0]);

                DrawMFS32P(keyd[KEY_MODIFY1_INP2].xpis + (keyd[KEY_MODIFY1_INP2].xsize - 13 * 4) / 2,
                           keyd[KEY_MODIFY1_INP2].ypis + SAFE_DIST4,
                           tmp_parameter, 4, 1,
                           keybmpind[KEY_MODIFY1_INP2].textColor[0]);
            }
        }
        // /[ˢ��edit2������*end]**********************************************************

        // /[ˢ��edit1������Z�ַ�ר��*begin]**********************************************************
        if (dis_updata12)
        {
            dis_updata12 = 0;
        }
        // /[ˢ��edit1������Z�ַ�ר��*end]**********************************************************

        // /[ˢ��edit2������Z�ַ�ר��*begin]**********************************************************
        if (dis_updata13)
        {
            dis_updata13 = 0;
        }
        // /[ˢ��edit2������Z�ַ�ר��*end]**********************************************************

        // /[ˢ�����������Ϣ*begin]**********************************************************
        if (dis_updata15)
        {
            dis_updata15 = 0;
            DrawFillRect(rect[RECT_MODIFY1_X_STR].xpis,
                         rect[RECT_MODIFY1_X_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_MODIFY1_X_STR].ysize,
                         rect[RECT_MODIFY1_X_STR].colorB[0]);
            DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                      g_dxpos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
            DrawFillRect(rect[RECT_MODIFY1_Y_STR].xpis,
                         rect[RECT_MODIFY1_Y_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_MODIFY1_Y_STR].ysize,
                         rect[RECT_MODIFY1_Y_STR].colorB[0]);
            DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                      g_dypos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
        }
        // /[ˢ�����������Ϣ*end]**********************************************************

        // /[��ʱ200ms��Ϣ����*begin]**********************************************************
        if (Timerms > 200 && dis_pos)
        {
            Timerms = 0;
            if (Read_xyposi())
            {

                if (absposx != g_xabspos)
                {
                    absposx = g_xabspos;
                    DrawFillRect(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_MODIFY1_X_STR].ypis,
                                 rect[RECT_MODIFY1_X_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_MODIFY1_X_STR].ysize,
                                 rect[RECT_MODIFY1_X_STR].colorB[0]);
                    DrawF16Str(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_MODIFY1_X_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos, 6, 1, Color_Black);
                    }
#else

                    DrawFS32P(rect[RECT_MODIFY1_X_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_MODIFY1_X_STR].ypis + SAFE_DIST4,
                              g_xabspos, 6, 1, rect[RECT_MODIFY1_X_STR].colorT[0]);
#endif
                }
                if (absposy != g_yabspos)
                {
                    absposy = g_yabspos;
                    DrawFillRect(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_MODIFY1_Y_STR].ypis,
                                 rect[RECT_MODIFY1_Y_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_MODIFY1_Y_STR].ysize,
                                 rect[RECT_MODIFY1_Y_STR].colorB[0]);
                    DrawF16Str(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_MODIFY1_Y_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos, 6, 1, Color_Black);
                    }
#else
                    DrawFS32P(rect[RECT_MODIFY1_Y_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_MODIFY1_Y_STR].ypis + SAFE_DIST4,
                              g_yabspos, 6, 1, rect[RECT_MODIFY1_Y_STR].colorT[0]);
#endif
                }
            }
        }
        // /[��ʱ200ms��Ϣ����*end]**********************************************************

        // /[��ѹ�Ű�ťˢ��*begin]**********************************************************
        if (pfootf != (U16)(S_PFOOT))
        {
            pfootf = (U16)(S_PFOOT);
            DrawButton(keyd[KEY_MODIFY1_PFOOT], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_MODIFY1_PFOOT].border[0],
                       (pfootf == 0) ? keybmpind[KEY_MODIFY1_PFOOT].icon[0] : keybmpind[KEY_MODIFY1_PFOOT].icon[1]);
        }
        // /[��ѹ�Ű�ťˢ��	*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MODIFY1)
        {
            plastno = MENU_MODIFY1;
            g_temp_pno = pno;
            break;
        }
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
#endif
}

//**************************************************************
#define KEY_MODIFY1_PCM_SCRNSVR 1 //1 ����
#define KEY_MODIFY1_PCM_BACK 2    //2 ����
#define KEY_MODIFY1_PCM_ENTER 3   //3 ȷ��
#define KEY_MODIFY1_PCM_PAT 4     //4 ����
#define KEY_MODIFY1_PCM_PFOOT 5   //5 ̧ѹ��
#define KEY_MODIFY1_PCM_CANCEL 6  //6 ȡ��

#define KEY_MODIFY1_PCM_DUP 7   //7 +1
#define KEY_MODIFY1_PCM_DDOWN 8 //8 -1
#define KEY_MODIFY1_PCM_0 9     //9 0
#define KEY_MODIFY1_PCM_1 10    //10 1
#define KEY_MODIFY1_PCM_2 11    //11 2
#define KEY_MODIFY1_PCM_3 12    //12 3
#define KEY_MODIFY1_PCM_4 13    //13 4
#define KEY_MODIFY1_PCM_5 14    //14 5
#define KEY_MODIFY1_PCM_6 15    //15 6
#define KEY_MODIFY1_PCM_7 16    //16 7
#define KEY_MODIFY1_PCM_8 17    //17 8
#define KEY_MODIFY1_PCM_9 18    //18 9

#define KEY_MODIFY1_PCM_MET1 19 //19 ����1
#define KEY_MODIFY1_PCM_MET2 20 //20 ����2
#define KEY_MODIFY1_PCM_MET3 21 //21 ����3
#define KEY_MODIFY1_PCM_MET4 22 //22 ����4
#define KEY_MODIFY1_PCM_MET5 23 //23 ����5
#define KEY_MODIFY1_PCM_MET6 24 //24 ����6

#define KEY_MODIFY1_PCM_DIRL 25     //25 �������
#define KEY_MODIFY1_PCM_DIRC 26     //26 ��������
#define KEY_MODIFY1_PCM_DIRR 27     //27 �Ҳ�����
#define KEY_MODIFY1_PCM_SELWIDTH 28 //28 ���
#define KEY_MODIFY1_PCM_SELPITCH 29 //29 ���

#define KEY_MODIFY1_PCM_MAX 30

#define RECT_MODIFY1_PCM_PIN_TIT 1   //1 ���-����
#define RECT_MODIFY1_PCM_PIN_STR 2   //2 ���-����
#define RECT_MODIFY1_PCM_TYPE 3      //3 ����
#define RECT_MODIFY1_PCM_DIR 4       //4 ���ַ���
#define RECT_MODIFY1_PCM_WIDTH_TIT 5 //5 ���־���-����
#define RECT_MODIFY1_PCM_PITCH_TIT 6 //6 ���ַ���-����
#define RECT_MODIFY1_PCM_WIDTH_STR 7 //7 ���־���-����
#define RECT_MODIFY1_PCM_PITCH_STR 8 //8 ���ַ���-����

#define RECT_MODIFY1_PCM_MAX 9

void Menu_ModifyPatChangeMethod(U32 para)
{

    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ

    /****������ز���end****/

    /****��־λ��ز���begin****/
    U32 i;
    U8 pfootf = 0xff;
    U8 dis_updata1 = 0; //��ʾ����(�����/���ּ�)

    U8 dis_updata3 = 0; //����ģʽ������ʾ
    U8 dis_updata4 = 0; //edit����ˢ�� para == 1|2|3|4
    U8 dis_updata5 = 0; //��ʾz��ѡ�񰴼�

    U8 dis_updata12 = 0; //Z�ַ�����Ϣˢ��
    U8 dis_updata13 = 0; //Z�ַ������Ϣˢ��

    U8 dis_updata18 = 0; //��ʾedit para == 1|2|3|4
    U8 dis_updata19 = 0; //��ʾz���ȣ�������ݿ�

    U8 dis_pos = 1; //��ʾʵ������
    U8 endir = 0;   //�����ʹ��
    U8 ennum = 0;   //���ּ�ʹ��
    U8 enmode = 1;  //ģʽѡ��ʹ��
    U8 enCanel = 1; //ʹ�������
    U8 enzig = 0;   //�����޸�Z��
    U8 f = 0;
    U16 title = 0;
    U32 tmp_feedc = 0 /*, tmp_feedsum = 0*/, tmp_pat_add = 0;
    U16 maxfeed = 0;
    U32 n, ii, dat;
    // U16 tmp_draw_add = 0, tmp_line_add = 0;
    U16 xx = 0, yy = 0;

    S16 x, y;

    U16 moddat = 0;
    U16 Borcmd;
    S8 Borpooft, Borpfdis;
    U8 speed;
    // U8 enZoom = 1;
    S16 dx = 0, dy = 0;

    U8 editflag = 0;      //�����ѡ�б�־λ
    U8 chSelInputMax = 0; //�����ѡ��������
    U8 chSelModeMax = 0;  //���������������
    /****��־λ��ز���end***/

    // const U8 keynum = {KEY_MODIADD_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                     //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},  //2 ����
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1}, //3 ȷ��
        {0, 0, 0, 0},                        //4 ����
        {TYPE_X4, TYPE_Y4, KEY_X1, KEY_Y1},  //5 ̧ѹ��
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1}, //6 ȡ��

        {TYPE_X15 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y13 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2}, //7 +1
        {TYPE_X15 + SAFE_DIST5 + KEY_X2, TYPE_Y13 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},       //8 -1
        {TYPE_X15, TYPE_Y13 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},                             //9 0
        {TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},                                                         //10 1
        {TYPE_X15 + SAFE_DIST5 + KEY_X2, TYPE_Y13, KEY_X2, KEY_Y2},                                   //11 2
        {TYPE_X15 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y13, KEY_X2, KEY_Y2},                             //12 3
        {TYPE_X15, TYPE_Y13 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},                                   //13 4
        {TYPE_X15 + SAFE_DIST5 + KEY_X2, TYPE_Y13 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},             //14 5
        {TYPE_X15 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y13 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},       //15 6
        {TYPE_X15, TYPE_Y13 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},                             //16 7
        {TYPE_X15 + SAFE_DIST5 + KEY_X2, TYPE_Y13 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},       //17 8
        {TYPE_X15 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y13 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2}, //18 9

        {TYPE_X14 + SAFE_DIST1, TYPE_Y31, KEY_X6, KEY_Y6},              //19 ����1
        {TYPE_X14 + SAFE_DIST1 + KEY_X6, TYPE_Y31, KEY_X6, KEY_Y6},     //20 ����2
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 2, TYPE_Y31, KEY_X6, KEY_Y6}, //21 ����3
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 3, TYPE_Y31, KEY_X6, KEY_Y6}, //22 ����4
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 4, TYPE_Y31, KEY_X6, KEY_Y6}, //23 ����5
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5, TYPE_Y31, KEY_X6, KEY_Y6}, //24 ����6

        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 1 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //25 �������
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 5 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //26 ��������
        {TYPE_X14 + SAFE_DIST1 + KEY_X6 * 9 / 2, TYPE_Y32, KEY_X6, KEY_Y6}, //27 �Ҳ�����

        {TYPE_X14 + TYPE_SIZE15 + SAFE_DIST5, 260, TYPE_SIZE14, TYPE_SIZE5},                           //28 ���
        {TYPE_X14 + TYPE_SIZE15 + SAFE_DIST5, 260 + TYPE_SIZE5 + SAFE_DIST6, TYPE_SIZE14, TYPE_SIZE5}, //29 ���

    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {1, 0, 0, 0}, {1}},               //1 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {2, 0, 0, 0}, {0}},          //2 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {3, 0, 0, 0}, {0}},          //3 ȷ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {37}, {4, 0, 0, 0}, {0}},              //4 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {17, 19, 18, 20}, {25, 5, 0, 0}, {0}}, //5 ̧ѹ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {361, 362}, {6, 0, 0, 0}, {0}},        //6 ȡ��

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200}, {16, 0, 0, 0}, {0}}, //7 +1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202}, {17, 0, 0, 0}, {0}}, //8 -1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {91, 101}, {0, 0, 0, 0}, {0}},   //9 0
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {92, 102}, {0, 0, 0, 0}, {0}},   //10 1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {93, 103}, {0, 0, 0, 0}, {0}},   //11 2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {94, 104}, {0, 0, 0, 0}, {0}},   //12 3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {95, 105}, {0, 0, 0, 0}, {0}},   //13 4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {96, 106}, {0, 0, 0, 0}, {0}},   //14 5
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {97, 107}, {0, 0, 0, 0}, {0}},   //15 6
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {98, 108}, {0, 0, 0, 0}, {0}},   //16 7
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {99, 109}, {0, 0, 0, 0}, {0}},   //17 8
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {100, 110}, {0, 0, 0, 0}, {0}},  //18 9

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {391, 393, 392}, {MOD1_PARA5_ADDR + 4, 0, 0, 0}, {0}}, //19 ����1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {394, 396, 395}, {MOD1_PARA5_ADDR + 5, 0, 0, 0}, {0}}, //20 ����2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {400, 402, 401}, {MOD1_PARA5_ADDR + 6, 0, 0, 0}, {0}}, //21 ����3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {397, 399, 398}, {MOD1_PARA5_ADDR + 7, 0, 0, 0}, {0}}, //22 ����4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {445, 447, 446}, {MOD1_PARA5_ADDR + 8, 0, 0, 0}, {0}}, //23 ����5
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {378, 380, 379}, {MOD1_PARA5_ADDR + 9, 0, 0, 0}, {0}}, //24 ����6

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {448, 0, 449}, {MOD1_PARA5_ADDR + 12, 0, 0, 0}, {0}}, //25 �������
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {450, 0, 451}, {MOD1_PARA5_ADDR + 13, 0, 0, 0}, {0}}, //26 ��������
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {452, 0, 453}, {MOD1_PARA5_ADDR + 14, 0, 0, 0}, {0}}, //27 �Ҳ�����

        {{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {MOD1_PARA5_ADDR + 10, 0, 0, 0}, {0}}, //28 ���
        {{KEY_Silver}, {Color_Black, Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {MOD1_PARA5_ADDR + 11, 0, 0, 0}, {0}}, //29 ���
    };

    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {MOD1_PARA5_ADDR + 3}},              //1 ���-����
        {TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}}, //2 ���-����

        {TYPE_X14, TYPE_Y3, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black, Color_White}, {MOD1_COM_ADDR + 17}},                              //3 ����
        {TYPE_X14, TYPE_Y3 + TYPE_SIZE28 + SAFE_DIST6, TYPE_SIZE29, TYPE_SIZE28, {Color_Black}, {Color_Black, Color_White}, {MOD1_PARA5_ADDR + 17}}, //4 ���ַ���

        {TYPE_X14, TYPE_Y3 + (TYPE_SIZE28 + SAFE_DIST6) * 2, TYPE_SIZE15, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {MOD1_PARA5_ADDR + 15}},                           //4 ���־���-����
        {TYPE_X14, TYPE_Y3 + (TYPE_SIZE28 + SAFE_DIST6) * 2 + TYPE_SIZE5 + SAFE_DIST6, TYPE_SIZE15, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {MOD1_PARA5_ADDR + 16}}, //5 �������-����
    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MODIFY1); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    Read_xyposi(); //��ȡһ�µ�ǰλ��
    if (F_FILETYPE == PAT_MIT)
    { //����

        // tmp_feedsum = g_patd.feed_sum;
        // tmp_draw_add = g_patd.draw_add;
        // tmp_line_add = g_patd.line_add;
        tmp_pat_add = g_patd.pat_add;
        tmp_feedc = g_jog_point;
    }
    else if (F_FILETYPE == PAT_SEW)
    { //�ֵ�

        // tmp_feedsum = g_patd.feed_sum;
        tmp_feedc = g_jog_point;
    }

    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************

    if (para == 5) //��������
    {

        if (g_selectmode > 5)
        {
            g_selectmode = 0;
        }
        tmp_selectmode = g_selectmode;

        if ((g_zig_width < 1) || (g_zig_width > 100))
            g_zig_width = 20;
        if ((g_zig_pitch < 5) || (g_zig_pitch > 100))
            g_zig_pitch = 30;
        if (g_zig_direction > 2)
            g_zig_direction = 0;

        if (F_FILETYPE == PAT_MIT)
        { //mit
            g_tmpoxpos = 0;
            g_tmpoypos = 0;
#if PATRULES_ORG_OFFSET
            if (SREG[3] == 7) //����-�޸�ԭ��λ��
            {
                g_tmpoxpos = (S16)(SREG[6]);
                g_tmpoypos = (S16)(SREG[7]);
            }
#endif
            n = g_jog_point;
            for (ii = 0; ii < n; ii++)
            {
                dat = SREG[tmp_pat_add + ii * 2] & 0x00ff;
                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                {
                    dat = SREG[tmp_pat_add + ii * 2 + 1];
                    xx = dat & 0x000000ff;
                    yy = ((U32)(dat >> 8)) & 0x000000ff;
                    if (xx >= 0x80)
                    {
                        xx -= 0x80;
                        g_tmpoxpos -= xx;
                    }
                    else
                    {
                        g_tmpoxpos += xx;
                    }
                    if (yy >= 0x80)
                    {
                        yy -= 0x80;
                        g_tmpoypos -= yy;
                    }
                    else
                    {
                        g_tmpoypos += yy;
                    }
                }
            }
            g_tmpox = g_tmpoxpos;
            g_tmpoy = g_tmpoypos;
            x = g_tmpoxpos + g_dxpos;
            y = g_tmpoypos + g_dypos;
            if (g_xabspos != x || g_yabspos != y)
            {
                Comm_Move(x, y, 50, MENU_MODIFY1, 0, 0);
            }
            g_modcordflag = 0;
        }
        else if (F_FILETYPE == PAT_SEW && plastno != MENU_MOIMAGE)
        {
            dx = 0;
            dy = 0;
            Bor_feedInfo(tmp_feedc, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, &dx, &dy, &speed, &Borpooft, &Borpfdis);
            g_tmpoxpos += dx;
            g_tmpoypos += dy;
        }
        dis_updata1 = 1;
        if (g_num_mov_flag == 2)
        { //���ּ�
            title = MOD1_PARA5_ADDR;
            editflag = 0;
            chSelModeMax = 6;

            dis_pos = 0;
            // enZoom = 0;
            ennum = 1;
            endir = 0;
            maxfeed = 127;
            moddat = 100;
            dis_updata3 = 1;

            if (tmp_selectmode < 4)
            {
                dis_updata4 = 1;
                chSelInputMax = 0;
            }
            else if (tmp_selectmode == 5)
            {
                dis_updata1 = 0;
                chSelInputMax = 0;
                enCanel = 0;
            }
            if (tmp_selectmode == 4)
            {
                tmp_zigwp = g_zig_width;
                chSelInputMax = 2;
            }
            if ((g_parameter == 0) || (g_parameter > 127))
                g_parameter = 30;
            tmp_parameter = g_parameter;
        }
    }

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                 SheetPage, title, rect[0].colorT[0]); //����

    if (enCanel)
    {
        DrawButton(keyd[KEY_MODIFY1_PCM_CANCEL], (U16 *)(ADD_ICONBMP),
                   keybmpind[KEY_MODIFY1_PCM_CANCEL].border[0], keybmpind[KEY_MODIFY1_PCM_CANCEL].icon[0]);
    }

    //����������ʾ
    for (i = 2; i <= KEY_MODIFY1_PCM_PFOOT; i++)
    {
        if (i == KEY_MODIFY1_PCM_PAT)
        {
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************

    ViewPage(usepageno); //������ʾ����
    if ((P_TYA == 0) || (P_TYA > 15))
        P_TYA = 1;

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_PCM_PFOOT + 1, 0);
        if (keydata == 0 && enCanel)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_PCM_CANCEL + 1, KEY_MODIFY1_PCM_CANCEL);
        }
        if (keydata == 0 && ennum)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_PCM_9 + 1, KEY_MODIFY1_PCM_DUP);
        }

        if (keydata == 0 && enmode)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_PCM_MET1 + chSelModeMax, KEY_MODIFY1_PCM_MET1);
        }

        if (keydata == 0 && enzig)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY1_PCM_SELWIDTH + chSelInputMax, KEY_MODIFY1_PCM_DIRL);
        }

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if (keyon >= KEY_MODIFY1_PCM_DUP && keyon <= KEY_MODIFY1_PCM_DDOWN)
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
            {
                keyon = keydata;
                if (keyon == KEY_MODIFY1_PCM_SCRNSVR) //�ް�����Ч
                {
                }
                //�ǳ��水��-//������ɫ���ְ���
                else if (keyon == KEY_MODIFY1_PCM_SELWIDTH || keyon == KEY_MODIFY1_PCM_SELPITCH)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //������������
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
                {
                    beeponf = 1;

                    if (g_keyBellT) //����ʱ����������ʱ��
                        Beep_On(g_keyBellT);

                    switch (keyon) //�������±�����ʾ����ʾ
                    {

                    case KEY_MODIFY1_PCM_PFOOT:
                        DrawFMenuStr((pfootf == 0) ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
                        break;
                    case KEY_MODIFY1_PCM_0:
                    case KEY_MODIFY1_PCM_1:
                    case KEY_MODIFY1_PCM_2:
                    case KEY_MODIFY1_PCM_3:
                    case KEY_MODIFY1_PCM_4:
                    case KEY_MODIFY1_PCM_5:
                    case KEY_MODIFY1_PCM_6:
                    case KEY_MODIFY1_PCM_7:
                    case KEY_MODIFY1_PCM_8:
                    case KEY_MODIFY1_PCM_9:
                        BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                        DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_MODIFY1_PCM_0, 1, rect[0].colorT[0]);
                        break;
                    default:
                        DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                        break;
                    }
                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                    delay = KEYNT;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }
                Times = 0; //��ǰ
                if (keyon >= KEY_MODIFY1_PCM_DUP && keyon <= KEY_MODIFY1_PCM_DDOWN)
                {
                    keyoff = keyon;
                }
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            if (keyoff >= KEY_MODIFY1_PCM_DUP && keyoff <= KEY_MODIFY1_PCM_9) //���ּ�
            {
                if ((para == 5) && (tmp_selectmode == 4 || (tmp_selectmode < 4 && g_modzig)))
                { //z�����
                    if (keyoff == KEY_MODIFY1_PCM_DUP)
                        tmp_zigwp++;
                    else if (keyoff == KEY_MODIFY1_PCM_DDOWN)
                    {
                        if (tmp_zigwp)
                            tmp_zigwp--;
                    }
                    else
                    {
                        tmp_zigwp = (tmp_zigwp % 100) * 10 + (keyoff - KEY_MODIFY1_PCM_0);
                        if (tmp_zigwp > 100)
                            tmp_zigwp = tmp_zigwp % 100;
                    }
                    if (tmp_zigwp > 100)
                        tmp_zigwp = 100;

                    if (editflag)
                    {
                        g_zig_pitch = (U8)(tmp_zigwp);
                        // if (tmp_selectmode == 4)
                        //     dis_updata11 = 1;
                        // else
                        dis_updata13 = 1;
                    }
                    else
                    {
                        g_zig_width = (U8)(tmp_zigwp);
                        // if (tmp_selectmode == 4)
                        //     dis_updata10 = 1;
                        // else
                        dis_updata12 = 1;
                    }
                }
                else
                {
                    if (keyoff == KEY_MODIFY1_PCM_DUP)
                    {
                        if (tmp_parameter < maxfeed)
                        {
                            tmp_parameter++;
                            dis_updata4 = 1;
                        }
                    }
                    else if (keyoff == KEY_MODIFY1_PCM_DDOWN)
                    {
                        if (tmp_parameter > 1)
                        {
                            tmp_parameter--;
                            dis_updata4 = 1;
                        }
                    }
                    else
                    {
                        tmp_parameter = (tmp_parameter % moddat) * 10 + keyoff - KEY_MODIFY1_PCM_0;
                        if (tmp_parameter > maxfeed)
                            tmp_parameter %= moddat;
                        if (tmp_parameter < 1)
                            tmp_parameter = 1;
                        dis_updata4 = 1;
                    }
                }
            }
            else if (keyoff >= KEY_MODIFY1_PCM_MET1 && keyoff <= KEY_MODIFY1_PCM_MET6)
            { //��������ģʽ
                // if (tmp_selectmode != keyoff - KEY_MODIFY1_PCM_MET1)
                // {
                //     if (tmp_selectmode == 4 || keyoff - KEY_MODIFY1_PCM_MET1 == 4 || keyoff - KEY_MODIFY1_PCM_MET1 == 5)
                //     {
                //         dis_updata17 = 1;

                //     }
                //     if (tmp_selectmode == 5)
                //     {
                //         dis_updata1 = 1;
                //     }
                // }
                if (tmp_selectmode != keyoff - KEY_MODIFY1_PCM_MET1)
                {
                    tmp_selectmode = keyoff - KEY_MODIFY1_PCM_MET1;
                    dis_updata3 = 1;
                }
            }

            else if (keyoff >= KEY_MODIFY1_PCM_SELWIDTH && keyoff <= KEY_MODIFY1_PCM_SELPITCH)
            { //z�����ѡ��
                editflag = keyoff - KEY_MODIFY1_PCM_SELWIDTH;
                if (editflag == 0)
                {
                    tmp_zigwp = g_zig_width;
                }
                else
                {
                    tmp_zigwp = g_zig_pitch;
                }
                dis_updata19 = 1;
            }
            else if (keyoff >= KEY_MODIFY1_PCM_DIRL && keyoff <= KEY_MODIFY1_PCM_DIRR)
            { //z�췽��
                g_zig_direction = keyon - KEY_MODIFY1_PCM_DIRL;
                dis_updata5 = 1;
            }

            else
            {
                switch (keyoff)
                {
                case KEY_MODIFY1_PCM_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    g_parameter = tmp_parameter;
                    break;

                case KEY_MODIFY1_PCM_BACK:
                    Send_Pfoot(1, MENU_MODIFY1, 1);

                    pno = poldno[--pnoindex];
                    Wait_Move_End();
                    break;

                case KEY_MODIFY1_PCM_ENTER:
                    Send_Pfoot(1, MENU_MODIFY1, 1);
                    if (F_FILETYPE == PAT_MIT)
                    { //mit
                        if (para == 5)
                        { //��������
                            if (g_num_mov_flag == 2)
                            {
                                g_num_mov_flag = 1;
                                g_parameter = tmp_parameter;
                                g_selectmode = tmp_selectmode;
                                para = g_modify_method;
                                g_sta_end_flag = 0;
                                if (g_zig_pitch < 5)
                                    g_zig_pitch = 5;
                                if (g_zig_width < 1)
                                    g_zig_width = 1;
                                pno = MENU_MODIFYADD;
                                poldno[pnoindex++] = MENU_MODIFY1;
                            }
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //sew
                        if (para == 5)
                        { //��������
                            if (g_num_mov_flag == 2)
                            {
                                g_num_mov_flag = 1;
                                g_parameter = tmp_parameter;
                                g_selectmode = tmp_selectmode;
                                para = g_modify_method;
                                g_sta_end_flag = 0;
                                if (g_zig_pitch < 5)
                                    g_zig_pitch = 5;
                                if (g_zig_width < 1)
                                    g_zig_width = 1;
                                pno = MENU_MODIFYADD;
                                poldno[pnoindex++] = MENU_MODIFY1;
                            }
                        }
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                    }
                    break;

                    // case KEY_MODIFY1_PCM_PAT:
                    //     pno = MENU_MOIMAGE;
                    //     poldno[pnoindex] = MENU_MODIFY1;
                    //     pnoindex++;
                    //     g_parameter = tmp_parameter;
                    //     g_selectmode = tmp_selectmode;
                    //     if ((para == 5 && g_num_mov_flag == 2))
                    //     { //���������,��׷��,������,��������
                    //         g_imgpara = 100;
                    //     }

                    //     break;

                case KEY_MODIFY1_PCM_CANCEL:

                    if (para == 5)
                    { //��������
                        if (g_num_mov_flag == 2)
                        {
                            if (tmp_selectmode == 4)
                            {
                                if (editflag)
                                {
                                    tmp_zigwp = 0;
                                    g_zig_pitch = 0;
                                    dis_updata13 = 1;
                                }
                                else
                                {
                                    tmp_zigwp = 0;
                                    g_zig_width = 0;
                                    dis_updata12 = 1;
                                }
                            }
                            else
                            {
                                if (tmp_parameter != 1)
                                {
                                    tmp_parameter = 1;
                                    dis_updata4 = 1;
                                }
                            }
                        }
                    }

                    break;

                case KEY_MODIFY1_PCM_PFOOT:
                    Send_Pfoot(0, MENU_MODIFY1, 0);
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    break;
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {
            if (keyon == KEY_MODIFY1_PCM_SCRNSVR) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else if (keyon == KEY_MODIFY1_PCM_SELWIDTH || keyon == KEY_MODIFY1_PCM_SELPITCH)
            {
                DrawButtonOnZoomInRes(keyd[keyon]);
            }
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                         SheetPage, title, rect[0].colorT[0]); //����
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_MODIFY1);
        }
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MODIFY1);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[���ְ�ť�뷽��ťˢ��*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
            if (para == 5) //�����ƶ�/��������
            {

                if (ennum == 1 && endir == 0)
                {
                    for (i = KEY_MODIFY1_PCM_DUP; i <= KEY_MODIFY1_PCM_9; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                }
                else
                {
                    BmpFillRect(keyd[KEY_MODIFY1_PCM_1].xpis, keyd[KEY_MODIFY1_PCM_1].ypis,
                                keyd[KEY_MODIFY1_PCM_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY1_PCM_1].ysize * 4 + SAFE_DIST5 * 3);
                }
            }
        }
        // /[���ְ�ť�뷽��ťˢ��*end]**********************************************************

        // /[��ʾedit para == 1|2|3|4*begin]**********************************************************
        if (dis_updata18)
        {
            dis_updata18 = 0;
            if (para == 5)
            {
                if (tmp_selectmode < 4)
                {
                    DrawFillRect(rect[RECT_MODIFY1_PCM_PIN_TIT].xpis, rect[RECT_MODIFY1_PCM_PIN_TIT].ypis,
                                 rect[RECT_MODIFY1_PCM_PIN_TIT].xsize, rect[RECT_MODIFY1_PCM_PIN_TIT].ysize, rect[RECT_MODIFY1_PCM_PIN_TIT].colorB[0]);

                    DrawEdit(rect[RECT_MODIFY1_PCM_PIN_STR], rect[RECT_MODIFY1_PCM_PIN_STR].colorT[0], rect[RECT_MODIFY1_PCM_PIN_STR].colorB[0]);

                    Draw24x24Str(rect[RECT_MODIFY1_PCM_PIN_TIT].xpis + (rect[RECT_MODIFY1_PCM_PIN_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY1_PCM_PIN_TIT].index[0]) * TitleLengthMax))) / 2,
                                 rect[RECT_MODIFY1_PCM_PIN_TIT].ypis + (rect[RECT_MODIFY1_PCM_PIN_TIT].ysize - 24) / 2,
                                 (MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY1_PCM_PIN_TIT].index[0] * TitleLengthMax)),
                                 SheetPage, rect[RECT_MODIFY1_PCM_PIN_TIT].index[0], rect[RECT_MODIFY1_PCM_PIN_TIT].colorT[0]); //���

                    DrawF16Str(rect[RECT_MODIFY1_PCM_PIN_STR].xpis, rect[RECT_MODIFY1_PCM_PIN_STR].ypis + rect[RECT_MODIFY1_PCM_PIN_STR].ysize + SAFE_DIST4,
                               "(1-12.7)", keybmpind[RECT_MODIFY1_PCM_PIN_STR].textColor[0]);
                    dis_updata4 = 1;
                }
                else
                {
                    BmpFillRect(rect[RECT_MODIFY1_PCM_PIN_TIT].xpis, rect[RECT_MODIFY1_PCM_PIN_TIT].ypis,
                                rect[RECT_MODIFY1_PCM_PIN_TIT].xsize + SAFE_DIST6 + rect[RECT_MODIFY1_PCM_PIN_STR].xsize,
                                rect[RECT_MODIFY1_PCM_PIN_TIT].ysize + rect[RECT_MODIFY1_PCM_PIN_STR].ysize);
                }
            }
            else
            {
                BmpFillRect(rect[RECT_MODIFY1_PCM_PIN_TIT].xpis, rect[RECT_MODIFY1_PCM_PIN_TIT].ypis,
                            rect[RECT_MODIFY1_PCM_PIN_TIT].xsize + SAFE_DIST6 + rect[RECT_MODIFY1_PCM_PIN_STR].xsize,
                            rect[RECT_MODIFY1_PCM_PIN_TIT].ysize + rect[RECT_MODIFY1_PCM_PIN_STR].ysize);
            }
        }
        // /[��ʾedit para == 1|2|3|4*end]**********************************************************

        // /[edit����ˢ�� para == 1|2|3|4*begin]**********************************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;

            DrawEdit(rect[RECT_MODIFY1_PCM_PIN_STR], rect[RECT_MODIFY1_PCM_PIN_STR].colorT[0], rect[RECT_MODIFY1_PCM_PIN_STR].colorB[0]);

            if (para == 5)
            { //��������-���

                DrawMFS32P(rect[RECT_MODIFY1_PCM_PIN_STR].xpis + (rect[RECT_MODIFY1_PCM_PIN_STR].xsize - 12 * 4) / 2,
                           rect[RECT_MODIFY1_PCM_PIN_STR].ypis + SAFE_DIST4,
                           tmp_parameter, 4, 1, rect[RECT_MODIFY1_PCM_PIN_STR].colorT[0]);
            }
        }
        // /[edit����ˢ�� para == 1|2|3|4*end]**********************************************************

        // /[����ģʽ������ʾ*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;

            BmpFillRect(rect[RECT_MODIFY1_PCM_TYPE].xpis, rect[RECT_MODIFY1_PCM_TYPE].ypis,
                        rect[RECT_MODIFY1_PCM_TYPE].xsize,
                        rect[RECT_MODIFY1_PCM_TYPE].ysize);

            DrawRect(rect[RECT_MODIFY1_PCM_TYPE].xpis, rect[RECT_MODIFY1_PCM_TYPE].ypis,
                     rect[RECT_MODIFY1_PCM_TYPE].xsize, rect[RECT_MODIFY1_PCM_TYPE].ysize, rect[RECT_MODIFY1_PCM_TYPE].colorB[0]);
            Draw24x24Str(rect[RECT_MODIFY1_PCM_TYPE].xpis + SAFE_DIST3, rect[RECT_MODIFY1_PCM_TYPE].ypis + SAFE_DIST3,
                         (MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY1_PCM_TYPE].index[0] * TitleLengthMax)),
                         SheetPage, rect[RECT_MODIFY1_PCM_TYPE].index[0], rect[RECT_MODIFY1_PCM_TYPE].colorT[0]); //�������������

            if (para == 5)
            {
                for (i = KEY_MODIFY1_PCM_MET1; i < KEY_MODIFY1_PCM_MET1 + chSelModeMax; i++)
                {
                    f = i - KEY_MODIFY1_PCM_MET1;
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (tmp_selectmode == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                }

                if (tmp_selectmode == 4)
                {
                    ennum = 1;
                    enCanel = 1;
                    dis_updata1 = 1;
                    dis_updata5 = 1;
                    dis_updata19 = 1;
                    dis_updata18 = 1;
                    DrawButton(keyd[KEY_MODIFY1_CANCEL], (U16 *)(ADD_ICONBMP), keybmpind[KEY_MODIFY1_CANCEL].border[0], keybmpind[KEY_MODIFY1_CANCEL].icon[0]);
                }
                else if (tmp_selectmode < 4)
                {

                    ennum = 1;
                    enCanel = 1;
                    dis_updata18 = 1;
                    dis_updata5 = 1;
                    dis_updata19 = 1;
                    DrawButton(keyd[KEY_MODIFY1_CANCEL], (U16 *)(ADD_ICONBMP), keybmpind[KEY_MODIFY1_CANCEL].border[0], keybmpind[KEY_MODIFY1_CANCEL].icon[0]);
                }
                else if (tmp_selectmode == 5)
                {
                    BmpFillRect(keyd[KEY_MODIFY1_CANCEL].xpis, keyd[KEY_MODIFY1_CANCEL].ypis, keyd[KEY_MODIFY1_CANCEL].xsize, keyd[KEY_MODIFY1_CANCEL].ysize);
                    enCanel = 0;
                    ennum = 0;
                    dis_updata1 = 1;
                    dis_updata5 = 1;
                    dis_updata19 = 1;
                    dis_updata18 = 1;
                }

                if (editflag > 1)
                    editflag = 0;

                if (tmp_selectmode == 4)
                {
                    enzig = 1;
                    chSelInputMax = 2;
                    if (g_modzig > 1)
                        g_modzig = 0;
                    // dis_updata14 = 1;
                }
                else
                {
                    enzig = 0;
                    chSelInputMax = 0;
                }
            }
        }
        // /[����ģʽ������ʾ*end]**********************************************************

        // /[Z�ַ췽����ࡢ������ð�ť����*begin]**********************************************************
        if (dis_updata5)
        {
            dis_updata5 = 0;
            if (tmp_selectmode == 4)
            {
                f = KEY_MODIFY1_PCM_DIRL + g_zig_direction;
                for (i = KEY_MODIFY1_PCM_DIRL; i <= KEY_MODIFY1_PCM_DIRR; i++)
                {
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (f == i) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (f == i) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                }

                DrawRect(rect[RECT_MODIFY1_PCM_DIR].xpis, rect[RECT_MODIFY1_PCM_DIR].ypis, rect[RECT_MODIFY1_PCM_DIR].xsize, rect[RECT_MODIFY1_PCM_DIR].ysize, rect[RECT_MODIFY1_PCM_DIR].colorB[0]);
                Draw24x24Str(rect[RECT_MODIFY1_PCM_DIR].xpis + SAFE_DIST4, rect[RECT_MODIFY1_PCM_DIR].ypis + SAFE_DIST3,
                             MEM_TITLE_ADD + rect[RECT_MODIFY1_PCM_DIR].index[0] * TitleLengthMax,
                             SheetPage, rect[RECT_MODIFY1_PCM_DIR].index[0], rect[RECT_MODIFY1_PCM_DIR].colorT[0]); //�������������
            }
            else
            {
                BmpFillRect(rect[RECT_MODIFY1_PCM_DIR].xpis, rect[RECT_MODIFY1_PCM_DIR].ypis, rect[RECT_MODIFY1_PCM_DIR].xsize, rect[RECT_MODIFY1_PCM_DIR].ysize);
            }
        }
        // /[Z�ַ췽����ࡢ������ð�ť����*end]**********************************************************

        // /[��ʾz���ȣ�������ݿ�*begin]**********************************************************
        if (dis_updata19)
        {
            dis_updata19 = 0;
            if (tmp_selectmode == 4)
            {
                DrawFillRect(rect[RECT_MODIFY1_PCM_WIDTH_TIT].xpis, rect[RECT_MODIFY1_PCM_WIDTH_TIT].ypis, rect[RECT_MODIFY1_PCM_WIDTH_TIT].xsize, rect[RECT_MODIFY1_PCM_WIDTH_TIT].ysize, rect[RECT_MODIFY1_PCM_WIDTH_TIT].colorB[0]);
                Draw24x24Str(rect[RECT_MODIFY1_PCM_WIDTH_TIT].xpis + (rect[RECT_MODIFY1_PCM_WIDTH_TIT].xsize - strlen((char *)(MEM_TITLE_ADD + rect[RECT_MODIFY1_PCM_WIDTH_TIT].index[0] * TitleLengthMax)) * 13) / 2,
                             rect[RECT_MODIFY1_PCM_WIDTH_TIT].ypis + (rect[RECT_MODIFY1_PCM_WIDTH_TIT].ysize - 24) / 2,
                             MEM_TITLE_ADD + rect[RECT_MODIFY1_PCM_WIDTH_TIT].index[0] * TitleLengthMax,
                             SheetPage, rect[RECT_MODIFY1_PCM_WIDTH_TIT].index[0], rect[RECT_MODIFY1_PCM_WIDTH_TIT].colorT[0]); //�������������

                DrawButtonEdit(keyd[KEY_MODIFY1_PCM_SELWIDTH],
                               (editflag == 0) ? keybmpind[KEY_MODIFY1_PCM_SELWIDTH].icon[1] : keybmpind[KEY_MODIFY1_PCM_SELWIDTH].icon[0],
                               (editflag == 0) ? keybmpind[KEY_MODIFY1_PCM_SELWIDTH].textColor[0] : keybmpind[KEY_MODIFY1_PCM_SELWIDTH].icon[0]);
                DrawF16Str(keyd[KEY_MODIFY1_PCM_SELWIDTH].xpis + keyd[KEY_MODIFY1_PCM_SELWIDTH].xsize + SAFE_DIST4,
                           keyd[KEY_MODIFY1_PCM_SELWIDTH].ypis + (keyd[KEY_MODIFY1_PCM_SELWIDTH].ysize - 16) / 2,
                           "(0.1-10.0)", keybmpind[KEY_MODIFY1_PCM_SELWIDTH].textColor[1]);

                dis_updata12 = 1;

                DrawFillRect(rect[RECT_MODIFY1_PCM_PITCH_TIT].xpis, rect[RECT_MODIFY1_PCM_PITCH_TIT].ypis, rect[RECT_MODIFY1_PCM_PITCH_TIT].xsize, rect[RECT_MODIFY1_PCM_PITCH_TIT].ysize, rect[RECT_MODIFY1_PCM_PITCH_TIT].colorB[0]);
                Draw24x24Str(rect[RECT_MODIFY1_PCM_PITCH_TIT].xpis + (rect[RECT_MODIFY1_PCM_PITCH_TIT].xsize - strlen((char *)(MEM_TITLE_ADD + rect[RECT_MODIFY1_PCM_PITCH_TIT].index[0] * TitleLengthMax)) * 13) / 2,
                             rect[RECT_MODIFY1_PCM_PITCH_TIT].ypis + (rect[RECT_MODIFY1_PCM_PITCH_TIT].ysize - 24) / 2,
                             MEM_TITLE_ADD + rect[RECT_MODIFY1_PCM_PITCH_TIT].index[0] * TitleLengthMax,
                             SheetPage, rect[RECT_MODIFY1_PCM_PITCH_TIT].index[0], rect[RECT_MODIFY1_PCM_PITCH_TIT].colorT[0]); //�������������

                DrawButtonEdit(keyd[KEY_MODIFY1_PCM_SELPITCH],
                               (editflag == 1) ? keybmpind[KEY_MODIFY1_PCM_SELPITCH].icon[1] : keybmpind[KEY_MODIFY1_PCM_SELPITCH].icon[0],
                               (editflag == 1) ? keybmpind[KEY_MODIFY1_PCM_SELPITCH].textColor[0] : keybmpind[KEY_MODIFY1_PCM_SELPITCH].icon[0]);
                DrawF16Str(keyd[KEY_MODIFY1_PCM_SELPITCH].xpis + keyd[KEY_MODIFY1_PCM_SELPITCH].xsize + SAFE_DIST4,
                           keyd[KEY_MODIFY1_PCM_SELPITCH].ypis + (keyd[KEY_MODIFY1_PCM_SELPITCH].ysize - 16) / 2,
                           "(0.5-10.0)", keybmpind[KEY_MODIFY1_PCM_SELPITCH].textColor[1]);

                dis_updata13 = 1;
            }
            else
            {
                BmpFillRect(rect[RECT_MODIFY1_PCM_WIDTH_TIT].xpis, rect[RECT_MODIFY1_PCM_WIDTH_TIT].ypis, TYPE_SIZE29, rect[RECT_MODIFY1_PCM_WIDTH_TIT].ysize);
                BmpFillRect(rect[RECT_MODIFY1_PCM_PITCH_TIT].xpis, rect[RECT_MODIFY1_PCM_PITCH_TIT].ypis, TYPE_SIZE29, rect[RECT_MODIFY1_PCM_PITCH_TIT].ysize);
            }
        }
        // /[��ʾz���ȣ�������ݿ�*end]**********************************************************

        // /[ˢ��edit1������*begin]**********************************************************
        if (dis_updata12)
        {
            dis_updata12 = 0;
            if (para == 5)
            {
                if (tmp_selectmode == 4)
                {
                    DrawButtonEdit(keyd[KEY_MODIFY1_PCM_SELWIDTH],
                                   (editflag == 0) ? keybmpind[KEY_MODIFY1_PCM_SELWIDTH].textColor[0] : keybmpind[KEY_MODIFY1_PCM_SELWIDTH].icon[0],
                                   (editflag == 0) ? keybmpind[KEY_MODIFY1_PCM_SELWIDTH].icon[1] : keybmpind[KEY_MODIFY1_PCM_SELWIDTH].icon[0]);

                    DrawMFS32P(keyd[KEY_MODIFY1_PCM_SELWIDTH].xpis + (keyd[KEY_MODIFY1_PCM_SELWIDTH].xsize - 13 * 4) / 2,
                               keyd[KEY_MODIFY1_PCM_SELWIDTH].ypis + SAFE_DIST4,
                               g_zig_width, 4, 1,
                               keybmpind[KEY_MODIFY1_PCM_SELWIDTH].textColor[0]);
                }
            }
        }
        // /[ˢ��edit1������*end]**********************************************************

        // /[ˢ��edit2������*begin]**********************************************************
        if (dis_updata13)
        {
            dis_updata13 = 0;
            if (para == 5)
            {
                if (tmp_selectmode == 4)
                {
                    DrawButtonEdit(keyd[KEY_MODIFY1_PCM_SELPITCH],
                                   (editflag == 1) ? keybmpind[KEY_MODIFY1_PCM_SELPITCH].textColor[0] : keybmpind[KEY_MODIFY1_PCM_SELPITCH].icon[0],
                                   (editflag == 1) ? keybmpind[KEY_MODIFY1_PCM_SELPITCH].icon[1] : keybmpind[KEY_MODIFY1_PCM_SELPITCH].icon[0]);

                    DrawMFS32P(keyd[KEY_MODIFY1_PCM_SELPITCH].xpis + (keyd[KEY_MODIFY1_PCM_SELPITCH].xsize - 13 * 4) / 2,
                               keyd[KEY_MODIFY1_PCM_SELPITCH].ypis + SAFE_DIST4,
                               g_zig_pitch, 4, 1,
                               keybmpind[KEY_MODIFY1_PCM_SELPITCH].textColor[0]);
                }
            }
        }
        // /[ˢ��edit2������*end]**********************************************************

        // /[��ʱ200ms��Ϣ����*begin]**********************************************************
        if (Timerms > 200 && dis_pos)
        {
            Timerms = 0;
        }
        // /[��ʱ200ms��Ϣ����*end]**********************************************************

        // /[��ѹ�Ű�ťˢ��*begin]**********************************************************
        if (pfootf != (U16)(S_PFOOT))
        {
            pfootf = (U16)(S_PFOOT);
            DrawButton(keyd[KEY_MODIFY1_PFOOT], (U16 *)(ADD_ICONBMP),
                       keybmpind[KEY_MODIFY1_PFOOT].border[0],
                       (pfootf == 0) ? keybmpind[KEY_MODIFY1_PFOOT].icon[0] : keybmpind[KEY_MODIFY1_PFOOT].icon[1]);
        }
        // /[��ѹ�Ű�ťˢ��	*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MODIFY1)
        {
            plastno = MENU_MODIFY1;
            g_temp_pno = pno;
            break;
        }
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

/***************************************************/
#define KEY_MODIFY2_SCRNSVR 1 //1 ����
#define KEY_MODIFY2_BACK 2    //2 ����
#define KEY_MODIFY2_ENTER 3   //3 ȷ��
#define KEY_MODIFY2_PAT 4     //4 ����
#define KEY_MODIFY2_CANCEL 5  //5 ȡ��
#define KEY_MODIFY2_ZOUT 6    //6 ��С
#define KEY_MODIFY2_ZIN 7     //7 �Ŵ�
#define KEY_MODIFY2_IMG 8     //8 �����ƶ�

#define KEY_MODIFY2_MUL 9   //9 ����
#define KEY_MODIFY2_MUM 10  //10 ��
#define KEY_MODIFY2_MUR 11  //11 ����
#define KEY_MODIFY2_MML 12  //12 ��
#define KEY_MODIFY2_MMSP 13 //13 ѹ������
#define KEY_MODIFY2_MMR 14  //14 ��
#define KEY_MODIFY2_MDL 15  //15 ����
#define KEY_MODIFY2_MDM 16  //16 ��
#define KEY_MODIFY2_MDR 17  //17 ����

#define KEY_MODIFY2_DUP 18   //18 +1
#define KEY_MODIFY2_DDOWN 19 //19 -1
#define KEY_MODIFY2_0 20     //20 0
#define KEY_MODIFY2_1 21     //21 1
#define KEY_MODIFY2_2 22     //22 2
#define KEY_MODIFY2_3 23     //23 3
#define KEY_MODIFY2_4 24     //24 4
#define KEY_MODIFY2_5 25     //25 5
#define KEY_MODIFY2_6 26     //26 6
#define KEY_MODIFY2_7 27     //27 7
#define KEY_MODIFY2_8 28     //28 8
#define KEY_MODIFY2_9 29     //29 9

#define KEY_MODIFY2_MET1 30 //30 ����1
#define KEY_MODIFY2_MET2 31 //31 ����2
#define KEY_MODIFY2_MET3 32 //32 ����3
#define KEY_MODIFY2_MET4 33 //33 ����4

#define KEY_MODIFY2_INP1 34 //34 ����1
#define KEY_MODIFY2_INP2 35 //35 ����2

#define KEY_MODIFY2_H 36   //36 �ٶ�-��
#define KEY_MODIFY2_MD1 37 //37 �ٶ�-�и�
#define KEY_MODIFY2_MD2 38 //38 �ٶ�-�е�
#define KEY_MODIFY2_L 39   //39 �ٶ�-��

#define RECT_MODIFY2_X_TIT 1
#define RECT_MODIFY2_X_STR 2
#define RECT_MODIFY2_Y_TIT 3
#define RECT_MODIFY2_Y_STR 4
#define RECT_MODIFY2_ST_TIT 5
#define RECT_MODIFY2_ST_STR 6
#define RECT_MODIFY2_EN_TIT 7
#define RECT_MODIFY2_EN_STR 8

#define RECT_MODIFY2_INP1_TIT 9
#define RECT_MODIFY2_INP2_TIT 10

#define RECT_MODIFY2_METHOD 11
#define RECT_MODIFY2_SPEED 12

void Menu_Modify2(U32 para)
{

#if 1
    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ

    /****������ز���end****/

    /****��־λ��ز���begin****/
    U32 i;
    U8 pfootf = 0xff;
    U8 dis_updata1 = 0; //��ʾ����(�����/���ּ�)
    U8 dis_updata2 = 0; //��ʾͼ��
    U8 dis_updata3 = 0; //����ģʽ������ʾ
    U8 dis_updata4 = 0; //edit����ˢ�� para == 1|2|3|4
    //U8 dis_updata5 = 0;  //��ʾz��ѡ�񰴼�
    U8 dis_updata6 = 0;  //��ʾ��Ŀ�����趨��ʾ��
    U8 dis_updata10 = 0; //edit1�������ˢ�� para == 5|6
    U8 dis_updata11 = 0; //deit2��������ˢ�� para == 5|6
    U8 dis_updata12 = 0; //Z�ַ��趨��Ϣˢ��
    U8 dis_updata13 = 0; //Z�ַ�����Ϣˢ��
    //U8 dis_updata14 = 0; //Z�ַ������Ϣˢ��

    U8 dis_updata15 = 0; //XY,����ˢ��
    U8 dis_updata16 = 0; //��������ģʽ����
    U8 dis_updata17 = 0; //����edit��ʾ para == 1|2|3|4
    U8 dis_updata18 = 0; //��ʾedit para == 1|2|3|4

    U8 dis_updata20 = 0; //ˢ���ٶȰ�ť

    U8 dis_pos = 1; //��ʾʵ������
    U8 endir = 0;   //�����ʹ��
    U8 ennum = 0;   //���ּ�ʹ��
    U8 enmode = 1;  //ģʽѡ��ʹ��
    U8 enCanel = 1; //ʹ�������
    //U8 enzig = 0;   //�����޸�Z��
    U8 ll = 0, f = 0;
    U16 title = 0;
    U32 tmp_feedc = 0, tmp_feedsum = 0;
    U16 maxfeed = 0;
    U32 dat;

    S16 xorg = 0, yorg = 0;
    S16 tmpxpos, tmpypos;
    S16 mvxx, mvyy;
    S32 scale = 0, scale1 = 0, vscale = 0;
    U8 bResetCaonvas = 0;
    //S16 dorgx=0,dorgy=0;
    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    S32 posx1, posy1;
    S16 absposx = 0xffff, absposy = 0xffff;
    S8 mult1 = 1, mult2 = 1, mult3 = 1, tmuly = 1;
    U16 moddat = 0;

    U8 enZoom = 1;

    const U16 ORG_X = CONVAS_ORG_PX9, ORG_Y = CONVAS_ORG_PY9;
    S16 xo = ORG_X;
    S16 yo = ORG_Y;
    U8 pressKey = 0;

    U8 getNearflag = 0 /*,vnear =0*/;
    U8 tonear = 0;

    U8 editflag = 0;      //�����ѡ�б�־λ
    U8 chSelInputMax = 0; //�����ѡ��������
    U8 chSelModeMax = 0;  //���������������
    /****��־λ��ز���end***/

    // const U8 keynum = {KEY_MODIADD_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},             //2 ����
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},            //3 ȷ��
        {0, 0, 0, 0},                                   //4 ����
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},            //5 ȡ��
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},             //6 ��С
        {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},             //7 �Ŵ�
        {CONVAS_PX9, CONVAS_PY9, CONVAS_X9, CONVAS_Y9}, //8 �����ƶ�

        {TYPE_X36, TYPE_Y36, KEY_X1, KEY_Y1},                           //9 ����
        {TYPE_X36 + KEY_X1, TYPE_Y36, KEY_X1, KEY_Y1},                  //10 ��
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36, KEY_X1, KEY_Y1},              //11 ����
        {TYPE_X36, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},                  //12 ��
        {TYPE_X36 + KEY_X1, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},         //13 ѹ���ٶ�
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36 + KEY_Y1, KEY_X1, KEY_Y1},     //14 ��
        {TYPE_X36, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1},              //15 ����
        {TYPE_X36 + KEY_X1, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1},     //16 ��
        {TYPE_X36 + KEY_X1 * 2, TYPE_Y36 + KEY_Y1 * 2, KEY_X1, KEY_Y1}, //17 ����

        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2}, //18 +1
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},       //19 -1
        {TYPE_X37, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 3, KEY_X2, KEY_Y2},                             //20 0
        {TYPE_X37, TYPE_Y36, KEY_X2, KEY_Y2},                                                         //21 1
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36, KEY_X2, KEY_Y2},                                   //22 2
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36, KEY_X2, KEY_Y2},                             //23 3
        {TYPE_X37, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},                                   //24 4
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},             //25 5
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + SAFE_DIST5 + KEY_Y2, KEY_X2, KEY_Y2},       //26 6
        {TYPE_X37, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},                             //27 7
        {TYPE_X37 + SAFE_DIST5 + KEY_X2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2},       //28 8
        {TYPE_X37 + (SAFE_DIST5 + KEY_X2) * 2, TYPE_Y36 + (SAFE_DIST5 + KEY_Y2) * 2, KEY_X2, KEY_Y2}, //29 9

        {TYPE_X4, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},              //30 ����1
        {TYPE_X4 + KEY_X6, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},     //31 ����2
        {TYPE_X4 + KEY_X6 * 2, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //32 ����3
        {TYPE_X4 + KEY_X6 * 3, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //33 ����4

        {TYPE_X38, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5},  //34 ����1
        {TYPE_X38, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5}, //35 ����2

        {TYPE_X4 + KEY_X6 * 2 + SAFE_DIST4 * 3, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},              //36 �ٶ�-��
        {TYPE_X4 + KEY_X6 * 2 + SAFE_DIST4 * 3 + KEY_X6, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},     //37 �ٶ�-�и�
        {TYPE_X4 + KEY_X6 * 2 + SAFE_DIST4 * 3 + KEY_X6 * 2, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //38 �ٶ�-�е�
        {TYPE_X4 + KEY_X6 * 2 + SAFE_DIST4 * 3 + KEY_X6 * 3, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //39 �ٶ�-��

    };

    const U16 keyBmpList[1][9] = {
        {583, 0, 584,
         581, 0, 582}, //�ٶ��޸ķ���
    };
    const U16 keyTitList[7][4] = {
        {6, 7}, //�ٶ��޸ķ���
    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {1, 0, 0, 0}, {1}},      //1 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {2, 0, 0, 0}, {0}}, //2 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {3, 0, 0, 0}, {0}}, //3 ȷ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {37}, {4, 0, 0, 0}, {0}},     //4 ����

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {361, 362}, {5, 0, 0, 0}, {0}},     //5 ȡ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {74}, {21, 0, 0, 0}, {0}},          //6 ��С
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {75}, {20, 0, 0, 0}, {0}},          //7 �Ŵ�
        {{CONVAS_COLOR_B}, {CONVAS_COLOR_T}, {CONVAS_COLOR_B}, {19, 0, 0, 0}, {0}}, //8 �����ƶ�

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {55}, {7, 0, 0, 0}, {0}},      //9 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {56}, {8, 0, 0, 0}, {0}},      //10 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {57}, {9, 0, 0, 0}, {0}},      //11 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {58}, {10, 0, 0, 0}, {0}},     //12 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {51, 52}, {11, 0, 0, 0}, {0}}, //13 ѹ���ٶ�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {59}, {12, 0, 0, 0}, {0}},     //14 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {60}, {13, 0, 0, 0}, {0}},     //15 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {61}, {14, 0, 0, 0}, {0}},     //16 ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {62}, {15, 0, 0, 0}, {0}},     //17 ����

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {199, 200}, {12, 0, 0, 0}, {0}}, //18 +1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {201, 202}, {13, 0, 0, 0}, {0}}, //19 -1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {91, 101}, {0, 0, 0, 0}, {0}},   //20 0
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {92, 102}, {0, 0, 0, 0}, {0}},   //21 1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {93, 103}, {0, 0, 0, 0}, {0}},   //22 2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {94, 104}, {0, 0, 0, 0}, {0}},   //23 3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {95, 105}, {0, 0, 0, 0}, {0}},   //24 4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {96, 106}, {0, 0, 0, 0}, {0}},   //25 5
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {97, 107}, {0, 0, 0, 0}, {0}},   //26 6
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {98, 108}, {0, 0, 0, 0}, {0}},   //27 7
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {99, 109}, {0, 0, 0, 0}, {0}},   //28 8
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {100, 110}, {0, 0, 0, 0}, {0}},  //29 9

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //30 ����1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //31 ����2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //32 ����3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //33 ����4

        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {Color_Black, 0, 0, 0}, {0}}, //34 ����1
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {Color_Black, 0, 0, 0}, {0}}, //35 ����2

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {893, 0, 894}, {8, 0, 0, 0}, {0}},  //36 �ٶ�-��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {895, 0, 896}, {9, 0, 0, 0}, {0}},  //37 �ٶ�-�и�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {897, 0, 898}, {10, 0, 0, 0}, {0}}, //38 �ٶ�-�е�
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {899, 0, 900}, {11, 0, 0, 0}, {0}}, //39 �ٶ�-��

    };

    const U16 rectTitList[1][2] = {
        //0:������⣬1:���������
        {17, 0}, //�ٶ��޸ķ���
    };
    const U16 rectInTitList[] = {
        15, //����
    };
    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X17, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {22}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {23}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {TYPE_X17, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {17}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {17}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {15}},
        {TYPE_X9, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {0}},

        {220, 396, 8, 80, {Color_Black}, {Color_Black}, {17}},
        {344, 396, 8, 80, {Color_Black}, {Color_Black}, {16}},

    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MODIFY2); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_MODIFY2_MUL_A_X);

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    Read_xyposi(); //��ȡһ�µ�ǰλ��
    if (F_FILETYPE == PAT_MIT)
    {              //����
        mult1 = 1; //g_dxpos,g_dypos
        mult2 = -1;
        tmuly = 1;
        tmp_feedc = 0;
        tmp_feedsum = g_patd.feed_sum;
        tmp_feedc = g_jog_point;
    }
    else if (F_FILETYPE == PAT_SEW)
    { //�ֵ�
#if MOVEUNIT == 1
        mult1 = 1;
        mult3 = 2;
#else
        mult1 = 2;
#endif
        mult2 = 1;
        tmuly = -1;
        tmp_feedc = 0;
        tmp_feedsum = g_patd.feed_sum;
        tmp_feedc = g_jog_point;
    }

    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************

    if (para == 0) //�ٶ��޸�
    {
        chSelModeMax = 2;
        chSelInputMax = 1;
        editflag = 0;
        title = 0;
        ennum = 1;
        g_num_mov_flag = 2;

        tmp_feedc = g_jog_point;
        tmp_feedsum = g_patd.feed_sum;

        if ((plastno != MENU_SCRNSVR) && (plastno != MENU_MOIMAGE))
        {
            if (g_selectmode > 1)
                g_selectmode = 0;
            tmp_selectmode = g_selectmode;

            if ((g_parameter == 0) || (g_parameter > 99))
                g_parameter = 3;
            tmp_parameter = g_parameter;

            if (g_speed == 0x60)
                tmp_speed = 0;
            else if (g_speed == 0x40)
                tmp_speed = 1;
            else if (g_speed == 0x20)
                tmp_speed = 2;
            else
                tmp_speed = 3;
            if (tmp_selectmode)
                g_num_mov_flag = 0;
            else
                g_num_mov_flag = 2;

            g_publicbuf[58] = plastno;
        }
        else
        {
            plastno = g_publicbuf[58];
        }

        for (i = 1; i <= RECT_MODIFY2_ST_STR; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis,
                         rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MODIFY2_X_TIT || i == RECT_MODIFY2_Y_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                             SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
            }
            else if (i == RECT_MODIFY2_ST_TIT)
            {
                Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                             (MEM_TITLE_ADD + (U32)((rectTitList[0][0]) * TitleLengthMax)),
                             SheetPage, (rectTitList[0][0]), rect[i].colorT[0]); //�������������
            }
        }

        DrawFU32(rect[RECT_MODIFY2_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY2_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedc, 5, rect[RECT_MODIFY2_ST_STR].colorT[0]);
        DrawF16Str(rect[RECT_MODIFY2_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY2_ST_STR].ypis + SAFE_DIST4,
                   "/", rect[RECT_MODIFY2_ST_STR].colorT[0]);
        DrawFU32(rect[RECT_MODIFY2_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY2_ST_STR].ypis + SAFE_DIST4,
                 tmp_feedsum, 5, rect[RECT_MODIFY2_ST_STR].colorT[0]);

#if MOVEUNIT == 1
        if (g_curPTtype == 2)
        {
            DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
            DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
        }
        else
        {
            DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
            DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
        }
#else
        DrawFS32P(rect[RECT_MODIFY2_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY2_X_STR].ypis + SAFE_DIST4,
                  g_dxpos, 6, 1, rect[RECT_MODIFY2_X_STR].colorT[0]);
        DrawFS32P(rect[RECT_MODIFY2_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY2_Y_STR].ypis + SAFE_DIST4,
                  g_dypos, 6, 1, rect[RECT_MODIFY2_Y_STR].colorT[0]);
#endif

        // Draw24x24Str(16, 538, MEM_TITLE_ADD + (MOD1_COM_ADDR + 17) * TitleLengthMax, SheetPage, MOD1_COM_ADDR + 17, Color_Black);    //����
        // Draw24x24Str(152, 742, MEM_TITLE_ADD + (MOD1_PARA1_ADDR + 4) * TitleLengthMax, SheetPage, MOD1_PARA1_ADDR + 4, Color_Black); //��λ��

        for (i = 0; i < chSelInputMax; i++)
        {
            DrawFillRect(rect[i + RECT_MODIFY2_INP1_TIT].xpis, rect[i + RECT_MODIFY2_INP1_TIT].ypis,
                         rect[i + RECT_MODIFY2_INP1_TIT].xsize, rect[i + RECT_MODIFY2_INP1_TIT].ysize, rect[i + RECT_MODIFY2_INP1_TIT].colorB[0]);

            DrawButtonEdit(keyd[i + KEY_MODIFY2_INP1],
                           (i == editflag) ? keybmpind[i + KEY_MODIFY2_INP1].textColor[0] : keybmpind[i + KEY_MODIFY2_INP1].icon[0],
                           (i == editflag) ? keybmpind[i + KEY_MODIFY2_INP1].icon[1] : keybmpind[i + KEY_MODIFY2_INP1].icon[0]);
        }
        Draw24x24Str(rect[RECT_MODIFY2_INP1_TIT].xpis + (rect[RECT_MODIFY2_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[0]) * TitleLengthMax))) / 2,
                     rect[RECT_MODIFY2_INP1_TIT].ypis + (rect[RECT_MODIFY2_INP1_TIT].ysize - 24) / 2,
                     (MEM_TITLE_ADD + (U32)(rectInTitList[0] * TitleLengthMax)),
                     SheetPage, rectInTitList[0], rect[i].colorT[0]); //����

        maxfeed = tmp_feedsum - tmp_feedc - 1;
        DrawF16Str(keyd[KEY_MODIFY2_INP1].xpis, keyd[KEY_MODIFY2_INP1].ypis + keyd[KEY_MODIFY2_INP1].ysize + SAFE_DIST4,
                   "(1-     )", keybmpind[KEY_MODIFY2_INP1].textColor[0]);

        DrawFU32(keyd[KEY_MODIFY2_INP1].xpis + 3 * 9, keyd[KEY_MODIFY2_INP1].ypis + keyd[KEY_MODIFY2_INP1].ysize + SAFE_DIST4,
                 maxfeed, 5, keybmpind[KEY_MODIFY2_INP1].textColor[0]);

        if (tmp_parameter > maxfeed || tmp_parameter < 1)
        {
            tmp_parameter = 1;
        }
        DrawMFS32P(keyd[KEY_MODIFY2_INP1].xpis + (keyd[KEY_MODIFY2_INP1].xsize - 13 * 5) / 2,
                   keyd[KEY_MODIFY2_INP1].ypis + SAFE_DIST4,
                   tmp_parameter, 5, 0,
                   keybmpind[KEY_MODIFY2_INP1].textColor[0]);

        if (maxfeed < 100)
        {
            moddat = 10;
        }
        else if (maxfeed < 1000)
        {
            moddat = 100;
        }
        else if (maxfeed < 10000)
        {
            moddat = 1000;
        }
        else
        {
            moddat = 10000;
        }

        AnaPattern(0, F_FILETYPE, (F_FILELEN));
        dis_updata1 = 1;
        dis_updata2 = 1;
        dis_updata3 = 1;
        dis_updata20 = 1;

        //		g_dispat2.IsDat = 0;
        //scale = getScale(0);
        //vscale = scale+scale1;
        //xorg = xo - (g_maxX + g_minX)/2*(vscale)/1000;
        //yorg = yo - tmuly*(g_maxY + g_minY)/2*(vscale)/1000;
        g_tmpoxpos = mult1 * g_xabspos;
        g_tmpoypos = mult1 * g_yabspos;
    }
    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                 SheetPage, title, rect[0].colorT[0]); //����

    if (enCanel)
    {
        DrawButton(keyd[KEY_MODIFY2_CANCEL], (U16 *)(ADD_ICONBMP),
                   keybmpind[KEY_MODIFY2_CANCEL].border[0], keybmpind[KEY_MODIFY2_CANCEL].icon[0]);
    }

    if (enZoom)
    {
        for (i = KEY_MODIFY2_ZOUT; i <= KEY_MODIFY2_ZIN; i++)
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                       keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
        DrawButtonDisPat(keyd[KEY_MODIFY2_IMG], keybmpind[KEY_MODIFY2_IMG].border[0], keybmpind[KEY_MODIFY2_IMG].icon[0]);
    }

    //����������ʾ
    for (i = 2; i <= KEY_MODIFY2_PAT; i++)
    {
        if (i == KEY_MODIFY2_PAT)
        {
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
    // getFeedPos(g_jog_point,F_FILETYPE,&fpx,&fpy);
    // scale = getScale(0);
    // vscale = scale + scale1;
    scale == pat_getScale(patRule.h_patConvasIndex, PATRULES_MODIFY2_MUL_A_X);
    vscale = pat_getFinalScale(scale, scale1);

    ViewPage(usepageno); //������ʾ����

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_PAT + 1, 0);
        if (keydata == 0 && enCanel)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_CANCEL + 1, KEY_MODIFY2_CANCEL);
        }
        if (keydata == 0 && enZoom)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_IMG + 1, KEY_MODIFY2_ZOUT);
        }
        if (keydata == 0 && tonear)
        {
            // keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_TOP + 1, KEY_MODIFY2_TOP);
        }
        if (keydata == 0 && endir)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_MDR + 1, KEY_MODIFY2_MUL);
        }
        if (keydata == 0 && ennum)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_9 + 1, KEY_MODIFY2_DUP);
        }

        if (keydata == 0 && enmode)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_MET1 + chSelModeMax, KEY_MODIFY2_MET1);
        }

        if (keydata == 0 && chSelInputMax > 1)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_INP1 + chSelInputMax, KEY_MODIFY2_INP1);
        }
        if (keydata == 0)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY2_L + 1, KEY_MODIFY2_H);
        }
        if (keydata == KEY_MODIFY2_MMSP)
        {
            keydata = 0;
        }

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon >= KEY_MODIFY2_MUL && keyon <= KEY_MODIFY2_MDR && keyon != KEY_MODIFY2_MMSP) ||
                (keyon >= KEY_MODIFY2_DUP && keyon <= KEY_MODIFY2_DDOWN) ||
                (keyon >= KEY_MODIFY2_ZOUT && keyon <= KEY_MODIFY2_IMG))
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
            {
                keyon = keydata;
                pressKey = 1;
                if (keyon == KEY_MODIFY2_SCRNSVR || keyon == KEY_MODIFY2_IMG) //�ް�����Ч
                {
                }
                //�ǳ��水��-//������ɫ���ְ���
                else if (keyon == KEY_MODIFY2_INP1 || keyon == KEY_MODIFY2_INP2)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //������������
                ll = 1;
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
                {
                    beeponf = 1;

                    if (g_keyBellT) //����ʱ����������ʱ��
                        Beep_On(g_keyBellT);

                    switch (keyon) //�������±�����ʾ����ʾ
                    {
                    case KEY_MODIFY2_MET1:
                    case KEY_MODIFY2_MET2:
                    case KEY_MODIFY2_MET3:
                    case KEY_MODIFY2_MET4:
                        DrawFMenuStr(keyTitList[para][keyon - KEY_MODIFY2_MET1]);
                        break;
                    case KEY_MODIFY2_INP1:
                    case KEY_MODIFY2_INP2:
                        DrawFMenuStr(rectTitList[para][keyon - KEY_MODIFY2_INP1 + 2]);
                        break;
                    //case KEY_MODIFY2_PFOOT:
                    //    DrawFMenuStr((pfootf == 0) ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
                    //   break;
                    case KEY_MODIFY2_0:
                    case KEY_MODIFY2_1:
                    case KEY_MODIFY2_2:
                    case KEY_MODIFY2_3:
                    case KEY_MODIFY2_4:
                    case KEY_MODIFY2_5:
                    case KEY_MODIFY2_6:
                    case KEY_MODIFY2_7:
                    case KEY_MODIFY2_8:
                    case KEY_MODIFY2_9:
                        BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                        DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_MODIFY2_0, 1, rect[0].colorT[0]);
                        break;
                    default:
                        DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                        break;
                    }
                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                    delay = KEYNT;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }
                Times = 0; //��ǰ
                if ((keyon >= KEY_MODIFY2_MUL && keyon <= KEY_MODIFY2_MDR && keyon != KEY_MODIFY2_MMSP) ||
                    (keyon >= KEY_MODIFY2_DUP && keyon <= KEY_MODIFY2_DDOWN) ||
                    (keyon >= KEY_MODIFY2_ZOUT && keyon <= KEY_MODIFY2_IMG))
                {
                    keyoff = keyon;
                }
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            pressKey = 1;
            if ((keyoff >= KEY_MODIFY2_MUL) && (keyoff <= KEY_MODIFY2_MDR) && (keyoff != KEY_MODIFY2_MMSP)) // �����
            {
                if ((!(S_CLAMP)) && (P_OST == 0))
                {
                    pno = MENU_RWHELP;
                    hleppara = 13;
                    poldno[pnoindex++] = MENU_MODIFY2;
                }
                else
                {
                    if ((g_dxpos == 0) && (g_dypos == 0))
                    {
                        if ((P_TYA == 0) || (P_TYA > 15))
                            P_TYA = 1;
                    }
                    getNearflag = 1;
                    tmpxpos = g_dxpos;
                    tmpypos = g_dypos;

                    if ((keyoff == KEY_MODIFY2_MUR) || (keyoff == KEY_MODIFY2_MMR) || (keyoff == KEY_MODIFY2_MDR))
                    {
                        g_dxpos += ll;
                    }
                    else if ((keyoff == KEY_MODIFY2_MUL) || (keyoff == KEY_MODIFY2_MML) || (keyoff == KEY_MODIFY2_MDL))
                    {
                        g_dxpos -= ll;
                    }
                    if ((keyoff == KEY_MODIFY2_MUL) || (keyoff == KEY_MODIFY2_MUM) || (keyoff == KEY_MODIFY2_MUR))
                    {
                        g_dypos += ll;
                    }
                    else if ((keyoff == KEY_MODIFY2_MDL) || (keyoff == KEY_MODIFY2_MDM) || (keyoff == KEY_MODIFY2_MDR))
                    {
                        g_dypos -= ll;
                    }

                    mvxx = g_tmpoxpos + mult1 * g_dxpos;
                    mvyy = g_tmpoypos + mult1 * g_dypos;
                    if (mvxx < mult1 * mult3 * g_Mac.max_xl)
                        g_dxpos = (mult1 * mult3 * g_Mac.max_xl - g_tmpoxpos) / mult1;
                    if (mvxx > mult1 * mult3 * g_Mac.max_xr)
                        g_dxpos = (mult1 * mult3 * g_Mac.max_xr - g_tmpoxpos) / mult1;
                    if (mvyy > mult1 * mult3 * g_Mac.max_yu)
                        g_dypos = (mult1 * mult3 * g_Mac.max_yu - g_tmpoypos) / mult1;
                    if (mvyy < mult1 * mult3 * g_Mac.max_yd)
                        g_dypos = (mult1 * mult3 * g_Mac.max_yd - g_tmpoypos) / mult1;

#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll / 2, MENU_MODIFY2, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                    }
                    else
                    {
                        f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll, MENU_MODIFY2, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
                    }
#else
                    f = Comm_Move(g_tmpoxpos + mult1 * g_dxpos, g_tmpoypos + mult1 * g_dypos, ll, MENU_MODIFY2, (F_FILETYPE == PAT_MIT) ? 0 : 1, 0);
#endif

                    if (f == 0)
                    {
                        g_dxpos = tmpxpos;
                        g_dypos = tmpypos;
                    }
                    if (ll < P_TYA * 8)
                        ll += P_TYA;
                    else
                        ll += P_TYA * 2;
                    dat = 20 + P_TYA * 5;
                    if (ll > dat)
                        ll = dat;

                    if (g_tmpoxpos + mult1 * g_dxpos > g_maxX || g_tmpoxpos + mult1 * g_dxpos < g_minX || mult2 * (g_tmpoypos + mult1 * g_dypos) > g_maxY || mult2 * (g_tmpoypos + mult1 * g_dypos) < g_minY)
                    {
                        if (g_tmpoxpos + mult1 * g_dxpos > g_maxX)
                        {
                            g_maxX = g_tmpoxpos + mult1 * g_dxpos;
                        }
                        if (g_tmpoxpos + mult1 * g_dxpos < g_minX)
                        {
                            g_minX = g_tmpoxpos + mult1 * g_dxpos;
                        }
                        if (mult2 * (g_tmpoypos + mult1 * g_dypos) > g_maxY)
                        {
                            g_maxY = mult2 * (g_tmpoypos + mult1 * g_dypos);
                        }
                        if (mult2 * (g_tmpoypos + mult1 * g_dypos) < g_minY)
                        {
                            g_minY = mult2 * (g_tmpoypos + mult1 * g_dypos);
                        }
                        dis_updata2 = 1;
                        dis_updata15 = 1;
                        // scale = getScale(0);
                        scale == pat_getScale(patRule.h_patConvasIndex, PATRULES_MODIFY2_MUL_A_X);
                        vscale = pat_getFinalScale(scale, scale1);
                    }
                    else
                    {
                        posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
                        posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
                        if (((posx1 < keyd[KEY_MODIFY2_IMG].xpis) ||
                             (posx1 > keyd[KEY_MODIFY2_IMG].xpis + keyd[KEY_MODIFY2_IMG].xsize) ||
                             (posy1 < keyd[KEY_MODIFY2_IMG].ypis) ||
                             (posy1 > keyd[KEY_MODIFY2_IMG].ypis + keyd[KEY_MODIFY2_IMG].ysize)))
                        { //������ʾ��Χ
                            dis_updata2 = 1;
                        }
                        else
                        {

                            RecoveData2();
                            ReadPointData2(posx1 - 4, posy1 - 4);
                            Draw_SmallCrossLimit(posx1, posy1, COLOR_CROSS, patRule.h_patConvasIndex);
                        }
                    }
                }
            }

            else if (keyoff >= KEY_MODIFY2_DUP && keyoff <= KEY_MODIFY2_9) //���ּ�
            {

                if (keyoff == KEY_MODIFY2_DUP)
                {
                    if (tmp_parameter < maxfeed)
                    {
                        tmp_parameter++;
                        dis_updata4 = 1;
                    }
                }
                else if (keyoff == KEY_MODIFY2_DDOWN)
                {
                    if (tmp_parameter > 1)
                    {
                        tmp_parameter--;
                        dis_updata4 = 1;
                    }
                }
                else
                {
                    tmp_parameter = (tmp_parameter % moddat) * 10 + keyoff - KEY_MODIFY2_0;
                    if (tmp_parameter > maxfeed)
                        tmp_parameter %= moddat;
                    if (tmp_parameter < 1)
                        tmp_parameter = 1;
                    dis_updata4 = 1;
                }

                if (para == 0) //�ٶ�����
                {
                    dis_updata2 = 1;
                }
            }
            else if (keyoff >= KEY_MODIFY2_MET1 && keyoff <= KEY_MODIFY2_MET4)
            {
                switch (para)
                {

                case 0: //�ٶ�����
                    tmp_selectmode = keyoff - KEY_MODIFY2_MET1;
                    dis_updata3 = 1;
                    dis_updata2 = 1;
                    if (tmp_selectmode == 1)
                    {
                        dis_updata17 = 1;
                        dis_updata1 = 1;
                        ennum = 0;
                        endir = 0;
                    }
                    else
                    {
                        dis_updata18 = 1;
                        dis_updata1 = 1;
                        g_num_mov_flag = 2;
                        ennum = 1;
                        endir = 0;
                    }
                    break;

                default:
                    break;
                }
            }
            else if (keyoff >= KEY_MODIFY2_INP1 && keyoff <= KEY_MODIFY2_INP2)
            {
                switch (para)
                {
                case 0: //�ٶ�����
                    break;

                default:
                    break;
                }
            }
            else if ((keyoff >= KEY_MODIFY2_H) && (keyoff <= KEY_MODIFY2_L))
            {
                tmp_speed = keyoff - KEY_MODIFY2_H;
                dis_updata20 = 1;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_MODIFY2_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    g_parameter = tmp_parameter;
                    break;

                case KEY_MODIFY2_BACK:
                    pno = poldno[--pnoindex];
                    Send_Pfoot(1, MENU_MODIFY2, 1);
                    break;

                case KEY_MODIFY2_ENTER:
                    Send_Pfoot(1, MENU_MODIFY2, 1);
                    g_parameter = tmp_parameter;
                    g_selectmode = tmp_selectmode;

                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        if (tmp_speed == 0)
                            g_speed = 0x60;
                        else if (tmp_speed == 1)
                            g_speed = 0x40;
                        else if (tmp_speed == 2)
                            g_speed = 0x20;
                        else
                            g_speed = 0;
                        pno = MENU_MODIFY;
                        pnoindex -= 2;
                        Mit_Modify_Data(7);
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        U16 Borcmd;
                        U8 speed;
                        S8 Borpooft, Borpfdis;

                        g_jog_point++;
                        Bor_ModSpeed(g_jog_point, g_parameter, tmp_speed, g_selectmode + 1, g_patd.pat_len);

                        g_modifyflag = 1;
                        g_Tsavetimes = g_savetimes;
                        g_Tsavetimes++;
                        g_savetimes = g_Tsavetimes;
                        BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                        pno = MENU_MODIFY;
                        pnoindex -= 2;

                        Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, (S16 *)&g_tmpoxpos, (S16 *)&g_tmpoypos, (S16 *)&g_dxpos, (S16 *)&g_dypos, &speed, &Borpooft, &Borpfdis);
                        g_tmpoxpos += g_dxpos;
                        g_tmpoypos += g_dypos;
                        Comm_Move(g_tmpoxpos, g_tmpoypos, 80, MENU_MODIFY, 1, 0);
                    }
                    g_ModifyMode = 0xff;
                    break;

                case KEY_MODIFY2_PAT:
                    pno = MENU_MOIMAGE;
                    poldno[pnoindex] = MENU_MODIFY2;
                    pnoindex++;
                    g_parameter = tmp_parameter;
                    g_jog_point = tmp_feedc;

                    break;

                case KEY_MODIFY2_CANCEL:
                    if (para == 0)
                    { //�ٶ�����
                        if (tmp_parameter != 1)
                        {
                            tmp_parameter = 1;
                            dis_updata4 = 1;
                            if (tmp_selectmode != 1)
                            {
                                dis_updata2 = 1;
                            }
                        }
                    }
                    break;

                case KEY_MODIFY2_ZOUT: //ͼ����С
                    if (scale1)
                    {
                        scale1 -= 50;
                        dis_updata2 = 2;
                    }
                    else
                    {
                        if (bResetCaonvas)
                        {
                            bResetCaonvas = 0;
                            xo = patConvas.conOrgX;
                            yo = patConvas.conOrgY;
                            scale1 = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            dis_updata2 = 2;
                        }
                    }
                    break;

                case KEY_MODIFY2_ZIN: //ͼ�ηŴ�
                    scale1 += 50;
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_MODIFY2_IMG: //ͼ���ƶ�
                    if (beeponf > 1)
                    {
                        if ((absInt(keyxpct - okeyxpct) <= 10) && (absInt(keyypct - okeyypct) <= 10) && (absInt(keyxpct - stkeyxpct) <= 10) && (absInt(keyypct - stkeyypct) <= 10) && (absInt(okeyxpct - stkeyxpct) <= 10) && (absInt(okeyypct - stkeyypct) <= 10) && (beeponf == 15))
                        {
                            xo = ORG_X;
                            yo = ORG_Y;
                            scale1 = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            Beep_On(BELLT);
                        }
                        else
                        {
                            xo += (keyxpct - okeyxpct);
                            yo += (keyypct - okeyypct);
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                        }
                    }
                    else
                    {
                        okeyxpct = keyxpct;
                        okeyypct = keyypct;
                        stkeyxpct = keyxpct;
                        stkeyypct = keyypct;
                    }
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {
            if (keyon == KEY_MODIFY2_SCRNSVR || keyon == KEY_MODIFY2_IMG) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else if (keyon == KEY_MODIFY2_INP1 || keyon == KEY_MODIFY2_INP2)
            {
                DrawButtonOnZoomInRes(keyd[keyon]);
            }
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            pressKey = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                         SheetPage, title, rect[0].colorT[0]); //����
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_MODIFY2);
        }
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MODIFY2);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[������ťˢ��*begin]**********************************************************
        if (getNearflag && pressKey == 0)
        {
            getNearflag = 0;
        }
        // /[������ťˢ��*end]**********************************************************

        // /[���ְ�ť�뷽��ťˢ��*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
            if (para == 0) //�ٶ�����
            {
                if (ennum == 1 && endir == 0)
                {
                    BmpFillRect(keyd[KEY_MODIFY2_MUL].xpis, keyd[KEY_MODIFY2_MUL].ypis,
                                keyd[KEY_MODIFY2_MUL].xsize * 3,
                                keyd[KEY_MODIFY2_MUL].ysize * 3);
                    for (i = KEY_MODIFY2_DUP; i <= KEY_MODIFY2_9; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);
                    }
                }
                else if (ennum == 0 && endir == 1)
                {
                    BmpFillRect(keyd[KEY_MODIFY2_1].xpis, keyd[KEY_MODIFY2_1].ypis,
                                keyd[KEY_MODIFY2_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY2_1].ysize * 4 + SAFE_DIST5 * 3);
                    for (i = KEY_MODIFY2_MUL; i <= KEY_MODIFY2_MDR; i++)
                    {
                        DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                                   keybmpind[i].border[0], keybmpind[i].icon[0]);

                        if (i == KEY_MODIFY2_MMSP)
                            DrawColor_Gray1(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize);
                    }
                }
                else
                {
                    BmpFillRect(keyd[KEY_MODIFY2_1].xpis, keyd[KEY_MODIFY2_1].ypis,
                                keyd[KEY_MODIFY2_1].xsize * 3 + SAFE_DIST5 * 2,
                                keyd[KEY_MODIFY2_1].ysize * 4 + SAFE_DIST5 * 3);
                    BmpFillRect(keyd[KEY_MODIFY2_MUL].xpis, keyd[KEY_MODIFY2_MUL].ypis,
                                keyd[KEY_MODIFY2_MUL].xsize * 3,
                                keyd[KEY_MODIFY2_MUL].ysize * 3);
                }
            }
        }
        // /[���ְ�ť�뷽��ťˢ��*end]**********************************************************

        // /[ˢ�»�����ʾ*begin]**********************************************************
        if (dis_updata2)
        {

            // vscale = scale + scale1;
            vscale = pat_getFinalScale(scale, scale1);
            xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
            yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
            posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
            posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
            if (((posx1 < keyd[KEY_MODIFY2_IMG].xpis) || (posx1 > keyd[KEY_MODIFY2_IMG].xpis + keyd[KEY_MODIFY2_IMG].xsize) || (posy1 < keyd[KEY_MODIFY2_IMG].ypis) || (posy1 > keyd[KEY_MODIFY2_IMG].ypis + keyd[KEY_MODIFY2_IMG].ysize)))
            { //������ʾ��Χ
                //printf("*--------------------------*\r\n");
                xorg = ORG_X;
                yorg = ORG_Y;
                xorg += (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg += tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;

                xo = xorg + (xorg - (S32)((g_tmpoxpos + mult1 * g_dxpos) * vscale / 1000 + xorg));
                yo = yorg + (yorg - (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * vscale / 1000 + yorg));

                xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
            }

            if (para == 0)
            {
                g_parameter = (tmp_selectmode == 1 ? g_patd.feed_sum : tmp_parameter);
            }
            // DrawFillRect(keyd[KEY_MODIFY2_IMG].xpis, keyd[KEY_MODIFY2_IMG].ypis,
            //              keyd[KEY_MODIFY2_IMG].xsize, keyd[KEY_MODIFY2_IMG].ysize, CONVAS_COLOR_B);
            // newDrawPat(20, scale1, xo, yo, 0, 0);
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(PATRULES_MODIFY2_MUL_A_X, scale1, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);

            // ReadPointData2(posx1 - 4, posy1 - 4);
            // RecoveData2();

            // ViewPoint(posx1, posy1, COLOR_CROSS, 1);
            dis_updata2 = 0;
        }
        // /[ˢ�»�����ʾ*end]**********************************************************

        // /[��������ģʽ����*begin]**********************************************************
        if (dis_updata16)
        {
            dis_updata16 = 0;
            BmpFillRect(rect[RECT_MODIFY2_METHOD].xpis, rect[RECT_MODIFY2_METHOD].ypis,
                        rect[RECT_MODIFY2_METHOD].xsize + keyd[KEY_MODIFY2_MET1].xsize * chSelModeMax, rect[RECT_MODIFY2_METHOD].ysize);
        }
        // /[��������ģʽ����*end]**********************************************************

        // /[����edit��ʾ para == 1|2|3|4*begin]**********************************************************
        if (dis_updata17)
        {
            dis_updata17 = 0;
            if (chSelInputMax == 1 || chSelInputMax == 0)
            {
                BmpFillRect(rect[RECT_MODIFY2_INP1_TIT].xpis, rect[RECT_MODIFY2_INP1_TIT].ypis,
                            rect[RECT_MODIFY2_INP1_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY2_INP1].xsize,
                            rect[RECT_MODIFY2_INP1_TIT].ysize + keyd[KEY_MODIFY2_INP1].ysize);
            }
            if (chSelInputMax == 2 || chSelInputMax == 0)
            {
                BmpFillRect(rect[RECT_MODIFY2_INP2_TIT].xpis, rect[RECT_MODIFY2_INP2_TIT].ypis,
                            rect[RECT_MODIFY2_INP2_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY2_INP2].xsize,
                            rect[RECT_MODIFY2_INP2_TIT].ysize + keyd[KEY_MODIFY2_INP2].ysize);
            }
        }
        // /[����edit��ʾ para == 1|2|3|4*end]**********************************************************

        // /[��ʾedit para == 1|2|3|4*begin]**********************************************************
        if (dis_updata18)
        {
            dis_updata18 = 0;
            if (para == 0)
            {
                dis_updata4 = 1;
                for (i = 0; i < chSelInputMax; i++)
                {
                    DrawFillRect(rect[i + RECT_MODIFY2_INP1_TIT].xpis, rect[i + RECT_MODIFY2_INP1_TIT].ypis,
                                 rect[i + RECT_MODIFY2_INP1_TIT].xsize, rect[i + RECT_MODIFY2_INP1_TIT].ysize, rect[i + RECT_MODIFY2_INP1_TIT].colorB[0]);

                    DrawButtonEdit(keyd[i + KEY_MODIFY2_INP1],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY2_INP1].textColor[0] : keybmpind[i + KEY_MODIFY2_INP1].icon[0],
                                   (i == editflag) ? keybmpind[i + KEY_MODIFY2_INP1].icon[1] : keybmpind[i + KEY_MODIFY2_INP1].icon[0]);
                }
                Draw24x24Str(rect[RECT_MODIFY2_INP1_TIT].xpis + (rect[RECT_MODIFY2_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[0]) * TitleLengthMax))) / 2,
                             rect[RECT_MODIFY2_INP1_TIT].ypis + (rect[RECT_MODIFY2_INP1_TIT].ysize - 24) / 2,
                             (MEM_TITLE_ADD + (U32)(rectInTitList[0] * TitleLengthMax)),
                             SheetPage, rectInTitList[0], rect[RECT_MODIFY2_INP1_TIT].colorT[0]); //����

                maxfeed = tmp_feedsum - tmp_feedc - 1;
                DrawF16Str(keyd[KEY_MODIFY2_INP1].xpis, keyd[KEY_MODIFY2_INP1].ypis + keyd[KEY_MODIFY2_INP1].ysize + SAFE_DIST4,
                           "(1-     )", keybmpind[KEY_MODIFY2_INP1].textColor[0]);

                DrawFU32(keyd[KEY_MODIFY2_INP1].xpis + 3 * 9, keyd[KEY_MODIFY2_INP1].ypis + keyd[KEY_MODIFY2_INP1].ysize + SAFE_DIST4,
                         maxfeed, 5, keybmpind[KEY_MODIFY2_INP1].textColor[0]);
            }
        }
        // /[��ʾedit para == 1|2|3|4*end]**********************************************************

        // /[����ģʽ������ʾ*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;

            BmpFillRect(rect[RECT_MODIFY2_METHOD].xpis, rect[RECT_MODIFY2_METHOD].ypis,
                        rect[RECT_MODIFY2_METHOD].xsize + chSelModeMax * keyd[KEY_MODIFY2_MET1].xsize,
                        rect[RECT_MODIFY2_METHOD].ysize);

            DrawRect(rect[RECT_MODIFY2_METHOD].xpis, rect[RECT_MODIFY2_METHOD].ypis,
                     rect[RECT_MODIFY2_METHOD].xsize + chSelModeMax * KEY_X6, rect[RECT_MODIFY2_METHOD].ysize, rect[RECT_MODIFY2_METHOD].colorB[0]);
            Draw16x16Str(rect[RECT_MODIFY2_METHOD].xpis + SAFE_DIST3, rect[RECT_MODIFY2_METHOD].ypis + SAFE_DIST3,
                         (MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY2_METHOD].index[0] * TitleLengthMax)),
                         SheetPage, rect[RECT_MODIFY2_METHOD].index[0], rect[RECT_MODIFY2_METHOD].colorT[0]); //�������������

            if (para == 0)
            {
                for (i = KEY_MODIFY2_MET1; i < KEY_MODIFY2_MET1 + chSelModeMax; i++)
                {
                    f = i - KEY_MODIFY2_MET1;
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (tmp_selectmode == f) ? keyBmpList[para][f * 3 + 2] : keyBmpList[para][f * 3 + 0]);
                }
            }
        }
        // /[����ģʽ������ʾ*end]**********************************************************

        // /[edit����ˢ�� para == 1|2|3|4*begin]**********************************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;

            DrawButtonEdit(keyd[KEY_MODIFY2_INP1],
                           keybmpind[KEY_MODIFY2_INP1].textColor[0], keybmpind[KEY_MODIFY2_INP1].icon[1]);
            if (para == 0) //�ٶ�����
            {
                DrawMFS32P(keyd[KEY_MODIFY2_INP1].xpis + (keyd[KEY_MODIFY2_INP1].xsize - 12 * 5) / 2,
                           keyd[KEY_MODIFY2_INP1].ypis + SAFE_DIST4,
                           tmp_parameter, 5, 0, keybmpind[KEY_MODIFY2_INP1].textColor[0]);
            }
        }
        // /[edit����ˢ�� para == 1|2|3|4*end]**********************************************************

        // /[��ʾ��Ŀ�����趨��ʾ��*begin]**********************************************************
        if (dis_updata6)
        {
            dis_updata6 = 0;
        }
        // /[��ʾ��Ŀ�����趨��ʾ��*end]**********************************************************

        // /[ˢ��edit1������*begin]**********************************************************
        if (dis_updata10)
        {
            dis_updata10 = 0;
        }
        // /[ˢ��edit1������*end]**********************************************************

        // /[ˢ��edit2������*begin]**********************************************************
        if (dis_updata11)
        {
            dis_updata11 = 0;
        }
        // /[ˢ��edit2������*end]**********************************************************

        // /[ˢ��edit1������Z�ַ�ר��*begin]**********************************************************
        if (dis_updata12)
        {
            dis_updata12 = 0;
        }
        // /[ˢ��edit1������Z�ַ�ר��*end]**********************************************************

        // /[ˢ��edit2������Z�ַ�ר��*begin]**********************************************************
        if (dis_updata13)
        {
            dis_updata13 = 0;
        }
        // /[ˢ��edit2������Z�ַ�ר��*end]**********************************************************

        // /[ˢ�����������Ϣ*begin]**********************************************************
        if (dis_updata15)
        {
            dis_updata15 = 0;
            DrawFillRect(rect[RECT_MODIFY2_X_STR].xpis,
                         rect[RECT_MODIFY2_X_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_MODIFY2_X_STR].ysize,
                         rect[RECT_MODIFY2_X_STR].colorB[0]);
            DrawFS32P(rect[RECT_MODIFY2_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY2_X_STR].ypis + SAFE_DIST4,
                      g_dxpos, 6, 1, rect[RECT_MODIFY2_X_STR].colorT[0]);
            DrawFillRect(rect[RECT_MODIFY2_Y_STR].xpis,
                         rect[RECT_MODIFY2_Y_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_MODIFY2_Y_STR].ysize,
                         rect[RECT_MODIFY2_Y_STR].colorB[0]);
            DrawFS32P(rect[RECT_MODIFY2_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY2_Y_STR].ypis + SAFE_DIST4,
                      g_dypos, 6, 1, rect[RECT_MODIFY2_Y_STR].colorT[0]);
        }
        // /[ˢ�����������Ϣ*end]**********************************************************

        // /[��ʱ200ms��Ϣ����*begin]**********************************************************
        if (Timerms > 200 && dis_pos)
        {
            Timerms = 0;
            if (Read_xyposi())
            {

                if (absposx != g_xabspos)
                {
                    absposx = g_xabspos;
                    DrawFillRect(rect[RECT_MODIFY2_X_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_MODIFY2_X_STR].ypis,
                                 rect[RECT_MODIFY2_X_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_MODIFY2_X_STR].ysize,
                                 rect[RECT_MODIFY2_X_STR].colorB[0]);
                    DrawF16Str(rect[RECT_MODIFY2_X_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_MODIFY2_X_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_MODIFY2_X_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos, 6, 1, Color_Black);
                    }
#else

                    DrawFS32P(rect[RECT_MODIFY2_X_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_MODIFY2_X_STR].ypis + SAFE_DIST4,
                              g_xabspos, 6, 1, rect[RECT_MODIFY2_X_STR].colorT[0]);
#endif
                }
                if (absposy != g_yabspos)
                {
                    absposy = g_yabspos;
                    DrawFillRect(rect[RECT_MODIFY2_Y_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_MODIFY2_Y_STR].ypis,
                                 rect[RECT_MODIFY2_Y_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_MODIFY2_Y_STR].ysize,
                                 rect[RECT_MODIFY2_Y_STR].colorB[0]);
                    DrawF16Str(rect[RECT_MODIFY2_Y_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_MODIFY2_Y_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_MODIFY2_Y_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos, 6, 1, Color_Black);
                    }
#else
                    DrawFS32P(rect[RECT_MODIFY2_Y_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_MODIFY2_Y_STR].ypis + SAFE_DIST4,
                              g_yabspos, 6, 1, rect[RECT_MODIFY2_Y_STR].colorT[0]);
#endif
                }
            }
        }
        // /[��ʱ200ms��Ϣ����*end]**********************************************************

        // /[��ѹ�Ű�ťˢ��*begin]**********************************************************
        if (pfootf != (U16)(S_PFOOT))
        {
            pfootf = (U16)(S_PFOOT);
        }
        // /[��ѹ�Ű�ťˢ��	*end]**********************************************************

        // /[�ٶȰ�����ʾ*begin]**********************************************************
        if (dis_updata20)
        {
            dis_updata20 = 0;

            BmpFillRect(rect[RECT_MODIFY2_SPEED].xpis, rect[RECT_MODIFY2_SPEED].ypis,
                        rect[RECT_MODIFY2_SPEED].xsize + 4 * keyd[RECT_MODIFY2_SPEED].xsize,
                        rect[RECT_MODIFY2_SPEED].ysize);

            DrawRect(rect[RECT_MODIFY2_SPEED].xpis, rect[RECT_MODIFY2_SPEED].ypis,
                     rect[RECT_MODIFY2_SPEED].xsize + 4 * KEY_X6, rect[RECT_MODIFY2_SPEED].ysize, rect[RECT_MODIFY2_SPEED].colorB[0]);
            Draw16x16Str(rect[RECT_MODIFY2_SPEED].xpis + SAFE_DIST3, rect[RECT_MODIFY2_SPEED].ypis + SAFE_DIST3,
                         (MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY2_SPEED].index[0] * TitleLengthMax)),
                         SheetPage, rect[RECT_MODIFY2_SPEED].index[0], rect[RECT_MODIFY2_SPEED].colorT[0]); //�������������

            if (para == 0)
            {
                for (i = KEY_MODIFY2_H; i < KEY_MODIFY2_H + 4; i++)
                {
                    f = i - KEY_MODIFY2_H;
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (tmp_speed == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (tmp_speed == f) ? keybmpind[i].icon[2] : keybmpind[i].icon[0]);
                }
            }
        }
        // /[�ٶȰ�����ʾ*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MODIFY2)
        {
            plastno = MENU_MODIFY2;
            break;
        }
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
#endif
}

/***************************************************/
#define KEY_MODIFY3_SCRNSVR 1 //1 ����
#define KEY_MODIFY3_BACK 2    //2 ����
#define KEY_MODIFY3_ENTER 3   //3 ȷ��
#define KEY_MODIFY3_PAT 4     //4 ����
#define KEY_MODIFY3_CANCEL 5  //5 ȡ��
#define KEY_MODIFY3_ZOUT 6    //6 ��С
#define KEY_MODIFY3_ZIN 7     //7 �Ŵ�
#define KEY_MODIFY3_IMG 8     //8 �����ƶ�

#define KEY_MODIFY3_MET1 9  //30 ����1
#define KEY_MODIFY3_MET2 10 //31 ����2
#define KEY_MODIFY3_MET3 11 //32 ����3
#define KEY_MODIFY3_MET4 12 //33 ����4
#define KEY_MODIFY3_MET5 13 //30 ����5
#define KEY_MODIFY3_MET6 14 //31 ����6
#define KEY_MODIFY3_MET7 15 //32 ����7
#define KEY_MODIFY3_MET8 16 //33 ����8

#define KEY_MODIFY3_INP1 17 //34 ����1
#define KEY_MODIFY3_INP2 18 //35 ����2

#define RECT_MODIFY3_X_TIT 1
#define RECT_MODIFY3_X_STR 2
#define RECT_MODIFY3_Y_TIT 3
#define RECT_MODIFY3_Y_STR 4
#define RECT_MODIFY3_ST_TIT 5
#define RECT_MODIFY3_ST_STR 6
#define RECT_MODIFY3_EN_TIT 7
#define RECT_MODIFY3_EN_STR 8

#define RECT_MODIFY3_INP1_TIT 9
#define RECT_MODIFY3_INP2_TIT 10

#define RECT_MODIFY3_METHOD 11

void Menu_Modify3(U32 para)
{

#if 1
    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ

    /****������ز���end****/

    /****��־λ��ز���begin****/
    U32 i;
    U8 pfootf = 0xff;
    U8 dis_updata1 = 0; //��ʾ����(�����/���ּ�)
    U8 dis_updata2 = 0; //��ʾͼ��
    U8 dis_updata3 = 0; //����ģʽ������ʾ
    U8 dis_updata4 = 0; //edit����ˢ�� para == 1|2|3|4

    U8 dis_updata15 = 0; //XY,����ˢ��
    U8 dis_updata16 = 0; //��������ģʽ����
    U8 dis_updata17 = 0; //����edit��ʾ para == 1|2|3|4
    U8 dis_updata18 = 0; //��ʾedit para == 1|2|3|4

    U8 dis_pos = 1; //��ʾʵ������
    U8 endir = 0;   //�����ʹ��
    U8 ennum = 0;   //���ּ�ʹ��
    U8 enmode = 1;  //ģʽѡ��ʹ��
    U8 enCanel = 1; //ʹ�������
    //U8 enzig = 0;   //�����޸�Z��
    U8 f = 0;
    U16 title = 0;
    U32 tmp_feedc = 0, tmp_feedsum = 0;

    U32 dat, dat1;

    S16 xorg = 0, yorg = 0;

    S32 scale = 0, scale1 = 0, vscale = 0;
    U8 bResetCaonvas = 0;
    //S16 dorgx=0,dorgy=0;
    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    S32 posx1, posy1;
    S16 absposx = 0xffff, absposy = 0xffff;
    S8 mult1 = 1, mult2 = 1, tmuly = 1;

    U8 enZoom = 1;

    const U16 ORG_X = CONVAS_ORG_PX9, ORG_Y = CONVAS_ORG_PY9;
    S16 xo = ORG_X;
    S16 yo = ORG_Y;

    U8 tonear = 0;

    U8 j, AddFunnum = 0; //��¼��ӹ��ܸ���
    U8 DelFunnum = 0;    //��¼��ǰָ���ɾ���ĸ��ӹ��ܸ���
    static U8 DelFun[8] = {0};
    U8 DelNO = 0;
    U8 delflag = 0;
    U8 tmpDelFuns = 0;
    U8 chpins = 0;
    U16 Borcmd = 0x2000;
    U8 speed;
    S8 Borpooft = 0, Borpfdis; //,dlforceval =0;
    S16 tmpxdat, tmpydat, dx, dy;

    U8 editflag = 0;      //�����ѡ�б�־λ
    U8 chSelInputMax = 0; //�����ѡ��������
    U8 chSelModeMax = 0;  //���������������
    /****��־λ��ز���end***/

    // const U8 keynum = {KEY_MODIADD_MAX};

    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},             //2 ����
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},            //3 ȷ��
        {0, 0, 0, 0},                                   //4 ����
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},            //5 ȡ��
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},             //6 ��С
        {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},             //7 �Ŵ�
        {CONVAS_PX9, CONVAS_PY9, CONVAS_X9, CONVAS_Y9}, //8 �����ƶ�

        {TYPE_X4, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},              //9 ����1
        {TYPE_X4 + KEY_X6, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6},     //10 ����2
        {TYPE_X4 + KEY_X6 * 2, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //11 ����3
        {TYPE_X4 + KEY_X6 * 3, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //12 ����4
        {TYPE_X4 + KEY_X6 * 4, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //13 ����5
        {TYPE_X4 + KEY_X6 * 5, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //14 ����6
        {TYPE_X4 + KEY_X6 * 6, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //15 ����7
        {TYPE_X4 + KEY_X6 * 7, TYPE_Y4 + KEY_Y1 - KEY_Y6, KEY_X6, KEY_Y6}, //16 ����8

        {TYPE_X38, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5},  //17 ����1
        {TYPE_X38, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5}, //18 ����2

    };

    const U16 keyBmpList[1][9] = {
        {583, 0, 584,
         581, 0, 582}, //����ɾ��
    };
    const U16 keyTitList[1][4] = {
        {6, 7}, //����ɾ��
    };

    const tydDisSrcIndex keybmpind[] = {
        //title_counter
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {1, 0, 0, 0}, {1}},            //1 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {72, 88}, {2, 0, 0, 0}, {0}},       //2 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {71, 87}, {3, 0, 0, 0}, {0}},       //3 ȷ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {37}, {4, 0, 0, 0}, {0}},           //4 ����
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {361, 362}, {5, 0, 0, 0}, {0}},     //5 ȡ��
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {74}, {21, 0, 0, 0}, {0}},          //6 ��С
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {75}, {20, 0, 0, 0}, {0}},          //7 �Ŵ�
        {{CONVAS_COLOR_B}, {CONVAS_COLOR_T}, {CONVAS_COLOR_B}, {19, 0, 0, 0}, {0}}, //8 �����ƶ�

        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //9 ����1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //10 ����2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //11 ����3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //12 ����4
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //13 ����1
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //14 ����2
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //15 ����3
        {{KEY_Silver, KEY_Gray, KEY_Blue}, {0}, {0}, {0, 0, 0, 0}, {0}}, //16 ����4

        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {Color_Black, 0, 0, 0}, {0}}, //34 ����1
        {{KEY_Silver}, {Color_Black}, {Color_White, Color_DeepSkyBlue, Color_Black, 0}, {Color_Black, 0, 0, 0}, {0}}, //35 ����2

    };

    const U16 rectTitList[2][2] = {
        //0:������⣬1:���������
        {6, 0}, //�������
        {6, 0}, //����ɾ��
    };
    const U16 rectInTitList[] = {
        5, //����
    };
    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X17, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {22}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {23}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {TYPE_X17, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {17}},
        {TYPE_X17 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},
        {TYPE_SIZE19, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE9, TYPE_SIZE4, {Color_KeyBlue}, {Color_White}, {17}},
        {TYPE_SIZE19 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y1 + TYPE_SIZE4 + SAFE_DIST3, TYPE_SIZE15, TYPE_SIZE4, {Color_White}, {Color_Black}, {0}},

        {TYPE_X9, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {15}},
        {TYPE_X9, TYPE_Y10, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {0}},

        {220, 396, 8, 80, {Color_Black}, {Color_Black}, {17}},

    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MODIFY3); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_setPatRule(PATRULES_MODIFY3_ENSURE);

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    tmp_feedc = g_jog_point;
    if (F_FILETYPE == PAT_SEW)
    {
        if (g_BorSTflag == 1)
        {
            tmp_feedc = g_jog_point;
        }
        else
        {
            tmp_feedc = g_jog_point + 1;
        }
    }

    tmp_feedsum = g_patd.feed_sum;

    if ((plastno != MENU_SCRNSVR) && (plastno != MENU_MOIMAGE))
    {
        g_publicbuf[59] = plastno;
    }
    else
    {
        plastno = g_publicbuf[59];
    }

    if (F_FILETYPE == PAT_MIT)
    { //����
        if (g_modify_code == 1)
        {
            dat1 = g_patd.pat_add + g_jog_point * 2;
            dat = SREG[dat1];
            for (dat1 = 1; dat1 < 19; dat1++)
            {
                if (dat == pat_code[dat1])
                {
                    if (dat1 != 14)
                    {
                        g_code_mode = dat1;
                        break;
                    }
                }
            }
            if ((dat & 0xff) == 0x3c)
            {
                g_code_mode = 20;
            } //��Ƕ�
            else if ((dat & 0xff) == 0x3d)
            {
                g_code_mode = 21;
            } //����һ
            else if ((dat & 0xff) == 0x3e)
            {
                g_code_mode = 22;
            } //���ö�
        }
        if (g_code_mode > 22)
            g_code_mode = 1;
    }
    else if (F_FILETYPE == PAT_SEW)
    { //�ֵ�
        if (g_modify_code == 1)
        { //ɾ������
            g_BorDelFuns = 0;
            Bor_feedInfo(tmp_feedc, g_patd.pat_len, &Borcmd, &tmpxdat, &tmpydat, &dx, &dy, &speed, &Borpooft, &Borpfdis);
            switch (Borcmd & 0xff00)
            {
            case 0xf000:
                g_code_mode = 30;
                break; //��ѹ��
            case 0xf100:
                g_code_mode = 31;
                break; //������
            case 0xf300:
            {
                if ((Borcmd & 0xfff0) == 0xf340)
                {
                    g_code_mode = 32;
                } //��ͣ��
                else if ((Borcmd & 0xfff0) == 0xf300)
                {
                    g_code_mode = 33;
                } //��ͣ��
            }
            break;
            case 0xf400:
                g_code_mode = 20;
                break; //����
            case 0xfb00:
            {
                if (Borcmd == 0xfb00)
                {
                    g_code_mode = 34;
                } //����
                else if (Borcmd == 0xfb01)
                {
                    g_code_mode = 35;
                } //�и���
                else if (Borcmd == 0xfb02)
                {
                    g_code_mode = 36;
                } //�е���
                else if (Borcmd == 0xfb03)
                {
                    g_code_mode = 37;
                } //����
            }
            break;
            case 0xfc00:
            {
                //#if Debug
                //printf("Borcmd = %04x \r\n",Borcmd);
                //#endif
                if (Borcmd == 0xfC01)
                {
                    g_code_mode = 9;
                } //����һ
                else if (Borcmd == 0xfC02)
                {
                    g_code_mode = 10;
                } //���ܶ�
                else if (Borcmd == 0xfC04)
                {
                    g_code_mode = 11;
                } //������
                else if (Borcmd == 0xfC08)
                {
                    g_code_mode = 12;
                } //������
                else if (Borcmd == 0xfC10)
                {
                    g_code_mode = 13;
                } //������
                else if (Borcmd == 0xfC20)
                {
                    g_code_mode = 14;
                } //������
                else if (Borcmd == 0xfC40)
                {
                    g_code_mode = 15;
                } //������
                else if (Borcmd == 0xfC80)
                {
                    g_code_mode = 16;
                } //���ܰ�
                else
                {
                    g_BorDelFuns = Borcmd & 0xff;
                }
            }
            break;
            case 0xfd00:
                g_code_mode = 19;
                break; //	����
            case 0xfe00:
            {
                if (Borcmd == 0xfe00)
                {
                    g_code_mode = 17;
                } //����ͣ
                else if (Borcmd == 0xfe01)
                {
                    g_code_mode = 18;
                } //����ͣ
            }
            break;
            case 0xec00:
                g_code_mode = 33;
                break; //	��Ƕ�
            case 0xea00:
                g_code_mode = 40;
                break; //	����һ
            case 0xeb00:
                g_code_mode = 41;
                break; //	���ö�
            default:
                if (g_jog_point == 0)
                {
                    g_code_mode = 21;
                }
                break; //��ԭ��
            }
        }
        else
        {
            if (g_BorAddFuns != 0)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((g_BorAddFuns >> i) & 0x01)
                        AddFunnum++;
                }
                // if (AddFunnum > 3)
                //     y = 471;
                // else
                //     y = 503;
                // x = 256;

                for (j = 0, i = 0; (j < AddFunnum) && (i < 8); i++)
                {
                    if ((g_BorAddFuns >> i) & 0x01)
                    {
                        g_code_mode = 9 + i;
                        if ((g_code_mode == 16) && (g_UdZhouFlag == 3))
                            chpins = 22;
                        else
                            chpins = 0;
                        // Draw24x24Str(x + 108 * (j % 3), y + 32 * (j / 3), (MEM_TITLE_ADD + (g_code_mode + chpins) * TitleLengthMax), SheetPage, g_code_mode + chpins, Color_Black);
                        j++;
                    }
                }
            }
        }
    }
    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************
    if (F_FILETYPE == PAT_MIT)
    {
        if ((g_code_mode == 8) && (g_UdZhouFlag == 3))
            chpins = 22;
        else if (g_code_mode == 20)
            chpins = 11;
        else if (g_code_mode == 21 || g_code_mode == 22)
            chpins = 11;
        else
            chpins = 0;
        // Draw24x24Str(256, 503, (MEM_TITLE_ADD + (g_code_mode + 8 + chpins) * TitleLengthMax), SheetPage, g_code_mode + 8 + chpins, Color_Black);
    }
    else if (F_FILETYPE == PAT_SEW)
    {
        if ((g_BorAddFuns == 0) || (g_modify_code == 1))
        {
            if (g_BorDelFuns != 0)
            {
                DelFunnum = 0;
                for (i = 0; i < 8; i++)
                {
                    if ((g_BorDelFuns >> i) & 0x01)
                        DelFunnum++;
                }

                for (j = 0, i = 0; (j < DelFunnum) && (i < 8); i++)
                {
                    if ((g_BorDelFuns >> i) & 0x01)
                    {
                        g_code_mode = 9 + i;
                        // Draw24x24Str(x + 120 * (j % 3), y + 32 * (j / 3), (MEM_TITLE_ADD + (g_code_mode)*TitleLengthMax), SheetPage, g_code_mode, Color_Black);

                        if (g_UpdataBorDelFuns == 1)
                            DelFun[chSelModeMax] = 1; //����Ĭ��ָ��ȫ��ɾ��

                        //DrawTextbox(keyd2[0], KEY_White);
                        if (DelFun[chSelModeMax])
                        {
                            // DrawFPBMPBTN(keyd2[0].xpis + 2, keyd2[0].ypis + 2, keyd2[0].xsize - 4, keyd2[0].ysize - 4, 20, 0xffff);
                        }
                        else
                        {
                            // DrawFillRect(keyd2[0].xpis + 2, keyd2[0].ypis + 2, keyd2[0].xsize - 4, keyd2[0].ysize - 4, RGB(250, 250, 250));
                        }
                        j++;
                        chSelModeMax++;
                    }
                }
                g_UpdataBorDelFuns = 0;
            }
            else
            {
                if ((g_code_mode == 16) && (g_UdZhouFlag == 3))
                    chpins = 22;
                else if (g_code_mode == 33)
                    chpins = 6;
                else if (g_code_mode == 40 || g_code_mode == 41)
                    chpins = 0;
                else
                    chpins = 0;
                // Draw24x24Str(256, 503, (MEM_TITLE_ADD + (g_code_mode + chpins) * TitleLengthMax), SheetPage, g_code_mode + chpins, Color_Black);
            }
        }
    }

    if (para == 0) //��Ӵ���
    {
        chSelModeMax = 0;
        chSelInputMax = 1;
        editflag = 0;
        title = 27;
    }
    else if (para == 1) //ɾ������
    {

        chSelModeMax = 0;
        chSelInputMax = 1;
        editflag = 0;
        title = 28;
    }
    tmp_feedc = g_jog_point;
    tmp_feedsum = g_patd.feed_sum;

    for (i = 1; i <= RECT_MODIFY3_ST_STR; i++)
    {
        DrawFillRect(rect[i].xpis, rect[i].ypis,
                     rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
        if (i == RECT_MODIFY3_X_TIT || i == RECT_MODIFY3_Y_TIT)
        {
            Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                         (MEM_TITLE_ADD + (U32)(rect[i].index[0] * TitleLengthMax)),
                         SheetPage, rect[i].index[0], rect[i].colorT[0]); //�������������
        }
        else if (i == RECT_MODIFY3_ST_TIT)
        {
            Draw16x16Str(rect[i].xpis + SAFE_DIST4, rect[i].ypis + (rect[i].ysize - 16) / 2,
                         (MEM_TITLE_ADD + (U32)((rectTitList[para][0]) * TitleLengthMax)),
                         SheetPage, (rectTitList[para][0]), rect[i].colorT[0]); //�������������
        }
    }

    DrawFU32(rect[RECT_MODIFY3_ST_STR].xpis + SAFE_DIST2, rect[RECT_MODIFY3_ST_STR].ypis + SAFE_DIST4,
             tmp_feedc, 5, rect[RECT_MODIFY3_ST_STR].colorT[0]);
    DrawF16Str(rect[RECT_MODIFY3_ST_STR].xpis + SAFE_DIST2 + 9 * 5, rect[RECT_MODIFY3_ST_STR].ypis + SAFE_DIST4,
               "/", rect[RECT_MODIFY3_ST_STR].colorT[0]);
    DrawFU32(rect[RECT_MODIFY3_ST_STR].xpis + SAFE_DIST2 + 9 * 6, rect[RECT_MODIFY3_ST_STR].ypis + SAFE_DIST4,
             tmp_feedsum, 5, rect[RECT_MODIFY3_ST_STR].colorT[0]);

#if MOVEUNIT == 1
    if (g_curPTtype == 2)
    {
        DrawMFS32P(112, 40, g_dxpos * 5, 7, 2, Color_Black);
        DrawMFS32P(112, 72, g_dypos * 5, 7, 2, Color_Black);
    }
    else
    {
        DrawMFS32P(112, 40, g_dxpos, 6, 1, Color_Black);
        DrawMFS32P(112, 72, g_dypos, 6, 1, Color_Black);
    }
#else
    DrawFS32P(rect[RECT_MODIFY3_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY3_X_STR].ypis + SAFE_DIST4,
              g_dxpos, 6, 1, rect[RECT_MODIFY3_X_STR].colorT[0]);
    DrawFS32P(rect[RECT_MODIFY3_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY3_Y_STR].ypis + SAFE_DIST4,
              g_dypos, 6, 1, rect[RECT_MODIFY3_Y_STR].colorT[0]);
#endif

    // Draw24x24Str(16, 538, MEM_TITLE_ADD + (MOD1_COM_ADDR + 17) * TitleLengthMax, SheetPage, MOD1_COM_ADDR + 17, Color_Black);    //����
    // Draw24x24Str(152, 742, MEM_TITLE_ADD + (MOD1_PARA1_ADDR + 4) * TitleLengthMax, SheetPage, MOD1_PARA1_ADDR + 4, Color_Black); //��λ��

    for (i = 0; i < chSelInputMax; i++)
    {
        DrawFillRect(rect[i + RECT_MODIFY3_INP1_TIT].xpis, rect[i + RECT_MODIFY3_INP1_TIT].ypis,
                     rect[i + RECT_MODIFY3_INP1_TIT].xsize, rect[i + RECT_MODIFY3_INP1_TIT].ysize, rect[i + RECT_MODIFY3_INP1_TIT].colorB[0]);

        DrawButtonEdit(keyd[i + KEY_MODIFY3_INP1],
                       (i == editflag) ? keybmpind[i + KEY_MODIFY3_INP1].textColor[0] : keybmpind[i + KEY_MODIFY3_INP1].icon[0],
                       (i == editflag) ? keybmpind[i + KEY_MODIFY3_INP1].icon[1] : keybmpind[i + KEY_MODIFY3_INP1].icon[0]);
    }
    Draw24x24Str(rect[RECT_MODIFY3_INP1_TIT].xpis + (rect[RECT_MODIFY3_INP1_TIT].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (U32)(rectInTitList[0]) * TitleLengthMax))) / 2,
                 rect[RECT_MODIFY3_INP1_TIT].ypis + (rect[RECT_MODIFY3_INP1_TIT].ysize - 24) / 2,
                 (MEM_TITLE_ADD + (U32)(rectInTitList[0] * TitleLengthMax)),
                 SheetPage, rectInTitList[0], rect[i].colorT[0]); //����

    if (F_FILETYPE == PAT_MIT)
    {
        Draw24x24Str(keyd[KEY_MODIFY3_INP1].xpis + (keyd[KEY_MODIFY3_INP1].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (g_code_mode + 8 + chpins) * TitleLengthMax))) / 2,
                     keyd[KEY_MODIFY3_INP1].ypis + SAFE_DIST4,
                     (MEM_TITLE_ADD + (g_code_mode + 8 + chpins) * TitleLengthMax),
                     SheetPage, g_code_mode + 8 + chpins, keybmpind[KEY_MODIFY3_INP1].textColor[0]);
    }
    else if (F_FILETYPE == PAT_SEW)
    {
        Draw24x24Str(keyd[KEY_MODIFY3_INP1].xpis + (keyd[KEY_MODIFY3_INP1].xsize - 13 * strlen((char *)(MEM_TITLE_ADD + (g_code_mode + chpins) * TitleLengthMax))) / 2,
                     keyd[KEY_MODIFY3_INP1].ypis + SAFE_DIST4,
                     (MEM_TITLE_ADD + (g_code_mode + chpins) * TitleLengthMax),
                     SheetPage, g_code_mode + chpins, keybmpind[KEY_MODIFY3_INP1].textColor[0]);
    }

    AnaPattern(0, F_FILETYPE, (F_FILELEN));
    // dis_updata1 = 1;
    dis_updata2 = 1;

    g_tmpoxpos = mult1 * g_xabspos;
    g_tmpoypos = mult1 * g_yabspos;

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                 SheetPage, title, rect[0].colorT[0]); //����

    if (enCanel)
    {
        DrawButton(keyd[KEY_MODIFY3_CANCEL], (U16 *)(ADD_ICONBMP),
                   keybmpind[KEY_MODIFY3_CANCEL].border[0], keybmpind[KEY_MODIFY3_CANCEL].icon[0]);
    }

    if (enZoom)
    {
        for (i = KEY_MODIFY3_ZOUT; i <= KEY_MODIFY3_ZIN; i++)
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                       keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
        DrawButtonDisPat(keyd[KEY_MODIFY3_IMG], keybmpind[KEY_MODIFY3_IMG].border[0], keybmpind[KEY_MODIFY3_IMG].icon[0]);
    }

    //����������ʾ
    for (i = 2; i <= KEY_MODIFY3_PAT; i++)
    {
        if (i == KEY_MODIFY3_PAT)
        {
        }
        else
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
    // getFeedPos(g_jog_point,F_FILETYPE,&fpx,&fpy);
    scale = pat_getScale(patRule.h_patConvasIndex, PATRULES_MODIFY3_ENSURE);
    vscale = pat_getFinalScale(scale, scale1);

    ViewPage(usepageno); //������ʾ����

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_PAT + 1, 0);
        if (keydata == 0 && enCanel)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_CANCEL + 1, KEY_MODIFY3_CANCEL);
        }
        if (keydata == 0 && enZoom)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_IMG + 1, KEY_MODIFY3_ZOUT);
        }
        if (keydata == 0 && tonear)
        {
            // keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_TOP + 1, KEY_MODIFY3_TOP);
        }
        if (keydata == 0 && endir)
        {
            // keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_MDR + 1, KEY_MODIFY3_MUL);
        }
        if (keydata == 0 && ennum)
        {
            // keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_9 + 1, KEY_MODIFY3_DUP);
        }
        if (keydata == 0 && enmode)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_MET1 + chSelModeMax, KEY_MODIFY3_MET1);
        }
        if (keydata == 0 && chSelInputMax > 1)
        {
            keydata = get_keydata((KEYPAD *)keyd, KEY_MODIFY3_INP1 + chSelInputMax, KEY_MODIFY3_INP1);
        }

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����
        //��Ч����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon >= KEY_MODIFY3_ZOUT && keyon <= KEY_MODIFY3_IMG))
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            //�ض�����²���Ч����
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ
            {
                keyon = keydata;
                if (keyon == KEY_MODIFY3_SCRNSVR || keyon == KEY_MODIFY3_IMG) //�ް�����Ч
                {
                }
                //�ǳ��水��-//������ɫ���ְ���
                else if (keyon == KEY_MODIFY3_INP1 || keyon == KEY_MODIFY3_INP2)
                {
                    DrawButtonOnZoomIn(keyd[keyon], 2);
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //������������
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
                {
                    beeponf = 1;

                    if (g_keyBellT) //����ʱ����������ʱ��
                        Beep_On(g_keyBellT);

                    switch (keyon) //�������±�����ʾ����ʾ
                    {
                    case KEY_MODIFY3_MET1:
                    case KEY_MODIFY3_MET2:
                    case KEY_MODIFY3_MET3:
                    case KEY_MODIFY3_MET4:
                        DrawFMenuStr(keyTitList[para][keyon - KEY_MODIFY3_MET1]);
                        break;
                    case KEY_MODIFY3_INP1:
                    case KEY_MODIFY3_INP2:
                        DrawFMenuStr(rectTitList[para][keyon - KEY_MODIFY3_INP1 + 2]);
                        break;
                    //case KEY_MODIFY3_PFOOT:
                    //    DrawFMenuStr((pfootf == 0) ? keybmpind[keyon].titleStr[0] : keybmpind[keyon].titleStr[1]);
                    //   break;
                    // case KEY_MODIFY3_0:
                    // case KEY_MODIFY3_1:
                    // case KEY_MODIFY3_2:
                    // case KEY_MODIFY3_3:
                    // case KEY_MODIFY3_4:
                    // case KEY_MODIFY3_5:
                    // case KEY_MODIFY3_6:
                    // case KEY_MODIFY3_7:
                    // case KEY_MODIFY3_8:
                    // case KEY_MODIFY3_9:
                    //     BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                    //     DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_MODIFY3_0, 1, rect[0].colorT[0]);
                    //     break;
                    default:
                        DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                        break;
                    }
                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                    delay = KEYNT;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }
                Times = 0; //��ǰ
                if ((keyon >= KEY_MODIFY3_ZOUT && keyon <= KEY_MODIFY3_IMG))
                {
                    keyoff = keyon;
                }
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            if (keyoff >= KEY_MODIFY3_MET1 && keyoff <= KEY_MODIFY3_MET8)
            {
                switch (para)
                {
                case 0:
                case 1:
                    DelNO = keyoff - KEY_MODIFY3_MET1;
                    if (DelNO)
                    {
                    }
                    dis_updata3 = 1;
                    break;
                default:
                    break;
                }
            }
            else if (keyoff >= KEY_MODIFY3_INP1 && keyoff <= KEY_MODIFY3_INP2)
            {
                switch (para)
                {
                case 0:
                case 1:
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (keyoff)
                {
                case KEY_MODIFY3_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;

                case KEY_MODIFY3_BACK:
                    pno = poldno[--pnoindex];
                    break;

                case KEY_MODIFY3_ENTER:
                    if (F_FILETYPE == PAT_MIT)
                    { //����
                        if (g_modify_code == 0)
                        {
                            f = Mit_Modify_Data(8);
                            if (g_code_mode == 20)
                            { //��Ƕ�
                                Write_Cmd(C_PIN_ANG, g_pinAng, 0);
                                Read_Status();
                                g_curPinAng = g_pinAng;
                                g_jog_point++;
                            }
                            else if (g_code_mode == 21)
                            { //����һ
                                Write_Cmd(C_SET_PARA1, g_code_para, 0);
                                Read_Status();
                                g_curSetPara1 = g_code_para;
                                g_jog_point++;
                            }
                            else if (g_code_mode == 22)
                            { //���ö�
                                Write_Cmd(C_SET_PARA2, g_code_para, 0);
                                Read_Status();
                                g_curSetPara2 = g_code_para;
                                g_jog_point++;
                            }
                        }
                        else
                        {
                            f = Mit_Modify_Data(9);
                        }
                        if (f != 3)
                        {
                            pno = MENU_MODIFY;
                            pnoindex -= 2;
                            //								if((g_code_mode==13)&&(g_modify_code==1)){
                            //									f = Go_Home(MENU_MODIFY3,1);	//0
                            //									if(f==0)break;
                            //									g_jog_point = 0;
                            //								}
                        }
                    }
                    else if (F_FILETYPE == PAT_SEW)
                    { //�ֵ�
                        if (g_modify_code == 1)
                        { //ɾ������
                            if (g_BorDelFuns != 0)
                            { //�ж��ָ��ӹ���
                                for (i = 0; i < DelFunnum; i++)
                                {
                                    if (DelFun[i] == 1)
                                        break; //��ѡ��ɾ���Ĺ���
                                }
                                if (i >= DelFunnum)
                                { //û��ѡ��ɾ��һ�ָ��ӹ���
                                    pnoindex--;
                                    pno = poldno[pnoindex];
                                    break;
                                }

                                wdt();
                                tmpDelFuns = g_BorDelFuns;
                                g_BorDelFuns = 0;
                                for (i = 0, j = 0; (i < 8) && (j < DelFunnum); i++)
                                {
                                    if ((tmpDelFuns >> i) & 0x01)
                                    {
                                        if (DelFun[j] == 1)
                                        {
                                            g_BorDelFuns |= (1 << i);
                                        }
                                        j++;
                                    }
                                }

                                if (tmpDelFuns == g_BorDelFuns)
                                { //ɾ��ȫ�����ӹ���ָ��
                                    delflag = 0;
                                }
                                else
                                { //ɾ�����ָ��ӹ���ָ��
                                    delflag = 1;
                                    g_BorDelFuns = ~g_BorDelFuns;
                                    g_BorDelFuns &= tmpDelFuns;
                                }
                            }
                            wdt();
                        }
                        f = Bor_ModCode(g_jog_point, g_modify_code + 1, g_code_mode - 8, g_patd.pat_len, delflag);
                        if (f == 0)
                        {
                            g_modifyflag = 1;
                            g_Tsavetimes = g_savetimes;
                            g_Tsavetimes++;
                            g_savetimes = g_Tsavetimes;
                            BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                            pno = MENU_MODIFY;
                            pnoindex -= 2;
                            if (g_modify_code == 0)
                            { //����ָ��
                                if (g_code_mode == 33)
                                { //��Ƕ�
                                    Write_Cmd(C_PIN_ANG, g_pinAng, 0);
                                    Read_Status();
                                    g_curPinAng = g_pinAng;
                                    g_jog_point++;
                                }
                                else if (g_code_mode == 40)
                                { //����һ
                                    Write_Cmd(C_SET_PARA1, g_code_para, 0);
                                    Read_Status();
                                    g_curSetPara1 = g_code_para;
                                    g_jog_point++;
                                }
                                else if (g_code_mode == 41)
                                { //���ö�
                                    Write_Cmd(C_SET_PARA2, g_code_para, 0);
                                    Read_Status();
                                    g_curSetPara2 = g_code_para;
                                    g_jog_point++;
                                }
                            }
                            if (g_code_mode == 21)
                            {
                                if (g_modify_code == 0)
                                { //�����ԭ��
                                    g_jog_point = 0;
                                }
                                else
                                { //ɾ����ԭ��
                                    g_jog_point = 1;
                                }
                            }
                        }
                        else if (f == 1)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 22;
                            poldno[pnoindex] = MENU_MODIFY3;
                            pnoindex++;
                            EREG[1] = 160;
                        }
                        else if (f == 2)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 66;
                            poldno[pnoindex] = MENU_MODIFY3;
                            pnoindex++;
                        }
                    }

                    break;

                case KEY_MODIFY3_PAT:
                    pno = MENU_MOIMAGE;
                    poldno[pnoindex] = MENU_MODIFY3;
                    pnoindex++;
                    g_parameter = tmp_parameter;
                    g_jog_point = tmp_feedc;

                    break;

                case KEY_MODIFY3_CANCEL:
                    if (para == 0)
                    { //�ٶ�����
                        if (tmp_parameter != 1)
                        {
                            tmp_parameter = 1;
                            dis_updata4 = 1;
                            if (tmp_selectmode != 1)
                            {
                                dis_updata2 = 1;
                            }
                        }
                    }
                    break;

                case KEY_MODIFY3_ZOUT: //ͼ����С
                    if (scale1)
                    {
                        scale1 -= 50;
                        dis_updata2 = 2;
                    }
                    else
                    {
                        if (bResetCaonvas)
                        {
                            bResetCaonvas = 0;
                            xo = patConvas.conOrgX;
                            yo = patConvas.conOrgY;
                            scale1 = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            dis_updata2 = 2;
                        }
                    }
                    break;

                case KEY_MODIFY3_ZIN: //ͼ�ηŴ�
                    scale1 += 50;
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;

                case KEY_MODIFY3_IMG: //ͼ���ƶ�
                    if (beeponf > 1)
                    {
                        if ((absInt(keyxpct - okeyxpct) <= 10) && (absInt(keyypct - okeyypct) <= 10) && (absInt(keyxpct - stkeyxpct) <= 10) && (absInt(keyypct - stkeyypct) <= 10) && (absInt(okeyxpct - stkeyxpct) <= 10) && (absInt(okeyypct - stkeyypct) <= 10) && (beeponf == 15))
                        {
                            xo = ORG_X;
                            yo = ORG_Y;
                            scale1 = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            Beep_On(BELLT);
                        }
                        else
                        {
                            xo += (keyxpct - okeyxpct);
                            yo += (keyypct - okeyypct);
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                        }
                    }
                    else
                    {
                        okeyxpct = keyxpct;
                        okeyypct = keyypct;
                        stkeyxpct = keyxpct;
                        stkeyypct = keyypct;
                    }
                    dis_updata2 = 1;
                    bResetCaonvas = 1;
                    break;
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {
            if (keyon == KEY_MODIFY3_SCRNSVR || keyon == KEY_MODIFY3_IMG) //����Ҫ��ʾ�İ���
            {
            }
            //�ǳ��水��-//������ɫ���ְ���
            else if (keyon == KEY_MODIFY3_INP1 || keyon == KEY_MODIFY3_INP2)
            {
                DrawButtonOnZoomInRes(keyd[keyon]);
            }
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }

            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
            Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + (U32)(title)*TitleLengthMax,
                         SheetPage, title, rect[0].colorT[0]); //����
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
        {
            ErrorMsg(MENU_MODIFY3);
        }
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MODIFY3);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[���ְ�ť�뷽��ťˢ��*begin]**********************************************************
        if (dis_updata1)
        {
            dis_updata1 = 0;
        }
        // /[���ְ�ť�뷽��ťˢ��*end]**********************************************************

        // /[ˢ�»�����ʾ*begin]**********************************************************
        if (dis_updata2)
        {

            vscale = pat_getFinalScale(scale, scale1);
            xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
            yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
            posx1 = (S32)((g_tmpoxpos + mult1 * g_dxpos) * (vscale) / 1000 + xorg);
            posy1 = (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * (vscale) / 1000 + yorg);
            if (((posx1 < keyd[KEY_MODIFY3_IMG].xpis) || (posx1 > keyd[KEY_MODIFY3_IMG].xpis + keyd[KEY_MODIFY3_IMG].xsize) || (posy1 < keyd[KEY_MODIFY3_IMG].ypis) || (posy1 > keyd[KEY_MODIFY3_IMG].ypis + keyd[KEY_MODIFY3_IMG].ysize)))
            { //������ʾ��Χ
                xorg = ORG_X;
                yorg = ORG_Y;
                xorg += (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg += tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;

                xo = xorg + (xorg - (S32)((g_tmpoxpos + mult1 * g_dxpos) * vscale / 1000 + xorg));
                yo = yorg + (yorg - (S32)(mult2 * tmuly * (g_tmpoypos + mult1 * g_dypos) * vscale / 1000 + yorg));

                xorg = xo - (g_maxX + g_minX) / 2 * (vscale) / 1000;
                yorg = yo - tmuly * (g_maxY + g_minY) / 2 * (vscale) / 1000;
            }
            pat_clearConvas(patRule.h_patConvasIndex);
            pat_drawPattern(PATRULES_MODIFY3_ENSURE, scale1, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
            // newDrawPat(25, scale1, xo, yo, 0, 0);
            dis_updata2 = 0;
        }
        // /[ˢ�»�����ʾ*end]**********************************************************

        // /[��������ģʽ����*begin]**********************************************************
        if (dis_updata16)
        {
            dis_updata16 = 0;
            BmpFillRect(rect[RECT_MODIFY3_METHOD].xpis, rect[RECT_MODIFY3_METHOD].ypis,
                        rect[RECT_MODIFY3_METHOD].xsize + keyd[KEY_MODIFY3_MET1].xsize * chSelModeMax, rect[RECT_MODIFY3_METHOD].ysize);
        }
        // /[��������ģʽ����*end]**********************************************************

        // /[����edit��ʾ para == 1|2|3|4*begin]**********************************************************
        if (dis_updata17)
        {
            dis_updata17 = 0;
            if (chSelInputMax == 1 || chSelInputMax == 0)
            {
                BmpFillRect(rect[RECT_MODIFY3_INP1_TIT].xpis, rect[RECT_MODIFY3_INP1_TIT].ypis,
                            rect[RECT_MODIFY3_INP1_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY3_INP1].xsize,
                            rect[RECT_MODIFY3_INP1_TIT].ysize + keyd[KEY_MODIFY3_INP1].ysize);
            }
            if (chSelInputMax == 2 || chSelInputMax == 0)
            {
                BmpFillRect(rect[RECT_MODIFY3_INP2_TIT].xpis, rect[RECT_MODIFY3_INP2_TIT].ypis,
                            rect[RECT_MODIFY3_INP2_TIT].xsize + SAFE_DIST6 + keyd[KEY_MODIFY3_INP2].xsize,
                            rect[RECT_MODIFY3_INP2_TIT].ysize + keyd[KEY_MODIFY3_INP2].ysize);
            }
        }
        // /[����edit��ʾ para == 1|2|3|4*end]**********************************************************

        // /[��ʾedit para == 1|2|3|4*begin]**********************************************************
        if (dis_updata18)
        {
            dis_updata18 = 0;
        }
        // /[��ʾedit para == 1|2|3|4*end]**********************************************************

        // /[����ģʽ������ʾ*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;

            BmpFillRect(rect[RECT_MODIFY3_METHOD].xpis, rect[RECT_MODIFY3_METHOD].ypis,
                        rect[RECT_MODIFY3_METHOD].xsize + chSelModeMax * keyd[KEY_MODIFY3_MET1].xsize,
                        rect[RECT_MODIFY3_METHOD].ysize);

            DrawRect(rect[RECT_MODIFY3_METHOD].xpis, rect[RECT_MODIFY3_METHOD].ypis,
                     rect[RECT_MODIFY3_METHOD].xsize + chSelModeMax * KEY_X6, rect[RECT_MODIFY3_METHOD].ysize, rect[RECT_MODIFY3_METHOD].colorB[0]);
            Draw16x16Str(rect[RECT_MODIFY3_METHOD].xpis + SAFE_DIST3, rect[RECT_MODIFY3_METHOD].ypis + SAFE_DIST3,
                         (MEM_TITLE_ADD + (U32)(rect[RECT_MODIFY3_METHOD].index[0] * TitleLengthMax)),
                         SheetPage, rect[RECT_MODIFY3_METHOD].index[0], rect[RECT_MODIFY3_METHOD].colorT[0]); //�������������

            if (para == 0)
            {
                for (i = KEY_MODIFY3_MET1; i < KEY_MODIFY3_MET1 + chSelModeMax; i++)
                {
                    f = i - KEY_MODIFY3_MET1;
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                               (tmp_selectmode == f) ? keybmpind[i].border[2] : keybmpind[i].border[0],
                               (tmp_selectmode == f) ? keyBmpList[para][f * 3 + 2] : keyBmpList[para][f * 3 + 0]);
                }
            }
        }
        // /[����ģʽ������ʾ*end]**********************************************************

        // /[edit����ˢ�� para == 1|2|3|4*begin]**********************************************************
        if (dis_updata4)
        {
            dis_updata4 = 0;
            DrawButtonEdit(keyd[KEY_MODIFY3_INP1],
                           keybmpind[KEY_MODIFY3_INP1].textColor[0], keybmpind[KEY_MODIFY3_INP1].icon[1]);
        }
        // /[edit����ˢ�� para == 1|2|3|4*end]**********************************************************

        // /[ˢ�����������Ϣ*begin]**********************************************************
        if (dis_updata15)
        {
            dis_updata15 = 0;
            DrawFillRect(rect[RECT_MODIFY3_X_STR].xpis,
                         rect[RECT_MODIFY3_X_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_MODIFY3_X_STR].ysize,
                         rect[RECT_MODIFY3_X_STR].colorB[0]);
            DrawFS32P(rect[RECT_MODIFY3_X_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY3_X_STR].ypis + SAFE_DIST4,
                      g_dxpos, 6, 1, rect[RECT_MODIFY3_X_STR].colorT[0]);
            DrawFillRect(rect[RECT_MODIFY3_Y_STR].xpis,
                         rect[RECT_MODIFY3_Y_STR].ypis,
                         SAFE_DIST3 + 9 * 6,
                         rect[RECT_MODIFY3_Y_STR].ysize,
                         rect[RECT_MODIFY3_Y_STR].colorB[0]);
            DrawFS32P(rect[RECT_MODIFY3_Y_STR].xpis + SAFE_DIST3, rect[RECT_MODIFY3_Y_STR].ypis + SAFE_DIST4,
                      g_dypos, 6, 1, rect[RECT_MODIFY3_Y_STR].colorT[0]);
        }
        // /[ˢ�����������Ϣ*end]**********************************************************

        // /[��ʱ200ms��Ϣ����*begin]**********************************************************
        if (Timerms > 200 && dis_pos)
        {
            Timerms = 0;
            if (Read_xyposi())
            {

                if (absposx != g_xabspos)
                {
                    absposx = g_xabspos;
                    DrawFillRect(rect[RECT_MODIFY3_X_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_MODIFY3_X_STR].ypis,
                                 rect[RECT_MODIFY3_X_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_MODIFY3_X_STR].ysize,
                                 rect[RECT_MODIFY3_X_STR].colorB[0]);
                    DrawF16Str(rect[RECT_MODIFY3_X_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_MODIFY3_X_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_MODIFY3_X_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1, g_xabspos, 6, 1, Color_Black);
                    }
#else

                    DrawFS32P(rect[RECT_MODIFY3_X_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_MODIFY3_X_STR].ypis + SAFE_DIST4,
                              g_xabspos, 6, 1, rect[RECT_MODIFY3_X_STR].colorT[0]);
#endif
                }
                if (absposy != g_yabspos)
                {
                    absposy = g_yabspos;
                    DrawFillRect(rect[RECT_MODIFY3_Y_STR].xpis + SAFE_DIST3 + 9 * 6,
                                 rect[RECT_MODIFY3_Y_STR].ypis,
                                 rect[RECT_MODIFY3_Y_STR].xsize - (SAFE_DIST3 + 9 * 6),
                                 rect[RECT_MODIFY3_Y_STR].ysize,
                                 rect[RECT_MODIFY3_Y_STR].colorB[0]);
                    DrawF16Str(rect[RECT_MODIFY3_Y_STR].xpis + SAFE_DIST3 + 9 * 6, rect[RECT_MODIFY3_Y_STR].ypis + SAFE_DIST4,
                               "(      )", rect[RECT_MODIFY3_Y_STR].colorT[0]);
#if MOVEUNIT == 1
                    if (g_curPTtype == 2)
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos * 5, 7, 2, Color_Black);
                    }
                    else
                    {
                        DrawMFS32P(vxp1, vyp1 + 32, g_yabspos, 6, 1, Color_Black);
                    }
#else
                    DrawFS32P(rect[RECT_MODIFY3_Y_STR].xpis + SAFE_DIST3 + 9 * 7,
                              rect[RECT_MODIFY3_Y_STR].ypis + SAFE_DIST4,
                              g_yabspos, 6, 1, rect[RECT_MODIFY3_Y_STR].colorT[0]);
#endif
                }
            }
        }
        // /[��ʱ200ms��Ϣ����*end]**********************************************************

        // /[��ѹ�Ű�ťˢ��*begin]**********************************************************
        if (pfootf != (U16)(S_PFOOT))
        {
            pfootf = (U16)(S_PFOOT);
        }
        // /[��ѹ�Ű�ťˢ��	*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MODIFY3)
        {
            plastno = MENU_MODIFY3;
            break;
        }
        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
#endif
}

/*************************************************/
#define KEY_MOIMAGE_SCRNSVR 1    //1 ����
#define KEY_MOIMAGE_BACK 2       //2 ����
#define KEY_MOIMAGE_ZOOMOUT 3    //3 ��С
#define KEY_MOIMAGE_ZOOMIN 4     //4 �Ŵ�
#define KEY_MOIMAGE_ENTER 5      //5 ȷ��
#define KEY_MOIMAGE_IMAGETOUCH 6 //6 ����Ų��
#define KEY_MOIMAGE_CODE 7       //7 ��ʾ����

#define KEY_MOIMAGE_MAX1 8

#define KEY_MOIMAGE_PP 8        //8 ��Ե�ֱ���ƶ�
#define KEY_MOIMAGE_PAT 9       //9 ��Ե㰴���ƶ�
#define KEY_MOIMAGE_PINS 10     //10 С���̵���
#define KEY_MOIMAGE_LOCATION 11 //11 ��ת��ǰ֡
#define KEY_MOIMAGE_MUP 12      //12 ������
#define KEY_MOIMAGE_MDOWN 13    //13 �����

#define KEY_MOIMAGE_MAX2 6

#define KEY_MOIMAGE_CANCLE 14 //14 ȡ��
#define KEY_MOIMAGE_OK 15     //15 ȷ��
#define KEY_MOIMAGE_CLR 16    //16 ���һλ
#define KEY_MOIMAGE_ADD 17    //17 +1
#define KEY_MOIMAGE_DES 18    //18 -1
#define KEY_MOIMAGE_0 19      //19 0
#define KEY_MOIMAGE_1 20      //20 1
#define KEY_MOIMAGE_2 21      //21 2
#define KEY_MOIMAGE_3 22      //22 3
#define KEY_MOIMAGE_4 23      //23 4
#define KEY_MOIMAGE_5 24      //24 5
#define KEY_MOIMAGE_6 25      //25 6
#define KEY_MOIMAGE_7 26      //26 7
#define KEY_MOIMAGE_8 27      //27 8
#define KEY_MOIMAGE_9 28      //28 9

#define KEY_MOIMAGE_MAX3 15

#define RECT_MOIMAGE_NAME_TIT 1  //1 ����-����
#define RECT_MOIMAGE_NAME_STR 2  //2 ����-����
#define RECT_MOIMAGE_NUM_TIT 3   //3 ���-����
#define RECT_MOIMAGE_NUM_STR 4   //4 ���-����
#define RECT_MOIMAGE_XSIZE_TIT 5 //5 X�ߴ�-����
#define RECT_MOIMAGE_XSIZE_STR 6 //6 X�ߴ�-����
#define RECT_MOIMAGE_YSIZE_TIT 7 //7 Y�ߴ�-����
#define RECT_MOIMAGE_YSIZE_STR 8 //8 Y�ߴ�-����
#define RECT_MOIMAGE_PINS_TIT 9  //9 ����-����
#define RECT_MOIMAGE_PINS_STR 10 //10 ����-����
#define RECT_MOIMAGE_TYPE_TIT 11 //11 ����-����
#define RECT_MOIMAGE_TYPE_STR 12 //12 ����-����

#define RECT_MOIMAGE_MAX1 13

#define RECT_MOIMAGE_PINDATA 13  //13 ��������
#define RECT_MOIMAGE_PININPUT 14 //14 ������-����
#define RECT_MOIMAGE_MOD 15      //15 ��������ģ��

#define RECT_MOIMAGE_MAX2 3

#define RECT_MOIMAGE_SIZE 16 //16 �ߴ���ϸ����

#define RECT_MOIMAGE_MAX3 1

#define RECT_MOIMAGE_MASK 17         //17 ����
#define RECT_MOIMAGE_PININPUT_TIT 18 //18 ������-����
#define RECT_MOIMAGE_PININPUT_STR 19 //19 ������-����

#define RECT_MOIMAGE_MAX4 3

//------------------------------------------
// Function   : void Menu_MoImage(U8 para)
// Description: �������
//------------------------------------------
void Menu_MoView(U8 para)
{
    U32 i;
    U8 f = 0;

    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
    /****������ز���end****/

    /****������ţ��������ƣ������б���ز���begin****/
    /****������ţ��������ƣ������б���ز���end****/

    /****������ʾ��ز���begin****/
    /****������ʾ��ز���end****/

    /****��־λ��ز���begin****/
    //U8 threadflag = 0;   //����ģʽ��־  0������   1���������봩��ģʽ  2�����ڴ���ģʽ
    //U8 vPins = 0;	// 0-����ʾ��� 1-��ʾ���
    //	S32 vpl,vpr,vpu,vpd;

    //	U8 xmove =0,ymove =0;
    U8 dis_updata = 1, dis_updata1 = 0,       //dis_updata:1��ȫ���ػ滨����2�������ƶ����� ;dis_updata1:���µ�ǰʵ����������
        dis_updata2 = 0, dis_updata3 = 0,     //dis_updata2:���뷽����ť��ʾ�л� ; dis_updata3:��ʾ��ǰָʾ������
        /*dis_updata4 = 0,*/ dis_updata5 = 0  //dis_updata4:��ʾС���̱�־(��pinsetfOld����); dis_updata5:С�������ݸ���
        /*dis_updata6 = 0, dis_updata7 = 0*/; //dis_updata6:��ʾָʾ���λ��;
    static U8 EnUPflag = 0;

    S16 xo = CONVAS_ORG_PX3; //��ǰ��ʾ��ԭ������
    S16 yo = CONVAS_ORG_PY3;
    COORDINATE2 POSI;
    S32 Scale = 0; //ʵ���������ű���
    U8 bResetCaonvas = 0;
    // S8 tmuly = 1;
    // S32 tscale = 1, tscale1 = 1;
    // S16 xtmp, ytmp;

    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    U16 keyxpos = 0, keyypos = 0, keyxdis = 0, keyydis = 0;
    U16 Borcmd;
    //S16 temp_oxpos,temp_oypos,dx,dy;
    S16 temp_oxpos1, temp_oypos1, temp_oxpos2, temp_oypos2, tempdx, tempdy;
    S8 Borpooft, Borpfdis;
    //U8 speed;
    U8 Up = 0;
    U8 Dn = 1;
    U8 pfootf = 0xff;
    U8 movfeed = 0;
    U8 excode = 0;
    U32 tmpfeed = 0, PinSets = 0; //PinSets:���������PINֵ��tmpfeed����ǰȷ�ϵ���ֵ
    U32 runf1 = 0, runf2 = 0;
    U8 pinsetf = 0, pinsetfOld = 0; //С�������빦�� 1:��С����
    U16 moddat = 0;                 //�������λ��
    U32 ii = 0, dat = 0, dat1 = 0;
    U8 uplock = 1;
    S8 pfootval = 0;
    S16 old_PfootOffVal = 0;
    S16 temp_PfVal = 0;
    U8 x = 0, y = 0;
    U8 isHome;
    U8 onpaint;
    U16 tBorcmd;
    S16 ttemp_oxpos1, ttemp_oypos1, tdxpos, tdypos;
    U8 toldspeed;
    S8 tBorpooft, tBorpfdis;

    U8 chCodeEnTemp = 0;
    U8 patRulesTmp = PATRULES_JOG_DETAIL;

    static U8 chViewType = 0;
    const char *chFileType[] = {"0", "M", "B", "E"};
    /****��־λ��ز���end***/

    //    const U8 keynum = {KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2 + KEY_MOIMAGE_MAX3};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},             //2 ����
        {TYPE_X10, TYPE_Y5, KEY_X1, KEY_Y1},            //3 ��С
        {TYPE_X11, TYPE_Y5, KEY_X1, KEY_Y1},            //4 �Ŵ�
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},            //5 ȷ��
        {CONVAS_PX3, CONVAS_PY3, CONVAS_X3, CONVAS_Y3}, //6 ����Ų��
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},            //7 ��ʾ����

        {TYPE_X10, TYPE_Y23, KEY_X1, KEY_Y1},                  //8 ��Ե�ֱ���ƶ�
        {TYPE_X11, TYPE_Y23, KEY_X1, KEY_Y1},                  //9 ��Ե㰴���ƶ�
        {TYPE_X11, TYPE_Y24, KEY_X1, KEY_Y1},                  //10 С���̵���
        {TYPE_X10, TYPE_Y24, KEY_X1, KEY_Y1},                  //11 ��ת��ǰ֡
        {TYPE_X1, 117, KEY_X1, KEY_Y1},                        //12 ������
        {TYPE_X1, 117 + KEY_Y1 + TYPE_SIZE16, KEY_X1, KEY_Y1}, //13 �����

        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},                                                           //14 ȡ��
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},                                                          //15 ȷ��
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 4, KEY_X2, KEY_Y2},                             //16 ���һλ
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},       //17 +1
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2}, //18 -1
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 3, KEY_X2, KEY_Y2},                             //19 0
        {TYPE_X15, TYPE_Y13, KEY_X2, KEY_Y2},                                                         //20 1
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13, KEY_X2, KEY_Y2},                                   //21 2
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13, KEY_X2, KEY_Y2},                             //22 3
        {TYPE_X15, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},                                   //23 4
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},             //24 5
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + KEY_Y2 + SAFE_DIST5, KEY_X2, KEY_Y2},       //25 6
        {TYPE_X15, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},                             //26 7
        {TYPE_X15 + KEY_X2 + SAFE_DIST5, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2},       //27 8
        {TYPE_X15 + (KEY_X2 + SAFE_DIST5) * 2, TYPE_Y13 + (KEY_Y2 + SAFE_DIST5) * 2, KEY_X2, KEY_Y2}, //28 9

    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {1, 0, 0, 0}, {1}},                                      //1 ����
        {{KEY_Silver}, {0}, {72}, {2, 0, 0, 0}, {2}},                                     //2 ����
        {{KEY_Silver}, {0}, {74}, {4, 0, 0, 0}, {0}},                                     //3 ��С
        {{KEY_Silver}, {0}, {75}, {3, 0, 0, 0}, {0}},                                     //4 �Ŵ�
        {{KEY_Silver}, {0}, {71}, {5, 0, 0, 0}, {5}},                                     //5 ȷ��
        {{KEY_Silver}, {0}, {Color_Black}, {9, 0, 0, 0}, {0}},                            //6 ����Ų��
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {375, 376, 377, 3}, {26, 0, 0, 25}, {0}}, //7 ��ʾ����

        {{KEY_Silver, KEY_Blue}, {0}, {82, 83}, {16, 0, 0, 0}, {0}},  //8 ��Ե�ֱ���ƶ�
        {{KEY_Silver, KEY_Blue}, {0}, {84, 85}, {17, 0, 0, 0}, {0}},  //9 ��Ե㰴���ƶ�
        {{KEY_Silver}, {0}, {86}, {18, 24, 0, 0}, {18, 24}},          //10 С���̵���
        {{KEY_Silver}, {0}, {38}, {6, 0, 0, 0}, {6}},                 //11 ��ת��ǰ֡
        {{KEY_Silver, KEY_Gray}, {0}, {191, 192}, {7, 0, 0, 0}, {0}}, //12 ������
        {{KEY_Silver, KEY_Gray}, {0}, {193, 194}, {8, 0, 0, 0}, {0}}, //13 �����

        {{KEY_Silver}, {0}, {72}, {19, 0, 0, 0}, {0}},  //14 ����
        {{KEY_Silver}, {0}, {71}, {20, 0, 0, 0}, {0}},  //15 ȷ��
        {{KEY_Silver}, {0}, {113}, {21, 0, 0, 0}, {0}}, //16 ���һλ
        {{KEY_Silver}, {0}, {199}, {22, 0, 0, 0}, {0}}, //17 +1
        {{KEY_Silver}, {0}, {201}, {23, 0, 0, 0}, {0}}, //18 -1
        {{KEY_Silver}, {0}, {91}, {0, 0, 0, 0}, {0}},   //19 0
        {{KEY_Silver}, {0}, {92}, {0, 0, 0, 0}, {0}},   //20 1
        {{KEY_Silver}, {0}, {93}, {0, 0, 0, 0}, {0}},   //21 2
        {{KEY_Silver}, {0}, {94}, {0, 0, 0, 0}, {0}},   //22 3
        {{KEY_Silver}, {0}, {95}, {0, 0, 0, 0}, {0}},   //23 4
        {{KEY_Silver}, {0}, {96}, {0, 0, 0, 0}, {0}},   //24 5
        {{KEY_Silver}, {0}, {97}, {0, 0, 0, 0}, {0}},   //25 6
        {{KEY_Silver}, {0}, {98}, {0, 0, 0, 0}, {0}},   //26 7
        {{KEY_Silver}, {0}, {99}, {0, 0, 0, 0}, {0}},   //27 8
        {{KEY_Silver}, {0}, {100}, {0, 0, 0, 0}, {0}},  //28 9
    };

    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X1, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {27}},                          //1 ����-����
        {TYPE_X1 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y2, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},  //2 ����-����
        {TYPE_X1, TYPE_Y16, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {28}},                         //3 ���-����
        {TYPE_X1 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y16, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}}, //4 ���-����

        {TYPE_X26, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {29}},                          //5 X�ߴ�-����
        {TYPE_X26 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y2, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},  //6 X�ߴ�-����
        {TYPE_X26, TYPE_Y16, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {30}},                         //7 Y�ߴ�-����
        {TYPE_X26 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y16, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}}, //8 Y�ߴ�-����

        {TYPE_X27, TYPE_Y2, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {31}},                          //9 ����-����
        {TYPE_X27 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y2, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}},  //10 ����-����
        {TYPE_X27, TYPE_Y16, TYPE_SIZE14, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {32}},                         //11 ����-����
        {TYPE_X27 + TYPE_SIZE14 + SAFE_DIST5, TYPE_Y16, TYPE_SIZE17, TYPE_SIZE5, {Color_White}, {Color_Black}, {0}}, //12 ����-����

        {TYPE_X1, 117 + KEY_Y1, KEY_X1, TYPE_SIZE16, {Color_Black}, {Color_White}, {0}},                                                  //13 ��������
        {TYPE_X10, TYPE_Y25, KEY_X1 * 2 + SAFE_DIST1, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}},                               //14 ������
        {TYPE_X10 - SAFE_DIST4, TYPE_Y23 - SAFE_DIST4, (KEY_X1 + SAFE_DIST1) * 2, KEY_Y1 + SAFE_DIST1, {Color_Gray}, {Color_White}, {0}}, //15 ��������ģ��

        {TYPE_X10, TYPE_Y25, KEY_X1 * 2 + SAFE_DIST1, TYPE_SIZE10, {Color_White}, {Color_Black}, {0}}, //16 �ߴ���ϸ����

        {TYPE_X15 - SAFE_DIST6, TYPE_Y3 - SAFE_DIST6, SAFE_DIST6 * 2 + 169, SAFE_DIST6 * 2 + 325, {Color_Gray}, {Color_White}, {0}}, //17 ����
        {TYPE_X15, TYPE_Y3, TYPE_SIZE9, TYPE_SIZE5, {Color_KeyBlue}, {Color_White}, {31}},                                           //18 Y�ߴ�-����
        {TYPE_X15 + TYPE_SIZE9 + SAFE_DIST5, TYPE_Y3, TYPE_SIZE14, TYPE_SIZE5, {Color_DeepSkyBlue}, {Color_Black}, {0}},             //19 Y�ߴ�-����

    };

    // const U8 keynum2 = {KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2 + KEY_MOIMAGE_MAX3};
    // const KEYPAD keyd2[] = {
    //     {0, 0, 0, 0},

    //     {0, 0, 128, 32},                                //1 ����
    //     {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},             //2 ����
    //     {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},             //3 ��С
    //     {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},             //4 �Ŵ�
    //     {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},            //5 ȷ��
    //     {CONVAS_PX3, CONVAS_PY3, CONVAS_X3, CONVAS_Y3}, //6 ����Ų��
    // };
    // const RECTPAD rect2[] = {
    //     //����
    //     {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

    //     {TYPE_X4, TYPE_Y4, TYPE_SIZE22, TYPE_SIZE9, {Color_Blue}, {Color_White}, {10}}, //1 �Ƿ�����XX����
    // };
    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MOIMAGE); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(patRulesTmp);
    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    if (g_imgpara == MOIMAGE_MOVE || g_imgpara == MOIMAGE_CHANGE || g_imgpara == MOIMAGE_SKIPJOG)
    {
        chViewType = MOIMAGE_SKIPJOG;
    }
    else if (g_imgpara == MOIMAGE_ENSURE1 || g_imgpara == MOIMAGE_ENSURE2 || g_imgpara == MOIMAGE_VIEW)
    {
        chViewType = MOIMAGE_VIEW;
    }
    IMAGEJOG = 0;
    g_ImgPoint = 0;

    if (plastno != MENU_SCRNSVR)
    {
        g_publicbuf[40] = plastno;
    }
    else
    {
        plastno = g_publicbuf[40];
    }
    Scale = 0;
    g_patd.scale = 100;
    if (g_IsLocal && (g_errLocal == 0)) //��ͼ����ʾ�������
    {
        E_RUNEN();
        Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
    }
    if (g_ImgMove == 1) //�Ӵ綯�������,֧�ֿ����ƶ�
    {
        IMAGEJOG = 1;
        g_ImgPoint = 1;
        tmpfeed = g_jog_point;

        pfootf = (U8)(S_PFOOT);
        if (pfootf == 0x10)
        { //����
            Up = 1;
            Dn = 0;
        }
        else
        {
            Up = 0;
            Dn = 1;
        }

        if (g_max_feedsum < 100)
        {
            moddat = 10;
        }
        else if (g_max_feedsum < 1000)
        {
            moddat = 100;
        }
        else if (g_max_feedsum < 10000)
        {
            moddat = 1000;
        }
        else if (g_max_feedsum < 100000)
        {
            moddat = 10000;
        }
        if (g_jog_point == 0)
        {
            EnUPflag = 1;
        }
        pfootf = 0xff;

        if (chViewType != MOIMAGE_VIEW)
        {
            dis_updata1 = 1;
            dis_updata2 = 1;
            dis_updata3 = 1;
        }
    }

    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //չ��ͼ��
    if (chViewType == MOIMAGE_SKIPJOG || chViewType == MOIMAGE_VIEW)
    {
        //��������
        for (i = 1; i < RECT_MOIMAGE_MAX1; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);

            if (i == RECT_MOIMAGE_NAME_TIT ||
                i == RECT_MOIMAGE_NUM_TIT ||
                i == RECT_MOIMAGE_XSIZE_TIT ||
                i == RECT_MOIMAGE_YSIZE_TIT ||
                i == RECT_MOIMAGE_PINS_TIT ||
                i == RECT_MOIMAGE_TYPE_TIT)
            {
                // Draw24x24Str(rect[i].xpis + (rect[i].xsize - strlen((char *)(MEM_TITLE_ADD + ((U32)rect[i].index[0]) * TitleLengthMax)) * 13) / 2,
                //              rect[i].ypis + (rect[i].ysize - 24) / 2,
                //              MEM_TITLE_ADD + ((U32)rect[i].index[0]) * TitleLengthMax,
                //              SheetPage, rect[i].index[0], rect[i].colorT[0]);
                Draw24x24Str(rect[i].xpis + SAFE_DIST4,
                             rect[i].ypis + (rect[i].ysize - 24) / 2,
                             MEM_TITLE_ADD + ((U32)rect[i].index[0]) * TitleLengthMax,
                             SheetPage, rect[i].index[0], rect[i].colorT[0]);
            }
        }
        DrawFMultiLStr24LimtL(rect[RECT_MOIMAGE_NAME_STR].xpis + SAFE_DIST4, rect[RECT_MOIMAGE_NAME_STR].ypis + (rect[RECT_MOIMAGE_NAME_STR].ysize - 24) / 2,
                              (U16 *)(&g_patd.fname), 11, 1, rect[RECT_MOIMAGE_NAME_STR].colorT[0]); //����1�У������ʾ28���ַ�
        DrawFFileno_24x12(rect[RECT_MOIMAGE_NUM_STR].xpis + SAFE_DIST4, rect[RECT_MOIMAGE_NUM_STR].ypis + (rect[RECT_MOIMAGE_NUM_STR].ysize - 24) / 2,
                          g_patd.fno, rect[RECT_MOIMAGE_NUM_STR].colorT[0]);
        DrawMFU32(rect[RECT_MOIMAGE_PINS_STR].xpis + SAFE_DIST4, rect[RECT_MOIMAGE_PINS_STR].ypis + (rect[RECT_MOIMAGE_PINS_STR].ysize - 24) / 2,
                  g_patd.feed_sum, 5, rect[RECT_MOIMAGE_PINS_STR].colorT[0]);
        Draw24X24FStr(rect[RECT_MOIMAGE_TYPE_STR].xpis + SAFE_DIST4, rect[RECT_MOIMAGE_TYPE_STR].ypis + (rect[RECT_MOIMAGE_TYPE_STR].ysize - 24) / 2,
                      (char *)chFileType[F_FILETYPE], rect[RECT_MOIMAGE_TYPE_STR].colorT[0]);

        DrawMFS32P(rect[RECT_MOIMAGE_XSIZE_STR].xpis + SAFE_DIST4, rect[RECT_MOIMAGE_XSIZE_STR].ypis + (rect[RECT_MOIMAGE_XSIZE_STR].ysize - 24) / 2,
                   F_FILETYPE == PAT_SEW ? ((g_patd.patmX - g_patd.patiX)) : (g_patd.patmX - g_patd.patiX),
                   5, 1, rect[RECT_MOIMAGE_XSIZE_STR].colorT[0]);
        Draw24X24FStr(rect[RECT_MOIMAGE_XSIZE_STR].xpis + rect[RECT_MOIMAGE_XSIZE_STR].xsize - SAFE_DIST10, rect[RECT_MOIMAGE_XSIZE_STR].ypis + (rect[RECT_MOIMAGE_XSIZE_STR].ysize - 24) / 2,
                      "mm", rect[RECT_MOIMAGE_XSIZE_STR].colorT[0]);

        DrawMFS32P(rect[RECT_MOIMAGE_YSIZE_STR].xpis + SAFE_DIST4, rect[RECT_MOIMAGE_YSIZE_STR].ypis + (rect[RECT_MOIMAGE_YSIZE_STR].ysize - 24) / 2,
                   F_FILETYPE == PAT_SEW ? ((g_patd.patmY - g_patd.patiY)) : (g_patd.patmY - g_patd.patiY),
                   5, 1, rect[RECT_MOIMAGE_YSIZE_STR].colorT[0]);
        Draw24X24FStr(rect[RECT_MOIMAGE_YSIZE_STR].xpis + rect[RECT_MOIMAGE_YSIZE_STR].xsize - SAFE_DIST10, rect[RECT_MOIMAGE_YSIZE_STR].ypis + (rect[RECT_MOIMAGE_YSIZE_STR].ysize - 24) / 2,
                      "mm", rect[RECT_MOIMAGE_YSIZE_STR].colorT[0]);

        //����������ʾ
        for (i = 2; i < KEY_MOIMAGE_MAX1; i++)
        {
            if (i == KEY_MOIMAGE_IMAGETOUCH)
            {
                DrawButton3(keyd[i], keybmpind[i].icon[0], 0);
            }
            else if (i == KEY_MOIMAGE_CODE)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0],
                           (chViewType == MOIMAGE_SKIPJOG) ? keybmpind[i].icon[3] : keybmpind[i].icon[0]);
            }
            else //���水ť
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
            }
        }
    }
    if (chViewType == MOIMAGE_VIEW)
    {
        for (i = RECT_MOIMAGE_MAX1 + RECT_MOIMAGE_MAX2; i < RECT_MOIMAGE_MAX1 + RECT_MOIMAGE_MAX2 + RECT_MOIMAGE_MAX3; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
        }
        DrawF16Str(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST3, rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2,
                   "x+:", rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawF16Str(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST3, rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2 * 2 + 16,
                   "x-:", rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawF16Str(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST3, rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2 * 3 + 16 * 2,
                   "y+:", rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawF16Str(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST3, rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2 * 4 + 16 * 3,
                   "y-:", rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawFS32P(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST4 + 9 * 3,
                  rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2,
                  g_patd.patmX, 5, 1, rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawFS32P(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST4 + 9 * 3,
                  rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2 * 2 + 16,
                  g_patd.patiX, 5, 1, rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawFS32P(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST4 + 9 * 3,
                  rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2 * 3 + 16 * 2,
                  g_patd.patmY, 5, 1, rect[RECT_MOIMAGE_SIZE].colorT[0]);
        DrawFS32P(rect[RECT_MOIMAGE_SIZE].xpis + SAFE_DIST4 + 9 * 3,
                  rect[RECT_MOIMAGE_SIZE].ypis + SAFE_DIST2 * 4 + 16 * 3,
                  g_patd.patiY, 5, 1, rect[RECT_MOIMAGE_SIZE].colorT[0]);
    }
    else if (chViewType == MOIMAGE_SKIPJOG)
    {
        //��������
        for (i = RECT_MOIMAGE_MAX1; i < RECT_MOIMAGE_MAX1 + RECT_MOIMAGE_MAX2; i++)
        {
            DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
            if (i == RECT_MOIMAGE_PININPUT)
            {
                DrawDottedRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
            }
        }
        //����������ʾ
        for (i = KEY_MOIMAGE_MAX1; i < KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2; i++)
        {

            if (i >= KEY_MOIMAGE_PP && i <= KEY_MOIMAGE_PAT)
            {
                U16 chJogMethodTemp = i - KEY_MOIMAGE_PP;
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           (g_jogmethod == chJogMethodTemp) ? keybmpind[i].border[1] : keybmpind[i].border[0],
                           (g_jogmethod == chJogMethodTemp) ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
            }

            else //���水ť
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
            }
        }
        //���·��һ�����Ϣ�� //������//�����
        DrawFillRect(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 2,
                     rect[RECT_MOIMAGE_PINDATA].xsize, TYPE_SIZE5, rect[RECT_MOIMAGE_PINDATA].colorB[0]);
        DrawMFU32(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 2 + SAFE_DIST4,
                  g_patd.feed_pat, 5, rect[RECT_MOIMAGE_PINDATA].colorT[0]);
        //���·��һ�����Ϣ�� //������//�����
        DrawFillRect(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 3,
                     rect[RECT_MOIMAGE_PINDATA].xsize, TYPE_SIZE5, rect[RECT_MOIMAGE_PINDATA].colorB[0]);
        DrawMFU32(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 3 + SAFE_DIST4,
                  g_patd.feed_sum, 5, rect[RECT_MOIMAGE_PINDATA].colorT[0]); //������

        DrawF16Str(rect[RECT_MOIMAGE_PININPUT].xpis + SAFE_DIST8,
                   rect[RECT_MOIMAGE_PININPUT].ypis + rect[RECT_MOIMAGE_PININPUT].ysize + SAFE_DIST4,
                   "(", rect[RECT_MOIMAGE_PININPUT].colorT[0]);
        DrawFU32(rect[RECT_MOIMAGE_PININPUT].xpis + SAFE_DIST8 + SAFE_DIST1,
                 rect[RECT_MOIMAGE_PININPUT].ypis + rect[RECT_MOIMAGE_PININPUT].ysize + SAFE_DIST4,
                 g_min_feedsum, 5, rect[RECT_MOIMAGE_PININPUT].colorT[0]);
        DrawF16Str(rect[RECT_MOIMAGE_PININPUT].xpis + SAFE_DIST8 + SAFE_DIST1 * 6,
                   rect[RECT_MOIMAGE_PININPUT].ypis + rect[RECT_MOIMAGE_PININPUT].ysize + SAFE_DIST4,
                   "-", rect[RECT_MOIMAGE_PININPUT].colorT[0]);
        DrawFU32(rect[RECT_MOIMAGE_PININPUT].xpis + SAFE_DIST8 + SAFE_DIST1 * 7,
                 rect[RECT_MOIMAGE_PININPUT].ypis + rect[RECT_MOIMAGE_PININPUT].ysize + SAFE_DIST4,
                 g_max_feedsum, 5, rect[RECT_MOIMAGE_PININPUT].colorT[0]);
        DrawF16Str(rect[RECT_MOIMAGE_PININPUT].xpis + SAFE_DIST8 + SAFE_DIST1 * 12,
                   rect[RECT_MOIMAGE_PININPUT].ypis + rect[RECT_MOIMAGE_PININPUT].ysize + SAFE_DIST4,
                   ")", rect[RECT_MOIMAGE_PININPUT].colorT[0]);
    }

    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
    f = Read_D(D_Address[SYS] + SYS_PFOOTNOW, 1, (U16 *)(&YREG[SYS_PFOOTNOW]));
    if (f == 0)
    {
        pno = MENU_RWHELP;
        hleppara = 15;
        return;
    }
    temp_PfVal = YREG[SYS_PFOOTNOW];
    old_PfootOffVal = temp_PfVal;
    if (g_jog_point == 0)
    {
        EnUPflag = 1;
    }
    // tmuly = (F_FILETYPE == PAT_SEW) ? -1 : 1;
    // xtmp = IMAGE_X7 * 100 / ((g_maxX - g_minX) < 2 ? 2 : (g_maxX - g_minX));
    // ytmp = IMAGE_Y7 * 100 / ((g_maxY - g_minY) < 2 ? 2 : (g_maxY - g_minY));

    // if (xtmp > ytmp)
    //     tscale = ytmp * 10;
    // else
    //     tscale = xtmp * 10;

    ViewPage(usepageno); //������ʾ����

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {

        if (pinsetf == 0)
        {
            if (chViewType == MOIMAGE_VIEW)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_MOIMAGE_MAX1, 0); //��ȡ��ֵ
            }
            else
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2, 0); //��ȡ��ֵ
            }
        }
        else
        {
            keydata = get_keydata((KEYPAD *)keyd, 2, 0); //��ȡ��ֵ
            if (keydata == 0)
            {
                keydata = get_keydata((KEYPAD *)keyd, KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2 + KEY_MOIMAGE_MAX3, KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2); //��ȡ��ֵ
            }
        }

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon == KEY_MOIMAGE_ZOOMOUT) || (keyon == KEY_MOIMAGE_ZOOMIN) ||
                (keyon == KEY_MOIMAGE_MUP) || (keyon == KEY_MOIMAGE_MDOWN) ||
                (keyon == KEY_MOIMAGE_IMAGETOUCH) || (keyon == KEY_MOIMAGE_ADD) || (keyon == KEY_MOIMAGE_DES))
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
            {
                keyon = keydata;
                if (keyon == KEY_MOIMAGE_SCRNSVR || keyon == KEY_MOIMAGE_IMAGETOUCH) //�ް�����Ч
                {
                }
                else if (keyon >= KEY_MOIMAGE_PP && keyon <= KEY_MOIMAGE_PAT) //�ǳ��水��
                {
                    U16 chJogMethodTemp = keyon - KEY_MOIMAGE_PP;
                    DrawButtonOn(keyd[keyon], (g_jogmethod == chJogMethodTemp) ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
                }
                else if (keyon == KEY_MOIMAGE_CODE)
                {
                    if (chViewType == MOIMAGE_VIEW)
                    {
                        DrawButtonOn(keyd[keyon], (g_CodeEn) ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
                    }
                    else
                    {
                        DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                    }
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //������������
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
                {
                    beeponf = 1;

                    if (g_keyBellT) //����ʱ����������ʱ��
                        Beep_On(g_keyBellT);

                    switch (keyon) //�������±�����ʾ����ʾ
                    {
                    case KEY_MOIMAGE_0:
                    case KEY_MOIMAGE_1:
                    case KEY_MOIMAGE_2:
                    case KEY_MOIMAGE_3:
                    case KEY_MOIMAGE_4:
                    case KEY_MOIMAGE_5:
                    case KEY_MOIMAGE_6:
                    case KEY_MOIMAGE_7:
                    case KEY_MOIMAGE_8:
                    case KEY_MOIMAGE_9:
                        BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4);
                        DrawMFU32(TITLE_XPOS_C2, SAFE_DIST4, keyon - KEY_MOIMAGE_0, 1, rect[0].colorT[0]);
                        break;
                    case KEY_MOIMAGE_CODE:
                        DrawFMenuStr((chViewType == MOIMAGE_SKIPJOG) ? keybmpind[keyon].titleStr[3] : keybmpind[keyon].titleStr[0]);
                        break;
                    default:
                        DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                        break;
                    }

                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                    if (((keyon == KEY_MOIMAGE_MUP) || (keyon == KEY_MOIMAGE_MDOWN)) && (F_FILETYPE == PAT_MIT))
                        delay = DELAY_JOG1;
                    else if (((keyon == KEY_MOIMAGE_MUP) || (keyon == KEY_MOIMAGE_MDOWN)) && (F_FILETYPE == PAT_SEW))
                        delay = DELAY_JOG2;
                    else if ((keyon == KEY_MOIMAGE_ZOOMOUT) || (keyon == KEY_MOIMAGE_ZOOMIN) ||
                             (keyon == KEY_MOIMAGE_ADD) || (keyon == KEY_MOIMAGE_DES))
                    {
                        delay = KEYDT;
                    }
                    else
                        delay = KEYNT;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }

                if ((keyon == KEY_MOIMAGE_ZOOMOUT) || (keyon == KEY_MOIMAGE_ZOOMIN) ||
                    (keyon == KEY_MOIMAGE_MUP) || (keyon == KEY_MOIMAGE_MDOWN) ||
                    (keyon == KEY_MOIMAGE_IMAGETOUCH) || (keyon == KEY_MOIMAGE_ADD) || (keyon == KEY_MOIMAGE_DES)) //���г����������ܰ����Ĺ���
                {
                    keyoff = keyon;
                    key_on_flag1 = 1;
                }
                Times = 0; //��ǰ
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {
            if (keyoff >= KEY_MOIMAGE_0 && keyoff <= KEY_MOIMAGE_9)
            {
                PinSets = (PinSets % moddat) * 10 + keyoff - KEY_MOIMAGE_0;
                if (PinSets > g_max_feedsum)
                    PinSets %= moddat;
                dis_updata5 = 1;
            }
            else
            {
                if (g_needtohome && ((keyoff == KEY_MOIMAGE_BACK) || (keyoff == KEY_MOIMAGE_LOCATION) || (keyoff == KEY_MOIMAGE_MUP) || (keyoff == KEY_MOIMAGE_MDOWN)))
                {
                    pno = MENU_RWHELP;
                    hleppara = 158;
                    poldno[pnoindex] = MENU_MOIMAGE;
                    pnoindex++;
                }
                else
                {
                    switch (keyoff)
                    {
                    case KEY_MOIMAGE_SCRNSVR:
                        pno = MENU_SCRNSVR;
                        break;
                    case KEY_MOIMAGE_CODE:
                        if (chViewType == MOIMAGE_SKIPJOG)
                        {
                            f = Go_Home(MENU_MOIMAGE, 1); //0
                            if (f == 0)
                                break;
                            EnUPflag = 1;

                            isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
                            if ((isHome & 0x02) || (isHome & 0x04))
                            {
                            }
                            else
                            {                     //û�лص���еԭ��λ��
                                g_needtohome = 1; //�綯ǰ��Ҫ��һ��ԭ��
                                break;
                            }
                            g_needtohome = 0;
                            if (F_FILETYPE == PAT_MIT)
                            { //����
                                g_jog_point = 0;
                                Up = 0;
                                Dn = 1;
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            { //�ֵ�
                                g_BorSTflag = 0;
                                g_jog_point = 0;
                                Up = 0;
                                Dn = 1;
                            }
                            g_ExCode = 0;
                            g_curExCode = 0;
                            g_curPinAng = 0;
                            g_curSetPara1 = 0;
                            g_curSetPara2 = 0;
                            g_pointPinAng = 0;
                            g_pointSetPara1 = 0;
                            g_pointSetPara2 = 0;
                            dis_updata = 1;

                            tmpfeed = g_jog_point;
                            dis_updata3 = 1;
                        }
                        else
                        {
                            if (g_CodeEn == 1)
                                g_CodeEn = 0;
                            else
                                g_CodeEn = 1;
                            dis_updata = 1;
                        }
                        break;
                    case KEY_MOIMAGE_ENTER:
                        if (tmpfeed <= g_max_feedsum && tmpfeed >= g_min_feedsum)
                        {
                            if (tmpfeed != g_jog_point)
                            {
                                goto LOCATION;
                            }
                        }
                    case KEY_MOIMAGE_BACK:
                        if (chViewType != MOIMAGE_VIEW)
                        {
                            if (g_IsLocal == 1)
                            {
                                Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����

                                f = Write_Cmd2(C_SETPINS, g_jog_point, 2, 3);
                                if (f == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    poldno[pnoindex] = MENU_MOIMAGE;
                                    pnoindex++;
                                    break;
                                }

                                if (Read1_Status(50) == 0)
                                {
                                    pno = MENU_RWHELP;
                                    hleppara = 15;
                                    poldno[pnoindex] = MENU_MOIMAGE;
                                    pnoindex++;
                                    break;
                                }
                                else if (DREG[D_CMDSTATUS] != 100)
                                {
                                    Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                                    if (DREG[D_ERRWAR] && (g_errLocal == 0))
                                    {
                                        pno = MENU_RWHELP;
                                        hleppara = 21;
                                        poldno[pnoindex] = MENU_MOIMAGE;
                                        pnoindex++;
                                        g_errLocal++;
                                        break;
                                    }
                                    pno = MENU_RWHELP;
                                    hleppara = 16;
                                    pnoindex = 1;
                                    poldno[0] = MENU_P1;
                                    g_errLocal = 0;
                                    E_RUNDIS();
                                    Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
                                    break;
                                }
                                E_RUNDIS();
                                Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
                            }
                            if (para == 201 && g_selectmode == 2)
                            { //Բ��
                                g_point_num--;
                            }
                        }

                        pnoindex--;
                        pno = poldno[pnoindex];
                        if ((g_jog_enpoint > g_jog_stpoint) && (para == 200))
                            g_jog_enpoint = g_jog_stpoint;

                        g_CodeEn = 1;
                        g_imgpara = 0;
                        g_ImgPoint = 0;
                        g_ImgMove = 0;
                        g_IsLocal = 0;

                        break;

                    case KEY_MOIMAGE_PINS:
                        if (pinsetf == 0)
                        {
                            pinsetf = 1;
                            PinSets = tmpfeed;
                        }
                        break;
                    case KEY_MOIMAGE_LOCATION:
                    LOCATION:
                        if (pinsetf == 1)
                            break;

                        if ((!(S_CLAMP)) && (P_OST == 0))
                        {
                            pno = MENU_RWHELP;
                            hleppara = 13;
                            poldno[pnoindex] = MENU_MOIMAGE;
                            pnoindex++;
                            break;
                        }
                        g_errLocal = 0;

                        if (tmpfeed != g_jog_point)
                        {
                            if (runf1)
                                runf1 = 0;
                            if (runf2)
                                runf2 = 0;
                            if (F_FILETYPE == PAT_MIT)
                            { //����
                                if (tmpfeed > g_jog_point)
                                    runf1 = tmpfeed;
                                else
                                    runf2 = tmpfeed;
                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }
                                if (g_jogmethod == 0)
                                { //ѭֱ���ƶ�
                                    wdt();
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (runf1)
                                    { //ǰ��
                                        dat1 = g_patd.pat_add + g_jog_point * 2;
                                        g_dxpos = 0;
                                        g_dypos = 0;
#if PATRULES_ORG_OFFSET
                                        if (SREG[3] == 7) //����-�޸�ԭ��λ��
                                        {
                                            g_dxpos = (S16)(SREG[6]);
                                            g_dypos = (S16)(SREG[7]);
                                        }
#endif
                                        excode = 0;
                                        g_tExCode = g_ExCode;
                                        for (ii = g_jog_point; ii < runf1; ii++)
                                        {
                                            dat = SREG[dat1] & 0x00ff;
                                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                            {
                                                dat = SREG[dat1 + 1];
                                                x = dat & 0x000000ff;
                                                y = ((U32)(dat >> 8)) & 0x000000ff;
                                                if (x >= 0x80)
                                                {
                                                    x -= 0x80;
                                                    g_dxpos -= x;
                                                }
                                                else
                                                {
                                                    g_dxpos += x;
                                                }
                                                if (y >= 0x80)
                                                {
                                                    y -= 0x80;
                                                    g_dypos -= y;
                                                }
                                                else
                                                {
                                                    g_dypos += y;
                                                }
                                            }
                                            else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                            { //���ӹ���
                                                excode = 0;
                                                if (dat == 0x05)
                                                    excode = 0x01; //����1
                                                else if (dat == 0x0b)
                                                    excode = 0x02; //����2
                                                else if (dat == 0x0c)
                                                    excode = 0x04; //����3
                                                else if (dat == 0x0d)
                                                    excode = 0x08; //����4
                                                else
                                                { //����5-8
                                                    excode = (SREG[dat1] & 0xff00) >> 8;
                                                    if (excode > 3)
                                                        excode = 0;
                                                    excode = (1 << (excode + 4));
                                                }
                                                if (excode != 0)
                                                {
                                                    g_tExCode ^= excode;
                                                }
                                            }
                                            else if (dat == 0x3c)
                                            {
                                                g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                                            }
                                            else if (dat == 0x3d)
                                            {
                                                g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                                            }
                                            else if (dat == 0x3e)
                                            {
                                                g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                                            }
                                            dat1 += 2;
                                        }
                                        if (g_modcordflag)
                                        {
                                            f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MOIMAGE, 0, 1);
                                        }
                                        else
                                        {
                                            f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 0, 1);
                                        }
                                        g_modcordflag = 0;

                                        movfeed = 0;
                                        if (f == 0)
                                        {
                                            break;
                                        }
                                        excode = g_ExCode ^ g_tExCode;
                                        if (excode != 0)
                                        {
                                            OutputExcode1(excode, MENU_MOIMAGE, 0);
                                        }
                                        sendExCodeCmd();
                                        g_jog_point = runf1;
                                        runf1 = 0;
                                        g_temp_oxpos += (S16)(g_dxpos);
                                        g_temp_oypos += (S16)(g_dypos);

                                        dis_updata = 1;
                                        // dis_updata6 = 1;
                                    }
                                    else if (runf2)
                                    { //����
                                        dat1 = g_patd.pat_add + g_jog_point * 2;
                                        g_dxpos = 0;
                                        g_dypos = 0;
                                        excode = 0;
                                        g_tExCode = g_ExCode;
                                        for (ii = g_jog_point; ii > runf2; ii--)
                                        {
                                            dat = SREG[dat1 - 2] & 0x00ff;
                                            if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                            {
                                                dat = SREG[dat1 - 1];
                                                x = dat & 0x000000ff;
                                                y = ((U32)(dat >> 8)) & 0x000000ff;
                                                if (x >= 0x80)
                                                {
                                                    x -= 0x80;
                                                    g_dxpos += x;
                                                }
                                                else
                                                {
                                                    g_dxpos -= x;
                                                }
                                                if (y >= 0x80)
                                                {
                                                    y -= 0x80;
                                                    g_dypos += y;
                                                }
                                                else
                                                {
                                                    g_dypos -= y;
                                                }
                                            }
                                            else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                            { //���ӹ���
                                                excode = 0;
                                                if (dat == 0x05)
                                                    excode = 0x01; //����1
                                                else if (dat == 0x0b)
                                                    excode = 0x02; //����2
                                                else if (dat == 0x0c)
                                                    excode = 0x04; //����3
                                                else if (dat == 0x0d)
                                                    excode = 0x08; //����4
                                                else
                                                { //����5-8
                                                    excode = (SREG[dat1 - 2] & 0xff00) >> 8;
                                                    if (excode > 3)
                                                        excode = 0;
                                                    excode = (1 << (excode + 4));
                                                }
                                                if (excode != 0)
                                                {
                                                    g_tExCode ^= excode;
                                                }
                                            }

                                            dat1 -= 2;
                                        }

                                        if (g_modcordflag)
                                        {
                                            f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MOIMAGE, 0, 1);
                                        }
                                        else
                                        {
                                            f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 0, 1);
                                        }
                                        g_modcordflag = 0;
                                        movfeed = 0;
                                        if (f == 0)
                                        {
                                            break;
                                        }

                                        excode = g_ExCode ^ g_tExCode;
                                        if (excode != 0)
                                        {
                                            OutputExcode1(excode, MENU_MOIMAGE, 0);
                                        }

                                        g_temp_oxpos += (S16)(g_dxpos);
                                        g_temp_oypos += (S16)(g_dypos);

                                        g_jog_point = runf2;
                                        g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 0);
                                        g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 1);
                                        g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + g_jog_point * 2, 2);
                                        sendExCodeCmd();

                                        runf2 = 0;
                                        dis_updata = 1;
                                        // dis_updata6 = 1;
                                    }

                                    g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                                    g_pf_absval += g_pfootd.base;
                                    if (g_pf_absval != old_PfootOffVal)
                                    {
                                        temp_PfVal = g_pf_absval;
                                        old_PfootOffVal = g_pf_absval;
                                        f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0); //������ѹ�Ÿ߶�
                                        Read_Status();
                                        pfootval = 0;
                                        Up = 0;
                                        Dn = 1;
                                        EnUPflag = 1;
                                        uplock = 1;
                                    }
                                }
                            }
                            else if (F_FILETYPE == PAT_SEW)
                            { //�ֵ�

                                if (Up == 1)
                                {
                                    Up = 0;
                                    Dn = 1;
                                    Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    pfootf = 0xff;
                                }

                                if (g_jogmethod == 0)
                                { //ѭֱ���ƶ�
                                    runf1 = tmpfeed;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    {
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                    }
                                    if (g_BorSTflag == 0)
                                    { //��ԭ��λ��
                                        temp_oxpos1 = 0;
                                        temp_oypos1 = 0;
                                    }
                                    else
                                    {
                                        Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&tempdx, (S16 *)&tempdy, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                        temp_oxpos1 += tempdx;
                                        temp_oypos1 += tempdy;
                                    }

                                    Bor_feedInfo(runf1, g_patd.pat_len, &Borcmd, &temp_oxpos2, &temp_oypos2, (S16 *)&tempdx, (S16 *)&tempdy, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                    temp_oxpos2 += tempdx;
                                    temp_oypos2 += tempdy;

                                    f = Comm_RelationMove(temp_oxpos2 - temp_oxpos1, temp_oypos2 - temp_oypos1, MENU_MOIMAGE, 1, 1);
                                    if (f == 0)
                                    {
                                        break;
                                    }

                                    excode = g_ExCode ^ g_tExCode;

                                    if (excode != 0)
                                    {
                                        OutputExcode1(excode, MENU_MOIMAGE, 0);
                                    }
                                    Borpooft += g_pfootd.base;
                                    if (Borpooft > PF_ABS_MAX)
                                        Borpooft = PF_ABS_MAX;
                                    if (Borpooft < PF_ABS_MIN)
                                        Borpooft = PF_ABS_MIN;
                                    f = Write_Cmd(C_PFOOTACTION, Borpooft, 0); //������ѹ�Ÿ߶�
                                    Read_Status();

                                    sendExCodeCmd();

                                    g_BorSTflag = 2;
                                    g_jog_point = runf1;
                                    runf1 = 0;
                                    dis_updata = 1;
                                    // dis_updata6 = 1;
                                }
                                else
                                { //ѭ�����켣
                                    if (g_BorSTflag == 0)
                                    { //��ԭ��λ�� ���ƶ�����ԭ��λ��
                                        g_temp_oxpos = g_patd.orgX;
                                        g_temp_oypos = g_patd.orgY;

                                        if (GoHome == 0)
                                        {                                                                      //XYͬʱ�ƶ�
                                            Comm_RelationMove(g_temp_oxpos, g_temp_oypos, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                        }
                                        else if (GoHome == 1)
                                        {                                                           //X�Ȼ�ԭ��,Y���ԭ��
                                            Comm_RelationMove(0, g_temp_oypos, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                            Wait_Move_End();                                        //�ȴ�ƽ̨�ƶ�����
                                            Comm_RelationMove(g_temp_oxpos, 0, MENU_MOIMAGE, 1, 1);
                                        }
                                        else
                                        {
                                            Comm_RelationMove(g_temp_oxpos, 0, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                            Wait_Move_End();                                        //�ȴ�ƽ̨�ƶ�����
                                            Comm_RelationMove(0, g_temp_oypos, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                        }
                                        if (g_patd.orgX != 0 || g_patd.orgY != 0)
                                        { //��ԭ��
                                            g_BorSTflag = 1;
                                        }
                                        else
                                        { //�޴�ԭ��
                                            g_BorSTflag = 2;
                                        }
                                    }

                                    if (tmpfeed > g_jog_point)
                                        runf1 = tmpfeed;
                                    else
                                        runf2 = tmpfeed;
                                }
                            }
                        }
                        break;
                    case KEY_MOIMAGE_CANCLE:
                        // dis_updata = 1;
                        pinsetf = 0;
                        PinSets = tmpfeed;
                        dis_updata3 = 1;
                        break;
                    case KEY_MOIMAGE_OK:
                        tmpfeed = PinSets;
                        dis_updata = 1;
                        pinsetf = 0;
                        dis_updata3 = 1;
                        break;
                    case KEY_MOIMAGE_CLR:
                        if (PinSets)
                        {
                            PinSets /= 10;
                            dis_updata5 = 1;
                        }
                        break;

                    case KEY_MOIMAGE_ADD:
                        if (PinSets < g_max_feedsum)
                        {
                            PinSets++;
                            dis_updata5 = 1;
                        }
                        break;

                    case KEY_MOIMAGE_DES:
                        if (PinSets > g_min_feedsum)
                        {
                            PinSets--;
                            dis_updata5 = 1;
                        }
                        break;

                    case KEY_MOIMAGE_ZOOMOUT:
                        if (Scale)
                        {
                            Scale -= 50;
                            dis_updata = 2;
                        }
                        else
                        {
                            if (bResetCaonvas)
                            {
                                bResetCaonvas = 0;
                                Scale = 0;
                                xo = patConvas.conOrgX;
                                yo = patConvas.conOrgY;
                                okeyxpct = keyxpct;
                                okeyypct = keyypct;
                                dis_updata = 2;
                            }
                        }
                        break;
                    case KEY_MOIMAGE_ZOOMIN:
                        Scale += 50;
                        dis_updata = 2;
                        bResetCaonvas = 1;
                        break;
                    case KEY_MOIMAGE_PP:
                        if (g_jogmethod)
                        {
                            g_jogmethod = 0;
                            dis_updata2 = 1;
                            WriteJogmethod(g_jogmethod);
                        }
                        break;

                    case KEY_MOIMAGE_PAT:
                        if (g_jogmethod == 0)
                        {
                            g_jogmethod = 1;
                            dis_updata2 = 1;
                            WriteJogmethod(g_jogmethod);
                        }
                        break;
                    case KEY_MOIMAGE_MUP:
                        if (pinsetf == 1)
                        {
                            pinsetf = 0;
                            dis_updata = 1;
                            break;
                        }

                        if ((!(S_CLAMP)) && (P_OST == 0))
                        {
                            pno = MENU_RWHELP;
                            hleppara = 13;
                            poldno[pnoindex] = MENU_MOIMAGE;
                            pnoindex++;
                            break;
                        }
                        g_errLocal = 0;
                        if (F_FILETYPE == PAT_MIT)
                        { //����
                            if (g_jog_point < g_max_feedsum)
                            {
                                dat1 = g_patd.pat_add + g_jog_point * 2;
                                if (uplock == 1)
                                {
                                    dat = SREG[dat1] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1))
                                    {
                                        if (((SREG[dat1] & 0xff00) != 0))
                                        {
                                            pfootval = (S8)(SREG[dat1] >> 8);
                                            temp_PfVal = g_pfootd.base + Mit_CailPadOffestPF(g_jog_point, 0, 0) + pfootval;
                                            //#if Debug
                                            //printf("pfootval = %d \r\n",pfootval);
                                            //#endif
                                        }
                                    }
                                    else if ((dat == 0x1b) || (dat == 0x03))
                                    { //����
                                        EnUPflag = 1;
                                        if (Up)
                                        {
                                            Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            Up = 0;
                                            Dn = 1;
                                            break;
                                        }
                                    }
                                    g_temp_PfootOffVal = temp_PfVal;
                                    uplock = 0;
                                }

                                if (((SREG[dat1] & 0x00ff) == 0x1) || ((SREG[dat1] & 0x00ff) == 0x21) || ((SREG[dat1] & 0x00ff) == 0x41) || ((SREG[dat1] & 0x00ff) == 0x61))
                                { //����
                                    if ((old_PfootOffVal != temp_PfVal) || Dn)
                                    {
                                        if ((Dn == 1) && ((EnUPflag == 1) || (old_PfootOffVal != temp_PfVal)))
                                        {
                                            Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            Up = 1;
                                            Dn = 0;
                                            break;
                                        }
                                        if (old_PfootOffVal != temp_PfVal)
                                        {
                                            old_PfootOffVal = temp_PfVal;
                                            //#if Debug
                                            //printf("pfootval = %d \r\n",pfootval);
                                            //#endif
                                            if (pfootval > 0)
                                            {
                                                f = Write_Cmd(C_PFEMOV, 1, pfootval);
                                            }
                                            else
                                            {
                                                f = Write_Cmd(C_PFEMOV, 2, (-pfootval));
                                            }
                                            Read_Status();
                                            //for(i=0;i<RDTIMES;i++){
                                            //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                                            //	if((DREG[D_CMDSTATUS]) == 100)break;
                                            //	DelayMs(WAITTIME);
                                            //}
                                        }
                                    }
                                    EnUPflag = 0;
                                }
                                else if (((SREG[dat1] & 0x00ff) == 0x03) || ((SREG[dat1] & 0x00ff) == 0x1b))
                                { //����
                                    EnUPflag = 1;
                                    if (Up)
                                    {
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                    }
                                    Up = 0;
                                    Dn = 1;
                                }
                                else if (((SREG[dat1] & 0x00ff) == 0x07))
                                { //����
                                    EnUPflag = 1;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    { //ѹ��������,ѹ����
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                    }
                                    else
                                    {                                   //ѹ��������,��ѹ�ŷ�����̧����
                                        Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        Up = 0;
                                        Dn = 1;
                                        pfootf = 0xff;
                                    }
                                }

                                dat = SREG[dat1] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                {
                                    if ((dat == 0x1b) || (dat == 0x3))
                                    {
                                        movfeed = 1;
                                    }
                                    dat = SREG[dat1 + 1];
                                    x = dat & 0x000000ff;
                                    y = ((U32)(dat >> 8)) & 0x000000ff;
                                    if (x >= 0x80)
                                    {
                                        x -= 0x80;
                                        g_dxpos = -x;
                                    }
                                    else
                                    {
                                        g_dxpos = x;
                                    }
                                    if (y >= 0x80)
                                    {
                                        y -= 0x80;
                                        g_dypos = -y;
                                    }
                                    else
                                    {
                                        g_dypos = y;
                                    }
                                    if (g_modcordflag)
                                    {
                                        f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MOIMAGE, 0, movfeed);
                                    }
                                    else
                                    {
                                        f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 0, movfeed);
                                    }
                                    g_modcordflag = 0;
                                    //f = Comm_RelationMove(g_dxpos,g_dypos,MENU_MOIMAGE,0,movfeed);
                                    movfeed = 0;
                                    if (f == 0)
                                    {
                                        break;
                                    }
                                }
                                else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                { //���ӹ���
                                    excode = 0;
                                    if (dat == 0x05)
                                        excode = 0x01; //����1
                                    else if (dat == 0x0b)
                                        excode = 0x02; //����2
                                    else if (dat == 0x0c)
                                        excode = 0x04; //����3
                                    else if (dat == 0x0d)
                                        excode = 0x08; //����4
                                    else
                                    { //����5-8
                                        excode = (SREG[dat1] & 0xff00) >> 8;
                                        if (excode > 3)
                                            excode = 0;
                                        excode = (1 << (excode + 4));
                                    }
                                    if (excode != 0)
                                    {
                                        OutputExcode1(excode, MENU_MOIMAGE, 0);
                                    }
                                }
                                else if (dat == 0x3c)
                                {
                                    g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                                }
                                else if (dat == 0x3d)
                                {
                                    g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                                }
                                else if (dat == 0x3e)
                                {
                                    g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                                }
                                sendExCodeCmd();

                                g_jog_point++;
                                g_temp_oxpos += (S16)(g_dxpos);
                                g_temp_oypos += (S16)(g_dypos);
                                //	dis_updata =1;
                                dis_updata1 = 1;
                                // dis_updata6 = 1;

                                if (g_jog_point < g_max_feedsum)
                                {
                                    dat = SREG[dat1 + 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                    {
                                        if ((SREG[dat1 + 2] & 0xff00) != 0)
                                        {
                                            if (Up == 0)
                                            {
                                                pfootval = (S8)(SREG[dat1 + 2] >> 8); //����jog_point֮ǰ����ѹ�Ÿ߶ȱ仯��
                                                temp_PfVal = g_pfootd.base + Mit_CailPadOffestPF(g_jog_point, 0, 0) + pfootval;
                                            }
                                            else
                                            {
                                                pfootval = (S8)(SREG[dat1 + 2] >> 8);
                                                temp_PfVal += pfootval;
                                            }
                                            g_temp_PfootOffVal = temp_PfVal;
                                        }
                                    }
                                }
                            }

                            g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                            g_pf_Set_Offest = 0;
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        { //�ֵ�
                            if (g_jog_point < g_max_feedsum)
                            {

                                if (g_BorSTflag == 0)
                                { //��ԭ��λ��
                                    g_ExCode = 0;
                                    g_curExCode = 0;
                                    g_curPinAng = 0;
                                    g_curSetPara1 = 0;
                                    g_curSetPara2 = 0;
                                    if (Up == 1)
                                    {
                                        Up = 0;
                                        Dn = 1;
                                        EnUPflag = 1;
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                    }
                                    g_temp_oxpos = g_patd.orgX;
                                    g_temp_oypos = g_patd.orgY;

                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (GoHome == 0)
                                    {                                                                      //XYͬʱ�ƶ�
                                        Comm_RelationMove(g_temp_oxpos, g_temp_oypos, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                    }
                                    else if (GoHome == 1)
                                    {                                                           //X�Ȼ�ԭ��,Y���ԭ��
                                        Comm_RelationMove(0, g_temp_oypos, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                        Wait_Move_End();                                        //�ȴ�ƽ̨�ƶ�����
                                        Comm_RelationMove(g_temp_oxpos, 0, MENU_MOIMAGE, 1, 1);
                                    }
                                    else
                                    {
                                        Comm_RelationMove(g_temp_oxpos, 0, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                        Wait_Move_End();                                        //�ȴ�ƽ̨�ƶ�����
                                        Comm_RelationMove(0, g_temp_oypos, MENU_MOIMAGE, 1, 1); //�ƶ�����ԭ��λ��
                                    }
                                    if (g_patd.orgX != 0 || g_patd.orgY != 0)
                                    { //��ԭ��
                                        g_BorSTflag = 1;
                                    }
                                    else
                                    { //�޴�ԭ��
                                        g_BorSTflag = 2;
                                    }
                                }
                                else if (g_BorSTflag == 1)
                                { //�ڴ�ԭ��λ��
                                    g_BorSTflag = 2;
                                }
                                else
                                {
                                    Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                    Borpooft += g_pfootd.base;
                                    if (Borpooft > PF_ABS_MAX)
                                        Borpooft = PF_ABS_MAX;
                                    if (Borpooft < PF_ABS_MIN)
                                        Borpooft = PF_ABS_MIN;

                                    if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                                    { //����,����
                                        EnUPflag = 1;
                                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                        if (Up == 1)
                                        {
                                            Up = 0;
                                            Dn = 1;
                                            Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            if ((Borcmd & 0xf000) == 0x2000)
                                            {
                                                break;
                                            }
                                        }
                                        if ((Borcmd & 0xf000) == 0x2000)
                                        {
                                            movfeed = 1;
                                        }
                                    }
                                    else if ((Borcmd & 0xf000) == 0x6000)
                                    { //����
                                        if ((Dn == 1) && (EnUPflag == 1))
                                        {
                                            Up = 1;
                                            Dn = 0;
                                            Wait_Move_End();                //�ȴ�ƽ̨�ƶ�����
                                            Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            break;
                                        }
                                        EnUPflag = 0;
                                    }
                                    else if ((Borcmd & 0xff00) == 0xf000)
                                    {                    //��ѹ�ŵ���
                                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                        if (Dn == 1)
                                        {
                                            Up = 1;
                                            Dn = 0;
                                            Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            break;
                                        }

                                        if (Borpfdis > 0)
                                        { //��ѹ������
                                            f = Write_Cmd(C_PFEMOV, 1, Borpfdis);
                                        }
                                        else if (Borpfdis < 0)
                                        { //��ѹ���½�
                                            f = Write_Cmd(C_PFEMOV, 2, -Borpfdis);
                                        }
                                    }
                                    else if ((Borcmd & 0xff00) == 0xf400)
                                    { //����
                                        EnUPflag = 1;
                                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                        if (Up == 1)
                                        { //ѹ��������,ѹ����
                                            Up = 0;
                                            Dn = 1;
                                            Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                        }
                                        else
                                        {                                   //ѹ��������,��ѹ�ŷ�����̧����
                                            Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                            Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            Up = 0;
                                            Dn = 1;
                                            pfootf = 0xff;
                                        }
                                    }
                                    else if ((Borcmd & 0xff00) == 0xfc00)
                                    { //���ӹ���
                                        excode = Borcmd & 0xff;
                                        if (excode != 0)
                                        {
                                            OutputExcode1(excode, MENU_MOIMAGE, 0);
                                        }
                                    }
                                    else if ((Borcmd & 0xff00) == 0xec00)
                                    { //��Ƕ�
                                    }
                                    else if ((Borcmd & 0xff00) == 0xea00)
                                    { //����һ
                                    }
                                    else if ((Borcmd & 0xff00) == 0xeb00)
                                    { //���ö�
                                    }

                                    sendExCodeCmd();
                                    g_jog_point++;
                                    Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 1, movfeed);
                                    movfeed = 0;
                                }

                                //								dis_updata =1;
                                dis_updata1 = 1;
                                // dis_updata6 = 1;
                            }
                        }

                        tmpfeed = g_jog_point;
                        dis_updata3 = 1;
                        break;
                    case KEY_MOIMAGE_MDOWN:
                        if (pinsetf == 1)
                        {
                            pinsetf = 0;
                            dis_updata = 1;
                            break;
                        }
                        if ((!(S_CLAMP)) && (P_OST == 0))
                        {
                            pno = MENU_RWHELP;
                            hleppara = 13;
                            poldno[pnoindex] = MENU_MOIMAGE;
                            pnoindex++;
                            break;
                        }

                        g_errLocal = 0;
                        if (F_FILETYPE == PAT_MIT)
                        { //����
                            g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                            if (g_jog_point > g_min_feedsum)
                            {
                                dat1 = g_patd.pat_add + g_jog_point * 2;
                                if (dat1 > g_patd.pat_add)
                                {
                                    dat = SREG[dat1 - 2] & 0x00ff;
                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1))
                                    {
                                        if ((SREG[dat1 - 2] & 0xff00) != 0)
                                        {
                                            pfootval = Mit_CailPadOffestPF(g_jog_point, 0, 0); //����jog_point֮ǰ����ѹ�Ÿ߶ȱ仯��
                                            temp_PfVal = g_pfootd.base + pfootval - (S8)(SREG[dat1 - 2] >> 8);
                                            pfootval = (S8)(SREG[dat1 - 2] >> 8);
                                        }
                                    }
                                    else if ((dat == 0x1b) || (dat == 0x03))
                                    { //����
                                        EnUPflag = 1;
                                        if (Up)
                                        {
                                            Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            Up = 0;
                                            Dn = 1;
                                            break;
                                        }
                                    }

                                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x03))
                                    {
                                        if ((SREG[dat1 - 2] & 0xff00) == 0)
                                        {
                                            g_temp_PfootOffVal = temp_PfVal;
                                            if (uplock == 0)
                                            {
                                                if (old_PfootOffVal != temp_PfVal)
                                                {
                                                    temp_PfVal = old_PfootOffVal;
                                                    g_temp_PfootOffVal = temp_PfVal;
                                                }
                                            }
                                        }
                                    }

                                    if (uplock == 0)
                                    {
                                        uplock = 1;
                                    }
                                }
                                if (((SREG[dat1 - 2] & 0x00ff) == 0x1) || ((SREG[dat1 - 2] & 0x00ff) == 0x21) || ((SREG[dat1 - 2] & 0x00ff) == 0x41) || ((SREG[dat1 - 2] & 0x00ff) == 0x61))
                                {
                                    if ((old_PfootOffVal != temp_PfVal) || Dn)
                                    {
                                        f = Write_Cmd(C_PFOOTACTION, old_PfootOffVal, 0);
                                        Read_Status();
                                        //for(i=0;i<RDTIMES;i++){
                                        //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                                        //	if((DREG[D_CMDSTATUS]) == 100)break;
                                        //	DelayMs(WAITTIME);
                                        //}
                                        if (Dn && ((old_PfootOffVal != temp_PfVal) || (EnUPflag == 1)))
                                        {
                                            Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                            Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                            pfootf = 0xff;
                                            Up = 1;
                                            Dn = 0;
                                            if (EnUPflag == 1)
                                                break;
                                        }

                                        if (old_PfootOffVal != temp_PfVal)
                                        {
                                            old_PfootOffVal = temp_PfVal;
                                            if (pfootval < 0)
                                            {
                                                f = Write_Cmd(C_PFEMOV, 1, (-pfootval));
                                            }
                                            else
                                            {
                                                f = Write_Cmd(C_PFEMOV, 2, pfootval);
                                            }
                                            Read_Status();
                                            //for(i=0;i<RDTIMES;i++){
                                            //	Read_D(D_Address[DISP]+D_CMDSTATUS,1,(U16 *)(&DREG[D_CMDSTATUS]));	// ��״̬
                                            //	if((DREG[D_CMDSTATUS]) == 100)break;
                                            //	DelayMs(WAITTIME);
                                            //}
                                        }
                                    }
                                    EnUPflag = 0;
                                }
                                else if (((SREG[dat1 - 2] & 0x00ff) == 0x07))
                                { //����
                                    EnUPflag = 1;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    { //ѹ��������,ѹ����
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                    }
                                    else
                                    {                                   //ѹ��������,��ѹ�ŷ�����̧����
                                        Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        Up = 0;
                                        Dn = 1;
                                        pfootf = 0xff;
                                    }
                                }

                                dat = SREG[dat1 - 2] & 0x00ff;
                                if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                                {
                                    if ((dat == 0x1b) || (dat == 0x3))
                                    {
                                        movfeed = 1;
                                    }
                                    dat = SREG[dat1 - 1];
                                    x = dat & 0x000000ff;
                                    y = ((U32)(dat >> 8)) & 0x000000ff;
                                    if (x >= 0x80)
                                    {
                                        x -= 0x80;
                                        g_dxpos = x;
                                    }
                                    else
                                    {
                                        g_dxpos = -x;
                                    }
                                    if (y >= 0x80)
                                    {
                                        y -= 0x80;
                                        g_dypos = y;
                                    }
                                    else
                                    {
                                        g_dypos = -y;
                                    }
                                    //f = Comm_RelationMove(g_dxpos,g_dypos,MENU_MOIMAGE,0,movfeed);
                                    if (g_modcordflag)
                                    {
                                        f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MOIMAGE, 0, movfeed);
                                    }
                                    else
                                    {
                                        f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 0, movfeed);
                                    }
                                    g_modcordflag = 0;
                                    movfeed = 0;
                                    if (f == 0)
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    g_dxpos = 0;
                                    g_dypos = 0;

                                    if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                                    { //���ӹ���
                                        excode = 0;
                                        if (dat == 0x05)
                                            excode = 0x01; //����1
                                        else if (dat == 0x0b)
                                            excode = 0x02; //����2
                                        else if (dat == 0x0c)
                                            excode = 0x04; //����3
                                        else if (dat == 0x0d)
                                            excode = 0x08; //����4
                                        else
                                        { //����5-8
                                            excode = (SREG[dat1 - 2] & 0xff00) >> 8;
                                            if (excode > 3)
                                                excode = 0;
                                            excode = (1 << (excode + 4));
                                        }
                                        if (excode != 0)
                                        {
                                            OutputExcode1(excode, MENU_MOIMAGE, 0);
                                        }
                                    }
                                    else if (dat == 0x3c)
                                    {
                                        g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 0);
                                    }
                                    else if (dat == 0x3d)
                                    {
                                        g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 1);
                                    }
                                    else if (dat == 0x3e)
                                    {
                                        g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 2);
                                    }
                                    sendExCodeCmd();
                                }

                                g_jog_point--;
                                g_temp_oxpos += (S16)(g_dxpos);
                                g_temp_oypos += (S16)(g_dypos);
                                //							dis_updata =1;
                                dis_updata1 = 1;
                                // dis_updata6 = 1;
                            }
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        { //�ֵ�
                            if (g_jog_point > g_min_feedsum)
                            {
                                Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                                Borpooft += g_pfootd.base;
                                if (Borpooft > PF_ABS_MAX)
                                    Borpooft = PF_ABS_MAX;
                                if (Borpooft < PF_ABS_MIN)
                                    Borpooft = PF_ABS_MIN;

                                if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                                { //����,����
                                    EnUPflag = 1;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    {
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        if ((Borcmd & 0xf000) == 0x2000)
                                        {
                                            break;
                                        }
                                    }

                                    if ((Borcmd & 0xf000) == 0x2000)
                                    {
                                        movfeed = 1;
                                    }
                                }
                                else if ((Borcmd & 0xf000) == 0x6000)
                                { //����
                                    if ((Dn == 1) && (EnUPflag == 1))
                                    {
                                        Up = 1;
                                        Dn = 0;
                                        Wait_Move_End();                //�ȴ�ƽ̨�ƶ�����
                                        Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        break;
                                    }
                                    EnUPflag = 0;
                                }
                                else if ((Borcmd & 0xff00) == 0xf000)
                                {                    //��ѹ�ŵ���
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Dn == 1)
                                    {
                                        Up = 1;
                                        Dn = 0;
                                        Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                        break;
                                    }
                                    if (Borpfdis < 0)
                                    { //��ѹ������
                                        f = Write_Cmd(C_PFEMOV, 1, -Borpfdis);
                                    }
                                    else if (Borpfdis > 0)
                                    { //��ѹ���½�
                                        f = Write_Cmd(C_PFEMOV, 2, Borpfdis);
                                    }
                                }
                                else if ((Borcmd & 0xff00) == 0xf400)
                                { //����
                                    EnUPflag = 1;
                                    Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                    if (Up == 1)
                                    { //ѹ��������,ѹ����
                                        Up = 0;
                                        Dn = 1;
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        pfootf = 0xff;
                                    }
                                    else
                                    {                                   //ѹ��������,��ѹ�ŷ�����̧����
                                        Send_Pfoot(2, MENU_MOIMAGE, 1); //ѹ�Ž�
                                        Send_Pfoot(1, MENU_MOIMAGE, 1); //ѹ����
                                        Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                                        Up = 0;
                                        Dn = 1;
                                        pfootf = 0xff;
                                    }
                                }
                                else if ((Borcmd & 0xff00) == 0xfc00)
                                { //���ӹ���
                                    excode = Borcmd & 0xff;
                                    if (excode != 0)
                                    {
                                        OutputExcode1(excode, MENU_MOIMAGE, 0);
                                    }
                                }
                                else if ((Borcmd & 0xff00) == 0xec00 || (Borcmd & 0xff00) == 0xea00 || (Borcmd & 0xff00) == 0xeb00)
                                { //��Ƕ�,/����һ,���ö�
                                    if (g_jog_point)
                                        Bor_feedInfo(g_jog_point - 1, g_patd.pat_len, &tBorcmd, &ttemp_oxpos1, &ttemp_oypos1, (S16 *)&tdxpos, (S16 *)&tdypos, (U8 *)&toldspeed, &tBorpooft, &tBorpfdis);
                                    else
                                    {
                                        g_pointPinAng = 0;
                                        g_pointSetPara1 = 0;
                                        g_pointSetPara2 = 0;
                                    }
                                }

                                sendExCodeCmd();
                                g_jog_point--;
                                Comm_RelationMove(-g_dxpos, -g_dypos, MENU_MODIFYADD, 1, movfeed);
                                movfeed = 0;
                                //								dis_updata =1;
                                dis_updata1 = 1;
                                // dis_updata6 = 1;
                            }
                        }

                        tmpfeed = g_jog_point;
                        dis_updata3 = 1;
                        break;

                    case KEY_MOIMAGE_IMAGETOUCH:
                        if (beeponf > 1)
                        {
                            if ((abs(keyxpct - okeyxpct) <= 10) && (abs(keyypct - okeyypct) <= 10) && (abs(keyxpct - stkeyxpct) <= 10) && (abs(keyypct - stkeyypct) <= 10) && (abs(okeyxpct - stkeyxpct) <= 10) && (abs(okeyypct - stkeyypct) <= 10) && (beeponf == 10))
                            {
                                xo = CONVAS_ORG_PX3;
                                yo = CONVAS_ORG_PY3;
                                Scale = 0;
                                okeyxpct = keyxpct;
                                okeyypct = keyypct;
                                Beep_On(BELLT);
                            }
                            else
                            {
                                xo += (keyxpct - okeyxpct);
                                yo += (keyypct - okeyypct);
                                okeyxpct = keyxpct;
                                okeyypct = keyypct;
                            }
                        }
                        else
                        {
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            stkeyxpct = keyxpct;
                            stkeyypct = keyypct;
                        }

                        if (IMAGEJOG)
                        {
                            keyxpos = keyxpct;
                            keyypos = keyypct;
                            keyxdis = keyd[KEY_MOIMAGE_IMAGETOUCH].xpis;
                            keyydis = keyd[KEY_MOIMAGE_IMAGETOUCH].ypis;
                        }

                        dis_updata = 2;
                        bResetCaonvas = 1;
                        break;
                    }
                }
            }
            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {

            if (keyon == KEY_MOIMAGE_SCRNSVR || keyon == KEY_MOIMAGE_IMAGETOUCH) //����Ҫ��ʾ�İ���
            {
            }
            else if (keyon >= KEY_MOIMAGE_PP && keyon <= KEY_MOIMAGE_PAT) //�ǳ��水��
            {
                U16 chJogMethodTemp = keyon - KEY_MOIMAGE_PP;
                DrawButtonOff(keyd[keyon], (g_jogmethod == chJogMethodTemp) ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
            }
            else if (keyon == KEY_MOIMAGE_CODE)
            {
                if (chViewType == MOIMAGE_VIEW)
                {
                    DrawButtonOff(keyd[keyon], (g_CodeEn) ? keybmpind[keyon].border[1] : keybmpind[keyon].border[0]);
                }
                else
                {
                    DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
                }
            }
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ��P1
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[С���̵�������*begin]**********************************************************
        if (pinsetf != pinsetfOld)
        {
            pinsetfOld = pinsetf;
            if (pinsetf == 1)
            {
                dis_updata5 = 1;
                PageWritern((usepageno == 1) ? 2 : 1, usepageno);
                ViewPage(usepageno);
                DrawColor_Gray1(0, TITLE_YLEN, LCD_WIDTH, LCD_HEIGHT - TITLE_YLEN);
                FillRectColorChang(0, TITLE_YLEN, LCD_WIDTH, LCD_HEIGHT - TITLE_YLEN,
                                   Color_White1, Color_Gray);
                //��������
                for (i = RECT_MOIMAGE_MAX1 + RECT_MOIMAGE_MAX2 + RECT_MOIMAGE_MAX3;
                     i < RECT_MOIMAGE_MAX1 + RECT_MOIMAGE_MAX2 + RECT_MOIMAGE_MAX3 + RECT_MOIMAGE_MAX4; i++)
                {
                    DrawFillRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorB[0]);
                    if (i == RECT_MOIMAGE_PININPUT_STR)
                    {
                        DrawDottedRect(rect[i].xpis, rect[i].ypis, rect[i].xsize, rect[i].ysize, rect[i].colorT[0]);
                    }
                    else if (i == RECT_MOIMAGE_PININPUT_TIT)
                    {
                        Draw24x24Str(rect[i].xpis + (rect[i].xsize - 24 * 2) / 2,
                                     rect[i].ypis + (rect[i].ysize - 24) / 2,
                                     MEM_TITLE_ADD + ((U32)rect[i].index[0]) * TitleLengthMax,
                                     SheetPage, rect[i].index[0], rect[i].colorT[0]);
                    }
                }
                //����������ʾ
                for (i = KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2; i < KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2 + KEY_MOIMAGE_MAX3; i++)
                {
                    //���水ť
                    DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
                }

                DrawF16Str(rect[RECT_MOIMAGE_PININPUT_STR].xpis,
                           rect[RECT_MOIMAGE_PININPUT_STR].ypis + rect[RECT_MOIMAGE_PININPUT_STR].ysize + SAFE_DIST4,
                           "(", rect[RECT_MOIMAGE_PININPUT_STR].colorT[0]);
                DrawFU32(rect[RECT_MOIMAGE_PININPUT_STR].xpis + SAFE_DIST1,
                         rect[RECT_MOIMAGE_PININPUT_STR].ypis + rect[RECT_MOIMAGE_PININPUT_STR].ysize + SAFE_DIST4,
                         g_min_feedsum, 5, rect[RECT_MOIMAGE_PININPUT_STR].colorT[0]);
                DrawF16Str(rect[RECT_MOIMAGE_PININPUT_STR].xpis + SAFE_DIST1 * 6,
                           rect[RECT_MOIMAGE_PININPUT_STR].ypis + rect[RECT_MOIMAGE_PININPUT_STR].ysize + SAFE_DIST4,
                           "-", rect[RECT_MOIMAGE_PININPUT_STR].colorT[0]);
                DrawFU32(rect[RECT_MOIMAGE_PININPUT_STR].xpis + SAFE_DIST1 * 7,
                         rect[RECT_MOIMAGE_PININPUT_STR].ypis + rect[RECT_MOIMAGE_PININPUT_STR].ysize + SAFE_DIST4,
                         g_max_feedsum, 5, rect[RECT_MOIMAGE_PININPUT_STR].colorT[0]);
                DrawF16Str(rect[RECT_MOIMAGE_PININPUT_STR].xpis + SAFE_DIST1 * 12,
                           rect[RECT_MOIMAGE_PININPUT_STR].ypis + rect[RECT_MOIMAGE_PININPUT_STR].ysize + SAFE_DIST4,
                           ")", rect[RECT_MOIMAGE_PININPUT_STR].colorT[0]);
            }
            else
            {
                PageWritern(usepageno, (usepageno == 1) ? 2 : 1);
                ViewPage(usepageno);
            }
        }
        // /[С���̵�������*end]**********************************************************

        // /[��ʱ1s��Ϣ����*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
        }
        // /[��ʱ1s��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
            ErrorMsg(MENU_MOIMAGE);
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MOIMAGE);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[ǰ������*begin]**********************************************************
        if (runf1)
        { //ǰ��
            if (F_FILETYPE == PAT_MIT)
            { //����
                while (g_jog_point < runf1)
                {
                    dat1 = g_patd.pat_add + g_jog_point * 2;
                    dat = SREG[dat1] & 0x00ff;
                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                    {
                        if ((dat == 0x1b) || (dat == 0x3))
                        {
                            movfeed = 1;
                        }
                        dat = SREG[dat1 + 1];
                        x = dat & 0x000000ff;
                        y = ((U32)(dat >> 8)) & 0x000000ff;
                        if (x >= 0x80)
                        {
                            x -= 0x80;
                            g_dxpos = -x;
                            g_temp_oxpos -= (S16)(x);
                        }
                        else
                        {
                            g_dxpos = x;
                            g_temp_oxpos += (S16)(x);
                        }
                        if (y >= 0x80)
                        {
                            y -= 0x80;
                            g_dypos = -y;
                            g_temp_oypos -= (S16)(y);
                        }
                        else
                        {
                            g_dypos = y;
                            g_temp_oypos += (S16)(y);
                        }
                        //f = Comm_RelationMove(g_dxpos,g_dypos,MENU_MOIMAGE,0,movfeed);
                        if (g_modcordflag)
                        {
                            f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MOIMAGE, 0, movfeed);
                        }
                        else
                        {
                            f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 0, movfeed);
                        }
                        g_modcordflag = 0;
                        movfeed = 0;
                        if (f == 0)
                        {
                            break;
                        }
                    }
                    else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                    { //���ӹ���
                        excode = 0;
                        if (dat == 0x05)
                            excode = 0x01; //����1
                        else if (dat == 0x0b)
                            excode = 0x02; //����2
                        else if (dat == 0x0c)
                            excode = 0x04; //����3
                        else if (dat == 0x0d)
                            excode = 0x08; //����4
                        else
                        { //����5-8
                            excode = (SREG[dat1] & 0xff00) >> 8;
                            if (excode > 3)
                                excode = 0;
                            excode = (1 << (excode + 4));
                        }
                        if (excode != 0)
                        {
                            f = OutputExcode1(excode, MENU_MOIMAGE, 0);
                            if (f == 0)
                            {
                                break;
                            }
                        }
                    }
                    else if (dat == 0x3c)
                    {
                        g_pointPinAng = (SREG[dat1] & 0xff00) >> 8;
                    }
                    else if (dat == 0x3d)
                    {
                        g_pointSetPara1 = (SREG[dat1] & 0xff00) >> 8;
                    }
                    else if (dat == 0x3e)
                    {
                        g_pointSetPara2 = (SREG[dat1] & 0xff00) >> 8;
                    }
                    sendExCodeCmd();
                    g_jog_point++;
                    // onpaint = Mit_ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    // ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    onpaint = 1;

                    if (onpaint)
                    {
                        GetDrawPatPos(g_jog_point, Scale, &xo, &yo, F_FILETYPE, (F_FILELEN), 0);
                        if (plastno == MENU_CONVERT1)
                        {
                            i = 8;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }
                        else if ((para == 201) || (para == 200) || (para == 220))
                        {
                            i = 6;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else if ((para == 211) || (para == 210))
                        {
                            i = 7;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else
                        {
                            i = 5;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }

                        pat_setPatRule(patRulesTmp);
                        pat_clearConvas(patRule.h_patConvasIndex);
                        pat_drawPattern(patRulesTmp, Scale, xo, yo, F_FILETYPE, F_FILELEN, keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                    }
                    dis_updata1 = 1;

                    //---�д������˳�-----------
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    if (S_ERROR)
                    {
                        Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                        if (DREG[D_ERRWAR])
                        {
                            break;
                        }
                    }
                }

                g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                g_pf_absval += g_pfootd.base;
                if (g_pf_absval != old_PfootOffVal)
                {
                    temp_PfVal = g_pf_absval;
                    old_PfootOffVal = g_pf_absval;
                    f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0);
                    Read_Status();
                    pfootval = 0;
                    Up = 0;
                    Dn = 1;
                    EnUPflag = 1;
                    uplock = 1;
                }
                runf1 = 0;
                dis_updata = 1;
            }
            else if (F_FILETYPE == PAT_SEW)
            { //�ֵ�
                while (g_jog_point < runf1)
                {
                    Bor_feedInfo(g_jog_point + 1, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                    //					Borpooft += g_pfootd.base;
                    //					if(Borpooft >PF_ABS_MAX)	Borpooft = PF_ABS_MAX;
                    //					if(Borpooft <PF_ABS_MIN)	Borpooft = PF_ABS_MIN;

                    if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                    {                    //����,����
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                         //						if(Up == 1){
                                         //							Up = 0;
                                         //							Dn = 1;
                                         //							Send_Pfoot(1,MENU_MOIMAGE);				//ѹ����
                                         //							Read_D(D_Address[SYS]+SYS_STATUS,1,(U16 *)(&YREG[SYS_STATUS]));
                                         //							pfootf = 0xff;
                                         //							if((Borcmd&0xf000)==0x2000){
                                         //								continue;
                                         //							}
                                         //						}
                        if ((Borcmd & 0xf000) == 0x2000)
                        {
                            movfeed = 1;
                        }
                    }
                    else if ((Borcmd & 0xf000) == 0x6000)
                    { //����
                        //						if(Dn == 1){
                        //							Up = 1;
                        //							Dn = 0;
                        //							Wait_Move_End();//�ȴ�ƽ̨�ƶ�����
                        //							Send_Pfoot(2,MENU_MOIMAGE);				//ѹ�Ž�
                        //							Read_D(D_Address[SYS]+SYS_STATUS,1,(U16 *)(&YREG[SYS_STATUS]));
                        //							pfootf = 0xff;
                        //							continue;
                        //						}
                    }
                    else if ((Borcmd & 0xff00) == 0xf000)
                    { //��ѹ�ŵ���
                        //						Wait_Move_End();//�ȴ�ƽ̨�ƶ�����
                        //						if(Dn == 1){
                        //							Up = 1;
                        //							Dn = 0;
                        //							Send_Pfoot(2,MENU_MOIMAGE);				//ѹ�Ž�
                        //							Read_D(D_Address[SYS]+SYS_STATUS,1,(U16 *)(&YREG[SYS_STATUS]));
                        //							pfootf = 0xff;
                        //							continue;
                        //						}
                        //
                        //						if(Borpfdis>0){ //��ѹ������
                        //							f = Write_Cmd(C_PFEMOV, 1, Borpfdis);
                        //						}else if(Borpfdis<0){	//��ѹ���½�
                        //							f = Write_Cmd(C_PFEMOV, 2,-Borpfdis);
                        //						}
                    }
                    else if ((Borcmd & 0xff00) == 0xfc00)
                    { //���ӹ���
                        excode = Borcmd & 0xff;
                        if (excode != 0)
                        {
                            f = OutputExcode1(excode, MENU_MOIMAGE, 0);
                            if (f == 0)
                            {
                                break;
                            }
                        }
                    }

                    f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 1, movfeed);
                    //Comm_Move(temp_oxpos1+g_dxpos,temp_oypos1+g_dypos,movfeed?80:50,MENU_MODIFYADD,1,movfeed);
                    movfeed = 0;
                    if (f == 0)
                    {
                        break;
                    }
                    Borpooft += g_pfootd.base;
                    if (Borpooft > PF_ABS_MAX)
                        Borpooft = PF_ABS_MAX;
                    if (Borpooft < PF_ABS_MIN)
                        Borpooft = PF_ABS_MIN;
                    f = Write_Cmd(C_PFOOTACTION, Borpooft, 0); //������ѹ�Ÿ߶�
                    Read_Status();

                    sendExCodeCmd();
                    g_jog_point++;
                    // onpaint = Mit_ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    // ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    onpaint = 1;
                    if (onpaint)
                    {
                        GetDrawPatPos(g_jog_point, Scale, &xo, &yo, F_FILETYPE, (F_FILELEN), 0);

                        if (plastno == MENU_CONVERT1)
                        {
                            i = 8;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }
                        else if ((para == 201) || (para == 200) || (para == 220))
                        {
                            i = 6;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else if ((para == 211) || (para == 210))
                        {
                            i = 7;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else
                        {
                            i = 5;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }

                        pat_setPatRule(patRulesTmp);
                        pat_clearConvas(patRule.h_patConvasIndex);
                        pat_drawPattern(patRulesTmp, Scale, xo, yo, F_FILETYPE, F_FILELEN, keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                    }
                    dis_updata1 = 1;

                    //---�д������˳�-----------
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    if (S_ERROR)
                    {
                        Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                        if (DREG[D_ERRWAR])
                        {
                            break;
                        }
                    }
                }
                runf1 = 0;
            }
            dis_updata = 1;
        }
        else if (runf2)
        { //����
            if (F_FILETYPE == PAT_MIT)
            { //����
                while (g_jog_point > runf2)
                {
                    dat1 = g_patd.pat_add + g_jog_point * 2;
                    dat = SREG[dat1 - 2] & 0x00ff;
                    if ((dat == 0x61) || (dat == 0x41) || (dat == 0x21) || (dat == 0x1) || (dat == 0x1b) || (dat == 0x3))
                    {
                        if ((dat == 0x1b) || (dat == 0x3))
                        {
                            movfeed = 1;
                        }
                        dat = SREG[dat1 - 1];
                        x = dat & 0x000000ff;
                        y = ((U32)(dat >> 8)) & 0x000000ff;
                        if (x >= 0x80)
                        {
                            x -= 0x80;
                            g_dxpos = x;
                            g_temp_oxpos += (S16)(x);
                        }
                        else
                        {
                            g_dxpos = -x;
                            g_temp_oxpos -= (S16)(x);
                        }
                        if (y >= 0x80)
                        {
                            y -= 0x80;
                            g_dypos = y;
                            g_temp_oypos += (S16)(y);
                        }
                        else
                        {
                            g_dypos = -y;
                            g_temp_oypos -= (S16)(y);
                        }
                        //f = Comm_RelationMove(g_dxpos,g_dypos,MENU_MOIMAGE,0,movfeed);
                        if (g_modcordflag)
                        {
                            f = Comm_RelationMove(g_dxpos + g_modcordx, g_dypos + g_modcordy, MENU_MOIMAGE, 0, movfeed);
                        }
                        else
                        {
                            f = Comm_RelationMove(g_dxpos, g_dypos, MENU_MOIMAGE, 0, movfeed);
                        }
                        g_modcordflag = 0;
                        movfeed = 0;
                        if (f == 0)
                        {
                            break;
                        }
                    }
                    else if ((dat == 0x05) || (dat == 0x0b) || (dat == 0x0c) || (dat == 0x0d) || (dat == 0x1d))
                    { //���ӹ���
                        excode = 0;
                        if (dat == 0x05)
                            excode = 0x01; //����1
                        else if (dat == 0x0b)
                            excode = 0x02; //����2
                        else if (dat == 0x0c)
                            excode = 0x04; //����3
                        else if (dat == 0x0d)
                            excode = 0x04; //����4
                        else
                        { //����5-8
                            excode = (SREG[dat1 - 2] & 0xff00) >> 8;
                            if (excode > 3)
                                excode = 0;
                            excode = (1 << (excode + 4));
                        }
                        if (excode != 0)
                        {
                            f = OutputExcode1(excode, MENU_MOIMAGE, 0);
                            if (f == 0)
                            {
                                break;
                            }
                        }
                    }
                    else if (dat == 0x3c)
                    {
                        g_pointPinAng = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 0);
                    }
                    else if (dat == 0x3d)
                    {
                        g_pointSetPara1 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 1);
                    }
                    else if (dat == 0x3e)
                    {
                        g_pointSetPara2 = mit_getExCodeData(g_patd.pat_add, g_patd.pat_add + (g_jog_point > 0 ? ((g_jog_point - 1) * 2) : 0), 2);
                    }
                    sendExCodeCmd();
                    g_jog_point--;
                    // onpaint = Mit_ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    // ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    onpaint = 1;
                    if (onpaint)
                    {
                        GetDrawPatPos(g_jog_point, Scale, &xo, &yo, F_FILETYPE, (F_FILELEN), 0);
                        if (plastno == MENU_CONVERT1)
                        {
                            i = 8;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }
                        else if ((para == 201) || (para == 200) || (para == 220))
                        {
                            i = 6;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else if ((para == 211) || (para == 210))
                        {
                            i = 7;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else
                        {
                            i = 5;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }

                        pat_setPatRule(patRulesTmp);
                        pat_clearConvas(patRule.h_patConvasIndex);
                        pat_drawPattern(patRulesTmp, Scale, xo, yo, F_FILETYPE, F_FILELEN, keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                    }
                    dis_updata1 = 1;

                    //---�д������˳�-----------
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    if (S_ERROR)
                    {
                        Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                        if (DREG[D_ERRWAR])
                        {
                            break;
                        }
                    }
                }
                g_pf_absval = Mit_CailPadOffestPF(g_jog_point, 0, 0);
                g_pf_absval += g_pfootd.base;
                if (g_pf_absval != old_PfootOffVal)
                {
                    temp_PfVal = g_pf_absval;
                    old_PfootOffVal = g_pf_absval;
                    f = Write_Cmd(C_PFOOTACTION, g_pf_absval, 0);
                    Read_Status();
                    pfootval = 0;
                    Up = 0;
                    Dn = 1;
                    EnUPflag = 1;
                    uplock = 1;
                }
                runf2 = 0;
                dis_updata = 1;
            }
            else if (F_FILETYPE == PAT_SEW)
            { //�ֵ�
                while (g_jog_point > runf2)
                {
                    Bor_feedInfo(g_jog_point, g_patd.pat_len, &Borcmd, &temp_oxpos1, &temp_oypos1, (S16 *)&g_dxpos, (S16 *)&g_dypos, (U8 *)&g_oldspeed, &Borpooft, &Borpfdis);
                    //					Borpooft += g_pfootd.base;
                    //					if(Borpooft >PF_ABS_MAX)	Borpooft = PF_ABS_MAX;
                    //					if(Borpooft <PF_ABS_MIN)	Borpooft = PF_ABS_MIN;

                    if (((Borcmd & 0xf000) == 0x2000) || (Borcmd == 0xfd00))
                    {                    //����,����
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����
                                         //						if(Up == 1){
                                         //							Up = 0;
                                         //							Dn = 1;
                                         //							Send_Pfoot(1,MENU_MOIMAGE);				//ѹ����
                                         //							Read_D(D_Address[SYS]+SYS_STATUS,1,(U16 *)(&YREG[SYS_STATUS]));
                                         //							pfootf = 0xff;
                                         //							if((Borcmd&0xf000)==0x2000){
                                         //								continue;
                                         //							}
                                         //						}

                        if ((Borcmd & 0xf000) == 0x2000)
                        {
                            movfeed = 1;
                        }
                    }
                    else if ((Borcmd & 0xf000) == 0x6000)
                    { //����
                        //						if(Dn == 1){
                        //							Up = 1;
                        //							Dn = 0;
                        //							Wait_Move_End();//�ȴ�ƽ̨�ƶ�����
                        //							Send_Pfoot(2,MENU_MOIMAGE);				//ѹ����
                        //							Read_D(D_Address[SYS]+SYS_STATUS,1,(U16 *)(&YREG[SYS_STATUS]));
                        //							pfootf = 0xff;
                        //							continue;
                        //						}
                    }
                    else if ((Borcmd & 0xff00) == 0xf000)
                    { //��ѹ�ŵ���
                        //						Wait_Move_End();//�ȴ�ƽ̨�ƶ�����
                        //						if(Dn == 1){
                        //							Up = 1;
                        //							Dn = 0;
                        //							Send_Pfoot(2,MENU_MOIMAGE);				//ѹ�Ž�
                        //							Read_D(D_Address[SYS]+SYS_STATUS,1,(U16 *)(&YREG[SYS_STATUS]));
                        //							pfootf = 0xff;
                        //							continue;
                        //						}
                        //						if(Borpfdis<0){ //��ѹ������
                        //							f = Write_Cmd(C_PFEMOV, 1, -Borpfdis);
                        //						}else if(Borpfdis>0){	//��ѹ���½�
                        //							f = Write_Cmd(C_PFEMOV, 2, Borpfdis);
                        //						}
                    }
                    else if ((Borcmd & 0xff00) == 0xfc00)
                    { //���ӹ���
                        excode = Borcmd & 0xff;
                        if (excode != 0)
                        {
                            f = OutputExcode1(excode, MENU_MOIMAGE, 0);
                            if (f == 0)
                            {
                                break;
                            }
                        }
                    }

                    f = Comm_RelationMove(-g_dxpos, -g_dypos, MENU_MOIMAGE, 1, movfeed);
                    //						Comm_Move(temp_oxpos1,temp_oypos1,movfeed?80:50,MENU_MODIFYADD,1,movfeed);
                    movfeed = 0;
                    if (f == 0)
                    {
                        break;
                    }
                    Borpooft += g_pfootd.base;
                    if (Borpooft > PF_ABS_MAX)
                        Borpooft = PF_ABS_MAX;
                    if (Borpooft < PF_ABS_MIN)
                        Borpooft = PF_ABS_MIN;
                    f = Write_Cmd(C_PFOOTACTION, Borpooft, 0); //������ѹ�Ÿ߶�
                    Read_Status();

                    if ((Borcmd & 0xff00) == 0xec00 || (Borcmd & 0xff00) == 0xea00 || (Borcmd & 0xff00) == 0xeb00)
                    { //��Ƕ�,/����һ,���ö�
                        if (g_jog_point)
                            Bor_feedInfo(g_jog_point - 1, g_patd.pat_len, &tBorcmd, &ttemp_oxpos1, &ttemp_oypos1, (S16 *)&tdxpos, (S16 *)&tdypos, (U8 *)&toldspeed, &tBorpooft, &tBorpfdis);
                        else
                        {
                            g_pointPinAng = 0;
                            g_pointSetPara1 = 0;
                            g_pointSetPara2 = 0;
                        }
                    }
                    sendExCodeCmd();
                    g_jog_point--;
                    // onpaint = Mit_ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    // ViewPoint1(g_jog_point, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0);
                    onpaint = 1;
                    if (onpaint)
                    {
                        GetDrawPatPos(g_jog_point, Scale, &xo, &yo, F_FILETYPE, (F_FILELEN), 0);
                        if (plastno == MENU_CONVERT1)
                        {
                            i = 8;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }
                        else if ((para == 201) || (para == 200) || (para == 220))
                        {
                            i = 6;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else if ((para == 211) || (para == 210))
                        {
                            i = 7;
                            patRulesTmp = PATRULES_JOG_A_X;
                        }
                        else
                        {
                            i = 5;
                            patRulesTmp = PATRULES_JOG_DETAIL;
                        }

                        pat_setPatRule(patRulesTmp);
                        pat_clearConvas(patRule.h_patConvasIndex);
                        pat_drawPattern(patRulesTmp, Scale, xo, yo, F_FILETYPE, F_FILELEN, keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                    }

                    dis_updata1 = 1;

                    //---�д������˳�-----------
                    Read_D(D_Address[SYS] + SYS_STATUS, 1, (U16 *)(&YREG[SYS_STATUS]));
                    if (S_ERROR)
                    {
                        Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                        if (DREG[D_ERRWAR])
                        {
                            break;
                        }
                    }
                }
                runf2 = 0;
            }
            dis_updata = 1;
        }
        // /[ǰ������*end]**********************************************************

        // /[���ƻ���ͼ��*begin]**********************************************************
        if (dis_updata)
        {
            if (dis_updata > 1)
            {
                if (xo < POSI.x0)
                {
                    xo = POSI.x0;
                }
                if (xo > POSI.x1)
                {
                    xo = POSI.x1;
                }
                if (yo < POSI.y0)
                {
                    yo = POSI.y0;
                }
                if (yo > POSI.y1)
                {
                    yo = POSI.y1;
                }
            }

            if (plastno == MENU_CURSOR)
            {
                i = 4;
                patRulesTmp = PATRULES_VIEW_SIMPLE;
            }
            else if (plastno == MENU_CONVERT1)
            {
                i = 8;
                patRulesTmp = PATRULES_JOG_DETAIL;
            }
            // else if ((para == 210) || (para == 200) || (para == 220))
            // {
            //     i = 6;
            // }
            // else if ((para == 211) || (para == 201))
            // {
            //     i = 7;
            // }
            else if ((para == 201) || (para == 200) || (para == 220))
            {
                i = 6;
                patRulesTmp = PATRULES_VIEW_SIMPLE;
            }
            else if ((para == 211) || (para == 210))
            {
                i = 7;
                patRulesTmp = PATRULES_JOG_A_X;
            }
            else
            {
                i = 5;
                patRulesTmp = PATRULES_JOG_DETAIL;
            }
            if (i == 4) //���
            {
                pat_setPatRule(patRulesTmp);
                pat_clearConvas(patRule.h_patConvasIndex);
                pat_drawPattern(PATRULES_JOG_DETAIL, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
            }
            else
            {
                if (chViewType == MOIMAGE_VIEW)
                {
                    // Draw_Pattern(i, Scale, xo, yo, F_FILETYPE, (F_FILELEN), 0, 0, NULL);
                    patRulesTmp = PATRULES_VIEW_DETAIL;
                    pat_setPatRule(patRulesTmp);
                    pat_clearConvas(patRule.h_patConvasIndex);
                    pat_drawPattern(PATRULES_VIEW_DETAIL, Scale, xo, yo, F_FILETYPE, F_FILELEN, 0, 0, NULL);
                }
                else
                {
                    pat_setPatRule(patRulesTmp);
                    pat_clearConvas(patRule.h_patConvasIndex);
                    pat_drawPattern(PATRULES_JOG_DETAIL, Scale, xo, yo, F_FILETYPE, F_FILELEN, keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                    // Draw_Pattern(i, Scale, xo, yo, F_FILETYPE, (F_FILELEN), keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                    dis_updata3 = 1;
                }

                if (IMAGEJOG)
                {
                    keyxpos = 0;
                    keyypos = 0;
                    keyxdis = 0;
                    keyydis = 0;
                }
            }

            POSI.x0 = (g_patd.patiX - g_patd.patmX) * g_patd.scale / 2000 - (CONVAS_X3 / 2) + CONVAS_ORG_PX3;
            POSI.x1 = (g_patd.patmX - g_patd.patiX) * g_patd.scale / 2000 + (CONVAS_X3 / 2) + CONVAS_ORG_PX3;
            POSI.y0 = (g_patd.patiY - g_patd.patmY) * g_patd.scale / 2000 - (CONVAS_Y3 / 2) + CONVAS_ORG_PY3;
            POSI.y1 = (g_patd.patmY - g_patd.patiY) * g_patd.scale / 2000 + (CONVAS_Y3 / 2) + CONVAS_ORG_PY3;

            dis_updata = 0;
        }
        // /[���ƻ���ͼ��*end]**********************************************************

        // /[��ǰʵ����λ��*begin]**********************************************************
        if (dis_updata1)
        {
#if 0
            S32 xPosJog = CONVAS_ORG_PX3, yPosJog = CONVAS_ORG_PY3;
            dis_updata1 = 0;
            // DrawFillRect(keyd2[0].xpis + 1, keyd2[0].ypis + 1, keyd2[0].xsize - 2, keyd2[0].ysize - 2, Color_White);
            // DrawMFU32(keyd2[0].xpis + 4, keyd2[0].ypis + 8, g_jog_point, 5, Color_Black);
            getFeedPos(g_jog_point, F_FILETYPE, &xtmp, &ytmp);

            tscale1 = Scale + tscale;
            xPosJog = (S32)(xtmp * tscale1 / 1000 + xPosJog);
            yPosJog = (S32)(tmuly * ytmp * tscale1 / 1000 + yPosJog);

            DrawBigPoint(xPosJog, yPosJog, COLOR_SELECT_P);

            //����룬�׶���
            DrawFillRect(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 1,
                         rect[RECT_MOIMAGE_PINDATA].xsize, TYPE_SIZE5, rect[RECT_MOIMAGE_PINDATA].colorB[0]);
            DrawMFU32(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 1 + SAFE_DIST4,
                      g_jog_point, 5, rect[RECT_MOIMAGE_PINDATA].colorT[0]); //��ǰ����Ϣλ��
            //���У��׶���
            DrawFillRect(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis,
                         rect[RECT_MOIMAGE_PINDATA].xsize, TYPE_SIZE5, rect[RECT_MOIMAGE_PINDATA].colorB[0]);
            viewFeedCode(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + SAFE_DIST1,
                         g_jog_point, F_FILETYPE, 1, 2); //��ʾ���
#else
            dis_updata1 = 0;
            //����룬�׶���
            DrawFillRect(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 1,
                         rect[RECT_MOIMAGE_PINDATA].xsize, TYPE_SIZE5, rect[RECT_MOIMAGE_PINDATA].colorB[0]);
            DrawMFU32(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + TYPE_SIZE5 * 1 + SAFE_DIST4,
                      g_jog_point, 5, rect[RECT_MOIMAGE_PINDATA].colorT[0]); //��ǰ����Ϣλ��
            //���У��׶���
            DrawFillRect(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis,
                         rect[RECT_MOIMAGE_PINDATA].xsize, TYPE_SIZE5, rect[RECT_MOIMAGE_PINDATA].colorB[0]);
            viewFeedCode(rect[RECT_MOIMAGE_PINDATA].xpis, rect[RECT_MOIMAGE_PINDATA].ypis + SAFE_DIST1,
                         g_jog_point, F_FILETYPE, 1, 2); //��ʾ���
            dis_updata = 1;
#endif
        }
        // /[��ǰʵ����λ��*end]**********************************************************

        // /[���뷽���İ�ťЧ���л�*begin]**********************************************************
        if (dis_updata2)
        {
            U8 chMethodTemp = 0;
            dis_updata2 = 0;
            chMethodTemp = g_jogmethod + KEY_MOIMAGE_PP;
            for (i = KEY_MOIMAGE_PP; i <= KEY_MOIMAGE_PAT; i++)
            {
                DrawButton(keyd[i], (U16 *)(ADD_ICONBMP),
                           (chMethodTemp == i) ? keybmpind[i].border[1] : keybmpind[i].border[0],
                           (chMethodTemp == i) ? keybmpind[i].icon[1] : keybmpind[i].icon[0]);
            }
        }
        // /[���뷽���İ�ťЧ���л�*end]**********************************************************

        // /[��ǰָʾ���λ��*begin]**********************************************************
        if (dis_updata3)
        {
            dis_updata3 = 0;
            if (tmpfeed > g_max_feedsum || tmpfeed < g_min_feedsum)
            {
                tmpfeed = g_jog_point;
            }
            DrawFillRect(rect[RECT_MOIMAGE_PININPUT].xpis + 1, rect[RECT_MOIMAGE_PININPUT].ypis + 1,
                         rect[RECT_MOIMAGE_PININPUT].xsize - 2, rect[RECT_MOIMAGE_PININPUT].ysize - 2,
                         rect[RECT_MOIMAGE_PININPUT].colorB[0]);

            DrawMFU32(rect[RECT_MOIMAGE_PININPUT].xpis + (rect[RECT_MOIMAGE_PININPUT].xsize - 12 * 5) / 2,
                      rect[RECT_MOIMAGE_PININPUT].ypis + SAFE_DIST4,
                      tmpfeed, 5, rect[RECT_MOIMAGE_PININPUT].colorT[0]);
        }
        // /[��ǰָʾ���λ��*end]**********************************************************

        // /[С���̵��������ݸ���*begin]**********************************************************

        if (dis_updata5)
        {
            dis_updata5 = 0;
            DrawFillRect(rect[RECT_MOIMAGE_PININPUT_STR].xpis + 1, rect[RECT_MOIMAGE_PININPUT_STR].ypis + 1,
                         rect[RECT_MOIMAGE_PININPUT_STR].xsize - 2, rect[RECT_MOIMAGE_PININPUT_STR].ysize - 2,
                         rect[RECT_MOIMAGE_PININPUT_STR].colorB[0]);

            DrawMFU32(rect[RECT_MOIMAGE_PININPUT_STR].xpis + (rect[RECT_MOIMAGE_PININPUT_STR].xsize - 12 * 5) / 2,
                      rect[RECT_MOIMAGE_PININPUT_STR].ypis + SAFE_DIST4,
                      PinSets, 5, rect[RECT_MOIMAGE_PININPUT_STR].colorT[0]);
        }
        // /[С���̵��������ݸ���*end]**********************************************************

        // /[Code��ťͼ��ˢ��*begin]**********************************************************
        if (chCodeEnTemp != g_CodeEn)
        {
            chCodeEnTemp = g_CodeEn;
            if (chViewType == MOIMAGE_VIEW)
            {
                DrawButton(keyd[KEY_MOIMAGE_CODE], (U16 *)(ADD_ICONBMP),
                           (g_CodeEn) ? keybmpind[KEY_MOIMAGE_CODE].border[1] : keybmpind[KEY_MOIMAGE_CODE].border[0],
                           (g_CodeEn) ? keybmpind[KEY_MOIMAGE_CODE].icon[1] : keybmpind[KEY_MOIMAGE_CODE].icon[0]);
            }
        }
        // /[Code��ťͼ��ˢ��*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MOIMAGE)
        {
            plastno = MENU_MOIMAGE;
            break;
        }

        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}

/***************************************/

#define KEY_MOIMAGE2_SCRNSVR 1    //1 ����
#define KEY_MOIMAGE2_BACK 2       //2 ����
#define KEY_MOIMAGE2_ZOOMOUT 3    //3 ��С
#define KEY_MOIMAGE2_ZOOMIN 4     //4 �Ŵ�
#define KEY_MOIMAGE2_ENTER 5      //5 ȷ��
#define KEY_MOIMAGE2_CODE 6       //6 ��ʾ����
#define KEY_MOIMAGE2_IMAGETOUCH 7 //7 ����Ų��

#define KEY_MOIMAGE2_MAX 8

#define RECT_MOIMAGE2_HINT 1 //1 ��ʾ
#define RECT_MOIMAGE2_MAX 2

void Menu_MoView2(U8 para)
{
    U32 i;
    U8 f = 0;

    /****������ز���begin****/
    U8 beeponf = 0;                         //������ ������־λ
    U16 delay = 0;                          //����ִ���ӳٵ��趨ʱ�䣬��Times���бȽ�
    U32 keyon = 0, keyoff = 0, keydata = 0; //keyon�������£�keyoff�����ɿ���keydata��ǰ������Ϣ
    /****������ز���end****/

    /****��־λ��ز���begin****/
    //U8 threadflag = 0;   //����ģʽ��־  0������   1���������봩��ģʽ  2�����ڴ���ģʽ
    //U8 vPins = 0;	// 0-����ʾ��� 1-��ʾ���
    //	S32 vpl,vpr,vpu,vpd;

    //	U8 xmove =0,ymove =0;
    U8 dis_updata = 1 /*, dis_updata1 = 0,       //dis_updata:1��ȫ���ػ滨����2�������ƶ����� ;dis_updata1:���µ�ǰʵ����������
        dis_updata2 = 0, dis_updata3 = 0      //dis_updata2:���뷽����ť��ʾ�л� ; dis_updata3:��ʾ��ǰָʾ������
        dis_updata6 = 0, dis_updata7 = 0*/
        ;             //dis_updata6:��ʾָʾ���λ��;
    // static U8 EnUPflag = 0;

    S16 xo = CONVAS_ORG_PX4; //��ǰ��ʾ��ԭ������
    S16 yo = CONVAS_ORG_PY4;
    COORDINATE2 POSI;
    S32 Scale;
    U8 bResetCaonvas = 0;

    U16 okeyxpct = 0, okeyypct = 0, stkeyxpct = 0, stkeyypct = 0;
    U16 keyxpos = 0, keyypos = 0, keyxdis = 0, keyydis = 0;
    // U8 Up = 0;
    // U8 Dn = 1;
    // U8 pfootf = 0xff;
    U32 tmpfeed = 0; //PinSets:���������PINֵ��tmpfeed����ǰȷ�ϵ���ֵ
    // U16 moddat = 0;                 //�������λ��
    // S16 old_PfootOffVal = 0;
    // S16 temp_PfVal = 0;
    U8 isHome;

    U8 chCodeEnTemp = 0;
    U8 patRulesTmp = PATRULES_VIEW_SIMPLE;

    /****��־λ��ز���end***/

    //    const U8 keynum = {KEY_MOIMAGE_MAX1 + KEY_MOIMAGE_MAX2 + KEY_MOIMAGE_MAX3};
    const KEYPAD keyd[] = {
        {0, 0, 0, 0},

        {0, 0, 128, 32},                                //1 ����
        {TYPE_X1, TYPE_Y4, KEY_X1, KEY_Y1},             //2 ����
        {TYPE_X2, TYPE_Y4, KEY_X1, KEY_Y1},             //3 ��С
        {TYPE_X3, TYPE_Y4, KEY_X1, KEY_Y1},             //4 �Ŵ�
        {TYPE_X11, TYPE_Y4, KEY_X1, KEY_Y1},            //5 ȷ��
        {TYPE_X10, TYPE_Y4, KEY_X1, KEY_Y1},            //6 ��ʾ����
        {CONVAS_PX4, CONVAS_PY4, CONVAS_X4, CONVAS_Y4}, //7 ����Ų��

    };

    const tydDisSrcIndex keybmpind[] = {
        {{KEY_Silver}, {0}, {0}, {0, 0, 0, 0}, {0}},
        {{KEY_Silver}, {0}, {0}, {1, 0, 0, 0}, {1}},                                      //1 ����
        {{KEY_Silver}, {0}, {72}, {2, 0, 0, 0}, {2}},                                     //2 ����
        {{KEY_Silver}, {0}, {74}, {4, 0, 0, 0}, {0}},                                     //3 ��С
        {{KEY_Silver}, {0}, {75}, {3, 0, 0, 0}, {0}},                                     //4 �Ŵ�
        {{KEY_Silver}, {0}, {71}, {5, 0, 0, 0}, {5, 0}},                                  //5 ȷ��
        {{KEY_Silver, KEY_Blue, KEY_Gray}, {0}, {375, 376, 377, 3}, {26, 0, 0, 25}, {0}}, //6 ��ʾ����
        {{KEY_Silver}, {0}, {Color_Black}, {9, 0, 0, 0}, {0}},                            //7 ����Ų��

    };

    const RECTPAD rect[] = {
        //����
        {0, 0, 0, 0, {Color_Black}, {Color_White}, {0}},

        {TYPE_X4, TYPE_Y4, TYPE_SIZE23, TYPE_SIZE9, {Color_Blue}, {Color_White}, {10}}, //1 ����-����

    };

    // /[������ʾ����ļ�·��������ҳ���ʼ��*begin]************************************************
    SetLanguage(MENU_MOIMAGE); //���õ�ǰ������ʾ������P1ҳ�棬��Ӧ41.bin
    InitScreen();
    initViewStatus();
    updateViewStatus();
    pat_resetPatRule();
    pat_setPatRule(PATRULES_VIEW_SIMPLE);

    // /[������ʾ����ļ�·��������ҳ���ʼ��*end]************************************************

    // /[��ʾ��ر�־λ��Ϣ��ȡ*begin]**********************************************************
    IMAGEJOG = 0;
    g_ImgPoint = 0;

    if (plastno != MENU_SCRNSVR)
    {
        g_publicbuf[40] = plastno;
    }
    else
    {
        plastno = g_publicbuf[40];
    }
    Scale = 0;
    g_patd.scale = 100;
    if (g_IsLocal && (g_errLocal == 0)) //��ͼ����ʾ�������
    {
        E_RUNEN();
        Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
    }
    if (g_ImgMove == 1) //�Ӵ綯�������,֧�ֿ����ƶ�
    {
        IMAGEJOG = 1;
        g_ImgPoint = 1;
        tmpfeed = g_jog_point;

        // pfootf = (U8)(S_PFOOT);
        // if (pfootf == 0x10)
        // { //����
        //     Up = 1;
        //     Dn = 0;
        // }
        // else
        // {
        //     Up = 0;
        //     Dn = 1;
        // }

        // if (g_max_feedsum < 100)
        // {
        //     moddat = 10;
        // }
        // else if (g_max_feedsum < 1000)
        // {
        //     moddat = 100;
        // }
        // else if (g_max_feedsum < 10000)
        // {
        //     moddat = 1000;
        // }
        // else if (g_max_feedsum < 100000)
        // {
        //     moddat = 10000;
        // }
        // if (g_jog_point == 0)
        // {
        //     EnUPflag = 1;
        // }
        // pfootf = 0xff;

        // if (para != MOIMAGE_VIEW)
        // {
        //     dis_updata1 = 1;
        //     dis_updata2 = 1;
        //     dis_updata3 = 1;
        // }
    }

    // /[��ʾ��ر�־λ��Ϣ��ȡ*end]**********************************************************

    // /[���Ƴ�ʼ��ʾҳ��*begin]**************************************************************

    Draw24x24Str(TITLE_XPOS_C2, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]); //չ��ͼ��

    for (i = 2; i < KEY_MOIMAGE2_MAX; i++)
    {
        if (i == KEY_MOIMAGE2_IMAGETOUCH) //����Ҫ��ʾ�İ���
        {
            DrawFillRect(keyd[i].xpis, keyd[i].ypis, keyd[i].xsize, keyd[i].ysize, keybmpind[i].icon[0]);
        }
        else //���水ť
        {
            DrawButton(keyd[i], (U16 *)(ADD_ICONBMP), keybmpind[i].border[0], keybmpind[i].icon[0]);
        }
    }

    if ((para == MOIMAGE_ENSURE1) || (para == MOIMAGE_ENSURE2) || (para == MOIMAGE_ENSURE3))
    {
        DrawFillRect(rect[RECT_MOIMAGE2_HINT].xpis, rect[RECT_MOIMAGE2_HINT].ypis,
                     rect[RECT_MOIMAGE2_HINT].xsize, rect[RECT_MOIMAGE2_HINT].ysize,
                     rect[RECT_MOIMAGE2_HINT].colorB[0]);
        Draw24x24Str(rect[RECT_MOIMAGE2_HINT].xpis + SAFE_DIST4, rect[RECT_MOIMAGE2_HINT].ypis + SAFE_DIST4,
                     MEM_TITLE_ADD + (g_selectmode + rect[RECT_MOIMAGE2_HINT].index[0]) * TitleLengthMax, SheetPage,
                     g_selectmode + rect[RECT_MOIMAGE2_HINT].index[0], rect[RECT_MOIMAGE2_HINT].colorT[0]);
    }
    // /[���Ƴ�ʼ��ʾҳ��*end]**********************************************************

    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*begin]**********************************************************
    f = Read_D(D_Address[SYS] + SYS_PFOOTNOW, 1, (U16 *)(&YREG[SYS_PFOOTNOW]));
    if (f == 0)
    {
        pno = MENU_RWHELP;
        hleppara = 15;
        return;
    }
    // temp_PfVal = YREG[SYS_PFOOTNOW];
    // old_PfootOffVal = temp_PfVal;
    // if (g_jog_point == 0)
    // {
    //     EnUPflag = 1;
    // }

    ViewPage(usepageno); //������ʾ����
    // /[����״̬��Ϣ����Ҫִ�е�������Ϣ��ȡ*end]**********************************************************

    Timer_ms = 30720; //0x7800
    Timerms = 0;
    Timer_ms1 = 0;
    while (1)
    {
        keydata = get_keydata((KEYPAD *)keyd, KEY_MOIMAGE2_MAX, 0); //��ȡ��ֵ

        // /[������ֵ����Ԥ����*begin]**********************************************************

        //�ض�����²���Ч����Ԥ����

        if (keydata == 0xff) //�����ɿ���ļ�ֵԤ����
        {
            if ((keyon == KEY_MOIMAGE2_ZOOMOUT) || (keyon == KEY_MOIMAGE2_ZOOMIN) || (keyon == KEY_MOIMAGE2_IMAGETOUCH))
            {
            }
            else if ((keyon) && (beeponf)) //����������ʱ�ļ�ֵ���������ɿ���ֵ
            {
                keyoff = keyon;
            }
            keydata = 0;
        }
        else if (keydata > USESCREENSAVER) //�������µļ�ֵԤ����    ����ʾ����Ϣ����
        {
            if (keyon == 0) //�������¼�ֵ��ֵ�밴������Ч����ʾ(��ǰû�а�������ʱ)
            {
                keyon = keydata;
                if (keyon == KEY_MOIMAGE2_SCRNSVR || keyon == KEY_MOIMAGE2_IMAGETOUCH) //�ް�����Ч
                {
                }
                else
                {
                    DrawButtonOn(keyd[keyon], keybmpind[keyon].border[0]);
                }
                Times = 0;
                delay = KEYDT; //������������
            }
            else if (keyon != keydata) //���а������£��������°�����Ϣ
            {
                keydata = 0;
            }

            if (Times > delay) //��������
            {
                if (beeponf == 0) //��������־λ //��һ�ν���
                {
                    beeponf = 1;

                    if (g_keyBellT) //����ʱ����������ʱ��
                        Beep_On(g_keyBellT);

                    switch (keyon) //�������±�����ʾ����ʾ
                    {
                    case KEY_MOIMAGE2_CODE:
                        DrawFMenuStr((para == MOIMAGE_SKIPJOG) ? keybmpind[keyon].titleStr[3] : keybmpind[keyon].titleStr[0]);
                        break;
                    default:
                        DrawFMenuStr(keybmpind[keyon].titleStr[0]);
                        break;
                    }

                    delay = KEYFT;
                }
                else if (beeponf == 1) //�ڶ��ν���
                {
                    beeponf = 2;
                    if ((keyon == KEY_MOIMAGE2_ZOOMOUT) || (keyon == KEY_MOIMAGE2_ZOOMIN))
                    {
                        delay = KEYDT;
                    }
                    else
                        delay = KEYNT;
                }
                else if (beeponf < 20)
                {
                    beeponf++;
                }

                if ((keyon == KEY_MOIMAGE2_ZOOMOUT) || (keyon == KEY_MOIMAGE2_ZOOMIN) || (keyon == KEY_MOIMAGE2_IMAGETOUCH)) //���г����������ܰ����Ĺ���
                {
                    keyoff = keyon;
                    key_on_flag1 = 1;
                }
                Times = 0; //��ǰ
            }
        }
        else //����û�а���
        {
            key_on_flag1 = 0;
        }

        // /[������ֵ����Ԥ����*end]**********************************************************

        // /[�����ɿ���ֵ����*begin]**********************************************************
        if (keyoff)
        {

            if (g_needtohome && (keyoff == KEY_MOIMAGE2_BACK))
            {
                pno = MENU_RWHELP;
                hleppara = 158;
                poldno[pnoindex] = MENU_MOIMAGE;
                pnoindex++;
            }
            else
            {
                switch (keyoff)
                {
                case KEY_MOIMAGE2_SCRNSVR:
                    pno = MENU_SCRNSVR;
                    break;
                case KEY_MOIMAGE2_CODE:
                    if (para == MOIMAGE_SKIPJOG)
                    {
                        f = Go_Home(MENU_MOIMAGE, 1); //0
                        if (f == 0)
                            break;
                        // EnUPflag = 1;

                        isHome = (U8)(S_HOME) | (U8)(S_NEWHOME);
                        if ((isHome & 0x02) || (isHome & 0x04))
                        {
                        }
                        else
                        {                     //û�лص���еԭ��λ��
                            g_needtohome = 1; //�綯ǰ��Ҫ��һ��ԭ��
                            break;
                        }
                        g_needtohome = 0;
                        if (F_FILETYPE == PAT_MIT)
                        { //����
                            g_jog_point = 0;
                            // Up = 0;
                            // Dn = 1;
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        { //�ֵ�
                            g_BorSTflag = 0;
                            g_jog_point = 0;
                            // Up = 0;
                            // Dn = 1;
                        }
                        g_ExCode = 0;
                        g_curExCode = 0;
                        g_curPinAng = 0;
                        g_curSetPara1 = 0;
                        g_curSetPara2 = 0;
                        g_pointPinAng = 0;
                        g_pointSetPara1 = 0;
                        g_pointSetPara2 = 0;
                        dis_updata = 1;
                    }
                    else
                    {
                        if (g_CodeEn == 1)
                            g_CodeEn = 0;
                        else
                            g_CodeEn = 1;
                        dis_updata = 1;
                    }
                    break;
                case KEY_MOIMAGE2_ENTER:
                    if (para == MOIMAGE_ENSURE1 || para == MOIMAGE_ENSURE2)
                    {
                        if (F_FILETYPE == PAT_MIT)
                        {
                            pnoindex -= 2;
                            pno = MENU_MODIFY;
                            g_pno_temp = MENU_MOIMAGE;
                            g_temp_pno = MENU_MOIMAGE;
                            Go_Stpoint(MENU_MOIMAGE, 0);
                            g_temp_point_num = g_point_num;
                            if (g_modzig)
                            {
                                //if((g_zig_width==0)||(g_zig_width>100)) g_zig_width=10;
                                //if((g_zig_pitch<5)||(g_zig_pitch>100))g_zig_pitch=30;
                                if ((g_zig_width < 1) || (g_zig_width > 100))
                                    g_modzig = 0;
                                if ((g_zig_pitch < 5) || (g_zig_pitch > 100))
                                    g_modzig = 0;
                            }
                            Mit_Modify_Data(5);

                            if ((pno != MENU_MODIFY) && (para == 220))
                            {
                                poldno[pnoindex - 1] = MENU_MODIFYADD;
                            }
                        }
                        else if (F_FILETYPE == PAT_SEW)
                        {
                            g_pno_temp = MENU_MOIMAGE;
                            g_temp_pno = MENU_MOIMAGE;
                            g_temp_point_num = g_point_num;

                            if (g_tdxpos != 0 || g_tdypos != 0)
                            {
                                Comm_RelationMove(-g_tdxpos, -g_tdypos, MENU_MOIMAGE, 1, 0);
                            }
                            Go_Stpoint(MENU_MOIMAGE, 1);
                            if (g_modzig)
                            {
                                //if((g_zig_width==0)||(g_zig_width>100)) g_zig_width=10;
                                //if((g_zig_pitch<5)||(g_zig_pitch>100))g_zig_pitch=30;
                                if ((g_zig_width < 1) || (g_zig_width > 100))
                                    g_modzig = 0;
                                if ((g_zig_pitch < 5) || (g_zig_pitch > 100))
                                    g_modzig = 0;
                            }
                            f = Bor_ModMoArea(g_jog_stpoint, g_jog_enpoint, 0, 0, g_parameter, g_selectmode + 1, g_patd.pat_len);
                            if (f == 0)
                            { //���곬����Χ
                                g_point_num--;
                                hleppara = 4;
                                pno = MENU_INHELP;
                                //pnoindex+=2;
                                poldno[pnoindex] = MENU_MODIFY1; //pnoindex++;
                                RecovrSREG(g_savetimes, 1);
                            }
                            else if (f == 2)
                            { //ͼ�γ�����Χ
                                pno = MENU_RWHELP;
                                hleppara = 65;
                                poldno[pnoindex] = MENU_MODIFY;
                                pnoindex++;
                                RecovrSREG(g_savetimes, 1);
                                //Go_Home(MENU_MODIFY,1);	//0
                                //g_jog_point =0;
                                //g_modifyflag = 1;
                                //g_Tsavetimes = g_savetimes;
                                //g_Tsavetimes++;
                                //g_savetimes = g_Tsavetimes;
                                //BackupSREG(g_Tsavetimes,0,g_patd.pat_len,1);
                            }
                            else
                            {
                                //��ƽ̨�ƶ�����ʼ��
                                //Bor_feedInfo(g_jog_stpoint,g_patd.pat_len,&Borcmd,&temp_oxpos,&temp_oypos,&dx,&dy,&speed,&Borpooft,&Borpfdis);
                                //g_jog_point = g_jog_stpoint;
                                //Send_Pfoot(1,MENU_MODIFY,1);	//ѹ����
                                //
                                //Comm_Move(temp_oxpos+dx,temp_oypos+dy,80,MENU_MODIFYADD,1,0);

                                g_modifyflag = 1;
                                g_Tsavetimes = g_savetimes;
                                g_Tsavetimes++;
                                g_savetimes = g_Tsavetimes;
                                BackupSREG(g_Tsavetimes, 0, g_patd.pat_len, 1);
                                pno = MENU_MODIFY;
                                pnoindex -= 2;
                            }
                        }
                        g_imgpara = 0;
                        g_ImgPoint = 0;
                        g_ImgMove = 0;
                        g_IsLocal = 0;

                        break;
                    }
                case KEY_MOIMAGE2_BACK:
                    if (g_IsLocal == 1)
                    {
                        Wait_Move_End(); //�ȴ�ƽ̨�ƶ�����

                        f = Write_Cmd2(C_SETPINS, g_jog_point, 2, 3);
                        if (f == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_MOIMAGE;
                            pnoindex++;
                            break;
                        }

                        if (Read1_Status(50) == 0)
                        {
                            pno = MENU_RWHELP;
                            hleppara = 15;
                            poldno[pnoindex] = MENU_MOIMAGE;
                            pnoindex++;
                            break;
                        }
                        else if (DREG[D_CMDSTATUS] != 100)
                        {
                            Read_D(D_Address[DISP] + D_ERRWAR, 1, (U16 *)(&DREG[D_ERRWAR]));
                            if (DREG[D_ERRWAR] && (g_errLocal == 0))
                            {
                                pno = MENU_RWHELP;
                                hleppara = 21;
                                poldno[pnoindex] = MENU_MOIMAGE;
                                pnoindex++;
                                g_errLocal++;
                                break;
                            }
                            pno = MENU_RWHELP;
                            hleppara = 16;
                            pnoindex = 1;
                            poldno[0] = MENU_P1;
                            g_errLocal = 0;
                            E_RUNDIS();
                            Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
                            break;
                        }
                        E_RUNDIS();
                        Write_Cmd(C_WRITESTATUS, YREG[SYS_ENABLE], SYS_ENABLE);
                    }
                    if (para == 201 && g_selectmode == 2)
                    { //Բ��
                        g_point_num--;
                    }
                    pnoindex--;
                    pno = poldno[pnoindex];
                    // if ((g_jog_enpoint > g_jog_stpoint) && (para == 200))
                    //     g_jog_enpoint = g_jog_stpoint;

                    g_CodeEn = 1;
                    g_imgpara = 0;
                    g_ImgPoint = 0;
                    g_ImgMove = 0;
                    g_IsLocal = 0;

                    break;

                case KEY_MOIMAGE2_ZOOMOUT:
                    if (Scale > 0)
                    {
                        Scale -= 10;
                        dis_updata = 2;
                    }
                    else
                    {
                        if (bResetCaonvas)
                        {
                            bResetCaonvas = 0;
                            xo = patConvas.conOrgX;
                            yo = patConvas.conOrgY;
                            Scale = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            dis_updata = 2;
                        }
                    }

                    break;
                case KEY_MOIMAGE2_ZOOMIN:
                    Scale += 10;
                    dis_updata = 2;
                    bResetCaonvas = 1;
                    break;

                case KEY_MOIMAGE2_IMAGETOUCH:
                    if (beeponf > 1)
                    {
                        if ((abs(keyxpct - okeyxpct) <= 10) && (abs(keyypct - okeyypct) <= 10) && (abs(keyxpct - stkeyxpct) <= 10) && (abs(keyypct - stkeyypct) <= 10) && (abs(okeyxpct - stkeyxpct) <= 10) && (abs(okeyypct - stkeyypct) <= 10) && (beeponf == 10))
                        {
                            xo = CONVAS_ORG_PX4;
                            yo = CONVAS_ORG_PY4;
                            Scale = 0;
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                            Beep_On(BELLT);
                        }
                        else
                        {
                            xo += (keyxpct - okeyxpct);
                            yo += (keyypct - okeyypct);
                            okeyxpct = keyxpct;
                            okeyypct = keyypct;
                        }
                    }
                    else
                    {
                        okeyxpct = keyxpct;
                        okeyypct = keyypct;
                        stkeyxpct = keyxpct;
                        stkeyypct = keyypct;
                    }

                    if (IMAGEJOG)
                    {
                        keyxpos = keyxpct;
                        keyypos = keyypct;
                        keyxdis = keyd[KEY_MOIMAGE_IMAGETOUCH].xpis;
                        keyydis = keyd[KEY_MOIMAGE_IMAGETOUCH].ypis;
                    }

                    dis_updata = 2;
                    bResetCaonvas = 1;
                    break;
                }
            }

            keyoff = 0;
        }
        // /[�����ɿ���ֵ����*end]**********************************************************

        // /[����������ʾ��Ϣ����*begin]**********************************************************
        if ((keyon) && (keydata == 0)) //����������ʾ��Ϣ����
        {

            if (keyon == KEY_MOIMAGE2_SCRNSVR || keyon == KEY_MOIMAGE2_IMAGETOUCH) //����Ҫ��ʾ�İ���
            {
            }
            else //���水��
            {
                DrawButtonOff(keyd[keyon], keybmpind[keyon].border[0]);
            }
            keyon = 0;
            beeponf = 0;
            BmpFillRect(TITLE_XPOS, SAFE_DIST4, LCD_TIME_WIDTH_LIMIT - TITLE_XPOS, TYPE_SIZE4); //��ʾ��P1
            Draw24x24Str(TITLE_XPOS, SAFE_DIST4, MEM_TITLE_ADD + rect[0].index[0] * TitleLengthMax, SheetPage, rect[0].index[0], rect[0].colorT[0]);
        }
        // /[����������ʾ��Ϣ����*end]**********************************************************

        // /[��ʱ1s��Ϣ����*begin]**********************************************************
        if ((Timerms > 1000))
        {
            Timerms = 0;
        }
        // /[��ʱ1s��Ϣ����*end]**********************************************************

        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*begin]**********************************************************
        if (key_on_flag1)
            ErrorMsg(MENU_MOIMAGE);
        // /[�������ܰ�����Ϣ����ʱ����ʱ��ⱨ����Ϣ*end]**********************************************************

        // /[������δ����Ϣ����,����������Ϣ����������*begin]**********************************************************
        if (beeponf == 0)
        {
            ErrorMsg(MENU_MOIMAGE);
        }
        // /[������δ����Ϣ����,����������Ϣ����������*end]**********************************************************

        // /[���ƻ���ͼ��*begin]**********************************************************
        if (dis_updata)
        {
            if (dis_updata > 1)
            {
                if (xo < POSI.x0)
                {
                    xo = POSI.x0;
                }
                if (xo > POSI.x1)
                {
                    xo = POSI.x1;
                }
                if (yo < POSI.y0)
                {
                    yo = POSI.y0;
                }
                if (yo > POSI.y1)
                {
                    yo = POSI.y1;
                }
            }

            if (plastno == MENU_CURSOR)
            {
                i = 4;
                patRulesTmp = PATRULES_VIEW_SIMPLE;
            }
            else if (plastno == MENU_CONVERT1)
            {
                i = 8;
                patRulesTmp = PATRULES_VIEW_SIMPLE;
            }
            else if ((para == 201) || (para == 200) || (para == 220))
            {
                i = 6;
                patRulesTmp = PATRULES_VIEW_SIMPLE;
            }
            else if ((para == 211) || (para == 210))
            {
                i = 7;
                patRulesTmp = PATRULES_JOG_A_X;
            }
            else
            {
                i = 5;
                patRulesTmp = PATRULES_VIEW_SIMPLE;
            }
            if (i == 4) //���
            {
                pat_setPatRule(patRulesTmp);
                pat_clearConvas(patRule.h_patConvasIndex);
                pat_drawPattern(PATRULES_VIEW_SIMPLE, Scale, xo, yo, ((g_GreatPatType == 1) ? PAT_MIT : PAT_SEW), F_FILELEN, 0, 0, NULL);
            }
            else
            {
                // Draw_Pattern(i, Scale, xo, yo, F_FILETYPE, (F_FILELEN), keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                pat_setPatRule(patRulesTmp);
                pat_clearConvas(patRule.h_patConvasIndex);
                pat_drawPattern(patRulesTmp, Scale, xo, yo, F_FILETYPE, F_FILELEN, keyxpos + keyxdis, keyypos + keyydis, &tmpfeed);
                if (IMAGEJOG)
                {
                    keyxpos = 0;
                    keyypos = 0;
                    keyxdis = 0;
                    keyydis = 0;
                    // if (para != MOIMAGE_VIEW)
                    // {
                    //     dis_updata1 = 1;
                    //     dis_updata3 = 1;
                    // }
                }
            }

            POSI.x0 = (g_patd.patiX - g_patd.patmX) * g_patd.scale / 2000 - (CONVAS_X4 / 2) + CONVAS_ORG_PX4;
            POSI.x1 = (g_patd.patmX - g_patd.patiX) * g_patd.scale / 2000 + (CONVAS_X4 / 2) + CONVAS_ORG_PX4;
            POSI.y0 = (g_patd.patiY - g_patd.patmY) * g_patd.scale / 2000 - (CONVAS_Y4 / 2) + CONVAS_ORG_PY4;
            POSI.y1 = (g_patd.patmY - g_patd.patiY) * g_patd.scale / 2000 + (CONVAS_Y4 / 2) + CONVAS_ORG_PY4;

            dis_updata = 0;
        }
        // /[���ƻ���ͼ��*end]**********************************************************

        // /[Code��ťͼ��ˢ��*begin]**********************************************************
        if (chCodeEnTemp != g_CodeEn)
        {
            chCodeEnTemp = g_CodeEn;

            DrawButton(keyd[KEY_MOIMAGE2_CODE], (U16 *)(ADD_ICONBMP),
                       (g_CodeEn) ? keybmpind[KEY_MOIMAGE2_CODE].border[1] : keybmpind[KEY_MOIMAGE2_CODE].border[0],
                       (g_CodeEn) ? keybmpind[KEY_MOIMAGE2_CODE].icon[1] : keybmpind[KEY_MOIMAGE2_CODE].icon[0]);
        }
        // /[Code��ťͼ��ˢ��*end]**********************************************************

        // /[ҳ���л�*begin]**********************************************************
        if (pno != MENU_MOIMAGE)
        {
            plastno = MENU_MOIMAGE;
            break;
        }

        // /[ҳ���л�*end]**********************************************************

        updateViewStatus();

        wdt();
    }
}
