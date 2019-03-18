/*
 * ������ � ������� nRF24L01P ����� spi
 *
 */ 


#ifndef NRF24L01P_H_
#define NRF24L01P_H_



#include <avr/io.h>
#include <avr/interrupt.h>
#include "nRF24L01P_Reg.h"
#include "bits_macros.h"
#include "spi.h"
#include "Settings.h"


#define nRF_SELECT()			ClearBit(nRF_CSN_PORT, nRF_CSN)
#define nRF_DESELECT()			SetBit(nRF_CSN_PORT, nRF_CSN)

#define nRF_GO()				SetBit(nRF_CE_PORT, nRF_CE)
#define nRF_STOP()				ClearBit(nRF_CE_PORT, nRF_CE)

//#define nRF_Init_IRQ			GIMSK |= Bit(nRF_IRQ)		// ������������� �� ���������� �� �����������

 struct Payload_t					// ��������� � �������
 {
	uint8_t	*Data;				// ��������� �� ������
	uint8_t	Length;				// ������ ������ � ������
	uint8_t	Pipe;				// Pipe, �� ������� ������� ������ 
 };


void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data);
uint8_t nRF_Receive(struct Payload_t *Payload);
void nRF_Transmit_NOACK (uint8_t *data);
void nRF_PowerUp_interrupt(void);
void nRF_PowerUp(void);
void nRF_PowerDown(void);
void nRF_Init(uint8_t channel);

#endif /* NRF24L01P_H_ */