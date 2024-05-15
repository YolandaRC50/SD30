#include "Pressure.h"
#include "Includes.h"
#define N 2000 //ÿͨ����1000��
#define M 1 //Ϊ12��ͨ�� 

#define BUFFER_LEN 20
//float ADC_S[2] = {0}; 

__IO uint16_t AD_Value[N]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
__IO uint16_t AD_Value_Vacuum[N]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ


uint16_t AD_Value_Buffer[BUFFER_LEN]={0};
uint16_t AD_Value_Buffer_Vacuum[BUFFER_LEN]={0};

#if(1)
 void Enable_PressureSensor_Auto_Detect(void)
{
				/*NVIC �ж�*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     //yolanda
		NVIC_InitTypeDef    NVIC_InitStructure;      
		NVIC_InitStructure.NVIC_IRQChannel=DMA_STREAMx_IRQN;     //�ж�     �ĸ�ͨ���� 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=13;    //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;           //��Ӧ(�ӣ����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //ʹ��IBQͨ��
		NVIC_Init(&NVIC_InitStructure);                            //��ʼ��
		
		/*��������ж�*/
		DMA_ITConfig(DMA_STREAMx, DMA_IT_HT, DISABLE);
		/*��������ж�*/
		DMA_ITConfig(DMA_STREAMx, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA_STREAMx, ENABLE);

		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TCIF0);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_HTIF0);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TE); 
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_DME);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_FE); 
	
		
}
 void Enable_VacuumSensor_Auto_Detect(void)
{
				/*NVIC �ж�*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     //yolanda
		NVIC_InitTypeDef    NVIC_InitStructure;      
		NVIC_InitStructure.NVIC_IRQChannel=DMA_STREAMy_IRQN;     //�ж�     �ĸ�ͨ���� 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;    //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;           //��Ӧ(�ӣ����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //ʹ��IBQͨ��
		NVIC_Init(&NVIC_InitStructure);                            //��ʼ��
		
		/*��������ж�*/
		DMA_ITConfig(DMA_STREAMy, DMA_IT_HT, DISABLE);
		/*��������ж�*/
		DMA_ITConfig(DMA_STREAMy, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA_STREAMy, ENABLE);

		DMA_ClearITPendingBit(DMA_STREAMy,DMA_IT_TCIF0);
		DMA_ClearITPendingBit(DMA_STREAMy,DMA_IT_HTIF0);
		DMA_ClearITPendingBit(DMA_STREAMy,DMA_IT_TE); 
		DMA_ClearITPendingBit(DMA_STREAMy,DMA_IT_DME);
		DMA_ClearITPendingBit(DMA_STREAMy,DMA_IT_FE); 
	
		
}
 
void	ADC_Configuration()
{
		
		GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_DMAx, ENABLE);			//ʹ��DMA2ʱ��    
	RCC_AHB1PeriphClockCmd(SENSOR_AN_IN_RCCx, ENABLE);			//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(SENSOR_AN_IN_ADCx_RCCx, ENABLE); 			//ʹ��ADC1ʱ��

	
  //�ȳ�ʼ��ADC1ͨ�� IO��
  GPIO_InitStructure.GPIO_Pin = SENSOR_AN_IN_GOIP_PINx_0;			//PC1	�� PC2			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;							//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//����������
  GPIO_Init(SENSOR_AN_IN_PORTx, &GPIO_InitStructure);										 
      
//    DMA_DeInit(DMA2_Stream0);
//    DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_CHANNELx;                   						//ѡ��Channel_0
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADCx_DR_ADDRESS;							//���ݴ���������׵�ַ��������
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;	 	//�Լ����������������׵�ַ��Ҫǿ��ת��Ϊ32λ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       							//���ݴ��䷽��ѡ��Ϊ����->�ڴ�
	DMA_InitStructure.DMA_BufferSize = N*M;                    										//�������ֵ�����Ƕ���������Сһ��		 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//���������ģʽ			
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        							//�ڴ������ģʽ		 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�������ݴ�СΪ����	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     		//�ڴ����ݴ�СΪ����		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     												//DMAΪѭ��ģʽ	 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   											//DMA���ȼ�Ϊ��		
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;												//����͹ٷ����̲�ͬ
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;							//FIFO��ص�����
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;										//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;						//����ͻ�����δ���
	DMA_Init(DMA_STREAMx, &DMA_InitStructure);
   
	DMA_Cmd(DMA_STREAMx, ENABLE);	 //ʹ��DMA2_Stream0ͨ��
       




  /* ��ʼ��ADC */
	/* ADC_CommonInit �Ĳ������ö�Ӧ����ADC->CCR */
	/* ADC_Init				�Ĳ������ö�Ӧ����ADC1->CR1��ADC1->CR2 */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;											//����ADC��ͨ�����ö���ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	//���������׶�֮����ӳ�20��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 			//DMAʹ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;										//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);																			//��ʼ��
    
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;												//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;																	//ɨ��ģʽ    
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;														//����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;												//�Ҷ���    
  ADC_InitStructure.ADC_NbrOfConversion = M;																		//1��ת���ڹ��������� Ҳ����ֻת����������1 
  
	ADC_Init(SENSOR_AN_IN_ADCx, &ADC_InitStructure);
	
  ADC_RegularChannelConfig(SENSOR_AN_IN_ADCx, SENSOR_AN_IN_ADCx_Channelx_0 , 1, ADC_SampleTime_480Cycles);	//����ͨ��1Ϊ��һ��ת��˳�� 
	
	

  /* ʹ�� ADC1 DMA */
  ADC_DMACmd(SENSOR_AN_IN_ADCx, ENABLE);
	

	/*ֻҪADC��������ת������DMA=1���㷢��DMA����
	��Ӧ��������ADC1->CR2�Ĵ�����λ9 */
	ADC_DMARequestAfterLastTransferCmd(SENSOR_AN_IN_ADCx, ENABLE);


  /* ����AD1ת���� */
  ADC_Cmd(SENSOR_AN_IN_ADCx, ENABLE);      


  /* ����ADC1 */
  ADC_SoftwareStartConv(SENSOR_AN_IN_ADCx);	
		
		 
}
void	ADC_Configuration_Vacuum()
{
		
		GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_DMAy, ENABLE);			//ʹ��DMA2ʱ��    
	RCC_AHB1PeriphClockCmd(SENSOR_AN_IN_RCCy, ENABLE);			//ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(SENSOR_AN_IN_ADCy_RCCy, ENABLE); 			//ʹ��ADC1ʱ��

	
  //�ȳ�ʼ��ADC1ͨ�� IO��
  GPIO_InitStructure.GPIO_Pin = SENSOR_AN_IN_GOIP_PINy_0;			//PC1	�� PC2			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;							//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//����������
  GPIO_Init(SENSOR_AN_IN_PORTy, &GPIO_InitStructure);										 
      
//    DMA_DeInit(DMA2_Stream0);
//    DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_CHANNELy;                   						//ѡ��Channel_1
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADCy_DR_ADDRESS;							//���ݴ���������׵�ַ��������
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value_Vacuum;	 	//�Լ����������������׵�ַ��Ҫǿ��ת��Ϊ32λ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       							//���ݴ��䷽��ѡ��Ϊ����->�ڴ�
	DMA_InitStructure.DMA_BufferSize = N*M;                    										//�������ֵ�����Ƕ���������Сһ��		 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//���������ģʽ			
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        							//�ڴ������ģʽ		 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�������ݴ�СΪ����	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     		//�ڴ����ݴ�СΪ����		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     												//DMAΪѭ��ģʽ	 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   											//DMA���ȼ�Ϊ��		
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;												//����͹ٷ����̲�ͬ
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;							//FIFO��ص�����
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;										//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;						//����ͻ�����δ���
	DMA_Init(DMA_STREAMy, &DMA_InitStructure);
   
	DMA_Cmd(DMA_STREAMy, ENABLE);	 //ʹ��DMA2_Stream2ͨ��
       




  /* ��ʼ��ADC */
	/* ADC_CommonInit �Ĳ������ö�Ӧ����ADC->CCR */
	/* ADC_Init				�Ĳ������ö�Ӧ����ADC1->CR1��ADC1->CR2 */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;											//����ADC��ͨ�����ö���ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	//���������׶�֮����ӳ�20��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 			//DMAʹ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;										//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);																			//��ʼ��
    
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;												//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;																	//ɨ��ģʽ    
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;														//����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;												//�Ҷ���    
  ADC_InitStructure.ADC_NbrOfConversion = M;																		//1��ת���ڹ��������� Ҳ����ֻת����������1 
  
	ADC_Init(SENSOR_AN_IN_ADCy, &ADC_InitStructure);
	
  ADC_RegularChannelConfig(SENSOR_AN_IN_ADCy, SENSOR_AN_IN_ADCy_Channely_0 , 1, ADC_SampleTime_480Cycles);	//����ͨ��1Ϊ��һ��ת��˳�� 
	
	

  /* ʹ�� ADC1 DMA */
  ADC_DMACmd(SENSOR_AN_IN_ADCy, ENABLE);
	

	/*ֻҪADC��������ת������DMA=1���㷢��DMA����
	��Ӧ��������ADC1->CR2�Ĵ�����λ9 */
	ADC_DMARequestAfterLastTransferCmd(SENSOR_AN_IN_ADCy, ENABLE);


  /* ����AD1ת���� */
  ADC_Cmd(SENSOR_AN_IN_ADCy, ENABLE);      


  /* ����ADC1 */
  ADC_SoftwareStartConv(SENSOR_AN_IN_ADCy);	
		
		 
}

void	DAC_Configuration()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		DAC_InitTypeDef DAC_InitType;
		
		RCC_AHB1PeriphClockCmd(SENSOR_AN_O_RCCx, ENABLE);//ʹ��GPIOAʱ��
		RCC_APB1PeriphClockCmd(SENSOR_AN_O_DAC_RCCx, ENABLE);//ʹ��DACʱ��
			 
		GPIO_InitStructure.GPIO_Pin = SENSOR_AN_O_GOIP_PINx_0;/*|SENSOR_AN_O_GOIP_PINx_1;*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
		GPIO_Init(SENSOR_AN_O_PORTx, &GPIO_InitStructure);//��ʼ��

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable; //DAC1�������ر� BOFF1=1
		DAC_Init(SENSOR_AN_DCA_Channel_0,&DAC_InitType);	 //��ʼ��DACͨ��1 
		DAC_Cmd(SENSOR_AN_DCA_Channel_0, ENABLE);  //ʹ��DACͨ��1 
		SENSOR_AN_DCA_SetChannel0Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
		
}
void	DAC_vacuum_Configuration()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		DAC_InitTypeDef DAC_InitType;
		
		RCC_AHB1PeriphClockCmd(SENSOR_AN_O_RCCy, ENABLE);//ʹ��GPIOAʱ��
		RCC_APB1PeriphClockCmd(SENSOR_AN_O_DAC_RCCx, ENABLE);//ʹ��DACʱ��
			 
		GPIO_InitStructure.GPIO_Pin = SENSOR_AN_O_GOIP_PINy_0;/*|SENSOR_AN_O_GOIP_PINx_1;*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
		GPIO_Init(SENSOR_AN_O_PORTy, &GPIO_InitStructure);//��ʼ��

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable; //DAC1�������ر� BOFF1=1
		DAC_Init(SENSOR_AN_DCA_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1 
		DAC_Cmd(SENSOR_AN_DCA_Channel_1, ENABLE);  //ʹ��DACͨ��1 
		SENSOR_AN_DCA_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
		
}





void filter(void)
{
		float sum = 0;     
		float sum_1 = 0;  
		for(int i=0;i<N;i++) 
		{
				sum+=AD_Value[i];
		}
		sum/=N;
		for(int i=BUFFER_LEN-1;i>0;i--)
		{
			AD_Value_Buffer[i]=AD_Value_Buffer[i-1];
			sum_1+=AD_Value_Buffer[i]; 
		} 
		AD_Value_Buffer[0]=sum;
		sum_1+=sum;
		Sys_RT_Status.RT_AN_Pressure=sum_1/BUFFER_LEN;
		
		Sys_RT_Status.RT_Pressure=(Sys_RT_Status.RT_AN_Pressure-Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value)
		/((Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value-Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value+1.00f)                   //
		/(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value-Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value));                     //  ʵ�ʵ�ѹ��
	 	 
}
void filterVacuum(void)
{
		float sum_Vacuum = 0;     
		float sum_1_Vacuum = 0;  
		for(int i=0;i<N;i++) 
		{
				sum_Vacuum+=AD_Value_Vacuum[i];
		}
		sum_Vacuum/=N;
		for(int i=BUFFER_LEN-1;i>0;i--)
		{
			AD_Value_Buffer_Vacuum[i]=AD_Value_Buffer_Vacuum[i-1];
			sum_1_Vacuum+=AD_Value_Buffer_Vacuum[i]; 
		} 
		AD_Value_Buffer_Vacuum[0]=sum_Vacuum;
		sum_1_Vacuum+=sum_Vacuum;
		Sys_RT_Status.RT_AN_Vacuum=sum_1_Vacuum/BUFFER_LEN;
		
		Sys_RT_Status.RT_Vacuum=(Sys_RT_Status.RT_AN_Vacuum-Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_3_Value)
		/((Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_4_Value-Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_3_Value+1.00f)
		/(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_4_Value-Sys_Params.Pressure_Param.Calibration_RT_Param.RT_3_Value));
	 	 
}

void DMA_STREAMx_IRQHandler(void)      
{
	
		if(DMA_GetITStatus(DMA_STREAMx,DMA_IT_TCIF0)==SET)
		{ 
			filter();
			DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TCIF0);          
		} 
		
}

void DMA_STREAMy_IRQHandler(void)      
{
	
		if(DMA_GetITStatus(DMA_STREAMy,DMA_IT_TCIF2)==SET)
		{ 
			filterVacuum();
			DMA_ClearITPendingBit(DMA_STREAMy,DMA_IT_TCIF2);          
		} 
		
}
#endif

void PressureSensor_Init(void)
{
//  RCC_AHB2PeriphClockCmd(SENSOR_AN_IN_ADCx_RCCx, ENABLE);  
//  
	  
	ADC_Configuration();
	ADC_Configuration_Vacuum();
	
	DAC_Configuration();
	DAC_vacuum_Configuration();

}
 
uint16_t _OUT_PUT_AN=0;
float getOutputPercent(void)
{
	float _Max_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value,_Min_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value;    //�û������������
	
	float _DeltaPressure=Sys_Params.Pressure_Param.TargetPreessure;//-Sys_RT_Status.RT_Pressure;
	float _MeasureRange=Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value-Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value;
	float _unitPreBar=/*417.0142;*/(_Max_AN-_Min_AN)/(_MeasureRange); 
	
	float _Delta_AN=_Min_AN+((_DeltaPressure)*_unitPreBar);
	
	//	float _Cal_1_AN=909;
	//	float _Cal_2_AN=2273;
	//	
	//	float _RT_1=1.925;
	//	float _RT_2=5.195;
	//	
	//	
	//	if(1==0)
	//	{
	//		
	//	}
//	if(_Delta_AN>=_Min_AN+(9.0*_unitPreBar))
//	{
//		_Delta_AN=_Max_AN;
//	}
	_OUT_PUT_AN=_Delta_AN;
}


void SetPressureSensorTargetValue(void)
{ 
	uint16_t temp=0;
	if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_OFF
		&&Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_OFF)
	{
		if(Sys_RT_Status.pressure_disable_Mode==POWER_ON)
		{
			getOutputPercent();
			temp=_OUT_PUT_AN;
		}
		else
		{
			_OUT_PUT_AN=0;
		}
	}
	else
	{
		if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_OFF)
		{
			_OUT_PUT_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value;
		}
		else if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_OFF && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON)
		{
			_OUT_PUT_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value;
		}
	}
	temp=_OUT_PUT_AN;
	
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ 
}


uint16_t _OUT_PUT_AN_Vacuum=0;
float getOutputPercent_Vacuum(void)
{	
	
	float _Max_AN_Vacuum=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value,_Min_AN_Vacuum=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value;
	//yolanda ��ѹת��Ϊ��ѹ
	//float _DeltaPressure_Vacuumtemp =Sys_Params.Pressure_Param.Targetvacuum;//-Sys_RT_Status.RT_Pressure;
	float _DeltaPressure_Vacuum = 8.7224*Sys_Params.Pressure_Param.Targetvacuum +0.4392-0.5704*Sys_Params.Pressure_Param.Targetvacuum*Sys_Params.Pressure_Param.Targetvacuum; //��ѹת��Ϊ��ѹ
	//y = -0.5704x2 + 8.7224x + 0.4392
	float _MeasureRange_Vacuum=Sys_Params.Pressure_Param.Calibration_Set_Param.RT_4_Value-Sys_Params.Pressure_Param.Calibration_Set_Param.RT_3_Value;
	float _unitPrePsi_Vacuum=/*417.0142;*/(_Max_AN_Vacuum-_Min_AN_Vacuum)/(_MeasureRange_Vacuum); //  y=kx+Analog_3_Value  y�Ǳ������������ѹ��x�Ǳ�������ѹ��
	
	float _Delta_AN_Vacuum=_Min_AN_Vacuum+((_DeltaPressure_Vacuum)*_unitPrePsi_Vacuum);
	
	_OUT_PUT_AN_Vacuum=_Delta_AN_Vacuum;
}
void SetVacuumSensorTargetValue(void)
{ 
	uint16_t temp=0;
	if(Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable==POWER_OFF
		&&Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable==POWER_OFF)
	{
		if(Sys_RT_Status.pressure_disable_Mode==POWER_ON)
		{
			getOutputPercent_Vacuum();
			temp=_OUT_PUT_AN_Vacuum;
		}
		else
		{
			_OUT_PUT_AN_Vacuum=0;
		}
	}
	else
	{
		if(Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable==POWER_ON && Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable==POWER_OFF)
		{
			_OUT_PUT_AN_Vacuum=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_3_Value;
		}
		else if(Sys_RT_Status.Vacuum_Set_Cal_0Bar_Enable==POWER_OFF && Sys_RT_Status.Vacuum_Set_Cal_5Bar_Enable==POWER_ON)
		{
			_OUT_PUT_AN_Vacuum=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_4_Value;
		}
	}
	temp=_OUT_PUT_AN_Vacuum;
	
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ 
}
 