#include "cmd.h"
#include "stm32f10x_usart.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "misc.h"
#include "can.h"
#include "USART.h"


static cmd_struct cmd_tbl[] = {
    /*
     * 若需添加命令，需要在此加上：
     * CMD_ADD("命令名","命令使用方法（可为空格,但不能不加双引号）",对应命令的执行函数名)
     * 注意最后一个不需要逗号，前面的都需要逗号
     */
    CMD_ADD("help","Print all command and usage",cmd_help_func),
    
};

static char cmd_line[MAX_CMD_LINE_LENGTH + 1];
static char *cmd_argv[MAX_ARGC];


void cmd_init(int baud){
    int i;
	
    Usart_init(USART_Debug , baud );
#if CMD_PERIPH_INIT_EN == 1
    USART_InitTypeDef usart_init_stru;
    NVIC_InitTypeDef nvic_init_stru;
#endif
    for(i = 0;i < MAX_ARGC;i++){
        cmd_argv[i] = (char *)malloc(MAX_CMD_ARG_LENGTH + 1);
    }
}



int cmd_parse(char *cmd_line,int *argc,char *argv[]){
    char c_temp;
    int i = 0,arg_index = 0;
    int arg_cnt = 0;
    c_temp = cmd_line[i++];  
    while(c_temp != '\r'){
        if(c_temp == ' '){
            if(arg_index == 0){   //如果命令或者参数字符串第一个是空格，则忽略   
                c_temp = cmd_line[i++];
                continue;
            }
            //空格为参数或者命令的分隔符
            if(arg_cnt == MAX_ARGC){   //如果参数个数过多,则返回
                return -1;
            }
            argv[arg_cnt][arg_index] = 0;
            arg_cnt++;//字数
            arg_index = 0;
            c_temp = cmd_line[i++];
            continue;
        }
        if(arg_index == MAX_CMD_ARG_LENGTH){   //如果参数长度过长，则报错返回
            return -2;
        }
        argv[arg_cnt][arg_index++] = c_temp;
        c_temp = cmd_line[i++];
    }
    if(arg_cnt == 0 && arg_index == 0){  //如果命令或者参数是空的，则返回
        return -3;
    }
    //最后一个参数的结束没有在上面的while循环中解析到
    argv[arg_cnt++][arg_index] = 0;
    *argc = arg_cnt;
    return 0;
}

int cmd_exec(int argc,char *argv[]){
    int cmd_index = 0;
    u32 cmd_num;
 
    cmd_num = sizeof(cmd_tbl)/sizeof(cmd_tbl[0]);

    if(argc == 0){  //如果参数是空的，则返回
        return -1;
    }
    for(cmd_index = 0;cmd_index < cmd_num;cmd_index++){   //查找命令
        if(strcmp((char *)(cmd_tbl[cmd_index].cmd_name),(char *)argv[0]) == 0){  //如果找到了命令，则执行命令相对应的函数
            //uprintf(CMD_USARTx,"\n");
            cmd_tbl[cmd_index].cmd_func(argc,argv);
            return 0;
        }
    }
    return -2;
}


#if CMD_USARTn == 1
void USART1_IRQHandler(void){
#elif CMD_USARTn == 3
void USART3_IRQHandler(void){
#endif 
    static u32 cmd_line_index = 0,cmd_line_length = 0;
    int cmd_argc,i;
    int erro_n;
    u8 c_recv;
    if(USART_GetITStatus(CMD_USARTx,USART_IT_RXNE) != RESET){      
        USART_ClearITPendingBit(CMD_USARTx,USART_IT_RXNE);
        c_recv = USART_ReceiveData(CMD_USARTx);			  //< = '0x5B'  ESC = 0x1B
        if(c_recv == '\n'){  //接受完一次指令
			//uprintf(CMD_USARTx,"done");
            if(cmd_line_index == 0){
                return;
            }
            cmd_line[cmd_line_length++] = (char)c_recv;
            erro_n = cmd_parse(cmd_line,&cmd_argc,cmd_argv);  //解析命令
            if(erro_n < 0){
                //打印函数执行错误信息
                if(erro_n == -3){
                cmd_line_index = 0;
                cmd_line_length = 0;
                memset(cmd_line,0,MAX_CMD_LINE_LENGTH);
                return;
                }else if(erro_n == -2){
                    uprintf(CMD_USARTx,"\nthe param is too long");
                }else if(erro_n == -1){
                    uprintf(CMD_USARTx,"\ntoo many param");
                }
                cmd_line_index = 0;
                cmd_line_length = 0;
                memset(cmd_line,0,MAX_CMD_LINE_LENGTH + 1);
                return;
            }
           
            erro_n = cmd_exec(cmd_argc,cmd_argv );   //执行命令
            if(erro_n < 0){
                //打印函数执行错误信息
                if(erro_n == -2){
                  	  uprintf(CMD_USARTx,"\r\nnot find commmand:%s\n",cmd_argv[0]);
                }
                cmd_line_index = 0;
                cmd_line_length = 0;
                memset(cmd_line,0,MAX_CMD_LINE_LENGTH + 1);
                return;
            }
            cmd_line_index = 0;
            cmd_line_length = 0;
            memset(cmd_line,0,MAX_CMD_LINE_LENGTH + 1);
        }else{
            if(cmd_line_index == MAX_CMD_LINE_LENGTH){
                //打印命令行太长的信息
                cmd_line_index = 0;
                cmd_line_length = 0;
                return;
            }
            for(i = 0;i < cmd_line_length - cmd_line_index;i++){
                cmd_line[cmd_line_length - i] = cmd_line[cmd_line_length - i -1];
            }
            cmd_line[cmd_line_index] = (char)c_recv;
			cmd_line_index++;
            cmd_line_length++;
			//uprintf(CMD_USARTx,"%c",c_recv);
        }			
    }
}



/////////////////////////////////下面所有方法都是具体的指令执行方法///////////////////////////////////////////////
void cmd_help_func(int argc,char *argv[]){
    int i;
    u32 cmd_num;
    cmd_num = sizeof(cmd_tbl)/sizeof(cmd_tbl[0]);
    if(argc > 1){
        uprintf(CMD_USARTx,"error\n");		
        return;			
    }
    for(i = 0;i < cmd_num;i++){  
        uprintf(CMD_USARTx,"cmd:%s   usage:%s\n",cmd_tbl[i].cmd_name,cmd_tbl[i].cmd_usage);
    }
}


/************************************
*   @fun : 保存参数
*
*
***************************************/
void cmd_reboot(int argc,char *argv[]){
    __disable_fault_irq();   
    NVIC_SystemReset();
}













