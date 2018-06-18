#include "stm32f10x_can.h"

/******************************�����Ҫ˵��**********************************/
/* ���ܣ�CAN�շ�
 * ʹ�õ�����:CAN1��CAN2��NVIC�жϿ�����
 * ʹ�õ�����:CAN1: CAN_RX PA11 , CAN_TX PA12
 *            CAN2: CAN_RX PB12 , CAN_TX PB13
 * ʹ��˵��:    ���������ѡ��ʹ��CAN1����CAN2����������ʱ�����⣬�Ҿ�û��дʵ������
 *          ����ͬʱʹ�õĴ��롣��CAN_SELECT����ѡ��ʹ����һ��CAN�����ú�CAN1_ID
 *          ����CAN2_ID���������Լ������͵����ݰ��ı�ʶ����CAN_MODE_Xѡ��CAN��ʹ
 *          �õ�ģʽ��ͨ��ΪCAN_Mode_Normalģʽ��������ģʽ��CAN���ߵ�ʱ��ǳ���
 *          Ҫ������������APB1ʱ��Ϊ42MHz�����õ�CAN����ʱ�򣬺�CAN_SJW_X��
 *          CAN_BS1_X��CAN_BS2_X��CAN_PRESCALER_X��������CAN���ߵ�λʱ���ͨ����
 *          �ʡ��ڱ������У���ֻʹ��һ��������������ģʽ��32λ����ģʽ����ͨ����
 *          CAN_FILTER_ID��CAN_FILTER_MASK��������Ҫ���˳��ı�ʶ�������ݰ���
 *              �����������жϵķ�ʽ����ȡ���ݵġ�ÿ�ν��յ�����֮�󣬻����һ��
 *          �������ú�����ʹ�����Լ�����ĺ������ڶ�CAN���г�ʼ��ʱ���Ѹú�����ַ
 *          ��Ϊ������ִ�к���can_init(msg_rcv_func),msg_rcv_func()�����ͻ��ڽ��յ�
 *          can��������֮���Զ����á�msg_rcv_func()�����Ĳ�������ΪCanRxMsg���Ͷ�
 *          �ҷ���ֵ��void��Ҳ���������붨�������(����������ν):
 *                      void msg_rcv_func(CanRxMsg* can_rx_msg){
 *                          
 *                      }
 *              �ṹ��can_rx_msg��CAN���߽��յ������ݣ������ں���msg_rcv_func()
 *          ������ݸ��Ƶ������ط����洢��Ҳ�����ں�����ֱ�Ӷ����ݽ��д���Ȼ��
 *          ִ����Ӧ�����������Ҳ��������һ����������Ϊmsg_rcv_func()��������һ
 *          ���жϺ���������õģ�Ҳ������msg_rcv_func()��ִ�й��̾����жϴ����
 *          �ɣ����ж���ͣ��̫�ã�������ʹ�ù��̳���һЩ���벻�������⡣�жϵ���
 *          �ȼ�����ͨ����CAN_NVIC_IRQPP��CAN_NVIC_IRQSP�����á�
 * 16��5��22�Ÿ���:can_send_msg()���������˱�ʶ������
 * 16��5��25�Ÿ���:�޸���can_init�������ú���û�в����ˡ�������can_add_callback()
 *          �������ú����������Ӷ�Ӧ��ʶ����callback��������ִ�к���
 *          can_add_callback(0x11,msg_rcv_func)����֮��ÿ�����յ���ʶ����0x11��
 *          CAN���ݰ�ʱ�������Զ����ú���msg_rcv_func(CanRxMsg can_rx_msg)
 *          
 *          
 */ 

#define CAN1_ID 0x12     //stm��CAN���ߵı�ʶ��

#define CAN_MODE_X CAN_Mode_Normal  //ģʽ
//CAN����ʱ�����������(��ʱ��Ϊ1Mbps)
#define CAN_SJW_X CAN_SJW_1tq
#define CAN_BS1_X CAN_BS1_3tq   //ʱ���1��ʱ�䵥Ԫ
#define CAN_BS2_X CAN_BS2_2tq   //ʱ���2��ʱ�䵥Ԫ
#define CAN_PRESCALER_X 6     //��Ƶϵ��
/* tq = (CAN_PRESCALER_X + 1)*tpclk
 * ���� = 1/((CAN_SJW_X + CAN_BS1_X + CAN_BS2_X)*tq)
 */


//����������(32λ)
#define CAN_FILTER_ID 0x00000000   //�����Ϊ0������������ʶ�������ݰ�
#define CAN_FILTER_MASK 0x00000000
//�ж����ȼ�����
#define CAN1_NVIC_IRQPP 1              //CAN1�ж��������ȼ�
#define CAN1_NVIC_IRQSP 1              //CAN1�ж������ȼ�

#define CANX CAN1
#define CAN_ID CAN1_ID



typedef struct{
    u16 can_id;
    void (*msg_rcv_callback)(CanRxMsg *can_rx_msg);
}can_callback_struct;

int can_init(int number);
void can_gpio_config(void);
void can_nvic_config(void);
int can_send_msg(u8 can_id,u8 *msg,u8 len);  //��������
int can_rcv_msg(void);   //������Ϣ
int can_add_callback(u8 can_id,void *msg_rcv_callback_func);
void USB_LP_CAN1_RX0_IRQHandler(void);
