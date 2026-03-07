/*
 * para.h
 *
 *  Created on: Jan 31, 2026
 *      Author: SAHNG
 */

#ifndef BSP_INC_PARA_H_
#define BSP_INC_PARA_H_

//各种参数放到不同文件中，进行调整太麻烦了，所以我要整理到这个文件中


//oled.h
#define OLED_ADDRESS 0x78							//SSD1306地址



//sk_motor.h
#define SERVO_WORK_MID		158		//舵机初始中值，试验出来的
#define SERVO_WORK_MAX_L	190		//左转最大值
#define SERVO_WORK_MAX_R	126		//右转最大值
#define SERVO_WORK_INIT		SERVO_WORK_MID

#define MOTOR_INIT_STATE 	GPIO_PIN_RESET	//STBY初始状态
#define MOTOR_ROTATE_INIT	0				//初始转速，0~1000



#endif /* BSP_INC_PARA_H_ */
