/***********************************************
    PID.c
* 修改时间：2018年01月04日  By DXC
* 备 注： 
***********************************************/
#include "PID.h"

extern float SumError;
 
extern short Leftspeed;
extern short Rightspeed;
//PID MOTOR_PID = 0, Angle_PID = 0, Ang_Vel_PID = 0, Direct_PID = 0, Turn_PID = 0, Distance_PID = 0;

int16 El[3]={0,0,0};
int16 Er[3]={0,0,0};

/*****************************************************
速度环总函数
By DXC
左右电机PID设置
 ****************************************************/

void Set_Pulse()
{
  static int V_Left=0,V_Right=0, PWM_Right=0,PWM_Left=0;   
  int SetPulse;

    PWM_Left = (int16)(SpeedPID_left(V_Left,Leftspeed));//增量式PID
    PWM_Right= (int16)(SpeedPID_right(V_Right,Rightspeed));
  
   if(PWM_Left<-MaxSpeed)  PWM_Left=-MaxSpeed;    //限幅
   if(PWM_Right<-MaxSpeed) PWM_Right=-MaxSpeed;

   if(PWM_Left>MaxSpeed)  PWM_Left=MaxSpeed;
   if(PWM_Right>MaxSpeed) PWM_Right=MaxSpeed;

   SetMotorPWM(PWM_Left,PWM_Right);
} 







/*****************************************************
 设置左右电机的PWM
下面这个求绝对值 By DXC
 uint16 abs1(int16 x)
{
    if(x<0)  return -x;
    else     return x;
}
 ****************************************************/
 uint16 abs1(int16 x)
{
    if(x<0)  return -x;
    else     return x;
}

float abs2(float x)
{
    if(x<0)  return -x;
    else     return x;
}

//电机的接口如果改变了，可以改变下列宏定义
#define CH0 FTM_CH0 
#define CH1 FTM_CH1 
#define CH2 FTM_CH2 
#define CH3 FTM_CH3 
//左右电机占空比设计
/*
void SetMotorPWM(int V_Left,int V_Right)
{ 
    if(V_Right>0)
    {
        V_Right = range_protect(V_Right, 0, MOTOR_MAX);	// 限幅保护
        FTM_PWM_Duty(FTM0,CH0,V_Right);
        FTM_PWM_Duty(FTM0,CH0,0);
    }
    else if(V_Right<0)
    {
         V_Right = range_protect(-V_Right, 0, MOTOR_MAX);	// 限幅保护
         FTM_PWM_Duty(FTM0,CH0,abs1(V_Right));
         FTM_PWM_Duty(FTM0,CH3,0);
    }
    else//V_Right == 0
    {
      V_Right = range_protect(V_Right, 0, MOTOR_MAX);	// 限幅保护
      FTM_PWM_Duty(FTM0,CH0,0);
      FTM_PWM_Duty(FTM0,CH3,0);
    }
    if(V_Left>0)
    {
         V_Left = range_protect(V_Left, 0, MOTOR_MAX);	// 限幅保护
         FTM_PWM_Duty(FTM0,CH0,0);
         FTM_PWM_Duty(FTM0,CH1,V_Left);
    }
    else if(V_Left<0)
    {
         V_Left = range_protect(-V_Left, 0, MOTOR_MAX);	// 限幅保护
         FTM_PWM_Duty(FTM0,CH0,0);
         FTM_PWM_Duty(FTM0,CH1,abs1(V_Left));
    }
    else
    {
         V_Left = range_protect(V_Left, 0, MOTOR_MAX);	// 限幅保护
         FTM_PWM_Duty(FTM0,CH0,0);
         FTM_PWM_Duty(FTM0,CH1,0);
    }
    if(SumError>0) //误差大于0，中线在左，右边加速
    {
      FTM_PWM_Duty(FTM0,CH0,abs1(V_Left-PD_OUT));
      FTM_PWM_Duty(FTM0,CH1,0);
      FTM_PWM_Duty(FTM0,CH2,0);
      FTM_PWM_Duty(FTM0,CH3,abs1(V_Right+PD_OUT));     
    }
    else if(SumError<0)//误差小于0，中线在右，左边加速
    {
      FTM_PWM_Duty(FTM0,CH0,abs1(V_Left+PD_OUT));
      FTM_PWM_Duty(FTM0,CH1,0);
      FTM_PWM_Duty(FTM0,CH2,0);
      FTM_PWM_Duty(FTM0,CH3,abs1(V_Right-PD_OUT));      
    }
    else                    //没有误差，维持速度不变
    {
      FTM_PWM_Duty(FTM0,CH0,0);
      FTM_PWM_Duty(FTM0,CH1,abs1(V_Left));
      FTM_PWM_Duty(FTM0,CH2,abs1(V_Right));
      FTM_PWM_Duty(FTM0,CH3,0);   
    }
    
    if(asdf>0)
    {  
     FTM_PWM_Duty(FTM0,CH0,0);
     FTM_PWM_Duty(FTM0,CH1,abs1(350+PD_OUT));//zuo jia su
     FTM_PWM_Duty(FTM0,CH2,abs1(350-PD_OUT));
     FTM_PWM_Duty(FTM0,CH3,0);
    }
    else
    {   
     FTM_PWM_Duty(FTM0,CH0,0);
     FTM_PWM_Duty(FTM0,CH1,abs1(350-PD_OUT));
     FTM_PWM_Duty(FTM0,CH2,abs1(350+PD_OUT));
     FTM_PWM_Duty(FTM0,CH3,0);
    }
    
     
}
*/

/**************************************************
   电机PID

左一个，右一个
变量定义
int16 El[3]={0,0,0};
int16 Er[3]={0,0,0};
#define Motor_Kpp   3
#define Motor_Ki    0.5
#define Motor_Kd    0
**************************************************/
//左边电机PID，增量式PID
int SpeedPID_left(int16 SetSpeed,int16 Acture_Pulse)
{
    static int16 PWM_Out=0,Pwm_Out=0;
    El[0] = SetSpeed - Acture_Pulse; //*********
    Pwm_Out = (int)(Motor_Kpp*(El[0]-El[1]) + Motor_Ki*El[0] + Motor_Kd*(El[0]-2*El[1]+El[2]));//PID计算
  
    El[2] = El[1];
    El[1] = El[0];
   
    PWM_Out +=Pwm_Out; //输出PWM

    if(PWM_Out>MaxSpeed)  PWM_Out=MaxSpeed;
    if(PWM_Out<-MaxSpeed) PWM_Out=-MaxSpeed;  //电机限幅

    return PWM_Out;
}
//右边电机PID，增量式PID
 int SpeedPID_right(int16 SetSpeed,int16 Acture_Pulse)
{
   static int16 PWM_Out=0,Pwm_Out=0;
    Er[0] = SetSpeed- Acture_Pulse;    //************

    Pwm_Out =(int)(Motor_Kpp*(Er[0]-Er[1]) + Motor_Ki*Er[0] + Motor_Kd*(Er[0]-2*Er[1]+Er[2]));

    Er[2] = Er[1];
    Er[1] = Er[0];

     PWM_Out +=Pwm_Out; //输出PWM

    if(PWM_Out>MaxSpeed)  PWM_Out=MaxSpeed;
    if(PWM_Out<-MaxSpeed) PWM_Out=-MaxSpeed;  //电机限幅

    return PWM_Out;
}






