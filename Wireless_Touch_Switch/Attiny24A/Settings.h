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
	DI / nRF_MISO	(PA6)  ->	|	| ->	(PA5) DO / nRF_MOSI
				-----------

				
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

	//.. ��������� ������� SPI ����� USI
#define	SPI_PORT			PORTA			// SPI ����
#define	SPI_DDR			DDRA

#define	MOSI			DO_BIT			// ����� MOSI
#define	MISO			DI_BIT			// ����� MISO
#define	SCK			USCK_BIT			// ����� �����
//#define	SS			nRF_CSN			// ����� CSN


	//... ��������� ������� �������������� � ����������� 
#define	nRF_CE_PORT		PORTA				//
#define	nRF_CE_DDR		DDRA				//
#define	nRF_CSN_PORT		PORTA
#define	nRF_CSN_DDR		DDRA

#define	nRF_CSN			PINA3			// ����� Chip Select
#define	nRF_CE			PINA2			// ����� Rx/Tx

#define	Vcc			PINA1			// ��� ��������� �������
#define	Vcc_PORT			PORTA

//#define nRF_channel			83			// ����� ������-��������

//#define nRF_REPEAT_MAX		15			// ���������� ������� ������� �������� ��� �������. 0 - ��������� ����������
//#define nRF_REPEAT_INTERVAL		nRF_RETR_250US		// �������� �����������
		



#endif /* SETTINGS_H_ */