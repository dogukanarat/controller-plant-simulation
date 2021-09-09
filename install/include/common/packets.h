#ifndef _COMMON_COMMUNICATION_H
#define _COMMON_COMMUNICATION_H

#include "stdint.h"
#include "frame.h"
#include "packet.h"
#include "consts.h"

namespace Packets
{
    typedef float  float32_t;
    typedef double float64_t;

    class PlantOut : public Needmon::Packet
    {
        public:
        PlantOut() {};
        ~PlantOut() {};

        float32_t noisySignal;

        virtual void Encode(Needmon::Payload& payload);
        virtual void Decode(Needmon::Payload& payload);
    };

    class ControllerOut : public Needmon::Packet
    {
        public:
        ControllerOut() {};
        ~ControllerOut() {};
        
        
        float32_t filteredSignal;
        

        virtual void Encode(Needmon::Payload& payload);
        virtual void Decode(Needmon::Payload& payload);
    };


}

#endif