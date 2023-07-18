#include <Arduino.h>
#include "portexpander.h"

constexpr int clockPin = 14;
constexpr int misoPin = 2;
constexpr int mosiPin = 15;
constexpr int chipSelect = 4;
constexpr int address = 0;

constexpr uint8_t pin = 7+1; // This is GPIO Bank B Pin 0,
unsigned long start;

// The easy Way

SPIClass hspi(2);
PortExpander portExpander(&hspi, clockPin, misoPin, mosiPin, chipSelect, address);

void setup() {
  
  Serial.begin(115200);
  Serial.println("Beginning SPI Test");
  
  portExpander.initPortExpander();
  portExpander.setBank(PortExpanderBank::B, IOType::OUTPUT_IO);
  
  for (uint8_t i = 0; i < 8; i++)
  {
    portExpander.writePort(PortExpanderBank::B, i, HIGH);
  }
  

  start = millis();
}

void loop() {
  auto static currentPort = 0;
  
  if (millis() - start < 50)
  {
    portExpander.writePort(PortExpanderBank::B, currentPort, LOW);
  } else {
    portExpander.writePort(PortExpanderBank::B, currentPort, HIGH);
    currentPort++;
    start = millis();
  }
  if (currentPort > 7)
  {
    currentPort = 0;
  }
  
}

