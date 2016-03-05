用户需在调用函数添加#include"mma7361.h"
还需定义__IO float Value[ADC_TONGDAO_NUM];

本库由于是传感器所以在使用时需要用到ad通道，使用时需要注意的有开启a0 a1 a2三个通道（可以修改，但要更改adc1中的东西），用户使用时需定义    double Vx = 0,Vy = 0,Vz = 0;该库为用户提供了void MMA_Get_Jiao (double *Vx,double *Vy,double *Vz ),使用时只需对定义的Vx = 0,Vy = 0,Vz = 0;取地址则可返回各个方向的角度

