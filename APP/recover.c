#include "includes.h"

OS_FLAG_GRP *Sys_Flags;
OS_FLAG_GRP *Limit_Flags;


/*******************************************************************************
* ������  : Get_Limit_Status()
* ����    : ��ȡ����������λ״̬		
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void Get_Limit_Status(void)
{
	INT8U i,err;
	INT8U limitAction; //��λ���ض�������
	static INT8U	Usart1_T_Buff[11]={0x02,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x03};	//����1���ͻ�����
	static Uart_Send_Data_Struct sendTmp; 
	INT16U	limitTable[]={UP_SWITCH,DOWN_SWITCH,OK_BUTTON,OPEN_BUTTON,RASTER_IN,LEFT_MOTOR_ON_LIMIT,RIGHT_MOTOR_ON_LIMIT,
							LEFT_MOTOR_OFF_LIMIT,RIGHT_MOTOR_OFF_LIMIT};

	for(i = 0; i < 9; i++)
	{
		limitAction = GET_IN_STATUS(limitTable[i]) == 0? OS_FLAG_SET:OS_FLAG_CLR;
		OSFlagPost(Limit_Flags,UP_SWITCH_ON_FLAG << i,limitAction,&err); //���ñ�־λ	
		if( i < 7)
		{
			if(Usart1_T_Buff[3 + i] != 0x30 + limitAction)
			{
			   Usart1_T_Buff[3 + i] = 0x30 + limitAction;
			   OSFlagPost(Sys_Flags,LIMIT_HAVE_CHANGE_FLAG,OS_FLAG_SET,&err); //���ñ�־λ
			}
			 		
		}
	}

	if(OSFlagAccept(Sys_Flags,LIMIT_HAVE_CHANGE_FLAG,OS_FLAG_WAIT_SET_OR,&err))
	{
		OSFlagPost(Sys_Flags,LIMIT_HAVE_CHANGE_FLAG,OS_FLAG_CLR,&err); //���ñ�־λ
		sendTmp.dataLen = 11;
		sendTmp.dataPoint = Usart1_T_Buff;
		OSQPost(CommandEvent,&sendTmp);   //�����������ݷ��͵����Ͷ���	
	}											 		
}

/*******************************************************************************
* ������  : Work_Status_Led_Set()
* ����    : ����״ָ̬ʾ������		
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ����״̬��ָʾ����ʾ��ɫ��������λ������������λ���ش�ʱ��ɫ��˸
*******************************************************************************/
void Work_Status_Led_Set(void)
{	 
	static BitAction temp = Bit_RESET;
	static INT8U	flash_times = 0;

	INT8U err;

	OSSchedLock();

	/////////////////////����ָʾ������//////////////////////
	if((OSFlagAccept(Limit_Flags,DOWN_SWITCH_ON_FLAG|RASTER_IN_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) != 
		(DOWN_SWITCH_ON_FLAG|RASTER_IN_ON_FLAG)) && 	//���Ŵ�
		!OSFlagAccept(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_WAIT_SET_OR,&err))			//��դ��Ч
	{	
		if(flash_times < 5)
			flash_times++;
		else
		{
			flash_times = 0;
			OUT_PIN_OFF(NORMAL_LED);
			OUT_PIN_ON(ACTION_LED);
			OUT_PIN_WRITE(ACTION_LED,temp);
			temp ^= 1;
		}

	}
	else
	{
	 	OUT_PIN_ON(NORMAL_LED);
		OUT_PIN_OFF(ACTION_LED);
	}

	///////////////////////////////����LED���ã����������ɿ���//////////////////
	if(OSFlagAccept(Limit_Flags,OPEN_BUTTON_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))
		OUT_PIN_ON(BUTTON_OPEN_LED);
	else
		OUT_PIN_OFF(BUTTON_OPEN_LED);
	if(OSFlagAccept(Limit_Flags,OK_BUTTON_ON_FLAG ,OS_FLAG_WAIT_SET_OR,&err))
		OUT_PIN_ON(BUTTON_OK_LED);
	else
		OUT_PIN_OFF(BUTTON_OK_LED);
	///////////////////////////////�ϲ����������ã���������������/////////////////////
	if(!OSFlagAccept(Limit_Flags,UP_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))
		OUT_PIN_ON(LIGHT_LED);
	else
		OUT_PIN_OFF(LIGHT_LED);
	OSSchedUnlock();
}

/*******************************************************************************
* ������  : Operation_Control()
* ����    : ��������Ƹ˿���		
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void Operation_Control(void)
{
	INT8U	err;

	if(OSFlagAccept(Limit_Flags,OPEN_BUTTON_ON_FLAG | DOWN_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) == 
		(OPEN_BUTTON_ON_FLAG & ~DOWN_SWITCH_ON_FLAG)) //���Ŵ򿪣�ֱ�Ӵ�����
	{
		OSFlagPost(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_SET,&err); //��λ���Ŵ򿪱�־
		
	}
	else if(OSFlagAccept(Limit_Flags,UP_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) && 
			(!OSFlagAccept(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err)))//�ر����ţ����ϵ����
	{
		 OUT_PIN_OFF(UP_LOCK);
	}

	//�յ�������������ŵ����
	if(OSFlagAccept(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err))
	{
		if(OSFlagAccept(Limit_Flags,UP_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))//������ȫ��֮ǰ�������һֱ�е�
		{
		 	OUT_PIN_ON(UP_LOCK);
		}
		else
		{
			OSFlagPost(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_CLR,&err); //��λ���Ŵ򿪱�־
		}	
	}	
	////////////////�յ�����������//////////////////
	if(OSFlagAccept(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err))
	{		
		if(OSFlagAccept(Limit_Flags,DOWN_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))
		{
			OUT_PIN_ON(DOWN_LOCK);
		}
		else
		{
			OSFlagPost(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_CLR,&err); //��λ���Ŵ򿪱�־
		}
		
	}

	if(OSFlagAccept(Limit_Flags,DOWN_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) && 
		(!OSFlagAccept(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err)))//��������ȫ��֮ǰ�������һֱ�е�
	{
		OUT_PIN_OFF(DOWN_LOCK);	
	}	
}
/*******************************************************************************
* ������  : Motor_Control()
* ����    : �Ƹ˿���		
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void Motor_Control(void)
{
	static INT8U	waitTime = 0;
	static motorStatus motor_status=idle;
	INT8U err;


	switch(motor_status)
	{
		case idle:
				OSFlagPend(Sys_Flags,OK_ACTION_FLAG,OS_FLAG_WAIT_SET_OR,0,&err);
			 	OSFlagPost(Sys_Flags,OK_ACTION_FLAG,OS_FLAG_CLR,&err); 	//��λͨ�ű�־

				MOTOR_OPEN();
				OUT_PIN_OFF(RASTER_OUT);
				OSFlagPost(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_SET,&err); //��λ��դʧЧ��־
				motor_status = openning;
			break;
		case openning:
			if(OSFlagAccept(Limit_Flags,LEFT_MOTOR_ON_FLAG | RIGHT_MOTOR_ON_FLAG ,OS_FLAG_WAIT_SET_AND,&err) 
				== (LEFT_MOTOR_ON_FLAG | RIGHT_MOTOR_ON_FLAG))
			{
				MOTOR_IDLE();
				motor_status = waiting;
			}
			break;
		case waiting:
			if(waitTime < 10)
			{
			  	OSTimeDlyHMSM(0,0,0,10);
				waitTime++;
			}
			else
			{
				waitTime = 0;
 				MOTOR_CLOSE();
				motor_status = closing;
			}
			break;
		case closing:
			if(waitTime < 10)
			{
			  	OSTimeDly(1000);
				waitTime++;
			}
			else
			{
				OUT_PIN_ON(RASTER_OUT);	 //�򿪹�դ����Ƿ������
				OSFlagPost(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_CLR,&err); //��λ��դʧЧ��־
			}
 		//����Ƹ���λ����
			if(OSFlagAccept(Limit_Flags,LEFT_MOTOR_OFF_LIMIT_ON_FLAG | RIGHT_MOTOR_OFF_LIMIT_ON_FLAG,OS_FLAG_WAIT_SET_AND,&err)
				==(LEFT_MOTOR_OFF_LIMIT_ON_FLAG | RIGHT_MOTOR_OFF_LIMIT_ON_FLAG))
			{
				MOTOR_IDLE();
				motor_status = idle;	
			}
			break;
	}	
}
