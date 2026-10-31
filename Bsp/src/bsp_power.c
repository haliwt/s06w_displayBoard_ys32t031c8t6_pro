#include "bsp.h"


#define THREADX_TICK_MS 10


#define MS_TO_TICKS(ms)  ((ms) / THREADX_TICK_MS)

#define DEFAULT_TEMP    40





typedef void (*task_handler_t)(void);

typedef struct {
    task_handler_t task_handler; // 任务回调函数
    uint32_t period;              // 运行周期 (Ticks)
    uint32_t last_tick;           // 上次运行时间 (Ticks)
} task_t;



// 任务函数前置声明
static void task_ui_key(void);
static void task_display_smg_timing(void);
static void task_dht11_display(void);
static void task_two_hours_timing(void);
static void task_send_version(void);
static void task_blink_colon(void);
static void task_compare_temp(void);
static void task_display_error_numbers(void);
static void task_copy_wating_ack(void);


// 任务配置表 (Table-Driven)
static task_t g_ui_tasks[] = {
    { task_ui_key,             10,    0 }, // 1*10m  
    { task_display_smg_timing,  6,    0}, //
    { task_blink_colon,        101,  0 }, // 101*10ms 冒号闪烁
    { task_dht11_display,      32,   0 }, // 300ms DHT11刷新
    { task_compare_temp,       340,  0 }, // 3s 控温比较
    { task_two_hours_timing,   129,  0 }, // 1.2s 运行计时
    { task_send_version,       3080,  0 }, // 3s  发送版本号
    {task_display_error_numbers,260,   0},
    {task_copy_wating_ack,20,0}
    
};

#define TASK_NUM (sizeof(g_ui_tasks) / sizeof(task_t))


typedef enum{

  PTC_STATE_OFF = 0,
  PTC_STATE_ON  = 1
}PTC_State;

static PTC_State ptc_state = PTC_STATE_OFF;



static void Set_TimerTiming_Number_Value(void);

bool ptc_prohibit_off_f;
bool  works_interval_f;

uint8_t fan_open_f;
uint8_t fan_speed_level;
bool fan_warning_f;
//ptc



//
uint8_t soft_version;
int8_t setting_timing_hour;

//wifi 

uint8_t  wifi_app_timer_power_on_f;

uint8_t key_be_pressed_f;
uint8_t disp_set_hours_time_f;


bool ptc_high_temperature_f ;


static void power_on_handler(void);
static void power_off_handler(void);

static void power_on_initial(void);
static void set_temperature_compare_value_fun(void);
static void works_two_hours_handler(void);
static void copy_order_waiting_ack(void);



/**
  * @brief  fan run is error
  * @note  
  * @param: 
  *
**/
void Clear_Ram(void)
{
    key_worked_f = 0;
	 
	  key_data = 0;
	  key_time = 0;
	
	  gpro_t.g_power_flag = 0;
		fan_speed_level = 100;
		fan_open_f = 0;
	
		

		gpro_t.g_dry_flag = 0;
		gpro_t.first_temp_compare_f=0;
		gpro_t.g_mouse_flag = 0;
		gpro_t.g_plasma_flag = 0;

		fan_warning_f = 0;
	
		//wifi 

		
	}

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
   
   switch(gpro_t.on_step){

   case 0:
   	  gpro_t.off_step = 0;

      gpro_t.works_two_minutes_value =0;
	  
	 //
	  if(gpro_t.connect_wifi_state == false){
		  gpro_t.one_minute_flag = false;
		  gpro_t.works_dispTime_minutes =0;
	      gpro_t.works_dispTime_hours =0;
		  gpro_t.gTimer_timing_seconds_counter=0;
	  }
	  //timer time
	  gpro_t.timer_dispTime_hours =0;
	  gpro_t.timer_dispTime_minutes = 0;
	  gpro_t.gTimer_timer_seconds_counter =0;
	  gpro_t.works_two_minutes_value=0;
						    
      //default temperature value 40 degree
      gpro_t.set_up_temperature_value = 40;

	  //wifi 
	  gpro_t.connecting_wifi_flag = false ;
	  
	

      power_on_led_open_handler();
  

   	 
		// 2. 初始化所有任务的 last_tick 镜像
	    init_tick = tx_time_get();
	    for (i = 0; i < TASK_NUM; i++) {
	        g_ui_tasks[i].last_tick = init_tick;
	    }
	 
	   gpro_t.on_step =0x10;

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

	if(gpro_t.on_step < 8){
		power_on_initial();
	}
	else{
		//✨   //【新增：紧急事件拦截响应】✨
		//如果按键任务设置完温度，将 g_pro.g_immediate_heat_f 置为 1

		if(gpro_t.time_10ms_f ==1 &&  ptc_high_temperature_f == 0 && fan_warning_f ==0){
			gpro_t.time_10ms_f=0;

		    if( gpro_t.direct_compare_temp_flag == true &&  gpro_t.rx_temp_flag == true){

				 TM1639_Write_2bit_SetUp_TempData(gpro_t.set_temperature_decade_value, gpro_t.set_temperature_unit_value, 0);
                 if( gpro_t.gTimer_key_temp_timing   > 2){
				    gpro_t.direct_compare_temp_flag =false;
				   gpro_t.rx_temp_flag = false;


				 }

			}
			else if(gpro_t.direct_compare_temp_flag == true &&  gpro_t.rx_temp_flag == false)
			{
			    gpro_t.direct_compare_temp_flag = false; // 立即清除触发标志，防止重复执行

			// 强制、立刻执行一次加热控制函数
			// 确保底层硬件（如继电器、PWM、PTC）在 20ms 内得到响应

              TM1639_Write_2bit_SetUp_TempData(gpro_t.set_temperature_decade_value, gpro_t.set_temperature_unit_value, 0);
	        // direct_temperature_compraison_handler();
			 
			}

		}

		copy_order_waiting_ack();

		// 2. 独立的 UI 周期任务轮询调度器（无论 10ms 标志位如何，时间到了就执行）
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

  if(ptc_high_temperature_f == 1  ||  fan_warning_f == 1) return;

  if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch < 3){

		 disp_mode_key_short_handler();
		 return ;
   }

   if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch > 2){

	  gpro_t.key_model_short_flag  =0;
	  
     if(gpro_t.set_timer_timing_value_success==false){
	 	  gpro_t.hours_two_unit_bit = gpro_t.works_dispTime_hours %10 ;
	      gpro_t.minutes_one_decade_bit = gpro_t.works_dispTime_minutes /10;

		   Display_Timing(gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes,0);
          gpro_t.ui_time_mode = TIME_MODE_NORMAL;
     }

   	}

   if (gpro_t.set_timer_timing_doing_value == 1 ){

       Set_TimerTiming_Number_Value();

	}
	
	// disp_smg_blink_set_tempeature_value();

}
/**
*@brief 
*@param
*@notice
**/
static void task_display_smg_timing(void)
{

      if(ptc_high_temperature_f == 1 ||  fan_warning_f == 1 || gpro_t.set_timer_timing_doing_value == 1 ) return;
   
     // 4. 正常显示工作时间（你原来的 Display_SmgTiming_Value）
	if(gpro_t.set_timer_timing_doing_value == 0 && gpro_t.key_model_short_flag == 0){
		 Display_SmgTiming_Value();
				
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
	if(fan_warning_f == 1 || ptc_high_temperature_f == true) return ;

	  works_two_hours_handler();

}
/**
*@brief 
*@param
*@notice
**/
static void task_send_version(void)
{
	 //SendData_Set_Command_Safe(0xF0,0x02);//
	// SendData_Set_Command(0xF0,0x02);
	 
}
/**
*@brief 
*@param
*@notice
**/

static void task_blink_colon(void)
{
   if(fan_warning_f == 1 || ptc_high_temperature_f == true) return ;
	Display_TimeColon_Blink_Fun();
	
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
/**
*@brief 
*@param
*@notice
**/
static void task_display_error_numbers(void)
{

  display_error_numbers_handler();

}
/**
*@brief 
*@param
*@notice
**/
static void task_copy_wating_ack(void)
{
   copy_order_waiting_ack();

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
    //以前已经设置过定时模式,现在显示之前的定时时间
    if(gpro_t.key_add_dec_pressed_flag == 0 && gpro_t.set_timer_timing_doing_value== 1 && gpro_t.gTimer_key_timing  < 4){
		if(gpro_t.set_timer_timing_value_success  == TIME_MODE_TIMER && gpro_t.key_add_dec_pressed_flag ==0 ){
			 gpro_t.hours_two_unit_bit = gpro_t.timer_dispTime_hours %10 ;
	         gpro_t.minutes_one_decade_bit = gpro_t.timer_dispTime_minutes /10;

			  Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);//don't display numbers

		}
		else if( gpro_t.set_timer_timing_value_success  == TIME_MODE_NORMAL && gpro_t.key_add_dec_pressed_flag ==0){

		    gpro_t.timer_dispTime_hours =0;
			gpro_t.timer_dispTime_minutes =0;

			Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);//don't display numbers
		}
    }


    if(gpro_t.key_add_dec_pressed_flag == 0 && gpro_t.set_timer_timing_doing_value== 1 && gpro_t.gTimer_key_timing  > 3){

        gpro_t.set_timer_timing_doing_value=0;
		if(gpro_t.set_timer_timing_value_success==false){
			gpro_t.hours_two_unit_bit = gpro_t.works_dispTime_hours %10 ;
	        gpro_t.minutes_one_decade_bit = gpro_t.works_dispTime_minutes /10;
			gpro_t.ui_time_mode = TIME_MODE_NORMAL;
		}
        return ;
	}
	
	if(gpro_t.set_timer_timing_doing_value== 1 && gpro_t.key_add_dec_pressed_flag ==1 && gpro_t.gTimer_key_timing < 4){

  
		if( gpro_t.timer_dispTime_hours >0){ //set up timer numbers value 
		
			gpro_t.set_timer_timing_value_success  = TIME_MODE_TIMER;//disp_timer_times;
			
		
			
			gpro_t.ui_time_mode = TIME_MODE_TIMER;
			 gpro_t.hours_two_unit_bit = gpro_t.timer_dispTime_hours %10 ;
	         gpro_t.minutes_one_decade_bit = gpro_t.timer_dispTime_minutes /10;

	        Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);



		}
	    else if(gpro_t.timer_dispTime_hours	== 0 ){ //set up timer numbers value 
			gpro_t.set_timer_timing_value_success  = TIME_MODE_NORMAL; //disp_works_times;
			//gpro_t.ui_time_mode = TIME_MODE_NORMAL;//key_t.disp_smg_mode_flag = disp_works_times;
			gpro_t.gTimer_timer_seconds_counter = 0;
			gpro_t.timer_dispTime_minutes=0;

			 gpro_t.hours_two_unit_bit = gpro_t.timer_dispTime_hours %10 ;
	         gpro_t.minutes_one_decade_bit = gpro_t.timer_dispTime_minutes /10;

            Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);




			//SendData_Tx_Data(0x2B, gpro_t.timer_dispTime_hours) ;
			//SendData_Set_Command(0x2B, gpro_t.timer_dispTime_hours);
			//tx_thread_sleep(2);
		}
	  
	}

    if(gpro_t.key_add_dec_pressed_flag == 1 && gpro_t.set_timer_timing_doing_value== 1 && gpro_t.gTimer_key_timing  > 3){

        gpro_t.set_timer_timing_doing_value=0;
		gpro_t.key_add_dec_pressed_flag =0;
	    gpro_t.gTimer_timer_seconds_counter = 0;
		gpro_t.timer_one_minute_flag = false;
		if(gpro_t.set_timer_timing_value_success==false){
			gpro_t.ui_time_mode = TIME_MODE_NORMAL;

		      gpro_t.hours_two_unit_bit = gpro_t.works_dispTime_hours %10 ;
	          gpro_t.minutes_one_decade_bit = gpro_t.works_dispTime_minutes /10;
			
			 Display_Timing(gpro_t.works_dispTime_hours,gpro_t.works_dispTime_minutes,0);
			
		}
		else if(gpro_t.set_timer_timing_value_success==true){
           
		   gpro_t.ui_time_mode = TIME_MODE_TIMER;
		    gpro_t.hours_two_unit_bit = gpro_t.timer_dispTime_hours %10 ;
	        gpro_t.minutes_one_decade_bit = gpro_t.timer_dispTime_minutes /10;
		   Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);

		}
        return ;
	}
	
}

#if 0

		gpro_t.timer_dispTime_hours = 0 ;
		gpro_t.timer_dispTime_minutes = 0;

		Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);

		
		gpro_t.set_timer_timing_doing_value =0;


		gpro_t.set_timer_timing_value_success  = 0;
		key_t.disp_smg_mode_flag = TIME_MODE_TIMER; //disp_works_times;


	   }

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
 void power_off_handler(void)
{
  

    switch(gpro_t.off_step){
	
		 case 0:
			gpro_t.on_step =0;
	       
		     power_off_clear_refer();
	         all_led_off();
	         TM1639_Display_ON_OFF(0);
			
			
			gpro_t.off_step = 1;
	
		 break;
	
		 case 1:
               if( gpro_t.gTimer_wifi_connect_counter>1){
			gpro_t.gTimer_wifi_connect_counter=0;
		        LED_POWER_TOGGLE();

               }
               gpro_t.off_step = 1;
			 
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
   gpro_t.g_power_flag = 0;
   System_Status_PowerOff() ;

        
    
}
/**
  * @brief  
  * @note  
  * @param: 
  *
**/
void works_two_hours_handler(void)
{
     

	 switch(works_interval_f){


	   case 0:
	 
		#if  0 //DEBUG_ENABLE 
			if( gpro_t.works_two_minutes_value>4 && works_interval_f==0){
		#else 
			if(gpro_t.works_two_minutes_value > 119 && works_interval_f==0){

		#endif
		    gpro_t.works_two_minutes_value =0;
		    gpro_t.gTimer_timer_seconds_counter =0;
            works_interval_f=1;
	        gpro_t.send_cmd_0x19_waiting_ack= 1;
			gpro_t.gTime_waiting_ack=0;
			SendData_Set_Command(0x19,0x01) ;//works two hours ,then have a rest 10 minutes.
	        tx_thread_sleep(2);
			gpro_t.gTime_waiting_ack=0;
			
		#if DEBUG_ENABLE 
			printf("works_interval_f = %d \n\r",works_interval_f);
		#endif 
		}

		
		
		  if(ptc_prohibit_off_f == false &&  gpro_t.g_dry_flag == true){
			 // 立即open
		      LED_DRY_ON();
		   }
		   else if(ptc_prohibit_off_f == true){
		      LED_DRY_OFF();

           }
		 
		

	   break;

	   case 1:

		#if 0
		   if(works_interval_f==1 &&  gpro_t.works_two_minutes_value>2){
		#else 
		   if(works_interval_f==1 && gpro_t.works_two_minutes_value >10){

		#endif 
			   gpro_t.works_two_minutes_value=0;
		       gpro_t.gTimer_timer_seconds_counter =0;
				works_interval_f =0;
				gpro_t.send_cmd_0x19_waiting_ack= 2;
				gpro_t.gTime_waiting_ack=0;
				SendData_Set_Command(0x19,0x0);
	            tx_thread_sleep(2);
				gpro_t.gTime_waiting_ack=0;
				 
				
		#if DEBUG_ENABLE 
			printf("works_interval_f = %d \n\r",works_interval_f);
		#endif 
		}


	
		break;

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
	*@brief environment gpro_t.dht11_temperature_value value compare set temperater value
	*@notice
	*@param
	*
**/
void Heat_Process(void)
{
     static uint8_t default_init = 0xff;   // 第一次比较标志
     
     
	   if(ptc_prohibit_off_f == true  ) return ;

	  uint8_t target_temp;

	  target_temp = gpro_t.set_up_temperature_value;

	  if(gpro_t.dht11_temperature_value > 39){

        gpro_t.g_dry_flag = 0;   // 立即关闭
        LED_DRY_OFF();
	    SendData_Set_Command(0x22,0);
		tx_thread_sleep(2);
	    gpro_t.first_temp_compare_f = 1; 
	   
	  
	     return ;

	  }

      // -----------------------------
    // 2. 第一次比较：必须立即决定 PTC 开关
    // -----------------------------
	if(gpro_t.first_temp_compare_f == 0){

		if(gpro_t.dht11_temperature_value >= target_temp){
            gpro_t.g_dry_flag = 0;   // 立即关闭
            LED_DRY_OFF();
		    SendData_Set_Command(0x22,0);
		    tx_thread_sleep(2);
		       
	}
    else{
            gpro_t.g_dry_flag = 1;   // 立即打开
            LED_DRY_ON();
		    SendData_Set_Command(0x22,1);
		    tx_thread_sleep(2);
            gpro_t.first_temp_compare_f = 1;         // 以后进入滞后控制
           
        }
        return;


	  }

		// -----------------------------
		// 3. 第二次及以后：使用 -2°C 滞后控制
		// -----------------------------
		if(gpro_t.first_temp_compare_f == 1)
		{
			// 当前是开启状态 → 高于设定温度则关闭
			if(gpro_t.dht11_temperature_value >= target_temp){
					gpro_t.g_dry_flag = 0;
					LED_DRY_OFF();
			        SendData_Set_Command(0x22,0);
		            tx_thread_sleep(2);
				
			}
			else
			{
				// 当前是关闭状态 → 低于设定温度 - 2 才重新打开
				if(gpro_t.dht11_temperature_value <  (target_temp - 2)){
					gpro_t.g_dry_flag = 1;
					LED_DRY_ON();
					SendData_Set_Command(0x22,1);
			        tx_thread_sleep(2);
					
					
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

/**
  * @brief  
  * @note  
  * @param: 
  *
**/
static void copy_order_waiting_ack(void)
{
  	if(gpro_t.send_cmd_0x19_waiting_ack== 1 && gpro_t.gTime_waiting_ack > 1){
		    gpro_t.gTime_waiting_ack=0;
			SendData_Set_Command(0x19,0x01) ;//works two hours ,then have a rest 10 minutes.
	        tx_thread_sleep(2);
	
	}
	else if(gpro_t.send_cmd_0x19_waiting_ack== 2 && gpro_t.gTime_waiting_ack > 1){
		   gpro_t.gTime_waiting_ack =0;
			SendData_Set_Command(0x19,0) ;//works two hours ,then have a rest 10 minutes.
	        tx_thread_sleep(2);
	
	}


}

