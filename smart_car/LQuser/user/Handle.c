#include "Handle.h"

typedef uint8	uchar;  /*  8 bits */
uchar Left_Line[62], Right_Line[62], Mid_Line[62];	// 原始左右边界数据
/************** 环路相关变量 *************/
uint8 Annulus_Count[10] = {1, 1, 1, 1, 0, 0, 1, 0, 1, 0};	// 1左环路，0右环路
uint8 Annulus_Times = 0;	// 环路次数
uint8 Annulus_Flag = 0;         //环路标志
uint8 Annulus_Left = 0;
uint8 Annulus_Right = 0;
int16 Annulus_Delay = 0;
uint8 Annulus_Mode = 0;		// 准备进环路状态
/************** 环路相关变量 *************/
uint8 Line_Count;	// 记录成功识别到的赛道行数
uint8 Out_Side = 0;	// 丢线控制
uint8 Foresight;	// 前瞻，用于速度控制
/************** 断路赛道变量 *************/
uint8 Break_Way = 0;

/************** 横断路障变量 *************/
uint8 Crossroads = 0;

uint8 zhangai = 0;
uint8 Left_Add_Start, Right_Add_Start;	// 左右补线起始行坐标
uint8 Left_Add_Stop, Right_Add_Stop;	// 左右补线结束行坐标
float Left_Ka = 0, Right_Ka = 0;
float Left_Kb = 0, Right_Kb = 0;	// 最小二乘法参数
/////////////////////////////////////////////////////////////////////
uint8 Left_Hazard_Flag, Right_Hazard_Flag;	// 左右障碍物标志
uint8 Left_Max, Right_Min;
int32 Area_Left = 0, Area_Right = 0;	// 左右侧赛面积
uchar Left_Add_Line[62], Right_Add_Line[62];		// 左右边界补线数据
uchar Left_Add_Flag[62], Right_Add_Flag[62];		// 左右边界补线标志位
uchar Jump[62];
uchar Width_Real[62];	// 实际赛道宽度
uchar Width_Add[62];	// 补线赛道宽度
uchar Width_Min;		// 最小赛道宽度

uint8 Foresight;	// 前瞻，用于速度控制
uint8 Line_Count;	// 记录成功识别到的赛道行数

uint8 Left_Add_Start, Right_Add_Start;	// 左右补线起始行坐标
uint8 Left_Add_Stop, Right_Add_Stop;	// 左右补线结束行坐标
uint8 Starting_Line_Flag = 0;	// 起跑线标志位
uint8 straightaway = 0;         //直道标志
uint8 xiao_S = 0;               //小S弯道标志
uint8 da_S = 0;                 //大S弯道标志

//////////////////////////////////////////////////////////////////////
uint8 Fresight[5][2] = {{21, 35},	// 130	前瞻控制
                        {21, 35},	// 140
                        {21, 33},	// 150
                        {21, 31},	// 160
                        {19, 29}};	// 170

unsigned char Fres;

extern long int  keep ;


void Fresight_Calculate(void)                   //计算摄像头 前瞻
{
    if (Speed_Min <= 100)                    //根据左右最小速度来确定前瞻
    {
            Fres = 0;
    }
    else if (Speed_Min > 100 && Speed_Min < 120)
    {
            Fres = 1;
    }
    else if (Speed_Min < 130 && Speed_Min >= 120)
    {
            Fres = 2;
    }
    else if (Speed_Min < 140 && Speed_Min >= 130)
    {
            Fres = 3;
    }
    else if (Speed_Min > 150 && Speed_Min >= 140)
    {
            Fres = 4;
    } 
    else if (Speed_Min < 160 && Speed_Min >= 150)
    {
            Fres = 5;
    }
    else if (Speed_Min < 170 && Speed_Min >= 160)
    {
            Fres = 6;
    }
    else if (Speed_Min < 180 && Speed_Min >= 170)
    {
            Fres = 7;
    } 
    else
    {
            Fres = 8;
    }
    
}

void Fresight_Calculate_dianci(void)           //计算电磁前瞻
{
	if (chabihe < 20&&chabihe>=0)
	{
		Fres = 0;
	}
	else if (chabihe>=20&&chabihe < 30)
	{
		Fres = 1;
	}
	else if (chabihe>= 30&&chabihe < 40)
	{
		Fres = 2;
	}
	else if (chabihe>= 40&&chabihe < 50)
	{
		Fres = 3;
	}
        else if (chabihe>= 50&&chabihe < 60)
	{
		Fres = 4;
	}
        else if (chabihe>= 60&&chabihe < 70)
	{
		Fres = 5;
	}
        else if (chabihe>= 70&&chabihe < 80)
	{
		Fres = 6;
	}
        else if (chabihe>= 80&&chabihe < 90)
	{
		Fres = 7;
	}
        else if (chabihe>= 90)
	{
		Fres = 8;
	}
        else if (chabihe<0 && chabihe>=-20)
        {
               Fres = 0;
        }
        else if (chabihe<-20 && chabihe>=-30)
        {
               Fres = 1;
        }
        else if (chabihe<-30 && chabihe>=-40)
        {
               Fres = 2;
        }
        else if (chabihe<-40 && chabihe>=-50)
        {
               Fres = 3;
        }
        else if (chabihe<-50 && chabihe>=-60)
        {
               Fres = 4;
        }
        else if (chabihe<-60 && chabihe>=-70)
        {
               Fres = 5;
        }
        else if (chabihe<-70 && chabihe>=-80)
        {
               Fres = 6;
        }
        else if (chabihe<-80 && chabihe>=-90)
        {
               Fres = 7;
        }
        else if (chabihe<-90)
        {
               Fres = 8;
        }
//        keep = Fres;       
}


