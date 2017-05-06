#ifndef __CAR_CONTROL_H__
#define __CAR_CONTROL_H__


void Car_Control_Init();
void Car_Control(int16 MeasuredDir,int16 TargetDir,float TargetSpeed);
void Over_Speed_Protect(uint16 max_speed);
void Stop_Stand();


#endif