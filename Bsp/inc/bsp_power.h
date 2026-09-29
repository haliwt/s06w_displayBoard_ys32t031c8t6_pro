/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.H
  * @version V1.0.0
  * @date    2024
  * @author  Yspring Firmware Team  
  * @brief   user Header Code.
  ******************************************************************************      
*/
#ifndef __BSP_POWER_H
#define __BSP_POWER_H

#ifdef __cplusplus
extern "C" {
#endif
   
#include "ys32t031.h"
#include "system_ys32t031.h"  
#include <stdint.h> 


extern uint8_t ptc_high_temperature_f;

extern uint16_t current_temperature;

extern uint16_t disp_temperature;
extern uint16_t disp_timing_time;
extern uint16_t disp_humidity;




extern bool ptc_prohibit_off_f;



extern uint16_t timing_is_reach_disptime;
/*countdown timer  */
extern int8_t setting_timing_hour;


extern uint8_t real_hours_counter;
extern int8_t temporary_timer_hours;








extern uint8_t fan_open_f;
extern uint8_t fan_speed_level;





//
extern uint8_t soft_version;



extern uint8_t humidity;




extern uint8_t fan_warning_f;

#define _NO_FAN_LOAD_CURRENT       50      //0.06A*0.67*4096/3.3   


//peripheral 
extern uint8_t key_be_pressed_f;
extern uint8_t disp_set_hours_time_f;
extern uint8_t  key_input_temp_f;


extern uint8_t heat_open_close_f;



//wifi ref


extern  uint8_t  wifi_app_timer_power_on_f;
extern bool  works_interval_f;

extern uint8_t  soft_version ;


//wifi end 







void Clear_Ram(void);

void Countdown_timer_Handler(void);

void works_two_hours_handler(void);

void beep_power_sound(void);

void power_on_off_handler(void);

void Heat_Process(void);




#ifdef __cplusplus
}
#endif

#endif /* __USER_H */
