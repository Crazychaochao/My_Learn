/////////ƫ��///////////
#include "include.h"
#include "common.h"
#include "DianCi_BianCha.h"
#include "math.h"

uint8   buttom=1; //���������궨  
uint8   position;
/************AD��ƫ���********************/
uint16  admin[5]={0,0,0,0,0} ,admaxold[5];   
//u16  admax[5] = {656,620,592,630,641};  //���ֵ�궨
//u16  admax[5] = {656,620,592,630,641};  //���ֵ�궨
//uint16  admax[5] = {250,250,250,250,250};  //���ֵ�궨
u16  admax[5] = {800,800,800,800,800};  //���ֵ�궨
uint16  ADresult[5],ad1[5][5],ad2[5],ad3[5][5],ad4[5],ad6[5];  //�ɻ�����ad
float ad5[5];    //С��/////ad5���Ϊ���ֵ
double deviation=0.0,deviation_old=0;
uint8 max_ad;             //����еı��
uint8 lose_wire=0;
char chabihe_0 = 0;
uint8 shizi_flag=0;
extern int huandao;
long int time = 0;
long int time1 = 0;
long int time2 = 0;
long int time_flag = 0;
int time1_flag = 0;
float chabihe_last = 0;
int ad5_right_last = 0;
int ad5_left_last = 0;

int time3_flag = 0;
int ad5_last0 = 0;
int ad5_last1 = 0;
int ad5_last2 = 0;
int ad5_last3 = 0;
int huandao1 = 1;
int huandao2 = 0;

uint8 huandao_real = 0;

extern float value_step ;
extern float g_stDirectionPID_err;
//extern int deep_y,diangan;
extern float ACC_y ,ACC_x;
extern long  int dirrectPID;
extern long  int diangan ;
extern long int  deep_y ;
extern long int  keep ;

double error[20];    //ƫ�����   error[0]

float sum_13 = 0;
float sub_13 = 0;         //���13�Ĳ�
float sum_04_13 = 0;
uint16 cross_sub_val = 0;         //1*3
float distance_err_max_val = 0; //ƫ���޷�
float PARAM_CROSS;

/************Ȩ�ز���㷨********************/
float fLeftRightAdd, fLeftRightSub;
float nLeft, nRight;                            /*  ������������ֵ��*/
float Left_Num=0,Right_Num=0;                   /*  distance = A*sub_16  + B*sub_34  */
float ad6_04,ad6_13;

/************��־����********************/
Lose_wire   Lose_Wire; //���߱�־
Deriection_flag   Deriection_Flag; //�����־
Cross_info    CrossRoad; //ʮ�ֱ�־
RoadRemember_info RoadRemember; //��������

/*************�������*******************/
double aa,bb,cc,dd;  
const float matrix[4][4]=
{
  {-0.0007,   0.0013,  -0.0013, 0.0007},
  {0.0067,    -0.0067, -0.0067, 0.0067},
  {0.0167,    -0.1333, 0.1333,  -0.0167},
  {-0.1667,   0.6667,  0.6667,  -0.1667}
};//���ζ���ʽ��Ϸ�



void gether_ad_once ()    //����1��
{
  
   ADresult[0]   =   ADC0_Once(ADC0_SE10, ADC_10bit);      //�� PTE5
   ADresult[1]   =   ADC0_Once(ADC0_SE4b, ADC_10bit);      //�� 
   ADresult[2]   =   ADC0_Once(ADC0_SE9,  ADC_10bit);      //��  
   ADresult[3]   =   ADC0_Once(ADC0_SE11, ADC_10bit);      //����  PTE12 
   ADresult[4]   =   ADC0_Once(ADC0_SE7a, ADC_10bit);      //����
   if(ADresult[0] + ADresult[3] + ADresult[2] < 40) //Speed_Min > 30 && 
   {
     chabihe_0 = 1;
   }
}


#pragma inline=forced  //ǿ��inline
  inline float f_steer(float x)
  { //����f(x����ֵ
    return x*(3*aa*x+2*bb)+cc;
  }

#pragma inline=forced   //ǿ��inline
  inline float d_steer(float x)//������ֵ 
  {
    return 6*aa*x+2*bb;
  }
  #pragma inline=forced //ǿ��inline
  /////////////////////////////////////////ţ�ٷ� �Ľ�ԭ���������ʽ
  inline float point_steer(float x)
  {
  //������x�ύ���xֵ
    return (x-f_steer(x)/d_steer(x));
  }
  ////////////////////////////////////////////////////////

#pragma inline=forced   //ǿ��inline
inline float math_deal(float x)
 {
   uint8 i=20;
  //�ؼ�ѭ�����裺
    do
    {
  
    x=point_steer(x);//�õ������ֵ
    
    }
   while ((fabs(f_steer(x))>0.0001)&&(i--)); //0.0001Ϊȡֵ����  count
   
   return x;
 }



//////////////////////////////////////  ///////////////////////////
void gether_ad_five_times()    //����5�Σ������ݴ���ad1[i][5]   
{
   uint8 k,i;

     for(k=0;k<5;k++)
     {
        gether_ad_once();
        for(i=0;i<5;i++)
        {
            ad1[i][k]=ADresult[i];
        }
     }
}

////////////////////////////////////////////////////////////////////////
void average_ad()   //�Ƚϣ���С��������,ȥ�������С����ƽ��
{
    uint8 k,i,j;
    uint16 ad0_max;
    for(i=0;i<5;i++)
    {
        for(j=4;j>0;j--)
        {
            for(k=0;k<j;k++)
            {
                if(ad1[i][k]>ad1[i][k+1])
                {
                    ad0_max=ad1[i][k];
                    ad1[i][k]=ad1[i][k+1];
                    ad1[i][k+1]=ad0_max;//�����ֵ����ad1[4]
                }
            }
        }
    }
    for(i=0;i<5;i++)
    {
        ad2[i]=(ad1[i][1]+ad1[i][2]+ad1[i][3])/3;    //��ֵƽ���˲�
    }
    
}

////////////////////////////////////////////////////////////////////////
void arrange_ad()  ////////////��ʱ��ֱ�ȡ5�����ݴ�����ad3��
{
    uint8 i,k;
    for(i=0;i<5;i++)
    {
        for(k=4;k>0;k--)
        {
           ad3[i][k]=ad3[i][k-1];
        }
        ad3[i][0]=ad2[i];     //����
    }
    
}

uint16 admax_val[5]={0};
uint8 Reception[10];

int huan = 0;
uint8 huandao_zhongduan = 0;
uint8 huandao_chu = 0;
uint8 frist = 1;

/*******����**********/
int8 sanlun_huan=0;
int8 sanlun_huandao_zhongduan = 0;

float chabihe1=0;
int zhili_cnt = 0;
int zhili_yi = 0;
int zhili_er = 0;
int zhili_san=0;
int zhili_si =0;
int zhili_wu = 0;
int zhili_obzisuo = 1;
int jian = 0;
int zhili_liu=0;
int zhili_qi=0;
int qqqqq=0;
int Left_huandao=0;
int Right_huandao=0;

float normalization_ad()
{
    uint8 i,j;
    
    for(j=0; j<5; j++)
          admax_val[j] = (int) (1.2 * admax[j]);
    for(i=0;i<5;i++)
    {
         ad4[i]=(5*ad3[i][0]+4*ad3[i][1]+3*ad3[i][2]+2*ad3[i][3]+ad3[i][4])/15;//��Ȩ�����ƣ�ƽ����

         ad5[i]=(float)(((float)(ad4[i]-admin[i]))/(admax[i]-admin[i]));//��һ  0---1֮��

         if(ad4[i]>=admax_val[i])    //��궨ֵ�Ƚ�
            ad5[i]=1.0;
         if(ad4[i]<=admin[i])
            ad5[i]=0.0;
         ad5[i] = ad5[i]*100;
    }
    ACC_y = (ad5[2] - ad5[0]);
    ACC_x = (ad5[0] + ad5[2]);
    chabihe_last = (int) chabihe;
    chabihe = (ACC_y/ACC_x)*100;
    
    /*---------------ֱ������-----------------*/
    if(disconnect_flag==0)   
    {
      if(zhilidistance<=950&&zhilidistance>=600&&Attitude_Angle.Y>-22&&zhili_obzisuo==1)
      {
         zhili_obzisuo=0;
         zhili_yi=1; 
         LED_Ctrl(LED2, ON);
      }
   }
   if(zhili_yi==1)
   {
      if(zhili_obstacle_youchu==1)
      {
        chabihe = 40;
      }
      else
      {
        chabihe = -40;
      }
      zhili_er=zhili_find_Black();     
      if(zhili_er==1)
      {        
         zhili_wu = 1;
      }
      if(zhili_wu==1)
      {  
         zhili_cnt++;        
         if(zhili_cnt>=100) 
         {            
            zhili_yi =0;
            zhili_cnt=0;
            zhili_wu =0;
            zhili_san=1;
            LED_Ctrl(LED2, OFF);
         }
      }
      return chabihe;
   }
   if(zhili_san==1)
   {
      LED_Ctrl(LED0, ON);
      if(zhili_obstacle_youchu==1)
      {
        chabihe = -45;
      }
      else
      {
        chabihe = 25;
      }
      zhili_cnt++;
      if(zhili_cnt>=500)
      {
        zhili_cnt = 0;
        zhili_san = 0;
        zhili_si=1;
      }
      return chabihe;
   }
   if(zhili_si==1)
   {
     if(1)//find_White()
     {
       if(zhili_obstacle_youchu==1)
       {
         chabihe = 80;
       }
       else
       {
         chabihe = -80;
       }
       zhili_cnt++;
       if(zhili_cnt>=130)
       {
          zhili_cnt = 0;
          zhili_si=0;
       }
       return chabihe;
     }     
   } 
   /*------------------------------------------------------------*/

    
    ad5_right_last = dirrectPID;
    ad5_right_last = deep_y ;
    
    dirrectPID = ADresult[3];//�м�
    diangan = ADresult[4];//�ұ�
    
    deep_y = (int) ad5[2];
    
    ad5_last3 = (int) ad5_last2;
    ad5_last2 = (int) ad5_last1;
    ad5_last1 = (int) ad5_last0;
    ad5_last0 = (int) ad5[2];
    //if(ad5[0] + ad5[1] + ad5[2] < 5)         Run_Flag = 0; //tingche
    
    ///////////////////��������/////////////////     
    /****************************ֱ��״̬*********************************************/                                                
   if(huan == 2)
    {
        time_flag++;
    }
    if(ad5[0] >= 40 && ad5[2] > 40 && ad5[1] > 40 && zhili_flag ==1 && huan ==0)    //��ʼ��⻷�� 
    {  
        huan = 1;
        //LED_Ctrl(LED0, ON);
    }
    if(huan == 1 && (ad5[3] > 30 || ad5[4] > 30))
    {
        huan = 2;
        //LED_Ctrl(LED2, ON);
    }
    if(huan == 2 && time_flag > 200)//(ad5[0] < 15 || ad5[2] < 15))//huandao_time >500)
    {
        huan = 3;
        time_flag = 0;
        if(ad5[3] < ad5[4])
        {   
            Right_huandao = 1;
            
        }
        if(ad5[3] > ad5[4])
           Left_huandao  = 1;   
            
    }
    if(huan == 3 )// && (ad5[1] < 10 || ad5[3] < 10))
    {
      ad5[0] = ad5[3];
      ad5[2] = ad5[4];
    }
    
    if( Right_huandao == 1 && ad5[1] < 45 && ad5[2] < 55)
    {
       huan = 4; 
    }
    if(Left_huandao == 1 && ad5[2] < 45 && ad5[1] < 45)
    {
       huan = 5;
       LED_Ctrl(LED2, ON);
    }
    if(huan == 2)// || huan == 3)// && (ad5[1] < 10 || ad5[3] < 10))
    { 
     if(chabihe1 < -55)
      chabihe1 = -55;
     if(chabihe1 > 55)
      chabihe1 = 55;  
    }
   if(huan == 4 && ad5[1] < 10)  //�һ������һ�£�����
   {
      chabihe1 = -60;  //-40
   }
   if(huan == 5 && ad5[3] < 10)  //�����Ұ�һ�£�����
   {
      chabihe1 = 60;    
   }
  if(huan >= 3)
  {
      ACC_y = (ad5[2] - ad5[0]);
      ACC_x = (ad5[0] + ad5[2]);
      chabihe_last = chabihe;
      chabihe1 = (ACC_y/ACC_x)*100;
      chabihe = chabihe1;
      
  }
  if(huan == 3)
  {
      if(ad5[3] > 30 && ad5[4] > 30)
      {
          time1_flag++;
          if(time1_flag <= 100)
          chabihe = 10;
      }
  }
  if( huan >= 4 && ad5[3] < 5 && ad5[4] < 5)
  {
    huan = 0;
    Right_huandao = 0;
    Left_huandao = 0;
    LED_Ctrl(LED3, ON);
  }  
    
    /************************����ͷ��������Ͻ�����������***************************/
     
    /****************************����״̬****************************************************/
              
      /*****************�һ���********************/
    if(huandao_flag ==2)
    {
        right_huandao_flag = 1;
    }
    if(right_huandao_flag == 1)
    { 
      /*----------------------------------�������ſ��Ǹ�����----------------------*/
      if(ad5[0] > 34 && ad5[1] > 58 && ad5[2] > 79&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿2:47��
           sanlun_right_huandao_flag = 3;
           qqqqq=1;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 40 && ad5[1] > 60 && ad5[2] > 69&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿2:50��
           sanlun_right_huandao_flag = 3;
           qqqqq=2;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 40 && ad5[1] > 60 && ad5[2] > 60&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿2:57��
           sanlun_right_huandao_flag = 3;
           qqqqq=3;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 68 && ad5[1] > 60 && ad5[2] > 40&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿3:00��
           sanlun_right_huandao_flag = 3;
           qqqqq=4;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 68 && ad5[1] > 60 && ad5[2] > 40&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿3:00��
           sanlun_right_huandao_flag = 3;
           qqqqq=4;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 25 && ad5[1] > 45 && ad5[2] > 89&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿3:03��
           sanlun_right_huandao_flag = 3;
           qqqqq=5;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 30 && ad5[1] > 65 && ad5[2] > 84&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 �賿3:05��
           sanlun_right_huandao_flag = 3;
           qqqqq=6;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      /*----------------------------------�����������Ǹ�����----------------------*/
      if(ad5[0] > 17 && ad5[1] > 34 && ad5[2] > 92&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:13��
           sanlun_right_huandao_flag = 3;
           qqqqq=19;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 24 && ad5[1] > 40 && ad5[2] > 86&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:15��
           sanlun_right_huandao_flag = 3;
           qqqqq=20;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 24 && ad5[1] > 40 && ad5[2] > 86&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:15��
           sanlun_right_huandao_flag = 3;
           qqqqq=20;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 30 && ad5[1] > 50 && ad5[2] > 76&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:17��
           sanlun_right_huandao_flag = 3;
           qqqqq=21;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 39 && ad5[1] > 56 && ad5[2] > 68&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:17��
           sanlun_right_huandao_flag = 3;
           qqqqq=22;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 49 && ad5[1] > 61 && ad5[2] > 59&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:17��
           sanlun_right_huandao_flag = 3;
           qqqqq=23;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 56 && ad5[1] > 62 && ad5[2] > 50&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:23��
           sanlun_right_huandao_flag = 3;
           qqqqq=24;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 65 && ad5[1] > 62 && ad5[2] > 42&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:23��
           sanlun_right_huandao_flag = 3;
           qqqqq=25;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
      if(ad5[0] > 71 && ad5[1] > 59 && ad5[2] > 37&&diszhidao_flag==2&&lose_right_flag == 1&&lose_left_flag==0&&sanlun_left_huandao_flag!=3)          
        {
          //2019-7-6 14:26��
           sanlun_right_huandao_flag = 3;
           qqqqq=26;
             LED_Ctrl(LED0, ON);
          /*  sanlun_huandao_zhongduan = 2;
            sanlun_huan = 0;*/
        }
    }
    
 /**************************�󻷵�***************************************************/
    if(huandao_flag==1)
    {
        left_huandao_flag=1;
    }
    if(left_huandao_flag==1)
    {
      /*----------------------------------�������ſ��Ǹ�����----------------------*/
      if(ad5[0] > 65 && ad5[1] > 61 && ad5[2] > 56&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 11:01��
           sanlun_left_huandao_flag = 3;
           qqqqq=7;
             LED_Ctrl(LED0, ON);
        }
      if(ad5[0] > 80 && ad5[1] > 60 && ad5[2] > 40&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 11:11��
           sanlun_left_huandao_flag = 3;
           qqqqq=8;
             LED_Ctrl(LED0, ON);
        }
      if(ad5[0] > 86 && ad5[1] > 53 && ad5[2] > 26&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 11:13��
           sanlun_left_huandao_flag = 3;
           qqqqq=9;
             LED_Ctrl(LED0, ON);
        }
      if(ad5[0] > 56 && ad5[1] > 58 && ad5[2] > 69&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 11:15��
           sanlun_left_huandao_flag = 3;
           qqqqq=10;
             LED_Ctrl(LED0, ON);
        }
      if(ad5[0] > 48 && ad5[1] > 50 && ad5[2] > 68&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 11:16��
           sanlun_left_huandao_flag = 3;
           qqqqq=11;
             LED_Ctrl(LED0, ON);
        }
       /*----------------------------------�����������Ǹ�����----------------------*/
      if(ad5[0] > 34 && ad5[1] > 44 && ad5[2] > 78&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:35��
           sanlun_left_huandao_flag = 3;
           qqqqq=12;
             LED_Ctrl(LED0, ON);
        }
     if(ad5[0] > 46 && ad5[1] > 54 && ad5[2] > 67&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:36��
           sanlun_left_huandao_flag = 3;
           qqqqq=13;
             LED_Ctrl(LED0, ON);
        }
     if(ad5[0] > 54 && ad5[1] > 59 && ad5[2] > 57&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:38��
           sanlun_left_huandao_flag = 3;
           qqqqq=14;
             LED_Ctrl(LED0, ON);
        }
     if(ad5[0] > 61 && ad5[1] > 60 && ad5[2] > 47&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:40��
           sanlun_left_huandao_flag = 3;
           qqqqq=15;
             LED_Ctrl(LED0, ON);
        }
     if(ad5[0] > 68 && ad5[1] > 59 && ad5[2] > 40&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:40��
           sanlun_left_huandao_flag = 3;
           qqqqq=16;
             LED_Ctrl(LED0, ON);
        }
     if(ad5[0] > 80 && ad5[1] > 52 && ad5[2] > 26&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:43��
           sanlun_left_huandao_flag = 3;
           qqqqq=17;
             LED_Ctrl(LED0, ON);
        }
     if(ad5[0] > 82 && ad5[1] > 50 && ad5[2] > 23&&diszhidao_flag==2&&lose_left_flag==1&&lose_right_flag==0&&sanlun_right_huandao_flag != 3)          
        {
          //2019-7-6 13:45��
           sanlun_left_huandao_flag = 3;
           qqqqq=18;
             LED_Ctrl(LED0, ON);
        }
    }
  return 0;    
}


//////////////////////////////////////////////   ƫ���ܳ���
void piancha(void)
{
        gether_ad_five_times();  //�ɼ����  ad1
        average_ad();            //���У���ֵ�˲�   ad2
        arrange_ad();             //��ʱ��ֱ�ȡ5�����ݴ�����ad3��
        normalization_ad();          //  ��Ȩ����һ��
}

void maxminda1()   
{
    uint8 i;

    gether_ad_five_times();
    average_ad();
    arrange_ad();            //��ʱ��ֱ�ȡ5�����ݴ�����ad3�У�   δ��һ��

    for(i=0;i<5;i++)
    {
         ad4[i]=(5*ad3[i][0]+4*ad3[i][1]+3*ad3[i][2]+2*ad3[i][3]+ad3[i][4])/15;     //��Ȩ
         admax[i]=ad4[i];   //ȡһ��ֵ��Ϊ��Ӧ��ŵ�е������Сֵ    �궨
    }
}

//------------------------------------------�����ɼ�ad����admax,admin�ƽϣ��ó�admax,admin
void maxminda2()
{
    uint8 i;
    uint16 ad_middle[5];

    gether_ad_five_times();
    average_ad();
    arrange_ad();       //��ʱ��ֱ�ȡ5�����ݴ�����ad3�У�   δ��һ��

    for(i=0;i<5;i++)
    {
         ad4[i]=(5*ad3[i][0]+4*ad3[i][1]+3*ad3[i][2]+2*ad3[i][3]+ad3[i][4])/15;    //��Ȩ
         ad_middle[i]=ad4[i]; 
    }
    
    for(i=0;i<5;i++)
    {
        if(ad_middle[i]>=admax[i])   
           admax[i]=ad_middle[i];
//       if(ad_middle[i]<=admin[i])   
//           admin[i]=ad_middle[i];
    }


}




void matrix_ad (void)   //���ζ���ʽ��Ϸ�����ax3 + bx2 + cx + d = y   -10cm�� -5cm�� 5cm�� 10cm
{
//��в���     0  1   2   3  4 
    //double x,x0,x1,x2,shu1,shu3,shu5,num;
    aa=matrix[0][0]*ad6[0]+matrix[0][1]*ad6[1]+matrix[0][2]*ad6[3]+matrix[0][3]*ad6[4];
    bb=matrix[1][0]*ad6[0]+matrix[1][1]*ad6[1]+matrix[1][2]*ad6[3]+matrix[1][3]*ad6[4];
    cc=matrix[2][0]*ad6[0]+matrix[2][1]*ad6[1]+matrix[2][2]*ad6[3]+matrix[2][3]*ad6[4];
//   dd=matrix[3][0]*ad6[0]+matrix[3][1]*ad6[1]+matrix[3][2]*ad6[3]+matrix[3][3]*ad6[4];

    
}

double SqrtByNewton(double x)
{
        float eps = 0.01;
	double val=x;//��ʼֵ
	double last;
        uint8 i=200;
	do
	{
		last = val;
		val =(val + x/val) / 2;
	}while((fabs(val-last) > eps)&&(i--));
	return val;
}

