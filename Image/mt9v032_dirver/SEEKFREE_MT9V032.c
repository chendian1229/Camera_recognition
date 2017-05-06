/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_uart.h"
#include "MK60_dma.h"
#include "SEEKFREE_MT9V032.h"




extern uint8 image_gray[IMG_H_YS][IMG_W_YS];//App/main.c
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头初始化
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_camera_init(void)
{
  //设置参数 具体请参看使用手册
  uint16 light;
  //摄像头配置数组
  uint8 MT9V032_CFG[8];
  DELAY_MS(50);           //延时以保证摄像头上面的51完成上电初始化
  uart_init(UART3,115200);//初始换串口 配置摄像头  
  //帧头
  MT9V032_CFG[0] = 0xFF;//帧头
  //命令位
  //具体请参看使用手册
  MT9V032_CFG[1] = 0x00;
  //分辨率选择位 分辨率切换后 最好断电一次系统
  //其他分辨率具体看说明书 不过列超过188后K60无法采集 提供的更大的分辨率是给STM32具有摄像头接口的用户
  switch(IMG_H_YS)
  {
    case 480:MT9V032_CFG[2] = 8;break;
    case 240:MT9V032_CFG[2] = 9;break;
    case 120:MT9V032_CFG[2] = 10;break;
    default :MT9V032_CFG[2] = 10;break;
  }
  //设置图像帧率 行数不同可设置的范围也不同 范围限制 120行的时候是1-200 240行的时候是1-132 480行的时候是1-66
  MT9V032_CFG[3] = 50;
  //曝光时间越大图像越亮 由于最大曝光时间受到fps与分辨率的共同影响 这里不给出具体范围 可以直接把曝光设置为1000
  //摄像头上的51收到之后会根据分辨率及FPS计算最大曝光时间 然后把曝光时间设置为最大曝光时间
  //并且会返回当前设置的最大曝光时间 这样就可以知道最大可以设置的曝光时间是多少了
  //然后觉得图像过亮 就可以在这个最大曝光值的基础上相应减少
  light = 600;
  MT9V032_CFG[4] = light>>8;    //曝光时间高八位
  MT9V032_CFG[5] = (uint8)light;//曝光时间低八位
  //设置为0表示关闭自动曝光，设置1-63则启用自动曝光，设置的越大图像就越亮  建议开启这个功能，可以有效适应各种场地
  MT9V032_CFG[6] = 0;
  //帧尾
  MT9V032_CFG[7] = 0x5A;
  //通过串口发送配置参数
  uart_putbuff(UART3,MT9V032_CFG,8);
  DELAY_MS(50);//延时以保障上个配置数据51已经成功写入到摄像头
  
  
  //以下用于设置摄像头亮度 与上面的曝光时间是不一样的
  MT9V032_CFG[0] = 0xFF;  //帧头
  MT9V032_CFG[1] = 0x02;  //命令位  
  MT9V032_CFG[2] = 0;     //无用，需设为0
  MT9V032_CFG[3] = 0;     //无用，需设为0
  MT9V032_CFG[4] = 0;     //无用，需设为0
  MT9V032_CFG[5] = 32;    //亮度等级选择 亮度等级 1 - 64
  MT9V032_CFG[6] = 0x35;
  MT9V032_CFG[7] = 0x5A;  //帧尾
  uart_putbuff(UART3,MT9V032_CFG,8);
  DELAY_MS(50);//延时以保障上个配置数据51已经成功写入到摄像头
  
  
  //摄像头采集初始化
  DisableInterrupts;
  dma_portx2buff_init(DMA_CH0,(void *)&PTB_B0_IN,(void *)image_gray,PTA27,DMA_BYTE1,IMG_W_YS,DADDR_KEEPON);
  port_init( PTA27 , ALT1 | DMA_FALLING | PULLDOWN);//PCLK 触发源设置
  DMA_DIS(DMA_CH0);                                 //禁用DMA通道
  DMA_IRQ_CLEAN(DMA_CH0);                           //清除通道传输中断标志位
  DMA_IRQ_EN(DMA_CH0);                              //允许DMA通道中断
  DMA_EN(DMA_CH0);                                  //使能DMA
  disable_irq(PORTA_IRQn);                          //关闭PTA的中断
  port_init( PTA29 , ALT1 | IRQ_FALLING | PULLDOWN);//场中断，下降沿触发中断、下拉
  enable_irq(PORTA_IRQn);
  EnableInterrupts;
}
uint16  now_row = 0;		  //当前正在采集行数
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头场中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void VSYNC(void)
{
	now_row = 0;
	dma_repeat(DMA_CH0,(void *)&PTB_B0_IN,(void *)image_gray[0],IMG_W_YS);
  
//  DMA_EN(DMA_CH0);
}
uint8 mt9v032_finish_flag = 0;//一场图像采集完成标志位
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头DMA完成中断
//  @param      NULL
//  @return     void	
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void row_finished(void)
{
  now_row++;
	if(now_row <  IMG_H_YS) dma_repeat(DMA_CH0,(void *)&PTB_B0_IN,(void *)image_gray[now_row],IMG_W_YS);
	if(now_row >= IMG_H_YS) mt9v032_finish_flag = 1;
  
//  mt9v032_finish_flag = 1;
  
	//本例程对黑边不做处理，大家在使用数据的时候不使用image_gray数组最左边与最右边即可
	//如果分辨率过大，就没办法转存了，因为K60的RAM会不够
  //一副图像从采集开始到采集结束耗时3.33MS左右(50FPS、188*120分辨率)
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_zzf(void *imgaddr, uint32_t imgsize)
{
  uart_putchar(VCAN_PORT,0x00);//发送命令
  uart_putchar(VCAN_PORT,0xff);
  uart_putchar(VCAN_PORT,0x01);
  uart_putchar(VCAN_PORT,0x01);
  uart_putbuff(VCAN_PORT,(uint8 *)imgaddr,imgsize);//发送图像
}