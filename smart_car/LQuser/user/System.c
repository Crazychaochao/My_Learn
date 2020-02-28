#include "System.h"

extern uint8 Run_Flag;      //小车跑的标志位
void System_Init()
{
    DisableInterrupts;
    PLL_Init(PLL235);         //设置内核及总线频率等 PORTA_IRQn
    time_delay_ms(100);        //延时  
    /************************ 配置 KV58中断的优先级 *****************************/
    NVIC_SetPriorityGrouping(4);        // 设置优先级分组,4bit 抢占优先级,没有亚优先级 
    NVIC_SetPriority(PORTD_IRQn,0);     //摄像头采集中断 PORTA_IRQn
    NVIC_SetPriority(PORTE_IRQn,1);         // 配置优先级
    NVIC_SetPriority(PIT0_IRQn,2);      //
    NVIC_SetPriority(PIT2_IRQn,3);      //
    //NVIC_SetPriority(UART4_RX_TX_IRQn,4);      //
    /************************器件初始化 **********************************/
    MT9V034_Init();                 //摄像头初始化
    LCD_Init();                     //LCD初始化
    LCD_CLS();                      //清屏
    KEY_Init();               //按键及输入口初始化  
    SW_Key_Init();              //拨码开关初始化
    LED_Init();               //LED初始化
    //UART_Init(UART_4,115200); //串口初始化
    
    UART_Init(UART_4,9600);
    UART_Irq_En(UART_4);
    UART_Init(UART_3,9600);         
    UART_Irq_En(UART_3);
   
    
    LED_Ctrl(LEDALL, OFF); 
    GPIO_Init (GPIOC, 12, GPI,0);
    /************************ 参数初始化 **********************************/
    PID_Parameter_Init(&Angle_PID);		// 角度环PID参数初始化--->平衡作用
    PID_Parameter_Init(&MOTOR_PID);		// 速度环PID参数初始化--->保持设定的速度，加入了速度环小车就会保持在原地前后摆动了
    PID_Parameter_Init(&Direct_PID);	        // 转向环PID参数初始化--->调节转向的角度
    PID_Parameter_Init(&Ang_Vel_PID);	        // 角速度环PID参数初始化
    PID_Parameter_Init(&Distance_PID);	        // 位置环PID参数初始化
    Radius = 0;				        // 初始化目标转向半径倒数为0
    Speed_Set = 0;			        // 初始化目标速度为0
    Target_Angle.Y = Zero_Angle*100;            //目标角度
    
    time_delay_ms(5);                          //延时 
    Balance_Init();                 //参数初始化
    uint8 Flage_picture = 0;
    uint8 Flage_DianCi = 0;
    /************************ 陀螺仪 初始化 **********************************/
    MPU6050_Init();                 //MPU6050初始化       
    MPU6050_Offset();                   //MPU6050零偏，这里停留的时间有点长
    Run_Flag = 1;
    ADC0_Init();
    /************************ 电机初始化 **********************************/ 
    //电机频率：275M/2/(2^4)/1000=8.6kHZ,电机频率越小，噪声越大
    FTM_PWM_Init(FTM0,FTM_CH0,10000,  0);//Mot0-PTC1
    FTM_PWM_Init(FTM0,FTM_CH1,10000,  0);//Mot1-PTC2 
    FTM_PWM_Init(FTM0,FTM_CH2,10000,  0);//Mot2-PTC3
    FTM_PWM_Init(FTM0,FTM_CH3,10000,  0);//Mot3-PTC4 
    /************************ 编码器初始化 **********************************/
    FTM_AB_Init(FTM1);             //编码器初始化  这个计数左电机
    FTM_AB_Init(FTM2);             //编码器初始化  这个计数右电机
    /*******************定时中断初始化**********************/
    PIT_Init(PIT0, 2);                          //定时 100ms,更新姿态以及平衡控制
    PIT_Init(PIT2, 1);   
    EnableInterrupts                            //使能全部中断
    System_OK = 1;
    Speed();
    Speed_Control();	           // 设置目标速度,非常重要
}





