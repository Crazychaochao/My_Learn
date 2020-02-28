#ifndef __HANDLE_H__
#define __HANDLE_H__
#include "common.h"
#include "include.h"
/************** 环路相关变量 *************/
extern uint8 Annulus_Count[10];	// 1左环路，0右环路
extern uint8 Annulus_Times;	// 环路次数
extern uint8 Annulus_Flag;         //环路标志
extern uint8 Annulus_Left;
extern uint8 Annulus_Right;
extern int16 Annulus_Delay;
extern uint8 Annulus_Mode;		// 准备进环路状态
/************** 环路相关变量 *************/
extern uint8 Break_Way;
extern uint8 Crossroads;
extern uint8 zhangai;
extern uint8 Line_Count;
extern uint8 Foresight;	// 前瞻，用于速度控制
extern unsigned char Fres;
extern void Fresight_Calculate(void);
extern int32 Point_Average(void);
extern void Fresight_Calculate_dianci(void);

extern uint8 Starting_Line_Flag;	// 起跑线标志位

extern uint8 straightaway;         //直道标志
extern uint8 xiao_S;               //小S弯道标志
extern uint8 da_S;                 //大S弯道标志
uint8 First_Line_Handle(uint8 *data);
uint8 Traversal_Left(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max);
void Annulus_Control(void);
void Handling_Common_Images(uint8 *data);			// 普通图像处理
uint8 Traversal_Right_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line);
uint8 Corrode_Filter(uint8 i, uint8 *data, uint8 Left_Min, uint8 Right_Max);
void Traversal_Mid_Line(uint8 i, uint8 *data, uint8 Mid, uint8 Left_Min, uint8 Right_Max, uint8 *Left_Line, uint8 *Right_Line, uint8 *Left_Add_Line, uint8 *Right_Add_Line);
uint8 Calculate_Add(uint8 i, float Ka, float Kb);	// 计算补线坐标
void Curve_Fitting(float *Ka, float *Kb, uint8 *Start, uint8 *Line, uint8 *Add_Flag, uint8 Mode);
void Line_Repair(uint8 Start, uint8 Stop, uint8 *data, uint8 *Line, uint8 *Line_Add, uint8 *Add_Flag, uint8 Mode);
void Mid_Line_Repair(uint8 count, uint8 *data);
uint8 Traversal_Left_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line);
uint8 Traversal_Right(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max);
void Image_Para_Init(void);
void Annulus_Handle(uint8 *data);
#endif