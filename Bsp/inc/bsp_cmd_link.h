#ifndef __BSP_CMD_LINK_H_
#define __BSP_CMD_LINK_H_
#include "main.h"

#define MAX_BUFFER_SIZE  12


#define FRAME_DISPLAY_ID      0XA5
#define FRAME_DISPLAY_CODE    0X02



extern uint8_t  inputCmd[30];
extern uint8_t outputBuf[MAX_BUFFER_SIZE];




void send_usart1_data(const uint8_t *pdata,uint8_t length);


void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data);


void SendWifiData_To_Data(uint8_t cmd,uint8_t data);
void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data);

void SendData_Set_Command(uint8_t cmd,uint8_t data);

void SendWifiData_olderCmd(uint8_t cmd,uint8_t data);//only send ox1F

void sendData_to_threeData(uint8_t cmd,uint8_t data1,uint8_t data2,uint8_t data3);

#endif 

