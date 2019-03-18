/*
 * EEPROM.c
 *
 * Created: 10.03.2018 12:44:06
 *  Author: Andrey
 */ 


 #include <avr/io.h>
 #include <avr/interrupt.h>

	//... Запись в EEPROM
void EEPROM_write(uint16_t ucAddress, uint8_t ucData)
{	
	while(EECR & (1<<EEPE));			//.. Ждем завершения предыдущей операции с памятью

	cli();
	
	EECR = (0<<EEPM1)|(0<<EEPM0);			//.. Установим режим работы - стирание и запись за одну операцию длительностью 3.4 мс (Atomic Operation)
	 
	EEAR = ucAddress;				//.. Запишем адрес ячейки и данные в регистры
	EEDR = ucData;	

	EECR |= (1<<EEMPE);				//.. Установим мастер бит EEMPE 
	
	EECR |= (1<<EEPE);				//.. и произведём запись в EEPROM

	sei();
}

/*
	//... Чтение из EEPROM
 uint8_t EEPROM_read(uint16_t ucAddress)
 {	 
	 while(EECR & (1<<EEPE));			//.. Ждем завершения предыдущей операции с памятью

	 cli();
	
	 EEAR = ucAddress;				//.. Запишем адрес ячейки 
	 
	 EECR |= (1<<EERE);				//.. Старт чтения EEPROM

	 sei();
	 
	 return EEDR;				//.. Возвращаем полученные данные
 }
 */