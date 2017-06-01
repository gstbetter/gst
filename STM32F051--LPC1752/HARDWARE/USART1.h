
#ifndef _USART1_H
#define _USART1_H

#include "stm32f0xx.h"
#include <stdio.h>


void USART1_Init(uint32_t baud);
void usart_transmit(unsigned char data);
void usart_send_string(char *str);
//unsigned char Usart_receive();
#endif
