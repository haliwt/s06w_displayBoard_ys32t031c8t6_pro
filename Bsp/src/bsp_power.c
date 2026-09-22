/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.C
  * @author  Yspring Firmware Team  
  * @brief   user Source Code.
  ******************************************************************************      
*/

#include "bsp.h"


#define THREADX_TICK_MS 10


#define MS_TO_TICKS(ms)  ((ms) / THREADX_TICK_MS)

#define TASK_NUM (sizeof(g_ui_tasks) / sizeof(task_t))

power_state gon_t;



typedef void (*task_handler_t)(void);

typedef struct {
    task_handler_t task_handler; // 任务回调函数
    uint32_t period;              // 运行周期 (Ticks)
    uint32_t last_tick;           // 上次运行时间 (Ticks)
} task_t;



// 任务函数前置声明
static void task_ui_key(void);
static void task_keys_and_refresh(void);
static void task_dht11_display(void);
static void task_two_hours_timing(void);
static void task_send_version(void);
static void task_blink_colon(void);
static void task_compare_temp(void);

// 任务配置表 (Table-Driven)
static task_t g_ui_tasks[] = {
    { task_ui_key,             1,   0 }, // 1*10m  
    { task_keys_and_refresh,   5,   0 }, // 5*10ms 刷新UI和按键
    { task_blink_colon,        50,  0 }, // 50*10ms 冒号闪烁
    { task_dht11_display,      30,  0 }, // 300ms DHT11刷新
    { task_compare_temp,       300, 0 }, // 3s 控温比较
    { task_two_hours_timing,   120, 0 }, // 1.2s 运行计时
    { task_send_version,       3000, 0 }, // 3s  发送版本号
};



typedef enum{

  PTC_STATE_OFF = 0,
  PTC_STATE_ON  = 1
}PTC_State;

static PTC_State ptc_state = PTC_STATE_OFF;



static void Set_TimerTiming_Number_Value(void);







volatile uint8_t Times5msCnt;
uint8_t Times10msCnt;
//uint8_t Times100msCnt;
uint8_t Times1minute;
uint16_t Times1minCnt;
uint8_t Cacl_time_sec;

volatile uint8_t time_5ms_f;

uint8_t time_wifi_10ms_f;

uint16_t fan_adc_value[1];
uint16_t ad_ptc_value[1];
uint16_t fan_current;
uint16_t ptc_current;



uint16_t current_temperature;
uint16_t setting_temperature;
uint16_t disp_temperature;
uint16_t disp_timing_time;
uint16_t disp_humidity;

uint8_t AI_led_open_f;
uint8_t PTC_heat_open_f;
uint8_t first_temp_compare_f;

uint8_t ptc_prohibit_off_f;

uint8_t Ultra_Sound_open_f;
uint8_t plasma_open_f;

uint16_t timing_is_reach_disptime;

uint8_t read_ntc_temperature_value;




uint8_t Is_time_setting_f;

uint8_t Is_countdown_timer_f;
uint8_t set_temperature_value_f;
uint8_t time_1s_counter;

//display second board
uint8_t disp_second_f;
uint8_t heat_open_close_f;
uint8_t  key_pressed_set_temp_f;





uint8_t key_net_config_f;
uint16_t key_net_config_time;


uint8_t flash_f;


uint16_t device_rest_time;

//countdown timer 
int8_t timing_min_cnt;
int8_t setting_timing_second;
uint8_t real_hours_counter;
int8_t temporary_timer_hours;
int8_t setting_timing_hour;

//end
//timer 
uint8_t  time_set_hours_counter;

//wroks time two hours

uint8_t  works_interval_f;



uint8_t fan_open_f;
uint8_t fan_speed_level;


volatile uint8_t beep_times;				//����
volatile uint8_t beep_lenght;			  //��ĳ��� *100ms
volatile uint8_t non_beep_length;		//���ʱ��
uint16_t beep_interval_time;

//temp ref
uint8_t temperature;
uint8_t humidity;

uint8_t  soft_version ;



//peripheral ref


uint16_t fan_current_det_time;
uint8_t fan_warning_f;

uint8_t disp_switch_temp_humi;
//
uint8_t soft_version;

//wifi 

uint8_t  link_net_step;
uint8_t  time_link_net_counter;
uint8_t  wifi_linking_tencent_f;
uint8_t  wifi_connected_success_f;
volatile uint8_t  wifi_rx_numbers;
uint8_t  wifi_cofig_success_f;
uint8_t  wifi_app_timer_power_on_f;
uint8_t  wifi_run_step ;
uint8_t  wifi_off_step;

uint8_t  wifi_first_connectoed_cloud_f;
uint8_t  wifi_read_net_data_f;

uint8_t  time_autolink_counter;
uint8_t  wifi_check_net_f;
uint8_t dc_connect_net_step	;

volatile uint8_t  rx_wifi_data_success;
volatile uint8_t   rx_wifi_data_counter;
uint8_t  mqtt_status;


//fan
uint8_t  fan_one_minute_cuonter;
uint8_t  time_10ms_f;
uint16_t ptc_adc_numbers;





uint8_t key_be_pressed_f;
uint8_t disp_set_hours_time_f;
uint8_t key_input_temp_f;

uint8_t ptc_high_temperature_f ;











uint8_t counter;
uint8_t power_Led_switch;	

volatile uint16_t i;
volatile uint16_t bw_i=0;
volatile uint16_t sw_i=0;
volatile uint16_t gw_i=0;
volatile uint16_t disp_timing_time_temp;
volatile uint16_t timing_diff_value_hour;
volatile uint16_t timing_diff_value_min;


volatile uint8_t static beep_sound_f =0;
static void power_on_handler(void);
static void power_off_handler(void);
static void power_on_initial(void);
static void set_temperature_compare_value_fun(void);

/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/
void Clear_Ram(void)
{
    time_5ms_f = 0;
	

	  gpro_t.time_400ms_f =0;
	  gpro_t.time_500ms_f =0;
	  gpro_t.time_1s_f = 0;
	  gpro_t.time_1m_f=0;
	

	  Times10msCnt = 0;

	  Times1minute = 0;
	  Times1minCnt = 0;
	  Cacl_time_sec = 0;
	
	
	
	  key_worked_f = 0;
	 
	  key_data = 0;
	  key_time = 0;
	
	  gpro_t.g_power_flag = 0;
		
		
		device_rest_time = 0;
		
		fan_speed_level = 100;
		fan_open_f = 0;
	
		
		AI_led_open_f = 0;
		PTC_heat_open_f = 0;
		first_temp_compare_f=0;
		Ultra_Sound_open_f = 0;
		plasma_open_f = 0;

		
		timing_is_reach_disptime = 0;
		
		Is_time_setting_f = 0;
	
		Is_countdown_timer_f = 0;
		
	
		flash_f = 0;
	
		
		timing_min_cnt = 0;
		
		fan_warning_f = 0;
		fan_current_det_time = 0;
		
		disp_switch_temp_humi = 0;
		beep_interval_time = 0;
		//wifi 
		wifi_linking_tencent_f=0;
		
	}

/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/

/************************************************************************
 * Function Name: LED_Power_Breathing(void)
 * 功能:
 * 参数:无
 * 返回值:无
 ************************************************************************/
static void power_on_initial(void)
{

   uint32_t current_tick = tx_time_get();
   uint8_t i ;
   uint32_t init_tick;
   
   switch(gon_t.on_step){

   case 0:
   	  gon_t.off_step = 0;
      wifi_off_step =0; //WT.EDT 2026.05.15
      
 

	 
	
      gon_t.on_step =1;
	

   break;

   case 1:
  
    gon_t.on_step =2;


   break;

   case 2:
   	 
		// 2. 初始化所有任务的 last_tick 镜像
	    init_tick = tx_time_get();
	    for (i = 0; i < TASK_NUM; i++) {
	        g_ui_tasks[i].last_tick = init_tick;
	    }
	 
	   gon_t.on_step =0xfe;

   break;

   	}
}
/************************************************************************
*
* Function Name: LED_Power_Breathing(void)
* 功能:
* 参数:无
* 返回值:无
*
************************************************************************/
uint16_t disp_counter;

void power_on_handler(void)
{

    uint32_t current_tick = tx_time_get();
	uint8_t i ;
	uint32_t init_tick;

	
        if(gon_t.on_step  < 8){
		  power_on_initial();
        }
		else{
	 // ✨【新增：紧急事件拦截响应】✨
        // 如果按键任务设置完温度，将 g_pro.g_immediate_heat_f 置为 1
       
//         if(time_10ms_f ==1 &&  ptc_high_temperature_f == 0 && fan_warning_f ==0){
//		    time_10ms_f=0;
           
		

//			if(heat_open_close_f == 1 && ptc_high_temperature_f == 0 && fan_warning_f ==0)
//	        {
//	           heat_open_close_f = 0; // 立即清除触发标志，防止重复执行
	            
//	            // 强制、立刻执行一次加热控制函数
//	            // 确保底层硬件（如继电器、PWM、PTC）在 20ms 内得到响应
//	          // compare_set_temp_value(); //set_temperature_value_handler(); 
//	        }
			    

//         }
//		 else{

	        for (i = 0; i < TASK_NUM; i++) {
			if ((current_tick - g_ui_tasks[i].last_tick) >= g_ui_tasks[i].period){
				// 防饱和截断：若卡顿超过 2 个周期，直接重置到当前 tick，放弃追赶
				if ((current_tick - g_ui_tasks[i].last_tick) > (g_ui_tasks[i].period * 2)) 
				{
					g_ui_tasks[i].last_tick = current_tick;
				} 
				else 
				{
					// 锁相滚动累加，消除长期运行漂移
					g_ui_tasks[i].last_tick += g_ui_tasks[i].period;
				}

				// 执行任务回调
				if (g_ui_tasks[i].task_handler != NULL) 
				{
					g_ui_tasks[i].task_handler();
				}
			}

		 }

     }
}
/**
*@brief 
*@param
*@notice
**/
static void task_ui_key(void)
{

  if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch < 3){

		 mode_key_short_fun();
		 return ;
   }

   if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch > 2){

	  gpro_t.key_model_short_flag  =0;

   }

   if (gpro_t.set_timer_timing_doing_value == 1 &&	ptc_high_temperature_f == 0 &&  fan_warning_f == 0) {

		Set_TimerTiming_Number_Value();

		return ;
	}
	
	// disp_smg_blink_set_tempeature_value();

}


/**
*@brief 
*@param
*@notice
**/
static void task_keys_and_refresh(void)
{
	
	// 1. 有告警时优先显示告警
		if (ptc_high_temperature_f || fan_warning_f) {
			
			return;
		}
	
	
		// 4. 正常显示工作时间（你原来的 Display_SmgTiming_Value）
		if (gpro_t.set_timer_timing_doing_value == 0){
				 
			if(gpro_t.key_model_short_flag == 1) return;
			
			Display_SmgTiming_Value();
			return;
		}

}
/**
*@brief 
*@param
*@notice
**/
static void task_dht11_display(void)
{
	disp_dht11_value();

}
/**
*@brief 
*@param
*@notice
**/
static void task_two_hours_timing(void)
{
	 

}
/**
*@brief 
*@param
*@notice
**/
static void task_send_version(void)
{
	 //SendData_Set_Command_Safe(0xF0,0x02);//
	 SendData_Set_Command(0xF0,0x02);
	 
}
/**
*@brief 
*@param
*@notice
**/
static void task_blink_colon(void)
{
	Display_TimeColon_Blink_Fun();
	if(gpro_t.wifi_led_fast_blink==1 && gpro_t.connect_wifi_state == false && gpro_t.gTimer_wifi_connect_counter > 125 ){
		gpro_t.wifi_led_fast_blink=0;

	}
	if(gpro_t.wifi_led_fast_blink==1 && gpro_t.connect_wifi_state == true){

		gpro_t.wifi_led_fast_blink=0;

	}
}
/**
*@brief 
*@param
*@notice
**/
static void task_compare_temp(void)
{
	set_temperature_compare_value_fun();

}
/************************************************************************************************
*
*Function Name:void set_temperature_compare_value_fun(void)
*Function:
*Input Ref:
*Return Ref:
*
*************************************************************************************************/
static void set_temperature_compare_value_fun(void)
{
   // static uint8_t counter;
	uint8_t target_temp,real_temp;

	if(fan_warning_f ==1 || ptc_high_temperature_f ==1 || gpro_t.g_manual_shutoff_dry_flag == 1\
		|| gpro_t.set_temperature_special_flag ==1)return ;


	real_temp = gpro_t.dht11_temperature_value;//gpro_t.temp_real_value;
	target_temp = gpro_t.set_up_temperature_value;//gpro_t.key_set_temperature;
   

	

	if(real_temp >= target_temp){

		   gpro_t.g_dry_flag = 0;
		   LED_DRY_OFF();
		   
		  ptc_state = PTC_STATE_OFF ;
		  gpro_t.first_set_ptc_on  = 1;
		  SendData_Set_Command(0x22,0);
		  tx_thread_sleep(2);
  
		  return ;
	}

	if(ptc_state == PTC_STATE_OFF){

		if(gpro_t.first_ptc_on==0 || gpro_t.first_ptc_on==1){

			if(real_temp < target_temp){

			   if(gpro_t.g_manual_shutoff_dry_flag==0){
				   gpro_t.g_dry_flag = 1;
					LED_DRY_ON();
				   ptc_state = PTC_STATE_ON ;
				   if(gpro_t.first_ptc_on==1)gpro_t.first_set_ptc_on  = 2;
				   
				   SendData_Set_Command(0x22,1);
				   tx_thread_sleep(2);
				}
			}
		}
		else{
			if(real_temp < (target_temp -2)){

			  if(gpro_t.g_manual_shutoff_dry_flag==0){

				gpro_t.g_dry_flag = 1;
				LED_DRY_ON();
		   
				ptc_state = PTC_STATE_ON ;
			   SendData_Set_Command(0x22,1);
			   tx_thread_sleep(2);
				}

			}


		}

	}
	else{
		if(real_temp >= target_temp){
			gpro_t.g_dry_flag = 0;
			 LED_DRY_OFF();
			ptc_state = PTC_STATE_OFF ;
		   SendData_Set_Command(0x22,0);
		   tx_thread_sleep(2);
		}

	}


	
}

/****************************************************************
*
*Function Name :void Set_Timing_Temperature_Number_Value(void)
*Function : set timer timing how many ?
*Input Parameters :NO
*Retrurn Parameter :NO
*
*****************************************************************/
void Set_TimerTiming_Number_Value(void)
{

	// switch(gpro_t.set_timer_first_smg_blink_flag)
	if(gpro_t.set_timer_first_smg_blink_flag ==1){
	gpro_t.set_timer_first_smg_blink_flag++;

	//以前已经设置过定时模式,现在显示之前的定时时间
	if(gpro_t.set_timer_timing_value_success  == TIME_MODE_TIMER && gpro_t.key_add_dec_pressed_flag ==0){
		gpro_t.hours_two_decade_bit = gpro_t.timer_dispTime_hours/10,
		gpro_t.hours_two_unit_bit  = gpro_t.timer_dispTime_hours %10;


		Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);//don't display numbers

	}
	else{

		gpro_t.hours_two_decade_bit = 0;//gpro_t.timer_dispTime_hours/10,
		gpro_t.hours_two_unit_bit  = 0;//gpro_t.timer_dispTime_hours %10;

		gpro_t.timer_dispTime_hours =0;
		gpro_t.timer_dispTime_minutes =0;

		Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);//don't display numbers
	}


	}
	else if(gpro_t.set_timer_first_smg_blink_flag==2 && gpro_t.gTimer_key_timing > 2){


		if( gpro_t.timer_dispTime_hours >0 && gpro_t.key_add_dec_pressed_flag ==1){ //set up timer numbers value 
		gpro_t.set_timer_timing_value_success  = TIME_MODE_TIMER;//disp_timer_times;
		//key_t.disp_smg_mode_flag = disp_timer_times;
		gpro_t.gTimer_timer_seconds_counter = 0;




		Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);


		gpro_t.set_timer_first_smg_blink_flag ++;
		gpro_t.set_timer_timing_doing_value =0;


		//SendData_Tx_Data(0x2B, gpro_t.timer_dispTime_hours) ;
		SendData_Set_Command(0x2B, gpro_t.timer_dispTime_hours);
		tx_thread_sleep(2);


	}
	else if(gpro_t.timer_dispTime_hours	== 0 && gpro_t.key_add_dec_pressed_flag ==1){ //set up timer numbers value 
		gpro_t.set_timer_timing_value_success  = TIME_MODE_TIMER; //disp_works_times;
		gpro_t.ui_time_mode = TIME_MODE_TIMER;//key_t.disp_smg_mode_flag = disp_works_times;
		gpro_t.gTimer_timer_seconds_counter = 0;



		Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);

		gpro_t.set_timer_first_smg_blink_flag ++;
		gpro_t.set_timer_timing_doing_value =0;


		//SendData_Tx_Data(0x2B, gpro_t.timer_dispTime_hours) ;
		SendData_Set_Command(0x2B, gpro_t.timer_dispTime_hours);
		tx_thread_sleep(2);


	}
	else{

		gpro_t.timer_dispTime_hours = 0 ;
		gpro_t.timer_dispTime_minutes = 0;

		Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);

		gpro_t.set_timer_first_smg_blink_flag ++;
		gpro_t.set_timer_timing_doing_value =0;


		gpro_t.set_timer_timing_value_success  = 0;
		key_t.disp_smg_mode_flag = TIME_MODE_TIMER; //disp_works_times;


	}

	}
}

#if 0

		 
	     

		switch(time_slot){

		case 0://1* 20ms
		     per_counter++;
		     if(per_counter > 40 &&  ptc_high_temperature_f == 0 && fan_warning_f ==0){ //10ms * 100
			 	per_counter =0;
		       //peripheral_fun_handler();
		     }

			
		break;



		case 1:
			 disp_counter ++;
			 if(disp_counter > 30 && ptc_high_temperature_f == 0 && fan_warning_f ==0 ){
			 disp_counter=0;	
			//  display_temperature_humidigy_handler();

			 }
			  

		break;

		case 2://2*20m = 40
		  if(gpro_t.time_3s_f > 3 && ptc_high_temperature_f == 0 && fan_warning_f ==0){
		    gpro_t.time_3s_f =0;	
		 

           }

		break;
		
		case 3:
		 if(wifi_connected_success_f==1 && gpro_t.time_4s_f > 0 && ptc_high_temperature_f == 0 && fan_warning_f ==0){
	  	   gpro_t.time_4s_f=0;
		   //wifi_power_on_handler();
         }
		
		break;

		
		case 4:
			if(ptc_high_temperature_f == 0 && fan_warning_f ==0){
				if(key_net_config_f)
				 {
					
					if(key_net_config_time>=130)
					{
						key_net_config_time = 0;

						key_net_config_f = 0;
						
					}
					else{ //conneting to wifi net 
				        
					
					}
				 } 
		  }
				
		break;

		
		case 5:
		if(gpro_t.time_5s_f > 1){
	   	  gpro_t.time_5s_f=0;
           Heat_Process(); //
	      }
				
		break;


		case 6:

		 if(gpro_t.time_6s_f > 2 && ptc_high_temperature_f == 0 && fan_warning_f ==0){
		   gpro_t.time_6s_f =0;
      	  
   	      }

		break;


		case 7:

		   if(Is_countdown_timer_f ==1 && ptc_high_temperature_f == 0 && fan_warning_f ==0){
             Countdown_timer_Handler();
	   	    }

		break;


		case 8:
			 if( ptc_high_temperature_f == 0 && fan_warning_f ==0){
			      works_nomal_run_time_handler();
			 }

		break;


		case 9:
	       if(gpro_t.time_7s_f > 4 && ptc_high_temperature_f == 0 && fan_warning_f ==0 && works_interval_f==0){

		    gpro_t.time_7s_f =0 ;
			fan_counter =1;
		


		 
	       }

		break;

		case 10:
			if(ptc_high_temperature_f == 0 && fan_warning_f ==0){
			 if(key_net_config_f==0 &&  wifi_linking_tencent_f ==0 && gpro_t.time_1m_wifi_f > 1){
	   	   gpro_t.time_1m_wifi_f =0;
		   #if DEBUG_ENABLE
		     printf("reconnection wifi ! \n\r");
		   #endif 
		 

	 		}
			}

		break;

		case 11:
           
			wifi_check_counter++; //20ms * 100
		    if(wifi_check_counter > 300 && ptc_high_temperature_f == 0 && fan_warning_f ==0){
			  wifi_check_counter =0;
                
		    }

		break;

		case 12:

		   ptc_counter++ ;
		   if(ptc_counter > 50 && ptc_high_temperature_f == 0){
		   	   ptc_counter =0;
			   switch_done=1;
		    
		   
             #if 0
			  printf_ptc_adc_numbers();
			 #endif 
			 
            }
		   

		break;

		 case 13:
		    if(switch_done==1){
				switch_done ++;
			
			
			
				 #if 0
						  printf("ntc_temp_v = %d \n\r",ptc_current);
						  printf("temperature = %d \n\r",read_ntc_temperature_value);
				 #endif 
						
			}

		break;

	    case 14:

		   
			
           if(switch_done==2){
		       switch_done++;

			if(read_ntc_temperature_value >120 && ptc_high_temperature_f == 0){

		       high_tmep_counter++;

		      if(high_tmep_counter > 2){

                  LED_DRY_OFF();
			 
		           ptc_high_temperature_f = 1;
		          // SMG_Display_Err(01);
			      // beep_high_temperature_sound();
                  
		       }
           }
		   else if(ptc_high_temperature_f == 0){
              high_tmep_counter =0;
		       read_ntc_temperature_value =0;

		   }

		   }
		   
               
		break;

		case 15:

		  has_warning_counter++;

         
		 if(has_warning_counter > 100){

		   has_warning_counter=0;
			
		  if(ptc_high_temperature_f == 1){
		  	  LED_DRY_OFF();

			  //SMG_Display_Err(01);
			 // beep_high_temperature_sound();
			 

		  }

		   if(fan_warning_f == 1){
			       fan_counter=0;
				   fan_error=0;
			        LED_DRY_OFF();
			 
					//SMG_Display_Err(02);
				
					
					
            }

		 }
		  
		   if(fan_counter ==1){
		   	  fan_counter ++; 
			  #if 0
				  printf("fan_current  = %d \n\r",fan_current );
				  printf("temperature = %d \n\r",read_ntc_temperature_value);
			  #endif 
           if(fan_current < 20  &&  fan_warning_f == 0 && works_interval_f==0){
		  	    
                 fan_error ++ ;
				 #if 0
				 
				  printf("fan_error= %d \n\r",fan_error);
			    #endif 
			     if(fan_error > 6){
				  fan_warning_f = 1;
				       LED_DRY_OFF();
					
					//	SMG_Display_Err(02);
						
						//beep_high_temperature_sound();
						
	            }
				 
			}
		    else if(fan_current  >19   &&  fan_warning_f == 0 && works_interval_f==0){

			   fan_error  =0;


			}

		 	}
		   
		 
		break;

		default:

		break;


		
       }

	 // ==================== 4. 时间片轮转维护 ====================
           time_slot++;
           if (time_slot >15 ) time_slot = 0;  //10ms* 16 = 160ms 

        
}
#endif 
/************************************************************************
 *
 * Function Name: LED_Power_Breathing(void)
 * 功能:
 * 参数:无
 * 返回值:无
 *
 ************************************************************************/
static void power_off_handler(void)
{
   static uint8_t dc_on=0,fan_one_f=0;

   static uint32_t wait_timeout = 0;

   if(tx_time_get() < wait_timeout){
       return ;
   }
	switch(gon_t.off_step){
	
		 case 0:
			gon_t.on_step =0;
	       
		    fan_one_f =1;
			time_1s_counter=0;
			fan_one_minute_cuonter =0;
			wifi_run_step = 0;
			wifi_off_step =0;
			//power_off_peripheral_handler();
			all_led_off();
	        TM1639_Display_ON_OFF(0);
			
			
			gon_t.off_step = 1;
	
		 break;
	
		 case 1:
             //power_off_peripheral_handler();
		  
           
			

			gon_t.off_step = 2;
			 
        break;


		case 2:
       

		break;

		case 3:

		  
			
			

				
		
	       gon_t.off_step = 4;
            	
		break;

		 case 4 :

		    
		   if(time_1s_counter > 1){
				 	time_1s_counter =0;
			
				  
			}

        
		    gon_t.off_step = 5;

		  break;

		  case 5:

		     if(time_1s_counter > 2){
				 	time_1s_counter =0;
				 
			}
		
		    gon_t.off_step =6;

		  break;

		  case 6:

		 

			
		
		  gon_t.off_step = 7;
		break;

		case 7:
			

		   
		    gon_t.off_step = 8;

		break;

		case 8:
			
		
          gon_t.off_step = 3;
		break;

   }
}

/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void Countdown_timer_Handler(void)
{
   static int8_t dsip_timer_value ;
 
   if(setting_timing_second >=60) //60s
    {
	   setting_timing_second=0;

	   #if DEBUG_ENABLE

		timing_min_cnt = timing_min_cnt - 40;
	   #else 
		 timing_min_cnt --;

	   #endif 

        if(timing_min_cnt <  0)
        {
           timing_min_cnt =59;
		   real_hours_counter++;
		   if((setting_timing_hour > 1) && setting_timing_hour !=1){
		   	
		          dsip_timer_value = temporary_timer_hours - real_hours_counter +1;
				  setting_timing_hour = dsip_timer_value;


		   }

		   if(setting_timing_hour ==1 || setting_timing_hour==0){
               
				   setting_timing_hour--;
				   
            }

        }

        if (setting_timing_hour < 0)
        {
             gpro_t.g_power_flag = 0;
			 System_Status_PowerOff() ;

        }
    }
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void works_nomal_run_time_handler(void)
{
     static uint8_t interval_10m_f = 0;
	 
		#if  0 //DEBUG_ENABLE 
			if(gpro_t.time_1m_f >11 && works_interval_f==0){
		#else 
			if(gpro_t.time_1m_f > 119 && works_interval_f==0){

		#endif 

			gpro_t.time_1m_f = 0;
		    gpro_t.time_base_1s_counter=0;
			works_interval_f=1;
			fan_one_minute_cuonter =0;
			
		#if DEBUG_ENABLE 
			printf("works_interval_f = %d \n\r",works_interval_f);
		#endif 
		}

		#if 0
		  else if(works_interval_f==1 && gpro_t.time_1m_f >9){
		#else 
		  else if(works_interval_f==1 && gpro_t.time_1m_f >10){

		#endif 
				gpro_t.time_1m_f = 0;  
				works_interval_f =0;
		        gpro_t.time_base_1s_counter=0;
				interval_10m_f = 1;
				
		#if DEBUG_ENABLE 
			printf("works_interval_f = %d \n\r",works_interval_f);
		#endif 
		}


		if(interval_10m_f == 1 && works_interval_f==0){
             interval_10m_f ++;
		
		  if(ptc_prohibit_off_f == 0 &&  PTC_heat_open_f == 1){
			 // 立即open
		      LED_DRY_ON();
		  
		  
		  	}
		 
		}
		
 }
  

/**
  * @brief  // 按键按下时调用
  * @note  
  * @param: 
  *
**/
void beep_power_sound(void)
{
  
	SendData_Set_Command(0x06,0x01);
	tx_thread_sleep(20);//delay_ms_dht11(20);//tx_thread_sleep(2);//2*10ms //delay_ms_dht11(20);//DelayMS(20);
  

}

/**
	*
	*@brief environment temperature value compare set temperater value
	*@notice
	*@param
	*
**/
void Heat_Process(void)
{
     static uint8_t default_init = 0xff;   // 第一次比较标志
     
     if(gpro_t.g_power_flag == 1){
	   if(ptc_prohibit_off_f == 1 || set_temperature_value_f ==1 ) return ;

	  uint8_t target_temp;

	  target_temp = setting_temperature;

	  if(temperature > 39){

        PTC_heat_open_f = 0;   // 立即关闭
	    first_temp_compare_f = 1; 
	    if(default_init != PTC_heat_open_f || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init= PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1){
					SendWifiData_To_Cmd(0x02,0);
		      
					}
		      

				}
	  
	     return ;

	  }

      // -----------------------------
    // 2. 第一次比较：必须立即决定 PTC 开关
    // -----------------------------
	  if(first_temp_compare_f == 0){

		if(temperature >= target_temp){
            PTC_heat_open_f = 0;   // 立即关闭

		       if(default_init != PTC_heat_open_f  || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f ++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0);
		        //delay_ms(100);//HAL_Delay(5);
		     

				}
		}
        else{
            PTC_heat_open_f = 1;   // 立即打开
            first_temp_compare_f = 1;         // 以后进入滞后控制
            if(default_init!= PTC_heat_open_f || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0x01);
		        //delay_ms(100);//HAL_Delay(5);
		      
			}
        }
        return;


	  }

		// -----------------------------
		// 3. 第二次及以后：使用 -2°C 滞后控制
		// -----------------------------
		if(first_temp_compare_f == 1)
		{
			// 当前是开启状态 → 高于设定温度则关闭
			if(temperature >= target_temp){
					PTC_heat_open_f = 0;
				if(default_init != PTC_heat_open_f  || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0);
		       // delay_ms(100);//HAL_Delay(5);
		      

				}
			}
			else
			{
				// 当前是关闭状态 → 低于设定温度 - 2 才重新打开
				if(temperature <  (target_temp - 2))
				PTC_heat_open_f = 1;
				
				if(default_init!= PTC_heat_open_f || key_input_temp_f ==1 || key_input_temp_f==2 ){
					default_init = PTC_heat_open_f;
					key_input_temp_f++;
				if(disp_second_f == 1)SendWifiData_To_Cmd(0x02,0x01);
		       // delay_ms(100);//HAL_Delay(5);
		      

				}
			}
		}

       }

}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void power_on_off_handler(void)
{

 
	 switch(gpro_t.g_power_flag){

      case 1:
           power_on_handler();
	 
	  break;

	  case 0:
	  	   power_off_handler();
		 

	  break;
      }

 

	
	

	
}

