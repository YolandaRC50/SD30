#ifndef __GPIO_H
#define __GPIO_H  
#include "APP_Params.h"    
#include "includes.h"
#define GP_I_LEN 7
#define GP_O_LEN 9
#define DI_EXTI_LEN 3

#if(1) //DI

#if(1) //DI_PORT
#define DI_PORT_0 GPIOD //脚踏开关1
#define DI_PORT_1 GPIOE //PLC胶开关按钮
#define DI_PORT_2 GPIOE // PRESSURE_ENABLE按钮channel5

#define DI_PORT_3 GPIOE////	RUN/STOP //channel6       //plc输入信号
#define DI_PORT_4 GPIOE////plc purge
#define DI_PORT_5 GPIOD//	BACKUP1
#define DI_PORT_6 GPIOD //	BACKUP2

 
#endif

#if(1) //DI_PIN
#define DI_PIN_0 GPIO_Pin_5//脚踏开关1
#define DI_PIN_1 GPIO_Pin_9//PLC点胶按钮开关
#define DI_PIN_2 GPIO_Pin_13 // PRESSURE_ENABLE按钮channel5
#define DI_PIN_3 GPIO_Pin_14 //	RUN/STOP //channel6       //plc输入信号
#define DI_PIN_4 GPIO_Pin_11 //	plc purge
#define DI_PIN_5 GPIO_Pin_14 //BACKUP1
#define DI_PIN_6 GPIO_Pin_15 //	BACKUP2

 
#endif

#if(1)
//  GPIO_PuPd_NOPULL = 0x00,
//  GPIO_PuPd_UP     = 0x01,
//  GPIO_PuPd_DOWN   = 0x02
#define DI_PuPd_Type_0 GPIO_PuPd_UP//脚踏开关1
#define DI_PuPd_Type_1 GPIO_PuPd_UP//脚踏开关2

#define DI_PuPd_Type_2 GPIO_PuPd_UP//液位开关1
#define DI_PuPd_Type_3 GPIO_PuPd_UP//液位开关2
#define DI_PuPd_Type_4 GPIO_PuPd_UP//通道开关1
#define DI_PuPd_Type_5 GPIO_PuPd_UP//通道开关2
#define DI_PuPd_Type_6 GPIO_PuPd_UP//通道开关2
#endif

#if(1) //DI_CLK
#define DI_CLK_0 RCC_AHB1Periph_GPIOD//脚踏开关
#define DI_CLK_1 RCC_AHB1Periph_GPIOE//UI点胶开关

#define DI_CLK_2 RCC_AHB1Periph_GPIOE//PRESSURE_ENABLE按钮channel5
#define DI_CLK_3 RCC_AHB1Periph_GPIOE//	RUN/STOP //channel6       //plc输入信号
#define DI_CLK_4 RCC_AHB1Periph_GPIOE//	//	plc purge
#define DI_CLK_5 RCC_AHB1Periph_GPIOD//BACKUP1
#define DI_CLK_6 RCC_AHB1Periph_GPIOD//BACKUP1


//#define DI_CLK_8 RCC_AHB1Periph_GPIOE
//#define DI_CLK_9 RCC_AHB1Periph_GPIOE
//#define DI_CLK_10 RCC_AHB1Periph_GPIOF
//#define DI_CLK_11 RCC_AHB1Periph_GPIOC
//#define DI_CLK_12 RCC_AHB1Periph_GPIOC
//#define DI_CLK_13 RCC_AHB1Periph_GPIOC
//#define DI_CLK_14 RCC_AHB1Periph_GPIOC
//#define DI_CLK_15 RCC_AHB1Periph_GPIOA
#endif

#endif
#if(1) //DO

#if(1) //DO_PORT
#define DO_PORT_0 GPIOG // 控制电磁阀和真空出气
#define DO_PORT_1 GPIOG  //控制真空进气口负压

#define DO_PORT_2 GPIOD	// SYS_RUN 
#define DO_PORT_3 GPIOD	// STATUS: CHANNEL 1 DISP...
#define DO_PORT_4 GPIOD	// STATUS: PRESSURE ALARM
#define DO_PORT_5 GPIOD	// PRE_ENA

#define DO_PORT_6 GPIOG//备用电磁阀 
#define DO_PORT_7 GPIOG//正压比例阀供电
#define DO_PORT_8 GPIOG//负压比例阀供电
  
#endif

#if(1) //DO_PIN
#define DO_PIN_0 GPIO_Pin_3 //控制电磁阀和真空出气
#define DO_PIN_1 GPIO_Pin_2 //控制真空进气口负压
#define DO_PIN_2 GPIO_Pin_10  //SYS_RUN   TO PLC
#define DO_PIN_3 GPIO_Pin_13  //Status: Channel 1 Dispensing to PLC
#define DO_PIN_4 GPIO_Pin_14  //Status: Pressure Alarm  PLC
#define DO_PIN_5 GPIO_Pin_15  //PRE_ENA  TO PLC
#define DO_PIN_6 GPIO_Pin_5//  ///备用电磁阀 
#define DO_PIN_7 GPIO_Pin_4//         //正压比例阀供电
#define DO_PIN_8 GPIO_Pin_1//         //负压比例阀供电
 
#endif

#if(1)
//  GPIO_PuPd_NOPULL = 0x00,
//  GPIO_PuPd_UP     = 0x01,
//  GPIO_PuPd_DOWN   = 0x02
#define DO_PuPd_Type_0 GPIO_PuPd_UP
#define DO_PuPd_Type_1 GPIO_PuPd_UP
#define DO_PuPd_Type_2 GPIO_PuPd_UP
#define DO_PuPd_Type_3 GPIO_PuPd_UP
#define DO_PuPd_Type_4 GPIO_PuPd_UP
#define DO_PuPd_Type_5 GPIO_PuPd_UP
#define DO_PuPd_Type_6 GPIO_PuPd_UP
#define DO_PuPd_Type_7 GPIO_PuPd_NOPULL
#define DO_PuPd_Type_8 GPIO_PuPd_NOPULL
#endif

#if(1) //DO_CLK
#define DO_CLK_0 RCC_AHB1Periph_GPIOG
#define DO_CLK_1 RCC_AHB1Periph_GPIOG
#define DO_CLK_2 RCC_AHB1Periph_GPIOD
#define DO_CLK_3 RCC_AHB1Periph_GPIOD
#define DO_CLK_4 RCC_AHB1Periph_GPIOD
#define DO_CLK_5 RCC_AHB1Periph_GPIOD 
#define DO_CLK_6 RCC_AHB1Periph_GPIOG//备用电磁阀 
#define DO_CLK_7 RCC_AHB1Periph_GPIOG//正压比例阀供电
#define DO_CLK_8 RCC_AHB1Periph_GPIOG//负压比例阀供电 
#endif





#define DI_FOOT_SWITCH				0
#define DI_DISPENSE_SWITCH 		1             //PLC输入点胶
#define DI_PRESSURE_ENABLE			2          //plc触发
#define DI_RUN_STOP 						3             //plc触发
#define DI_PURGE    						4             //plc PURGE
#define DI_BACKUP_1 						5
#define DI_BACKUP_2 						6
 
// 

// 
#define DO_PRESSURE_VALVE 			0         // 控制电磁阀和真空出气
#define DO_VACUUM_VALVE 				1          //控制真空进气口负压
#define DO_SYS_RUN 							2
#define DO_DISPENSING 					3          //输出点胶情况
#define DO_PRESSURE_ALARM 			4          //
#define DO_PRE_ENA  						5           //输出压力开关
#define DO_BACK_VALVE 					6
#define DO_PRESSURE_POWER 			7           //1050供电
#define DO_VACUUM_POWER 			  8          //0090供电

 

#define DO_SYS_RUN_FORCE_INDEX 0 
#define	DO_LOW_LEVEL_LOWER_FORCE_INDEX	1
#define	DO_CHANNEL_1_DISPSING_FORCE_INDEX	2
#define	DO_CHANNEL_2_DISPSING_FORCE_INDEX	3
#define	DO_PRESSURE_ALARM_FORCE_INDEX	4
#define	DO_PRE_ENA_FORCE_INDEX	5
 
#define PURGE_LOW_STATUS 						GPIO_IN[DI_LOW_LEVEL_SENSOR_1].GPIO_Value 
#define DISPENSING_IO_STATUS() 	Check_Dispensing_IO_Status()
#define CHECK_PURGE_IO_STATUS							GPIO_IN[DI_PURGE].GPIO_Value
#define CHECK_PRESSURE_ENABLE 			GPIO_IN[DI_PRESSURE_ENABLE].GPIO_Value 
#define CHECK_RUN_STOP 							GPIO_IN[DI_RUN_STOP].GPIO_Value 
#define CHECK_BACKUP_1							GPIO_IN[BACKUP_1].GPIO_Value 
#define CHECK_BACKUP_2							GPIO_IN[BACKUP_1].GPIO_Value 

#define DISPENSING_RUN() 				Dispensing(POWER_ON)
#define DISPENSING_STOP() 				Dispensing(POWER_OFF)
 
#define PRESSURE_ALARM(Power) 			GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=Power
	
#define CARTRIDGE_LEVEL(Status) 		Cartridge_Level_Status(Status)
#define DO_PRESSURE_SENSOR_POWER_ON GPIO_OUT[DO_PRESSURE_POWER].GPIO_Value=POWER_ON    //yolanda
#define DO_VACUUM_SENSOR_POWER_ON   GPIO_OUT[DO_VACUUM_POWER].GPIO_Value=POWER_ON     //yolanda
#define DO_BACK_VALVE_STATUS(Power) 				GPIO_OUT[DO_BACK_VALVE].GPIO_Value=Power
#define SET_RUN_STOP_STATUS(Power) 				GPIO_OUT[DO_SYS_RUN].GPIO_Value=Power
#define SET_PRE_ENA_STATUS(Power) 				GPIO_OUT[DO_PRE_ENA].GPIO_Value=Power
#define SET_DIPENSE_STATUS(Power) 				GPIO_OUT[DO_DISPENSING].GPIO_Value=Power






#endif
 
 
 #if(1==1)
#define DI_FOOT_SWITCH_1_DI_Index 			0
#define DI_CHANNEL_SWITCH_1_DI_Index		1     //PLC输入点胶信号
#define DI_PURGE_SWITCH_1_DI_Index			2     //plc输入purge信号


 
#define DI_FOOT_SWITCH_1_EXTI_PortSourceGPIOx 			EXTI_PortSourceGPIOD
#define DI_CHANNEL_SWITCH_1_EXTI_PortSourceGPIOx 		EXTI_PortSourceGPIOE
#define DI_PURGE_SWITCH_1_EXTI_PortSourceGPIOx 			EXTI_PortSourceGPIOE

 
 
#define DI_FOOT_SWITCH_1_EXTI_PinSourcex						EXTI_PinSource5
#define DI_CHANNEL_SWITCH_1_EXTI_PinSourcex 				EXTI_PinSource9
#define DI_PURGE_SWITCH_1_EXTI_PinSourcex			  		EXTI_PinSource11

 
#define DI_FOOT_SWITCH_1_EXTIx_IRQn									EXTI9_5_IRQn
#define DI_CHANNEL_SWITCH_1_EXTIx_IRQn 							EXTI9_5_IRQn
#define DI_PURGE_SWITCH_1_EXTIx_IRQn								EXTI15_10_IRQn



#define DI_FOOT_SWITCH_1_EXTI_LineX									EXTI_Line5
#define DI_CHANNEL_SWITCH_1_EXTI_LineX 							EXTI_Line9
#define DI_PURGE_SWITCH_1_EXTI_LineX								EXTI_Line11


//#define EXTI0_IRQHandler
//#define	EXTI1_IRQHandler
//#define	EXTI2_IRQHandler
//#define EXTI3_IRQHandler
//#define	EXTI4_IRQHandler
//#define	EXTI9_5_IRQHandler
//#define	EXTI15_10_IRQHandler
#endif
typedef struct _DI_EXTI_
{
	uint8_t DI_List_Index;
	uint8_t EXTI_PortSourceGPIOx;
	uint8_t EXTI_PinSourcex;
	uint8_t EXTIx_IRQn;
	uint32_t EXTI_LineX;
}DI_Exti;

typedef struct _GPIO_
{
	uint32_t DO_CLKx;
	GPIO_TypeDef* GPIOx; 
	uint16_t GPIO_Pinx;
	GPIOPuPd_TypeDef GPIOPuPd_Type;
	uint8_t GPIO_Value;
}GPIO;
typedef struct _FORCE_
{
	uint16_t Index;
	POWER_STATUS Forced_Status;
}FORCE;
extern GPIO GPIO_IN[GP_I_LEN];
extern GPIO GPIO_OUT[GP_O_LEN];
extern DI_Exti DI_Exti_List[DI_EXTI_LEN];

extern FORCE Force_List[6];

void GOIO_Init(void);
void Refresh_GPIO_Value(void); 
void Cartridge_Level_Status(Cartridge_Status viStatus);
void Dispensing(POWER_STATUS Power);
POWER_STATUS Check_Dispensing_IO_Status();
extern POWER_STATUS  FORCE_ENABLE;
#endif