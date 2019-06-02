/***************************飞音云电子****************************
**  工程名称：YS-V0.7语音识别模块驱动程序
**	CPU: STC89LE52
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.7语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.4.1
**  说明：口令模式： 即每次识别时都需要说“小杰”这个口令 ，才能够进行下一级的识别
/***************************飞音云电子******************************/


#ifndef LD_CHIP_H
#define LD_CHIP_H

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

//	以下五个状态定义用来记录程序是在运行ASR识别过程中的哪个状态
#define LD_ASR_NONE				0x00	 /*	表示没有在作ASR识别*/
#define LD_ASR_RUNING			0x01	/*	表示LD3320正在作ASR识别中*/
#define LD_ASR_FOUNDOK			0x10	/*表示一次识别流程结束后，有一个识别结果*/
#define LD_ASR_FOUNDZERO 		0x11	/*表示一次识别流程结束后，没有识别结果*/
#define LD_ASR_ERROR	 		0x31	/*表示一次识别流程中LD3320芯片内部出现不正确的状态*/


#define CLK_IN   		22.1184	/* 用户注意修改输入的晶振时钟大小 */
#define LD_PLL_11			(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

//函数声明
void LD_Reset();
void LD_Init_Common();
void LD_Init_ASR();
uint8 RunASR(void);
void LD_AsrStart();
uint8 LD_AsrRun();
uint8 LD_AsrAddFixed();
uint8 LD_GetResult();


//识别码客户修改处 
#define CODE_CMD             0x00   //该命令码0x00用户不可进行修改。
#define CODE_STOP            0x01   //停止
#define CODE_FORWARD         0x02   //前进
#define CODE_BACKWARD        0x03   //后退
#define CODE_LEFTROTATE      0x04   //左转
#define CODE_RIGHTROTATE     0x05   //右转
#define CODE_LEFT            0x06   //向左走
#define CODE_RIGHT           0x07   //向右走
#define CODE_DANCE           0x08   //跳舞
#define CODE_CANSING         0x09   //会唱歌吗
#define CODE_APPLESING       0x10   //小苹果
#define CODE_COMEBACKSING    0x11   //你快回来
#define CODE_TENYEARSSING    0x12   //十年
#define CODE_CHINASING       0x13   //我爱你中国
#define CODE_INTUODUCE       0x14   //自我介绍
#define CODE_STANDUP         0x15   //立正
#define CODE_NAME            0x16   //你叫什么名字
#define CODE_CANDANCE        0x17   //会跳舞吗
#define CODE_WEATHER         0x18   //天气如何
#define CODE_SING            0x19   //唱一首歌


//数值越大越灵敏识别距离越远，但误识别率就越大， 根据自己的实际情况调节。
#define MIC_VOL 0x65	 //咪头增益（灵敏度调节） 范围：00-7f 
#endif
