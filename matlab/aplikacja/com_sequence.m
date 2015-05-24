dataTab = zeros(1, 1000);
flushinput(s);
for i = 1:1000
    com_send([0]);
    pause(0.05);
    [data, data_size] = com_receive();
    % data
    acc = typecast(data, 'single')
   
    dataTab(i) = acc;
end    