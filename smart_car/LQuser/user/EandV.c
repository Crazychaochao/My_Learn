#include "EandV.h"
/*
float Direct[4]  = {0.4, 0.0, 0.163, 0};	// ת��PID λ��
float MOTOR[4]   = {25,0,2,0};		// �ٶȻ�PID ���һ��Ϊ�����޷�
float Angle[4]   = {0.3, 0, 0.1, 500};		// �ǶȻ�PID 
float Ang_Vel[4] = {3.6, 0.108, 0.0027, 1000};
*/
float sanlunDirect[4]  = {0.05, 0.0, 0.118, 0};	// ת��PID λ�� 0.35, 0.0, 0.118, 0
float sanlunAngle[4]   = {0.2, 0, 0.1, 500};
float sanlunMOTOR[4]   = {10,0,2,0};	

float Direct[4]  = {0.05,0.0,0.118, 0};	// ת��PID λ��
float MOTOR[4]   = {60,0,5,0};		// �ٶȻ�PID ���һ��Ϊ�����޷�

float Angle[4] = {0.3, 0.0, 0.15, 0};
float Ang_Vel[4] = {1.5, 0.108, 0.0027, 1000};

float MOTOR_sanlun[4]   = {20,0,2,0};		// �ٶȻ�PID ���һ��Ϊ�����޷�
float Ang_Vel_sanlun[4] = {1.5, 0.105, 0.0035, 1000}; 


char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, DMP_Flag;

PID MOTOR_PID, Angle_PID, Ang_Vel_PID, Direct_PID, Turn_PID, Distance_PID;//����PID���Ƕ�pid�����ٶ�pid������pid��ת��pid��λ��pid
float sanlunTurn[9][4] ={{8, 0, 0.0, 0},	        // 130	// ת���⻷��̬PID	�����
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 150
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0},
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0}};   // 170��

float Turn[9][4] = {{8, 0, 0.0, 0},	        // 130	// ת���⻷��̬PID	�����
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 150
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0},
                    {8, 0, 0.0, 0},	// 140
                    {8, 0, 0.0, 0},	// 160
                    {8, 0, 0.0, 0}};   // 170��

float Turn_picture[5][4] = {{100, 7, 7, 400},	// 130	// ת���⻷��̬PID	
                            {100, 8, 8, 400},	// 140
                            {100, 8, 8, 400},	// 150
                            {100, 9, 9, 400},	// 160
                            {100, 10, 10, 400}};// 170

/*float Turn_DianCi[10][4] = {{100, 5, 45, 400},	// 130	// ת���⻷��̬PID	���߷�
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
float Turn_DianCi[10][4] = {{100, 5, 50, 400},	// 130	// ת���⻷��̬PID	���߷�
                            {100, 5, 50, 400},	// 140
                            {100, 5, 50, 400},	// 150
                            {100, 5, 50, 400},	// 160
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400},
                            {100, 5, 50, 400}};     // 170

#define Middle_Angle     0   //��е��ֵ, ���ص������й� 1000
int32 MOTOR_Duty_Left  = 0;
int32 MOTOR_Duty_Right = 0;
S_FLOAT_XYZ 
      GYRO_Real,		// ������ת���������
      ACC_Real,		        // ���ٶȼ�ת���������
      Attitude_Angle,	        // ��ǰ�Ƕ� 
      Last_Angle,		// �ϴνǶ�
      Target_Angle;	        // Ŀ��Ƕ�
S_INT16_XYZ
      GYRO,			// ������ԭʼ����
      GYRO_Offset,	        // ��������Ʈ
      GYRO_Last,		// �������ϴ�����
      ACC, 			// ���ٶȼ�����
      ACC_Offset,		// ���ٶȼ���Ʈ
      ACC_Last;		        // ���ٶȼ��ϴ�����
S_INT32_XYZ
      Tar_Ang_Vel,	        // Ŀ����ٶ�
      Tar_Ang_Vel_Last; 	// �ϴ�Ŀ����ٶ�

int32 
      Speed_Now,		// ��ǰʵ���ٶ�
      Speed_Min,		// ������С�ٶ�
      Speed_Set, 		// Ŀ���趨�ٶ�
      Theory_Duty = 0,          // ����ֱ��ռ�ձ�
      Vel_Set,		        // Ŀ��ת����ٶ�
      Direct_Parameter = 0,         //ת��Ĳ���
      Direct_Last = 0,          //��һ�ε�ת��
      Radius;                   //�뾶
int32 duty_a = 0;               //�������ֱ��
int32 Direct_Parameter_Last = 0;
int32 Difference = 0;
uint8_t System_OK = 0;
uint8 Run_Flag = 0;      //С���ܵı�־λ       
extern uint8 Stop_Flag = 0;     //С��ͣ�ı�־λ 
uint8 Run_Stop = 1;
char Left_Crazy = 0;	// �����ת
char Right_Crazy = 0;	// �����ת
int32 RC_Get = 1500;	// ң������ͨ��ֵ
int32 MOTOR_Speed_Left = 0;
int32 MOTOR_Speed_Right = 0; 
int32 MOTOR_Speed_Left_Last = 0;
int32 MOTOR_Speed_Right_Last = 0;
int32 MOTOR_Left_Acc = 0;
int32 MOTOR_Right_Acc = 0;
int16 a[4];
uint32 Annulus_Error = 0;	// ��·���п���
int zhili_flag = 0;
extern long  int dirrectPID;
extern long  int diangan ;
 int32 MOTOR_Duty_L  = 0;
 int32 MOTOR_Duty_R = 0;

/******************************����ƽ�����*****************************************/
void Balance_Control(void)
{
    float Accel_Angle;
    //ֱ�����ٶȻ����
    if (Ang_Velocity_Flag)	// ֱ�����ٶȻ�	2ms����pit1�ж��У���־λAng_Velocity_Flag�Ѿ���1
    {
        Ang_Velocity_Flag = 0;         
        MPU6050_GetData(&GYRO, &ACC);	
        Data_Filter();	
        
        if(disconnect_flag == 1 || Obstacle_flag == 1 || Obstacle_flag == 2)  //����״̬
        {
              if(Attitude_Angle.Y >= -4)
              {
                  Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// ����ֱ��PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -1500, 1500);  
              }   
              if(Attitude_Angle.Y < -4 && Attitude_Angle.Y >= -5)
              {
                  Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// ����ֱ��PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -2000, 2000);            //���������ռ�ձ����60%����С-60% 
              }
              if(Attitude_Angle.Y < -5 && Attitude_Angle.Y > -6)
              {
                  Theory_Duty +=  PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// ����ֱ��PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -3000, 3000);            //���������ռ�ձ����60%����С-60% 
              }
              if(Attitude_Angle.Y < -6)
              {
                  Theory_Duty +=  PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// ����ֱ��PWM
                  Theory_Duty  = (int32) range_protect(Theory_Duty, -3500, 3500);            //���������ռ�ձ����60%����С-60% 
              }
        }
        else //ֱ��״̬       
        {
            Theory_Duty +=  1 * (PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y)));	// ����ֱ��PWM
            Theory_Duty = (int32) range_protect(Theory_Duty, -9000, 9000); 
        }
        
        if (System_OK)
        {    									
            if(disconnect_flag == 1 || Obstacle_flag == 1 || Obstacle_flag == 2)     // ����
            {
                Direct_Parameter_Last = Direct_Parameter;
                Direct_Parameter = 1 * PID_Realize(&Direct_PID, sanlunDirect, (int32)((GYRO_Real.Z)*100) ,Radius*Speed_Min);	
                Direct_Parameter = (int32) (0.7 * Direct_Parameter + 0.3 * Direct_Parameter_Last);
                Direct_Parameter = range_protect(Direct_Parameter, -3000, 3000);
            }
            else    //ֱ��
            {
                Direct_Parameter_Last = Direct_Parameter;
                Direct_Parameter = 1 * PID_Realize(&Direct_PID, Direct, (int32)((GYRO_Real.Z)*100) ,Radius*Speed_Min);	// ת�������Ҹ�
                Direct_Parameter = (int32) (0.7 * Direct_Parameter + 0.3 * Direct_Parameter_Last);
                Direct_Parameter = range_protect(Direct_Parameter, -3000, 3000);
            }
        
        }
        Direct_Last = (int32) (Direct_Last*0.3 + Direct_Parameter*0.7);	// �����ϴν��ٶȻ����,Direct_Parameter System_OKΪ1ʱ��pwmֵ
        MOTOR_Duty_Left  = (int32) (1 * (Theory_Duty + Direct_Last));	// ���ҵ������ת��ϵ����������
        MOTOR_Duty_Right = (int32) (1 * (Theory_Duty - Direct_Last));	//���������ʵ��ת��,����ת
        MOTOR_Duty_Left = (int32) range_protect(MOTOR_Duty_Left, -9000, 9000);            
        MOTOR_Duty_Right = (int32) range_protect(MOTOR_Duty_Right, -9000, 9000);           
        
        if (Run_Flag)   //��system.c����1
        {
            Test_MOTOR_Control(MOTOR_Duty_Left, MOTOR_Duty_Right);	// �ı�ռ�ձȣ��������ҵ��            
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
            else //Stop_Flag��־λ��Ϊ1ʱ
            {
                Test_MOTOR_Control(0, 0);//�ж�Ϊ��ת֮��ͣ��
            }
        }
        Accel_Angle=-atan2(ACC_Real.X,ACC_Real.Z)*180/PI;    
        KalmanFilter(Accel_Angle); 
            
    }	
    if (Angle_Flag)		// ֱ���ǶȻ� �Ƕ�10ms����һ��
    {
        Angle_Flag = 0;
        Speed_Measure();                                                                     /* �ǶȻ��ӵ����ٶȻ��ϴ������� */
        if(disconnect_flag==1)   //����
        {
            Tar_Ang_Vel.Y = 1 * (PID_Realize(&Angle_PID, sanlunAngle, (int32)(Attitude_Angle.Y*100), (int32)Target_Angle.Y));	// ���Ϊ�Ŵ�10����Ŀ����ٶ�	
            Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500);	
        }
        else                    //ֱ��
        {
            Tar_Ang_Vel.Y = 1 * (PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), (int32)Target_Angle.Y));	// ���Ϊ�Ŵ�10����Ŀ����ٶ�	
            Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500);	      
        }
        if(Attitude_Angle.Y < -9)               zhili_flag = 1;
        else                                    zhili_flag = 0;
    }
    if (Speed_Flag)	     // �ٶȻ�	
    {
      if(disconnect_flag==1 || Obstacle_flag == 1 || Obstacle_flag == 2)  //����
      {
        Speed_Flag = 0;                                                                                       
        Target_Angle.Y = -PID_Realize(&MOTOR_PID, sanlunMOTOR, Speed_Now, Speed_Set);	   
        Target_Angle.Y = range_protect((int32)Target_Angle.Y, 300, 1800);	
      }
      else                    //ֱ��
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
λ��ʽPID����
 ****************************************************/

int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	// ��ǰ���
		 Realize;	// ���ó���ʵ������

	iError = Point - NowData;	// ���㵱ǰ���
	sptr->SumError += PID[KI] * iError;	// ������
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
	sptr->PrevError = sptr->LastError;	// ����ǰ�����
	sptr->LastError = iError;		// �����ϴ����
	sptr->LastData  = NowData;		// �����ϴ�����

	return Realize;	// ����ʵ��ֵ
}

/*****************************************************
 �޷�����������ռ�ձ�
 ****************************************************/
int32 range_protect(int32 duty, int32 min, int32 max)//�޷�����
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
/******* ����ٶȲ��� ********/ 
void Speed_Measure(void)
{
      char txt[16]; 
      static int32 Speed_Last = 0;
      static int32 Crazy_Count = 0;
      //Test_AB_Pulse_Cnt();
      PulsesR =FTM_AB_Get(FTM1);     //��ȡ���������ٶȣ�������ʾ����;   
      ftm_clean(FTM1) ;   
      PulsesR = -PulsesR;
      PulsesL =FTM_AB_Get(FTM2);     //��ȡ���������ٶȣ�������ʾ����; 
      ftm_clean(FTM2) ;   
      PulsesL = PulsesL;
      /******* �ҵ���ٶ���ؿ��� ********/
  
      MOTOR_Speed_Right = PulsesR;	// �õ�����ת��
      MOTOR_Right_Acc = MOTOR_Speed_Right - MOTOR_Speed_Right_Last;	// ������ٶ�
      if (MOTOR_Right_Acc > 100)
      {
              Right_Crazy = 1;	// ��ת
      }
      if (MOTOR_Speed_Right > Speed_Set + 200)
      {
              Right_Crazy = 2;	// ��ת
      }
      if (MOTOR_Speed_Right < -350)
      {
              Right_Crazy = -1;	// ��ת
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
              MOTOR_Speed_Right_Last = MOTOR_Speed_Right;	// ���������ٶ�
      }
      else
      {
              MOTOR_Speed_Right = (int32) (MOTOR_Speed_Right*0.5 + MOTOR_Speed_Right_Last*0.5);
              MOTOR_Speed_Right_Last = MOTOR_Speed_Right;	// ���������ٶ�
      }
      /******* �ҵ���ٶ���ؿ��ƽ��� ********/
      
      /******* �����ٶ���ؿ��� ********/
      MOTOR_Speed_Left = PulsesL;	// �õ�����ת��
      MOTOR_Left_Acc = MOTOR_Speed_Left - MOTOR_Speed_Left_Last;	// �����ٶȵ���ֵ
      if (MOTOR_Left_Acc > 100)                                       //����ٶȵ���ֵ����100���ж�Ϊ��ת
      {
              Left_Crazy = 1;
      }
      if (MOTOR_Speed_Left > Speed_Set + 200)                         //���������ٶȴ��ڣ�Ŀ���ٶ�+200�����ж�Ϊ��ת
      {
              Left_Crazy = 2;
      }
      if (MOTOR_Speed_Left < -350)                                    //���������ٶ�С��-350���ж�Ϊ��ת       
      {
              Left_Crazy = -1;
      }
      
      if (Left_Crazy)                                                 //��������ת
      {
              if (MOTOR_Left_Acc <= 100)                              //�����ٶȵ�����<=100
              {
                      if (MOTOR_Speed_Left < Speed_Set + 200 && MOTOR_Speed_Left > 0)//������ٶȴ��ڣ�Ŀ���ٶ�+200���͵�����ٶȴ���0
                      {
                              Left_Crazy = 0;                                         //�򲻷�ת
                      }
              }
      }
      
      if (!Left_Crazy)                                                //����������ת
      {
              MOTOR_Speed_Left = (int32) (0.9*MOTOR_Speed_Left + 0.1*MOTOR_Speed_Left_Last);	// ��ͨ�˲�
              MOTOR_Speed_Left_Last = MOTOR_Speed_Left;	// ���������ٶ�
      }
      else
      {
              MOTOR_Speed_Left = (int32) (0.5*MOTOR_Speed_Left + 0.5*MOTOR_Speed_Left_Last);	// ��ͨ�˲�
              MOTOR_Speed_Left_Last = MOTOR_Speed_Left;	// ���������ٶ�
      }

      
      /******* �����ٶ���ؿ��ƽ��� ********/
      
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
      
      /******* �����ת���⴦�� ********/
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
                    Speed_Now = MOTOR_Speed_Right;	// ������ת��ʹ���ϴ��ٶ���Ϊ��ǰʵ���ٶ�
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
                    Speed_Now = MOTOR_Speed_Left;	// �ҵ����ת��ʹ���ϴ��ٶ���Ϊ��ǰʵ���ٶ�
            }
      }
      else
      {
              Speed_Now = (MOTOR_Speed_Left + MOTOR_Speed_Right) / 2;	// ����ȡƽ�����㳵��ʵ���ٶ�
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
                      
                      led(LED_MAX,LED_OFF);// �ر�LEDָʾ��
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
        LDuty = range_protect(LDuty, 0, MOTOR_MAX);	// �޷�����                          
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH0,0);	  	// ռ�ձ����990������
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH1,LDuty);	// ռ�ձ����990������
    }
    else
    {
        LDuty = range_protect(-LDuty, 0, MOTOR_MAX);// �޷�����
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH1,0);		// ռ�ձ����990������
        FTM_PWM_Duty(MOTOR_FTM, FTM_CH0,LDuty);	// ռ�ձ����990������
    }
    if (RDuty >= 0)
    {
          RDuty = range_protect(RDuty, 0, MOTOR_MAX);	// �޷�����
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH3,0);	  	// ռ�ձ����990������
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH2,RDuty);	// ռ�ձ����990������	
    }
    else
    {
          RDuty = range_protect(-RDuty, 0, MOTOR_MAX);// �޷�����
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH2,0);		// ռ�ձ����990������
          FTM_PWM_Duty(MOTOR_FTM, FTM_CH3,RDuty);	// ռ�ձ����990������
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
        case 2:		Speed_Set = 140;	// Ĭ�ϳ�ʼ�ٶ� 140
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

// ����ʽPID�������
//��Balance_Control()�б�����
//�������룺���ٶ�pid�����ٶȣ������ǵ�Yֵ��Ŀ����ٶ�Y
//ÿ������Increase��1
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point) 
{
	//��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		Increase;	//���ó���ʵ������

	iError = Point - NowData;	// ���㵱ǰ���

	Increase =  (int32) (PID[KP] * (iError - sptr->LastError)
			  + PID[KI] * iError
			  + PID[KD] * (iError - 2 * sptr->LastError + sptr->PrevError));
	
	sptr->PrevError = sptr->LastError;	// ����ǰ�����
	sptr->LastError = iError;		  	// �����ϴ����
	sptr->LastData  = NowData;			// �����ϴ�����
	return Increase;	// ��������
}


/* ��ʼ���õ���һЩ���� */
void Balance_Init(void)
{
	Attitude_Angle.Y = 0;           // ��ǰ�Ƕ�
	Tar_Ang_Vel.Y = 0;              // Ŀ����ٶ�
	Tar_Ang_Vel.Z = 0;
}

// PID������ʼ��
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0; 
}


int d[4];
		//��̬P
	
// λ��ʽ��̬PID����
int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
	//����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		  Actual;	//���ó���ʵ�����ֵ
	float Kp = 0;
	iError = SetPoint - NowPiont;	//���㵱ǰ���,�趨ֵ-����ֵ
	sprt->SumError += iError*0.01;  //ƫ�����
//        diangan = sprt->SumError;
	if (sprt->SumError >= PID[KT])
	{
		sprt->SumError = PID[KT];
	}
	else if (sprt->SumError <= -PID[KT])
	{
		sprt->SumError = -PID[KT];
	}
	//Kp = 0.453 * (iError*iError) / PID[KP] + PID[KI];	//Pֵ���ֵ�ɶ��κ�����ϵ���˴�P��I����PID���������Ƕ�̬PID������Ҫע�⣡����
	Kp = 0.08 * (iError*iError) / PID[KP] + PID[KI];
        //Kp = PID[KI];
        //dirrectPID =  Kp;
        if(huandao_chu == 1)
        {
            Kp = 0.005 * (iError*iError) / PID[KP] + PID[KI];
        }
	Actual = (int32) (Kp * iError
		   + PID[KD] * ((0.8*iError + 0.2*sprt->LastError) - sprt->LastError));//ֻ��PD���õ�ʵ�����ֵ
	sprt->LastError = iError;		//�����ϴ����
//	Actual += sprt->SumError*0.1;
        
	Actual = range_protect(Actual, -1000, 1000);      //�޷�����������ռ�ձ�
//        deep_y = Actual;
	return Actual;                                  //�õ�ʵ�����ֵ������ռ�ձ�
}
PID * Data;
int32 Radius_PID(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
    //����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
    int32 iError,	                //��ǰ���
             result;	              //���ó���ʵ�����ֵ
    int32 Last_result;
    int32 Increase;
    float Kp = 0,Kd;
    float proportional_range;           //������
    uint8 flag;
    
    iError = SetPoint - NowPiont;	//���㵱ǰ���,�趨ֵ-����ֵ
    sprt->SumError += iError*0.001;  //ƫ�����
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
	//	   + PID[KD] * ((0.8*iError + 0.2*sprt->LastError) - sprt->LastError) + sprt->SumError);//ֻ��PD���õ�ʵ�����ֵ
    result = (int32) (Kp * iError
		   + Kd * (iError- sprt->LastError));//ֻ��PD���õ�ʵ�����ֵ
    if(Data->PrevError > Data->LastError)       flag = 1;
    else                                        flag = 0;
    Increase = filter_1_x(result,Data->LastError,0.7,flag);             //һ�׵�ͨ��̬�˲�
    Data->PrevError = Data->LastError;
    Data->LastError = result;
    result = (int16) (result * 0.7 + sprt->LastError * 0.3);
    sprt->LastError = result;		//�����ϴ����
    result = range_protect(result, -260, 260);      //�޷�����������ռ�ձ�
    return Increase;                                  //�õ�ʵ�����ֵ������ռ�ձ�
    
}
/*��ڣ�NEW_DATA �²���ֵ
       OLD_DATA �ϴ��˲����
       k        �˲�ϵ��(0~255)(�������˲�����е�Ȩ��)
  ���ڣ�         �����˲����
 */
char filter_1(char NEW_DATA,char OLD_DATA,char k)
{
    int result;
    if(NEW_DATA<OLD_DATA)
    {
        result=OLD_DATA-NEW_DATA;
        result=result*k;
        result=result+128;//+128��Ϊ����ɫ����
        result=result/256;
        result=OLD_DATA-result;
    }
    else if(NEW_DATA>OLD_DATA)
    {
        result=NEW_DATA-OLD_DATA;
        result=result*k;
        result=result+128;//+128��Ϊ����ɫ����
        result=result/256;
        result=OLD_DATA-result;
    }
    else result=OLD_DATA;
    return((char)result);
}
//��MPU6050������ݣ���x���˲�����
#define Threshold_1     3       //��ֵ1����һ�״����˲������仯�Ƕȴ��ڴ�ֵʱ����������
#define Threshold_2     8      //��ֵ2����һ�״����˲���������ֵ���ڴ�ֵʱ�������������ǿ�˲�����
float K_x=0;             //�˲�ϵ��
u8 new_flag_x=0;         //�������ݱ仯����
u8 num_x=0;              //�˲�������
/*****��ϵ���޸ĵ�һ���˲�����

��ڣ� NEW_DATA    �²����ĽǶ�ֵ
      OLD_DATA    �ϴ��˲���õĽǶȽ��
      k           �˲�ϵ��(�������˲�����е�Ȩ��)
      flag        �ϴ����ݱ仯����
���ڣ� result      �����˲��ǶȽ��
 */
int16 filter_1_x(int16 NEW_DATA,int16 OLD_DATA,float k,u8 flag)
{
    //�Ƕȱ仯����new_flag=1��ʾ�Ƕ����ӣ�=0��ʾ�Ƕ����ڼ�С
    if((NEW_DATA-OLD_DATA)>0)   new_flag_x=1;
    else if((NEW_DATA-OLD_DATA)<0)      new_flag_x=0;
    if(new_flag_x==flag)  //�˴α仯��ǰһ�α仯�����Ƿ�һ�£���ȱ�ʾ�Ƕȱ仯����һ��
    {
        num_x++;
        if(fabs((NEW_DATA-OLD_DATA))>Threshold_1)
    //���仯�Ƕȴ���Threshold_1�ȵ�ʱ�򣬽��м�����num�������ӣ��Դﵽ��������Kֵ����߸�����
            num_x+=5;                           

        if(num_x>Threshold_2)   //������ֵ���ã����Ƕȵ�����ݼ��ٶȴﵽһ������ʱ������Kֵ
        {
            K_x=k+0.1;          //0.2ΪK_x������ֵ����ʵ����Ҫ�޸�
            num_x=0;
        }
    }
    else 
    {
        num_x=0;
        K_x=0.7;     //�Ƕȱ仯�ȶ�ʱK_xֵ����ʵ���޸�
    }
    OLD_DATA=(int16) ((1-K_x)*OLD_DATA+K_x*NEW_DATA);
    return OLD_DATA;
}

void Speed(void)
{
       if(GPIO_Get(PTA5) == 1)       Mode_Set = 8;                //����2           //ѡ��Ŀ���ٶ�
       if(GPIO_Get(PTA7) == 1)       Mode_Set = 9;               // ����3
       if(GPIO_Get(PTA9) == 1)       Mode_Set = 11;              // ����4
}
void Start_Control(void)
{
	static uint8 Start_OK = 0;
	static uint8 Ready_Stop = 0;
	static uint16 No_Start_Line_Count = 0;
	
	if (Run_Flag)
	{									/** ���ܽ׶μ�� **/
            if (!Starting_Line_Flag && !Start_OK)	//û�м�⵽��������δ�ɹ�����
            {
                No_Start_Line_Count++;			//�൱����ʱԼ5s
                if (No_Start_Line_Count >= 300)	//����200��û�м�⵽�����ߣ���������ʱ�Ĺ����쳣����
                {
                        No_Start_Line_Count = 0;
                        Start_OK = 1;	//�����ɹ�
                }
            }
										/** �����׶μ�� **/
            if (Start_OK && Starting_Line_Flag)	//�ɹ����ܺ��⵽�յ���
            {
                    Ready_Stop = 1;	//׼��ͣ��
            }
            
            if (Ready_Stop)
            {
                if (!Starting_Line_Flag)	/** ��⵽ͣ���ߺ���ʱһ��ʱ��ͣ�� **/
                {
                    No_Start_Line_Count++;
                    if (No_Start_Line_Count >= 15)	//����30��û�м�⵽�����ߣ�Լ��ʱ1s
                    {
                        No_Start_Line_Count = 0;
                        if (Run_Stop)	// ����Ļ�����Ƿ�ͣ����Ĭ��ͣ��
                        {
                                Stop_Flag = 1;
                                Run_Flag = 0;			//ͣ��
                        }
                        Start_OK = 0;			//��������ɹ���־λ
                        Ready_Stop = 0;			//���׼��ͣ����־λ
                        //led(LED_MAX,LED_ON);	//�ر�LEDָʾ��
                    }
                }
            }
	}
}

/******************************************************************************
��·����
*******************************************************************************/
void Handle_Break_Way(void)
{
    
}
/******************************************************************************
·�ϴ���
*******************************************************************************/
void Handle_Road_Block(void)
{
    
}
