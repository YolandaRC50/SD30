#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "includes.h"
#include "lan8720.h"  
 
 
 #if(1==1) //WIFI_MODE//BLUETOOTH_MODE

	#define WIFI_SEND(x)      Wifi_Send(x)

  #define OS_UARTx_THREAD_GET() xEventGroupSetBits(Event_Handle, (1)<<0)	
	#define  EVENT_HANDLE_BLUE   ((1)<<1)
	#define  EVENT_HANDLE_WIFI   ((1)<<1)
	
  #define WIFI_MODE_BOUND 					115200

 
   #if(1==1) //WIFI_MODE//
  #define WIFI_RCC_AHBxPeriph_GPIOx RCC_AHB1Periph_GPIOB
  #define WIFI_RCC_AHBxPeriph_USARTx RCC_APB1Periph_USART3
	#define WIFI_RCC_AHBxPeriph_DMAx RCC_AHB1Periph_DMA1
	
	
	#define WIFI_GPIOx GPIOB
	#define WIFI_GPIO_PinSourceRX GPIO_PinSource11
	#define WIFI_GPIO_PinSourceTX GPIO_PinSource10
	#define WIFI_GPIO_AF_USARTx	GPIO_AF_USART3 
	#define WIFI_GPIO_PINx_RX GPIO_Pin_11
	#define WIFI_GPIO_PINx_TX GPIO_Pin_10
	#define WIFI_USARTx USART3
	#define WIFI_USARTx_IRQn USART3_IRQn
	//#define WIFI_Uartx_Struct_DMA WIFI_DMA_Uart
	#define WIFI_DMA_Channel_x DMA_Channel_4
	#define WIFI_USARTx_IRQHandler USART3_IRQHandler
	
	//TX
	#define WIFI_DMAx_Streamx_TX DMA1_Stream3 
	#define WIFI_DMA_FLAG_TCIFx_TX DMA_FLAG_TCIF3
	#define WIFI_DMAx_Streamx_IRQn_TX  DMA1_Stream3_IRQn
	#define WIFI_DMAx_Streamx_IRQHandler_TX   DMA1_Stream3_IRQHandler
	//RX 
	#define WIFI_DMAx_Streamx_RX DMA1_Stream1
	#define WIFI_DMA_FLAG_TCIFx_RX DMA_FLAG_TCIF1
	#define WIFI_DMA_CLEARFLAG_RX DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1
	#define WIFI_DMAx_Streamx_IRQn_RX  DMA1_Stream1_IRQn 
	#define WIFI_DMAx_Streamx_IRQHandler_RX   DMA1_Stream1_IRQHandler
	
 #endif

	

	//typedef enum {Uart1 = 1, Uart2, Uart3, Uart4, Uart5} Function_Uart;

	typedef struct
	{
		uint8_t *Rxd_Fifo;
		uint16_t Rxd_L;
			
		uint8_t *Txd_Fifo;
		uint16_t Txd_L;
		
		uint8_t Dma_R_Flag, Dma_T_Flag;	 
		
	} WIFI_BLUETOOTH_DMA_UART;	

 #endif
 

 #if(1==1) //ETHERNET_MODE
	#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数
 #endif
   

 #if(1==1) //WIFI_MODE//BLUETOOTH_MODE
	 
	extern EventGroupHandle_t Event_Handle;
	extern WIFI_BLUETOOTH_DMA_UART   Wifi_Bluetooth_DMA_Uart;  

 //uint8_t Send_Data_Dma( WIFI_BLUETOOTH_DMA_UART *I_Struct_DMA_Uart );
 #endif
 

//extern __lwip_dev lwipdev;	//lwip控制结构体
 
//------------------------------------

 #if(1==1) //WIFI_MODE
 
  void	Wifi_Init(void);
	uint8_t Wifi_Send_Dma( WIFI_BLUETOOTH_DMA_UART *I_Struct_DMA_Uart );
  int16_t Wifi_Send(char *str);
 #endif



//-------------------------------------

 #if(1==1) //ETHERNET_MODE
void lwip_pkt_handle(void);
void lwip_comm_default_ip_set(void);
u8 lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
u8 lwip_comm_init(void);
void lwip_comm_dhcp_creat(void);
void lwip_comm_dhcp_delete(void);
void lwip_comm_destroy(void);
void lwip_comm_delete_next_timeout(void);


static void vDispensing_Task(void *pvParameters);
static void vDispensing_Task(void *pvParameters);
 #endif
#endif













