/*
 * @Author: your name
 * @Date: 2021-05-14 09:28:01
 * @LastEditTime: 2021-06-23 17:24:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_modify.h
 */
#ifndef _MENU_MODIFY_H
#define _MENU_MODIFY_H

#include "stm32cpu.h"

#define MOIMAGE_VIEW 1      //��Ԥ�� - ��������ϢԤ��
#define MOIMAGE_SKIPJOG 0   //����Ԥ�� - ��������Ϣ����
#define MOIMAGE_CVIEW 50    //��Ԥ��-�������-��Ԥ��

#define MOIMAGE_MOVE 210    //����Ԥ��-�����޸�-���ƶ�
#define MOIMAGE_CHANGE 211  //����Ԥ��-�����޸�-���޸�
#define MOIMAGE_ENSURE1 200 //��Ԥ��-���޸�-ȷ������-ֱ��/Z�ַ�
#define MOIMAGE_ENSURE2 201 //��Ԥ��-���޸�-ȷ������-����/Բ��/����/����
#define MOIMAGE_ENSURE3 220 //��Ԥ��
//200

#define MODIFY_STA_CHANGE 0  //�޸�-����
#define MODIFY_PIN_DEL 1     //�޸�-��ɾ��
#define MODIFY_PIN_ADD 2     //�޸�-�����
#define MODIFY_PIN_MOVE 3    //�޸�-���ƶ�
#define MODIFY_PART_MOVE 4   //�޸�-���ƶ�
#define MODIFY_PART_CHANGE 5 //�޸�-���޸�
#define MODIFY_PART_ADD 9    //�޸�-�β���
#define MODIFY_PITCH 6       //�޸�-����޸�
#define MODIFY_PF 7          ////�޸�-��ѹ�Ÿ߶�
#define MODIFY_LF 8          //�޸�-��������
#define MODIFY_SPEED 15      //�޸�-�����ٶ�
#define MODIFY_CODE_ADD 20   //�޸�-�������
#define MODIFY_CODE_DEL 21   //�޸�-����ɾ��

#define CONVERT_TACKING 26  //�任-����
#define CONVERT_ZIGZAG 27   //�任-Z�ַ�
#define CONVERT_ZOOM 28     //�任-����
#define CONVERT_MIRROR 29   //�任-�Գ�
#define CONVERT_ROTATE 30   //�任-��ת
#define CONVERT_OFFSMULT 31 //�任-���ط�
#define CONVERT_OFFSET 32   //�任-��߷�
#define CONVERT_CHANGE 33   //�任-��λ�Ե�
#define CONVERT_PINANG 34   //�任-��Ƕ�

S32 getScale(U16 id);

U8 getNearPointPos(s16 inx, S16 iny, S16 *outx, S16 *outy);

void Menu_Modify(void);

void Menu_ModiAdd(U32 para);

void Menu_Modify1(U32 para);

void Menu_Modify2(U32 para);

void Menu_Modify3(U32 para);

void Menu_ModifyPatChangeMethod(U32 para);

void sendExCodeCmd(void);

void Menu_MoView(U8 para);

void Menu_MoView2(U8 para);

#endif
