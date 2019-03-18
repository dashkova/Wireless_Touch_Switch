/*
 * EEPROM.c
 *
 * Created: 10.03.2018 12:44:06
 *  Author: Andrey
 */ 


 #include <avr/io.h>
 #include <avr/interrupt.h>

	//... ������ � EEPROM
void EEPROM_write(uint16_t ucAddress, uint8_t ucData)
{	
	while(EECR & (1<<EEPE));			//.. ���� ���������� ���������� �������� � �������

	cli();
	
	EECR = (0<<EEPM1)|(0<<EEPM0);			//.. ��������� ����� ������ - �������� � ������ �� ���� �������� ������������� 3.4 �� (Atomic Operation)
	 
	EEAR = ucAddress;				//.. ������� ����� ������ � ������ � ��������
	EEDR = ucData;	

	EECR |= (1<<EEMPE);				//.. ��������� ������ ��� EEMPE 
	
	EECR |= (1<<EEPE);				//.. � ��������� ������ � EEPROM

	sei();
}

/*
	//... ������ �� EEPROM
 uint8_t EEPROM_read(uint16_t ucAddress)
 {	 
	 while(EECR & (1<<EEPE));			//.. ���� ���������� ���������� �������� � �������

	 cli();
	
	 EEAR = ucAddress;				//.. ������� ����� ������ 
	 
	 EECR |= (1<<EERE);				//.. ����� ������ EEPROM

	 sei();
	 
	 return EEDR;				//.. ���������� ���������� ������
 }
 */