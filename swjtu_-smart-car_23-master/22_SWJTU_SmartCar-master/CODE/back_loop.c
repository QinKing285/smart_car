/*
 * cross.c
 *
 *  Created on: 2022��7��3��
 *      Author: paper
 */

//�ػ�ͼ���복������ �°벿��ʶ��Ϊ����Ѱ������,���϶�������Ŀ�ж�,ת�����Ƴ���(�����½��в���)
//��Ϊʡ�������̶� �����㷨�ж����� (����˼·)
#include <back_loop.h>
#define BACK_LOOP_sensitivity 30  //�ػ�������,ԽСԽ����
unsigned char back_loop_flag = 0,icm_back_loop_flag=0;
unsigned char white_line_hang = 0;
unsigned char white_line_point = 0;
unsigned char back_loop_find_L = 0;
unsigned char back_loop_lose_L = 0;
unsigned char back_loop_find_R = 0;
unsigned char back_loop_lose_R = 0;
unsigned char first_way = 0;//0 �� 1 ��
float K_back_loop = 5;
int back_loop_range = 300000,icm_back_loop=0,number_of_back_loop = 0;

#define image_Y 120
#define image_X 180
void back_loop_find(void)
{
    if((number_of_back_loop==0)||(number_of_back_loop==1&&Number_of_san_cha==1&&number_of_ring==1))
    {
            //�����·�ͻ��
        //��ʼ״̬ ��������Ѳ��
        back_loop_find_L = 0;
        back_loop_lose_L = 0;
        back_loop_find_R = 0;
        back_loop_lose_R = 0;
        for(unsigned char hang = image_Y-1;hang >= frontline;hang--)
        {
            if(leftlineflag[hang]!=0)
            {
                back_loop_find_L ++;
            }
            else if(leftlineflag[hang]==0)
            {
                back_loop_lose_L ++;
            }
            if(rightlineflag[hang]!=0)
            {
                back_loop_find_R ++;
            }
            else if(rightlineflag[hang]==0)
            {
                back_loop_lose_R ++;
            }
        } 
        //��һ״̬ �ж�����
        if((number_of_back_loop == 0&&back_loop_lose_L>=(image_Y-frontline)*1/7)&&(back_loop_lose_R>=(image_Y-frontline)*1/7)&&(back_loop_find_L<=(image_Y-frontline)*6/7)&&(back_loop_find_R<=(image_Y-frontline)*6/7))//&&frontline>image_Y*1/7)
        {
            Buzzer(1);
            back_loop_flag = 1;
        }
        else if((number_of_back_loop == 1&&back_loop_lose_L>=(image_Y-frontline)*1/6)&&(back_loop_lose_R>=(image_Y-frontline)*1/6)&&(back_loop_find_L<=(image_Y-frontline)*5/6)&&(back_loop_find_R<=(image_Y-frontline)*5/6)&&frontline>image_Y*2/7)
        {
            Buzzer(1);
            back_loop_flag = 1;
        }
        else if((frontline>image_Y*1/3)&&back_loop_find_L==0&&back_loop_lose_L==(image_Y-frontline)&&back_loop_lose_R>image_Y*1/6)
        {
            back_loop_flag = 1;
            Buzzer(1);
        }
        else if((frontline>image_Y*1/3)&&back_loop_find_R==0&&back_loop_lose_R==(image_Y-frontline)&&back_loop_lose_L>image_Y*1/6)
        {
            back_loop_flag = 1;
            Buzzer(1);
        }
        else
        {
            //back_loop_flag = 0;
            Buzzer(0);
        }
    }
    
}
//���ػ�.˼·�ͳ�������
int leftline_back_loop =0,rightline_back_loop=0;
void back_loop_chu(void)
{
    if(first_way&&back_loop_flag == 1&&number_of_ring!=0)//�����
    {
        //����
        for(uint8 hang = image_Y-1;hang>frontline;hang--)
        {
            leftline[hang] = 0;
            rightline_back_loop =  180 - frontline*K_back_loop;
            if(rightline_back_loop <= 0 )
            {
                rightline_back_loop = 0;
            }
            rightline[hang] = rightline_back_loop;
            leftlineflag[hang] = 2;
            rightlineflag[hang] = 2;
        }
        //�ۼƽǶ�
        icm_back_loop_flag = 1;
        if(icm_back_loop > back_loop_range)
        {
            Buzzer(0);
            number_of_back_loop++;
            back_loop_flag = 0;
            icm_back_loop_flag = 0;
            first_way=2;
        }
    }
    else if(!first_way&&back_loop_flag == 1)//�ҳ���
    {
        //����
        for(uint8 hang = image_Y-1;hang>frontline;hang--)
        {
            rightline[hang] = 180 ;
            leftline_back_loop = frontline*K_back_loop;
            if(leftline_back_loop>=179)
            {
                leftline_back_loop = 179;
            }
            leftline[hang] = leftline_back_loop;
            
            leftlineflag[hang] = 2;
            rightlineflag[hang] = 2;
        }
        //�ۼƽǶ�
        icm_back_loop_flag = 1;
        if(icm_back_loop < -back_loop_range)
        {
            Buzzer(0);
            number_of_back_loop++;
            back_loop_flag = 0;
            icm_back_loop_flag = 0;
            first_way=1;
        }
    }
}
