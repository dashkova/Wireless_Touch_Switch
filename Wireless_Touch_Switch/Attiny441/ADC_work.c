/*
 * ADC_work.c
 *
 * Created: 11.06.2018 0:30:36
 *  Author: Andrey
 */ 


 
 #include <stddef.h>
 #include <avr/io.h>

 #include "ADC_work.h"
 #include "../bits_macros.h"

 void ADC_Start_Conversion (uint8_t Analog_input)
 {
	 ADMUX |= Analog_input;

	 ADCSRA |= Bit(ADSC);
 }


 uint8_t ADC_Get_Data (void)
 {
	 return ADCH;
 }