/************************************************************************************************************
 *  Laboratorium SPAiR
 *
 *  Podstawowe oprogramowanie mikrokontrolera serii STM32F4xx
 *
 *  Zastosowanie w ukladzie pomiarowym
 *  Podstawowe funkcje oprogramowania:
 *   - zegar czasu rzeczywistego
 *   - obs�uga protoko�u komunikacyjnego z wykorzystaniem UART (z DMA)
 *   - obs�uga akcelerometru przez interfejs SPI
 *   - obs�uga przetwornika A/C (z DMA)
 *
 *   Opracowanie: D. Pazderski, Katedra Sterowania i In�ynierii System�w 2015-2017
 *   v. 1.6.0
 *
 ************************************************************************************************************/

#include "common.h"

App * pApp;

int main(void)
{
	App app;
	pApp = &app;
	app.Init();

	__enable_irq();

	app.Run();
}


