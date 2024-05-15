#ifndef __CARTRIDGE_DETECT_H
#define __CARTRIDGE_DETECT_H
#include "includes.h"
#define LEVEL_DETECT_ONLINE_VALUE_BUFFER_LEN (uint16_t)30
#define GAS_SOURCE_ONLINE_VALUE_BUFFER_LEN (uint16_t)30 

#define DELAY_AT_STEP Cartridge_Detect_Data.Cartridge_Specification_And_Step_Params_Data.Delay_AT_Step
#define AN_ACCURACY   Cartridge_Detect_Data.Cartridge_Specification_And_Step_Params_Data.Step_Jump_AN_Accuracy/*��ת��������*/
#define STEP_JUMP_CONDITION_NUM  (uint16_t)Cartridge_Detect_Data.Cartridge_Specification_And_Step_Params_Data.Step_Jump_Condition_Num/*�ﵽ��ת��������е���ѭ���жϴ���*/
extern double ADV_Online_Value_Buffer[128];

void Cartridge_DetectSystem_Working(void);
void Cartridge_Debugging(void);
void DetectSystem_Productting(void);
void DetectSystem_Detectting(void);

/*����ֶ���������*/
void Manual_Exhaust_Switch(void); 
 
void Detect_Air_Guage_Data(void);
	/*�򿪵�ŷ�3���رյ�ŷ�2�͵�ŷ�4��ʹ������·�ڲ�����ж��*/
void DetectSystem_AIR_DIS(void);
	/*��·����:  �򿪷�A���رշ�B���ر�й������ֱ��ѹ����ѹ����������Ϊ�ﵽ��ת����*/
void DetectSystem_AIR_INFLATION(void);
	/*��·����ƽ��: �رշ�A,�رշ�B���ر�й�������ȴ�2~3�룬�ﵽ��ת����*/
void DetectSystem_AIR_INFLATION_BALANCE(void);	
	/*��Ͱ���������ֹرշ�A���򿪷�B���ر�й������ֱ��ѹ������������½�,��ʱ1�룬��ȡʮ�����ݣ���ƽ��ֵ*/
void DetectSystem_AIR_TO_CARTRIDGE(void);	
	/*������ɣ��򿪷�A,��B���ر�й������ֱ��ѹ������������������ɶ���*/
void DetectSystem_AIR_DETECT_COMPLETE(void);

uint8_t CheckOnlineData(void);
#endif
