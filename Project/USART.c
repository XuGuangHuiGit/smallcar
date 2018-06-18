#include "USART.h"

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStruct;


void Usart_init(USART_TypeDef* USARTx , u32 baud)
{
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if(USARTx == USART1)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 	//usart1--TX		引脚初始化
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//usart1--RX
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if (USARTx == USART2)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 	//usart2--TX		引脚初始化
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	//usart2--RX
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if (USARTx == USART3)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//usart3--TX		引脚初始化
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//usart3--RX
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else if (USARTx == UART4)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	//uart4--TX			引脚初始化
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//uart4--RX
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	else if (USARTx == UART5)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 	//uart5--TX			引脚初始化
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//uart5--RX
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	else 
	{
		return;
	}
	
	USART_InitStructure.USART_BaudRate = baud;                  //串口基本设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //提升USART时钟时使能还是失能，钟低电平活动
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;   //时钟第二个边缘进行数据捕获
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //指定SLCK引脚上时钟的极性
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;  //在SCLK引脚上输出最后发送的那个数据字的脉冲不从SCLK输出
	USART_ClockInit(USARTx, &USART_ClockInitStruct); 
	
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);       //使能接收中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	if(USARTx == USART1)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //设置串口1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //葊占詤袌芏 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //負詤袌芏为0
	}
	else if (USARTx == USART2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //设置串口1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //葊占詤袌芏 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //負詤袌芏为0
	}
	else if (USARTx == USART3)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //设置串口1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //葊占詤袌芏 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;    //負詤袌芏为0
	}
	else if (USARTx == UART4)
	{
		 NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;         //设置串口1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //葊占詤袌芏 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    //負詤袌芏为0
	}
	else if (USARTx == UART5)
	{
		 NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;         //设置串口1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //葊占詤袌芏 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;    //負詤袌芏为0
	}
	else
	{
		return ;
	}
   
  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //使能
  NVIC_Init(&NVIC_InitStructure);
}

void uprintf(USART_TypeDef* USARTx, char *fmt, ...)
{
	char buffer[128+1];
	u8 i = 0;
	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, 128+1, fmt, arg_ptr);
	USART_ClearFlag(USARTx,USART_FLAG_TXE);
	while ((i < 128) && buffer[i])
	{
		if(buffer[i] == '\n')
		{
			
			USART_SendData(USARTx,(u8)'\r');
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
			USART_SendData(USARTx,(u8)buffer[i++]);
		}
		else
		{
	    USART_SendData(USARTx, (u8) buffer[i++]);
		}
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
	}
	va_end(arg_ptr);
 
}



 