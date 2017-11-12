#ifndef _LCD12864_H
#define _LCD12864_H

//==================================================
// 文件包含
//==================================================
#include "config.h"
#include "delay.h"

//==================================================
// 宏定义
//==================================================
#define LCD_DATA	P0
#define LCD_CMD		P3
#define BUSY_BUS	0x80

//==================================================
// 位定义
//==================================================
sbit RE = LCD_CMD^4;
sbit RS = LCD_CMD^5;
sbit RW = LCD_CMD^6;
sbit EN = LCD_CMD^7;

//==================================================
// 函数声明
//==================================================
void lcd_init(void);
void lcd_putchar(uchar row,uchar col,uchar ch);
void lcd_display(uchar line,uchar *string);
void lcd_clear(void);

#endif