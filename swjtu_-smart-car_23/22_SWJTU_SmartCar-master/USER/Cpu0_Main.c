/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC364DP
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-11-23
 ********************************************************************************************************************/
//UART1_TX_P20_10 UART1_RX_P20_9  ���Ͻ�5v -> p20.10 ->20.9 ->5vgnd 
//��� 5V 5V-GND  00.8IO w
//��� 5V 5V-GND  Tx15.6 Rx15.7



#include "Cpu0_Main.h"
#include "headfile.h"
#pragma section all "cpu0_dsram"
float Speed_Left  =   0.0;
float Speed_Right =   0.0;
float Speed_Left_set  =   2.5                        *   109.0925 /5;
float Speed_Right_set =    2.5                    *   109.0925 /5;
uint16 duty = 664;
float p =1.3, i=0, d=0;
int core0_main(void)
{
    //��ʼ��
    Init();
    //������Ҫѭ��
    while (TRUE)
    {
        //if(mt9v03x_finish_flag)
            if(warning)
            {
                Warning_STOP();
            }
            /////////////////////////////////////////////////Debug //////////////////////////////////////////////////////////////////
            //��R_lost_cnt��L_find_cnt��ring_white��ring_white_di��ALL_find_cnt��ring_tiaobian��ring_B_R
            // if(sw1_status&&imageover)
            if(sw1_status)
            {
            //ͨ�����ߴ��ڷ���ͼ����λ��,���뿪��1
                send_pc();
                // for(uint8 hang=0;hang<119;hang=hang+2)
                // {
                // for(uint8 lie=0;lie<179;lie=lie+2)
                // {
                //     // ZW_Image[hang][lie]=255;
                //     ZW_Image[hang/2][lie/2]= Threshold_Image[hang][lie];
                // }
                // ZW_Image[hang/2][leftline[hang]/2]=0;
                // ZW_Image[hang/2][rightline[hang]/2]=0;
                // ZW_Image[hang/2][midline[hang]/2]=0;
                // }
                // ZW_Send_Image(ZW_Image);
                imageover=0;
            }
            // if(!go) //ȥ�����ע�Ϳ��Դﵽ�������Ч�� �� ƽʱΪ�˵��Է����뱣��ע��
            //{
               
            if(sw1_status)
            { 
                //��Ļ��ʾ����,���뿪��2
                //Distance_read();
                ips200_displayimage032_zoom(Threshold_Image[0], MT9V03X_W, MT9V03X_H,180,120);
                ips200_showfloat(10,11, scratch_cnt,10,6);                     ips200_showstr(100,11,"scratch_cnt");
                 ips200_showfloat(10,12, duty,10,6);                                    ips200_showstr(100,12,"duty");
                ips200_showfloat(10,13, in_home_hang,10,6);                                    ips200_showstr(100,13,"in_home_hang");
                ips200_showfloat(10,14, encoder_all_left,10,6);                                    ips200_showstr(100,14,"encoder_all_left");
                ips200_showfloat(10,15, encoder_all,10,6);                                    ips200_showstr(100,15,"encoder_all");
//                 ips200_showfloat(10,16, Differential_speed_1,10,6);                               ips200_showstr(100,16,"D_speed_1");
                // ips200_showint32(10,17,Differential_speed,10);                                  ips200_showstr(100,17,"D_speed");
                // ips200_showfloat(10,16, p,10,6);              ips200_showstr(100,16,"p");
                // ips200_showfloat(10,17, i,10,6);              ips200_showstr(100,17,"i");
                // ips200_showfloat(10,18, d,10,6);              ips200_showstr(100,18,"d");
                Draw_line();
                //ips200_clear(WHITE);
                
            }
            // }
            //�������ش���,����ͣ��
              //mt9v03x_finish_flag=0;

      }
}

#pragma section all restore
