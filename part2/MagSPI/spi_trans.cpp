// Reading and Writing the SPI bus

//------------------------------------------------------------------------------
// Headers
//------------------------------------------------------------------------------
#include "spi_trans.h"

//******************************************************************************
// Function Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// spi_write_cmd(): Transmit to a SPI device
//------------------------------------------------------------------------------
void spi_write_cmd(uint8_t address, uint8_t tx_data) {
#ifdef SPI_DEBUG
  SerialUSB.println("Write over SPI");
#endif 
  digitalWrite(CS_PIN, LOW); //chip select low active
  delay(50); //delay to let device initialize
  SPI.beginTransaction(SPISettings(SPI_HZ, SPI_ORDER, SPI_MODE)); //configure parameters

#ifdef SPI_DEBUG
  SerialUSB.println(SPI.transfer(0xC0 | (0x7f & address))); //RW bit and lower 7 bits of address
  SerialUSB.println(SPI.transfer(0xC0 & address)); //highest bit of addresss and 7 zeros (don't care)
  SerialUSB.println(SPI.transfer(tx_data)); // transfer the data itself 
#else 
  SPI.transfer(0xC0 | (0x7f & address)); //RW bit and lower 7 bits of address
  SPI.transfer(0xC0 & address); //highest bit of addresss and 7 zeros (don't care)
  SPI.transfer(tx_data); // transfer the data itself 
#endif 

  digitalWrite(CS_PIN, HIGH); //release chip select
  SPI.endTransaction();

#ifdef SPI_DEBUG
  SerialUSB.println("Finish write over SPI");
#endif 
}

//------------------------------------------------------------------------------
// spi_read_cmd(): Read from a SPI device. Return the data read from register
//------------------------------------------------------------------------------
uint8_t spi_read_cmd(uint8_t address) {
#ifdef SPI_DEBUG
  SerialUSB.println("Read over SPI");
#endif 

  uint8_t rx_data;

  digitalWrite(CS_PIN, LOW); //chip select low active
  delay(50);
  SPI.beginTransaction(SPISettings(SPI_HZ, SPI_ORDER, SPI_MODE)); //configure parameters

#ifdef SPI_DEBUG //print what we get back at end step
  SerialUSB.println(SPI.transfer(0x7f & address)); //RW bit and lower 7 bits of address
  SerialUSB.println(SPI.transfer(0xC0 & address)); //highest bit of addresss and 7 zeros (don't care)
  rx_data = SPI.transfer(0);  //receive data; need to keep sending clock pulses
  SerialUSB.println(rx_data); 
#else 
  SPI.transfer(0xC0 | (0x7f & address)); //RW bit and lower 7 bits of address
  SPI.transfer(0xC0 & address); //highest bit of addresss and 7 zeros (don't care)
  rx_data = SPI.transfer(0); // receive data; need to keep sending clock pulses
#endif 

  digitalWrite(CS_PIN, HIGH); //release chip select
  SPI.endTransaction();

#ifdef SPI_DEBUG
  SerialUSB.println("Finish read over SPI");
#endif 

  return rx_data;

}

//*****************************************************************************
