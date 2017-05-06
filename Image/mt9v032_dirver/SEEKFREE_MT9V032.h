#ifndef _MT9V032_h
#define _MT9V032_h


extern uint8 mt9v032_finish_flag;//一场图像采集完成标志位

void mt9v032_camera_init();
void VSYNC();
void row_finished();
void seekfree_sendimg_zzf(void *imgaddr, uint32_t imgsize);


#endif