/*
 * USI_SPI.c
 *
 * USI / SPI from attiny
 *
 * Created: 10.03.2018
 * Modification: 10.01.2019
 *
 *  Author: Andrey
 */ 

#define F_CPU 8000000UL


#include "usi_spi.h"
#include "../bits_macros.h"
#include "../nRF24L01P.h"



	//... ������������� SPI
void SPI_Init(void)
{
	SPI_DDR |= (1<<MOSI) | (1<<SCK) | (0<<MISO);		// ��� ������, ����� MISO, ������
	//SPI_PORT |= (0<<MOSI) | (0<<SCK) | (0<<MISO);

	//USICR |= (Bit(USIWM0) | Bit(USICLK));
	USICR = (1<<USIWM0) | (1<<USICLK);
}

	//... �������� ���� ������
uint8_t SPI_WriteByte(uint8_t data)
{
	uint8_t clk0 = Bit(USIWM0) | Bit(USITC);
	uint8_t clk1 = Bit(USIWM0) | Bit(USITC) | Bit(USICLK);

	//... �������� ���� � ������� USIDR
	USIDR = data;

	USICR = clk0;			// ����� SPI, ��������� ���� ����
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;
	USICR = clk0;
	USICR = clk1;

/*	
	//... ��������� ���� ������ ��������					
	USISR |= Bit(USIOIF);

	//... � ����� ������ �� ������
	while(!(USISR & Bit(USIOIF)))
	{
		USICR |= ( Bit(USITC));		// ����� SPI, ��������� ���� ����

		//USICR = ( Bit(USITC) | Bit(USIWM0));
		//USICR = ( Bit(USITC) | Bit(USIWM0) | Bit(USICLK));
	}
*/
	return USIDR;

}
/*
	//... �������� � �������� ���� ������ 
uint8_t SPI_ReadByte(uint8_t data)
{
	//... ���������� ����
	SPI_WriteByte(data);

	//... �������� ���� ����������
	return USIDR; 
}

	//... ��������� ��������� ���� �� SPI. cmd - �������, data - ������ ��� ��������
void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data)
{
	nRF_SELECT();
	
	//... �������� �������
	SPI_WriteByte(cmd);

	//... ����� ������
	while(num--)
	{
		SPI_WriteByte(*data++);
	}

	nRF_DESELECT();
}

	//... ��������� � �������� ��������� ���� �� SPI. �������� ������ ����� � data 
void SPI_ReadArray(uint8_t num, uint8_t *data)
{

	while(num--)
	{      
		*data++ = SPI_WriteByte(*data); 
	}

}

*/