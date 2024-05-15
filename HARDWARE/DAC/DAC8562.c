#include "DAC8562.h"
#include "sys_timer.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitDAC8562
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� DAC8562
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitDAC8562(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(A_RCC_SCLK | A_RCC_DIN | A_RCC_SYNC | A_RCC_LDAC | A_RCC_CLR, ENABLE);

	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = A_PIN_SCLK;
	GPIO_Init(A_PORT_SCLK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A_PIN_DIN;
	GPIO_Init(A_PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A_PIN_SYNC;
	GPIO_Init(A_PORT_SYNC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A_PIN_LDAC;
	GPIO_Init(A_PORT_LDAC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A_PIN_CLR;
	GPIO_Init(A_PORT_CLR, &GPIO_InitStructure);
	
	A_SYNC_1();	/* SYNC = 1 */
	A_CLR_0();
	
	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(B_RCC_SCLK | B_RCC_DIN | B_RCC_SYNC | B_RCC_LDAC, ENABLE);

	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = B_PIN_SCLK;
	GPIO_Init(B_PORT_SCLK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = B_PIN_DIN;
	GPIO_Init(B_PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = B_PIN_SYNC;
	GPIO_Init(B_PORT_SYNC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = B_PIN_LDAC;
	GPIO_Init(B_PORT_LDAC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = B_PIN_CLR;
	GPIO_Init(B_PORT_CLR, &GPIO_InitStructure);
	
	B_SYNC_1();	/* SYNC = 1 */
	B_CLR_0();
	
	/* Power up DAC-A and DAC-B */
	DAC8562_WriteCmd((4 << 19) | (0 << 16) | (3 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((4 << 19) | (0 << 16) | (3 << 0), 1);
	delay_us(10);
	/* LDAC pin inactive for DAC-B and DAC-A  ��ʹ��LDAC���Ÿ������� */
	DAC8562_WriteCmd((6 << 19) | (0 << 16) | (3 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((6 << 19) | (0 << 16) | (3 << 0), 1);
	delay_us(10);
	
	/* ��λ2��DAC���м�ֵ, ���2.5V */

	/*
	DAC8562_SetData(0, 0, 43690);
	DAC8562_SetData(0, 1, 43690);
	DAC8562_SetData(1, 0, 0);
	DAC8562_SetData(1, 1, 0);
	
	DAC8562_SetData(0, 0, 0);
	DAC8562_SetData(0, 1, 0);
	DAC8562_SetData(1, 0, 0);
	DAC8562_SetData(1, 1, 0);
	*/

	/* ѡ���ڲ��ο�����λ2��DAC������=2 ����λʱ���ڲ��ο��ǽ�ֹ��) */
	DAC8562_WriteCmd((7 << 19) | (0 << 16) | (1 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((7 << 19) | (0 << 16) | (1 << 0), 1);
	delay_us(10);
}

void DAC8562_Reset(void)
{
	A_CLR_1();
	delay_us(10);
	B_CLR_1();
	
	delay_us(20);
	A_SYNC_1();	/* SYNC = 1 */
	delay_us(10);
	A_CLR_0();
	delay_us(10);
	
	B_SYNC_1();	/* SYNC = 1 */
	delay_us(10);
	B_CLR_0();
	delay_us(10);
	
	/*software reset*/
	DAC8562_WriteCmd((5 << 19) | (0 << 16) | (3 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((5 << 19) | (0 << 16) | (3 << 0), 1);
	
	delay_us(20);
	
	/* Power up DAC-A and DAC-B */
	DAC8562_WriteCmd((4 << 19) | (0 << 16) | (3 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((4 << 19) | (0 << 16) | (3 << 0), 1);
	delay_us(10);
	/* LDAC pin inactive for DAC-B and DAC-A  ��ʹ��LDAC���Ÿ������� */
	DAC8562_WriteCmd((6 << 19) | (0 << 16) | (3 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((6 << 19) | (0 << 16) | (3 << 0), 1);
	delay_us(10);
	
	DAC8562_WriteCmd((7 << 19) | (0 << 16) | (1 << 0), 0);
	delay_us(10);
	DAC8562_WriteCmd((7 << 19) | (0 << 16) | (1 << 0), 1);
	delay_us(10);
}

/*
*********************************************************************************************************
*	�� �� ��: DAC8562_WriteCmd
*	����˵��: ��SPI���߷���24��bit���ݡ�
*	��    ��: _cmd : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DAC8562_WriteCmd(uint32_t _cmd, uint8_t address)
{
	uint8_t i;
	switch(address)
	{
		case 0:
			A_SYNC_0();
			delay_us(10);
			/*��DAC8562 SCLKʱ�Ӹߴ�50M����˿��Բ��ӳ� */
			for(i = 0; i < 24; i++)
			{
				if (_cmd & 0x800000)
				{
					A_DIN_1();
				}
				else
				{
					A_DIN_0();
				}
				A_SCLK_1();
				delay_us(10);
				_cmd <<= 1;
				A_SCLK_0();
				delay_us(10);
			}
			
			A_SYNC_1();
			break;
		case 1:
			B_SYNC_0();
			delay_us(10);
			/*��DAC8562 SCLKʱ�Ӹߴ�50M����˿��Բ��ӳ� */
			for(i = 0; i < 24; i++)
			{
				if (_cmd & 0x800000)
				{
					B_DIN_1();
				}
				else
				{
					B_DIN_0();
				}
				B_SCLK_1();
				delay_us(10);
				_cmd <<= 1;
				B_SCLK_0();
				delay_us(10);
			}
			
			B_SYNC_1();
			delay_us(10);
			break;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DAC8562_SetData
*	����˵��: ����DAC��������������¡�
*	��    ��: _ch, ͨ��, 0 , 1
*		     _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DAC8562_SetData(uint8_t address, uint8_t _ch,uint16_t _dac,uint8_t reset8562)
{
	if(reset8562==1)
	{
		DAC8562_Reset();
		delay_us(20);
	}
	if (_ch == 0)
	{
		/* Write to DAC-A input register and update DAC-A; */
		DAC8562_WriteCmd((3 << 19) | (0 << 16) | (_dac << 0), address);
		delay_us(20);
	}
	else if (_ch == 1)
	{
		/* Write to DAC-B input register and update DAC-B; */
		DAC8562_WriteCmd((3 << 19) | (1 << 16) | (_dac << 0), address);		
		delay_us(20);
	}
}


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
