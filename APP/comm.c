#include "includes.h"

OS_EVENT *CommandEvent;
void *CommandBuf[COMMAND_BUF_SIZE];

/*******************************************************************************
* ������  : UART1_Recive_Check
* ����    : ����1����������
* ����    : ������
* ���    : ��
* ����    : �� 
* ˵��   
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
		////////////////////////���ڽ��ռ��/////////////////////
		if(GET_USART1_UP_DOOR_RREG == 0x31 ) //ͬ�������
		{
			OSFlagPost(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_SET,&err); //��λ���Ŵ򿪱�־
		}

		if(GET_USART1_MOTOR_RREG == 0x31) //ͬ����Ƹ�
		{
			OSFlagPost(Sys_Flags,OK_ACTION_FLAG,OS_FLAG_SET,&err); //��λ�Ƹ˶�����־
		}

		if(GET_USART1_DOWN_DOOR_RREG == 0x31)//������
		{
			OSFlagPost(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_SET,&err); //��λ���Ŵ򿪱�־
		}
		
		OSTimeDlyHMSM(0,0,0,100);	
	}
}

/*******************************************************************************
* ������  : UART1_Reg_Check
* ����    : ��⴮��1�ļĴ�����һ�б仯������������
* ����    : ������
* ���    : ��
* ����    : �� 
* ˵��    :  ���ݼĴ���ΪUsart1_T_Buff[3]~Usart1_T_Buff[9]
*******************************************************************************/
void UART1_Reg_Check(void *p_arg)
{
	INT8U	err;
	Uart_Send_Data_Struct	*msg;
	
	(void)p_arg;

	 while(1)
	 {
		msg = (Uart_Send_Data_Struct*)OSQPend(CommandEvent,0,&err);//���Ͷ���Ϊ����ȴ�

		if(err == OS_NO_ERR)
		{
			USARTx_SendData(USART1,msg);//���ͼĴ�������
			OSTimeDlyHMSM(0,0,0,300);		
		}
	 
	 }
}
