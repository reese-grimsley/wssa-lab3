#include <SPI.h>
#include "FXOS8700CQ.h"

FXOS8700CQ sensor;

void setup() {
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  // Initialize SerialUSB 
  SerialUSB.begin(9600);
  while(!SerialUSB);
  SerialUSB.println("Serial configured; setup SPI");
  
  // Initialize SPI
  SPI.begin();

  // Initialize sensor
  sensor = FXOS8700CQ();
  sensor.init();
  delay(10); //delay to initialize fully
  
  // check WhoAmI
  sensor.checkWhoAmI();

}

void loop() {
//Fill code here to read from chip
//  sensor.readMagData();
//
//  SerialUSB.print("X value = "); 
//  SerialUSB.println(sensor.magData.x);
//  
//  SerialUSB.print("Y value = "); 
//  SerialUSB.println(sensor.magData.y);
//  
//  SerialUSB.print("Z value = "); 
//  SerialUSB.println(sensor.magData.z);
//
//  SerialUSB.println("");
  uint8_t who;
  SerialUSB.println("Start reads");
  digitalWrite(6, HIGH);
  for (int i = 0; i < 10000; i++) {
      
      sensor.readReg(FXOS8700CQ_WHO_AM_I);

  }
  digitalWrite(6, LOW);
  SerialUSB.println("End Reads");
  delay(1000);

}
