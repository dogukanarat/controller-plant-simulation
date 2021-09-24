#ifndef _COMMON_COMMUNICATION_H
#define _COMMON_COMMUNICATION_H

#include <stdint.h>
#include <message.h>
#include <template.h>
#include <consts.h>
#include <types.h>

namespace Messages
{
    class PlantOut : public Needmon::Template
    {
    public:
        PlantOut(){};
        ~PlantOut(){};

        Float32 noisySignal;

        virtual void Encode(Needmon::Payload &payload);
        virtual void Decode(Needmon::Payload &payload);
    };

    class ControllerOut : public Needmon::Template
    {
    public:
        ControllerOut(){};
        ~ControllerOut(){};

        Float32 filteredSignal;

        virtual void Encode(Needmon::Payload &payload);
        virtual void Decode(Needmon::Payload &payload);
    };

}

#endif
