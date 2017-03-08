#pragma once
#include <windows.h>
#include <stdint.h>

class WinCom  
{
private:
	int portNumber;					
	TCHAR portName[10];					
	HANDLE hPort;						
	HANDLE sentSync, recvSync, prepareSync;
	
	static const int bufSize = 1024;	
	int state;
public:

    int error;
	bool receivedFrame;
	bool isClosed;
	uint8_t data[bufSize];	

	WinCom()
	{
        error = 0;
		portNumber = -1;
		isClosed = true;
		state = 1;
	}

	bool Send(const uint8_t *ptr, size_t len)
	{
		DWORD dummy;
		
		if(isClosed) {
			Sleep(10);
			return true;							
		}

		if(state == 2) // exit
		{
			return false;
		}
		else
		{
			receivedFrame = false;
			// clear the output buffer
            FlushFileBuffers(hPort);		
            if(!WriteFile(hPort, ptr, len, &dummy, 0) ) 
				return false;
        }
		return true;
	}

	uint32_t Receive()
	{
		DWORD readChars;
		ReadFile(hPort, data, bufSize, &readChars, 0);
		return readChars;
	}

	void Close()
	{
		if( !isClosed )
			if(!CloseHandle(hPort))
			{
				
			}
		isClosed = true;
	}

	int Open(int p, int baudrate)
	{
		COMMTIMEOUTS CommTimeOuts;
		DCB dcb;

		if (!isClosed) 
			return 0;

		//	determine name of the selected serial port
		if(portNumber==-1 )
		{
			#ifdef __OS_WIN95__
				wsprintf(portName, "COM%d", p);
			#else
                wsprintf(portName, "\\\\.\\COM%d", p);
			#endif
		}


		// set transmission parameters
		if ( (hPort=CreateFile(portName, GENERIC_READ | GENERIC_WRITE,
                  0,                    // exclusive access
                  NULL,                 // no security attrs
                  OPEN_EXISTING,
                  FILE_FLAG_NO_BUFFERING,
                  NULL ))==(HANDLE)-1 )
		{
			return -1;
		}
		SetupComm(hPort, 100, 100);

		CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;	
		CommTimeOuts.ReadTotalTimeoutConstant = 0;	
		CommTimeOuts.WriteTotalTimeoutMultiplier = 1;
		CommTimeOuts.WriteTotalTimeoutConstant = 0;
		SetCommTimeouts(hPort, &CommTimeOuts);
		GetCommState(hPort, &dcb);

		dcb.BaudRate = baudrate;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;

		if(!SetCommState(hPort, &dcb))
			return -2;
		isClosed = false;
		return 0;
	}
};
