#include "bupt.h"
#include "can.h"
#include "TIM.h"
#include "stdlib.h"
#include "gpio.h"

int speedParam[4] = {0};
PWMInit_Struct pwminit_Struct[4];
FeedBack_Struct *feedback_Struct;
int PWMbuf;
extern u8 warning_flag;

void communicate(u8 ID){
	int i = 0;
	can_init(ID);
	feedback_Struct = (FeedBack_Struct*)malloc(sizeof(FeedBack_Struct));
	for(i=0 ; i<4 ;i++){
		pwminit_Struct[i].channel = 0;
		pwminit_Struct[i].Enable_speed_flag = 0;
		pwminit_Struct[i].pwm_freq = 0;
	}
	can_add_callback(ID ,  communicate_CallBack);
}

void communicate_CallBack(CanRxMsg *can_rx_msg){
	int i;
	u8 Length = can_rx_msg ->DLC;
	GPIO_ResetBits(SEX_LED , LED_gre);
	if (Length == 7){//设置或者复位
		for(i = 0 ; i < 7 ; i++ ){
			if(can_rx_msg->Data[i] != 0) break;
			if(i == 6) {
				__disable_fault_irq();   
				NVIC_SystemReset();//全0消息，重启
			}
		}
		if(can_rx_msg->Data[1] == 6||can_rx_msg->Data[1]  == 7||can_rx_msg->Data[1]  == 8||can_rx_msg->Data[1]  ==9){//需开启pwm功能
			pwminit_Struct[can_rx_msg->Data[1] - 6].Enable_speed_flag = can_rx_msg->Data[0]&0x01;//闭环使能
			pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq = can_rx_msg->Data[2];//获得ＰＷＭ频率
			pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq = (pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq<<8)|can_rx_msg->Data[3];
			
			
			
			/***以下记录ＰＷＭ定时器及通道***/
			if(can_rx_msg->Data[1] == 6){
				pwminit_Struct[can_rx_msg->Data[1] - 6].PWM_TIM = TIM8;
				pwminit_Struct[can_rx_msg->Data[1] - 6].channel = 1;
				if(pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq == 0||pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq>=65535){
					warning_flag = 1;
					return;
				}
				PWM_init(TIM8 , 1 , pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq);//初始化定时器
			}else if(can_rx_msg->Data[1] == 7){
				pwminit_Struct[can_rx_msg->Data[1] - 6].PWM_TIM = TIM8;
				pwminit_Struct[can_rx_msg->Data[1] - 6].channel = 2;
				if(pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq == 0||pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq>=65535){
					warning_flag = 1;
					return;
				}
				PWM_init(TIM8 , 2 , pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq);//初始化定时器
			}else if(can_rx_msg->Data[1] == 8){
				pwminit_Struct[can_rx_msg->Data[1] - 6].PWM_TIM = TIM1;
				pwminit_Struct[can_rx_msg->Data[1] - 6].channel = 1;
				if(pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq == 0||pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq>=65535){
					warning_flag = 1;
					return;
				}
				PWM_init(TIM1 , 1 , pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq);//初始化定时器
			}else if(can_rx_msg->Data[1] == 9){
				pwminit_Struct[can_rx_msg->Data[1] - 6].PWM_TIM = TIM1;
				pwminit_Struct[can_rx_msg->Data[1] - 6].channel = 2;
				if(pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq == 0||pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq>=65535){
					warning_flag = 1;
					return;
				}
				PWM_init(TIM1 , 2 , pwminit_Struct[can_rx_msg->Data[1] - 6].pwm_freq);//初始化定时器
			}else {
				warning_flag = 1;
				return;
			}
			/***以上为记录ＰＷＭ定时器**/
			/*****一下记录码盘定时器，并初始化***/
			if(can_rx_msg->Data[4] == 2) {
				pwminit_Struct[can_rx_msg->Data[1] - 6].timx = TIM5;
				encode_init(TIM5);//TIM 2和5共用引脚
			}else if(can_rx_msg->Data[4] == 3) {
				pwminit_Struct[can_rx_msg->Data[1] - 6].timx = TIM3;
				encode_init(TIM3);//初始化定时器
			}else if(can_rx_msg->Data[4] == 4) {
				pwminit_Struct[can_rx_msg->Data[1] - 6].timx = TIM4;
				encode_init(TIM4);//
			}else {
			}
			/**以上为记录码盘定时器***/
			
		}else{//单独开启码盘反馈
			if(can_rx_msg->Data[5] == 1){//使能了反馈
				feedback_Struct->EnablefeedBack_flag = 1;
				feedback_Struct->backTIM = can_rx_msg->Data[6];
				feedback_Struct->backSpeed = 0;
				feedback_Struct->backPosition = 0;
				
				encode_init(TIM5);
				encode_init(TIM3);
				encode_init(TIM4);
				TIMStatus.feedBackTIM2_flag = 1;
				TIMStatus.feedBackTIM3_flag = 1;
				TIMStatus.feedBackTIM4_flag = 1;
			}
		}
		
		
		

		//以上把相应功能单元初始化
		
	}else if(Length == 1){//单独继电器模式

		if((can_rx_msg->Data[0]&(1<<0)) !=0){
			GPIO_SetBits(GPIOB , GPIO_Pin_12);
		}else{
			GPIO_ResetBits(GPIOB , GPIO_Pin_12);
		}
		
		if((can_rx_msg->Data[0]&(1<<1)) !=0){
			GPIO_SetBits(GPIOB , GPIO_Pin_13);
		}else{
			GPIO_ResetBits(GPIOB , GPIO_Pin_13);
		}
		
		if((can_rx_msg->Data[0]&(1<<2)) !=0){
			GPIO_SetBits(GPIOB , GPIO_Pin_14);
		}else{
			GPIO_ResetBits(GPIOB , GPIO_Pin_14);
		}
	}else if (Length == 6){//速度设置或者P波设置
		canToSet(0 , can_rx_msg->Data[0] , can_rx_msg);
		canToSet(3 , can_rx_msg->Data[3] , can_rx_msg);
	}else if (Length == 3){//继电器&速度设置或者P波设置 2 + 2
		canToSet(0 , can_rx_msg->Data[0] , can_rx_msg);
	}
	GPIO_SetBits(SEX_LED , LED_gre);
}

void canToSet(u8 NumOfcmd, u8 cmd , CanRxMsg *can_rx_msg){
	switch(cmd){
		case 0x06:
			if(pwminit_Struct[0].Enable_speed_flag == 0){
				return ;//未使能速度闭环，不能设置速度
			}
			speedParam[0] = can_rx_msg->Data[NumOfcmd + 1];
			speedParam[0] = (speedParam[0] <<8)|can_rx_msg->Data[NumOfcmd + 2];
			break;
		case 0x07:
			if(pwminit_Struct[1].Enable_speed_flag == 0){
				return ;//未使能速度闭环，不能设置速度
			}
			speedParam[1] = can_rx_msg->Data[NumOfcmd + 1];
			speedParam[1] = (speedParam[1] <<8)|can_rx_msg->Data[NumOfcmd + 2];
			break;
		case 0x08:
			if(pwminit_Struct[2].Enable_speed_flag == 0){
				return ;//未使能速度闭环，不能设置速度
			}
			speedParam[2] = can_rx_msg->Data[NumOfcmd + 1];
			speedParam[2] = (speedParam[2] <<8)|can_rx_msg->Data[NumOfcmd + 2];
			break;
		case 0x09:
			if(pwminit_Struct[3].Enable_speed_flag == 0){
				return ;//未使能速度闭环，不能设置速度
			}
			speedParam[3] = can_rx_msg->Data[NumOfcmd + 1];
			speedParam[3] = (speedParam[3] <<8)|can_rx_msg->Data[NumOfcmd + 2];
			break;
		case 0x60:
			PWMbuf = can_rx_msg->Data[NumOfcmd + 1];
			PWMbuf = (PWMbuf <<8)|can_rx_msg->Data[NumOfcmd + 2];
			if(pwminit_Struct[0].Enable_speed_flag == 1){//使能了速度闭环，安全起见，不允许开环控制
				return;
			}
			PWM_set(TIM8 , 1 , PWMbuf  , pwminit_Struct[0].pwm_freq);//定时器，通道，占空比，频率
			break;
		case 0x70:
			PWMbuf = can_rx_msg->Data[NumOfcmd + 1];
			PWMbuf = (PWMbuf  <<8)|can_rx_msg->Data[NumOfcmd + 2];
			if(pwminit_Struct[1].Enable_speed_flag == 1){//使能了速度闭环，安全起见，不允许开环控制
				return;
			}
			PWM_set(TIM8 , 2 , PWMbuf , pwminit_Struct[1].pwm_freq);//定时器，通道，占空比，频率
			break;
		case 0x80:
			PWMbuf = can_rx_msg->Data[NumOfcmd + 1];
			PWMbuf = (PWMbuf <<8)|can_rx_msg->Data[NumOfcmd + 2];
			if(pwminit_Struct[1].Enable_speed_flag == 1){//使能了速度闭环，安全起见，不允许开环控制
				return;
			}
			PWM_set(TIM1 , 1 , PWMbuf , pwminit_Struct[2].pwm_freq);//定时器，通道，占空比，频率
			break;
		case 0x90:
			PWMbuf = can_rx_msg->Data[NumOfcmd + 1];
			PWMbuf = (PWMbuf <<8)|can_rx_msg->Data[NumOfcmd + 2];
			if(pwminit_Struct[1].Enable_speed_flag == 1){//使能了速度闭环，安全起见，不允许开环控制
				return;
			}
			PWM_set(TIM1 , 2 , PWMbuf , pwminit_Struct[3].pwm_freq);//定时器，通道，占空比，频率
			break;
		default:break;
	}
}
