/*
 * mpu6050.c
 *
 *  Created on: 2015-7-26
 *      Author: cc
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "My_uart.h"
#include "mpu6050.h"

void LCD12864_Delay(unsigned int n)
{
    unsigned int i = 0;
    for(i = n;i > 0;i--)
            SysCtlDelay(SysCtlClockGet() / 3000);
}

int DataGet(float *a, float *w, float *A)
{
	char temp1, temp2, temp, count = 0, buffer[55], ucStra[6], ucStrw[6], ucStrAngle[6];
	float Value[3];
	short ucYear, ucMonth, ucDay, ucHour, ucMinute;
	float ucSecond, usMiliSecond, Second, Acc[3], Gyro[3], Angle[3];
	int Mag[3];
	long height;


	buffer[0] = 0x55;
	buffer[1] = 0x51;

	while(1)
	    {
			temp1 = UARTCharGet(UART1_BASE);
			//UARTprintf("%x ", temp1);
			temp2 = UARTCharGet(UART1_BASE);
			//UARTprintf("%x ", temp2);
			count = 2;
			if (temp1 == 0x55 && temp2 == 0x50)
			{
				//UARTprintf("%x ", temp1);
				//UARTprintf("%x ", temp2);
				while(1)
				{
					temp = UARTCharGet(UART1_BASE);
					//UARTprintf("%x ", temp);
					buffer[count] = temp;
					count++;
					if (count == 77)
						{
						//LCD12864_Delay(1000);
							count = 0;
							//if (buffer[11] == 0x55 && buffer[12] == 0x51 && buffer[22] == 0x55 && buffer[23] == 0x52 && buffer[33] == 0x55 && buffer[34] == 0x53 && buffer[44] == 0x55 && buffer[45] == 0x54 && buffer[55] == 0x55 && buffer[56] == 0x55 && buffer[66] == 0x55 && buffer[67] == 0x56)
							if (buffer[11] == 0x55 && buffer[12] == 0x51 && buffer[22] == 0x55 && buffer[23] == 0x52 && buffer[33] == 0x55 && buffer[34] == 0x53 && buffer[44] == 0x55 && buffer[45] == 0x54)
							{
										//UARTprintf("%x %x %x %x %x %x %x %x %x %x %x",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10]);

										//UARTCharPut(UART0_BASE, buffer[1]);
//													ucStra[0]=buffer[2];
//													ucStra[1]=buffer[3];
//													ucStra[2]=buffer[4];
//													ucStra[3]=buffer[5];
//													ucStra[4]=buffer[6];
//													ucStra[5]=buffer[7];
//													ucStrw[0]=buffer[2+11];
//													ucStrw[1]=buffer[3+11];
//													ucStrw[2]=buffer[4+11];
//													ucStrw[3]=buffer[5+11];
//													ucStrw[4]=buffer[6+11];
//													ucStrw[5]=buffer[7+11];
//													ucStrAngle[0]=buffer[2+22];
//													ucStrAngle[1]=buffer[3+22];
//													ucStrAngle[2]=buffer[4+22];
//													ucStrAngle[3]=buffer[5+22];
//													ucStrAngle[4]=buffer[6+22];
//													ucStrAngle[5]=buffer[7+22];
//										count = 0;
//
//									a[0] = ((short)(ucStra[1]<<8| ucStra[0]))/32768.0*16;
//									a[1] = ((short)(ucStra[3]<<8| ucStra[2]))/32768.0*16;
//									a[2] = ((short)(ucStra[5]<<8| ucStra[4]))/32768.0*16;
//									//UARTprintf("a:%d %d %d  ",(int)(a[0]*1000),(long)(a[1]*1000),(int)(a[2])*1000);
//
//									w[0] = ((short)(ucStrw[1]<<8| ucStrw[0]))/32768.0*2000;
//									w[1] = ((short)(ucStrw[3]<<8| ucStrw[2]))/32768.0*2000;
//									w[2] = ((short)(ucStrw[5]<<8| ucStrw[4]))/32768.0*2000;
//									//UARTprintf("w:%d %d %d  ",(int)w[0],(long)w[1],(int)w[2]);
//
//									A[0] = ((short)(ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180;
//									A[1] = ((short)(ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180;
//									A[2] = ((short)(ucStrAngle[5]<<8| ucStrAngle[4]))/32768.0*180;

						ucYear 		= (short)(buffer[2]);
						ucMonth 	= (short)(buffer[3]);
						ucDay 		= (short)(buffer[4]);
						ucHour 		= (short)(buffer[5]);
						ucMinute 	= (short)(buffer[6]);
						ucSecond 	= (float)(buffer[7]);
						usMiliSecond=(float)(((unsigned short)buffer[9]<<8)|buffer[8]);
						Second = ucSecond + usMiliSecond/1000;

						Acc[0] = ((float)((short)(buffer[3+11]<<8)|buffer[2+11])/32768*16);
						Acc[1] = ((float)((short)(buffer[5+11]<<8)|buffer[4+11])/32768*16);
						Acc[2] = ((float)((short)(buffer[7+11]<<8)|buffer[6+11])/32768*16);

						Gyro[0] = ((float)((short)(buffer[3+22]<<8)|buffer[2+22])/32768*2000);
						Gyro[1] = ((float)((short)(buffer[5+22]<<8)|buffer[4+22])/32768*2000);
						Gyro[2] = ((float)((short)(buffer[7+22]<<8)|buffer[6+22])/32768*2000);

						Angle[0] = ((float)((short)(buffer[3+33]<<8)|buffer[2+33])/32768*180);
						Angle[1] = ((float)((short)(buffer[5+33]<<8)|buffer[4+33])/32768*180);
						Angle[2] = ((float)((short)(buffer[7+33]<<8)|buffer[6+33])/32768*180);

						Mag[0] = (int)((short)(buffer[3+44]<<8)|buffer[2+44]);
						Mag[1] = (int)((short)(buffer[5+44]<<8)|buffer[4+44]);
						Mag[2] = (int)((short)(buffer[7+44]<<8)|buffer[6+44]);

						//height = ((long)(buffer[9+66]<<24)|(buffer[8+66]<<16)|(buffer[7+66]<<8)|buffer[6+66]);

						//count = 0;

						UARTprintf("t:%d-%d-%d  %d:%d:%d\n",ucYear,ucMonth,ucDay,ucHour,ucMinute,(long)Second);
						UARTprintf("a:%d %d %d  \n",(long)Acc[0],(long)Acc[1],(long)Acc[2]);
						UARTprintf("g:%d %d %d  \n",(long)Gyro[0],(long)Gyro[1],(long)Gyro[2]);
						UARTprintf("ang:%d %d %d  \n",(int)Angle[0],(int)Angle[1],(int)Angle[2]);
						UARTprintf("m:%d %d %d  \n",(long)Mag[0],(long)Mag[1],(long)Mag[2]);
						UARTprintf("h:%d  \n", height);
									return 0;
							}
							break;
						}
					}
			}
			}
}

