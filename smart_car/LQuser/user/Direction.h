#ifndef __DIRECTION_H
#define __DIRECTION_H
#include "include.h"
#define PWM_Speed  6000

extern float PD_OUT;
extern void Sermotor_PD( void );
void Sermotor_PD_Zhi_Li( void );
void Sermotor_PD2_zhi_li(float sum_error);
extern float SumError;
extern void SetPwm();
float Cal_Center_Error( uint8 start_x,uint8 end_x,uint8 Clear_Flag);


#endif