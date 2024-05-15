#ifndef __IAP_H__
#define __IAP_H__  

#include "stm32f4xx.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

void MSR_MSP(u32 addr);	//���ö�ջ��ַ 
extern uint8_t jump_flag;
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.   
#define FLASH_APP1_ADDR		0x08040000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
											//����0X08000000~0X0800FFFF�Ŀռ�ΪBootloaderʹ��(��64KB)	   
void iap_load_app(u32 appxaddr);			//��ת��APP����ִ��
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin
#endif







































