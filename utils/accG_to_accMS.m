function output = accG_to_accMS(input)

output = input;
g = 9.80655;        % m/s/s

output.accX = input.accX.*(g/1000);
output.accY = input.accY.*(g/1000);
output.accZ = input.accZ.*(g/1000);
output.accN = input.accN.*(g/1000);
output.accE = input.accE.*(g/1000);
output.accD = input.accD.*(g/1000);

end

