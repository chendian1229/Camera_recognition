/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "mpu6050.h"

//软件I2C相关宏定义     SCL:PTE1 SDA:PTE0
#define SCL_INIT        gpio_init(PTE1,GPO,1)//初始化GPIO SCL输出模式 输出1
#define SCL_IN_MODE     gpio_ddr(PTE1,GPI)
#define SCL_GET         gpio_get(PTE1)
#define SCL_OUT_MODE    gpio_ddr(PTE1,GPO)
#define SCL_OUT_1       gpio_set(PTE1,1)
#define SCL_OUT_0       gpio_set(PTE1,0)

#define SDA_INIT        gpio_init(PTE0,GPO,1)//初始化GPIO SDA输出模式 输出1
#define SDA_IN_MODE     gpio_ddr(PTE0,GPI)
#define SDA_GET         gpio_get(PTE0)
#define SDA_OUT_MODE    gpio_ddr(PTE0,GPO)
#define SDA_OUT_1       gpio_set(PTE0,1)
#define SDA_OUT_0       gpio_set(PTE0,0)

#define I2C_DELAY       asm("nop")//DELAY_US(1)
/**********************************************************************************************
                                  软件I2C GPIO初始化
**********************************************************************************************/ 
void I2C_Init()
{
  //当总线空闲时 两根线均为高电平
  
  SCL_INIT;//SCL 输出模式 高电平
  SDA_INIT;//SDA 输出模式 高电平
}
/**********************************************************************************************
                                      I2C_Start
**********************************************************************************************/ 
void I2C_Start()
{
  //SCL SDA均置高 模拟总线空闲状态
  SCL_OUT_1;//SCL=1
  SDA_OUT_1;//SDA=1
  I2C_DELAY;
  //SCL为高时 SDA下降沿表示起始信号
  SDA_OUT_0;//SDA=0 
  I2C_DELAY;
  //钳住I2C总线 准备发送或接受数据
  SCL_OUT_0;//SCL=0 
  
  //此时SCL=0 SDA=0
}
/**********************************************************************************************
                                     I2C_SendByte
**********************************************************************************************/ 
uint8 I2C_SendByte(uint8 byte)
{
  uint8 ack_flag;//从机应答标志位 1从机已应答 0从机未应答
  uint8 i;       //用于for循环
  
  
  for(i=0;i<8;i++)
	{
		if(	((byte<<i)&0x80)==0x80	)//先发送MSB
		{//说明该位为1
			SDA_OUT_1;//SDA=1
		}
		else
		{//说明该位为0
			SDA_OUT_0;//SDA=0
		}
		SCL_OUT_1;//SCL=1
		I2C_DELAY;
		SCL_OUT_0;//SCL=0
	}
  
  //发送完数据后释放SDA 准备接受来自从机的应答信号
  SDA_IN_MODE;//SDA输入模式
  
  //第9个时钟信号 准备接受来自从机的应答信号
	SCL_OUT_1;//SCL=1
  I2C_DELAY;
  if( SDA_GET==1 )
		ack_flag=0;
	else
		ack_flag=1;
	SCL_OUT_0;//SCL=0 从机响应信号接收完毕
  
  SDA_OUT_MODE;//SDA恢复输出模式
	I2C_DELAY;
  
  
  if(ack_flag==0)
		return 0;
	else
		return 1;
}
/**********************************************************************************************
                                    I2C_ReceiveByte_Ack
**********************************************************************************************/ 
uint8 I2C_ReceiveByte_Ack()
{
  uint8 data;//接收到数据
  uint8 i;   //用于for循环
  
  
  SDA_IN_MODE;//SDA输入模式 准备接受数据
	for(i=0;i<8;i++)
	{
		SCL_OUT_0;
		I2C_DELAY;
		SCL_OUT_1;//开始接受数据
		I2C_DELAY;
		data=data<<1;
		if(SDA_GET==1) data=data+1;
		I2C_DELAY;
	}
	SCL_OUT_0;
  
  SDA_OUT_MODE;
  SDA_OUT_0;//发送响应信号
	SCL_OUT_1;
	I2C_DELAY;
	SCL_OUT_0;
  
  
  return data;
}
/**********************************************************************************************
                                    I2C_ReceiveByte_NoAck
**********************************************************************************************/ 
uint8 I2C_ReceiveByte_NoAck()
{
  uint8 data;//接收到数据
  uint8 i;   //用于for循环
  
  
  SDA_IN_MODE;//SDA输入模式 准备接受数据
	for(i=0;i<8;i++)
	{
		SCL_OUT_0;
		I2C_DELAY;
		SCL_OUT_1;//开始接受数据
		I2C_DELAY;
		data=data<<1;
		if(SDA_GET==1) data=data+1;
		I2C_DELAY;
	}
	SCL_OUT_0;
  
  SDA_OUT_MODE;
  SDA_OUT_1;//发送不响应信号
	SCL_OUT_1;
	I2C_DELAY;
	SCL_OUT_0;
  
  
  return data;
}
/**********************************************************************************************
                                       I2C_Stop
**********************************************************************************************/ 
void I2C_Stop()
{
	SDA_OUT_0;
	SCL_OUT_1;
	I2C_DELAY;
	SDA_OUT_1;
	I2C_DELAY;
	//SCL SDA均置高 总线空闲状态
}
/**********************************************************************************************
                                    配置MPU6050
**********************************************************************************************/
void Configure_MPU6050()
{
  I2C_Init();        //软件I2C GPIO初始化
  
  I2C_Start();       //向MPU6050的0x6b寄存器写0 启动MPU6050
  I2C_SendByte(0xd0);
  I2C_SendByte(0x6b);
  I2C_SendByte(0x00);
  I2C_Stop();
}
/**********************************************************************************************
                                 从MPU6050获取数据
**********************************************************************************************/
void GetData_MPU6050(int16 data_buf[])
{
  uint8 byte[14];//定义为uint8型 存放6050中的14个寄存器值
  
  I2C_Start();
  I2C_SendByte(0xd0);
  I2C_SendByte(0x3b);
  I2C_Start();
  I2C_SendByte(0xd1);
  byte[0] =I2C_ReceiveByte_Ack();//acc_x
  byte[1] =I2C_ReceiveByte_Ack();data_buf[0]=byte[0]<<8|byte[1];
  byte[2] =I2C_ReceiveByte_Ack();//acc_y
  byte[3] =I2C_ReceiveByte_Ack();data_buf[1]=byte[2]<<8|byte[3];
  byte[4] =I2C_ReceiveByte_Ack();//acc_z
  byte[5] =I2C_ReceiveByte_Ack();data_buf[2]=byte[4]<<8|byte[5];
  byte[6] =I2C_ReceiveByte_Ack();//temp
  byte[7] =I2C_ReceiveByte_Ack();data_buf[3]=byte[6]<<8|byte[7];
  byte[8] =I2C_ReceiveByte_Ack();//gyr_x
  byte[9] =I2C_ReceiveByte_Ack();data_buf[4]=byte[8]<<8|byte[9];
  byte[10]=I2C_ReceiveByte_Ack();//gyr_y
  byte[11]=I2C_ReceiveByte_Ack();data_buf[5]=byte[10]<<8|byte[11];
  byte[12]=I2C_ReceiveByte_Ack();//gyr_z
  byte[13]=I2C_ReceiveByte_NoAck();data_buf[6]=byte[12]<<8|byte[13];
  I2C_Stop();
}
/**********************************************************************************************
                    注：data_cali_init数组保存校准及初始化后的数值
**********************************************************************************************/
void Init_and_Cali_MPU6050(int16 data_cali_init[])
{
  int16 mpu6050_buffer[7];//存放来自MPU6050的数据
  int32 mpu6050_buffer_sum[7]={0};//用来求和 一定要全部初始化为0

  int16 i;
  for(i=0;i<1000;i++)//采样1000次 求和
  {
    GetData_MPU6050(mpu6050_buffer);

    //int32 <-- int16
    mpu6050_buffer_sum[0]+=mpu6050_buffer[0];//acc_x
    mpu6050_buffer_sum[1]+=mpu6050_buffer[1];//acc_y
    mpu6050_buffer_sum[2]+=mpu6050_buffer[2];//acc_z
    mpu6050_buffer_sum[3]+=mpu6050_buffer[3];//temp
    mpu6050_buffer_sum[4]+=mpu6050_buffer[4];//gyr_x
    mpu6050_buffer_sum[5]+=mpu6050_buffer[5];//gyr_y
    mpu6050_buffer_sum[6]+=mpu6050_buffer[6];//gyr_z
  }
  
  //求平均值
  mpu6050_buffer_sum[0]/=1000;//acc_x
  mpu6050_buffer_sum[1]/=1000;//acc_y
  mpu6050_buffer_sum[2]/=1000;//acc_z
  mpu6050_buffer_sum[3]/=1000;//temp
  mpu6050_buffer_sum[4]/=1000;//gyr_x
  mpu6050_buffer_sum[5]/=1000;//gyr_y
  mpu6050_buffer_sum[6]/=1000;//gyr_z

  //通过data_cali_init数组在函数间传递数据
  //int16 <-- int32
  data_cali_init[0]=mpu6050_buffer_sum[0];
  data_cali_init[1]=mpu6050_buffer_sum[1];
  data_cali_init[2]=mpu6050_buffer_sum[2];
  data_cali_init[3]=mpu6050_buffer_sum[3];
  data_cali_init[4]=mpu6050_buffer_sum[4];
  data_cali_init[5]=mpu6050_buffer_sum[5];
  data_cali_init[6]=mpu6050_buffer_sum[6];
}