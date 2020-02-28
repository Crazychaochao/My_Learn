#ifndef _Mists_Pid_H
#define _Mists_Pid_H
#include "include.h"

#define Mists_Kp 1
#define Mists_Ki 1
#define Mists_Kd 1

#define NB       -60
#define NM	 -30
#define NS	 -10
#define ZE	 0
#define PB	 60
#define PM	 30
#define PS	 10


static const float Fuzzy_Kp[7][7]={
	PB,	PB,	PM,	PM,	PS,	PS,	ZE,
	PB,	PB,	PM,	PM,	PS,	ZE,	ZE,
	PM,	PM,	PM,	PS,	ZE,	NS,	NM,
	PM,	PS,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	ZE,	ZE,	NS,	NM,	NM,	NM,	NB,
	ZE,	NS,	NS,	NM,	NM,	NB,	NB
};

static const float Fuzzy_Ki[7][7]={
	NB,	NB,	NB,	NM,	NM,	ZE,	ZE,
	NB,	NB,	NM,	NM,	NS,	ZE,	ZE,
	NM,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	ZE,	PS,	PS,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PM,
	ZE,	ZE,	PS,	PM,	PM,	PB,	PB,
	ZE,	ZE,	PS,	PM,	PB,	PB,	PB
};
static const float Fuzzy_Kd[7][7]={
	PS,	PS,	ZE,	ZE,	ZE,	PB,	PB,
	NS,	NS,	NS,	NS,	ZE,	NS,	PM,
	NB,	NB,	NM,	NS,	ZE,	PS,	PM,
	NB,	NM,	NM,	NS,	ZE,	PS,	PM,
	NB,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	NS,	ZE,	PS,	PS,
	PS,	ZE,	ZE,	ZE,	ZE,	PB,	PB
};

typedef struct{
	float Kp;
	float Ki;
	float Kd;
}Mists_KID;
typedef struct
{
	float SumError;	//ŒÛ≤Ó¿€º∆	
	int32 LastError;	//Error[-1]
	int32 PrevError;	//Error[-2]	
	int32 LastData;	//Speed[-1]
}Mists_PID;
Mists_KID fuzzy(float e,float ec);
float speed_pid(float cur,float tar);
int32 MoHu_PID_Control(Mists_PID *sprt, Mists_KID *PID, int32 NowPiont, int32 SetPoint);

#endif