/*
 * nRF24L01P.c
 *
 *
 * Created: 24.05.2018 22:13:18
 *  Author: Andrey
 */ 

#include "nRF24L01P.h"
#include "Global_Settings.h"


#define CONFIG	(1<<nRF_MASK_RX_DR) | (1<<nRF_MASK_MAX_RT) | (0<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO)	// маски прерываний от событий
												// Режим передатчика
												// Проверка CRC разрешена, 1 байт CRC


//... Отправить несколько байт по SPI. cmd - команда, data - данные для отправки
void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data)
{
	nRF_SELECT();

	//.. Отправим команду
	SPI_WriteByte(cmd);

	//.. Затем данные
	while(num--)
	{
		SPI_WriteByte(*data++);
	}

	nRF_DESELECT();
}

//... Отправить и получить несколько байт по SPI. Принятые данные лежат в data
void SPI_ReadArray(uint8_t num, uint8_t *data)
{
	while(num--)
	{
		*data++ = SPI_WriteByte(*data);
	}
}


	//... Функция приема данных. Функция возвращает 0, если длина в буфере не совпадает
uint8_t nRF_Receive(struct Payload_t *Payload)
{
	uint8_t status;
	uint8_t length;

	nRF_SELECT();	
								
	status = SPI_WriteByte(nRF_R_RX_PL_WID);			// Запрашиваем длину сообщения
	length = SPI_WriteByte(nRF_NOP);				// Читаем длину сообщения

	nRF_DESELECT();

	SPI_WriteArray(nRF_WR_REG(nRF_STATUS), 1, &status);		// Сбрасываем все прерывания

	(*Payload).Pipe = (status >> 1) & (0b111);

	if ((*Payload).Pipe == 0b111)
	{			
		return 0;						// Если в приемнике ничего нет, то выходим
	}

	if (length == (*Payload).Length)				// Если длина ожидаемая, то читаем содержимое буфера
	{
		nRF_SELECT();

		SPI_WriteByte(nRF_R_RX_PAYLOAD);			// Команда на чтение буфера RX
		SPI_ReadArray(length, (*Payload).Data);			// Читаем данные

		nRF_DESELECT();

		return 1;
	}
	
	nRF_SELECT();

	SPI_WriteByte(nRF_FLUSH_RX);					// Сбросим буфер, если там мусор

	nRF_DESELECT();

	return 0;							// Выходим с нулем, если длина сообщения не совпадает
}

	//.. Загрузить данные для отправки без пакета ACK 
 void nRF_Transmit_NOACK (uint8_t *data)
 {
	nRF_GO();								// Инициировать передачу данных

	SPI_WriteArray(nRF_W_TX_PAYLOAD_NOACK, nRF_SEND_LEN, data);			// Загрузить данные в передатчик с флагом NO_ACK (без подтверждения)
 }

	//... Вывести из сна
void nRF_PowerUp(void)
{
	uint8_t Buf[1];

	//nRF_SELECT();

	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (1<<nRF_MASK_TX_DS);			// Запускаем трансивер, отключаем прерывания
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
	
	//nRF_DESELECT();
}

	//... Вывести из сна
void nRF_PowerUp_interrupt(void)
{
	uint8_t Buf[1];
		//nRF_SELECT();
	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (0<<nRF_MASK_TX_DS);			// Запускаем трансивер, включаем прерывание по отправке данных
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
		
	//nRF_DESELECT();
}

	//... Сон передатчика
void nRF_PowerDown(void)
{
	uint8_t Buf[1];

	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (1<<nRF_MASK_TX_DS);			// Выключаем трансивер, отключаем прерывания
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
}

	//.. Инициализация трансивера
 void nRF_Init(uint8_t channel)
 {
	 uint8_t Buf[5];

	//.. CONFIG
	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (1<<nRF_MASK_TX_DS);			// Запускаем трансивер, отключаем прерывания
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);				// Отправляем команду. Пин CSN удерживается внутри функции
/*
	nRF_SELECT();							// Инициируем общение - держим пин CSN весь сеанс общения,
									// иначе трансивер всегда будет возвращать регистр статуса
	SPI_WriteByte(nRF_RD_REG(nRF_CONFIG));					// Читаем, что записали
	Buf[1] = SPI_ReadByte(nRF_NOP);

	nRF_DESELECT();

	if (!(Buf[0] == Buf[1]))						// Если не совпадает
	{
		//return 1;							// то выходим из функции
	}
*/
	//.. RF_CH  Настройка канала
	Buf[0] = channel;						// Установка частоты канала передачи
	SPI_WriteArray(nRF_WR_REG(nRF_RF_CH), 1, Buf);			// см. Settings.h
/*
	nRF_SELECT();							// Инициируем общение - держим пин CSN весь сеанс общения,
									// иначе трансивер всегда будет возвращать регистр статуса
	SPI_WriteByte(nRF_RD_REG(nRF_RF_CH));					// Читаем, что записали
	Buf[1] = SPI_ReadByte(nRF_NOP);

	nRF_DESELECT();
*/
	//.. Длина принимаемых данных по каналам
/*	Buf[0] = nRF_SEND_LEN;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P0), 1, Buf);		
	
	Buf[0] = 0;						// каналы не используются в работе
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P1), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P2), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P3), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P4), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P5), 1, Buf);*/

	//.. RF_SETUP  Настройки радиоканала
	Buf[0] = (0<<nRF_RF_DR) | ((0x03)<<nRF_RF_PWR0);			// Скорость передачи 1 Mbps, мощность: 0dbm
	SPI_WriteArray(nRF_WR_REG(nRF_RF_SETUP), 1, Buf);

	//.. EN_AA  Автоматическая отправка пакета приема данных по каналу
/*	Buf[0] = (1<<nRF_ENAA_P0)|(1<<nRF_ENAA_P1)|(0<<nRF_ENAA_P2)|(0<<nRF_ENAA_P3)|(0<<nRF_ENAA_P4)|(0<<nRF_ENAA_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_AA), 1, Buf);	*/		

	//.. EN_RXADDR  Используемые каналы приемника. На канале 0 принимается пакет автоподтверждения
/*	Buf[0] = (1<<nRF_ERX_P0)|(1<<nRF_ERX_P1)|(0<<nRF_ERX_P2)|(0<<nRF_ERX_P3)|(0<<nRF_ERX_P4)|(0<<nRF_ERX_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_RXADDR), 1, Buf);	*/	

	//.. FEATURE  Опции
	Buf[0] = (1<<nRF_EN_DYN_ACK);					// Разрешаем отправку пакетов 
	SPI_WriteArray(nRF_WR_REG(nRF_FEATURE), 1, Buf);			// не требующих подтверждения

	//.. SETUP_AW  Длина адреса
	//Buf[0] = (1<<nRF_AW0) | (1<<nRF_AW1);				// Длина адреса 5 байт
	//SPI_WriteArray(nRF_WR_REG(nRF_SETUP_AW), 1, Buf);

	//.. DYNPD  Прием данных переменной длины
	//Buf[0] = (0<<nRF_DPL_P0)|(0<<nRF_DPL_P1)|(0<<nRF_DPL_P2)|(0<<nRF_DPL_P3)|(0<<nRF_DPL_P4)|(0<<nRF_DPL_P5);
	//SPI_WriteArray(nRF_WR_REG(nRF_DYNPD), 1, Buf);

	//.. RX_ADDR_P0  Адрес канала 0 приемника
	Buf[0] = 0xC2;								//адрес канала 
	Buf[1] = 0xC2;
	Buf[2] = 0xC2;
	Buf[3] = 0xC2;
	Buf[4] = 0xC2;
//	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P0), 5, Buf);

	//.. TX_ADDR  Адрес канала удаленного приемника
// 	Buf[0] = 0x11;								//адрес канала 
// 	Buf[1] = 0x11;
// 	Buf[2] = 0x11;
// 	Buf[3] = 0x11;
// 	Buf[4] = 0x11;
	SPI_WriteArray(nRF_WR_REG(nRF_TX_ADDR), 5, Buf);	// Адрес канала для передачи
	 
	//.. SETUP_RETR  Настройка автоподтверждения
/*	Buf[0] = nRF_REPEAT_INTERVAL | nRF_REPEAT_MAX;			// Автоподтверждение и интервал повтора
	SPI_WriteArray(nRF_WR_REG(nRF_SETUP_RETR), 1, Buf);		// смотреть Settings.h
*/
}
