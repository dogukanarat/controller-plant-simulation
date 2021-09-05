#include "packets.h"

using namespace Packets;

Periodic::Periodic()
{

}

Periodic::~Periodic()
{
    
}

void Periodic::Encode(Needmon::Frame &frame)
{
    frame.EncodeReset();
    
    frame.Write( Data.data1 );
    frame.Write( Data.data2 );
    frame.Write( Data.data3 );
    frame.Write( Data.data4 );
}

void Periodic::Decode(Needmon::Frame &frame)
{
    frame.DecodeReset();

    frame.Read( Data.data1 );
    frame.Read( Data.data2 );
    frame.Read( Data.data3 );
    frame.Read( Data.data4 );
}