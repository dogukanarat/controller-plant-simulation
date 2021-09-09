#include "main.h"
#include "network.h"

using namespace OSAL;

void *client( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;

    OS::display("[CLIENT] Controller client has been started! ");

    Needmon::Ethernet* insTcp         = new Needmon::TCP("127.0.0.1", 5001);
    Needmon::Communication* insClient = new Needmon::Client(insTcp);

    Needmon::ErrorNo errorNo = true;

    errorNo = insClient->Connect();

    uint16_t counter = 0;

    while( errorNo > 0 )
    {

        // messageFrame.Encode( periodicPacket );
        // messageFrame.Serialize( messageBuffer );

        errorNo = insClient->Write(messageBuffer);

        // OS::print("[CLIENT] Message is sent | Message : %d \n", periodicPacket.Data.data1);

        OS::waitUs(500000);
    }

    return 0;
}

void *controller( void *arg )
{
    UNUSED( arg );

    uint32_t aliveCounter = 0;

    while( true )
    {
        OS::print("[CONTROLLER] Another Thread Alive Counter : %d\n", aliveCounter++);
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