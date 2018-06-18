//��������
//��λ��
//	cmd[0] = 0x00, cmd[1] = 0x00 , cmd[2] = 0x00 , cmd[3] = 0x00 , cmd[4] = 0x00;
//���ã�
//	cmd[0] ,ʹ����ˢ�ջ��������̷�������һλΪ��ˢ���ڶ�λ���̷���
//		���磬cmd[0] |= (1<<1) ,��Ϊʹ�������̷���
//	cmd[1] ,PWM���ͨ������Ϊ6 ��7 ��8 ��9����Ӧ���ӵ�C6��C7��A8,A9����û��ʹ����ˢ�ջ�ʱ��
//		Ҳ����ʹ��PWM�������,ʹ�÷����ǣ���ѡ����0ˢ�ջ����ܣ�������PWM���Ƶ�ʣ����������ͨ��
//	cmd[2] ,����PWM��Ƶ�ʣ���Ÿ�λ��
//	cmd[3] ,����PWM��Ƶ�ʣ���ŵ�λ���������֧�ֵ�65535HZ��P�����
//	cmd[4] ,����ʹ���ĸ���ʱ��������ˢ�ķ���������2 3 4����ӦTIM2,TIM3,TIM4
//	cmd[5] ,���÷�����ʱ������ʱ����ʹ�õĹ���
//	cmd[6] ,���÷������ڣ���ʱ����ʹ��
//���ƣ�
//	�����̵���ģʽ
//		cmd[0] , ��һλ--B12 , �ڶ�λ--B13 �� ����λ--B14
//		���磬cmd[0] |=1<<2 ,��B14�ø�
//	�ٶȻ�P��Ƶ������
//		����ģʽ
//		һ������һ·���ã���3����Ա���һ����Ϣ�飬��һ����ԱΪ��ѡ��������Ϊ����ֵ
//		cmd[0] ,��4λ��Ϊ����Ϊ�ٶ����ã�����λ��Ϊ����ΪP��Ƶ������
//			���磬cmd[0] |= 7<<4,�򱾴�Ϊ����7ͨ����p��Ƶ��
//		cmd[1] ,����ֵ�ĸ�8λ
//		cmd[2] ,����ֵ�õͰ�λ
//		������·ͬʱ���ã��������һ��ָ������������Ϣ�飬����һ��������cmd[0]��cmd[3]λ��ѡ

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

