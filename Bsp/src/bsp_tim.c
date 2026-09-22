#include "bsp.h"


void tim6_isr_hander(void)
{

  volatile static uint8_t cnt10 =0,cnt100 =0,cnt1000,cnt1m=0,cnt20ms=0;

  volatile static uint8_t c100ms;
	
	

		time_5ms_f = 1;
		cnt10++;  

		if(cnt10 > 1){//5ms*2 =10ms

			cnt10 =0; 
		   time_10ms_f = 1;
		
			gpro_t.time_50ms_f++;

			if(++cnt100 >=10){ //10* 10 = 100ms .
				cnt100 =0;
				
			    wifi_fast_led_state();
	            
                
			

				if(++cnt1000> 9){ // 100ms *10 =1000ms=1s 
					cnt1000 = 0;
					time_1s_counter ++ ;

				    gpro_t.time_1s_f =1;
				
					time_link_net_counter++;
					gpro_t.gTimer_disp_mode_switch++;

					disp_switch_temp_humi++;
					time_set_hours_counter++;
					setting_timing_second ++;
					time_autolink_counter++;
					fan_one_minute_cuonter++;
					key_net_config_time++;
					gpro_t.gTimer_wifi_connect_counter++;
					gpro_t.gTimer_timer_seconds_counter++;
					gpro_t.gTimer_timing_seconds_counter++;
					gpro_t.gTimer_time_colon ++;
				
					gpro_t.time_2s_f++;
					gpro_t.time_3s_f++;
					gpro_t.time_4s_f++;
					gpro_t.time_5s_f++;
					gpro_t.time_6s_f++;
				    gpro_t.time_7s_f++;
					
				     if(++gpro_t.time_base_1s_counter > 59){//1s *60 =60s 
					     gpro_t.time_base_1s_counter = 0;
					    
						gpro_t.time_1m_f++;
						gpro_t.time_1m_wifi_f++;
						gpro_t.time_2m_f++;
						
						
						
					}

				} 

			}

		}
}




