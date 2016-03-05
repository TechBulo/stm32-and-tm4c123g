#include "moto.h"

uint MOTO1_PWM = 0;
uint MOTO2_PWM = 0;    
uint MOTO3_PWM = 0;
uint MOTO4_PWM = 0;

void Moto_PwmRflash(long MOTO1_PWM,long MOTO2_PWM, long MOTO3_PWM, long MOTO4_PWM)
{		
	/*if(MOTO1_PWM>Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	if(MOTO2_PWM>Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	if(MOTO3_PWM>Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	if(MOTO4_PWM>Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
	if(MOTO1_PWM<0)	MOTO1_PWM = 0;
	if(MOTO2_PWM<0)	MOTO2_PWM = 0;
	if(MOTO3_PWM<0)	MOTO3_PWM = 0;
	if(MOTO4_PWM<0)	MOTO4_PWM = 0;*/
	
	//Êä³öPWMµÄÖµ
	PWM_0_Width += MOTO1_PWM;
	PWM_1_Width += MOTO2_PWM;
	PWM_2_Width += MOTO3_PWM;
	PWM_3_Width += MOTO4_PWM;
	if (PWM_0_Width > 4800)
		PWM_0_Width = 4800;
	if (PWM_1_Width > 4800)
		PWM_1_Width = 4800;
	if (PWM_2_Width > 4800)
		PWM_2_Width = 4800;
	if (PWM_3_Width > 4800)
		PWM_3_Width = 4800;
	if (PWM_0_Width < 3200)
		PWM_0_Width = 3200;
	if (PWM_1_Width < 3200)
		PWM_1_Width = 3200;
	if (PWM_2_Width < 3200)
		PWM_2_Width = 3200;
	if (PWM_3_Width < 3200)
		PWM_3_Width = 3200;

	//DisEnablePWM();
	SetPWM();
	//EnablePWM();
}


/*void Moto_Init(void)
{
	InitFourPWM();
	SetPWM();
	EnablePWM();
}
*/
