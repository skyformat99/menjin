#ifndef __PlayMode_H__
#define __PlayMode_H__	

#include "stm32f0xx.h"


void Selectmode(void);
void UART_Send_Missage(u8 *Data);
void UART2_Send_Missage(u8 *Data);
int write_flash(u32 StartAddr,u16 *buf,u16 len);
int read_flash(u32 StartAddr,u32 *buf,u16 len);

#endif