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



	//... Инициализация SPI
void SPI_Init(void)
{
	SPI_DDR |= (1<<MOSI) | (1<<SCK) | (0<<MISO);		// Все выводы, кроме MISO, выходы
	//SPI_PORT |= (0<<MOSI) | (0<<SCK) | (0<<MISO);

	//USICR |= (Bit(USIWM0) | Bit(USICLK));
	USICR = (1<<USIWM0) | (1<<USICLK);
}

	//... Передать байт данных
uint8_t SPI_WriteByte(uint8_t data)
{
	uint8_t clk0 = Bit(USIWM0) | Bit(USITC);
	uint8_t clk1 = Bit(USIWM0) | Bit(USITC) | Bit(USICLK);

	//... Копируем байт в регистр USIDR
	USIDR = data;

	USICR = clk0;			// Режим SPI, тактируем шину сами
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
	//... Установим флаг начала передачи					
	USISR |= Bit(USIOIF);

	//... В цикле следим за флагом
	while(!(USISR & Bit(USIOIF)))
	{
		USICR |= ( Bit(USITC));		// Режим SPI, тактируем шину сами

		//USICR = ( Bit(USITC) | Bit(USIWM0));
		//USICR = ( Bit(USITC) | Bit(USIWM0) | Bit(USICLK));
	}
*/
	return USIDR;

}
/*
	//... Передать и получить байт данных 
uint8_t SPI_ReadByte(uint8_t data)
{
	//... Отправляем байт
	SPI_WriteByte(data);

	//... Принятый байт возвращаем
	return USIDR; 
}

	//... Отправить несколько байт по SPI. cmd - команда, data - данные для отправки
void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data)
{
	nRF_SELECT();
	
	//... Отправим команду
	SPI_WriteByte(cmd);

	//... Затем данные
	while(num--)
	{
		SPI_WriteByte(*data++);
	}

	nRF_DESELECT();
}

	//... Отправить и получить несколько байт по SPI. Принятые данные лежат в data 
void SPI_ReadArray(uint8_t num, uint8_t *data)
{

	while(num--)
	{      
		*data++ = SPI_WriteByte(*data); 
	}

}

*/