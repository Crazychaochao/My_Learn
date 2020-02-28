#ifndef _PIANCHA_H
#define _PIANCHA_H

#include "common.h"
#include "include.h"

#define byte unsigned char  //自己加的
#define word unsigned int   //自己加的



extern int8 sanlun_huandao_zhongduan;

extern uint8 huandao_chu;
extern uint16 ad2[5];
extern uint16 ad6[5];
extern float relative[5];
extern uint16 admax[5];
extern uint8 max_ad;
extern uint8 buttom;   //按键用来标定
extern double deviation;
extern byte scan_flag;
extern uint8 position;
extern double error[20];    //偏差队列   error[0]
extern uint16  ADresult[5];
extern float ad5[5];    //小数/////ad5电感为相对值
/****************丢线标志************************/
typedef struct 
{
    uint16 lose_line_flag;        //丢线标志位
    uint16 lose_line_val;         //丢线阈值
    uint16 lose_line_counter;     //丢线次数
    uint16 turn_left_flag;        //左转标志位
    uint16 turn_right_flag;       //右转标志位
}Lose_wire;

/****************方向标志************************/
typedef struct 
{
    byte  left0;
    byte  right0;
    byte  shizi;
    byte  base;     
}Deriection_flag;


/****************十字标志************************/
typedef struct
{
      
  uint16 cross_fazhi_in;        //进十字道电感25乘积阈值
  uint16 cross_fazhi_out_in;    //十字道123456电感值和阈值
  uint16 cross_fazhi_out;       //出十字2*5阈值
  uint16 cross_counter;        //判别进入十字道次数计数
  uint16 cross_flag;           //十字道标志位
}Cross_info;

/****************赛道记忆************************/
typedef struct
{
  uint16 cross_road_remember;   //十字次数  
  uint16 zhijiao_road_remember; //直角次数
  uint16 lose_line_remember;    //丢线次数
  uint16 ramp_road_remember;    //坡道次数
  
}RoadRemember_info;
extern uint8 huandao_real;

extern int huan;
extern uint8 huandao_zhongduan;
extern Lose_wire   Lose_Wire; 
extern Deriection_flag   Deriection_Flag;
extern Cross_info       CrossRoad;
extern RoadRemember_info RoadRemember;
extern int jian;

extern void caiyang();              //总采样
extern void piancha(void);             //总偏差
extern void hanshu_chuli(); 

void maxminda1();
void maxminda2();
void gether_ad_five_times();
void average_ad();
void arrange_ad();
float normalization_ad();
void matrix_ad ();                      //原来的程序是double matrix_ad ();


//double SqrtByNewton(double x);

double SqrtByNewton(double x);

#endif
