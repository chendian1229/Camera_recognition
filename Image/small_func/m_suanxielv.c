#include "common.h"
#include "_small_func_.h"




/**********************************************************************************************
名    称：                    输入两个点M1、M2计算出斜率

功能描述： 输入两点 计算斜率
入口参数： M1、M2
出口参数： 斜率的存在性及其大小

备    注： ★输入的两点一定不能重合 否则函数输出将无意义★
**********************************************************************************************/
slope_t calcu_slope(zb_math_t M_1,zb_math_t M_2)
{
  slope_t slope_return;
  
  if( (M_1.x)==(M_2.x) )//斜率不存在
  {
    slope_return.exist=0;
  }
  else//斜率存在
  {
    slope_return.exist=1;
    
    slope_return.value = (float)(M_1.y-M_2.y)/(float)(M_1.x-M_2.x) ;
  }
  
  return slope_return;
}