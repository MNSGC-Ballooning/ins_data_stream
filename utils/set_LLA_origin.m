function output = set_LLA_origin(input)

% Function to find the first initial "origin coordinates" of a test run

lat_fidx = find(input.lat ~= 0, 1);
lon_fidx = find(input.lon ~= 0, 1);
alt_fidx = find(input.alt ~= 0, 1);

fidx = max([lat_fidx lon_fidx alt_fidx]);


output = input;

output.lat_origin = input.lat(fidx);
output.lon_origin = input.lon(fidx);
output.alt_origin = input.alt(fidx);
output.origin_idx = fidx;

end