#ifndef PORTEXPANDER
#define PORTEXPANDER
#include <Arduino.h>
#include <MCP23S17.h>

enum class IOType{
    OUTPUT_IO = 0x03,
    INPUT_IO = 0x01,
    INPUT_PULLUP_IO = 0x05,
};

enum class PortExpanderBank{
    A,
    B
};

class PortExpander{
    public:
        /**
         * @brief Default CTOR
         *
         * @param spi Pointer to an SPIClass Object
         * @param clock The Clock Pin of the SPI Bus 
         * @param miso The miso Pin of the SPI Bus
         * @param mosi The mosi Pin of the SPI Bus
         * @param chipSelect The Chipselect Pin to which the Portexpander is Attached to
         */
        PortExpander(SPIClass *spi, int clock, int miso, int mosi, int chipSelect, uint8_t address);

        /**
         * @brief Initialization Method for the PortExpander
         * 
         */
        void initPortExpander();
        
        /**
         * @brief Sets the Given Bank ENTIRELY to the Given Type of Output
         * 
         * @param bank The Bank to be set
         * @param type The type of port to be set -> OUTPUT_IO, INPUT_IO or INPUT_PULLUP_IO
         */
        void setBank(PortExpanderBank bank, IOType type);

        /**
         * @brief Set the Given port on a bank to a given type
         * 
         * @param bank The Bank on which the output is set
         * @param port The port that should be affected between 0 and 7
         * @param type The type of port to be set -> OUTPUT_IO, INPUT_IO or INPUT_PULLUP_IO
         */
        void setPort(PortExpanderBank bank, int8_t port, IOType type);

        /**
         * @brief Writes a given value to a given Port on a bank
         * 
         * @param bank The Bank on which the output is used
         * @param port The Port that should be affected between 0 and 7
         * @param value The value to be written either 0 or 1
         */
        void writePort(PortExpanderBank bank, int8_t port, int8_t value);

    private:
        SPIClass *spi;
        MCP23S17 mcp;
        IOType bankA[8];
        IOType bankB[8];
        int spiValues[4];
};

#endif