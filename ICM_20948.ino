// ICM-20948 Functions
#ifdef ICM20948

#define X_SCALE 1
#define Y_SCALE 1
#define Z_SCALE 1
#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0

void initICM() {
  // I2C initialization stuff
  Wire.begin();
  Wire.setClock(400000);

  // ICM IMU initialization
  bool initialized = false;
  while(!initialized) {
    icm.begin(Wire,1);  // input values are (Wire Port, AD0 Value)

    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(icm.statusString());
    if(icm.status != ICM_20948_Stat_Ok){
      Serial.println( "Trying again..." );
      delay(500);
    }else{
      initialized = true;
    }
  }
  
  Serial.println("Initialized ICM...");
}

void updateICM() {
  if(icm.dataReady()) {
    icm_millis = millis();
    icm.getAGMT();
  }

  icm_data.magX = correctMag(mag.x(),'x');
  icm_data.magY = correctMag(mag.y(),'y');
  icm_data.magZ = correctMag(mag.z(),'z');

  icm_data.pitch = getPitch(icm.accX(),icm.accY(),icm.accZ());
  icm_data.roll = getRoll(icm.accX(),icm.accY(),icm.accZ());
  icm_data.heading = getHeading(icm_data.magX,icm_data.magY,icm_data.magZ,icm_data.pitch,icm_data.roll);
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

#endif  // ICM20948
