/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������KV58F24���ܳ�VDĸ��
����    д���ܽŸ��ò�����CHIUSIR�޸�ȷ�ϣ�Դ������SDK��Ұ����մ��������ֲ
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2017��12��15��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4
��Target  ��MKV58F1M0VLQ24
��Crystal �� 50.000Mhz
��busclock��137.500MHz
��pllclock��275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
extern char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, DMP_Flag;
//-------------------------------------------------------------------------*
//������: pit_init
//��  ��: ��ʼ��PIT
//��  ��: pitn:ģ����PIT0��PIT1��PIT2��PIT3
//        cnt �ж�ʱ�䣬��λ1ms
//��  ��: ��
//��  ��: pit_init(PIT0,1000); PIT0�жϣ�1000ms����1s����PIT0_interrupt()һ��
//-------------------------------------------------------------------------*
void PIT_Init(PITn pitn, u32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��

    /* ����ʱ��*/
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //ʹ��PITʱ��

    /* PITģ����� PIT Module Control Register (PIT_MCR) */
    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    /* ��ʱ������ֵ���� Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt*40*1000;                                   //��������ж�ʱ��

    //��ʱʱ�䵽�˺�TIF �� 1 ��д1��ʱ��ͻ���0
    PIT_Flag_Clear(pitn);                                             //���жϱ�־λ

    /* ��ʱ�����ƼĴ��� Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) |= ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //ʹ�� PITn��ʱ��,����PITn�ж�

    NVIC_EnableIRQ((IRQn_Type)(pitn + 48));			      //���������ŵ�IRQ�ж�
}



//-------------------------------------------------------------------------*
//������: PIT0_interrupt
//��  ��: PIT�жϺ���
//��  ��: ��
//��  ��: ��
//��  ��: �ɳ�ʼ���������೤ʱ�����һ��
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
    PIT_Flag_Clear(PIT0);                //���жϱ�־λ
    static uint8 Time_10ms = 0;
    static uint8 Time_50ms = 0;
    static uint8 Time_100ms = 0;
    static uint8 Time_200ms = 0;
    
    Time_10ms++;
    Time_50ms++;
    Time_100ms++;
    Time_200ms++;               
    //���½��ٶȣ���̬�Ƕȣ��ٶ�
    Ang_Velocity_Flag = 1;	// ���ٶ�2ms��ȡһ��
    if (Time_10ms == 5)         // ��̬�Ƕ�10ms����һ��	5
    {
        Time_10ms = 0;
        Angle_Flag = 1;	        
    }
    if (Time_50ms == 25)        //50ms����һ��
    {
        Time_50ms = 0;
    }
    if (Time_100ms == 50)        // �ٶ�100ms����һ�� 50
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
    Balance_Control();	                // ����ƽ�����
}
extern uint8 Run_Flag;      //С���ܵı�־λ 
void PIT1_IRQHandler()
{
    PIT_Flag_Clear(PIT1);       //���жϱ�־λ
}
void PIT2_IRQHandler()
{
    PIT_Flag_Clear(PIT2);       //���жϱ�־λ
    piancha();
    
    
}

void PIT3_IRQHandler()
{
  PIT_Flag_Clear(PIT3);       //���жϱ�־λ

}

// test function 
void Test_PIT(void)
{    
       //UART������ʾ
    UART_Init(UART_4,115200);    
    UART_Put_Str(UART_4,"LongQiu UART4 ADC\n"); 
    PIT_Init(PIT0, 200);          //PIT��ʱ�жϳ�ʼ��
    
    while (1)
    {       
      //LED��˸
      LED_Ctrl(LED1, RVS);
      UART_Put_Str(UART_4,"LongQiu PIT0 INTERRUPT!\n"); 
      //systick�ж���ʱ
      time_delay_ms(500);
    }
    
}

void Pit_Close(PITn pitn)
{
    PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
}