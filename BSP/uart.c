#include "includes.h"

static INT8U Usart1_R_Buff[USART1_REC_MAXLEN]={0};	//����1���ݽ��ջ����� 
static INT8U Usart2_R_Buff[USART2_REC_MAXLEN]={0};	//����2���ݽ��ջ�����

static INT8U Usart1_R_Count=0;   //���ڽ��ռ���
static INT8U Usart1_T_Count=0;   //���ڷ��ͼ���
static INT8U Usart2_R_Count=0;   //���ڽ��ռ���
static INT8U Usart2_T_Count=0;   //���ڷ��ͼ���
static Uart_Send_Data_Struct txGlobalData1;
static Uart_Send_Data_Struct txGlobalData2;

OS_EVENT *UART1Mbox;
OS_EVENT *UART2Mbox;

/*******************************************************************************
* ������  : USART_SendData
* ����    : USART1��ʼ������
* ����    : ������
* ���    : ��
* ����    : �� 
* ˵��    : �����ʿ�ȡֵ��4800 9600 115200��
*******************************************************************************/
void UART2_Init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART1��GPIOA����ʱ��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	

	/*��λ����1*/
 	USART_DeInit(USART2);  
	
	/*USART1_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure);										//��ʼ������1

   /*Usart1 NVIC����*/
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���2�����ж�

    USART_Cmd(USART2, ENABLE);                    			//ʹ�ܴ��� 

	USART_ClearFlag(USART2, USART_FLAG_TC);					//���������ɱ�־
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);			//ʹ�ܴ���1��������ж�
		
}

/*******************************************************************************
* ������  : USART_SendData
* ����    : USART1��ʼ������
* ����    : ������
* ���    : ��
* ����    : �� 
* ˵��    : �����ʿ�ȡֵ��4800 9600 115200��
*******************************************************************************/
void UART1_Init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART1��GPIOA����ʱ��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	

	/*��λ����1*/
 	USART_DeInit(USART1);  
	
	/*USART1_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

   /*Usart1 NVIC����*/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���1�����ж�

    USART_Cmd(USART1, ENABLE);                    			//ʹ�ܴ��� 

	USART_ClearFlag(USART1, USART_FLAG_TC);					//���������ɱ�־
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);			//ʹ�ܴ���1��������ж�
		
}

/*******************************************************************************
* ������  : USART_SendData
* ����    : USART�������ݻ���������
* ����    : 
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USARTx_SendData(USART_TypeDef *channel,Uart_Send_Data_Struct *temp)
{   
	
	if(channel == USART1)
	{
		 txGlobalData1 = *temp;
		 Usart1_T_Count = 0;
		 USART_SendData(channel ,txGlobalData1.dataPoint[Usart1_T_Count++]);//���͵�һ�ֽ����� 
	}	
	else
	{
		txGlobalData2 = *temp;
		Usart2_T_Count = 0;
		USART_SendData(channel ,txGlobalData2.dataPoint[Usart2_T_Count++]);//���͵�һ�ֽ�����	
	}
}


/*******************************************************************************
* ������  : USART1_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART1_IRQHandler(void)                	
{
	INT8U Res=0;
	//INT8U err;

	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
	{
		Res =USART_ReceiveData(USART1); //��ȡ���յ�������(USART1->DR)

		#if USART1_ADDR_1 && USART1_ADDR_2
		#define HEAD	2
		#else
		#define HEAD	1
		#endif

		if(Usart1_R_Count < HEAD)//��ͷ
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
			//	OSFlagPost(Sys_Flags,UART1_RECIVE_SUCCESS_FLAG,OS_FLAG_SET,&err);//��λ������ɱ�־
				Usart1_R_Count = 0;	
			}
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);					//���������ɱ�־
		if((Usart1_T_Count < txGlobalData1.dataLen) && (txGlobalData1.dataLen < USART1_T_MAXXLEN))  //С�ڷ��ͳ��ȣ���������
		{
			USART_SendData(USART1 ,txGlobalData1.dataPoint[Usart1_T_Count]);
			Usart1_T_Count++;		
		}
		else
		{
			txGlobalData1.dataLen = 0;
			Usart1_T_Count = 0;
		//	USART_ITConfig(USART1, USART_IT_TC, DISABLE);			//ʹ�ܴ���1��������ж�
		}				
	}
	OSIntExit();
}
/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
	INT8U Res=0;
	//INT8U err;

	OSIntEnter();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		Res =USART_ReceiveData(USART2); //��ȡ���յ�������(USART1->DR)

		#if USART2_ADDR_1 && USART2_ADDR_2
		#define HEAD	2
		#else
		#define HEAD	1
		#endif

		if(Usart2_R_Count < HEAD)//��ͷ
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
			//	OSFlagPost(Sys_Flags,UART1_RECIVE_SUCCESS_FLAG,OS_FLAG_SET,&err);//��λ������ɱ�־
				Usart2_R_Count = 0;	
			}
		}
	}
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_TC);					//���������ɱ�־
		if((Usart2_T_Count < txGlobalData2.dataLen) && (txGlobalData2.dataLen < USART2_T_MAXXLEN))  //С�ڷ��ͳ��ȣ���������
		{
			USART_SendData(USART2 ,txGlobalData2.dataPoint[Usart2_T_Count]);
			Usart2_T_Count++;		
		}
		else
		{
			txGlobalData2.dataLen = 0;
			Usart2_T_Count = 0;
		//	USART_ITConfig(USART1, USART_IT_TC, DISABLE);			//ʹ�ܴ���1��������ж�
		}				
	}
	OSIntExit();
}
