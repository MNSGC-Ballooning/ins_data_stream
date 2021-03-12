function output = interpolate_gps(input)

output = input;

idx = find(diff(input.posE) ~=  0) + 1;

n = length(input.posE);

for i = 1:length(idx)    
    if i == length(idx)
        output.posE(idx(i):n) = interp1([output.imuSeconds(idx(i)) output.imuSeconds(n)],[output.posE(n) output.posE(n)], output.imuSeconds(idx(i):n));
        output.posN(idx(i):n) = interp1([output.imuSeconds(idx(i)) output.imuSeconds(n)],[output.posN(n) output.posN(n)], output.imuSeconds(idx(i):n));
        output.posU(idx(i):n) = interp1([output.imuSeconds(idx(i)) output.imuSeconds(n)],[output.posU(n) output.posU(n)], output.imuSeconds(idx(i):n));
    else
        output.posE(idx(i):idx(i+1)) = interp1([output.imuSeconds(idx(i)) output.imuSeconds(idx(i+1))]',[output.posE(idx(i)) output.posE(idx(i+1))]', output.imuSeconds(idx(i):idx(i+1)));
        output.posN(idx(i):idx(i+1)) = interp1([output.imuSeconds(idx(i)) output.imuSeconds(idx(i+1))],[output.posN(idx(i)) output.posN(idx(i+1))], output.imuSeconds(idx(i):idx(i+1)));
        output.posU(idx(i):idx(i+1)) = interp1([output.imuSeconds(idx(i)) output.imuSeconds(idx(i+1))],[output.posU(idx(i)) output.posU(idx(i+1))], output.imuSeconds(idx(i):idx(i+1)));
    end
end

end