/*******************************************************************************
** �ļ���:      sys_usart.c
** �汾��       1.0
** ��������:    RealView MDK-ARM
** ����:        Adam
** ����:        ������������
** ����ļ�:    ��
** �޸���־��   2017-09-09   �����ĵ�
*******************************************************************************/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "sys_usart.h"

/* �ӿڶ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/* ���Ͷ��� ------------------------------------------------------------------*/
/* ��������  -----------------------------------------------------------------*/
/* ��������  -----------------------------------------------------------------*/

/******************************************************************************/
/*           		             ��������                                     */
/******************************************************************************/

/******************************************************************************
- * @��������   usart_init
- * @����˵��   ��ʼ������
- * @�������   bound:������
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
void usart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2��
//    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
 	USART_DeInit(USART2);  //��λ����1
	 //USART2_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PB6
   
    //USART2_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PB7


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
   //USART2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}

/*******************************************************************************
- * @��������	usart_send_nbytes
- * @����˵��   ����һ����������
- * @�������   send_buf������ָ�룬num������
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
void usart_send_nbytes(uint8_t* send_buf,uint16_t num)
{
    int i;
    for (i=0;i<num;i++)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
        USART_SendData(USART2,*send_buf++);
    }
}
/*******************************************************************************
- * @��������	usart_send_str
- * @����˵��   ����һ�������ַ���
- * @�������   str���ַ���ָ��
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
void usart_send_str(char* str)
{
    while (*str)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
        USART_SendData(USART2,*str++);
    }
}
/*******************************************************************************
- * @��������	UART2_Handle
- * @����˵��   �����жϷ�����
- * @�������   ��
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        USART_ReceiveData(USART2);
    }
}

/************************************�ļ�����**********************************/
