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
* ������  : Sys_Status_Led_Task()
* ����    : ����״ָ̬ʾ������				   
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
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
* ������  : Work_Status_Led_Task()
* ����    : ����״ָ̬ʾ������				   
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ����״̬��ָʾ����ʾ��ɫ��������λ������������λ���ش�ʱ��ɫ��˸
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
* ������  : Get_Limit_Status_Task()
* ����    : ��ȡ��λ����״̬����				   
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
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
* ������  : Operation_Task()
* ����    : ������������				   
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
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
* ������  : Motor_Control_Task()
* ����    : ������������				   
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
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
* ������  : TASK_LED()
* ����    : ��ʼ����				   
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
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

