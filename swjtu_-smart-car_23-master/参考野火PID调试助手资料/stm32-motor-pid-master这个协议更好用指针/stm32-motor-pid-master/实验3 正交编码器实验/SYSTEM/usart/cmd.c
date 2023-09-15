#include "cmd.h"
#include "sys.h"
#include "usart.h"
#include "motor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint8_t Recv[128];//���ڽ��ջ���
u8 rx_cnt=0;//�������ݸ�����������

int count=0;
int size=50;
uint8_t buf[64];//���ֻȡǰ64������


#define N 2
typedef struct struct_dostr
{
	char name[32];
	int (*fun)(char *argv[]);
}struct_dostr;

static int hello(char* p[]);
static int pwm(char* p[]);
	
struct_dostr struct_dostr1[N]={
	{"hello",hello},
	{"pwm",  pwm},	
};

static int hello(char* p[])
{
	printf("hello~~~~~~~~~~\r\n");
	return 0;
}

static int pwm(char* p[])
{
	int p0;
	p0=atoi(p[0]);
	
	if(p0 > 1000) p0 = 1000;
	if(p0 < -1000) p0 = -1000;
	
	printf("set pwm: %d\r\n",p0);
	set_motor_rotate(p0);
	
	return 0;
}

static void copy_uart_data()
{
	//��ձ��ؽ�������
	memset(buf,0,size);
	
	//printf("%s",Data);
	if(rx_cnt < size)//�յ������ݳ�����size��Χ��
	{
		//void *memcpy(void *str1, const void *str2, size_t n)  
		//�Ӵ洢�� str2 ���� n ���ֽڵ��洢�� str1��
		memcpy(buf,Recv,rx_cnt);//�м������Ƽ���
		count=rx_cnt;
		//printf("%s\r\n", buf);
	}
	else//�յ������ݳ���̫����
	{
		memcpy(buf,Recv,size);//ֻ����size��
		count=size;
	}
	rx_cnt=0;
}

static void process_cmd_data()
{
	
	//���ж�ָ������
	char *cmd;//��ʾ����
	char *paras;//��ʾ�����Ĳ���
	char *ret;
	int i;
	cmd = strtok_r((char*)buf, " ", &paras);
//	printf("receive:%s\r\n",buf);
	for (i = 0; i < N;i++)
	{
		ret = strstr(cmd, struct_dostr1[i].name);
		if(ret!=NULL)
		{
//			printf("find cmd in funname[%d]\r\n", i);
			break;
		}
	}
//	printf("i:%d\r\n",i);
//	printf("cmd:[%s]\r\n", cmd);
//	printf("paras:%s\r\n", paras);
	if(i==N)
	{
		printf("can't find cmd in funname[]\r\n");
	}
	else
	{				
		//����Ч��ָ������жϺ�������
		char* para[4]={0};//�޶�������4������
		int j= 0;
		while((para[j]=strtok(paras," "))!= NULL)//�ĳ������Ϳ�����
		{
			j++;
			paras=NULL;
			if(j==4)
				break;
		}
//		printf("paras nums:%d\r\n",j);
//		if(j>0)printf("para[0]:%s\r\n", para[0]);
//		if(j>1)printf("para[1]:%s\r\n", para[1]);
//		if(j>2)printf("para[2]:%s\r\n", para[2]);
//		if(j>3)printf("para[3]:%s\r\n", para[3]);
		
		//ִ�ж�Ӧ�ĺ���
		struct_dostr1[i].fun(para);
	}
}



void process_uart_data()
{
	copy_uart_data();
	process_cmd_data();
}


