#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "main.h"


void usart1_isr_callback_handler(uint8_t data);


void decoder_handler(void);

uint8_t bcc_check(const unsigned char *data, int len) ;


#endif 

