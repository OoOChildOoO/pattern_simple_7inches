//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  menu.h									*
//*  ��   ��  ��:  OUYANGQINGJIANG							*
//*  �� �� �� ��:  2015��10��01��							*
//*  ����޸�����: 2016��03��01�� 							*
//*---------------------------------------------------------*
//*  ��������: 												*
//*---------------------------------------------------------*
//*  Ӳ������  PCB:											*
//*			   CPU:											*
//*---------------------------------------------------------*
//*  �������: �������Keil Vision4 V4.74					*
//*  ���													*
//*	 														*
//*															*
//*---------------------------------------------------------*
//*  �汾�޶�: 												*
//*---------------------------------------------------------*

#ifndef __MENU_H
#define __MENU_H

#include "stm32cpu.h"
#include "lcddisp.h"
#include "color.h"

#define BMP_H 311  //ԭ��ͼƬ
#define BMP_H2 312 //��ԭ��ͼƬ
#define BMP_P 313  //��ѹ��ͼƬ
#define BMP_U 314  //����ͣͼƬ
#define BMP_D 315  //����ͣͼƬ
#define BMP_J 316  //����ͼƬ
#define BMP_C 317  //����ͼƬ
#define BMP_T 318  //����ͼƬ
#define BMP_F1 319 //����1ͼƬ
#define BMP_F2 320 //����2ͼƬ
#define BMP_F3 321 //����3ͼƬ
#define BMP_F4 322 //����4ͼƬ
#define BMP_F5 323 //����5ͼƬ
#define BMP_F6 324 //����6ͼƬ
#define BMP_F7 325 //����7ͼƬ
#define BMP_F8 326 //����8ͼƬ
#define BMP_FX 327 //����9ͼƬ
/****************���ָ������С***************/
#define CODE_BMP_X 14
#define CODE_BMP_Y 14
#define CODE_BMP_HX 7
#define CODE_BMP_HY 7

/****************�����ߴ�***************/
#define TYPE_SIZE1 12
#define TYPE_SIZE2 15
#define TYPE_SIZE3 20
#define TYPE_SIZE4 24
#define TYPE_SIZE5 32
#define TYPE_SIZE6 36
#define TYPE_SIZE7 40
#define TYPE_SIZE8 48
#define TYPE_SIZE9 64
#define TYPE_SIZE10 72
#define TYPE_SIZE11 75
#define TYPE_SIZE12 80
#define TYPE_SIZE13 96
#define TYPE_SIZE14 100
#define TYPE_SIZE15 120
#define TYPE_SIZE16 125
#define TYPE_SIZE17 150
#define TYPE_SIZE18 200
#define TYPE_SIZE19 250
#define TYPE_SIZE20 8
#define TYPE_SIZE21 50
#define TYPE_SIZE22 300
#define TYPE_SIZE23 350
#define TYPE_SIZE24 400
#define TYPE_SIZE25 500
#define TYPE_SIZE26 16
#define TYPE_SIZE27 34
#define TYPE_SIZE28 95
#define TYPE_SIZE29 352

#define TYPE_SIZE30 92
#define TYPE_SIZE31 260

#define TYPE_SIZE32 220

#define TYPE_SIZE33 600
#define TYPE_SIZE34 700
#define TYPE_SIZE35 56
/****************��ȫ���ߴ�***************/
#define SAFE_DIST1 8
#define SAFE_DIST2 1
#define SAFE_DIST3 2
#define SAFE_DIST4 4
#define SAFE_DIST5 5
#define SAFE_DIST6 10
#define SAFE_DIST7 15
#define SAFE_DIST8 20
#define SAFE_DIST9 25
#define SAFE_DIST10 30
#define SAFE_DIST11 40
#define SAFE_DIST12 6
#define SAFE_DIST13 50
#define SAFE_DIST14 100
#define SAFE_DIST15 12
/****************������׼�ߴ�***************/
//extern const U16 PC_bmp[16];
#define KEY_X1 64
#define KEY_Y1 64
#define KEY_X2 48
#define KEY_Y2 48
#define KEY_X3 118
#define KEY_Y3 64
#define KEY_X4 100
#define KEY_Y4 48
#define KEY_X5 72
#define KEY_Y5 48
#define KEY_X6 56
#define KEY_Y6 56
#define KEY_X7 88
#define KEY_Y7 88
#define KEY_X8 128
#define KEY_Y8 48
#define KEY_X9 240
#define KEY_Y9 80
#define KEY_X10 152
#define KEY_Y10 88
#define KEY_X11 64
#define KEY_Y11 32
#define KEY_X12 136
#define KEY_Y12 64
#define KEY_X13 200
#define KEY_Y13 40
#define KEY_X14 200
#define KEY_Y14 72
#define KEY_X15 120
#define KEY_Y15 56
#define KEY_X16 104
#define KEY_Y16 64
#define KEY_X17 64
#define KEY_Y17 56

#define KEY_X99 152
#define KEY_Y99 88
/****************������ɫ���***************/
#define KEY_COLOR_STYLE KEY_Silver
/****************����X����***************/
#define TYPE_X1 SAFE_DIST1
#define TYPE_X2 80
#define TYPE_X3 152
#define TYPE_X4 224
#define TYPE_X5 296
#define TYPE_X6 368
#define TYPE_X7 440
#define TYPE_X8 512
#define TYPE_X9 584
#define TYPE_X10 656
#define TYPE_X11 728

#define TYPE_X12 400
#define TYPE_X13 0
#define TYPE_X14 100
#define TYPE_X15 566
#define TYPE_X16 150
#define TYPE_X17 40
#define TYPE_X18 50
#define TYPE_X19 180

#define TYPE_X20 174
#define TYPE_X21 199
#define TYPE_X22 230
#define TYPE_X23 567
#define TYPE_X24 494

#define TYPE_X25 323
#define TYPE_X26 273
#define TYPE_X27 537

#define TYPE_X28 142
#define TYPE_X29 277
#define TYPE_X30 398
#define TYPE_X31 555
#define TYPE_X32 662

#define TYPE_X33 60
#define TYPE_X34 676

#define TYPE_X35 30

#define TYPE_X36 600
#define TYPE_X37 615
#define TYPE_X38 690

#define TYPE_X40 45
#define TYPE_X41 187
#define TYPE_X42 329
#define TYPE_X43 471
#define TYPE_X44 613

#define TYPE_X45 138

#define TYPE_X46 606

#define TYPE_X47 119
#define TYPE_X48 155
#define TYPE_X49 179
#define TYPE_X50 204
#define TYPE_X51 68
#define TYPE_X52 558

#define TYPE_X53 422

#define TYPE_X54 493


#define KEY_X87 152
#define KEY_X88 64
#define TYPE_X89 119
#define TYPE_X90 SAFE_DIST8
#define TYPE_X91 280
#define TYPE_X92 540
#define TYPE_X93 SAFE_DIST10
#define TYPE_X94 606
#define TYPE_X99 138




/****************����Y����***************/
#define TYPE_Y1 35
#define TYPE_Y2 40
#define TYPE_Y3 50
#define TYPE_Y4 408 //LCD_HEIGHT - SAFE_DIST1 - KEY_Y1
#define TYPE_Y5 336
#define TYPE_Y6 60
#define TYPE_Y7 70
#define TYPE_Y8 80
#define TYPE_Y9 90
#define TYPE_Y10 100
#define TYPE_Y11 1
#define TYPE_Y12 240
#define TYPE_Y13 115
#define TYPE_Y14 164
#define TYPE_Y15 278

#define TYPE_Y16 77
#define TYPE_Y17 114
#define TYPE_Y18 156
#define TYPE_Y19 205
#define TYPE_Y20 254
#define TYPE_Y21 303
#define TYPE_Y22 352

#define TYPE_Y23 264
#define TYPE_Y24 192

#define TYPE_Y25 120

#define TYPE_Y26 182
#define TYPE_Y27 88
#define TYPE_Y28 208
#define TYPE_Y29 61

#define TYPE_Y30 366

#define TYPE_Y31 81
#define TYPE_Y32 186
#define TYPE_Y33 291
#define TYPE_Y34 396
#define TYPE_Y35 335

#define TYPE_Y36 185

#define TYPE_Y40 120
#define TYPE_Y41 255
#define TYPE_Y42 145
#define TYPE_Y43 280

#define TYPE_Y44 45

#define TYPE_Y45 110
#define TYPE_Y46 128

#define TYPE_Y47 243
#define TYPE_Y48 292
#define TYPE_Y49 341
#define TYPE_Y50 390
#define TYPE_Y51 55
#define TYPE_Y52 97

#define TYPE_Y54 188

#define KEY_Y87 88
#define KEY_Y88 24
#define TYPE_Y92 390 
#define TYPE_Y93 341 
#define TYPE_Y94 292
#define TYPE_Y95 243
#define TYPE_Y96 65
#define TYPE_Y97 45
#define TYPE_Y98 110

/****************������С�뻭��λ��***************/
//������
#define CONVAS_X1 490
#define CONVAS_Y1 300
#define CONVAS_PX1 TYPE_X2
#define CONVAS_PY1 TYPE_Y7
#define CONVAS_ORG_PX1 ((CONVAS_X1 + 2 * CONVAS_PX1) / 2)
#define CONVAS_ORG_PY1 ((CONVAS_Y1 + 2 * CONVAS_PY1) / 2)
#define CONVAS_COLOR1 Color_Black

//��ȡ�����б����
#define CONVAS_X2 120
#define CONVAS_Y2 60
#define CONVAS_COLOR2 Color_Black
#define CONVAS_PX2_1 30
#define CONVAS_PY2_1 TYPE_Y3
#define CONVAS_PX2_2 205
#define CONVAS_PY2_2 TYPE_Y3
#define CONVAS_PX2_3 390
#define CONVAS_PY2_3 TYPE_Y3
#define CONVAS_PX2_4 570
#define CONVAS_PY2_4 TYPE_Y3
#define CONVAS_PX2_5 30
#define CONVAS_PY2_5 TYPE_Y14
#define CONVAS_PX2_6 205
#define CONVAS_PY2_6 TYPE_Y14
#define CONVAS_PX2_7 390
#define CONVAS_PY2_7 TYPE_Y14
#define CONVAS_PX2_8 570
#define CONVAS_PY2_8 TYPE_Y14
#define CONVAS_PX2_9 30
#define CONVAS_PY2_9 TYPE_Y15
#define CONVAS_PX2_10 205
#define CONVAS_PY2_10 TYPE_Y15
#define CONVAS_PX2_11 390
#define CONVAS_PY2_11 TYPE_Y15
#define CONVAS_PX2_12 570
#define CONVAS_PY2_12 TYPE_Y15

#define CONVAS_ORG_PX2_1 ((CONVAS_X2 + 2 * CONVAS_PX2_1) / 2)
#define CONVAS_ORG_PY2_1 ((CONVAS_Y2 + 2 * CONVAS_PY2_1) / 2)
#define CONVAS_ORG_PX2_2 ((CONVAS_X2 + 2 * CONVAS_PX2_2) / 2)
#define CONVAS_ORG_PY2_2 ((CONVAS_Y2 + 2 * CONVAS_PY2_2) / 2)
#define CONVAS_ORG_PX2_3 ((CONVAS_X2 + 2 * CONVAS_PX2_3) / 2)
#define CONVAS_ORG_PY2_3 ((CONVAS_Y2 + 2 * CONVAS_PY2_3) / 2)
#define CONVAS_ORG_PX2_4 ((CONVAS_X2 + 2 * CONVAS_PX2_4) / 2)
#define CONVAS_ORG_PY2_4 ((CONVAS_Y2 + 2 * CONVAS_PY2_4) / 2)
#define CONVAS_ORG_PX2_5 ((CONVAS_X2 + 2 * CONVAS_PX2_5) / 2)
#define CONVAS_ORG_PY2_5 ((CONVAS_Y2 + 2 * CONVAS_PY2_5) / 2)
#define CONVAS_ORG_PX2_6 ((CONVAS_X2 + 2 * CONVAS_PX2_6) / 2)
#define CONVAS_ORG_PY2_6 ((CONVAS_Y2 + 2 * CONVAS_PY2_6) / 2)
#define CONVAS_ORG_PX2_7 ((CONVAS_X2 + 2 * CONVAS_PX2_7) / 2)
#define CONVAS_ORG_PY2_7 ((CONVAS_Y2 + 2 * CONVAS_PY2_7) / 2)
#define CONVAS_ORG_PX2_8 ((CONVAS_X2 + 2 * CONVAS_PX2_8) / 2)
#define CONVAS_ORG_PY2_8 ((CONVAS_Y2 + 2 * CONVAS_PY2_8) / 2)
#define CONVAS_ORG_PX2_9 ((CONVAS_X2 + 2 * CONVAS_PX2_9) / 2)
#define CONVAS_ORG_PY2_9 ((CONVAS_Y2 + 2 * CONVAS_PY2_9) / 2)
#define CONVAS_ORG_PX2_10 ((CONVAS_X2 + 2 * CONVAS_PX2_10) / 2)
#define CONVAS_ORG_PY2_10 ((CONVAS_Y2 + 2 * CONVAS_PY2_10) / 2)
#define CONVAS_ORG_PX2_11 ((CONVAS_X2 + 2 * CONVAS_PX2_11) / 2)
#define CONVAS_ORG_PY2_11 ((CONVAS_Y2 + 2 * CONVAS_PY2_11) / 2)
#define CONVAS_ORG_PX2_12 ((CONVAS_X2 + 2 * CONVAS_PX2_12) / 2)
#define CONVAS_ORG_PY2_12 ((CONVAS_Y2 + 2 * CONVAS_PY2_12) / 2)

//ͼƬ������Ϣ���������
#define CONVAS_X3 510
#define CONVAS_Y3 340
#define CONVAS_PX3 113
#define CONVAS_PY3 117
#define CONVAS_ORG_PX3 ((CONVAS_X3 + 2 * CONVAS_PX3) / 2)
#define CONVAS_ORG_PY3 ((CONVAS_Y3 + 2 * CONVAS_PY3) / 2)
#define CONVAS_COLOR3 Color_Black

//ͼƬԤ��
#define CONVAS_X4 510
#define CONVAS_Y4 340
#define CONVAS_PX4 145
#define CONVAS_PY4 40
#define CONVAS_ORG_PX4 ((CONVAS_X4 + 2 * CONVAS_PX4) / 2)
#define CONVAS_ORG_PY4 ((CONVAS_Y4 + 2 * CONVAS_PY4) / 2)
#define CONVAS_COLOR4 Color_Black

//���������
#define CONVAS_X5 450
#define CONVAS_Y5 300
#define CONVAS_PX5 80
#define CONVAS_PY5 100
#define CONVAS_ORG_PX5 ((CONVAS_X5 + 2 * CONVAS_PX5) / 2)
#define CONVAS_ORG_PY5 ((CONVAS_Y5 + 2 * CONVAS_PY5) / 2)
#define CONVAS_COLOR_B Color_Black
#define CONVAS_COLOR_T Color_White

//�޸ġ��任������
#define CONVAS_X6 390
#define CONVAS_Y6 260
#define CONVAS_PX6 390
#define CONVAS_PY6 98
#define CONVAS_ORG_PX6 ((CONVAS_X6 + 2 * CONVAS_PX6) / 2)
#define CONVAS_ORG_PY6 ((CONVAS_Y6 + 2 * CONVAS_PY6) / 2)

//�޸ġ��任ѡ�ν���
#define CONVAS_X7 510
#define CONVAS_Y7 340
#define CONVAS_PX7 200
#define CONVAS_PY7 40
#define CONVAS_ORG_PX7 ((CONVAS_X7 + 2 * CONVAS_PX7) / 2)
#define CONVAS_ORG_PY7 ((CONVAS_Y7 + 2 * CONVAS_PY7) / 2)

//�޸ġ��任�ƶ�����-��
#define CONVAS_X8 510
#define CONVAS_Y8 340
#define CONVAS_PX8 40
#define CONVAS_PY8 40
#define CONVAS_ORG_PX8 ((CONVAS_X8 + 2 * CONVAS_PX8) / 2)
#define CONVAS_ORG_PY8 ((CONVAS_Y8 + 2 * CONVAS_PY8) / 2)

//�޸ġ��任�ƶ�����-С
#define CONVAS_X9 450
#define CONVAS_Y9 300
#define CONVAS_PX9 40
#define CONVAS_PY9 90
#define CONVAS_ORG_PX9 ((CONVAS_X9 + 2 * CONVAS_PX9) / 2)
#define CONVAS_ORG_PY9 ((CONVAS_Y9 + 2 * CONVAS_PY9) / 2)
/****************ͼ���С***************/
//������
#define IMAGE_X1 420
#define IMAGE_Y1 280

//��ȡ�����б����
#define IMAGE_X2 100
#define IMAGE_Y2 50

//ͼƬ������Ϣ���������
#define IMAGE_X3 450
#define IMAGE_Y3 300

//ͼƬ��Ԥ��
#define IMAGE_X4 450
#define IMAGE_Y4 300

//���������
#define IMAGE_X5 420
#define IMAGE_Y5 280

//�޸ġ��任������
#define IMAGE_X6 360
#define IMAGE_Y6 240

//�޸ġ��任ѡ�ν���
#define IMAGE_X7 450
#define IMAGE_Y7 300

//�޸ġ��任�ƶ�����-��
#define IMAGE_X8 450
#define IMAGE_Y8 300

//�޸ġ��任�ƶ�����-С
#define IMAGE_X9 420
#define IMAGE_Y9 280
/****************����߿�ߴ�***************/
#define SAFE_BORDER_L1 SAFE_DIST1
#define SAFE_BORDER_R1 LCD_WIDTH - SAFE_DIST1
#define SAFE_BORDER_U1 TITLE_YLEN + SAFE_DIST1
#define SAFE_BORDER_D1 LCD_HEIGHT - SAFE_DIST1

#define SAFE_BORDER_U2 TITLE_YLEN + 3
#define SAFE_BORDER_D2 SAFE_BORDER_D1 - KEY_Y1

/****************���ƻ�������ɫ***************/
#define COLOR_ORG Color_Auqamarin

#define COLOR_CROSS RGB(255,20,100)
#define COLOR_CROSS_BEF RGB(255,120,190)

#define COLOR_LINE_EMPTY Color_DimGray
#define COLOR_LINE_H2H2 Color_DimGray
#define COLOR_LINE_ED2H Color_Gray
#define COLOR_LINE_FINISH Color_Red 
#define COLOR_LINE_SEW RGB(200,200,255)
#define COLOR_LINE_SEW_H RGB(200,200,255)
#define COLOR_LINE_SEW_MD1 RGB(120,120,255)
#define COLOR_LINE_SEW_MD2 RGB(80,80,200)
#define COLOR_LINE_SEW_L RGB(50,50,150)
#define COLOR_LINE_CHANGE Color_Orange
#define COLOR_LINE_SELECT RGB(200,50,50) 

#define COLOR_PIN_FINISH Color_Red 
#define COLOR_PIN_SELECT Color_Red  
#define COLOR_PIN_SEW Color_Yellow

#define COLOR_SELECT_P RGB(255,10,20)  
#define COLOR_SELECT_E RGB(255,80,120) 
#define COLOR_SELECT_S RGB(255,120,190)
#define COLOR_SELECT_TOP Color_Green

#define COLOR_PART_N  Color_Green//�����ε���ɫ
#define COLOR_CTRL RGB(255,80,120)//���������Ƶ����ɫ

#define COLOR_CODE_C Color_Green

#define COLOR_CONVAS Color_Black
#define COLOR_SUBLINE Color_White

#define COLOR_BG Color_SkyBlue
#define COLOR_TITLE_BG Color_Black
#define COLOR_TITLE_STR Color_White
/****************����ͼ������***************/
#define PAT_P1 0//������
#define PAT_P1_ZOOM 1//������Ŵ���С
#define PAT_MODIFY 2//�����޸ĵ���ҳ��
#define PAT_CONVENT 3//�����任����ҳ��
#define PAT_CREATE 9//����������ҳ��
#define PAT_READ 10//��ȡ����ҳ��
#define PAT_WRITE 11//д����
#define PAT_READ2 12//��ȡ����ҳ��2
#define PAT_FILE 13//�ļ�����
#define PAT_FILE_VIEW 14//�ļ�����-����ͼ

#define PAT_VIEW_P1 20//Ԥ������-P1
#define PAT_VIEW_READ 21//Ԥ������-��ȡ
#define PAT_VIEW_CREATE 4//Ԥ������-���-ͼ��
#define PAT_VIEW_MODIFY1 5//Ԥ������-�޸�-�����޸ġ��任
#define PAT_VIEW_MODIFY2 6//Ԥ������-�޸�-�����޸ģ�����
#define PAT_VIEW_MODIFY3 7//����Ԥ������-�޸�-�����ƶ�,����
#define PAT_VIEW_CONVENT 8//Ԥ������-�任-ͼ��Ԥ��

//19 ��ͼ��g_feedc == g_feedsum��
//18 ��ͼ��g_feedc != g_feedsum��
//17 ��ͼ��dis_updataP != 2��



#define NPAT_STA_CHANGE 1//�޸�-�޸�����
#define NPAT_PIN_DEL 6//�޸�-��ɾ��
#define NPAT_PART_MOVE 8   //�޸�-���ƶ�
#define NPAT_PART_CHANGE 9 //�޸�-���޸�
#define NPAT_PART_ADD 12    //�޸�-�β���

#define NPAT_MODI1_MODE1 0//���������,��׷��,������
#define NPAT_MODI1_MODE2 5//��ɾ��
#define NPAT_MODI1_MODE3 7//�����ƶ�/��������
#define NPAT_MODI1_MODE4 10//��������,g_num_mov_flag,g_jogmode=0
#define NPAT_MODI1_MODE5 11//��������,g_num_mov_flag,g_jogmode=1
#define NPAT_MODI1_MODE6 26//���

#define NPAT_ST_ON 3//�޸�-��ʼ��-���뿪��
#define NPAT_ST_OFF 4//�޸�-��ʼ��-����ر�
#define NPAT_ED_ON 13//�޸�-������-���뿪��
#define NPAT_ED_OFF 14//�޸�-������-����ر�
#define NPAT_MU_ON 15//�޸�-��ѡ��-���뿪��
#define NPAT_MU_OFF 16//�޸�-��ѡ��-����ر�

#define NPAT_CREATE_ADD 17//���-��ʾָ��
#define NPAT_CREATE_ADD2 18//���-����
#define NPAT_CREATE_FIN 19//���-���

#define KEYX1 64
#define KEYY1 64
#define KEYX2 126
#define KEYY2 64
#define KEYX3 128
#define KEYY3 64
#define KEYX4 160
#define KEYY4 96
#define KEYX5 255
#define KEYY5 80
#define KEYX6 96
#define KEYY6 48
#define KEYX7 96
#define KEYY7 64
#define KEYX8 80
#define KEYY8 64
#define KEYX9 80
#define KEYY9 80
#define KEYX10 96
#define KEYY10 96
#define KEYX11 144
#define KEYY11 48
#define KEYX12 128
#define KEYY12 48
#define KEYX13 192
#define KEYY13 48
#define KEYX14 512
#define KEYY14 64
#define KEYX15 80
#define KEYY15 40
#define KEYX16 86
#define KEYY16 48
#define KEYX17 48
#define KEYY17 48
#define KEYX18 40
#define KEYY18 40
#define KEYX19 84
#define KEYY19 84
#define KEYX20 80
#define KEYY20 80
#define KEYX21 56
#define KEYY21 56
#define KEYX22 96
#define KEYY22 56
#define KEYX23 80
#define KEYY23 56
#define KEYX24 56
#define KEYY24 56
#define KEYX25 84
#define KEYY25 84
#define KEYX27 84
#define KEYY27 84
#define KEYX28 118
#define KEYY28 64
#define KEYX29 136
#define KEYY29 64
#define KEYX30 104
#define KEYY30 64
#define KEYX31 88
#define KEYY31 88
#define KEYX32 108
#define KEYY32 64
#define KEYX33 72
#define KEYY33 72


#define USEFLASH 0
#define USESD 1
#define USEUSB 2

#define MENU_SCRNSVR 1
#define MENU_TOUCHCHECK 1000

#define MENU_P1 2
#define MENU_IMAGE 21
#define MENU_LINEFORCE 22
#define MENU_PFOOT 23
#define MENU_P2 3
#define MENU_COUNTER 31
#define MENU_WIND 32
#define MENU_MENU 4
#define MENU_READ 5
#define MENU_BARCODE 51
#define MENU_DIRREAD 52
#define MENU_WRITE 6
#define MENU_WRITE1 61
#define MENU_INPUT 7
#define MENU_CURSOR 71
#define MENU_INMENU 72
#define MENU_INHELP 73
#define MENU_PITCH 74
#define MENU_SKIPJOG 75
#define MENU_METHOD 76
#define MENU_MULTI 77
#define MENU_ZIGZAG 78
#define MENU_TACKING 79
#define MENU_CODE 710
#define MENU_EXCODE 711
#define MENU_MOIMAGE 712
#define MENU_CALLFILE 713
#define MENU_EXCODE1 714
#define MENU_READCOVN 715
#define MENU_TACKING1 716
#define MENU_DIRCALLFILE 717//����
#define MENU_MODIFY 8
#define MENU_MODIFYADD 81
#define MENU_MODIFY1 82
#define MENU_MODIFY2 83
#define MENU_MODIFY3 84
#define MENU_PARAMODIFY 85
#define MENU_CONVERT 9
#define MENU_CONVERT1 91
#define MENU_PROG 10
#define MENU_PROGMSG11 107
#define MENU_PROFRAME 101
#define MENU_PROPARA 102
#define MENU_PROPASS 103
#define MENU_DIFFERENT 104
#define MENU_FPROG 105
#define MENU_FPROGS 106

#define MENU_FUNCTION 11
#define MENU_FTCOPY 111
#define MENU_FTNEEDLE 112
#define MENU_FTFORMAT 113
#define MENU_FTGROUP 114//��ϻ���-������
#define MENU_FTGROUPREAD1 1141//��ϻ���-��ȡ�ӻ���
#define MENU_FTGROUPREAD2 1142//��ϻ���-��ȡ��ϻ���
#define MENU_FTGROUPWRITE 1143//��ϻ���-д��ϻ���
#define MENU_FTDIRREAD 1144//��ϻ���-ֱ�Ӷ�ȡ�ӻ���
#define MENU_FTSYSVERSION 115
#define MENU_FTSCREENSET 116
#define MENU_FTSCREENBOOTSET 1161

#define MENU_FTSYSRESTORE 117
#define MENU_FTSYSBACKUP 118
#define MENU_FTUSER 119
#define MENU_FTERRRECORD 1110
#define MENU_FTRUNRECORD 1111
#define MENU_FTNETWORD 1112
#define MENU_FTNETCIG 11120
#define MENU_FTSETIP 11121
#define MENU_FTNETRW 11122
#define MENU_FTCHAT 11123

#define MENU_FTRESET 1113
#define MENU_FTVIEWPARA 1114

#define MENU_FTSERVO 1115
#define MENU_FTSTEPPER 1116
#define MENU_FTFILES 1117
#define MENU_FTKEYBOARD 1118
#define MENU_FTFBAG 1119
#define MENU_FTFBAG1 1120
#define MENU_FTFBAG2 1121

#define MENU_IOTEST 12
#define MENU_INPUTSTATUS 121
#define MENU_OUTPUTSTATUS 122
#define MENU_SENSORSTATUS 123
#define MENU_INPUTSET 124
#define MENU_OUTPUTSET 125
#define MENU_ANALOGSET 126

#define MENU_HELP 13
#define MENU_SPTCNT 131
#define MENU_VIDEO 132
#define MENU_PLAYAVI 1320
#define MENU_BOOK 133

#define MENU_RWHELP 14
#define MENU_RWHELP1 141

#define MENU_THREAD 15

extern const U16 keypicdata[];
extern unsigned short const PARA_DATA1[];

extern volatile U8 usepageno;
extern volatile U16 pno;
extern volatile U16 plastno;
extern volatile U16 pnoindex, tmppnoindex;
extern volatile U16 poldno[10];
extern volatile U16 DREG[];
extern volatile U16 hleppara, g_para0, g_para1, g_imgpara;
extern volatile U16 g_publicbuf[]; // �����Ĵ���
extern volatile U8 buttonflag[];
extern volatile U32 g_pno_temp;
extern volatile U8 key_on_flag1;
extern volatile U8 g_data_flash; //�������ݸ���

//extern void Menu_P1(void);				// �������˵�1
//extern void Menu_P2(void);				// �������˵�2
//extern void Menu_Menu(void);			// Ŀ¼����
//extern void Menu_Read(void);			// ��ȡģ�߻���
//extern void Menu_Write(void);			// ����ģ�߻���
//extern void Menu_Write1(void);			//д��ģʽѡ��
//extern void Menu_Image(void);			// ������ʾ�˵�
//extern void Menu_Barcode(void);			// ���������
//extern void Menu_DirRead(void);			//ֱ�Ӷ�ȡĿ¼
//extern void Menu_Counter(U16 para);		// ������
//extern void Menu_Wind(void);			// ������ʾ�˵�
//extern void Menu_SkipJog(void);			// ��Ծ�綯����
//extern void Menu_ViewRunPara(void);	//�鿴ϵͳ���в���
//extern void Menu_Scrnsvr(U8 tt);		// ��������

//extern void Menu_RwHelp(U16 hleppara);	// ��д��������
//extern void Menu_RwHelp1(void);

//extern void Menu_CtInput(void);			// ��������
//extern void Menu_CtCursor(void);			// ���ɻ���
//extern void Menu_CtInputMenu(void);		// ���빦�ܲ˵�����
//extern void Menu_Ctpitch(void);			// �޸���໭��
//extern void Menu_CtMethod(void);			// ���뷽�����û���
//extern void Menu_Ctmultiple(void);		// ���ط��趨����
//extern void Menu_Ctzigzag(void);			// Z�ַ����ò˵�
//extern void Menu_Cttacking(void);			// �������ò˵�
//extern void Menu_CtCode(void);			// ���븨�����û���
//extern void Menu_CtExCode(void);			// �����������仭��
//extern void Menu_CtEx1Code(void);	// �����������仭��1
//extern void Menu_CtPfootSet(U16 para);		// ��ѹ�����á��޸Ľ���
//extern void Menu_Ctlineforce(U16 para);		// �����������ò˵�
//extern void Menu_CtInHelp(U16 para);			// ��������˵�
//extern void Menu_CtCallFile(void);		// ��������
//extern void Menu_CtReadConv(void);	//������ȡ�任

//extern void Menu_Modify(void);			// �����޸Ļ���
//extern void Menu_ModiAdd(U32 para);		// �綯�˵�
//extern void Menu_Modify1(U32 para);		// ��Ŀ����
//extern void Menu_Modify2(void);			// �ٶ�ָ������
//extern void Menu_Modify3(void);			// ���ӹ�������
//extern void Menu_MoImage(U8 para); 		// �޸�ģ���е�ͼ������

//extern void Menu_Convert(void);			// �����任����
//extern void Menu_Convert1(U32 para);		// �����任�˵�

//extern void Menu_Prog(void);			// ��������
//extern void Menu_ProFrame(void);		// �������ò˵����
//extern void Menu_ProPara(void);			// ������ò˵���������
//extern void Menu_ProPass(void);			// ������ò˵���������
//extern void Menu_ProDifferent(void);	// ������ò˵��鿴�޸Ĺ��Ĳ���
//extern void Menu_FastProg(void);		// ���������޸Ļ���
//extern U16 Menu_SetProgNo(U16 inNo);

//extern void Menu_Iotest(void);			// IO���Ի���
//extern void Menu_Inputstatus(void);		// �����ź�״̬��ʾ
//extern void Menu_Outputstatus(void);	// ����ź�״̬��ʾ
//extern void Menu_Sensorstatus(void);	// ������ź�״̬��ʾ
//extern void Menu_Inputset(void);		// �����ź�����
//extern void Menu_Outputset(void);		// ����ź�����
//extern void Menu_AnalogSet(void);		//ģ���������趨

//extern void Menu_Function(void);		// ���ܻ���
//extern void Menu_FtCopy(void);			// ͼ�ο���
//extern void Menu_FtNeedle(void);		// ��ͣ������
//extern void Menu_FtFormat(void);		// ��ʽ��
//extern void Menu_FtGroup(void);			// ͼ�����
//extern void Menu_FtReadMem(void);		// ͼ����϶��ڴ��ļ�
//extern void Menu_FtReadCombine(void);	// ͼ����϶����ͼ��
//extern void Menu_FtWriteCombine(void);	// ͼ����ϱ������ͼ��
//extern void Menu_FtSysversion(void);	// �汾
//extern void Menu_FtScreenset(void);		// ��ʾ����
//extern void Menu_FtSysrestore(void);	// ϵͳ��ԭ����
//extern void Menu_FtSysbackup(void);		// ϵͳ����
//extern void Menu_FtUser(void);			// �û���¼
//extern void Menu_FtErrrecord(void);		// ������Ϣ��¼
//extern void Menu_FtRunrecord(void);		// ������Ϣ��¼
//extern void Menu_FtNewChat(void);		//��������
//extern void Menu_FtNetword(void);		// ���繤����
//extern void Menu_FtSetIP(U8 inpara);			//���û���IP������
//extern void Menu_FtNetwordRW(U8 para,U8 SelMac);	//���ͽ���Զ������
//extern void Menu_FtReset(void);			// һ����ԭ
//extern void Menu_FtParaCorr(void);		// ������������
//extern void Menu_FtServo(void);			//�ŷ�����
//extern void Menu_FtScreenset(void);		// ��ʾ�����

//extern void Menu_Help(void);			// ����֧�ֻ���
//extern void Menu_HeSupportConnect(U8 para);	// ��������
//extern void Menu_HeVideo(void);			//��Ƶ�̳�
//extern void Menu_HePlayAvi(U16 aviNO);	//����avi
//extern void Menu_HeBook(void);	//˵����

//#define 	DISABSAXIS
#define ZOOMBT (buttonflag[0])    //���Ű���
#define TENSIONBT (buttonflag[1]) //���߰���
#define PFOOTBT (buttonflag[2])   //ѹ�Ű���
#define ORGSUBBT (buttonflag[3])  //��������
#define IMAGEJOG (buttonflag[4])  //ͼ������
#define TESTKEYF (buttonflag[5])  //���԰���

#define MINFILENO 1     // ��С�ļ����
#define MAXFILENO 999   // ����ļ����
#define DREG_PT_INDEX 0 // 16 * 400 = 6400 (��ţ�100 - 499)

#define PREG_PAGE_MAX 256
#define PREG_LENGTH 4
#define PREG_MIN 0
#define PREG_MAX 1
#define PREG_DEFAULT 2
#define PREG_NOW 3

#define MSG1_LENGTH 15
#define MSG2_LENGTH 44
#define MSG3_LENGTH 54
#define MSG4_LENGTH 100
#define MSG4_LENGTH_E 170
#define PMSG2_PARANAME 0
#define PMSG2_MSG 3
#define PMSG2_UNIT 33
#define PMSG2_TYPE 39
#define PMSG2_MIN 40
#define PMSG2_MAX 41
#define PMSG2_MSG3 42
#define PMSG2_HELP 43
#define PMSG3_PARANAME 0
#define PMSG3_MSG 3

#define IOSETNAME_LENGTH 50
#define IOSETNAME_CODE 0
#define IOSETNAME_CHINA 2
#define IOSETNAME_ENGLISH 18
#define INSETPRSUM 81
#define OUTSETPRSUM 73
#define CONTENTMSG_LENGTH 100
#define CONTENTMSG_CHINA 0
#define CONTENTMSG_ENGLISH 20

#define C1_MSG1_LENGTH 20

#define C1_MSG2_LENGTH 60
#define C1_PMSG2_PARANAME 0
#define C1_PMSG2_MSG 3
#define C1_PMSG2_UNIT 50
#define C1_PMSG2_TYPE 56
#define C1_PMSG2_MIN 57
#define C1_PMSG2_MAX 58
#define C1_PMSG2_MSG3 59

#define C1_MSG3_LENGTH 178
#define C1_PMSG3_PARANAME 0
#define C1_PMSG3_MSG 3
//#define		SST_SREG_SADD			0x60700000		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ���ʼ��ַ
//#define		SST_SREG_EADD			0x607EFFFF		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ������ַ(960K)
//#define		SST_SREG_MAXS			0x00078000

//#define		SST_SREG_BACKUP			0x60600000 		//SST���ݴ��޸Ļ����������ݣ��ڸ���ԭ���ݻ�������ǰʹ��
//#define		SST_SREG_PARA_BACKUP	0x606D0000

//#define		SST_SREG_SADD			0xC0900000		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ���ʼ��ַ
//#define		SST_SREG_EADD			0xC09EFFFF		// SST�������޸Ļ���ʱ�������ݵ��ڴ�ռ������ַ(960K)
//#define		SST_SREG_MAXS			0x00078000

//#define		SST_SREG_BACKUP			0xC0800000 		//SST���ݴ��޸Ļ����������ݣ��ڸ���ԭ���ݻ�������ǰʹ��
//#define		SST_SREG_PARA_BACKUP	0xC08D0000

#endif
