/*
 * EEPROM.h
 *
 * Created: 10.03.2018 12:44:06
 *  Author: Andrey
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


void EEPROM_write(uint16_t ucAddress, uint8_t ucData);

//uint8_t EEPROM_read(uint16_t ucAddress);

	//... Чтение из EEPROM
	inline uint8_t EEPROM_read(uint16_t ucAddress)
	{
		while(EECR & (1<<EEPE));			//.. Ждем завершения предыдущей операции с памятью

		cli();
		
		EEAR = ucAddress;				//.. Запишем адрес ячейки
		
		EECR |= (1<<EERE);				//.. Старт чтения EEPROM

		sei();
		
		return EEDR;				//.. Возвращаем полученные данные
	}


#endif /* EEPROM_H_ */