#ifndef __SMALL_FUNC_H__
#define __SMALL_FUNC_H__

/*****************************************公  用  区*****************************************/
#include "lcd_draw_line.h"//lcd上画彩色直线

/*****************************************图像操作区*****************************************/
/*点间运算 （i_djys.c）*/
uint8 pdldsfch_img(zb_imgarr_t I_1,zb_imgarr_t I_2);
zb_imgarr_t zhong_dian_img(zb_imgarr_t I_1,zb_imgarr_t I_2);

/*二值化（i_er_zhi_hua.c）*/
void ChouQu_ErZhi(uint8 gray[][IMG_W_YS],uint8 yuzhi,uint8 binarized[][IMG_W_USED]);

  /*图像上垂直找跳变沿（i_f_edge_cz.c）*/
  uint8    f__edge_cz_czjdbxs(uint8 line[][IMG_W_USED],uint8 start_i,uint8 final_i);
  uint8    f__edge_cz_cktdbqvi(uint8 line[][IMG_W_USED],uint8 position_j,uint8 start_i);


  /*图像上水平找跳变沿（i_f_edge_sp.c）*/
  void     f__edge_sp_czj(uint8 line[],uint8 range,uint8 *flag_l,uint8 *left,uint8 *flag_r,uint8 *right);
  void     f__edge_sp_czj_plus(uint8 line[],uint8 range,uint8 *flag_l,uint8 *left,uint8 *flag_r,uint8 *right);
  uint8    f_Sedge_sp_cmd(uint8 line[],uint8 start_position,uint8 range,uint8 *p_position);
  uint8    f_Xedge_sp_cmd(uint8 line[],uint8 start_position,uint8 range,uint8 *p_position);
  uint8    f_Sedge_sp_cybjqvj(uint8 line[],uint8 s_j);
  n_edge_t f__edge_sp_all(uint8 line[],edge_t edge[]);
  void     f__midline(uint8 line[],uint8 gary_v,uint8 range,uint8 *flag,uint8 *position);

  /*图像上找拐点（i_f_edge.c）*/
  void f__guaidian_X(uint8 img[][IMG_W_YS],uint8 *flg_l,zb_imgarr_t *gd_l,uint8 *flg_r,zb_imgarr_t *gd_r);
  void f__guaidian_S(uint8 img[][IMG_W_YS],uint8 s_i,uint8 s_j_l,uint8 s_j_r,uint8 *flg_l,zb_imgarr_t *gd_l,uint8 *flg_r,zb_imgarr_t *gd_r);
  void gd_hua_zfx(uint8 img[][IMG_W_USED],uint8 gray_v,zb_imgarr_t I_zfx_mid);

/*图像上画线（i_hua_xian.c）*/
void liang_dian_lian_xian(uint8 img[][IMG_W_USED],uint8 gray_v,zb_imgarr_t I_1,zb_imgarr_t I_2);
void xiang_shang_hua_yan_shen_xian(uint8 img[][IMG_W_USED],uint8 gray_v,zb_imgarr_t I_1,zb_imgarr_t I_2);

/*阈值（i_yu_zhi.c）*/
uint8 OTSU(uint8 img_gray[][IMG_W_USED]);

/*****************************************数学运算区*****************************************/
/*点间运算 （m_djys.c）*/
uint8 pdldsfch_math(zb_math_t M_1,zb_math_t M_2);
zb_math_t zhong_dian_math(zb_math_t M_1,zb_math_t M_2);

/*斜率（m_xie_lv.c）*/
slope_t calcu_slope(zb_math_t M_1,zb_math_t M_2);//★输入的两点一定不能重合 否则函数输出将无意义★

/*坐标（m_zuo_biao.c）*/
uint8 x_frm_j(uint8 j);
uint8 y_frm_i(uint8 i);
uint8 j_frm_x(uint8 x);
uint8 i_frm_y(uint8 y);
zb_math_t covert_zb(zb_imgarr_t I_imgarr);
zb_imgarr_t covert_fan_zb(zb_math_t M_math);

#endif