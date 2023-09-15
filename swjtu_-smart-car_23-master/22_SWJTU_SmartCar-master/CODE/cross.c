/*
 * cross.c
 *
 *  Created on: 2022��2��26��
 *      Author: paper
 */
#include "cross.h"
#define my_abs(num) (((num)>0)?(num):-(num))
uint8 left_lost_dn,left_lost_up,right_lost_dn,right_lost_up,cross_xie;
int8 left_lost_line,right_lost_line;
uint8 cross_flag = 0;

void cross_find(void)
{
    //����ɨ��
    
        //����
        left_lost_dn=0,left_lost_up=0,right_lost_dn=0,right_lost_up=0;
        //������
        for(uint8 hang=image_Y-5;hang>endline;hang--)
        {
            if(leftlineflag[hang] != 0 && leftlineflag[hang-1] == 0)
            {
                left_lost_dn = hang;
                break;
            }
        }
        //������
        if(left_lost_dn)
        {
            for(uint8 hang=0;hang<left_lost_dn;hang++)
            {
                if(leftlineflag[hang] != 0 && leftlineflag[hang+1] == 0)
                {
                    left_lost_up = hang;
                    break;
                }
            }
        }
        ////////////////////////////////////
        //����
        for(uint8 hang=image_Y-5;hang>endline;hang--)
        {
            if(rightlineflag[hang] != 0 && rightlineflag[hang-1] == 0)
            {
                right_lost_dn = hang;
                break;
            }
        }
        //������
        if(right_lost_dn)
        {
            for(uint8 hang=0;hang<left_lost_dn;hang++)
            {
                if(rightlineflag[hang] != 0 && rightlineflag[hang+1] == 0)
                {
                    right_lost_up = hang;
                    break;
                }
            }
        }
        ///����
        if(right_lost_up&&left_lost_up)
        {
            left_lost_line =  left_lost_dn - left_lost_up;
           
            right_lost_line =  right_lost_dn - right_lost_up;
            

            //�ж϶���������
            if(my_abs(left_lost_line-right_lost_line)<=10&&left_lost_line>10&&right_lost_line>10)
            {
                cross_flag =1;
                
            }
            else
            {
                cross_flag = 0;
            
            } 
        }
        // //б��ʮ�֣����ң�
        // else if(right_lost_up&&(left_lost_up==0))
        // {
        //     //����
        //     cross_xie = 0;
            
        //     //��α���·����ߵ��²�������������
        //     for(uint8 hang = right_lost_dn;hang<image_Y-5;hang++)
        //     {
        //         if(rightline[hang]> rightline[hang+1])
        //         {
        //             cross_xie++;
        //         }
        //     }
        //     if(cross_xie > 5)
        //     {
        //         //��б��ʮ��
        //         cross_flag = 2;
                
        //     }
        //      for(uint8 hang =image_Y-5;hang>right_lost_dn;hang--)
        //     {
        //         if(leftlineflag[hang] != 0)
        //         {
        //             cross_flag = 0;
                    
        //         }
        //     }
        // }
        // //б��ʮ�֣�����
        // else if(left_lost_up&&(right_lost_up==0))
        // {
        //     //����
        //     cross_xie = 0;
        //     left_lost_line = left_lost_dn - left_lost_up;
        //     //��α���·����ߵ��²�������������
        //     for(uint8 hang = left_lost_dn;hang<image_Y-5;hang++)
        //     {
        //         if(leftline[hang]<leftline[hang+1])
        //         {
        //             cross_xie++;
        //         }
        //     }
        //     if(cross_xie > 5)
        //     {
        //         //��б��ʮ��
        //         cross_flag = 3;
        //     }
        //     for(uint8 hang =image_Y-5;hang>left_lost_dn;hang--)
        //     {
        //         if(rightlineflag[hang] != 0)
        //         {
        //             cross_flag = 0;
                    
        //         }
        //     }
        // }
        else
        {
            cross_flag = 0;
            Buzzer(0);
        }
    
    
}
void cross_ru(void)
{
   
    //���ж��Ƿ���ʮ��
    if(leftlineflag[image_Y-5]==0&&rightlineflag[image_Y-5]==0)
    {
        cross_flag =0;  
    } 
    //����
    else if(cross_flag ==1)
    {
        Buzzer(1);
        //��ʼ����
        //�������б��(kΪ��)
        float k_left =  (float)(leftline[left_lost_up] - leftline[left_lost_dn])/left_lost_line;
        float kk_left = leftline[left_lost_dn];
        for(uint8 hang = left_lost_dn;hang>=left_lost_up;hang--)
        {
             kk_left+=k_left;
             leftline[hang]=kk_left;leftlineflag[hang]=2;
        }
        //�����ұ�б��(kΪ��)
        float k_right =  (float)(rightline[right_lost_up] - rightline[right_lost_dn])/right_lost_line;
        float kk_right = rightline[right_lost_dn];
        for(uint8 hang = right_lost_dn;hang>=right_lost_up;hang--)
        {
             kk_right+=k_right;
             rightline[hang]=kk_right;rightlineflag[hang]=2;
        }
    }
    //������
    // else if(cross_flag == 2)
    // {
    //     Buzzer(1);
    //     //��ʼ����
    //     //�����ұ�б��(kΪ��)
    //     //Ѱ����ʵ���ߵ�
    //     for(uint8 hang = right_lost_dn;hang<image_Y-5;hang++)
    //     {
    //         if((rightline[hang+2]>rightline[hang])&&(rightline[hang]>rightline[hang-2]))
    //         {
    //             right_lost_dn = hang;
    //             break;
    //         }
    //     }
    //     for(uint8 hang = 0;hang<right_lost_dn;hang++)
    //     {
    //         if((rightlineflag[hang]!=0)&&(rightlineflag[hang+1]==0))
    //         {
    //             right_lost_up = hang;
    //             break;
    //         }//389100 11400
    //     }
    //     right_lost_line = right_lost_dn - right_lost_up;
    //     float k_right =  (float)(rightline[right_lost_up] - rightline[right_lost_dn])/right_lost_line;
    //     float kk_right = rightline[right_lost_dn];
    //     for(uint8 hang = right_lost_dn;hang>=right_lost_up;hang--)
    //     {
    //          kk_right+=k_right;
    //          rightline[hang]=kk_right;rightlineflag[hang]=2;
    //          leftline[hang]=5;leftlineflag[hang]=0;
    //     }
    // }
    // //������
    //  else if(cross_flag == 3)
    // {
    //     Buzzer(1);
    //     //��ʼ����
    //     //�������б��(kΪ��)
    //     //Ѱ����ʵ���ߵ�
    //     for(uint8 hang = left_lost_dn;hang<image_Y-5;hang++)
    //     {
    //         if((leftline[hang+2]>leftline[hang])&&(leftline[hang]>leftline[hang-2]))
    //         {
    //             left_lost_dn = hang;
    //             break;
    //         }
    //     }
    //     for(uint8 hang = 0;hang<right_lost_dn;hang++)
    //     {
    //         if((leftlineflag[hang]!=0)&&(leftlineflag[hang+1]==0))
    //         {
    //             left_lost_up = hang;
    //             break;
    //         }
    //     }
    //     left_lost_line = left_lost_dn - left_lost_up;
    //     float k_left =  (float)(leftline[left_lost_up] - leftline[left_lost_dn])/left_lost_line;
    //     float kk_left = leftline[left_lost_dn];
    //     for(uint8 hang = left_lost_dn;hang>=left_lost_up;hang--)
    //     {
    //          kk_left+=k_left;
    //          leftline[hang]=kk_left;leftlineflag[hang]=2;
    //          rightline[hang]=5;rightlineflag[hang]=0;
    //     }
    // }


    
}


