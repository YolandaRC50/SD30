#include "ADC.h"

float ADC_S[4] = {0}; 
#define BUFFER_LEN 20
#define N 1000 //ÿͨ����1000��
#define M 4 //Ϊ12��ͨ�� 
uint16_t AD_Value[N][M]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
uint16_t AD_Value_Buffer[BUFFER_LEN][M]={0};


void ADC_Config(void)
{
//  ADC_InitTypeDef       ADC_InitStructure;
//  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//  DMA_InitTypeDef       DMA_InitStructure;
//  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADCx, DMA and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(ADCx_CHANNEL_GPIO_CLK, ENABLE);  
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);
  
	GPIO_Configuration();

	ADC3_Configuration();

	DMA_Configuration();

  /* DMA2 Stream0 channel2 configuration **************************************/
//  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADCx_DR_ADDRESS;
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_S;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//  DMA_InitStructure.DMA_BufferSize = 4;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_Init(DMA_STREAMx, &DMA_InitStructure);
//  DMA_Cmd(DMA_STREAMx, ENABLE);

//  /* Configure ADC3 Channel5 Channel4 pin as analog input ******************************/
//  GPIO_InitStructure.GPIO_Pin = GPIO_PIN | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3;
//	
//   GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AN;//GPIO_Mode_AIN; //GPIO_Mode_AN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
//	
//  /* ADC Common Init **********************************************************/
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
//  ADC_CommonInit(&ADC_CommonInitStructure);

//  /* ADC1 Init ****************************************************************/
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
////  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_NbrOfConversion = 4;
//  ADC_Init(ADCx, &ADC_InitStructure);

//  /* ADC1 regular channel7 configuration **************************************/
//  ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_15Cycles);
//	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL1, 2, ADC_SampleTime_15Cycles);
//  ADC_RegularChannelConfig(ADCx, ADC_CHANNEL2, 3, ADC_SampleTime_15Cycles);
//	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL3, 4, ADC_SampleTime_15Cycles);	
// /* Enable DMA request after last transfer (Single-ADC mode) */
//  ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADCx, ENABLE);

//  /* Enable ADC1 */
//  ADC_Cmd(ADCx, ENABLE);
//	ADC_SoftwareStartConv(ADCx);
}
void GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_PIN | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3; 
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AN;//GPIO_Mode_AIN; //GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
}

void DMA_Configuration(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_InitStructure.DMA_Channel = DMA_CHANNELx; //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
		DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)ADCx_DR_ADDRESS; //DMA����ADC����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr =(u32)&AD_Value; //DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
		DMA_InitStructure.DMA_BufferSize = N*M; //DMAͨ����DMA����Ĵ�С
		DMA_InitStructure.DMA_PeripheralInc =DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
		DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
		DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
		DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
		DMA_InitStructure.DMA_Priority =DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
		//DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		DMA_Init(DMA_STREAMx, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
		DMA_Cmd(DMA_STREAMx, ENABLE);
 
		/*��������ж�*/
		DMA_ITConfig(DMA_STREAMx, DMA_IT_HT, DISABLE);
		/*��������ж�*/
		DMA_ITConfig(DMA_STREAMx, DMA_IT_TC, ENABLE);
		
		DMA_Cmd(DMA_STREAMx, ENABLE);
		
		//#if (1==1)
		
			/*NVIC �ж�*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitTypeDef    NVIC_InitStructure;     
		//NVIC_InitStructure.NVIC_IRQChannel=DMA2_Channel1_IRQn;     //�ж�     �ĸ�ͨ���� 
		NVIC_InitStructure.NVIC_IRQChannel=DMA_STREAMx_IRQN;     //�ж�     �ĸ�ͨ���� 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;    //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;           //��Ӧ(�ӣ����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //ʹ��IBQͨ��
		NVIC_Init(&NVIC_InitStructure);                            //��ʼ��
		

		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TCIF0);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_HTIF0);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TE); 
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_DME);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_FE); 
	//#endif	
}
void DMA2_Stream0_IRQHandler(void)      
{
	
		if(DMA_GetITStatus(DMA_STREAMx,DMA_IT_TCIF0)==SET)
		{ 
			filter();
			DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TCIF0);          
		} 
		
}
void ADC3_Configuration(void)
{
	
		ADC_InitTypeDef       ADC_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;   
		//****************************

		 /* ADC Common Init **********************************************************/
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInit(&ADC_CommonInitStructure);

		/* ADC1 Init ****************************************************************/
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ģ��ת��������ɨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ģ��ת������������ת��ģʽ
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ⲿ����ת���ر�
		//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfConversion = M;//˳����й���ת����ADCͨ������Ŀ
		ADC_Init(ADCx, &ADC_InitStructure);

		/* ADC1 regular channel7 configuration **************************************/
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL1, 2, ADC_SampleTime_480Cycles);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL2, 3, ADC_SampleTime_480Cycles);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL3, 4, ADC_SampleTime_480Cycles);	
		/* Enable DMA request after last transfer (Single-ADC mode) */
		ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

		/* Enable ADC1 DMA */
		ADC_DMACmd(ADCx, ENABLE);

		/* Enable ADC1 */
		ADC_Cmd(ADCx, ENABLE);
		ADC_SoftwareStartConv(ADCx);
		
}

u16 GetVolt(u16 advalue) 
{

	return (u16)(advalue * 330 / 4096); //��Ľ��������100���������������С��

}

void filter(void)
{
		float sum[M] = {0};     
		float sum_1[M] = {0};  
		for(int i=0;i<N;i++) 
		{

				for ( int j=0;j<M;j++) 
				{ 
						sum[j] += AD_Value[i][j]; 
				} 
		}
		for(int i=BUFFER_LEN-1;i>0;i--)
		{
			for(int j=0;j<M;j++)
			{
				AD_Value_Buffer[i][j]=AD_Value_Buffer[i-1][j];
				sum_1[j]+=AD_Value_Buffer[i][j];
			}
		} 
		for ( int j=0;j<M;j++) 
		{  
			AD_Value_Buffer[0][j]=(sum[j]/N);  
			sum_1[j]+=AD_Value_Buffer[0][j];
			ADC_S[j]=sum_1[j]/BUFFER_LEN;
		}
	 
			 
}

