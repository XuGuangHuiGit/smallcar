#include "relay.h"

void Relay_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_Relay, ENABLE);
	GPIO_InitStructure.GPIO_Pin = Pin_Relay;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_Relay, &GPIO_InitStructure);

	GPIO_ResetBits(GPIO_Relay,Pin_Relay);			  //初始化为低电平		

}

void Relay_Turn(Relay r){  //继电器状态翻转
	u32 temp;
	temp=GPIO_Relay->ODR;
	temp^=r;
	
	GPIO_Write(GPIO_Relay, temp);
}



u16 Set_CH_GPIO(TIM tim,TIM_CH ch){
	u16 temp;
	u16 pin_ch1,pin_ch2,pin_ch3,pin_ch4;
	GPIO_TypeDef * port;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(tim==T5)
		tim=T2;
	
	
	if(tim!=T3){   //不是TIM3的情况
		switch(tim){
			case T1:
				port=TIM1_PORT;
				pin_ch1=TIM1_CH1;
				pin_ch2=TIM1_CH2;
				pin_ch3=TIM1_CH3;
				pin_ch4=TIM1_CH4;
				break;
			case T2:
				port=TIM2_PORT;
				pin_ch1=TIM2_CH1;
				pin_ch2=TIM2_CH2;
				pin_ch3=TIM2_CH3;
				pin_ch4=TIM2_CH4;
				break;
			case T4:
				port=TIM4_PORT;
				pin_ch1=TIM4_CH1;
				pin_ch2=TIM4_CH2;
				pin_ch3=TIM4_CH3;
				pin_ch4=TIM4_CH4;
				break;
			case T8:
				port=TIM8_PORT;
				pin_ch1=TIM8_CH1;
				pin_ch2=TIM8_CH2;
				pin_ch3=TIM8_CH3;
				pin_ch4=TIM8_CH4;
				break;
		}		
		if(ch&0x01)
			temp|=pin_ch1;
		if(ch&=0x02)
			temp|=pin_ch2;
		if(ch&=0x04)
			temp|=pin_ch3;
		if(ch&=0x08)
			temp|=pin_ch4;
		
		GPIO_InitStructure.GPIO_Pin = temp;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(port, &GPIO_InitStructure);
	}else{
		if(ch&0x01)
			temp|=TIM3_CH1;
		if(ch&=0x02)
			temp|=TIM3_CH1;		
		GPIO_InitStructure.GPIO_Pin = temp;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		temp=0;
		if(ch&=0x04)
			temp|=pin_ch3;
		if(ch&=0x08)
			temp|=pin_ch4;
		GPIO_InitStructure.GPIO_Pin = temp;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	
		/*
	GPIO_InitStructure.GPIO_Pin = brush_A|brush_B ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_brush, &GPIO_InitStructure);
	*/
}
/*
这个函数用来将自己定义的枚举和STM32定义的TIM1-8关联起来
我想不出更好的办法
stm32定义的TIM1-8似乎不能用于switch  case中

*/
TIM_TypeDef*  Get_Tim(TIM tim){
	TIM_TypeDef* temp;
	switch(tim){
		case T1:
			temp=TIM1;
			break;
		case T2:
			temp=TIM2;
			break;
		case T3:
			temp=TIM3;
			break;
		case T4:
			temp=TIM4;
			break;
		case T5:
			temp=TIM5;
			break;
		case T8:
			temp=TIM8;
			break;
	}
	return temp;
}

//freq 10
//duty 精度10000，如7000为70%
void PWM_Init(TIM tim,int freq,int duty,u8 ch){
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	u32 RCC_TIM;
	u32 RCC_TIM_GPIO;
	u32 TIM_GPIO_pin;
	u16	period;
	u32 pulse;
	
	if(ch>0x0F)
		return ;
	
	
	switch(tim){
		case T1:
			RCC_TIM=RCC_APB2Periph_TIM1;
			RCC_TIM_GPIO=RCC_APB2Periph_GPIOA;
			break;
		case T2:
			RCC_TIM=RCC_APB1Periph_TIM2;
			RCC_TIM_GPIO=RCC_APB2Periph_GPIOA;
			break;
		case T3:
			RCC_TIM=RCC_APB1Periph_TIM3;
			RCC_TIM_GPIO=RCC_APB2Periph_GPIOA;
			break;			
		case T4:
			RCC_TIM=RCC_APB1Periph_TIM4;
			RCC_TIM_GPIO=RCC_APB2Periph_GPIOB;
			break;
		case T5:
			RCC_TIM=RCC_APB1Periph_TIM5;
			RCC_TIM_GPIO=RCC_APB2Periph_GPIOA;
			break;
		case T8:
			RCC_TIM=RCC_APB2Periph_TIM8;
			RCC_TIM_GPIO=RCC_APB2Periph_GPIOC;
			break;			
		default:
			return ;
	}

	if(tim==T1||tim==T8)
		RCC_APB2PeriphClockCmd(RCC_TIM,ENABLE);
	else
		RCC_APB1PeriphClockCmd(RCC_TIM,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_TIM_GPIO, ENABLE);
	

	
	if(freq<=1000){
		TIM_TimeBaseStructure.TIM_Prescaler = 99; 	//一百分频 
		period=720000/freq ;

	}else{
		TIM_TimeBaseStructure.TIM_Prescaler = 0; 	
		period=72000000/freq;
	}
	TIM_TimeBaseStructure.TIM_Period=period - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(Get_Tim(tim), &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	
	pulse=period*duty/10000;
	TIM_OCInitStructure.TIM_Pulse = pulse;								
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	Set_CH_GPIO(tim,ch);
	
	if(ch&0x01){
		TIM_OC1Init(Get_Tim(tim), &TIM_OCInitStructure);
	}
	if(ch&0x02)
		TIM_OC2Init(Get_Tim(tim), &TIM_OCInitStructure);
	if(ch&0x04)
		TIM_OC3Init(Get_Tim(tim), &TIM_OCInitStructure);
	if(ch&0x08)
		TIM_OC4Init(Get_Tim(tim), &TIM_OCInitStructure);
	
	TIM_Cmd(Get_Tim(tim), ENABLE);

	if(tim==T1||tim==T8){
		TIM_CtrlPWMOutputs(Get_Tim(tim), ENABLE);
	}		
}


void PWM_Set(TIM tim,TIM_CH ch,int duty){
	u16 period;
	u32 temp;
	TIM_TypeDef* TIMx=Get_Tim(tim);
	period=TIMx->ARR;
	temp=duty*period/10000;
	if(ch&0x01)
		TIM_SetCompare1(TIMx,temp);
	if(ch&0x02)
		TIM_SetCompare2(TIMx,temp);
	if(ch&0x04)
		TIM_SetCompare3(TIMx,temp);
	if(ch&0x08)
		TIM_SetCompare4(TIMx,temp);	
}