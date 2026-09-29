#ifndef __BSP_H
#define __BSP_H
#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>




#include "ys32t031.h"
#include "main.h"

#include "uart.h"  
#include "tim.h"
#include "iwdg.h"
//#include "key.h"
#include "adc.h"
#include "gpio.h"
#include "dma.h"




#include "tx_api.h"







//
#include "bsp_power.h"
#include "bsp_led.h"

#include "bsp_key.h"
#include "bsp_cmd_link.h"
#include "bsp_usart.h"
#include "bsp_tm1639.h"
#include "bsp_key_app.h"
#include "bsp_tim.h"
#include "bsp_smg_app.h"


#include "bsp_threadx.h"


//wifi


#define Enable_EventRecorder  0

#define DEBUG_ENABLE    0





#if(Enable_EventRecorder == 1)
	#include "EventRecorder.h"
#endif


typedef struct _main_ref{

 
 bool g_power_flag ;
 uint8_t on_step;
 uint8_t off_step;
 uint8_t set_up_temperature_value;
 uint8_t set_temperature_decade_value;
 uint8_t set_temperature_unit_value;
 bool  set_temperature_special_flag;
 bool g_manual_shutoff_dry_flag ;

 int8_t timer_dispTime_minutes;
 uint8_t timer_dispTime_hours ;

 uint8_t works_dispTime_hours;
 int8_t works_dispTime_minutes;


 uint8_t works_two_minutes_value;

 uint8_t hours_two_decade_bit    ;
 uint8_t hours_two_unit_bit  ;
 uint8_t minutes_one_decade_bit ;

 bool g_plasma_flag;
 bool g_dry_flag;
 bool g_mouse_flag;

 bool g_time_disp_colon_flag;

 bool key_add_dec_pressed_flag;
 bool set_timer_timing_value_success;
 bool ui_time_mode;
 bool one_minute_flag;
 bool timer_one_minute_flag;
 bool connecting_wifi_flag;

 
 uint8_t first_set_ptc_on;
 uint8_t  first_ptc_on;
 uint8_t  set_timer_first_smg_blink_flag;
 

 uint8_t set_timer_timing_doing_value;
 uint8_t dht11_temperature_value;
 uint8_t dht11_humidity_value;
 

 
 bool key_model_short_flag ;

 //wifi
 bool wifi_led_fast_blink; 
 bool  connect_wifi_state;

 //timer
 uint8_t seting_timer_flag;
 uint8_t first_temp_compare_f;
 uint8_t setting_temperature_value;



 volatile uint8_t time_10ms_f;
 volatile uint8_t time_20ms_f;
 volatile uint8_t time_50ms_f;

 //volatile uint8_t time_200ms_f;
 volatile uint8_t time_100ms_fast_led_f;

 volatile uint8_t time_400ms_f;
 volatile uint8_t time_500ms_f;
 volatile uint8_t time_600ms_f;
 volatile uint8_t time_900ms_f;










  volatile uint8_t gTimer_key_temp_timing  ;
  volatile uint8_t gTimer_key_timing ;
  volatile uint8_t gTimer_disp_mode_switch;
  volatile uint8_t gTimer_wifi_connect_counter;
  volatile uint8_t gTimer_timer_seconds_counter;
  volatile uint8_t gTimer_timing_seconds_counter;
  volatile uint8_t  gTimer_time_colon ;




}main_ref;

extern main_ref gpro_t;



void bsp_init(void);


void task_scheduler(void);





uint32_t Get_Unique_ID_32bit(void);




#endif 

