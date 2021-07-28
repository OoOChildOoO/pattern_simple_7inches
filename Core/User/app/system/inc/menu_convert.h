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

#define CONV_ORGSUB 0   //变换-原点修正
#define CONV_TACKING 1  //变换-前后倒缝
#define CONV_ZIG 2      //变换-z字缝
#define CONV_SCALE 3    //变换-缩放
#define CONV_MIRROR 4   //变换-镜像
#define CONV_ROTATE 5   //变换-旋转
#define CONV_OFFSET 6   //变换-离边缝
#define CONV_OFFSMULT 7 //变换-多重缝
#define CONV_CHANGE 8   //变换-首尾对调
#define CONV_PINANG 9   //变换-针角度调整

void Menu_Convert(void);
void Menu_Convert1(U32 para);

#endif
