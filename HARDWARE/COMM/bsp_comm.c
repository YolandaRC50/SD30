
#if(1==1) 
#include "bsp_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
//#include "ethernetif.h" 
#include "netif/ethernetif.h" 
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
#include "malloc.h"
#include "delay.h"
#include "usart.h"  
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include <stdio.h>
#include "bsp_ethernet.h"
#endif
//#include "lcd.h" 						  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
//__lwip_dev lwipdev;						//lwip���ƽṹ�� 
struct netif lwip_netif;				//����һ��ȫ�ֵ�����ӿ�
u8_t wfok = 0; 
extern u32 memp_get_memorysize(void);	//��memp.c���涨��
extern u8_t *memp_memory;				//��memp.c���涨��.
extern u8_t *ram_heap;					//��mem.c���涨��.
TaskHandle_t LWIP_DHCP_TaskHandler;
/////////////////////////////////////////////////////////////////////////////////


#if(1==1)  //WIFI_MODE//BLUETOOTH_MODE

WIFI_BLUETOOTH_DMA_UART Wifi_Bluetooth_DMA_Uart; 
 
	
uint8_t Wifi_Bluetooth_DMA_Buffer_RX[IOT_SENDANDRECEIVE_BUFFER_LEN];
uint8_t Wifi_Bluetooth_DMA_Buffer_TX[IOT_SENDANDRECEIVE_BUFFER_LEN]; 

#if(1==1) //Wfif
/*
1 AT+GMR ����������!!������ѯ�汾��Ϣ
2 AT+CWMODE_DEF=1 ������!!����WiFiģ�鹤��ģʽΪ��STAģʽ���������ñ�����flash
3 AT+CWJAP_DEF=��newifi_F8A0��,��anxinke123�� !!��������
4 AT+CWAUTOCONN=1 ������!!ʹ���ϵ��Զ�����AP
3,AT+CIPSTART=��TCP��,��192.168.99.217��,6001 ����!!����TCP������,��ʵ��������������ֽ��в���

5,AT+CIPMODE=1 ����������������������������������!!��ʽ����ʹ��͸��ģʽ��������
6,AT+CIPSEND ������������������������������������!!�������ݣ�Data:Test2��,����ͼ1��
7,���ͣ�+++ ������������������������������������!!�˳�͸����������������+++����Ҫ��ѡ���У�\r\n��,����ͼ��
6,AT+CIPCLOSE ��
*/


//���ڲ�������
void Wifi_Uart_Param(void)
{ 
	WIFI_BLUETOOTH_DMA_UART P_Struct_DMA_Uart;
	
	P_Struct_DMA_Uart.Dma_R_Flag=0;
	P_Struct_DMA_Uart.Dma_T_Flag=0; 
	 
	
	P_Struct_DMA_Uart.Rxd_Fifo =Wifi_Bluetooth_DMA_Buffer_RX;
	P_Struct_DMA_Uart.Txd_Fifo =Wifi_Bluetooth_DMA_Buffer_TX; 
	memcpy(&Wifi_Bluetooth_DMA_Uart,&P_Struct_DMA_Uart, sizeof(P_Struct_DMA_Uart) );
	 
}
//PA2 PA3
void Wifi_Init_Bound(u32 bound)
{
#if(1==0)
	#if 1 
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(WIFI_RCC_AHBxPeriph_GPIOx,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(WIFI_RCC_AHBxPeriph_USARTx,ENABLE);//ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(WIFI_RCC_AHBxPeriph_DMAx,ENABLE);  //DMA1ʱ��ʹ�� 
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(WIFI_GPIOx,WIFI_GPIO_PinSourceRX,WIFI_GPIO_AF_USARTx); //GPIOA2����ΪUSART1
	GPIO_PinAFConfig(WIFI_GPIOx,WIFI_GPIO_PinSourceTX,WIFI_GPIO_AF_USARTx); //GPIOA3����ΪUSART1
 
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = WIFI_GPIO_PINx_RX | WIFI_GPIO_PINx_TX; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(WIFI_GPIOx,&GPIO_InitStructure); //��ʼ��PA2��PA3
 
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(WIFI_USARTx, &USART_InitStructure); //��ʼ������1
 
	USART_Cmd(WIFI_USARTx, ENABLE);  //ʹ�ܴ���1 
 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(WIFI_USARTx, USART_IT_IDLE, ENABLE);//��������ж�
 
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = WIFI_USARTx_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
 
	USART_DMACmd(WIFI_USARTx,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����   
	USART_DMACmd(WIFI_USARTx,USART_DMAReq_Rx,ENABLE);  //ʹ�ܴ���1��DMA����
 #endif
//Dma_Tx();
	//****************************����UART2����
#if 1  //TX
	{
		DMA_DeInit(WIFI_DMAx_Streamx_TX);
		while (DMA_GetCmdStatus(WIFI_DMAx_Streamx_TX) != DISABLE);//�ȴ�DMA������ 
		/* ���� DMA Stream */
		DMA_InitStructure.DMA_Channel = WIFI_DMA_Channel_x;  //ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&WIFI_USARTx->DR;//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr =   (u32)Wifi_Bluetooth_DMA_Uart.Txd_Fifo;//DMA �洢��0��ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
		DMA_InitStructure.DMA_BufferSize =  IOT_SENDANDRECEIVE_BUFFER_LEN;//;//���ݴ����� 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
		
		DMA_Init(WIFI_DMAx_Streamx_TX, &DMA_InitStructure);//��ʼ��DMA Stream
		//DMA NVIC  
		NVIC_InitStructure.NVIC_IRQChannel = WIFI_DMAx_Streamx_IRQn_TX;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure);  
		DMA_ITConfig(WIFI_DMAx_Streamx_TX,DMA_IT_TC,ENABLE);
}
 #endif
	//****************************����UART2����
#if 1  //RT
{
	DMA_DeInit(WIFI_DMAx_Streamx_RX);
	while (DMA_GetCmdStatus(WIFI_DMAx_Streamx_RX) != DISABLE);//�ȴ�DMA������ 
	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = WIFI_DMA_Channel_x;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&WIFI_USARTx->DR;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Wifi_Bluetooth_DMA_Uart.Rxd_Fifo;//(u32)ReceiveBuff;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//RECEIVE_BUF_SIZE;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(WIFI_DMAx_Streamx_RX, &DMA_InitStructure);//��ʼ��DMA Stream
	
	
	//DMA NVIC  
	NVIC_InitStructure.NVIC_IRQChannel = WIFI_DMAx_Streamx_IRQn_RX;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_Cmd(WIFI_DMAx_Streamx_RX, ENABLE);  //����DMA���� 
	DMA_ITConfig(WIFI_DMAx_Streamx_RX,DMA_IT_TC,ENABLE);
}
	#endif
#endif
#if(1)
#if 1 
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);  //DMA1ʱ��ʹ�� 
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA2����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA3����ΪUSART3
 
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA2��PA3
 
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
 
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//��������ж�
 
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
 
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����   
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);  //ʹ�ܴ���1��DMA����
 #endif
	//****************************����UART3����
#if 1
	{
		DMA_DeInit(DMA1_Stream3);
		while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);//�ȴ�DMA������ 
		/* ���� DMA Stream */
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr =   (u32)Wifi_Bluetooth_DMA_Uart.Txd_Fifo;//DMA �洢��0��ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
		DMA_InitStructure.DMA_BufferSize =  IOT_SENDANDRECEIVE_BUFFER_LEN;//;//���ݴ����� 
		
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
		
		DMA_Init(DMA1_Stream3, &DMA_InitStructure);//��ʼ��DMA Stream
		//DMA NVIC  
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure);  
		DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);
}
 #endif
 



	//****************************����UART3����
#if 1
{
	DMA_DeInit(DMA1_Stream1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);//�ȴ�DMA������ 
	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Wifi_Bluetooth_DMA_Uart.Rxd_Fifo;//(u32)ReceiveBuff;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//RECEIVE_BUF_SIZE;//���ݴ����� 
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream
	//DMA NVIC  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_Cmd(DMA1_Stream1, ENABLE);  //����DMA���� 
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
}
	#endif
#endif
}
 
#if 2 //Uart3
//��������ж�
void WIFI_DMAx_Streamx_IRQHandler_TX(void)
{
	wfok = 1;
	//�����־
	if(DMA_GetFlagStatus(WIFI_DMAx_Streamx_TX,WIFI_DMA_FLAG_TCIFx_TX)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(WIFI_DMAx_Streamx_TX,WIFI_DMA_FLAG_TCIFx_TX);//���DMA2_Steam7������ɱ�־
		USART_ITConfig(WIFI_USARTx, USART_IT_TC, ENABLE);//��������ж�
	}
	//
}
//��������ж�
void WIFI_DMAx_Streamx_IRQHandler_RX(void)
{
	//�����־
	if(DMA_GetFlagStatus(WIFI_DMAx_Streamx_RX,WIFI_DMA_FLAG_TCIFx_RX)!=RESET)//�ȴ�DMA1_Steam5�������
	{ 
		Wifi_Bluetooth_DMA_Uart.Rxd_L =IOT_SENDANDRECEIVE_BUFFER_LEN - DMA_GetCurrDataCounter(WIFI_DMAx_Streamx_RX);
		Wifi_Bluetooth_DMA_Uart.Dma_R_Flag =1;
		
		DMA_Cmd(WIFI_DMAx_Streamx_RX, DISABLE); //�ر�DMA,��ֹ�������������
		DMA_ClearFlag(WIFI_DMAx_Streamx_RX,WIFI_DMA_CLEARFLAG_RX);//���DMA1_Steam5������ɱ�־
		DMA_SetCurrDataCounter(WIFI_DMAx_Streamx_RX, IOT_SENDANDRECEIVE_BUFFER_LEN);
		DMA_Cmd(WIFI_DMAx_Streamx_RX, ENABLE);     //��DMA,
		
    OS_UARTx_THREAD_GET();
//		osSignalSet( UART1_GET, 1 );
	}
}

//�����ж�
void WIFI_USARTx_IRQHandler(void)
{
	u16 data;
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_TC) != RESET)
	{
		USART_ClearFlag(WIFI_USARTx, USART_IT_TC);
		USART_ITConfig(WIFI_USARTx, USART_IT_TC, DISABLE);//���ж�
		Wifi_Bluetooth_DMA_Uart.Dma_T_Flag =0;
	}
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_IDLE) != RESET)
	{
		data = WIFI_USARTx->SR;
		data = WIFI_USARTx->DR;
		
		Wifi_Bluetooth_DMA_Uart.Rxd_L =IOT_SENDANDRECEIVE_BUFFER_LEN - DMA_GetCurrDataCounter(WIFI_DMAx_Streamx_RX);
		Wifi_Bluetooth_DMA_Uart.Dma_R_Flag =1;
		
		DMA_Cmd(WIFI_DMAx_Streamx_RX, DISABLE); //�ر�DMA,��ֹ�������������
		DMA_ClearFlag(WIFI_DMAx_Streamx_RX,WIFI_DMA_CLEARFLAG_RX);//���DMA2_Steam7������ɱ�־
		DMA_SetCurrDataCounter(WIFI_DMAx_Streamx_RX, IOT_SENDANDRECEIVE_BUFFER_LEN);
		DMA_Cmd(WIFI_DMAx_Streamx_RX, ENABLE);     //��DMA,
		
    OS_UARTx_THREAD_GET();
//		os_Uart2_Thread_Get();
	}
} 
#endif
void Wifi_Init()
{
	
	Wifi_Uart_Param();
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{
			Wifi_Init_Bound(WIFI_MODE_BOUND);
			break;
		}
		case ETHERNET_MODE:
		{
			Wifi_Init_Bound(WIFI_MODE_BOUND);    //yolanda
			break;
		} 
		default:break;
	}
}

//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void Wifi_DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}	 
uint8_t Wifi_Send_Dma( WIFI_BLUETOOTH_DMA_UART *I_Struct_DMA_Uart )
{ 
	WIFI_BLUETOOTH_DMA_UART *P_DMA_Uart=I_Struct_DMA_Uart; 
	Wifi_DmaSendDataProc(WIFI_DMAx_Streamx_TX,P_DMA_Uart->Txd_L); //��ʼһ��DMA����  
	return 1;
}
int16_t Wifi_Send(char *str)
{ 
		strcpy((char *)Wifi_Bluetooth_DMA_Uart.Txd_Fifo, str);
		Wifi_Bluetooth_DMA_Uart.Txd_L =strlen(str);
    puts(str);
		Wifi_Send_Dma(&Wifi_Bluetooth_DMA_Uart);
 }


#endif
#endif 


#if(1==1)  //ETHERNET_MODE
//lwip����������(�ں������DHCP����)
 
//lwip DHCP����
//�����������ȼ�
#define LWIP_DHCP_TASK_PRIO       		4
//���������ջ��С
#define LWIP_DHCP_STK_SIZE  		   	512
//������
void lwip_dhcp_task(void *pdata); 
 
//������̫���жϵ���
void lwip_pkt_handle(void)
{
	ethernetif_input(&lwip_netif);
}
 
//lwip��mem��memp���ڴ�����
//����ֵ:0,�ɹ�;
//    ����,ʧ��
u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
    INTX_DISABLE();//���ж�
	mempsize=memp_get_memorysize();			//�õ�memp_memory�����С
	memp_memory=mymalloc(SRAMIN,mempsize);	//Ϊmemp_memory�����ڴ�
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//�õ�ram heap��С
	ram_heap=mymalloc(SRAMIN,ramheapsize);	//Ϊram_heap�����ڴ�   
	//TCPIP_THREAD_TASK_STK=mymalloc(SRAMIN,TCPIP_THREAD_STACKSIZE*4);//���ں����������ջ,����FreeRTOS���������ʱ��ʹ���˶�̬����������Ǿ�̬��������Ҫ�����ջ
	//LWIP_DHCP_TASK_STK=mymalloc(SRAMIN,LWIP_DHCP_STK_SIZE*4);		  //��dhcp�����ջ�����ڴ�ռ�
    INTX_ENABLE();//���ж�
	if(!memp_memory||!ram_heap)//������ʧ�ܵ�
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip��mem��memp�ڴ��ͷ�
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMIN,memp_memory);
	myfree(SRAMIN,ram_heap);
}
//lwip Ĭ��IP����
//lwipx:lwip���ƽṹ��ָ��
void lwip_comm_default_ip_set()
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//��ȡSTM32��ΨһID��ǰ24λ��ΪMAC��ַ�����ֽ� 
	//MAC��ַ����(�����ֽڹ̶�Ϊ:2.0.0,�����ֽ���STM32ΨһID)
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[0]=2;//�����ֽ�(IEEE��֮Ϊ��֯ΨһID,OUI)��ַ�̶�Ϊ:2.0.0
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[1]=0;
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[2]=0;
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[3]=(sn0>>16)&0XFF;//�����ֽ���STM32��ΨһID
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[4]=(sn0>>8)&0XFFF;
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[5]=sn0&0XFF;  
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.dhcpstatus=0;//û��DHCP	 
} 
 
//LWIP��ʼ��(LWIP������ʱ��ʹ��)
//����ֵ:0,�ɹ�
//      1,�ڴ����
//      2,LAN8720��ʼ��ʧ��
//      3,�������ʧ��.
u8 lwip_comm_init(void)
{
    u8 retry=0;
	struct netif *Netif_Init_Flag;		//����netif_add()����ʱ�ķ���ֵ,�����ж������ʼ���Ƿ�ɹ� 
 
	if(ETH_Mem_Malloc())return 1;		//�ڴ�����ʧ��
	if(lwip_comm_mem_malloc())return 2;	//�ڴ�����ʧ��
	puts("1");
	  
	if(LAN8720_Init())		        //��ʼ��LAN8720,���ʧ�ܵĻ�������5��
    {
//			puts("2");
//        retry++;
//        if(retry>5) {retry=0;return 3;} //LAN8720��ʼ��ʧ��
			return 3;
    }
	puts("2");
	tcpip_init(NULL,NULL);				//��ʼ��tcp ip�ں�,�ú�������ᴴ��tcpip_thread�ں�����
	lwip_comm_default_ip_set();	//����Ĭ��IP����Ϣ
 
	taskENTER_CRITICAL();
#if LWIP_DHCP		//ʹ�ö�̬IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//ʹ�þ�̬IP
	Netif_Init_Flag=netif_add(
		&lwip_netif,
		&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Loca_IPAddr,
		&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Net_Mask,
		&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Gate_way,
		NULL,
		&ethernetif_init,
		&tcpip_input
		);//�������б������һ������ 
#endif
	taskEXIT_CRITICAL();  //�˳��ٽ���
	if(Netif_Init_Flag==NULL)return 4;//�������ʧ�� 
	else//������ӳɹ���,����netifΪĬ��ֵ,���Ҵ�netif����
	{
		netif_set_default(&lwip_netif); //����netifΪĬ������
		netif_set_up(&lwip_netif);		//��netif����
	} 
	return 0;//����OK.
}   
//���ʹ����DHCP 

#endif
