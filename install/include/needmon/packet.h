#ifndef _NEEDMON_PACKET_H
#define _NEEDMON_PACKET_H

#include "payload.h"

namespace Needmon
{
    class Packet
    {
        public:
        Packet() {};
        ~Packet() {};

        virtual void Encode(Payload& payload) {};
        virtual void Decode(Payload& payload) {};
    };
}

#endif