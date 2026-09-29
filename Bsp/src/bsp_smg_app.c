#include "bsp.h"







/******************************************************************************
*
*Function Name:static void Setup_Timer_Times(void)
*Funcion:display setup timer times  //__asm("NOP");//等待1个指令周期，系统主频24M
*Iinput Ref:NO
*Return Ref:NO
*
******************************************************************************/
void Display_SmgTiming_Value(void)
{
    static uint8_t switch_f;

   if(ptc_high_temperature_f == 1 || fan_warning_f ==1){

        if(ptc_high_temperature_f  ==1  && fan_warning_f ==0){
                 
				 SMG_Display_Err(0);
				
		}
		else  if(ptc_high_temperature_f  ==0 && fan_warning_f ==1){


                SMG_Display_Err(1);

		 }
		 else  if(ptc_high_temperature_f == 1 && fan_warning_f ==1){

				 switch_f = switch_f ^ 0x01;

		         if(switch_f ==1){
				    SMG_Display_Err(0);


				 }
				 else{

				    SMG_Display_Err(1);

				 }
		}
       return ;
   }


    switch(gpro_t.ui_time_mode){

	   case TIME_MODE_TIMER:
		

		if(gpro_t.timer_one_minute_flag == true){
			gpro_t.timer_one_minute_flag = false;

			gpro_t.timer_dispTime_minutes -- ;

			if(gpro_t.timer_dispTime_minutes <  0 ){

				gpro_t.timer_dispTime_hours -- ;
				gpro_t.timer_dispTime_minutes =59;

				
			}



			if(gpro_t.timer_dispTime_hours < 0 ){

				gpro_t.gTimer_timer_seconds_counter = 57 ;
				gpro_t.timer_dispTime_hours=0;
				gpro_t.timer_dispTime_minutes=0;

				//gpro_t.send_ack_cmd = check_ack_power_off;//ack_power_off;
			
				SendData_Set_Command(0X01,0);//SendData_PowerOnOff(0);//power off
                tx_thread_sleep(2);
			}

			//dataToSend[3] = {run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes, run_t.gTimer_timer_seconds_counter}; // 要发送的 3 个数据
			sendData_to_threeData(0x6B,gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes, gpro_t.gTimer_timer_seconds_counter); // cmd=0x1A, 数据长度=3
			tx_thread_sleep(2);
			Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);
		}

		
		
			
        
	    break;

		case TIME_MODE_NORMAL: //NO_AI_MODE by timer timing  auto be changed AI_MODE
			
          if(gpro_t.one_minute_flag==true){
    		 gpro_t.one_minute_flag = false;
    		 
    		 if(gpro_t.connect_wifi_state == 0){
				//dataToSend[3] = {run_t.works_dispTime_hours,run_t.works_dispTime_minutes, run_t.gTimer_timing_seconds_counter}; // 要发送的 3 个数据
				sendData_to_threeData(0x6C,gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes, gpro_t.gTimer_timing_seconds_counter); // cmd=0x1A, 数据长度=3
				tx_thread_sleep(2);
                }
			   Display_Timing(gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes,0);
           }
           
           Timer_Timing_Donot_Display();
     
			break;

	   	}

}

/****************************************************************
* 
* Function Name: static void Timer_Timing_Donot_Display(void)
* Function :function of pointer 
* Input Ref:NO
*
* 
*****************************************************************/
static void Timer_Timing_Donot_Display(void)
{
	
	if(gpro_t.timer_one_minute_flag == true  && gpro_t.set_timer_timing_value_success== TIME_MODE_TIMER){
	     gpro_t.timer_one_minute_flag = false;
	    gpro_t.timer_dispTime_minutes -- ;

	   if(gpro_t.timer_dispTime_minutes <  0 ){

		  gpro_t.timer_dispTime_hours -- ;
		  gpro_t.timer_dispTime_minutes =59;
	  }



	  if(gpro_t.timer_dispTime_hours <0){ 
		  gpro_t.gTimer_timer_seconds_counter = 57 ;
		  gpro_t.timer_dispTime_hours=0;
		  gpro_t.timer_dispTime_minutes=0;
		  SendData_Set_Command(0x01,0); // power off
          tx_thread_sleep(2);
		  gpro_t.off_step=0;
		  gpro_t.g_power_flag = false;

	   }
	
		sendData_to_threeData(0x6B,gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes, gpro_t.gTimer_timer_seconds_counter); // cmd=0x1A, 数据长度=3
		tx_thread_sleep(2); 	
	}

}

/********************************************************************************
*
*Functin Name: void Display_Error_Digital(uint8_t errnumbers,uint8_t sel)
*Function : Timer of key be pressed handle
*Input Ref:  error digital 
*Return Ref: NO
*
********************************************************************************/
/********************************************************************************
*
*Functin Name: void Display_TimeColon_Blink_Fun(void)
*Function : 
*Input Ref:  NO
*Return Ref: NO
*
********************************************************************************/
void Display_TimeColon_Blink_Fun(void)
{
	static uint8_t timer_f = 0;

	if(gpro_t.set_timer_timing_doing_value==1){

		if(timer_f ==0){
			timer_f ++;

			SmgBlink_Colon_Function(gpro_t.hours_two_unit_bit,gpro_t.minutes_one_decade_bit,1);
		}
		return ;

	}





	if(timer_f !=0)timer_f = 0;

	gpro_t.g_time_disp_colon_flag = !gpro_t.g_time_disp_colon_flag ;

	SmgBlink_Colon_Function(gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes,gpro_t.g_time_disp_colon_flag);

	
}





