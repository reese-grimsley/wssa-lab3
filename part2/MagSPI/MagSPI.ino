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

  delay(1000);
  
  uint8_t whoami = spi_read_cmd(FXOS8700CQ_WHO_AM_I);
  SerialUSB.println(whoami, HEX);

  // Initialize sensor
  sensor = FXOS8700CQ();
  sensor.init();

}

void loop() {
//Fill code here to read from chip
}
