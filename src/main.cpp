#include <Arduino.h>
#include "portexpander.h"
#include "rtd.h"

constexpr int clockPin = 14;
constexpr int misoPin = 2;
constexpr int mosiPin = 15;
constexpr int chipSelect = 4;
constexpr int address = 0;

constexpr uint8_t pin = 7+1; // This is GPIO Bank B Pin 0,
unsigned long start;
uint32_t numReads = 0U;

constexpr float nominalResistanceRTD = 1000.0f;
constexpr float referenceResistanceRTD = 4300.0f;

// The easy Way

SPIClass hspi(2);
PortExpander portExpander(&hspi, clockPin, misoPin, mosiPin, chipSelect, address);

RTD rtd1(0, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd2(1, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd3(2, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd4(3, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd0(4, portExpander.getMcp(), GPIOBank::NONE, RTDWireMode::TWO_WIRE);

void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial.begin(115200);
  Serial.println("Beginning SPI Test");
  
  portExpander.initPortExpander();
  portExpander.setBank(PortExpanderBank::B, IOType::OUTPUT_IO);
  
  for (uint8_t i = 0; i < 8; i++)
  {
    portExpander.writePort(PortExpanderBank::B, i, HIGH);
  }

  /*
  rtd1.rtdInit();
  rtd2.rtdInit();
  rtd3.rtdInit();
  rtd4.rtdInit();
  */

  rtd0.rtdInit();
  start = millis();

}

void loop() {
  if (millis() - start > 500)
  {
    Serial.printf("Read Number : %i \n", numReads);
    Serial.println(rtd0.readTempC(nominalResistanceRTD, referenceResistanceRTD));
    Serial.println(rtd0.readFault());
    Serial.println("\n");
    numReads++;
    start = millis();
  }
}

