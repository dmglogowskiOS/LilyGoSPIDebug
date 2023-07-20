#include <rtd.h>
#include <MCP23S17.h>
#include <SPI.h>

RTD::RTD(uint8_t pinSet, MCP23S17* mcp, GPIOBank bank, RTDWireMode mode):
    bank(bank)
{
    this->pin = pinSet;
    config = DEFAULT_CONFIG;
    this->mcp = mcp;
    spi = mcp->getSPI();
    address = pin + 64;
    if  (bank == GPIOBank::B)
    {
        pin += 8;
    }

    setWires(mode);
}

float RTD::readTempC(float RTDnominal, float refResistor){
    float Z1, Z2, Z3, Z4, rt, temp;

    auto RTDraw = readRTD();
    rt = RTDraw / 32768;
    rt *= refResistor;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / RTDnominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
        return temp;

    // ugh.
    rt /= RTDnominal;
    rt *= 100; // normalize to 100 ohm

    float rpoly = rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= rt; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= rt; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= rt; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= rt; // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
}

void RTD::rtdInit(){
    writeReg(MAX31865_CONFIG_REG_WRITE, config);
}

uint16_t RTD::readRTD(){
    clearFault();
    enableBias(true);
    delay(12);

    config |= MAX31865_CONFIG_1SHOT;
    writeReg(MAX31865_CONFIG_REG_WRITE, config);
    delay(65);
    uint16_t rtd = readReg16(MAX31865_RTDMSB_REG);

    enableBias(false);
    return rtd;
}

void RTD::writeReg(uint8_t regAddress, uint8_t data){
    if (regAddress < 0b10000000)
    {
        regAddress |= 0b10000000;
    }

    mcp->digitalWrite(pin,LOW);
    spi->transfer(regAddress);
    spi->transfer(data);
    mcp->digitalWrite(pin, HIGH);
    
}

uint8_t RTD::readReg8(uint8_t regAddress){
    uint8_t data = 0;
    readRegN(regAddress, &data, 1);
    return data;
}

uint16_t RTD::readReg16(uint8_t regAddress){
    uint8_t buffer[2] = {0, 0};
    readRegN(regAddress, &buffer[0], 2); 
    uint16_t data = buffer[0];
    data <<= 8;
    data |= buffer[1];
    
    return data;
}

void RTD::readRegN(uint8_t regAddress, uint8_t buffer[], uint8_t bytesToRead){
    
    mcp->digitalWrite(pin, LOW);
    spi->transfer(regAddress);
    spi->transfer(&buffer[0], bytesToRead);
    mcp->digitalWrite(pin, HIGH);
}

uint8_t RTD::readFault(){
    auto fault = readReg8(MAX31865_FAULTSTAT_REG);
    clearFault();
    return fault;
}

void RTD::clearFault(){
    uint8_t state = readReg8(MAX31865_CONFIG_REG_READ);
    state &= ~0x2C;
    state |= MAX31865_CONFIG_FAULTSTAT;
    writeReg(MAX31865_CONFIG_REG_WRITE, state);
}

void RTD::setWires(RTDWireMode mode){
    switch (mode)
    {
    case RTDWireMode::FOUR_WIRE:
         // Fallthrough expected : Same Behaviour
    case RTDWireMode::TWO_WIRE:
        config |= MAX31865_CONFIG_24WIRE;
        break;
    case RTDWireMode::THREE_WIRE:
        config |= MAX31865_CONFIG_3WIRE;
        break;
    }
    writeReg(MAX31865_CONFIG_REG_WRITE, config);
}

void RTD::enableBias(bool biasMode){
    if (biasMode)
    {
        config |= MAX31865_CONFIG_BIAS;
    } else {
        config &= ~MAX31865_CONFIG_BIAS;
    }
    
    writeReg(MAX31865_CONFIG_REG_WRITE, config);
}


