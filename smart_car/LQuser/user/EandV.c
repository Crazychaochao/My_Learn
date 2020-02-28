#include "EandV.h"
/*
float Direct[4]  = {0.4, 0.0, 0.163, 0};	// 转向环PID 位置
float MOTOR[4]   = {25,0,2,0};		// 速度环PID 最后一项为积分限幅
float Angle[4]   = {0.3, 0, 0.1, 500};		// 角度环PID 
float Ang_Vel[4] = {3.6, 0.108, 0.0027, 1000};
*/
float sanlunDirect[4]  = {0.05, 0.0, 0.118, 0};	// 转向环PID 位置 0.35, 0.0, 0.118, 0
float sanlunAngle[4]   = {0.2, 0, 0.1, 500};
float sanlunMOTOR[4]   = {10,0,2,0};	

float Direct[4]  = {0.05,0.0,0.118, 0};	// 转向环PID 位置
float MOTOR[4]   = {60,0,5,0};		// 速度环PID 最后一项为积分限幅

float Angle[4] = {0.3, 0.0, 0.15, 0};
float Ang_Vel[4] = {1.5, 0.108, 0.0027, 1000};

float MOTOR_sanlun[4]   = {20,0,2,0};		// 速度环PID 最后一项为积分限幅
float Ang_Vel_sanlun[4] = {1.5, 0.105, 0.0035, 1000}; 


char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, DMP_Flag;

PID MOTOR_PID, Angle_PID, Ang_Vel_PID, Direct_PID, Turn_PID, Distance_PID;//定义PID，角度pid，角速度pid，方向pid，转向pid，位置pid
float sanlunTurn[9][4] ={{8, 0, 0.0, 0},	        // 130	// 转向外环动态PID	面积法
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 150
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0},
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0}};   // 170、

float Turn[9][4] = {{8, 0, 0.0, 0},	        // 130	// 转向外环动态PID	面积法
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 150
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0},
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0}};   // 170、

float Turn_picture[5][4] = {{100, 7, 7, 400},	// 130	// 转向外环动态PID	
                            {100, 8, 8, 400},	// 140
                            {100, 8, 8, 400},	// 150
                            {100, 9, 9, 400},	// 160
                            {100, 10, 10, 400}};// 170

/*float Turn_DianCi[10][4] = {{100, 5, 45, 400},	// 130	// 转向外环动态PID	中线法
                            {100, 5, 45, 400},	// 140
                            {100, 5, 45, 400},	// 150
                            {100, 6, 45, 700},	// 160
                            {100, 6, 45, 700},
                            {100, 6, 45, 720},
                            {100, 7, 45, 740},
                            {100, 7, 45, 760},
                            {100, 7, 45, 780},
                            {100, 8, 45, 800}};     // 170
*/
float Turn_DianCi[10][4] = {{100, 5, 50, 400},	// 130	// 转向外环动态PID	中线法
                            {100, 5, 50, 400},	// 140
                            {100, 5, 50, 400},	// 150
                            {100, 5, 50, 400},	// 160
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400}};     // 170

#define Middle_Angle     0   //机械中值, 与电池的重量有关 1000
int32 MOTOR_Duty_Left  = 0;
int32 MOTOR_Duty_Right = 0;
S_FLOAT_XYZ 
      GYRO_Real,		// 陀螺仪转化后的数据
      ACC_Real,		        // 加速度计转化后的数据
      Attitude_Angle,	        // 当前角度 
      Last_Angle,		// 上次角度
      Target_Angle;	        // 目标角度
S_INT16_XYZ
      GYRO,			// 陀螺仪原始数据
      GYRO_Offset,	        // 陀螺仪温飘
      GYRO_Last,		// 陀螺仪上次数据
      ACC, 			// 加速度计数据
      ACC_Offset,		// 加速度计温飘
      ACC_Last;		        // 加速度计上次数据
S_INT32_XYZ
      Tar_Ang_Vel,	        // 目标角速度
      Tar_Ang_Vel_Last; 	// 上次目标角速度

int32 
      Speed_Now,		// 当前实际速度
      Speed_Min,		// 左右最小速度
      Speed_Set, 		// 目标设定速度
      Theory_Duty = 0,          // 理论直立占空比
      Vel_Set,		        // 目标转向角速度
      Direct_Parameter = 0,         //转向的参数
      Direct_Last = 0,          //上一次的转向
      Radius;                   //半径
int32 duty_a = 0;               //添加用于直立
int32 Direct_Parameter_Last = 0;
int32 Difference = 0;
uint8_t System_OK = 0;
uint8 Run_Flag = 0;      //小车跑的标志位       
extern uint8 Stop_Flag = 0;     //小车停的标志位 
uint8 Run_Stop = 1;
char Left_Crazy = 0;	// 电机疯转
char Right_Crazy = 0;	// 电机疯转
int32 RC_Get = 1500;	// 遥控器的通道值
int32 MOTOR_Speed_Left = 0;
int32 MOTOR_Speed_Right = 0; 
int32 MOTOR_Speed_Left_Last = 0;
int32 MOTOR_Speed_Right_Last = 0;
int32 MOTOR_Left_Acc = 0;
int32 MOTOR_Right_Acc = 0;
int16 a[4];
uint32 Annulus_Error = 0;	// 环路误判控制
int zhili_flag = 0;
extern long  int dirrectPID;
extern long  int diangan ;
 int32 MOTOR_Duty_L  = 0;
 int32 MOTOR_Duty_R = 0;

/******************************串级平衡控制*****************************************/
void Balance_Control(void)
{
    float Accel_Angle;
    //直立角速度环完成
    if (Ang_Velocity_Flag)	// 直立角速度环	2ms，在pit1中端中，标志位Ang_Velocity_Flag已经置1
    {
        Ang_Velocity_Flag = 0;         
        MPU6050_GetData(&GYRO, &ACC);	
        Data_Filter();	
        
        if(disconnect_flag == 1 || Obstacle_flag == 1 || Obstacle_flag == 2)  //三轮状态
        {
              if(Attitude_Angle.Y >= -4)
              {
                  Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// 计算直立PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -1500, 1500);  
              }   
              if(Attitude_Angle.Y < -4 && Attitude_Angle.Y >= -5)
              {
                  Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// 计算直立PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -2000, 2000);            //相幅保护，占空比最大60%，最小-60% 
              }
              if(Attitude_Angle.Y < -5 && Attitude_Angle.Y > -6)
              {
                  Theory_Duty +=  PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// 计算直立PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -3000, 3000);            //相幅保护，占空比最大60%，最小-60% 
              }
              if(Attitude_Angle.Y < -6)
              {
                  Theory_Duty +=  PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// 计算直立PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -3500, 3500);            //相幅保护，占空比最大60%，最小-60% 
              }
        }
        else //直立状态       
        {
            Theory_Duty +=  1 * (PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y)));	// 计算直立PWM
            Theory_Duty = (int32) range_protect(Theory_Duty, -9000, 9000); 
        }
        
        if (System_OK)
        {    									
            if(disconnect_flag == 1 || Obstacle_flag == 1 || Obstacle_flag == 2)     // 三轮
            {
                Direct_Parameter_Last = Direct_Parameter;
                Direct_Parameter = 1 * PID_Realize(&Direct_PID, sanlunDirect, (int32)((GYRO_Real.Z)*100) ,Radius*Speed_Min);	
                Direct_Parameter = (int32) (0.7 * Direct_Parameter + 0.3 * Direct_Parameter_Last);
                Direct_Parameter = range_protect(Direct_Parameter, -3000, 3000);
            }
            else    //直立
            {
                Direct_Parameter_Last = Direct_Parameter;
                Direct_Parameter = 1 * PID_Realize(&Direct_PID, Direct, (int32)((GYRO_Real.Z)*100) ,Radius*Speed_Min);	// 转向环左正右负
                Direct_Parameter = (int32) (0.7 * Direct_Parameter + 0.3 * Direct_Parameter_Last);
                Direct_Parameter = range_protect(Direct_Parameter, -3000, 3000);
            }
        
        }
        Direct_Last = (int32) (Direct_Last*0.3 + Direct_Parameter*0.7);	// 更新上次角速度环结果,Direct_Parameter System_OK为1时的pwm值
        MOTOR_Duty_Left  = (int32) (1 * (Theory_Duty + Direct_Last));	// 左右电机根据转向系数调整差速
        MOTOR_Duty_Right = (int32) (1 * (Theory_Duty - Direct_Last));	//在这里可以实现转向,往右转
        MOTOR_Duty_Left = (int32) range_protect(MOTOR_Duty_Left, -9000, 9000);            
        MOTOR_Duty_Right = (int32) range_protect(MOTOR_Duty_Right, -9000, 9000);           
        
        if (Run_Flag)   //在system.c中置1
        {
            Test_MOTOR_Control(MOTOR_Duty_Left, MOTOR_Duty_Right);	// 改变占空比，控制左右电机            
        }
        else
        {
            if (Stop_Flag)
            {
                if (Speed_Now > 20)
                {
                    
                    Test_MOTOR_Control(-3000, -3000);
                }
                else
                {
                    Test_MOTOR_Control(0, 0);
                }
            }
            else //Stop_Flag标志位不为1时
            {
                Test_MOTOR_Control(0, 0);//判断为疯转之后，停车
            }
        }
        Accel_Angle=-atan2(ACC_Real.X,ACC_Real.Z)*180/PI;    
        KalmanFilter(Accel_Angle); 
            
    }	
    if (Angle_Flag)		// 直立角度环 角度10ms控制一次
    {
        Angle_Flag = 0;
        Speed_Measure();                                                                     /* 角度环加到角速度环上串级控制 */
        if(disconnect_flag==1)   //三轮
        {
            Tar_Ang_Vel.Y = 1 * (PID_Realize(&Angle_PID, sanlunAngle, (int32)(Attitude_Angle.Y*100), (int32)Target_Angle.Y));	// 结果为放大10倍的目标角速度	
            Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500);	
        }
        else                    //直立
        {
            Tar_Ang_Vel.Y = 1 * (PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), (int32)Target_Angle.Y));	// 结果为放大10倍的目标角速度	
            Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500);	      
        }
        if(Attitude_Angle.Y < -9)               zhili_flag = 1;
        else                                    zhili_flag = 0;
    }
    if (Speed_Flag)	     // 速度环	
    {
      if(disconnect_flag==1 || Obstacle_flag == 1 || Obstacle_flag == 2)  //三轮
      {
        Speed_Flag = 0;                                                                                       
        Target_Angle.Y = -PID_Realize(&MOTOR_PID, sanlunMOTOR, Speed_Now, Speed_Set);	   
        Target_Angle.Y = range_protect((int32)Target_Angle.Y, 300, 1800);	
      }
      else                    //直立
      {
        Target_Angle.Y = -PID_Realize(&MOTOR_PID, MOTOR, Speed_Now, Speed_Set);	    
        Target_Angle.Y = range_protect((int32)Target_Angle.Y, -2000, -1450);	
      }
      if(jian == 1)
      {
          Target_Angle.Y = -PID_Realize(&MOTOR_PID, MOTOR, Speed_Now, Speed_Set);	    
          Target_Angle.Y = range_protect((int32)Target_Angle.Y, -1500, -1450);
      }
      Speed_Min = (int32) (Speed_Min * 0.1 + Speed_Now * 0.9);
      if (Speed_Min < 40) 
      {
          Speed_Min = 40;
      }
    }
}


/*****************************************************
位置式PID控制
 ****************************************************/

int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	// 当前误差
		 Realize;	// 最后得出的实际增量

	iError = Point - NowData;	// 计算当前误差
	sptr->SumError += PID[KI] * iError;	// 误差积分
	if (sptr->SumError >= PID[KT])
	{
            sptr->SumError = PID[KT];
	}
	else if (sptr->SumError <= -PID[KT])
	{
            sptr->SumError = -PID[KT];
	}

	Realize = (int32) (PID[KP] * iError
			+ sptr->SumError 
			+ PID[KD] * (iError - sptr->LastError));
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		// 更新上次误差
	sptr->LastData  = NowData;		// 更新上次数据

	return Realize;	// 返回实际值
}

/*****************************************************
 限幅保护，返回占空比
 ****************************************************/
int32 range_protect(int32 duty, int32 min, int32 max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}

int32 PulsesL = 0,PulsesR = 0;
/******* 电机速度测量 ********/ 
void Speed_Measure(void)
{
      char txt[16]; 
      static int32 Speed_Last = 0;
      static int32 Crazy_Count = 0;
      //Test_AB_Pulse_Cnt();
      PulsesR =FTM_AB_Get(FTM1);     //获取正交解码速度，正负表示方向;   
      ftm_clean(FTM1) ;   
      PulsesR = -PulsesR;
      PulsesL =FTM_AB_Get(FTM2);     //获取正交解码速度，正负表示方向; 
      ftm_clean(FTM2) ;   
      PulsesL = PulsesL;
      /******* 右电机速度相关控制 ********/
  
      MOTOR_Speed_Right = PulsesR;	// 得到右轮转速
      MOTOR_Right_Acc = MOTOR_Speed_Right - MOTOR_Speed_Right_Last;	// 计算加速度
      if (MOTOR_Right_Acc > 100)
      {
              Right_Crazy = 1;	// 疯转
      }
      if (MOTOR_Speed_Right > Speed_Set + 200)
      {
              Right_Crazy = 2;	// 疯转
      }
      if (MOTOR_Speed_Right < -350)
      {
              Right_Crazy = -1;	// 倒转
      }
      
      if (Right_Crazy)
      {
              if (MOTOR_Right_Acc <= 100)
              {
                      if (MOTOR_Speed_Right < Speed_Set + 200 && MOTOR_Speed_Right > 0)
                      {
                              Right_Crazy = 0;
                      }
              }
      }
      
      if (!Right_Crazy)
      {
              MOTOR_Speed_Right =(int32) (MOTOR_Speed_Right*0.9 + MOTOR_Speed_Right_Last*0.1);
              MOTOR_Speed_Right_Last = MOTOR_Speed_Right;	// 更新右轮速度
      }
      else
      {
              MOTOR_Speed_Right = (int32) (MOTOR_Speed_Right*0.5 + MOTOR_Speed_Right_Last*0.5);
              MOTOR_Speed_Right_Last = MOTOR_Speed_Right;	// 更新右轮速度
      }
      /******* 右电机速度相关控制结束 ********/
      
      /******* 左电机速度相关控制 ********/
      MOTOR_Speed_Left = PulsesL;	// 得到右轮转速
      MOTOR_Left_Acc = MOTOR_Speed_Left - MOTOR_Speed_Left_Last;	// 计算速度的增值
      if (MOTOR_Left_Acc > 100)                                       //如果速度的增值大于100，判断为疯转
      {
              Left_Crazy = 1;
      }
      if (MOTOR_Speed_Left > Speed_Set + 200)                         //如果电机的速度大于（目标速度+200），判断为疯转
      {
              Left_Crazy = 2;
      }
      if (MOTOR_Speed_Left < -350)                                    //如果电机的速度小于-350，判断为疯转       
      {
              Left_Crazy = -1;
      }
      
      if (Left_Crazy)                                                 //如果电机疯转
      {
              if (MOTOR_Left_Acc <= 100)                              //并且速度的增量<=100
              {
                      if (MOTOR_Speed_Left < Speed_Set + 200 && MOTOR_Speed_Left > 0)//电机的速度大于（目标速度+200）和电机的速度大于0
                      {
                              Left_Crazy = 0;                                         //则不疯转
                      }
              }
      }
      
      if (!Left_Crazy)                                                //如果不电机疯转
      {
              MOTOR_Speed_Left = (int32) (0.9*MOTOR_Speed_Left + 0.1*MOTOR_Speed_Left_Last);	// 低通滤波
              MOTOR_Speed_Left_Last = MOTOR_Speed_Left;	// 更新左轮速度
      }
      else
      {
              MOTOR_Speed_Left = (int32) (0.5*MOTOR_Speed_Left + 0.5*MOTOR_Speed_Left_Last);	// 低通滤波
              MOTOR_Speed_Left_Last = MOTOR_Speed_Left;	// 更新左轮速度
      }

      
      /******* 左电机速度相关控制结束 ********/
      
      if ((Left_Crazy && Right_Crazy) || (Left_Crazy && MOTOR_Speed_Right < 20) || (Right_Crazy && MOTOR_Speed_Left < 20))
      {
            Crazy_Count++;
            if (Crazy_Count >= 40)
            {
                    Crazy_Count = 0;
                    Run_Flag = 0;
            }
      }
      else
      {
            Right_Crazy = 0;
      }
      
      /******* 电机疯转特殊处理 ********/
      if ((Left_Crazy > 0) && (Right_Crazy > 0))
      {
              Speed_Now = Speed_Set;			
      }
      else if (Left_Crazy)
      {
            if (MOTOR_Speed_Right > Speed_Set)
            {
                    Speed_Now = Speed_Set;
            }
            else
            {
                    Speed_Now = MOTOR_Speed_Right;	// 左电机疯转，使用上次速度作为当前实际速度
            }
      }
      else if (Right_Crazy)
      {
            if (MOTOR_Speed_Left > Speed_Set)
            {
                    Speed_Now = Speed_Set;
            }
            else
            {
                    Speed_Now = MOTOR_Speed_Left;	// 右电机疯转，使用上次速度作为当前实际速度
            }
      }
      else
      {
              Speed_Now = (MOTOR_Speed_Left + MOTOR_Speed_Right) / 2;	// 左右取平均计算车子实际速度
      }        
      Speed_Now = (int32) (Speed_Now *0.9 + Speed_Last * 0.1);
      Speed_Last = Speed_Now;
  //        dirrectPID = MOTOR_Speed_Left;
  //        diangan = MOTOR_Speed_Right;
  /*	if (Annulus_Mode == 1 || Annulus_Mode == 2)
      {
              Annulus_Error += Speed_Now;
              if (Annulus_Error >= 10000)
              {
                      Annulus_Error = 0;
                      
                      led(LED_MAX,LED_OFF);// 关闭LED指示灯
                      Annulus_Mode = 0;
                      Annulus_Flag = 0;
                      if (Annulus_Times)
                      {
                              Annulus_Times--;
                      }
                      else
                      {
                              Annulus_Times = 9;
                      }
              }
      }
      else
      {
              Annulus_Error = 0;
      }*/
}

void Test_MOTOR_Control(int32 LDuty, int32 RDuty)
{
    if (LDuty >= 0)
    {
        LDuty = range_protect(LDuty, 0, MOTOR_MAX);	// 限幅保护                          
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH0,0);	  	// 占空比最大990！！！
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH1,LDuty);	// 占空比最大990！！！
    }
    else
    {
        LDuty = range_protect(-LDuty, 0, MOTOR_MAX);// 限幅保护
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH1,0);		// 占空比最大990！！！
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH0,LDuty);	// 占空比最大990！！！
    }
    if (RDuty >= 0)
    {
          RDuty = range_protect(RDuty, 0, MOTOR_MAX);	// 限幅保护
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH3,0);	  	// 占空比最大990！！！
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH2,RDuty);	// 占空比最大990！！！	
    }
    else
    {
          RDuty = range_protect(-RDuty, 0, MOTOR_MAX);// 限幅保护
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH2,0);		// 占空比最大990！！！
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH3,RDuty);	// 占空比最大990！！！
    }
}


void Speed_Control(void)
{
    switch(Mode_Set)
    {
        case 0:	        Speed_Set = 120;        //120
                                break;
        case 1:		Speed_Set = 130;        //130
                                break;
        case 2:		Speed_Set = 140;	// 默认初始速度 140
                                break;
        case 3:		Speed_Set = 150;        //150
                                break;
        case 4:		Speed_Set = 155;        //155
                                break;
        case 5:		Speed_Set = 160;        //160
                                break;
        case 6:		Speed_Set = 165;        //165
                                break;
        case 7:		Speed_Set = 170;        //150
                                break;
        case 8:		Speed_Set = 175;        //155
                                break;
        case 9:		Speed_Set = 180;        //160
                                break;
        case 10:	Speed_Set = 185;        //165
                                break;
        case 11:	Speed_Set = 190;        //150
                                break;
        case 12:	Speed_Set = 195;        //155
                                break;
        case 13:	Speed_Set = 200;        //160
                                break;
        case 14:	Speed_Set = 210;        //165
                                break;
        default:	Speed_Set = 0;          //0
                                break;
    }
}

// 增量式PID电机控制
//在Balance_Control()中被调用
//参数输入：角速度pid，角速度，陀螺仪的Y值，目标角速度Y
//每次增量Increase减1
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point) 
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		Increase;	//最后得出的实际增量

	iError = Point - NowData;	// 计算当前误差

	Increase =  (int32) (PID[KP] * (iError - sptr->LastError)
			  + PID[KI] * iError
			  + PID[KD] * (iError - 2 * sptr->LastError + sptr->PrevError));
	
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		  	// 更新上次误差
	sptr->LastData  = NowData;			// 更新上次数据
	return Increase;	// 返回增量
}


/* 初始化用到的一些变量 */
void Balance_Init(void)
{
	Attitude_Angle.Y = 0;           // 当前角度
	Tar_Ang_Vel.Y = 0;              // 目标角速度
	Tar_Ang_Vel.Z = 0;
}

// PID参数初始化
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0; 
}


int d[4];
		//动态P
	
// 位置式动态PID控制
int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		  Actual;	//最后得出的实际输出值
	float Kp = 0;
	iError = SetPoint - NowPiont;	//计算当前误差,设定值-测量值
	sprt->SumError += iError*0.01;  //偏差积分
//        diangan = sprt->SumError;
	if (sprt->SumError >= PID[KT])
	{
		sprt->SumError = PID[KT];
	}
	else if (sprt->SumError <= -PID[KT])
	{
		sprt->SumError = -PID[KT];
	}
	//Kp = 0.453 * (iError*iError) / PID[KP] + PID[KI];	//P值与差值成二次函数关系，此处P和I不是PID参数，而是动态PID参数，要注意！！！
	Kp = 0.08 * (iError*iError) / PID[KP] + PID[KI];
        //Kp = PID[KI];
        //dirrectPID =  Kp;
        if(huandao_chu == 1)
        {
            Kp = 0.005 * (iError*iError) / PID[KP] + PID[KI];
        }
	Actual = (int32) (Kp * iError
		   + PID[KD] * ((0.8*iError + 0.2*sprt->LastError) - sprt->LastError));//只用PD，得到实际输出值
	sprt->LastError = iError;		//更新上次误差
//	Actual += sprt->SumError*0.1;
        
	Actual = range_protect(Actual, -1000, 1000);      //限幅保护，返回占空比
//        deep_y = Actual;
	return Actual;                                  //得到实际输出值，返回占空比
}
PID * Data;
int32 Radius_PID(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
    //定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
    int32 iError,	                //当前误差
             result;	              //最后得出的实际输出值
    int32 Last_result;
    int32 Increase;
    float Kp = 0,Kd;
    float proportional_range;           //比例度
    uint8 flag;
    
    iError = SetPoint - NowPiont;	//计算当前误差,设定值-测量值
    sprt->SumError += iError*0.001;  //偏差积分
    if (sprt->SumError >= PID[KT])
    {
            sprt->SumError = PID[KT];
    }
    else if (sprt->SumError <= -PID[KT])
    {
            sprt->SumError = -PID[KT];
    }
    Kp = PID[KP];
    Kd  = PID[KD];
    if(right_huandao_flag == 3 || left_huandao_flag == 3 || remain_flag == 1)        Kd = Kd - 0.23;
    //result = (int32) (PID[KP] * iError
	//	   + PID[KD] * ((0.8*iError + 0.2*sprt->LastError) - sprt->LastError) + sprt->SumError);//只用PD，得到实际输出值
    result = (int32) (Kp * iError
		   + Kd * (iError- sprt->LastError));//只用PD，得到实际输出值
    if(Data->PrevError > Data->LastError)       flag = 1;
    else                                        flag = 0;
    Increase = filter_1_x(result,Data->LastError,0.7,flag);             //一阶低通动态滤波
    Data->PrevError = Data->LastError;
    Data->LastError = result;
    result = (int16) (result * 0.7 + sprt->LastError * 0.3);
    sprt->LastError = result;		//更新上次误差
    result = range_protect(result, -260, 260);      //限幅保护，返回占空比
    return Increase;                                  //得到实际输出值，返回占空比
    
}
/*入口：NEW_DATA 新采样值
       OLD_DATA 上次滤波结果
       k        滤波系数(0~255)(代表在滤波结果中的权重)
  出口：         本次滤波结果
 */
char filter_1(char NEW_DATA,char OLD_DATA,char k)
{
    int result;
    if(NEW_DATA<OLD_DATA)
    {
        result=OLD_DATA-NEW_DATA;
        result=result*k;
        result=result+128;//+128是为了四色五入
        result=result/256;
        result=OLD_DATA-result;
    }
    else if(NEW_DATA>OLD_DATA)
    {
        result=NEW_DATA-OLD_DATA;
        result=result*k;
        result=result+128;//+128是为了四色五入
        result=result/256;
        result=OLD_DATA-result;
    }
    else result=OLD_DATA;
    return((char)result);
}
//用MPU6050测得数据；对x轴滤波处理
#define Threshold_1     3       //阈值1用于一阶带参滤波器，变化角度大于此值时，计数增加
#define Threshold_2     8      //阈值2用于一阶带参滤波器，计数值大于此值时，增大参数，增强滤波跟随
float K_x=0;             //滤波系数
u8 new_flag_x=0;         //本次数据变化方向
u8 num_x=0;              //滤波计数器
/*****带系数修改的一阶滤波函数

入口： NEW_DATA    新采样的角度值
      OLD_DATA    上次滤波获得的角度结果
      k           滤波系数(代表在滤波结果中的权重)
      flag        上次数据变化方向
出口： result      本次滤波角度结果
 */
int16 filter_1_x(int16 NEW_DATA,int16 OLD_DATA,float k,u8 flag)
{
    //角度变化方向，new_flag=1表示角度增加，=0表示角度正在减小
    if((NEW_DATA-OLD_DATA)>0)   new_flag_x=1;
    else if((NEW_DATA-OLD_DATA)<0)      new_flag_x=0;
    if(new_flag_x==flag)  //此次变化与前一次变化方向是否一致，相等表示角度变化方向一致
    {
        num_x++;
        if(fabs((NEW_DATA-OLD_DATA))>Threshold_1)
    //当变化角度大于Threshold_1度的时候，进行计数器num快速增加，以达到快速增大K值，提高跟随性
            num_x+=5;                           

        if(num_x>Threshold_2)   //计数阈值设置，当角度递增或递减速度达到一定速率时，增大K值
        {
            K_x=k+0.1;          //0.2为K_x的增长值，看实际需要修改
            num_x=0;
        }
    }
    else 
    {
        num_x=0;
        K_x=0.7;     //角度变化稳定时K_x值，看实际修改
    }
    OLD_DATA=(int16) ((1-K_x)*OLD_DATA+K_x*NEW_DATA);
    return OLD_DATA;
}

void Speed(void)
{
       if(GPIO_Get(PTA5) == 1)       Mode_Set = 8;                //拨码2           //选择目标速度
       if(GPIO_Get(PTA7) == 1)       Mode_Set = 9;               // 拨码3
       if(GPIO_Get(PTA9) == 1)       Mode_Set = 11;              // 拨码4
}
void Start_Control(void)
{
	static uint8 Start_OK = 0;
	static uint8 Ready_Stop = 0;
	static uint16 No_Start_Line_Count = 0;
	
	if (Run_Flag)
	{									/** 起跑阶段检测 **/
            if (!Starting_Line_Flag && !Start_OK)	//没有检测到起跑线且未成功出发
            {
                No_Start_Line_Count++;			//相当于延时约5s
                if (No_Start_Line_Count >= 300)	//连续200次没有检测到起跑线，避免起跑时的光线异常干扰
                {
                        No_Start_Line_Count = 0;
                        Start_OK = 1;	//出发成功
                }
            }
										/** 结束阶段检测 **/
            if (Start_OK && Starting_Line_Flag)	//成功起跑后检测到终点线
            {
                    Ready_Stop = 1;	//准备停车
            }
            
            if (Ready_Stop)
            {
                if (!Starting_Line_Flag)	/** 检测到停车线后延时一段时间停车 **/
                {
                    No_Start_Line_Count++;
                    if (No_Start_Line_Count >= 15)	//连续30次没有检测到起跑线，约延时1s
                    {
                        No_Start_Line_Count = 0;
                        if (Run_Stop)	// 由屏幕设置是否停车，默认停车
                        {
                                Stop_Flag = 1;
                                Run_Flag = 0;			//停车
                        }
                        Start_OK = 0;			//清除出发成功标志位
                        Ready_Stop = 0;			//清除准备停车标志位
                        //led(LED_MAX,LED_ON);	//关闭LED指示灯
                    }
                }
            }
	}
}

/******************************************************************************
断路处理
*******************************************************************************/
void Handle_Break_Way(void)
{
    
}
/******************************************************************************
路障处理
*******************************************************************************/
void Handle_Road_Block(void)
{
    
}
