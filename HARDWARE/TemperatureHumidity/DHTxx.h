#ifndef __DHTxx_H
#define __DHTxx_H 
#include "Includes.h"
#include "stm32f4xx.h" 

#define DHTxx_RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOG
#define DHTxx_GPIOx_Port GPIOG
#define DHTxx_WIRE_GPIO_Pin GPIO_Pin_9

 
 

//IO��������
#define DHTxx_IO_IN()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<9*2;}	//PG9����ģʽ
#define DHTxx_IO_OUT() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<9*2;} 	//PG9���ģʽ 
////IO��������										 
#define DHTxx_OUT_SET GPIO_SetBits(DHTxx_GPIOx_Port, DHTxx_WIRE_GPIO_Pin) 
#define DHTxx_OUT_RESET GPIO_ResetBits(DHTxx_GPIOx_Port, DHTxx_WIRE_GPIO_Pin) 
#define	DHTxx_DQ_IN  GPIO_ReadInputDataBit(DHTxx_GPIOx_Port,DHTxx_WIRE_GPIO_Pin)  //���ݶ˿�	PG9 

uint8_t DHTxx_Init(void);//��ʼ��DHTxx
uint8_t DHTxx_Read_Data(void);//��ȡ��ʪ��
uint8_t DHTxx_Read_Byte(void);//����һ���ֽ�
uint8_t DHTxx_Read_Bit(void);//����һ��λ
uint8_t DHTxx_Check(void);//����Ƿ����DHTxx
void DHTxx_Rst(void);//��λDHTxx    
#endif