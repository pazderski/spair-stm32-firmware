#include "uart_communication_interface.h"

// Computation of  16-bit CRC (similar to Modbus RTU)
uint16_t UartCommunicationInterface::CRC16(const uint8_t *nData, uint16_t wLength)
{
  static const uint16_t wCRCTable[] =
  {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
  };

  uint8_t nTemp;
  uint16_t wCRCWord = 0xFFFF;

  while (wLength--)
  {
     nTemp = *nData++ ^ wCRCWord;
     wCRCWord >>= 8;
     wCRCWord ^= wCRCTable[nTemp];
  }
  return wCRCWord;
}

void UartCommunicationInterface::HardwareInit()
{

	// Switch on PA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Switch on clock for USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// Switch on clock for DMA1
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	// PA2 - UART2_TX (out), PA3 - UART_RX (in)
	// lines remapping (based on STM32F407 datasheet)
	GPIOA->AFR[0] |= (uint32_t) 7 << 8 | (uint32_t) 7 << 12;
	GPIOA->MODER |=  GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_1 | GPIO_OSPEEDER_OSPEEDR3_1;
	GPIOA->ODR |= GPIO_ODR_ODR_2; // podwieszenie linii

	// Transmission parameters
	USART2->BRR = (uint16_t) (84000000/115200/2); //0x16D; //115,2kbs //0x0341;//
	USART2->CR1 = USART_CR1_RE | USART_CR1_TE;
	USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;
	USART2->CR1 |= USART_CR1_UE;

	// DMA1: USART2 RX : stream 5,  USART2 TX : stream 6

	// RX - device->memory, auto-increment of the pointer memory, 8-bit chars, cyclic buffer
	DMA_USART_RX->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_MINC | DMA_SxCR_CIRC;
	DMA_USART_RX->NDTR = RX_BUF_SIZE;
	DMA_USART_RX->PAR = (uint32_t) &(USART2->DR);
	DMA_USART_RX->M0AR = (uint32_t) rxBuf;

	// TX - memory->device, auto-increment of the pointer memory
	DMA_USART_TX->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_MINC | DMA_SxCR_DIR_1;
	DMA_USART_TX->PAR = (uint32_t) &(USART2->DR);

	// Clear status bits (w tym flag b³êdów) kontrolera DMA1
	DMA1->HIFCR = DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;

	// Start DMA transmission
	DMA_USART_RX->CR |= DMA_SxCR_EN;
}

void UartCommunicationInterface::Init()
{

	HardwareInit();

	isFrameSending = false;
	isFrameReceived = false;

	rxDmaCounterPrev = DMA_USART_RX->NDTR;

	txData = txBuf + 4;
	rxData = rxFrame + 1;

	// Initialization of the frame header
	txBuf[0] = txBuf[1] = 0xAA; txBuf[2] = 0x00;
}

void UartCommunicationInterface::Send(uint16_t size)
{

	txBuf[3] = size;
	auto crc = CRC16(txBuf+3, size+1);
	auto crcFrame = (uint16_t*) &txBuf[size+4];
	*crcFrame = crc;

	// reset DMA stream
	DMA_USART_TX->CR = 0;
	DMA_USART_TX->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_MINC | DMA_SxCR_DIR_0;
	DMA_USART_TX->NDTR = size + 6;
	DMA_USART_TX->M0AR = (int) txBuf;

	// clear status bits (along with error flags) for DMA1
	DMA1->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6;
	USART2->SR &= ~(USART_SR_TC);

	isFrameSending = true;

	// Allow for interrupt from DMA stream
	DMA_USART_TX->CR |= DMA_SxCR_TCIE;
	// Start DMA transmission
	DMA_USART_TX->CR |= DMA_SxCR_EN;
}

bool UartCommunicationInterface::CheckFrame(void)
{
	auto crc1 = CRC16(rxFrame, rxFrameSize-1);
	auto crc2 = 0;

	// TO DO!!
	crc2 = crc1;
	if (crc1 == crc2)
	{
		return true;
	}
	else return false;
}

void UartCommunicationInterface::PeriodicUpdate()
{

  // Check if a frame is being evaluated or transmitted
  if (isFrameReceived || isFrameSending) return;

  // Check the timemout condition
  if (rxState != FR_IDLE)
  {
	  clock++;
	  if (clock > RX_TIMEOUT)
	  {
		  // Ignore a frame (it is not collected in the assumed time period)
		  rxState = FR_IDLE;
	  }
  }

  // Read DMA counter
  auto rxDmaCounter = DMA_USART_RX->NDTR;

  // Check if there are new characters
  if (rxDmaCounter != rxDmaCounterPrev)
  {

	  rxDmaCounterPrev = rxDmaCounter;
	  rxBufIndexWrite = RX_BUF_SIZE - rxDmaCounter;

	  // Look for stored characters in the cyclic buffer
	  while (rxBufIndexWrite != rxBufIndexRead)
	  {
		  // Read character from the buffer
		  auto c = rxBuf[rxBufIndexRead];

		  // Decode a frame (use FSM)
		  switch (rxState)
		  {
	  	  	  case FR_IDLE:
	  	  		  if (c == 0xAA)
		  	  	  {
	  	  			  rxState = FR_START_1;
		  	  		  clock = 0;
		  	  	  }
		  	  break;

		  	  case FR_START_1:
		  	  	  if (c == 0xAA)
		  	  		  rxState = FR_START_2;
		  	  	  else
		  	  		  rxState = FR_IDLE;
		  	  break;

		  	  case FR_START_2:
		  	      if (c == 0x00)
		  	  		  rxState = FR_SIZE;
		  	  	  else
		  	  		  rxState = FR_IDLE;
		  	  break;

		  	  case FR_SIZE:
		  		  rxFrameIndex = 0;
		  	  	  rxFrameSize = c+2;
		  	  	  rxFrame[rxFrameIndex] = c;
		  	  	  rxState = FR_DATA;
		  	  break;

		  	  case FR_DATA:
		  	  	  rxFrameIndex++;
		  	  	  rxFrame[rxFrameIndex] = c;
		  	  	  if (rxFrameIndex == rxFrameSize)
		  	  	  {
		  	  		  rxState = FR_IDLE;
		  	  		  isFrameReceived = true;
		  	  	  }
		  	  break;
		  }
	  // Update a buffer index (and limit its value)
      rxBufIndexRead = (++rxBufIndexRead) & (RX_BUF_SIZE-1);
	  }
  }
}


