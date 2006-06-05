//-------------------------------------------------------------------------
// Desc:	FLAIM text routines and conversion tables
// Tabs:	3
//
//		Copyright (c) 1991-2006 Novell, Inc. All Rights Reserved.
//
//		This program is free software; you can redistribute it and/or
//		modify it under the terms of version 2 of the GNU General Public
//		License as published by the Free Software Foundation.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//		GNU General Public License for more details.
//
//		You should have received a copy of the GNU General Public License
//		along with this program; if not, contact Novell, Inc.
//
//		To contact Novell about this file by physical or electronic mail,
//		you may find current contact information at www.novell.com
//
// $Id$
//-------------------------------------------------------------------------

#include "flaimsys.h"

#define UTOWP60_ENTRIES					1502
#define WP60toUni_MAX					15

#define Upper_JP_A						0x2520
#define Upper_JP_Z						0x2539
#define Upper_KR_A						0x5420
#define Upper_KR_Z						0x5439
#define Upper_CS_A						0x82FC
#define Upper_CS_Z						0x8316
#define Upper_CT_A						0xA625
#define Upper_CT_Z						0xA63E

#define Lower_JP_a						0x2540
#define Lower_JP_z						0x2559
#define Lower_KR_a						0x5440
#define Lower_KR_z						0x5459
#define Lower_CS_a						0x82DC
#define Lower_CS_z						0x82F5
#define Lower_CT_a						0xA60B
#define Lower_CT_z						0xA624

// Number of characters in each character set

#define ASC_N								95
#define ML1_N								242
#define ML2_N								145
#define BOX_N								88
#define TYP_N								103
#define ICN_N								255
#define MTH_N								238
#define MTX_N								229
#define GRK_N								219
#define HEB_N								123
#define CYR_N								250
#define KAN_N								63
#define USR_N								255
#define ARB_N								196
#define ARS_N								220

// TOTAL: 1447 WP + 255 User Characters

#define	C_N 								ASC_N + ML1_N + ML2_N + BOX_N + MTH_N + \
												MTX_N + TYP_N + ICN_N + GRK_N + \
												HEB_N + CYR_N + KAN_N + USR_N + ARB_N + \
												ARS_N

// Misc

#define WPCH_HIMASK	   				0x00FF
#define WPCH_LOMASK	   				0xFF00
#define WPCH_MAX_COMPLEX				5
#define NON_DISPLAYABLE_CHAR			0xFF

/****************************************************************************
Desc:
****************************************************************************/
typedef struct BASE_DIACRIT_TABLE
{
	FLMBYTE		base;
	FLMBYTE		diacrit;
} BASE_DIACRIT_TABLE;

/****************************************************************************
Desc:
****************************************************************************/
typedef struct BASE_DIACRIT
{
	FLMUINT16				char_count;		// Number of characters in table
	FLMUINT16				start_char;		// Start char
	BASE_DIACRIT_TABLE * table;
} BASE_DIACRIT;

/****************************************************************************
Desc:	Defines the range of characters within the set which are case
		convertible
****************************************************************************/
static FLMBYTE flmCaseConvertableRange[] =
{
	26, 241,									// Multinational 1
	0, 0,										// Multinational 2
	0, 0,										// Box Drawing
	0, 0,										// Symbol 1
	0, 0,										// Symbol 2
	0, 0,										// Math 1
	0, 0,										// Math 2
	0, 69,									// Greek 1
	0, 0,										// Hebrew
	0, 199,									// Cyrillic
	0, 0,										// Japanese Kana
	0, 0,										// User-defined
	0, 0,										// Not defined
	0, 0,										// Not defined
	0, 0,										// Not defined
};

/****************************************************************************
Desc:	Base character location bit mapped table. 
			(1) - corresponding base char is in same set as combined
			(0) - corresponding base char is in ascii set
****************************************************************************/
FLMBYTE flm_ml1_cb60[] = 
{
	0x00,										// 0-7
	0x00,										// 8-15
	0x00,										// 16-23
	0x00,										// 24-31
	0x00,										// 32-39
	0x00,										// 40-47
	0x55,										// 48-55
	0x00,										// 56-63
	0x00,										// 64-71
	0x00,										// 72-79
	0x00,										// 80-87
	0x00,										// 88-95
	0x00,										// 96-103
	0x00,										// 104-111
	0x00,										// 112-119
	0x00,										// 120-127
	0x14,										// 128-135
	0x44,										// 136-143
	0x00,										// 144-151
	0x00,										// 152-159
	0x00,										// 160-167
	0x00,										// 168-175
	0x00,										// 176-183
	0x00,										// 184-191
	0x00,										// 192-199
	0x00,										// 200-207
	0x00,										// 208-215
	0x00,										// 216-223
	0x00,										// 224-231
	0x04,										// 232-239
	0x00,										// 240-241
};

/****************************************************************************
Desc:
****************************************************************************/
BASE_DIACRIT_TABLE flm_ml1c_table[] =
{
	{ 'A',  acute   },
	{ 'a',  acute   },
	{ 'A',  circum  },
	{ 'a',  circum  },
	{ 'A',  umlaut  },
	{ 'a',  umlaut  },
	{ 'A',  grave   },
	{ 'a',  grave   },
	{ 'A',  ring    },
	{ 'a',  ring    },
	{ 0xff, 0xff    },
	{ 0xff, 0xff    },
	{ 'C',cedilla},
	{ 'c',cedilla},
	{ 'E',acute},
	{ 'e',acute},
	{ 'E',circum},
	{ 'e',circum},
	{ 'E',umlaut},
	{ 'e',umlaut},
	{ 'E',grave},
	{ 'e',grave},
	{ 'I',acute},
	{ dotlesi,acute},
	{ 'I',circum},
	{ dotlesi,circum},
	{ 'I',umlaut},
	{ dotlesi,umlaut},
	{ 'I',grave},
	{ dotlesi,grave},
	{ 'N',tilde},
	{ 'n',tilde},
	{ 'O',acute},
	{ 'o',acute},
	{ 'O',circum},
	{ 'o',circum},
	{ 'O',umlaut},
	{ 'o',umlaut},
	{ 'O',grave},
	{ 'o',grave},
	{ 'U',acute},
	{ 'u',acute},
	{ 'U',circum},
	{ 'u',circum},
	{ 'U',umlaut},
	{ 'u',umlaut},
	{ 'U',grave},
	{ 'u',grave},
	{ 'Y',umlaut},
	{ 'y',umlaut},
	{ 'A',tilde},
	{ 'a',tilde},
	{ 'D',crossb},
	{ 'd',crossb},
	{ 'O',slash},
	{ 'o',slash},
	{ 'O',tilde},
	{ 'o',tilde},
	{ 'Y',acute},
	{ 'y',acute},
	{ 0xff,0xff},
	{ 0xff,0xff},
	{ 0xff,0xff},
	{ 0xff,0xff},
	{ 'A',breve},
	{ 'a',breve},
	{ 'A',macron},
	{ 'a',macron},
	{ 'A',ogonek},
	{ 'a',ogonek},
	{ 'C',acute},
	{ 'c',acute},
	{ 'C',caron},
	{ 'c',caron},
	{ 'C',circum},
	{ 'c',circum},
	{ 'C',dota},
	{ 'c',dota},
	{ 'D',caron},
	{ 'd',caron},
	{ 'E',caron},
	{ 'e',caron},
	{ 'E',dota},
	{ 'e',dota},
	{ 'E',macron},
	{ 'e',macron},
	{ 'E',ogonek},
	{ 'e',ogonek},
	{ 'G',acute},
	{ 'g',acute},
	{ 'G',breve},
	{ 'g',breve},
	{ 'G',caron},
	{ 'g',caron},
	{ 'G',cedilla},
	{ 'g',aposab},
	{ 'G',circum},
	{ 'g',circum},
	{ 'G',dota},
	{ 'g',dota},
	{ 'H',circum},
	{ 'h',circum},
	{ 'H',crossb},
	{ 'h',crossb},
	{ 'I',dota},
	{ dotlesi,dota},
	{ 'I',macron},
	{ dotlesi,macron},
	{ 'I',ogonek},
	{ 'i',ogonek},
	{ 'I',tilde},
	{ dotlesi,tilde},
	{ 0xff,0xff},
	{ 0xff,0xff},
	{ 'J',circum},
	{ dotlesj,circum},
	{ 'K',cedilla},
	{ 'k',cedilla},
	{ 'L',acute},
	{ 'l',acute},
	{ 'L',caron},
	{ 'l',caron},
	{ 'L',cedilla},
	{ 'l',cedilla},
	{ 'L',centerd},
	{ 'l',centerd},
	{ 'L',stroke},
	{ 'l',stroke},
	{ 'N',acute},
	{ 'n',acute},
	{ 'N',aposba},
	{ 'n',aposba},
	{ 'N',caron},
	{ 'n',caron},
	{ 'N',cedilla},
	{ 'n',cedilla},
	{ 'O',dacute},
	{ 'o',dacute},
	{ 'O',macron},
	{ 'o',macron},
	{ 0xff,0xff},
	{ 0xff,0xff},
	{ 'R',acute},
	{ 'r',acute},
	{ 'R',caron},
	{ 'r',caron},
	{ 'R',cedilla},
	{ 'r',cedilla},
	{ 'S',acute},
	{ 's',acute},
	{ 'S',caron},
	{ 's',caron},
	{ 'S',cedilla},
	{ 's',cedilla},
	{ 'S',circum},
	{ 's',circum},
	{ 'T',caron},
	{ 't',caron},
	{ 'T',cedilla},
	{ 't',cedilla},
	{ 'T',crossb},
	{ 't',crossb},
	{ 'U',breve},
	{ 'u',breve},
	{ 'U',dacute},
	{ 'u',dacute},
	{ 'U',macron},
	{ 'u',macron},
	{ 'U',ogonek},
	{ 'u',ogonek},
	{ 'U',ring},
	{ 'u',ring},
	{ 'U',tilde},
	{ 'u',tilde},
	{ 'W',circum},
	{ 'w',circum},
	{ 'Y',circum},
	{ 'y',circum},
	{ 'Z',acute},
	{ 'z',acute},
	{ 'Z',caron},
	{ 'z',caron},
	{ 'Z',dota},
	{ 'z',dota},
	{ 0xff,0xff},
	{ 0xff,0xff},
	{ 'D',macron},
	{'d',macron},
	{'L',macron},
	{'l',macron},
	{'N',macron},
	{'n',macron},
	{'R',grave},
	{'r',grave},
	{'S',macron},
	{'s',macron},
	{'T',macron},
	{'t',macron},
	{'Y',breve},
	{'y',breve},
	{'Y',grave},
	{'y',grave},
	{'D',aposbes},
	{'d',aposbes},
	{'O',aposbes},
	{'o',aposbes},
	{'U',aposbes},
	{'u',aposbes},
	{'E',breve},
	{'e',breve},
	{'I',breve},
	{dotlesi,breve},
	{0xff,0xff},
	{0xff,0xff},
	{'O',breve},
	{'o',breve}
};

/****************************************************************************
Desc:
****************************************************************************/
BASE_DIACRIT flm_ml1c = 
{
	216,    						 			// Number of characters in table
	26,       					 			// Start char
	flm_ml1c_table,
};

/****************************************************************************
Desc:
****************************************************************************/
static BASE_DIACRIT_TABLE flm_grk_c_table[] = 
{
	{  0, ghprime },						// ALPHA High Prime
	{  1, gacute },						// alpha acute
	{ 10, ghprime },						// EPSILON High Prime
	{ 11, gacute },						// epsilon Acute
	{ 14, ghprime },						// ETA High Prime
	{ 15, gacute },						// eta Acute
	{ 18, ghprime },						// IOTA High Prime
	{ 19, gacute },						// iota Acute
	{ 0xFF, 0xFF },						// IOTA Diaeresis
	{ 19, gdia },							// iota Diaeresis
	{ 30, ghprime },						// OMICRON High Prime
	{ 31, gacute },						// omicron Acute
	{ 42, ghprime },						// UPSILON High Prime
	{ 43, gacute },						// upsilon Acute
	{ 0xFF, 0xFF }, 						// UPSILON Diaeresis
	{ 43,gdia }, 							// upsilon Diaeresis
	{ 50,ghprime }, 						// OMEGA High Prime
	{ 51,gacute }, 						// omega Acute
	{ 0xFF, 0xFF },						// epsilon (Variant)
	{ 0xFF, 0xFF },						// theta (Variant)
	{ 0xFF, 0xFF },						// kappa (Variant)
	{ 0xFF, 0xFF },						// pi (Variant)
	{ 0xFF, 0xFF },						// rho (Variant)
	{ 0xFF, 0xFF },						// sigma (Variant)
	{ 0xFF, 0xFF },						// UPSILON (Variant)
	{ 0xFF, 0xFF },						// phi (Variant)
	{ 0xFF, 0xFF },						// omega (Variant)
	{ 0xFF, 0xFF },						// Greek Question Mark
	{ 0xFF, 0xFF },						// Greek Semicolon
	{ 0xFF, 0xFF },						// High Prime
	{ 0xFF, 0xFF },						// Low Prime
	{ 0xFF, 0xFF },						// Acute (Greek)
	{ 0xFF, 0xFF },						// Diaeresis (Greek)
	{ gacute,gdia },						// Acute Diaeresis
	{ ggrave, gdia },						// Grave Diaeresis
	{ 0xFF, 0xFF },						// Grave (Greek)
	{ 0xFF, 0xFF },						// Circumflex (Greek)
	{ 0xFF, 0xFF },						// Smooth Breathing
	{ 0xFF, 0xFF },						// Rough Breathing
	{ 0xFF, 0xFF },						// Iota Subscript
	{ gsmooth, gacute },					// Smooth Breathing Acute
	{ grough, gacute },					// Rough Breathing Acute
	{ gsmooth, ggrave },					// Smooth Breathing Grave
	{ grough, ggrave },					// Rough Breathing Grave
	{ gsmooth, gcircm },					// Smooth Breathing Circumflex
	{ grough, gcircm },					// Rough Breathing Circumflex
	{ gacute, giota },					// Acute w/Iota Subscript
	{ ggrave, giota },					// Grave w/Iota Subscript
	{ gcircm, giota },					// Circumflex w/Iota Subscript
	{ gsmooth, giota },					// Smooth Breathing w/Iota Subscript
	{ grough, giota },					// Rough Breathing w/Iota Subscript
	{ gsmact, giota },					// Smooth Breathing Acute w/Iota Subscript
	{ grgact, giota },					// Rough Breathing Acute w/Iota Subscript
	{ gsmgrv, giota },					// Smooth Breathing Grave w/Iota Subscript
	{ grggrv, giota },					// Rough Breathing Grave w/Iota Subscript
	{ gsmcir, giota },					// Smooth Breathing Circumflex w/Iota Sub
	{ grgcir, giota },					// Rough Breathing Circumflex w/Iota Sub
	{ 1, ggrave },							// alpha Grave
	{ 1, gcircm },							// alpha Circumflex
	{ 1, giota	 },						// alpha w/Iota
	{ 1, gactio },							// alpha Acute w/Iota
	{ 1, ggrvio },							// alpha Grave w/Iota
	{ 1, gcirio },							// alpha Circumflex w/Iota
	{ 1, gsmooth },						// alpha Smooth
	{ 1, gsmact },							// alpha Smooth Acute
	{ 1, gsmgrv },							// alpha Smooth Grave
	{ 1, gsmcir },							// alpha Smooth Circumflex
	{ 1, gsmio	 },						// alpha Smooth w/Iota
	{ 1, gsmaio },							// alpha Smooth Acute w/Iota
	{ 1, gsmgvio },						// alpha Smooth Grave w/Iota
	{ 1, gsmcio },							// alpha Smooth Circumflex w/Iota
	{ 1, grough },							// alpha Rough
	{ 1, grgact },							// alpha Rough Acute
	{ 1, grggrv },							// alpha Rough Grave
	{ 1, grgcir },							// alpha Rough Circumflex
	{ 1, grgio	 },						// alpha Rough w/Iota
	{ 1, grgaio },							// alpha Rough Acute w/Iota
	{ 1, grggvio },						// alpha Rough Grave w/Iota
	{ 1, grgcio },							// alpha Rough Circumflex w/Iota
	{ 11, ggrave },						// epsilon Grave
	{ 11, gsmooth },						// epsilon Smooth
	{ 11, gsmact },						// epsilon Smooth Acute
	{ 11, gsmgrv },						// epsilon Smooth Grave
	{ 11, grough },						// epsilon Rough
	{ 11, grgact },						// epsilon Rough Acute
	{ 11, grggrv },						// epsilon Rough Grave
	{ 15, ggrave },						// eta Grave
	{ 15, gcircm },						// eta Circumflex
	{ 15, giota },							// eta w/Iota
	{ 15, gactio },						// eta Acute w/Iota
	{ 15, ggrvio },						// eta Grave w/Iota
	{ 15, gcirio },						// eta Circumflex w/Iota
	{ 15, gsmooth },						// eta Smooth
	{ 15, gsmact },						// eta Smooth Acute
	{ 15, gsmgrv },						// eta Smooth Grave
	{ 15, gsmcir },						// eta Smooth Circumflex
	{ 15, gsmio },							// eta Smooth w/Iota
	{ 15, gsmaio },						// eta Smooth Acute w/Iota
	{ 15, gsmgvio },						// eta Smooth Grave w/Iota
	{ 15, gsmcio },						// eta Smooth Circumflex w/Iota
	{ 15, grough },						// eta Rough
	{ 15, grgact },						// eta Rough Acute
	{ 15, grggrv },						// eta Rough Grave
	{ 15, grgcir },						// eta Rough Circumflex
	{ 15, grgio },							// eta Rough w/Iota
	{ 15, grgaio },						// eta Rough Acute w/Iota
	{ 15, grggvio },						// eta Rough Grave w/Iota
	{ 15, grgcio },						// eta Rough Circumflex w/Iota
	{ 19, ggrave },						// iota Grave
	{ 19, gcircm },						// iota Circumflex
	{ 19, gactdia },						// iota Acute Diaeresis
	{ 19, ggrvdia },						// iota Grave Diaeresis
	{ 19, gsmooth },						// iota Smooth
	{ 19, gsmact },						// iota Smooth Acute
	{ 19, gsmgrv },						// iota Smooth Grave
	{ 19, gsmcir },						// iota Smooth Circumflex
	{ 19, grough },						// iota Rough
	{ 19, grgact },						// iota Rough Acute
	{ 19, grggrv },						// iota Rough Grave
	{ 19, grgcir },						// iota Rough Circumflex
	{ 31, ggrave },						// omicron Grave
	{ 31, gsmooth },						// omicron Smooth
	{ 31, gsmact },						// omicron Smooth Acute
	{ 31, gsmgrv },						// omicron Smooth Grave
	{ 31, grough },						// omicron Rough
	{ 31, grgact },						// omicron Rough Acute
	{ 31, grggrv },						// omicron Rough Grave
	{ 0xFF, 0xFF },						// rho rough
	{ 0xFF, 0xFF },						// rho smooth
	{ 43, ggrave },						// upsilon Grave
	{ 43, gcircm },						// upsilon Circumflex
	{ 43, gactdia },						// upsilon Acute Diaeresis
	{ 43, ggrvdia },						// upsilon Grave Diaeresis
	{ 43, gsmooth },						// upsilon Smooth
	{ 43, gsmact },						// upsilon Smooth Acute
	{ 43, gsmgrv },						// upsilon Smooth Grave
	{ 43, gsmcir },						// upsilon Smooth Circumflex
	{ 43, grough },						// upsilon Rough
	{ 43, grgact },						// upsilon Rough Acute
	{ 43, grggrv },						// upsilon Rough Grave
	{ 43, grgcir },						// upsilon Rough Circumflex
	{ 51, ggrave },						// omega Grave
	{ 51, gcircm },						// omega Circumflex
	{ 51, giota },							// omega w/Iota
	{ 51, gactio },						// omega Acute w/Iota
	{ 51, ggrvio },						// omega Grave w/Iota
	{ 51, gcirio },						// omega Circumflex w/Iota
	{ 51, gsmooth },						// omega Smooth
	{ 51, gsmact },						// omega Smooth Acute
	{ 51, gsmgrv },						// omega Smooth Grave
	{ 51, gsmcir },						// omega Smooth Circumflex
	{ 51, gsmio },							// omega Smooth w/Iota
	{ 51, gsmaio },						// omega Smooth Acute w/Iota
	{ 51, gsmgvio },						// omega Smooth Grave w/Iota
	{ 51, gsmcio },						// omega Smooth Circumflex w/Iota
	{ 51, grough },						// omega Rough
	{ 51, grgact },						// omega Rough Acute
	{ 51, grggrv },						// omega Rough Grave
	{ 51, grgcir },						// omega Rough Circumflex
	{ 51, grgio },							// omega Rough w/Iota
	{ 51, grgaio },						// omega Rough Acute w/Iota
	{ 51, grggvio },						// omega Rough Grave w/Iota
	{ 51, grgcio}							// omega Rough Circumflex w/Iota
};

/****************************************************************************
Desc:
****************************************************************************/
static BASE_DIACRIT flm_grk_c = 
{
	163,										// Number of characters in table
	52,										// Start char
	flm_grk_c_table
};

/****************************************************************************
Desc:
****************************************************************************/
static BASE_DIACRIT_TABLE flm_rus_c_table[] = 
{
	{ 14, 204 },							// ZHE with right descender
	{ 15, 204 },							// zhe with right descender
	{ 0xFF, 0xFF},							// DZE
	{ 0xFF, 0xFF},							// dze
	{ 0xFF, 0xFF},							// Z 
	{ 0xFF, 0xFF},							// z
	{ 18, 206 },							// II with macron
	{ 19, 206},								// ii with macron
	{ 0xFF, 0xFF},							// I
	{ 0xFF, 0xFF},							// i 
	{ 0xFF, 0xFF},							// YI
	{ 0xFF, 0xFF},							// yi
	{ 0xFF, 0xFF},							// I ligature
	{ 0xFF, 0xFF},							// i ligature
	{ 0xFF, 0xFF},							// JE
	{ 0xFF, 0xFF},							// je
	{ 0xFF, 0xFF},							// KJE
	{ 0xFF, 0xFF},							// kje
	{ 22, 204},								// KA with right descender
	{ 23, 204},								// ka with right descender
	{ 22, 205 },							// KA ogonek
	{ 23, 205 },							// ka ogonek 
	{ 0xFF, 0xFF},							// KA vertical bar
	{ 0xFF, 0xFF},							// ka vertical bar
	{ 0xFF, 0xFF},							// LJE
	{ 0xFF, 0xFF},							// lje
	{ 28, 204 },							// EN with right descender
	{ 29, 204 },							// en with right descender
	{ 0xFF, 0xFF},							// NJE
	{ 0xFF, 0xFF},							// nje
	{ 0xFF, 0xFF},							// ROUND OMEGA
	{ 0xFF, 0xFF},							// round omega
	{ 0xFF, 0xFF},							// OMEGA
	{ 0xFF, 0xFF},							// omega
	{ 0xFF, 0xFF},							// TSHE 
	{ 0xFF, 0xFF},							// tshe
	{ 0xFF, 0xFF},							// SHORT U
	{ 0xFF, 0xFF},							// short u
	{ 40, 206},								// U with macron
	{ 41, 206 },							// u with macron
	{ 0xFF, 0xFF},							// STRAIGHT U
	{ 0xFF, 0xFF},							// straight u
	{ 0xFF, 0xFF},							// STRAIGHT U BAR
	{ 0xFF, 0xFF},							// straight u bar
	{ 0xFF, 0xFF},							// OU ligature
	{ 0xFF, 0xFF},							// ou ligature
	{ 44, 204 },							// KHA with right descender
	{ 45, 204 },							// kha with right descender
	{ 44, 205 },							// KHA ogonek
	{ 45, 205 },							// kha ogonek
	{ 0xFF, 0xFF},							// H
	{ 0xFF, 0xFF},							// h
	{ 0xFF, 0xFF},							// OMEGA titlo
	{ 0xFF, 0xFF},							// omega titlo
	{ 0xFF, 0xFF},							// DZHE
	{ 0xFF, 0xFF},							// dzhe
	{ 48, 204 },							// CHE with right descender
	{ 49, 204 },							// che with right descender
	{ 0xFF, 0xFF},							// CHE vertical bar
	{ 0xFF, 0xFF},							// che vertical bar
	{ 0xFF, 0xFF},							// SHCHA (variant)
	{ 0xFF, 0xFF},							// shcha (variant)
	{ 0xFF, 0xFF},							// YAT
	{ 0xFF, 0xFF},							// yat
	{ 0xFF, 0xFF},							// YUS BOLSHOI
	{ 0xFF, 0xFF},							// yus bolshoi
	{ 0xFF, 0xFF},							// BIG MALYI
	{ 0xFF, 0xFF},							// big malyi
	{ 0xFF, 0xFF},							// KSI
	{ 0xFF, 0xFF},							// ksi
	{ 0xFF, 0xFF},							// PSI
	{ 0xFF, 0xFF},							// psi
	{ 0xFF, 0xFF},							// FITA
	{ 0xFF, 0xFF},							// fita
	{ 0xFF, 0xFF},							// IZHITSA
	{ 0xFF, 0xFF},							// izhitsa
	{ 00, racute},							// Russian A acute
	{ 01, racute },						// Russian a acute
	{ 10, racute },						// Russian IE acute
	{ 11, racute },						// Russian ie acute
	{ 78, racute },						// Russian E acute
	{ 79, racute },						// Russian e acute
	{ 18, racute },						// Russian II acute
	{ 19, racute },						// Russian ii acute
	{ 88, racute },						// Russian I acute
	{ 89, racute },						// Russian i acute
	{ 90, racute },						// Russian YI acute
	{ 91, racute },						// Russian yi acute
	{ 30, racute },						// Russian O acute
	{ 31, racute },						// Russian o acute
	{ 40, racute },						// Russian U acute
	{ 41, racute },						// Russian u acute
	{ 56, racute },						// Russian YERI acute
	{ 57, racute },						// Russian yeri acute
	{ 60, racute },						// Russian REVERSED E acute
	{ 61, racute },						// Russian reversed e acute
	{ 62, racute },						// Russian IU acute
	{ 63, racute },						// Russian iu acute
	{ 64, racute },						// Russian IA acute
	{ 65, racute },						// Russian ia acute
	{ 00, rgrave },						// Russian A grave
	{ 01, rgrave },						// Russian a grave
	{ 10, rgrave },						// Russian IE grave
	{ 11, rgrave },						// Russian ie grave
	{ 12, rgrave },						// Russian YO grave
	{ 13, rgrave },						// Russian yo grave
	{ 18, rgrave },						// Russian I grave
	{ 19, rgrave },						// Russian i grave
	{ 30, rgrave },						// Russian O grave
	{ 31, rgrave },						// Russian o grave
	{ 40, rgrave },						// Russian U grave
	{ 41, rgrave },						// Russian u grave
	{ 56, rgrave },						// Russian YERI grave
	{ 57, rgrave },						// Russian yeri grave
	{ 60, rgrave },						// Russian REVERSED E grave
	{ 61, rgrave },						// Russian reversed e grave
	{ 62, rgrave },						// Russian IU grave
	{ 63, rgrave },						// Russian iu grave
	{ 64, rgrave },						// Russian IA grave
	{ 65, rgrave}							// Russian ia grave
};

/****************************************************************************
Desc:
****************************************************************************/
static BASE_DIACRIT flm_rus_c = {
	120,										// Number of characters in table
	156,										// Start char
	flm_rus_c_table,
};

/****************************************************************************
Desc:
****************************************************************************/
BASE_DIACRIT * flm_car60_c[ NCHSETS] = 
{
	(BASE_DIACRIT*)0,						// no composed characters for ascii
	&flm_ml1c,
	(BASE_DIACRIT*)0,						// no composed characters for multinational 2
	(BASE_DIACRIT*)0,						// no composed characters for line draw
	(BASE_DIACRIT*)0,						// no composed characters for typographic
	(BASE_DIACRIT*)0,						// no composed characters for icons
	(BASE_DIACRIT*)0,						// no composed characters for math
	(BASE_DIACRIT*)0,						// no composed characters for math extension
	&flm_grk_c,								// Greek
	(BASE_DIACRIT*)0,						// Hebrew
	&flm_rus_c,								// Cyrillic - Russian
	(BASE_DIACRIT*)0,						// Hiragana or Katakana (Japanese)
	(BASE_DIACRIT*)0,						// no composed characters for user
	(BASE_DIACRIT*)0,						// no composed characters for Arabic
	(BASE_DIACRIT*)0,						// no composed characters for Arabic Script
};

/****************************************************************************
Desc:	Number of characters in each character set
****************************************************************************/
FLMBYTE flm_c60_max[] = 
{
	ASC_N,									// ascii
	ML1_N,									// multinational 1
	ML2_N,									// multinational 2
	BOX_N,									// line draw
	TYP_N,									// typographic
	ICN_N,									// icons
	MTH_N,									// math
	MTX_N,									// math extension
	GRK_N,									// Greek
	HEB_N,									// Hebrew
	CYR_N,									// Cyrillic - Russian
	KAN_N,									// Kana
	USR_N,									// user
	ARB_N,									// Arabic
	ARS_N,									// Arabic Script
};

/****************************************************************************
Desc:
****************************************************************************/
FLMUINT16 WP_UTOWP60[ UTOWP60_ENTRIES][ 2] =
{
	{ 0x00A1, 0x0407 },					//  7 ,  4,
	{ 0x00A2, 0x0413 },					// 19 ,  4,
	{ 0x00A3, 0x040b },					// 11 ,  4,
	{ 0x00A4, 0x0418 },					// 24 ,  4,
	{ 0x00A5, 0x040c },					// 12 ,  4,
	{ 0x00A7, 0x0406 },					//  6 ,  4,
	{ 0x00A9, 0x0417 },					// 23 ,  4,
	{ 0x00AA, 0x040f },					// 15 ,  4,
	{ 0x00AB, 0x0409 },					//  9 ,  4,
	{ 0x00AC, 0x0614 },					// 20 ,  6,
	{ 0x00AE, 0x0416 },					// 22 ,  4,
	{ 0x00B0, 0x0624 },					// 36 ,  6,
	{ 0x00B1, 0x0601 },					//  1 ,  6,
	{ 0x00B2, 0x0414 },					// 20 ,  4,
	{ 0x00B3, 0x041a },					// 26 ,  4,
	{ 0x00B5, 0x0625 },					// 37 ,  6,
	{ 0x00B6, 0x0405 },					//  5 ,  4,
	{ 0x00B7, 0x0101 },					// 101,  1,
	{ 0x00B9, 0x044e },					// 78 ,  4,
	{ 0x00BA, 0x0410 },					// 16 ,  4,
	{ 0x00BB, 0x040a },					// 10 ,  4,
	{ 0x00BC, 0x0412 },					// 18 ,  4,
	{ 0x00BD, 0x0411 },					// 17 ,  4,
	{ 0x00BE, 0x0419 },					// 25 ,  4,
	{ 0x00BF, 0x0408 },					//  8 ,  4,
	{ 0x00C0, 0x0120 },					// 32 ,  1,
	{ 0x00C1, 0x011a },					// 26 ,  1,
	{ 0x00C2, 0x011c },					// 28 ,  1,
	{ 0x00C3, 0x014c },					// 76 ,  1,
	{ 0x00C4, 0x011e },					// 30 ,  1,
	{ 0x00C5, 0x0122 },					// 34 ,  1,
	{ 0x00C6, 0x0124 },					// 36 ,  1,
	{ 0x00C7, 0x0126 },					// 38 ,  1,
	{ 0x00C8, 0x012e },					// 46 ,  1,
	{ 0x00C9, 0x0128 },					// 40 ,  1,
	{ 0x00CA, 0x012a },					// 42 ,  1,
	{ 0x00CB, 0x012c },					// 44 ,  1,
	{ 0x00CC, 0x0136 },					// 54 ,  1,
	{ 0x00CD, 0x0130 },					// 48 ,  1,
	{ 0x00CE, 0x0132 },					// 50 ,  1,
	{ 0x00CF, 0x0134 },					// 52 ,  1,
	{ 0x00D0, 0x0156 },					// 86 ,  1,
	{ 0x00D1, 0x0138 },					// 56 ,  1,
	{ 0x00D2, 0x0140 },					// 64 ,  1,
	{ 0x00D3, 0x013a },					// 58 ,  1,
	{ 0x00D4, 0x013c },					// 60 ,  1,
	{ 0x00D5, 0x0152 },					// 82 ,  1,
	{ 0x00D6, 0x013e },					// 62 ,  1,
	{ 0x00D7, 0x0627 },					// 39 ,  6,
	{ 0x00D8, 0x0150 },					// 80 ,  1,
	{ 0x00D9, 0x0148 },					// 72 ,  1,
	{ 0x00DA, 0x0142 },					// 66 ,  1,
	{ 0x00DB, 0x0144 },					// 68 ,  1,
	{ 0x00DC, 0x0146 },					// 70 ,  1,
	{ 0x00DD, 0x0154 },					// 84 ,  1,
	{ 0x00DE, 0x0158 },					// 88 ,  1,
	{ 0x00DF, 0x0117 },					// 23 ,  1,
	{ 0x00E0, 0x0121 },					// 33 ,  1,
	{ 0x00E1, 0x011b },					// 27 ,  1,
	{ 0x00E2, 0x011d },					// 29 ,  1,
	{ 0x00E3, 0x014d },					// 77 ,  1,
	{ 0x00E4, 0x011f },					// 31 ,  1,
	{ 0x00E5, 0x0123 },					// 35 ,  1,
	{ 0x00E6, 0x0125 },					// 37 ,  1,
	{ 0x00E7, 0x0127 },					// 39 ,  1,
	{ 0x00E8, 0x012f },					// 47 ,  1,
	{ 0x00E9, 0x0129 },					// 41 ,  1,
	{ 0x00EA, 0x012b },					// 43 ,  1,
	{ 0x00EB, 0x012d },					// 45 ,  1,
	{ 0x00EC, 0x0137 },					// 55 ,  1,
	{ 0x00ED, 0x0131 },					// 49 ,  1,
	{ 0x00EE, 0x0133 },					// 51 ,  1,
	{ 0x00EF, 0x0135 },					// 53 ,  1,
	{ 0x00F0, 0x0157 },					// 87 ,  1,
	{ 0x00F1, 0x0139 },					// 57 ,  1,
	{ 0x00F2, 0x0141 },					// 65 ,  1,
	{ 0x00F3, 0x013b },					// 59 ,  1,
	{ 0x00F4, 0x013d },					// 61 ,  1,
	{ 0x00F5, 0x0153 },					// 83 ,  1,
	{ 0x00F6, 0x013f },					// 63 ,  1,
	{ 0x00F7, 0x0608 },					//  8 ,  6,
	{ 0x00F8, 0x0151 },					// 81 ,  1,
	{ 0x00F9, 0x0149 },					// 73 ,  1,
	{ 0x00FA, 0x0143 },					// 67 ,  1,
	{ 0x00FB, 0x0145 },					// 69 ,  1,
	{ 0x00FC, 0x0147 },					// 71 ,  1,
	{ 0x00FD, 0x0155 },					// 85 ,  1,
	{ 0x00FE, 0x0159 },					// 89 ,  1,
	{ 0x00FF, 0x014b },					// 75 ,  1,
	{ 0x0100, 0x015c },					// 92 ,  1,
	{ 0x0101, 0x015d },					// 93 ,  1,
	{ 0x0102, 0x015a },					// 90 ,  1,
	{ 0x0103, 0x015b },					// 91 ,  1,
	{ 0x0104, 0x015e },					// 94 ,  1,
	{ 0x0105, 0x015f },					// 95 ,  1,
	{ 0x0106, 0x0160 },					// 96 ,  1,
	{ 0x0107, 0x0161 },					// 97 ,  1,
	{ 0x0108, 0x0164 },					// 100,  1,
	{ 0x0109, 0x0165 },					// 101,  1,
	{ 0x010A, 0x0166 },					// 102,  1,
	{ 0x010B, 0x0167 },					// 103,  1,
	{ 0x010C, 0x0162 },					// 98 ,  1,
	{ 0x010D, 0x0163 },					// 99 ,  1,
	{ 0x010E, 0x0168 },					// 104,  1,
	{ 0x010F, 0x0169 },					// 105,  1,
	{ 0x0110, 0x014e },					// 78 ,  1,
	{ 0x0111, 0x014f },					// 79 ,  1,
	{ 0x0112, 0x016e },					// 110,  1,
	{ 0x0113, 0x016f },					// 111,  1,
	{ 0x0114, 0x01ea },					// 234,  1,
	{ 0x0115, 0x01eb },					// 235,  1,
	{ 0x0116, 0x016c },					// 108,  1,
	{ 0x0117, 0x016d },					// 109,  1,
	{ 0x0118, 0x0170 },					// 112,  1,
	{ 0x0119, 0x0171 },					// 113,  1,
	{ 0x011A, 0x016a },					// 106,  1,
	{ 0x011B, 0x016b },					// 107,  1,
	{ 0x011C, 0x017a },					// 122,  1,
	{ 0x011D, 0x017b },					// 123,  1,
	{ 0x011E, 0x0174 },					// 116,  1,
	{ 0x011F, 0x0175 },					// 117,  1,
	{ 0x0120, 0x017c },					// 124,  1,
	{ 0x0121, 0x017d },					// 125,  1,
	{ 0x0122, 0x0178 },					// 120,  1,
	{ 0x0123, 0x0179 },					// 121,  1,
	{ 0x0124, 0x017e },					// 126,  1,
	{ 0x0125, 0x017f },					// 127,  1,
	{ 0x0126, 0x0180 },					// 128,  1,
	{ 0x0127, 0x0181 },					// 129,  1,
	{ 0x0128, 0x0188 },					// 136,  1,
	{ 0x0129, 0x0189 },					// 137,  1,
	{ 0x012A, 0x0184 },					// 132,  1,
	{ 0x012B, 0x0185 },					// 133,  1,
	{ 0x012C, 0x01ec },					// 236,  1,
	{ 0x012D, 0x01ed },					// 237,  1,
	{ 0x012E, 0x0186 },					// 134,  1,
	{ 0x012F, 0x0187 },					// 135,  1,
	{ 0x0130, 0x0182 },					// 130,  1,
	{ 0x0131, 0x01ef },					// 239,  1,
	{ 0x0132, 0x018a },					// 138,  1,
	{ 0x0133, 0x018b },					// 139,  1,
	{ 0x0134, 0x018c },					// 140,  1,
	{ 0x0135, 0x018d },					// 141,  1,
	{ 0x0136, 0x018e },					// 142,  1,
	{ 0x0137, 0x018f },					// 143,  1,
	{ 0x0138, 0x0118 },					// 24 ,  1,
	{ 0x0139, 0x0190 },					// 144,  1,
	{ 0x013A, 0x0191 },					// 145,  1,
	{ 0x013B, 0x0194 },					// 148,  1,
	{ 0x013C, 0x0195 },					// 149,  1,
	{ 0x013D, 0x0192 },					// 146,  1,
	{ 0x013E, 0x0193 },					// 147,  1,
	{ 0x013F, 0x0196 },					// 150,  1,
	{ 0x0140, 0x0197 },					// 151,  1,
	{ 0x0141, 0x0198 },					// 152,  1,
	{ 0x0142, 0x0199 },					// 153,  1,
	{ 0x0143, 0x019a },					// 154,  1,
	{ 0x0144, 0x019b },					// 155,  1,
	{ 0x0145, 0x01a0 },					// 160,  1,
	{ 0x0146, 0x01a1 },					// 161,  1,
	{ 0x0147, 0x019e },					// 158,  1,
	{ 0x0148, 0x019f },					// 159,  1,
	{ 0x0149, 0x019d },					// 157,  1,
	{ 0x014A, 0x01d2 },					// 210,  1,
	{ 0x014B, 0x01d3 },					// 211,  1,
	{ 0x014C, 0x01a4 },					// 164,  1,
	{ 0x014D, 0x01a5 },					// 165,  1,
	{ 0x014E, 0x01f0 },					// 240,  1,
	{ 0x014F, 0x01f1 },					// 241,  1,
	{ 0x0150, 0x01a2 },					// 162,  1,
	{ 0x0151, 0x01a3 },					// 163,  1,
	{ 0x0152, 0x01a6 },					// 166,  1,
	{ 0x0153, 0x01a7 },					// 167,  1,
	{ 0x0154, 0x01a8 },					// 168,  1,
	{ 0x0155, 0x01a9 },					// 169,  1,
	{ 0x0156, 0x01ac },					// 172,  1,
	{ 0x0157, 0x01ad },					// 173,  1,
	{ 0x0158, 0x01aa },					// 170,  1,
	{ 0x0159, 0x01ab },					// 171,  1,
	{ 0x015A, 0x01ae },					// 174,  1,
	{ 0x015B, 0x01af },					// 175,  1,
	{ 0x015C, 0x01b4 },					// 180,  1,
	{ 0x015D, 0x01b5 },					// 181,  1,
	{ 0x015E, 0x01b2 },					// 178,  1,
	{ 0x015F, 0x01b3 },					// 179,  1,
	{ 0x0160, 0x01b0 },					// 176,  1,
	{ 0x0161, 0x01b1 },					// 177,  1,
	{ 0x0162, 0x01b8 },					// 184,  1,
	{ 0x0163, 0x01b9 },					// 185,  1,
	{ 0x0164, 0x01b6 },					// 182,  1,
	{ 0x0165, 0x01b7 },					// 183,  1,
	{ 0x0166, 0x01ba },					// 186,  1,
	{ 0x0167, 0x01bb },					// 187,  1,
	{ 0x0168, 0x01c6 },					// 198,  1,
	{ 0x0169, 0x01c7 },					// 199,  1,
	{ 0x016A, 0x01c0 },					// 192,  1,
	{ 0x016B, 0x01c1 },					// 193,  1,
	{ 0x016C, 0x01bc },					// 188,  1,
	{ 0x016D, 0x01bd },					// 189,  1,
	{ 0x016E, 0x01c4 },					// 196,  1,
	{ 0x016F, 0x01c5 },					// 197,  1,
	{ 0x0170, 0x01be },					// 190,  1,
	{ 0x0171, 0x01bf },					// 191,  1,
	{ 0x0172, 0x01c2 },					// 194,  1,
	{ 0x0173, 0x01c3 },					// 195,  1,
	{ 0x0174, 0x01c8 },					// 200,  1,
	{ 0x0175, 0x01c9 },					// 201,  1,
	{ 0x0176, 0x01ca },					// 202,  1,
	{ 0x0177, 0x01cb },					// 203,  1,
	{ 0x0178, 0x014a },					// 74 ,  1,
	{ 0x0179, 0x01cc },					// 204,  1,
	{ 0x017A, 0x01cd },					// 205,  1,
	{ 0x017B, 0x01d0 },					// 208,  1,
	{ 0x017C, 0x01d1 },					// 209,  1,
	{ 0x017D, 0x01ce },					// 206,  1,
	{ 0x017E, 0x01cf },					// 207,  1,
	{ 0x0192, 0x040e },					// 14 ,  4,
	{ 0x0194, 0x0a7c },					// 124, 10,
	{ 0x01A0, 0x01e6 },					// 230,  1,
	{ 0x01A1, 0x01e7 },					// 231,  1,
	{ 0x01AF, 0x01e8 },					// 232,  1,
	{ 0x01B0, 0x01e9 },					// 233,  1,
	{ 0x01C0, 0x0605 },					//  5 ,  6,
	{ 0x0250, 0x0237 },					// 55 ,  2,
	{ 0x0251, 0x0238 },					// 56 ,  2,
	{ 0x0252, 0x0239 },					// 57 ,  2,
	{ 0x0253, 0x023a },					// 58 ,  2,
	{ 0x0254, 0x023c },					// 60 ,  2,
	{ 0x0255, 0x023d },					// 61 ,  2,
	{ 0x0256, 0x023f },					// 63 ,  2,
	{ 0x0257, 0x0240 },					// 64 ,  2,
	{ 0x0258, 0x0241 },					// 65 ,  2,
	{ 0x0259, 0x0242 },					// 66 ,  2,
	{ 0x025A, 0x0243 },					// 67 ,  2,
	{ 0x025B, 0x0244 },					// 68 ,  2,
	{ 0x025C, 0x0245 },					// 69 ,  2,
	{ 0x025D, 0x0246 },					// 70 ,  2,
	{ 0x025E, 0x0248 },					// 72 ,  2,
	{ 0x025F, 0x0249 },					// 73 ,  2,
	{ 0x0260, 0x024c },					// 76 ,  2,
	{ 0x0261, 0x024b },					// 75 ,  2,
	{ 0x0262, 0x024d },					// 77 ,  2,
	{ 0x0263, 0x024f },					// 79 ,  2,
	{ 0x0264, 0x0250 },					// 80 ,  2,
	{ 0x0265, 0x0251 },					// 81 ,  2,
	{ 0x0266, 0x0252 },					// 82 ,  2,
	{ 0x0267, 0x0253 },					// 83 ,  2,
	{ 0x0268, 0x0255 },					// 85 ,  2,
	{ 0x0269, 0x0257 },					// 87 ,  2,
	{ 0x026A, 0x0256 },					// 86 ,  2,
	{ 0x026B, 0x025a },					// 90 ,  2,
	{ 0x026C, 0x025b },					// 91 ,  2,
	{ 0x026D, 0x025c },					// 92 ,  2,
	{ 0x026E, 0x025e },					// 94 ,  2,
	{ 0x026F, 0x0260 },					// 96 ,  2,
	{ 0x0270, 0x0261 },					// 97 ,  2,
	{ 0x0271, 0x0262 },					// 98 ,  2,
	{ 0x0272, 0x0263 },					// 99 ,  2,
	{ 0x0273, 0x0264 },					// 100,  2,
	{ 0x0274, 0x0265 },					// 101,  2,
	{ 0x0275, 0x0279 },					// 121,  2,
	{ 0x0276, 0x0266 },					// 102,  2,
	{ 0x0277, 0x0267 },					// 103,  2,
	{ 0x0278, 0x024a },					// 74 ,  2,
	{ 0x0279, 0x0269 },					// 105,  2,
	{ 0x027A, 0x026a },					// 106,  2,
	{ 0x027B, 0x026b },					// 107,  2,
	{ 0x027C, 0x026c },					// 108,  2,
	{ 0x027D, 0x026d },					// 109,  2,
	{ 0x027E, 0x026e },					// 110,  2,
	{ 0x027F, 0x026f },					// 111,  2,
	{ 0x0280, 0x0270 },					// 112,  2,
	{ 0x0281, 0x0271 },					// 113,  2,
	{ 0x0282, 0x0272 },					// 114,  2,
	{ 0x0283, 0x0273 },					// 115,  2,
	{ 0x0284, 0x0274 },					// 116,  2,
	{ 0x0285, 0x0275 },					// 117,  2,
	{ 0x0286, 0x0276 },					// 118,  2,
	{ 0x0287, 0x0277 },					// 119,  2,
	{ 0x0288, 0x0278 },					// 120,  2,
	{ 0x0289, 0x027a },					// 122,  2,
	{ 0x028A, 0x027b },					// 123,  2,
	{ 0x028B, 0x027d },					// 125,  2,
	{ 0x028C, 0x027c },					// 124,  2,
	{ 0x028D, 0x027e },					// 126,  2,
	{ 0x028E, 0x025f },					// 95 ,  2,
	{ 0x028F, 0x0280 },					// 128,  2,
	{ 0x0290, 0x0281 },					// 129,  2,
	{ 0x0291, 0x0282 },					// 130,  2,
	{ 0x0292, 0x0283 },					// 131,  2,
	{ 0x0293, 0x0284 },					// 132,  2,
	{ 0x0294, 0x0285 },					// 133,  2,
	{ 0x0295, 0x0286 },					// 134,  2,
	{ 0x0296, 0x0287 },					// 135,  2,
	{ 0x0297, 0x023e },					// 62 ,  2,
	{ 0x0298, 0x028a },					// 138,  2,
	{ 0x0299, 0x023b },					// 59 ,  2,
	{ 0x029A, 0x0247 },					// 71 ,  2,
	{ 0x029B, 0x024e },					// 78 ,  2,
	{ 0x029C, 0x0254 },					// 84 ,  2,
	{ 0x029D, 0x0258 },					// 88 ,  2,
	{ 0x029E, 0x0259 },					// 89 ,  2,
	{ 0x029F, 0x025d },					// 93 ,  2,
	{ 0x02A0, 0x0268 },					//	104,  2,
	{ 0x02A1, 0x0288 },					// 136,  2,
	{ 0x02A2, 0x0289 },					// 137,  2,
	{ 0x02A3, 0x028b },					// 139,  2,
	{ 0x02A4, 0x028c },					// 140,  2,
	{ 0x02A5, 0x028d },					// 141,  2,
	{ 0x02A6, 0x028e },					// 142,  2,
	{ 0x02A7, 0x028f },					// 143,  2,
	{ 0x02A8, 0x0290 },					// 144,  2,
	{ 0x02B0, 0x0235 },					// 53 ,  2,
	{ 0x02B6, 0x0236 },					// 54 ,  2,
	{ 0x02B9, 0x0200 },					//  0 ,  2,
	{ 0x02BA, 0x0201 },					//  1 ,  2,
	{ 0x02BB, 0x0202 },					//  2 ,  2,
	{ 0x02BC, 0x0205 },					//  5 ,  2,
	{ 0x02BD, 0x0204 },					//  4 ,  2,
	{ 0x02BE, 0x0207 },					//  7 ,  2,
	{ 0x02BF, 0x0208 },					//  8 ,  2,
	{ 0x02C6, 0x0217 },					// 23 ,  2,
	{ 0x02C7, 0x0218 },					// 24 ,  2,
	{ 0x02C8, 0x020f },					// 15 ,  2,
	{ 0x02C9, 0x0211 },					// 17 ,  2,
	{ 0x02CA, 0x0212 },					// 18 ,  2,
	{ 0x02CB, 0x0213 },					// 19 ,  2,
	{ 0x02CC, 0x0210 },					// 16 ,  2,
	{ 0x02CD, 0x0214 },					// 20 ,  2,
	{ 0x02CE, 0x0215 },					// 21 ,  2,
	{ 0x02CF, 0x0216 },					// 22 ,  2,
	{ 0x02D0, 0x020a },					// 10 ,  2,
	{ 0x02D1, 0x020b },					// 11 ,  2,
	{ 0x02D2, 0x022a },					// 42 ,  2,
	{ 0x02D3, 0x022b },					// 43 ,  2,
	{ 0x02DA, 0x021b },					// 27 ,  2,
	{ 0x02DB, 0x0231 },					// 49 ,  2,
	{ 0x02DC, 0x0219 },					// 25 ,  2,
	{ 0x02DE, 0x0233 },					// 51 ,  2,
	{ 0x0300, 0x0100 },					//  0 ,  1,
	{ 0x0301, 0x0106 },					//  6 ,  1,
	{ 0x0302, 0x0103 },					//  3 ,  1,
	{ 0x0303, 0x0102 },					//  2 ,  1,
	{ 0x0304, 0x0108 },					//  8 ,  1,
	{ 0x0305, 0x0115 },					// 21 ,  1,
	{ 0x0306, 0x0116 },					// 22 ,  1,
	{ 0x0307, 0x010f },					// 15 ,  1,
	{ 0x0308, 0x0107 },					//  7 ,  1,
	{ 0x030A, 0x010e },					// 14 ,  1,
	{ 0x030B, 0x0110 },					// 16 ,  1,
	{ 0x030C, 0x0113 },					// 19 ,  1,
	{ 0x0310, 0x0209 },					//  9 ,  2,
	{ 0x0311, 0x0858 },					// 88 ,  8,
	{ 0x0313, 0x0109 },					//  9 ,  1,
	{ 0x0314, 0x085a },					// 90 ,  8,
	{ 0x0315, 0x010a },					// 10 ,  1,
	{ 0x031C, 0x0221 },					// 33 ,  2,
	{ 0x031D, 0x0222 },					// 34 ,  2,
	{ 0x031E, 0x0223 },					// 35 ,  2,
	{ 0x031F, 0x0224 },					// 36 ,  2,
	{ 0x0320, 0x0225 },					// 37 ,  2,
	{ 0x0321, 0x0226 },					// 38 ,  2,
	{ 0x0322, 0x0227 },					// 39 ,  2,
	{ 0x0323, 0x021e },					// 30 ,  2,
	{ 0x0324, 0x0220 },					// 32 ,  2,
	{ 0x0325, 0x021a },					// 26 ,  2,
	{ 0x0326, 0x010c },					// 12 ,  1,
	{ 0x0327, 0x0111 },					// 17 ,  1,
	{ 0x0328, 0x0112 },					// 18 ,  1,
	{ 0x0329, 0x020e },					// 14 ,  2,
	{ 0x032A, 0x0228 },					// 40 ,  2,
	{ 0x032B, 0x0229 },					// 41 ,  2,
	{ 0x032C, 0x021d },					// 29 ,  2,
	{ 0x032D, 0x021c },					// 28 ,  2,
	{ 0x032E, 0x020d },					// 13 ,  2,
	{ 0x0335, 0x0104 },					//  4 ,  1,
	{ 0x0337, 0x0114 },					// 20 ,  1,
	{ 0x0338, 0x0105 },					//  5 ,  1,
	{ 0x033E, 0x0230 },					// 48 ,  2,
	{ 0x0345, 0x085b },					// 91 ,  8,
	{ 0x0374, 0x0851 },					// 81 ,  8,
	{ 0x0375, 0x0852 },					// 82 ,  8,
	{ 0x0391, 0x0800 },					//  0 ,  8,
	{ 0x0392, 0x0802 },					//  2 ,  8,
	{ 0x0393, 0x0806 },					//  6 ,  8,
	{ 0x0394, 0x0808 },					//  8 ,  8,
	{ 0x0395, 0x080a },					// 10 ,  8,
	{ 0x0396, 0x080c },					// 12 ,  8,
	{ 0x0397, 0x080e },					// 14 ,  8,
	{ 0x0398, 0x0810 },					// 16 ,  8,
	{ 0x0399, 0x0812 },					// 18 ,  8,
	{ 0x039A, 0x0814 },					// 20 ,  8,
	{ 0x039B, 0x0816 },					// 22 ,  8,
	{ 0x039C, 0x0818 },					// 24 ,  8,
	{ 0x039D, 0x081a },					// 26 ,  8,
	{ 0x039E, 0x081c },					// 28 ,  8,
	{ 0x039F, 0x081e },					// 30 ,  8,
	{ 0x03A0, 0x0820 },					// 32 ,  8,
	{ 0x03A1, 0x0822 },					// 34 ,  8,
	{ 0x03A3, 0x0824 },					// 36 ,  8,
	{ 0x03A4, 0x0828 },					// 40 ,  8,
	{ 0x03A5, 0x082a },					// 42 ,  8,
	{ 0x03A6, 0x082c },					// 44 ,  8,
	{ 0x03A7, 0x082e },					// 46 ,  8,
	{ 0x03A8, 0x0830 },					// 48 ,  8,
	{ 0x03A9, 0x0832 },					// 50 ,  8,
	{ 0x03AA, 0x083c },					// 60 ,  8,
	{ 0x03AB, 0x0842 },					// 66 ,  8,
	{ 0x03AC, 0x0835 },					// 53 ,  8,
	{ 0x03AD, 0x0837 },					// 55 ,  8,
	{ 0x03AE, 0x0839 },					// 57 ,  8,
	{ 0x03AF, 0x083b },					// 59 ,  8,
	{ 0x03B1, 0x0801 },					//  1 ,  8,
	{ 0x03B2, 0x0803 },					//  3 ,  8,
	{ 0x03B3, 0x0807 },					//  7 ,  8,
	{ 0x03B4, 0x0809 },					//  9 ,  8,
	{ 0x03B5, 0x080b },					// 11 ,  8,
	{ 0x03B6, 0x080d },					// 13 ,  8,
	{ 0x03B7, 0x080f },					// 15 ,  8,
	{ 0x03B8, 0x0811 },					// 17 ,  8,
	{ 0x03B9, 0x0813 },					// 19 ,  8,
	{ 0x03BA, 0x0815 },					// 21 ,  8,
	{ 0x03BB, 0x0817 },					// 23 ,  8,
	{ 0x03BC, 0x0819 },					// 25 ,  8,
	{ 0x03BD, 0x081b },					// 27 ,  8,
	{ 0x03BE, 0x081d },					// 29 ,  8,
	{ 0x03BF, 0x081f },					// 31 ,  8,
	{ 0x03C0, 0x0821 },					// 33 ,  8,
	{ 0x03C1, 0x0823 },					// 35 ,  8,
	{ 0x03C2, 0x0827 },					// 39 ,  8,
	{ 0x03C3, 0x0825 },					// 37 ,  8,
	{ 0x03C4, 0x0829 },					// 41 ,  8,
	{ 0x03C5, 0x082b },					// 43 ,  8,
	{ 0x03C6, 0x082d },					// 45 ,  8,
	{ 0x03C7, 0x082f },					// 47 ,  8,
	{ 0x03C8, 0x0831 },					// 49 ,  8,
	{ 0x03C9, 0x0833 },					// 51 ,  8,
	{ 0x03CA, 0x083d },					// 61 ,  8,
	{ 0x03CB, 0x0843 },					// 67 ,  8,
	{ 0x03CC, 0x083f },					// 63 ,  8,
	{ 0x03CD, 0x0841 },					// 65 ,  8,
	{ 0x03CE, 0x0845 },					// 69 ,  8,
	{ 0x03D0, 0x0805 },					//  5 ,  8,
	{ 0x03D1, 0x0847 },					// 71 ,  8,
	{ 0x03D2, 0x084c },					// 76 ,  8,
	{ 0x03D5, 0x084d },					// 77 ,  8,
	{ 0x03D6, 0x0849 },					// 73 ,  8,
	{ 0x03D7, 0x084f },					// 79 ,  8,
	{ 0x03DA, 0x08d7 },					// 215,  8,
	{ 0x03DB, 0x084B },					// 75 ,  8,
	{ 0x03DC, 0x08d8 },					// 216,  8,
	{ 0x03DE, 0x08d9 },					// 217,  8,
	{ 0x03E0, 0x08da },					// 218,  8,
	{ 0x03F0, 0x0848 },					// 72 ,  8,
	{ 0x03F1, 0x084a },					// 74 ,  8,
	{ 0x0401, 0x0a0c },					// 12 , 10,
	{ 0x0402, 0x0a4a },					// 74 , 10,
	{ 0x0403, 0x0a44 },					// 68 , 10,
	{ 0x0404, 0x0a4e },					// 78 , 10,
	{ 0x0405, 0x0a52 },					// 82 , 10,
	{ 0x0406, 0x0a58 },					// 88 , 10,
	{ 0x0407, 0x0a5a },					// 90 , 10,
	{ 0x0408, 0x0a5e },					// 94 , 10,
	{ 0x0409, 0x0a68 },					// 104, 10,
	{ 0x040A, 0x0a6c },					// 108, 10,
	{ 0x040B, 0x0a72 },					// 114, 10,
	{ 0x040C, 0x0a60 },					// 96 , 10,
	{ 0x040E, 0x0a74 },					// 116, 10,
	{ 0x040F, 0x0a86 },					// 134, 10,
	{ 0x0410, 0x0a00 },					//  0 , 10,
	{ 0x0411, 0x0a02 },					//  2 , 10,
	{ 0x0412, 0x0a04 },					//  4 , 10,
	{ 0x0413, 0x0a06 },					//  6 , 10,
	{ 0x0414, 0x0a08 },					//  8 , 10,
	{ 0x0415, 0x0a0a },					// 10 , 10,
	{ 0x0416, 0x0a0e },					// 14 , 10,
	{ 0x0417, 0x0a10 },					// 16 , 10,
	{ 0x0418, 0x0a12 },					// 18 , 10,
	{ 0x0419, 0x0a14 },					// 20 , 10,
	{ 0x041A, 0x0a16 },					// 22 , 10,
	{ 0x041B, 0x0a18 },					// 24 , 10,
	{ 0x041C, 0x0a1a },					// 26 , 10,
	{ 0x041D, 0x0a1c },					// 28 , 10,
	{ 0x041E, 0x0a1e },					// 30 , 10,
	{ 0x041F, 0x0a20 },					// 32 , 10,
	{ 0x0420, 0x0a22 },					// 34 , 10,
	{ 0x0421, 0x0a24 },					// 36 , 10,
	{ 0x0422, 0x0a26 },					// 38 , 10,
	{ 0x0423, 0x0a28 },					// 40 , 10,
	{ 0x0424, 0x0a2a },					// 42 , 10,
	{ 0x0425, 0x0a2c },					// 44 , 10,
	{ 0x0426, 0x0a2e },					// 46 , 10,
	{ 0x0427, 0x0a30 },					// 48 , 10,
	{ 0x0428, 0x0a32 },					// 50 , 10,
	{ 0x0429, 0x0a34 },					// 52 , 10,
	{ 0x042A, 0x0a36 },					// 54 , 10,
	{ 0x042B, 0x0a38 },					// 56 , 10,
	{ 0x042C, 0x0a3a },					// 58 , 10,
	{ 0x042D, 0x0a3c },					// 60 , 10,
	{ 0x042E, 0x0a3e },					// 62 , 10,
	{ 0x042F, 0x0a40 },					// 64 , 10,
	{ 0x0430, 0x0a01 },					//  1 , 10,
	{ 0x0431, 0x0a03 },					//  3 , 10,
	{ 0x0432, 0x0a05 },					//  5 , 10,
	{ 0x0433, 0x0a07 },					//  7 , 10,
	{ 0x0434, 0x0a09 },					//  9 , 10,
	{ 0x0435, 0x0a0b },					// 11 , 10,
	{ 0x0436, 0x0a0f },					// 15 , 10,
	{ 0x0437, 0x0a11 },					// 17 , 10,
	{ 0x0438, 0x0a13 },					// 19 , 10,
	{ 0x0439, 0x0a15 },					// 21 , 10,
	{ 0x043A, 0x0a17 },					// 23 , 10,
	{ 0x043B, 0x0a19 },					// 25 , 10,
	{ 0x043C, 0x0a1b },					// 27 , 10,
	{ 0x043D, 0x0a1d },					// 29 , 10,
	{ 0x043E, 0x0a1f },					// 31 , 10,
	{ 0x043F, 0x0a21 },					// 33 , 10,
	{ 0x0440, 0x0a23 },					// 35 , 10,
	{ 0x0441, 0x0a25 },					// 37 , 10,
	{ 0x0442, 0x0a27 },					// 39 , 10,
	{ 0x0443, 0x0a29 },					// 41 , 10,
	{ 0x0444, 0x0a2b },					// 43 , 10,
	{ 0x0445, 0x0a2d },					// 45 , 10,
	{ 0x0446, 0x0a2f },					// 47 , 10,
	{ 0x0447, 0x0a31 },					// 49 , 10,
	{ 0x0448, 0x0a33 },					// 51 , 10,
	{ 0x0449, 0x0a35 },					// 53 , 10,
	{ 0x044A, 0x0a37 },					// 55 , 10,
	{ 0x044B, 0x0a39 },					// 57 , 10,
	{ 0x044C, 0x0a3b },					// 59 , 10,
	{ 0x044D, 0x0a3d },					// 61 , 10,
	{ 0x044E, 0x0a3f },					// 63 , 10,
	{ 0x044F, 0x0a41 },					// 65 , 10,
	{ 0x0451, 0x0a0d },					// 13 , 10,
	{ 0x0452, 0x0a4b },					// 75 , 10,
	{ 0x0453, 0x0a45 },					// 69 , 10,
	{ 0x0454, 0x0a4f },					// 79 , 10,
	{ 0x0455, 0x0a53 },					// 83 , 10,
	{ 0x0456, 0x0a59 },					// 89 , 10,
	{ 0x0457, 0x0a5b },					// 91 , 10,
	{ 0x0458, 0x0a5f },					// 95 , 10,
	{ 0x0459, 0x0a69 },					// 105, 10,
	{ 0x045A, 0x0a6d },					// 109, 10,
	{ 0x045B, 0x0a73 },					// 115, 10,
	{ 0x045C, 0x0a61 },					// 97 , 10,
	{ 0x045E, 0x0a75 },					// 117, 10,
	{ 0x045F, 0x0a87 },					// 135, 10,
	{ 0x0460, 0x0a70 },					// 112, 10,
	{ 0x0461, 0x0a71 },					// 113, 10,
	{ 0x0462, 0x0a8e },					// 142, 10,
	{ 0x0463, 0x0a8f },					// 143, 10,
	{ 0x0466, 0x0a90 },					// 144, 10,
	{ 0x0467, 0x0a91 },					// 145, 10,
	{ 0x046A, 0x0a92 },					// 146, 10,
	{ 0x046B, 0x0a93 },					// 147, 10,
	{ 0x046E, 0x0a94 },					// 148, 10,
	{ 0x046F, 0x0a95 },					// 149, 10,
	{ 0x0470, 0x0a96 },					// 150, 10,
	{ 0x0471, 0x0a97 },					// 151, 10,
	{ 0x0472, 0x0a98 },					// 152, 10,
	{ 0x0473, 0x0a99 },					// 153, 10,
	{ 0x0474, 0x0a9a },					// 154, 10,
	{ 0x0475, 0x0a9b },					// 155, 10,
	{ 0x047A, 0x0a6e },					// 110, 10,
	{ 0x047B, 0x0a6f },					// 111, 10,
	{ 0x047E, 0x0a84 },					// 132, 10,
	{ 0x047F, 0x0a85 },					// 133, 10,
	{ 0x0490, 0x0a46 },					// 70 , 10,
	{ 0x0491, 0x0a47 },					// 71 , 10,
	{ 0x0492, 0x0a48 },					// 72 , 10,
	{ 0x0493, 0x0a49 },					// 73 , 10,
	{ 0x0496, 0x0a50 },					// 80 , 10,
	{ 0x0497, 0x0a51 },					// 81 , 10,
	{ 0x049A, 0x0a62 },					// 98 , 10,
	{ 0x049B, 0x0a63 },					// 99 , 10,
	{ 0x049C, 0x0a66 },					// 102, 10,
	{ 0x049D, 0x0a67 },					// 103, 10,
	{ 0x04A2, 0x0a6a },					// 106, 10,
	{ 0x04A3, 0x0a6b },					// 107, 10,
	{ 0x04AE, 0x0a78 },					// 120, 10,
	{ 0x04AF, 0x0a79 },					// 121, 10,
	{ 0x04B0, 0x0a7a },					// 122, 10,
	{ 0x04B1, 0x0a7b },					// 123, 10,
	{ 0x04B2, 0x0a7e },					// 126, 10,
	{ 0x04B3, 0x0a7f },					// 127, 10,
	{ 0x04B6, 0x0a88 },					// 136, 10,
	{ 0x04B7, 0x0a89 },					// 137, 10,
	{ 0x04B8, 0x0a8a },					// 138, 10,
	{ 0x04B9, 0x0a8b },					// 139, 10,
	{ 0x04BA, 0x0a82 },					// 130, 10,
	{ 0x04BB, 0x0a83 },					// 131, 10,
	{ 0x04D8, 0x0a42 },					// 66 , 10,
	{ 0x04D9, 0x0a43 },					// 67 , 10,
	{ 0x04EE, 0x0a76 },					// 118, 10,
	{ 0x04EF, 0x0a77 },					// 119, 10,
	{ 0x05B0, 0x0920 },					// 32 ,  9,
	{ 0x05B1, 0x0921 },					// 33 ,  9,
	{ 0x05B2, 0x0922 },					// 34 ,  9,
	{ 0x05B3, 0x0923 },					// 35 ,  9,
	{ 0x05B4, 0x0924 },					// 36 ,  9,
	{ 0x05B5, 0x0925 },					// 37 ,  9,
	{ 0x05B6, 0x0926 },					// 38 ,  9,
	{ 0x05B7, 0x0927 },					// 39 ,  9,
	{ 0x05B8, 0x0928 },					// 40 ,  9,
	{ 0x05B9, 0x0929 },					// 41 ,  9,
	{ 0x05BB, 0x092b },					// 43 ,  9,
	{ 0x05BC, 0x092c },					// 44 ,  9,
	{ 0x05BD, 0x092d },					// 45 ,  9,
	{ 0x05BF, 0x092e },					// 46 ,  9,
	{ 0x05C0, 0x091c },					// 28 ,  9,
	{ 0x05C3, 0x091d },					// 29 ,  9,
	{ 0x05D0, 0x0900 },					//  0 ,  9,
	{ 0x05D1, 0x0901 },					//  1 ,  9,
	{ 0x05D2, 0x0902 },					//  2 ,  9,
	{ 0x05D3, 0x0903 },					//  3 ,  9,
	{ 0x05D4, 0x0904 },					//  4 ,  9,
	{ 0x05D5, 0x0905 },					//  5 ,  9,
	{ 0x05D6, 0x0906 },					//  6 ,  9,
	{ 0x05D7, 0x0907 },					//  7 ,  9,
	{ 0x05D8, 0x0908 },					//  8 ,  9,
	{ 0x05D9, 0x0909 },					//  9 ,  9,
	{ 0x05DA, 0x090a },					// 10 ,  9,
	{ 0x05DB, 0x090b },					// 11 ,  9,
	{ 0x05DC, 0x090c },					// 12 ,  9,
	{ 0x05DD, 0x090d },					// 13 ,  9,
	{ 0x05DE, 0x090e },					// 14 ,  9,
	{ 0x05DF, 0x090f },					// 15 ,  9,
	{ 0x05E0, 0x0910 },					// 16 ,  9,
	{ 0x05E1, 0x0911 },					// 17 ,  9,
	{ 0x05E2, 0x0912 },					// 18 ,  9,
	{ 0x05E3, 0x0913 },					// 19 ,  9,
	{ 0x05E4, 0x0914 },					// 20 ,  9,
	{ 0x05E5, 0x0915 },					// 21 ,  9,
	{ 0x05E6, 0x0916 },					// 22 ,  9,
	{ 0x05E7, 0x0917 },					// 23 ,  9,
	{ 0x05E8, 0x0918 },					// 24 ,  9,
	{ 0x05E9, 0x0919 },					// 25 ,  9,
	{ 0x05EA, 0x091a },					// 26 ,  9,
	{ 0x05F0, 0x0931 },					// 49 ,  9,
	{ 0x05F1, 0x0932 },					// 50 ,  9,
	{ 0x05F2, 0x0933 },					// 51 ,  9,
	{ 0x05F3, 0x091e },					// 30 ,  9,
	{ 0x05F4, 0x091f },					// 31 ,  9,
	{ 0x060C, 0x0d26 },					// 38 , 13,
	{ 0x061B, 0x0d27 },					// 39 , 13,
	{ 0x061F, 0x0d28 },					// 40 , 13,
	{ 0x0621, 0x0da4 },					// 164, 13,
	{ 0x0622, 0x0db1 },					// 177, 13,
	{ 0x0623, 0x0da5 },					// 165, 13,
	{ 0x0624, 0x0da9 },					// 169, 13,
	{ 0x0625, 0x0da7 },					// 167, 13,
	{ 0x0626, 0x0dab },					// 171, 13,
	{ 0x0627, 0x0d3a },					// 58 , 13,
	{ 0x0628, 0x0d3c },					// 60 , 13,
	{ 0x0629, 0x0d98 },					// 152, 13,
	{ 0x062A, 0x0d40 },					// 64 , 13,
	{ 0x062B, 0x0d44 },					// 68 , 13,
	{ 0x062C, 0x0d48 },					// 72 , 13,
	{ 0x062D, 0x0d4c },					// 76 , 13,
	{ 0x062E, 0x0d50 },					// 80 , 13,
	{ 0x062F, 0x0d54 },					// 84 , 13,
	{ 0x0630, 0x0d56 },					// 86 , 13,
	{ 0x0631, 0x0d58 },					// 88 , 13,
	{ 0x0632, 0x0d5a },					// 90 , 13,
	{ 0x0633, 0x0d5c },					// 92 , 13,
	{ 0x0634, 0x0d60 },					// 96 , 13,
	{ 0x0635, 0x0d64 },					// 100, 13,
	{ 0x0636, 0x0d68 },					// 104, 13,
	{ 0x0637, 0x0d6c },					// 108, 13,
	{ 0x0638, 0x0d70 },					// 112, 13,
	{ 0x0639, 0x0d74 },					// 116, 13,
	{ 0x063A, 0x0d78 },					// 120, 13,
	{ 0x0640, 0x0dc2 },					// 194, 13,
	{ 0x0641, 0x0d7c },					// 124, 13,
	{ 0x0642, 0x0d80 },					// 128, 13,
	{ 0x0643, 0x0d84 },					// 132, 13,
	{ 0x0644, 0x0d88 },					// 136, 13,
	{ 0x0645, 0x0d8c },					// 140, 13,
	{ 0x0646, 0x0d90 },					// 144, 13,
	{ 0x0647, 0x0d94 },					// 148, 13,
	{ 0x0648, 0x0d9a },					// 154, 13,
	{ 0x0649, 0x0da0 },					// 160, 13,
	{ 0x064A, 0x0d9c },					// 156, 13,
	{ 0x064B, 0x0d10 },					// 16 , 13,
	{ 0x064C, 0x0d11 },					// 17 , 13,
	{ 0x064E, 0x0d0a },					// 10 , 13,
	{ 0x064F, 0x0d0c },					// 12 , 13,
	{ 0x0650, 0x0d0e },					// 14 , 13,
	{ 0x0651, 0x0d16 },					// 22 , 13,
	{ 0x0652, 0x0d14 },					// 20 , 13,
	{ 0x0660, 0x0d38 },					// 56 , 13,
	{ 0x0661, 0x0d2f },					// 47 , 13,
	{ 0x0662, 0x0d30 },					// 48 , 13,
	{ 0x0663, 0x0d31 },					// 49 , 13,
	{ 0x0664, 0x0d32 },					// 50 , 13,
	{ 0x0665, 0x0d33 },					// 51 , 13,
	{ 0x0666, 0x0d34 },					// 52 , 13,
	{ 0x0667, 0x0d35 },					// 53 , 13,
	{ 0x0668, 0x0d36 },					// 54 , 13,
	{ 0x0669, 0x0d37 },					// 55 , 13,
	{ 0x066A, 0x0d2a },					// 42 , 13,
	{ 0x0671, 0x0db3 },					// 179, 13,
	{ 0x0674, 0x0d24 },					// 36 , 13,
	{ 0x0679, 0x0e3c },					// 60 , 14,
	{ 0x067A, 0x0e4c },					// 76 , 14,
	{ 0x067B, 0x0e30 },					// 48 , 14,
	{ 0x067C, 0x0e40 },					// 64 , 14,
	{ 0x067D, 0x0e48 },					// 72 , 14,
	{ 0x067E, 0x0e38 },					// 56 , 14,
	{ 0x067F, 0x0e44 },					// 68 , 14,
	{ 0x0680, 0x0e34 },					// 52 , 14,
	{ 0x0681, 0x0e64 },					// 100, 14,
	{ 0x0683, 0x0e54 },					// 84 , 14,
	{ 0x0684, 0x0e50 },					// 80 , 14,
	{ 0x0685, 0x0e60 },					// 96 , 14,
	{ 0x0686, 0x0e58 },					// 88 , 14,
	{ 0x0687, 0x0e5c },					// 92 , 14,
	{ 0x0688, 0x0e68 },					// 104, 14,
	{ 0x0689, 0x0e6a },					// 106, 14,
	{ 0x068A, 0x0e70 },					// 112, 14,
	{ 0x068C, 0x0e6c },					// 108, 14,
	{ 0x068D, 0x0e72 },					// 114, 14,
	{ 0x068E, 0x0e6e },					// 110, 14,
	{ 0x0691, 0x0e76 },					// 118, 14,
	{ 0x0692, 0x0e7C },					// 124, 14,
	{ 0x0693, 0x0e74 },					// 116, 14,
	{ 0x0695, 0x0e7a },					// 122, 14,
	{ 0x0696, 0x0e80 },					// 128, 14,
	{ 0x0698, 0x0e7e },					// 126, 14,
	{ 0x0699, 0x0e78 },					// 120, 14,
	{ 0x069A, 0x0e84 },					// 132, 14,
	{ 0x06A0, 0x0e88 },					// 136, 14,
	{ 0x06A4, 0x0e8c },					// 140, 14,
	{ 0x06A6, 0x0e90 },					// 144, 14,
	{ 0x06A9, 0x0e94 },					// 148, 14,
	{ 0x06AA, 0x0e9c },					// 156, 14,
	{ 0x06AB, 0x0ea8 },					// 168, 14,
	{ 0x06AF, 0x0ea0 },					// 160, 14,
	{ 0x06B1, 0x0eac },					// 172, 14,
	{ 0x06B3, 0x0eb0 },					// 176, 14,
	{ 0x06B5, 0x0eb4 },					// 180, 14,
	{ 0x06BA, 0x0eba },					// 186, 14,
	{ 0x06BB, 0x0ec2 },					// 194, 14,
	{ 0x06BC, 0x0ebe },					// 190, 14,
	{ 0x06C0, 0x0eda },					// 218, 14,
	{ 0x06C6, 0x0ec6 },					// 198, 14,
	{ 0x06CA, 0x0ec8 },					// 200, 14,
	{ 0x06CE, 0x0ed0 },					// 208, 14,
	{ 0x06D1, 0x0ed6 },					// 214, 14,
	{ 0x06D2, 0x0ed4 },					// 212, 14,
	{ 0x06D6, 0x0d25 },					// 37 , 13,
	{ 0x06E4, 0x0d22 },					// 34 , 13,
	{ 0x06F4, 0x0e29 },					// 41 , 14,
	{ 0x06F5, 0x0e2b },					// 43 , 14,
	{ 0x06F6, 0x0e2c },					// 44 , 14,
	{ 0x06F7, 0x0e2e },					// 46 , 14,
	{ 0x06F8, 0x0e2f },					// 47 , 14,
	{ 0x10D0, 0x0ad2 },					// 210, 10,
	{ 0x10D1, 0x0ad3 },					// 211, 10,
	{ 0x10D2, 0x0ad4 },					// 212, 10,
	{ 0x10D3, 0x0ad5 },					// 213, 10,
	{ 0x10D4, 0x0ad6 },					// 214, 10,
	{ 0x10D5, 0x0ad7 },					// 215, 10,
	{ 0x10D6, 0x0ad8 },					// 216, 10,
	{ 0x10D7, 0x0ada },					// 218, 10,
	{ 0x10D8, 0x0adb },					// 219, 10,
	{ 0x10D9, 0x0adc },					// 220, 10,
	{ 0x10DA, 0x0add },					// 221, 10,
	{ 0x10DB, 0x0ade },					// 222, 10,
	{ 0x10DC, 0x0adf },					// 223, 10,
	{ 0x10DD, 0x0ae1 },					// 225, 10,
	{ 0x10DE, 0x0ae2 },					// 226, 10,
	{ 0x10DF, 0x0ae3 },					// 227, 10,
	{ 0x10E0, 0x0ae4 },					// 228, 10,
	{ 0x10E1, 0x0ae5 },					// 229, 10,
	{ 0x10E2, 0x0ae6 },					// 230, 10,
	{ 0x10E3, 0x0ae7 },					// 231, 10,
	{ 0x10E4, 0x0ae9 },					// 233, 10,
	{ 0x10E5, 0x0aea },					// 234, 10,
	{ 0x10E6, 0x0aeb },					// 235, 10,
	{ 0x10E7, 0x0aec },					// 236, 10,
	{ 0x10E8, 0x0aed },					// 237, 10,
	{ 0x10E9, 0x0aee },					// 238, 10,
	{ 0x10EA, 0x0aef },					// 239, 10,
	{ 0x10EB, 0x0af0 },					// 240, 10,
	{ 0x10EC, 0x0af1 },					// 241, 10,
	{ 0x10ED, 0x0af2 },					// 242, 10,
	{ 0x10EE, 0x0af3 },					// 243, 10,
	{ 0x10EF, 0x0af5 },					// 245, 10,
	{ 0x10F0, 0x0af6 },					// 246, 10,
	{ 0x10F1, 0x0ad9 },					// 217, 10,
	{ 0x10F2, 0x0ae0 },					// 224, 10,
	{ 0x10F3, 0x0ae8 },					// 232, 10,
	{ 0x10F4, 0x0af4 },					// 244, 10,
	{ 0x10F5, 0x0af7 },					// 247, 10,
	{ 0x10F6, 0x0af8 },					// 248, 10,
	{ 0x1F00, 0x0873 },					// 115,  8,
	{ 0x1F01, 0x087b },					// 123,  8,
	{ 0x1F02, 0x0875 },					// 117,  8,
	{ 0x1F03, 0x087d },					// 125,  8,
	{ 0x1F04, 0x0874 },					// 116,  8,
	{ 0x1F05, 0x087c },					// 124,  8,
	{ 0x1F10, 0x0884 },					// 132,  8,
	{ 0x1F11, 0x0887 },					// 135,  8,
	{ 0x1F12, 0x0886 },					// 134,  8,
	{ 0x1F13, 0x0889 },					// 137,  8,
	{ 0x1F14, 0x0885 },					// 133,  8,
	{ 0x1F15, 0x0888 },					// 136,  8,
	{ 0x1F20, 0x0890 },					// 144,  8,
	{ 0x1F21, 0x0898 },					// 152,  8,
	{ 0x1F22, 0x0892 },					// 146,  8,
	{ 0x1F23, 0x089a },					// 154,  8,
	{ 0x1F24, 0x0891 },					// 145,  8,
	{ 0x1F25, 0x0899 },					// 153,  8,
	{ 0x1F30, 0x08a4 },					// 164,  8,
	{ 0x1F31, 0x08a8 },					// 168,  8,
	{ 0x1F32, 0x08a6 },					// 166,  8,
	{ 0x1F33, 0x08aa },					// 170,  8,
	{ 0x1F34, 0x08a5 },					// 165,  8,
	{ 0x1F35, 0x08a9 },					// 169,  8,
	{ 0x1F40, 0x08ad },					// 173,  8,
	{ 0x1F41, 0x08b0 },					// 176,  8,
	{ 0x1F42, 0x08af },					// 175,  8,
	{ 0x1F43, 0x08b2 },					// 178,  8,
	{ 0x1F44, 0x08ae },					// 174,  8,
	{ 0x1F45, 0x08b1 },					// 177,  8,
	{ 0x1F50, 0x08b9 },					// 185,  8,
	{ 0x1F51, 0x08bd },					// 189,  8,
	{ 0x1F52, 0x08bb },					// 187,  8,
	{ 0x1F53, 0x08bf },					// 191,  8,
	{ 0x1F54, 0x08ba },					// 186,  8,
	{ 0x1F55, 0x08be },					// 190,  8,
	{ 0x1F60, 0x08c7 },					// 199,  8,
	{ 0x1F61, 0x08cf },					// 207,  8,
	{ 0x1F62, 0x08c9 },					// 201,  8,
	{ 0x1F63, 0x08d1 },					// 209,  8,
	{ 0x1F64, 0x08c8 },					// 200,  8,
	{ 0x1F65, 0x08d0 },					// 208,  8,
	{ 0x1F70, 0x086d },					// 109,  8,
	{ 0x1F72, 0x0883 },					// 131,  8,
	{ 0x1F74, 0x088a },					// 138,  8,
	{ 0x1F76, 0x08a0 },					// 160,  8,
	{ 0x1F78, 0x08ac },					// 172,  8,
	{ 0x1F7A, 0x08b5 },					// 181,  8,
	{ 0x1F7C, 0x08c1 },					// 193,  8,
	{ 0x1F80, 0x0877 },					// 119,  8,
	{ 0x1F81, 0x087f },					// 127,  8,
	{ 0x1F82, 0x0879 },					// 121,  8,
	{ 0x1F83, 0x0881 },					// 129,  8,
	{ 0x1F84, 0x0878 },					// 120,  8,
	{ 0x1F85, 0x0880 },					// 128,  8,
	{ 0x1F90, 0x0894 },					// 148,  8,
	{ 0x1F91, 0x089c },					// 156,  8,
	{ 0x1F92, 0x0896 },					// 150,  8,
	{ 0x1F93, 0x089e },					// 158,  8,
	{ 0x1F94, 0x0895 },					// 149,  8,
	{ 0x1F95, 0x089d },					// 157,  8,
	{ 0x1FA0, 0x08cb },					// 203,  8,
	{ 0x1FA1, 0x08d3 },					// 211,  8,
	{ 0x1FA2, 0x08cd },					// 205,  8,
	{ 0x1FA3, 0x08d5 },					// 213,  8,
	{ 0x1FA4, 0x08cc },					// 204,  8,
	{ 0x1FA5, 0x08d4 },					// 212,  8,
	{ 0x1FB2, 0x0871 },					// 113,  8,
	{ 0x1FB3, 0x086f },					// 111,  8,
	{ 0x1FB4, 0x0870 },					// 112,  8,
	{ 0x1FC2, 0x088e },					// 142,  8,
	{ 0x1FC3, 0x088c },					// 140,  8,
	{ 0x1FC4, 0x088d },					// 141,  8,
	{ 0x1FCD, 0x085e },					// 94 ,  8,
	{ 0x1FCE, 0x085c },					// 92 ,  8,
	{ 0x1FDD, 0x085f },					// 95 ,  8,
	{ 0x1FDE, 0x085d },					// 93 ,  8,
	{ 0x1FE4, 0x08B4 },					// 180,  8,
	{ 0x1FE5, 0x08B3 },					// 179,  8,
	{ 0x1FF2, 0x08c5 },					// 197,  8,
	{ 0x1FF3, 0x08c3 },					// 195,  8,
	{ 0x1FF4, 0x08c4 },					// 196,  8,
	{ 0x2007, 0x0517 },					// 23 ,  5,
	{ 0x2012, 0x0432 },					// 50 ,  4,
	{ 0x2013, 0x0421 },					// 33 ,  4,
	{ 0x2014, 0x0422 },					// 34 ,  4,
	{ 0x2017, 0x022f },					// 47 ,  2,
	{ 0x2018, 0x041d },					// 29 ,  4,
	{ 0x2019, 0x041c },					// 28 ,  4,
	{ 0x201A, 0x043e },					// 62 ,  4,
	{ 0x201B, 0x041b },					// 27 ,  4,
	{ 0x201C, 0x0420 },					// 32 ,  4,
	{ 0x201D, 0x041f },					// 31 ,  4,
	{ 0x201E, 0x043f },					// 63 ,  4,
	{ 0x201F, 0x041e },					// 30 ,  4,
	{ 0x2020, 0x0427 },					// 39 ,  4,
	{ 0x2021, 0x0428 },					// 40 ,  4,
	{ 0x2022, 0x0403 },					//  3 ,  4,
	{ 0x2026, 0x0438 },					// 56 ,  4,
	{ 0x2030, 0x044b },					// 75 ,  4,
	{ 0x2033, 0x0580 },					// 128,  5,
	{ 0x2034, 0x0671 },					// 113,  6,
	{ 0x2036, 0x057f },					// 127,  5,
	{ 0x2039, 0x0423 },					// 35 ,  4,
	{ 0x203A, 0x0424 },					// 36 ,  4,
	{ 0x203C, 0x050d },					// 13 ,  5,
	{ 0x203E, 0x0626 },					// 38 ,  6,
	{ 0x207F, 0x0415 },					// 21 ,  4,
	{ 0x20A0, 0x043c },					// 60 ,  4,
	{ 0x20A2, 0x043b },					// 59 ,  4,
	{ 0x20A3, 0x043a },					// 58 ,  4,
	{ 0x20A4, 0x043d },					// 61 ,  4,
	{ 0x20A6, 0x0457 },					// 87 ,  4,
	{ 0x20A7, 0x040d },					// 13 ,  4,
	{ 0x20A8, 0x0458 },					// 88 ,  4,
	{ 0x20A9, 0x0456 },					// 86 ,  4,
	{ 0x20AA, 0x097A },					// 122,  9,
	{ 0x20AC, 0x0466 },	   			// 102,  4,
	{ 0x20DD, 0x066d },					// 109,  6,
	{ 0x20E1, 0x06e1 },					// 225,  6,
	{ 0x2102, 0x06d5 },					// 213,  6,
	{ 0x2104, 0x0515 },					// 21 ,  5,
	{ 0x2105, 0x0449 },					// 73 ,  4,
	{ 0x2106, 0x044a },					// 74 ,  4,
	{ 0x210C, 0x06e9 },					// 233,  6,
	{ 0x210F, 0x0632 },					// 50 ,  6,
	{ 0x2111, 0x0633 },					// 51 ,  6,
	{ 0x2112, 0x0669 },					// 105,  6,
	{ 0x2113, 0x0631 },					// 49 ,  6,
	{ 0x2115, 0x06d7 },					// 215,  6,
	{ 0x2116, 0x044c },					// 76 ,  4,
	{ 0x2118, 0x0635 },					// 53 ,  6,
	{ 0x211C, 0x0634 },					// 52 ,  6,
	{ 0x211D, 0x06d8 },					// 216,  6,
	{ 0x211E, 0x042b },					// 43 ,  4,
	{ 0x2120, 0x042a },					// 42 ,  4,
	{ 0x2122, 0x0429 },					// 41 ,  4,
	{ 0x2127, 0x06a7 },					// 167,  6,
	{ 0x2128, 0x066b },					// 107,  6,
	{ 0x212B, 0x0623 },					// 35 ,  6,
	{ 0x212D, 0x066a },					// 106,  6,
	{ 0x212F, 0x0630 },					// 48 ,  6,
	{ 0x2130, 0x06d3 },					// 211,  6,
	{ 0x2131, 0x06d4 },					// 212,  6,
	{ 0x2153, 0x0440 },					// 64 ,  4,
	{ 0x2154, 0x0441 },					// 65 ,  4,
	{ 0x215B, 0x0442 },					// 66 ,  4,
	{ 0x215C, 0x0443 },					// 67 ,  4,
	{ 0x215D, 0x0444 },					// 68 ,  4,
	{ 0x215E, 0x0445 },					// 69 ,  4,
	{ 0x2190, 0x0590 },					// 144,  5,
	{ 0x2191, 0x0617 },					// 23 ,  6,
	{ 0x2192, 0x05d5 },					// 213,  5,
	{ 0x2193, 0x0618 },					// 24 ,  6,
	{ 0x2194, 0x05d6 },					// 214,  5,
	{ 0x2195, 0x05d7 },					// 215,  5,
	{ 0x2196, 0x0640 },					// 64 ,  6,
	{ 0x2197, 0x063e },					// 62 ,  6,
	{ 0x2198, 0x063f },					// 63 ,  6,
	{ 0x2199, 0x0641 },					// 65 ,  6,
	{ 0x219D, 0x0690 },					// 144,  6,
	{ 0x21A3, 0x0693 },					// 147,  6,
	{ 0x21A8, 0x050f },					// 15 ,  5,
	{ 0x21A9, 0x0691 },					// 145,  6,
	{ 0x21AA, 0x0692 },					// 146,  6,
	{ 0x21B5, 0x0514 },					// 20 ,  5,
	{ 0x21BC, 0x0694 },					// 148,  6,
	{ 0x21BD, 0x0695 },					// 149,  6,
	{ 0x21BE, 0x069b },					// 155,  6,
	{ 0x21BF, 0x069a },					// 154,  6,
	{ 0x21C0, 0x0696 },					// 150,  6,
	{ 0x21C1, 0x0697 },					// 151,  6,
	{ 0x21C2, 0x069d },					// 157,  6,
	{ 0x21C3, 0x069c },					// 156,  6,
	{ 0x21C4, 0x0636 },					// 54 ,  6,
	{ 0x21C6, 0x0637 },					// 55 ,  6,
	{ 0x21C7, 0x069f },					// 159,  6,
	{ 0x21C9, 0x069e },					// 158,  6,
	{ 0x21CB, 0x0699 },					// 153,  6,
	{ 0x21CC, 0x0698 },					// 152,  6,
	{ 0x21D0, 0x0639 },					// 57 ,  6,
	{ 0x21D1, 0x063a },					// 58 ,  6,
	{ 0x21D2, 0x0638 },					// 56 ,  6,
	{ 0x21D3, 0x063b },					// 59 ,  6,
	{ 0x21D4, 0x063c },					// 60 ,  6,
	{ 0x21D5, 0x063d },					// 61 ,  6,
	{ 0x21E6, 0x0597 },					// 151,  5,
	{ 0x21E8, 0x0596 },					// 150,  5,
	{ 0x2200, 0x067a },					// 122,  6,
	{ 0x2202, 0x062c },					// 44 ,  6,
	{ 0x2203, 0x0679 },					// 121,  6,
	{ 0x2204, 0x06d0 },					// 208,  6,
	{ 0x2205, 0x0648 },					// 72 ,  6,
	{ 0x2207, 0x062b },					// 43 ,  6,
	{ 0x2208, 0x060f },					// 15 ,  6,
	{ 0x2209, 0x06d1 },					// 209,  6,
	{ 0x220B, 0x06db },					// 219,  6,
	{ 0x220D, 0x0647 },					// 71 ,  6,
	{ 0x220F, 0x0629 },					// 41 ,  6,
	{ 0x2210, 0x0672 },					// 114,  6,
	{ 0x2211, 0x0612 },					// 18 ,  6,
	{ 0x2212, 0x0600 },					//  0 ,  6,
	{ 0x2213, 0x062a },					// 42 ,  6,
	{ 0x2214, 0x06ae },					// 174,  6,
	{ 0x2215, 0x0606 },					//  6 ,  6,
	{ 0x2216, 0x0607 },					//  7 ,  6,
	{ 0x2218, 0x0621 },					// 33 ,  6,
	{ 0x2219, 0x0622 },					// 34 ,  6,
	{ 0x221A, 0x0704 },					//  4 ,  7,
	{ 0x221D, 0x0604 },					//  4 ,  6,
	{ 0x221E, 0x0613 },					// 19 ,  6,
	{ 0x221F, 0x06da },					// 218,  6,
	{ 0x2220, 0x064f },					// 79 ,  6,
	{ 0x2221, 0x06a8 },					// 168,  6,
	{ 0x2222, 0x06a9 },					// 169,  6,
	{ 0x2223, 0x0609 },					//  9 ,  6,
	{ 0x2224, 0x06ce },					// 206,  6,
	{ 0x2225, 0x0611 },					// 17 ,  6,
	{ 0x2226, 0x06cd },					// 205,  6,
	{ 0x2227, 0x0655 },					// 85 ,  6,
	{ 0x2228, 0x0656 },					// 86 ,  6,
	{ 0x2229, 0x0610 },					// 16 ,  6,
	{ 0x222A, 0x0642 },					// 66 ,  6,
	{ 0x222B, 0x0628 },					// 40 ,  6,
	{ 0x222E, 0x0668 },					// 104,  6,
	{ 0x2234, 0x0666 },					// 102,  6,
	{ 0x2235, 0x0665 },					// 101,  6,
	{ 0x2237, 0x0667 },					// 103,  6,
	{ 0x223C, 0x060c },					// 12 ,  6,
	{ 0x2241, 0x06bd },					// 189,  6,
	{ 0x2243, 0x0673 },					// 115,  6,
	{ 0x2244, 0x06be },					// 190,  6,
	{ 0x2245, 0x0674 },					// 116,  6,
	{ 0x2247, 0x06bf },					// 191,  6,
	{ 0x2248, 0x060d },					// 13 ,  6,
	{ 0x2249, 0x06c0 },					// 192,  6,
	{ 0x224D, 0x06b3 },					// 179,  6,
	{ 0x224E, 0x06b2 },					// 178,  6,
	{ 0x2250, 0x06af },					// 175,  6,
	{ 0x2252, 0x06b0 },					// 176,  6,
	{ 0x2253, 0x06b1 },					// 177,  6,
	{ 0x225F, 0x06d9 },					// 217,  6,
	{ 0x2260, 0x0663 },					// 99 ,  6,
	{ 0x2261, 0x060e },					// 14 ,  6,
	{ 0x2262, 0x0664 },					// 100,  6,
	{ 0x2264, 0x0602 },					//  2 ,  6,
	{ 0x2265, 0x0603 },					//  3 ,  6,
	{ 0x226A, 0x064d },					// 77 ,  6,
	{ 0x226B, 0x064e },					// 78 ,  6,
	{ 0x226C, 0x06b6 },					// 182,  6,
	{ 0x226D, 0x06cf },					// 207,  6,
	{ 0x226E, 0x06b9 },					// 185,  6,
	{ 0x226F, 0x06bb },					// 187,  6,
	{ 0x2270, 0x06ba },					// 186,  6,
	{ 0x2271, 0x06bc },					// 188,  6,
	{ 0x2272, 0x06eb },					// 235,  6,
	{ 0x2273, 0x06ec },					// 236,  6,
	{ 0x227A, 0x0675 },					// 117,  6,
	{ 0x227B, 0x0677 },					// 119,  6,
	{ 0x227C, 0x0676 },					// 118,  6,
	{ 0x227D, 0x0678 },					// 120,  6,
	{ 0x2280, 0x06c1 },					// 193,  6,
	{ 0x2281, 0x06c3 },					// 195,  6,
	{ 0x2282, 0x0643 },					// 67 ,  6,
	{ 0x2283, 0x0644 },					// 68 ,  6,
	{ 0x2284, 0x06c5 },					// 197,  6,
	{ 0x2285, 0x06c6 },					// 198,  6,
	{ 0x2286, 0x0645 },					// 69 ,  6,
	{ 0x2287, 0x0646 },					// 70 ,  6,
	{ 0x2288, 0x06c7 },					// 199,  6,
	{ 0x2289, 0x06c8 },					// 200,  6,
	{ 0x228A, 0x067e },					// 126,  6,
	{ 0x228B, 0x067f },					// 127,  6,
	{ 0x228E, 0x067d },					// 125,  6,
	{ 0x228F, 0x0682 },					// 130,  6,
	{ 0x2290, 0x0685 },					// 133,  6,
	{ 0x2291, 0x0683 },					// 131,  6,
	{ 0x2292, 0x0686 },					// 134,  6,
	{ 0x2293, 0x0680 },					// 128,  6,
	{ 0x2294, 0x0681 },					// 129,  6,
	{ 0x2295, 0x0651 },					// 81 ,  6,
	{ 0x2296, 0x0652 },					// 82 ,  6,
	{ 0x2297, 0x0650 },					// 80 ,  6,
	{ 0x2299, 0x0654 },					// 84 ,  6,
	{ 0x229A, 0x06a4 },					// 164,  6,
	{ 0x229B, 0x06a5 },					// 165,  6,
	{ 0x229D, 0x06a6 },					// 166,  6,
	{ 0x22A2, 0x065b },					// 91 ,  6,
	{ 0x22A3, 0x065c },					// 92 ,  6,
	{ 0x22A4, 0x0658 },					// 88 ,  6,
	{ 0x22A5, 0x0659 },					// 89 ,  6,
	{ 0x22A8, 0x06b4 },					// 180,  6,
	{ 0x22BB, 0x0657 },					// 87 ,  6,
	{ 0x22C5, 0x061f },					// 31 ,  6,
	{ 0x22C6, 0x0670 },					// 112,  6,
	{ 0x22C8, 0x068c },					// 140,  6,
	{ 0x22D0, 0x06a2 },					// 162,  6,
	{ 0x22D1, 0x06a3 },					// 163,  6,
	{ 0x22D2, 0x06a1 },					// 161,  6,
	{ 0x22D3, 0x06a0 },					// 160,  6,
	{ 0x22D8, 0x067b },					// 123,  6,
	{ 0x22D9, 0x067c },					// 124,  6,
	{ 0x22E0, 0x06c2 },					// 194,  6,
	{ 0x22E1, 0x06c4 },					// 196,  6,
	{ 0x22E2, 0x06cb },					// 203,  6,
	{ 0x22E3, 0x06cc },					// 204,  6,
	{ 0x22E4, 0x0684 },					// 132,  6,
	{ 0x22E5, 0x0687 },					// 135,  6,
	{ 0x22EE, 0x06de },					// 222,  6,
	{ 0x22EF, 0x06dc },					// 220,  6,
	{ 0x22F1, 0x06df },					// 223,  6,
	{ 0x2302, 0x050c },					// 12 ,  5,
	{ 0x2308, 0x0649 },					// 73 ,  6,
	{ 0x2309, 0x064a },					// 74 ,  6,
	{ 0x230A, 0x064b },					// 75 ,  6,
	{ 0x230B, 0x064c },					// 76 ,  6,
	{ 0x2310, 0x0510 },					// 16 ,  5,
	{ 0x2312, 0x065a },					// 90 ,  6,
	{ 0x2319, 0x0511 },					// 17 ,  5,
	{ 0x231A, 0x051f },					// 31 ,  5,
	{ 0x231B, 0x0520 },					// 32 ,  5,
	{ 0x2320, 0x0700 },					//  0 ,  7,
	{ 0x2321, 0x0701 },					//  1 ,  7,
	{ 0x2322, 0x068e },					// 142,  6,
	{ 0x2323, 0x068d },					// 141,  6,
	{ 0x2329, 0x060a },					// 10 ,  6,
	{ 0x232A, 0x060b },					// 11 ,  6,
	{ 0x2409, 0x044f },					// 79 ,  4,
	{ 0x240A, 0x0452 },					// 82 ,  4,
	{ 0x240B, 0x0454 },					// 84 ,  4,
	{ 0x240C, 0x0450 },					// 80 ,  4,
	{ 0x240D, 0x0451 },					// 81 ,  4,
	{ 0x2424, 0x0453 },					// 83 ,  4,
	{ 0x24C2, 0x0446 },					// 70 ,  4,
	{ 0x24C5, 0x0447 },					// 71 ,  4,
	{ 0x24CA, 0x0448 },					// 72 ,  4,
	{ 0x2500, 0x0308 },					//  8 ,  3,
	{ 0x2502, 0x0309 },					//  9 ,  3,
	{ 0x250C, 0x030a },					// 10 ,  3,
	{ 0x2510, 0x030b },					// 11 ,  3,
	{ 0x2514, 0x030d },					// 13 ,  3,
	{ 0x2518, 0x030c },					// 12 ,  3,
	{ 0x251C, 0x030e },					// 14 ,  3,
	{ 0x251E, 0x033e },					// 62 ,  3,
	{ 0x251F, 0x033c },					// 60 ,  3,
	{ 0x2521, 0x033f },					// 63 ,  3,
	{ 0x2522, 0x033d },					// 61 ,  3,
	{ 0x2524, 0x0310 },					// 16 ,  3,
	{ 0x2526, 0x0345 },					// 69 ,  3,
	{ 0x2527, 0x0344 },					// 68 ,  3,
	{ 0x2529, 0x0347 },					// 71 ,  3,
	{ 0x252A, 0x0346 },					// 70 ,  3,
	{ 0x252C, 0x030f },					// 15 ,  3,
	{ 0x252D, 0x0342 },					// 66 ,  3,
	{ 0x252E, 0x0340 },					// 64 ,  3,
	{ 0x2531, 0x0343 },					// 67 ,  3,
	{ 0x2532, 0x0341 },					// 65 ,  3,
	{ 0x2534, 0x0311 },					// 17 ,  3,
	{ 0x2535, 0x034a },					// 74 ,  3,
	{ 0x2536, 0x0348 },					// 72 ,  3,
	{ 0x2539, 0x034b },					// 75 ,  3,
	{ 0x253A, 0x0349 },					// 73 ,  3,
	{ 0x253C, 0x0312 },					// 18 ,  3,
	{ 0x253D, 0x0352 },					// 82 ,  3,
	{ 0x253E, 0x034e },					// 78 ,  3,
	{ 0x2540, 0x034f },					// 79 ,  3,
	{ 0x2541, 0x034c },					// 76 ,  3,
	{ 0x2543, 0x0355 },					// 85 ,  3,
	{ 0x2544, 0x0350 },					// 80 ,  3,
	{ 0x2545, 0x0353 },					// 83 ,  3,
	{ 0x2546, 0x034d },					// 77 ,  3,
	{ 0x2547, 0x0357 },					// 87 ,  3,
	{ 0x2548, 0x0354 },					// 84 ,  3,
	{ 0x2549, 0x0356 },					// 86 ,  3,
	{ 0x254A, 0x0351 },					// 81 ,  3,
	{ 0x2550, 0x0313 },					// 19 ,  3,
	{ 0x2551, 0x0314 },					// 20 ,  3,
	{ 0x2552, 0x031e },					// 30 ,  3,
	{ 0x2553, 0x0322 },					// 34 ,  3,
	{ 0x2554, 0x0315 },					// 21 ,  3,
	{ 0x2555, 0x031f },					// 31 ,  3,
	{ 0x2556, 0x0323 },					// 35 ,  3,
	{ 0x2557, 0x0316 },					// 22 ,  3,
	{ 0x2558, 0x0321 },					// 33 ,  3,
	{ 0x2559, 0x0325 },					// 37 ,  3,
	{ 0x255A, 0x0318 },					// 24 ,  3,
	{ 0x255B, 0x0320 },					// 32 ,  3,
	{ 0x255C, 0x0324 },					// 36 ,  3,
	{ 0x255D, 0x0317 },					// 23 ,  3,
	{ 0x255E, 0x0326 },					// 38 ,  3,
	{ 0x255F, 0x032a },					// 42 ,  3,
	{ 0x2560, 0x0319 },					// 25 ,  3,
	{ 0x2561, 0x0328 },					// 40 ,  3,
	{ 0x2562, 0x032c },					// 44 ,  3,
	{ 0x2563, 0x031b },					// 27 ,  3,
	{ 0x2564, 0x032b },					// 43 ,  3,
	{ 0x2565, 0x0327 },					// 39 ,  3,
	{ 0x2566, 0x031a },					// 26 ,  3,
	{ 0x2567, 0x032d },					// 45 ,  3,
	{ 0x2568, 0x0329 },					// 41 ,  3,
	{ 0x2569, 0x031c },					// 28 ,  3,
	{ 0x256A, 0x032f },					// 47 ,  3,
	{ 0x256B, 0x032e },					// 46 ,  3,
	{ 0x256C, 0x031d },					// 29 ,  3,
	{ 0x2574, 0x0330 },					// 48 ,  3,
	{ 0x2575, 0x0331 },					// 49 ,  3,
	{ 0x2576, 0x0332 },					// 50 ,  3,
	{ 0x2577, 0x0333 },					// 51 ,  3,
	{ 0x2578, 0x0334 },					// 52 ,  3,
	{ 0x2579, 0x0335 },					// 53 ,  3,
	{ 0x257A, 0x0336 },					// 54 ,  3,
	{ 0x257B, 0x0337 },					// 55 ,  3,
	{ 0x257C, 0x0338 },					// 56 ,  3,
	{ 0x257D, 0x033a },					// 58 ,  3,
	{ 0x257E, 0x0339 },					// 57 ,  3,
	{ 0x257F, 0x033b },					// 59 ,  3,
	{ 0x2580, 0x0305 },					//  5 ,  3,
	{ 0x2584, 0x0307 },					//  7 ,  3,
	{ 0x2588, 0x0303 },					//  3 ,  3,
	{ 0x258C, 0x0304 },					//  4 ,  3,
	{ 0x2590, 0x0306 },					//  6 ,  3,
	{ 0x2591, 0x0300 },					//  0 ,  3,
	{ 0x2592, 0x0301 },					//  1 ,  3,
	{ 0x2593, 0x0302 },					//  2 ,  3,
	{ 0x25A0, 0x0402 },					//  2 ,  4,
	{ 0x25A1, 0x0426 },					// 38 ,  4,
	{ 0x25AA, 0x042f },					// 47 ,  4,
	{ 0x25AB, 0x0431 },					// 49 ,  4,
	{ 0x25AC, 0x050b },					// 11 ,  5,
	{ 0x25B2, 0x0573 },					// 115,  5,
	{ 0x25B3, 0x0688 },					// 136,  6,
	{ 0x25B4, 0x061d },					// 29 ,  6,
	{ 0x25B5, 0x06ac },					// 172,  6,
	{ 0x25B8, 0x061b },					// 27 ,  6,
	{ 0x25B9, 0x068b },					// 139,  6,
	{ 0x25BC, 0x0574 },					// 116,  5,
	{ 0x25BD, 0x0689 },					// 137,  6,
	{ 0x25BE, 0x061e },					// 30 ,  6,
	{ 0x25BF, 0x06ad },					// 173,  6,
	{ 0x25C2, 0x061c },					// 28 ,  6,
	{ 0x25C3, 0x068a },					// 138,  6,
	{ 0x25C6, 0x0575 },					// 117,  5,
	{ 0x25C7, 0x066f },					// 111,  6,
	{ 0x25CA, 0x065f },					// 95 ,  6,
	{ 0x25CB, 0x0401 },					//  1 ,  4,
	{ 0x25CF, 0x0400 },					//  0 ,  4,
	{ 0x25D6, 0x059e },					// 158,  5,
	{ 0x25D7, 0x0577 },					// 119,  5,
	{ 0x25D8, 0x0512 },					// 18 ,  5,
	{ 0x25D9, 0x0513 },					// 19 ,  5,
	{ 0x25E6, 0x042d },					// 45 ,  4,
	{ 0x2605, 0x0548 },					// 72,   5,
	{ 0x260E, 0x051e },					// 30 ,  5,
	{ 0x2610, 0x0518 },					// 24 ,  5,
	{ 0x2612, 0x0519 },					// 25 ,  5,
	{ 0x261B, 0x052a },					// 42 ,  5,
	{ 0x261C, 0x0516 },					// 22 ,  5,
	{ 0x261E, 0x052b },					// 43 ,  5,
	{ 0x2639, 0x051a },					// 26 ,  5,
	{ 0x263A, 0x0507 },					//  7 ,  5,
	{ 0x263B, 0x0508 },					//  8 ,  5,
	{ 0x263C, 0x0506 },					//  6 ,  5,
	{ 0x2640, 0x0505 },					//  5 ,  5,
	{ 0x2642, 0x0504 },					//  4 ,  5,
	{ 0x2660, 0x05ab },					// 171,  5,
	{ 0x2661, 0x0500 },					//  0 ,  5,
	{ 0x2662, 0x0501 },					//  1 ,  5,
	{ 0x2663, 0x05a8 },					// 168,  5,
	{ 0x2664, 0x0503 },					//  3 ,  5,
	{ 0x2665, 0x05aa },					// 170,  5,
	{ 0x2666, 0x05a9 },					// 169,  5,
	{ 0x2667, 0x0502 },					//  2 ,  5,
	{ 0x266A, 0x0509 },					//  9 ,  5,
	{ 0x266C, 0x050a },					// 10 ,  5,
	{ 0x266D, 0x051c },					// 28 ,  5,
	{ 0x266E, 0x051d },					// 29 ,  5,
	{ 0x266F, 0x051b },					// 27 ,  5,
	{ 0x2701, 0x0521 },					// 33 ,  5,
	{ 0x2702, 0x0522 },					// 34 ,  5,
	{ 0x2703, 0x0523 },					// 35 ,  5,
	{ 0x2704, 0x0524 },					// 36 ,  5,
	{ 0x2706, 0x0526 },					// 38 ,  5,
	{ 0x2707, 0x0527 },					// 39 ,  5,
	{ 0x2708, 0x0528 },					// 40 ,  5,
	{ 0x2709, 0x0529 },					// 41 ,  5,
	{ 0x270C, 0x052c },					// 44 ,  5,
	{ 0x270D, 0x052d },					// 45 ,  5,
	{ 0x270E, 0x052e },					// 46 ,  5,
	{ 0x270F, 0x052f },					// 47 ,  5,
	{ 0x2710, 0x0530 },					// 48 ,  5,
	{ 0x2711, 0x0531 },					// 49 ,  5,
	{ 0x2712, 0x0532 },					// 50 ,  5,
	{ 0x2713, 0x0533 },					// 51 ,  5,
	{ 0x2714, 0x0534 },					// 52 ,  5,
	{ 0x2715, 0x0535 },					// 53 ,  5,
	{ 0x2716, 0x0536 },					// 54 ,  5,
	{ 0x2717, 0x0537 },					// 55 ,  5,
	{ 0x2718, 0x0538 },					// 56 ,  5,
	{ 0x2719, 0x0539 },					// 57 ,  5,
	{ 0x271A, 0x053a },					// 58 ,  5,
	{ 0x271B, 0x053b },					// 59 ,  5,
	{ 0x271C, 0x053c },					// 60 ,  5,
	{ 0x271D, 0x053d },					// 61 ,  5,
	{ 0x271E, 0x053e },					// 62 ,  5,
	{ 0x271F, 0x053f },					// 63 ,  5,
	{ 0x2720, 0x0540 },					// 64 ,  5,
	{ 0x2721, 0x0541 },					// 65 ,  5,
	{ 0x2722, 0x0542 },					// 66 ,  5,
	{ 0x2723, 0x0543 },					// 67 ,  5,
	{ 0x2724, 0x0544 },					// 68 ,  5,
	{ 0x2725, 0x0545 },					// 69 ,  5,
	{ 0x2726, 0x0546 },					// 70 ,  5,
	{ 0x2727, 0x0547 },					// 71 ,  5,
	{ 0x2729, 0x0549 },					// 73 ,  5,
	{ 0x272A, 0x054a },					// 74 ,  5,
	{ 0x272B, 0x054b },					// 75 ,  5,
	{ 0x272C, 0x054c },					// 76 ,  5,
	{ 0x272D, 0x054d },					// 77 ,  5,
	{ 0x272E, 0x054e },					// 78 ,  5,
	{ 0x272F, 0x054f },					// 79 ,  5,
	{ 0x2730, 0x0550 },					// 80 ,  5,
	{ 0x2731, 0x0551 },					// 81 ,  5,
	{ 0x2732, 0x0552 },					// 82 ,  5,
	{ 0x2733, 0x0553 },					// 83 ,  5,
	{ 0x2734, 0x0554 },					// 84 ,  5,
	{ 0x2735, 0x0555 },					// 85 ,  5,
	{ 0x2736, 0x0556 },					// 86 ,  5,
	{ 0x2737, 0x0557 },					// 87 ,  5,
	{ 0x2738, 0x0558 },					// 88 ,  5,
	{ 0x2739, 0x0559 },					// 89 ,  5,
	{ 0x273A, 0x055a },					// 90 ,  5,
	{ 0x273B, 0x055b },					// 91 ,  5,
	{ 0x273C, 0x055c },					// 92 ,  5,
	{ 0x273D, 0x055d },					// 93 ,  5,
	{ 0x273E, 0x055e },					// 94 ,  5,
	{ 0x273F, 0x055f },					// 95 ,  5,
	{ 0x2740, 0x0560 },					// 96 ,  5,
	{ 0x2741, 0x0561 },					// 97 ,  5,
	{ 0x2742, 0x0562 },					// 98 ,  5,
	{ 0x2743, 0x0563 },					// 99 ,  5,
	{ 0x2744, 0x0564 },					// 100,  5,
	{ 0x2745, 0x0565 },					// 101,  5,
	{ 0x2746, 0x0566 },					// 102,  5,
	{ 0x2747, 0x0567 },					// 103,  5,
	{ 0x2748, 0x0568 },					// 104,  5,
	{ 0x2749, 0x0569 },					// 105,  5,
	{ 0x274A, 0x056a },					// 106,  5,
	{ 0x274B, 0x056b },					// 107,  5,
	{ 0x274D, 0x056d },					// 109,  5,
	{ 0x274F, 0x056f },					// 111,  5,
	{ 0x2750, 0x0570 },					// 112,  5,
	{ 0x2751, 0x0571 },					// 113,  5,
	{ 0x2752, 0x0572 },					// 114,  5,
	{ 0x2756, 0x0576 },					// 118,  5,
	{ 0x2758, 0x0578 },					// 120,  5,
	{ 0x2759, 0x0579 },					// 121,  5,
	{ 0x275A, 0x057a },					// 122,  5,
	{ 0x275B, 0x057b },					// 123,  5,
	{ 0x275C, 0x057c },					// 124,  5,
	{ 0x275D, 0x057d },					// 125,  5,
	{ 0x275E, 0x057e },					// 126,  5,
	{ 0x2761, 0x05a1 },					// 161,  5,
	{ 0x2762, 0x05a2 },					// 162,  5,
	{ 0x2763, 0x05a3 },					// 163,  5,
	{ 0x2764, 0x05a4 },					// 164,  5,
	{ 0x2765, 0x05a5 },					// 165,  5,
	{ 0x2766, 0x05a6 },					// 166,  5,
	{ 0x2767, 0x05a7 },					// 167,  5,
	{ 0x2776, 0x05b6 },					// 182,  5,
	{ 0x2777, 0x05b7 },					// 183,  5,
	{ 0x2778, 0x05b8 },					// 184,  5,
	{ 0x2779, 0x05b9 },					// 185,  5,
	{ 0x277A, 0x05ba },					// 186,  5,
	{ 0x277B, 0x05bb },					// 187,  5,
	{ 0x277C, 0x05bc },					// 188,  5,
	{ 0x277D, 0x05bd },					// 189,  5,
	{ 0x277E, 0x05be },					// 190,  5,
	{ 0x277F, 0x05bf },					// 191,  5,
	{ 0x2780, 0x05c0 },					// 192,  5,
	{ 0x2781, 0x05c1 },					// 193,  5,
	{ 0x2782, 0x05c2 },					// 194,  5,
	{ 0x2783, 0x05c3 },					// 195,  5,
	{ 0x2784, 0x05c4 },					// 196,  5,
	{ 0x2785, 0x05c5 },					// 197,  5,
	{ 0x2786, 0x05c6 },					// 198,  5,
	{ 0x2787, 0x05c7 },					// 199,  5,
	{ 0x2788, 0x05c8 },					// 200,  5,
	{ 0x2789, 0x05c9 },					// 201,  5,
	{ 0x278A, 0x05ca },					// 202,  5,
	{ 0x278B, 0x05cb },					// 203,  5,
	{ 0x278C, 0x05cc },					// 204,  5,
	{ 0x278D, 0x05cd },					// 205,  5,
	{ 0x278E, 0x05ce },					// 206,  5,
	{ 0x278F, 0x05cf },					// 207,  5,
	{ 0x2790, 0x05d0 },					// 208,  5,
	{ 0x2791, 0x05d1 },					// 209,  5,
	{ 0x2792, 0x05d2 },					// 210,  5,
	{ 0x2793, 0x05d3 },					// 211,  5,
	{ 0x2794, 0x05d4 },					// 212,  5,
	{ 0x2798, 0x05d8 },					// 216,  5,
	{ 0x2799, 0x05d9 },					// 217,  5,
	{ 0x279A, 0x05da },					// 218,  5,
	{ 0x279B, 0x05db },					// 219,  5,
	{ 0x279C, 0x05dc },					// 220,  5,
	{ 0x279D, 0x05dd },					// 221,  5,
	{ 0x279E, 0x05de },					// 222,  5,
	{ 0x279F, 0x05df },					// 223,  5,
	{ 0x27A0, 0x05e0 },					// 224,  5,
	{ 0x27A1, 0x05e1 },					// 225,  5,
	{ 0x27A2, 0x05e2 },					// 226,  5,
	{ 0x27A3, 0x05e3 },					// 227,  5,
	{ 0x27A4, 0x05e4 },					// 228,  5,
	{ 0x27A5, 0x05e5 },					// 229,  5,
	{ 0x27A6, 0x05e6 },					// 230,  5,
	{ 0x27A7, 0x05e7 },					// 231,  5,
	{ 0x27A8, 0x05e8 },					// 232,  5,
	{ 0x27A9, 0x05e9 },					// 233,  5,
	{ 0x27AA, 0x05ea },					// 234,  5,
	{ 0x27AB, 0x05eb },					// 235,  5,
	{ 0x27AC, 0x05ec },					// 236,  5,
	{ 0x27AD, 0x05ed },					// 237,  5,
	{ 0x27AE, 0x05ee },					// 238,  5,
	{ 0x27AF, 0x05ef },					// 239,  5,
	{ 0x27B1, 0x05f1 },					// 241,  5,
	{ 0x27B2, 0x05f2 },					// 242,  5,
	{ 0x27B3, 0x05f3 },					// 243,  5,
	{ 0x27B4, 0x05f4 },					// 244,  5,
	{ 0x27B5, 0x05f5 },					// 245,  5,
	{ 0x27B6, 0x05f6 },					// 246,  5,
	{ 0x27B7, 0x05f7 },					// 247,  5,
	{ 0x27B8, 0x05f8 },					// 248,  5,
	{ 0x27B9, 0x05f9 },					// 249,  5,
	{ 0x27BA, 0x05fa },					// 250,  5,
	{ 0x27BB, 0x05fb },					// 251,  5,
	{ 0x27BC, 0x05fc },					// 252,  5,
	{ 0x27BD, 0x05fd },					// 253,  5,
	{ 0x27BE, 0x05fe },					// 254,  5,
	{ 0xFB00, 0x0433 },					// 51 ,  4,
	{ 0xFB01, 0x0436 },					// 54 ,  4,
	{ 0xFB02, 0x0437 },					// 55 ,  4,
	{ 0xFB03, 0x0434 },					// 52 ,  4,
	{ 0xFB04, 0x0435 },					// 53 ,  4,
	{ 0xFB1E, 0x0930 },					// 48 ,  9,
	{ 0xFF61, 0x0b00 },					//  0 , 11,
	{ 0xFF62, 0x0b01 },					//  1 , 11,
	{ 0xFF63, 0x0b02 },					//  2 , 11,
	{ 0xFF64, 0x0b03 },					//  3 , 11,
	{ 0xFF65, 0x0b04 },					//  4 , 11,
	{ 0xFF66, 0x0b05 },					//  5 , 11,
	{ 0xFF67, 0x0b06 },					//  6 , 11,
	{ 0xFF68, 0x0b07 },					//  7 , 11,
	{ 0xFF69, 0x0b08 },					//  8 , 11,
	{ 0xFF6A, 0x0b09 },					//  9 , 11,
	{ 0xFF6B, 0x0b0a },					// 10 , 11,
	{ 0xFF6C, 0x0b0b },					// 11 , 11,
	{ 0xFF6D, 0x0b0c },					// 12 , 11,
	{ 0xFF6E, 0x0b0d },					// 13 , 11,
	{ 0xFF6F, 0x0b0e },					// 14 , 11,
	{ 0xFF70, 0x0b0f },					// 15 , 11,
	{ 0xFF71, 0x0b10 },					// 16 , 11,
	{ 0xFF72, 0x0b11 },					// 17 , 11,
	{ 0xFF73, 0x0b12 },					// 18 , 11,
	{ 0xFF74, 0x0b13 },					// 19 , 11,
	{ 0xFF75, 0x0b14 },					// 20 , 11,
	{ 0xFF76, 0x0b15 },					// 21 , 11,
	{ 0xFF77, 0x0b16 },					// 22 , 11,
	{ 0xFF78, 0x0b17 },					// 23 , 11,
	{ 0xFF79, 0x0b18 },					// 24 , 11,
	{ 0xFF7A, 0x0b19 },					// 25 , 11,
	{ 0xFF7B, 0x0b1a },					// 26 , 11,
	{ 0xFF7C, 0x0b1b },					// 27 , 11,
	{ 0xFF7D, 0x0b1c },					// 28 , 11,
	{ 0xFF7E, 0x0b1d },					// 29 , 11,
	{ 0xFF7F, 0x0b1e },					// 30 , 11,
	{ 0xFF80, 0x0b1f },					// 31 , 11,
	{ 0xFF81, 0x0b20 },					// 32 , 11,
	{ 0xFF82, 0x0b21 },					// 33 , 11,
	{ 0xFF83, 0x0b22 },					// 34 , 11,
	{ 0xFF84, 0x0b23 },					// 35 , 11,
	{ 0xFF85, 0x0b24 },					// 36 , 11,
	{ 0xFF86, 0x0b25 },					// 37 , 11,
	{ 0xFF87, 0x0b26 },					// 38 , 11,
	{ 0xFF88, 0x0b27 },					// 39 , 11,
	{ 0xFF89, 0x0b28 },					// 40 , 11,
	{ 0xFF8A, 0x0b29 },					// 41 , 11,
	{ 0xFF8B, 0x0b2a },					// 42 , 11,
	{ 0xFF8C, 0x0b2b },					// 43 , 11,
	{ 0xFF8D, 0x0b2c },					// 44 , 11,
	{ 0xFF8E, 0x0b2d },					// 45 , 11,
	{ 0xFF8F, 0x0b2e },					// 46 , 11,
	{ 0xFF90, 0x0b2f },					// 47 , 11,
	{ 0xFF91, 0x0b30 },					// 48 , 11,
	{ 0xFF92, 0x0b31 },					// 49 , 11,
	{ 0xFF93, 0x0b32 },					// 50 , 11,
	{ 0xFF94, 0x0b33 },					// 51 , 11,
	{ 0xFF95, 0x0b34 },					// 52 , 11,
	{ 0xFF96, 0x0b35 },					// 53 , 11,
	{ 0xFF97, 0x0b36 },					// 54 , 11,
	{ 0xFF98, 0x0b37 },					// 55 , 11,
	{ 0xFF99, 0x0b38 },					// 56 , 11,
	{ 0xFF9A, 0x0b39 },					// 57 , 11,
	{ 0xFF9B, 0x0b3a },					// 58 , 11,
	{ 0xFF9C, 0x0b3b },					// 59 , 11,
	{ 0xFF9D, 0x0b3c },					// 60 , 11,
	{ 0xFF9E, 0x0b3d },					// 61 , 11,
	{ 0xFF9F, 0x0b3e }					// 62 , 11
};

/****************************************************************************
Desc:	WP60 to Unicode - Multinational 1
****************************************************************************/
FLMUINT16  WPCH_WP60UNI1[] = 
{
	0x0300, 0x00B7, 0x0303, 0x0302, 0x0335,
	0x0338, 0x0301, 0x0308, 0x0304, 0x0313,
	0x0315, 0x02BC, 0x0326, 0x0315, 0x030A,
	0x0307, 0x030B, 0x0327, 0x0328, 0x030C,
	0x0337, 0x0305, 0x0306, 0x00DF, 0x0138,
	0xF801, 0x00C1, 0x00E1, 0x00C2, 0x00E2,
	0x00C4, 0x00E4, 0x00C0, 0x00E0, 0x00C5,
	0x00E5, 0x00C6, 0x00E6, 0x00C7, 0x00E7,
	0x00C9, 0x00E9, 0x00CA, 0x00EA, 0x00CB,
	0x00EB, 0x00C8, 0x00E8, 0x00CD, 0x00ED,
	0x00CE, 0x00EE, 0x00CF, 0x00EF, 0x00CC,
	0x00EC, 0x00D1, 0x00F1, 0x00D3, 0x00F3,
	0x00D4, 0x00F4, 0x00D6, 0x00F6, 0x00D2,
	0x00F2, 0x00DA, 0x00FA, 0x00DB, 0x00FB,
	0x00DC, 0x00FC, 0x00D9, 0x00F9, 0x0178,
	0x00FF, 0x00C3, 0x00E3, 0x0110, 0x0111,
	0x00D8, 0x00F8, 0x00D5, 0x00F5, 0x00DD,
	0x00FD, 0x00D0, 0x00F0, 0x00DE, 0x00FE,
	0x0102, 0x0103, 0x0100, 0x0101, 0x0104,
	0x0105, 0x0106, 0x0107, 0x010C, 0x010D,
	0x0108, 0x0109, 0x010A, 0x010B, 0x010E,
	0x010F, 0x011A, 0x011B, 0x0116, 0x0117,
	0x0112, 0x0113, 0x0118, 0x0119, 0x0047,
	0x0067, 0x011E, 0x011F, 0x0047, 0x0067,
	0x0122, 0x0123, 0x011C, 0x011D, 0x0120,
	0x0121, 0x0124, 0x0125, 0x0126, 0x0127,
	0x0130, 0x0069, 0x012A, 0x012B, 0x012E,
	0x012F, 0x0128, 0x0129, 0x0132, 0x0133,
	0x0134, 0x0135, 0x0136, 0x0137, 0x0139,
	0x013A, 0x013D, 0x013E, 0x013B, 0x013C,
	0x013F, 0x0140, 0x0141, 0x0142, 0x0143,
	0x0144, 0xF802, 0x0149, 0x0147, 0x0148,
	0x0145, 0x0146, 0x0150, 0x0151, 0x014C,
	0x014D, 0x0152, 0x0153, 0x0154, 0x0155,
	0x0158, 0x0159, 0x0156, 0x0157, 0x015A,
	0x015B, 0x0160, 0x0161, 0x015E, 0x015F,
	0x015C, 0x015D, 0x0164, 0x0165, 0x0162,
	0x0163, 0x0166, 0x0167, 0x016C, 0x016D,
	0x0170, 0x0171, 0x016A, 0x016B, 0x0172,
	0x0173, 0x016E, 0x016F, 0x0168, 0x0169,
	0x0174, 0x0175, 0x0176, 0x0177, 0x0179,
	0x017A, 0x017D, 0x017E, 0x017B, 0x017C,
	0x014A, 0x014B, 0xF000, 0xF001, 0xF002,
	0xF003, 0xF004, 0xF005, 0xF006, 0xF007,
	0xF008, 0xF009, 0xF00A, 0xF00B, 0xF00C,
	0xF00D, 0xF00E, 0xF00F, 0x010E, 0x010F,
	0x01A0, 0x01A1, 0x01AF, 0x01B0, 0x0114,
	0x0115, 0x012C, 0x012D, 0x0049, 0x0131,
	0x014E, 0x014F
};

/****************************************************************************
Desc:	WP60 to Unicode - Standard Phonetic
****************************************************************************/
FLMUINT16 WPCH_WP60UNI2[] =
{
	0x02B9, 0x02BA, 0x02BB, 0xF813, 0x02BD,
	0x02BC, 0xF814, 0x02BE, 0x02BF, 0x0310,
	0x02D0, 0x02D1, 0x0306, 0x032E, 0x0329,
	0x02C8, 0x02CC, 0x02C9, 0x02CA, 0x02CB,
	0x02CD, 0x02CE, 0x02CF, 0x02C6, 0x02C7,
	0x02DC, 0x0325, 0x02DA, 0x032D, 0x032C,
	0x0323, 0x0308, 0x0324, 0x031C, 0x031D,
	0x031E, 0x031F, 0x0320, 0x0321, 0x0322,
	0x032A, 0x032B, 0x02D2, 0x02D3, 0xF815,
	0xF816, 0x005F, 0x2017, 0x033E, 0x02DB,
	0x0327, 0x02DE, 0x02C8, 0x02B0, 0x02B6,	
	0x0250, 0x0251, 0x0252, 0x0253, 0x0299, 
	0x0254, 0x0255, 0x0297,	0x0256, 0x0257,
	0x0258, 0x0259, 0x025A,	0x025B, 0x025C,
	0x025D, 0x029A, 0x025E, 0x025F, 0x0278,
	0x0261, 0x0260, 0x0262, 0x029B, 0x0263,
	0x0264, 0x0265, 0x0266, 0x0267, 0x029C,
	0x0268, 0x026A, 0x0269, 0x029D, 0x029E,
	0x026B, 0x026C, 0x026D, 0x029F, 0x026E,
	0x028E, 0x026F, 0x0270, 0x0271, 0x0272, 
	0x0273, 0x0274, 0x0276, 0x0277, 0x02A0, 
	0x0279, 0x027A, 0x027B, 0x027C, 0x027D, 
	0x027E, 0x027F, 0x0280, 0x0281, 0x0282,
	0x0283, 0x0284, 0x0285, 0x0286, 0x0287,
	0x0288, 0x0275, 0x0289, 0x028A, 0x028C,
	0x028B, 0x028D, 0x0058, 0x028F, 0x0290,
	0x0291, 0x0292, 0x0293, 0x0294, 0x0295,
	0x0296, 0x02A1, 0x02A2, 0x0298, 0x02A3,
	0x02A4, 0x02A5, 0x02A6, 0x02A7, 0x02A8
};

/****************************************************************************
Desc:	WP51/WP60 to Unicode - Box Drawing
****************************************************************************/
FLMUINT16 WPCH_WPUNI3[] = 
{
	0x2591, 0x2592, 0x2593, 0x2588, 0x258C,
	0x2580, 0x2590, 0x2584, 0x2500, 0x2502,
	0x250C, 0x2510, 0x2518, 0x2514, 0x251C,
	0x252C, 0x2524, 0x2534, 0x253C, 0x2550,
	0x2551, 0x2554, 0x2557, 0x255D, 0x255A,
	0x2560, 0x2566, 0x2563, 0x2569, 0x256C,
	0x2552, 0x2555, 0x255B, 0x2558, 0x2553,
	0x2556, 0x255C, 0x2559, 0x255E, 0x2565,
	0x2561, 0x2568, 0x255F, 0x2564, 0x2562,
	0x2567, 0x256B, 0x256A, 0x2574, 0x2575,
	0x2576, 0x2577, 0x2578, 0x2579, 0x257A,
	0x257B, 0x257C, 0x257E, 0x257D, 0x257F,
	0x251F, 0x2522, 0x251E, 0x2521, 0x252E,
	0x2532, 0x252D, 0x2531, 0x2527, 0x2526,
	0x252A, 0x2529, 0x2536, 0x253A, 0x2535,
	0x2539, 0x2541, 0x2546, 0x253E, 0x2540,
	0x2544, 0x254A, 0x253D, 0x2545, 0x2548,
	0x2543, 0x2549, 0x2547
};

/****************************************************************************
Desc:	WP51/WP60 to Unicode - Typographic Symbols
****************************************************************************/
FLMUINT16 WPCH_WPUNI4[] = 
{
	0x25CF, 0x25CB, 0x25A0, 0x2022, 0xF817,
	0x00B6, 0x00A7, 0x00A1, 0x00BF, 0x00AB,
	0x00BB, 0x00A3, 0x00A5, 0x20A7, 0x0192,
	0x00AA, 0x00BA, 0x00BD, 0x00BC, 0x00A2,
	0x00B2, 0x207F, 0x00AE, 0x00A9, 0x00A4,
	0x00BE, 0x00B3, 0x201B, 0x2019, 0x2018,
	0x201F, 0x201D, 0x201C, 0x2013, 0x2014,
	0x2039, 0x203A, 0x25CB, 0x25A1, 0x2020,
	0x2021, 0x2122, 0x2120, 0x211E, 0x25CF,
	0x25E6, 0x25A0, 0x25AA, 0x25A1, 0x25AB,
	0x2012, 0xFB00, 0xFB03, 0xFB04, 0xFB01,
	0xFB02, 0x2026, 0x0024, 0x20A3, 0x20A2,
	0x20A0, 0x20A4, 0x201A, 0x201E, 0x2153,
	0x2154, 0x215B, 0x215C, 0x215D, 0x215E,
	0x24C2, 0x24C5, 0x24CA, 0x2105, 0x2106,
	0x2030, 0x2116, 0xF818, 0x00B9, 0x2409,
	0x240C, 0x240D, 0x240A, 0x2424, 0x240B,
	0xF819, 0x20A9, 0x20A6, 0x20A8, 0xF81A,
	0xF81B, 0xF81C, 0xF81D, 0xF81E, 0xF81F,
	0xF820, 0xF821, 0xF822, 0xF823, 0xF824,
	0xF825, 0xF826, 0x20AC
};

/****************************************************************************
Desc:	WP60 to Unicode - Iconic Symbols
****************************************************************************/
FLMUINT16 WPCH_WP60UNI5[] = 
{
	0x2661, 0x2662, 0x2667, 0x2664, 0x2642,
	0x2640, 0x263C, 0x263A, 0x263B, 0x266A,
	0x266C, 0x25AC, 0x2302, 0x203C, 0x221A,
	0x21A8, 0x2310, 0x2319, 0x25D8, 0x25D9,
	0x21B5, 0x2104, 0x261C, 0x2007, 0x2610,
	0x2612, 0x2639, 0x266F, 0x266D, 0x266E,
	0x260E, 0x231A, 0x231B, 0x2701, 0x2702,
	0x2703, 0x2704, 0x260E, 0x2706, 0x2707,
	0x2708, 0x2709, 0x261B, 0x261E, 0x270C,
	0x270D, 0x270E, 0x270F, 0x2710, 0x2711,
	0x2712, 0x2713, 0x2714, 0x2715, 0x2716,
	0x2717, 0x2718, 0x2719, 0x271A, 0x271B,
	0x271C, 0x271D, 0x271E, 0x271F, 0x2720,
	0x2721, 0x2722, 0x2723, 0x2724, 0x2725,
	0x2726, 0x2727, 0x2605, 0x2729, 0x272A,
	0x272B, 0x272C, 0x272D, 0x272E, 0x272F,
	0x2730, 0x2731, 0x2732, 0x2733, 0x2734,
	0x2735, 0x2736, 0x2737, 0x2738, 0x2739,
	0x273A, 0x273B, 0x273C, 0x273D, 0x273E,
	0x273F, 0x2740, 0x2741, 0x2742, 0x2743,
	0x2744, 0x2745, 0x2746, 0x2747, 0x2748,
	0x2749, 0x274A, 0x274B, 0x25CF, 0x274D,
	0x25A0, 0x274F, 0x2750, 0x2751, 0x2752,
	0x25B2, 0x25BC, 0x25C6, 0x2756, 0x25D7,
	0x2758, 0x2759, 0x275A, 0x275B, 0x275C,
	0x275D, 0x275E, 0x2036, 0x2033, 0xF827,
	0xF828, 0xF829, 0xF82A, 0x2329, 0x232A,
	0x005B, 0x005D, 0xF82B, 0xF82C, 0xF82D,
	0xF82E, 0xF82F, 0xF830, 0xF831, 0x2190,
	0xF832, 0xF833, 0xF834, 0xF835, 0xF836,
	0x21E8, 0x21E6, 0x2794, 0xF838, 0xF839,
	0xF83A, 0xF83B, 0xF83C, 0x25D6, 0xF83D,
	0xF83E, 0x2761, 0x2762, 0x2763, 0x2764,
	0x2765, 0x2766, 0x2767, 0x2663, 0x2666,
	0x2665, 0x2660, 0x2780, 0x2781, 0x2782,
	0x2783, 0x2784, 0x2785, 0x2786, 0x2787,
	0x2788, 0x2789, 0x2776, 0x2777, 0x2778,
	0x2779, 0x277A, 0x277B, 0x277C, 0x277D,
	0x277E, 0x277F, 0x2780, 0x2781, 0x2782,
	0x2783, 0x2784, 0x2785, 0x2786, 0x2787,
	0x2788, 0x2789, 0x278A, 0x278B, 0x278C,
	0x278D, 0x278E, 0x278F, 0x2790, 0x2791,
	0x2792, 0x2793, 0x2794, 0x2192, 0x2194,
	0x2195, 0x2798, 0x2799, 0x279A, 0x279B,
	0x279C, 0x279D, 0x279E, 0x279F, 0x27A0,
	0x27A1, 0x27A2, 0x27A3, 0x27A4, 0x27A5,
	0x27A6, 0x27A7, 0x27A8, 0x27A9, 0x27AA,
	0x27AB, 0x27AC, 0x27AD, 0x27AE, 0x27AF,
	0xF83F, 0x27B1, 0x27B2, 0x27B3, 0x27B4,
	0x27B5, 0x27B6, 0x27B7, 0x27B8, 0x27B9,
	0x27BA, 0x27BB, 0x27BC, 0x27BD, 0x27BE
};

/****************************************************************************
Desc:	WP51/WP60 to Unicode - Math/Scientific
****************************************************************************/
FLMUINT16 WPCH_WPUNI6[] = 
{
	0x2212, 0x00B1, 0x2264, 0x2265, 0x221D,
	0x01C0, 0x2215, 0x2216, 0x00F7, 0x2223,
	0x2329, 0x232A, 0x223C, 0x2248, 0x2261,
	0x2208, 0x2229, 0x2225, 0x2211, 0x221E,
	0x00AC, 0x2192, 0x2190, 0x2191, 0x2193,
	0x2194, 0x2195, 0x25B8, 0x25C2, 0x25B4,
	0x25BE, 0x22C5, 0xF850, 0x2218, 0x2219,
	0x212B, 0x00B0, 0x00B5, 0x203E, 0x00D7,
	0x222B, 0x220F, 0x2213, 0x2207, 0x2202,
	0x02B9, 0x02BA, 0x2192, 0x212F, 0x2113,
	0x210F, 0x2111, 0x211C, 0x2118, 0x21C4,
	0x21C6, 0x21D2, 0x21D0, 0x21D1, 0x21D3,
	0x21D4, 0x21D5, 0x2197, 0x2198, 0x2196,
	0x2199, 0x222A, 0x2282, 0x2283, 0x2286,
	0x2287, 0x220D, 0x2205, 0x2308, 0x2309,
	0x230A, 0x230B, 0x226A, 0x226B, 0x2220,
	0x2297, 0x2295, 0x2296, 0xF851, 0x2299,
	0x2227, 0x2228, 0x22BB, 0x22A4, 0x22A5,
	0x2312, 0x22A2, 0x22A3, 0x25A1, 0x25A0,
	0x25CA, 0xF852, 0xF853, 0xF854, 0x2260,
	0x2262, 0x2235, 0x2234, 0x2237, 0x222E,
	0x2112, 0x212D, 0x2128, 0x2118, 0x20DD,
	0xF855, 0x25C7, 0x22C6, 0x2034, 0x2210,
	0x2243, 0x2245, 0x227A, 0x227C, 0x227B,
	0x227D, 0x2203, 0x2200, 0x22D8, 0x22D9,
	0x228E, 0x228A, 0x228B, 0x2293, 0x2294,
	0x228F, 0x2291, 0x22E4, 0x2290, 0x2292,
	0x22E5, 0x25B3, 0x25BD, 0x25C3, 0x25B9,
	0x22C8, 0x2323, 0x2322, 0xF856, 0x219D,
	0x21A9, 0x21AA, 0x21A3, 0x21BC, 0x21BD,
	0x21C0, 0x21C1, 0x21CC, 0x21CB, 0x21BF,
	0x21BE, 0x21C3, 0x21C2, 0x21C9, 0x21C7,
	0x22D3, 0x22D2, 0x22D0, 0x22D1, 0x229A,
	0x229B, 0x229D, 0x2127, 0x2221, 0x2222,
	0x25C3, 0x25B9, 0x25B5, 0x25BF, 0x2214,
	0x2250, 0x2252, 0x2253, 0x224E, 0x224D,
	0x22A8, 0xF857, 0x226C, 0x0285, 0x2605,
	0x226E, 0x2270, 0x226F, 0x2271, 0x2241,
	0x2244, 0x2247, 0x2249, 0x2280, 0x22E0,
	0x2281, 0x22E1, 0x2284, 0x2285, 0x2288,
	0x2289, 0xF858, 0xF859, 0x22E2, 0x22E3,
	0x2226, 0x2224, 0x226D, 0x2204, 0x2209,
	0xF85A, 0x2130, 0x2131, 0x2102, 0xF85B, 
	0x2115, 0x211D, 0x225F, 0x221F, 0x220B,
	0x22EF, 0xF85C, 0x22EE, 0x22F1, 0xF85D,
	0x20E1, 0x002B, 0x002D, 0x003D, 0x002A,
	0xF85E, 0xF85F, 0xF860, 0x210C, 0x2118, 
	0x2272, 0x2273, 0xF861
};

/****************************************************************************
Desc:	WP51/WP60 to Unicode - Math/Science Extension
****************************************************************************/
FLMUINT16 WPCH_WPUNI7[] =
{
	0x2320, 0x2321, 0xF702, 0xF703, 0x221A,
	0xF705, 0xF706, 0xF707, 0xF708, 0xF709,
	0xF70A, 0xF70B, 0xF70C, 0xF70D, 0xF70E,
	0xF70F, 0xF710, 0xF711, 0xF712, 0xF713,
	0xF714, 0xF715, 0xF716, 0xF717, 0xF718,
	0xF719, 0xF71A, 0xF71B, 0xF71C, 0xF71D,
	0xF71E, 0xF71F, 0xF720, 0xF721, 0xF722,
	0xF723, 0xF724, 0xF725, 0xF726, 0xF727,
	0xF728, 0xF729, 0xF72A, 0xF72B, 0xF72C,
	0xF72D, 0xF72E, 0xF72F, 0xF730, 0xF731,
	0xF732, 0xF733, 0xF734, 0xF735, 0xF736,
	0xF737, 0xF738, 0xF739, 0xF73A, 0xF73B,
	0xF73C, 0xF73D, 0xF73E, 0xF73F, 0xF740,
	0xF741, 0xF742, 0xF743, 0xF744, 0xF745,
	0xF746, 0xF747, 0xF748, 0xF749, 0xF74A,
	0xF74B, 0xF74C, 0xF74D, 0xF74E, 0xF74F,
	0xF750, 0xF751, 0xF752, 0xF753, 0xF754,
	0xF755, 0xF756, 0xF757, 0xF758, 0xF759,
	0xF75A, 0xF75B, 0xF75C, 0xF75D, 0xF75E,
	0xF75F, 0xF760, 0xF761, 0xF762, 0xF763,
	0xF764, 0xF765, 0xF766, 0xF767, 0xF768,
	0xF769, 0xF76A, 0xF76B, 0xF76C, 0xF76D,
	0xF76E, 0xF76F, 0xF770, 0xF771, 0xF772,
	0xF773, 0xF774, 0xF775, 0xF776, 0xF777,
	0xF778, 0xF779, 0xF77A, 0xF77B, 0xF77C,
	0xF77D, 0xF77E, 0xF77F, 0xF780, 0xF781,
	0xF782, 0xF783, 0xF784, 0xF785, 0xF786,
	0xF787, 0xF788, 0xF789, 0xF78A, 0xF78B,
	0xF78C, 0xF78D, 0xF78E, 0xF78F, 0xF790,
	0xF791, 0xF792, 0xF793, 0xF794, 0xF795,
	0xF796, 0xF797, 0xF798, 0xF799, 0xF79A,
	0xF79B, 0xF79C, 0xF79D, 0xF79E, 0xF79F,
	0xF7A0, 0xF7A1, 0xF7A2, 0xF7A3, 0xF7A4,
	0xF7A5, 0xF7A6, 0xF7A7, 0xF7A8, 0xF7A9,
	0xF7AA, 0xF7AB, 0xF7AC, 0xF7AD, 0xF7AE,
	0xF7AF, 0xF7B0, 0xF7B1, 0xF7B2, 0xF7B3,
	0xF7B4, 0xF7B5, 0xF7B6, 0xF7B7, 0xF7B8,
	0xF7B9, 0xF7BA, 0xF7BB, 0xF7BC, 0xF7BD,
	0xF7BE, 0xF7BF, 0xF7C0, 0xF7C1, 0xF7C2,
	0xF7C3, 0xF7C4, 0xF7C5, 0xF7C6, 0xF7C7,
	0xF7C8, 0xF7C9, 0xF7CA, 0xF7CB, 0xF7CC,
	0xF7CD, 0xF7CE, 0xF7CF, 0xF7D0, 0xF7D1,
	0xF7D2, 0xF7D3, 0xF7D4, 0xF7D5, 0xF7D6,
	0xF7D7, 0xF7D8, 0xF7D9, 0xF7DA, 0xF7DB,
	0xF7DC, 0xF7DD, 0xF7DE, 0xF7DF, 0xF7E0,
	0xF7E1, 0xF7E2, 0xF7E3, 0xF7E4
};

/****************************************************************************
Desc:	WP60 to Unicode - Greek
****************************************************************************/
FLMUINT16  WPCH_WP60UNI8[] = 
{
	0x0391, 0x03B1, 0x0392, 0x03B2, 0x0392,
	0x03D0, 0x0393, 0x03B3, 0x0394, 0x03B4,
	0x0395, 0x03B5, 0x0396, 0x03B6, 0x0397,
	0x03B7, 0x0398, 0x03B8, 0x0399, 0x03B9,
	0x039A, 0x03BA, 0x039B, 0x03BB, 0x039C,
	0x03BC, 0x039D, 0x03BD, 0x039E, 0x03BE,
	0x039F, 0x03BF, 0x03A0, 0x03C0, 0x03A1,
	0x03C1, 0x03A3, 0x03C3, 0x03A3, 0x03C2,
	0x03A4, 0x03C4, 0x03A5, 0x03C5, 0x03A6,
	0x03C6, 0x03A7, 0x03C7, 0x03A8, 0x03C8,
	0x03A9, 0x03C9, 0xF106, 0x03AC, 0xF107,
	0x03AD, 0xF108, 0x03AE, 0xF109, 0x03AF,
	0x03AA, 0x03CA, 0xF10A, 0x03CC, 0xF10B,
	0x03CD, 0x03AB, 0x03CB, 0xF10C, 0x03CE, 
	0x03B5, 0x03D1, 0x03F0, 0x03D6, 0x03F1,
	0x03DB, 0x03D2, 0x03D5, 0x03D6, 0x03D7, 
	0x00B7, 0x0374, 0x0375, 0x0301, 0x0308,
	0xF216, 0xF217, 0x0300, 0x0311, 0x0313,
	0x0314, 0x0345, 0x1FCE, 0x1FDE, 0x1FCD,
	0x1FDD, 0xF200, 0xF201, 0xF022, 0xF021,
	0xF202, 0xF203, 0xF204, 0xF300, 0xF301,
	0xF302, 0xF303, 0xF304, 0xF305, 0x1F70,
	0xF100, 0x1FB3, 0x1FB4, 0x1FB2, 0xF205,
	0x1F00, 0x1F04, 0x1F02, 0xF206, 0x1F80, 
	0x1F84, 0x1F82, 0xF306, 0x1F01, 0x1F05,
	0x1F03, 0xF207, 0x1F81, 0x1F85, 0x1F83,
	0xF307, 0x1F72, 0x1F10, 0x1F14, 0x1F12,
	0x1F11, 0x1F15, 0x1F13, 0x1F74, 0xF101,
	0x1FC3, 0x1FC4, 0x1FC2, 0xF208, 0x1F20,
	0x1F24, 0x1F22, 0xF209, 0x1F90, 0x1F94,
	0x1F92, 0xF308, 0x1F21, 0x1F25, 0x1F23,
	0xF20A, 0x1F91, 0x1F95, 0x1F93, 0xF309,
	0x1F76, 0xF102, 0xF20B, 0xF20C, 0x1F30,
	0x1F34, 0x1F32, 0xF20D, 0x1F31, 0x1F35,	
	0x1F33, 0xF20E, 0x1F78, 0x1F40, 0x1F44,
	0x1F42, 0x1F41, 0x1F45, 0x1F43, 0x1FE5, 
	0x1FE4, 0x1F7A, 0xF103, 0xF20F, 0xF210,
	0x1F50, 0x1F54, 0x1F52, 0xF211, 0x1F51,
	0x1F55, 0x1F53, 0xF212, 0x1F7C, 0xF104,
	0x1FF3, 0x1FF4, 0x1FF2, 0xF213, 0x1F60,
	0x1F64, 0x1F62, 0xF214, 0x1FA0, 0x1FA4,
	0x1FA2, 0xF30A, 0x1F61, 0x1F65, 0x1F63, 
	0xF215, 0x1FA1, 0x1FA5, 0x1FA3, 0xF30B,
	0x03DA, 0x03DC, 0x03DE, 0x03E0
};

/****************************************************************************
Desc:	WP60 to Unicode - Hebrew
****************************************************************************/
FLMUINT16  WPCH_WP60UNI9[] = 
{
	0x05D0, 0x05D1, 0x05D2, 0x05D3, 0x05D4,
	0x05D5, 0x05D6, 0x05D7, 0x05D8, 0x05D9,
	0x05DA, 0x05DB, 0x05DC, 0x05DD, 0x05DE,
	0x05DF, 0x05E0, 0x05E1, 0x05E2, 0x05E3,
	0x05E4, 0x05E5, 0x05E6, 0x05E7, 0x05E8,
	0x05E9, 0x05EA, 0xF862, 0x05C0, 0x05C3,
	0x05F3, 0x05F4, 0x05B0, 0x05B1, 0x05B2,
	0x05B3, 0x05B4, 0x05B5, 0x05B6, 0x05B7,
	0x05B8, 0x05B9, 0x05B9, 0x05BB, 0x05BC,
	0x05BD, 0x05BF, 0x05B7, 0xFB1E, 0x05F0,
	0x05F1, 0x05F2, 0xF114, 0xF8B0, 0xF863,
	0xF864, 0xF865, 0xF866, 0xF867, 0xF868,
	0xF869, 0xF86A, 0xF86B, 0xF86C, 0xF86D,
	0xF86E, 0xF86F, 0xF870, 0xF871, 0xF872,
	0xF873, 0xF874, 0x05F3, 0x05F3, 0x05F4,
	0xF876, 0xF877, 0xF878, 0xF879, 0xF87A,
	0xF87B, 0xF87C, 0xF87D, 0xF87E, 0xF115,
	0xF116, 0xF87F, 0xF117, 0xF118, 0xF119,
	0xF11A, 0xF11B, 0xF11C, 0xF11D, 0xF11E,
	0xF11F, 0xF120, 0xF121, 0xF122, 0xF123,
	0xF124, 0xF125, 0xF126, 0xF127, 0xF218,
	0xF128, 0xF129, 0xF12A, 0xF12B, 0xF12C,
	0xF12D, 0xF880, 0xF12E, 0xF12F, 0xF130,
	0xF219, 0x05E9, 0xF131, 0xF132, 0xF140,
	0xF141, 0xF142, 0x20AA
};

/****************************************************************************
Desc:	WP60 to Unicode - Cyrillic/Georgian
****************************************************************************/
FLMUINT16 WPCH_WP60UNI10[] = 
{
	0x0410, 0x0430, 0x0411, 0x0431, 0x0412,
	0x0432, 0x0413, 0x0433, 0x0414, 0x0434,
	0x0415, 0x0435, 0x0401, 0x0451, 0x0416,
	0x0436, 0x0417, 0x0437, 0x0418, 0x0438,
	0x0419, 0x0439, 0x041A, 0x043A, 0x041B,
	0x043B, 0x041C, 0x043C, 0x041D, 0x043D,
	0x041E, 0x043E, 0x041F, 0x043F, 0x0420,
	0x0440, 0x0421, 0x0441, 0x0422, 0x0442,
	0x0423, 0x0443, 0x0424, 0x0444, 0x0425,
	0x0445, 0x0426, 0x0446, 0x0427, 0x0447,
	0x0428, 0x0448, 0x0429, 0x0449, 0x042A,
	0x044A, 0x042B, 0x044B, 0x042C, 0x044C,
	0x042D, 0x044D, 0x042E, 0x044E, 0x042F,
	0x044F, 0x04D8, 0x04D9, 0x0403, 0x0453,
	0x0490, 0x0491, 0x0492, 0x0493, 0x0402,
	0x0452, 0x0404, 0x0454, 0x0404, 0x0454,
	0x0496, 0x0497, 0x0405, 0x0455, 0xF159,
	0xF889, 0xF15E, 0xF15F, 0x0406, 0x0456,
	0x0407, 0x0457, 0xF88C, 0xF88D, 0x0408,
	0x0458, 0x040C, 0x045C, 0x049A, 0x049B,
	0xF160, 0xF161, 0x049C, 0x049D, 0x0409,
	0x0459, 0x04A2, 0x04A3, 0x040A, 0x045A,
	0x047A, 0x047B, 0x0460, 0x0461, 0x040B,
	0x045B, 0x040E, 0x045E, 0x04EE, 0x04EF,
	0x04AE, 0x04AF, 0x04B0, 0x04B1, 0x0194,
	0x0263, 0x04B2, 0x04B3, 0xF162, 0xF163,
	0x04BA, 0x04BB, 0x047E, 0x047F, 0x040F,
	0x045F, 0x04B6, 0x04B7, 0x04B8, 0x04B9,
	0xF164, 0xF165, 0x0462, 0x0463, 0x0466,
	0x0467, 0x046A, 0x046B, 0x046E, 0x046F,
	0x0470, 0x0471, 0x0472, 0x0473, 0x0474,
	0x0475, 0xF400, 0xF401, 0xF402, 0xF403,
	0xF404, 0xF405, 0xF406, 0xF407, 0xF408,
	0xF409, 0xF40A, 0xF40B, 0xF40C, 0xF40D,
	0xF40E, 0xF40F, 0xF410, 0xF411, 0xF412,
	0xF413, 0xF414, 0xF415, 0xF416, 0xF417,
	0xF418, 0xF419, 0xF41A, 0xF41B, 0xF41C,
	0xF41D, 0xF41E, 0xF41F, 0xF420, 0xF421,
	0xF422, 0xF423, 0xF424, 0xF425, 0xF426,
	0xF427, 0xF428, 0xF429, 0xF42A, 0xF42B,
	0x0301, 0x0300, 0x0308, 0x0306, 0x0326,
	0x0328, 0x0304, 0xF893, 0x201E, 0x201F,
	0x10D0, 0x10D1, 0x10D2, 0x10D3, 0x10D4,
	0x10D5, 0x10D6, 0x10F1, 0x10D7, 0x10D8,
	0x10D9, 0x10DA, 0x10DB, 0x10DC, 0x10F2,
	0x10DD, 0x10DE, 0x10DF, 0x10E0, 0x10E1,
	0x10E2, 0x10E3, 0x10F3, 0x10E4, 0x10E5,
	0x10E6, 0x10E7, 0x10E8, 0x10E9, 0x10EA,
	0x10EB, 0x10EC, 0x10ED, 0x10EE, 0x10F4,
	0x10EF, 0x10F0, 0x10F5, 0x10F6, 0xF42C
};

/****************************************************************************
Desc:	WP60 to Unicode - Japanese
****************************************************************************/
FLMUINT16 WPCH_WP60UNI11[] = 
{
	0xFF61, 0xFF62, 0xFF63, 0xFF64, 0xFF65,
	0xFF66, 0xFF67, 0xFF68, 0xFF69, 0xFF6A,
	0xFF6B, 0xFF6C, 0xFF6D, 0xFF6E, 0xFF6F,
	0xFF70, 0xFF71, 0xFF72, 0xFF73, 0xFF74,
	0xFF75, 0xFF76, 0xFF77, 0xFF78, 0xFF79,
	0xFF7A, 0xFF7B, 0xFF7C, 0xFF7D, 0xFF7E,
	0xFF7F, 0xFF80, 0xFF81, 0xFF82, 0xFF83,
	0xFF84, 0xFF85, 0xFF86, 0xFF87, 0xFF88,
	0xFF89, 0xFF8A, 0xFF8B, 0xFF8C, 0xFF8D,
	0xFF8E, 0xFF8F, 0xFF90, 0xFF91, 0xFF92,
	0xFF93, 0xFF94, 0xFF95, 0xFF96, 0xFF97,
	0xFF98, 0xFF99, 0xFF9A, 0xFF9B, 0xFF9C,
	0xFF9D, 0xFF9E, 0xFF9F
};

/****************************************************************************
Desc:	WP60 to Unicode - Arabic
****************************************************************************/
FLMUINT16 WPCH_WPUNI13[] = 
{
	0xF895, 0xF896, 0xF897, 0xF898, 0xF899,
	0xF89A, 0xF89B, 0xF89C, 0xF89D, 0xF89E,
	0x064E, 0x064E, 0x064F, 0x064F, 0x0650,
	0x0650, 0x064B, 0x064C, 0x064C, 0x0650,
	0x0652, 0x0652, 0x0651, 0xF503, 0xF502,
	0xF504, 0xF508, 0xF505, 0xF509, 0xF506,
	0xF50A, 0xF50B, 0xF507, 0xF50C, 0x06E4,
	0x06E4, 0x0674, 0x06D6, 0x060C, 0x061B,
	0x061F, 0x002A, 0x066A, 0x226B, 0x226A,
	0x0029, 0x0028, 0x0661, 0x0662, 0x0663,
	0x0664, 0x0665, 0x0666, 0x0667, 0x0668,
	0x0669, 0x0660, 0x0662, 0x0627, 0x0628,
	0x0628, 0x0628, 0x0628, 0x0628, 0x062A,
	0x062A, 0x062A, 0x062A, 0x062B, 0x062B,
	0x062B, 0x062B, 0x062C, 0x062C, 0x062C,
	0x062C, 0x062D, 0x062D, 0x062D, 0x062D,
	0x062E, 0x062E, 0x062E, 0x062E, 0x062F,
	0x062F, 0x0630, 0x0630, 0x0631, 0x0631,
	0x0632, 0x0632, 0x0633, 0x0633, 0x0633,
	0x0633, 0x0634, 0x0634, 0x0634, 0x0634,
	0x0635, 0x0635, 0x0635, 0x0635, 0x0636,
	0x0636, 0x0636, 0x0636, 0x0637, 0x0637,
	0x0637, 0x0637, 0x0638, 0x0638, 0x0638,
	0x0638, 0x0639, 0x0639, 0x0639, 0x0639,
	0x063A, 0x063A, 0x063A, 0x063A, 0x0641, 
	0x0641, 0x0641, 0x0641, 0x0642, 0x0642,
	0x0642, 0x0642, 0x0643, 0x0643, 0x0643,
	0x0643, 0x0644, 0x0644, 0x0644, 0x0644,
	0x0645, 0x0645, 0x0645, 0x0645, 0x0646,
	0x0646, 0x0646, 0x0646, 0x0647, 0x0647,
	0x0647, 0x0647, 0x0629, 0x0629, 0x0648,
	0x0648, 0x064A, 0x064A, 0x064A, 0x064A, 
	0x0649, 0x0649, 0x0649, 0x0649, 0x0621,
	0x0623, 0x0623, 0x0625, 0x0625, 0x0624,
	0x0624, 0x0626, 0x0626, 0x0626, 0x0626,
	0xF50D, 0xF50D, 0x0622, 0x0622, 0x0671,
	0x0671, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0640,
	0x0640
};

/****************************************************************************
Desc:	WP60 to Unicode - Arabic Script
****************************************************************************/
FLMUINT16 WPCH_WPUNI14[] = 
{
	0xF8B6, 0xF8B7, 0xF8B8, 0xF8B9, 0xF8BA,
	0xF8BB, 0xF8BC, 0xF8BD, 0xF8BE, 0xF8BF,			  
	0xF8C0, 0xF8C1, 0xF8C2, 0xF8C3, 0xF8C4,
	0xF8C5, 0xF8C6, 0xF8C7, 0xF8C8, 0xF8C9,
	0xF8CA, 0xF8CB, 0xF8CC, 0xF8CD, 0xF8CE,
	0xF8CF, 0xF8D0, 0x064E, 0x0652, 0xF8D3,
	0xF8D4, 0xF8D5, 0xF8D6, 0xF8D7, 0xF8D8,
	0xF8D9, 0x0674, 0x064C, 0xF8B2, 0xF8DB,
	0xF8DC, 0x06F4, 0x06F4, 0x06F5, 0x06F6,
	0x06F6, 0x06F7, 0x06F8, 0x067B, 0x067B,
	0x067B, 0x067B, 0x0680, 0x0680, 0x0680,
	0x0680, 0x067E, 0x067E, 0x067E, 0x067E,
	0x0679, 0x0679, 0x0679, 0x0679, 0x067C,
	0x067C, 0x067C, 0x067C, 0x067F, 0x067F,
	0x067F, 0x067F, 0x067D, 0x067D, 0x067D,
	0x067D, 0x067A, 0x067A, 0x067A, 0x067A,
	0x0684, 0x0684, 0x0684, 0x0684, 0x0683,
	0x0683, 0x0683, 0x0683, 0x0686, 0x0686, 
	0x0686, 0x0686, 0x0687, 0x0687, 0x0687,
	0x0687, 0x0685, 0x0685, 0x0685, 0x0685,
	0x0681, 0x0681, 0x0681, 0x0681, 0x0688,
	0x0688, 0x0689, 0x0689, 0x068C, 0x068C,
	0x068E, 0x068E, 0x068A, 0x068A, 0x068D,
	0x068D, 0x0693, 0x0693, 0x0691, 0x0691,
	0x0699, 0x0699, 0x0695, 0x0695, 0x0692,
	0x0692, 0x0698, 0x0698, 0x0696, 0x0696,
	0x0696, 0x0696, 0x069A, 0x069A, 0x069A,
	0x069A, 0x06A0, 0x06A0, 0x06A0, 0x06A0, 
	0x06A4, 0x06A4, 0x06A4, 0x06A4, 0x06A6,
	0x06A6, 0x06A6, 0x06A6, 0x06A9, 0x06A9, 
	0x06A9, 0x06A9, 0x06A9, 0x06A9, 0x06A9,
	0x06A9, 0x06AA, 0x06AA, 0x06AA, 0x06AA, 
	0x06AF, 0x06AF, 0x06AF, 0x06AF, 0x06AF,
	0x06AF, 0x06AF, 0x06AF, 0x06AB, 0x06AB, 
	0x06AB, 0x06AB, 0x06B1, 0x06B1, 0x06B1,
	0x06B1, 0x06B3, 0x06B3, 0x06B3, 0x06B3, 
	0x06B5, 0x06B5, 0x06B5, 0x06B5, 0x0000,
	0x0000, 0x06BA, 0x06BA, 0x06BA, 0x06BA, 
	0x06BC, 0x06BC, 0x06BC, 0x06BC, 0x06BB,
	0x06BB, 0x06BB, 0x06BB, 0x06C6, 0x06C6,
	0x06CA, 0x06CA, 0x06CA, 0x06CA, 0x0647,
	0x0647, 0x0647, 0x0647, 0x06CE, 0x06CE, 
	0x06CE, 0x06CE, 0x06D2, 0x06D2, 0x06D1,
	0x06D1, 0x06D1, 0x06D1, 0x06C0, 0x06C0
};

/****************************************************************************
Desc:	WP60 to Unicode - Complex Character Unit Table (Multinational 1)
****************************************************************************/
FLMUINT16 WPCH_CPXTAB1[][5] = 
{
	{ 0x0044, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0063, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x004C, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x006C, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x004E, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x006E, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0052, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0072, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0053, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0073, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0054, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0074, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0059, 0x0306, 0x0000, 0x0000, 0x0000 },
	{ 0x0079, 0x0306, 0x0000, 0x0000, 0x0000 },
	{ 0x0059, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0079, 0x0300, 0x0000, 0x0000, 0x0000 }
};

/****************************************************************************
Desc:	WP60 to Unicode - Complex Character Unit Table (Greek)
****************************************************************************/
FLMUINT16 WPCH_CPXGREEK[][5] = 
{
	{ 0x0020, 0x0313, 0x0301, 0x0000, 0x0000 },
	{ 0x0020, 0x0314, 0x0301, 0x0000, 0x0000 },
	{ 0x0020, 0x0313, 0x0300, 0x0000, 0x0000 },
	{ 0x0020, 0x0314, 0x0300, 0x0000, 0x0000 },
	{ 0x0020, 0x0313, 0x0302, 0x0000, 0x0000 },
	{ 0x0020, 0x0314, 0x0302, 0x0000, 0x0000 },
	{ 0x0020, 0x0345, 0x0301, 0x0000, 0x0000 },
	{ 0x0020, 0x0345, 0x0300, 0x0000, 0x0000 },
	{ 0x0020, 0x0345, 0x0302, 0x0000, 0x0000 },
	{ 0x0020, 0x0313, 0x0345, 0x0000, 0x0000 },
	{ 0x0020, 0x0314, 0x0345, 0x0000, 0x0000 },
	{ 0x0020, 0x0313, 0x0301, 0x0345, 0x0000 },
	{ 0x0020, 0x0314, 0x0301, 0x0345, 0x0000 },
	{ 0x0020, 0x0313, 0x0300, 0x0345, 0x0000 },
	{ 0x0020, 0x0314, 0x0300, 0x0345, 0x0000 },
	{ 0x0020, 0x0313, 0x0302, 0x0345, 0x0000 },
	{ 0x0020, 0x0302, 0x0345, 0x0314, 0x0000 },
	{ 0x03B1, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03B1, 0x0302, 0x0000, 0x0000, 0x0000 },
	{ 0x03B1, 0x0345, 0x0000, 0x0000, 0x0000 },
	{ 0x03B1, 0x0301, 0x0345, 0x0000, 0x0000 },
	{ 0x03B1, 0x0302, 0x0345, 0x0000, 0x0000 },
	{ 0x03B1, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03B1, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03B1, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03B1, 0x0302, 0x0313, 0x0000, 0x0000 },
	{ 0x03B1, 0x0345, 0x0313, 0x0000, 0x0000 },
	{ 0x03B1, 0x0301, 0x0345, 0x0313, 0x0000 },
	{ 0x03B1, 0x0302, 0x0345, 0x0313, 0x0000 },
	{ 0x03B1, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03B1, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03B1, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03B1, 0x0302, 0x0314, 0x0000, 0x0000 },
	{ 0x03B1, 0x0345, 0x0314, 0x0000, 0x0000 },
	{ 0x03B1, 0x0301, 0x0345, 0x0314, 0x0000 },
	{ 0x03B1, 0x0302, 0x0345, 0x0314, 0x0000 },
	{ 0x03B5, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03B5, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03B5, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03B5, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03B5, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03B5, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03B5, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03B7, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03B7, 0x0310, 0x0000, 0x0000, 0x0000 },
	{ 0x03B7, 0x0345, 0x0000, 0x0000, 0x0000 },
	{ 0x03B7, 0x0301, 0x0345, 0x0000, 0x0000 },
	{ 0x03B7, 0x0300, 0x0345, 0x0000, 0x0000 },
	{ 0x03B7, 0x0302, 0x0345, 0x0000, 0x0000 },
	{ 0x03B7, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03B7, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03B7, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03B7, 0x0302, 0x0313, 0x0000, 0x0000 },
	{ 0x03B7, 0x0345, 0x0313, 0x0000, 0x0000 },
	{ 0x03B7, 0x0301, 0x0345, 0x0313, 0x0000 },
	{ 0x03B7, 0x0302, 0x0345, 0x0313, 0x0000 },
	{ 0x03B7, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03B7, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03B7, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03B7, 0x0302, 0x0314, 0x0000, 0x0000 },
	{ 0x03B7, 0x0345, 0x0314, 0x0000, 0x0000 },
	{ 0x03B7, 0x0301, 0x0345, 0x0314, 0x0000 },
	{ 0x03B7, 0x0302, 0x0345, 0x0314, 0x0000 },
	{ 0x03B9, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03B9, 0x0302, 0x0000, 0x0000, 0x0000 },
	{ 0x03B9, 0x0308, 0x0301, 0x0000, 0x0000 },
	{ 0x03B9, 0x0308, 0x0300, 0x0000, 0x0000 },
	{ 0x03B9, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03B9, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03B9, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03B9, 0x0302, 0x0313, 0x0000, 0x0000 },
	{ 0x03B9, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03B9, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03B9, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03B9, 0x0302, 0x0314, 0x0000, 0x0000 },
	{ 0x03BF, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03BF, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03BF, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03BF, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03BF, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03BF, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03BF, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03C5, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03C5, 0x0302, 0x0000, 0x0000, 0x0000 },
	{ 0x03C5, 0x0308, 0x0301, 0x0000, 0x0000 },
	{ 0x03C5, 0x0308, 0x0300, 0x0000, 0x0000 },
	{ 0x03C5, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03C5, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03C5, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03C5, 0x0302, 0x0313, 0x0000, 0x0000 },
	{ 0x03C5, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03C5, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03C5, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03C5, 0x0302, 0x0314, 0x0000, 0x0000 },
	{ 0x03C9, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x03C9, 0x0302, 0x0000, 0x0000, 0x0000 },
	{ 0x03C9, 0x0345, 0x0000, 0x0000, 0x0000 },
	{ 0x03C9, 0x0301, 0x0345, 0x0000, 0x0000 },
	{ 0x03C9, 0x0300, 0x0345, 0x0000, 0x0000 },
	{ 0x03C9, 0x0302, 0x0345, 0x0000, 0x0000 },
	{ 0x03C9, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03C9, 0x0301, 0x0313, 0x0000, 0x0000 },
	{ 0x03C9, 0x0300, 0x0313, 0x0000, 0x0000 },
	{ 0x03C9, 0x0302, 0x0313, 0x0000, 0x0000 },
	{ 0x03C9, 0x0345, 0x0313, 0x0000, 0x0000 },
	{ 0x03C9, 0x0301, 0x0345, 0x0313, 0x0000 },
	{ 0x03C9, 0x0302, 0x0345, 0x0313, 0x0000 },
	{ 0x03C9, 0x0314, 0x0000, 0x0000, 0x0000 },
	{ 0x03C9, 0x0301, 0x0314, 0x0000, 0x0000 },
	{ 0x03C9, 0x0300, 0x0314, 0x0000, 0x0000 },
	{ 0x03C9, 0x0302, 0x0314, 0x0000, 0x0000 },
	{ 0x03C9, 0x0345, 0x0314, 0x0000, 0x0000 },
	{ 0x03C9, 0x0301, 0x0345, 0x0314, 0x0000 },
	{ 0x03C9, 0x0302, 0x0345, 0x0314, 0x0000 },
	{ 0x0391, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0395, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0397, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0399, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x039F, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x03A5, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x03A9, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x03C1, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03B1, 0x0300, 0x0345, 0x0000, 0x0000 },
	{ 0x03B1, 0x0300, 0x0313, 0x0345, 0x0000 },
	{ 0x03B1, 0x0300, 0x0314, 0x0345, 0x0000 },
	{ 0x03B7, 0x0300, 0x0313, 0x0345, 0x0000 },
	{ 0x03B7, 0x0300, 0x0314, 0x0345, 0x0000 },
	{ 0x03C1, 0x0313, 0x0000, 0x0000, 0x0000 },
	{ 0x03C9, 0x0300, 0x0313, 0x0345, 0x0000 },
	{ 0x03C9, 0x0300, 0x0314, 0x0345, 0x0000 },
	{ 0x0020, 0x0301, 0x0308, 0x0000, 0x0000 },
	{ 0x0020, 0x0300, 0x0308, 0x0000, 0x0000 }
};

/****************************************************************************
Desc:	WP60 to Unicode - Complex Character Unit Table (Hebrew)
****************************************************************************/
FLMUINT16 WPCH_CPXHEBREW[][5] = {
	{ 0x05F2, 0x05B7, 0x0000, 0x0000, 0x0000 },
	{ 0x05D0, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D1, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D2, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D3, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D4, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D5, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D5, 0x05B9, 0x0000, 0x0000, 0x0000 },
	{ 0x05D6, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D7, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D8, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D9, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05D9, 0x05B4, 0x0000, 0x0000, 0x0000 },
	{ 0x05DB, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05B0, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05B5, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05B1, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05B7, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05B8, 0x0000, 0x0000, 0x0000 },
	{ 0x05DA, 0x05BC, 0x05B8, 0x0000, 0x0000 },
	{ 0x05DC, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05DE, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05E0, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05DF, 0x05B8, 0x0000, 0x0000, 0x0000 },
	{ 0x05E1, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05E4, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05E6, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05E7, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05E9, 0x05B9, 0x0000, 0x0000, 0x0000 },
	{ 0x05E9, 0x05B9, 0x05BC, 0x0000, 0x0000 },
	{ 0x05E9, 0x05BC, 0x0000, 0x0000, 0x0000 },
	{ 0x05EA, 0x05BC, 0x0000, 0x0000, 0x0000 }
};

/****************************************************************************
Desc:	WP60 to Unicode - Complex Character Unit Table (Arabic)
****************************************************************************/
FLMUINT16 WPCH_CPXARABIC[][5] = 
{
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
};

/****************************************************************************
Desc:	WP60 to Unicode - Complex Character Unit Table (Arabic Script)
****************************************************************************/
FLMUINT16  WPCH_CPXARABIC2[][5] = 
{
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
};	

/****************************************************************************
Desc:	WP60 to Unicode - Complex Character Unit Table (Cyrillic / Georgian)
****************************************************************************/
FLMUINT16 WPCH_CPXCYRILLIC[][5] = {
	{ 0x0410, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0430, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0415, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0435, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0404, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0454, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0418, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0438, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0406, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0456, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0407, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0457, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x041E, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x043E, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0423, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0443, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x042B, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x044B, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x042D, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x044D, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x042E, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x044E, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x042F, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x044F, 0x0301, 0x0000, 0x0000, 0x0000 },
	{ 0x0410, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0430, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0415, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0435, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0401, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0451, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0418, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0438, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x041E, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x043E, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0423, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x0443, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x042B, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x044B, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x042D, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x044D, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x042E, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x044E, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x042F, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x044F, 0x0300, 0x0000, 0x0000, 0x0000 },
	{ 0x10E3, 0x0302, 0x0000, 0x0000, 0x0000 },
	{ 0x0037, 0x0339, 0x0000, 0x0000, 0x0000 },
	{ 0x0428, 0x0329, 0x0000, 0x0000, 0x0000 },
	{ 0x0448, 0x0329, 0x0000, 0x0000, 0x0000 },
	{ 0x0406, 0x031C, 0x0000, 0x0000, 0x0000 },
	{ 0x0446, 0x031C, 0x0000, 0x0000, 0x0000 },
	{ 0x0418, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x0438, 0x0304, 0x0000, 0x0000, 0x0000 },
	{ 0x041A, 0x0328, 0x0000, 0x0000, 0x0000 },
	{ 0x043A, 0x0328, 0x0000, 0x0000, 0x0000 },
	{ 0x0425, 0x0328, 0x0000, 0x0000, 0x0000 },
	{ 0x0445, 0x0328, 0x0000, 0x0000, 0x0000 },
	{ 0x0428, 0x0329, 0x0000, 0x0000, 0x0000 },
	{ 0x0448, 0x0329, 0x0000, 0x0000, 0x0000 }
};

/****************************************************************************
Desc:
****************************************************************************/
FLMUINT16 * WP60toUni[] = 
{
	0,
	WPCH_WP60UNI1,
	WPCH_WP60UNI2,
	WPCH_WPUNI3,
	WPCH_WPUNI4,
	WPCH_WP60UNI5,
	WPCH_WPUNI6,
	WPCH_WPUNI7,
	WPCH_WP60UNI8,
	WPCH_WP60UNI9,
	WPCH_WP60UNI10,
	WPCH_WP60UNI11,
	0,
	WPCH_WPUNI13,
	WPCH_WPUNI14
};

/****************************************************************************
Desc:
****************************************************************************/
FLMUINT16 * WP60toCpxUni[] = 
{
	0,
	(FLMUINT16 *)WPCH_CPXTAB1,
	0,
	0,
	0,
	0,
	0,
	0,
	(FLMUINT16 *)WPCH_CPXGREEK,
	(FLMUINT16 *)WPCH_CPXHEBREW,
	(FLMUINT16 *)WPCH_CPXCYRILLIC,
	(FLMUINT16 *)WPCH_CPXARABIC,
	(FLMUINT16 *)WPCH_CPXARABIC2
};

/****************************************************************************
Desc: Converts a character to upper case (if possible)
****************************************************************************/
FLMUINT16 flmCh6Upper(
	FLMUINT16	ui16WpChar)
{
	if (ui16WpChar < 256)
	{
		if (ui16WpChar >= ASCII_LOWER_A && ui16WpChar <= ASCII_LOWER_Z)
		{

			// Return ASCII upper case

			return (ui16WpChar & 0xdf);
		}
	}
	else
	{
		FLMBYTE	ucCharSet = ui16WpChar >> 8;

		if (ucCharSet == CHSMUL1)
		{
			FLMBYTE	ucChar = ui16WpChar & 0xFF;

			if (ucChar >= flmCaseConvertableRange[(CHSMUL1 - 1) * 2] &&
				 ucChar <= flmCaseConvertableRange[((CHSMUL1 - 1) * 2) + 1])
			{
				return (ui16WpChar & 0xFFFE);
			}
		}
		else if (ucCharSet == CHSGREK)
		{
			if ((ui16WpChar & 0xFF) <= flmCaseConvertableRange[
					 ((CHSGREK - 1) * 2) + 1])
			{
				return (ui16WpChar & 0xFFFE);
			}
		}
		else if (ucCharSet == CHSCYR)
		{
			if ((ui16WpChar & 0xFF) <= flmCaseConvertableRange[
					 ((CHSCYR - 1) * 2) + 1])
			{
				return (ui16WpChar & 0xFFFE);
			}
		}
		else if (ui16WpChar >= Lower_JP_a)
		{

			// Possible double byte character set alphabetic character?

			if (ui16WpChar <= Lower_JP_z)
			{

				// Japanese?

				ui16WpChar = (ui16WpChar - Lower_JP_a) + Upper_JP_A;
			}
			else if (ui16WpChar >= Lower_KR_a && ui16WpChar <= Lower_KR_z)
			{

				// Korean?

				ui16WpChar = (ui16WpChar - Lower_KR_a) + Upper_KR_A;
			}
			else if (ui16WpChar >= Lower_CS_a && ui16WpChar <= Lower_CS_z)
			{

				// Chinese Simplified?

				ui16WpChar = (ui16WpChar - Lower_CS_a) + Upper_CS_A;
			}
			else if (ui16WpChar >= Lower_CT_a && ui16WpChar <= Lower_CT_z)
			{

				// Chinese Traditional?

				ui16WpChar = (ui16WpChar - Lower_CT_a) + Upper_CT_A;
			}
		}
	}

	// Return original character - original not in lower case.

	return (ui16WpChar);
}

/****************************************************************************
Desc: Checks to see if WP character is upper case
****************************************************************************/
FLMBOOL flmIsUpper(
	FLMUINT16	ui16WpChar)
{
	FLMBYTE		ucChar;
	FLMBYTE		ucCharSet;

	// Get character

	ucChar = (FLMBYTE) (ui16WpChar & 0xFF);

	// Test if ASCII character set

	if (!(ui16WpChar & 0xFF00))
	{
		return ((ucChar >= ASCII_LOWER_A && ucChar <= ASCII_LOWER_Z) 
							? FALSE 
							: TRUE);
	}

	// Get the character set

	ucCharSet = (FLMBYTE) (ui16WpChar >> 8);

	// CHSMUL1 == Multinational 1 character set 
	// CHSGREK == Greek character set 
	// CHSCYR == Cyrillic character set

	if ((ucCharSet == CHSMUL1 && ucChar >= 26 && ucChar <= 241) ||
		 (ucCharSet == CHSGREK && ucChar <= 69) ||
		 (ucCharSet == CHSCYR && ucChar <= 199))
	{
		return ((ucChar & 1) ? FALSE : TRUE);
	}

	// Don't care that double ss is lower

	return (TRUE);
}

/****************************************************************************
Desc: Converts a character to lower case (if possible)
****************************************************************************/
FLMUINT16 flmCh6Lower(
	FLMUINT16	ui16WpChar)
{
	if (ui16WpChar < 256)
	{
		if (ui16WpChar >= ASCII_UPPER_A && ui16WpChar <= ASCII_UPPER_Z)
		{
			return (ui16WpChar | 0x20);
		}
	}
	else
	{
		FLMBYTE	ucCharSet = ui16WpChar >> 8;

		if (ucCharSet == CHSMUL1)
		{
			FLMBYTE	ucChar = ui16WpChar & 0xFF;

			if (ucChar >= flmCaseConvertableRange[(CHSMUL1 - 1) * 2] &&
				 ucChar <= flmCaseConvertableRange[((CHSMUL1 - 1) * 2) + 1])
			{
				return (ui16WpChar | 1);
			}
		}
		else if (ucCharSet == CHSGREK)
		{
			if ((ui16WpChar & 0xFF) <= flmCaseConvertableRange[
					 ((CHSGREK - 1) * 2) + 1])
			{
				return (ui16WpChar | 1);
			}
		}
		else if (ucCharSet == CHSCYR)
		{
			if ((ui16WpChar & 0xFF) <= flmCaseConvertableRange[
					 ((CHSCYR - 1) * 2) + 1])
			{
				return (ui16WpChar | 1);
			}
		}
		else if (ui16WpChar >= Upper_JP_A)
		{

			// Possible double byte character set alphabetic character?

			if (ui16WpChar <= Upper_JP_Z)
			{

				// Japanese?

				ui16WpChar = ui16WpChar - Upper_JP_A + Lower_JP_a;
			}
			else if (ui16WpChar >= Upper_KR_A && ui16WpChar <= Upper_KR_Z)
			{

				// Korean?

				ui16WpChar = ui16WpChar - Upper_KR_A + Lower_KR_a;
			}
			else if (ui16WpChar >= Upper_CS_A && ui16WpChar <= Upper_CS_Z)
			{

				// Chinese Simplified?

				ui16WpChar = ui16WpChar - Upper_CS_A + Lower_CS_a;
			}
			else if (ui16WpChar >= Upper_CT_A && ui16WpChar <= Upper_CT_Z)
			{

				// Chinese Traditional?

				ui16WpChar = ui16WpChar - Upper_CT_A + Lower_CT_a;
			}
		}
	}

	// Return original character, original not in upper case

	return (ui16WpChar);
}

/****************************************************************************
Desc: Break a WP character into a base and a diacritical char. 
Ret: 	TRUE - if not found FALSE - if found
****************************************************************************/
FLMBOOL flmCh6Brkcar(
	FLMUINT16		ui16WpChar,
	FLMUINT16 *		pui16BaseChar,
	FLMUINT16 *		pui16DiacriticChar)
{
	BASE_DIACRIT *	pBaseDiacritic;
	FLMINT			iTableIndex;

	if ((pBaseDiacritic = flm_car60_c[HI( ui16WpChar)]) == 0)
	{
		return (TRUE);
	}

	iTableIndex = ((FLMBYTE) ui16WpChar) - pBaseDiacritic->start_char;
	
	if (iTableIndex < 0 ||
		 iTableIndex > pBaseDiacritic->char_count ||
		 pBaseDiacritic->table[iTableIndex].base == (FLMBYTE) 0xFF)
	{
		return (TRUE);
	}

	if ((HI( ui16WpChar) != CHSMUL1) ||
		 ((flm_ml1_cb60[((FLMBYTE) ui16WpChar) >> 3] >> 
											(7 - (ui16WpChar & 0x07))) & 0x01))
	{

		// normal case, same base as same as characters

		*pui16BaseChar = (ui16WpChar & 0xFF00) |
							  pBaseDiacritic->table[iTableIndex].base;
							  
		*pui16DiacriticChar = (ui16WpChar & 0xFF00) |
									 pBaseDiacritic->table[iTableIndex].diacrit;
	}
	else
	{

		// Multi-national where base is ascii value.

		*pui16BaseChar = pBaseDiacritic->table[iTableIndex].base;
		*pui16DiacriticChar = (ui16WpChar & 0xFF00) | 
									 pBaseDiacritic->table[iTableIndex].diacrit;
	}

	return (FALSE);
}

/****************************************************************************
Desc:		Take a base and a diacritic and compose a WP character.
Ret:		TRUE - if not found FALSE - if found 
****************************************************************************/
FLMBOOL flmCh6Cmbcar(
	FLMUINT16 *		pui16WpChar,
	FLMUINT16		ui16BaseChar,
	FLMINT16			ui16DiacriticChar)
{
	FLMUINT					uiRemaining;
	FLMBYTE					ucCharSet;
	FLMBYTE					ucChar;
	BASE_DIACRIT *			pBaseDiacritic;
	BASE_DIACRIT_TABLE *	pTable;

	ucCharSet = HI( ui16BaseChar);
	if (ucCharSet > NCHSETS)
	{
		return (TRUE);
	}

	// Is base ASCII? If so, look in multinational 1

	if (!ucCharSet)
	{
		ucCharSet = CHSMUL1;
	}

	if ((pBaseDiacritic = flm_car60_c[ucCharSet]) == 0)
	{
		return (TRUE);
	}

	ucChar = LO( ui16BaseChar);
	ui16DiacriticChar = LO( ui16DiacriticChar);
	pTable = pBaseDiacritic->table;
	for (uiRemaining = pBaseDiacritic->char_count;
		  uiRemaining;
		  uiRemaining--, pTable++)
	{

		// Same base?

		if (pTable->base == ucChar &&
			 (pTable->diacrit & 0x7F) == ui16DiacriticChar)
		{

			// Same diacritic?

			*pui16WpChar = (FLMUINT16) (((FLMUINT16) ucCharSet << 8) + 
									(pBaseDiacritic->start_char + 
										(FLMUINT16) (pTable - pBaseDiacritic->table)));
			return (FALSE);
		}
	}

	return (TRUE);
}

/****************************************************************************
Desc:
****************************************************************************/
FINLINE FLMUINT flmCharTypeAnsi7(
	FLMUINT16	ui16Char)
{
	if ((ui16Char >= ASCII_LOWER_A && ui16Char <= ASCII_LOWER_Z) ||
		 (ui16Char >= ASCII_UPPER_A && ui16Char <= ASCII_UPPER_Z) ||
		 (ui16Char >= ASCII_ZERO && ui16Char <= ASCII_NINE))
	{
		return (SDWD_CHR);
	}

	if (ui16Char == 0x27)
	{
		return (WDJN_CHR);
	}

	if (ui16Char <= 0x2B)
	{
		return (DELI_CHR);
	}

	if (ui16Char == ASCII_COMMA ||
		 ui16Char == ASCII_DASH ||
		 ui16Char == ASCII_DOT ||
		 ui16Char == ASCII_SLASH ||
		 ui16Char == ASCII_COLON ||
		 ui16Char == ASCII_AT ||
		 ui16Char == ASCII_BACKSLASH ||
		 ui16Char == ASCII_UNDERSCORE)
	{
		return (WDJN_CHR);
	}

	return (DELI_CHR);
}

/****************************************************************************
Desc: Return the next WP or unicode character value and parsing type.
****************************************************************************/
FLMUINT flmTextGetCharType(
	const FLMBYTE *		pText,
	FLMUINT					uiLen,
	FLMUINT16 *				pui16WPValue,
	FLMUNICODE *			puzUniValue,
	FLMUINT *				pType)
{
	FLMUINT			uiReturnLen;
	FLMUINT16		wpValue;
	FLMUNICODE		uniValue;
	FLMUINT			uiCharSet;

	uiReturnLen = flmTextGetValue( pText, uiLen, NULL, FLM_COMP_COMPRESS_WHITESPACE,
											pui16WPValue, puzUniValue);
	wpValue = *pui16WPValue;
	uniValue = *puzUniValue;

	if (wpValue)
	{
		if (wpValue < 0x080)
		{
			*pType = flmCharTypeAnsi7( wpValue);
			goto Exit;
		}

		uiCharSet = (FLMUINT) (wpValue >> 8);

		if (uiCharSet == 1 ||
			 uiCharSet == 2 ||
			 (uiCharSet >= 8 && uiCharSet <= 11))
		{
			*pType = SDWD_CHR;
			goto Exit;
		}

		*pType = DELI_CHR;
	}
	else
	{

		// For now all unicode is a delimeter

		*pType = DELI_CHR;
	}

Exit:

	return (uiReturnLen);
}

/****************************************************************************
Desc:		Return the next WP or unicode character value
Return: 	Number of bytes formatted to return the character value
****************************************************************************/
FLMUINT flmTextGetValue(
	const FLMBYTE *	pText,			// [in] Points to current value.
	FLMUINT				uiLen,			// [in] Bytes remaining in text.
	FLMUINT *			puiWpChar2,		// Was there a double character?
	FLMUINT				uiFlags,			// [in]
	FLMUINT16 *			pui16WPValue,	// [out] WP Character value or 0 if unicode.
	FLMUNICODE *		puzUniValue)	// [out] Unicode or OEM value if *pui16WPChar is zero.
{
	FLMUINT			uiReturnLength = 0;
	FLMUINT			uiObjectLength;
	FLMUINT16		ui16CurValue;
	FLMUNICODE		uzUniValue;

	uiReturnLength = 0;
	ui16CurValue = 0;
	uzUniValue = 0;

	if (puiWpChar2 && *puiWpChar2)
	{
		ui16CurValue = (FLMUINT16) (*puiWpChar2);
		*puiWpChar2 = 0;
		uiObjectLength = 0;
		goto Check_White_Space;
	}

	while (uiLen && !ui16CurValue && !uzUniValue)
	{
		ui16CurValue = (FLMUINT16) * pText;

		switch (flmTextObjType( ui16CurValue))
		{
			case ASCII_CHAR_CODE:				// 0nnnnnnn
			{
				uiObjectLength = 1;

Check_White_Space:

				// Do all of the bIgnore* stuff here. WHITE SPACE CODE doesn't
				// apply.

				if (ui16CurValue == (FLMUINT16) ASCII_UNDERSCORE &&
					 (uiFlags & FLM_COMP_NO_UNDERSCORES))
				{
					ui16CurValue = (FLMUINT16) ASCII_SPACE;
				}

				if (ui16CurValue == (FLMUINT16) ASCII_SPACE)
				{
					if (uiFlags & FLM_COMP_NO_WHITESPACE)
					{
						ui16CurValue = 0;
					}
					else if (uiFlags & FLM_COMP_COMPRESS_WHITESPACE)
					{

						// Eat up the remaining spaces and underscores (if
						// FLM_COMP_NO_UNDERSCORES).

						while ((pText[uiObjectLength] == ASCII_SPACE || 
								 (pText[uiObjectLength] == ASCII_UNDERSCORE && 
									(uiFlags & FLM_COMP_NO_UNDERSCORES))) && 
								uiObjectLength < uiLen)
						{
							uiObjectLength++;
						}
					}
				}
				else if (ui16CurValue == ASCII_DASH && (uiFlags & FLM_COMP_NO_DASHES))
				{
					ui16CurValue = 0;
				}
				
				break;
			}
			
			case CHAR_SET_CODE:					// 10nnnnnn - Character Set | Char
			{
				uiObjectLength = 2;
				ui16CurValue = (FLMUINT16) (((FLMUINT16) (ui16CurValue & 
										(~CHAR_SET_MASK)) << 8) + 
											(FLMUINT16) *(pText + 1));
					
				break;
			}
			
			case WHITE_SPACE_CODE:				// 110nnnnn
			{
				FLMBYTE	ucTmpByte;

				uiObjectLength = 1;
				ucTmpByte = *pText & (~WHITE_SPACE_MASK);

				ui16CurValue = ((ucTmpByte == HARD_HYPHEN) || 
									 (ucTmpByte == HARD_HYPHEN_EOL) ||
									 (ucTmpByte == HARD_HYPHEN_EOP)) 
									 	? (FLMUINT16) 0x2D 
										: (FLMUINT16) 0x20;
				break;
			}

			case EXT_CHAR_CODE:					// Full extended character
			{
				uiObjectLength = 3;
				ui16CurValue = (FLMUINT16) (((FLMUINT16) *(pText + 1) << 8) + 
													  (FLMUINT16) *(pText + 2));
				break;
			}
			
			case UNICODE_CODE:		// Unconvertable UNICODE code
			{
				uiObjectLength = 3;
				ui16CurValue = 0;
				uzUniValue = (FLMUINT16) (((FLMUINT16) *(pText + 1) << 8) + 
													(FLMUINT16) *(pText + 2));
				break;
			}
			
			case OEM_CODE:
			{
				uiObjectLength = 2;	// OEM characters are always >= 128

				// Make this a unicode character

				ui16CurValue = 0;
				uzUniValue = (FLMUINT16) * (pText + 1);
				break;
			}

			// Skip all of the unknown stuff

			case UNK_GT_255_CODE:
			{
				uiObjectLength = (FLMUINT16) (1 + sizeof(FLMUINT16) + 
															FB2UW( pText + 1));
				break;
			}
			
			case UNK_LE_255_CODE:
			{
				uiObjectLength = 2 + (FLMUINT16) * (pText + 1);
				break;
			}
			
			case UNK_EQ_1_CODE:
			{
				uiObjectLength = 2;
				break;
			}
			
			default:
			{
				// Coded to skip remaining data

				ui16CurValue = 0;
				uiObjectLength = uiLen;
				break;
			}
		}

		uiReturnLength += uiObjectLength;
		pText += uiObjectLength;
		uiLen -= uiObjectLength;
	}

	*pui16WPValue = ui16CurValue;
	*puzUniValue = uzUniValue;
	
	return (uiReturnLength);
}

/****************************************************************************
Desc:	Returns the size of buffer needed to hold the unicode string in
		FLAIM's storage format.
****************************************************************************/
FLMEXP FLMUINT FLMAPI FlmGetUnicodeStorageLength(
	const FLMUNICODE *		puzStr)
{
	FLMBYTE		chrSet;
	FLMUINT		uiStorageLength = 0;
	FLMUINT		uniLength;
	FLMUINT16	wp60Buf[12];

	flmAssert( puzStr != NULL);

	// Two passes are needed to store a UNICODE string: 1st pass
	// determines the storage length (via FlmGetUnicodeStorageLength) 2nd
	// pass stores the string into FLAIMs internal text format (via
	// FlmUnicode2Storage).

	do
	{
		if (*puzStr < 0x20)
		{
			uniLength = 1;
			uiStorageLength += 3;
		}
		else
		{
			if (*puzStr < 0x7F)
			{
				uiStorageLength++;
				puzStr++;
				continue;
			}

			uniLength = flmUnicodeToWP( puzStr, wp60Buf);

			if (!uniLength)
			{
				uiStorageLength += 3;
				uniLength = 1;
			}
			else
			{
				if ((chrSet = (FLMBYTE) (wp60Buf[0] >> 8)) == 0)
				{
					uiStorageLength++;
				}
				else
				{
					uiStorageLength += (chrSet <= 63) ? 2 : 3;
				}
			}
		}

		puzStr += uniLength;
	} while (*puzStr != 0);

	return (uiStorageLength);
}

/****************************************************************************
Desc:	Copies and formats a Unicode string into FLAIM's storage format.
		The Unicode string must be in little endian format.
		Unicode values that are not represented as WordPerfect 6.x 
		characters are preserved as non-WP characters.
****************************************************************************/
FLMEXP RCODE FLMAPI FlmUnicode2Storage(
	const FLMUNICODE *		puzStr,
	FLMUINT *					puiBufLength,
	FLMBYTE *					pBuf)
{
	FLMBYTE		chrSet;
	FLMUINT16	wp60Buf[12];
	FLMUINT		uiStorageLength = 0;
	FLMUINT		uniLength;

	flmAssert( puzStr != NULL);
	flmAssert( pBuf != NULL);

	do
	{
		if (*puzStr < 0x20)
		{

			// Output the character as an unconvertable unicode character.

			*pBuf++ = UNICODE_CODE;
			*pBuf++ = *puzStr >> 8;
			*pBuf++ = (FLMBYTE) *puzStr;
			uniLength = 1;
			uiStorageLength += 3;
		}
		else
		{
			if (*puzStr < 0x7F)
			{
				uiStorageLength++;
				*pBuf++ = (FLMBYTE) *puzStr++;
				continue;
			}

			uniLength = flmUnicodeToWP( puzStr, wp60Buf);

			if (!uniLength)
			{
				*pBuf++ = UNICODE_CODE;
				*pBuf++ = *puzStr >> 8;
				*pBuf++ = (FLMBYTE) *puzStr;
				uniLength = 1;
				uiStorageLength += 3;
			}
			else
			{
				chrSet = wp60Buf[0] >> 8;

				if (chrSet == 0)
				{
					*pBuf++ = (FLMBYTE) wp60Buf[0];
					uiStorageLength++;
				}
				else if (chrSet <= 63)
				{
					*pBuf++ = CHAR_SET_CODE | chrSet;
					*pBuf++ = (FLMBYTE) wp60Buf[0];
					uiStorageLength += 2;
				}
				else
				{
					*pBuf++ = EXT_CHAR_CODE;
					*pBuf++ = chrSet;
					*pBuf++ = (FLMBYTE) wp60Buf[0];
					uiStorageLength += 3;
				}
			}
		}

		puzStr += uniLength;

		// Make sure input buffer was large enough

		if (*puiBufLength < uiStorageLength)
		{
			return (RC_SET( FERR_CONV_DEST_OVERFLOW));
		}
	} while (*puzStr != 0);

	*puiBufLength = uiStorageLength;
	return (FERR_OK);
}

/****************************************************************************
Desc:	Convert from Unicode to 1 and only 1 WP60 character 
Ret:	Conversion count - 0 means Unicode character could not be converted
****************************************************************************/
FLMUINT flmUnicodeToWP(
	const FLMUNICODE *	pUniStr,
	FLMUINT16 *				pWPChr)
{
	FLMUINT			uiReturnLen = 1;
	FLMUNICODE		uzUniChar = *pUniStr;
	FLMINT16			max;
	FLMINT16			min;
	FLMINT16			temp;
	FLMUINT16 *		tablePtr;
	FLMUINT16		tblChr;

	if (uzUniChar < 127)
	{
		*pWPChr = uzUniChar;
		goto Exit;
	}

	tablePtr = (FLMUINT16 *) WP_UTOWP60;

	// Value we should use ... max = UTOWP60_ENTRIES - 1;
	// Bug introduced before Nov99 where UTOWP60_ENTRIES is actually 1502
	// and the value of 2042 was used. Through debugging, all values in the
	// table from 1021 to 1502 were never converted to WP character. So, in
	// order to search correctly on these values we must preserve the WRONG
	// conversion of these characters (Unicode x222E on). The new max table
	// size is 1021 so max will be set to 1020 to work correctly.

	max = 1020;
	min = 0;

	do
	{
		temp = (min + max) >> 1;
		tblChr = *(tablePtr + (temp * 2));
		
		if (tblChr < uzUniChar)
		{
			min = temp + 1;
		}
		else if (tblChr > uzUniChar)
		{
			max = temp - 1;
		}
		else
		{
			*pWPChr = *(tablePtr + (temp * 2) + 1);
			goto Exit;
		}
	} while (min <= max);

	uiReturnLen = 0;

Exit:

	return (uiReturnLen);
}

/****************************************************************************
Desc:	Converts storage formats to UNICODE.
****************************************************************************/
FLMEXP RCODE FLMAPI FlmStorage2Unicode(
	FLMUINT				uiValueType,
	FLMUINT				uiValueLength,
	const FLMBYTE *	pucValue,
	FLMUINT *			puiStrBufLen,
	FLMUNICODE *		puzStrBuf)
{
	FLMUNICODE *	tablePtr;
	FLMBYTE			c;
	FLMUINT			bytesProcessed = 0;
	FLMUINT			bytesOutput = 0;
	FLMUINT			outputData;
	FLMUINT			maxOutLen;
	FLMBYTE			objType;
	FLMUINT			objLength = 0;
	FLMBYTE			tempBuf[80];
	FLMBYTE			chrSet;
	FLMBYTE			chrVal;
	FLMUNICODE		newChrVal;
	RCODE				rc = FERR_OK;

	// If the value is a number, convert to text first

	if (uiValueType != FLM_TEXT_TYPE)
	{
		if (pucValue == NULL)
		{
			uiValueLength = 0;
		}
		else
		{
			if (uiValueType == FLM_NUMBER_TYPE)
			{
				uiValueLength = sizeof(tempBuf);
				rc = GedNumToText( pucValue, tempBuf, &uiValueLength);
			}
			else
			{
				rc = RC_SET( FERR_CONV_ILLEGAL);
				goto Exit;
			}

			if (RC_BAD( rc))
			{
				goto Exit;
			}

			pucValue = &tempBuf[0];
		}
	}

	maxOutLen = *puiStrBufLen;
	outputData = ((puzStrBuf != NULL) && (maxOutLen > 1));

	if (outputData)
	{
		maxOutLen -= 2;
	}

	// Parse through the string outputting data to the buffer as we go

	while (bytesProcessed < uiValueLength)
	{

		// Determine what we are pointing at

		c = *pucValue;
		objType = flmTextObjType( c);
		
		switch (objType)
		{
			case ASCII_CHAR_CODE:
			{
				objLength = 1;
				if (outputData)
				{
					if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto GedGetUNICODE_Output;
					}

					*puzStrBuf++ = c;
				}

				bytesOutput += 2;
				break;
			}
			
			case CHAR_SET_CODE:
			{
				objLength = 2;
				if (outputData)
				{
					if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto GedGetUNICODE_Output;
					}

					// Convert WP to UNICODE

					chrSet = c & 0x3F;
					chrVal = *(pucValue + 1);

					goto ConvertWPToUni;
				}

				bytesOutput += 2;
				break;
			}
			
			case WHITE_SPACE_CODE:
			{
				objLength = 1;

				if (outputData)
				{
					if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto GedGetUNICODE_Output;
					}

					if (c == (WHITE_SPACE_CODE | NATIVE_TAB))
					{
						*puzStrBuf = (FLMUNICODE) 9;
					}
					else if (c == (WHITE_SPACE_CODE | NATIVE_LINEFEED))
					{
						*puzStrBuf = (FLMUNICODE) 10;
					}
					else if (c == (WHITE_SPACE_CODE | HARD_RETURN))
					{
						*puzStrBuf = (FLMUNICODE) 13;
					}
					else
					{
						*puzStrBuf = (FLMUNICODE) 0x20;
					}

					puzStrBuf++;
				}

				bytesOutput += 2;
				break;
			}
			
			case EXT_CHAR_CODE:
			{
				objLength = 3;
				if (outputData)
				{
					if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto GedGetUNICODE_Output;
					}

					// Convert back from WP to UNICODE

					chrSet = *(pucValue + 1);
					chrVal = *(pucValue + 2);

ConvertWPToUni:

					if ((chrSet < WP60toUni_MAX) &&
						 ((tablePtr = WP60toUni[chrSet]) != 0))
					{
						FLMUNICODE *		pCpxUniStr;

						newChrVal = tablePtr[chrVal];

						if ((newChrVal & WPCH_LOMASK) == 0xF000)
						{

							// Does character convert to many Unicode chars?

							pCpxUniStr = WP60toCpxUni[chrSet];
							pCpxUniStr += (newChrVal & WPCH_HIMASK) * WPCH_MAX_COMPLEX;

							while (*pCpxUniStr)
							{
								if (outputData)
								{
									if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
									{
										rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
										goto GedGetUNICODE_Output;
									}

									*puzStrBuf++ = *pCpxUniStr++;
								}

								bytesOutput += 2;
							}
						}
						else
						{
							if (outputData)
							{
								if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
								{
									rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
									goto GedGetUNICODE_Output;
								}

								*puzStrBuf++ = newChrVal;
							}

							bytesOutput += 2;
						}
					}
					else
					{

						// Big extended WP char

						if (outputData)
						{
							if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
							{
								rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
								goto GedGetUNICODE_Output;
							}

							*puzStrBuf++ = 0x03;
						}

						bytesOutput += 2;
					}
				}
				break;
			}
			
			case OEM_CODE:
			{

				// We always just skip OEM codes

				objLength = 2;
				break;
			}
			
			case UNICODE_CODE:
			{
				objLength = 3;
				if (outputData)
				{
					if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto GedGetUNICODE_Output;
					}

					*puzStrBuf++ = (*(pucValue + 1) << 8) + *(pucValue + 2);
				}

				bytesOutput += 2;
				break;
			}
			
			case UNK_EQ_1_CODE:
			{
				objLength = 2;
				if (outputData)
				{
					if ((maxOutLen < 2) || (bytesOutput > maxOutLen - 2))
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto GedGetUNICODE_Output;
					}

					*puzStrBuf++ = *(pucValue + 1);
				}

				bytesOutput += 2;
				break;
			}
			
			default:
			{
				flmAssert( 0);
				bytesProcessed = uiValueLength;
				break;
			}
		}

		pucValue += objLength;
		bytesProcessed += objLength;
	}

	// Add TWO terminating NULL characters, but DO NOT increment the
	// bytesOutput counter.

GedGetUNICODE_Output:

	if (outputData)
	{
		*puzStrBuf = 0;
	}

	*puiStrBufLen = bytesOutput;

Exit:

	return (rc);
}

/****************************************************************************
Desc:	Returns the size of buffer needed to hold the native string in
		FLAIM's storage format.
****************************************************************************/
FLMEXP FLMUINT FLMAPI FlmGetNativeStorageLength(
	const char *		pszStr)
{
	RCODE			rc;
	FLMUINT		uiStorageLength;

	rc = FlmNative2Storage( pszStr, 0, &uiStorageLength, NULL);
	flmAssert( rc == FERR_OK);

	return (uiStorageLength);
}

/****************************************************************************
Desc:	Copies and formats a native 8-bit null terminated string into a
		caller supplied buffer, It converts the string into an internal FLAIM
		TEXT string.
****************************************************************************/
FLMEXP RCODE FLMAPI FlmNative2Storage(
	const char *	pszNativeString,
	FLMUINT			uiStringLength,
	FLMUINT *		puiStorageLen,
	FLMBYTE *		pStorageBuffer)
{
	FLMBOOL			bGetLengthPass;
	const char *	pCurChar;
	const char *	pEnd = NULL;
	FLMBYTE			ucAsciiChar;
	FLMUINT			uiStorageLen = 0;

	// Are we determining the needed length or converting the data

	bGetLengthPass = pStorageBuffer ? FALSE : TRUE;

	if (uiStringLength)
	{
		pEnd = &pszNativeString[uiStringLength];
	}

	// Parse through the string

	pCurChar = pszNativeString;

	for (;;)
	{
		if (*pCurChar == 0 || (pEnd && pCurChar >= pEnd))
		{

			// We have reached end of the string, return the storage length

			*puiStorageLen = uiStorageLen;
			return (FERR_OK);
		}

		// Put the character in a local variable for speed

		ucAsciiChar = f_toascii( *pCurChar);

		if (ucAsciiChar < ASCII_SPACE)
		{

			// If it is a tab, carriage return, or linefeed, output a
			// whitespace code for indexing and each word purposes

			if (ucAsciiChar == ASCII_TAB)
			{
				if (bGetLengthPass)
				{
					uiStorageLen++;
				}
				else
				{
					*pStorageBuffer++ = WHITE_SPACE_CODE | NATIVE_TAB;
				}
			}
			else if (ucAsciiChar == ASCII_NEWLINE)
			{
				if (bGetLengthPass)
				{
					uiStorageLen++;
				}
				else
				{
					*pStorageBuffer++ = WHITE_SPACE_CODE | NATIVE_LINEFEED;
				}
			}
			else if (ucAsciiChar == ASCII_CR)
			{
				if (bGetLengthPass)
				{
					uiStorageLen++;
				}
				else
				{
					*pStorageBuffer++ = WHITE_SPACE_CODE | HARD_RETURN;
				}
			}
			else
			{
				if (bGetLengthPass)
				{
					uiStorageLen += 2;
				}
				else
				{

					// Output the character as an unknown byte if no WP char
					// found

					*pStorageBuffer++ = UNK_EQ_1_CODE | NATIVE_TYPE;
					*pStorageBuffer++ = ucAsciiChar;
				}
			}
		}
		else if (ucAsciiChar < 127)
		{

			// For now assume < 127 means character set zero. Value 127 is
			// very sacred in WP land and is really an extended character

			if (bGetLengthPass)
			{
				uiStorageLen++;
			}
			else
			{
				*pStorageBuffer++ = ucAsciiChar;
			}
		}
		else
		{
			if (bGetLengthPass)
			{
				uiStorageLen += 2;
			}
			else
			{
				*pStorageBuffer++ = OEM_CODE;
				*pStorageBuffer++ = ucAsciiChar;
			}
		}

		pCurChar++;
	}
}

/****************************************************************************
Desc:	Convert a storage text string into a native string
****************************************************************************/
FLMEXP RCODE FLMAPI FlmStorage2Native(
	FLMUINT				uiValueType,
	FLMUINT				uiValueLength,
	const FLMBYTE *	pucValue,
	FLMUINT *			puiOutBufLenRV,
	char *				pOutBuffer)
{
	RCODE					rc = FERR_OK;
	const FLMBYTE *	ptr = pucValue;
	char *				outPtr;
	FLMBYTE				c;
	FLMUINT				bytesProcessed;
	FLMUINT				bytesOutput;
	FLMUINT				valLength = uiValueLength;
	FLMUINT				outputData;
	FLMUINT				maxOutLen = 0;
	FLMBYTE				objType;
	FLMUINT				objLength = 0;
	FLMBYTE				TempBuf[80];
	FLMUINT				length;

	// If the input is not a TEXT or a NUMBER node, return an error for now

	if ((uiValueType == FLM_BINARY_TYPE) || (uiValueType == FLM_CONTEXT_TYPE))
	{
		rc = RC_SET( FERR_CONV_ILLEGAL);
		goto Exit;
	}

	// If the node is a number, convert to text first

	if (uiValueType != FLM_TEXT_TYPE)
	{
		if (ptr == NULL)
		{
			valLength = 0;
		}
		else
		{
			valLength = sizeof(TempBuf);
			flmAssert( uiValueType == FLM_NUMBER_TYPE);
			if (RC_BAD( rc = GedNumToText( ptr, TempBuf, &valLength)))
			{
				goto Exit;
			}

			ptr = &TempBuf[0];
		}
	}

	outputData = ((pOutBuffer != NULL) && (*puiOutBufLenRV));
	if (outputData)
	{
		maxOutLen = *puiOutBufLenRV - 1;
	}

	bytesProcessed = 0;
	bytesOutput = 0;
	outPtr = pOutBuffer;

	while (bytesProcessed < valLength)
	{
		c = *ptr;
		objType = (FLMBYTE) flmTextObjType( c);

		switch (objType)
		{
			case ASCII_CHAR_CODE:
			{
				objLength = 1;
				if (outputData)
				{
					if (bytesOutput < maxOutLen)
					{
						*outPtr++ = f_tonative( c);
					}
					else
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto Native_Output;
					}
				}

				bytesOutput++;
				break;
			}
			
			case CHAR_SET_CODE:
			{
				objLength = 2;
				if (outputData)
				{
					if (bytesOutput < maxOutLen)
					{
						if ((c & (~objType)) == 0)
						{
							*outPtr++ = f_tonative( *(ptr + 1));
						}
						else
						{
							*outPtr++ = NON_DISPLAYABLE_CHAR;
						}
					}
					else
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto Native_Output;
					}
				}

				bytesOutput++;
				break;
			}
			
			case WHITE_SPACE_CODE:
			{
				objLength = 1;

				// ALWAYS OUTPUT A SPACE WHEN WE SEE A WHITE_SPACE_CODE ;
				// UNLESS IT IS A HYPHEN
				
				if (outputData)
				{
					if (bytesOutput < maxOutLen)
					{
						c &= (~WHITE_SPACE_MASK);
						if ((c == HARD_HYPHEN) ||
							 (c == HARD_HYPHEN_EOL) ||
							 (c == HARD_HYPHEN_EOP))
						{
							c = ASCII_DASH;
						}
						else if (c == NATIVE_TAB)
						{
							c = ASCII_TAB;
						}
						else if (c == NATIVE_LINEFEED)
						{
							c = ASCII_NEWLINE;
						}
						else if (c == HARD_RETURN)
						{
							c = ASCII_CR;
						}
						else
						{
							c = ASCII_SPACE;
						}

						*outPtr++ = f_tonative( c);
					}
					else
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto Native_Output;
					}
				}

				bytesOutput++;
				break;
			}
			
			case UNK_GT_255_CODE:
			case UNK_LE_255_CODE:
			{
				if (objType == UNK_GT_255_CODE)
				{
					length = FB2UW( ptr + 1);
					objLength = (FLMUINT16) (1 + sizeof(FLMUINT16) + length);
				}
				else
				{
					length = (FLMUINT16) * (ptr + 1);
					objLength = (FLMUINT16) (2 + length);
				}

				// Skip it if it is not a NATIVE code

				if ((c & (~objType)) == NATIVE_TYPE)
				{
					if (outputData)
					{
						if ((maxOutLen < length) || (bytesOutput > maxOutLen - length))
						{
							rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
							goto Native_Output;
						}

						if (objType == UNK_LE_255_CODE)
						{
							f_memcpy( outPtr, ptr + 2, length);
						}
						else
						{
							f_memcpy( outPtr, ptr + 1 + sizeof(FLMUINT16), length);
						}

						outPtr += length;
					}

					bytesOutput += length;
				}
				
				break;
			}
			
			case UNK_EQ_1_CODE:
			{
				objLength = 2;

				// Skip it if it is not a NATIVE code

				if ((c & (~objType)) == NATIVE_TYPE)
				{
					if (outputData)
					{
						if (bytesOutput < maxOutLen)
						{
							*outPtr++ = f_tonative( *(ptr + 1));
						}
						else
						{
							rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
							goto Native_Output;
						}
					}

					bytesOutput++;
				}
				break;
			}
			
			case EXT_CHAR_CODE:
			{
				objLength = 3;
				if (outputData)
				{
					if (bytesOutput < maxOutLen)
					{
						*outPtr += NON_DISPLAYABLE_CHAR;
					}
					else
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto Native_Output;
					}
				}

				bytesOutput++;
				break;
			}
			
			case OEM_CODE:
			{
				objLength = 2;
				if (outputData)
				{
					if (bytesOutput < maxOutLen)
					{
						*outPtr++ = f_tonative( *(ptr + 1));
					}
					else
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto Native_Output;
					}
				}

				bytesOutput++;
				break;
			}
			
			case UNICODE_CODE:
			{
				objLength = 3;
				if (outputData)
				{
					if (bytesOutput < maxOutLen)
					{
						*outPtr++ = UNICODE_UNCONVERTABLE_CHAR;
					}
					else
					{
						rc = RC_SET( FERR_CONV_DEST_OVERFLOW);
						goto Native_Output;
					}
				}

				bytesOutput++;
				break;
			}
			
			default:
			{
				break;
			}
		}

		ptr += objLength;
		bytesProcessed += objLength;
	}

	// Add a terminating NULL character, but DO NOT increment the ;
	// bytesOutput counter!
	
Native_Output:

	if (outputData)
	{
		*outPtr = 0;
	}

	*puiOutBufLenRV = bytesOutput;

Exit:

	return (rc);
}
