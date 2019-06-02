#include "music.h"
#include "lcd.h"
#include "intel_window.h"
#include "sys.h"



//语音识别模式初始化
void MUSIC_Init(void)
{
//  GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

//  //GPIOF9,F10初始化设置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
//	
//	GPIO_ResetBits(GPIOF,GPIO_Pin_10);//GPIOF10设置低
}


//进入音乐播放模式
void music_test(void)
{
	u8 i;
	u8 flag=0;
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(50,150,240,24, "音乐播放模式",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(65,320-40,240,24, "返回上一页",24,1);	
	
	while(!paj7620u2_init());//PAJ7620U2传感器初始化 
	
	music_handleGesture();//处理手势

}

