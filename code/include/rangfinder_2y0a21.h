#pragma once

#include "stm32f4xx.h"

// ADC1 -> DMA2, stream 4
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

	void Init()
	{
		HardwareInit();
		isDataReady = 0;
		pulses = 0;
	}

	uint16_t ReadBlock()
	{
		uint16_t counter = 0;
		uint16_t dmaCounter, dmaIndexWrite;

		// Read DMA counter
		dmaCounter = DMA2_ADC->NDTR;

		// Check if there are new samples
		if (dmaCounter != dmaPrevCounter)
		{
			dmaPrevCounter = dmaCounter;
			// Compute index in DMA buffer
			dmaIndexWrite = DMA_SIZE - dmaCounter;

			while (dmaIndexWrite != dmaIndexRead)
			{
				// Copy data from DMA buffer to another buffer
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
			// Start the recording stage
		}
		if (pulses == 2)
		{
			ReadBlock();
			isDataReady = true;
			pulses = 0;
		}
	}

	void Irq()
	{
		pulses = 0;
		// Clear interrupt flag
		EXTI->PR = EXTI_PR_PR0;
	}
};
