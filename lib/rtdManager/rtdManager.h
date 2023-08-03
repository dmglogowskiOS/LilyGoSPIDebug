#ifndef RTDMANAGERH
#define RTDMANAGERH
#include <rtd.h>
#include <portexpander.h>
#include <vector>

/**
 * @brief Class encapsulating multiple RTDs
 */
class RtdManager{
    public:
        /**
         * @brief Default Constructor for RtdManager
         * 
         * @param portExpander Pointer to a PortExpander Object
         */
        RtdManager(PortExpander *portExpander);

        /**
         * @brief initializes both the PortExpander and all attached RTDs correctly
         */
        void init();

        /**
         * @brief Reads all attached RTDs
         * 
         * @return std::vector<float> of all read Temperatures in °C
         */
        std::vector<float> readAll();

        /**
         * @brief 
         * 
         * @param rtd 
         */
        void addRTD(RTD rtd);

        std::vector<RTD> m_rtdList;
        PortExpander *m_portEx;
};

#endif
