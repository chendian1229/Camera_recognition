#ifndef __MPU6050_H__
#define	__MPU6050_H__

/*
void I2C_Init();
void I2C_Start();
uint8 I2C_SendByte(uint8 byte);
uint8 I2C_ReceiveByte_Ack();
uint8 I2C_ReceiveByte_NoAck();
void I2C_Stop();
*/
void Configure_MPU6050();
void GetData_MPU6050(int16 data_buf[]);
void Init_and_Cali_MPU6050(int16 data_cali_init[]);


#endif