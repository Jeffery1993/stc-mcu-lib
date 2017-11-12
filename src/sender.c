#include "keyboard.h"
#include "i2c1602.h"
#include "nrf24l01.h"

//==================================================
// 函数功能：主函数
// 输入参数：无
// 输出参数：无
//==================================================
void main(void)
{
	uchar ch = NULL;
	uchar p = 1;
	
	lcd_init();
	keyboard_init();
	spi_init();
	
	lcd_display(1,"I am a sender");
	
	while(1)
	{
		if(is_triggered() == TRUE)
		{
			ch = read_keyboard();
			if(ch == 'f')
			{
				lcd_clear();
				p=1;
			}
			else
			{
				lcd_putchar(2,p,ch);
				spi_putchar(ch);
				p++;
			}
		}
	}
}