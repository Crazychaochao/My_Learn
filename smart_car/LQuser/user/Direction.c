/*******************************************************************************
							   Future			
*@ 文件名 ： LSD_SERMOTOR.C
*@ 说明   ： PID，陀螺仪，基本算法控制

*******************************************************************************/
#include "Direction.h"

float PD_OUT = 0;
float PD_OUT_Last = 0;
float SumError = 0;
float Last_SumError = 0;

