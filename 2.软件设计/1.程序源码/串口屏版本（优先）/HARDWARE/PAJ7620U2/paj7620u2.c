#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "uart4.h"

char stop_send_buf[5]={0x55,0x55,0x02,0x07};

//选择PAJ7620U2 BANK区域
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0寄存器区域
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1寄存器区域
	}	
}

//PAJ7620U2唤醒
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	data = GS_Read_Byte(0x00);//读取状态
	if(data!=0x20) return 0; //唤醒失败
	
	return 1;
}

//PAJ7620U2初始化
//返回值：0:失败 1:成功
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC初始化
    status = paj7620u2_wakeup();//唤醒PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//初始化PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	return 1;
}

//手势识别测试
u8 handleGesture(void)
{
  u8 t;
	u8 time1;
	u8 i=0;
  u8 status;
	u8 data[2]={0x00};
	u16 gesture_data1;
	char gesture_send_buf[7]={0x55,0x55,0x05,0x06,0x00,0x01,0x00};
	
AGAIN1:	
	while(!paj7620u2_init());//PAJ7620U2传感器初始化 	
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域

	time1=0;
	while(1)
	{
    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态			
		if(!status)
		{   
			gesture_data1 =(u16)data[1]<<8 | data[0];
			if(gesture_data1) 
			{
				switch(gesture_data1)
				{
					case GES_UP: 
						   gesture_send_buf[4]=0x01; break; //向上
					case GES_DOWM: 
						   gesture_send_buf[4]=0x02; break; //向下
					case GES_LEFT: 
						   gesture_send_buf[4]=0x07; break; //向左
					case GES_RIGHT: 
						   gesture_send_buf[4]=0x08; break; //向右
					case GES_FORWARD: 
						   gesture_send_buf[4]=0x14; break; //向前
					case GES_BACKWARD:  
							 gesture_send_buf[4]=0x19; break; //向后
//					case GES_CLOCKWISE:
//						   gesture_send_buf[4]=0x01; break; //顺时针
//					case GES_COUNT_CLOCKWISE: 
//						   gesture_send_buf[4]=0x01; break; //逆时针
					case GES_WAVE: 
						   gesture_send_buf[4]=0x0A; break; //挥动
					default: 
						   gesture_send_buf[4]=0x00; break;
				}	
        for(i=0;i<7;i++)
				{
					USART_SendData(USART1, gesture_send_buf[i]);//向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}				
			}	
		}		
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[8]==0x37)
					{
						UART4_RX_CNT=0;
						for(t=0;t<4;t++)
						{
							USART_SendData(USART1, stop_send_buf[t]);//向串口1发送数据
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
						}
						GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//关闭手势识别中断输出
						GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
						return 1;
					}
				}
			}
		  UART4_RX_CNT=0;
		}
		delay_ms(50);	 
    time1++;
    if (time1==100) goto AGAIN1;			
	}
}


u8 music_handleGesture(void) 
{
	u8 t,time2,i=0;
	u8 status;
	u8 data[2]={0x00};
	u16 gesture_data2;
	static char musicing=0xc8;
	static char music_send_buf[7]={0x55,0x55,0x05,0x06,0x00,0x01,0x00};

AGAIN2:	
	while(!paj7620u2_init());//PAJ7620U2传感器初始化 		
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	time2=0;
	while(1)
	{
    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态			
		if(!status)
		{  			
			gesture_data2 =(u16)data[1]<<8 | data[0];
			if(gesture_data2) 
			{
				switch(gesture_data2)
				{
					case GES_UP: 
							 musicing++;
							 if (musicing>220) musicing=0xc8;
							 music_send_buf[4]=musicing;
							 for(t=0;t<7;t++)
							 {
							   USART_SendData(USART1, music_send_buf[t]);//向串口1发送数据
							 	 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
						   }
							 break;
					case GES_DOWM: 
						   musicing--;
							 if (musicing<200) musicing=0xDC;
							 music_send_buf[4]=musicing;
							 for(t=0;t<7;t++)
							 {
							   USART_SendData(USART1, music_send_buf[t]);//向串口1发送数据
								 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
							 }
						   break;
					case GES_LEFT: 
						   for(t=0;t<4;t++)
							 {
							   USART_SendData(USART1, stop_send_buf[t]);//向串口1发送数据
								 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
							 }
						break;
					case GES_RIGHT: 
						   for(t=0;t<7;t++)
							 {
							 	 USART_SendData(USART1, music_send_buf[t]);//向串口1发送数据
								 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
							 }
						   break;
					default:break;
				}				
			}	
		}
		if (UART4_RX_CNT) 
		{	
			delay_ms(100);
      if ((UART4_RX_BUF[0]==0xa5) && (UART4_RX_BUF[1]==0x5a))  //普通按键返回
			{
				if (UART4_RX_BUF[3]==0x83)
				{
					if (UART4_RX_BUF[8]==0x38)
					{
						UART4_RX_CNT=0;
            for(t=0;t<4;t++)
						{
							USART_SendData(USART1, stop_send_buf[t]);//向串口1发送数据
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
						}
						GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//关闭手势识别中断输出
						GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
						return 1;
					}
				}
			}
		  UART4_RX_CNT=0;
		}
		delay_ms(50);	   
		time2++;
    if (time2==100) goto AGAIN2;	
	}
}
