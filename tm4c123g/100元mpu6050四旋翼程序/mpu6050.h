/*
 * mpu6050.h
 *
 *  Created on: 2015-7-26
 *      Author: cc
 */

#ifndef MPU6050_H_
#define MPU6050_H_

int DataGet(float *a, float *w, float *A);
void LCD12864_Delay(unsigned int n);

#endif /* MPU6050_H_ */
