/*
 * main.c
 * Wireless_Touch_Switch
 *
 * Сенсорный радиовыключатель
 *
 * MK: Attiny24A/44A/84A
 * Created: 16.03.2019 20:03:17
 * Author : parovoZZ
 *

 

	Структура пакета:
	0 - адрес, старший байт
	1 - адрес, младший байт
	2 - тип
	3 - поле состояния кнопок
	4 - напряжение питания
	5 - доп. сообщение (ошибка)	????
	|__
		0 бит	бит PORF - загрузка по сбросу питания
		1 бит	бит EXTRF - перезагрузка по ресету
		2 бит	бит BORF - перезагрузка по детектору питания
		3 бит	бит WDRF - перезагрузка по вачдогу
		4 бит
		5 бит	ошибка измерения Т
		6 бит	ошибка измерения RH 
		7 бит	ошибка записи в регистр статуса						

 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "USI_SPI.h"
#include "nRF24L01P.h"
#include "Settings.h"
#include "Sleep.h"
#include "ADC.h"
#include "bits_macros.h"

#define F_CPU		1000000UL


//.. Питание трансивера и МК
#define Vcc_On()		Vcc_PORT |= Bit(Vcc)
#define Vcc_Off()		Vcc_PORT &= ~Bit(Vcc)

//... Сторожевая собака
#define WDT_on			WDTCSR = Bit(WDE) | Bit(WDCE)
#define WDT_int_16mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE)			// Включение собаки на 16 мс и разрешение прерываний от нее
#define WDT_int_64mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP1)		// Включение собаки на 64 мс и разрешение прерываний от нее
#define WDT_int_125mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP1) | Bit(WDP0)	// Включение собаки на 125 мс и разрешение прерываний от нее
#define WDT_int_500mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP2) | Bit(WDP0)
#define WDT_int_8S			WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP3) | Bit(WDP0)	// Включение собаки на 8 с и разрешение прерываний от нее
#define WDT_off			WDT_on; WDTCSR = 0

//.. Таймер 1
#define T1_Start			TCCR1B = Bit (CS10) | Bit (WGM12)
#define T1_Set_Period(T)		OCR1A = (T)
#define T1_Clear			TCNT1 = 0


volatile	uint8_t	data[5];

ISR(WATCHDOG_vect)
{
		
}

ISR(ADC_vect)
{
	data[6] = ADC_Get_Data();			// Получаем результат 
	ADC_Off();				// Запрещаем работу ADC
	PRR = (1<<PRTIM0) | (1<<PRTIM1) | (1<<ADC);	// Тактируем только USI
}

/*
ISR(EXT_INT0_vect)
{

}
*/

ISR(PCINT1_vect)		// Прерывание от пинов PCINT0..PCINT7
{
	
}

/*
ISR(TIM1_COMPA_vect)
{

}
*/

int main(void)
{
	uint8_t	temp;	
	uint8_t	Vcc_measurement = 0;

//	data[7] = MCUSR;				// Здесь статус перезагрузки
//	MCUSR = 0;

	PRR = 0xFF;				// Выключаем всю периферию

	DDRA =	(1<<Vcc) | (0<<Q0) |		// Конфигурируем пины для питания на выход
		(0<<Q1) | (0<<Q2) |			// Пины входов подтяжки как вход
		(1<<MOSI) | (1<<SCK) | (0<<MISO) |	// Пины модуля USI
		(1<<Q3);				// Свободный пин на выход

	PORTA =	(1<<Vcc) | (0<<Q0) |
		(0<<Pullup_SCK) | (0<<Pullup_DATA) |
		(0<<MOSI) | (0<<SCK) | (0<<MISO) |
		(0<<PINA0);
		
	Vcc_On();					// Подаем питание на плату (через ключ)

	WDT_int_125mS;				// Взводим будильник на 125 мс

	PRR = (1<<PRTIM0) | (1<<PRTIM1) | (1<<ADC);

	USICR |= (Bit(USIWM0) | Bit(USICLK));		// Конфигурируем USI на работу по SPI	

	sei();
	
	Sleep(SLEEP_MODE_PWR_DOWN);			// Уходим в сон для ожидания запуска трансивера

	PRR = (1<<PRTIM0) | (1<<PRTIM1);		// Тактируем только ADC и USI
	ADC_Init();				// Инициализация ADC. Необходимо выдержать 1 мс
						// до начала измерения напряжения питания

	nRF_Init(nRF_channel);			// Конфигурируем регистры и там же
						// включаем RF часть трансивера для выдержки паузы
						// до начала сеанса передачи
	ADC_Start_Conversion();			// Даем команду на измерение напряжения

data[0] = 222;

CLKPR = (1<<CLKPCE);				// Разрешить изменение прескалера тактовой частоты
CLKPR = (1<<CLKPS1) | (1<<CLKPS0);			// Устанавливаем тактовую частоту 1 МГц
//CLKPR = (1<<CLKPS2);				// Устнавливаем частоту 500 кГц			

	//MCUCR |=	(0<<ISC01) | (1<<ISC00);

	
	   
    while (1) 
    {	
    	nRF_Transmit_NOACK(data);			// Отправить данные без подтверждения

	GIMSK = (1<<PCIE1);				// Разрешаем прерываание на пинах PCINT8..PCINT11
PCMSK1 = (1<<PCINT10);			// Выделяем маской прерывание только на PCINT10

	WDT_int_16mS;				// Взводим вачдог если не получим
						// прерывания от трансивера
	Sleep(SLEEP_MODE_PWR_DOWN);			// и уходим в сон
						//
	nRF_STOP();				// Переводим трансивер в StandBy-I
	GIMSK = 0;





	nRF_Reset_Interrupt();			// Как проснемся - сбрасываем флаг прерывания
	nRF_PowerDown();				// и выключаем RF часть трансивера

	PRR = 0xFF;				// Выключаем периферию

	nRF_Vcc_PORT &= ~ ((1<<nRF_Vcc) | (1<<SHT1x_Vcc));	// Выключаем трансивер и датчик

	Vcc_measurement--;
	temp = 2;

	do 
	{
		WDT_int_8S;			// Запускаем будильник на 8 сек.
		Sleep(SLEEP_MODE_PWR_DOWN);		// Уходим в глубокий сон	
	} while (temp--);	
    }
}

