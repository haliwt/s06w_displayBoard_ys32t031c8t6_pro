#ifndef __BSP_KEY_APP_H
#define __BSP_KEY_APP_H
#include "ys32t031.h"
#include "main.h"

typedef enum {
    TIME_MODE_NORMAL = 0,   // 显示正常时间
    TIME_MODE_TIMER  = 1    // 显示定时
} time_display_mode_t;





typedef struct{

  uint8_t key_wifi_flag;
  uint8_t key_power_flag;
  uint8_t key_mode_flag;
  uint8_t key_dec_flag;
  uint8_t key_add_flag;
  uint8_t key_plasma_flag;
  uint8_t key_dry_flag;
  uint8_t key_mouse_flag;
  uint8_t disp_smg_mode_flag;


}KEY_T_TYPEDEF;

extern KEY_T_TYPEDEF key_t;

void process_keys(void) ;




void mode_key_handler(void);

//void wifi_mode_key_handler(void);

void handle_mode_key_long_press(void);


void power_key_handler(void) ;

void mode_key_handler(void);


void handle_mode_key_long_press(void);

void key_dec_fun(void);

void key_add_fun(void);


void mouse_key_handler(void) ;

void dry_key_handler(void) ;

void plasma_key_handler(void) ;

void power_key_handler(void) ;

void direct_temperature_compraison_handler(void);


void mode_key_short_fun(void);




#endif 
