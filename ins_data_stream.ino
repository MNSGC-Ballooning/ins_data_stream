// Script to simultaneously pull and store IMU and GPS data into an onboard teensy SD card

// Libraries
#include <UbloxGPS.h>
#include "ICM_20948.h"
#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <SD.h>

// IMU Selection (uncomment the IMU in use)
//#define ICM20948
#define BNO055

// Heading Scheme Selection
#define INITIAL_HEADING_WITH_MAG
#define INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING


// Communications, Pins, and Loop Definitions
#define UBLOX_SERIAL Serial1
#define THERMISTOR A0
#define GPS_LOOP 500
#define IMU_LOOP 50
#define GPS_HEADING_LOOP 10000
#define GPS_PIN 29
#define SD_PIN 30

// Thresholds
#define GPS_HEADING_MIN_GROUND_VEL 200  // minimum GPS ground speed necessary to make a heading calculation (in ft/min)

// Constants
#define FT_PER_DEG_LAT 364537

// Timer
unsigned int imu_timer = 0;
unsigned int gps_timer = 0;
unsigned int gps_heading_timer = 0;

// GPS data struct
struct gps_data
{
  unsigned int millis;
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
  unsigned int sats;
  float lat;
  float lon;
  float alt;
  float velE;
  float velN;
  float velU;
  float velGround;
  float gpsHeading;
};
typedef struct gps_data gpsData;

#ifdef INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  float prevLat = 0;
  float prevLon = 0;
  float prevAlt = 0;
  unsigned int gpsDataCounter = 0;
  float t1 = 0;
  float t2 = 0;
#endif  // INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING


// ICM 20948 Data Structs
#ifdef ICM20948
  struct imu_data // Imu extra data struct
  {
    float magX;
    float magY;
    float magZ;
    float pitch;
    float roll;
    float heading;
  };
  typedef struct imu_data imuData;
  
  // Qwiic ICM-20948
  ICM_20948_I2C icm;
  unsigned int icm_millis;
  imuData icm_data;
#endif  // ICM20948


// BNO055 Data Structs
#ifdef BNO055
  struct imu_data
  {
    float accX;
    float accY;
    float accZ;
    float linaX;
    float linaY;
    float linaZ;
    float gravX;
    float gravY;
    float gravZ;
    float gyrX;
    float gyrY;
    float gyrZ;
    float magX;
    float magY;
    float magZ;
    float heading;
    float eulerX;
    float eulerY;
    float eulerZ;
    float quatW;
    float quatX;
    float quatY;
    float quatZ;
  };
  typedef struct imu_data imuData;
  
  // BNO055 variables
  Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
  imuData bno_data;
  unsigned int bno_millis;
  float initialHeading = 0;
  float headingOffset = 0;
#endif  // BNO055

// Thermistor variables
float thermTemp = 0;

// UBLOX variables
UbloxGPS ublox = UbloxGPS(&UBLOX_SERIAL);
gpsData ublox_data;

// SD
const int chipSelect = BUILTIN_SDCARD;
File dataLog;
char fileName[] = "ins00.csv";
bool sdActive = false;

// LEDs
bool gpsLEDOn = false;


void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("Serial Ports intialized...");

  analogReadResolution(13);

  pinMode(GPS_PIN, OUTPUT);   // initalize indicator LEDs
  pinMode(SD_PIN, OUTPUT);

  initSD();       // initialize SD card

  initUBLOX();    // initalize Ublox GPS

  // initialize IMU
  #ifdef ICM20948
  initICM();
  #endif  // ICM20948

  #ifdef BNO055
  initBNO();
  #endif  // BNO055
  
  Serial.println(buildHeaderString());  // print data header
}


void loop() {
  ublox.update();
  updateThermistor();

  if(millis() - gps_timer >  GPS_LOOP) {
    gps_timer = millis();

    updateUBLOX();

    gpsLEDCheck();
  }

  if(millis() - imu_timer >  IMU_LOOP) {
    imu_timer = millis();

    #ifdef ICM20948
    updateICM();
    #endif  // ICM20948

    #ifdef BNO055
    updateBNO();
    #endif  // BNO055
    
    logData();
  }

  #ifdef INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  if(millis() - gps_heading_timer > GPS_HEADING_LOOP) {
    gps_heading_timer = millis();
    gpsDataCounter++;
    t1 = ublox_data.millis/1000;
    float dt = (t1 - t2)/60;    // set dt in minutes so velocities can be in feet/min
    
    if(gpsDataCounter >= 2) {
      ublox_data.velE = getVelE(ublox_data.lat,prevLat,ublox_data.lon,prevLon,dt);
      ublox_data.velN = getVelN(ublox_data.lat,prevLat,dt);
      ublox_data.velU = getVelU(ublox_data.alt,prevAlt,dt);
      ublox_data.velGround = sqrt(ublox_data.velE*ublox_data.velE + ublox_data.velN*ublox_data.velN);
      ublox_data.gpsHeading = getGPSHeading(ublox_data.lat,prevLat,ublox_data.lon,prevLon);  // get the gps heading
      adjustHeadingOffset();                    // adjust the heading offset
    }
    
    prevLat = ublox_data.lat;
    prevLon = ublox_data.lon;
    prevAlt = ublox_data.alt;
    t2 = t1;
  }
  #endif  // INCREMENTAL_CORRECTIONS_WITH_GPS_HEADING
  
}
