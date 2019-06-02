#ifndef __PAJ7620U2_H
#define __PAJ7620U2_H

#include "paj7620u2_iic.h"



//BANK寄存器枚举
typedef enum{
	BANK0 = 0, //BANK0寄存器
	BANK1,     //BANK1寄存器
}bank_e;

#define PAJ7620_ID           0x73<<1 //设备地址
#define PAJ_REGITER_BANK_SEL 0XEF    //BANK选择寄存器
#define PAJ_BANK0            0X00    //BANK0
#define PAJ_BANK1            0X01    //BANK1

//BANK0 寄存器组
#define PAJ_SUSPEND_CMD            0X03 //设置设备挂起
#define PAJ_SET_INT_FLAG1          0X41 //设置手势检测中断寄存器1
#define PAJ_SET_INT_FLAG2          0X42 //设置手势检测中断寄存器2
#define PAJ_GET_INT_FLAG1          0X43 //获取手势检测中断标志寄存器1(获取手势结果)
#define PAJ_GET_INT_FLAG2          0X44 //获取手势检测中断标志寄存器2(获取手势结果)
#define PAJ_GET_STATE              0X45 //获取手势检测工作状态
#define PAJ_SET_HIGH_THRESHOLD     0x69 //设置滞后高阀值（仅在接近检测模式下）
#define PAJ_SET_LOW_THRESEHOLD     0X6A //设置滞后低阀值
#define PAJ_GET_APPROACH_STATE     0X6B //获取接近状态 （1：PS data>= PS threshold ,0:PS data<= Low threshold）
#define PAJ_GET_GESTURE_DATA       0X6C //获取接近数据
#define PAJ_GET_OBJECT_BRIGHTNESS  0XB0 //获取被照物体亮度（最大255）
#define PAJ_GET_OBJECT_SIZE_1      0XB1 //获取被照物体大小低八位（bit7:0）(最大900)
#define PAJ_GET_OBJECT_SIZE_2      0XB2 //获取被照物体大小高四位（bit3:0）

//BANK1 寄存器组
#define PAJ_SET_PS_GAIN         0X44 //设置检测增益大小 (0:1x gain 1:2x gain)
#define PAJ_SET_IDLE_S1_STEP_0  0x67 //设置S1的响应因子
#define PAJ_SET_IDLE_S1_STEP_1  0x68 
#define PAJ_SET_IDLE_S2_STEP_0  0X69 //设置S2的响应因子
#define PAJ_SET_IDLE_S2_STEP_1  0X6A 
#define PAJ_SET_OP_TO_S1_STEP_0 0X6B //设置OP到S1的过度时间
#define PAJ_SET_OP_TO_S1_STEP_1 0X6C
#define PAJ_SET_S1_TO_S2_STEP_0 0X6D //设置S1到S2的过度时间
#define PAJ_SET_S1_TO_S2_STEP_1 0X6E
#define PAJ_OPERATION_ENABLE    0X72 //设置PAJ7620U2使能寄存器

//手势识别效果
#define BIT(x) 1<<(x)

#define GES_UP              BIT(0) //向上
#define GES_DOWM            BIT(1) //向下
#define GES_LEFT            BIT(2) //向左
#define GES_RIGHT           BIT(3) //向右
#define GES_FORWARD         BIT(4) //向前
#define GES_BACKWARD        BIT(5) //向后
#define GES_CLOCKWISE       BIT(6) //顺时针
#define GES_COUNT_CLOCKWISE BIT(7) //逆时针
#define GES_WAVE            BIT(8) //挥动

u8 paj7620u2_init(void);
void paj7620u2_sensor_test(void);


#endif

