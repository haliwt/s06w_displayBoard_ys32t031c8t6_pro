#include "bsp.h"


void all_led_off(void)
{

	LED_DRY_OFF();
	LED_PLASMA_OFF();
	LED_MOUSE_OFF();
	LED_WIFI_OFF();
	LED_TIME_OFF();



}

void power_on_led_open_handler(void)
{
	if(wifi_app_timer_power_on_f==0){

	     
		 LED_DRY_ON();
		 LED_PLASMA_ON();
		 LED_MOUSE_ON();
		 LED_WIFI_ON();
		 LED_POWER_ON();
		 LED_TIME_ON();
		
	
		gpro_t.g_dry_flag = 1;        // 默认开启加热
	    gpro_t.g_mouse_flag = 1;     // 默认开启超声波
	    gpro_t.g_plasma_flag = 1;          // 默认开启等离子
	    LED_DRY_ON();


	  }
	  else{
	
		 LED_WIFI_ON();
		 LED_POWER_ON();
	
		


	  }

	  if(gpro_t.connect_wifi_state == false){
	  gpro_t.hours_two_decade_bit=9;
	  gpro_t.hours_two_unit_bit =9;
	  gpro_t.minutes_one_decade_bit = 7;

	 
	 // TM1639_Write_4Bit_Time(gpro_t.hours_two_decade_bit,gpro_t.hours_two_unit_bit,9,6,0);
	  Display_Timing(13,36,0);
	  tx_thread_sleep(10);
	  disp_dht11_value();
	  tx_thread_sleep(10);
     //Display_DHT11_Value(); //WT.EIDT 2025.05.10
     }
	 else{

        Display_Timing(gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes,0);
		
	 }
    

}
//300ms
void wifi_fast_led_state(void)
{
   static uint8_t slowly_led_counter = 0;//100ms
   if((gpro_t.g_power_flag ==1) && (key_net_config_f ==1)){
	    LED_WIFI_TOGGLE();
		
   }
   else if((gpro_t.g_power_flag ==1) && (key_net_config_f ==0)){

      
		if(++slowly_led_counter > 9){//100ms *10 =1000ms =1s 

		    slowly_led_counter =0;
		     LED_WIFI_TOGGLE();
		}
   }
   else if(gpro_t.g_power_flag ==0){
	     
	   if(++slowly_led_counter > 9){//100ms *10 =1000ms =1s
	     slowly_led_counter=0;
        LED_POWER_TOGGLE();

      }
   }
  
}


void wifi_led_state_handler(void)
{
	
     if(key_net_config_f==1) return ;
	
	}




