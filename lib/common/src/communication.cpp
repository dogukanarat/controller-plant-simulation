#include "communication.h"

using namespace Communication;

void Periodic::Serialize(Needmon::MessageFrame &frame)
{
    frame.Write( Data.data1 );
    frame.Write( Data.data2 );
    frame.Write( Data.data3 );
    frame.Write( Data.data4 );
}

void Periodic::Parse(Needmon::MessageFrame &frame)
{
    frame.Read( Data.data1 );
    frame.Read( Data.data2 );
    frame.Read( Data.data3 );
    frame.Read( Data.data4 );
}