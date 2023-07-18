#include <rtd.h>
#include <MCP23S17.h>
#include <SPI.h>

class RTD{
    public:
        RTD(uint8_t pinSet, MCP23S17* mcp, GPIOBank bank, RTDWireMode mode){
            this->pin = pinSet;
            config = DEFAULT_CONFIG;
            this->mcp = mcp;
            spi = mcp->getSPI();

            if (bank == GPIOBank::A)
            {
                address = pin + 64;
            } else {
                address = (pin - 8) + 64;
            }

            if (mode == RTDWireMode::THREE_WIRE)
            {
                config |= MAX31865_CONFIG_3WIRE;
            } else {
                config |= MAX31865_CONFIG_24WIRE;
            }
            
            rtdInit();
        };

        void rtdInit(){
            writeReg(MAX31865_CONFIG_REG, config);
        };



    private:
        uint8_t pin;
        uint8_t config;
        uint8_t address;
        MCP23S17* mcp;
        SPIClass* spi;

        uint16_t readRTD(){
            
        };

        void writeReg(uint8_t regAddress, uint8_t data){
            if (address < 0x80)
            {
                address += 0x80;
            }
            
            mcp->digitalWrite(pin,LOW);
            spi->transfer(address);
            spi->transfer(data);
            spi->transfer(regAddress);
            mcp->digitalWrite(pin, HIGH);
        };

        uint8_t readReg(uint8_t regAddress){
            uint8_t data = 0b00000000;
            mcp->digitalWrite(pin, LOW);
            spi->transfer(address);
            spi->transfer(regAddress);
            spi->transferBytes(nullptr, &data, 1);
            mcp->digitalWrite(pin, HIGH);
            return data;
        }
};