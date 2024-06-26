/*******************************************************************************
** 文件名: 		sys_wdg.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM
** 作者: 		Adam
** 生成日期: 	2017-02-12
** 功能:		看门狗
** 相关文件:		无
** 修改日志：	2017-02-12   创建文档
*******************************************************************************/
/* 包含的头文件---------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "sys_wdg.h"
#include "stm32f4xx_iwdg.h"

#if WDG_ENABLE//使能看门狗功能


/*******************************************************************************
- * @函数名称	dwg_init
- * @函数说明   独立看门狗功能初始化
- * @输入参数   无
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
/*void dwg_init(void) 
{
    //时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
    u8 prer=4;
    u16 rlr=625;
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
	
	IWDG_SetPrescaler(prer);  //设置IWDG预分频值:设置IWDG预分频值为64
	
	IWDG_SetReload(rlr);  //设置IWDG重装载值
	
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
	
	IWDG_Enable();  //使能IWDG
    
	dwg_feed();
}*/

/*******************************************************************************
- * @函数名称	dwg_feed
- * @函数说明   独立看门狗喂狗
- * @输入参数   无
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
/*void dwg_feed(void)
{
 	IWDG_ReloadCounter();//reload										   
}*/
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
	
	IWDG_SetPrescaler(prer); //设置IWDG分频系数

	IWDG_SetReload(rlr);   //设置IWDG装载值

	IWDG_ReloadCounter(); //reload
	
	IWDG_Enable();       //使能看门狗
}

//喂独立看门狗
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}


#endif

/************************************文件结束*********************************/