/*
 * nRF24L01P.c
 *
 *
 * Created: 24.05.2018 22:13:18
 *  Author: Andrey
 */ 

#include "nRF24L01P.h"
#include "Global_Settings.h"

//... ��������� ��������� ���� �� SPI. cmd - �������, data - ������ ��� ��������
void SPI_WriteArray(uint8_t cmd, uint8_t num, uint8_t *data)
{
	nRF_SELECT();

	//.. �������� �������
	SPI_WriteByte(cmd);

	//.. ����� ������
	while(num--)
	{
		SPI_WriteByte(*data++);
	}

	nRF_DESELECT();
}

//... ��������� � �������� ��������� ���� �� SPI. �������� ������ ����� � data
void SPI_ReadArray(uint8_t num, uint8_t *data)
{
	while(num--)
	{
		*data++ = SPI_WriteByte(*data);
	}
}


	//... ������� ������ ������. ������� ���������� 0, ���� ����� � ������ �� ���������
uint8_t nRF_Receive(struct Payload_t *Payload)
{
	uint8_t status;
	uint8_t length;

	nRF_SELECT();	
								
	status = SPI_WriteByte(nRF_R_RX_PL_WID);			// ����������� ����� ���������
	length = SPI_WriteByte(nRF_NOP);				// ������ ����� ���������

	nRF_DESELECT();

	SPI_WriteArray(nRF_WR_REG(nRF_STATUS), 1, &status);		// ���������� ��� ����������

	(*Payload).Pipe = (status >> 1) & (0b111);

	if ((*Payload).Pipe == 0b111)
	{			
		return 0;						// ���� � ��������� ������ ���, �� �������
	}

	if (length == (*Payload).Length)				// ���� ����� ���������, �� ������ ���������� ������
	{
		nRF_SELECT();

		SPI_WriteByte(nRF_R_RX_PAYLOAD);			// ������� �� ������ ������ RX
		SPI_ReadArray(length, (*Payload).Data);			// ������ ������

		nRF_DESELECT();

		return 1;
	}
	
	nRF_SELECT();

	SPI_WriteByte(nRF_FLUSH_RX);					// ������� �����, ���� ��� �����

	nRF_DESELECT();

	return 0;							// ������� � �����, ���� ����� ��������� �� ���������
}


	//... ������� �������� ������
void nRF_Transmit(uint8_t *tx_data)
{
	nRF_GO();								// ���������� ��������

	SPI_WriteArray(nRF_W_TX_PAYLOAD, nRF_SEND_LEN, tx_data);			// ��������� ������ � �����
}

	//.. ��������� ������ ��� �������� � ������� ACK �� ������ channel
uint8_t nRF_ACK_Transmit(uint8_t *tx_data, uint8_t channel)
{
	if (channel > 5) return 0;
	//<-- �������� �������� FIFO
	SPI_WriteArray(nRF_W_ACK_PAYLOAD_MASK | channel, nRF_ACK_LEN, tx_data);

	return 1;
}

	//... ����� �����������
void nRF_TX_Mode (void)
{
	uint8_t Buf[1];

	nRF_STOP();							// ���� CE ������� - ��������� ����� � ������� ����� � �������� 
									// ������
	//nRF_SELECT();

	Buf[0] = (1<<nRF_PWR_UP) | (0<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	// ��������� ����� � ����� �����������
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);				// 

	//nRF_DESELECT();
}

	//... ����� ���������
void nRF_RX_Mode(void)
{
	uint8_t Buf[1];

	//nRF_SELECT();

	Buf[0] = (1<<nRF_PWR_UP) | (1<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	// ��������� ����� � ����� ������ 
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);

	//nRF_DESELECT();

	nRF_GO();
}

	//... ������� �� ���
void nRF_PowerUp(void)
{
	uint8_t Buf[1];

	//nRF_SELECT();

	Buf[0] = (1<<nRF_PWR_UP) | (1<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	//�������� �����
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
	
	//nRF_DESELECT();
}

	//... ��� �����������
void nRF_PowerDown(void)
{
	uint8_t Buf[1];

	Buf[0] = (0<<nRF_PWR_UP) | (1<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO);	//��������� �����
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
}


	//... �������� ����� FIFO
void nRF_ClearBuf(void)
{	
	nRF_SELECT();

	SPI_WriteByte(nRF_FLUSH_RX);				
	SPI_WriteByte(nRF_FLUSH_TX);

	nRF_DESELECT();
}

	//.. ������������� ����������
void nRF_Init(void)
 {
	 uint8_t Buf[5];

	nRF_CE_DDR |= (1<<nRF_CE);
	nRF_CSN_DDR |= (1<<nRF_CSN);

	nRF_CE_PORT |= (0<<nRF_CE);
	nRF_CSN_PORT |= (1<<nRF_CSN);

	//.. CONFIG
	Buf[0] =	(0<<nRF_MASK_RX_DR) | (0<<nRF_MASK_TX_DS) | (0<<nRF_MASK_MAX_RT) |	// ����� ���������� �� �������
 #if (DrumStick != no)
		(0<<nRF_PRIM_RX) |						// ����� ����������� ��� �������
 #else
		(1<<nRF_PRIM_RX) |						// ����� ��������� ��� MIDI ���������
 #endif
		(1<<nRF_EN_CRC) | (0<<nRF_CRCO) |				// �������� CRC ���������, 1 ���� CRC
		(1<<nRF_PWR_UP);						// ��������� ���������
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);				// ���������� �������. ��� CSN ������������ ������ �������

	//.. RF_CH  ��������� ������
	Buf[0] = nRF_channel;					// ��������� ������� ������ ��������
	SPI_WriteArray(nRF_WR_REG(nRF_RF_CH), 1, Buf);			// ��. Global_Settings.h

	//.. RF_SETUP  ��������� �����������
	Buf[0] = (0<<nRF_RF_DR) | ((0x03)<<nRF_RF_PWR0);			// �������� �������� 1 Mbps, ��������: 0dbm
	SPI_WriteArray(nRF_WR_REG(nRF_RF_SETUP), 1, Buf);		
	
	Buf[1] = RX_ADDR_msb;					
	Buf[2] = RX_ADDR_msb;
	Buf[3] = RX_ADDR_msb;
	Buf[4] = RX_ADDR_msb;

 #if (DrumStick == no)				// ���� MIDI ��������
	//.. RX_ADDR_P0  ����� ������ 0 ���������
	Buf[0] = RX_ADDR_P0;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P0), 5, Buf);

	//.. RX_ADDR_P1  ����� ������ 1 ���������
	Buf[0] = RX_ADDR_P1;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P1), 5, Buf);

	//.. RX_ADDR_P2  ����� ������ 2 ���������
	Buf[0] = RX_ADDR_P2;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P2), 1, Buf);

	//.. RX_ADDR_P3  ����� ������ 3 ���������
	Buf[0] = RX_ADDR_P3;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P3), 1, Buf);

	//.. RX_ADDR_P4  ����� ������ 4 ���������
	Buf[0] = RX_ADDR_P4;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P4), 1, Buf);

	//.. RX_ADDR_P5  ����� ������ 5 ���������
	Buf[0] = RX_ADDR_P5;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P5), 1, Buf);

	//.. FEATURE  �����
	Buf[0] = (1<<nRF_EN_DPL) | (1<<nRF_EN_ACK_PAY);			// ��������� ������
	SPI_WriteArray(nRF_WR_REG(nRF_FEATURE), 1, Buf);			// ���������� �����

	//.. EN_AA  �������������� �������� ACK � ������ ������ �� ������
	Buf[0] = (1<<nRF_ENAA_P0)|(1<<nRF_ENAA_P1)|(1<<nRF_ENAA_P2)|(1<<nRF_ENAA_P3)|(1<<nRF_ENAA_P4)|(1<<nRF_ENAA_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_AA), 1, Buf);

	//.. DYNPD  ����� ������ ���������� �����
	Buf[0] = (1<<nRF_DPL_P0)|(1<<nRF_DPL_P1)|(1<<nRF_DPL_P2)|(1<<nRF_DPL_P3)|(1<<nRF_DPL_P4)|(1<<nRF_DPL_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_DYNPD), 1, Buf);

	//.. EN_RXADDR  ������������ ������ ���������
	Buf[0] = (1<<nRF_ERX_P0)|(1<<nRF_ERX_P1)|(1<<nRF_ERX_P2)|(1<<nRF_ERX_P3)|(1<<nRF_ERX_P4)|(1<<nRF_ERX_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_RXADDR), 1, Buf);

/*	//.. ����� ����������� ������ �� �������
	Buf[0] = nRF_SEND_LEN;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P0), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P1), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P2), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P3), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P4), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P5), 1, Buf);*/
 #endif

 #if (DrumStick != no)				// ���� �������
	//.. RX_ADDR_P0  ����� ������ 0 ���������
	Buf[0] = RX_ADDR;				// �� ���� ����� ����� ��������� ACK ������
	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P0), 5, Buf);

	//.. TX_ADDR  ����� ������ ���������� ���������
	SPI_WriteArray(nRF_WR_REG(nRF_TX_ADDR), 5, Buf);	// ����� ������ MIDI ��������, �� ������� ����� ������� ������

	//.. EN_RXADDR  ������������ ������ ���������. �� ������ 0 ����������� ����� �����������������
	Buf[0] = (1<<nRF_ERX_P0)|(0<<nRF_ERX_P1)|(0<<nRF_ERX_P2)|(0<<nRF_ERX_P3)|(0<<nRF_ERX_P4)|(0<<nRF_ERX_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_RXADDR), 1, Buf);
/*
	//.. ����� ����������� ������ �� �������
	Buf[0] = nRF_ACK_LEN;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P0), 1, Buf);
 */		
	//.. SETUP_RETR  ��������� �����������������
	Buf[0] = nRF_REPEAT_INTERVAL | nRF_REPEAT_MAX;			// ����������������� � �������� �������
	SPI_WriteArray(nRF_WR_REG(nRF_SETUP_RETR), 1, Buf);		// �������� Settings.h

	//.. FEATURE  �����
	Buf[0] = (1<<nRF_EN_DPL) | (1<<nRF_EN_ACK_PAY);			// ��������� ����� ������
	SPI_WriteArray(nRF_WR_REG(nRF_FEATURE), 1, Buf);			// ���������� �����

	//.. DYNPD  ����� ������ ���������� �����
	Buf[0] = (1<<nRF_DPL_P0)|(0<<nRF_DPL_P1)|(0<<nRF_DPL_P2)|(0<<nRF_DPL_P3)|(0<<nRF_DPL_P4)|(0<<nRF_DPL_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_DYNPD), 1, Buf);

	//.. EN_AA  �������������� �������� ACK � ������ ������ �� ������
//	Buf[0] = (1<<nRF_ENAA_P0)|(0<<nRF_ENAA_P1)|(0<<nRF_ENAA_P2)|(0<<nRF_ENAA_P3)|(0<<nRF_ENAA_P4)|(0<<nRF_ENAA_P5);
//	SPI_WriteArray(nRF_WR_REG(nRF_EN_AA), 1, Buf);

 #endif
}
