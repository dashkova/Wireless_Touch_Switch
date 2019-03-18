/*
 * Settings.h
 * ������ ��������� ��������
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

	//.. ��������� ������� SPI ����� USI
#define	SPI_PORT			PORTA			// SPI ����
#define	SPI_DDR			DDRA

#define	MOSI_PIN			PINA6			// ����� MOSI
#define	MISO_PIN			PINA5			// ����� MISO
#define	SCK_PIN			PINA4			// ����� �����
//#define	SS			nRF_CSN			// ����� CSN


	//... ��������� ������� �������������� � ����������� 
#define	nRF_CE_PORT		PORTA				//
#define	nRF_CE_DDR		DDRA				//
#define	nRF_CSN_PORT		PORTA
#define	nRF_CSN_DDR		DDRA

#define	nRF_CSN			PINA3			// ����� Chip Select
#define	nRF_CE			PINA2			// ����� Rx/Tx
#define	nRF_IRQ			PINA7

	//... ��������� ������ �������
#define	Q0			PINA1
#define	Q1			PINB1
#define	Q2			PINA0
#define	Q3			PINB0

#define	Q0_PCINT			PCINT1
#define	Q1_PCINT			PCINT9
#define	Q2_PCINT			PCINT0
#define	Q3_PCINT			PCINT8

#define	Power			PINB2			// ��� ��������� �������
#define	Power_PORT		PORTB

//#define nRF_channel			83			// ����� ������-��������

//#define nRF_REPEAT_MAX		15			// ���������� ������� ������� �������� ��� �������. 0 - ��������� ����������
//#define nRF_REPEAT_INTERVAL		nRF_RETR_250US		// �������� �����������
		



#endif /* SETTINGS_H_ */