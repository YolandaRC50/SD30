#include "flash_spi.h" 	
#include "stm32f4xx_spi.h"  


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void FLASH_SPIx_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(FLASH_SPI_RCC_AHBxPeriph_GPIOx, ENABLE);//ʹ��GPIOBʱ��
  RCC_APB2PeriphClockCmd(FLASH_SPI_RCC_APBxPeriph_SPIx, ENABLE);//ʹ��SPI1ʱ��
 
  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_GPIO_Pin_SCK
																|FLASH_SPI_GPIO_Pin_MISO
																|FLASH_SPI_GPIO_Pin_MOSI;//PB3~5���ù������	
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(FLASH_SPI_GPIO_PORTx, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(FLASH_SPI_GPIO_PORTx,FLASH_SPI_GPIO_Pin_Source_SCK,FLASH_GPIO_AF_SPIx); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(FLASH_SPI_GPIO_PORTx,FLASH_SPI_GPIO_Pin_Source_MISO,FLASH_GPIO_AF_SPIx); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(FLASH_SPI_GPIO_PORTx,FLASH_SPI_GPIO_Pin_Source_MOSI,FLASH_GPIO_AF_SPIx); //PB5����Ϊ SPI1
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(FLASH_SPI_RCC_APBxPeriph_SPIx,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(FLASH_SPI_RCC_APBxPeriph_SPIx,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(FLASH_SPIx, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(FLASH_SPIx, ENABLE); //ʹ��SPI����

	//FLASH_SPIx_ReadWriteByte(0xff);//��������		 
}   
//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void FLASH_SPIx_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	FLASH_SPIx->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	FLASH_SPIx->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(FLASH_SPIx,ENABLE); //ʹ��SPI1
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 FLASH_SPIx_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(FLASH_SPIx, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(FLASH_SPIx); //����ͨ��SPIx������յ�����	
 		    
}








