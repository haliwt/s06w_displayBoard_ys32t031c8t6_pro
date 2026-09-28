#include "bsp.h"




/**
*
*@brief  key of power
*@notice
*@param
*
**/
void key_power_short_handler(void)
{
 
	  // 1. 无论是开还是关，都要响一下蜂鸣器并翻转状态，直接提到最前面

   // 2. 根据翻转后的最新状态，决定执行开机动作还是关机动作
	if (gpro_t.g_power_flag ==false) 
	{ 
	    // 最新状态为 true，说明刚刚执行了“开机”翻转
	   gon_t.on_step=0;
	   SendWifiData_To_Cmd(0x01,0x01);
	
    }
	else 
	{ 
	    // 最新状态为 false，说明刚刚执行了“关机”翻转
	     gon_t.off_step=0;
	     SendWifiData_To_Cmd(0x01,0);
      
	}

}
/**
*
*@brief is set timer module 
*@notice
*@param
*
**/
void key_power_long_handler(void)
{
   tm1639_set_timer_digit(time_t.g_time_hours);
   time_t.g_key_set_timer_flag =1;
   time_t.g_has_been_key_flag = false;
   gpro_t.gTimer_set_timer_counter = 0;

}

/**
*
*@brief 
*@notice
*@param
*
**/

void key_ai_short_handler(void)
{
   if(time_t.g_key_set_timer_flag ==0){
   	
	   gpro_t.g_ai_flag = !gpro_t.g_ai_flag;
	   if(gpro_t.g_ai_flag ==true)LED_KEY_AI_ON();
	   else LED_KEY_AI_OFF();
	  
	   SendData_Set_Command(0x07,gpro_t.g_ai_flag);

   }
}
/**
*
*@brief  fan is dec press.
*@notice
*@param
*
**/
void key_fan_short_handler(void)
{
   
   if(time_t.g_key_set_timer_flag ==1){
   	 
       gpro_t.gTimer_set_timer_counter = 0;
     // 24 -> 0 减少，不循环：减到 0 之后不再变动
            if (time_t.g_time_hours > 0) {
                time_t.g_time_hours--;
            }
		time_t.g_has_been_key_flag = true;
        tm1639_display_time_digit(time_t.g_time_hours);
   }
   else{
   
   if(gpro_t.g_ai_flag == true ) return;

    
      gpro_t.g_fan_speed++;
      if (gpro_t.g_fan_speed > 3) {
         gpro_t.g_fan_speed= 1; // 确保异常时能正确恢复到 1 档
         SendWifiData_To_Data(0x1E,gpro_t.g_fan_speed);
      }

   }
   
}

/**
*
*@brief 
*@notice
*@param
*
**/
void key_plasma_short_handler(void)
{ 

   if(time_t.g_key_set_timer_flag ==1){
      	gpro_t.gTimer_set_timer_counter = 0;

     // 24 -> 0 减少，不循环：减到 0 之后不再变动
            if (time_t.g_time_hours < 24) {
                time_t.g_time_hours--;
            }
		time_t.g_has_been_key_flag = true;

		tm1639_display_time_digit(time_t.g_time_hours);

   }
   else{

   if(gpro_t.g_ai_flag == true) return;
   
   gpro_t.g_plasma_flag = !gpro_t.g_plasma_flag;
   if(gpro_t.g_plasma_flag ==true)LED_PLASMA_ON();
   else{
      LED_PLASMA_OFF();
   }
   SendData_Set_Command(0x03, gpro_t.g_plasma_flag);

   }
 }
/**
*
*@brief 
*@notice
*@param
*
**/
void ai_module_hanlder(void)
{
   if(gpro_t.g_ai_flag == true){
    
	  LED_FAN_ON();
      LED_PLASMA_ON();
      LED_KEY_AI_ON();

   }
   else{

     LED_KEY_AI_OFF();
	 

   }

 }




