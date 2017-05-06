#include "common.h"
#include "_small_func_.h"




/**********************************************************************************************
名    称：                           两点直接连线

功能描述： 输入两点 直接连线
入口参数： 图像数组、要染的灰度值、I_1、I_2
出口参数： 无

备    注： 如果两个点重合 那么这个函数什么也不干
**********************************************************************************************/
void liang_dian_lian_xian(uint8 img[][IMG_W_USED],uint8 gray_v,zb_imgarr_t I_1,zb_imgarr_t I_2)
{
  //只有当两个点不重合时才进行连线操作 否则此函数什么也不干
  if( pdldsfch_img(I_1,I_2)==0 )//说明两个点不重合 继续进行连线操作
  {
    slope_t slope;
    zb_imgarr_t I_t;
    zb_math_t M_1,M_2;
    zb_math_t M_t;
    
    M_1=covert_zb(I_1);
    M_2=covert_zb(I_2);
    
    slope=calcu_slope(M_1,M_2);
    
    if(slope.exist==1)//斜率存在
    {
      if(M_1.x>M_2.x)//顺着x连一次
      {
        I_t=I_2;
        I_2=I_1;
        I_1=I_t;
        M_1=covert_zb(I_1);
        M_2=covert_zb(I_2);
      }
      for( M_t.x=M_1.x ; (M_t.x<=M_2.x)&&(M_t.x<=188) ; M_t.x++ )//有时候函数莫名会卡在这里 暂时这样解决下
      {
        M_t.y = (uint8)( slope.value*(M_t.x-M_1.x) + M_1.y );//公式y-y0=k*(x-x0)
        
        I_t=covert_fan_zb(M_t);
        img[I_t.i][I_t.j]=gray_v;
      }

      if( (slope.value>0.01)||(slope.value<-0.01) )
      {
        if(M_1.y>M_2.y)//顺着y连一次 x和y都连一次 让线在像素上时连续的
        {
          I_t=I_2;
          I_2=I_1;
          I_1=I_t;
          M_1=covert_zb(I_1);
          M_2=covert_zb(I_2);
        }
        for( M_t.y=M_1.y ; (M_t.y<=M_2.y)&&(M_t.y<=80) ; M_t.y++ )//有时候函数莫名会卡在这里 暂时这样解决下
        {
          M_t.x = (uint8)((M_t.y-M_1.y)/slope.value + M_1.x) ;//公式y-y0=k*(x-x0)变形
          
          I_t=covert_fan_zb(M_t);
          img[I_t.i][I_t.j]=gray_v;
        }
      }
    }
    else//斜率不存在
    {
      if(M_1.y>M_2.y)
      {
        I_t=I_2;
        I_2=I_1;
        I_1=I_t;
        M_1=covert_zb(I_1);
        M_2=covert_zb(I_2);
      }
      M_t.x=M_1.x;
      for( M_t.y=M_1.y ; (M_t.y<=M_2.y)&&(M_t.y<=80) ; M_t.y++ )//有时候函数莫名会卡在这里 暂时这样解决下
      {
        I_t=covert_fan_zb(M_t);
        img[I_t.i][I_t.j]=gray_v;
      }
    }
  }
}
/**********************************************************************************************
名    称：                  两个点向上画延伸线函数（用于十字补线）

功能描述： 输入两点画出其延长线
入口参数： 图像数组、要染的灰度值、I_1、I_2、要延伸到的高度
出口参数： 无

备    注： 当两点重合或者当两点i值相同时 无法向上画延伸线 函数将什么也不干
**********************************************************************************************/
void xiang_shang_hua_yan_shen_xian(uint8 img[][IMG_W_USED],uint8 gray_v,zb_imgarr_t I_1,zb_imgarr_t I_2)
{
  if( pdldsfch_img(I_1,I_2)==0 )//说明两个点不重合
  {
    if( I_1.i!=I_2.i )//只有当两个点i值不同时 函数才会画延伸线
    {
      zb_imgarr_t I_t;
      zb_math_t M_1,M_2,M_t;
      
      slope_t slope;
      
      uint8 x_height;
      
      M_1=covert_zb(I_1);
      M_2=covert_zb(I_2);
      
      if(M_1.x==M_2.x)//说明斜率不存在
      {
        if(M_1.y>M_2.y)
        {
          I_t=I_2;
          I_2=I_1;
          I_1=I_t;
          M_1=covert_zb(I_1);
          M_2=covert_zb(I_2);
        }
        
        M_t.x=M_1.x;
        M_t.y=80;
        
        I_t.i=i_frm_y(M_t.y);
        I_t.j=j_frm_x(M_t.x);
        liang_dian_lian_xian(img,gray_v,I_1,I_t);
      }
      else            //说明斜率存在
      {
        if(M_1.y>M_2.y)
        {
          I_t=I_2;
          I_2=I_1;
          I_1=I_t;
          M_1=covert_zb(I_1);
          M_2=covert_zb(I_2);
        }
        slope=calcu_slope(M_1,M_2);
        
        x_height=(uint8)( (80-M_1.y)/slope.value + M_1.x );//公式y-y0=k*(x-x0)的变形
        
        M_t.x=x_height;
        M_t.y=80;
        
        I_t.i=i_frm_y(M_t.y);
        I_t.j=j_frm_x(M_t.x);
        liang_dian_lian_xian(img,gray_v,I_1,I_t);
      }
    }
  }
}