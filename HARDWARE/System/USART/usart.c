#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "cmd_queue.h"
#include "sys_timer.h"
#include "bell.h"  

uint8_t moedbus_busy = 0;
//���ڷ��ͻ����� 	
__align(8) uint8_t USART3_TX_BUF[USART_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
__align(8) uint8_t USART6_TX_BUF[USART_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�  
//���ڽ��ջ����� 	
uint8_t USART3_RX_BUF[USART_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
uint8_t USART6_RX_BUF[USART_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.

uint16_t USART3_RX_STA=0;   	 
//void USART3_IRQHandler(void)
//{	   
//	uint8_t res = 0;
//	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
//	{  
//		USART_ClearFlag(USART3, USART_FLAG_PE);  
//	}  

//	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
//	{   
//		USART_ClearFlag(USART3, USART_FLAG_ORE);  
//	}  

//	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
//	{  
//		USART_ClearFlag(USART3, USART_FLAG_FE);  
//	}

//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
//	{
//		res =USART_ReceiveData(USART3);//;��ȡ���յ�������USART2->DR
//		if(USART3_RX_STA< USART_MAX_RECV_LEN)
//		{
//			USART3_RX_BUF[USART3_RX_STA]=res;		//��¼���յ���ֵ
//			USART3_RX_STA++;						//������������1 
//		}
//		USART_ClearFlag(USART3, USART_FLAG_RXNE);  
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		
//	}										 
//}  
	 
void USART3_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
	{  
		USART_ClearFlag(USART3, USART_FLAG_PE);  
	}  

	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
	{  
		USART_ClearFlag(USART3, USART_FLAG_ORE);  
	}  

	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
	{   
		USART_ClearFlag(USART3, USART_FLAG_FE);  
	}

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		uint8_t data = USART_ReceiveData(USART3);
		queue_push(data);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		
	}										 
}

uint16_t USART6_RX_STA=0;   	 
void USART6_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART6, USART_FLAG_PE) != RESET)  
	{  
	USART_ClearFlag(USART6, USART_FLAG_PE);  
	}  

	if(USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)  
	{  
	USART_ClearFlag(USART6, USART_FLAG_ORE);  
	}  

	if (USART_GetFlagStatus(USART6, USART_FLAG_FE) != RESET)  
	{   
	USART_ClearFlag(USART6, USART_FLAG_FE);  
	}

	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		
		USART_ClearFlag(USART6, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);		
		
		uint8_t data = USART_ReceiveData(USART6); 
		/*Start*/ 
		/*End*/
		 
	}										 
} 

//��ʼ��IO ����3
//bound:������	  
void USART485_Init(uint32_t bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

 	USART_DeInit(USART3);  //��λ����2
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB10����ΪUSART3	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
	USART_InitStructure.USART_BaudRate = bound;//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//2��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������2
 
	USART_Cmd(USART3, ENABLE);               //ʹ�ܴ��� 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOB10��GPIOB11��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);//��������ģʽ
	USART3_RX_STA=0;		//����
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]); 	 //�������ݵ�����3 
	}
}

void RS485_Send_Data(uint8_t *buf,uint8_t len)
{
	
	uint8_t t;

	GPIO_SetBits(GPIOA, GPIO_Pin_3);//��������ģʽ
	for(t=0;t<len;t++)		//ѭ����������
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //�ȴ����ͽ���	
			USART_SendData(USART3,buf[t]); //��������
	}	 
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
		USART3_RX_STA=0;
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);//��������ģʽ
	moedbus_busy=0;
}
 
void USART232_Init(uint32_t bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART3ʱ��

 	USART_DeInit(USART6);  //��λ����5
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9����ΪUSART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14����ΪUSART6	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��GPIOG9
	
	
	USART_InitStructure.USART_BaudRate = bound;//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART6, &USART_InitStructure); //��ʼ������2
 
	USART_Cmd(USART6, ENABLE);               //ʹ�ܴ��� 
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//�����ж�   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART6_RX_STA=0;		//����
} 


//����6,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u6_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART6_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART6_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(USART6,(uint8_t)USART6_TX_BUF[j]); 	 //�������ݵ�����3 
	}
}



void USARTDIS_Init(uint32_t bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��

 	USART_DeInit(USART3);  //��λ����2
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_USART3); //GPIOD6����ΪUSART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_USART3); //GPIOD5����ΪUSART2	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOD6GPIOD5��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��GPIOD6��GPIOD5
	
	USART_InitStructure.USART_BaudRate = bound;//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������2
 
	USART_Cmd(USART3, ENABLE);               //ʹ�ܴ��� 
	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//��ռ���ȼ�8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}


void ui_sendsinglechar(uint8_t dat)
{
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
//	USART_SendData(USART2,dat); 	 //�������ݵ�����1
	
	   USART_SendData(USART3,dat);
	    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
}

//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
//uint8_t modbus_t = 0;
void RS485_Receive_Data(uint8_t *buf,uint16_t viTagLen,uint16_t *len)
{
	
	moedbus_busy = 1;
	uint16_t rxlen=0;
	uint8_t i=0;
	while(viTagLen!=USART3_RX_STA && i++<50)
	{
		delay_ms(1);	
	}
	rxlen=USART3_RX_STA;
	*len=0;				  //Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	
	if(rxlen==USART3_RX_STA && rxlen==viTagLen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];	
		}		
		*len=USART3_RX_STA;	//��¼�������ݳ���
	}
		USART3_RX_STA=0;		//����
//	modbus_t++;
//	
//	if(modbus_t > 10)
//	{
//		modbus_t = 0;
	moedbus_busy = 0;
//	}
}

/*!
*   \brief  ����1���ֽ�
*   \param  t ���͵��ֽ�
*/
void  Custom_RS232_SendChar(unsigned char  t)
{
	    USART_SendData(USART6,t);
	    while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
	    //DelayMS(1);
}