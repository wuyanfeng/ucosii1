#ifndef _GPIO_OUT_H_
#define _GPIO_OUT_H_

#define OUT_RCC    			RCC_APB2Periph_GPIOB
#define OUT_PORT			GPIOB
#define NORMAL_LED			GPIO_Pin_0
#define ACTION_LED			GPIO_Pin_1
#define BUTTON_OPEN_LED		GPIO_Pin_2
#define BUTTON_OK_LED		GPIO_Pin_5
#define LIGHT_LED			GPIO_Pin_6
#define UP_LOCK				GPIO_Pin_7
#define DOWN_LOCK			GPIO_Pin_8
#define LEFT_MOTOR_ON		GPIO_Pin_9
#define LEFT_MOTOR_OFF		GPIO_Pin_10
#define RIGHT_MOTOR_ON		GPIO_Pin_11
#define RIGHT_MOTOR_OFF		GPIO_Pin_12
#define RASTER_OUT			GPIO_Pin_13

#define IN_RCC     					RCC_APB2Periph_GPIOC
#define IN_PORT						GPIOC
#define UP_SWITCH					GPIO_Pin_0
#define DOWN_SWITCH					GPIO_Pin_1
#define OPEN_BUTTON					GPIO_Pin_2
#define OK_BUTTON					GPIO_Pin_3
#define RASTER_IN					GPIO_Pin_4
#define LEFT_MOTOR_ON_LIMIT			GPIO_Pin_5
#define LEFT_MOTOR_OFF_LIMIT		GPIO_Pin_6
#define RIGHT_MOTOR_ON_LIMIT		GPIO_Pin_7
#define RIGHT_MOTOR_OFF_LIMIT		GPIO_Pin_8
//#define LA_KUANG_LIMIT				GPIO_Pin_9



#define OUT_PIN_OFF(a)    GPIO_SetBits(OUT_PORT,a)
#define OUT_PIN_ON(a)	  GPIO_ResetBits(OUT_PORT,a)
#define OUT_PIN_WRITE(a,b)  GPIO_WriteBit(OUT_PORT,a,b)
#define GET_IN_STATUS(a)  GPIO_ReadInputDataBit(IN_PORT,a)

#define MOTOR_IDLE();	 OUT_PIN_OFF(LEFT_MOTOR_ON);\
						 OUT_PIN_OFF(LEFT_MOTOR_OFF);\
						 OUT_PIN_OFF(RIGHT_MOTOR_OFF);\
						 OUT_PIN_OFF(RIGHT_MOTOR_ON);

#define MOTOR_OPEN();	 OUT_PIN_ON(LEFT_MOTOR_ON);\
						 OUT_PIN_OFF(LEFT_MOTOR_OFF);\
						 OUT_PIN_ON(RIGHT_MOTOR_ON);\
						 OUT_PIN_OFF(RIGHT_MOTOR_OFF);

#define MOTOR_CLOSE();	 OUT_PIN_OFF(LEFT_MOTOR_ON);\
						 OUT_PIN_ON(LEFT_MOTOR_OFF);\
						 OUT_PIN_OFF(RIGHT_MOTOR_ON);\
						 OUT_PIN_ON(RIGHT_MOTOR_OFF);

#define LIMIT_ON			0x30
#define LIMIT_OFF			0x31

void Out_GPIO_Init(void);

void In_GPIO_Init(void);

#endif
