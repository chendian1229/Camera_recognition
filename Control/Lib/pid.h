#ifndef __PID_H__
#define __PID_H__


int16 Angle_PID(float measured,float target);
float Speed_PID(float measured,float target);
int16 Dir_PID(int16 measured,int16 target);


#endif