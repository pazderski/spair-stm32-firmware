#pragma once

#include "stm32f4xx.h"
#include <string.h>

// DMA streams associated with the selected UART driver
#define DMA_USART_RX	DMA1_Stream5
#define DMA_USART_TX	DMA1_Stream6

class UartCommunicationInterface
{
	// finite states machine - declaration of states names
	enum FsmState
	{
		FR_IDLE,
		FR_START_1,
		FR_START_2,
		FR_SIZE,
		FR_DATA
	};

	// max time slot for a communication frame
	static uint16_t const RX_TIMEOUT = 50;

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
	uint8_t * rxData;

	volatile bool isFrameSending;
	volatile bool isFrameReceived;

	volatile uint8_t  rxFrameSize;

	void Init(void);
	void PeriodicUpdate();
	void Send(uint16_t size);
	bool CheckFrame(void);

	void SendUserData(void * data, uint16_t size)
	{
		memcpy(txData, data, size);
		Send(size);
	}

	void GetUserData(void * data, uint16_t size)
	{
		memcpy(data, rxData, size);
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

