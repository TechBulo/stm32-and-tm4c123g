#ifndef _MOTO_H_
#define _MOTO_H_

#include "Four_PWM.h"

#define CPU_F ((double)8000000) 
#define delayus(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delayms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uint unsigned int
#define Moto_PwmMax 1000

void Moto_PwmRflash(long MOTO1_PWM,long MOTO2_PWM,long MOTO3_PWM,long MOTO4_PWM);
void Moto_Init(void);

#endif
