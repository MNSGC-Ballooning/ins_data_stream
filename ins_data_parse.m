function output = ins_data_parse(filepath,imu,n)
% Function that parses data stored from ins_data_stream .csv files

% extract file data and prepare to save the output struct data
dataTable = readtable(filepath);        % read csv data into a MATLAB table

[path,name] = fileparts(filepath);      % extract file name information
matfilepath = fullfile(path,'matfiles');

if ~exist(matfilepath,'dir')            % verify that the matfile folder exists
    mkdir(matfilepath);                 % create it if it doesn't
end


% parse the data table values into a friendlier output struct
output.gpsMillis = dataTable.GPSMillis;
output.lat = dataTable.Latitude;
output.lon = dataTable.Longitude;
output.alt = dataTable.Altitude;
output.sats = dataTable.Satellites;
output.velE = dataTable.VelE;
output.velN = dataTable.VelN;
output.velU = dataTable.VelU;
output.velGround = dataTable.GroundVelocity;
output.imuMillis = dataTable.IMUMillis;
output.accX = dataTable.AccX;
output.accY = dataTable.AccY;
output.accZ = dataTable.AccZ;
output.gyrX = dataTable.GyrX;
output.gyrY = dataTable.GyrY;
output.gyrZ = dataTable.GyrZ;
output.magX = dataTable.MagX;
output.magY = dataTable.MagY;
output.magZ = dataTable.MagZ;
output.gpsHeading = dataTable.GPSHeading;

if  strcmp(imu,'ICM20948')
    output.pitch = dataTable.Pitch;
    output.roll = dataTable.Roll;
    output.heading = dataTable.Heading;
elseif strcmp(imu,'BNO055')
    output.linaX = dataTable.LinaX;
    output.linaY = dataTable.LinaY;
    output.linaZ = dataTable.LinaZ;
    output.gravX = dataTable.GravX;
    output.gravY = dataTable.GravY;
    output.gravZ = dataTable.GravZ;
    output.heading = dataTable.Heading;
    output.pitch = dataTable.Pitch;
    output.roll = dataTable.Roll;
    output.quatW = dataTable.QuatW;
    output.quatX = dataTable.QuatX;
    output.quatY = dataTable.QuatY;
    output.quatZ = dataTable.QuatZ;
    output.magHeading = dataTable.MagHeading;
    output.headingOffset = dataTable.HeadingOffset;
end

% find the magnitude of the acc, gyro, and mag vectors
output.accTot = vecnorm([output.accX output.accY output.accZ]')';
output.gyrTot = vecnorm([output.gyrX output.gyrY output.gyrZ]')';
output.magTot = vecnorm([output.magX output.magY output.magZ]')';

if strcmp(imu,'ICM20948')
    % acc calibration
    output.accX_offset = -12.0778;
    output.accY_offset = 1.3897;
    output.accZ_offset = 1007.6;

    output.accX_offset = 0;
    output.accY_offset = 0;
    output.accZ_offset = 1000;

    output.accX = output.accX - output.accX_offset;
    output.accY = output.accY - output.accY_offset;
    output.accZ = output.accZ - output.accZ_offset;

    output = remove_inertial_outliers(output);

    % Convert acceleration values to m/s/s
    output = accG_to_accMS(output);
    
end

% Find the NED acc components
output = imu_body_to_NED(output,imu);

% find time vectors in seconds
output.imuSeconds = output.imuMillis./1000;
output.gpsSeconds = output.gpsMillis./1000;

% Find local frame gps positions
output = LLA_to_local_frame(output);

% Integrate acc to get pos
% output = acc_to_pos_integration(output);

% Interpolate the GPS position values between new measurements
output = interpolate_gps(output);

% simulate a scenario where GPS data is lost
if n~= 1
    output = sim_data_without_gps(output,n);
end
    
% apply heading propogation algorithm
output = position_propogation_with_heading(output,n);

% save the output data as "insData", then clear it
insData = output;
save(fullfile(matfilepath,sprintf('%s.mat',sprintf('%s_%s',name,imu))),'insData');

clear insData

end