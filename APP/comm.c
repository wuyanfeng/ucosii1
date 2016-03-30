#include "includes.h"

OS_EVENT *CommandEvent;
void *CommandBuf[COMMAND_BUF_SIZE];

/*******************************************************************************
* 函数名  : UART1_Recive_Check
* 描述    : 串口1接收命令检测
* 输入    : 波特率
* 输出    : 无
* 返回    : 无 
* 说明   
*******************************************************************************/
#define GET_USART1_UP_DOOR_RREG		msg[3]
#define GET_USART1_DOWN_DOOR_RREG   msg[5]
#define GET_USART1_MOTOR_RREG		msg[4]
void UART1_Recive_Check(void *p_arg)
{
	INT8U err;
	INT8U *msg;
	(void)p_arg;

	while(1)
	{
		msg = OSMboxPend(UART1Mbox,0,&err);
		////////////////////////串口接收检测/////////////////////
		if(GET_USART1_UP_DOOR_RREG == 0x31 ) //同意打开上门
		{
			OSFlagPost(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_SET,&err); //置位上门打开标志
		}

		if(GET_USART1_MOTOR_RREG == 0x31) //同意打开推杆
		{
			OSFlagPost(Sys_Flags,OK_ACTION_FLAG,OS_FLAG_SET,&err); //置位推杆动作标志
		}

		if(GET_USART1_DOWN_DOOR_RREG == 0x31)//打开下门
		{
			OSFlagPost(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_SET,&err); //置位下门打开标志
		}
		
		OSTimeDlyHMSM(0,0,0,100);	
	}
}

/*******************************************************************************
* 函数名  : UART1_Reg_Check
* 描述    : 检测串口1的寄存器，一有变化立即发出命令
* 输入    : 波特率
* 输出    : 无
* 返回    : 无 
* 说明    :  数据寄存器为Usart1_T_Buff[3]~Usart1_T_Buff[9]
*******************************************************************************/
void UART1_Reg_Check(void *p_arg)
{
	INT8U	err;
	Uart_Send_Data_Struct	*msg;
	
	(void)p_arg;

	 while(1)
	 {
		msg = (Uart_Send_Data_Struct*)OSQPend(CommandEvent,0,&err);//发送队列为空则等待

		if(err == OS_NO_ERR)
		{
			USARTx_SendData(USART1,msg);//发送寄存器数据
			OSTimeDlyHMSM(0,0,0,300);		
		}
	 
	 }
}
