/*
 * key.h
 *
 *  Created on: 2022��1��6��
 *      Author: paper
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_
#include "headfile.h"
//���尴������
#define KEY1    P22_0
#define KEY2    P22_1
#define KEY3    P22_2
#define KEY4    P22_3
//���岦�뿪������
#define SW1     P33_12
#define SW2     P33_13
void Key_scan(void);
void Buzzer(uint8 value);
void Encoder_scan(void);
extern uint8 sw1_status;
extern uint8 sw2_status;
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;

extern float encoder_left,encoder_left_speed,encoder_right_speed;
extern float encoder_right;
extern double encoder_all,encoder_all_home,encoder_all_right,encoder_all_left;
#endif /* CODE_KEY_H_ */
