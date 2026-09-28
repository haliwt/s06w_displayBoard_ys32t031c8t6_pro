#ifndef __BSP_H
#define __BSP_H
#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h> // 确保包含此头文件以使用 NULL 定义，或者通过 main.h 引入
#include <stdbool.h>



#include "ys32t031.h"
#include "main.h"

#include "dma.h"
#include "uart.h"  
#include "tim.h"
#include "iwdg.h"


//


#include "tx_api.h"

#include "bsp_key.h"

#include "bsp_tm1639.h"
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_time.h"
#include "bsp_cmd_link.h"
#include "bsp_threadx.h"



#include "interrupt_manager.h"



#define Enable_EventRecorder  0

#define DEBUG_ENABLE    0





#if(Enable_EventRecorder == 1)
	#include "EventRecorder.h"
#endif


typedef struct _main_ref{

	bool g_power_flag;

	bool g_ai_flag;
	uint8_t g_fan_speed;
	bool g_plasma_flag;


	int16_t temperature;
	int16_t humidity;



	bool dma_dong_flag ;

	bool fan_warning_flag;

	bool water_pos_warning_flag ;

    volatile uint8_t gTimer_water_led_blink;
	volatile uint8_t gTimer_counter;
	volatile uint8_t gTimer_set_timer_counter ;

}main_ref;

extern main_ref gpro_t;



void bsp_init(void);











#endif 

