/*
 * init.h
 *
 *  Created on: 2022��1��6��
 *      Author: paper
 */

#ifndef CODE_INIT_H_
#define CODE_INIT_H_
#include "headfile.h"
void Init(void);
#define BEEP_PIN   P33_10       //�������������
#define MOTOR1_PWM   ATOM0_CH0_P21_2    //����1���PWM����
#define MOTOR2_PWM   ATOM0_CH2_P21_4    //����2���PWM����
#define MOTOR3_PWM   ATOM0_CH3_P21_5    //����3���PWM����
#define MOTOR4_PWM   ATOM0_CH1_P21_3    //����4���PWM����
#define S_MOTOR_PIN   ATOM1_CH1_P33_9       //����������
#endif /* CODE_INIT_H_ */
#define BEEP_PIN   P33_10       //�������������


// #define MOTOR1_DIR   P21_2              //����1�������������� ����  in12
// #define MOTOR1_PWM   ATOM0_CH1_P21_3    //����1���PWM����  ���� in11
 
// #define MOTOR2_DIR   P21_4              //����2��������������  ����
// #define MOTOR2_PWM   ATOM0_CH3_P21_5    //����2���PWM����   ����
