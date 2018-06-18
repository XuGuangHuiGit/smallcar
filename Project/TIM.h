#include "stm32f10x_tim.h"
#define TIM_Status_unuse 	0x01
#define TIM_Status_encode 	0x02
#define TIM_Status_PWM		0x04
#define TIM_Status_PIT  	0x08

#define Motor_1 0
#define Motor_2 1
#define Motor_3 2


void TIM2_init(int Time_ms);
void PWM_init(TIM_TypeDef* TIMx , u8 channel , u32 freq);
void encode_init(TIM_TypeDef* TIMx);
void PWM_set(TIM_TypeDef* TIMx , u8 channel , int duty , u32 freq);
void delay_ms(u16 nms);

static void TIM4_Mode_Config(void);
void TIM4_Init(void);

static void TIM5_Mode_Config(void);
void TIM5_Init(void);

void motorInit();
void setSpeed(int which , int speed);
