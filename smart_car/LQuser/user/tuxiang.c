/**************************************
* tuxiang.c
*By DXC
***************************************/
#include "include.h"
#include "tuxiang.h"

#define GET 33 
extern uint8 Image_Data[IMAGEH][IMAGEW]; 
int one_obstacle = 0;

struct nop
{
   int a;
   int b;
}A,B,C;

int get = 38;//开始行
int8 zuoremain_flag = 0;
int8 out_left_flag =0;
int8 out_right_flag = 0;
float  Last_Error;
float Sum_Error = 0;
float  Middle_Error; 
float End_Error = 0;

//中线误差，核心判断
int  Middle_Line[60];                       //中线数组
int black_left[60];                        //左边界数组
int black_right[60];
int lose_right[60];
int lose_left[60];
int lose_left_flag = 0;             //近处丢线判断1
int lose_right_flag = 0;

int8 find_A_flag = 0;
int8 find_B_flag = 0;

int8 remain_flag = 0;
int8 left_huandao_flag=0;
int8 in_left_flag = 0;
int8 right_huandao_flag=0;
int8 diszhidao_flag=0;             
int8 huandao_flag = 0;
int distance1=0;

/*****三轮环岛标志*****/
int8 sanlun_left_huandao_flag=0;
int8 sanlun_right_huandao_flag=0;


uint8 start_hang_zhi_li = 59;           
uint8 end_hang_zhi_li = 1;
uint8 start_hang_san_lun = 59;          
uint8 end_hang_san_lun = 1;


int find_effected()
{
   int i,left=0,middle=0,right=0,effected=0;;

   if(Imagebuf[59][23]==White)
   {
       for(i=58;i>20;i--)
       {
          if(Imagebuf[i][23]==Black)
          {
            left = i;
            break;
          }
       }
   }
   else
   {
       left=59;
   }
   
   if(Imagebuf[59][46]==White)
   {
       for(i=58;i>20;i--)
       {
          if(Imagebuf[i][46]==Black)
          {
            middle = i;
            break;
          }
       }
   }
   else
   {
       middle=59;
   }
   
   if(Imagebuf[59][69]==White)
   {
       for(i=58;i>20;i--)
       {
          if(Imagebuf[i][69]==Black)
          {
            right = i;
            break;
          }
       }
   }
   else
   {
       right=59;
   }

   if(left!=59)
   {
      effected = left;
   }
   if(middle!=59)  
   {
      if(middle<effected)
      {
          effected = middle;
      }
   }
   if(right!=59)  
   {
      if(right<effected)
      {
          effected = right;
      }
   }
   if(effected>=55)
   {
      effected=55;
   }
   if(effected<=get)
   {
      effected=get;
   }
   return effected;
}

void scan(void)
{
    int i,j,k,middle;           
    lose_left_flag = 0;
    lose_right_flag = 0;
    for(i=0;i<60;i++)        
    {
        lose_left[i]=0;        
        lose_right[i]=0;
    }
    middle = 47;
    for(i = start_hang_san_lun;i > end_hang_san_lun; i--)                               //
    {
        if(Imagebuf[i][middle] == Black)           //如果中间为黑，那么就扫描到弯道了
        {
            if(lose_left[i+1] == 1)                 //中点左移，右移
            {
                middle -= 5;
            }
            if(lose_right[i+1] == 1)
            {
                middle += 5;
            }
        }
        for(j = middle;j > 1;j--)                      //往中线左边扫描     
        {
            if(Imagebuf[i][j] == Black)            //确定左边界线     
            {
                 black_left[i] = j;
                 break;
            } 
            if(j == 2)                              //左边界找不到,该行丢线，左丢线置1
            { 
                lose_left[i] = 1;
            }
        }
        for(j=middle;j<91;j++)                     //往中线右边扫描       
        {
            if(Imagebuf[i][j] == Black)            //确定右边界线          
            {
                black_right[i] = j;
                break;
            }
            if(j == 90)
            {
                lose_right[i]= 1;                  //右边界找不到,该行丢线，右丢线置1
             }   
        }
    }                             
    int black_line = 0;
    diszhidao_flag = 1;
    for(k = 20;k > 10;k--)   //夜晚       27 20                 
    {  
      if(Imagebuf[k][47] == Black)
      {
        black_line++;
      }
    }
    if(black_line >= 3)   //不直
    {
        diszhidao_flag = 0;	
    }
    else
    {
        diszhidao_flag = 2;//直
    }  
    if(diszhidao_flag==0)
    {
      get = find_effected();
    }
    if(lose_left[get+5] + lose_left[get+4] + lose_left[get+3] + lose_left[get+2] + lose_left[get+1] >= 4)
    lose_left_flag = 1;   
    if(lose_right[get+5] + lose_right[get+3] + lose_right[get+2] + lose_right[get+1] + lose_right[get+4]>= 4)
    lose_right_flag = 1; 
    
}

int buxianwan=0;
int8 zuoyici=0;
int8 youyici=0;
float error_biancha(void)
{
    int i,k;
    int8 n = 0,m = 0;

    int zhong[5] = {get+15,get+14,get+13,get+12,get+11};//get+21,get+20,get+19,get+18,get+1  //get+11,get+10,get+9,get+8,get+7
    if(buxianwan==1)
    {
       for(i=5;i>0;i--)
       {
         zhong[i] = get+16+i;
       }     
    }  
 /*----------------------------右丢线----------------------------------*/   
    if(lose_left_flag == 0 && lose_right_flag == 1)  
    {
        k = 0;
        for(i = get + 5;i > get;i--)                        
        {
             Middle_Line[i] = black_left[i] + zhong[k]; 
             k++;	
        }
        Middle_Error = 0;
        for(i=get + 5;i > get;i--)                         
        {  
            Middle_Error += Middle_Line[i];
        }
        Middle_Error/=5;
        Middle_Error-=46;
        Sum_Error  = Middle_Error;
        Last_Error = Sum_Error;       
    } 
   /*----------------------------左丢线----------------------------------*/
    else if(lose_left_flag == 1 && lose_right_flag == 0)      //左丢线 
    {  
        k = 0;       
        for(i=get + 5;i > get;i--)                       
        {
           Middle_Line[i] = black_right[i] - zhong[k]; 
           k++;	
        } //中线滤波   误差计算
        Middle_Error = 0;
        for(i=get + 5;i > get;i--)                       //确定中线误差
        {  
           Middle_Error += Middle_Line[i];
        }
        Middle_Error/=5;
        Middle_Error-=46;
        Sum_Error = Middle_Error;
        Last_Error = Sum_Error; 
       
    }
   /*----------------------------全丢线----------------------------------*/
    else if(lose_left_flag == 1 && lose_right_flag==1 ) //全丢线      非直道，维持原判，直道，远瞻 
    {     
      Middle_Error = 0;    
      for(i = 50;i>20;i--)
      {
         if(lose_right[i] == 0 && lose_left[i] == 0)
         {
            n += 1;
            Middle_Line[i] = (black_left[i] + black_right[i])/2; 
            Middle_Error += Middle_Line[i];
         }
      } 
      Middle_Error/=n;
      Middle_Error-=46;
      Sum_Error = Middle_Error;
      Last_Error = Sum_Error;     
    }
   /*----------------------------不丢线----------------------------------*/
    else if(lose_left_flag == 0 && lose_right_flag == 0)  
    {
        k = 0;
        
        for(i=get + 5;i > get;i--)                       //确定中线
        {
             Middle_Line[i] = (black_left[i] + black_right[i])/2+1; 
             k++;	
        }	
        Middle_Error = 0;
        for(i=get + 5;i > get;i--)                       //确定中线误差
        {  
            Middle_Error += Middle_Line[i];
        }
        Middle_Error/=5;
        Middle_Error-=46;
        Sum_Error = Middle_Error;
        Last_Error = Sum_Error; 
    }
    else
    { 
        Sum_Error = Last_Error;
    }
    
    return Sum_Error;
}


//环岛处理
void Handle_Huandao(void)
{
    int i;
    if(lose_left_flag == 1 && lose_right_flag == 1 && right_huandao_flag <=1 &&left_huandao_flag<=1 &&  in_left_flag == 0 && in_right_flag == 0)
    {
        huandao_flag =0;                  //左右丢线，没有检测到左右环岛，检测到十字
        left_huandao_flag =0;
        right_huandao_flag =0;
    }
    if(huandao_flag == 1 && lose_right_flag == 0 && lose_left_flag == 1 && left_huandao_flag == 0 && in_left_flag == 0)
    {
        left_huandao_flag = 1;          //左环岛，左丢线，右不丢线，左环岛标志位为0，没进入环岛
        number_flag = 0;
    }
    if(huandao_flag == 2 && lose_right_flag == 1 && lose_left_flag == 0 && right_huandao_flag == 0 && in_right_flag==0)
    {
        right_huandao_flag = 1;         //右环岛，右丢线,左不丢线，右环岛标志位为0，没进入环岛
        number_flag = 0;
    }
    if(left_huandao_flag == 1 && lose_right_flag == 0 && lose_left_flag == 0 && diszhidao_flag == 1 && number_flag >= 10)
    {
        left_huandao_flag = 2;          //检测到环岛中段，左右同时不丢线
    }
    if(right_huandao_flag == 1 && lose_right_flag == 0 && lose_left_flag == 0 && diszhidao_flag == 1 && number_flag >= 10)
    {
        right_huandao_flag = 2;         //检测到环岛中段，左右同时不丢线
    }
    if(left_huandao_flag == 2 && lose_right_flag == 0 && lose_left_flag == 1 && diszhidao_flag == 1)
    {
        left_huandao_flag = 3;          //左环岛，左丢线，右不丢线
    }
    if(right_huandao_flag == 2 && lose_right_flag == 1 && lose_left_flag == 0 && diszhidao_flag == 1)
    {
        right_huandao_flag = 3;         //右环岛，右丢线,左不丢线
    }
    if(left_huandao_flag == 3 || right_huandao_flag == 3)               //开始补线
    {
        find_A_flag = 0;
        find_B_flag = 0;
        for(i=6;i<get+4;i++) 
        {
            if(lose_left[i] != lose_left[i+1] && lose_right[i] == 0)
            {
                find_A_flag = 1;
                A.a = i;
                A.b = black_left[i-1];
                break;
            }
            if(lose_right[i] != lose_right[i+1] && lose_left[i] == 0)
            {
                find_B_flag = 1;	
                B.a = i;
                B.b = black_right[i];
                break;
            }
        }
    }
}


int find_O_flag=1;
int find_C_flag=0;

int find_ob()
{
  int i,j,k,ob=0,w=0;
  for(i=1;i<=25;i++)
  {
     if(black_right[i]-black_left[i]<=20)
     {
       for(j=black_right[i];j>black_left[i];j--)
       {
          if(Imagebuf[i][j]==White)
          {
            w++;
          }
       }
       if(w>=12)
       {
         w=0;
         for(k=i;k<=30;k++)
         {
            if(Imagebuf[k][black_left[i]+(black_right[i]-black_left[i])/2]==White)
            {
              w++;
            }
         }
         if(w>15)
         {
            w=0;
            ob = i;
            find_O_flag = 0;
            find_C_flag = 1;
            break;
         }
       }
     }
  }

  if(find_C_flag == 1)
  {
    return ob+16;
  }
  else
    return 0;
  
}

int zhili_find_White()
{
   if(Imagebuf[55][37]==White&&Imagebuf[55][47]==White&&Imagebuf[55][57]==White)
   {
     return 1;
   }
   else
   {
     return 0;
   }
}


int find_White()
{
   if(Imagebuf[59][37]==White&&Imagebuf[59][47]==White&&Imagebuf[59][57]==White)
   {
     return 1;
   }
   else
   {
     return 0;
   }
}

int zhili_find_Black()
{
   if(Imagebuf[55][37]==Black&&Imagebuf[55][47]==Black&&Imagebuf[55][57]==Black&&Imagebuf[55][27]==Black)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

int find_Black()
{
   if(Imagebuf[59][37]==Black&&Imagebuf[59][47]==Black&&Imagebuf[59][57]==Black)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

int Round_right()
{
   int i;
   int left=0,middle=0,right=0;
   if(Imagebuf[46][37]==White)
   {
     for(i=45;i>10;i--)
     {
       if(Imagebuf[i][37]==Black) 
       { 
         left=i;
         break;
       }
     }
   }
   
   if(Imagebuf[46][47]==White)
   {
     for(i=45;i>10;i--)
     {
       if(Imagebuf[i][47]==Black) 
       { 
         middle=i;
         break;
       }
     }
   }
   
   if(Imagebuf[46][57]==White)
   {
     for(i=45;i>10;i--)
     {
       if(Imagebuf[i][57]==Black) 
       { 
         right=i;
         break;
       }
     }
   }
 
   if(left-middle<=2&&left-middle>=-2&&right-middle<=2&&right-middle>=-2&&middle!=0) 
   {
      return 1;
   }
   else
   {
     return 0;
   }
}


void Huandao()
{
  int i;
  //如果左右都丢线，就不是环岛
  if(lose_left_flag == 1 && lose_right_flag == 1 && right_huandao_flag <=1 
     && left_huandao_flag<=1&&in_right_flag == 0 && in_left_flag == 0 && remain_flag==0)
  {
    huandao_flag =0;
    left_huandao_flag  =0;
    right_huandao_flag =0;
  }
  //如果左丢线，右边不丢线，初步判断左环岛 
  if(huandao_flag == 1 && lose_right_flag == 0 && lose_left_flag == 1 
     && left_huandao_flag == 0 && in_left_flag == 0)
  {
         left_huandao_flag = 1;
  }
 
      //如果右丢线，左边不丢线，初步判断右环岛
  if(huandao_flag == 2 && lose_right_flag == 1 
     && right_huandao_flag == 0 && in_right_flag==0)
  {
         right_huandao_flag = 1;         //这里百分之一百可以到  
         LED_Ctrl(LED2, ON);
  }
  
  /*  //如果已经经历了一次丢线，图像去到了环岛中间那里，左右没丢线，右
  if(right_huandao_flag == 1 && lose_right_flag == 0 && lose_left_flag == 0)
  {
         right_huandao_flag = 2; 
         LED_Ctrl(LED0, ON);
  }  
  //如果已经经历了一次丢线，图像去到了环岛中间那里，左右没丢线，左
  if(left_huandao_flag == 1 && lose_right_flag == 0 && lose_left_flag == 0)
  {
         left_huandao_flag = 2;
  }

  //如果已经经历了前面丢线和没丢线，图像去到了环岛入口那里，左边丢线，入口
  if(left_huandao_flag == 2 && lose_right_flag == 0 && lose_left_flag == 1)
  {
         left_huandao_flag = 3;       
  }
  //如果已经经历了前面丢线和没丢线，图像去到了环岛入口那里，右边丢线，入口
  if(right_huandao_flag == 2 && lose_right_flag == 1 && lose_left_flag == 0)
  {
          right_huandao_flag = 3;          
  }
  //到了入口，开始找点
  if(left_huandao_flag == 3 || right_huandao_flag == 3)
  {
    find_A_flag = 0;
    find_B_flag = 0;
    for(i=10;i<get+4;i++) //+4
    {
      //当前行丢了，上去一行没丢，并且右边没有丢线，定义此行为A点
      if(lose_left[i] != lose_left[i+1] && lose_right[i] == 0)
      {
          find_A_flag = 1;    //这个标志不知道有没有清掉
          A.a = i;            //记录A点所在行
          A.b = black_left[i];//所在行的左边界
          break;
      }
      //当前行丢了，上去一行没丢，并且左边没有丢线，定义此行为B点
      if(lose_right[i] != lose_right[i+1] && lose_left[i] == 0&&lose_right[i+1]==1)
      {
          find_B_flag = 1;      
          B.a = i;              //记录B点所在行
          B.b = black_right[i]; //所在行的右边界
          break;
      }
    }
  }*/
}

float k;

void youreplenish()
{
  int i;

  lose_left_flag =0;
  k = (black_left[get+5]-B.b)/(B.a - get-5);//计算斜率 
  black_left[get+5] = 25;
  for(i = get+4;i>get;i--)//计算点 
  {
      black_left[i] = (int)(black_left[i+1]+(get+5-i) * 1 * k); 
  }              
  
}

void zuoreplenish()
{
  int i;

  lose_right_flag = 0;
  //计算斜率 
  k = (black_right[get-5]-A.b)/(A.a - get-5);
  //计算点 
  black_right[get+5] = 65;
  for(i = get+4;i>get;i--)
  {
      black_right[i] = black_right[i+1]+ (get+5-i) * 0.5 * k; 
  }      
}


int obstacle = 0;
int obstaclerow = 0;
int cnt = 0;
int yi=0;
int er=0;
int san=0;
int si=0;
int wu=0;
int you_out=0;
int zuo_out=0;
int clear_flag=0;
int round_zuo=0;
int youweichi=0;
int you_clear_flag=0;
float camera(void)     
{
    int i;
   // Car_Stop();
    scan();           
    find_change();
    //===============障碍==============
    if(distance1<=950&&distance1>=100&&find_O_flag&&Attitude_Angle.Y>-5)
    {
      obstaclerow = 1;
      obstacle    = find_ob(); 
      if(obstacle==0)
      {
        obstaclerow = 0; 
      }
    }   
    if(obstaclerow==1&&find_C_flag == 1)  
    {     
       one_obstacle = 1;
       if(sanlun_obstacle_youchu==1)
       {
         Sum_Error=40;
       }
       else
       {
         Sum_Error=-40; 
       }
       er = find_Black();
       if(er==1)
       {
          cnt++;
          if(cnt>=5)
          {
            cnt = 0;
            LED_Ctrl(LED2, ON);
            find_C_flag=0;
            obstaclerow = 0;
            yi = 1;
          }
       }
       return Sum_Error;
    }
    if(yi==1)
    { 
      if(sanlun_obstacle_youchu==1)
      {
         Sum_Error = -30; 
      }
      else
      {
         Sum_Error = 30; 
      }
      cnt++;
      if(cnt>=55)  //95
      {              
        cnt= 0;
        yi = 0;
        san=1;       
      }
      return Sum_Error;
    }
    if(san==1)
    { 
    
      if(find_White())
      {
         si=1;
      }
      if(si==1)
      {
        LED_Ctrl(LED0, ON);
        if(sanlun_obstacle_youchu==1)
        {
          Sum_Error =  70;
        }
        else
        {
          Sum_Error = -70;
        }
        cnt++;
        if(cnt>=10)
        {
           cnt=0;
           san=0;
           one_obstacle = 0;
           si=0;
        }
        return  Sum_Error; 
      }
    }
    
    
   //三轮环岛
    /*左边*/
    if(sanlun_left_huandao_flag==3)
    {
      for(i=10;i<get+6;i++) //+4
      {
       if(lose_left[i] != lose_left[i+1] && lose_right[i] == 0&&lose_left[i+1]==1)
       {
          find_A_flag = 1;    //这个标志不知道有没有清掉
          A.a = i;            //记录A点所在行
          A.b = black_left[i];//所在行的左边界
          sanlun_left_huandao_flag=0;
          break;
       }
      }
    }
    if(find_A_flag ==1)
    {
       zuoreplenish();
       cnt++;
       if(cnt>=25)
       {
         cnt=0;
         find_A_flag=0;
         buxianwan=1;
         huandao_flag=0;
         left_huandao_flag=0;
         round_zuo=1;
       }      
    }
    
   if(clear_flag==1&&round_zuo==2)
   {
      clear_flag=0;
      round_zuo=0;
      buxianwan=0;
      lose_left_flag=1;
      lose_right_flag=0;
      for(i=get;i<get+5;i++)
      {
          black_right[i] = 99;
      }      
   }
   if(buxianwan==1&&lose_right[45] == 1&&round_zuo==1)
    {
      zuo_out=Round_right();    
      // buxianwan=0;      
    }
   if(zuo_out>=1)
    {
       zuo_out++;
      // LED_Ctrl(LED2, ON);
       if(zuo_out>=3)
       {
         zuo_out=0;
         round_zuo=2;
         clear_flag=1;
       }
       Sum_Error=-70;
       return Sum_Error;
    } 
    
    //右环岛
    if(sanlun_right_huandao_flag==3)
    {
      for(i=10;i<get+6;i++) 
      {
        if(lose_right[i] != lose_right[i+1] && lose_left[i] == 0&&lose_right[i+1]==1)
        {
            find_B_flag = 1;      
            B.a = i;              //记录B点所在行
            B.b = black_right[i]; //所在行的右边界
            sanlun_right_huandao_flag=0;
            huandao_flag=0;
            break;
        }
      }
    }
    if(find_B_flag ==1)
    { 
       youreplenish();
       cnt++;
       if(cnt>=20)
       {
         cnt=0;
         find_B_flag=0;
         buxianwan=1;
         right_huandao_flag=0;
       }
    }
   if(you_clear_flag==1)
   {   
      buxianwan=0;
      you_clear_flag=0;
     /* Sum_Error=20;
      cnt++;
      if(cnt>=3)
      {
        cnt=0;
        LED_Ctrl(LED3, ON);
        clear_flag=0;
      }
      return Sum_Error;*/
      lose_left_flag=0;
      lose_right_flag=1;
      for(i=get;i<get+5;i++)
      {
          black_left[i] = -1;
      }      
   }
   if(buxianwan==1&&lose_left[45] == 0)
    {
      you_out=Round_right();    
      // buxianwan=0;
       
    }
   if(you_out>=1)
    {
   //  LED_Ctrl(LED2, ON);
  /*   lose_left_flag = 0;
     lose_right_flag = 1;
     for(i=get;i<get+5;i++)
     {
        black_left[i] = 120-i;
     }*/
     you_out++;
     if(you_out>=3)
     {
       you_out=0;
       you_clear_flag=1;
     }
     Sum_Error=70;
     return Sum_Error;
    }
    
    Find_disconnect();          
    error_biancha();    
    return  Sum_Error;
}

void find_change()
{ 
   int i,p=0,q=0;  
   for(i=50;i>15;i--) 
    { 
      if(lose_left[i] != lose_left[i+1] &&lose_left[i+1] == lose_left[i+2] && lose_right_flag==0&&q==0)
        {
          if((black_left[i+1] >= 7 && p == 0) || (black_left[i] >= 7 && p==1))
          {
                    p++;                    
          }
        } 
      
      if(lose_right[i] != lose_right[i+1] &&lose_right[i+1] == lose_right[i+2] && 
         lose_left[i]==0&&lose_left[i-1]==0&&lose_left[i+1]==0&&lose_left_flag==0&&p==0)
        {
          if((black_right[i+1] <=88  && q == 0) || (black_right[i] <= 88 && q==1))
           {
                    q++;                   
           }
        }        
    }
    if( p == 2 && diszhidao_flag ==2&&q==0)  
    {     
      huandao_flag = 1;//左环岛
    }
    if( q == 2 && diszhidao_flag ==2&&p==0) 
    {      
      huandao_flag = 2;
             
    }
} 



/***************************************************
*  函数名称： Line_Draw
*  功能说明：写中线进数组 
****************************************************/
void Line_Draw(void)
{
  uint16 i;
  for(i=get;i<get+5;i++)
  //for(i=GET;i>0;i--)  
  {
     Imagebuf[i][Middle_Line[i]]=Black;//把中线写到图像数组，发送到上位机看中线
  }
}  



/***************************************************
*  函数名称：dengjia
*  功能说明：数组之间赋值      By DXC 
****************************************************/
void dengjia()
{
     uint8 i,j; 
      uint8 k=0,m=0; 
      for(i = 0 ;i <IMAGEH;i+=2)
      {          
            for(j = 0;j < IMAGEW;j+=2)
            {
                  Imagebuf[k][m] = Image_Data[i][j];
                  m++;
            }
            m=0;
            k++;
      }

}
/***************************************************
*  函数名称：Two_One
*  功能说明：二位数据转一维数组
****************************************************/
void Two_One(uint8 Imagebuf[IMAGEH][IMAGEW] , uint8 data[IMAGEH * IMAGEW])
{
    int i,j;  
    for(i = 0;i < IMAGEH;i++)
    {
        for(j = 0;j < IMAGEW;j++)
        {
              *(data ++) = Image_Data[i][j];
        }
    }

}
/***************************************************
*  函数名称：Two_One
*  功能说明：二维书数组转二维维数组
****************************************************/
void Two_TWO(uint8 Imagebuf[120][188] , uint8 data[60][94])
{
    int i,j;  
    for(i = 0;i < 60;i++)
    {
        for(j = 0;j < 94;j++)
        {
             data[i][j]  = Imagebuf[i][j];
        }
    }

}


