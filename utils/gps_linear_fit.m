function [output, inputRate] = gps_linear_fit(input,time)

% Function that takes a GPS position value and propogates the inputted
% value so as to ensure there are no "repeated" values.

fidx = find(input ~= 0, 1);
inputRate = zeros(length(input),1); 
output = input;

% find the indices where a "new" GPS value is recorded
newidx = find(diff(input(fidx:end)) > 0) + fidx;

% fill the input measurement rate vector according to the "new" GPS values
for i  = 2:length(newidx)
    dx = input(newidx(i)) - input(newidx(i-1));
    dt = time(newidx(i)) - time(newidx(i-1));
    
    inputRate(newidx(i-1):newidx(i)-1) = dx/dt; 
end

% using the rate vector, increment a new GPS position vector using linear
% fits determined by the rate vector
for j = newidx(2):length(input)
    if ~ismember(j,newidx)
        dt = time(j) - time(j-1);
        output(j) = output(j-1) + inputRate(j)*dt;
    end
end

end
    