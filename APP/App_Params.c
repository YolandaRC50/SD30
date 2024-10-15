#include "APP_Params.h"
#include "w25qxx.h"
 
const uint16_t PARAMS_LEN = sizeof(System_Params)+4; 
const int PARAMS_START_ADDR=0;
const uint8_t BUFFER_HEADER=0x55;
	
uint8_t m_Params_Buffer[PARAMS_LEN];
float test_Buffer[10];
System_RT_Status Sys_RT_Status;
System_Params Sys_Params=
{ 
	
	
	/*
		IOT_COMMICATION_MODE_ENUM Commication_Mode;
		IOT_Loca_Params Loca_Params;
		IOT_Server_Params Server_Params;
		IoT_Ethernet_Params Ethernet_Params;
		IoT_WIFI_Params WIFI_Params;
		IoT_Bluetooth_Params Bluetooth_Params;
		*/
		
		#if(1) /*Commication_Mode*/  
		.IOT_Params.Commication_Mode= ETHERNET_MODE,
		#endif
		#if(1) /*Loca_Params*/  
		.IOT_Params.Loca_Params.Loca_Name="ISD II NAE",
		.IOT_Params.Loca_Params.Loca_IDH="ISD II IDH",
		.IOT_Params.Loca_Params.Loca_SN="ISD II SN", 
		#endif
		#if(1) /*Server_Params*/ 
	  .IOT_Params.Server_Params.Server_IPAddress="192.168.1.250",
		.IOT_Params.Server_Params.Sever_Port=8088,
		#endif
		#if(1) /*Ethernet_Params*/   
		.IOT_Params.Ethernet_Params.IP_Address="192.168.1.101",
		.IOT_Params.Ethernet_Params.Gate_way="192.168.1.1",
		.IOT_Params.Ethernet_Params.Net_Mask="255.255.255.0",
		#endif
		#if(1) /*WIFI_Params*/  
		.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name="Rody_PLC_LINK",
		.IOT_Params.WIFI_Params.Wlan_Password="18980928898", 
		#endif

		 
};
 
/* 
��������fת��Ϊ4���ֽ����ݴ����byte[4]�� 
*/  
 void Float_to_Byte(float f,uint8_t byte[],uint16_t *index)  
{  
    FloatLongType fl;  
    fl.fdata=f;  
    byte[*index+0]=(unsigned char)fl.ldata;  
    byte[*index+1]=(unsigned char)(fl.ldata>>8);  
    byte[*index+2]=(unsigned char)(fl.ldata>>16);  
    byte[*index+3]=(unsigned char)(fl.ldata>>24);  
		*index+=4;
}  
/* 
��4���ֽ�����byte[4]ת��Ϊ�����������*f�� 
*/  
float Byte_to_Float(uint8_t byte[],uint16_t *index) 
{  
	float  tf;
  FloatLongType fl;  
	if(byte[*index+0]==0xFF
		&&byte[*index+1]==0xFF
		&&byte[*index+2]==0xFF
		&&byte[*index+3]==0xFF)
		{
			 tf=0.0f;
			 return tf;
		}
    fl.ldata=0;  
    fl.ldata=byte[*index+3];  
    fl.ldata=(fl.ldata<<8)|byte[*index+2];  
    fl.ldata=(fl.ldata<<8)|byte[*index+1];  
    fl.ldata=(fl.ldata<<8)|byte[*index+0];  
    tf=fl.fdata; 
		*index+=4;
		return tf;
}  


uint16_t Byte_to_int(uint8_t byte[],uint16_t *index)
{ 

	int16_t _u16= (byte[*index])+(byte[*index+1]<<8);
	*index+=2;
	return _u16; 
}
/* 
��Byte�ֽ�ת��Ϊ�ַ�����
*/  
uint16_t Byte_to_String(uint8_t byte[],char strArr[],uint16_t *index,uint8_t ArrayLen)
{ 
	for(uint16_t Len=0;Len<ArrayLen;Len++)
	{
		strArr[Len]=  byte[*index+Len];
		if(strArr[Len]=='\0')
			break;
	}
	
	*index+=ArrayLen;
	return ArrayLen;
}


void Int_to_Byte(uint16_t viValue,uint8_t byte[],uint16_t *index)
{ 
	byte[*index]=viValue;
	byte[*index+1]=viValue>>8;
	*index+=2; 
}




int16_t String_to_Byte(char strArr[],uint8_t byte[],uint16_t *index,uint8_t ArrayLen)
{  
	for(uint16_t Len=0;Len<ArrayLen;Len++)
	{
		 byte[*index+Len]=strArr[Len];
		if(strArr[Len]=='\0')
			break;
	} 
	*index+=ArrayLen;
	return ArrayLen;
}


void ResetAllParams()
{
	Sys_Params.Dispense_Param.Run_Mode_Params=CONTINUE;
	Sys_Params.Dispense_Param.Dis_Timer=0.0f;
	Sys_Params.Dispense_Param.Inter_timer=0.0f;
	for(uint8_t i=0;i<PRONUM;i++)
	{
			Sys_Params.save_pro_buffer[i][0] = 0.0f;
			Sys_Params.save_pro_buffer[i][1] = 0.0f;
			Sys_Params.save_pro_buffer[i][2] = 0.0f;
			Sys_Params.save_pro_buffer[i][3] = 0.0f;
			Sys_Params.save_pro_buffer[i][4] = 0.0f;
			Sys_Params.save_pro_buffer[i][5] = 0.0f;
	}
//	Sys_Params.Dispense_Param.shot_num=10.0f;
//	Sys_Params.Dispense_Param.sys_count=10.0f;
	Sys_Params.Dispense_Param.program_number=10.0f;
	
	
	
	
	Sys_Params.Pressure_Param.TargetPreessure=0.1;
	Sys_Params.Pressure_Param.TargetPreessure_Range=0;
	Sys_Params.Pressure_Param.Targetvacuum=0.10f;
	
	Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=1024.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value=4096.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value=9.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_3_Value=1024.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_4_Value=4096.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.RT_3_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.RT_4_Value=9.0f;
	
	Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value=2048.0f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value=5.0f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value=2048.0f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.RT_3_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.RT_4_Value=5.0f;
	Sys_Params.Pressure_Param.Pressure_Unit= BAR; 
	Sys_Params.Pressure_Param.Vacuum_Unit = VPSI;
	Sys_Params.Pressure_Param.Tem_Unit = fahr ; 
	Sys_Params.FPASS_AC=POWER_OFF;
	Sys_Params.SPASS_AC=POWER_OFF;
	
	/*
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=4096;
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=50.0;
	
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=4096;
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=100.0;   */
	

	
	#if(1) /*IOT����*/
		/*
		IOT_COMMICATION_MODE_ENUM Commication_Mode;
		IOT_Loca_Params Loca_Params;
		IOT_Server_Params Server_Params;
		IoT_Ethernet_Params Ethernet_Params;
		IoT_WIFI_Params WIFI_Params;
		IoT_Bluetooth_Params Bluetooth_Params;
		*/
		
		#if(1) /*Commication_Mode*/ 
		Sys_Params.IOT_Params.Commication_Mode= ETHERNET_MODE;
		#endif
		#if(1) /*Loca_Params*/ 
		#endif
		#if(1) /*Server_Params*/ 
		Sys_Params.IOT_Params.Server_Params.Sever_Port=8088;
		#endif
		#if(1) /*Ethernet_Params*/ 
		#endif
		#if(1) /*WIFI_Params*/ 
		#endif
		#if(1) /*Bluetooth_Params*/  
		#endif
		
	#endif
}
void LoadSystemParams()
{
	W25QXX_Read(m_Params_Buffer,PARAMS_START_ADDR,PARAMS_LEN);
	
	if(m_Params_Buffer[0]==BUFFER_HEADER &&m_Params_Buffer[1]==BUFFER_HEADER)
	{
		uint16_t m_Index=2;
		
		Sys_Params.Dispense_Param.Run_Mode_Params=Byte_to_int(m_Params_Buffer,&m_Index);
		Sys_Params.Dispense_Param.Dis_Timer=Byte_to_Float(m_Params_Buffer,&m_Index);
 		Sys_Params.Dispense_Param.Inter_timer=Byte_to_Float(m_Params_Buffer,&m_Index);
		for(uint8_t i=0;i<PRONUM;i++)
		{
				Sys_Params.save_pro_buffer[i][0] = Byte_to_Float(m_Params_Buffer,&m_Index);
				Sys_Params.save_pro_buffer[i][1] = Byte_to_Float(m_Params_Buffer,&m_Index);
				Sys_Params.save_pro_buffer[i][2] = Byte_to_Float(m_Params_Buffer,&m_Index);
				Sys_Params.save_pro_buffer[i][3] = Byte_to_Float(m_Params_Buffer,&m_Index);
				Sys_Params.save_pro_buffer[i][4] = Byte_to_int(m_Params_Buffer,&m_Index);
				Sys_Params.save_pro_buffer[i][5] = Byte_to_int(m_Params_Buffer,&m_Index);			
		}
		
		
//		Sys_Params.Dispense_Param.shot_num=Byte_to_int(m_Params_Buffer,&m_Index);
//		Sys_Params.Dispense_Param.sys_count=Byte_to_int(m_Params_Buffer,&m_Index);
		Sys_Params.Dispense_Param.program_number=Byte_to_int(m_Params_Buffer,&m_Index);
			
		Sys_Params.Pressure_Param.TargetPreessure=Byte_to_Float(m_Params_Buffer,&m_Index); 
		Sys_Params.Pressure_Param.TargetPreessure_Range=Byte_to_Float(m_Params_Buffer,&m_Index); 
		Sys_Params.Pressure_Param.Targetvacuum=Byte_to_Float(m_Params_Buffer,&m_Index); 

		 
		Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_3_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_4_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.RT_3_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.RT_4_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		
		Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.RT_3_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.RT_4_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		Sys_Params.Pressure_Param.Pressure_Unit= Byte_to_int(m_Params_Buffer,&m_Index) ; 
		Sys_Params.Pressure_Param.Vacuum_Unit = Byte_to_int(m_Params_Buffer,&m_Index) ; 
		Sys_Params.Pressure_Param.Tem_Unit = Byte_to_int(m_Params_Buffer,&m_Index) ; 
			/*Int_to_Byte(Sys_Params.Pressure_Param.Pressure_Unit,m_Params_Buffer,&m_Index);
	Int_to_Byte(Sys_Params.Pressure_Param.Vacuum_Unit,m_Params_Buffer,&m_Index);
	
	Int_to_Byte(Sys_Params.Pressure_Param.Tem_Unit,m_Params_Buffer,&m_Index);*/
		
		/*Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);*/
		
 
		
		
	#if (1==1) /*IOT����*/
		/*
		IOT_COMMICATION_MODE_ENUM Commication_Mode;
		IOT_Loca_Params Loca_Params;
		IOT_Server_Params Server_Params;
		IoT_Ethernet_Params Ethernet_Params;
		IoT_WIFI_Params WIFI_Params;
		IoT_Bluetooth_Params Bluetooth_Params;
		*/
		
		#if(1==1) /*Commication_Mode*/ 
		Sys_Params.IOT_Params.Commication_Mode= Byte_to_int(m_Params_Buffer,&m_Index); 
		#endif
		
//		#define DEVICE_NAME_LEN 20
//		#define IP_ADDRESS_LEN  20
//		#define PASSWORD_LEN    20 

		#if (1==1) /*Loca_Params*/ 
		
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Loca_Params.Loca_Name ,&m_Index,DEVICE_NAME_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Loca_Params.Loca_IDH ,&m_Index,DEVICE_NAME_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Loca_Params.Loca_SN ,&m_Index,DEVICE_NAME_LEN);
		#endif
		#if (1==1) /*Server_Params*/ 
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Server_Params.Server_IPAddress ,&m_Index,IP_ADDRESS_LEN);		
		Sys_Params.IOT_Params.Server_Params.Sever_Port=Byte_to_int(m_Params_Buffer,&m_Index);
		#endif
		#if (1==1) /*Ethernet_Params*/ 
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Ethernet_Params.IP_Address ,&m_Index,IP_ADDRESS_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Ethernet_Params.Gate_way ,&m_Index,IP_ADDRESS_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Ethernet_Params.Net_Mask ,&m_Index,IP_ADDRESS_LEN);
		#endif
		#if (1==1) /*WIFI_Params*/ 
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name ,&m_Index,DEVICE_NAME_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.WIFI_Params.Wlan_Password ,&m_Index,PASSWORD_LEN);
		#endif

		
		#if (1==1) /*Login_Params*/  
		Byte_to_String(m_Params_Buffer,Sys_Params.Login_Pwd.First_Password ,&m_Index,LOGIN_PWD_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.Login_Pwd.Second_Password ,&m_Index,LOGIN_PWD_LEN);
		Sys_Params.FPASS_AC=Byte_to_int(m_Params_Buffer,&m_Index);
		Sys_Params.SPASS_AC=Byte_to_int(m_Params_Buffer,&m_Index);
		#endif
	#endif
	}
	else
	{
		ResetAllParams();
	}
}
void SaveSystemParams()
{
	uint16_t m_Index=0;
	m_Params_Buffer[m_Index++]=BUFFER_HEADER;
	m_Params_Buffer[m_Index++]=BUFFER_HEADER;

	Int_to_Byte(Sys_Params.Dispense_Param.Run_Mode_Params,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Dispense_Param.Dis_Timer,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Dispense_Param.Inter_timer,m_Params_Buffer,&m_Index);
	for(uint8_t i=0;i<PRONUM;i++)
	{
			Float_to_Byte(Sys_Params.save_pro_buffer[i][0],m_Params_Buffer,&m_Index);
			Float_to_Byte(Sys_Params.save_pro_buffer[i][1],m_Params_Buffer,&m_Index);
			Float_to_Byte(Sys_Params.save_pro_buffer[i][2],m_Params_Buffer,&m_Index);
			Float_to_Byte(Sys_Params.save_pro_buffer[i][3],m_Params_Buffer,&m_Index);
			Int_to_Byte(Sys_Params.save_pro_buffer[i][4],m_Params_Buffer,&m_Index);
			Int_to_Byte(Sys_Params.save_pro_buffer[i][5],m_Params_Buffer,&m_Index);
	}
//	Int_to_Byte(Sys_Params.Dispense_Param.shot_num,m_Params_Buffer,&m_Index);
//	Int_to_Byte(Sys_Params.Dispense_Param.sys_count,m_Params_Buffer,&m_Index);
	Int_to_Byte(Sys_Params.Dispense_Param.program_number,m_Params_Buffer,&m_Index);

	
	Float_to_Byte(Sys_Params.Pressure_Param.TargetPreessure,m_Params_Buffer,&m_Index);//9.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.TargetPreessure_Range,m_Params_Buffer,&m_Index);//9.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Targetvacuum,m_Params_Buffer,&m_Index);//9.0f;


	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//1024.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value,m_Params_Buffer,&m_Index);//4096.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value,m_Params_Buffer,&m_Index);//0.001f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value,m_Params_Buffer,&m_Index);//9.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_3_Value,m_Params_Buffer,&m_Index);//1024.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_4_Value,m_Params_Buffer,&m_Index);//4096.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_3_Value,m_Params_Buffer,&m_Index);//0.001f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_4_Value,m_Params_Buffer,&m_Index);//9.0f;


	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//1024.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value,m_Params_Buffer,&m_Index);//4096.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value,m_Params_Buffer,&m_Index);//0.001f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value,m_Params_Buffer,&m_Index);//9.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value,m_Params_Buffer,&m_Index);//1024.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value,m_Params_Buffer,&m_Index);//4096.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.RT_3_Value,m_Params_Buffer,&m_Index);//0.001f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.RT_4_Value,m_Params_Buffer,&m_Index);//9.0f;

	
	Int_to_Byte(Sys_Params.Pressure_Param.Pressure_Unit,m_Params_Buffer,&m_Index);
	Int_to_Byte(Sys_Params.Pressure_Param.Vacuum_Unit,m_Params_Buffer,&m_Index);
	
	Int_to_Byte(Sys_Params.Pressure_Param.Tem_Unit,m_Params_Buffer,&m_Index);
	
	/*
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//4096;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//50.0;

	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//4096;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//100.0;*/
	
	

	
	#if (1==1) /*IOT Paeams*/
		#if(1) /*Commication_Mode*/  
		Int_to_Byte(Sys_Params.IOT_Params.Commication_Mode,m_Params_Buffer,&m_Index);
		#endif
				
//		#define DEVICE_NAME_LEN 20
//		#define IP_ADDRESS_LEN  20
//		#define PASSWORD_LEN    20 
		#if (1==1) /*Loca_Params*/ 
		
		String_to_Byte (Sys_Params.IOT_Params.Loca_Params.Loca_Name ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN);
		String_to_Byte (Sys_Params.IOT_Params.Loca_Params.Loca_IDH ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN);
		String_to_Byte (Sys_Params.IOT_Params.Loca_Params.Loca_SN ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN); 
		
		 
		#endif
		#if (1==1) /*Server_Params*/  
		String_to_Byte (Sys_Params.IOT_Params.Server_Params.Server_IPAddress ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		Int_to_Byte(Sys_Params.IOT_Params.Server_Params.Sever_Port,m_Params_Buffer,&m_Index); 
		#endif
		#if (1==1) /*Ethernet_Params*/  
		String_to_Byte (Sys_Params.IOT_Params.Ethernet_Params.IP_Address ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		String_to_Byte (Sys_Params.IOT_Params.Ethernet_Params.Gate_way ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		String_to_Byte (Sys_Params.IOT_Params.Ethernet_Params.Net_Mask ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		#endif
		#if (1==1) /*WIFI_Params*/  
		String_to_Byte (Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN);  
		String_to_Byte (Sys_Params.IOT_Params.WIFI_Params.Wlan_Password ,m_Params_Buffer,&m_Index,PASSWORD_LEN); 
		#endif

		
		#if (1==1) /*Login_Params*/  
		String_to_Byte (Sys_Params.Login_Pwd.First_Password ,m_Params_Buffer,&m_Index,LOGIN_PWD_LEN); 
		String_to_Byte (Sys_Params.Login_Pwd.Second_Password,m_Params_Buffer,&m_Index,LOGIN_PWD_LEN);
		Int_to_Byte(Sys_Params.FPASS_AC,m_Params_Buffer,&m_Index);
		Int_to_Byte(Sys_Params.SPASS_AC,m_Params_Buffer,&m_Index);				
		#endif
		
	#endif	
	  
	 
	 W25QXX_Write(m_Params_Buffer,PARAMS_START_ADDR,PARAMS_LEN);
	 
	 uint8_t __ReadBuffer[PARAMS_LEN];
	 W25QXX_Read(__ReadBuffer,PARAMS_START_ADDR,PARAMS_LEN);
	 POWER_STATUS __RT=POWER_ON;
	 for(uint16_t _Index=0;_Index<PARAMS_LEN;_Index++)
	 {
		 if(__ReadBuffer[_Index]==m_Params_Buffer[_Index])
		 {
			 continue;
		 }
		 else
		 {
			 __RT=POWER_OFF;
			 break;
		 }
	 }
	  
}

/*POWER_STATUS Match_Pwd(uint8_t viLevel)
{
	POWER_STATUS Login_Status=POWER_OFF;
	if(viLevel==0||viLevel==1)
	{
		for(int i=0;i<LOGIN_PWD_LEN;i++)
		{
			if(Sys_RT_Status.Login_Pwd.First_Password[i]!=Sys_Params.Login_Pwd.First_Password[i])
				break;
			if(i==LOGIN_PWD_LEN-1)
				Login_Status=POWER_ON;
		}
	}
	else if(viLevel==2)
	{
		for(int i=0;i<LOGIN_PWD_LEN;i++)
		{
			
			if(Sys_RT_Status.Login_Pwd.Second_Password[i]!=Sys_Params.Login_Pwd.Second_Password[i])
				break;
			if(i==LOGIN_PWD_LEN-1)
				Login_Status=POWER_ON;
		}
	} 
	
	return Login_Status;
		
}*/

