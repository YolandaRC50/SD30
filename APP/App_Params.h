#ifndef __APP_PARAMS_H
#define __APP_PARAMS_H 
#include "stm32f4xx.h" 
#include "lwip/api.h"
#define __PACKED_LEN 4

#define DEVICE_NAME_LEN 20
#define IP_ADDRESS_LEN  20
#define PASSWORD_LEN    20 
#define IOT_SENDANDRECEIVE_BUFFER_LEN 512
#define BLUETOOTH_MAX_CONNECT_NUM 9
#define LOGIN_PWD_LEN   20
#define PRONUM  40

#if(1) /*ENUM*/

/* 
Ҫ����ʾ: 
1. float��unsigned long������ͬ�����ݽṹ���� 
2. union������������ݴ������ͬ������ռ� 
*/  
typedef union  __FLOATLONGTYPE
{  
    float fdata;  
    unsigned long ldata;  
}FloatLongType; 


 
	#if (1)  /*ENUM*/
typedef enum __IOT_COMMICATION_MODE_ENUM
	{
		NO_CONNECT=0,
		ETHERNET_MODE=1,
		WIFI_MODE=2,
	}IOT_COMMICATION_MODE_ENUM;
		
	#endif	

/*--->
ϵͳ����
-->*/
typedef enum __SYSTEM_LANGUAGE
{
	
	 LANG_ENGLISH=0,
	 LANG_CHINESE=1,
 } SYSTEM_LANGUAGE;
typedef enum __DISPENSING_STEP
{
	DISPENSING_STOP=0,
	DISPENSING_STATUS=1,
	DISPENSING_INTER=2,
}DISPENSING_STEP;

typedef enum __PRESSURE_UNIT_MODE
{
	BAR=0,
	PSI=1,
} PRESSURE_UNIT;
typedef enum __VACUUM_UNIT_MODE
{	
	mmHg=0,
	VPSI=1,
} VACUUM_UNIT;


//TEM_UNIT
typedef enum __TEM_UNIT_MODE
{
	fahr=0,
	centi=1,
} TEM_UNIT;

 typedef enum __SYSTEM_OPTION_MODE
 {
	 STOP_MODE=0,
	 AUTORUN_MODE=1,
 }SYSTEM_OPTION_MODE;
typedef enum __POWER_STATUS
{
	POWER_ON=1,
	POWER_OFF=!POWER_ON,
}POWER_STATUS;
 
typedef enum __CARTRIDGE_STATUS
	{
	FULL=0,
	LOWER=1,
	EMPTY=2,
} Cartridge_Status;

typedef enum __SYSTEM_STATUS{
	READY=0,
	DISPENSING=1,
	STOP=2,
	ALM=3,
} System_Status;

typedef enum __LOG_TYPE{
	MACHINE_INFO_LOG=0,
	RUN_STOP_LOG=1,
	ERROR_MESSAGE_LOG=2,
	PARAMETER_LOG=3
}Log_Type;

typedef enum __RUN_MODE
{
	PRO_NO = 0, 
	TIMER = 1,
	CYCLE = 2,
	CONTINUE = 3,	
	
}RUN_MODE;

#endif

#if(1) /*Struct*/
		//typedef struct __LOG_PATH_ITEM{
		//	Log_Type Log_Type;
		//	char Log_Path[20];
		//}Log_Path_Item;

		//typedef struct __LOG_PATH_LIST{
		//	Log_Path_Item Machine_info_log;
		//	Log_Path_Item Run_stop_log;
		//	Log_Path_Item Error_message_log;
		//	Log_Path_Item Parameter_log;
		//} Log_Path_List;

		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct _SYSTEM_RTC
		{
		float Analog_1_Value;   //�洢�����У��ֵ0
		float RT_1_Value;
		float Analog_2_Value;   //�洢�����У��ֵ5
		float RT_2_Value;
		float Analog_3_Value;  //�洢�����У��ֵ0
		float RT_3_Value;
		float Analog_4_Value;  //�洢�����У��ֵ0.5
		float RT_4_Value;
			

		} Calibration_Params;                   

		#pragma pack()

		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct _CALIBRATION_PARAMS
		{
		uint8_t year;
		uint8_t month;
		uint8_t week;
		uint8_t day;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
		uint8_t year_BCD;
		uint8_t month_BCD;
		uint8_t week_BCD;
		uint8_t day_BCD;
		uint8_t hour_BCD;
		uint8_t minute_BCD;
		uint8_t second_BCD;

		} System_RTC;


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct _DISPENSE_PARAMS_ITEM
		{
		RUN_MODE Run_Mode_Params;
		POWER_STATUS Pro_mode;	
		float Dis_Timer;
		float Inter_timer;
		uint32_t shot_num;
		uint32_t sys_count;
		uint8_t program_number;
		} Dispense_Params_Item;


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __PRESSURE_PARAMS
		{
		Calibration_Params Calibration_RT_Param;   //���������ص���ʵֵ
		Calibration_Params Calibration_Set_Param;    //�趨ֵ
		PRESSURE_UNIT Pressure_Unit;
		VACUUM_UNIT Vacuum_Unit;
		TEM_UNIT Tem_Unit;
			
		float TargetPreessure;
		float TargetPreessure_Range;
		float Targetvacuum;
		//float Targetvacuum_Range;
			
		 
		} Pressure_Params; 


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __TEMPERATURE_HUMIDITY
		{ 
		Calibration_Params Temperature_Calibration_Param;
		Calibration_Params Humidity_Calibration_Param;

		} Temperature_Humidity;


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		#pragma pack() 
		#endif


		#if (1)  /*IOT_PARAMS*/
		 
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_WIFI_PARAMS   //40
		{	 
			/*����������*/
			char Wlan_SwitchHub_Name[DEVICE_NAME_LEN]; 		 //20
			/*��¼����*/ 
			char Wlan_Password[PASSWORD_LEN];  //20
		} IoT_WIFI_Params;


		#pragma pack()


		/*-------------------------------------*/

		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_BLUETOOTH_PARAMS //20
		{ 
			/*�������*/
			char Bluetooth_Match_Password[PASSWORD_LEN]; //20
		} IoT_Bluetooth_Params; 
		 
		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_ETHERNET_PARAMS //60
		{ 
			/*IP��ַ*/
			char IP_Address[IP_ADDRESS_LEN]; 	 //20
			/*����*/
			char Gate_way[IP_ADDRESS_LEN];  //20
			/*��������*/ //20
			char Net_Mask[IP_ADDRESS_LEN];   //20
			
			
			
		} IoT_Ethernet_Params;


		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_SERVER_PARAMS //24
		{
			/*������IP��ַ*/
			char Server_IPAddress[IP_ADDRESS_LEN]; //20
			/*�������˿ں�*/
			uint16_t Sever_Port;  //2
			
		} IOT_Server_Params;
			

		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_LOCA_PARAMS //60
		{
			/*��������*/
			
			char Loca_Name[DEVICE_NAME_LEN]; //20
			char Loca_IDH[DEVICE_NAME_LEN]; //20
			char Loca_SN[DEVICE_NAME_LEN];  //20
			 
		} IOT_Loca_Params;



		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __SYSTEM_IOT_PARAMS
		{
			IOT_Loca_Params Loca_Params; //60
			IOT_Server_Params Server_Params;//24
			IoT_Bluetooth_Params Bluetooth_Params;
			IoT_WIFI_Params WIFI_Params;
			IoT_Ethernet_Params Ethernet_Params;
			IOT_COMMICATION_MODE_ENUM Commication_Mode;  //2
		} System_IOT_Params;


		#pragma pack() 


		#endif
		/*-------------------------------------*/
		
		#if(1) /*Login_PARAMS*/
		#pragma pack(__PACKED_LEN)
		typedef struct __LOGIN_PWD
		{
			 /*����������*/
			char First_Password[LOGIN_PWD_LEN]; 		 //20
			/*��¼����*/ 
			char Second_Password[LOGIN_PWD_LEN];  //20
			
		} LOGIN_PWD;
		#pragma pack() 
		#endif

		#if(1)  /*IOT_STSTUS*/
		typedef struct __RT_WIFI_SEARCH
		{
			uint8_t Ecn; //ģʽ
			char Ssid[DEVICE_NAME_LEN]; //�ȵ�����
			int16_t Rssi;    //�ź�ǿ��
		} RT_Wifi_Search;		
		 
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_WIFI_STATUS
		{ 
			RT_Wifi_Search Wifi_Search_List[5]; 
			int16_t Status_Step;
		} RT_WIFI_Status;
		
		
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_BLUETOOTH_STATUS
		{
			#if (1)
			char array_name[BLUETOOTH_MAX_CONNECT_NUM][DEVICE_NAME_LEN];
			char array_ip[BLUETOOTH_MAX_CONNECT_NUM][DEVICE_NAME_LEN];// 1-8 ��8��
			uint8_t scan_num;//ɨ�赽����������
			uint8_t scan_use_num;//ʹ�õ����� 
			char array_connect_name[DEVICE_NAME_LEN];
			char array_connect_ip[IP_ADDRESS_LEN];// 1-8 ��8��
			char array_password[PASSWORD_LEN];//�������
			#endif
			uint16_t Command;
			int16_t  Status;
		} RT_Bluetooth_Status;
		 
		
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_WIFI_BLUETOOTH_STATUS
		{
			char *p_Recive_Buffer;  
			uint16_t Recive_Buffer_Len; //���ջ������ó���
			uint16_t Send_Buffer_Len;		//���ͻ������ó���
			
		}RT_WIFI_Bluetooth_Status;
		
		
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_ETHERNET_STATUS
		{
				struct netconn *tcp_clientconn;	//TCP CLIENT�������ӽṹ��
				uint16_t r_l;
				uint16_t r_flag; 
				uint16_t t_l;
				uint16_t t_flag;
				char tcp_client_flag;		//TCP�ͻ������ݷ��ͱ�־λ 
				struct pbuf *q;
			
				uint8_t flag_send;
			
			
			
				uint8_t mac[6];      //MAC��ַ
				uint8_t dhcpstatus;	//dhcp״̬ 
												//0,δ��ȡDHCP��ַ;
												//1,����DHCP��ȡ״̬
												//2,�ɹ���ȡDHCP��ַ
												//0XFF,��ȡʧ��.
		} RT_Ethernet_Status;
		#pragma pack()
		
		#pragma pack(__PACKED_LEN)
		typedef	struct __RT_COMM_STATUS_DATA
		{
			char  			*p_in_u8;//���յ�������
			char  			*p_out_u8;
			uint16_t 		*p_in_u8_l;//�������ݵĳ���
			uint16_t 		*p_out_u8_l;
			
			ip_addr_t   Server_IPAddr;
			ip_addr_t   Loca_IPAddr; 
			ip_addr_t 	Gate_way;  //����
			ip_addr_t 	Net_Mask;   //����
			uint16_t  	Ethernet_Loca_Port,WIFI_Loca_Port;
		}RT_Comm_Status_Data;
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __SYSTEM_RT_IOT_STATUS
		{
			
			RT_WIFI_Status WIFI_Status;
			RT_Bluetooth_Status Bluetooth_Status;
			RT_Ethernet_Status Ethernet_Status; 
			POWER_STATUS Connect_Status;
			char Recive_Buffer[IOT_SENDANDRECEIVE_BUFFER_LEN];
			char Send_Buffer[IOT_SENDANDRECEIVE_BUFFER_LEN];		 
			RT_Comm_Status_Data Comm_Status_Data;
			
			RT_WIFI_Bluetooth_Status WIFI_Bluetooth_Status;
		} System_RT_IOT_Status;
		
	 
		#pragma pack() 
		#endif
 
 
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __SYSTEM_RT_STATUS
		{
			//Cartridge_Status Cartridge_Status;
			System_Status System_Status;    //ϵͳ״̬
			//System_Status CH_Status[2];
			float Temperature_RT;
			float Humidity_RT;
			float RT_Pressure;
			float RT_AN_Pressure;     //
			float RT_Vacuum;
			float RT_AN_Vacuum;
			POWER_STATUS Pressure_Set_Cal_0Bar_Enable;
			POWER_STATUS Pressure_Set_Cal_5Bar_Enable;
			POWER_STATUS Vacuum_Set_Cal_0Bar_Enable;
			POWER_STATUS Vacuum_Set_Cal_5Bar_Enable;
			POWER_STATUS reset;
			POWER_STATUS admin;
			POWER_STATUS PURGE;
			POWER_STATUS TEACH;   //TEACHģʽ����
			POWER_STATUS SHOT;
			POWER_STATUS SUPERAD;  //�����볬��Ȩ��
			//POWER_STATUS FPASS_AC;  //��һ������Ȩ��
			//POWER_STATUS SPASS_AC;  //�ڶ�������Ȩ��
			
			POWER_STATUS pressure_disable_Mode;      //predisable����
			
			DISPENSING_STEP Dispensing_Step;    //   �㽺״̬
			uint64_t DISPENSING_COUNT_DOWN;     //�㽺ʱ�����
			uint64_t DISPENSING_COUNT_UP;     //�㽺ʱ���ۼ�
			uint64_t Intertime_COUNT_DOWN;     //�㽺ʱ�����
			uint64_t Intertime_COUNT_UP;     //�㽺ʱ���ۼ�
			SYSTEM_OPTION_MODE System_Option_Mode;    //run/stop��ť
			 
			System_RTC System_RTC;	                          //ʱ��
			System_RT_IOT_Status RT_IOT_Status;            //iot״̬
			
			//LOGIN_PWD Login_Pwd ;
		} System_RT_Status;

		#pragma pack()

		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN) 
		typedef struct __SYSTEM_PARAMS
		{ 
			//�����ǲ�������Ҫ�����
			Dispense_Params_Item Dispense_Param;
			Pressure_Params Pressure_Param;
			float save_pro_buffer[PRONUM][6];
			Temperature_Humidity Temperature_Humidity_Params; 
			System_IOT_Params IOT_Params;
			LOGIN_PWD Login_Pwd ;
			POWER_STATUS FPASS_AC;  //��һ������Ȩ��
			POWER_STATUS SPASS_AC;  //�ڶ�������Ȩ��
		} System_Params;
		#pragma pack()
		/*-------------------------------------*/
 

extern System_RT_Status Sys_RT_Status;
extern System_Params Sys_Params;

void LoadSystemParams(void);
void SaveSystemParams(void);
void	ResetAllParams(void);		
//POWER_STATUS Match_Pwd(uint8_t viLevel);
#endif