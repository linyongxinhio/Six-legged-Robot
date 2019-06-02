#ifndef __WINDOW_H
#define __WINDOW_H	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "lcd.h"   
#include "w25qxx.h" 	 
#include "touch.h" 	 
#include "malloc.h"
#include "string.h"    
#include "ff.h"

void WINDOW_Init(u16 x,u16 y);
void window_key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 window_get_keynum(u16 x,u16 y);
u8 window_back(void);

#endif
