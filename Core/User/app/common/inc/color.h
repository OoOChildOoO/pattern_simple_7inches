//***********************************************************
//*				   广州速控电子科技有限公司					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  产 品 名 称:  工业电脑花样缝纫机控制系统				*
//*  产品开发名称: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  版 本 序 号:  Release Version 1.0						*
//*  文   件  名:  color.h									*
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


#ifndef __COLOR_H 
#define __COLOR_H


extern const unsigned char mCOLOR;
//extern unsigned short RGB(unsigned char r,unsigned char g,unsigned char b);
#define 	RGB(r,g,b)					(((r&0xf8)<<8)|((g&0xfc)<<3)|(b>>3))




#define 	Color_Black					RGB(0,0,0)			// 黑色
#define 	Color_White					RGB(255,255,255)	// 白色
#define 	Color_Blue					RGB(0,0,255)		// 蓝色
#define 	Color_Green					RGB(0,255,0)		// 绿色
#define 	Color_Red					RGB(255,0,0)		// 红色
#define 	Color_White1				RGB(245,245,245)	// 白色 250 250 250

#define 	Color_Yellow				RGB(255,255,0)		// 纯黄
#define 	Color_Magenta 				RGB(255,0,255)		// 洋红
#define 	Color_Fuchsia 				RGB(255,0,255)		// 灯笼海棠(紫红色)
#define 	Color_Navy 					RGB(0,0,128)		// 海军蓝 #000080  0,0,128 
#define 	Color_Purple 				RGB(128,0,128)		// 紫色 #800080  128,0,128 
#define 	Color_Cyan 					RGB(0,255,255)		// 青色 #00FFFF  0,255,255 
#define 	Color_Aqua 					RGB(0,255,255)		// 水绿色 #00FFFF  0,255,255 
#define 	Color_Teal 					RGB(0,128,128)		// 水鸭色 #008080  0,128,128 
#define 	Color_Lime 					RGB(0,255,0)		// 酸橙色 #00FF00  0,255,0 
#define 	Color_Olive 				RGB(128,128,0)		// 橄榄 #808000  128,128,0 
#define 	Color_Maroon 				RGB(128,0,0)		// 栗色 #800000  128,0,0 
#define 	Color_LightGray 			RGB(211,211,211)	// 浅灰色 #D3D3D3  211,211,211 
//#define 	Color_Silver 				RGB(192,192,192)	// 银白色 #C0C0C0  192,192,192 
#define 	Color_Silver				RGB(211,211,211)
//#define 	Color_Silver 				RGB(0,191,255)		//Color_DeepSkyBlue
#define 	Color_DarkGray 				RGB(169,169,169)	// 深灰色 #A9A9A9  169,169,169 
//#define 	Color_DarkGray 			    RGB(135,206,250)	//  Color_LightSkyBlue
#define 	Color_Gray 					RGB(128,128,128)	// 灰色 #808080  128,128,128 
#define 	Color_Brown 				RGB(165,42,42)		// 棕色 #A52A2A  165,42,42 
#define 	Color_LBlue					RGB(128,255,255)		// 浅蓝
#define 	Color_GrassGreen			RGB(0,153,51)		// 草绿色
#define 	Color_SkyBlue1 				RGB(0,204,255)	// 天空蓝 #87CEEB  135,206,235 
#define 	Color_Dim1Gray 				RGB(64,64,64)	// 暗淡的灰色 #696969  105,105,105

#define 	Color_LightPink				RGB(255,182,193)	// LightPink 浅粉红 #FFB6C1 255,182,193
#define 	Color_Pink					RGB(255,192,203)	// Pink 粉红 #FFC0CB 255,192,203
//#define 	Color_Pink					RGB(255,20,147)
#define 	Color_Crimson				RGB(220,20,60)		// Crimson 猩红 #DC143C 220,20,60
#define 	Color_DeepPink				RGB(255,20,147)		// DeepPink 深粉色 #FF1493  255,20,147 
#define 	Color_Thistle 				RGB(216,191,216)	// 蓟 #D8BFD8  216,191,216 
#define 	Color_Plum 					RGB(221,160,221)	// 李子 #DDA0DD  221,160,221 
#define 	Color_Violet 				RGB(238,130,238)	// 紫罗兰 #EE82EE  238,130,238 
#define 	Color_Indigo 				RGB(75,0,130)		// 靛青 #4B0082  75,0,130 
#define 	Color_LavenderBlush 		RGB(255,240,245)	// 脸红的淡紫色 #FFF0F5  255,240,245 
#define 	Color_PaleVioletRed 		RGB(219,112,147)	// 苍白的紫罗兰红色 #DB7093  219,112,147 
#define 	Color_HotPink 				RGB(255,105,180)	// 热情的粉红 #FF69B4  255,105,180 
#define 	Color_MediumVioletRed		RGB(199,21,133)		// 适中的紫罗兰红色 #C71585  199,21,133 
#define 	Color_Orchid 				RGB(218,112,214)	// 兰花的紫色 #DA70D6  218,112,214 
#define 	Color_DarkMagenta 			RGB(139,0,139)		// 深洋红色 #8B008B  139,0,139 
#define 	Color_MediumOrchid 			RGB(186,85,211)		// 适中的兰花紫 #BA55D3  186,85,211 
#define 	Color_DarkVoilet 			RGB(148,0,211)		// 深紫罗兰色 #9400D3  148,0,211 
#define 	Color_DarkOrchid 			RGB(153,50,204)		// 深兰花紫 #9932CC  153,50,204 
#define 	Color_BlueViolet 			RGB(138,43,226)		// 深紫罗兰的蓝色 #8A2BE2  138,43,226 
#define 	Color_MediumPurple 			RGB(147,112,219)	// 适中的紫色 #9370DB  147,112,219 
#define 	Color_MediumSlateBlue 		RGB(123,104,238)	// 适中的板岩暗蓝灰色 #7B68EE  123,104,238 
#define 	Color_SlateBlue 			RGB(106,90,205)		// 板岩暗蓝灰色 #6A5ACD  106,90,205 
#define 	Color_DarkSlateBlue 		RGB(72,61,139)		// 深岩暗蓝灰色 #483D8B  72,61,139 
#define 	Color_Lavender 				RGB(230,230,250)	// 熏衣草花的淡紫色 #E6E6FA  230,230,250 
#define 	Color_GhostWhite 			RGB(248,248,255)	// 幽灵的白色 #F8F8FF  248,248,255 
#define 	Color_MediumBlue 			RGB(0,0,205)		// 适中的蓝色 #0000CD  0,0,205 
#define 	Color_MidnightBlue 			RGB(25,25,112)		// 午夜的蓝色 #191970  25,25,112 
#define 	Color_DarkBlue 				RGB(0,0,139)		// 深蓝色 #00008B  0,0,139 
#define 	Color_RoyalBlue 			RGB(65,105,225)		// 皇军蓝 #4169E1  65,105,225 
#define 	Color_CornflowerBlue 		RGB(100,149,237)	// 矢车菊的蓝色 #6495ED  100,149,237 
#define 	Color_LightSteelBlue 		RGB(176,196,222)	// 淡钢蓝 #B0C4DE  176,196,222 
#define 	Color_LightSlateGray 		RGB(119,136,153)	// 浅石板灰 #778899  119,136,153 
#define 	Color_SlateGray 			RGB(112,128,144)	// 石板灰 #708090  112,128,144 
#define 	Color_DoderBlue 			RGB(30,144,255)		// 道奇蓝 #1E90FF  30,144,255 
#define 	Color_AliceBlue 			RGB(240,248,255)	// 爱丽丝蓝 #F0F8FF  240,248,255 
#define 	Color_SteelBlue 			RGB(70,130,180)		// 钢蓝 #4682B4  70,130,180 
#define 	Color_LightSkyBlue 			RGB(135,206,250)	// 淡蓝色 #87CEFA  135,206,250 
#define 	Color_SkyBlue 				RGB(135,206,235)	// 天蓝色 #87CEEB  135,206,235 
#define 	Color_MidSkyBlue 			RGB(0,130,255)	// 中天蓝色 #87CEEB  135,206,235 
#define 	Color_DeepSkyBlue 			RGB(0,191,255)		// 深天蓝 #00BFFF  0,191,255 
//#define 	Color_DeepSkyBlue 			RGB(176,142,107)		//B08E6B
//#define 	Color_DeepSkyBlue 			RGB(176,152,107)		//B89D7C  RGB(176,152,107)
//#define 	Color_DeepSkyBlue 			RGB(163,141,105)		//B89D7C
#define 	Color_LightBLue 			RGB(173,216,230)	// 淡蓝 #ADD8E6  173,216,230 
#define 	Color_PowDerBlue 			RGB(176,224,230)	// 火药蓝 #B0E0E6  176,224,230 
#define 	Color_CadetBlue 			RGB(95,158,160)		// 军校蓝 #5F9EA0  95,158,160 
#define 	Color_Azure 				RGB(240,255,255)	// 蔚蓝色 #F0FFFF  240,255,255 
#define 	Color_LightCyan 			RGB(225,255,255)	// 淡青色 #E1FFFF  225,255,255 
#define 	Color_PaleTurquoise 		RGB(175,238,238)	// 苍白的绿宝石 #AFEEEE  175,238,238 
#define 	Color_DarkTurquoise 		RGB(0,206,209)		// 深绿宝石 #00CED1  0,206,209 
#define 	Color_DarkSlateGray 		RGB(47,79,79)		// 深石板灰 #2F4F4F  47,79,79 
#define 	Color_DarkCyan 				RGB(0,139,139)		// 深青色 #008B8B  0,139,139 
#define 	Color_MediumTurquoise 		RGB(72,209,204)		// 适中的绿宝石 #48D1CC  72,209,204 
#define 	Color_LightSeaGreen 		RGB(32,178,170)		// 浅海洋绿 #20B2AA  32,178,170 
#define 	Color_Turquoise 			RGB(64,224,208)		// 绿宝石 #40E0D0  64,224,208 
#define 	Color_Auqamarin 			RGB(127,255,170)	// 绿玉\碧绿色 #7FFFAA  127,255,170 
#define 	Color_MediumAquamarine 		RGB(0,250,154)		// 适中的碧绿色 #00FA9A  0,250,154 
#define 	Color_MediumSpringGreen 	RGB(245,255,250)	// 适中的春天的绿色 #F5FFFA  245,255,250 
#define 	Color_MintCream 			RGB(0,255,127)		// 薄荷奶油 #00FF7F  0,255,127 
#define 	Color_SpringGreen 			RGB(60,179,113)		// 春天的绿色 #3CB371  60,179,113 
#define 	Color_SeaGreen 				RGB(46,139,87)		// 海洋绿 #2E8B57  46,139,87 
#define 	Color_Honeydew 				RGB(240,255,240)	// 蜂蜜 #F0FFF0  240,255,240 
#define 	Color_LightGreen 			RGB(144,238,144)	// 淡绿色 #90EE90  144,238,144 
#define 	Color_PaleGreen 			RGB(152,251,152)	// 苍白的绿色 #98FB98  152,251,152 
#define 	Color_DarkSeaGreen 			RGB(143,188,143)	// 深海洋绿 #8FBC8F  143,188,143 
#define 	Color_LimeGreen 			RGB(50,205,50)		// 酸橙绿 #32CD32  50,205,50 
#define 	Color_ForestGreen 			RGB(34,139,34)		// 森林绿 #228B22  34,139,34 
#define 	Color_DarkGreen 			RGB(0,100,0)		// 深绿色 #006400  0,100,0 
#define 	Color_Chartreuse 			RGB(127,255,0)		// 查特酒绿 #7FFF00  127,255,0 
#define 	Color_LawnGreen 			RGB(124,252,0)		// 草坪绿 #7CFC00  124,252,0 
#define 	Color_GreenYellow 			RGB(173,255,47)		// 绿黄色 #ADFF2F  173,255,47 
#define 	Color_OliveDrab 			RGB(85,107,47)		// 橄榄土褐色 #556B2F  85,107,47 
#define 	Color_Beige 				RGB(107,142,35)		// 米色(浅褐色) #6B8E23  107,142,35 
#define 	Color_LightGoldenrodYellow 	RGB(250,250,210)	// 浅秋麒麟黄 #FAFAD2  250,250,210 
#define 	Color_Ivory 				RGB(255,255,240)	// 象牙 #FFFFF0  255,255,240 
#define 	Color_LightYellow 			RGB(255,255,224)	// 浅黄色 #FFFFE0  255,255,224 
#define 	Color_DarkKhaki 			RGB(189,183,107)	// 深卡其布 #BDB76B  189,183,107 
#define 	Color_LemonChiffon 			RGB(255,250,205)	// 柠檬薄纱 #FFFACD  255,250,205 
#define 	Color_PaleGodenrod 			RGB(238,232,170)	// 灰秋麒麟 #EEE8AA  238,232,170 
#define 	Color_Khaki 				RGB(240,230,140)	// 卡其布 #F0E68C  240,230,140 
#define 	Color_Gold 					RGB(255,215,0)		// 金 #FFD700  255,215,0 
#define 	Color_Cornislk 				RGB(255,248,220)	// 玉米色 #FFF8DC  255,248,220 
#define 	Color_GoldEnrod 			RGB(218,165,32)		// 秋麒麟 #DAA520  218,165,32 
#define 	Color_FloralWhite 			RGB(255,250,240)	// 花的白色 #FFFAF0  255,250,240 
#define 	Color_OldLace 				RGB(253,245,230)	// 老饰带 #FDF5E6  253,245,230 
#define 	Color_Wheat 				RGB(245,222,179)	// 小麦色 #F5DEB3  245,222,179 
#define 	Color_Moccasin 				RGB(255,228,181)	// 鹿皮鞋 #FFE4B5  255,228,181 
#define 	Color_Orange 				RGB(255,165,0)		// 橙色 #FFA500  255,165,0 
#define 	Color_PapayaWhip 			RGB(255,239,213)	// 番木瓜 #FFEFD5  255,239,213 
#define 	Color_BlanchedAlmond 		RGB(255,235,205)	// 漂白的杏仁 #FFEBCD  255,235,205 
#define 	Color_NavajoWhite 			RGB(255,222,173)	// Navajo白 #FFDEAD  255,222,173 
#define 	Color_AntiqueWhite 			RGB(250,235,215)	// 古代的白色 #FAEBD7  250,235,215 
#define 	Color_Tan 					RGB(210,180,140)	// 晒黑 #D2B48C  210,180,140 
#define 	Color_BrulyWood 			RGB(222,184,135)	// 结实的树 #DEB887  222,184,135 
#define 	Color_Bisque 				RGB(255,228,196)	// (浓汤)乳脂,番茄等 #FFE4C4  255,228,196 
#define 	Color_DarkOrange 			RGB(255,140,0)		// 深橙色 #FF8C00  255,140,0 
#define 	Color_Linen 				RGB(250,240,230)	// 亚麻布 #FAF0E6  250,240,230 
#define 	Color_Peru 					RGB(205,133,63)		// 秘鲁 #CD853F  205,133,63 
#define 	Color_PeachPuff 			RGB(255,218,185)	// 桃色 #FFDAB9  255,218,185 
#define 	Color_SandyBrown 			RGB(244,164,96)		// 沙棕色 #F4A460  244,164,96 
#define 	Color_Chocolate 			RGB(210,105,30)		// 巧克力 #D2691E  210,105,30 
#define 	Color_SaddleBrown 			RGB(139,69,19)		// 马鞍棕色 #8B4513  139,69,19 
#define 	Color_SeaShell 				RGB(255,245,238)	// 海贝壳 #FFF5EE  255,245,238 
#define 	Color_Sienna 				RGB(160,82,45)		// 黄土赭色 #A0522D  160,82,45 
#define 	Color_LightSalmon 			RGB(255,160,122)	// 浅鲜肉(鲑鱼)色 #FFA07A  255,160,122 
#define 	Color_Coral 				RGB(255,127,80)		// 珊瑚 #FF7F50  255,127,80 
#define 	Color_OrangeRed 			RGB(255,69,0)		// 橙红色 #FF4500  255,69,0 
#define 	Color_DarkSalmon 			RGB(233,150,122)	// 深鲜肉(鲑鱼)色 #E9967A  233,150,122 
#define 	Color_Tomato 				RGB(255,99,71)		// 番茄 #FF6347  255,99,71 
#define 	Color_MistyRose 			RGB(255,228,225)	// 薄雾玫瑰 #FFE4E1  255,228,225 
#define 	Color_Salmon 				RGB(250,128,114)	// 鲜肉(鲑鱼)色 #FA8072  250,128,114 
#define 	Color_Snow 					RGB(255,250,250)	// 雪 #FFFAFA  255,250,250 
#define 	Color_LightCoral 			RGB(240,128,128)	// 淡珊瑚色 #F08080  240,128,128 
#define 	Color_RosyBrown 			RGB(188,143,143)	// 玫瑰棕色 #BC8F8F  188,143,143 
#define 	Color_IndianRed 			RGB(205,92,92)		// 印度红 #CD5C5C  205,92,92 
#define 	Color_FireBrick 			RGB(178,34,34)		// 耐火砖 #B22222  178,34,34 
#define 	Color_DarkRed 				RGB(139,0,0)		// 深红色 #8B0000  139,0,0 
#define 	Color_WhiteSmoke 			RGB(245,245,245)	// 白烟 #F5F5F5  245,245,245 
//#define 	Color_Gainsboro 			RGB(220,220,220)	// Gainsboro #DCDCDC  220,220,220 
#define 	Color_Gainsboro 			RGB(212,212,212)	// Gainsboro #DCDCDC  220,220,220 
#define 	Color_DimGray 				RGB(105,105,105)	// 暗淡的灰色 #696969  105,105,105
#define 	Color_KeyBlue				RGB(53,130,202)		// 
#define 	Color_Red1					RGB(206,0,103)		// 红色
#define 	Color_1						RGB(236,214,192)		// 
#define 	Color_BG1					RGB(106,147,97)
#define 	Color_BG2					RGB(0,158,180)
#define 	Color_BG3					RGB(45,213,204)
#define 	Color_BG4					RGB(218,208,182)

#define 	Color_Frame					Color_Dim1Gray		// 按键框颜色
#define 	Color_Frame1				RGB(251,239,11)			// 按键框按下颜色


#define 	KEY_Silver				0
#define 	KEY_BlueViolet			1
#define 	KEY_Blue				2
#define 	KEY_Gray				3
#define 	KEY_Gainsboro			4
#define 	KEY_Green				5
#define 	KEY_Red					6
#define 	KEY_White				7	
#define 	KEY_COLORTL				9

#define 	KEY_Silver1				0	

#define		Color_Dsew			RGB(165,26,200)
//#define		Color_Dsew			RGB(115,23,183)	
#endif

