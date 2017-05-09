#include "common.h"
#include "_small_func_.h"




/**********************************************************************************************
名    称：              水平从中间同时往两边找“左侧上升沿和右侧下降沿”函数

功能描述： 
入口参数： 一维数组首地址line[]、范围（最大输入93即188个像素全行扫描）
出口参数： 跳变沿存在性及位置

备    注： 只提取左侧的上升沿和右侧的下降沿
**********************************************************************************************/
void f__edge_sp_czj(uint8 line[],uint8 range,uint8 *flag_l,uint8 *left,uint8 *flag_r,uint8 *right)
{
  *flag_l=0;//对 左侧上升沿找到标志位 清零
  *flag_r=0;//对 右侧下降沿找到标志位 清零
  
  //先对最中间的两个像素点 93 94（下标从0开始）进行比较
  if( line[93]!=line[94] )
  {
    if(line[93]<line[94])
    {
      *flag_l=1;
      
      *left=93;
    }
    else
    {
      *flag_r=1;
      
      *right=94;
    }
  }
  
  //比较完最中间两个像素后再继续同时往两边扫
  for( uint8 Cj=1 ; Cj<=range ; Cj++ )//若range==186/2==93 则全部扫描
  {
    //往左边扫
    if( (*flag_l)==0 )
    {
      if( line[93-Cj]==line[93-Cj+1] )
      {
      }
      else
      {
        if( line[93-Cj]<line[93-Cj+1] )
        {
          *flag_l=1;
          
          *left=(93-Cj);
        }
      }
    }
    //往右边扫
    if( (*flag_r)==0 )
    {
      if( line[94+Cj-1]==line[94+Cj] )
      {
      }
      else
      {
        if( line[94+Cj-1]>line[94+Cj] )
        {
          *flag_r=1;
          
          *right=(94+Cj);
        }
      }
    }
    //判断是否还需要再执行循环
    if( (*flag_l==1)&&(*flag_r==1) ) break;
  }
  
  //在给定范围内都扫完了还没有扫到上升沿、下降沿 则返回左右边界
  if(*flag_l==0)
    *left =93-range;
  if(*flag_r==0)
    *right=94+range;
}
/**********************************************************************************************
名    称：            水平从中间同时往两边找“左侧上升沿和右侧下降沿”函数 加强版

功能描述： 
入口参数： 一维数组首地址line[]、范围（最大输入93即188个像素全行扫描）
出口参数： 跳变沿存在性及位置

备    注： 只提取左侧的上升沿和右侧的下降沿
**********************************************************************************************/
void f__edge_sp_czj_plus(uint8 line[],uint8 range,uint8 *flag_l,uint8 *left,uint8 *flag_r,uint8 *right)
{
  *flag_l=0;//对 左侧上升沿找到标志位 清零
  *flag_r=0;//对 右侧下降沿找到标志位 清零
  
  int16 tmpj;
  uint8 S_start_j;
  uint8 S_final_j;
  uint8 X_start_j;
  uint8 X_final_j;
  
  #define DEVIATION 30
  S_start_j = 93+DEVIATION;//从右往左扫 tmpj--
  S_final_j = 93-range;    //range==93 时为 0
  X_start_j = 94-DEVIATION;//从左往右扫 tmpj++
  X_final_j = 94+range;    //range==93 时为 187
  #undef  DEVIATION
  
  //扫上升沿
  for( tmpj=S_start_j ; tmpj>=S_final_j ; tmpj-- )
  {
    if( (*flag_l)==0 )
    {
      if( line[tmpj]==line[tmpj+1] )
      {
      }
      else
      {
        if( line[tmpj]<line[tmpj+1] )
        {
          *flag_l=1;
          
          *left=tmpj;
        }
      }
    }
    //判断是否还需要再执行循环
    if( (*flag_l)==1 ) break;
  }
  
  //扫下降沿
  for( tmpj=X_start_j ; tmpj<=X_final_j ; tmpj++ )
  {
    if( (*flag_r)==0 )
    {
      if( line[tmpj-1]==line[tmpj] )
      {
      }
      else
      {
        if( line[tmpj-1]>line[tmpj] )
        {
          *flag_r=1;
          
          *right=tmpj;
        }
      }
    }
    //判断是否还需要再执行循环
    if( (*flag_r)==1 ) break;
  }
  
  //在给定范围内都扫完了还没有扫到上升沿、下降沿 则返回左右边界
  if(*flag_l==0)
    *left =93-range;
  if(*flag_r==0)
    *right=94+range;
  
//  //本函数的测试代码 放进main函数的while(1)里即可
//  zb_imgarr_t I_a,I_b;
//  I_a.i=50;
//  I_b.i=50;
//  uint8 left_flg,right_flg;
//  f__edge_sp_czj_plus(image_binarized[50],93,&left_flg,&(I_a.j),&right_flg,&(I_b.j));
//  liang_dian_lian_xian(image_binarized,128,I_a,I_b);
}
/**********************************************************************************************
名    称：                  水平从某点开始同时往两边“找上升沿”函数

功能描述： 
入口参数： 一维数组、起始的点、范围
出口参数： 是否存在标志位、位置

备    注： 仅当返回的标志位为1时 位置才会返回 否则*p_position指向的变量不会被更新
**********************************************************************************************/
uint8 f_Sedge_sp_cmd(uint8 line[],uint8 start_position,uint8 range,uint8 *p_position)
{
  uint8 S_flag_l=0;//往左找到上升沿标志位
  uint8 S_flag_r=0;//往右找到上升沿标志位
  
  uint8 position_l;//往左边找 找到的上升沿的位置
  uint8 position_r;//往右边找 找到的上升沿的位置
  
  int16 tmp_l;//被比较的左侧像素下标
  int16 tmp_r;//被比较的右侧像素下标
  
  for( uint8 Cj=1 ; Cj<=range ; Cj++ )
  {
    //往左边扫
    if( S_flag_l==0 )
    {
      //求被比较两个像素的下标并限幅 防止不在0~187范围内
      tmp_l=start_position-Cj;
      tmp_r=start_position-Cj+1;
      if( tmp_l<0 )
        tmp_l=0;
      if( tmp_r>187 )
        tmp_r=187;
      
      //找上升沿
      if( line[tmp_l]==line[tmp_r] )
      {
      }
      else
      {
        if( line[tmp_l]<line[tmp_r] )
        {
          S_flag_l=1;
          
          position_l=tmp_l;
        }
        else
        {
          //说明是下降沿 我们要找的是上升沿 故不要
        }
      }
    }
    //往右边扫
    if( S_flag_r==0 )
    {
      //求被比较两个像素的下标并限幅 防止不在0~187范围内
      tmp_l=start_position+Cj-1;
      tmp_r=start_position+Cj;
      if( tmp_l<0 )
        tmp_l=0;
      if( tmp_r>187 )
        tmp_r=187;
      
      //找上升沿
      if( line[tmp_l]==line[tmp_r] )
      {
      }
      else
      {
        if( line[tmp_l]<line[tmp_r] )
        {
          S_flag_r=1;
          
          position_r=tmp_l;
        }
        else
        {
          //说明是下降沿 我们要找的是上升沿 故不要
        }
      }
    }
    //判断是否还需要再执行循环
    if( (S_flag_l==1)&&(S_flag_r==1) )//必须要这样才可以跳出循环 以防止找错
    {
      break;
    }
  }
  
  //返回是否找到上升沿标志位
  if( (S_flag_l==1)||(S_flag_r==1) ) 
  {
    if( (S_flag_l==1)&&(S_flag_r==0) )
    {
      *p_position=position_l;
    }
    if( (S_flag_l==0)&&(S_flag_r==1) )
    {
      *p_position=position_r;
    }
    if( (S_flag_l==1)&&(S_flag_r==1) )
    {
      *p_position=position_r;
    }
    return 1;
  }
  else
  {
    return 0;
  }
}
/**********************************************************************************************
名    称：                  水平从某点开始同时往两边“找下降沿”函数

功能描述： 水平从某点往两边找下降沿
入口参数： 一维数组、开始的点、范围
出口参数： 是否存在标志位、位置

备    注： 仅当返回的标志位为1时 位置才会返回 否则*p_position指向的变量不会被更新
**********************************************************************************************/
uint8 f_Xedge_sp_cmd(uint8 line[],uint8 start_position,uint8 range,uint8 *p_position)
{
  uint8 X_flag_l=0;//往左找到上升沿标志位
  uint8 X_flag_r=0;//往右找到上升沿标志位
  
  uint8 position_l;//往左边找 找到的上升沿的位置
  uint8 position_r;//往右边找 找到的上升沿的位置
  
  int16 tmp_l;//被比较的左侧像素下标
  int16 tmp_r;//被比较的右侧像素下标
  
  for( uint8 Cj=1 ; Cj<=range ; Cj++ )
  {
    //往左边扫
    if( X_flag_l==0 )
    {
      //求被比较两个像素的下标并限幅 防止不在0~187范围内
      tmp_l=start_position-Cj;
      tmp_r=start_position-Cj+1;
      if( tmp_l<0 )
        tmp_l=0;
      if( tmp_r>187 )
        tmp_r=187;
      
      //找上升沿
      if( line[tmp_l]==line[tmp_r] )
      {
      }
      else
      {
        if( line[tmp_l]>line[tmp_r] )
        {
          X_flag_l=1;
          
          position_l=tmp_r;
        }
        else
        {
          //说明是下降沿 我们要找的是上升沿 故不要
        }
      }
    }
    //往右边扫
    if( X_flag_r==0 )
    {
      //求被比较两个像素的下标并限幅 防止不在0~187范围内
      tmp_l=start_position+Cj-1;
      tmp_r=start_position+Cj;
      if( tmp_l<0 )
        tmp_l=0;
      if( tmp_r>187 )
        tmp_r=187;
      
      //找上升沿
      if( line[tmp_l]==line[tmp_r] )
      {
      }
      else
      {
        if( line[tmp_l]>line[tmp_r] )
        {
          X_flag_r=1;
          
          position_r=tmp_r;
        }
        else
        {
          //说明是下降沿 我们要找的是上升沿 故不要
        }
      }
    }
    //判断是否还需要再执行循环
    if( (X_flag_l==1)&&(X_flag_r==1) )//必须要这样才可以跳出循环 以防止找错
    {
      break;
    }
  }
  
  //返回是否找到上升沿标志位
  if( (X_flag_l==1)||(X_flag_r==1) ) 
  {
    if( (X_flag_l==1)&&(X_flag_r==0) )
    {
      *p_position=position_l;
    }
    if( (X_flag_l==0)&&(X_flag_r==1) )
    {
      *p_position=position_r;
    }
    if( (X_flag_l==1)&&(X_flag_r==1) )
    {
      *p_position=position_l;
    }
    return 1;
  }
  else
  {
    return 0;
  }
}
/**********************************************************************************************
名    称：                 水平“从已知右侧边界往左找左边界”函数

功能描述： 
入口参数： 
出口参数： 

备    注： “从右边界求vj”
**********************************************************************************************/
uint8 f_Sedge_sp_cybjqvj(uint8 line[],uint8 s_j)
{
  uint8 zc_value_j;//扫描到的左侧j值 函数返回值
  
  uint8 flag=0;//是否找到左侧边界标志位
  
  int16 tmp_l;//被比较的左侧像素下标
  int16 tmp_r;//被比较的右侧像素下标
  
  for( int16 Cj=s_j ; Cj>=1 ; Cj-- )//从右往左 故Cj--
  {
    //限幅 防止不在0~187范围内
    tmp_l = Cj-1;
    tmp_r = Cj;
    
    if( tmp_l<0 )
      tmp_l=0;
    if(tmp_r>187)
      tmp_r=187;
    
    //找上升沿
    if( line[tmp_l]==line[tmp_r] )
    {
    }
    else
    {
      if( line[tmp_l]<line[tmp_r] )
      {
        flag=1;
        
        zc_value_j=tmp_l;
        
        break;
      }
      else
      {
        //说明是上升沿 我们要找的是下降沿 故不要
      }
    }
  }
  
  if( flag==0 )//如果往左边扫扫到头了 还没扫到左侧边界 则强行返回下标0
  {
    zc_value_j=0;
  }
  
  return  zc_value_j;
}
/**********************************************************************************************
名    称：                    遍历整个一行 返回所有跳变沿 下降沿

功能描述： 
入口参数： 
出口参数： 

备    注： 
**********************************************************************************************/
n_edge_t f__edge_sp_all(uint8 line[],edge_t edge[])
{
  n_edge_t n_edge;//函数返回值
  
  n_edge.all=0;//所有跳变沿个数清零
  n_edge.s_n=0;//上升沿个数清零
  n_edge.x_n=0;//下降沿个数清零
  
  for( uint8 Cj=0 ; Cj<(IMG_W_USED-1) ; Cj++ )
  {
    if( line[Cj]!=line[Cj+1] )
    {
      n_edge.all++;
      if( line[Cj]<line[Cj+1] )//黑0 < 白255 上升沿
      {
        edge[n_edge.all].sx = 1;
        edge[n_edge.all].wz = Cj;
        
        n_edge.s_n++;
      }
      else                     //白255 > 黑0 下降沿
      {
        edge[n_edge.all].sx = -1;
        edge[n_edge.all].wz = Cj+1;
        
        n_edge.x_n++;
      }
    }
  }
  
  return n_edge;
}
/**********************************************************************************************
名    称：                  从提取中线后的图像中查找中线位置

功能描述： 
入口参数： 
出口参数： 

备    注： 如果没有找到中线坐标 标志位置0 且位置将不会被更新
**********************************************************************************************/
void f__midline(uint8 line[],uint8 gary_v,uint8 range,uint8 *flag,uint8 *position)
{
  *flag=0;//指示是否查找到中线位置
  
  //最中间的两个像素点 93 94（下标从0开始）
  for( uint8 Cj=0 ; Cj<=range ; Cj++ )//range最大为93
  {
    //往左边扫
    if( (*flag)==0 )
    {
      if( line[93-Cj]==gary_v )
      {
        *flag=1;
        
        *position=93-Cj;
        break;
      }
    }
    //往右边扫
    if( (*flag)==0 )
    {
      if( line[94+Cj]==gary_v )
      {
        *flag=1;
        
        *position=94+Cj;
        break;
      }
    }
  }
  
}