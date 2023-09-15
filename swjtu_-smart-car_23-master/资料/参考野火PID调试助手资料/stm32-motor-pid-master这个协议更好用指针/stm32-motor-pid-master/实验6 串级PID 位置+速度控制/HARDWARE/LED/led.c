//*******************************
//STM32F407 ����LED����
//File: led.c
//Author: xxpcb(wxgzh:��ũ��ѧϰ)
//Version: V1.0
//Date: 2020/05/30
//*******************************

#include "led.h" 

//===========================================
//LED IO��ʼ��
//��ʼ��PA6��PA7Ϊ�����.��ʹ���������ڵ�ʱ��		    
//===========================================
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_LED, ENABLE);//ʹ��GPIOAʱ��

	//GPIOA6,A7��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LEDa | GPIO_Pin_LEDb;//LEDa��LEDb��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIO_LED, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_SetBits(GPIO_LED,GPIO_Pin_LEDa | GPIO_Pin_LEDb);//���øߣ�����
}
