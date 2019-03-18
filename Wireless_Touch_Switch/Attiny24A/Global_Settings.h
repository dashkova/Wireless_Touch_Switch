/*
 * Global_Settings.h
 * ������ ���������� ��������
 *
 * Created:	16.01.2019
 *
 * Author:	Andrey
 *		parovoZZ
 *		Dashkova
 */ 


 #ifndef GLOBAL_SETTINGS_H_
 #define GLOBAL_SETTINGS_H_

	//.. ��������� ����������
 #define	RX_ADDR_msb		0xC2		// �������� ������ ��� BYTE4..1
 #define	RX_ADDR_P0		0xC1		// �������� �������� ����� ��� PIPE0
 #define	RX_ADDR_P1		0xC2		// �������� �������� ����� ��� PIPE1
 #define	RX_ADDR_P2		0xC3		// �������� �������� ����� ��� PIPE2
 #define	RX_ADDR_P3		0xC4		// �������� �������� ����� ��� PIPE3
 #define	RX_ADDR_P4		0xC5		// �������� �������� ����� ��� PIPE4
 #define	RX_ADDR_P5		0xC6		// �������� �������� ����� ��� PIPE5


 #if (DrumStick == 1)				// ��������� ������� ����������� �� ����� �1
	#define TX_ADDR		RX_ADDR_P0	// �����, �� ������� ����� ������������ ������
	#define RX_ADDR		RX_ADDR_P0	// �����, �� ������� ����� ����������� ACK ������

 #elif (DrumStick == 2)
	#define TX_ADDR		RX_ADDR_P1
	#define RX_ADDR		RX_ADDR_P1

 #elif (DrumStick == 3)
	#define TX_ADDR		RX_ADDR_P2
	#define RX_ADDR		RX_ADDR_P2

 #elif (DrumStick == 4)
	#define TX_ADDR		RX_ADDR_P3
	#define RX_ADDR		RX_ADDR_P3

 #elif (DrumStick == 5)
	#define TX_ADDR		RX_ADDR_P4
	#define RX_ADDR		RX_ADDR_P4
 
 #elif (DrumStick == 6)
	#define TX_ADDR		RX_ADDR_P5
	#define RX_ADDR		RX_ADDR_P5

#endif


 #define nRF_channel		83			// ����� ������-��������

 #define nRF_SEND_LEN		1			// ����� ������ ������ �� ������� � MIDI �����������
 #define nRF_ACK_LEN		3			// ����� ACK ������ �� MIDI ����������� � �������


	//.. ��� ����������� ������ ����������� ��� ���������
 #define no			0 



#endif /* GLOBAL_SETTINGS_H_ */