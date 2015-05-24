function [highByte, lowByte] = crc16_calculate(data)

    % Appends the crc (Low byte, high byte) to message for modbus
    % communication. Message is an array of bytes. Developed for (but not
    % limmited to) use with a watlow 96 controller.

    N = length(data);
    crc = hex2dec('ffff');
    polynomial = hex2dec('a001');

    for i = 1:N
        crc = bitxor(crc,uint16(data(i)));
        for j = 1:8
            if bitand(crc,1)
                crc = bitshift(crc,-1);
                crc = bitxor(crc,polynomial);
            else
                crc = bitshift(crc,-1);
            end
        end
    end

    lowByte = bitand(crc,hex2dec('ff'));
    highByte = bitshift(bitand(crc,hex2dec('ff00')),-8);
end