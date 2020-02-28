/****************************************************************
                ģ��PID
ʱ�䣺2019.3.13
*****************************************************************/
#include "Mists_Pid.h"
/***************************************************************************
������˵����
������汾��V1.0
�������¡�2019.3.13
����������PID fuzzy(float e,float ec)
������ֵ������ģ��PID����
������ֵ��float e,float ec e��ec���仯��
***************************************************************************/
     float etemp,ectemp;          //����ƫ�ƫ��仯��
     float eLefttemp,ecLefttemp;
     float eRighttemp ,ecRighttemp;
     int eLeftIndex,ecLeftIndex;
     int eRightIndex,ecRightIndex;
Mists_KID fuzzy(float e,float ec)
{

     Mists_KID      fuzzy_PID;
     //�ж�ƫ���������
     etemp = e > 60.0 ? 0.0 : (e < - 60 ? 0.0 : (e >= 0.0 ? (e >= 40 ? 50: (e >= 20 ? 30 : 10)) : (e >= -20 ? -10 : (e >= -40 ? -30 : (e >= -60 ? -50 : 0.0) ))));
 
     eLeftIndex = (int)e;
     eRightIndex = eLeftIndex;
     eLeftIndex = (int)((etemp-10) + 60);        //[-3,3] -> [2.5,5.5]
     eRightIndex = (int)((etemp+10) + 60);       //[-3,3] -> [3.5,6.5]
 
     eLefttemp =etemp == 0.0 ? 0.0:((etemp+10)-e);
     eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-10));
     //�ж�ƫ��仯�ʵ�������
     ectemp = ec > 60 ? 0.0 : (ec < - 60 ? 0.0 : (ec >= 0.0 ? (ec >= 40 ? 50: (ec >= 20 ? 30 : 10)) : (ec >= -20 ? -10 : (ec >= -40 ? -30 : (ec >= -60 ? -50 : 0.0) ))));
 
     ecLeftIndex = (int)((ectemp-10) + 60);        //[-3,3] -> [2.5,5.5]
     ecRightIndex = (int)((ectemp+10) + 60);       //[-3,3] -> [3.5,6.5]
 
     ecLefttemp = ectemp == 0.0 ? 0.0:((ectemp+10)-ec);
     ecRighttemp = ectemp == 0.0 ? 0.0:( ec-(ectemp-10));
 
/*************************************��ģ��*************************************/

    fuzzy_PID.Kp = (eLefttemp * ecLefttemp *  Fuzzy_Kp[eLeftIndex][ecLeftIndex]                    
                                    + eLefttemp * ecRighttemp * Fuzzy_Kp[eRightIndex][ecLeftIndex]
                                    + eRighttemp * ecLefttemp * Fuzzy_Kp[eLeftIndex][ecRightIndex]
                                    + eRighttemp * ecRighttemp * Fuzzy_Kp[eRightIndex][ecRightIndex]);

    fuzzy_PID.Ki =   (eLefttemp * ecLefttemp * Fuzzy_Ki[eLeftIndex][ecLeftIndex]
                                    + eLefttemp * ecRighttemp * Fuzzy_Ki[eRightIndex][ecLeftIndex]
                                    + eRighttemp * ecLefttemp * Fuzzy_Ki[eLeftIndex][ecRightIndex]
                                    + eRighttemp * ecRighttemp * Fuzzy_Ki[eRightIndex][ecRightIndex]);

    fuzzy_PID.Kd = (eLefttemp * ecLefttemp *    Fuzzy_Kd[eLeftIndex][ecLeftIndex]
                                    + eLefttemp * ecRighttemp * Fuzzy_Kd[eRightIndex][ecLeftIndex]
                                    + eRighttemp * ecLefttemp * Fuzzy_Kd[eLeftIndex][ecRightIndex]
                                    + eRighttemp * ecRighttemp * Fuzzy_Kd[eRightIndex][ecRightIndex]);
    return fuzzy_PID;
 
}
    Mists_KID OUT = {0, 0, 0};
int16 v[5];
float speed_pid(float cur,float tar)
{
    float e;
    float ec;
    float result = 0;
    static Mists_KID pid= {13, 5, 0};      //�����ֵkp��ki��kd
    static float sumE = 0;                   //�ۼ�ƫ��
    static float lastE = 0;


    e = tar - cur;             //Ŀ��ֵ - ʵ��ֵ
    ec = e - lastE;            //���仯��
    sumE += e;
    lastE = e;
    OUT = fuzzy(e, ec);      //ģ�����Ƶ���  kp��ki��kd
    result = (pid.Kp+OUT.Kp)*e + (pid.Kd+OUT.Kd)*ec;
    //range_protect(result,-400,400);
    v[0] = (int16) (e*10);
    v[1] = (int16) (ec*10);
    v[2] = (int16) (OUT.Kp*100);
    v[3] = (int16) (OUT.Kd*100);
    v[4] = (int16) (result);
    //Vcan_Sendware(v,sizeof(v));
    return result;

}

int32 MoHu_PID_Control(Mists_PID *sprt, Mists_KID *PID, int32 NowPiont, int32 SetPoint)
{
	//����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		  Actual;	//���ó���ʵ�����ֵ
        static Mists_KID OUT= {0, 0, 0};      //�����ֵkp��ki��kd
        iError = SetPoint - NowPiont;	//���㵱ǰ���,�趨ֵ-����ֵ
        OUT = fuzzy(iError, (sprt->LastError-iError));      //ģ�����Ƶ���  kp��ki��kd

	sprt->SumError += iError;  //ƫ�����
	
        Actual = (PID->Kp + OUT.Kp) * iError + (PID->Kd + OUT.Kd) * (sprt->LastError-iError) + (PID->Ki + OUT.Ki) * sprt->SumError;
	sprt->LastError = iError;		//�����ϴ����
	Actual = range_protect(Actual, -800, 800);      //�޷�����������ռ�ձ�
	return Actual;                                  //�õ�ʵ�����ֵ������ռ�ձ�
}