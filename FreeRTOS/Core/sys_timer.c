/*******************************************************************************
** �ļ���:      timer.c
** �汾��       1.0
** ��������:    RealView MDK-ARM
** ����:        Adam
** ����:        ʱ����س���
** ����ļ�:    ��
** �޸���־��   2017-12-19   �����ĵ�
*******************************************************************************/
/* ������ͷ�ļ�---------------------------------------------------------------*/
#include "sys_core.h"
#include "sys_wdg.h"
/* �������� ------------------------------------------------------------------*/
volatile __int64 run_time_ms=0;/*�豸����ʱ�䣨ms��*/
volatile __int64 time_ms=0;/*����ʱ�䣨ms��*/


#if RTOS_ENABLE //ʹ�ò���ϵͳ

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��

extern void xPortSysTickHandler(void);
extern void LED_shine_handle(void);
/*******************************************************************************
- * @��������	SysTick_Handler
- * @����˵��   SysTick�жϷ������жϴ�����
- * @�������   ��
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
//void SysTick_Handler(void)
//{
//    run_time_ms++;
//    time_ms++;
//		LED_shine_handle();
//    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
//    {
//        xPortSysTickHandler();
//    }
//}
void SysTick_Handler(void)
{	
    run_time_ms++;
    time_ms++;
//	  LED_shine_handle();
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
}
///*******************************************************************************
//- * @��������	delay_init
//- * @����˵��   ��ʱ������ʼ��
//- * @�������   ��
//- * @�������   ��
//- * @���ز���   ��
//*******************************************************************************/
//void delay_init(void)
//{
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�����ж����ȼ�����Ϊ��4
//	fac_us=SystemCoreClock/1000000;                 //
//	fac_ms=1000/configTICK_RATE_HZ;                 //����OS������ʱ�����ٵ�λ	   
//	SysTick_Config(SystemCoreClock / 1000);
//}
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;							//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SYSCLK;							//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/configTICK_RATE_HZ;		//����configTICK_RATE_HZ�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.0998s����	
	fac_ms=1000/configTICK_RATE_HZ;			//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/configTICK_RATE_HZ��һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK     
}		
///*******************************************************************************
//- * @��������	delay_us
//- * @����˵��   ��ʱn΢��
//- * @�������   nus:Ҫ��ʱ��΢����
//- * @�������   ��
//- * @���ز���   ��
//*******************************************************************************/
//void delay_us(u32 nus)
//{
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
//	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
//	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{
//			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
//			else tcnt+=reload-tnow+told;
//			told=tnow;
//			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
//		}
//	}
//}
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};										    
} 
///*******************************************************************************
//- * @��������	delay_ms
//- * @����˵��   ��ʱn����
//- * @�������   nus:Ҫ��ʱ�ĺ�����
//- * @�������   ��
//- * @���ز���   ��
//*******************************************************************************/
//void delay_ms(u32 nms)
//{
//	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
//	{
//		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
//		{
//   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS��ʱ
//		}
//		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
//	}
//	delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ
//}
void delay_ms(u32 nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	{		
		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS��ʱ
		}
		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ
}
#else //��ʹ�ò���ϵͳ
#include "stm32f10x.h"
volatile u32 timing_delay=0;/*��ʱ����*/
/*******************************************************************************
- * @��������	delay_init
- * @����˵��   ��ʱ������ʼ��
- * @�������   ��
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
void delay_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//�����ж����ȼ�����Ϊ��4
    SysTick_Config(SystemCoreClock / 1000);
}

/*******************************************************************************
- * @��������	SysTick_Handler
- * @����˵��   SysTick�жϷ������жϴ�����
- * @�������   ��
- * @�������   ��
- * @���ز���   ��
*******************************************************************************/
void SysTick_Handler(void)
{
    run_time_ms++;
    time_ms++;
	if(timing_delay)
		timing_delay--;
}

/*******************************************************************************
  * @��������	delay_ms
  * @����˵��   ���ؽӿڳ�ʼ��
  * @�������   nms����ʱʱ�䣬��λ������
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void delay_ms(u32 nms)
{
	timing_delay = nms;
	while(timing_delay != 0)
        dwg_feed();
}
#endif

/************************************�ļ�����*********************************/
