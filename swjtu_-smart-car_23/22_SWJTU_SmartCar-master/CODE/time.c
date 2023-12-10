/*
 * time.c
 *
 *  Created on: 2022��1��6��
 *      Author: paper
 */

#include "time.h"
Body Flag;                  //�ṹ�嶨�����ؼ���־λ
float pid_speed_target_val_left_1 = 0;
float pid_speed_target_val_right_1 = 0;
float actual_val_left = 0;
 float actual_val_right = 0;
 float pid_speed_target_val_left = 0;
float pid_speed_target_val_right = 0;
unsigned char int_OK=1;//��ʼ���ɹ���־λ
int start_count = 0;
void Fuse_result(void)
{
  if(int_OK)                //��ʼ���ɹ������жϳɹ�
  {
/////////////////////////���¾��ǿ��Ƶ�ִ�д���//////////////////////
// 2ms����  ������ �ٶ�pid
    if(Flag.T_2ms==1)   // 2ms��һ��
    {
       if(go)
          {
            scratch_line();//�����������ʱ
          }
      //scratch_line();
      get_icm20602_accdata_spi();
      get_icm20602_gyro_spi();
      if(icm_acc_z>=10000&&in_san_cha == 0)
      {
        warning= 1;
      }
      //������ۼƽǶ�
     if(icm_Home_flag)
     {
       icm_home += icm_gyro_x ;
     }
     else
     {
       icm_home = 0;
     }
      //Բ���ۼƽǶ�
      if(icm_ring_flag)
      {
        icm_ring += icm_gyro_x ;
      }
      else
      {
        icm_ring = 0;
      }
      //�����ۼƽǶ�
      if(icm_san_cha_flag)
      {
        icm_san_cha += icm_gyro_x ;
      }
      else
      {
        icm_san_cha = 0;
      }
      //�ػ��ۼƽǶ�
      if(icm_back_loop_flag)
      {
        icm_back_loop += icm_gyro_x ;
      }
      else
      {
        icm_back_loop = 0;
      }
      if(go)
      { 
        //��ȡ������ֵ
        Encoder_scan();
        //���ۼƾ���
        encoder_all += (encoder_left+encoder_right)/2;
        encoder_all_right  += encoder_right;
        encoder_all_left  += encoder_left;
        //������
        if(In_Home_flag==1)
        {
          encoder_all_home += (encoder_left+encoder_right)/2;
        }
        //�����ۼƾ���
        if(encoder_san_cha_flag)
        {
          encoder_san_cha += (encoder_left+encoder_right)/2;
        }
        else
        {
          encoder_san_cha = 0;
        }
       
        //����
        if(start_count>=50)
        {
          control_speed();
        } 
        //���λ��pid
        pid_wheel();
      }
        if(right_wheel.OUT>=20000||left_wheel.OUT>=20000||left_wheel.OUT<=-20000||right_wheel.OUT<=-20000)
        {
            warning=1;
        }
        if(warning)
        {
        left_wheel.OUT=0;
        right_wheel.OUT=0;
        }
       
        Motor_control_left(left_wheel.OUT);
        Motor_control_right(right_wheel.OUT); 

      // pid_speed_target_val_left_1 += left_wheel.OUT;
      // pid_speed_target_val_right_1 += right_wheel.OUT;
      Flag.T_2ms=0;
     }
  // 6ms����
      if(Flag.T_6ms==1)
      {
        Flag.T_6ms = 0;
      }
  //10ms����  �ٶȿ���
      if(Flag.T_10ms==1)
      {
        if(go)
        { 
        //��ȡ������ֵ
        actual_val_left = encoder_left_speed;
        actual_val_right = encoder_right_speed;
        //����Ŀ��ֵ
        pid_speed_target_val_left = pid_speed_target_val_left_1;
        pid_speed_target_val_right = pid_speed_target_val_right_1;
        pid_speed_target_val_left_1 = 0;
        pid_speed_target_val_right_1 = 0;
        encoder_left_speed = 0;
        encoder_right_speed = 0;
        //����ٶ�pid
        // pid_speed_left_OUT  = pid_speed(pid_speed_target_val_left,actual_val_left,pid_speed_Kp_left,pid_speed_Ki_left,pid_speed_Kd_left);
        // pid_speed_right_OUT = pid_speed(pid_speed_target_val_right,actual_val_right,pid_speed_Kp_right,pid_speed_Ki_right,pid_speed_Kd_right);
        // if(pid_speed_left_OUT>=30000||pid_speed_right_OUT>=30000)
        // {
        //    warning=1;
        // }
        // if(warning)
        // {
        // pid_speed_left_OUT=0;
        // pid_speed_right_OUT=0;
        // }
        // Motor_control_left(pid_speed_left_OUT);
        // Motor_control_right(pid_speed_right_OUT);
        Flag.T_10ms=0;
        }
      }
  //20ms����  ת��pid����  
      if(Flag.T_20ms==1)
      {
        // if(go)
        //   {
        //     scratch_line();//�����������ʱ
        //   }
          //����ɨ��
          Key_scan();
          //ת��һ��pid
          DM_Control();
          //ת�����pid
          D2M_Control();
          if((sw1_status||go)&&hall_detection_flag==0)
          {
            if(yawspeed>=80)
            {
              pwm_duty(S_MOTOR_PIN,duty-80);
            }
            else if(yawspeed<=-80)
            {
              pwm_duty(S_MOTOR_PIN,duty+80);
            }
            else
            {
              pwm_duty(S_MOTOR_PIN,duty-yawspeed);
            }
            
          }
          if(start_count<=150&&go)
            {
              start_count++;
            }
          Flag.T_20ms=0;
      }
  //1000ms����  
      if(Flag.T_1s==1)
      {
        
          ring_ok_flag = 0;
          san_cha_ok_flag =0;
          Flag.T_1s=0;
      }
    }
}

//���Ĺؼ��ı�־λ�ṹ���ʼ��
void Flag_Init(void)
{
  Flag.T_2ms = 0;
  Flag.T_6ms=0;
  Flag.T_10ms =0;
  Flag.T_20ms =0;
  Flag.T_1s =0;
}


