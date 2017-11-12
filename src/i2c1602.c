#include "i2c1602.h"
#include "delay.h"
#include <intrins.h>

//==================================================
// 函数声明
//==================================================
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_ack(void);
void i2c_nack(void);
bit i2c_check_ack(void);
void i2c_write_byte(uchar temp);
uchar i2c_read_byte(void);
void lcd_write_cmd(uchar cmd);
void lcd_write_data(uchar dat);
void delay_us(void);

//==================================================
// 函数功能：i2c初始化
// 输入参数：无
// 输出参数：无
//==================================================
void i2c_init(void)
{
	SCL = 1;
	delay_us();
	SDA = 1;
	delay_us();
}

//==================================================
// 函数功能：i2c启动信号
// 输入参数：无
// 输出参数：无
//==================================================
void i2c_start(void)
{
	SDA = 1;
	_nop_();
	SCL = 1;
	delay_us();
	SDA = 0;
	delay_us();
	SCL = 0;
}

//==================================================
// 函数功能：i2c停止信号
// 输入参数：无
// 输出参数：无
//==================================================
void i2c_stop(void)
{
	SDA = 0;
	_nop_();
	SCL = 1;
	delay_us();
	SDA = 1;
	delay_us();
}

//==================================================
// 函数功能：i2c从机应答
// 输入参数：无
// 输出参数：无
//==================================================
void i2c_ack(void)
{
	uchar i = 0;
	
	SDA = 0;
	SCL = 1;
	delay_us();
	while((SDA == 1)&& i < 255)
		i++;
	SCL = 0;
	SDA = 1;
}

//==================================================
// 函数功能：i2c主机非应答信号
// 输入参数：无
// 输出参数：无
//==================================================
void i2c_nack(void)
{
	SDA = 1;
	SCL = 1;
	delay_us();
	SCL = 0;
	SDA = 0;
}

//==================================================
// 函数功能：i2c检查应答信号
// 输入参数：无
// 输出参数：位
//==================================================
bit i2c_check_ack(void)
{
	bit state = 0;
	SDA = 1;
	SCL = 1;
	state = SDA;
	SCL = 0;
	return state;
}

//==================================================
// 函数功能：i2c写一个字节
// 输入参数：无
// 输出参数：无
//==================================================
void i2c_write_byte(uchar temp)
{
	uchar i;
	for(i=0;i<8;i++){
		SDA=(bit)(temp & 0x80) ;
		temp <<=1;
		_nop_();
		SCL=1;
		delay_us();
		SCL=0;
	}
	delay_us();
	SDA=1;
	delay_us();
	SCL=1;	
	delay_us();
	while(SDA);
	_nop_();
	SCL=0;
}

//==================================================
// 函数功能：i2c读一个字节
// 输入参数：无
// 输出参数：无
//==================================================
uchar i2c_read_byte(void)
{
	uchar i,temp;
	SCL = 0;
	delay_us();
	SDA = 1;
	for(i = 0; i < 8; i++)
	{
		SCL = 1;
		delay_us();
		temp = (temp << 1) | SDA;
		delay_us();
		SCL = 0;
		delay_us();
	}
	delay_us();
	return temp;
}

//==================================================
// 函数功能：i2c1602写入命令
// 输入参数：字符型cmd
// 输出参数：无
//==================================================
void lcd_write_cmd(uchar cmd)
{
	uchar temp;
	
	i2c_start();
	i2c_write_byte(SLAVE_ADDR);
	
	temp = cmd & 0xF0;
	temp |= 0x0C;
	i2c_write_byte(temp);
	delay_ms(20);
	temp &= 0xFB;
	i2c_write_byte(temp);
	
	temp = (cmd & 0x0F)<<4;
	temp |= 0x0C;
	i2c_write_byte(temp);
	delay_ms(20);
	temp &= 0xFB;
	i2c_write_byte(temp);
	
	i2c_stop();
}

//==================================================
// 函数功能：i2c1602写入数据
// 输入参数：字符型dat
// 输出参数：无
//==================================================
void lcd_write_data(uchar dat)
{
	uchar temp;
	
	i2c_start();
	i2c_write_byte(SLAVE_ADDR);
	
	temp = dat & 0xF0;
	temp |= 0x0D;
	i2c_write_byte(temp);
	delay_ms(20);
	temp &= 0xFB;
	i2c_write_byte(temp);
	
	temp = (dat & 0x0F)<<4;
	temp |= 0x0D;
	i2c_write_byte(temp);
	delay_ms(20);
	temp &= 0xFB ;
	i2c_write_byte(temp);
	
	i2c_stop();
}

//==================================================
// 函数功能：i2c1602初始化
// 输入参数：无
// 输出参数：无
//==================================================
void lcd_init(void)
{
	i2c_init();
	delay_ms(10);
	lcd_write_cmd(0x33);
	delay_ms(50);
	lcd_write_cmd(0x32);
	delay_ms(50);
	lcd_write_cmd(0x28);
	delay_ms(50);
	lcd_write_cmd(0x0C);
	delay_ms(50);
	lcd_write_cmd(0x06);
	delay_ms(50);
	lcd_write_cmd(0x01);
	delay_ms(50);
}

//==================================================
// 函数功能：i2c1602显示字符
// 输入参数：字符型ch
// 输出参数：无
//==================================================
void lcd_putchar(uchar row,uchar col,uchar ch)
{
	uchar addr;
	if(row==1) addr = 0x80;
	else if(row==2) addr = 0xC0;
	else return;
	
	if(col>=1 && col<=16) addr += col-1;
	else return;
	lcd_write_cmd(addr);
	lcd_write_data(ch);
}

//==================================================
// 函数功能：i2c1602显示字符串
// 输入参数：字符型line，字符串string
// 输出参数：无
//==================================================
void lcd_display(uchar line,uchar *string)
{
	uchar addr;
	if(line==1) addr = 0x80;
	else if(line==2) addr = 0xC0;
	else return;
	
	lcd_write_cmd(addr);
	while(*string!=0)
	{
		lcd_write_data(*string++);
	}
}

//==================================================
// 函数功能：i2c1602清除屏幕
// 输入参数：无
// 输出参数：无
//==================================================
void lcd_clear(void)
{
	lcd_write_cmd(0x01);
}

//==================================================
// 函数功能：延时微秒
// 输入参数：无
// 输出参数：无
//==================================================
void delay_us(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}