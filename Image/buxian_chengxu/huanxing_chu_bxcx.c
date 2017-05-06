#include "common.h"
#include "_small_func_.h"
#include "_buxian_chengxu_.h"




/**********************************************************************************************
名    称：                            出环形补线程序

功能描述： 
入口参数： 
出口参数： 

备    注： 
**********************************************************************************************/
void huanxing_chu_bxcx(uint8 img_bin[][IMG_W_USED])
{
  /*------------------------------------------*/
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
  
  //先从最下面5行找边界 并求取平均值
  start_bj_j_left=0;
  start_bj_j_right=0;
  for( Ci=79 ; Ci>=75; Ci-- )//假如bx_start_i==79 则是79 78 77 76 75 共计5行
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
  for( Ci=74 ; Ci>=0 ; Ci-- )
  {
    if(over_flag_l==0)
    {
      if( f_Sedge_sp_cmd(img_bin[Ci],left_last,15,&left_now)==1 )
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
      if( f_Xedge_sp_cmd(img_bin[Ci],right_last,15,&right_now)==1 )
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
  /*------------------------------------------*/
  
  //用于出环形连线的上左拐点
  zb_imgarr_t chx_gd_l;
  chx_gd_l.i=0;//假如没有找到出环形的拐点 那么就会使用图像的左上角
  chx_gd_l.j=0;
  
  //关键变量：公共i值 右侧j值 左侧j值 水平宽度
  int8  last_n;
  uint8 last_i;
  uint8 last_j_r;
  uint8 last_j_l;
  uint8 kuandu_sp_last;
  last_n   = n_bj_r-1;                               //上次n值
  last_i   = bj_right[last_n].i;                          //上次i值 公共
  last_j_r = bj_right[last_n].j;                          //上次j值 右侧
  last_j_l = f_Sedge_sp_cybjqvj(img_bin[last_i],last_j_r);//上次j值 左侧
  kuandu_sp_last = last_j_r - last_j_l + 1;               //上次水平宽度
  
  //将开始找的i值画线指示出来
  lcd_draw_spx(last_i,BLUE);
  
  int8  tmp_n;
  uint8 tmp_i;
  uint8 tmp_j_r;
  uint8 tmp_j_l;
  uint8 kuandu_sp_tmp;
  for( tmp_n=(last_n-1) ; tmp_n>=0 ; tmp_n=tmp_n-2 )     //本次n值
  {
    tmp_i   = bj_right[tmp_n].i;                         //上次i值 公共
    tmp_j_r = bj_right[tmp_n].j;                         //本次j值 右侧
    tmp_j_l = f_Sedge_sp_cybjqvj(img_bin[tmp_i],tmp_j_r);//本次j值 左侧
    kuandu_sp_tmp = tmp_j_r - tmp_j_l + 1;               //本次水平宽度
    
    if( (kuandu_sp_tmp-kuandu_sp_last)>10 )
    {
      chx_gd_l.i=last_i;
      chx_gd_l.j=last_j_l;
      
      break;
    }
    
    last_i  = tmp_i;               //保存i值 公共
    last_j_l = tmp_j_l;            //保存j值 左侧
    kuandu_sp_last = kuandu_sp_tmp;//保存赛道宽度
  }
  
  //出环形补线
  liang_dian_lian_xian(img_bin,0,chx_gd_l,bj_left[0]);
  
  //当作直到弯道提取中线
  zhidao_wandao_bxcx(img_bin,79,25);
}