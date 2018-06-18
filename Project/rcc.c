#include "rcc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "TIM.h"
/******
*AHB    1��Ƶ   72M    ���72M
*APB1   2��Ƶ   36M    ���36M
*APB2   1��Ƶ   72M    ���72M
*
*/

void system_clk_set(void){ 
    ErrorStatus HSEStartUpStatus;
    RCC_DeInit();
 
    RCC_HSEConfig(RCC_HSE_ON );   //���ⲿʱ��
 
    HSEStartUpStatus = RCC_WaitForHSEStartUp();  //�ȴ��ⲿʱ�Ӵ����ȶ�
 
   if(HSEStartUpStatus == SUCCESS)     
   {
		FLASH_SetLatency(FLASH_Latency_2);   
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);       //flashʱ�ӵ��������  
		RCC_PLLCmd(DISABLE);  //����PLL֮ǰ��Ҫ�ر�PLL
		RCC_HCLKConfig(RCC_SYSCLK_Div1);   //HCLK��Ƶ
		RCC_PCLK2Config(RCC_HCLK_Div1);   //PCLK2��Ƶ
		RCC_PCLK1Config(RCC_HCLK_Div2);    //PCLK1��Ƶ
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE); //ʹ��PLL
	 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){      //�ȴ�����ֱ��PLLʹ�����
    }
 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //ѡ��PLLʱ��Ϊϵͳʱ��
 
    while(RCC_GetSYSCLKSource() != 0x08)       //�ȴ�����PLLʱ�����ó�ϵͳʱ�����
       { 
       }
     }
//    #if(__FPU_PRESENT == 1)&&(__FPU_USED == 1)
//			SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  //����FPU
//		#endif
}


void rcc_init(void){
	RCC_DeInit();
	system_clk_set();
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
}