#include "common.h"
#include "_small_func_.h"
#include "_buxian_chengxu_.h"




/**********************************************************************************************
名    称：                           直道弯道补线程序

功能描述： 
入口参数： 
出口参数： 

备    注： 无特殊要求时给形参bx_start_i传入79即可
**********************************************************************************************/
void zhidao_wandao_bxcx(uint8 img_bin[][IMG_W_USED],uint8 bx_start_i,uint8 bx_range)
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
  
  zb_imgarr_t mid_point[8];//分7等份 共计8个中间点
  
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
  
  

  
  
  
  
  //取10个均匀的中间点
  mid_point[0]  = zhong_dian_img( bj_left[ 0*n_bj_l/7 ],bj_right[ 0*n_bj_r/7 ] );
  mid_point[1]  = zhong_dian_img( bj_left[ 1*n_bj_l/7 -1],bj_right[ 1*n_bj_r/7 -1] );
  mid_point[2]  = zhong_dian_img( bj_left[ 2*n_bj_l/7 -1],bj_right[ 2*n_bj_r/7 -1] );
  mid_point[3]  = zhong_dian_img( bj_left[ 3*n_bj_l/7 -1],bj_right[ 3*n_bj_r/7 -1] );
  mid_point[4]  = zhong_dian_img( bj_left[ 4*n_bj_l/7 -1],bj_right[ 4*n_bj_r/7 -1] );
  mid_point[5]  = zhong_dian_img( bj_left[ 5*n_bj_l/7 -1],bj_right[ 5*n_bj_r/7 -1] );
  mid_point[6]  = zhong_dian_img( bj_left[ 6*n_bj_l/7 -1],bj_right[ 6*n_bj_r/7 -1] );
  mid_point[7]  = zhong_dian_img( bj_left[ 7*n_bj_l/7 -1],bj_right[ 7*n_bj_r/7 -1] );
  
  //依次连接
  liang_dian_lian_xian(img_bin,1,mid_point[0],mid_point[1]);
  liang_dian_lian_xian(img_bin,1,mid_point[1],mid_point[2]);
  liang_dian_lian_xian(img_bin,1,mid_point[2],mid_point[3]);
  liang_dian_lian_xian(img_bin,1,mid_point[3],mid_point[4]);
  liang_dian_lian_xian(img_bin,1,mid_point[4],mid_point[5]);
  liang_dian_lian_xian(img_bin,1,mid_point[5],mid_point[6]);
  liang_dian_lian_xian(img_bin,1,mid_point[6],mid_point[7]);
  
  //顶行指示线
  liang_dian_lian_xian(img_bin,128,bj_left[n_bj_l-1],bj_right[n_bj_r-1]);
}