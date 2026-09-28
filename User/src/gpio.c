#include "gpio.h"



// GPIO 初始化配置
void GPIO_Configuration(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);

  // GPIO_Output
  LL_GPIO_SetOutputPin(LED_POWER_GPIO_Port, LL_LED_POWER_Pin);
  LL_GPIO_SetOutputPin(LED_MOUSE_GPIO_Port, LL_LED_MOUSE_Pin);
  LL_GPIO_SetOutputPin(LED_PLASMA_GPIO_Port, LL_LED_PLASMA_Pin);
 
  
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Pin = LL_LED_POWER_Pin | LL_LED_MOUSE_Pin | LL_LED_PLASMA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


 // GPIO_Input
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Pin = LL_KEY_POWER_Pin | LL_KEY_MODEL_Pin | LL_KEY_DOWN_Pin | LL_KEY_UP_Pin | LL_KEY_WIFI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(KEY_POWER_GPIO_Port, &GPIO_InitStruct);

  // UART1_TX
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  GPIO_InitStruct.Pin = LL_DSP1_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(DSP1_TX_GPIO_Port, &GPIO_InitStruct);

  // UART1_RX
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  GPIO_InitStruct.Pin = LL_DSP1_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(DSP1_RX_GPIO_Port, &GPIO_InitStruct);

  // GPIO_Input
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Pin = LL_KEY_MOUSE_Pin | LL_KEY_STER_Pin | LL_KEY_DRY_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(KEY_MOUSE_GPIO_Port, &GPIO_InitStruct);

  // GPIO_Output
  LL_GPIO_ResetOutputPin(MCU_CLK_GPIO_Port, LL_MCU_CLK_Pin);
  LL_GPIO_ResetOutputPin(MCU_STB_GPIO_Port, LL_MCU_STB_Pin);
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Pin = LL_MCU_CLK_Pin | LL_MCU_STB_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(MCU_CLK_GPIO_Port, &GPIO_InitStruct);

  // GPIO_Output
  LL_GPIO_ResetOutputPin(LED_TIME_GPIO_Port, LL_LED_TIME_Pin);
  LL_GPIO_SetOutputPin(LED_DRY_GPIO_Port, LL_LED_DRY_Pin);
  LL_GPIO_SetOutputPin(LED_WIFI_GPIO_Port, LL_LED_WIFI_Pin);
  LL_GPIO_ResetOutputPin(MCU_DIO_GPIO_Port, LL_MCU_DIO_Pin);

  
  LL_GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.Pin = LL_LED_TIME_Pin | LL_LED_DRY_Pin | LL_LED_WIFI_Pin | LL_MCU_DIO_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_TIME_GPIO_Port, &GPIO_InitStruct);
}





