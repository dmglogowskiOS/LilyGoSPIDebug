#ifndef RTDH
#define RTDH
#include <Arduino.h>
#include <MCP23S17.h>
#include <SPI.h>

class RTD{
    public:
        RTD(u_int8_t pinSet, MCP23S17* mcp);
        void rtdInit();
    private:
        uint8_t pin;
        uint8_t config;
        MCP23S17* mcp;
        SPIClass* spi;
};

#endif