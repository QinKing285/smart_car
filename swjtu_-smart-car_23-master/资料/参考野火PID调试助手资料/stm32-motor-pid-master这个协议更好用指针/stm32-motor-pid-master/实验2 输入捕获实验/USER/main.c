/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0
  * @date    2021-xx-xx
  * @brief   ���벶�����
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
#include "capture.h"

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ 


int main(void)
{ 
	u16 led0pwmval=0;    
	u8 dir=1;
	long long temp=0; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	
 	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
	
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ�������1������ʱ1us�� 
	
	while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
 		delay_ms(10);
		
		if(dir)
		{
			led0pwmval+=5;//dir==1 led0pwmval����
		}
		else 
		{
			led0pwmval-=5;	//dir==0 led0pwmval�ݼ� 
		}
 		if(led0pwmval>300)
		{
			dir=0;//led0pwmval����300�󣬷���Ϊ�ݼ�
		}
		if(led0pwmval==0)
		{
			dir=1;	//led0pwmval�ݼ���0�󣬷����Ϊ����
		}
 
		TIM_SetCompare1(TIM3,led0pwmval);	/*CCR �޸ıȽ�ֵ��ռ�ձȣ�*/
		
		
		/* �ɹ�������һ�θߵ�ƽ (1000 0000) */
		if(TIM5CH1_CAPTURE_STA&0X80)        
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;  /* ��ȡ����Ĵ���N (0011 1111) */
			temp*=0XFFFFFFFF;		 		/* ���ʱ���ܺ� = N*�������ֵ */
			temp+=TIM5CH1_CAPTURE_VAL;		/* �ܵĸߵ�ƽʱ�� = ���ʱ���ܺ� + �½���ʱ�ļ���ֵ*/
			
			printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;			 //������һ�β���
		}
	}
}
