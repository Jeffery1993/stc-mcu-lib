#include "lcd1602.h"

//==================================================
// 函数声明
//==================================================
void write_cmd(uchar cmd);
void write_data(uchar dat);

//==================================================
// 函数功能：初始化
// 输入参数：无
// 输出参数：无
//==================================================
void lcd_init(void)
{
	EN=0;
	write_cmd(0x38);
	write_cmd(0x0c);
	write_cmd(0x06);
	write_cmd(0x01);
}

//==================================================
// 函数功能：显示字符
// 输入参数：字符ch
// 输出参数：无
//==================================================
void lcd_putchar(uchar row,uchar col,uchar ch)
{
	uchar addr;
	if(line==1) addr = 0x80;
	else if(line==2) addr = 0xC0;
	else return;
	
	if(col>=1 && col<=16) addr += col-1;
	else return;
	
	write_cmd(addr);
	write_data(ch);
}

//==================================================
// 函数功能：显示字符串
// 输入参数：字符型line，字符串string
// 输出参数：无
//==================================================
void lcd_display(uchar line,uchar *string)
{
	uchar addr;
	if(line==1) addr = 0x80;
	else if(line==2) addr = 0xC0;
	else return;
	
	write_cmd(addr);
	while(*string!=0)
	{
		write_data(*string++);
		delay_ms(5);
	}
}

//==================================================
// 函数功能：清除屏幕
// 输入参数：无
// 输出参数：无
//==================================================
void lcd_clear(void)
{
	write_cmd(0x01);
}

//==================================================
// 函数功能：写入命令
// 输入参数：字符cmd
// 输出参数：无
//==================================================
void write_cmd(uchar cmd)
{
	RS=0;
	RW=0; 
	LCD_DATA = cmd;
	delay_ms(5);
	EN=1;
	delay_ms(5);
	EN=0;
}

//==================================================
// 函数功能：写入数据
// 输入参数：字符dat
// 输出参数：无
//==================================================
void write_data(uchar dat)
{
	RS=1;
	RW=0; 
	LCD_DATA = dat;
	delay_ms(5);
	EN=1;
	delay_ms(5);
	EN=0;
}
