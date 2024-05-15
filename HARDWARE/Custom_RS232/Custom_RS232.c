#include "Custom_RS232.h"
#include "Custom_Rs232_Process.h"



/*!
*   \brief   ���ڳ�ʼ�� 
*/
void Custom_RS232_Init()
{
	
	NVIC_InitTypeDef NVIC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG/* | RCC_APB2Periph_AFIO*/, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_USARTx, ENABLE);
  	//GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable, ENABLE );

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =CUSTOM_RS232_TX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CUSTOM_RS232_PORT, &GPIO_InitStructure);

	//  /* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = CUSTOM_RS232_RX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CUSTOM_RS232_PORT, &GPIO_InitStructure);
	
	USART_DeInit(CUSTOM_RS232);                                           //��λUSART1

	USART_InitStructure.USART_BaudRate = CUSTOM_RS232_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =   USART_Mode_Tx|USART_Mode_Rx;

	/* USART configuration */
	USART_Init(CUSTOM_RS232, &USART_InitStructure);
	USART_ITConfig(CUSTOM_RS232, USART_IT_RXNE, ENABLE);                  //�����ж�ʹ��

	/* Enable USART */
	USART_Cmd(CUSTOM_RS232, ENABLE);


	/*!
	CPU��Сȱ�ݣ��������úã����ֱ��Send��
	���1���ֽڷ��Ͳ���ȥ �����������1���ֽ��޷���ȷ���ͳ�ȥ������
	*/
	USART_ClearFlag(CUSTOM_RS232, USART_FLAG_TC);
	//USART_ClearFlag(CUSTOM_RS232, USART_FLAG_TXE);

	NVIC_InitStructure.NVIC_IRQChannel = CUSTOM_RS232_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	

}

/*!
*   \brief  ����1���ֽ�
*   \param  t ���͵��ֽ�
*/
void  Custom_RS232_SendChar(unsigned char  t)
{
	    USART_SendData(CUSTOM_RS232,t);
	    while(USART_GetFlagStatus(CUSTOM_RS232, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(CUSTOM_RS232, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
	    //DelayMS(1);
}


/*****************************************************************
* ��    �ƣ� USART2_IRQHandler()
* ��    �ܣ� USART2_IRQHandler�����ж�
* ��ڲ����� ��
* ���ڲ����� ��
*****************************************************************/
void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(CUSTOM_RS232, USART_FLAG_PE) != RESET)  
	  {   
	       //USART_ReceiveData(CUSTOM_RS232);
	     USART_ClearFlag(CUSTOM_RS232, USART_FLAG_PE);  
	  }  
      
	  if(USART_GetFlagStatus(CUSTOM_RS232, USART_FLAG_ORE) != RESET)  
	  {  
	      // USART_ReceiveData(CUSTOM_RS232);
	     USART_ClearFlag(CUSTOM_RS232, USART_FLAG_ORE);  
	  }  
      
	  if (USART_GetFlagStatus(CUSTOM_RS232, USART_FLAG_FE) != RESET)  
	  {   
	      // USART_ReceiveData(CUSTOM_RS232);
	     USART_ClearFlag(CUSTOM_RS232, USART_FLAG_FE);  
	  }
	
	if(USART_GetITStatus(CUSTOM_RS232, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		 USART_ClearFlag(CUSTOM_RS232, USART_FLAG_RXNE);  
		 USART_ClearITPendingBit(CUSTOM_RS232, USART_IT_RXNE);		
		
		 uint8_t data = USART_ReceiveData(CUSTOM_RS232);
		/*   */
		// queue_push(data);
		
		 Custom_rs232_queue_push(data);
	}										 
}

