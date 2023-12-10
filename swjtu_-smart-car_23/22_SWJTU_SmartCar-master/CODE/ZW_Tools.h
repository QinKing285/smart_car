/*---------------------------------------------------------------------
                          ����ͼ�����ߺ���
													
��ƽ    ̨��RT1064
����    д�����Ļ�ϲ
����ϵ��ʽ��QQ��320388825 ΢�ţ�LHD0617_
�������¡�2022.03.16
������ƽ̨��MDK 5.28.0
����    �ܡ�����ͼ�����ߺ���
��ע�����������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;��
---------------------------------------------------------------------*/
#ifndef _ZW_TOOLS_H_
#define _ZW_TOOLS_H_

/*���������������������п�����Ҫ�������UART����ͷ�ļ��ɸ����Լ��Ŀ����и���*/
#include "headfile.h"


/*ͼ��ߴ��С		�мǱ�������λ����д�ĳߴ��Ӧ�����޷�ʹ��*/
/*ͼ��ߴ��С		���ó��� ������<=10000*/
#define		ZW_ImgSize_H		60
#define		ZW_ImgSize_W		90

/*�����ݶ���*/
#define		ZW_NULL					0

/*UART���ݷ��궨��	��Ҫ�û�����Ϊ�Լ���UART���ͺ���*/
#define 	ZW_Putchar(ZW_data)		uart_putchar(UART_0,ZW_data)

/*��������*/
typedef         unsigned char       ZW_uint8;
typedef         unsigned short      ZW_uint16;

/*��������*/
void ZW_Send_Image(ZW_uint8* ZW_Image);
void ZW_Send_Boundary(ZW_uint8* ZW_Left_Boundary, ZW_uint8* ZW_Right_Boundary);
extern uint8 ZW_Image[ZW_ImgSize_H][ZW_ImgSize_W];
void send_pc(void);
#endif


