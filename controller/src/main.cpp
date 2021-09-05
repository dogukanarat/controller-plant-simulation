#include "main.h"
#include "network.h"

using namespace OSAL;

void *client( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;
    Packets::Periodic periodicPacket;

    OS::waitUs(5000000);

    OS::display("[CLIENT] Controller client has been started! ");

    Needmon::Ethernet* insTcp         = new Needmon::TCP("127.0.0.1", 5001);
    OS::print("[CLIENT] Creating client object...\n");
    Needmon::Communication* insClient = new Needmon::Client(insTcp);

    Needmon::ErrorNo errorNo = true;

    errorNo = insClient->Connect();

    uint16_t counter = 0;

    while( errorNo > 0 )
    {
        OS::display("[CLIENT] Data is being preparing!");

        periodicPacket.Data.data1 = 0x23;
        OS::display("[CLIENT] Data is prepared!");

        periodicPacket.Encode( messageFrame );
        OS::display("[CLIENT] Message is encoded!");

        if( counter == 14 )
        {
            counter = 14;
        }

        messageFrame.Serialize( messageBuffer );
        OS::display("[CLIENT] Message is serialized!");

        errorNo = insClient->Write(messageBuffer);
        OS::print("[CLIENT] Message is sent -> Message No: %d \n", counter++);



        OS::waitUs(500000);
    }

    return 0;
}

void *controller( void *arg )
{
    UNUSED( arg );

    while( true )
    {
        //OS::display("[CONTROLLER] Controller thread is worked!");
        OS::waitUs(500000);
    }

    return 0;
}

int main()
{
    OS::display("[PROCESS] Controller has been started!");

    /**
     * @brief 
     * 
     */
    OS::thread_t thread_client;
    OS::thread_t thread_controller;
    

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create( &thread_client, client );
    OS::create( &thread_controller, controller );
    

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_client );
    OS::join( thread_controller );
  
    return 0;
}