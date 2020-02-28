#ifndef _PIANCHA_H
#define _PIANCHA_H

#include "common.h"
#include "include.h"

#define byte unsigned char  //�Լ��ӵ�
#define word unsigned int   //�Լ��ӵ�



extern int8 sanlun_huandao_zhongduan;

extern uint8 huandao_chu;
extern uint16 ad2[5];
extern uint16 ad6[5];
extern float relative[5];
extern uint16 admax[5];
extern uint8 max_ad;
extern uint8 buttom;   //���������궨
extern double deviation;
extern byte scan_flag;
extern uint8 position;
extern double error[20];    //ƫ�����   error[0]
extern uint16  ADresult[5];
extern float ad5[5];    //С��/////ad5���Ϊ���ֵ
/****************���߱�־************************/
typedef struct 
{
    uint16 lose_line_flag;        //���߱�־λ
    uint16 lose_line_val;         //������ֵ
    uint16 lose_line_counter;     //���ߴ���
    uint16 turn_left_flag;        //��ת��־λ
    uint16 turn_right_flag;       //��ת��־λ
}Lose_wire;

/****************�����־************************/
typedef struct 
{
    byte  left0;
    byte  right0;
    byte  shizi;
    byte  base;     
}Deriection_flag;


/****************ʮ�ֱ�־************************/
typedef struct
{
      
  uint16 cross_fazhi_in;        //��ʮ�ֵ����25�˻���ֵ
  uint16 cross_fazhi_out_in;    //ʮ�ֵ�123456���ֵ����ֵ
  uint16 cross_fazhi_out;       //��ʮ��2*5��ֵ
  uint16 cross_counter;        //�б����ʮ�ֵ���������
  uint16 cross_flag;           //ʮ�ֵ���־λ
}Cross_info;

/****************��������************************/
typedef struct
{
  uint16 cross_road_remember;   //ʮ�ִ���  
  uint16 zhijiao_road_remember; //ֱ�Ǵ���
  uint16 lose_line_remember;    //���ߴ���
  uint16 ramp_road_remember;    //�µ�����
  
}RoadRemember_info;
extern uint8 huandao_real;

extern int huan;
extern uint8 huandao_zhongduan;
extern Lose_wire   Lose_Wire; 
extern Deriection_flag   Deriection_Flag;
extern Cross_info       CrossRoad;
extern RoadRemember_info RoadRemember;
extern int jian;

extern void caiyang();              //�ܲ���
extern void piancha(void);             //��ƫ��
extern void hanshu_chuli(); 

void maxminda1();
void maxminda2();
void gether_ad_five_times();
void average_ad();
void arrange_ad();
float normalization_ad();
void matrix_ad ();                      //ԭ���ĳ�����double matrix_ad ();


//double SqrtByNewton(double x);

double SqrtByNewton(double x);

#endif
