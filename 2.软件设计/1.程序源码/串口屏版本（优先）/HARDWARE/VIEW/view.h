#ifndef __VIEW_H
#define __VIEW_H	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	

void usart_view(u8 data);
void usart_run_view(u8 adress,u8 data);
void usart_status_view(u8 data);
void view_back(void);
#endif