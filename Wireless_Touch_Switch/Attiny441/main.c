/*
 * main.c
 * Wireless_Touch_Switch
 *
 * Сенсорный радиовыключатель
 *
 * MK: Attiny441/841
 * Created: 16.03.2019 20:03:17
 * Author : parovoZZ
 *

 

	Структура пакета:
	0 - адрес, старший байт
	1 - адрес, младший байт
	2 - тип
	3 - поле состояния кнопок (Q3 - кнопка0, Q0 - кнопка1, Q2 - кнопка2, Q1 - кнопка3)
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
#include <avr/sleep.h>

#include "SPI.h"
#include "nRF24L01P.h"
#include "Settings.h"
#include "ADC.h"
#include "bits_macros.h"
#include "Global_Settings.h"

#define F_CPU		1000000UL


//.. Питание трансивера и МК
#define Vcc_On()		PORTB = (0<<Q1) | (0<<Q3) | (1<<Power)
#define Vcc_Off()		PORTB = (0<<Q1) | (0<<Q3) | (0<<Power)

//... Сторожевая собака
#define WDT_on		CCP = 0xD8; WDTCSR = Bit(WDE) | Bit(WDCE)
#define WDT_int_16mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) 			// Включение собаки на 16 мс и разрешение прерываний от нее
#define WDT_int_64mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP1)		// Включение собаки на 64 мс и разрешение прерываний от нее
#define WDT_int_125mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP1) | Bit(WDP0)	// Включение собаки на 125 мс и разрешение прерываний от нее
#define WDT_int_500mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP2) | Bit(WDP0)
#define WDT_int_8S		CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP3) | Bit(WDP0)	// Включение собаки на 8 с и разрешение прерываний от нее
#define WDT_off		CCP = 0xD8; WDT_on; WDTCSR = 0


volatile	uint8_t	data[5];

void Get_Buttons(void)
{
	data[3] &= 0b11110000;			// Обнуляем поле с кнопками	
	data[3] |= ((PINB & 0b11111110) << 0);		// Считываем кнопку Q3(0) и переносим ее на свое место в массиве
	data[3] |= ((PINA & 0b11111101) >> 0);		// Считываем кнопку Q0(1) и переносим ее на свое место в массиве
	data[3] |= ((PINA & 0b11111110) << 2);		// Считываем кнопку Q2(2) и переносим ее на свое место в массиве
	data[3] |= ((PINB & 0b11111101) << 2);		// Считываем кнопку Q1(3) и переносим ее на свое место в массиве
}

ISR(WDT_vect)
{
		
}

ISR(ADC_vect)
{
	data[4] = ADC_Get_Data();			// Получаем результат 

	ADC_Off();				// Запрещаем работу ADC

	PRR =	(1<<PRTWI) | (1<<PRUSART1) | (1<<PRUSART0) |
		(0<<PRSPI) | (1<<PRTIM2) | (1<<PRTIM1) |// Тактируем только SPI
		(1<<PRTIM0) | (1<<PRADC);	
}

ISR(PCINT1_vect)		// Прерывание от пинов PCINT8..PCINT11
{
	Get_Buttons();
}


ISR(PCINT0_vect)		// Прерывание от пинов PCINT0..PCINT7
{	
	Get_Buttons();
}

void Sleep(uint8_t Sleep_mode)
{
	CCP = 0xD8;
	set_sleep_mode(Sleep_mode);
	
	CCP = 0xD8;
	sleep_enable();

	CCP = 0xD8;
	sleep_cpu();						// Уходим в сон здесь

	CCP = 0xD8;						// Из прерывания возвращаемся сюда
	sleep_disable();
}

int main(void)
{
	PRR = 0xFF;				// Выключаем всю периферию

	DDRB =	(0<<Q1) | (0<<Q3) |	(1<<Power);	// Конфигурируем пин для питания на выход

	Vcc_On();					// Подаем питание на плату (через ключ)						

	WDT_int_125mS;				// Взводим будильник на 125 мс

	DDRA =	(0<<Q2) | (0<<Q0) |	(1<<nRF_CE) |	// 
		(1<<nRF_CSN) | (1<<SCK_PIN) | (0<<MISO_PIN) |	// 
		(1<<MOSI_PIN) | (0<<nRF_IRQ);		//	

	sei();
	
	Sleep(SLEEP_MODE_PWR_DOWN);			// Уходим в сон для ожидания запуска трансивера

	PRR =	(1<<PRTWI) | (1<<PRUSART1) | (1<<PRUSART0) |// Тактируем только ADC и SPI
		(0<<PRSPI) | (1<<PRTIM2) | (1<<PRTIM1) |
		(1<<PRTIM0) | (0<<PRADC);

	SPI_Init();

	ADC_Init();				// Инициализация ADC. Необходимо выдержать 1 мс
						// до начала измерения напряжения питания

	nRF_Init(nRF_channel);			// Конфигурируем регистры и там же
						// включаем RF часть трансивера для выдержки паузы
						// до начала сеанса передачи
	ADC_Start_Conversion();			// Даем команду на измерение напряжения

	GIMSK = (1<<PCIE1) | (1<<PCIE0);		// Разрешаем прерываание на пинах PCINT...

	PCMSK0 = (1<<Q0_PCINT) | (1<<Q2_PCINT);
	PCMSK1 = (1<<Q1_PCINT) | (1<<Q3_PCINT);		// Выделяем маской прерывание только на Q...

data[0] = 222;

CCP = 0xD8;					// Разрешить изменение прескалера тактовой частоты
CLKPR = (1<<CLKPS1) | (1<<CLKPS0);			// Устанавливаем тактовую частоту 1 МГц
//CLKPR = (1<<CLKPS2);				// Устнавливаем частоту 500 кГц				
	   
    while (1) 
    {	
	if (data[3] == 0)
	{
		nRF_PowerUp_interrupt();		// Включаем прерывание по отправке

		nRF_Transmit_NOACK(data);		// Отправить данные без подтверждения

		WDT_int_16mS;			// Взводим вачдог если не получим
						// прерывания от трансивера
		Sleep(SLEEP_MODE_PWR_DOWN);		// и уходим в сон

		Vcc_Off();			// Всё вырубаем
	} 

	nRF_Transmit_NOACK(data);			// Отправить данные без подтверждения

	nRF_STOP();				// Переводим трансивер в StandBy-I
	
	nRF_PowerDown();				// и выключаем RF часть трансивера

	WDT_int_8S;				// Взводим вачдог если не получим
						// прерывания от кнопок
	Sleep(SLEEP_MODE_PWR_DOWN);			// и уходим в сон	

	nRF_PowerUp();

	//PRR = 0xFF;				// Выключаем периферию

    }
}

