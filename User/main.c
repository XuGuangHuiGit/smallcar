/**
  ******************************************************************************
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "USART.h"
#include "rcc.h"
#include "TIM.h"
#include "main.h"
#include "stdlib.h"
#include "brush.h"
#include "cmd.h"
#include "lcd.h"
#include "gpio.h"
#include "math.h"
#include "outputdata.h"
#include <time.h>

//xgh
#define MAXOUT 		10000
struct PID_Param{
	float P;
	float I;
	float D;
	int WantSpeed;
	//以上需要人工初始化参数
	
	//不许人工初始化的
	int LastE;
	int AllE;
	int MAX_AllE;	
};
//xgh end;

//xgh
struct PID_Param PID_TIM4 = {0.1, 0, 0, 0, 0,0,0};//P I D Wantspeed ** ** **
struct PID_Param PID_TIM3 = {0.1, 0, 0, 0, 0,0,0};
int Lspeed = 0;
int Rspeed = 0;
void pidParamInit(struct PID_Param *pidGroup);
//#define PIDCTRL   ///有这个宏定义的时候，有闭环控制

//xgh end;

int main()
{
	
	rcc_init();

    motorInit();
	
	//Usart_init(USART1 , 115200);
	//Usart_init(USART2 , 115200);
	//cmd_init(115200);
    setSpeed(Motor_1 , 1000);
    setSpeed(Motor_2 , 1000);
    setSpeed(Motor_3 , -1000);
//xgh
#ifdef PIDCTRL
	TIM2_init(10);//一个10毫秒的定时中断
#endif
//xgh end		

	while(1)
	{	
	}
}

//xgh
void pidParamInit(struct PID_Param *pidGroup){
	pidGroup->AllE = 0;
	pidGroup->LastE = 0;
	pidGroup->MAX_AllE = MAXOUT/(pidGroup->I);

}

int PID(struct PID_Param *pid, int Speed){
	int error = Speed - pid->WantSpeed;
	int output;
	pid->AllE += error;
	if(pid->AllE > pid->MAX_AllE){//上限
		pid->AllE = pid->MAX_AllE;
	}
	if(pid->AllE < (-1*pid->MAX_AllE)){//下限
		pid->AllE = pid->MAX_AllE;
	}
	
	output = pid->P * error + pid->D * (pid->LastE - error) + pid->I * pid->AllE;
	
	pid->LastE = error;
	return output;
}
//xgh end

//xgh改
void TIM2_IRQHandler()
{
	int speed_TIM3;
	int speed_TIM4;
	int LS;
	int RS;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
		
		//xgh
		speed_TIM4 = TIM4->CNT - 5000;
		TIM4->CNT = 5000;
		
		speed_TIM3 = TIM3->CNT - 5000;
		TIM3->CNT = 5000;
		
		PID_TIM4.WantSpeed = Lspeed;
		PID_TIM3.WantSpeed = Rspeed;
		
		LS = PID(&PID_TIM4, speed_TIM4);
		RS = PID(&PID_TIM3, speed_TIM3);
		
#ifdef	PIDCTRL
		PWM_set(TIM8 , Left_Motor, LS , 20000);
		PWM_set(TIM8 , Right_Motor , RS	, 20000);
#endif		
	}
}
//xgh end


void rebootSYS()
{
	__disable_fault_irq();   
    NVIC_SystemReset();
}