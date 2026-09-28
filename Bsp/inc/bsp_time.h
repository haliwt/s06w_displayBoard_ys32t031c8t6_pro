#ifndef __BSP_TIME_H
#define __BSP_TIME_H
#include "main.h"


// 定时器结构体
typedef struct {

    bool     g_key_set_timer_flag; 
volatile     bool     g_timer_flag; 
    uint8_t  g_time_hours;    // 当前剩余小时 (0~99)
    uint8_t  g_time_minutes;  // 当前剩余分钟 (0~59)
    uint8_t  g_time_seconds;  // 当前剩余秒数 (0~59)
    bool     g_has_been_key_flag;
    
    uint32_t total_seconds;   // 总剩余秒数 (99小时最大 356,400 秒)
    bool     is_running;      // 运行状态
    bool     is_timeout;      // 定时到达标志
} Timer_TypeDef;

// 实例化全局变量
extern Timer_TypeDef time_t;






#endif 

