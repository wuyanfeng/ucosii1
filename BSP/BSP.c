#include "includes.h"
void SYSCLK_Init(void)
{
	RCC_HSEConfig(RCC_HSE_ON);	 //ʹ���ⲿʱ��
 	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);			//PLL����Ϊ72Mhz
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_HCLKConfig(RCC_HCLK_Div1);						 //AHB����Ƶ��72Mhz
	RCC_PCLK1Config(RCC_PCLK2_Div2);   //APB1����Ƶ��36Mhz
	RCC_PCLK2Config(RCC_PCLK2_Div2);	//APB2����Ƶ��72Mhz

}

void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
		// �رյδ�ʱ��  
//	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	// ʹ�ܵδ�ʱ��  
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
