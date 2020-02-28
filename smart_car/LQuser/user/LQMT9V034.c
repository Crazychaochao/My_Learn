#include "include.h"

int distance;
int zhilidistance;
int zhilidistance1;
uint8 Imagebuf[60][94];         //ͼ���ֵ���������
uint8 Image_Send[60][94];
uint8 Image_Data[IMAGEH][IMAGEW];      //ͼ��ԭʼ���ݴ��
volatile u8 Image_Use[LCDH][LCDW]; //ѹ����֮�����ڴ����Ļ��ʾ����
u16 Pixle[LCDH][LCDW];              //��ֵ��������OLED��ʾ������
uint8_t Threshold;                  //OSTU��򷨼����ͼ����ֵ
volatile u8  Line_Cont=0;          //�м���
volatile u8  Field_Over_Flag=0;    //����ʶ
uint8 data[IMAGEH * IMAGEW];         //ͼ���ֵ���������
int OFFSET0=0;      //��Զ������������ֵ�ۺ�ƫ����
int OFFSET1=0;      //�ڶ���
int OFFSET2=0;      //�����������
int TXV=0;          //���ε���߶ȣ��Ҹ߶�

int8 in_right_flag = 0;
int8 chu_flag = 0;

int8  Obstacle_flag = 0;              //�ϰ��ж�
uint8 Obstacle_black = 0;
uint8 Obstacle_White = 0;
uint8 number_Black = 0;
uint8 number_White = 0;
extern uint8 broken;       //�ϰ�

extern uint8 hinder;       //�谭
extern uint8 roadblock;

__ramfunc void DMATransDataInit(DMA_CHn CHn,void *SADDR, void *DADDR,PTXn_e ptxn,DMA_BYTEn byten,u32 count,DMA_Count_cfg cfg) ;
__ramfunc void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val) ;

//����ͷͼ��ɼ��жϴ����� 
void PORTD_IRQHandler(void) 
{  
  //���ж�PTD13 
  if((PORTD_ISFR & 0x2000)) //���ж� PTD13 (1<<13) 
  { 
    PORTD_ISFR |= 0x2000;   //����жϱ�ʶ 
    if(Line_Cont > IMAGEH)  //�ɼ�����
    { 
      Line_Cont=0; 
      return ;
    }
    DMATransDataStart(DMA_CH4,(uint32_t)(&Image_Data[Line_Cont][0]),IMAGEW); //DMA��ʼ��������PTD12�ܽŴ���
    ++Line_Cont;            //�м���
    return ;
  }
  //���ж�PTD14
  if((PORTD_ISFR & 0x4000))//���ж� PTD14 (1<<14)
  {
    PORTD_ISFR |= 0x4000;  //����жϱ�ʶ    
    // �û����� 
    Line_Cont = 0;         //�м�������
    Field_Over_Flag=1;     //��������ʶ
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
extern float pitch,roll,yaw; 		//��̬�� 
//unsigned char Fres = 0;	// ǰհ
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
             Binaryzation_ZP(110);   //����С  �����        
          }
          Sum_Error = camera();    
          //UARTSendPicture(Imagebuf);
          
          
          Line_Draw();             
          Get_Use_Image();       
          tramsform();                         
          Draw_Road();           
                 
         if(disconnect_flag == 1||Obstacle_flag == 2)     //����ͷ
         {
             Fresight_Calculate();            
             Radius = Radius_PID(&Turn_PID, sanlunTurn[Fres], Sum_Error, 0);          
         }
         else                                            //���ֱ��                                                            
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
        
       Field_Over_Flag= 0;         //����ͼ�ɼ���ϱ�־λField_Over_Flag��0
    }
  }
}
void Line_Draw1(void)
{
  uint16 i;
  for(i=0;i<60;i++)
  //for(i=GET;i>0;i--)  
  {
        Imagebuf[i][Middle_Line[i]]=Black;//������д��ͼ�����飬���͵���λ��������
  }
  Imagebuf[5][46] = Black;
  Imagebuf[35][46] = Black;
}  

float f;

/******************************************************
   
      i,��ʼ��
      j��������
      flag 1�����Һ�
      flag 0�����Ұ�
      ����ֵ�����ҵ�ȫ��������
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
        if(Obstacle_black == 0)                 //ֱ����·
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
        
        if(Obstacle_black == 0 && one_time == 1)                 //ֱ����·
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
        for(j = 37;j < 57;j++)                          //ֱ��
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
        if(black_right[59] < black_right[57] && black_right[57] < black_right[55] ) //�����
          Obstacle_White = 1;
    }
}
/*************************************************************** 
* 
* �������ƣ�void change(void)
* ����˵�������ݴ�� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� ���д������
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
  
  //GPIO�ڳ�ʼ��
  EXTI_Init(GPIOD,13,rising_down);   //���ж�
  EXTI_Init(GPIOD,14,falling_up);    //���ж�  
  GPIO_Init(GPIOD,0,GPI,0);          //��λ���������      
  GPIO_Init(GPIOD,1,GPI,0);
  GPIO_Init(GPIOD,2,GPI,0);
  GPIO_Init(GPIOD,3,GPI,0);
  GPIO_Init(GPIOD,4,GPI,0);
  GPIO_Init(GPIOD,5,GPI,0);
  GPIO_Init(GPIOD,6,GPI,0);
  GPIO_Init(GPIOD,7,GPI,0);
  //GPIO_Init(GPIOD,12,GPI,0);     //����  
  
  //����ͷ�Ĵ�������
  SCCB_Init();                     //������ַ�ڶ����ߣ�ΪMT9V034_I2C_ADDR 
 
  if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)//��ȡ����ͷ�汾�Ĵ��� 
  {     
    if(data != MT9V034_CHIP_ID)                                  //оƬID����ȷ��˵��û����ȷ��ȡ�����ݣ��ȴ�      
    { 
      LCD_P6x8Str(2,1,(u8*)"V034 NG");                      //����ͷʶ��ʧ�ܣ�ֹͣ����
      while(1); 
    } 
    else                                                   //оƬID��ȷ
    {
      LCD_P6x8Str(2,1,(u8*)"V034 OK");
    }  
  } 
  else 
  { 
    while(1); //����ͷʶ��ʧ�ܣ�ֹͣ����
  }  


  MT9V034_SetFrameResolution(IMAGEH, IMAGEW);//��������ͷͼ��4*4��Ƶ���PCLK, 27/4 = 6.75M ,BIT4,5��������:�������Ҿ����� 
  
  MT9V034_SetAutoExposure(true);
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0xAC, 0x0001);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0xAD, 0x01E0);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x7F, 0x3000);           // test pattern
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0F, 0x0000);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_HDR_ENABLE_REG, 0x0101); // 0x0F bit8:1HDR,0linear; bit1:1��ɫ,0�Ҷ�;bit0:1HDR,0linear
  //MT9V034_WriteReg16(MT9V034_HDR_ENABLE_REG, 0x0103);     // 0x0F bit8:1HDR,0linear; bit1:1��ɫ,0�Ҷ�;bit0:1HDR,0linear
  //0x07 Chip Control bit2-0:0����ɨ��,1��Ч��2����3������bit5:1�����Ӿ�ʹ��,bit7:1����ʹ�ܣ�bit8:1ͬ��ģʽ;bit9:1����������bit15:0A/1B
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_CHIP_CONTROL, 0x0188);          //Context A  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ROW_NOISE_CORR_CTRL_REG, 0);   //0x70  0x0000 
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE_REG, 0x0303);   //0xAF  AEC/AGC A~bit0:1AE;bit1:1AG/B~bit2:1AE;bit3:1AG
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, 0x0001);     //0xAC  ��С�ֿ��ſ��   0x0001
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, 0x01E0);     //0xAD  ���׿��ſ��   0x01E0-480
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 50);             //0xAB  ���ģ������     64
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 188*120);//0xB0  ����AEC/AGCֱ��ͼ������Ŀ,���44000   4096=320*240  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);     //0x1C  here is the way to regulate darkness :)    
  
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,0x03C7);//0x01C7�ԱȶȲ���ף�0x03C7�Աȶ���� Recommended by design to improve performance in HDR mode and when frame rate is low. ������������HDRģʽ�����ܺ�֡����
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.������R0x20��9���е�ȫ�ָ�λʱ�������ظ���ƫ��
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.�Ľ���FPN
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.  
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x08, 0x03D4);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x09, 0x03E7);
  //100DB //�����ع�ʱ��
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH1,0x01BB);        //0x08 Coarse Shutter IMAGEW 1
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH2,0x01D9);        //0x09 Coarse Shutter IMAGEW 2
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH_CONTROL,0x0064); //0x0A Coarse Shutter IMAGEW Control 
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A  
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,0x00E8);   //0x0B Coarse Shutter IMAGEW Total
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_DESIRED_BIN_REG, 37); //0xA5  ͼ������  50  1-64
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_ANALOG_GAIN,0x8010);           //0x35
  
  //80dB HDR
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x08, 0x03CA);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x09, 0x03DE);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0A, 0x0064);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0F, 0x0103);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x35, 0x8010);   

  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x03);          //0x0c  ��λ
  
  DMATransDataInit(DMA_CH4,(void*)&PTD_BYTE0_IN,(void*)Image_Data,PTD12,DMA_BYTE1,IMAGEW,DMA_rising_down);//��ʼ��DMA�ɼ�  

}
void MT9V034_SetFrameResolution(uint16_t height,uint16_t width)
{
  uint16_t data = 0x0002;  //ԭ����0
  
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
  
  //����ͼ�������СΪ120*188������ʧ��Ұ��ͬʱ�������ܳ���Ҫ
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x030A);   //��������ͷͼ��4*4��Ƶ���PCLK, 27/4 = 6.75M ,BIT4,5�������� 
  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x033A);   //��������ͷͼ��4*4��Ƶ���PCLK, 27/4 = 6.75M ,BIT4,5��������:�������Ҿ����� 
  
  data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);       //LQ-MT9V034 needs vertical mirror to capture correct image
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_READ_MODE, data);//��������ͷ��ȡģʽ
  
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
* �������ƣ�void DMATransDataInit(DMA_CHn CHn,void *SADDR, void *DADDR,PTXn_e ptxn,DMA_BYTEn byten,u32 count,DMA_PORTx2BUFF_cfg cfg) 
* ����˵������ʼ��DMA 
* ����˵����ͨ����ţ�Դ��ַ��Ŀ�ĵ�ַ�������ܽţ��ֽ�����ѭ���������������� 
* �������أ��� 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
***************************************************************/ 
__ramfunc void DMATransDataInit(DMA_CHn CHn,void *SADDR, void *DADDR,PTXn_e ptxn,DMA_BYTEn byten,u32 count,DMA_Count_cfg cfg) 
{ 
  u8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //���㴫���ֽ���
  //����ʱ�� 
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //��DMAģ��ʱ��
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //��DMA��·������ʱ��
  // ���� DMA ͨ�� �� ������ƿ� TCD ( Transfer Control Descriptor ) 
  DMA_SADDR(CHn) =    (u32)SADDR;                         // ����  Դ��ַ
  DMA_DADDR(CHn) =    (u32)DADDR;                         // ����Ŀ�ĵ�ַ
  DMA_SOFF(CHn)  =    0;                                  // ����Դ��ַ����
  DMA_DOFF(CHn)  =    BYTEs;                                  // ÿ�δ����Ŀ�ļ�BYUEs
  DMA_ATTR(CHn)  =    (0
                       | DMA_ATTR_SMOD(0x0)                // Դ��ַģ����ֹ  Source address modulo feature is disabled
                       | DMA_ATTR_SSIZE(byten)             // Դ����λ�� ��DMA_BYTEn  ��    SSIZE = 0 -> 8-bit ��SSIZE = 1 -> 16-bit ��SSIZE = 2 -> 32-bit ��SSIZE = 4 -> 16-byte
                       | DMA_ATTR_DMOD(0x0)                // Ŀ���ַģ����ֹ
                       | DMA_ATTR_DSIZE(byten)             // Ŀ������λ�� ��DMA_BYTEn  ��  ���òο�  SSIZE
                       );
  DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count); //��ǰ��ѭ������
  DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKYES_BITER(count);//��ʼ��ѭ������
  
  DMA_CR &= ~DMA_CR_EMLM_MASK;                            // CR[EMLM] = 0  disable Minor Loop Mapping
  
  DMA_NBYTES_MLNO(CHn) =   DMA_NBYTES_MLNO_NBYTES(BYTEs); // ͨ��ÿ�δ����ֽ�������������ΪBYTEs���ֽڡ�ע��ֵΪ0��ʾ����4GB 
  
  // ���� DMA ���������Ĳ��� 
  DMA_SLAST(CHn)      =   0;                              //����Դ��ַ�ĸ���ֵ,��ѭ��������ָ�Դ��ַ
  DMA_DLAST_SGA(CHn)  =   0;                              //����Ŀ�ĵ�ַ�ĸ���ֵ,��ѭ��������ָ�Ŀ�ĵ�ַ���߱��ֵ�ַ
  DMA_CSR(CHn)        =   (0
                           | DMA_CSR_DREQ_MASK            //��ѭ��������ֹͣӲ������
                           | DMA_CSR_INTMAJOR_MASK        //��ѭ������������ж�
                          );
  // ���� DMA ����Դ 
  DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
                                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                                            | DMAMUX_CHCFG_SOURCE((ptxn >> 5) + DMA_Port_A) // ͨ����������Դ:     
                                           );
  //GPIO_Init(GPIOD,12,GPI,0); //����  
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << (ptxn>>5));                                //����PORTx�˿�
  GPIO_PDDR_REG(GPIOX[(ptxn>>5)]) &= ~(1 << (ptxn & 0x1f));                        //���ö˿ڷ���Ϊ����
  PORT_PCR_REG(PORTX[(ptxn>>5)], (ptxn & 0x1F)) = ( 0
                                                   | PORT_PCR_MUX(1)               // ����GPIO
                                                   | PORT_PCR_IRQC(cfg & 0x03 )    // ȷ������ģʽ
                                                   | ((cfg & 0xc0 ) >> 6)          // �����������������裬����û��
                                                  );
  GPIO_PDDR_REG(GPIOX[(ptxn>>5)]) &= ~(1 << (ptxn && 0x1F));  
  
  //�����ж� 
  DMA_EN(CHn);                                      //ʹ��ͨ��CHn Ӳ������
  //DMA_IRQ_EN(CHn);                                //����DMAͨ������
} 

/*************************************************************** 

* 
* �������ƣ�void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val) 
* ����˵��������DMA�����Ŀ�ĵ�ַ��ѭ������ 
* ����˵���� 
* �������أ���
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
***************************************************************/ 
__ramfunc void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val) 
{ 
  DMA_DADDR(CHn) = address;                              //Ŀ�ĵ�ַ 
  DMA_CITER_ELINKNO(CHn) = DMA_CITER_ELINKNO_CITER(Val); //��ǰ��ѭ������ 
  DMA_BITER_ELINKNO(CHn) = DMA_BITER_ELINKYES_BITER(Val);//��ʼ��ѭ������ 
  //�����ж� 
  DMA_EN(CHn);                                           //ʹ��ͨ��CHn Ӳ������
  //DMA_IRQ_EN(CHn);                                     //����DMAͨ������
} 

// ��ȡ��Ҫ��ͼ������
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
->Function:   ���OLED��ʾ����������
->Editor    :    DXC
*******************************************************************************/
void tramsform()
{
  int i = 0,j = 0;
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      //if(Image_Use[i][j] >GaveValue)//ƽ��ֵ��ֵ
      if(Image_Use[i][j] ==0xFE) //�����ֵ   ��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}

//���վ�ֵ�ı������ж�ֵ��
void Get_01_Value(void)
{
  int i = 0,j = 0;
  u8 GaveValue;
  u32 tv=0;
  char txt[16];
  
  //�ۼ�
  for(i = 0; i <LCDH; i++)
  {    
    for(j = 0; j <LCDW; j++)
    {                            
      tv+=Image_Use[i][j];   //�ۼ�  
    } 
  }
  GaveValue=tv/LCDH/LCDW;     //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100 
  sprintf(txt,"%03d:%03d",Threshold,GaveValue);//ǰ��Ϊ�����õ���ֵ������Ϊƽ��ֵ  
  LCD_P6x8Str(80,1,(u8*)txt);
  //���վ�ֵ�ı������ж�ֵ��
  GaveValue=GaveValue*7/10+10;        //�˴���ֵ���ã����ݻ����Ĺ������趨 
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      //if(Image_Use[i][j] >GaveValue)//ƽ��ֵ��ֵ
      if(Image_Use[i][j] >Threshold) //�����ֵ   ��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}
//��ʾͼ��OLEDģ��
void Draw_Road(void)
{ 	 
    u8 i = 0, j = 0,temp=0;
  
  //����֡ͷ��־
  for(i=8;i<56;i+=8)//6*8=48�� 
  {
    LCD_Set_Pos(18,i/8+1);//��ʼλ��
    for(j=0;j<LCDW;j++)  //����
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
//�������Ϸ���Χ��������
void Pixle_Filter(void)
{  
  int nr; //��
  int nc; //��
  
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
*  �������ƣ�void Pixle_Filter_Tu_Xiang(void)                                          *
*  ����˵�������ͼ������                                                  *
*  ����˵������                                                            *
*  �������أ�ֵ�Ĵ�С                                                      *
*  �޸�ʱ�䣺                                                 *
*  ��    ע��               *
***************************************************************************/
void Pixle_Filter_Tu_Xiang(void)
{  
  int nr; //��
  int nc; //��
  
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
*  �������ƣ�int Seek_Road(void)                                           *
*  ����˵����Ѱ�Ұ�ɫ����ƫ��ֵ                                            *
*  ����˵������                                                            *
*  �������أ�ֵ�Ĵ�С                                                      *
*  �޸�ʱ�䣺2017-07-16                                                    *
*  ��    ע�����м�Ϊ0������һ���Ҳ��һ����ֵ����1�����                *
*            ��������ӵ�һ�п�ʼ�������ڶ��н�����                        *
*            ������Ϊ��������ֵԽ��˵��Խƫ��ߣ�                        *
*            ������Ϊ��������ֵԽ��˵��Խƫ�ұߡ�                        *
***************************************************************************/ 
void Seek_Road(void)
{  
  int nr; //��
  int nc; //��
  int temp=0;//��ʱ��ֵ
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
  int nr; //��
  int nc; //��     
  
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
        zb[nr]=nc;//����أ�Խ��Խ��
      }
      if((Pixle[nr+8][nc-1]==1)&&(Pixle[nr+8][nc]==1)&&(Pixle[nr+8][nc+1]==0)&&(Pixle[nr+8][nc+2]==0))
      {
        yb[nr]=nc;//�ұ��أ�Խ��ԽС
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Init(void)
*  ����˵��������SCCB��������ΪGPIO���ܣ���ʱ���������ݷ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
*************************************************************************/
void SCCB_Init(void)
{
  GPIO_Init(GPIOE, 0,GPO,0);//����ΪGPIO����
  GPIO_Init(GPIOE, 1,GPO,0);//����ΪGPIO����  
}

/*************************************************************************
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Wait(void)
*  ����˵����SCCB�ȴ���ʾ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Star(void)
*  ����˵������������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Stop(void)
*  ����˵����ֹͣ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�uint8 SCCB_SendByte(uint8 Data)
*  ����˵����SCCB�����ֽں���
*  ����˵����Ҫ���͵��ֽ�
*  �������أ�Ӧ���ź�
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �������ƣ�uint8 SCCB_ReadByte(void) 
* ����˵����SCCB��ȡ�ֽں��� 
* ����˵���� 
* �������أ���ȡ�ֽ� 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
***************************************************************/ 
uint8 SCCB_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t Byte; 
  SCL_Out; 
  SDA_In; //ʹ������
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
* �������ƣ�static void SCCB_Ack(void) 
* ����˵����IIC�лظ��ź� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�static void SCCB_NAck(void) 
* ����˵����IIC�޻ظ��ź� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
*  ����˵�������豸д����
*  ����˵����Ҫ���͵��ֽ�
*  �������أ�Ӧ���ź�
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �������ƣ�uint8_t SCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
* ����˵������ȡ���� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�int SCCB_Probe(uint8_t chipAddr) 
* ����˵������ѯ�õ�ַ���豸�Ƿ���� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�Sendshanwai1  byDXC 
* ����˵��������ͼ��ɽ����λ��������Ƿ���һά�����
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��12��28�� 
* �� ע�� 
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
        tmImage[i]=0xfe; //��ֹ���ͱ�־λ 
      } 
      UART_Put_Char(UART_4,tmImage[i]); 
     
  }
    UART_Put_Char(UART_4,~CMD_WARE);
    UART_Put_Char(UART_4,CMD_WARE);
}
/***************************************************************
* 
* �������ƣ�Sendshanwai2  byDXC 
* ����˵��������ͼ��ɽ����λ��������Ƿ��Ͷ�ά�����
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��12��28�� 
* �� ע�� 
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
            tmImage[i][j]=0xfe; //��ֹ���ͱ�־λ 
          } 
          UART_Put_Char(UART_4,tmImage[i][j]); 
        } 
    }
    UART_Put_Char(UART_4,~CMD_WARE);
    UART_Put_Char(UART_4,CMD_WARE);
}

/***************************************************************
* 
* �������ƣ�SendPhone 
* ����˵��������һά���鵽�ֻ� ��

***************************************************************/ 
void SendPhone(uint8  tmImage[100])
{
  int i = 0;
  UART_Put_Char(UART_0,0xFE); //����֡ͷ��־ WindowsFormsApplication1.exe
  UART_Put_Char(UART_0,0xEF); //����֡ͷ��־ WindowsFormsApplication1.exe
  for(i=0;i < 100; i++) 
  {      
      if(tmImage[i]==0xff) 
      { 
        tmImage[i]=0xfe; //��ֹ���ͱ�־λ 
      } 
      UART_Put_Char(UART_4,tmImage[i]); 
     
  }
  UART_Put_Char(UART_0,0xEF); //����֡β��־ 
  UART_Put_Char(UART_0,0xFE); //����֡β��־ 
}

/***************************************************************
* 
* �������ƣ�SendPicture 
* ����˵��������ͼ����λ�� ����ͬ����λ��ע���޸Ķ�Ӧ�����ݽ���Э��
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
***************************************************************/ 
void UARTSendPicture2(uint8_t tmImage[60][94]) 
{ 
  int i = 0, j = 0; 
  UART_Put_Char(UART_4,0xFE); //����֡ͷ��־ WindowsFormsApplication1.exe
  UART_Put_Char(UART_4,0xEF); //����֡ͷ��־ WindowsFormsApplication1.exe
  for(i=0;i < IMAGEH; i++) 
  { 
    for(j=0;j <IMAGEW;j++) 
    { 
      if(tmImage[i][j]==0xff) 
      { 
        tmImage[i][j]=0xfe; //��ֹ���ͱ�־λ 
      } 
      UART_Put_Char(UART_4,tmImage[i][j]); 
    } 
  }
  UART_Put_Char(UART_4,0xEF); //����֡β��־ 
  UART_Put_Char(UART_4,0xFE); //����֡β��־ 
} 

void UARTSendPicture(uint8_t tmImage[60][94]) 
{ 
  int i = 0, j = 0; 
  UART_Put_Char(UART_4,0xFF); //����֡ͷ��־ DEMOK��λ��  
  for(i=0;i < 60; i++) 
  { 
    for(j=0;j <94;j++) 
    { 
      if(tmImage[i][j]==0xff) 
      { 
        tmImage[i][j]=0xfe; //��ֹ���ͱ�־λ 
      } 
      UART_Put_Char(UART_4,tmImage[i][j]); 
    } 
  }
} 
//ɽ����λ��
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    s8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    s8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

   UART_Put_Buff(UART_4, cmdf, sizeof(cmdf));    //�ȷ�������
   
   UART_Put_Buff(UART_4, (s8 *)imgaddr, imgsize); //�ٷ���ͼ��

   UART_Put_Buff(UART_4, cmdr, sizeof(cmdr));    //�ȷ�������
}
/*************************************************************** 
* 
* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵��������ֵ��С 
* ����˵���� 
* �������أ���ֵ��С 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
�ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7�������g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
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
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 
  
  for (j = 0; j < IMAGEH; j++) 
  { 
    for (i = 0; i < IMAGEW; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
      
  if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ    
  if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //���������ֵ;
} 
/*************************************************************** 
* 
* �������ƣ�void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵����ͼ�����ݶ�ֵ�� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
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
* �������ƣ�void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵����ͼ�����ݶ�ֵ�� ,����Ϊ��ָ���ֵ��
* �޸�ʱ�䣺2018��12��31��  By DXC
* �� ע�� 
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



