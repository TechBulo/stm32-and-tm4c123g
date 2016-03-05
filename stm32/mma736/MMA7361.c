#include "mma7361.h"

extern vu16 ADC1_ConvertedValue[ADC_TONGDAO_NUM];
extern __IO float Value[ADC_TONGDAO_NUM];

void MMA_Get_Jiao (double *Vx,double *Vy,double *Vz )
{
    int i;
    *Vx = 0;
    *Vy = 0;
    *Vz = 0;
    for(i = 0;i<5;i++)
    {
           Value[0] = GetValue(ADC1_ConvertedValue[0]);
            *Vx += (Value[0] - 1.494506);
            *Vx /= 2;
            delay200ms();
            Value[1] = GetValue(ADC1_ConvertedValue[1]);
            *Vy += (Value[1] - 1.655640);
            *Vy /= 2;
            delay200ms();
            Value[2] = GetValue(ADC1_ConvertedValue[2]);
            *Vz += (Value[2] -1.417163);
            *Vz /= 2;
            delay200ms();
     }
     if(*Vx > 0)
        *Vx = *Vx/0.755;
     else
        *Vx = *Vx/0.765;//此处的0.78要参考90和-90的最大
        *Vx = (*Vx > 1)? 1:*Vx;
        *Vx = (*Vx < -1)? -1:*Vx;
     *Vx = asin(*Vx)*180/3.14;

     if(*Vy > 0)
        *Vy = *Vy/0.795;
     else
        *Vy = *Vy/0.782;//此处的0.78要参考90和-90的最大
        *Vy = (*Vy > 1)? 1:*Vy;
        *Vy = (*Vy < -1)? -1:*Vy;
        *Vy = asin(*Vy)*180/3.14;

     if(*Vz > 0)
        *Vz = *Vz/0.745;
     else
        *Vz = *Vz/0.723;//此处的0.78要参考90和-90的最大
        *Vz = (*Vz > 1)? 1:*Vz;
        *Vz = (*Vz < -1)? -1:*Vz;
        *Vz = asin(*Vz)*180/3.14;
}

