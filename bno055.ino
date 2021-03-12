// BNO055 IMU functions
#ifdef BNO055

#define X_SCALE 0.9581
#define Y_SCALE 1.1085
#define Z_SCALE 0.9487
#define X_OFFSET 0.0938
#define Y_OFFSET 1.6250
#define Z_OFFSET 0.8125

// initialization function
void initBNO() {
  if(!bno.begin())
  {
    Serial.println("No BNO055 detected...check wiring or the I2C address.");
    while(1);
  }

  delay(50);

  Serial.println("BNO055 initialized...");
  Serial.println("Finding initial heading. Slowly shake the BNO for a couple of seconds...");

  float magX = 0;
  float magY = 0;
  float magZ = 0;
  float pitch = 0;
  float roll = 0;

  #ifdef INITIAL_HEADING_WITH_MAG
    // 15 seconds to initialize the bno, 5 seconds to actually get data
    digitalWrite(SD_PIN, HIGH);
    for(int i = 0; i < 2000; i++) {
      imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
      delay(10);
      if(i>=1500) {
        digitalWrite(SD_PIN, LOW);
        magX += correctMag(mag.x(),'x');
        magY += correctMag(mag.y(),'y');
        magZ += correctMag(mag.z(),'z');
      }
    }
  
    magX /= 500; magY /= 500; magZ /= 500;
  
    initialHeading = getHeading(magX,magY,magZ,0,0);
    headingOffset = initialHeading;
  
    Serial.print("Intial heading is: ");
    Serial.print(initialHeading);
    Serial.println(" deg.");
  #endif  // INITIAL_HEADING_WITH_MAG
  
}

// data update function
void updateBNO() {
  bno_millis = millis();
  
  // pull data and store it into the RAM
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> grav = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  imu::Vector<3> lina = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  imu::Vector<3> gyr = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Quaternion quat = bno.getQuat();

  // store acceleration values in data struct
  bno_data.accX = acc.x();    // pure acceleration
  bno_data.accY = acc.y();
  bno_data.accZ = acc.z();
  bno_data.gravX = grav.x();  // gravity vector
  bno_data.gravY = grav.y();
  bno_data.gravZ = grav.z();
  bno_data.linaX = lina.x();  // linear acceleration vector (without gravity)
  bno_data.linaY = lina.y();
  bno_data.linaZ = lina.z();

  // store gyroscope values in data struct
  bno_data.gyrX = gyr.x();
  bno_data.gyrY = gyr.y();
  bno_data.gyrZ = gyr.z();

  // store magnetometer values in data struct
  bno_data.magX = correctMag(mag.x(),'x');
  bno_data.magY = correctMag(mag.y(),'y');
  bno_data.magZ = correctMag(mag.z(),'z');

  bno_data.heading = boundHeading(getHeading(bno_data.magX,bno_data.magY,bno_data.magZ,euler.y(),euler.z()));

  // store euler angle values in data struct
  bno_data.eulerX = boundHeading(-1*euler.x()+headingOffset);
  bno_data.eulerY = euler.y();
  bno_data.eulerZ = euler.z();

  // store quaternion values in data struct
  bno_data.quatW = quat.w();
  bno_data.quatX = quat.x();
  bno_data.quatY = quat.y();
  bno_data.quatZ = quat.z();
  
}

float correctMag(float rawMag, char c) {
  float correctedMag = 0;
  
  switch(c){
    case 'x': correctedMag = X_SCALE*(rawMag - X_OFFSET);
    case 'y': correctedMag = Y_SCALE*(rawMag - Y_OFFSET);
    case 'z': correctedMag = Z_SCALE*(rawMag - Z_OFFSET);
  }

  return correctedMag;
}

#endif  // BNO055
