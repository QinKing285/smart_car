/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0
  * @date    2021-xx-xx
  * @brief   PWM�������
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
#include "led.h"
#include "pwm.h"

int tmp1,tmp2;
int main(void)
{ 
	u16 led0pwmval=0;    
	u8 dir=1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	delay_init(168);  //��ʼ����ʱ����
	
 	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
	
	while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
		//tmp1 = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6);
		//tmp2 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
 		delay_ms(10);
		
		if(dir)
		{
			led0pwmval+=5;//dir==1 led0pwmval����
		}
		else 
		{
			led0pwmval-=5;	//dir==0 led0pwmval�ݼ� 
		}
 		if(led0pwmval>500)
		{
			dir=0;//led0pwmval����500�󣬷���Ϊ�ݼ�
		}
		if(led0pwmval==0)
		{
			dir=1;	//led0pwmval�ݼ���0�󣬷����Ϊ����
		}
 
		TIM_SetCompare1(TIM3,led0pwmval);	/*CCR �޸ıȽ�ֵ��ռ�ձȣ�*/
	}
}
