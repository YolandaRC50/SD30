#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "stm32f4xx.h"                  // Device header

//�������빦��	ʹ�ܣ�˳ʱ��ʹ�ܣ���ʱ��ʹ�ܣ�˳ʱ��+��ʱ��ʹ�ܣ�ʹ��+����ֹͣ��PWM�趨ֵ��RC Servo�趨ֵ���̶��趨ֵ
//ģ�����빦��	�趨ֵ���������ޣ�ƫ�ƣ�����
//�����������	������ת�ټ춨�ǣ������춨�ǣ�����Ƶ��
//ģ���������	�����������ת�ټ�������¶ȣ��̶�ֵ

//ģ�����
#define AOUT_A1_S		GPIO_Pin_6
#define AOUT_A2_S		GPIO_Pin_7
#define AOUT_B1_S		GPIO_Pin_8
#define AOUT_B2_S		GPIO_Pin_9

#define AOUT_PORT		GPIOF

//����������� ����1 PWM
#define MD_DOA04		GPIO_Pin_6
#define MD_DIA04		GPIO_Pin_7
#define	MD_DOA03		GPIO_Pin_8
#define MD_DIA03		GPIO_Pin_9
#define MD_DIA02		GPIO_Pin_0
#define MD_DIA01		GPIO_Pin_1

#define MD_APORT_3_4		 GPIOB
#define MD_APORT_1_2		 GPIOE

#define MD_DOB04		GPIO_Pin_10
#define MD_DIB04		GPIO_Pin_0
#define	MD_DOB03		GPIO_Pin_1
#define MD_DIB03		GPIO_Pin_2
#define MD_DIB02		GPIO_Pin_3
#define MD_DIB01		GPIO_Pin_2

#define MD_BPORT_O4				GPIOF
#define MD_BPORT_2_3_4		GPIOG
#define MD_BPORT_1				GPIOA
//������ ��������� ����Ϊ��
#define MOTORA_ID_S	GPIO_Pin_9
#define MOTORA_ID_PORT	GPIOE

#define MOTORB_ID_S GPIO_Pin_8
#define MOTORB_ID_PORT	GPIOE

#define acc_r 0x01
#define	acc_l 0x00
#define ch_a	0x00
#define ch_b	0x01

#define PU15_CALIBRATION_RATE									120.0f
#define PU16_CALIBRATION_RATE									240.0f
#define PU15_VOLUME_PER_REVOLUTION						12.0f
#define PU16_VOLUME_PER_REVOLUTION						50.0f
#define PUMP_CALIRATION_TIME_S								120.0f
 
#define MIN_ACC_DEC														0.0f
#define MAX_ACC_DEC														300000.0f

 
extern uint8_t SPEED_i;
extern float SPEED_AVR[4];
void MOTOR_Init(void);
double VOL_TO_SPPED(uint16_t vol, uint8_t cmd);
void SPEED_Control(void);
float SPEED_TO_ANALOG(float speed, uint8_t cmd);
float ANALOG_TO_SPEED(float Analog, uint8_t cmd);
uint8_t SPEED_Check(void);
uint8_t Motor_ConnectCheck(void);
uint8_t Motor_ControllerStatusCheck(void);
float getPumpCalibrationVol(uint8_t PU15_16);
void Acc_Dec_To_Anlog(void);
int16_t CheckPumpSpeed(void);

uint8_t GetMotorRunStatus(void);
void MotorControllerReset(void);
void MotorControllerReset(void);
#endif
