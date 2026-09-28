#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "main.h"


#define LED_POWER_ON()      LL_GPIO_ResetOutputPin(LED_POWER_GPIO_Port, LL_LED_POWER_Pin)
#define LED_POWER_OFF()     LL_GPIO_SetOutputPin(LED_POWER_GPIO_Port, LL_LED_POWER_Pin)

#define LED_FAN_ON()       LL_GPIO_ResetOutputPin(LED_FAN_GPIO_Port, LL_LED_FAN_Pin)
#define LED_FAN_OFF()      LL_GPIO_SetOutputPin(LED_FAN_GPIO_Port, LL_LED_FAN_Pin)

#define LED_PLASMA_ON()    LL_GPIO_ResetOutputPin(LED_PLASMA_GPIO_Port, LL_LED_PLASMA_Pin)
#define LED_PLASMA_OFF()   LL_GPIO_SetOutputPin(LED_PLASMA_GPIO_Port, LL_LED_PLASMA_Pin)

#define LED_KEY_AI_ON()        LL_GPIO_ResetOutputPin(LED_AI_GPIO_Port, LL_LED_AI_Pin)
#define LED_KEY_AI_OFF()       LL_GPIO_SetOutputPin(LED_AI_GPIO_Port, LL_LED_AI_Pin)



//WATER FULL OR EMPTY
#define LED_WATER_FULL_ON()      	LL_GPIO_ResetOutputPin(LED_WATER_FULL_GPIO_Port,LL_LED_WATER_FULL_Pin)
#define LED_WATER_FULL_OFF()        LL_GPIO_SetOutputPin(LED_WATER_FULL_GPIO_Port,LL_LED_WATER_FULL_Pin)
#define LED_WATER_FULL_TOGGLE()       LL_GPIO_TogglePin(LED_WATER_FULL_GPIO_Port,LL_LED_WATER_FULL_Pin)


#define LED_WATER_INDICATE_ON()    	LL_GPIO_ResetOutputPin(LED_WATER_INDICAT_GPIO_Port,LL_LED_WATER_INDICAT_Pin)
#define LED_WATER_INDICATE_OFF()    LL_GPIO_SetOutputPin(LED_WATER_INDICAT_GPIO_Port,LL_LED_WATER_INDICAT_Pin)



void power_on_led_handler(void);

void power_off_led_handler(void);


void water_full_led_blink(void);



#endif 

