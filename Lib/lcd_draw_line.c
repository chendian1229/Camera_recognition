#include "common.h"
#include "lcd_draw_line.h"


void lcd_draw_spx(uint8 position_i,uint16 color)
{
  Site_t site_spx;
  site_spx.y=(uint8)(   ( (float)position_i )/80.0*128.0   );//画水平线
  for(uint8 i_hx=0;i_hx<128;i_hx++)
  {
    site_spx.x=i_hx;    
    LCD_point(site_spx,color);
  }
}
void lcd_draw_czx(uint8 position_j,uint16 color)
{
  Site_t site_czx;
  site_czx.x=(uint8)(   ( (float)position_j )/188.0*128.0   );//画垂直线
  for(uint8 i_hx=0;i_hx<128;i_hx++)
  {
    site_czx.y=i_hx;
    LCD_point(site_czx,color);//画垂直线中线
  }
}