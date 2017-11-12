#ifndef _LED_H
#define _LED_H

//==================================================
// 文件包含
//==================================================
#include "config.h"

//==================================================
// 宏定义
//==================================================
#define LED_PORT P1
sbit DIO = LED_PORT^0;			//serial data transmission
sbit RCK = LED_PORT^1;			//clock signal
sbit SCK = LED_PORT^2;			//enable signal

//==================================================
// 函数声明
//==================================================
void led_display(uchar *led_buff);

#endif