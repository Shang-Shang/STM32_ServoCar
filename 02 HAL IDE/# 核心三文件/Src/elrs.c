/*
 * elrs.c
 *
 *  Created on: Feb 14, 2026
 *      Author: SAHNG
 */




#include "elrs.h"
#include "oled.h"
#include "usart.h"
#include "dma.h"
#include "gpio.h"



uint8_t  DataLen_elrs = 26;
uint8_t DataB[26] = {0};	//Second, data in process, no change because of DMA receive
uint8_t DataC[26] = {0};	//Third,  data afer CRC check
uint16_t Channels[10];


uint8_t ConnectOK = 0;		//默认0，若连接成功，改为1


uint16_t ELRS_CRC8(uint8_t *Data){
	/*
	进行CRC8校验：
		-2）CRC8校验，CRSF多项式位0xD5，初始值为0x00
	*/
	uint8_t crc = 0;		//CRC记得一定要赋0，不然不一定跳出什么数，跟Data异或运算

	for(uint8_t i=2; i< 25; i++){	//从类型开始，到负载最后一个字节结束
		crc ^= Data[i];
		for(uint8_t j=0;j<8;j++){
			if(crc & 0x80)	crc = (crc<<1) ^ 0xD5;	//如果crc最高项 = 1
			else 			crc <<= 1;
		}
	}

	return crc;
}



void ELRS_DataAnalysis(uint16_t * Channel, uint8_t * ReceiveData)
{	/*
	结果：
	Channel		min		middle		max
		1	 	174		 996		1811
		2	 	174		 996		1811
		3	 	174		 999		1811
		4	 	175		 984		1811

		5	 	191		  /			1792
		6	 	191		  /			1792

		7	 	191		 997		1792
		8	 	191		 997		1792
		9	 	191		 997		1792
		10	 	191		 997		1792
	*/
	//传输的字节，是低位先进来
	Channel[0] = ((uint16_t)ReceiveData[3] >> 0 | (uint16_t)ReceiveData[4] << 8) & 0x07FF;  // 4444 4|444 3333 3333 ;如此余下11位
	Channel[1] = ((uint16_t)ReceiveData[4] >> 3 | (uint16_t)ReceiveData[5] << 5) & 0x07FF;  // ---55|55 5555 4444 4|444
	Channel[2] = ((uint16_t)ReceiveData[5] >> 6 | (uint16_t)ReceiveData[6] << 2 | ReceiveData[7] << 10) & 0x07FF; 	// 7777 777|7 6666 6666 55|55 5555
	Channel[3] = ((uint16_t)ReceiveData[7] >> 1 | (uint16_t)ReceiveData[8] << 7) & 0x07FF;  // 8888|8888 7777 777|7

	Channel[4] = ((uint16_t)ReceiveData[8] >> 4 | (uint16_t)ReceiveData[9] << 4) & 0x07FF;  // 9|999 9999 8888|8888
	Channel[5] = ((uint16_t)ReceiveData[9] >> 7 | (uint16_t)ReceiveData[10]<< 1 | ReceiveData[11]<< 9) & 0x07FF;  // BBBB BB|BB AAAA AAAA 9|999 9999

	Channel[6] = ((uint16_t)ReceiveData[11]>> 2 | (uint16_t)ReceiveData[12]<< 6) & 0x07FF;  // CCC|C CCCC BBBB BB|BB
	Channel[7] = ((uint16_t)ReceiveData[12]>> 5 | (uint16_t)ReceiveData[13]<< 3) & 0x07FF;  // |DDDD DDDD CCC|C CCCC
	Channel[8] = ((uint16_t)ReceiveData[14]>> 0 | (uint16_t)ReceiveData[15]<< 8) & 0x07FF;  // FFFF F|FFF EEEE EEEE|
	Channel[9] = ((uint16_t)ReceiveData[15]>> 3 | (uint16_t)ReceiveData[16]<< 5) & 0x07FF;  // GG|GG GGGG FFFF F|FFF

}


uint16_t ELRS_ToPercentage(uint16_t CHn, uint16_t Min, uint16_t Max) {
	/*
	最简单的实现办法：return (ElrsChannel-Min)*100/(Max-Min);	// (摇杆值-最小值)*100/范围，取整会在返回过程中实现

	但是为了防止意外跳动，使得输出小于0或者大于100，采用以下模式
	*/
	if(CHn > Min && CHn < Max)
	{
		return (CHn-Min)*100/(Max-Min);	// (摇杆值-最小值)*100/范围，取整会在返回过程中实现
	}
	else if(CHn <= Min) 		return 0;
	else if(CHn >= Max)		return 100;
	else                    				return 0;		//逻辑上前三种情况已经没问题了，但显示不讲逻辑，所以我加上了最后这一行
}






void ELRS_Process(uint8_t * DataA, uint8_t * CH){
	/*
	DataA：26长度的数组，是串口DMA在Main程序中接收的数据

	CH：信号处理好后，从0~100之间变化的数据，通道为0~9，请传输10位数组
	*/

	//DMA获取数据，转移到DataB中
	for(uint8_t i = 0; i < DataLen_elrs; i++) DataB[i] = DataA[i];
	//通过CRC校验（25位是校验码）
	if(ELRS_CRC8(DataB) == DataB[25]){
		for(uint8_t i = 0; i < DataLen_elrs; i++) DataC[i] = DataB[i];
		ConnectOK += 1;			//第一次进入这个程序，ConnectOK变为1，TEST中闪烁两秒，之后再进入程序就不再闪烁了。
	}

/*
	//从右向左
	OLED_ShowHexNum(1, 15,DataC[0], 2);
	OLED_ShowHexNum(1, 13,DataC[1], 2);
	OLED_ShowHexNum(1, 11,DataC[2], 2);
	OLED_ShowHexNum(1, 9, DataC[3], 2);
	OLED_ShowHexNum(1, 7, DataC[4], 2);
	OLED_ShowHexNum(1, 5, DataC[5], 2);
	OLED_ShowHexNum(1, 3, DataC[6], 2);
	OLED_ShowHexNum(1, 1, DataC[7], 2);

	OLED_ShowHexNum(2, 15,DataC[8], 2);
	OLED_ShowHexNum(2, 13,DataC[9], 2);
	OLED_ShowHexNum(2, 11,DataC[10], 2);
	OLED_ShowHexNum(2, 9, DataC[11], 2);
	OLED_ShowHexNum(2, 7, DataC[12], 2);
	OLED_ShowHexNum(2, 5, DataC[13], 2);
	OLED_ShowHexNum(2, 3, DataC[14], 2);
	OLED_ShowHexNum(2, 1, DataC[15], 2);

	OLED_ShowHexNum(3, 15,DataC[16], 2);
	OLED_ShowHexNum(3, 13,DataC[17], 2);
	OLED_ShowHexNum(3, 11,DataC[18], 2);
	OLED_ShowHexNum(3, 9, DataC[19], 2);
	OLED_ShowHexNum(3, 7, DataC[20], 2);
	OLED_ShowHexNum(3, 5, DataC[21], 2);
	OLED_ShowHexNum(3, 3, DataC[22], 2);
	OLED_ShowHexNum(3, 1, DataC[23], 2);

	OLED_ShowHexNum(4, 15, DataC[24], 2);
	OLED_ShowHexNum(4, 13, DataC[25], 2);

//	OLED_ShowHexNum(4, 9, DataC[30], 2);
//	OLED_ShowHexNum(4, 7, DataC[31], 2);
//	OLED_ShowHexNum(4, 5, DataC[32], 2);
//	OLED_ShowHexNum(4, 3, DataC[33], 2);

//	OLED_ShowHexNum(1, 14, ELRS_CRC8(DataC), 	 2);
//	OLED_ShowHexNum(2, 14, (uint8_t)(DataC[25]), 2);
//*/

	ELRS_DataAnalysis(Channels, DataC);		//解码信号

/*
	OLED_ShowNum(1, 1, Channels[0], 4);	//右-左右
	OLED_ShowNum(2, 1, Channels[1], 4);	//右-上下
	OLED_ShowNum(3, 1, Channels[2], 4);	//右-油门
	OLED_ShowNum(4, 1, Channels[3], 4);	//右-左右

	OLED_ShowNum(1, 6, Channels[4], 4);	//SE
	OLED_ShowNum(2, 6, Channels[5], 4);	//SE

	OLED_ShowNum(1, 11, Channels[6], 4);
	OLED_ShowNum(2, 11, Channels[7], 4);
	OLED_ShowNum(3, 11, Channels[8], 4);
	OLED_ShowNum(4, 11, Channels[9], 4);
//*/

	/*
		结果：
		Channel		min		middle		max
			1	 	174		 996		1811
			2	 	174		 996		1811
			3	 	174		 999		1811
			4	 	174		 984		1811

			5	 	191		  /			1792
			6	 	191		  /			1792

			7	 	191		 997		1792
			8	 	191		 997		1792
			9	 	191		 997		1792
			10	 	191		 997		1792
		*/
	//转为百分比
	CH[0] = ELRS_ToPercentage(Channels[0], 174, 1811);
	CH[1] = ELRS_ToPercentage(Channels[1], 174, 1811);
	CH[2] = ELRS_ToPercentage(Channels[2], 174, 1811);
	CH[3] = ELRS_ToPercentage(Channels[3], 174, 1811);

	CH[4] = ELRS_ToPercentage(Channels[4], 191, 1792);
	CH[5] = ELRS_ToPercentage(Channels[5], 191, 1792);

	CH[6] = ELRS_ToPercentage(Channels[6], 191, 1792);
	CH[7] = ELRS_ToPercentage(Channels[7], 191, 1792);
	CH[8] = ELRS_ToPercentage(Channels[8], 191, 1792);
	CH[9] = ELRS_ToPercentage(Channels[9], 191, 1792);

	/*
		OLED_ShowNum(1, 1, CH[0], 4);	//右-左右
		OLED_ShowNum(2, 1, CH[1], 4);	//右-上下
		OLED_ShowNum(3, 1, CH[2], 4);	//右-油门
		OLED_ShowNum(4, 1, CH[3], 4);	//右-左右

		OLED_ShowNum(1, 6, CH[4], 4);	//SE
		OLED_ShowNum(2, 6, CH[5], 4);	//SE

		OLED_ShowNum(1, 11, CH[6], 4);
		OLED_ShowNum(2, 11, CH[7], 4);
		OLED_ShowNum(3, 11, CH[8], 4);
		OLED_ShowNum(4, 11, CH[9], 4);
	//*/




}

void ELRS_Test(uint8_t CHn){
	/* OLED屏幕坏了，用板子上的LED灯测试接收效果
	 * 1）若检测信号接收成功，LED会闪频两秒，若不成功，会持续低频闪烁
	 * 2）接收的新城，通过LED闪的频率表示
	 *
	 *
	 *	CHn：取值0~100，CHn越大，LED闪烁越快
	 * */
	if(ConnectOK == 0){
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// toggle
		HAL_Delay(1000);
	}
	if(ConnectOK == 1){
		//闪烁两秒
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay(750);

		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay(750);

		ConnectOK += 1;		//防止再次进入这个程序
	}

	if(ConnectOK > 1){
		//CHn越大，LED闪烁越快
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// light
		HAL_Delay((101 - CHn)*5);
		HAL_GPIO_TogglePin(LED_OnBoard_GPIO_Port, LED_OnBoard_Pin);		// dark
		HAL_Delay((101 - CHn)*5);
	}




}
