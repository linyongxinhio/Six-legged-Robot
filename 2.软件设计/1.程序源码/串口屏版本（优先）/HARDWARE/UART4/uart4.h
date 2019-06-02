#ifndef __UART4_H
#define __UART4_H	 
#include "sys.h"  
   
extern u8 UART4_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 UART4_RX_CNT;   			//接收到的数据长度

//模式控制
//如果想串口中断接收，请不要注释以下宏定义
#define EN_UART4_RX 	1			//0,不接收;1,接收.




void Uart4_Init(u32 bound);
void UART4_Send_Data(u8 *buf,u8 len);
void UART4_Receive_Data(u8 *buf,u8 *len);
#endif













