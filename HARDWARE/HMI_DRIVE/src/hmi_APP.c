#include "HMI_App.h" 
#include "includes.h" 

#define UI_STYLE_BW			0//��ɫ
uint16_t current_screen_id=0;
uint8_t Pump_AccButtonView=0;
	
uint16_t BackICONIndex=40;
uint16_t StatusICONIndex=50;
void SetRTCTime(uint8_t second,uint8_t minute,uint8_t hour,uint8_t day,uint8_t week,uint8_t month,uint8_t year);
uint8_t cmd_buffer[CMD_MAX_SIZE];//ָ���
static int32_t test_value = 0;//����ֵ
  
static uint8_t Level_Vol_Enable_Counter=0;	
extern POWER_STATUS m_Last_DI_Dispensing_Status;
uint8_t Auto_AT=0;
uint8_t Temp_Alm_Counter_Trigger=0;
uint8_t settime_flag=0;
uint8_t cont_flag=0;

POWER_STATUS Pressure_Display_Enable=POWER_OFF;     //ѹ��������

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
	sprintf((char *)buffer,"%d",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValueint8_t(uint16_t screen_id, uint16_t control_id,int8_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%d",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValue0Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%0.0lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.1lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValue2Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.2lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValue3Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.3lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValue4Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.4lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
} 
 
/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16_t screen_id)
{
	
	//TODO: ����û�����
	current_screen_id = screen_id;//�ڹ��������п��������л�֪ͨ����¼��ǰ����ID
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
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬
				AnimationPlayFrame(current_screen_id,14, Sys_Params.Pressure_Param.Tem_Unit);    //��ʾ�¶ȵ�λ
				AnimationPlayFrame(current_screen_id,9, Sys_Params.Dispense_Param.Run_Mode_Params);    //��ʾ�㽺ģʽ

				AnimationPlayFrame(current_screen_id,2,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
				
				if(Sys_Params.Pressure_Param.Tem_Unit == centi)
				{
				
					SetTextValueFloat(current_screen_id,15, Sys_RT_Status.Temperature_RT);   //�¶�����
				}
				else
				{
					SetTextValueFloat(current_screen_id,15, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //�¶Ȼ���
				}					
				SetTextValueFloat(current_screen_id,16, Sys_RT_Status.Humidity_RT);     //ʪ��
				SetTextValueint32_t(current_screen_id,17, Sys_Params.Dispense_Param.shot_num);   //����㽺����			

				
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
			AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
			break;
		} 
		case _SCREEN_UNIT_4:
		{ 	
			AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
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
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
			
			break;
		} 
		case _SCREEN_CALIBRATIONV_7:
		{	
			SetTextValue0Float(current_screen_id,7,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value);
			SetTextValue0Float(current_screen_id,12,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value);
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
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
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
			SetTextValueint32_t(current_screen_id,5,Sys_Params.Dispense_Param.sys_count);
			break;
		} 
		case _SCREEN_SYSTEMINI_10:
		{
			AnimationPlayFrame(_SCREEN_SYSTEMINI_10,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
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
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
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
				SetControlEnable(current_screen_id,11,POWER_ON);    //shot�ɰ�
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
			AnimationPlayFrame(current_screen_id,25, Sys_Params.IOT_Params.Commication_Mode);    //��ʾ��������
			AnimationPlayFrame(current_screen_id,24, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬
			AnimationPlayFrame(current_screen_id,20, Sys_Params.Pressure_Param.Tem_Unit);    //��ʾ�¶ȵ�λ
			//AnimationPlayFrame(current_screen_id,27, Sys_Params.Pressure_Param.Pressure_Unit);    //��ʾ��ѹ��λ
			AnimationPlayFrame(current_screen_id,28, Sys_Params.Pressure_Param.Vacuum_Unit);    //��ʾ��ѹ��λ

			
			if(Sys_Params.Pressure_Param.Tem_Unit == centi)
			{
			
				SetTextValueFloat(current_screen_id,21, Sys_RT_Status.Temperature_RT);   //�¶�����
			}
			else 
			{
				SetTextValueFloat(current_screen_id,21, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //�¶Ȼ���
			}					
			SetTextValueFloat(current_screen_id,22, Sys_RT_Status.Humidity_RT);     //ʪ��
			SetTextValueint32_t(current_screen_id,23, Sys_Params.Dispense_Param.shot_num);   //����㽺����		
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
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: ����û�����
}

void UI_Init(void)
{ 
//	SetTextSpeed(0,2,10);
//	UpdateUI();
	
//	if(System_Language==LANG_ENGLISH)
//	{
//		//��������ΪEnglish
//		Setlanguage_EN();
//	}
//	else
//	{
//		//��������Ϊ����
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
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬
				AnimationPlayFrame(current_screen_id,14, Sys_Params.Pressure_Param.Tem_Unit);    //��ʾ�¶ȵ�λ
				AnimationPlayFrame(current_screen_id,9, Sys_Params.Dispense_Param.Run_Mode_Params);    //��ʾ�㽺ģʽ
				//AnimationPlayFrame(current_screen_id,21, Sys_Params.Pressure_Param.Pressure_Unit);    //��ʾ��ѹ��λ
				//AnimationPlayFrame(current_screen_id,22, Sys_Params.Pressure_Param.Vacuum_Unit);    //��ʾ��ѹ��λ
				AnimationPlayFrame(current_screen_id,2,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
				
				if(Sys_Params.Pressure_Param.Tem_Unit == centi)
				{
				
					SetTextValueFloat(current_screen_id,15, Sys_RT_Status.Temperature_RT);   //�¶�����
				}
				else
				{
					SetTextValueFloat(current_screen_id,15, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //�¶Ȼ���
				}					
				SetTextValueFloat(current_screen_id,16, Sys_RT_Status.Humidity_RT);     //ʪ��
				SetTextValueint32_t(current_screen_id,17, Sys_Params.Dispense_Param.shot_num);   //����㽺����	
				
			 
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
					AnimationPlayFrame(current_screen_id,21, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]);    //��ʾ��ѹ��λ
					AnimationPlayFrame(current_screen_id,22, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]);    //��ʾ��ѹ��λ
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
					
					AnimationPlayFrame(current_screen_id,21, Sys_Params.Pressure_Param.Pressure_Unit);    //��ʾ��ѹ��λ
					AnimationPlayFrame(current_screen_id,22, Sys_Params.Pressure_Param.Vacuum_Unit);    //��ʾ��ѹ��λ
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
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬		
				AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������		

							
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
			
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬	
			AnimationPlayFrame(current_screen_id,9,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
											
			break;
		}
		case _SCREEN_NETWORK_5:
		{
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬		
				AnimationPlayFrame(current_screen_id,1,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
		    AnimationPlayFrame(current_screen_id,2,Sys_Params.IOT_Params.Commication_Mode);     //��ʾwifi
				SetButtonValue(_SCREEN_NETWORK_5,10,Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE);
				SetButtonValue(_SCREEN_NETWORK_5,11,Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE);
				
			break;
		} 
		case _SCREEN_CALIBRATIONP_6:
		{
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬	
				AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
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
				AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬	
				AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
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
			
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬	
		  AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);     //��ʾwifi
			
			break;
		}
		case _SCREEN_RESET_SYCOUNT_9:
		{
			
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬	
			AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);    	//��ʾ��������
			SetTextValueint32_t(current_screen_id,5,Sys_Params.Dispense_Param.sys_count);
			SetTextValueint32_t(current_screen_id,6,Sys_Params.Dispense_Param.shot_num);
			AnimationPlayFrame(current_screen_id,21, Sys_Params.Pressure_Param.Pressure_Unit);    //��ʾ��ѹ��λ
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
			AnimationPlayFrame(current_screen_id,18, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬	
		  AnimationPlayFrame(current_screen_id,15,Sys_Params.IOT_Params.Commication_Mode);     //��ʾwifi			
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
			AnimationPlayFrame(current_screen_id,25, Sys_Params.IOT_Params.Commication_Mode);    //��ʾ��������
			AnimationPlayFrame(current_screen_id,24, Sys_RT_Status.System_Status);    //��ʾ�㽺״̬
			AnimationPlayFrame(current_screen_id,20, Sys_Params.Pressure_Param.Tem_Unit);    //��ʾ�¶ȵ�λ

			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,13,Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO);
			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,14,Sys_Params.Dispense_Param.Run_Mode_Params == TIMER);
			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,15,Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE);
			SetButtonValue(_SCREEN_MAIN_UNLOCK_13,16,Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE);					
			
			if(Sys_Params.Pressure_Param.Tem_Unit == centi)
			{
			
				SetTextValueFloat(current_screen_id,21, Sys_RT_Status.Temperature_RT);   //�¶�����
			}
			else 
			{
				SetTextValueFloat(current_screen_id,21, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));  //�¶Ȼ���
			}					
			SetTextValueFloat(current_screen_id,22, Sys_RT_Status.Humidity_RT);     //ʪ��
			SetTextValueint32_t(current_screen_id,23, Sys_Params.Dispense_Param.shot_num);   //����㽺����	
//�㽺��ʾ			
			if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
			{
				SetTextValue0Float(current_screen_id,5, Sys_Params.Dispense_Param.program_number);
				SetTextValue2Float(current_screen_id,4, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]);
				SetTextValue2Float(current_screen_id,8, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]);
				//SetTextValue2Float(current_screen_id,2, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][2]);
				//SetTextValue2Float(current_screen_id,3, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][3]);
				AnimationPlayFrame(current_screen_id,27, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][4]);    //��ʾ��ѹ��λ
				AnimationPlayFrame(current_screen_id,28, Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][5]);    //��ʾ��ѹ��λ
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
				
				AnimationPlayFrame(current_screen_id,27, Sys_Params.Pressure_Param.Pressure_Unit);    //��ʾ��ѹ��λ
				AnimationPlayFrame(current_screen_id,28, Sys_Params.Pressure_Param.Vacuum_Unit);    //��ʾ��ѹ��λ
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
			//AnimationPlayFrame(current_screen_id,27, Sys_Params.Pressure_Param.Pressure_Unit);    //��ʾ��ѹ��λ
			//AnimationPlayFrame(current_screen_id,28, Sys_Params.Pressure_Param.Vacuum_Unit);    //��ʾ��ѹ��λ
			
			
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
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t state)
{ 
	
	current_screen_id = screen_id;//�ڹ��������п��������л�֪ͨ����¼��ǰ����ID
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
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=Sys_RT_Status.RT_AN_Pressure;       //��������������������
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
					AnimationPlayFrame(_SCREEN_MAIN_2,9, Sys_Params.Dispense_Param.Run_Mode_Params);    //��ʾ�㽺ģʽ
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
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
	//TODO: ����û�����
	int32_t value = 0; 
	float value_double = 0;
	current_screen_id = screen_id;//�ڹ��������п��������л�֪ͨ����¼��ǰ����ID
	
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
			sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ��
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
			
			sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ��
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
			
			sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ��
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
			sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ��
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
				  sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ�� 
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
			sscanf((char *)str,"%d",&value);//���ַ���ת��Ϊ��
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
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
	if(screen_id==5&&control_id==2)//�������
	{
		//test_value = value;

		//SetProgressValue(5,1,test_value); //���½�������ֵ
	}
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8_t status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
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
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//ָ������
	uint8_t ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8_t control_type = msg->control_type;//�ؼ�����
	uint16_t screen_id = PTR2U16(&msg->screen_id);//����ID
	uint16_t control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	uint32_t value = PTR2U32(msg->param);//��ֵ

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://����������
	case NOTIFY_TOUCH_RELEASE://�������ɿ�
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://дFLASH�ɹ�
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://дFLASHʧ��
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://��ȡFLASH�ɹ�
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD://��ȡFLASHʧ��
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
		//Sys_RT_Status.System_RTC.year = cmd_buffer[2];
		//Sys_RT_Status.System_RTC.month = cmd_buffer[3];
		//Sys_RT_Status.System_RTC.day = cmd_buffer[5];
		//Sys_RT_Status.System_RTC.hour = cmd_buffer[6];
		//Sys_RT_Status.System_RTC.minute = cmd_buffer[7];
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://�ı��ؼ�
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //�������ؼ�
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://�˵��ؼ�
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://ѡ��ؼ�
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://����ʱ�ؼ�
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
