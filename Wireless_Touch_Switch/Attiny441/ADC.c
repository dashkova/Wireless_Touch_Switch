/*
 * ADC.c
 *
 * Created: 11.06.2018 0:30:20
 *  Author: Andrey
 */ 


#include <avr/io.h>

#include "ADC.h"

/*
void ADC_Init (void)
{
	DIDR0 =	(1<<ADC1D) | (1<<ADC2D) | (1<<ADC3D);
	
	ADMUX =	(1<<REFS1) | (0<<REFS0);			// Внутренний ИОН 1.1в

	ADCSRA =	(1<<ADEN) |				// Разрешаем работу ADC
		(1<<ADIE) |				// Разрешаем прерывание
		(1<<ADATE) |				// Старт преобразования вручную
		(0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		// Прескалер = 8

	ADCSRB =	(1<<BIN) | (0<<ADLAR) | (0<<ADTS1) | (0<<ADTS0);		// Работаем только со старшими 8-ю битами	
}

void ADC_Change_in (uint8_t Analog_input)
{
	ADMUX = (1<<REFS1) | (0<<REFS0) | Analog_input;
}*/
/*
 void ADC_Start_Conversion (uint8_t Analog_input)
 {
	 ADMUX |= Analog_input;

	ADCSRA =	(1<<ADEN) |				// Разрешаем работу ADC
	 	(1<<ADIE) |				// Разрешаем прерывание
	 	(1<<ADATE) |				// Старт преобразования вручную
	 	(0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) |		// Прескалер = 8
		(1<<ADSC);				// Старт конвертации
 }*/
 /*
 uint8_t ADC_Get_Data (void)
 {
	 return ADCH;
 }

 void ADC_Off (void)
 {
	ADCSRA =	0;
 }*/