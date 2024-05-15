#include "stm32f4xx.h"
#include "cheakID.h"
#include "sys_core.h"
#include "string.h"
#define ID_addr 0x080C0000

unsigned __int64 p = 1061743811;//
unsigned __int64 g = 486450389;//

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  

uint16_t ID_STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;
}

void ID_FLSAH_EraseBlock(u32 BlockAddr)
{
    FLASH_Unlock();									//���� 
    FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
    
    FLASH_EraseSector(ID_STMFLASH_GetFlashSector(BlockAddr),VoltageRange_3);
    
    FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
	delay_ms(20);
}

FLASH_Status ID_FLASH_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
    FLASH_Status status = FLASH_COMPLETE; 
    FLASH_Unlock();									//���� 
    FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
 		
    delay_ms(2);
  do
  {
    /* Transfer data to the memory */
    status = FLASH_ProgramHalfWord(WriteAddr, *pBuffer++);
    WriteAddr = WriteAddr + 2;
    NumHalfwordToWrite--;
  }
  while((status == FLASH_COMPLETE) && (NumHalfwordToWrite != 0));
   FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
  return (status); 
}

/**********************************************************************************
* 核心算法
***********************************************************************************/
unsigned __int64 XpowYmodN(__int64 x, __int64 y, __int64 N)
{
	char road[64];
	__int64 tmp;
	int i = 0;
	while (y != 1)
	{
		if ((y & 1) == 0)
		{
			y /= 2;
			road[i++] = 0;
		}
		else
		{
			y = (y - 1) / 2;
			road[i++] = 1;
		}
	}
	tmp = (x % N);
	do
	{
		if (road[--i] == 0)
			tmp = ((tmp * tmp) % N);
		else
		{
			//tmp = XpowYmodN(x,(y-1)/2,N);
			tmp = ((tmp * tmp) % N);
			tmp = ((tmp * x) % N);
		}
	} while (i != 0);
	return (tmp);
}

/**********************************************************************************

***********************************************************************************/
unsigned __int64 GetPublicKey(unsigned __int64 X) {
	return XpowYmodN(g, X, p);

}
/**********************************************************************************

***********************************************************************************/
unsigned __int64  GetKey(unsigned __int64 HisPublicKey, unsigned __int64 X) {
	return XpowYmodN(HisPublicKey, X, p);
}

unsigned char k[256] = { 0 };

/*��ʼ������*/
void rc4_init(unsigned char*s, unsigned char*key, unsigned long Len)
{
	int i = 0, j = 0;
	
	//unsigned char tmp = 0;
	for (i = 0; i<256; i++)
	{
		s[i] = i;
		k[i] = key[i%Len];
	}
	for (i = 0; i<256; i++)
	{
		j = (j + s[i] + k[i]) % 256;
		s[i] ^= s[j];
		s[j] ^= s[i];
		s[i] ^= s[j];
		/*tmp = s[i];
		s[i] = s[j];//交换s[i]和s[j]
		s[j] = tmp;*/
	}
}
// 
///*�ӽ���*/
//void rc4_crypt(unsigned char*s, unsigned char*Data, unsigned long Len)
//{
//    int i = 0, j = 0, t = 0;
//    unsigned long k = 0;
//    unsigned char tmp;
//    for (k = 0; k<Len; k++)
//    {
//        i = (i + 1) % 256;
//        j = (j + s[i]) % 256;
//        //tmp = s[i];
//        //s[i] = s[j];//����s[x]��s[y]
//        //s[j] = tmp;
//        t = (s[i] + s[j]) % 256;
//        Data[k] ^= s[t];
//    }
//}

void rc4_crypt(unsigned char*s, unsigned char*Data, unsigned long Len)
{
	unsigned int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	//unsigned char tmp;
	for (k = 0; k<Len; k++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		//tmp = s[i];
		//s[i] = s[j];//交换s[x]和s[y]
		//s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		Data[k] ^= s[t];
	}
}

u8 cheak_register(__int64 prv)
{
	__int64 pub,key;
	unsigned char s[256]={0};
	u8 com_rx[50];//�ⲿ����
	memcpy(com_rx, (void *)ID_addr, 50);
	memcpy(&pub, com_rx, 8);
	key = GetKey(pub, prv);
	rc4_init(s,(u8 *)&key,8);
	
	rc4_crypt(s,com_rx+8,strlen ("both together do best of all."));
			
	if (strncmp((char *)com_rx+8,"both together do best of all.",strlen ("both together do best of all.")) == 0)
	{
		return 1;
	}
	else
		return 0;
}
unsigned char s_write[256]={0};
void wait_register(__int64 prv)
{
	__int64 pub, key;
	int last_rx_len=0;
    int stop_rx_cnt=0;
	
	u16 com_rx[25];//�ⲿ����
	while (1)
    {
        if (rx_len != 0)//���ݲ�Ϊ��
        {
            if (last_rx_len != rx_len)//���ڽ�������
            {
                last_rx_len = rx_len;
                stop_rx_cnt=0;
                delay_ms(2);
                continue;
            }
            else
            {
                ++stop_rx_cnt;
                if (stop_rx_cnt < 5)//ֹͣ��������
                {
                    delay_ms(2);
                    continue;
                }
                else
                {
                    stop_rx_cnt=0;
                }
            }
        }
        else
        {
            stop_rx_cnt=0;
            delay_ms(2);
            continue;
        }
		if (rx_len>=37)
		{
			ID_FLSAH_EraseBlock(ID_addr);
			memcpy(com_rx,com_rx_buff,50);
			memcpy(&pub, com_rx_buff, 8);
			key = GetKey(pub, prv);
			rc4_init(s_write,(u8 *)&key,8);
			rc4_crypt(s_write,com_rx_buff+8,strlen ("both together do best of all."));
			
			if (strncmp((char *)com_rx_buff+8,"both together do best of all.",strlen ("both together do best of all.")) == 0)
			{
				ID_FLASH_WriteBuffer(com_rx,ID_addr,25);
				dbg_print("OK\n");
				return ;
			}
		}
		rx_len = 0;
    }
}

void STM_Read_Cheak_ID(u16 *pBuffer)
{
	u16 i;
    u32 ReadAddr=ID_addr;//�洢��ַ
    
	for(i=0;i<6;i++)
	{
		pBuffer[i]=*(vu16*)ReadAddr;//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}
#if CHEAK_ID_ENABLE

//#include "lcd.h"
//u8 ID_CHADLLLL[8]={0x75,0x21,0xe2,0x39,0x00,0x00,0x00,0x00};
char CheckID(void)
{
	char dis_inf[50];
	int i;
	__int64 pub, prv;
    u32 temp0,temp1,temp2;

	temp0 = *(__IO u32*)(0x1FFF7A10);
    temp1 = *(__IO u32*)(0x1FFF7A14);
    temp2 = *(__IO u32*)(0x1FFF7A18);
	prv = ((__int64)temp0<<32 | temp1)+temp2;
	prv %= p;
//return 1;
	if (cheak_register(prv))
		return 1;
    pub = GetPublicKey(prv);
	//��ӡ��Կ
	//��ȡ��Կ������

	for (i=0;i<8;i++)
	{
		sprintf(dis_inf+(3*i),"%02X ",((u8 *)&pub)[i]);
	}

//    rs232_send_str(dis_inf);
	wait_register(prv);
	NVIC_SystemReset();//��λ
    return 1;

}

#endif
