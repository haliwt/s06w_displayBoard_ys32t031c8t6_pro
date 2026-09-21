#ifndef __BSP_SENSOR_H
#define __BSP_SENSOR_H
#include "main.h"


#include "ys32t031.h"
#include "tx_api.h"
#include <stdint.h>

/* 根据你的实际接线修改 */
#define DHT11_GPIO_PORT      GPIOA
#define DHT11_GPIO_PIN       GPIO_Pin_4
#define DHT11_GPIO_CLK       RCC_AHB2Periph_GPIOA

#define DHT11_DATA_PIN              GPIO_Pin_4
#define DHT11_DATA_GPIO_PORT        GPIOA


/* 对外接口 */







void Delay_US_dht11(uint16_t us);




#endif 

