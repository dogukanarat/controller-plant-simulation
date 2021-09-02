#include "packets.h"

using namespace Packets;

Periodic::Periodic()
{

}

Periodic::~Periodic()
{
    
}

void Periodic::Decode(Needmon::MessageFrame &frame)
{
    frame.Write( Data.data1 );
    frame.Write( Data.data2 );
    frame.Write( Data.data3 );
    frame.Write( Data.data4 );
}

void Periodic::Encode(Needmon::MessageFrame &frame)
{
    frame.Read( Data.data1 );
    frame.Read( Data.data2 );
    frame.Read( Data.data3 );
    frame.Read( Data.data4 );
}