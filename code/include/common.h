#pragma once

#include <string.h>

#include "stm32f4xx.h"
#include "rangfinder_2y0a21.h"
#include "uart_communication_interface.h"
#include "accelerometer_lis302.h"
#include "led_interface.h"
#include "filter2_iir.h"

#include "data_recorder.h"


#define CPU_CLK	((uint32_t)168000000)

class App
{

	DataRecorder < 2048, float > rec1;
	//DataRecorder < 512, uint16_t > rec2;

	float filterParams1[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	Filter2Iir filter1;

	// cykle w [ms]
	volatile uint32_t mainClock;
	volatile uint32_t auxClock;

public:


	Rangfinder2Y0A21 rang;
	AccelerometerLIS302 acc;
	UartCommunicationInterface com;


	App(): filter1(filterParams1)
	{
		mainClock = 0;
		auxClock = 0;
	};

	void GeneralHardwareInit()
	{
		// inicjalizacja mikrokontrolera
		SystemInit();

		// ustawienie zegara systemowego w programie
		if (SysTick_Config(CPU_CLK/1000))
		{
			while (1);
		}

		NVIC_EnableIRQ(DMA1_Stream6_IRQn);
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_EnableIRQ(SPI1_IRQn);
		NVIC_EnableIRQ(EXTI0_IRQn);
	}

	void Init()
	{
		GeneralHardwareInit();
		acc.Init();
	    rang.Init();
		com.Init();

		rang.Start();

	}

	void PeriodicUpdate()
	{
		mainClock++;
		auxClock++;

		com.PeriodicUpdate();
		rang.PeriodicUpdate();
		acc.WriteReadStart();

		if (auxClock == 500)
		{
		  Led::Green()^= 1;
		  auxClock = 0;
		}
	}

	void Run()
	{
		while(1)
		{
			if(com.isFrameReceived)
			{
				if(com.CheckFrame())
				{
					// przygotowanie danych do wyslania
					memcpy(com.txData, &filter1.output, sizeof(float));
					com.Send(sizeof(float));
				}
				com.isFrameReceived = false;
			}

			if(acc.isDataReady)
			{
				float out;
				acc.ScaleData();
				out = filter1.CalculateOutput(acc.accVal[0]);
				rec1.RecordCyclically(out);
				acc.isDataReady = false;
			}

			if(rang.isDataReady)
			{
				// obrobka sygnalu

			}
		}
	}

};

extern App * pApp;
