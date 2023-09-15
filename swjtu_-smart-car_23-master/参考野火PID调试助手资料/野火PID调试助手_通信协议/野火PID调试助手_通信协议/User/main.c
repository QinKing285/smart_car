/**
******************************************************************************
//		Ұ��PID����ͨ��Э��
//����˵���뿴  ->  �ض�˵��
//�и�����ֵĵط��� �Ұ���������Ϊ0�����������Լ��ı��9�ˣ����Ǹ��������ǻ����ҷ������ڵ�
//������+9����֪��Ϊʲô��������֪�����ϸ�����ظ����ҡ�д���д��ͷ��û��ȥ�Ľ����������ɡ�
******************************************************************************
*/
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "timer.h"
#include "bsp_led.h" 
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
uint32_t time=0;
int now_dat=111;
uint8_t	Target=100;

PIDC Pid_Test=
{
	.KP=10,
	.KI=11,
	.KD=12
};

int main(void)
{		
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();
	Basic_Time_Init();
	LED_GPIO_Config();
	/* ����һ���ַ��� */

	Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_CHANNAL_TARGET,1,&Target);//����Ŀ��ֵ
	Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_CHANNAL_REALY,1,&now_dat);//����ʵ��ֵ
	Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_PID_PARAMETER,4,&Pid_Test);//����PIDֵ
	Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_PERIOD,1,&time);//��������
	Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_START,0,0);//������ʼ����
	
  while(1)
	{	
		if(time == 100)
		{
			static u8 flag = 0;
			if(flag == 0)
			{
				if(++now_dat >=130 ) 
				{
					flag = 1;
					LED_YELLOW;
				}					
			}
			else 
			{
				if(--now_dat <=90 ) 
				{
					flag = 0;
					LED_CYAN;
				}
			}			
				
		Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_CHANNAL_REALY,1,&now_dat);	
		time = 0;
		}	
	}
	
}



/*********************************************END OF FILE**********************/

