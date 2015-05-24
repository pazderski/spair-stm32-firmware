#pragma once

#include "stm32f4xx.h"

// powiazania kanalow DMA z nadajnikiem i odbiornikiem UART
#define DMA_USART_RX	DMA1_Stream5
#define DMA_USART_TX	DMA1_Stream6

class UartCommunicationInterface {

	// definicje stanow automatu do odbioru ramek komunikacyjnych
	enum FsmState
	{
		FR_IDLE,
		FR_START_1,
		FR_START_2,
		FR_SIZE,
		FR_DATA
	};

	static uint16_t const RX_TIMEOUT = 50;	// definicja maksymalnego czasu przesylania danych w ramce

	volatile uint8_t  rxFrameIndex;
	volatile FsmState  rxState;

	volatile uint16_t rxBufIndexRead;
	uint16_t rxBufIndexWrite;

	uint16_t rxDmaCounterPrev;
	volatile uint32_t clock;

    volatile uint8_t  crcLo;
	volatile uint8_t  crcHi;

	static uint16_t const RX_BUF_SIZE = 256;
	static uint16_t const TX_BUF_SIZE = 256;


	uint8_t rxBuf[RX_BUF_SIZE];
	uint8_t txBuf[TX_BUF_SIZE];

	uint8_t rxFrame[RX_BUF_SIZE];

	uint16_t CRC16(const uint8_t *nData, uint16_t wLength);

	void HardwareInit();

public:

	uint8_t * txData;
	volatile bool isFrameSending;
	volatile bool isFrameReceived;

	volatile uint8_t  rxFrameSize;

	void Init(void);
	void PeriodicUpdate();
	void Send(uint16_t size);

	bool CheckFrame(void)
	{
		auto crc1 = CRC16(rxFrame, rxFrameSize-1);
		auto crc2 = __REV16(0);

		// tymczasowo
		crc2 = crc1;
		if (crc1 == crc2)
		{
			return true;
		}
		else return false;
	}

	void IrqDma()
	{
		DMA1->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6;
		DMA_USART_TX->CR &= ~DMA_SxCR_EN;
		USART2->CR1 |= USART_CR1_TCIE;
	}

	void IrqTx()
	{
		if (USART2->SR & USART_SR_TC)
		{
		    USART2->CR1 &= ~USART_CR1_TCIE;
		    isFrameSending = false;
		}
	}
};

