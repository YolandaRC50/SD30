/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/*  Porting by Michael Vysotsky <michaelvy@hotmail.com> August 2011   */

#define SYS_ARCH_GLOBALS

/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/lwip_sys.h"
#include "lwip/mem.h"
#include "delay.h"
#include "arch/sys_arch.h"
#include "FreeRTOS.h"
#include "task.h"
#include "malloc.h"



const uint32_t NullMessage;

//创建一个消息邮箱
//*mbox:消息邮箱
//size:邮箱大小
//返回值:ERR_OK,创建成功
//         其他,创建失败
err_t sys_mbox_new( sys_mbox_t *mbox, int size)
{
	(void )	size;														//if(size>MAX_QUEUE_ENTRIES)size=MAX_QUEUE_ENTRIES;原句是这一句，导致出错，DHCP不能使用
	mbox->xQueue = xQueueCreate(MAX_QUEUE_ENTRIES,sizeof(void *));		//创建消息队列，该消息队列存放指针
	LWIP_ASSERT("OSQCreate",mbox->xQueue!=NULL);
	if(mbox->xQueue==NULL) return ERR_MEM;	//消息队列创建错误 
	
	return ERR_OK; 					//返回ERR_OK，表示消息队列创建成功
} 
//释放并删除一个消息邮箱
//*mbox:要删除的消息邮箱
void sys_mbox_free(sys_mbox_t * mbox)
{
	vQueueDelete(mbox->xQueue);
	//LWIP_ASSERT("OSQDel",mbox->xQueue!=NULL);
	mbox->xQueue=NULL;
}
//向消息邮箱中发送一条消息(必须发送成功)
//*mbox:消息邮箱
//*msg:要发送的消息
void sys_mbox_post(sys_mbox_t *mbox,void *msg)
{    
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(msg==NULL)
		msg = (void*)&NullMessage;	//当msg为空时 msg等于pvNullPointer指向的值,这一句有待考察
	//if((SCB_ICSR_REG&0xFF) == 0)	//线程执行
	//{
		while(xQueueSendToBack(mbox->xQueue, &msg, portMAX_DELAY) != pdPASS);	//portMAX_DELAY,死等知道发送成功
	//}
	//else
	//{
	//	while(xQueueSendToBackFromISR(mbox->xQueue, &msg, &xHigherPriorityTaskWoken) != pdPASS);
	//	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	//}
}
//尝试向一个消息邮箱发送消息
//此函数相对于sys_mbox_post函数只发送一次消息，
//发送失败后不会尝试第二次发送
//*mbox:消息邮箱
//*msg:要发送的消息
//返回值:ERR_OK,发送OK
// 	     ERR_MEM,发送失败
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{ 
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(msg==NULL)
		msg = (void*)&NullMessage;	//当msg为空时 msg等于pvNullPointer指向的值,这一句有待考察
	if((SCB_ICSR_REG&0xFF) == 0)	//线程执行
	{
		if(xQueueSendToBack(mbox->xQueue, &msg, 0) != pdPASS)	//portMAX_DELAY,死等知道发送成功
			return ERR_MEM;
	}
	else
	{
		if(xQueueSendToBackFromISR(mbox->xQueue, &msg, &xHigherPriorityTaskWoken) != pdPASS)
			return ERR_MEM;
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	return ERR_OK;
}

//等待邮箱中的消息
//*mbox:消息邮箱
//*msg:消息
//timeout:超时时间，如果timeout为0的话,就一直等待
//返回值:当timeout不为0时如果成功的话就返回等待的时间，
//		失败的话就返回超时SYS_ARCH_TIMEOUT
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{ 
	u32_t rtos_timeout,timeout_new;
	BaseType_t temp;
	
	temp=xQueueReceive(mbox->xQueue,msg,0);
	if((temp == pdPASS)&&(*msg != NULL))
	{
		if(*msg == (void*)&NullMessage) *msg = NULL;
		return 0;
	}
	
	if(timeout!=0)
	{
		rtos_timeout=(timeout*configTICK_RATE_HZ)/1000;	//转换为节拍数，因为FreeRTOS延时使用的是节拍数，而LWIP是用ms
		if(rtos_timeout<1)
		{
			rtos_timeout=1;//至少1个节拍
		}
		else if(rtos_timeout >= portMAX_DELAY)
		{
			rtos_timeout = portMAX_DELAY - 1;
		}
	}else rtos_timeout = 0;
	timeout = HAL_GetTick();	//原函数这里使用了HAL_GitTick()，xTaskGetTickCount()可以代替获取时钟节拍数
	if(rtos_timeout != 0)
		temp = xQueueReceive(mbox->xQueue,msg,rtos_timeout);	//请求消息队列，等待时限为rtos_timwout
	else
		temp = xQueueReceive(mbox->xQueue,msg,portMAX_DELAY);	//为0则无限等
	
	if(temp == errQUEUE_EMPTY)
	{
		timeout=SYS_ARCH_TIMEOUT;	//请求超时
		*msg = NULL;
	}	
	else
	{
		if(*msg!=NULL)
		{
			if(*msg == (void*)&NullMessage)
				*msg = NULL;
		}
		timeout_new =  HAL_GetTick();	//原函数这里使用了HAL_GetTick()，xTaskGetTickCount()可以代替获取时钟节拍数
		if(timeout_new>timeout) timeout_new = timeout_new - timeout;//算出请求消息或使用的时间
		else timeout_new = 0xffffffff - timeout + timeout_new;
		timeout=timeout_new*1000/configTICK_RATE_HZ + 1;
	}
	return timeout;
}
//尝试获取消息
//*mbox:消息邮箱
//*msg:消息
//返回值:等待消息所用的时间/SYS_ARCH_TIMEOUT
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	return sys_arch_mbox_fetch(mbox,msg,1);//尝试获取一个消息
}
//检查一个消息邮箱是否有效
//*mbox:消息邮箱
//返回值:1,有效.
//      0,无效
int sys_mbox_valid(sys_mbox_t *mbox)
{  
	if(mbox->xQueue != NULL)
	{
		return 1;
	}
	return 0;
} 
//设置一个消息邮箱为无效
//*mbox:消息邮箱
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	mbox->xQueue=NULL;
} 
//创建一个信号量
//*sem:创建的信号量
//count:信号量值
//返回值:ERR_OK,创建OK
// 	     ERR_MEM,创建失败
err_t sys_sem_new(sys_sem_t * sem, u8_t count)
{  
	*sem=xSemaphoreCreateCounting(0xFF,count);
	if(*sem==NULL)return ERR_MEM; 
	LWIP_ASSERT("OSSemCreate ",*sem != NULL );
	return ERR_OK;
} 
//等待一个信号量
//*sem:要等待的信号量
//timeout:超时时间
//返回值:当timeout不为0时如果成功的话就返回等待的时间，
//		失败的话就返回超时SYS_ARCH_TIMEOUT
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{ 
	u32_t rtos_timeout,timeout_new;
	BaseType_t temp;
	if(xSemaphoreTake(*sem,0) == pdPASS)
	{
		return 0;
	}
	if(timeout!=0)
	{
		rtos_timeout=(timeout * configTICK_RATE_HZ)/1000;	//转换为节拍数，因为FreeRTOS延时使用的是节拍数，而LWIP是用ms
		if(rtos_timeout<1)
		{
			rtos_timeout=1;//至少1个节拍
		}
	}else rtos_timeout = 0;
	timeout = HAL_GetTick();	//原函数这里使用了HAL_GitTick()，xTaskGetTickCount()可以代替获取时钟节拍数
	if(rtos_timeout != 0)
		temp = xSemaphoreTake(*sem,rtos_timeout);	//请求信号量，等待时限为rtos_timwout
	else
		temp = xSemaphoreTake(*sem,portMAX_DELAY);	//为0则无限等
	
	if(temp != pdPASS)
	{
		timeout=SYS_ARCH_TIMEOUT;	//请求超时
	}	
	else
	{
		timeout_new =  HAL_GetTick();	//原函数这里使用了HAL_GitTick()，xTaskGetTickCount()可以代替获取时钟节拍数
		if(timeout_new>timeout) timeout_new = timeout_new - timeout;//算出请求消息或使用的时间
		else timeout_new = 0xffffffff - timeout + timeout_new;
		timeout=timeout_new*1000/configTICK_RATE_HZ + 1;
	}
	return timeout;
}
//发送一个信号量
//sem:信号量指针
void sys_sem_signal(sys_sem_t *sem)
{
	BaseType_t pxHigherPriorityTaskWoken;
	if((SCB_ICSR_REG&0xFF) == 0)
	{
		xSemaphoreGive(*sem);
	}
	else
	{
		xSemaphoreGiveFromISR(*sem,&pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);//如果需要，进行任务切换
	}
}
//释放并删除一个信号量
//sem:信号量指针
void sys_sem_free(sys_sem_t *sem)
{
	vSemaphoreDelete(*sem);
	*sem = NULL;
} 
//查询一个信号量的状态,无效或有效
//sem:信号量指针
//返回值:1,有效.
//      0,无效
int sys_sem_valid(sys_sem_t *sem)
{
	if(*sem != NULL)
		return 1;
	else
		return 0;
} 
//设置一个信号量无效
//sem:信号量指针
void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem=NULL;
} 
//arch初始化
void sys_init(void)
{ 
    //这里,我们在该函数,不做任何事情
} 

TaskHandle_t LWIP_ThreadHandler;
//创建一个新进程
//*name:进程名称
//thred:进程任务函数
//*arg:进程任务函数的参数
//stacksize:进程任务的堆栈大小
//prio:进程任务的优先级
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	taskENTER_CRITICAL(); //进入临界区
	xTaskCreate((TaskFunction_t )thread,             
                (const char*    )name,           
                (uint16_t       )stacksize,        
                (void*          )arg,                  
                (UBaseType_t    )prio,        
                (TaskHandle_t*  )&LWIP_ThreadHandler); //创建TCP IP内核任务
	taskEXIT_CRITICAL();	//退出临界区
	return 0;
} 
//lwip延时函数 避免重定义
//ms:要延时的ms数
void sys_msleep(u32_t ms)
{
	delay_ms(ms);
}
//获取系统时间,LWIP1.4.1增加的函数
//返回值:当前系统时间(单位:毫秒)
u32_t sys_now(void)
{
	u32_t ucos_time, lwip_time;
	ucos_time = HAL_GetTick();	//获取当前系统时间 得到的是FreeRTOS的节拍数  xTaskGetTickCount();应该是可以代替
	lwip_time=(ucos_time*1000/configTICK_RATE_HZ+1);//将节拍数转换为LWIP的时间MS
	return lwip_time; 		//返回lwip_time;
}
/**************************************--ADD--**************************************************/
/*用在cc.h的SYS_ARCH_PROTECT(lev)*/
uint32_t Enter_Critical(void)
{
	if(SCB_ICSR_REG&0xFF)	//在中断里
	{
		return taskENTER_CRITICAL_FROM_ISR();
	}
	else	//在线程里
	{
		taskENTER_CRITICAL();
		return 0;
	}
}

/*用在cc.h的SYS_ARCH_UNPROTECT(lev)*/
void Exit_Critical(uint32_t lev)
{
	if(SCB_ICSR_REG&0xFF)	//在中断里
	{
		taskEXIT_CRITICAL_FROM_ISR(lev);
	}
	else	//在线程里
	{
		taskEXIT_CRITICAL();
	}
}
