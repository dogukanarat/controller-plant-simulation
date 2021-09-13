#include "main.h"
#include "network.h"

using namespace OSAL;

OS::Mutex printMutex;

Needmon::Queue g_controllerInQueue(20);
Needmon::Queue g_controllerOutQueue(20);

uint8_t g_isConnected = false;

void *client( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Packets::ControllerOut controllerOutPacket;
    Packets::PlantOut plantOutPacket;

    OS::display("[CLIENT] Controller client has been started! ");

    Needmon::Ethernet* insTcp         = new Needmon::TCP("127.0.0.1", 5001);
    Needmon::Communication* insClient = new Needmon::Client(insTcp);

    Needmon::ErrorNo errorNo = true;

    errorNo = insClient->Connect();

    if( errorNo == true )
    {
        errorNo = insClient->Process();
        
    } else {}

    uint32_t aliveCounter = 0;

    while( errorNo == true )
    {
        {
            errorNo = insClient->Read(messageBuffer);

            if( errorNo == true )
            {
                std::shared_ptr<Needmon::Frame> messageFrame(new Needmon::Frame());

                messageFrame->Parse(messageBuffer);
                messageFrame->Decode(plantOutPacket);
                
                while( false == printMutex.TryLock());
                OS::print("[CLIENT] Message is received | Message: %d\n");
                printMutex.Unlock();

                if( g_controllerInQueue.Push(messageFrame).IsFailed() == false )
                {

                } else {}

            } else {}
        }

        {
            std::shared_ptr<Needmon::Frame> messageFrame;

            if( g_controllerOutQueue.Pop(messageFrame).IsFailed() == false )
            {
                messageFrame->Encode(controllerOutPacket);
                messageFrame->Serialize(messageBuffer);

                errorNo = insClient->Write(messageBuffer);
                
                while( false == printMutex.TryLock());
                OS::print("[SERVER] Message is transmitted | Message: %d\n");
                printMutex.Unlock();
                

            } else {}
        }    
    }

    return 0;
}

void *controller( void *arg )
{
    UNUSED( arg );

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{100};

    uint32_t timestamp = 0;

    while( true )
    {
        currentStartTime = OS::Now();

        while( false == printMutex.TryLock());
        OS::print("[CONTROLLER] Timestamp : %d\n", timestamp);
        printMutex.Unlock();

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