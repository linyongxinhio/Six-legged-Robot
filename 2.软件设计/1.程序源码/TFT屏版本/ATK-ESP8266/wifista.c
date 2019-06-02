#include "common.h"
#include "stdlib.h"
#include "window.h"

//ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码

u8 atk_8266_wifista_test(void)
{
	u8 j=0;
	char online_buf[5]={0xDA,0x00,0x03,0xFF,0xFF};
	u8 netpro=1;	//网络模式
	u8 key;
	u8 timex=0; 
	//u8 ipbuf[16]; 	//IP缓存
	u8 ipbuf[16]="45.43.222.110"; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
PRESTA:
	delay_ms(1000);
	delay_ms(1000);
	netpro|=1;	//选择网络模式
	if(netpro&0X01)     //TCP Client    透传模式测试
	{
		LCD_Clear(BLACK);
		POINT_COLOR=BROWN;
		Show_Str_Mid(0,30,"ESP8266 WIFI-STA 测试",16,240); 
		Show_Str(30,50,200,16,"正在配置ESP8266模块,请稍等...",12,1);
		if(atk_8266_ip_set("WIFI-STA 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf)) //IP输入
		{
			delay_ms(300);
			WINDOW_Init(0,180);
			delay_ms(300);
			return 1;	
		}
		atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
		sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
		while (atk_8266_send_cmd(p,"OK",200))
		{
			LCD_Clear(BLACK);
			POINT_COLOR=BROWN;
			Show_Str_Mid(0,40,"ESP8266 连接TCP Server失败",12,240); //连接失败	
			POINT_COLOR=GRAY;
			LCD_DrawRectangle(0,180+92,180+240,180+140);
			Show_Str(70,320-40,240,24, "返回首页",24,1);   
			for (j=0;j<20;j++) 
      {
        if (window_back()) 
				{
				  atk_8266_quit_trans();//退出透传
		      atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
					return 1; 
				}
				delay_ms(200);
			}
    }			

		atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传			
	}
	
	LCD_Clear(BLACK);
	POINT_COLOR=BROWN;
	Show_Str(0,30,240,12, "作品：基于单片机的六足机器人控制系统设计",12,1);
	Show_Str(0,50,200,12, "学校：天津城建大学                     ",12,1);
	Show_Str(0,70,200,12, "学院：控制与机械工程学院               ",12,1);
	Show_Str(0,90,200,12, "班级：14建筑电气与智能化2班            ",12,1);
	Show_Str(0,110,200,12,"学生名字：林永欣                          ",12,1);
	Show_Str(0,130,200,12,"指导老师：杨国庆                          ",12,1);
   
	LCD_Fill(0,150,239,80+12,BLACK);
	POINT_COLOR=BROWN;
	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
	sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
	Show_Str(0,150,200,12,p,12,1);				//显示IP地址和端口	
	Show_Str(0,170,200,12,"状态:",12,1); 		//连接状态
	Show_Str(90,170,200,12,"模式:",12,1); 		//连接状态
	Show_Str(120,170,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,1); 		//连接状态
	POINT_COLOR=GRAY;
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "返回首页",24,1);	
	
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
			//constate=atk_8266_consta_check();//得到连接状态
			if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
			else t=0;                   //状态为已经连接了,10秒后再检查
		}
		if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
		{
			t=0;
			LCD_Fill(30,170,80,182,BLACK);
			constate=atk_8266_consta_check();//得到连接状态
			if(constate=='+')	
			{
				POINT_COLOR=BROWN;
				Show_Str(30,170,200,12,"连接成功",12,1);  //连接状态
			}
			else 
			{
				POINT_COLOR=GRAY;
				Show_Str(30,170,200,12,"连接失败",12,1); 
				delay_ms(200);
				atk_8266_quit_trans();//退出透传
				atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
				WINDOW_Init(0,180);
				delay_ms(200);
				myfree(SRAMIN,p);		//释放内存 
				return 1; 
      }				
			//发送心跳包
			atk_8266_quit_trans();
			atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传  
			for (j=0;j<5;j++) 
			{  
				USART_SendData(USART3,online_buf[j]); 
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕 
			}	
		}
		for (j=0;j<10;j++) 
		  if (window_back()) 
				{
					myfree(SRAMIN,p);		//释放内存 
					return 1; 
				}
	}
	myfree(SRAMIN,p);		//释放内存 
	return res;		
} 




























