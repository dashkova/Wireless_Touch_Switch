/*
 * USI_SPI.h
 *
 * Created: 10.03.2018 12:44:06
 *  Author: Andrey
 */ 


#ifndef USI_SPI_H
#define USI_SPI_H

#include <avr/io.h>


	//... ������������� SPI
void SPI_Init(void);

	//... �������� � �������� ���� ������
uint8_t SPI_WriteByte(uint8_t data);

	//... �������� � �������� ���� ������
//uint8_t SPI_ReadByte(uint8_t data);

	//... ��������� ��������� ���� �� SPI. cmd - �������, data - ������ ��� ��������
//void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data);

	//... ��������� ���� ���� � �������� ��������� ���� �� SPI. �������� ������ ����� � data
//void SPI_ReadArray(uint8_t num, uint8_t *data);

#endif //USI_SPI_H