dc = diff(c);

VAL_TH = 25;
TIME_TH = 200;
p = 1;
k = 1;

n = size(dc, 1);
for i = 1:n
   if (abs(dc(i))> VAL_TH) 
        if ((i - p) > TIME_TH)
            k = i;
            break;
        else 
            p = i;
        end
   end
end
