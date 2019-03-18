/*
 * SPI.h
 *
 * SPI from AtMega
 *
 * Created:	15.01.2019
 * Modification:	15.01.2019
 *
 *  Author:	Andrey
 *		parovoZZ
 *		Dashkova
 */ 

 #include "Settings.h"


#ifndef SPI_H
#define SPI_H

 
	//... ������������� SPI
 inline void SPI_Init(void)
 {
	//SPI_DDR |= (1<<MOSI_PIN) | (1<<SCK_PIN) | (0<<MISO_PIN);				// ��� ������, ����� MISO, ������
 	//SPI_PORT |= (0<<MOSI_PIN) | (0<<SCK_PIN) | (0<<MISO_PIN);
		
	SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0);		// ���������� spi,������� ��� ������, ������
	SPSR = (1<<SPI2X);								// ��������� �������� ������������
 }

 	//... �������� � ������� ���� ������
 inline uint8_t SPI_WriteByte(uint8_t data)
 {
	uint8_t temp;
	
	SPDR = data; 	
	
	while(!(SPSR & (1<<SPIF)));
 		
	temp = SPDR;

 	return temp;
 }


#endif //SPI_H