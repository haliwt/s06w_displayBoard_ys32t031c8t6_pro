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
static void parse_cmd_or_data(uint8_t *pddata)
{
   
  
   switch(pddata[2]){
   	
	case 0x01: //power on or off

	if(pddata[3] == 0x01){//power on
	    gon_t.on_step=0;
		gpro_t.g_power_flag = true;
	    power_on_led_handler();
		SendWifiData_Answer_Cmd(0x01,0x01);
	 
	}
	else{//power off 
	    gon_t.off_step = 0;
	    gpro_t.g_power_flag = false;
	    power_off_led_handler();
		SendWifiData_Answer_Cmd(0x01,0);
	   
	}
	break;

	case 0x02: //PTC 
	if(pddata[3] == 0x01){//ptc on
         
	     SendWifiData_Answer_Cmd(0x02,0x01); //close ptc 
		
	}
	else{//power off 
	      
		 SendWifiData_Answer_Cmd(0x02,0x0); //close ptc 
    }

	break;

	case 0x03://plasma
	if(pddata[3] == 0x01){//ptc on

	    gpro_t.g_plasma_flag = 1;
		LED_PLASMA_ON();
	   	
	    SendWifiData_Answer_Cmd(0x03,0x01); //close ptc 

	}
	else{//power off 
		   
	   gpro_t.g_plasma_flag = 0;
	   LED_PLASMA_OFF();
	   SendWifiData_Answer_Cmd(0x03,0x0); //close ptc 
				//tx_thread_sleep(2);
	}

	break;

	case 0x04://ultrasonic 
	if(pddata[3] == 0x01){//ptc on

	    SendWifiData_Answer_Cmd(0x04,0x01); //close ptc 
			
	}
   else{//power off 

	  SendWifiData_Answer_Cmd(0x04,0x0); //close ptc 
		
    }
    break;

	case 0x05: // link wifi cmd .
      

	break;

	case 0x07://AI comm turn on or turn off

      if(pddata[3]== 0x01 || pddata[3]==0x02){
         gpro_t.g_ai_flag = 1;
		 LED_KEY_AI_ON();
	     LED_PLASMA_ON();
	  }
	  else if(pddata[3]== 0){
          gpro_t.g_ai_flag = 0; 
		  LED_KEY_AI_OFF();

	  }

	break;

	case 0x08: //temperature of high warning.

		if(pddata[3] == 0x01){  //warning 
         
	      SendWifiData_Answer_Cmd(0x08,0x01);
		  

		}
		else if(pddata[3]== 0x0){ //close 

		
	    }
	break;

	case 0x09: //fan of default of warning.error

	if(pddata[3] == 0x01){  //warning 
       gpro_t.fan_warning_flag = true;
	   SMG_Display_Err(0x02);
	   SendWifiData_Answer_Cmd(0x09,0x01);
		

	}
	else if(pddata[3] == 0x0){ //close 

	  gpro_t.fan_warning_flag = false;
	  SendWifiData_Answer_Cmd(0x09,0);
	
	}

	break;

	case 0x0B://fan turn on or turn off
		if(pddata[3] == 0x01){//ptc on
			

		}
		else{//power off 
			
		}

	break;

	case 0x0C://高水位报警
		if(pddata[3] == 0x01){//ptc on

		 gpro_t.water_pos_warning_flag = true;
		 SendWifiData_Answer_Cmd(0x0C,0x01);
		 
 
		}
		else{//power off 
		 gpro_t.water_pos_warning_flag = false;
		 SendWifiData_Answer_Cmd(0x0C,0);
		}

	break;

	 case 0x11:
	   	
       if(pddata[3]== 0x01){

	      SendWifiData_Answer_Cmd(0x11,1);
				 
	    }
		else{
					
		}

	 break;


	// begin 0x1x ---cmd
	case 0x15: //传递三个参数 ---通知:1. ptc,2.plasma 3. ultrasonic 

	break;

   
	case 0x1A: //read sensor "DHT11" temperature and humidity value .
       if(pddata[4]==2){ //数据的长度是:2个字节
          gpro_t.humidity= pddata[5];
	      gpro_t.temperature = pddata[6];

       }

     break;
	


	case 0x1C: //time is hours,minutes,seconds value .

	if(pddata[4] == 0x03){ //has three reference . hours,minutes,seconds.

    }
	break;

	case 0x1E: //fan of speed is data 123435

	if( pddata[5] < 34){
	
        

	}
	else if( pddata[5] < 67 &&  pddata[5] > 33){
	

	}
	else if(pddata[5] > 66){
	
	}
    break;


	case 0x1F: //link wifi is notice.--不是数据

	if(pddata[3] == 0x01){  // link wifi 

	
		SendWifiData_Answer_Cmd(0x1F,0x01);
		//tx_thread_sleep(2);

	}
	else{ //don't link wifi 

	 
	    SendWifiData_Answer_Cmd(0x1F,0);
		//tx_thread_sleep(2);

	}
    break;

	//begin 0x2x command or notice or data 

	case 0x20: //smart phone normal power on or off 
	    if(pddata[3] == 0x01){//power on
	        
		}
		else{//power off 
	

		}

    break;

	
   case 0x21: //APP smart phone Timer power on or off that App timer ---new .
	if(pddata[3]==0x01){ //power on by smart phone APP
		SendWifiData_Answer_Cmd(0x21,0x01);
		 //tx_thread_sleep(2);
		
    }
	else if(pddata[3]==0x0){  //power off by smart phone APP
		
		SendWifiData_Answer_Cmd(0x21,0x0);
		//tx_thread_sleep(2);
		   
	}

	break;


	//case 0x07:

     case 0x27 : //AI mode by smart phone of APP be control.
   
      if(pddata[3]== 0x01 || pddata[3]==0x02){
         gpro_t.g_ai_flag = 1;
	  }
	  else if (pddata[3]== 0){
          gpro_t.g_ai_flag = 0;  

	  }

	break;

	case 0x2A: //APP set up temperature data value at once open or close .

	break;

	case 0x2B: //APP set up timer timing value .
	

	break;

	case 0xFF:
		
	    parse_copy_cmd_or_data_handler(pddata);

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
static void parse_copy_cmd_or_data_handler(uint8_t *pdata)
{
    switch(pdata[3]){

        case 0x01:
				
			if(pdata[4] == 0x01){//power on

			  if(gpro_t.g_power_flag == true){


			  }
			  else{
			  	gon_t.on_step  =0;
		        gpro_t.g_power_flag = true;
				 power_on_led_handler();
			    
			  }
		
			}
			else if(pdata[4]==0 || pdata[4]==2){
			      if(gpro_t.g_power_flag == false){


				  }
				  else{
				  	gon_t.off_step =0;
			        gpro_t.g_power_flag = false;
					power_off_led_handler();
				    
				  }
              
		   }
	   break;

	   case 0x10:

	     	if(pdata[4] == 0x01){//power on
			
		     
	
			   

			}
			else if(pdata[4]==0){
		
          
	
			

			}


	   break;

	   

	   case  0x02:
	   	
		if(pdata[4] == 0x01){//ptc on
		
			
			
		}
		else {
			
		
              
		 }

		

	   break;

	   case 0x03:
	   	
		if(pdata[4] == 0x01){//ptc on
			
			
			}
			else {
			

			}

	   break;

	   case 0x04:
	   	
		if(pdata[4]  == 0x01){//ptc on
			

		}
		else {
	
		}

	   break;

	   case 0x05://0x05
	   	
	      if(pdata[4]  ==1){
            
			  
   
		  }
	   
	  break;

	   case 0x0B://风扇打开或者关闭

		if(pdata[3] == 0x01){//
			

		}
		else{//power off 
			

		}

	   break;


	    case 0x19://两个小时,休息指令
	   	    if(pdata[3]== 0x01){
	   	
               
	       	 }
			 else{
	              
			}

	   break;


	   case 0x1C:

	       if(pdata[4]== 0x01){
	   	
                 
	       	 }
			 else{
	             
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


