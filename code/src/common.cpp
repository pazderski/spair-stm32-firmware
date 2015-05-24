#include "common.h"

extern "C"
{
	void SysTick_Handler(void);
	void EXTI0_IRQHandler(void);
	void SPI1_IRQHandler(void);
	void DMA1_Stream6_IRQHandler(void);
	void USART2_IRQHandler(void);
}

void Delay(volatile uint32_t count)
{
	while(count--) {};
}


void EXTI0_IRQHandler(void) {
	pApp->rang.Irq();
}

void SPI1_IRQHandler(void) {
	pApp->acc.Irq();
}

// Obsluga przerwania od nadajnika UART poprzez DMA
void DMA1_Stream6_IRQHandler()
{
	pApp->com.IrqDma();
}

// Obsluga przerwania od odbiornika/nadajnika USART2
void USART2_IRQHandler() {
	pApp->com.IrqTx();
}

void SysTick_Handler(void) {
	pApp->PeriodicUpdate();
}


