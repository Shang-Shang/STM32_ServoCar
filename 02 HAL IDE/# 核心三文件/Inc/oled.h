/*
 * oled.h
 *
 *  Created on: Jan 27, 2026
 *      Author: SAHNG
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_


#include "para.h"
#include "i2c.h"
#include "gpio.h"

//#define OLED_ADDRESS 0x78							//SSD1306地址


void OLED_Init();
void OLED_Test();

void OLED_Clear();
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

//内部引用的程序
void OLED_Init_CH1116();
void OLED_Init_SSD1306();
void OLED_SendCmd(uint8_t cmd);
void OLED_SendByte(uint8_t Byte);
void OLED_SetCursor(uint8_t Y, uint8_t X);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);

#endif /* INC_OLED_H_ */
