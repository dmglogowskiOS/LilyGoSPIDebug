#include <Arduino.h>
#include <MCP23S17.h>
#include <vector>

constexpr int clockPin = 14;
constexpr int misoPin = 2;
constexpr int mosiPin = 15;
constexpr int chipSelect = 4;

constexpr uint8_t pin = 0;

SPIClass hspi(2);
MCP23S17 mcp(&hspi, 4, 0);
constexpr byte dataOn = 0xFF; 
constexpr byte dataOff = 0x00;
constexpr uint16_t bigDataOn = 0xFFFF;
constexpr uint16_t bigDataOff = 0x0000;

void setup() {
  
  Serial.begin(115200);
  Serial.println("Beginning SPI Test");
  // Setting the Pins
  mcp.begin(clockPin, misoPin, mosiPin, chipSelect);
  mcp.pinMode(pin,OUTPUT);
  mcp.writePort(bigDataOn);

}

void loop() {
  mcp.digitalWrite(pin, HIGH);
  delay(150);
  mcp.writePort(pin, LOW);
  delay(150);
}


