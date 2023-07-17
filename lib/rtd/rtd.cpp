#include <rtd.h>
#include <MCP23S17.h>
#include <SPI.h>

// Defining Various Configs:
#define DEFAULT_CONFIG 0b00000001

// Defining Registers:
#define CONFIG_REG_READ 0x00
#define CONFIG_REG_WRITE 0x80
#define RTD_MSB_REG_READ 0x01
#define RTD_LSB_REG_READ 0x02
#define HIGH_FAULT_MSB_REG_READ 0x03
#define HIGH_FAULT_MSB_REG_WRITE 0x83
#define HIGH_FAULT_LSB_REG_READ 0x04
#define HIGH_FAULT_LSB_REG_WRITE 0x84
#define LOW_FAULT_MSB_REG_READ 0x05
#define LOW_FAULT_MSB_REG_WRITE 0x85 
#define LOW_FAULT_LSB_REG_READ 0x06
#define LOW_FAULT_LSB_REG_WRITE 0x86
#define FAULT_MODE_REG 0x07

class RTD{
    public:
        RTD(uint8_t pinSet, MCP23S17* mcp){
            this->pin = pinSet;
            config = DEFAULT_CONFIG;
            this->mcp = mcp;
            spi = mcp->getSPI();
            rtdInit();
        };

        void rtdInit(){
            mcp->digitalWrite(pin,LOW);
            
        };

    private:
        uint8_t pin;
        uint8_t config;
        MCP23S17* mcp;
        SPIClass* spi;

};