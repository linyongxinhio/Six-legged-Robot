#include "common.h"
#include "stdlib.h"
#include "view.h"
#include "uart4.h"

//ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码

u8 atk_8266_wifista_test(void)
{
	u8 i,j=0;
	char online_buf[5]={0xDA,0x00,0x03,0xFF,0xFF};
	u8 netpro=1;	//网络模式
	//u8 IP_BUF[16]; 	//IP缓存
	u8 IP_BUF[16]="45.43.222.110"; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u16 rlen=0;
	u8 constate=0;	//连接状态
  
BEGIN1:
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	
	if (UART4_RX_CNT) 
	{	
		UART4_RX_CNT=0;
		if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
		{
			if ((UART4_RX_BUF[3]==0x83) && (UART4_RX_BUF[8]==0x04))
			{
				myfree(SRAMIN,p);		//释放内存 
				usart_view(0x08);
				return 1;  
			}
		}
	}
	
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300))   //连接目标路由器,并且获得IP
	{
		if (UART4_RX_CNT) 
		{	
			UART4_RX_CNT=0;
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{
				if ((UART4_RX_BUF[3]==0x83) && (UART4_RX_BUF[8]==0x04))
				{
					myfree(SRAMIN,p);		//释放内存 
					usart_view(0x08);
					return 1;  
				}
			}
		}
		delay_ms(10);
	}					

	delay_ms(1000);
	delay_ms(1000);
	netpro|=1;	//选择网络模式
	usart_run_view(0x05,2);
	usart_view(0x0b);
	usart_run_view(0x06,2);
	while (netpro&0X01)     //TCP Client    透传模式测试
	{
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[2]==0x06)
					{
						if (UART4_RX_BUF[8]==0x12)
						{
							myfree(SRAMIN,p);		//释放内存 
							UART4_RX_CNT=0;
							usart_view(0x08);
							return 1;
						}
						else if (UART4_RX_BUF[8]==0x11)
						{
							UART4_RX_CNT=0;
							delay_ms(100);
							usart_run_view(0x06,1);
							atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
							sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",IP_BUF,(u8*)PORTNUM);    //配置目标TCP服务器
							while (atk_8266_send_cmd(p,"OK",200))
							{
								if (UART4_RX_CNT) 
								{	
									UART4_RX_CNT=0;
									if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
									{
										if ((UART4_RX_BUF[3]==0x83) && (UART4_RX_BUF[8]==0x12))
										{
											myfree(SRAMIN,p);		//释放内存 
											usart_view(0x08);
											return 1;  
										}
									}
								}
							}
							atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传	
							delay_ms(100);
							usart_view(0x02);//连接处理
							usart_run_view(0x06,2);
							goto BEGIN2;
						}
					}
					else 
					{
						UART4_RX_CNT=0;
						i=7;
						while (UART4_RX_BUF[i]!=0xff)
						{
              IP_BUF[i-7]=UART4_RX_BUF[i];
							i++;
						}					
					}
				}
			}
		  UART4_RX_CNT=0;
		}
    delay_ms(100);		
	}

BEGIN2:	
	USART3_RX_STA=0;
	while(1)
	{
		t++;
		delay_ms(10);
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
			USART3_RX_BUF[rlen]=0;		//添加结束符 
			
		  DataTransfer((char*)USART3_RX_BUF,rlen-2);//解析收到的数据，将其转换成16进制指令
			USART3_RX_STA=0;
//			constate=atk_8266_consta_check();//得到连接状态
//			if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
//			else t=0;                   //状态为已经连接了,10秒后再检查
		}
		if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
		{
			t=0;
			constate=atk_8266_consta_check();//得到连接状态
			if(constate=='+')	
			{
				usart_status_view(1); //连接状态成功
			}
			else 
			{
				usart_status_view(0); //连接状态失败
      }				
			
			//发送心跳包
			atk_8266_quit_trans();
			atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传  
			for (j=0;j<5;j++) 
			{  
				//printf(online_buf);
				USART_SendData(USART3,online_buf[j]); 
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕 
			}	
		}
		
		if (UART4_RX_CNT) 
		{	
			if (UART4_RX_BUF[8]==0x13)
			{
				myfree(SRAMIN,p);		//释放内存 
				UART4_RX_CNT=0;
				usart_view(0x08);
				return 1;  
			}
		}
	}	
	myfree(SRAMIN,p);		//释放内存 	
} 




























