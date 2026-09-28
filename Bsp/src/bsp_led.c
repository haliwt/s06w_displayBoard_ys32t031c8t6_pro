#include "bsp.h"




/**
*@brief:  
*@param:
*@notice
**/
void power_on_led_handler(void)
{
   LED_POWER_ON();
   LED_FAN_ON();
   LED_PLASMA_ON();
   LED_KEY_AI_ON();
   LED_WATER_INDICATE_ON();
   if(gpro_t.water_pos_warning_flag == 1){
       LED_WATER_FULL_ON();
   }
   else LED_WATER_FULL_OFF();
  
}


/**
*@brief:  
*@param:
*@notice
**/
void power_off_led_handler(void)
{
	 LED_POWER_OFF();
	 LED_FAN_OFF();
	 LED_PLASMA_OFF();
	 LED_KEY_AI_OFF();
   //water pos led 
      LED_WATER_INDICATE_OFF();
      LED_WATER_FULL_OFF();
  
 
}

/**
*@brief:  
*@param:
*@notice
**/
void water_full_led_blink(void)
{
   if(gpro_t.water_pos_warning_flag == true){

        if(gpro_t.gTimer_water_led_blink > 0){
		    gpro_t.gTimer_water_led_blink =0;
			LED_WATER_FULL_TOGGLE();


		}

   }
   else{
     LED_WATER_FULL_OFF();

   }
}
/**
*@brief:  
*@param:
*@notice
**/



/**
*@brief:  water of detected display led
*@param:
*@notice
**/





