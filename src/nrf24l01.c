#include "nrf24l01.h"

//==================================================
// 变量定义
//==================================================
uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
uchar RX_BUF[TX_PLOAD_WIDTH];
uchar TX_BUF[TX_PLOAD_WIDTH];
uchar flag;
uchar bdata sta;
sbit  RX_DR	 = sta^6;
sbit  TX_DS	 = sta^5;
sbit  MAX_RT = sta^4;

//==================================================
// 函数声明
//==================================================
uchar spi_rw(uchar byte);
uchar spi_rw_reg(uchar reg, uchar value);
uchar spi_read(uchar reg);
uchar spi_read_buf(uchar reg, uchar * pBuf, uchar bytes);
uchar spi_write_buf(uchar reg, uchar * pBuf, uchar bytes);
void spi_RX_mode(void);
void spi_TX_mode(uchar * BUF);
uchar spi_check_ack(bit clear);

//==================================================
// 函数功能：初始化IO
// 输入参数：无
// 输出参数：无
//==================================================
void spi_init(void)
{
	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCK = 0;        // SPI时钟置低
	IRQ = 1;        // 中断复位
}

//==================================================
// 函数功能：发送数据
// 输入参数：字符ch
// 输出参数：无
//==================================================
void spi_putchar(uchar ch)
{
	TX_BUF[0] = ch;
	spi_TX_mode(TX_BUF);
	spi_check_ack(1);
}

//==================================================
// 函数功能：发送数据
// 输入参数：字符串string
// 输出参数：无
//==================================================
void spi_puts(uchar *string)
{
	while(*string!=0)
	{
		spi_putchar(*string++);
	}
	spi_putchar(0);
}

//==================================================
// 函数功能：读取数据
// 输入参数：无
// 输出参数：字符型
//==================================================
uchar spi_getchar(void)
{
	spi_RX_mode();
	sta=spi_read(STATUS);
	if(RX_DR)
	{
		spi_read_buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);
		flag=1;
	}
	spi_rw_reg(WRITE_REG+STATUS,sta);
	if(flag)
	{
		flag=0;
		return(RX_BUF[0]);
	}
	return NULL;
}

//==================================================
// 函数功能：读取数据
// 输入参数：无
// 输出参数：字符串
//==================================================
uchar* spi_gets(void)
{
	uchar ch;
	uchar i = 0;
	uchar string[MAX_LENGTH];
	
	while((ch=spi_getchar())!=NULL)
	{
		string[i] = ch;
		i++;
		if(i>=MAX_LENGTH-1) break;
	}
	string[i] = 0;
	return string;
}

//==================================================
// 函数功能：根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01读出一字节
// 输入参数：字符型byte
// 输出参数：字符型byte
//==================================================
uchar spi_rw(uchar byte)
{
	uchar i;
	for(i=0; i<8; i++)				// 循环8次
	{
		MOSI = (byte & 0x80);		// byte最高位输出到MOSI
		byte <<= 1;             // 低一位移位到最高位
		SCK = 1;                // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
		byte |= MISO;       		// 读MISO到byte最低位
		SCK = 0;            		// SCK置低
	}
	return(byte);           	// 返回读出的一字节
}

//==================================================
// 函数功能：写数据value到reg寄存器
// 输入参数：字符型value
// 输出参数：字符型状态status
//==================================================
uchar spi_rw_reg(uchar reg, uchar value)
{
	uchar status;
	CSN = 0;                   // CSN置低，开始传输数据
	status = spi_rw(reg);      // 选择寄存器，同时返回状态字
	spi_rw(value);             // 然后写数据到该寄存器
	CSN = 1;                   // CSN拉高，结束数据传输
	return(status);            // 返回状态寄存器
}

//==================================================
// 函数功能：从reg寄存器读一字节
// 输入参数：字符型reg
// 输出参数：字符型数据reg_val
//==================================================
uchar spi_read(uchar reg)
{
	uchar reg_val;
	CSN = 0;                    // CSN置低，开始传输数据
	spi_rw(reg);                // 选择寄存器
	reg_val = spi_rw(0);        // 然后从该寄存器读数据
	CSN = 1;                    // CSN拉高，结束数据传输
	return(reg_val);            // 返回寄存器数据
}

//==================================================
// 函数功能：从reg寄存器读出bytes个字节，通常用来读取接收通道数据或接收/发送地址
// 输入参数：字符型reg、bytes，字符型指针pBuf
// 输出参数：字符型状态寄存器status
//==================================================
uchar spi_read_buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
	CSN = 0;                    // CSN置低，开始传输数据
	status = spi_rw(reg);       // 选择寄存器，同时返回状态字
	for(i=0; i<bytes; i++)
		pBuf[i] = spi_rw(0);    	// 逐个字节从nRF24L01读出
	CSN = 1;                    // CSN拉高，结束数据传输
	return(status);             // 返回状态寄存器
}

//==================================================
// 函数功能：把pBuf缓存中的数据写入到nRF24L01，通常用来写入发射通道数据或接收/发送地址
// 输入参数：字符型reg、bytes，字符型指针pBuf
// 输出参数：字符型状态寄存器status
//==================================================
uchar spi_write_buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
	CSN = 0;                    // CSN置低，开始传输数据
	status = spi_rw(reg);       // 选择寄存器，同时返回状态字
	for(i=0; i<bytes; i++)
		spi_rw(pBuf[i]);        	// 逐个字节写入nRF24L01
	CSN = 1;                    // CSN拉高，结束数据传输
	return(status);             // 返回状态寄存器
}

//==================================================
// 函数功能：这个函数设置nRF24L01为接收模式，等待接收发送设备的数据包
// 输入参数：无
// 输出参数：无
//==================================================
void spi_RX_mode(void)
{
	CE = 0;
	spi_write_buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
	spi_rw_reg(WRITE_REG + EN_AA, 0x01);               // 使能接收通道0自动应答
	spi_rw_reg(WRITE_REG + EN_RXADDR, 0x01);           // 使能接收通道0
	spi_rw_reg(WRITE_REG + RF_CH, 40);                 // 选择射频通道0x40
	spi_rw_reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
	spi_rw_reg(WRITE_REG + RF_SETUP, 0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	spi_rw_reg(WRITE_REG + CONFIG, 0x0f);              // CRC使能，16位CRC校验，上电，接收模式
	CE = 1;                                            // 拉高CE启动接收设备
}

//==================================================
// 函数功能：这个函数设置nRF24L01为发送模式，（CE=1持续至少10us），
// 130us后启动发射，数据发送结束后，发送模块自动转入接收
// 模式等待应答信号。
// 输入参数：无
// 输出参数：无
//==================================================
void spi_TX_mode(uchar * BUF)
{
	CE = 0;
	spi_write_buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	spi_write_buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
	spi_write_buf(WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);                  // 写数据包到TX FIFO
	spi_rw_reg(WRITE_REG + EN_AA, 0x01);       // 使能接收通道0自动应答
	spi_rw_reg(WRITE_REG + EN_RXADDR, 0x01);   // 使能接收通道0
	spi_rw_reg(WRITE_REG + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
	spi_rw_reg(WRITE_REG + RF_CH, 40);         // 选择射频通道0x40
	spi_rw_reg(WRITE_REG + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	spi_rw_reg(WRITE_REG + CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	CE = 1;
}

//==================================================
// 函数功能：检查接收设备有无接收到数据包，设定没有收到应答信号是否重发
// 输入参数：位数据clear
// 输出参数：字符型
//==================================================
uchar spi_check_ack(bit clear)
{
	while(IRQ);
		sta = spi_rw(NOP);									// 返回状态寄存器
	if(MAX_RT)
		if(clear)                         	// 是否清除TX FIFO，没有清除在复位MAX_RT中断标志后重发
			spi_rw(FLUSH_TX);
	spi_rw_reg(WRITE_REG + STATUS, sta);	// 清除TX_DS或MAX_RT中断标志
	IRQ = 1;
	if(TX_DS)
		return(0x00);
	else
		return(0xff);
}