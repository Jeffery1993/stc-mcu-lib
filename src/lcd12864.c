#include "lcd12864.h"

//==================================================
// 函数声明
//==================================================
void read_busy(void);
void write_cmd(uchar wrcommand,is_busy);
void write_data(uchar wrdata);

//==================================================
// 函数功能：初始化
// 输入参数：无
// 输出参数：无
//==================================================
void lcd_init(void)
{
	delay_ms(5);
	RE=1;
	RE=0;
	RE=1;
	write_cmd(0x30,0);
	delay_ms(5);
	write_cmd(0x30,0);
	delay_ms(5);
	write_cmd(0x0C,1);
	delay_ms(5);
	write_cmd(0x01,1);
	delay_ms(10);
	write_cmd(0x06,1);
	delay_ms(5);
}

//==================================================
// 函数功能：显示字符
// 输入参数：字符ch
// 输出参数：无
//==================================================
void lcd_putchar(uchar row,uchar col,uchar ch)
{
	uchar addr;
	if(row==1) addr=0x80;
	else if(row==2) addr=0x90;
	else if(row==3) addr=0x88;
	else if(row==4) addr=0x98;
	else return;
	
	if(col>=1 && col<=16) addr += col-1;
	else return;
	
	write_cmd(addr,1);
	write_data(ch);
}

//==================================================
// 函数功能：显示字符串
// 输入参数：整型line、字符串string
// 输出参数：无
//==================================================
void lcd_display(uchar line,uchar *string)
{
	uchar addr;
	if(line==1) addr=0x80;
	else if(line==2) addr=0x90;
	else if(line==3) addr=0x88;
	else if(line==4) addr=0x98;
	else return;
	
	write_cmd(addr,1);
	
	while(*string!=0)
	{
		write_data(*string++);
	}
}

//==================================================
// 函数功能：清除屏幕
// 输入参数：字符型ch
// 输出参数：无
//==================================================
void lcd_clear(void)
{
	write_cmd(0x01,1);
}

//==================================================
// 函数功能：读取状态
// 输入参数：无
// 输出参数：无
//==================================================
void read_busy(void)
{
	while(1)
	{
		RS=0;
		RW=1;
		EN=0;
		LCD_DATA=0xFF;
		EN=1;
		if((LCD_DATA & BUSY_BUS)==0)
		break;
	}
}

//==================================================
// 函数功能：写入命令
// 输入参数：字符型
// 输出参数：无
//==================================================
void write_cmd(uchar wrcommand,is_busy)
{
	if(is_busy) read_busy();
	RS=0;
	RW=0;
	EN=1;
	LCD_DATA = wrcommand;
	EN=0;
}

//==================================================
// 函数功能：写入数据
// 输入参数：字符型
// 输出参数：无
//==================================================
void write_data(uchar wrdata)
{
	read_busy();
	RS=1;
	RW=0;
	EN=1;
	LCD_DATA = wrdata;
	EN=0;
}