/*
 * ADC.h
 *
 * Created: 11.06.2018 0:30:05
 *  Author: Andrey
 */ 


#ifndef ADC_H_
#define ADC_H_


#define ADC_Trigger_T0_CompA		ADCSRB |= (1<<ADLAR) | (1<<ADTS1) | (1<<ADTS0)
#define ADC_Trigger_T0_Overflow	ADCSRB |= (1<<ADLAR) | (1<<ADTS2)
#define ADC_Trigger_T1_CompB		ADCSRB |= (1<<ADLAR) | (1<<ADTS2) | (1<<ADTS0)
#define ADC_Trigger_T1_Overflow	ADCSRB |= (1<<ADLAR) | (1<<ADTS2) | (1<<ADTS1)

#define ADC_Prescaler_2		ADCSRA |= (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0)
#define ADC_Prescaler_4		ADCSRA |= (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0)
#define ADC_Prescaler_8		(0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
#define ADC_Prescaler_16		ADCSRA |= (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0)
#define ADC_Prescaler_32		ADCSRA |= (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0)
#define ADC_Prescaler_64		ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0)
#define ADC_Prescaler_128		ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
#define AREF_internal		(1<<REFS1) | (0<<REFS0)			// AREF 1.1v
#define AREF_external		(0<<REFS1) | (1<<REFS0)			// AREF external (PA0)


 inline void ADC_Start_Conversion (void)
 {
	 ADCSRA =	(1<<ADEN) |				// ��������� ������ ADC
		(1<<ADIE) |				// ��������� ����������
		(1<<ADATE) |				// ����� �������������� �������
		(0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0) |		// ��������� = 1
		(1<<ADSC);				// ����� �����������
 }

inline void ADC_Off(void)
{
	ADCSRA =	0;
}

inline void ADC_Init (void)
{
	//DIDR0 = (1<<ADC1D) | (1<<ADC2D) | (1<<ADC3D);				// ��������� ���������� ���� �� �������� ������
	
	ADMUXA =	(1<<MUX0) | (1<<MUX2) | (1<<MUX3);			// �������� ���������� �� ���������� ��� 1.1�
									// ������������ �������� - ���������� �������
	ADCSRA =	(1<<ADEN) |						// ��������� ������ ADC
		(1<<ADIE) |						// ��������� ����������
		(0<<ADATE) |						// ����� �������������� �������
		(0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);				// ��������� = 2

	ADCSRB =	(1<<ADLAR);						// �������� ������ �� �������� 8-� ������
}

inline uint8_t ADC_Get_Data (void)
 {
	 return ADCH;
 }


#endif /* ADC_H_ */