#ifndef HMI_APP_H
#define HMI_APP_H


#include "stm32f4xx.h"                  // Device header
#include "includes.h" 
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>

void UI_Init(void);
void UpdateUI(void);

//创建画面Load_0的命令索引ID
#define  _SCREEN_LOAD_0                                                        0

//创建画面SoftUpdate_1的命令索引ID
#define  _SCREEN_SOFTUPDATE_1                                                  1

//创建画面Main_2的命令索引ID
#define  _SCREEN_MAIN_2                                                        2

//创建画面Setting_3的命令索引ID
#define  _SCREEN_SETTING_3                                                     3


#define  _SCREEN_UNIT_4																			4

#define  _SCREEN_NETWORK_5																			5



//创建画面Setting_More_4的命令索引ID

#define  _SCREEN_CALIBRATIONP_6                                                6
//创建画面Setting_More_5的命令索引ID

#define  _SCREEN_CALIBRATIONV_7                                               7

#define  _SCREEN_CHANGE_PWD_8																				8
#define  _SCREEN_RESET_SYCOUNT_9                                                   9
//创建画面Systemini_5的命令索引ID
#define  _SCREEN_SYSTEMINI_10                                                   10

#define  _SCREEN_KEYBOARD_11                         11

#define  _SCREEN_INPUT_FIRPWD_12																				12

#define  _SCREEN_MAIN_UNLOCK_13																				13


//创建画面Setting_Save_Dlg_6的命令索引ID
#define  _SCREEN_SETTING_SAVE_DLG_14                                            14


#define  _SCREEN_RESET_SAVE_DLG_15                         15

#define  _SCREEN_PRESSURE_ALM_DLG_16                                       		16
#define  _SCREEN_INPUT_SYSCOUNT_17                                       		17
#define  _SCREEN_INPUT_SECPWD_18                                       		18



extern uint8_t cmd_buffer[CMD_MAX_SIZE];//指令缓存

extern uint16_t current_screen_id;//当前画面ID

extern POWER_STATUS Pressure_Display_Enable;
#endif
