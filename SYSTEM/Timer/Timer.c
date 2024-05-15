#include "timer.h"
uint32_t m_Timer_Counter=0;
void TIMx_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = ARR; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=PSC;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);//��������ж�����λ
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIMx,ENABLE); ////��ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIMx_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=15; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void TIMx_IRQHandler_10MS(void);
//void TIMx_IRQHandler_100MS(void);
//void TIMx_IRQHandler_1S(void);

//��ʱ��3�жϷ�����
void TIMx_IRQHandler(void)
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update)==SET) //����ж�
	{
		m_Timer_Counter++;
//		TIMx_IRQHandler_10MS(); 
		
		if((m_Timer_Counter+1)%100==0)
		{ 
			//TIMx_IRQHandler_100MS();
			TIMx_IRQHandler_1S();
			m_Timer_Counter=0;
		}
		else if((m_Timer_Counter+1)%10==0)
		{ 
//			TIMx_IRQHandler_100MS();
		} 
	}
	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);  //����жϱ�־λ
}
void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APBxPeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = ARR; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=PSC;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//��������ж�����λ
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM4,ENABLE); ////��ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=14; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void TIMx_IRQHandler_10MS(void);
//void TIMx_IRQHandler_100MS(void);
//void TIMx_IRQHandler_1S(void);

//��ʱ��3�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		//m_Timer_Counter++;
		TIM4_IRQHandler_10MS(); 
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}