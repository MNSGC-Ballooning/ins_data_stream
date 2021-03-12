// tab containing SD functions

void initSD(){
  pinMode(chipSelect, OUTPUT);
  
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(chipSelect))                                                            //Attempt to start SD communication
    Serial.println("Card failed, not present, or voltage supply is too low.");          //Print out error if failed; remind user to check card
  else {                                                                                //If successful, attempt to create file
    Serial.println("Card initialized successfully.\nCreating File...");
    for (byte i = 0; i < 100; i++) {                                                    //Can create up to 100 files with similar names, but numbered differently
      fileName[3] = '0' + i / 10;
      fileName[4] = '0' + i % 10;
      if (!SD.exists(fileName)) {                                                       //If a given filename doesn't exist, it's available
        dataLog = SD.open(fileName, FILE_WRITE);                                        //Create file with that name
        sdActive = true;                                                                //Activate SD logging since file creation was successful
        Serial.println("Logging to: " + String(fileName));                              //Tell user which file contains the data for this run of the program
        break;                                                                          //Exit the for loop now that we have a file
      }
    }
    if (!sdActive) Serial.println("No available file names; clear SD card to enable logging");
  }
  
  if (sdActive) {
    dataLog.println(buildHeaderString());
    dataLog.close();
  }
}


void logData() { 
  String data = buildDataString();
  Serial.println(data);

  if(sdActive) {
    if(ublox_data.alt < 10000) {
      digitalWrite(SD_PIN, HIGH);   // indicate data is being logged to SD
    }
    dataLog = SD.open(fileName, FILE_WRITE);
    delay(10);
    dataLog.println(data);
    dataLog.close();
    digitalWrite(SD_PIN, LOW);
  }  
}

String buildDataString() {
  String dataString = String(ublox_data.millis) + ", " + String(ublox_data.lat,6) + ", " + String(ublox_data.lon,6) + ", " + String(ublox_data.alt,6) + ", " + String(ublox_data.sats) + ", ";

  #ifdef INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  dataString += String(ublox_data.velE) + ", " + String(ublox_data.velN) + ", " + String(ublox_data.velU) + ", " + String(ublox_data.velGround) + ", ";
  #endif  // INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  
  #ifdef ICM20948
  dataString += String(icm_millis) + ", " + String(icm.accX(), 4) + ", " + String(icm.accY(), 4) + ", " + String(icm.accZ(), 4) + ", ";
  dataString += String(icm.gyrX(), 4) + ", " + String(icm.gyrY(), 4) + ", " + String(icm.gyrZ(), 4) + ", ";
  dataString += String(icm_data.magX, 4) + ", " + String(icm_data.magY, 4) + ", " + String(icm_data.magZ, 4) + ", ";
  dataString += String(icm_data.pitch) + ", " + String(icm_data.roll) + ", " + String(icm_data.heading) + ", ";
  #endif  // IMC20948

  #ifdef BNO055
  dataString += String(bno_millis) + ", " + String(bno_data.accX, 4) + ", " + String(bno_data.accY, 4) + ", " + String(bno_data.accZ, 4) + ", ";
  dataString += String(bno_data.linaX, 4) + ", " + String(bno_data.linaY, 4) + ", " + String(bno_data.linaZ, 4) + ", ";
  dataString += String(bno_data.gravX, 4) + ", " + String(bno_data.gravY, 4) + ", " + String(bno_data.gravZ, 4) + ", ";
  dataString += String(bno_data.gyrX, 4) + ", " + String(bno_data.gyrY, 4) + ", " + String(bno_data.gyrZ, 4) + ", ";
  dataString += String(bno_data.magX, 4) + ", " + String(bno_data.magY, 4) + ", " + String(bno_data.magZ, 4) + ", " + String(bno_data.heading, 4) + ", ";
  #endif  // BNO055

  #ifdef INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  dataString += String(ublox_data.gpsHeading) + ", ";
  #endif // INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING

  #ifdef BNO055
  dataString += String(headingOffset) + ", " + String(bno_data.eulerX) + ", " + String(bno_data.eulerY) + ", " + String(bno_data.eulerZ) + ", ";
  dataString += String(bno_data.quatW) + ", " + String(bno_data.quatX) + ", " + String(bno_data.quatY) + ", " + String(bno_data.quatZ) + ", ";
  #endif  // BNO055

  dataString += String(thermTemp);
  return dataString;
}

String buildHeaderString() {
  String headerString = "GPS Millis, Latitude, Longitude, Altitude, Satellites, ";

  #ifdef INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  headerString += "VelE, VelN, VelU, Ground Velocity, ";
  #endif  // INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING

  #ifdef ICM20948
  headerString += "IMU Millis, AccX, AccY, AccZ, GyrX, GyrY, GyrZ, MagX, MagY, MagZ, ";
  headerString += "Pitch, Roll, Heading, ";
  #endif  // ICM20948

  #ifdef BNO055
  headerString += "IMU Millis, AccX, AccY, AccZ, LinaX, LinaY, LinaZ, GravX, GravY, GravZ, ";
  headerString += "GyrX, GyrY, GyrZ, MagX, MagY, MagZ, MagHeading, ";
  #endif  // BNO055

  #ifdef INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  headerString += "GPSHeading, ";
  #endif  // INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING

  #ifdef BNO055
  headerString += "Heading Offset, Heading, Pitch, Roll, QuatW, QuatX, QuatY, QuatZ, ";
  #endif  // BNO055

  headerString += "ThermTemp";

  return headerString;
}
