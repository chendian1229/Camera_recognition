#include "common.h"
#include "_small_func_.h"
#include  "include.h"
#include "panduan_sdlx.h"

uint8  f__edge_cz_ry(uint8 line[][IMG_W_USED],uint8 row,uint8 start_i,uint8 final_i); //声明垂直扫函数
uint8 huanxing_shizi(uint8 img_bin[][IMG_W_USED],uint8 bx_start_i,uint8 bx_range);     //声明环形弯道识别程序

zb_imgarr_t l_end_img,r_end_img;        //定义最后一行坐标与上一帧最后一行
zb_imgarr_t l_end_last_img,r_end_last_img;    


#define Hang_3    30     //        打方向
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
 
  

  
  zb_math_t l_1_math,r_1_math;            //定义数学坐标
  zb_math_t l_2_math,r_2_math;
  zb_math_t l_3_math,r_3_math;

  
  slope_t K_l_13,K_r_13;                        //定义左右斜率
  slope_t K_l_12,K_r_12;
  
  uint8 kuandu_last;                      //定义上一次最后一行宽度数据
  uint8 kuandu;                           //定义最后一行宽度数据
  
  static uint8 leixing_flag=0;                  //定义判断类型标志位
  static uint8 chuhuanxing_flag=0;              //定义出环形标志位
  static uint8 shizi_huanxing_flag=0;           //定义十字环形标志位
  

  
  f__edge_sp_czj_plus(img_bin[Hang_1],93,&f_l_1,&left_1,&f_r_1,&right_1);//水平扫第一行
  f__edge_sp_czj_plus(img_bin[Hang_2],93-30,&f_l_2,&left_2,&f_r_2,&right_2);//水平扫第二行
  f__edge_sp_czj_plus(img_bin[Hang_3],93-50,&f_l_3,&left_3,&f_r_3,&right_3);//水平扫第三行
  f__edge_sp_czj_plus(img_bin[Hang_end],93,&f_l_end,&left_end,&f_r_end,&right_end);//水平扫最后一行
  
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
  
  shizi_huanxing_flag=huanxing_shizi(img_bin,79,7);      //判断十字环形左右两边的点的个数

  
/*--------------------连线函数-----------------------*/
//  //两个斜率图像连线
//  liang_dian_lian_xian(img_bin,125,l_1_img,l_3_img);  
//  liang_dian_lian_xian(img_bin,125,r_1_img,r_3_img);
//  liang_dian_lian_xian(img_bin,125,l_1_img,r_1_img);
//  liang_dian_lian_xian(img_bin,125,l_3_img,r_3_img);
//  
//
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
    if(zhidao)   
    {
      gpio_set(PTD15, 0);
      sdlx_return=ZhiDao;
//      printf("t0.txt=\"zhidao\"");
//      UART_End();
    }
    
    //判断为弯道 
    if(wandao)
    {
      gpio_set(PTD15, 0);
      sdlx_return=WanDao;  
//      printf("t0.txt=\"wandao\"");
//      UART_End();
    }

}
//判断为十字
if((((f_l_3==0)&&(f_r_3==0))&&(shizi_huanxing_flag==0)&&(leixing_flag!=2))||(leixing_flag==1))             
{

        leixing_flag=1;          //赛道类型置为2，保持十字直到最后一行宽度小于100    
        gpio_set(PTE26,0);        
        sdlx_return=ShiZi;
//        printf("t0.txt=\"shizi\"");
//        UART_End();  

        if((kuandu_last>100)&&(kuandu<100))
        {
          leixing_flag=0;
          shizi_huanxing_flag=0;
        }        
      
}
  
//入环形
if((((f_l_3==0)&&(f_r_3==0))&&(shizi_huanxing_flag==1)&&(leixing_flag!=1))||(leixing_flag==2))
{
	if((chuhuanxing_flag==0)||(leixing_flag==2))
	{
              //定义为入环形以后可以开启出环形
            sdlx_return=RuHuanXing;                             
            gpio_set(PTD15, 0);
            gpio_set(PTE26,0);
            leixing_flag=2;
//            printf("t0.txt=\"ruhuan\"");
//            UART_End();
            if((kuandu_last>130)&&(kuandu<130))
            {
              leixing_flag=0;
              chuhuanxing_flag=1;
              shizi_huanxing_flag=0;
              sdlx_return=ChuHuanXing;
            }
		
	}
	
}

  l_end_last_img=l_end_img;     //保留最后一行上一帧的跳变沿坐标
  r_end_last_img=r_end_img;

  return sdlx_return;
}

/**********************************************************************************************
名    称：             

功能描述：        从底部向上垂直,start_i到final_i是否为全为黑色，
入口参数： 
出口参数： 

备    注： 全为黑色则flag置1，否则flag置0
**********************************************************************************************/
uint8 f__edge_cz_ry(uint8 line[][IMG_W_USED],uint8 row,uint8 start_i,uint8 final_i)
{
  //从中间底部向上
  uint8 flag=1;
  for( uint8 Ci=start_i ; Ci>=final_i ; Ci-- )
  {
    //从第 93(94也可以) 列处 由下往上 垂直扫描跳变沿
    if( line[Ci][row]==255)
    {
         flag=0;
         break;
    }

  }
  return flag;
}
/**********************************************************************************************
名    称：                           环形弯道识别程序

功能描述： 
入口参数： 
出口参数： 

备    注： 无特殊要求时给形参bx_start_i传入79即可
**********************************************************************************************/
uint8 huanxing_shizi(uint8 img_bin[][IMG_W_USED],uint8 bx_start_i,uint8 bx_range)
{
  int16 Ci;
  
  uint8 nomeaning_l;//用于最底部5行 从中间往两边扫赛道边界
  uint8 nomeaning_r;//同上
  
  uint8 n_bj_l=0;//搜索到的左边点界个数
  uint8 n_bj_r=0;//搜索到的右边点界个数
  zb_imgarr_t bj_left[IMG_H_USED]; //左边界点数组
  zb_imgarr_t bj_right[IMG_H_USED];//右边界点数组
  
  uint16 start_bj_j_left; //起始左边界点 j 值（前5行平均值）
  uint16 start_bj_j_right;//起始右边界点 j 值（前5行平均值）
  
  uint8 over_flag_l=0;       //是否结束扫描左边界标志位 0不结束 1结束
  uint8 over_flag_r=0;       //是否结束扫描右边界标志位 0不结束 1结束
  uint8 left_now,right_now;  //本次扫描所使用左右边界点的j值
  uint8 left_last,right_last;//下次扫描所使用左右边界点的j值（即本次结果） 
  uint8 shihuan_flag=0;
  
  
  //先从最下面5行找边界 并求取平均值
  start_bj_j_left=0;
  start_bj_j_right=0;
  for( Ci=bx_start_i ; Ci>=(bx_start_i-4); Ci-- )//假如bx_start_i==79 则是79 78 77 76 75 共计5行
  {
    //需要注意 程序默认最底部5行都是可以找到边界点的
    f__edge_sp_czj(img_bin[Ci],93,&nomeaning_l,&(bj_left[n_bj_l].j),&nomeaning_r,&(bj_right[n_bj_r].j));
    bj_left[n_bj_l].i  = Ci;
    bj_right[n_bj_r].i = Ci;
    start_bj_j_left   += bj_left[n_bj_l].j;
    start_bj_j_right  += bj_right[n_bj_r].j;
    
    n_bj_l++;
    n_bj_r++;
  }
  start_bj_j_left /=5;//将平均值作为以后的起始边界j值
  start_bj_j_right/=5;//同上
  
  //前5行扫完后 往后每行在上一行的左右边界附近查找本行边界 以节省时间 同时找的更远
  left_last  = start_bj_j_left;
  right_last = start_bj_j_right;
  for( Ci=(bx_start_i-5) ; Ci>=0 ; Ci-- )
  {
    if(over_flag_l==0)
    {
      if( f_Sedge_sp_cmd(img_bin[Ci],left_last,bx_range,&left_now)==1 )
      {
        bj_left[n_bj_l].i=Ci;
        bj_left[n_bj_l].j=left_now;
        
        left_last=left_now;
        n_bj_l++;
      }
      else
      {
        over_flag_l=1;
      }
    }
    
    if(over_flag_r==0)
    {
      if( f_Xedge_sp_cmd(img_bin[Ci],right_last,bx_range,&right_now)==1 )
      {
        bj_right[n_bj_r].i=Ci;
        bj_right[n_bj_r].j=right_now;
        
        right_last=right_now;
        n_bj_r++;
      }
      else
      {
        over_flag_r=1;
      }
    }
    
    if( (over_flag_l==1)&&(over_flag_r==1) )
    {
      break;
    }
    
  }
  
  //自拟代码，判断环形和十字左右两边点的个数
  
    if(n_bj_l>=n_bj_r)
    {
        if(n_bj_r>60)
        {
          shihuan_flag=1;               //置1则为环形
        }
    }
    else
    {
         if(n_bj_l>60)
        {
          shihuan_flag=1;               //置1则为环形
        }
    }
  return shihuan_flag;
}
