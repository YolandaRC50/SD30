#include "BSP.h"

uint32_t Start_Loop = 0; 
int main(void)
{
	
	BSP_Init();	 
	
	delay_ms(100);
	
	//printf("����ѭ��ɨ��RCC_GetFlagStatus,��״̬ΪSETʱ�Զ�����...\r\n");
	
	//while ( RCC_GetFlagStatus(RCC_FLAG_SFTRST) == RESET);
	
	printf("����������ʼ����ɣ����ڿ�ʼѭ��ɨ��USB�豸...\r\n");
	while (1)
	{
		if(!jump_flag &&Detect_USB==0)
		{
			
			printf("��%d��U��ɨ��...\r\n",Start_Loop);
			
			USBH_Process(&USB_OTG_Core, &USB_Host);   
		}  
		if(jump_flag)
		{ 
		 
			if(Detect_USB==1) 
			{ 
				USBH_DeInit(&USB_OTG_Core,&USB_Host);
			}	
			__disable_irq();
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}
		}
		delay_ms(1);
		if(Detect_USB==0)
		{
			if(Start_Loop > 500)
			{
				Start_Loop=0;
				jump_flag = 1;
				
			}
			Start_Loop++;
		}
	}
}

