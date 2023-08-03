#include <Arduino.h>
#include "rtdManager.h"
#include "portexpander.h"
#include "rtd.h"

constexpr int clockPin = 14;
constexpr int misoPin = 2;
constexpr int mosiPin = 15;
constexpr int chipSelect = 4;
constexpr int address = 0;

unsigned long start;

constexpr float nominalResistanceRTD = 1000.0f;
constexpr float referenceResistanceRTD = 4300.0f;

// The easy Way

SPIClass hspi(2);
PortExpander portExpander(&hspi, clockPin, misoPin, mosiPin, chipSelect, address);
RtdManager rtdManager(&portExpander);

RTD rtd1(0, portExpander, PortExpanderBank::B, RTDWireMode::TWO_WIRE, nominalResistanceRTD, referenceResistanceRTD);
RTD rtd2(1, portExpander, PortExpanderBank::B, RTDWireMode::TWO_WIRE, nominalResistanceRTD, referenceResistanceRTD);
RTD rtd3(2, portExpander, PortExpanderBank::B, RTDWireMode::TWO_WIRE, nominalResistanceRTD, referenceResistanceRTD);
RTD rtd4(3, portExpander, PortExpanderBank::B, RTDWireMode::TWO_WIRE, nominalResistanceRTD, referenceResistanceRTD);

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning SPI Test");
  hspi.setDataMode(SPI_MODE3);

  rtdManager.init();
  
  rtdManager.addRTD(rtd1);
  rtdManager.addRTD(rtd2);
  rtdManager.addRTD(rtd3);
  rtdManager.addRTD(rtd4);

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



