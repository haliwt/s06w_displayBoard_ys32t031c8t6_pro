#include "bsp.h"


void tim6_isr_hander(void)
{

  volatile static uint8_t cnt10 =0,cnt100 =0,cnt1000,cnt1m=0,cnt20ms=0;

  volatile static uint8_t c100ms;
	
	    cnt10++;  

		if(cnt10 > 0){//10ms*1=10ms

			cnt10 =0; 
		    gpro_t.time_10ms_f = 1;
		
			gpro_t.time_50ms_f++;

			if(++cnt100 >=10){ //10* 10 = 100ms .
				cnt100 =0;
				
			    wifi_fast_led_state();
	            if(++cnt1000> 9){ // 100ms *10 =1000ms=1s 
					cnt1000 = 0;
	

				
					time_link_net_counter++;
					gpro_t.gTimer_disp_mode_switch++;

				
	
					gpro_t.gTimer_wifi_connect_counter++;
					
			
					gpro_t.gTimer_time_colon ++;

					if(++gpro_t.gTimer_timer_seconds_counter >59){
						 gpro_t.gTimer_timer_seconds_counter=0;
                         gpro_t.timer_one_minute_flag = true;
                    }
				
				     if(++ gpro_t.gTimer_timing_seconds_counter> 59){//1s *60 =60s 
					    gpro_t.gTimer_timing_seconds_counter = 0;
						gpro_t.one_minute_flag=true;
						gpro_t.works_dispTime_minutes++;
					   
					    if(gpro_t.works_dispTime_minutes > 59){
                            gpro_t.works_dispTime_minutes =0;
                            gpro_t.works_dispTime_hours ++ ;
						    if(gpro_t.works_dispTime_hours >99)gpro_t.works_dispTime_hours =0;
						}
				     }

				} 

			}

		}
}




