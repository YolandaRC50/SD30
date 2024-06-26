
#include "BSP.h"

#include "malloc.h"
//FATFS fs;

void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	
	delay_init(168);
	//delay_ms(1000);
	TIMx_Init();
	TIM4_Init();
	//
	//Pressure Sensor
	PressureSensor_Init();   
	//HMI
	HMI_USART_Init();
	  
	BELL_Init();
	LED_Init();

	AT24CXX_Init();   //初始化存储芯片M24C04
	W25QXX_Init();    //初始化flashW25Q128
	//ResetAllParams();
	//SaveSystemParams();
	
	LoadSystemParams();
	
	//HARDWARE/GPIO
	GOIO_Init();
	
	my_mem_init(SRAMIN);            	//初始化内部内存池
//	my_mem_init(SRAMEX);		        //初始化外部内存池
	my_mem_init(SRAMCCM);		          //初始化CCM内存池
	
	//HARDWARE/SENSOR
	DHTxx_Init();
	
	Communication_Param_Init();
 
	
	//Files
	

	
	#if WDG_ENABLE//使能看门狗功能
	IWDG_Init(4,10000);
	#endif
}
