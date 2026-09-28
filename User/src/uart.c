/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    uart.C
  * @author  Yspring Firmware Team  
  * @brief   uart Source Code.
  ******************************************************************************      
*/

#include "uart.h"   
#include "ys32t031.h"
#include "bsp.h"



// UART1 初始化配置
void UART1_Configuration(void)
{
  LL_UART_InitTypeDef UART_InitStructure= {0};

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_UART1);

  LL_UART_DeInit(UART1);
  LL_UART_StructInit(&UART_InitStructure);
  UART_InitStructure.BaudRate = 9600;
  UART_InitStructure.DataWidth = LL_UART_DATAWIDTH_8B;
  UART_InitStructure.StopBits = LL_UART_STOPBITS_1;
  UART_InitStructure.Parity = LL_UART_PARITY_NONE;
  UART_InitStructure.TransferDirection = LL_UART_DIRECTION_TX_RX;
  LL_UART_Init(UART1, &UART_InitStructure);

   LL_UART_EnableDMAReq_TX(UART1);
  
   LL_UART_EnableIT_RXNE(UART1); ////使能UART1的接收中断
   LL_UART_EnableIT_TC(UART1);
   
    LL_UART_Enable(UART1);
}


#if 0
/*********************************************************************************
* Function    : UART Configuration
* Description : Configuration UART
* Parameter   ：Baudrate
* Retval      : NONE
**********************************************************************************/
void UART_Configuration(uint32_t Baudrate)
{
    LL_UART_InitTypeDef UART_InitStructure= {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_RCC_ClocksTypeDef RCC_CLOCKS = {0};
    
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_UART1); 
    
    LL_UART_DeInit(UART1);    
    /******************************************************************
                  CONFIG PA9 AS UART TX PIN
    *******************************************************************/
    GPIO_InitStruct.Pin 				= LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode 				= LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed 			    = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType 	        = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull 				= LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate 	        = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /******************************************************************
                  CONFIG PA10 AS UART RX PIN
    *******************************************************************/
    GPIO_InitStruct.Pin 				= LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode 				= LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed 			    = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType 	        = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull 				= LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate 	        = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /***************************************************************************
    UART Parameter Initialization:  BaudRate     DataWidth   StopBit  ParityBit
                                    115200         8         1        0(NO)
    ***************************************************************************/
    UART_InitStructure.BaudRate = Baudrate;                       
    UART_InitStructure.DataWidth = LL_UART_DATAWIDTH_8B;        
    UART_InitStructure.StopBits = LL_UART_STOPBITS_1;             
    UART_InitStructure.Parity = LL_UART_PARITY_NONE ;               
    UART_InitStructure.TransferDirection = LL_UART_DIRECTION_TX_RX;    
    LL_UART_Init(UART1, &UART_InitStructure);                          

    LL_UART_Enable(UART1); 
    
    LL_RCC_GetSystemClocksFreq(&RCC_CLOCKS);
 
    HCLK = (RCC_CLOCKS.HCLK_Frequency)/Freq_M;
    PCLK = (RCC_CLOCKS.PCLK_Frequency)/Freq_M;
    
    printf("MCU is running, HCLK=%dMHz, PCLK=%dMHz\n",HCLK,PCLK);
    
    LL_UART_EnableDMAReq_RX(UART1);
    LL_UART_EnableDMAReq_TX(UART1);

    
    NVIC_SetPriority(UART1_IRQn, 3);
    NVIC_EnableIRQ(UART1_IRQn); //配置NVIC支持UART1
	
	LL_UART_DisableIT_RXNE(UART1); //使能UART1的接收中断
	LL_UART_EnableIT_IDLE(UART1); //使能UART1的空闲中断
    LL_UART_ClearFlag_IDLE(UART1);

    

}

#endif 

void UART1_Int_Call(void)
{

  volatile uint8_t data ;


  #if 1
  if(LL_UART_IsActiveFlag_RXNE(UART1)&& LL_UART_IsEnabledIT_RXNE(UART1))
  {
    /* USER CODE BEGIN Code_UART1_Int_Call_UART_FLAG_RXNE */
     // UART_ReceiveData(UART1);
     data = LL_UART_ReceiveData8(UART1);
	 usart1_isr_callback_handler(data);
    
    /* USER CODE END Code_UART1_Int_Call_UART_FLAG_RXNE */
  }

  if(LL_UART_IsActiveFlag_ORE(UART1)){

     volatile uint32_t dummy_read = UART1->RDR;

      ((void)dummy_read);

  }

  if (LL_UART_IsActiveFlag_TC(UART1) && LL_UART_IsEnabledIT_TC(UART1))
  {
    /* USER CODE BEGIN Code_UART1_Int_Call_UART_FLAG_TC */
    
    /* USER CODE END Code_UART1_Int_Call_UART_FLAG_TC */
    LL_UART_ClearFlag_TC(UART1);
  }

  UART1->ICR = 0xFF;

  #else 
   if(LL_UART_IsActiveFlag_IDLE(UART1) && LL_UART_IsEnabledIT_IDLE(UART1))
    {
        LL_UART_ClearFlag_IDLE(UART1);
        LL_UART_ClearFlag_RTO(UART1);
       data = LL_UART_ReceiveData8(UART1);
	    usart1_isr_callback_handler(data);

   	}



  #endif 
}








#if 0
/**
  * @brief  UART2 DMA 发送函数（非阻塞）
  */
void UART2_DMA_Wifi_Send(const uint8_t *pData, uint16_t Size)
{

  /* 1. 安全边界检查：指针为空或长度为0时直接退出，拒绝非法非法操作 */
  if (pData == NULL || Size == 0) return ;
  
  while(LL_DMA_IsEnabledChannel(DMA, LL_DMA_CHANNEL_2));
  LL_DMA_ClearFlag_TC2(DMA);

  LL_DMA_SetMemoryAddress(DMA, LL_DMA_CHANNEL_2, (uint32_t)pData);
  LL_DMA_SetDataLength(DMA, LL_DMA_CHANNEL_2, Size);

  LL_DMA_EnableChannel(DMA, LL_DMA_CHANNEL_2);
}


#endif 




/*********************************************************************************
* Function    : Fputc
* Description : Remapping Printf
* Parameter   £ºCharacter
* Retval      : Character
**********************************************************************************/
int fputc(int ch, FILE *f)
{
    LL_UART_TransmitData8(UART1, ch);
    while(LL_UART_IsActiveFlag_TC(UART1) == RESET);    
    return ch;
}



// 自己写一个安全的、绝对不依赖 C 库的字符发送函数
void My_UART_SendChar(uint8_t ch) 
{
    // 只有时钟开启且寄存器有效时，这样写才安全
    // 如果怕卡死，这里甚至不需要等
    UART1->TDR = ch; 
}



