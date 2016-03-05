/*
 * main.c
 *
 *  Created on: 2015年8月6日
 *      Author: HK
 */
/*#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
*/



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"





#include "My_uart.h"
#include "mpu6050.h"
#include "Four_PWM.h"
#include "delay.h"
#include "pidrevise.h"
#include "CTYPE.h"

#define MAX_HZ	0.8
#define MIN_HZ	0.3

#define N 5
float wx,wy,wz;								//X轴角速度		Y轴角速度		Z轴角速度
float x,y,z;								//三轴翻滚角
float wx_buf[N],wy_buf[N],wz_buf[N];		//三轴角速度缓冲区
float x_buf[N],y_buf[N],z_buf[N];			//三轴翻滚角缓冲区
int Downflag=-30;
unsigned short ma0=4800;  							//
unsigned short mb0=4800;
unsigned short mc0=4800;
unsigned short md0=4800;

long moto1=0,moto2=0,moto3=0,moto4=0;

float ekx = 0;          		//控制偏差
float ukx;             			//控制量

float eky=0;           			//控制偏差
float uky;            			//控制量

float ekz=0;           			//控制偏差
float ukz;            			//控制量

double sum_ekx = 0;				//偏差和
double sum_eky = 0;				//偏差和

float x0=-0.23;         	 	//期望的设定值
//float y0=1.30;          		//期望的设定值
float y0=-0.23;
float z0=0.0;          			//期望的设定值
float wx0=-1.00;
//float wy0=-0.06;
float wy0=-1.00;
float wz0=-0.06;

float Kpx = 2.80;
float Kix = 0.05;
float Kdx = 1.35;	//

float Kpy = 2.80;
float Kiy = 0.05;
float Kdy = 1.35;

double propa = 2.8, intea = 0.05, diffa = 1.35, propb = 2.80, inteb = 0.05, diffb = 1.35;
char Spropa[20], Sintea[20], Sdiffa[20], Spropb[20], Sinteb[20], Sdiffb[20];
void PID(void);

int main()
{
	unsigned char i=0,j=0;
	float tmp;
	unsigned char m=0,n=0;
	float acc[3], gyr[3], angle[3];

//	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	UART1_INIT();
	UART0_init();
    InitFourPWM();

    UARTprintf("%d", (long)(-1.0));
    //while(1);
    while(1)
    {
    	DataGet(acc, gyr, angle);
    }

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 8000);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 8000);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 8000);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 8000);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 0);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 0);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);

    EnablePWM();
    for (i = 0; i < 60; i++)
    {
    	DisEnablePWM();
    	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 8000);
    	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 8000);
    	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 8000);
    	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 8000);

    	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 8000*(0+0.01*i));
    	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 8000*(0+0.01*i));
    	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 8000*(0+0.01*i));
    	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 8000*(0+0.01*i));
    	EnablePWM();
    	SysCtlDelay(SysCtlClockGet()*0.1);
    }

    while(1)
    {

    	int index;
    	for(index = 0; index < N; index++)
    	{
    		DataGet(acc, gyr, angle);
//    		UARTprintf("a:%d %d %d ",(int)(acc[0]*1000), (int)(acc[1]*1000), (int)(acc[2]*1000));
//    		UARTprintf("w:%d %d %d ",(int)gyr[0], (int)gyr[1], (int)gyr[2]);
//    		UARTprintf("A:%d %d %d ",(int)angle[0], (int)angle[1], (int)angle[2]);
    		wx_buf[index] = gyr[0];
    		wy_buf[index] = gyr[1];
    		wz_buf[index] = gyr[2];

    		x_buf[index] = angle[0];
    		y_buf[index] = angle[1];
    		z_buf[index] = angle[2];
    	}

    	for( m=0;m<N-1;m++)
    	{
    		for( n=0;n<N-1-m;n++)
    		{
    			if(wx_buf[n]>wx_buf[n+1])
    			{
    				tmp=wx_buf[n];
    				wx_buf[n]=wx_buf[n+1];
    				wx_buf[n+1]=tmp;
    				tmp=wy_buf[n];
    				wy_buf[n]=wy_buf[n+1];
    				wy_buf[n+1]=tmp;
    				tmp=wz_buf[n];
    				wz_buf[n]=wz_buf[n+1];
    				wz_buf[n+1]=tmp;
    			}
    		}
    	}

/*    	wx=wx_buf[N/2];
    	wy=wy_buf[N/2];
    	wz=wz_buf[N/2];
*/
    	wx = (wx_buf[0] + wx_buf[1] + wx_buf[2] + wx_buf[3] + wx_buf[4]) / N;
    	wy = (wy_buf[0] + wy_buf[1] + wy_buf[2] + wy_buf[3] + wy_buf[4]) / N;
    	wz = (wz_buf[0] + wz_buf[1] + wz_buf[2] + wz_buf[3] + wz_buf[4]) / N;
//    	UARTprintf("GYR:%d %d %d ", (int)wx, (int)wy, (int)wz);

    	for( m=0;m<N-1;m++)
    	{
    		for( n=0;n<N-1-m;n++)
    		{
    			if(x_buf[n]>x_buf[n+1])
    			{
    				tmp=x_buf[n];
    				x_buf[n]=x_buf[n+1];
    				x_buf[n+1]=tmp;
    				tmp=y_buf[n];
    				y_buf[n]=y_buf[n+1];
    				y_buf[n+1]=tmp;
    				tmp=z_buf[n];
    				z_buf[n]=z_buf[n+1];
    				z_buf[n+1]=tmp;
    			}
    		}
    	}
/*    	x=x_buf[N/2];
    	y=y_buf[N/2];
    	z=z_buf[N/2];
*/
    	x = (x_buf[0] + x_buf[1]+ x_buf[2]+ x_buf[3]+ x_buf[4]) / 5;
    	y = (y_buf[0] + y_buf[1]+ y_buf[2]+ y_buf[3]+ y_buf[4]) / 5;
    	z = (z_buf[0] + z_buf[1]+ z_buf[2]+ z_buf[3]+ z_buf[4]) / 5;
//    	UARTprintf("ANGLE:%d %d %d ", (int)x, (int)y, (int)z);
    	PID();
    }
}


void PID(void)
{
/*	float Kpx = 2.80;
	float Kix = 0.05;
	float Kdx = 1.35;	//

	float Kpy = 2.80;
	float Kiy = 0.05;
	float Kdy = 1.35;
*/
//float Kpz = 0.00;
//float Kiz = 0.50;
//float Kdz = 0.00;

	wx=wx-wx0;
	wy=wy-wy0;
	wz=wz-wz0;
	eky = y0 - y;          											//
	//if(fabs(eky) > 30)brake();								//
	sum_eky += eky;
	uky = Kpy * eky + Kiy * sum_eky - Kdy * wy; //
																								//
	PWM_0_Width = ma0 - uky;
	PWM_2_Width = mb0 + uky;

	ekx = x0 - x;          											//
	//if(fabs(ekx) > 30)brake();								//
	sum_ekx += ekx;
	ukx = Kpx * ekx + Kix * sum_ekx - Kdx * wx; //

	PWM_1_Width = mc0 - ukx;
	PWM_3_Width = md0 + ukx;

	if (PWM_0_Width > 8000 * MAX_HZ)
		PWM_0_Width = 8000 * MAX_HZ;
	if (PWM_1_Width > 8000 * MAX_HZ)
		PWM_1_Width = 8000 * MAX_HZ;
	if (PWM_2_Width > 8000 * MAX_HZ)
		PWM_2_Width = 8000 * MAX_HZ;
	if (PWM_3_Width > 8000 * MAX_HZ)
		PWM_3_Width = 8000 * MAX_HZ;
	if (PWM_0_Width < 8000 * MIN_HZ)
		PWM_0_Width = 8000 * MIN_HZ;
	if (PWM_1_Width < 8000 * MIN_HZ)
		PWM_1_Width = 8000 * MIN_HZ;
	if (PWM_2_Width < 8000 * MIN_HZ)
		PWM_2_Width = 8000 * MIN_HZ;
	if (PWM_3_Width < 8000 * MIN_HZ)
		PWM_3_Width = 8000 * MIN_HZ;

	//printf("%5d%5d%5d%5d ",MA,MB,MC,MD);
//	Moto_PwmRflash(moto1, moto2, moto3, moto4);
  SetPWM();
}

void UART0_Handler(void)
{
    static int i = 0;
    static int flag = 0;
	static char dat[100] = {0};
	int k = 0;
	char temp;
	char realdat[100] = {0};

    uint32_t ITStatus;
    ITStatus = UARTIntStatus(UART0_BASE, true);    //中断标志位
    UARTIntClear(UART0_BASE, ITStatus);      //清除中断标志

    if (flag)
    {
    	temp = UARTCharGet(UART0_BASE);
    	//UARTCharPut(UART0_BASE, temp);
    	dat[i] = temp;
    	i++;
    	if (dat[i-1] == '\r')
    	{
    		dat[i-1] = 0;
    		flag = 0;
    		i = 0;
    		//UARTprintf("%s", dat);
    		if (dat[1] == 'P' && dat[5] == 'A')
    		{
    			for (k = 0; dat[k+6] != 0; k++)
    				realdat[k] = dat[k+6];
    			propa = atof(realdat);
    		}
    		else if (dat[1] == 'I' && dat[5] == 'A')
    		{
    			for (k = 0; dat[k+6] != 0; k++)
    				realdat[k] = dat[k+6];
    			intea = atof(realdat);
    		}
    		else if (dat[1] == 'D' && dat[5] == 'A')
    		{
    			for (k = 0; dat[k+6] != 0; k++)
    				realdat[k] = dat[k+6];
    			diffa = atof(realdat);
    		}
    		else if (dat[1] == 'P' && dat[5] == 'B')
    		{
    			for (k = 0; dat[k+6] != 0; k++)
    				realdat[k] = dat[k+6];
    			propb = atof(realdat);
    		}
    		else if (dat[1] == 'I' && dat[5] == 'B')
    		{
    			for (k = 0; dat[k+6] != 0; k++)
    				realdat[k] = dat[k+6];
    			inteb = atof(realdat);
    		}
    		else if (dat[1] == 'D' && dat[5] == 'B')
    		{
    			for (k = 0; dat[k+6] != 0; k++)
    				realdat[k] = dat[k+6];
    			diffb = atof(realdat);
    		}
    		Kpx = propa;
    		Kix = intea;
    		Kdx = diffa;

    		Kpy = propb;
    		Kiy = inteb;
    		Kdy = diffb;
//    		UARTprintf("PROPA:%d, INTEA:%d, DIFFA:%d, PROPB:%d, INTEB:%d, DIFFB:%d\n", (int)(propa*1000), (int)(intea*1000), (int)(diffa*1000), (int)(propb*1000), (int)(inteb*1000), (int)(diffb*1000));
    		ftoa(Spropa, propa, 5);
    		ftoa(Sintea, intea, 5);
    		ftoa(Sdiffa, diffa, 5);
    		ftoa(Spropb, propb, 5);
    		ftoa(Sinteb, inteb, 5);
    		ftoa(Sdiffb, diffb, 5);

    		UART0_send_string("PROPA: ");
    		UART0_send_string(Spropa);
    		UART0_send_string(" INTEA: ");
    		UART0_send_string(Sintea);
    		UART0_send_string(" DIFFA : ");
    		UART0_send_string(Sdiffa);
    		UART0_send_string(" PROPB: ");
    		UART0_send_string(Spropb);
    		UART0_send_string(" INTEB: ");
    		UART0_send_string(Sinteb);
    		UART0_send_string(" DIFFB : ");
    		UART0_send_string(Sdiffb);
    		UART0_send_string("\n");
    	}
    }
    else if (flag == 0)
    {
		temp = UARTCharGet(UART0_BASE);
		if (temp == '@')
		{
			flag = 1;
			dat[0] = '@';
			i++;
		}
    }

}
