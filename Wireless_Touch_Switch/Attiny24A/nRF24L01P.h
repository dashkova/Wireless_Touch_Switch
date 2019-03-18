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

#if (DrumStick != no)
	#include "Drumstick__transmitter/usi_spi.h"
	#include "Drumstick__transmitter/Settings.h"
#else
	#include "MIDI_controller__receiver/spi.h"
	#include "MIDI_controller__receiver/Settings.h"
#endif



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
void nRF_Transmit(uint8_t *tx_data);
uint8_t nRF_ACK_Transmit(uint8_t *tx_data, uint8_t channel);
void nRF_TX_Mode (void);
void nRF_RX_Mode(void);
void nRF_PowerUp(void);
void nRF_PowerDown(void);
void nRF_ClearBuf(void);
void nRF_Init(void);

#endif /* NRF24L01P_H_ */