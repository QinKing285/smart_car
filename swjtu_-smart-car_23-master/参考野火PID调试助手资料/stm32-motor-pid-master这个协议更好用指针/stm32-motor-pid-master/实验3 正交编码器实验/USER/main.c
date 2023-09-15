/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0
  * @date    2021-xx-xx
  * @brief   ��������������
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
#include "cmd.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "capture.h"
#include "motor.h"

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ 



extern u8 isReceiveUartCmd;
int main(void)
{ 
	int encoder;
	int t =0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	
	MOTOT_Init();/*����������ų�ʼ��*/
	TIM3_PWM_Init(1000-1,84-1);/*���PWMʹ���ת��*/

	TIM4_ENCODER_Init();/*��������������������*/
	TIM7_Int_Init(1000-1,8400-1);/*��ʱ�������Խ����ٶȼ���*/
	/*8400��Ƶ��10kHz, ��1ms����10�Σ�ARR��Ϊ1000����100ms���һ�Σ�ʵ��ÿ100ms����һ��ת��*/

	while(1)
	{
		if(isReceiveUartCmd)/*���մ��ڵ�PWM����ָ��*/
		{
			isReceiveUartCmd = 0;
			process_uart_data();
		}
	}
}
