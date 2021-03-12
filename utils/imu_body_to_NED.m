function output = imu_body_to_NED(input,imu)

% Function that transforms body-fixed acceleration components into
% North-East_Down acceleration components

output = input;

n = length(input.accX);
fidx = find(input.accX > 0,1);

R = zeros(n,3,3);
input.accN = zeros(n,1);
input.accE = zeros(n,1);
input.accD = zeros(n,1);

if strcmp(imu,'ICM20948')
    accX = input.accX;
    accY = input.accY;
    accZ= input.accZ;
elseif strcmp(imu,'BNO055')
    accX = input.linaX;
    accY = input.linaY;
    accZ = input.linaZ;
end

for i = fidx:n
    p = input.pitch(i);
    r = input.roll(i);
    h = input.heading(i);
    
    Rx = [1 0 0; 0 cosd(r) sind(r); 0 -sind(r) cosd(r)];   
    Ry = [-sind(p) 0 cosd(p); 0 1 0; cosd(p) 0 sind(p)];
    Rz = [cosd(h) sind(h) 0; -sind(h) cosd(h) 0; 0 0 1];
    
    R(i,:,:) = (Rx*Ry*Rz)';
    
%     R(i,1,1) = cosd(h)*cosd(p);
%     R(i,1,2) = -sind(h)*cosd(r) + cosd(h)*sind(p)*sind(r);
%     R(i,1,3) = sind(h)*sind(r) + cosd(h)*cosd(r)*sind(p);
%     R(i,2,1) = sind(h)*cosd(p);
%     R(i,2,2) = cosd(h)*cosd(r) + sind(r)*sind(p)*sind(h);
%     R(i,2,3) = -cosd(h)*sind(r) + sind(p)*sind(h)*cosd(r);
%     R(i,3,1) = -sind(p);
%     R(i,3,2) = cosd(p)*sind(r);
%     R(i,3,3) = cosd(p)*cosd(r);
    
    output.accD(i) = squeeze(R(i,1,:))'*[accX(i); accY(i); accZ(i)];
    output.accE(i) = squeeze(R(i,2,:))'*[accX(i); accY(i); accZ(i)];
    output.accN(i) = squeeze(R(i,3,:))'*[accX(i); accY(i); accZ(i)];
    
    output.gyrD(i) = squeeze(R(i,1,:))'*[input.gyrX(i); input.gyrY(i); input.gyrZ(i)];
    output.gyrE(i) = squeeze(R(i,2,:))'*[input.gyrX(i); input.gyrY(i); input.gyrZ(i)];
    output.gyrN(i) = squeeze(R(i,3,:))'*[input.gyrX(i); input.gyrY(i); input.gyrZ(i)];
    
end

% remove nan indices
nans = find(isnan(output.accN) == 1);

output.accN = rmmissing(output.accN)';
output.accE = rmmissing(output.accE)';
output.accD = rmmissing(output.accD)';
output.gyrN = rmmissing(output.gyrN)';
output.gyrE = rmmissing(output.gyrE)';
output.gyrD = rmmissing(output.gyrD)';

if length(nans) > 0
    output.imuMillis(nans) = [];
    output.imuSeconds(nans) = [];
    output.gpsMillis(nans) = [];
    output.gpsSeconds(nans) = [];
    output.lat(nans) = [];
    output.lon(nans) = [];
    output.alt(nans) = [];
    output.sats(nans) = [];
    output.pitch(nans) = [];
    output.roll(nans) = [];
    output.heading(nans) = [];
    output.accTot(nans) = [];
    output.gyrTot(nans) = [];
    output.magTot(nans) = [];
end


end
