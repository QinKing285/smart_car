/*
  * ������
 *  ��ʮ�߽��ѧ�������������� SWJTU ��������ͷ��
 *  Created on: 2022��1��6��
 *      Author: paper
 */
#include "headfile.h"
#pragma section all "cpu1_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��
void core1_main(void)
{
  //////////////debug   Բ���ʹ���pid��ͻ��?  idea ���߽�ֹ������.  debug Differential_speed ������ɵ��жϸ�ֵ��ͻ idea ����Differential_speed ��̬���ei
    disableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    //�û��ڴ˴����ø��ֳ�ʼ��������
    //�ȴ����к��ĳ�ʼ�����
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE)
    {    
        //�û��ڴ˴���д�������
        if(mt9v03x_finish_flag)
          {
          //��ȡ����״̬
         // Key_scan();//(���ж���)
          //////////////////////////////////////////////////ͼ����//////////////////////////////////////////////////////////////////
          //��򷨶�ֵ��
          Get_Threshold_Image();
          //�����˲�
          //Pixle_Filter();
          //�������б�(�ж�)
          // if(go)
          // {
          //   scratch_line();//�����������ʱ
          // }
          //������
          if(go&&start_count<=50)
          {
            Speed_Left = Speed_Left_set/50*start_count;
            Speed_Right = Speed_Right_set/50*start_count;
          }
          //ZW_Send_Image(Threshold_Image);
          //��ʼѲ��
          get_route_first(119,90);
          //����Ѳ��
          get_route_all();
          //Ѱ�ҽ�ֹ��
          get_route_endline();
          //////////////////////////////////////////////////Ԫ��ʶ��//////////////////////////////////////////////////////////////////
          //���ʼ����Ҷ����ж�
          Curvature_determination();
          //������ж�
          if(go)
          {
            if(Out_Home_flag)
            {
              chu_ku();
            }
            else if(In_Home_flag)
            {
              ru_ku();
            }
          }
          //����ʶ��
          if(ringflag==0&&(san_cha_flag==0||san_cha_flag==1)&&san_cha_ok_flag == 0&&Out_Home_flag == 0&&back_loop_flag == 0&&number_of_back_loop!=0)
          {
            Three_Forks_Road_scan();
          }
            Three_Forks_Road_ru();
            Three_Forks_Road_chu();
          //Բ��ʶ��
          if(ringflag==0&&ring_ok_flag==0&&san_cha_flag==0&&san_cha_ok_flag==0&&Out_Home_flag == 0&&in_san_cha == 0&&back_loop_flag == 0&&Number_of_san_cha!=0&&number_of_back_loop!=0)
          {
            ring_find();
            
          }
         //   ring_start();
            ring_ru();
            ring_chu();
          //�ػ�ʶ��
          if(ringflag==0&&ring_ok_flag==0&&san_cha_flag==0&&san_cha_ok_flag==0&&Out_Home_flag == 0&&in_san_cha == 0 && start_count>=50&&back_loop_flag == 0)
          {
            back_loop_find();
          }
            back_loop_chu();
          //////////////////////////////////////////////////�������//////////////////////////////////////////////////////////////////
          //������
          get_route_midline();
          //����(��һ��Error�ľ����Ȩ)
          get_deviation();
          //��Ȩת��ֵ(������������Error�Ķ��δ���/���������ж�)
          if((ringflag==0||ringflag==3||ringflag==4||ringflag==13||ringflag==14)&&san_cha_flag==0&&in_san_cha==0)
          {
            Weighted_error();
          }
           //if(encoder_all >= 118861)
                 //warning = 1;
          mt9v03x_finish_flag=0;
          imageover=1;
      }
    }
}
#pragma section all restore
