#include "includes.h"

void Out_GPIO_Init(void)
{
	INT8U 	err;
	GPIO_InitTypeDef Out_Init;

	RCC_APB2PeriphClockCmd(OUT_RCC, ENABLE);			//ʹ��GPIOB������ʱ��

	Out_Init.GPIO_Pin = GPIO_Pin_All ;
	Out_Init.GPIO_Speed = GPIO_Speed_50MHz;
	Out_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OUT_PORT,&Out_Init);
	OUT_PIN_OFF(GPIO_Pin_All );
	OUT_PIN_ON(RASTER_OUT); //�򿪹�դ
	OSFlagPost(Sys_Flags,RASTER_STATUS_FLAG,OS_FLAG_CLR,&err); //��λ��դʧЧ��־
}

void In_GPIO_Init(void)
{
	GPIO_InitTypeDef In_Init;

	RCC_APB2PeriphClockCmd(IN_RCC, ENABLE);			//ʹ��GPIOB������ʱ��

	In_Init.GPIO_Pin = GPIO_Pin_All ;
	In_Init.GPIO_Speed = GPIO_Speed_50MHz;
	In_Init.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(IN_PORT,&In_Init);
}

