#include "bsp_pid.h"
#include "protocol.h"
#include "common.h"

/*����һ��PID�ṹ���͵�ȫ�ֱ���*/
PID pid;

/**
  * @brief  PID������ʼ��
  *	@note 	��
  * @retval ��
  */
void PID_param_init()
{
	float pid_temp[3];

	/* ��ʼ������ */
	pid.target_val=0.0;				
	pid.output_val=0.0;
	pid.err=0.0;
	pid.err_last=0.0;
	pid.integral=0.0;

	pid.Kp = 0.0;
	pid.Ki = 0.0;
	pid.Kd = 0.0;

#if defined(PID_ASSISTANT_EN)
	pid_temp[0] = pid.Kp;
	pid_temp[1] = pid.Ki;
	pid_temp[2] = pid.Kd;
	set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ��1����PIDֵ
#endif
}

extern float det_plus;
/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
  *	@note 	��
  * @retval ��
  */
void set_pid_target(float temp_val)
{  
	pid.target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
  *	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(void)
{
	return pid.target_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
  *	@note 	��
  * @retval ��
  */
void set_p_i_d(float p, float i, float d)
{
	pid.Kp = p;    // ���ñ���ϵ�� P
	pid.Ki = i;    // ���û���ϵ�� I
	pid.Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float PID_realize(float actual_val)
{
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
	pid.err = pid.target_val - actual_val;
	
	/*������*/
	pid.integral += pid.err;

	/*PID�㷨ʵ��*/
	pid.output_val = pid.Kp * pid.err + 
				     pid.Ki * pid.integral + 
				     pid.Kd * (pid.err - pid.err_last);

	/*����*/
	pid.err_last = pid.err;

	/*���ص�ǰʵ��ֵ*/
	return pid.output_val;
}


