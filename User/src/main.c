/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    s06w display board
  * @author  
  * @version 1.0.0
  * @date    2026.09.17
  * @brief  
  *
  * 
  *        
  *
  * 
  *         
  * 
  *         
  *
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* USER CODE END header */
#include "ys32t031.h"
#include "main.h"
//#include "delay.h"

#include "uart.h"  
#include "tim.h"
#include "iwdg.h"
#include "adc.h"
#include "gpio.h"
#include "dma.h"
#include "system_ys32t031.h"





#include "bsp.h"

#include "tx_api.h"

///* ThreadX 强制要求的应用定义入口 */
//void tx_application_define(void *first_unused_memory)
//{
//    /* 
//       此时硬件和内核已就绪。
//       你可以在这里调用 tx_thread_create 来创建你的 WiFi 或传感器线程。
//       暂时留空以通过编译。
//    */
//    threadx_handler();
//}


// RCC initialization configuration
void RCC_Configuration(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  // Enable LSI
  LL_RCC_LSI_Enable();

  LL_RCC_HSI_Enable();
  LL_RCC_HSI_SetDiv(LL_RCC_HSI_DIV_1);
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_16MHz);
  while (LL_RCC_HSI_IsReady() != 1);
  LL_Init1msTick(16000000);

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_4);
  LL_RCC_PLL_Enable();
  // while( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  LL_mDelay(1);

  // HCLK = 64 MHz
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  // PCLK = 64 MHz
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  // SYSCLK = 64 MHz
  SystemCoreClockUpdate();

  LL_Init1msTick(64000000);
}






// NVIC 初始化配置
void NVIC_Configuration(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  NVIC_SetPriority(TIM6_LPTIM_IRQn, 0);
  NVIC_EnableIRQ(TIM6_LPTIM_IRQn);

  //NVIC_SetPriority(TIM17_IRQn, 0);
  //NVIC_EnableIRQ(TIM17_IRQn);

  NVIC_SetPriority(UART1_IRQn, 1);
  NVIC_EnableIRQ(UART1_IRQn);
}



/******************************************************
函数名：main
功能：主函数入口
参数：无
返回值：int
*********************** ********************************/
int main(void)
{
    RCC_Configuration();           //系统时钟配置   
	
    GPIO_Configuration();          //IO口配置
	
   Clear_Ram();                   //变量初始化
	
 UART1_Configuration();   //串口1 用于和外接显示板通信
	
	
	
 TIM6_Configuration();          //TIM6基本定时配置
 TIM17_Configuration();
   
	
    IWDG_Configuration();          //独立看门狗配置

    NVIC_Configuration();          //中断嵌套向量配置
		
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		//TSC_StartCmd(ENABLE);          //开始扫描
		bsp_init();
	     UART1_TX_RX_DMA_Init();
		

		
		 tx_kernel_enter(); 
		
		
    while(1)
    {
       
			  
    }
}


			  
			
			


