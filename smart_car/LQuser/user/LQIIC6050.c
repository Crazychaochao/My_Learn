/*******************************************************************************
【平    台】龙邱KV58F24智能车VD母板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2017年12月15日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4及以上版本
【Target  】MKV58F1M0VLQ24
【Crystal 】 50.000Mhz
【busclock】137.500MHz
【pllclock】275.000MHz

本程序尚在调试中，用户可以在次基础上修改--2018/11/20
如果你能调试并共享给大家，相信赠人玫瑰手留余香，大家都会感谢你的！！！

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
 
char Offset_OK = 0;
uint8_t MPU6050_Init(void)
{
    uint8_t res;
    IIC_Init();     ////MPU6050 支持400K I2C
    res=MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);           //读取MPU6050的ID
    if(res!=MPU6050_ID) //器件ID正确
    {
        printf("ID=%#X\r\n",res);
        printf("MPU6050 is fail!\n");
    }
    else  printf("MPU6050 is OK!\n");

    res = 0;
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
    time_delay_ms(100);  //延时100ms
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU6050
    res += MPU_Set_Gyro_Fsr(3);					        	//陀螺仪传感器,±2000dps   
    res += MPU_Set_Accel_Fsr(1);					       	 	//加速度传感器,±4g
    res += MPU_Set_Rate(50);						       	 	//设置采样率50Hz
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,0x02);      //设置数字低通滤波器   98hz
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_INT_EN_REG,0X00);   //关闭所有中断
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
//    MPU_Write_Byte(MPU6050_ADDR,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
//    MPU_Write_Byte(MPU6050_ADDR,MPU_INTBP_CFG_REG,0X80);//INT引脚低电平有效
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X01);  	//设置CLKSEL,PLL X轴为参考
    res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT2_REG,0X00);  	//加速度与陀螺仪都工作
    
    if(res == 0)  //上面寄存器都写入成功
    {
        printf("MPU set is OK!\n");
    }
    else return 1;
    
    return 0;
}



//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
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
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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

//得到加计值、温度值、角速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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


//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    return IIC_WriteMultByteToSlave(addr, reg, len, buf);
} 

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    return IIC_ReadMultByteFromSlave(addr, reg, len, buf);     
}


//IIC写一个字节 
//devaddr:器件IIC地址
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t value)
{
    return IIC_WriteByteToSlave(addr, reg, value);
}

//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t value;
    IIC_ReadByteFromSlave(addr, reg, &value);
    return value;
}
//将MPU_Get_Raw_data()得到的数据存储到结构体中
uint8_t MPU6050_GetData(S_INT16_XYZ *GYRO,S_INT16_XYZ *ACC)
{
    uint8_t buf[14],res;  
    res=IIC_ReadMultByteFromSlave(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
    //int16_t T=0;
    //int16_t temp;                          //温度 
    //uint16_t a[8];
    //uint8_t  txt[30]="X:";
    //MPU6050_Init();                          //初始化MPU6050    
    //temp=MPU_Get_Temperature();	//得到温度值
    if(res==0)
    {
        //零偏的标志位为1，即
        if (Offset_OK)
        {
            //获取加速度计原始数据
            ACC->X=((int8_t)buf[0]<<8)|buf[1];  
            ACC->Y=((int8_t)buf[2]<<8)|buf[3];  
            ACC->Z=((int8_t)buf[4]<<8)|buf[5];
            //获取陀螺仪原始数据
            GYRO->X=((int8_t)buf[8]<<8)|buf[9]- GYRO_Offset.X;  //将得到的数据减去零点偏移
            GYRO->Y=((int8_t)buf[10]<<8)|buf[11]- GYRO_Offset.Y;  //姿态角有偏差
            GYRO->Z=((int8_t)buf[12]<<8)|buf[13]- GYRO_Offset.Z;   
        }
        else
        {
            //陀螺仪的x,y,z
            ACC->X=((int8_t)buf[0]<<8)|buf[1];  
            ACC->Y=((int8_t)buf[2]<<8)|buf[3];  
            ACC->Z=((int8_t)buf[4]<<8)|buf[5];
            //加速度的x,y,z
            GYRO->X=((int8_t)buf[8]<<8)|buf[9];  
            GYRO->Y=((int8_t)buf[10]<<8)|buf[11];  
            GYRO->Z=((int8_t)buf[12]<<8)|buf[13];
        }
    } 	
    return res;
}


#define AcceRatio 	984.0f          
#define GyroRatio 	16.4f           
#define Gyro_Gr		0.0010653	// 角速度变成弧度	此参数对应陀螺2000度每秒
#define ACC_FILTER_NUM 5		// 加速度计滤波深度
#define GYRO_FILTER_NUM 1		// 陀螺仪滤波深度
int32 ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];	// 滤波缓存数组
int32 GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
/*
 * 函数名：Data_Filter
 * 描述  ：数据滑动滤波
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Data_Filter(void)	// 数据滤波
{
	uint8 i;
	float ACC_Angle;//加速度角度
	int64 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;
	
	ACC_X_BUF[0] = ACC.X;	// 更新滑动窗口数组
	ACC_Y_BUF[0] = ACC.Y;
	ACC_Z_BUF[0] = ACC.Z;
	GYRO_X_BUF[0] = GYRO.X;
	GYRO_Y_BUF[0] = GYRO.Y;
	GYRO_Z_BUF[0] = GYRO.Z;
	
	for(i=0;i<ACC_FILTER_NUM;i++)//将加速度计中的各总数存放在temp中
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
		
	}
	for(i=0;i<GYRO_FILTER_NUM;i++)//将角速度的各总数存放在temp中
	{
		temp4 += GYRO_X_BUF[i];
		temp5 += GYRO_Y_BUF[i];
		temp6 += GYRO_Z_BUF[i];
	}
	
	ACC_Real.X = temp1 / ACC_FILTER_NUM / AcceRatio;//加速度计X/采集总数/AcceRatio
	ACC_Real.Y = temp2 / ACC_FILTER_NUM / AcceRatio;
	ACC_Real.Z = temp3 / ACC_FILTER_NUM / AcceRatio;
	GYRO_Real.X = temp4 / GYRO_FILTER_NUM / GyroRatio;//角速度计X/采集总数/GyroRatio
	GYRO_Real.Y = temp5 / GYRO_FILTER_NUM / GyroRatio;
	GYRO_Real.Z = temp6 / GYRO_FILTER_NUM / GyroRatio;
	//数组滑动跟新，将新跟新的数据存放数组中
	for(i = 0; i < ACC_FILTER_NUM - 1; i++)
	{ 
		ACC_X_BUF[ ACC_FILTER_NUM-1-i ] = ACC_X_BUF[ ACC_FILTER_NUM-2-i ];
		ACC_Y_BUF[ ACC_FILTER_NUM-1-i ] = ACC_Y_BUF[ ACC_FILTER_NUM-2-i ];
		ACC_Z_BUF[ ACC_FILTER_NUM-1-i ] = ACC_Z_BUF[ ACC_FILTER_NUM-2-i ];
		
	}
        //数组滑动跟新，将新跟新的数据存放数组中
	for(i = 0; i < GYRO_FILTER_NUM - 1; i++)
	{
		GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
	}
}



//非矩阵卡尔曼滤波，这些参数不用改
#define Peried 1/500.0f		        //卡尔曼积分周期
#define Q 14.0f				//过程噪声2.0		越小积分越慢，跟踪加速度计越慢越平滑
#define R 900.0f			//测量噪声5000.0	越小跟踪加速度计越快
float KalmanGain = 1.0f;	        //卡尔曼增益


void KalmanFilter(float ACC_Angle)
{
	//卡尔曼滤波局部参量
    static float Priori_Estimation = 0;//先验估计
    static float Posterior_Estimation = 0;//后验估计
    static float Priori_Convariance = 0;//先验方差
    static float Posterior_Convariance = 0;//后验方差
		
	//卡尔曼滤波
    //1.时间更新(预测) : X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k) 
    Priori_Estimation = Posterior_Estimation - GYRO_Real.Y*Peried;		//先验估计，积分获得角度
	if (Priori_Estimation != Priori_Estimation)
	{
		Priori_Estimation = 0;
	}
	
    //2.更新先验协方差  : P(k|k-1) = A(k,k-1)*P(k-1|k-1)*A(k,k-1)'+Q(k) 
    Priori_Convariance = (float)sqrt( Posterior_Convariance * Posterior_Convariance + Q * Q );
	if (Priori_Convariance != Priori_Convariance)
	{
		Priori_Convariance = 0;
	}
	
    //  卡尔曼后验估计：测量更新  
    // 1.计算卡尔曼增益  : K(k) = P(k|k-1)*H(k)' / (H(k)*P(k|k-1)*H(k)' + R(k)) /
    KalmanGain = (float)sqrt( Priori_Convariance * Priori_Convariance / ( Priori_Convariance * Priori_Convariance + R * R ) );
	if (KalmanGain != KalmanGain)
	{
		KalmanGain = 1;
	}
	
    //2.测量更新(校正): X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1)) 
    Posterior_Estimation  = Priori_Estimation + KalmanGain * (ACC_Angle - Priori_Estimation );
	if (Posterior_Estimation != Posterior_Estimation)
	{
		Posterior_Estimation = 0;
	}
	
    // 3.更新后验协方差  : P(k|k) =（I-K(k)*H(k)）*P(k|k-1) 
    Posterior_Convariance = (float)sqrt(( 1 - KalmanGain ) * Priori_Convariance * Priori_Convariance );
	if (Posterior_Convariance != Posterior_Convariance)
	{
		Posterior_Convariance = 0;
	}
	
    //得到最终角度 
    Attitude_Angle.Y = Posterior_Estimation;
	
	if (Attitude_Angle.Y != Attitude_Angle.Y)
	{
		Attitude_Angle.Y = 1;
	}
}

/*
 * 函数名：MPU6050_Offset
 * 描述  ：传感器采集零偏
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void MPU6050_Offset(void)
{
	uint8 i, Count = 250;           //采集20组数据      原来是100
	int64 temp[6] = {0};
	
	GYRO_Offset.X = 0;
	GYRO_Offset.Y = 0;
	GYRO_Offset.Z = 0;
	
	for (i = 0; i < Count; i++)
	{
		MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
		time_delay_ms(2);
		
		temp[0] += ACC.X;
		temp[1] += ACC.Y;
		temp[2] += ACC.Z;
		
		temp[3] += GYRO.X;
		temp[4] += GYRO.Y;
		temp[5] += GYRO.Z;
	}
        //求各个方向的平均值
	ACC_Offset.X = temp[0] / Count;
	ACC_Offset.Y = temp[1] / Count;
	ACC_Offset.Z = temp[2] / Count;
	
	GYRO_Offset.X = temp[3] / Count;
	GYRO_Offset.Y = temp[4] / Count;
	GYRO_Offset.Z = temp[5] / Count;
	
	Offset_OK = 1;
        
}


/*
 * 函数名：Get_Attitude
 * 描述  ：姿态解算
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Get_Attitude(void)	// 姿态解算
{
	IMUupdate(GYRO_Real.X*Gyro_Gr*GyroRatio, 
			  GYRO_Real.Y*Gyro_Gr*GyroRatio, 
			  GYRO_Real.Z*Gyro_Gr*GyroRatio, 
			  ACC_Real.X * AcceRatio, 
			  ACC_Real.Y * AcceRatio, 
			  ACC_Real.Z * AcceRatio);	// 姿态解算出欧拉角
}


//===============================四元素============================================
#define Kp 1.6f //10.0f             	// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f//1.2f // //0.008f  	// integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                   	// half the sample period采样周期的一半
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; 	// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0; 	// scaled integral error
/*
 * 函数名：IMUupdate
 * 描述  ：四元素解算欧拉角
 * 输入  ：陀螺仪 加速度计
 * 输出  ：无
 * 调用  ：内部调用
 */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	// 先把这些用得到的值算好
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
		
	norm = sqrt(ax*ax + ay*ay + az*az);	// acc数据归一化
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)	估计重力方向和流量/变迁
	vx = 2*(q1q3 - q0q2);									// 四元素中xyz的表示
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) ;		// 向量外积在相减得到差分就是误差
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;	// 对误差进行积分
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;	// 将误差PI后补偿到陀螺仪，即补偿零点漂移
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;	// 这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

	// integrate quaternion rate and normalise	// 四元素的微分方程
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
  int16_t aacx,aacy,aacz;	//加速度传感器原始数据
  int16_t gyrox,gyroy,gyroz;	//陀螺仪原始数据
  int16_t temp;                          //温度 
  uint16_t a[8];
  uint8_t  txt[30]="X:";
  MPU6050_Init();                          //初始化MPU6050
  LCD_Init();
  while(1)
  {
//    temp=MPU_Get_Temperature();	//得到温度值
    /*
    MPU_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);	//得到加速度传感器数据
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
    MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
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
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    Uart_Putbuff(UART_4, cmdf, sizeof(cmdf));    //先发送前命令
    Uart_Putbuff(UART_4, (uint8_t *)wareaddr, waresize);    //发送数据
    Uart_Putbuff(UART_4, cmdr, sizeof(cmdr));    //发送后命令

}