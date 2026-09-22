/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    ys32t031_it.c 
  * @author  YSPRING Application Team
  * @version V1.0.0
  * @date    
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ys32t031_it.h"
#include "uart.h"
#include "bsp.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN includes */

/* USER CODE END includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN typedef */

/* USER CODE END typedef */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN define */

/* USER CODE END define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN macro */

/* USER CODE END macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN variables */

/* USER CODE END variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN prototypes */

/* USER CODE END prototypes */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN External variables */

/* USER CODE END External variables */

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NMI_Handler */

  /* USER CODE END NMI_Handler */
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  /* USER CODE BEGIN HardFault_Handler 0 */

  /* USER CODE END HardFault_Handler 0 */
  while (1)
  {
    /* USER CODE BEGIN HardFault_Handler 1 */
    
    /* USER CODE END HardFault_Handler 1 */
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_Handler */

  /* USER CODE END SVC_Handler */
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if 0
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_Handler */
    _tx_thread_context_switch();
  /* USER CODE END PendSV_Handler */
}
#endif 
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#if 0
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_Handler */
    _tx_timer_interrupt();
  /* USER CODE END SysTick_Handler */
}
#endif 


/**
  * @brief  This function handles TIM6_LPTIM_IRQHandler.
  * @param  timer 5ms 
  * @retval None
  */
void TIM6_LPTIM_IRQHandler (void)
{

   
	
	if(LL_TIM_IsActiveFlag_UPDATE(TIM6) == 1 ) 
	{
	 // 2. ???????????????,????????
	  LL_TIM_ClearFlag_UPDATE(TIM6);
       tim6_isr_hander;

	}
}

		
/**
  * @brief  This function handles Uart1 Handler.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
   UART1_Int_Call(); 
}


#if 0
/**
  * @brief  This function handles Uart2 Handler.
  * @param  wifi receive 
  * @retval None
  */
void UART2_IRQHandler(void)
{
	  uint8_t res;
	
    if(UART_GetFlagStatus(UART2, UART_FLAG_RXNE) == SET)
    {
		    UART_ClearFlag(UART2, UART_FLAG_RXNE);

			  res = UART2->RDR;
			  usart2_rx_callback_invoke(res);

		#if 0
			
	      if(uart2_rx_cnt<sizeof(UART2_RX_BUF))
				{
				    UART2_RX_BUF[uart2_rx_cnt++] = res;
				}
				else
				{
				    uart2_rx_cnt = 0;
				}
		#endif 
	}	

    if(UART_GetFlagStatus(UART2, UART_FLAG_TC) == SET)
    {
        UART_ClearFlag(UART2, UART_FLAG_TC);
    }
	 UART_ClearFlag(UART2, UART_FLAG_ORE);
    
    UART2->ICR = 0xFF;  //清除所有中断请求标志				
}



#endif 







