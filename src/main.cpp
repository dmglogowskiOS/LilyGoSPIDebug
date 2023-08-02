#include <Arduino.h>
#include "rtdManager.h"
#include "portexpander.h"
#include "rtd.h"

constexpr int clockPin = 14;
constexpr int misoPin = 2;
constexpr int mosiPin = 15;
constexpr int chipSelect = 4;
constexpr int address = 0;

constexpr uint8_t pin = 7+1; // This is GPIO Bank B Pin 0,
unsigned long start;

constexpr float nominalResistanceRTD = 1000.0f;
constexpr float referenceResistanceRTD = 4300.0f;

// The easy Way

SPIClass hspi(2);
PortExpander portExpander(&hspi, clockPin, misoPin, mosiPin, chipSelect, address);
RtdManager rtdManager(portExpander);

RTD rtd1(0, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd2(1, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd3(2, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);
RTD rtd4(3, portExpander.getMcp(), GPIOBank::B, RTDWireMode::TWO_WIRE);

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning SPI Test");
  hspi.setDataMode(SPI_MODE3);
  
  portExpander.initPortExpander();
  portExpander.setBank(PortExpanderBank::B, IOType::OUTPUT_IO);
  
  for (uint8_t i = 0; i < 8; i++)
  {
    portExpander.writePort(PortExpanderBank::B, i, HIGH);
  }

  rtdManager.addRTD(rtd1);
  rtdManager.addRTD(rtd2);
  rtdManager.addRTD(rtd3);
  rtdManager.addRTD(rtd4);
  
  rtdManager.setResistances(nominalResistanceRTD, referenceResistanceRTD);
  rtdManager.init();

  pinMode(misoPin, INPUT_PULLUP);
  start = millis();

}

void loop() {
  if (millis() - start > 1000)
  {
    std::vector<float> temps = rtdManager.readAll();

    for (size_t i = 0; i < temps.size(); i++)
    {
      Serial.printf("RTD Nr %i : %f \n", i, temps[i]);
    }
    
    Serial.println("\n");
    start = millis();
  }
}



