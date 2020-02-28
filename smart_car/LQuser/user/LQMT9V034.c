#include "include.h"

int distance;
int zhilidistance;
int zhilidistance1;
uint8 Imagebuf[60][94];         //图像二值化后的数据
uint8 Image_Send[60][94];
uint8 Image_Data[IMAGEH][IMAGEW];      //图像原始数据存放
volatile u8 Image_Use[LCDH][LCDW]; //压缩后之后用于存放屏幕显示数据
u16 Pixle[LCDH][LCDW];              //二值化后用于OLED显示的数据
uint8_t Threshold;                  //OSTU大津法计算的图像阈值
volatile u8  Line_Cont=0;          //行计数
volatile u8  Field_Over_Flag=0;    //场标识
uint8 data[IMAGEH * IMAGEW];         //图像二值化后的数据
int OFFSET0=0;      //最远处，赛道中心值综合偏移量
int OFFSET1=0;      //第二格
int OFFSET2=0;      //最近，第三格
int TXV=0;          //梯形的左高度，右高度

int8 in_right_flag = 0;
int8 chu_flag = 0;

int8  Obstacle_flag = 0;              //障碍判断
uint8 Obstacle_black = 0;
uint8 Obstacle_White = 0;
uint8 number_Black = 0;
uint8 number_White = 0;
extern uint8 broken;       //障碍

extern uint8 hinder;       //阻碍
extern uint8 roadblock;

__ramfunc void DMATransDataInit(DMA_CHn CHn,void *SADDR, void *DADDR,PTXn_e ptxn,DMA_BYTEn byten,u32 count,DMA_Count_cfg cfg) ;
__ramfunc void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val) ;

//摄像头图像采集中断处理函数 
void PORTD_IRQHandler(void) 
{  
  //行中断PTD13 
  if((PORTD_ISFR & 0x2000)) //行中断 PTD13 (1<<13) 
  { 
    PORTD_ISFR |= 0x2000;   //清除中断标识 
    if(Line_Cont > IMAGEH)  //采集结束
    { 
      Line_Cont=0; 
      return ;
    }
    DMATransDataStart(DMA_CH4,(uint32_t)(&Image_Data[Line_Cont][0]),IMAGEW); //DMA开始传输数据PTD12管脚触发
    ++Line_Cont;            //行计数
    return ;
  }
  //场中断PTD14
  if((PORTD_ISFR & 0x4000))//场中断 PTD14 (1<<14)
  {
    PORTD_ISFR |= 0x4000;  //清除中断标识    
    // 用户程序 
    Line_Cont = 0;         //行计数清零
    Field_Over_Flag=1;     //场结束标识
    return ;
  }   
} 
extern float Turn[9][4];
extern float Turn_DianCi[10][4];
extern unsigned char Fres;
uint8 Change[IMAGEH/2][IMAGEW/2];
float  ACC_y = 0;
float  ACC_x = 0;
float chabihe = 0;
uint8 huandao = 0;
long  int dirrectPID = 0;
long int  deep_y = 0;
long  int  diangan = 0;
extern float pitch,roll,yaw; 		//姿态角 
//unsigned char Fres = 0;	// 前瞻
long int keep = 0;
long int Radius_last = 0;
uint8 Flage_picture = 1;
uint8 Flage_DianCi = 0;
char Mode_Set = 0;

void Test_LQV034(void)
{  
  int16 var[5];
  while(1)
  {       
    if(Field_Over_Flag)
    { 
          change(); 
          Threshold = GetOSTU(Image_Data);  
          if(disconnect_flag==1 || Obstacle_flag == 2 || Obstacle_flag == 1)
          {
             Binaryzation_ZP(Threshold);       
          }
          else
          {
             Binaryzation_ZP(110);   //晚上小  白天大        
          }
          Sum_Error = camera();    
          //UARTSendPicture(Imagebuf);
          
          
          Line_Draw();             
          Get_Use_Image();       
          tramsform();                         
          Draw_Road();           
                 
         if(disconnect_flag == 1||Obstacle_flag == 2)     //摄像头
         {
             Fresight_Calculate();            
             Radius = Radius_PID(&Turn_PID, sanlunTurn[Fres], Sum_Error, 0);          
         }
         else                                            //电磁直立                                                            
         {        
             Fresight_Calculate_dianci();                             
             Radius = PlacePID_Control(&Turn_PID, Turn_DianCi[Fres], chabihe, 0);
         }
          
         var[0] = ad5[0];
         var[1] = ad5[1];
         var[2] = ad5[2];
         var[3] = (int32) (Attitude_Angle.Y*100);
         var[4] = Speed_Now;
         //Vcan_Sendware(var,sizeof(var)); 
        
       Field_Over_Flag= 0;         //摄像图采集完毕标志位Field_Over_Flag置0
    }
  }
}
void Line_Draw1(void)
{
  uint16 i;
  for(i=0;i<60;i++)
  //for(i=GET;i>0;i--)  
  {
        Imagebuf[i][Middle_Line[i]]=Black;//把中线写到图像数组，发送到上位机看中线
  }
  Imagebuf[5][46] = Black;
  Imagebuf[35][46] = Black;
}  

float f;

/******************************************************
   
      i,起始点
      j，结束点
      flag 1，查找黑
      flag 0，查找白
      返回值：查找到全黑线行数
*******************************************************/
uint8 one_time = 1;
void Find_Obstacle(uint8 n,uint8 m,uint8 flag)
{
    uint8 i , j;
    uint8 a = 0;
    uint8 c = 0;
    number_White = 0;
    number_Black = 0;
    if(flag == 1)
    {
        if(Obstacle_black == 0)                 //直道断路
        {
            for(j = 32;j < 62;j++)
            {
                if(Imagebuf[24][j]==Black)
                {
                    number_Black++;
                    if(Imagebuf[27][j]==White)
                    {
                        number_Black++;   
                    }
                }
                if(Imagebuf[24][j]==Black)
                {
                    number_White++;
                    if(Imagebuf[27][j]==White)
                    {
                        number_White++;   
                    }
                }
            }
            if(number_Black == 2 * (62-32))         Obstacle_black = 1;
            if(number_White == 2 * (62-32))         Obstacle_black = 1;    
            /*for(i = 0;i < 90;i++)
            {   
                if(Imagebuf[27][i]==Black)      c++;
                if(c == 90 && black_right[57] >= 89 && black_left[37] >= 5) 
                {
                    //Obstacle_black = 1;
                }
            }*/
        }
        
        if(Obstacle_black == 0 && one_time == 1)                 //直道断路
        {
            for(i = 10;i<50;i++)
            {
                if(Imagebuf[59][i]==Black)      a++;
            }
            if(a == 40)         
            {
                one_time = 0;
                Obstacle_black = 1;
            }
        }
        
    }
    if(flag == 0)
    {
        for(j = 37;j < 57;j++)                          //直道
        {
            if(Imagebuf[30][j] == White)           
            {
                number_Black++;
                if(Imagebuf[35][j] == Black)           number_Black++;   
            }
            if(Imagebuf[32][j] == White)           
            {
                number_White++;
                if(Imagebuf[37][j] == Black)           number_White++;   
            }
            
            if(number_Black == 2 * (57-37))         Obstacle_White = 1;
            if(number_White == 2 * (57-37))         Obstacle_White = 1;
        }
        if(black_right[59] < black_right[57] && black_right[57] < black_right[55] ) //右弯道
          Obstacle_White = 1;
    }
}
/*************************************************************** 
* 
* 函数名称：void change(void)
* 功能说明：数据存放 
* 参数说明： 
* 函数返回：void 
* 修改时间：2018年3月27日 
* 备 注： 隔行存放数据
***************************************************************/ 
void change(void) 
{ 
  int i = 0, j = 0; 
  for(i = 0;i < (IMAGEH/2);i++) 
  { 
    for(j = 0; j< (IMAGEW/2);j++) 
    { 
      Imagebuf[i][j] = Image_Data[2*i][2*j];
    }  
  } 
}  


int disconnect_flag = 0;

void Find_disconnect()                       
{
   if(zhili_find_Black() && zhili_flag == 1 && Obstacle_flag == 0 &&one_obstacle ==0)
   {
      Obstacle_flag = 1;  
   }
   if( Obstacle_flag == 1 && find_White() && zhili_flag == 0&&one_obstacle ==0)
   {
      Obstacle_flag = 2;
   }
   if(Obstacle_flag==2 && find_Black()  && zhili_flag == 0&&one_obstacle ==0 )
   {
      Obstacle_flag = 3;
   }
   if(Obstacle_flag ==3&& zhili_find_White()  && zhili_flag == 1&&one_obstacle ==0)
   {
      Obstacle_flag = 0;
   }
} 



void MT9V034_Init(void)
{       
  uint16_t data = 0; 
  
  //GPIO口初始化
  EXTI_Init(GPIOD,13,rising_down);   //行中断
  EXTI_Init(GPIOD,14,falling_up);    //场中断  
  GPIO_Init(GPIOD,0,GPI,0);          //八位数据输入口      
  GPIO_Init(GPIOD,1,GPI,0);
  GPIO_Init(GPIOD,2,GPI,0);
  GPIO_Init(GPIOD,3,GPI,0);
  GPIO_Init(GPIOD,4,GPI,0);
  GPIO_Init(GPIOD,5,GPI,0);
  GPIO_Init(GPIOD,6,GPI,0);
  GPIO_Init(GPIOD,7,GPI,0);
  //GPIO_Init(GPIOD,12,GPI,0);     //像素  
  
  //摄像头寄存器设置
  SCCB_Init();                     //两个地址口都拉高，为MT9V034_I2C_ADDR 
 
  if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)//读取摄像头版本寄存器 
  {     
    if(data != MT9V034_CHIP_ID)                                  //芯片ID不正确，说明没有正确读取导数据，等待      
    { 
      LCD_P6x8Str(2,1,(u8*)"V034 NG");                      //摄像头识别失败，停止运行
      while(1); 
    } 
    else                                                   //芯片ID正确
    {
      LCD_P6x8Str(2,1,(u8*)"V034 OK");
    }  
  } 
  else 
  { 
    while(1); //摄像头识别失败，停止运行
  }  


  MT9V034_SetFrameResolution(IMAGEH, IMAGEW);//设置摄像头图像4*4分频输出PCLK, 27/4 = 6.75M ,BIT4,5镜像设置:上下左右均镜像 
  
  MT9V034_SetAutoExposure(true);
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0xAC, 0x0001);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0xAD, 0x01E0);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x7F, 0x3000);           // test pattern
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0F, 0x0000);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_HDR_ENABLE_REG, 0x0101); // 0x0F bit8:1HDR,0linear; bit1:1彩色,0灰度;bit0:1HDR,0linear
  //MT9V034_WriteReg16(MT9V034_HDR_ENABLE_REG, 0x0103);     // 0x0F bit8:1HDR,0linear; bit1:1彩色,0灰度;bit0:1HDR,0linear
  //0x07 Chip Control bit2-0:0逐行扫描,1无效，2场，3单场；bit5:1立体视觉使能,bit7:1并口使能；bit8:1同步模式;bit9:1正常操作；bit15:0A/1B
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_CHIP_CONTROL, 0x0188);          //Context A  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ROW_NOISE_CORR_CTRL_REG, 0);   //0x70  0x0000 
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE_REG, 0x0303);   //0xAF  AEC/AGC A~bit0:1AE;bit1:1AG/B~bit2:1AE;bit3:1AG
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, 0x0001);     //0xAC  最小粗快门宽度   0x0001
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, 0x01E0);     //0xAD  最大醋快门宽度   0x01E0-480
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 50);             //0xAB  最大模拟增益     64
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 188*120);//0xB0  用于AEC/AGC直方图像素数目,最大44000   4096=320*240  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);     //0x1C  here is the way to regulate darkness :)    
  
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,0x03C7);//0x01C7对比度差，发白；0x03C7对比度提高 Recommended by design to improve performance in HDR mode and when frame rate is low. 建议设计以提高HDR模式的性能和帧速率
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.纠正当R0x20（9）中的全局复位时启用像素负暗偏移
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.改进列FPN
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.  
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x08, 0x03D4);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x09, 0x03E7);
  //100DB //设置曝光时间
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH1,0x01BB);        //0x08 Coarse Shutter IMAGEW 1
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH2,0x01D9);        //0x09 Coarse Shutter IMAGEW 2
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH_CONTROL,0x0064); //0x0A Coarse Shutter IMAGEW Control 
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A  
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,0x00E8);   //0x0B Coarse Shutter IMAGEW Total
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_DESIRED_BIN_REG, 37); //0xA5  图像亮度  50  1-64
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_ANALOG_GAIN,0x8010);           //0x35
  
  //80dB HDR
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x08, 0x03CA);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x09, 0x03DE);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0A, 0x0064);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0F, 0x0103);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x35, 0x8010);   

  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x03);          //0x0c  复位
  
  DMATransDataInit(DMA_CH4,(void*)&PTD_BYTE0_IN,(void*)Image_Data,PTD12,DMA_BYTE1,IMAGEW,DMA_rising_down);//初始化DMA采集  

}
void MT9V034_SetFrameResolution(uint16_t height,uint16_t width)
{
  uint16_t data = 0x0002;  //原来是0
  
  if((width*4)<=MAX_IMAGE_WIDTH && (height*4)<=MAX_IMAGE_HEIGHT)
  {
    width *= 4;
    height *= 4;
    data |= MT9V034_READ_MODE_ROW_BIN_4;
    data |= MT9V034_READ_MODE_COL_BIN_4;
  }
  else if((width*2)<=MAX_IMAGE_WIDTH && (height*2)<=MAX_IMAGE_HEIGHT)
  {
    width *= 2;
    height *= 2;
    data |= MT9V034_READ_MODE_ROW_BIN_2;
    data |= MT9V034_READ_MODE_COL_BIN_2;
  }
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x01,MT9V034_COLUMN_START_DEF);     // Column Start
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x02,MT9V034_ROW_START_DEF);        // Row Start  
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x03,MT9V034_WINDOW_HEIGHT_DEF);    // height 
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x04,MT9V034_WINDOW_WIDTH_DEF);     // width  
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x05,MT9V034_HORIZONTAL_BLANKING_MIN);   // Horizontal Blanking  809-640
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x06,MT9V034_VERTICAL_BLANKING_MIN);     // Vertical Blanking    499-480 
  
  //设置图像输出大小为120*188，不损失视野，同时满足智能车需要
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x030A);   //设置摄像头图像4*4分频输出PCLK, 27/4 = 6.75M ,BIT4,5镜像设置 
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x033A);   //设置摄像头图像4*4分频输出PCLK, 27/4 = 6.75M ,BIT4,5镜像设置:上下左右均镜像 
  
  data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);       //LQ-MT9V034 needs vertical mirror to capture correct image
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_READ_MODE, data);//更改摄像头读取模式
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_WINDOW_WIDTH,  width);
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_WINDOW_HEIGHT, height);
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_COLUMN_START, (MAX_IMAGE_WIDTH-width)/2+MT9V034_COLUMN_START_MIN);
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ROW_START, (MAX_IMAGE_HEIGHT-height)/2+MT9V034_ROW_START_MIN);
}

void MT9V034_SetAutoExposure(bool enable)
{
  uint16_t reg =0;
  SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
  if(true == enable)
  {
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
  }
  else
  {
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
  }
}
void MT9V034_SetFrameRate(uint8_t frameRate)
{
  
}

void MT9V034_Reset(void)
{
  //Reset MT9V034, but register config will not change.
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
  time_delay_ms(10);
  
  //Unlock MT9V034, allowing user to initiate register settings and readout
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_CHIP_CONTROL, 0x0188);
  
  //Reset Again.
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
  time_delay_ms(10);
}

void MT9V034_SetReservedReg(void)
{
  //Here we write some reserved registers as recommendations from Rev.G datasheet, Table.8
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x13, 0x2D2E);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x20, 0x03C7);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x24, 0x001B);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2B, 0x0003);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2F, 0x0003);
}
/***************************************************************  
* 
* 函数名称：void DMATransDataInit(DMA_CHn CHn,void *SADDR, void *DADDR,PTXn_e ptxn,DMA_BYTEn byten,u32 count,DMA_PORTx2BUFF_cfg cfg) 
* 功能说明：初始化DMA 
* 参数说明：通道编号，源地址，目的地址，触发管脚，字节数，循环次数，触发边沿 
* 函数返回：无 
* 修改时间：2018年3月27日 
* 备 注： 
***************************************************************/ 
__ramfunc void DMATransDataInit(DMA_CHn CHn,void *SADDR, void *DADDR,PTXn_e ptxn,DMA_BYTEn byten,u32 count,DMA_Count_cfg cfg) 
{ 
  u8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //计算传输字节数
  //开启时钟 
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟
  // 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) 
  DMA_SADDR(CHn) =    (u32)SADDR;                         // 设置  源地址
  DMA_DADDR(CHn) =    (u32)DADDR;                         // 设置目的地址
  DMA_SOFF(CHn)  =    0;                                  // 设置源地址不变
  DMA_DOFF(CHn)  =    BYTEs;                                  // 每次传输后，目的加BYUEs
  DMA_ATTR(CHn)  =    (0
                       | DMA_ATTR_SMOD(0x0)                // 源地址模数禁止  Source address modulo feature is disabled
                       | DMA_ATTR_SSIZE(byten)             // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
                       | DMA_ATTR_DMOD(0x0)                // 目标地址模数禁止
                       | DMA_ATTR_DSIZE(byten)             // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
                       );
  DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count); //当前主循环次数
  DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKYES_BITER(count);//起始主循环次数
  
  DMA_CR &= ~DMA_CR_EMLM_MASK;                            // CR[EMLM] = 0  disable Minor Loop Mapping
  
  DMA_NBYTES_MLNO(CHn) =   DMA_NBYTES_MLNO_NBYTES(BYTEs); // 通道每次传输字节数，这里设置为BYTEs个字节。注：值为0表示传输4GB 
  
  // 配置 DMA 传输结束后的操作 
  DMA_SLAST(CHn)      =   0;                              //调整源地址的附加值,主循环结束后恢复源地址
  DMA_DLAST_SGA(CHn)  =   0;                              //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址
  DMA_CSR(CHn)        =   (0
                           | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                           | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                          );
  // 配置 DMA 触发源 
  DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
                                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                                            | DMAMUX_CHCFG_SOURCE((ptxn >> 5) + DMA_Port_A) // 通道触发传输源:     
                                           );
  //GPIO_Init(GPIOD,12,GPI,0); //像素  
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << (ptxn>>5));                                //开启PORTx端口
  GPIO_PDDR_REG(GPIOX[(ptxn>>5)]) &= ~(1 << (ptxn & 0x1f));                        //设置端口方向为输入
  PORT_PCR_REG(PORTX[(ptxn>>5)], (ptxn & 0x1F)) = ( 0
                                                   | PORT_PCR_MUX(1)               // 复用GPIO
                                                   | PORT_PCR_IRQC(cfg & 0x03 )    // 确定触发模式
                                                   | ((cfg & 0xc0 ) >> 6)          // 开启上拉或下拉电阻，或者没有
                                                  );
  GPIO_PDDR_REG(GPIOX[(ptxn>>5)]) &= ~(1 << (ptxn && 0x1F));  
  
  //开启中断 
  DMA_EN(CHn);                                      //使能通道CHn 硬件请求
  //DMA_IRQ_EN(CHn);                                //允许DMA通道传输
} 

/*************************************************************** 

* 
* 函数名称：void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val) 
* 功能说明：设置DMA传输的目的地址和循环次数 
* 参数说明： 
* 函数返回：无
* 修改时间：2018年3月27日 
* 备 注： 
***************************************************************/ 
__ramfunc void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val) 
{ 
  DMA_DADDR(CHn) = address;                              //目的地址 
  DMA_CITER_ELINKNO(CHn) = DMA_CITER_ELINKNO_CITER(Val); //当前主循环次数 
  DMA_BITER_ELINKNO(CHn) = DMA_BITER_ELINKYES_BITER(Val);//起始主循环次数 
  //开启中断 
  DMA_EN(CHn);                                           //使能通道CHn 硬件请求
  //DMA_IRQ_EN(CHn);                                     //允许DMA通道传输
} 

// 获取需要的图像数据
__ramfunc void Get_Use_Image(void)
{
 int i = 0,j = 0,row = 0,line = 0;
  for(i=0;i<60;i++)
  {
    for(j=0;j<94;j++)
    {
      Image_Use[i][j] = Imagebuf[i][j];
    }   
  }
}
/*******************************************************************************
							   Future			
->Function:   获得OLED显示的数组数据
->Editor    :    DXC
*******************************************************************************/
void tramsform()
{
  int i = 0,j = 0;
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      //if(Image_Use[i][j] >GaveValue)//平均值阈值
      if(Image_Use[i][j] ==0xFE) //大津法阈值   数值越大，显示的内容越多，较浅的图像也能显示出来    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}

//按照均值的比例进行二值化
void Get_01_Value(void)
{
  int i = 0,j = 0;
  u8 GaveValue;
  u32 tv=0;
  char txt[16];
  
  //累加
  for(i = 0; i <LCDH; i++)
  {    
    for(j = 0; j <LCDW; j++)
    {                            
      tv+=Image_Use[i][j];   //累加  
    } 
  }
  GaveValue=tv/LCDH/LCDW;     //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100 
  sprintf(txt,"%03d:%03d",Threshold,GaveValue);//前者为大津法求得的阈值，后者为平均值  
  LCD_P6x8Str(80,1,(u8*)txt);
  //按照均值的比例进行二值化
  GaveValue=GaveValue*7/10+10;        //此处阈值设置，根据环境的光线来设定 
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      //if(Image_Use[i][j] >GaveValue)//平均值阈值
      if(Image_Use[i][j] >Threshold) //大津法阈值   数值越大，显示的内容越多，较浅的图像也能显示出来    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}
//显示图像到OLED模块
void Draw_Road(void)
{ 	 
    u8 i = 0, j = 0,temp=0;
  
  //发送帧头标志
  for(i=8;i<56;i+=8)//6*8=48行 
  {
    LCD_Set_Pos(18,i/8+1);//起始位置
    for(j=0;j<LCDW;j++)  //列数
    { 
      temp=0;
      if(Pixle[0+i][j]) temp|=1;
      if(Pixle[1+i][j]) temp|=2;
      if(Pixle[2+i][j]) temp|=4;
      if(Pixle[3+i][j]) temp|=8;
      if(Pixle[4+i][j]) temp|=0x10;
      if(Pixle[5+i][j]) temp|=0x20;
      if(Pixle[6+i][j]) temp|=0x40;
      if(Pixle[7+i][j]) temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  } 
}
//三面以上反数围绕清除噪点
void Pixle_Filter(void)
{  
  int nr; //行
  int nc; //列
  
  for(nr=1; nr<LCDH-1; nr++)
  {  	    
    for(nc=1; nc<LCDW-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc]=1;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc]=0;
      }	
    }	  
  }  
}

/***************************************************************************
*                                                                          *
*  函数名称：void Pixle_Filter_Tu_Xiang(void)                                          *
*  功能说明：清除图像噪声                                                  *
*  参数说明：无                                                            *
*  函数返回：值的大小                                                      *
*  修改时间：                                                 *
*  备    注：               *
***************************************************************************/
void Pixle_Filter_Tu_Xiang(void)
{  
  int nr; //行
  int nc; //列
  
  for(nr=1; nr<LCDH-1; nr++)
  {  	    
    for(nc=1; nc<LCDW-1; nc=nc+1)
    {
     
      if((Imagebuf[nr][nc]==White)&&(Imagebuf[nr-1][nc]+Imagebuf[nr+1][nc]+Imagebuf[nr+1][nc+1]+Imagebuf[nr-1][nc-1]<2))         
      {
        Imagebuf[nr][nc]=0;
      }	
      else if((Imagebuf[nr][nc]==0)&&(Imagebuf[nr-1][nc]+Imagebuf[nr+1][nc]+Imagebuf[nr+1][nc+1]+Imagebuf[nr-1][nc-1]>763))         
      {
        Imagebuf[nr][nc]=White;
      }	
    }	  
  } 
}
/***************************************************************************
*                                                                          *
*  函数名称：int Seek_Road(void)                                           *
*  功能说明：寻找白色区域偏差值                                            *
*  参数说明：无                                                            *
*  函数返回：值的大小                                                      *
*  修改时间：2017-07-16                                                    *
*  备    注：以中间为0，左侧减一，右侧加一，数值代表1的面积                *
*            计算区域从第一行开始到倒数第二行结束。                        *
*            如果面积为负数，数值越大说明越偏左边；                        *
*            如果面积为正数，数值越大说明越偏右边。                        *
***************************************************************************/ 
void Seek_Road(void)
{  
  int nr; //行
  int nc; //列
  int temp=0;//临时数值
  //for(nr=1; nr<MAX_ROW-1; nr++)
  temp=0;
  for(nr=8; nr<24; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET0=temp;
  temp=0;
  for(nr=24; nr<40; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET1=temp;    	
  temp=0;
  for(nr=40; nr<56; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET2=temp;   	
  return;  
}

u8 zb[48],yb[48];
void FindTiXing(void)
{
  int nr; //行
  int nc; //列     
  
  for(nr=0; nr<48; nr++)
  {  	    
    zb[nr]=0;
    yb[nr]=100;   
  }  	
  for(nr=0; nr<48; nr++)
  {  	    
    for(nc=2;nc<MAX_COL-2;nc++)
    {
      if((Pixle[nr+8][nc-1]==0)&&(Pixle[nr+8][nc]==0)&&(Pixle[nr+8][nc+1]==1)&&(Pixle[nr+8][nc+2]==1))
      {
        zb[nr]=nc;//左边沿，越来越大
      }
      if((Pixle[nr+8][nc-1]==1)&&(Pixle[nr+8][nc]==1)&&(Pixle[nr+8][nc+1]==0)&&(Pixle[nr+8][nc+2]==0))
      {
        yb[nr]=nc;//右边沿，越来越小
      }                   
    }	    
  }
  TXV=0;
  for(nr=0; nr<47; nr++)
  {  	    
    if((zb[nr]>=zb[nr+1])&&(zb[nr]>0))   TXV++;          
    if((yb[nr]<=yb[nr+1])&&(yb[nr]<100)) TXV--;          
  }  	   
  return;  
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Init(void)
*  功能说明：配置SCCB所用引脚为GPIO功能，暂时不配置数据方向
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Init(void)
{
  GPIO_Init(GPIOE, 0,GPO,0);//配置为GPIO功能
  GPIO_Init(GPIOE, 1,GPO,0);//配置为GPIO功能  
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Wait(void)
*  功能说明：SCCB等待演示
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Wait(void)
{
  uint8 i=0;
  for(i=0;i<100;i++)
  { 
    asm ("nop");
  }  
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Star(void)
*  功能说明：启动函数
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Star(void)
{
  SCL_Out;
  SDA_Out;
  SCCB_Wait();
  SDA_High;
  SCL_High; 
  SCCB_Wait();
  SDA_Low;
  SCCB_Wait();
  SCL_Low; 
  SCCB_Wait();
}
/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Stop(void)
*  功能说明：停止函数
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Stop(void)
{
  SCL_Out;
  SDA_Out;
  SCCB_Wait();
  SDA_Low;
  SCCB_Wait();
  SCL_High; 
  SCCB_Wait();
  SDA_High;
  SCCB_Wait();  
}
/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：uint8 SCCB_SendByte(uint8 Data)
*  功能说明：SCCB发送字节函数
*  参数说明：要发送的字节
*  函数返回：应答信号
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
uint8 SCCB_SendByte(uint8 Data)
{
  uint8 i;
  uint8 Ack;
  SDA_Out;
  for( i=0; i<8; i++)
  {
    if(Data & 0x80) SDA_High;
    else            SDA_Low;    
    Data <<= 1;
    SCCB_Wait();
    SCL_High;      
    SCCB_Wait();
    SCL_Low;
    SCCB_Wait();
  }
  SDA_High;
  SDA_In;
  SCCB_Wait();
  
  SCL_High;
  SCCB_Wait();
  Ack = SDA_Data;
  SCL_Low;
  SCCB_Wait();
  return Ack;
}
/*************************************************************** 

* 
* 函数名称：uint8 SCCB_ReadByte(void) 
* 功能说明：SCCB读取字节函数 
* 参数说明： 
* 函数返回：读取字节 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
uint8 SCCB_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t Byte; 
  SCL_Out; 
  SDA_In; //使能输入
  for( i=0; i<8; i++) 
  { 
    SCL_Low;
    SCCB_Wait(); 
    SCL_High;
    SCCB_Wait();
    Byte = (Byte<<1)|(SDA_Data & 1);
  }
  SCL_Low; 
  SDA_Out;
  SCCB_Wait(); 
  return Byte; 
} 
/*************************************************************** 

* 
* 函数名称：static void SCCB_Ack(void) 
* 功能说明：IIC有回复信号 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
static void SCCB_Ack(void) 
{ 
  SCL_Out; 
  SDA_Out;
  SCL_Low;
  SDA_Low;
  SCCB_Wait();
  SCL_High;
  SCCB_Wait();
  SCL_Low;
  SCCB_Wait();
} 
/*************************************************************** 

* 
* 函数名称：static void SCCB_NAck(void) 
* 功能说明：IIC无回复信号 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
static void SCCB_NAck(void) 
{ 
  SCL_Out; 
  SDA_Out;
  SCL_Low;
  SCCB_Wait();
  SDA_High;
  SCCB_Wait();
  SCL_High;
  SCCB_Wait();
  SCL_Low;
  SCCB_Wait();
} 

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
*  功能说明：向设备写数据
*  参数说明：要发送的字节
*  函数返回：应答信号
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
{
  uint8 i;
  uint8 Ack;
  
  for( i=0; i<3; i++)
  {
    SCCB_Star();
    Ack = SCCB_SendByte(Device);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = SCCB_SendByte(Address);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = SCCB_SendByte((uint8)(Data>>8));
    Ack = SCCB_SendByte((uint8)Data);
    if( Ack == 1 )
    {
      continue;
    }
    
    SCCB_Stop();
    if( Ack == 0 ) break;
  }
}
/*************************************************************** 

* 
* 函数名称：uint8_t SCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
* 功能说明：读取数据 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
uint8_t SCCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
{ 
  uint8 i; 
  uint8 Ack = 0;
  Device = Device<<1;
  SCCB_Star();
  Ack += SCCB_SendByte(Device);
  
  Ack += SCCB_SendByte(Address);
  
  SCCB_Star();
  Ack += SCCB_SendByte(Device + 1);
  
  *Data = SCCB_ReadByte();
  SCCB_Ack();
  *Data = *Data<<8;
  
  *Data += SCCB_ReadByte();
  SCCB_NAck();
  
  SCCB_Stop();
  
  return  Ack;
} 
/***************************************************************  
* 
* 函数名称：int SCCB_Probe(uint8_t chipAddr) 
* 功能说明：查询该地址的设备是否存在 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
int SCCB_Probe(uint8_t chipAddr) 
{ 
  uint8_t err;
  err = 0;
  chipAddr <<= 1;
  
  SCCB_Star();
  err = SCCB_SendByte(chipAddr);
  SCCB_Stop();
  
  return err;
}
/***************************************************************
* 
* 函数名称：Sendshanwai1  byDXC 
* 功能说明：发送图像到山外上位机，这个是发送一维数组的
* 参数说明： 
* 函数返回：void 
* 修改时间：2018年12月28日 
* 备 注： 
***************************************************************/ 
void Sendshanwai1(uint8  tmImage[120])
{
#define CMD_WARE  1
  int i = 0;
    UART_Put_Char(UART_4,CMD_WARE);
    UART_Put_Char(UART_4,~CMD_WARE);
    for(i=0;i < 120; i++) 
  { 
    
      if(tmImage[i]==0xff) 
      { 
        tmImage[i]=0xfe; //防止发送标志位 
      } 
      UART_Put_Char(UART_4,tmImage[i]); 
     
  }
    UART_Put_Char(UART_4,~CMD_WARE);
    UART_Put_Char(UART_4,CMD_WARE);
}
/***************************************************************
* 
* 函数名称：Sendshanwai2  byDXC 
* 功能说明：发送图像到山外上位机，这个是发送二维数组的
* 参数说明： 
* 函数返回：void 
* 修改时间：2018年12月28日 
* 备 注： 
***************************************************************/ 
void Sendshanwai2(uint8 tmImage[60][94])
{
#define CMD_WARE  1
    int i = 0,j=0;
    UART_Put_Char(UART_4,CMD_WARE);
    UART_Put_Char(UART_4,~CMD_WARE);
    for(i=0;i < IMAGEH; i++) 
    { 
        for(j=0;j <IMAGEW;j++) 
        { 
          if(tmImage[i][j]==0xff) 
          { 
            tmImage[i][j]=0xfe; //防止发送标志位 
          } 
          UART_Put_Char(UART_4,tmImage[i][j]); 
        } 
    }
    UART_Put_Char(UART_4,~CMD_WARE);
    UART_Put_Char(UART_4,CMD_WARE);
}

/***************************************************************
* 
* 函数名称：SendPhone 
* 功能说明：发送一维数组到手机 ，

***************************************************************/ 
void SendPhone(uint8  tmImage[100])
{
  int i = 0;
  UART_Put_Char(UART_0,0xFE); //发送帧头标志 WindowsFormsApplication1.exe
  UART_Put_Char(UART_0,0xEF); //发送帧头标志 WindowsFormsApplication1.exe
  for(i=0;i < 100; i++) 
  {      
      if(tmImage[i]==0xff) 
      { 
        tmImage[i]=0xfe; //防止发送标志位 
      } 
      UART_Put_Char(UART_4,tmImage[i]); 
     
  }
  UART_Put_Char(UART_0,0xEF); //发送帧尾标志 
  UART_Put_Char(UART_0,0xFE); //发送帧尾标志 
}

/***************************************************************
* 
* 函数名称：SendPicture 
* 功能说明：发送图像到上位机 ，不同的上位机注意修改对应的数据接收协议
* 参数说明： 
* 函数返回：void 
* 修改时间：2018年3月27日 
* 备 注： 
***************************************************************/ 
void UARTSendPicture2(uint8_t tmImage[60][94]) 
{ 
  int i = 0, j = 0; 
  UART_Put_Char(UART_4,0xFE); //发送帧头标志 WindowsFormsApplication1.exe
  UART_Put_Char(UART_4,0xEF); //发送帧头标志 WindowsFormsApplication1.exe
  for(i=0;i < IMAGEH; i++) 
  { 
    for(j=0;j <IMAGEW;j++) 
    { 
      if(tmImage[i][j]==0xff) 
      { 
        tmImage[i][j]=0xfe; //防止发送标志位 
      } 
      UART_Put_Char(UART_4,tmImage[i][j]); 
    } 
  }
  UART_Put_Char(UART_4,0xEF); //发送帧尾标志 
  UART_Put_Char(UART_4,0xFE); //发送帧尾标志 
} 

void UARTSendPicture(uint8_t tmImage[60][94]) 
{ 
  int i = 0, j = 0; 
  UART_Put_Char(UART_4,0xFF); //发送帧头标志 DEMOK上位机  
  for(i=0;i < 60; i++) 
  { 
    for(j=0;j <94;j++) 
    { 
      if(tmImage[i][j]==0xff) 
      { 
        tmImage[i][j]=0xfe; //防止发送标志位 
      } 
      UART_Put_Char(UART_4,tmImage[i][j]); 
    } 
  }
} 
//山外上位机
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    s8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    s8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

   UART_Put_Buff(UART_4, cmdf, sizeof(cmdf));    //先发送命令
   
   UART_Put_Buff(UART_4, (s8 *)imgaddr, imgsize); //再发送图像

   UART_Put_Buff(UART_4, cmdr, sizeof(cmdr));    //先发送命令
}
/*************************************************************** 
* 
* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：求阈值大小 
* 参数说明： 
* 函数返回：阈值大小 
* 修改时间：2018年3月27日 
* 备 注： 
参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7）将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
***************************************************************/ 
uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图 
  
  for (j = 0; j < IMAGEH; j++) 
  { 
    for (i = 0; i < IMAGEW; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
      
  if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色    
  if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //前景像素点数
    PixelFore = Amount - PixelBack;         //背景像素点数
    OmegaBack = (double)PixelBack / Amount;//前景像素百分比
    OmegaFore = (double)PixelFore / Amount;//背景像素百分比
    PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
    MicroBack = (double)PixelIntegralBack / PixelBack;   //前景灰度百分比
    MicroFore = (double)PixelIntegralFore / PixelFore;   //背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
    if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
} 
/*************************************************************** 
* 
* 函数名称：void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：图像数据二值化 
* 参数说明： 
* 函数返回：void 
* 修改时间：2018年3月27日 
* 备 注： 
***************************************************************/ 
void Binaryzation_ZP(uint8_t ThresholdVV)
{
  int i = 0, j = 0; 
  for(i = 0;i < (IMAGEH/2);i++) 
  { 
    for(j = 0; j< (IMAGEW/2);j++) 
    { 
      if(Imagebuf[i][j] >= ThresholdVV) 
      { 
        Imagebuf[i][j] = 0xFE; 
      } 
      else 
      { 
        Imagebuf[i][j] = 0X00; 
      } 
    }
  }
} 
/*************************************************************** 
* 
* 函数名称：void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：图像数据二值化 ,更改为传指针二值化
* 修改时间：2018年12月31日  By DXC
* 备 注： 
***************************************************************/ 

void BinaryImage(u8 *dst,uint8_t ThresholdV) 
{ 
  int i = 0, j = 0; 
  for(i = 0;i < IMAGEH;i++) 
  { 
    for(j = 0; j< IMAGEW;j++) 
    { 
      if(*dst >= ThresholdV) 
      { 
        *dst = 0xFE; 
      } 
      else 
      { 
        *dst = 0X00; 
      } 
      *dst ++;
    } 
  } 
} 



