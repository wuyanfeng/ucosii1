#include "includes.h"

OS_FLAG_GRP *Sys_Flags;
OS_FLAG_GRP *Limit_Flags;


/*******************************************************************************
* 函数名  : Get_Limit_Status()
* 描述    : 获取按键或者限位状态		
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Get_Limit_Status(void)
{
	INT8U i,err;
	INT8U limitAction; //限位开关动作缓存
	static INT8U	Usart1_T_Buff[11]={0x02,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x03};	//串口1发送缓冲区
	static Uart_Send_Data_Struct sendTmp; 
	INT16U	limitTable[]={UP_SWITCH,DOWN_SWITCH,OK_BUTTON,OPEN_BUTTON,RASTER_IN,LEFT_MOTOR_ON_LIMIT,RIGHT_MOTOR_ON_LIMIT,
							LEFT_MOTOR_OFF_LIMIT,RIGHT_MOTOR_OFF_LIMIT};

	for(i = 0; i < 9; i++)
	{
		limitAction = GET_IN_STATUS(limitTable[i]) == 0? OS_FLAG_SET:OS_FLAG_CLR;
		OSFlagPost(Limit_Flags,UP_SWITCH_ON_FLAG << i,limitAction,&err); //设置标志位	
		if( i < 7)
		{
			if(Usart1_T_Buff[3 + i] != 0x30 + limitAction)
			{
			   Usart1_T_Buff[3 + i] = 0x30 + limitAction;
			   OSFlagPost(Sys_Flags,LIMIT_HAVE_CHANGE_FLAG,OS_FLAG_SET,&err); //设置标志位
			}
			 		
		}
	}

	if(OSFlagAccept(Sys_Flags,LIMIT_HAVE_CHANGE_FLAG,OS_FLAG_WAIT_SET_OR,&err))
	{
		OSFlagPost(Sys_Flags,LIMIT_HAVE_CHANGE_FLAG,OS_FLAG_CLR,&err); //设置标志位
		sendTmp.dataLen = 11;
		sendTmp.dataPoint = Usart1_T_Buff;
		OSQPost(CommandEvent,&sendTmp);   //将缓冲区内容发送到发送队列	
	}											 		
}

/*******************************************************************************
* 函数名  : Work_Status_Led_Set()
* 描述    : 工作状态指示灯设置		
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 正常状态下指示灯显示绿色，拉框限位开关与下门限位开关打开时红色闪烁
*******************************************************************************/
void Work_Status_Led_Set(void)
{	 
	static BitAction temp = Bit_RESET;
	static INT8U	flash_times = 0;

	INT8U err;

	OSSchedLock();

	/////////////////////工作指示灯设置//////////////////////
	if((OSFlagAccept(Limit_Flags,DOWN_SWITCH_ON_FLAG|RASTER_IN_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) != 
		(DOWN_SWITCH_ON_FLAG|RASTER_IN_ON_FLAG)) && 	//下门打开
		!OSFlagAccept(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_WAIT_SET_OR,&err))			//光栅有效
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

	///////////////////////////////按键LED设置，按下亮，松开灭//////////////////
	if(OSFlagAccept(Limit_Flags,OPEN_BUTTON_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))
		OUT_PIN_ON(BUTTON_OPEN_LED);
	else
		OUT_PIN_OFF(BUTTON_OPEN_LED);
	if(OSFlagAccept(Limit_Flags,OK_BUTTON_ON_FLAG ,OS_FLAG_WAIT_SET_OR,&err))
		OUT_PIN_ON(BUTTON_OK_LED);
	else
		OUT_PIN_OFF(BUTTON_OK_LED);
	///////////////////////////////上仓照明灯设置，开门亮，关门灭/////////////////////
	if(!OSFlagAccept(Limit_Flags,UP_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))
		OUT_PIN_ON(LIGHT_LED);
	else
		OUT_PIN_OFF(LIGHT_LED);
	OSSchedUnlock();
}

/*******************************************************************************
* 函数名  : Operation_Control()
* 描述    : 电磁锁及推杆控制		
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void Operation_Control(void)
{
	INT8U	err;

	if(OSFlagAccept(Limit_Flags,OPEN_BUTTON_ON_FLAG | DOWN_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) == 
		(OPEN_BUTTON_ON_FLAG & ~DOWN_SWITCH_ON_FLAG)) //下门打开，直接打开上门
	{
		OSFlagPost(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_SET,&err); //置位上门打开标志
		
	}
	else if(OSFlagAccept(Limit_Flags,UP_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) && 
			(!OSFlagAccept(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err)))//关闭上门，锁上电磁锁
	{
		 OUT_PIN_OFF(UP_LOCK);
	}

	//收到开门命令打开上门电磁锁
	if(OSFlagAccept(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err))
	{
		if(OSFlagAccept(Limit_Flags,UP_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))//上门完全打开之前，电磁锁一直有电
		{
		 	OUT_PIN_ON(UP_LOCK);
		}
		else
		{
			OSFlagPost(Sys_Flags,OPEN_UP_DOOR_FLAG,OS_FLAG_CLR,&err); //复位上门打开标志
		}	
	}	
	////////////////收到打开下门命令//////////////////
	if(OSFlagAccept(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err))
	{		
		if(OSFlagAccept(Limit_Flags,DOWN_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err))
		{
			OUT_PIN_ON(DOWN_LOCK);
		}
		else
		{
			OSFlagPost(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_CLR,&err); //复位下门打开标志
		}
		
	}

	if(OSFlagAccept(Limit_Flags,DOWN_SWITCH_ON_FLAG,OS_FLAG_WAIT_SET_OR,&err) && 
		(!OSFlagAccept(Sys_Flags,OPEN_DOWN_DOOR_FLAG,OS_FLAG_WAIT_SET_OR,&err)))//下门门完全打开之前，电磁锁一直有电
	{
		OUT_PIN_OFF(DOWN_LOCK);	
	}	
}
/*******************************************************************************
* 函数名  : Motor_Control()
* 描述    : 推杆控制		
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 
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
			 	OSFlagPost(Sys_Flags,OK_ACTION_FLAG,OS_FLAG_CLR,&err); 	//复位通信标志

				MOTOR_OPEN();
				OUT_PIN_OFF(RASTER_OUT);
				OSFlagPost(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_SET,&err); //置位光栅失效标志
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
				OUT_PIN_ON(RASTER_OUT);	 //打开光栅检测是否夹衣物
				OSFlagPost(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_CLR,&err); //复位光栅失效标志
			}
 		//检测推杆限位开关
			if(OSFlagAccept(Limit_Flags,LEFT_MOTOR_OFF_LIMIT_ON_FLAG | RIGHT_MOTOR_OFF_LIMIT_ON_FLAG,OS_FLAG_WAIT_SET_AND,&err)
				==(LEFT_MOTOR_OFF_LIMIT_ON_FLAG | RIGHT_MOTOR_OFF_LIMIT_ON_FLAG))
			{
				MOTOR_IDLE();
				motor_status = idle;	
			}
			break;
	}	
}
