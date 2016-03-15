/*
 * Four_PWM.h
 *
 *  Created on: 2014Äê10ÔÂ14ÈÕ
 *      Author: hanken
 */

#ifndef FOUR_PWM_H_
#define FOUR_PWM_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"

void InitFourPWM(void);
void SetPWM(void);
void EnablePWM(void);
void DisEnablePWM(void);

extern uint32_t PWM_0_Period;
extern uint32_t PWM_1_Period;
extern uint32_t PWM_2_Period;
extern uint32_t PWM_3_Period;

extern float PWM_0_prop;
extern float PWM_1_prop;
extern float PWM_2_prop;
extern float PWM_3_prop;

extern uint32_t PWM_0_Width;
extern uint32_t PWM_1_Width;
extern uint32_t PWM_2_Width;
extern uint32_t PWM_3_Width;

#endif /* FOUR_PWM_H_ */
