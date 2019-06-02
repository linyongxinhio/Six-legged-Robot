#include "sys.h"
#include "delay.h"  
#include "key.h"    	
#include "common.h"
#include "usart.h"  
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "ble.h"
#include "light.h"
#include "voice.h"
#include "music.h"
#include "gesture.h"

//人机交互模式
u8 intelligent_test(void)
{
	u8 p=0;
	VOICE_Init();
	LIGHT_Init();
	while (1)
	{
		if (UART4_RX_CNT) 
		{
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					UART4_RX_CNT=0;
					switch (UART4_RX_BUF[8])
					{
						case 0x31:return 1; break;
						case 0x32:voice_test(); OPEN_VOICE=0; break;//工作模式一：语音识别模式
						case 0x33:gesture_test(); break; //工作模式二：手势识别模式
						case 0x34:music_test(); break;//工作模式三：音乐播放模式
//						case 0x35:printf("lightok");light_test(); break;//工作模式四：防止跌落模式
					}
				}
			}
		  UART4_RX_CNT=0;
		}
	}
}

//主函数
int main(void)
{	 	    
  u8 t;
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	usart2_init(9600);		//初始化串口波特率为9600
	usart3_init(115200);		//初始化串口3 
	uart4_init(115200);		//初始化串口4
	
	BLE_Init();         //蓝牙初始化 

	while(1)
	{	
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
			UART4_RX_CNT=0;
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{ 
				//oder_len=UART4_RX_BUF[2];   //指令的字节长度，待换成10进制
				if (UART4_RX_BUF[3]==0x83)
				{
					//data_len=UART4_RX_BUF[6]*2;  //数据的字长度，待换成16进制
					switch (UART4_RX_BUF[8])
					{
						case 0x01:         //WIFI模式
							atk_8266_test();
							break;
						case 0x02:         //蓝牙模式
							if (ble_test()==1) OPEN_BLE=0;
							break;
						case 0x03:         //人机交互模式
							intelligent_test(); 
							break;
					}
				}
			}
		}
	}
}


















