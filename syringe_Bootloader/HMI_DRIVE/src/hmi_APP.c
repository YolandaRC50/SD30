#include "HMI_App.h"
#include "usbh_usr.h" 

uint8_t cmd_buffer[CMD_MAX_SIZE];//ָ���
static uint16_t current_screen_id = 0;//��ǰ����ID
/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16_t screen_id)
{
	//TODO: ����û�����
	current_screen_id = screen_id;//�ڹ��������п��������л�֪ͨ����¼��ǰ����ID

	if(current_screen_id==4)//
	{
	}
	else if(current_screen_id==9)//
	{
	}
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: ����û�����
}

void SetTextValueint32_t(uint16_t screen_id, uint16_t control_id,int32_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%ld",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.1f",value);//�Ѹ�����ת��Ϊ�ַ���(����һλС��)
	SetTextValue(screen_id,control_id,buffer);
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	//uint8_t HMI_MAIN_INDEX=2;
	//uint8_t HMI_UPGRAD_INDEX=1;
	//uint8_t HMI_PROGRAS_CONTROL_INDEX=4;
	//uint8_t HMI_ENTRY_BUTTON_INDEX=1;
	//uint8_t HMI_CANCEL_BUTTON_INDEX=2;
	
	//TODO: ����û�����
	if(screen_id==HMI_UPGRAD_INDEX)//��ť��ͼ�ꡢ��������
	{
		if(control_id==HMI_ENTRY_BUTTON_INDEX)//������ť
		{
			answer = 1;
		}else if(control_id==HMI_CANCEL_BUTTON_INDEX)//�������˳�
		{
			answer = 2;
		}
	}
}


/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//ָ������
	uint8_t ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8_t control_type = msg->control_type;//�ؼ�����
	uint16_t screen_id = PTR2U16(&msg->screen_id);//����ID
	uint16_t control_id = PTR2U16(&msg->control_id);//�ؼ�ID

	switch(cmd_type)
	{		
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id,control_id,msg->param[1]);
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}
