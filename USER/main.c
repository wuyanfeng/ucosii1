/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：用3.5.0版本建的工程模板。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "includes.h"

static OS_STK start_task_stk[START_TASK_STK_SIZE];
/* 
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
	{
  	 BSP_Init();
	 OSInit();
	 OSTaskCreate(TASK_Start,(void*)0,&start_task_stk[START_TASK_STK_SIZE - 1],START_TASK_PRIO	);
	 OSStart();
	 return 0;
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


