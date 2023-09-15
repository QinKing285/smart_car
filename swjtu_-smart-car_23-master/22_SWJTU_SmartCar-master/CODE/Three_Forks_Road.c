/*
 * pid.c
 *
 *  Created on: 2022��1��13��
 *      Author: paper
 */
#include "Three_Forks_Road.h"
#define image_Y 120
#define image_X 180
#define True 1
#define False 0
#define my_abs(num) (((num)>0)?(num):-(num))
uint8 san_cha_tiaobian=0,san_cha_point=0,left_san_cha_flag=0,right_san_cha_flag=0,san_cha_L_chu_zhe_flag=0;
uint8 left_san_cha_hang=0,right_san_cha_hang=0,san_cha_flag=0;
uint8 left_san_cha_lose=0,right_san_cha_lose=0,left_san_cha_find=0,right_san_cha_find=0;
uint8 Number_of_san_cha=0,in_san_cha=0;
uint32 white_point_sum=0;
int16 white_point_L[image_Y];
int16 white_point_R[image_Y];
uint8 san_cha_da_L=0,san_cha_xiao_L=0;
uint8 san_cha_da_R=0,san_cha_xiao_R=0;
uint8 san_cha_ok_flag = 0,icm_san_cha_flag=0;
int32 icm_san_cha = 0;
int32 dutyru = 300000;
int32 dutychu = 380000;
void Three_Forks_Road_scan(void)
{
    //��һ���ж�
    if(san_cha_flag==0&&in_san_cha==0)
    {
        memset(white_point_L,0,sizeof(white_point_L));
        memset(white_point_R,0,sizeof(white_point_R));
        white_point_sum=0;
        left_san_cha_hang=0;
        left_san_cha_flag=False;
        right_san_cha_hang=0;
        right_san_cha_flag=False;
        left_san_cha_lose=0;
        right_san_cha_lose=0;
        //���²������յ�
        for(uint8 hang=image_Y-1;hang>endline;hang--)
        {
            if((leftline[hang-5]<leftline[hang])&&(leftline[hang+5]<leftline[hang]))
            {
                left_san_cha_hang=hang;
                if(left_san_cha_hang>image_Y*1/4)
                    left_san_cha_flag=True;
                //�ҵ����յ�
                break;
            }
        }
        for(uint8 hang=image_Y-1;hang>endline;hang--)
        {
            if((rightline[hang-5]>rightline[hang])&&(rightline[hang+5]>rightline[hang]))
            {
                right_san_cha_hang=hang;
                if(right_san_cha_hang>image_Y*1/4)
                    right_san_cha_flag=True;
                //�ҵ��Ҳ�յ�
                break;
            }
        }
        //����                   
        if(right_san_cha_flag==True&&left_san_cha_flag==True&&(my_abs(left_san_cha_hang-right_san_cha_hang)<=30))
        {
            //�жϹյ㸽����������ȱ仯����
            //��һ��������
            uint8 Three_Forks_Road[image_Y][image_X];
            for(uint8 hang=0;hang<image_Y;hang++)
            {
                for(uint8 lie=0;lie<image_X;lie++)
                {
                    Three_Forks_Road[hang][lie] = Threshold_Image[hang][lie];
                }
            }
            //���ȿ�ʼ���׿�
            for(uint8 hang=left_san_cha_hang;hang>0;hang--)
            {
                if(Threshold_Image[hang][image_X/2-1]==black)
                {
                    for(uint8 lie=image_X/2-1;lie>5;lie--)
                    {
                        if(Three_Forks_Road[hang][lie]==black)
                            Three_Forks_Road[hang][lie]=white;
                        else
                            continue ;
                    }
                }
            }
            for(uint8 hang=right_san_cha_hang;hang>0;hang--)
            {
                if(Threshold_Image[hang][image_X/2]==black)
                {
                    for(uint8 lie=image_X/2;lie<image_X-5;lie++)
                    {
                        if(Three_Forks_Road[hang][lie]==black)
                            Three_Forks_Road[hang][lie]=white;
                        else
                            continue ;
                    }
                }
            }
            //ɨ��������� �ӹյ�����ɨ��
            for(uint8 hang=left_san_cha_hang;hang>=left_san_cha_hang-25;hang--)
            {
                //���׵�
                for(uint8 lie=image_X/2;lie>5;lie--)
                {
                    if(Three_Forks_Road[hang][lie]==white)
                    {
                        white_point_L[hang]++;
                    }
                } 
            }
            for(uint8 hang=right_san_cha_hang;hang>=right_san_cha_hang-25;hang--)
            {
                //�Ҳ�׵�
                for(uint8 lie=image_X/2;lie<image_X-5;lie++)
                {
                    if(Three_Forks_Road[hang][lie]==white)
                    {
                        white_point_R[hang]++;
                    }
                }   
            }
            //ɨ������  (v2.0 ��Ϊ���������ҵ�)
            for(uint8 hang=left_san_cha_hang;hang>=left_san_cha_hang-25;hang--)//���µ���
            {
                //���
                if(white_point_L[hang]>white_point_L[hang-1])//��С
                {
                    san_cha_da_L++;
                }
                else if(white_point_L[hang]<white_point_L[hang-1])
                {
                    san_cha_xiao_L++;
                } 
            }
            for(uint8 hang=right_san_cha_hang;hang>=right_san_cha_hang-25;hang--)//���µ���
            {
                 //�Ҳ�
                if(white_point_R[hang]>white_point_R[hang-1])//��С
                {
                    san_cha_da_R++;
                }
                else if(white_point_R[hang]<white_point_R[hang-1])
                {
                    san_cha_xiao_R++;
                }
            }
            //���Ϊ��С�ٱ�������   �򵥵Ĵ���������������
            //if((san_cha_xiao_L>=7&&san_cha_xiao_R>=7)||(san_cha_xiao_L>=18&&san_cha_xiao_R>=3)||(san_cha_xiao_L>=3&&san_cha_xiao_R>=18))//||(san_cha_da_L>=8&&san_cha_xiao_L>=1)||(san_cha_da_R>=8&&san_cha_xiao_R>=1))
            if(san_cha_xiao_L>=7&&san_cha_xiao_R>=7)
            {
                san_cha_flag=1;//����ȷ��������������
            }
            san_cha_xiao_L=0;
            san_cha_da_L=0;
            san_cha_xiao_R=0;
            san_cha_da_R=0;
            
        }
    }
    //�����ж�,������������(�յ����϶���)
    else if(san_cha_flag==1)
    {
        //����
        left_san_cha_lose=0;
        right_san_cha_lose=0;
        san_cha_tiaobian=0;
        right_san_cha_find=0;
        left_san_cha_find=0;
        //���ߵ�
       for(uint8 hang=left_san_cha_hang;hang>0;hang--)
       {
           if(leftlineflag[hang]!=0&&leftlineflag[hang-1]==0)
             {
               left_san_cha_lose=1;
                 break;
             }
       }
       for(uint8 hang=right_san_cha_hang;hang>0;hang--)
       {
           if(rightlineflag[hang]!=0&&rightlineflag[hang-1]==0)
             {
               right_san_cha_lose=1;
                 break;
             }
       }
      //�Ƕ��ߵ㣨��ʮ�������֣�(�ݼ��Ƕ��ߣ�
        for(uint8 hang=left_san_cha_hang;hang>endline;hang--)
        {
            if(leftlineflag[hang]==2&&(leftline[hang]-leftline[hang+1]<0))
                left_san_cha_find++;
        }
        for(uint8 hang=right_san_cha_hang;hang>endline;hang--)
        {
            if(rightlineflag[hang]==2&&(rightline[hang]-rightline[hang+1]>0))
               right_san_cha_find++;
        }
      //���� ���� б�� �Ƕ���
       //if((right_san_cha_lose==1)&&(left_san_cha_lose==1))
       //{   
           //if(((left_san_cha_find>=13)&&(right_san_cha_find>=13))||((left_san_cha_find>=2)&&(right_san_cha_find>=18))||((left_san_cha_find>=18)&&(right_san_cha_find>=2)))
        if((left_san_cha_find>=8)&&(right_san_cha_find>=8))
        {
            san_cha_flag=2;
        }
        else
        {
            san_cha_flag=0;
        }
       //}
       // else
       // {
       //    san_cha_flag=0;
       // }
    }
}
uint8 real_endline=0;
void Three_Forks_Road_ru(void)
{
    if(san_cha_flag==2||san_cha_flag==3)
    {
        Buzzer(1);
        //������(���������ж�δ������)
        uint8 ulose=0;
        //�ж��Ƿ��Ѿ��������� 
        for(uint8 hang=image_Y-10;hang>image_Y/2;hang--)
        {
            if(leftlineflag[hang]==2&&rightlineflag[hang]==2)
            {
                ulose++;
            }
        }
        if(san_cha_flag==3)
        {  
            icm_san_cha_flag = 1;
            //if(ulose>=45)
            if(icm_san_cha>dutyru||icm_san_cha<-dutyru)
            {
                san_cha_flag=0;
                left_san_cha_find=0;
                right_san_cha_find=0;
                real_endline=0;
                Buzzer(0);
                in_san_cha=1; 
            }
        }
        //����Ѱ��
        for(uint8 hang=image_Y/2;hang>0;hang--)
        {
            if(leftlineflag[hang]==2&&rightlineflag[hang]==2)
            {
                ulose++;
            }
        }
        for(uint8 lie=15;lie<image_X-15;lie++)
        {
           if(Threshold_Image[real_endline][lie]==white&&Threshold_Image[real_endline][lie+1]==black)
               real_endline++;
        }
        real_endline--;//��ֹ����
        //endline = real_endline;
            //if(real_endline>10)
            if(ulose<40)
            {
                if(Number_of_san_cha==0) //��һ�ν������� ��ת
                {
                    san_cha_flag=3;
                    for(uint8 hang=endline;hang<image_Y;hang++)
                    {
                        rightlineflag[hang]=0;
                        if(leftlineflag[hang]==0)
                        {
                            leftlineflag[hang]=2;
                            leftline[hang]=1;
                        }
                    }
                }
                else//�ڶ�����ת
                //if(real_endline>10)
                //if(ulose<60)
                {
                    san_cha_flag=3;
                    for(uint8 hang=endline;hang<image_Y;hang++)
                    {
                    leftlineflag[hang]=0;
                    if(rightlineflag[hang]==0)
                        {
                            leftlineflag[hang]=2;
                            leftline[hang]=1;
                            // rightlineflag[hang]=2;
                            // rightline[hang]=image_X-1;
                        }
                    }
                }
            }
            else//ȡ�����Ƶ�֮�ϵ�Ѳ��,��ֹ������
            {
                for(uint8 hang = 0;hang<image_Y-40;hang++)
                {
                    rightlineflag[hang]=0;
                    leftlineflag[hang]=0;
                }
            }
            
     }
}
uint8 left_san_cha_chu_lost=0,right_san_cha_chu_lost=0;
uint8 encoder_san_cha_flag = 0;
int   encoder_san_cha = 0;
void Three_Forks_Road_chu(void)
{
    left_san_cha_flag=0;
    right_san_cha_flag=0;
    if(in_san_cha==1)
    {

        // for(uint8 hang=image_Y-1;hang>endline;hang--)
        // {
        //     if((leftline[hang-5]<leftline[hang])&&(leftline[hang+5]<leftline[hang]))
        //     {
        //         left_san_cha_hang=hang;
        //         if(left_san_cha_hang>image_Y*1/4)
        //             left_san_cha_flag=True;
        //         //�ҵ����յ�
        //         break;
        //     }
        // }
        // for(uint8 hang=image_Y-1;hang>endline;hang--)
        // {
        //     if((rightline[hang-5]>rightline[hang])&&(rightline[hang+5]>rightline[hang]))
        //     {
        //         right_san_cha_hang=hang;
        //         if(right_san_cha_hang>image_Y*1/4)
        //             right_san_cha_flag=True;
        //         //�ҵ��Ҳ�յ�
        //         break;
        //     }
        // }

        // if((my_abs(left_san_cha_hang-right_san_cha_hang)<=70)&&right_san_cha_flag==True&&left_san_cha_flag==True)
        // {
        //     uint8 Three_Forks_Road[image_Y][image_X];
        //     for(uint8 hang=0;hang<image_Y;hang++)
        //     {
        //         for(uint8 lie=0;lie<image_X;lie++)
        //         {
        //             Three_Forks_Road[hang][lie] = Threshold_Image[hang][lie];
        //         }
        //     }
        //     memset(white_point_L,0,sizeof(white_point_L));
        //     memset(white_point_R,0,sizeof(white_point_R));
        //     //�жϹյ㸽����������ȱ仯����
        //     //���ȿ�ʼ���׿�
        //     for(uint8 hang=left_san_cha_hang;hang>0;hang--)
        //     {
        //         if(Threshold_Image[hang][image_X/2-1]==black)
        //         {
        //             for(uint8 lie=image_X/2-1;lie>5;lie--)
        //             {
        //                 if(Three_Forks_Road[hang][lie]==black)
        //                     Three_Forks_Road[hang][lie]=white;
        //                 else
        //                     continue ;
        //             }
        //         }
        //     }
        //     for(uint8 hang=right_san_cha_hang;hang>0;hang--)
        //     {
        //         if(Threshold_Image[hang][image_X/2]==black)
        //         {
        //             for(uint8 lie=image_X/2;lie<image_X-5;lie++)
        //             {
        //                 if(Three_Forks_Road[hang][lie]==black)
        //                     Three_Forks_Road[hang][lie]=white;
        //                 else
        //                     continue ;
        //             }
        //         }
        //     }
        //         for(uint8 hang=0;hang<image_Y;hang++)
        //     {
        //         for(uint8 lie=0;lie<image_X;lie++)
        //         {
        //             Three_Forks_Road[hang][lie] = Threshold_Image[hang][lie];
        //         }
        //     }
        //     //ɨ��������� �ӹյ�����ɨ��
        //     for(uint8 hang=left_san_cha_hang;hang>=left_san_cha_hang-25;hang--)
        //     {
        //         //���׵�
        //         for(uint8 lie=image_X/2;lie>5;lie--)
        //         {
        //             if(Three_Forks_Road[hang][lie]==white)
        //             {
        //                 white_point_L[hang]++;
        //             }
        //         } 
        //     }
        //     for(uint8 hang=right_san_cha_hang;hang>=right_san_cha_hang-25;hang--)
        //     {
        //         //�Ҳ�׵�
        //         for(uint8 lie=image_X/2;lie<image_X-5;lie++)
        //         {
        //             if(Three_Forks_Road[hang][lie]==white)
        //             {
        //                 white_point_R[hang]++;
        //             }
        //         }   
        //     }
        //     //ɨ������  (v2.0 ��Ϊ���������ҵ�)
        //     san_cha_da_L = 0;
        //     san_cha_xiao_L = 0;
        //     san_cha_da_R = 0;
        //     san_cha_xiao_R = 0;
        //     for(uint8 hang=left_san_cha_hang;hang>=left_san_cha_hang-25;hang--)//���µ���
        //     {
        //         //���
        //         if(white_point_L[hang]>white_point_L[hang-1])//��С
        //         {
        //             san_cha_da_L++;
        //         }
        //         else if(white_point_L[hang]<white_point_L[hang-1])
        //         {
        //             san_cha_xiao_L++;
        //         } 
        //     }
        //     for(uint8 hang=right_san_cha_hang;hang>=right_san_cha_hang-25;hang--)//���µ���
        //     {
        //         //�Ҳ�
        //         if(white_point_R[hang]>white_point_R[hang-1])//��С
        //         {
        //             san_cha_da_R++;
        //         }
        //         else if(white_point_R[hang]<white_point_R[hang-1])
        //         {
        //             san_cha_xiao_R++;
        //         }
        //     }
        // }

    
        // //����������ﵽ ����Ѳ��
        // if(san_cha_xiao_L>=8&&san_cha_xiao_R>=8)
         //������,���������ǵĳ������㷨
         if((icm_san_cha<-dutychu&&Number_of_san_cha==0)||(Number_of_san_cha==1&&icm_san_cha<-dutychu))
        {        
            //������
            in_san_cha=3;
            Buzzer(1);
        }
  
    } 
    // else if(in_san_cha==2)
    // {
    //     uint8 ulose=0;
    //     for(uint8 hang=image_Y-10;hang>0;hang--)
    //     {
    //         if(leftlineflag[hang]==2&&rightlineflag[hang]==2)
    //         {
    //             ulose++;
    //         }
    //     }
    //     if(ulose<80)
    //     {
    //         icm_san_cha_flag = 1;
    //         in_san_cha=3;
    //     }
    
    else if(in_san_cha==3)
    {
        if((icm_san_cha>-dutychu*2/3&&Number_of_san_cha==0)||(Number_of_san_cha==1&&icm_san_cha>-dutychu*2/3))
        {
            encoder_san_cha_flag = 0;
            icm_san_cha_flag = 0;
            in_san_cha=0;
            right_san_cha_chu_lost=0;
            left_san_cha_chu_lost=0;
            Number_of_san_cha++;
            // if(Number_of_san_cha==2)
            // {
            //     Number_of_san_cha=0;
            // }
            san_cha_ok_flag = 1;
            Buzzer(0);
        }
        if(Number_of_san_cha == 0)
            {
                for(uint8 hang=endline;hang<image_Y;hang++)
                {
                    rightlineflag[hang] = 0;
                    if(leftlineflag[hang]==0)
                    {
                         leftlineflag[hang]=2;
                         leftline[hang]=1;
                    }
                }
            }
        else if(Number_of_san_cha == 1)
        {
           for(uint8 hang=endline;hang<image_Y;hang++)
                {
                    rightlineflag[hang] = 0;
                    if(leftlineflag[hang]==0)
                    {
                         leftlineflag[hang]=2;
                         leftline[hang]=1;
                    }
                }
        }
        // uint8 ulose=0;
        // for(uint8 hang=image_Y-10;hang>image_Y/2;hang--)
        // {
        //     if(leftlineflag[hang]==2&&rightlineflag[hang]==2)
        //     {
        //         ulose++;
        //     }
        // }
        //if(ulose>45)
        
    }
}
