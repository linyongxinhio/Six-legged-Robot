#ifndef __LIGHT_H
#define __LIGHT_H	 
#include "sys.h"

#define GET_LOW_BYTE(A) (u8)((A))
#define GET_HIGH_BYTE(A) (u8)((A) >> 8)

#define LIGHT0  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)//读取前左传感器
#define LIGHT1  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)//读取前右传感器
#define LIGHT2  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)//读取后左传感器 
#define LIGHT3  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)//读取后右传感器

void LIGHT_Init(void);
void light_test(void);

#endif
