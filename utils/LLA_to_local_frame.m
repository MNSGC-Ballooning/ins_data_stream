function output = LLA_to_local_frame(input)

% Function that returns local coordinate frame values instead of LLA values

output = set_LLA_origin(input);

n = length(output.alt);

output.posE = zeros(output.origin_idx-1,1);
output.posN = zeros(output.origin_idx-1,1);
output.posU = zeros(output.origin_idx-1,1);

for i = output.origin_idx:n
    output.posE(i) = (output.lon(i) - output.lon_origin)*364537*cosd(output.lat(i));
    output.posN(i) = (output.lat(i) - output.lat_origin)*364537;
    output.posU(i) = output.alt(i) - output.alt_origin;
end

end
