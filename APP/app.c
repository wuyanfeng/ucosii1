#include "includes.h"
#include <stdio.h>
#include <string.h>

static OS_STK work_status_task_stk[WORK_STATUS_LED_TASK_STK_SIZE];
static OS_STK comm_send_task_stk[COMM_SEND_TASK_STK_SIZE];
static OS_STK limit_test_task_stk[LIMIT_TEST_TASK_STK_SIZE];
static OS_STK operation_control_task_stk[OPERATION_CONTROL_TASK_STK_SIZE];
static OS_STK comm_recive_task_stk[COMM_RECIVE_TASK_STK_SIZE];
static OS_STK motor_control_task_stk[MOTOR_CONTROL_TASK_STK_SIZE];
static OS_STK sys_status_led_task_stk[SYS_STATUS_LED_TASK_STK_SIZE];

/*******************************************************************************
* 函数名  : Sys_Status_Led_Task()
* 描述    : 工作状态指示灯设置				   
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Sys_Status_Led_Task(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
		LED_ON();
		OSTimeDlyHMSM(0,0,0,50);
		LED_OFF();
		OSTimeDlyHMSM(0,0,0,100);
		LED_ON();
		OSTimeDlyHMSM(0,0,0,50);
		LED_OFF();
		OSTimeDlyHMSM(0,0,1,100);
	}
}

/*******************************************************************************
* 函数名  : Work_Status_Led_Task()
* 描述    : 工作状态指示灯设置				   
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 正常状态下指示灯显示绿色，拉框限位开关与下门限位开关打开时红色闪烁
*******************************************************************************/
void Work_Status_Led_Task(void *p_arg)
{
	(void)p_arg;

	while(1)
	{
		Work_Status_Led_Set();
		OSTimeDlyHMSM(0,0,0,100);
	}
}
/*******************************************************************************
* 函数名  : Get_Limit_Status_Task()
* 描述    : 获取限位开关状态任务				   
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Get_Limit_Status_Task(void *p_arg)
{
	(void)p_arg;

	while(1)
	{
		Get_Limit_Status();
		OSTimeDlyHMSM(0,0,0,100);
	}
}
/*******************************************************************************
* 函数名  : Operation_Task()
* 描述    : 动作管理任务				   
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Operation_Task(void *p_arg)
{
	(void)p_arg;
	 while(1)
	 {
	 	Operation_Control();
		OSTimeDlyHMSM(0,0,0,100);
	 }
}
/*******************************************************************************
* 函数名  : Motor_Control_Task()
* 描述    : 动作管理任务				   
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Motor_Control_Task(void *p_arg)
{
	(void)p_arg;
	 while(1)
	 {
	 	Motor_Control();
		OSTimeDlyHMSM(0,0,0,100);
	 }
}
/*******************************************************************************
* 函数名  : TASK_LED()
* 描述    : 初始任务				   
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void TASK_Start(void *p_arg)
{
	INT8U err;
	(void)p_arg;

	Sys_Flags = OSFlagCreate(0,&err);
	Limit_Flags = OSFlagCreate(0,&err);
	UART1Mbox = OSMboxCreate((void*)0);
	UART2Mbox = OSMboxCreate((void*)0);
	CommandEvent = OSQCreate(CommandBuf,COMMAND_BUF_SIZE);

	OSTaskCreate(Work_Status_Led_Task,(void*)0,&work_status_task_stk[WORK_STATUS_LED_TASK_STK_SIZE - 1],WORK_STATUS_LED_TASK_PRIO);
	OSTaskCreate(UART1_Reg_Check,(void*)0,&comm_send_task_stk[COMM_SEND_TASK_STK_SIZE - 1],COMM_SEND_TASK_PRIO);
	OSTaskCreate(Get_Limit_Status_Task,(void*)0,&limit_test_task_stk[LIMIT_TEST_TASK_STK_SIZE - 1],LIMIT_TEST_TASK_PRIO);
	OSTaskCreate(UART1_Recive_Check,(void*)0,&comm_recive_task_stk[COMM_RECIVE_TASK_STK_SIZE - 1],COMM_RECIVE_TASK_PRIO);
	OSTaskCreate(Operation_Task,(void*)0,&operation_control_task_stk[OPERATION_CONTROL_TASK_STK_SIZE - 1],OPERATION_CONTROL_TASK_PRIO);
	OSTaskCreate(Motor_Control_Task,(void*)0,&motor_control_task_stk[MOTOR_CONTROL_TASK_STK_SIZE - 1],MOTOR_CONTROL_TASK_PRIO);
	OSTaskCreate(Sys_Status_Led_Task,(void*)0,&sys_status_led_task_stk[SYS_STATUS_LED_TASK_STK_SIZE - 1],SYS_STATUS_LED_TASK_PRIO);
	while(1)
	{
	 	OSTimeDlyHMSM(0,0,10,0);
	}
}

