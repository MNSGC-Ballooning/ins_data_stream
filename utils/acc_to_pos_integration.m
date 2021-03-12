function output = acc_to_pos_integration(input)

output = input;

% thresholding?
threshold = 0;

output.accN(abs(input.accN) < threshold) = 0;
output.accE(abs(input.accE) < threshold) = 0;
output.accD(abs(input.accD) < threshold) = 0;

% integrate to get velocity
output.velN_int = cumtrapz(input.imuSeconds,input.accN);
output.velE_int = cumtrapz(input.imuSeconds,input.accE);
output.velD_int = cumtrapz(input.imuSeconds,input.accD);

% integrate to get position
output.posN_int = cumtrapz(input.imuSeconds,output.velN_int);
output.posE_int = cumtrapz(input.imuSeconds,output.velE_int);
output.posD_int = cumtrapz(input.imuSeconds,output.velD_int);

end
