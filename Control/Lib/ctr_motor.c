/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_ftm.h"
#include "ctr_motor.h"
/**********************************************************************************************
                                初始化FTM 用于驱动电机
**********************************************************************************************/
void Init_Control_Motor()
{
  //注意！
  //1.Chip/MK60_FTM.h  精度必须配置为1000
  //2.Board/VCAN_key.c 必须将PTD7替换为其他
  //3.电机驱动程序针对左右轮电机特性差异进行了软件补偿？
  
  //左轮电机部分Left
  ftm_pwm_init(FTM0, FTM_CH4,10*1000,0);//FTM0_CH0 10Khz 占空比0 PTD4
  ftm_pwm_init(FTM0, FTM_CH5,10*1000,0);//FTM0_CH1 10Khz 占空比0 PTD5
  //右轮电机部分Right
  ftm_pwm_init(FTM0, FTM_CH6,10*1000,0);//FTM0_CH2 10Khz 占空比0 PTD6
  ftm_pwm_init(FTM0, FTM_CH7,10*1000,0);//FTM0_CH3 10Khz 占空比0 PTD7
}
/**********************************************************************************************
                                    控制左轮电机
**********************************************************************************************/
void Ctr_Motor_L(int16 pwm)//-1000~+1000
{
  if(pwm>900) pwm=900;//限幅  -900~+900
  if(pwm<-900) pwm=-900;
  
  
  if(pwm>0)
  {
    ftm_pwm_duty(FTM0,FTM_CH4,0);
    ftm_pwm_duty(FTM0,FTM_CH5, (uint32)(pwm+0) );
  }
  else
  {
    if(pwm<0)
    {
      ftm_pwm_duty(FTM0,FTM_CH4, (uint32)(-pwm+0) );
      ftm_pwm_duty(FTM0,FTM_CH5,0);
    }
    else
    {
      ftm_pwm_duty(FTM0,FTM_CH4,0);
      ftm_pwm_duty(FTM0,FTM_CH5,0);
    }
  }
}
/**********************************************************************************************
                                    控制右轮电机
**********************************************************************************************/
void Ctr_Motor_R(int16 pwm)//-1000~+1000
{
  if(pwm>900) pwm=900;//限幅  -900~+900
  if(pwm<-900) pwm=-900;
  
  
  if(pwm>0)
  {
    ftm_pwm_duty(FTM0,FTM_CH6,0);
    ftm_pwm_duty(FTM0,FTM_CH7, (uint32)pwm );
  }
  else
  {
    if(pwm<0)
    {
      ftm_pwm_duty(FTM0,FTM_CH6, (uint32)(-pwm) );
      ftm_pwm_duty(FTM0,FTM_CH7,0);
    }
    else
    {
      ftm_pwm_duty(FTM0,FTM_CH6,0);
      ftm_pwm_duty(FTM0,FTM_CH7,0);
    }
  }
}