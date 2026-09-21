#include "bsp.h"


/*
 * bsp_key_app.c
 *
 *  Created on: 2025年2月19日
 *      Author: Administrator
 */

#include "bsp.h"


KEY_T_TYPEDEF key_t;







typedef struct {
    uint8_t *flag;
    uint8_t threshold;
    void (*onPress)(void);
} KeyHandler;

/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * // 设置温度并做边界检查
 * 
 **********************************************************************************/
void set_temperature_value(int8_t delta) 
{
   

     gpro_t.set_up_temperature_value   = gpro_t.set_up_temperature_value + delta;
	 if (gpro_t.set_up_temperature_value < 20) gpro_t.set_up_temperature_value = 20;
     if (gpro_t.set_up_temperature_value > 40) gpro_t.set_up_temperature_value= 40;
   

	

    gpro_t.set_temperature_decade_value =  gpro_t.set_up_temperature_value / 10;
    gpro_t.set_temperature_unit_value   =  gpro_t.set_up_temperature_value % 10;

    gpro_t.set_temperature_special_flag = 1;
    gpro_t.gTimer_key_temp_timing       = 0;
    gpro_t.g_manual_shutoff_dry_flag   = 0;
   
	

    TM1639_Write_2bit_SetUp_TempData(gpro_t.set_temperature_decade_value, gpro_t.set_temperature_unit_value, 0);
	direct_temperature_compraison_handler();
}



/*******************************************************
	*
	*Function Name: void bsp_plasma_handler(uint8_t data)
	*Function :
	*
	*
*******************************************************/
void adjust_timer_minutes(int8_t delta_min) 
{
    
    gpro_t.timer_dispTime_minutes=0;
    gpro_t.timer_dispTime_hours += delta_min;//gpro_t.timer_dispTime_minutes

	if(gpro_t.timer_dispTime_hours > 24){
         gpro_t.timer_dispTime_hours =24;
   	}
	else if (gpro_t.timer_dispTime_hours < 0) {
        gpro_t.timer_dispTime_hours = 0 ;  // 循环处理负值
    }

	gpro_t.hours_two_decade_bit    = gpro_t.timer_dispTime_hours / 10;
    gpro_t.hours_two_unit_bit      = gpro_t.timer_dispTime_hours % 10;
  


	Display_Timing(gpro_t.timer_dispTime_hours,gpro_t.timer_dispTime_minutes,0);

    
}

/****************************************************************
	*
	*Function Name :void handle_key(KeyHandler *handler) 
	*Function : set timer timing how many ?
	*Input Parameters : struct KeyHandler of reference
	*Retrurn Parameter :NO
	*
*****************************************************************/
void handle_key(KeyHandler *handler) 
{
    if (*(handler->flag) == 1) {
        *(handler->flag) += 1;

        if (handler->threshold > 0 && *(handler->flag) > handler->threshold) {
            *(handler->flag) = 80; // 特殊情况处理
        }

        if (handler->onPress) {
            handler->onPress();
        }
    }
}

/**********************************************************************************************************
*	函 数 名: void power_key_handler(void) 
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
**********************************************************************************************************/
void power_key_handler(void) 
{


    if(gpro_t.g_power_flag == false){
        SendData_Set_Command(0x01,0x01);//SendData_PowerOnOff(1); // power on
        tx_thread_sleep(2); 
    } 
	else {
        SendData_Set_Command(0x01,0); // power off
        tx_thread_sleep(2);
    }
}



/**********************************************************************************************************
*	函 数 名: void plasma_key_handler(void) 
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
**********************************************************************************************************/
void plasma_key_handler(void) 
{
    
        if(gpro_t.g_plasma_flag== true){
            gpro_t.g_plasma_flag= false;
            SendData_Set_Command(0x03, 0x00);
		    tx_thread_sleep(2);
            LED_PLASMA_OFF();
            //gpro_t.send_ack_cmd = check_ack_plasma_off;
        } else {
            gpro_t.g_plasma_flag= true;
            SendData_Set_Command(0x03, 0x01);
			tx_thread_sleep(2);
            LED_PLASMA_ON();
           // gpro_t.send_ack_cmd = check_ack_plasma_on;
        }
       
    //}
}
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void dry_key_handler(void) 
{
   
        if(gpro_t.g_dry_flag == 0) {
            SendData_Set_Command(0x02, 0x01);//sendCommandAndAck(dry_cmd, 0x01, check_ack_ptc_on);
			tx_thread_sleep(2);
            gpro_t.g_dry_flag = 1;
            gpro_t.g_manual_shutoff_dry_flag = 0;
            LED_DRY_ON();
        } 
		else if(gpro_t.g_dry_flag == 1){
            SendData_Set_Command(0x02, 0x00);//sendCommandAndAck(dry_cmd, 0x00, check_ack_ptc_off);
			tx_thread_sleep(2);
            gpro_t.g_dry_flag = 0;
            gpro_t.g_manual_shutoff_dry_flag = 1; // 手动关闭后不再自动开启
            LED_DRY_OFF();
        }
  
}
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void mouse_key_handler(void) 
{
   
        if(gpro_t.g_mouse_flag == 0) {
            // 开启 Mouse 功能
            SendData_Set_Command(0X04, 0x01);
            tx_thread_sleep(2);
            gpro_t.g_mouse_flag = 1;
            LED_MOUSE_ON();
           // gpro_t.send_ack_cmd = check_ack_mouse_on;  // 假设有对应的反馈类型
           

        } else if(gpro_t.g_mouse_flag == 1){
            // 关闭 Mouse 功能
            SendData_Set_Command(0x04, 0x00);
            tx_thread_sleep(2);
            gpro_t.g_mouse_flag = 0;
            LED_MOUSE_OFF();
          //  gpro_t.send_ack_cmd = check_ack_mouse_off;  // 假设有对应的反馈类型
           
        }
   
}
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void key_add_fun(void)
{
    if(gpro_t.ptc_warning == 1) return;

   
   
    switch(gpro_t.set_timer_timing_doing_value)
    {

	    case 3:
		case 0:  // 设置温度增加
            
            set_temperature_value(+1);
		   
		    
            break;

        case 1:  // 设置定时增加（每次加60分钟）
         
            gpro_t.gTimer_key_timing = 0;
            gpro_t.key_add_dec_pressed_flag = true;
            adjust_timer_minutes(1);  // 固定每次加60分钟
            break;
    }
   	
}


/****************************************************************
	*
	*Function Name :void key_dec_fun(void)
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void key_dec_fun(void)
{
    if(gpro_t.ptc_warning == 1) return;


    switch(gpro_t.set_timer_timing_doing_value)
    {

        case 3:
		case 0:  // 设置温度减少
          
            set_temperature_value(-1);
            break;

        case 1:  // 设置定时减少（每次减60分钟）
         
            gpro_t.gTimer_key_timing = 0;
            gpro_t.key_add_dec_pressed_flag = 1;
            adjust_timer_minutes(-1);  // 固定每次减60分钟
        break;
    }
}
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void mode_key_handler(void)
{

     
   gpro_t.gTimer_disp_mode_switch=0;
   gpro_t.key_model_short_flag = 1;
   if(gpro_t.ui_time_mode == TIME_MODE_TIMER){
      gpro_t.ui_time_mode=TIME_MODE_TIMER;
   }
   else if(gpro_t.ui_time_mode == TIME_MODE_NORMAL){
     gpro_t.ui_time_mode= TIME_MODE_TIMER;

   }
   #if DEBUG_FALG

    printf("sound again \r\n");


   #endif 
   
 }
		  
/****************************************************************
	*
	*Function Name :void direct_temperature_comparison_handler(void)
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void direct_temperature_compraison_handler(void)
{
	if(gpro_t.set_up_temperature_value > gpro_t.dht11_temperature_value){
 
	   gpro_t.g_dry_flag = 1; //gpro_t.gPtc=1;
	   LED_DRY_ON();
	   SendData_Set_Command(0x23,1);
	   tx_thread_sleep(1);
	

	}
	else{

		gpro_t.g_dry_flag =0;//gpro_t.gPtc =0 ;//gctl_t.g_dry_flag =0;

		LED_DRY_OFF();//PTC_SetLow();
	    SendData_Set_Command(0x23,0);
	    tx_thread_sleep(1);

		
	}

}


 
/****************************************************************
	*
	*Function Name :void wifi_mode_key_handler(void)
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/


/*
*********************************************************************************************************
*	函 数 名: void process_keys(void) 
*	功能说明:
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/




