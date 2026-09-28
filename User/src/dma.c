#include "dma.h"
#include "bsp.h"


// DMA 初始化配置
uint8_t BUFFER[BUF_SIZE];


// DMA 初始化配置
void DMA_Configuration()
{
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA);
  LL_DMA_DisableChannel(DMA, LL_DMA_CHANNEL_1);
  LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_UART1_RX);

  LL_DMA_DisableChannel(DMA, LL_DMA_CHANNEL_2);
  LL_SYSCFG_SetDMARemap_CH2(LL_SYSCFG_DMA_MAP_UART1_TX);
}
void LL_DMA_Configuration_Channel1(uint32_t MemoryOrDstAddr, uint32_t PeriphOrSrcAddr, uint16_t BufferSize)
{
  LL_DMA_InitTypeDef DMA_InitStruct;
  LL_DMA_StructInit(&DMA_InitStruct);
  DMA_InitStruct.PeriphOrM2MSrcAddress = PeriphOrSrcAddr;
  DMA_InitStruct.MemoryOrM2MDstAddress = MemoryOrDstAddr;
  DMA_InitStruct.NbData = BufferSize;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_LOW;
  LL_DMA_Init(DMA, LL_DMA_CHANNEL_1, &DMA_InitStruct);

  LL_DMA_EnableChannel(DMA, LL_DMA_CHANNEL_1);
}

void LL_DMA_Configuration_Channel2(uint32_t MemoryOrDstAddr, uint32_t PeriphOrSrcAddr, uint16_t BufferSize)
{
  LL_DMA_InitTypeDef DMA_InitStruct;
  LL_DMA_StructInit(&DMA_InitStruct);
  DMA_InitStruct.PeriphOrM2MSrcAddress = PeriphOrSrcAddr;
  DMA_InitStruct.MemoryOrM2MDstAddress = MemoryOrDstAddr;
  DMA_InitStruct.NbData = BufferSize;
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_LOW;
  LL_DMA_Init(DMA, LL_DMA_CHANNEL_2, &DMA_InitStruct);

  LL_DMA_EnableChannel(DMA, LL_DMA_CHANNEL_2);
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
        
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);                           //´ò¿ªSYSÊ±ÖÓ
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA);                              //´ò¿ªDMAÊ±ÖÓ

    LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_2);
  //  LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_3);

    while( ((DMA_CHANNEL2 -> CCR) & 0x1) != 0x00);
    while( ((DMA_CHANNEL1 -> CCR) & 0x1) != 0x00);


    DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&UART1->TDR;                  //Ö¸¶¨DMA°áÒÆ¶ÔÓ¦µÄÄ¿µÄÍâÉèµØÖ·
    DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t)&outputBuf[0];                   //Ö¸¶¨DMA°áÒÆ¶ÔÓ¦µÄÔ´ÄÚ´æµØÖ·
    DMA_InitStructure.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;      //Ö¸¶¨DMA°áÒÆ·½Ïò:ÄÚ´æ->ÍâÉè
    DMA_InitStructure.Mode                   = LL_DMA_MODE_NORMAL;                     //ÆÕÍ¨Ä£Ê½
    DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;              //Ä¿µÄÍâÉèµØÖ·²»×ÔÔö
    DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;                //Ô´ÄÚ´æµØÖ·×ÔÔö
    DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;                 //Ö¸¶¨°áÒÆµÄÊý¾Ý¿éÎª1×Ö½Ú
    DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;                 //Ö¸¶¨°áÒÆµÄÊý¾Ý¿éÎª1×Ö½Ú
    DMA_InitStructure.NbData                 = sizeof(BUFFER);                         //Ö¸¶¨DMA»º³åÇø´óÐ¡
    DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;                   //Ö¸¶¨¸ßÓÅÏÈ¼¶
    LL_DMA_Init(DMA, LL_DMA_CHANNEL_2, &DMA_InitStructure);                            //ÅäÖÃDMAÍ¨µÀ2
 #if 0      
    DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&UART1->RDR;                  //Ö¸¶¨DMA°áÒÆ¶ÔÓ¦µÄÔ´ÍâÉèµØÖ·
    DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t)&BUFFER[0];                   //Ö¸¶¨DMA°áÒÆ¶ÔÓ¦µÄÄ¿±êÄÚ´æµØÖ·
    DMA_InitStructure.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;      //Ö¸¶¨DMA°áÒÆ·½Ïò:ÍâÉè->ÄÚ´æ
    DMA_InitStructure.Mode                   = LL_DMA_MODE_NORMAL;                     //ÆÕÍ¨Ä£Ê½
    DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;              //Ô´ÍâÉèµØÖ·²»×ÔÔö
    DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;                //Ä¿±êÄÚ´æµØÖ·×ÔÔö
    DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;                 //Ö¸¶¨°áÒÆµÄÊý¾Ý¿éÎª1×Ö½Ú
    DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;                 //Ö¸¶¨°áÒÆµÄÊý¾Ý¿éÎª1×Ö½Ú
    DMA_InitStructure.NbData                 = sizeof(BUFFER);                         //Ö¸¶¨DMA»º³åÇø´óÐ¡
    DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;                   //Ö¸¶¨¸ßÓÅÏÈ¼¶
    LL_DMA_Init(DMA, LL_DMA_CHANNEL_1, &DMA_InitStructure);                            //ÅäÖÃDMAÍ¨µÀ3
  #endif 
    LL_SYSCFG_SetDMARemap_CH2(LL_SYSCFG_DMA_MAP_UART1_TX);                          //Ó³ÉäDMAÍ¨µÀ2×÷Îª´®¿Ú1µÄ·¢ËÍ¹¦ÄÜ
   // LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_UART1_RX);                          //Ó³ÉäDMAÍ¨µÀ3×÷Îª´®¿Ú1µÄ½ÓÊÕ¹¦ÄÜ
    
    //LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_2);
	 LL_DMA_EnableChannel(DMA,LL_DMA_CHANNEL_2);
  //  LL_DMA_EnableChannel(DMA,LL_DMA_CHANNEL_1);

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
 // while(LL_DMA_IsEnabledChannel(DMA, LL_DMA_CHANNEL_2) && LL_DMA_GetDataLength(DMA,LL_DMA_CHANNEL_2) > 0);


  LL_DMA_DisableChannel(DMA,LL_DMA_CHANNEL_2);
  /* 2. 清除通道 2 的传输完成标志位 */
  LL_DMA_ClearFlag_TC2(DMA);

  /* 3. 动态重新配置内存地址和数据长度 */
  LL_DMA_SetMemoryAddress(DMA, LL_DMA_CHANNEL_2, (uint32_t)pData);
  LL_DMA_SetDataLength(DMA, LL_DMA_CHANNEL_2, Size);

  /* 4. 使能 DMA 通道，立刻启动硬件级发送 */
  LL_DMA_EnableChannel(DMA, LL_DMA_CHANNEL_2);
}


