/*
 * nRF24L01P.c
 *
 *
 * Created: 24.05.2018 22:13:18
 *  Author: Andrey
 */ 

#include "nRF24L01P.h"
#include "Global_Settings.h"

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


	//... Функция отправки данных
void nRF_Transmit(uint8_t *tx_data)
{
	nRF_GO();								// Инициируем передачу

	SPI_WriteArray(nRF_W_TX_PAYLOAD, nRF_SEND_LEN, tx_data);			// Загрузить данные в буфер
}

	//.. Загрузить данные для отправки с пакетом ACK по каналу channel
uint8_t nRF_ACK_Transmit(uint8_t *tx_data, uint8_t channel)
{
	if (channel > 5) return 0;
	//<-- Вставить проверку FIFO
	SPI_WriteArray(nRF_W_ACK_PAYLOAD_MASK | channel, nRF_ACK_LEN, tx_data);

	return 1;
}

	//... Режим передатчика
void nRF_TX_Mode (void)
{
	uint8_t Buf[1];

	nRF_STOP();							// Нога CE опущена - выключаем прием и готовим плату к передаче 
									// данных
	//nRF_SELECT();

	Buf[0] = (1<<nRF_PWR_UP) | (0<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	// Переводим плату в режим передатчика
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);				// 

	//nRF_DESELECT();
}

	//... Режим приемника
void nRF_RX_Mode(void)
{
	uint8_t Buf[1];

	//nRF_SELECT();

	Buf[0] = (1<<nRF_PWR_UP) | (1<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	// Переводим плату в режим приема 
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);

	//nRF_DESELECT();

	nRF_GO();
}

	//... Вывести из сна
void nRF_PowerUp(void)
{
	uint8_t Buf[1];

	//nRF_SELECT();

	Buf[0] = (1<<nRF_PWR_UP) | (1<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	//Включить плату
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
	
	//nRF_DESELECT();
}

	//... Сон передатчика
void nRF_PowerDown(void)
{
	uint8_t Buf[1];

	Buf[0] = (0<<nRF_PWR_UP) | (1<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	//Выключить плату
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
}


	//... Очистить буфер FIFO
void nRF_ClearBuf(void)
{	
	nRF_SELECT();

	SPI_WriteByte(nRF_FLUSH_RX);				
	SPI_WriteByte(nRF_FLUSH_TX);

	nRF_DESELECT();
}

	//.. Инициализация трансивера
void nRF_Init(void)
 {
	 uint8_t Buf[5];

	nRF_CE_DDR |= (1<<nRF_CE);
	nRF_CSN_DDR |= (1<<nRF_CSN);

	nRF_CE_PORT |= (0<<nRF_CE);
	nRF_CSN_PORT |= (1<<nRF_CSN);

	//.. CONFIG
	Buf[0] =	(0<<nRF_MASK_RX_DR) | (0<<nRF_MASK_TX_DS) | (0<<nRF_MASK_MAX_RT) |	// маски прерываний от событий
 #if (DrumStick != no)
		(0<<nRF_PRIM_RX) |						// Режим передатчика для палочки
 #else
		(1<<nRF_PRIM_RX) |						// Режим приемника для MIDI приемника
 #endif
		(1<<nRF_EN_CRC) | (0<<nRF_CRCO) |				// Проверка CRC разрешена, 1 байт CRC
		(1<<nRF_PWR_UP);						// Запускаем трансивер
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);				// Отправляем команду. Пин CSN удерживается внутри функции

	//.. RF_CH  Настройка канала
	Buf[0] = nRF_channel;					// Установка частоты канала передачи
	SPI_WriteArray(nRF_WR_REG(nRF_RF_CH), 1, Buf);			// см. Global_Settings.h

	//.. RF_SETUP  Настройки радиоканала
	Buf[0] = (0<<nRF_RF_DR) | ((0x03)<<nRF_RF_PWR0);			// Скорость передачи 1 Mbps, мощность: 0dbm
	SPI_WriteArray(nRF_WR_REG(nRF_RF_SETUP), 1, Buf);		
	
	Buf[1] = RX_ADDR_msb;					
	Buf[2] = RX_ADDR_msb;
	Buf[3] = RX_ADDR_msb;
	Buf[4] = RX_ADDR_msb;

 #if (DrumStick == no)				// Если MIDI приемник
	//.. RX_ADDR_P0  Адрес канала 0 приемника
	Buf[0] = RX_ADDR_P0;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P0), 5, Buf);

	//.. RX_ADDR_P1  Адрес канала 1 приемника
	Buf[0] = RX_ADDR_P1;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P1), 5, Buf);

	//.. RX_ADDR_P2  Адрес канала 2 приемника
	Buf[0] = RX_ADDR_P2;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P2), 1, Buf);

	//.. RX_ADDR_P3  Адрес канала 3 приемника
	Buf[0] = RX_ADDR_P3;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P3), 1, Buf);

	//.. RX_ADDR_P4  Адрес канала 4 приемника
	Buf[0] = RX_ADDR_P4;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P4), 1, Buf);

	//.. RX_ADDR_P5  Адрес канала 5 приемника
	Buf[0] = RX_ADDR_P5;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P5), 1, Buf);

	//.. FEATURE  Опции
	Buf[0] = (1<<nRF_EN_DPL) | (1<<nRF_EN_ACK_PAY);			// Разрешаем данные
	SPI_WriteArray(nRF_WR_REG(nRF_FEATURE), 1, Buf);			// переменной длины

	//.. EN_AA  Автоматическая отправка ACK о приеме данных по каналу
	Buf[0] = (1<<nRF_ENAA_P0)|(1<<nRF_ENAA_P1)|(1<<nRF_ENAA_P2)|(1<<nRF_ENAA_P3)|(1<<nRF_ENAA_P4)|(1<<nRF_ENAA_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_AA), 1, Buf);

	//.. DYNPD  Прием данных переменной длины
	Buf[0] = (1<<nRF_DPL_P0)|(1<<nRF_DPL_P1)|(1<<nRF_DPL_P2)|(1<<nRF_DPL_P3)|(1<<nRF_DPL_P4)|(1<<nRF_DPL_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_DYNPD), 1, Buf);

	//.. EN_RXADDR  Используемые каналы приемника
	Buf[0] = (1<<nRF_ERX_P0)|(1<<nRF_ERX_P1)|(1<<nRF_ERX_P2)|(1<<nRF_ERX_P3)|(1<<nRF_ERX_P4)|(1<<nRF_ERX_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_RXADDR), 1, Buf);

/*	//.. Длина принимаемых данных по каналам
	Buf[0] = nRF_SEND_LEN;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P0), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P1), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P2), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P3), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P4), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P5), 1, Buf);*/
 #endif

 #if (DrumStick != no)				// Если палочка
	//.. RX_ADDR_P0  Адрес канала 0 приемника
	Buf[0] = RX_ADDR;				// На этот адрес будут приходить ACK пакеты
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P0), 5, Buf);

	//.. TX_ADDR  Адрес канала удаленного приемника
	SPI_WriteArray(nRF_WR_REG(nRF_TX_ADDR), 5, Buf);	// Адрес канала MIDI премника, на который будут уходить пакеты

	//.. EN_RXADDR  Используемые каналы приемника. На канале 0 принимается пакет автоподтверждения
	Buf[0] = (1<<nRF_ERX_P0)|(0<<nRF_ERX_P1)|(0<<nRF_ERX_P2)|(0<<nRF_ERX_P3)|(0<<nRF_ERX_P4)|(0<<nRF_ERX_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_RXADDR), 1, Buf);
/*
	//.. Длина принимаемых данных по каналам
	Buf[0] = nRF_ACK_LEN;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P0), 1, Buf);
 */		
	//.. SETUP_RETR  Настройка автоподтверждения
	Buf[0] = nRF_REPEAT_INTERVAL | nRF_REPEAT_MAX;			// Автоподтверждение и интервал повтора
	SPI_WriteArray(nRF_WR_REG(nRF_SETUP_RETR), 1, Buf);		// смотреть Settings.h

	//.. FEATURE  Опции
	Buf[0] = (1<<nRF_EN_DPL) | (1<<nRF_EN_ACK_PAY);			// Разрешаем прием данных
	SPI_WriteArray(nRF_WR_REG(nRF_FEATURE), 1, Buf);			// переменной длины

	//.. DYNPD  Прием данных переменной длины
	Buf[0] = (1<<nRF_DPL_P0)|(0<<nRF_DPL_P1)|(0<<nRF_DPL_P2)|(0<<nRF_DPL_P3)|(0<<nRF_DPL_P4)|(0<<nRF_DPL_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_DYNPD), 1, Buf);

	//.. EN_AA  Автоматическая отправка ACK о приеме данных по каналу
//	Buf[0] = (1<<nRF_ENAA_P0)|(0<<nRF_ENAA_P1)|(0<<nRF_ENAA_P2)|(0<<nRF_ENAA_P3)|(0<<nRF_ENAA_P4)|(0<<nRF_ENAA_P5);
//	SPI_WriteArray(nRF_WR_REG(nRF_EN_AA), 1, Buf);

 #endif
}
