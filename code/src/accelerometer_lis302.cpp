#include "accelerometer_lis302.h"

void AccelerometerLIS302::Fsm(uint16_t data) {

	switch (fsmState) {

		case STATE_IDLE:
		break;

		case STATE_STATUS:

			// Check ready bit
			if (data & ZYXDA_BIT)
			{
				fsmState = STATE_X_AXIS;
				SPI_CS() = 0;
				// Command to send
				SPI1->DR = OUTX;
			}
			else
			{
				// No data available
				fsmState = STATE_IDLE;
			}
		break;

		case STATE_X_AXIS:
			rawDataX = data;
			isDataReady = true;
			// Complete measurement cycle
			fsmState = STATE_IDLE;
		break;

		default:
		break;
	}

}

void AccelerometerLIS302::HardwareInit()
{

	// Set peripheral clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// GPIO: PA5 - SCK (out), PA6 - MISO (in), PA7 - MOSI (out), PE3 - SS (out)

	// Set speed of GPIOs
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1 | GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;
	// Set multiplexer (cf. datasheet of MCU)
	GPIOA->AFR[0] |= (uint32_t) 5 << (4*5) | (uint32_t) 5 << (4*6) | (uint32_t) 5 << (4*7);
	// Select mode of GPIOs
	GPIOA->MODER |= GPIO_MODER_MODER5_1 |GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
	GPIOE->MODER |= GPIO_MODER_MODER3_0;

	// SPI settings: 16-bit transmission with software control of CS line
	SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA | SPI_CR1_DFF;
	SPI1->CR2 = 0x00;
	// Switch on SPI
	SPI1->CR1 |= SPI_CR1_SPE;
}

