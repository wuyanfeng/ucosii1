#ifndef _COMM_H_
#define _COMM_H_


#define COMMAND_BUF_SIZE	10

extern OS_EVENT *CommandEvent;
extern void *CommandBuf[COMMAND_BUF_SIZE];

void UART1_Reg_Check(void *p_arg);	 //���Ĵ����仯
void UART1_Recive_Check(void *p_arg);

#endif
