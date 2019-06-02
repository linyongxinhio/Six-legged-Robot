#include "voice.h"
#include "sys.h"
#include "usart2.h"	
#include "uart4.h"	

//语音识别模式初始化
void VOICE_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED1->PE.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE.5
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);						 //PE.5 输出低
}

//进入语音识别模式
u8 voice_test(void)
{
	char stop_send_buf[5]={0x55,0x55,0x02,0x07};
  int j=0;
	int t=0;
  OPEN_VOICE=1;
	while(1)
	{
		if(USART2_RX_STA&0x8000)
		{					   
			j=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<j;t++)
			{
				USART_SendData(USART1, USART2_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			USART2_RX_STA=0;
		}
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[8]==0x36)
					{
						UART4_RX_CNT=0;
						for(t=0;t<4;t++)
						{
							USART_SendData(USART1, stop_send_buf[t]);//向串口1发送数据
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
						}
						return 1;
					}
				}
			}
		  UART4_RX_CNT=0;
		}
	}
}

