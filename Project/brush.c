#include "brush.h"

#define brush_A 			GPIO_Pin_6
#define brush_B 			GPIO_Pin_7
#define dis					GPIO_Pin_9

#define RCC_brush_TIM   RCC_APB1Periph_TIM4



#define GPIO_brush						GPIOB

#define RCC_EN_GPIO_brush		RCC_APB2Periph_GPIOB
#define RCC_GPIO_brush			RCC_APB2Periph_GPIOB

#define TIM_brush						TIM4

void brush_init(){
	GPIO_InitTypeDef GPIO_InitStructure;

	//RCC_APB2PeriphClockCmd(RCC_EN_GPIO_brush, ENABLE);//使能引脚初始化
	GPIO_InitStructure.GPIO_Pin = dis;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_brush, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_brush,dis);					//初始化为高电平
	
	brush_pwm_init();														//初始化PWM波
	GPIO_ResetBits(GPIO_brush,dis);

}

void brush_pwm_init(){
	

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_brush_TIM,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_GPIO_brush, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = brush_A|brush_B ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ｐ波io初始化

	GPIO_Init(GPIO_brush, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period =3000;            //f=17khz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM_brush, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 10;								//duty=50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM_brush, &TIM_OCInitStructure);
	TIM_OC2Init(TIM_brush, &TIM_OCInitStructure);

	TIM_Cmd(TIM_brush, ENABLE);

	//TIM_CtrlPWMOutputs(TIM_brush, ENABLE);
}

void brush_low(uint16_t brush){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_GPIO_brush, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = brush;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_Init(GPIO_brush, &GPIO_InitStructure);
	
	//设为普通IO输出
	
	GPIO_ResetBits(GPIO_brush,brush);//设置为低电平
}

void brush_high(uint16_t brush){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_GPIO_brush, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = brush;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_Init(GPIO_brush, &GPIO_InitStructure);	   //设为PWM波输出
	
}

void set_A(int duty){
	u16 cmp;
	cmp = 3000*duty/5000;//这里的2048是17K P波计算的
	TIM_SetCompare1(TIM_brush,cmp);
}

void set_B(int duty){
	u16 cmp;
	cmp = 3000*duty/5000;//这里的2048是17K P波计算的
	TIM_SetCompare2(TIM_brush,cmp);
}


void set_speed(int speed_pwm){
	
	if (speed_pwm >4800){
		speed_pwm = 4800;
	}
	if (speed_pwm <-4800){
		speed_pwm = -4800;
	}
	
	if(speed_pwm >=0){//通道1产生P波，通道2 0，

		set_A(speed_pwm);
		set_B(0);
		 //brush_low(brush_B);
	}else{
		speed_pwm = -1*speed_pwm;
		//brush_low(brush_A);
		set_A(0);
		set_B(speed_pwm); 
	}
	
}







