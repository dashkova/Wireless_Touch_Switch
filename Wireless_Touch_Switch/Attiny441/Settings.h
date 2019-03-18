/*
 * Settings.h
 * Модуль локальных настроек
 *
 * Created:	10.06.2018 23:19:18
 *
 * Author:	Andrey
 *		parovoZZ
 *		dAshkova
 *

				-----------
			VCC	|	|	GND
		Q3	(PB0)  <-	|	| <-	(PA0) Q2
		Q1	(PB1)  <-	|	| <-	(PA1) Q0
		RESET	(PB3)  ->	|	| <-	(PA2) nRF_CE
		Power	(PB2)  <-	|	| <-	(PA3) nRF_CSN
		nRF_IRQ	(PA7)  -> |	| ->	(PA4) nRF_SCK
		nRF_MOSI	(PA6)  ->	|	| ->	(PA5) nRF_MISO
				-----------

				
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

	//.. Установки выводов SPI через USI
#define	SPI_PORT			PORTA			// SPI порт
#define	SPI_DDR			DDRA

#define	MOSI_PIN			PINA6			// линия MOSI
#define	MISO_PIN			PINA5			// линия MISO
#define	SCK_PIN			PINA4			// линия такта
//#define	SS			nRF_CSN			// линия CSN


	//... Установки выводов взаимодействия с трансивером 
#define	nRF_CE_PORT		PORTA				//
#define	nRF_CE_DDR		DDRA				//
#define	nRF_CSN_PORT		PORTA
#define	nRF_CSN_DDR		DDRA

#define	nRF_CSN			PINA3			// линия Chip Select
#define	nRF_CE			PINA2			// линия Rx/Tx
#define	nRF_IRQ			PINA7

	//... Установка прочих выводов
#define	Q0			PINA1
#define	Q1			PINB1
#define	Q2			PINA0
#define	Q3			PINB0

#define	Q0_PCINT			PCINT1
#define	Q1_PCINT			PCINT9
#define	Q2_PCINT			PCINT0
#define	Q3_PCINT			PCINT8

#define	Power			PINB2			// Пин включения питания
#define	Power_PORT		PORTB

//#define nRF_channel			83			// Канал приемо-передачи

//#define nRF_REPEAT_MAX		15			// Количество попыток повтора передачи при неудаче. 0 - отключить автоповтор
//#define nRF_REPEAT_INTERVAL		nRF_RETR_250US		// Интервал автоповтора
		



#endif /* SETTINGS_H_ */