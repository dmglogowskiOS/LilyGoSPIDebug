#include <portexpander.h>


PortExpander::PortExpander(SPIClass *spi, int clock, int miso, int mosi, int chipSelect, uint8_t address):
    mcp(spi, chipSelect, address), spi(spi)
    {
        spiValues[0] = clock;
        spiValues[1] = miso;
        spiValues[2] = mosi;
        spiValues[3] = chipSelect;

        for (size_t i = 0; i < 8; i++)
        {
            bankA[i] = IOType::INPUT_IO;
            bankB[i] = IOType::INPUT_IO;
        }
    };

PortExpander::PortExpander(SPIClass *spi, int clock, int miso, int mosi, int chipSelect, uint8_t address, IOType bankAConfig[], IOType bankBConfig[]):
mcp(spi, chipSelect, address), spi(spi)
{
    spiValues[0] = clock;
    spiValues[1] = miso;
    spiValues[2] = mosi;
    spiValues[3] = chipSelect;

    for (size_t i = 0; i < 8; i++)
    {
        bankA[i] = bankAConfig[i];
        bankB[i] = bankBConfig[i];
    }
    
};

void PortExpander::init(){
        mcp.begin(spiValues[0], spiValues[1], spiValues[2], spiValues[3]);
        for (size_t i = 0; i < 8; i++)
        {
            setPin(PortExpanderBank::A, i, bankA[i]);
            setPin(PortExpanderBank::B, i, bankB[i]);
        }
        
    };

void PortExpander::setPin(PortExpanderBank bank, int8_t port, IOType type){
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

void PortExpander::writePin(PortExpanderBank bank, int8_t port, int8_t value){
            if (bank == PortExpanderBank::A)
            {
                mcp.digitalWrite(port, value);
            } else if (bank == PortExpanderBank::B)
            {
                port += 8;
                mcp.digitalWrite(port, value);
            }
        }

void PortExpander::setAllOutputs(int value){
    for (size_t i = 0; i < 8; i++)
    {   
        if (bankA[i] == IOType::OUTPUT_IO)
        {
            writePin(PortExpanderBank::A, i, value);
        }
        if (bankB[i] == IOType::OUTPUT_IO)
        {
            writePin(PortExpanderBank::B, i, value);
        }
    }
}

void PortExpander::setIOConfigs(IOType bankAConfig[], IOType bankBConfig[]){
    for (size_t i = 0; i < 8; i++)
    {
        bankA[i] = bankAConfig[i];
        setPin(PortExpanderBank::A, i, bankA[i]);
        bankB[i] = bankBConfig[i];
        setPin(PortExpanderBank::B, i, bankB[i]);
    }
    
}

SPIClass* PortExpander::getSPI(){
    return spi;
}