#ifndef _COMMUNICATION_HPP
#define _COMMUNICATION_HPP

#include "stdint.h"
#include "messageframe.h"

namespace Communication
{
    class Periodic
    {
        public:
        Periodic() { };
        ~Periodic();
        
        struct 
        {
            uint32_t data1;
            uint16_t data2;
            float data3;
            double data4; 
        } Data;

        void Serialize(Needmon::MessageFrame &frame);
        void Parse(Needmon::MessageFrame &frame);
    };

}

#endif