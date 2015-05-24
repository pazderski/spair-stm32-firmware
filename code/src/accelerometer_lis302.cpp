#include "accelerometer_lis302.h"

void AccelerometerLIS302::Fsm(uint16_t data) {

	switch (fsmState) {

		case STATE_IDLE:
		break;

		case STATE_STATUS:

			// sprawdzenie bitu gotowosci do odczytu danych
			if (data & ZYXDA_BIT)
			{
				fsmState = STATE_X_AXIS;
				SPI_CS() = 0;
				// komenda - rejestr wartosci akcelerometru w osi x
				SPI1->DR = OUTX;
			}
			else
			{
				// brak nowych danych
				fsmState = STATE_IDLE;
			}
		break;

		case STATE_X_AXIS:
			rawDataX = data;
			isDataReady = true;
			// koniec cyklu pomiaru
			fsmState = STATE_IDLE;
		break;

		default:
		break;
	}

}

void AccelerometerLIS302::HardwareInit()
{

	// za³aczenie CLK  dla peryferiów
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// GPIO: PA5 - SCK (out), PA6 - MISO (in), PA7 - MOSI (out), PE3 - SS (out)

	// ustawienie predkosci pracy linii portu
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1 | GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;
	// ustawienie multipleksera (wedlug dokumencjacji danego typu procesora)
	GPIOA->AFR[0] |= (uint32_t) 5 << (4*5) | (uint32_t) 5 << (4*6) | (uint32_t) 5 << (4*7);
	// wybor trybu pracy linii portu
	GPIOA->MODER |= GPIO_MODER_MODER5_1 |GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
	GPIOE->MODER |= GPIO_MODER_MODER3_0;

	// ustawienia SPI - transmisja 16-bitowa z programowa kontrola przeplywu
	SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA | SPI_CR1_DFF;
	SPI1->CR2 = 0x00;
	// wlaczenie urzadzenia
	SPI1->CR1 |= SPI_CR1_SPE;
}

