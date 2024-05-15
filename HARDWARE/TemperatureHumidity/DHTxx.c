#include "DHTxx.h"  

    
//��λDHTxx
void DHTxx_Rst(void)	   
{                 
	DHTxx_IO_OUT(); 	//SET OUTPUT
  DHTxx_OUT_RESET; 	//����DQ
  delay_ms(20);    	//��������18ms
  DHTxx_OUT_SET; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}
//�ȴ�DHTxx�Ļ�Ӧ
//����1:δ��⵽DHTxx�Ĵ���
//����0:����
u8 DHTxx_Check(void) 	   
{   
	u8 retry=0;
	DHTxx_IO_IN();//SET INPUT	 
    while (DHTxx_DQ_IN&&retry<100)//DHTxx������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHTxx_DQ_IN&&retry<100)//DHTxx���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//��DHTxx��ȡһ��λ
//����ֵ��1/0
u8 DHTxx_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHTxx_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHTxx_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHTxx_DQ_IN)return 1;
	else return 0;		   
}
//��DHTxx��ȡһ���ֽ�
//����ֵ������������
u8 DHTxx_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHTxx_Read_Bit();
    }						    
    return dat;
}
//��DHTxx��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint16_t _ReadCount=0;
uint16_t _MaxReadCount=100;
u8 DHTxx_Read_Data()    
{        
 	u8 buf[5];
	u8 i;
	DHTxx_Rst();
	if(DHTxx_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHTxx_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{ 
			Sys_RT_Status.Temperature_RT=buf[2]*1.001f;
			Sys_RT_Status.Humidity_RT=buf[0]*1.001f;
			_ReadCount=0;
		} 
	}
	else 
	{
			if(_ReadCount<_MaxReadCount)
			{
				_ReadCount++;
			}
			else
			{
//				Sys_RT_Status.Temperature_RT=-1;
//				Sys_RT_Status.Humidity_RT=-1;
			}
		return 1;
	}
	return 0;	    
}
//��ʼ��DHTxx��IO�� DQ ͬʱ���DHTxx�Ĵ���
//����1:������
//����0:����    	 
u8 DHTxx_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(DHTxx_RCC_AHB1Periph_GPIOx, ENABLE);//ʹ��GPIOGʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = DHTxx_WIRE_GPIO_Pin ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(DHTxx_GPIOx_Port, &GPIO_InitStructure);//��ʼ��
	DHTxx_Rst();
	return DHTxx_Check();
}
