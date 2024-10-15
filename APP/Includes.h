#ifndef _INCLUDES_H_
#define _INCLUDES_H_
#define VERSION "1.1.7"
#define IDH "2974793"
#define PD "Pulse EQ SD30 Precision Syringe Dispenser"
#define BAR_PSI_TR 14.5102f
#define mmHg_PSI_TR 51.7149f
//#define  pressurerange 1.05f
 

//±ê×¼¿â
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  <string.h>
#include  "stm32f4xx.h"                  // Device header 
#include  "stm32f4xx_exti.h"
#include  "stm32f4xx_spi.h"
#include  "system_stm32f4xx.h"

//OS
#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include 	"croutine.h"
#include 	"semphr.h"
#include 	"event_groups.h"
#include	"sys_core.h"
#include  "timer.h"

//APP
#include 	"APP.h"
#include	"BSP.h"

//HARDWARE/USART
#include	"usart.h"

//Device/OTHER
#include "Bell.h"
#include "Led.h"

//HARDWARE/FLASH_EEPROM
#include "EEP_I2C.h"	 
#include "Flash_SPI.h"	 
#include "24Cxx.h"
#include "w25qxx.h"  

//HARDWARE/SENSOR
#include "DHTxx.h"  
#include "Pressure.h"

//HARDWARE/GPIO
#include "GPIO.h"

//Device/HMI
#include "hmi_usart.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "HMI_App.h"



//Params 
#include "APP_Params.h" 

//Comm
#include "bsp_comm.h" 
#include "bsp_ethernet.h"
#include "bsp_esp8266.h"
#include "bsp_hc05.h"
#include "App_Comm.h" 
//Files
#include "sdio_sdcard.h"
#include "ff.h"  
#include "sys.h"
  
	
//File
#if FILES_ENABLE == 1
#include "file_operate.h"
#endif
void Dispensing_Logic(void);
//extern SYSTEM_LANGUAGE System_Language;

 
#endif
