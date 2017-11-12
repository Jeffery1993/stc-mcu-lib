#ifndef _KEYBOARD_H
#define _KEYBOARD_H

//==================================================
// �ļ�����
//==================================================
#include "config.h"
#include "delay.h"

//==================================================
// ���Ŷ���
//==================================================
#define KEYBOARD_PORT P2
sbit R1 = KEYBOARD_PORT^0;
sbit R2 = KEYBOARD_PORT^1;
sbit R3 = KEYBOARD_PORT^2;
sbit R4 = KEYBOARD_PORT^3;
sbit C1 = KEYBOARD_PORT^4;
sbit C2 = KEYBOARD_PORT^5;
sbit C3 = KEYBOARD_PORT^6;
sbit C4 = KEYBOARD_PORT^7;

//==================================================
// ��������
//==================================================
void keyboard_init(void);
bit is_triggered(void);
uchar read_keyboard(void);

#endif