#include <rtdManager.h>

IOType allOutPut[] = {IOType::OUTPUT_IO, IOType::OUTPUT_IO, IOType::OUTPUT_IO, IOType::OUTPUT_IO, IOType::OUTPUT_IO, IOType::OUTPUT_IO, IOType::OUTPUT_IO, IOType::OUTPUT_IO};

RtdManager::RtdManager(PortExpander *portExpander)
: m_portEx(portExpander)
{
    
}

void RtdManager::init(){
    m_portEx->init();

    for (size_t i = 0; i < 8; i++)
    {
        m_portEx->setAllOutputs(HIGH);
    }
}

std::vector<float> RtdManager::readAll(){
    std::vector<float> temps = std::vector<float>();
    temps.resize(m_rtdList.size());

    for (size_t i = 0; i < m_rtdList.size(); i++)
    {
        temps[i] = m_rtdList[i].readTempC();
    }
    
    return temps;
}

void RtdManager::addRTD(RTD rtd) {
    m_portEx->setPin(rtd.bank, rtd.pin, IOType::OUTPUT_IO);
    m_rtdList.push_back(rtd);
    m_portEx->setAllOutputs(HIGH);
    rtd.rtdInit();
}