#include "Key.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = KEY_Pin1 | KEY_Pin2; //��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_Init(KEY_Port, &GPIO_InitStructure);//��ʼ��GPIOE2,3
}

void KEY_EXTI_Init(void)
{	
	//KEY_Init(); //������Ӧ��IO�ڳ�ʼ��
}

