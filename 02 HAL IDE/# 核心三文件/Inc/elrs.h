/*
 * elrs.h
 *
 *  Created on: Feb 14, 2026
 *      Author: SAHNG
 */

#ifndef BSP_INC_ELRS_H_
#define BSP_INC_ELRS_H_

#include "main.h"
#include "usart.h"

/*
 * USART配置：
 * USART1
 * 420Kbits，8N1
 * NVIC global打开，DMA RX 普通模式
 *
 * 两种状态，前三位字节（同步字节，长度，类型）：
 * 1. 没有接收到信号：C8，0C，14
 * 2. 接收到遥控信号：C8，18，16
 *
 * */

uint16_t ELRS_CRC8(uint8_t *Data);		//对2~24位进行CRC校验，返回CRC码
void ELRS_DataAnalysis(uint16_t * Channel, uint8_t * ReceiveData); //将输入的第1~8位8进制ELRS负载数据进行解析，返回十进制的CH1~CH10通道信号
uint16_t ELRS_ToPercentage(uint16_t CHn, uint16_t Min, uint16_t Max);


void ELRS_Process(uint8_t * DataA, uint8_t * CH);			//ELRS内部函数调用，减少与主程序之间的耦合
// DataA传输数据，DataD返回数据
void ELRS_Test(uint8_t CHn);





#endif /* BSP_INC_ELRS_H_ */
