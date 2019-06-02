#include "light.h"
#include "lcd.h"
#include "delay.h"

//智能巡光模式初始化
void LIGHT_Init(void)
{    	 
 	GPIO_InitTypeDef GPIO_InitStructure;  //初始化
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PORTF时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PE0~2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOE0,1,2
}

/********************动作组调用函数*************************/
void PrintCom(u8 *DAT,u8 len)
{
	u8 t;
	for(t=0;t<len;t++)
	{
	 USART_SendData(USART1, DAT[t]);//向串口1发送数据
	 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
}

void stopActionGroup()
{
	u8 buf[6];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 2 ;
	buf[3] = 0x07 ;
	PrintCom(buf, 4) ;
}

void runActionGroup(u8 numofAction, int Times)
{
	static u8 buf[9];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 5 ;
	buf[3] = 0x06 ;
	buf[4] = (u8)numofAction ;		
	buf[5] = GET_LOW_BYTE(Times) ;
	buf[6] = GET_HIGH_BYTE((int)Times) ;
	PrintCom(buf, 7) ;
}

void light_handle()
{
	static u8  step = 0;   //静态变量，用于记录步骤
  static u8  lastStep = 0; //静态变量，用于记录最后一步的步骤
	static u8  actiongroup_flag=0;

	switch (step)
	{
		case 0:        //步骤0
      if (!actiongroup_flag)              //如果没有动作组在运行
			{  
        if ((LIGHT0) && (LIGHT1))              //如果前左前右红外传感器均没有悬空
				{ 
          runActionGroup(61,1);    //运行前进的第一步
          step = 1;                            //步骤转移到步骤1
          delay_ms(300);   
        }
				else                     //如果前左或前右任意一个脚红外传感器没有检测到桌面
				{                               
          lastStep = 0;                        //最后步骤赋值为0
          step = 3;                            //转移到步骤3
          delay_ms(200);             //延时200毫秒
        }
      }
      break;   //结束switch语句
    case 1:    //步骤1
      if (!actiongroup_flag) {   //如果没有动作组在运行
        if ((LIGHT0) && (LIGHT1)) {  //如果前左前右红外传感器均没有悬空
          runActionGroup(81, 1);    //运行前进的第二步
          step = 0;                            //转移到步骤0
          delay_ms(300);             //延时300毫秒
        } else {                               //如果前左或前右任意一个脚红外传感器没有检测到桌面
          lastStep = 1;                        //最后步骤赋值为1
          step = 3;                            //转移到步骤3
          delay_ms(200);              //延时200毫秒
        }
      }
      break;
    case 3:  //步骤3
      stopActionGroup();      //发送停止动作组命令
      step = 4;                          //转移到步骤4
      delay_ms(200);             //延时200毫秒
      break;                             //结束switch语句
    case 4:  //步骤4
      if (lastStep == 0) {                //如果最后步骤为0
        runActionGroup(71, 1); //运行81号动作组，81为前进1即71号动作组的反向动作
      }
      if (lastStep == 1) {                //如果最后步骤为1
        runActionGroup(51, 1); //运行61号动作组，61为前进2即51号动作组的反向动作
      }
      delay_ms(300);      //延时300毫秒
      step = 5;                   //转移到步骤5
      break;                      //结束switch语句
    case 5:
      if (!actiongroup_flag) {  //如果没有动作组在运行
        runActionGroup(2, 1); //运行2号动作组2次，2号为后退动作
      }
      delay_ms(300);     //演示600毫秒再执行
      step = 6;                  //转移到步骤6
      break;                     //结束switch语句
    case 6:
      if (!actiongroup_flag) { //如果没有动作组在运行
        if (lastStep == 0) {       //如果最后步骤为0
          runActionGroup(4, 7); //运行4号动作组7次，4号动作组为右转动作,可根据需求修改
        }
        if (lastStep == 1) {      //如果最后步骤为1
          runActionGroup(3, 7);  //运行3号动作组7次， 3号动作组为左转动作
        }
        lastStep = 0; //最后步骤复位0
        step = 0;     //步骤转移回0
      }
      delay_ms(300);  //延时300毫秒
      break;          //结束switch语句
	}
}

//进入智能巡光模式
void light_test(void)
{
	LCD_Clear(BLACK);
  POINT_COLOR=BROWN;	
	Show_Str(50,150,240,24, "防止跌落模式",24,1);	
	POINT_COLOR=GRAY;	
	LCD_DrawRectangle(0,180+92,180+240,180+140);
	Show_Str(70,320-40,240,24, "返回上一页",24,1);		
	while(1)
	{
   	light_handle();
    if (intel_window_back()) 	break;
	}
}

