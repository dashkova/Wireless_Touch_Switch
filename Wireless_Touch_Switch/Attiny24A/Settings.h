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
	DI / nRF_MISO	(PA6)  ->	|	| ->	(PA5) DO / nRF_MOSI
				-----------

				
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

	//.. Установки выводов SPI через USI
#define	SPI_PORT			PORTA			// SPI порт
#define	SPI_DDR			DDRA

#define	MOSI			DO_BIT			// линия MOSI
#define	MISO			DI_BIT			// линия MISO
#define	SCK			USCK_BIT			// линия такта
//#define	SS			nRF_CSN			// линия CSN


	//... Установки выводов взаимодействия с трансивером 
#define	nRF_CE_PORT		PORTA				//
#define	nRF_CE_DDR		DDRA				//
#define	nRF_CSN_PORT		PORTA
#define	nRF_CSN_DDR		DDRA

#define	nRF_CSN			PINA3			// линия Chip Select
#define	nRF_CE			PINA2			// линия Rx/Tx

#define	Vcc			PINA1			// Пин включения питания
#define	Vcc_PORT			PORTA

//#define nRF_channel			83			// Канал приемо-передачи

//#define nRF_REPEAT_MAX		15			// Количество попыток повтора передачи при неудаче. 0 - отключить автоповтор
//#define nRF_REPEAT_INTERVAL		nRF_RETR_250US		// Интервал автоповтора
		



#endif /* SETTINGS_H_ */