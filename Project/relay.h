#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"


#define RCC_Relay			RCC_APB2Periph_GPIOB
#define Pin_Relay			GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3
#define GPIO_Relay		GPIOB

typedef enum Relay{
	Relay1=GPIO_Pin_3,
	Relay2=GPIO_Pin_4,
	Relay3=GPIO_Pin_5,
	Relay4=GPIO_Pin_6
}Relay;



typedef enum TIM{
	T1=1,
	T2=2,
	T3=3,
	T4=4,
	T5=5,
	T8=8
}TIM;

typedef enum{
	ch1=0x01,
	ch2=0x02,
	ch3=0x04,
	ch4=0x08
}TIM_CH;
#define TIM1_CH1		GPIO_Pin_8
#define TIM1_CH2		GPIO_Pin_9
#define TIM1_CH3		GPIO_Pin_10
#define TIM1_CH4		GPIO_Pin_11

#define TIM1_PORT		GPIOA

#define TIM2_CH1		GPIO_Pin_0
#define TIM2_CH2		GPIO_Pin_1
#define TIM2_CH3		GPIO_Pin_2
#define TIM2_CH4		GPIO_Pin_3

#define TIM2_PORT		GPIOA

#define TIM3_CH1		GPIO_Pin_6   //A6
#define TIM3_CH2		GPIO_Pin_7
#define TIM3_CH3		GPIO_Pin_0	//B0
#define TIM3_CH4		GPIO_Pin_1

#define TIM1_PORT		GPIOA

#define TIM4_CH1		GPIO_Pin_6
#define TIM4_CH2		GPIO_Pin_7
#define TIM4_CH3		GPIO_Pin_8
#define TIM4_CH4		GPIO_Pin_9
#define TIM4_PORT		GPIOB

#define TIM8_CH1		GPIO_Pin_6
#define TIM8_CH2		GPIO_Pin_7
#define TIM8_CH3		GPIO_Pin_8
#define TIM8_CH4		GPIO_Pin_9
#define TIM8_PORT		GPIOC


#define Relay_Set(r)				GPIO_SetBits(GPIO_Relay,r)
#define Relay_Reset(r)			GPIO_ResetBits(GPIO_Relay,r)

void Relay_Turn(Relay r);
void Relay_Init();
u16 Set_CH_GPIO(TIM tim,TIM_CH ch);
TIM_TypeDef*  Get_Tim(TIM tim);
void PWM_Init(TIM tim,int freq,int duty,u8 ch);
void PWM_Set(TIM tim,TIM_CH ch,int duty);

#endif