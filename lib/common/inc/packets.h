#ifndef _COMMON_COMMUNICATION_H
#define _COMMON_COMMUNICATION_H

#include "stdint.h"
#include "messageframe.h"
#include "packet.h"
#include "consts.h"

namespace Packets
{
    typedef float  float32_t;
    typedef double float64_t;

    class Periodic : public Needmon::Packet
    {
        public:
        Periodic();
        ~Periodic();
        
        struct 
        {
            uint32_t  data1;
            uint16_t  data2;
            float32_t data3;
            float64_t data4; 
        } Data;

        void Encode(Needmon::MessageFrame &frame);
        void Decode(Needmon::MessageFrame &frame);
    };

}

#endif