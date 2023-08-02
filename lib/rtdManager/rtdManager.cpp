#include <rtdManager.h>

void rtdManager::init(){

};

std::vector<float> rtdManager::readAll() {
    std::vector<float> temps = std::vector<float>();
    temps.resize(m_rtdList.size());

    for (size_t i = 0; i < m_rtdList.size(); i++)
    {
        temps[i] = m_rtdList[i].readTempC(nominalResistance, referenceResistance);
    }
    
    return temps;
}

void rtdManager::setResistances(float nominalRes, float referenceRes){
    if (nominalRes <= 0)
    {
        Serial.println("Nominal Resistance cannot be 0 or less than 0");
    } else {
        nominalResistance = nominalRes;
    }

    if (referenceResistance <= 0)
    {
        Serial.println("Reference Resistance cannot be 0 or less than 0");
    } else if (referenceRes < nominalResistance) {
        Serial.println("Reference Resistance has to be larger than nominal Resistance");
    } else {
        referenceResistance = referenceRes;
    }
}