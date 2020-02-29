/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������KV58F24���ܳ�VDĸ��
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
�������汾��V1.0
�������¡�2017��12��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4
��Target  ��MKV58F1M0VLQ24
��Crystal �� 50.000Mhz
��busclock��137.500MHz
��pllclock��275.000MHz

���������ڵ����У��û������ڴλ������޸�--20171222
������ܵ��Բ���������ң���������õ���������㣬��Ҷ����л��ģ�����

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

int16_t ACC_X,ACC_Y,ACC_Z,GYRO_X,GYRO_Y,GYRO_Z,MAG_X,MAG_Y,MAG_Z;

void Test_9AX(void)
{
  u16 tem=0;
  float fv=0.01;
  unsigned char  txt[16]="X:";
  Init_LQ_9AX();
  while(1)
  {
    //��������ˢ��
    Update9AX();
    sprintf((char*)txt,"ax:%06d",ACC_X);
    LCD_P6x8Str(0,0,txt);
    sprintf((char*)txt,"ay:%06d",ACC_Y);
    LCD_P6x8Str(0,1,txt);
    sprintf((char*)txt,"az:%06d",ACC_Z);
    LCD_P6x8Str(0,2,txt);
    sprintf((char*)txt,"gx:%06d",GYRO_X);
    LCD_P6x8Str(0,3,txt);
    sprintf((char*)txt,"gy:%06d",GYRO_Y);
    LCD_P6x8Str(0,4,txt);
    sprintf((char*)txt,"gz:%06d",GYRO_Z);
    LCD_P6x8Str(0,5,txt);
    sprintf((char*)txt,"MX:%06d",MAG_X);
    LCD_P6x8Str(0,6,txt);
    sprintf((char*)txt,"MY:%06d",MAG_Y);   //
    LCD_P6x8Str(0,7,txt);
    time_delay_ms(100);
  }
}

void Init_LQ_9AX(void)
{
  IIC_Init();                         //��ʼ��I2C1 
  
  ///////FXAS21002//////////////////////////////////////////////////////////////////////////////////////////
  // write 0000 0000 = 0x00 to CTRL_REG1 to place FXOS21002 in Standby
  // [7]: ZR_cond=0
  // [6]: RST=0
  // [5]: ST=0 self test disabled
  // [4-2]: DR[2-0]=000 for 800Hz
  // [1-0]: Active=0, Ready=0 for Standby mode
  IIC_WriteByteToSlave( FXAS21002_ADDR, FXAS21002_CTRL_REG1, 0x00); 
  // write 0000 0000 = 0x00 to CTRL_REG0 to configure range and filters
  // [7-6]: BW[1-0]=00, LPF disabled
  // [5]: SPIW=0 4 wire SPI (irrelevant)
  // [4-3]: SEL[1-0]=00 for 10Hz HPF at 200Hz ODR
  // [2]: HPF_EN=0 disable HPF
  // [1-0]: FS[1-0]=00 for 1600dps (TBD CHANGE TO 2000dps when final trimmed parts available)
  IIC_WriteByteToSlave(FXAS21002_ADDR, FXAS21002_CTRL_REG0, 0x00);     
  time_delay_ms(100);  
  // write 0000 0001 = 0x01 to CTRL_REG1 to configure 800Hz ODR and enter Active mode
  // [7]: ZR_cond=0
  // [6]: RST=0
  // [5]: ST=0 self test disabled
  // [4-2]: DR[2-0]=000 for 800Hz ODR
  // [1-0]: Active=1, Ready=0 for Active mode
  IIC_WriteByteToSlave(FXAS21002_ADDR, FXAS21002_CTRL_REG1, 0x03);
  
  //////FXOS8700///////////////////////////////////////////////////////////////////////////////////////////
  time_delay_ms(100);    
  uint8_t val;
  IIC_ReadByteFromSlave(FXOS8700_ADDR, FXOS8700_CTRL_REG1, &val);  //��CTRL1�Ĵ���
  IIC_WriteByteToSlave(FXOS8700_ADDR, FXOS8700_CTRL_REG1, val & (uint8_t)~ACTIVE_MASK);   //ʹ8700���ڴ���ģʽ
  IIC_WriteByteToSlave(FXOS8700_ADDR, F_SETUP_REG,F_MODE_DISABLED);    //��FIFO
  IIC_WriteByteToSlave(FXOS8700_ADDR, FXOS8700_M_CTRL_REG2, MOD_HIGH_RES);   //�߷ֱ���ģʽ
  IIC_WriteByteToSlave( FXOS8700_ADDR, M_CTRL_REG1, (M_RST_MASK | M_OSR_MASK | M_HMS_MASK));   //���ģʽ���Ӽƺ͵شż�ͬʱʹ��
  IIC_WriteByteToSlave(FXOS8700_ADDR, M_CTRL_REG2, M_HYB_AUTOINC_MASK); 
  IIC_WriteByteToSlave(FXOS8700_ADDR, XYZ_DATA_CFG_REG, FULL_SCALE_4G);       //�Ӽ� ����4gģʽ
  IIC_WriteByteToSlave(FXOS8700_ADDR, FXOS8700_CTRL_REG1, (HYB_DATA_RATE_200HZ | ACTIVE_MASK));       //�����������Ƶ�� 200hz ���Ҽ���FX8700
  time_delay_ms(10);
}

void Update9AX(void)
{      

  
  uint8_t acc_buf[6]; 
  uint8_t mag_buf[6]; 
  uint8_t gyr_buf[6];
  IIC_ReadMultByteFromSlave(FXOS8700_ADDR,0x01,6,acc_buf);
  IIC_ReadMultByteFromSlave(FXOS8700_ADDR,0x33,6,mag_buf); 
  IIC_ReadMultByteFromSlave(FXAS21002_ADDR,0x01,6,gyr_buf);
  
  
  ACC_X = (int16_t)((uint16_t)acc_buf[0]<<8 | (uint16_t)acc_buf[1])/4;  //�Ӽ��� 14λ�Ĵ�����
  ACC_Y = (int16_t)((uint16_t)acc_buf[2]<<8 | (uint16_t)acc_buf[3])/4;
  ACC_Z = (int16_t)((uint16_t)acc_buf[4]<<8 | (uint16_t)acc_buf[5])/4;

  MAG_X = (int16_t)((uint16_t)mag_buf[0]<<8 | (uint16_t)mag_buf[1]);
  MAG_Y = (int16_t)((uint16_t)mag_buf[2]<<8 | (uint16_t)mag_buf[3]);
  MAG_Z = (int16_t)((uint16_t)mag_buf[4]<<8 | (uint16_t)mag_buf[5]);
  
  GYRO_X = (int16_t)((uint16_t)gyr_buf[0]<<8 | (uint16_t)gyr_buf[1]);
  GYRO_Y = (int16_t)((uint16_t)gyr_buf[2]<<8 | (uint16_t)gyr_buf[3]);
  GYRO_Z = (int16_t)((uint16_t)gyr_buf[4]<<8 | (uint16_t)gyr_buf[5]);
 
}

