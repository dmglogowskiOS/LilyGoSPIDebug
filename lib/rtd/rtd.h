#ifndef RTDH
#define RTDH
#include <Arduino.h>
#include <MCP23S17.h>
#include <SPI.h>

// Defining Configs

#define MAX31865_CONFIG_REG_READ 0x00
#define MAX31865_CONFIG_REG_WRITE 0x80
#define MAX31865_CONFIG_BIAS 0x80
#define MAX31865_CONFIG_MODEAUTO 0x40
#define MAX31865_CONFIG_MODEOFF 0x00
#define MAX31865_CONFIG_1SHOT 0x20
#define MAX31865_CONFIG_3WIRE 0x10
#define MAX31865_CONFIG_24WIRE 0x00
#define MAX31865_CONFIG_FAULTSTAT 0x02
#define MAX31865_CONFIG_FILT50HZ 0x01
#define MAX31865_CONFIG_FILT60HZ 0x00
#define DEFAULT_CONFIG 0b00000001

// Defining Registers

#define MAX31865_RTDMSB_REG 0x01
#define MAX31865_RTDLSB_REG 0x02
#define MAX31865_HFAULTMSB_REG 0x03
#define MAX31865_HFAULTLSB_REG 0x04
#define MAX31865_LFAULTMSB_REG 0x05
#define MAX31865_LFAULTLSB_REG 0x06
#define MAX31865_FAULTSTAT_REG 0x07

#define MAX31865_FAULT_HIGHTHRESH 0x80
#define MAX31865_FAULT_LOWTHRESH 0x40
#define MAX31865_FAULT_REFINLOW 0x20
#define MAX31865_FAULT_REFINHIGH 0x10
#define MAX31865_FAULT_RTDINLOW 0x08
#define MAX31865_FAULT_OVUV 0x04

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

/**
 * @brief Represents which GPIO Bank the chip is connected to on the MCP
 * 
 */
enum class GPIOBank{
    A,
    B,
    NONE,
};

/**
 * @brief Represents the Wire Mode for the RTD
 * 
 */
enum class RTDWireMode{
    TWO_WIRE,
    THREE_WIRE,
    FOUR_WIRE
};

/**
 * @brief Models an RTD interfaced with via an MCP23S17
 * 
 */
class RTD{
    public:
        /**
         * @brief Create a new RTD object
         * 
         * @param pinSet Sets the pin used on the MCP23S17
         * @param mcp Pointer to an MCP23S17 Object
         * @param bank Memory Bank, be aware, that you can only have 8 RTDs at once as they only have 3 Address Bits
         * @param mode Wire Mode for configuration
         */
        RTD(u_int8_t pinSet, MCP23S17* mcp, GPIOBank bank, RTDWireMode mode);

        /**
         * @brief Transmits default config to the attached RTD
         * 
         */
        void rtdInit();

        /**
         * @brief 
         * 
         * @param RTDnominal Nominal Resistance of the RTD => Usually 100 or 1000 Ohm
         * @param refResistor Reference Resistance => Usually 430 or 4300 Ohm
         * @return float 
         */
        float readTempC(float RTDnominal, float refResistor);

        /**
         * @brief Readout of the Fault Register
         * 
         * @return uint8_t 
         */
        uint8_t readFault();
        
        /**
         * @brief Clears the fault register
         * 
         */
        void clearFault();

        /**
         * @brief Allows to set the WireMode dynamically
         * 
         * @param mode RTDWireMode Enum Member -> See RTDWireMode enum class
         */
        void setWires(RTDWireMode mode);

    private:
        uint8_t pin;
        uint8_t config;
        uint8_t address;
        MCP23S17* mcp;
        SPIClass* spi;
        GPIOBank bank;

        /**
         * @brief Private Method to read the RTD Raw Value
         * 
         * @return uint16_t - RAW TEMP Value as an unsigned 16bit INT => NOT TEMP
         */
        uint16_t readRTD();

        /**
         * @brief Private Method to write Data to RTD Registers
         * 
         * @param regAddress Address of the Register to write to
         * @param data Data to write at that Register
         */
        void writeReg(uint8_t regAddress, uint8_t data);

        /**
         * @brief Reads the given 8bit Register
         * 
         * @param regAddress Address of the Register
         * @return uint8_t Value of the Register
         */
        uint8_t readReg8(uint8_t regAddress);

        /**
         * @brief Reads 16 Bits from the given Register 
         * 
         * @param regAddress Register the Read is initialized on, this will overrun if the given Register is only 8 Bits long,
         * this behaviour is wanted. 
         * @return uint16_t 
         */
        uint16_t readReg16(uint8_t regAddress);

        /**
         * @brief Reads the Given Number of bytes into the buffer starting at the given Register
         * 
         * @param regAddress Starting Register 
         * @param buffer Buffer to read into
         * @param bytesToRead The number of Bytes to Read -> multiply by 8 to get the number of Bits you are reading.
         */
        void readRegN(uint8_t regAddress, uint8_t buffer[], uint8_t bytesToRead);
        
        /**
         * @brief Private Method to enable the Bias of the system
         * 
         * @param biasMode true to Enable the bias, false to disable.
         */
        void enableBias(bool biasMode);
};


#endif