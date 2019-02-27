% 
% Application for communication with STM32 microcontroller 
% in Matlab environment (for details see filr commdrv.cpp 
% and firmware for the microcontroller)
% SPAiR lab 2017
%

portNumber = 8;

if (commdrv('open', portNumber) == -1)
    return;
end

% sampling time
Ts = 0.05;

% lifespan
Tf = 5;

n = floor(Tf/Ts)+1;

% allocate buffers
D1 = zeros(1, n);
D2 = zeros(1, n);
time = zeros(1, n);

i = 1;
tau = 0;

tic;

while (tau < Tf)
       
    tau = toc;
    
    % prepare data to send (cf. CmdMaster structure)
    data1 = uint8(67*(sin(tau)+1));
    data2 = typecast(int16(tau*100), 'uint8');
    
    dataout = double([1 data1 data2]);

    commdrv('send', dataout);
    wait(tau, Ts);
    
    data = commdrv('read');
    
    % Check if new data is available
    if (~isempty(data))

        % decode the received data (cf. CmdSlave structure)
        D1(i) = typecast(data(1:2), 'uint16');
        D2(i) = typecast(data(3:4), 'int16');
        
        % store time read from PC
        time(i) = tau; 
        i = i+1;
    end
end   

% clear buffers (based on sample numbers)
D1 = D1(1:(i-1));
D2 = D2(1:(i-1));
time = time(1:(i-1));