/*
 * sk_motor.c
 *
 *  Created on: Jan 21, 2026
 *      Author: SAHNG
 *
 *  程序细节在头文件中秒速
 */

#include "sk_motor.h"
#include "tim.h"
#include "gpio.h"


void Servo_Init(void){
	//设计舵机角度
	//TIM2输出PWM（PA0），50Hz（设置为PRE=80，ARR=2000）
	//占空比2.5%~12.5%对应0~180度；
	//则比较器设为 50~250，对应0~180度
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SERVO_WORK_INIT+5);	//设置舵机初始角度,试验出的值

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	//矫正
	HAL_Delay(30);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SERVO_WORK_INIT-5);	//设置舵机初始角度,试验出的值
	HAL_Delay(30);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, SERVO_WORK_INIT);	//设置舵机初始角度,试验出的值

	//***********************************************************************************//
	//MID=158，>MID左转（MAX=190），<MID右转(MAX=126)
	//为了避免舵机吱吱的响，可以先设一个大角度，过30ms后，再设一个小5的角度，这样就不会响了

}

void Servo_Move(uint8_t CHn){
	/* 根据输入数值，改变舵机转向。
	 * 参数：
	 * 		CHn：要求0~100，0是左转最大值，100是右转最大值
	 *
	 * 无返回值
	 * */
	CHn = 100 - CHn;	//左右转对调
	uint8_t Servo_Value = (CHn*(SERVO_WORK_MAX_L - SERVO_WORK_MAX_R) + SERVO_WORK_MAX_R*100)/100;	// 最后除以100，不然小数会被消掉
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Servo_Value);
}

void Motor_Init(void){

	//设置PWM波初始值
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, MOTOR_ROTATE_INIT);	//设置比较器的初始值

	//设置IC参数
	//STBY：1=正常工作；0=待机状态
	//IN = 11 制动，01反转，10正转，00停止
	HAL_GPIO_WritePin(MotorIC_STBY_GPIO_Port, MotorIC_STBY_Pin, MOTOR_INIT_STATE);	//STBY = 0, no work

	HAL_GPIO_WritePin(MotorIC1_AIN1_GPIO_Port, MotorIC1_AIN1_Pin, GPIO_PIN_RESET);	//IC1 电机A
	HAL_GPIO_WritePin(MotorIC1_AIN2_GPIO_Port, MotorIC1_AIN2_Pin, GPIO_PIN_SET);	//IC1 电机A

	HAL_GPIO_WritePin(MotorIC1_BIN1_GPIO_Port, MotorIC1_BIN1_Pin, GPIO_PIN_SET);	//IC1 电机B
	HAL_GPIO_WritePin(MotorIC1_BIN2_GPIO_Port, MotorIC1_BIN2_Pin, GPIO_PIN_RESET);	//IC1 电机B

	HAL_GPIO_WritePin(MotorIC2_AIN1_GPIO_Port, MotorIC2_AIN1_Pin, GPIO_PIN_SET);	//IC2 电机A
	HAL_GPIO_WritePin(MotorIC2_AIN2_GPIO_Port, MotorIC2_AIN2_Pin, GPIO_PIN_RESET);	//IC2 电机A

	HAL_GPIO_WritePin(MotorIC2_BIN1_GPIO_Port, MotorIC2_BIN1_Pin, GPIO_PIN_SET);	//IC2 电机B
	HAL_GPIO_WritePin(MotorIC2_BIN2_GPIO_Port, MotorIC2_BIN2_Pin, GPIO_PIN_RESET);	//IC2 电机B


	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);		// Very Very Important
}

void Motor_Move(uint8_t CHn, uint8_t Start){
	/* 根据输入值，改变油门大小
	 * 参数：
	 * 		CHn：	要求0~100，0是油门最小值，100是油门最大值
	 * 		Start:	要求0~100，是电机开始转动的起始值
	 * 无返回值
	 * */
	if(CHn > Start){
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, CHn*10);	//设置比较器的值，范围0~1000
		HAL_GPIO_WritePin(MotorIC_STBY_GPIO_Port, MotorIC_STBY_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(MotorIC_STBY_GPIO_Port, MotorIC_STBY_Pin, GPIO_PIN_RESET);
	}

}



