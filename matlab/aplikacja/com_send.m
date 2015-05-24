function com_send(message)
    global s;

    N = length(message);
    message = [uint8(N) message]; 
    [crcHi, crcLo] = crc16_calculate(message);
    
    ENQ=[170 170 00 message crcHi crcLo];
    fwrite(s,ENQ,'sync'); % wyslanie ramki zapytania
