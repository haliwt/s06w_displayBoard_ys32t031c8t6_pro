/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    main.h
  * @author  YSPRING Application Team
  * @version V1.0.1
  * @date    2023.3.20
  * @brief   Library configuration file.
  ******************************************************************************
  */
/* USER CODE END header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ys32t031.h"
#include "ys32t031_ll_utils.h"
#include "ys32t031_ll_system.h"
#include "ys32t031_ll_cortex.h"
#include "ys32t031_ll_bus.h"
#include "ys32t031_ll_adc.h"
#include "ys32t031_ll_comp.h"
#include "ys32t031_ll_crc.h"
#include "ys32t031_ll_dma.h"
#include "ys32t031_ll_exti.h"
#include "ys32t031_ll_flash.h"
#include "ys32t031_ll_gpio.h"
#include "ys32t031_ll_i2c.h"
#include "ys32t031_ll_iwdg.h"
#include "ys32t031_ll_led.h"
#include "ys32t031_ll_lptim.h"
#include "ys32t031_ll_pwr.h"
#include "ys32t031_ll_rcc.h"
#include "ys32t031_ll_rtc.h"
#include "ys32t031_ll_spi.h"
#include "ys32t031_ll_tim.h"
#include "ys32t031_ll_uart.h"
#include "ys32t031_ll_vrefbuf.h"
#include "ys32t031_ll_wwdg.h"
#include "ys32t031_ll_tsc.h"

#if defined(USE_FULL_ASSERT)
#include "ys32_assert.h"
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN includes */

/* USER CODE END includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN types */

/* USER CODE END types */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN constants */

/* USER CODE END constants */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN macro */

/* USER CODE END macro */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN prototypes */

/* USER CODE END prototypes */

/* Private defines -----------------------------------------------------------*/

//LED KEY GPIO
#define KEY_POWER_GPIO_Port 		GPIOB
#define LL_KEY_POWER_Pin 			LL_GPIO_PIN_4

#define KEY_FAN_GPIO_Port 			GPIOB
#define LL_KEY_FAN_Pin 				LL_GPIO_PIN_5

#define KEY_PLASMA_GPIO_Port 		GPIOB
#define LL_KEY_PLASMA_Pin 			LL_GPIO_PIN_6

#define KEY_AI_GPIO_Port 			GPIOB
#define LL_KEY_AI_Pin 				LL_GPIO_PIN_7


//SMG GPIO 
#define MCU_STB_GPIO_Port      	GPIOA
#define LL_MCU_STB_Pin 			LL_GPIO_PIN_5

#define MCU_CLK_GPIO_Port 		GPIOA
#define LL_MCU_CLK_Pin 			LL_GPIO_PIN_6

#define MCU_DIO_GPIO_Port 		GPIOA
#define LL_MCU_DIO_Pin 			LL_GPIO_PIN_7



//CTROL GPIO


//LED GPIO 
#define LED_PLASMA_GPIO_Port GPIOB
#define LL_LED_PLASMA_Pin LL_GPIO_PIN_3

#define LED_FAN_GPIO_Port GPIOB
#define LL_LED_FAN_Pin LL_GPIO_PIN_8

#define LED_POWER_GPIO_Port GPIOB
#define LL_LED_POWER_Pin LL_GPIO_PIN_9

#define LED_WATER_INDICAT_GPIO_Port GPIOF
#define LL_LED_WATER_INDICAT_Pin LL_GPIO_PIN_9

#define LED_AI_GPIO_Port GPIOA
#define LL_LED_AI_Pin LL_GPIO_PIN_15

#define LED_WATER_FULL_GPIO_Port GPIOC
#define LL_LED_WATER_FULL_Pin LL_GPIO_PIN_13

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

