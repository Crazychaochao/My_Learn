/****************************************************************
                模糊PID
时间：2019.3.13
*****************************************************************/
#include "Mists_Pid.h"
/***************************************************************************
【功能说明】
【软件版本】V1.0
【最后更新】2019.3.13
【函数名】PID fuzzy(float e,float ec)
【返回值】返回模糊PID参数
【参数值】float e,float ec e误差，ec误差变化率
***************************************************************************/
     float etemp,ectemp;          //定义偏差、偏差变化率
     float eLefttemp,ecLefttemp;
     float eRighttemp ,ecRighttemp;
     int eLeftIndex,ecLeftIndex;
     int eRightIndex,ecRightIndex;
Mists_KID fuzzy(float e,float ec)
{

     Mists_KID      fuzzy_PID;
     //判断偏差的隶属度
     etemp = e > 60.0 ? 0.0 : (e < - 60 ? 0.0 : (e >= 0.0 ? (e >= 40 ? 50: (e >= 20 ? 30 : 10)) : (e >= -20 ? -10 : (e >= -40 ? -30 : (e >= -60 ? -50 : 0.0) ))));
 
     eLeftIndex = (int)e;
     eRightIndex = eLeftIndex;
     eLeftIndex = (int)((etemp-10) + 60);        //[-3,3] -> [2.5,5.5]
     eRightIndex = (int)((etemp+10) + 60);       //[-3,3] -> [3.5,6.5]
 
     eLefttemp =etemp == 0.0 ? 0.0:((etemp+10)-e);
     eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-10));
     //判断偏差变化率的隶属度
     ectemp = ec > 60 ? 0.0 : (ec < - 60 ? 0.0 : (ec >= 0.0 ? (ec >= 40 ? 50: (ec >= 20 ? 30 : 10)) : (ec >= -20 ? -10 : (ec >= -40 ? -30 : (ec >= -60 ? -50 : 0.0) ))));
 
     ecLeftIndex = (int)((ectemp-10) + 60);        //[-3,3] -> [2.5,5.5]
     ecRightIndex = (int)((ectemp+10) + 60);       //[-3,3] -> [3.5,6.5]
 
     ecLefttemp = ectemp == 0.0 ? 0.0:((ectemp+10)-ec);
     ecRighttemp = ectemp == 0.0 ? 0.0:( ec-(ectemp-10));
 
/*************************************反模糊*************************************/

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
    static Mists_KID pid= {13, 5, 0};      //赋予初值kp，ki，kd
    static float sumE = 0;                   //累加偏差
    static float lastE = 0;


    e = tar - cur;             //目标值 - 实际值
    ec = e - lastE;            //误差变化率
    sumE += e;
    lastE = e;
    OUT = fuzzy(e, ec);      //模糊控制调整  kp，ki，kd
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
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		  Actual;	//最后得出的实际输出值
        static Mists_KID OUT= {0, 0, 0};      //赋予初值kp，ki，kd
        iError = SetPoint - NowPiont;	//计算当前误差,设定值-测量值
        OUT = fuzzy(iError, (sprt->LastError-iError));      //模糊控制调整  kp，ki，kd

	sprt->SumError += iError;  //偏差积分
	
        Actual = (PID->Kp + OUT.Kp) * iError + (PID->Kd + OUT.Kd) * (sprt->LastError-iError) + (PID->Ki + OUT.Ki) * sprt->SumError;
	sprt->LastError = iError;		//更新上次误差
	Actual = range_protect(Actual, -800, 800);      //限幅保护，返回占空比
	return Actual;                                  //得到实际输出值，返回占空比
}