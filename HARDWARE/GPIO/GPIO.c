
#include "gpio.h" 
POWER_STATUS  FORCE_ENABLE=POWER_OFF;

extern u8_t footpress_flag;
extern u8_t footswitch_flag;
#if(1)
GPIO GPIO_IN[GP_I_LEN]=
{
	{DI_CLK_0,DI_PORT_0,DI_PIN_0,DI_PuPd_Type_0},     //��̤����
	{DI_CLK_1,DI_PORT_1,DI_PIN_1,DI_PuPd_Type_1},     //plc�㽺����
	{DI_CLK_2,DI_PORT_2,DI_PIN_2,DI_PuPd_Type_2},      //  plc������������pressureenable
	{DI_CLK_3,DI_PORT_3,DI_PIN_3,DI_PuPd_Type_3},      //PLC����runstop
	{DI_CLK_4,DI_PORT_4,DI_PIN_4,DI_PuPd_Type_4},     //PLC����PURGE
	{DI_CLK_5,DI_PORT_5,DI_PIN_5,DI_PuPd_Type_5},      //����
	{DI_CLK_6,DI_PORT_5,DI_PIN_5,DI_PuPd_Type_5},      //����

};

GPIO GPIO_OUT[GP_O_LEN]=
{
	{DO_CLK_0,DO_PORT_0,DO_PIN_0,DO_PuPd_Type_0,0},
	{DO_CLK_1,DO_PORT_1,DO_PIN_1,DO_PuPd_Type_1,0},
	{DO_CLK_2,DO_PORT_2,DO_PIN_2,DO_PuPd_Type_2,0},
	{DO_CLK_3,DO_PORT_3,DO_PIN_3,DO_PuPd_Type_3,0},
	{DO_CLK_4,DO_PORT_4,DO_PIN_4,DO_PuPd_Type_4,0},
	{DO_CLK_5,DO_PORT_5,DO_PIN_5,DO_PuPd_Type_5,0},
	{DO_CLK_6,DO_PORT_6,DO_PIN_6,DO_PuPd_Type_6,0},
	{DO_CLK_7,DO_PORT_7,DO_PIN_7,DO_PuPd_Type_7,0}, 
	{DO_CLK_8,DO_PORT_8,DO_PIN_8,DO_PuPd_Type_8,0},     
};
DI_Exti DI_Exti_List[DI_EXTI_LEN]=
{
	{DI_FOOT_SWITCH_1_DI_Index,DI_FOOT_SWITCH_1_EXTI_PortSourceGPIOx,DI_FOOT_SWITCH_1_EXTI_PinSourcex,DI_FOOT_SWITCH_1_EXTIx_IRQn,DI_FOOT_SWITCH_1_EXTI_LineX},
	{DI_CHANNEL_SWITCH_1_DI_Index,DI_CHANNEL_SWITCH_1_EXTI_PortSourceGPIOx,DI_CHANNEL_SWITCH_1_EXTI_PinSourcex ,DI_CHANNEL_SWITCH_1_EXTIx_IRQn ,DI_CHANNEL_SWITCH_1_EXTI_LineX },
	{DI_PURGE_SWITCH_1_DI_Index,DI_PURGE_SWITCH_1_EXTI_PortSourceGPIOx,DI_PURGE_SWITCH_1_EXTI_PinSourcex,DI_PURGE_SWITCH_1_EXTIx_IRQn,DI_PURGE_SWITCH_1_EXTI_LineX},
};



void GOIO_IN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	uint32_t _CLK=0;
	for(uint8_t index=0;index<GP_I_LEN;index++)
	{
		_CLK|=GPIO_IN[index].DO_CLKx;
	}
	
	RCC_AHB1PeriphClockCmd(_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	 
	for(uint8_t index = 0; index < GP_I_LEN; index++)
	{ 
		GPIO_InitStructure.GPIO_PuPd = GPIO_IN[index].GPIOPuPd_Type;	
		GPIO_InitStructure.GPIO_Pin = GPIO_IN[index].GPIO_Pinx;
		GPIO_Init(GPIO_IN[index].GPIOx, &GPIO_InitStructure);
	}
}
void GOIO_OUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t _CLK=0;
	for(uint8_t index=0;index<GP_O_LEN;index++)
	{
		_CLK|=GPIO_OUT[index].DO_CLKx;
	}
	
	RCC_AHB1PeriphClockCmd(_CLK, ENABLE);
	
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	////GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	 
	for(uint8_t index = 0; index < GP_O_LEN; index++)
	{ 
		GPIO_InitStructure.GPIO_Pin = GPIO_OUT[index].GPIO_Pinx;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIO_OUT[index].GPIOx, &GPIO_InitStructure); 
	
		GPIO_SetBits(GPIO_OUT[index].GPIOx,GPIO_OUT[index].GPIO_Pinx);  //��������Ӧ����GPIOF8���ͣ� 
	}
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOG, &GPIO_InitStructure);
		GPIO_SetBits(GPIOG,GPIO_Pin_1);
	
	

}


void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ�� 
	for(int i=0;i<DI_EXTI_LEN;i++)
	{
		SYSCFG_EXTILineConfig(DI_Exti_List[i].EXTI_PortSourceGPIOx,DI_Exti_List[i].EXTI_PinSourcex);
		
		EXTI_InitStructure.EXTI_Line=DI_Exti_List[i].EXTI_LineX;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�������½��ش���																													//
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
		EXTI_Init(&EXTI_InitStructure);//����
		
			//
		NVIC_InitStructure.NVIC_IRQChannel = DI_Exti_List[i].EXTIx_IRQn;//�ⲿ�ж�2
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = i;//��ռ���ȼ�3
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure);//����
	}  
}

void EXTI_IRQHandler(void)
{
	uint32_t EXTI_LineX = EXTI->PR;       //ȡ�����Ǹ����ŵ��ж� 
	for(int i=0;i<DI_EXTI_LEN;i++)
	{
		if(EXTI_LineX==DI_Exti_List[i].EXTI_LineX)
		{ 	
			if(EXTI_GetITStatus(EXTI_LineX) == SET)
			{ 
				//footpress_flag = 1;
				//footswitch_flag ++;	
				Refresh_GPIO_Value();	 
				//Dispensing_Logic();
				EXTI_ClearITPendingBit(EXTI_LineX);//���LINE�ϵ��жϱ�־λ 
			}
		}
	} 
}
void EXTI0_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI1_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI2_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI3_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI4_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI9_5_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI15_10_IRQHandler(void)
{
	EXTI_IRQHandler();
}

#endif
 

void GOIO_Init(void)
{
	GOIO_IN_Init();
	GOIO_OUT_Init();
	EXTIX_Init();
}

void Refresh_GPIO_Value()
{
	for(uint8_t index=0;index<GP_I_LEN;index++)
	{
		GPIO_IN[index].GPIO_Value=1-GPIO_ReadInputDataBit(GPIO_IN[index].GPIOx, GPIO_IN[index].GPIO_Pinx);	 
	}
	for(uint8_t index=0;index<GP_O_LEN;index++)
	{ 
		
		if(GPIO_OUT[index].GPIO_Value) 
		{
			GPIO_ResetBits(GPIO_OUT[index].GPIOx, GPIO_OUT[index].GPIO_Pinx); 
		}
		else 
		{
			GPIO_SetBits(GPIO_OUT[index].GPIOx, GPIO_OUT[index].GPIO_Pinx);
		} 
	}
 
}




void Dispensing(POWER_STATUS Power)
{

		 GPIO_OUT[DO_PRESSURE_VALVE].GPIO_Value=Power;

}
void VACUUMENABLE(POWER_STATUS Power)
{

		 GPIO_OUT[DO_VACUUM_VALVE].GPIO_Value=Power;
}

POWER_STATUS Check_Dispensing_IO_Status()
{
	POWER_STATUS _Power_Status=POWER_OFF;

		_Power_Status=(GPIO_IN[DI_FOOT_SWITCH].GPIO_Value)||(GPIO_IN[DI_DISPENSE_SWITCH].GPIO_Value);


	return _Power_Status;
}


