
#ifndef __ADC_H__
#define __ADC_H__ 

typedef enum
{
  //参考手册p910 40.1.3.1 ADC0 Channel Assignment for 144-Pin Package      
  // -------------------------------HSADC0A 20181219确认----------------------- 
  HSADC0A_CH0=0,//PTE16
  HSADC0A_CH1=1,//PTE17
  HSADC0A_CH2=2,//HSADC1A_CH2
  HSADC0A_CH3=3,//HSADC1A_CH3
  HSADC0A_CH4=4,//PTE29
  HSADC0A_CH5=5,//PTE30
  HSADC0A_CH6=6,//ADC0_SE7A
  HSADC0A_CH7=7,//ADC0_SE4B
  HSADC0A_CH8=8,//PTE20
  HSADC0A_CH9=9,//PTE21
  HSADC0A_CH10=10,//HSADC1B_CH2
  HSADC0A_CH11=11,//HSADC1B_CH3
  HSADC0A_CH12=12,//CMP0_IN4/MP0_IN3
  HSADC0A_CH13=13,//无
  HSADC0A_CH14=14,//PTB2
  HSADC0A_CH15=15,//PTA17
  HSADC0A_CH16=16,//VREFH
  HSADC0A_CH17=17,//无
  HSADC0A_CH18=18,//DAC0_OUT
  //SYNCA  //
  //EOSA   //
  
  HSADC0B_CH0=0,//PTE18
  HSADC0B_CH1=1,//PTE19
  HSADC0B_CH2=2,//PTB0
  HSADC0B_CH3=3,//PB1
  HSADC0B_CH4=4,//PTE24
  HSADC0B_CH5=5,//PTE25
  HSADC0B_CH6=6,//PTB10
  HSADC0B_CH7=7,//PTB11
  HSADC0B_CH8=8,//PTC0
  HSADC0B_CH9=9,//PTC1
  HSADC0B_CH10=10,//PTE2
  HSADC0B_CH11=11,//PTE3
  HSADC0B_CH12=12,//VREFH
  HSADC0B_CH13=13,//no 
  HSADC0B_CH14=14,//
  HSADC0B_CH15=15,//PTB3
  HSADC0B_CH16=16,//PTE0
  HSADC0B_CH17=17,//PTE1
  HSADC0B_CH18=18,//DAC0_OUT
  //SYNCA  //
  //EOSA   //  
  
  // -----------------------------HSADC1A 20181219确认-----------------------     
  HSADC1A_CH0=0,//PTE0
  HSADC1A_CH1=1,//PTE1
  HSADC1A_CH2=2,//HSADC0A_CH2
  HSADC1A_CH3=3,//HSADC0A_CH3
  HSADC1A_CH4=4,//PTE4
  HSADC1A_CH5=5,//PTE5
  HSADC1A_CH6=6,//PTE11
  HSADC1A_CH7=7,//PTE21
  HSADC1A_CH8=8,//PTD5
  HSADC1A_CH9=9,//PTD6
  HSADC1A_CH10=10,//NO
  HSADC1A_CH11=11,//PTD1
  HSADC1A_CH12=12,//PTB6
  HSADC1A_CH13=13,//PTB7
  HSADC1A_CH14=14,//NO
  HSADC1A_CH15=15,//NO
  HSADC1A_CH16=16,//VREFH
  HSADC1A_CH17=17,//NO
  HSADC1A_CH18=18,//DAC0_OUT
  //SYNCA  //
  //EOSA   //   
  
  HSADC1B_CH0=0,//PTE2
  HSADC1B_CH1=1,//PTE3
  HSADC1B_CH2=2,//HSADC0A_CH10
  HSADC1B_CH3=3,//HSADC0A_CH11
  HSADC1B_CH4=4,//PTE24
  HSADC1B_CH5=5,//PTE25
  HSADC1B_CH6=6,//PTE12
  HSADC1B_CH7=7,//PTE6
  HSADC1B_CH8=8,//PTA7
  HSADC1B_CH9=9,//PTA8
  HSADC1B_CH10=10,//PTC2
  HSADC1B_CH11=11,//PTC8
  HSADC1B_CH12=12,//PTC9
  HSADC1B_CH13=13,//PTC10
  HSADC1B_CH14=14,//PTC11
  HSADC1B_CH15=15,//NO
  HSADC1B_CH16=16,//VREFH
  HSADC1B_CH17=17,//NO
  HSADC1B_CH18=18,//DAC0_OUT
  //SYNCA  //
  //EOSA   //   
} HSADCn_Ch_e;

typedef enum  //ADC模块
{    
  HSADC_0a,
  HSADC_1,
} HSADCn_e;

//精度位数
typedef enum HSADC_nbit
{
  HSADC_6bit  = 0x00,    
  HSADC_8bit  = 0x01,
  HSADC_10bit = 0x02,
  HSADC_12bit = 0x03
} HSADC_nbit;

typedef enum
{
  //参考手册p910 40.1.3.1 ADC0 Channel Assignment for 144-Pin Package
  // -------------------------------ADC0 20181218确认-------------------------       
  ADC0_DP0=0,    //ADC0_SE0
  ADC0_DP1=1,    //PTE16  
  ADC0_DP2=2,    //PTE4    
  ADC0_DP3=3,    //PTE11  
  //ADC0_CFG2 &=~ADC_CFG2_MUXSEL_MASK   0选择a  0 ADxxa channels are selected.
  ADC0_SE4a=4,   //PTE6      
  ADC0_SE5a=5,   //PTE18      
  ADC0_SE6a=6,   //PTE19      
  ADC0_SE7a=7,   //HSADC0A_CH6
  //ADC0_CFG2 |=ADC_CFG2_MUXSEL_MASK    1选择b  1 ADxxb channels are selected.
  ADC0_SE4b=12,  //HSADC0A_CH7 
  ADC0_SE5b=13,  //PTE20  
  ADC0_SE6b=14,  //PTB4  
  ADC0_SE7b=15,  //PTB5      
  ADC0_SE8=8,    //ADC0_SE8 
  ADC0_SE9=9,    //PTE17  
  ADC0_SE10=10,  //PTE5 
  ADC0_SE11=11,  //PTE12       
  
} ADC0_Ch_e;
//精度位数
typedef enum ADC0_nbit
{
  ADC_8bit   = 0x00,
  ADC_10bit  = 0x02,
  ADC_12bit  = 0x01,
  ADC_16bit  = 0x03
}ADC0_nbit;

void ADC0_Init(void);
u16 ADC0_Once(ADC0_Ch_e adc_ch,ADC0_nbit bit);
u16 ADC0_Mid(ADC0_Ch_e adc_ch,ADC0_nbit bit);
u16 ADC0_Ave(ADC0_Ch_e adc_ch,ADC0_nbit bit,u16 N);
void ADC0_Start(ADC0_Ch_e adc_ch,ADC0_nbit bit);
void Test_ADC0(void);
uint16_t Test_voltage(void);
void adc_start(ADC0_Ch_e adcn_ch, ADC0_nbit bit);

#endif 
