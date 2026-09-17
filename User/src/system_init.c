/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    system_init.C
  * @author  Yspring Firmware Team  
  * @brief   system_init Source Code.
  ******************************************************************************      
*/

#include "system_init.h"
//#include "bsp.h"
#include "delay.h"
#include "ys32t031.h"
#include "ys32t031_it.h"   
#include <stdint.h>



void RCC_Configuration(void);

void NVIC_Configuration(void);





//NVIC中断配置
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
	  NVIC_InitStructure.NVIC_IRQChannel = TIM6_LPTIM_IRQn ;   //设置中断来源
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;          //设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	  NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;         //IRQ通道:串口1
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;          //优先级 :1级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //使能IRQ通道
    NVIC_Init(&NVIC_InitStructure);
	
	  NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;         //IRQ通道:串口1
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;          //优先级 :1级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //使能IRQ通道
    NVIC_Init(&NVIC_InitStructure);
}










