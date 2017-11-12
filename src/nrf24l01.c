#include "nrf24l01.h"

//==================================================
// ��������
//==================================================
uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // ����һ����̬���͵�ַ
uchar RX_BUF[TX_PLOAD_WIDTH];
uchar TX_BUF[TX_PLOAD_WIDTH];
uchar flag;
uchar bdata sta;
sbit  RX_DR	 = sta^6;
sbit  TX_DS	 = sta^5;
sbit  MAX_RT = sta^4;

//==================================================
// ��������
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
// �������ܣ���ʼ��IO
// �����������
// �����������
//==================================================
void spi_init(void)
{
	CE  = 0;        // ����
	CSN = 1;        // SPI��ֹ
	SCK = 0;        // SPIʱ���õ�
	IRQ = 1;        // �жϸ�λ
}

//==================================================
// �������ܣ���������
// ����������ַ�ch
// �����������
//==================================================
void spi_putchar(uchar ch)
{
	TX_BUF[0] = ch;
	spi_TX_mode(TX_BUF);
	spi_check_ack(1);
}

//==================================================
// �������ܣ���������
// ����������ַ���string
// �����������
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
// �������ܣ���ȡ����
// �����������
// ����������ַ���
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
// �������ܣ���ȡ����
// �����������
// ����������ַ���
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
// �������ܣ�����SPIЭ�飬дһ�ֽ����ݵ�nRF24L01��ͬʱ��nRF24L01����һ�ֽ�
// ����������ַ���byte
// ����������ַ���byte
//==================================================
uchar spi_rw(uchar byte)
{
	uchar i;
	for(i=0; i<8; i++)				// ѭ��8��
	{
		MOSI = (byte & 0x80);		// byte���λ�����MOSI
		byte <<= 1;             // ��һλ��λ�����λ
		SCK = 1;                // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
		byte |= MISO;       		// ��MISO��byte���λ
		SCK = 0;            		// SCK�õ�
	}
	return(byte);           	// ���ض�����һ�ֽ�
}

//==================================================
// �������ܣ�д����value��reg�Ĵ���
// ����������ַ���value
// ����������ַ���״̬status
//==================================================
uchar spi_rw_reg(uchar reg, uchar value)
{
	uchar status;
	CSN = 0;                   // CSN�õͣ���ʼ��������
	status = spi_rw(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
	spi_rw(value);             // Ȼ��д���ݵ��üĴ���
	CSN = 1;                   // CSN���ߣ��������ݴ���
	return(status);            // ����״̬�Ĵ���
}

//==================================================
// �������ܣ���reg�Ĵ�����һ�ֽ�
// ����������ַ���reg
// ����������ַ�������reg_val
//==================================================
uchar spi_read(uchar reg)
{
	uchar reg_val;
	CSN = 0;                    // CSN�õͣ���ʼ��������
	spi_rw(reg);                // ѡ��Ĵ���
	reg_val = spi_rw(0);        // Ȼ��ӸüĴ���������
	CSN = 1;                    // CSN���ߣ��������ݴ���
	return(reg_val);            // ���ؼĴ�������
}

//==================================================
// �������ܣ���reg�Ĵ�������bytes���ֽڣ�ͨ��������ȡ����ͨ�����ݻ����/���͵�ַ
// ����������ַ���reg��bytes���ַ���ָ��pBuf
// ����������ַ���״̬�Ĵ���status
//==================================================
uchar spi_read_buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
	CSN = 0;                    // CSN�õͣ���ʼ��������
	status = spi_rw(reg);       // ѡ��Ĵ�����ͬʱ����״̬��
	for(i=0; i<bytes; i++)
		pBuf[i] = spi_rw(0);    	// ����ֽڴ�nRF24L01����
	CSN = 1;                    // CSN���ߣ��������ݴ���
	return(status);             // ����״̬�Ĵ���
}

//==================================================
// �������ܣ���pBuf�����е�����д�뵽nRF24L01��ͨ������д�뷢��ͨ�����ݻ����/���͵�ַ
// ����������ַ���reg��bytes���ַ���ָ��pBuf
// ����������ַ���״̬�Ĵ���status
//==================================================
uchar spi_write_buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
	CSN = 0;                    // CSN�õͣ���ʼ��������
	status = spi_rw(reg);       // ѡ��Ĵ�����ͬʱ����״̬��
	for(i=0; i<bytes; i++)
		spi_rw(pBuf[i]);        	// ����ֽ�д��nRF24L01
	CSN = 1;                    // CSN���ߣ��������ݴ���
	return(status);             // ����״̬�Ĵ���
}

//==================================================
// �������ܣ������������nRF24L01Ϊ����ģʽ���ȴ����շ����豸�����ݰ�
// �����������
// �����������
//==================================================
void spi_RX_mode(void)
{
	CE = 0;
	spi_write_buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	spi_rw_reg(WRITE_REG + EN_AA, 0x01);               // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	spi_rw_reg(WRITE_REG + EN_RXADDR, 0x01);           // ʹ�ܽ���ͨ��0
	spi_rw_reg(WRITE_REG + RF_CH, 40);                 // ѡ����Ƶͨ��0x40
	spi_rw_reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	spi_rw_reg(WRITE_REG + RF_SETUP, 0x07);            // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	spi_rw_reg(WRITE_REG + CONFIG, 0x0f);              // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	CE = 1;                                            // ����CE���������豸
}

//==================================================
// �������ܣ������������nRF24L01Ϊ����ģʽ����CE=1��������10us����
// 130us���������䣬���ݷ��ͽ����󣬷���ģ���Զ�ת�����
// ģʽ�ȴ�Ӧ���źš�
// �����������
// �����������
//==================================================
void spi_TX_mode(uchar * BUF)
{
	CE = 0;
	spi_write_buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	spi_write_buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	spi_write_buf(WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);                  // д���ݰ���TX FIFO
	spi_rw_reg(WRITE_REG + EN_AA, 0x01);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	spi_rw_reg(WRITE_REG + EN_RXADDR, 0x01);   // ʹ�ܽ���ͨ��0
	spi_rw_reg(WRITE_REG + SETUP_RETR, 0x0a);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	spi_rw_reg(WRITE_REG + RF_CH, 40);         // ѡ����Ƶͨ��0x40
	spi_rw_reg(WRITE_REG + RF_SETUP, 0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	spi_rw_reg(WRITE_REG + CONFIG, 0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	CE = 1;
}

//==================================================
// �������ܣ��������豸���޽��յ����ݰ����趨û���յ�Ӧ���ź��Ƿ��ط�
// ���������λ����clear
// ����������ַ���
//==================================================
uchar spi_check_ack(bit clear)
{
	while(IRQ);
		sta = spi_rw(NOP);									// ����״̬�Ĵ���
	if(MAX_RT)
		if(clear)                         	// �Ƿ����TX FIFO��û������ڸ�λMAX_RT�жϱ�־���ط�
			spi_rw(FLUSH_TX);
	spi_rw_reg(WRITE_REG + STATUS, sta);	// ���TX_DS��MAX_RT�жϱ�־
	IRQ = 1;
	if(TX_DS)
		return(0x00);
	else
		return(0xff);
}