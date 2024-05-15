#ifndef _TEMPERATURESTATUS_H_
#define _TEMPERATURESTATUS_H_


#include "stm32f4xx.h"                  // Device header

//�������빦��	ʹ�ܣ�˳ʱ��ʹ�ܣ���ʱ��ʹ�ܣ�˳ʱ��+��ʱ��ʹ�ܣ�ʹ��+����ֹͣ��PWM�趨ֵ��RC Servo�趨ֵ���̶��趨ֵ
//ģ�����빦��	�趨ֵ���������ޣ�ƫ�ƣ�����
//�����������	������ת�ټ춨�ǣ������춨�ǣ�����Ƶ��
//ģ���������	�����������ת�ټ�������¶ȣ��̶�ֵ



//��������
#define TEMPERATURE_ALARM_DI_PORT		GPIOA
#define TEMPERATURE_ALARM_STATUS_01		GPIO_Pin_4
#define TEMPERATURE_ALARM_STATUS_02		GPIO_Pin_5


void Temperature_Status_Init(void);
uint8_t Temperature_Pump_Alarm_Check(void);
uint8_t Temperature_Cartridge_Alarm_Check(void);


#endif
