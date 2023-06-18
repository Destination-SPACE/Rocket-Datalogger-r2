#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_LSM6DS3TRC.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define SD_CS 3
#define BUZZER 2
#define LED 13

float ALT_GND;
int BZR_STS = LOW;

Adafruit_BMP3XX bmp;
Adafruit_LSM6DS3TRC LSM;
File myFile;

void setup() {
  
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  for(int i=0;i<5;i++) {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(50);
  }
  Serial.begin(115200);
  Wire.begin();
  if(!bmp.begin_I2C()){
    while(true){
      for(int i=0;i<2;i++) {
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        digitalWrite(BUZZER, LOW);
        delay(50);
      }
      delay(200);
    }
  }
  if(!LSM.begin_I2C()){
    while(true) {
      for(int i=0;i<3;i++) {
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        digitalWrite(BUZZER, LOW);
        delay(50);
      }
      delay(200);
    }
  }
  if(!SD.begin(SD_CS)){
    while(true) {
      for(int i=0;i<4;i++) {
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        digitalWrite(BUZZER, LOW);
        delay(50);
      }
      delay(200);
    }
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  LSM.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
  LSM.setGyroRange(LSM6DS_GYRO_RANGE_500_DPS);
  LSM.setAccelDataRate(LSM6DS_RATE_208_HZ);
  LSM.setGyroDataRate(LSM6DS_RATE_208_HZ);

  char buffer[64];
  sprintf(buffer, "time, temp, pres, accelX, accelY, accelZ, gyroX, gyroY, gyroZ");
  myFile = SD.open("rktlog.csv", FILE_WRITE);
  if(myFile){
    myFile.println(buffer);
    myFile.close();
  }

  bmp.performReading();
  ALT_GND = bmp.readAltitude(SEALEVELPRESSURE_HPA);

}

void loop() {
  sensors_event_t ACCEL;
  sensors_event_t GYRO;
  sensors_event_t TEMP;
  LSM.getEvent(&ACCEL, &GYRO, &TEMP);
  unsigned long previousMillis;
  float BMP_TEMP;
  float PRES;
  float ALT;

  float MAX_ALT = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  unsigned long START_TIME = millis();

  if(abs(GYRO.gyro.x) < 1 && abs(GYRO.gyro.y) < 1 && abs(GYRO.gyro.z) < 1 && ACCEL.acceleration.x < -9.2){
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(1000);
  }

  if(ACCEL.acceleration.x < -12){
    while(true){
      unsigned long TIME = millis() - START_TIME;
      
      if ((TIME - previousMillis) >= 750) {
        previousMillis = TIME;
        if (BZR_STS == LOW) {
        BZR_STS = HIGH;
        } 
        else {
          BZR_STS = LOW;
        }
        digitalWrite(BUZZER, BZR_STS);
      }

      bmp.performReading();
      BMP_TEMP = bmp.temperature;
      PRES = bmp.pressure / 100.00;
      ALT = bmp.readAltitude(SEALEVELPRESSURE_HPA);

      if(ALT > MAX_ALT) MAX_ALT = ALT;

      if(TIME > 30000){
        for(int i=0;i<3;i++) {
          digitalWrite(BUZZER, HIGH);
          delay(1000);
          digitalWrite(BUZZER, LOW);
          delay(100);
        }
        int maxAltFinal = MAX_ALT - ALT_GND;
        int maxAlt_xx1 = maxAltFinal % 10;
        int maxAlt_x1x = (maxAltFinal / 10) % 10;
        int maxAlt_1xx = (maxAltFinal / 100) % 10;
        Serial.println(maxAlt_1xx);
        Serial.println(maxAlt_x1x);
        Serial.println(maxAlt_xx1);
        while(true){
          for(int i=0;i<5;i++) {
            for(int j=0;j<180;j++) {
              float sinVal = (sin(j*(3.1412/180)));
              int toneVal = 2000+(int(sinVal*1000));
              tone(BUZZER, toneVal);
              delay(2);
            }
          }
          noTone(BUZZER);
          delay(1000);
          if(maxAlt_1xx = 0){
            for(int i=0;i<10;i++){
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(200);
            }
          }
          for(int i=0;i<maxAlt_1xx;i++){
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(200);
          }
          delay(1000);

          if(maxAlt_x1x = 0){
            for(int i=0;i<10;i++){
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(200);
            }
          }
          for(int i=0;i<maxAlt_x1x;i++){
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(200);
          }
          delay(1000);

          if(maxAlt_xx1 = 0){
            for(int i=0;i<10;i++){
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(200);
            }
          }
          for(int i=0;i<maxAlt_xx1;i++){
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(200);
          }
          delay(4000);
        }
      }
    }
    char buffer[32];
    sprintf(buffer, "%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f", BMP_TEMP, PRES, ALT, ACCEL.acceleration.x, ACCEL.acceleration.y, ACCEL.acceleration.z);

    myFile = SD.open("rktlog.csv", FILE_WRITE);
    if(myFile){
      myFile.println(buffer);
      myFile.close();
    }
  }
}