#ifndef _I2C1602_H
#define _I2C1602_H

//==================================================
// �ļ�����
//==================================================
#include "config.h"

//==================================================
// �궨��
//==================================================
#define SLAVE_ADDR	0x4E
#define I2C_PORT		P1

//==================================================
// ���Ŷ���
//==================================================
sbit SCL = I2C_PORT^7;
sbit SDA = I2C_PORT^6;

//==================================================
// �����
//==================================================
#define DISPLAY_MODE	0x38
#define DISPLAY_ON		0x0C
#define SHOW_CURSOR		0x06
#define CLEAR_SCREEN	0x01
#define DISPLAY_OFF		0x08

//==================================================
// ��������
//==================================================
void lcd_init(void);
void lcd_putchar(uchar row,uchar col,uchar ch);
void lcd_display(uchar line,uchar *string);
void lcd_clear(void);

#endif