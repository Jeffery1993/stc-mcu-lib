#include "lcd12864.h"

//==================================================
// ��������
//==================================================
void read_busy(void);
void write_cmd(uchar wrcommand,is_busy);
void write_data(uchar wrdata);

//==================================================
// �������ܣ���ʼ��
// �����������
// �����������
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
// �������ܣ���ʾ�ַ�
// ����������ַ�ch
// �����������
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
// �������ܣ���ʾ�ַ���
// �������������line���ַ���string
// �����������
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
// �������ܣ������Ļ
// ����������ַ���ch
// �����������
//==================================================
void lcd_clear(void)
{
	write_cmd(0x01,1);
}

//==================================================
// �������ܣ���ȡ״̬
// �����������
// �����������
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
// �������ܣ�д������
// ����������ַ���
// �����������
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
// �������ܣ�д������
// ����������ַ���
// �����������
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