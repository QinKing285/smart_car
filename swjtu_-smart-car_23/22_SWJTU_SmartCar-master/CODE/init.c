/*
 * init.c
 *
 *  Created on: 2022��1��6��
 *      Author: paper
 */
//��ʼ�����룬ֻ����һ��
#include "init.h"
void Init(void)
{
 
        get_clk();
        //��ʼ������������
        gpio_init(BEEP_PIN, GPO, 0, PUSHPULL);
        Buzzer(1);
        //������ʼ��
        gpio_init(KEY1,GPI,0,PULLUP);
        gpio_init(KEY2,GPI,0,PULLUP);
        gpio_init(KEY3,GPI,0,PULLUP);
        gpio_init(KEY4,GPI,0,PULLUP);
        //���뿪�س�ʼ��
        gpio_init(SW1,GPI,0,PULLUP);
        gpio_init(SW2,GPI,0,PULLUP);
        //��ʼ�����
        gtm_pwm_init(S_MOTOR_PIN, 50, duty);
        //��ʼ��PWM����
        gtm_pwm_init(MOTOR1_PWM, 17000, 0);
        gtm_pwm_init(MOTOR2_PWM, 17000, 0);
        gtm_pwm_init(MOTOR3_PWM, 17000, 0);
        gtm_pwm_init(MOTOR4_PWM, 17000, 0);
        //��ʼ������
        gpio_init(P10_5, GPI, 0, PULLDOWN);  //�������ų�ʼ��
        
        //��ʼ��������
        gpt12_init(GPT12_T5, GPT12_T5INB_P10_3, GPT12_T5EUDB_P10_1);
        gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);
        //��ʼ���ٶȻ�pid
        Init_pid_wheel();
        //��ʼ��ͼ��
        uart_init(UART_0, 2000000, UART0_TX_P14_0,UART0_RX_P14_1);
        //��ʼ�����ߴ���
        //seekfree_wireless_init(); 

        //��ʼ��tof
        Distance_measurement_Init();
        //��ʼ��ICM
        icm_int();
        //��ʼ������ͷ
        
        mt9v03x_init();
        Buzzer(0);
        //��ʼ����Ļ
        ips200_init();
        //��ʱ����ʼ��
        pit_interrupt_ms(CCU6_0, PIT_CH0, 2);
        //�ȴ����к��ĳ�ʼ�����
        IfxCpu_emitEvent(&g_cpuSyncEvent);
        IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
        //�������ж�
        enableInterrupts();
        //������ʵ�������
        track_width_real();
}



