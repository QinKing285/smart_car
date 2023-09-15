#include "capture.h"

/**
* @brief TIM5 ͨ��1���벶������ 
* @param arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
*        psc��ʱ��Ԥ��Ƶ��
*/
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;            /*GPIO �ṹ��*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; /*ʱ�� �ṹ��*/
	TIM_ICInitTypeDef  TIM5_ICInitStructure;        /*����ͨ�� �ṹ��*/
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	/*�����źŵ�GPIO��ʼ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        /*���ù���*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      /*����*/
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0����λ��ʱ��5
  
	/*ʱ����ʼ��*/
	TIM_TimeBaseStructure.TIM_Period=arr;     /* �Զ���װ��ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  /* ��ʱ����Ƶ */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	/*����ͨ����ʼ������ʼ��TIM5���벶�����*/
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;                //CC1S=01 ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 /* �����ز��� */
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	

	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5

    /*��ʱ���ж�����*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                     	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}


/* ����״̬ TIM5CH1_CAPTURE_STA
*  [7]: 0,û�гɹ��Ĳ���  
*       1,�ɹ�����һ��
*  [6]: 0,��û���񵽵͵�ƽ
*       1,�Ѿ����񵽵͵�ƽ��
*  [5:0]: 00000~11111,����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
*/
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬������һ�����ƵļĴ���ʹ�ã���ʼΪ0��	

u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

/**
* @brief ��ʱ��5�жϷ������
*/
void TIM5_IRQHandler(void)
{ 		
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	(1000 0000)
	{
		/*��ʱ������ж�*/
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)/* ֮ǰ����˿�ʼ�ź�(0100 0000) */
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F) /* �ߵ�ƽ̫����,��������� (0011 1111) */
				{
					TIM5CH1_CAPTURE_STA|=0X80;		 /* (ǿ��)��ǳɹ�������һ�� (1000 0000) */
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;  /* ��Ϊ�������N�����ټ��ˣ��ͽ���ǰ�Ĳ���ֵ����Ϊ32λ�����ֵ����ЧNmax+1*/
				}
				else /* ��������ǲ�����������յó���ȷ�ĸߵ�ƽʱ�� */
				{
					TIM5CH1_CAPTURE_STA++; /* �ۼƶ�ʱ���������N */
				}
			}
			else
			{
				/* ��û�в����ź�ʱ����ʱ�������ʲôҲ�������Լ���������������� */
			}
		}
		
		/*����1���������¼�*/
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
		{	
			/*����һ���½���(�����ź�)*/
			if(TIM5CH1_CAPTURE_STA&0X40) /* ֮ǰ����˿�ʼ�ź�(0100 0000) */		 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		                   /* ��ǳɹ�����һ�θߵ�ƽ���� (1000 0000) */
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);         /* ��ȡ��ǰ�Ĳ���ֵ */
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); /* CC1P=0 ��������Ϊ�����ز��������´β�׽�ź� */
			}      
			/*��δ��ʼ,��һ�β��� ������(��ʼ�ź�) */
			else  								
			{
				TIM5CH1_CAPTURE_STA=0;			/* ��� ����״̬�Ĵ��� */
				TIM5CH1_CAPTURE_VAL=0;          /* ��� ����ֵ */
				TIM5CH1_CAPTURE_STA|=0X40;		/* ��ǲ����������� (0100 0000) */
				
				TIM_Cmd(TIM5,DISABLE ); 	    /* �رն�ʱ��5 */
	 			TIM_SetCounter(TIM5,0);         /* ���CNT�����´�0��ʼ���� */
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);	/* CC1P=1 ����Ϊ�½��ز��� */
				TIM_Cmd(TIM5,ENABLE ); 	        /* ʹ�ܶ�ʱ��5 */
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}
