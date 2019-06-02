#include "ble.h"
#include "lcd.h"

void BLE_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出低
}

//进入蓝牙模式
void ble_test(void)
{
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(25,150,240,24, "短程蓝牙控制模式",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "返回首页",24,1);	
	
	OPEN_BLE=1;
	
	while(1)
	{
    if (window_back()) 	break;
	}
}

