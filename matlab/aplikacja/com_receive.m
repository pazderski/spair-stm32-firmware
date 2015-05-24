function [out, data_size] = com_receive()
global s;

    N = s.BytesAvailable;
    [msg, N, status]=fread(s, N);
    status;
    msg
    
    data_size = N - 6
    data_in = uint8(zeros(1, data_size));
    for i=1:data_size
        data_in(i) = uint8(msg(i+4));
    end
    
    out = data_in;
end