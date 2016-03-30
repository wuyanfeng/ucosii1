#include "includes.h"

void LED_Config(void)
{
	GPIO_InitTypeDef led_init;

	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);			//使能GPIOB的外设时钟

	led_init.GPIO_Pin = LED_PIN;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_GPIO,&led_init);
	LED_OFF();
}

void LED_OFF(void)
{
	GPIO_SetBits(LED_GPIO,LED_PIN);
}

void LED_ON(void)
{
	GPIO_ResetBits(LED_GPIO,LED_PIN);
}

