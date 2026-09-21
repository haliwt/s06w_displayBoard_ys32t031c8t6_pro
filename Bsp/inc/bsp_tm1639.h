#ifndef __BSP_TM1639_H
#define __BSP_TM1639_H
#include "ys32t031.h"
#include "main.h"
#include "gpio.h"

// TM1639 引脚定义

// TM1639 引脚擝作宝定�?
#define TM1639_CLK_SetHigh()          do{MCU_STB_GPIO_Port-> BSRR =LL_MCU_CLK_Pin ; }while(0)//HAL_GPIO_WritePin(TM1639_GPIO, TM1639_CLK_PIN, GPIO_PIN_SET)
#define TM1639_CLK_SetLow()           do{MCU_STB_GPIO_Port-> BSRR =(uint32_t)LL_MCU_CLK_Pin <<16 ;}while(0)//HAL_GPIO_WritePin(TM1639_GPIO, TM1639_CLK_PIN, GPIO_PIN_RESET)

#define TM1639_DIO_SetHigh()          do{MCU_DIO_GPIO_Port-> BSRR =LL_MCU_DIO_Pin;}while(0)//HAL_GPIO_WritePin(TM1639_GPIO, TM1639_DIO_PIN, GPIO_PIN_SET)
#define TM1639_DIO_SetLow()           do{MCU_DIO_GPIO_Port-> BSRR =(uint32_t)LL_MCU_DIO_Pin<<16;}while(0)//HAL_GPIO_WritePin(TM1639_GPIO, TM1639_DIO_PIN, GPIO_PIN_RESET)

#define TM1639_STB_SetHigh()          do{MCU_STB_GPIO_Port-> BSRR =LL_MCU_STB_Pin;}while(0)//HAL_GPIO_WritePin(TM1639_GPIO, TM1639_STB_PIN, GPIO_PIN_SET)
#define TM1639_STB_SetLow()           do{MCU_STB_GPIO_Port-> BSRR =(uint32_t)LL_MCU_STB_Pin<<16;}while(0)//HAL_GPIO_WritePin(TM1639_GPIO, TM1639_STB_PIN, GPIO_PIN_RESET)

// TM1639 命令定义
#define TM1639_CMD_DATA               0x40    // 数杮命令
#define TM1639_CMD_DISPLAY            0x88    // 显示控制命令
#define TM1639_DONOT_DISPLAY          0x80    //  关闭显示

// TM1639 显示地址定义（毝个数字需覝高4佝和�?4佝两个地�?�?
// �?1佝数砝管（最左）
#define TM1639_ADDR_DIG1_H           0xC0    // �?4佝地�?
#define TM1639_ADDR_DIG1_L           0xC1    // �?4佝地�?
// �?2佝数砝管（中间）
#define TM1639_ADDR_DIG2_H           0xC2    // �?4佝地�?
#define TM1639_ADDR_DIG2_L           0xC3    // �?4佝地�?
// �?3佝数砝管（最坳）
#define TM1639_ADDR_DIG3_H           0xC4    //  �?4佝地�?
#define TM1639_ADDR_DIG3_L           0xC5    // �?4佝地�?

// TM1639 显示模弝
#define TM1639_DISPLAY_ON            0x08    // 显示�?
#define TM1639_DISPLAY_OFF           0x00    // 显示�?

// TM1639 亮度级别 (0-7)
#define TM1639_BRIGHTNESS_MAX        0x07    // �?大亮�? //WT.EDIT 2025.04.23
#define TM1639_BRIGHTNESS_MIN        0x00    // �?尝亮�?

// 特殊字符段砝定义
//#define TM1639_CHAR_H               0x76    // H: 0111 0110 (b,c,e,f,g)
//#define TM1639_CHAR_DEGREE          0x63    // 度数符坷 (b,c,g)
//#define TM1639_CHAR_C               0x39    // 摄氝度C (a,d,e,f)
//#define TM1639_CHAR_RH              0x50    // 湿度符坷RH的H部分 (e,g)
//#define TM1639_DOT                  0x80    // 尝数�? (dp)

#define SEG_A                       0x01
#define SEG_B                       0x02
#define SEG_C                       0x04
#define SEG_D                       0x08
#define SEG_E                       0x10
#define SEG_F                       0x20
#define SEG_G                       0x40
#define SEG_H                       0x80


// TM1639 LED显示地址定义（GRID4-GRID8）
 //Display Address 
#define TM1639_ADDR_GRID1_H 		0xC0//??00H
#define TM1639_ADDR_GRID1_L  		0xC1//??01H

#define  TM1639_ADDR_GRID2_H   		0xC2//??02H
#define  TM1639_ADDR_GRID2_L   		0xC3//??03H
//digital 3
#define TM1639_ADDR_GRID3_H  		0xC4//??03H
#define TM1639_ADDR_GRID3_L 		0xC5//??03H

//digital 4

#define TM1639_ADDR_GRID4_H           0xC6    // GRID4 高4位地址
#define TM1639_ADDR_GRID4_L           0xC7    // GRID4 低4位地址
//digital 5

#define TM1639_ADDR_GRID5_H           0xC8    // GRID5 高4位地址
#define TM1639_ADDR_GRID5_L           0xC9    // GRID5 低4位地址

#define TM1639_ADDR_GRID6_H           0xCA    // GRID6 高4位地址
#define TM1639_ADDR_GRID6_L           0xCB    // GRID6 低4位地址

#define TM1639_ADDR_GRID7_H           0xCC    // GRID7 高4位地址
#define TM1639_ADDR_GRID7_L           0xCD    // GRID7 低4位地址

#define TM1639_ADDR_GRID8_H           0xCE    // GRID8 高4位地址
#define TM1639_ADDR_GRID8_L           0xCF    // GRID8 低4位地址

#define   seg_h  0x08      //SEG7 -> ":"

#define OpenDispTM1639          0x88// 



// 函数声明
void TM1639_Init(void);                      // 初始化TM1639
void TM1639_Display_3_Digit(uint8_t num);   // 显示3佝数�?
void TM1639_Set_Brightness(uint8_t bright);  // 设置亮度
void TM1639_Display_ON_OFF(uint8_t status);  // 显示�?关控�?
void TM1639_Clear(void);                     // 清空显示
void TM1639_Display_H(uint8_t position);     // 在指定佝置显示字毝H
void TM1639_Display_Temperature(int8_t temp); // 显示温度值（-9�?99℃）
void TM1639_Display_Humidity(uint8_t humi);  // 显示湿度值（0-99%RH�?
void TM1639_Display_Decimal(uint16_t num, uint8_t dot_pos); // 显示带尝数点的数�?
void TM1639_Write_Digit_Full(uint8_t addr_h, uint8_t addr_l, uint8_t data); // 写入完整的一佝数砝管
void TM1639_All_Off(void);                  // 关闭�?有显示（包括数砝管和LED�?

//void TM1639_Write_Half_Digit(uint8_t addr,uint8_t data);


void TM1639_Display_setTimerHours_3_Digit(uint8_t num);

void TM1639_Display_setTimerMinutes_3_Digit(uint8_t num);

void TM1639_Write_2bit_SetUp_TempData(uint8_t onebit,uint8_t twobit,uint8_t sel);

void Display_Timing(uint8_t hours,uint8_t minutes,uint8_t disp);



#endif /* __BSP_TM1639_H_ */




