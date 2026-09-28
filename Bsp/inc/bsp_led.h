#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "main.h"




#define LED_PLASMA_OFF()             do{LED_PLASMA_GPIO_Port-> BSRR = LL_LED_PLASMA_Pin;}while(0)//{GPIO_SetBits(PLASMA_GPIO_PORT, PLASMA_PIN);}
#define LED_PLASMA_ON()            do{LED_PLASMA_GPIO_Port-> BSRR =(uint32_t)LL_LED_PLASMA_Pin <<16;}while(0)//{GPIO_ResetBits(PLASMA_GPIO_PORT, PLASMA_PIN);}




#define LED_WIFI_OFF()          		do{LED_WIFI_GPIO_Port ->BSRR = LL_LED_WIFI_Pin;}while(0)        
#define LED_WIFI_ON()              do{LED_WIFI_GPIO_Port ->BSRR = (uint32_t)LL_LED_WIFI_Pin << 16;}while(0)  
#define LED_WIFI_TOGGLE()           do{LED_WIFI_GPIO_Port->ODR ^= LL_LED_WIFI_Pin;}while(0)

#define LED_DRY_OFF()                do{LED_DRY_GPIO_Port ->BSRR = LL_LED_DRY_Pin;}while(0)
#define LED_DRY_ON()               do{LED_DRY_GPIO_Port ->BSRR = (uint32_t)LL_LED_DRY_Pin<<16;}while(0)



#define LED_MOUSE_OFF()             do{LED_MOUSE_GPIO_Port->BSRR = LL_LED_MOUSE_Pin;}while(0)
#define LED_MOUSE_ON()            do{LED_MOUSE_GPIO_Port->BSRR = (uint32_t)LL_LED_MOUSE_Pin<<16;}while(0)



#define LED_POWER_ON()          do{LED_POWER_GPIO_Port->BSRR =(uint32_t)LL_LED_POWER_Pin<<16 ;}while(0)//{GPIO_ResetBits(LED_POWER_GPIO_PORT, LED_POWER_PIN);}
#define LED_POWER_OFF()         do{LED_POWER_GPIO_Port->BSRR = LL_LED_POWER_Pin;}while(0)//GPIO_SetBits(LED_POWER_GPIO_PORT, LED_POWER_PIN);}
#define LED_POWER_TOGGLE()      do{LED_POWER_GPIO_Port->ODR ^= LL_LED_POWER_Pin;}while(0)//GPIO_TogglePin(LED_POWER_GPIO_PORT, LED_POWER_PIN)





#define LED_TIME_ON()          do{LED_TIME_GPIO_Port ->BSRR = LL_LED_TIME_Pin;}while(0)
#define LED_TIME_OFF()         do{LED_TIME_GPIO_Port ->BSRR = (uint32_t)LL_LED_TIME_Pin<<16;}while(0) 






void all_led_off(void);

void wifi_fast_led_state(void);


void wifi_led_state_handler(void);
void power_on_led_open_handler(void);






#endif 

