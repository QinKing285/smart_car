/*
 * time.h
 *
 *  Created on: 2022��1��6��
 *      Author: paper
 */

#ifndef CODE_TIME_H_
#define CODE_TIME_H_

#include "headfile.h"

extern unsigned char   int_OK;//��ʼ���ɹ���־λ
//���Ĺؼ��ı�־λ�ṹ�嶨��
typedef struct
{        
      unsigned char T_2ms;             
      unsigned char T_6ms ;             
      unsigned char T_10ms ;             
      unsigned char T_20ms ;  
      unsigned char T_1s ;            
       

}Body;
extern Body Flag;
extern int test;
extern int start_count;
/*********��������**************/
void Fuse_result(void)  ;
//���Ĺؼ��ı�־λ�ṹ���ʼ��
void Flag_Init(void);
extern float actual_val_left;
  extern      float actual_val_right ;

extern float pid_speed_target_val_left;
extern float pid_speed_target_val_right;


#endif /* CODE_TIME_H_ */
