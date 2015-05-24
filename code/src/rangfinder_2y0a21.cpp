#include "rangfinder_2y0a21.h"

void Rangfinder2Y0A21::HardwareInit()
{
		// zalaczanie zegarów do uk³adów perfyferyjnych
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_SYSCFGEN;
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

		GPIOB->MODER |= GPIO_MODER_MODER4;
		GPIOB->BSRRH = (1<<9);
		GPIOB->BSRRL = (1<<3);

		GPIOC->OSPEEDR = GPIO_OSPEEDER_OSPEEDR1_1;
		GPIOC->MODER |= GPIO_MODER_MODER1; //analog
		GPIOC->IDR |= GPIO_IDR_IDR_1;


		// ustawienie przetwornika ADC
		ADC->CCR = ADC_CCR_ADCPRE_0;
		ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_CONT | ADC_CR2_DMA | ADC_CR2_DDS;
		ADC1->SMPR1 = ADC_SMPR1_SMP11_0 | ADC_SMPR1_SMP11_1 | ADC_SMPR1_SMP11_2;
		ADC1->SQR1 = 0;
		ADC1->SQR3 = ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_3;	//odczyt danych z kanalu 11 podczas pierwszej konwersji

		DMA2_ADC->CR = 0;
		DMA2_ADC->CR = DMA_SxCR_MINC | DMA_SxCR_PSIZE_0 | DMA_SxCR_MSIZE_0 | DMA_SxCR_CIRC;
		DMA2_ADC->NDTR = DMA_SIZE;
		DMA2_ADC->PAR = (int) & (ADC1->DR);
		DMA2_ADC->M0AR = (int) dmaBuf;

		dmaPrevCounter = DMA_SIZE;

		// zeruj bity statusowe (w tym flagi b³êdów) kontrolera DMA2
		DMA2->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4;
		DMA2_ADC->CR |= DMA_SxCR_EN;	// start kana³u DMA

		// wybor portu PB0 jako wejscia zdarzenia
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
		EXTI->EMR |= EXTI_EMR_MR0;
		EXTI->IMR |= EXTI_IMR_MR0;
		EXTI->FTSR |= EXTI_FTSR_TR0;	// falling edge
}


