/*
 * sk_motor.h
 *
 *  Created on: Jan 21, 2026
 *      Author: SAHNG
 *
 *  本程序驱动旨在驱动小车电机：
 *  PWM驱动：PA6
 */

#ifndef BSP_INC_SK_MOTOR_H_
#define BSP_INC_SK_MOTOR_H_


#include "tim.h"
#include "para.h"


void Servo_Init(void);
void Servo_Move(uint8_t CHn);

void Motor_Init(void);
void Motor_Move(uint8_t CHn, uint8_t Start);


#endif /* BSP_INC_SK_MOTOR_H_ */
