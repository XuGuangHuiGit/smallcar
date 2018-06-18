#include "can.h"

static void (*msg_rcv_callback)(CanRxMsg *can_rx_msg);
static CanRxMsg g_can_rx_msg;
can_callback_struct can_cb_array[256]; 

/* 函数名：void can_init(void *)
 * 功能：can总线初始化
 * 参数：
 * 返回值：初始化成功则返回1
 */
int can_init(int number)
{
    int i;
	
	int check_out_ID;
    CAN_InitTypeDef can_init_struct;
	CAN_FilterInitTypeDef can_FilterInitTypeDef;
	
    can_gpio_config();
    CAN_DeInit(CANX); 
    CAN_StructInit(&can_init_struct);
    can_init_struct.CAN_TTCM = DISABLE;  //非时间触发通信模式
    can_init_struct.CAN_AWUM = DISABLE;  //睡眠模式通过软件唤醒
    can_init_struct.CAN_ABOM = DISABLE;  //软件自动离线管理
    can_init_struct.CAN_NART = DISABLE;   //禁止报文自动传送
    can_init_struct.CAN_RFLM = DISABLE;  //报文不锁定，新的覆盖旧的
    can_init_struct.CAN_TXFP = ENABLE;  //优先级由报文标识符决定
    can_init_struct.CAN_Mode = CAN_MODE_X;     //模式设置
    can_init_struct.CAN_SJW  = CAN_SJW_X;     //同步跳跃宽度
    can_init_struct.CAN_BS1  = CAN_BS1_X; //范围CAN_BS1_1tq ~ CAN_BS1_16tq
    can_init_struct.CAN_BS2  = CAN_BS2_X; //范围CAN_BS1_1tq ~ CAN_BS1_16tq
    can_init_struct.CAN_Prescaler = CAN_PRESCALER_X;  //分频系数(Fdiv)CAN_PRESCALER_X + 1
    CAN_Init(CANX,&can_init_struct);
     

	if((number &0xF0) !=0){
		check_out_ID = number<<5;
	}else {
		check_out_ID = number<<9;
	}

	can_FilterInitTypeDef.CAN_FilterNumber = 0;
	can_FilterInitTypeDef.CAN_FilterMode = CAN_FilterMode_IdMask;
	can_FilterInitTypeDef.CAN_FilterScale = CAN_FilterScale_32bit;
	can_FilterInitTypeDef.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	
	if(number != 0){
		can_FilterInitTypeDef.CAN_FilterMaskIdLow = 0x0000;
		if((number &0xF0) !=0){
			can_FilterInitTypeDef.CAN_FilterMaskIdHigh = 0xFF<<5;
		}else{
			can_FilterInitTypeDef.CAN_FilterMaskIdHigh = 0xF<<9;
		}
		
		can_FilterInitTypeDef.CAN_FilterIdLow = 0x0000;
		can_FilterInitTypeDef.CAN_FilterIdHigh = check_out_ID;//屏蔽部分ＩＤ
	}else{
		can_FilterInitTypeDef.CAN_FilterMaskIdLow = 0x0000;
		can_FilterInitTypeDef.CAN_FilterMaskIdHigh = 0;
		can_FilterInitTypeDef.CAN_FilterIdLow = 0x0000;
		can_FilterInitTypeDef.CAN_FilterIdHigh = 0;
	}
	can_FilterInitTypeDef.CAN_FilterActivation = ENABLE;
	
	CAN_FilterInit(&can_FilterInitTypeDef);
	
	
	
//    CAN1->FMR |= 1; //过滤器组工作在初始化模式  
//    CAN1->FMR &= 0xffffc0ff;//对FMR寄存器中CAN2SB[5:0]进行清零
//    CAN1->FMR |= (14<<8);  //CAN2的过滤器组从14开始  
//    CAN1->FM1R &= ~(1<<0);//过滤器组14的寄存器工作在屏蔽模式  
//    CAN1->FS1R |= (1<<0);//过滤器组14为单个32位寄存器
//    CAN1->FFA1R = 0x00000000;//全部关联到FIFO0
//	
//    CAN1->FA1R &= ~(1<<0);//禁用过滤器组0
//    CAN1->sFilterRegister[0].FR1 &= 0x00000000;  
//    CAN1->sFilterRegister[0].FR2 &= 0x00000000;  
//    CAN1->sFilterRegister[0].FR1 |= CAN_FILTER_ID;  
//    CAN1->sFilterRegister[0].FR2 |= CAN_FILTER_MASK;  
//    CAN1->FA1R |= (1<<0);//使能过滤器组0

//    CAN1->FMR &= ~1; //过滤器组正常工作
    CAN_ITConfig(CANX,CAN_IT_FMP0,ENABLE);  //FIFO0消息挂号中断允许

    for(i = 0;i < 256;i++){
        can_cb_array[i].can_id = i;
        can_cb_array[i].msg_rcv_callback = 0;
    }
    can_nvic_config();
    return 1;
}

/* 函数名：int can_add_callback(u8 can_id,void *msg_rcv_callback_func)
 * 功能：增加对应标识符的CALLBACK函数
 * 参数：u8 can_id,接收消息的CALLBACK函数对应的标识符
         void *msg_rcv_callback_fuc 邮件接收回调函数的指针，每当接收到所需要的
 *       消息时会自动调用该函数。注意：这里是在接收中断里调用这个函数的，所以该
 *       函数不宜执行太久。如果该函数指针为NULL，则不使用邮件接收中断。
 * 返回值：-1,已经存在函数
 *          1,成功增加函数
 */
int can_add_callback(u8 can_id,void *msg_rcv_callback_func){
    if(can_cb_array[can_id].msg_rcv_callback != 0){
        return -1;
    }
    can_cb_array[can_id].msg_rcv_callback = msg_rcv_callback_func;
    return 1;
}

void can_gpio_config(){
	
    GPIO_InitTypeDef gpio_init_stru;
    gpio_init_stru.GPIO_Mode = GPIO_Mode_IPU;
    gpio_init_stru.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_stru.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA,&gpio_init_stru);
	
	gpio_init_stru.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init_stru.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA,&gpio_init_stru);
}

/* 函数名：void can_nvic_config()
 * 功能：设置can的中断控制器nvic
 * 参数：
 * 返回值：
 */
void can_nvic_config(){
    NVIC_InitTypeDef nvic_init_struct;

    nvic_init_struct.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = CAN1_NVIC_IRQPP;
    nvic_init_struct.NVIC_IRQChannelSubPriority = CAN1_NVIC_IRQSP;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
}

/* 函数名：int can_send_msg()
 * 功能：can发送一组数据
 * 参数：u8  can_id 数据包的标识符
 *       u8* msg 数据数组
 *       u8  len 数据数组长度，最大为8
 * 返回值：-1,表示发送失败
 *         1 ,表示发送成功
 */
int can_send_msg(u8 can_id,u8 *msg,u8 len){
    CanTxMsg tx_msg;
    u8 mbox;
    u16 i = 0;

    tx_msg.StdId = can_id;
    tx_msg.IDE = CAN_Id_Standard;   //使用标准标识符
    tx_msg.RTR = CAN_RTR_Data;   //消息类型为数据帧
    tx_msg.DLC = len;   //消息长度
    for(i = 0;i < len;i++){
        tx_msg.Data[i] = msg[i];
    }
    mbox = CAN_Transmit(CANX,&tx_msg);  //开始发送消息
    i = 0;
    while((CAN_TransmitStatus(CANX,mbox) == CAN_TxStatus_Failed) && (i < 0xfff))i++;
    if(i >= 0xfff) return -1;
    return 1;
}
/*
*can1总线接受中断
*
*/

void USB_LP_CAN1_RX0_IRQHandler(void){
    if(CAN_GetITStatus(CANX,CAN_IT_FMP0) != RESET){
        CAN_Receive(CANX,CAN_FIFO0,&g_can_rx_msg);
        if(can_cb_array[g_can_rx_msg.StdId].msg_rcv_callback != 0){
            (can_cb_array[g_can_rx_msg.StdId].msg_rcv_callback)(&g_can_rx_msg);
        }
        CAN_ClearITPendingBit(CANX,CAN_IT_FMP0);
    }
}







