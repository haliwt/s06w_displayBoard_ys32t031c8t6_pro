/**
  ******************************************************************************
  * @file    system_ys32t031.c
  * @author  ys Application Team
  * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer System Source File.
  *
  * 1. This file provides two functions and one global variable to be called from
  *    user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_ys32t031.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  */

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ys32t031.h"
#include "system_ys32t031.h"

#ifdef USE_HAL_DRIVER
#include "ys32t031_hal_conf.h"
#elif USE_FULL_HAL_DRIVER
#include "ys32t031_hal_conf.h"
#elif USE_LL_DRIVER
#include "ys32t031_ll_rcc.h"
#elif USE_FULL_LL_DRIVER
#include "ys32t031_ll_rcc.h"
#endif

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = 16000000;

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
const uint32_t HSIFreqTable[4] = {48000000U, 36000000U, 24000000U, 16000000U};

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit(void)
{
  /* NOTE :SystemInit(): This function is called at startup just after reset and 
                         before branch to main program. This call is made inside
                         the "startup_ys32t031.s" file.
                         User can setups the default system clock (System clock source, PLL Multiplier
                         and Divider factors, AHB/APBx prescalers and Flash settings).
   */
  uint32_t reg;

  reg = RCC->ICSCR;
  reg &= ~(RCC_ICSCR_HSI_FS | RCC_ICSCR_HSI_TRIM);
  reg |= ((RCC_ICSCR_HSI_FS_16Mhz) | ((*(uint32_t *)(0x1FFF1C0C)) & 0x7FF));                  //16M
  RCC->ICSCR = reg;
  
  reg = RCC->CR;
  reg &= ~(RCC_CR_HSIDIV);
  reg |= RCC_CR_HSIDIV_1;              //DIV1
  RCC->CR = reg;
}
     
void SystemCoreClockUpdate(void)
{
  uint32_t hsidiv;
  uint32_t sysclockfreq;
  uint32_t hsiIndex;
  uint32_t pllsource;
  
  if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI)
  {
    /* HSISYS can be derived for HSI */
    hsidiv = (1UL << ((READ_BIT(RCC->CR, RCC_CR_HSIDIV)) >> RCC_CR_HSIDIV_Pos));

    /* HSISYS used as system clock source */
    hsiIndex = (RCC->ICSCR&RCC_ICSCR_HSI_FS_Msk)>>RCC_ICSCR_HSI_FS_Pos;
    if (hsiIndex > 3)
    {
      hsiIndex = 0;
    }
    sysclockfreq = (HSIFreqTable[hsiIndex] / hsidiv);
  }
  else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSE)
  {
    /* HSE used as system clock source */
    sysclockfreq = HSE_VALUE;
  }

  else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)
  {
    pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);

    switch (pllsource)
    {
    case RCC_PLLCFGR_PLLSRC:  /* HSE used as PLL clock source */
      if(RCC->CFGR & RCC_CFGR_PLLMUL)
        sysclockfreq =  HSE_VALUE  * 4;
      else
        sysclockfreq =  HSE_VALUE  * 2;
      break;

    case 0U:  /* HSI used as PLL clock source */
    default:
      hsiIndex = (RCC->ICSCR&RCC_ICSCR_HSI_FS_Msk)>>RCC_ICSCR_HSI_FS_Pos;
      if (hsiIndex > 3)
      {
        hsiIndex = 0;
      }
      if(RCC->CFGR & RCC_CFGR_PLLMUL)
        sysclockfreq =  HSIFreqTable[hsiIndex]  * 4;
      else
        sysclockfreq =  HSIFreqTable[hsiIndex]  * 2;
      break;

    }
  }

  else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_LSE)
  {
    /* LSE used as system clock source */
    sysclockfreq = LSE_VALUE;
  }

  else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_LSI)
  {
    /* LSI used as system clock source */
    sysclockfreq = LSI_VALUE;
  }
  else
  {
    sysclockfreq = 0U;
  }
  
  //AHB
  SystemCoreClock = sysclockfreq >> (AHBPrescTable[ ((RCC->CFGR >> RCC_CFGR_HPRE_Pos) & 0x0F) ]);
}


#if defined(__CC_ARM)

#endif

