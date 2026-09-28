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

power_state gon_t;



uint8_t  soft_version ;



// 任务结构体：这次我们直接检查标志位
typedef struct {
	uint32_t last_tick; 	   // 记录上一次真正运行时的系统绝对时间戳
    //volatile uint8_t *task_f; // 指向定时器置位的标志位
    //uint32_t period;        // 任务运行周期
    uint32_t period_ms;     // 存储的是 毫秒(ms) 值，更改名字更清晰
    void (*task_handler)(void);  // 任务函数
} Task_Config_t;


typedef struct{

  uint8_t adc_6_channels_done_flag;
  uint8_t fan_adc_daone_flag;

}Run_Ref_t;

Run_Ref_t gl_ref;


// 1ms 系统心跳计数器

// --- 任务函数声明 ---

static void handler_display_gxht40ad_value(void);
static void handler_AI_module_action(void);
static void handler_time_timer(void);



// 2. 任务注册表：将标志位地址与函数关联
static Task_Config_t g_tasks[] = {
    // last_tick,  period(ms),  task_handler
    {0,            55,         handler_AI_module_action},//10ms*50=500ms
    {0,            100,        handler_display_gxht40ad_value},
    {0,            220,        handler_time_timer}
 
 };


#define TASK_COUNT     (sizeof(g_tasks) / sizeof(Task_Config_t))


volatile uint8_t static beep_sound_f =0;
static void power_on_handler(void);
static void power_off_handler(void);
static void power_on_initial(void);
static void power_on_cycle_handler(void);

static void works_two_hours_times_handler(void);

uint8_t gxht4_rec=0;


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
   uint32_t boot_tick;
   uint8_t i;

   switch(gon_t.on_step){

   case 0:
   	  gon_t.off_step = 0;
      gpro_t.g_fan_speed =3;
      gpro_t.g_plasma_flag = true;
	  gpro_t.g_ai_flag = true;

	  //time
	  time_t.g_has_been_key_flag = false;
	  time_t.g_timer_flag =0;
	  time_t.g_time_hours =0;
	  time_t.g_time_minutes =0;
	  time_t.g_time_seconds =0 ;
	
       //error 
	   gpro_t.fan_warning_flag=0;
	  //wifi 
	  //gpro_t.water_pos_warning_flag=0;
      
	  TM1639_Clear();
  
      gon_t.on_step =1;
	

   break;

   case 1:
    
     //tm1639_display_temperature_digit(gpro_t.temperature);
     tm1639_display_time_digit(0);
     tm1639_display_humidity_digit(gpro_t.humidity);
    gon_t.on_step =2;


   break;

   case 2:
   	    boot_tick = tx_time_get();
		for(i=0;i <TASK_COUNT;i ++){

		     g_tasks[i].last_tick = boot_tick;
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
void power_on_handler(void)
{

        if(gon_t.on_step  < 8){
		  power_on_initial();
        }
		else{
	 // ✨【新增：紧急事件拦截响应】✨
        // 如果按键任务设置完温度，将 g_pro.g_immediate_heat_f 置为 1
          if(time_t.g_key_set_timer_flag == true && gpro_t.gTimer_set_timer_counter > 3){

			  tm1639_set_timer_digit(time_t.g_time_hours);
		      if(time_t.g_time_hours > 0 && time_t.g_has_been_key_flag == 1){
			  	time_t.g_has_been_key_flag =0;
			  	time_t.g_timer_flag = true;
				time_t.g_time_seconds =0;
			    time_t.g_time_minutes=0;
		      }
			  else time_t.g_timer_flag = false;
			  
			  time_t.g_key_set_timer_flag= false;
			  

		  }
          power_on_cycle_handler();
	

		}
        
}
/************************************************************************
 *
 * Function Name:void power_on_cycle_handler(void)
 * 功能:
 * 参数:无
 * 返回值:无
 *
 ************************************************************************/
void power_on_cycle_handler(void)
{
	 // 获取当前系统的绝对时间戳
      uint32_t current_tick = tx_time_get();
    // 通过时间片轮询核心算法，分时调用各个功能模块
    for (uint8_t i = 0; i < TASK_COUNT; i++) 
    {

       // 【关键对齐】：将配置表的 ms 转换为当前硬件环境的 Tick 数
        // 既然 1 Tick = 10ms，那么 Tick数 = ms / 10
        //uint32_t period_tick = g_tasks[i].period_ms / 10;
        
        // 防止配置错误：如果误填了小于 10ms 的周期，强制算作 1 个 Tick
        
       // if (period_tick == 0) {
         //   period_tick = 1; 
       // }

        // 使用纯 Tick 单位进行无符号减法，完美天然支持死循环绕回（Overflow）
		if ((current_tick - g_tasks[i].last_tick) >= g_tasks[i].period_ms) 
        {
            // 【工业级进化：防轰炸饱和截断】
            // 如果卡顿/被高优先级抢占的时间超过了 2 个周期，直接对齐当前时间，放弃追赶
            if ((current_tick - g_tasks[i].last_tick) > ( g_tasks[i].period_ms * 2)) 
            {
                g_tasks[i].last_tick = current_tick;
            }
            else 
            {
                // 如果只是正常范围内的轻微抖动，滚动累加周期，死锁锁相，消除长期长跑漂移
                g_tasks[i].last_tick += g_tasks[i].period_ms;
            }
            
            // 触发对应周期的执行函数（确保不为 NULL，防止空指针崩溃）
            if (g_tasks[i].task_handler != NULL)
            {
                g_tasks[i].task_handler(); 
            }
        }
    }


}


/************************************************************************
 *
 * Function Name: 
 * 功能:
 * 参数:无
 * 返回值:无
 *
 ************************************************************************/
void handler_display_gxht40ad_value(void)
{
   if(gpro_t.fan_warning_flag==false){
	 //tm1639_display_temperature_digit(gpro_t.temperature);

     tm1639_display_humidity_digit(gpro_t.humidity);

   	}
    else{
       SMG_Display_Err(0x02);//is fan is warning.

	}


}

static void handler_AI_module_action(void)
{
   ai_module_hanlder();
   water_full_led_blink();
}

static void handler_time_timer(void)
{
      switch(time_t.g_timer_flag){

	  case 0:
	      if(time_t.g_time_seconds > 59 ){
			 time_t.g_time_seconds =0;
			 time_t.g_time_minutes++;
		     if(time_t.g_time_minutes > 59){
                time_t.g_time_minutes=0;
                time_t.g_time_hours ++ ;
				if(time_t.g_time_hours > 99)time_t.g_time_hours =0;
			 }

	      }


	  break;

	  case 1:

	    if(time_t.g_time_seconds > 59 ){
			   time_t.g_time_seconds =0;
               time_t.g_time_minutes--;

               if(time_t.g_time_minutes < 0){

			      time_t.g_time_minutes = 59;
				  time_t.g_time_hours --;

				  if(time_t.g_time_hours < 0){
    
					  gon_t.off_step=0;
	                  SendWifiData_To_Cmd(0x01,0);
				  }

			   }

        }

	    break;



      }

}
/************************************************************************
 *
 * Function Name: 
 * 功能:
 * 参数:无
 * 返回值:无
 *
 ************************************************************************/
static void power_off_handler(void)
{
  
	
	switch(gon_t.off_step){
	
		 case 0:
		     gon_t.on_step=0;
		  //time
			  time_t.g_has_been_key_flag = false;
			  time_t.g_timer_flag =0;
			  time_t.g_time_hours =0;
			  time_t.g_time_minutes =0;
			  time_t.g_time_seconds =0 ;
	         power_off_led_handler();
		
	        TM1639_All_Off();
		     gon_t.off_step = 1;
	     break;

		 case 1:

		   if(gpro_t.gTimer_counter > 6){
		   	gpro_t.gTimer_counter=0;
		   SendData_Set_Command(0x11,0x01);

		   	}
		   gon_t.off_step = 0;

		 break;
	
   }
}

/**
  * @brief  
  * @note  
  * @param: 
  *
**/





/**
	*
	*@brief 
	*@notice
	*@param
	*
**/

/**
  * @brief  
  * @note  
  * @param: 
  *
**/
uint8_t power_counter;
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
void power_on_ref_init_handler(void)
{
  gpro_t.g_ai_flag = 1;
  gpro_t.g_plasma_flag = 1;
  gpro_t.g_fan_speed = 100 ;


}

void power_off_ref_init_handler(void)
{
  gpro_t.g_ai_flag = 0;
  gpro_t.g_plasma_flag = 0;
  gpro_t.g_fan_speed = 0 ;


}


