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
	spi_init();
	
	lcd_display(1,"I am a receiver");
	
	while(1)
	{
		ch = spi_getchar();
		if(ch != NULL)
		{
			if(p>=17)
			{
				p=0;
				lcd_clear();
			}
			lcd_putchar(2,p,ch);
			p++;
		}
	}
}