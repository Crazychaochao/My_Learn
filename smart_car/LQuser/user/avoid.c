#include "include.h"
#include "avoid.h"

/**-----------------------------------------------------
            选择避障左出还是右出的变量
            置1右出，0左出
-----------------------------------------------------**/

int zhili_obstacle_youchu  = 1;
int sanlun_obstacle_youchu = 1;

uint8 hinder = 0;       
int op;
char receive[6] = "";

void find_obstruct(void)
{
    
    uint8 i = 0;
    int distance = 0;
    while(UART_Get_Char(UART_3) != '\n');
    for(i = 0;i < 10;i++)
    {
        receive[i] = UART_Get_Char(UART_3);
        if(receive[i] == 'm')   break;
    }
    i = 0;
    right_huandao_flag = 1;
    while(receive[i] != 'm')
    {
        distance = distance * 10 + (receive[i] - 48);                                    //-48是因为接收到的是字符串，‘0’ = 48
        i++;
    }
    op = distance;
    if(distance <= 1200)         hinder = 1;             //120cm
    if(distance <= 1000)         hinder = 2;             //100cm
    if(distance <= 800)          hinder = 3;             //80cm
}



void Car_Stop()
{
   int i,j,k,p=0;;
   for(i=get;i<get+6;i++)
   {
     if(Imagebuf[i][47]==White)
     {
       for(j=48;j<90;j++)
       {
          if(Imagebuf[i][j]!=White)
          {
             k++;
          }
       }
       for(j=46;j>1;j--)
       {
          if(Imagebuf[i][j]!=White)
          {
             k++;
          }
       }
     }
     if(Imagebuf[i][47]==Black)
     {
       for(j=48;j<90;j++)
       {
          if(Imagebuf[i][j]!=Black)
          {
             k++;
          }
       }
       for(j=46;j>1;j--)
       {
          if(Imagebuf[i][j]!=Black)
          {
             k++;
          }
       }
     }
     
     if(k>=10)
     {
       p++; 
       k=0;
     }
   }
   
   if(p>=3)
   {
     p=0;
     Run_Flag = 0;
   }
}
/*
int left,middle,right;
int zhili_filter()
{ 
   int i,j;
/*
   for(i=30;i>1;i--)
   {
     if(Imagebuf[i][37]==Black)
     {
       left = i;
       break;
     }    
   }
   for(i=30;i>1;i--)
   {
     if(Imagebuf[i][47]==Black)
     {
       middle = i;
       break;
     }
   }
   for(i=30;i>1;i--)
   {
     if(Imagebuf[i][57]==Black)
     {
       right = i;  
       break;
     }
   }   
   if(left<=10&&middle<=10&&right<=10)
   {
     return 1;
   }
   else
   {
    return 0;
   }
}

float zhiliobstacle()
{
   if(disconnect_flag==0)
   {
      if(zhilidistance<=950&&zhilidistance>=500&&zhili_flag==1&&zhili_obzisuo==1)
      {
         zhili_obzisuo=0;
         zhili_yi=1; 
         LED_Ctrl(LED2, ON);
      }
   }
   if(zhili_yi==1)
   {
      chabihe = 40;
      zhili_er=zhili_find_Black();     
      if(zhili_er==1)
      {        
         zhili_wu = 1;
      }
      if(zhili_wu==1)
      {  
         zhili_cnt++;        
         if(zhili_cnt>=250) //慢     //快100
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
      chabihe = -35;
      zhili_cnt++;
      if(zhili_cnt>=300)
      {
        zhili_cnt = 0;
        zhili_san = 0;
        zhili_si=1;
        LED_Ctrl(LED0, OFF);
      }
      return chabihe;
   }
   if(zhili_si==1)
   {
     if(find_White())
     {
       chabihe = 50;
       zhili_cnt++;
       if(zhili_cnt>=150)
       {
          zhili_cnt = 0;
          zhili_si=0;
       }
       return chabihe;
     }     
   }
   return chabihe;
}*/


