#include "common.h"
#include "_small_func_.h"




/**********************************************************************************************
名    称：                     判断两个点是否重合函数（数学坐标系）

功能描述： 判断两个点是否重合
入口参数： M_1、M_2
出口参数： 0/1

备    注： 1重合 0不重合
**********************************************************************************************/
uint8 pdldsfch_math(zb_math_t M_1,zb_math_t M_2)
{
  if(M_1.x==M_2.x)
  {
    if(M_1.y==M_2.y)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}
/**********************************************************************************************
名    称：                        求中间点函数（数学坐标系）

功能描述： 输入两个点 返回中间点
入口参数： M_1、M_2
出口参数： M_mid

备    注： 
**********************************************************************************************/
zb_math_t zhong_dian_math(zb_math_t M_1,zb_math_t M_2)
{
  zb_math_t M_mid;
  
  M_mid.x=(M_1.x+M_2.x)/2;
  M_mid.y=(M_1.y+M_2.y)/2;
  
  return M_mid;
}