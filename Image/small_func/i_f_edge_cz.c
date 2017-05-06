#include "common.h"
#include "_small_func_.h"
/**********************************************************************************************
名    称：                 垂直扫跳变沿

功能描述： 
入口参数： 
出口参数： 

备    注： 任意列垂直扫跳变沿 只是跳变沿 不判断是由黑到白还是由白到黑
**********************************************************************************************/
uint8 f__edge_cz_ry(uint8 line[][IMG_W_USED],uint8 row,uint8 start_i,uint8 final_i)
{
  //从中间底部向上
  uint8 flag=0;
  if( (line[start_i][row]||line[final_i][row])==0 )
  {
        //扫到了跳变沿
      flag=1;
      
  }

  return flag;
}



/**********************************************************************************************
名    称：             从最中间底部向上垂直“扫描跳边沿”

功能描述： 
入口参数： 
出口参数： 

备    注： 第93列或第94列 只是跳变沿 不判断是由黑到白还是由白到黑
**********************************************************************************************/
uint8 f__edge_cz_czjdbxs(uint8 line[][IMG_W_USED],uint8 start_i,uint8 final_i)
{
  //从中间底部向上
  uint8 flag=0;
  for( uint8 Ci=start_i ; Ci>=final_i ; Ci-- )
  {
    //从第 93(94也可以) 列处 由下往上 垂直扫描跳变沿
    if( line[Ci][93]==line[Ci+1][93] )
    {
      //没扫到跳变沿
    }
    else
    {
      //扫到了跳变沿
      flag=1;
      break;//终止整个for循环
    }
  }
  return flag;
}
/**********************************************************************************************
名    称：                       从可调底部求i值

功能描述： 
入口参数： 
出口参数： 

备    注： 用于提取十字的上左右拐点 及 环形黑色中心的左右侧边点
**********************************************************************************************/
uint8 f__edge_cz_cktdbqvi(uint8 line[][IMG_W_USED],uint8 position_j,uint8 start_i)
{
  //从可调底部求i值
  uint8 flag=0;
  uint8 vi_return;
  for( uint8 Ci=start_i ; Ci>=1 ; Ci-- )
  {
    if( line[Ci][position_j]==line[Ci-1][position_j] )
    {
      //没扫到跳变沿
    }
    else
    {
      if( line[Ci][position_j]>line[Ci-1][position_j] )
      {
        //扫到了需要的跳变沿 记下返回值 终止整个for循环
        flag=1;
        
        vi_return = Ci;
        break;
      }
      else
      {
        //没扫到需要的跳变沿
      }
    }
  }
  if( flag==0 )
  {
    vi_return=0;
  }
  
  return vi_return;
}