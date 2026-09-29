#include "bsp.h"

/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/

void all_led_off(void)
{

	LED_DRY_OFF();
	LED_PLASMA_OFF();
	LED_MOUSE_OFF();
	LED_WIFI_OFF();
	LED_TIME_OFF();



}
/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/
void power_on_led_open_handler(void)
{
	if(wifi_app_timer_power_on_f==0){

	     
		 LED_DRY_ON();
		 LED_PLASMA_ON();
		 LED_MOUSE_ON();
		 LED_WIFI_ON();
		 LED_POWER_ON();
		 LED_TIME_ON();
		
	
		gpro_t.g_dry_flag = true;        // 默认开启加热
	    gpro_t.g_mouse_flag = true;     // 默认开启超声波
	    gpro_t.g_plasma_flag = true;          // 默认开启等离子
	    LED_DRY_ON();


	  }
	  else{
	     gpro_t.connect_wifi_state = true;
		 LED_WIFI_ON();
		 LED_POWER_ON();
	   }

       disp_dht11_value();
	   tx_thread_sleep(20);

       Display_Timing(gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes,0);
	   tx_thread_sleep(20);
	 
}
/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/
void wifi_fast_led_state(void)
{
   static uint8_t slowly_led_counter = 0;//100ms
   if((gpro_t.g_power_flag ==true) && (gpro_t.connecting_wifi_flag ==true)){
	    LED_WIFI_TOGGLE();

        if(gpro_t.gTimer_wifi_connect_counter > 130 || gpro_t.connect_wifi_state==true ){
		   gpro_t.connecting_wifi_flag = false;
        }
		
   }
   else if((gpro_t.g_power_flag ==true) && (gpro_t.connecting_wifi_flag == false) && gpro_t.connect_wifi_state==false){

      
		if(++slowly_led_counter > 9){//100ms *10 =100ms =1s 

		    slowly_led_counter =0;
		     LED_WIFI_TOGGLE();
		}
   }
   else if(gpro_t.connect_wifi_state==true && gpro_t.g_power_flag ==true){

       LED_WIFI_ON();


   }
   else if(gpro_t.g_power_flag == false){
	 LED_WIFI_OFF();
      
   }
  
}





