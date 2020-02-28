/***********************************************
    EandV.h
    Future

* �޸�ʱ�䣺2018��01��10��
* �� ע�� 
***********************************************/
#ifndef __EANDV_H
#define __EANDV_H
#include "include.h"
#include "LQFTM.h"
#define KP 0
#define KI 1
#define KD 2

#define KT 3
#define KB 4
#define KF 5
extern float Sum_Error;
extern uint8 Run_Flag;      //С���ܵı�־λ  
extern int zhili_flag;
typedef struct PID
{
	float SumError;	        //����ۼ�	
	int32 LastError;	//Error[-1]
	int32 PrevError;	//Error[-2]	
	int32 LastData;	        //Speed[-1]
}PID;
void Speed(void);
extern int32 MOTOR_Duty_Left;
extern int32 MOTOR_Duty_Right;
extern PID MOTOR_PID, Angle_PID, Ang_Vel_PID, Direct_PID, Turn_PID, Distance_PID;
extern float MOTOR[4], Angle[4], Ang_Vel[4], Direct[4],sanlunTurn[9][4], Turn[9][4],Turn_picture[5][4], Distance[4],Turn_DianCi[10][4];
extern uint8_t System_OK;
extern  int32 MOTOR_Duty_L;
extern  int32 MOTOR_Duty_R;
extern S_FLOAT_XYZ 
	GYRO_Real,		// ������ת���������
	ACC_Real,		// ���ٶȼ�ת���������
	Attitude_Angle,	// ��ǰ�Ƕ� 
	Last_Angle,		// �ϴνǶ�
	Target_Angle;	// Ŀ��Ƕ�
	

extern S_INT16_XYZ
	GYRO,			// ������ԭʼ����
	GYRO_Offset,	        // ��������Ʈ
	GYRO_Last,		// �������ϴ�����
	ACC, 			// ���ٶȼ�����
	ACC_Offset,		// ���ٶȼ���Ʈ
	ACC_Last;		// ���ٶȼ��ϴ�����
extern S_INT32_XYZ
	Tar_Ang_Vel,	// Ŀ����ٶ�
	Tar_Ang_Vel_Last;	// �ϴ�Ŀ����ٶ�

extern int32 
	Speed_Now,		// ��ǰʵ���ٶ�
	Speed_Min,		// ������С�ٶ�
	Speed_Set, 		// Ŀ���趨�ٶ�
        Theory_Duty,            // ����ֱ��ռ�ձ�
	Vel_Set,		// Ŀ��ת����ٶ�
	Direct_Parameter,
        Direct_Last, 
	Radius;
#define MOTOR_MAX   9000
#define MOTOR_FTM   FTM0
extern int32 PulsesL;
extern int32 PulsesR;
extern int32 Difference;
#define Zero_Angle 2.0f	// ��ɫ���
extern uint32 Annulus_Error;	// ��·���п���
extern int32 range_protect(int32 duty, int32 min, int32 max);//�޷�����
extern void Balance_Control(void);
int32 PID_Velocity(int encoder_left,int encoder_right);
extern int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point);
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point);
char filter_1(char NEW_DATA,char OLD_DATA,char k);
int16 filter_1_x(int16 NEW_DATA,int16 OLD_DATA,float k,u8 flag);
extern void Speed_Control(void);
extern void Speed_Measure(void);
extern void Test_MOTOR_Control(int32 LDuty, int32 RDuty);
extern void Balance_Init(void);
extern void PID_Parameter_Init(PID *sptr);
extern int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint);
int32 Radius_PID(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint);
extern void Speed_Set_6(void);
void Start_Control(void);

void Handle_Break_Way(void);
void Handle_Road_Block(void);


#endif