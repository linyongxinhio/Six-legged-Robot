#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "sys.h"



#define GS_SDA_IN()  {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=8<<16;}
#define GS_SDA_OUT() {GPIOE->CRL&=0XFFF0FFFF;GPIOE->CRL|=3<<16;}

//IO²Ù×÷º¯Êý	 
#define GS_IIC_SCL    PEout(3) 		//SCL
#define GS_IIC_SDA    PEout(4) 		//SDA	 
#define GS_READ_SDA   PEin(4) 		    //ÊäÈëSDA 

u8 GS_Write_Byte(u8 REG_Address,u8 REG_data);
u8 GS_Read_Byte(u8 REG_Address);
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);
#endif


