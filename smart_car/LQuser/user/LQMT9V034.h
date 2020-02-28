/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱KV58F24智能车VD母板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年3月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4及以上
【Target  】MKV58F1M0VLQ24
【Crystal 】 50.000Mhz
【busclock】137.500MHz
【pllclock】275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_MT9V034_H_
#define __LQ_MT9V034_H_
#include "include.h"

#define IMAGEH  120  //行 HEIGHT 待采集摄像头图像高度行数
#define IMAGEW  188  //列 WIDTH  待采集摄像头图像宽度列数

#define LCDH    60  //OLED显示的行数
#define LCDW    94  //OLED显示的列数

#define SCL_Out     DDRE1=1      //配置输出作为SCL_Out
#define SDA_Out     DDRE0=1      //配置作为输出作为SDA_Out
#define SDA_In      DDRE0=0      //配置作为输入作为SDA_In

#define SCL_High    PTE1_OUT=1   //配置输出高电平
#define SCL_Low     PTE1_OUT=0   //配置输出低电平
#define SDA_High    PTE0_OUT=1   //配置输出高电平
#define SDA_Low     PTE0_OUT=0   //配置输出低电平
#define SDA_Data    PTE0_IN      //读取引脚上的引脚状态

#define SLH       24 
#define FLINE     25  
#define SLINE     40
#define TLINE     55
#define MAX_ROW   60
#define MAX_COL   94 
#define CAMERA_SIZE IMAGEH*IMAGEW

extern float chabihe;
extern int disconnect_flag;
extern int distance;
extern int zhilidistance;
extern int zhilidistance1;
extern uint8 number_flag;
extern int lose_left_flag;
extern int8 Obstacle_flag;              //障碍判断
extern int lose_right_flag;
extern int8 diszhidao_flag;              //直道标志
extern int8 in_right_flag;   
extern int8 in_left_flag;  
extern int8 left_huandao_flag;           //左环岛判断1
extern int8 right_huandao_flag;    
extern int8 remain_flag ;
extern int8 out_left_flag;              //出环岛
extern int8 out_right_flag;
extern uint8 Obstacle_black;
extern uint8 Obstacle_White;
void Find_Obstacle(uint8 n,uint8 m,uint8 flag);
float camera(void)  ;
void Find_disconnect();                      //障碍检测 
void in_huandao(void);
void Handle_Huandao(void);
void replenish(void);
float error_biancha(void);
float error_biancha_sanlun(void);
void scan(void);
void SendPicture(void);
void SCCB_Init(void);
void SCCB_Wait(void);
void SCCB_Stop(void);
void SCCB_Star(void);
uint8 SCCB_SendByte(uint8 Data);
void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data);
uint8_t SCCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) ;
int SCCB_Probe(uint8_t chipAddr);
extern uint8 Flage_picture;
extern uint8 Flage_DianCi;
void change(void) ;
extern void MT9V034_Reset(void);
extern void MT9V034_SetReservedReg(void);
extern void MT9V034_SetFrameRate(uint8_t frameRate);
extern void MT9V034_SetFrameResolution(uint16_t height,uint16_t width);
extern void MT9V034_SetAutoExposure(bool enable);
void vcan_sendimg(void *imgaddr, uint32_t imgsize);
void UARTSendPicture(uint8_t tmImage[60][94]) ;
uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) ;
void BinaryImage(u8 *dst,uint8_t ThresholdV) ;

void Binaryzation_ZP(uint8_t ThresholdVV);
void MT9V034_Init(void); 
void Cam_Init(void);
void Get_Pixel(void);
void Get_Back(void);
void Draw_Road(void);
void Get_Use_Image(void);
void Get_01_Value(void);
void Pixle_Filter(void);
void Pixle_Filter_Tu_Xiang(void);
void Seek_Road(void);
void FindTiXing(void);
void Test_LQV034(void);
void Line_Draw1(void);
void UARTSendPicture3(uint8_t tmImage[IMAGEH][IMAGEW]) ;
void UARTSendPicture2(uint8_t tmImage[60][94]) ;
void Sendshanwai2(uint8 tmImage[60][94]);
void Sendshanwai1(uint8  tmImage[120]);
void SendPhone(uint8  tmImage[100]);
extern void tramsform();
void Find_broken(void);
#define MT9V034_I2C_ADDR	                    0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT		    480
#define MAX_IMAGE_WIDTH			    752
#define MT9V034_PIXEL_ARRAY_HEIGHT	            492
#define MT9V034_PIXEL_ARRAY_WIDTH	            782
#define MT9V034_CHIP_VERSION			    0x00
#define MT9V034_CHIP_ID	                    0x1324

#define MT9V034_COLUMN_START	                    0x01
#define MT9V034_COLUMN_START_MIN	            1
#define MT9V034_COLUMN_START_DEF	            1
#define MT9V034_COLUMN_START_MAX	            752

#define MT9V034_ROW_START	                    0x02
#define MT9V034_ROW_START_MIN	            4
#define MT9V034_ROW_START_DEF	            4
#define MT9V034_ROW_START_MAX	            482

#define MT9V034_WINDOW_HEIGHT	            0x03
#define MT9V034_WINDOW_HEIGHT_MIN	            1
#define MT9V034_WINDOW_HEIGHT_DEF	            64
#define MT9V034_WINDOW_HEIGHT_MAX	            480

#define MT9V034_WINDOW_WIDTH	                    0x04
#define MT9V034_WINDOW_WIDTH_MIN	            1
#define MT9V034_WINDOW_WIDTH_DEF	            64
#define MT9V034_WINDOW_WIDTH_MAX	            752

#define MINIMUM_HORIZONTAL_BLANKING	    91 // see datasheet

#define MT9V034_HORIZONTAL_BLANKING	            0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN	    43
#define MT9V034_HORIZONTAL_BLANKING_MAX	    1023

#define MT9V034_VERTICAL_BLANKING	            0x06
#define MT9V034_VERTICAL_BLANKING_MIN	    4
#define MT9V034_VERTICAL_BLANKING_MAX	    3000

#define MT9V034_CHIP_CONTROL	                    0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE         (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE         (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL	    (1 << 8)

#define MT9V034_SHUTTER_WIDTH1	            0x08
#define MT9V034_SHUTTER_WIDTH2	            0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL	    0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH	    0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN	    1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF	    480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX	    32767

#define MT9V034_RESET	                    0x0C

#define MT9V034_READ_MODE	                    0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK	    (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT	    0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK        (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT       2
#define MT9V034_READ_MODE_ROW_BIN_2         (1<<0)
#define MT9V034_READ_MODE_ROW_BIN_4         (1<<1)
#define MT9V034_READ_MODE_COL_BIN_2         (1<<2)
#define MT9V034_READ_MODE_COL_BIN_4         (1<<3)
#define MT9V034_READ_MODE_ROW_FLIP	            (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP	    (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS	    (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS	            (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE	            0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR       (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR         (1 << 6)

#define MT9V034_V1_CTRL_REG_A	        0x31
#define MT9V034_V2_CTRL_REG_A	        0x32
#define MT9V034_V3_CTRL_REG_A	        0x33
#define MT9V034_V4_CTRL_REG_A	        0x34

#define MT9V034_ANALOG_GAIN	                    0x35
#define MT9V034_ANALOG_GAIN_MIN	            16
#define MT9V034_ANALOG_GAIN_DEF	            16
#define MT9V034_ANALOG_GAIN_MAX	            64

#define MT9V034_MAX_ANALOG_GAIN	            0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX	            127

#define MT9V034_FRAME_DARK_AVERAGE	            0x42
#define MT9V034_DARK_AVG_THRESH	            0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK         (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT        0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK	    (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT	    8

#define MT9V034_ROW_NOISE_CORR_CONTROL	    0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE	    (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG	    (1 << 7)

#define MT9V034_PIXEL_CLOCK		            0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE		    (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME	    (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE		    (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE	    (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK	    (1 << 4)

#define MT9V034_TEST_PATTERN		            0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK	    (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT	    0
#define MT9V034_TEST_PATTERN_USE_DATA	    (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK	    (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE	    (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL	    (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL	    (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL	    (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE		    (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP		    (1 << 14)

#define MT9V034_AEC_AGC_ENABLE		    0xAF
#define MT9V034_AEC_ENABLE		            (1 << 0)
#define MT9V034_AGC_ENABLE		            (1 << 1)
#define MT9V034_THERMAL_INFO		            0xc1
#define MT9V034_ANALOG_CTRL                     (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE                 (1<<7)
#define MT9V034_MAX_GAIN                        (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A      (0xD5)
#define MT9V034_HDR_ENABLE_REG	        0x0F
#define MT9V034_ADC_RES_CTRL_REG	        0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG	0x70
#define MT9V034_TEST_PATTERN_REG       	0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG	0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG	0xA5
#define MT9V034_MAX_GAIN_REG        	0xAB
#define MT9V034_MIN_EXPOSURE_REG       	0xAC  // datasheet min coarse shutter width
#define MT9V034_MAX_EXPOSURE_REG       	0xAD  // datasheet max coarse shutter width
#define MT9V034_AEC_AGC_ENABLE_REG	0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG	0xB0

#endif