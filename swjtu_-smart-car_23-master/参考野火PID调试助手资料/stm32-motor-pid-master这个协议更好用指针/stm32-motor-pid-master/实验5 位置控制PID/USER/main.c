/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0
  * @date    2021-xx-xx
  * @brief   PIDλ�ÿ��Ʋ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:STM32F407 
  * ���ں�  :��ũ��ѧϰ(Coder-love-study)
  * ���˲���:https://xxpcb.gitee.io
  *
  ******************************************************************************
  */ 
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "capture.h"
#include "motor.h"
#include "protocol.h"
#include "bsp_pid.h"
#include "common.h"



int main(void)
{ 
	int32_t target_speed = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	delay_init(168);  //��ʼ����ʱ����
	
	/*����������ų�ʼ��*/
	MOTOT_Init();
	
	/*���PWM��ʼ��*/
	TIMx_pwm_init(1000-1,84-1);

	/*������������ȡ��ʼ��*/
	TIMx_encoder_init();
	
	/*8400��Ƶ��10kHz, ��1ms����10�Σ�ARR��Ϊ100����10ms���һ�Σ�ʵ��ÿ10ms����һ��pid*/
	TIMx_calcPID_init(100-1,8400-1);/*��ʱ10ms*/
	
	/*Э�����ݽ�����ʼ��*/
	protocol_init();

	/*��ʼ�����ڲ�����Ϊ115200*/
	uart_init(115200);

    /* PID ������ʼ�� */
	PID_param_init();
	
#if defined(PID_ASSISTANT_EN)
	/*��ʼ��ʱ���Ϸ�stop��ͬ����λ����������ť״̬*/
	set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);  

	/*��ȡĬ�ϵ�Ŀ��ֵ*/
	target_speed = (int32_t)get_pid_target();
	/*��ͨ��1����Ŀ��ֵ*/
	set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_speed, 1);     
#endif
	
	while(1)
	{
		/* �������ݴ��� */
		receiving_process();
	}
}
