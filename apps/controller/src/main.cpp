#include "main.h"
#include "network.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define RECEIVE_PORT_NO 5002
#define TRANSMIT_PORT_NO 5001
#define SOCKET_DELAY_US 100000

Needmon::Queue g_controllerInQueue(QUEUE_SIZE);
Needmon::Queue g_controllerOutQueue(QUEUE_SIZE);

OS::Mutex printMutex;
OS::Mutex controllerInQueueMutex;
OS::Mutex controllerOutQueueMutex;

void *client( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer         messageBuffer;
    Packets::ControllerOut  controllerOutPacket;
    Packets::PlantOut       plantOutPacket;
    Needmon::ErrorNo        errorNo = true;
    uint32_t                aliveCounter = 0;
    Needmon::Ethernet*      insTransmitTcp = nullptr;
    Needmon::Communication* insClient = nullptr;

    OS::display("[CLIENT] Controller client has been started! ");

    insTransmitTcp = new Needmon::TCP("127.0.0.1", TRANSMIT_PORT_NO);
    insClient      = new Needmon::Client(insTransmitTcp);

    do
    {
        errorNo = insClient->Connect();
    } while( errorNo == false );

    OS::print("[CLIENT] Connected\n");
    
    errorNo = insClient->Process();

    while( errorNo == true )
    {
        std::shared_ptr<Needmon::Frame> messageFrame;

        controllerOutQueueMutex.Lock();
        uint8_t queueResult = g_controllerOutQueue.Pop(messageFrame).IsFailed();
        controllerOutQueueMutex.Unlock();

        if( queueResult == false )
        {
            messageFrame->Encode(plantOutPacket);
            messageFrame->Serialize(messageBuffer);

            errorNo = insClient->Write(messageBuffer);

            printMutex.Lock();
            OS::print("[CLIENT] Message is transmitted | Message: %d\n", aliveCounter);
            printMutex.Unlock();
        } else {}
        
        OS::waitUs(SOCKET_DELAY_US);
    }

    return 0;
}

void *server( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer         messageBuffer;
    Packets::ControllerOut  controllerOutPacket;
    Packets::PlantOut       plantOutPacket;
    Needmon::ErrorNo        errorNo = true;
    uint32_t                aliveCounter = 0;
    Needmon::Ethernet*      insReceiveTcp;
    Needmon::Communication* insServer;


    OS::display("[SERVER] Controller server has been started! ");

    insReceiveTcp  = new Needmon::TCP("localhost", RECEIVE_PORT_NO);
    insServer      = new Needmon::Server(insReceiveTcp);

    do
    {
        errorNo = insServer->Connect();
    } while( errorNo == false);

    errorNo = insServer->Process();
    
    OS::display("[SERVER] Controller server has been connected to the client! ");

    while( errorNo == true )
    {
        errorNo = insServer->Read(messageBuffer);

        if( errorNo == true )
        {
            std::shared_ptr<Needmon::Frame> messageFrame(new Needmon::Frame());

            messageFrame->Parse(messageBuffer);
            messageFrame->Decode(controllerOutPacket);

            printMutex.Lock();
            OS::print("[SERVER] Message is received | Message: %d\n", aliveCounter);
            printMutex.Unlock();

            controllerInQueueMutex.Lock();
            uint8_t queueResult = g_controllerInQueue.Push(messageFrame).IsFailed();
            controllerInQueueMutex.Unlock();

            if( queueResult == false )
            {

            } else {}

        } else {}
        
        OS::waitUs(SOCKET_DELAY_US);

    }
    
    return 0;
}

void *controller( void *arg )
{
    UNUSED( arg );

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{1000};

    uint32_t timestamp = 0;

    while( true )
    {
        currentStartTime = OS::Now();

        // printMutex.Lock()
        // OS::print("[CONTROLLER] Timestamp : %d\n", timestamp);
        // printMutex.Unlock();

        std::shared_ptr<Needmon::Frame> plantOutMessage(new Needmon::Frame());

        if( g_controllerInQueue.Pop(plantOutMessage).IsFailed() == false )
        {
            while( false == printMutex.TryLock());
            OS::print("[CONTROLLER] Packet is received! \n", timestamp);
            printMutex.Unlock();
        } else {}

        timestamp++;

        nextStartTime = currentStartTime + intervalMillis;
        OS::SleepUntil( nextStartTime );
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
    OS::thread_t thread_server;
    OS::thread_t thread_controller;
    

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create( &thread_client, client );
    OS::create( &thread_server, server );
    OS::create( &thread_controller, controller );
    

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_client );
    OS::join( thread_server );
    OS::join( thread_controller );
  
    return 0;
}