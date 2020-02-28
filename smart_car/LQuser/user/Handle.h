#ifndef __HANDLE_H__
#define __HANDLE_H__
#include "common.h"
#include "include.h"
/************** ��·��ر��� *************/
extern uint8 Annulus_Count[10];	// 1��·��0�һ�·
extern uint8 Annulus_Times;	// ��·����
extern uint8 Annulus_Flag;         //��·��־
extern uint8 Annulus_Left;
extern uint8 Annulus_Right;
extern int16 Annulus_Delay;
extern uint8 Annulus_Mode;		// ׼������·״̬
/************** ��·��ر��� *************/
extern uint8 Break_Way;
extern uint8 Crossroads;
extern uint8 zhangai;
extern uint8 Line_Count;
extern uint8 Foresight;	// ǰհ�������ٶȿ���
extern unsigned char Fres;
extern void Fresight_Calculate(void);
extern int32 Point_Average(void);
extern void Fresight_Calculate_dianci(void);

extern uint8 Starting_Line_Flag;	// �����߱�־λ

extern uint8 straightaway;         //ֱ����־
extern uint8 xiao_S;               //СS�����־
extern uint8 da_S;                 //��S�����־
uint8 First_Line_Handle(uint8 *data);
uint8 Traversal_Left(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max);
void Annulus_Control(void);
void Handling_Common_Images(uint8 *data);			// ��ͨͼ����
uint8 Traversal_Right_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line);
uint8 Corrode_Filter(uint8 i, uint8 *data, uint8 Left_Min, uint8 Right_Max);
void Traversal_Mid_Line(uint8 i, uint8 *data, uint8 Mid, uint8 Left_Min, uint8 Right_Max, uint8 *Left_Line, uint8 *Right_Line, uint8 *Left_Add_Line, uint8 *Right_Add_Line);
uint8 Calculate_Add(uint8 i, float Ka, float Kb);	// ���㲹������
void Curve_Fitting(float *Ka, float *Kb, uint8 *Start, uint8 *Line, uint8 *Add_Flag, uint8 Mode);
void Line_Repair(uint8 Start, uint8 Stop, uint8 *data, uint8 *Line, uint8 *Line_Add, uint8 *Add_Flag, uint8 Mode);
void Mid_Line_Repair(uint8 count, uint8 *data);
uint8 Traversal_Left_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line);
uint8 Traversal_Right(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max);
void Image_Para_Init(void);
void Annulus_Handle(uint8 *data);
#endif