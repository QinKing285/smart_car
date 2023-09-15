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

//PWMֵ����
static int pwm_val_protect(int pwm_input)
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
	else if((pwm_input>-50)&&(pwm_input<50)) 
	{
		pwm_output = 0;
	}
	else
	{
		pwm_output = pwm_input;
	}
	
	return pwm_output;
}

//Ŀ���ٶ�ֵ����
//#define TARGET_SPEED_MAX   60 // Ŀ���ٶȵ����ֵ r/m
static float TARGET_SPEED_MAX = 60.0;
static void speed_val_protect(float *speed_val)
{
	/*Ŀ���ٶ����޴���*/
	if (*speed_val > TARGET_SPEED_MAX)
	{
		*speed_val = TARGET_SPEED_MAX;
	}
	else if (*speed_val < -TARGET_SPEED_MAX)
	{
		*speed_val = -TARGET_SPEED_MAX;
	}	
}

/*��λ��Ŀ��ֵ��ȡ������*/
void SetTargetMaxSpeed(int speed)
{
	TARGET_SPEED_MAX = (float)speed;
}
int GetTargetMaxSpeed(void)
{
	return (int)TARGET_SPEED_MAX;
}


extern __IO int16_t EncoderOverflowCnt;

int temp=0;
//���ڶ�ʱ���Ļص�����
void AutoReloadCallback()
{
	static uint32_t location_timer = 0;    // λ�û�����
	
	static __IO int encoderNow = 0;    /*��ǰʱ���ܼ���ֵ*/
    static __IO int encoderLast = 0;   /*��һʱ���ܼ���ֵ*/
	int encoderDelta = 0; /*��ǰʱ������һʱ�̱������ı仯��*/
	float actual_speed = 0;  /*ʵ�ʲ���ٶ�*/
	int actual_speed_int = 0;
	
	int res_pwm = 0;/*PID����õ���PWMֵ*/
	static int i=0;
	
	/*��1����ȡ��������ֵ*/
	encoderNow = read_encoder() + EncoderOverflowCnt*ENCODER_TIM_PERIOD;/*��ȡ��ǰ���ۼ�ֵ*/
	encoderDelta = encoderNow - encoderLast; /*�õ��仯ֵ*/
	encoderLast = encoderNow;/*�����ϴε��ۼ�ֵ*/
	
	/*��2��λ��PID���㣬�õ�PWM����ֵ*/
	if ((location_timer++ % 2) == 0)
	{
		float control_val = 0;   /*��ǰ����ֵ*/
		
		/*λ��PID����*/
		control_val = location_pid_realize(&pid_location, encoderNow);  
		
        /*Ŀ���ٶ�ֵ����*/
		speed_val_protect(&control_val);

		/*�趨�ٶ�PID��Ŀ��ֵ*/
		set_pid_target(&pid_speed, control_val);    

		#if defined(PID_ASSISTANT_EN)
		if ((location_timer % 16) == 8)
		{
			temp = (int)control_val;
			set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &temp, 1);     // ��ͨ�� 2 ����Ŀ��ֵ
		}
		#endif
	}
	  
	/* ת��(1����ת����Ȧ)=��λʱ���ڵļ���ֵ/�ֱܷ���*ʱ��ϵ��, �ٳ�60��Ϊ1����ת����Ȧ */
    actual_speed = (float)encoderDelta / TOTAL_RESOLUTION * 10 * 60;
    
	/*��3���ٶ�PID���㣬�õ�PWM����ֵ*/
	actual_speed_int = actual_speed;
	res_pwm = pwm_val_protect((int)speed_pid_realize(&pid_speed, actual_speed));
	
	/*��4��PWM���Ƶ��*/
	set_motor_rotate(res_pwm);
	
	/*��5�������ϴ�����λ����ʾ*/
#if (PID_ASSISTANT_EN)
	i++; 
	if(i%12 == 5)
	{
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &encoderNow, 1);   /*��ͨ��1����ʵ�ʵĵ����λ�á�ֵ*/
	}
//	else if(i%12 == 10)
//	{
//		set_computer_value(SEND_FACT_CMD, CURVES_CH2, &actual_speed_int, 1); /*��ͨ��2����ʵ�ʵĵ�����ٶȡ�ֵ*/
//	}
	
#else
	i++;
	if(i==100)
	{
		i=0;
		printf("ʵ��ֵ�ٶȣ�%d. ʵ��λ��ֵ��%d, Ŀ��λ��ֵ��%.0f\n", actual_speed, encoderNow, get_pid_target(&pid_location)); 
		
	}
#endif
}



//��ʱ��7�жϷ�����
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
	{
		AutoReloadCallback();
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //����жϱ�־λ
	}
}
