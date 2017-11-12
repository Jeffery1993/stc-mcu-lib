#ifndef _NRF24L01_H
#define _NRF24L01_H

//==================================================
// 文件包含
//==================================================
#include "config.h"

//==================================================
// 宏定义
//==================================================
#define NRF_BUS	P1

//==================================================
// SPI引脚定义
//==================================================
sbit CE   = NRF_BUS^0;  // Chip Enable pin signal (output)
sbit CSN  = NRF_BUS^1;  // Slave Select pin, (output to CSN, nRF24L01)
sbit SCK  = NRF_BUS^2;  // Serial Clock pin, (output)
sbit MOSI = NRF_BUS^3;  // Master Out, Slave In pin (output)
sbit MISO = NRF_BUS^4;  // Master In, Slave Out pin (input)
sbit IRQ  = NRF_BUS^5;  // Interrupt signal, from nRF24L01 (input)

//==================================================
// SPI命令定义
//==================================================
#define READ_REG    0x00  // Define read command to register
#define WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 0x61  // Define RX payload register address
#define WR_TX_PLOAD 0xA0  // Define TX payload register address
#define FLUSH_TX    0xE1  // Define flush TX register command
#define FLUSH_RX    0xE2  // Define flush RX register command
#define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define NOP         0xFF  // Define No Operation, might be used to read status register

//==================================================
// SPI寄存器
//==================================================
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address

//==================================================
// 数据宽度定义
//==================================================
#define TX_ADR_WIDTH		5  // 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH	4  // 数据通道有效数据宽度
#define MAX_LENGTH			16

//==================================================
// 函数声明
//==================================================
void spi_init(void);
void spi_putchar(uchar ch);
void spi_puts(uchar *string);
uchar spi_getchar(void);
uchar* spi_gets(void);

#endif