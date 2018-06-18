//唉。。。
//复位：
//	cmd[0] = 0x00, cmd[1] = 0x00 , cmd[2] = 0x00 , cmd[3] = 0x00 , cmd[4] = 0x00;
//设置：
//	cmd[0] ,使能无刷闭环或者码盘反馈，第一位为无刷，第二位码盘反馈
//		例如，cmd[0] |= (1<<1) ,此为使能了码盘反馈
//	cmd[1] ,PWM输出通道，可为6 ，7 ，8 ，9，对应板子的C6，C7，A8,A9。当没有使能无刷闭环时，
//		也可以使用PWM输出功能,使用方法是，不选用无0刷闭环功能，但设置PWM输出频率，并设置输出通道
//	cmd[2] ,设置PWM的频率，存放高位，
//	cmd[3] ,设置PWM的频率，存放低位，所以最大支持到65535HZ的P波输出
//	cmd[4] ,设置使用哪个定时器进行无刷的反馈，可用2 3 4，对应TIM2,TIM3,TIM4
//	cmd[5] ,设置反馈定时器，暂时不能使用的功能
//	cmd[6] ,设置反馈周期，暂时不能使用
//控制：
//	单独继电器模式
//		cmd[0] , 第一位--B12 , 第二位--B13 ， 第三位--B14
//		例如，cmd[0] |=1<<2 ,将B14置高
//	速度或P波频率设置
//		两种模式
//		一、单独一路设置，由3个成员组成一个信息块，第一个成员为复选，后两个为参数值
//		cmd[0] ,低4位认为本块为速度设置，高四位认为本块为P波频率设置
//			例如，cmd[0] |= 7<<4,则本次为设置7通道的p波频率
//		cmd[1] ,参数值的高8位
//		cmd[2] ,参数值得低八位
//		二、两路同时设置，区别就是一次指令里有两个信息块，其他一样，其中cmd[0]和cmd[3]位复选

#include "stm32f10x.h"


#define SPEED_6 	0x06
#define SPEED_7	 	0x07
#define SPEED_8 	0x08
#define SPEED_9 	0x09
#define PWM_6		0x60
#define PWM_7 		0x70
#define PWM_8 		0x80
#define PWM_9 		0x90


typedef struct PWMInit_Struct{
	u8 Enable_speed_flag;
	u8 channel;
	u32 pwm_freq;
	TIM_TypeDef* timx;
	TIM_TypeDef* PWM_TIM;
}PWMInit_Struct;

typedef struct FeedBack_Struct{
	u8 EnablefeedBack_flag;
	int backSpeed;
	u32 backPosition;
	u8 backTIM;
}FeedBack_Struct;

extern int speedParam[4];
extern PWMInit_Struct pwminit_Struct[4];
extern FeedBack_Struct *feedback_Struct;


void communicate(u8 ID);
void canToSet(u8 NumOfcmd, u8 cmd , CanRxMsg *can_rx_msg);
void communicate_CallBack(CanRxMsg *can_rx_msg);

