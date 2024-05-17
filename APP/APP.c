#include "APP.h" 

#include "shell.h" 
#include "stddef.h"


//#include "Cartridge_Detect.h"
//USBH_HOST  USB_Host;
//USB_OTG_CORE_HANDLE  USB_OTG_Core;
//static uint8_t update_en = 0;                                                          //更新标记
uint8_t timer_tick_last_update = 0;                                                 //上一次更新的时间 
uint8_t StartLoop=0;
uint8_t m_LoadLoopMax=0x05;
uint8_t LoadSuccess=0; 
uint64_t m_Timer_10MS=0;
uint64_t m_Timer_100MS=0;
uint64_t m_Timer_1S=0;
POWER_STATUS m_Pressure_Alm_Status=POWER_OFF; 
POWER_STATUS m_Last_Pressure_Enable_Status=POWER_OFF;
uint8_t m_Get_HMI_RTC_MARK=1;
uint16_t m_Pressure_Building_Timer=0;
const uint16_t m_Pressure_Building_Timer_Max=10;
extern u8_t wfok; 
u8_t intertest = 0; 
float distime_temp = 0;
float intertime_temp = 0;

extern uint8_t mcuidbuffer[25];
u8_t r_com = 0;           //网络设置成功
extern uint8_t settime_flag;

POWER_STATUS m_Last_DI_Pressure_Enable_Status =POWER_OFF;
POWER_STATUS m_Last_DI_Run_Stop_Status =POWER_OFF;
POWER_STATUS m_Last_DI_Dispensing_Status=POWER_OFF;

POWER_STATUS m_Last_Cycle_Manual_Status=POWER_OFF;

POWER_STATUS m_Last_Pro_Manual_Status=POWER_OFF;
uint16_t m_Last_DI_Pressure_Enable_Status_Counter =0;
uint16_t m_Last_DI_Run_Stop_Status_Counter =0; 
uint8_t  m_WIFI_Connect_Mark=0;
float  temping1 = 0,temping2=0;
POWER_STATUS m_Wifi_Search_Success=POWER_OFF;  
uint64_t add_timer = 0;
char m_WIFI_Ecn[5][10]={"OPEN","WEP","WPA","WPA2","WPA_WPA2"};
char testtcp[4]={"test"};
char dispenseflag = 0;
char numflag = 0;
char save_flag=0;
char cycle_flag = 0;
char pro_flag = 0;
//int da=0,db=0,dc=0,dd=0,de=0,df=0,dg=0,dh=0,di=0;
/********************************** 内核对象句柄 *********************************/
// 事件标志组
EventGroupHandle_t Event_Handle =NULL;

// 软件定时器句柄 
static TimerHandle_t Swtmr1_Handle =NULL;  




/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
  
/////////////
//void TIMx_IRQHandler_10MS(void)
//{
	//m_Timer_10MS++;                 //导致死机的原因
//}
void TIM4_IRQHandler_10MS(void)
{ 
	if(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER)
	{
		if(Sys_RT_Status.TEACH == POWER_ON)
		{	
			//Sys_RT_Status.DISPENSING_COUNT_UP = 0;
			if(Sys_RT_Status.SHOT ==POWER_ON||DISPENSING_IO_STATUS() == POWER_ON )
			{
				Sys_RT_Status.DISPENSING_COUNT_UP++;
			}

		}
		else
		{
			Sys_RT_Status.DISPENSING_COUNT_UP = 0;
		}
	}
	
	
	switch(Sys_RT_Status.Dispensing_Step)
	{
		case DISPENSING_STOP:
		{
			switch(Sys_Params.Dispense_Param.Run_Mode_Params)
			{
				case PRO_NO:
					
				case CYCLE:								
				{
					break;					
				}
				
			  case CONTINUE:
				{
					
					break;
				}
				case TIMER:
				{
					//Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
					break;
				}
			}
			break;
		}

		case DISPENSING_STATUS:
		{
			switch(Sys_Params.Dispense_Param.Run_Mode_Params)
			{
				case PRO_NO:
				{
					if(Sys_Params.Dispense_Param.Pro_mode == POWER_OFF)             //PRO的timer模式
					{
							if(Sys_RT_Status.DISPENSING_COUNT_DOWN > 0)
							{
								Sys_RT_Status.DISPENSING_COUNT_DOWN--;
							}
							else
							{
								if(Sys_RT_Status.DISPENSING_COUNT_DOWN == 0)
								{
										Sys_Params.Dispense_Param.shot_num++;
										Sys_Params.Dispense_Param.sys_count++;
								}
								Sys_RT_Status.Dispensing_Step = DISPENSING_STOP;
								Sys_RT_Status.SHOT = POWER_OFF;
								Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0] = distime_temp; 
							}
					}
					else                                                                 //pro的cycle模式
					{
							if(temping1 > 0)
							{
								temping1--;
							}
							else
							{
								if(temping1 == 0)
								{
										Sys_Params.Dispense_Param.shot_num++;
										Sys_Params.Dispense_Param.sys_count++;
								}
								Sys_RT_Status.Dispensing_Step = DISPENSING_INTER;
								temping2 = Sys_RT_Status.Intertime_COUNT_DOWN;
								Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0] = distime_temp;
								
								
								
							}
					}
					break;
				}
					
				case CYCLE:								
				{
					if(temping1 > 0)
					{
						temping1--;
					}
					else
					{
						if(temping1 == 0)
						{
								Sys_Params.Dispense_Param.shot_num++;
								Sys_Params.Dispense_Param.sys_count++;
						}
						Sys_RT_Status.Dispensing_Step = DISPENSING_INTER;
						temping2 = Sys_RT_Status.Intertime_COUNT_DOWN;
						//Sys_Params.Dispense_Param.Dis_Timer = distime_temp*10/1000;
						Sys_Params.Dispense_Param.Dis_Timer = distime_temp;
						
					}
					break;
					
				}
				
			  case CONTINUE:
				{				
					if(Sys_RT_Status.DISPENSING_COUNT_UP==0)
					{						
						Sys_Params.Dispense_Param.shot_num++;
						Sys_Params.Dispense_Param.sys_count++;
					}
					Sys_RT_Status.DISPENSING_COUNT_UP++;
					break;
				}
				case TIMER:
				{
						if(Sys_RT_Status.DISPENSING_COUNT_DOWN > 0)
						{
							Sys_RT_Status.DISPENSING_COUNT_DOWN--;
						}
						else
						{
							if(Sys_RT_Status.DISPENSING_COUNT_DOWN == 0)
							{
									Sys_Params.Dispense_Param.shot_num++;
									Sys_Params.Dispense_Param.sys_count++;
							}
							Sys_RT_Status.Dispensing_Step = DISPENSING_STOP;
							Sys_RT_Status.SHOT = POWER_OFF;
							Sys_Params.Dispense_Param.Dis_Timer = distime_temp; 
						}
						break;
				}
			}
			break;
		}
		case DISPENSING_INTER:
		{
			switch(Sys_Params.Dispense_Param.Run_Mode_Params)
			{
				case PRO_NO:
				{
					if(temping2 > 0)
					{
						temping2--;
					}
					else
					{
						Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
						temping1 = Sys_RT_Status.DISPENSING_COUNT_DOWN;
						//Sys_Params.Dispense_Param.Inter_timer =  intertime_temp*10/1000;
						Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1] =  intertime_temp;
						
					}
					break;
					
				}
					
				case CYCLE:								
				{
					if(temping2 > 0)
					{
						temping2--;
					}
					else
					{
						Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
						temping1 = Sys_RT_Status.DISPENSING_COUNT_DOWN;
						//Sys_Params.Dispense_Param.Inter_timer =  intertime_temp*10/1000;
						Sys_Params.Dispense_Param.Inter_timer =  intertime_temp;
						
					}
					break;
					
				}
				
			  case CONTINUE:break;
				case TIMER:
				{
					//Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
					break;
				}
			}
			break;
		}		
	}
}

//void TIMx_IRQHandler_100MS(void)
//{
	//m_Timer_100MS++;    //导致死机
//}
void TIMx_IRQHandler_1S(void)
{
	m_Timer_1S++;//导致死机
	
	//Sys_RT_Status.System_RTC.second++;
	if(m_Timer_1S>=60)
	{
		m_Timer_1S=0;
	  //Sys_RT_Status.System_RTC.minute++;
		m_Get_HMI_RTC_MARK=1;
		intertest = 1;
		
	} 
	if(m_Timer_1S>=3600)
	{
		save_flag = 1;
	}
		
}
void Dispensing_Logic(void)
{
				if(Sys_RT_Status.System_Option_Mode==STOP_MODE)    //PURGE不能用脚踏开关
				{ 
					if(Sys_RT_Status.PURGE == POWER_ON||CHECK_PURGE_IO_STATUS==POWER_ON)
					{
						DISPENSING_RUN();											
					}
					else
					{
						DISPENSING_STOP();
					}
						

					#if(1)
					Sys_RT_Status.SHOT= POWER_OFF;
					
					Sys_RT_Status.Dispensing_Step=DISPENSING_STOP;	 
					#endif
				}
				else
				{ 

					#if(1)
					{
						 switch(Sys_Params.Dispense_Param.Run_Mode_Params)
						 {
							 case PRO_NO:
							 {
								 if(Sys_Params.Dispense_Param.Pro_mode == POWER_OFF)   //TIMER
								 {
									 switch(Sys_RT_Status.Dispensing_Step)
										{
											case DISPENSING_STOP:
											{
												if((m_Last_DI_Dispensing_Status==POWER_OFF&&DISPENSING_IO_STATUS() == POWER_ON)|| Sys_RT_Status.SHOT ==POWER_ON )
												{
													
													distime_temp = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
													Sys_RT_Status.DISPENSING_COUNT_DOWN=Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]*100.0;
													Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
																									
												}
												DISPENSING_STOP();
												break;
												
											}
											case DISPENSING_STATUS:
											{
												DISPENSING_RUN();					
												Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0] = (float) Sys_RT_Status.DISPENSING_COUNT_DOWN * 10 / 1000;
												break;
												
											}
											default:
												break;																																		
										}								 								 
								 }
								 else
								 {
										switch(Sys_RT_Status.Dispensing_Step)
										{
											case DISPENSING_STOP:
											{
												distime_temp = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0];
												intertime_temp = Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1];

												if(Sys_RT_Status.SHOT ==POWER_ON ||pro_flag == 1)
												{											
													//distime_temp = Sys_Params.Dispense_Param.Dis_Timer;
													//intertime_temp = Sys_Params.Dispense_Param.Inter_timer;
													
													Sys_RT_Status.DISPENSING_COUNT_DOWN=Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0]*100.0;
													temping1 = Sys_RT_Status.DISPENSING_COUNT_DOWN;
													Sys_RT_Status.Intertime_COUNT_DOWN=Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]*100.0;	
													temping2 = Sys_RT_Status.Intertime_COUNT_DOWN;
													Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
													
												}																					
												DISPENSING_STOP();
												break;
											}
											
											case DISPENSING_STATUS:
											{
												
												DISPENSING_RUN();
												Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0] = temping1 * 10 / 1000;													
												break;
											}	
											case DISPENSING_INTER:
											{
												DISPENSING_STOP();
												Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1] = temping2 * 10 / 1000;
												break;
											}	
													
										}	

										if(Sys_RT_Status.SHOT ==POWER_OFF&&pro_flag==0)
										{	
											Sys_RT_Status.Dispensing_Step = DISPENSING_STOP;
											Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][0] = distime_temp;
											Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1] = intertime_temp;

										}
										break;
									 
								 }
								 
								break;								 							 
							 }
							 case CYCLE:
							 {
								switch(Sys_RT_Status.Dispensing_Step)
								{
									case DISPENSING_STOP:
									{
										distime_temp = Sys_Params.Dispense_Param.Dis_Timer;
										intertime_temp = Sys_Params.Dispense_Param.Inter_timer;

										if(Sys_RT_Status.SHOT ==POWER_ON ||cycle_flag == 1)
										{											
											//distime_temp = Sys_Params.Dispense_Param.Dis_Timer;
											//intertime_temp = Sys_Params.Dispense_Param.Inter_timer;
											
											Sys_RT_Status.DISPENSING_COUNT_DOWN=Sys_Params.Dispense_Param.Dis_Timer*100.0;
											temping1 = Sys_RT_Status.DISPENSING_COUNT_DOWN;
											Sys_RT_Status.Intertime_COUNT_DOWN=Sys_Params.Dispense_Param.Inter_timer*100.0;	
											temping2 = Sys_RT_Status.Intertime_COUNT_DOWN;
											Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
											
										}																					
										DISPENSING_STOP();
										break;
									}
									
									case DISPENSING_STATUS:
									{
										
										DISPENSING_RUN();
										Sys_Params.Dispense_Param.Dis_Timer = temping1 * 10 / 1000;													
										break;
									}	
									case DISPENSING_INTER:
									{
										DISPENSING_STOP();
										Sys_Params.Dispense_Param.Inter_timer = temping2 * 10 / 1000;
										break;
									}	
											
								}	

								if(Sys_RT_Status.SHOT ==POWER_OFF&&cycle_flag==0)
								{	
									Sys_RT_Status.Dispensing_Step = DISPENSING_STOP;
									Sys_Params.Dispense_Param.Dis_Timer = distime_temp;
									Sys_Params.Dispense_Param.Inter_timer = intertime_temp;

								}
								break;
								 
							 }
							 case CONTINUE:
							 {
									if(DISPENSING_IO_STATUS() == POWER_ON|| Sys_RT_Status.SHOT ==POWER_ON )
									{
											DISPENSING_RUN();
											Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
											//Sys_RT_Status.DISPENSING_COUNT_UP = 0;
											Sys_Params.Dispense_Param.Dis_Timer = (float)Sys_RT_Status.DISPENSING_COUNT_UP * 10 / 1000;
																						
									}
									else
									{
											Sys_RT_Status.DISPENSING_COUNT_UP = 0;
											Sys_RT_Status.Dispensing_Step = DISPENSING_STOP;
											DISPENSING_STOP();	
									}										
								 break;
							 }
							 case TIMER:
							 {								 								 
								 if(Sys_RT_Status.TEACH == POWER_ON)
								 {
										
										if(Sys_RT_Status.SHOT ==POWER_ON ||DISPENSING_IO_STATUS() == POWER_ON)
										{
											DISPENSING_RUN();
											//Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;
											Sys_Params.Dispense_Param.Dis_Timer = (float)Sys_RT_Status.DISPENSING_COUNT_UP * 10 / 1000;																			
										}
										else
										{
											//Sys_RT_Status.DISPENSING_COUNT_UP = 0;
											//Sys_RT_Status.Dispensing_Step = DISPENSING_STOP;
											DISPENSING_STOP();
											//add_timer =add_timer + Sys_RT_Status.DISPENSING_COUNT_UP;
											//Sys_RT_Status.DISPENSING_COUNT_UP=0;
											//Sys_Params.Dispense_Param.Dis_Timer = add_timer*100.0;											
										}									 
								 }
								 else
								 {
										Sys_RT_Status.DISPENSING_COUNT_UP = 0;
									  switch(Sys_RT_Status.Dispensing_Step)
										{
											case DISPENSING_STOP:
											{
												if((m_Last_DI_Dispensing_Status==POWER_OFF&&DISPENSING_IO_STATUS() == POWER_ON)|| Sys_RT_Status.SHOT ==POWER_ON )
												{
													distime_temp = Sys_Params.Dispense_Param.Dis_Timer;
													Sys_RT_Status.DISPENSING_COUNT_DOWN=Sys_Params.Dispense_Param.Dis_Timer*100.0;
													Sys_RT_Status.Dispensing_Step = DISPENSING_STATUS;																									
												}
												DISPENSING_STOP();
												break;												
											}
											case DISPENSING_STATUS:
											{
												DISPENSING_RUN();
												Sys_Params.Dispense_Param.Dis_Timer = (float)Sys_RT_Status.DISPENSING_COUNT_DOWN * 10 / 1000;												
												break;												
											}
											default:
												break;																																		
										}								 								 								 
									}
									break;
							 }
						 }
					}
					#endif																				
				}
				
				m_Last_DI_Dispensing_Status=DISPENSING_IO_STATUS()||Sys_RT_Status.SHOT;
			
}




static void vINF_Task(void *pvParameters)
{		 
	uint8_t ch = 1; 
//	uint8_t TMP_Getloop=0;   
//	USART3_RX_STA=0;
	while (1)
	{  
		LED_Work(ch);
		ch=1-ch;
		vTaskDelay(100);
  }
}

static void vUI_Task(void *pvParameters)
{
	UI_Init();
	qsize  size = 0;  
	current_screen_id=_SCREEN_LOAD_0; 
	while (1) 
	{ 
		if(StartLoop<=m_LoadLoopMax)
		{
 			StartLoop++;
		}
		
		  size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); 		//从缓冲区中获取一条指令         
		
		 if(StartLoop<m_LoadLoopMax)
		 {
				BELL_Work(1);
				vTaskDelay(100);
				BELL_Work(0);
				vTaskDelay(200); 
				BELL_Work(1);
				vTaskDelay(100);
				BELL_Work(0);
			  		
		}
		else if(StartLoop>=m_LoadLoopMax)
		{

			if(LoadSuccess==0)
			{

				GetRTCTime();
				SetScreen(_SCREEN_MAIN_2); 
				NotifyScreen(_SCREEN_MAIN_2); 
				LoadSuccess=1;
			}
			if(size>0&&cmd_buffer[1]!=0x07)                                              //接收到指令 ，及判断是否为开机提示
			{                                                                           
			    ProcessMessage((PCTRL_MSG)cmd_buffer, size);                             //指令处理  
			}                                                                           
			else if(size>0&&cmd_buffer[1]==0x07)                                         //如果为指令0x07就软重置STM32  
			{                                                                           
			    __disable_fault_irq();                                                   
			    NVIC_SystemReset();                                                                                                                                          
			} 
		}
		else
		{
			current_screen_id=_SCREEN_LOAD_0;
		}
		 
		UpdateUI();  
		vTaskDelay(100);
	}
}

static void vStatus_Task(void *pvParameters)
{ 
		while (1) 
		{
			DHTxx_Read_Data();  
			SetPressureSensorTargetValue(); 
			SetVacuumSensorTargetValue();    //20220920			
			if(Sys_RT_Status.pressure_disable_Mode!=POWER_ON)
			{
				Sys_RT_Status.System_Option_Mode=STOP_MODE;
			}
			if(Sys_RT_Status.System_Option_Mode!=AUTORUN_MODE)
			{
				Sys_RT_Status.Dispensing_Step=DISPENSING_STOP; 
				Sys_RT_Status.SHOT=POWER_OFF;
				Sys_RT_Status.TEACH=POWER_OFF;				
			}
			if(Sys_RT_Status.Dispensing_Step == DISPENSING_STATUS )
			{
				Sys_RT_Status.System_Status = DISPENSING;
				
				
			}
			else
			{
				if(Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)
				{
					Sys_RT_Status.System_Status = READY;
				}
				else
				{
					Sys_RT_Status.System_Status = STOP;
				}
			}
			if(Sys_RT_Status.pressure_disable_Mode==POWER_ON)
			{
				if((Sys_RT_Status.RT_Pressure<=(Sys_Params.Pressure_Param.TargetPreessure+Sys_Params.Pressure_Param.TargetPreessure_Range))&&(Sys_RT_Status.RT_Pressure>=(Sys_Params.Pressure_Param.TargetPreessure-Sys_Params.Pressure_Param.TargetPreessure_Range)))
				{
					m_Pressure_Building_Timer = 0;
					//GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=POWER_OFF;   //20240408
				}
				else
				{
					
					if(m_Pressure_Building_Timer<=(m_Pressure_Building_Timer_Max*5))
					{
						m_Pressure_Building_Timer++;  
					}
					//if(Sys_RT_Status.Dispensing_Step == DISPENSING_STOP)
					//{
					if(m_Pressure_Building_Timer>(m_Pressure_Building_Timer_Max*5))
					{
						Pressure_Display_Enable=POWER_ON;                 //压力报警信号
						//GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=POWER_ON;				//20240408			
						Sys_RT_Status.pressure_disable_Mode=POWER_OFF;
					}
					//}
					
				}
			}
			else
			{
				Sys_RT_Status.System_Option_Mode=STOP_MODE;
				GPIO_OUT[DO_SYS_RUN].GPIO_Value = POWER_OFF;
				m_Pressure_Building_Timer=0; 
				//GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=POWER_OFF;
			}
			if(Sys_RT_Status.pressure_disable_Mode)
			{		
				GPIO_OUT[DO_PRE_ENA].GPIO_Value = POWER_ON;
			}
			else
			{
				GPIO_OUT[DO_PRE_ENA].GPIO_Value = POWER_OFF;
			}
			if(Sys_RT_Status.System_Option_Mode)
			{		
				GPIO_OUT[DO_SYS_RUN].GPIO_Value = POWER_ON;
			}
			else
			{
				GPIO_OUT[DO_SYS_RUN].GPIO_Value = POWER_OFF;
			}
			if(Sys_RT_Status.System_Status==DISPENSING)
			{		
				GPIO_OUT[DO_DISPENSING].GPIO_Value = POWER_ON;
			}
			else
			{
				GPIO_OUT[DO_DISPENSING].GPIO_Value = POWER_OFF;
			}
			if(Sys_RT_Status.Dispensing_Step==DISPENSING_STOP&&Sys_Params.Dispense_Param.Run_Mode_Params==PRO_NO)
			{
				if(Sys_Params.save_pro_buffer[Sys_Params.Dispense_Param.program_number][1]==0)
				{
					Sys_Params.Dispense_Param.Pro_mode = POWER_OFF;
				}
				else
				{
					Sys_Params.Dispense_Param.Pro_mode = POWER_ON;
				}				
			}

		
			
			vTaskDelay(100);
			/*if(r_com==1)
			{
				SaveSystemParams();
				r_com=0;
				
			}*/
			if(save_flag ==1)
			{
				save_flag = 0;
				SaveSystemParams();
				
			}

		}


		
}

void Pointer_stringcat(char *str1,const char *str2)
{

    while (*(str1++)!='\0');     //一直将指向str1的指针移到字符串的末尾
    str1--;
    while (*str2!='\0')
    {
        *(str1++) = *(str2++);  //开始连接
    }
    *str1 = '\0';               //连接完后,添加上字符串结束标识符

}

uint16_t recv_Count = 0;
static void vCommunicationResolving_Task(void *pvParameters)
{

	if (Sys_Params.IOT_Params.Commication_Mode != ETHERNET_MODE && Sys_Params.IOT_Params.Commication_Mode != WIFI_MODE)
	{
		Sys_Params.IOT_Params.Commication_Mode = ETHERNET_MODE;
	}
//	printf("DEBUGCOMunication1\r\n\r\n\r\n");  //yolanda


	while (1)
	{
		taskENTER_CRITICAL(); //锟斤拷锟斤拷锟劫斤拷锟斤拷
		if (Sys_Params.IOT_Params.Commication_Mode != WIFI_MODE || (m_WIFI_Connect_Mark == 1 && Sys_Params.IOT_Params.Commication_Mode == WIFI_MODE))
		{
#if (1) //  锟斤拷锟斤拷息
			if (communication_back_poll() == 1)
			{

				uint8_t _Recv_Finish = 0;

				char *token = strtok(Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8, " ");
				if (token != NULL)
				{
					POWER_STATUS __Read = POWER_ON;
					char Respond[400] = "";
// token = strtok(NULL, " ");
#if (1) /*锟斤拷取状态*/

					if (strcmp(token, "S_STATUS") == 0)
					{
						sprintf((char *)Respond, "{\"S_STATUS\": {\"Run\":%s}}", "true"); //{锟斤拷S_STATUS锟斤拷: {锟斤拷Run锟斤拷: True}}
						_Recv_Finish = 1;
					}


#endif
#if (1) /*锟斤拷锟斤拷状态锟斤拷锟斤拷锟?*/
					/*

					   */

					else if (strcmp(token, "W_RUN_MODE") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{

							if (strcmp(token, "AUTORUN_MODE") == 0)
							{
								Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;
							}
							else
							{
								Sys_RT_Status.System_Option_Mode = STOP_MODE;
							}

							sprintf((char *)Respond, "{\"W_RUN_MODE\": {\"RUN_MODE\":%s}}", Sys_RT_Status.System_Option_Mode == AUTORUN_MODE ? "\"AUTORUN_MODE\"" : "\"STOP_MODE\"");
							_Recv_Finish = 1;
						}
						// sprintf((char *)Respond,"MODE %s",Sys_RT_Status.System_Run_Mode==POWER_ON?"TRUE":"FALSE");
					}
					else if (strcmp(token, "W_DIS_MODE") == 0)
					{
						/*
						   CONTINUE=0,
						   TIMER=1,
						*/
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							if (strcmp(token, "TIMER") == 0)
							{
								Sys_Params.Dispense_Param.Run_Mode_Params = TIMER;
							}
							else if (strcmp(token, "CONTINUE") == 0)
							{
								Sys_Params.Dispense_Param.Run_Mode_Params = CONTINUE;
							}
							else if (strcmp(token, "PRO_NO") == 0)
							{
								Sys_Params.Dispense_Param.Run_Mode_Params = PRO_NO;
							}
							else if (strcmp(token, "CYCLE") == 0)
							{
								Sys_Params.Dispense_Param.Run_Mode_Params = CYCLE;
							}

							//SaveSystemParams();
							r_com = 1;   //成功设置参数
							

							sprintf((char *)Respond, "{\"W_DIS_MODE\": {\"DIS_MODE\":%s}}", Sys_Params.Dispense_Param.Run_Mode_Params == TIMER ? "\"TIMER\"" : Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE?"\"CYCLE\"" :Sys_Params.Dispense_Param.Run_Mode_Params == CONTINUE?"\"CONTINUE\"" :"\"PRO_NO\"");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_TIMER") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						/*
						 */

						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double1 = 0;
							sscanf((char *)token, "%f", &value_double1); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷

							Sys_Params.Dispense_Param.Dis_Timer = value_double1;
							//SaveSystemParams();
							r_com = 1;   //成功设置参数

							sprintf((char *)Respond, "{\"W_TIMER\": {\"DIS_TIME\":%.2f}}", Sys_Params.Dispense_Param.Dis_Timer);
							_Recv_Finish = 1;
						}

					}
					else if (strcmp(token, "W_CYCLE") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						/*
						 */

						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double1 = 0;
							sscanf((char *)token, "%f", &value_double1); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷

							Sys_Params.Dispense_Param.Dis_Timer = value_double1;
							//SaveSystemParams();

							//sprintf((char *)Respond, "{\"W_CYCLE\": {\"DIS_TIME\":%.2f}}", Sys_Params.Dispense_Param.Dis_Timer);
							//_Recv_Finish = 1;
						}
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double2 = 0;
							sscanf((char *)token, "%f", &value_double2); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷

							Sys_Params.Dispense_Param.Inter_timer = value_double2;
							//SaveSystemParams();
;

						}
						sprintf((char *)Respond, "{\"W_CYCLE\": {\"DIS_TIME\":%.2f},{\"INTER_TIME\":%.2f}}", Sys_Params.Dispense_Param.Dis_Timer,Sys_Params.Dispense_Param.Inter_timer);
						_Recv_Finish = 1;
						r_com = 1;   //成功设置参数
					}
					else if (strcmp(token, "W_SET_TIME") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						/*
						 */

						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double11 = 0;
							sscanf((char *)token, "%f", &value_double11); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
							Sys_RT_Status.System_RTC.year = value_double11;
							Sys_RT_Status.System_RTC.year_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.year);
							//Sys_Params.Dispense_Param.Dis_Timer = value_double11;
							//SaveSystemParams();

							//sprintf((char *)Respond, "{\"W_CYCLE\": {\"DIS_TIME\":%.2f}}", Sys_Params.Dispense_Param.Dis_Timer);
							//_Recv_Finish = 1;
						}
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double12 = 0;
							sscanf((char *)token, "%f", &value_double12); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
							Sys_RT_Status.System_RTC.month=value_double12;
							Sys_RT_Status.System_RTC.month_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.month);
							//SaveSystemParams();
;

						}
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double13 = 0;
							sscanf((char *)token, "%f", &value_double13); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
							Sys_RT_Status.System_RTC.day = value_double13;
							Sys_RT_Status.System_RTC.day_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.day);
							//SaveSystemParams();
;

						}
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double14 = 0;
							sscanf((char *)token, "%f", &value_double14); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
							Sys_RT_Status.System_RTC.hour = value_double14;
							Sys_RT_Status.System_RTC.hour_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.hour);
							//SaveSystemParams();
;

						}
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double15 = 0;
							sscanf((char *)token, "%f", &value_double15); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
							Sys_RT_Status.System_RTC.minute = value_double15;
							Sys_RT_Status.System_RTC.minute_BCD = RTC_TEN_TO_BCD(Sys_RT_Status.System_RTC.minute);
							//SaveSystemParams();
;

						}
						settime_flag=1;
						sprintf((char *)Respond, "{\"W_SET_TIME\": {\"SET_TIME\":%d/%d/%d  %d:%d }}",Sys_RT_Status.System_RTC.year,Sys_RT_Status.System_RTC.month,Sys_RT_Status.System_RTC.day,Sys_RT_Status.System_RTC.hour,Sys_RT_Status.System_RTC.minute);
						_Recv_Finish = 1;
						r_com = 1;   //成功设置参数
					}
					else if (strcmp(token, "W_PRO_NO") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						/*
						 */

						token = strtok(NULL, " ");
						if (token != NULL)
						{
							int value_double1 = 0;
							sscanf((char *)token, "%d", &value_double1); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷

							Sys_Params.Dispense_Param.program_number = value_double1;
	
						}
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							float value_double2 = 0;
							sscanf((char *)token, "%f", &value_double2); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷

							Sys_Params.Dispense_Param.Dis_Timer = value_double2;
						}
						if (token != NULL)
						{
							float value_double3 = 0;
							sscanf((char *)token, "%f", &value_double3); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷

							Sys_Params.Dispense_Param.Inter_timer = value_double3;

						}
						//SaveSystemParams();
						sprintf((char *)Respond, "{\"W_PRO_NO\": {\"INTER_TIME\":%d},{\"INTER_TIME\":%.2f},{\"INTER_TIME\":%.2f}}",Sys_Params.Dispense_Param.program_number, Sys_Params.Dispense_Param.Dis_Timer,Sys_Params.Dispense_Param.Inter_timer);
						_Recv_Finish = 1;
						r_com = 1;   //成功设置参数
					}
					else if (strcmp(token, "W_TARGET_PRESSURE") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						token = strtok(NULL, " ");
						if (token != NULL)
						{

							float value_double = 0;
							sscanf((char *)token, "%f", &value_double); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
																		// yolanda 20220208
							if (Sys_Params.Pressure_Param.Pressure_Unit == BAR)
							{
								Sys_Params.Pressure_Param.TargetPreessure = value_double;
							}
							else
							{
								Sys_Params.Pressure_Param.TargetPreessure = (value_double / BAR_PSI_TR);
							}
							// Sys_Params.Pressure_Param.TargetPreessure=value_double;   // replaced by 765-772

							//SaveSystemParams();
							if(Sys_Params.Pressure_Param.Pressure_Unit == BAR)
							{
								sprintf((char *)Respond, "{\"W_TARGET_PRESSURE\": {\"TARGET_PRESSURE\":%.2f}}", Sys_Params.Pressure_Param.TargetPreessure);
							}
							else
							{
								sprintf((char *)Respond, "{\"W_TARGET_PRESSURE\": {\"TARGET_PRESSURE\":%.2f}}", Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
							}
							
							_Recv_Finish = 1;
							r_com = 1;   //成功设置参数
						}
					}
					else if (strcmp(token, "W_PRESSURE_UNIT") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{

							// BAR=0,
							// PSI=1,
							if (strcmp(token, "BAR") == 0)
							{
								Sys_Params.Pressure_Param.Pressure_Unit = BAR;
							}
							else if (strcmp(token, "PSI") == 0)
							{
								Sys_Params.Pressure_Param.Pressure_Unit = PSI;
							}
							//SaveSystemParams();
							r_com = 1;   //成功设置参数
							sprintf((char *)Respond, "{\"W_PRESSURE_UNIT\": {\"PRESSURE_UNIT\":%s}}", Sys_Params.Pressure_Param.Pressure_Unit == BAR ? "\"BAR\"" : "\"PSI\"");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_TARGET_VACUUM") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						token = strtok(NULL, " ");
						if (token != NULL)
						{

							float value_double = 0;
							sscanf((char *)token, "%f", &value_double); //锟斤拷锟街凤拷锟斤拷转锟斤拷为锟斤拷
																		// yolanda 20220208
							if (Sys_Params.Pressure_Param.Vacuum_Unit == VPSI)
							{
								Sys_Params.Pressure_Param.Targetvacuum = value_double;
							}
							else
							{
								Sys_Params.Pressure_Param.Targetvacuum = (value_double / mmHg_PSI_TR);
							}
							// Sys_Params.Pressure_Param.TargetPreessure=value_double;   // replaced by 765-772

							//SaveSystemParams();
							r_com = 1;   //成功设置参数
							if(Sys_Params.Pressure_Param.Vacuum_Unit == VPSI)
							{
								sprintf((char *)Respond, "{\"W_TARGET_VACUUM\": {\"TARGET_VACUUM\":%.2f}}", Sys_Params.Pressure_Param.Targetvacuum);
							}
							else
							{
								sprintf((char *)Respond, "{\"W_TARGET_VACUUM\": {\"TARGET_VACUUM\":%.2f}}", Sys_Params.Pressure_Param.Targetvacuum*mmHg_PSI_TR);
							}
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_VACUUM_UNIT") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{

							// BAR=0,
							// PSI=1,
							if (strcmp(token, "VPSI") == 0)
							{
								Sys_Params.Pressure_Param.Vacuum_Unit = VPSI;
							}
							else if (strcmp(token, "mmHg") == 0)
							{
								Sys_Params.Pressure_Param.Vacuum_Unit = mmHg;
							}
							//SaveSystemParams();
							r_com = 1;   //成功设置参数
							sprintf((char *)Respond, "{\"W_VACUUM_UNIT\": {\"VACUUM_UNIT\":%s}}", Sys_Params.Pressure_Param.Vacuum_Unit == VPSI ? "\"VPSI\"" : "\"mmHg\"");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_PRESSURE_STATUS") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							if (strcmp(token, "POWER_ON") == 0)
							{
								Sys_RT_Status.pressure_disable_Mode = POWER_ON;
							}
							else
							{
								Sys_RT_Status.pressure_disable_Mode = POWER_OFF;
							}
							//sprintf((char *)Respond, "{\"W_PRESSURE_STATUS\": {\"PRESSURE_STATUS\":%s}}", Sys_RT_Status.pressure_disable_Mode == POWER_ON ? "true" : "false");
							sprintf((char *)Respond, "{\"W_PRESSURE_STATUS\": {\"W_PRESSURE_STATUS\":%s}}", Sys_RT_Status.pressure_disable_Mode == POWER_ON ? "\"true\"" :  "\"false\"");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_TEACH") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							if (strcmp(token, "POWER_ON") == 0)
							{
								Sys_RT_Status.TEACH = POWER_ON;
							}
							else
							{
								Sys_RT_Status.TEACH = POWER_OFF;
							}
							//sprintf((char *)Respond, "{\"W_PRESSURE_STATUS\": {\"PRESSURE_STATUS\":%s}}", Sys_RT_Status.pressure_disable_Mode == POWER_ON ? "true" : "false");
							sprintf((char *)Respond, "{\"W_TEACH\": {\"TEACH\":%s}}", Sys_RT_Status.TEACH == POWER_ON ? "\"true\"" :  "\"false\"");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_SHOT") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							if (strcmp(token, "POWER_ON") == 0)
							{
								Sys_RT_Status.SHOT = POWER_ON;
							}
							else
							{
								Sys_RT_Status.SHOT = POWER_OFF;
							}
							//sprintf((char *)Respond, "{\"W_PRESSURE_STATUS\": {\"PRESSURE_STATUS\":%s}}", Sys_RT_Status.pressure_disable_Mode == POWER_ON ? "true" : "false");
							sprintf((char *)Respond, "{\"W_SHOT\": {\"SHOT\":%s}}", Sys_RT_Status.SHOT == POWER_ON ? "\"true\"" :  "\"false\"");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "W_PURGE") == 0)
					{
						token = strtok(NULL, " ");
						if (token != NULL)
						{
							if (strcmp(token, "POWER_ON") == 0)
							{
								Sys_RT_Status.PURGE = POWER_ON;
							}
							else
							{
								Sys_RT_Status.PURGE = POWER_OFF;
							}
							//sprintf((char *)Respond, "{\"W_PRESSURE_STATUS\": {\"PRESSURE_STATUS\":%s}}", Sys_RT_Status.pressure_disable_Mode == POWER_ON ? "true" : "false");
							sprintf((char *)Respond, "{\"W_PURGE\": {\"PURGE\":%s}}", Sys_RT_Status.PURGE == POWER_ON ? "\"true\"" :  "\"false\"");
							_Recv_Finish = 1;
						}
					}
#endif

#if (1) /*锟斤拷取状态锟斤拷锟斤拷*/


					else if (strcmp(token, "R_RUN_MODE") == 0)
					{
						sprintf((char *)Respond, "{\"R_RUN_MODE\": {\"RUN_MODE\":%s}}", Sys_RT_Status.System_Option_Mode == AUTORUN_MODE ? "\"AUTORUN_MODE\"" : "\"STOP_MODE\"");
						_Recv_Finish = 1;
					}
					//sprintf((char *)buffer,"%s",VERSION);		
					else if (strcmp(token, "R_VERSION") == 0)
					{
						sprintf((char *)Respond, "{\"R_VERSION\": {\"VERSION\":%s}}", VERSION);
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_IDH") == 0)
					{
						sprintf((char *)Respond, "{\"R_IDH\": {\"IDH\":%s}}", IDH);
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_PD") == 0)
					{
						sprintf((char *)Respond, "{\"R_PD\": {\"PD\":%s}}", PD);
						_Recv_Finish = 1;
					}
					
					//Sys_Params.Dispense_Param.Run_Mode_Params
					else if (strcmp(token, "R_DIS_MODE") == 0)
					{
						sprintf((char *)Respond, "{\"R_DIS_MODE\": {\"DIS_MODE\":%s}}", Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO ? "\"PRO_NO\"" : Sys_Params.Dispense_Param.Run_Mode_Params == TIMER ? "\"TIMER\"" :Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE?"\"CYCLE\"" :"\"CONTINUE\"");
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_RT_TIME") == 0)
					{
						sprintf((char *)Respond, "{\"R_RT_TIME\": {\"RT_TIME\":%d/%d/%d  %d:%d}}", Sys_RT_Status.System_RTC.year,Sys_RT_Status.System_RTC.month,Sys_RT_Status.System_RTC.day,Sys_RT_Status.System_RTC.hour,Sys_RT_Status.System_RTC.minute);
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_MCUID") == 0)
					{
						sprintf((char *)Respond, "{\"R_MCUID\": {\"MCUID\":\"%s\"}}", mcuidbuffer);
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_PRO_NO") == 0)
					{
						if(Sys_Params.Dispense_Param.Run_Mode_Params == PRO_NO)
						{
							sprintf((char *)Respond, "{\"R_PRO_NO\": {\"PRO_NO\":%d,\"DIS_TIME\":%.2f,\"INTER_TIME\":%.2f}}", Sys_Params.Dispense_Param.program_number,Sys_Params.Dispense_Param.Dis_Timer,Sys_Params.Dispense_Param.Inter_timer);
							_Recv_Finish = 1;
						}
						else
						{
							sprintf((char *)Respond, "{\"ERROR\"}");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "R_TIMER") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						/*
						 */
						if(Sys_Params.Dispense_Param.Run_Mode_Params == TIMER)
						{
							sprintf((char *)Respond, "{\"R_TIMER\": {\"DIS_TIME\":%.2f}}", Sys_Params.Dispense_Param.Dis_Timer);
							_Recv_Finish = 1;
						}
						else
						{
							sprintf((char *)Respond, "{\"ERROR\"}");
							_Recv_Finish = 1;
						}
					}
					else if (strcmp(token, "R_CYCLE") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						/*
						 */
						if(Sys_Params.Dispense_Param.Run_Mode_Params == CYCLE)
						{
							sprintf((char *)Respond, "{\"R_CYCLE\": {\"DIS_TIME\":%.2f,\"INTER_TIME\":%.2f}}", Sys_Params.Dispense_Param.Dis_Timer,Sys_Params.Dispense_Param.Inter_timer);
							_Recv_Finish = 1;
						}
						else
						{
							sprintf((char *)Respond, "{\"ERROR\"}");
							_Recv_Finish = 1;
						}
					}

					else if (strcmp(token, "R_TARGET_PRESSURE") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						if(Sys_Params.Pressure_Param.Pressure_Unit == BAR)
						{
							sprintf((char *)Respond, "{\"R_TARGET_PRESSURE\": {\"TARGET_PRESSURE\":%.2f}}", Sys_Params.Pressure_Param.TargetPreessure);
						}
						else
						{
							sprintf((char *)Respond, "{\"R_TARGET_PRESSURE\": {\"TARGET_PRESSURE\":%.2f}}", Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
						}
						_Recv_Finish = 1;
					}

					else if (strcmp(token, "R_PRESSURE_UNIT") == 0)
					{
						sprintf((char *)Respond, "{\"R_PRESSURE_UNIT\": {\"R_PRESSURE_UNIT\":%s}}", Sys_Params.Pressure_Param.Pressure_Unit == BAR ? "\"BAR\"" : "\"PSI\"");
						_Recv_Finish = 1;
					}
					//Sys_Params.Pressure_Param.Vacuum_Unit
					else if (strcmp(token, "R_TARGET_VACUUM") == 0)
					{
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						if(Sys_Params.Pressure_Param.Vacuum_Unit == VPSI)
						{
							sprintf((char *)Respond, "{\"R_TARGET_VACUUM\": {\"TARGET_VACUUM\":%.2f}}", Sys_Params.Pressure_Param.Targetvacuum);
						}
						else
						{
							sprintf((char *)Respond, "{\"R_TARGET_VACUUM\": {\"TARGET_VACUUM\":%.2f}}", Sys_Params.Pressure_Param.Targetvacuum*mmHg_PSI_TR);
						}
						_Recv_Finish = 1;
					}
					//	VPSI=0,mmHg=1,

					else if (strcmp(token, "R_VACUUM_UNIT") == 0)
					{
						sprintf((char *)Respond, "{\"R_VACUUM_UNIT\": {\"VACUUM_UNIT\":%s}}", Sys_Params.Pressure_Param.Vacuum_Unit == VPSI ? "\"VPSI\"" : "\"mmHg\"");
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_TEM_HUM") == 0)
					{
						sprintf((char *)Respond, "{\"R_TEM_HUM\": {\"TEM\":%f,\"Hum\":%f}}", Sys_RT_Status.Temperature_RT, Sys_RT_Status.Humidity_RT);
						_Recv_Finish = 1;
					}
					//						Sys_RT_Status.PURGE=POWER_ON;Sys_Params.Dispense_Param.shot_num++;Sys_Params.Dispense_Param.sys_count++;
					else if (strcmp(token, "R_SHOT_NUM") == 0)
					{
						sprintf((char *)Respond, "{\"R_SHOT_NUM\": {\"SHOT_NUM\":%d}}",Sys_Params.Dispense_Param.shot_num);
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_SYS_COUNT") == 0)
					{
						sprintf((char *)Respond, "{\"R_SYS_COUNT\": {\"SYS_COUNT\":%d}}",Sys_Params.Dispense_Param.sys_count);
						_Recv_Finish = 1;
					}
//Sys_RT_Status.SUPERAD = POWER_ON;	Sys_RT_Status.TEACH = POWER_ON;Sys_RT_Status.SHOT = POWER_ON;Sys_RT_Status.PURGE=POWER_OFF;
					else if (strcmp(token, "R_SUPERAD") == 0)
					{
						sprintf((char *)Respond, "{\"R_SUPERAD\": {\"SUPERAD\":%s}}",Sys_RT_Status.SUPERAD== POWER_ON ? "\"true\"" :  "\"false\"");
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_TEACH") == 0)
					{
						sprintf((char *)Respond, "{\"R_TEACH\": {\"TEACH\":%s}}",Sys_RT_Status.TEACH== POWER_ON ? "\"true\"" :  "\"false\"");
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_SHOT") == 0)
					{
						sprintf((char *)Respond, "{\"R_SHOT\": {\"SHOT\":%s}}",Sys_RT_Status.SHOT == POWER_ON ? "\"true\"" :  "\"false\"");
						_Recv_Finish = 1;
					}
					else if (strcmp(token, "R_PURGE") == 0)
					{
						sprintf((char *)Respond, "{\"R_PURGE\": {\"PURGE\":%s}}",Sys_RT_Status.PURGE== POWER_ON ? "\"true\"" :  "\"false\"");
						_Recv_Finish = 1;
					}

					else if (strcmp(token, "R_PRESSURE_STATUS") == 0)
					{

						sprintf((char *)Respond, "{\"R_PRESSURE_STATUS\": {\"R_PRESSURE_STATUS\":%s}}", Sys_RT_Status.pressure_disable_Mode == POWER_ON ? "\"true\"" :  "\"false\"");
						// sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);
						_Recv_Finish = 1;
					}

#endif

					*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l = 0;
					memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
									  
					if(__Read==POWER_ON && strlen(Respond)>0)
					{  
						strcpy( Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, Respond);
						*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =strlen(Respond);
						communication_send(); 
					} 
				}


				

				*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l = 0;
				memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
			}
#endif
#if (1) //锟斤拷锟斤拷息
			if (*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l == 0)
			{
				// communication_send();
			}
#endif
		}
		taskEXIT_CRITICAL(); //锟剿筹拷锟劫斤拷锟斤拷
							 //			puts("1231");
		// vTaskDelay(500);
		vTaskDelay(20); // YOLANDA 20220421
	}
	
}

static void vGPIO_Task(void *pvParameters)
{  
	
		vTaskDelay(5000);
		DO_PRESSURE_SENSOR_POWER_ON;//YOLANDA
		DO_VACUUM_SENSOR_POWER_ON;
		vTaskDelay(1000);
		Enable_PressureSensor_Auto_Detect();
		Enable_VacuumSensor_Auto_Detect();
	
		while (1) 
		{  
				Refresh_GPIO_Value();
			//PLC输入pressureenable
			  if(CHECK_PRESSURE_ENABLE==POWER_ON && m_Last_DI_Pressure_Enable_Status==POWER_OFF&& m_Last_DI_Pressure_Enable_Status_Counter<=10)   
				{
					m_Last_DI_Pressure_Enable_Status_Counter++;
					if(m_Last_DI_Pressure_Enable_Status_Counter>=8)
					{
						if(Sys_RT_Status.pressure_disable_Mode!=POWER_ON)
						{
							Sys_RT_Status.pressure_disable_Mode=POWER_ON;
						}
						else
						{ 
							Sys_RT_Status.pressure_disable_Mode=POWER_OFF;
							
						}
						
						m_Last_DI_Pressure_Enable_Status=CHECK_PRESSURE_ENABLE;
						m_Last_DI_Pressure_Enable_Status_Counter=0;
					}
				}
				else
				{
					m_Last_DI_Pressure_Enable_Status=CHECK_PRESSURE_ENABLE;
					m_Last_DI_Pressure_Enable_Status_Counter=0;
				}
			//PLC输入run stop
			  if(CHECK_RUN_STOP==POWER_ON && m_Last_DI_Run_Stop_Status==POWER_OFF && m_Last_DI_Run_Stop_Status_Counter<=10)
				{
					m_Last_DI_Run_Stop_Status_Counter++;
					if(m_Last_DI_Run_Stop_Status_Counter>=8)
					{
						if(Sys_RT_Status.System_Option_Mode != AUTORUN_MODE)
						{
							Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;

						}
						else
						{
							Sys_RT_Status.System_Option_Mode = STOP_MODE; 
						}
					m_Last_DI_Run_Stop_Status=CHECK_RUN_STOP;
					m_Last_DI_Run_Stop_Status_Counter=0;
					}
				}
				else
				{
					m_Last_DI_Run_Stop_Status=CHECK_RUN_STOP;
					m_Last_DI_Run_Stop_Status_Counter=0;
				}
				if(Sys_Params.Dispense_Param.Run_Mode_Params==CYCLE)
				{
					if(DISPENSING_IO_STATUS()==POWER_ON&&m_Last_Cycle_Manual_Status==POWER_OFF)
					{	
						vTaskDelay(10);

						if(DISPENSING_IO_STATUS()==POWER_ON)
						{
							cycle_flag = !cycle_flag;													
						}										
					}
					m_Last_Cycle_Manual_Status=DISPENSING_IO_STATUS();
				}
				if(Sys_Params.Dispense_Param.Run_Mode_Params==PRO_NO&&Sys_Params.Dispense_Param.Pro_mode == POWER_ON)
				{
					if(DISPENSING_IO_STATUS()==POWER_ON&&m_Last_Pro_Manual_Status==POWER_OFF)
					{	
						vTaskDelay(10);

						if(DISPENSING_IO_STATUS()==POWER_ON)
						{
							pro_flag = !pro_flag;													
						}										
					}
					m_Last_Pro_Manual_Status=DISPENSING_IO_STATUS();
				}																																
			  vTaskDelay(1);
			
		}
}
static void vDispensing_Task(void *pvParameters)
{  
		while (1) 
		{   
			Dispensing_Logic();
			vTaskDelay(4);
		}
}
   
static void vWifi_Bluetooth_Recive_Task(void *pvParameters)
{
	
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{ 
			break ;
		}
		case ETHERNET_MODE:
		{ 
			 
			return;
		}
		default:break;
	}
	
	WIFI_BLUETOOTH_DMA_UART *p_Struct_DMA_Uart=&Wifi_Bluetooth_DMA_Uart;
	//Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer = Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
	EventBits_t r_event;  /* 定义一个事件接收变量 */
	while(1)
	{
		switch(Sys_Params.IOT_Params.Commication_Mode)
		 { 
			case WIFI_MODE:
			{
				#if 1
				r_event = xEventGroupWaitBits(Event_Handle,  /* 事件对象句柄 */
																			 (1)<<0,/* 接收线程感兴趣的事件 */
																				pdTRUE,   /* 退出时清除事件位 */
																				pdTRUE,   /* 满足感兴趣的所有事件 */
																				portMAX_DELAY);/* 指定超时事件,一直等 */
				
				switch(r_event&((1)<<0))
				{
					case 1:
							{
								p_Struct_DMA_Uart->Dma_R_Flag=0;
								switch (Sys_Params.IOT_Params.Commication_Mode)
								{
									case WIFI_MODE://WiFi
									{
										puts("--------->\r");
										puts((char*)p_Struct_DMA_Uart->Rxd_Fifo);
									
										//预防数据越界
										if( IOT_SENDANDRECEIVE_BUFFER_LEN< Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len+p_Struct_DMA_Uart->Rxd_L)
										{
											
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len=0;
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
										}
										if(m_WIFI_Connect_Mark==1)
										{
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
										}
										memcpy(Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer,
															 p_Struct_DMA_Uart->Rxd_Fifo,
															 p_Struct_DMA_Uart->Rxd_L);
										
										Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len +=p_Struct_DMA_Uart->Rxd_L;		
										Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer+=p_Struct_DMA_Uart->Rxd_L;
										memset( p_Struct_DMA_Uart->Rxd_Fifo,0, IOT_SENDANDRECEIVE_BUFFER_LEN );
									}
										break;
									
									default:
										break;
								}
							}
						break;
					case 0:
						Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step = 0;
						break;
						
						default :
							break;
				}
				#endif
				break;
			}
			case ETHERNET_MODE:
			{
				
				break;
			}
			default:break;
		 }
	}
}
static void vWifi_Bluetooth_Task(void *pvParameters)
{
	
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{ 
			Wifi_Init();
			break;
		}
		case ETHERNET_MODE:
		{ 
			return;
		}
		default: break;
	}
	
		
	
	#if 1 ///WIFI_MODE
	char *p=NULL;
	uint16_t i=0;
	Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step =0;
	#endif
	
	#if 1 //BLUETOOTH_MODE 
	EventBits_t r_event;  /* 定义一个事件接收变量 */ 
	POWER_STATUS __LoadFirst=POWER_OFF;
	#endif
	
	vTaskDelay(1000);
	while(1)
	{ 		
     switch(Sys_Params.IOT_Params.Commication_Mode)
		 {
			case WIFI_MODE:
			{
				
					#if 1
			//		puts( "vWifi_Task" );
					switch(Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step)
					{
						case 0://连接wifi
						{
			//				AT+CWJAP_DEF=“newifi_F8A0”,“anxinke123” !!连接网络
							WIFI_SEND("+++");
							vTaskDelay(2000);
							memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWMODE=1\r\n" );
							
							# if(0) //Set host Station name
							strcat(Struct_Wifi0.array_wifi_send, "AT+CWHOSTNAME=\"" );
							strcat(Struct_Wifi0.array_wifi_send, "001002003004" );
							strcat(Struct_Wifi0.array_wifi_send, "\"\r\n" ); 
							
							#endif
							
							WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
							vTaskDelay(2000);
							
							
							#if (1) // 强制更改 WIFI 连接名称
							
							//memset(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name,0,20);
							//sprintf((char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name,"%s","Rody_PLC_LINK");	
							
							#endif
							
							memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0,  IOT_SENDANDRECEIVE_BUFFER_LEN);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWJAP_DEF=\"" );  
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\",\"" );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer,  Sys_Params.IOT_Params.WIFI_Params.Wlan_Password);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\"\r\n" );
							WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "WIFI CONNECTED");
							p =strstr(p, "WIFI GOT IP");
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							/*if( p!=NULL )
							{
								puts("连接成功\r\n");
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							}
							else
							{
								p=strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "OK");
								if( p!=NULL )
								{
									puts("连接成功\r\n");
									Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								}
									puts("连接失败\r\n");
							}*/
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 1://关闭使能上电自动连接AP
						{
							WIFI_SEND("AT+CWAUTOCONN=0\r\n");
							vTaskDelay(5000);
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							//检测返回值
			//				p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CWAUTOCONN");
			//				p =strstr(p, "OK");
			//				if( p!=NULL )
			//				{
			//					Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
			//					puts("AT+CWAUTOCONN 连接成功\r\n");
			//				}
			//				else
			//				{
			//						puts("AT+CWAUTOCONN 连接失败\r\n");
			//				}
							
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 2://连接连接服务器
						{
							char array_port[8];
							memset(array_port,0,8);
							sprintf(array_port,"%d",Sys_Params.IOT_Params.Server_Params.Sever_Port);
							//AT+CIPSTART=“TCP”,“192.168.99.217”,6001 　　!!连接TCP服务器,本实验用网络调试助手进行测试
							memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CIPSTART=\"TCP\",\"" );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, Sys_Params.IOT_Params.Server_Params.Server_IPAddress);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\"," );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, array_port );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\r\n" );
							
							WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CIPSTART");
							p =strstr(p, "CONNECT");
							if( p!=NULL )
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								puts("AT+CIPSTART=\"TCP\" 连接成功\r\n");
							}
							else
							{
									puts("AT+CIPSTART=\"TCP\" 连接失败\r\n");
							}
							//检测返回值
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0,IOT_SENDANDRECEIVE_BUFFER_LEN);
						}break;
						case 3://使用透传模式发送数据
						{
							WIFI_SEND("AT+CIPMODE=1\r\n");
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CIPMODE=1");
							p =strstr(p, "OK");
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							/*
							if( p!=NULL )
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								puts("AT+CIPMODE=1 连接成功\r\n");
							}
							else
							{
									puts("AT+CIPMODE=1 连接失败\r\n");
							}
							*/
						
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 4://使用透传模式发送数据 发送数据
						{
							WIFI_SEND("AT+CIPSEND\r\n");
							vTaskDelay(5000);
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							/*
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CIPSEND");
							p =strstr(p, "OK");
							if( p!=NULL )
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								puts("AT+CIPSEND 连接成功\r\n");
							}
							else
							{
									puts("AT+CIPSEND 连接失败\r\n");
							}
							*/
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 5://发送数据
						{
							vTaskDelay(5000);
							WIFI_SEND("Ready\r\n");
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step=8;
						}
							break;
						
						case 6://退出透传模式
						{
							vTaskDelay(1000);
							WIFI_SEND("+++");
							vTaskDelay(2000);
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;				
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 7://断开服务器 
						{
							WIFI_SEND("AT+CIPCLOSE\r\n");
							vTaskDelay(2000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "CLOSED");
							p =strstr(p, "OK");
							if( p!=NULL )
							{
								puts("AT+CIPCLOSE 连接成功\r\n");
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;	
							}
							else
							{
									puts("AT+CIPCLOSE连接失败\r\n");
							}
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 8:
						{
							m_WIFI_Connect_Mark=1;
							for(int ys=0;ys<10;ys++)
							{
							  vTaskDelay(10000);
							}
							WIFI_SEND("test\r\n");
							vTaskDelay(1000);
							
							if(wfok == 1)
							{
								m_WIFI_Connect_Mark=1;
								wfok = 0;
							}
							else
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step = 0;
							}
				
							
						}
						break;
						case 10://版本信息
						{
							WIFI_SEND("AT+GMR\r\n");
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+GMR");
							p =strstr(p, "OK");
							if( p!=NULL )
							{
								puts("--->ok\r\n");
							}
							else
							{
								puts("--->err\r\n");
							}
							puts("\r\n--->\r\n");
							puts( Sys_RT_Status.RT_IOT_Status.Recive_Buffer );
							
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 20:
						{
							/*
							AT+CWLAP 列出当前可用AP
							执行指令 AT+CWLAP
							响应 终端返回AP列表+ CWLAP: <ecn>,<ssid>,<rssi> OK ERROR
							参数说明 
							< ecn >
							0 OPEN 
							1 WEP 
							2 WPA_PSK 
							3 WPA2_PSK 
							4 WPA_WPA2_PSK 
							<ssid> 字符串参数，接入点名称
							<rssi> 信号强度 
							*/
							if(m_Wifi_Search_Success==POWER_OFF)
							{
								m_WIFI_Connect_Mark=0;
								
								WIFI_SEND("+++");
								vTaskDelay(2000);
								memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
								strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWMODE=1\r\n" );
								WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
								vTaskDelay(1000);
								
								strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWLAP\r\n" );
								WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
								vTaskDelay(5000);
								m_Wifi_Search_Success=POWER_ON; 
								
							}
							break;
						}
						default:
			//				Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step=100;
							break;
					}
					#endif
				break;
			}
			case ETHERNET_MODE:
			{
				
				break;
			}
			default:break;
		 }
	}
}
 


static void vEthernet_Task(void *pvParameters)
{
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{ 
			return;
		}
		case ETHERNET_MODE:
		{ 
			
		  lwip_comm_init();
			break;
		}
		default:break;
	}
	#if 1
  err_t err,recv_err;
	u32 data_len = 0; 
	
  for(;;)
  {
		lable0:
		#if 1
		//puts("udp_thread_task link...\r\n");
		vTaskDelay(2000);
		//创建一个TCP链接
		Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn = netconn_new(NETCONN_TCP);  
		//连接服务器
		err = netconn_connect(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn,
												 &Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Server_IPAddr,
												  Sys_Params.IOT_Params.Server_Params.Sever_Port);
		//返回值不等于ERR_OK,删除tcp_clientconn连接
		if(err != ERR_OK)  netconn_delete(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn);
			//处理新连接的数据		
		else if (err == ERR_OK)   
		{ 
			struct netbuf *recvbuf;
			Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn->recv_timeout = 10;
			//获取本地IP主机IP地址和端口号
			netconn_getaddr(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn,
									   &Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Loca_IPAddr,
										 &Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Ethernet_Loca_Port,1); 
			//printf("连接上服务器%d.%d.%d.%d,本机端口号为:%d\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3],Struct_Tcp_Data0.loca_port);
			while(1)
			{
					if(intertest ==1)
					{
						intertest = 0 ;
						err = netconn_write(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn ,
																	testtcp,
																	4,
																	NETCONN_COPY); //发送tcp_server_sendbuf中的数据
						if(err != ERR_OK)
						{
							//printf("发送失败\r\n");
							goto lable0;
						}
						
					}
					if( Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l != 0&&Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_flag !=0) //有数据要发送
					{
            if( Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l >IOT_SENDANDRECEIVE_BUFFER_LEN )
              Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l =IOT_SENDANDRECEIVE_BUFFER_LEN;
						err = netconn_write(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn ,
                                Sys_RT_Status.RT_IOT_Status.Send_Buffer,
                                Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l,
                                NETCONN_COPY); //发送tcp_server_sendbuf中的数据
						if(err != ERR_OK)
						{
							//printf("发送失败\r\n");
							goto lable0;
						}
            Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l =0;
						Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_flag =0;
					}
				
					//接收到数据
					recv_err = netconn_recv(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn,&recvbuf);
					switch(recv_err)
					{
						case ERR_OK:
						{
							//数据接收缓冲区清零
							memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0,IOT_SENDANDRECEIVE_BUFFER_LEN);  
							taskENTER_CRITICAL(); 
							for(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q=recvbuf->p; 
							Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q!=NULL;
							Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q=Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->next)  //遍历完整个pbuf链表
							{
								//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
								//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
								if(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->len > (IOT_SENDANDRECEIVE_BUFFER_LEN-data_len)) 
								{
									memcpy(Sys_RT_Status.RT_IOT_Status.Recive_Buffer+data_len,
												 Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->payload,
												(IOT_SENDANDRECEIVE_BUFFER_LEN-data_len));
								}
								else
								{
										memcpy(Sys_RT_Status.RT_IOT_Status.Recive_Buffer+data_len,
													 Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->payload,
													 Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->len);
								}						
								data_len += Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->len;  	
								if(data_len > IOT_SENDANDRECEIVE_BUFFER_LEN) break; //超出TCP客户端接收数组,跳出	
							}
							taskEXIT_CRITICAL();
							
              Sys_RT_Status.RT_IOT_Status.Ethernet_Status.r_l = data_len;
							netbuf_delete(recvbuf);
              //printf("%s\r\n",Struct_Tcp_Data0.tcp_client_recvbuf);
              
              data_len=0;  //复制完成后data_len要清零。
						}
							break;
						case ERR_CLSD:
						{
							netconn_close(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn);
							netconn_delete(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn);
							//printf("服务器%d.%d.%d.%d断开连接\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
							goto lable0;
						}
							break;
					}
			 
			}
		}
    #endif
  }
  #endif
	/* USER CODE END StartDefaultTask */
}
/*static void vWatchdog_Task(void *pvParameters)
{
	//IWDG_Init(4,500);
	while(1)
	{ 
		IWDG_Feed( );
		vTaskDelay(1);
	}
	
}*/
void AppTaskCreate(void)
{
		taskENTER_CRITICAL();           //进入临界区	
		//									
		xTaskCreate(   (TaskFunction_t )vGPIO_Task,         			/* 任务函数  */
								 (const char*    )"vGPIO_Task",        				/* 任务名    */
								 (uint16_t       )256,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )30,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */			
 
		xTaskCreate(   (TaskFunction_t )vUI_Task,          				/* 任务函数  */
								 (const char*    )"vUI_Task",        					/* 任务名    */
								 (uint16_t       )512,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )29,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */		

		xTaskCreate(   (TaskFunction_t )vINF_Task,          			/* 任务函数  */
								 (const char*    )"vINF_Task",        				/* 任务名    */
								 (uint16_t       )256,              	 							/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )28,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */	
									
		xTaskCreate(   (TaskFunction_t )vStatus_Task,         		/* 任务函数  */
								 (const char*    )"vStatus_Task",        			/* 任务名    */
								 (uint16_t       )128,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )27,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */		
			
									
		xTaskCreate(   (TaskFunction_t )vCommunicationResolving_Task,         			/* 任务函数  */
								 (const char*    )"vCommunicationResolving_Task",        				/* 任务名    */
								 (uint16_t       )256,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )25,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */		
			

		//									
		xTaskCreate(   (TaskFunction_t )vDispensing_Task,         			/* 任务函数  */
								 (const char*    )"vDispensing_Task",        				/* 任务名    */
								 (uint16_t       )128,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )23,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */									 

		switch(Sys_Params.IOT_Params.Commication_Mode)
		{
			case WIFI_MODE:
			{ 
				xTaskCreate(   (TaskFunction_t )vWifi_Bluetooth_Recive_Task,       /* 任务函数  */
											 (const char*    )"vWifi_Bluetooth_Recive_Task",        			/* 任务名    */
											 (uint16_t       )256,               				/* 任务栈大小，单位word，也就是4字节 */
											 (void*          )NULL,   																																														            			/* 任务参数  */
											 (UBaseType_t    )22,                  			/* 任务优先级*/
											 (TaskHandle_t*  )NULL );             			/* 任务句柄  */	
				xTaskCreate(   (TaskFunction_t )vWifi_Bluetooth_Task,         			/* 任务函数  */
											 (const char*    )"vWifi_Bluetooth_Task",        			/* 任务名    */
											 (uint16_t       )256,               				/* 任务栈大小，单位word，也就是4字节 */
											 (void*          )NULL,               			/* 任务参数  */
											 (UBaseType_t    )21,                  			/* 任务优先级*/
											 (TaskHandle_t*  )NULL );             			/* 任务句柄  */												 
											 	
				break;
			}
			case ETHERNET_MODE:
			{  
								 
				xTaskCreate(   (TaskFunction_t )vEthernet_Task,         			/* 任务函数  */
											 (const char*    )"vEthernet_Task",        			/* 任务名    */
											 (uint16_t       )256,               				/* 任务栈大小，单位word，也就是4字节 */
											 (void*          )NULL,               			/* 任务参数  */
											 (UBaseType_t    )22,                  			/* 任务优先级*/
											 (TaskHandle_t*)NULL );//创建UDP任务	
				break;
			}
			default:break;
		}
		
 
	taskEXIT_CRITICAL();            //退出临界区
}
static SemaphoreHandle_t  xMutex = NULL;
void AppObjCreate (void)
{
	 /* 创建 Event_Handle */
  Event_Handle = xEventGroupCreate();	
	/* 创建互斥信号量 */
  xMutex = xSemaphoreCreateMutex();
	
	if(xMutex == NULL)
  {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
  }
}
 
