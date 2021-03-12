function output = sim_data_without_gps(input,n)

output = input;

idx = find(diff(input.gpsHeading) ~= 0)+1;

nidx = 1:n:length(idx);

idx = idx(nidx);

for i = 1:length(idx)-1
    output.velGround(idx(i):idx(i+1)-1) = output.velGround(idx(i));
    output.heading(idx(i):idx(i+1)-1) = output.heading(idx(i):idx(i+1)-1)-(output.headingOffset(idx(i):idx(i+1)-1)-output.headingOffset(idx(i)));      
end

end