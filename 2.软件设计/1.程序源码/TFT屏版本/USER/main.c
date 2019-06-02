#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "lcd.h"
#include "key.h"  
#include "usmart.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "usmart.h"	
#include "touch.h"		
#include "usart3.h"
#include "common.h" 
#include "string.h"
#include "usart2.h"
#include "window.h"
#include "ble.h"
 
int main(void)
{	 
	u8 channel=0; 	    

	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	usart2_init(9600);		//初始化串口波特率为9600
	usart3_init(115200);		//初始化串口3 
	LCD_Init();			   		//初始化LCD   
	W25QXX_Init();				//初始化W25Q128
	tp_dev.init();				//初始化触摸屏
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH.
	font_init();			//检查字库是否OK
	
  WINDOW_Init(0,180);          //开机界面显示处理
	BLE_Init();         //蓝牙初始化 
	
	while (1)
	{
		channel=window_get_keynum(0,180);
		switch (channel)
		{
			case 1:delay_ms(300);atk_8266_test();	break;	//工作模式一：WIFI模式
			case 2:delay_ms(300);ble_test(); OPEN_BLE=0;break; //工作模式二：蓝牙模式
	    case 3:delay_ms(300);intelligent_test(); break;//工作模式三：智能语音模式
		}
	}
}


















