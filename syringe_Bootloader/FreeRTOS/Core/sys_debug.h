/*******************************************************************************
** �ļ���: 		sys_debug.h
** �汾��  		1.0
** ��������: 	RealView MDK-ARM
** ����: 		Adam
** ����:		debug��س���
** ����ļ�:		��
** �޸���־��	2017-02-12   �����ĵ�
*******************************************************************************/
/* ��ֹ�ض���-----------------------------------------------------------------*/
#ifndef _CORE_DEBUG_H
#define _CORE_DEBUG_H
/* ������ͷ�ļ�---------------------------------------------------------------*/
#include "sys_config.h"

#if DEBUG_ENABLE
    void dbg_print (const char *format, ...);
//    #define DEBUG(format, ...) Debug_Printf("FILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)
    //��������Ϊ __FILE__ ���к���
//    #define DEBUG(format, ...) debug_printf("FILE: %s, LINE: %d: "format"\n", __FILE__, __LINE__, ##__VA_ARGS__)
//    #define DEBUG(format, ...) dbg_print(""format"\n", ##__VA_ARGS__)
#else
//    #define debug_printf(format, ...);
    #define dbg_print(format, ...);
#endif

#endif

/************************************�ļ�����**********************************/
