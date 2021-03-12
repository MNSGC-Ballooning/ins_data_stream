function output = remove_inertial_outliers(input)

% Remove inertial data outliers

output = input;

output.accX(isoutlier(output.accX)) = 0; 
output.accY(isoutlier(output.accY)) = 0; 
output.accZ(isoutlier(output.accZ)) = 0; 
output.gyrX(isoutlier(output.gyrX)) = 0; 
output.gyrY(isoutlier(output.gyrY)) = 0; 
output.gyrZ(isoutlier(output.gyrZ)) = 0; 
output.magX(isoutlier(output.magX)) = 0; 
output.magY(isoutlier(output.magY)) = 0; 
output.magZ(isoutlier(output.magZ)) = 0; 

end