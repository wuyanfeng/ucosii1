#ifndef _RECOVER_H_
#define _RECOVER_H_

////////////////////////系统标识定义////////////////////
#define LIMIT_HAVE_CHANGE_FLAG			(OS_FLAGS)1
#define UART1_RECIVE_SUCCESS_FLAG	(OS_FLAGS)2
#define OPEN_UP_DOOR_FLAG			(OS_FLAGS)4
#define OPEN_DOWN_DOOR_FLAG			(OS_FLAGS)8
#define OK_ACTION_FLAG				(OS_FLAGS)0x10
#define COMM_NULL_FLAG				(OS_FLAGS)0X20
#define RASTER_STATUS_FLAG			(OS_FLAGS)0X40

///////////////////////////限位标识定义///////////////////////
#define UP_SWITCH_ON_FLAG 			(OS_FLAGS)1
#define DOWN_SWITCH_ON_FLAG 			(OS_FLAGS)2
#define OK_BUTTON_ON_FLAG 			(OS_FLAGS)4
#define OPEN_BUTTON_ON_FLAG 			(OS_FLAGS)8
#define RASTER_IN_ON_FLAG 			(OS_FLAGS)0x10
#define LEFT_MOTOR_ON_FLAG 			(OS_FLAGS)0x20
#define RIGHT_MOTOR_ON_FLAG 			(OS_FLAGS)0x40
#define LEFT_MOTOR_OFF_LIMIT_ON_FLAG 			(OS_FLAGS)0x80
#define RIGHT_MOTOR_OFF_LIMIT_ON_FLAG 			(OS_FLAGS)0x100

typedef enum {idle,openning,closing,open_complete,close_complete,waiting}motorStatus;


extern OS_FLAG_GRP *Sys_Flags;
extern OS_FLAG_GRP *Limit_Flags;


void Work_Status_Led_Set(void);
void Botton_Status_Led_Set(void);
void Get_Limit_Status(void);
void Operation_Control(void);
void Motor_Control(void);

#endif
