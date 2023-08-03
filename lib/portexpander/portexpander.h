#ifndef PORTEXPANDER
#define PORTEXPANDER
#include <Arduino.h>
#include <MCP23S17.h>

constexpr uint8_t testPin = 12;

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
         * @brief CTOR without defining the inital Configuration of the two banks IO
         * Default Config is all INPUT
         *
         * @param spi Pointer to an SPIClass Object
         * @param clock The Clock Pin of the SPI Bus 
         * @param miso The miso Pin of the SPI Bus
         * @param mosi The mosi Pin of the SPI Bus
         * @param chipSelect The Chipselect Pin to which the Portexpander is Attached to
         * @param address The address of the PortExpander
         */
        PortExpander(SPIClass *spi, int clock, int miso, int mosi, int chipSelect, uint8_t address);

        /**
         @brief CTOR for configuring the GPIOBanks during Initialization 
         *
         * @param spi Pointer to an SPIClass Object
         * @param clock The Clock Pin of the SPI Bus 
         * @param miso The miso Pin of the SPI Bus
         * @param mosi The mosi Pin of the SPI Bus
         * @param chipSelect The Chipselect Pin to which the Portexpander is Attached to
         * @param address The address of the PortExpander
         * @param bankAConfig Configuration of the A Bank of Pins
         * @param bankBConfig Configuration of the B Bank of Pins
         */
        PortExpander(SPIClass *spi, int clock, int miso, int mosi, int chipSelect, uint8_t address, IOType bankAConfig[], IOType bankBConfig[]);

        /**
         * @brief Initialization Method for the PortExpander
         * 
         */
        void init();

        /**
         * @brief Set the Given port on a bank to a given type
         * 
         * @param bank The Bank on which the output is set
         * @param port The port that should be affected between 0 and 7
         * @param type The type of port to be set -> OUTPUT_IO, INPUT_IO or INPUT_PULLUP_IO
         */
        void setPin(PortExpanderBank bank, int8_t port, IOType type);

        /**
         * @brief Writes a given value to a given Port on a bank
         * 
         * @param bank The Bank on which the output is used
         * @param port The Port that should be affected between 0 and 7
         * @param value The value to be written either 0 or 1
         */
        void writePin(PortExpanderBank bank, int8_t port, int8_t value);

        /**
         * @brief allows to set both GPIOBanks via the given Arrays
         * 
         * @param bankAConfig Config for Bank A
         * @param bankBConfig Config for Bank B
         */
        void setIOConfigs(IOType bankAConfig[], IOType bankBConfig[]);

        /**
         * @brief Set all Output Pins to the given Value
         * 
         * @param value either HIGH or LOW as per the esp IDF definitions
         */
        void setAllOutputs(int value);

        SPIClass* getSPI();
    private:
        SPIClass *spi;
        MCP23S17 mcp;
        IOType bankA[8];
        IOType bankB[8];
        int spiValues[4];

};

#endif