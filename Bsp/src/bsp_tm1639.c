#include "bsp.h"

#if 0
static void delay_us(uint32_t us)
{
    // 每个 us 需要大约 16 次循环（64 cycles / 4 cycles per loop）
    uint32_t cycles = us * 16;//  

    while(cycles--)
    {
        __NOP();
    }
}
#endif 

// 数码管段码表�?0-9的显示码
static const uint8_t TM1639_Number_Table[] = {
    0xF3, // 0: 0011 1111   （f,e,d,c,b,a�?--0x3F)(abcd efgh = 0b1111 0011)
    0x60, // 1: 0000 0110 --0x06--写数据式冲低位开始，向高位开始写
    0xB5, // 2: 0101 1011 --0x5B
    0xF4, // 3: 0100 1111 --0x4F
    0x66, // 4: 0110 0110
    0xD6, // 5: 0110 1101 --0x6D
    0xD7, // 6: 0111 1101  --0x7D 
    0x70, // 7: 0000 0111
    0xF7, // 8: 0111 1111
    0xF6,  // 9: 0110 1111
    0X0   //0x0A
};

// 字母和特殊字符显示码
static const uint8_t TM1639_Char_Table[] = {
    0x67, // H: 0111 0110 (hgfe,dcba = 0111 0110)--(abcd efgh =0110 0111)
    0x36, // °: 0110 0011 (hgfe,dcba= 0110  0011)
    0x93, // C: 0011 1001 (hgfe,dcba = 0011 1001)
    0x05,  // RH的H部分: 0101 0000 (hgfe,dcba= 0101 0000)
    0x45   //n:(hgfe dcba= 0101 0100) =  (abcd efgh = 0b0100 0101) //字节序列的排列
};

#define TM1639_CHAR_H 					TM1639_Char_Table[0]
#define TM1639_CHAR_DEGREE 				TM1639_Char_Table[1]
#define TM1639_CHAR_C 					TM1639_Char_Table[2]
#define TM1639_CHAR_RH 					TM1639_Char_Table[3]

#define TM1639_CHAR_N                   TM1639_Char_Table[4]

#define TM1639_DOT  0x08 // 小数点段�?,from low position start


static void TIM17_Init_1MHz(void)
{
    /* 使能 TIM17 时钟 (APB2总线) */
     LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM17);
    /* 设置预分频器：48MHz / 48 = 1MHz */
    LL_TIM_SetPrescaler(TIM17, 64 - 1);

    /* 设置自动重载值（周期） */
    LL_TIM_SetAutoReload(TIM17, 0xFFFF);

    /* 设置计数器模式为向上计数 */
    LL_TIM_SetCounterMode(TIM17, LL_TIM_COUNTERMODE_UP);

    /* 使能 TIM17 定时器计数器 */
    LL_TIM_EnableCounter(TIM17);
}

void Delay_US_dht11(uint16_t us)
{
   #if 1

	uint16_t start = TIM17->CNT;

    while ((uint16_t)(TIM17->CNT - start) < us)
    {
        /* busy wait */
    }
	#else 
	  while (us--)
    {
        // 48MHz ?,1us ?? 48 ???
        // ?? while ?????????????(? 6~9 ???)
        // ??? 40 ?????? NOP ??
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }


	#endif 
}




/**
 * @brief  TM1639写入�?个字�?
 * @param  byte: 要写入的字节
 * @retval None
 */
static void TM1639_Write_Byte(uint8_t byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        TM1639_CLK_SetLow();
       
        
        if(byte & 0x01)
            TM1639_DIO_SetHigh(); //写入数据 �?1�?
        else
            TM1639_DIO_SetLow(); //写入数据 �?0�?
            
    
        TM1639_CLK_SetHigh();
        Delay_US_dht11(4);//delay_us(4);//is big error .DATA.2025.06.13
        byte >>= 1;
    }
}

/**
 * @brief  TM1639�?始信�?
 * @param  None
 * @retval None
 */
static void TM1639_Start(void)
{
    TM1639_STB_SetHigh();
    ///Delay_US_dht11(8);//delay_us(8);//2
    TM1639_CLK_SetHigh();
   // Delay_US_dht11(8);//delay_us(8);//2
    TM1639_STB_SetLow();
    //Delay_US_dht11(8);//delay_us(8);//2
}

/**
 * @brief  TM1639停止信号
 * @param  None
 * @retval None
 */
static void TM1639_Stop(void)
{
    TM1639_CLK_SetLow();
   /// Delay_US_dht11(8);//delay_us(8);//2
    TM1639_DIO_SetLow();
   /// Delay_US_dht11(8);//delay_us(8);//
    TM1639_STB_SetHigh();
   // Delay_US_dht11(8);//delay_us(8);//
}

/**
 * @brief  初始化TM1639
 * @param  None
 * @retval None
 */
void TM1639_Init(void)
{
    // 设置数据命令：自动地�?增加
    TM1639_Start();
    TM1639_Write_Byte(TM1639_CMD_DATA);
    TM1639_Stop();
    
    // 设置显示控制：显示开，最大亮�?
    TM1639_Display_ON_OFF(1);
    TM1639_Set_Brightness(TM1639_BRIGHTNESS_MAX);
}

/**
 * @brief  设置显示亮度
 * @param  bright: 亮度级别(0-7)
 * @retval None
 */
void TM1639_Set_Brightness(uint8_t bright)
{
    if(bright > TM1639_BRIGHTNESS_MAX)
        bright = TM1639_BRIGHTNESS_MAX;
        
    TM1639_Start();
    TM1639_Write_Byte(TM1639_CMD_DISPLAY | TM1639_DISPLAY_ON | bright);
    TM1639_Stop();
}

/**
 * @brief  显示�?关控�?
 * @param  status: 1-�?显示�?0-关显�?
 * @retval None
 */
void TM1639_Display_ON_OFF(uint8_t status)
{
    TM1639_Start();
    if(status)
        TM1639_Write_Byte(TM1639_CMD_DISPLAY | TM1639_BRIGHTNESS_MAX);
    else
        TM1639_Write_Byte(TM1639_DONOT_DISPLAY);
    TM1639_Stop();
}

/**
 * @brief  写入完整的一位数码管（包括高4位和�?4位）
 * @param  addr_h: �?4位地�?
 * @param  addr_l: �?4位地�?
 * @param  data: 要显示的段码数据
 * @retval None
 */
void TM1639_Write_Digit_Full(uint8_t addr_h, uint8_t addr_l, uint8_t data)
{
    // 先写入低4�?
    TM1639_Start();
    TM1639_Write_Byte(addr_l);
    TM1639_Write_Byte(data & 0x0F);  // �?4位数�?
    TM1639_Stop();
    
    // 再写入高4�?
    TM1639_Start();
    TM1639_Write_Byte(addr_h);
    TM1639_Write_Byte(data >> 4);  // �?4位数�?
    TM1639_Stop();
}

/**
 * @brief  写入半个�?位数码管（包括高4位或�?4位）
 * @param  addr_h: �?4位地�?
 * @param  addr_l: �?4位地�?
 * @param  data: 要显示的段码数据
 * @retval None
 */
void TM1639_Write_Half_Digit(uint8_t addr, uint8_t data)
{

    
    // 先写�?4�?--高字节或者低字节
    TM1639_Start();
    TM1639_Write_Byte(addr);
    TM1639_Write_Byte(data);  // �?4位数�?
    TM1639_Stop();
    
  
}


/**
 * @brief  显示3位数�?
 * @param  num: 要显示的数字(0-999)
 * @retval None
 */
void TM1639_Display_3_Digit(uint8_t num)
{
    static uint8_t ten, one;
    
   // 提取各位数字
   
    ten = num  / 10;
    one = num % 10;
    
    // 写入十位（最左边�?
    
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Number_Table[ten]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Number_Table[one]);
        
    // 写入个位（最右边�?'H'

	TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,TM1639_CHAR_H);

}

/**
 * @brief  显示3位数�?
 * @param  num: 要显示的数字(0-999)
 * @retval None
 */
void TM1639_Display_setTimerHours_3_Digit(uint8_t num)
{
    static uint8_t ten, one;
    
   // 提取各位数字
   
    ten = num  / 10;
    one = num % 10;
    
    // 写入十位（最左边�?
    
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Number_Table[ten]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Number_Table[one]);
        
    // 写入个位（最右边�?'H'
	
	 TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,TM1639_CHAR_H);
	 
	 
}
/**
 *@breif
 *@param :
 *
 *
**/
void TM1639_Display_setTimerMinutes_3_Digit(uint8_t num)
{
    static uint8_t ten, one;
    
   // 提取各位数字
   
    ten = num  / 10;
    one = num % 10;
    
    // 写入十位（最左边�?
    
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Number_Table[ten]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Number_Table[one]);

	#if 0 //WT.EDIT 2026.03.09
        
    // 写入个位（最右边 N)
	
	 TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,TM1639_CHAR_N);
	#else
	    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,0x00);
	#endif  
	 
}

/**
 * @brief  显示带小数点的数�?
 * @param  num: 要显示的数字(0-999)
 * @param  dot_pos: 小数点位�?(0-2)�?0表示第一位数字后的小数点
 * @retval None
 */
void TM1639_Display_Decimal(uint16_t num, uint8_t dot_pos)
{
    uint8_t hundred, ten, one;
    
    if(num > 999) num = 999;
    if(dot_pos > 2) dot_pos = 2;
    
    // 提取各位数字
    hundred = num / 100;
    ten = (num % 100) / 10;
    one = num % 10;
    
    // 写入百位（可能带小数点）
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L,
        TM1639_Number_Table[hundred] | (dot_pos == 0 ? TM1639_DOT : 0));
    
    // 写入十位（可能带小数点）
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L,
        TM1639_Number_Table[ten] | (dot_pos == 1 ? TM1639_DOT : 0));
    
    // 写入个位（可能带小数点）
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,
        TM1639_Number_Table[one] | (dot_pos == 2 ? TM1639_DOT : 0));
}

/**
 * @brief  显示温度�?
 * @param  temp: 温度值（-9�?99℃）
 * @retval None
 */
void TM1639_Display_Temperature(int8_t temp)
{

        // 显示十位
       if(temp >= 10){
	   	     
            TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L,TM1639_Number_Table[temp / 10] );
       	}
       else{
	   	  
            TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L,TM1639_Number_Table[0]);
       	}
        
        // 显示个位
       TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L,TM1639_Number_Table[temp % 10] | TM1639_DOT);
        
        // 显示度数符号
       //TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_CHAR_DEGREE);
        //显示小数点�?��?��?? 显示数字�?0�?
       TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,TM1639_Number_Table[0]);
}
/**
 * @brief  显示湿度�?
 * @param  humi: 湿度值（0-99%RH�?
 * @retval None
 */
void TM1639_Display_Humidity(uint8_t humi)
{
    if(humi > 99) humi = 99;

    // 显示十位
    if(humi >= 10){
        TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 
            TM1639_Number_Table[humi / 10]);
    }
    else
        TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Number_Table[0]);
    
    // 显示个位带小数点
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L,TM1639_Number_Table[humi % 10] | TM1639_DOT);
    
    // 显示RH符号
    //TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_CHAR_RH);
    //显示小数点�?��?��?? + 数字 �?0�?
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_Number_Table[0]);
}

/**
 * @brief  清空显示
 * @param  None
 * @retval None
 */
void TM1639_Clear(void)
{
    // 清空�?有显示位
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, 0x00);
}

/**
 * @brief  在指定位置显示字母H
 * @param  position: 显示位置(0-2)�?0为最左边
 * @retval None
 */
void TM1639_Display_H(uint8_t position)
{
 
    
    if(position > 2) position = 2;
    
    TM1639_Start();
    TM1639_Write_Byte(position); // 设置显示位置
    TM1639_Write_Byte(TM1639_CHAR_H);              // 写入字母H的段�?
    TM1639_Stop();
}

/**
 * @brief  关闭�?有显示（包括数码管和LED�?
 * @param  None
 * @retval None
 */
void TM1639_All_Off(void)
{
    // 关闭数码管显示（GRID1-GRID3�?
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, 0x00);
    
    // 关闭LED显示（GRID4-GRID8�?
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID4_H, TM1639_ADDR_GRID4_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L, 0x00);
    
    // 关闭显示
    TM1639_Display_ON_OFF(TM1639_DISPLAY_OFF);
}

/**
 * @brief  显示小数�?
 * @param  num: 
 * @retval None
 */

void TM1639_Write_2bit_SetUp_TempData(uint8_t onebit,uint8_t twobit,uint8_t sel)
{

	
	 TM1639_STB_SetLow();
	 TM1639_Write_Byte(0x40);//To write display register 0x40
	 TM1639_STB_SetHigh();


	 TM1639_Start();
     TM1639_Write_Byte(0x44);//Add fixed reg
     TM1639_Stop();

	 
	 //digital 1
     
      //TM1639_Write_Byte(0xC0);//0xC4H->GRID7->BIT_1
     if(sel==0){
         //TM1639_Write_OneByte(segNumber_Low[onebit]);//display ""
         TM1639_Write_Digit_Full(TM1639_ADDR_GRID1_H, TM1639_ADDR_GRID1_L,TM1639_Number_Table[onebit]);
     }
     else{
		 //TM1639_Write_Digit_Full(segNumber_Low[0x10]);
		  TM1639_Write_Digit_Full(TM1639_ADDR_GRID1_H, TM1639_ADDR_GRID1_L,TM1639_Number_Table[0x0A]);

	 }
    

	 

     //digital 2

    //  TM1639_Write_Byte(AddrC2H);//0xC7H->GRID8->BIT_2
     if(sel==0){
     	// TM1639_Write_Byte(segNumber_Low[twobit]);//display ""
     	TM1639_Write_Digit_Full(TM1639_ADDR_GRID2_H, TM1639_ADDR_GRID2_L,TM1639_Number_Table[twobit]|seg_h);
     }
     else{
	      //TM1639_Write_Byte(segNumber_Low[0x10]);
	      
	    TM1639_Write_Digit_Full(TM1639_ADDR_GRID2_H, TM1639_ADDR_GRID2_L,TM1639_Number_Table[0X0A]);


	 }
     

	

    
    //open diplay
    TM1639_Start();
     TM1639_Write_Byte(OpenDispTM1639|0x8f);//
    TM1639_Stop();
    
}


/*******************************************************************************************************
    *
    *Function Name:void TM1640_Write_4Bit_Data(uint8_t onebit,uint8_t twobit,uint8_t threebit,uint8_t fourbit)
    *Function :Smg display times hour minute
    *Input Ref: onebit ,twobit hours ,threebit fourbit minute,sl -select "H" or "numbers"
    *Return Ref: NO
    *
********************************************************************************************************/
void TM1639_Write_4Bit_Time(uint8_t onebit,uint8_t twobit,uint8_t threebit,uint8_t fourbit,uint8_t sl)
{

	
	 TM1639_STB_SetLow();
	  TM1639_Write_Byte(0X40);//To Address of fixed reg 0x44
	 TM1639_STB_SetHigh();
    
    TM1639_STB_SetLow();
      TM1639_Write_Byte(0X44);//To Address of fixed reg 0x44
     TM1639_STB_SetHigh();

	 
    //digital 1
     TM1639_Start();
     //TM1639_Write_OneByte(0xC8);//0xC0H->GRID_1->BIT_1
    if(sl ==0){
         TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L,TM1639_Number_Table[onebit]);//TM1639_Write_OneByte(segNumber_Low_4bit[onebit]);//display "10"
      }
     else{
	 	   
          TM1639_Write_Digit_Full(TM1639_ADDR_GRID5_H, TM1639_ADDR_GRID5_L,TM1639_Number_Table[0x0A]);   //TM1639_Write_OneByte(segNumber_Low_4bit[0x10]);//display "10"
	 	  
	 }
//     TM1639_Stop();
//    // ai_ico_fast_blink();

//	 TM1639_Start();
//     TM1639_Write_OneByte(0XC9);//0xC1H->GRID_1->BIT_1
//     if(sl ==0){
//         TM1639_Write_OneByte(segNumber_High_4bit[onebit]);//display "01"
//     }
//     else {
//	 	     TM1639_Write_OneByte(segNumber_High_4bit[0x10]);//display "10"
//     }
//     TM1639_Stop();
    
     // ai_ico_fast_blink();
     //dighital 2
   
    // TM1639_Start();
    // TM1639_Write_OneByte(0xCA);//0xC1H->GRID_2->BIT_2
     if(sl==0){
	 	if(gpro_t.g_time_disp_colon_flag== true){
         TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L,TM1639_Number_Table[twobit|seg_h]); //TM1639_Write_OneByte(segNumber_Low_4bit[twobit]);//display "2 :"
	 	}
		else{
		  TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L,TM1639_Number_Table[twobit]);

		}
	}
     else {
	 	  
     	 TM1639_Write_Digit_Full(TM1639_ADDR_GRID6_H, TM1639_ADDR_GRID6_L,TM1639_Number_Table[0x0A]);// TM1639_Write_OneByte(segNumber_Low_4bit[0x10]);
     }
    
  

//	TM1639_Start();

//    TM1639_Write_OneByte(0xCB);//0xC1H->GRID_2->BIT_2
//     if(gpro_t.g_time_disp_colon_flag==1){
//         TM1639_Write_OneByte(segNumber_High_4bit[twobit]|seg_h);//WT.EDIT 2025.03.10
   
//     }
//     else {
      
//        TM1639_Write_OneByte(segNumber_High_4bit[twobit]); //WT.EDIT 2025.03.10
//	 }
	 
//    TM1639_Stop();
	 
 
     //digital 3 
     //minute 
   // TM1639_Start();
    //TM1639_Write_OneByte(0xCC);//0xC2H->GRID_3->BIT_3
    if(sl==0){//TM1639_Write_OneByte(OFFLED);//display "NULL"
	    TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L,TM1639_Number_Table[threebit]);//TM1639_Write_OneByte(segNumber_Low_4bit[threebit]);//display ""

    }
    else{
        TM1639_Write_Digit_Full(TM1639_ADDR_GRID7_H, TM1639_ADDR_GRID7_L,TM1639_Number_Table[0x0A]);//TM1639_Write_OneByte(segNumber_Low_4bit[0x10]);
     }
    //TM1639_Stop();
  
    //minute 
//    TM1639_Start();
//    TM1639_Write_OneByte(0xCD);//0xC2H->GRID_3->BIT_3
//    if(gpro_t.g_time_disp_colon_flag==1){
//	    TM1639_Write_OneByte(segNumber_High_4bit[threebit]|seg_h);//display ""

//	}//TM1639_Write_OneByte(OFFLED);//display "NULL"
//    else TM1639_Write_OneByte(segNumber_High_4bit[threebit]); //WT.EDIT 2025.03.10
	
//    TM1639_Stop();
	
   
    //digital 4
	//minute 
   // TM1639_Start();
   // TM1639_Write_OneByte(0xCE);//0xC2H->GRID_4
    if(sl==0){//TM1639_Write_OneByte(OFFLED);//display "NULL"
	     TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L,TM1639_Number_Table[fourbit]);//TM1639_Write_OneByte(segNumber_Low_4bit[fourbit]);//display ""

    }
    else{
		TM1639_Write_Digit_Full(TM1639_ADDR_GRID8_H, TM1639_ADDR_GRID8_L,TM1639_Number_Table[0x0A]);//TM1639_Write_OneByte(segNumber_Low_4bit[0x10]);
    }
    //TM1639_Stop();
  
    //minute 
//    TM1639_Start();
//    TM1639_Write_OneByte(0xCF);//0xC2H->GRID_4
//    if(sl==0){//TM1639_Write_OneByte(OFFLED);//display "NULL"
//	    TM1639_Write_OneByte(segNumber_High_4bit[fourbit]);//display ""

//    }
//    else TM1639_Write_OneByte(segNumber_High_4bit[0x10]);
//    TM1639_Stop();
     //open diplay
    TM1639_Start();
    TM1639_Write_Byte(OpenDispTM1639|0x8f);//0xC2H->GRID3->BIT_3
    TM1639_Stop();
 
    
}

/**********************************************************************
*
*Functin Name: void Display_Timing(uint8_t hours,uint8_t minutes)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_Timing(uint8_t hours,uint8_t minutes,uint8_t disp)
{ 
    static uint8_t m,q;
	m = hours /10 ;
	gpro_t.hours_two_unit_bit =	hours%10; 
	gpro_t.minutes_one_decade_bit= minutes/10 ;
	q=  minutes%10;
	TM1639_Write_4Bit_Time(m,gpro_t.hours_two_unit_bit,gpro_t.minutes_one_decade_bit,q,disp) ; //timer is default 12 hours "12:00"


}

