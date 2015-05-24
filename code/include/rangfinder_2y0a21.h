#pragma once

#include "stm32f4xx.h"

// ADC1 -> DMA2, strumien 4
#define DMA2_ADC   DMA2_Stream4

class Rangfinder2Y0A21
{

	const static uint16_t DMA_SIZE = 4096;
	const static uint16_t BUF_SIZE = 4096;

	uint16_t dmaBuf[DMA_SIZE];
	uint16_t sampleBuf[BUF_SIZE];

	volatile uint16_t dmaIndexRead;
	volatile uint16_t dmaPrevCounter;

	volatile uint16_t sampleIndex;

	void HardwareInit();

	volatile uint16_t pulses;
public:

	bool isDataReady;

	void Init() {
		HardwareInit();
		isDataReady = 0;
		pulses = 0;
	}

	uint16_t ReadBlock()
	{
		uint16_t counter = 0;
		uint16_t dmaCounter, dmaIndexWrite;

		// odczyt licznika kanalu DMA
		dmaCounter = DMA2_ADC->NDTR;

		// sprawdzenie, czy w buforze sa nowe dane
		if (dmaCounter != dmaPrevCounter)
		{
			dmaPrevCounter = dmaCounter;
			// obliczenie indeksu w buforze zapisu w DMA
			dmaIndexWrite = DMA_SIZE - dmaCounter;

			while (dmaIndexWrite != dmaIndexRead)
			{
				// kopiowanie z buforu DMA do buforu programowego
				sampleBuf[sampleIndex++] = dmaBuf[dmaIndexRead++];
				sampleIndex &= BUF_SIZE-1;
				dmaIndexRead &= DMA_SIZE-1;
				counter++;
			}
		}
		return counter;
	}


	void Record()
	{
		uint16_t dmaIndexWrite;
		dmaIndexWrite = DMA_SIZE - DMA2_ADC->NDTR;
		dmaIndexRead = dmaIndexWrite;
		sampleIndex = 0;
	}

	void Start()
	{
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}

	void PeriodicUpdate()
	{
		pulses++;
		if (pulses == 1)
		{
			// rozpoczêcie cyklu rejestracji
		}
		if (pulses == 2) {
			ReadBlock();
			isDataReady = true;
			pulses = 0;
		}
	}

	void Irq()
	{
		pulses = 0;
		// wyzerowanie flagi zdarzenia
		EXTI->PR = EXTI_PR_PR0;
	}
};
