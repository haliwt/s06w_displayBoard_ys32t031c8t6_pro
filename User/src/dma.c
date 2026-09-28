#include "dma.h"
#include "bsp.h"


// DMA 初始化配置
uint8_t BUFFER[BUF_SIZE];


// DMA 初始化配置
void DMA_Configuration()
{
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA);
  
  //LL_DMA_DisableChannel(DMA, LL_DMA_CHANNEL_1);
  //LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_UART1_RX);

  LL_DMA_DisableChannel(DMA, LL_DMA_CHANNEL_2);
  LL_SYSCFG_SetDMARemap_CH2(LL_SYSCFG_DMA_MAP_UART1_TX);
}


/************************************************
函数名称 ： UART_TX_DMA_CH2
功    能 ： 串口1DMA 通道2发送配置
参    数 ： NULL
返 回 值 ： 无
*************************************************/
void UART1_TX_RX_DMA_Init(void)
{
    LL_DMA_InitTypeDef DMA_InitStructure;
        
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);                           //打开SYS时钟
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA);                              //打开DMA时钟

  //  LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_1);
    LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_2);

  //  while( ((DMA_CHANNEL1 -> CCR) & 0x1) != 0x00);
    while( ((DMA_CHANNEL2 -> CCR) & 0x1) != 0x00);


    DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&UART1->TDR;                  //指定DMA搬移对应的目的外设地址
    DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t)&outputBuf[0];                   //指定DMA搬移对应的源内存地址
    DMA_InitStructure.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;      //指定DMA搬移方向:内存->外设
    DMA_InitStructure.Mode                   = LL_DMA_MODE_NORMAL;                     //普通模式
    DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;              //目的外设地址不自增
    DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;                //源内存地址自增
    DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;                 //指定搬移的数据块为1字节
    DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;                 //指定搬移的数据块为1字节
    DMA_InitStructure.NbData                 = sizeof(outputBuf);                         //指定DMA缓冲区大小
    DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;                   //指定高优先级
    LL_DMA_Init(DMA, LL_DMA_CHANNEL_2, &DMA_InitStructure);                            //配置DMA通道2
    #if 0
    DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&UART1->RDR;                  //指定DMA搬移对应的源外设地址
    DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t)&BUFFER[0];                   //指定DMA搬移对应的目标内存地址
    DMA_InitStructure.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;      //指定DMA搬移方向:外设->内存
    DMA_InitStructure.Mode                   = LL_DMA_MODE_NORMAL;                     //普通模式
    DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;              //源外设地址不自增
    DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;                //目标内存地址自增
    DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;                 //指定搬移的数据块为1字节
    DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;                 //指定搬移的数据块为1字节
    DMA_InitStructure.NbData                 = sizeof(BUFFER);                         //指定DMA缓冲区大小
    DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;                   //指定高优先级
    LL_DMA_Init(DMA, LL_DMA_CHANNEL_1, &DMA_InitStructure);                            //配置DMA通道1


     LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_UART1_RX);                          //映射DMA通道3作为串口1的接收功能
   #endif 
	 LL_SYSCFG_SetDMARemap_CH2(LL_SYSCFG_DMA_MAP_UART1_TX);                          //映射DMA通道2作为串口1的发送功能
   
    
    //LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_1);
    LL_DMA_EnableChannel(DMA,LL_DMA_CHANNEL_2);

}

/**
  * @brief  UART1 DMA 发送函数（非阻塞）
  * @param  pData: 待发送的数据缓冲区指针
  * @param  Size:  发送数据长度
  */
void UART1_DMA_Disp_Send(const uint8_t *pData, uint16_t Size)
{

  /* 1. 安全边界检查：指针为空或长度为0时直接退出，拒绝非法非法操作 */
  if (pData == NULL || Size == 0) return ;
  /* 1. 等待上一次 DMA 发送完成（如果通道还开启着，说明还没发完） */
 // while(LL_DMA_IsActiveFlag_TC2(DMA)!= SET);  //wait transmit finish     
  while(LL_DMA_IsEnabledChannel(DMA, LL_DMA_CHANNEL_2) && LL_DMA_GetDataLength(DMA,LL_DMA_CHANNEL_2) > 0);


  LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_2);
  /* 2. 清除通道 2 的传输完成标志位 */
  LL_DMA_ClearFlag_TC2(DMA);

  /* 3. 动态重新配置内存地址和数据长度 */
  LL_DMA_SetMemoryAddress(DMA, LL_DMA_CHANNEL_2, (uint32_t)pData);
  LL_DMA_SetDataLength(DMA, LL_DMA_CHANNEL_2, Size);

  /* 4. 使能 DMA 通道，立刻启动硬件级发送 */
  LL_DMA_EnableChannel(DMA, LL_DMA_CHANNEL_2);
}


