#include "bsp.h"



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
    0x00, // 10: null 
};

// 字母和特殊字符显示码
static const uint8_t TM1639_Char_Table[] = {
    0x67, // H: 0111 0110 (hgfe,dcba = 0111 0110)--(abcd efgh =0110 0111)
    0x36, // °: 0110 0011 (hgfe,dcba= 0110  0011)
    0x93, // C: 0011 1001 (hgfe,dcba = 0011 1001)
    0x05,  // RH的H部分: 0101 0000 (hgfe,dcba= 0101 0000)
    0x45   //n:(hgfe dcba= 0101 0100) =  (abcd efgh = 0b0100 0101) //字节序列的排列
};


// 字母和特殊字符显示码
static const uint8_t TM1639_Char_Err_Table[] = {
    0x97, // E: 0111 1001 (b,c,e,f,g)
    0x05, // r: 0101 0000 (b,c,g)
    
  
};


#define TM1639_CHAR_H 					TM1639_Char_Table[0]
#define TM1639_CHAR_DEGREE 				TM1639_Char_Table[1]
#define TM1639_CHAR_C 					TM1639_Char_Table[2]
#define TM1639_CHAR_RH 					TM1639_Char_Table[3]

#define TM1639_CHAR_N                   TM1639_Char_Table[4]

#define TM1639_DOT  0x08 // 小数点段�?,from low position start


static void TM1639_Display_ON_OFF(uint8_t status);
static void TM1639_Set_Brightness(uint8_t bright);  // 设置亮度

/**
 * @brief 针对 64MHz 时钟优化的微秒延时
 * @param 
 * @retval
 */
void Delay_US_dht11(uint32_t us)
{
   #if 0
    // 64MHz 下，1微秒大约需要 64 个周期
    // 扣除函数调用开销，每微秒循环大约 21 次 (21 * 3 = 63 周期)
    volatile uint32_t count = us * 21; 
    while(count--)
    {
        __NOP(); // 插入空指令，防止某些极端编译器行为
    }
	#else 
	// 64MHz 时钟下，1微秒 = 64 个计数节拍 (Ticks)
    uint32_t ticks_to_wait = us * 64; 
    
    // 读取 ThreadX 配置的 SysTick 重载值（比如 1ms 对应的计数值）
    uint32_t reload = SysTick->LOAD; 
    
    uint32_t start_tick = SysTick->VAL;
    uint32_t current_tick;
    uint32_t elapsed_ticks = 0;

    while (elapsed_ticks < ticks_to_wait)
    {
        current_tick = SysTick->VAL;
        
        // 因为 SysTick 是向下计数的 (Decrementing)
        if (current_tick <= start_tick)
        {
            // 正常情况：当前值小于或等于起始值
            elapsed_ticks = start_tick - current_tick;
        }
        else
        {
            // 溢出情况：在读取期间，SysTick 减到 0 并触发了 ThreadX 中断重新加载了
            elapsed_ticks = start_tick + (reload - current_tick);
        }
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
 * @brief  
 * @param  addr_h: �?4位地�?
 * @param  addr_l: �?4位地�?
 * @param  data: 要显示的段码数据
 * @retval None
 */
void tm1639_display_humidity_digit(uint8_t num)
{
    static uint8_t ten, one;
    
   // 提取各位数字
   
    ten = num  / 10;
    one = num % 10;
    
    // 写入十位（最左边�?
    
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Number_Table[ten]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Number_Table[one]|TM1639_DOT);

	 
}

/**
 * @brief  
 * @param  num: 要显示的数字(0-999)
 * @param  dot_pos: 小数点位�?(0-2)�?0表示第一位数字后的小数点
 * @retval None
 */
void tm1639_display_time_digit(uint8_t num)
{
	 static uint8_t ten, one;
    
   // 提取各位数字
   
    ten = num  / 10;
    one = num % 10;
    
    // 写入十位（最左边�?
    
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_Number_Table[ten]|TM1639_DOT);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG4_H, TM1639_ADDR_DIG4_L, TM1639_Number_Table[one]);

}


void tm1639_set_timer_digit(uint8_t num)
{


	///tm1639_display_time_digit(num);

   // tx_thread_sleep(30);
    
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, TM1639_Number_Table[10]|TM1639_DOT);
        
    // 写入十位（中间）
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG4_H, TM1639_ADDR_DIG4_L, TM1639_Number_Table[10]);
	tx_thread_sleep(30);
	tm1639_display_time_digit(num);

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
	TM1639_Write_Digit_Full(TM1639_ADDR_DIG4_H, TM1639_ADDR_DIG4_L, 0x00);

   // turn on display 
    TM1639_Display_ON_OFF(TM1639_DISPLAY_ON);
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
    
    // 关闭显示
    TM1639_Display_ON_OFF(TM1639_DISPLAY_OFF);
}


void TM1639_Display_On(void)
{
    // 关闭数码管显示（GRID1-GRID3�?
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, 0x00);
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L, 0x00);
    
    // 关闭LED显示（GRID4-GRID8�?
    TM1639_Write_Digit_Full(TM1639_ADDR_GRID4_H, TM1639_ADDR_GRID4_L, 0x00);
    
    // 关闭显示
    TM1639_Display_ON_OFF(TM1639_DISPLAY_ON);
}

/**
 * @brief  显示�?关控�?
 * @param  status: 1-�?显示�?0-关显�?
 * @retval None
 */
static void TM1639_Display_ON_OFF(uint8_t status)
{
    TM1639_Start();
    if(status)
        TM1639_Write_Byte(TM1639_CMD_DISPLAY | TM1639_BRIGHTNESS_MAX);
    else
        TM1639_Write_Byte(TM1639_DONOT_DISPLAY);
    TM1639_Stop();
}
/******************************************************************************
	*
	*Function Name:void SMG_Display_Err(uint8_t idata)
	*Funcion: 
	*Input Ref: idata: 1 -ptc warning  2 - fan warning
	*Return Ref:
	*
******************************************************************************/
void SMG_Display_Err(uint8_t idata)
{

    TM1639_Write_Digit_Full(TM1639_ADDR_DIG1_H, TM1639_ADDR_DIG1_L, TM1639_Char_Err_Table[0]);
        
    // 写入十位（中间）
 
    TM1639_Write_Digit_Full(TM1639_ADDR_DIG2_H, TM1639_ADDR_DIG2_L, TM1639_Char_Err_Table[1]| TM1639_DOT);
        
    
     TM1639_Write_Digit_Full(TM1639_ADDR_DIG3_H, TM1639_ADDR_DIG3_L,TM1639_Number_Table[0]|TM1639_DOT);

  
     TM1639_Write_Digit_Full(TM1639_ADDR_DIG4_H, TM1639_ADDR_DIG4_L, TM1639_Number_Table[2] );

}





