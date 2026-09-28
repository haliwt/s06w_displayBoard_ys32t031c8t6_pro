#ifndef __GPIO_H
#define __GPIO_H
#include "ys32t031.h"
#include "main.h"


//LED GPIO 
#define LED_POWER_GPIO_Port GPIOA
#define LL_LED_POWER_Pin LL_GPIO_PIN_1

#define LED_MOUSE_GPIO_Port GPIOA
#define LL_LED_MOUSE_Pin LL_GPIO_PIN_11

#define LED_PLASMA_GPIO_Port GPIOA
#define LL_LED_PLASMA_Pin LL_GPIO_PIN_12

#define LED_TIME_GPIO_Port GPIOF
#define LL_LED_TIME_Pin LL_GPIO_PIN_4

#define LED_DRY_GPIO_Port GPIOF
#define LL_LED_DRY_Pin LL_GPIO_PIN_6

#define LED_WIFI_GPIO_Port GPIOF
#define LL_LED_WIFI_Pin LL_GPIO_PIN_7




//KEY GPIO
#define KEY_POWER_GPIO_Port GPIOA
#define LL_KEY_POWER_Pin LL_GPIO_PIN_4

#define KEY_MODEL_GPIO_Port GPIOA
#define LL_KEY_MODEL_Pin LL_GPIO_PIN_5

#define KEY_DOWN_GPIO_Port GPIOA
#define LL_KEY_DOWN_Pin LL_GPIO_PIN_6

#define KEY_UP_GPIO_Port GPIOA
#define LL_KEY_UP_Pin LL_GPIO_PIN_7

#define KEY_WIFI_GPIO_Port GPIOA
#define LL_KEY_WIFI_Pin LL_GPIO_PIN_8


#define KEY_MOUSE_GPIO_Port GPIOB
#define LL_KEY_MOUSE_Pin LL_GPIO_PIN_13

#define KEY_STER_GPIO_Port GPIOB
#define LL_KEY_STER_Pin LL_GPIO_PIN_14

#define KEY_DRY_GPIO_Port GPIOB
#define LL_KEY_DRY_Pin LL_GPIO_PIN_15


//TM1639 GPIO
#define MCU_CLK_GPIO_Port GPIOC
#define LL_MCU_CLK_Pin LL_GPIO_PIN_13

#define MCU_STB_GPIO_Port GPIOC
#define LL_MCU_STB_Pin LL_GPIO_PIN_14

#define MCU_DIO_GPIO_Port GPIOF
#define LL_MCU_DIO_Pin LL_GPIO_PIN_9

//USART DISPLAY BOARD GPIO 
#define DSP1_TX_GPIO_Port GPIOA
#define LL_DSP1_TX_Pin LL_GPIO_PIN_9

#define DSP1_RX_GPIO_Port GPIOA
#define LL_DSP1_RX_Pin LL_GPIO_PIN_10










void GPIO_Configuration(void);

#endif 


