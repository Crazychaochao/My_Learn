#include "System.h"

extern uint8 Run_Flag;      //С���ܵı�־λ
void System_Init()
{
    DisableInterrupts;
    PLL_Init(PLL235);         //�����ں˼�����Ƶ�ʵ� PORTA_IRQn
    time_delay_ms(100);        //��ʱ  
    /************************ ���� KV58�жϵ����ȼ� *****************************/
    NVIC_SetPriorityGrouping(4);        // �������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ� 
    NVIC_SetPriority(PORTD_IRQn,0);     //����ͷ�ɼ��ж� PORTA_IRQn
    NVIC_SetPriority(PORTE_IRQn,1);         // �������ȼ�
    NVIC_SetPriority(PIT0_IRQn,2);      //
    NVIC_SetPriority(PIT2_IRQn,3);      //
    //NVIC_SetPriority(UART4_RX_TX_IRQn,4);      //
    /************************������ʼ�� **********************************/
    MT9V034_Init();                 //����ͷ��ʼ��
    LCD_Init();                     //LCD��ʼ��
    LCD_CLS();                      //����
    KEY_Init();               //����������ڳ�ʼ��  
    SW_Key_Init();              //���뿪�س�ʼ��
    LED_Init();               //LED��ʼ��
    //UART_Init(UART_4,115200); //���ڳ�ʼ��
    
    UART_Init(UART_4,9600);
    UART_Irq_En(UART_4);
    UART_Init(UART_3,9600);         
    UART_Irq_En(UART_3);
   
    
    LED_Ctrl(LEDALL, OFF); 
    GPIO_Init (GPIOC, 12, GPI,0);
    /************************ ������ʼ�� **********************************/
    PID_Parameter_Init(&Angle_PID);		// �ǶȻ�PID������ʼ��--->ƽ������
    PID_Parameter_Init(&MOTOR_PID);		// �ٶȻ�PID������ʼ��--->�����趨���ٶȣ��������ٶȻ�С���ͻᱣ����ԭ��ǰ��ڶ���
    PID_Parameter_Init(&Direct_PID);	        // ת��PID������ʼ��--->����ת��ĽǶ�
    PID_Parameter_Init(&Ang_Vel_PID);	        // ���ٶȻ�PID������ʼ��
    PID_Parameter_Init(&Distance_PID);	        // λ�û�PID������ʼ��
    Radius = 0;				        // ��ʼ��Ŀ��ת��뾶����Ϊ0
    Speed_Set = 0;			        // ��ʼ��Ŀ���ٶ�Ϊ0
    Target_Angle.Y = Zero_Angle*100;            //Ŀ��Ƕ�
    
    time_delay_ms(5);                          //��ʱ 
    Balance_Init();                 //������ʼ��
    uint8 Flage_picture = 0;
    uint8 Flage_DianCi = 0;
    /************************ ������ ��ʼ�� **********************************/
    MPU6050_Init();                 //MPU6050��ʼ��       
    MPU6050_Offset();                   //MPU6050��ƫ������ͣ����ʱ���е㳤
    Run_Flag = 1;
    ADC0_Init();
    /************************ �����ʼ�� **********************************/ 
    //���Ƶ�ʣ�275M/2/(2^4)/1000=8.6kHZ,���Ƶ��ԽС������Խ��
    FTM_PWM_Init(FTM0,FTM_CH0,10000,  0);//Mot0-PTC1
    FTM_PWM_Init(FTM0,FTM_CH1,10000,  0);//Mot1-PTC2 
    FTM_PWM_Init(FTM0,FTM_CH2,10000,  0);//Mot2-PTC3
    FTM_PWM_Init(FTM0,FTM_CH3,10000,  0);//Mot3-PTC4 
    /************************ ��������ʼ�� **********************************/
    FTM_AB_Init(FTM1);             //��������ʼ��  �����������
    FTM_AB_Init(FTM2);             //��������ʼ��  ��������ҵ��
    /*******************��ʱ�жϳ�ʼ��**********************/
    PIT_Init(PIT0, 2);                          //��ʱ 100ms,������̬�Լ�ƽ�����
    PIT_Init(PIT2, 1);   
    EnableInterrupts                            //ʹ��ȫ���ж�
    System_OK = 1;
    Speed();
    Speed_Control();	           // ����Ŀ���ٶ�,�ǳ���Ҫ
}





