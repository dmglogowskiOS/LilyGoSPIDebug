#include <rtd.h>
#include <MCP23S17.h>
#include <SPI.h>

RTD::RTD(uint8_t pin, PortExpander portExpander, PortExpanderBank bank, RTDWireMode mode, float nominalResistance, float referenceResistance):
    bank(bank), wireMode(mode), pin(pin), nominalResistance(nominalResistance), referenceResistance(referenceResistance), portExpander(portExpander)
{
    config = DEFAULT_CONFIG;
    spi = portExpander.getSPI();
    autoConvertEnabled = false;
}

float RTD::readTempC(){
    float Z1, Z2, Z3, Z4, rt, temp;

    auto RTDraw = static_cast<float>(readRTD());
    Serial.printf("RTD Raw: %.2f\n", RTDraw);
    rt = RTDraw / 32768;
    rt *= referenceResistance;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / nominalResistance;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
        return temp;

    // ugh.
    rt /= nominalResistance;
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
    setWires(wireMode);
    enable50hz(false);
    enableBias(true);
    autoConvert(true);
    setThresholds(0, 0xFFFF);
    clearFault();
}

uint16_t RTD::readRTD(){

    clearFault();
    
    if (!autoConvertEnabled)
    {
        enableBias(true);
        delay(12);

        config |= MAX31865_CONFIG_1SHOT;
        writeReg(MAX31865_CONFIG_REG_WRITE, config, 1);
        delay(65);
    }

    delay(65);
    uint16_t rtd = readReg16(MAX31865_RTDMSB_REG);

    if (!autoConvertEnabled)
    {
        enableBias(false);
    }
    
    rtd >>= 1;
    
    return rtd;
}

void RTD::writeReg(uint8_t regAddress, uint8_t data, uint8_t bytesToWrite){
    if (regAddress < 0b10000000)
    {
        regAddress |= 0b10000000;
    }

    portExpander.writePin(bank, pin, LOW);
    spi->transferBytes(&regAddress, NULL, 1);
    spi->transferBytes(&data, NULL, bytesToWrite);
    portExpander.writePin(bank, pin, HIGH);
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
    portExpander.writePin(bank, pin, LOW);
    spi->transferBytes(&regAddress, NULL, 1);
    spi->transferBytes(NULL ,&buffer[0], bytesToRead);
    portExpander.writePin(bank, pin, HIGH);
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
    writeReg(MAX31865_CONFIG_REG_WRITE, state, 1);
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
    writeReg(MAX31865_CONFIG_REG_WRITE, config, 1);
}

void RTD::enableBias(bool biasMode){
    if (biasMode)
    {
        config |= MAX31865_CONFIG_BIAS;
    } else {
        config &= ~MAX31865_CONFIG_BIAS;
    }
    
    writeReg(MAX31865_CONFIG_REG_WRITE, config, 1);
}

void RTD::setThresholds(uint16_t lower, uint16_t upper){
    writeReg(MAX31865_LFAULTLSB_REG, lower & 0xFF, 1);
    writeReg(MAX31865_LFAULTMSB_REG, lower >> 8, 1);
    writeReg(MAX31865_HFAULTLSB_REG, upper & 0xFF, 1);
    writeReg(MAX31865_HFAULTMSB_REG, upper >> 8, 1);
}

void RTD::autoConvert(bool conversionMode){
    autoConvertEnabled = conversionMode;
    if (conversionMode)
    {
        config |= MAX31865_CONFIG_MODEAUTO;
    } else {
        config &= ~MAX31865_CONFIG_MODEAUTO;
    }
    
    writeReg(MAX31865_CONFIG_REG_WRITE, config, 1);
}

void RTD::enable50hz(bool mode){
    if (mode)
    {
        config |= MAX31865_CONFIG_FILT50HZ;
    } else {
        config &= ~MAX31865_CONFIG_FILT50HZ;
    }
    
    writeReg(MAX31865_CONFIG_REG_WRITE, config, 1);
}

void RTD::forceConfig(u_int8_t value){
    writeReg(0x80, value, 0x1);
}
