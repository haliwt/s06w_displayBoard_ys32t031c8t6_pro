#include "bsp.h"


// 为协议中的魔术字节定义常量，提高可读性
#define FRAME_HEADER        0xA5        //receive display board header  
#define FRAME_NUM           0x02          //main deviece number is 0x10 
#define FRAME_OLD_NUM       0x01          //older version device NUM
#define FRAME_ACK_NUM       0x80          //new version from main answer singnal 0x80 new version . 
#define FRAME_END_BYTE              0xFE
#define DATA_FRAME_TYPE_INDICATOR   0x0F
#define FRAME_COPY_NUM              0xFF   //this is older version .

#define ACK_SUCCESS 0x00U
#define ACK_FAILURE 0x01U




volatile uint8_t uart1_rx_buf[UART1_RX_BUF_SIZE];
volatile uint16_t uart1_rx_head ;
volatile uint16_t uart1_rx_tail ;

uint8_t fan_rx_stop_flag;



typedef void (*Usart1RxCallback)(uint8_t data);

static Usart1RxCallback usart1_rx_cb = NULL;  //定义一个全局静态函数指针

//static void usart1_invoke_callback(uint8_t data);


//static void usart1_isr_callback_handler(uint8_t data);
static void parse_recieve_copy_data(uint8_t *pddata);


uint8_t rx_inputBuf[12];
uint8_t check_bcc_code;
uint8_t rx1_data;
uint8_t counter_power_flag;
uint8_t ptc_onoff_default ;


//提供注册接口
void usart1_register_rx_callback(Usart1RxCallback cb)
{
   usart1_rx_cb = cb;

}

void usart1_invoke_callback(uint8_t data)
{
   if(usart1_rx_cb !=NULL){

       usart1_rx_cb(data);
   }


}


void callback_register_usart1_rx(void)
{

   usart1_register_rx_callback(usart1_isr_callback_handler);

}




volatile uint8_t rx_state;



typedef enum{

  open =1,
  close =2,
  no_change =0  

}atcion_state_e;



typedef enum {
    UART_STATE_WAIT_HEADER = 0,
    UART_STATE_NUM=1,
    UART_STATE_CMD_NOTICE=2,
    UART_STATE_EXEC_CMD_OR_LEN=3,
    UART_STATE_FRAME_END=4,
    UART_STATE_BCC_CHECK,
    UART_STATE_OLDER_BCC_CHECK,
    UART_STATE_DATA_LEN,
    UART_STATE_DATA,
    UART_STATE_DATA_END,
    UART_STATE_DATA_BCC
} uart_parse_state_t;

typedef enum{

    power_on_off=1,
    ptc_on_off=2,
    plasma_on_off=3,
    ultrasonic_on_off=4,
    wifi_link=5,
    buzzer_sound_s=6,
    ai_mode=7,
    temp_high_warning=8,
    fan_warning_s=9,
    fan_on_off = 0x0B,

     //notice no sound 
    ack_power_on_off = 0x10,
    ack_ptc_on_off = 0x12,
    ack_plasma_on_ff= 0x13,
    ack_ultrasonic_on_off = 0x14,
}signal_parase_t;


typedef struct Msg
{
    
	uint8_t   cmd_notice;
	uint8_t   execuite_cmd_notice;
	uint8_t   copy_cmd_flag;
	uint8_t   rx_data_flag;	
    uint8_t   bcc_check_code;
	uint8_t   check_code_hex;
    uint8_t   receive_data_length;
    uint8_t   data_length;
	uint8_t   rc_data_length;
	uint8_t   total_data_length;
    uint8_t   rx_end_code;
	uint8_t   rx_total_numbers;
	uint8_t   rx_data[4];
	uint8_t   usData[12];
	uint8_t   desData[12];

}MSG_T;

MSG_T   gl_tMsg; 

uint8_t inputBuf[1];



static void usart1_protocol_state_machine(uint8_t *pdata);

volatile uint8_t rx_usart1_data_counter=0;

uint8_t parse_exit_flag,parse_decoder_flag;
/********************************************************************************
	**
	*Function Name:void usart1_isr_callback_handler(void)
	*Function :  this is receive data from mainboard.
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
#if 1
void usart1_isr_callback_handler(uint8_t data)
{
   
	switch(rx_state){

	 case 0:
		if(data == FRAME_HEADER){
			rx_usart1_data_counter=0;
			gl_tMsg.usData[rx_usart1_data_counter]=data;
			
			rx_state =1;

		}
		else{
		   rx_state =0;

		}
	 break;

	 case 1:
			rx_usart1_data_counter++;
			gl_tMsg.usData[rx_usart1_data_counter]=data;

	        if(gl_tMsg.usData[rx_usart1_data_counter]==0x01 || gl_tMsg.usData[rx_usart1_data_counter]==0x02){
			 
				 rx_state = 2;
			 }
			 else{
				rx_state = 0;
				
             }

	 break;

	 case 2:
	 
		   rx_usart1_data_counter++;
           gl_tMsg.usData[rx_usart1_data_counter]=data;
		   
		  if(gl_tMsg.usData[rx_usart1_data_counter]==0xFE && rx_usart1_data_counter> 4){
		      rx_state = 3;
		  }
		 
     break;
			 
	 case 3:
		       rx_usart1_data_counter++;
	           gl_tMsg.usData[rx_usart1_data_counter]=data;
			 
	           rx_state = 0;
               gl_tMsg.rx_total_numbers = rx_usart1_data_counter;

			   gl_tMsg.rx_end_code = 0;
			   
		       //gpro_t.decoder_success_flag=1;

			   gl_tMsg.bcc_check_code = data;

               wifi_semaphore_xtask();//display_board_xtask_notice();

	 break;

	 default:
	   rx_state =0;

	 break;

	 }

}
#else 
void usart1_isr_callback_handler(uint8_t data)
{
    switch(rx_state) {
        case 0: // 寻找帧头 0xA5
            if(data == FRAME_HEADER) {
                rx_usart1_data_counter = 0;
                gl_tMsg.usData[rx_usart1_data_counter] = data;
                rx_state = 1; 
                gpro_t.decoder_success_flag = 0; // 开始新的一帧，清除成功标志
            }
            break;

        case 1: // 接收数据体
            rx_usart1_data_counter++;
            if(rx_usart1_data_counter < 12) { // 防止数组越界
                gl_tMsg.usData[rx_usart1_data_counter] = data;
                
                // 关键点：只有当收到 0xFE 时，才认为数据接收完毕，准备接收最后的校验位
                if(data == FRAME_END_BYTE) {
                    rx_state = 2;
                }
            } else {
                // 超长错误，重置
                rx_state = 0;
            }
            break;

        case 2: // 接收最后的 BCC 校验位
            rx_usart1_data_counter++;
            if(rx_usart1_data_counter < 12) {
                gl_tMsg.usData[rx_usart1_data_counter] = data;
                gl_tMsg.bcc_check_code = data; // 存入校验码
                gl_tMsg.rx_total_numbers = rx_usart1_data_counter + 1;
                
                // --- 校验逻辑 ---
                // 这里可以写一个简单的循环计算 BCC，如果匹配再置标志位
                gpro_t.decoder_success_flag = 1; 
                
                // 只有一整帧完全接收且校验过，才唤醒任务
                //display_board_xtask_notice(); 
            }
            rx_state = 0; // 处理完无论成功失败，必须回到状态0等待新帧
            break;

        default:
            rx_state = 0;
            break;
    }
}



#endif 
/********************************************************************************
	**
	*Function Name:void usart1_protocol_state_machine(void)
	*Function :  in process bsp_freertos.c xTaskMsgPro
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
static void usart1_protocol_state_machine(uint8_t *pdata)
{

   static uint8_t ptc_set_wifi = 0xff;
   switch(pdata[2]){

   case 0:

   break;

   case power_on_off: 

         
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

	  case ptc_on_off: //PTC key of command .

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

	  
     case plasma_on_off: //PLASMA ACTIVE OPEN OR CLOSE
   
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

	
     case  wifi_link: // link wifi command

       if(pdata[3] == 0x01){  // link wifi 
        
      
          link_net_step =0;
          key_net_config_f =1;
		  key_net_config_time =0;
         
		
          SendWifiData_Answer_Cmd(0x05,0x01); //WT.EDIT 2024.12.28
          tx_thread_sleep(1);
         
      
        }


     break;

	 case buzzer_sound_s: //buzzer sound command 
         
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
			  fan_rx_stop_flag =0 ;
		  
        
              if(ptc_prohibit_off_f >1)ptc_prohibit_off_f=1;//2026.02.27 WT.EDIT
            
		
              if(ptc_prohibit_off_f ==1 &&ptc_prohibit_off_f==0){
			  
				
              }
			  if(gpro_t.g_plasma_flag==true)LED_PLASMA_ON();
			  
		}
	   

	  break;

  
	 case 0x1c ://表示时间：小时，分，秒,beijing timing

        if(pdata[4] == 0x03){ //数据,has three data

            if(pdata[5] < 24){ //WT.EDIT 2024.11.23
      
		    gpro_t.connect_wifi_state = true;
            LED_WIFI_ON();
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
			
			    setting_temperature = pdata[5] ;
			    if(setting_temperature > gpro_t.dht11_temperature_value ){ //gpro_t.dht11_temperature_value

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
/**********************************************************************
	*
	*Function Name:static void parse_recieve_copy_data_handler(void)
	*Function: display board send to mainboard answer signal
	*Input Ref:NO
	*Return Ref:NO
	*
**********************************************************************/
static void parse_recieve_copy_data(uint8_t *pddata)
{

    switch(pddata[3]){
    
       case 0:
    
    
       break;

	   case 0x01:

	     if(pddata[4] == 0x01){ //open

           if(gpro_t.g_power_flag == true) return;
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
   // gpro_t.decoder_success_flag=0;
	check_bcc_code = bcc_check(gl_tMsg.usData,gl_tMsg.rx_total_numbers);
	if(check_bcc_code == gl_tMsg.bcc_check_code){
		usart1_protocol_state_machine(gl_tMsg.usData);
    }
}







