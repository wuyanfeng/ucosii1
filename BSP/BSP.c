#include "includes.h"
void SYSCLK_Init(void)
{
	RCC_HSEConfig(RCC_HSE_ON);	 //使能外部时钟
 	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);			//PLL设置为72Mhz
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_HCLKConfig(RCC_HCLK_Div1);						 //AHB总线频率72Mhz
	RCC_PCLK1Config(RCC_PCLK2_Div2);   //APB1总线频率36Mhz
	RCC_PCLK2Config(RCC_PCLK2_Div2);	//APB2总线频率72Mhz

}

void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
//	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

void BSP_Init(void)
{
	SYSCLK_Init();
	SysTick_Init();	
	LED_Config();
	Out_GPIO_Init();
	In_GPIO_Init();
	UART1_Init(UART1_BAUD);
	UART2_Init(UART2_BAUD);
}
