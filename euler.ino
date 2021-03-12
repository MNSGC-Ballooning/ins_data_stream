float getPitch(float accX, float accY, float accZ) {

  float pitch = 0;
  float r2d = 180/PI;

  // pitch caclulations
  pitch = atan(accX/sqrt(accY*accY + accZ*accZ))*r2d;

  return pitch;
}

float getRoll(float accX, float accY, float accZ) {

  float roll = 0;
  float r2d = 180/PI;

  // roll calculations
  roll = atan(accY/accZ)*r2d;

  return roll;
}

float getHeading(float magX, float magY, float magZ, float pitch, float roll) {
  
  float heading = 0;
  float r2d = 180/PI;
  
//  float cos_roll = cos(roll/r2d);
//  float sin_roll = sin(roll/r2d);
//  float cos_pitch = cos(pitch/r2d);
//  float sin_pitch = sin(pitch/r2d);
//
//  float magX2 = magX*cos_pitch + magY*sin_roll*sin_pitch + magZ*cos_roll*sin_pitch;
//  float magY2 = magY*cos_roll + magZ*sin_roll;

  // heading calculations
//  heading = atan2(magY2,magX2)*r2d;       // tilt compansated heading
  heading = atan2(magY, magX)*r2d;        // non-tilt compensated heading
  heading = -1*(heading-90);

  return heading;
}

float boundHeading(float rawHeading){
  float heading = 0;
  if(rawHeading <= -180) {
    heading =  rawHeading + 360;
  }
  else if (rawHeading > 180) {
    heading = rawHeading - 360;
  }
  else {
    heading = rawHeading;
  }
  
  return heading;
}

float getGPSHeading(float lat1, float lat2, float lon1, float lon2) {
  float heading = 0;
  if(ublox_data.sats >= 4 && lat1 != 0 && lat2 != 0 && ublox_data.velGround >= GPS_HEADING_MIN_GROUND_VEL) {
    float r2d = 180/PI;
    float d2r = PI/180;
    float deltaLon = (lon1 - lon2)*d2r;
    float x = cos(lat1*d2r)*sin(deltaLon);
    float y = cos(lat2*d2r)*sin(lat1*d2r) - sin(lat2*d2r)*cos(lat1*d2r)*cos(deltaLon);
    heading = atan2(y,x)*r2d;
    heading = boundHeading(heading);
  }

  return heading;
}

// Function to adjust the heading offset to align the BNO heading with the GPS heading
void adjustHeadingOffset(void) {
  if(ublox_data.gpsHeading != 0) {
    float deltaHeading = ublox_data.gpsHeading - bno_data.eulerX;
    headingOffset = boundHeading(headingOffset + deltaHeading); 
  }
}
