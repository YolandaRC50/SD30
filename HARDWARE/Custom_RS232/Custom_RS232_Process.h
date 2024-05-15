#ifndef _CUSTOM_RS232_PROCESS_H
#define _CUSTOM_RS232_PROCESS_H
#include "includes.h"
#include  "stm32f4xx.h"
#include "custom_rs232.h"

#if(1==1) /*Process*/
#define CUSTOM_RS232_CMD_MAX_SIZE 32           // ����ָ���С��������Ҫ�������������ô�һЩ
#define CUSTOM_RS232_QUEUE_MAX_SIZE 64        // ָ����ջ�������С��������Ҫ�������������ô�һЩ

#define CUSTOM_RS232_PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define CUSTOM_RS232_PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])  //�ӻ�����ȡ32λ����

 
typedef unsigned char Custom_rs232_qdata;
typedef unsigned short Custom_rs232_qsize; 

typedef enum 
{
	LEVEL_DETECT_SYSTEM=0,
	ROBOT_OR_PLC_SYSTEM=1,
} CUSTOM_RS232_CON_DIV;



typedef enum 
{
	CUSTOM_RS232_CMD_HANDSHAKE 		=01,/*����*/ 
	
	CUSTOM_RS232_CMD_REQUEST_RESPONSE 		=03,/*֪ͨ/Ӧ��*/
	
}CUSTOM_RS232_CMD_TYPE;

typedef enum 
{
	/**/
	CUSTOM_RS232_CTR_MSG_NONE=0x00,
	/*HANDSHAKE*/
	CUSTOM_RS232_CTR_MSG_HANDSHAKE =0x01,
	
	/*RESSURE_CAL*/
	CUSTOM_RS232_CTR_MSG_RESSURE_CAL_0BAR=0x10,
	CUSTOM_RS232_CTR_MSG_RESSURE_CAL_2BAR=0x11,
	
	/*CARTRIDGE CAL AND DETECT*/
	CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_EMPTY=0x20,
	CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_FULL=0x21,
	CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT=0x22,
	/*PARAMS*/
	CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT_PARAMS=0x30,
	
	/*CAL AND DETECT*/
	CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_EMPTY_FINISHED=0x40,
	CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_FULL_FINISHED=0x41,
	CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_DETECT_FINISHED=0x42,
	
	
	
	/*PC OR PLC REQUEST*/ 
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_STATUS=0x50,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_X=0x51,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Y=0x52,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Z=0x53,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_CT=0x54,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_PT=0x55,
	
	
	
	
	
	/*PC OR PLC RESPONSE*/  
	CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_STATUS=0x60,
	CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_X=0x61,
	CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Y=0x62,
	CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Z=0x63,
	CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_CT=0x64,
	CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_PT=0x65,
	
	/*PC OR PLC PARAMS SET*/  
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_X=0x71,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Y=0x72,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Z=0x73,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_CT=0x74,
	CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_PT=0x75,
	
	/*STATUS*/
	CUSTOM_RS232_CTR_MSG_PRODUCT_STSTUS=0xFF, 
	
}CUSTOM_RS232_CTR_MSG;

#define CUSTOM_RS232_CMD_HEAD 0XEE                                                  //֡ͷ
#define CUSTOM_RS232_CMD_TAIL 0XFFFCFFFF                                            //֡β

typedef struct _CUSTOM_RS232_QUEUE                                             
{                                                                 
     Custom_rs232_qsize _head;                                                       //����ͷ
     Custom_rs232_qsize _tail;                                                       //����β
     Custom_rs232_qdata _data[CUSTOM_RS232_QUEUE_MAX_SIZE];                                       //�������ݻ�����
}CUSTOM_RS232_QUEUE;                                                           

extern  CUSTOM_RS232_QUEUE Custom_rs232_que ;                                            //ָ�����
extern uint32_t Custom_rs232_cmd_state ;                                           //����֡β���״̬
extern Custom_rs232_qsize Custom_rs232_cmd_pos;                                              //��ǰָ��ָ��λ�� 
CARTRIDGE_STATUS_ENUM ;

#pragma pack(push)
#pragma pack(1)	//���ֽڶ���
typedef struct
{
	uint8_t    cmd_head;  	//֡ͷ
	uint8_t    cmd_type;  
	 
	uint8_t    Con_div;
	
	uint8_t    ctrl_msg;   
	
	uint8_t    param[256];	//�ɱ䳤�Ȳ��������256���ֽ� 
	
	uint8_t cmd_tail[4];   	//֡β
}CUSTOM_RS232_MSG,*PCUSTOM_RS232_MSG;

#pragma pack(pop)

#endif


 

#if(1==1) /*��׼����*/
#define CUSTOM_TX_8(P1) CUSTOM_SEND_DATA((P1)&0xFF)                    					        //���͵����ֽ�
#define CUSTOM_TX_8N(P,N) CUSTOM_SENDNU8((uint8_t *)P,N)                 					//����N���ֽ�
#define CUSTOM_TX_16(P1) CUSTOM_TX_8((P1)>>8);CUSTOM_TX_8(P1)                 			//����16λ����
#define CUSTOM_TX_16N(P,N) CUSTOM_SENDNU16((uint16_t *)P,N)              					//����N��16λ����
#define CUSTOM_TX_32(P1) CUSTOM_TX_16((P1)>>16);CUSTOM_TX_16((P1)&0xFFFF)     	//����32λ����
#define CUSTOM_SEND_DATA(P) Custom_RS232_SendChar(P)          //����һ���ֽ�
#define CUSTOM_BEGIN_CMD(void) CUSTOM_TX_8(0XEE)            //֡ͷ
#define CUSTOM_END_CMD(void) CUSTOM_TX_32(0XFFFCFFFF)       //֡β
	
void Custom_rs232_ProcessMessage( PCUSTOM_RS232_MSG msg, uint16_t size );
void Custom_rs232_queue_reset(void);
void Custom_rs232_queue_push(Custom_rs232_qdata _data);
Custom_rs232_qsize Custom_rs232_queue_find_cmd(Custom_rs232_qdata *cmd,Custom_rs232_qsize buf_len);

void CUSTOM_SENDNU8(uint8_t *pData,uint16_t nDataLen);
void CUSTOM_SENDNU16(uint16_t *pData,uint16_t nDataLen);

#endif

typedef struct
{
	CARTRIDGE_STATUS_ENUM Main_Status;
	
	POWER_STATUS Vavel_Status_IN;
	POWER_STATUS Vavel_Status_Out;
	POWER_STATUS Vavel_Status_Relief;
	
	float Pressure_Bar;
	float Pressure_AN;
	float LastTestValue;
	
	CUSTOM_RS232_CTR_MSG The_Last_MSG;
	
}CARTRIDGE_STATUS_FROM_DETECT_SYSTEM;

extern POWER_STATUS Custom_Rs232_CMD_HandShake_Status;
extern CARTRIDGE_STATUS_FROM_DETECT_SYSTEM Level_Detect_Status;

#if(1==1)
extern CUSTOM_RS232_CON_DIV Custom_RS232_Con_Div;
extern CUSTOM_RS232_CTR_MSG SendToServerMessage; 
extern POWER_STATUS DIspensingFinished_Mark;
extern POWER_STATUS System_Power_Level_Detect_Status; 
void Custom_rs232_APP(void);
void Custom_rs232_Send_HandShake(CUSTOM_RS232_CON_DIV div);
void Update_Status(void);
void Cartridge_CalLibrationAndDetect_Finished(uint8_t voMode);
void DispensingFinished(void);
void Custom_rs232_Request_Form_PC_PLC(CUSTOM_RS232_CTR_MSG msg,uint8_t *Params);
void Custom_rs232_Response_To_PC_PLC(CUSTOM_RS232_CTR_MSG msg);
void Custom_rs232_Set_Params_Form_PC_PLC(CUSTOM_RS232_CTR_MSG msg,float vfParams);
#endif
#endif

