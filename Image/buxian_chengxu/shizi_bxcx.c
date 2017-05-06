#include "common.h"
#include "_small_func_.h"
#include "_buxian_chengxu_.h"



/**********************************************************************************************
名    称：                         十字补线程序

功能描述： 
入口参数： 
出口参数： 

备    注： 集成了 入十字 十字中 出十字 三个阶段的补线程序
**********************************************************************************************/
uint8 flg_msycsz;//马上要出十字标志位（每当判断出十字时，先将此标志位清零再执行下面的十字补线程序）
void shizi_bxcx(uint8 img_bin[][IMG_W_USED])
{
  static uint8 msycsz_bx_start_i;
  
  if( flg_msycsz==0 )
  {
    uint8 xgd_flag_l;//下左拐点是否找到标志位
    uint8 xgd_flag_r;//下右拐点是否找到标志位
    zb_imgarr_t xgd_l;//下左拐点
    zb_imgarr_t xgd_r;//下右拐点
    
    //先提取“下左右拐点”
    f__guaidian_X(img_bin,&xgd_flag_l,&xgd_l,&xgd_flag_r,&xgd_r);
    
    uint8 sgd_flag_l;     //上左拐点是否找到标志位
    uint8 sgd_flag_r;     //上右拐点是否找到标志位
    zb_imgarr_t sgd_l;    //上左拐点
    zb_imgarr_t sgd_r;    //上右拐点
    uint8 f_sgd_s_j_l;    //找上拐点的左侧起始j值
    uint8 f_sgd_s_j_r;    //找上拐点的右侧起始j值
    uint8 f_sgd_start_i;  //查找找上左右拐点的起始i值
    
    f_sgd_s_j_l = xgd_l.j-20; //计算上述j值
    f_sgd_s_j_r = xgd_r.j+20; //同上
    if( xgd_l.i<xgd_r.i )     //下左右拐点谁的i值靠上（即越小）就用谁
      f_sgd_start_i=xgd_l.i-5;//同上
    else                      //同上
      f_sgd_start_i=xgd_r.i-5;//同上
    
    //再依据“下左右拐点”去提取“上左右拐点”
    f__guaidian_S(img_bin,f_sgd_start_i,f_sgd_s_j_l,f_sgd_s_j_r,&sgd_flag_l,&sgd_l,&sgd_flag_r,&sgd_r);
    if( (sgd_flag_l==1)&&(sgd_flag_r==1) )
    {
      if( MAX(sgd_l.i,sgd_r.i)>69 )//当上左右拐点 低到一定程度时
      {
        flg_msycsz=1;
        
        msycsz_bx_start_i=MIN(sgd_l.i,sgd_r.i)-5;
      }
    }
    
    //十字补线
    zb_imgarr_t zdd_l;//最底部left
    zb_imgarr_t zdd_r;//最底部right
    zdd_l.i=79;
    zdd_r.i=79;
    uint8 nomeaning_l;
    uint8 nomeaning_r;
    f__edge_sp_czj(img_bin[79],93,&nomeaning_l,&(zdd_l.j),&nomeaning_r,&(zdd_r.j)); 
    if(sgd_flag_l==1)
    {
      liang_dian_lian_xian(img_bin,0,xgd_l,sgd_l);//上拐点找到了 上下拐点直接连线 补全十字边界
    }
    else
    {
      xiang_shang_hua_yan_shen_xian(img_bin,0,zdd_l,xgd_l);//上拐点没找到 画延伸线 补全十字边界
    }
    if(sgd_flag_r==1)
    {
      liang_dian_lian_xian(img_bin,0,xgd_r,sgd_r);//上拐点找到了 上下拐点直接连线 补全十字边界
    }
    else
    {
      xiang_shang_hua_yan_shen_xian(img_bin,0,zdd_r,xgd_r);//上拐点没找到 画延伸线 补全十字边界
    }
    
    //补完十字边界线后 再执行直道弯道补线程序 补出中线
    zhidao_wandao_bxcx(img_bin,79,10);
    
    //在图像上画出这几个拐点
    if(sgd_flag_l==1) gd_hua_zfx(img_bin,200,sgd_l);
    if(sgd_flag_r==1) gd_hua_zfx(img_bin,200,sgd_r);
    if(xgd_flag_l==1) gd_hua_zfx(img_bin,100,xgd_l); 
    if(xgd_flag_r==1) gd_hua_zfx(img_bin,100,xgd_r);
  }
  else//flg_msycsz==1 马上要出十字了
  {
    zhidao_wandao_bxcx(img_bin,msycsz_bx_start_i,10);
  }
}