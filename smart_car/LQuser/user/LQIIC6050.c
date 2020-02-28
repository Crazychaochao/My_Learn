/*******************************************************************************
��ƽ    ̨������KV58F24���ܳ�VDĸ��
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2017��12��15��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4�����ϰ汾
��Target  ��MKV58F1M0VLQ24
��Crystal �� 50.000Mhz
��busclock��137.500MHz
��pllclock��275.000MHz

���������ڵ����У��û������ڴλ������޸�--2018/11/20
������ܵ��Բ��������ң���������õ���������㣬��Ҷ����л��ģ�����

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
 
char Offset_OK = 0;
uint8_t MPU6050_Init(void)
{
    uint8_t res;
    IIC_Init();     ////MPU6050 ֧��400K I2C
    res=MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);           //��ȡMPU6050��ID
    if(res!=MPU6050_ID) //����ID��ȷ
    {
        printf("ID=%#X\r\n",res);
        printf("MPU6050 is fail!\n");
    }
    else  printf("MPU6050 is OK!\n");

    res = 0;
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
    time_delay_ms(100);  //��ʱ100ms
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X00);//����MPU6050
    res += MPU_Set_Gyro_Fsr(3);					        	//�����Ǵ�����,��2000dps   
    res += MPU_Set_Accel_Fsr(1);					       	 	//���ٶȴ�����,��4g
    res += MPU_Set_Rate(50);						       	 	//���ò�����50Hz
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,0x02);      //�������ֵ�ͨ�˲���   98hz
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_INT_EN_REG,0X00);   //�ر������ж�
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
//    MPU_Write_Byte(MPU6050_ADDR,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
//    MPU_Write_Byte(MPU6050_ADDR,MPU_INTBP_CFG_REG,0X80);//INT���ŵ͵�ƽ��Ч
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X01);  	//����CLKSEL,PLL X��Ϊ�ο�
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT2_REG,0X00);  	//���ٶ��������Ƕ�����
    
    if(res == 0)  //����Ĵ�����д��ɹ�
    {
        printf("MPU set is OK!\n");
    }
    else return 1;
    
    return 0;
}



//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU6050_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((uint16_t)buf[0]<<8)|buf[1];  
    temp=21+((double)raw)/333.87;  
    return (short)temp*100;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res; 
	res=MPU_Read_Len(MPU6050_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}

//�õ��Ӽ�ֵ���¶�ֵ�����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Raw_data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz)
{
    uint8_t buf[14],res;  
	res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
        *gx=((uint16_t)buf[8]<<8)|buf[9];  
		*gy=((uint16_t)buf[10]<<8)|buf[11];  
		*gz=((uint16_t)buf[12]<<8)|buf[13];
	} 	
    return res;
}


//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    return IIC_WriteMultByteToSlave(addr, reg, len, buf);
} 

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    return IIC_ReadMultByteFromSlave(addr, reg, len, buf);     
}


//IICдһ���ֽ� 
//devaddr:����IIC��ַ
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t value)
{
    return IIC_WriteByteToSlave(addr, reg, value);
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t value;
    IIC_ReadByteFromSlave(addr, reg, &value);
    return value;
}
//��MPU_Get_Raw_data()�õ������ݴ洢���ṹ����
uint8_t MPU6050_GetData(S_INT16_XYZ *GYRO,S_INT16_XYZ *ACC)
{
    uint8_t buf[14],res;  
    res=IIC_ReadMultByteFromSlave(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
    //int16_t T=0;
    //int16_t temp;                          //�¶� 
    //uint16_t a[8];
    //uint8_t  txt[30]="X:";
    //MPU6050_Init();                          //��ʼ��MPU6050    
    //temp=MPU_Get_Temperature();	//�õ��¶�ֵ
    if(res==0)
    {
        //��ƫ�ı�־λΪ1����
        if (Offset_OK)
        {
            //��ȡ���ٶȼ�ԭʼ����
            ACC->X=((int8_t)buf[0]<<8)|buf[1];  
            ACC->Y=((int8_t)buf[2]<<8)|buf[3];  
            ACC->Z=((int8_t)buf[4]<<8)|buf[5];
            //��ȡ������ԭʼ����
            GYRO->X=((int8_t)buf[8]<<8)|buf[9]- GYRO_Offset.X;  //���õ������ݼ�ȥ���ƫ��
            GYRO->Y=((int8_t)buf[10]<<8)|buf[11]- GYRO_Offset.Y;  //��̬����ƫ��
            GYRO->Z=((int8_t)buf[12]<<8)|buf[13]- GYRO_Offset.Z;   
        }
        else
        {
            //�����ǵ�x,y,z
            ACC->X=((int8_t)buf[0]<<8)|buf[1];  
            ACC->Y=((int8_t)buf[2]<<8)|buf[3];  
            ACC->Z=((int8_t)buf[4]<<8)|buf[5];
            //���ٶȵ�x,y,z
            GYRO->X=((int8_t)buf[8]<<8)|buf[9];  
            GYRO->Y=((int8_t)buf[10]<<8)|buf[11];  
            GYRO->Z=((int8_t)buf[12]<<8)|buf[13];
        }
    } 	
    return res;
}


#define AcceRatio 	984.0f          
#define GyroRatio 	16.4f           
#define Gyro_Gr		0.0010653	// ���ٶȱ�ɻ���	�˲�����Ӧ����2000��ÿ��
#define ACC_FILTER_NUM 5		// ���ٶȼ��˲����
#define GYRO_FILTER_NUM 1		// �������˲����
int32 ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];	// �˲���������
int32 GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
/*
 * ��������Data_Filter
 * ����  �����ݻ����˲�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Data_Filter(void)	// �����˲�
{
	uint8 i;
	float ACC_Angle;//���ٶȽǶ�
	int64 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;
	
	ACC_X_BUF[0] = ACC.X;	// ���»�����������
	ACC_Y_BUF[0] = ACC.Y;
	ACC_Z_BUF[0] = ACC.Z;
	GYRO_X_BUF[0] = GYRO.X;
	GYRO_Y_BUF[0] = GYRO.Y;
	GYRO_Z_BUF[0] = GYRO.Z;
	
	for(i=0;i<ACC_FILTER_NUM;i++)//�����ٶȼ��еĸ����������temp��
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
		
	}
	for(i=0;i<GYRO_FILTER_NUM;i++)//�����ٶȵĸ����������temp��
	{
		temp4 += GYRO_X_BUF[i];
		temp5 += GYRO_Y_BUF[i];
		temp6 += GYRO_Z_BUF[i];
	}
	
	ACC_Real.X = temp1 / ACC_FILTER_NUM / AcceRatio;//���ٶȼ�X/�ɼ�����/AcceRatio
	ACC_Real.Y = temp2 / ACC_FILTER_NUM / AcceRatio;
	ACC_Real.Z = temp3 / ACC_FILTER_NUM / AcceRatio;
	GYRO_Real.X = temp4 / GYRO_FILTER_NUM / GyroRatio;//���ٶȼ�X/�ɼ�����/GyroRatio
	GYRO_Real.Y = temp5 / GYRO_FILTER_NUM / GyroRatio;
	GYRO_Real.Z = temp6 / GYRO_FILTER_NUM / GyroRatio;
	//���黬�����£����¸��µ����ݴ��������
	for(i = 0; i < ACC_FILTER_NUM - 1; i++)
	{ 
		ACC_X_BUF[ ACC_FILTER_NUM-1-i ] = ACC_X_BUF[ ACC_FILTER_NUM-2-i ];
		ACC_Y_BUF[ ACC_FILTER_NUM-1-i ] = ACC_Y_BUF[ ACC_FILTER_NUM-2-i ];
		ACC_Z_BUF[ ACC_FILTER_NUM-1-i ] = ACC_Z_BUF[ ACC_FILTER_NUM-2-i ];
		
	}
        //���黬�����£����¸��µ����ݴ��������
	for(i = 0; i < GYRO_FILTER_NUM - 1; i++)
	{
		GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
	}
}



//�Ǿ��󿨶����˲�����Щ�������ø�
#define Peried 1/500.0f		        //��������������
#define Q 14.0f				//��������2.0		ԽС����Խ�������ټ��ٶȼ�Խ��Խƽ��
#define R 900.0f			//��������5000.0	ԽС���ټ��ٶȼ�Խ��
float KalmanGain = 1.0f;	        //����������


void KalmanFilter(float ACC_Angle)
{
	//�������˲��ֲ�����
    static float Priori_Estimation = 0;//�������
    static float Posterior_Estimation = 0;//�������
    static float Priori_Convariance = 0;//���鷽��
    static float Posterior_Convariance = 0;//���鷽��
		
	//�������˲�
    //1.ʱ�����(Ԥ��) : X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k) 
    Priori_Estimation = Posterior_Estimation - GYRO_Real.Y*Peried;		//������ƣ����ֻ�ýǶ�
	if (Priori_Estimation != Priori_Estimation)
	{
		Priori_Estimation = 0;
	}
	
    //2.��������Э����  : P(k|k-1) = A(k,k-1)*P(k-1|k-1)*A(k,k-1)'+Q(k) 
    Priori_Convariance = (float)sqrt( Posterior_Convariance * Posterior_Convariance + Q * Q );
	if (Priori_Convariance != Priori_Convariance)
	{
		Priori_Convariance = 0;
	}
	
    //  ������������ƣ���������  
    // 1.���㿨��������  : K(k) = P(k|k-1)*H(k)' / (H(k)*P(k|k-1)*H(k)' + R(k)) /
    KalmanGain = (float)sqrt( Priori_Convariance * Priori_Convariance / ( Priori_Convariance * Priori_Convariance + R * R ) );
	if (KalmanGain != KalmanGain)
	{
		KalmanGain = 1;
	}
	
    //2.��������(У��): X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1)) 
    Posterior_Estimation  = Priori_Estimation + KalmanGain * (ACC_Angle - Priori_Estimation );
	if (Posterior_Estimation != Posterior_Estimation)
	{
		Posterior_Estimation = 0;
	}
	
    // 3.���º���Э����  : P(k|k) =��I-K(k)*H(k)��*P(k|k-1) 
    Posterior_Convariance = (float)sqrt(( 1 - KalmanGain ) * Priori_Convariance * Priori_Convariance );
	if (Posterior_Convariance != Posterior_Convariance)
	{
		Posterior_Convariance = 0;
	}
	
    //�õ����սǶ� 
    Attitude_Angle.Y = Posterior_Estimation;
	
	if (Attitude_Angle.Y != Attitude_Angle.Y)
	{
		Attitude_Angle.Y = 1;
	}
}

/*
 * ��������MPU6050_Offset
 * ����  ���������ɼ���ƫ
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void MPU6050_Offset(void)
{
	uint8 i, Count = 250;           //�ɼ�20������      ԭ����100
	int64 temp[6] = {0};
	
	GYRO_Offset.X = 0;
	GYRO_Offset.Y = 0;
	GYRO_Offset.Z = 0;
	
	for (i = 0; i < Count; i++)
	{
		MPU6050_GetData(&GYRO, &ACC);	// ��ȡ����������
		time_delay_ms(2);
		
		temp[0] += ACC.X;
		temp[1] += ACC.Y;
		temp[2] += ACC.Z;
		
		temp[3] += GYRO.X;
		temp[4] += GYRO.Y;
		temp[5] += GYRO.Z;
	}
        //����������ƽ��ֵ
	ACC_Offset.X = temp[0] / Count;
	ACC_Offset.Y = temp[1] / Count;
	ACC_Offset.Z = temp[2] / Count;
	
	GYRO_Offset.X = temp[3] / Count;
	GYRO_Offset.Y = temp[4] / Count;
	GYRO_Offset.Z = temp[5] / Count;
	
	Offset_OK = 1;
        
}


/*
 * ��������Get_Attitude
 * ����  ����̬����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Get_Attitude(void)	// ��̬����
{
	IMUupdate(GYRO_Real.X*Gyro_Gr*GyroRatio, 
			  GYRO_Real.Y*Gyro_Gr*GyroRatio, 
			  GYRO_Real.Z*Gyro_Gr*GyroRatio, 
			  ACC_Real.X * AcceRatio, 
			  ACC_Real.Y * AcceRatio, 
			  ACC_Real.Z * AcceRatio);	// ��̬�����ŷ����
}


//===============================��Ԫ��============================================
#define Kp 1.6f //10.0f             	// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f//1.2f // //0.008f  	// integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                   	// half the sample period�������ڵ�һ��
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; 	// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0; 	// scaled integral error
/*
 * ��������IMUupdate
 * ����  ����Ԫ�ؽ���ŷ����
 * ����  �������� ���ٶȼ�
 * ���  ����
 * ����  ���ڲ�����
 */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	// �Ȱ���Щ�õõ���ֵ���
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q1q1 = q1*q1;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;

	if (ax*ay*az == 0)
	{
		return;
	}
		
	norm = sqrt(ax*ax + ay*ay + az*az);	// acc���ݹ�һ��
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)	�����������������/��Ǩ
	vx = 2*(q1q3 - q0q2);									// ��Ԫ����xyz�ı�ʾ
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) ;		// �������������õ���־������
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;	// �������л���
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;	// �����PI�󲹳��������ǣ����������Ư��
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;	// �����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

	// integrate quaternion rate and normalise	// ��Ԫ�ص�΢�ַ���
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	
	Attitude_Angle.Y = asin(-2*q1*q3 + 2*q0*q2) * 57.3; // pitch
//	Attitude_Angle.X = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)*57.3; // roll
//	Attitude_Angle.Z = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1)*57.3; // yaw
//	Attitude_Angle.Z = 0;
}


void Test_MPU6050(void)
{
  int16_t aacx,aacy,aacz;	//���ٶȴ�����ԭʼ����
  int16_t gyrox,gyroy,gyroz;	//������ԭʼ����
  int16_t temp;                          //�¶� 
  uint16_t a[8];
  uint8_t  txt[30]="X:";
  MPU6050_Init();                          //��ʼ��MPU6050
  LCD_Init();
  while(1)
  {
//    temp=MPU_Get_Temperature();	//�õ��¶�ֵ
    /*
    MPU_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);	//�õ����ٶȴ���������
    sprintf((char*)txt,"ax:%06d",aacx);
    LCD_P6x8Str(0,0,txt);
    sprintf((char*)txt,"ay:%06d",aacy);
    LCD_P6x8Str(0,1,txt);
    sprintf((char*)txt,"az:%06d",aacz);
    LCD_P6x8Str(0,2,txt);
    sprintf((char*)txt,"gx:%06d",gyrox);
    LCD_P6x8Str(0,3,txt);
    sprintf((char*)txt,"gy:%06d",gyroy);
    LCD_P6x8Str(0,4,txt);
    sprintf((char*)txt,"gz:%06d",gyroz);
    LCD_P6x8Str(0,5,txt);
    a[0]=aacx;
    a[1]=aacy;
    a[2]=aacz;
    a[3]=gyrox;
    a[4]=gyroy;
    a[5]=gyroz;    
    UART_Put_Str(UART_4,(u8*)a);  
    time_delay_ms(100);
   */
    MPU6050_GetData(&GYRO, &ACC);	// ��ȡ����������
    sprintf((char*)txt,"ax:%06d",ACC.X);
    LCD_P6x8Str(0,0,txt);
    sprintf((char*)txt,"ay:%06d",ACC.Y);
    LCD_P6x8Str(0,1,txt);
    sprintf((char*)txt,"az:%06d",ACC.Z);
    LCD_P6x8Str(0,2,txt);
    sprintf((char*)txt,"gx:%06d",GYRO.X);
    LCD_P6x8Str(0,3,txt);
    sprintf((char*)txt,"gy:%06d",GYRO.Y);
    LCD_P6x8Str(0,4,txt);
    sprintf((char*)txt,"gz:%06d",GYRO.Z);
    LCD_P6x8Str(0,5,txt);
    time_delay_ms(100);
    
  }
}
void OLED_show(S_INT16_XYZ *GYRO,S_INT16_XYZ *ACC)
{
    uint8_t  txt[30]="X:";
    sprintf((char*)txt,"ax:%06d",ACC->X);
    LCD_P6x8Str(0,0,txt);
    sprintf((char*)txt,"ay:%06d",ACC->Y);
    LCD_P6x8Str(0,1,txt);
    sprintf((char*)txt,"az:%06d",ACC->Z);
    LCD_P6x8Str(0,2,txt);
    sprintf((char*)txt,"gx:%06d",GYRO->X);
    LCD_P6x8Str(0,3,txt);
    sprintf((char*)txt,"gy:%06d",GYRO->Y);
    LCD_P6x8Str(0,4,txt);
    sprintf((char*)txt,"gz:%06d",GYRO->Z);
    LCD_P6x8Str(0,5,txt);
    
    sprintf((char*)txt,"speed_Left:%04d",speed_Left);
    LCD_P6x8Str(0,4,txt);
    sprintf((char*)txt,"speed_Right:%04d",speed_Right);
    LCD_P6x8Str(0,5,txt);
    

}
void Vcan_Sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    Uart_Putbuff(UART_4, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    Uart_Putbuff(UART_4, (uint8_t *)wareaddr, waresize);    //��������
    Uart_Putbuff(UART_4, cmdr, sizeof(cmdr));    //���ͺ�����

}