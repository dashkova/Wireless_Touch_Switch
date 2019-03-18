/*
 * main.c
 * Wireless_Touch_Switch
 *
 * ��������� ����������������
 *
 * MK: Attiny24A/44A/84A
 * Created: 16.03.2019 20:03:17
 * Author : parovoZZ
 *

 

	��������� ������:
	0 - �����, ������� ����
	1 - �����, ������� ����
	2 - ���
	3 - ���� ��������� ������
	4 - ���������� �������
	5 - ���. ��������� (������)	????
	|__
		0 ���	��� PORF - �������� �� ������ �������
		1 ���	��� EXTRF - ������������ �� ������
		2 ���	��� BORF - ������������ �� ��������� �������
		3 ���	��� WDRF - ������������ �� �������
		4 ���
		5 ���	������ ��������� �
		6 ���	������ ��������� RH 
		7 ���	������ ������ � ������� �������						

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


//.. ������� ���������� � ��
#define Vcc_On()		Vcc_PORT |= Bit(Vcc)
#define Vcc_Off()		Vcc_PORT &= ~Bit(Vcc)

//... ���������� ������
#define WDT_on			WDTCSR = Bit(WDE) | Bit(WDCE)
#define WDT_int_16mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE)			// ��������� ������ �� 16 �� � ���������� ���������� �� ���
#define WDT_int_64mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP1)		// ��������� ������ �� 64 �� � ���������� ���������� �� ���
#define WDT_int_125mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP1) | Bit(WDP0)	// ��������� ������ �� 125 �� � ���������� ���������� �� ���
#define WDT_int_500mS		WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP2) | Bit(WDP0)
#define WDT_int_8S			WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDCE) | Bit(WDP3) | Bit(WDP0)	// ��������� ������ �� 8 � � ���������� ���������� �� ���
#define WDT_off			WDT_on; WDTCSR = 0

//.. ������ 1
#define T1_Start			TCCR1B = Bit (CS10) | Bit (WGM12)
#define T1_Set_Period(T)		OCR1A = (T)
#define T1_Clear			TCNT1 = 0


volatile	uint8_t	data[5];

ISR(WATCHDOG_vect)
{
		
}

ISR(ADC_vect)
{
	data[6] = ADC_Get_Data();			// �������� ��������� 
	ADC_Off();				// ��������� ������ ADC
	PRR = (1<<PRTIM0) | (1<<PRTIM1) | (1<<ADC);	// ��������� ������ USI
}

/*
ISR(EXT_INT0_vect)
{

}
*/

ISR(PCINT1_vect)		// ���������� �� ����� PCINT0..PCINT7
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

//	data[7] = MCUSR;				// ����� ������ ������������
//	MCUSR = 0;

	PRR = 0xFF;				// ��������� ��� ���������

	DDRA =	(1<<Vcc) | (0<<Q0) |		// ������������� ���� ��� ������� �� �����
		(0<<Q1) | (0<<Q2) |			// ���� ������ �������� ��� ����
		(1<<MOSI) | (1<<SCK) | (0<<MISO) |	// ���� ������ USI
		(1<<Q3);				// ��������� ��� �� �����

	PORTA =	(1<<Vcc) | (0<<Q0) |
		(0<<Pullup_SCK) | (0<<Pullup_DATA) |
		(0<<MOSI) | (0<<SCK) | (0<<MISO) |
		(0<<PINA0);
		
	Vcc_On();					// ������ ������� �� ����� (����� ����)

	WDT_int_125mS;				// ������� ��������� �� 125 ��

	PRR = (1<<PRTIM0) | (1<<PRTIM1) | (1<<ADC);

	USICR |= (Bit(USIWM0) | Bit(USICLK));		// ������������� USI �� ������ �� SPI	

	sei();
	
	Sleep(SLEEP_MODE_PWR_DOWN);			// ������ � ��� ��� �������� ������� ����������

	PRR = (1<<PRTIM0) | (1<<PRTIM1);		// ��������� ������ ADC � USI
	ADC_Init();				// ������������� ADC. ���������� ��������� 1 ��
						// �� ������ ��������� ���������� �������

	nRF_Init(nRF_channel);			// ������������� �������� � ��� ��
						// �������� RF ����� ���������� ��� �������� �����
						// �� ������ ������ ��������
	ADC_Start_Conversion();			// ���� ������� �� ��������� ����������

data[0] = 222;

CLKPR = (1<<CLKPCE);				// ��������� ��������� ���������� �������� �������
CLKPR = (1<<CLKPS1) | (1<<CLKPS0);			// ������������� �������� ������� 1 ���
//CLKPR = (1<<CLKPS2);				// ������������ ������� 500 ���			

	//MCUCR |=	(0<<ISC01) | (1<<ISC00);

	
	   
    while (1) 
    {	
    	nRF_Transmit_NOACK(data);			// ��������� ������ ��� �������������

	GIMSK = (1<<PCIE1);				// ��������� ����������� �� ����� PCINT8..PCINT11
PCMSK1 = (1<<PCINT10);			// �������� ������ ���������� ������ �� PCINT10

	WDT_int_16mS;				// ������� ������ ���� �� �������
						// ���������� �� ����������
	Sleep(SLEEP_MODE_PWR_DOWN);			// � ������ � ���
						//
	nRF_STOP();				// ��������� ��������� � StandBy-I
	GIMSK = 0;





	nRF_Reset_Interrupt();			// ��� ��������� - ���������� ���� ����������
	nRF_PowerDown();				// � ��������� RF ����� ����������

	PRR = 0xFF;				// ��������� ���������

	nRF_Vcc_PORT &= ~ ((1<<nRF_Vcc) | (1<<SHT1x_Vcc));	// ��������� ��������� � ������

	Vcc_measurement--;
	temp = 2;

	do 
	{
		WDT_int_8S;			// ��������� ��������� �� 8 ���.
		Sleep(SLEEP_MODE_PWR_DOWN);		// ������ � �������� ���	
	} while (temp--);	
    }
}

