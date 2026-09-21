/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    iwdg.C
  * @author  Yspring Firmware Team  
  * @brief   iwdg Source Code.
  ******************************************************************************      
*/


#include "ys32t031.h"
#include "main.h"


void IWDG_Configuration(void);



// IWDG ≥ı ºªØ≈‰÷√
void IWDG_Configuration(void)
{
  LL_RCC_LSI_Enable();
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_64);
  LL_IWDG_SetReloadCounter(IWDG, 31999);
  while (LL_IWDG_IsReady(IWDG) == RESET){}

  LL_IWDG_SetWindow(IWDG, 4095);
  LL_IWDG_ReloadCounter(IWDG);
  LL_IWDG_Enable(IWDG);
}









