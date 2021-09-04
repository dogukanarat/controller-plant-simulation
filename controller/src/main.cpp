#include "main.h"
#include "network.h"

using namespace OSAL;

void receiveHandler(uint8_t* buffer, uint32_t size)
{
    OS::print("[HANDLER] Data size: %d\n", size);
    OS::print("[HANDLER] Data 1: 0x%X\n", buffer[0]);
}

void *server( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;
    Packets::Periodic periodicPacket;

    OS::display("[SERVER] Controller server has been started! ");

    Needmon::Ethernet* insTcp         = new Needmon::TCP("127.0.0.1", 5001);
    Needmon::Communication* insServer = new Needmon::Server(insTcp);

    insServer->SetHandler(receiveHandler);

    insServer->Connect();

    insServer->Process();
    
    return 0;
}

void *client( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;
    Packets::Periodic periodicPacket;

    periodicPacket.Data.data1 = 0x23;
    periodicPacket.Encode( messageFrame );
    messageFrame.Serialize( messageBuffer );

    OS::waitUs(5000000);

    OS::display("[CLIENT] Controller client has been started! ");

    Needmon::Ethernet* insTcp         = new Needmon::TCP("127.0.0.1", 5001);
    Needmon::Communication* insClient = new Needmon::Client(insTcp);

    //insClient->SetHandler(receiveHandler);

    insClient->Connect();

    insClient->Process();

    return 0;
}

int main()
{
    OS::display("[PROCESS] Controller has been started!");

    /**
     * @brief 
     * 
     */
    OS::thread_t thread_server;
    OS::thread_t thread_client;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create( &thread_server, server );
    OS::create( &thread_client, client );

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_server );
    OS::join( thread_client );
  
    return 0;
}