#include "common.h"

#include "ctr_motor.h"
#include "encoder.h"
#include "mpu6050.h"
#include "pid.h"

#include "Car_Control.h"




int16 mpu6050_init_cali[7];//存放用于mpu6050的初始化及校准的数据
int16 mpu6050_buf[7];      //存放mpu6050每次读取的数据

float acc_x;//单位m/s2
float acc_z;//单位m/s2
float gyr_y;//单位°/sec

float acc_x_init;//用于求初始角度
float acc_z_init;//同上
float gyr_y_cali;//用于校准陀螺仪

float angle_acc; //直接由acc分解而得
float angle_gyr; //直接由gyr积分而得
float angle_yjhb;//一阶互补而得
float angle_clhd;//由angle_yjhb处理后的 去掉小数点后抖动的数据 用于angle_pid函数

float speed_left;          //左轮速度
float speed_right;         //右轮速度
float speed_average;       //左右轮平均速度
float speed_average_last=0;//上次左右轮平均速度（用于速度的一阶低通滤波）

int64 EncoderCounter_L=0;//左轮编码器计数
int64 EncoderCounter_R=0;//右轮编码器计数

int16 pwm_common;       //由角度环PID算得的pwm值
int16 pwm_dir;          //方向环算的pwm值
int16 pwm_speed_last=0; //速度环pwm值
int16 pwm_speed_now;    //同上
int16 pwm_speed_delta;  //同上
int16 pwm_speed_used;   //同上

int16 MeasuredDir=95;
float TargetSpeed;

/**********************************************************************************************
                                    直立控制初始化初始化
**********************************************************************************************/
void Car_Control_Init()
{
  //电机控制FTM模块初始化
  Init_Control_Motor();
  
  
  //正交解码模块初始化
  Encoder_Init();
  
  
  //MPU6050初始化及校准
  Configure_MPU6050();
  Init_and_Cali_MPU6050(mpu6050_init_cali);
  acc_x_init = mpu6050_init_cali[0] * 19.61330 / 32767; //转换为实际物理值m/s2
  acc_z_init = mpu6050_init_cali[2] * 19.61330 / 32767; //转换为实际物理值m/s2
  gyr_y_cali = mpu6050_init_cali[5] * 250.0 / 32767;    //转换为实际物理值°/s
  angle_gyr  = atan2(acc_x_init, acc_z_init) * 57.29578;//直接由gyr积分角度的初值
  angle_yjhb = atan2(acc_x_init, acc_z_init) * 57.29578;//一阶互补对应角度的初值
}
/**********************************************************************************************
                        直立控制函数(将这段代码放在10ms的定时器中断函数里)
**********************************************************************************************/
/**********************************************************************************************
                        直立控制函数(将这段代码放在10ms的定时器中断函数里)
**********************************************************************************************/
void Car_Control(int16 MeasuredDir,int16 TargetDir,float TargetSpeed)
{
  float K = 0.005;//一阶互补滤波系数
  int16 temp_left,temp_right;//用于编码器计数 速度计算 局部变量
  static uint8 flag_dir=0;//调节方向控制的执行周期
  static uint8 flag_sp=0;//调节方向控制的执行周期
  
  
  GetData_MPU6050(mpu6050_buf);//读取数据
  acc_x = mpu6050_buf[0] * 19.61330 / 32767;
  acc_z = mpu6050_buf[2] * 19.61330 / 32767;
  gyr_y = mpu6050_buf[5] * 250.0 / 32767;
  
  
  gyr_y = gyr_y - gyr_y_cali;//消除陀螺仪温漂
  angle_acc=atan2(acc_x, acc_z) * 57.29578;//直接由acc分解出的值
  angle_gyr=angle_gyr-gyr_y*0.01;//直接由gyr积分出的值
  angle_yjhb = K * angle_acc + (1 - K) * (angle_yjhb - gyr_y * 0.01);//一阶互补滤波
  
  
  angle_clhd=(int)(angle_yjhb*10);
  angle_clhd=angle_clhd/10;
  
  
  //直立pwm
  pwm_common = (int16)( -Angle_PID( angle_clhd+4.5 , 0 ) );
  
  
  //方向pwm
  flag_dir++;
  if( flag_dir == 2 )//1不分频 2二分频 3三分频 以此类推
  {
    flag_dir=0;
    
    //方向控制周期：20ms
    pwm_dir = Dir_PID( MeasuredDir , TargetDir );
  }
  
  
  //速度pwm
  flag_sp++;
  if( flag_sp==1 )
  {
    temp_left = Get_Encoder_Left();
    temp_right = Get_Encoder_Right();
    EncoderCounter_L+=temp_left;//编码器计数
    EncoderCounter_R-=temp_right;
    speed_left =( temp_left  )/5.0;//计算速度 编码器的角速度(r/s)
    speed_right=( -temp_right )/5.0;
    speed_average=(speed_left+speed_right)/2;
    Encoder_Clear();//左右编码器清零
    
    speed_average=speed_average*0.4+speed_average_last*0.6;
    speed_average_last=speed_average;
    
    
    pwm_speed_now=(int16)Speed_PID( speed_average , TargetSpeed );
    pwm_speed_delta=pwm_speed_now-pwm_speed_last;
    pwm_speed_used=pwm_speed_last+(pwm_speed_delta*1/10);
  }
  if( flag_sp == 2 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*2/10);}
  if( flag_sp == 3 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*3/10);}
  if( flag_sp == 4 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*4/10);}
  if( flag_sp == 5 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*5/10);}
  if( flag_sp == 6 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*6/10);}
  if( flag_sp == 7 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*7/10);}
  if( flag_sp == 8 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*8/10);}
  if( flag_sp == 9 ){pwm_speed_used=pwm_speed_last+(pwm_speed_delta*9/10);}
  if( flag_sp == 10 )//1不分频 2二分频 3三分频 以此类推
  {
    flag_sp=0;
    
    pwm_speed_used=pwm_speed_last+(pwm_speed_delta*10/10);
    pwm_speed_last=pwm_speed_now;
  }
  
  
  //直接叠加
  Ctr_Motor_L(pwm_common+pwm_speed_used+pwm_dir);
  Ctr_Motor_R(pwm_common+pwm_speed_used-pwm_dir);
}
/**********************************************************************************************
                     车速过快保护(将这段代码放在主函数的while(1)里即可)
**********************************************************************************************/
void Over_Speed_Protect(uint16 max_speed)
{
  if( (speed_average>max_speed)||(speed_average<-max_speed) )
  {
    disable_irq(PIT0_IRQn);
    Ctr_Motor_L(0);
    Ctr_Motor_R(0);
  }
}
/**********************************************************************************************
                            关闭直立定时器中断 关电机输出
**********************************************************************************************/
void Stop_Stand()
{
  disable_irq(PIT0_IRQn);
  Ctr_Motor_L(0);
  Ctr_Motor_R(0);
}