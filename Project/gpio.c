#include "gpio.h"

GPIO_InitTypeDef GPIO_initstruct;

void gpio_init(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin , GPIOSpeed_TypeDef GPIO_Speed , GPIOMode_TypeDef GPIO_Mode){
	GPIO_initstruct.GPIO_Pin = GPIO_Pin;
	GPIO_initstruct.GPIO_Speed = GPIO_Speed;
	GPIO_initstruct.GPIO_Mode = GPIO_Mode;
	
	GPIO_Init(GPIOx , &GPIO_initstruct);
}