#include "DS1302.h"
#include "sys_core.h"

unsigned char second = 0,minute = 0,hour = 0,week = 0,day = 0,month = 0,year = 0, second_last = 0;   
unsigned char time[]={0x12,0x09,0x07,0x03,0x18,0x44,0x00}; //
unsigned char time_date_dis[11] = "2018-09-05";
unsigned char time_hms_dis[9] = "12:00:00";

void Ds1302_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DS1302_CLK_Pin | DS1302_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DS1302_Port1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DS1302_RST_Pin;
	GPIO_Init(DS1302_Port2, &GPIO_InitStructure);
}

void DS1302_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = DS1302_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DS1302_Port1, &GPIO_InitStructure);
}

void DS1302_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = DS1302_IO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DS1302_Port1, &GPIO_InitStructure);
}

//DS1302дһ�ֽ�
void InputByte(unsigned char byte1)
{
	char i;
	DS1302_OUT();
	
	for(i=8;i>0;i--)
	{
	 if(byte1&0x01)
		 DS1302_IO_H;
	 else
		 DS1302_IO_L;
	 DS1302_CLK_H;
	 delay_us(10);
	 DS1302_CLK_L;
	 byte1>>=1;
	 delay_us(10);
	}
	return;
}

//DS1302��һ�ֽ�
unsigned char outputbyte(void) 
{
	unsigned char i;
	unsigned char ucdat=0;
	for(i=8;i>0;i--)
	{
		DS1302_IO_H;
		ucdat>>=1;
		DS1302_IN();
		if(READ_1302IO)ucdat|=0x80;
		DS1302_OUT();
		DS1302_CLK_H;
		delay_us(10);
		DS1302_CLK_L;
		delay_us(10);
	}
	return(ucdat);
}

//DS1302д������
void write_ds1302(unsigned char addr,unsigned char TDat)
{
	 DS1302_RST_L;
	 delay_us(10);
	 DS1302_CLK_L;
	 delay_us(10);
	 DS1302_RST_H;
	 InputByte(addr);
	 delay_us(10);
	 InputByte(TDat);
	 DS1302_CLK_H;
	 delay_us(10);
	 DS1302_RST_L;
}

//DS1302��������
unsigned char read_ds1302(unsigned char addr)
{
	unsigned char timedata;
	DS1302_RST_L;
	delay_us(10);
	DS1302_CLK_L;
	delay_us(10);
	DS1302_RST_H;
	InputByte(addr);
	timedata=outputbyte();
	DS1302_CLK_H;
	delay_us(10);
	DS1302_RST_L;
	return(timedata);
}



void Ds1302_Init(void)//�趨ʱ��
{
	Ds1302_GPIOInit();
	
	write_ds1302(0x8e,0x00);      //
	delay_ms(10);
	write_ds1302(0x8c,time[0]);   //
	delay_ms(10);
	write_ds1302(0x88,time[1]);   //
	delay_ms(10);
	write_ds1302(0x86,time[2]);   //
	delay_ms(10);
	write_ds1302(0x8A,time[3]);   //
	delay_ms(10);
	write_ds1302(0x84,time[4]);   //
	delay_ms(10);
	write_ds1302(0x82,time[5]);   //
	delay_ms(10);
	write_ds1302(0x80,time[6]);   //
	delay_ms(10);
	write_ds1302(0x8e,0x80);      //
	
	delay_ms(1000);
}

void hexToChar(unsigned char str, unsigned char str1[2])//hex��char֮���ת��
{
	unsigned char tem = 0;
	
	tem = (str & 0xf0) >> 4;
	if(tem < 10)
	{
		str1[0] = tem + 0x30;
	}else
	{
		str1[0] = tem - 0x0a + 0x41;
	}
	tem = str & 0x0f;
	if(tem < 10)
	{
		str1[1] = tem + 0x30;
	}else
	{
		str1[1] = tem - 0x0a + 0x41;
	}
}

void read_time(void)//��ʱ��
{

	week=read_ds1302(0x8B);     //����
	day=read_ds1302(0x87);      //��
	month=read_ds1302(0x89);    //��
	year=read_ds1302(0x8d);     //��

	hour=read_ds1302(0x85);     //ʱ
	minute=read_ds1302(0x83);   //��
	second=read_ds1302(0x81);   //��
	
//	hexToChar(second, &time_date_dis[2]);		
	if(month <= 96)//����ת��
	{
		hexToChar(month, &time_date_dis[5]);
	}else
	{
		while(month > 96)
		{
			month=read_ds1302(0x89);   //
		}
		hexToChar(month, &time_date_dis[5]);	
	}

	if(day <= 96)//����ת��
	{
		hexToChar(day, &time_date_dis[8]);	
	}else
	{
		while(day > 96)
		{
			day=read_ds1302(0x87);   //
		}
		hexToChar(day, &time_date_dis[8]);	
	}	
	
	time_date_dis[2] = year % 100 / 10 + 0x30;
	time_date_dis[3] = year % 10 + 0x30;
//	time_date_dis[5] = month % 100 / 10 + 0x30;
//	time_date_dis[6] = month % 10 + 0x30;
//	time_date_dis[8] = day % 100 / 10 + 0x30;
//	time_date_dis[9] = day % 10 + 0x30;
	
	if(second <= 96)
	{		
		hexToChar(second, &time_hms_dis[6]);
	}else//�������
	{
		while(second > 96)//ֱ������ȷΪֹ
		{
			second=read_ds1302(0x81);   //
		}
		hexToChar(second, &time_hms_dis[6]);		
	}
	
	if(minute <= 96)//����ת��
	{
		hexToChar(minute, &time_hms_dis[3]);
	}else
	{
		while(minute > 96)
		{
			minute=read_ds1302(0x83);   //
		}
		hexToChar(minute, &time_hms_dis[3]);		
	}
	
	if(hour <= 36)//Сʱת��
	{
		hexToChar(hour, &time_hms_dis[0]);
	}else
	{
		while(hour > 36)
		{
			hour=read_ds1302(0x85);   //
		}
		hexToChar(hour, &time_hms_dis[0]);		
	}
}
