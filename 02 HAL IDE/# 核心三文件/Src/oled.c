/*
 * oled.c
 *
 *  Created on: Jan 27, 2026
 *      Author: SAHNG
 */

/*CH1116为例
 * 指令通信：0x7A 0x00 0xXX
 * 数据通信：0x7A 0x40 0xXX
 */

/**
 * 地址：
 * 		CH1116： 0x7A
 * 		SSD1306：0x78
 *
 */

#include "oled.h"
#include "oled_font.h"


void OLED_SendCmd(uint8_t cmd){			//发送指令 0x00
	uint8_t sendBuffer[2];
	sendBuffer[0] = 0x00;	//指令通信标识
	sendBuffer[1] = cmd;	//要发送的指令
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, 2, HAL_MAX_DELAY);
}

void OLED_SendByte(uint8_t Byte){
	uint8_t sendBuffer[2];
	sendBuffer[0] = 0x40;
	sendBuffer[1] = Byte;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, 2, HAL_MAX_DELAY);
}

void OLED_SetCursor(uint8_t Y, uint8_t X){	//设置初始写入位置
	OLED_SendCmd(0xB0 | Y);	//设置页地址为第Y页
	OLED_SendCmd(0x00 | ( X & 0x0F));		//列地址低四位0x
	OLED_SendCmd(0x10 | ((X & 0xF0) >> 4)); //列地址高四位1x
	//设置页地址0，列地址00（列地址，若00发送00-10；若25发送02-15；以此类推）
}

void OLED_Clear(void){
	uint8_t i, j;
	for (j = 0; j < 8; j++){
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++){
			OLED_SendByte(0x00);
		}
	}
}


/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char){	//注意，需要用‘ ’括住char
	uint8_t i;
	//显示上半部分
	OLED_SetCursor((Line-1)*2, (Column-1)*8);
	for(i=0; i<8; i++){
		OLED_SendByte(OLED_F8x16[Char - ' '][i]);	//Char - " ", 得到ASCII表中的位置
	}
	//显示下半部分
	OLED_SetCursor((Line-1)*2 + 1, (Column-1)*8);
	for(i=0; i<8; i++){
		OLED_SendByte(OLED_F8x16[Char - ' '][i + 8]);	//Char - " ", 得到ASCII表中的位置
	}
}
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String){
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)	//计算X的Y次方，方便之后显示数字
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}
/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}
/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}
/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}



void OLED_Test(){
	OLED_SetCursor(0, 10);

//	发送到屏幕显示的指令
//	uint8_t sendBuffer[] = {0x40, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
//	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, sendBuffer, sizeof(sendBuffer), HAL_MAX_DELAY);

	OLED_ShowChar(1, 1, 'A');	//发送单个字符
	OLED_ShowString(1, 3, "Hello");
	OLED_ShowNum(2, 1, 10, 2);
	OLED_ShowHexNum(2, 4, 0xAA, 2);
	OLED_ShowBinNum(2, 7, 1010, 4);



}

void OLED_Init_SSD1306(){

	// OLED SSD1306 init code as below
	OLED_SendCmd(0xAE);	//关闭显示

	OLED_SendCmd(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_SendCmd(0x80);

	OLED_SendCmd(0xA8);	//设置多路复用率
	OLED_SendCmd(0x3F);

	OLED_SendCmd(0xD3);	//设置显示偏移
	OLED_SendCmd(0x00);

	OLED_SendCmd(0x40);	//设置显示开始行

	OLED_SendCmd(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置

	OLED_SendCmd(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_SendCmd(0xDA);	//设置COM引脚硬件配置
	OLED_SendCmd(0x12);

	OLED_SendCmd(0x81);	//设置对比度控制
	OLED_SendCmd(0xCF);

	OLED_SendCmd(0xD9);	//设置预充电周期
	OLED_SendCmd(0xF1);

	OLED_SendCmd(0xDB);	//设置VCOMH取消选择级别
	OLED_SendCmd(0x30);

	OLED_SendCmd(0xA4);	//设置整个显示打开/关闭

	OLED_SendCmd(0xA6);	//设置正常/倒转显示

	OLED_SendCmd(0x8D);	//设置充电泵
	OLED_SendCmd(0x14);

	OLED_SendCmd(0xAF);	//开启显示
}

void OLED_Init_CH1116(){
	// OLED CH1116 init code as below
	OLED_SendCmd(0xAE); /*关闭显示 display off*/

	OLED_SendCmd(0x02); /*设置列起始地址 set lower column address*/
	OLED_SendCmd(0x10); /*设置列结束地址 set higher column address*/

	OLED_SendCmd(0x40); /*设置起始行 set display start line*/

	OLED_SendCmd(0xB0); /*设置页地址 set page address*/

	OLED_SendCmd(0x81); /*设置对比度 contract control*/
	OLED_SendCmd(0xCF); /*128*/

	OLED_SendCmd(0xA1); /*设置分段重映射 从右到左 set segment remap*/

	OLED_SendCmd(0xA6); /*正向显示 normal / reverse*/

	OLED_SendCmd(0xA8); /*多路复用率 multiplex ratio*/
	OLED_SendCmd(0x3F); /*duty = 1/64*/

	OLED_SendCmd(0xAD);
	OLED_SendCmd(0x8B);

	OLED_SendCmd(0x33);

	OLED_SendCmd(0xC8);

	OLED_SendCmd(0xD3);
	OLED_SendCmd(0x00);

	OLED_SendCmd(0xD5);
	OLED_SendCmd(0xC0);

	OLED_SendCmd(0xD9);
	OLED_SendCmd(0x1F);

	OLED_SendCmd(0xDA);
	OLED_SendCmd(0x12);

	OLED_SendCmd(0xDB);
	OLED_SendCmd(0x40);

	OLED_SendCmd(0xAF);
}


void OLED_Init(){
	//gpio pin set (since i connect GND to PB6, VCC to PB7)
//	HAL_GPIO_WritePin(OLED_GND_GPIO_Port, OLED_GND_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(OLED_VCC_GPIO_Port, OLED_VCC_Pin, GPIO_PIN_SET);

	// 上电延时，确保OLED电源稳定
	HAL_Delay(100);

	//Set IC init code
	if(OLED_ADDRESS == 0x78) OLED_Init_SSD1306();
	if(OLED_ADDRESS == 0x7A) OLED_Init_CH1116();

	//OLED清屏
	OLED_Clear();
}
