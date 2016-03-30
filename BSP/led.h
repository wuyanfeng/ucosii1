#ifndef _LED_H_
#define _LED_H_

#define LED_GPIO GPIOA
#define LED_PIN	GPIO_Pin_8
#define LED_RCC     RCC_APB2Periph_GPIOA

void LED_Config(void);
void LED_OFF(void);
void LED_ON(void);

#endif
