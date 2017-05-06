#include "common.h"
#include "_small_func_.h"




/**********************************************************************************************
名    称：                      提取“下拐左右”拐点函数

功能描述： 提取“下拐左右”拐点
入口参数： 图像数组
出口参数： 下左右拐点存在标志位 拐点坐标

备    注： 如果没找到下拐点 函数会强制返回一个固定的拐点 需要注意！！！
**********************************************************************************************/
void f__guaidian_X(uint8 img[][IMG_W_YS],uint8 *flg_l,zb_imgarr_t *gd_l,uint8 *flg_r,zb_imgarr_t *gd_r)
{
  uint8       nomeaning_tmp_l;  uint8       nomeaning_tmp_r;
  zb_imgarr_t I_l;              zb_imgarr_t I_r;
  zb_imgarr_t I_last_l;         zb_imgarr_t I_last_r;
  uint8       l_sdkd;           uint8       r_sdkd;
  uint8       l_sdkd_last;      uint8       r_sdkd_last;
  uint8       l_jxz_flg=1;      uint8       r_jxz_flg=1;//是否继续找标志位
  
  uint8 RANGE=93-30;
  
  *flg_l=0;
  *flg_r=0;
  
  //假如函数运行完没有找到下拐点 则会强制返回下面这个固定的拐点
  (*gd_l).i=79;
  (*gd_r).i=79;
  (*gd_l).j=0   + 45;
  (*gd_r).j=187 - 45;
  
  I_last_l.i=79;
  I_last_r.i=79;
  f__edge_sp_czj(img[79],RANGE,&nomeaning_tmp_l,&(I_last_l.j),&nomeaning_tmp_r,&(I_last_r.j));
  l_sdkd_last = 93 - I_last_l.j + 1;
  r_sdkd_last = I_last_r.j - 94 + 1;
  
  if(nomeaning_tmp_l==0)
  {
    l_jxz_flg=0;
  }
  if(nomeaning_tmp_r==0)
  {
    r_jxz_flg=0;
  }
  
  for( int16 Ci=78 ; Ci>=0 ; Ci=Ci-2 )
  {
    if(Ci==78)
    {
      if( (l_jxz_flg==0)&&(r_jxz_flg==0) ) break;
    }
    
    I_l.i=Ci;
    I_r.i=Ci;
    f__edge_sp_czj(img[Ci],RANGE,&nomeaning_tmp_l,&(I_l.j),&nomeaning_tmp_r,&(I_r.j));
    
    l_sdkd=93 - I_l.j + 1;
    r_sdkd=I_r.j - 94 + 1;
    
    if( (l_jxz_flg==1)&&((*flg_l)==0)&&((l_sdkd-l_sdkd_last)>2) )
    {
      *flg_l=1;
      
      (*gd_l).i=I_last_l.i;
      (*gd_l).j=I_last_l.j;
    }
    if( (r_jxz_flg==1)&&((*flg_r)==0)&&((r_sdkd-r_sdkd_last)>2) )
    {
      *flg_r=1;
      
      (*gd_r).i=I_last_r.i;
      (*gd_r).j=I_last_r.j;
    }
    
    if( ((*flg_l)==1)&&((*flg_r)==1) ) break;
    
    I_last_l=I_l;
    I_last_r=I_r;
    l_sdkd_last=l_sdkd;
    r_sdkd_last=r_sdkd;
    
    
    if(nomeaning_tmp_l==0)
    {
      l_jxz_flg=0;
    }
    if(nomeaning_tmp_r==0)
    {
      r_jxz_flg=0;
    }
    if( (l_jxz_flg==0)&&(r_jxz_flg==0) ) break;
  }
  
}
/**********************************************************************************************
名    称：                      提取“上拐左右”拐点函数

功能描述： 
入口参数： 
出口参数： 

备    注： 
**********************************************************************************************/
void f__guaidian_S(uint8 img[][IMG_W_YS],uint8 s_i,uint8 s_j_l,uint8 s_j_r,uint8 *flg_l,zb_imgarr_t *gd_l,uint8 *flg_r,zb_imgarr_t *gd_r)
{
  zb_imgarr_t I_last_l;       zb_imgarr_t I_last_r;     //上次点
  zb_imgarr_t I_l;            zb_imgarr_t I_r;          //本次点
  uint8       szkd_last_l;    uint8       szkd_last_r;  //上次竖直方向宽度
  uint8       szkd_l;         uint8       szkd_r;       //本次竖直方向宽度
  uint8       j_left;         uint8       j_right;

  *flg_l=0;//上拐点存在标志位清零
  *flg_r=0;//同上
  
  //提取“上左”拐点
  I_last_l.j  = s_j_l;
  I_last_l.i  = f__edge_cz_cktdbqvi( img , s_j_l , s_i );
  szkd_last_l = s_i - I_last_l.i + 1 ;
  for( j_left=s_j_l+1 ; j_left<=(93+40) ; j_left+=2 )
  {
    I_l.j  = j_left;
    I_l.i  = f__edge_cz_cktdbqvi( img , j_left , s_i );
    szkd_l = s_i - I_l.i + 1;
    
    if( ((*flg_l)==0)&&((szkd_l-szkd_last_l)>2) )
    {
      *flg_l=1;
      
      (*gd_l).i = I_last_l.i;
      (*gd_l).j = I_last_l.j;
    }
    if( (*flg_l)==1 ) break;
    
    I_last_l=I_l;
    szkd_last_l=szkd_l;
    
//  zb_imgarr_t p;
//  p.i=s_i;
//  p.j=j_left;
//  liang_dian_lian_xian(img,123,p,I_l);
  }
  
  //提取“上右”拐点
  I_last_r.j  = s_j_r;
  I_last_r.i  = f__edge_cz_cktdbqvi( img , s_j_r , s_i );
  szkd_last_r = s_i - I_last_r.i + 1 ;
  for( j_right=s_j_r-1 ; j_right>=(94-40) ; j_right-=2 )
  {
    I_r.j  = j_right;
    I_r.i  = f__edge_cz_cktdbqvi( img , j_right , s_i );
    szkd_r = s_i - I_r.i + 1;
    
    if( ((*flg_r)==0)&&(szkd_r-szkd_last_r)>2 )
    {
      *flg_r=1;
      
      (*gd_r).i = I_last_r.i;
      (*gd_r).j = I_last_r.j;
    }
    if( (*flg_r)==1 ) break;
    
    I_last_r=I_r;
    szkd_last_r=szkd_r;
    
//    zb_imgarr_t p;
//    p.i=s_i;
//    p.j=j_right;
//    liang_dian_lian_xian(img,123,p,I_r);
  }
}
/**********************************************************************************************
名    称：                        在拐点周围画正方形函数

功能描述： 在拐点周围画正方 用于指示拐点
入口参数： 图像数组、要染灰度值、拐点
出口参数： 

备    注： 
**********************************************************************************************/
void gd_hua_zfx(uint8 img[][IMG_W_USED],uint8 gray_v,zb_imgarr_t I_zfx_mid)
{
  zb_imgarr_t ShangZuo,ShangYou;//正方形上左点坐标 正方形上右点坐标
  zb_imgarr_t XiaZuo,XiaYou;    //正方形下左点坐标 正方形下右点坐标
  
  int16 tmp_i;
  int16 tmp_j;
  
  #define ZFXKD_HZFX 4
  tmp_i=I_zfx_mid.i-ZFXKD_HZFX;
  tmp_j=I_zfx_mid.j-ZFXKD_HZFX;
  if(tmp_i<0  ) tmp_i=0;
  if(tmp_i>79 ) tmp_i=79;
  if(tmp_j<0  ) tmp_j=0;
  if(tmp_j>187) tmp_j=187;
  ShangZuo.i=tmp_i;
  ShangZuo.j=tmp_j;
  
  tmp_i=I_zfx_mid.i-ZFXKD_HZFX;
  tmp_j=I_zfx_mid.j+ZFXKD_HZFX;
  if(tmp_i<0  ) tmp_i=0;
  if(tmp_i>79 ) tmp_i=79;
  if(tmp_j<0  ) tmp_j=0;
  if(tmp_j>187) tmp_j=187;
  ShangYou.i=tmp_i;
  ShangYou.j=tmp_j;
  
  tmp_i=I_zfx_mid.i+ZFXKD_HZFX;
  tmp_j=I_zfx_mid.j+ZFXKD_HZFX;
  if(tmp_i<0  ) tmp_i=0;
  if(tmp_i>79 ) tmp_i=79;
  if(tmp_j<0  ) tmp_j=0;
  if(tmp_j>187) tmp_j=187;
  XiaYou.i=tmp_i;
  XiaYou.j=tmp_j;
  
  tmp_i=I_zfx_mid.i+ZFXKD_HZFX;
  tmp_j=I_zfx_mid.j-ZFXKD_HZFX;
  if(tmp_i<0  ) tmp_i=0;
  if(tmp_i>79 ) tmp_i=79;
  if(tmp_j<0  ) tmp_j=0;
  if(tmp_j>187) tmp_j=187;
  XiaZuo.i=tmp_i;
  XiaZuo.j=tmp_j;
  
  liang_dian_lian_xian(img,gray_v,ShangZuo,ShangYou);
  liang_dian_lian_xian(img,gray_v,ShangYou,XiaYou);
  liang_dian_lian_xian(img,gray_v,XiaYou,XiaZuo);
  liang_dian_lian_xian(img,gray_v,XiaZuo,ShangZuo);
  #undef ZFXKD_HZFX
}