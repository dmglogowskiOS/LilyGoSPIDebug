#include <portexpander.h>


PortExpander::PortExpander(SPIClass *spi, int clock, int miso, int mosi, int chipSelect, uint8_t address):
    mcp(spi, chipSelect, address), spi(spi)
    {
        spiValues[0] = clock;
        spiValues[1] = miso;
        spiValues[2] = mosi;
        spiValues[3] = chipSelect;
    };

void PortExpander::initPortExpander(){
        mcp.begin(spiValues[0], spiValues[1], spiValues[2], spiValues[3]);
    };

void PortExpander::setBank(PortExpanderBank bank, IOType type){
        auto iotype = 0x00;
        switch (type)
        {
        case IOType::OUTPUT_IO:
            iotype = 0x03;
            break;
        case IOType::INPUT_IO:
            iotype = 0x01;
            break;
        
        case IOType::INPUT_PULLUP_IO:
            iotype = 0x05;
            break;
        default:
            Serial.println("Error : NO IOType GIVEN ");
            return;
        }
        if (bank == PortExpanderBank::A)
        {
            for (size_t i = 0; i < 8; i++)
            {   
                bankA[i] = type;
                mcp.pinMode(i, iotype);
            }
        } else if (bank == PortExpanderBank::B)
        {
            for (size_t i = 8; i < 16; i++)
            {   
                bankB[i-8] = type;
                mcp.pinMode(i, iotype);
            }
            
        }
    };

void PortExpander::setPort(PortExpanderBank bank, int8_t port, IOType type){
            auto iotype = 0x00;
            switch (type)
            {
            case IOType::OUTPUT_IO:
                iotype = 0x03;
                break;

            case IOType::INPUT_IO:
                iotype = 0x01;
                break;
            
            case IOType::INPUT_PULLUP_IO:
                iotype = 0x05;
                break;
            default:
                Serial.println("Error : NO IOType GIVEN ");
                return;
            }

            if (bank == PortExpanderBank::A)
            {
                bankA[port] = type;
                mcp.pinMode(port, iotype);
            } else if (bank == PortExpanderBank::B)
            {
                bankB[port] = type;
                port += 8;
                mcp.pinMode(port, iotype);
            }
            
            
        };

void PortExpander::writePort(PortExpanderBank bank, int8_t port, int8_t value){
            if (bank == PortExpanderBank::A)
            {
                mcp.digitalWrite(port, value);
            } else if (bank == PortExpanderBank::B)
            {
                port += 8;
                mcp.digitalWrite(port, value);
            }
        }

MCP23S17* PortExpander::getMcp(){
    return &mcp;
};
