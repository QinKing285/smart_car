/*---------------------------------------------------------------------
                          ����ͼ�����ߺ���
													
��ƽ    ̨��RT1064
����    д�����Ļ�ϲ
����ϵ��ʽ��QQ��320388825 ΢�ţ�LHD0617_
�������¡�2021.11.30
������ƽ̨��MDK 5.28.0
����    �ܡ�����ͼ�����ߺ���
��ע�����������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;��
---------------------------------------------------------------------*/
/*����ͷ�ļ�*/
#include "ZW_Tools.h"

/************************************************************
												 ����ͼ��
								
���������ơ�ZW_Send_Image
����    �ܡ�ͼ���ͺ���
����    ����ZW_Image��ֵ��ͼ�����飨��0��1���Ķ�ֵ��ͼ��
���� �� ֵ����
��ʵ    ����ZW_Send_Image(mt9v03x_csi_image);
��ע�������ֻ�ܴ����ֵ��ͼ�񣨷�0��1��
						��.h�ļ��к궨��ߴ��������λ������д�ĳߴ��Ӧ�����޷�ʹ��
						�۸ú��������ͼ��������±���������λ������

************************************************************/
uint8 ZW_Image[ZW_ImgSize_H][ZW_ImgSize_W];
void ZW_Send_Image(ZW_uint8* ZW_Image)
{
	ZW_uint8 ZW_data;
	ZW_uint16 ZW_i;
	ZW_uint8 ZW_j;

	for(ZW_i = 0; ZW_i < ZW_ImgSize_H*ZW_ImgSize_W; ZW_i += 8)
	{
		ZW_data = 0;
		for(ZW_j = 0; ZW_j < 8; ZW_j++)
		{
			if(*(ZW_Image + ZW_i + ZW_j))		ZW_data |= 0x01 << (7-ZW_j);
		}
		ZW_Putchar(ZW_data);
	}
}
void send_pc(void)
{
    // ��������
    float ch[4];
    float a = 0;
    a = (Speed_Left+Differential_speed_1);
    ch[0] =  (float) Error;
    ch[1] =  (float) yawspeed;
	ch[2] =  (float)leftline_Curvature;
    ch[3] = (float) a;

	seekfree_wireless_send_buff((char *)ch, sizeof(float) * 4);
    // ����֡β
    char tail[4] = {0x00, 0x00, 0x80, 0x7f};
	seekfree_wireless_send_buff(tail, 4);
}

