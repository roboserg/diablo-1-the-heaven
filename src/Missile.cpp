#include "stdafx.h"

int MissileIndexList[MissilesMax];// 00697860
int MissileAmount;// 0069D048
int ManaShieldActed;// 0069D04C
int MayBeExplodingMagicFlag;// 0069D62C
char PlayerSpellIndexOfCurNova = 0;// 00B04FFE
char CastingNovaInProgress;// 00B04FFF

char IsSearchActive;// 006EE738
bool BattleTransSlowMissile = 0;

int CircleOffsets[20] = {// 00490068
	0,		//	0
	3, 		//	1
	12, 	//	2
	45, 	//	3
	94, 	//	4
	159, 	//	5
	240, 	//	6
	337, 	//	7
	450, 	//	8
	579, 	//	9
	724, 	//	10
	885, 	//	11
	1062, 	//	12
	1255, 	//	13
	1464, 	//	14
	1689, 	//	15
	1930, 	//	16
	2187, 	//	17
	2460, 	//	18
	0		//	19
};

char CircleAndWallSegmentDeltas[2752] = {// 004a6950
	1,	0,	0,
	4,	0,	1,	0,	-1,	-1,	0,	1,	0,
	16,	0,	2,	0,	-2,	-1,	2,	1,	2,	-1,	-2,	1,	-2,	-1,	1,	1,	1,	-1,	-1,	1,	-1,	-2,	1,	2,	1,	-2,	-1,	2, -1, -2, 0, 2, 0,	
	24,	0,	3,	0,	-3,	-1,	3,	1,	3,	-1,	-3,	1,	-3,	-2,	3,	2,	3,	-2,	-3,	2,	-3,	-2,	2,	2,	2,	-2,	-2,	2,	-2,	-3,	2,	3,	2,	-3,	-2,	3,	-2,	-3,	1,	3,	1,	-3,	-1,	3,	-1,	-3,	0,	3,	0,	
	32,	0,	4,	0,	-4,	-1,	4,	1,	4,	-1,	-4,	1,	-4,	-2,	4,	2,	4,	-2,	-4,	2,	-4,	-3,	4,	3,	4,	-3,	-4,	3,	-4,	-3,	3,	3,	3,	-3,	-3,	3,	-3,	-4,	3,	4,	3,	-4,	-3,	4,	-3,	-4,	2,	4,	2,	-4,	-2,	4,	-2,	-4,	1,	4,	1,	-4,	-1,	4,	-1,	-4,	0,	4,	0,	
	40,	0,	5,	0,	-5,	-1,	5,	1,	5,	-1,	-5,	1,	-5,	-2,	5,	2,	5,	-2,	-5,	2,	-5,	-3,	5,	3,	5,	-3,	-5,	3,	-5,	-4,	5,	4,	5,	-4,	-5,	4,	-5,	-4,	4,	4,	4,	-4,	-4,	4,	-4,	-5,	4,	5,	4,	-5,	-4,	5,	-4,	-5,	3,	5,	3,	-5,	-3,	5,	-3,	-5,	2,	5,	2,	-5,	-2,	5,	-2,	-5,	1,	5,	1,	-5,	-1,	5,	-1,	-5,	0,	5,	0,
	48,	0,	6,	0,	-6,	-1,	6,	1,	6,	-1,	-6,	1,	-6,	-2,	6,	2,	6,	-2,	-6,	2, -6,	-3,	6,	3,	6,	-3,	-6,	3,	-6,	-4,	6,	4,	6,	-4,	-6,	4,	-6,	-5,	6,	5,	6,	-5,	-6,	5,	-6,	-5,	5,	5,	5,	-5,	-5,	5,-5,	-6,	5,	6,	5,	-6,	-5,	6, -5,	-6,	4,	6,	4,	-6,	-4,	6,	-4,	-6,	3,	6,	3,	-6,	-3,	6,	-3,	-6,	2,	6, 2,	-6,	-2,	6,	-2,	-6,	1,	6,	1,	-6,	-1,	6,	-1,	-6,	0,	6,	0,	
	56,	0,	7,	0,	-7,	-1,	7,	1,	7,	-1,	-7,	1,	-7,	-2,	7,	2,	7,	-2,	-7,	2,	-7,	-3,	7,	3,	7,	-3,	-7,	3,	-7,	-4,	7,	4,	7,	-4,	-7,	4,	-7,	-5,	7,	5,	7,	-5,	-7,	5,	-7,	-6,	7,	6,	7,	-6,	-7,	6,	-7,	-6,	6,	6,  6,	-6,	-6,	6,	-6,	-7,	6,	7,	6,	-7,	-6,	7,	-6,	-7,	5,	7,	5,	-7,	-5,	7,	-5,	-7,	4,	7,	4,	-7,	-4,	7,	-4,	-7,	3,	7,	3,	-7,	-3,	7,	-3,	-7,	2,	7,	2,	-7,	-2,	7,	-2,	-7,	1,	7,	1,	-7,	-1,	7,	-1,	-7,	0,	7,	0,	
	64,	0,	8,	0,	-8,	-1,	8,	1,	8,	-1,	-8,	1,	-8,	-2,	8,	2,	8,	-2,	-8,	2,	-8,	-3,	8,	3,	8,	-3,	-8,	3,	-8,	-4,	8,	4,	8,	-4,	-8,	4,	-8,	-5,	8,	5,	8,	-5,	-8,	5,	-8,	-6,	8,	6,	8,	-6,	-8,	6,	-8,	-7,	8,	7,	8,	-7,	-8,	7, -8,	-7,	7,	7,7,	-7,	-7,	7,	-7,	-8,	7,	8,	7,	-8,	-7,	8,	-7,	-8,	6,	8,	6,	-8,	-6,	8,	-6,	-8,	5,	8,	5,	-8,	-5,	8,	-5,	-8,	4,	8,	4,	-8,	-4,	8,	-4,	-8,	3,	8,	3,	-8,	-3,	8,	-3,	-8,	2,	8,	2,	-8,	-2,	8,	-2,	-8,	1,	8,	1,	-8,	-1,	8,	-1,	-8,	0,	8,	0,	
	72,	0,	9,	0,	-9,	-1,	9,	1,	9,	-1,	-9,	1,	-9,	-2,	9,	2,	9,	-2,	-9,	2,	-9,	-3,	9,	3,	9,	-3,	-9,	3,	-9,	-4,	9,	4,	9,	-4,	-9,	4,	-9,	-5,	9,	5,	9,	-5,	-9,	5,	-9,	-6,	9,	6,	9,	-6,	-9,	6,	-9,	-7,	9,	7,	9,	-7,	-9,	7,	-9,	-8,	9,	8,	9,	-8,	-9,	8,	-9,	-8,	8,	8,	8,	-8,	-8,	8,	-8,	-9,	8,	9,	8,	-9,	-8,	9,	-8,	-9,	7,	9,	7,	-9,	-7,	9,	-7,	-9,	6,	9,	6,	-9,	-6,	9,	-6,	 -9,	5,	9,	5,	-9,	-5,	9,	-5,	-9,	4,	9,	4,	-9,	-4,	9,	-4,	-9,	3,	9,	3,	-9,	-3,	9,	-3,	-9,	2,	9,	2,	-9,	-2,	9,	-2,	-9,	1,	9,	1,	-9,	-1,	9,	-1,	-9,	0,	9,	0,	
	80,	0,	10,	0,	-10,	-1,	10,	1,	10,	-1,	-10,	1,	-10,	-2,	10,	2,	10,	-2,	-10,	2,	-10,	-3,	10,	3,	10,	-3,	-10,	3,	-10,	-4,	10,	4,	10,	-4,	-10,	4,		-10,	-5,	10,	5,	10,	-5,	-10,	5,	-10,	-6,	10,	6,	10,	-6,	-10,	6,	-10,	-7,	10,	7,	10,	-7,	-10,	7,	-10,	-8,	10,	8,	10,	-8,	-10,	8,	-10,	-9,	10,	9,	10,	-9,	-10,	9,		-10,	-9,	9,	9,	9,	-9,	-9,	9,	-9,	-10,	9,	10,	9,	-10,	-9,	10,	-9,	-10,	8,	10,	8,	-10,	-8,	10,	-8,	-10,	7,	10,	7,	-10,	-7,	10,	-7,	-10,	6,	10,	6,	-10,	-6,	10,		-6,	-10,	5,	10,	5,	-10,	-5,	10,	-5,	-10,	4,	10,	4,	-10,	-4,	10,	-4,	-10,	3,	10,	3,	-10,	-3,	10,	-3,	-10,	2,	10,	2,	-10,	-2,	10,	-2,	-10,	1,	10,	1,	-10,	-1,	10,	-1,	-10,	0,	10,	0,	
	88,	0,	11,	0,	-11,	-1,	11,	1,	11,	-1,	-11,	1,	-11,	-2,	11,	2,	11,	-2,	-11,	2,	-11,	-3,	11,	3,	11,	-3,	-11,	3,	-11,	-4,	11,	4,	11,	-4,	-11,		4,	-11,	-5,	11,	5,	11,	-5,	-11,	5,	-11,	-6,	11,	6,	11,	-6,	-11,	6,	-11,	-7,	11,	7,	11,	-7,	-11,	7,	-11,	-8,	11,	8,	11,	-8,	-11,	8,	-11,	-9,	11,	9,	11,	-9,	-11,		9,	-11,	-10,	11,	10,	11,	-10,	-11,	10,	-11,	-10,	10,	10,	10,	-10,	-10,	10,	-10,	-11,	10,	11,	10,	-11,	-10,	11,	-10,	-11,	9,	11,	9,	-11,	-9,	11,	-9,	-11,	8,	11,	8,	-11,	-8,	11,	-8,	-11,	7,	11,	7,	-11,	-7,	11,	-7,	-11,	6,	11,	6,	-11,	-6,	11,	-6,	-11,	5,	11,	5,	-11,	-5,	11,	-5,	-11,	4,	11,	4,	-11,	-4,	11,	-4,	-11,	3,	11,	3,	-11,	-3,	11,	-3,	-11,	2,	11,	2,	-11,	-2,	11,	-2,	-11,	1,	11,	1,	-11,	-1,	11,	-1,	-11,	0,	11,	0,	
	96,	0,	12,	0,	-12,	-1,	12,	1,	12,	-1,	-12,	1,	-12,	-2,	12,	2,	12,	-2,	-12,	2,	-12,	-3,	12,	3,	12,	-3,	-12,	3,	-12,	-4,	12,	4,	12,	-4,	-12,	4,	-12,	-5,	12,	5,	12,	-5,	-12,	5,	-12,	-6,	12,	6,	12,	-6,	-12,	6,	-12,	-7,	12,	7,	12,	-7,		-12,	7,	-12,	-8,	12,	8,	12,	-8,	-12,	8,	-12,	-9,	12,	9,	12,	-9,	-12,	9,	-12,	-10, 12,	10,	12,	-10,	-12,	10,	-12,	-11,	12,	11,	12,	-11,	-12,	11,	-12,	-11,	11,	11,	11,	-11,		-11,	11,	-11,	-12,	11,	12,	11,	-12,	-11,	12,	-11,	-12,	10,	12,	10,	-12,	-10,	12,	-10,	-12, 9,	12,	9,	-12,	-9,	12,	-9,	-12,	8,	12,	8,	-12,	-8,	12,	-8,	-12,	7,	12,	7,	-12,		-7,	12,	-7,	-12,	6,	12,	6,	-12,	-6,	12,	-6,	-12,	5,	12,	5,	-12,	-5,	12,	-5,	-12, 4,	12,	4,	-12,	-4,	12,	-4,	-12,	3,	12,	3,	-12,	-3,	12,	-3,	-12,	2,	12,	2,	-12,		-2,	12,	-2,	-12,	1,	12,	1,	-12,	-1,	12,	-1,	-12,	0,	12,	0,	
	104,	0,	13,	0,	-13, -1,	13,	1,	13,	-1,	-13,	1,	-13,	-2,	13,	2,	13,	-2,	-13,	2,	-13,	-3,	13,	3,	13,		-3,	-13,	3,	-13,	-4,	13,	4,	13,	-4,	-13,	4,	-13,	-5,	13,	5,	13,	-5,	-13,	5,	-13, -6,	13,	6,	13,	-6,	-13,	6,	-13,	-7,	13,	7,	13,	-7,	-13,	7,	-13,	-8,	13,	8,	13,		-8,	-13,	8,	-13,	-9,	13,	9,	13,	-9,	-13,	9,	-13,	-10,	13,	10,	13,	-10,	-13,	10,	-13, -11,	13,	11,	13,	-11,	-13,	11,	-13,	-12,	13,	12,	13,	-12,	-13,	12,	-13,	-12,	12,	12,	12,		-12,	-12,	12,	-12,	-13,	12,	13,	12,	-13,	-12,	13,	-12,	-13,	11,	13,	11,	-13,	-11,	13,	-11, -13,	10,	13,	10,	-13,	-10,	13,	-10,	-13,	9,	13,	9,	-13,	-9,	13,	-9,	-13,	8,	13,	8,		-13,	-8,	13,	-8,	-13,	7,	13,	7,	-13,	-7,	13,	-7,	-13,	6,	13,	6,	-13,	-6,	13,	-6,	-13,	5,	13,	5,	-13,	-5,	13,	-5,	-13,	4,	13,	4,	-13,	-4,	13,	-4,	-13,	3,	13,	3,		-13,	-3,	13,	-3,	-13,	2,	13,	2,	-13,	-2,	13,	-2,	-13,	1,	13,	1,	-13,	-1,	13,	-1,	-13,	0,	13,	0,	
	112,	0,	14,	0,	-14,	-1,	14,	1,	14,	-1,	-14,	1,	-14,	-2,	14,	2,		14,	-2,	-14,	2,	-14,	-3,	14,	3,	14,	-3,	-14,	3,	-14,	-4,	14,	4,	14,	-4,	-14,	4,	-14,	-5,	14,	5,	14,	-5,	-14,	5,	-14,	-6,	14,	6,	14,	-6,	-14,	6,	-14,	-7,	14,	7,		14,	-7,	-14,	7,	-14,	-8,	14,	8,	14,	-8,	-14,	8,	-14,	-9,	14,	9,	14,	-9,	-14,	9,	-14,	-10,	14,	10,	14,	-10,	-14,	10,	-14,	-11,	14,	11,	14,	-11,	-14,	11,	-14,	-12,	14,	12,		14,	-12,	-14,	12,	-14,	-13,	14,	13,	14,	-13,	-14,	13,	-14,	-13,	13,	13,	13,	-13,	-13,	13,		-13,	-14,	13,	14,	13,	-14,	-13,	14,	-13,	-14,	12,	14,	12,	-14,	-12,	14,	-12,	-14,	11,	14,		11,	-14,	-11,	14,	-11,	-14,	10,	14,	10,	-14,	-10,	14,	-10,	-14,	9,	14,	9,	-14,	-9,	14,		-9,	-14,	8,	14,	8,	-14,	-8,	14,	-8,	-14,	7,	14,	7,	-14,	-7,	14,	-7,	-14,	6,	14,		6,	-14,	-6,	14,	-6,	-14,	5,	14,	5,	-14,	-5,	14,	-5,	-14,	4,	14,	4,	-14,	-4,	14,		-4,	-14,	3,	14,	3,	-14,	-3,	14,	-3,	-14,	2,	14,	2,	-14,	-2,	14,	-2,	-14,	1,	14,		1,	-14,	-1,	14,	-1,	-14,	0,	14,	0,	
	120,	0,	15,	0,	-15,	-1,	15,	1,	15,	-1,	-15,		1,	-15,	-2,	15,	2,	15,	-2,	-15,	2,	-15,	-3,	15,	3,	15,	-3,	-15,	3,	-15,	-4,	15,		4,	15,	-4,	-15,		4,	-15,	-5,	15,	5,	15,	-5,	-15,	5,	-15,	-6,	15,	6,	15,	-6,	-15,		6,	-15,	-7,	15,	7,	15,	-7,	-15,	7,	-15,	-8,	15,	8,	15,	-8,	-15,	8,	-15,		-9,	15,		9,	15,	-9,	-15,	9,	-15,	-10,	15,	10,	15,	-10,	-15,	10,	-15,	-11,	15,	11,	15,	-11,	-15,		11,	-15,	-12,	15,		12,	15,	-12,	-15,	12,	-15,	-13,	15,	13,	15,	-13,	-15,	13,	-15,	-14,	15,		14,	15,	-14,	-15,	14,	-15,	-14,	14,	14,	14,	-14,		-14,	14,	-14,	-15,	14,	15,	14,	-15,	-14,		15,	-14,	-15,	13,	15,	13,	-15,	-13,	15,	-13,	-15,	12,	15,	12,	-15,	-12,	15,	-12,		-15,	11,		15,	11,	-15,	-11,	15,	-11,	-15,	10,	15,	10,	-15,	-10,	15,	-10,	-15,	9,	15,	9,	-15,	-9,		15,	-9,	-15,	8,	15,	8,	-15,		-8,	15,	-8,	-15,	7,	15,	7,	-15,	-7,	15,	-7,	-15,	6,		15,	6,	-15,	-6,	15,	-6,	-15,	5,	15,	5,	-15,	-5,	15,	-5,	-15,	4,	15,	4,	-15,	-4,			15,	-4,	-15,	3,	15,	3,	-15,	-3,	15,	-3,	-15,	2,	15,	2,	-15,	-2,	15,	-2,	-15,	1,		15,	1,	-15,	-1,	15,	-1,	-15,	0,	15,	0,	
	(char)128,	0,	16,		0,	-16,	-1,	16,	1,	16,	-1,		-16,	1,	-16,	-2,	16,	2,	16,	-2,	-16,	2,	-16,	-3,	16,	3,	16,	-3,	-16,	3,	-16,	-4,		16,	4,	16,	-4,	-16,	4,	-16,	-5,	16,	5,	16,	-5,	-16,	5,	-16,	-6,	16,	6,	16,	-6,		-16,	6,	-16,	-7,	16,	7,	16,	-7,	-16,	7,	-16,	-8,	16,	8,	16,	-8,	-16,	8,		-16,	-9,		16,	9,	16,	-9,	-16,	9,	-16,	-10,	16,	10,	16,	-10,	-16,	10,	-16,	-11,	16,	11,	16,	-11,		-16,	11,	-16,	-12,	16,	12,		16,	-12,	-16,	12,	-16,	-13,	16,	13,	16,	-13,	-16,	13,	-16,	-14,		16,	14,	16,	-14,	-16,	14,	-16,	-15,	16,	15,	16,	-15,	-16,		15,	-16,	-15,	15,	15,	15,	-15,		-15,	15,	-15,	-16,	15,	16,	15,	-16,	-15,	16,	-15,	-16,	14,	16,	14,	-16,	-14,	16,	-14,	-16,			13,	16,	13,	-16,	-13,	16,	-13,	-16,	12,	16,	12,	-16,	-12,	16,	-12,	-16,	11,	16,	11,	-16,		-11,	16,	-11,	-16,	10,	16,	10,	-16,		-10,	16,	-10,	-16,	9,	16,	9,	-16,	-9,	16,	-9,	-16,		8,	16,	8,	-16,	-8,	16,	-8,	-16,	7,	16,	7,	-16,	-7,	16,	-7,	-16,	6,	16,	6,	-16,			-6,	16,	-6,	-16,	5,	16,	5,	-16,	-5,	16,	-5,	-16,	4,	16,	4,	-16,	-4,	16,	-4,	-16,		3,	16,	3,	-16,	-3,	16,	-3,	-16,	2,	16,	2,	-16,	-2,	16,	-2,		-16,	1,	16,	1,	-16,		-1,	16,	-1,	-16,	0,	16,	0,	
	(char)136,	0,	17,	0,	-17,	-1,	17,	1,	17,	-1,	-17,	1,	-17,		-2,	17,	2,	17,	-2,	-17,	2,	-17,	-3,	17,	3,	17,	-3,	-17,	3,	-17,		-4,	17,	4,	17,		-4,	-17,	4,	-17,	-5,	17,	5,	17,	-5,	-17,	5,	-17,	-6,	17,	6,	17,	-6,	-17,	6,	-17,		-7,	17,	7,	17,	-7,	-17,		7,	-17,	-8,	17,	8,	17,	-8,	-17,	8,	-17,	-9,	17,	9,	17,		-9,	-17,	9,	-17,	-10,	17,	10,	17,	-10,	-17,	10,	-17,	-11,		17,	11,	17,	-11,	-17,	11,	-17,		-12,	17,	12,	17,	-12,	-17,	12,	-17,	-13,	17,	13,	17,	-13,	-17,	13,	-17,	-14,	17,	14,		17,		-14,	-17,	14,	-17,	-15,	17,	15,	17,	-15,	-17,	15,	-17,	-16,	17,	16,	17,	-16,	-17,	16,	-17,		-16,	16,	16,	16,	-16,		-16,	16,	-16,	-17,	16,	17,	16,	-17,	-16,	17,	-16,	-17,	15,	17,	15,		-17,	-15,	17,	-15,	-17,	14,	17,	14,	-17,	-14,	17,	-14,		-17,	13,	17,	13,	-17,	-13,	17,	-13,		-17,	12,	17,	12,	-17,	-12,	17,	-12,	-17,	11,	17,	11,	-17,	-11,	17,	-11,	-17,	10,	17,	10,			-17,	-10,	17,	-10,	-17,	9,	17,	9,	-17,	-9,	17,	-9,	-17,	8,	17,	8,	-17,	-8,	17,	-8,		-17,	7,	17,	7,	-17,	-7,	17,	-7,	-17,	6,	17,	6,		-17,	-6,	17,	-6,	-17,	5,	17,	5,		-17,	-5,	17,	-5,	-17,	4,	17,	4,	-17,	-4,	17,	-4,	-17,	3,	17,	3,	-17,	-3,	17,	-3,		-17,	2,	17,	2,	-17,		-2,	17,	-2,	-17,	1,	17,	1,	-17,	-1,	17,	-1,	-17,	0,	17,	0,		
	(char)144,	0,	18,	0,	-18,	-1,	18,	1,	18,	-1,	-18,	1,	-18,	-2,	18,	2,	18,	-2,	-18,	2,			-18,	-3,	18,	3,	18,	-3,	-18,	3,	-18,	-4,	18,	4,	18,	-4,	-18,	4,	-18,	-5,	18,	5,		18,	-5,	-18,	5,	-18,	-6,	18,	6,	18,	-6,	-18,	6,	-18,	-7,	18,	7,	18,	-7,	-18,	7,		-18,	-8,	18,	8,	18,	-8,	-18,	8,	-18,	-9,	18,	9,	18,	-9,	-18,	9,	-18,	-10,	18,	10,		18,	-10,	-18,	10,	-18,	-11,		18,	11,	18,	-11,	-18,	11,	-18,	-12,	18,	12,	18,	-12,	-18,	12,		-18,	-13,	18,	13,	18,	-13,	-18,	13,	-18,	-14,	18,	14,	18,	-14,	-18,	14,		-18,	-15,	18,	15,		18,	-15,	-18,	15,	-18,	-16,	18,	16,	18,	-16,	-18,	16,	-18,	-17,	18,	17,	18,	-17,	-18,	17,		-18,	-17,	17,	17,	17,		-17,	-17,	17,	-17,	-18,	17,	18,	17,	-18,	-17,	18,	-17,	-18,	16,	18,		16,	-18,	-16,	18,	-16,	-18,	15,	18,	15,	-18,	-15,	18,	-15,	-18,		14,	18,	14,	-18,	-14,	18,		-14,	-18,	13,	18,	13,	-18,	-13,	18,	-13,	-18,	12,	18,	12,	-18,	-12,	18,	-12,	-18,	11,	18,		11,	-18,	-11,		18,	-11,	-18,	10,	18,	10,	-18,	-10,	18,	-10,	-18,	9,	18,	9,	-18,	-9,	18,		-9,	-18,	8,	18,	8,	-18,	-8,	18,	-8,	-18,	7,	18,	7,	-18,	-7,	18,		-7,	-18,	6,	18,		6,	-18,	-6,	18,	-6,	-18,	5,	18,	5,	-18,	-5,	18,	-5,	-18,	4,	18,	4,	-18,	-4,	18,		-4,	-18,	3,	18,	3,	-18,	-3,	18,	-3,	-18,		2,	18,	2,	-18,	-2,	18,	-2,	-18,	1,	18,		1,	-18,	-1,	18,	-1,	-18,	0,	18,	0,	0,	0,	0,	
};

uchar NovaCircleDeltas [ 23 ][ 30 ] = {// 004A7460
	{1,	0,		2,	0,		3,	0,		4,	0,		5,	0,		6,	0,		7,	0,		8,	0,		9,	0,		10,	0,		11,	0,		12,	0,		13,	0,		14,	0,		15,	0},
	{1,	0,		2,	0,		3,	0,		4,	0,		5,	0,		6,	0,		7,	0,		8,	1,		9,	1,		10,	1,		11,	1,		12,	1,		13,	1,		14,	1,		15,	1},
	{1,	0,		2,	0,		3,	0,		4,	1,		5,	1,		6,	1,		7,	1,		8,	1,		9,	1,		10,	1,		11,	1,		12,	2,		13,	2,		14,	2,		15,	2},
	{1,	0,		2,	0,		3,	1,		4,	1,		5,	1,		6,	1,		7,	1,		8,	2,		9,	2,		10,	2,		11,	2,		12,	2,		13,	3,		14,	3,		15,	3},
	{1,	0,		2,	1,		3,	1,		4,	1,		5,	1,		6,	2,		7,	2,		8,	2,		9,	3,		10,	3,		11,	3,		12,	3,		13,	4,		14,	4,		0,	0},
	{1,	0,		2,	1,		3,	1,		4,	1,		5,	2,		6,	2,		7,	3,		8,	3,		9,	3,		10,	4,		11,	4,		12,	4,		13,	5,		14,	5,		0,	0},
	{1,	0,		2,	1,		3,	1,		4,	2,		5,	2,		6,	3,		7,	3,		8,	3,		9,	4,		10,	4,		11,	5,		12,	5,		13,	6,		14,	6,		0,	0},
	{1,	1,		2,	1,		3,	2,		4,	2,		5,	3,		6,	3,		7,	4,		8,	4,		9,	5,		10,	5,		11,	6,		12,	6,		13,	7,		0,	0,		0,	0},
	{1,	1,		2,	1,		3,	2,		4,	2,		5,	3,		6,	4,		7,	4,		8,	5,		9,	6,		10,	6,		11,	7,		12,	7,		12,	8,		13,	8,		0,	0},
	{1,	1,		2,	2,		3,	2,		4,	3,		5,	4,		6,	5,		7,	5,		8,	6,		9,	7,		10,	7,		10,	8,		11,	8,		12,	9,		0,	0,		0,	0},
	{1,	1,		2,	2,		3,	3,		4,	4,		5,	5,		6,	5,		7,	6,		8,	7,		9,	8,		10,	9,		11,	9,		11,	10,		0,	0,		0,	0,		0,	0},
	{1,	1,		2,	2,		3,	3,		4,	4,		5,	5,		6,	6,		7,	7,		8,	8,		9,	9,		10,	10,		11,	11,		0,	0,		0,	0,		0,	0,		0,	0},
	{1,	1,		2,	2,		3,	3,		4,	4,		5,	5,		5,	6,		6,	7,		7,	8,		8,	9,		9,	10,		9,	11,		10,	11,		0,	0,		0,	0,		0,	0},
	{1,	1,		2,	2,		2,	3,		3,	4,		4,	5,		5,	6,		5,	7,		6,	8,		7,	9,		7,	10,		8,	10,		8,	11,		9,	12,		0,	0,		0,	0},
	{1,	1,		1,	2,		2,	3,		2,	4,		3,	5,		4,	6,		4,	7,		5,	8,		6,	9,		6,	10,		7,	11,		7,	12,		8,	12,		8,	13,		0,	0},
	{1,	1,		1,	2,		2,	3,		2,	4,		3,	5,		3,	6,		4,	7,		4,	8,		5,	9,		5,	10,		6,	11,		6,	12,		7,	13,		0,	0,		0,	0},
	{0,	1,		1,	2,		1,	3,		2,	4,		2,	5,		3,	6,		3,	7,		3,	8,		4,	9,		4,	10,		5,	11,		5,	12,		6,	13,		6,	14,		0,	0},
	{0,	1,		1,	2,		1,	3,		1,	4,		2,	5,		2,	6,		3,	7,		3,	8,		3,	9,		4,	10,		4,	11,		4,	12,		5,	13,		5,	14,		0,	0},
	{0,	1,		1,	2,		1,	3,		1,	4,		1,	5,		2,	6,		2,	7,		2,	8,		3,	9,		3,	10,		3,	11,		3,	12,		4,	13,		4,	14,		0,	0},
	{0,	1,		0,	2,		1,	3,		1,	4,		1,	5,		1,	6,		1,	7,		2,	8,		2,	9,		2,	10,		2,	11,		2,	12,		3,	13,		3,	14,		3,	15},
	{0,	1,		0,	2,		0,	3,		1,	4,		1,	5,		1,	6,		1,	7,		1,	8,		1,	9,		1,	10,		1,	11,		2,	12,		2,	13,		2,	14,		2,	15},
	{0,	1,		0,	2,		0,	3,		0,	4,		0,	5,		0,	6,		0,	7,		1,	8,		1,	9,		1,	10,		1,	11,		1,	12,		1,	13,		1,	14,		1,	15},
	{0,	1,		0,	2,		0,	3,		0,	4,		0,	5,		0,	6,		0,	7,		0,	8,		0,	9,		0,	10,		0,	11,		0,	12,		0,	13,		0,	14,		0,	15},
};

// \*\([_0-9a-zA-z*&() ]+Missiles\[0\]\.(\b(_\w+|[\w-[0-9_]]\w*)\b)[^+)]+\+[^+)]+\)
// spell.$1

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация, Инлайн функция чуть упрощающая рассчеты для смещения фаербластов
__forceinline bool SomeStrangeFireblastExRoomsCheck(int row, int col)
{
	return TransList[TransMap[row][col]] && TileBlockWalking[FineMap[row][col]];
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (th4) -------------------------------------------------------------
int __fastcall SpellElement(Player& player, int spellIndex)
{
	if( player.fullClassId == PFC_ELEMENTALIST && spellIndex == PS_13_HYDRA ){
		return ET_6_COLD;
	}else{
		return Spells[spellIndex].Element;
	}
}

//----- (0042E856) --------------------------------------------------------
int __fastcall GetDamageOfPlayerSpell(int playerIndex, int spellIndex, int spellLevel, int element, int* pMinDamage /*=0*/, int* pMaxDamage /*=0*/ )
{
	int minDamage = 0, maxDamage = 0;
	Player& player = Players[playerIndex];
	switch( spellIndex ){
	case PS_1_FIREBOLT:
	{
		if (GameMode == GM_CLASSIC) {
			minDamage = player.CurMagic / 8 + spellLevel + 1;
			maxDamage = minDamage + 9;
		}
		else {
			minDamage = player.CurMagic / 5 + spellLevel;
			maxDamage = minDamage + 5 + (player.CharLevel / 5);
		}
	}
		break;
	case PS_13_HYDRA:
		// formula taken from from CastHydra: missile.Damage = 4 * (RNG(16) + player.CurMagic / 2 + missile.SpellLevel);
		// minDamage = player.CurMagic / 2 + spellLevel;
		// maxDamage = minDamage + 15;

		// same as firebolt, now using for hydramancer charge bolts also
		if (GameMode == GM_CLASSIC) {
			minDamage = player.CurMagic / 8 + spellLevel + 1;
			maxDamage = minDamage + 9;
		}
		else {
			minDamage = player.CurMagic / 5 + spellLevel;
			maxDamage = minDamage + 5 + (player.CharLevel / 5);
		}
		
		// element adjusting example, uncomment if need:
		//switch(element){
		//case ET_4_ACID:
		//case ET_6_COLD: minDamage *= 4; maxDamage *= 4; break;
		//}
		break;
	case PS_2_HEALING:
		
			// basic Healing formula:
		minDamage = 1 + (player.BaseVitality / 2) + spellLevel;
		maxDamage = 4 + (player.BaseVitality / 2) * 2 + spellLevel * 2;		

			// advanced rules
		if (player.fullClassId == PFC_EXECUTIONER) {
			minDamage += minDamage / 2;
			maxDamage += maxDamage / 2;
		}
		else if ( is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK) ){
			if (!HasTrait(playerIndex, TraitId::Mamluk)) { // Mamulk is a fighter and he needs decent healing (so he skips the nerf)
				minDamage /= 2;
				maxDamage /= 2;
			}
		}
		minDamage += minDamage * (PerkValue(PERK_AUGMENTED_HEALING, playerIndex) + PerkValue(PERK_HEALING_MASTERY, playerIndex)) / 100;
		maxDamage += maxDamage * (PerkValue(PERK_AUGMENTED_HEALING, playerIndex) + PerkValue(PERK_HEALING_MASTERY, playerIndex)) / 100;
		break;
	case PS_3_LIGHTNING:
	case PS_48_RUNE_OF_LIGHT:
		if (GameMode == GM_CLASSIC) {
			minDamage = 4;
			maxDamage = 4 + 2 * player.CharLevel;
		}
		else {
			minDamage = 2 + (player.CharLevel / 4) + (player.CurMagic / 10);
			maxDamage = 2 + (player.CharLevel / 2) + (player.CharLevel / 4) + (player.CurMagic / 10);
		}
		break;
	case PS_4_FLASH:
		if (GameMode == GM_CLASSIC) {
			minDamage = (3 * player.CharLevel / 2) * ((9 / 8) * spellLevel);
			maxDamage = minDamage;
		}
		else {
			minDamage = ((((player.CurMagic + ((player.CharLevel * 5) * spellLevel) / 2) * 3) * 19) / 64);
			minDamage = minDamage + minDamage / 2;
			maxDamage = minDamage;
		}
		break;
	case PS_5_IDENTIFY:
	case PS_7_TOWN_PORTAL:
	case PS_8_STONE_CURSE:
	case PS_9_INFRAVISION:
	case PS_10_PHASING:
		minDamage = -1;
		maxDamage = -1;
		break;

	case PS_11_MANA_SHIELD:
	{
		if (HasTrait(playerIndex, TraitId::Paladin) || GameMode == GM_CLASSIC) {
			minDamage = 100 - PerkValue(PERK_FORTIFIED_SHIELD, playerIndex);
		}
		else {
			minDamage = 100 + 100 * (100 - ( (spellLevel / 4) + (player.CharLevel / 6) + PerkValue(PERK_MIND_OVER_MATTER, playerIndex) + PerkValue(SYNERGY_ENERGY_FIELD, playerIndex))) / 100;
		}
	}
		maxDamage = minDamage;
		break;
	case PS_16_REFLECT:
	case PS_22_FURY:
	case PS_23_TELEPORT:
	case PS_25_ETHEREAL:
	case PS_26_ITEM_REPAIR:
	case PS_27_STAFF_RECHARGE:
	case PS_28_TRAP_DISARM:
	case PS_32_BONE_SPIRIT_REAL_RESSURECT:
	case PS_33_TELEKINES:
	case PS_36_BONE_SPIRIT:
	case PS_42_WARP:
	case PS_46_RELIC_OF_CONCENTR:
	case PS_51_EMPTY:
	case PS_52_LESSER_SUMMON:
	case PS_53_COMMON_SUMMON:
	case PS_54_GREATER_SUMMON:
	case PS_37_MANA_RECHARGE:
	case PS_38_MAGI:
		minDamage = -1;
		maxDamage = -1;
		break;
	case PS_6_FIRE_WALL:
	case PS_17_LIGHTING_RING:
	case PS_19_FLAME_RING:
	case PS_40_LIGHTING_WALL:
	case PS_45_RING_OF_FIRE:
		maxDamage = minDamage = 1 + player.CharLevel / 2;
		//maxDamage = minDamage = 20 * (1 + player.CharLevel / 5);
		break;
	case PS_12_FIREBLAST:
		if (GameMode == GM_CLASSIC) {
			minDamage = ( 4 + 2 * player.CharLevel) * ((9 / 8) * spellLevel);
			maxDamage = (40 + 2 * player.CharLevel) * ((9 / 8) * spellLevel);
		}
		else {
			minDamage = player.CharLevel * 2 + spellLevel * 12;
			maxDamage = minDamage + player.CurMagic;
		}
		break;
	case PS_14_BALL_LIGHTNING:
	case PS_47_NO_SPELL:
		minDamage = 3 * player.CharLevel / 2 + spellLevel * 10;
		maxDamage = minDamage + 2 * player.CurMagic / 3;
		break;
	case PS_15_FORCE_WAVE:
		minDamage = player.CharLevel / 4 + 1;
		maxDamage = player.CharLevel / 4 + 5;
		break;
	case PS_18_LIGHTNING_NOVA:
	case PS_41_FIERY_NOVA:
		//minDamage = (player.CurMagic * 3 / 2) + (spellLevel * 10);
		//maxDamage = minDamage + (player.CharLevel * 5);
		minDamage = player.CurMagic + 20 * spellLevel;
		maxDamage = minDamage + player.CharLevel * 10;
		break;
	case PS_49_RUNE_OF_NOVA:
	case PS_50_RUNE_OF_IMMOLATION:
		minDamage = 20 + ((player.CharLevel * player.CharLevel) / 20);
		maxDamage = minDamage + player.CharLevel;
		break;
	case PS_20_INCINERATE:
		if (GameMode == GM_CLASSIC) {
			minDamage = 6 + 4 * player.CharLevel / 3;
			maxDamage = minDamage;
		}
		else {
			minDamage = (0 + (player.CharLevel * player.CurMagic) / 3) / 12;
			maxDamage = ((player.CharLevel * player.CharLevel) + (player.CharLevel * player.CurMagic) / 3) / 12;
		}
		break;
	case PS_21_GOLEM:
		minDamage = 2 * (spellLevel + 1);
		maxDamage = 4 * (spellLevel + 10);
		break;
	case PS_24_APOCALYPSE:
		if (GameMode == GM_CLASSIC) {
			minDamage = player.CharLevel;
			maxDamage = player.CharLevel * 6;
		}
		else {
			minDamage = (player.CharLevel * player.CharLevel) + (player.CharLevel * player.CurMagic / 15);
			// minDamage = player.CharLevel * player.CurMagic / 4;
			maxDamage = minDamage;
		}
		break;
	case PS_29_ELEMENTAL:
		if (GameMode == GM_CLASSIC) {
			minDamage = (4 + 2 * player.CharLevel) * ((9 / 8) * spellLevel);
			maxDamage = (40 + 2 * player.CharLevel) * ((9 / 8) * spellLevel);
		}
		else {
			minDamage = ((4 * player.CharLevel / 5) + (spellLevel * 7));
			maxDamage = minDamage + 2 * player.CurMagic / 5;
		}
		break;
	case PS_30_CHARGED_BOLT:
		if (GameMode == GM_CLASSIC) {
			minDamage = 1;
			maxDamage = minDamage + player.CurMagic / 4;
		}
		else {
			minDamage = 1 + spellLevel;
			maxDamage = 7 * player.CurMagic / 5 + minDamage;
		}
		break;
	case PS_31_HOLY_BOLT:
		if (GameMode == GM_CLASSIC) {
			minDamage = player.CharLevel + 9;
			maxDamage = minDamage + 9;
		}
		else {
			minDamage = 5 + player.CharLevel * 2 + 8 * spellLevel;
			maxDamage = 5 + minDamage + 8 * player.CurMagic / 11;
			if (player.fullClassId == PFC_TEMPLAR) { minDamage *= 2; maxDamage *= 2; }
		}
		break;
	case PS_39_HOLY_NOVA:
		minDamage = player.CurMagic + spellLevel * 15;
		maxDamage = minDamage + player.CharLevel * 7;
		if (player.fullClassId == PFC_TEMPLAR) { minDamage *= 2; maxDamage *= 2; }
		break;	
	case PS_34_HEAL_OTHER:
		minDamage = player.CharLevel + spellLevel + 1;
		maxDamage = 10 + player.CharLevel * 4 + spellLevel * 6;
		switch(player.ClassID){
		case PC_0_WARRIOR or PC_3_MONK or PC_5_SAVAGE:
			minDamage *= 2;
			maxDamage *= 2;
			break;
		case PC_1_ARCHER or PC_4_ROGUE:
			minDamage += (minDamage / 2);
			maxDamage += (maxDamage / 2);
			break;
		}
		break;
	case PS_35_ARCANE_STAR or PS_44_BERSERK:// berserk is not used
		if (GameMode == GM_CLASSIC) {
			minDamage = 3 * player.CurMagic / 8 + 3 * spellLevel;
			maxDamage = minDamage + player.CharLevel / 5;
		}
		else {
			minDamage = player.CharLevel + spellLevel * 9;
			maxDamage = minDamage + 4 * player.CurMagic / 5;
		}
		break;
	case PS_43_ARCANE_NOVA:
		minDamage = player.CurMagic + spellLevel * 20;
		maxDamage = minDamage + player.CharLevel * 8;		
		break;
	case PS_56_ICE_BOLT:
		minDamage = 1 + player.CurMagic/3 + spellLevel;
		maxDamage = minDamage + 10 + player.CharLevel / 2;
		break;
	case PS_59_RANCID_BOLT:
		minDamage = 1 + player.CurMagic/3 + spellLevel;
		maxDamage = minDamage + 15 + player.CharLevel;
		break;
	case PS_57_FREEZING_BALL:
		minDamage = player.CharLevel + spellLevel * 10;
		maxDamage = minDamage + player.CurMagic;
		break;
	case PS_60_TOXIC_BALL:
		minDamage = (player.CharLevel * 2) + spellLevel * 15;
		maxDamage = minDamage + (player.CurMagic * 5 / 3);
		break;
	case PS_58_FROST_NOVA:
		minDamage = player.CurMagic + spellLevel * 20;
		maxDamage = minDamage + player.CharLevel * 9;
		break;
	case PS_61_ACID_NOVA:
		minDamage = player.CurMagic * 2 + spellLevel * 20;
		maxDamage = minDamage + player.CharLevel * 10;
		break;
	default:
		break;
	}
	//int wildsorcery_trait_benefit = HasTrait(playerIndex, TraitId::WildSorcery) ? 20 : 0;
	if( pMinDamage && pMaxDamage ){
		if( minDamage != -1 && maxDamage != -1 ){
			minDamage += minDamage * player.spellPercent[ spellIndex ] / 100;
			maxDamage += maxDamage * player.spellPercent[ spellIndex ] / 100;
			minDamage += player.spellDamageMin[ spellIndex ];
			maxDamage += player.spellDamageMax[ spellIndex ];
			if (spellIndex != PS_11_MANA_SHIELD) {
				minDamage += minDamage * player.elementPercent[element] / 100;
				maxDamage += maxDamage * player.elementPercent[element] / 100;
				minDamage += player.elementMinDamage[element];
				maxDamage += player.elementMaxDamage[element];
			}
		}
		int avatar_of_cold_trait_min = minDamage / 4;
		int avatar_of_cold_trait_max = maxDamage / 4; // saving pure dmg value
		int dark_pact_trait_benefit_min = minDamage * player.CharLevel / 100;
		int dark_pact_trait_benefit_max = maxDamage * player.CharLevel / 100;
		int damage_min_save = minDamage;
		int damage_max_save = maxDamage;
		if (HasTrait(playerIndex, TraitId::WildSorcery) && (spellIndex != PS_11_MANA_SHIELD)) { // common mage classes trait
			*pMinDamage = minDamage * 130 / 100;
			*pMaxDamage = maxDamage * 130 / 100;
		}
		else {
			*pMinDamage = minDamage;
			*pMaxDamage = maxDamage;
		}
		
		// ---class specific traits -------
		if (HasTrait(playerIndex, TraitId::DarkPact)) { // warlock trait
			if (element == ET_4_ACID) {
				*pMinDamage += dark_pact_trait_benefit_min;
				*pMaxDamage += dark_pact_trait_benefit_max;
			}
		}
		else if (HasTrait(playerIndex, TraitId::AvatarOfCold)) { // elementalist trait
			if (element == ET_6_COLD) {
				*pMinDamage += avatar_of_cold_trait_min;
				*pMaxDamage += avatar_of_cold_trait_max;
			}
			else if (element == ET_1_FIRE) {
				*pMinDamage -= avatar_of_cold_trait_min;
				*pMaxDamage -= avatar_of_cold_trait_max;				
			}
		}		
		else if (HasTrait(playerIndex, TraitId::Cleric)) { // mage trait
			if (element == ET_5_HOLY) {
				*pMinDamage += damage_min_save * 50 / 100;
				*pMaxDamage += damage_max_save * 50 / 100;
			}
			else if (is(element, ET_1_FIRE, ET_2_LIGHTNING, ET_3_ARCAN, ET_0_PHYSICAL)) {
				*pMinDamage -= damage_min_save * 20 / 100;
				*pMaxDamage -= damage_max_save * 20 / 100;
			}
		}

		//-----spell damage masteries---------
		if (element == ET_1_FIRE) {
			*pMinDamage += damage_min_save * (PerkValue(PERK_FIRE_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
			*pMaxDamage += damage_max_save * (PerkValue(PERK_FIRE_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (element == ET_2_LIGHTNING) {
			*pMinDamage += damage_min_save * (PerkValue(PERK_LIGHTNING_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
			*pMaxDamage += damage_max_save * (PerkValue(PERK_LIGHTNING_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (element == ET_3_ARCAN) {
			*pMinDamage += damage_min_save * (PerkValue(PERK_ARCANE_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
			*pMaxDamage += damage_max_save * (PerkValue(PERK_ARCANE_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (element == ET_4_ACID) {
			*pMinDamage += damage_min_save * (PerkValue(PERK_ACID_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
			*pMaxDamage += damage_max_save * (PerkValue(PERK_ACID_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (element == ET_6_COLD) {
			*pMinDamage += damage_min_save * PerkValue(PERK_COLD_MASTERY, playerIndex) / 100;
			*pMaxDamage += damage_max_save * PerkValue(PERK_COLD_MASTERY, playerIndex) / 100;
		}
		else if (element == ET_0_PHYSICAL) {
			*pMinDamage += damage_min_save * (PerkValue(PERK_MAGIC_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
			*pMaxDamage += damage_max_save * (PerkValue(PERK_MAGIC_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (element == ET_5_HOLY) {
			*pMinDamage += damage_min_save * ( PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1) + PerkValue(SYNERGY_ABJURATION, playerIndex, 1)) / 100;
			*pMaxDamage += damage_max_save * ( PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1) + PerkValue(SYNERGY_ABJURATION, playerIndex, 1)) / 100;
		}
		if (HasTrait(playerIndex, TraitId::GrimDeal)) {
			damage_min_save -= (player.CharLevel / 3) * damage_min_save / 100;
			damage_max_save -= (player.CharLevel / 3) * damage_max_save / 100;
		}
		return 0;
	}else{
		if( minDamage == -1 && maxDamage == -1 ){
			return 0;
		}
		int damage = RngFromRange( minDamage, maxDamage );
		if( is(element, ET_1_FIRE, ET_2_LIGHTNING, ET_3_ARCAN) ){
            damage *= 4; // this is compensation for monsters always having resistance to MFL elements
        }
		return damage;
	}
}

//----- (0042EF52) --------------------------------------------------------
int __fastcall IsPathBlockedToMoveSpell( int row1, int col1, int row2, int col2 )
{
	int col = col1;
	int row = row1;
	int result = 0;
	while( row != row2 || col != col2 ){
		int orientation = OrientationToTarget(row, col, row2, col2);
		row += RowDelta[orientation];
		col += ColDelta[orientation];
		if( TileBlockWalking[FineMap[ row ][ col ]] ){
			result = 1; // место для оптимизации после сверки, можно поставить return 1;
		}
	}
	return result;
}

//----- (0042EFAB) --------------------------------------------------------
int __fastcall FindMonsterTargetInRadius( int startRow, int startCol, int radius, std::function<bool(uint monsterIndex)> check )
{
	if( radius > 19 ){
		radius = 19;
	}
	int circleOffset = 0;
	int coordinatesCount = 0;
	int monsterNumber = 0;
	for( int circleOffsetIndex = 1; circleOffsetIndex < radius; circleOffsetIndex++ ){
		circleOffset = CircleOffsets[circleOffsetIndex];
		coordinatesCount = CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = startRow + CircleAndWallSegmentDeltas[ 1 + circleOffset + coordinateIndex * 2 ];
			int col = startCol + CircleAndWallSegmentDeltas[ 1 + circleOffset + coordinateIndex * 2 + 1 ];
			if( To112(row, col) ){
				int monsterNumber = MonsterMap[ row ][ col ];
				if( monsterNumber > 0 && !IsPathBlockedToMoveSpell( startRow, startCol, row, col ) ){
				    int monsterIndex = monsterNumber - 1;
				    if( check( monsterIndex ) ){
                        return monsterIndex;
                    }
				}
			}
		}
	}
	return monsterNumber - 1;
}

//----- (th4) ------------------------------------------------------------
int SpellLevelMax()
{
	return /*GameMode == GM_CLASSIC ? 15 :*/ SpellLevelMax_200;
}

//----- (th4) ------------------------------------------------------------
int SpellLevelBookMax()
{
	return GameMode == GM_CLASSIC ? 15 : SpellLevelBookMax_100;
}

//----- (0042F055) --------------------------------------------------------
int __fastcall PlayerSpellLevel(uint playerIndex, uint spellIndex, int spellOrigin /*=-1*/)
{
	Player& player = Players[playerIndex];
	int slvl = 0;
	if( spellIndex < size(player.SpellLevels) ){
		if( spellOrigin == -1 ) spellOrigin = player.SpellType;
		if( spellIndex >= PS_47_NO_SPELL && spellIndex <= PS_51_EMPTY ){ // runes
			slvl += player.CharLevel / 4;
		}else{
			slvl += player.SpellLevels[spellIndex];
		}
		__int64 spellMask = player.AvailableSpellMask | player.AvailableSkillMask;
		if( is(spellOrigin, SO_2_RELIC, SO_3_EQUIPED_ITEM) ){
			spellMask |= player.AvailableChargesMask | player.AvailableRelictMask;
		}
		if( spellMask & BIT(spellIndex) ){
			slvl += player.allSpellExtraLevel + player.spellExtraLevel[ spellIndex ];
		}
		LimitToRange(slvl, 0, SpellLevelMax());
	}
	return slvl;
}

//----- (0042F08B) --------------------------------------------------------
void __fastcall RemoveMissile(int missileIndex, int missileIndexIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( missile.BaseMissileIndex == MI_145_TENTACLE ){
		Monster& monster = Monsters[missile.CasterIndex];
		monster.flag &= ~MF_16_TRACK_ATTACK;
	}
	int newMissileAmount = MissileAmount - 1;
	MissileIndexList[MissilesMax - MissileAmount] = missileIndex;
	if( MissileAmount > 1 && missileIndexIndex != MissileAmount ){ // swap deleted missile with last ? //Is > 1 reserved for player TP?  Or should it be > 0?
		MissileIndexes[missileIndexIndex] = MissileIndexes[newMissileAmount];
	}
	--MissileAmount;
}

//----- (th2) -------------------------------------------------------------
bool IsMonsterImmuneToMissile( int monsterIndex, int damageType, int playerIndex )
{
	const Monster& monster = Monsters[monsterIndex];
	const MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	int monsterResistImmunity = monster.ResistImmune;
	
	// Quest monster has something to say
	if( monster.speechIndex ){
		return true;
	}
	
	// Monster dies
	if( (monster.CurrentLife & ~63) <= 0 ){
		return true;
	}
	
	// Stone cursed or charging monsters
	if( monster.CurAction == A_15_STONE && GameMode != GM_CLASSIC && ( !HasTrait(playerIndex, TraitId::Petrifier, TraitId::BreakerOfStones) || damageType != ET_0_PHYSICAL )
	 || monster.CurAction == A_14_CHARGE ){
		return true;
	}
	
	// Retreating illusion weaver // TODO: check for DUNGEON id needing, instead of level
	if( is( monsterSprite.baseMonsterIndex, BM_92_ILLUSION_WEAVER, BM_8_BONE_CREEPER ) && Dungeon->level >= 21 && monster.goal == MG_2_RETREAT ){
		return true;
	}

	// Classic mode Illusion Weavers are immune to player attacks when retreating
	if (GameMode == GM_CLASSIC && monsterSprite.baseMonsterIndex == BM_C_344_ILLUSION_WEAVER && monster.goal == MG_2_RETREAT) {
		return true;
	}
	
	MONSTER_RESIST immunityFlag = MR_0_NO;
	switch( damageType ){
	case ET_1_FIRE:      immunityFlag = MR_5_IM_FIRE;  break;
	case ET_2_LIGHTNING: immunityFlag = MR_6_IM_LIGHT; break;
	case ET_4_ACID:      immunityFlag = MR_8_IM_ACID;  break;
	case ET_3_ARCAN:     immunityFlag = MR_4_IM_ARCAN; break;
	case ET_6_COLD:      immunityFlag = MR_11_IM_COLD; break;
	}

	// Monster has immunity to element
	if( monsterResistImmunity & immunityFlag /*&& !(Players[playerIndex].gameChanger & BIT(GC_11_VACCINATED))*/ ){
		return true;
	}
	
	return false;
}

//----- (0042F0BF) --------------------------------------------------------
int __fastcall MvM_Ranged( int defenderIndex, int attackerIndex, int minDamage, int maxDamage, int distance, int spellEffect, char isInternalHP, bool reflected )
{
	Monster& defender = Monsters[defenderIndex];// Attacked monster
	Monster& attacker = Monsters[attackerIndex];// Attacking monster
	MonsterSprite& monsterSprite = MonsterSprites[defender.SpriteIndex];
	int monsterResistImmunity = defender.ResistImmune;
	
	int damageType = BaseMissiles[spellEffect].DamageType;
	int effectType = BaseMissiles[spellEffect].EffectType;
	int playerIndex = attackerIndex / SummonMonstersPerPlayer_Count;
	if( (uint)attackerIndex < SummonMonsters_Count && IsMonsterImmuneToMissile( defenderIndex, damageType, playerIndex ) ){
		return 0;
	}
 
    // Prevent very long distance missiles hits and monsters activation
    if( distance > 40 ){
        return 0;
    }
 
	if( attackerIndex != M1_TRAP && Monsters[attackerIndex].flag & MF_6_FRIENDLY && !defender.ActivationCounter ){
		defender.ActivationCounter = 255;
		if( reflected && (uint)attackerIndex < SummonMonsters_Count ){
			defender.LastTargetPositionRow = Players[playerIndex].Row;
			defender.LastTargetPositionCol = Players[playerIndex].Col;
		}else{
			defender.LastTargetPositionRow = Monsters[attackerIndex].Row;
			defender.LastTargetPositionCol = Monsters[attackerIndex].Col;
		}
		MonsterChainActivation( defenderIndex );
	}
	
	int isResist = 0;
	if( monsterResistImmunity & MR_1_ARCAN && damageType == ET_3_ARCAN 
	 || monsterResistImmunity & MR_2_FIRE  && damageType == ET_1_FIRE 
	 || monsterResistImmunity & MR_3_LIGHT && damageType == ET_2_LIGHTNING
	 || monsterResistImmunity & MR_9_ACID  && damageType == ET_4_ACID
	 || monsterResistImmunity & MR_10_COLD && damageType == ET_6_COLD ){
		isResist = 1;
	}
	int missChance = RNG(100); 
	bool canHitStoneCursed = false;
	if (defender.CurAction == A_15_STONE) {
		if( attackerIndex < SummonMonsters_Count && attackerIndex != M1_TRAP ){
			const int ownerIndex = attackerIndex / SummonMonstersPerPlayer_Count;
			if (HasTrait(ownerIndex, TraitId::BreakerOfStones)) {
				canHitStoneCursed = true;
			}
		}
		if (!canHitStoneCursed) {
			missChance = 100;
		}
	}
	int defender_monster_AC = defenderIndex < SummonMonsters_Count ? defender.ArmorClass : (defender.ArmorClass / 2);
	int hitChance = attackerIndex != M1_TRAP ? 35 + attacker.ToHit - defender_monster_AC
		: 90 - defender.intel - distance; // old incompetent logic
	LimitToRange(hitChance, 15, 85);
	if( spellEffect == MI_140_EXPLODER ){
		hitChance = 100;
	}

	if( missChance >= hitChance ){
		return 0;
	}

	if( defender.CurAction == A_15_STONE && !canHitStoneCursed){
		return 0;
	}

	int isMonsterHit;
	if( CheckMonsterPhaseOrBlock(defenderIndex, &isMonsterHit) ){
		return isMonsterHit;
	}

	int damage = RngFromRange(minDamage, maxDamage);
	if( !isInternalHP ){
		damage *= 64;
	}
	if( spellEffect == MI_140_EXPLODER ){
		damage *= 14;
	}
	if( monsterSprite.baseMonsterIndex == BM_109_GOLEM ){
	    damage -= (50 + PerkValue(PERK_GOLEM_RESISTANCE, defenderIndex / SummonMonstersPerPlayer_Count)) * damage / 100; // legacy golem range resist to all damage
		damage -= PerkValue(PERK_GOLEM_MASTERY, defenderIndex) * damage / 100; // mordor: preparing perk that will increase golem's damage resistance (upd: the time has come!)
    }else if( defender.flag & MF_6_FRIENDLY ){
        damage /= 2; // resist for summons
    }
	
	if( isResist ){
	    damage /= 4;
    }

	if( attackerIndex != M1_TRAP ){
		if (! reflected && MinionCrits(attackerIndex, defenderIndex)) {
			damage *= 2;
		}
		if( ! reflected && Necropathy(attackerIndex, defenderIndex) ){
			damage += damage / 5;
		}
	}
	if (defender.newBossId) {
		damage /= 2;
	}

	if( defender.newBossId - 1  == UM_12_Uber_Diablo || defender.newBossId - 1 == UM_773_The_Dark_Lord){
		damage = 2 * damage / 5;
	}	
	
	if( ! reflected && attackerIndex != M1_TRAP && Monsters[attackerIndex].flag & MF_6_FRIENDLY ){
	    if( MaxCountOfPlayersInGame != 1 ){
            if(       NetPlayerCount == 2 ){ damage -= damage/4; // -25% damage
            }else if( NetPlayerCount == 3 ){ damage -= 2*damage/5; // -40% damage
            }else if( NetPlayerCount == 4 ){ damage -= damage/2; // -50% damage
            }
        }
    }
	if (has(defender.affixes, MAF_ARMORED) && (effectType == MT_0_ARROW && damageType == ET_0_PHYSICAL)) { //33% arrow damage reduce from monster affix
		damage = damage * 67 / 100;
	}
	if (has(defender.affixes, MAF_RESISTANT) && is(effectType, MT_1_MAGIC, MT_2_MAGIC_EXPLODE) || (effectType == MT_0_ARROW && damageType != ET_0_PHYSICAL)) { //33% magical damage reduce from monster affix
		damage = damage * 67 / 100;
	}
	defender.CurrentLife -= damage;
	
	if( (defender.CurrentLife & (-64)) > 0 ){
	    DamageMonsterByMonster(defenderIndex, attackerIndex, damage);
	}else{
		DrawFloatingDamage(damage >> 6, Monsters[defenderIndex].Row, Monsters[defenderIndex].Col,defenderIndex);
		KillMonsterByMonster(attackerIndex, defenderIndex);
	}
	return 1;
}

//----- (00705A50) --------------------------------------------------------
int __fastcall DoesMissileKnockback(int playerIndex, int effectIndex)
{
	BaseMissile& baseMissile = BaseMissiles[effectIndex];
	Player& player = Players[playerIndex];
	if (GameMode == GM_CLASSIC) {
		return 0;
	}
	else if( baseMissile.EffectType == MT_0_ARROW ){
		if( is(effectIndex, MI_121_TRAPPER_GUN_ARROW) || baseMissile.DamageType != ET_0_PHYSICAL ){ // not elemental weapon explosion
			return 0;
		}else if( player.effectFlag[EA_KNOCK_BACK] ){
			return 1;
		}
	}else if( is(effectIndex, MI_61_ELEMENTAL, MI_63_BONE_SPIRIT, MI_53_HOLY_BOLT, MI_41_HELLFIRE, MI_14_HELLFIRE_SEGMENT) ){ // MI_6_FIREBLAST, MI_24_ARCANE_STAR
		return 1;
	}
	return 0;
}

const int IMMUNE_TO_RANGE_ATTACK = -1;

//----- (th2) --------------------------------------------------------
int Calc_PvM_Ranged_Hit_Chance( int playerIndex, int monsterIndex, int distance, int baseMissileIndex)
{
	Player& player = Players[ playerIndex ];
    Monster& monster = Monsters[monsterIndex];
    BaseMissile& baseMissle = BaseMissiles[baseMissileIndex];
	BaseMonster* baseMonster = monster.BasePtr;
    ushort monsterClass = baseMonster->MonsterClass;
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
    int damageType = baseMissle.DamageType;
	
	if (IsMonsterImmuneToMissile(monsterIndex, damageType, playerIndex)) {
		return IMMUNE_TO_RANGE_ATTACK;
	}
	
	if( monsterClass == MON_0_UNDEAD && is( baseMissileIndex, MI_61_ELEMENTAL, MI_63_BONE_SPIRIT)
	 || monsterClass != MON_0_UNDEAD && damageType == ET_5_HOLY // по андедам попадает весь урон + holy, по остальным только физический
	 || baseMonsterIndex == BM_109_GOLEM && MaxCountOfPlayersInGame != 1 && PlayerFriendly() ){
        return IMMUNE_TO_RANGE_ATTACK;
    }
    int hitChance = 0;
    if( playerIndex == -1 ){ // mor: after short discussion we agreed it's traps
        //hitChance = RNG(75) - 2 * monster.MonsterLevel;// old
		hitChance = 80;// new: traps have static 80% to hit monster
    }
	else{
		// physical arrow part AND elemental weapon damage accuracy (yes, it depends on accuracy and Dexterity)
        if( baseMissle.EffectType == MT_0_ARROW /*&& damageType == ET_0_PHYSICAL*/ ){
			int ez_tohit_bonus = is(GameMode, GM_EASY/*, GM_CLASSIC*/) ? 30 : 0;
			if (HasTrait(playerIndex, TraitId::Ranger)) { 
				ez_tohit_bonus += 20 + player.CharLevel; 
			}
			else if (HasTrait(playerIndex, TraitId::Mamluk)) {
				ez_tohit_bonus += 25;
			}
			int monsterAC = GetMonsterAcAfterAp( playerIndex, monster.ArmorClass )
				//+ by( Difficulty, 0, RNG(51) - 50, RNG(101) - 100 )
				+ Difficulty == DL_0_HORROR ? 0 : (RNG( 1 + Difficulty * 50 ) - (Difficulty * 50 ))
				+ ( monster.newBossId ? RNG(21) : 0 );
            hitChance = player.CharLevel
				+ player.ItemsAddToHitPercents
				+ player.spiciesToHit[monster.BasePtr->MonsterClass]
				- monsterAC
				- (PerkValue(PERK_FARSIGHT, playerIndex) == 1 ? ((distance * distance) / 4) : (( distance * distance ) / 2))
				// - player.armorPiercing // removed? // (mor: no, look above, it's unified)
				+ player.CurDexterity
				+ 30
				+ ez_tohit_bonus;
            switch( player.fullClassId ){ // accuracy with (x)bow
			case PFC_SHARPSHOOTER:									hitChance += 40; break;
			case PFC_WARRIOR or PFC_INQUISITOR or PFC_TEMPLAR:		hitChance += 10; break;
			case PFC_DIMACHAERUS or PFC_THRAEX or PFC_SECUTOR:		hitChance += 15; break;
			case PFC_GUARDIAN or PFC_IRON_MAIDEN:					hitChance += 30; break;
			case PFC_EXECUTIONER or PFC_SAVAGE or PFC_BERSERKER:                                         hitChance += 25; break;
			case PFC_ARCHER or PFC_SCOUT or PFC_MURMILLO:			hitChance += 20; break;
			case PFC_ASSASSIN:										hitChance += 15; break;
			}

			if (player.fullClassId == PFC_EXECUTIONER) { // because Executioner now has max Dex of 25, he needs this boost
				hitChance += player.CharLevel;
			}
			if (HasTrait(playerIndex, TraitId::Crupellarius)) { hitChance -= 10; }

			switch( baseMissileIndex ){ // magic arrow elemental explosion (not yet used in affixes)
			case MI_27_FIRE_ARROW: hitChance += player.effectActionPercent[ EA_ARROW_FIRE ].chance; break;
			case MI_56_LIGHTING_ARROW: hitChance += player.effectActionPercent[ EA_ARROW_LIGHTNING ].chance; break;
			case MI_77_HOLY_BOLT_ARROW: hitChance += player.effectActionPercent[ EA_ARROW_HOLY ].chance; break;
			case MI_108_ARCANE_ARROW: hitChance += player.effectActionPercent[ EA_ARROW_ARCAN ].chance; break;
			case MI_109_ACID_ARROW: hitChance += player.effectActionPercent[ EA_ARROW_ACID ].chance; break;
			// what about cold?
			}
		}
		else{ // spells and magic
			int ez_tohit_bonus_mag = is(GameMode, GM_EASY/*, GM_CLASSIC*/) ? 30 : 0;
			hitChance = (player.CurMagic / 2) - (monster.ArmorClass / 4 + 1) - distance + 25 + ez_tohit_bonus_mag
				+ player.elementToHit[ damageType ];
			switch( player.fullClassId ){
			case PFC_INQUISITOR:	hitChance += 80; break;
			case PFC_DRUID:
			case PFC_SCOUT:
			case PFC_TRAPPER:		hitChance += 50; break;
			case PFC_MAGE:			
			case PFC_ELEMENTALIST:	
			case PFC_WARLOCK:		hitChance += 25; break;
			case PFC_DEMONOLOGIST:	
			case PFC_NECROMANCER:	
			case PFC_BEASTMASTER:	hitChance += 10; break;
			case PFC_ROGUE:			
			case PFC_ASSASSIN:
			case PFC_BOMBARDIER:	hitChance += 60; break;
			}
			if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) { hitChance += 30; 	}
			hitChance += PerkValue(PERK_MASTER_CASTER, playerIndex) + PerkValue(PERK_SANCTITY, playerIndex);			
			if (HasTrait(playerIndex, TraitId::Paladin)) {	hitChance += 30;	}
        }
    }
	
    // Temp hack to see damage from trappers trap
    if( baseMissileIndex == MI_121_TRAPPER_GUN_ARROW )
	{
		hitChance = 100;
	}
    
    int maxChanceToHit = monster.newBossId ? 80 : 95;
	if (baseMonsterIndex == BM_162_CORRUPT_PALADIN) {
		if (is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON )) {
			maxChanceToHit = 47;
		}
		else if (GameMode == GM_EASY) {
			maxChanceToHit = 58;
		}
		else if (GameMode == GM_HARD) {
			maxChanceToHit = 29;
		}
		else {
			maxChanceToHit = 35;
		}
	}
	if (baseMonsterIndex == BM_118_DRAGON_KIN) {
		if (MaxCountOfPlayersInGame != 1) {
			maxChanceToHit = 57 - Difficulty * 2; // MP : 57 - 41
		}
		else {
			maxChanceToHit = 76 - Difficulty * 2; // SP : 76 - 60
		}
		if (GameMode == GM_HARD) {
			maxChanceToHit -= 10;
		}
		else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
			maxChanceToHit += 10;
		}
	}
	if (HasTrait(playerIndex, TraitId::Zealot)) {
		maxChanceToHit -= 15 * maxChanceToHit / 100;
	}
	LimitToMin(maxChanceToHit, 15);
	LimitToRange(hitChance, 5, maxChanceToHit);
	return hitChance;
}

//----- (th2) --------------------------------------------------------
int CalculateArrowDistance( int playerIndex, int monsterIndex )
{
    int speed = GetPlayerArrowSpeed( playerIndex );
    Monster& monster = Monsters[monsterIndex];
    Player& player = Players[playerIndex];
    
    int deltaRow = monster.Row - player.Row;
    int deltaCol = monster.Col - player.Col;
    double y = (double)(( deltaRow - deltaCol ) << 21);
    double x = (double)(( deltaRow + deltaCol ) << 21);
    double range = sqrt(y * y + x * x);
    return static_cast<int>( range / double( speed << 16 ) );
    
    //missile.DeltaY = ftol((double)(missileSpeed << 16) * y / range);
    //missile.DeltaX = ftol((double)(missileSpeed << 15) * x / range);
    
    
    /*auto xDelta = static_cast<double>( (player.Row  - monster.Row) * 64 );
    auto yDelta = static_cast<double>( (player.Col  - monster.Col) * 64 );
    return static_cast<int>( sqrt( xDelta * xDelta + yDelta * yDelta ) / double( speed ) );*/
}

//----- (th2) --------------------------------------------------------
void MultiplyElementalWeaponExplosion( int& minDamage, int& maxDamage, int damageType, int baseMissileIndex, int playerIndex )
{
	Player& player = Players[ playerIndex ];
	
	double classCoef = 1; // more value - more damage

	switch( player.fullClassId ){
    case PFC_WARRIOR     : classCoef = 0.3; break;
	case PFC_INQUISITOR: if (damageType == ET_5_HOLY) { classCoef = 0.5; }
						 else if (damageType == ET_1_FIRE) { classCoef = 1.0 + ((double)PerkValue(PERK_EXTRA_CRISPY, playerIndex) / 100); }
						 else if (damageType == ET_2_LIGHTNING) { classCoef = 1.0 + ((double)PerkValue(PERK_FULLY_CHARGED, playerIndex) / 100); }
						 else if (damageType == ET_3_ARCAN) { classCoef = 1.0 + ((double)PerkValue(PERK_CHAOS_BORN, playerIndex) / 100); }
						 else if (damageType == ET_6_COLD) { classCoef = 1.0 + ((double)PerkValue(PERK_DEEP_FREEZE, playerIndex) / 100); }
						 else if (damageType == ET_4_ACID) { classCoef = 1.0 + ((double)PerkValue(PERK_TOXICITY, playerIndex) / 100); }
						 
						 if (HasTrait(playerIndex, TraitId::Tormentor)) { classCoef += 0.2; }
						 break;
	case PFC_GUARDIAN    : classCoef = 0.3; break;
    case PFC_TEMPLAR     : classCoef = damageType == ET_5_HOLY ? 1.0 : 0.3; break;

    case PFC_ARCHER      : classCoef = 0.3; break;
    case PFC_SCOUT       : if (damageType == ET_1_FIRE) { classCoef = 1.0 + ((double)PerkValue(PERK_EXTRA_CRISPY, playerIndex) / 100); }
						   else if (damageType == ET_2_LIGHTNING) { classCoef = 1.0 + ((double)PerkValue(PERK_FULLY_CHARGED, playerIndex) / 100); }
						   else if (damageType == ET_5_HOLY) { classCoef = 1.0 + ((double)PerkValue(PERK_BLESSED_ARROWS, playerIndex) / 100); }
						   else if (damageType == ET_4_ACID) { classCoef = 1.0 + ((double)PerkValue(PERK_TOXICITY, playerIndex) / 100); }
						   else if (damageType == ET_3_ARCAN) { classCoef = 1.0 + ((double)PerkValue(PERK_CHAOS_BORN, playerIndex) / 100); }
						   else if (damageType == ET_6_COLD) { classCoef = 1.0 + ((double)PerkValue(PERK_DEEP_FREEZE, playerIndex) / 100); }
						   break;
    case PFC_SHARPSHOOTER: classCoef = 0.3; break;
    case PFC_TRAPPER     : classCoef = 1.0; break;

	case PFC_MAGE        : classCoef = 1.0; break;
    case PFC_ELEMENTALIST: classCoef = damageType == ET_4_ACID ? 1.0 : 0.6; break;
    case PFC_DEMONOLOGIST: classCoef = 1.0; break;
    case PFC_NECROMANCER : classCoef = 1.0; break;
    case PFC_BEASTMASTER : classCoef = 1.0; break;
    case PFC_WARLOCK     : classCoef = 1.0; break;

	case PFC_MONK		 : classCoef = 0.3; break;
	case PFC_KENSEI		 : classCoef = 0.3; break;
    case PFC_SHUGOKI     : classCoef = 0.3; break;
    case PFC_SHINOBI     : classCoef = 0.3; break;

	case PFC_ROGUE       : classCoef = 0.3; break;
    case PFC_ASSASSIN    : if (damageType == ET_4_ACID) { classCoef = 1.0 + ((double)PerkValue(PERK_TOXICITY, playerIndex) / 100); }
						   else { classCoef = 0.3; }
						   break;
    case PFC_IRON_MAIDEN : classCoef = 0.3; break;
    case PFC_BOMBARDIER  : classCoef = 1.0; break;

    case PFC_SAVAGE      : classCoef = 0.3; break;
    case PFC_BERSERKER   : classCoef = 0.3; break;
    case PFC_EXECUTIONER : classCoef = 0.3; break;
    case PFC_THRAEX      : classCoef = 0.3; break;
    case PFC_MURMILLO    : classCoef = 0.3; break;
    case PFC_DIMACHAERUS : classCoef = 0.3; break;
	case PFC_SECUTOR     : classCoef = 0.3; break;
	case PFC_DRUID       : classCoef = 0.3; break;
	}
	
	if (GameMode == GM_CLASSIC) classCoef = 0.5;

	double baseDamageCoef = 0.06 * player.BaseDamage;
	// if( baseDamageCoef < 1.0 ){ baseDamageCoef = 1.0; } // this is making it worse, wont let balance low level values
	
	minDamage = int( ( baseDamageCoef * minDamage + 5 + (double)player.CharLevel / 2 ) * classCoef );
	maxDamage = int( ( baseDamageCoef * maxDamage + 5 + (double)player.CharLevel / 2 ) * classCoef );
	
	// damage = damage + player.BaseDamage; // вариант TH 1

	// damage = damage * log( player.BaseDamage, 4 ); // вариант с логарифмической кривой

	double elemMultiplier = 1; // more value - more damage
	if( baseMissileIndex == MI_64_WEAPON_ELEMENTAL_DAMAGE ){ // melee weapon elemental explosion
		switch( damageType ){
		case ET_1_FIRE     : elemMultiplier = 0.8; break;
		case ET_2_LIGHTNING: elemMultiplier = 0.8; break;
		case ET_3_ARCAN    : elemMultiplier = 0.8; break;
		case ET_4_ACID     : elemMultiplier = 0.16; break;
		case ET_5_HOLY     : elemMultiplier = 0.2; break;
		case ET_6_COLD     : elemMultiplier = 0.16; break;
		}
	}else{ // magic arrow elemental explosion
		switch( damageType ){
		case ET_1_FIRE     : elemMultiplier = 0.8; break;
		case ET_2_LIGHTNING: elemMultiplier = 0.8; break;
		case ET_3_ARCAN    : elemMultiplier = 0.8; break;
		case ET_4_ACID     : elemMultiplier = 0.16; break;
		case ET_5_HOLY     : elemMultiplier = 0.2; break;
		case ET_6_COLD     : elemMultiplier = 0.16; break;
		}
	}
	minDamage = int( elemMultiplier * minDamage );
	maxDamage = int( elemMultiplier * maxDamage );
}

//int acidCount = 0;
//int dam = 0;
//----- (0042F27D) --------------------------------------------------------
int __fastcall PvM_Ranged( int playerIndex, int monsterIndex, int minDamage, int maxDamage, int distance, int baseMissileIndex, char isInternalHP, int missileIndex, int overTime /*= 0*/ )
{
	Monster& monster = Monsters[monsterIndex];
	Missile& missile = Missiles[missileIndex];
	BaseMissile& baseMissle = BaseMissiles[baseMissileIndex];
	MISSILE_TYPE effectType = baseMissle.EffectType;
	//assert( playerIndex >= 0 && playerIndex < 4 );
	Player& player = Players[ playerIndex ];
	ushort monsterClass = monster.BasePtr->MonsterClass;
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
    int damageType = baseMissle.DamageType;

	int toHitRandFactor = RNG(100);
	int hitChance = Calc_PvM_Ranged_Hit_Chance( playerIndex, monsterIndex, distance, baseMissileIndex );
	if( hitChance < 0 ){
		return 0;
	}
	//dam += isInternalHP ? (minDamage + maxDamage) / 2 : (minDamage + maxDamage) * 32;
	
	if( OneShoot ){
		KillMonsterByPlayer(monsterIndex, playerIndex); return 1;
	}
	if( toHitRandFactor >= hitChance ){
		return 0;
	}
	bool monsterMayBlock = is( baseMonsterIndex, BM_162_CORRUPT_PALADIN, BM_208_WARPED_ONE ) ? (baseMissle.EffectType == MT_0_ARROW && damageType == ET_0_PHYSICAL) : true;
	if( int isMonsterHit = 0; monsterMayBlock && CheckMonsterPhaseOrBlock( monsterIndex, &isMonsterHit ) ){ return isMonsterHit; }
	if( effectType == MT_0_ARROW && damageType != ET_0_PHYSICAL ){
		MultiplyElementalWeaponExplosion( minDamage, maxDamage, damageType, baseMissileIndex, playerIndex );
	}
	int damage = baseMissileIndex == MI_63_BONE_SPIRIT ? ((monster.CurrentLife / (8 + Difficulty - PerkValue(PERK_ATROPHY, playerIndex))) >> 6u) : RngFromRange(minDamage, maxDamage);
	bool trap = baseMissileIndex == MI_121_TRAPPER_GUN_ARROW;
	bool bs = baseMissileIndex == MI_63_BONE_SPIRIT;

	// damage from elemental explosion on xbow doubles
	if( Item &left = player.OnBodySlots[IS_LeftHand], &right = player.OnBodySlots[IS_RightHand];
	 effectType == MT_0_ARROW && damageType != ET_0_PHYSICAL
	 &&( left.ItemCode != IC_M1_NONE && BaseItems[left.baseItemIndex].AnimationDelay
	 || right.ItemCode != IC_M1_NONE && BaseItems[right.baseItemIndex].AnimationDelay ) ){
		damage *= 2;
	}

	if( effectType == MT_0_ARROW && damageType == ET_0_PHYSICAL ){ // physical arrow part (including magic arrow)
		damage += damage * player.ItemsAddDamagePercents / 100;
		int damageFromItem = RngFromRange(player.MinDamageFromItem, player.MaxDamageFromItem);
		
		if( ! trap ){ // на физические ловушки игрока не действуют аффиксы физического урона на вещах, это странно (Mor: все тут правильно, эти целочисленные добавки могут сильно калечить баланс)			
			damage += player.BaseDamage; // на физические ловушки игрока не действует базовый урон игрока, это вроде логично
			if (missile.flag & MSF_MULTISHOT) { // sub arrow from multiply shot
				switch (player.fullClassId) {
				case PFC_SHARPSHOOTER: damage /= 4; break; // damage = int((double) damage * 0.25); break;
				default:      damage = damage * (40 + PerkValue(PERK_STRONG_LATERALS, playerIndex)) / 100; break; // side damage = 40% + 10% per perk level of "strong laterals"
				}
			}			
			damage += player.ItemsAddDamage;
			damage += RngFromRange(player.spiciesDamageMin[monsterClass], player.spiciesDamageMax[monsterClass]);// flat species damages are not applied to trap damage
		}
		damage += damageFromItem * (player.spiciesDamagePercent[monsterClass]) / 100;

		// perks increasing total dmg percent vs species
		damage += damage * PerkValue(by(monsterClass, PERK_PURGE, PERK_EXORCISM, PERK_LETHALITY), playerIndex) / 100;

		int clvl = player.CharLevel;
		int maxArrowDamage = 100'000; // just init value for debug, should always overwrite below
		if(       clvl <  50 ){ maxArrowDamage = (15 + 2 * clvl / 5) * clvl;
		}else /* clvl 50+ */ {	maxArrowDamage = 35 * clvl;
		}

		if( BaseItems[player.OnBodySlots[IS_LeftHand].baseItemIndex].AnimationDelay ){// crossbows can damage more...
		    maxArrowDamage *= 6; // animDelay + 2;
		}
		LimitToMax(damage, maxArrowDamage);
		
		if (HasTrait(playerIndex, TraitId::Cautious)) {
			bool isPointBlank = abs(missile.CasterRow - monster.Row) > 1 || abs(missile.CasterCol - monster.Col) > 1;
			if (isPointBlank) {
				damage = damage * 130 / 100;
			}
			else {
				damage = damage * 50 / 100;
			}
		}
		else if (HasTrait(playerIndex, TraitId::PointBlank)) {
			bool isPointBlank2 = abs(missile.CasterRow - monster.Row) > 1 || abs(missile.CasterCol - monster.Col) > 1;
			if (isPointBlank2) {
				damage = damage * 75 / 100;
			}
			else {
				damage = damage * 165 / 100;
			}
		}
		if (HasTrait(playerIndex, TraitId::Pistoleer) && (abs(missile.CasterRow - monster.Row) >= 2 || abs(missile.CasterCol - monster.Col) >= 2)) {
			int dist_row = abs(missile.CasterRow - monster.Row), dist_col = abs(missile.CasterCol - monster.Col);// initializing variables for distance coordinates
			int dist_used = dist_row > dist_col ? dist_row : dist_col;// picking the greater distance coordinats
			damage += ( (PerkValue(PERK_MARKSMAN, playerIndex) * (dist_used - 1) ) * damage) / 100;// the actual formula for adjusting distant bullet damage
		}
		if (HasTrait(playerIndex, TraitId::Destroyer)) {
			damage += 3 * damage / 20; // +15%
		}
		//crit checks
		bool canCrit = true;
		if (is(player.fullClassId, PFC_SAVAGE, PFC_SCOUT) || HasTrait(playerIndex, TraitId::Tormentor)) {
			canCrit = false;
		}
		if (canCrit) {
			ActCriticalHit(damage, playerIndex, monster.Row, monster.Col, true, damageType, baseMissileIndex);
		}
		//------------
		if (GameMode == GM_CLASSIC) { goto CLASSIC_DAMAGE_MODIFY_SKIP_R; }
        if( baseMonsterIndex == BM_137_UBER_DIABLO ){ 
			int damageReducePercents;
			if (is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON )) {
				if (MaxCountOfPlayersInGame == 1) {
					damageReducePercents = 56; // IM / NM:  sp -56%
				}
				else {
					damageReducePercents = by(Difficulty, 67, 80, 85, 85, 85, 85, 85, 85, 85); // IM / NM:  mp horror: -67%, -80% prg, 85% doom+
				}
			}
			else {
				if (MaxCountOfPlayersInGame == 1) {
					damageReducePercents = 68; // single normal and hc -68%
				}
				else {
					damageReducePercents = by(Difficulty, 77, 82, 86, 86, 86, 86, 86, 86, 86); // normal and hc, mp: horror -77%, prg -82%, doom+ -86%
				}
			}
			damage -= damage * damageReducePercents / 100;

		// old code below, saving if smth goes wrong				
		/*if( MaxCountOfPlayersInGame != 1 && Difficulty == DL_2_DOOM ){ 
				damage /= 2; 
			}*/
        }
		else if( baseMonsterIndex == BM_198_HORN_CLAN ){ 
			damage /= 2;
        }
		else if( baseMonsterIndex == BM_109_GOLEM ){ 
			// nothing
        }
		else if( baseMonsterIndex == BM_2_ROTTING_CARCASS ){
            if( MaxCountOfPlayersInGame != 1 ){
                damage /= by( Difficulty, 1, 2, 3, 4, 4, 4, 4, 4, 4 );
                if( Dungeon->level == 5 ){
                    damage /= 2;
                }
            }
        }
		else if( is( baseMonsterIndex, BM_120_NECROMORPH, BM_135_GROTESQUE, BM_141_SHAMBLER, BM_142_HELL_MEAT, BM_169_ZOMBIE) ){
            if( MaxCountOfPlayersInGame != 1 ){ 
				damage = by( Difficulty, 
					damage - damage / 4, 
					damage / 2, 
					damage / 2 - damage / 16, 
					damage / 2 - damage / 16, 
					damage / 2 - damage / 16, 
					damage / 2 - damage / 16, 
					damage / 2 - damage / 16, 
					damage / 2 - damage / 16, 
					damage / 2 - damage / 16);
			}
			else {
				if (Dungeon == DUN_23_CRYPT_3) { // necromorph damage resistance increased for dlvls 23 & 24
					damage -= damage / 4;
				}
				else if (Dungeon == DUN_24_CRYPT_4) {
					damage -= damage / 3;
				}
			}
        }
		else if( baseMonsterIndex == BM_52_BILE_DEMON ){ 
			if( Dungeon->level == 9 ){ // TODO: check for id instead of level
				damage -= damage / 4; 
			}
        }
		else if( baseMonsterIndex == BM_31_UNSEEN ){
            if( MaxCountOfPlayersInGame != 1 ){
                if(       Dungeon->level == 9 ){ 
					damage -= damage / 4;
                }else if( Dungeon->level == 10 ){ 
					damage = damage / 8 + damage / 2;
                }
            }
        }
		else if( baseMonsterIndex == BM_40_QUASIT ){ 
			if( Difficulty == DL_0_HORROR && Dungeon->level == 3 ){ 
				damage += damage / 4; 
			}
        }
		else if( baseMonsterIndex == BM_110_DIABLO ){ 
			damage -= damage / 4;
        }
		else if( baseMonsterIndex == BM_58_DEFILER ){ 
			damage -= damage / 4 - damage / 8;
            if( Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3 ){
                damage /= 4;
            }
        }
		else if( baseMonsterIndex == BM_35_FIRE_CLAN ){ 
			if( Dungeon->level < 4 && Difficulty == DL_0_HORROR ){ 
				damage = damage / 8 + damage / 2; 
			}
        }
		else if( baseMonsterIndex == BM_125_GOLD_GOLEM ){
            if(       Dungeon->level == 21 ){ 
				damage -= damage / 4;
            }else if( Dungeon->level == 22 ){ 
				damage -= damage / 4 - damage / 8;
            }else{                          
				damage /= 2;
            }
        }
		else if( baseMonsterIndex == BM_64_DEATH_GUARDIAN ){ 
			if( Dungeon->level == 16 ){ // prince albrecht no longer in pandemonium, deactivated
				damage /= 8; 
			}
        }
		else if( is( baseMonsterIndex, BM_0_BURNED_CORPSE, BM_6_GRAVE_DIGGER) ){
            damage = by( Difficulty, 
				Dungeon->level == 2 ? damage - damage / 4 : damage, 
				damage - damage / 4 - damage / 8, 
				damage / 2,
				damage / 2,
				damage / 2,
				damage / 2,
				damage / 2,
				damage / 2,
				damage / 2	);
        }
		else if( baseMonsterIndex == BM_15_TOMB_SLAVE ){
			damage = by( Difficulty, 
				Dungeon->level == 3 ? damage - damage / 8 : Dungeon->level == 4 ? damage - damage / 4 : damage, // gotta love them ternars! (no)
				damage - damage / 4 - damage / 8, 
				damage / 2,
				damage / 2, 
				damage / 2, 
				damage / 2, 
				damage / 2, 
				damage / 2, 
				damage / 2 );
        }
		else if( is( baseMonsterIndex, BM_72_FIREWING, BM_132_BONEWING) ){ 
			if( Dungeon->level == 5 ){ 
				damage -= damage / 4; 
		}
		}
		else if (baseMonsterIndex == BM_286_BRINE_CORPSE /*|| baseMonsterIndex == BM_287_ARMORED_SKELETON*/ || baseMonsterIndex == BM_292_BUGBEAR) {
			damage -= damage / 3;
		}
		else if (baseMonsterIndex == BM_287_ARMORED_SKELETON) {
			if (Dungeon == DUN_9_CAVE_1) {
				damage -= 2 * damage / 5;
			}
			else {
				damage -= damage / 3;
			}
		}
		else if (baseMonsterIndex == BM_140_GHOUL) {
			if (MaxCountOfPlayersInGame == 1) { // single
				if (Difficulty == DL_0_HORROR) {
					if (Dungeon->level == 6) {
						damage -= damage / 4;
					}
					else if (Dungeon->level == 7) {
						damage -= 37 * damage / 100;
					}
				}
				else { // sp purg & doom
					damage /= 2;
				}
			}
			else if (Difficulty == DL_0_HORROR && Dungeon->level == 5) { // multi
				damage -= damage / 4;
			}
			else if (Difficulty == DL_0_HORROR && Dungeon->level == 6) {
				damage -= 37 * damage / 100;
			}
			else if (Difficulty == DL_0_HORROR && Dungeon->level == 7) {
				damage /= 2;
			}
			else if (Difficulty == DL_1_PURGATORY && Dungeon->level == 5) {
				damage -= 60 * damage / 100;
			}
			else if (Difficulty == DL_1_PURGATORY && Dungeon->level == 6) {
				damage -= 2 * damage / 3;
			}
			else if (Difficulty == DL_1_PURGATORY && Dungeon->level == 7 || Difficulty >= DL_2_DOOM) {
				damage -= 72 * damage / 100;
			}
		}
		else if( is( baseMonsterIndex, BM_144_DARK_SERVANT, BM_145_BLACK_MOURNER) ){
			damage /= 2;
        }
		else if( baseMonsterIndex == BM_155_FALLEN_ONE ){
            if( Difficulty == DL_0_HORROR ){
                if(       Dungeon->level == 3 ){ 
					damage /= 2;
                }else if( Dungeon->level == 4 ){ 
					damage -= damage / 2 - damage / 8;
                }
            }
        }
		else if( baseMonsterIndex == BM_207_DREGBEZ ){ 
			damage /= 4;
        }
		else if( baseMonsterIndex == BM_208_WARPED_ONE ){ 
			damage /= 2;
        }
		else if (baseMonsterIndex == BM_163_STINGER_LORD) {
			damage /= 2;
		}
		else if( baseMonsterIndex == BM_243_IZUAL ){
			damage -= 65 * damage / 100 ; 
			
        }
		else if( baseMonsterIndex == BM_85_BALOR ){
            if( MaxCountOfPlayersInGame != 1 ){
                damage /= 2;
                if( Dungeon->level == 24 ){
                    damage /= 2;
                }
            }
        }
		else if( baseMonsterIndex == BM_95_GHOST_WARRIOR || baseMonsterIndex == BM_285_PHANTOM_KNIGHT){ // knights
			int flat_dfe_gw;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
					flat_dfe_gw = by(Difficulty, 35, 133, 266, 400, 533, 666, 800, 933, 1066);	}
			else {	flat_dfe_gw = by(Difficulty, 50, 200, 400, 600, 800, 1000, 1200, 1400, 1600);	}
			flat_dfe_gw -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_gw, 0);
			damage -= flat_dfe_gw * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
        }
		else if( baseMonsterIndex == BM_96_CURSE_BEARER   ){ 
			int flat_dfe_cb;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
					flat_dfe_cb = by(Difficulty, 40, 150, 300, 450, 600, 750, 900, 1050, 1200);	}
			else {	flat_dfe_cb = by(Difficulty, 60, 225, 425, 625, 825, 1025, 1225, 1425, 1625);	}
			flat_dfe_cb -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_cb, 0);
			damage -= flat_dfe_cb * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if (baseMonsterIndex == BM_261_SKELETAL_KNIGHT) {
			int flat_dfe_cb;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
				flat_dfe_cb = by(Difficulty, 45, 160, 310, 460, 610, 760, 910, 1060, 1210);
			}
			else { flat_dfe_cb = by(Difficulty, 70, 250, 450, 660, 900, 1100, 1350, 1550, 1800); }
			flat_dfe_cb -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_cb, 0);
			damage -= flat_dfe_cb * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if( baseMonsterIndex == BM_122_BLOOD_SPECTRE ){
			int flat_dfe_bs;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
					flat_dfe_bs = by(Difficulty, 52, 180, 330, 480, 630, 780, 930, 1080, 1230);	}
			else {	flat_dfe_bs = by(Difficulty, 80, 275, 500, 750, 1000, 1250, 1500, 1750, 2000);	}
			flat_dfe_bs -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_bs, 0);
			damage -= flat_dfe_bs * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if( is(baseMonsterIndex, BM_233_BLOOD_LORD, BM_297_OROG) ){
			if (Dungeon == DUN_113_THE_MAZE) {
				damage -= damage / 8;
			}
			else {
				damage -= damage / 5;
			}
			int flat_dfe_bl = 0;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
					flat_dfe_bl = by(Difficulty, 12, 48, 96, 144, 192, 240, 288, 336, 384);	
			}
			else {	
				flat_dfe_bl = by(Difficulty, 24, 96, 192, 288, 384, 480, 576, 672, 768); 
			}
			flat_dfe_bl -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_bl, 0);
			damage -= flat_dfe_bl * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if (is(baseMonsterIndex, BM_267_BONE_BLADE, BM_268_BONE_BLADE_2)) {
			damage -= damage / 2;
			int flat_dfe_bl = 0;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
				flat_dfe_bl = 15 + Difficulty * 20;
			}
			else { flat_dfe_bl = 20 + Difficulty * 25; }
			flat_dfe_bl -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_bl, 0);
			damage -= flat_dfe_bl * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if (baseMonsterIndex == BM_275_BEHEMOTH) {
			damage -= 6 * damage / 10;
			int flat_dfe_bl = 0;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
				flat_dfe_bl = 15 + Difficulty * 20; 
			}
			else { flat_dfe_bl = 20 + Difficulty * 30; } 
			flat_dfe_bl -= PerkValue(PERK_PENETRATION, playerIndex); 
			LimitToMin(flat_dfe_bl, 0); 
			damage -= flat_dfe_bl * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100; 
		}
		else if (baseMonsterIndex == BM_234_STEEL_LORD) {
			damage -= damage / 4;
			int flat_dfe_sl = 0;
			if (is(GameMode, GM_EASY, GM_IRONMAN, GM_SPEEDRUN)) {
					flat_dfe_sl = by(Difficulty, 20, 75, 135, 195, 255, 315, 375, 435, 495); }
			else {	flat_dfe_sl = by(Difficulty, 40, 150, 270, 390, 510, 630, 750, 870, 990); }
			flat_dfe_sl -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_sl, 0);
			damage -= flat_dfe_sl * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if (baseMonsterIndex == BM_242_REANIMATED_HORROR) {
			damage -= damage / 4;
			int flat_dfe_ro = by(Difficulty, 10, 70, 140, 210, 280, 350, 420, 490, 560);
			flat_dfe_ro -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_ro, 0);
			damage -= flat_dfe_ro * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
		}
		else if (baseMonsterIndex == BM_197_CEROMORTH) {
			damage /= 4;
		}
		else if (is(baseMonsterIndex, BM_251_WENDIGO, BM_271_THORNED_HULK, BM_272_SPIDER_QUEEN)) {
			damage -= damage / 3;
		}
		else if (baseMonsterIndex == BM_274_FLESH_DEVOURER) {
			damage -= damage / 5;
		}
		else if (is(baseMonsterIndex, BM_115_GRUNT_BEAST, BM_123_BICLOPS)) {
			int flat_dfe_biclops = by(Difficulty, 6, 24, 41, 60, 80, 100, 120, 140, 160);
			flat_dfe_biclops -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_biclops, 0);
			damage -= flat_dfe_biclops * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
			damage -= damage / 3;
		}	
		else if (is(baseMonsterIndex, BM_252_HEPHASTO)) {
			int flat_dfe_smith = by(Difficulty, 40, 160, 320, 480, 640, 800, 960, 1120, 1280);
			if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE, GM_EASY)) {
				flat_dfe_smith /= 2;
			}
			flat_dfe_smith -= PerkValue(PERK_PENETRATION, playerIndex);
			LimitToMin(flat_dfe_smith, 0);
			damage -= flat_dfe_smith * (100 - PerkValue(PERK_BODKIN_POINTS, playerIndex) - PerkValue(PERK_BODKIN_ARROWS, playerIndex)) / 100;
			if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE, GM_EASY)) {
				damage -= damage / 4;
			}
			else {
				damage -= 2 * damage / 5;
			}
		}
	}	
	else { // spells and magic arrow/melee elemental explosion
	   // spell bonus
		int spellIndex = missile.spellIndex;
		damage += damage * player.spellPercent[spellIndex] / 100;
		if (player.spellDamageMin[spellIndex] || player.spellDamageMax[spellIndex]) {
			int addDamage = RngFromRange(player.spellDamageMin[spellIndex], player.spellDamageMax[spellIndex]);
			if (is(damageType, ET_1_FIRE, ET_2_LIGHTNING, ET_3_ARCAN)) {
				addDamage *= 4; // this is compensation for monsters always having resistance to MFL elements
			}
			damage += addDamage;
		}
		// elemental bonus
		damage += damage * player.elementPercent[damageType] / 100;
		if (player.elementMinDamage[damageType] || player.elementMaxDamage[damageType]) {
			int addDamage = RngFromRange(player.elementMinDamage[damageType], player.elementMaxDamage[damageType]);
			if (is(damageType, ET_1_FIRE, ET_2_LIGHTNING, ET_3_ARCAN)) {
				addDamage *= 4; // this is compensation for monsters always having resistance to MFL elements
			}
			damage += addDamage;
		}
		int damage_value = damage;
		if (HasTrait(playerIndex, TraitId::WildSorcery)) {
			damage += damage_value * 30 / 100;
		}
		
		if (HasTrait(playerIndex, TraitId::Cleric)) { // mage trait
			if (damageType == ET_5_HOLY) {
				damage += damage_value * 50 / 100;
			}
			if (is(damageType, ET_0_PHYSICAL, ET_1_FIRE, ET_2_LIGHTNING, ET_3_ARCAN)) {
				damage -= damage_value * 20 / 100;
			}
		}		
		else if (HasTrait(playerIndex, TraitId::AvatarOfCold)) { // elementalist trait
			if (damageType == ET_6_COLD) {
				damage += damage_value * 25 / 100;
			}
			else if (damageType == ET_1_FIRE) {
				damage -= damage_value * 25 / 100;
			}
		}
		else if (HasTrait(playerIndex, TraitId::DarkPact)){ // warlock trait
			if (damageType == ET_4_ACID) {
				damage += damage_value * player.CharLevel / 100; // +CLVL% acid spell damage with Dark Pact trait
			}
		}
		else if (HasTrait(playerIndex, TraitId::Toxic_at_Heart)) { // bombardier trait
			if (damageType == ET_4_ACID) {
				damage -= damage_value * 40 / 100; // -40% acid damage with "toxic at heart" trait
			}
		}
		
		// spell damage masteries
		if (damageType == ET_6_COLD) {
			damage += damage_value * PerkValue(PERK_COLD_MASTERY, playerIndex) / 100;
		}
		else if (damageType == ET_1_FIRE) {
			damage += damage_value * (PerkValue(PERK_FIRE_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (damageType == ET_2_LIGHTNING) {
			damage += damage_value * (PerkValue(PERK_LIGHTNING_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (damageType == ET_3_ARCAN) {
			damage += damage_value * (PerkValue(PERK_ARCANE_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (damageType == ET_4_ACID) {
			damage += damage_value * (PerkValue(PERK_ACID_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (damageType == ET_0_PHYSICAL) {
			damage += damage_value * (PerkValue(PERK_MAGIC_MASTERY, playerIndex) + PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1)) / 100;
		}
		else if (damageType == ET_5_HOLY) {
			damage += damage_value * (PerkValue(SYNERGY_SPELL_POWER, playerIndex) + PerkValue(PERK_CASTING_POWERS, playerIndex) + PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 1) + PerkValue(SYNERGY_ABJURATION, playerIndex, 1)) / 100;
		}
		if (HasTrait(playerIndex, TraitId::GrimDeal)) {
			damage -= (player.CharLevel / 3) * damage / 100;
		}

		//----------spell damage crits!---------------
		bool canCrit_with_elemental_damage = true;
		if (player.fullClassId == PFC_SAVAGE || HasTrait(playerIndex, TraitId::Toxic_at_Heart)) {
			canCrit_with_elemental_damage = false;
		}
		if (player.fullClassId == PFC_INQUISITOR && HasTrait(playerIndex, TraitId::Tormentor)) {
			canCrit_with_elemental_damage = false;
		}
		// Scout can crit with elemental damage!
		//if (player.fullClassId == PFC_SCOUT) {
		//	canCrit_with_elemental_damage = false;
		//}

		if (canCrit_with_elemental_damage) { // ability to crit with spells and elemental damage
			if (!bs) {
				if (HasTrait(playerIndex, TraitId::Black_Witchery)) {
					if (RNG(100) < 10 + player.CharLevel / 5) {
						damage *= 3; // (10 + clvl/5)% chance of crit for +200% damage with black witchery
					}
				}
				else {
					ActCriticalHit(damage, playerIndex, monster.Row, monster.Col, true, damageType, baseMissileIndex);
				}
			}
		}
		// -----  MODIFICATION OF PvM MAGIC DAMAGE AGAINST MONSTERS  ------
		if (GameMode == GM_CLASSIC) { goto CLASSIC_DAMAGE_MODIFY_SKIP_R; }
		if (baseMonsterIndex == BM_137_UBER_DIABLO) {
			// damage /= MaxCountOfPlayersInGame != 1 ? by (Difficulty, 4, 6, 8) : 4; // тут как и с мили. <- old code, changed to the one below in v1.0036
			int damageReducePercents;
			if (is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON )) {
				if (MaxCountOfPlayersInGame == 1) {
					damageReducePercents = 56; // для IM / NM:  в сингле -56%
				}
				else {
					damageReducePercents = by(Difficulty, 67, 80, 85, 85, 85, 85, 85, 85, 85); // для IM / NM:  в мульти  хорроре: -67%,  -80% на пурге, и - 85% на думе+
				}
			}
			else {
				if (MaxCountOfPlayersInGame == 1) {
					if (GameMode == GM_HARD) { damageReducePercents = 73; } // single hard: 73% dmg resist
					else if (is(GameMode, GM_EASY)) { damageReducePercents = 63; } // single easy: 63% dmg resist
					else { damageReducePercents = 68; }// rest normal and hardcore: 68% dmg resist					
				}
				else {
					damageReducePercents = by(Difficulty, 77, 82, 86, 86, 86, 86, 86, 86, 86); // для нормы  и хардкора в мульти: хоррор -77%, пург: -82%, дум+: -86%
				}
			}
			damage -= damage * damageReducePercents / 100;

		}
		else if (baseMonsterIndex == BM_286_BRINE_CORPSE /*|| baseMonsterIndex == BM_287_ARMORED_SKELETON*/ || baseMonsterIndex == BM_292_BUGBEAR) {
			damage -= damage / 3;
		}
		else if (baseMonsterIndex == BM_287_ARMORED_SKELETON) {
			if (Dungeon == DUN_9_CAVE_1) {
				damage -= 2 * damage / 5;
			}
			else {
				damage -= damage / 3;
			}
		}
		else if( baseMonsterIndex == BM_133_SOUL_BURNER ){
			if( MaxCountOfPlayersInGame != 1 ){ 
				damage /= by( Difficulty, 2, 3, 3, 4, 4, 5, 5, 6, 6);
			}
        }
		else if (baseMonsterIndex == BM_235_STYGIAN_FURY) {
			if (MaxCountOfPlayersInGame != 1) {
				damage /= by(Difficulty, 2, 3, 3, 4, 4, 4, 4, 4, 4); // MP: - 50/66/75 % dmg
			}
			else {
				damage -= damage / 3; // SP: -33% dmg
			}
		}
		else if( baseMonsterIndex == BM_32_INVISIBLE_ONE ){
            if( MaxCountOfPlayersInGame != 1 ){
                if(       Dungeon->level ==  8 ){ 
					damage -= damage / 8;
                }else if( Dungeon->level ==  9 ){ 
					damage -= damage / 8 - damage / 16;
                }else if( Dungeon->level == 10 ){ 
					damage -= damage / 4;
                }else if( Dungeon->level == 11 ){ 
					damage -= damage / 4 - damage / 16;
                }else if( Dungeon->level == 12 ){ 
					damage -= damage / 4 - damage / 8;
                }
                if( Difficulty >= DL_2_DOOM ){
                    damage /= 2;
                }
            }
        }else if( is( baseMonsterIndex, BM_33_MOON_CLAN, BM_55_GRIM_DEMON, BM_57_HELL_CLAN, BM_83_LASH_WORM, BM_87_DARK_REAPER, BM_99_CLEAVER_BOSS, BM_111_OBSIDIAN_GOLEM,
		 BM_115_GRUNT_BEAST, BM_123_BICLOPS, BM_125_GOLD_GOLEM, BM_130_BLACK_GOLEM, BM_136_BLOOD_SLAYER, BM_152_BLACK_REAPER, BM_153_DEATH_MINION) ){
            if( MaxCountOfPlayersInGame != 1 ){
                damage -= by( Difficulty, 
					damage / 4, 
					damage / 4 - damage / 8,
					damage / 4 - damage / 8 - damage / 16,
					damage / 4 - damage / 8 - damage / 16,
					damage / 4 - damage / 8 - damage / 16,
					damage / 4 - damage / 8 - damage / 16,
					damage / 4 - damage / 8 - damage / 16,
					damage / 4 - damage / 8 - damage / 16,
					damage / 4 - damage / 8 - damage / 16 ); // -25%, -37%, -43%
            }
        }else if( baseMonsterIndex == BM_28_BLOOD_WARPER ){
            if( MaxCountOfPlayersInGame == 1 ){
                if( Difficulty == DL_1_PURGATORY ){
                    switch( Dungeon->level ){
                    case 17: damage -= damage / 8 - damage / 32; break;
                    case 18: damage -= damage / 8 - damage / 16; break;
                    case 19: damage -= damage / 8 - damage / 16 - damage / 32; break;
                    default: damage -= damage / 4; break;
                    }
                }else if( Difficulty >= DL_2_DOOM ){
                    switch( Dungeon->level ){
                    case 17: damage -= damage / 4 - damage / 32; break;
                    case 18: damage -= damage / 4 - damage / 16; break;
                    case 19: damage -= damage / 4 - damage / 8 - damage / 16; break;
                    default: damage -= damage / 4 - damage / 8; break;
                    }
                }else{ // DL_0_HORROR
                    switch( Dungeon->level ){
                    case 17: damage -= damage / 32; break;
                    case 18: damage -= damage / 16; break;
                    case 19: damage -= damage / 16 - damage / 32; break;
                    default: damage -= damage / 8; break;
                    }
                }
			}else{ // MaxCountOfPlayersInGame != 1
                if( Difficulty == DL_1_PURGATORY ){
                    switch( Dungeon->level ){
                    case 17: damage = damage / 2 - damage / 32; break;
					case 18: damage = damage / 2 - damage / 16; break;
					case 19: damage -= damage / 2 - damage / 16 - damage / 32; break;
					default: damage = damage / 2 - damage / 8; break;
                    }
                }else if( Difficulty >= DL_2_DOOM ){
                    switch( Dungeon->level ){
					case 17: damage = damage / 2 - damage / 8; break;
					case 18: damage = damage / 2 - damage / 8 - damage / 16; break;
					case 19: damage = damage / 2 - damage / 8 - damage / 16 - damage / 32; break;
					default: damage = damage / 2 - damage / 4; break;
                    }
                }else{ // DL_0_HORROR
                    switch( Dungeon->level ){
					case 17: damage -= damage / 4 - damage / 8 - damage / 32; break;
					case 18: damage -= damage / 4 - damage / 8 - damage / 16; break;
					case 19: damage -= damage / 4 - damage / 8 - damage / 16 - damage / 32; break; 
					default: damage /= 2; break;
                    }
                }
            }
        }else if( baseMonsterIndex == BM_110_DIABLO ){ damage -= damage / 4;
        }else if( baseMonsterIndex == BM_58_DEFILER ){ damage -= damage / 4 - damage / 8;
            if( Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3 ){ damage /= 4; }
        }else if( baseMonsterIndex == BM_35_FIRE_CLAN ){ if( Dungeon->level < 4u && Difficulty == DL_0_HORROR ){ damage -= damage / 4 - damage / 8; }
        }else if( baseMonsterIndex == BM_64_DEATH_GUARDIAN ){ if( Dungeon->level == 16 ){ damage /= 8; }
        }else if( is( baseMonsterIndex, BM_140_GHOUL, BM_149_THRESHER) ){
            if( MaxCountOfPlayersInGame != 1 ){
                damage /=  1 + Difficulty;
            }else{
                damage -= damage * (35 + 5 * Difficulty / 2) / 100 ; // shoule be: 35 - 55 percent
            }
		}else if (is(baseMonsterIndex, BM_154_DEATH_MINION, BM_155_FALLEN_ONE)) { damage /= 2;
		}else if (baseMonsterIndex == BM_261_SKELETAL_KNIGHT) { damage -= damage / 3;
        }else if( baseMonsterIndex == BM_166_GOATLORD_BOSS ){
            if( MaxCountOfPlayersInGame != 1 ){
                damage /= 2;
            }else{
				damage -= damage * (25 + 5 * Difficulty / 2) / 100 ; // 25 - 45 percent
            }
        }else if( baseMonsterIndex == BM_162_CORRUPT_PALADIN ){ damage /= 4;
        }else if( baseMonsterIndex == BM_163_STINGER_LORD	 ){	damage /= 2;
		}else if( baseMonsterIndex == BM_197_CEROMORTH       ){ damage /= 2;
        }else if( baseMonsterIndex == BM_207_DREGBEZ         ){ damage /= 8;
        }else if( baseMonsterIndex == BM_208_WARPED_ONE      ){ damage /= 3;
        }else if( baseMonsterIndex == BM_216_ZEALOT          ){ damage /= 15;
        }else if( baseMonsterIndex == BM_243_IZUAL			 ){ damage -= (Difficulty + 1) + damage / 4;
        }
		// below are new specific damage reducers for Reapers (who are new tough monsters for casters and Bombardier)
		// their extra defense works like DFE, reducing spell damage from player. called 'energy shield'
		// on Hard mode, these energy shields are absorbing double the amount
		if (baseMonsterIndex == BM_87_DARK_REAPER) {
			//if (GameMode == GM_HARD) {damage -= by(Difficulty, 40, 160, 320, 480, 640, 800, 960, 1120, 1280);}
			//else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE)) 
			//								{damage -= by(Difficulty, 10,  40,  80, 120, 160, 200, 240, 280, 320);}
			//else							{damage -= by(Difficulty, 20,  80, 160, 240, 320, 400, 480, 560, 640);}
			damage /= 2;
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_136_BLOOD_SLAYER) {
			//if (GameMode == GM_HARD) {damage -= by(Difficulty, 50, 180, 350, 500, 650, 800, 950, 1100, 1250);}
			//else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE)) 
			//								{damage -= by(Difficulty, 15,  45,  90, 135, 180, 225, 270, 315, 360);}
			//else							{damage -= by(Difficulty, 25,  90, 175, 260, 340, 420, 500, 620, 740);}
			damage /= 2; 
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_152_BLACK_REAPER) {
			//if (GameMode == GM_HARD) {damage -= by(Difficulty, 70, 220, 400, 600, 800, 1000, 1200, 1400, 1600);}
			//else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE)) 
			//								{damage -= by(Difficulty, 20,  55, 100, 140, 180, 220, 260, 300, 340);}
			//else							{damage -= by(Difficulty, 35, 110, 200, 290, 380, 470, 560, 650, 740);}
			damage /= 2; 
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_153_DEATH_MINION){
			//if (GameMode == GM_HARD) {damage -= by(Difficulty, 90, 240, 460, 660, 860, 1060, 1260, 1460, 1660);}
			//else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE)) 
			//								{damage -= by(Difficulty, 25,  60, 115, 160, 200, 250, 300, 350, 400);}
			//else							{damage -= by(Difficulty, 45, 120, 230, 333, 444, 555, 666, 777, 888);}
			damage /= 2; 
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_154_DEATH_MINION) {
			//if (GameMode == GM_HARD) {damage -= by(Difficulty, 90, 240, 460, 580, 700, 820, 940, 1060, 1180, 1300);}
			//else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE)) 
			//								{damage -= by(Difficulty, 25,  60, 115, 170, 225, 280, 335, 390, 445);}
			//else							{damage -= by(Difficulty, 45, 120, 230, 340, 450, 560, 670, 780, 890);}
			damage /= 2; 
			LimitToMin(damage, 0);
		}
		else if( is(baseMonsterIndex, BM_233_BLOOD_LORD, BM_297_OROG) ){
			if (Dungeon == DUN_113_THE_MAZE) {
				damage -= damage / 8;
			}
			else {
				damage -= damage / 5;
			}
			if (GameMode == GM_HARD) {damage -= by(Difficulty, 3, 6, 9, 12, 15, 18, 21, 24, 27);}
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE))
											{damage -= by(Difficulty, 1, 2, 3,  4,  5,  6,  7,  8,  9);}
			else							{damage -= by(Difficulty, 2, 4, 6,  8, 10, 12, 14, 16, 18);}
			LimitToMin(damage, 0);
		}
		else if (is(baseMonsterIndex, BM_267_BONE_BLADE, BM_268_BONE_BLADE_2)) {
			damage -= damage / 2;
			if (GameMode == GM_HARD) { damage -= by(Difficulty, 3, 6, 9, 12, 15, 18, 21, 24, 27); }
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE))
			{
				damage -= by(Difficulty, 1, 2, 3, 4, 5, 6, 7, 8, 9);
			}
			else { damage -= by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18); }
			LimitToMin(damage, 0);
		}
		else if (is(baseMonsterIndex, BM_275_BEHEMOTH)) {
			damage -= 6 * damage / 10;
			if (GameMode == GM_HARD) { damage -= by(Difficulty, 3, 6, 9, 12, 15, 18, 21, 24, 27); }
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE))
			{
				damage -= by(Difficulty, 1, 2, 3, 4, 5, 6, 7, 8, 9);
			}
			else { damage -= by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18); }
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_234_STEEL_LORD) {
			damage -= damage / 4;
			if (GameMode == GM_HARD) {damage -= by(Difficulty, 4, 7, 10, 13, 16, 19, 22, 25, 28);}
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE))
											{damage -= by(Difficulty, 2, 3,  4,  5,  6,  7,  8,  9, 10);}
			else							{damage -= by(Difficulty, 3, 5,  7,  9, 11, 13, 15, 17, 19);}
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_252_HEPHASTO) {	
			// dfe
			if (GameMode == GM_HARD) {damage -= by(Difficulty, 90, 360, 720, 1000, 1300, 1600, 1900, 2200, 2500);}
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE))
											{damage -= by(Difficulty, 4, 8, 12, 16, 20, 24, 28, 32, 36);}
			else							{damage -= by(Difficulty, 60, 240, 480, 620, 760, 900, 1040, 1180, 1320);}
			// resistance
			if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE)) {
				damage -= damage / 3;
			}
			else {
				damage -= 2 * damage / 5;
			}
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_274_FLESH_DEVOURER) { 
			damage -= damage / 5; 
		}
		else if (is(baseMonsterIndex, BM_242_REANIMATED_HORROR, BM_271_THORNED_HULK, BM_272_SPIDER_QUEEN)) {
			damage -= damage / 3;
			/*if (GameMode == GM_HARD) { damage -= by(Difficulty, 20, 80, 160, 230, 300, 370, 440, 510, 580); }
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY, GM_NIGHTMARE))
			{
				damage -= by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18);
			}
			else { damage -= by(Difficulty, 10, 40, 80, 120, 160, 200, 240, 280, 320); }*/
			LimitToMin(damage, 0);
		}
		else if (baseMonsterIndex == BM_251_WENDIGO) {
			damage -= 9 * damage / 20;// 45%
		}
		else if (baseMonsterIndex == BM_120_NECROMORPH) {
			if (Dungeon == DUN_23_CRYPT_3) { // necromorph damage resistance increased for dlvls 23 & 24
				damage -= damage / 4;
			}
			else if (Dungeon == DUN_24_CRYPT_4) {
				damage -= damage / 3;
			}
		}
	}
	if (player.gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
		damage *= 2;
	}
	if( monster.newBossId ){
		damage /= 2; // additional -50% damage reduction for all bosses
	}
	else {// code for non-bosses
		if (player.fullClassId == PFC_SHARPSHOOTER && (!bs) && effectType == MT_0_ARROW && damageType == ET_0_PHYSICAL) {// mor: precaution, to prevent it from working for spells/elem.arrows
			damage += PerkValue(PERK_TAKE_DOWN, playerIndex) * (monster.BaseLife / 64) / 100;			
		}
	}
CLASSIC_DAMAGE_MODIFY_SKIP_R:
	// another SMD patch: Uber Bosses of MP won't be exploitable with Bone Spirit now. damage against them is divided by (number of players in game + 3)
	if (bs) {
		if ((baseMonsterIndex == BM_205_PINKY
			|| baseMonsterIndex == BM_195_UBER_BUTCHER
			|| baseMonsterIndex == BM_193_ABATAN) && MaxCountOfPlayersInGame != 1) {
			damage /= NetPlayerCount + 2;
		}
	}
	if( MaxCountOfPlayersInGame != 1 ){ // additional damage reduction according to number of players online
		if (NetPlayerCount == 2) {		damage -= damage / 4; // -25% damage
		}else if (NetPlayerCount == 3) {damage -= 2 * damage / 5; // -40% damage
		}else if (NetPlayerCount == 4) {damage -= damage / 2; // -50% damage
		}
	}
	if( ! (effectType == MT_0_ARROW && damageType != ET_0_PHYSICAL) ){ // not for elemental explosion from magic arrow/weapon
		if (!bs) { // if not bone spirit
			if( player.effectFlag[EA_MORE_DAMAGE_CHANCE] ){
				if (RNG(100) < 30) {
					damage = 15 * damage / 10; // 30% chance for +50% damage now
				}
			}
			if( player.effectFlag[EA_RANDOM_DAMAGE] ){ // Rnd [80-150]% damage now
				int damagePercent = RNG(71) + 80;
				damage = damage * damagePercent / 100;
			}
		}
		if (MaxCountOfPlayersInGame == 1 && HasTrait(playerIndex, TraitId::Fatality)) {
			if (RNG(100) < 10) {
				damage *= 3;
				PlayLocalSound(S_972_CRIT_SOUND_01, player.Row, player.Col);
			}
		}

	}
	if (HasTrait(playerIndex, TraitId::Hunger)) { // trait 'Hunger' reduces all damage dealt to monsters in ranged battle
		damage -= (Difficulty * 24 + Dungeon->level) / 2;
	}
	LimitToMin( damage, effectType == MT_0_ARROW && damageType == ET_0_PHYSICAL ? 1 : 0 ); // for physical arrow min is 1, for other magic min is 0
	if( ! isInternalHP ){
		damage <<= 6;
	}
	int monsterHaveResist = 0;
	
	if( monster.ResistImmune & MR_1_ARCAN && baseMissle.DamageType == ET_3_ARCAN
	 || monster.ResistImmune & MR_2_FIRE  && baseMissle.DamageType == ET_1_FIRE
	 || monster.ResistImmune & MR_3_LIGHT && baseMissle.DamageType == ET_2_LIGHTNING
	 || monster.ResistImmune & MR_9_ACID  && baseMissle.DamageType == ET_4_ACID
	 || monster.ResistImmune & MR_10_COLD && baseMissle.DamageType == ET_6_COLD ){
		monsterHaveResist = 2;
	}
	// if( effectIndex == MI_53_HOLY_BOLT ) // остатки старого кода
	
	if (effectType == MT_0_ARROW && damageType == ET_0_PHYSICAL) {
		// no additional resist
	}
	else {
		if( is(baseMonsterIndex, BM_109_GOLEM, BM_137_UBER_DIABLO, BM_197_CEROMORTH) ){
			monsterHaveResist = (baseMissle.DamageType == ET_4_ACID && overTime) ? 0 : 2;
		}
	}
	
	if( monsterHaveResist > 0 ){
		damage >>= monsterHaveResist;
	}

	

	if( monster.CurAction == A_15_STONE && damageType == ET_0_PHYSICAL && HasTrait( playerIndex, TraitId::Petrifier) ){
		damage /= 2;
	}
	
	if( has(monster.affixes, MAF_ARMORED) && (effectType == MT_0_ARROW && damageType == ET_0_PHYSICAL)) { //33% arrow damage reduce from monster affix
		damage = damage * 67 / 100;
	}
	
	if( has(monster.affixes, MAF_RESISTANT) && is(effectType, MT_1_MAGIC, MT_2_MAGIC_EXPLODE) || (effectType == MT_0_ARROW && damageType != ET_0_PHYSICAL)) { //33% magical damage reduce from monster affix
		damage = damage * 67 / 100;
	}
	
	LimitToMin(damage, 0); // mor: safety measures!

	if (playerIndex == CurrentPlayerIndex && damageType != ET_4_ACID) {
		monster.CurrentLife -= damage;
	} else if (playerIndex == CurrentPlayerIndex) { // Acid attack.  Only caster updates monster.acidDot and sends sync packets.  If N acid missiles are hitting at the same time, this code will execute N times.
		if (!overTime) {
			switch (missile.spellIndex) {
			case PS_59_RANCID_BOLT:
			case PS_60_TOXIC_BALL:
			case PS_61_ACID_NOVA: overTime = 4 * 20; break; // 4 sec
			default:
				switch (baseMissileIndex) {
				case MI_109_ACID_ARROW:
				case MI_64_WEAPON_ELEMENTAL_DAMAGE: overTime = 4 * 20; break; // 4 sec
				default: overTime = 4 * 20; break; // all other acid
				} break;
			}
		}
		if (monster.acidDot < 0) { monster.acidDot = 0; }
		if (monster.overTime <= 0) { monster.overTime = 0; /*monster.prevOverTime = 0;*/ }
		if (monster.overTime == 0) {
			monster.overTime = overTime;
//			monster.prevOverTime = overTime;
		} else {
			double oldDamageTick = (double)monster.acidDot / monster.overTime; // Maybe should use static_cast here.
			double newDamageTick = (double)damage / overTime;
			double averageOverTime = double(monster.acidDot + damage) / (oldDamageTick + newDamageTick);
			if (abs(averageOverTime - overTime) < 1.0) {
				monster.overTime = overTime;
//				monster.prevOverTime = overTime;
			} else {
				monster.overTime = (int)round(averageOverTime); // LHS is ushort.
//				monster.prevOverTime = overTime;
			}
		}
		// In original code, due to monster / missile / stun desync, code may not reach here for every player: acid / monster target would not sync properly.
		// 2 acid attackers could update .dotLastAttacker at the same time: a race condition.  Worst case: monster has green bar, but .dotLastAttacker != CurrentPlayerIndex for every attacker, so its HP won't drop because MonsterDot() won't run.
		// The code below resolves this race condition by choosing the highest DPS caster, and makes sure only the highest DPS caster runs MonsterDot().  All players will agree on the same .dotLastAttacker.  If acid runs out and monster is alive, .dotLastAttacker is reset to -1.
		if (monster.dotLastAttacker != playerIndex && monster.dotLastAttacker != -1) {
			if (Players[monster.dotLastAttacker].dungeon != Dungeon) { // If previous acid attacker is not on the same map, change attacker ID to current caster.
				monster.dotLastAttacker = playerIndex;
			} else if (UpdateAuraDamage % ENGINE_FPS == monsterIndex % ENGINE_FPS && MaxAcidDPS(monsterIndex, monster.dotLastAttacker) < MaxAcidDPS(monsterIndex, playerIndex)) { // Highest DPS guy on the same map becomes the one and only one who runs MonsterDot().  The timer check makes sure expensive function MaxAcidDPS() doesn't spam like acid missile attack.
				monster.dotLastAttacker = playerIndex;
			}
		} else { // If attacker is the current player or not initialized (e.g. player just enters map), it will be the current player.
			monster.dotLastAttacker = playerIndex;
		}
		// Damage and green bar sync once per second for continuous type
		monster.acidDot += damage;
		if (missile.BaseMissileIndex == MI_59_ACID_POOLS) { // If acid damage is continuous, bundle up the damage and sync once per second instead of at every tick, and ensure the same amount of damage output.
			monster.acidDotSum += damage;
			if (UpdateAuraDamage % ENGINE_FPS == monsterIndex % ENGINE_FPS || missile.TimeToLive == 0) { // Aura tick ranges from 0 to 19, and always runs even if char is dead.  Acid sync packets will send out at different ticks based on monsterIndex to avoid packets jam.  Otherwise, send damage sync packet at the last tick before missile disappears.
				SendCmdAcidDotMonster(0, monsterIndex, monster.acidDotSum, monster.dungeon, monster.acidDot, monster.dotLastAttacker); // Caster broadcasts the accumulated change in monster.acidDot to other players once per second for the given monster.  This greatly reduces acid-related packet spam, especially for flasks.
				monster.acidDotSum = 0;
			}
		} else { // If acid damage is instantaneous, like Rancid Bolt / Explosive Acid Flask, then immediately sync damage.  Not sure how long MI_109_ACID_ARROW or MI_64_WEAPON_ELEMENTAL_DAMAGE hits, keep them unchanged for now.
			SendCmdAcidDotMonster(0, monsterIndex, damage, monster.dungeon, monster.acidDot, monster.dotLastAttacker);
		}
	}

	LimitToMin(damage, 0); // mor: safety measures again

	if( (monster.CurrentLife & ~63) > 0 ){
		if( effectType == MT_0_ARROW ){
			CastMonsterCritSplash(monsterIndex, 0, playerIndex );
		}
		if( monster.CurAction == A_15_STONE ){
			if( monsterIndex >= SummonMonsters_Count ){
				if( damageType != ET_4_ACID ){
					DamageMonsterByPlayer( monsterIndex, playerIndex, damage );
				}
			}
			monster.CurAction = A_15_STONE;
		}else{
			if( DoesMissileKnockback(playerIndex, baseMissileIndex) ){
				if (baseMissileIndex != MI_14_HELLFIRE_SEGMENT) {
					KnockbackMonster(monsterIndex, OrientationToTarget(monster.Row, monster.Col, missile.CasterRow, missile.CasterCol), GameMode == GM_HARD ? 1 : 2);
				}else{
					KnockbackMonster(monsterIndex, (missile.direction + 4) % 8, GameMode == GM_HARD ? 1 : 2);//Direction of knockback from Force Wave should be in the direction of the wave front, NOT LOS. This avoids multihits and weird monster movement
				}
			}
			if( monsterIndex >= SummonMonsters_Count ){
				if( damageType != ET_4_ACID ){
					DamageMonsterByPlayer( monsterIndex, playerIndex, damage );
				}
			}
		}
	}else{
		if( effectType == MT_0_ARROW ){
			CastMonsterCritSplash(monsterIndex, 1, playerIndex);
		}
		if( monster.CurAction == A_15_STONE ){
			DrawFloatingDamage( damage >> 6, monster.Row, monster.Col, monsterIndex );
			KillMonsterByPlayer( monsterIndex, playerIndex );
			monster.CurAction = A_15_STONE;
		}else{
			DrawFloatingDamage( damage >> 6, monster.Row, monster.Col, monsterIndex );
			KillMonsterByPlayer( monsterIndex, playerIndex );
		}
	}
	if( ! monster.ActivationCounter ){
		monster.ActivationCounter = 255;
		monster.LastTargetPositionRow = player.Row;
		monster.LastTargetPositionCol = player.Col;
		if( ! (monster.flag & MF_6_FRIENDLY) ){
			MonsterChainActivation( monsterIndex );
		}
	}
	return 1;
}

//----- (00713270) --------------------------------------------------------
void __fastcall MonsterChainActivation( int monsterIndex )// #chain activation
{
	Player& player = Players[CurrentPlayerIndex];
	int activateRadius;
	if( NetPlayerCount > 1 ){
		activateRadius = 5;
	}else{
		switch( player.fullClassId ){
		case PFC_BERSERKER:
		case PFC_EXECUTIONER:
		case PFC_MAGE:
		case PFC_WARLOCK:
		case PFC_ELEMENTALIST:
		case PFC_SAVAGE:
		case PFC_DEMONOLOGIST:
		case PFC_NECROMANCER:
		case PFC_BEASTMASTER:	activateRadius = 5;
		{
			if (MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				activateRadius = 3;
			}
		}
			break;
		case PFC_ASSASSIN:		activateRadius = 2; 
			break;
		case PFC_ROGUE:			activateRadius = 3 - PerkValue(PERK_FROM_THE_SHADOWS, CurrentPlayerIndex);
			break;
		case PFC_TRAPPER:
		case PFC_SCOUT:
		case PFC_SHARPSHOOTER:
		case PFC_ARCHER:		activateRadius = 3; 
			break;
		default:				activateRadius = 4; 
			break;
		}
	}
	if (Dungeon == DUN_48_BLOOD_HIVE || Dungeon == DUN_54_ABANDONED_FANE) {
		activateRadius += 1;
	}
	if (MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::CrowdSeeker)) {
		activateRadius += 2;
	}
	LimitToMin(activateRadius, 2);
	MonsterGreetings( monsterIndex );
	Monster& monster = Monsters[monsterIndex];
	int maxCountToActivate = 8;
	int countToActivate = MonstersCount - SummonMonsters_Count; // было MonstersCount (глюк th1)
	if( countToActivate <= 0 ){
		return;
	}
	for( int* nextMonsterIndex = &MonsterIndexes[ SummonMonsters_Count ]; countToActivate > 0; ++nextMonsterIndex, --countToActivate ){
		Monster& next = Monsters[ *nextMonsterIndex ];
		if( ! next.ActivationCounter
		 && abs( monster.Row - next.Row ) < activateRadius && abs( monster.Col - next.Col ) < activateRadius
		 && ! ( next.flag & MF_6_FRIENDLY ) && *nextMonsterIndex != monsterIndex
		 && CheckCastInSight( next.Row, next.Col, monster.Row, monster.Col ) ){
			next.ActivationCounter = 255;
			next.LastTargetPositionRow = monster.LastTargetPositionRow;
			next.LastTargetPositionCol = monster.LastTargetPositionCol;
			MonsterChainActivation( *nextMonsterIndex );
			if( --maxCountToActivate <= 0 ){
				countToActivate = 0;
			}
		}
	}
}

//----- (0070B1A0) --------------------------------------------------------
void __fastcall Limit_MvP_Ranged_Arrow_ToHit_Chance (int playerIndex, int& realToHitChance, int& Autohit, int& Automiss)
// ограничение макс шанса монстрам попасть - по игроку стрелами
{
	Player& player = Players[playerIndex];
		
	if (is(player.fullClassId, PFC_ROGUE, PFC_MONK, PFC_ASSASSIN)) {
		Autohit = 12;
		if (HasTrait(playerIndex, TraitId::BigFrame)) {
			Autohit += 2;
		}
	}
	else if (player.fullClassId == PFC_DIMACHAERUS) {
		Autohit = 13;
	}
	else if (player.fullClassId == PFC_EXECUTIONER) {
		Autohit = 20 - PerkValue(PERK_AVOIDANCE, playerIndex);
	}
	
	if (HasTrait(playerIndex, TraitId::CrossbowTraining)) {
		Autohit += 5;
	}
	else if (HasTrait(playerIndex, TraitId::Ranger)) {
		Autohit -= 1;
	}
	else if (HasTrait(playerIndex, TraitId::Mamluk)) {
		Autohit = 13;
	}
	else if (HasTrait(playerIndex, TraitId::Crupellarius)) {
		Autohit -= 1;
	}

	// doomwhorl extra check
	if (HasTrait(playerIndex, TraitId::Doomwhorl)) {
		Autohit += 8;
	}

	Autohit -= PerkValue(PERK_ACROBATICS, playerIndex);
	Autohit -= PerkValue(SYNERGY_EXTRA_AVOIDANCE, playerIndex);
	Autohit -= PerkValue(SYNERGY_DEATH_DENIER, playerIndex);
	Autohit -= HasTrait(playerIndex, TraitId::SmallFrame) ? 3 : 0;
	Autohit += PerkValue(SYNERGY_HARDENED_STEEL, playerIndex, 1);

	Automiss -= PerkValue(PERK_ACROBATICS, playerIndex);
	Automiss -= PerkValue(SYNERGY_EXTRA_AVOIDANCE, playerIndex);
	Automiss -= PerkValue(SYNERGY_DAZZLING_LIGHT, playerIndex);
	Automiss -= PerkValue(PERK_SHOT_DEFLECTION, playerIndex);
	LimitToMin(Autohit, 5);

	LimitToMin(Automiss, 25);
	if (GameMode == GM_CLASSIC) {
		Automiss = 100;
		Autohit = 10;
		if (Dungeon->level == 14) Autohit = 20;
		if (Dungeon->level == 15) Autohit = 25;
		if (Dungeon->level == 16) Autohit = 30;
	}
	LimitToRange(realToHitChance, Autohit, Automiss);
}

// ограничение макс шанса монстрам попасть - по игроку магией -------------
void __fastcall Limit_MvP_Ranged_Magical_ToHit_Chance(int playerIndex, int& realToHitChance)
{
	Player& player = Players[playerIndex];
	int Autohit = 15; // minimum 15%
	int Automiss = 95;
	int PFC = player.fullClassId;
	int big_frame_trait_penalty_magic_autohit = HasTrait(playerIndex, TraitId::BigFrame) ? 2 : 0;
	if (PFC == PFC_ROGUE) {
		Autohit = 12 + big_frame_trait_penalty_magic_autohit;
	}	
	else if (PFC == PFC_ASSASSIN) {
		Autohit = 12;
	}
	else if (PFC == PFC_EXECUTIONER) {
		Autohit = 22;
	}
	
	if (HasTrait(playerIndex, TraitId::CrossbowTraining)) {
		Autohit += 5;
	}
	else if (HasTrait(playerIndex, TraitId::Crupellarius)) {
		Autohit -= 1;
	}

	// doomwhorl extra check
	if (HasTrait(playerIndex, TraitId::Doomwhorl)) {
		Autohit += 8;
	}
	Autohit -= PerkValue(PERK_EVASION, playerIndex);// values: 2,4,6
	Automiss -= PerkValue(PERK_EVASION, playerIndex);
	// Automiss -= player.BaseDexterity / 10;
	LimitToMin(Automiss, 35);
	LimitToRange(realToHitChance, Autohit, Automiss);
}

//----- (0042F60F) --------------------------------------------------------
int __fastcall AddDamageReduction(int playerIndex, int damage) // #ard
{
	Player& player = Players[playerIndex];
	switch (player.fullClassId) {
	case PFC_WARRIOR:		
	case PFC_INQUISITOR:
	{
		if (HasTrait(playerIndex, TraitId::Fechtmeister)) {
			damage = damage * 48 / 100;
		}
		else{
			damage = damage * 50 / 100;
		}
	}
	break;
	case PFC_DRUID:
	case PFC_GUARDIAN:		damage = damage * 48 / 100;	break;
	case PFC_TEMPLAR:		damage = damage * 50 / 100;	break;
	case PFC_ARCHER:		
	case PFC_SCOUT:			
	case PFC_SHARPSHOOTER:	
	case PFC_TRAPPER:		damage = damage * 40 / 100;	break;
	case PFC_MAGE:			
	case PFC_ELEMENTALIST:	
	case PFC_DEMONOLOGIST:	
	case PFC_NECROMANCER:	
	case PFC_BEASTMASTER:	
	case PFC_WARLOCK: 
	{
		if (HasTrait(playerIndex, TraitId::Mamluk)) {
			damage = damage * 44 / 100;
		}
		else {
			damage = damage * 50 / 100;
		}
	}	
	break;
	case PFC_SHUGOKI:		damage = damage * 50 / 100;	break;
	case PFC_MONK:
	case PFC_KENSEI:		
	case PFC_SHINOBI:		damage = damage * 46 / 100;	break;
	case PFC_ROGUE:			
	case PFC_ASSASSIN:		
	case PFC_IRON_MAIDEN:	
	case PFC_BOMBARDIER:	damage = damage * 44 / 100;	break;
	case PFC_SAVAGE:		
	case PFC_BERSERKER:		
	case PFC_EXECUTIONER:	
	case PFC_THRAEX:		
	case PFC_MURMILLO: 
	{
		if (HasTrait(playerIndex, TraitId::Crupellarius)) {
			damage = damage * 46 / 100;
		}
		else {
			damage = damage * 50 / 100;
		}
	}		break;
	case PFC_DIMACHAERUS:	damage = damage * 46 / 100;	break;
	case PFC_SECUTOR:		damage = damage * 42 / 100;	break;
	
		/*if (player.ClassID == PC_1_ARCHER || player.ClassID == PC_3_MONK) {
			damage -= damage >> 3;
		}
		else if (player.ClassID == PC_4_ROGUE) {
			damage -= damage >> 4;
		}*/
		
	}
	return damage;
}

void MvP_Ranged_Max_Block_Chance(int playerIndex, int& maxBlockChanceRanged)
{
	Player& player = Players[playerIndex];
	
	switch (player.fullClassId) {
	case PFC_IRON_MAIDEN:
	case PFC_WARRIOR:	maxBlockChanceRanged = 70; break;
	case PFC_ASSASSIN:	maxBlockChanceRanged = 20; break;
	default:			maxBlockChanceRanged = 65; break;
	}
}

//----- (0042F60F) -------------------------------------------------------- MvP Ranged
int __fastcall MvP_Ranged( int playerIndex, int monsterIndex, int distance, int minDamage, int maxDamage, int effectIndex, char isInternalHP, int isLightningOrFirewall, uchar* attackBlockedPtr, uchar* missileReflected /*= 0*/)
{
	if( (uint)(monsterIndex + 1) >= Monsters_Max_Count + 1 || (uint)playerIndex >= PlayersMax_4 ){ return 0; }
	Player& player = Players[ playerIndex ];
	Monster& monster = Monsters[ monsterIndex ];
	BaseMissile& bm = BaseMissiles[ effectIndex ];
	#if GODMODE_ALLOWED
	if( IsGodMode ) return 0;
	#endif
	if( (player.CurLife & ~63) <= 0 || player.notHittableInTransition || player.notHittableFramesAfterTransition > 0
	 || player.activeBuffFlag & BF_1_ETHEREAL && bm.EffectType != MT_0_ARROW ){ // Etherealize now makes user immune to magical attacks
		return 0;
	}
	int toHitRnd = RNG(100);
	int realToHit = 0;
	if( bm.EffectType != MT_0_ARROW ){ // spells
		if( monsterIndex == M1_TRAP ){ // trap
			realToHit = 100 - PerkValue(PERK_LIGHT_STEP, playerIndex);			
		}
		else if (effectIndex == MI_140_EXPLODER) {
			realToHit = 100;
		}
		else{ // monster
			// old formula
			//realToHit = 2 * monster.MonsterLevel + 40 - 2 * player.CharLevel - 2 * distance - PerkValue(PERK_SPELL_DEFLECTION, playerIndex); // MvP magic hit chance
			// new
			int playerAC = 0;
			Player_AC(playerIndex, playerAC);
			realToHit = 95 - (2 * distance) - (playerAC / 50) - PerkValue(PERK_SPELL_DEFLECTION, playerIndex); // MvP magic hit chance
		}
		Limit_MvP_Ranged_Magical_ToHit_Chance(playerIndex, realToHit);
	}else{ // arrow
		int playerAC = 0;
		Player_AC(playerIndex, playerAC);
		int enemyToHit = 0;
		if( monsterIndex == M1_TRAP ){ // trap
			enemyToHit = 250 - distance - PerkValue(PERK_LIGHT_STEP, playerIndex);
			playerAC /= 2;
		}else{ // monster
			int mc = monster.BasePtr->MonsterClass;
			playerAC += (playerAC * player.spiciesAcPercent[mc]) / 100; // AE_SPICIES_AC_PERCENT
			playerAC += player.spiciesAc[mc];// AE_SPICIES_AC
			enemyToHit = monster.ToHit + 2 * (monster.MonsterLevel - player.CharLevel);
			if (GameMode == GM_CLASSIC) {
				enemyToHit += 20 + RNG(21);
				goto SKIPPY;
			}
			enemyToHit += by( Difficulty, 30 - 10 + RNG(21), // variate from -10 to +10 on difficulty 0
										-  55 + RNG(116), // from -55 to + 61
										- 155 + RNG(221), // -155 to + 66
										- 255 + RNG(326),
										- 355 + RNG(431),
										- 455 + RNG(536),
										- 555 + RNG(641),
										- 655 + RNG(746),
										- 755 + RNG(851)
			);
			SKIPPY:
			enemyToHit -= 2 * distance;
		}
		realToHit = enemyToHit - playerAC;
		int Autohit = 14; // minimum 14%
		int Automiss = 95;
		Limit_MvP_Ranged_Arrow_ToHit_Chance(playerIndex, realToHit, Autohit, Automiss);
	}	
	bool canBlock = player.CanBlock;
	if( Item* b = player.OnBodySlots; player.ClassID == PC_5_SAVAGE // can't block missiles with melee 2h weapons
	 && fold( || (not( x.ItemCode, IC_M1_NONE, IC_3_BOW ) && x.EquippedLocation == EL_2_TWO_HANDED), b[IS_LeftHand], b[IS_RightHand] ) ){
		canBlock = false;
	}
	bool checkFor2H = false;
	if ((player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_RightHand].EquippedLocation == EL_2_TWO_HANDED)
		|| (player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_LeftHand].EquippedLocation == EL_2_TWO_HANDED))
		checkFor2H = true;

	if (player.fullClassId == PFC_GUARDIAN || player.fullClassId == PFC_KENSEI) {
		if ((checkFor2H && (player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD))
			|| (checkFor2H && (player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE || player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE)))
			canBlock = false;
	}
	if (player.fullClassId == PFC_SHUGOKI) {
		if ((checkFor2H && (player.OnBodySlots[IS_RightHand].ItemCode == IC_2_AXE || player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE)))
			canBlock = false;
	}
	if (player.fullClassId == PFC_DIMACHAERUS) {
			canBlock = false;
	}

	int blockRnd = not( player.CurAction, PCA_0_STAND, PCA_4_ATTACK ) || !canBlock ? 100 : RNG(100);
	if( isInternalHP == 1 || effectIndex == MI_59_ACID_POOLS ){
		blockRnd = 100;
	}
	int a_rose_with_thorns_trait_penalty_ranged = HasTrait(playerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 3) : 0;
	int blockChance = player.BlockBonus
		+ player.BaseDexterity
		+ player.blockChance
		- a_rose_with_thorns_trait_penalty_ranged
		- PerkValue(SYNERGY_AGGRESSION, playerIndex, 0);
	if (player.fullClassId == PFC_MURMILLO) {
		blockChance += player.CharLevel;
	}
	if( monsterIndex != M1_TRAP ){
		//blockChance += player.CharLevel - (monster.ArmorClass / 2);
		blockChance -= monster.ArmorClass / 2;
	}
	int maxBlockChanceRanged = 0;
	MvP_Ranged_Max_Block_Chance(playerIndex, maxBlockChanceRanged);
	LimitToRange(blockChance, 0, (maxBlockChanceRanged - a_rose_with_thorns_trait_penalty_ranged - PerkValue(SYNERGY_AGGRESSION, playerIndex, 0)));
	
	*attackBlockedPtr = 0;
	bool lithe = true;
	if (HasTrait(playerIndex, TraitId::LitheBuild)) {
		if (RNG(100) > 91) {
			lithe = false;
		}
	}
	if( toHitRnd < realToHit && lithe){
		float resistPercent = player.resistPercent[ bm.DamageType ];
		if( is( effectIndex, MI_1_FIREBOLT, MI_6_FIREBLAST, MI_21_MAGMA_BALL, MI_24_ARCANE_STAR, MI_52_CHARGED_BOLT,
			MI_98_LICH_ORA_BLAST, MI_99_BLUE_BLAST, MI_100_RED_BLAST, MI_101_YELOW_BLAST, 
			MI_102_BLUE_BLAST_2, MI_127_HABIBI, MI_129_SNOW_BOLT, MI_131_ARCANE_STAR_BLUE ) ){
			//resist = (3 * resist / 4 + 1) + RNG(resist / 4 + 1);
			resistPercent = resistPercent * 0.75f + 0.01f + RngPercent(resistPercent * 0.25f + 0.01f);
		}
		else if (effectIndex == MI_57_ACID_MISSILE) {
			int acid_spit_resistance_modifier = 5;
			if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				acid_spit_resistance_modifier = 10;
			}
			else if (GameMode == GM_HARD) {
				acid_spit_resistance_modifier = 3;
			}
			else if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_SPEEDRUN)) {
				acid_spit_resistance_modifier = 7;
			}
			//int random_rez_part = resist / acid_spit_resistance_modifier + 1;
			float random_rez_part = resistPercent / acid_spit_resistance_modifier + 0.01f;
			resistPercent -= random_rez_part;
			resistPercent += RngPercent(random_rez_part);
		}
		else if (effectIndex == MI_59_ACID_POOLS) {
			if (RNG(100) < 40) {
				return 0; // acid pools have 40% chance to miss hitting a player
			}
		}
		int damage = 0;
		int rebound_dmg = 0;
		if( effectIndex == MI_63_BONE_SPIRIT ){
			damage = player.CurLife / 8;
		}else{
			if( isInternalHP ){
				damage = RngFromRange(minDamage, maxDamage);
			}else{
				damage = (minDamage << 6) + RNG((maxDamage - minDamage + 1) << 6);
			}
			rebound_dmg = damage;
			if (GameMode != GM_CLASSIC) { // ARD skipped in classic
				damage = AddDamageReduction(playerIndex, damage);
				if (bm.EffectType == MT_0_ARROW) {
					damage += damage / 2; // if arrow, ARD reduction is not 50%, but 25% (make arrow damage resistance great again!)
				}
			}
			if (HasTrait(playerIndex, TraitId::Doomwhorl)) {
				damage -= damage / 5;
			}
			damage += player.DamageFromEnemy << 6;
			if (monsterIndex != M1_TRAP) {
				damage += player.spiciesDfe[monster.BasePtr->MonsterClass] << 6;
			}
			if( bm.EffectType == MT_0_ARROW ){
                float reflectPercent = ResistPercentArrow(playerIndex);
                int reflectDamagebyPlayer = int((double)damage * reflectPercent);
                damage -= reflectDamagebyPlayer;
            }
			/*if( MaxCountOfPlayersInGame != 1 && (Difficulty == 0 && player.CharLevel >= 32 || Difficulty == 1 && player.CharLevel >= 46) ){ // <-- executioner mode turned off
				damage = 300 << 6; // executioner mode (for high level chars on low difficulties
			}else{*/
			if (bm.EffectType == MT_0_ARROW) {
				LimitToMin(damage, (Dungeon->level / 2 + by(Difficulty, 0, 12, 24, 36, 48, 60, 72, 84, 96)) << 6);
			}
			else { // bm.EffectType >= MT_1_MAGIC
				int minDamage = 0;
				switch (effectIndex) {
				case MI_59_ACID_POOLS:
					minDamage = Dungeon->level / 10 + by(Difficulty, 0, 2, 4, 5, 6, 7, 8, 9, 10);
					break;
				case MI_48_INCINERATE_SEGMENT:
					minDamage = Dungeon->level / 4 + by(Difficulty, 0, 5, 10, 13, 16, 19, 22, 25, 28); // multi-frame spell (threshold higher because Incinerate is less dangerous than Lite/Flash)
					break;
				case MI_8_LIGHTING_SEGMENT:
				case MI_23_MONSTER_LIGHTING_SEGMENT:
				case MI_11_FLASH_FR:
				case MI_12_FLASH_BACK:
					minDamage = Dungeon->level / 6 + by(Difficulty, 0, 4, 8, 10, 12, 14, 16, 18, 20); // multi-frame spells
					break;
				case MI_22_MONSTER_LIGHTNING:
					minDamage = 1 + Dungeon->level / 8 + Difficulty * 3; // advocate lightning?
					break;
				default:
					minDamage = Dungeon->level / 4 + by(Difficulty, 0, 6, 12, 17, 22, 27, 32, 37, 42); // other spells (missile type) need higher minThreshold
					break;
				}
				LimitToMin(minDamage, 1);
				LimitToMin(damage, minDamage << 6);
			}
			//}
		}
		if( PerkValue(SYNERGY_REBOUND, playerIndex, 0) > RNG(100) && bm.EffectType == MT_0_ARROW && bm.DamageType == ET_0_PHYSICAL ){
			int reflectedArrowDamage = (PerkValue(SYNERGY_REBOUND, playerIndex, 1) + 100) * rebound_dmg / 100; // place for reflected arrow damage formula
			auto dir = OrientationToTarget(player.Row, player.Col, monster.Row, monster.Col);
			// see PM_DoRangedAttack and CastSpecialArrowSpell for used service values [0,1,2,5,6,7]
			int casterIndex = playerIndex * SummonMonstersPerPlayer_Count;
			Monsters[casterIndex].flag |= MF_6_FRIENDLY; // fix pseudo caster to correct MvM_Ranged logic for reflected arrow
			CastMissile(player.Row, player.Col, monster.Row, monster.Col, dir, bm.BaseMissileIndex, CT_1_MONSTER_AND_TRAP, casterIndex, reflectedArrowDamage, 0, 0, {0}, 0, MSF_REFLECTED);
			*missileReflected = 1;
			PlayLocalSound(RngFromRange(1627, 1629), player.Row, player.Col);
		}
		// Checks block chance. Additionally, always can block arrows (including elemental arrows)
		// Spells blocked if block chance is success AND resist+(0-40) < 20. 
		
		//else if (blockRnd < blockChance && (bm.EffectType == MT_0_ARROW || resist + RNG(41) < 20)) {
		else if (blockRnd < blockChance && ((bm.EffectType == MT_0_ARROW && bm.DamageType == ET_0_PHYSICAL) || int(resistPercent * 100) + RNG(41) < 20)) {
			*attackBlockedPtr = 1;
			int dir = monsterIndex == M1_TRAP ? player.dir : OrientationToTarget( player.Row, player.Col, monster.Row, monster.Col );
			StartPlayerBlock( playerIndex, dir );
		}else{
			//if( resist > 0 ) damage -= damage * resist / 100;
			if( resistPercent > 0 ) damage -= int(double(damage) * resistPercent);

			if( monsterIndex == M1_TRAP ){ // Sixth sense + Light-Fingered
				if (PerkValue(SYNERGY_LIGHT_FINGERED, playerIndex, 0) > 0) {
					damage -= damage * PerkValue(SYNERGY_LIGHT_FINGERED, playerIndex, 0) / 100;
				}
				else {
					damage -= damage * PerkValue(PERK_SIXTH_SENSE, playerIndex) / 100;
				}
			}

			if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				damage = damage * 80 / 100; // -20% easy mode
				if (player.gameChanger & BIT(GC_4_EFFORTLESS) && MaxCountOfPlayersInGame == 1) {
					damage /= 2;
				}
			}
			else if (GameMode == GM_HARD) {
				damage = damage * 120 / 100; // +20% hard mode
				if (player.gameChanger & BIT(GC_6_IMPOSSIBLE)) {
					damage *= 2;
				}
			}
			else if (GameMode == GM_NIGHTMARE && RNG(100) < 3) {
				damage *= 2;
				PlayLocalSound(S_972_CRIT_SOUND_01, player.Row, player.Col);
			}
			if (MaxCountOfPlayersInGame == 1 && HasTrait(playerIndex, TraitId::Fatality)) {
				if (RNG(100) < 10) {
					damage *= 3;
					PlayLocalSound(S_972_CRIT_SOUND_01, player.Row, player.Col);
				}
			}
			// executioning mode:
			if (
				(HasTrait(playerIndex, TraitId::Psion) && player.fullClassId != PFC_MAGE)
				|| (HasTrait(playerIndex, TraitId::Cleric) && player.fullClassId != PFC_MAGE)
				|| (HasTrait(playerIndex, TraitId::Hydramancer) && player.fullClassId != PFC_WARLOCK)
				|| (HasTrait(playerIndex, TraitId::LordOfTheRings) && player.fullClassId != PFC_MONK)
				) {
				if (RNG(100) < 10) {
					damage *= 20;
				}
			}

			// "Thick-Skinned" trait extra defense (-80% dmg) from "Critical Defense" perk:
			if (RNG(100) < PerkValue(PERK_CRITICAL_DEFENSE, playerIndex)) {
				damage /= 5;
			}

			if (RNG(100) < PerkValue(SYNERGY_AUXILIARY_ARMOR, playerIndex, 0)) {
				damage = damage * (100 - PerkValue(SYNERGY_AUXILIARY_ARMOR, playerIndex, 1)) / 100;
			}

			if (RNG(100) < PerkValue(SYNERGY_LAST_STAND, playerIndex, 0) && (player.CurLife < PerkValue(SYNERGY_LAST_STAND, playerIndex, 2) * player.MaxCurLife / 100)) {
				damage = damage * (100 - PerkValue(SYNERGY_LAST_STAND, playerIndex, 1)) / 100;
			}

			if (player.fullClassId == PFC_BERSERKER) {
				damage = damage * (100 - PerkValue(SYNERGY_INVINCIBLE, playerIndex)) / 100;
			}

			// "Emerge Unscathed" perk nullifies ranged damage from monsters if the roll is successful:
			if (RNG(100) < PerkValue(PERK_EMERGE_UNSCATHED, playerIndex)) {
				damage = 0;
			}

			if( playerIndex == CurrentPlayerIndex ){
				player.CurLife -= damage;
				player.BaseLife -= damage;
			}
			if( player.CurLife > player.MaxCurLife ){
				player.CurLife = player.MaxCurLife;
				player.BaseLife = player.MaxBaseLife;
			}
			player.lastAttacker = 1 + PlayersMax_4 + monsterIndex;
			if( (player.CurLife & ~63) <= 0 ){
			    TryToDie( playerIndex, isLightningOrFirewall );
            }else{
                StartPlayerHit( playerIndex, damage, 0 );
            }
		}
		return 1;
	}
	return 0;
}

// ------- th3 ------------
void Player_AC(int playerIndex, int& playerAC) {
	Player& player = Players[playerIndex];
	playerAC = player.ACFromClass + player.ACFromItems;
	if (HasTrait(playerIndex, TraitId::Pyromaniac)) {
		// nothing more happens
	}
	else {
		int basic_AC_divider = 5;
		if (HasTrait(playerIndex, TraitId::SmallFrame)) {
			basic_AC_divider = 3;
		}
		playerAC += player.CurDexterity / basic_AC_divider;
	}
	//return playerAC;
}

//----- (0042FA86) --------------------------------------------------------
void __fastcall SetMissileDirection( int missileIndex, int direction )
{
	Missile& missile = Missiles[missileIndex];
	missile.direction = direction;
	SetMissileSprite(missileIndex, missile.spriteIndex);
}

//----- (0042FAA0) --------------------------------------------------------
void __fastcall SetMissileSprite( int missileIndex, uint spriteIndex )
{

	if( spriteIndex == MIS_6_PORTAL && (BlueTownPortal || Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) || GameMode == GM_CLASSIC) ){
		spriteIndex = MIS_15_EMPTY;
	}
	Missile& missile = Missiles[missileIndex];
	int orient = missile.direction;
	missile.spriteIndex = (MISSILE_SPRITE) spriteIndex;
	if( missile.spriteIndex < MIS_count ){
		MissileSprite& missileSprite = MissileSprites[spriteIndex];
		missile.spriteFileFlags = missileSprite.fileFlag;
		missile.curAnimationPtr = missileSprite.anim[orient];
		missile.animationDelayFrameCount = missileSprite.animDelay[orient];
		missile.framesCount = missileSprite.animFrames[orient];
		missile.animWidth = missileSprite.animWidth[orient];
		missile.animOffset = missileSprite.animOffset[orient];
		missile.AnimationDelayIndex = 0;
		missile.FrameIndex = 1;
	}
}

//----- (th2) -------------------------------------------------------------
void ColorMissileSprite(uint spriteIndex, uchar* table)
{
	MissileSprite& missileSprite = MissileSprites[spriteIndex];
	for( int dir = 0; dir < missileSprite.animDirectionsCount; ++dir ){
		ColorSpriteAnimation(missileSprite.anim[dir], table, missileSprite.animFrames[dir]);
	}
}

//----- (0042FB24) --------------------------------------------------------
void __fastcall LoadMissileSprite( uint spriteIndex )
{
	char fileName[256];
	MissileSprite& missileSprite = MissileSprites[spriteIndex];

	if( missileSprite.fileFlag & MFF_MULTI ){// вся анимация в одном файле
		// TODO: никаких проверок на то, загружен уже файл или нет
		sprintf(fileName, strchr(missileSprite.name, '\\') ? "%s" : "Missiles\\%s.CL2", missileSprite.name);
		char* file = (char*)LoadFile(fileName);
		for( int dir = 0; dir < missileSprite.animDirectionsCount; ++dir ){
			// таблица в начале файла это оффсеты от начала файла на начало анимации
			int curAnimationFileOffset = ((int*)file)[dir];
			missileSprite.anim[dir] = &file[curAnimationFileOffset];
		}
	}else{
		char* format = missileSprite.animDirectionsCount == 1 ? "Missiles\\%s.CL2" : "Missiles\\%s%i.CL2";
		for( int dir = 0; dir < missileSprite.animDirectionsCount; ++dir ){
			sprintf(fileName, format, missileSprite.name, dir + 1);
			if( ! missileSprite.anim[dir] ){
				missileSprite.anim[dir] = (char*)LoadFile(fileName);
			}
		}
	}
	char* trn = 0;
	switch( spriteIndex ){
	case MIS_59_HARROW    or MIS_60_HARROW_EXPLO:   trn = "X\\Missile_TRNs\\Fire-Arrow-to-Holy-Arrow5.trn";       break;
	case MIS_61_ARCARROW  or MIS_62_ARCARROW_EXPLO: trn = "X\\Missile_TRNs\\LightningArrow-to-CrimsonArrow1.trn"; break;
	case MIS_63_ACIDARROW or MIS_64_ACIDARROWEXPLO: trn = "X\\Missile_TRNS\\Fire-Arrow-to-Acid-Arrow1.trn";       break;
	case MIS_74_COLDARROW or MIS_73_COLDEXPLO:      trn = "X\\Missile_TRNS\\Fire-Arrow-to-Cold-Arrow2.trn";       break;
	case MIS_82_TOXIC_AURA:                         trn = "X\\Missile_TRNS\\Fire-Arrow-to-Acid-Arrow1.trn";       break;
	case MIS_81_COLD_GUARD:                         trn = "X\\Missile_TRNS\\GuardianOfIce.trn";                   break;
	case MIS_83_THROWING_KNIFE:                     trn = "X\\Missile_TRNs\\krull_steel.trn";                     break;
	}
	if( trn ){
		uchar* colorTable = (uchar*)LoadFile(trn);
		ColorMissileSprite(spriteIndex, colorTable);
		FreeMemZero(colorTable);
	}
}

//----- (0042FC12) --------------------------------------------------------
void LoadMissileGFX() // InitMissileGFX
{
	for( int misIndex = 0; misIndex < MIS_count; ++misIndex ){
		MissileSprite& missileSprite = MissileSprites[misIndex];
		if( !(missileSprite.fileFlag & MFF_MONST_ONLY) ){
			LoadMissileSprite(misIndex);
		}
	}
}

//----- (0042FC3F) --------------------------------------------------------
void FreePlayerMissileSprites()
{
	for( int misIndex = 0; misIndex < MIS_count; ++misIndex ){
		MissileSprite& missileSprite = MissileSprites[misIndex];
		if( !(missileSprite.fileFlag & MFF_MONST_ONLY) ){
			FreeMissileSprite(misIndex);
		}
	}
}

//----- (0042FC6D) --------------------------------------------------------
void __fastcall FreeMissileSprite( uint spriteIndex )
{
	MissileSprite& missileSprite = MissileSprites[spriteIndex];

	if( missileSprite.fileFlag & MFF_MULTI ){// вся анимация в одном файле
		if( missileSprite.anim[0] ){
			int headerSize = missileSprite.animDirectionsCount * sizeof (int);
			void* filePtr = (void*)(missileSprite.anim[0] - headerSize);
			FreeMem(filePtr);
			missileSprite.anim[0] = 0;
		}
	}else{
		// в простой анимации, каждая анимация находится с самого начала файла
		// просто освобождаем каждую. Числом AnimDirectionsCount
		for( int orientation = 0; orientation < missileSprite.animDirectionsCount; ++orientation ){
			if( missileSprite.anim[orientation] ){
				FreeMemZero(missileSprite.anim[orientation]);
			}
		}
	}
}

//----- (0042FCD6) --------------------------------------------------------
void FreeNotPlayerMissileSprites()
{
	for( int misIndex = 0; misIndex < MIS_count; ++misIndex ){
		MissileSprite& missileSprite = MissileSprites[misIndex];
		if( missileSprite.fileFlag & MFF_MONST_ONLY ){
			FreeMissileSprite(misIndex);
		}
	}
}

//----- (0042FD04) --------------------------------------------------------
void InitMissiles()
{
	Player& player = Players[CurrentPlayerIndex];
	player.activeBuffFlag = 0;
	BattleTrance = false;
	InitBerserk = true;
	CalcCharParams(CurrentPlayerIndex, 1);
	if( player.Infravision == 1 ){
		for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; ++missileIndexIndex ){
			int missileIndex = MissileIndexes[missileIndexIndex];
			Missile& missile = Missiles[missileIndex];
			if( missile.BaseMissileIndex == MI_39_INFRAVISION && missile.CasterIndex == CurrentPlayerIndex ){
				CalcCharParams(missile.CasterIndex, 1);
			}
		}
	}
	MissileAmount = 0;
	memset(MissileIndexes, 0, sizeof( MissileIndexes ));
	for( int i = 0; i < MissilesMax; ++i ){
		MissileIndexList[i] = i;
	}
	for( int r = 0; r < FineMap_112; ++r ) for( int c = 0; c < FineMap_112; ++c ){
		FlagMap[r][c] &= ~CF_1_SPELL_CAST;
	}
	player.CountOfReflectCharges = 0;
}

//----- (0042FF33) --------------------------------------------------------
void __fastcall CastAbyssEnterExplode(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	CastMissile(80, 62, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0, missile.spellIndex );
	CastMissile(80, 63, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0, missile.spellIndex );
	CastMissile(81, 62, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0, missile.spellIndex );
	CastMissile(81, 63, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0, missile.spellIndex );
	missile.IsDeleted = true;
}

//----- (0042FFC8) --------------------------------------------------------
void __fastcall CastRuneOfFire(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_47_NO_SPELL);// TODO: руна не используется, потому заменена
	}
	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){ // TODO: try remove check, moved to CursorSkillApply
		int runeActivationEffect = MI_94_BIG_FIRE_EX;
		missile.value[0] = runeActivationEffect;
		missile.IsDeleted = false;
		missile.LightIndex = AddLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//----- (00430048) --------------------------------------------------------
bool __fastcall FoundCellForTargetInRadius( int missileIndex, int* targetRow, int* targetCol, int checkRadius )
{
	Missile& missile = Missiles[missileIndex];
	
	if( checkRadius > 19 ){
		checkRadius = 19;
	}else if( checkRadius <= 0 ){
		return false;
	}

	for( int circleOffsetIndex = 1; circleOffsetIndex < checkRadius; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = *targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = *targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( To112(row, col) && !TileBlockWalking[FineMap[ row ][ col ]] && !ObjectsMap[ row ][ col ] /*&& !MissilesMap[ row ][ col ]*/ ){ // TODO: check why here was check fom missile
				if( missileIndex >= 0 ){
					missile.Row = row;
					missile.Col = col;
					*targetRow = row;
					*targetCol = col;
				}
				return true;
			}
		}
	}
	return false;
}

//----- (0043013E) --------------------------------------------------------
void __fastcall CastRuneOfLighting(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_48_RUNE_OF_LIGHT);
	}
	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){ // TODO: try remove check, moved to CursorSkillApply
        int runeActivationEffect = MI_4_LIGHTNING_NOVA_SEGMENT;
        missile.value[0] = runeActivationEffect;
		missile.IsDeleted = false;
		missile.LightIndex = AddLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//----- (004301BE) --------------------------------------------------------
void __fastcall CastRuneOfGreatLightning(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_49_RUNE_OF_NOVA);
	}
	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){ // TODO: try remove check, moved to CursorSkillApply
        int runeActivationEffect = MI_42_LIGHTNING_NOVA;
        missile.value[0] = runeActivationEffect;
		missile.IsDeleted = false;
		missile.LightIndex = AddLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//----- (0043023E) --------------------------------------------------------
void __fastcall CastRuneOfImmolation(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_50_RUNE_OF_IMMOLATION);
	}
	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){ // TODO: try remove check, moved to CursorSkillApply
        int runeActivationEffect = MI_72_FIRE_NOVA;
        missile.value[0] = runeActivationEffect;
		missile.IsDeleted = false;
		missile.LightIndex = AddLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//----- (004302BE) --------------------------------------------------------
void __fastcall CastRuneOfStone(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_51_EMPTY);// не используется
	}
	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){ // TODO: try remove check, moved to CursorSkillApply
        int runeActivationEffect = MI_30_STONE_CURSE;
        missile.value[0] = runeActivationEffect;
		missile.IsDeleted = false;
		missile.LightIndex = AddLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//----- (0043033E) --------------------------------------------------------
void __fastcall CastReflect(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	if( casterIndex >= 0 ){
		int spellLevel = missile.SpellLevel;
		if( !spellLevel ){
			spellLevel = 2;// C реликта при незнании заклинания уровень считается 2м
		}
		int armadillo_trait_benefit = HasTrait(casterIndex, TraitId::Armadillo) ? (player.CharLevel / 13 + 2) : 0;
		player.CountOfReflectCharges += 1 + (player.BaseVitality / 50) + armadillo_trait_benefit + PerkValue(SYNERGY_IRON_BULWARK, casterIndex);
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_16_REFLECT);
	}
	// 0043038E
	missile.TimeToLive = 0;
	missile.IsDeleted = false;
}

// отключено в 1.150c RC8
//----- (004303A8) --------------------------------------------------------
void __fastcall CastBerserk(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 )	{
		for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
			int circleOffset = CircleOffsets[circleOffsetIndex];
			int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
			for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
				int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
				int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
				int cell = row * 112 + col;
				if( To112(row, col) ){
					int monsterNumber = MonsterMap[ row ][ col ];
					int monsterIndex = abs(monsterNumber) - 1;
					Monster& monster = Monsters[monsterIndex];
					int monsterCurAction = monster.CurAction;
					int monsterResist = monster.ResistImmune;
					if( monsterIndex >= SummonMonsters_Count
						&& !monster.newBossId 
						&& monster.ai != AI_27_Diablo 
						&& monsterCurAction != A_8_FADE_IN && monsterCurAction != A_9_FADE_OUT && monster.CurAction != A_14_CHARGE 
						&& !(monsterResist & MR_4_IM_ARCAN) && (monsterResist & MR_1_ARCAN) == 0 || (monsterResist & MR_1_ARCAN) && !RNG(2) ){

							int spellLevel = PlayerSpellLevel(casterIndex, PS_44_BERSERK);
							monster.flag |= MF_6_FRIENDLY | MF_12_BERSERK;
							monster.MinDamage = ftol(((double)(RNG(10) + 20) * 0.01 + 1.0) * (double)monster.MinDamage + spellLevel);
							monster.MaxDamage = ftol(((double)(RNG(10) + 20) * 0.01 + 1.0) * (double)monster.MaxDamage + spellLevel);
							monster.SecondMinDamage = ftol(((double)(RNG(10) + 20) * 0.01 + 1.0) * (double)monster.SecondMinDamage	+ spellLevel);
							monster.SecondMaxDamage = ftol(((double)(RNG(10) + 20) * 0.01 + 1.0) * (double)monster.SecondMaxDamage + spellLevel);
							// в 1.216 эта функция не только отключена, но и сломана, и нижний код отсутсвует как занятый куском другой функции
							monster.MaxDamage = LimitPvMMeleeDamage(casterIndex, monster.MaxDamage);
							int lightness;
							if( Dungeon->graphType != DT_6_ABYSS ){
								lightness = 3;
							}else{
								lightness = 9;
							}
							Monsters[monsterIndex].LightIndex = AddLight(monster.Row, monster.Col, lightness);
							MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_44_BERSERK);
							goto BREAK_LABEL;
					}
				}
			}
		}
BREAK_LABEL:;

	}
	missile.TimeToLive = 0;
	missile.IsDeleted = true;
}

//----- (00430624) --------------------------------------------------------
void __fastcall CastSpawnMeatBall(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 8);
	missile.TimeToLive = 9;
	missile.value[0] = casterDirection;
	PutMissile(missileIndex);
}

//----- (00430661) --------------------------------------------------------
void __fastcall SetMissileMovementDelta( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int missileSpeed )
{
	Missile& missile = Missiles[missileIndex];
	if( BattleTrance && missile.CasterType != CT_0_PLAYER ){
		missileSpeed /= 2;
	}
	if( casterRow == targetRow && casterCol == targetCol ){
		int casterDirection = 0;
		if( missile.CasterType == CT_1_MONSTER_AND_TRAP && (uint)missile.CasterIndex < Monsters_Max_Count ){
			casterDirection = Monsters[missile.CasterIndex].Orientation;
		}else if( missile.CasterType == CT_0_PLAYER && (uint)missile.CasterIndex < PlayersMax_4 ){
			casterDirection = Players[missile.CasterIndex].dir;
		}
		casterDirection &= 7;
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	int deltaRow = targetRow - casterRow;
	int deltaCol = targetCol - casterCol;
	double y = (double)(( deltaRow - deltaCol ) << 21);
	double x = (double)(( deltaRow + deltaCol ) << 21);
	double range = sqrt(y * y + x * x);
	missile.DeltaY = ftol((double)(missileSpeed << 16) * y / range);
	missile.DeltaX = ftol((double)(missileSpeed << 15) * x / range);
}

//----- (004306FA) --------------------------------------------------------
void __fastcall PutMissile( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	int row = missile.Row;
	int col = missile.Col;
	if( ! To112(row, col) ){
		missile.IsDeleted = 1;
	}
	if( ! missile.IsDeleted ){
		// добавил работу с этой функцией в MotionInterpolationBegin
		FlagMap[ row ][ col ] |= CF_1_SPELL_CAST;
		if( MissilesMap[ row ][ col ] == 0 ){
			MissilesMap[ row ][ col ] = missileIndex + 1;
		}else{
			MissilesMap[ row ][ col ] = Missile_Many;
		}
		if( IsMainDraw && missile.MayBeIsExploding ){
			MayBeExplodingMagicFlag = true;
		}
	}
}

//----- (0043076A) --------------------------------------------------------
void __fastcall CastJester(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	int spellEffectIndex;
	switch( RNG(10) ){
		case 0:
		case 1:		spellEffectIndex = MI_1_FIREBOLT;		break;
		case 2:		spellEffectIndex = MI_6_FIREBLAST;		break;
		case 3:		spellEffectIndex = MI_38_FIRE_WALL;		break;
		case 4:		spellEffectIndex = MI_2_HYDRA;		break;
		case 5:		spellEffectIndex = MI_15_ARCANE_NOVA;		break;
		case 6:
			spellEffectIndex = MI_10_TOWN_PORTAL;
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_7_TOWN_PORTAL);
			break;
		case 7:		spellEffectIndex = MI_26_TELEPORT;		break;
		case 8:		spellEffectIndex = MI_44_APOCALYPSE;		break;
		case 9:		spellEffectIndex = MI_30_STONE_CURSE;		break;
		default:	spellEffectIndex = MI_1_FIREBOLT;		break;
	}
	// 004307BE
	Missile& missile = Missiles[missileIndex];
	CastMissile(casterRow, casterCol, targetRow, targetCol, casterDirection, spellEffectIndex, missile.CasterType, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex );
	missile.IsDeleted = true;
	missile.TimeToLive = 0;
}

//----- (0043082D) --------------------------------------------------------
void __fastcall CastSpell82_StealBottlesFromBelt(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.CasterIndex = casterIndex;
	bool soundPlayed = false;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 3; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = casterRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = casterCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( To112(row, col) && PlayerMap[ row ][ col ] ){
				int playerIndex = abs(PlayerMap[ row ][ col ]) - 1;
				Player& player = Players[playerIndex];
				Item& itemOnCursor = player.ItemOnCursor;
				// 004308DD
				for( int beltIndex = 0; beltIndex < IS_8_Belt_Count; beltIndex++ ){
					Item& beltItem = player.BeltInventory[beltIndex];
					int beltItemMagicCode = beltItem.MagicCode;
					int newMagicCode = -1;
					if( beltItem.ItemCode != IC_0_OTHER ){
						goto checkMagicCode; // ну такой вот он, оригинальный код
					}
					if( !RNG(2) ){
						continue;
					}
					if( beltItemMagicCode == MC_3_POTION_OF_HEALING || beltItemMagicCode == MC_6_POTION_OF_MANA ){
						ClearBeltSlot(playerIndex, beltIndex);
						goto stealSound;
					}
					switch (beltItemMagicCode ){
						case MC_2_POTION_OF_FULL_HEALING:	newMagicCode = MC_3_POTION_OF_HEALING;	break;
						case MC_7_POTION_OF_FULL_MANA:	newMagicCode = MC_6_POTION_OF_MANA;	break;
						case MC_18_POTION_OF_REJUVENATION:
							if( RNG(2) ){
								newMagicCode = MC_6_POTION_OF_MANA;
							}else{
								newMagicCode = MC_3_POTION_OF_HEALING;
							}
							break;
						case MC_19_POTION_OF_FULL_REJUVENATION:
							{
								int randFactor = RNG(3);
								if( randFactor == 0 ){
									newMagicCode = MC_7_POTION_OF_FULL_MANA;
								}else if( randFactor == 1 ){
									newMagicCode = MC_2_POTION_OF_FULL_HEALING;
								}else{
									newMagicCode = MC_18_POTION_OF_REJUVENATION;
								}
							}
							break;
					}
					checkMagicCode:
					// ловушка для бутылок, добавить сохранение/восстановление курсорной вещи как переключении оружия
					if( newMagicCode != -1 ){
						int baseItemIndex = FindBaseItemWithMagicCode(newMagicCode);
						if( baseItemIndex != -1 ){
							memcpy( &TempItem, &itemOnCursor, sizeof( TempItem ) );
							SetPlayerHandItem(itemOnCursor, baseItemIndex);
							itemOnCursor.MakeSeed();
							itemOnCursor.IsReqMet = 1;
							memcpy(&beltItem, &itemOnCursor, sizeof( itemOnCursor ));
							memcpy( &itemOnCursor, &TempItem, sizeof( TempItem ) );
						}
					}
					stealSound:
					if( !soundPlayed ){
						PlayLocalSound(S_21_I_URNPOP2, row, col);
						soundPlayed = true;
					}
				}
			}
		}
	}
	missile.TimeToLive = 0;
	missile.IsDeleted = true;
}

//----- (00430A78) --------------------------------------------------------
void __fastcall CastManaTrap(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.CasterIndex = casterIndex;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 3; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = casterRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = casterCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( To112(row, col) && PlayerMap[ row ][ col ] ){
				int playerIndex = abs(PlayerMap[ row ][ col ]) - 1;
				Player& player = Players[playerIndex];
				player.CurMana = 0;
				player.BaseMana = player.CurMana + player.MaxBaseMana - player.MaxCurMana;
				RecalcPlayer(playerIndex, 0);
				PlayLocalSound(S_80_CAST10, row, col);
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:
	missile.TimeToLive = 0;
	missile.IsDeleted = true;
}

//----- (00430B89) --------------------------------------------------------
void __fastcall CastMagicArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	int arrowSpeed = 24;
	if( casterType == CT_0_PLAYER && missile.BaseMissileIndex < MI_139_THROWING_KNIFE ){
		arrowSpeed = GetPlayerArrowSpeed(casterIndex);
	}
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, arrowSpeed);
	if( missile.BaseMissileIndex != MI_139_THROWING_KNIFE ){
		SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	}else{
		missile.FrameIndex = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol) + 1;
	}
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 5);
}

char DirTable_16[16][16] = {
	{	99,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},
	{	4,	2,	1,	1,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},
	{	4,	3,	2,	1,		1,	1,	1,	1,		0,	0,	0,	0,		0,	0,	0,	0,	},
	{	4,	3,	3,	2,		2,	1,	1,	1,		1,	1,	1,	1,		0,	0,	0,	0,	},

	{	4,	4,	3,	2,		2,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},
	{	4,	4,	3,	3,		2,	2,	2,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},
	{	4,	4,	3,	3,		2,	2,	2,	2,		2,	1,	1,	1,		1,	1,	1,	1,	},
	{	4,	4,	3,	3,		3,	3,	2,	2,		2,	2,	1,	1,		1,	1,	1,	1,	},

	{	4,	4,	4,	3,		3,	3,	2,	2,		2,	2,	2,	1,		1,	1,	1,	1,	},
	{	4,	4,	4,	3,		3,	3,	3,	2,		2,	2,	2,	2,		1,	1,	1,	1,	},
	{	4,	4,	4,	3,		3,	3,	3,	3,		2,	2,	2,	2,		2,	2,	1,	1,	},
	{	4,	4,	4,	3,		3,	3,	3,	3,		3,	2,	2,	2,		2,	2,	2,	1,	},

	{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	2,	2,		2,	2,	2,	2,	},
	{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	2,	2,		2,	2,	2,	2,	},
	{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	3,	2,		2,	2,	2,	2,	},
	{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	3,	3,		2,	2,	2,	2,	},
};
uchar FirstQuadrantDirections_16 [5] = { 14, 15,  0,  1,  2 };
uchar FourthQuadrantDirections_16[5] = { 14, 13, 12, 11, 10 };
uchar SecondQuadrantDirections_16[5] = {  6,  5,  4,  3,  2 };
uchar ThirdQuadrantDirections_16 [5] = {  6,  7,  8,  9, 10 };
// в тх2 заменена на универсальную, хоть и более медленную SelectMissileDirection
//----- (00430C93) --------------------------------------------------------
int __fastcall SelectMissileDirectionFrom16(int casterRow, int casterCol, int targetRow, int targetCol)
{
	int deltaRow = abs(targetRow - casterRow);
	LimitToMax(deltaRow, 15);
	int deltaCol = abs(targetCol - casterCol);
	LimitToMax(deltaCol, 15);
	int directionIndex = DirTable_16[deltaCol][deltaRow];
	if( casterRow <= targetRow ){
		if( casterCol <= targetCol ){
			return FirstQuadrantDirections_16[directionIndex];
		}else{
			return FourthQuadrantDirections_16[directionIndex];
		}
	}else{
		if( casterCol <= targetCol ){
			return SecondQuadrantDirections_16[directionIndex];
		}else{
			return ThirdQuadrantDirections_16[directionIndex];
		}
	}
}

//----- (th2) -------------------------------------------------------------
int GetPlayerArrowSpeed( int playerIndex )
{
    Player& player = Players[playerIndex];
    int arrowSpeed = 24;    
	char classId = player.ClassID;
	if( player.effectFlag[EA_RANDOM_SPEED_ARROW] ){
        arrowSpeed = RNG(30) + 10;
    }else if( classId == PC_1_ARCHER ){
        arrowSpeed = /*(player.CharLevel / 25) +*/ 28;  // add perks here to improve speed!!!!!!!!  <<<<< 2 levels, +4 per level. up to 40 with fastest attack
    }else if( classId == PC_0_WARRIOR || classId == PC_5_SAVAGE ){
        arrowSpeed = /*(player.CharLevel / 25) +*/ 26;
    }
	if (HasTrait(playerIndex, TraitId::Pistoleer)) {
		arrowSpeed += 15;
	}
	arrowSpeed += player.attackSpeed;
	LimitToMax(arrowSpeed, 40);
    return arrowSpeed;
}

//----- (00431323) --------------------------------------------------------
void __fastcall CastArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( missile.value[7] /* explosionElement ? */ ) return CastMagicArrow(missileIndex, casterRow, casterCol, targetRow, targetCol, casterDirection, casterType, casterIndex, damage);

	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	int arrowSpeed = 30;
	// 00431355
	if( casterType == CT_0_PLAYER ){ // caster is player
	    arrowSpeed = GetPlayerArrowSpeed( casterIndex );
	}
	// 004313FF
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, arrowSpeed);
	if( missile.BaseMissileIndex == MI_141_THROWING_MALLET ){
		SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	}else{
		missile.FrameIndex = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol) + 1;
	}
	missile.TimeToLive = 256;
}

// unused (old original fire blast, fire + lighting flags hack)
//----- (00431434) --------------------------------------------------------
void __fastcall CastSpecialArrowSpell(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	int arrowSpeed = 0;
	if( casterType == CT_0_PLAYER ){
		arrowSpeed = GetPlayerArrowSpeed(casterIndex);
	}
	missile.TimeToLive = 1;
	missile.value[0] = targetRow;
	missile.value[1] = targetCol;
	missile.value[2] = arrowSpeed;
}

//----- (004314DD) --------------------------------------------------------
void __fastcall FoundFreeCellForTeleportPlayer(int missileIndex, int targetRow, int targetCol)
{
	Missile& missile = Missiles[missileIndex];
	for( int radius = 1; radius < 50; radius++ ){
		for( int colOffset = -radius; colOffset <= radius; colOffset++ ){
			for( int rowOffset = -radius; rowOffset <= radius; rowOffset++ ){
				int row = targetRow + rowOffset;
				int col = targetCol + colOffset;
				if( CellFreeForPlayer(CurrentPlayerIndex, row, col) ){
					missile.Row = row;
					missile.Col = col;
					return;
				}
			}
		}
	}
	missile.Row = targetRow;
	missile.Col = targetCol;
}

//----- (0043157B) --------------------------------------------------------
void __fastcall CastPhasing(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	int row;
	int col;
	bool isPlaceSelect = false;
	Missile& missile = Missiles[missileIndex];

	for( int i = 1; i <= 500; i++){// Пробуем случайные перемещения до тех пор пока перемещение не окажется допустимым
		int casterRowChange = RNG(3) + 4;
		int casterColChange = RNG(3) + 4;
		if( RNG(2) ){
			casterRowChange = -casterRowChange;
		}
		if( RNG(2) ){
			casterColChange = -casterColChange;
		}
		row = casterRow + casterRowChange;
		col = casterCol + casterColChange;

		if( In112(row, col) ){// Проверка на выход за границу карты
			int dungeonMap = FineMap[ row ][ col ];
			if( !TileBlockWalking[dungeonMap] && !ObjectsMap[ row ][ col ] && !MonsterMap[ row ][ col ] ){// Проверка допустимости нахождения в клетке игрока
				isPlaceSelect = true;
				break;
			}
		}
	}

	if( !isPlaceSelect){// Если после 500 попыток не вышло найти место
		row = casterRow;
		col = casterCol;// phase where you are then
	}
	// 00431633
	missile.TimeToLive = 2;
	missile.value[0] = 0;
	
	if( Dungeon->isQuest && Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
		// 0043165D
		int baseObjectIndex = Objects[ObjectsMap[ targetRow ][ targetCol ] - 1].BaseObjectIndex;
		if( baseObjectIndex == BO_84_MCIRL || baseObjectIndex == BO_85_MCIRL ){
			missile.Row = targetRow;
			missile.Col = targetCol;
			if( !CellFreeForPlayer(CurrentPlayerIndex, targetRow, targetCol) ){
				FoundFreeCellForTeleportPlayer(missileIndex, targetRow, targetCol);
			}
		}
	}
	else if (is(Dungeon, DUN_103_ALTAR_OF_IMMOLATION, DUN_104_NETHERWORLD_GATES, DUN_113_THE_MAZE)) {
		// 0043165D
		int baseObjectIndex = Objects[ObjectsMap[targetRow][targetCol] - 1].BaseObjectIndex;
		missile.Row = targetRow;
		missile.Col = targetCol;
		if (!CellFreeForPlayer(CurrentPlayerIndex, targetRow, targetCol)) {
			FoundFreeCellForTeleportPlayer(missileIndex, targetRow, targetCol);
		}
	}
	else{
		// 004316AF
		missile.Row = row;
		missile.Col = col;
		if( casterType == CT_0_PLAYER ){
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_10_PHASING);
		}
	}
}

//----- (004316D2) --------------------------------------------------------
void __fastcall CastWarp(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	if( casterIndex >= 0 ){
		Player& player = Players[casterIndex];
		casterRow = player.Row;
		casterCol = player.Col;
	}
	Missile& missile = Missiles[missileIndex];
	int minDistanceToWarp = INT_MAX;
	int row = casterRow;
	int col = casterCol;
	for( auto& warp: Warps ){
		if( is(warp.GameMessage, WM_1032_RETURN_TO_TOWN, WM_1027_PREV_DUNGEON, WM_1026_NEXT_DUNGEON, WM_1028_RETURN_FROM_QUEST_DUNGEON) ){
			// Чтобы не телепортнуться на клетку внутри лестницы делаем отступ.
			int warpRow = warp.Row;
			int warpCol = warp.Col;
			if( is(Dungeon->genType, DT_1_CHURCH, DT_2_CATACOMB)
			 && is(warp.GameMessage, WM_1026_NEXT_DUNGEON, WM_1027_PREV_DUNGEON, WM_1028_RETURN_FROM_QUEST_DUNGEON) ){
				warpCol++; // 00431781
			}else{
				warpRow++; // 00431778
			}
			int deltaRow = casterRow - warpRow; // 00431788
			int deltaCol = casterCol - warpCol;
			int distanceToWarp = deltaRow * deltaRow + deltaCol * deltaCol;
			if( distanceToWarp < minDistanceToWarp || minDistanceToWarp == -1 ){
				minDistanceToWarp = distanceToWarp;
				row = warpRow;
				col = warpCol;
			}
		}
	}
	missile.TimeToLive = 2; // 004317BF
	missile.value[0] = 0;
	missile.Row = row;
	missile.Col = col;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_42_WARP);
	}
}

//----- (00431803) --------------------------------------------------------
void __fastcall CastFirebolt(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow = RowDelta[casterDirection] + targetRow;
		targetCol = ColDelta[casterDirection] + targetCol;
	}
	// 00431830
	int missileSpeed;
	if( casterType == CT_0_PLAYER ){
		// 0043183A
		bool isHydraFirebolt = false;
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex2 = MissileIndexes[spellIndexIndex];
			Missile& spell2 = Missiles[spellIndex2];
            
            int& CurHydraFireboltIndex = spell2.value[2];
			if( spell2.BaseMissileIndex == MI_2_HYDRA && spell2.CasterIndex == casterIndex && CurHydraFireboltIndex == missileIndex ){
				isHydraFirebolt = true;
				break;
			}
		}
		if( !isHydraFirebolt && !(missile.spellIndex == PS_13_HYDRA && HasTrait(missile.CasterIndex, TraitId::Hydramancer)) ){
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_1_FIREBOLT);
		}
		// 00431895
		if( casterIndex == M1_TRAP ){
			missileSpeed = 16;
		}else{
			missileSpeed = missile.SpellLevel + 10;
		}
		LimitToMax(missileSpeed, 30);
	}else{
		// 004318BB
		missileSpeed = 26;// ловушки, монстры?
	}
	// 004318BE
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (0043191C) --------------------------------------------------------
void __fastcall CastMagmaBall(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.WholeDeltaY += 3 * missile.DeltaY;
	missile.WholeDeltaX += 3 * missile.DeltaX;
	MoveMissile(missileIndex);
	if( missile.DeltaY & 0xFFFF0000 || missile.DeltaX & 0xFFFF0000 ){
		missile.TimeToLive = 256;
	}else{
		missile.TimeToLive = 1;
	}
	// 0043198E
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	// mor: below, tried to read missile damage from monster secondary damage values, but it didn't work (help appreciated)
	//Monster& monster = Monsters[missile.CasterIndex];
	//missile.Damage = RandFromRange(monster.SecondMinDamage, monster.SecondMaxDamage);
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (004319B3) --------------------------------------------------------
void __fastcall MoveMissile(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	int wholeDeltaX = missile.WholeDeltaX >> 16;
	int wholeDeltaY = missile.WholeDeltaY >> 16;
	int wholePixelDeltaRow = 2 * wholeDeltaX + wholeDeltaY; // isometric transformation ? 
	int wholePixelDeltaCol = 2 * wholeDeltaX - wholeDeltaY; //                            
	int deltaRow, deltaCol, deltaRow2, deltaCol2;
	if( wholePixelDeltaRow >= 0 ){
		deltaRow = wholePixelDeltaRow >> 3;
		deltaRow2 = wholePixelDeltaRow >> 6;
	}else{
		deltaRow = -(-wholePixelDeltaRow >> 3);
		deltaRow2 = -(-wholePixelDeltaRow >> 6);
	}
	if( wholePixelDeltaCol >= 0 ){
		deltaCol = wholePixelDeltaCol >> 3;
		deltaCol2 = wholePixelDeltaCol >> 6;
	}else{
		deltaCol = -(-wholePixelDeltaCol >> 3);
		deltaCol2 = -(-wholePixelDeltaCol >> 6);
	}
	int wholeRowDelta = wholePixelDeltaRow / 64;
	int wholeColDelta = wholePixelDeltaCol / 64;
	int row = wholeRowDelta + missile.CasterRow;
	int col = wholeColDelta + missile.CasterCol;
	if( In112(row, col) ){
		missile.Row = row;
		missile.Col = col;
		missile.ColDisplacement = wholeDeltaY + 32 * wholeColDelta - 32 * wholeRowDelta;
		missile.RowDisplacement = wholeDeltaX - 16 * wholeColDelta - 16 * wholeRowDelta;
	}else if( IsMainDraw ){
		missile.IsDeleted = true;
	}
	if( IsMainDraw ){
		ChangeLightOffset(missile.LightIndex, deltaRow - 8 * deltaRow2, deltaCol - 8 * deltaCol2); // possible to make a per-pixel lighting
	}
}

//----- (00431A8F) --------------------------------------------------------
void __fastcall CastSpell51_KRULL(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	PutMissile(missileIndex);
}

//----- (00431AD5) --------------------------------------------------------
void __fastcall CastTeleport(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( To112(row, col) && !PlayerMap[row][col] && !ObjectsMap[row][col] 
			 && !MonsterMap[row][col] && !TileBlockWalking[FineMap[row][col]] ){
				missile.Row = row;
				missile.Col = col;
				missile.CasterRow = row;
				missile.CasterCol = col;
				missile.IsDeleted = false;
				MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_23_TELEPORT);
				missile.TimeToLive = 2;
				return;
			}
		}
	}
}

//----- (00431BD5) --------------------------------------------------------
void __fastcall CastLightingNovaSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.Damage = damage;
	missile.FrameIndex = RNG(8) + 1;
	missile.TimeToLive = 255;
	
	int& startRow = missile.value[0];
	int& startCol = missile.value[1];
	
	if( casterIndex < 0 ){
		startRow = casterRow;
		startCol = casterCol;
	}else{
		Player& player = Players[casterIndex];
		startRow = player.Row;
		startCol = player.Col;
	}
}

//----- (00705810) --------------------------------------------------------
int __fastcall LimitSlvlParamForFireWallTimeliveCalc( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	int slvlParam = missile.SpellLevel;
	LimitToRange(slvlParam, 0, 50);
	return slvlParam;// eax
}

//----- (00431C56) --------------------------------------------------------
void __fastcall CastSpell70LightingWallSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
 
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.Damage = damage;
	missile.FrameIndex = RNG(8) + 1;
    
    if( missile.SpellLevel < 0 ){
        // segment from flask
        missile.TimeToLive = -missile.SpellLevel;
    }else{
        if( is( casterType, CT_0_PLAYER, CT_2_WALL ) && (uint) casterIndex < 4 ){
			int timeSlvlParam = LimitSlvlParamForFireWallTimeliveCalc(missileIndex);
			missile.TimeToLive = 500 + (10 * timeSlvlParam) + (20 * PerkValue(PERK_INSULATION, casterIndex)) + (20 * PerkValue(SYNERGY_ABJURATION, casterIndex, 0));
			if (casterIndex != CurrentPlayerIndex && PlayerWall[casterIndex][SpellIdxToWallType[missile.spellIndex]][6] != 0) { // If 2 players are on the same map during initial wall cast, PlayerWall[][][6] would be 0.  NOT 0 if it's a wall recast!
				missile.TimeToLive = PlayerWall[casterIndex][SpellIdxToWallType[missile.spellIndex]][6]; // Need to minus the time spent loading the map to be more accurate, not sure how to do it...
			}
		}else{
	        missile.TimeToLive = 100/* + 20 * DungeonLevel*/; // 5 seconds
		}	
    }
    
	// 00431CA4
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	if( casterIndex >= 0 && casterType == CT_0_PLAYER){
		Player& player = Players[casterIndex];
		lastRow = player.Row;
		lastCol = player.Col;
	}else{
		lastRow = casterRow;
		lastCol = casterCol;
	}
}

//----- (00431CE0) --------------------------------------------------------
void __fastcall CastSpell5_FireWallSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
    
    if( missile.SpellLevel < 0 ){
        // segment from flask
        missile.TimeToLive = -missile.SpellLevel;
        missile.Damage = damage;
    }else{
        // original diablo 1 damage formula
		// missile.Damage = 16 * (RNG(10) + RNG(10) + player.CharLevel + 2) >> 1;
		if( is( casterType, CT_0_PLAYER, CT_2_WALL ) && (uint) casterIndex < 4 ){
			Player& player = Players[casterIndex];
			missile.Damage = 1 + player.CharLevel / 2; // old formula: 250% clvl (2 - 125). new: 1 + clvl / 2 (1 - 26)
			int timeSlvlParam = LimitSlvlParamForFireWallTimeliveCalc(missileIndex);
			missile.TimeToLive = 500 + (10 * timeSlvlParam) + (20 * PerkValue(PERK_INSULATION, casterIndex)) + (20 * PerkValue(SYNERGY_ABJURATION, casterIndex, 0));
			if (casterIndex != CurrentPlayerIndex && PlayerWall[casterIndex][SpellIdxToWallType[missile.spellIndex]][6] != 0) { // If 2 players are on the same map during initial wall cast, PlayerWall[][][6] would be 0.  NOT 0 if it's a wall recast!
				missile.TimeToLive = PlayerWall[casterIndex][SpellIdxToWallType[missile.spellIndex]][6]; // Need to minus the time spent loading the map to be more accurate, not sure how to do it...
			}
        }else{ // trap, shrine ?
			missile.Damage = 5 * ((Dungeon->level / 3 + 1 ) + ( 8 * Difficulty)); // (1-9) * 10 HORROR, (9-17) * 10 PURGATORY, (17-25) *  10 DOOM
			missile.TimeToLive = 100/* + DungeonLevel * 20*/;
        }
    }
    
    int& StartAnimationEndFrame = missile.value[0];
    
    SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	StartAnimationEndFrame = missile.TimeToLive - missile.framesCount;
	missile.value[1] = 0;
}

//----- (00431DCA) --------------------------------------------------------
void __fastcall CastFireblast(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00431DFF
	int missileSpeed; 
	if (casterType != CT_0_PLAYER) {
		// 00431E92
		/*int damaggio;
		if (IsThisBoss(missile.CasterIndex, 488)) {// mor: preset FB damage, tried to assign it, but failed
			switch (Difficulty) {
			case DL_0_HORROR:		damaggio = 150;	break;
			case DL_1_PURGATORY:	damaggio = 300;	break;
			case DL_2_DOOM:			damaggio = 420;	break;
			}
		}*/	
			// new formula, shorter and better balanced ( http://thehellmod.ucoz.ru/Pix/Clip_21.jpg ):
		/*int damage = 25 + (Dungeon->level * 2)
					+ 79 * Difficulty
					+ RNG(1 + Dungeon->level) ;*/
			// old (overcomplicated) damage formula:
		//switch (Difficulty) {
		//case DL_0_HORROR:		damage = DungeonLevel * 2 +  15 + RNG(DungeonLevel + 1);	break; // 63 - 87
		//case DL_1_PURGATORY:	damage = DungeonLevel * 3 +  67 + RNG(DungeonLevel * 2);	break; // 139 - 184
		//case DL_2_DOOM:			damage = DungeonLevel * 3 + 161 + RNG(DungeonLevel * 2);	break; // 233 - 280
		//}

		Monster& monster = Monsters[missile.CasterIndex]; // 007163DE 
		MonsterSprite* monsterSprite = monster.SpritePtr;
		short bmi = monsterSprite->baseMonsterIndex;
		switch (bmi) {
		case BM_240_DEMON_PRIEST:
		case BM_239_AFFLICTED:
		case BM_18_VORTEX_ROGUE:
		case BM_178_OVERLORD:
			damage = RngFromRange(monster.SecondMinDamage, monster.SecondMaxDamage); break;
		default:
			damage = RngFromRange(monster.MinDamage, monster.MaxDamage); break;
		}
		missile.Damage = damage;
		missileSpeed = 10 + (Difficulty * 6) + (Dungeon->level / 3) + RNG(1 + (Difficulty * 2) + (Dungeon->level / 2)); // 30-40
		LimitToMax(missileSpeed, 40);
	}
	else{
		/*missile.Damage = 4 * (player.CharLevel + RNG(50) + RNG(50)) + 90; // <- old damage formula
		for( int i = 0; i < missile.SpellLevel; i++){
			missile.Damage += missile.Damage / 16;
		}*/
		missile.Damage = (player.CharLevel + RNG(player.CurMagic / 2 + 1) + (missile.SpellLevel * 6)) * 4;
		// some analytics below. adjust formulae in future if it's required, consult the info prior!
		// result avg: 143 + 1300 / 2 + 150 * 6 = 143 + 650 + 900 = ~1700. with  +100% to fire element, it's 1700 * 2 = 3400 per fireblast in late game
		// and late game inferno SP monsters are about 32'000 for rangers and 70'000 for melees. so, 10 to 20 FBs per monster. Tough, but it's ok for balance.
		
		// spellbook formula below, for reference
#if 0
	case PS_12_FIREBLAST:
			minDamage = ((player.CharLevel) + (spellLevel * 6)) * 2;
			maxDamage = ((player.CharLevel) + (player.CurMagic / 2) + (spellLevel * 6)) * 2;
			break;
#endif
		// 00431E70
		missileSpeed = (missile.SpellLevel / 3) + 20 + RNG(missile.SpellLevel / 3 + 1);
		LimitToMax(missileSpeed, 40);
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_12_FIREBLAST);
	}
	// 00431E95
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	missile.value[2] = 0;// похоже 3 неиспользуемых значения
	missile.value[3] = casterRow;
	missile.value[4] = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (00431F08) --------------------------------------------------------
void __fastcall CastExplosiveRuneExplode(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterType == CT_0_PLAYER || casterType == CT_2_WALL ){
		missile.Damage = 2 * (Players[casterIndex].CharLevel + RNG(10) + RNG(10)) + 4;
		for( int i = 0; i < missile.SpellLevel; i++ ){
			missile.Damage += missile.Damage / 8;
		}
		int castDamage = missile.Damage;
		MissileAttack(missileIndex, castDamage, castDamage, 0, missile.Row, missile.Col, 1);
		for( int i = 0; i < 8; i++ ){
			MissileAttack(missileIndex, castDamage, castDamage, 0, missile.Row + RowDelta[i], missile.Col + ColDelta[i], 1);
		}
	}
	// 0043208F
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
	SetMissileDirection(missileIndex, 0);
	missile.IsDeleted = false;
	missile.TimeToLive = missile.framesCount - 1;
}

//----- (00431F08) --------------------------------------------------------
void __fastcall CastExploder(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
	SetMissileDirection(missileIndex, 0);
	missile.IsDeleted = false;
	missile.TimeToLive = 1; // was 10 - hitting for 10 frames in a row
}

//----- (0072341F) --------------------------------------------------------
void __fastcall MonsterHorizontalHitFix( int& monsterNum, int row, int col )
{
	if( monsterNum < 0 && Monsters[ -monsterNum - 1 ].Col == col && Monsters[ -monsterNum - 1 ].Row == row 
	 &&(row > 0 && col < FineMap_112 - 1 && MonsterMap[ row - 1 ][ col + 1 ] == monsterNum 
	 || row < FineMap_112 - 1 && col > 0 && MonsterMap[ row + 1 ][ col - 1 ] == monsterNum) ){
		monsterNum = -monsterNum;
		HorizontalHit = true;
	}
}

//----- (004320CE) --------------------------------------------------------
void __fastcall MissileAttack( int missileIndex, int minDamage, int maxDamage, char isInternalHp, int attackRow, int attackCol, char isPierce, int overTime /*= 0*/)
{
	int isPlayerHit = 0;
	int isMonsterHit = 0;
	int monsterNum = 0;
	char playerNum = 0;
	int monsterIndex = 0;
	uchar attackBlocked = 0; // in original cross with ((char*)&attackRow)[3]
	uchar missileReflected = 0;
	Missile& missile = Missiles[missileIndex];
	
	bool isCoordinateCorrect = In112(attackRow, attackCol) && attackCol >= 0;
	bool isCastIndexCorrect = missileIndex >= 0 && missileIndex < MissilesMax;
	
	if( !(Dungeon->genType || Dungeon->isQuest) || !isCastIndexCorrect || !isCoordinateCorrect){
		HorizontalHit = false;
		return;
	}

	int casterIndex = missile.CasterIndex;
	bool PiercingShot = false;
	if (missile.CasterType == CT_0_PLAYER && BaseMissiles[missile.BaseMissileIndex].EffectType == MT_0_ARROW && HasTrait(missile.CasterIndex, TraitId::PiercingShot)) {
		const int pierceRandFactor = RNG(100);
		if (pierceRandFactor < Players[missile.CasterIndex].CharLevel / 3) {
			PiercingShot = true;
		}
	}
	if (missile.CasterType == CT_0_PLAYER && HasTrait(missile.CasterIndex, TraitId::Pistoleer)) {
		const int pierceRandFactor = RNG(100);
		if (pierceRandFactor < PerkValue(PERK_PIERCING_BULLETS, missile.CasterIndex)) {
			PiercingShot = true;
		}
	}
	if( missile.CasterType == CT_2_WALL || casterIndex == -1 ){ // trap, rebound arrow
		// Fix horizontal walk hit MvM 1 
		monsterNum = MonsterMap[ attackRow ][ attackCol ];
		MonsterHorizontalHitFix( monsterNum, attackRow, attackCol );
		if( monsterNum > 0 ){
			if( missile.CasterType == CT_2_WALL ){ // golem? hydra?
				// TODO: уязвимость, монстры и алтари тоже умеют кастовать стены
				isMonsterHit = PvM_Ranged(missile.CasterIndex, monsterNum - 1, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isInternalHp, missileIndex );
			}else{ // cast.CasterType == -1 
				isMonsterHit = MvM_Ranged(monsterNum - 1, missile.CasterIndex, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isInternalHp);
			}
			if( isMonsterHit ){
				if( !isPierce ){
					missile.TimeToLive = 0;
				}
				missile.isHitTarget = 1;
			}
		}
		playerNum = PlayerMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit MvP 1
		if( playerNum <= -1 && playerNum >= -4 && Players[-playerNum - 1].Row == attackRow && Players[-playerNum - 1].Col == attackCol && (PlayerMap[ attackRow - 1 ][ attackCol + 1 ] == playerNum || PlayerMap[ attackRow + 1 ][ attackCol - 1 ] == playerNum) ){
			playerNum = -playerNum;
			HorizontalHit = true;
		}
		if( playerNum > 0 ){
			int isLightningOrFirewall = missile.spriteIndex == MIS_4_FIREWAL || missile.spriteIndex == MIS_3_LGHNING;
			casterIndex = -1; // trap ?
			isPlayerHit = MvP_Ranged(playerNum - 1, casterIndex, missile.Distance, minDamage, maxDamage, missile.BaseMissileIndex, isInternalHp, isLightningOrFirewall, &attackBlocked, &missileReflected);
		}
	}else if( missile.CasterType == CT_0_PLAYER ){ // caster is player
		monsterNum = MonsterMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit PvM
		MonsterHorizontalHitFix( monsterNum, attackRow, attackCol );
		if( monsterNum > 0 ){
			monsterIndex = monsterNum - 1;
			if( !(Monsters[monsterIndex].flag & MF_6_FRIENDLY) ){
                isMonsterHit = PvM_Ranged(casterIndex, monsterIndex, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isInternalHp, missileIndex, overTime );
                if( isMonsterHit ){
                    if( !isPierce && !PiercingShot ){
                        missile.TimeToLive = 0;
                    }
                    missile.isHitTarget = 1;
                }
            }
		}
		playerNum = PlayerMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit PvP
		if( playerNum <= -1 && playerNum >= -4 && Players[-playerNum - 1].Row == attackRow && Players[-playerNum - 1].Col == attackCol
		 && (attackRow > 0 && attackCol < FineMap_112 - 1 && PlayerMap[ attackRow - 1 ][ attackCol + 1 ] == playerNum
		 || attackRow < FineMap_112 - 1 && attackCol > 0 && PlayerMap[ attackRow + 1 ][ attackCol - 1 ] == playerNum) ){
			playerNum = -playerNum;
			HorizontalHit = true;
		}
		if( playerNum > 0 ){
			casterIndex = missile.CasterIndex;
			int targetPlayerIndex = playerNum - 1;
			if( targetPlayerIndex != casterIndex ){
				isPlayerHit = PvP_Ranged(casterIndex, targetPlayerIndex, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isInternalHp, attackBlocked);
			}
		}
	}else{ // missile.CasterType == CT_1_MONSTER_AND_TRAP
	    monsterNum = MonsterMap[ attackRow ][ attackCol ];
		MonsterHorizontalHitFix( monsterNum, attackRow, attackCol );
		if( monsterNum > 0 ){
			monsterIndex = monsterNum - 1;
			if( (Monsters[casterIndex].flag & MF_6_FRIENDLY) != (Monsters[monsterIndex].flag & MF_6_FRIENDLY) ){
			    isMonsterHit = MvM_Ranged(monsterNum - 1, missile.CasterIndex, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isInternalHp, missile.flag & MSF_REFLECTED);
				if( isMonsterHit ){
					if( !isPierce ){
						missile.TimeToLive = 0;
					}
					missile.isHitTarget = 1;
				}
            }
		}
		
        if( !(Monsters[casterIndex].flag & MF_6_FRIENDLY) ){
            playerNum = PlayerMap[ attackRow ][ attackCol ];
            if( playerNum <= -1 && playerNum >= -4 && Players[-playerNum - 1].Row == attackRow && Players[-playerNum - 1].Col == attackCol
             && (attackRow > 0 && attackCol < FineMap_112 - 1 && PlayerMap[ attackRow - 1 ][ attackCol + 1 ] == playerNum
             || attackRow < FineMap_112 - 1 && attackCol > 0 && PlayerMap[ attackRow + 1 ][ attackCol - 1 ] == playerNum) ){
                playerNum = -playerNum;
                HorizontalHit = true;
            }
            if( playerNum > 0 ){
                isPlayerHit = MvP_Ranged(playerNum - 1, missile.CasterIndex, missile.Distance, minDamage, maxDamage, missile.BaseMissileIndex, isInternalHp, 0, &attackBlocked, &missileReflected);
            }
        }
	}
	if( isPlayerHit ){
		if( attackBlocked ){
			int animDirectionsCount = MissileSprites[missile.spriteIndex].animDirectionsCount;
			int direction = missile.direction + (RNG(2) ? 1 : -1);
			if( direction >= 0 ){
				if( direction >= animDirectionsCount ){
					direction = 0;
				}
			}else{
				direction = animDirectionsCount - 1;
			}
			SetMissileDirection(missileIndex, direction);
		}
		else if(missileReflected) {
			missile.TimeToLive = 0;
		}
		else if( !isPierce ){
			missile.TimeToLive = 0;
		}
		missile.isHitTarget = 1;
	}
	int objectNum = ObjectsMap[ attackRow ][ attackCol ];
	if( objectNum ){
		int objectIndex;
		if( objectNum <= 0 ){
			objectIndex = -objectNum - 1;
		}else{
			objectIndex = objectNum - 1;
		}
		if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				if (Objects[objectIndex].destructable == 1) {
					DamageDestroyableOnMapObject(CurrentPlayerIndex, objectIndex);
					if (!isPierce) {
						missile.TimeToLive = 0;
					}
					missile.isHitTarget = 0;
				}
		}
		else {
			if ( !Objects[objectIndex].objectAllowWalking ) {
				if (Objects[objectIndex].destructable == 1) {
					DamageDestroyableOnMapObject(-1, objectIndex);
				}
				if( !isPierce ){
					missile.TimeToLive = 0;
				}
				missile.isHitTarget = 0;
			}
		}
	}
	if( TileBlockMissle[FineMap[ attackRow ][ attackCol ]] ){
		if( !isPierce ){
			missile.TimeToLive = 0;
		}
		if( ! isPlayerHit ){ // fixed multi damage from some attack on enter tile (tentacle for example)
			missile.isHitTarget = 0;
		}
	}
	if( !missile.TimeToLive && BaseMissiles[missile.BaseMissileIndex].ActSound != S_M1_NO_SOUND ){
		PlayLocalSound(BaseMissiles[missile.BaseMissileIndex].ActSound, missile.Row, missile.Col);
	}
	HorizontalHit = false;
}

//----- (00432460) --------------------------------------------------------
bool __fastcall PvP_Ranged( int attackerPlayerIndex, int targetPlayerIndex, int minDamage, int maxDamage, int distance, int spellIndex, char mayBeIsTrap, uchar& attackBlocked )
{
	Player& attacker = Players[attackerPlayerIndex];
	Player& defender = Players[targetPlayerIndex];
    const BaseMissile& baseMissile = BaseMissiles[spellIndex];

	attackBlocked = 0;

//	if (IsPlayerFriendly || defender.IsNotHittableByAnySpellMissiles || spellIndex == MI_53_HOLY_BOLT//IsPlayerFriendly is LOCAL. Player 1 hostiles player 2 while player 2 remains friendly. When player 1 missile attacks player 2, player 1 sees missile hits while player 2 sees missile miss.
	if (!attacker.IsPlayerEnemy || defender.notHittableInTransition || defender.notHittableFramesAfterTransition > 0 || spellIndex == MI_53_HOLY_BOLT//Use force sync'd Player field .IsPlayerEnemy to fix the above desync.
	 || defender.activeBuffFlag & BF_1_ETHEREAL && baseMissile.EffectType != MT_0_ARROW ){
		return 0;
	}

	float defenderResistancePercent = defender.resistPercent[ baseMissile.DamageType ];
	int toHitRandFactor = RNG(100);
	
	int hitChance;
	if( BaseMissiles[spellIndex].EffectType != MT_0_ARROW ){// маг атака
		hitChance = attacker.CurMagic - 2 * defender.CharLevel - distance + 25;
		switch (attacker.ClassID){
			case PC_2_MAGE:			hitChance += 20;		break;
			case PC_4_ROGUE:		hitChance += 40;		break;
		}
		hitChance += PerkValue(PERK_MASTER_CASTER, attackerPlayerIndex);
	}else{
		int basic_AC_divider = 5;
		if (HasTrait(targetPlayerIndex, TraitId::SmallFrame)) {
			basic_AC_divider = 3;
		}
		hitChance = attacker.CharLevel
			+ attacker.CurDexterity
			+ attacker.ItemsAddToHitPercents
			- (distance * distance) / 2
			- defender.ACFromItems
			- defender.ACFromClass
			- (defender.CurDexterity / basic_AC_divider) 
			+ 25;
		switch (attacker.ClassID){
			case PC_1_ARCHER:		hitChance += 20;		break;
			case PC_0_WARRIOR:
			case PC_4_ROGUE:		hitChance += 10;		break;
		}
	}
	LimitToRange(hitChance, 5, 95);

	if( toHitRandFactor >= hitChance ){ 
		return 0;
	}

	int blockRandFactor;
	if( defender.CurAction != PCA_0_STAND && defender.CurAction != PCA_4_ATTACK 
		|| !defender.CanBlock || mayBeIsTrap == 1 ){
			blockRandFactor = 100;
	}else{
		blockRandFactor = RNG(100);
	}

	int blockChance = defender.BaseDexterity + defender.BlockBonus + 2 * (defender.CharLevel - attacker.CharLevel);
	blockChance += defender.blockChance;
	LimitToRange(blockChance, 0, 80);

	int calculateDamage;
	if( spellIndex == MI_63_BONE_SPIRIT ){
		calculateDamage = defender.CurLife / 8;
	}else if( spellIndex == MI_41_HELLFIRE ){
		return 0;
	}else{
		calculateDamage = RngFromRange(minDamage, maxDamage);
		if( BaseMissiles[spellIndex].EffectType == MT_0_ARROW ){
			calculateDamage += attacker.BaseDamage + attacker.ItemsAddDamage + calculateDamage * attacker.ItemsAddDamagePercents / 100;
		}
		if( !mayBeIsTrap ){
			calculateDamage <<= 6;
		}
		if( BaseMissiles[spellIndex].EffectType ){ // spells
			calculateDamage /= GameMode == GM_COLISEUM ? 2 : 32;// was col: 4
		}
		else{ // arrows
			calculateDamage /= GameMode == GM_COLISEUM ? 1 : 16;// was col: 2
		}
	}
	
	if( defenderResistancePercent <= 0 ){
		if( blockRandFactor >= blockChance ){
			if( attackerPlayerIndex == CurrentPlayerIndex ){
                NetSendCmdDamage( 1, targetPlayerIndex, calculateDamage );
			}
			//StartPlayerHit(targetPlayerIndex, calculateDamage, 0);
			//defender.lastAttacker = 1 + attackerPlayerIndex;
		}else{
			StartPlayerBlock(targetPlayerIndex, OrientationToTarget(defender.Row, defender.Col, attacker.Row, attacker.Col));
			attackBlocked = 1;
		}
		return 1;
	}

	calculateDamage -= int(double(calculateDamage) * defenderResistancePercent);
	if( attackerPlayerIndex == CurrentPlayerIndex ){
        NetSendCmdDamage( 1, targetPlayerIndex, calculateDamage );
	}
	int attackSound;
	switch(defender.ClassID){
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:			attackSound = S_798_WAR_69;				break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:			attackSound = S_689_ROG_69;				break;
		case PC_2_MAGE:				attackSound = S_586_SOR_69;				break;
		case PC_3_MONK:				attackSound = S_906_MONK_69;				break;
		default:														return 1;
	}
	PlayLocalSound(attackSound, defender.Row, defender.Col);
	StartPlayerHit(targetPlayerIndex, calculateDamage, 0); 
	defender.lastAttacker = 1 + attackerPlayerIndex; 
	return 1;
}

//----- (004327C9) --------------------------------------------------------
void __fastcall CastSpell88FireBall(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	int missileSpeed;
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004327FE
	if( casterType != CT_0_PLAYER ){
		missileSpeed = 16;
	}else{
		missile.Damage = 2 * (Players[casterIndex].CharLevel + RNG(10) + RNG(10)) + 4;
		for( int i = 0; i < missile.SpellLevel; i++){
			missile.Damage += missile.Damage / 8;
		}
		missileSpeed = 2 * missile.SpellLevel + 16;
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_12_FIREBLAST);
	}
	LimitToMax(missileSpeed, 50);
	// 00432894
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.value[0] = casterRow;
	missile.value[1] = casterCol;
	missile.value[2] = 0;
	missile.value[3] = casterRow;
	missile.value[4] = casterCol;
	missile.value[5] = 2;
	missile.value[6] = 2;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (00432916) --------------------------------------------------------
void __fastcall CastFireNovaSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetCol += ColDelta[casterDirection];
		targetRow += RowDelta[casterDirection];
	}
	// 00432948
	int missileSpeed;
	if( casterType != CT_0_PLAYER ){
		// 00432969
		missileSpeed = 16;
	}else{
		// 0043294E
		missileSpeed = missile.SpellLevel + 16;
	}
	LimitToMax(missileSpeed, 50);
	// 0043296C
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	missile.value[2] = 0;
	missile.value[3] = casterRow;
	missile.value[4] = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (004329DA) --------------------------------------------------------
void __fastcall CastLightning(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	if( !damage && casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_3_LIGHTNING);
	}
	// 004329FB
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 32);
	missile.FrameIndex = RNG(8) + 1;
	missile.TimeToLive = 256;
}

//----- (00432A46) --------------------------------------------------------
void __fastcall CastLightningAsArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00432A73
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 32);
	missile.FrameIndex = RNG(8) + 1;
	missile.TimeToLive = 255;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	if( casterIndex < 0 ){
		lastRow = casterRow;
		lastCol = casterCol;
	}else{
		Player& player = Players[casterIndex];
		lastRow = player.Row;
		lastCol = player.Col;
	}
	missile.Damage *= 64;
}

//----- (00432AE5) --------------------------------------------------------
void __fastcall Cast_Lightning_Segment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int sourceCastIndex, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Missile& sourceCast = Missiles[sourceCastIndex];
	missile.CasterRow = targetRow;
	missile.CasterCol = targetCol;
	if( sourceCastIndex >= 0 ){
		missile.ColDisplacement = sourceCast.ColDisplacement; // молния не двигается, а стоит на месте и исчезает за 8-10 кадров
		missile.RowDisplacement = sourceCast.RowDisplacement;
		missile.WholeDeltaY	 = sourceCast.WholeDeltaY;
		missile.WholeDeltaX	 = sourceCast.WholeDeltaX;
	}
	// 00432B43
	missile.FrameIndex = RNG(8) + 1;
	if( sourceCastIndex < 0 ){
		// 00432B8D
		missile.TimeToLive = 8;
	}else if( casterType == CT_1_MONSTER_AND_TRAP || casterIndex == -1 ){
		// 00432B77
		if( sourceCastIndex >= 0 && casterIndex != -1 ){
			missile.TimeToLive = 10;
		}else{
			missile.TimeToLive = 8;
		}
	}else{ // player cast
		// 00432B64
		missile.TimeToLive = (missile.SpellLevel / 2) + 6;
	}
	// 00432B97
	missile.LightIndex = AddLight(missile.Row, missile.Col, 4);
}

//----- (00432BB6) --------------------------------------------------------
void __fastcall CastMagicExplosion(int missileIndex, int casterRow, int casterCol, int sourceCastIndex, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	if( casterType != CT_0_PLAYER && casterIndex > 0 ){
		int spriteIndex;
		// arcane star explosion colors by witch type (base monster index)
		if (not(Monsters[casterIndex].SpritePtr->baseMonsterIndex, BM_103_BLOOD_WITCH, BM_104_DARK_WITCH, BM_284_SIN_SPAWN) && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest){
			if (IsThisBoss(casterIndex, UM_85_Lustbrood_the_Carnal) ) {
				SetMissileSprite(missileIndex, MIS_45_SCBSEXPD);
				goto SSS;
			}
			else if (IsThisBoss(casterIndex, UM_529_Sybil)) {
				SetMissileSprite(missileIndex, MIS_23_FLAREEXP);
				goto SSS;
			}
			else {
				spriteIndex = -1; // standard explode colors
				goto SKIP_RECOLORS;
			}
		}
		switch( Monsters[casterIndex].SpritePtr->baseMonsterIndex ){
			//case 101: spriteIndex = MIS_23_FLAREEXP;	break; // crimson ?
			case BM_102_SUCCUBUS: spriteIndex = MIS_41_SCBSEXPB;	break; // level colors explode
			case BM_103_BLOOD_WITCH or BM_C_323_HELL_SPAWN: spriteIndex = MIS_45_SCBSEXPD;	break; // red explode 
			case BM_104_DARK_WITCH or BM_C_324_SOUL_BURNER: spriteIndex = MIS_43_SCBSEXPC;	break; // yellow explode
			case BM_C_322_SNOW_WITCH: spriteIndex = MIS_79_SCBSEXPBLU; break;
			default:  spriteIndex = -1;	break;
		}
	SKIP_RECOLORS:
		if( spriteIndex != -1){
			SetMissileSprite(missileIndex, spriteIndex);
		}
	}	
	SSS:
	// 00432BFA
	Missile& missile = Missiles[missileIndex];
	Missile& sourceSpell = Missiles[sourceCastIndex];
	
	int& LightLevelIndex = missile.value[0];
	
	missile.Row = sourceSpell.Row;
	missile.Col = sourceSpell.Col;
	missile.CasterRow = sourceSpell.CasterRow;
	missile.CasterCol = sourceSpell.CasterCol;
	missile.ColDisplacement = sourceSpell.ColDisplacement;
	missile.RowDisplacement = sourceSpell.RowDisplacement;
	missile.WholeDeltaY = sourceSpell.WholeDeltaY;
	missile.WholeDeltaX = sourceSpell.WholeDeltaX;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.TimeToLive = missile.framesCount;
	LightLevelIndex = 0;
}

//----- (00432C91) --------------------------------------------------------
void __fastcall CastWeaponElementalDamage(int missileIndex, int casterRow, int casterCol, int damageType, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.value[0] = 0; // Light Level Index
    missile.value[1] = damageType; // Weapon Elemental Damage Type
	missile.direction = 0;
	SetMissileSprite( missileIndex, by( damageType, MIS_M1_NONE, MIS_5_MAGBLOS, MIS_26_MINILTNG, MIS_62_ARCARROW_EXPLO, MIS_64_ACIDARROWEXPLO, MIS_60_HARROW_EXPLO, MIS_73_COLDEXPLO ) );
	int sound = S_M1_NO_SOUND;
	switch( damageType ){
	case ET_1_FIRE:      sound = RandSound( 1642, 1644 ); break;
	case ET_2_LIGHTNING: sound = RandSound( 1638, 1641 ); break;
	case ET_3_ARCAN:     sound = RandSound( 1646, 1648 ); break;
	case ET_4_ACID:      sound = S_1632_Devast_tox_aura;  break;
	case ET_5_HOLY:      sound = /*S_1633_Devast_holy_aura*/ S_1650; break;
	case ET_6_COLD:      sound = RandSound( 1635, 1637 ); break;
	}
	if( sound != S_M1_NO_SOUND ) PlayLocalSound( sound, casterRow, casterCol );
	missile.TimeToLive = missile.framesCount - 1;
}

//----- For Aura Hit GFX --------------------------------------------------
void __fastcall CastAuraElementalGFX(int missileIndex, int casterRow, int casterCol, int damageType, int monsterIndex, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.Row = casterRow;
	missile.Col = casterCol + (damageType == ET_1_FIRE); // Lich's fireball explosion offset fix col + 1
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.value[0] = 0; // Unused
	missile.value[1] = damageType; // Aura Elemental Damage Type
	missile.value[2] = monsterIndex; // Aura segment attacked monster index
	missile.value[3] = Monsters[monsterIndex].Row;
	missile.value[4] = Monsters[monsterIndex].Col;
	missile.direction = 0; // Some GFX has multiple directions, direction 0 is usually the most fitting
	SetMissileSprite(missileIndex, by(damageType, MIS_10_BLOOD, MIS_57_EX_ORA1, MIS_26_MINILTNG, MIS_62_ARCARROW_EXPLO, MIS_82_TOXIC_AURA, MIS_60_HARROW_EXPLO, MIS_73_COLDEXPLO));
	int sound = S_M1_NO_SOUND;
	switch( damageType ){
	case ET_0_PHYSICAL:  sound = S_106_ELEMENTL;          break;
	case ET_1_FIRE:      sound = S_1630_Devast_fire_aura; break;
	case ET_2_LIGHTNING: sound = S_1631_Devast_zap_aura;  break;
	case ET_3_ARCAN:     sound = S_1634_Devast_arc_aura;  break;
	case ET_4_ACID:      sound = S_1632_Devast_tox_aura;  break;
	case ET_5_HOLY:      sound = S_1633_Devast_holy_aura; break;
	case ET_6_COLD:      sound = S_1212_COLD4;            break;
	}
	if( sound != S_M1_NO_SOUND ) PlayLocalSound( sound, casterRow, casterCol );
	missile.TimeToLive = missile.framesCount - 1;
}

//----- (00432CFC) --------------------------------------------------------
bool __fastcall IsPointNearWarp( int row, int col )
{
	for( auto& warp: Warps ){
		if( abs(warp.Row - row) < 2 && abs(warp.Col - col) < 2 ) return true;
	}
	return false;
}

//----- (00432D5F) --------------------------------------------------------
void __fastcall CastTownPortal(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	
	int& StartAnimationEndFrame = missile.value[0];

	if( Dungeon == DUN_0_TOWN ){
		// Появление портала в городе. кастуется програмно при заходе в город
		// 00432E5E
		missile.Row = targetRow;
		missile.Col = targetCol;
		missile.CasterRow = targetRow;
		missile.CasterCol = targetCol;
		missile.IsDeleted = false;
	}else if( casterRow == 0 && casterCol == 0 ){ // If player is changing level via TP or stairs, his coordinates are set to (0,0), resulting in CheckCastInSight() fail and TP deleted
		Portal& portal = Portals[casterIndex];
		if( portal.open ) { // Check TP exists or not, is this needed?
			missile.Row = targetRow;
			missile.Col = targetCol;
			missile.CasterRow = targetRow;
			missile.CasterCol = targetCol;
			// Make sure a TP cast in Quest area will not re-appear in a non-Quest area, whose Dlvl index is the same as QuestFloorIndex of the TP
			// and a TP cast in a non-Quest area will not re-appear in a Quest area, whose QuestFloorIndex is the same as Dlvl index of the TP
			// This should fix the bug where a TP can show up on multiple Horazon Levels as they all have the same Dlvl = 23 but different QuestFloorIndex.
			// However, if P1 opens TP in Horazon Haven, P2 in Summoning Room at the same (X,Y) coordinates can still see the tiles light up...
			// Need to check SetupTownPortal(), SetPortalStats() and DPortal struct to see if we need to add QuestFloorIndex check in there to remove TP lights...
			//missile.IsDeleted = (portal.isQuestFloor && !IsQuestFloor) || (!portal.isQuestFloor && IsQuestFloor) || (portal.isQuestFloor && IsQuestFloor && portal.dungeon != QuestFloorIndex);
			missile.IsDeleted = Dungeons[portal.dungeon].isQuest && !Dungeon->isQuest || !Dungeons[portal.dungeon].isQuest && Dungeon->isQuest;
		}
	}else{
		char steps[28];
		missile.IsDeleted = true;
		for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
			int circleOffset = CircleOffsets[circleOffsetIndex];
			int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
			for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
				int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
				int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
				int cell = row * 112 + col;
				if( To112(row, col)
				 && !TileBlockWalking[FineMap[ 0 ][ cell ]] 
				 && !TileBlockMissle[FineMap[ 0 ][ cell ]] 
				 && !MissilesMap[ 0 ][ cell ]
				 && !ObjectsMap[ 0 ][ cell ] 
				 && !PlayerMap[ 0 ][ cell ] 
				 && !IsPointNearWarp(row, col)
				 && (Dungeon == DUN_44_RAVENHOLM
				  ? FindPath(CellFreeForPlayer, casterIndex, casterRow, casterCol, targetRow, targetCol, steps)
				  : CheckCastInSight(casterRow, casterCol, row, col)) ){
					// 00432E29
					missile.Row = row;
					missile.Col = col;
					missile.CasterRow = row;
					missile.CasterCol = col;
					missile.IsDeleted = false;
					goto BREAK_LABEL;
				}
			}
		}
		BREAK_LABEL:;
	}
	// 00432E89
	missile.TimeToLive = 100;
	StartAnimationEndFrame = 100 - missile.framesCount;
	missile.value[1] = 0;

	// закрываем другие порталы того же игрока
	for( int i = 0; i < MissileAmount; i++){
		int castIndex = MissileIndexes[i];
		Missile& missile = Missiles[castIndex];
		if( missile.BaseMissileIndex == MI_10_TOWN_PORTAL 
			&& castIndex != missileIndex 
			&& missile.CasterIndex == casterIndex){
				missile.TimeToLive = 0;
		}
	}
	// 00432EEC
	PutMissile(missileIndex);
	if( casterIndex == CurrentPlayerIndex && !missile.IsDeleted && Dungeon != DUN_0_TOWN ){
		int destLvl = (DUNGEON)Dungeon;
		if( Dungeon->isQuest
		 && MonstersCount > SummonMonsters_Count && !SaveAlwaysEnabled && not(GameMode, GM_EASY, GM_CLASSIC) && MaxCountOfPlayersInGame == 1 ){ // Allow quest area TP in MP
			missile.IsDeleted = true;
			return;
		}
        NetSendCmdLocParam3( 1, CMD_56_ACTIVATEPORTAL, missile.Row, missile.Col, destLvl, Dungeon->genType, Dungeon->isQuest );
	}
}

//----- (00432F59) --------------------------------------------------------
void __fastcall CastFlashForward(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterType != CT_0_PLAYER ){ // Монстр
		missile.Damage = 16 * (Monsters[casterIndex].ArmorClass - 3); // 0043300C
	}else if( casterIndex == -1 ){ // Ловушка? Алтарь
		missile.Damage = Dungeon->level; // 00432FFB
	}else{ // Игрок
		/*missile.Damage = 0; // 00432F76 <<- old fucked up formula
		for( int i = 0; i <= Players[casterIndex].CharLevel; i++){
			missile.Damage += RNG(43) + 1;
		}
		for( int i = 0; i < missile.SpellLevel; i++){ // 00432FBB
			missile.Damage += missile.Damage / 14;
		}*/
		missile.Damage = (( Players[casterIndex].CurMagic ) + ( Players[casterIndex].CharLevel * 5 ) * missile.SpellLevel)*4 ;
		missile.Damage += missile.Damage / 2; // 00432FD9
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_4_FLASH);
	}
	missile.TimeToLive = 19;
}

//----- (0043303D) --------------------------------------------------------
void __fastcall CastNullSpell(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	return;
}

//----- (00433040) --------------------------------------------------------
void __fastcall CastSomeAnotherFlashBk(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterType != CT_0_PLAYER ){ // Монстры не умеют это кастовать?
		return;
	}
	if( casterIndex == -1 ){// Ловушки не умеют этого?
		return;
	}
	missile.Damage = 0;
	int timeToLive;
	if( 2 * (casterIndex > 0) ){// TODO: а ничего страшного что у игрока в сингле индекс 0?
		timeToLive = Players[casterIndex].CharLevel;
	}else{
		timeToLive = 1;
	}
	missile.TimeToLive = timeToLive + 10 * missile.SpellLevel + 245;
}

//----- (0043309C) --------------------------------------------------------
void __fastcall CastFlashBack( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterType == CT_0_PLAYER ){
		if( casterIndex == -1 ){ // TODO: не будет выполнено. У ловушек casterType 1
			missile.Damage = Dungeon->level; // 0043313F
		}else{
			Player& player = Players[casterIndex]; // 004330BE
			/*missile.Damage = 0; // <<- old fucked up code
			for( int i = 0; i <= player.CharLevel; ++i ){
				missile.Damage += RNG(43) + 1;
			}
			int damage = missile.Damage; // 00433103
			for( int i = 0; i < missile.SpellLevel; ++i ){
				damage += damage / 14;
			}
			missile.Damage = damage;*/
			missile.Damage = (( Players[casterIndex].CurMagic ) + (Players[casterIndex].CharLevel * 5) * missile.SpellLevel)*4;
			missile.Damage += missile.Damage / 2; // 00433129

		}
	}
	if( casterType != CT_0_PLAYER ){ // 00433154
		missile.Damage = 16 * (Monsters[casterDirection].ArmorClass - 3);
	}
	missile.MayBeIsExploding = 1; // 0043315D
	missile.TimeToLive = 19;
}

//----- (00433178) --------------------------------------------------------
void __fastcall CastManaShield(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	missile.TimeToLive = 48 * player.CharLevel;
    int& playerLastCurLife = missile.value[0];
    int& playerLastBaseLife = missile.value[1];
    playerLastCurLife = player.CurLife;
    playerLastBaseLife = player.BaseLife;
	missile.value[7] = -1;
	// 004331C4
	if( casterType == CT_0_PLAYER ){// ненужная перестраховка. Всё равно никто кроме игроков не способен кастовать мш
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_11_MANA_SHIELD);
	}
}

//----- (004331DA) --------------------------------------------------------
void __fastcall CastHellfireSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	
	int& HellFireFramesFromStart = missile.value[0];
    int& Lightness = missile.value[1];

//	missile.direction = OrientationToTarget(missile.Row, missile.Col, targetRow, targetCol);
	missile.direction = casterDirection;//Carry direction info to modify KB execution
	missile.Damage = RNG(5) + Players[casterIndex].CharLevel / 4 + 1;
	if (casterDirection % 2 == 0) {
		missile.Damage /= 2;//Penalize diagonal force wave damage by a factor of 2 because of 2 layers of walls
	}
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 6);
	missile.TimeToLive = 255;
	HellFireFramesFromStart = 0;
	Lightness = 0;
	missile.Row++;
	missile.Col++;
	missile.RowDisplacement -= 32;
}

//----- (00713F80) --------------------------------------------------------
int __fastcall MaxCountOfHydrasForPlayer (int playerIndex)
{
	Player& player = Players[playerIndex];
	int maxCount = player.CharLevel / 26 + 2; // max 7 naturally
	maxCount += PerkValue(PERK_SUMMONING, playerIndex);
	if (HasTrait(playerIndex, TraitId::Hydramancer)) {
		maxCount = 2 + PerkValue(PERK_CONJURATION, playerIndex);
	}
	LimitToMin(maxCount, 1);
	return maxCount;
}

//----- (00714000) --------------------------------------------------------
int __fastcall CountOfPlayerHydras (int playerIndex)
{
	Player& player = Players[playerIndex];
	int countOfHydras = 0;
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_2_HYDRA 
			&& spell.CasterIndex == playerIndex
			&& spell.TimeToLive > 15){
				countOfHydras++;
		}
	}
	return countOfHydras;
}

//----- (00714080) --------------------------------------------------------
void __fastcall ClearOneOldestPlayerHydra (int playerIndex)
{
	Player& player = Players[playerIndex];
	Missile* oldestHydra = 0;
	int oldestHydraIndex = 0;
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int missileIndex = MissileIndexes[spellIndexIndex];
		Missile& missile = Missiles[missileIndex];
		if( missile.BaseMissileIndex == MI_2_HYDRA 
		 && missile.CasterIndex == playerIndex
		 && missile.TimeToLive > 15
		 // There's a bug in TH1 where the zero index isn't used to find the oldest hydra. We emulate it during reconciliation.
		 && ( !oldestHydra || oldestHydra->TimeToLive > missile.TimeToLive ) ){
			oldestHydra = &missile;
			oldestHydraIndex = missileIndex;
		}
	}
	if( oldestHydra ){
		oldestHydra->TimeToLive = 15;
	}
}

//----- (00714100) --------------------------------------------------------
void __fastcall LimitPlayerHydras (int playerIndex)
{
	Player& player = Players[playerIndex];
	int countOfHydras = CountOfPlayerHydras (playerIndex);
	int maxCountOfHydras = MaxCountOfHydrasForPlayer (playerIndex);
	for( int i = countOfHydras; i > maxCountOfHydras; i-- ){
		ClearOneOldestPlayerHydra (playerIndex);
	}
}

//----- (0043325D) --------------------------------------------------------
void __fastcall CastHydra(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
 
	if( player.fullClassId == PFC_ELEMENTALIST ){
        SetMissileSprite(missileIndex, MIS_81_COLD_GUARD);
    }
	
	int& StartAnimationEndFrame = missile.value[0];
	
	// 004332C1
	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];

			if( To112(row, col) // было +-15 в th2, сделал как в th1 с 1 до 111
			 && CheckCastInSight(casterRow, casterCol, row, col) 
			 && !MonsterMap[ row ][ col ] 
			 && !TileBlockMissle[FineMap[ row ][ col ]] 
			 && !TileBlockWalking[FineMap[ row ][ col ]] 
			 && !ObjectsMap[ row ][ col ] 
			 && !MissilesMap[ row ][ col ] ){
				missile.Row = row;
				missile.Col = col;
				missile.CasterRow = row;
				missile.CasterCol = col;
				missile.IsDeleted = false;
				MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_13_HYDRA);
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:;
	// 004333CA
	if( missile.IsDeleted ){
		return;
	}
	// 004333D7
	missile.CasterIndex = casterIndex;
	missile.LightIndex = AddLight(missile.Row, missile.Col, 1);
	int slvlParam = missile.SpellLevel + (player.CharLevel / 2);
	if (HasTrait(casterIndex, TraitId::Hydramancer)) {
		missile.TimeToLive = ( 20 * 20 ) + ( PerkValue(PERK_RETENTION, casterIndex) * 20 ) ; // Hydramancer Hydra durability =  20 sec (basic) + seconds from 'Retention' perk
		LimitToRange(missile.TimeToLive, 20 * 20, (20 + 40) * 20 ); // limits in seconds: [ 20 - 60 ]
	}
	else {
		missile.TimeToLive = slvlParam;
		LimitToMax(missile.TimeToLive, 120);
		missile.TimeToLive *= 16;
		LimitToMin(missile.TimeToLive, 120);
	}
	StartAnimationEndFrame = missile.TimeToLive - missile.framesCount;
	missile.value[1] = 0;
	missile.value[2] = 1;
	LimitPlayerHydras (casterIndex);
}

//----- (00433479) --------------------------------------------------------
void __fastcall CastChainLightning( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.value[0] = targetRow;
	missile.value[1] = targetCol;
	missile.TimeToLive = 1;
}

//----- (004334AC) --------------------------------------------------------
void __fastcall CastBlood( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	SetMissileDirection(missileIndex, targetRow);
	missile.value[0] = targetRow;
	missile.Damage = 0;
	missile.flag |= MSF_LIGHT;
	missile.TimeToLive = missile.framesCount;
}

//----- (004334DF) --------------------------------------------------------
void __fastcall CastBone( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	int variationIndex = targetRow;
	if( targetRow > 3 ){
		variationIndex = 2;
	}
	SetMissileDirection(missileIndex, variationIndex);
	missile.Damage = 0;
	missile.flag |= MSF_LIGHT;
	missile.TimeToLive = missile.framesCount;
}

//----- (0043351C) --------------------------------------------------------
void __fastcall CastMetlhit( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	int variationIndex = targetRow;
	if( targetRow > 3 ){
		variationIndex = 2;
	}
	SetMissileDirection(missileIndex, variationIndex);
	missile.Damage = 0;
	missile.flag |= MSF_LIGHT;
	missile.TimeToLive = missile.framesCount;
}

//----- (0043355B) --------------------------------------------------------
void __fastcall CastRunAttack( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[casterIndex];

	MonsterSprite* monsterSprite = monster.SpritePtr;
	int baseMonsterIndex = monsterSprite->baseMonsterIndex;

	int anim = ANIM_1_WALK;
	int action = A_1_WALK_UP;
	switch( baseMonsterIndex ){
	case BM_16_HORNED_BEAST
	  or BM_65_HELL_SPAWN
	  or BM_66_HORNED_DEATH
	  or BM_67_BLACK_HORN
	  or BM_171_MUD_LORD
	  or BM_185_BLOOD_HORN
	  or BM_C_345_HORNED_DEMON
	  or BM_C_346_MUD_RUNNER
	  or BM_C_347_FROST_CHARGER
	  or BM_C_348_OBSIDIAN_LORD
	  or BM_278_LEAPER
	  or BM_197_CEROMORTH:      anim = ANIM_5_SECOND; action = A_7_SECOND; break;
	case BM_89_AZURE_DRAKE
	  or BM_90_CAVE_SNAKE
	  or BM_91_GOLD_SERPENT
	  or BM_C_373_CAVE_VIPER
	  or BM_C_374_FIRE_DRAKE
	  or BM_C_375_GOLD_VIPER
	  or BM_C_376_AZURE_DRAKE
	  or BM_92_ILLUSION_WEAVER: anim = ANIM_2_ATTACK; action = A_4_MELEE; break;
	}
	MonsterAnim* monsterAnimation = &monsterSprite->animation[anim];
	missile.CasterType = CT_1_MONSTER_AND_TRAP;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 18);
	missile.direction = casterDirection;
	missile.spriteFileFlags = 0;

	missile.curAnimationPtr = monsterAnimation->oriented[casterDirection].data;
	missile.animationDelayFrameCount = monsterAnimation->animationDelay;
	missile.framesCount = monsterAnimation->frameCount;

	missile.animWidth = monsterSprite->Pitch;
	missile.animOffset = monsterSprite->StartOfsX;
	missile.FrameStep = 1;
	if( (baseMonsterIndex >= BM_89_AZURE_DRAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER) || (baseMonsterIndex >= BM_C_373_CAVE_VIPER && baseMonsterIndex <= BM_C_376_AZURE_DRAKE) ) {
		missile.FrameIndex = 7;
	}
	// 0043361C
	missile.value[0] = 0;
	missile.value[1] = 0;
	missile.value[2] = 0; // new monster yOffset
	missile.value[3] = 0; // new monster xOffset
	int tilesInCell = 0;
	MonsterPosAdjust(&monsterAnimation->oriented[casterDirection], missile.value[3], missile.value[2], tilesInCell);
	missile.flag |= MSF_LIGHT;
	if( monster.newBossId ){// (16) расширение номера босса 0043362A
		missile.uniqTrans = monster.uniqTrans + 1;
		missile.LightIndex = monster.LightIndex;
	}
	// 00433654
	missile.TimeToLive = 256;
	PutMissile(missileIndex);
}

//----- (0043366C) --------------------------------------------------------
void __fastcall CastSomeAnotherMonsterRun( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[casterIndex];
	
	MonsterSprite* monsterSprite = monster.SpritePtr;
	int baseMonsterIndex = monsterSprite->baseMonsterIndex;
	
	MonsterAnim* monsterAnimation = &monsterSprite->animation[ANIM_1_WALK];

	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.direction = casterDirection;
	missile.spriteFileFlags = 0;

	missile.curAnimationPtr = monsterAnimation->oriented[casterDirection].data;
	missile.animationDelayFrameCount = monsterAnimation->animationDelay;
	missile.framesCount = monsterAnimation->frameCount;

	missile.animWidth = monsterSprite->Pitch;
	missile.animOffset = monsterSprite->StartOfsX;

	missile.FrameStep = 1;
	missile.value[0] = 0;
	missile.value[1] = 0;
	missile.flag |= MSF_LIGHT;
	if( monster.newBossId ){// (17) расширение номера босса 004336B5
		missile.uniqTrans = monster.uniqTrans + 1;
	}
	missile.TimeToLive = 256;
	MonsterMap[ monster.Row ][ monster.Col ] = 0;
	PutMissile(missileIndex);
}

//----- (0043374A) --------------------------------------------------------
void __fastcall CastArcaneStar( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00707F70
	int spellSpeed = Difficulty + 16;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, spellSpeed);

	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);

	if( casterType == CT_0_PLAYER ){
		// 004337C4

			bool isHydraFirebolt = false; //Check if the source is hydra to remove mana cost of every iceball
			for (int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++) {
				int spellIndex2 = MissileIndexes[spellIndexIndex];
				Missile& spell2 = Missiles[spellIndex2];

				int& CurHydraFireboltIndex = spell2.value[2];
				if (spell2.BaseMissileIndex == MI_2_HYDRA && spell2.CasterIndex == casterIndex && CurHydraFireboltIndex == missileIndex) {
					isHydraFirebolt = true;
					break;
				}
			}

		Player& player = Players[casterIndex];
		if( !CastingNovaInProgress && !isHydraFirebolt && !(missile.spellIndex == PS_13_HYDRA && HasTrait(missile.CasterIndex, TraitId::Hydramancer))){
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, missile.spellIndex);
		}
		player.CurLife += 0;
		player.BaseLife += 0;
		if( player.CurLife <= 0 ){
			TryToDie(casterIndex, 0);
		}
	}else if( casterIndex > 0 ){
		// 00716350
		if( IsThisBoss(missile.CasterIndex, UM_579_Andariel) ){// andariel's preset AS damage
			damage = 250 + 200 * Difficulty; // 250 - 1850
		}
		missile.Damage = damage;
		// 00433838 - arcane star colors, by witch type
		int baseMonsterIndex = Monsters[casterIndex].SpritePtr->baseMonsterIndex;
		if (GameMode == GM_CLASSIC) {
			if (baseMonsterIndex == BM_C_322_SNOW_WITCH) {
				SetMissileSprite(missileIndex, MIS_78_SCUBMISBLU);
			}
			else if (baseMonsterIndex == BM_C_323_HELL_SPAWN) {
				SetMissileSprite(missileIndex, MIS_44_SCUBMISD);
			}
			else if (baseMonsterIndex == BM_C_324_SOUL_BURNER) {
				SetMissileSprite(missileIndex, MIS_42_SCUBMISC);
			}
			else {
				SetMissileSprite(missileIndex, MIS_22_FLARE);
			}
		}
		else if( baseMonsterIndex == BM_101_DARK_MISTRESS && !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) && !Dungeon->isQuest){
			SetMissileSprite(missileIndex, MIS_22_FLARE);
		}
		else if( baseMonsterIndex == BM_102_SUCCUBUS && !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) && !Dungeon->isQuest){
			SetMissileSprite(missileIndex, MIS_40_SCUBMISB);
		}
		else if (IsThisBoss(missile.CasterIndex, UM_85_Lustbrood_the_Carnal) && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
			SetMissileSprite(missileIndex, MIS_44_SCUBMISD);
		}
		else if (IsThisBoss(missile.CasterIndex, UM_529_Sybil) && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
			SetMissileSprite(missileIndex, MIS_22_FLARE);
		}
		else if( baseMonsterIndex == BM_103_BLOOD_WITCH && Dungeon->level != 18 ){
			if( !IsThisBoss(missile.CasterIndex, UM_593_Witchmoon) ){
				SetMissileSprite(missileIndex, MIS_44_SCUBMISD);
			}
		}
		else if( baseMonsterIndex == BM_104_DARK_WITCH ){
			if( !IsThisBoss(missile.CasterIndex, UM_319_Lustsoul) ){
				SetMissileSprite(missileIndex, MIS_42_SCUBMISC);
			}
		}
	}
	// 00433896
	if( MissileSprites[missile.spriteIndex].animDirectionsCount == 16 ){
		int orientation = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol);
		SetMissileDirection(missileIndex, orientation);
	}
}

//----- (004338CC) --------------------------------------------------------
void __fastcall CastAcidMissile( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	int orientation = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol);
	SetMissileDirection(missileIndex, orientation);
	if( (missile.DeltaY & 0xFFFF0000) || (missile.DeltaX & 0xFFFF0000) ){ // похоже предотвращение висения плевка в воздухе
		if( casterType == CT_0_PLAYER ){
			if( !CastingNovaInProgress && !(missile.spellIndex == PS_13_HYDRA && HasTrait(missile.CasterIndex, TraitId::Hydramancer))){
				MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, missile.spellIndex);
			}
			missile.TimeToLive = 256;
		}else{
			missile.TimeToLive = 5 * (Monsters[casterIndex].intel + 4);
		}
	}else{
		missile.TimeToLive = 1;
	}
	missile.LightIndex = -1;
	
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	PutMissile(missileIndex);
}

//----- (00433965) --------------------------------------------------------
void __fastcall CastSpell29( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.Damage = damage;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.TimeToLive = 50;
	missile.value[0] = 50 - missile.framesCount;
	missile.value[1] = 0;
}

int PoolOverTime = 0;
//----- (004339A1) --------------------------------------------------------
void __fastcall CastSpell59AcidPools( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	if( missile.SpellLevel < 0 )
    {
        // segment from flask
        missile.TimeToLive = -missile.SpellLevel;
		missile.value[5] = PoolOverTime;
		PoolOverTime = 0;
        //missile.Damage = damage;
    }
    else
    {
        AcidPoolLimiter( missileIndex );
        missile.TimeToLive = RNG(15) + 40 * (Monsters[missile.CasterIndex].intel + 1);// TODO: идиотский рассчет времени жизни
    }
    
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.ColDisplacement = 0;
	missile.RowDisplacement = 0;
	missile.flag |= MSF_LIGHT;
	missile.MayBeIsExploding = 1;
}

//----- (00433A05) --------------------------------------------------------
void __fastcall CastStoneCurse( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	int& lastMonsterTargetAction = missile.value[0];
    int& monsterTargetIndex = missile.value[1];
	
	bool monsterTargetIsFound = false;
	int row, col;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];

			if( To112(row, col) ){
				int monsterNumber = MonsterMap[row][col];
				MonsterHorizontalHitFix(monsterNumber, row, col);
				int monsterIndex = monsterNumber - 1;
				Monster& monsterTarget = Monsters[monsterIndex];
				//Player Golem and Summons should not be stone cursed, as well as dying monsters
				if( monsterIndex < SummonMonsters_Count || is(monsterTarget.CurAction, A_6_DEATH, A_8_FADE_IN, A_9_FADE_OUT, A_14_CHARGE)
				 || monsterTarget.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO || (monsterTarget.ResistImmune & MR_7_IM_CURSE)) {
					continue;
				}
				monsterTargetIsFound = true;
				lastMonsterTargetAction = monsterTarget.CurAction;
				monsterTarget.CurAction = A_15_STONE;
				monsterTargetIndex = monsterIndex;
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:;
	if(!monsterTargetIsFound ){
		missile.IsDeleted = true;
		return;
	}
	// 00433B2E
	missile.Row = row;
	missile.Col = col;
	missile.CasterRow = missile.Row;
	missile.CasterCol = missile.Col;
	Player& player = Players[casterIndex];
	int slvlParam = missile.SpellLevel + 6;
	missile.TimeToLive = slvlParam;
	LimitToMax(missile.TimeToLive, 12);
	missile.TimeToLive *= 16;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_8_STONE_CURSE);
}

//----- (th2) -------------------------------------------------------------
int CalculateLesserSummonsMaxAmount( int slvl )
{
    int result = 3 + slvl / 2;
    LimitToRange( result, 3, SummonLesserAmount );
    return result;
}

//----- (th2) -------------------------------------------------------------
int CalculateCommonSummonsMaxAmount( int slvl )
{
    int result = 1 + slvl / 3;
    LimitToRange( result, 1, SummonCommonAmount );
    return result;
}

//----- (th2) -------------------------------------------------------------
int CalculateGolemMaxAmount(int playerindex)
{
	Player& player = Players[playerindex];

	if (player.fullClassId != PFC_MAGE) {
		return 1;
	}
	else {
		return 1 + PerkValue(PERK_EXTRA_GOLEMS, playerindex);
	}	
}

//----- (th2) -------------------------------------------------------------
int CalculateSummonsMaxAmount( int summonType, int slvl, int playerindex )
{
    switch( summonType ){
        case SUM_LESSER: return CalculateLesserSummonsMaxAmount( slvl ); break;
        case SUM_COMMON: return CalculateCommonSummonsMaxAmount( slvl ); break;
		case SUM_GOLEM:  return CalculateGolemMaxAmount ( playerindex ); break;
        default:                                                        break;
    }
    return 1;
}

//----- (th2) -------------------------------------------------------------
void GetSummonsTypeOffsets( int summonType, int* summonsStartIndex, int* summonsSlotsAmount, int playerindex )
{
    *summonsStartIndex = 0;
    *summonsSlotsAmount = 0;
    
	if (PerkValue(PERK_EXTRA_GOLEMS, playerindex)) { //other offsets for if Perk is on (for Mage only)
		if (summonType == SUM_GOLEM) {
			*summonsStartIndex = SummonGolemOffset;
			*summonsSlotsAmount = CalculateGolemMaxAmount(playerindex);
		} //we are sure here that its a Mage, so we dont take any other summons into account and give all space to golems
	}
	else {
		switch (summonType) {
		case SUM_GOLEM:
			*summonsStartIndex = SummonGolemOffset;
			*summonsSlotsAmount = SummonGolemAmount;
			break;
		case SUM_LESSER:
			*summonsStartIndex = SummonLesserOffset;
			*summonsSlotsAmount = SummonLesserAmount;
			break;
		case SUM_COMMON:
			*summonsStartIndex = SummonCommonOffset;
			*summonsSlotsAmount = SummonCommonAmount;
			break;
		case SUM_GREATER:
			*summonsStartIndex = SummonGreaterOffset;
			*summonsSlotsAmount = SummonGreaterAmount;
			break;
		}
	}
}

//----- (th2) -------------------------------------------------------------
bool IsSummonMonsterOnMap( int monsterIndex )
{
    const Monster& summon = Monsters[monsterIndex];
    return !(summon.Row == 1 && summon.Col == 0);
}

//----- (th2) -------------------------------------------------------------
bool IsSummonAlive( int monsterIndex )
{
    const Monster& summon = Monsters[monsterIndex];
    return IsSummonMonsterOnMap( monsterIndex ) && ( summon.CurrentLife & ~63 ) > 0;
}

//----- (th2) -------------------------------------------------------------
bool IsSummonLessImportant( int lhs, int rhs )
{
    return Monsters[lhs].summonOrder < Monsters[rhs].summonOrder;
}

//----- (th2) -------------------------------------------------------------
void GetAliveSummonsByRef( std::vector<int>& result, int ownerIndex, int summonType )
{
    int summonsStartIndex = 0;
    int summonsSlotsAmount = 0;
    GetSummonsTypeOffsets( summonType, &summonsStartIndex, &summonsSlotsAmount, ownerIndex);
    const int summonsOffset = SummonMonstersPerPlayer_Count * ownerIndex;
    const int summonsBegin = summonsStartIndex + summonsOffset;
    const int summonsEnd = summonsBegin + summonsSlotsAmount;
    for( int summonIndex = summonsBegin; summonIndex < summonsEnd; ++summonIndex ){
        if( IsSummonAlive( summonIndex ) ){
            result.push_back( summonIndex );
        }
    }
}

std::vector<int> GetAliveSummons( int ownerIndex, int summonType )
{
    std::vector<int> aliveSummons; aliveSummons.reserve(SummonMonstersPerPlayer_Count);
    GetAliveSummonsByRef( aliveSummons, ownerIndex, summonType );
    return aliveSummons;
}

//----- (th2) -------------------------------------------------------------
void KillExtraSummons( int ownerIndex, int summonType, int slvl )
{
    int summonsMaxAmount = CalculateSummonsMaxAmount( summonType, slvl, ownerIndex);
    std::vector<int> aliveSummons; aliveSummons.reserve(SummonMonstersPerPlayer_Count);
    GetAliveSummonsByRef( aliveSummons, ownerIndex, summonType );
    int aliveSummonsAmount = aliveSummons.size();
    
    std::sort( aliveSummons.begin(), aliveSummons.end(),
        []( int lhs, int rhs ){ return IsSummonLessImportant( lhs, rhs ); } );
    
    // Kill N less important summons
    int mostersToKill = aliveSummonsAmount - summonsMaxAmount;
    if( mostersToKill > 0 ){
        for( int i = 0; i < mostersToKill; ++i ){
            KillMonsterByPlayer( aliveSummons[i], ownerIndex );
        }
    }
}

//----- (th2) -------------------------------------------------------------
void RecalcSummonOrder( int ownerIndex, int summonType )
{
    std::vector<int> aliveSummons = GetAliveSummons( ownerIndex, summonType );
    
    std::sort( aliveSummons.begin(), aliveSummons.end(),
        []( int lhs, int rhs ){ return IsSummonLessImportant( lhs, rhs ); } );
        
    for( int i = 0, ie = aliveSummons.size(); i != ie; ++i ){
        Monsters[aliveSummons[i]].summonOrder = i;
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall CastUnsummon( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
    Missile& missile = Missiles[missileIndex];
    missile.IsDeleted = true;
    
    if( casterIndex != CurrentPlayerIndex ){
        return;
    }
    
    const int targetNumber = MonsterMap[ targetRow ][ targetCol ];
    
    if( targetNumber ){
        const int targetIndex = abs(targetNumber) - 1;
        if( targetIndex < SummonMonsters_Count ){
            const int summonOwner = targetIndex / SummonMonstersPerPlayer_Count;
            if( casterIndex == summonOwner ){
                if( IsSummonAlive( targetIndex ) ){
                    KillMonsterByPlayer( targetIndex, casterIndex );
                }
            }
        }
    }
}

//----- (00433BA7) --------------------------------------------------------
void __fastcall CastGolem( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	if( casterIndex != CurrentPlayerIndex ){
	    MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, missile.spellIndex);
        missile.IsDeleted = true;
        return;
    }
	
	missile.IsDeleted = false;

	int summonsOffset = SummonMonstersPerPlayer_Count * casterIndex;
	std::bitset<SummonMonsters_Count> summonSlotMask {};
    summonSlotMask.set();

    // Forbid summon slots that used by another summon missile of the same type
    for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; ++spellIndexIndex ){
		int spell2Index = MissileIndexes[spellIndexIndex];
		const Missile& spell2 = Missiles[spell2Index];
		if( spell2.BaseMissileIndex == missile.BaseMissileIndex
			&& spell2Index != missileIndex
			&& spell2.CasterIndex == casterIndex ){
			    summonSlotMask.reset( spell2.value[5] ); // monster index
		}
	}
   
    int summonType = 0;
    switch( missile.BaseMissileIndex ){
        case MI_33_GOLEM:           summonType = 0; break;
        case MI_122_LESSER_SUMMON:  summonType = 1; break;
        case MI_123_COMMON_SUMMON:  summonType = 2; break;
        case MI_124_GREATER_SUMMON: summonType = 3; break;
    }
    
    KillExtraSummons( casterIndex, summonType, missile.SpellLevel );
    
    int summonsMaxAmount = CalculateSummonsMaxAmount( summonType, missile.SpellLevel, casterIndex);
    std::vector<int> aliveSummons = GetAliveSummons( casterIndex, summonType );
    int aliveSummonsAmount = aliveSummons.size();
    
    int summonsStartIndex = 0;
    int summonsSlotsAmount = 0;
    GetSummonsTypeOffsets( summonType, &summonsStartIndex, &summonsSlotsAmount, casterIndex );
    
	int summonsBegin = summonsStartIndex;
    int summonsEnd = summonsBegin + summonsSlotsAmount;

    int summonSlotIndex = -1;
    
    if( aliveSummonsAmount < summonsMaxAmount ){
        // Find unused yet summon slot
        for( int summonIndex = summonsBegin; summonIndex < summonsEnd; summonIndex++ ){
            if( summonSlotMask.test( summonsOffset + summonIndex ) && !IsSummonMonsterOnMap( summonsOffset + summonIndex ) ){
                summonSlotIndex = summonIndex;
                break;
            }
        }
    }
    
    auto getLessImportantSummon = [summonsOffset]( int prev, int current ){
            if( prev == -1 ){ return current; }
            return (Monsters[summonsOffset + prev].summonOrder < Monsters[summonsOffset + current].summonOrder ) ? prev : current;
        };
    
    // Find less important summon to replace with
    if( summonSlotIndex == -1 ){
        
        std::sort( aliveSummons.begin(), aliveSummons.end(),
            []( int lhs, int rhs ){ return IsSummonLessImportant( lhs, rhs ); } );
    
        for( int monsterIndex : aliveSummons ){
            int summonIndex = monsterIndex - summonsOffset;
            if( summonSlotMask.test( monsterIndex ) ){
                summonSlotIndex = summonIndex;
                break;
            }
        }
    }

    if( summonSlotIndex == -1 ){
        missile.IsDeleted = true;
        return;
    }
    
	missile.value[0] = casterRow;
	missile.value[1] = casterCol;
	missile.value[3] = targetRow;
	missile.value[4] = targetCol;
	missile.value[5] = summonsOffset + summonSlotIndex; // monster index
	
	// Kill monster if summon on selected slot is still alive
	if( casterIndex == CurrentPlayerIndex ){
	    if( IsSummonAlive( summonsOffset + summonSlotIndex ) ){
	        KillMonsterByPlayer( summonsOffset + summonSlotIndex, casterIndex );
        }
    }
    
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, missile.spellIndex);
}

//----- (00433C70) --------------------------------------------------------
void __fastcall CastEthereal( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
		
	missile.TimeToLive = 400 + (PerkValue(PERK_ETHEREALITY, casterIndex) * 20);

	//missile.TimeToLive += missile.TimeToLive * player.field_554C >> 7; // time to life is 20 sec, that's enough, thank you
	missile.value[0] = player.CurLife;
	missile.value[1] = player.BaseLife;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_25_ETHEREAL);
	}
}

//----- (00433D0A) --------------------------------------------------------
void __fastcall CastSpell31_Emply( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
}

//----- (00433D1D) --------------------------------------------------------
void __fastcall CastFurySplash( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int picIndex )
{
	Missile& missile = Missiles[missileIndex];
	missile.Damage = picIndex;
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.CasterIndex = casterIndex;
	if( picIndex == 1 ){
		SetMissileDirection(missileIndex, 0);
		missile.value[0] = 0;
	}else{
		SetMissileDirection(missileIndex, 1);
		missile.value[0] = 1;
	}
	// 00433D63
	missile.flag |= MSF_LIGHT;
	missile.TimeToLive = missile.framesCount;
}

//----- (00433D83) --------------------------------------------------------
void __fastcall CastApocExplode( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	int& isApocFragmentHitTarget = missile.value[0];
	
	missile.Row = targetRow;
	missile.Col = targetCol;
	missile.CasterRow = targetRow;
	missile.CasterCol = targetCol;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.Damage = damage;
	missile.TimeToLive = missile.framesCount;
	isApocFragmentHitTarget = 0;
}

//----- (00433DD6) --------------------------------------------------------
void __fastcall CastHealing( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Player& player = Players[casterIndex];
	Missile& missile = Missiles[missileIndex];

	int healingPoints = (RNG(4) + 1) * 64;
	if( player.CharLevel > 0 ){
		int i = 0;
		do{
			healingPoints += (RNG(2) + 1) * 64; // +[1-2] * baseVIT

		}while( ++i < player.BaseVitality / 2);// TODO: несоответствие проверки вначале и тут, баг
	}
	// 00433E2C
	for( int i = 0; i < missile.SpellLevel; i++ ){
		healingPoints += (RNG(2) + 1) * 64; // + [1-2] hit points per SLVL. TOTAL: [1-2] * SLVL + [1-2] * bVIT
	}
	// 00433E60
	int playerClass = player.ClassID;
	if( playerClass == PC_2_MAGE && (!HasTrait(casterIndex, TraitId::Mamluk)) ){// -50%	for Magi, except Mamluk	
		healingPoints /= 2;		
	}else if( player.fullClassId == PFC_EXECUTIONER ){ // +50%
		healingPoints += healingPoints / 2;
	}

	healingPoints += healingPoints * (PerkValue(PERK_AUGMENTED_HEALING, casterIndex) + PerkValue(PERK_HEALING_MASTERY, casterIndex)) / 100;

	// 00433E8D
	player.CurLife  += healingPoints;
	LimitToMax(player.CurLife, player.MaxCurLife);
	player.BaseLife  += healingPoints;
	LimitToMax(player.BaseLife, player.MaxBaseLife);
	// 00433ED9
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_2_HEALING);
	missile.IsDeleted = true;
}

//----- (00433EF9) --------------------------------------------------------
void __fastcall CastManaRecharge( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	int manaAddsCount = (RNG(4) + 1) * 64;
	if( player.CharLevel > 0 ){
		int i = 0;
		do{
			manaAddsCount += (RNG(2) + 1) * 64;
		}while( ++i < player.BaseMagic);// TODO: несоответствие проверки вначале и тут, баг
	}
	// 00433F4F
	for( int i = 0; i < missile.SpellLevel; i++ ){
		manaAddsCount += (RNG(2) + 1) * 64;
	}
	// 00433F83
	int playerClass = player.ClassID;
	if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
		manaAddsCount = 7 * manaAddsCount / 10;
	}
	else if( playerClass == PC_5_SAVAGE && player.fullClassId != PFC_DRUID){
		manaAddsCount /= 9;
	}
	else if( playerClass != PC_2_MAGE ){
		manaAddsCount /= 5;
	}
	else {
		manaAddsCount = 4 * manaAddsCount / 10; // mage classes now have less mana recovered from Mana spell
	}
	// 00433FA7
	player.CurMana += manaAddsCount;
	LimitToMax(player.CurMana, player.MaxCurMana);
	// 00433FC7
	player.BaseMana += manaAddsCount;
	LimitToMax(player.BaseMana, player.MaxBaseMana);
	// 00433FF3
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_37_MANA_RECHARGE);
	missile.IsDeleted = true;
}

//----- (00434013) --------------------------------------------------------
void __fastcall CastMagi( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	player.CurMana = player.MaxCurMana;
	player.BaseMana = player.MaxBaseMana;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_38_MAGI);
	missile.IsDeleted = true;
}

//----- (0043406C) --------------------------------------------------------
void __fastcall CastHealOther( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];

//	if (!CheckCastInSight(casterRow, casterCol, targetRow, targetCol)) {//Cannot heal behind wall or lamp
//		missile.IsDeleted = true;
//		return;
//	}

	if (casterIndex != CurrentPlayerIndex) {//Fix for Heal Other applies # of players times in MP
		missile.IsDeleted = true;
		return;
	}

	int radius = 3;
	int HealedPlayerIndex = -1;
	int CursorPlayerDistance = 44444;//Make it large enough initially

	for (int row = targetRow - radius, rowEnd = targetRow + radius; row <= rowEnd; ++row) {
		for (int col = targetCol - radius, colEnd = targetCol + radius; col <= colEnd; ++col) {
			if (To112(row, col)) {
				int SearchedPlayerIndex = abs(PlayerMap[row][col]) - 1;
				if (SearchedPlayerIndex == -1 || SearchedPlayerIndex == casterIndex) {//Heal Other does NOT heal caster himself
					continue;
				}
				int SearchedDistance = (targetRow - row) * (targetRow - row) + (targetCol - col) * (targetCol - col);//Squared distance
				if (SearchedDistance < CursorPlayerDistance) {//Update target player index only if it's closer
					HealedPlayerIndex = SearchedPlayerIndex;
					CursorPlayerDistance = SearchedDistance;
				}
			}
		}
	}

	if (HealedPlayerIndex == -1) {//Don't heal if no target player is found
		missile.IsDeleted = true;
		return;
	}

	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_34_HEAL_OTHER);
	NetSendCmdParam1(0, CMD_85_HEALOTHER, HealedPlayerIndex);
}

//----- (0043409F) --------------------------------------------------------
void __fastcall CastElemental( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
    
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int& HomingStatus = missile.value[2];
    int& homingBiginRow = missile.value[3];
    int& homingBiginCol = missile.value[4];
	
	Player& player = Players[casterIndex];

	if( casterRow == targetRow && casterCol == targetCol ){
		targetCol += ColDelta[casterDirection];
		targetRow += RowDelta[casterDirection];
	}

	// 004340D3
	//missile.Damage = 2 * (player.CharLevel + RNG(40) + RNG(40)) + 70; // old damage
	missile.Damage = ((player.CharLevel * 4 / 5) + RNG(player.CurMagic * 2 / 5 + 1) + (missile.SpellLevel * 7)); // ~60% in strength of fireblast
	/*for( int i = 0; i < missile.SpellLevel; i++ ){
		missile.Damage += missile.Damage / 16;
	}*/
	// 00434134
	//missile.Damage /= 2;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	lastRow = casterRow;
	lastCol = casterCol;
	HomingStatus = 0;
	homingBiginRow = targetRow;
	homingBiginCol = targetCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
	MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, PS_29_ELEMENTAL );
}

// в тх2 заменена на универсальную, хоть и более медленную SelectMissileDirection
char DirTable_8[16][16] = {
	{	2,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0	},// 0
	{	2,	1,	1,	1,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0	},// 1
	{	2,	1,	1,	1,		1,	1,	1,	0,		0,	0,	0,	0,		0,	0,	0,	0	},// 2
	{	2,	1,	1,	1,		1,	1,	1,	1,		1,	0,	0,	0,		0,	0,	0,	0	},// 3

	{	2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,		0,	0,	0,	0	},// 4
	{	2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	0,	0	},// 5
	{	2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 6
	{	2,	2,	2,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 7

	{	2,	2,	2,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 8
	{	2,	2,	2,	2,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 9
	{	2,	2,	2,	2,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 10
	{	2,	2,	2,	2,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 11

	{	2,	2,	2,	2,		2,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 12
	{	2,	2,	2,	2,		2,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 13
	{	2,	2,	2,	2,		2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 14
	{	2,	2,	2,	2,		2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1	},// 15
};
uchar FirstQuadrantDirections_8 [3] = { 7, 0, 1 };
uchar FourthQuadrantDirections_8[3] = { 7, 6, 5 };
uchar SecondQuadrantDirections_8[3] = { 3, 2, 1 };
uchar ThirdQuadrantDirections_8 [3] = { 3, 4, 5 };
//----- (004341B8) -------------------------------------------------------
int __fastcall SelectMissileDirectionFrom8(int casterRow, int casterCol, int targetRow, int targetCol)
{

	int deltaRow = abs(targetRow - casterRow);
	LimitToMax(deltaRow, 15);
	int deltaCol = abs(targetCol - casterCol);
	LimitToMax(deltaCol, 15);
	int directionIndex = DirTable_8[deltaCol][deltaRow];
	if( casterRow <= targetRow ){
		if( casterCol <= targetCol ){
			return FirstQuadrantDirections_8[directionIndex];
		}else{
			return FourthQuadrantDirections_8[directionIndex];
		}
	}else{
		if( casterCol <= targetCol ){
			return SecondQuadrantDirections_8[directionIndex];
		}else{
			return ThirdQuadrantDirections_8[directionIndex];
		}
	}
}

//----- (004347EE) --------------------------------------------------------
void __fastcall CastIdentify( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterIndex == CurrentPlayerIndex && Cur.GraphicsID == CM_1_NORMAL_HAND ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_5_IDENTIFY);
		IsSpellBookVisible = false;
		IsINVPanelVisible = true;
		SetCursorGraphics(CM_2_IDENTIFY);
		Cur.IsTradeService = false;
	}
}

//----- (th4) -------------------------------------------------------------
bool LocateWallSegments(int casterRow, int casterCol, int targetRow, int targetCol, int* row /*= 0*/, int* col /*= 0*/)
{
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int r = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int c = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( To112(r, c) && CheckCastInSight(casterRow, casterCol, r, c) 
			 && (casterRow != r || casterCol != c) 
			 && !TileBlockWalking[FineMap[ r ][ c ]] 
			 && !ObjectsMap[ r ][ c ] ){
				if( row && col ){
					*row = r;
					*col = c;
				}
				return true;
			}
		}
	}
	return false;
}

//----- (0043483F) --------------------------------------------------------
void __fastcall CastMagicWall( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	int row, col;
	if( LocateWallSegments(casterRow, casterCol, targetRow, targetCol, &row, &col) ){
		missile.IsDeleted = false;
		missile.value[0] = row;                       // wallFirstBeamRow
		missile.value[1] = col;                       // wallFirstBeamCol
		missile.value[2] = (casterDirection - 2) & 7; // firstBeamOrientation
		missile.value[3] = (casterDirection + 2) & 7; // secondBeamOrientation
		missile.value[4] = row;                       // wallSecondBeamRow
		missile.value[5] = col;                       // wallSecondBeamCol
		missile.value[6] = 0;                         // isSecondBeamBlocked
		missile.value[7] = 0;                         // isFirstBeamBlocked
		missile.TimeToLive = 7; // длина стены в каждую сторону от точки каста
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, missile.spellIndex);// TODO: и у молниевой стены и у огненной снимается как у огненной
	}
}

//----- (0043497B) --------------------------------------------------------
void __fastcall Cast_Fire_or_Lightning_Ring( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, missile.spellIndex);
	}
	missile.value[0] = casterRow;
	missile.value[1] = casterCol;
	missile.IsDeleted = false;
	missile.value[2] = 0;
	missile.value[3] = 0;
	missile.value[4] = 0;
	missile.value[5]  = 0;
	missile.value[6] = 0;
    int& isFirstBeamBlocked = missile.value[7];
	isFirstBeamBlocked = 0;
	missile.TimeToLive = 7;
}

//----- (00434AF1) --------------------------------------------------------
void __fastcall CastInfravision( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	missile.TimeToLive = 4096;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_9_INFRAVISION);
	}
}

//----- (00434B64) --------------------------------------------------------
void __fastcall Cast_Forcewave( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.value[0] = targetRow;
	missile.value[1] = targetCol;
	missile.value[2] = 0;
	missile.value[3] = 0;
	missile.TimeToLive = 1;
	missile.FrameIndex = 4;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_15_FORCE_WAVE);
}

//----- (00434BAF) --------------------------------------------------------
void __fastcall CastNova( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	CastingNovaInProgress = true;
	Missile& missile = Missiles[missileIndex];
	missile.value[0] = targetRow;
	missile.value[1] = targetCol;
	
	if( casterIndex == M1_TRAP ){ // trap nova damage
		missile.Damage = 3 + RNG(16) + 2 * Dungeon->level + Difficulty * 34;// mor: this looks like THE formula for lightning nova trap. not sure yet
	}else{ // player
		missile.Damage = GetDamageOfPlayerSpell( casterIndex, missile.spellIndex, missile.SpellLevel, BaseMissiles[missile.BaseMissileIndex].DamageType );
		MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, missile.spellIndex );
	}
	missile.TimeToLive = 1;
}

//----- (00434CB5) --------------------------------------------------------1.145
void __fastcall CastFury( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Player& player = Players[casterIndex];
	Missile& missile = Missiles[missileIndex];
	
	if( casterIndex == -1 || player.activeBuffFlag & (BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY) // casterIndex == -1 это ловушка/моб ?
	 /*|| player.CurLife <= player.CharLevel * 64*/ ){ // was commented what this condition is extra 
		missile.IsDeleted = true; // repeated Fury deletes immediately 
	}else{
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_22_FURY);
		missile.value[0] = casterIndex; // playerIndex
		player.activeBuffFlag |= BF_2_FURY_ACTIVE;
		if( player.fullClassId == PFC_DRUID ){
			player.LoadPlayerAnimationMask = PAF_0_NO;
		}
		if (player.fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
			BattleTrance = true;
		}
		missile.value[1] = (6 * player.CharLevel) * 64; // duration of BF_3_FURY_LETARGY phase
		int casterLevelParam = (uint) casterIndex < 4 ? (player.CharLevel / 5) : 1; // was original TH1 bug: casterIndex > 0
		int additionalFuryDuration = player.effectFlag[EA_FURY_DURATION] ? (player.CharLevel / 6 + 10) : 0; // was original TH1 bug: current player
		missile.TimeToLive = (30 + casterLevelParam + /*10 * missile.SpellLevel +*/ additionalFuryDuration + PerkValue(PERK_RAMPAGE, casterIndex)) * 20 ;
		CalcCharParams(casterIndex, 1);
		int si = -1;
		switch ( player.fullClassId ) {
			case PFC_WARRIOR:
			case PFC_INQUISITOR:
			case PFC_GUARDIAN:
			case PFC_TEMPLAR:			si = S_800_WAR_FURY; break;	// PlayLocalSound( S_800_WAR_FURY, player.Row, player.Col ); break;
			case PFC_ARCHER:
			case PFC_SCOUT:
			case PFC_SHARPSHOOTER:
			case PFC_TRAPPER:			si = S_691_ROG_71B; break;
			case PFC_MAGE:
			case PFC_ELEMENTALIST:
			case PFC_DEMONOLOGIST:
			case PFC_NECROMANCER:
			case PFC_BEASTMASTER:
			case PFC_WARLOCK:			si = /*S_588_SOR_70*/ S_1529; break;
			case PFC_MONK:
			case PFC_KENSEI:
			case PFC_SHUGOKI:
			case PFC_SHINOBI:			si = S_1183; break;
			case PFC_ROGUE:
			case PFC_ASSASSIN:
			case PFC_IRON_MAIDEN:
			case PFC_BOMBARDIER:
				switch ( RNG(2) ) {
					case 0:	si = S_1207; break;
					case 1:	si = S_1208; break;
					}
				break;
			case PFC_SAVAGE:
				switch ( RNG(2) ) {
					case 0:	si = S_1201; break;
					case 1:	si = S_1202; break;
					}
				break;
			case PFC_BERSERKER:
				switch ( RNG(2) ) {
					case 0:	si = S_1197; break;
					case 1:	si = S_1198; break;
					}
				break;
			case PFC_EXECUTIONER:
				switch ( RNG(4) ) {
					case 0:	si = S_1176; break;
					case 1:	si = S_1176; break;
					case 2:	si = S_1176; break;
					case 3:	si = S_1176; break;
				}
				break;
			case PFC_MURMILLO:
				switch ( RNG(4) ) {
					case 0:	si = S_1184; break;
					case 1:	si = S_1194; break;
					case 2:	si = S_1195; break;
					case 3:	si = S_1196; break;
				}
				break;
			case PFC_THRAEX:
				switch ( RNG(2) ) {
					case 0:	si = S_1205; break;
					case 1:	si = S_1206; break;
				}
				break;
			case PFC_DIMACHAERUS:
				switch ( RNG(2) ) {
					case 0:	si = S_1203; break;
					case 1:	si = S_1204; break;
				}
				break;
			case PFC_SECUTOR:
				switch ( RNG(2) ) {
					case 0:	si = S_1199; break;
					case 1:	si = S_1200; break;
				}
				break;
			case PFC_DRUID:
				switch ( RNG(2) ) {
					case 0:	si = S_1812_WW_FURY; break;
					case 1:	si = S_1812_WW_FURY; break;// add another later!
				}
				break;
		}
		if( si != -1 ){
			PlayLocalSound ( si, player.Row, player.Col );
		}
	}
}

//----- (00434DD7) --------------------------------------------------------
void __fastcall CastItemRepair( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterIndex == CurrentPlayerIndex && Cur.GraphicsID == CM_1_NORMAL_HAND ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_26_ITEM_REPAIR);
		IsSpellBookVisible = false;
		IsINVPanelVisible = true;
		SetCursorGraphics(CM_3_REPAIR);
		Cur.IsTradeService = false;
	}
}

//----- (00434E28) --------------------------------------------------------
void __fastcall CastStaffRecharge( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterIndex == CurrentPlayerIndex && Cur.GraphicsID == CM_1_NORMAL_HAND ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_27_STAFF_RECHARGE);
		IsSpellBookVisible = false;
		IsINVPanelVisible = true;
		SetCursorGraphics(CM_4_STAFF_RECHARGE);
		Cur.IsTradeService = false;
	}
}

//----- (00434E79) --------------------------------------------------------
void __fastcall CastTrapDisarm( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterIndex == CurrentPlayerIndex && Cur.GraphicsID == CM_1_NORMAL_HAND ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_28_TRAP_DISARM);
		SetCursorGraphics(CM_5_TRAP_DISARM);
	}
}

//----- (00434EAC) --------------------------------------------------------
void __fastcall CastApocalypse( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	int& apocRadius = missile.value[0];
	int& apocMinCol = missile.value[1];
	int& apocMaxCol = missile.value[2];
	int& apocMinRow = missile.value[3];
	int& apocMaxRow = missile.value[4];
	int& curApocCol = missile.value[5];

	apocRadius = 8;
	apocMinCol = casterCol - apocRadius;
	apocMaxCol = casterCol + apocRadius;
	apocMinRow = casterRow - apocRadius;
	apocMaxRow = casterRow + apocRadius;
	curApocCol = apocMinCol;

	LimitToMin(apocMinCol, 1);
	LimitToMax(apocMaxCol, FineMap_112 - 1);
	LimitToMin(apocMinRow, 1);
	LimitToMax(apocMaxRow, FineMap_112 - 1);

	// 00434F42
	/*for( int i = 0; i < player.CharLevel; i++ ){
		missile.Damage += RNG(player.CharLevel) + 1;
	}*/

	// old Apoc damage
	// missile.Damage = player.CharLevel * player.CurMagic / 4;

	missile.Damage = (player.CharLevel * player.CharLevel) + ( player.CharLevel * player.CurMagic / 15 );


	// 00434F7E
	missile.TimeToLive = 255;
	missile.IsDeleted = false;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_24_APOCALYPSE);
}

//----- (00434FA1) --------------------------------------------------------
void __fastcall CastIncinerateSegment( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int incinerateIndex, int casterType, int casterIndex, int fragmentIndex )
{
	Missile& missile = Missiles[missileIndex];
	missile.value[1] = 5 * fragmentIndex;
	// 00434FCA
	missile.CasterRow = targetRow;
	missile.CasterCol = targetCol;
	Missile& missile2 = Missiles[incinerateIndex];// ссылка на инферно который создал этот фрагмент
	missile.ColDisplacement = missile2.ColDisplacement;
	missile.RowDisplacement = missile2.RowDisplacement;
	missile.WholeDeltaY = missile2.WholeDeltaY;
	missile.WholeDeltaX = missile2.WholeDeltaX;
	missile.TimeToLive = missile.value[1] + 20;
	missile.LightIndex = AddLight(casterRow, casterCol, 1);
	if( casterType == CT_0_PLAYER ){
		// 0043503F
		Player& player = Players[casterIndex];
		//missile.Damage = ( 8 * ( RNG(50 + player.CharLevel) ) + 16 ) * 16 ;// old formula
		missile.Damage = RNG(player.CharLevel * player.CharLevel) + ((player.CharLevel * player.CurMagic) / 3);
	}else{
		// 0043507D
		Monster& monster = Monsters[casterIndex];
		int damage = monster.ArmorClass + RNG(16 * monster.ArmorClass);
		if( IsThisBoss(casterIndex, UM_582_Izual) ){
			if (is(GameMode, GM_NORMAL, GM_SURVIVAL, GM_NIGHTMARE )) {
				damage *= 4;
			}
			else if (GameMode == GM_HARD) {
				damage *= 5;
			}
			else {
				damage *= 3;
			}
		}
		if (IsThisBoss(casterIndex, UM_595_The_Cerberus)) {// Cerberus, +100% damage
			damage *= 2;
		}
		missile.Damage = damage;
	}
}

//----- (004350B2) --------------------------------------------------------
void __fastcall CastIncinerate( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004350DF
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 32);
	if( casterType == CT_0_PLAYER ){// человек
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_20_INCINERATE);
	}
	// 004350FE
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
    int& CountOfIncinerateFragmentsCasted = missile.value[2];
	
	lastRow = casterRow;
	lastCol = casterCol;
	CountOfIncinerateFragmentsCasted = 0;
	missile.TimeToLive = 256;
}

//----- (0043512A) --------------------------------------------------------
void __fastcall CastChargedBolt( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	int& LightLevelIndex = missile.value[0];
    int& mainDirection = missile.value[1];
    int& changeDirectionDelay = missile.value[2];
    
	Player& player = Players[casterIndex];
	if( casterType != CT_0_PLAYER ){
		// 00710F00
		int damage = Dungeon->level * 4 + Difficulty * 90;
		missile.chargedBoltChangeDirectionParam = RNG(15) + 1;
		missile.Damage = damage;
	}else{
		missile.chargedBoltChangeDirectionParam = RNG(15) + 1;
		missile.Damage = RNG(3 * player.CurMagic / 2) + 1; // 00435140
	}
	// 0043519E
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004351C5
	missile.FrameIndex = RNG(8) + 1;
	missile.LightIndex = AddLight(casterRow, casterCol, 5);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 8);
	LightLevelIndex = 5;
	mainDirection = casterDirection;
	changeDirectionDelay = 0;
	missile.TimeToLive = 256;
}

//----- (th4) -------------------------------------------------------------
void __fastcall CastHydramancerChargedBolt(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];

	int& LightLevelIndex = missile.value[0];
	int& mainDirection = missile.value[1];
//	int& changeDirectionDelay = missile.value[2];// Does not change direction

	Player& player = Players[casterIndex];
	if (casterType != CT_0_PLAYER) {
		// 00710F00
		int damage = Dungeon->level * 4 + Difficulty * 90;
//		missile.chargedBoltChangeDirectionParam = RNG(15) + 1;
		missile.Damage = damage;
	} else {
//		missile.chargedBoltChangeDirectionParam = RNG(15) + 1;
//		missile.Damage = RNG(3 * player.CurMagic / 2) + 1; // 00435140
		missile.Damage = damage;// Use damage set by PS_13_HYDRA
	}
	// 0043519E
	if (casterRow == targetRow && casterCol == targetCol) {
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004351C5
	missile.FrameIndex = RNG(8) + 1;
	missile.LightIndex = AddLight(casterRow, casterCol, 5);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);// Increased speed, originally 8
	LightLevelIndex = 5;
	mainDirection = casterDirection;
//	changeDirectionDelay = 0;
	missile.TimeToLive = 256;
}

//----- (00435225) --------------------------------------------------------
void __fastcall CastChargedBoltArrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	int& LightLevelIndex = missile.value[0];
    int& mainDirection = missile.value[1];
    int& changeDirectionDelay = missile.value[2];
    
	if( casterType != CT_0_PLAYER ){
		missile.Damage = 15;
	}
	missile.chargedBoltChangeDirectionParam = RNG(15) + 1;
	// 00435277
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 0043529E
	missile.FrameIndex = RNG(8) + 1;
	missile.LightIndex = AddLight(casterRow, casterCol, 5);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 8);
	LightLevelIndex = 5;
	mainDirection = casterDirection;
	changeDirectionDelay = 0;
	missile.TimeToLive = 256;
}

//----- (004352FF) --------------------------------------------------------
void __fastcall CastHolyBolt( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00435331
	int missileSpeed;
	if( casterIndex != -1 ){
		// 00435337
		missileSpeed = missile.SpellLevel + 10;
	}else{
		// 00435353
		missileSpeed = 16;
	}
	LimitToMax(missileSpeed, 30);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
	missile.Damage = GetDamageOfPlayerSpell(casterIndex, missile.spellIndex, missile.SpellLevel, BaseMissiles[missile.BaseMissileIndex].DamageType);
	if( !CastingNovaInProgress ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_31_HOLY_BOLT);
	}
}

//----- (00705580) --------------------------------------------------------
void __fastcall CastBallLightning( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& spell = Missiles[spellCastIndex];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 007055B5
	Player& player = Players[casterIndex];
	/*spell.Damage = RNG(player.CharLevel * 32) + RNG(100) + player.CharLevel + 100; // <- old damage formula

	for( int i = 0; i < spell.SpellLevel; i++){
		spell.Damage += spell.Damage / 16;
	}*/
	spell.Damage = (player.CharLevel + RNG(player.CurMagic / 2) + (spell.SpellLevel * 10)) * 4;
	// 00431E70
	int missileSpeed = (spell.SpellLevel / 3) + 20 + RNG(spell.SpellLevel / 3 + 1);
	LimitToMax(missileSpeed, 40);
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_14_BALL_LIGHTNING);
	// 00431E95
	SetMissileMovementDelta(spellCastIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(spellCastIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	spell.TimeToLive = 256;
	int& lastRow = spell.value[0];
	int& lastCol = spell.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	spell.value[2] = 0;
	spell.value[3] = casterRow;
	spell.value[4] = casterCol;
	spell.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (004353E6) --------------------------------------------------------
void __fastcall CastSpell77HolyBoltArrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00435413
	int missileSpeed = 16;
	if( casterIndex != -1 ){// ловушка?
		missileSpeed = 2 * missile.SpellLevel + 16;
	}
	LimitToMax(missileSpeed, 63);
	// 00435437
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	int orientation = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol);
	SetMissileDirection(missileIndex, orientation);
	missile.TimeToLive = 256;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	lastRow = casterRow;
	lastCol = casterCol;
	Player& player = Players[casterIndex];
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (00435492) --------------------------------------------------------
void __fastcall CastSpellRising( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterIndex == CurrentPlayerIndex && Cur.GraphicsID == CM_1_NORMAL_HAND ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_32_BONE_SPIRIT_REAL_RESSURECT);// TODO: заклинание воскрешения похоже отсутствует в списке заклинаний и вместо него теперь бон спирит
		SetCursorGraphics(CM_8_RISING);
	}
}

//----- (004354C9) --------------------------------------------------------
void __fastcall Cast62_RisingLight( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.Row = targetRow;
	missile.Col = targetCol;
	missile.CasterRow = missile.Row;
	missile.CasterCol = missile.Col;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.TimeToLive = MissileSprites[MIS_36_RESSUR1].animFrames[0];
}

//----- (00435517) --------------------------------------------------------
void __fastcall CastTelekinesis( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterIndex == CurrentPlayerIndex && Cur.GraphicsID == CM_1_NORMAL_HAND ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_33_TELEKINES);
		SetCursorGraphics(CM_7_TELEKINESIS);
	}
}

//----- (0043554A) --------------------------------------------------------
void __fastcall CastBoneSpirit( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
    
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int& HomingStatus = missile.value[2];
    int& homingBiginRow = missile.value[3];
    int& homingBiginCol = missile.value[4];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow = RowDelta[casterDirection] + targetRow;
		targetCol = ColDelta[casterDirection] + targetCol;
	}
	// 00435579
	missile.Damage = 0;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 10);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	
 
	lastRow = casterRow;
	lastCol = casterCol;
	HomingStatus = 0;
	
	homingBiginRow = targetRow;
	homingBiginCol = targetCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
	if( casterType == CT_0_PLAYER ){
		Player& player = Players[casterIndex];
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_36_BONE_SPIRIT);
		int damage = 6 * 64;
		player.CurLife -= damage;
		player.BaseLife -= damage;
		if( player.CurLife <= 0 ){
			TryToDie(casterIndex, 0);
		}
	}
}

//----- (00435659) --------------------------------------------------------
void __fastcall CastRedPortal( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	int& StartAnimationEndFrame = missile.value[0];
    int& Lightness = missile.value[1];
	
	if( IsQuestOnLevel(Q_20_SUNLESS_SEA) || Dungeon == DUN_40_ISLAND ){
		SetMissileSprite(missileIndex, MIS_15_EMPTY);
	}
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.TimeToLive = 100;
	StartAnimationEndFrame = 100 - missile.framesCount;
	Lightness = 0;
	PutMissile(missileIndex);
}

//----- (004356A3) --------------------------------------------------------
void __fastcall CastMonsterApoc( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	for( int playerIndex = 0; playerIndex < MaxCountOfPlayersInGame; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists ){
			if( CheckCastInSight(casterRow, casterCol, player.Row, player.Col) ){
				// 004356DF
				CastMissile(0, 0, player.Row, player.Col, 0, MI_66_MONSTER_APOC_EXPLODE, casterType, casterIndex, damage, 0, missile.spellIndex);
			}
			int summonOffset = playerIndex * SummonMonstersPerPlayer_Count;
			for( int summonIndex = 0; summonIndex < SummonMonstersPerPlayer_Count; ++summonIndex ){
			    Monster& summon = Monsters[summonOffset + summonIndex];
			    if( ! (summon.flag & MF_15_KILLED) ){
			        if( CheckCastInSight(casterRow, casterCol, summon.Row, summon.Col) ){
                        CastMissile(0, 0, summon.Row, summon.Col, 0, MI_66_MONSTER_APOC_EXPLODE, casterType, casterIndex, damage, 0, missile.spellIndex);
                    }
                }
            }
		}
	}
	missile.IsDeleted = true;
}

//----- (th4) -------------------------------------------------------------
void __fastcall CastTentacle(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	Monster& monster = Monsters[missile.CasterIndex];
	monster.flag |= MF_16_TRACK_ATTACK;
	missile.Damage = RngFromRange(monster.SecondMinDamage, monster.SecondMaxDamage);
	missile.Damage = 7 * missile.Damage / 5; // tentacle damage increased by 40%
	int missileSpeed = 11 + Difficulty / 2;
	LimitToMax(missileSpeed, 40);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.value[0] = casterRow; // lastRow 
	missile.value[1] = casterCol; // lastCol 
	missile.value[2] = targetRow; // targetRow 
	missile.value[3] = targetCol; // targetCol 
	missile.value[4] = missile.Row;
	missile.value[5] = missile.Col;
	missile.value[6] = missile.RowDisplacement;
	missile.value[7] = missile.ColDisplacement;
	missile.LightIndex = -1;// AddLight(casterRow, casterCol, 8);
}

//----- (th4) -------------------------------------------------------------
void __fastcall ActTentacle(int missileIndex)
{
	if( ! IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[missile.CasterIndex];
	missile.TimeToLive--;
	if( missile.spriteIndex == MIS_86_SPIKE ){
		if( missile.TimeToLive <= 0 ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}else{
			if( ! missile.isHitTarget ){ // commit condition to return all frame tick damage
				MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 1);
			}
		}
		PutMissile(missileIndex);
	}else{ // MIS_85_TENTACLE1
		swap(missile.Row, missile.value[4]); // set real missile pos
		swap(missile.Col, missile.value[5]);
		swap(missile.RowDisplacement, missile.value[6]);
		swap(missile.ColDisplacement, missile.value[7]);
		if( max(abs(missile.Row - monster.Row), abs(missile.Col - monster.Col)) < 2 ){
			missile.FrameIndex = 1;
			missile.HasActSprite = false;
		}else{
			missile.HasActSprite = true; // draw tentacle on floor at range >= 2 (over the cell)
		}
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( missile.Row == missile.value[2] && missile.Col == missile.value[3] ){ // target row/col
			missile.TimeToLive = 0;
			missile.RowDisplacement = 0; // 16 ?
			missile.ColDisplacement = 0; // 16 ?
		}
		if( missile.TimeToLive > 0 ){
			int& lastRow = missile.value[0];
			int& lastCol = missile.value[1];
			if( missile.Row != lastRow || missile.Col != lastCol ){
				lastRow = missile.Row;
				lastCol = missile.Col;
				ChangeLight(missile.LightIndex, missile.Row, missile.Col, 8);
			}
			if( missile.FrameIndex == 1 ){ // reset tentacle on floor pos
				missile.value[4] = missile.Row;
				missile.value[5] = missile.Col;
				missile.value[6] = missile.RowDisplacement;
				missile.value[7] = missile.ColDisplacement;
			}else{
				swap(missile.Row, missile.value[4]); // set tentacle on floor pos for drawing
				swap(missile.Col, missile.value[5]);
				swap(missile.RowDisplacement, missile.value[6]);
				swap(missile.ColDisplacement, missile.value[7]);
			}
			PutMissile(missileIndex);
		}else{ // TimeToLive <= 0
			ChangeLight(missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex );
			SetMissileSprite(missileIndex, MIS_86_SPIKE);
			missile.HasActSprite = true;
			missile.TimeToLive = missile.framesCount; // * (missile.animationDelayFrameCount - 1); // can used only after animDelay will be fixed
			PutMissile(missileIndex);
		}
	}
}

//----- (0043572F) --------------------------------------------------------
int __fastcall CastMissile(int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection_missileIndex, int baseMissileIndex, int casterType, int casterIndex, int damage, int spellLevel, int spellIndex, const int(&value)[8], bool isSpriteToBeHidden /*= false*/, int flag /*=0*/)
{
	if( MissileAmount >= MissilesMax - 1 ) return -1;
	if( is(baseMissileIndex, MI_39_INFRAVISION, MI_13_MANA_SHIELD, MI_79_REFLECT, MI_34_ETHEREAL) ){
		for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; missileIndexIndex++ ){
			Missile& missile = Missiles[MissileIndexes[missileIndexIndex]];
			if( missile.BaseMissileIndex == baseMissileIndex && missile.CasterIndex == casterIndex ) return -1;
		}
	}
	int missileIndex = MissileIndexList[0];
	MissileIndexList[0] = MissileIndexList[MissilesMax - 1 - MissileAmount];
	MissileIndexes[MissileAmount++] = missileIndex;
	Missile& missile = Missiles[missileIndex];
	BaseMissile& baseMissile = BaseMissiles[baseMissileIndex];
	memset(&missile, 0, sizeof( Missile ));
	missile.BaseMissileIndex = baseMissileIndex;
	missile.CasterType = casterType;
	missile.CasterIndex = casterIndex;
	missile.spriteIndex = baseMissile.Image;
	if (!isSpriteToBeHidden) {
		missile.HasActSprite = baseMissile.HasSprite;
	} else {
		missile.HasActSprite = false;//Must check for this again in Draw.cpp to make missile invisible
	}
	missile.SpellLevel = spellLevel;
	int casterDirection = casterDirection_missileIndex;
	missile.direction = casterDirection;
	//__debugbreak();
	if( missile.spriteIndex == MIS_M1_NONE || MissileSprites[missile.spriteIndex].animDirectionsCount < 8 ){// Если имеются касты не для всех 8ми направлений выбираем 0е направление
		casterDirection = 0;
	}
	SetMissileDirection(missileIndex, casterDirection);
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.ColDisplacement = 0;
	missile.RowDisplacement = 0;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.WholeDeltaY = 0;
	missile.WholeDeltaX = 0;
	missile.IsDeleted = false;
	missile.FrameStep = 1;
	missile.flag = flag;
	missile.MayBeIsExploding = 0;
	missile.uniqTrans = 0;
	missile.Damage = damage;
	missile.isHitTarget = 0;
	missile.Distance = 0;
	missile.LightIndex = -1;
	missile.chargedBoltChangeDirectionParam = 0;
	missile.spellIndex = spellIndex;
	if( baseMissile.CastSound != S_M1_NO_SOUND ){
		PlayLocalSound(baseMissile.CastSound, missile.Row, missile.Col);
	}
	memcpy(missile.value, value, sizeof(value));
	missile.timeFrame = 0;
	missile.drawDelay = 0;
	if( casterType == CT_1_MONSTER_AND_TRAP && casterIndex >= SummonMonsters_Count ){
		switch( Monsters[casterIndex].SpritePtr->baseMonsterIndex ){
		case BM_259_WATER_WYRM or BM_263_RED_WYRM: missile.drawDelay = 8; break;
		}
	}
	baseMissile.castFunc(missileIndex, casterRow, casterCol, targetRow, targetCol, casterDirection_missileIndex, casterType, casterIndex, damage);
	return missileIndex;
}

struct Hydra { int elem, missile, spell; } Hydras[] = {
	{ ET_1_FIRE     , MI_1_FIREBOLT                  , PS_1_FIREBOLT      },
	{ ET_2_LIGHTNING, MI_144_HYDRAMANCER_CHARGED_BOLT, PS_30_CHARGED_BOLT },
	{ ET_3_ARCAN    , MI_24_ARCANE_STAR              , PS_35_ARCANE_STAR  },
	{ ET_4_ACID     , MI_137_RANCID_BOLT             , PS_59_RANCID_BOLT  },
};
//----- (004358C1) --------------------------------------------------------
bool __fastcall AttackCellByHydra( int missileIndex, int row, int col )
{
	Missile& missile = Missiles[missileIndex];
	if( CheckCastInSight(missile.Row, missile.Col, row, col) ){
		int monsterNumber = MonsterMap[ row ][ col ];
		MonsterHorizontalHitFix(monsterNumber, row, col);
		if( monsterNumber > 0 ){
			int monsterIndex = monsterNumber - 1;
			Monster& monster = Monsters[monsterIndex];
			int elem  = ET_1_FIRE;
			int bolt  = MI_1_FIREBOLT;
			int spell = PS_1_FIREBOLT;
			if( Players[missile.CasterIndex].fullClassId == PFC_ELEMENTALIST ){
				elem  = ET_6_COLD;
				bolt  = MI_129_SNOW_BOLT;
				spell = PS_56_ICE_BOLT;
			}else if( HasTrait(missile.CasterIndex, TraitId::Hydramancer) ){
				auto& hydra = Hydras[RNG(countof(Hydras))];
				elem  = hydra.elem;
				bolt  = hydra.missile;
				spell = hydra.spell;
				//Firing mana cost is turned off in CastFireBolt(), CastArcaneStar(), CastAcidMissile().  For Charged Bolt, it's automatically turned off.
				if( PlayerSpellLevel(missile.CasterIndex, spell, SO_1_SPELL) <= 0 ) return false;
			}
			if( ! IsMonsterImmuneToMissile(monsterIndex, elem, missile.CasterIndex) && monsterIndex >= SummonMonsters_Count ){
				int damage = GetDamageOfPlayerSpell(missile.CasterIndex, missile.spellIndex, missile.SpellLevel, elem); // use hydra level for damage calc
				int dir = OrientationToTarget(missile.Row, missile.Col, row, col);
				missile.value[2] = MissileIndexList[0]; // CurHydraFireboltIndex
				int boltLevel = PlayerSpellLevel(missile.CasterIndex, spell, SO_1_SPELL); // use bolt spell level for correct missile speed (instead of hydra level)
				CastMissile(missile.Row, missile.Col, row, col, dir, bolt, CT_0_PLAYER, missile.CasterIndex, damage, boltLevel, missile.spellIndex );
				if (elem == ET_3_ARCAN) {
					PlayLocalSound(/*Spells[spell].SoundType*/ S_1843, missile.Row, missile.Col);
				}
				SetMissileDirection(missileIndex, 2); // TODO: check wtf is this
				missile.value[1] = 3;
				return true;
			}
		}
	}
	return false;
}

//----- (004359A0) --------------------------------------------------------
void __fastcall ActSpawnMeatBall(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){
		if( missile.TimeToLive > 0 ) goto motion;
		return;
	}
	
	int casterDirection = missile.value[0];
	
	missile.TimeToLive--;
	MissileAttack(missileIndex, 0, 0, 0, missile.Row, missile.Col, 0);
	if( missile.TimeToLive > 0 ){
		missile.Distance++;
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		PutMissile(missileIndex);
		return;
	}

	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 2; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = missile.Row + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = missile.Col + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			int cell = row * 112 + col;
			
			if( To112(row, col) 
			 && !TileBlockWalking[FineMap[ 0 ][ cell ]] 
			 && !MonsterMap[ 0 ][ cell ] 
			 && !PlayerMap[ 0 ][ cell ]
			 && !ObjectsMap[ 0 ][ cell ] ){
				// 00435A76
				int monsterIndex = AddMonster(row, col, casterDirection, 1, 1);
				FixMonsterPosition(monsterIndex, casterDirection);
				goto BREAK_LABEL;
			}


		}
	}
	BREAK_LABEL:;
	PutMissile(missileIndex);
}

//----- (00435AD8) --------------------------------------------------------
void __fastcall ActRune(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int row = missile.Row;
	int col = missile.Col;
	int monsterIndex = abs (MonsterMap[ row ][ col ]) - 1;// TODO: уязвимость из за убранного условия наступления монстра на руну. Может читать данные -1 монстра. хорошо что только читать
	missile.IsDeleted = true;
	int orientation = OrientationToTarget(row, col, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col);
	LightOff(missile.LightIndex);
	
	int runeActivationEffect = missile.value[0];
	CastMissile(row, col, row, col, orientation, runeActivationEffect, CT_2_WALL, missile.CasterIndex, missile.Damage, missile.SpellLevel, missile.spellIndex );
	PutMissile(missileIndex);
}

//----- (00435BB3) --------------------------------------------------------
void __fastcall ActGolem(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
 
	int golemIndex = missile.value[5];
	const Monster& golem = Monsters[golemIndex];
	bool isGolemOnMap = golem.Row != 1 || golem.Col;
	if( isGolemOnMap ){
	    if( golem.CurAction == A_6_DEATH ){
	       return; // wait until golem is death
        }
		missile.IsDeleted = true;
		return;
	}

	int targetRow = missile.value[3];
	int targetCol = missile.value[4];
	int casterRow = missile.value[0];
	int casterCol = missile.value[1];

	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			int cell = row * 112 + col;
			
			if( To112(row, col)
				&& CheckCastInSight(casterRow, casterCol, row, col) 
				&& ! MonsterMap[ 0 ][ cell ] 
				&& ! TileBlockWalking[ FineMap[ 0 ][ cell ] ] 
				&& ! ObjectsMap[ 0 ][ cell ] ){
					// 00435CA8
					MakeGolem(golemIndex, row, col, missileIndex);
					missile.IsDeleted = true;
					return;
			}
		}
	}

	missile.IsDeleted = true;
}

//----- (00435CD9) --------------------------------------------------------
void __fastcall ActManaShield(int missileIndex)
{
	ManaShieldActed = true;
}

//----- (00435CE4) --------------------------------------------------------
void __fastcall ActMagicArrow( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	static struct { int effect, sprite; } explosions[] = { {0, 0},
		{ EA_ARROW_FIRE,        MIS_5_MAGBLOS         },
		{ EA_ARROW_LIGHTNING,	MIS_26_MINILTNG       },
		{ EA_ARROW_ARCAN,		MIS_62_ARCARROW_EXPLO },
		{ EA_ARROW_ACID,		MIS_64_ACIDARROWEXPLO },
		{ EA_ARROW_HOLY,		MIS_60_HARROW_EXPLO   },
		{ EA_ARROW_COLD,		MIS_73_COLDEXPLO      },
	};
	int explosionElement = ET_1_FIRE;
	int curMissileElement = ET_1_FIRE;
	switch( missile.BaseMissileIndex ){
	case MI_27_FIRE_ARROW:      curMissileElement = ET_1_FIRE     ; break;
	case MI_56_LIGHTING_ARROW:  curMissileElement = ET_2_LIGHTNING; break;
	case MI_108_ARCANE_ARROW:   curMissileElement = ET_3_ARCAN    ; break;
	case MI_109_ACID_ARROW:     curMissileElement = ET_4_ACID     ; break;
	case MI_77_HOLY_BOLT_ARROW: curMissileElement = ET_5_HOLY     ; break;
	case MI_126_COLD_ARROW:     curMissileElement = ET_6_COLD     ; break;
	default: curMissileElement = missile.value[5]; break;
	}
	bool isExplosion = missile.value[6];
	if( isExplosion || missile.value[7] > 0 ){
		explosionElement = missile.value[7];
	}else{
		explosionElement = curMissileElement;
		isExplosion = missile.spriteIndex == explosions[explosionElement].sprite;
		LimitToRange(explosionElement, ET_1_FIRE, ET_6_COLD);
		if( ! isExplosion ) explosionElement = 1 << (explosionElement - 1);
	}
	if( !IsMainDraw ){ if( isExplosion ) return; else goto motion; }
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	Player& player = Players[ casterIndex ];
	Monster& monster = Monsters[ casterIndex ];
	BaseMissile& baseMissile = BaseMissiles[missile.BaseMissileIndex];
	int minDamage = 0, maxDamage = 0;
	if( isExplosion ){
		auto& explosion = explosions[explosionElement];
		ChangeLight( missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex + 5 ); // magic explosion goes as second attack
		if( casterIndex == M1_TRAP ){ // trap
			minDamage = RNG(Dungeon->level * by(Difficulty, 2, 4, 8, 10, 12, 14, 16, 18, 20)) 
						  + Dungeon->level * by(Difficulty, 4, 6, 8, 10, 12, 14, 16, 18, 20) 
						  + (Difficulty * 50 + 1);
			maxDamage = RNG(Dungeon->level * by(Difficulty, 4, 8, 16, 20, 24, 28, 32, 36, 40)) 
						  + Dungeon->level * by(Difficulty, 4, 8, 10, 12, 14, 16, 18, 20, 22) 
						  + (Difficulty * 100 + 25);

			if( GameMode != GM_NORMAL && GameMode != GM_HARD ){
				minDamage /= 4;
				maxDamage /= 4;
			}
			else {
				minDamage /= 2;
				maxDamage /= 2;
			}
		}else if( missile.CasterType != CT_0_PLAYER ){ // monster elemental arrows
			int elemental_arrow_damage_divider = 4 + Difficulty / 3;
			if (is(GameMode, GM_IRONMAN, GM_EASY, /*GM_CLASSIC,*/ GM_SPEEDRUN, GM_NIGHTMARE, GM_HARDCORE )) {
				elemental_arrow_damage_divider += 1;
			}
			minDamage = monster.MinDamage / elemental_arrow_damage_divider;
			maxDamage = monster.MaxDamage / elemental_arrow_damage_divider;
		}else{ // player
			minDamage = player.effectActionValue[ explosion.effect ].minVal;
			maxDamage = player.effectActionValue[ explosion.effect ].maxVal;
			minDamage += minDamage * player.effectActionPercent[ explosion.effect ].minVal / 100;
			maxDamage += maxDamage * player.effectActionPercent[ explosion.effect ].minVal / 100; // only minVal due to no range for percent
		}
		ELEMENTAL_TYPE oldDamageType = baseMissile.DamageType;
		baseMissile.DamageType = (ELEMENTAL_TYPE) explosionElement; // magic explosion damage part of magic arrow damage
		MissileAttack( missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1 ); // explosion can pierce (do damage up to anim frames times)
		baseMissile.DamageType = oldDamageType;
	}else{ // flying arrow
		++missile.Distance;
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		if( casterIndex == M1_TRAP ){
			minDamage = by(Difficulty,  4, 4, 5, 5, 5, 5, 5, 5, 5 ) * Dungeon->level + by(Difficulty,  15, 155, 345, 445, 545, 645, 745, 845, 945 );
			maxDamage = minDamage + by( Difficulty, 2, 3, 3, 3, 3, 3, 4, 4, 4 ) * Dungeon->level;
		}else if( missile.CasterType != CT_0_PLAYER ){ // monster
			minDamage = monster.MinDamage;
			maxDamage = monster.MaxDamage;
		}else{ // player
			minDamage = player.MinDamageFromItem;
			maxDamage = player.MaxDamageFromItem;
		}
		if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
			ELEMENTAL_TYPE oldDamageType = baseMissile.DamageType;
			baseMissile.DamageType = ET_0_PHYSICAL; // physical arrow damage part of magic arrow damage
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 0); // arrow can't pierce (do damage only once)
			baseMissile.DamageType = oldDamageType;
		}
		if( missile.TimeToLive > 0 ){ // fly
			int& lastRow = missile.value[0];
			int& lastCol = missile.value[1];
			if( missile.Row != lastRow || missile.Col != lastCol ){
				lastRow = missile.Row;
				lastCol = missile.Col;
				ChangeLight(missile.LightIndex, missile.Row, missile.Col, 5);
			}
			if( explosionElement & explosionElement - 1 && missile.FrameIndex + missile.FrameStep > missile.framesCount ){
				static int magicArrows[] = { MI_27_FIRE_ARROW, MI_56_LIGHTING_ARROW, MI_108_ARCANE_ARROW, MI_109_ACID_ARROW, MI_77_HOLY_BOLT_ARROW, MI_126_COLD_ARROW };
				for( int i = 0; i < countof(magicArrows); ++i ){
					if( ++curMissileElement > ET_6_COLD ) curMissileElement = ET_1_FIRE;
					if( explosionElement & (1 << (curMissileElement - 1)) ){
						if( missile.BaseMissileIndex < MI_139_THROWING_KNIFE ){
							missile.BaseMissileIndex = magicArrows[curMissileElement - 1];
							SetMissileSprite(missileIndex, BaseMissiles[missile.BaseMissileIndex].Image );
						}else{
							missile.value[5] = curMissileElement;
						}
						break;
					}
				}
			}
		}else{ // start explosion
			Missile sourceMis = missile;
			for( int e = curMissileElement, i = 0, count = ET_1_FIRE; count < ET_COUNT; ++count ){
				if( explosionElement & (1 << (e - 1)) ){
					Missile* expMis = &missile;
					int expMisIndex = missileIndex;
					if( i > 0 ){
						expMisIndex = CastMissile( 0, 0, 0, 0, 0, MI_85_SEARCH, 0, 0, 0, 0, 0 );
						if( expMisIndex == -1 ) break;
						expMis = &Missiles[expMisIndex];
						*expMis = sourceMis;
						expMis->LightIndex = AddLight(expMis->Row, expMis->Col, 5);
					}
					expMis->direction = 0;
					expMis->Distance = 0;
					if( ! expMis->isHitTarget ){
						expMis->WholeDeltaY -= expMis->DeltaY;
						expMis->WholeDeltaX -= expMis->DeltaX;
						MoveMissile(expMisIndex);
					}
					SetMissileSprite(expMisIndex, explosions[e].sprite );
					int sound = S_M1_NO_SOUND;
					switch( e ){
					case ET_1_FIRE:      sound = RandSound( 1642, 1644 ); break;
					case ET_2_LIGHTNING: sound = RandSound( 1638, 1641 ); break;
					case ET_3_ARCAN:     sound = RandSound( 1646, 1648 ); break;
					case ET_4_ACID:      sound = S_1632_Devast_tox_aura;  break;
					case ET_5_HOLY:      sound = /*S_1633_Devast_holy_aura*/ S_1650; break;
					case ET_6_COLD:      sound = RandSound( 1635, 1637 ); break;
					}
					if( sound != S_M1_NO_SOUND ) PlayLocalSound( sound, expMis->Row, expMis->Col );
					expMis->TimeToLive = expMis->framesCount - 1;
					expMis->value[6] = true; // isExplosion
					expMis->value[7] = e;
					++i;
				}
				if( ++e > ET_6_COLD ) e = ET_1_FIRE;
			}
		}
	}
	if( missile.TimeToLive <= 0 ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}
//----- (0043600F) --------------------------------------------------------
void __fastcall ActArrow( int missileIndex )
{
	Missile& missile = Missiles[ missileIndex ];
	if( missile.value[7] ) return ActMagicArrow(missileIndex);
	bool isInternalHp = false;

	if( ! IsMainDraw ) goto motion;
	missile.TimeToLive--;
	missile.Distance++;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( ! IsMainDraw ) return;
	int minDamage, maxDamage;
	if( missile.CasterIndex == M1_TRAP ){ // trap ? TODO: change to CT_M1_MAP_OBJECT ?
		// 0043608F
		int maxDmgModifier = 200;
		if (GameMode == GM_HARD) {
			maxDmgModifier = 250; // for Normal and Hard, max damage is 3x instead of 2x
		}
		else if (GameMode == GM_NORMAL) {
			maxDmgModifier = 225; // for Normal and Hard, max damage is 3x instead of 2x
		}
		int hardModeBonus = (GameMode == GM_HARD) ? 5 : 0;
		minDamage = Dungeon->level * 5 + Difficulty * 125 + 5 + hardModeBonus;
		maxDamage = maxDmgModifier * minDamage / 100;
	}else if( missile.CasterType != CT_0_PLAYER ){
		// 00436079
		Monster& monster = Monsters[missile.CasterIndex];
		if( missile.flag & MSF_REFLECTED ){ // rebound reflectedArrowDamage
			minDamage = maxDamage = missile.Damage;
			isInternalHp = true;
		}else{
			minDamage = monster.MinDamage;
			maxDamage = monster.MaxDamage;
		}
	}else{ // missile.CasterType == CT_0_PLAYER // TODO: change to switch everywhere, for CASTER_TYPE expandability
		// 0043605D
		Player& player = Players[missile.CasterIndex];
		minDamage = player.MinDamageFromItem;
		maxDamage = player.MaxDamageFromItem;
	}
	//00436099
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		MissileAttack(missileIndex, minDamage, maxDamage, isInternalHp, missile.Row, missile.Col, 0);
	}
	//004360C9
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	PutMissile(missileIndex);
}

//----- (004360E8) --------------------------------------------------------
void __fastcall ActFireArcanBolt(int missileIndex) // actarcanestar
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	if( missile.BaseMissileIndex == MI_63_BONE_SPIRIT && missile.direction == 8 ){// лишний код. вон спирит больше не по этой функции проходит
		if( !missile.TimeToLive ){
			if( missile.LightIndex >= 0 ){
				LightOff(missile.LightIndex);
			}
			missile.IsDeleted = true;
			PlayLocalSound(S_100_BSIMPCT, missile.Row, missile.Col);
		}
		PutMissile(missileIndex);
		return;
	}
	// 00436158
	int oldWholeDeltaX = missile.WholeDeltaX;
	int oldWholeDeltaY = missile.WholeDeltaY;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int damage;
	if( missile.CasterIndex == -1 ){ // ловушка // 00436241 // хак 0045A2CA сразу подставлен
		// damage = ( DungeonLevel + by( Difficulty, 0, 40, 200 ) ) * 8 + RNG(ModifyFireBoltDamage() + 1 ); // old damage
		if (Difficulty < 1) {
			damage = 40 + Dungeon->level * 15 + RNG(Dungeon->level * 5);
		}
		else if (Difficulty == 1) {
			damage = 500 + Dungeon->level * 30 + RNG(Dungeon->level * 15);
		}
		else if (Difficulty == 2) {
			damage = 1500 + Dungeon->level * 60 + RNG(Dungeon->level * 30);
		}
		else { // new difficulties (3-8)
			damage = 1000+ Difficulty * 500 + Dungeon->level * 65 + RNG(Dungeon->level * 40);
		}
		if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_EASY/*, GM_CLASSIC*/)) {
			damage -= damage / 3;
		}
	}else if( missile.CasterType != CT_0_PLAYER ){
		if( IsThisBoss( missile.CasterIndex, UM_579_Andariel ) ){ // 00436216
			damage = 250 + Difficulty * 200 ; // 007163B0
		}else{
			Monster& monster = Monsters[missile.CasterIndex]; // 007163DE
			damage = RngFromRange(monster.MinDamage, monster.MaxDamage);
		}
	}else{
		Player& player = Players[missile.CasterIndex]; // 004361A4
		if( missile.spellIndex == PS_13_HYDRA ){
			damage = missile.Damage;
		}else{
			switch( missile.BaseMissileIndex ){
			case MI_63_BONE_SPIRIT: damage = 0; break; // 004361BB
			default: damage = GetDamageOfPlayerSpell(missile.CasterIndex, missile.spellIndex, missile.SpellLevel, BaseMissiles[missile.BaseMissileIndex].DamageType ); break;
			}
		}
	}
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){ // 0043625F
		MissileAttack( missileIndex, damage, damage, 0, missile.Row, missile.Col, 0 );
	}
	if( !missile.TimeToLive ){ //00436296
		missile.IsDeleted = true;
		if( ! missile.isHitTarget ){
			missile.WholeDeltaY = oldWholeDeltaY;
			missile.WholeDeltaX = oldWholeDeltaX;
			MoveMissile( missileIndex );
		}
		int exEffectIndex = -1;
		if( missile.BaseMissileIndex == MI_63_BONE_SPIRIT ){
			SetMissileDirection( missileIndex, 8 ); // 004363CB
			missile.TimeToLive = 7;
			missile.IsDeleted = false;
			PutMissile( missileIndex );
			return;
		}
		if( int afterEffect = BaseMissiles[missile.BaseMissileIndex].AfterEffect ){
			CastMissile(missile.Row, missile.Col, missileIndex, 0, missile.direction, afterEffect, missile.CasterType, missile.CasterIndex, 0, 0, missile.spellIndex );
		}
		// 004363B8
		if( missile.LightIndex >= 0 ){
			LightOff(missile.LightIndex);
		}
		PutMissile(missileIndex);
		return;
	}
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	if( missile.Row != lastRow || missile.Col != lastCol ){
		lastRow = missile.Row;
		lastCol = missile.Col;
		if( missile.LightIndex >= 0 ){
			ChangeLight(missile.LightIndex, missile.Row, missile.Col, 8);
		}
	}
	PutMissile(missileIndex);
}

//----- (00436439) --------------------------------------------------------
void __fastcall ActLightningNovaSegment(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int prevTimeToLive = missile.TimeToLive;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
	if( missile.isHitTarget == 1 ){
		missile.TimeToLive = prevTimeToLive;//
	}
	// 004364B7
	int specialRow = missile.value[0];// если находимся в стартовой точке, а в ней алтарь то двигаемся дальше. Чтобы адекватно работал алтарь с новой
	int specialCol = missile.value[1];
	int objectNum = abs(ObjectsMap[ specialRow ][ specialCol ]);
	if( objectNum && specialRow == missile.Row && specialCol == missile.Col ){
		int baseObjectIndex = Objects[objectNum - 1].BaseObjectIndex;
		if( baseObjectIndex == BO_59_SHRINE || baseObjectIndex == BO_60_SHRINE_2 ){
			// 004364FC
			missile.TimeToLive = prevTimeToLive;
		}
	}
	// 00436505
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	PutMissile(missileIndex);
}

//----- (00436521) --------------------------------------------------------

void __fastcall ActSpell70LightningWallSegment(int castIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[castIndex];
	missile.TimeToLive--;
	MissileAttack(castIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 1);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	PutMissile(castIndex);
}

//----- (00436583) --------------------------------------------------------
void __fastcall ActSpell51_KRULL( int castIndex )
{
	Missile& missile = Missiles[castIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(castIndex);
	if( !IsMainDraw ) return;
	MissileAttack(castIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	PutMissile(castIndex);
}

//----- (004365ED) --------------------------------------------------------
void __fastcall ActSpell59AcidPools( int spellIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[spellIndex];
	missile.TimeToLive--;
	int timeToLive = missile.TimeToLive;
	MissileAttack(spellIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 0, missile.value[5]);
	missile.TimeToLive = timeToLive;
	if( !timeToLive ){
		if( missile.direction == 0 ){
			SetMissileDirection(spellIndex, 1);
			missile.TimeToLive = missile.framesCount;
		}else{
			missile.IsDeleted = true;
		}
	}
	PutMissile(spellIndex);
}

//----- (00436661) --------------------------------------------------------

void __fastcall ActFireWallSegment(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int& StartAnimationEndFrame = missile.value[0];
    int& Lightness = missile.value[1];
	
	int lightLevels[14] = { 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 12, 0 };
	missile.TimeToLive--;
	if( missile.TimeToLive == StartAnimationEndFrame ){// анимацию старта огненной стены програли, ставим анимацию просто стены
		SetMissileDirection(missileIndex, 1);
		missile.FrameIndex = RNG(11) + 1;
	}
	// 004366FA
	if( missile.TimeToLive == missile.framesCount - 1 ){// начать анимацию исчезновения огненной стены
		SetMissileDirection(missileIndex, 0);
		missile.FrameIndex = 13;
		missile.FrameStep = -1;
	}
	// 00436724
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 1);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	// 0043675B
	if( missile.direction && missile.TimeToLive && missile.FrameStep != -1 && Lightness < 12 ){
		if( ! Lightness ){
			missile.LightIndex = AddLight( missile.Row, missile.Col, lightLevels[0]);
		}
		// 0043679F
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, Lightness < countof(lightLevels) ? lightLevels[Lightness] : 0 );
		Lightness++;
	}
	PutMissile(missileIndex);
}

//----- (004367D2) --------------------------------------------------------
void __fastcall ActFireblast(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	int casterIndex = missile.CasterIndex;
	int spellDamage = missile.Damage;
	int casterRow, casterCol;
	if( ! IsMainDraw ){
		if( missile.spriteIndex != MIS_19_BIGEXP ) goto motion;
		return;
	}
	missile.TimeToLive--;
	if( missile.CasterType != CT_0_PLAYER ){
		casterRow = Monsters[casterIndex].Row;
		casterCol = Monsters[casterIndex].Col;
	}else{
		casterRow = Players[casterIndex].Row;
		casterCol = Players[casterIndex].Col;
	}
	// 00436836
	if( missile.spriteIndex == MIS_19_BIGEXP ){
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
		//00436BD8
		PutMissile(missileIndex);
		return;
	}
	// 00436864
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( ! IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		// 004368A0
		MissileAttack(missileIndex, spellDamage, spellDamage, 0, missile.Row, missile.Col, 0);
	}
	// 004368B4
	if( missile.TimeToLive ){
		int& lastRow = missile.value[0];
		int& lastCol = missile.value[1];
		if( row != lastRow || col != lastCol ){
			lastRow = row;
			lastCol = col;
			ChangeLight(missile.LightIndex, row, col, 8);
		}
		//00436BD8
		PutMissile(missileIndex);
		return;
	}
	// 004368C0
	ChangeLight(missile.LightIndex, row, col, missile.FrameIndex );
	for( int i = 0; i < 9; ++i ){
		if( !IsPathBlockedToMoveSpell(casterRow, casterCol, row + RowDeltaFB[i] , col + ColDeltaFB[i] ) ){
			MissileAttack(missileIndex, spellDamage, spellDamage, 0, row + RowDeltaFB[i], col + ColDeltaFB[i], 1);
		}
	}
	// 00436A64
	// очевидно хитрая проверка чтобы взрывы фаерболов впечатавшихся в углы и стены выглядели более верно, не торчащими из стены
	if( !TransList[TransMap[ row ][ col ]] 
		|| missile.DeltaY < 0 && (SomeStrangeFireblastExRoomsCheck (row, col + 1)	|| SomeStrangeFireblastExRoomsCheck (row, col - 1)) ){
			missile.Row++;
			missile.Col++;
			missile.RowDisplacement -= 32;
	}
	if( missile.DeltaX > 0 && ( SomeStrangeFireblastExRoomsCheck (row + 1, col) || SomeStrangeFireblastExRoomsCheck (row - 1, col) ) ){
		missile.RowDisplacement -= 32;
	}
	if( missile.DeltaY > 0 && (SomeStrangeFireblastExRoomsCheck (row, col + 1) || SomeStrangeFireblastExRoomsCheck (row, col - 1)) ){
		missile.ColDisplacement -= 32;
	}
	// 00436B76
	missile.direction = 0;
	SetMissileSprite(missileIndex, MIS_19_BIGEXP);
	missile.TimeToLive = missile.framesCount - 1;
	// 00436BD8
	PutMissile(missileIndex);
}

//----- (00436BE5) --------------------------------------------------------
void __fastcall ActAbyssEnterExplodeFragment( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}

//----- (th2) -------------------------------------------------------------
void __fastcall ActExploder( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	MissileAttack(missileIndex, 3 * missile.Damage / 4, missile.Damage, 0, missile.Row, missile.Col, 0);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}

//----- (00436C23) --------------------------------------------------------
void __fastcall ActSpell88FireBall( int missileIndex )
{
	Missile& missile = Missiles[ missileIndex ];
	if( !IsMainDraw ){
		if( missile.spriteIndex != MIS_19_BIGEXP ) goto motion;
		return;
	}
	int damage = missile.Damage;
	int casterIndex = missile.CasterIndex;
	
	if( missile.value[6] >= 0 ){
		missile.value[6]--;
	}else{
		missile.value[5] *= 2;
		missile.value[6] = missile.value[5];
		missile.direction--;
		Wrap(missile.direction, 8);
	}
	int deltaY, deltaX;
	switch( missile.direction ){
	case 0:
	case 4:
		deltaY = missile.DeltaY;
		deltaX = 0;
		break;
	case 2:
	case 6:
		deltaY = 0;
		deltaX = missile.DeltaX;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
		deltaY = missile.DeltaY;
		deltaX = missile.DeltaX;
		break;
	default:
		deltaY = 0;// в оригинале неициализированный мусор
		deltaX = 0;
		break;
	}
	// 00436CB5
	missile.TimeToLive--;
	int casterRow, casterCol;
	if( missile.CasterType != CT_0_PLAYER ){
		casterRow = Monsters[casterIndex].Row;
		casterCol = Monsters[casterIndex].Col;
	}else{
		casterRow = Players[casterIndex].Row;
		casterCol = Players[casterIndex].Col;
	}

	if( missile.spriteIndex == MIS_19_BIGEXP ){
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
		PutMissile(missileIndex);
		return;
	}
	motion:
	missile.WholeDeltaY += deltaY;
	missile.WholeDeltaX += deltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		MissileAttack(missileIndex, damage, damage, 0, row, col, 0);
	}

	if( missile.TimeToLive ){
		int& lastRow = missile.value[0];
		int& lastCol = missile.value[1];
		if( row != lastRow || col != lastCol ){
			lastRow = row;
			lastCol = col;
			ChangeLight(missile.LightIndex, row, col, 8);
		}
		PutMissile(missileIndex);
		return;
	}

	ChangeLight(missile.LightIndex, row, col, missile.FrameIndex);
	MissileAttack(missileIndex, damage, damage, 0, row, col, 1);
	for( int i = 0; i < 8; i++ ){
		MissileAttack(missileIndex, damage, damage, 0, row + RowDelta[i], col + ColDelta[i], 1);
	}
	// 00436F06
	if( !TransList[TransMap[ row ][ col ]] 
		|| missile.DeltaY < 0 && (SomeStrangeFireblastExRoomsCheck(row, col + 1) || SomeStrangeFireblastExRoomsCheck(row, col - 1) ) ){
			missile.Row++;
			missile.Col++;
			missile.RowDisplacement -= 32;
	}
	if( missile.DeltaX > 0 && ( SomeStrangeFireblastExRoomsCheck(row + 1, col) || SomeStrangeFireblastExRoomsCheck(row - 1, col) ) ){
		missile.RowDisplacement -= 32;
	}
	if( missile.DeltaY > 0 && ( SomeStrangeFireblastExRoomsCheck(row, col + 1) || SomeStrangeFireblastExRoomsCheck(row, col - 1) ) ){
		missile.ColDisplacement -= 32;
	}
	missile.direction = 0;
	SetMissileSprite(missileIndex, MIS_19_BIGEXP);
	missile.TimeToLive = missile.framesCount - 1;
	PutMissile(missileIndex);
}

//----- (004370B0) --------------------------------------------------------
void __fastcall ActLightning(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	int damage = 0;
	if( missile.CasterIndex == -1 ){ // trap
		int liteTrapDmgMultiplier = 140;
		if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
			liteTrapDmgMultiplier = 110;
		}
		else if (GameMode == GM_HARD) {
			liteTrapDmgMultiplier = 190;
		}
		damage = RNG( 80 * (Dungeon + Difficulty * 25) ) + ( liteTrapDmgMultiplier * ( Dungeon + Difficulty * 25 )) ;		
	}else if( missile.CasterType != CT_0_PLAYER ){
		Monster& monster = Monsters[missile.CasterIndex]; // 0043710F
		int rngArmor = RNG(monster.ArmorClass);
		damage = 4 + (rngArmor + 1) * 8 + rngArmor / 2 + (Difficulty * 64 * 6); // 42 / 8 = 5 avg
		//damage = monster.ArmorClass * 5 + RNG(monster.ArmorClass * 4) + Difficulty * 100;
		//damage = (Dungeon->level + Difficulty * 24 ) * 64;
	}else{
		Player& player = Players[missile.CasterIndex]; // 004370DD
		damage = ((player.CharLevel / 3) + RNG(player.CharLevel / 2 + 1) + 2 + (player.CurMagic / 10)) * 64; //  ( (CLVL / 3) + Rnd[ (CLVL / 2) + 1 ] + 2 + (curMAG / 10 ) )
	}
	// 00437155
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	if( !missile.TimeToLive || ! To112(row, col) ){ // 00437290
		missile.IsDeleted = true;
		return;
	}
	if( TileBlockMissle[ FineMap[ row ][ col ] ] ){ // 00437174
		if( missile.CasterIndex != -1 || missile.CasterRow != row || missile.CasterCol != col ){// не первый тайл ловушки
			missile.TimeToLive = 0;
			missile.IsDeleted = true;
		}
		return;
	}
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	if( row == lastRow && col == lastCol){ // 004371C4 // Если не передвинулись на следующий тайл
		return;
	}
	if( missile.CasterIndex != -1 ){
		if( missile.CasterType == CT_1_MONSTER_AND_TRAP ){ // 004371FA // monster type 2 (normal ?)
			int baseMonsterIndex = Monsters[missile.CasterIndex].SpritePtr->baseMonsterIndex;
			if( is(baseMonsterIndex, BM_76_DEATH_CLAW, BM_79_FEAR_HUNTER, BM_77_STORM_DEMON, BM_78_EVISCERATOR, BM_160_CHASM_DEMON, BM_164_LIGHTNING_DEMON )){
				CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_23_MONSTER_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
			}else{
				CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
			}
		}else{ // player
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
		}
	}else{
		int casterIndex = missile.CasterIndex;
		CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, casterIndex, damage, missile.SpellLevel, missile.spellIndex ); // cast lightning segment, caster is player
	}
	lastRow = row;
	lastCol = col;
}

//----- (004372BB) --------------------------------------------------------
void __fastcall ActLightningAsArrow(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	int damage = missile.Damage;
	// 004372FF
	if( TileBlockMissle[FineMap[ row ][ col ]] ){
		if( missile.CasterIndex != -1 
			|| missile.CasterRow != row 
			|| missile.CasterCol != col ){// не первый тайл ловушки
				missile.TimeToLive = 0;
		}
		// 00437414
		if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){
			missile.IsDeleted = true;
		}
		return;
	}

	// 0043733C
	
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
	if( row == lastRow && col == lastCol ){// Если не передвинулись на следующий тайл
		// 00437414
		if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){
			missile.IsDeleted = true;
		}
		return;
	}

	// 00437414
	if( row <= 0 || col <= 0 || row >= 112 || col > 112 ){// вышли за пределы карты
		missile.IsDeleted = true;
		return;
	}

	// 00437372
	if( missile.CasterIndex != -1 && missile.CasterType == CT_1_MONSTER_AND_TRAP ){ // monster type 2 (normal ?)
		int baseMonsterIndex = Monsters[missile.CasterIndex].SpritePtr->baseMonsterIndex;
		if( baseMonsterIndex >= 76 && baseMonsterIndex <= 79 ){
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_23_MONSTER_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
		}else{
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
		}
	}else{
		int casterIndex = missile.CasterIndex;
		CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, casterIndex, damage, missile.SpellLevel, missile.spellIndex ); // cast lightning segment, caster is player
	}
	// 004373FC
	lastRow = missile.Row;
	lastCol = missile.Col;
	// 00437414
	if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){
		missile.IsDeleted = true;
	}
}

//----- (0043743E) --------------------------------------------------------
void __fastcall ActLightningSegment(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
	missile.TimeToLive--;
	int prevTTL = missile.TimeToLive;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		// 00437473
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 0);
	}
	// 0043748C
	if( missile.isHitTarget == 1 ){
		missile.TimeToLive = prevTTL;
	}
	// 0043749B
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	// 004374B9
	PutMissile(missileIndex);
}

//----- (004374C4) --------------------------------------------------------
void __fastcall ActTownPortal( int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	
	int& StartAnimationEndFrame = missile.value[0];
    int& Lightness = missile.value[1];
	
	if( missile.TimeToLive > 1 ){
		missile.TimeToLive--;
	}
	// 00437556
	if( missile.TimeToLive == StartAnimationEndFrame ){
		SetMissileDirection(missileIndex, 1);
	}
	// 0043756B
	if( Dungeon != DUN_0_TOWN && missile.direction != 1 && missile.TimeToLive ){
		if( !Lightness ){// Если лайт радиуса ещё нет, добавляем
			missile.LightIndex = AddLight(missile.Row, missile.Col,	1);
		}
		int lightLevel[17] = {
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15
		};
		ChangeLight( missile.LightIndex, missile.Row, missile.Col, Lightness < countof(lightLevel) ? lightLevel[Lightness++] : 0 );// Увеличиваем интенсивность
	}

	// 004375CD
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists 
			&& Dungeon == player.dungeon 
			&& !player.lvlChanging
			&& player.CurAction == PCA_0_STAND
			&& player.Row == missile.Row
			&& player.Col == missile.Col ){ // Do we need a QuestFloorIndex check here?  Added it just in case for potential multiple TP bugs in Horazon's levels, doesn't seem to do much...
				ClearPlayerPath(playerIndex);
				if( playerIndex == CurrentPlayerIndex ){
					// 00437622
					if( MaxCountOfPlayersInGame == 1 // в сингле свободный вход
					 || missile.CasterIndex == playerIndex // в свой тп свободный вход
					 || Dungeon != DUN_0_TOWN ){// в город свободный вход
						// 00713A90
						if( MaxCountOfPlayersInGame != 1 || Dungeon->isQuest || IsSaveAndLeavingEnabled() ){
                            NetSendCmdParam1( 1, CMD_31_WARP, missile.CasterIndex );
							player.CurAction = PCA_10_NEW_LEVEL;
						}else{
							CannotEnterBeforeClearing(3, OM_4_battle_not_over);
						}
						break;
					}
					// 007139AB
					int destDungeonTypeLevel = (Dungeons[Portals[playerIndex].dungeon].level - 1)/ 4;
					int clvlReq = ClvlReqsForEnterInDungeon[ Difficulty ][ destDungeonTypeLevel ];// << arrays need update
					Portal& portal = Portals[missile.CasterIndex];
					
					// Relax the above condition to allow player taking another player's TP in multi-level quest area like Andariel's Throne.
					// Hope this won't cause the out of map disaster mentioned above...
					if( MaxCountOfPlayersInGame != 1 && Dungeons[portal.dungeon].isQuest && !player.dungeonVisited[portal.dungeon] ){
						NetSendCmdLoc(PRIORITY_LOW, CMD_1_WALKXY, player.Row, player.Col + 1);
						break;
					}
					if( player.CharLevel >= clvlReq ){// доросли до входа в тп
						if( MaxCountOfPlayersInGame != 1 || Dungeon->isQuest || IsSaveAndLeavingEnabled() ){
                            NetSendCmdParam1( 1, CMD_31_WARP, missile.CasterIndex );
							player.CurAction = PCA_10_NEW_LEVEL;
						}else{
							CannotEnterBeforeClearing(3, OM_4_battle_not_over);
						}
						break;
					}
					// 007139E0
					// не доросли до входа в тп. Пишем сообщение, играем звук, отходим
					int messageIndex = ClvlNotReqForEnterInDungeonMessagesIndexesTable[ Difficulty ][ destDungeonTypeLevel ];// << arrays need update
					AddOnScreenMessage(messageIndex);
					SOUND soundIndex = S_M1_NO_SOUND;
					switch (player.ClassID ){
						case PC_0_WARRIOR:
						case PC_5_SAVAGE:		soundIndex = S_772_WAR_43;	break;
						case PC_1_ARCHER:
						case PC_4_ROGUE:		soundIndex = S_663_ROG_43;	break;
						case PC_2_MAGE:			soundIndex = S_560_SOR_43;	break;
						case PC_3_MONK:			soundIndex = S_880_MONK_43;	break;
					}
					if( soundIndex != MIS_M1_NONE ){
						PlayGlobalSound(soundIndex);
					}
                    NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, player.Row, player.Col + 1 );
				}
		}
	}

	// 004375D2
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}

	PutMissile(missileIndex);
}

//----- (00437677) --------------------------------------------------------
void __fastcall ActFlashForward(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
	Player& player = Players[missile.CasterIndex];
	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		player.notHittableInTransition = 1;
	}
	// 004376AE
	missile.TimeToLive--;

	// фикс старыми недофункциями ещё до расширения. Его функции вызываются как функции, а используют регистры от этой
	MordorsFlashAttackHack1(missileIndex, missile.Row, missile.Col);
	MordorsFlashAttackHack1(missileIndex, missile.Row, missile.Col + 1);
	MordorsFlashAttackHack1(missileIndex, missile.Row, missile.Col - 1);
	// 0043776A
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
			player.notHittableInTransition = 0;
		}
	}
	// 004377A0
	PutMissile(missileIndex);
}

//----- (004376DC) --------------------------------------------------------
void __fastcall MordorsFlashAttackHack1 (int missileIndex, int row, int col)
{
	MordorsFlashAttackHack2 (missileIndex, row, col);
	MordorsFlashAttackHack2 (missileIndex, row + 1, col);
	MordorsFlashAttackHack2 (missileIndex, row - 1, col);
}

//----- (004376FA) --------------------------------------------------------
void __fastcall MordorsFlashAttackHack2 (int missileIndex, int row, int col)
{
	Missile& missile = Missiles[missileIndex];
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, row, col, 1);
}

//----- (004377AB) --------------------------------------------------------
void __fastcall ActSomeAnotherFlashFr( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		missile.Row = player.Row;
		missile.Col = player.Col;
		missile.WholeDeltaY = player.xOfs << 16;
		missile.WholeDeltaX = player.yOfs << 16;
	}
	// 00437802
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
			player.BlockBonus -= 50; // обратно не восстанавливается, разобраться (хотя этот каст пока не используется)
		}
	}
	PutMissile(missileIndex);
}

//----- (00437845) --------------------------------------------------------
void __fastcall ActSomeAnotherFlashBk( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];

	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		missile.Row = player.NextRow;
		missile.Col = player.NextCol;
	}
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	PutMissile(missileIndex);
}

//----- (004378AB) --------------------------------------------------------
void __fastcall ActFlashBack(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		player.notHittableInTransition = 1;
	}
	// 004378E2
	missile.TimeToLive--;
	// mor: these 3 lines were turned off because they cause increased flash damage in North, North-East and East directions
	// i have tested it, and the results seem systematic. if there is a better solution, you are welcome to try. but consult me first.
	//MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row - 1, missile.Col - 1, 1);
	//MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col - 1, 1);
	//MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row + 1, missile.Col - 1, 1);
	// 00437946
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
			player.notHittableInTransition = 0;
		}
	}
	PutMissile(missileIndex);
}

//----- (00437987) --------------------------------------------------------
void __fastcall ActReflect(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	
	missile.WholeDeltaY = player.xOfs << 16;
	missile.WholeDeltaX = player.yOfs << 16;
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		missile.CasterRow = player.NextRow + 2;
		missile.CasterCol = player.NextCol - 1;
	}else{
		missile.CasterRow = player.Row + 2;
		missile.CasterCol = player.Col - 1;
	}
	// 004379E6
	MoveMissile(missileIndex);
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		if( player.dir == 2 ){
			missile.Row++;
		}else{
			missile.Col++;
		}
	}
	if( !IsMainDraw ) return;

	// 00437A14
	if( missile.CasterIndex != CurrentPlayerIndex && Dungeon != player.dungeon ){
		missile.IsDeleted = true;
	}
	// 00437A31
	if( !player.CountOfReflectCharges ){ 
		missile.IsDeleted = true;
        NetSendCmd( 1, CMD_99_DESTROY_REFLECT );
	}

	PutMissile(missileIndex);
}

//----- (00437A54) --------------------------------------------------------
void __fastcall ManaShieldAction(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	int playerIndex = missile.CasterIndex;
	Player& player = Players[playerIndex];
	if( !IsMainDraw ) goto motion;
	missile.Row = player.Row;
	missile.Col = player.Col;
	motion:
	missile.WholeDeltaY = player.xOfs << 16;
	missile.WholeDeltaX = player.yOfs << 16;
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		missile.CasterRow = player.NextRow;
		missile.CasterCol = player.NextCol;
	}else{
		missile.CasterRow = player.Row;
		missile.CasterCol = player.Col;
	}
	MoveMissile(missileIndex);
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		if( player.dir == 2 ){
			missile.Row++;
		}else{
			missile.Col++;
		}
	}
	if( !IsMainDraw ) return;

	if( playerIndex != CurrentPlayerIndex ){
		if( Dungeon != player.dungeon ){
			missile.IsDeleted = true;
		}
		PutMissile(missileIndex);
		return;
	}
	if( player.CurMana <= 0 || !player.IsExists ){
		missile.TimeToLive = 0;
	}

	int& prevCurLife = missile.value[0];
    int& prevBaseLife = missile.value[1];
	
	// Здесь считается урон под ManaShield
	if( player.CurLife < prevCurLife ){
		int damage = prevCurLife - player.CurLife;
		int modified_damage = damage;
		int lifeBeforeRestore = player.CurLife;

		int slvl = missile.SpellLevel;
		LimitToMax(slvl, 200);
		int clvlModifier = player.CharLevel / 6;
		LimitToMax(clvlModifier, 25);
		int slvlModifier = slvl / 4;
		LimitToMax(clvlModifier, 50);
		int momModifier = PerkValue(PERK_MIND_OVER_MATTER, playerIndex);
		int efModifier = PerkValue(SYNERGY_ENERGY_FIELD, playerIndex);
		if (HasTrait(playerIndex, TraitId::Paladin) || GameMode == GM_CLASSIC) {
			modified_damage -= modified_damage * PerkValue(PERK_FORTIFIED_SHIELD, playerIndex) / 100;
		}
		else{
			modified_damage += modified_damage * (	100 - (	slvlModifier/*50 max*/ + clvlModifier/*25 max*/ + momModifier/*50 max*/	+ efModifier/*10 max*/ ) ) / 100;
		}
		LimitToMin(modified_damage, 0);
		player.CurLife = prevCurLife;
		player.BaseLife = prevBaseLife;
		if( lifeBeforeRestore <= 0 ){
			StartPlayerHit(playerIndex, damage, 0);
		}
		if( player.CurMana > modified_damage ){
			player.CurMana -= modified_damage;
			player.BaseMana -= modified_damage;
		}else{
			// снятие жизни в процентном соотношении по остатку урона по манашиту
			damage = (modified_damage - player.CurMana) * 128 / modified_damage * damage / 128;
			player.CurLife -= damage;
			player.BaseLife -= damage;
			player.CurMana = 0;
			player.BaseMana = player.MaxBaseMana - player.MaxCurMana;
			missile.TimeToLive = 0;
			missile.IsDeleted = 1; //curLife; // = StartPlayerHit() всегда возвращает true
			if( player.CurLife < 0 ){
				SetPlayerHitPoints(playerIndex, 0);
			}
			if( !(player.CurLife & ~63) && playerIndex == CurrentPlayerIndex ){
				TryToDie(playerIndex, missile.value[7]);
			}
		}
	}
    prevCurLife = player.CurLife;
    prevBaseLife = player.BaseLife;
	if( !missile.TimeToLive ){
		missile.IsDeleted = 1; // curLife
        NetSendCmd( 1, CMD_97_ENDSHIELD );
	}
	PutMissile(missileIndex);
}

//----- (00437CC0) --------------------------------------------------------
void __fastcall ActEthereal(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	missile.Row = player.Row;
	missile.Col = player.Col;
	motion:
	missile.WholeDeltaY = player.xOfs << 16;
	missile.WholeDeltaX = player.yOfs << 16;
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		missile.CasterRow = player.NextRow;
		missile.CasterCol = player.NextCol;
	}else{
		missile.CasterRow = player.Row;
		missile.CasterCol = player.Col;
	}
	// 00437D30
	MoveMissile(missileIndex);
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		if( player.dir == 2 ){
			missile.Row++;
		}else{
			missile.Col++;
		}
	}
	if( !IsMainDraw ) return;
	// 00437D5D
	player.activeBuffFlag |= BF_1_ETHEREAL;
	if( !missile.TimeToLive || player.CurLife <= 0 ){
		// 00437D7E
		player.activeBuffFlag &= ~BF_1_ETHEREAL;
		missile.IsDeleted = true;
	}
	// 00437D8E
	PutMissile(missileIndex);
}

//----- (00437D99) --------------------------------------------------------
void __fastcall ActForcewaveSegment(int missileIndex)
{
	Missile& missile = Missiles[ missileIndex ];
	
	int& HellFireFramesFromStart = missile.value[0];
	
	if( !IsMainDraw ) goto motion;
	int lightLevels[14] = { 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 12, 0 };
	missile.Row--;
	missile.Col--;
	missile.RowDisplacement += 32;
	HellFireFramesFromStart++;
	// 00437E44
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) goto motion2;
	int lastTTL = missile.TimeToLive;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);// вообще надо сделать флаг 1 в конце
	if( missile.isHitTarget == 1 ){
		missile.TimeToLive = lastTTL;
	}
	// 00437E9A
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	// 00437F50
	motion2:
	missile.Row++;
	missile.Col++;
	missile.RowDisplacement -= 32;
	if( !IsMainDraw ) return;
	PutMissile(missileIndex);
}

//----- (00437F70) --------------------------------------------------------
void __fastcall ActHydra(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];

	missile.TimeToLive--;
	if( missile.value[1] > 0 ){
		missile.value[1]--;
	}
	if( missile.TimeToLive == missile.value[0]
	 || missile.direction == 2 && !missile.value[1] ){
		SetMissileDirection(missileIndex, 1);
	}

	if( !(missile.TimeToLive % 16) ){
		int prevRowDelta = 0;
		int prevColDelta = 0;

//		for( int radius = 0; radius < 14; ++radius ){ // в оригинале радиус 5, в th1 1.216 радиус 20 (переполнение, на лету исправлено)
//			for( int angleOffset = 0; angleOffset < 23; angleOffset++){
		for (int angleOffset = 0; angleOffset < 23; angleOffset++) {
			for (int radius = 0; radius < 14; ++radius) {
				int rowDelta = NovaCircleDeltas[ angleOffset ][ radius * 2 ];
				int colDelta = NovaCircleDeltas[ angleOffset ][ radius * 2 + 1 ];
				if( !rowDelta && !colDelta ) break; // оба нулевых сдвига - out of radius
				if( prevRowDelta != rowDelta || prevColDelta != colDelta ){ // два раза не проверять одну и ту же клетку на близких расстояниях
					if( AttackCellByHydra(missileIndex, missile.Row + rowDelta, missile.Col + colDelta)
					 || AttackCellByHydra(missileIndex, missile.Row - rowDelta, missile.Col - colDelta)
					 || AttackCellByHydra(missileIndex, missile.Row + rowDelta, missile.Col - colDelta)
					 || AttackCellByHydra(missileIndex, missile.Row - rowDelta, missile.Col + colDelta)){
						goto BREAK_LABEL;
					}
					prevRowDelta = rowDelta;
					prevColDelta = colDelta;
				}
			}
		}
		BREAK_LABEL:;
	}

	if( missile.TimeToLive == 14 ){
		SetMissileDirection(missileIndex, 0);
		missile.FrameIndex = 15;
		missile.FrameStep = -1;
	}

	missile.value[2] += missile.FrameStep;
	int lightLevel = missile.value[2];
	if( lightLevel > 15 ){
		missile.value[2] = 15;
	}else if( lightLevel > 0 ){
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, lightLevel);
	}
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}


//----- (0043816D) --------------------------------------------------------
void __fastcall ActChainLightning(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	//int targetRow = missile.FirstServiseValue;
	//int targetCol = missile.SecondServiseValue;
	//int dir = OrientationToTarget(missile.Row, missile.Col, targetRow, targetCol);
	//CastMissile(missile.Row, missile.Col, targetRow, targetCol, dir, MI_7_LIGHTNING, CT_0_PLAYER, missile.CasterIndex, 1, missile.SpellLevel, missile.spellIndex);

	int extraChains = PerkValue(PERK_CHAIN_LIGHTNING, missile.CasterIndex);
	if (GameMode == GM_CLASSIC) {
		extraChains = missile.SpellLevel - 1;
	}
	int rad = 10; // max radius + 1, 10 == from 0 to 9
	//rad = missile.SpellLevel + 3;
	if( rad > 20 ){ rad = 20; } // can be up to 20
	if( extraChains > 0 ){
		for (int circleOffsetIndex = 0; circleOffsetIndex < rad; circleOffsetIndex++) {
			int circleOffset = CircleOffsets[circleOffsetIndex];
			int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
			for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
				int row = missile.Row + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex * 2];
				int col = missile.Col + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex * 2 + 1];
				if( To112(row, col) ){
					int monsterNum = MonsterMap[row][col];
					if( monsterNum > 0 && CheckCastInSight(missile.Row, missile.Col, row, col ) ){
						int monsterIndex = monsterNum - 1;
						Monster& monster = Monsters[monsterIndex];
						if( ! IsMonsterImmuneToMissile(monsterIndex, BaseMissiles[missile.BaseMissileIndex].DamageType, missile.CasterIndex) && monsterIndex >= SummonMonsters_Count ){
							int dir = OrientationToTarget(missile.Row, missile.Col, row, col);
							CastMissile(missile.Row, missile.Col, row, col, dir, MI_7_LIGHTNING, CT_0_PLAYER, missile.CasterIndex, 1, missile.SpellLevel, missile.spellIndex);
							if (--extraChains <= 0) { goto endOfChains; }
						}
					}

				}
			}
		}
	}
	endOfChains:
	if( --missile.TimeToLive <= 0 ){
		missile.IsDeleted = true;
	}
}

//----- (0043829E) --------------------------------------------------------
void __fastcall ActSpecialEffect(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
	    AddBlood( missile.Row, missile.Col, missile.spriteIndex, missile.value[0] );
		missile.IsDeleted = true;
	}
	// 004382B8
	if( missile.FrameIndex == missile.framesCount ){
		missile.MayBeIsExploding = 1;
	}
	// 004382CC
	PutMissile(missileIndex);
}

//----- (004382D3) --------------------------------------------------------
void __fastcall ActWeaponElementalDamage(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	BaseMissile& bm = BaseMissiles[missile.BaseMissileIndex];
	
	int& LightLevelIndex = missile.value[0];
    int& weaponElementalDamageType = missile.value[1];
	
	missile.TimeToLive--;
	int lightLevels[] = { 9, 10, 11, 12, 11, 10, 8, 6, 4, 2 };

	ELEMENTAL_TYPE defaultDamageType = bm.DamageType;
	Player& player = Players[missile.CasterIndex];// тут нужно добавить проверку на кастер тайп. И только если игрок сюда переводить
	auto& damage = player.effectActionValue[EA_MELEE_FIRE + weaponElementalDamageType - 1];
	bm.DamageType = (ELEMENTAL_TYPE) weaponElementalDamageType;
	MissileAttack(missileIndex, damage.minVal, damage.maxVal, 0, missile.Row, missile.Col, 1);
	bm.DamageType = defaultDamageType;

	if( LightLevelIndex ){
		if( missile.TimeToLive ){
			ChangeLight( missile.LightIndex, missile.Row, missile.Col, LightLevelIndex < countof(lightLevels) ? lightLevels[LightLevelIndex] : 0 );
		}
	}else{
		missile.LightIndex = AddLight(missile.Row, missile.Col, 9);
	}
	// 004383EE
	LightLevelIndex++;
	if( missile.TimeToLive ){
		PutMissile(missileIndex);
	}else{
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
}

//----- For Aura Hit GFX --------------------------------------------------
void __fastcall ActAuraElementalGFX(int missileIndex)
{
	if (!IsMainDraw) return;
	Missile& missile = Missiles[missileIndex];

	missile.TimeToLive--;

	if (missile.TimeToLive) {
		PutMissile(missileIndex);
	}
	else {
		missile.IsDeleted = true;
	}
}

//----- (0043841F) --------------------------------------------------------
void __fastcall ActMagicExplosion(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int& LightLevelIndex = missile.value[0];
	
	int light [15] = { 9, 10, 11, 12, 11, 10, 8, 6, 4, 2, 1, 0, 0, 0, 0	};
	if( missile.TimeToLive-- == 1 ){
		// 00438488
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}else{
		if( LightLevelIndex ){ // 0043849B
			ChangeLight( missile.LightIndex, missile.Row, missile.Col, LightLevelIndex < countof( light ) ? light[ LightLevelIndex ] : 0 ); // 004384BF
		}else{
			missile.LightIndex = AddLight(missile.Row, missile.Col, 9); // 004384A5
		}
		++LightLevelIndex; // 004384DA
		PutMissile(missileIndex);
	}
}

//----- (004384EB) --------------------------------------------------------
void __fastcall ActAcidSplash(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	#ifdef OLD_VERSION
	if( missile.TimeToLive == missile.framesCount ){
		missile.Row++;
		missile.Col++;
		missile.RowDisplacement -= 32;
	}
	#endif
	if( missile.TimeToLive-- == 1){ // 0043851E
		missile.IsDeleted = true; // 00438522
		int damage = RNG((uint)Dungeon->level >> 2 ) + 2* Difficulty + 1;
		if (Monsters[missile.CasterIndex].SpritePtr->baseMonsterIndex == BM_230_BEASTMASTER_SUMMON_2 && MaxCountOfPlayersInGame > 1) {
			// leave no pool
		}
		else {
			CastMissile(missile.Row, missile.Col, missileIndex, 0, missile.direction, MI_59_ACID_POOLS, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex);
		}
	}else{
		PutMissile(missileIndex); // 00438572
	}
}

//----- (0043857C) --------------------------------------------------------
void __fastcall ActTeleport(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		return;
	}
	PlayerMap[ player.Row ][ player.Col ] = 0;
	SomeWithTeleportAndTransparent2(player.Row, player.Col);// установили непрозрачносте всем комнатам
	player.PrevRow = player.Row;
	player.PrevCol = player.Col;
	player.PrevCursorX = player.CursorRow;
	player.PrevCursorY = player.CursorCol;
	player.Row = missile.Row;
	player.Col = missile.Col;
	player.NextRow = player.Row;
	player.NextCol = player.Col;
	player.oldRow = player.Row;
	player.oldCol = player.Col;
	player.TargetRow = player.Row;//Fix back and forth movements seen by other players after Teleport in MP.  These 2 lines here make sure in multi.cpp, MakePlayerPath( playerIndex, pkt->targx, pkt->targy, 1 ); takes the correct arguments.
	player.TargetCol = player.Col;
	SomeWithTeleportAndTransparent(player.Row, player.Col);// установили прозрачность комнате с игроком
	missile.value[0] = 1;
	PlayerMap[ player.Row ][ player.Col ] = (uchar)missile.CasterIndex + 1;
	if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){
		ChangeLightPos(player.LightIndex, player.Row, player.Col);
		ChangeVision(player.lightRadiusIdentificator, player.Row, player.Col);
	}
	if( missile.CasterIndex == CurrentPlayerIndex ){
		PlayerRowPos = player.Row - Scroll.dx;
		PlayerColPos = player.Col - Scroll.dy;
	}
}

//----- (004386CB) --------------------------------------------------------
void __fastcall ActStoneCurse(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];

	int& lastMonsterTargetAction = missile.value[0];
    int& monsterTargetIndex = missile.value[1];

	Monster& monsterTarget = Monsters[monsterTargetIndex];
	missile.TimeToLive--;

	if(monsterTarget.CurrentLife <= 0 && missile.spriteIndex != MIS_18_SHATTER1){
		missile.direction = 0;
		SetMissileSprite(missileIndex, MIS_18_SHATTER1);
		missile.TimeToLive = missile.framesCount - 1;
	}
	// 0043870E
	if (monsterTarget.CurAction != A_15_STONE && monsterTarget.CurAction != A_6_DEATH) {//Set HasSprite to 1 in SpellCasts.cpp, and added death state check here to make sure monsters dying in stone cursed state plays stone shattered animation.  Not sure if this will cause other bugs...
		missile.IsDeleted = true;
		return;
	}
	if (missile.TimeToLive <= 0) {
		missile.IsDeleted = true;
		if(monsterTarget.CurrentLife <= 0){
			// 0043874D
			AddMonsterCorpse(monsterTarget.Row, monsterTarget.Col, monsterTarget.newBossId ? monsterTarget.udeadNum : monsterTarget.SpritePtr->deadSpriteNum, monsterTarget.Orientation); // это байт устанавливается в InitDead
		}else{
			monsterTarget.CurAction = lastMonsterTargetAction;
		}
	}
	// 0043876A
	if(missile.spriteIndex == MIS_18_SHATTER1){
		PutMissile(missileIndex);
	}
}

//----- (0043877E) --------------------------------------------------------
void __fastcall ActApocExplode(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	
	int& isApocFragmentHitTarget = missile.value[0];
	
	missile.TimeToLive--;
	if( !isApocFragmentHitTarget ){
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 1);
	}
	// 004387B8
	//if( missile.isHitTarget == 1 ){ // Unexpect3D: fixed Apoc from monster, hitting player multiple times when plr is standing on a warp tile
		isApocFragmentHitTarget = 1;
	//}
	// 004387C6
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	// 004387D5
	PutMissile(missileIndex);
}

//----- (004387E0) --------------------------------------------------------
void __fastcall ActMonstersRun(int missileIndex)
{
	Missile& missile = Missiles[ missileIndex ];
	Monster& monster = Monsters[ missile.CasterIndex ];
	if( ! IsMainDraw ){
		if( monster.CurAction == A_14_CHARGE) goto motion;
		return;
	}
	if( monster.CurAction != A_14_CHARGE ){
		// 00438952
		missile.IsDeleted = true;
		return;
	}
	// 00438812
	MoveMissile(missileIndex);
	int row = missile.Row;
	int col = missile.Col;
	MonsterMap[ row ][ col ] = 0;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) goto motion2;
	int castRow = missile.Row;
	int castCol = missile.Col;
	if( !CheckCellForMeleeMonsterSafeMove(missile.CasterIndex, castRow, castCol) ){
		// 00438944
		ChargeAttackImpact(missileIndex, row, col);
		missile.IsDeleted = true;
		return;
	}
	// 004388E0
	monster.NextRow = castRow;
	monster.PrevRow = castRow;
	monster.Row = castRow;
	monster.NextCol = castCol;
	monster.PrevCol = castCol;
	monster.Col = castCol;
	MonsterMap[ castRow ][ castCol ] = -1 - missile.CasterIndex;
	if( monster.newBossId ){// (18) расширение номера босса 00438914
		ChangeLightPos(missile.LightIndex, castRow, castCol);
	}
	// 00438932
	motion2:
	MonsterRunSpellMoving(missileIndex);
	if( !IsMainDraw ) return;
	PutMissile(missileIndex);
}

//----- (00438961) --------------------------------------------------------
void __fastcall MonsterRunSpellMoving( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	
	int rowOffset, colOffset;
	switch( missile.direction ){
	case 2:
		rowOffset = 0;
		colOffset = 1;
		break;
	case 3:
	case 4:
	case 5:
		rowOffset = 0;
		colOffset = 0;
		break;
	case 6:
		rowOffset = 1;
		colOffset = 0;
		break;
	case 0:
	case 1:
	case 7:
		rowOffset = 1;
		colOffset = 1;
		break;
	default:
		rowOffset = 0;
		colOffset = 0;
		break;
	}
	if( CheckCellForMeleeMonsterSafeMove(missile.CasterIndex, missile.Row + rowOffset, missile.Col + colOffset) ){
		missile.Row += rowOffset;
		missile.Col += colOffset;
		missile.ColDisplacement += 32 * colOffset - 32 * rowOffset;
		missile.RowDisplacement -= 16 * rowOffset + 16 * colOffset;
	}
	missile.RowDisplacement += missile.value[2];
	missile.ColDisplacement += missile.value[3];
}


//----- (00438A10) --------------------------------------------------------
void __fastcall ActSomeAnotherMonsterRun(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[missile.CasterIndex];
	if( !IsMainDraw ) goto motion;
	MoveMissile(missileIndex);
	int row = missile.Row;
	int col = missile.Col;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int targetIndex = monster.TargetIndex;
	int targetRow, targetCol;
	int castRow = missile.Row;
	int castCol = missile.Col;
	if( !(monster.flag & MF_5_TARGET_MONSTER) ){
		targetRow = Players[targetIndex].Row;
		targetCol = Players[targetIndex].Col;
	}else{
		targetRow = Monsters[targetIndex].Row;
		targetCol = Monsters[targetIndex].Col;
	}

	// 00438AAE
	int isHitTarget;
	if( (castRow != row || castCol != col)
	 && (missile.value[0] & 1 && (abs(row - targetRow) >= 4 || abs(col - targetCol) >= 4) || missile.value[1] > 1)
	 && CheckCellForMeleeMonsterSafeMove(missile.CasterIndex, row, col) ){
		// 00438B05
		ChargeAttackImpact(missileIndex, row, col);
		isHitTarget = targetCol;// Мусор?
		missile.IsDeleted = true;
	}else{
		// 00438B21
		if( monster.flag & MF_5_TARGET_MONSTER ){
			isHitTarget = MonsterMap[ castRow ][ castCol ];
		}else{
			isHitTarget = PlayerMap[ castRow ][ castCol ];
		}
	}

	if( !IsCellNotBlockSight(castRow, castCol) || isHitTarget > 0 && !(missile.value[0] & 1) ){
		// 00438B66
		missile.DeltaY = -missile.DeltaY;
		missile.DeltaX = -missile.DeltaX;
		int orientation = OrientInverted[missile.direction];
		missile.direction = orientation;
		
		missile.curAnimationPtr = monster.SpritePtr->animation[ANIM_1_WALK].oriented[orientation].data;
		missile.value[1]++;
		if( isHitTarget > 0 ){
			missile.value[0] |= 1;
		}
	}
	MonsterRunSpellMoving(missileIndex);
	PutMissile(missileIndex);
}

//----- (00438BCD) --------------------------------------------------------
void __fastcall ActFireWall(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--; // длина стены в каждую сторону от точки каста
	int casterIndex = missile.CasterIndex;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		return;
	}
	
	int& wallFirstBeamRow      = missile.value[0];
	int& wallFirstBeamCol      = missile.value[1];
	int& firstBeamOrientation  = missile.value[2];
	int& secondBeamOrientation = missile.value[3];
	int& wallSecondBeamRow     = missile.value[4];
	int& wallSecondBeamCol     = missile.value[5];
	int& isSecondBeamBlocked   = missile.value[6];
	int& isFirstBeamBlocked    = missile.value[7];
	
	// 00438BFC
	int wallFirstBeamNextRow = wallFirstBeamRow + RowDelta[firstBeamOrientation];// wallFirstBeamRow
	int wallFirstBeamNextCol = wallFirstBeamCol + ColDelta[firstBeamOrientation];// wallFirstBeamCol
	if( TileBlockMissle[FineMap[ wallFirstBeamRow ][ wallFirstBeamCol ]]
	 || isFirstBeamBlocked || ! In112(wallFirstBeamNextRow, wallFirstBeamNextCol) ){
		// 00438C94
		isFirstBeamBlocked = 1;
	}else{
		// 00438C57
		CastMissile(wallFirstBeamRow, wallFirstBeamCol, wallFirstBeamRow, wallFirstBeamCol, Players[casterIndex].dir, MI_5_FIRE_WALL_SEGMENT, 2, casterIndex, 0, missile.SpellLevel, missile.spellIndex );
		if( wallFirstBeamRow != wallFirstBeamNextRow && wallFirstBeamCol != wallFirstBeamNextCol && !TileBlockMissle[FineMap[ wallFirstBeamRow ][ wallFirstBeamNextCol ]] ){ // каст стены по диагонали
			CastMissile(wallFirstBeamRow, wallFirstBeamNextCol, wallFirstBeamRow, wallFirstBeamNextCol, Players[casterIndex].dir, MI_5_FIRE_WALL_SEGMENT, 2, casterIndex, 0, missile.SpellLevel, missile.spellIndex );
		}
		wallFirstBeamRow = wallFirstBeamNextRow;
		wallFirstBeamCol = wallFirstBeamNextCol;
	}

	// 00438C9E
	int wallSecondBeamNextRow = wallSecondBeamRow + RowDelta[secondBeamOrientation];
	int wallSecondBeamNextCol = wallSecondBeamCol + ColDelta[secondBeamOrientation];
	if( TileBlockMissle[FineMap[ wallSecondBeamRow ][ wallSecondBeamCol ]]
	 || isSecondBeamBlocked || ! In112(wallSecondBeamNextRow, wallSecondBeamNextCol) ){
		// 00438D34
		isSecondBeamBlocked = 1;
	}else{
		// 00438CF9
		CastMissile(wallSecondBeamRow, wallSecondBeamCol, wallSecondBeamRow, wallSecondBeamCol, Players[casterIndex].dir, MI_5_FIRE_WALL_SEGMENT, 2, casterIndex, 0, missile.SpellLevel, missile.spellIndex );
		if( wallSecondBeamRow != wallSecondBeamNextRow && wallSecondBeamCol != wallSecondBeamNextCol && !TileBlockMissle[FineMap[ wallSecondBeamNextRow ][ wallSecondBeamCol ]] ){ // каст стены по диагонали
			CastMissile(wallSecondBeamNextRow, wallSecondBeamCol, wallSecondBeamNextRow, wallSecondBeamCol, Players[casterIndex].dir, MI_5_FIRE_WALL_SEGMENT, 2, casterIndex, 0, missile.SpellLevel, missile.spellIndex );
		}
		wallSecondBeamRow = wallSecondBeamNextRow;
		wallSecondBeamCol = wallSecondBeamNextCol;
	}
}

int __fastcall CheckCellForElementalRing( int, int row, int col )
{
	if( To112(row, col) ){
	    return !TileBlockWalking[FineMap[ row ][ col ]];
    }
	return false;
}

void __fastcall ActElementalRing(const int missileIndex, const int damage, const MISSILE missileType )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
    
    const int casterRow = missile.value[0];
    const int casterCol = missile.value[1];
    
	const int radius = 3;
	missile.IsDeleted = true;
	const int circleOffset = CircleOffsets[radius];
	const int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
	for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
		const int row = casterRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
		const int col = casterCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
		if( To112(row, col) ){
		    // Ignore tiles that block walking and not destructable
		    if( const int objectNumber = ObjectsMap[ row ][ col ]; objectNumber ){
		        const Object& object = Objects[ abs(objectNumber) - 1 ];
		        if( object.isBlockWalking && !object.destructable ){
		            continue;
                }
            }
		    
		    // Ignore tiles that are not walkable
		    const int cell = FineMap[ row ][ col ];
		    if( TileBlockMissle[cell] ){
		        continue;
            }
		    
		    // Ignore tiles that are not reachable by walking
		    if( !CheckCastInSight(missile.Row, missile.Col, row, col) ){
		        char steps[28];
                const int stepsAmount = FindPath(CheckCellForElementalRing, 0, missile.Row, missile.Col, row, col, steps);
                if( stepsAmount == 0 || stepsAmount > 6 ){
                    continue;
                }
            }
		    
		    CastMissile(row, col, row, col, 0, missileType, CT_2_WALL, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
        }
	}
}

//----- (00438D43) --------------------------------------------------------
void __fastcall ActFireRing(int missileIndex)
{
    if( !IsMainDraw ) return;
    
    const Missile& missile = Missiles[missileIndex];
	const int casterIndex = missile.CasterIndex;
	int clvl;
	if( missile.CasterType == CT_0_PLAYER ){
		clvl = Players[casterIndex].CharLevel;
	}else{
		clvl = Dungeon->level;
	}
	const int damage = 16 * (RNG(10) + RNG(10) + clvl + 32) / 2;
    ActElementalRing(missileIndex, damage, MI_5_FIRE_WALL_SEGMENT);
}

//----- (00438E93) --------------------------------------------------------
void __fastcall ActLightningRing(int missileIndex)
{
    if( !IsMainDraw ) return;
    
    const Missile& missile = Missiles[missileIndex];
	const int casterIndex = missile.CasterIndex;
	int clvl;
	if( missile.CasterType == CT_0_PLAYER ){
		clvl = Players[casterIndex].CharLevel;
	}else{
		clvl = Dungeon->level;
	}
	const int damage = 16 * (RNG(10) + RNG(10) + clvl + 2) / 2;
    ActElementalRing(missileIndex, damage, MI_70_LIGHTNING_WALL_SEGMENT);
}

//----- (00439025) --------------------------------------------------------
void __fastcall ActLightningWall(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--; // длина стены в каждую сторону от точки каста
	int casterIndex = missile.CasterIndex;
	int clvl = 0;
	if( casterIndex < 0 ) return; // ловушки ? только игроки (ниже используется как playerIndex)
	if( false ){ // отключаем пока, чтобы не дохли от собственных стен
		Player& player = Players[casterIndex];
		clvl = player.CharLevel;
	}

	// 00439060	
	//int damage = 16 * (RNG(10) + RNG(10) + clvl + 2);// old formula
	int damage = 1 + clvl / 2; // new one, intended to reduce dmg vs players
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		return;
	}
	
	int& wallFirstBeamRow = missile.value[0];
	int& wallFirstBeamCol = missile.value[1];
	int& firstBeamOrientation = missile.value[2];
	int& secondBeamOrientation = missile.value[3];
	int& wallSecondBeamRow = missile.value[4];
	int& wallSecondBeamCol = missile.value[5];
	int& isSecondBeamBlocked = missile.value[6];
	int& isFirstBeamBlocked = missile.value[7];

	// 0043909A
	int wallFirstBeamNextRow = wallFirstBeamRow + RowDelta[firstBeamOrientation];// wallFirstBeamRow
	int wallFirstBeamNextCol = wallFirstBeamCol + ColDelta[firstBeamOrientation];// wallFirstBeamCol
	if( TileBlockMissle[FineMap[ wallFirstBeamRow ][ wallFirstBeamCol ]]
		|| isFirstBeamBlocked
		|| wallFirstBeamNextRow <= 0 
		|| wallFirstBeamNextRow >= 112 
		|| wallFirstBeamNextCol <= 0 
		|| wallFirstBeamNextCol >= 112 ){
			// 00439133
			isFirstBeamBlocked = 1;
	}else{
		// 004390F5
		CastMissile(wallFirstBeamRow, wallFirstBeamCol, wallFirstBeamRow, wallFirstBeamCol, Players[casterIndex].dir, MI_70_LIGHTNING_WALL_SEGMENT, 2, casterIndex, damage, missile.SpellLevel, missile.spellIndex );
		if( wallFirstBeamRow != wallFirstBeamNextRow && wallFirstBeamCol != wallFirstBeamNextCol && !TileBlockMissle[FineMap[ wallFirstBeamRow ][ wallFirstBeamNextCol ]] ){ // каст стенки по диагонали
			CastMissile(wallFirstBeamRow, wallFirstBeamNextCol, wallFirstBeamRow, wallFirstBeamNextCol, Players[casterIndex].dir, MI_70_LIGHTNING_WALL_SEGMENT, 2, casterIndex, damage, missile.SpellLevel, missile.spellIndex );
		}
		wallFirstBeamRow = wallFirstBeamNextRow;
		wallFirstBeamCol = wallFirstBeamNextCol;
	}

	// 0043913D
	int wallSecondBeamNextRow = wallSecondBeamRow + RowDelta[secondBeamOrientation];
	int wallSecondBeamNextCol = wallSecondBeamCol + ColDelta[secondBeamOrientation];
	if( TileBlockMissle[FineMap[ wallSecondBeamRow ][ wallSecondBeamCol ]]
		|| isSecondBeamBlocked
		|| wallSecondBeamNextRow <= 0 
		|| wallSecondBeamNextRow >= 112 
		|| wallSecondBeamNextCol <= 0 
		|| wallSecondBeamNextCol >= 112 ){
			// 004391D4
			isSecondBeamBlocked = 1;
	}else{
		// 00439198
		CastMissile(wallSecondBeamRow, wallSecondBeamCol, wallSecondBeamRow, wallSecondBeamCol, Players[casterIndex].dir, MI_70_LIGHTNING_WALL_SEGMENT, 2, casterIndex, damage, missile.SpellLevel, missile.spellIndex );
		if( wallSecondBeamRow != wallSecondBeamNextRow && wallSecondBeamCol != wallSecondBeamNextCol && !TileBlockMissle[FineMap[ wallSecondBeamNextRow ][ wallSecondBeamCol ]] ){ // каст стенки по диагонали
			CastMissile(wallSecondBeamNextRow, wallSecondBeamCol, wallSecondBeamNextRow, wallSecondBeamCol, Players[casterIndex].dir, MI_70_LIGHTNING_WALL_SEGMENT, 2, casterIndex, damage, missile.SpellLevel, missile.spellIndex );
		}
		wallSecondBeamRow = wallSecondBeamNextRow;
		wallSecondBeamCol = wallSecondBeamNextCol;
	}
}

//----- (004391E3) --------------------------------------------------------
void __fastcall ActInfravision(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	missile.TimeToLive--;
	if (!(player.gameChanger & BIT(GC_21_C_NO_EVIL))) {
		player.Infravision = 1;
	}
	if( !missile.TimeToLive ){
		// 0043921B
		missile.IsDeleted = true;
		CalcCharParams(missile.CasterIndex, 1);
		if( missile.CasterIndex == CurrentPlayerIndex ){
			PlayLocalSound(S_117_GOLUMDED, player.Row, player.Col);
		}
	}
}

//----- (0043922C) --------------------------------------------------------
void __fastcall ActApocalypse(int missileIndex)
{
	if (!IsMainDraw) return;
	Missile& missile = Missiles[missileIndex];
	bool acted = false;

	int& apocRadius = missile.value[0];
	int& apocMinCol = missile.value[1];
	int& apocMaxCol = missile.value[2];
	int& apocMinRow = missile.value[3];
	int& apocMaxRow = missile.value[4];
	int& curApocCol = missile.value[5];

	int row;
	int col;
	for (row = apocMinRow; row < apocMaxRow && !acted; ++row) {
		for (col = apocMinCol; col < apocMaxCol && !acted; ++col) {
			//			int cell = 112 * row + col;
			int monsterNumber = MonsterMap[row][col];
			MonsterHorizontalHitFix(monsterNumber, row, col);
			if (monsterNumber < 0 && (Monsters[-monsterNumber - 1].CurAction == A_4_MELEE || Monsters[-monsterNumber - 1].goal == MG_1_ATTACK) && Monsters[-monsterNumber - 1].Row == row && Monsters[-monsterNumber - 1].Col == col) {//Additional targeting fix
				monsterNumber = -monsterNumber;
			}
			Monster& monster = Monsters[monsterNumber - 1];
//			if( monsterNumber > 3 //0043928F TODO: не голем. только вот голем 3го игрока может попасть под раздачу
//				//&& !TileBlockWalking[FineMap[ 0 ][ cell ]]  removing unwalkable tile condition
//				&& CheckCastInSight(missile.Row, missile.Col, row, col) ){
			if (monsterNumber > SummonMonsters_Count && (FlagMap[row][col] & CF_2_VISIBLE_BY_PLAYER)) {//Area of a square is greater than that of a circle with the same radius, could be unintended nerf if apocRadius = 8, but CheckCastInSight() is buggy...
				// 004392BE
				CastMissile(row, col, row, col, Players[missile.CasterIndex].dir, MI_36_APOC_EXPLODE, CT_0_PLAYER, missile.CasterIndex, missile.Damage, 0, missile.spellIndex);
				acted = true;
			}
		}
		if (!acted) {
			apocMinCol = curApocCol;
		}
	}

	// 00439324
	if (acted) {
		apocMinRow = row - 1;
		apocMinCol = col;
	}
	else {
		missile.IsDeleted = true;
	}
}

//----- (00439349) --------------------------------------------------------
void __fastcall ActForcewave(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
	
	int& targetRow = missile.value[0];
	int& targetCol = missile.value[1];

	Player& player = Players[missile.CasterIndex];

	int orientation = OrientationToTarget(missile.Row, missile.Col, targetRow, targetCol);
	int hellfireRowDelta = RowDelta[orientation];
	int hellfireColDelta = ColDelta[orientation];

	int hellFireRow = missile.Row + hellfireRowDelta;
	int hellFireCol = missile.Col + hellfireColDelta;

	//2D array containing tile offsets for the 2nd layer of Force Wave in diagonal directions.     Dir 0: -1, 0, -1, 0;     Dir 2: 0, -1, 0, -1;     Dir 4: 1, 0, 1, 0;     Dir 6: 0, 1, 0, 1;
	static const int SecondLayerOffset[4][4] = { {-1, 0, -1, 0}, {0, -1, 0, -1}, {1, 0, 1, 0}, {0, 1, 0, 1} };

	if( !TileBlockMissle[FineMap[ hellFireRow ][ hellFireCol ]] ){
		// 004393D7
		CastMissile(hellFireRow, hellFireCol, hellFireRow + hellfireRowDelta, hellFireCol + hellfireColDelta, player.dir, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex );

		//Add 2nd layer of Forcewave in diagonal directions
		if (orientation % 2 == 0) {
			CastMissile(hellFireRow + SecondLayerOffset[orientation/2][0], hellFireCol + SecondLayerOffset[orientation/2][1], hellFireRow + hellfireRowDelta + SecondLayerOffset[orientation/2][2], hellFireCol + hellfireColDelta + SecondLayerOffset[orientation/2][3], player.dir, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex, Missile::zeroValue, true);
		}

		int firstBeamOrientation = (orientation - 2) & 7;
		int firstBeamRowDelta = RowDelta[firstBeamOrientation];
		int firstBeamColDelta = ColDelta[firstBeamOrientation];

		int secondBeamOrientation = (orientation + 2) & 7;
		int secondBeamRowDelta = RowDelta[secondBeamOrientation];
		int secondBeamColDelta = ColDelta[secondBeamOrientation];
		
		int firstBeamRow = hellFireRow + firstBeamRowDelta;
		int firstBeamCol = hellFireCol + firstBeamColDelta;
		int secondBeamRow = hellFireRow + secondBeamRowDelta;
		int secondBeamCol = hellFireCol + secondBeamColDelta;

		int hellfireLen = (missile.SpellLevel >> 1) + 2;
		bool isFirstBeamBlocked = 0;
		bool isSecondBeamBlocked = 0;
		for( int i = 0; i < hellfireLen; i++ ){

			// 00439473
			if( TileBlockMissle[FineMap[ firstBeamRow ][ firstBeamCol ]] 
			 || isFirstBeamBlocked || ! In112(firstBeamRow, firstBeamCol) ){
				// 004394F9
				isFirstBeamBlocked = 1;
			}else{
				// 004394AA
				CastMissile(firstBeamRow, firstBeamCol, firstBeamRow + hellfireRowDelta, firstBeamCol + hellfireColDelta, player.dir, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex );

				//Add 2nd layer of Forcewave in diagonal directions
				if( orientation % 2 == 0 ){
					CastMissile(firstBeamRow + SecondLayerOffset[orientation/2][0], firstBeamCol + SecondLayerOffset[orientation/2][1], firstBeamRow + hellfireRowDelta + SecondLayerOffset[orientation/2][2], firstBeamCol + hellfireColDelta + SecondLayerOffset[orientation/2][3], player.dir, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex, Missile::zeroValue, true);//Invisible
				}

				firstBeamRow += firstBeamRowDelta;
				firstBeamCol += firstBeamColDelta;
			}

			// 00439500
			if( TileBlockMissle[FineMap[ secondBeamRow ][ secondBeamCol ]] 
			|| isSecondBeamBlocked || ! In112(secondBeamRow, secondBeamCol) ){
				// 0043957C
				isSecondBeamBlocked = 1;
			}else{
				CastMissile(secondBeamRow, secondBeamCol, secondBeamRow + hellfireRowDelta, secondBeamCol + hellfireColDelta, player.dir, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex );

				//Add 2nd layer of Forcewave in diagonal directions
				if( orientation % 2 == 0 ){
					CastMissile(secondBeamRow + SecondLayerOffset[orientation/2][0], secondBeamCol + SecondLayerOffset[orientation/2][1], secondBeamRow + hellfireRowDelta + SecondLayerOffset[orientation/2][2], secondBeamCol + hellfireColDelta + SecondLayerOffset[orientation/2][3], player.dir, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel, missile.spellIndex, Missile::zeroValue, true);//Invisible
				}

				secondBeamRow += secondBeamRowDelta;
				secondBeamCol += secondBeamColDelta;
			}

		}

	}
	// 0043959B
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
}

//----- (004395B2) --------------------------------------------------------
void __fastcall ActNova(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];

	int casterIndex = missile.CasterIndex;
	int row = missile.Row;
	int col = missile.Col;
	int damage = missile.Damage;

	int casterType, direction;
	if( casterIndex == M1_TRAP ){
		casterType = CT_1_MONSTER_AND_TRAP;
		direction = 0;
	}else{
		casterType = CT_0_PLAYER;
		direction = Players[casterIndex].dir;
	}
	int e = BaseMissiles[missile.BaseMissileIndex].AfterEffect;
	for( int angleOffset = 0, prevRowDelta = 0, prevColDelta = 0; angleOffset < 23; ++angleOffset ){
		int rowDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 ];
		int colDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 + 1 ];
		if( prevRowDelta != rowDelta || prevColDelta != colDelta ){
			struct {int r,c;} q[] = { row + rowDelta, col + colDelta, row - rowDelta, col - colDelta, row - rowDelta, col + colDelta, row + rowDelta, col - colDelta }; // quarters from down
			for( auto& rc: q ){ CastMissile(row, col, rc.r, rc.c, direction, e, casterType, casterIndex, damage, missile.SpellLevel, missile.spellIndex ); }
			prevRowDelta = rowDelta;
			prevColDelta = colDelta;
		}
	}
	if( --missile.TimeToLive <= 0 ){
		missile.IsDeleted = true;
	}
	CastingNovaInProgress = false;
}

//----- (0043986E) --------------------------------------------------------
void __fastcall ActSpecialArrowSpell(int missileIndex) // TODO: check if unused
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int orientation = 0;
	int casterIndex = missile.CasterIndex;
	int row = missile.Row;
	int col = missile.Col;
	int targetRow = missile.value[0];
	int targetCol = missile.value[1];
	int effect = MI_0_NONE_ARROW;
	int damage = missile.Damage;
	int speed = missile.value[2];
	int casterType;
	if( casterIndex == -1 ){
		// 00439915
		casterType = CT_1_MONSTER_AND_TRAP;
	}else{
		// 004398C5
		Player& player = Players[casterIndex];
		orientation = player.dir;
		casterType = CT_0_PLAYER;
		#if 0
		if( is( player.MinLightningDamage, MI_74_FIREBOLT_ARROW, MI_75_LIGHTNING_ARROW, MI_76_CHARGED_BOLT_ARROW, MI_77_HOLY_BOLT_ARROW) ){
			effect = player.MinLightningDamage; // Item.MinLightningDamage
		} 
		#endif
	}
	// 0043991C
	CastMissile(row, col, targetRow, targetCol, orientation, effect, casterType, casterIndex, damage, speed, missile.spellIndex );
	if( effect == MI_76_CHARGED_BOLT_ARROW ){
		CastMissile(row, col, targetRow, targetCol, orientation, MI_76_CHARGED_BOLT_ARROW, casterType, casterIndex, damage, speed, missile.spellIndex );
		CastMissile(row, col, targetRow, targetCol, orientation, MI_76_CHARGED_BOLT_ARROW, casterType, casterIndex, damage, speed, missile.spellIndex );
	}
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
}

//----- (00439992) --------------------------------------------------------1.145
void __fastcall ActFury (int missileIndex)
{
	if( !IsMainDraw ) return;
	int soundIndexesByClass[6] = {S_802_WAR_72, S_693_ROG_72, S_590_SOR_72, S_590_SOR_72, S_693_ROG_72, S_802_WAR_72};
	Missile& missile = Missiles[missileIndex];
	
	int& playerIndex = missile.value[0];
	Player& player = Players[playerIndex];
	--missile.TimeToLive;
	if( missile.TimeToLive <= 0 ){
		if( player.activeBuffFlag & BF_2_FURY_ACTIVE ){
			if( player.fullClassId == PFC_DRUID && player.CurAction != PCA_8_DEATH ){
				StartStand( playerIndex, player.dir );
				ClearPlrPVars(playerIndex);
				LoadPlayerGFX(playerIndex, PAF_10_TURNING);
				NewPlayerAnimation(playerIndex, player.turningAnimationsPointers[player.dir], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
			}
			player.activeBuffFlag &= ~BF_2_FURY_ACTIVE; // lethargy begins
			if( player.fullClassId == PFC_DRUID ){
				player.LoadPlayerAnimationMask = PAF_0_NO;
			}
			player.activeBuffFlag |= BF_3_FURY_LETARGY;
			if( player.fullClassId == PFC_DRUID && player.CurAction != PCA_8_DEATH ){
				missile.TimeToLive = player.SpellAnimationFrameCount;
			}else{
				missile.TimeToLive = 1; // lethargy timing (only 1 frame), there was calculation from SecondServiceValue
			}
			//CalcCharParams(playerIndex, 1);
			PlayLocalSound(soundIndexesByClass[player.ClassID], player.Row, player.Col);
		}else{
			player.activeBuffFlag &= ~BF_3_FURY_LETARGY; // lethargy ends
			BattleTrance = false;
			InitBerserk = true;
			//CalcCharParams(playerIndex, 1); // mor: nopped to fix the bug with player corpse life regen starting after fury ends while dead
			if( playerIndex == CurrentPlayerIndex ){
				PlayLocalSound(soundIndexesByClass[player.ClassID], player.Row, player.Col); // now it's local
			}
			if( player.fullClassId == PFC_DRUID && player.CurAction != PCA_8_DEATH ){
				InitPlayerGFX(playerIndex);
				if( Dungeon->genType || Dungeon->isQuest ){
					if( player.CurAction == PCA_0_STAND ){
						StartStand( playerIndex, player.dir );
						ClearPlrPVars(playerIndex);
					}
				}else{
					StartWalkStand(playerIndex);
					if( SpellAnimInTown ) StartStand( playerIndex, player.dir ); //Avoid spell animation loop for warrior classes once spell animation is enabled in town
					ClearPlrPVars(playerIndex);
				}
			}
			missile.IsDeleted = true;
		}
	}
}

//----- (00439B65) --------------------------------------------------------
void __fastcall ActIncinerateSegment(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	
	missile.TimeToLive--;
	missile.value[1]--;
	int timeToLive = missile.TimeToLive;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 0);
	if( !missile.TimeToLive && missile.isHitTarget == 1 ){
		missile.TimeToLive = timeToLive;
	}
	// 00439BB9
	if( !missile.value[1] ){
		missile.FrameIndex = 20;
	}
	// 00439BD1
	if( missile.value[1] <= 0 ){
		int level = missile.FrameIndex;
		if( level > 11 ){
			level = 24 - level;
		}
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, level);
	}
	// 00439BFD
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	// 00439C1A
	if( missile.value[1] <= 0 ){
		PutMissile(missileIndex);
	}
}

//----- (00439C2E) --------------------------------------------------------
void __fastcall ActIncinerate(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int& lastRow = missile.value[0];
	int& lastCol = missile.value[1];
    int& CountOfIncinerateFragmentsCasted = missile.value[2];
	if( missile.Row != lastRow || missile.Col != lastCol ){
		// 00439C80
		if( TileBlockMissle[FineMap[ missile.Row ][ missile.Col ]] ){
			missile.TimeToLive = 0;
		}else{
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_48_INCINERATE_SEGMENT, missile.CasterType, missile.CasterIndex, CountOfIncinerateFragmentsCasted, missile.SpellLevel, missile.spellIndex );
		}
		lastRow = missile.Row;
		lastCol = missile.Col;
		++CountOfIncinerateFragmentsCasted;
	}

	// 00439CEC
	int incinerate_range;
	if (GameMode == GM_HARD) {
		incinerate_range = 6;
	}
	else {
		incinerate_range = 3;
	}
	if( !missile.TimeToLive || CountOfIncinerateFragmentsCasted
	 ==	(missile.CasterType != 1 || !IsThisBoss(missile.CasterIndex, UM_582_Izual) ? incinerate_range : 9) ){ // Ограничение в 3 элемента ( 9 for Izual )
		missile.IsDeleted = true;
	}
}

//----- (00439D0C) --------------------------------------------------------
void __fastcall ActChargedBolt(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){
		if( missile.spriteIndex != MIS_3_LGHNING ) goto motion;
		return;
	}
	
	int& LightLevelIndex = missile.value[0];
    int& mainDirection = missile.value[1];
    int& changeDirectionDelay = missile.value[2];
    
	missile.TimeToLive--;
	if( missile.spriteIndex != MIS_3_LGHNING ){
		// 00439D6B
		if( changeDirectionDelay ){
			// 00439DCC
			changeDirectionDelay--;
		}else{
			// 00439D75
			int changeDirectionArray[16] = {-1, 0, 1, -1, 0, 1, -1, -1, 0,	0, 1, 1, 0, 1, -1, 0};
			int curOrientation = mainDirection + changeDirectionArray[missile.chargedBoltChangeDirectionParam];
			Wrap(curOrientation, 8);
			missile.chargedBoltChangeDirectionParam++;
			Wrap(missile.chargedBoltChangeDirectionParam, 16);
			SetMissileMovementDelta(missileIndex, missile.Row, missile.Col, missile.Row + RowDelta[curOrientation], missile.Col + ColDelta[curOrientation], 8);
			changeDirectionDelay = 16;
		}
		// 00439DD3
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
		if( missile.isHitTarget == 1 ){
			// 00439E18
			LightLevelIndex = 8;
			missile.direction = 0;
			missile.ColDisplacement = 0;
			missile.RowDisplacement = 0;
			SetMissileSprite(missileIndex, MIS_3_LGHNING);
			missile.TimeToLive = missile.framesCount;
			MoveMissile(missileIndex);
		}
		// 00439E53
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, LightLevelIndex);
	}

	// 00439E70
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}

//----- (th4) -------------------------------------------------------------
void __fastcall ActHydramancerChargedBolt(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if (!IsMainDraw) {
		if (missile.spriteIndex != MIS_3_LGHNING) goto motion;
		return;
	}

	int& LightLevelIndex = missile.value[0];
//	int& mainDirection = missile.value[1];
//	int& changeDirectionDelay = missile.value[2];// Does not change direction

	missile.TimeToLive--;
	if (missile.spriteIndex != MIS_3_LGHNING) {
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if (!IsMainDraw) return;
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
		if (missile.isHitTarget == 1) {
			// 00439E18
			LightLevelIndex = 8;
			missile.direction = 0;
			missile.ColDisplacement = 0;
			missile.RowDisplacement = 0;
			SetMissileSprite(missileIndex, MIS_3_LGHNING);
			missile.TimeToLive = missile.framesCount;
			MoveMissile(missileIndex);
		}
		// 00439E53
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, LightLevelIndex);
	}

	// 00439E70
	if (!missile.TimeToLive) {
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}

//----- (00439E96) --------------------------------------------------------
void __fastcall ActHolyBolt(int missileIndex)
{

	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	if( ActTrash16SpellEffect( missileIndex * sizeof( Missile ) ) ){
			// 00439F96
			ChangeLight(missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex + 7);
			if( !missile.TimeToLive ){
				missile.IsDeleted = true;
				LightOff(missile.LightIndex);
			}
			PutMissile(missileIndex);
			return;
	}
	// 00439EB5
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		// 00439EF4
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
	}
	// 00439F07
	if( missile.TimeToLive ){
		int& lastRow = missile.value[0];
		int& lastCol = missile.value[1];
		if( missile.Row != lastRow || missile.Col != lastCol ){
			lastRow = missile.Row;
			lastCol = missile.Col;
			ChangeLight(missile.LightIndex, missile.Row, missile.Col, 8);
		}
	}else{
		// 00439F10
		missile.WholeDeltaY -= missile.DeltaY;
		missile.WholeDeltaX -= missile.DeltaX;
		MoveMissile(missileIndex);
		missile.direction = 0;
		SetHolyBoltExSpriteHack(missileIndex, MIS_28_HOLYEXPL, missileIndex * sizeof( Missile ));
		missile.TimeToLive = missile.framesCount - 1;
	}
	PutMissile(missileIndex);
}

//----- (00439FDF) --------------------------------------------------------
void __fastcall ActElemental(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
    
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int& HomingStatus = missile.value[2];
    int& homingBiginRow = missile.value[3];
    int& homingBiginCol = missile.value[4];
	
	if( !IsMainDraw ){
		if( missile.spriteIndex != MIS_19_BIGEXP ) goto motion;
		return;
	}
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	int damage = missile.Damage;
	int spellRow = missile.Row;
	int spellCol = missile.Col;

	if( missile.spriteIndex == MIS_19_BIGEXP ){
		// 0043A015
		Player& player = Players[casterIndex];
		int playerRow = player.Row;
		int playerCol = player.Col;
		ChangeLight(missile.LightIndex, spellRow, spellCol, missile.FrameIndex);
		if( !IsPathBlockedToMoveSpell(playerRow, playerCol, spellRow, spellCol) ){
			MissileAttack(missileIndex, damage, damage, 1, spellRow, spellCol, 1);
		}
		#ifndef REFACTOR
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow, spellCol + 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow, spellCol + 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow, spellCol - 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow, spellCol - 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow + 1, spellCol ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow + 1, spellCol, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow + 1, spellCol - 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow + 1, spellCol - 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow + 1, spellCol + 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow + 1, spellCol + 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow - 1, spellCol ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow - 1, spellCol, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow - 1, spellCol + 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow - 1, spellCol + 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow - 1, spellCol - 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow - 1, spellCol - 1, 1 );
		}
		#else
		for( int i = 0; i < 8; i++ ){
			if( !IsPathBlockedToMoveSpell(playerRow, playerCol, spellRow + RowDelta[i], spellCol + ColDelta[i]) ){
				MissileAttack(missileIndex, damage, damage, 0, spellRow + RowDelta[i], spellCol + ColDelta[i], 1);
			}
		}
		#endif
		// 0043A1BE
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
	}else{
		// 0043A1E1
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		spellRow = missile.Row;
		spellCol = missile.Col;
		MissileAttack(missileIndex, damage, damage, 0, spellRow, spellCol, 0);
		if( !HomingStatus && spellRow == homingBiginRow && spellCol == homingBiginCol ){
			HomingStatus = 1;// дошли до целевой точки, включаем поиск цели
		}

		// 0043A245
		if( HomingStatus == 1 ){// нужна наводка
			HomingStatus = 2;// наводка произведена
			missile.TimeToLive = 255;
			int monsterIndex = FindMonsterTargetInRadius(spellRow, spellCol, 19,
			    [casterIndex]( int monsterIndex )
			    {
			        if( IsMonsterImmuneToMissile( monsterIndex, ET_0_PHYSICAL, casterIndex ) ){
			            return false;
                    }
                    if( monsterIndex < SummonMonsters_Count ){
                        return false;
                    }
                    if( Monsters[monsterIndex].BasePtr->MonsterClass == MON_0_UNDEAD ){
                        return false;
                    }
			        return true;
			    });
			if( monsterIndex <= 0 ){
				// 0043A2B1
				int orientation = Players[casterIndex].dir;
				SetMissileDirection(missileIndex, orientation);
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, spellRow + RowDelta[orientation], spellCol + ColDelta[orientation], 16);
			}else{
				// 0043A276
				Monster& monster = Monsters[monsterIndex];
				int monsterRow = monster.Row;
				int monsterCol = monster.Col;
				SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(spellRow, spellCol, monsterRow, monsterCol));
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, monsterRow, monsterCol, 16);
			}
		}

		// 0043A2F1
		if( spellRow != lastRow || spellCol != lastCol ){
			lastRow = spellRow;
			lastCol = spellCol;
			ChangeLight(missile.LightIndex, spellRow, spellCol, 8);
		}

		// 0043A322
		if( !missile.TimeToLive ){
			missile.direction = 0;
			SetMissileSprite(missileIndex, MIS_19_BIGEXP);
			missile.TimeToLive = missile.framesCount - 1;
		}
	}
	PutMissile(missileIndex);
}

//----- (0043A357) --------------------------------------------------------
void __fastcall ActBoneSpirit(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
    
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int& HomingStatus = missile.value[2];
    int& homingBiginRow = missile.value[3];
    int& homingBiginCol = missile.value[4];
    
	if( !IsMainDraw ){
		if( missile.direction != 8 ) goto motion;
		return;
	}
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	if( missile.direction == 8 ){// анимация взрыва
		// 0043A38E
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex);
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
	}else{// полёт
		// 0043A3D2
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		int spellRow = missile.Row;
		int spellCol = missile.Col;
		int spellDamage = missile.Damage;
		MissileAttack(missileIndex, spellDamage, spellDamage, 0, spellRow, spellCol, 0);
  
		if( !HomingStatus && spellRow == homingBiginRow && spellCol == homingBiginCol ){// достижение точки смены направления
			HomingStatus = 1;
		}
		// 0043A434
		if( HomingStatus == 1 ){// если пора менять направление
			HomingStatus = 2;// направление сменили
			missile.TimeToLive = 255;
			int monsterIndex = FindMonsterTargetInRadius(spellRow, spellCol, 19,
			    [casterIndex]( int monsterIndex )
			    {
			        if( IsMonsterImmuneToMissile( monsterIndex, ET_0_PHYSICAL, casterIndex ) ){
			            return false;
                    }
                    if( monsterIndex < SummonMonsters_Count ){
                        return false;
                    }
                    if( Monsters[monsterIndex].BasePtr->MonsterClass == MON_0_UNDEAD ){
                        return false;
                    }
			        return true;
			    });
			if( monsterIndex <= 0 ){
				// 0043A4BA
				int orientation = Players[casterIndex].dir;
				SetMissileDirection(missileIndex, orientation);
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, spellRow + RowDelta[orientation], spellCol + ColDelta[orientation], 16);
			}else{
				// 0043A463
				Monster& monster = Monsters[monsterIndex];
				int monsterRow = monster.Row;
				int monsterCol = monster.Col;
				missile.Damage = monster.CurrentLife >> 7;
				SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(spellRow, spellCol, monsterRow, monsterCol));
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, monsterRow, monsterCol, 12);
			}
		}
		// 0043A4FF
		if( spellRow != lastRow || spellCol != lastCol ){
			lastRow = spellRow;
			lastCol = spellCol;
			ChangeLight(missile.LightIndex, spellRow, spellCol, 8);
		}
		// 0043A52D
		if( !missile.TimeToLive ){
			SetMissileDirection(missileIndex, 8);
			missile.TimeToLive = 7;
		}
	}
	PutMissile(missileIndex);
}

//----- (0043A558) --------------------------------------------------------
void __fastcall ActSpell62_RisingLight(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( missile.TimeToLive <= 0 ){
		missile.IsDeleted = true;
	}
	PutMissile(missileIndex);
}

//----- (0043A577) --------------------------------------------------------
void __fastcall ActRedPortal(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	
	int& StartAnimationEndFrame = missile.value[0];
    int& Lightness = missile.value[1];
	
	int light[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15};
	if( missile.TimeToLive > 1 ){
		missile.TimeToLive--;
	}
	// 0043A608
	if( missile.TimeToLive == StartAnimationEndFrame ){
		SetMissileDirection(missileIndex, 1);
	}
	// 0043A61D
	if( Dungeon != DUN_0_TOWN && missile.direction != 1 && missile.TimeToLive ){
		if( !Lightness ){
			missile.LightIndex = AddLight(missile.Row, missile.Col,	1);
		}
		ChangeLight(missile.LightIndex, missile.Row, missile.Col, Lightness < countof(light) ? light[Lightness++] : 0);
	}
	if (missile.TimeToLive) {
		LightList[missile.LightIndex].off = false;
	}
	// 0043A67F
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
		PutMissile(missileIndex);
		return;
	}
	PutMissile(missileIndex);
}


//----- (0043A6A5) --------------------------------------------------------
void ProcessMissiles()
{
	BattleTransSlowMissile = !BattleTransSlowMissile;
	int missileArraySize = MissileAmount;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& missile = Missiles[spellIndex];
		if( missile.timeFrame != numeric_limits<decltype(missile.timeFrame)>::max() ){
			++missile.timeFrame;
		}
		int row = missile.Row;
		int col = missile.Col;
		if( ! In112(row, col) ){// очищаем вышедшие за карту спелы
			missile.IsDeleted = 1;
		}
	}
	for( auto& f: flat(FlagMap) ) f &= ~CF_1_SPELL_CAST;
	memset( MissilesMap, 0, sizeof(MissilesMap) ); // очищаем информацию о спелах на карте

	int spellIndexIndex = 0;
	while( spellIndexIndex < missileArraySize ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		if( Missiles[spellIndex].IsDeleted ){
		    // Here we remove bottle. Is bug here>
			RemoveMissile(spellIndex, spellIndexIndex);
			missileArraySize = MissileAmount;
			spellIndexIndex = 0;
		}else{
			spellIndexIndex++;
		}
	}

	MayBeExplodingMagicFlag = false;
	ManaShieldActed = false;

	
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int missileIndex = MissileIndexes[spellIndexIndex];
		Missile& missile = Missiles[missileIndex];
		//if( missileIndex == 123 ) __debugbreak();
		BaseMissiles[missile.BaseMissileIndex].actFunc(missileIndex);
		if( ! (missile.spriteFileFlags & MFF_STATIC) ){// playAnimation
			missile.AnimationDelayIndex++;
			if( missile.AnimationDelayIndex >= missile.animationDelayFrameCount ){ // TODO: change to > and fix the MissileSprites animDelay data
				missile.AnimationDelayIndex = 0;
				if( BattleTrance ){
					if( BattleTransSlowMissile || missile.CasterType == CT_0_PLAYER ) missile.FrameIndex += missile.FrameStep;
				}else{
					missile.FrameIndex += missile.FrameStep;
				}
				if( missile.FrameIndex > missile.framesCount ){
					missile.FrameIndex = 1;
				}
				if( missile.FrameIndex < 1 ){
					missile.FrameIndex = missile.framesCount;
				}
			}
		}
		missileArraySize = MissileAmount;
	}

	if( ManaShieldActed ){
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& missile = Missiles[spellIndex];
			if( missile.BaseMissileIndex == MI_13_MANA_SHIELD ){ //Dragon Mana shield
				ManaShieldAction(spellIndex);
				missileArraySize = MissileAmount;
			}
		}
	}

	spellIndexIndex = 0;
	while( spellIndexIndex < missileArraySize ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		if( Missiles[spellIndex].IsDeleted /*== 1*/ ){ // в th1 проверка именно на int 1 // уже заменил на лету на !0
			RemoveMissile(spellIndex, spellIndexIndex);
			missileArraySize = MissileAmount;
			spellIndexIndex = 0;
		}else{
			spellIndexIndex++;
		}
	}
}

//----- (0043A851) --------------------------------------------------------
void SetupMissilesAnimationsAfterLoading()
{
	for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; missileIndexIndex++ ){
		int missileIndex = MissileIndexes[missileIndexIndex];
		Missile& missile = Missiles[missileIndex];
		// для заклинаний без графики(spriteIndex == -1) получали выход за границу
		MissileSprite& missileSprite = MissileSprites[ missile.spriteIndex < MIS_count ? missile.spriteIndex : 0 ];
		int direction = missile.direction;
		missile.curAnimationPtr = missileSprite.anim[direction];
		if( missile.BaseMissileIndex == MI_20_RUN_ATTACK ){

			Monster& monster = Monsters[missile.CasterIndex];

			MonsterSprite* monsterSprite = monster.SpritePtr;
			int baseMonsterIndex = monsterSprite->baseMonsterIndex;
	
			MonsterAnim* monsterAnimation;
			if( is(baseMonsterIndex, BM_16_HORNED_BEAST, BM_65_HELL_SPAWN, BM_66_HORNED_DEATH, BM_67_BLACK_HORN, BM_171_MUD_LORD, BM_185_BLOOD_HORN, BM_197_CEROMORTH, BM_278_LEAPER)){
				monsterAnimation = &monsterSprite->animation[ANIM_5_SECOND];
			}
			else if (is(baseMonsterIndex, BM_68_GORE_VIPER, BM_89_AZURE_DRAKE, BM_90_CAVE_SNAKE, BM_91_GOLD_SERPENT, BM_118_DRAGON_KIN, BM_161_CHASM_DRAKE)) {
				monsterAnimation = &monsterSprite->animation[ANIM_2_ATTACK];
			}
			else{
				monsterAnimation = &monsterSprite->animation[ANIM_1_WALK];
			}

			missile.curAnimationPtr = monsterAnimation->oriented[direction].data;

		}
	}
}

//----- (0043A8DD) --------------------------------------------------------
void __fastcall ClearMissileCell(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	FlagMap[ missile.Row ][ missile.Col ] &= ~CF_1_SPELL_CAST;
	MissilesMap[ missile.Row ][ missile.Col ] = 0;
}

// =============================== FLASK ==================================

//----- (th3) -------------------------------------------------------------
void __fastcall CastFlaskThrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int& minDamage = missile.value[2];
    int& maxDamage = missile.value[3];
    int& flaskCode = missile.value[4];
	int& overTime = missile.value[5];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}

	int deltaRow = casterRow - targetRow;
    int deltaCol = casterCol - targetCol;
    double distance = sqrt( deltaRow * deltaRow + deltaCol * deltaCol );
    int spellSpeed = 0;
    
	if( casterType == CT_0_PLAYER )
    {
        Player& player = Players[missile.CasterIndex];
    
        Item& leftHand = player.OnBodySlots[IS_LeftHand];
        Item& rightHand = player.OnBodySlots[IS_RightHand];
        Item* flaskHand = nullptr;
    
        if( leftHand.ItemCode == IC_18_FLASK )
        {
            flaskHand = &leftHand;
        }
        else if( rightHand.ItemCode == IC_18_FLASK )
        {
            flaskHand = &rightHand;
        }
        if( !flaskHand )
		{
			missile.TimeToLive = 0;
			missile.IsDeleted = 1;
			return;
		}
        
        const BaseItem& baseItem = BaseItems[flaskHand->baseItemIndex];
    
        // Add +damage% here because if it is a MT_1_MAGIC then it won't be added in PvM_Ranged
        minDamage = flaskHand->MinDamage + flaskHand->MinDamage * player.ItemsAddDamagePercents / 100;
		maxDamage = flaskHand->MaxDamage + flaskHand->MaxDamage * player.ItemsAddDamagePercents / 100;
		overTime = flaskHand->overTime * 20;
		
		ELEMENTAL_TYPE element = BaseMissiles[missile.BaseMissileIndex].DamageType;
		// flask damage modification, by element
		int min_dmg_saver = minDamage;
		int max_dmg_saver = maxDamage;
		
		if (HasTrait(casterIndex, TraitId::Pyromaniac) && element == ET_1_FIRE) {
			minDamage += min_dmg_saver / 4;
			maxDamage += max_dmg_saver / 4;
		}

		if (element == ET_1_FIRE) {
			minDamage += min_dmg_saver * PerkValue(PERK_FIRE_AFFINITY, casterIndex) / 100;
			maxDamage += max_dmg_saver * PerkValue(PERK_FIRE_AFFINITY, casterIndex) / 100;
		}
		else if (element == ET_2_LIGHTNING) {
			minDamage += min_dmg_saver * PerkValue(PERK_LIGHTNING_AFFINITY, casterIndex) / 100;
			maxDamage += max_dmg_saver * PerkValue(PERK_LIGHTNING_AFFINITY, casterIndex) / 100;
		}
		else if (element == ET_3_ARCAN) {
			minDamage += min_dmg_saver * PerkValue(PERK_ARCANE_AFFINITY, casterIndex) / 100;
			maxDamage += max_dmg_saver * PerkValue(PERK_ARCANE_AFFINITY, casterIndex) / 100;
		}
		else if (element == ET_4_ACID) {
			minDamage += min_dmg_saver * PerkValue(PERK_ACID_AFFINITY, casterIndex) / 100;
			maxDamage += max_dmg_saver * PerkValue(PERK_ACID_AFFINITY, casterIndex) / 100;
		}
		
        // Monsters 4x damage resist compensation
		if( is( element, ET_1_FIRE, ET_2_LIGHTNING, ET_3_ARCAN ) ){
			minDamage *= 4;
			maxDamage *= 4;
		}
		flaskCode = baseItem.FlaskOrTrapCode;
	
		const FlaskInfo& flaskInfo = FlasksInfo[flaskCode];
		double MaxFlaskThrowDistance = flaskInfo.MaxFlaskThrowDistance + PerkValue(PERK_HEAVE_HO, casterIndex);
		if( distance < 1.) distance = 1.;
		if( distance > MaxFlaskThrowDistance) distance = MaxFlaskThrowDistance;
		spellSpeed = static_cast<int>( distance * 3.8 );
    }
	
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, spellSpeed);

    missile.FrameIndex = 2;
	missile.TimeToLive = 16 - missile.FrameIndex;
	lastRow = casterRow;
	lastCol = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (th3) -------------------------------------------------------------
void __fastcall ActFlaskThrow( int missileIndex)//, int playerIndex )
{
	//Player& player = Players[playerIndex];
	Missile& missile = Missiles[missileIndex];
    if( ! IsMainDraw ) goto motion;

	
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int minDamage = missile.value[2];
    int maxDamage = missile.value[3];
    int flaskCode = missile.value[4];
	int overTime = missile.value[5];
	
	if( missile.TimeToLive <= 0 ) return; // assert
	--missile.TimeToLive;

	int oldWholeDeltaX = missile.WholeDeltaX;
	int oldWholeDeltaY = missile.WholeDeltaY;
    
    motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
    if( ! IsMainDraw ) return;
 
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol )
	{
	    // Don't attack in the middle of the throw
        if( missile.FrameIndex < 4 || missile.FrameIndex > 12 )
        {
            // 1. Direct bottle damage
            MissileAttack( missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 0, overTime );
        }
        else
		{
			if( TileBlockMissle[FineMap[ missile.Row ][ missile.Col ]] )
			{
				missile.TimeToLive = 0;
			}
		}
	}
	
	if( !missile.TimeToLive )
	{
		missile.IsDeleted = true;
		if( !missile.isHitTarget )
		{
			missile.WholeDeltaY = oldWholeDeltaY;
			missile.WholeDeltaX = oldWholeDeltaX;
			MoveMissile( missileIndex );
		}
  
        int sfxIndex = S_M1_NO_SOUND;
       
		// 1.5 play random bottle break sound
		switch( missile.BaseMissileIndex ){
		case MI_110_FLASK_OF_FIRE: 		    sfxIndex = S_1113_FLASK_FIRE_EX_1;   	    break;
		case MI_111_FLASK_OF_LIGHTNING: 	sfxIndex = S_1116_FLASK_LIGHTNING_EX_1; 	break;
		case MI_112_FLASK_OF_ACID: 	        sfxIndex = S_1114_FLASK_ACID_EX_1;         break;
		case MI_113_FLASK_OF_FIREBOMB: 		sfxIndex = S_1115_FLASK_EXPLOSION_EX_1;   	break;
		case MI_114_FLASK_OF_LIGHTNINGBOMB: sfxIndex = S_1117_FLASK_SHOCK_EX_1;break;
		case MI_115_FLASK_OF_ACIDBOMB: 		sfxIndex = S_1118_FLASK_ACIDBOMB_EX_1;   	break;
		case MI_118_FLASK_OF_ARCANEBOMB: 	sfxIndex = S_1177;   break;
		}
		if( sfxIndex != S_M1_NO_SOUND ){
            PlayLocalSound( sfxIndex, missile.Row, missile.Col );
        }
        
		// 2. Splash damage for adjustment tiles
		missile.Distance = 0; // zero distance to damage with max possible accuracy
		
		const FlaskInfo& flaskInfo = FlasksInfo[flaskCode];
		int splashHitDamagePercentage = flaskInfo.splashHitDamagePercentage;
		int minSplashDamage = minDamage * splashHitDamagePercentage / 100;
		int maxSplashDamage = maxDamage * splashHitDamagePercentage / 100;
		int row = missile.Row;
		int col = missile.Col;
		for( int i = 0; i < 9; ++i ){
			MissileAttack(missileIndex, minSplashDamage, maxSplashDamage, 0, row + RowDelta[i], col + ColDelta[i], 1, overTime);
		}
		
		// 3 Cast additional effect cast.
        int dotEffectIndex = -1;
        switch( missile.BaseMissileIndex )
        {
            case MI_110_FLASK_OF_FIRE:      dotEffectIndex = MI_5_FIRE_WALL_SEGMENT;        break;
            case MI_111_FLASK_OF_LIGHTNING: dotEffectIndex = MI_70_LIGHTNING_WALL_SEGMENT;  break;
            case MI_112_FLASK_OF_ACID:      dotEffectIndex = MI_59_ACID_POOLS;              break;
        }
        
        const BaseMissile& effect = BaseMissiles[missile.BaseMissileIndex];
        int damageType = effect.DamageType;

		for( int i = 0; i < 9; ++i ){
			int effectRow = row + RowDelta[i];
			int effectCol = col + ColDelta[i];
            
            if( ! In112(effectRow, effectCol) ) continue;

			int tileIndex = FineMap[ effectRow ][ effectCol ]; // commented some check below to fix flasks not working on tiles with broken barrels and in doorways
			if( /* ObjectsMap[ effectRow ][ effectCol ] || */ TileBlockMissle[tileIndex] || (TileBlockWalking[tileIndex] & WT_2_FLY) ){ //fix for aoe flask not appearing over lava
                continue;
            }
            
            if( dotEffectIndex != -1 ){
                int damage = RngFromRange(minDamage, maxDamage) * ( flaskInfo.DamageOverTimePercentage + PerkValue(PERK_BURN_IN_HELL, missile.CasterIndex) ) / 100;
                damage <<= 6; // To compensate per frame damage
				PoolOverTime = overTime;
                CastMissile( effectRow, effectCol, effectRow, effectCol, 0, dotEffectIndex, CT_0_PLAYER, missile.CasterIndex, damage, -flaskInfo.DamageOverTimeDuration , missile.spellIndex );
            }else{
                // Explosion effect
                int monsterNumber = MonsterMap[effectRow][effectCol];
                if( monsterNumber ){
                    int monsterIndex = abs(monsterNumber) - 1;
                    const Monster& monster = Monsters[monsterIndex];
                    if( monsterIndex >= SummonMonsters_Count && !IsMonsterImmuneToMissile( monsterIndex, damageType, missile.CasterIndex ) ){
                        KnockbackMonster( monsterIndex, OrientationToTarget( monster.Row, monster.Col, missile.CasterRow, missile.CasterCol ), GameMode == GM_HARD ? 1 : 2 );
                    }
                }
            }
		}
        
        int exEffectIndex = -1;
        switch( missile.BaseMissileIndex ){
            case MI_110_FLASK_OF_FIRE:          break;
            case MI_111_FLASK_OF_LIGHTNING:     break;
            case MI_112_FLASK_OF_ACID:          exEffectIndex = MI_117_FLASK_ACID_EX;   break;
            case MI_113_FLASK_OF_FIREBOMB:      exEffectIndex = MI_116_FLASK_FIRE_EX;   break;
            case MI_114_FLASK_OF_LIGHTNINGBOMB: exEffectIndex = MI_106_BLU_EX;          break;
            case MI_115_FLASK_OF_ACIDBOMB:      exEffectIndex = MI_117_FLASK_ACID_EX;   break;
			case MI_118_FLASK_OF_ARCANEBOMB:    exEffectIndex = MI_104_RED_EX;			break; // ? needs new effect support
        }
        
		if( exEffectIndex != -1 ){
			CastMissile(missile.Row, missile.Col, missileIndex, 0, missile.direction, exEffectIndex, missile.CasterType, missile.CasterIndex, 0, 0, missile.spellIndex );
		}
		
		if( missile.LightIndex >= 0 ){
			LightOff(missile.LightIndex);
		}
		PutMissile(missileIndex);
		return;
	}

    if( missile.LightIndex >= 0 ){
        ChangeLight(missile.LightIndex, missile.Row, missile.Col, 8);
    }
	PutMissile(missileIndex);
}

// =============================== TRAPS ==================================

//----- (th3) -------------------------------------------------------------
void __fastcall CastTrapThrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
  	int& minDamage = missile.value[2];
  	int& maxDamage = missile.value[3];
    int& trapCode = missile.value[4];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}

	int deltaRow = casterRow - targetRow;
    int deltaCol = casterCol - targetCol;
    double distance = sqrt( deltaRow * deltaRow + deltaCol * deltaCol );
    int spellSpeed = 0;
    
	if( casterType == CT_0_PLAYER )
    {
        Player& player = Players[missile.CasterIndex];
    
        Item& leftHand = player.OnBodySlots[IS_LeftHand];
        Item& rightHand = player.OnBodySlots[IS_RightHand];
        Item* trapHand = nullptr;
    
        if( leftHand.ItemCode == IC_19_TRAP )
        {
            trapHand = &leftHand;
        }
        else if( rightHand.ItemCode == IC_19_TRAP )
        {
            trapHand = &rightHand;
        }
        if( !trapHand )
		{
			missile.TimeToLive = 0;
			missile.IsDeleted = 1;
			return;
		}
        
        const BaseItem& baseItem = BaseItems[trapHand->baseItemIndex];
        // Don't add +damage% if it is a MT_0_ARROW then it will be added in PvM_NonMelee
        minDamage = trapHand->MinDamage; // + trapHand->MinDamage * player.ItemsAddDamagePercents / 100;
		maxDamage = trapHand->MaxDamage; // + trapHand->MaxDamage * player.ItemsAddDamagePercents / 100;
        trapCode = baseItem.FlaskOrTrapCode;
	
		const TrapInfo& trapInfo = TrapsInfo[trapCode];
		double MaxTrapThrowDistance = trapInfo.MaxTrapThrowDistance;
		if( distance < 1.) distance = 1.;
		if( distance > MaxTrapThrowDistance) distance = MaxTrapThrowDistance;
		spellSpeed = static_cast<int>( distance * 5.0 );
    }
	
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, spellSpeed);

    missile.FrameIndex = 2;
	missile.TimeToLive = 11 - missile.FrameIndex;
	lastRow = casterRow;
	lastCol = casterCol;
	missile.LightIndex = AddLight(casterRow, casterCol, 8);
}

//----- (th3) -------------------------------------------------------------
void __fastcall ActTrapThrow( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
    if( ! IsMainDraw ) goto motion;
	
    int& lastRow = missile.value[0];
    int& lastCol = missile.value[1];
    int minDamage = missile.value[2];
    int maxDamage = missile.value[3];
    int trapCode = missile.value[4];
	
	if( missile.TimeToLive <= 0 ) return; // assert
	--missile.TimeToLive;

	int oldWholeDeltaX = missile.WholeDeltaX;
	int oldWholeDeltaY = missile.WholeDeltaY;
    
    motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
    if( ! IsMainDraw ) return;
	
	// Don't attack, detect only walls
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol )
    {
		if( TileBlockMissle[FineMap[ missile.Row ][ missile.Col ]] )
		{
			missile.TimeToLive = 0;
		}
    }

		//-----
	
	if( !missile.TimeToLive )
	{
		missile.IsDeleted = true;
		if( !missile.isHitTarget )
		{
			missile.WholeDeltaY = oldWholeDeltaY;
			missile.WholeDeltaX = oldWholeDeltaX;
			MoveMissile( missileIndex );
		}
  
		// 1.5 play random bottle break sound
        int sfxIndex = S_M1_NO_SOUND;
        switch( missile.BaseMissileIndex ){
        case MI_119_TRAP_OF_ARROWS: sfxIndex = RandFrom(S_1181_TRAP_LOCK); break;
        }

		if( sfxIndex != S_M1_NO_SOUND )
		{
			PlayLocalSound( sfxIndex, missile.Row, missile.Col );
		}
        
		int exEffectIndex = -1;
		switch( missile.BaseMissileIndex )
		{
			case MI_119_TRAP_OF_ARROWS:
				CastMissile(minDamage, maxDamage, missile.Row, missile.Col, 0, MI_120_TRAPPER_GUN, missile.CasterType, missile.CasterIndex, 0, trapCode, 0 );
				break;
		}
		
		if( missile.LightIndex >= 0 )
		{
			LightOff(missile.LightIndex);
		}
		PutMissile(missileIndex);
		return;
	}

    if( missile.LightIndex >= 0 ){
        ChangeLight(missile.LightIndex, missile.Row, missile.Col, 8);
    }
	PutMissile(missileIndex);
}


// ========================== TRAP ACTION =================================

//----- (th3) -------------------------------------------------------------
int __fastcall MaxCountOfTrapsForPlayer (int playerIndex)
{
    Player& player = Players[playerIndex];
    int maxCount = player.CharLevel / 8 + 1;// whats that?
    //maxCount += getPerksValueFromParameter(PERK_SUMMONING, playerIndex);
    LimitToMin(maxCount, 1);
    return maxCount;
}

//----- (th3) -------------------------------------------------------------
int __fastcall CountOfPlayerTraps (int playerIndex)
{
    Player& player = Players[playerIndex];
    int countOfTraps = 0;
    for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
        int spellIndex = MissileIndexes[spellIndexIndex];
        Missile& spell = Missiles[spellIndex];
        if( spell.BaseMissileIndex == MI_120_TRAPPER_GUN
            && spell.CasterIndex == playerIndex
            && spell.TimeToLive > 15){
            countOfTraps++;
        }
    }
    return countOfTraps;
}

//----- (th3) -------------------------------------------------------------
void __fastcall ClearOneOldestPlayerTrap (int playerIndex)
{
    Player& player = Players[playerIndex];
    Missile* oldestHydra = nullptr;
    for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
        int missileIndex = MissileIndexes[spellIndexIndex];
        Missile& missile = Missiles[missileIndex];
        if( missile.BaseMissileIndex == MI_120_TRAPPER_GUN
            && missile.CasterIndex == playerIndex
            && missile.TimeToLive > 15
            && ( !oldestHydra || oldestHydra->TimeToLive > missile.TimeToLive ) ){
            oldestHydra = &missile;
        }
    }
    if( oldestHydra ){
        oldestHydra->TimeToLive = 15;
    }
}

//----- (th3) -------------------------------------------------------------
void __fastcall LimitPlayerTrapperTraps (int playerIndex)
{
    Player& player = Players[playerIndex];
    int countOfTraps = CountOfPlayerTraps (playerIndex);
    int maxCountOfTraps = MaxCountOfTrapsForPlayer (playerIndex);
    for( int i = countOfTraps; i > maxCountOfTraps; i-- ){
        ClearOneOldestPlayerTrap (playerIndex);
    }
}

//----- (th3) -------------------------------------------------------------
void __fastcall CastTrapperTrap(int missileIndex, int damageMin, int damageMax, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];

			if( To112(row, col) // было +-15 в th2, сделал как в th1 с 1 до 111
			 && CheckCastInSight(targetRow, targetCol, row, col)
			 && !MonsterMap[ row ][ col ] 
			 && !TileBlockMissle[FineMap[ row ][ col ]] 
			 && !TileBlockWalking[FineMap[ row ][ col ]] 
			 && !ObjectsMap[ row ][ col ] 
			 && !MissilesMap[ row ][ col ] ){
				missile.Row = row;
				missile.Col = col;
				missile.IsDeleted = false;
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:;
	if( missile.IsDeleted ){
		return;
	}
	
	missile.CasterIndex = casterIndex;
	missile.LightIndex = AddLight(missile.Row, missile.Col, 1);
	
	
	const TrapInfo& trapInfo = TrapsInfo[missile.SpellLevel];
	missile.TimeToLive = trapInfo.TimeToLive;
	
    // LimitPlayerTrapperTraps (casterIndex);
}

//----- (th2) -------------------------------------------------------------
int __fastcall AttackCellByTrapperTrap( int missileIndex, int row, int col )
{
    Missile& missile = Missiles[missileIndex];
    int isFindTarget = 0;
    if( CheckCastInSight(missile.Row, missile.Col, row, col) ){
        int monsterNumber = MonsterMap[ row ][ col ];
		if (monsterNumber < 0) {
			int monsterIndex = -monsterNumber - 1;
			Monster& monster = Monsters[monsterIndex];
			if (monster.CurAction == A_3_WALK_HORIZ && monster.Row == row && monster.Col == col) { // might be NextRow and NextCol for next cell
				monsterNumber = -monsterNumber;
			}
		}
		if( monsterNumber > 0 ){
			int monsterIndex = monsterNumber - 1;
			Monster& monster = Monsters[monsterIndex];
			if( !IsMonsterImmuneToMissile(monsterIndex, ET_0_PHYSICAL, missile.CasterIndex) && monsterIndex >= SummonMonsters_Count ){ // не голем
				int orientation = OrientationToTarget(missile.Row, missile.Col, row, col);
				int damage = RngFromRange( missile.CasterRow, missile.CasterCol);
				const TrapInfo& trapInfo = TrapsInfo[missile.SpellLevel];
				CastMissile(missile.Row, missile.Col, row, col, orientation, trapInfo.MissileIndex, CT_0_PLAYER, missile.CasterIndex, damage, missile.SpellLevel, missile.spellIndex );
				isFindTarget = -1;
			}
        }
    }
    return isFindTarget;
}

//----- (th3) -------------------------------------------------------------
void __fastcall ActTrapperTrap(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	const TrapInfo& trapInfo = TrapsInfo[missile.SpellLevel];
	
    --missile.TimeToLive;
	
	if( !(missile.TimeToLive % trapInfo.ShootDelay) ){
		int prevRowDelta = 0;
		int prevColDelta = 0;

		for( int radius = 0, radiuse = trapInfo.TargetDetectRadius; radius < radiuse; ++radius ){ // в оригинале радиус 5, в th1 1.216 радиус 20 (переполнение, на лету исправлено)
			for( auto& NovaCircleDelta : NovaCircleDeltas )
			{
				int rowDelta = NovaCircleDelta[ radius * 2 ];
				int colDelta = NovaCircleDelta[ radius * 2 + 1 ];
				if( !rowDelta && !colDelta ) break; // оба нулевых сдвига - out of radius
				if( prevRowDelta != rowDelta || prevColDelta != colDelta ){ // два раза не проверять одну и ту же клетку на близких расстояниях
					if( AttackCellByTrapperTrap(missileIndex, missile.Row + rowDelta, missile.Col + colDelta) == -1
					 || AttackCellByTrapperTrap(missileIndex, missile.Row - rowDelta, missile.Col - colDelta) == -1
					 || AttackCellByTrapperTrap(missileIndex, missile.Row + rowDelta, missile.Col - colDelta) == -1
					 || AttackCellByTrapperTrap(missileIndex, missile.Row - rowDelta, missile.Col + colDelta) == -1){
						goto BREAK_LABEL;
					}
					prevRowDelta = rowDelta;
					prevColDelta = colDelta;
				}
			}
		}
		BREAK_LABEL:;
	}
	
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	PutMissile(missileIndex);
}

//----- (th3) -------------------------------------------------------------
void __fastcall CastTrapperTrapArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
    Missile& missile = Missiles[missileIndex];
	const TrapInfo& trapInfo = TrapsInfo[missile.SpellLevel];
    if( casterRow == targetRow && casterCol == targetCol ){
        targetRow += RowDelta[casterDirection];
        targetCol += ColDelta[casterDirection];
    }
    int arrowSpeed = trapInfo.ArrowSpeed;

    SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, arrowSpeed);
    missile.FrameIndex = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol) + 1;
    missile.TimeToLive = 256;
}

//----- (th3) -------------------------------------------------------------
void __fastcall ActTrapperTrapArrow( int missileIndex )
{
    Missile& missile = Missiles[ missileIndex ];
    if( ! IsMainDraw ) goto motion;
    missile.TimeToLive--;
    missile.Distance++;
    motion:
    missile.WholeDeltaY += missile.DeltaY;
    missile.WholeDeltaX += missile.DeltaX;
    MoveMissile(missileIndex);
    if( ! IsMainDraw ) return;
    if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
        MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
    }
    if( !missile.TimeToLive ){
        missile.IsDeleted = true;
    }
    PutMissile(missileIndex);
}
