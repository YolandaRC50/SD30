#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h" 
						  	    													  
void SPI3_Init(void);			 //��ʼ��SPI1��
void SPI3_SetSpeed(uint8_t SpeedSet); //����SPI1�ٶ�   
uint8_t SPI3_ReadWriteByte(uint8_t TxData);//SPI1���߶�дһ���ֽ�
			
#endif

