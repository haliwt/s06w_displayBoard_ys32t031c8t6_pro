#include "bsp.h"


/**
  * @brief	:  
  * @note	: timer 17 is 10ms .
  * @param	 None
  * @retval  None
**/
void tim17_10ms_tick_handler(void)
{

  volatile static uint8_t cnt100 =0;

  volatile static uint8_t c100ms;
       

	cnt100++ ;

	if(cnt100 >99){ //10ms* 100 = 1000ms = 1s
		cnt100 =0;


       
            gpro_t.gTimer_counter++;
			
            gpro_t.gTimer_set_timer_counter++;
	        gpro_t.gTimer_water_led_blink++;
			
			time_t.g_time_seconds++;

     } 

	

}






