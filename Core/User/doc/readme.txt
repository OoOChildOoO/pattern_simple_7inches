问题记录：
花样绘制过程中执行段插入的问题：
1.正常绘制某个花样后，在该花样任意处执行段插入（直线，折线，曲线），然后通过寸动键
回到最后一个点的位置，退出了段插入模式；再进行任意花样的绘制时，坐标光标移动时，
会漂移到以插入的花样的结束点作为起始坐标去移动，而非最后一点的位置作为起始点，但
最终绘制出的图案的坐标又是正常的。
2.正常绘制某个花样后，在该花样任意处执行段插入（圆，圆弧），原有的绘图数据全部消失，
插入的圆也没有正常创建成功，最后图案变成从原点到段插入结束点的空送。

3.因为Mit_Create_Data()中有一句显示P点个数的语句，在绘制任意花样时，会导致在屏幕的异常位置显示该数据的问题；
4.在7寸屏中，绘制圆、圆弧花样时，并且在输入坐标点均正常的情况下，
因Mit_Create_Data()会导致其报“生成花样超过范围”的错误，此问题在8寸屏中正常。
5.根据欧阳昨天提供的code.c，新建了一个new_Create_Data()来代替Mit_Create_Data()，
圆、圆弧的绘制正常，但因为内部没有书写直线、折线、曲线的生成代码，又导致上述3个花样生成失败。
，但是用Mit_Create_Data()去生成直线、折线、曲线又会遇到问题3.
6.在欧阳建议下，尝试使用段插入的方法去代替直线、折线、曲线的生成，参考了Mit_Menu_CtCursor()
中段插入的功能，将该部分代码填入new_Create_Data()，发现生成的直线会覆盖掉原有数据。



7.在原点偏移的情况下，去执行绘制花样，修改花样时，绝对坐标的计算有多个地方没有引入SREG[3]的数据，
进而导致含有移动界面的光标会漂移。
现在将通过原点坐标去计算得到的绝对坐标前均加了一个SREG[3]==7?的判断，
这样可以使获取到的g_xabs的值获取均正确。
但是部分界面的绝对坐标是从Read_xyposi()函数中获取，该函数从主控读取了相关位置；
			g_xabspos = (S16)(DREG[D_CMDPARA1]);
			g_yabspos = (S16)(DREG[D_CMDPARA2]);
            此处却没有处理原点偏移数据，从而导致关于显示绝对坐标的数据显示存在偏差，
            此数据也应该在某个时刻发送给主控，所以此处数据有点不同步。
8.特殊情况，如果对已经有零点偏移的函数进行段插入操作，进入花样绘制界面后，通过
        if (g_GreatPatType == 1)
        {
            mitPinmsg = mit_getPinsMsg((U16 *)SREG, 52000, g_feedc);
            g_oxpos = mitPinmsg.posx;
            g_oypos = mitPinmsg.posy;
            g_inoxpos = g_oxpos;
            g_inoypos = g_oypos;
            g_dxpos = 0;
            g_dypos = 0;
        }
该库函数去获取当前的修改针点的绝对位置，但是查看g_oxpos得到的值是错误的，
(如：原点偏移为 x:+120 y:0;选中点绝对坐标为x:-90 y:0,而得到的g_oxpos = -210)；
所以在记录绘图的起始点时，就出现了错误，还会影响绘制的图形生成的位置。


//****************************上述问题已解决*************************************************
2021-07-08:面板与主控联调时存在的相关问题记录：

1.主控-IREG2[35*N]中，存储为花样索引的花样格式类型，MLK系统中 高位的0x0100才表示文件格式，而朱工主控使用 低位的0x0001;会导致花样文件识别有误。
[PS：HREH[8]表示对于花样的头文件，此处正常]
2.主控-花样打板过后，使用“不保存并退出”时，会在menu_documents.c的tmppara=10中的if (Read_Status() == 0)处，无法及时收到响应。再去执行其他指令时，花样数据已经无法正常保存。
3.主控-穿线界面中，发送穿线指令 Write_Cmd1(C_THERAD, 0, 0) 无法及时响应。
4.主控-无法删除花样 在 花样读取界面中，删除某个选中花样时，进入 menu_documents.c 中的等待删除画面。
  在执行完 f = Write_D(D_Address[DISP] + D_COMMAND, 3, (U16 *)(&DREG[D_COMMAND]));后的 if (Read_Status() == 0)出。无法及时收到响应。


5.进行点删除操作时，原有花样进行了点移动，并且超过了原来花样的最大大小，可能导致点删除时，花样显示位置错乱的问题。【具体问题尚不明】。
6.在原来已经修改过花样的情况下，添加花样代码，可能导致花样显示位置错乱的问题。【具体问题尚不明】。
7.MLK功能缺陷：进行段修改后，原来的备份数据全部会被清除；

8.压脚界面，压脚范围存在负数时，显示有冲突。
9.在执行花样修改后，中压脚基准值意外偏移。
                
10.MLK设计缺陷：已P点为中心的动画移动预览界面，因为花样无时无刻都要重绘，此处刷新感到明显的闪屏，十分不舒服。
【暂不进行修改，根据实际情况去执行】
11.花样打板界面，进行一系列绘图后，擦除所有绘制的图案，本来可以通过缩放，使原点标记回到画布中心，但实际存在偏差。

12.主控-快捷参数预览界面中，返回主界面时，会报警-操作失败。
13.花样变换界面进入的倒缝设置时，显示预览图像功能指向有问题。
【完成】
14.花样变换界面的图像对称界面进入显示预览功能时，画布设定存在问题。
【完成】

15.部分花样变换界面等一系列可能导致原点位置向基准点移动时，最后回到原点的线绘制异常。[PS：库函数导致，最后的一段移送完坐标没有指向原点坐标]
【目前通过修改绘图函数完成修改，治标不治本】

16.主控-主界面使用寸动功能后，无法再使用回到原点功能

17.主控-组合花样功能设置无效

18.花样绘制界面的相对坐标输入模式下，确认可当前点后，坐标显示没有及时清零。
    且涉及不合理，应当使能光标预览。
    【完成】


主控问题涉及的指令类型, 17 58 46 90 20 102   36  126  199

19.在插上U盘的情况下，每次切换界面，时间与U盘状态栏总会刷新重新显示
【完成】



/*************************************************************************************/
1.触摸灵敏度设置界面处，内部部分显示内容不符合设计预期（如标题等），需求开放修改（该界面未库函数）