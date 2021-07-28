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

#define MOIMAGE_VIEW 1      //仅预览 - 含花样信息预览
#define MOIMAGE_SKIPJOG 0   //跳针预览 - 含花样信息跳针
#define MOIMAGE_CVIEW 50    //仅预览-花样打板-仅预览

#define MOIMAGE_MOVE 210    //跳针预览-花样修改-段移动
#define MOIMAGE_CHANGE 211  //跳针预览-花样修改-段修改
#define MOIMAGE_ENSURE1 200 //仅预览-段修改-确认生成-直线/Z字缝
#define MOIMAGE_ENSURE2 201 //仅预览-段修改-确认生成-折线/圆弧/曲线/空送
#define MOIMAGE_ENSURE3 220 //仅预览
//200

#define MODIFY_STA_CHANGE 0  //修改-起缝点
#define MODIFY_PIN_DEL 1     //修改-针删除
#define MODIFY_PIN_ADD 2     //修改-针添加
#define MODIFY_PIN_MOVE 3    //修改-针移动
#define MODIFY_PART_MOVE 4   //修改-段移动
#define MODIFY_PART_CHANGE 5 //修改-段修改
#define MODIFY_PART_ADD 9    //修改-段插入
#define MODIFY_PITCH 6       //修改-针距修改
#define MODIFY_PF 7          ////修改-中压脚高度
#define MODIFY_LF 8          //修改-松线力度
#define MODIFY_SPEED 15      //修改-缝纫速度
#define MODIFY_CODE_ADD 20   //修改-代码添加
#define MODIFY_CODE_DEL 21   //修改-代码删除

#define CONVERT_TACKING 26  //变换-倒缝
#define CONVERT_ZIGZAG 27   //变换-Z字缝
#define CONVERT_ZOOM 28     //变换-缩放
#define CONVERT_MIRROR 29   //变换-对称
#define CONVERT_ROTATE 30   //变换-旋转
#define CONVERT_OFFSMULT 31 //变换-多重缝
#define CONVERT_OFFSET 32   //变换-离边缝
#define CONVERT_CHANGE 33   //变换-首位对调
#define CONVERT_PINANG 34   //变换-针角度

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
