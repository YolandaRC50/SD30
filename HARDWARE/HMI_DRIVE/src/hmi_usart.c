 
#include "hmi_usart.h"



void HMI_USART_Init()
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(HMI_RCC_AHB1Periph_GPIOx,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(HMI_RCC_APB2Periph_USARTx,ENABLE);//ʹ��USART2ʱ��

 	USART_DeInit(HMI_USARTx);  //��λ����2
	
	GPIO_PinAFConfig(HMI_GPIOx,HMI_GPIO_PinSource_TX,HMI_GPIO_AF_USARTx); //GPIOD6����ΪUSART2
	GPIO_PinAFConfig(HMI_GPIOx,HMI_GPIO_PinSource_RX,HMI_GPIO_AF_USARTx); //GPIOD5����ΪUSART2	
	
  GPIO_InitStructure.GPIO_Pin = HMI_GPIO_Pin_TX | HMI_GPIO_Pin_RX; //GPIOD6GPIOD5��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(HMI_GPIOx,&GPIO_InitStructure); //��ʼ��GPIOD6��GPIOD5
	
	USART_InitStructure.USART_BaudRate = HMI_BOUND;//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(HMI_USARTx, &USART_InitStructure); //��ʼ������2
 
	USART_Cmd(HMI_USARTx, ENABLE);               //ʹ�ܴ��� 
	
  USART_ITConfig(HMI_USARTx, USART_IT_RXNE, ENABLE);//�����ж�   
	
	NVIC_InitStructure.NVIC_IRQChannel = HMI_USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=12;//��ռ���ȼ�8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}

#if 1
void HMI_USARTx_IRQHandler(void)
{
	if (USART_GetFlagStatus(HMI_USARTx, USART_FLAG_PE) != RESET)  
	{  
		USART_ClearFlag(HMI_USARTx, USART_FLAG_PE);  
	}  

	if(USART_GetFlagStatus(HMI_USARTx, USART_FLAG_ORE) != RESET)  
	{  
		USART_ClearFlag(HMI_USARTx, USART_FLAG_ORE);  
	}  

	if (USART_GetFlagStatus(HMI_USARTx, USART_FLAG_FE) != RESET)  
	{   
		USART_ClearFlag(HMI_USARTx, USART_FLAG_FE);  
	}

	if(USART_GetITStatus(HMI_USARTx, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		uint8_t data = USART_ReceiveData(HMI_USARTx);
		queue_push(data);
		USART_ClearFlag(HMI_USARTx, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(HMI_USARTx, USART_IT_RXNE);		
	}										 
}
#endif
void HMI_SendSingleChar(uint8_t dat)
{ 
	   USART_SendData(HMI_USARTx,dat);
	    while(USART_GetFlagStatus(HMI_USARTx, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(HMI_USARTx, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
}
