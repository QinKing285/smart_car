#include "timer.h"
#include "capture.h"
#include "motor.h"
#include "protocol.h"
#include "common.h"
#include "bsp_pid.h"

//ͨ�ö�ʱ��7��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIMx_calcPID_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM7ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	TIM_Cmd(TIM7,DISABLE); //��ʼ��ʱ�Ȳ�������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMx_calcPID_start(void)
{
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��7
}

void TIMx_calcPID_stop(void)
{
	TIM_Cmd(TIM7,DISABLE); //�رն�ʱ��7
}

//PID���ڼ��������
int pwm_val_protect(int pwm_input)
{
	int pwm_output = 0;
	
	if(pwm_input>999) 
	{
		pwm_output = 999;
	}
	else if(pwm_input<-999) 
	{
		pwm_output = -999;
	}
	else if((pwm_input>-100)&&(pwm_input<100)) 
	{
		pwm_output = 0;
	}
	else
	{
		pwm_output = pwm_input;
	}
	
	return pwm_output;
}




//���ڶ�ʱ���Ļص�����
void AutoReloadCallback()
{
	int sum = 0;
	int res_pwm = 0;
	static int i=0;
	
	sum = read_encoder();

    res_pwm = pwm_val_protect((int)PID_realize(sum));
	
	set_motor_rotate(res_pwm);
	
#if (PID_ASSISTANT_EN)
	i++;
	if(i==8)
	{
		i=0;
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &sum, 1); // ��ͨ�� 1 ����ʵ��ֵ
	}
#else
	i++;
	if(i==100)
	{
		i=0;
		printf("sum:%d set_pwm:%d\r\n",sum,res_pwm);
		
	}
#endif
}



//��ʱ��7�жϷ�����
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
	{
		AutoReloadCallback();
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //����жϱ�־λ
}
