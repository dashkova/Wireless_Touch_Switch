/*
 * Global_Settings.h
 * Модуль глобальных настроек
 *
 * Created:	16.01.2019
 *
 * Author:	Andrey
 *		parovoZZ
 *		Dashkova
 */ 


 #ifndef GLOBAL_SETTINGS_H_
 #define GLOBAL_SETTINGS_H_

	//.. Настройки трансивера
 #define	RX_ADDR_msb		0xC2		// Значения адреса для BYTE4..1
 #define	RX_ADDR_P0		0xC1		// Значение младшего байта для PIPE0
 #define	RX_ADDR_P1		0xC2		// Значение младшего байта для PIPE1
 #define	RX_ADDR_P2		0xC3		// Значение младшего байта для PIPE2
 #define	RX_ADDR_P3		0xC4		// Значение младшего байта для PIPE3
 #define	RX_ADDR_P4		0xC5		// Значение младшего байта для PIPE4
 #define	RX_ADDR_P5		0xC6		// Значение младшего байта для PIPE5


 #if (DrumStick == 1)				// Настройки адресов передатчика на палке №1
	#define TX_ADDR		RX_ADDR_P0	// Адрес, на который будут отправляться пакеты
	#define RX_ADDR		RX_ADDR_P0	// Адрес, на который будут приниматься ACK пакеты

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


 #define nRF_channel		83			// Канал приемо-передачи

 #define nRF_SEND_LEN		1			// Длина пакета данных от палочки к MIDI контроллеру
 #define nRF_ACK_LEN		3			// Длина ACK пакета от MIDI контроллера к палочке


	//.. Для возможности выбора передатчика или приемника
 #define no			0 



#endif /* GLOBAL_SETTINGS_H_ */