//*******************************
//STM32F407 ����LED����
//File: led.h
//Author: xxpcb(wxgzh:��ũ��ѧϰ)
//Vesion: V1.0
//Date: 2020/05/30
//*******************************

#ifndef __LED_H
#define __LED_H
#include "sys.h"

//=========================
//��������LED,�͵�ƽ����
//A6:D2(������ΪLEDa)
//A7:D3(������ΪLEDb)
//=========================

//LED�˿ڶ���
#define RCC_AHB1Periph_LED RCC_AHB1Periph_GPIOA
#define GPIO_LED GPIOA

#define GPIO_Pin_LEDa GPIO_Pin_6
#define GPIO_Pin_LEDb GPIO_Pin_7

#define digitalHigh(p,i)     {p->BSRRH=i;}  //��������ߵ�ƽ       
#define digitalLow(p,i)      {p->BSRRL=i;}   //��������͵�ƽ
#define digitalToggle(p,i)   {p->ODR ^=i;} //��ת

#define LEDa_OFF      digitalLow(GPIO_LED,GPIO_Pin_LEDa)
#define LEDb_OFF      digitalLow(GPIO_LED,GPIO_Pin_LEDb)

#define LEDa_ON     digitalHigh(GPIO_LED,GPIO_Pin_LEDa)
#define LEDb_ON     digitalHigh(GPIO_LED,GPIO_Pin_LEDb)

#define LEDa_Toggle  digitalToggle(GPIO_LED,GPIO_Pin_LEDa)
#define LEDb_Toggle  digitalToggle(GPIO_LED,GPIO_Pin_LEDb)

void LED_Init(void);//��ʼ��		 				    
#endif
