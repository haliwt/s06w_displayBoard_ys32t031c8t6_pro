#include "bsp.h"

// 为协议中的魔术字节定义常量，提高可读性
#define FRAME_HEADER        0x5A        //receive mainboard board header  
#define FRAME_NUM           0x10          //main deviece number is 0x10 
#define FRAME_OLD_NUM       0x01          //older version device NUM
#define FRAME_ACK_NUM       0x80          //new version from main answer singnal 0x80 new version . 
#define FRAME_END_BYTE              0xFE
#define DATA_FRAME_TYPE_INDICATOR   0x0F
#define FRAME_COPY_NUM              0xFF   //this is older version .

#define ACK_SUCCESS 0x00U
#define ACK_FAILURE 0x01U


typedef enum {
    CMD_STATE_IDLE = 0,      // 空闲状态
    CMD_STATE_WAIT_RESP,     // 已发出指令，等待主板响应
    CMD_STATE_SUCCESS,       // 成功（收到主板应答）
    CMD_STATE_FAILED         // 失败（达到最大重试次数仍无应答）
} Cmd_State_t;

typedef struct {
    Cmd_State_t state;
    uint32_t send_timestamp;  // 记录发送时间戳 (ms)
    uint8_t  retry_count;     // 当前重试次数
    uint8_t  max_retries;     // 最大重试次数（如 2 次：发1次+重试1次）
    uint8_t  cmd_type;        // 当前执行的指令类型（区分开机或关机）
} System_Cmd_Ctrl_t;

static System_Cmd_Ctrl_t g_cmd_ctrl = {0};

#define CMD_TIMEOUT_MS   1000  // 规定超时时间 1秒
#define MAX_RETRY_TIMES  2     // 最多发送次数

// 定义指令类型
#define CMD_TYPE_NONE     			0
#define CMD_TYPE_POWERON  			1
#define CMD_TYPE_SHUTDOWN 			2



typedef struct Msg
{
    
    uint8_t   tx_counter_nums;
    uint8_t   bcc_check_code;
	uint8_t   check_code_hex;
    uint8_t   receive_data_length;
    uint8_t   data_length;
	uint8_t   rc_data_length;
	uint8_t   total_data_length;
	uint8_t   rx_total_numbers;
	uint8_t   rx_data[4];
	uint8_t   usData[12];


}MSG_T;

MSG_T   gl_tMsg; 

static void parse_cmd_or_data(uint8_t *pddata);
static void parse_copy_cmd_or_data_handler(uint8_t *pdata);
static void parse_recieve_copy_data(uint8_t *pddata);

/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC????
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}



/********************************************************************************
	**
	*Function Name:void usart1_isr_callback_handler(void)
	*Function :  this is receive data from mainboard.
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
void usart1_isr_callback_handler(uint8_t data)
{
   volatile static uint8_t rx_state;
	switch(rx_state){

	 case 0:
		if(data == FRAME_HEADER){
			gl_tMsg.tx_counter_nums=0;
			gl_tMsg.usData[gl_tMsg.tx_counter_nums]=data;
			
			rx_state =1;

		}
		else{
		   rx_state =0;

		}
	 break;

	 case 1:
			gl_tMsg.tx_counter_nums++;
			gl_tMsg.usData[gl_tMsg.tx_counter_nums]=data;

	        if(gl_tMsg.usData[gl_tMsg.tx_counter_nums]== FRAME_NUM ){
			 
				 rx_state = 2;
			 }
			 else{
				rx_state = 0;
				
             }

	 break;

	 case 2:
	 
		   gl_tMsg.tx_counter_nums++;
           gl_tMsg.usData[gl_tMsg.tx_counter_nums]=data;
		   
		  if(gl_tMsg.usData[gl_tMsg.tx_counter_nums]==0xFE && gl_tMsg.tx_counter_nums> 4){
		      rx_state = 3;
		  }
		 
     break;
			 
	 case 3:
		       gl_tMsg.tx_counter_nums++;
	           gl_tMsg.usData[gl_tMsg.tx_counter_nums]=data;
			 
	           rx_state = 0;
               gl_tMsg.rx_total_numbers = gl_tMsg.tx_counter_nums;


			   gl_tMsg.bcc_check_code = data;

               wifi_semaphore_xtask();//display_board_xtask_notice();

	 break;

	 default:
	   rx_state =0;

	 break;

	 }

}

/**
  * @brief This function handles USART1 global interrupt 
  * @param
  * @retrval 
**/
static void parse_cmd_or_data(uint8_t *pdata)
{
   
   static uint8_t ptc_set_wifi = 0xff;
   switch(pdata[2]){

   case 0:

   break;

   case 0x01: 

         
        if(pdata[3] == 0x01){ //open
                
		        
				SendWifiData_Answer_Cmd(0x01,0x01);
	            tx_thread_sleep(2);
				
				gpro_t.g_power_flag  = true;
				gpro_t.on_step=0;
				
		}
	    else if(pdata[3] == 0x0){ //close 

		  
		
             SendWifiData_Answer_Cmd(0x01,0); //compatible older version 
	          tx_thread_sleep(2);
		      gpro_t.off_step=0;
              gpro_t.g_power_flag  = false ;//gpro_t.gpower_on = false;
        }
      
       

     break;

	  case 0x10: // 确认设备是否开机和关机，没有蜂鸣器声响

         if(pdata[3] == 0x01){ //open
	        if(gpro_t.g_power_flag  == true) return ;
		 
	       gpro_t.on_step=0;
	       gpro_t.g_power_flag = true;
		   SendWifiData_Answer_Cmd(0x10,0x01);
	       tx_thread_sleep(1);
	
	     } 
	     else if(pdata[3] == 0x0){ //close 

		    if(gpro_t.g_power_flag == false) return ;
		
			 gpro_t.off_step=0;
              gpro_t.g_power_flag = 0;
			 
              SendWifiData_Answer_Cmd(0x10,0x0); //power off .
              tx_thread_sleep(1); 
     }

     break;

	 case 0x21: //smart phone power on or off that App timer .
           if(pdata[3]==0x01){

		 
			gpro_t.connect_wifi_state = true;
		    LED_WIFI_ON();
		   	wifi_app_timer_power_on_f =1;
    
		   	SendWifiData_Answer_Cmd(0x21,0x01);
	        tx_thread_sleep(2);
		   	gpro_t.g_power_flag  = true;
			gpro_t.on_step=0;
			 
		   	}
		    else{
                 gpro_t.connect_wifi_state = true;
				
				SendWifiData_Answer_Cmd(0x21,0);
	            tx_thread_sleep(2);
				gpro_t.off_step=0;
                 gpro_t.g_power_flag  = false ;//gpro_t.gpower_on = false;

			}
           
     break; 

	  case 0X02: //PTC key of command .

        if(pdata[3] == 0x01 ){//phone_cmd_power
               ptc_prohibit_off_f =0; 
			 
			 if(works_interval_f==0){//two hours have a rest ten minutes .
	            if(fan_warning_f  ==0 && ptc_high_temperature_f ==0){ //PTC warning flag
                    gpro_t.g_dry_flag = true;
					LED_DRY_ON();
               }
			 }
           SendWifiData_Answer_Cmd(0x02,0x01); //
           tx_thread_sleep(1); 
	   }
       else if(pdata[3]== 0x0 ){
	   
		    ptc_prohibit_off_f = 1;
            SendWifiData_Answer_Cmd(0x02,0x0); //
             tx_thread_sleep(1); 

		     gpro_t.g_dry_flag = false;
			 LED_DRY_OFF();
       }
      break;

	  
     case 0X03: //PLASMA ACTIVE OPEN OR CLOSE
   
		  if(pdata[3]== 0x01){
			 
		     if(works_interval_f==0){//two hours have a rest ten minutes .
	            if(fan_warning_f  ==0 && ptc_high_temperature_f ==0){ //PTC warning flag
                    gpro_t.g_plasma_flag = true;
					LED_PLASMA_ON();
               }
			 }
			SendWifiData_Answer_Cmd(0x03,0x01); //
			tx_thread_sleep(1); 
			 
		  }
		  else if(pdata[3]  == 0x0){
			
		    SendWifiData_Answer_Cmd(0x03,0x0); //
			tx_thread_sleep(1); 
			  
		    gpro_t.g_plasma_flag = false;
			LED_PLASMA_OFF();
		  }
   
   
	break;

	   
   	case 0x04: //ultrasonic	ACTIVE OPEN OR CLOSE
          
		  if(pdata[3]  == 0x01){  //open 
		
			
   
			if(works_interval_f==0 && fan_warning_f  ==0 && ptc_high_temperature_f ==0){
				
               gpro_t.g_mouse_flag = true;
			   LED_MOUSE_ON();
			}
			
			SendWifiData_Answer_Cmd(0x04,0x01); //
			tx_thread_sleep(1); 
   
		  }
		  else if(pdata[3] == 0x0){ //close 
          
		   if(works_interval_f==0 && fan_warning_f  ==0 && ptc_high_temperature_f ==0){
				
               gpro_t.g_mouse_flag = false;
			   LED_MOUSE_OFF();
			}
			
			SendWifiData_Answer_Cmd(0x04,0x0); //
			tx_thread_sleep(1); 
   
		  }
   
   
    break;

	
     case  0x05: // link wifi command

       if(pdata[3] == 0x01){  // link wifi 
        
      
          link_net_step =0;
          key_net_config_f =1;

         
		
          SendWifiData_Answer_Cmd(0x05,0x01); //WT.EDIT 2024.12.28
          tx_thread_sleep(1);
         
      
        }


     break;

	 case 0x06: //buzzer sound command 
         
     break;

     case 0x27:
	 case 0x07: //
		
	  if(pdata[4]== 0x01){
           gpro_t.ui_time_mode = TIME_MODE_TIMER;
	       gpro_t.key_model_short_flag  =1;
		   gpro_t.gTimer_disp_mode_switch=0;
		  

		}
		else{
		       
		    gpro_t.ui_time_mode = TIME_MODE_NORMAL;

		}
	 break;

	 
	 case 0x08 ://gpro_t.dht11_temperature_value of high warning.
	 
			 if(pdata[3] == 0x01){
				 
				 ptc_high_temperature_f  = 1;
				 gpro_t.g_dry_flag = false;
				 LED_DRY_OFF();
			 }
			 else if(pdata[3] == 0x0){ //close
	 
				 ptc_high_temperature_f  = 0;
				// gpro_t.g_dry_flag = true;
				// LED_DRY_ON();   
	 
	 
		    }
	 
		   break;
	 
	case 0x09: //fan of default of warning.
	 
			  if(pdata[3] == 0x01){  //warning
	 
				fan_warning_f = 1;
	         }
			 else if(pdata[3] == 0x0){ //close
	 
				fan_warning_f = 0;
	 
			 }
	 
	 
		   break;


      case 0x19: //works 2 hours ,then have a rest 10 minutes ->notice 

	    if(pdata[3]==1){ // recach 2 hours 

            works_interval_f=1;
        }
		else if(pdata[3]==0){
			  works_interval_f=0;//WT.EDIT 2026.01.26
	
		  
        
              if(ptc_prohibit_off_f >1)ptc_prohibit_off_f=1;//2026.02.27 WT.EDIT
            
		
              if(ptc_prohibit_off_f ==1 &&ptc_prohibit_off_f==0){
			  
				
              }
			  if(gpro_t.g_plasma_flag==true)LED_PLASMA_ON();
			  
		}
	   

	  break;

	 case 0x1A:
	 if(pdata[4] == 0x02){ //数据,has three data

	    gpro_t.dht11_humidity_value = pdata[5];
		gpro_t.dht11_temperature_value = pdata[6];

	 }
	 break;

  
	 case 0x1c ://表示时间：小时，分，秒,beijing timing

        if(pdata[4] == 0x03){ //数据,has three data

            if(pdata[5] < 24){ //WT.EDIT 2024.11.23
      
		    gpro_t.connect_wifi_state = true;
             gpro_t.works_dispTime_hours= pdata[5];// run_t.dispTime_hours  =  pdata[5];
             gpro_t.works_dispTime_minutes =pdata[6];//run_t.dispTime_minutes = pdata[6];
             gpro_t.gTimer_timing_seconds_counter =  pdata[7];//run_t.gTimer_disp_time_seconds =  pdata[7];
           }


        }
     break;


	 case 0x1F:
	   if(pdata[3]==1){ // recach 2 hours 
	     gpro_t.connect_wifi_state = true;
		 LED_WIFI_ON();
	   }
	   else if(pdata[3]==0){
         gpro_t.connect_wifi_state = false;

	   }


	 break;


	case 0x22: //PTC ON OR OFF by compare gpro_t.dht11_temperature_value value .

	    if(ptc_prohibit_off_f  == 1)return ;
		
        if(pdata[3]== 0x01){
		     if(works_interval_f ==0 &&ptc_prohibit_off_f==0){
			     gpro_t.g_dry_flag = true;
				 LED_DRY_ON();
			 
			  }   	
	   }
       else if(pdata[3]== 0x0){
	   
           ptc_prohibit_off_f =0 ;//gctl_t.g_dry_flag =0;
           gpro_t.g_dry_flag = false;
		   LED_DRY_ON();
	    }
	
   
     break;

	 case 0x23: //PTC ON OR OFF by compare gpro_t.dht11_temperature_value value .

	    if(pdata[3]== 0x01){

		    ptc_prohibit_off_f =0;
		     if(works_interval_f ==0){
			     gpro_t.g_dry_flag = true;
				 LED_DRY_ON();
			 
			  }   	
	   }
       else if(pdata[3]== 0x0){
	   
           ptc_prohibit_off_f =0 ;//gctl_t.g_dry_flag =0;
           gpro_t.g_dry_flag = false;
		   LED_DRY_ON();
	    }
	
   
     break;


	 
	 case 0x2A: //smart phone or display  board set gpro_t.dht11_temperature_value .receive.
	 
		   if(pdata[4]==0x01 && gpro_t.g_power_flag == true){
			  
			   if(pdata[5] >19 && pdata[5] < 41){
			   	ptc_prohibit_off_f  = 0;
				gpro_t.connect_wifi_state = true;
			
			    gpro_t.setting_temperature_value = pdata[5] ;
			    if(gpro_t.setting_temperature_value > gpro_t.dht11_temperature_value ){ //gpro_t.dht11_temperature_value

				   gpro_t.g_dry_flag = true;
			       if(works_interval_f ==0){

                       LED_DRY_ON();
				   }
				  
                }
			  }
		   
		   }
	break;


	case 0x2B :// timer timing value .
          if(pdata[4]== 0x01){ // one only data 
			 
             if(pdata[5] > 0){
	
			  gpro_t.set_timer_timing_value_success= true;
			  key_t.disp_smg_mode_flag= TIME_MODE_TIMER;
			  gpro_t.ui_time_mode = TIME_MODE_TIMER;
 
			   gpro_t.timer_dispTime_hours=pdata[5];
			   gpro_t.timer_dispTime_minutes=0;
	  
			   gpro_t.gTimer_timer_seconds_counter=0;
             }
			 else if(pdata[5]== 0){

	             gpro_t.set_timer_timing_value_success= false;
				 key_t.disp_smg_mode_flag=TIME_MODE_NORMAL;
				  gpro_t.ui_time_mode = TIME_MODE_NORMAL;
	 
				   gpro_t.timer_dispTime_hours=0;
			       gpro_t.timer_dispTime_minutes=0;
	  

			 }
				
      	  }
		  	
			

	break;

	 

	 case 0x6C: //Synchronize local time ->two display board 

	   if(pdata[4]==0x03){ 

		     if(pdata[5] < 24 && pdata[6] < 61 && pdata[7] < 61){
         
		      gpro_t.works_dispTime_hours= pdata[5];
			 
			  gpro_t.works_dispTime_minutes =pdata[6];
			

			 gpro_t.gTimer_timing_seconds_counter=pdata[7];
			
		     }
		 }

	 break;

	 case 0xF0: //software version difference older and new sotfware 
      
            soft_version = pdata[3];
	 
		   #if DEBUG_ENABLE


               printf("soft_version = %d \r\n",soft_version);
		   

		   #endif 

    break;


	case 0xFF: //copy comand or notice or data.

	       parse_recieve_copy_data(pdata) ;

	break;

	}
  

}

/**
* @brief receive copy cmd or data from mainboard .
* @note  处理 ACK 帧
* @param
* @return 
*
*/
static void parse_recieve_copy_data(uint8_t *pddata)
{

    switch(pddata[3]){
    
       case 0:
    
    
       break;

	   case 0x01:

	     if(pddata[4] == 0x01){ //open

           if(gpro_t.g_power_flag == true){
		   	g_cmd_ctrl.state = CMD_STATE_SUCCESS;
		   	return;

           }
		   gpro_t.on_step=0;
	       gpro_t.g_power_flag = true;

		 }
        else if(pddata[4] == 0x0){ //close 
		  if(gpro_t.g_power_flag == false) return;

		   gpro_t.off_step=0;
           gpro_t.g_power_flag =false;
			 
		}
	   

	   break;

	  case 0x02:
   
    if(pddata[4]==1){

         if(gpro_t.g_dry_flag== true) return ;
		 gpro_t.g_dry_flag= true ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
         ptc_prohibit_off_f =0;
		 LED_DRY_ON();
    }
    else if(pddata[4]==0){
      if(gpro_t.g_dry_flag== false) return ;
	  gpro_t.g_dry_flag= false ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
      ptc_prohibit_off_f =0;
		
	  LED_DRY_OFF();

    }

    break;

	   
    }
      
 }



/**
  * @brief This function handles USART1 global interrupt 
  * @param
  * @retrval 
**/
void decoder_handler(void)
{
    uint8_t check_bcc_code;
	check_bcc_code = bcc_check(gl_tMsg.usData,gl_tMsg.rx_total_numbers);
	if(check_bcc_code == gl_tMsg.bcc_check_code){
		parse_cmd_or_data(gl_tMsg.usData);
    }
}


