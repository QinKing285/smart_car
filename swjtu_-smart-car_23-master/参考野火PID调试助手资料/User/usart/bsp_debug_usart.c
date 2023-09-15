/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

/****************************ʹ�õ�һ�±���***********************************/
//���ڽ�������
unsigned char UART_RxBuffer[UART_RX_BUFFER_SIZE];
//���ڽ�������ָ��
unsigned char UART_RxPtr;

PIDC Pid_Test_C;
/* ���������� */

uint8_t receive_cmd = 0,recevie_data_length=11;

/***************************************************************/
 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

  /* ʹ�� USART ʱ�� */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
  
  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  /* �ֳ�(����λ+У��λ)��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(DEBUG_USART, &USART_InitStructure); 
	
  /* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
  
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE,ENABLE);
	USART_ITConfig(DEBUG_USART,	USART_IT_IDLE,ENABLE);	
	
  /* ʹ�ܴ��� */
  USART_Cmd(DEBUG_USART, ENABLE);
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************************������ͨ��Э��ĺ�����Ҫ��ֲ�Ļ�ֱ�Ӵ�������оͺ�**********************************************************************/
///****************************************************************************
/// @data     :         ���ͽṹ�庯��                                                        
/// @input    :                                                                 
/// @output   :                                                                 
/// @brief    :                                                                 
///****************************************************************************
void Usart_SendStruct(USART_TypeDef * pUSARTx, uint8_t *str,uint8_t length)
{
	static uint8_t i=0;
	USART_ClearFlag(pUSARTx,USART_FLAG_TC); 
	for(i=0;i<length;i++)
	{
		USART_SendData(pUSARTx,str[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	}
}
///****************************************************************************
/// @data     :            У��ͼ��麯����Ұ��д�ģ�����                                                     
/// @input    :                                                                 
/// @output   :                                                                 
/// @brief    :                                                                 
///****************************************************************************
uint8_t Calculate_CheckSum(uint8_t Sum_init, uint8_t *ptr, uint8_t len)
{
	uint8_t sum=Sum_init;
	
	while(len--)
	{
		sum += *ptr;
		ptr++;
	}
	
	return sum;
}
///****************************************************************************
/// @data     :       �ӻ��������ݸ�����ʹ�õ�                                                          
/// @input    :                                                                 
/// @output   :                                                                 
/// @brief    :       float���ĸ��ֽ�                                                          
///****************************************************************************
void Send_SlaveTOHost(uint8_t channal,uint8_t command,uint8_t parameter_Number,void *data)
{
	static Packed Send_Pack;
	uint8_t pack_sum=0;
	
	parameter_Number *= 4;
	
	Send_Pack.Pack_Channel=channal;
	Send_Pack.Pack_Command=(uint8_t)command;
	Send_Pack.Pack_Length=(0x0B+parameter_Number);//������
	Send_Pack.Pack_Start=PACK_SART_VALUE;
//	Send_Pack.Pack_Sum=0;
	
	pack_sum=Calculate_CheckSum(0,(uint8_t *)&Send_Pack,sizeof(Send_Pack));//��ͷУ���
	//���ݽṹ���������֪���������һ��sum�Ĵ�С
	pack_sum=Calculate_CheckSum(pack_sum,(uint8_t *)&data,parameter_Number);//����У���
	
	Usart_SendStruct(USART1,(uint8_t *)&Send_Pack,sizeof(Send_Pack));//��������ͷ
	Usart_SendStruct(USART1,(uint8_t *)data,parameter_Number);//���Ͳ���
	Usart_SendStruct(USART1,(uint8_t *)&pack_sum,sizeof(pack_sum));//����У���
	
}
///****************************************************************************
/// @data     :       �ӻ�������������ʹ��                                                          
/// @input    :       �ĸ�U8���� �ϲ���һ��float���ݣ���Ҫ��ѧϰ�������ʹ��                                                          
/// @output   :                                                                 
/// @brief    :                                                                 
///****************************************************************************
void u8TOFloat(uint8_t Array_Target,float *PID)
{
	uint8_t j=0,cnt=0;
	Union_Data Receive_St;
	
	cnt=Array_Target;
	for(j=0;j<4;j++)
		Receive_St.Rec_data[j]=UART_RxBuffer[cnt++];
	//���յ������ݣ�С����ǰ��
	
		*PID=Receive_St.flo;
}
///****************************************************************************
/// @data     :     �����������ݸ��ӻ����ӻ�������ʱ��ʹ�õ�                                                            
/// @input    :                                                                 
/// @output   :                                                                 
/// @brief    :                                                                 
///****************************************************************************
int8_t Receive_HostTOSlave(uint8_t *counter)
{
	Packed Recevie_Packed;
	
	Recevie_Packed.Pack_Command	=UART_RxBuffer[CMD_INDEX_VAL];
	//���ܵ������ݣ��ھ�λ�����������һ�ּ򻯵�д��
	Recevie_Packed.Pack_Length	=COMPOUND_32BIT(&UART_RxBuffer[LEN_INDEX_VAL]);//�ϳɳ���
	Recevie_Packed.Pack_Start		=COMPOUND_32BIT(&UART_RxBuffer[HEAD_INDEX_VAL]);//�ϳɰ�ͷ
	
//	*counter=Recevie_Packed.Pack_Length;
	recevie_data_length +=(Recevie_Packed.Pack_Length-0x0B);
	//��������ܹ�ռ����8λ����
				
		*counter=0;
		if(Recevie_Packed.Pack_Start == PACK_SART_VALUE)
		{
			if(Calculate_CheckSum(0,UART_RxBuffer,Recevie_Packed.Pack_Length -1) == UART_RxBuffer[Recevie_Packed.Pack_Length-1])
			{		
				/*��������������������ݲ�ͬ��������Ӳ�ͬ�Ĵ������ͺá�*/
				switch(Recevie_Packed.Pack_Command)
				{
					case SET_SLAVE_PID:
						LED_YELLOW;		
						u8TOFloat(P_INDEX_VAL,&Pid_Test_C.KP);
						u8TOFloat(I_INDEX_VAL,&Pid_Test_C.KI);
						u8TOFloat(D_INDEX_VAL,&Pid_Test_C.KD);
						Send_SlaveTOHost(HOST_CHANNAL_1,SET_HOST_PID_PARAMETER,4,&Pid_Test_C);
						break;
					
					case SET_SLAVE_TARGET:
						LED_BLUE;
						break;
					
					case SET_SLAVE_START:
						LED_GREEN
						break;
					
					case SET_SLAVE_STOP:
						LED_CYAN;
						break;
					
					case SET_SLAVE_PERIOD:
						LED_WHITE;
						break;
					
					case SET_SLAVE_RESET:
						LED_RED;
						break;				
				}		
			}		
		}
		
	return 1;
}
/*********************************************END OF FILE**********************/
