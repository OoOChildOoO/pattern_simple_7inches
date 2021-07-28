//***********************************************************
//*				   �����ٿص��ӿƼ����޹�˾					*
//* GUANGZHOU SPEED CONTROL ELECTRONIC TECHNOLOGY CO., LTD. *
//*				   http://www.sukotech.com					*
//***********************************************************
//*  �� Ʒ �� ��:  ��ҵ���Ի������һ�����ϵͳ				*
//*  ��Ʒ��������: CPU										*
//*  C   P    U:   STM32F429IG								*
//*  �� �� �� ��:  Release Version 1.0						*
//*  ��   ��  ��:  color.h									*
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


#ifndef __COLOR_H 
#define __COLOR_H


extern const unsigned char mCOLOR;
//extern unsigned short RGB(unsigned char r,unsigned char g,unsigned char b);
#define 	RGB(r,g,b)					(((r&0xf8)<<8)|((g&0xfc)<<3)|(b>>3))




#define 	Color_Black					RGB(0,0,0)			// ��ɫ
#define 	Color_White					RGB(255,255,255)	// ��ɫ
#define 	Color_Blue					RGB(0,0,255)		// ��ɫ
#define 	Color_Green					RGB(0,255,0)		// ��ɫ
#define 	Color_Red					RGB(255,0,0)		// ��ɫ
#define 	Color_White1				RGB(245,245,245)	// ��ɫ 250 250 250

#define 	Color_Yellow				RGB(255,255,0)		// ����
#define 	Color_Magenta 				RGB(255,0,255)		// ���
#define 	Color_Fuchsia 				RGB(255,0,255)		// ��������(�Ϻ�ɫ)
#define 	Color_Navy 					RGB(0,0,128)		// ������ #000080  0,0,128 
#define 	Color_Purple 				RGB(128,0,128)		// ��ɫ #800080  128,0,128 
#define 	Color_Cyan 					RGB(0,255,255)		// ��ɫ #00FFFF  0,255,255 
#define 	Color_Aqua 					RGB(0,255,255)		// ˮ��ɫ #00FFFF  0,255,255 
#define 	Color_Teal 					RGB(0,128,128)		// ˮѼɫ #008080  0,128,128 
#define 	Color_Lime 					RGB(0,255,0)		// ���ɫ #00FF00  0,255,0 
#define 	Color_Olive 				RGB(128,128,0)		// ��� #808000  128,128,0 
#define 	Color_Maroon 				RGB(128,0,0)		// ��ɫ #800000  128,0,0 
#define 	Color_LightGray 			RGB(211,211,211)	// ǳ��ɫ #D3D3D3  211,211,211 
//#define 	Color_Silver 				RGB(192,192,192)	// ����ɫ #C0C0C0  192,192,192 
#define 	Color_Silver				RGB(211,211,211)
//#define 	Color_Silver 				RGB(0,191,255)		//Color_DeepSkyBlue
#define 	Color_DarkGray 				RGB(169,169,169)	// ���ɫ #A9A9A9  169,169,169 
//#define 	Color_DarkGray 			    RGB(135,206,250)	//  Color_LightSkyBlue
#define 	Color_Gray 					RGB(128,128,128)	// ��ɫ #808080  128,128,128 
#define 	Color_Brown 				RGB(165,42,42)		// ��ɫ #A52A2A  165,42,42 
#define 	Color_LBlue					RGB(128,255,255)		// ǳ��
#define 	Color_GrassGreen			RGB(0,153,51)		// ����ɫ
#define 	Color_SkyBlue1 				RGB(0,204,255)	// ����� #87CEEB  135,206,235 
#define 	Color_Dim1Gray 				RGB(64,64,64)	// �����Ļ�ɫ #696969  105,105,105

#define 	Color_LightPink				RGB(255,182,193)	// LightPink ǳ�ۺ� #FFB6C1 255,182,193
#define 	Color_Pink					RGB(255,192,203)	// Pink �ۺ� #FFC0CB 255,192,203
//#define 	Color_Pink					RGB(255,20,147)
#define 	Color_Crimson				RGB(220,20,60)		// Crimson �ɺ� #DC143C 220,20,60
#define 	Color_DeepPink				RGB(255,20,147)		// DeepPink ���ɫ #FF1493  255,20,147 
#define 	Color_Thistle 				RGB(216,191,216)	// �� #D8BFD8  216,191,216 
#define 	Color_Plum 					RGB(221,160,221)	// ���� #DDA0DD  221,160,221 
#define 	Color_Violet 				RGB(238,130,238)	// ������ #EE82EE  238,130,238 
#define 	Color_Indigo 				RGB(75,0,130)		// ���� #4B0082  75,0,130 
#define 	Color_LavenderBlush 		RGB(255,240,245)	// ����ĵ���ɫ #FFF0F5  255,240,245 
#define 	Color_PaleVioletRed 		RGB(219,112,147)	// �԰׵���������ɫ #DB7093  219,112,147 
#define 	Color_HotPink 				RGB(255,105,180)	// ����ķۺ� #FF69B4  255,105,180 
#define 	Color_MediumVioletRed		RGB(199,21,133)		// ���е���������ɫ #C71585  199,21,133 
#define 	Color_Orchid 				RGB(218,112,214)	// ��������ɫ #DA70D6  218,112,214 
#define 	Color_DarkMagenta 			RGB(139,0,139)		// �����ɫ #8B008B  139,0,139 
#define 	Color_MediumOrchid 			RGB(186,85,211)		// ���е������� #BA55D3  186,85,211 
#define 	Color_DarkVoilet 			RGB(148,0,211)		// ��������ɫ #9400D3  148,0,211 
#define 	Color_DarkOrchid 			RGB(153,50,204)		// �������� #9932CC  153,50,204 
#define 	Color_BlueViolet 			RGB(138,43,226)		// ������������ɫ #8A2BE2  138,43,226 
#define 	Color_MediumPurple 			RGB(147,112,219)	// ���е���ɫ #9370DB  147,112,219 
#define 	Color_MediumSlateBlue 		RGB(123,104,238)	// ���еİ��Ұ�����ɫ #7B68EE  123,104,238 
#define 	Color_SlateBlue 			RGB(106,90,205)		// ���Ұ�����ɫ #6A5ACD  106,90,205 
#define 	Color_DarkSlateBlue 		RGB(72,61,139)		// ���Ұ�����ɫ #483D8B  72,61,139 
#define 	Color_Lavender 				RGB(230,230,250)	// Ѭ�²ݻ��ĵ���ɫ #E6E6FA  230,230,250 
#define 	Color_GhostWhite 			RGB(248,248,255)	// ����İ�ɫ #F8F8FF  248,248,255 
#define 	Color_MediumBlue 			RGB(0,0,205)		// ���е���ɫ #0000CD  0,0,205 
#define 	Color_MidnightBlue 			RGB(25,25,112)		// ��ҹ����ɫ #191970  25,25,112 
#define 	Color_DarkBlue 				RGB(0,0,139)		// ����ɫ #00008B  0,0,139 
#define 	Color_RoyalBlue 			RGB(65,105,225)		// �ʾ��� #4169E1  65,105,225 
#define 	Color_CornflowerBlue 		RGB(100,149,237)	// ʸ���յ���ɫ #6495ED  100,149,237 
#define 	Color_LightSteelBlue 		RGB(176,196,222)	// ������ #B0C4DE  176,196,222 
#define 	Color_LightSlateGray 		RGB(119,136,153)	// ǳʯ��� #778899  119,136,153 
#define 	Color_SlateGray 			RGB(112,128,144)	// ʯ��� #708090  112,128,144 
#define 	Color_DoderBlue 			RGB(30,144,255)		// ������ #1E90FF  30,144,255 
#define 	Color_AliceBlue 			RGB(240,248,255)	// ����˿�� #F0F8FF  240,248,255 
#define 	Color_SteelBlue 			RGB(70,130,180)		// ���� #4682B4  70,130,180 
#define 	Color_LightSkyBlue 			RGB(135,206,250)	// ����ɫ #87CEFA  135,206,250 
#define 	Color_SkyBlue 				RGB(135,206,235)	// ����ɫ #87CEEB  135,206,235 
#define 	Color_MidSkyBlue 			RGB(0,130,255)	// ������ɫ #87CEEB  135,206,235 
#define 	Color_DeepSkyBlue 			RGB(0,191,255)		// ������ #00BFFF  0,191,255 
//#define 	Color_DeepSkyBlue 			RGB(176,142,107)		//B08E6B
//#define 	Color_DeepSkyBlue 			RGB(176,152,107)		//B89D7C  RGB(176,152,107)
//#define 	Color_DeepSkyBlue 			RGB(163,141,105)		//B89D7C
#define 	Color_LightBLue 			RGB(173,216,230)	// ���� #ADD8E6  173,216,230 
#define 	Color_PowDerBlue 			RGB(176,224,230)	// ��ҩ�� #B0E0E6  176,224,230 
#define 	Color_CadetBlue 			RGB(95,158,160)		// ��У�� #5F9EA0  95,158,160 
#define 	Color_Azure 				RGB(240,255,255)	// ε��ɫ #F0FFFF  240,255,255 
#define 	Color_LightCyan 			RGB(225,255,255)	// ����ɫ #E1FFFF  225,255,255 
#define 	Color_PaleTurquoise 		RGB(175,238,238)	// �԰׵��̱�ʯ #AFEEEE  175,238,238 
#define 	Color_DarkTurquoise 		RGB(0,206,209)		// ���̱�ʯ #00CED1  0,206,209 
#define 	Color_DarkSlateGray 		RGB(47,79,79)		// ��ʯ��� #2F4F4F  47,79,79 
#define 	Color_DarkCyan 				RGB(0,139,139)		// ����ɫ #008B8B  0,139,139 
#define 	Color_MediumTurquoise 		RGB(72,209,204)		// ���е��̱�ʯ #48D1CC  72,209,204 
#define 	Color_LightSeaGreen 		RGB(32,178,170)		// ǳ������ #20B2AA  32,178,170 
#define 	Color_Turquoise 			RGB(64,224,208)		// �̱�ʯ #40E0D0  64,224,208 
#define 	Color_Auqamarin 			RGB(127,255,170)	// ����\����ɫ #7FFFAA  127,255,170 
#define 	Color_MediumAquamarine 		RGB(0,250,154)		// ���еı���ɫ #00FA9A  0,250,154 
#define 	Color_MediumSpringGreen 	RGB(245,255,250)	// ���еĴ������ɫ #F5FFFA  245,255,250 
#define 	Color_MintCream 			RGB(0,255,127)		// �������� #00FF7F  0,255,127 
#define 	Color_SpringGreen 			RGB(60,179,113)		// �������ɫ #3CB371  60,179,113 
#define 	Color_SeaGreen 				RGB(46,139,87)		// ������ #2E8B57  46,139,87 
#define 	Color_Honeydew 				RGB(240,255,240)	// ���� #F0FFF0  240,255,240 
#define 	Color_LightGreen 			RGB(144,238,144)	// ����ɫ #90EE90  144,238,144 
#define 	Color_PaleGreen 			RGB(152,251,152)	// �԰׵���ɫ #98FB98  152,251,152 
#define 	Color_DarkSeaGreen 			RGB(143,188,143)	// ����� #8FBC8F  143,188,143 
#define 	Color_LimeGreen 			RGB(50,205,50)		// ����� #32CD32  50,205,50 
#define 	Color_ForestGreen 			RGB(34,139,34)		// ɭ���� #228B22  34,139,34 
#define 	Color_DarkGreen 			RGB(0,100,0)		// ����ɫ #006400  0,100,0 
#define 	Color_Chartreuse 			RGB(127,255,0)		// ���ؾ��� #7FFF00  127,255,0 
#define 	Color_LawnGreen 			RGB(124,252,0)		// ��ƺ�� #7CFC00  124,252,0 
#define 	Color_GreenYellow 			RGB(173,255,47)		// �̻�ɫ #ADFF2F  173,255,47 
#define 	Color_OliveDrab 			RGB(85,107,47)		// �������ɫ #556B2F  85,107,47 
#define 	Color_Beige 				RGB(107,142,35)		// ��ɫ(ǳ��ɫ) #6B8E23  107,142,35 
#define 	Color_LightGoldenrodYellow 	RGB(250,250,210)	// ǳ������� #FAFAD2  250,250,210 
#define 	Color_Ivory 				RGB(255,255,240)	// ���� #FFFFF0  255,255,240 
#define 	Color_LightYellow 			RGB(255,255,224)	// ǳ��ɫ #FFFFE0  255,255,224 
#define 	Color_DarkKhaki 			RGB(189,183,107)	// ��䲼 #BDB76B  189,183,107 
#define 	Color_LemonChiffon 			RGB(255,250,205)	// ���ʱ�ɴ #FFFACD  255,250,205 
#define 	Color_PaleGodenrod 			RGB(238,232,170)	// �������� #EEE8AA  238,232,170 
#define 	Color_Khaki 				RGB(240,230,140)	// ���䲼 #F0E68C  240,230,140 
#define 	Color_Gold 					RGB(255,215,0)		// �� #FFD700  255,215,0 
#define 	Color_Cornislk 				RGB(255,248,220)	// ����ɫ #FFF8DC  255,248,220 
#define 	Color_GoldEnrod 			RGB(218,165,32)		// ������ #DAA520  218,165,32 
#define 	Color_FloralWhite 			RGB(255,250,240)	// ���İ�ɫ #FFFAF0  255,250,240 
#define 	Color_OldLace 				RGB(253,245,230)	// ���δ� #FDF5E6  253,245,230 
#define 	Color_Wheat 				RGB(245,222,179)	// С��ɫ #F5DEB3  245,222,179 
#define 	Color_Moccasin 				RGB(255,228,181)	// ¹ƤЬ #FFE4B5  255,228,181 
#define 	Color_Orange 				RGB(255,165,0)		// ��ɫ #FFA500  255,165,0 
#define 	Color_PapayaWhip 			RGB(255,239,213)	// ��ľ�� #FFEFD5  255,239,213 
#define 	Color_BlanchedAlmond 		RGB(255,235,205)	// Ư�׵����� #FFEBCD  255,235,205 
#define 	Color_NavajoWhite 			RGB(255,222,173)	// Navajo�� #FFDEAD  255,222,173 
#define 	Color_AntiqueWhite 			RGB(250,235,215)	// �Ŵ��İ�ɫ #FAEBD7  250,235,215 
#define 	Color_Tan 					RGB(210,180,140)	// ɹ�� #D2B48C  210,180,140 
#define 	Color_BrulyWood 			RGB(222,184,135)	// ��ʵ���� #DEB887  222,184,135 
#define 	Color_Bisque 				RGB(255,228,196)	// (Ũ��)��֬,���ѵ� #FFE4C4  255,228,196 
#define 	Color_DarkOrange 			RGB(255,140,0)		// ���ɫ #FF8C00  255,140,0 
#define 	Color_Linen 				RGB(250,240,230)	// ���鲼 #FAF0E6  250,240,230 
#define 	Color_Peru 					RGB(205,133,63)		// ��³ #CD853F  205,133,63 
#define 	Color_PeachPuff 			RGB(255,218,185)	// ��ɫ #FFDAB9  255,218,185 
#define 	Color_SandyBrown 			RGB(244,164,96)		// ɳ��ɫ #F4A460  244,164,96 
#define 	Color_Chocolate 			RGB(210,105,30)		// �ɿ��� #D2691E  210,105,30 
#define 	Color_SaddleBrown 			RGB(139,69,19)		// ����ɫ #8B4513  139,69,19 
#define 	Color_SeaShell 				RGB(255,245,238)	// ������ #FFF5EE  255,245,238 
#define 	Color_Sienna 				RGB(160,82,45)		// ������ɫ #A0522D  160,82,45 
#define 	Color_LightSalmon 			RGB(255,160,122)	// ǳ����(����)ɫ #FFA07A  255,160,122 
#define 	Color_Coral 				RGB(255,127,80)		// ɺ�� #FF7F50  255,127,80 
#define 	Color_OrangeRed 			RGB(255,69,0)		// �Ⱥ�ɫ #FF4500  255,69,0 
#define 	Color_DarkSalmon 			RGB(233,150,122)	// ������(����)ɫ #E9967A  233,150,122 
#define 	Color_Tomato 				RGB(255,99,71)		// ���� #FF6347  255,99,71 
#define 	Color_MistyRose 			RGB(255,228,225)	// ����õ�� #FFE4E1  255,228,225 
#define 	Color_Salmon 				RGB(250,128,114)	// ����(����)ɫ #FA8072  250,128,114 
#define 	Color_Snow 					RGB(255,250,250)	// ѩ #FFFAFA  255,250,250 
#define 	Color_LightCoral 			RGB(240,128,128)	// ��ɺ��ɫ #F08080  240,128,128 
#define 	Color_RosyBrown 			RGB(188,143,143)	// õ����ɫ #BC8F8F  188,143,143 
#define 	Color_IndianRed 			RGB(205,92,92)		// ӡ�Ⱥ� #CD5C5C  205,92,92 
#define 	Color_FireBrick 			RGB(178,34,34)		// �ͻ�ש #B22222  178,34,34 
#define 	Color_DarkRed 				RGB(139,0,0)		// ���ɫ #8B0000  139,0,0 
#define 	Color_WhiteSmoke 			RGB(245,245,245)	// ���� #F5F5F5  245,245,245 
//#define 	Color_Gainsboro 			RGB(220,220,220)	// Gainsboro #DCDCDC  220,220,220 
#define 	Color_Gainsboro 			RGB(212,212,212)	// Gainsboro #DCDCDC  220,220,220 
#define 	Color_DimGray 				RGB(105,105,105)	// �����Ļ�ɫ #696969  105,105,105
#define 	Color_KeyBlue				RGB(53,130,202)		// 
#define 	Color_Red1					RGB(206,0,103)		// ��ɫ
#define 	Color_1						RGB(236,214,192)		// 
#define 	Color_BG1					RGB(106,147,97)
#define 	Color_BG2					RGB(0,158,180)
#define 	Color_BG3					RGB(45,213,204)
#define 	Color_BG4					RGB(218,208,182)

#define 	Color_Frame					Color_Dim1Gray		// ��������ɫ
#define 	Color_Frame1				RGB(251,239,11)			// ����������ɫ


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

