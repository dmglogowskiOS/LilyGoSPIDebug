#include <rtdManager.h>

RtdManager::RtdManager(PortExpander portExpander)
: m_portEx(portExpander)
{

}

void RtdManager::init(){
    for (size_t i = 0; i < m_rtdList.size(); i++)
    {
        m_rtdList[i].rtdInit();
    }
    
}

std::vector<float> RtdManager::readAll(){
    std::vector<float> temps = std::vector<float>();
    temps.resize(m_rtdList.size());

    for (size_t i = 0; i < m_rtdList.size(); i++)
    {
        temps[i] = m_rtdList[i].readTempC(nominalResistance, referenceResistance);
    }
    
    return temps;
}

void RtdManager::setResistances(float nominalRes, float referenceRes){
    if (nominalRes <= 0)
    {
        Serial.println("Nominal Resistance cannot be 0 or less than 0");
    } else {
        nominalResistance = nominalRes;
    }

    if (referenceRes <= 0)
    {
        Serial.println("Reference Resistance cannot be 0 or less than 0");
    } else if (referenceRes < nominalResistance) {
        Serial.println("Reference Resistance has to be larger than nominal Resistance");
    } else {
        referenceResistance = referenceRes;
    }
}

void RtdManager::addRTD(RTD rtd) {
    m_rtdList.push_back(rtd);
}