#ifndef CODE_PID_H_
#define CODE_PID_H_
#include "headfile.h"

typedef struct
{
    float Kp;                       //����ϵ��Proportional
    float Ki;                       //����ϵ��Integral
    float Kd;                       //΢��ϵ��Derivative
    float Ek;                       //��ǰ���
    float Ek1;                      //ǰһ����� e(k-1)
    float Ek2;                      //��ǰһ����� e(k-2)
    float OUT;                       //PID���
} PID_IncTypeDef;
extern uint8 warning;
void PID_Inc_Init(PID_IncTypeDef* sptr,float kp,float ki,float kd);
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID); //pid��������
float pid_speed(float pid_speed_target_val,float actual_val,float pid_speed_Kp,float pid_speed_Ki,float pid_speed_Kd);
void pid_wheel(void);
void Init_pid_wheel(void);
void Warning_STOP(void);
extern PID_IncTypeDef left_wheel;
extern PID_IncTypeDef right_wheel;
extern float Differential_speed_1 ;
//pid����
    extern    float pid_speed_Kp_left ;
   extern     float pid_speed_Ki_left;
    extern    float pid_speed_Kd_left ;

   extern     float pid_speed_Kp_right ;
   extern     float pid_speed_Ki_right ;
   extern     float pid_speed_Kd_right ;

   extern     float pid_speed_left_OUT  ;
    extern    float pid_speed_right_OUT ;
#endif/* CODE_PID_H_ */
