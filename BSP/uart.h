#ifndef _UART_H_
#define _UART_H_

#define USART1_REC_MAXLEN 10
#define USART1_T_MAXXLEN 20

#define USART1_PACKET_HEAD	  0X02
#define USART1_PACKET_TAIL	  0X03

#define USART1_ADDR_1		  0X30
#define USART1_ADDR_2		  0X31

#define USART2_REC_MAXLEN 10
#define USART2_T_MAXXLEN 20

#define USART2_PACKET_HEAD	  0X02
#define USART2_PACKET_TAIL	  0X03

#define USART2_ADDR_1		  0X30
#define USART2_ADDR_2		  0X31

extern OS_EVENT *UART1Mbox;//接收到数据邮箱
extern OS_EVENT *UART2Mbox;//接收到数据邮箱

typedef struct{
	INT8U dataLen;
	INT8U *dataPoint;
}Uart_Send_Data_Struct;

///////////////////////对外接口函数/////////////////////////		
void UART1_Init(u32 bound);
void UART2_Init(u32 bound);
void USARTx_SendData(USART_TypeDef *channel,Uart_Send_Data_Struct *temp);

#endif
