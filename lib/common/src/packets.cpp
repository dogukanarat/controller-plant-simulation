#include "packets.h"

using namespace Packets;

void PlantOut::Encode(Needmon::Payload& payload)
{
    payload.EncodeReset();
    
    payload.Write( noisySignal );
}

void PlantOut::Decode(Needmon::Payload& payload)
{
    payload.EncodeReset();

    payload.Read( noisySignal );
}

void ControllerOut::Encode(Needmon::Payload& payload)
{
    payload.EncodeReset();
    
    payload.Write( filteredSignal );
}

void ControllerOut::Decode(Needmon::Payload& payload)
{
    payload.EncodeReset();

    payload.Read( filteredSignal );
}