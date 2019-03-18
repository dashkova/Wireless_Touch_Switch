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

	//... ������ �� EEPROM
	inline uint8_t EEPROM_read(uint16_t ucAddress)
	{
		while(EECR & (1<<EEPE));			//.. ���� ���������� ���������� �������� � �������

		cli();
		
		EEAR = ucAddress;				//.. ������� ����� ������
		
		EECR |= (1<<EERE);				//.. ����� ������ EEPROM

		sei();
		
		return EEDR;				//.. ���������� ���������� ������
	}


#endif /* EEPROM_H_ */