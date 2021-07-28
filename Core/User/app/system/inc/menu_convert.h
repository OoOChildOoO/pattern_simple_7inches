/*
 * @Author: your name
 * @Date: 2021-06-18 14:40:36
 * @LastEditTime: 2021-06-23 17:15:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F429(7inches)\App\app\system\inc\menu_convert.h
 */

#ifndef _MENU_CONVERT_H
#define _MENU_CONVERT_H

#include "stm32cpu.h"

#define CONV_ORGSUB 0   //�任-ԭ������
#define CONV_TACKING 1  //�任-ǰ�󵹷�
#define CONV_ZIG 2      //�任-z�ַ�
#define CONV_SCALE 3    //�任-����
#define CONV_MIRROR 4   //�任-����
#define CONV_ROTATE 5   //�任-��ת
#define CONV_OFFSET 6   //�任-��߷�
#define CONV_OFFSMULT 7 //�任-���ط�
#define CONV_CHANGE 8   //�任-��β�Ե�
#define CONV_PINANG 9   //�任-��Ƕȵ���

void Menu_Convert(void);
void Menu_Convert1(U32 para);

#endif
