#ifndef _CONFIG_H
#define _CONFIG_H

//==================================================
// 文件包含
//==================================================
#include <reg52.h>

//==================================================
// 宏定义
//==================================================
#define MAIN_Fosc 11059200L
#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif
#ifndef NULL
	#define NULL 0
#endif
#define ushort	unsigned short
#define uint		unsigned int
#define uchar		unsigned char

//==================================================
// 类型别名
//==================================================
typedef bit							U1;
typedef unsigned char		U8;
typedef signed char			S8;
typedef unsigned int		U16;
typedef signed int			S16;
typedef unsigned long		U32;
typedef signed long			S32;

#endif