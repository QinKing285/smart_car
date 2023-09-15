/*
 * init.c
 *
 *  Created on: 2022��11��18��
 *      Author: paper
 */

#include "init.h"
void Init(void) {
    //��ʼ������������
    gpio_init(BEEP_PIN, GPO, 0, PUSHPULL);
    Buzzer(1);
    //������ʼ��
    gpio_init(KEY1, GPI, 0, PULLUP);
    gpio_init(KEY2, GPI, 0, PULLUP);
    gpio_init(KEY3, GPI, 0, PULLUP);
    gpio_init(KEY4, GPI, 0, PULLUP);
    //���뿪�س�ʼ��
    gpio_init(SW1, GPI, 0, PULLUP);
    gpio_init(SW2, GPI, 0, PULLUP);
    //��ʼ��PWM����
    gtm_pwm_init(MOTOR1_PWM, 12500, 0);//����ǰ_��ˢ���PWM����
    gtm_pwm_init(MOTOR2_PWM, 12500, 0);//�����_��ˢ���PWM����
    gtm_pwm_init(MOTOR3_PWM, 12500, 0);//������ˢ���PWM����

    //��ʼ��������
    gpt12_init(GPT12_T5, GPT12_T5INB_P10_3, GPT12_T5EUDB_P10_1);
    gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);
}