#include "delay.h"
#include "uart4.h"


//接收缓存区 	
u8 UART4_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 UART4_RX_CNT=0;   		  
  
void UART4_IRQHandler(void)
{
	u8 res;	 
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
	  res =USART_ReceiveData(UART4);//;读取接收到的数据
		UART4_RX_BUF[UART4_RX_CNT]=res;		//记录接收到的值
		UART4_RX_CNT++;						//接收数据增加1 
	}  											 
}						

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void uart4_init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART4时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
	//UART-RX-PC11  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 
	USART_InitStructure.USART_BaudRate = bound;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  
	USART_InitStructure.USART_Parity = USART_Parity_No ;  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(UART4, &USART_InitStructure);   
	USART_Cmd(UART4, ENABLE);     
	USART_ClearFlag(UART4,USART_FLAG_TC); 
  
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //使能串口4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
   
  USART_Cmd(UART4, ENABLE);                    //使能串口 
}


















