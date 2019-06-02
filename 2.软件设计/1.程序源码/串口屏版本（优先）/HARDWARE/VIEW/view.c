#include "view.h"

//串口屏发指令显示
void usart_view(u8 data)
{
	u8 i;
	u8 * hex_str;
  u8 VIEW_TX_BUF[7]={0xa5,0x5a,0x04,0x80,0x03,0x00,0x00};
	VIEW_TX_BUF[6]=data;
	for(i=0;i<7;i++)
	{
		USART_SendData(UART4, VIEW_TX_BUF[i]);//向串口屏发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

//串口屏动画显示
void usart_run_view(u8 adress,u8 data)
{
	u8 i;
	u8 * hex_str;
  u8 RUN_VIEW_TX_BUF[8]={0xa5,0x5a,0x05,0x82,0x00,0x05,0x00,0x00};
	RUN_VIEW_TX_BUF[5]=adress;
	RUN_VIEW_TX_BUF[7]=data;
	for(i=0;i<8;i++)
	{
		USART_SendData(UART4, RUN_VIEW_TX_BUF[i]);//向串口屏发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

//连接状态 
void usart_status_view(u8 data)
{
	u8 i;
	u8 * hex_str;
  u8 STATUS_VIEW_TX_BUF[16]={0xa5,0x5a,0x0d,0x82,0x01,0x12,0xc1,0xac,0xbd,0xd3,0x00,0x00,0x00,0x00,0xff,0xff};
	if (data==1)
	{
		STATUS_VIEW_TX_BUF[10]=0xB3;
		STATUS_VIEW_TX_BUF[11]=0xC9;
		STATUS_VIEW_TX_BUF[12]=0xB9;
		STATUS_VIEW_TX_BUF[13]=0xA6;
	}
	else 
	{
		STATUS_VIEW_TX_BUF[10]=0xCA;
		STATUS_VIEW_TX_BUF[11]=0xA7;
		STATUS_VIEW_TX_BUF[12]=0xB0;
		STATUS_VIEW_TX_BUF[13]=0xDC;		
	}
	for(i=0;i<16;i++)
	{
		USART_SendData(UART4, STATUS_VIEW_TX_BUF[i]);//向串口屏发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}