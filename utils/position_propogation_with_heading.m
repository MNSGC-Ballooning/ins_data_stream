function output = position_propogation_with_heading(input,n)

idx = find(diff(input.gpsHeading) ~= 0)+1;

nidx = 1:n:length(idx);

idx = idx(nidx);

num = length(input.posE);

output = input;

output.posE_prop = zeros(num,1);
output.posN_prop = zeros(num,1);
output.posU_prop = zeros(num,1);

output.posE_prop([1:idx(1) idx']) = output.posE([1:idx(1) idx']);
output.posN_prop([1:idx(1) idx']) = output.posN([1:idx(1) idx']);
output.posU_prop([1:idx(1) idx']) = output.posU([1:idx(1) idx']);


for i = 1:length(idx)-1
    for j = idx(i):idx(i+1)-2
        dt = output.imuSeconds(j+1) - output.imuSeconds(j);
        output.posE_prop(j+1) = output.posE_prop(j) + (output.velGround(j)/60)*cosd(output.heading(j))*dt;
        output.posN_prop(j+1) = output.posN_prop(j) + (output.velGround(j)/60)*sind(output.heading(j))*dt;
    end
    for j = idx(i+1):num
        output.posE_prop(j) = output.posE(j);
        output.posN_prop(j) = output.posN(j);
    end
end

output.dE = output.posE_prop - output.posE;
output.dN = output.posN_prop - output.posN;
output.dx = (output.dE.^2 + output.dN.^2).^(1/2);
