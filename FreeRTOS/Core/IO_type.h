/*******************************************************************************
** �ļ���: 		IO_type.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM
** ����: 		Adam
** ����:		Ӳ���ӿڶ���
** ����ļ�:		��
** �޸���־��	2018-03-12   �����ĵ�
*******************************************************************************/
#ifndef __IO_TYPE_H
#define __IO_TYPE_H

/* ���Ͷ��� ------------------------------------------------------------------*/
#include "stm32f4xx.h"

typedef struct 
{
	GPIO_TypeDef* port;
	uint16_t pin;
}IOPort_TypeDef;//�ӿڲ���

#define PORT_OUT_SET(xport)      GPIO_SetBits(xport.port, xport.pin)
#define PORT_OUT_RESET(xport)    GPIO_ResetBits(xport.port, xport.pin)
#define PORT_IN_READ(xport)     GPIO_ReadInputDataBit(xport.port, xport.pin)

#endif


