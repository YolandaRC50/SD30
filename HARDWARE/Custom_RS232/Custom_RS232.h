#ifndef _Custom_RS232_H_
#define _Custom_RS232_H_

#include "stm32f4xx_it.h"     //�����û�MCU�����޸�
#include "stm32f4xx.h"
#include "string.h"
#include "sys_config.h"

#if (1==1)  /* Custom_RS232*/
#define CUSTOM_RS232_TX_Pin GPIO_Pin_9 
#define CUSTOM_RS232_RX_Pin GPIO_Pin_10 

#define CUSTOM_RS232 USART6
#define RCC_APBxPeriph_USARTx RCC_APB2Periph_USART6
#define CUSTOM_RS232_PORT GPIOG
#define CUSTOM_RS232_BAUDRATE 19200
#define CUSTOM_RS232_IRQn USART6_IRQn
#define  CUSTOM_RS232_PORT_AHBxPeriph RCC_AHB1Periph_GPIOG

#endif




/******************************************************************************/

#if (1==1) /*����*/



/****************************************************************************
* ��    �ƣ� CUSTOM_RS232_Init()
* ��    �ܣ� ���ڳ�ʼ�� 
* ���ڲ����� ��
****************************************************************************/
void Custom_RS232_Init(void);

/*****************************************************************
* ��    �ƣ� CUSTOM_RS232_SendChar()
* ��    �ܣ� ����1���ֽ�
* ��ڲ����� t  ���͵��ֽ�
* ���ڲ����� ��
*****************************************************************/
void  Custom_RS232_SendChar(unsigned char t);

/*****************************************************************
* ��    �ƣ� USART1_IRQHandler()
* ��    �ܣ� USART1_IRQHandler�����ж�
* ��ڲ����� ��
* ���ڲ����� ��
*****************************************************************/
void USART1_IRQHandler(void);

#endif


#endif

