/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱KV58F24智能车VD母板
【编    写】管脚复用部分由CHIUSIR修改确认，源程序在SDK，野火和苏大基础上移植
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2017年12月15日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4
【Target  】MKV58F1M0VLQ24
【Crystal 】 50.000Mhz
【busclock】137.500MHz
【pllclock】275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
extern char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, DMP_Flag;
//-------------------------------------------------------------------------*
//函数名: pit_init
//功  能: 初始化PIT
//参  数: pitn:模块名PIT0或PIT1或PIT2或PIT3
//        cnt 中断时间，单位1ms
//返  回: 无
//简  例: pit_init(PIT0,1000); PIT0中断，1000ms，即1s进入PIT0_interrupt()一次
//-------------------------------------------------------------------------*
void PIT_Init(PITn pitn, u32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率

    /* 开启时钟*/
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //使能PIT时钟

    /* PIT模块控制 PIT Module Control Register (PIT_MCR) */
    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //使能PIT定时器时钟 ，调试模式下继续运行

    /* 定时器加载值设置 Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt*40*1000;                                   //设置溢出中断时间

    //定时时间到了后，TIF 置 1 。写1的时候就会清0
    PIT_Flag_Clear(pitn);                                             //清中断标志位

    /* 定时器控制寄存器 Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) |= ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //使能 PITn定时器,并开PITn中断

    NVIC_EnableIRQ((IRQn_Type)(pitn + 48));			      //开接收引脚的IRQ中断
}



//-------------------------------------------------------------------------*
//函数名: PIT0_interrupt
//功  能: PIT中断函数
//参  数: 无
//返  回: 无
//简  例: 由初始化决定，多长时间进入一次
//-------------------------------------------------------------------------*
short speed=0;
int speed_Left = 0;
int speed_Right = 0;
extern int32 MOTOR_Speed_Left;
extern int32 MOTOR_Speed_Right; 
extern int32 MOTOR_Speed_Left_Last;
extern int32 MOTOR_Speed_Right_Last;
extern int32 MOTOR_Left_Acc;
extern int32 MOTOR_Right_Acc;
extern int16 Annulus_Delay;
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);                //清中断标志位
    static uint8 Time_10ms = 0;
    static uint8 Time_50ms = 0;
    static uint8 Time_100ms = 0;
    static uint8 Time_200ms = 0;
    
    Time_10ms++;
    Time_50ms++;
    Time_100ms++;
    Time_200ms++;               
    //跟新角速度，姿态角度，速度
    Ang_Velocity_Flag = 1;	// 角速度2ms获取一次
    if (Time_10ms == 5)         // 姿态角度10ms控制一次	5
    {
        Time_10ms = 0;
        Angle_Flag = 1;	        
    }
    if (Time_50ms == 25)        //50ms更新一次
    {
        Time_50ms = 0;
    }
    if (Time_100ms == 50)        // 速度100ms控制一次 50
    {
        Time_100ms = 0;
        Speed_Flag = 1;      
    }
   
   /* if(Obstacle_flag == 2 || Obstacle_flag == 1||disconnect_flag==1)
    {
       distance1=distance;      
    }
    else
    {
       zhilidistance = zhilidistance1;       
    }*/
    distance1=distance;
    zhilidistance = zhilidistance1;
    Balance_Control();	                // 车身平衡控制
}
extern uint8 Run_Flag;      //小车跑的标志位 
void PIT1_IRQHandler()
{
    PIT_Flag_Clear(PIT1);       //清中断标志位
}
void PIT2_IRQHandler()
{
    PIT_Flag_Clear(PIT2);       //清中断标志位
    piancha();
    
    
}

void PIT3_IRQHandler()
{
  PIT_Flag_Clear(PIT3);       //清中断标志位

}

// test function 
void Test_PIT(void)
{    
       //UART串口演示
    UART_Init(UART_4,115200);    
    UART_Put_Str(UART_4,"LongQiu UART4 ADC\n"); 
    PIT_Init(PIT0, 200);          //PIT定时中断初始化
    
    while (1)
    {       
      //LED闪烁
      LED_Ctrl(LED1, RVS);
      UART_Put_Str(UART_4,"LongQiu PIT0 INTERRUPT!\n"); 
      //systick中断延时
      time_delay_ms(500);
    }
    
}

void Pit_Close(PITn pitn)
{
    PIT_Flag_Clear(pitn);                                       //清中断标志位
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
}