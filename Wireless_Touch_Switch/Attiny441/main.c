/*
 * main.c
 * Wireless_Touch_Switch
 *
 * ��������� ����������������
 *
 * MK: Attiny441/841
 * Created: 16.03.2019 20:03:17
 * Author : parovoZZ
 *

 

	��������� ������:
	0 - �����, ������� ����
	1 - �����, ������� ����
	2 - ���
	3 - ���� ��������� ������ (Q3 - ������0, Q0 - ������1, Q2 - ������2, Q1 - ������3)
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
#include <avr/sleep.h>

#include "SPI.h"
#include "nRF24L01P.h"
#include "Settings.h"
#include "ADC.h"
#include "bits_macros.h"
#include "Global_Settings.h"

#define F_CPU		1000000UL


//.. ������� ���������� � ��
#define Vcc_On()		PORTB = (0<<Q1) | (0<<Q3) | (1<<Power)
#define Vcc_Off()		PORTB = (0<<Q1) | (0<<Q3) | (0<<Power)

//... ���������� ������
#define WDT_on		CCP = 0xD8; WDTCSR = Bit(WDE) | Bit(WDCE)
#define WDT_int_16mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) 			// ��������� ������ �� 16 �� � ���������� ���������� �� ���
#define WDT_int_64mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP1)		// ��������� ������ �� 64 �� � ���������� ���������� �� ���
#define WDT_int_125mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP1) | Bit(WDP0)	// ��������� ������ �� 125 �� � ���������� ���������� �� ���
#define WDT_int_500mS	CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP2) | Bit(WDP0)
#define WDT_int_8S		CCP = 0xD8; WDTCSR = Bit(WDIE) | Bit(WDE) | Bit(WDP3) | Bit(WDP0)	// ��������� ������ �� 8 � � ���������� ���������� �� ���
#define WDT_off		CCP = 0xD8; WDT_on; WDTCSR = 0


volatile	uint8_t	data[5];

void Get_Buttons(void)
{
	data[3] &= 0b11110000;			// �������� ���� � ��������	
	data[3] |= ((PINB & 0b11111110) << 0);		// ��������� ������ Q3(0) � ��������� �� �� ���� ����� � �������
	data[3] |= ((PINA & 0b11111101) >> 0);		// ��������� ������ Q0(1) � ��������� �� �� ���� ����� � �������
	data[3] |= ((PINA & 0b11111110) << 2);		// ��������� ������ Q2(2) � ��������� �� �� ���� ����� � �������
	data[3] |= ((PINB & 0b11111101) << 2);		// ��������� ������ Q1(3) � ��������� �� �� ���� ����� � �������
}

ISR(WDT_vect)
{
		
}

ISR(ADC_vect)
{
	data[4] = ADC_Get_Data();			// �������� ��������� 

	ADC_Off();				// ��������� ������ ADC

	PRR =	(1<<PRTWI) | (1<<PRUSART1) | (1<<PRUSART0) |
		(0<<PRSPI) | (1<<PRTIM2) | (1<<PRTIM1) |// ��������� ������ SPI
		(1<<PRTIM0) | (1<<PRADC);	
}

ISR(PCINT1_vect)		// ���������� �� ����� PCINT8..PCINT11
{
	Get_Buttons();
}


ISR(PCINT0_vect)		// ���������� �� ����� PCINT0..PCINT7
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
	sleep_cpu();						// ������ � ��� �����

	CCP = 0xD8;						// �� ���������� ������������ ����
	sleep_disable();
}

int main(void)
{
	PRR = 0xFF;				// ��������� ��� ���������

	DDRB =	(0<<Q1) | (0<<Q3) |	(1<<Power);	// ������������� ��� ��� ������� �� �����

	Vcc_On();					// ������ ������� �� ����� (����� ����)						

	WDT_int_125mS;				// ������� ��������� �� 125 ��

	DDRA =	(0<<Q2) | (0<<Q0) |	(1<<nRF_CE) |	// 
		(1<<nRF_CSN) | (1<<SCK_PIN) | (0<<MISO_PIN) |	// 
		(1<<MOSI_PIN) | (0<<nRF_IRQ);		//	

	sei();
	
	Sleep(SLEEP_MODE_PWR_DOWN);			// ������ � ��� ��� �������� ������� ����������

	PRR =	(1<<PRTWI) | (1<<PRUSART1) | (1<<PRUSART0) |// ��������� ������ ADC � SPI
		(0<<PRSPI) | (1<<PRTIM2) | (1<<PRTIM1) |
		(1<<PRTIM0) | (0<<PRADC);

	SPI_Init();

	ADC_Init();				// ������������� ADC. ���������� ��������� 1 ��
						// �� ������ ��������� ���������� �������

	nRF_Init(nRF_channel);			// ������������� �������� � ��� ��
						// �������� RF ����� ���������� ��� �������� �����
						// �� ������ ������ ��������
	ADC_Start_Conversion();			// ���� ������� �� ��������� ����������

	GIMSK = (1<<PCIE1) | (1<<PCIE0);		// ��������� ����������� �� ����� PCINT...

	PCMSK0 = (1<<Q0_PCINT) | (1<<Q2_PCINT);
	PCMSK1 = (1<<Q1_PCINT) | (1<<Q3_PCINT);		// �������� ������ ���������� ������ �� Q...

data[0] = 222;

CCP = 0xD8;					// ��������� ��������� ���������� �������� �������
CLKPR = (1<<CLKPS1) | (1<<CLKPS0);			// ������������� �������� ������� 1 ���
//CLKPR = (1<<CLKPS2);				// ������������ ������� 500 ���				
	   
    while (1) 
    {	
	if (data[3] == 0)
	{
		nRF_PowerUp_interrupt();		// �������� ���������� �� ��������

		nRF_Transmit_NOACK(data);		// ��������� ������ ��� �������������

		WDT_int_16mS;			// ������� ������ ���� �� �������
						// ���������� �� ����������
		Sleep(SLEEP_MODE_PWR_DOWN);		// � ������ � ���

		Vcc_Off();			// �� ��������
	} 

	nRF_Transmit_NOACK(data);			// ��������� ������ ��� �������������

	nRF_STOP();				// ��������� ��������� � StandBy-I
	
	nRF_PowerDown();				// � ��������� RF ����� ����������

	WDT_int_8S;				// ������� ������ ���� �� �������
						// ���������� �� ������
	Sleep(SLEEP_MODE_PWR_DOWN);			// � ������ � ���	

	nRF_PowerUp();

	//PRR = 0xFF;				// ��������� ���������

    }
}

