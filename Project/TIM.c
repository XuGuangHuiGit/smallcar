#include "TIM.h"
#include "bupt.h"

u16	Parameter = 5000; //xçº¿éœå°”ç¼–ç å™¨

/*
*TIM2£º ¶¨Ê±ÖÐ¶Ï
*²ÎÊý£º¶¨Ê±Ê±³¤
*
*/

static void TIM5_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*- ç¼–ç å™¨è¾“å…¥ PA->0   PA->1 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           

	/*- TIM3ç¼–ç å™¨æ¨¡å¼è®¾ç½® -*/
	TIM_DeInit(TIM5);
	TIM_TimeBaseStructure.TIM_Period = Parameter * 2;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              
	
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); //é…ç½®ç¼–ç å™¨æ¨¡å¼è§¦å‘æºå’Œæžæ€§
	
	TIM_ICStructInit(&TIM_ICInitStructure); //é…ç½®æ»¤æ³¢å™¨
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
		
	TIM3->CNT = Parameter;

	TIM_Cmd(TIM5, ENABLE);   //å¯åŠ¨TIM4å®šæ—¶å™¨
}

void TIM5_Init(void)
{
  TIM5_Mode_Config();
}

static void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*- ç¼–ç å™¨è¾“å…¥ PB->6   PB->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	/*- TIM4ç¼–ç å™¨æ¨¡å¼è®¾ç½® -*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = Parameter * 2;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); //é…ç½®ç¼–ç å™¨æ¨¡å¼è§¦å‘æºå’Œæžæ€§
	
	TIM_ICStructInit(&TIM_ICInitStructure); //é…ç½®æ»¤æ³¢å™¨
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
		
	TIM4->CNT = Parameter;//xgh æ”¹5000ï¼Œå³å®šæ—¶å™¨ä»Ž5000å¼€å§‹è®¡æ•°

	TIM_Cmd(TIM4, ENABLE);   //å¯åŠ¨TIM4å®šæ—¶å™¨
}

void TIM4_Init(void)
{
  TIM4_Mode_Config();
}





void TIM2_init(int Time_ms){
	int Period = 2*Time_ms;
	NVIC_InitTypeDef nvic_InitStructure;
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&tim_TimeBaseInitTypeDef);

	tim_TimeBaseInitTypeDef.TIM_Prescaler = 36000-1;  // 1M*2 
	tim_TimeBaseInitTypeDef.TIM_Period =Period; 
	tim_TimeBaseInitTypeDef.TIM_ClockDivision =TIM_CKD_DIV1;  //²»·ÖÆµ 
	tim_TimeBaseInitTypeDef.TIM_CounterMode =TIM_CounterMode_Up;   //ÉÏÊý
	TIM_TimeBaseInit(TIM2,&tim_TimeBaseInitTypeDef);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);

	nvic_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   
	nvic_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	nvic_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	nvic_InitStructure.NVIC_IRQChannelSubPriority = 0;   
	NVIC_Init(&nvic_InitStructure);  

	TIM_Cmd(TIM2, ENABLE); 
}
void encode_init(TIM_TypeDef* TIMx){
	
	
	GPIO_InitTypeDef gpio_InitStructure;
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	TIM_ICInitTypeDef tim_ICInitTypeDef;

	gpio_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(TIMx == TIM1){
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;
		GPIO_Init(GPIOA, &gpio_InitStructure);
	}else if(TIMx == TIM3){
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_Init(GPIOA, &gpio_InitStructure);
	}else if(TIMx == TIM4){
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_Init(GPIOB, &gpio_InitStructure);
	}else if(TIMx == TIM5||TIMx == TIM2){
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
		GPIO_Init(GPIOA, &gpio_InitStructure);
	}else if(TIMx == TIM8){
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_Init(GPIOC, &gpio_InitStructure);
	}else {
		return ;
	}

	TIM_DeInit(TIMx);
	TIM_TimeBaseStructInit(&tim_TimeBaseInitTypeDef);

	tim_TimeBaseInitTypeDef.TIM_Prescaler = 0x0;  // Noprescaling
	tim_TimeBaseInitTypeDef.TIM_Period =49999; 
	tim_TimeBaseInitTypeDef.TIM_ClockDivision =TIM_CKD_DIV1;  //²»·ÖÆµ 
	tim_TimeBaseInitTypeDef.TIM_CounterMode =TIM_CounterMode_Up;   //ÉÏÊý
	TIM_TimeBaseInit(TIMx,&tim_TimeBaseInitTypeDef);

	TIM_EncoderInterfaceConfig(TIMx,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);   //IC1 IC2¶¼¼ÆÊý
	TIM_ICStructInit(&tim_ICInitTypeDef);
	tim_ICInitTypeDef.TIM_ICFilter = 0x6;
	TIM_ICInit(TIMx,&tim_ICInitTypeDef);

	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
	TIM_ITConfig(TIMx, TIM_IT_Update,ENABLE);

	TIMx->CNT =6000;

	TIM_Cmd(TIMx, ENABLE); 
}


/**
 *¹¦ÄÜ£ºPWM ³õÊ¼»¯
 *²ÎÊý£ºTIMx--¶¨Ê±Æ÷x
 *		channel--Í¨µÀ1234
 *		freq--ÆµÂÊ
 */
void PWM_init(TIM_TypeDef* TIMx , u8 channel , u32 freq){	
	int prescaler = 0;
	int period = 0;
    u8 flag_1 = 0;
    u8 flag_8 = 0;
	GPIO_InitTypeDef gpio_InitStructure;
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	gpio_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(TIMx == TIM8&&flag_8!=1){
        flag_8 = 1;
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_Init(GPIOC, &gpio_InitStructure);
	}else if(TIMx == TIM1&&flag_1 !=1){
        flag_1 =1;
		gpio_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
		GPIO_Init(GPIOA, &gpio_InitStructure);
	}else{
		return ;
	}
	
	if(freq <=1000){
		prescaler = 71;//µÍÆµÂÊÊ±72·ÖÆµ
		period = 1000000/freq - 1;
	}else {
		prescaler = 3;//¸ßÆµÊ±4·ÖÆµ
		period = 18000000/freq - 1;
	}
	
	tim_TimeBaseInitTypeDef.TIM_Prescaler = prescaler; //1M
	tim_TimeBaseInitTypeDef.TIM_CounterMode = TIM_CounterMode_Up;
	tim_TimeBaseInitTypeDef.TIM_Period =period;           
	tim_TimeBaseInitTypeDef.TIM_ClockDivision = 0;
	tim_TimeBaseInitTypeDef.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIMx, &tim_TimeBaseInitTypeDef);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse =1000;								//duty=50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    //µ±ÎªPWMÄ£Ê½2Ê±£¬¼«ÐÔÒª·´¹ýÀ´
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	
	if(channel == 1){
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	}else if(channel == 2){
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	}else if(channel == 3){
        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
    }else if(channel ==4){
        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
    }else{
		return ;
        
	}
	TIM_Cmd(TIMx, ENABLE);
	TIM_CtrlPWMOutputs(TIMx, ENABLE);

}



/**
 *¹¦ÄÜ£ºPWM ÉèÖÃ
 *²ÎÊý£ºTIMx--¶¨Ê±Æ÷x
 *		channel--Í¨µÀ1234
 *		duty--Õ¼¿Õ±È£¬1--10000
 *		freq--ÆµÂÊ
 */
void PWM_set(TIM_TypeDef* TIMx , u8 channel , int duty , u32 freq){
	
	
	u32 cmp;
	int prescaler ,period;
	
	
	if(freq <=1000){
		prescaler = 71;//µÍÆµÂÊÊ±72·ÖÆµ
		period = 1000000/freq - 1;
	}else {
		prescaler = 3;//¸ßÆµÊ±4·ÖÆµ
		period = 18000000/freq - 1;
	}
	
	cmp = period*duty/10000.0;
	
	if(channel == 1){
		TIM_SetCompare1(TIMx,cmp);
	}else if(channel == 2){
		TIM_SetCompare2(TIMx,cmp);
	}else if(channel == 3){
		TIM_SetCompare3(TIMx,cmp);
	}else if(channel == 4){
		TIM_SetCompare4(TIMx,cmp);
	}
}

void delay_ms(u16 nms)  
{  
	u32 temp;
	SysTick ->LOAD = 9000*nms;
	SysTick ->VAL = 0x00;
	SysTick ->CTRL = 0x01;
	do{
		temp = SysTick ->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick ->CTRL = 0x00;
	SysTick ->VAL = 0x00;
   
} 


void motorInit(){
    PWM_init(TIM1, 1, 15000);
    PWM_init(TIM1, 2, 15000);
    PWM_init(TIM1, 3, 15000);
    PWM_init(TIM1, 4, 15000);

    PWM_init(TIM8, 1, 15000);
    PWM_init(TIM8, 2, 15000);
    
    encode_init(TIM3);
    encode_init(TIM4);
    encode_init(TIM5);
}


void setSpeed(int which , int speed){
    if(which == Motor_1){
        if(speed >= 0){
            PWM_set(TIM1, 1, 0 ,15000);
            PWM_set(TIM1, 2, speed ,15000);
        }else{
            speed = -1*speed ;
            PWM_set(TIM1, 1, speed ,15000);
            PWM_set(TIM1, 2, 0 ,15000);
        }
    }else if(which == Motor_2){
        if(speed >= 0){
            PWM_set(TIM1, 3, 0 ,15000);
            PWM_set(TIM1, 4, speed ,15000);
        }else{
            speed = -1*speed ;
            PWM_set(TIM1, 3, speed ,15000);
            PWM_set(TIM1, 4, 0 ,15000);
        }
    }else if(which == Motor_3){
        if(speed >= 0){
            PWM_set(TIM8, 1, 0 ,15000);
            PWM_set(TIM8, 2, speed ,15000);
        }else{
            speed = -1*speed ;
            PWM_set(TIM8, 1, speed ,15000);
            PWM_set(TIM8, 2, 0 ,15000);
        }
    }
}




