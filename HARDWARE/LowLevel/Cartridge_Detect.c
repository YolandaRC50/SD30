#include "Cartridge_Detect.h"
double ADV_Online_Value_Buffer[128];
int Step_Loop_Num=0;

/*�������*/
#if(1==1)
void Clean_Online_Value_Buffer()
{
	uint16_t loopIndex=0;
	
	 for(loopIndex=0;loopIndex<STEP_JUMP_CONDITION_NUM;loopIndex++)
		ADV_Online_Value_Buffer[loopIndex]=0;
}
#endif
void Cartridge_DetectSystem_Working(void)
{ 
	if(READ_EXHAUST_SWITCH_STATUS())
	{
			VALVE_PRODUCE_SET();
			SSR_A_RESET();
			SSR_B_SET();
			SSR_C_SET();
	}
	else
	{ 
		if(Cartridge_Detect_Data.Auto_Detect_Switch==POWER_ON &&  DIspensingFinished_Mark==POWER_ON)
		{
			
			DIspensingFinished_Mark=POWER_OFF;
			Cartridge_Detect_Data.Dispensing_Finished_Counter--;
			if( Cartridge_Detect_Data.Dispensing_Finished_Counter<=0)
			{
						Cartridge_Detect_Data.Dispensing_Finished_Counter=Cartridge_Detect_Data.Auto_Detect_Counter;
				/*���Կ�ʼ*/ 
						Cartridge_Detect_Data.Cartridge_Main_Status=CARTRIDGE_STATUS_CARTRIDGE_DETECT;
						Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_DIS; 
				/*���Կ�ʼ*/ 
			}
		}
							
		switch(Cartridge_Detect_Data.Cartridge_Main_Status)
		{
			case CARTRIDGE_STATUS_PRODUCTION:
			{
				if(Cartridge_Detect_Data.CARTRIDGE_DEBUG_POWER==POWER_OFF)
					{
						Cartridge_Detect_Data.Cartridge_Main_Status=CARTRIDGE_STATUS_PRODUCTION;
						DetectSystem_Productting();
					}
					else
					{
						Cartridge_Detect_Data.Cartridge_Main_Status=CARTRIDGE_STATUS_DEBUG;
						Cartridge_Debugging();
					}
				
				break;
			} 
			case CARTRIDGE_STATUS_DEBUG:
			{
				Cartridge_Debugging();
				break;
			}
			case CARTRIDGE_STATUS_EMPTY_CALIBRATION:
			case CARTRIDGE_STATUS_FULL_CALIBRATION: 
			case CARTRIDGE_STATUS_CARTRIDGE_DETECT:
			{
				DetectSystem_Detectting();
				break;
			}
		}
	}
}

void Detect_Air_Guage_Data(void)
{
	float RT_A=Cartridge_Detect_Data.Level_Detect_Guage_Data_AN_AVG;//ADC_READ();
	Cartridge_Detect_Data.Last_RT_Pressure_Controller_Data_AN=Cartridge_Detect_Data.RT_Pressure_Controller_Data_AN;
	Cartridge_Detect_Data.RT_Pressure_Controller_Data_AN=RT_A;
	
	Cartridge_Detect_Data.Last_Pressure_Controller_Data=Cartridge_Detect_Data.RT_Pressure_Controller_Data;
	
	 
	
	for(int index__ =STEP_JUMP_CONDITION_NUM-1;index__>0;index__--)
	{
		 ADV_Online_Value_Buffer[index__]=ADV_Online_Value_Buffer[index__-1];
	}
	ADV_Online_Value_Buffer[0]=(double)Cartridge_Detect_Data.RT_Pressure_Controller_Data_AN;

 
}
/*����ֶ���������*/
void Manual_Exhaust_Switch(void)
{
	if(READ_EXHAUST_SWITCH_STATUS())
	{
		
	}
	else
	{
		
	}
}
void DetectSystem_Productting(void)
{
	VALVE_PRODUCE_RESET();
	SSR_A_RESET();
	SSR_B_RESET();
	SSR_C_RESET();
	
}
void Cartridge_Debugging(void)
{
	Cartridge_Detect_Data.Valve_Status_Set.Switch_Status==POWER_ON?VALVE_PRODUCE_SET():VALVE_PRODUCE_RESET();
	Cartridge_Detect_Data.Valve_Status_Set.Storage_Status==POWER_ON?SSR_A_SET():SSR_A_RESET();
	Cartridge_Detect_Data.Valve_Status_Set.Exhaust_Status==POWER_ON?SSR_B_SET():SSR_B_RESET();
	Cartridge_Detect_Data.Valve_Status_Set.Syringe_Status==POWER_ON?SSR_C_SET():SSR_C_RESET();
}
void DetectSystem_Detectting(void)
{
	switch(Cartridge_Detect_Data.Cartridge_Sub_Status)
	{
			/*��:Ĭ�϶���*/
			case AIR_NONE:
			{ 
				if(Cartridge_Detect_Data.CARTRIDGE_DEBUG_POWER==POWER_OFF)
				{
					Cartridge_Detect_Data.Cartridge_Main_Status=CARTRIDGE_STATUS_PRODUCTION;
				}
				else
				{
					Cartridge_Detect_Data.Cartridge_Main_Status=CARTRIDGE_STATUS_DEBUG;
				}
				break;
			}
			/*���������������������������������������������������������������������������������������������������������������������� 
			��·й��:  �򿪵�ŷ�3���رյ�ŷ�2�͵�ŷ�4��ʹ������·�ڲ�����ж��
			����������������������������������������������������������������������������������������������������������������������*/
		  case AIR_DIS:
			{
				DetectSystem_AIR_DIS();
				break;
			}
			/*���������������������������������������������������������������������������������������������������������������������� 
			��·����:  �򿪵�ŷ�2���رյ�ŷ�3�͵�ŷ�4��������ƿ�ڳ�����
			ֱ��ѹ����ѹ����������Ϊ�ﵽ��ת���������ɼ���ѹ��2�ϵ�ģ�����õ�P1 
			(��һ�����ڸ������޳������õ�����ѹֵP1��������Vs)
			����������������������������������������������������������������������������������������������������������������������*/
			case AIR_INFLATION:
			{ 
				DetectSystem_AIR_INFLATION();
				break;
			}  
			/*����������������������������������������������������������������������������������������������������������������������
			��·����ƽ��: �رյ�ŷ�2����ŷ�3�͵�ŷ�4��ʹ���屣���ڴ�������0.1s���ȴ�2~3�룬�ﵽ��ת����
			����������������������������������������������������������������������������������������������������������������������*/
			case AIR_INFLATION_BALANCE:
			{ 
				DetectSystem_AIR_INFLATION_BALANCE();
				break;
			}
			/*���������������������������������������������������������������������������������������������������������������������� 
			��Ͱ���������ֹرշ�A���򿪷�B���ر�й������ֱ��ѹ������������½�,��ʱ1�룬��ȡʮ�����ݣ���ƽ��ֵ
			����������������������������������������������������������������������������������������������������������������������*/
			case AIR_TO_CARTRIDGE:
			{ 
				DetectSystem_AIR_TO_CARTRIDGE();
				break;
			}
	
			/*������ɣ��򿪷�A,��B���ر�й������ֱ��ѹ������������������ɶ���*/
			case AIR_DETECT_COMPLETE:
			{ 
				DetectSystem_AIR_DETECT_COMPLETE();
				break;
			}
	}
}

/*�򿪵�ŷ�3,4���رյ�ŷ�2��ʹ������·�ڲ�����ж��*/
void DetectSystem_AIR_DIS(void)
{
	VALVE_PRODUCE_SET();
	SSR_A_RESET();
	SSR_B_SET();
	SSR_C_SET();
	
	if(CheckOnlineData()==1)
	{
		Step_Loop_Num++;
		
		if(Step_Loop_Num>STEP_JUMP_CONDITION_NUM)
		{
			Clean_Online_Value_Buffer();
			Step_Loop_Num=0;
			delay_ms(DELAY_AT_STEP*1000);
			Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_INFLATION;
		}
	}
	else
	{
		Step_Loop_Num=0;
	}
}

/*
	��·����:  �򿪵�ŷ�2���رյ�ŷ�3�͵�ŷ�4��������ƿ�ڳ�����
	ֱ��ѹ����ѹ����������Ϊ�ﵽ��ת���������ɼ���ѹ��2�ϵ�ģ�����õ�P1 
	����һ�����ڸ������޳������õ�����ѹֵP1��������Vs��
*/
void DetectSystem_AIR_INFLATION(void)
{
	VALVE_PRODUCE_SET();
	SSR_A_SET();
	SSR_B_RESET();
	SSR_C_RESET();
	
	if(CheckOnlineData()==1)
	{
		Step_Loop_Num++;
		
		if(Step_Loop_Num>(STEP_JUMP_CONDITION_NUM*2))
		{
			Clean_Online_Value_Buffer();
			Step_Loop_Num=0;
			delay_ms(DELAY_AT_STEP*1000);
			
//			float __RT_Pressure_Data=Cartridge_Detect_Data.Level_Detect_Guage_Data_AN_AVG; 
//			
//			/*����������������������������������������������������������������������������������������������������������������������
//			����ɼ�P1
//			����������������������������������������������������������������������������������������������������������������������*/
//			if(Cartridge_Detect_Data.Cartridge_Main_Status==CARTRIDGE_STATUS_FULL_CALIBRATION)
//			{ 
//				Cartridge_Detect_Data.Cartridge_Calibration_Data.Fill_Air_Data_AN=__RT_Pressure_Data;
//			}
//			/*����������������������������������������������������������������������������������������������������������������������
//			����ɼ�P2
//			����������������������������������������������������������������������������������������������������������������������*/
//			else 
//			{
//			 Cartridge_Detect_Data.RT_Cartridge_Detect_Data.Fill_Air_Data_AN=__RT_Pressure_Data;
//			}
			Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_INFLATION_BALANCE;
		}
	}
	else
	{
		Step_Loop_Num=0;
	}
	
}
/*����������������������������������������������������������������������������������������������������������������������
��·����ƽ��:�رյ�ŷ�2����ŷ�3�͵�ŷ�4��ʹ���屣���ڴ�������0.1s
����������������������������������������������������������������������������������������������������������������������*/
void DetectSystem_AIR_INFLATION_BALANCE(void)
{	
	VALVE_PRODUCE_SET();
	SSR_A_RESET();
	SSR_B_RESET();
	SSR_C_RESET();
	if(CheckOnlineData()==1)
	{ 
		Step_Loop_Num++;

		if(Step_Loop_Num>(STEP_JUMP_CONDITION_NUM*2))
		{
			Step_Loop_Num=0;
			Clean_Online_Value_Buffer();
			delay_ms((uint32_t)((DELAY_AT_STEP)*1000));
			Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_TO_CARTRIDGE;
		}
	}
	else
	{
		Step_Loop_Num=0;
	}
	
	
}	
/*����������������������������������������������������������������������������������������������������������������������
�رյ�ŷ�2�͵�ŷ�3���򿪵�ŷ�4����Cartridge heater�ڳ�����
���ɼ���ѹ��2�ϵ�ģ�����õ�PL (��һ�����ڸ�������ɢ���壬�õ�����ѹֵPL��������Vs��
����������������������������������������������������������������������������������������������������������������������*/
void DetectSystem_AIR_TO_CARTRIDGE(void)
{
	VALVE_PRODUCE_SET();
	SSR_A_RESET();
	SSR_B_RESET();		
	SSR_C_SET();
	
	if(CheckOnlineData()==1)
	{ 
		Step_Loop_Num++;
		
		if(Step_Loop_Num>(STEP_JUMP_CONDITION_NUM*2))
		{
			Clean_Online_Value_Buffer();
			Step_Loop_Num=0;
			delay_ms(DELAY_AT_STEP*1000);
			Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_DETECT_COMPLETE;
		}
	}
	else
	{
		Step_Loop_Num=0;
	}
	
}	
/*����������������������������������������������������������������������������������������������������������������������
������ɣ�
����������������������������������������������������������������������������������������������������������������������*/
void DetectSystem_AIR_DETECT_COMPLETE(void)
{
 
	 
	float __RT_Pressure_Data=Cartridge_Detect_Data.Level_Detect_Guage_Data_AN_AVG;//Read_ADC_AverageValue();//GetValueSum/(float)GetValueIndex;
	
	switch(Cartridge_Detect_Data.Cartridge_Main_Status)
	{ 
		case CARTRIDGE_STATUS_EMPTY_CALIBRATION:
		{
			//Cartridge_Calibration_And_Detect_Finished(CARTRIDGE_STATUS_EMPTY_CALIBRATION ,__RT_Pressure_Data);
			Cartridge_Detect_Data.Cartridge_Calibration_Data.Empty_Calibration_Data_AN=__RT_Pressure_Data;
			//XX;
			break;
		}
		case CARTRIDGE_STATUS_FULL_CALIBRATION: 
		{  
			/*����������������������������������������������������������������������������������������������������������������������
			������ɣ��ɼ�PL,����¼
			����������������������������������������������������������������������������������������������������������������������*/
			
			//Cartridge_Detect_Data.Cartridge_Calibration_Data.Detect_Air_Data_AN=__RT_Pressure_Data;
			//Cartridge_Calibration_And_Detect_Finished(CARTRIDGE_STATUS_FULL_CALIBRATION ,__RT_Pressure_Data);
			//Cartridge_Calibration_And_Detect_Finished(CARTRIDGE_STATUS_FULL_CALIBRATION ,__RT_Pressure_Data);
			Cartridge_Detect_Data.Cartridge_Calibration_Data.Full_Calibrationt_Data_AN=__RT_Pressure_Data;
			break;
		}
		case CARTRIDGE_STATUS_CARTRIDGE_DETECT:
		{ 
			
			/*����������������������������������������������������������������������������������������������������������������������
			������ɣ��ɼ�Pa,����¼,����
			����������������������������������������������������������������������������������������������������������������������*/
 

			double P1=Cartridge_Detect_Data.Cartridge_Calibration_Data.Full_Calibrationt_Data_AN*(__RT_Pressure_Data-Cartridge_Detect_Data.Cartridge_Calibration_Data.Empty_Calibration_Data_AN);
			double P2=__RT_Pressure_Data*(Cartridge_Detect_Data.Cartridge_Calibration_Data.Full_Calibrationt_Data_AN-Cartridge_Detect_Data.Cartridge_Calibration_Data.Empty_Calibration_Data_AN);
			
			double Percent=P2==0.0f?0.0f:(P1/P2);
			
			Cartridge_Detect_Data.Cartridge_Detect_Data_Precent=Percent;
			Cartridge_Detect_Data.Cartridge_Detect_Data
			=Percent
			*(Cartridge_Detect_Data.Cartridge_Calibration_Data.Full_Sryinge_Vol
			-Cartridge_Detect_Data.Cartridge_Calibration_Data.Empty_Sryinge_Vol);
			
			Running_status.cartridge
			= (Cartridge_Detect_Data.Cartridge_Calibration_Data.Full_Sryinge_Vol
			-Cartridge_Detect_Data.Cartridge_Calibration_Data.Empty_Sryinge_Vol)*1000.0f;// 
			
			Running_status.cartridge_now= Cartridge_Detect_Data.Cartridge_Detect_Data*1000.0f;// 
			Running_status.cartridge_status	=Percent*100.0f;
			
			Cartridge_Detect_Data.Dispensing_Finished_Counter=Cartridge_Detect_Data.Auto_Detect_Counter;
			
			break;
		} 
		default:
		{
			break;
		}
	}
	
	Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_NONE;
	
}


uint8_t CheckOnlineData(void)
{
	//ADV_Online_Value_Buffer[STEP_JUMP_CONDITION_NUM]
	double MaxValue__=ADV_Online_Value_Buffer[0];
	double MinValue__=MaxValue__;
	
	for(int index__ =0;index__<STEP_JUMP_CONDITION_NUM;index__++)
	{
		
		if(ADV_Online_Value_Buffer[index__]>MaxValue__)
			MaxValue__=ADV_Online_Value_Buffer[index__]; 
		
		if(ADV_Online_Value_Buffer[index__]<MinValue__)
			MinValue__=ADV_Online_Value_Buffer[index__];
	} 
	 
	
	if(abs(MaxValue__-MinValue__)<=AN_ACCURACY)
	{
		return 1;
	}
	else
		return 0;
}


