/*
 * USI_SPI.h
 *
 * Created: 10.03.2018 12:44:06
 *  Author: Andrey
 */ 


#ifndef USI_SPI_H
#define USI_SPI_H

#include <avr/io.h>


	//... Инициализация SPI
void SPI_Init(void);

	//... Передать и получить байт данных
uint8_t SPI_WriteByte(uint8_t data);

	//... Передать и получить байт данных
//uint8_t SPI_ReadByte(uint8_t data);

	//... Отправить несколько байт по SPI. cmd - команда, data - данные для отправки
//void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data);

	//... Отправить один байт и получить несколько байт по SPI. Принятые данные лежат в data
//void SPI_ReadArray(uint8_t num, uint8_t *data);

#endif //USI_SPI_H