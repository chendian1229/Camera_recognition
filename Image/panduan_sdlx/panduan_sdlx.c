#include "common.h"
#include "_small_func_.h"
#include  "MK60_gpio.h"
#include "panduan_sdlx.h"

zb_imgarr_t l_end_img,r_end_img;        //定义最后一行坐标与上一帧最后一行
zb_imgarr_t l_end_last_img,r_end_last_img;    


#define Hang_3    40     //        打方向
#define Hang_2    50     //        环形识别行
#define Hang_1    75     //        求斜率
#define Hang_end  79     //        最后一行，判断十字

#define wandao          ((K_l_13.value>=0)&&(K_r_13.value>=0))||((K_l_13.value<0)&&(K_r_13.value<0))
#define zhidao          (K_l_13.value>0)&&(K_r_13.value<0)
#define shizi           (K_l_13.value<0)&&(K_r_13.value>0)
#define ruhuanxing      (K_l_13.value<0)&&(K_r_13.value>0)
#define chuhuanxing     (f_l_2==0)&&(f_r_2==1)


uint8 f_l_end;//底部一行
uint8 left_end;
uint8 f_r_end;
uint8 right_end;

uint8 f_l_1;//第 1 行
uint8 left_1;
uint8 f_r_1;
uint8 right_1;

uint8 f_l_2;//第 2 行
uint8 left_2;
uint8 f_r_2;
uint8 right_2;

uint8 f_l_3;//第 3 行
uint8 left_3;
uint8 f_r_3;
uint8 right_3;

/*-----------------赛道类型判断函数---------------*/
SaiDao_type_m  pdsdlx(uint8 img_bin[][IMG_W_USED])
{
  
  SaiDao_type_m sdlx_return;    //定义返回的赛道类型
  
  zb_imgarr_t l_1_img,r_1_img;            //定义第一行，第二行，第三行左右跳变沿图像坐标
  zb_imgarr_t l_2_img,r_2_img;
  zb_imgarr_t l_3_img,r_3_img;
 
  
  zb_imgarr_t czs1,czs2;                  //垂直扫显示
  
  zb_math_t l_1_math,r_1_math;            //定义数学坐标
  zb_math_t l_3_math,r_3_math;
  zb_math_t l_2_math,r_2_math;
  
  slope_t K_l_13,K_r_13;                        //定义左右斜率
  slope_t K_l_12,K_r_12;
  
  uint8 kuandu_last;                      //定义上一次最后一行宽度数据
  uint8 kuandu;                           //定义最后一行宽度数据
  
  static uint8 leixing_flag=0;               //定义判断类型标志位
  static uint8 ruhuanxing_flag=0;              //定义入环形标志位
  static uint8 chuhuanxing_flag=0;            //定义出环形标志位
  static uint8 chuhuanxingbc_flag=0;            //定义出环形保持标志位
         uint8 chuizhi_flag=0;                  //定义环形垂直扫标志位
  

  
  f__edge_sp_czj_plus(img_bin[Hang_1],93,&f_l_1,&left_1,&f_r_1,&right_1);//水平扫第一行
  f__edge_sp_czj_plus(img_bin[Hang_2],93-30,&f_l_2,&left_2,&f_r_2,&right_2);//水平扫第二行
  f__edge_sp_czj_plus(img_bin[Hang_3],93-40,&f_l_3,&left_3,&f_r_3,&right_3);//水平扫第三行
  f__edge_sp_czj_plus(img_bin[Hang_end],93,&f_l_end,&left_end,&f_r_end,&right_end);//水平扫最后一行
  
  chuizhi_flag=(f__edge_cz_ry(img_bin,84,15,10))&&(f__edge_cz_ry(img_bin,103,15,10));       //中间垂直扫函数
      
  czs1.i=10;                                    //垂直扫连线显示坐标
  czs1.j=88;
  
  czs2.i=5;
  czs2.j=99;
      
  l_1_img.i=Hang_1;     //  左1图像坐标赋值            //给图像坐标赋值 
  l_1_img.j=left_1;
  r_1_img.i=Hang_1;     //  右1图像坐标赋值
  r_1_img.j=right_1;
  
  l_2_img.i=Hang_2;     //  左2图像坐标赋值
  l_2_img.j=left_2;
  r_2_img.i=Hang_2;     //  右2图像坐标赋值
  r_2_img.j=right_2;
  
  l_3_img.i=Hang_3;     //  左3图像坐标赋值
  l_3_img.j=left_3;
  r_3_img.i=Hang_3;     //  右3图像坐标赋值
  r_3_img.j=right_3;


  l_end_img.i=Hang_end;   //最后一行图像坐标赋值
  l_end_img.j=left_end;
  r_end_img.i=Hang_end;
  r_end_img.j=right_end;      
      
  l_1_math=covert_zb(l_1_img);      //第一行，第三行左右跳变沿坐标转换用于计算斜率
  r_1_math=covert_zb(r_1_img);
  l_3_math=covert_zb(l_3_img);
  r_3_math=covert_zb(r_3_img);
  
  l_2_math=covert_zb(l_2_img);      //第二行左右跳变沿坐标转换
  r_2_math=covert_zb(r_2_img);      

  K_l_13=calcu_slope(l_1_math,l_3_math);    //计算1_3左斜率
  K_r_13=calcu_slope(r_1_math,r_3_math);    //计算1_3右斜率
  
  K_l_12=calcu_slope(l_1_math,l_2_math);    //计算1_2左斜率  尚未使用
  K_r_12=calcu_slope(r_1_math,r_2_math);    //计算1_2右斜率
  
  
  gpio_init (PTD15,GPO,1);      //指示赛道类型的灯      D15为直道和弯道
  gpio_init (PTE26,GPO,1);      //                      E26为十字
  
/*--------------------连线函数-----------------------*/
  //两个斜率图像连线
//  liang_dian_lian_xian(img_bin,125,l_1_img,l_3_img);  
//  liang_dian_lian_xian(img_bin,125,r_1_img,r_3_img);
//  liang_dian_lian_xian(img_bin,125,l_1_img,r_1_img);
//  liang_dian_lian_xian(img_bin,125,l_3_img,r_3_img);
//  
//  liang_dian_lian_xian(img_bin,125,czs1,czs2);        //垂直扫函数
//  liang_dian_lian_xian(img_bin,125,l_2_img,r_2_img);  //出环形判断线连线
///*---------------------------------------------------------  */
//  
///*-------------------串口屏显示------------------------*/
//  printf("n0.val=%d",(int8)(K_l_13.value*10));     //左斜率
//  UART_End();
//  
//  printf("n1.val=%d",(int8)(K_r_13.value*10));     //右斜率
//  UART_End();
//  
//  printf("n2.val=%d",f_l_3);
//  UART_End();
//  
//  printf("n3.val=%d",f_r_3);
//  UART_End();
////  //printf("hello");
/*--------------------计算底行宽度判断跳出条件------------------------------*/  
  
  kuandu_last=r_end_last_img.j-l_end_last_img.j;               //计算上一帧最后一行宽度
  kuandu=r_end_img.j-l_end_img.j;                              //和现在这一帧最后一行宽度之差
  
/*---------------------------判断赛道类型-----------------------------------*/  
//直道弯道
if(((f_l_3==1)||(f_r_3==1))&&(leixing_flag==0))
{
	 //判断为直道 
    if((zhidao)&&(chuhuanxingbc_flag==0))   
    {
      gpio_set(PTD15, 0);
      sdlx_return=ZhiDao;
//      printf("t0.txt=\"zhidao\"");
//      UART_End();
    }
    
    //判断为弯道 
    if((wandao)&&(chuhuanxingbc_flag==0))
    {
      gpio_set(PTD15, 0);
      sdlx_return=WanDao;  
//      printf("t0.txt=\"wandao\"");
//      UART_End();
    }
	//判断为出环形
    if(((chuhuanxing)&&(chuhuanxing_flag==1))||(chuhuanxingbc_flag==1))
    {
      
      sdlx_return=ChuHuanXing;      
      chuhuanxingbc_flag=1;
//      printf("t0.txt=\"chuhuan\"");
//      UART_End();
      if((kuandu_last>110)&&(kuandu<110))
      {
        chuhuanxingbc_flag=0;
        chuhuanxing_flag=0;
      }      
    }
}
//入环形
if((((f_l_3==0)&&(f_r_3==0))&&(chuizhi_flag==1))||(leixing_flag==2))
{
	if((chuhuanxing_flag==0)||(leixing_flag==2))
	{
              //定义为入环形以后可以开启出环形
            sdlx_return=RuHuanXing;                             
            gpio_set(PTD15, 0);
            gpio_set(PTE26,0);
            leixing_flag=2;
      //      printf("t0.txt=\"ruhuan\"");
      //      UART_End();
            if((kuandu_last>130)&&(kuandu<130))
            {
              leixing_flag=0;
              chuhuanxing_flag=1;
              
            }
		
	}
	
}
if((((f_l_3==0)&&(f_r_3==0))&&(chuizhi_flag==0)&&(leixing_flag!=2))||(leixing_flag==1))               //else中为十字，第三行丢线
{
    //判断为十字
    if((shizi)||(leixing_flag==1))
    {
        leixing_flag=1;          //赛道类型置为2，保持十字直到最后一行宽度小于100
        gpio_set(PTE26,0);        
        sdlx_return=ShiZi;
//        printf("t0.txt=\"shizi\"");
//        UART_End();  

        if((kuandu_last>100)&&(kuandu<100))
        {
          leixing_flag=0;
        }        
    }   
}
  
  
      

  l_end_last_img=l_end_img;     //保留最后一行上一帧的跳变沿坐标
  r_end_last_img=r_end_img;

  return sdlx_return;
}