/**************************************
* tuxiang.h
*By DXC
 
***************************************/
#ifndef __TUXIANG_H_
#define __TUXIANG_H_
#include "include.h"

#define Black 0x00
#define White 0xFE

extern uint8 Imagebuf[60][94];
extern int Middle_Line[60];            //中线数组
extern int lose_left[60];
extern int lose_right[60];
extern int black_left[60];                        //左边界数组
extern int black_right[60];
extern int get;
extern int8 find_A_flag;
extern int8 find_B_flag;
extern void Line_Draw(void);
extern int distance1;
extern int one_obstacle;
extern int8 sanlun_left_huandao_flag;
extern int8 sanlun_right_huandao_flag;
extern int8 zuoyici;
extern int8 youyici;
extern int find_White();
extern int find_Black();
extern int zhili_find_Black();
extern int zhili_find_White();
extern void find_change();
extern int Round_right();
extern int buxianwan;
extern int8 huandao_flag;
#endif


