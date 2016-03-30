#include "includes.h"

static INT8U Usart1_R_Buff[USART1_REC_MAXLEN]={0};	//串口1数据接收缓冲区 
static INT8U Usart2_R_Buff[USART2_REC_MAXLEN]={0};	//串口2数据接收缓冲区

static INT8U Usart1_R_Count=0;   //串口接收计数
static INT8U Usart1_T_Count=0;   //串口发送计数
static INT8U Usart2_R_Count=0;   //串口接收计数
static INT8U Usart2_T_Count=0;   //串口发送计数
static Uart_Send_Data_Struct txGlobalData1;
static Uart_Send_Data_Struct txGlobalData2;

OS_EVENT *UART1Mbox;
OS_EVENT *UART2Mbox;

/*******************************************************************************
* 函数名  : USART_SendData
* 描述    : USART1初始化函数
* 输入    : 波特率
* 输出    : 无
* 返回    : 无 
* 说明    : 波特率可取值：4800 9600 115200等
*******************************************************************************/
void UART2_Init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*使能USART1和GPIOA外设时钟*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	

	/*复位串口1*/
 	USART_DeInit(USART2);  
	
	/*USART1_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)


   /*USART1 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
    USART_Init(USART2, &USART_InitStructure);										//初始化串口1

   /*Usart1 NVIC配置*/
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
	  
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能串口2接收中断

    USART_Cmd(USART2, ENABLE);                    			//使能串口 

	USART_ClearFlag(USART2, USART_FLAG_TC);					//清除发送完成标志
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);			//使能串口1发送完成中断
		
}

/*******************************************************************************
* 函数名  : USART_SendData
* 描述    : USART1初始化函数
* 输入    : 波特率
* 输出    : 无
* 返回    : 无 
* 说明    : 波特率可取值：4800 9600 115200等
*******************************************************************************/
void UART1_Init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*使能USART1和GPIOA外设时钟*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	

	/*复位串口1*/
 	USART_DeInit(USART1);  
	
	/*USART1_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)


   /*USART1 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
    USART_Init(USART1, &USART_InitStructure);										//初始化串口1

   /*Usart1 NVIC配置*/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
	  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//使能串口1接收中断

    USART_Cmd(USART1, ENABLE);                    			//使能串口 

	USART_ClearFlag(USART1, USART_FLAG_TC);					//清除发送完成标志
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);			//使能串口1发送完成中断
		
}

/*******************************************************************************
* 函数名  : USART_SendData
* 描述    : USART发送数据缓冲区数据
* 输入    : 
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void USARTx_SendData(USART_TypeDef *channel,Uart_Send_Data_Struct *temp)
{   
	
	if(channel == USART1)
	{
		 txGlobalData1 = *temp;
		 Usart1_T_Count = 0;
		 USART_SendData(channel ,txGlobalData1.dataPoint[Usart1_T_Count++]);//发送第一字节数据 
	}	
	else
	{
		txGlobalData2 = *temp;
		Usart2_T_Count = 0;
		USART_SendData(channel ,txGlobalData2.dataPoint[Usart2_T_Count++]);//发送第一字节数据	
	}
}


/*******************************************************************************
* 函数名  : USART1_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void USART1_IRQHandler(void)                	
{
	INT8U Res=0;
	//INT8U err;

	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{
		Res =USART_ReceiveData(USART1); //读取接收到的数据(USART1->DR)

		#if USART1_ADDR_1 && USART1_ADDR_2
		#define HEAD	2
		#else
		#define HEAD	1
		#endif

		if(Usart1_R_Count < HEAD)//包头
		{
			Usart1_R_Buff[Usart1_R_Count++] = Res;				
		}
		else if(Usart1_R_Count == HEAD)
		{
			#if USART1_ADDR_1 && USART1_ADDR_2
			if(Usart1_R_Buff[0] == USART1_PACKET_HEAD && Usart1_R_Buff[1] == USART1_ADDR_1 && Res == USART1_ADDR_2)
			{
				Usart1_R_Buff[Usart1_R_Count++] = Res;	
			}
			#else
			if(Usart1_R_Buff[0] == USART1_PACKET_HEAD)
			{
				Usart1_R_Buff[Usart1_R_Count++] = Res;	
			}
			#endif

			else
			{
				Usart1_R_Count = 0;
			}
		}
		else if(Usart1_R_Count > HEAD)
		{
			if(Usart1_R_Count < USART1_REC_MAXLEN && Res != USART1_PACKET_TAIL)
			{
				Usart1_R_Buff[Usart1_R_Count++] = Res;
			}
			else
			{
				Usart1_R_Buff[Usart1_R_Count++] = Res;
				OSMboxPost(UART1Mbox,Usart1_R_Buff);
			//	OSFlagPost(Sys_Flags,UART1_RECIVE_SUCCESS_FLAG,OS_FLAG_SET,&err);//置位接收完成标志
				Usart1_R_Count = 0;	
			}
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);					//清除发送完成标志
		if((Usart1_T_Count < txGlobalData1.dataLen) && (txGlobalData1.dataLen < USART1_T_MAXXLEN))  //小于发送长度，继续发送
		{
			USART_SendData(USART1 ,txGlobalData1.dataPoint[Usart1_T_Count]);
			Usart1_T_Count++;		
		}
		else
		{
			txGlobalData1.dataLen = 0;
			Usart1_T_Count = 0;
		//	USART_ITConfig(USART1, USART_IT_TC, DISABLE);			//使能串口1发送完成中断
		}				
	}
	OSIntExit();
}
/*******************************************************************************
* 函数名  : USART2_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
	INT8U Res=0;
	//INT8U err;

	OSIntEnter();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
	{
		Res =USART_ReceiveData(USART2); //读取接收到的数据(USART1->DR)

		#if USART2_ADDR_1 && USART2_ADDR_2
		#define HEAD	2
		#else
		#define HEAD	1
		#endif

		if(Usart2_R_Count < HEAD)//包头
		{
			Usart2_R_Buff[Usart2_R_Count++] = Res;				
		}
		else if(Usart2_R_Count == HEAD)
		{
			#if USART2_ADDR_1 && USART2_ADDR_2
			if(Usart2_R_Buff[0] == USART2_PACKET_HEAD && Usart2_R_Buff[1] == USART2_ADDR_1 && Res == USART2_ADDR_2)
			{
				Usart2_R_Buff[Usart2_R_Count++] = Res;	
			}
			#else
			if(Usart2_R_Buff[0] == USART2_PACKET_HEAD)
			{
				Usart2_R_Buff[Usart2_R_Count++] = Res;	
			}
			#endif

			else
			{
				Usart2_R_Count = 0;
			}
		}
		else if(Usart2_R_Count > HEAD)
		{
			if(Usart2_R_Count < USART2_REC_MAXLEN && Res != USART2_PACKET_TAIL)
			{
				Usart2_R_Buff[Usart2_R_Count++] = Res;
			}
			else
			{
				Usart2_R_Buff[Usart2_R_Count++] = Res;
				OSMboxPost(UART2Mbox,Usart2_R_Buff);
			//	OSFlagPost(Sys_Flags,UART1_RECIVE_SUCCESS_FLAG,OS_FLAG_SET,&err);//置位接收完成标志
				Usart2_R_Count = 0;	
			}
		}
	}
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_TC);					//清除发送完成标志
		if((Usart2_T_Count < txGlobalData2.dataLen) && (txGlobalData2.dataLen < USART2_T_MAXXLEN))  //小于发送长度，继续发送
		{
			USART_SendData(USART2 ,txGlobalData2.dataPoint[Usart2_T_Count]);
			Usart2_T_Count++;		
		}
		else
		{
			txGlobalData2.dataLen = 0;
			Usart2_T_Count = 0;
		//	USART_ITConfig(USART1, USART_IT_TC, DISABLE);			//使能串口1发送完成中断
		}				
	}
	OSIntExit();
}
