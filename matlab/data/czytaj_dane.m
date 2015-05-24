fid = fopen('data', 'r');
%c = fread(fid, 'int8')
c = fread(fid, 'float')

fclose(fid);