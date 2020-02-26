#pragma once

#include "stm32f4xx_hal.h"
#include "accelerometer_lis302.hpp"
#include "data_recorder.hpp"
#include "led_interface.hpp"
#include "rangefinder_2y0a21.hpp"
#include "uart_communication_interface.hpp"
#include "filter2_iir.hpp"
#include "cmd_msgs.hpp"


void SystemClockConfig(void);

class App
{
	DataRecorder < 2048, float > rec1;
	//DataRecorder < 512, uint16_t > rec2;

	// TODO: define parameters of the filter
	float filterParams1[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	Filter2Iir filter1;

	// cycles in [ms]
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
		// microcontroller initialisation
		HAL_Init();
	    SystemClockConfig();

		NVIC_EnableIRQ(DMA1_Stream6_IRQn);
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_EnableIRQ(SPI1_IRQn);
		NVIC_EnableIRQ(EXTI0_IRQn);
	}

	void Init()
	{
		GeneralHardwareInit();

		Led::Init();
		acc.Init();
	    rang.Init();
		com.Init();
		//rang.Start();

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
					static CmdMaster cmdM;
					static CmdSlave cmdS;

					// analyze data from master
					com.GetUserData(&cmdM, sizeof(CmdMaster));

					// prepare data to send
					cmdS.data1 = cmdM.data1*5;
					cmdS.data2 = acc.accVal[0];

					com.SendUserData(&cmdS, sizeof(CmdSlave));
				}
				com.isFrameReceived = false;
			}

			if(acc.isDataReady)
			{
				float out;
				acc.ScaleData();
				out = filter1(acc.accVal[0]);
				//rec1.RecordCyclically(out);
				acc.isDataReady = false;
			}

			if(rang.isDataReady)
			{
				// Signal processing

			}
		}
	}

};

extern App * pApp;
