/************************************************************************************************************
 *  Laboratorium SPAiR
 *
 *  Podstawowe oprogramowanie mikrokontrolera serii STM32F4xx
 *
 *  Zastosowanie w ukladzie pomiarowym
 *  Podstawowe funkcje oprogramowania:
 *   - zegar czasu rzeczywistego
 *   - obs³uga protoko³u komunikacyjnego z wykorzystaniem UART (z DMA)
 *   - obs³uga akcelerometru przez interfejs SPI
 *   - obs³uga przetwornika A/C (z DMA)
 *
 *   Opracowanie: D. Pazderski, Katedra Sterowania i In¿ynierii Systemów 2015
 *   v. 1.5.0
 *
 ************************************************************************************************************/

#include "common.h"

App * pApp;

int main(void)
{
	App app;
	pApp = &app;
	app.Init();
	Led::Init();

	__enable_irq();

	app.Run();
}


