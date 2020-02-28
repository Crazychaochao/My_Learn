/***********************************************
    PID.h
* 修改时间：2018年01月04日  By DXC
* 备 注： 
***********************************************/
#ifndef __PID_H
#define __PID_H
#include "include.h"
#include "EandV.h"//必须要有

extern void SetMotorPWM();
extern void Set_Pulse();
extern  uint16 abs1(int16 x);
extern float abs2(float x);
extern void SetMotorPWM(int V_Left,int V_Right);
extern int SpeedPID_left(int16 SetSpeed,int16 Acture_Pulse);
extern int SpeedPID_right(int16 SetSpeed,int16 Acture_Pulse);
//extern int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point) ;

#define Motor_Kpp   3   //3
#define Motor_Ki    0.5    //0.5
#define Motor_Kd    0      //0
#define MaxSpeed 500
#endif