/*
 * image.c
 *
 *  Created on: 2022��1��6��
 *      Author: paper
 */
//����ͼ��Ĵ��룬����ֵ���á�
#include "image.h"
#define image_Y 120
#define image_X 180
//������ȱ���
#define Width_zhixian  1
#define Width_wandao  ((1+(Speed_Left-150)*0.008)) //1.6
#define Straight 0
#define Left     1
#define Right    2
#define Sancha   3
#define Lose     0

#define Find     2
uint8 track_width[Row]=
{     0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,   
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,   
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,   
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,   
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,   
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,   
      0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0   
};
//��ֵ��ͼ��
uint8 Threshold_Image[image_Y][image_X];
//��ֵ����ֵ
uint8 Gate=115;
//�������б������� 1-118
uint8 leftline[image_Y];
int16 midline[image_Y];
uint8 rightline[image_Y];

//�洢��־ 2���� 1��һ������ 0����
uint8 leftlineflag[image_Y];
uint8 rightlineflag[image_Y];
uint8 DropRow  = 0;
//��ֹ�б���
uint8 endline = 0,frontline = 0;
uint8 whiteline_left =image_Y;
uint8 whiteline_right =image_Y;
uint8 BlackEndL = 0;
uint8 BlackEndM = 0;
uint8 BlackEndR = 0;
uint8 BlackEndMax = 0;
uint8 BlackEndMin = 0;
uint8 leftline_endline =0 ;
uint8 rightline_endline =0 ;
//б�ʱ���
float leftline_Curvature = 0;
float rightline_Curvature = 0;
float leftline_slope = 0;
float rightline_slope = 0;
uint8 LEFTLOSED=0;
uint8 RIGHTLOSED=0;
uint8 imageover=0;
float track_width_error=149;
//������ʵ�������
void track_width_real(void)
{
    for(uint8 hang=image_Y-1;hang>0;hang--)
    {
      track_width[hang]=track_width_error;
      track_width_error-=1.006;
    }
}
//�õ���ֵ��ͼ��
void Get_Threshold_Image(void)
{
   Gate = my_adapt_threshold(*mt9v03x_image,MT9V03X_W,MT9V03X_H);
   if(Gate>195)Gate=195;
   else if(Gate<175)Gate=175;
  //  if(Gate>125)Gate=125;
  //  else if(Gate<115)Gate=115;
  //��ֵ��
  for(uint8 hang=0;hang<MT9V03X_H;hang++)
    for(uint8 lie=0;lie<MT9V03X_W;lie++)
    {
      if(mt9v03x_image[hang][lie]>=Gate)
          Threshold_Image[hang][lie]=white;
      else
          Threshold_Image[hang][lie]=black;
    }
   
}
//�����˲�
void Pixle_Filter(void) 
{
  int nr;  //��
  int nc;  //��

  for (nr = 10; nr < 110; nr++) 
  {
    for (nc = 10; nc < 170; nc++) 
    {
      if ((Threshold_Image[nr][nc] == black) && (Threshold_Image[nr - 1][nc] + Threshold_Image[nr + 1][nc] + Threshold_Image[nr][nc + 1] + Threshold_Image[nr][nc - 1] >=white*3)) 
      {
        Threshold_Image[nr][nc] = white;
      }
    }
  }
}
//����㷨
uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
   #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }
    //����ÿ������ֵ�ĵ�������ͼ���еı���
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }
    //�����Ҷȼ�[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;


        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {

                w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
                u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

               w1=1-w0;
               u1tmp=gray_sum/pixelSum-u0tmp;

                u0 = u0tmp / w0;              //����ƽ���Ҷ�
                u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
                u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
                deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
                if (deltaTmp > deltaMax)
                {
                    deltaMax = deltaTmp;
                    threshold = (uint8)j;
                }
                if (deltaTmp < deltaMax)
                {
                break;
                }
         }
    return threshold;
}//Ѱ�������ߣ����Ҽ�¼���ߵ�
//��һ��ͼ��Ѱ��
//������������ͼ��Ѱ�߷�ʽ����
void get_route_first(uint8 hang,uint8 start)
{
  //����

    for(uint8 a=0;a<=image_Y;a++)
    {
      leftline[a]=0;
      rightline[a]=image_X-1;
      leftlineflag[a]=0;
      rightlineflag[a]=0;
      midline[a]=image_X/2;
    }
    LEFTLOSED=0;
    RIGHTLOSED=0; 
    //////////////////////////////////////////////////////////////�����
    if(Threshold_Image[hang][start]==white) //�ױ��е�Ϊ��
    {
    //�ױ��е�Ϊ�� �� ��Ѳ�������� (��->��)  //������� ֱ������ �������
      for(uint8 lie=start;lie>=2;lie--)
      {
        
        if(Threshold_Image[hang][lie]==white&&Threshold_Image[hang][lie-1]==black)
        {
            leftline[hang]=lie;
            leftlineflag[hang]=Find;
            break;
        }
      }
    }
    //�ױ��е�Ϊ�� �� ��Ѳ�������� (��->��) //������� �Ҽ������
    else
    {
      
      for(uint8 lie=start;lie<=image_X-2;lie++)
      {
        if(Threshold_Image[hang][lie]==black&&Threshold_Image[hang][lie+1]==white)
        {
            leftline[hang]=lie;
            leftlineflag[hang]=Find;
            break;
        }
      }
    }
    //��� û�ҵ� ��ֱ�Ӹ���ֵ
    if(leftlineflag[hang]==Lose)
    {
      leftline[hang]=1;
      leftlineflag[hang]=Lose;
    }
    
    //////////////////////////////////////////////////////////////�ұ���
    if(Threshold_Image[hang][start]==white)
    {
      
      //�ױ��е�Ϊ�� �� ��Ѳ�������� (��->��)  //������� ֱ������ �������
      for(uint8 lie=start;lie<=image_X-2;lie++)
      {
        if(Threshold_Image[hang][lie]==white&&Threshold_Image[hang][lie+1]==black)
        {
            rightline[hang]=lie;
            rightlineflag[hang]=Find;
            break;
        }
      }
    }
    //�ױ��е�Ϊ�� �� ��Ѳ�������� (��->��) //������� �Ҽ������
    else
    {
      
      //������
      for(uint8 lie=start;lie>=2;lie--)
      {
        if(Threshold_Image[hang][lie]==black&&Threshold_Image[hang][lie-1]==white)
        {   
            rightline[hang]=lie;
            rightlineflag[hang]=Find;
            break;
        }
      }
    }
    //���û�ҵ���ֱ�Ӹ���ֵ
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=image_X-2;
      rightlineflag[hang]=Lose;
    }
    //������ұ��߶������Կ���
    // if(rightlineflag[hang]==1&&leftlineflag[hang]==1)
    // {
    //   leftline[hang]=1;
    //   rightline[hang]=image_X-2;
    // }
    // leftlineflag[hang]=1;
    // rightlineflag[hang]=1;
}
//����Ѱ��
void get_route_all(void)
{
  //���Ѳ������������(��ʾ���������Ҳ�Ѳ��)
  //idea:���ϰ�����Ѳ��
  //��ʼ��
  uint8 Lstart=0,L_max=0,L_min=0;
  uint8 Rstart=0,R_max=0,R_min=0;
  uint8 range = 5;
  for(uint8 hang=image_Y-2;hang>0;hang--)
  {
    //�����
    //ȷ�����߷�Χ
    //������һ��ȷ������Ѱ�ߵ�
    if(leftlineflag[hang+1]==Lose)//�����һ�ж���,���մ��м俪ɨ�ķ�ʽѲ��
    {
      Lstart=3;
      L_max=image_X-2;
      L_min=2;
    }
    else//�����һ��δ����,���ո���Ѳ�ߵķ�ʽѲ��
    {
      Lstart=leftline[hang+1];
      L_max=leftline[hang+1]+range;
      L_min=leftline[hang+1]-range;
      //�޷�
      if(L_max>image_X-1)
        L_max=image_X-2;
      if(L_min<0)
        L_min=2;
    }
    //�����
    if(Threshold_Image[hang][Lstart]==black)//����һ�е�������Ӧ���е�Ϊ�ڵ�(����͸��ԭ��һ�����)
    {
      //������ ��->�� ����       ֱ������ ������� �Ҽ������
      for(uint8 lie=Lstart;lie<L_max;lie++)
      {
        if(Threshold_Image[hang][lie]==black&&Threshold_Image[hang][lie+1]==white)
        {
            leftline[hang]=lie;
            leftlineflag[hang]=Find;
            break;
        }
      }
    }
    else//����һ�е�������Ӧ���е�Ϊ�ڵ�(������ת)
    {
      //������ ��->�� ����    ������� �������
      for(uint8 lie=Lstart;lie>L_min;lie--)
      {
        if(Threshold_Image[hang][lie]==white&&Threshold_Image[hang][lie-1]==black)
        {
            leftline[hang]=lie;
            leftlineflag[hang]=Find;
            break;
        }
      }
    } 
    //�����û�ҵ�˵������
    if(leftlineflag[hang]==Lose)
    {
        leftline[hang]=1;
    }
    //�ұ���
    //ȷ��Ѱ�߷�Χ
    if(rightlineflag[hang+1]==Lose)
    {
      Rstart=image_X-3;
      R_max=image_X-2;
      R_min=2;
    }
    else
    {
      Rstart=rightline[hang+1];
      R_max=rightline[hang+1]+range;
      R_min=rightline[hang+1]-range;
      if(R_max>image_X-1)
        R_max=image_X-2;
      if(R_min<0)
        R_min=2;
    }
    //�ұ���
    if(Threshold_Image[hang][Rstart]==black)
    {
      //������
      for(uint8 lie=Rstart;lie>R_min;lie--)
      {
        if(Threshold_Image[hang][lie]==black&&Threshold_Image[hang][lie-1]==white)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=Find;
          break;
        }
      }  
    }
    else
    {
      //������
      for(uint8 lie=Rstart;lie<R_max;lie++)
      {
        if(Threshold_Image[hang][lie]==white&&Threshold_Image[hang][lie+1]==black)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=Find;
          break;
        }
      }
      //�����û�ҵ�
      if(rightlineflag[hang]==Lose)
      {
        rightline[hang]=178;
      }
    }
    //Ѱ�Ҵ���,���ն��ߴ���
    if(leftline[hang]>rightline[hang])
    {
        leftline[hang]=1;
        rightline[hang]=178;
        leftlineflag[hang]=Lose;
        rightlineflag[hang]=Lose;
    }
  }
  //����Զһ�а���һ�в�
  leftline[0]=leftline[1];
  leftlineflag[0]=leftlineflag[1];
  rightline[0]=rightline[1];
  rightlineflag[0]=rightlineflag[1];
}

//�������
void get_route_midline(void)
{
  for(uint8 hang=image_Y-1;hang>endline;hang--)
  {
      
     
       if((leftlineflag[hang]==2)&&(rightlineflag[hang]==0))
      {
       // rightline[hang]=leftline[hang]+Width_zhixian*track_width[hang];
        midline[hang]=leftline[hang]+Width_zhixian*track_width[hang]/2;
      }
      //���Ҳ���
      else if((leftlineflag[hang]==0)&&(rightlineflag[hang]==2))
      {
        //leftline[hang]=rightline[hang]-Width_zhixian*track_width[hang];
        midline[hang]=rightline[hang]-Width_zhixian*track_width[hang]/2;
      }
      //ȫ����
      else if((leftlineflag[hang]==2)&&(rightlineflag[hang]==2))
      {
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
      else
          midline[hang]=image_X/2;
    }
}
//��ֹ��
void get_route_endline(void)
{
  unsigned char LEndFlag  =  0;//��־λ
  unsigned char MEndFlag  = 0;
  unsigned char REndFlag  = 0;
  int i=0;
  BlackEndL=0;
  BlackEndM=0;
  BlackEndR=0;
  for (i=Row-1;i>0;i--)
  {
    if(Threshold_Image[i][Col/2] == white && !MEndFlag )
        {
        BlackEndM++;//���߽�ֹ�� ����
        }
    else if(i > 1 && Threshold_Image[i-1][Col/2] == black && Threshold_Image[i-2][Col/2] == black)//���������Ǻ�ɫ
        {
        MEndFlag = 1;//���߽�ֹ�����ҵ� 
        }


    if(Threshold_Image[i][Col/4] == white && !LEndFlag )
        {
        BlackEndL++;//���߽�ֹ�� ����
        }
    else if(i > 1 && Threshold_Image[i-1][Col/4] == black && Threshold_Image[i-2][Col/4] == black)
        {
        LEndFlag = 1;//���߽�ֹ�����ҵ�
        }

    
    if(Threshold_Image[i][Col*3/4] == white && !REndFlag )
        {
        BlackEndR++;//���߽�ֹ�� ����
        }
    else if(i > 1 && Threshold_Image[i-1][Col*3/4] == black && Threshold_Image[i-2][Col*3/4] == black)
        {
        REndFlag = 1;//���߽�ֹ�����ҵ�
        }
  }
  BlackEndMax =max(BlackEndL,BlackEndM,BlackEndR);//ȡ��ֵ
  BlackEndMin =BlackEndM;
  if(BlackEndMax > 120 )
   {
      BlackEndMax = 120;
   }
  if(BlackEndMin > 120 )
    {
      BlackEndMin = 120;
    }
  endline = 120-BlackEndMax;//��ֹ������
  frontline = 120-BlackEndMin;
  if(endline>=120) endline=120;
  else if(endline<=0)  endline=0;

  if(frontline>=120) frontline=120;
  else if(frontline<=0)  frontline=0;

  if(frontline<endline) frontline = endline;
}

void Draw_line(void)
{
    for(uint8 y=119;y>0;y--)
    {
        if((leftlineflag[y]==0)||(rightlineflag[y]==0))
        {
            ips200_drawpoint(midline[y],y,RED);
            ips200_drawpoint(midline[y]+1,y,RED);
            ips200_drawpoint(midline[y]-1,y,RED);
        }
        else
        {
            ips200_drawpoint(midline[y]+1,y,GREEN);
            ips200_drawpoint(midline[y],y,GREEN);
            ips200_drawpoint(midline[y]-1,y,GREEN);
        }
            ips200_drawpoint(leftline[y]+1,y,BLUE);
            ips200_drawpoint(leftline[y],y,BLUE);
            ips200_drawpoint(leftline[y]-1,y,BLUE);
            ips200_drawpoint(rightline[y]+1,y,BLUE);
            ips200_drawpoint(rightline[y],y,BLUE);
            ips200_drawpoint(rightline[y]-1,y,BLUE);
            ips200_drawpoint(leftline[y]+1,y,BLUE);
    }
      for(uint8 lie=179;lie>0;lie--)
    {
      ips200_drawpoint(lie-1,endline,RED);
      ips200_drawpoint(lie  ,endline,RED);
      ips200_drawpoint(lie+1,endline,RED);
      ips200_drawpoint(lie-1,frontline,BLUE);
      ips200_drawpoint(lie  ,frontline,BLUE);
      ips200_drawpoint(lie+1,frontline,BLUE);
      ips200_drawpoint(lie-1,frontline,ring_end_R);
      ips200_drawpoint(lie  ,frontline,ring_end_R);
      ips200_drawpoint(lie+1,frontline,ring_end_R);
    }
}
uint8 max(uint8 a,uint8 b,uint8 c)
{
     uint8 max;
     max = a;
     if (b > max) {
         max = b;
     }
     if (c > max) {
         max = c;
     }
     return max;    
}
uint8 min(uint8 a,uint8 b,uint8 c)
{
     uint8 min;
     min = a;
     if (b < min) {
         min = b;
     }
     if (c < min) {
         min = c;
     }
     return min;
}

void Curvature_determination(void)
{
  leftline_endline = 0;
  rightline_endline = 0;
  //�ж������ʵ��Ѳ�߽�ֹ��( �� �� �� �� Ѳ �� )
  for(uint8 hang=119;hang>endline;hang--)
  {
    //����Ѿ�û���� 
    if(leftlineflag[hang]==0) {LEFTLOSED=1;break;}
  }
  for(uint8 hang=110;hang>endline;hang--)
  {
    if(leftlineflag[hang+2]==Find&&leftlineflag[hang+1]==Find&&leftlineflag[hang]==Find&&leftlineflag[hang-1]!=Find)
    {
      leftline_endline=hang;
      break;
    }
    
  }
  if(leftline_endline==0)
      leftline_endline=endline+5;
  //�ж��ұ���ʵ��Ѳ�߽�ֹ��
  for(uint8 hang=119;hang>endline;hang--)
  {
     //�Ҳ��Ѿ�û����
    if(rightlineflag[hang]==0) {RIGHTLOSED=1;break;}
  }
   for(uint8 hang=110;hang>endline;hang--)
  {
    if(rightlineflag[hang+2]==Find&&rightlineflag[hang+1]==Find&&rightlineflag[hang]==Find&&rightlineflag[hang-1]!=Find)
    {
      rightline_endline=hang;
      break;
    }
  }
  if(rightline_endline==0)
      rightline_endline=endline+5;

  if(!LEFTLOSED)
  {
    //leftline_Curvature = 2*((leftline[119-leftline_endline/2]-leftline[image_Y-1])*(leftline_endline-image_Y+1)-(leftline[leftline_endline]-leftline[image_Y-1])*(119-leftline_endline/2-image_Y+1))
    leftline_Curvature = (((leftline[leftline_endline])*1.0)/((120-leftline_endline)*1.0))*5.0;
    // int32 x1 = leftline[image_Y-1-5],x2 =leftline[(image_Y-1-(5+image_Y-1-leftline_endline)/2)],x3 = leftline[leftline_endline];
    // int32 y1 = 5,y3 = image_Y-leftline_endline,y2 = (y1+y3)/2;
    // int32 AB = m_sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
    // int32 BC = m_sqrt((y3-y2)*(y3-y2)+(x3-x2)*(x3-x2));
    // int32 AC = m_sqrt((y3-y1)*(y3-y1)+(x3-x1)*(x3-x1));
    // int32 S_ABC = ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
    //leftline_Curvature = 4.0*S_ABC/(AB*BC*AC)*1000;
    //leftline_slope     = (y3-y1)/(x3-x1);
  }
     else
     {
       leftline_Curvature = 0;
       leftline_slope = 0;
     }
    

  if(!RIGHTLOSED)
  {
     rightline_Curvature = (((rightline[rightline_endline]-180)*1.0)/((120-rightline_endline)*1.0))*5.0;
    // int32 x1 = rightline[image_Y-1-5],x2 =rightline[(image_Y-1-(5+image_Y-1-rightline_endline)/2)],x3 = rightline[rightline_endline];
    // int32 y1 = 5,y3 = image_Y-rightline_endline,y2 = (y1+y3)/2;
    // int32 AB = m_sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
    // int32 BC = m_sqrt((y3-y2)*(y3-y2)+(x3-x2)*(x3-x2));
    // int32 AC = m_sqrt((y3-y1)*(y3-y1)+(x3-x1)*(x3-x1));
    // int32 S_ABC = ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
    //rightline_Curvature = 4.0*S_ABC/(AB*BC*AC)*1000;
    //rightline_slope     = (y3-y1)/(x3-x1);
  }
  else
  {
    rightline_Curvature = 0;
    rightline_slope = 0;
  }
  
  //���ݽ�ֹ�����򵥵ؼ�������������б��
}
unsigned int m_sqrt(unsigned int x) 
{ 
 unsigned char ans=0,p=0x80;
 while(p!=0) 
 { 
 ans+=p; 
 if(ans*ans>x) 
 { 
 ans-=p; 
 } 
 p=(unsigned)(p/2);
 } 
 return(ans); 
}
