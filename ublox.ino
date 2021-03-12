// UBLOX GPS functions

void initUBLOX() {
  UBLOX_SERIAL.begin(UBLOX_BAUD);
  delay(50);
  ublox.init();
  Serial.println("UBLOX initialized...");

  byte i = 0;
  while (i<100) {
    i++;
    if (ublox.setAirborne()) {
      Serial.println("Air mode successfully set.");
      // communicate successful airborn mode setting
      for(byte j = 0; j < 3; j++) {
        digitalWrite(GPS_PIN, HIGH);
        delay(1000);
        digitalWrite(GPS_PIN, LOW);
        delay(1000);
      }
      break;
    }
    if (i==50){
      Serial.println("Failed to set to air mode.");
      delay(5000);
    }
  }
}

void updateUBLOX() {
  // Update GPS data struct
  ublox_data.millis = millis();
  ublox_data.year = ublox.getYear();
  ublox_data.month = ublox.getMonth();
  ublox_data.day = ublox.getDay();
  ublox_data.hour = ublox.getHour();
  ublox_data.min = ublox.getMinute();
  ublox_data.sec = ublox.getSecond();
  ublox_data.sats = ublox.getSats();
  ublox_data.lat = ublox.getLat();
  ublox_data.lon = ublox.getLon();
  ublox_data.alt = ublox.getAlt_feet(); 

}

void gpsLEDCheck() {
  if (!gpsLEDOn && ublox_data.sats >= 4 && ublox_data.lat != 0 && ublox_data.alt < 10000) {
    digitalWrite(GPS_PIN, HIGH);
    gpsLEDOn = true;
  }
  else if((ublox_data.sats < 4 || ublox_data.alt > 10000) && gpsLEDOn) {
    digitalWrite(GPS_PIN, LOW);
    gpsLEDOn = false;
  }
}

// GPS velocity calculations (return veloicty in ft/min)
float getVelE(float lat1, float lat2, float lon1, float lon2, float dt) {
  float d2r = PI/180;
  float avgLat = (lat1 + lat2)/2;
  float ft_per_deg_lon = FT_PER_DEG_LAT*cos(avgLat*d2r);
  float velE = (lon1 - lon2)*ft_per_deg_lon;
  velE /= dt;

  return velE;
}

float getVelN(float lat1, float lat2, float dt) {
  float d2r = PI/180;
  float velN = (lat1 - lat2)*FT_PER_DEG_LAT;
  velN /= dt;

  return velN;
}

float getVelU(float alt1, float alt2, float dt) {
  float velU = (alt1 - alt2)/dt;

  return velU;
}
