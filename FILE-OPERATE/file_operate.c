#include "file_operate.h"

//char string[1024];
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
FILINFO fileinfo;								//�ļ���Ϣ
char ReadBuffer[1024*1]="";        /* �������� */
char Log_Path_List[4][20]={"MACHINE_INFO_LOG","RUN_STOP_LOG","ERROR_MESSAGE_LOG","PARAMETER_LOG"};
 
FRESULT FR_Result=FR_INT_ERR;
int Sd_f_mount(void)
{
		res_sd = f_mount(&fs,"0:",1);
		/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		//printf("-->SD dont have file system ��will format to file system...\r\n");
    /* ��ʽ�� */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			//printf("-->SD format to file system success\r\n");
      /* ��ʽ������ȡ������ */
			res_sd = f_mount(NULL,"0:",1);			
      /* ���¹���	*/			
			res_sd = f_mount(&fs,"0:",1);
			FR_Result=FR_OK;
			return 0;
		}
		else
		{
			//printf("-->SD format to file system fail\r\n");
			return -1;
		}
	}
  else if(res_sd!=FR_OK)
  {
//    printf("!!SD mount file system fail(%d)\r\n",res_sd);
//    printf("����may due to ��SD initialzation fail��\r\n");
		return  -1;
  }
  else
  {
    //printf("SD mount file system success,can read and write\r\n");
		return 0;
  }
}

int Sd_f_unmount(void)
{
	FRESULT res=f_mount(NULL,"0:",1);
	switch(res)
	{
		case 0:
			return  0;
			break;
		default  :
			return  1;
			break;
	}
}


void Sd_f_dir(s8 *path)
{
//		memset(string,0, 1024);
		static FILINFO FilInfo; //���徲̬�ļ���Ϣ�ṹ����
	
	DIR DirInfo; //����Ŀ¼����
// FRESULT res; /* FatFs ����ͨ�ý������ */
#if _USE_LFN//���ʹ��֧�ֳ��ļ��� �ȶ����������ʼ��
	  uint8_t P_Data[_MAX_LFN * 2 + 1];
    FilInfo.lfsize=_MAX_LFN * 2 + 1;//
    FilInfo.lfname=(TCHAR*)P_Data;//
#endif
    if(f_opendir(&DirInfo,(const TCHAR*)path) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir�ṹ���б��� */
    {
        while(f_readdir(&DirInfo, &FilInfo) == FR_OK)  /* ���ļ���Ϣ���ļ�״̬�ṹ���� */
        {
            if(!FilInfo.fname[0]) break; /* ����ļ���Ϊ��\0'��˵����ȡ��ɽ��� */
            //printf("%s/",path);//��ӡ·��
#if _USE_LFN
            //printf(" %s\r\n", FilInfo.fname);//

#endif
        }
    }
}
int Sd_f_creat_folder(char *path)
{
	FRESULT res= f_mkdir(path);
	switch(res)
	{
		case 	FR_OK: 
			puts( "success\r\n");
			return 0;
			break;
		default:
//			printf("ERR code: %d", res);
			return -1;
			break;
	}
}
int Sd_f_delete_folder( char *path)
{
	FRESULT res= f_unlink(path);
	switch(res)
	{
		case 	FR_OK: 
			puts( "success\r\n");
			return 0;
			break;
		default:
			printf("ERR code: %d", res);
			return -1;
			break;
	}
}
		
void Sd_f_write(char *path, char *p_data)
{
	int num;
	res_sd = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE );
	
	if ( res_sd == FR_OK )
	{
		f_lseek(&fnew,f_size(&fnew));
	
    /* ��ָ���洢������д�뵽�ļ��� */
//		res_sd=f_write(&fnew,p_data,sizeof(p_data),&fnum);
		
				char buffer[256] = {0};
			sprintf((char *)buffer
				,"\r\n%02d:%02d:%02d  %s" 
			,Sys_RT_Status.System_RTC.hour
			,Sys_RT_Status.System_RTC.minute
			,Sys_RT_Status.System_RTC.second
			,p_data
			);		
			 
		num =f_puts (buffer, &fnew);
    if(num!=-1)
    {
      printf("Write Number: %d\n",num);
      printf("Write Data��\r\n%s\r\n",p_data);
    }
    else
    {
      printf("Write fail\r\n");
    }    
		/* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
	}
	else
	{	
		printf("Creat file fail��\r\n");
	}
}

void Sd_f_read(char *path)
{
	

	res_sd = f_open(&fnew, path, FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{		
		printf("file open success\r\n");
		memset(ReadBuffer, 0, sizeof(ReadBuffer) );
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_sd==FR_OK)
    {
      printf("read data number: %d\r\n",fnum);
      printf("read data is :\r\n");
			for( uint16_t i=0; i<fnum; i++ )
				putchar(ReadBuffer[i]);
			puts("-->\r\n");
    }
    else
    {
      printf("read file fail: (%d)\n",res_sd);
    }		
	}
	else
	{
		printf("!! file open fail\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
}

//
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_mount, Sd_f_mount,  Sd_f_mount);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_unmount, Sd_f_unmount,  Sd_f_unmount);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_dir,   Sd_f_dir,    Sd_f_dir);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_write, Sd_f_write,  Sd_f_write);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_read,  Sd_f_read,   Sd_f_read);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_creat_folder,  Sd_f_creat_folder,   Sd_f_creat_folder);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_delete_folder, Sd_f_delete_folder,   Sd_f_delete_folder);

char File_Path[100];

//�ȴ�����Ҫ���ļ���
int Load_Cread_LogFolder()
{  
	Sd_f_mount();
	
	if(FR_Result!=FR_OK) return 0;
	for(uint16_t i=0; i<4; i++)
		Sd_f_creat_folder(&Log_Path_List[i][0]);
}
//0 ���� -1�ļ�ϵͳ����ʧ�� -2�ļ�д��ʧ��
int file_write(Log_Type viLogType, char *p_data )
{
	if(FR_Result!=FR_OK) return 0;
		memset(File_Path,0,100);
		//char buffer[100] = {0};
		sprintf((char *)File_Path
			,"/%s/20%02d-%02d-%02d.txt"
		,Log_Path_List[viLogType]
		,Sys_RT_Status.System_RTC.year
		,Sys_RT_Status.System_RTC.month
		,Sys_RT_Status.System_RTC.day);		
		 
		Sd_f_mount();
		//Log(path);
		
		Sd_f_write(File_Path,p_data);
}
//0 ���� -1�ļ�ϵͳ����ʧ�� -2�ļ�д��ʧ��
int file_read(Log_Type viLogType,  char *p_dataFileName)
{
	if(FR_Result!=FR_OK) return 0;
	 memset(File_Path,0,100);
		sprintf((char *)File_Path
			,"/%s/%s.txt"
		,Log_Path_List[viLogType]
		,p_dataFileName);
		
	Sd_f_mount(); 
	Sd_f_read(File_Path);
}
 
//
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), file_write, file_write,  file_write);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Load_Cread_LogFolder, Load_Cread_LogFolder,  Load_Cread_LogFolder);

