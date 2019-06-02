/***************************飞音云电子****************************
**  工程名称：YS-V0.7语音识别模块驱动程序
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.7语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2013.9.13
**  说明：口令模式： 即每次识别时都需要说“小杰”这个口令 ，才能够进行下一级的识别
/***************************飞音云电子******************************/
#include "config.h"
/************************************************************************************/
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //识别处理函数
void delay(unsigned long uldata);
void User_handle(uint8 dat);//用户执行操作函数
void Delay200ms();
void Led_test(void);//单片机工作指示
uint8_t G0_flag=DISABLE;//运行标志，ENABLE:运行。DISABLE:禁止运行 
sbit LED=P4^2;//信号指示灯

/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i=0;
	Led_test();
	MCU_init();
	LD_Reset();
	UartIni(); /*串口初始化*/
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
	
	while(1)
	{
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
				break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}
			case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
			{				
				nAsrRes = LD_GetResult();		/*获取结果*/
				User_handle(nAsrRes);//用户执行函数 
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				break;
			}
		}// switch	 			
	}// while

}
/***********************************************************
* 名    称： 	 LED灯测试
* 功    能： 单片机是否工作指示
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void Led_test(void)
{
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
}
/***********************************************************
* 名    称： void MCU_init()
* 功    能： 单片机初始化
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void MCU_init()
{
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;
	

	LD_MODE = 0;		//	设置MD管脚为低，并行模式读写
	IE0=1;
	EX0=1;
	EA=1;
}
/***********************************************************
* 名    称：	延时函数
* 功    能：
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void Delay200us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

void  delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	while(uldata--)
	Delay200us();
}

void Delay200ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/***********************************************************
* 名    称： 中断处理函数
* 功    能：
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				
}

/********************动作组调用函数*************************/
void stopActionGroup()
{
	volatile  uint8_t buf[6];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 2 ;
	buf[3] = 0x07 ;
	buf[4] = 0X0D;
	buf[5] = 0x0A;	
	PrintCom(buf, 6) ;
}
void runActionGroup(uint8_t numofAction, uint16_t Times)
{
	static volatile  uint8_t buf[9];
	buf[0] = 0x55 ;
	buf[1] = 0x55 ;
	buf[2] = 5 ;
	buf[3] = 0x06 ;
	buf[4] = (uint8_t)numofAction ;		
	buf[5] = GET_LOW_BYTE(Times) ;
	buf[6] = GET_HIGH_BYTE((uint16_t)Times) ;
  buf[7] = 0x0d;
	buf[8] = 0x0a;
	PrintCom(buf, 9) ;
	//PrintCom(buf,7) ;
	LED = !LED;
	Delay200ms();
	Delay200ms();
	Delay200ms();
}

/***********************************************************
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改 
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void 	User_handle(uint8 dat)
{
	 int	i=0;
	 if(0==dat)
	 {
			G0_flag=ENABLE;
		LED=0;
		PrintCom("ok\r\n",4); /*text.....*/
	 }
	 else if(ENABLE==G0_flag)
	 {	
			G0_flag=DISABLE;
			LED=1;
		  switch(dat)		   /*对结果执行相关操作,客户可删除Printcom 串口输出语句替换为其他需要控制的代码*/
			{
				case CODE_STOP:			/*命令“停”*/
						 stopActionGroup();
						 break;
				case CODE_FORWARD:	 /*命令“前进”*/
						 runActionGroup(33,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(1,3);
						 break;					
				case CODE_BACKWARD:		/*命令“后退”*/				
						 runActionGroup(34,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(2,3);	
						 break;
				case CODE_LEFTROTATE:		/*命令“左转”*/				
						 runActionGroup(35,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(3,2);
						 break;
				case CODE_RIGHTROTATE:		/*命令“右转”*/				
						 runActionGroup(36,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(4,2);
						 break;
				case CODE_LEFT:		/*命令“左走”*/				
						 runActionGroup(37,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(7,3);
						 break;
				case CODE_RIGHT:		/*命令“右走”*/				
						 runActionGroup(38,1);
						 for (i=0;i<10;i++) Delay200ms();
						 runActionGroup(8,3);
						 break;
				case CODE_DANCE:			/*命令“跳个舞”*/
						 runActionGroup(43,1);
						 for (i=0;i<12;i++) Delay200ms();
						 runActionGroup(9,2);
						 break;
				case CODE_CANSING:	 /*命令“会唱歌吗”*/
						 runActionGroup(44,1);
						 break;
				case CODE_APPLESING:		/*命令“唱小苹果”*/				
						 runActionGroup(45,1);  
						 for (i=0;i<13;i++) Delay200ms();
						 runActionGroup(92,1);
						 break;
				case CODE_COMEBACKSING:		/*命令“唱你快回来”*/				
						 runActionGroup(46,1); 
						 for (i=0;i<20;i++) Delay200ms();
						 runActionGroup(91,1);
						 break;	
				case CODE_TENYEARSSING:		/*命令“唱十年”*/				
						 runActionGroup(47,1); 
						 for (i=0;i<16;i++) Delay200ms();
						 runActionGroup(97,1);
						 break;		
				case CODE_CHINASING:		/*命令“唱一首我爱你中国”*/				
						 runActionGroup(48,1); 
						 for (i=0;i<20;i++) Delay200ms();
						 runActionGroup(98,1);
						 break;			
				case CODE_INTUODUCE:		/*命令“自我介绍”*/				
						 runActionGroup(49,1);
						 break;	
				case CODE_STANDUP:		/*命令“立正”*/				
						 runActionGroup(99,1);
						 break;	
				case CODE_NAME:		/*命令“你叫什么名字”*/				
						 runActionGroup(38,1);
						 break;		
				case CODE_CANDANCE:		/*命令“会跳舞吗”*/				
						 runActionGroup(42,1);
						 break;	
				case CODE_WEATHER:		/*命令“天气怎么样”*/				
						 runActionGroup(39,1);
						 break;
				case CODE_SING:		/*命令“唱一首歌”*/				
						 runActionGroup(98,1);
						 break;		
				default:								
						 runActionGroup(37,1);
						 break;
			}	
		}	
		else 	
		{
			PrintCom("请说出一级口令\r\n",16); /*text.....*/	
		}
}	 
