/*
 * nRF24L01P.c
 *
 *
 * Created: 24.05.2018 22:13:18
 *  Author: Andrey
 */ 

#include "nRF24L01P.h"
#include "Global_Settings.h"


#define CONFIG	(1<<nRF_MASK_RX_DR) | (1<<nRF_MASK_MAX_RT) | (0<<nRF_PRIM_RX) | (1<<nRF_EN_CRC) | (0<<nRF_CRCO)	// ����� ���������� �� �������
												// ����� �����������
												// �������� CRC ���������, 1 ���� CRC


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

	//.. ��������� ������ ��� �������� ��� ������ ACK 
 void nRF_Transmit_NOACK (uint8_t *data)
 {
	nRF_GO();								// ������������ �������� ������

	SPI_WriteArray(nRF_W_TX_PAYLOAD_NOACK, nRF_SEND_LEN, data);			// ��������� ������ � ���������� � ������ NO_ACK (��� �������������)
 }

	//... ������� �� ���
void nRF_PowerUp(void)
{
	uint8_t Buf[1];

	//nRF_SELECT();

	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (1<<nRF_MASK_TX_DS);			// ��������� ���������, ��������� ����������
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
	
	//nRF_DESELECT();
}

	//... ������� �� ���
void nRF_PowerUp_interrupt(void)
{
	uint8_t Buf[1];
		//nRF_SELECT();
	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (0<<nRF_MASK_TX_DS);			// ��������� ���������, �������� ���������� �� �������� ������
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
		
	//nRF_DESELECT();
}

	//... ��� �����������
void nRF_PowerDown(void)
{
	uint8_t Buf[1];

	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (1<<nRF_MASK_TX_DS);			// ��������� ���������, ��������� ����������
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);
}

	//.. ������������� ����������
 void nRF_Init(uint8_t channel)
 {
	 uint8_t Buf[5];

	//.. CONFIG
	Buf[0] =	CONFIG | (1<<nRF_PWR_UP) | (1<<nRF_MASK_TX_DS);			// ��������� ���������, ��������� ����������
	SPI_WriteArray(nRF_WR_REG(nRF_CONFIG), 1, Buf);				// ���������� �������. ��� CSN ������������ ������ �������
/*
	nRF_SELECT();							// ���������� ������� - ������ ��� CSN ���� ����� �������,
									// ����� ��������� ������ ����� ���������� ������� �������
	SPI_WriteByte(nRF_RD_REG(nRF_CONFIG));					// ������, ��� ��������
	Buf[1] = SPI_ReadByte(nRF_NOP);

	nRF_DESELECT();

	if (!(Buf[0] == Buf[1]))						// ���� �� ���������
	{
		//return 1;							// �� ������� �� �������
	}
*/
	//.. RF_CH  ��������� ������
	Buf[0] = channel;						// ��������� ������� ������ ��������
	SPI_WriteArray(nRF_WR_REG(nRF_RF_CH), 1, Buf);			// ��. Settings.h
/*
	nRF_SELECT();							// ���������� ������� - ������ ��� CSN ���� ����� �������,
									// ����� ��������� ������ ����� ���������� ������� �������
	SPI_WriteByte(nRF_RD_REG(nRF_RF_CH));					// ������, ��� ��������
	Buf[1] = SPI_ReadByte(nRF_NOP);

	nRF_DESELECT();
*/
	//.. ����� ����������� ������ �� �������
/*	Buf[0] = nRF_SEND_LEN;
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P0), 1, Buf);		
	
	Buf[0] = 0;						// ������ �� ������������ � ������
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P1), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P2), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P3), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P4), 1, Buf);
	SPI_WriteArray(nRF_WR_REG(nRF_RX_PW_P5), 1, Buf);*/

	//.. RF_SETUP  ��������� �����������
	Buf[0] = (0<<nRF_RF_DR) | ((0x03)<<nRF_RF_PWR0);			// �������� �������� 1 Mbps, ��������: 0dbm
	SPI_WriteArray(nRF_WR_REG(nRF_RF_SETUP), 1, Buf);

	//.. EN_AA  �������������� �������� ������ ������ ������ �� ������
/*	Buf[0] = (1<<nRF_ENAA_P0)|(1<<nRF_ENAA_P1)|(0<<nRF_ENAA_P2)|(0<<nRF_ENAA_P3)|(0<<nRF_ENAA_P4)|(0<<nRF_ENAA_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_AA), 1, Buf);	*/		

	//.. EN_RXADDR  ������������ ������ ���������. �� ������ 0 ����������� ����� �����������������
/*	Buf[0] = (1<<nRF_ERX_P0)|(1<<nRF_ERX_P1)|(0<<nRF_ERX_P2)|(0<<nRF_ERX_P3)|(0<<nRF_ERX_P4)|(0<<nRF_ERX_P5);
	SPI_WriteArray(nRF_WR_REG(nRF_EN_RXADDR), 1, Buf);	*/	

	//.. FEATURE  �����
	Buf[0] = (1<<nRF_EN_DYN_ACK);					// ��������� �������� ������� 
	SPI_WriteArray(nRF_WR_REG(nRF_FEATURE), 1, Buf);			// �� ��������� �������������

	//.. SETUP_AW  ����� ������
	//Buf[0] = (1<<nRF_AW0) | (1<<nRF_AW1);				// ����� ������ 5 ����
	//SPI_WriteArray(nRF_WR_REG(nRF_SETUP_AW), 1, Buf);

	//.. DYNPD  ����� ������ ���������� �����
	//Buf[0] = (0<<nRF_DPL_P0)|(0<<nRF_DPL_P1)|(0<<nRF_DPL_P2)|(0<<nRF_DPL_P3)|(0<<nRF_DPL_P4)|(0<<nRF_DPL_P5);
	//SPI_WriteArray(nRF_WR_REG(nRF_DYNPD), 1, Buf);

	//.. RX_ADDR_P0  ����� ������ 0 ���������
	Buf[0] = 0xC2;								//����� ������ 
	Buf[1] = 0xC2;
	Buf[2] = 0xC2;
	Buf[3] = 0xC2;
	Buf[4] = 0xC2;
//	SPI_WriteArray(nRF_WR_REG(nRF_RX_ADDR_P0), 5, Buf);

	//.. TX_ADDR  ����� ������ ���������� ���������
// 	Buf[0] = 0x11;								//����� ������ 
// 	Buf[1] = 0x11;
// 	Buf[2] = 0x11;
// 	Buf[3] = 0x11;
// 	Buf[4] = 0x11;
	SPI_WriteArray(nRF_WR_REG(nRF_TX_ADDR), 5, Buf);	// ����� ������ ��� ��������
	 
	//.. SETUP_RETR  ��������� �����������������
/*	Buf[0] = nRF_REPEAT_INTERVAL | nRF_REPEAT_MAX;			// ����������������� � �������� �������
	SPI_WriteArray(nRF_WR_REG(nRF_SETUP_RETR), 1, Buf);		// �������� Settings.h
*/
}
