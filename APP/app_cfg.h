/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   IAR STM32-SK Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*******************************设置任务优先级**************************************/

#define START_TASK_PRIO				4
#define WORK_STATUS_LED_TASK_PRIO	10
#define COMM_SEND_TASK_PRIO			7
#define LIMIT_TEST_TASK_PRIO        5
#define OPERATION_CONTROL_TASK_PRIO	8
#define COMM_RECIVE_TASK_PRIO		6
#define MOTOR_CONTROL_TASK_PRIO		9
#define SYS_STATUS_LED_TASK_PRIO	20

#define START_TASK_STK_SIZE					80
#define WORK_STATUS_LED_TASK_STK_SIZE   	80
#define COMM_SEND_TASK_STK_SIZE				128
#define LIMIT_TEST_TASK_STK_SIZE			80
#define OPERATION_CONTROL_TASK_STK_SIZE		80
#define COMM_RECIVE_TASK_STK_SIZE			128
#define MOTOR_CONTROL_TASK_STK_SIZE			80
#define SYS_STATUS_LED_TASK_STK_SIZE		40

#endif
