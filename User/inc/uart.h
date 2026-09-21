/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    uart.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   uart Header Code.
  ******************************************************************************      
*/
#ifndef __UART_H
#define __UART_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "ys32t031.h"	
#include "main.h"
#include <stdint.h>  






void UART1_Configuration(void);

void UART1_SendByte(uint8_t Data);
void UART1_Send_Str(uint8_t *String,uint8_t s1);



#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

