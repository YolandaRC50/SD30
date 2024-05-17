#include "HMI_App.h" 
#include "includes.h" 

#define UI_STYLE_BW			0//黑色
uint16_t current_screen_id=0;
uint8_t Pump_AccButtonView=0;
	
uint16_t BackICONIndex=40;
uint16_t StatusICONIndex=50;
void SetRTCTime(uint8_t second,uint8_t minute,uint8_t hour,uint8_t day,uint8_t week,uint8_t month,uint8_t year);
uint8_t cmd_buffer[CMD_MAX_SIZE];//指令缓存
static int32_t test_value = 0;//测试值
  
static uint8_t Level_Vol_Enable_Counter=0;	
extern POWER_STATUS m_Last_DI_Dispensing_Status;
uint8_t Auto_AT=0;
uint8_t Temp_Alm_Counter_Trigger=0;
uint8_t settime_flag=0;
uint8_t cont_flag=0;

POWER_STATUS Pressure_Display_Enable=POWER_OFF;     //压力不正常

uint16_t _Debug_UI_Open_Count =0;
 
IOT_COMMICATION_MODE_ENUM Comm_Mode=ETHERNET_MODE;
uint8_t pron;
//extern float save_pro_buffer[PRONUM][2];

extern int Float_to_Byte();
extern uint8_t mcuidbuffer[25];
float attmp;
	//SetScreen()
uint8_t Level_To_Screen_ID=2;
uint8_t Prev_Screen=2;
char New_Level_1_pwd[PASSWORD_LEN];
char New_Level_2_pwd[PASSWORD_LEN];
char Input_FPwd[PASSWORD_LEN];
char Input_SPwd[PASSWORD_LEN];
uint8_t New_Level_1_pwd_Update=0;
uint8_t New_Level_2_pwd_Update=0;
//uint8_t Sys_In=0;
char Super_Level_1_pwd[PASSWORD_LEN]="888888";
char Super_Level_2_pwd[PASSWORD_LEN]="654321";
void SetTextValueint32_t(uint16_t screen_id, uint16_t control_id,int32_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%d",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValueint8_t(uint16_t screen_id, uint16_t control_id,int8_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%d",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValue0Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%0.0lf",value);//把浮点数转换为字符串(保留2位小数)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.1lf",value);//把浮点数转换为字符串(保留2位小数)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValue2Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.2lf",value);//把浮点数转换为字符串(保留2位小数)
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValue3Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.3lf",value);//把浮点数转换为字符串(保留2位小数)
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValue4Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.4lf",value);//把浮点数转换为字符串(保留2位小数)
	SetTextValue(screen_id,control_id,buffer);
} 
 
/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16_t screen_id)
{
	
	//TODO: 添加用户代码
	current_screen_id = screen_id;//在工程配置中开启画面切换通知，记录当前画面ID
	Clearscreen(0);
	Clearscreen(1);
	if(screen_id!=_SCREEN_CALIBRATIONP_6)
	{
		Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable=POWER_OFF;
		Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable=POWER_OFF;
	}
	if(screen_id!=_SCREEN_CALIBRATIONV_7)
	{
		Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable=POWER_OFF;
		Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable=POWER_OFF;
	}

	switch(screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{ 
			  //current_screen_id = _SCREEN_MAIN_2;
				if(Sys_RT_Status.pressure_disable_Mode == POWER_OFF )
				{
					Sys_RT_Status.System_Option_Mode=STOP_MODE; 
				} 

				SetButtonValue(current_screen_id,10,Sys_RT_Status.pressure_disable_Mode);
				SetButtonValue(current_screen_id,11,Sys_RT_Status.System_Option_Mode);
				
				if(Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)
				{				
					SetControlEnable(current_screen_id,12,POWER_ON);
					if(Sys_Params.Dispense_Param.Run_Mode_Params != TIMER)	
					{
						SetControlEnable(current_screen_id,8,POWER_OFF);
					}	
					else
					{
						SetControlEnable(current_screen_id,8,POWER_ON);   
					}
					SetControlEnable(current_screen_id,13,POWER_OFF);
				}
				else
				{			
					SetControlEnable(current_screen_id,13,POWER_ON);
					SetControlEnable(current_screen_id,12,POWER_OFF);
					SetControlEnable(current_screen_id,8,POWER_OFF);
				}
				SetButtonValue(current_screen_id,8,Sys_RT_Status.TEACH);
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态
				AnimationPlayFrame(current_screen_id,14, Sys_Params.Pressure_Param.Tem_Unit);    //显示温度单位
				AnimationPlayFrame(current_screen_id,9, Sys_Params.Dispense_Param.Run_Mode_Params);    //显示点胶模式

				AnimationPlayFrame(current_screen_id,2,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
				
				if(Sys_Params.Pressure_Param.Tem_Unit == centi)
				{
				
					SetTextValueFloat(current_screen_id,15, Sys_RT_Status.Temperature_RT);   //温度摄氏
				}
				else
				{
					SetTextValueFloat(current_screen_id,15, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //温度华氏
				}					
				SetTextValueFloat(current_screen_id,16, Sys_RT_Status.Humidity_RT);     //湿度
				SetTextValueint32_t(current_screen_id,17, Sys_Params.Dispense_Param.shot_num);   //当天点胶次数			

				
				SetButtonValue(current_screen_id,12,Sys_RT_Status.SHOT);
				SetButtonValue(current_screen_id,13,Sys_RT_Status.PURGE);

			
				if(Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE)
				{
					
					uint8_t nopro[]="--";	
					SetTextValue2Float(current_screen_id,5, Sys_Params.Dispense_Param.Dis_Timer);
					SetTextValue(current_screen_id,6, nopro);
				}
				if(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER)
				{
					uint8_t nopro[]="--";	
					SetTextValue(current_screen_id,6, nopro);
					SetTextValue2Float(current_screen_id,5, Sys_Params.Dispense_Param.Dis_Timer);
				}
				if(Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE)
				{
					SetTextValue2Float(current_screen_id,5, Sys_Params.Dispense_Param.Dis_Timer);
					SetTextValue2Float(current_screen_id,6, Sys_Params.Dispense_Param.Inter_timer);
				}

			break;
		} 
		case _SCREEN_SETTING_3:
		{ 	
			AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			break;
		} 
		case _SCREEN_UNIT_4:
		{ 	
			AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			SetButtonValue(_SCREEN_UNIT_4,1,Sys_Params.Pressure_Param.Pressure_Unit== BAR);
			SetButtonValue(_SCREEN_UNIT_4,2,Sys_Params.Pressure_Param.Pressure_Unit== PSI);
			SetButtonValue(_SCREEN_UNIT_4,3,Sys_Params.Pressure_Param.Vacuum_Unit== mmHg);
			SetButtonValue(_SCREEN_UNIT_4,6,Sys_Params.Pressure_Param.Vacuum_Unit== VPSI);
			SetButtonValue(_SCREEN_UNIT_4,7,Sys_Params.Pressure_Param.Tem_Unit== fahr);
			SetButtonValue(_SCREEN_UNIT_4,8,Sys_Params.Pressure_Param.Tem_Unit== centi);
			break;
		} 
		case _SCREEN_CALIBRATIONP_6:
		{			 
			SetTextValue0Float(current_screen_id,13,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value);  
			SetTextValue0Float(current_screen_id,14,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value);  
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			
			break;
		} 
		case _SCREEN_CALIBRATIONV_7:
		{	
			SetTextValue0Float(current_screen_id,7,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value);
			SetTextValue0Float(current_screen_id,12,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value);
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_14:
		{
			break;
		}
		case _SCREEN_PRESSURE_ALM_DLG_16:
		{
			break;
		}
		case _SCREEN_RESET_SYCOUNT_9:
		{
			//SetTextValueint32_t(current_screen_id,1,Sys_Params.Dispense_Param.sys_count);
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			SetTextValueint32_t(current_screen_id,5,Sys_Params.Dispense_Param.sys_count);
			break;
		} 
		case _SCREEN_SYSTEMINI_10:
		{
			AnimationPlayFrame(_SCREEN_SYSTEMINI_10,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			break;
		} 


		case _SCREEN_NETWORK_5:
		{ 
			AnimationPlayFrame(_SCREEN_NETWORK_5,1,Sys_Params.IOT_Params.Commication_Mode);
			AnimationPlayFrame(_SCREEN_NETWORK_5,2,Sys_Params.IOT_Params.Commication_Mode);
			SetButtonValue(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
			SetButtonValue(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
			
			Comm_Mode=Sys_Params.IOT_Params.Commication_Mode;
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);				
			SetTextValue(current_screen_id,3,(uint8_t *)Sys_Params.IOT_Params.Ethernet_Params.IP_Address); 
			 
			//sprintf((char *)buffer,"%s","255.255.255.0");				
			SetTextValue(current_screen_id,4,(uint8_t *)Sys_Params.IOT_Params.Ethernet_Params.Net_Mask); 
			
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Ethernet_Params.Gate_way);			
			SetTextValue(current_screen_id,5,(uint8_t *)Sys_Params.IOT_Params.Ethernet_Params.Gate_way); 
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Ethernet_Params.IP_Address);					
			SetTextValue(current_screen_id,6,(uint8_t *)Sys_Params.IOT_Params.Server_Params.Server_IPAddress); 
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Server_Params.Server_IPAddress);				
			SetTextValue(current_screen_id,8,(uint8_t *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.WIFI_Params.Wlan_Password);				
			SetTextValue(current_screen_id,9,(uint8_t *)Sys_Params.IOT_Params.WIFI_Params.Wlan_Password);
					 			 
			SetTextValueint32_t(current_screen_id,7,Sys_Params.IOT_Params.Server_Params.Sever_Port); 
			
			break; 
		}

		case _SCREEN_CHANGE_PWD_8:
		{
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			break;
		}
		case _SCREEN_KEYBOARD_11:
		{
			
			break;
		}
		case _SCREEN_INPUT_FIRPWD_12:
		{
			break;
		}
		case _SCREEN_MAIN_UNLOCK_13:
		{
			if(Sys_RT_Status.pressure_disable_Mode == POWER_OFF )
			{
				Sys_RT_Status.System_Option_Mode=STOP_MODE; 
			} 
			SetButtonValue(current_screen_id,9,Sys_RT_Status.pressure_disable_Mode);
			SetButtonValue(current_screen_id,10,Sys_RT_Status.System_Option_Mode);
			SetButtonValue(current_screen_id,11,Sys_RT_Status.SHOT);
			SetButtonValue(current_screen_id,12,Sys_RT_Status.PURGE);
			
			if(Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)
			{				
				SetControlEnable(current_screen_id,11,POWER_ON);    //shot可按
				if(Sys_Params.Dispense_Param.Run_Mode_Params != TIMER)	
				{
					SetControlEnable(current_screen_id,19,POWER_OFF);
				}	
				else
				{
					SetControlEnable(current_screen_id,19,POWER_ON);
				}
				SetControlEnable(current_screen_id,12,POWER_OFF);
			}
			else
			{			
				SetControlEnable(current_screen_id,12,POWER_ON);
				SetControlEnable(current_screen_id,11,POWER_OFF);
				SetControlEnable(current_screen_id,19,POWER_OFF);
			}
			AnimationPlayFrame(current_screen_id,25, Sys_Params.IOT_Params.Commication_Mode);    //显示有限网络
			AnimationPlayFrame(current_screen_id,24, Sys_RT_Status.System_Status);    //显示点胶状态
			AnimationPlayFrame(current_screen_id,20, Sys_Params.Pressure_Param.Tem_Unit);    //显示温度单位
			//AnimationPlayFrame(current_screen_id,27, Sys_Params.Pressure_Param.Pressure_Unit);    //显示正压单位
			AnimationPlayFrame(current_screen_id,28, Sys_Params.Pressure_Param.Vacuum_Unit);    //显示负压单位

			
			if(Sys_Params.Pressure_Param.Tem_Unit == centi)
			{
			
				SetTextValueFloat(current_screen_id,21, Sys_RT_Status.Temperature_RT);   //温度摄氏
			}
			else 
			{
				SetTextValueFloat(current_screen_id,21, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //温度华氏
			}					
			SetTextValueFloat(current_screen_id,22, Sys_RT_Status.Humidity_RT);     //湿度
			SetTextValueint32_t(current_screen_id,23, Sys_Params.Dispense_Param.shot_num);   //当天点胶次数		
			if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
			{
				SetTextValue0Float(current_screen_id,5, Sys_Params.Dispense_Param.program_number);
				SetTextValue2Float(current_screen_id,4, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
				SetTextValue2Float(current_screen_id,8, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
			}				
			else
			{
				uint8_t nopro[]="--";	
				SetTextValue(current_screen_id,5, nopro);
			}
			
			
			if(Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE)
			{
				
				uint8_t nopro[]="--";	
				SetTextValue2Float(current_screen_id,4, Sys_Params.Dispense_Param.Dis_Timer);
				SetTextValue(current_screen_id,8, nopro);
			}
			if(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER)
			{
				uint8_t nopro[]="--";	
				SetTextValue(current_screen_id,8, nopro);
				SetTextValue2Float(current_screen_id,4, Sys_Params.Dispense_Param.Dis_Timer);
			}
			if(Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE)
			{
				SetTextValue2Float(current_screen_id,4, Sys_Params.Dispense_Param.Dis_Timer);
				SetTextValue2Float(current_screen_id,8, Sys_Params.Dispense_Param.Inter_timer);
			}
			break;
		}
		case _SCREEN_RESET_SAVE_DLG_15:
		{
			break;
		}


	}
	
	UpdateUI();
}

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: 添加用户代码
}

void UI_Init(void)
{ 
//	SetTextSpeed(0,2,10);
//	UpdateUI();
	
//	if(System_Language==LANG_ENGLISH)
//	{
//		//设置语言为English
//		Setlanguage_EN();
//	}
//	else
//	{
//		//设置语言为中文
//		Setlanguage_CH();
//	} 
	SetScreen(_SCREEN_LOAD_0);
	
} 
 
void UpdateUI(void)
{  
	
	 if(m_Get_HMI_RTC_MARK==1)
	 {
		 m_Get_HMI_RTC_MARK=POWER_OFF;
		 GetRTCTime();
		 //SetRTCTime(0x11,0x11,0x11,0x11,0x03,0x11,0x11);
		 //SetRTCTime(0x00,Sys_RT_Status.System_RTC.minute,Sys_RT_Status.System_RTC.hour,Sys_RT_Status.System_RTC.day,0x00,Sys_RT_Status.System_RTC.month,Sys_RT_Status.System_RTC.year);
	 }
	 if(settime_flag==1)
	 {
			SetRTCTime(0x00,Sys_RT_Status.System_RTC.minute_BCD,Sys_RT_Status.System_RTC.hour_BCD,Sys_RT_Status.System_RTC.day_BCD,0x00,Sys_RT_Status.System_RTC.month_BCD,Sys_RT_Status.System_RTC.year_BCD);
			settime_flag = 0;
	 }
	 //SetRTCTime(0x00,Sys_RT_Status.System_RTC.minute,Sys_RT_Status.System_RTC.hour,Sys_RT_Status.System_RTC.day,0x00,Sys_RT_Status.System_RTC.month,Sys_RT_Status.System_RTC.year);

	if(Pressure_Display_Enable==POWER_ON)
	{
		SetScreen(_SCREEN_PRESSURE_ALM_DLG_16);
		current_screen_id=_SCREEN_PRESSURE_ALM_DLG_16;
		GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=POWER_ON; //20240408
	}
	/*if((Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)&&(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]==0))
	{
		Sys_Params.Dispense_Param.Pro_mode = POWER_OFF;
	}
	if((Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)&&(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]>0))
	{
		//Sys_RT_Status.System_Status
		Sys_Params.Dispense_Param.Pro_mode = POWER_ON;
	}*/
	Comm_Mode=Sys_Params.IOT_Params.Commication_Mode;
			
	switch(current_screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			uint8_t buffer[15] = {0};
			sprintf((char *)buffer,"%s",VERSION);				
			SetTextValue(_SCREEN_LOAD_0,2,buffer);
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{
				if(Sys_RT_Status.pressure_disable_Mode == POWER_OFF )
				{
					Sys_RT_Status.System_Option_Mode=STOP_MODE;	
					SetControlEnable(current_screen_id,11,POWER_OFF);
				
				}
				else
				{
					SetControlEnable(current_screen_id,11,POWER_ON);
				}

									
				SetButtonValue(current_screen_id,10,Sys_RT_Status.pressure_disable_Mode);
				SetButtonValue(current_screen_id,11,Sys_RT_Status.System_Option_Mode);

				if(Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)
				{				
					SetControlEnable(current_screen_id,12,POWER_ON);
				}
				else
				{			
					SetControlEnable(current_screen_id,12,POWER_OFF);
				}
				if((Sys_RT_Status.System_Option_Mode==STOP_MODE)&&(Sys_RT_Status.pressure_disable_Mode == POWER_ON))
				{
					SetControlEnable(current_screen_id,13,POWER_ON);
				}
				else
				{
					SetControlEnable(current_screen_id,13,POWER_OFF);
				}
				SetControlEnable(current_screen_id,8,POWER_OFF);
				if(Sys_RT_Status.TEACH == POWER_ON )
				{
					SetControlEnable(current_screen_id,8,POWER_ON);
					SetButtonValue(current_screen_id,8,POWER_ON);					
				}
				else
				{
					SetButtonValue(current_screen_id,8,POWER_OFF);
					SetControlEnable(current_screen_id,8,POWER_OFF);					
				}
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态
				AnimationPlayFrame(current_screen_id,14, Sys_Params.Pressure_Param.Tem_Unit);    //显示温度单位
				AnimationPlayFrame(current_screen_id,9, Sys_Params.Dispense_Param.Run_Mode_Params);    //显示点胶模式
				//AnimationPlayFrame(current_screen_id,21, Sys_Params.Pressure_Param.Pressure_Unit);    //显示正压单位
				//AnimationPlayFrame(current_screen_id,22, Sys_Params.Pressure_Param.Vacuum_Unit);    //显示负压单位
				AnimationPlayFrame(current_screen_id,2,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
				
				if(Sys_Params.Pressure_Param.Tem_Unit == centi)
				{
				
					SetTextValueFloat(current_screen_id,15, Sys_RT_Status.Temperature_RT);   //温度摄氏
				}
				else
				{
					SetTextValueFloat(current_screen_id,15, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //温度华氏
				}					
				SetTextValueFloat(current_screen_id,16, Sys_RT_Status.Humidity_RT);     //湿度
				SetTextValueint32_t(current_screen_id,17, Sys_Params.Dispense_Param.shot_num);   //当天点胶次数	
				
			 
				//SetButtonValue(current_screen_id,4,Sys_RT_Status.reset);
				//SetButtonValue(current_screen_id,5,Sys_RT_Status.admin);
				
				SetButtonValue(current_screen_id,12,Sys_RT_Status.SHOT);
				SetButtonValue(current_screen_id,13,Sys_RT_Status.PURGE);
				//Sys_RT_Status.pressure_disable_Mode=POWER_ON;
							
				if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
				{
					SetTextValue0Float(current_screen_id,7, Sys_Params.Dispense_Param.program_number);
					SetTextValue2Float(current_screen_id,5, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
					SetTextValue2Float(current_screen_id,6, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
					//SetTextValue2Float(current_screen_id,2, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]);
					//SetTextValue2Float(current_screen_id,3, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]);
					AnimationPlayFrame(current_screen_id,21, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]);    //显示正压单位
					AnimationPlayFrame(current_screen_id,22, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]);    //显示负压单位
					if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]==BAR)    
					{
						
						SetTextValue2Float(current_screen_id,1, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]);
						//Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
					}
					else
					{
						SetTextValue2Float(current_screen_id,1, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]*BAR_PSI_TR);
						//Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]*BAR_PSI_TR;
					}				
					if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]==VPSI)    
					{
						
						SetTextValue2Float(current_screen_id,4, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]);

					}
					else
					{
						SetTextValue2Float(current_screen_id,4, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]*mmHg_PSI_TR);
						
					}	
				}				
				else
				{
					uint8_t nopro[]="--";	
					SetTextValue(current_screen_id,7, nopro);
					
					AnimationPlayFrame(current_screen_id,21, Sys_Params.Pressure_Param.Pressure_Unit);    //显示正压单位
					AnimationPlayFrame(current_screen_id,22, Sys_Params.Pressure_Param.Vacuum_Unit);    //显示负压单位
					if(Sys_Params.Pressure_Param.Pressure_Unit == BAR)
					{
						SetTextValue2Float(current_screen_id,1, Sys_Params.Pressure_Param.TargetPreessure);
					}
					else
					{
						SetTextValue2Float(current_screen_id,1, Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
					}
					if(Sys_Params.Pressure_Param.Vacuum_Unit==VPSI)
					{
						SetTextValue2Float(current_screen_id,4, Sys_Params.Pressure_Param.Targetvacuum);
					}
					else
					{
						SetTextValue2Float(current_screen_id,4, Sys_Params.Pressure_Param.Targetvacuum*mmHg_PSI_TR);
					}
				}								
				if(Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE)
				{
					
					uint8_t nopro[]="--";	
					SetTextValue2Float(current_screen_id,5, Sys_Params.Dispense_Param.Dis_Timer);
					SetTextValue(current_screen_id,6, nopro);
				}
				if(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER)
				{
					uint8_t nopro[]="--";	
					SetTextValue(current_screen_id,6, nopro);
					SetTextValue2Float(current_screen_id,5, Sys_Params.Dispense_Param.Dis_Timer);
				}
				if(Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE)
				{
					SetTextValue2Float(current_screen_id,5, Sys_Params.Dispense_Param.Dis_Timer);
					SetTextValue2Float(current_screen_id,6, Sys_Params.Dispense_Param.Inter_timer);
				}

				//SetButtonValue(current_screen_id,9,Sys_RT_Status.TEACH);
									
				break;
		}
		

		case _SCREEN_SETTING_3:
		{
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态		
				AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络		

							
			break;
		} 
		case _SCREEN_UNIT_4:
		{
			if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
			{
				if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]==BAR)
				{
					SetButtonValue(_SCREEN_UNIT_4,1,1);
					SetButtonValue(_SCREEN_UNIT_4,2,0);
				}
				else
				{
					SetButtonValue(_SCREEN_UNIT_4,1,0);
					SetButtonValue(_SCREEN_UNIT_4,2,1);
				}
			}
			
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态	
			AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
											
			break;
		}
		case _SCREEN_NETWORK_5:
		{
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态		
				AnimationPlayFrame(current_screen_id,1,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
		    AnimationPlayFrame(current_screen_id,2,Sys_Params.IOT_Params.Commication_Mode);     //显示wifi
				SetButtonValue(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
				SetButtonValue(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
				
			break;
		} 
		case _SCREEN_CALIBRATIONP_6:
		{
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态	
				AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
				//SetTextValue2Float(current_screen_id,5, Sys_RT_Status.RT_AN_Pressure);
				//if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
				//{
				SetTextValue2Float(current_screen_id,6, Sys_RT_Status.RT_Pressure); //BUG
				//}
				/*else
				{
					SetTextValue2Float(current_screen_id,6, Sys_RT_Status.RT_Pressure*BAR_PSI_TR);
				}*/
				SetTextValue0Float(current_screen_id,13, Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value);
				SetTextValue0Float(current_screen_id,14, Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value);
			
			break;
		} 
		case _SCREEN_CALIBRATIONV_7:
		{
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态	
				AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
				//SetTextValue2Float(current_screen_id,1, Sys_RT_Status.RT_AN_Vacuum);
				SetTextValue2Float(current_screen_id,2, Sys_RT_Status.RT_Vacuum/BAR_PSI_TR);				
				SetTextValue0Float(current_screen_id,7, Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value);
				SetTextValue0Float(current_screen_id,12, Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value);
			
			break;
		} 
		case _SCREEN_CHANGE_PWD_8:
		{
			if(Sys_Params.FPASS_AC == POWER_ON)
			{
				Sys_Params.SPASS_AC = POWER_ON;
				SetControlEnable(current_screen_id,2,0);
			}
			else
			{
				SetControlEnable(current_screen_id,2,1);
			}
			SetButtonValue(current_screen_id,1,Sys_Params.FPASS_AC == POWER_ON);
			SetButtonValue(current_screen_id,2,Sys_Params.SPASS_AC == POWER_ON);
			
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态	
		  AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);     //显示wifi
			
			break;
		}
		case _SCREEN_RESET_SYCOUNT_9:
		{
			
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态	
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//显示有限网络
			SetTextValueint32_t(current_screen_id,5,Sys_Params.Dispense_Param.sys_count);
			SetTextValueint32_t(current_screen_id,6,Sys_Params.Dispense_Param.shot_num);
			AnimationPlayFrame(current_screen_id,21, Sys_Params.Pressure_Param.Pressure_Unit);    //显示正压单位
			SetTextValueint8_t(current_screen_id,7,Sys_RT_Status.System_RTC.year);
			SetTextValueint8_t(current_screen_id,8,Sys_RT_Status.System_RTC.month);
			SetTextValueint8_t(current_screen_id,9,Sys_RT_Status.System_RTC.day);
			SetTextValueint8_t(current_screen_id,10,Sys_RT_Status.System_RTC.hour);
			SetTextValueint8_t(current_screen_id,11,Sys_RT_Status.System_RTC.minute);
			
			if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)    
			{
				
				SetTextValue2Float(current_screen_id,12, Sys_Params.Pressure_Param.TargetPreessure_Range);

			}
			else
			{
				SetTextValue2Float(current_screen_id,12, Sys_Params.Pressure_Param.TargetPreessure_Range*BAR_PSI_TR);
				
			}	
			
			break;
		}
		case _SCREEN_SYSTEMINI_10:
		{		
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //显示点胶状态	
		  AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);     //显示wifi			
			uint8_t buffer[25] = {0};
			sprintf((char *)buffer,"%s",VERSION);				
			SetTextValue(_SCREEN_SYSTEMINI_10,2,buffer);
//IDH
			uint8_t buffer1[25] = {0};
			sprintf((char *)buffer1,"%s",IDH);				
			SetTextValue(_SCREEN_SYSTEMINI_10,4,buffer1);
			
			SetTextValue(_SCREEN_SYSTEMINI_10,5,mcuidbuffer);
			uint8_t buffer2[50] = {0};
			sprintf((char *)buffer2,"%s",PD);				
			SetTextValue(_SCREEN_SYSTEMINI_10,1,buffer2);
			
			break;
		}
		case _SCREEN_INPUT_FIRPWD_12:
		{
			
			break;
		}

		case _SCREEN_MAIN_UNLOCK_13:
		{
			//Sys_RT_Status.pressure_disable_Mode		
			if(Sys_RT_Status.pressure_disable_Mode == POWER_OFF )
			{
				Sys_RT_Status.System_Option_Mode=STOP_MODE; 
				//SetControlEnable(_SCREEN_MAIN_UNLOCK_13,10,0);
			}

			
			if(Sys_RT_Status.Dispensing_Step == DISPENSING_STOP)
			{
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,5,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,6,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,7,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,8,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE||Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,1,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,2,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,3,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,4,Sys_Params.Dispense_Param.Run_Mode_Params != CONTINUE);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,9,1);
				if(Sys_RT_Status.pressure_disable_Mode == POWER_OFF )
				{
					//Sys_RT_Status.System_Option_Mode=STOP_MODE; 
					SetControlEnable(_SCREEN_MAIN_UNLOCK_13,10,0);
				}
				else
				{
					SetControlEnable(_SCREEN_MAIN_UNLOCK_13,10,1);
				}
				//SetControlEnable(_SCREEN_MAIN_UNLOCK_13,10,1);
				SetControlEnable(current_screen_id,12,(Sys_RT_Status.System_Option_Mode==STOP_MODE)&&(Sys_RT_Status.pressure_disable_Mode == POWER_ON));
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,13,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,14,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,15,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,16,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,17,1);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,18,1);				
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,30,1);
				SetControlEnable(current_screen_id,11,Sys_RT_Status.System_Option_Mode==AUTORUN_MODE);
				SetControlEnable(current_screen_id,19,(Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)&&(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER));									
			}
			SetButtonValue(current_screen_id,9,Sys_RT_Status.pressure_disable_Mode);
			SetButtonValue(current_screen_id,10,Sys_RT_Status.System_Option_Mode);
			SetButtonValue(current_screen_id,11,Sys_RT_Status.SHOT);
			AnimationPlayFrame(current_screen_id,25, Sys_Params.IOT_Params.Commication_Mode);    //显示有限网络
			AnimationPlayFrame(current_screen_id,24, Sys_RT_Status.System_Status);    //显示点胶状态
			AnimationPlayFrame(current_screen_id,20, Sys_Params.Pressure_Param.Tem_Unit);    //显示温度单位

			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,13,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,14,Sys_Params.Dispense_Param.Run_Mode_Params == TIMER);
			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,15,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE);
			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,16,Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE);					
			
			if(Sys_Params.Pressure_Param.Tem_Unit == centi)
			{
			
				SetTextValueFloat(current_screen_id,21, Sys_RT_Status.Temperature_RT);   //温度摄氏
			}
			else 
			{
				SetTextValueFloat(current_screen_id,21, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //温度华氏
			}					
			SetTextValueFloat(current_screen_id,22, Sys_RT_Status.Humidity_RT);     //湿度
			SetTextValueint32_t(current_screen_id,23, Sys_Params.Dispense_Param.shot_num);   //当天点胶次数	
//点胶显示			
			if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
			{
				SetTextValue0Float(current_screen_id,5, Sys_Params.Dispense_Param.program_number);
				SetTextValue2Float(current_screen_id,4, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
				SetTextValue2Float(current_screen_id,8, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
				//SetTextValue2Float(current_screen_id,2, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]);
				//SetTextValue2Float(current_screen_id,3, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]);
				AnimationPlayFrame(current_screen_id,27, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]);    //显示正压单位
				AnimationPlayFrame(current_screen_id,28, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]);    //显示负压单位
				if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]==BAR)    
				{
					
					SetTextValue2Float(current_screen_id,2, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]);
					//Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
				}
				else
				{
					SetTextValue2Float(current_screen_id,2, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]*BAR_PSI_TR);
					//Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]*BAR_PSI_TR;
				}				
				if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]==VPSI)    
				{
					
					SetTextValue2Float(current_screen_id,3, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]);

				}
				else
				{
					SetTextValue2Float(current_screen_id,3, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]*mmHg_PSI_TR);
					
				}	
			}				
			else
			{
				uint8_t nopro[]="--";	
				SetTextValue(current_screen_id,5, nopro);
				
				AnimationPlayFrame(current_screen_id,27, Sys_Params.Pressure_Param.Pressure_Unit);    //显示正压单位
				AnimationPlayFrame(current_screen_id,28, Sys_Params.Pressure_Param.Vacuum_Unit);    //显示负压单位
				if(Sys_Params.Pressure_Param.Pressure_Unit == BAR)
				{
					SetTextValue2Float(current_screen_id,2, Sys_Params.Pressure_Param.TargetPreessure);
				}
				else
				{
					SetTextValue2Float(current_screen_id,2, Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
				}
				if(Sys_Params.Pressure_Param.Vacuum_Unit==VPSI)
				{
					SetTextValue2Float(current_screen_id,3, Sys_Params.Pressure_Param.Targetvacuum);
				}
				else
				{
					SetTextValue2Float(current_screen_id,3, Sys_Params.Pressure_Param.Targetvacuum*mmHg_PSI_TR);
				}
			}
			//AnimationPlayFrame(current_screen_id,27, Sys_Params.Pressure_Param.Pressure_Unit);    //显示正压单位
			//AnimationPlayFrame(current_screen_id,28, Sys_Params.Pressure_Param.Vacuum_Unit);    //显示负压单位
			
			
			if(Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE)
			{				
				uint8_t nopro[]="--";	
				SetTextValue2Float(current_screen_id,4, Sys_Params.Dispense_Param.Dis_Timer);								
				SetTextValue(current_screen_id,8, nopro);				
			}
			if(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER)
			{
				uint8_t nopro[]="--";	
				SetTextValue(current_screen_id,8, nopro);
				SetTextValue2Float(current_screen_id,4, Sys_Params.Dispense_Param.Dis_Timer);
			}
			if(Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE)
			{
				SetTextValue2Float(current_screen_id,4, Sys_Params.Dispense_Param.Dis_Timer);
				SetTextValue2Float(current_screen_id,8, Sys_Params.Dispense_Param.Inter_timer);
			}
			/*
			SetControlEnable(_SCREEN_MAIN_UNLOCK_13,5,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
			SetControlEnable(_SCREEN_MAIN_UNLOCK_13,6,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
			SetControlEnable(_SCREEN_MAIN_UNLOCK_13,7,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
			SetControlEnable(_SCREEN_MAIN_UNLOCK_13,8,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE||Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
			*/
			if(Sys_RT_Status.Dispensing_Step == DISPENSING_STATUS || Sys_RT_Status.Dispensing_Step == DISPENSING_INTER)
			{
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,1,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,2,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,3,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,4,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,5,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,6,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,7,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,8,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,9,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,10,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,12,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,13,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,14,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,15,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,16,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,17,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,18,0);				
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,19,0);
				SetControlEnable(_SCREEN_MAIN_UNLOCK_13,30,0);								
			}	
			
			break;
		}		
		case _SCREEN_SETTING_SAVE_DLG_14:
		{
			break;
		}
		case _SCREEN_PRESSURE_ALM_DLG_16:
		{
			break;
		}	
		case _SCREEN_RESET_SAVE_DLG_15:
		{
			
			break;
		}

	}
}

/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t state)
{ 
	
	current_screen_id = screen_id;//在工程配置中开启画面切换通知，记录当前画面ID
	switch(screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{
			switch(control_id)
			{
				case 2:
				{
					if(state)
					{
						SetScreen(_SCREEN_SYSTEMINI_10);
					}


					break;
				}
				case 3:
				{
					if(Sys_Params.FPASS_AC == POWER_ON)
					{
						SetScreen(_SCREEN_INPUT_FIRPWD_12);
						Level_To_Screen_ID=_SCREEN_MAIN_UNLOCK_13;
						Prev_Screen=_SCREEN_MAIN_2;
					}
					else
					{
						SetScreen(_SCREEN_MAIN_UNLOCK_13);
						NotifyScreen(_SCREEN_MAIN_UNLOCK_13);
					}					
					break;
				}
				

				case 13:
				{
					if(state)
					{
						Sys_RT_Status.PURGE=POWER_ON;
						Sys_Params.Dispense_Param.shot_num++;
						Sys_Params.Dispense_Param.sys_count++;
						
					}
					else
					{
						Sys_RT_Status.PURGE=POWER_OFF;
					}
					break;
				}
				case 10:
				{
					if(state)
					{
						Sys_RT_Status.pressure_disable_Mode=POWER_ON;
					}
					else
					{
						Sys_RT_Status.pressure_disable_Mode=POWER_OFF;
					}
					break;
				}
				case 11:
				{
					if(state)
					{
						Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;
					}
					else
					{
						Sys_RT_Status.System_Option_Mode=STOP_MODE;
					}
					break;
				}
				case 12:
				{
					if(state)
					{
						Sys_RT_Status.SHOT = POWER_ON;

						SaveSystemParams();
						
					}
					else
					{
						Sys_RT_Status.SHOT = POWER_OFF;
					}
					break;
				}
				

			}
			
			break;
		} 
		case _SCREEN_SETTING_3:
		{

			switch(control_id)
			{
				case 1:
				{
					if(state)
					{
						SetScreen(_SCREEN_MAIN_UNLOCK_13);
					}
					current_screen_id = _SCREEN_MAIN_UNLOCK_13;
					
					break;
				}
				case 2:
				{
					if(state)
					{
						SetScreen(_SCREEN_SYSTEMINI_10);
						NotifyScreen(_SCREEN_SYSTEMINI_10);
					}
					
					break;
				} 
				case 3:
				{
					if(state)
					{
						if(Sys_Params.SPASS_AC == POWER_ON)
						{
							SetScreen(_SCREEN_INPUT_SECPWD_18);
							Level_To_Screen_ID=_SCREEN_UNIT_4;
							Prev_Screen=_SCREEN_SETTING_3;
						}
						else
						{
							SetScreen(_SCREEN_UNIT_4);
							NotifyScreen(_SCREEN_UNIT_4);
						}
					}
					break;
				} 				
				case 4:
				{
					if(state)
					{
						if(Sys_Params.SPASS_AC == POWER_ON)
						{
							SetScreen(_SCREEN_INPUT_SECPWD_18);
							Level_To_Screen_ID=_SCREEN_NETWORK_5;
							Prev_Screen=_SCREEN_SETTING_3;
						}
						else
						{
							SetScreen(_SCREEN_NETWORK_5);
							NotifyScreen(_SCREEN_NETWORK_5);
						}
					}
					break;
				}
				case 5:
				{
					if(state)
					{
						if(Sys_Params.SPASS_AC == POWER_ON)
						{
							SetScreen(_SCREEN_INPUT_SECPWD_18);
							Level_To_Screen_ID=_SCREEN_CALIBRATIONP_6;
							Prev_Screen=_SCREEN_SETTING_3;
						}
						else
						{
							SetScreen(_SCREEN_CALIBRATIONP_6);
							NotifyScreen(_SCREEN_CALIBRATIONP_6);
						}
					}
					break;
				}
				case 6:
				{
					if(state)
					{
						if(Sys_Params.SPASS_AC == POWER_ON)
						{
							SetScreen(_SCREEN_INPUT_SECPWD_18);
							Level_To_Screen_ID=_SCREEN_CALIBRATIONV_7;
							Prev_Screen=_SCREEN_SETTING_3;
						}
						else
						{
							SetScreen(_SCREEN_CALIBRATIONV_7);
							NotifyScreen(_SCREEN_CALIBRATIONV_7);
							
						}
					}
					break;
				}
				case 7:
				{					
					if(Sys_Params.SPASS_AC == POWER_ON)
					{
						SetScreen(_SCREEN_INPUT_SECPWD_18);
						Level_To_Screen_ID=_SCREEN_CHANGE_PWD_8;
						Prev_Screen=_SCREEN_SETTING_3;
					}
					else
					{
						SetScreen(_SCREEN_CHANGE_PWD_8);
						NotifyScreen(_SCREEN_CHANGE_PWD_8);						
					}					
					break;
				}
				case 8:
				{										
					if(Sys_Params.SPASS_AC == POWER_ON)
					{
						SetScreen(_SCREEN_INPUT_SECPWD_18);
						Level_To_Screen_ID=_SCREEN_RESET_SYCOUNT_9;
						Prev_Screen=_SCREEN_SETTING_3;
					}
					else
					{
						SetScreen(_SCREEN_RESET_SYCOUNT_9);
						NotifyScreen(_SCREEN_RESET_SYCOUNT_9);
					}
				
				break;
				}
			}
			break;
		} 
		case _SCREEN_UNIT_4:
		{
			switch(control_id)
			{
				case 1:
				{
					if(state)
					{
						Sys_Params.Pressure_Param.Pressure_Unit= BAR;

						SetButtonValue(_SCREEN_UNIT_4,1,1);
						SetButtonValue(_SCREEN_UNIT_4,2,0);
						if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
						{
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]=Sys_Params.Pressure_Param.Pressure_Unit;
						}
						
					}
					else
					{
						Sys_Params.Pressure_Param.Pressure_Unit = PSI;
						SetButtonValue(_SCREEN_UNIT_4,2,1);
						SetButtonValue(_SCREEN_UNIT_4,1,0);
					}
					break;
				}
				case 2:
				{
					if(state)
					{
						Sys_Params.Pressure_Param.Pressure_Unit= PSI;
						SetButtonValue(_SCREEN_UNIT_4,2,1);
						SetButtonValue(_SCREEN_UNIT_4,1,0);
						
					}
					else
					{
						Sys_Params.Pressure_Param.Pressure_Unit = BAR;
						SetButtonValue(_SCREEN_UNIT_4,1,1);
						SetButtonValue(_SCREEN_UNIT_4,2,0);
					}
					if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
					{
						Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]=Sys_Params.Pressure_Param.Pressure_Unit;
					}
					break;
				}
				case 3:
				{
					if(state)
					{
						Sys_Params.Pressure_Param.Vacuum_Unit= mmHg;
						SetButtonValue(_SCREEN_UNIT_4,3,1);
						SetButtonValue(_SCREEN_UNIT_4,6,0);
						
					}
					else
					{
						Sys_Params.Pressure_Param.Vacuum_Unit= VPSI;
						SetButtonValue(_SCREEN_UNIT_4,6,1);
						SetButtonValue(_SCREEN_UNIT_4,3,0);
					}
					if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
					{
						Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]=Sys_Params.Pressure_Param.Vacuum_Unit;
					}
					break;
				}
				case 6:
				{
					if(state)
					{
						Sys_Params.Pressure_Param.Vacuum_Unit= VPSI;
						SetButtonValue(_SCREEN_UNIT_4,6,1);
						SetButtonValue(_SCREEN_UNIT_4,3,0);
						
					}
					else
					{
						Sys_Params.Pressure_Param.Vacuum_Unit= mmHg;
						SetButtonValue(_SCREEN_UNIT_4,3,1);
						SetButtonValue(_SCREEN_UNIT_4,6,0);
					}
					if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
					{
						Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]=Sys_Params.Pressure_Param.Vacuum_Unit;
					}
					break;
				}
				case 7:
				{
					if(state)
					{
						Sys_Params.Pressure_Param.Tem_Unit = fahr;
						SetButtonValue(_SCREEN_UNIT_4,8,0);
						SetButtonValue(_SCREEN_UNIT_4,7,1);
						
					}
					else
					{
						Sys_Params.Pressure_Param.Tem_Unit = centi;
						SetButtonValue(_SCREEN_UNIT_4,8,1);
						SetButtonValue(_SCREEN_UNIT_4,7,0);
					}
					break;
				}
				case 8:
				{
					if(state)
					{
						Sys_Params.Pressure_Param.Tem_Unit = centi;
						SetButtonValue(_SCREEN_UNIT_4,7,0);
						SetButtonValue(_SCREEN_UNIT_4,8,1);
						
					}
					else
					{
						Sys_Params.Pressure_Param.Tem_Unit = fahr;
						SetButtonValue(_SCREEN_UNIT_4,8,0);
						SetButtonValue(_SCREEN_UNIT_4,7,1);
					}
					break;
				}

				

				case 4:
				{
					if(state) 
					{
						Level_To_Screen_ID=_SCREEN_SETTING_SAVE_DLG_14;
						Prev_Screen=_SCREEN_SETTING_3;
						//Sys_Params.save_pro_buffer[pron][0] = Sys_Params.Dispense_Param.Dis_Timer;
						//Sys_Params.save_pro_buffer[pron][1] = Sys_Params.Dispense_Param.Inter_timer;
						//SaveSystemParams();
						
					}
					break;
				}	
				case 5:
				{
					if(state) 
					{
						NotifyScreen(_SCREEN_SETTING_3);	
						LoadSystemParams();						
					}
					break;
				}				
			}
			
			break;
		}
		case _SCREEN_NETWORK_5:
		{
			
			switch(control_id)
			{ 
				case 11:          
				{					
					if(state)
					{
						Sys_Params.IOT_Params.Commication_Mode=ETHERNET_MODE;
						//AnimationPlayFrame(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
						//AnimationPlayFrame(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
						AnimationPlayFrame(_SCREEN_NETWORK_5,1,Sys_Params.IOT_Params.Commication_Mode);
						AnimationPlayFrame(_SCREEN_NETWORK_5,2,Sys_Params.IOT_Params.Commication_Mode);
						SetButtonValue(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
						SetButtonValue(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
					}
					else
					{
						Sys_Params.IOT_Params.Commication_Mode = NO_CONNECT; 
						AnimationPlayFrame(_SCREEN_NETWORK_5,1,Sys_Params.IOT_Params.Commication_Mode);
						AnimationPlayFrame(_SCREEN_NETWORK_5,2,Sys_Params.IOT_Params.Commication_Mode);				
					}
					break;
				}
				case 10:
				{					
					if(state)
					{ 
						Sys_Params.IOT_Params.Commication_Mode=WIFI_MODE;
						//AnimationPlayFrame(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
						//AnimationPlayFrame(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
						AnimationPlayFrame(_SCREEN_NETWORK_5,1,Sys_Params.IOT_Params.Commication_Mode);
						AnimationPlayFrame(_SCREEN_NETWORK_5,2,Sys_Params.IOT_Params.Commication_Mode);
						SetButtonValue(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
						SetButtonValue(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
					}
					else
					{
						Sys_Params.IOT_Params.Commication_Mode = NO_CONNECT; 
						AnimationPlayFrame(_SCREEN_NETWORK_5,1,Sys_Params.IOT_Params.Commication_Mode);
						AnimationPlayFrame(_SCREEN_NETWORK_5,2,Sys_Params.IOT_Params.Commication_Mode);	
						
					}
					break;
				}
				case 12:
				{					
					if(state)
					{
						NotifyScreen(_SCREEN_SETTING_3);
						LoadSystemParams();
					}					 
					break;
				}
				case 14:
				{					
					if(state)
					{
						Level_To_Screen_ID=_SCREEN_SETTING_SAVE_DLG_14;
						Prev_Screen=_SCREEN_SETTING_3;
					}					 
					break;
				}
				
			}
			break;
		}	
		case _SCREEN_CALIBRATIONP_6:
		{
			
			switch(control_id)
			{
				case 7:
				{
					Level_To_Screen_ID=_SCREEN_SETTING_SAVE_DLG_14;
					Prev_Screen=_SCREEN_SETTING_3;
					SetButtonValue(_SCREEN_CALIBRATIONP_6,1,0);
					SetButtonValue(_SCREEN_CALIBRATIONP_6,4,0);
					break;
				}
				
 				case 8:
				{
					//if(state) 
					//{
						NotifyScreen(_SCREEN_SETTING_3);
						SetButtonValue(_SCREEN_CALIBRATIONP_6,1,0);
						SetButtonValue(_SCREEN_CALIBRATIONP_6,4,0);
						LoadSystemParams();
					//}
					break;
				}
				case 1:
				{ 
					if(state) 
					{
						Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable=POWER_ON;		
						Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable=POWER_OFF;		
						//SetButtonValue(current_screen_id,1,1);						
						//SetButtonValue(current_screen_id,4,0);
						//SetButtonValue(current_screen_id,3,0);
					}
					break;
				} 				
				case 2:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=Sys_RT_Status.RT_AN_Pressure;       //比例阀传过来的数字量
						Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value=0;      
						SetButtonValue(current_screen_id,1,0);
					}
					break;
				}
				case 4:
				{ 
					if(state) 
					{
						Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable=POWER_OFF;		
						Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable=POWER_ON;		
						
						//SetButtonValue(current_screen_id,2,0);						
						//SetButtonValue(current_screen_id,4,1);
					}
					break;
				}
				case 3:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value=Sys_RT_Status.RT_AN_Pressure;
						Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value=5;
						SetButtonValue(current_screen_id,4,0);
					}
					break;
				} 


				case 9:
				{ 
					if(state && Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value--;
						SetTextValue0Float(current_screen_id,13,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value); 
					}
					break;
				} 
				case 10:
				{
					if(state && Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value++;
						SetTextValue0Float(current_screen_id,13,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value); 
					}
					break;
				} 
				case 11:
				{
					if(state && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value--;
						SetTextValue0Float(current_screen_id,14,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value); 
					}
					break;
				} 
				case 12:
				{
					if(state && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value++;
						SetTextValue0Float(current_screen_id,14,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value); 
					}
					break;
				}


			}
			break;
		} 
		case _SCREEN_CALIBRATIONV_7:
		{
			
			switch(control_id)
			{

				case 13:
				{
					Level_To_Screen_ID=_SCREEN_SETTING_SAVE_DLG_14;
					Prev_Screen=_SCREEN_SETTING_3;
					SetButtonValue(_SCREEN_CALIBRATIONV_7,3,0);
					SetButtonValue(_SCREEN_CALIBRATIONV_7,4,0);					
					break;
				} 
 				case 14:
				{
					if(state) 
					{
						NotifyScreen(_SCREEN_SETTING_3);
						SetButtonValue(_SCREEN_CALIBRATIONV_7,3,0);
						SetButtonValue(_SCREEN_CALIBRATIONV_7,4,0);	
						LoadSystemParams();
					}
					break;
				}				
				case 8:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_3_Value=Sys_RT_Status.RT_AN_Vacuum;
						Sys_Params.Pressure_Param.Calibration_RT_Param.RT_3_Value=0;
						SetButtonValue(current_screen_id,3,0);		
					}
					break;
				}
				case 9:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_4_Value=Sys_RT_Status.RT_AN_Vacuum;
						Sys_Params.Pressure_Param.Calibration_RT_Param.RT_4_Value=72.551;
						SetButtonValue(current_screen_id,4,0);
					}
					break;
				} 
				case 3:
				{ 
					if(state) 
					{
						Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable=POWER_ON;		
						Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable=POWER_OFF;		
					}
					break;
				}
				case 4:
				{ 
					if(state) 
					{
						Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable=POWER_OFF;		
						Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable=POWER_ON;		
					}
				}
				case 5:
				{ 
					if(state && Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value--;
						SetTextValue0Float(current_screen_id,7,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value); 
					}
					break;
				} 
				case 6:
				{
					if(state && Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value++;
						SetTextValue0Float(current_screen_id,7,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value); 
					}
					break;
				} 
				case 10:
				{
					if(state && Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value--;
						SetTextValue0Float(current_screen_id,12,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value); 
					}
					break;
				} 
				case 11:
				{
					if(state && Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value++;
						SetTextValue0Float(current_screen_id,12,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value); 
					}
					break;
				}
			}
			break;
		} 
		
		case _SCREEN_CHANGE_PWD_8:
		{
			switch(control_id)
			{ 
				case 1:
				{
					if(state) 
					{
						Sys_Params.FPASS_AC = POWER_ON;
						//Sys_RT_Status.SPASS_AC = POWER_ON;
						SetButtonValue(_SCREEN_CHANGE_PWD_8,1,Sys_Params.FPASS_AC);
						SetButtonValue(_SCREEN_CHANGE_PWD_8,2,Sys_Params.SPASS_AC);
						
					}
					else
					{
						Sys_Params.FPASS_AC = POWER_OFF;
					}
					break;
				}
				case 2:
				{
					if(state) 
					{
						Sys_Params.SPASS_AC = POWER_ON;
						
					}
					else
					{
						Sys_Params.SPASS_AC = POWER_OFF;
					}
					break;
				}
					
				case 5:
				{  
					if(state) 
					{  
							
						if(New_Level_1_pwd_Update==1)
						{
							memset(Sys_Params.Login_Pwd.First_Password, 0, PASSWORD_LEN); 
							sprintf((char *)Sys_Params.Login_Pwd.First_Password,"%s",New_Level_1_pwd);	
						}
						if(New_Level_2_pwd_Update==1)
						{
							memset(Sys_Params.Login_Pwd.Second_Password, 0, PASSWORD_LEN); 
							sprintf((char *)Sys_Params.Login_Pwd.Second_Password,"%s",New_Level_2_pwd);	
						}
							SaveSystemParams();
						  New_Level_1_pwd_Update=0;
							New_Level_2_pwd_Update=0;
							memset(New_Level_1_pwd, 0, PASSWORD_LEN); 
							memset(New_Level_2_pwd, 0, PASSWORD_LEN);   
							Prev_Screen=_SCREEN_SETTING_3;	
					}
					break;
				}
				case 6:
				{
					//if(state) 
					//{
						New_Level_1_pwd_Update=0;
						New_Level_2_pwd_Update=0;
						memset(New_Level_1_pwd, 0, PASSWORD_LEN); 
						memset(New_Level_2_pwd, 0, PASSWORD_LEN); 
						NotifyScreen(_SCREEN_SETTING_3);
						LoadSystemParams();						
					//}
				}
			}
			break;
		}
		case _SCREEN_RESET_SYCOUNT_9:
		{
			int32_t value = 0; 
			float value_double = 0;
			switch(control_id)
			{
/*
				case 1: 
				{
					Sys_RT_Status.reset=POWER_ON;
					SetScreen(_SCREEN_RESET_SAVE_DLG_15);	
					NotifyScreen(_SCREEN_RESET_SAVE_DLG_15);					
					break;
				}
				case 2: 
				{
					if(state)
					{
						Sys_In++;
						if(Sys_In>=10)
						{					
							SetScreen(_SCREEN_INPUT_SYSCOUNT_17);
							NotifyScreen(_SCREEN_INPUT_SYSCOUNT_17);	
							Sys_In = 0;
						}
					}
				
					break;
				}*/

				case 3: 
				{
//					Sys_In = 0;
					Level_To_Screen_ID=_SCREEN_SETTING_SAVE_DLG_14;
					Prev_Screen=_SCREEN_SETTING_3;
					SaveSystemParams();
					break;
				}
				case 4: 
				{
//					Sys_In = 0;
					LoadSystemParams();
					NotifyScreen(_SCREEN_SETTING_3);
					break;
				}
				//settime_flag=1;
				case 13: 
				{
					settime_flag=1;
					Sys_RT_Status.System_RTC.year_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.year);
					Sys_RT_Status.System_RTC.month_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.month);
					Sys_RT_Status.System_RTC.day_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.day);
					Sys_RT_Status.System_RTC.hour_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.hour);
					Sys_RT_Status.System_RTC.minute_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.minute);
					break;
				}
			}
			break;
		}
		case _SCREEN_SYSTEMINI_10:
		{
			switch(control_id)
			{
				

				case 6: 
				{
					SetScreen(_SCREEN_SETTING_3);
					NotifyScreen(_SCREEN_SETTING_3);					
					break;
				}
				case 7: 
				{
						SetScreen(_SCREEN_MAIN_UNLOCK_13);
						NotifyScreen(_SCREEN_MAIN_UNLOCK_13);				
					break;
				}

			}
			break;
		}
	
	 

		case _SCREEN_INPUT_FIRPWD_12:
		{
			switch(control_id)
			{ 
				case 2:
				{ 
					if(state) 
					{ 
						uint8_t login_Success_1=0;
						
						if(Level_To_Screen_ID==_SCREEN_MAIN_UNLOCK_13)
						{
							if(strcmp(Input_FPwd,Sys_Params.Login_Pwd.First_Password)==0||strcmp(Input_FPwd,Super_Level_1_pwd)==0)
							{
								login_Success_1=1;
							}
						}
						 if(login_Success_1==1)
						 {
							 SetScreen(Level_To_Screen_ID);		
							 NotifyScreen(Level_To_Screen_ID);
						 }	
						 else
						 {
							 SetScreen(Prev_Screen);	
							 NotifyScreen(Prev_Screen);
						 }
						uint8_t buffer[15] = {0};
						sprintf((char *)buffer,"%s","");	 
						SetTextValue(_SCREEN_INPUT_FIRPWD_12,1,buffer);
						Level_To_Screen_ID=Prev_Screen;
						memset(Input_FPwd, 0, PASSWORD_LEN); 
					}
					break;
				}
				case 3: 
				{
					if(state) 
					{ 
						 SetScreen(Prev_Screen);
						 Level_To_Screen_ID=Prev_Screen;
						 memset(Input_FPwd, 0, PASSWORD_LEN); 
					}
					break;
				}
			}
			break;
		}
		case _SCREEN_INPUT_SECPWD_18:
		{
			switch(control_id)
			{ 
				case 2:
				{ 
					if(state) 
					{ 
						uint8_t login_Success=0;
						if(Level_To_Screen_ID==_SCREEN_UNIT_4||Level_To_Screen_ID==_SCREEN_NETWORK_5||Level_To_Screen_ID==_SCREEN_CALIBRATIONP_6||Level_To_Screen_ID==_SCREEN_CALIBRATIONV_7||
							Level_To_Screen_ID==_SCREEN_CHANGE_PWD_8 ||Level_To_Screen_ID==_SCREEN_RESET_SYCOUNT_9)
						{
							if(strcmp(Input_SPwd,Sys_Params.Login_Pwd.Second_Password)==0||strcmp(Input_SPwd,Super_Level_2_pwd)==0)
							{
								login_Success=1;
							}
						}
					 if(login_Success==1)
					 {
						 SetScreen(Level_To_Screen_ID);		
						 NotifyScreen(Level_To_Screen_ID);
					 }	
					 else
					 {
						 SetScreen(Prev_Screen);	
						 NotifyScreen(Prev_Screen);
					 }
						uint8_t buffer[15] = {0};
						sprintf((char *)buffer,"%s","");	 
						SetTextValue(_SCREEN_INPUT_SECPWD_18,1,buffer);
						Level_To_Screen_ID=Prev_Screen;
						memset(Input_SPwd, 0, PASSWORD_LEN); 
					}
					break;
				}
				case 3: 
				{
					if(state) 
					{ 
						 SetScreen(Prev_Screen);
						 Level_To_Screen_ID=Prev_Screen;
						 memset(Input_SPwd, 0, PASSWORD_LEN); 
					}
					break;
				}
			}
			break;
		}
		case _SCREEN_MAIN_UNLOCK_13:
		{
			switch(control_id)
			{
				case 1:
				{
					SaveSystemParams();
					AnimationPlayFrame(_SCREEN_MAIN_2,9, Sys_Params.Dispense_Param.Run_Mode_Params);    //显示点胶模式
					SetScreen(_SCREEN_MAIN_2);
					NotifyScreen(_SCREEN_MAIN_2);					
					break;
				}
				case 6:
				{

					if(state&&Sys_Params.Dispense_Param.program_number>=1) 
					{
						Sys_Params.Dispense_Param.program_number-=1;
						//pron = Sys_Params.Dispense_Param.program_number;
						//Sys_Params.Dispense_Param.Dis_Timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
						//Sys_Params.Dispense_Param.Inter_timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1];
						//Sys_Params.Dispense_Param.Dis_Timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
						//Sys_Params.Dispense_Param.Inter_timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1];
						Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
						Sys_Params.Pressure_Param.Targetvacuum = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3];				
						Sys_Params.Pressure_Param.Pressure_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4];
						Sys_Params.Pressure_Param.Vacuum_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5];
						SetTextValue0Float(current_screen_id,5,Sys_Params.Dispense_Param.program_number);
						SetTextValue2Float(current_screen_id,4,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
						SetTextValue2Float(current_screen_id,8,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);

						
					}
					else
					{
						if(state&&Sys_Params.Dispense_Param.program_number==0) 
						{
							Sys_Params.Dispense_Param.program_number = 39;
							Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
							Sys_Params.Pressure_Param.Targetvacuum = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3];				
							Sys_Params.Pressure_Param.Pressure_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4];
							Sys_Params.Pressure_Param.Vacuum_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5];
							SetTextValue0Float(current_screen_id,5,Sys_Params.Dispense_Param.program_number);
							SetTextValue2Float(current_screen_id,4,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
							SetTextValue2Float(current_screen_id,8,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
							
						}
					}
					break;

				}
				case 7:
				{
					if(state==1)
					{
						if(Sys_Params.Dispense_Param.program_number<39) 
						{
							Sys_Params.Dispense_Param.program_number+=1;
							//pron = Sys_Params.Dispense_Param.program_number;
							//Sys_Params.Dispense_Param.Dis_Timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
							//Sys_Params.Dispense_Param.Inter_timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1];
							//Sys_Params.Dispense_Param.Dis_Timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
							//Sys_Params.Dispense_Param.Inter_timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1];
							Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
							Sys_Params.Pressure_Param.Targetvacuum = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3];				
							Sys_Params.Pressure_Param.Pressure_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4];
							Sys_Params.Pressure_Param.Vacuum_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5];
							SetTextValue0Float(current_screen_id,5,Sys_Params.Dispense_Param.program_number);
							SetTextValue2Float(current_screen_id,4,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
							SetTextValue2Float(current_screen_id,8,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
							
						}
						else
						{
							Sys_Params.Dispense_Param.program_number=0;
							Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
							Sys_Params.Pressure_Param.Targetvacuum = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3];				
							Sys_Params.Pressure_Param.Pressure_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4];
							Sys_Params.Pressure_Param.Vacuum_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5];
							SetTextValue0Float(current_screen_id,5,Sys_Params.Dispense_Param.program_number);
							SetTextValue2Float(current_screen_id,4,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
							SetTextValue2Float(current_screen_id,8,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
						}
					}
					break;

				}
				case 9:
				{
					if(state)
					{
						Sys_RT_Status.pressure_disable_Mode=POWER_ON;
					}
					else
					{
						Sys_RT_Status.pressure_disable_Mode=POWER_OFF;
					}
					break;
				}
				case 10:
				{
					if(state)
					{
						Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;
						SetButtonValue(current_screen_id,12,0);
						Sys_RT_Status.PURGE=POWER_OFF;
					}
					else
					{
						Sys_RT_Status.System_Option_Mode=STOP_MODE;
					}
					break;
				}
				case 11:
				{
					if(state)
					{
						Sys_RT_Status.SHOT = POWER_ON;						
					}
					else
					{
						Sys_RT_Status.SHOT = POWER_OFF;
					}
					break;
				}
				case 19:
				{
					if(state)
					{
						Sys_RT_Status.TEACH = POWER_ON;
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,19,Sys_RT_Status.TEACH);
						
					}
					else
					{
						Sys_RT_Status.TEACH = POWER_OFF;
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,19,Sys_RT_Status.TEACH);
						
					}
					break;
				}
				case 12:
				{
					if(state)
					{
						Sys_RT_Status.PURGE=POWER_ON;
						Sys_Params.Dispense_Param.shot_num++;
						Sys_Params.Dispense_Param.sys_count++;
						
					}
					else
					{
						Sys_RT_Status.PURGE=POWER_OFF;
					}
					break;
				}
				case 13:
				{
					if(state)
					{
						Sys_Params.Dispense_Param.Run_Mode_Params = PRO_NO;
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,13,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,14,Sys_Params.Dispense_Param.Run_Mode_Params == TIMER);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,15,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,16,Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE);
						//Sys_Params.Dispense_Param.Dis_Timer = Sys_Params.save_pro_buffer[pron][0];	
						//Sys_Params.Dispense_Param.Inter_timer = Sys_Params.save_pro_buffer[pron][1];
						
					}
					break;
				}
				case 14:
				{
					if(state)
					{
						Sys_Params.Dispense_Param.Run_Mode_Params = TIMER;
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,13,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,14,Sys_Params.Dispense_Param.Run_Mode_Params == TIMER);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,15,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,16,Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE);

						
					}
					break;
				}
				case 15:
				{
					if(state)
					{
						Sys_Params.Dispense_Param.Run_Mode_Params = CYCLE;
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,13,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,14,Sys_Params.Dispense_Param.Run_Mode_Params == TIMER);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,15,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,16,Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE);
						
					}
					break;
				}
				case 16:
				{
					if(state)
					{
						Sys_Params.Dispense_Param.Run_Mode_Params = CONTINUE;
						cont_flag	= 0;					
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,13,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,14,Sys_Params.Dispense_Param.Run_Mode_Params == TIMER);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,15,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE);
						SetButtonValue(_SCREEN_MAIN_UNLOCK_13,16,Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE);

					}
					else
					{
						cont_flag = 1;
					}
					break;
				}
				
				case 17:
				{
						SetScreen(_SCREEN_SETTING_3);
						NotifyScreen(_SCREEN_SETTING_3);
					break;
				}
				case 18:
				{
						SetScreen(_SCREEN_SYSTEMINI_10);
						NotifyScreen(_SCREEN_SYSTEMINI_10);
					break;
				}	
				case 30:
				{
					if(state)
					{
						SaveSystemParams();

					}

					break;
				}					
			}			
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_14:
		{ 
					
			switch(control_id)
			{
				case 1:
				{
					if(state) 
					{
						SaveSystemParams();
						SetScreen(Prev_Screen);
						NotifyScreen(Prev_Screen);
					}
					break;
				}
				case 2: 
				{
					if(state) 
					{
						LoadSystemParams();
						SetScreen(Prev_Screen);
						NotifyScreen(Prev_Screen);
					}
					break;
				}
			}
			break;
		}
		case _SCREEN_RESET_SAVE_DLG_15:
		{		
			switch(control_id)
			{
				case 1:
				{
					if(state) 
					{
						if(Sys_RT_Status.reset == POWER_ON)
						{
							Sys_Params.Dispense_Param.shot_num = 0;
							SetScreen(_SCREEN_RESET_SYCOUNT_9);
						}					
					}
					break;
				}
				case 2: 
				{
					if(state) 
					{
						LoadSystemParams();
						SetScreen(_SCREEN_RESET_SYCOUNT_9);
					}
					break;
				}
			}
			break; 
		}
		case _SCREEN_PRESSURE_ALM_DLG_16:
		{
			switch(control_id)
			{

				case 1: 
				{
					if(state) 
					{
						GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=POWER_OFF;  //20240408
						Pressure_Display_Enable=POWER_OFF;
						SetScreen(_SCREEN_MAIN_UNLOCK_13);
						current_screen_id = _SCREEN_MAIN_UNLOCK_13;
					}
					break;
				}
			}
			break;
		}
		case _SCREEN_INPUT_SYSCOUNT_17:
		{			
			switch(control_id)
			{

				case 2: 
				{
					if(state) 
					{
						SaveSystemParams();
						Prev_Screen=_SCREEN_RESET_SYCOUNT_9;
						
					}
					break;
				}
				case 3: 
				{
					if(state) 
					{
						SetScreen(_SCREEN_RESET_SYCOUNT_9);
						NotifyScreen(_SCREEN_RESET_SYCOUNT_9);
					}
					break;
				}
			}
			break;
		}
	}
		

}

/*! 
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
	//TODO: 添加用户代码
	int32_t value = 0; 
	float value_double = 0;
	current_screen_id = screen_id;//在工程配置中开启画面切换通知，记录当前画面ID
	
	switch(screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{
			break;
		} 
		case _SCREEN_MAIN_UNLOCK_13:
		{
			sscanf((char *)str,"%f",&value_double);//把字符串转换为数
			switch (control_id)
			{
				case 5:
				{					
					if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
					{
						if(value_double<=39)
						{
							Sys_Params.Dispense_Param.program_number=value_double;
							//pron = Sys_Params.Dispense_Param.program_number;
							//Sys_Params.Dispense_Param.Dis_Timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
							//Sys_Params.Dispense_Param.Inter_timer = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1];
							Sys_Params.Pressure_Param.TargetPreessure = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2];
							Sys_Params.Pressure_Param.Targetvacuum = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3];				
							Sys_Params.Pressure_Param.Pressure_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4];
							Sys_Params.Pressure_Param.Vacuum_Unit = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5];
							SetTextValue2Float(current_screen_id,4,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
							SetTextValue2Float(current_screen_id,8,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
						}
						else
						{
							value_double = 39;
						}
					}
					break;
				}				
				case 4:
				{
					
						//Sys_Params.Dispense_Param.Dis_Timer = fabsf(value_double);	  //0817
						if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
						{
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]=fabsf(value_double);
							SetTextValue2Float(current_screen_id,4,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
						}
						else
						{
							Sys_Params.Dispense_Param.Dis_Timer = fabsf(value_double);
							SetTextValue2Float(current_screen_id,4,Sys_Params.Dispense_Param.Dis_Timer);	
						}				
					break;
				}
				case 8:
				{
						if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
						{
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]=fabsf(value_double);
							SetTextValue2Float(current_screen_id,8,Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
							/*if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]==0)
							{
								Sys_Params.Dispense_Param.Pro_mode = POWER_OFF;
							}
							else
							{
								Sys_Params.Dispense_Param.Pro_mode = POWER_ON;
							}*/							
						}
						else
						{
							Sys_Params.Dispense_Param.Inter_timer = fabsf(value_double);
							SetTextValue2Float(current_screen_id,8,Sys_Params.Dispense_Param.Inter_timer);
						}
					break;
				}
				
				case 2:
				{										
					if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
					{
						if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]==BAR)
						{
							Sys_Params.Pressure_Param.TargetPreessure=fabsf(value_double);
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]=fabsf(value_double);
						}
						else
						{
							Sys_Params.Pressure_Param.TargetPreessure=(value_double/BAR_PSI_TR);
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]=fabsf(value_double)/BAR_PSI_TR;
						}
					}
					else
					{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							Sys_Params.Pressure_Param.TargetPreessure=fabsf(value_double);						
						}
						else
						{
							Sys_Params.Pressure_Param.TargetPreessure=fabsf(value_double/BAR_PSI_TR);		
						}						
					}
					break;
				}				
				case 3:
				{
					if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)	
					{
						if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]==VPSI)
						{
							Sys_Params.Pressure_Param.Targetvacuum=fabsf(value_double);
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]=fabsf(value_double);
						}
						else
						{
							Sys_Params.Pressure_Param.Targetvacuum=fabsf(value_double/BAR_PSI_TR);
							Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]=fabsf(value_double)/mmHg_PSI_TR;
						}
					}
					else
					{
						if(Sys_Params.Pressure_Param.Vacuum_Unit==VPSI)
						{
							Sys_Params.Pressure_Param.Targetvacuum=fabsf(value_double);						
						}
						else
						{
							Sys_Params.Pressure_Param.Targetvacuum=fabsf(value_double/mmHg_PSI_TR);		
						}						
					}										
					break;
				}
			}
			break;
		} 
		case _SCREEN_CALIBRATIONP_6:
		{
			
			sscanf((char *)str,"%f",&value_double);//把字符串转换为数
			switch(control_id)
			{
				case 13:
				{
					Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value=0;
					if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON)
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value=(value_double+(0.005));   
					}
					break;
				}
				case 14:
				{
					Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value=5.0;
					if(Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON)
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value=(value_double+(0.005));
					}
					break;
				}
			}
			break;
		} 
		case _SCREEN_CALIBRATIONV_7:
		{
			
			sscanf((char *)str,"%f",&value_double);//把字符串转换为数
			switch(control_id)
			{
				case 7:
				{
					Sys_Params.Pressure_Param.Calibration_Set_Param.RT_3_Value=0;
					if(Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable==POWER_ON)
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value=(value_double+(0.005));
					}
					break;
				}
				case 12:
				{
					Sys_Params.Pressure_Param.Calibration_Set_Param.RT_4_Value=72.551;
					if(Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable==POWER_ON)
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value=(value_double+(0.005));
					}
					break;
				}
			}
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_14:
		{
			break;
		}

		case _SCREEN_PRESSURE_ALM_DLG_16:
		{
			break;
		}
		case _SCREEN_RESET_SYCOUNT_9:
		{
			sscanf((char *)str,"%f",&value_double);//把字符串转换为数
			switch (control_id)
			{
				case 7:
				{	
					Sys_RT_Status.System_RTC.year=value_double;
					Sys_RT_Status.System_RTC.year_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.year);
					//settime_flag=1;
					
					break;
				}
				case 8:
				{
					Sys_RT_Status.System_RTC.month=value_double;	
					Sys_RT_Status.System_RTC.month_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.month);
					//settime_flag=1;
					break;
				}
				case 9:
				{	
					Sys_RT_Status.System_RTC.day=value_double;
					Sys_RT_Status.System_RTC.day_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.day);
					//settime_flag=1;
					break;
				}
				case 10:
				{	
					Sys_RT_Status.System_RTC.hour=value_double;
					Sys_RT_Status.System_RTC.hour_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.hour);
					//settime_flag=1;					
					break;
				}
				case 11:
				{	
					Sys_RT_Status.System_RTC.minute=value_double;
					Sys_RT_Status.System_RTC.minute_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.minute);
					//settime_flag=1;
					break;
				}
				case 12:
				{		
					Sys_Params.Pressure_Param.TargetPreessure_Range = value_double;
					if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
					{
						Sys_Params.Pressure_Param.TargetPreessure_Range=value_double;
						//Sys_Params.save_pro_buffer[pron][2]=Sys_Params.Pressure_Param.TargetPreessure;	
					}
					else
					{
						Sys_Params.Pressure_Param.TargetPreessure_Range=(value_double/BAR_PSI_TR);
						//Sys_Params.save_pro_buffer[pron][2]=Sys_Params.Pressure_Param.TargetPreessure;	
					}
					break;
				}
				
			}

			break;
		}
		case _SCREEN_SYSTEMINI_10:
		{
			break;
		}
		case _SCREEN_NETWORK_5:
		{ 
			switch (control_id)
			{
				case 3:
				{
					sprintf((char *)Sys_Params.IOT_Params.Ethernet_Params.IP_Address,"%s",str);			
					break;
				}
				case 4:
				{
					sprintf((char *)Sys_Params.IOT_Params.Ethernet_Params.Net_Mask,"%s",str);	
					break;
				}
				case 5:
				{
					sprintf((char *)Sys_Params.IOT_Params.Ethernet_Params.Gate_way,"%s",str);		
					break;
				}
				case 6:
				{
					sprintf((char *)Sys_Params.IOT_Params.Server_Params.Server_IPAddress,"%s",str);	
					break;
				}
				case 8:
				{
					sprintf((char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name,"%s",str);		
					break;
				}
				case 9:
				{
					sprintf((char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_Password,"%s",str);		
					break;
				}
				case 7:
				{
				  sscanf((char *)str,"%f",&value_double);//把字符串转换为数 
					Sys_Params.IOT_Params.Server_Params.Sever_Port=value_double;
					break;
				}
			}
			break;
			   
		}

		case _SCREEN_CHANGE_PWD_8:
		{
			switch (control_id)
			{ 
				case 3:
				{
					New_Level_1_pwd_Update=1;
					memset(New_Level_1_pwd, 0, PASSWORD_LEN); 
					sprintf((char *)New_Level_1_pwd,"%s",str);		
				}
				case 4:
				{
					
					New_Level_2_pwd_Update=1;
					memset(New_Level_2_pwd, 0, PASSWORD_LEN); 
					sprintf((char *)New_Level_2_pwd,"%s",str);		
				}
			}
			break;
		}
		case _SCREEN_INPUT_FIRPWD_12:
		{
			switch (control_id)
			{ 
				case 1:
				{
					
					memset(Input_FPwd, 0, PASSWORD_LEN); 
					sprintf((char *)Input_FPwd,"%s",str);		
				}
				default:
					break;
			}
			break;
		}
		case _SCREEN_INPUT_SECPWD_18:
		{
			switch (control_id)
			{ 
				case 1:
				{
					
					memset(Input_FPwd, 0, PASSWORD_LEN); 
					sprintf((char *)Input_SPwd,"%s",str);		
				}
				default:
					break;
			}
			break;
		}
		case _SCREEN_KEYBOARD_11:
		{
			sscanf((char *)str,"%f",&value_double);
			switch (control_id)
			{
				case 1:
				{
					attmp = value_double;
					break;				
				}
				default:
					break;
			}
			
		}
		case _SCREEN_INPUT_SYSCOUNT_17:
		{
			sscanf((char *)str,"%d",&value);//把字符串转换为数
			switch(control_id)
			{
				case 1:
				{
					Sys_Params.Dispense_Param.sys_count = value;
					//SetTextValueint32_t(_SCREEN_RESET_SYCOUNT_9,2,Sys_Params.Dispense_Param.sys_count);
					SetTextValueint32_t(_SCREEN_INPUT_SYSCOUNT_17,1,Sys_Params.Dispense_Param.sys_count);
					break;
				}
			}
		}
		
	}
}

/*! 
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
	if(screen_id==5&&control_id==2)//滑块控制
	{
		//test_value = value;

		//SetProgressValue(5,1,test_value); //更新进度条数值
	}
}

/*! 
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8_t status)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取RTC时间，注意返回的是BCD码
 *  \param year 年（BCD）
 *  \param month 月（BCD）
 *  \param week 星期（BCD）
 *  \param day 日（BCD）
 *  \param hour 时（BCD）
 *  \param minute 分（BCD）
 *  \param second 秒（BCD）
 */
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
	Sys_RT_Status.System_RTC.year=RTC_BCD_TO_TEN(year);
	Sys_RT_Status.System_RTC.month=RTC_BCD_TO_TEN(month);
	Sys_RT_Status.System_RTC.week=RTC_BCD_TO_TEN(week);
	Sys_RT_Status.System_RTC.day=RTC_BCD_TO_TEN(day);
	Sys_RT_Status.System_RTC.hour=RTC_BCD_TO_TEN(hour);
	Sys_RT_Status.System_RTC.minute=RTC_BCD_TO_TEN(minute);
	Sys_RT_Status.System_RTC.second=RTC_BCD_TO_TEN(second)+1; 
}
void NotifyWriteRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
	RTC_TEN_TO_BCD(year);
	
}

/*! 
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//指令类型
	uint8_t ctrl_msg = msg->ctrl_msg;   //消息的类型
	uint8_t control_type = msg->control_type;//控件类型
	uint16_t screen_id = PTR2U16(&msg->screen_id);//画面ID
	uint16_t control_id = PTR2U16(&msg->control_id);//控件ID
	uint32_t value = PTR2U32(msg->param);//数值

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://触摸屏按下
	case NOTIFY_TOUCH_RELEASE://触摸屏松开
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://写FLASH成功
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://写FLASH失败
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://读取FLASH成功
		NotifyReadFlash(1,cmd_buffer+2,size-6);//去除帧头帧尾
		break;
	case NOTIFY_READ_FLASH_FAILD://读取FLASH失败
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://读取RTC时间
		NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
		//Sys_RT_Status.System_RTC.year = cmd_buffer[2];
		//Sys_RT_Status.System_RTC.month = cmd_buffer[3];
		//Sys_RT_Status.System_RTC.day = cmd_buffer[5];
		//Sys_RT_Status.System_RTC.hour = cmd_buffer[6];
		//Sys_RT_Status.System_RTC.minute = cmd_buffer[7];
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//画面ID变化通知
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //按钮控件
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://文本控件
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //进度条控件
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //滑动条控件
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //仪表控件
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://菜单控件
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://选择控件
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://倒计时控件
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}
