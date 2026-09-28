/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    TIM.C
  * @author  Yspring Firmware Team  
  * @brief   TIM Source Code.
  ******************************************************************************      
*/

#include "ys32t031.h"
#include "tim.h"
#include "bsp.h"


// TIM6 ?????
void TIM6_Configuration(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  // Overflow time = ((Auto-reload 9999 + 1) * (Prescaler 63 + 1)) / 64000000 = 10 ms, frequency= 100 Hz
  LL_TIM_StructInit(&TIM_InitStruct);
  TIM_InitStruct.Prescaler = 63;
  TIM_InitStruct.Autoreload = 9999; // TIM_ARR
  LL_TIM_Init(TIM6, &TIM_InitStruct);

  LL_TIM_EnableIT_UPDATE(TIM6);
  LL_TIM_EnableCounter(TIM6);
}



// TIM17 初始化配置
void TIM17_Configuration(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM17);

  // Overflow time = ((Auto-reload 0 + 1) * (Prescaler 63 + 1)) / 64000000 = 1000 ns, frequency= 1000 kHz
  LL_TIM_StructInit(&TIM_InitStruct);
  TIM_InitStruct.Prescaler = 63;
  TIM_InitStruct.Autoreload   = 0xFFFF; // 最大周期 // TIM_ARR
  TIM_InitStruct.ClockDivision = 0;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM17, &TIM_InitStruct);

  //LL_TIM_EnableIT_UPDATE(TIM17);
  LL_TIM_EnableCounter(TIM17);
}








