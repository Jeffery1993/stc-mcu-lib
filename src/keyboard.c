#include "keyboard.h"

//==================================================
// 函数声明
//==================================================
uchar scan_keyboard(void);

//==================================================
// 函数功能：键盘初始化
// 输入参数：无
// 输出参数：无
//==================================================
void keyboard_init(void)
{
	KEYBOARD_PORT |= 0xf0;
	KEYBOARD_PORT &= 0xf0;
}

//==================================================
// 函数功能：检查键盘是否被触发
// 输入参数：无
// 输出参数：布尔型
//==================================================
bit is_triggered(void)
{
	if(!(C1&&C2&&C3&&C4))
		return TRUE;
	else
		return FALSE;
}

//==================================================
// 函数功能：读取键盘扫描结果
// 输入参数：无
// 输出参数：字符型
//==================================================
uchar read_keyboard(void)
{
	uchar ch = NULL;
	delay_ms(10);
	if(!(C1&&C2&&C3&&C4))
	{
		ch = scan_keyboard();
	}
	return ch;
}

//==================================================
// 函数功能：扫描键盘
// 输入参数：无
// 输出参数：字符型
//==================================================
uchar scan_keyboard(void)
{
	uchar R = 0;
	uchar C = 0;
	KEYBOARD_PORT &= 0x0f;
	KEYBOARD_PORT |= 0x0f;
	if(R1==0) R = 1;
	if(R2==0) R = 2;
	if(R3==0) R = 3;
	if(R4==0) R = 4;
	
	KEYBOARD_PORT |= 0xf0;
	KEYBOARD_PORT &= 0xf0;
	if(C1==0) C = 1;
	if(C2==0) C = 2;
	if(C3==0) C = 3;
	if(C4==0) C = 4;
	
	while(!(C1&&C2&&C3&&C4));
	
	if(R==1&&C==1) return '0';
	if(R==1&&C==2) return '1';
	if(R==1&&C==3) return '2';
	if(R==1&&C==4) return '3';
	
	if(R==2&&C==1) return '4';
	if(R==2&&C==2) return '5';
	if(R==2&&C==3) return '6';
	if(R==2&&C==4) return '7';
	
	if(R==3&&C==1) return '8';
	if(R==3&&C==2) return '9';
	if(R==3&&C==3) return 'a';
	if(R==3&&C==4) return 'b';
	
	if(R==4&&C==1) return 'c';
	if(R==4&&C==2) return 'd';
	if(R==4&&C==3) return 'e';
	if(R==4&&C==4) return 'f';
	
	return NULL;
}