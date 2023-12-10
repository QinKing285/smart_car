//ȫ�������������˲����������������˲�
//1. ��ֵ�˲�  2. �������˲�  3. ��Ԫ���˲�  4.һ�׻����˲�
#include "filter.h"


//---------------------------------------------------------------
//     ��ȻҪע�⣺���벻ͨ����û�ý��в�������
//     ����ʹ�ÿ������˲�������Ԫ���˲�
//---------------------------------------------------------------

 float ENC_lvbo,ENC2_lvbo_out;
//----------------------------------------------------------------
//  @brief      ��ֵ�˲�Ч����̫��
//----------------------------------------------------------------
float encfilter(signed int encgetvalue)
{

    static float encout;

    encout = ((ENC_lvbo * encgetvalue) + (1 - ENC_lvbo) * ENC2_lvbo_out);

    return encout;

}


//----------------------------------------------------------------
//  @brief      �������˲�
//  @param      angle       �Ƕ�
//  @param      angle_dot   ���ٶ�
//  @param      Q_angle     ���ٶ��������Ŷ�
//  @param      Q_gyro      ���ٶ��������Ŷ�
//  @return     float       �����ںϺ�ĽǶ�
//----------------------------------------------------------------
float dt=0.002;//ע�⣺dt��ȡֵΪkalman�˲�������ʱ��
float angle, angle_dot;//�ǶȺͽ��ٶ�
float P[2][2] = {{ 1, 0 },
                 { 0, 1 }};
float Pdot[4] ={ 0,0,0,0};
float Q_angle=0.001, Q_gyro=0.005; //�Ƕ��������Ŷ�,���ٶ��������Ŷ�
float R_angle=0.5 ,C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//angleAx=atan2((ACCEL_XOUT_H / 16384),(ACCEL_ZOUT_H / 16384))*180/3.14;
//�������˲�
float Kalman_Filter(float angle_m, float gyro_m)//angleAx �� gyroGy
{
        angle+=(gyro_m-q_bias) * dt;
        angle_err = angle_m - angle;
        Pdot[0]=Q_angle - P[0][1] - P[1][0];
        Pdot[1]=- P[1][1];
        Pdot[2]=- P[1][1];
        Pdot[3]=Q_gyro;
        P[0][0] += Pdot[0] * dt;
        P[0][1] += Pdot[1] * dt;
        P[1][0] += Pdot[2] * dt;
        P[1][1] += Pdot[3] * dt;
        PCt_0 = C_0 * P[0][0];
        PCt_1 = C_0 * P[1][0];
        E = R_angle + C_0 * PCt_0;
        K_0 = PCt_0 / E;
        K_1 = PCt_1 / E;
        t_0 = PCt_0;
        t_1 = C_0 * P[0][1];
        P[0][0] -= K_0 * t_0;
        P[0][1] -= K_0 * t_1;
        P[1][0] -= K_1 * t_0;
        P[1][1] -= K_1 * t_1;
        angle += K_0 * angle_err; //���ŽǶ�
        q_bias += K_1 * angle_err;
        angle_dot = gyro_m-q_bias;//���Ž��ٶ�

        return angle;
}

//----------------------------------------------------------------
//  @brief      ��Ԫ���˲�
//  @return     float       �����ںϺ�ĽǶ�
//----------------------------------------------------------------

 short Roll,Pitch,Yaw;

#define pi      3.14159265f
float Kp   =   0.8;//ԭ0.8
float Ki    =  0.01;
float halfT =  0.001;//ԭ0.004

float  q0=1,q1=0,q2=0,q3=0;
float  exInt=0,eyInt=0,ezInt=0;


void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    float  norm;
    float  vx, vy, vz;
    float  ex, ey, ez;

    norm = sqrt(ax*ax + ay*ay + az*az); 
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;

    ex = (ay*vz - az*vy) ;
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;

    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    q0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT;
    q1 = q1 + ( q0*gx + q2*gz - q3*gy) * halfT;
    q2 = q2 + ( q0*gy - q1*gz + q3*gx) * halfT;
    q3 = q3 + ( q0*gz + q1*gy - q2*gx) * halfT;

    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    Pitch = asin(2*(q0*q2 - q1*q3 )) * 57.2957795f; 
    Roll = asin(2*(q0*q1 + q2*q3 )) * 57.2957795f; 
    Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.2957795f;
}


float acc_ratio = 0;      //���ٶȼƱ���
float gyro_ratio = 1;    //�����Ǳ���
float dt_Yijie = 0.002;           //��������

//----------------------------------------------------------------
//  @brief      һ�׻����˲�
//  @param      angle_m     ���ٶȼ�����
//  @param      gyro_m      ����������
//  @return     float       �����ںϺ�ĽǶ�
//----------------------------------------------------------------
float angle_calc(float angle_m, float gyro_m)
{
    float temp_angle;
    float gyro_now;
    float error_angle;

    static float last_angle;
    static unsigned char first_angle;

    if(!first_angle)//�ж��Ƿ�Ϊ��һ�����б�����
    {
        //����ǵ�һ�����У����ϴνǶ�ֵ����Ϊ����ٶ�ֵһ��
        first_angle = 1;
        last_angle = angle_m;
    }

    gyro_now = gyro_m * gyro_ratio;

    //���ݲ������ļ��ٶ�ֵת��Ϊ�Ƕ�֮�����ϴεĽǶ�ֵ��ƫ��
    error_angle = (angle_m - last_angle)*acc_ratio;

    //����ƫ���������ǲ����õ��ĽǶ�ֵ���㵱ǰ�Ƕ�ֵ
    temp_angle = last_angle + (error_angle + gyro_now)*dt_Yijie;

    //���浱ǰ�Ƕ�ֵ
    last_angle = temp_angle;

    return temp_angle;
}
