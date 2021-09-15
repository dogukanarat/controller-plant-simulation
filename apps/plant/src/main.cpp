#include "main.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define RECEIVE_PORT_NO 5001
#define TRANSMIT_PORT_NO 5002
#define SOCKET_DELAY_US 100000

Needmon::Queue g_plantInQueue(QUEUE_SIZE);
Needmon::Queue g_plantOutQueue(QUEUE_SIZE);

OS::Mutex printMutex;
OS::Mutex plantInQueueMutex;
OS::Mutex plantOutQueueMutex;

uint8_t IsConnected = false;

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

    OS::display("[CLIENT] Plant client has been started! ");

    insTransmitTcp = new Needmon::TCP("127.0.0.1", TRANSMIT_PORT_NO);
    insClient      = new Needmon::Client(insTransmitTcp);

    do
    {
        errorNo = insClient->Connect();
    } while( errorNo == false );

    OS::print("[CLIENT] Connected\n");

    IsConnected = true;
    
    errorNo = insClient->Process();

    while( errorNo == true )
    {
        std::shared_ptr<Needmon::Frame> messageFrame;

        plantOutQueueMutex.Lock();
        uint8_t queueResult = g_plantOutQueue.Pop(messageFrame).IsFailed();
        plantOutQueueMutex.Unlock();

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

    OS::display("[SERVER] Plant server has been started! ");

    insReceiveTcp  = new Needmon::TCP("localhost", RECEIVE_PORT_NO);
    insServer      = new Needmon::Server(insReceiveTcp);

    do
    {
        errorNo = insServer->Connect();
    } while( errorNo == false);

    errorNo = insServer->Process();

    OS::display("[SERVER] Plant server has been connected to the client! ");

    while( errorNo == true )
    {
        errorNo = insServer->Read(messageBuffer);

        if( errorNo == true )
        {
            std::shared_ptr<Needmon::Frame> messageFrame(new Needmon::Frame());

            messageFrame->Parse(messageBuffer);
            messageFrame->Decode(controllerOutPacket);

            printMutex.Lock();
            OS::print("[SERVER] Message is transmitted | Message: %d\n", aliveCounter);
            printMutex.Unlock();

            if( g_plantInQueue.Push(messageFrame).IsFailed() == false )
            {

            } else {}

        } else {}
        
        OS::waitUs(SOCKET_DELAY_US);

    }

    return 0;
}

void *plant( void *arg )
{
    UNUSED( arg );

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{1000};

    uint32_t timestamp = 0;
    
    Control::GaussianDistribution noiseGenerator(0, 1.0f);

    Packets::PlantOut plantOutPacket;
    Packets::ControllerOut controllerOutPacket;

    Control::Decimal noisyValue = 0.0f;

    int returnMutex = 0;
    
    while( IsConnected )
    {
        currentStartTime = OS::Now();

        Control::Decimal cosOut = 5.0f * cos( ( 2.0f * M_PI / 180.0f)* (timestamp * 5) );
        noisyValue = noiseGenerator.Random(static_cast<Control::Decimal>(cosOut));
        plantOutPacket.noisySignal = static_cast<float>(noisyValue);

        std::shared_ptr<Needmon::Frame> plantOutMessage(new Needmon::Frame());

        plantOutMessage->Encode(controllerOutPacket);

        plantOutQueueMutex.Lock();
        uint8_t queueResult = g_plantOutQueue.Push(plantOutMessage).IsFailed();
        plantOutQueueMutex.Unlock();

        if( queueResult == true )
        {
            // printMutex.Lock();
            // OS::print("[PLANT] Queue is full\n");
            // printMutex.Unlock();
        }
        else
        {
            // printMutex.Lock();
            // OS::print("[PLANT] Queue size is %d\n", g_plantOutQueue.Length());
            // printMutex.Unlock();
        }
        

        printMutex.Lock();
        OS::print("[PLANT] Timestamp: %d \t Clear Signal: %.2f \t Noisy Value: %.2f \t\n", timestamp, cosOut, noisyValue );
        printMutex.Unlock();

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
    OS::thread_t thread_server;
    OS::thread_t thread_client;
    OS::thread_t thread_plant;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create( &thread_server, server );
    OS::create( &thread_client, client );
    OS::create( &thread_plant, plant );

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_server );
    OS::join( thread_client );
    OS::join( thread_plant );
  
    return 0;
}