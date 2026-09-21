#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "ys32t031.h"
#include "main.h"

#define KEY_POWER_VALUE()         ((GPIOA->IDR & LL_GPIO_PIN_4) ? 1 : 0 )//sys_read_gpio_pin_value(GPIOD, KEY_POWER_Pin) //GPIO_1 按键按下，返回�??: 1

#define KEY_MODEL_VALUE()         ((GPIOA->IDR & LL_GPIO_PIN_5) ? 1 : 0 ) //sys_read_gpio_pin_value(GPIOD, KEY_MODE_Pin)

#define KEY_DOWN_VALUE()          ((GPIOA->IDR & LL_GPIO_PIN_6) ? 1 : 0 )

#define KEY_UP_VALUE()          ((GPIOA->IDR & LL_GPIO_PIN_7) ? 1 : 0 )//sys_read_gpio_pin_value(GPIOD, KEY_DOWN_Pin)

#define KEY_MOUSE_VALUE()        ((GPIOB->IDR & LL_GPIO_PIN_13) ? 1 : 0 )

#define KEY_PLASMA_VALUE()        ((GPIOB->IDR & LL_GPIO_PIN_14) ? 1 : 0 )

#define KEY_DRY_VALUE()            ((GPIOB->IDR & LL_GPIO_PIN_15) ? 1 : 0 )


typedef enum{

    KEY_DOWN,
	KEY_UP
}key_state;

extern uint8_t key_worked_f;
//extern uint8_t key_long_f;
extern uint16_t key_data;
extern uint16_t key_time;



void Key_Scan(void);

void System_Status_PowerOff(void) ;

void System_Status_PowerOn(void) ;

void key_power_short_handler(void);

void key_power_long_handler(void);

void key_mode_short_handler(void);

void key_mode_long_handler(void);

void key_up_short_handler(void);


void key_down_short_handler(void);

void key_down_long_handler(void);


#endif 

