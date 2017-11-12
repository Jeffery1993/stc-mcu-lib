#ifndef _INTERRUPT_H
#define _INTERRUPT_H

//==================================================
// 文件包含
//==================================================
#include "config.h"

//==================================================
// 函数声明
//==================================================
void exint0_init(void);
void exint0();			//interrupt 0
void timer0_init(void);
void timer0();			//interrupt 1
void exint1_init(void);
void exint1();			//interrupt 2
void timer1_init(void);
void timer1();			//interrupt 3
void serial();			//interrupt 4
void send_char(uchar ch);
void send_string(uchar ch, uchar *str);
void timer2_init(void);
void timer2();			//interrupt 5
void exint2();			//interrupt 6
void exint3();			//interrupt 7

#endif