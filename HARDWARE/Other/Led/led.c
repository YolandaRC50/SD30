#include "Led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LED_RCC_AHB1Periph_GPIOx, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_Pin_ALM | LED_Pin_RUN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(LED_Port, &GPIO_InitStructure);
	LED_Work(0);
}

void LED_Work(uint8_t cmd)
{
	if(cmd)
	{
		GPIO_SetBits(LED_Port, LED_Pin_RUN);
		GPIO_ResetBits(LED_Port, LED_Pin_ALM);
	}
	else
	{
		GPIO_ResetBits(LED_Port, LED_Pin_RUN);
		GPIO_SetBits(LED_Port, LED_Pin_ALM);
	}
}



