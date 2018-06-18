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
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 	//usart1--TX		���ų�ʼ��
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//usart1--RX
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if (USARTx == USART2)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 	//usart2--TX		���ų�ʼ��
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	//usart2--RX
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if (USARTx == USART3)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//usart3--TX		���ų�ʼ��
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//usart3--RX
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else if (USARTx == UART4)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	//uart4--TX			���ų�ʼ��
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//uart4--RX
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	else if (USARTx == UART5)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 	//uart5--TX			���ų�ʼ��
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//uart5--RX
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	else 
	{
		return;
	}
	
	USART_InitStructure.USART_BaudRate = baud;                  //���ڻ�������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //����USARTʱ��ʱʹ�ܻ���ʧ�ܣ��ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;   //ʱ�ӵڶ�����Ե�������ݲ���
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //ָ��SLCK������ʱ�ӵļ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;  //��SCLK�������������͵��Ǹ������ֵ����岻��SCLK���
	USART_ClockInit(USARTx, &USART_ClockInitStruct); 
	
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);       //ʹ�ܽ����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	if(USARTx == USART1)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //���ô���1�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //ȀռԅЈܶ 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //ؓԅЈܶΪ0
	}
	else if (USARTx == USART2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //���ô���1�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //ȀռԅЈܶ 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //ؓԅЈܶΪ0
	}
	else if (USARTx == USART3)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //���ô���1�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //ȀռԅЈܶ 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;    //ؓԅЈܶΪ0
	}
	else if (USARTx == UART4)
	{
		 NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;         //���ô���1�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //ȀռԅЈܶ 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    //ؓԅЈܶΪ0
	}
	else if (USARTx == UART5)
	{
		 NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;         //���ô���1�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //ȀռԅЈܶ 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;    //ؓԅЈܶΪ0
	}
	else
	{
		return ;
	}
   
  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ʹ��
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



 