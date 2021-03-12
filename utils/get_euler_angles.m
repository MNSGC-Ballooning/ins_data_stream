function output = get_euler_angles(input)
% function that draws the roll, pitch, and yaw euler angles from IMU angles

r2d = 180/pi;                               % radians to degrees

output = input;

% find the euler angles
output.roll = atan(input.accY./sqrt(input.accX.^2 + input.accZ.^2)).*r2d;
output.pitch = atan(-input.accX./input.accZ).*r2d;
output.heading = 90 - atan(input.magX./input.magY).*r2d;                             % will need to be resolved

end