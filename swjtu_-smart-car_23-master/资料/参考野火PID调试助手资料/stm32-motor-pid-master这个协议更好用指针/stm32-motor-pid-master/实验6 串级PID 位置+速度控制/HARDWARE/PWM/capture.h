#ifndef _CAPTURE_H
#define _CAPTURE_H

#include "sys.h"

#define ENCODER_TIM_PSC  0          /*��������Ƶ*/
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
#define CNT_INIT 0                  /*��������ֵ*/

#define ENCODER_RESOLUTION 11    /*������һȦ������������*/
#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 34 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
/* 11*4*34= 1496*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 

void TIMx_encoder_init(void);
void calc_motor_rotate_speed(void);
uint32_t read_encoder(void);
#endif
